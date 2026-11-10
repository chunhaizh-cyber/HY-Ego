#pragma once

#include <cstdint>
#include <string_view>
#include <memory>
#include <stop_token>
#include <chrono>

#include "新_本能函数集.h"
#include "进入场景.本能合同.h"
#include "本能字段询问.运行接口.h"

namespace 海中鱼巣 {
inline constexpr std::string_view 新本能函数登记名_询问指定需求字段 = "builtin.demand.field.ask.v1";
inline constexpr std::string_view 新本能函数登记名_获取交互者 =
    "builtin.interactor.acquire.v1";
inline constexpr std::string_view 新本能函数登记名_获取服务对象 =
    "builtin.service_object.acquire.v4";
// v4内置监听、不以交互者为条件（INTERACT-SCENE-INSTINCT）；v3停用。
inline constexpr std::string_view 新本能函数登记名_旧五参数获取服务对象 =
    "builtin.service_object.acquire.v3";
// v3以自我的服务对象特征为结果，不改写交互者（R93）；v2停用。
inline constexpr std::string_view 新本能函数登记名_旧四参数获取服务对象 =
    "builtin.service_object.acquire.v2";
inline constexpr std::string_view 新本能函数登记名_获取服务对象新需求 =
    "builtin.service_object.demand.acquire_new.v1";
inline constexpr std::string_view 新本能函数登记名_获取服务对象需求节点 =
    "builtin.service_object.demand.acquire_node.v1";
inline constexpr std::string_view 新本能函数登记名_旧两参数获取服务对象 =
    "builtin.service_object.acquire.v1";
// 通用本能“进入场景”（6220 §2，R94/R95）。
inline constexpr std::string_view 新本能函数登记名_进入场景 =
    "builtin.self.enter_scene.v1";

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

    static 新本能函数端口装配状态 装配服务对象新需求端口(
        std::shared_ptr<新服务对象新需求本能端口>) noexcept;
    static 新本能函数端口装配状态 关闭服务对象新需求端口(
        新服务对象新需求本能端口&) noexcept;
    static 新本能函数端口装配状态 装配服务对象获取端口(
        std::shared_ptr<新服务对象获取本能端口>) noexcept;
    static 新本能函数端口装配状态 关闭服务对象获取端口(
        新服务对象获取本能端口&) noexcept;
    static 新本能函数端口装配状态 装配进入场景端口(
        std::shared_ptr<新进入场景本能端口>) noexcept;
    static 新本能函数端口装配状态 关闭进入场景端口(
        新进入场景本能端口&) noexcept;
    static 新本能函数端口装配状态 装配字段询问端口(std::shared_ptr<新字段询问登记端口>) noexcept;
    static 新本能函数端口装配状态 关闭字段询问端口(新字段询问登记端口&) noexcept;

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
    static std::shared_ptr<新字段询问登记端口> 租用字段询问端口() noexcept;
    static std::shared_ptr<新服务对象获取本能端口> 租用服务对象获取端口() noexcept;
    static std::shared_ptr<新进入场景本能端口> 租用进入场景端口() noexcept;
    static std::shared_ptr<新服务对象新需求本能端口> 租用服务对象新需求端口() noexcept;

    friend class 新_本能函数集;
    friend class 新_方法类;
};

} // namespace 海中鱼巣
