#pragma once

#include <csignal>
#include <cstdint>
#include <string_view>

namespace 海中鱼巣 {

enum class 网页对话宿主状态 : std::uint8_t {
    用户结束 = 0,
    停止信号结束,
    网络初始化失败,
    令牌生成失败,
    套接字创建失败,
    回环地址绑定失败,
    监听失败,
    浏览器启动失败,
    运行失败,
    内部错误
};

struct 网页对话宿主结果 final {
    网页对话宿主状态 状态 = 网页对话宿主状态::内部错误;

    bool 成功() const noexcept;
};

// 返回true表示本条消息已经被上层按同一会话、序号和正文接纳；false
// 表示暂未接纳，宿主不递增序号，浏览器可以重试同一条消息。
using 网页对话消息接收函数 = bool (*)(
    std::string_view 会话标识,
    std::uint64_t 消息序号,
    std::string_view UTF8正文) noexcept;

// 在当前线程运行只监听127.0.0.1的HTTP服务，并用系统默认浏览器显示页面。
// 页面显式请求结束或进程停止信号到达前，本函数保持运行。
网页对话宿主结果 运行本地网页对话宿主(
    const volatile std::sig_atomic_t& 停止请求,
    网页对话消息接收函数 消息接收 = nullptr) noexcept;

} // namespace 海中鱼巣
