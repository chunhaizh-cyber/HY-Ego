#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <vector>

#include "新_方法公共类型.h"
#include "新_方法参数场景.h"
#include "新_本能函数集.h"
#include "新_内部治理对象引用.h"

namespace 海中鱼巣 {

class 新_存在类;
class 新_场景类;
class 新_特征类;
class 概念_存在类;
class 概念_特征类;
class 新_任务类;
class 新_状态类;

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

class 新_方法类 final : public 新方法治理对象读取接口 {
public:
    新_方法类(
        新_存在类& 存在服务,
        新_场景类& 场景服务,
        新_特征值类& 特征值服务,
        新_特征类& 特征服务,
        概念_存在类& 存在概念服务,
        概念_特征类& 特征概念服务,
        新_状态类& 状态服务) noexcept;

    // 只建立独立方法组织根以及本类使用的类型、字段节点。
    bool 初始化() noexcept;
    // 独立缺失列表：不建M/Vm/条件结果，不进方法根，不注册或调用函数。
    // 同一来源和规范化定义重复时复用。数据集生命周期内保留，不自动晋升/删除。
    新缺失方法登记结果 登记缺失方法(const 新缺失方法登记请求&) noexcept;
    // nullopt仅表示该身份不在列表；工程读取错误终止，不伪装成空列表。
    std::optional<新缺失方法信息> 读取缺失方法(稳定编码 方法头节点) const noexcept;
    std::vector<新缺失方法信息> 查询缺失方法列表() const noexcept;
    bool 装配内部治理世界(新方法治理世界协作接口&) noexcept;
    void 初始化治理场景组织() noexcept;
    std::optional<新方法虚拟存在读回> 读取方法虚拟存在引用(
        const 新方法虚拟存在引用&) const noexcept override;

    // 建立方法知识，不登记或调用本能函数。
    新方法建立结果 建立方法知识(
        const 新方法知识建立请求& 请求) noexcept;
    // 每次接受一对：完整条件/结果判重，无同条件时追加，模拟不增加命中。
    新方法学习发布结果 追加模拟条件结果(
        稳定编码 方法节点,
        const std::vector<新方法条件结果定义>& 新配对定义组) noexcept;

    // 以登记名定位自身；仅新建时发布初始知识，不覆盖后来学习的配对。
    新方法建立结果 确保本能方法登记(
        const std::string& 登记名,
        const 新方法知识建立请求& 请求) noexcept;
    // 仅退役已完整识别的旧网页方法族；历史方法知识保持不变。
    新方法建立结果 退役旧网页本能登记(const std::string& 登记名) noexcept;
    新方法建立结果 查询本能登记(
        const std::string& 登记名) const noexcept;
    新方法建立结果 确保服务结算Q方法登记(
        const 新方法知识建立请求& 新请求) noexcept;
    std::optional<稳定编码> 读取服务结算固定加一父代(
        稳定编码 当前Q方法) const noexcept;

    // 只建立“方法节点 -> 稳定登记名 -> C++函数入口”的调用绑定。
    // 不读取或写入参数规格、条件结果和方法虚拟存在内容，也不调用函数。
    新方法操作状态 登记本能函数(
        稳定编码 方法节点,
        const std::string& 本能函数登记名) noexcept;
    std::optional<新方法信息> 获取方法(
        稳定编码 方法节点) const noexcept;
    enum class 执行结构读取状态 : std::uint8_t {
        已读取 = 1, 入口拒绝, 尚未初始化, 方法不存在, 来源不是抽象方法
    };
    struct 执行结构读取结果 final {
        执行结构读取状态 状态 = 执行结构读取状态::入口拒绝;
        std::vector<稳定编码> 根方法节点组;
        std::vector<新方法信息> 方法组;
    };
    // 一次方法锁内读完整调用图；不复制知识、不授予执行资格。
    执行结构读取结果 读取方法执行结构闭包(
        const std::vector<稳定编码>& 根方法节点组) const noexcept;
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
        std::vector<新方法参数角色绑定> 角色绑定组,
        const 新内部治理对象读取接口* 治理对象读取 = nullptr,
        const 新服务要求材料读取接口* 服务要求读取 = nullptr) const noexcept;
    新方法参数场景结果 建立本能登记参数场景(
        std::vector<新方法参数角色绑定> 角色绑定组) const noexcept;
    新方法参数场景结果 建立服务结算治理参数场景(
        稳定编码 方法节点, const 新任务贡献治理参数&,
        const 新内部治理对象读取接口& 治理对象读取) const noexcept;
    // 服务需求两入口的登记专用空容器，不放入任何运行实参。
    新方法参数场景结果 建立服务需求登记参数场景() const noexcept;
    新方法参数场景结果 建立模拟参数场景(
        稳定编码 方法节点,
        std::vector<新方法参数角色绑定> 绑定,
        std::shared_ptr<新方法模拟运行接口> 模拟) const noexcept;

    // 只在参数场景已经由本方法建立且方法仍启用时解析登记名并调用一次。
    // 本函数不建立任务执行实例，不判断需求满足，也不自动重试现实动作。
    新方法本能调用结果 调用本能函数(
        稳定编码 方法节点,
        const 方法参数场景& 参数场景) const noexcept;

private:
    friend class 新_任务类;
    // 仅标记本次私有准备是否已经完整交付，不作为长期业务状态。
    struct 方法虚拟存在准备 final {
        新方法虚拟存在读回 信息;
        bool 完整准备 = false;
    };
    void 核验治理组织就绪() const;
    方法虚拟存在准备 准备方法虚拟存在(
        稳定编码 方法, 稳定编码 虚拟存在, std::int64_t 启用);
    void 核验方法私有材料未消费(const 方法虚拟存在准备&,
        std::optional<新方法治理场景信息> 本批场景 = std::nullopt) const;
    void 清理未绑定方法特征(const 方法虚拟存在准备&) noexcept;
    void 同步方法专属概念(const 新方法虚拟存在引用&) noexcept;
    新方法建立结果 发布方法结构(const 新方法知识建立请求&,
        const std::string* 登记名, const 新方法信息* 替代父代 = nullptr) noexcept;
    void 回收本批方法图(const std::vector<稳定编码>& 节点,
        const std::vector<稳定编码>& 字段,
        const std::vector<方法虚拟存在准备>& 虚拟组,
        bool 已发布) noexcept;
    enum class 新方法实例条件覆盖状态 : std::uint8_t {
        已覆盖 = 1, 无变化, 入口拒绝, 来源已变化,
        值域不支持, 资源失败, 已生效但结构不一致, 结构不一致
    };
    struct 新方法实例条件覆盖结果 final {
        新方法实例条件覆盖状态 状态 = 新方法实例条件覆盖状态::入口拒绝;
        bool 已生效 = false;
        std::optional<新方法条件结果信息> 当前配对;
    };
    新方法实例条件覆盖结果 覆盖实例配对条件值域(
        稳定编码 实例根节点, 稳定编码 方法节点,
        const 新方法条件结果信息& 预期配对,
        const std::vector<特征概念I64闭区间>& 所需条件域组) noexcept;

    enum class 新方法实例交付状态 : std::uint8_t {
        已采用 = 1, 未采用, 采用状态不明
    };
    enum class 新方法实例增补状态 : std::uint8_t {
        已扩充 = 1, 精确重复, 入口拒绝, 尚未初始化,
        实例不存在, 来源方法不存在, 来源无法对齐,
        结构不一致, 资源失败, 未采用, 发布结果未知
    };
    struct 新方法实例增补结果 final {
        新方法实例增补状态 状态 = 新方法实例增补状态::入口拒绝;
        std::optional<稳定编码> 实例根节点;
    };
    新方法实例增补结果 扩充并交付混合方法实例(
        稳定编码 预期旧实例根,
        const std::vector<稳定编码>& 候选抽象方法组,
        const std::function<新方法实例交付状态(稳定编码)>& 采用) noexcept;
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
    新_状态类& 状态服务_;
    新方法治理世界协作接口* 内部治理世界_ = nullptr;
    bool 治理组织已就绪_ = false;
};

} // namespace 海中鱼巣
