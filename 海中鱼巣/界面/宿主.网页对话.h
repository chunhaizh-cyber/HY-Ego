#pragma once

#include <cstddef>
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
    内部错误,
    会话绑定失败
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

enum class 网页对话会话核验状态 : std::uint8_t {
    已匹配 = 1, 宿主未运行, 会话不匹配, 输入拒绝, 资源失败, 内部错误
};
struct 网页对话会话核验结果 final {
    网页对话会话核验状态 状态 = 网页对话会话核验状态::输入拒绝;
    std::optional<std::uint64_t> 当前页面末序号;
};
网页对话会话核验结果 核验网页对话当前会话(
    std::string_view 公开会话标识) noexcept;

enum class 网页对话页面消息快照状态 : std::uint8_t {
    已完成 = 1, 额度不足, 宿主未运行, 会话不匹配,
    输入拒绝, 资源失败
};
struct 网页对话页面消息快照结果 final {
    网页对话页面消息快照状态 状态 = 网页对话页面消息快照状态::输入拒绝;
    std::string 公开会话标识;
    std::uint64_t 请求已读页面序号 = 0;
    std::uint64_t 截止页面序号 = 0;
    std::uint64_t 已覆盖页面序号 = 0;
    bool 覆盖完整 = false;
    std::vector<网页对话已显示消息> 消息组;
};

// 仅快照宿主已追加的页面消息；部分覆盖时由状态和覆盖序号标明进度。
网页对话页面消息快照结果 读取网页对话会话页面消息快照(
    std::string_view 公开会话标识,
    std::uint64_t 已读页面序号,
    std::size_t 最大消息条数,
    std::size_t 最大正文总字节数) noexcept;

// 会话标识只使用公开来源标识，不传递URL中的私有令牌。
// 对话框标识仅定位本次页面，不是认证身份。正式事实接收后的工程错误
// 必须终止，不能返回false诱使客户端把已经收到的内容当作未接收。
using 网页对话消息接收函数 = bool (*)(
    std::string_view 会话标识,
    std::string_view 对话框标识,
    std::uint64_t 消息序号,
    std::string_view UTF8正文) noexcept;

enum class 网页服务输入状态 : std::uint8_t {
    已完成 = 1, 匿名会话, 已复用, 未绑定, 会话失效, 输入不完整,
    输入不合法, 对象不匹配, 理解不存在, 结构不一致, 资源失败,
    需重新确认, 语义快照暂不支持, 已知值冲突, 要求不存在
};

struct 网页服务目标提交 {
    std::uint64_t 特征类型编码 = 0;
    std::int64_t 目标形式 = 0;
    std::optional<std::uint64_t> 目标域编码;
    std::optional<std::uint64_t> 比较概念编码;
    std::optional<std::int64_t> 变化方向;
};

struct 网页服务目标提示 {
    std::uint64_t 特征类型编码 = 0;
    std::uint64_t 当前采用概念编码 = 0;
};

struct 网页服务目标选项结果 {
    网页服务输入状态 状态 = 网页服务输入状态::未绑定;
    std::optional<std::uint64_t> 服务对象编码;
    std::vector<网页服务目标提示> 当前特征提示组;
};

struct 网页服务要求显示信息 {
    std::uint64_t 理解编码 = 0;
    std::uint64_t 服务对象编码 = 0;
    网页服务目标提交 目标;
    std::optional<std::uint64_t> 确认编码;
    std::string UTF8重述;
};

struct 网页服务要求结果 {
    网页服务输入状态 状态 = 网页服务输入状态::输入不合法;
    std::optional<网页服务要求显示信息> 信息;
};

struct 网页部分服务要求提交 {
    std::optional<std::uint64_t> 要求编码;
    std::string 来源要求标识;
    std::optional<std::uint64_t> 特征类型编码, 目标域编码, 比较概念编码;
    std::optional<std::int64_t> 目标形式, 变化方向;
};
struct 网页部分服务要求结果 {
    网页服务输入状态 状态 = 网页服务输入状态::输入不合法;
    std::optional<std::uint64_t> 要求编码;
    std::string 来源要求标识;
    std::optional<std::uint64_t> 服务对象编码, 需求编码, 任务编码;
    网页部分服务要求提交 已知目标;
    std::vector<std::string> 缺项, 冲突项;
    std::optional<网页服务要求显示信息> 完整预览;
    bool 消息已接纳 = false;
};

struct 网页服务输入回调组 {
    网页服务输入状态 (*会话开始)(std::string_view 公开会话标识) noexcept = nullptr;
    void (*会话结束)(std::string_view 公开会话标识) noexcept = nullptr;
    网页服务目标选项结果 (*查询目标提示)(std::string_view 公开会话标识) noexcept = nullptr;
    网页服务要求结果 (*预览)(std::string_view 公开会话标识,
        const 网页服务目标提交&) noexcept = nullptr;
    网页服务要求结果 (*读取预览)(std::string_view 公开会话标识,
        std::uint64_t 理解编码) noexcept = nullptr;
    网页服务要求结果 (*确认)(std::string_view 公开会话标识,
        std::uint64_t 理解编码) noexcept = nullptr;
    网页部分服务要求结果 (*提交部分要求)(std::string_view,
        const 网页部分服务要求提交&) noexcept = nullptr;
    网页部分服务要求结果 (*查询部分要求)(std::string_view,
        std::uint64_t 要求编码) noexcept = nullptr;
};

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

网页对话宿主结果 运行本地网页对话宿主(
    const volatile std::sig_atomic_t& 停止请求,
    网页对话消息接收函数 消息接收,
    const 网页服务输入回调组& 服务输入) noexcept;

} // namespace 海中鱼巣
