#include "宿主.网页对话.h"

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
#include <limits>
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
};

struct 页面消息 final {
    std::uint64_t 序号 = 0;
    std::string 文本;
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
            std::size_t 解析值 = 0;
            const auto 转换 = std::from_chars(值.data(), 值.data() + 值.size(), 解析值);
            if (转换.ec != std::errc{} || 转换.ptr != 值.data() + 值.size()) {
                return 请求读取状态::请求无效;
            }
            正文长度 = 解析值;
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

std::string 构造消息JSON(const std::vector<页面消息>& 消息组) {
    std::string 结果 = "{\"messages\":[";
    bool 首项 = true;
    for (const auto& 消息 : 消息组) {
        if (!首项) 结果.push_back(',');
        首项 = false;
        结果 += "{\"id\":" + std::to_string(消息.序号)
            + ",\"role\":\"user\",\"text\":\""
            + JSON转义(消息.文本) + "\"}";
    }
    结果 += "]}";
    return 结果;
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
main{width:min(880px,94vw);height:min(760px,92vh);display:grid;grid-template-rows:auto 1fr auto;overflow:hidden;border:1px solid #3c516d;border-radius:18px;background:#182230;box-shadow:0 24px 80px #0008}
header{padding:18px 22px;border-bottom:1px solid #304157;display:flex;justify-content:space-between;align-items:center}
h1{font-size:20px;margin:0}.status{font-size:13px;color:#8fd3a7}
#messages{overflow:auto;padding:20px;display:flex;flex-direction:column;gap:12px}
.empty{margin:auto;color:#91a0b3}.message{align-self:flex-end;max-width:76%;padding:12px 15px;border-radius:15px 15px 4px 15px;background:#2e73c8;white-space:pre-wrap;overflow-wrap:anywhere}
form{padding:16px;border-top:1px solid #304157;display:grid;grid-template-columns:1fr auto auto;gap:10px}
textarea{resize:none;min-height:54px;max-height:160px;padding:12px;border:1px solid #405773;border-radius:12px;background:#0f1823;color:inherit;font:inherit}
button{border:0;border-radius:11px;padding:0 18px;font:inherit;color:white;background:#2e73c8;cursor:pointer}button.secondary{background:#4a5566}button:disabled{opacity:.55;cursor:wait}
footer{grid-column:1/-1;font-size:12px;color:#8d9aac}
</style>
</head>
<body>
<main>
<header><h1>海中鱼巣 · 网页交互窗口</h1><span class="status" id="status">已连接本机服务</span></header>
<section id="messages"><div class="empty">尚无对话消息</div></section>
<form id="form"><textarea id="input" maxlength="1048576" placeholder="输入消息；Enter发送，Shift+Enter换行"></textarea><button type="submit">发送</button><button type="button" class="secondary" id="close">结束</button><footer>网页接收成功只表示消息进入当前交互宿主，不表示任务完成或需求满足。</footer></form>
</main>
<script>
const base=')HTML";
    页面 += 基础路径;
    页面 += R"HTML(';
const messages=document.getElementById('messages');
const input=document.getElementById('input');
const form=document.getElementById('form');
const status=document.getElementById('status');
function render(items){messages.replaceChildren();if(!items.length){const e=document.createElement('div');e.className='empty';e.textContent='尚无对话消息';messages.append(e);return;}for(const item of items){const e=document.createElement('div');e.className='message';e.textContent=item.text;messages.append(e);}messages.scrollTop=messages.scrollHeight;}
async function refresh(){try{const r=await fetch(base+'/api/messages',{cache:'no-store'});if(!r.ok)throw new Error();render((await r.json()).messages);status.textContent='已连接本机服务';}catch{status.textContent='本机服务不可用';}}
form.addEventListener('submit',async e=>{e.preventDefault();const text=input.value;if(!text)return;const button=form.querySelector('button[type=submit]');button.disabled=true;try{const r=await fetch(base+'/api/messages',{method:'POST',headers:{'Content-Type':'text/plain;charset=utf-8'},body:text});if(!r.ok)throw new Error();input.value='';await refresh();}catch{status.textContent='消息提交失败';}finally{button.disabled=false;input.focus();}});
input.addEventListener('keydown',e=>{if(e.key==='Enter'&&!e.shiftKey){e.preventDefault();form.requestSubmit();}});
document.getElementById('close').addEventListener('click',async()=>{try{await fetch(base+'/api/close',{method:'POST'});}finally{document.body.innerHTML='<main style="height:auto;padding:28px"><h1>交互宿主已结束</h1><p>现在可以关闭此页面。</p></main>';}});
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

bool 处理请求(SOCKET 客户端,
    std::string_view 基础路径,
    const std::string& 页面,
    std::vector<页面消息>& 消息组,
    std::uint64_t& 下一序号,
    bool& 请求结束) {
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
    if (请求.方法 == "GET" && 请求.目标 == 基础路径) {
        发送响应(客户端, "200 OK", "text/html;charset=utf-8", 页面);
        return true;
    }
    if (请求.方法 == "GET" && 请求.目标 == 消息路径) {
        发送响应(客户端, "200 OK", "application/json;charset=utf-8",
            构造消息JSON(消息组));
        return true;
    }
    if (请求.方法 == "POST" && 请求.目标 == 消息路径) {
        if (请求.正文.empty() || !是有效UTF8(请求.正文)) {
            发送响应(客户端, "422 Unprocessable Content", "text/plain;charset=utf-8",
                "消息必须是非空UTF-8文本");
            return true;
        }
        消息组.push_back({下一序号++, std::move(请求.正文)});
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

网页对话宿主结果 运行本地网页对话宿主(
    const volatile std::sig_atomic_t& 停止请求) noexcept {
    try {
        网络运行租约 网络;
        if (!网络.初始化()) return {网页对话宿主状态::网络初始化失败};

        std::string 令牌;
        if (!生成会话令牌(令牌)) return {网页对话宿主状态::令牌生成失败};

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
        if (!启动系统浏览器(浏览器地址)) {
            return {网页对话宿主状态::浏览器启动失败};
        }

        std::vector<页面消息> 消息组;
        std::uint64_t 下一序号 = 1;
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
            (void)处理请求(客户端.取得(), 基础路径, 页面, 消息组,
                下一序号, 请求结束);
        }
        return {请求结束
            ? 网页对话宿主状态::用户结束
            : 网页对话宿主状态::停止信号结束};
    } catch (...) {
        return {网页对话宿主状态::内部错误};
    }
}

} // namespace 海中鱼巣
