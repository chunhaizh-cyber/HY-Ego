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
};

struct 网页对话共享状态 final {
    std::mutex 互斥;
    bool 运行中 = false;
    std::string 会话标识;
    std::vector<网页对话已显示消息> 消息组;
    std::uint64_t 下一上行序号 = 1;
    std::uint64_t 下一页面序号 = 1;
    std::optional<std::string> 待提交文本;
};

网页对话共享状态 共享状态;

class 网页对话会话租约 final {
public:
    网页对话会话租约() = default;

    bool 开始(std::string_view 会话标识) noexcept {
        try {
            std::lock_guard 锁(共享状态.互斥);
            if (共享状态.运行中 || 会话标识.empty()) return false;
            共享状态.会话标识.assign(会话标识);
            共享状态.消息组.clear();
            共享状态.下一上行序号 = 1;
            共享状态.下一页面序号 = 1;
            共享状态.待提交文本.reset();
            共享状态.运行中 = true;
            已开始_ = true;
            return true;
        } catch (...) {
            return false;
        }
    }

    ~网页对话会话租约() {
        if (!已开始_) return;
        try {
            std::lock_guard 锁(共享状态.互斥);
            共享状态.运行中 = false;
            共享状态.待提交文本.reset();
            共享状态.会话标识.clear();
        } catch (...) {
        }
    }

    网页对话会话租约(const 网页对话会话租约&) = delete;
    网页对话会话租约& operator=(const 网页对话会话租约&) = delete;

private:
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
        } else if (名称 == "content-type") {
            if (已有内容类型) return 请求读取状态::请求无效;
            已有内容类型 = true;
            请求.内容类型 = 转小写ASCII(值);
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

bool 发送全部(SOCKET 客户端, std::string_view 内容) noexcept {
    while (!内容.empty()) {
        const auto 本次 = (std::min)(内容.size(),
            static_cast<std::size_t>((std::numeric_limits<int>::max)()));
        const int 已发 = send(客户端, 内容.data(), static_cast<int>(本次), 0);
        if (已发 == SOCKET_ERROR || 已发 == 0) return false;
        内容.remove_prefix(static_cast<std::size_t>(已发));
    }
    return true;
}

void 发送响应(SOCKET 客户端,
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
        (void)发送全部(客户端, 响应);
    } catch (...) {
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
    case 网页服务输入状态::需重新确认: return "409 Conflict";
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
    auto 状态 = 解析正整数(取得表单字段(字段组, "ft"), 目标.特征类型编码);
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
    if (目标.特征类型编码 == 0) return false;
    if (目标.目标形式 == 1) return 目标.目标域编码 && *目标.目标域编码 != 0
        && 目标.比较概念编码 && *目标.比较概念编码 != 0 && !目标.变化方向;
    return 目标.目标形式 == 2 && !目标.目标域编码 && !目标.比较概念编码
        && 目标.变化方向 && (*目标.变化方向 == 1 || *目标.变化方向 == 2);
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
        + "\",\"info\":{\"preview\":\"" + std::to_string(信息.理解编码)
        + "\",\"object\":\"" + std::to_string(信息.服务对象编码)
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
                    {"ft", "form", "domain", "comparison", "direction"}, 字段组)
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
</style>
</head>
<body>
<main>
<header><h1>海中鱼巣 · 网页交互窗口</h1><span class="status" id="status">已连接本机服务</span></header>
<section id="messages"><div class="empty">尚无对话消息</div></section>
<form id="form"><textarea id="input" maxlength="1048576" placeholder="输入消息；Enter发送，Shift+Enter换行"></textarea><button type="submit">发送</button><button type="button" class="secondary" id="close">结束</button><footer>网页接收成功只表示消息进入当前交互宿主，不表示任务完成或需求满足。</footer></form>
<section id="service">
<h2>结构化服务要求</h2>
<p>目标人由本次本机启动配置指定。填写已有正式概念编码；当前特征提示不限制可输入的编码。先查看完整重述，再明确确认。</p>
<p id="service-hints"></p>
<form id="service-form">
<fieldset id="service-fields" disabled>
<label>目标人编码（只读）<input id="service-object" readonly></label>
<label>目标特征类型编码<input name="ft" inputmode="numeric" required autocomplete="off"></label>
<label>目标形式<select name="form" id="service-mode" required><option value="">请选择</option><option value="1">值目标</option><option value="2">方向目标</option></select></label>
<label id="domain-label">目标域概念编码<input name="domain" inputmode="numeric" autocomplete="off" disabled></label>
<label id="comparison-label">比较概念编码<input name="comparison" inputmode="numeric" autocomplete="off" disabled></label>
<label id="direction-label">变化方向<select name="direction" disabled><option value="">请选择</option><option value="1">增加</option><option value="2">减少</option></select></label>
</fieldset>
<div class="service-actions"><button type="submit" id="service-submit" disabled>生成预览</button><button type="button" id="service-confirm" disabled>确认当前预览</button></div>
</form>
<p id="service-status">正在核对本次会话绑定</p><pre id="service-preview"></pre>
</section>
</main>
<script>
const base=')HTML";
    页面 += 基础路径;
    页面 += R"HTML(';
const messages=document.getElementById('messages');
const input=document.getElementById('input');
const form=document.getElementById('form');
const status=document.getElementById('status');
function render(items){messages.replaceChildren();if(!items.length){const e=document.createElement('div');e.className='empty';e.textContent='尚无对话消息';messages.append(e);return;}for(const item of items){const e=document.createElement('div');e.className='message '+item.role;e.textContent=item.text;messages.append(e);}messages.scrollTop=messages.scrollHeight;}
async function refresh(){try{const r=await fetch(base+'/api/messages',{cache:'no-store'});if(!r.ok)throw new Error();render((await r.json()).messages);status.textContent='已连接本机服务';}catch{status.textContent='本机服务不可用';}}
form.addEventListener('submit',async e=>{e.preventDefault();const text=input.value;if(!text)return;const button=form.querySelector('button[type=submit]');button.disabled=true;try{const r=await fetch(base+'/api/messages',{method:'POST',headers:{'Content-Type':'text/plain;charset=utf-8'},body:text});if(!r.ok)throw new Error();input.value='';await refresh();}catch{status.textContent='消息提交失败';}finally{button.disabled=false;input.focus();}});
input.addEventListener('keydown',e=>{if(e.key==='Enter'&&!e.shiftKey){e.preventDefault();form.requestSubmit();}});
document.getElementById('close').addEventListener('click',async()=>{try{await fetch(base+'/api/close',{method:'POST'});}finally{document.body.innerHTML='<main style="height:auto;padding:28px"><h1>交互宿主已结束</h1><p>现在可以关闭此页面。</p></main>';}});
const serviceForm=document.getElementById('service-form');
const serviceFields=document.getElementById('service-fields');
const serviceMode=document.getElementById('service-mode');
const serviceSubmit=document.getElementById('service-submit');
const serviceConfirm=document.getElementById('service-confirm');
const serviceStatus=document.getElementById('service-status');
const servicePreview=document.getElementById('service-preview');
let previewId=null,formRevision=0,serviceBusy=false;
function invalidatePreview(){formRevision++;previewId=null;serviceConfirm.disabled=true;servicePreview.textContent='';serviceStatus.textContent='输入已变化，尚未完整预览或确认';}
function selectServiceMode(){for(const name of ['domain','comparison','direction']){const field=serviceForm.elements.namedItem(name);const enabled=name==='direction'?serviceMode.value==='2':serviceMode.value==='1';field.disabled=!enabled;field.required=enabled;document.getElementById(name+'-label').hidden=!enabled;}}
serviceForm.addEventListener('input',()=>{invalidatePreview();selectServiceMode();});
serviceForm.addEventListener('change',()=>{invalidatePreview();selectServiceMode();});
async function serviceRequest(path,body){const options={cache:'no-store'};if(body!==undefined){options.method='POST';options.headers={'Content-Type':'application/x-www-form-urlencoded'};options.body=body.toString();}const response=await fetch(base+'/api/service/'+path,options);const data=await response.json();if(!response.ok)throw new Error(data.status==='需重新确认'?'语义已变化，请重新生成预览并确认':data.status||'服务要求处理失败');return data;}
function displayServiceInfo(info){if(!info||typeof info.preview!=='string'||typeof info.object!=='string'||typeof info.text!=='string')throw new Error('服务回执结构不一致');servicePreview.textContent='目标人编码：'+info.object+'\n理解编码：'+info.preview+'\n'+info.text;}
async function loadServiceOptions(){try{const data=await serviceRequest('options');document.getElementById('service-object').value=data.object;document.getElementById('service-hints').textContent='当前特征提示：'+(data.hints.length?data.hints.map(h=>'FT '+h.ft+' / 当前概念 '+h.concept).join('；'):'无现有特征提示');serviceFields.disabled=false;serviceSubmit.disabled=false;serviceStatus.textContent='尚未确认。填写目标后生成预览。';selectServiceMode();}catch(error){serviceStatus.textContent=error.message+'；结构化输入不可用，文本对话仍可使用。';}}
serviceForm.addEventListener('submit',async event=>{event.preventDefault();if(serviceBusy)return;const revision=formRevision;const body=new URLSearchParams(new FormData(serviceForm));previewId=null;serviceConfirm.disabled=true;serviceBusy=true;serviceSubmit.disabled=true;serviceStatus.textContent='正在生成预览';try{const data=await serviceRequest('preview',body);if(revision!==formRevision)return;displayServiceInfo(data.info);if(data.info.confirmation){serviceStatus.textContent='该份内容已确认，确认编码：'+data.info.confirmation;}else{previewId=data.info.preview;serviceConfirm.disabled=false;serviceStatus.textContent='尚未确认。请核对下面固定语义的完整重述，再点击确认。';}}catch(error){if(revision===formRevision)serviceStatus.textContent=error.message;}finally{serviceBusy=false;serviceSubmit.disabled=false;}});
serviceConfirm.addEventListener('click',async()=>{if(!previewId||serviceBusy)return;const confirming=previewId;previewId=null;serviceBusy=true;serviceConfirm.disabled=true;serviceSubmit.disabled=true;serviceFields.disabled=true;serviceStatus.textContent='正在确认当前预览';try{const data=await serviceRequest('confirm',new URLSearchParams({preview:confirming}));displayServiceInfo(data.info);serviceStatus.textContent='已确认以上内容，确认编码：'+data.info.confirmation+'。这不表示任务已建立或目标已满足。';}catch(error){serviceStatus.textContent=error.message+'；可重新预览同一内容后重试。';}finally{serviceBusy=false;serviceSubmit.disabled=false;serviceFields.disabled=false;selectServiceMode();}});
loadServiceOptions();
refresh();setInterval(refresh,750);
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

bool 追加交互者消息(
    std::string_view 会话标识,
    std::uint64_t 上行序号,
    std::string UTF8正文) noexcept {
    try {
        std::lock_guard 锁(共享状态.互斥);
        if (!共享状态.运行中 || 共享状态.会话标识 != 会话标识
            || 共享状态.下一上行序号 != 上行序号
            || 共享状态.下一页面序号 == 0
            || 共享状态.下一页面序号
                > static_cast<std::uint64_t>(
                    (std::numeric_limits<std::int64_t>::max)())) {
            return false;
        }
        共享状态.消息组.push_back({共享状态.下一页面序号,
            网页对话消息角色::交互者, std::move(UTF8正文)});
        ++共享状态.下一页面序号;
        ++共享状态.下一上行序号;
        return true;
    } catch (...) {
        return false;
    }
}

bool 处理请求(SOCKET 客户端,
    std::string_view 基础路径,
    std::string_view 会话标识,
    const std::string& 页面,
    bool& 请求结束,
    网页对话消息接收函数 消息接收,
    const 网页服务输入回调组& 服务输入) {
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

    const std::string 消息路径 = std::string(基础路径) + "/api/messages";
    const std::string 结束路径 = std::string(基础路径) + "/api/close";
    if (处理服务输入请求(客户端, 请求, 基础路径, 会话标识, 服务输入)) return true;
    if (请求.方法 == "GET" && 请求.目标 == 基础路径) {
        发送响应(客户端, "200 OK", "text/html;charset=utf-8", 页面);
        return true;
    }
    if (请求.方法 == "GET" && 请求.目标 == 消息路径) {
        const auto 读取 = 读取网页对话已显示消息(0);
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
        if (请求.正文.empty() || !是有效UTF8(请求.正文)) {
            发送响应(客户端, "422 Unprocessable Content", "text/plain;charset=utf-8",
                "消息必须是非空UTF-8文本");
            return true;
        }
        const auto 上行序号 = 取得下一上行序号(会话标识);
        if (!上行序号
            || (消息接收
                && !消息接收(会话标识, *上行序号, 请求.正文))) {
            发送响应(客户端, "503 Service Unavailable",
                "application/json;charset=utf-8",
                "{\"accepted\":false,\"retryable\":true}");
            return true;
        }
        if (!追加交互者消息(
                会话标识, *上行序号, std::move(请求.正文))) {
            发送响应(客户端, "503 Service Unavailable",
                "application/json;charset=utf-8",
                "{\"accepted\":false,\"retryable\":true}");
            return true;
        }
        发送响应(客户端, "202 Accepted", "application/json;charset=utf-8",
            "{\"accepted\":true}");
        return true;
    }
    if (请求.方法 == "POST" && 请求.目标 == 结束路径 && 请求.正文.empty()) {
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
        if (!共享状态.运行中)
            return {网页对话会话核验状态::宿主未运行, std::nullopt};
        if (共享状态.会话标识 != 公开会话标识)
            return {网页对话会话核验状态::会话不匹配, std::nullopt};
        if (共享状态.下一页面序号 == 0)
            return {网页对话会话核验状态::内部错误, std::nullopt};
        return {网页对话会话核验状态::已匹配,
            共享状态.下一页面序号 - 1};
    } catch (const std::bad_alloc&) {
        return {网页对话会话核验状态::资源失败, std::nullopt};
    } catch (const std::length_error&) {
        return {网页对话会话核验状态::资源失败, std::nullopt};
    } catch (...) {
        return {网页对话会话核验状态::内部错误, std::nullopt};
    }
}

网页对话页面消息快照结果 读取网页对话会话页面消息快照(
    std::string_view 公开会话标识,
    std::uint64_t 已读页面序号,
    std::size_t 最大消息条数,
    std::size_t 最大正文总字节数) noexcept {
    if (公开会话标识.empty() || !是有效UTF8(公开会话标识)
        || 最大消息条数 == 0 || 最大消息条数 > 1024
        || 最大正文总字节数 == 0
        || 最大正文总字节数 > 8U * 1024U * 1024U) {
        return {};
    }

    try {
        std::lock_guard 锁(共享状态.互斥);
        if (!共享状态.运行中)
            return {网页对话页面消息快照状态::宿主未运行};
        if (共享状态.会话标识 != 公开会话标识)
            return {网页对话页面消息快照状态::会话不匹配};
        if (共享状态.下一页面序号 == 0) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__,
                L"读取网页对话会话页面消息快照", L"页面序号结构损坏。");
        }

        const auto 截止 = 共享状态.下一页面序号 - 1;
        if (已读页面序号 > 截止)
            return {网页对话页面消息快照状态::输入拒绝};
        if (截止 > (std::numeric_limits<std::size_t>::max)()
            || 共享状态.消息组.size() != static_cast<std::size_t>(截止)) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__,
                L"读取网页对话会话页面消息快照", L"页面消息组长度与截止序号不一致。");
        }
        for (std::size_t i = 0; i < 共享状态.消息组.size(); ++i) {
            const auto& 消息 = 共享状态.消息组[i];
            if (消息.页面序号 != static_cast<std::uint64_t>(i) + 1
                || (消息.角色 != 网页对话消息角色::交互者
                    && 消息.角色 != 网页对话消息角色::自我)) {
                工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__,
                    L"读取网页对话会话页面消息快照", L"页面消息顺序或角色结构损坏。");
            }
        }

        网页对话页面消息快照结果 结果;
        结果.公开会话标识.assign(公开会话标识);
        结果.请求已读页面序号 = 已读页面序号;
        结果.截止页面序号 = 截止;
        结果.已覆盖页面序号 = 已读页面序号;
        std::size_t 已用正文总字节数 = 0;
        for (std::size_t i = static_cast<std::size_t>(已读页面序号);
            i < 共享状态.消息组.size(); ++i) {
            const auto& 消息 = 共享状态.消息组[i];
            if (结果.消息组.size() == 最大消息条数
                || 消息.UTF8正文.size() > 最大正文总字节数 - 已用正文总字节数) {
                结果.状态 = 网页对话页面消息快照状态::额度不足;
                return 结果;
            }
            结果.消息组.push_back(消息);
            已用正文总字节数 += 消息.UTF8正文.size();
            结果.已覆盖页面序号 = 消息.页面序号;
        }
        结果.状态 = 网页对话页面消息快照状态::已完成;
        结果.覆盖完整 = true;
        return 结果;
    } catch (const std::bad_alloc&) {
        return {网页对话页面消息快照状态::资源失败};
    } catch (const std::length_error&) {
        return {网页对话页面消息快照状态::资源失败};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__,
            L"读取网页对话会话页面消息快照", L"宿主页面快照发生工程异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__,
            L"读取网页对话会话页面消息快照", L"宿主页面快照发生未知工程异常。");
    }
}

网页对话消息读取结果 读取网页对话已显示消息(
    std::uint64_t 已读页面序号) noexcept {
    网页对话消息读取结果 结果;
    try {
        std::lock_guard 锁(共享状态.互斥);
        if (!共享状态.运行中) {
            结果.状态 = 网页对话外设状态::宿主未运行;
            return 结果;
        }
        结果.最新页面序号 = 共享状态.下一页面序号 - 1;
        if (已读页面序号 > 结果.最新页面序号) {
            结果.状态 = 网页对话外设状态::入口拒绝;
            return 结果;
        }
        for (const auto& 消息 : 共享状态.消息组) {
            if (消息.页面序号 > 已读页面序号) 结果.消息组.push_back(消息);
        }
        结果.状态 = 结果.消息组.empty()
            ? 网页对话外设状态::无变化
            : 网页对话外设状态::已完成;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.消息组.clear();
        结果.状态 = 网页对话外设状态::资源失败;
    } catch (...) {
        结果.消息组.clear();
        结果.状态 = 网页对话外设状态::内部错误;
    }
    return 结果;
}

网页对话外设操作结果 设置网页对话待提交文本(
    std::string_view UTF8正文) noexcept {
    if (UTF8正文.empty() || UTF8正文.size() > 最大消息字节数
        || !是有效UTF8(UTF8正文)) {
        return {网页对话外设状态::入口拒绝, std::nullopt};
    }
    try {
        std::lock_guard 锁(共享状态.互斥);
        if (!共享状态.运行中) {
            return {网页对话外设状态::宿主未运行, std::nullopt};
        }
        if (共享状态.待提交文本
            && *共享状态.待提交文本 == UTF8正文) {
            return {网页对话外设状态::无变化, std::nullopt};
        }
        共享状态.待提交文本 = std::string(UTF8正文);
        return {网页对话外设状态::已完成, std::nullopt};
    } catch (const std::bad_alloc&) {
        return {网页对话外设状态::资源失败, std::nullopt};
    } catch (...) {
        return {网页对话外设状态::内部错误, std::nullopt};
    }
}

网页对话外设操作结果 提交网页对话待提交文本() noexcept {
    try {
        std::lock_guard 锁(共享状态.互斥);
        if (!共享状态.运行中) {
            return {网页对话外设状态::宿主未运行, std::nullopt};
        }
        if (!共享状态.待提交文本 || 共享状态.待提交文本->empty()) {
            return {网页对话外设状态::入口拒绝, std::nullopt};
        }
        if (共享状态.下一页面序号 == 0
            || 共享状态.下一页面序号
                > static_cast<std::uint64_t>(
                    (std::numeric_limits<std::int64_t>::max)())) {
            return {网页对话外设状态::内部错误, std::nullopt};
        }
        const auto 页面序号 = 共享状态.下一页面序号;
        共享状态.消息组.push_back({页面序号,
            网页对话消息角色::自我,
            std::move(*共享状态.待提交文本)});
        共享状态.待提交文本.reset();
        ++共享状态.下一页面序号;
        return {网页对话外设状态::已完成, 页面序号};
    } catch (const std::bad_alloc&) {
        return {网页对话外设状态::资源失败, std::nullopt};
    } catch (...) {
        return {网页对话外设状态::内部错误, std::nullopt};
    }
}

网页对话宿主结果 运行本地网页对话宿主(
    const volatile std::sig_atomic_t& 停止请求,
    网页对话消息接收函数 消息接收) noexcept {
    return 运行本地网页对话宿主(停止请求, 消息接收, {});
}

网页对话宿主结果 运行本地网页对话宿主(
    const volatile std::sig_atomic_t& 停止请求,
    网页对话消息接收函数 消息接收,
    const 网页服务输入回调组& 服务输入) noexcept {
    try {
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
        const std::string 浏览器地址 = "http://127.0.0.1:"
            + std::to_string(ntohs(地址.sin_port)) + 基础路径;
        网页对话会话租约 会话;
        if (!会话.开始(公开会话标识)) return {网页对话宿主状态::运行失败};
        网页服务会话租约 服务会话(公开会话标识, 服务输入);
        if (!服务会话.开始()) return {网页对话宿主状态::会话绑定失败};
        if (!启动系统浏览器(浏览器地址)) {
            return {网页对话宿主状态::浏览器启动失败};
        }

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
            (void)处理请求(客户端.取得(), 基础路径, 公开会话标识, 页面,
                请求结束, 消息接收, 服务输入);
        }
        return {请求结束
            ? 网页对话宿主状态::用户结束
            : 网页对话宿主状态::停止信号结束};
    } catch (...) {
        return {网页对话宿主状态::内部错误};
    }
}

} // namespace 海中鱼巣
