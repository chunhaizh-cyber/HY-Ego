#pragma once

#include <csignal>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

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

enum class 网页对话消息角色 : std::uint8_t {
    交互者 = 1,
    自我
};

struct 网页对话已显示消息 final {
    std::uint64_t 页面序号 = 0;
    网页对话消息角色 角色 = 网页对话消息角色::交互者;
    std::string UTF8正文;
    friend bool operator==(const 网页对话已显示消息&,
        const 网页对话已显示消息&) = default;
};

enum class 网页对话外设状态 : std::uint8_t {
    已完成 = 1,
    无变化,
    宿主未运行,
    入口拒绝,
    资源失败,
    内部错误
};

struct 网页对话消息读取结果 final {
    网页对话外设状态 状态 = 网页对话外设状态::入口拒绝;
    std::uint64_t 最新页面序号 = 0;
    std::vector<网页对话已显示消息> 消息组;
};

struct 网页对话外设操作结果 final {
    网页对话外设状态 状态 = 网页对话外设状态::入口拒绝;
    std::optional<std::uint64_t> 页面序号;
};

// 返回true表示本条消息已经被上层按同一会话、序号和正文接纳；false
// 表示暂未接纳，宿主不递增序号，浏览器可以重试同一条消息。
using 网页对话消息接收函数 = bool (*)(
    std::string_view 会话标识,
    std::uint64_t 消息序号,
    std::string_view UTF8正文) noexcept;

// 本能函数适配层只通过以下三个入口接触网页宿主。读取返回服务器已经
// 提供给页面的数据；设置只形成待提交内容，提交才增加一条自我消息。
网页对话消息读取结果 读取网页对话已显示消息(
    std::uint64_t 已读页面序号) noexcept;
网页对话外设操作结果 设置网页对话待提交文本(
    std::string_view UTF8正文) noexcept;
网页对话外设操作结果 提交网页对话待提交文本() noexcept;

// 在当前线程运行只监听127.0.0.1的HTTP服务，并用系统默认浏览器显示页面。
// 页面显式请求结束或进程停止信号到达前，本函数保持运行。
网页对话宿主结果 运行本地网页对话宿主(
    const volatile std::sig_atomic_t& 停止请求,
    网页对话消息接收函数 消息接收 = nullptr) noexcept;

} // namespace 海中鱼巣
