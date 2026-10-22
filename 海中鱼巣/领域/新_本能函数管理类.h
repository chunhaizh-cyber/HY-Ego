#pragma once

#include <cstdint>
#include <string_view>
#include <memory>
#include <stop_token>
#include <chrono>

#include "新_本能函数集.h"
#include "服务值结算.本能合同.h"
#include "连接.本能合同.h"

namespace 海中鱼巣 {
inline constexpr std::string_view 新本能函数登记名_服务值结算 = "服务值结算";
inline constexpr std::string_view 新本能函数登记名_获取服务对象 =
    "builtin.service_object.acquire.v1";
inline constexpr std::string_view 新本能函数登记名_建立连接 =
    "builtin.connection.establish.v1";
inline constexpr std::string_view 新本能函数登记名_主动获取服务需求 =
    "builtin.service_requirement.acquire_active.v1";
inline constexpr std::string_view 新本能函数登记名_被动获取服务需求 =
    "builtin.service_requirement.acquire_passive.v1";

// 稳定程序内登记名不是自然语言方法名称，也不作为能力判断依据。
inline constexpr std::string_view 新本能函数登记名_读取网页对话消息 =
    "builtin.web_dialog.read_messages.v1";
inline constexpr std::string_view 新本能函数登记名_设置网页对话输入文本 =
    "builtin.web_dialog.set_input_text.v1";
inline constexpr std::string_view 新本能函数登记名_提交网页对话消息 =
    "builtin.web_dialog.submit_message.v1";

// 网页宿主或浏览器适配层提供这三个入口。本结构只传函数，不传DOM对象、
// 浏览器句柄、文本副本或持久状态；外设提供者必须比所有调用存活更久。
struct 新本能网页对话函数端口 final {
    新本能函数 读取已显示消息 = nullptr;
    新本能函数 设置输入框文本 = nullptr;
    新本能函数 提交消息 = nullptr;
    friend bool operator==(const 新本能网页对话函数端口&,
        const 新本能网页对话函数端口&) = default;
};

enum class 新本能函数端口装配状态 : std::uint8_t {
    已装配 = 1,
    精确重复,
    入口拒绝,
    所有者冲突,
    内部错误,
    已关闭,
    已卸载,
    存在活动调用
};

enum class 新本能函数登记状态 : std::uint8_t {
    已登记 = 1,
    精确重复,
    未登记,
    入口拒绝,
    登记名冲突,
    资源失败,
    内部错误
};

enum class 新本能函数解析状态 : std::uint8_t {
    已解析 = 1,
    入口拒绝,
    未登记,
    内部错误
};

enum class 新本能函数可用状态 : std::uint8_t {
    可调用 = 1,
    入口拒绝,
    未登记,
    依赖未装配,
    内部错误
};

struct 新本能函数解析结果 final {
    新本能函数解析状态 状态 = 新本能函数解析状态::入口拒绝;
    新本能函数 函数 = nullptr;
};

// 统一持有“稳定登记名 -> C++函数入口”注册表和不可替换的外设端口。
// 方法节点只保存登记名；函数地址和端口不写入基础数据集。
class 新_本能函数管理类 final {
public:
    // 登记新_本能函数集提供的首批普通本能函数。重复调用不重复登记。
    static 新本能函数登记状态 初始化() noexcept;

    // 进程内只允许一个真实网页对话端口；同一端口可重复装配，不允许替换。
    static 新本能函数端口装配状态 装配网页对话端口(
        const 新本能网页对话函数端口& 端口) noexcept;
    static 新本能函数端口装配状态 装配服务值结算端口(新服务值结算本能端口&) noexcept;
    static 新本能函数端口装配状态 关闭服务值结算端口(新服务值结算本能端口&) noexcept;
    static 新本能函数端口装配状态 卸载服务值结算端口(新服务值结算本能端口&) noexcept;
    static 新本能函数端口装配状态 装配服务需求获取端口(
        std::shared_ptr<新服务需求获取本能端口>) noexcept;
    static 新本能函数端口装配状态 关闭服务需求获取端口(
        新服务需求获取本能端口&) noexcept;
    static 新本能函数端口装配状态 装配服务对象获取端口(
        std::shared_ptr<新服务对象获取本能端口>) noexcept;
    static 新本能函数端口装配状态 关闭服务对象获取端口(
        新服务对象获取本能端口&) noexcept;
    static 新本能函数端口装配状态 装配连接端口(
        std::shared_ptr<新连接本能端口>) noexcept;
    static 新本能函数端口装配状态 关闭连接端口(
        新连接本能端口&) noexcept;

    // 线程池调用期的工程停止能力；不进入业务参数或条件结果知识。
    class 本能停止守卫 final {
    public:
        explicit 本能停止守卫(std::stop_token) noexcept;
        ~本能停止守卫();
        本能停止守卫(const 本能停止守卫&) = delete;
        本能停止守卫& operator=(const 本能停止守卫&) = delete;
    private:
        std::stop_token 原停止_;
    };
    static bool 本次停止可用() noexcept;
    static bool 本次停止已请求() noexcept;
    static void 等待本次停止(std::chrono::milliseconds);

    // X属于外层执行器；调用期绑定精确容器地址，不成为函数参数或世界事实。
    class 本能执行守卫 final {
    public:
        本能执行守卫(const 方法参数场景&, 稳定编码 执行实例节点) noexcept;
        ~本能执行守卫();
        本能执行守卫(const 本能执行守卫&) = delete;
        本能执行守卫& operator=(const 本能执行守卫&) = delete;
    private:
        const 方法参数场景* 场景_;
    };
    static std::optional<稳定编码> 读取本次执行实例(const 方法参数场景&) noexcept;
    class 本能直接调用守卫 final {
    public:
        本能直接调用守卫(const 方法参数场景&, 稳定编码 调用身份) noexcept;
        ~本能直接调用守卫();
        本能直接调用守卫(const 本能直接调用守卫&) = delete;
        本能直接调用守卫& operator=(const 本能直接调用守卫&) = delete;
    private:
        const 方法参数场景* 场景_ = nullptr;
    };
    static std::optional<稳定编码> 读取本次直接调用(const 方法参数场景&) noexcept;
    // 仅为本次调用的一次入口工程守卫，不构成业务授权或动作完成事实。
    static void 标记本次动作入口(const 方法参数场景&) noexcept;

    // 同名同入口返回精确重复；同名不同入口拒绝，已登记入口不允许替换。
    static 新本能函数登记状态 登记(
        std::string_view 登记名,
        新本能函数 函数) noexcept;

    static 新本能函数解析结果 解析(
        std::string_view 登记名) noexcept;

    // 只检查登记入口及其不可变宿主依赖，不执行现实动作。
    static 新本能函数可用状态 检查可用(
        std::string_view 登记名) noexcept;

    // 每次只调用一次已解析入口；不自动重试现实动作。
    static 新本能函数本次条件结果对 执行(
        std::string_view 登记名,
        const 方法参数场景& 参数场景) noexcept;

private:
    static std::shared_ptr<新服务对象获取本能端口> 租用服务对象获取端口() noexcept;
    static std::shared_ptr<新连接本能端口> 租用连接端口() noexcept;
    static std::shared_ptr<新服务需求获取本能端口> 租用服务需求获取端口() noexcept;
    class 服务值结算调用守卫 final {
    public:
        服务值结算调用守卫() noexcept;
        ~服务值结算调用守卫();
        服务值结算调用守卫(const 服务值结算调用守卫&) = delete;
        服务值结算调用守卫& operator=(const 服务值结算调用守卫&) = delete;
        新服务值结算本能端口* operator->() const noexcept { return 端口_; }
        explicit operator bool() const noexcept { return 端口_ != nullptr; }
    private:
        新服务值结算本能端口* 端口_ = nullptr;
    };
    static 服务值结算调用守卫 租用服务值结算端口() noexcept;
    enum class 网页对话入口 : std::uint8_t {
        读取已显示消息 = 1,
        设置输入框文本,
        提交消息
    };

    friend class 新_本能函数集;
    static 新本能函数本次条件结果对 调用网页对话入口(
        网页对话入口 入口,
        const 方法参数场景& 参数场景) noexcept;
};

} // namespace 海中鱼巣
