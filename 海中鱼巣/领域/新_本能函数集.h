#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

#include "新_方法参数场景.h"

namespace 海中鱼巣 {

// 这是稳定的程序内登记名，不是自然语言方法名称，也不作为能力判断依据。
inline constexpr std::string_view 新本能函数登记名_读取网页对话消息 =
    "builtin.web_dialog.read_messages.v1";
inline constexpr std::string_view 新本能函数登记名_设置网页对话输入文本 =
    "builtin.web_dialog.set_input_text.v1";
inline constexpr std::string_view 新本能函数登记名_提交网页对话消息 =
    "builtin.web_dialog.submit_message.v1";

enum class 新本能函数动作阶段 : std::uint8_t {
    未进入动作 = 1,
    已进入动作,
    部分执行,
    已按函数合同返回,
    结果未知
};

enum class 新本能函数执行状态 : std::uint8_t {
    已按合同返回 = 1,
    异步已受理,
    入口拒绝,
    函数未登记,
    参数不匹配,
    依赖未装配,
    动作执行失败,
    动作结果未知,
    资源失败,
    内部错误
};

// 函数报告只说明本次函数调用实际到达的位置及已经形成的正式材料。
// 它不证明任务完成、需求满足、预测命中、服务结算或现实效果成立。
struct 新本能函数执行结果 final {
    新本能函数执行状态 状态 = 新本能函数执行状态::入口拒绝;
    新本能函数动作阶段 动作阶段 = 新本能函数动作阶段::未进入动作;
    std::vector<稳定编码> 已形成状态节点组;
    std::vector<稳定编码> 已形成动态节点组;
    std::vector<稳定编码> 建议观察特征节点组;
};

using 新本能函数 = 新本能函数执行结果 (*)(
    const 方法参数场景& 参数场景) noexcept;

// 网页宿主或浏览器适配层提供这三个入口。本结构只传函数，不传DOM对象、
// 浏览器句柄、文本副本或持久状态；外设提供者必须比所有调用存活更久。
// 设置输入框只改变待提交内容；提交后仍须通过读取消息独立确认实际结果。
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
    内部错误
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

// 只维护“稳定登记名 -> C++函数入口”的进程内注册表。
// 方法节点继续只保存登记名；函数地址不写入基础数据集。
class 新_本能函数集 final {
public:
    // 登记首批内置入口。重复调用不重复登记。
    static 新本能函数登记状态 初始化() noexcept;

    // 进程内只允许一个真实网页对话端口；同一端口可重复装配，不允许替换。
    static 新本能函数端口装配状态 装配网页对话端口(
        const 新本能网页对话函数端口& 端口) noexcept;

    // 同名同入口返回精确重复；同名不同入口拒绝，已登记入口不允许替换。
    static 新本能函数登记状态 登记(
        std::string_view 登记名,
        新本能函数 函数) noexcept;

    static 新本能函数解析结果 解析(
        std::string_view 登记名) noexcept;

    // 只检查登记入口及其不可变宿主依赖是否已经装配，不执行现实动作。
    // 已装配端口在进程生命周期内不可替换或撤销，因此通过后不会因
    // 本端口注册表自身发生回退。
    static 新本能函数可用状态 检查可用(
        std::string_view 登记名) noexcept;

    // 每次只调用一次已解析入口；本函数不自动重试现实动作。
    static 新本能函数执行结果 执行(
        std::string_view 登记名,
        const 方法参数场景& 参数场景) noexcept;
};

} // namespace 海中鱼巣
