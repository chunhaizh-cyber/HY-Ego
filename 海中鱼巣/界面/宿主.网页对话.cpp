#include <fstream>
#include "宿主.网页对话.h"
#include "../核心/容错检查.h"

#ifndef _WIN32
#error 本地网页对话宿主当前只实现Windows系统浏览器边界。
#endif

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include <shellapi.h>
#include <bcrypt.h>

#include <algorithm>
#include <array>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <limits>
#include <mutex>
#include <new>
#include <optional>
#include <stdexcept>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace 海中鱼巣 {
namespace {

using 网页对话消息接收函数 = std::function<bool(
    std::string_view, std::string_view, std::uint64_t, std::string_view)>;
struct 网页对话框事实回调组 final {
    std::function<bool(const 网页对话框目标&)> 确认页面;
    std::function<void(const 网页定向发送事实&)> 发送完成;
};

constexpr std::size_t 最大请求头字节数 = 64U * 1024U;
constexpr std::size_t 最大消息字节数 = 1024U * 1024U;

class 网络运行租约 final {
public:
    网络运行租约() = default;

    bool 初始化() noexcept {
        WSADATA 数据{};
        已初始化_ = WSAStartup(MAKEWORD(2, 2), &数据) == 0;
        return 已初始化_;
    }

    ~网络运行租约() {
        if (已初始化_) WSACleanup();
    }

    网络运行租约(const 网络运行租约&) = delete;
    网络运行租约& operator=(const 网络运行租约&) = delete;

private:
    bool 已初始化_ = false;
};

class 套接字租约 final {
public:
    套接字租约() = default;
    explicit 套接字租约(SOCKET 套接字) noexcept : 套接字_(套接字) {}
    ~套接字租约() { 关闭(); }

    套接字租约(const 套接字租约&) = delete;
    套接字租约& operator=(const 套接字租约&) = delete;

    套接字租约(套接字租约&& 其它) noexcept
        : 套接字_(std::exchange(其它.套接字_, INVALID_SOCKET)) {}

    套接字租约& operator=(套接字租约&& 其它) noexcept {
        if (this != &其它) {
            关闭();
            套接字_ = std::exchange(其它.套接字_, INVALID_SOCKET);
        }
        return *this;
    }

    SOCKET 取得() const noexcept { return 套接字_; }
    bool 有效() const noexcept { return 套接字_ != INVALID_SOCKET; }

private:
    void 关闭() noexcept {
        if (套接字_ != INVALID_SOCKET) {
            closesocket(套接字_);
            套接字_ = INVALID_SOCKET;
        }
    }

    SOCKET 套接字_ = INVALID_SOCKET;
};

struct HTTP请求 final {
    std::string 方法;
    std::string 目标;
    std::string 正文;
    std::string 内容类型;
    std::optional<std::string> 对话框标识;
    std::optional<std::string> 主机, 来源, 控制输出标记;
    bool 控制安全头无效 = false;
    bool 有正文长度 = false;
};

struct 网页待发送项 final {
    网页对话框目标 目标;
    std::uint64_t 提交序号 = 0, 正式消息编码 = 0;
    网页定向发送结果 结果;
};
struct 网页对话框传输状态 final {
    网页对话框目标 目标;
    std::vector<网页对话已显示消息> 消息组;
    std::vector<网页待发送项> 提交组;
    std::uint64_t 下一页面序号 = 1;
    std::optional<std::uint64_t> 出站占用身份;
};
struct 网页对话共享状态 final {
    std::mutex 互斥;
    bool 运行中 = false;
    bool 接纳提交 = false;
    // 只在运行租约内借用；所有读取、清空都持本锁，不向调用者交付引用。
    const volatile std::sig_atomic_t* 停止信号 = nullptr;
    std::string 会话标识;
    std::vector<网页对话框传输状态> 页面组;
    std::uint64_t 下一上行序号 = 1;
    const 网页输出队列回调组* 输出队列 = nullptr;
};

网页对话共享状态 共享状态;
std::mutex 队列操作互斥;

// 调用者持队列操作锁；外部事实回调期间绝不持宿主共享锁。
void 停止接纳提交_已持队列锁() {
    std::vector<网页对话框目标> 目标组;
    const 网页输出队列回调组* 回调 = nullptr;
    {
        std::lock_guard 锁(共享状态.互斥);
        目标组.reserve(共享状态.页面组.size());
        for (const auto& 页面 : 共享状态.页面组) 目标组.push_back(页面.目标);
        回调 = 共享状态.输出队列;
        共享状态.接纳提交 = false;
    }
    if (!目标组.empty() && (!回调 || !回调->清空))
        throw std::runtime_error("outbound queue shutdown provider missing");
    for (const auto& 目标 : 目标组) {
        const auto 结果 = 回调->清空(目标);
        if (结果.状态 != 网页输出队列状态::已清空 && 结果.状态 != 网页输出队列状态::空)
            throw std::runtime_error("outbound queue shutdown incomplete");
    }
    std::lock_guard 锁(共享状态.互斥);
    for (auto& 页面 : 共享状态.页面组) {
        页面.出站占用身份.reset();
        for (auto& 项 : 页面.提交组)
            if (项.结果.状态 == 网页定向发送状态::已排队) 项.结果.状态 = 网页定向发送状态::已取消;
    }
}
void 停止接纳提交() {
    std::lock_guard 操作锁(队列操作互斥);
    停止接纳提交_已持队列锁();
}

class 网页对话会话租约 final {
public:
    网页对话会话租约() = default;

    bool 开始(std::string_view 会话标识, const volatile std::sig_atomic_t& 停止请求,
        const 网页输出队列回调组& 输出队列) noexcept {
        try {
            std::lock_guard 操作锁(队列操作互斥);
            std::lock_guard 锁(共享状态.互斥);
            if (共享状态.运行中 || 会话标识.empty()) return false;
            共享状态.会话标识.assign(会话标识);
            共享状态.页面组.clear();
            共享状态.下一上行序号 = 1;
            共享状态.停止信号 = &停止请求;
            共享状态.输出队列 = &输出队列;
            共享状态.接纳提交 = 停止请求 == 0;
            共享状态.运行中 = true;
            已开始_ = true;
            return true;
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"开始网页会话", L"传输状态初始化失败。");
        }
    }

    ~网页对话会话租约() {
        if (!已开始_) return;
        try {
            std::lock_guard 操作锁(队列操作互斥);
            停止接纳提交_已持队列锁();
            std::lock_guard 锁(共享状态.互斥);
            共享状态.停止信号 = nullptr;
            共享状态.运行中 = false;
            for (auto& 页面 : 共享状态.页面组) {
                for (auto& 项 : 页面.提交组) {
                    if (项.结果.状态 == 网页定向发送状态::已排队)
                        项.结果.状态 = 网页定向发送状态::已取消;
                    if (项.结果.状态 == 网页定向发送状态::正在写出)
                        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"结束网页会话", L"真实写出尚未完成事实收口。");
                }
            }
            共享状态.会话标识.clear();
            共享状态.输出队列 = nullptr;
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"结束网页会话", L"传输状态取消失败。");
        }
    }

    网页对话会话租约(const 网页对话会话租约&) = delete;
    网页对话会话租约& operator=(const 网页对话会话租约&) = delete;

private:
    bool 已开始_ = false;
};

class 网页外设会话租约 final {
public:
    网页外设会话租约(std::string_view 标识, const 网页外设运行回调组& 回调)
        : 标识_(标识), 回调_(回调) {}
    bool 开始() { 已开始_ = 回调_.会话开始(标识_); return 已开始_; }
    ~网页外设会话租约() {
        if (!已开始_) return;
        try { 回调_.会话结束(标识_); }
        catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"结束网页外设会话",
                L"外设会话回收工程异常，不把它作为业务状态继续运行。");
        }
    }
    网页外设会话租约(const 网页外设会话租约&) = delete;
    网页外设会话租约& operator=(const 网页外设会话租约&) = delete;
private:
    std::string_view 标识_;
    const 网页外设运行回调组& 回调_;
    bool 已开始_ = false;
};

// 标识的存储由外层宿主持有，先于本租约建立、晚于本租约析构。
class 网页服务会话租约 final {
public:
    网页服务会话租约(std::string_view 公开标识,
        const 网页服务输入回调组& 回调) noexcept
        : 公开标识_(公开标识), 回调_(回调) {}

    bool 开始() noexcept {
        if (!回调_.会话开始) return true;
        const auto 状态 = 回调_.会话开始(公开标识_);
        已开始_ = 状态 == 网页服务输入状态::已完成
            || 状态 == 网页服务输入状态::匿名会话;
        return 已开始_;
    }

    ~网页服务会话租约() {
        if (已开始_) 回调_.会话结束(公开标识_);
    }

    网页服务会话租约(const 网页服务会话租约&) = delete;
    网页服务会话租约& operator=(const 网页服务会话租约&) = delete;

private:
    std::string_view 公开标识_;
    const 网页服务输入回调组& 回调_;
    bool 已开始_ = false;
};

enum class 请求读取状态 : std::uint8_t {
    已读取 = 0,
    连接结束,
    请求无效,
    请求过大
};

std::string 转小写ASCII(std::string_view 输入) {
    std::string 结果;
    结果.reserve(输入.size());
    for (const unsigned char 字符 : 输入) {
        结果.push_back(字符 >= 'A' && 字符 <= 'Z'
            ? static_cast<char>(字符 - 'A' + 'a')
            : static_cast<char>(字符));
    }
    return 结果;
}

bool 是有效UTF8(std::string_view 文本) noexcept {
    std::size_t 位置 = 0;
    while (位置 < 文本.size()) {
        const auto 首字节 = static_cast<unsigned char>(文本[位置]);
        std::size_t 后续数 = 0;
        std::uint32_t 码点 = 0;
        if (首字节 <= 0x7fU) {
            ++位置;
            continue;
        }
        if ((首字节 & 0xe0U) == 0xc0U) {
            后续数 = 1;
            码点 = 首字节 & 0x1fU;
            if (码点 == 0) return false;
        } else if ((首字节 & 0xf0U) == 0xe0U) {
            后续数 = 2;
            码点 = 首字节 & 0x0fU;
        } else if ((首字节 & 0xf8U) == 0xf0U) {
            后续数 = 3;
            码点 = 首字节 & 0x07U;
        } else {
            return false;
        }
        if (位置 + 后续数 >= 文本.size()) return false;
        for (std::size_t 偏移 = 1; 偏移 <= 后续数; ++偏移) {
            const auto 后续 = static_cast<unsigned char>(文本[位置 + 偏移]);
            if ((后续 & 0xc0U) != 0x80U) return false;
            码点 = (码点 << 6U) | (后续 & 0x3fU);
        }
        if ((后续数 == 1 && 码点 < 0x80U)
            || (后续数 == 2 && 码点 < 0x800U)
            || (后续数 == 3 && 码点 < 0x10000U)
            || 码点 > 0x10ffffU
            || (码点 >= 0xd800U && 码点 <= 0xdfffU)) {
            return false;
        }
        位置 += 后续数 + 1;
    }
    return true;
}

bool 是合法对话框标识(std::string_view 值) noexcept {
    if (值.size() != 36 || 值[14] != '4'
        || (值[19] != '8' && 值[19] != '9' && 值[19] != 'a' && 值[19] != 'b')) return false;
    for (std::size_t i = 0; i < 值.size(); ++i) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            if (值[i] != '-') return false;
        } else if (!((值[i] >= '0' && 值[i] <= '9') || (值[i] >= 'a' && 值[i] <= 'f'))) return false;
    }
    return true;
}

请求读取状态 读取请求(SOCKET 客户端, HTTP请求& 请求) {
    std::string 缓冲;
    缓冲.reserve(4096);
    std::size_t 头结束 = std::string::npos;
    std::array<char, 4096> 分块{};
    while ((头结束 = 缓冲.find("\r\n\r\n")) == std::string::npos) {
        const int 已读 = recv(客户端, 分块.data(), static_cast<int>(分块.size()), 0);
        if (已读 == 0) return 请求读取状态::连接结束;
        if (已读 == SOCKET_ERROR) return 请求读取状态::请求无效;
        缓冲.append(分块.data(), static_cast<std::size_t>(已读));
        if (缓冲.size() > 最大请求头字节数) return 请求读取状态::请求过大;
    }

    const auto 首行结束 = 缓冲.find("\r\n");
    if (首行结束 == std::string::npos) return 请求读取状态::请求无效;
    {
        std::istringstream 首行(缓冲.substr(0, 首行结束));
        std::string 版本;
        if (!(首行 >> 请求.方法 >> 请求.目标 >> 版本)
            || (版本 != "HTTP/1.1" && 版本 != "HTTP/1.0")) {
            return 请求读取状态::请求无效;
        }
    }

    std::size_t 正文长度 = 0;
    bool 已有正文长度 = false;
    bool 已有内容类型 = false;
    std::size_t 当前行 = 首行结束 + 2;
    while (当前行 < 头结束) {
        const auto 行结束 = 缓冲.find("\r\n", 当前行);
        if (行结束 == std::string::npos || 行结束 > 头结束) {
            return 请求读取状态::请求无效;
        }
        const std::string_view 行(缓冲.data() + 当前行, 行结束 - 当前行);
        const auto 冒号 = 行.find(':');
        if (冒号 == std::string_view::npos) return 请求读取状态::请求无效;
        const auto 名称 = 转小写ASCII(行.substr(0, 冒号));
        std::string_view 值 = 行.substr(冒号 + 1);
        const bool 标识含控制字符 = 名称 == "x-hy-dialogue-id"
            && std::any_of(值.begin(), 值.end(), [](unsigned char 字符) {
                return 字符 < 0x20U || 字符 == 0x7fU;
            });
        while (!值.empty() && (值.front() == ' ' || 值.front() == '\t')) 值.remove_prefix(1);
        while (!值.empty() && (值.back() == ' ' || 值.back() == '\t')) 值.remove_suffix(1);
        if (名称 == "content-length") {
            if (已有正文长度) return 请求读取状态::请求无效;
            已有正文长度 = true;
            std::size_t 解析值 = 0;
            const auto 转换 = std::from_chars(值.data(), 值.data() + 值.size(), 解析值);
            if (转换.ec != std::errc{} || 转换.ptr != 值.data() + 值.size()) {
                return 请求读取状态::请求无效;
            }
            正文长度 = 解析值;
            请求.有正文长度 = true;
        } else if (名称 == "content-type") {
            if (已有内容类型) return 请求读取状态::请求无效;
            已有内容类型 = true;
            请求.内容类型 = 转小写ASCII(值);
        } else if (名称 == "x-hy-dialogue-id") {
            if (请求.对话框标识 || 标识含控制字符 || !是合法对话框标识(值))
                return 请求读取状态::请求无效;
            请求.对话框标识 = std::string(值);
        } else if (名称 == "host" || 名称 == "origin" || 名称 == "x-hy-control-output") {
            auto& 目标头 = 名称 == "host" ? 请求.主机
                : 名称 == "origin" ? 请求.来源 : 请求.控制输出标记;
            if (目标头 || 值.empty() || std::any_of(值.begin(), 值.end(), [](unsigned char 字符) {
                    return 字符 < 0x20U || 字符 == 0x7fU;
                })) 请求.控制安全头无效 = true;
            if (!目标头) 目标头 = std::string(值);
        } else if (名称 == "transfer-encoding") {
            return 请求读取状态::请求无效;
        }
        当前行 = 行结束 + 2;
    }
    if (正文长度 > 最大消息字节数) return 请求读取状态::请求过大;

    const std::size_t 正文开始 = 头结束 + 4;
    while (缓冲.size() - 正文开始 < 正文长度) {
        const auto 剩余 = 正文长度 - (缓冲.size() - 正文开始);
        const int 期望 = static_cast<int>((std::min)(剩余, 分块.size()));
        const int 已读 = recv(客户端, 分块.data(), 期望, 0);
        if (已读 <= 0) return 请求读取状态::请求无效;
        缓冲.append(分块.data(), static_cast<std::size_t>(已读));
    }
    请求.正文.assign(缓冲.data() + 正文开始, 正文长度);
    return 请求读取状态::已读取;
}

struct 网页HTTP写出结果 final {
    bool 完整写出 = false;
    std::size_t 已写字节数 = 0;
    int 系统错误 = 0;
};

网页HTTP写出结果 发送全部(SOCKET 客户端, std::string_view 内容) noexcept {
    网页HTTP写出结果 结果;
    while (!内容.empty()) {
        const auto 本次 = (std::min)(内容.size(),
            static_cast<std::size_t>((std::numeric_limits<int>::max)()));
        const int 已发 = send(客户端, 内容.data(), static_cast<int>(本次), 0);
        if (已发 == SOCKET_ERROR) {
            结果.系统错误 = WSAGetLastError();
            return 结果;
        }
        if (已发 == 0) return 结果;
        结果.已写字节数 += static_cast<std::size_t>(已发);
        内容.remove_prefix(static_cast<std::size_t>(已发));
    }
    结果.完整写出 = true;
    return 结果;
}

网页HTTP写出结果 发送响应(SOCKET 客户端,
    std::string_view 状态,
    std::string_view 类型,
    std::string_view 正文) noexcept {
    try {
        std::string 响应;
        响应.reserve(256 + 正文.size());
        响应 += "HTTP/1.1 ";
        响应 += 状态;
        响应 += "\r\nContent-Type: ";
        响应 += 类型;
        响应 += "\r\nContent-Length: ";
        响应 += std::to_string(正文.size());
        响应 += "\r\nConnection: close\r\nCache-Control: no-store\r\n";
        响应 += "X-Content-Type-Options: nosniff\r\n";
        响应 += "Referrer-Policy: no-referrer\r\n";
        响应 += "Content-Security-Policy: default-src 'self'; script-src 'unsafe-inline'; ";
        响应 += "style-src 'unsafe-inline'; connect-src 'self'; base-uri 'none'; ";
        响应 += "frame-ancestors 'none'\r\n\r\n";
        响应.append(正文);
        return 发送全部(客户端, 响应);
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"构造网页响应", L"完整HTTP载荷构造失败，不解释为网络断开。");
    }
}

std::string JSON转义(std::string_view 文本) {
    std::string 结果;
    结果.reserve(文本.size() + 8);
    constexpr char 十六进制[] = "0123456789abcdef";
    for (const unsigned char 字符 : 文本) {
        switch (字符) {
        case '"': 结果 += "\\\""; break;
        case '\\': 结果 += "\\\\"; break;
        case '\b': 结果 += "\\b"; break;
        case '\f': 结果 += "\\f"; break;
        case '\n': 结果 += "\\n"; break;
        case '\r': 结果 += "\\r"; break;
        case '\t': 结果 += "\\t"; break;
        default:
            if (字符 < 0x20U) {
                结果 += "\\u00";
                结果.push_back(十六进制[字符 >> 4U]);
                结果.push_back(十六进制[字符 & 0x0fU]);
            } else {
                结果.push_back(static_cast<char>(字符));
            }
        }
    }
    return 结果;
}

std::string 构造消息JSON(const std::vector<网页对话已显示消息>& 消息组) {
    std::string 结果 = "{\"messages\":[";
    bool 首项 = true;
    for (const auto& 消息 : 消息组) {
        if (!首项) 结果.push_back(',');
        首项 = false;
        结果 += "{\"id\":\"" + std::to_string(消息.页面序号)
            + "\",\"role\":\""
            + (消息.角色 == 网页对话消息角色::交互者
                ? "user" : "assistant")
            + "\",\"text\":\"" + JSON转义(消息.UTF8正文) + "\"}";
    }
    结果 += "]}";
    return 结果;
}

bool 服务输入已完成(网页服务输入状态 状态) noexcept {
    return 状态 == 网页服务输入状态::已完成
        || 状态 == 网页服务输入状态::已复用;
}

std::string_view 服务输入状态名称(网页服务输入状态 状态) noexcept {
    switch (状态) {
    case 网页服务输入状态::已完成: return "已完成";
    case 网页服务输入状态::匿名会话: return "匿名会话";
    case 网页服务输入状态::已复用: return "已复用";
    case 网页服务输入状态::未绑定: return "未绑定";
    case 网页服务输入状态::会话失效: return "会话失效";
    case 网页服务输入状态::输入不完整: return "输入不完整";
    case 网页服务输入状态::输入不合法: return "输入不合法";
    case 网页服务输入状态::对象不匹配: return "对象不匹配";
    case 网页服务输入状态::理解不存在: return "理解不存在";
    case 网页服务输入状态::结构不一致: return "结构不一致";
    case 网页服务输入状态::资源失败: return "资源失败";
    case 网页服务输入状态::需重新确认: return "需重新确认";
    case 网页服务输入状态::语义快照暂不支持: return "语义快照暂不支持";
    case 网页服务输入状态::已知值冲突: return "已知值冲突";
    case 网页服务输入状态::要求不存在: return "要求不存在";
    case 网页服务输入状态::目标范围已变化: return "目标范围已变化";
    }
    return "结构不一致";
}

std::string_view 服务输入HTTP状态(网页服务输入状态 状态) noexcept {
    switch (状态) {
    case 网页服务输入状态::已完成:
    case 网页服务输入状态::已复用: return "200 OK";
    case 网页服务输入状态::匿名会话:
    case 网页服务输入状态::未绑定:
    case 网页服务输入状态::会话失效: return "403 Forbidden";
    case 网页服务输入状态::输入不完整:
    case 网页服务输入状态::输入不合法: return "400 Bad Request";
    case 网页服务输入状态::对象不匹配:
    case 网页服务输入状态::已知值冲突:
    case 网页服务输入状态::需重新确认:
    case 网页服务输入状态::目标范围已变化: return "409 Conflict";
    case 网页服务输入状态::要求不存在:
    case 网页服务输入状态::理解不存在: return "404 Not Found";
    case 网页服务输入状态::语义快照暂不支持: return "422 Unprocessable Content";
    default: return "503 Service Unavailable";
    }
}

void 发送服务输入失败(SOCKET 客户端, 网页服务输入状态 状态) noexcept {
    try {
        发送响应(客户端, 服务输入HTTP状态(状态),
            "application/json;charset=utf-8",
            "{\"status\":\"" + std::string(服务输入状态名称(状态))
                + "\",\"info\":null}");
    } catch (...) {
        发送响应(客户端, "503 Service Unavailable", "application/json;charset=utf-8",
            "{\"status\":\"资源失败\",\"info\":null}");
    }
}

using 表单字段组 = std::vector<std::pair<std::string, std::string>>;

bool 解码表单分量(std::string_view 输入, std::string& 输出) {
    输出.clear();
    输出.reserve(输入.size());
    const auto 十六进制值 = [](char 字符) noexcept -> int {
        if (字符 >= '0' && 字符 <= '9') return 字符 - '0';
        if (字符 >= 'a' && 字符 <= 'f') return 字符 - 'a' + 10;
        if (字符 >= 'A' && 字符 <= 'F') return 字符 - 'A' + 10;
        return -1;
    };
    for (std::size_t i = 0; i < 输入.size(); ++i) {
        if (输入[i] == '+') 输出.push_back(' ');
        else if (输入[i] == '%') {
            if (i + 2 >= 输入.size()) return false;
            const int 高位 = 十六进制值(输入[i + 1]);
            const int 低位 = 十六进制值(输入[i + 2]);
            if (高位 < 0 || 低位 < 0) return false;
            输出.push_back(static_cast<char>(高位 * 16 + 低位));
            i += 2;
        } else 输出.push_back(输入[i]);
    }
    return 是有效UTF8(输出);
}

bool 解析表单(std::string_view 正文,
    const std::vector<std::string_view>& 允许名称, 表单字段组& 字段组) {
    字段组.clear();
    if (正文.empty()) return true;
    while (true) {
        const auto 分隔 = 正文.find('&');
        const auto 项 = 正文.substr(0, 分隔);
        const auto 等号 = 项.find('=');
        if (项.empty() || 等号 == std::string_view::npos) return false;
        std::string 名称, 值;
        if (!解码表单分量(项.substr(0, 等号), 名称)
            || !解码表单分量(项.substr(等号 + 1), 值)
            || std::find(允许名称.begin(), 允许名称.end(), 名称) == 允许名称.end()
            || std::any_of(字段组.begin(), 字段组.end(),
                [&](const auto& 已有) { return 已有.first == 名称; })) return false;
        字段组.emplace_back(std::move(名称), std::move(值));
        if (分隔 == std::string_view::npos) return true;
        正文.remove_prefix(分隔 + 1);
    }
}

const std::string* 取得表单字段(const 表单字段组& 字段组,
    std::string_view 名称) noexcept {
    for (const auto& 字段 : 字段组) {
        if (字段.first == 名称) return &字段.second;
    }
    return nullptr;
}

template<class 整数>
网页服务输入状态 解析正整数(const std::string* 文本, 整数& 值) noexcept {
    if (!文本 || 文本->empty()) return 网页服务输入状态::输入不完整;
    const auto 转换 = std::from_chars(文本->data(), 文本->data() + 文本->size(), 值);
    if (转换.ec != std::errc{} || 转换.ptr != 文本->data() + 文本->size()
        || 值 <= 0) return 网页服务输入状态::输入不合法;
    return 网页服务输入状态::已完成;
}

网页服务输入状态 解析服务目标(const 表单字段组& 字段组,
    网页服务目标提交& 目标) noexcept {
    auto 状态 = 解析正整数(取得表单字段(字段组, "target"), 目标.目标存在编码);
    if (!服务输入已完成(状态)) return 状态;
    状态 = 解析正整数(取得表单字段(字段组, "ft"), 目标.特征类型编码);
    if (!服务输入已完成(状态)) return 状态;
    状态 = 解析正整数(取得表单字段(字段组, "form"), 目标.目标形式);
    if (!服务输入已完成(状态)) return 状态;
    if (目标.目标形式 == 1) {
        if (取得表单字段(字段组, "direction")) return 网页服务输入状态::输入不合法;
        std::uint64_t 域 = 0, 比较 = 0;
        状态 = 解析正整数(取得表单字段(字段组, "domain"), 域);
        if (!服务输入已完成(状态)) return 状态;
        状态 = 解析正整数(取得表单字段(字段组, "comparison"), 比较);
        if (!服务输入已完成(状态)) return 状态;
        目标.目标域编码 = 域;
        目标.比较概念编码 = 比较;
    } else if (目标.目标形式 == 2) {
        if (取得表单字段(字段组, "domain") || 取得表单字段(字段组, "comparison"))
            return 网页服务输入状态::输入不合法;
        std::int64_t 方向 = 0;
        状态 = 解析正整数(取得表单字段(字段组, "direction"), 方向);
        if (!服务输入已完成(状态)) return 状态;
        if (方向 != 1 && 方向 != 2) return 网页服务输入状态::输入不合法;
        目标.变化方向 = 方向;
    } else return 网页服务输入状态::输入不合法;
    return 网页服务输入状态::已完成;
}

bool 服务目标形状完整(const 网页服务目标提交& 目标) noexcept {
    if (目标.目标存在编码 == 0 || 目标.特征类型编码 == 0) return false;
    if (目标.目标形式 == 1) return 目标.目标域编码 && *目标.目标域编码 != 0
        && 目标.比较概念编码 && *目标.比较概念编码 != 0 && !目标.变化方向;
    return 目标.目标形式 == 2 && !目标.目标域编码 && !目标.比较概念编码
        && 目标.变化方向 && (*目标.变化方向 == 1 || *目标.变化方向 == 2);
}

网页服务输入状态 解析部分服务目标(const 表单字段组& 字段组,
    网页部分服务要求提交& 目标) {
    const auto* 来源 = 取得表单字段(字段组, "source");
    if (!来源 || 来源->empty() || !是有效UTF8(*来源)) return 网页服务输入状态::输入不合法;
    目标.来源要求标识 = *来源;
    const auto 可选整数 = [&]<class T>(std::string_view 名称, std::optional<T>& 输出) {
        const auto* 文本 = 取得表单字段(字段组, 名称);
        if (!文本 || 文本->empty()) return true;
        T 值{};
        if (解析正整数(文本, 值) != 网页服务输入状态::已完成) return false;
        输出 = 值; return true;
    };
    if (!可选整数("request",目标.要求编码) || !可选整数("target",目标.目标存在编码)
        || !可选整数("ft",目标.特征类型编码)
        || !可选整数("form",目标.目标形式) || !可选整数("domain",目标.目标域编码)
        || !可选整数("comparison",目标.比较概念编码) || !可选整数("direction",目标.变化方向))
        return 网页服务输入状态::输入不合法;
    if ((目标.目标形式 && *目标.目标形式 != 1 && *目标.目标形式 != 2)
        || (目标.变化方向 && *目标.变化方向 != 1 && *目标.变化方向 != 2))
        return 网页服务输入状态::输入不合法;
    return 网页服务输入状态::已完成;
}

std::string_view 服务材料投递状态名称(网页服务材料投递状态 状态) noexcept {
    switch (状态) {
    case 网页服务材料投递状态::未尝试: return "未尝试";
    case 网页服务材料投递状态::等待后继消费: return "等待后继消费";
    case 网页服务材料投递状态::消息已接纳: return "消息已接纳";
    case 网页服务材料投递状态::暂未接纳: return "暂未接纳";
    }
    return "未尝试";
}

void 发送部分服务要求结果(SOCKET 客户端, const 网页部分服务要求结果& r) {
    std::string 正文 = "{\"status\":\"" + std::string(服务输入状态名称(r.状态)) + "\"";
    const auto 文本 = [&](std::string_view 名称, std::string_view 值) {
        正文 += ",\""; 正文 += 名称; 正文 += "\":\""; 正文 += JSON转义(值); 正文 += '"';
    };
    const auto 数 = [&](std::string_view 名称, const auto& 值) {
        if (值) 文本(名称, std::to_string(*值));
    };
    数("request",r.要求编码); 数("object",r.服务对象编码);
    文本("source",r.来源要求标识);
    数("target",r.已知目标.目标存在编码);
    文本("delivery",服务材料投递状态名称(r.材料投递状态));
    数("ft",r.已知目标.特征类型编码); 数("form",r.已知目标.目标形式);
    数("domain",r.已知目标.目标域编码); 数("comparison",r.已知目标.比较概念编码);
    数("direction",r.已知目标.变化方向);
    正文 += std::string(",\"accepted\":") + (r.消息已接纳 ? "true" : "false");
    const auto 列表 = [&](std::string_view 名称, const std::vector<std::string>& 值) {
        正文 += ",\""; 正文 += 名称; 正文 += "\":[";
        bool first=true; for (const auto& x : 值) {
            if (!first) 正文 += ','; first=false;
            正文 += '"'; 正文 += JSON转义(x); 正文 += '"';
        }
        正文 += ']';
    };
    列表("missing",r.缺项); 列表("conflicts",r.冲突项);
    if (r.完整预览) {
        文本("preview",std::to_string(r.完整预览->理解编码));
        文本("text",r.完整预览->UTF8重述);
        数("confirmation",r.完整预览->确认编码);
    }
    正文 += '}';
    发送响应(客户端,服务输入HTTP状态(r.状态),"application/json;charset=utf-8",正文);
}

void 发送服务要求结果(SOCKET 客户端, const 网页服务要求结果& 结果,
    bool 要求已确认) {
    if (!服务输入已完成(结果.状态)) {
        发送服务输入失败(客户端, 结果.状态);
        return;
    }
    if (!结果.信息 || !结果.信息->理解编码 || !结果.信息->服务对象编码
        || !服务目标形状完整(结果.信息->目标) || 结果.信息->UTF8重述.empty()
        || !是有效UTF8(结果.信息->UTF8重述)
        || (结果.信息->确认编码 && *结果.信息->确认编码 == 0)
        || (要求已确认 && !结果.信息->确认编码)) {
        发送服务输入失败(客户端, 网页服务输入状态::结构不一致);
        return;
    }
    const auto& 信息 = *结果.信息;
    const auto& 目标 = 信息.目标;
    std::string 正文 = "{\"status\":\"" + std::string(服务输入状态名称(结果.状态))
        + "\",\"delivery\":\"" + std::string(服务材料投递状态名称(结果.材料投递状态))
        + "\",\"info\":{\"preview\":\"" + std::to_string(信息.理解编码)
        + "\",\"object\":\"" + std::to_string(信息.服务对象编码)
        + "\",\"target\":\"" + std::to_string(目标.目标存在编码)
        + "\",\"ft\":\"" + std::to_string(目标.特征类型编码)
        + "\",\"form\":\"" + std::to_string(目标.目标形式) + "\"";
    if (目标.目标域编码) 正文 += ",\"domain\":\"" + std::to_string(*目标.目标域编码) + "\"";
    if (目标.比较概念编码) 正文 += ",\"comparison\":\"" + std::to_string(*目标.比较概念编码) + "\"";
    if (目标.变化方向) 正文 += ",\"direction\":\"" + std::to_string(*目标.变化方向) + "\"";
    正文 += ",\"confirmation\":";
    正文 += 信息.确认编码 ? "\"" + std::to_string(*信息.确认编码) + "\"" : "null";
    正文 += ",\"text\":\"" + JSON转义(信息.UTF8重述) + "\"}}";
    发送响应(客户端, 服务输入HTTP状态(结果.状态), "application/json;charset=utf-8", 正文);
}

void 发送服务目标选项(SOCKET 客户端, const 网页服务目标选项结果& 结果) {
    if (!服务输入已完成(结果.状态)) {
        发送服务输入失败(客户端, 结果.状态);
        return;
    }
    if (!结果.服务对象编码 || *结果.服务对象编码 == 0
        || std::any_of(结果.当前特征提示组.begin(), 结果.当前特征提示组.end(),
            [](const auto& 项) { return !项.特征类型编码 || !项.当前采用概念编码; })) {
        发送服务输入失败(客户端, 网页服务输入状态::结构不一致);
        return;
    }
    std::string 正文 = "{\"status\":\"" + std::string(服务输入状态名称(结果.状态))
        + "\",\"object\":\"" + std::to_string(*结果.服务对象编码) + "\",\"hints\":[";
    bool 首项 = true;
    for (const auto& 项 : 结果.当前特征提示组) {
        if (!首项) 正文 += ',';
        首项 = false;
        正文 += "{\"ft\":\"" + std::to_string(项.特征类型编码)
            + "\",\"concept\":\"" + std::to_string(项.当前采用概念编码) + "\"}";
    }
    正文 += "]}";
    发送响应(客户端, 服务输入HTTP状态(结果.状态), "application/json;charset=utf-8", 正文);
}

bool 处理服务输入请求(SOCKET 客户端, const HTTP请求& 请求,
    std::string_view 基础路径, std::string_view 公开会话标识,
    const 网页服务输入回调组& 回调) noexcept {
    try {
        const std::string 服务路径 = std::string(基础路径) + "/api/service/";
        if (!请求.目标.starts_with(服务路径)) return false;
        if (!回调.会话开始) {
            发送服务输入失败(客户端, 网页服务输入状态::未绑定);
            return true;
        }
        const std::string_view 路由 = std::string_view(请求.目标).substr(服务路径.size());
        if (请求.方法 == "GET" && 路由 == "options" && 请求.正文.empty()) {
            发送服务目标选项(客户端, 回调.查询目标提示(公开会话标识));
            return true;
        }
        表单字段组 字段组;
        if (请求.方法 == "GET" && 路由.starts_with("request?") && 请求.正文.empty()) {
            std::uint64_t 编码 = 0;
            const auto 状态 = 解析表单(路由.substr(8), {"id"}, 字段组)
                ? 解析正整数(取得表单字段(字段组,"id"),编码) : 网页服务输入状态::输入不合法;
            if (!服务输入已完成(状态)) 发送服务输入失败(客户端,状态);
            else if (!回调.查询部分要求) 发送服务输入失败(客户端,网页服务输入状态::未绑定);
            else 发送部分服务要求结果(客户端,回调.查询部分要求(公开会话标识,编码));
            return true;
        }
        if (请求.方法 == "POST" && 路由 == "request") {
            网页部分服务要求提交 输入;
            const auto 状态 = 请求.内容类型 == "application/x-www-form-urlencoded"
                && 解析表单(请求.正文,{"request","source","target","ft","form","domain","comparison","direction"},字段组)
                ? 解析部分服务目标(字段组,输入) : 网页服务输入状态::输入不合法;
            if (!服务输入已完成(状态)) 发送服务输入失败(客户端,状态);
            else if (!回调.提交部分要求) 发送服务输入失败(客户端,网页服务输入状态::未绑定);
            else 发送部分服务要求结果(客户端,回调.提交部分要求(公开会话标识,输入));
            return true;
        }
        if (请求.方法 == "GET" && 路由.starts_with("preview?") && 请求.正文.empty()) {
            std::uint64_t 编码 = 0;
            const auto 状态 = 解析表单(路由.substr(8), {"id"}, 字段组)
                ? 解析正整数(取得表单字段(字段组, "id"), 编码)
                : 网页服务输入状态::输入不合法;
            if (!服务输入已完成(状态)) 发送服务输入失败(客户端, 状态);
            else 发送服务要求结果(客户端, 回调.读取预览(公开会话标识, 编码), false);
            return true;
        }
        if (请求.方法 == "POST" && (路由 == "preview" || 路由 == "confirm")) {
            if (请求.内容类型 != "application/x-www-form-urlencoded") {
                发送服务输入失败(客户端, 网页服务输入状态::输入不合法);
                return true;
            }
            if (路由 == "preview") {
                网页服务目标提交 目标;
                const auto 状态 = 解析表单(请求.正文,
                    {"target", "ft", "form", "domain", "comparison", "direction"}, 字段组)
                    ? 解析服务目标(字段组, 目标) : 网页服务输入状态::输入不合法;
                if (!服务输入已完成(状态)) 发送服务输入失败(客户端, 状态);
                else 发送服务要求结果(客户端, 回调.预览(公开会话标识, 目标), false);
            } else {
                std::uint64_t 编码 = 0;
                const auto 状态 = 解析表单(请求.正文, {"preview"}, 字段组)
                    ? 解析正整数(取得表单字段(字段组, "preview"), 编码)
                    : 网页服务输入状态::输入不合法;
                if (!服务输入已完成(状态)) 发送服务输入失败(客户端, 状态);
                else 发送服务要求结果(客户端, 回调.确认(公开会话标识, 编码), true);
            }
            return true;
        }
        发送服务输入失败(客户端, 网页服务输入状态::输入不合法);
    } catch (const std::bad_alloc&) {
        发送服务输入失败(客户端, 网页服务输入状态::资源失败);
    } catch (...) {
        发送服务输入失败(客户端, 网页服务输入状态::结构不一致);
    }
    return true;
}

std::string 构造页面(std::string_view 基础路径) {
    std::string 页面 = R"HTML(<!doctype html>
<html lang="zh-CN">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>海中鱼巣</title>
<style>
:root{color-scheme:dark;font-family:"Microsoft YaHei UI",sans-serif;background:#10151d;color:#edf3fa}
body{margin:0;display:grid;place-items:center;min-height:100vh;background:radial-gradient(circle at top,#23334a,#10151d 55%)}
main{width:min(1040px,94vw);min-height:90vh;display:grid;grid-template-rows:auto minmax(160px,1fr) auto auto;border:1px solid #3c516d;border-radius:18px;background:#182230;box-shadow:0 24px 80px #0008}
header{padding:18px 22px;border-bottom:1px solid #304157;display:flex;justify-content:space-between;align-items:center}
h1{font-size:20px;margin:0}.status{font-size:13px;color:#8fd3a7}
#messages{overflow:auto;max-height:40vh;padding:20px;display:flex;flex-direction:column;gap:12px}
.empty{margin:auto;color:#91a0b3}.message{align-self:flex-end;max-width:76%;padding:12px 15px;border-radius:15px 15px 4px 15px;background:#2e73c8;white-space:pre-wrap;overflow-wrap:anywhere}.message.assistant{align-self:flex-start;border-radius:15px 15px 15px 4px;background:#34475f}
form{padding:16px;border-top:1px solid #304157;display:grid;grid-template-columns:1fr auto auto;gap:10px}
textarea{resize:none;min-height:54px;max-height:160px;padding:12px;border:1px solid #405773;border-radius:12px;background:#0f1823;color:inherit;font:inherit}
button{border:0;border-radius:11px;padding:0 18px;font:inherit;color:white;background:#2e73c8;cursor:pointer}button.secondary{background:#4a5566}button:disabled{opacity:.55;cursor:wait}
footer{grid-column:1/-1;font-size:12px;color:#8d9aac}
#service{padding:18px;border-top:1px solid #304157}#service h2{font-size:18px;margin:0 0 10px}
#service-form{padding:0;border:0;display:block}#service-fields{border:0;padding:0;display:grid;grid-template-columns:repeat(auto-fit,minmax(210px,1fr));gap:12px}
label{display:grid;gap:6px;font-size:14px}[hidden]{display:none!important}input,select{min-width:0;padding:9px;border:1px solid #405773;border-radius:8px;background:#0f1823;color:inherit;font:inherit}input[readonly]{color:#a5b6c9}
#service p{font-size:13px;color:#a5b6c9;overflow-wrap:anywhere}.service-actions{display:flex;gap:10px;margin-top:12px}.service-actions button{min-height:40px}
#service-preview{white-space:pre-wrap;overflow-wrap:anywhere;font:inherit;font-size:14px;max-height:45vh;overflow:auto}#service-status{white-space:pre-wrap}
body{display:block;background:#0e1520}main{width:min(1760px,96vw);margin:20px auto;display:block;min-height:0}header{gap:16px;flex-wrap:wrap}
.workspace{display:grid;grid-template-columns:minmax(240px,.8fr) minmax(300px,1fr) minmax(360px,1.25fr);gap:14px;padding:14px;align-items:start}.panel{min-width:0;background:#121d2a;border:1px solid #304157;border-radius:12px;overflow:hidden}.panel h2{font-size:16px;margin:0;padding:14px 16px;border-bottom:1px solid #304157}.panel-body{padding:14px 16px}.muted,.hint{font-size:12px;color:#9badc1;line-height:1.7}.hint{margin:8px 0}.overview{padding:14px;display:grid;grid-template-columns:repeat(auto-fit,minmax(190px,1fr));gap:10px}.metric{border:1px solid #304157;border-radius:10px;padding:12px;background:#121d2a}.metric .caption{color:#9badc1;font-size:12px}.metric .value{font-size:16px;margin-top:6px;overflow-wrap:anywhere}.tree{max-height:68vh;overflow:auto}.tree ul{padding-left:18px;list-style:none}.tree button{width:100%;padding:9px;text-align:left;margin:3px 0;background:#233449;font-size:13px;overflow-wrap:anywhere}.tree button.selected{background:#2e6595;outline:1px solid #87b8e4}.tree .task{background:#1b2a3d}.detail{max-height:68vh;overflow:auto}dl{margin:0}dt{font-size:12px;color:#94abc2;margin-top:12px}dd{margin:4px 0;overflow-wrap:anywhere;white-space:pre-wrap;font-size:13px;line-height:1.6}.object{border-left:2px solid #365573;padding-left:10px;margin:5px 0}.object>summary{cursor:pointer;font-size:13px;color:#b9cbe0}.stack{display:grid;gap:14px}#messages{height:27vh;max-height:380px}#service{padding:14px}#service-fields{grid-template-columns:repeat(auto-fit,minmax(160px,1fr))}.service-actions{flex-wrap:wrap}.service-actions button{padding:8px 12px;font-size:13px}#form{grid-template-columns:1fr auto auto;padding:12px;gap:8px}#form button{padding:0 12px}#control-status{color:#b4c6db;font-size:12px}#control-meta{padding:0 16px 12px}.section-label{font-size:13px;color:#b4c6db;margin:16px 0 8px}.notice{padding:10px;border:1px dashed #41556d;border-radius:8px;color:#a4b5c8;font-size:12px;line-height:1.7}.warning{color:#f1c47b}
@media(max-width:1120px){.workspace{grid-template-columns:minmax(240px,1fr) minmax(340px,1.4fr)}.interaction-column{grid-column:1/-1}.tree,.detail{max-height:50vh}#messages{height:30vh}}@media(max-width:680px){main{width:98vw;margin:5px auto}.workspace{grid-template-columns:1fr;padding:8px}.interaction-column{grid-column:auto}header{padding:14px}h1{font-size:17px}.overview{padding:8px;grid-template-columns:1fr 1fr}.tree,.detail{max-height:40vh}#form{grid-template-columns:1fr auto}#input{grid-column:1/-1}.message{max-width:90%}}
</style>
)HTML";
    页面 += R"HTML(
</head>
<body>
<main>
<header><div><h1>海中鱼巣 · 运行控制台</h1><div class="muted">正式数据只读投影 · 交互与治理分账</div></div><div><div id="control-status">控制快照：等待首次采集</div><span class="status" id="status">正在访问本机服务</span></div></header>
<section class="overview" id="overview"><div class="notice">运行总览等待正式快照；不以页面可达表示业务正常。</div></section>
<div id="control-meta" class="muted">分项只读，非跨服务原子快照；采集时间不是业务推进时间。</div>
<div class="workspace">
<section class="panel"><h2>需求与任务</h2><div id="control-tree" class="panel-body tree"><p class="muted">尚无快照</p></div></section>
<section class="panel"><h2 id="detail-title">选中项详情</h2><div id="control-detail" class="panel-body detail"><p class="muted">选择需求或任务，查看目标、筹办和执行材料。</p></div></section>
<div class="stack interaction-column"><section class="panel"><h2>当前窗口交互</h2>
<section id="messages"><div class="empty">尚无对话消息</div></section>
<form id="form"><textarea id="input" maxlength="1048576" placeholder="输入消息；Enter发送，Shift+Enter换行"></textarea><button type="submit">发送</button><button type="button" class="secondary" id="close">结束</button><footer>网页接收成功只表示消息进入当前交互宿主，不表示任务完成或需求满足。</footer></form>
<section id="output-control" style="padding:14px;border-top:1px solid #304157">
<h2>控制台提交输出</h2>
<p class="notice">操作员供稿：将正文提交到当前窗口的正式输出队列。不是交互入站，不表示自我自主生成意图，不生成需求或本能动作。对话历史中的出站方向也不证明内容自主生成。</p>
<form id="output-form" style="display:grid;gap:8px">
<label for="output-input">待输出正文</label><textarea id="output-input" maxlength="1048576" placeholder="输入要从本窗口输出的正文"></textarea>
<button id="output-submit" type="submit" disabled>控制台提交输出</button>
<label id="output-unknown-confirm" hidden><input id="output-unknown-ack" type="checkbox">我已核对，知道新提交可能重复；下一次明确点击将作为新输出提交</label>
</form><p id="output-status" class="hint" aria-live="polite">等待本窗口完成登记</p>
<div id="output-receipts" aria-live="polite"></div>
</section>
<section id="service">
<h2>结构化服务要求</h2>
<p>目标人由本次本机启动配置指定。可先提交已知部分，随后补齐同一要求；已保存字段不能换义，新诉求请点“新要求”。完整重述后再明确确认。</p>
<p id="service-hints"></p>
<form id="service-form">
<fieldset id="service-fields" disabled>
<label>需求主体编码（会话绑定）<input id="service-object" readonly></label>
<label>目标存在编码<input name="target" id="service-target" inputmode="numeric" pattern="[0-9]+" placeholder="可暂不填写，保存为缺项"></label>
<label>目标特征类型编码<input name="ft" inputmode="numeric" autocomplete="off"></label>
<label>目标形式<select name="form" id="service-mode"><option value="">暂不明确</option><option value="1">值目标</option><option value="2">方向目标</option></select></label>
<label id="domain-label">目标域概念编码<input name="domain" inputmode="numeric" autocomplete="off" disabled></label>
<label id="comparison-label">比较概念编码<input name="comparison" inputmode="numeric" autocomplete="off" disabled></label>
<label id="direction-label">变化方向<select name="direction" disabled><option value="">请选择</option><option value="1">增加</option><option value="2">减少</option></select></label>
</fieldset>
<div class="service-actions"><button type="submit" id="service-submit" disabled>保存并补充要求</button><button type="button" id="service-confirm" disabled>确认当前预览</button><button type="button" id="service-refresh" disabled>刷新状态</button><button type="button" id="service-new">新要求</button></div>
</form>
<p id="service-status">正在核对本次会话绑定</p><pre id="service-preview"></pre>
</section>
</section><section class="panel"><h2>方法与学习</h2><div class="panel-body" id="control-learning"><p class="muted">等待快照</p></div></section></div>
</div><section class="panel" style="margin:0 14px 14px"><h2>方法目录与能力缺口</h2><div class="panel-body" style="display:grid;grid-template-columns:repeat(auto-fit,minmax(280px,1fr));gap:12px"><div class="tree" id="control-method-catalog">等待正式方法目录</div><div class="tree" id="control-missing-methods">等待缺失方法列表</div><div class="detail" id="control-method-detail">选择方法或缺失方法头查看详情</div></div></section>
<section class="panel" style="margin:0 14px 14px"><h2>任务执行证据时间线</h2><div class="panel-body" id="control-timeline"><div class="notice">等待正式执行引用；本区不表示三世界完整历史。</div></div></section>
</main>
<script>
const base=')HTML";
    页面 += 基础路径;
    页面 += R"HTML(';
const messages=document.getElementById('messages');
const input=document.getElementById('input');
const form=document.getElementById('form');
const status=document.getElementById('status');
const dialogueId=globalThis.crypto?.randomUUID?.();
const outputForm=document.getElementById('output-form'),outputInput=document.getElementById('output-input');
const outputButton=document.getElementById('output-submit'),outputReceipts=document.getElementById('output-receipts');
const outputStatus=document.getElementById('output-status'),outputUnknownConfirm=document.getElementById('output-unknown-confirm');
const outputUnknownAck=document.getElementById('output-unknown-ack'),outputRows=[];
const outputStates=new Set(['已排队','正在写出','已完整写出','写出未完成','已取消']);
const outputTerminalStates=new Set(['已完整写出','写出未完成','已取消']);
let outputReady=false,outputClosed=false,outputPosting=false,outputQuerying=false,outputUnknown=false,outputFault=false;
let outputSubmitAbort=null,outputStateAbort=null;
function renderOutputReceipts(){
  outputButton.disabled=!dialogueId||!outputReady||outputClosed||outputPosting||outputFault||(outputUnknown&&!outputUnknownAck.checked);
  outputUnknownConfirm.hidden=!outputUnknown;
  outputStatus.textContent=outputClosed?'输出操作已停止':outputFault?'控制操作界面错误，已禁用提交':outputPosting?'处理中，不重复提交':outputUnknown?'提交结果不确定，可能已经入列；不会自动重发':outputReady?'当前窗口已登记；排队不等于发送完成':'等待本窗口完成登记';
  outputReceipts.replaceChildren();
  for(const row of outputRows){
    const article=document.createElement('article');article.className='notice';
    const title=document.createElement('p');title.textContent='控制台提交（操作员供稿） · '+(row.submissionId?'正式提交 #'+row.submissionId:'尚无已知正式序号');
    const body=document.createElement('pre');body.style.whiteSpace='pre-wrap';body.textContent=row.text;
    const detail=document.createElement('p');detail.textContent=row.state+(row.queryError?'；本次查询失败，保留上次状态：'+row.queryError:'');
    if(row.state==='已完整写出')detail.textContent+='；完整HTTP已写出，不证明对方阅读或理解';
    if(row.state==='写出未完成')detail.textContent+='；可能部分写出，不自动补发';
    if(row.uncertaintyAcknowledged)detail.textContent+='；操作员已确认后续新提交可能重复，本条结果仍不确定';
    article.append(title,body,detail);outputReceipts.append(article);
  }
}
async function outputRefreshStates(){
  if(outputClosed||outputQuerying||outputFault)return;
  outputQuerying=true;
  try{
    for(const row of outputRows){
      if(outputClosed||outputFault)break;
      if(!row.submissionId||outputTerminalStates.has(row.state))continue;
      const abort=new AbortController();outputStateAbort=abort;const timer=setTimeout(()=>abort.abort(),4000);
      try{
        const response=await fetch(base+'/api/control/output/status?id='+encodeURIComponent(row.submissionId),{cache:'no-store',headers:{'X-HY-Dialogue-Id':dialogueId,'X-HY-Control-Output':'1'},signal:abort.signal});
        if(!response.ok)throw new Error('HTTP '+response.status);
        let data;
        try{data=await response.json();}catch(error){if(error instanceof SyntaxError){outputFault=true;throw new Error('正式状态回执JSON格式错误');}throw error;}
        if(!data||data.accepted!==true||data.automaticRetry!==false||data.submissionId!==row.submissionId||!outputStates.has(data.status)||typeof data.httpBytes!=='string'||!/^\d+$/.test(data.httpBytes)||!Number.isInteger(data.systemError)){outputFault=true;throw new Error('正式状态回执结构不一致');}
        if(outputClosed)break;
        row.state=data.status;row.queryError='';
      }catch(error){if(!outputClosed)row.queryError=error.name==='AbortError'?'请求超时':error.message;}
      finally{clearTimeout(timer);if(outputStateAbort===abort)outputStateAbort=null;}
    }
  }catch(error){outputFault=true;outputStatus.textContent='控制操作界面错误：'+error.message;}
  finally{outputQuerying=false;if(!outputClosed)renderOutputReceipts();}
}
outputUnknownAck.addEventListener('change',()=>renderOutputReceipts());
outputForm.addEventListener('submit',async event=>{
  event.preventDefault();
  if(outputClosed||outputPosting||outputFault||!outputReady||!dialogueId||(outputUnknown&&!outputUnknownAck.checked))return;
  const text=outputInput.value;
  if(!text||new TextEncoder().encode(text).length>1048576){outputStatus.textContent='正文必须非空且不超过1 MiB UTF-8字节；未提交';return;}
  if(outputUnknown){for(const row of outputRows)if(row.unknown)row.uncertaintyAcknowledged=true;outputUnknown=false;}
  outputUnknownAck.checked=false;
  const row={text,submissionId:null,state:'处理中，不重复提交',queryError:'',unknown:false};outputRows.push(row);
  const abort=new AbortController();outputSubmitAbort=abort;outputPosting=true;renderOutputReceipts();
  try{
    const response=await fetch(base+'/api/control/output',{method:'POST',headers:{'Content-Type':'text/plain;charset=utf-8','X-HY-Dialogue-Id':dialogueId,'X-HY-Control-Output':'1'},body:text,signal:abort.signal});
    const data=await response.json();
    if(outputClosed)return;
    if(response.status===202&&data.accepted===true&&data.automaticRetry===false&&data.status==='已排队'&&typeof data.submissionId==='string'&&/^[1-9]\d{0,19}$/.test(data.submissionId)&&BigInt(data.submissionId)<=18446744073709551615n&&data.httpBytes==='0'&&data.systemError===0){
      row.submissionId=data.submissionId;row.state=data.status;if(outputInput.value===text)outputInput.value='';
    }else if(!response.ok&&data.accepted===false&&data.automaticRetry===false&&data.submissionId===null&&typeof data.status==='string'){
      row.state='未入列：HTTP '+response.status+' · '+data.status;
    }else throw new Error('提交回执结构不一致');
  }catch(error){row.state='提交结果不确定，可能已经入列：'+(error.name==='AbortError'?'客户端取消等待':error.message);row.unknown=true;outputUnknown=true;}
  finally{if(outputSubmitAbort===abort)outputSubmitAbort=null;outputPosting=false;if(!outputClosed)renderOutputReceipts();}
});
const controlLabels={status:'状态',reason:'原因',id:'身份',parentId:'父需求',subject:'需求主体',executionDomain:'执行域',activity:'活动状态',weight:'权重',sourceType:'来源类型',sourceTaskId:'来源任务',complete:'字段完整',target:'目标',existence:'目标存在',featureType:'特征类型',form:'目标形式',valueDomain:'目标范围',comparison:'比较要求',direction:'方向',referencedDemand:'引用需求',contentPath:'内容路径',demandId:'承接需求',state:'任务状态',waitReason:'等待原因',schedulingStatus:'调度状态',lastMethodId:'最近方法',executionId:'执行实例',executionOrganizationId:'执行组织',queryComplete:'候选查询完整',candidates:'候选方法',conditionReview:'条件核验',execution:'执行材料',preparation:'最近筹办摘要',conditionProposalCount:'条件提议数',causalProposalCount:'因果提议数',compositeProposalCount:'复合目标提议数',self:'自我线程',manager:'任务管理线程',serviceValue:'自我服务值',lastBusinessProgressAt:'最近业务推进时间',threadStatus:'学习线程状态',methods:'当前候选方法（非全量目录）',parameterCount:'参数数量',pairCount:'条件结果对数量',enabled:'启用',parameters:'参数规格',position:'参数位置',informationType:'信息类型',range:'范围概念',lifecycle:'生命周期',activeMessageCount:'活动消息数',activeWorkCount:'活动工作数',pendingFeedbackTaskCount:'待反馈任务数',schedulingInFlight:'调度扫描在途',pendingLearningMethodCount:'待学习方法数',stopRequested:'停止已请求',completed:'已完成',recentLearningEnd:'最近学习结束',recentLearningError:'最近学习运行异常',waitingMethodInstanceConsumed:'等待方法实例消费'};
)HTML";
    页面 += R"HTML(
Object.assign(controlLabels,{activeMessages:'活动消息数',activeWork:'活动工作数',pendingFeedback:'待反馈任务数',pendingReview:'待复核任务数',pendingMethods:'自我待学习方法数',finished:'线程已完成',lastSelfCompletionMethodId:'自我最近学习结束的方法',completionDeliveredToManager:'学习结束已交接管理',lastManagerCompletionMethodId:'管理最近学习结束的方法',waitingMethodInstance:'等待方法实例消费',lastSelfError:'自我最近学习运行异常',lastManagerError:'管理最近学习运行异常',methodId:'方法引用',kind:'缺口种类',sourcePairId:'来源条件结果对',conditionIndex:'条件项序号',decompositionSource:'分解来源',childRelation:'子需求组织关系',startupStage:'装配阶段',scene:'自我所在场景',source:'材料来源',gaps:'条件缺口',roleId:'旧参数角色',selection:'方法选择',actionId:'正式动作引用',actualStateIds:'实际状态引用',assessmentId:'影响评估引用',diagnostic:'工程诊断',concept:'存在概念',retained:'保留上次成功材料',lastSuccessfulAt:'本区块上次成功采集时间'});
let controlSnapshot=null,controlSelection=null,controlRefreshing=false,controlClosed=false,controlAbort=null;
Object.assign(controlLabels,{feature:'实例特征引用',value:'实际值',scope:'覆盖范围',workSnapshot:'学习活动工作',thread:'学习线程',items:'工作/记录列表',queuedCount:'排队中数量',runningCount:'执行中数量',awaitingAcknowledgementCount:'已结束待确认数量',currentMethodId:'当前方法引用',queuePosition:'FIFO排队位置',archiveId:'问题归档引用',taskId:'任务引用',timeSemantic:'时间语义',timeNs:'发生或区间结束时间（纳秒）',startTimeNs:'区间开始（UTC纳秒）',endTimeNs:'区间结束（UTC纳秒）',latestEvidenceAt:'本次范围最近执行证据（UTC纳秒）',subjectId:'被描述存在/动作主体引用',fixedValue:'固定准确值',stateIds:'正式状态引用',memberDynamicIds:'成员动态引用',actionDefinitionId:'动作定义引用',actionSourceId:'动作来源引用',targetKind:'动作目标类别',targetId:'动作目标引用',targetFeatureId:'动作目标特征引用',actionResults:'动作显式结果',relation:'关联类型',resultDynamicId:'结果动态引用',taskIds:'来源任务引用',executionIds:'来源执行引用',sources:'任务与执行来源配对'});
const controlStatus=document.getElementById('control-status');
controlLabels.kind='类别';
let controlMethodSelection=null;
Object.assign(controlLabels,{methodCatalog:'正式方法目录',missingMethods:'缺失方法列表',headId:'方法头引用',head:'方法头声明（不推断与/或）',virtualExistence:'方法虚拟存在引用',implementation:'实现方式',behavior:'读取/改变',registrationName:'正式函数登记名',sourceMethodId:'来源方法引用',instanceRootId:'实例组织根引用',parentMethodId:'父代方法引用',pairCount:'配对引用数量（未逐对核验）',root:'正式方法组织根',format:'参数规格格式',requiresScene:'要求场景',objectKind:'旧规格对象种类',features:'旧特征规格',secondaryConcepts:'二次特征概念',collectionConstraint:'集合成员约束',collectionRoleId:'集合参数角色',existenceConcepts:'作用存在概念',featureTypes:'涉及特征类型',possibleResults:'可能结果概念',sourceDemandId:'来源需求引用'});
function controlText(value){if(value===null||value===undefined)return '尚无材料';if(typeof value==='boolean')return value?'是':'否';if(typeof value==='object'&&typeof value.id==='string'&&typeof value.label==='string')return value.label+' · #'+value.id;return String(value);}
function controlNode(tag,text,cls){const node=document.createElement(tag);if(text!==undefined)node.textContent=text;if(cls)node.className=cls;return node;}
function controlFields(container,value,depth=0){if(depth>14){container.append(controlNode('p','内容层级过深，停止页面展开','warning'));return;}if(value===null||typeof value!=='object'){container.append(controlNode('p',controlText(value),'muted'));return;}if(typeof value.id==='string'&&typeof value.label==='string'&&Object.keys(value).length===2){container.append(controlNode('p',controlText(value)));return;}if(Array.isArray(value)){if(!value.length)container.append(controlNode('p','已取得：无项目','muted'));for(const item of value){const part=controlNode('div',undefined,'object');controlFields(part,item,depth+1);container.append(part);}return;}const dl=controlNode('dl');for(const [key,item] of Object.entries(value)){dl.append(controlNode('dt',controlLabels[key]||key));const dd=controlNode('dd');if(item!==null&&typeof item==='object'){const detail=controlNode('details',undefined,'object');detail.open=depth<2;detail.append(controlNode('summary',Array.isArray(item)?'查看项目（'+item.length+'）':'查看材料'));controlFields(detail,item,depth+1);dd.append(detail);}else dd.textContent=controlText(item);dl.append(dd);}container.append(dl);}
function controlTime(ns){if(typeof ns!=='string')return '无可比较实际时刻';try{const ms=BigInt(ns)/1000000n;if(ms>=-8640000000000000n&&ms<=8640000000000000n)return new Date(Number(ms)).toISOString()+' · '+ns+' ns';}catch{}return ns+' ns';}
function controlLearning(snapshot){const area=document.getElementById('control-learning');area.replaceChildren();const block=snapshot.learning?.workSnapshot;area.append(controlNode('p','学习活动工作：'+controlText(block?.status),'notice'));if(block?.reason)area.append(controlNode('p',block.reason,'warning'));if(block?.scope)area.append(controlNode('p',block.scope,'hint'));if(block?.retained)area.append(controlNode('p','以下为上次成功材料，采集于 '+controlText(block.lastSuccessfulAt),'warning'));if(block){const {items,...meta}=block;controlFields(area,meta);}if(Array.isArray(block?.items)){if(!block.items.length)area.append(controlNode('p',block.retained?'上次读取没有活动工作':'已取得：当前没有活动学习工作','muted'));for(const item of block.items){const detail=controlNode('details',undefined,'object');detail.append(controlNode('summary',controlText(item.state)+' · '+controlText(item.kind)+' · '+controlText(item.methodId)+(item.queuePosition?' · FIFO #'+item.queuePosition:'')));controlFields(detail,item);area.append(detail);}}area.append(controlNode('p','“已结束待确认”只表示工作已返回或异常结束，不表示学习成功或条件结果对已发表。','notice'));if(snapshot.learning){const {workSnapshot,...feedback}=snapshot.learning;const detail=controlNode('details',undefined,'object');detail.append(controlNode('summary','自我／任务管理线程的学习反馈'));controlFields(detail,feedback);area.append(detail);}area.append(controlNode('p','正式方法目录与缺失方法头在独立区域展示；完整学习历史仍未接入。','notice'));}
function controlTimeline(snapshot){const area=document.getElementById('control-timeline');area.replaceChildren();const block=snapshot.timeline;area.append(controlNode('p','执行证据读取：'+controlText(block?.status),'notice'));if(block?.scope)area.append(controlNode('p',block.scope,'hint'));if(block?.reason)area.append(controlNode('p',block.reason,'warning'));if(block?.retained)area.append(controlNode('p','本轮暂不可用，以下为上次成功材料，采集于 '+controlText(block.lastSuccessfulAt),'warning'));area.append(controlNode('p','本次范围最近执行证据：'+controlTime(block?.latestEvidenceAt),'hint'));area.append(controlNode('p','按正式UTC时刻倒序；同刻排序不表示因果。动作不等于效果，记录不等于任务完成。三世界完整历史未接入。','notice'));if(!Array.isArray(block?.items))return;if(!block.items.length)area.append(controlNode('p',block.retained?'上次读取范围内没有执行证据':'已取得：本轮任务引用范围内没有执行证据','muted'));for(const item of block.items){const detail=controlNode('details',undefined,'object');const when=item.timeSemantic==='实例绝对UTC'?controlTime(item.timeNs):'无可比较实际时刻（'+controlText(item.timeSemantic)+'）';detail.append(controlNode('summary',controlText(item.kind)+' · '+when+' · #'+item.id));controlFields(detail,item);area.append(detail);}}
)HTML";
    页面 += R"HTML(
function controlMethodDirectory(snapshot){const areas=[['methodCatalog','control-method-catalog','正式方法目录'],['missingMethods','control-missing-methods','待形成能力／缺失方法头']];for(const [key,id,title] of areas){const area=document.getElementById(id),block=snapshot[key];area.replaceChildren();area.append(controlNode('h3',title,'section-label'));area.append(controlNode('p','读取状态：'+controlText(block?.status),'notice'));if(block?.scope)area.append(controlNode('p',block.scope,'hint'));if(block?.reason)area.append(controlNode('p',block.reason,'warning'));if(block?.retained)area.append(controlNode('p','显示上次成功材料，采集于 '+controlText(block.lastSuccessfulAt),'warning'));const items=Array.isArray(block?.items)?block.items:null;if(items){area.append(controlNode('p','本份材料共 '+items.length+' 项','hint'));if(!items.length)area.append(controlNode('p',block.retained?'上次成功材料中无项目':'已取得：当前无项目','muted'));for(const item of items){let label;if(key==='methodCatalog')label=controlText(item.label)+' · '+(item.enabled?'启用':'停用')+' · '+(item.sourceMethodId?'执行副本':'长期方法')+' · '+(item.pairCount==='0'?'尚无配对引用':item.pairCount+' 个配对引用');else label=(item.head?.featureTypes||[]).map(controlText).join('／')+' · 待形成能力';const button=controlNode('button',label+' · #'+item.id,'task');button.type='button';if(controlMethodSelection?.key===key&&controlMethodSelection.id===item.id)button.classList.add('selected');button.addEventListener('click',()=>{controlMethodSelection={key,id:item.id};controlMethodDirectory(controlSnapshot);});area.append(button);}}area.append(controlNode('p',key==='methodCatalog'?'启用不等于适用或可执行；配对只计已发布引用，未逐对核验。':'仅缺失能力记录，尚非已掌握方法；未进入方法树，不提供执行或晋升操作。','notice'));}const detail=document.getElementById('control-method-detail');detail.replaceChildren();if(!controlMethodSelection){detail.append(controlNode('p','选择方法或缺失方法头，查看同份快照的详情。','muted'));return;}const block=snapshot[controlMethodSelection.key],item=block?.items?.find(x=>x.id===controlMethodSelection.id);if(!item){detail.append(controlNode('p','本次快照未包含原选中对象；不改选其它对象。','notice'));return;}detail.append(controlNode('h3',(controlMethodSelection.key==='methodCatalog'?'方法':'缺失方法头')+' · #'+item.id));if(block.retained)detail.append(controlNode('p','以下为上次成功材料，采集于 '+controlText(block.lastSuccessfulAt),'warning'));controlFields(detail,item);if(controlMethodSelection.key==='methodCatalog'&&Array.isArray(item.parameters)&&!item.parameters.length)detail.append(controlNode('p','正式参数规格：无参数。','hint'));}
)HTML";
    页面 += R"HTML(
function controlSummary(d){const t=d.target;if(!t)return '待完善目标';return [controlText(t.existence),controlText(t.featureType),t.form,t.direction||controlText(t.valueDomain)].filter(Boolean).join(' · ');}
function controlDetail(){const area=document.getElementById('control-detail');area.replaceChildren();if(!controlSelection){area.append(controlNode('p','选择需求或任务查看详情。','muted'));return;}const source=controlSelection.kind==='demand'?controlSnapshot?.demands:controlSnapshot?.tasks;const item=source?.items?.find(x=>x.id===controlSelection.id);document.getElementById('detail-title').textContent=(controlSelection.kind==='demand'?'需求':'任务')+' · #'+controlSelection.id;if(!item){area.append(controlNode('p','本次快照未包含该对象；保留原选择，不替换为其它对象。','notice'));return;}controlFields(area,item);if(controlSelection.kind==='task'){area.append(controlNode('p','方法返回不等于目标满足。候选排除理由、完整因果轨迹尚未接入。','notice'));}}
function controlButton(item,kind,text){const button=controlNode('button',text,kind==='task'?'task':'');button.type='button';if(controlSelection?.kind===kind&&controlSelection.id===item.id)button.classList.add('selected');button.addEventListener('click',()=>{controlSelection={kind,id:item.id};controlRender(controlSnapshot);});return button;}
function controlRender(snapshot){controlSnapshot=snapshot;const overview=document.getElementById('overview');overview.replaceChildren();for(const [key,value] of Object.entries(snapshot.overview||{})){const card=controlNode('div',undefined,'metric');card.append(controlNode('div',controlLabels[key]||key,'caption'));if(value!==null&&typeof value==='object'){controlFields(card,value);}else card.append(controlNode('div',controlText(value),'value'));overview.append(card);}if(snapshot.receivedDemandCognition){const card=controlNode('div',undefined,'metric');card.append(controlNode('div','真实接收需求认知','caption'));controlFields(card,snapshot.receivedDemandCognition);overview.append(card);}if(!overview.childNodes.length)overview.append(controlNode('p','运行总览尚无材料','notice'));document.getElementById('control-meta').textContent='采集开始：'+controlText(snapshot.readStartedAt)+' ｜ 完成：'+controlText(snapshot.readCompletedAt)+' ｜ '+controlText(snapshot.consistency);
const tree=document.getElementById('control-tree');tree.replaceChildren();const demands=snapshot.demands,tasks=snapshot.tasks;tree.append(controlNode('p','需求：'+controlText(demands?.status)+'；任务：'+controlText(tasks?.status),'hint'));if(demands?.reason)tree.append(controlNode('p',demands.reason,'warning'));if(tasks?.reason)tree.append(controlNode('p',tasks.reason,'warning'));const ds=Array.isArray(demands?.items)?demands.items:[],ts=Array.isArray(tasks?.items)?tasks.items:[],shownTasks=new Set();const self=ds.filter(d=>d.executionDomain==='自我执行');const index=new Map(self.map(d=>[d.id,d]));const visited=new Set();
function addDemand(parent,d,path){const li=controlNode('li');parent.append(li);if(path.has(d.id)){li.append(controlNode('p','父关系出现循环，停止展开 #'+d.id,'warning'));return;}if(visited.has(d.id))return;visited.add(d.id);li.append(controlButton(d,'demand',controlSummary(d)));li.append(controlNode('div',(d.complete?'完整':'待完善')+' · '+controlText(d.activity)+' · '+controlText(d.sourceType),'hint'));const children=controlNode('ul');for(const t of ts.filter(t=>t.demandId===d.id)){children.append(controlButton(t,'task','任务 #'+t.id+' · '+controlText(t.state)));shownTasks.add(t.id);}const next=new Set(path);next.add(d.id);for(const child of self.filter(x=>x.parentId===d.id))addDemand(children,child,next);if(children.childNodes.length)li.append(children);}
tree.append(controlNode('h3','自我需求树','section-label'));const roots=controlNode('ul');for(const d of self.filter(d=>d.parentId===null||d.parentId===undefined||!index.has(d.parentId))){if(d.parentId)roots.append(controlNode('p','本轮未定位父节点 #'+d.parentId,'warning'));addDemand(roots,d,new Set());}for(const d of self)if(!visited.has(d.id))addDemand(roots,d,new Set());tree.append(roots);if(!self.length)tree.append(controlNode('p',demands?.status==='已取得'?'已取得：无自我需求':'尚无可用自我需求材料','muted'));tree.append(controlNode('p','子需求“与／或”组织关系未接入；不由树形排列推断。','notice'));const others=ds.filter(d=>d.executionDomain!=='自我执行');if(others.length){tree.append(controlNode('h3','被认知需求 / 其它执行域（不入自我树）','section-label'));for(const d of others)tree.append(controlButton(d,'demand',controlText(d.executionDomain)+' · '+controlSummary(d)));}const unbound=ts.filter(t=>!shownTasks.has(t.id));if(unbound.length){tree.append(controlNode('h3','本轮未关联到自我树的任务','section-label'));for(const t of unbound)tree.append(controlButton(t,'task','任务 #'+t.id+' · '+controlText(t.state)));}
controlDetail();controlLearning(snapshot);controlMethodDirectory(snapshot);controlTimeline(snapshot);}
function controlRetainSections(snapshot){const retain=(block,previous)=>{if(block?.status==='已取得'){block.lastSuccessfulAt=snapshot.readCompletedAt;}else if(block&&Array.isArray(previous?.items)){block.items=previous.items;block.retained=true;block.lastSuccessfulAt=previous.lastSuccessfulAt||controlSnapshot.readCompletedAt;for(const key of ['thread','queuedCount','runningCount','awaitingAcknowledgementCount','latestEvidenceAt','root'])if(key in previous)block[key]=previous[key];block.reason=(block.reason||'本次读取未完成')+'；本区块保留上次成功材料，采集于 '+block.lastSuccessfulAt;}};for(const key of ['demands','tasks','timeline','methodCatalog','missingMethods'])retain(snapshot[key],controlSnapshot?.[key]);retain(snapshot.learning?.workSnapshot,controlSnapshot?.learning?.workSnapshot);return snapshot;}
async function controlRefresh(){if(controlRefreshing||controlClosed)return;controlRefreshing=true;const abort=new AbortController();controlAbort=abort;const timer=setTimeout(()=>abort.abort(),4000);try{const response=await fetch(base+'/api/control',{cache:'no-store',signal:abort.signal});if(!response.ok)throw new Error('HTTP '+response.status);const data=await response.json();if(typeof data.status!=='string'||typeof data.sequence!=='string'||typeof data.ageMs!=='string')throw new Error('快照结构不一致');controlStatus.textContent='控制快照：'+data.status+' · #'+data.sequence+' · 距采集 '+data.ageMs+' ms'+(data.collecting?' · 正在采集':'')+(data.stale?' · 快照已陈旧':'');controlStatus.className=data.stale||data.status==='采集失败'?'warning':'';if(data.snapshot!==null)controlRender(controlRetainSections(data.snapshot));}catch(error){controlStatus.textContent='控制快照读取失败：'+(error.name==='AbortError'?'请求超时':error.message)+'；保留上次显示，不影响消息交互';controlStatus.className='warning';}finally{clearTimeout(timer);if(controlAbort===abort)controlAbort=null;controlRefreshing=false;if(!controlClosed)setTimeout(controlRefresh,2000);}}
window.addEventListener('pagehide',()=>{controlClosed=true;controlAbort?.abort();outputClosed=true;outputReady=false;outputSubmitAbort?.abort();outputStateAbort?.abort();renderOutputReceipts();});
controlRefresh();
function render(items){messages.replaceChildren();if(!items.length){const e=document.createElement('div');e.className='empty';e.textContent='尚无对话消息';messages.append(e);return;}for(const item of items){const e=document.createElement('div');e.className='message '+item.role;e.textContent=item.text;messages.append(e);}messages.scrollTop=messages.scrollHeight;}
let refreshing=false;
async function refresh(){if(refreshing||outputClosed)return;refreshing=true;try{if(!dialogueId)throw new Error();const headers={'X-HY-Dialogue-Id':dialogueId};const outbound=await fetch(base+'/api/outbound',{cache:'no-store',headers});if(outbound.status!==204){if(!outbound.ok)throw new Error();await outbound.json();}const r=await fetch(base+'/api/messages',{cache:'no-store',headers});if(!r.ok)throw new Error();render((await r.json()).messages);outputReady=true;renderOutputReceipts();status.textContent='本机服务可访问';await outputRefreshStates();}catch{status.textContent=dialogueId?'本机服务取件异常':'浏览器不支持安全对话框标识';}finally{refreshing=false;}}
async function poll(){if(outputClosed)return;await refresh();if(!outputClosed)setTimeout(poll,750);}
form.addEventListener('submit',async e=>{e.preventDefault();const text=input.value;if(!text)return;if(!dialogueId){status.textContent='浏览器不支持安全对话框标识，不能提交';return;}const button=form.querySelector('button[type=submit]');button.disabled=true;try{const r=await fetch(base+'/api/messages',{method:'POST',headers:{'Content-Type':'text/plain;charset=utf-8','X-HY-Dialogue-Id':dialogueId},body:text});if(!r.ok)throw new Error();input.value='';await refresh();}catch{status.textContent='消息提交失败';}finally{button.disabled=false;input.focus();}});
input.addEventListener('keydown',e=>{if(e.key==='Enter'&&!e.shiftKey){e.preventDefault();form.requestSubmit();}});
document.getElementById('close').addEventListener('click',async()=>{controlClosed=true;controlAbort?.abort();outputClosed=true;outputReady=false;outputSubmitAbort?.abort();outputStateAbort?.abort();renderOutputReceipts();try{await fetch(base+'/api/close',{method:'POST'});}finally{const ended=controlNode('main');ended.append(controlNode('h1','交互宿主已结束'),controlNode('p','现在可以关闭此页面。'));document.body.replaceChildren(ended);}});
)HTML";
    页面 += R"HTML(
const serviceForm=document.getElementById('service-form');
const serviceFields=document.getElementById('service-fields');
const serviceMode=document.getElementById('service-mode');
const serviceSubmit=document.getElementById('service-submit');
const serviceConfirm=document.getElementById('service-confirm');
const serviceStatus=document.getElementById('service-status');
const servicePreview=document.getElementById('service-preview');
let previewId=null,requestId=null,sourceId=crypto.randomUUID(),formRevision=0,serviceBusy=false;
const serviceRefresh=document.getElementById('service-refresh');
function invalidatePreview(){formRevision++;previewId=null;serviceConfirm.disabled=true;servicePreview.textContent='';serviceStatus.textContent='输入尚未保存；补充将继续使用原要求身份，不会清空已知字段。';}
function selectServiceMode(){for(const name of ['domain','comparison','direction']){const field=serviceForm.elements.namedItem(name);const enabled=name==='direction'?serviceMode.value==='2':serviceMode.value==='1';field.disabled=!enabled;field.required=false;document.getElementById(name+'-label').hidden=!enabled;}}
serviceForm.addEventListener('input',()=>{invalidatePreview();selectServiceMode();});
serviceForm.addEventListener('change',()=>{invalidatePreview();selectServiceMode();});
async function serviceRequest(path,body){const options={cache:'no-store'};if(body!==undefined){options.method='POST';options.headers={'Content-Type':'application/x-www-form-urlencoded'};options.body=body.toString();}const response=await fetch(base+'/api/service/'+path,options);const data=await response.json();if(!response.ok)throw new Error(data.status==='需重新确认'?'语义已变化，请重新生成预览并确认':data.status||'服务要求处理失败');return data;}
function displayServiceInfo(info){if(!info||typeof info.preview!=='string'||typeof info.object!=='string'||typeof info.target!=='string'||typeof info.text!=='string')throw new Error('服务回执结构不一致');servicePreview.textContent='需求主体编码：'+info.object+'\n目标存在编码：'+info.target+'\n理解编码：'+info.preview+'\n'+info.text;}
async function loadServiceOptions(){try{const data=await serviceRequest('options');document.getElementById('service-object').value=data.object;document.getElementById('service-hints').textContent='需求主体 #'+data.object+' 的当前特征提示（非任意目标）：'+(data.hints.length?data.hints.map(h=>'FT '+h.ft+' / 当前概念 '+h.concept).join('；'):'无现有特征提示');serviceFields.disabled=false;serviceSubmit.disabled=false;serviceStatus.textContent='可先保存部分信息。缺项由你补充，不会自动推测。';selectServiceMode();}catch(error){serviceStatus.textContent=error.message+'；结构化输入不可用，文本对话仍可使用。';}}
function deliveryText(state){switch(state){case '等待后继消费':return '内容已确认，尚待后继消费，未交自我承接。';case '消息已接纳':return '消息已接纳，不代表自我已承接或任务已建立。';case '暂未接纳':return '内容确认保留，本次消息暂未接纳；可重新提交或确认以重试。';case '未尝试':return '本次未投递消息；刷新只读取材料。';default:throw new Error('材料投递回执结构不一致');}}
function displayPartial(data,delivery=data.delivery){if(data.request){requestId=data.request;serviceRefresh.disabled=false;}previewId=null;serviceConfirm.disabled=true;const lines=['要求编码：'+(data.request||'尚未建立'),'需求主体编码：'+(data.object||'未知'),'目标存在编码：'+(data.target||'尚未明确'),'处理：'+data.status,'缺项：'+((data.missing||[]).join('、')||'无'),'冲突：'+((data.conflicts||[]).join('、')||'无')];if(data.text)lines.push(data.text);if(data.confirmation)lines.push('内容已确认；确认编码：'+data.confirmation);servicePreview.textContent=lines.join('\n');if(data.preview&&!data.confirmation&&data.status!=='需重新确认'){previewId=data.preview;serviceConfirm.disabled=false;}serviceStatus.textContent=data.status==='需重新确认'?'概念含义已变化，原要求保留且暂停执行；本轮不自动改写或刷新确认。':deliveryText(delivery);}
async function partialRequest(body){const options={cache:'no-store'};let path='request?id='+encodeURIComponent(requestId);if(body){path='request';options.method='POST';options.headers={'Content-Type':'application/x-www-form-urlencoded'};options.body=body.toString();}const response=await fetch(base+'/api/service/'+path,options);const data=await response.json();if(!response.ok&&!data.request)throw new Error(data.status||'要求处理失败');return data;}
serviceForm.addEventListener('submit',async event=>{event.preventDefault();if(serviceBusy)return;const body=new URLSearchParams(new FormData(serviceForm));body.set('source',sourceId);if(requestId)body.set('request',requestId);serviceBusy=true;serviceSubmit.disabled=true;serviceFields.disabled=true;serviceConfirm.disabled=true;try{displayPartial(await partialRequest(body));}catch(error){serviceStatus.textContent=error.message+'；保留来源标识，可用相同输入重试。';}finally{serviceBusy=false;serviceSubmit.disabled=false;serviceFields.disabled=false;selectServiceMode();}});
serviceRefresh.addEventListener('click',async()=>{if(!requestId||serviceBusy)return;serviceBusy=true;try{displayPartial(await partialRequest());}catch(error){serviceStatus.textContent=error.message;}finally{serviceBusy=false;}});
document.getElementById('service-new').addEventListener('click',()=>{if(serviceBusy)return;serviceForm.reset();requestId=null;sourceId=crypto.randomUUID();serviceRefresh.disabled=true;invalidatePreview();selectServiceMode();});
serviceConfirm.addEventListener('click',async()=>{if(!previewId||serviceBusy)return;const confirming=previewId;previewId=null;serviceBusy=true;serviceConfirm.disabled=true;serviceSubmit.disabled=true;serviceFields.disabled=true;try{const data=await serviceRequest('confirm',new URLSearchParams({preview:confirming}));displayServiceInfo(data.info);serviceStatus.textContent='内容已确认；'+deliveryText(data.delivery);if(requestId){try{displayPartial(await partialRequest(),data.delivery);}catch(error){serviceStatus.textContent='内容已确认；'+deliveryText(data.delivery)+' 材料刷新失败：'+error.message;}}}catch(error){serviceStatus.textContent=error.message+'；原要求保留，可刷新状态。';}finally{serviceBusy=false;serviceSubmit.disabled=false;serviceFields.disabled=false;selectServiceMode();}});
loadServiceOptions();
poll();
</script>
</body>
</html>)HTML";
    return 页面;
}

bool 生成会话令牌(std::string& 令牌) {
    std::array<unsigned char, 16> 随机{};
    if (BCryptGenRandom(nullptr, 随机.data(), static_cast<ULONG>(随机.size()),
            BCRYPT_USE_SYSTEM_PREFERRED_RNG) < 0) {
        return false;
    }
    constexpr char 十六进制[] = "0123456789abcdef";
    令牌.clear();
    令牌.reserve(随机.size() * 2);
    for (const auto 字节 : 随机) {
        令牌.push_back(十六进制[字节 >> 4U]);
        令牌.push_back(十六进制[字节 & 0x0fU]);
    }
    return true;
}

bool 启动系统浏览器(std::string_view 地址) noexcept {
    try {
        const std::wstring 宽地址(地址.begin(), 地址.end());
        const auto 结果 = reinterpret_cast<INT_PTR>(ShellExecuteW(
            nullptr, L"open", 宽地址.c_str(), nullptr, nullptr, SW_SHOWNORMAL));
        return 结果 > 32;
    } catch (...) {
        return false;
    }
}

std::optional<std::uint64_t> 取得下一上行序号(
    std::string_view 会话标识) noexcept {
    try {
        std::lock_guard 锁(共享状态.互斥);
        if (!共享状态.运行中 || 共享状态.会话标识 != 会话标识
            || 共享状态.下一上行序号 == 0
            || 共享状态.下一上行序号
                > static_cast<std::uint64_t>(
                    (std::numeric_limits<std::int64_t>::max)())) {
            return std::nullopt;
        }
        return 共享状态.下一上行序号;
    } catch (...) {
        return std::nullopt;
    }
}

// 以下引用仅在持有宿主互斥期间使用，绝不跨锁或回调保存。
网页对话框传输状态* 查找传输页面(const 网页对话框目标& 目标) {
    网页对话框传输状态* 命中 = nullptr;
    for (auto& 页面 : 共享状态.页面组) {
        if (页面.目标 != 目标) continue;
        if (命中) throw std::runtime_error("duplicate dialogue transport identity");
        命中 = &页面;
    }
    return 命中;
}

void 追加页面消息(网页对话框传输状态& 页面, 网页对话消息角色 角色, std::string 正文) {
    if (页面.下一页面序号 == 0 || 页面.下一页面序号 == (std::numeric_limits<std::uint64_t>::max)()
        || 页面.消息组.size() != 页面.下一页面序号 - 1)
        throw std::runtime_error("dialogue display sequence corrupted");
    页面.消息组.push_back({页面.下一页面序号, 角色, std::move(正文)});
    ++页面.下一页面序号;
}

bool 确认传输页面(const 网页对话框目标& 目标, const 网页对话框事实回调组& 回调) {
    // 不持宿主锁进入外设适配器；返回后再核验运行身份。
    if (!回调.确认页面(目标)) return false;
    std::lock_guard 操作锁(队列操作互斥);
    std::lock_guard 锁(共享状态.互斥);
    if (!共享状态.运行中 || !共享状态.接纳提交
        || 共享状态.会话标识 != 目标.公开会话标识) return false;
    if (!查找传输页面(目标)) 共享状态.页面组.push_back({目标});
    return true;
}

void 追加交互者消息(const 网页对话框目标& 目标,
    std::uint64_t 上行序号, std::string UTF8正文) {
    std::lock_guard 锁(共享状态.互斥);
    auto* 页面 = 查找传输页面(目标);
    if (!共享状态.运行中 || 共享状态.会话标识 != 目标.公开会话标识
        || !页面 || 共享状态.下一上行序号 != 上行序号)
        throw std::runtime_error("received dialogue projection identity changed");
    追加页面消息(*页面, 网页对话消息角色::交互者, std::move(UTF8正文));
    ++共享状态.下一上行序号;
}

void 处理定向取件(SOCKET 客户端, const 网页对话框目标& 目标,
    const 网页对话框事实回调组& 回调) {
    std::optional<网页输出队列项> 事实;
    {
        std::lock_guard 操作锁(队列操作互斥);
        const 网页输出队列回调组* 队列 = nullptr;
        bool 请求停止 = false, 可领取 = true;
        {
            std::lock_guard 锁(共享状态.互斥);
            auto* 页面 = 查找传输页面(目标);
            if (!共享状态.运行中 || 共享状态.会话标识 != 目标.公开会话标识 || !页面)
                throw std::runtime_error("outbound dialogue identity invalid");
            if (!共享状态.停止信号 || !共享状态.输出队列)
                throw std::runtime_error("outbound queue/stop provider unavailable");
            请求停止 = *共享状态.停止信号 != 0;
            队列 = 共享状态.输出队列;
            可领取 = 共享状态.接纳提交 && std::none_of(页面->提交组.begin(), 页面->提交组.end(),
                [](const auto& 项) { return 项.结果.状态 == 网页定向发送状态::正在写出; });
            if (可领取) 页面->提交组.reserve(页面->提交组.size() + 1);
        }
        if (请求停止) { 停止接纳提交_已持队列锁(); 可领取 = false; }
        if (可领取) {
            // 在正式出列之前准备新投影的目标字符串及容器空间。
            网页待发送项 新索引{目标};
            auto 领取 = 队列->领取(目标);
            if (领取.状态 != 网页输出队列状态::空) {
                if (领取.状态 != 网页输出队列状态::已领取 || !领取.项
                    || 领取.项->目标 != 目标 || !领取.项->提交序号
                    || !领取.项->正式消息编码 || 领取.项->UTF8正文.empty()
                    || !是有效UTF8(领取.项->UTF8正文))
                    throw std::runtime_error("outbound formal queue receipt invalid");
                事实 = std::move(领取.项);
                std::lock_guard 锁(共享状态.互斥);
                auto* 页面 = 查找传输页面(目标);
                if (!页面) throw std::runtime_error("outbound claimed page disappeared");
                网页待发送项* 命中 = nullptr;
                for (auto& 项 : 页面->提交组) if (项.提交序号 == 事实->提交序号) {
                    if (命中) throw std::runtime_error("outbound submission duplicated");
                    命中 = &项;
                }
                if (命中) {
                    if (命中->目标 != 目标 || 命中->正式消息编码 != 事实->正式消息编码
                        || 命中->结果.提交序号 != 事实->提交序号
                        || 命中->结果.状态 != 网页定向发送状态::已排队)
                        throw std::runtime_error("outbound formal queue and transport index disagree");
                    命中->结果.状态 = 网页定向发送状态::正在写出;
                } else {
                    // 正式队列允许普通外设调用者直接生产；传输索引不是发送资格。
                    新索引.提交序号 = 事实->提交序号;
                    新索引.正式消息编码 = 事实->正式消息编码;
                    新索引.结果 = {网页定向发送状态::正在写出, 事实->提交序号};
                    页面->提交组.push_back(std::move(新索引));
                }
            } else if (领取.项) throw std::runtime_error("empty outbound queue carried a message");
        }
    }
    if (!事实) {
        (void)发送响应(客户端, "204 No Content", "application/json;charset=utf-8", "");
        return;
    }
    const std::string 正文 = "{\"id\":\"" + std::to_string(事实->提交序号)
        + "\",\"text\":\"" + JSON转义(事实->UTF8正文) + "\"}";
    const 网页定向发送事实 发送事实{事实->目标, 事实->提交序号, 事实->UTF8正文};
    const auto 写出 = 发送响应(客户端, "200 OK", "application/json;charset=utf-8", 正文);
    if (写出.完整写出) 回调.发送完成(发送事实);
    {
        std::lock_guard 锁(共享状态.互斥);
        auto* 页面 = 查找传输页面(目标);
        if (!共享状态.运行中 || 共享状态.会话标识 != 目标.公开会话标识 || !页面)
            throw std::runtime_error("outbound result dialogue disappeared");
        网页待发送项* 命中 = nullptr;
        for (auto& 项 : 页面->提交组) {
            if (项.提交序号 != 事实->提交序号) continue;
            if (命中) throw std::runtime_error("outbound submission duplicated");
            命中 = &项;
        }
        if (!命中 || 命中->结果.状态 != 网页定向发送状态::正在写出
            || 命中->目标 != 目标 || 命中->正式消息编码 != 事实->正式消息编码)
            throw std::runtime_error("outbound result payload changed");
        命中->结果.状态 = 写出.完整写出 ? 网页定向发送状态::已完整写出 : 网页定向发送状态::写出未完成;
        命中->结果.HTTP已写字节数 = 写出.已写字节数;
        命中->结果.系统错误 = 写出.系统错误;
        if (写出.完整写出) 追加页面消息(*页面, 网页对话消息角色::自我, 事实->UTF8正文);
    }
}

// 只投影正式回执；工程异常向上送出到宿主唯一诊断边界。
std::string 构造控制输出JSON(const 网页定向发送结果& 结果, bool 已接纳) {
    using S = 网页定向发送状态;
    std::string_view 状态;
    switch (结果.状态) {
    case S::已排队: 状态 = "已排队"; break;
    case S::正在写出: 状态 = "正在写出"; break;
    case S::已完整写出: 状态 = "已完整写出"; break;
    case S::写出未完成: 状态 = "写出未完成"; break;
    case S::已取消: 状态 = "已取消"; break;
    case S::输入拒绝: 状态 = "输入拒绝"; break;
    case S::宿主未运行: 状态 = "宿主未运行"; break;
    case S::会话失效: 状态 = "会话失效"; break;
    case S::对话框未登记: 状态 = "对话框未登记"; break;
    case S::提交不存在: 状态 = "提交不存在"; break;
    case S::页面被占用: 状态 = "页面被占用"; break;
    default: throw std::runtime_error("control output unknown formal state");
    }
    const bool 正式记录 = 结果.状态 == S::已排队 || 结果.状态 == S::正在写出
        || 结果.状态 == S::已完整写出 || 结果.状态 == S::写出未完成 || 结果.状态 == S::已取消;
    if (已接纳 != 正式记录 || (已接纳 && (!结果.提交序号 || !*结果.提交序号))
        || (!已接纳 && 结果.提交序号))
        throw std::runtime_error("control output receipt identity/state mismatch");
    return "{\"status\":\"" + std::string(状态) + "\",\"submissionId\":"
        + (结果.提交序号 ? "\"" + std::to_string(*结果.提交序号) + "\"" : "null")
        + ",\"httpBytes\":\"" + std::to_string(结果.HTTP已写字节数)
        + "\",\"systemError\":" + std::to_string(结果.系统错误)
        + ",\"accepted\":" + (已接纳 ? "true" : "false") + ",\"automaticRetry\":false}";
}

// 不持宿主/队列锁调用正式提交或查询；不创建页面、不生成发送事实。
bool 处理控制台输出请求(SOCKET 客户端, const HTTP请求& 请求,
    std::string_view 基础路径, std::string_view 公开会话标识, std::string_view 页面Origin) {
    const std::string 提交路径 = std::string(基础路径) + "/api/control/output";
    const std::string 状态路径 = 提交路径 + "/status";
    const bool 提交 = 请求.目标 == 提交路径;
    if (!提交 && !请求.目标.starts_with(状态路径)) return false;
    const auto 拒绝 = [&](std::string_view HTTP状态, const 网页定向发送结果& 结果 = {}) {
        (void)发送响应(客户端, HTTP状态, "application/json;charset=utf-8",
            构造控制输出JSON(结果, false));
        return true;
    };
    if ((提交 && 请求.方法 != "POST") || (!提交 && 请求.方法 != "GET"))
        return 拒绝("400 Bad Request");
    if (!页面Origin.starts_with("http://127.0.0.1:"))
        throw std::runtime_error("control output trusted origin missing");
    if (请求.控制安全头无效 || !请求.对话框标识 || !请求.主机 || !请求.控制输出标记)
        return 拒绝("400 Bad Request");
    if (*请求.主机 != 页面Origin.substr(7) || *请求.控制输出标记 != "1"
        || (请求.来源 && *请求.来源 != 页面Origin)
        || (提交 && !请求.来源)) return 拒绝("403 Forbidden");
    std::uint64_t 序号 = 0;
    if (提交) {
        if (!请求.有正文长度) return 拒绝("400 Bad Request");
        if (请求.内容类型 != "text/plain;charset=utf-8") return 拒绝("415 Unsupported Media Type");
        if (请求.正文.size() > 最大消息字节数) return 拒绝("413 Payload Too Large");
        if (请求.正文.empty() || !是有效UTF8(请求.正文)) return 拒绝("422 Unprocessable Content");
    } else {
        if (!请求.正文.empty()) return 拒绝("400 Bad Request");
        const std::string 前缀 = 状态路径 + "?id=";
        if (!请求.目标.starts_with(前缀)) return 拒绝("400 Bad Request");
        const std::string_view ID = std::string_view(请求.目标).substr(前缀.size());
        if (ID.empty() || ID.front() < '1' || ID.front() > '9'
            || !std::all_of(ID.begin(), ID.end(), [](char 字符) { return 字符 >= '0' && 字符 <= '9'; }))
            return 拒绝("400 Bad Request");
        const auto 转换 = std::from_chars(ID.data(), ID.data() + ID.size(), 序号);
        if (转换.ec != std::errc{} || 转换.ptr != ID.data() + ID.size() || !序号)
            return 拒绝("400 Bad Request");
    }
    const 网页对话框目标 目标{std::string(公开会话标识), *请求.对话框标识};
    const auto 结果 = 提交 ? 提交网页对话框消息({目标, 请求.正文, std::nullopt})
        : 查询网页对话框发送(目标, 序号);
    using S = 网页定向发送状态;
    switch (结果.状态) {
    case S::已排队: case S::正在写出: case S::已完整写出: case S::写出未完成: case S::已取消:
        if ((提交 && 结果.状态 != S::已排队)
            || (!提交 && 结果.提交序号 != std::optional<std::uint64_t>{序号}))
            throw std::runtime_error("control output returned another submission or unexpected transition");
        (void)发送响应(客户端, 提交 ? "202 Accepted" : "200 OK", "application/json;charset=utf-8",
            构造控制输出JSON(结果, true));
        return true;
    case S::输入拒绝: return 拒绝("422 Unprocessable Content", 结果);
    case S::宿主未运行: return 拒绝("503 Service Unavailable", 结果);
    case S::会话失效: return 拒绝("410 Gone", 结果);
    case S::对话框未登记: case S::提交不存在: return 拒绝("404 Not Found", 结果);
    case S::页面被占用: return 拒绝("409 Conflict", 结果);
    default: throw std::runtime_error("control output invalid formal operation result");
    }
}

bool 处理请求(SOCKET 客户端,
    std::string_view 基础路径,
    std::string_view 会话标识,
    std::string_view 页面Origin,
    const std::string& 页面,
    bool& 请求结束,
    const 网页对话消息接收函数& 消息接收,
    const 网页服务输入回调组& 服务输入,
    const 网页对话框事实回调组& 对话框事实,
    const 网页控制面板回调组& 控制面板) {
    HTTP请求 请求;
    const auto 读取状态 = 读取请求(客户端, 请求);
    if (读取状态 == 请求读取状态::请求过大) {
        发送响应(客户端, "413 Payload Too Large", "text/plain;charset=utf-8", "请求过大");
        return true;
    }
    if (读取状态 != 请求读取状态::已读取) {
        发送响应(客户端, "400 Bad Request", "text/plain;charset=utf-8", "请求无效");
        return true;
    }

    if (处理控制台输出请求(客户端, 请求, 基础路径, 会话标识, 页面Origin)) return true;
    const std::string 消息路径 = std::string(基础路径) + "/api/messages";
    const std::string 取件路径 = std::string(基础路径) + "/api/outbound";
    const std::string 结束路径 = std::string(基础路径) + "/api/close";
    if (请求.目标 == std::string(基础路径) + "/api/control") {
        if (请求.方法 != "GET" || !请求.正文.empty()) {
            发送响应(客户端, "400 Bad Request", "text/plain;charset=utf-8", "控制快照只接受空正文GET");
            return true;
        }
        const auto 快照 = 控制面板.读取 ? 控制面板.读取() : 网页控制快照读取结果{};
        std::string_view 状态 = "未知";
        switch (快照.状态) {
        case 网页控制快照状态::未接入: 状态 = "未接入"; break;
        case 网页控制快照状态::采集中: 状态 = "采集中"; break;
        case 网页控制快照状态::已取得: 状态 = "已取得"; break;
        case 网页控制快照状态::采集失败: 状态 = "采集失败"; break;
        case 网页控制快照状态::已停止: 状态 = "已停止"; break;
        default: throw std::runtime_error("control snapshot state invalid");
        }
        // 采集中或快照过大时不回传正文，避免单线程 accept 被控制轮询占满而饿死 /api/messages。
        const bool 回传快照 = 快照.UTF8JSON && !快照.正在采集
            && 快照.UTF8JSON->size() <= 8192;
        const bool 陈旧 = !回传快照
            || (快照.UTF8JSON && 快照.距采集完成毫秒 > 10000);
        const std::string 正文 = "{\"status\":\"" + std::string(状态)
            + "\",\"sequence\":\"" + std::to_string(快照.序号)
            + "\",\"ageMs\":\"" + std::to_string(快照.距采集完成毫秒)
            + "\",\"collecting\":" + (快照.正在采集 ? "true" : "false")
            + ",\"stale\":" + (陈旧 ? "true" : "false")
            + ",\"snapshot\":" + (回传快照 ? *快照.UTF8JSON : "null") + "}";
        发送响应(客户端, "200 OK", "application/json;charset=utf-8", 正文);
        return true;
    }
    if (处理服务输入请求(客户端, 请求, 基础路径, 会话标识, 服务输入)) return true;
    if (请求.方法 == "GET" && 请求.目标 == 基础路径) {
        发送响应(客户端, "200 OK", "text/html;charset=utf-8", 页面);
        return true;
    }
    std::optional<网页对话框目标> 目标;
    if ((请求.方法 == "GET" && (请求.目标 == 消息路径 || 请求.目标 == 取件路径))
        || (请求.方法 == "POST" && 请求.目标 == 消息路径)) {
        if (!请求.对话框标识) {
            发送响应(客户端, "400 Bad Request", "text/plain;charset=utf-8", "缺少对话框标识");
            return true;
        }
        if (请求.方法 == "POST" && (请求.正文.empty() || !是有效UTF8(请求.正文))) {
            发送响应(客户端, "422 Unprocessable Content", "text/plain;charset=utf-8", "消息必须是非空UTF-8文本");
            return true;
        }
        目标 = 网页对话框目标{std::string(会话标识), *请求.对话框标识};
        if (!确认传输页面(*目标, 对话框事实)) {
            发送响应(客户端, "503 Service Unavailable", "application/json;charset=utf-8", "{\"available\":false}");
            return true;
        }
    }
    if (请求.方法 == "GET" && 请求.目标 == 取件路径) {
        处理定向取件(客户端, *目标, 对话框事实);
        return true;
    }
    if (请求.方法 == "GET" && 请求.目标 == 消息路径) {
        const auto 读取 = 读取网页对话框显示消息(*目标, 0);
        if (读取.状态 != 网页对话外设状态::已完成
            && 读取.状态 != 网页对话外设状态::无变化) {
            发送响应(客户端, "503 Service Unavailable",
                "application/json;charset=utf-8",
                "{\"available\":false}");
            return true;
        }
        发送响应(客户端, "200 OK", "application/json;charset=utf-8",
            构造消息JSON(读取.消息组));
        return true;
    }
    if (请求.方法 == "POST" && 请求.目标 == 消息路径) {
        if (!请求.对话框标识) {
            发送响应(客户端, "400 Bad Request", "text/plain;charset=utf-8", "缺少对话框标识");
            return true;
        }
        if (请求.正文.empty() || !是有效UTF8(请求.正文)) {
            发送响应(客户端, "422 Unprocessable Content", "text/plain;charset=utf-8",
                "消息必须是非空UTF-8文本");
            return true;
        }
        const auto 上行序号 = 取得下一上行序号(会话标识);
        if (!上行序号
            || (消息接收
                && !消息接收(会话标识, *请求.对话框标识, *上行序号, 请求.正文))) {
            发送响应(客户端, "503 Service Unavailable",
                "application/json;charset=utf-8",
                "{\"accepted\":false,\"retryable\":true}");
            return true;
        }
        if(消息接收&&服务输入.接收事实已记录)
            服务输入.接收事实已记录(会话标识,*请求.对话框标识,*上行序号);
        try {
            追加交互者消息(*目标, *上行序号, std::move(请求.正文));
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"发布网页接收投影",
                L"外设已接收真实消息，但宿主显示列表追加失败，保留正式事实。");
        }
        发送响应(客户端, "202 Accepted", "application/json;charset=utf-8",
            "{\"accepted\":true}");
        return true;
    }
    if (请求.方法 == "POST" && 请求.目标 == 结束路径 && 请求.正文.empty()) {
        停止接纳提交();
        请求结束 = true;
        发送响应(客户端, "200 OK", "application/json;charset=utf-8",
            "{\"closed\":true}");
        return true;
    }
    发送响应(客户端, "404 Not Found", "text/plain;charset=utf-8", "未找到");
    return true;
}

} // namespace

bool 网页对话宿主结果::成功() const noexcept {
    return 状态 == 网页对话宿主状态::用户结束
        || 状态 == 网页对话宿主状态::停止信号结束;
}

网页对话会话核验结果 核验网页对话当前会话(
    std::string_view 公开会话标识) noexcept {
    if (公开会话标识.empty() || !是有效UTF8(公开会话标识)) return {};
    try {
        std::lock_guard 锁(共享状态.互斥);
        if (!共享状态.运行中) return {网页对话会话核验状态::宿主未运行, std::nullopt};
        if (共享状态.会话标识 != 公开会话标识) return {网页对话会话核验状态::会话不匹配, std::nullopt};
        return {网页对话会话核验状态::已匹配, std::nullopt};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验网页会话", L"会话状态读取异常。");
    }
}

网页对话页面消息快照结果 读取网页对话会话页面消息快照(
    std::string_view, std::uint64_t, std::size_t, std::size_t) noexcept { return {}; }

网页对话消息读取结果 读取网页对话已显示消息(std::uint64_t) noexcept { return {}; }
网页对话外设操作结果 设置网页对话待提交文本(std::string_view) noexcept { return {}; }
网页对话外设操作结果 提交网页对话待提交文本() noexcept { return {}; }

网页定向发送结果 提交网页对话框消息(const 网页定向发送请求& 请求) noexcept {
    if (请求.目标.公开会话标识.empty() || !是有效UTF8(请求.目标.公开会话标识)
        || !是合法对话框标识(请求.目标.对话框标识) || 请求.UTF8正文.empty()
        || 请求.UTF8正文.size() > 最大消息字节数 || !是有效UTF8(请求.UTF8正文)) return {};
    try {
        std::lock_guard 操作锁(队列操作互斥);
        const 网页输出队列回调组* 队列 = nullptr;
        bool 请求停止 = false;
        {
            std::lock_guard 锁(共享状态.互斥);
            if (!共享状态.运行中) return {网页定向发送状态::宿主未运行};
            if (共享状态.会话标识 != 请求.目标.公开会话标识) return {网页定向发送状态::会话失效};
            if (!共享状态.停止信号 || !共享状态.输出队列)
                throw std::runtime_error("running dialogue lacks output queue lifetime");
            请求停止 = *共享状态.停止信号 != 0;
            if (!请求停止 && !共享状态.接纳提交) return {网页定向发送状态::宿主未运行};
            if (!请求停止) {
                auto* 页面 = 查找传输页面(请求.目标);
                if (!页面) return {网页定向发送状态::对话框未登记};
                if (页面->出站占用身份 != 请求.出站占用身份)
                    return {网页定向发送状态::页面被占用};
                页面->提交组.reserve(页面->提交组.size() + 1);
                队列 = 共享状态.输出队列;
            }
        }
        if (请求停止) {
            停止接纳提交_已持队列锁();
            return {网页定向发送状态::宿主未运行};
        }
        网页待发送项 索引{请求.目标};
        const 网页输出入列请求 输入{请求.目标, 请求.UTF8正文};
        const auto 入列 = 队列->入列(输入);
        if (入列.状态 != 网页输出队列状态::已入列
            || !入列.项 || 入列.项->目标 != 请求.目标 || !入列.项->提交序号
            || !入列.项->正式消息编码 || 入列.项->UTF8正文 != 请求.UTF8正文)
            throw std::runtime_error("outbound formal enqueue receipt invalid");
        const auto 序号 = 入列.项->提交序号;
        网页定向发送结果 结果{网页定向发送状态::已排队, 序号};
        索引.提交序号 = 序号;
        索引.正式消息编码 = 入列.项->正式消息编码;
        索引.结果 = 结果;
        {
            std::lock_guard 锁(共享状态.互斥);
            auto* 页面 = 查找传输页面(请求.目标);
            if (!页面 || std::any_of(页面->提交组.begin(), 页面->提交组.end(),
                    [&](const auto& 项) { return 项.提交序号 == 序号; }))
                throw std::runtime_error("outbound transport index changed during enqueue");
            页面->提交组.push_back(std::move(索引));
        }
        return 结果;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"提交定向网页消息", L"传输队列或载荷冻结失败。");
    }
}

bool 取得网页对话框出站占用(const 网页对话框目标& 目标,
    std::uint64_t 调用编码) noexcept {
    if (!调用编码 || 目标.公开会话标识.empty()
        || !是有效UTF8(目标.公开会话标识)
        || !是合法对话框标识(目标.对话框标识)) return false;
    try {
        std::lock_guard 操作锁(队列操作互斥);
        std::lock_guard 锁(共享状态.互斥);
        if (!共享状态.运行中 || !共享状态.接纳提交
            || !共享状态.停止信号 || *共享状态.停止信号 != 0
            || 共享状态.会话标识 != 目标.公开会话标识) return false;
        auto* 页面 = 查找传输页面(目标);
        if (!页面 || 页面->出站占用身份) return false;
        for (const auto& 项 : 页面->提交组) {
            if (项.结果.状态 == 网页定向发送状态::已排队
                || 项.结果.状态 == 网页定向发送状态::正在写出) return false;
        }
        页面->出站占用身份 = 调用编码;
        return true;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"取得网页出站占用", L"页面传输结构读取失败。");
    }
}

bool 核验网页对话框出站占用可用(const 网页对话框目标& 目标,
    std::uint64_t 调用编码) noexcept {
    if (!调用编码 || 目标.公开会话标识.empty()
        || !是有效UTF8(目标.公开会话标识)
        || !是合法对话框标识(目标.对话框标识)) return false;
    try {
        std::lock_guard 操作锁(队列操作互斥);
        std::lock_guard 锁(共享状态.互斥);
        if (!共享状态.运行中 || !共享状态.接纳提交
            || !共享状态.停止信号 || *共享状态.停止信号 != 0
            || 共享状态.会话标识 != 目标.公开会话标识) return false;
        const auto* 页面 = 查找传输页面(目标);
        return 页面 && 页面->出站占用身份 == 调用编码;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验网页出站占用",
            L"页面租约或会话状态读取异常。");
    }
}

bool 释放网页对话框出站占用(const 网页对话框目标& 目标,
    std::uint64_t 调用编码) noexcept {
    if (!调用编码 || 目标.公开会话标识.empty()
        || !是有效UTF8(目标.公开会话标识)
        || !是合法对话框标识(目标.对话框标识)) return false;
    try {
        std::lock_guard 操作锁(队列操作互斥);
        std::lock_guard 锁(共享状态.互斥);
        // 旧会话已经结束或切换时，本调用旧租约已不在当前宿主中；
        // 绝不按调用编码单独触碰新会话的页面。
        if (!共享状态.运行中 || 共享状态.会话标识 != 目标.公开会话标识)
            return true;
        auto* 页面 = 查找传输页面(目标);
        if (!页面 || !页面->出站占用身份) return true;
        if (页面->出站占用身份 != 调用编码) return false;
        页面->出站占用身份.reset();
        return true;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"释放网页出站占用", L"页面传输结构读取失败。");
    }
}

网页定向发送结果 取消网页对话框提交(const 网页对话框目标& 目标,
    std::uint64_t 提交序号, std::uint64_t 占用身份) noexcept {
    if (!提交序号 || !占用身份 || 目标.公开会话标识.empty()
        || !是有效UTF8(目标.公开会话标识)
        || !是合法对话框标识(目标.对话框标识)) return {};
    try {
        std::lock_guard 操作锁(队列操作互斥);
        const 网页输出队列回调组* 队列 = nullptr;
        网页待发送项* 命中 = nullptr;
        {
            std::lock_guard 锁(共享状态.互斥);
            if (!共享状态.运行中) return {网页定向发送状态::宿主未运行};
            if (共享状态.会话标识 != 目标.公开会话标识) return {网页定向发送状态::会话失效};
            auto* 页面 = 查找传输页面(目标);
            if (!页面) return {网页定向发送状态::对话框未登记};
            if (页面->出站占用身份 != 占用身份) return {网页定向发送状态::页面被占用};
            for (auto& 项 : 页面->提交组) if (项.提交序号 == 提交序号) {
                if (命中 || 项.目标 != 目标 || 项.结果.提交序号 != 提交序号 || !项.正式消息编码)
                    throw std::runtime_error("outbound cancel item identity corrupted");
                命中 = &项;
            }
            if (!命中) return {网页定向发送状态::提交不存在};
            if (命中->结果.状态 != 网页定向发送状态::已排队) return 命中->结果;
            队列 = 共享状态.输出队列;
        }
        if (!队列) throw std::runtime_error("outbound cancellation provider missing");
        const auto 取消 = 队列->取消(目标, 提交序号);
        if (取消.状态 != 网页输出队列状态::已移除 || !取消.项
            || 取消.项->目标 != 目标 || 取消.项->提交序号 != 提交序号
            || 取消.项->正式消息编码 != 命中->正式消息编码)
            throw std::runtime_error("outbound cancellation and formal queue disagree");
        std::lock_guard 锁(共享状态.互斥);
        命中->结果.状态 = 网页定向发送状态::已取消;
        return 命中->结果;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"取消网页提交", L"页面传输结构读取失败。");
    }
}

网页定向发送结果 查询网页对话框发送(const 网页对话框目标& 目标, std::uint64_t 提交序号) noexcept {
    if (!提交序号 || 目标.公开会话标识.empty() || !是有效UTF8(目标.公开会话标识)
        || !是合法对话框标识(目标.对话框标识)) return {};
    try {
        std::lock_guard 操作锁(队列操作互斥);
        std::lock_guard 锁(共享状态.互斥);
        if (!共享状态.运行中) return {网页定向发送状态::宿主未运行};
        if (共享状态.会话标识 != 目标.公开会话标识) return {网页定向发送状态::会话失效};
        const auto* 页面 = 查找传输页面(目标);
        if (!页面) return {网页定向发送状态::对话框未登记};
        const 网页待发送项* 命中 = nullptr;
        for (const auto& 项 : 页面->提交组) {
            if (项.目标 != 目标 || 项.结果.提交序号 != 项.提交序号 || !项.正式消息编码)
                throw std::runtime_error("outbound query item identity corrupted");
            if (项.提交序号 != 提交序号) continue;
            if (命中) throw std::runtime_error("outbound query duplicate submission");
            命中 = &项;
        }
        return 命中 ? 命中->结果 : 网页定向发送结果{网页定向发送状态::提交不存在};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询定向网页消息", L"传输状态结构不一致。");
    }
}

网页对话消息读取结果 读取网页对话框显示消息(const 网页对话框目标& 目标,
    std::uint64_t 已读页面序号) noexcept {
    if (目标.公开会话标识.empty() || !是有效UTF8(目标.公开会话标识)
        || !是合法对话框标识(目标.对话框标识)) return {};
    try {
        std::lock_guard 锁(共享状态.互斥);
        if (!共享状态.运行中) return {网页对话外设状态::宿主未运行};
        if (共享状态.会话标识 != 目标.公开会话标识) return {};
        const auto* 页面 = 查找传输页面(目标);
        if (!页面) return {};
        if (!页面->下一页面序号 || 页面->消息组.size() != 页面->下一页面序号 - 1)
            throw std::runtime_error("dialogue display sequence invalid");
        网页对话消息读取结果 结果;
        结果.最新页面序号 = 页面->下一页面序号 - 1;
        if (已读页面序号 > 结果.最新页面序号) return {};
        for (std::size_t 序号 = 0; 序号 < 页面->消息组.size(); ++序号) {
            const auto& 项 = 页面->消息组[序号];
            if (项.页面序号 != 序号 + 1
                || (项.角色 != 网页对话消息角色::自我 && 项.角色 != 网页对话消息角色::交互者))
                throw std::runtime_error("dialogue display item invalid");
            if (项.页面序号 > 已读页面序号) 结果.消息组.push_back(项);
        }
        结果.状态 = 结果.消息组.empty() ? 网页对话外设状态::无变化 : 网页对话外设状态::已完成;
        return 结果;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取对话框显示消息", L"页面投影读取异常。");
    }
}

网页对话宿主结果 运行本地网页对话宿主(
    const volatile std::sig_atomic_t& 停止请求,
    const 网页外设运行回调组& 外设,
    const 网页服务输入回调组& 服务输入,
    const 网页控制面板回调组& 控制面板) noexcept {
    try {
        const auto& 输出队列 = 外设.输出队列;
        const auto& 消息接收 = 外设.接收消息;
        const 网页对话框事实回调组 对话框事实{外设.确认页面, 外设.发送完成};
        if (!输出队列.入列 || !输出队列.领取 || !输出队列.取消 || !输出队列.清空)
            return {网页对话宿主状态::内部错误};
        if (!外设.会话开始 || !外设.会话结束 || !消息接收
            || !对话框事实.确认页面 || !对话框事实.发送完成)
            return {网页对话宿主状态::会话绑定失败};
        const bool 有服务输入 = 服务输入.会话开始 || 服务输入.会话结束
            || 服务输入.查询目标提示 || 服务输入.预览 || 服务输入.读取预览 || 服务输入.确认;
        if (有服务输入 && (!服务输入.会话开始 || !服务输入.会话结束
            || !服务输入.查询目标提示 || !服务输入.预览 || !服务输入.读取预览 || !服务输入.确认)) {
            return {网页对话宿主状态::会话绑定失败};
        }
        网络运行租约 网络;
        if (!网络.初始化()) return {网页对话宿主状态::网络初始化失败};

        std::string 令牌;
        if (!生成会话令牌(令牌)) return {网页对话宿主状态::令牌生成失败};
        std::string 公开会话标识;
        if (!生成会话令牌(公开会话标识) || 公开会话标识 == 令牌) {
            return {网页对话宿主状态::令牌生成失败};
        }

        套接字租约 监听(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
        if (!监听.有效()) return {网页对话宿主状态::套接字创建失败};

        sockaddr_in 地址{};
        地址.sin_family = AF_INET;
        地址.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        地址.sin_port = 0;
        if (bind(监听.取得(), reinterpret_cast<const sockaddr*>(&地址), sizeof(地址))
            == SOCKET_ERROR) {
            return {网页对话宿主状态::回环地址绑定失败};
        }
        if (listen(监听.取得(), SOMAXCONN) == SOCKET_ERROR) {
            return {网页对话宿主状态::监听失败};
        }

        int 地址长度 = sizeof(地址);
        if (getsockname(监听.取得(), reinterpret_cast<sockaddr*>(&地址), &地址长度)
            == SOCKET_ERROR) {
            return {网页对话宿主状态::运行失败};
        }
        const std::string 基础路径 = "/session/" + 令牌;
        const std::string 页面 = 构造页面(基础路径);
        const std::string 页面Origin = "http://127.0.0.1:" + std::to_string(ntohs(地址.sin_port));
        const std::string 浏览器地址 = 页面Origin + 基础路径;
        // 逆序析构：传输收束/队列清空 → 可选业务会话 → 外设会话。
        网页外设会话租约 外设会话(公开会话标识, 外设);
        网页服务会话租约 服务会话(公开会话标识, 服务输入);
        网页对话会话租约 会话;
        if (!外设会话.开始()) return {网页对话宿主状态::会话绑定失败};
        if (!会话.开始(公开会话标识, 停止请求, 输出队列)) return {网页对话宿主状态::运行失败};
        if (!服务会话.开始()) return {网页对话宿主状态::会话绑定失败};
        try {
            std::ofstream 会话旁路("D:/TEMP/海中鱼巣/last-session-url.txt", std::ios::trunc);
            if (会话旁路) 会话旁路 << 浏览器地址 << std::flush;
        } catch (...) {}
        // 不自动打开浏览器：页面轮询会在单线程 accept 上与启动期任务承接争用外设/场景锁，导致入站饿死甚至死锁。
        // 会话地址已写入旁路文件，由外部按同一 /api/messages 契约提交。
        (void)浏览器地址;

        bool 请求结束 = false;
        while (停止请求 == 0 && !请求结束) {
            fd_set 可读{};
            FD_ZERO(&可读);
            FD_SET(监听.取得(), &可读);
            timeval 等待{0, 200000};
            const int 选择结果 = select(0, &可读, nullptr, nullptr, &等待);
            if (选择结果 == SOCKET_ERROR) {
                return {网页对话宿主状态::运行失败};
            }
            if (选择结果 == 0) continue;

            套接字租约 客户端(accept(监听.取得(), nullptr, nullptr));
            if (!客户端.有效()) {
                if (WSAGetLastError() == WSAEINTR) continue;
                return {网页对话宿主状态::运行失败};
            }
            const DWORD 超时毫秒 = 2000;
            (void)setsockopt(客户端.取得(), SOL_SOCKET, SO_RCVTIMEO,
                reinterpret_cast<const char*>(&超时毫秒), sizeof(超时毫秒));
            (void)setsockopt(客户端.取得(), SOL_SOCKET, SO_SNDTIMEO,
                reinterpret_cast<const char*>(&超时毫秒), sizeof(超时毫秒));
            (void)处理请求(客户端.取得(), 基础路径, 公开会话标识, 页面Origin, 页面,
                请求结束, 消息接收, 服务输入, 对话框事实, 控制面板);
        }
        停止接纳提交();
        return {请求结束
            ? 网页对话宿主状态::用户结束
            : 网页对话宿主状态::停止信号结束};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"网页传输运行", L"载荷或传输结构发生工程异常，保留已发生事实。");
    }
}

} // namespace 海中鱼巣
