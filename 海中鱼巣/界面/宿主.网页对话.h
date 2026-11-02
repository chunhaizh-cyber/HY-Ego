#pragma once

#include <cstddef>
#include <csignal>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace 海中鱼巣 {

enum class 网页控制快照状态 : std::uint8_t {
    未接入, 采集中, 已取得, 采集失败, 已停止
};
struct 网页控制快照读取结果 final {
    网页控制快照状态 状态 = 网页控制快照状态::未接入;
    std::shared_ptr<const std::string> UTF8JSON;
    std::uint64_t 序号 = 0;
    std::uint64_t 距采集完成毫秒 = 0;
    bool 正在采集 = false;
};
struct 网页控制面板回调组 final {
    // 仅取得共享缓存，不得在HTTP线程查询业务owner。
    std::function<网页控制快照读取结果()> 读取;
};

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
    // 兼容字段：全局页面游标已退出，始终为nullopt。
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

enum class 网页服务输入状态 : std::uint8_t {
    已完成 = 1, 匿名会话, 已复用, 未绑定, 会话失效, 输入不完整,
    输入不合法, 对象不匹配, 理解不存在, 结构不一致, 资源失败,
    需重新确认, 语义快照暂不支持, 已知值冲突, 要求不存在, 目标范围已变化
};

struct 网页服务目标提交 {
    std::uint64_t 目标存在编码 = 0;
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

enum class 网页服务材料投递状态 : std::uint8_t {
    未尝试 = 0, 等待后继消费 = 1, 消息已接纳 = 2, 暂未接纳 = 3
};

struct 网页服务要求结果 {
    网页服务输入状态 状态 = 网页服务输入状态::输入不合法;
    std::optional<网页服务要求显示信息> 信息;
    网页服务材料投递状态 材料投递状态 = 网页服务材料投递状态::未尝试;
};

struct 网页部分服务要求提交 {
    std::optional<std::uint64_t> 要求编码;
    std::optional<std::uint64_t> 目标存在编码;
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
    网页服务材料投递状态 材料投递状态 = 网页服务材料投递状态::未尝试;
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
    void (*接收事实已记录)(std::string_view 会话,std::string_view 页面,std::uint64_t 上行序号) noexcept = nullptr;
};

struct 网页对话框目标 final {
    std::string 公开会话标识;
    std::string 对话框标识;
    friend bool operator==(const 网页对话框目标&, const 网页对话框目标&) = default;
};
enum class 网页定向发送状态 : std::uint8_t {
    已排队 = 1, 正在写出, 已完整写出, 写出未完成, 已取消,
    输入拒绝, 宿主未运行, 会话失效, 对话框未登记, 提交不存在,
    页面被占用
};
struct 网页定向发送请求 final {
    网页对话框目标 目标;
    std::string UTF8正文;
    std::optional<std::uint64_t> 出站占用身份;
};
struct 网页定向发送结果 final {
    网页定向发送状态 状态 = 网页定向发送状态::输入拒绝;
    std::optional<std::uint64_t> 提交序号;
    std::size_t HTTP已写字节数 = 0;
    int 系统错误 = 0;
};
struct 网页定向发送事实 final {
    网页对话框目标 目标;
    std::uint64_t 提交序号 = 0;
    std::string UTF8正文;
};
struct 网页输出入列请求 final {
    网页对话框目标 目标;
    std::string UTF8正文;
};
struct 网页输出队列项 final {
    网页对话框目标 目标;
    std::uint64_t 提交序号 = 0, 正式消息编码 = 0;
    std::string UTF8正文;
};
enum class 网页输出队列状态 : std::uint8_t {
    已入列 = 1, 已复用, 已领取, 已移除, 已清空, 空, 不在队列
};
struct 网页输出队列回执 final {
    网页输出队列状态 状态 = 网页输出队列状态::空;
    std::optional<网页输出队列项> 项;
};
// 只在宿主运行租约内借用；提供者不得回调宿主或重新取得应用上下文锁。
struct 网页输出队列回调组 final {
    std::function<网页输出队列回执(const 网页输出入列请求&)> 入列;
    std::function<网页输出队列回执(const 网页对话框目标&)> 领取;
    std::function<网页输出队列回执(const 网页对话框目标&, std::uint64_t)> 取消;
    std::function<网页输出队列回执(const 网页对话框目标&)> 清空;
};
// 完整网页外设控制域；业务服务及自我/任务状态不参与外设事实准入。
// 闭包保活适配状态，外层必须在world销毁前关闭模块并收束宿主。
struct 网页外设运行回调组 final {
    std::function<bool(std::string_view)> 会话开始;
    std::function<void(std::string_view)> 会话结束;
    std::function<bool(const 网页对话框目标&)> 确认页面;
    std::function<bool(std::string_view, std::string_view,
        std::uint64_t, std::string_view)> 接收消息;
    std::function<void(const 网页定向发送事实&)> 发送完成;
    网页输出队列回调组 输出队列;
};
网页定向发送结果 提交网页对话框消息(const 网页定向发送请求&) noexcept;
网页定向发送结果 查询网页对话框发送(
    const 网页对话框目标&, std::uint64_t 提交序号) noexcept;
bool 取得网页对话框出站占用(const 网页对话框目标&, std::uint64_t 调用编码) noexcept;
bool 核验网页对话框出站占用可用(const 网页对话框目标&, std::uint64_t 调用编码) noexcept;
bool 释放网页对话框出站占用(const 网页对话框目标&, std::uint64_t 调用编码) noexcept;
网页定向发送结果 取消网页对话框提交(
    const 网页对话框目标&, std::uint64_t 提交序号, std::uint64_t 占用身份) noexcept;
网页对话消息读取结果 读取网页对话框显示消息(
    const 网页对话框目标&, std::uint64_t 已读页面序号) noexcept;

// 已退出的无目标入口：仅保留ABI，入口拒绝且零写入，不选择默认页面。
网页对话消息读取结果 读取网页对话已显示消息(
    std::uint64_t 已读页面序号) noexcept;
网页对话外设操作结果 设置网页对话待提交文本(
    std::string_view UTF8正文) noexcept;
网页对话外设操作结果 提交网页对话待提交文本() noexcept;

// 在当前线程运行只监听127.0.0.1的HTTP服务，并用系统默认浏览器显示页面。
// 页面显式请求结束或进程停止信号到达前，本函数保持运行。
网页对话宿主结果 运行本地网页对话宿主(
    const volatile std::sig_atomic_t& 停止请求,
    const 网页外设运行回调组& 外设,
    const 网页服务输入回调组& 服务输入,
    const 网页控制面板回调组& 控制面板) noexcept;

} // namespace 海中鱼巣
