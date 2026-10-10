#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <vector>

#include "新_方法公共类型.h"
#include "新_方法参数场景.h"
#include "新_本能函数集.h"

namespace 海中鱼巣 {

class 新_存在类;
class 新_场景类;
class 新_特征类;
class 概念_存在类;
class 概念_特征类;
class 新_任务类;

// 方法拥有自己的独立根；该根不挂入世界树、需求树或任务集合根。
extern 稳定编码 新方法组织根节点;

struct 新方法参数场景结果 final {
    新方法操作状态 状态 = 新方法操作状态::入口拒绝;
    std::shared_ptr<const 方法参数场景> 参数场景;
};

enum class 新方法本能调用状态 : std::uint8_t {
    已调用 = 1,
    入口拒绝,
    方法不存在,
    方法停用,
    不是本能方法,
    参数场景不匹配,
    函数未登记,
    资源失败,
    内部错误
};

struct 新方法本能调用结果 final {
    新方法本能调用状态 状态 = 新方法本能调用状态::入口拒绝;
    std::optional<新本能函数本次条件结果对> 本次条件结果对;
};

class 新_方法类 final {
public:
    新_方法类(
        新_存在类& 存在服务,
        新_场景类& 场景服务,
        新_特征值类& 特征值服务,
        新_特征类& 特征服务,
        概念_存在类& 存在概念服务,
        概念_特征类& 特征概念服务) noexcept;

    // 只建立独立方法组织根以及本类使用的类型、字段节点。
    bool 初始化() noexcept;

    // 建立方法知识，不登记或调用本能函数。
    新方法建立结果 建立方法知识(
        const 新方法知识建立请求& 请求) noexcept;

    // 只建立“方法节点 -> 稳定登记名 -> C++函数入口”的调用绑定。
    // 不读取或写入参数规格、条件结果和方法虚拟存在内容，也不调用函数。
    新方法操作状态 登记本能函数(
        稳定编码 方法节点,
        const std::string& 本能函数登记名) noexcept;
    std::optional<新方法信息> 获取方法(
        稳定编码 方法节点) const noexcept;
    // 完整复制内部结构及被调用方法闭包；不裁剪路径、不复制外部概念。
    新方法复制结果 复制方法(稳定编码 来源方法节点) noexcept;
    新方法混合实例结果 建立混合方法实例(
        const std::vector<稳定编码>& 来源方法节点组) noexcept;
    // 旧单方法副本根投影为一个候选；混合根本身不是可调用方法。
    std::optional<新方法混合实例信息> 获取混合方法实例(
        稳定编码 实例根节点) const noexcept;
    新方法实例配对结果 查询实例条件结果(
        稳定编码 实例根节点) const noexcept;
    bool 是方法节点(稳定编码 节点) const noexcept;
    新方法候选结果 查询候选方法(
        const 新方法候选查询& 查询) const noexcept;

    新方法操作状态 修改启用状态(
        稳定编码 方法节点,
        新方法启用状态 新状态) noexcept;

    新方法结构节点结果 添加组合步骤(
        const 新方法组合步骤添加请求& 请求) noexcept;
    新方法操作状态 设置组合入口步骤(
        稳定编码 组合方法节点,
        稳定编码 步骤节点) noexcept;
    新方法结构节点结果 添加结果转移(
        const 新方法结果转移添加请求& 请求) noexcept;

    // 形成一次调用期只读视图。这里仅绑定并校验实时节点，不复制当前值。
    新方法参数场景结果 建立参数场景(
        稳定编码 方法节点,
        std::vector<新方法参数角色绑定> 角色绑定组) const noexcept;

    // 只在参数场景已经由本方法建立且方法仍启用时解析登记名并调用一次。
    // 本函数不建立任务执行实例，不判断需求满足，也不自动重试现实动作。
    新方法本能调用结果 调用本能函数(
        稳定编码 方法节点,
        const 方法参数场景& 参数场景) const noexcept;

private:
    friend class 新_任务类;
    enum class 新方法实例交付状态 : std::uint8_t {
        已采用 = 1, 未采用, 采用状态不明
    };
    新方法混合实例结果 建立并交付混合方法实例(
        const std::vector<稳定编码>& 来源方法节点组,
        const std::function<新方法实例交付状态(稳定编码)>& 采用) noexcept;
    新方法复制结果 复制方法闭包(
        const std::vector<稳定编码>& 来源方法节点组, bool 混合,
        const std::function<新方法实例交付状态(稳定编码)>& 采用 = {}) noexcept;

    新_存在类& 存在服务_;
    新_场景类& 场景服务_;
    新_特征值类& 特征值服务_;
    新_特征类& 特征服务_;
    概念_存在类& 存在概念服务_;
    概念_特征类& 特征概念服务_;
};

} // namespace 海中鱼巣
