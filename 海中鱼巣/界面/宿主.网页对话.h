#pragma once

#include <csignal>
#include <cstdint>

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

// 在当前线程运行只监听127.0.0.1的HTTP服务，并用系统默认浏览器显示页面。
// 页面显式请求结束或进程停止信号到达前，本函数保持运行。
网页对话宿主结果 运行本地网页对话宿主(
    const volatile std::sig_atomic_t& 停止请求) noexcept;

} // namespace 海中鱼巣
