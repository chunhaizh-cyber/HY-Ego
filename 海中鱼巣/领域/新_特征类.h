#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <variant>
#include <vector>

#include "../核心/基础数据集.h"
#include "新_特征值类.h"
#include "概念_特征类.h"

namespace 海中鱼巣 {

using 新特征准确值 = 特征概念准确值;

struct 新特征值命中信息 final {
    新特征准确值 值;
    std::int64_t 命中次数 = 0;
    friend bool operator==(const 新特征值命中信息&,
        const 新特征值命中信息&) = default;
};

struct 新特征信息 final {
    稳定编码 节点;
    // 顶层实例特征没有上级特征；归组形成的子特征才记录上级特征节点。
    // 存在、场景等业务持有关系由对应数据类通过内部字段维护。
    std::optional<稳定编码> 上级特征节点;
    稳定编码 特征概念节点;
    // 调用者只掌握区间材料、尚无准确当前值时使用；它不是历史聚合结果。
    std::optional<特征概念值域> 实例区间值;
    std::optional<特征概念值域> 实例历史聚合值域;
    std::optional<稳定编码> 实例单位;
    std::optional<新特征准确值> 当前值;
    std::optional<新特征准确值> 标准值;
    std::vector<新特征准确值> 历史不同值;
    std::vector<新特征值命中信息> 历史值命中信息;
    friend bool operator==(const 新特征信息&, const 新特征信息&) = default;
};

// 本次匹配采用的值式定义，不携带名称、统计或可变概念树引用视图。
struct 新特征匹配定义快照 final {
    稳定编码 概念节点;
    稳定编码 特征类型根;
    特征概念材料物理类型 材料类型 = 特征概念材料物理类型::I64标量;
    std::optional<稳定编码> 单位;
    特征概念比较规则 比较规则 = 特征概念比较规则::I64数值;
    特征概念值域 值域;
    friend bool operator==(const 新特征匹配定义快照&,
        const 新特征匹配定义快照&) = default;
};

struct 新特征匹配实例快照 final {
    稳定编码 特征节点;
    std::optional<稳定编码> 上级特征节点;
    新特征匹配定义快照 定义;
    std::optional<稳定编码> 实例单位;
    std::optional<新特征准确值> 当前值;
    // 非存在引用的编码准确值携带不可变完整材料；I64和存在引用不携带。
    std::optional<新特征值信息> 当前完整材料;
    std::optional<特征概念值域> 实例区间值;
    friend bool operator==(const 新特征匹配实例快照&,
        const 新特征匹配实例快照&) = default;
};

enum class 新特征写入状态 : std::uint8_t {
    已完成 = 1,
    无变化 = 2,
    入口拒绝 = 3,
    特征不存在 = 4,
    概念不存在 = 5,
    材料类型不相容 = 6,
    结构不一致 = 7,
    值已更新但聚合未完成 = 8,
    资源失败 = 9,
    联合更新占用 = 10,
    前提已变化 = 11
};

struct 新特征值添加结果 final {
    新特征写入状态 状态 = 新特征写入状态::入口拒绝;
    bool 当前值已改变 = false;
    bool 历史集合已增加 = false;
    bool 命中次数已增加 = false;
    bool 标准值已改变 = false;
    std::optional<稳定编码> 接收子特征节点;
    std::optional<特征概念聚合结果> 聚合结果;
};
struct 新特征建立结果 final {
    新特征写入状态 状态 = 新特征写入状态::入口拒绝;
    std::optional<稳定编码> 特征节点;
};

enum class 新特征I64准备状态 : std::uint8_t {
    已准备 = 1, 入口拒绝, 特征不存在, 概念不存在,
    材料类型不相容, 预期已变化, 联合更新占用, 资源失败
};

// 同线程、同owner的一次更新凭据；不暴露锁或可独立提交的自有片段。
class 新特征I64更新准备 final {
public:
    新特征I64更新准备(新特征I64更新准备&&) noexcept;
    新特征I64更新准备& operator=(新特征I64更新准备&&) noexcept;
    ~新特征I64更新准备() noexcept;
    新特征I64更新准备(const 新特征I64更新准备&) = delete;
    新特征I64更新准备& operator=(const 新特征I64更新准备&) = delete;
    稳定编码 特征节点() const noexcept;
    std::optional<std::int64_t> 原当前值() const noexcept;
    std::int64_t 新当前值() const noexcept;
    bool 核心有变化() const noexcept;
private:
    struct 实现;
    explicit 新特征I64更新准备(std::unique_ptr<实现>) noexcept;
    std::unique_ptr<实现> 实现_;
    friend class 新_特征类;
};

struct 新特征I64准备结果 final {
    新特征I64准备状态 状态 = 新特征I64准备状态::入口拒绝;
    std::optional<新特征I64更新准备> 准备;
};

struct 新特征I64提交结果 final {
    基础节点组追加状态 状态 = 基础节点组追加状态::入口拒绝;
    std::vector<基础片段发布回执> 附加片段回执组;
};

class 新特征引用集合更新准备 final {
public:
    新特征引用集合更新准备(新特征引用集合更新准备&&) noexcept;
    新特征引用集合更新准备& operator=(新特征引用集合更新准备&&) noexcept;
    ~新特征引用集合更新准备() noexcept;
    新特征引用集合更新准备(const 新特征引用集合更新准备&) = delete;
    新特征引用集合更新准备& operator=(const 新特征引用集合更新准备&) = delete;
private:
    struct 实现;
    explicit 新特征引用集合更新准备(std::unique_ptr<实现>) noexcept;
    std::unique_ptr<实现> 实现_;
    friend class 新_特征类;
};

enum class 新特征引用集合保护状态 : std::uint8_t {
    已读取=1, 前提已变化, 目标不合法, 结构不一致, 资源失败
};
struct 新特征引用集合保护信息 final {
    新特征信息 顶层特征;
    稳定编码 特征类型根, 当前准确材料节点;
    std::vector<稳定编码> 当前需求节点组;
    std::vector<基础联合节点快照> 完整保护组;
};
struct 新特征引用集合保护结果 final {
    新特征引用集合保护状态 状态 = 新特征引用集合保护状态::目标不合法;
    std::optional<新特征引用集合保护信息> 信息;
};
struct 新特征引用集合准备结果 final {
    新特征I64准备状态 状态 = 新特征I64准备状态::入口拒绝;
    std::optional<新特征引用集合更新准备> 准备;
};

struct 新特征引用集合提交结果 final {
    基础节点组追加状态 状态 = 基础节点组追加状态::入口拒绝;
    std::vector<基础片段发布回执> 附加片段回执组;
};

enum class 新特征比较状态 : std::uint8_t {
    已完成 = 1,
    入口拒绝 = 2,
    特征不存在 = 3,
    概念不存在 = 4,
    概念不相容 = 5,
    单位不相容 = 6,
    材料类型不相容 = 7,
    值不存在 = 8,
    结构不一致 = 9,
    资源失败 = 10,
    规则不支持 = 11,
    前提已变化 = 12
};

enum class 新特征比较材料 : std::uint8_t {
    双准确值 = 1,
    左准确值与右历史域 = 2,
    左历史域与右准确值 = 3,
    双历史聚合值域 = 4,
    仅特征概念 = 5
};

enum class 新特征比较数值类型 : std::uint8_t {
    有向差值 = 1,
    相似度 = 2,
    精确判等 = 3
};

struct 新特征比较结果 final {
    新特征比较状态 状态 = 新特征比较状态::入口拒绝;
    std::optional<新特征比较材料> 材料;
    std::optional<新特征比较数值类型> 数值类型;
    // 有向差值采用饱和I64；相似度为0～10000，-1表示没有适用算法。
    // 精确判等仅为10000（相同）或0（不同），不表示大小或接近程度。
    std::optional<std::int64_t> 比较值;
    // 仅用于“一个准确值与另一方历史聚合值域”的比较。
    std::optional<bool> 准确值命中值域;
    // 仅用于双历史聚合值域比较。
    std::optional<特征概念值域关系> 值域关系;
    // 双准确值比较会先尝试形成这一单特征概念关系，再给出数值兜底。
    std::optional<单特征概念关系> 抽象概念关系;
    std::optional<稳定编码> 抽象共同上位;
};

struct 新特征概念值域关系结果 final {
    新特征比较状态 状态 = 新特征比较状态::入口拒绝;
    std::optional<特征概念值域关系> 关系;
};

struct 新特征准确I64比较材料 final {
    std::int64_t 值 = 0;
};

struct 新特征直接I64值域比较材料 final {
    特征概念I64闭区间 值域;
};

struct 新特征概念值域比较材料 final {
    稳定编码 概念节点;
};

struct 新特征准确值比较材料 final { 新特征准确值 值; };
struct 新特征直接值域比较材料 final { 特征概念值域 值域; };

struct 新特征值域比较输入 final {
    // 根或同树子概念提供类型语义，不限制实际材料必须落在该概念域内。
    稳定编码 特征类型概念节点;
    // 空表示无单位，不表示继承或通配。
    std::optional<稳定编码> 单位;
    std::variant<std::monostate, 新特征准确I64比较材料,
        新特征直接I64值域比较材料, 新特征概念值域比较材料,
        新特征准确值比较材料, 新特征直接值域比较材料> 材料;
};

struct 新特征值域关系比较结果 final {
    新特征比较状态 状态 = 新特征比较状态::入口拒绝;
    // 沿用特征概念值域关系：1相等、2左包含右、3右包含左、4相交、5分离。
    // 这是关系身份，不是有向差值或相似度；非成功不携带数值。
    std::optional<std::int64_t> 关系编码;
};

struct 新特征概念相似度结果 final {
    新特征比较状态 状态 = 新特征比较状态::入口拒绝;
    // 0～10000；不支持的材料或无法形成代表材料时不返回数值。
    std::optional<std::int64_t> 相似度;
};

enum class 新特征清理准备状态 : std::uint8_t {
    已准备 = 1, 联合更新占用, 仍被外部引用, 入口拒绝, 结构不一致, 资源失败
};
class 新特征清理准备 final {
public:
    ~新特征清理准备();
    新特征清理准备(新特征清理准备&&) noexcept;
    新特征清理准备& operator=(新特征清理准备&&) noexcept;
    新特征清理准备(const 新特征清理准备&) = delete;
    新特征清理准备& operator=(const 新特征清理准备&) = delete;
private:
    struct 实现;
    explicit 新特征清理准备(std::unique_ptr<实现>);
    std::unique_ptr<实现> 实现_;
    friend class 新_特征类;
};
struct 新特征清理准备结果 final {
    新特征清理准备状态 状态 = 新特征清理准备状态::入口拒绝;
    std::optional<新特征清理准备> 准备;
};

class 新_特征类 final {
public:
    新特征引用集合保护结果 捕获引用集合更新保护(稳定编码) const noexcept;
    新特征引用集合准备结果 准备顶层引用集合更新(稳定编码 特征节点, 稳定编码 新准确材料节点) noexcept;
    新特征引用集合提交结果 提交顶层引用集合更新(新特征引用集合更新准备&,
        const std::vector<基础封存追加片段>&) noexcept;
    新特征值添加结果 完成顶层引用集合更新(新特征引用集合更新准备&) noexcept;
    void 结束顶层引用集合更新(新特征引用集合更新准备&) noexcept;
    void 取消顶层引用集合更新(新特征引用集合更新准备&) noexcept;
    // 两个依赖服务必须操作全局基础数据集，并且比本对象存活更久。
    新_特征类(
        新_特征值类& 特征值服务,
        概念_特征类& 特征概念服务) noexcept;

    void 装配存在引用读取接口(
        const 特征存在引用读取接口& 读取接口) noexcept;

    // 建立本类共享的类型节点与字段节点；重复调用不会重复建立。
    bool 初始化() noexcept;

    // 建立独立的实例特征节点。业务持有者由存在类、场景类等上层数据类
    // 通过自己的内部字段引用该节点，本类不建立业务外部父子关系。
    // 提供实际值时，会在同一特征类型概念图中选择包含该值的最具体概念
    // 作为模板；本函数只建立节点，不写入实际值。
    稳定编码 建立特征(
        稳定编码 特征概念节点,
        std::optional<新特征准确值> 实际值 = std::nullopt) noexcept;
    新特征建立结果 建立特征强类型(
        稳定编码 特征概念节点,
        std::optional<新特征准确值> 实际值 = std::nullopt) noexcept;
    新特征写入状态 准备特征初值保护(
        稳定编码 特征概念节点,
        std::optional<新特征准确值> 实际值 = std::nullopt) noexcept;

    // 使用既有特征概念和显式区间值建立实例特征。当前值、标准值、
    // 历史命中和历史聚合值域均保持为空；本函数不生成状态。
    稳定编码 建立区间值特征(
        稳定编码 特征概念节点,
        const 特征概念值域& 区间值) noexcept;

    std::optional<新特征信息> 获取特征(
        稳定编码 特征节点) const noexcept;

    // 捕获只读物理投影，不调用普通getter的当前/历史归属验证。
    // 顶层请求不存在返回空；已声明引用或物理结构损坏走工程终止。
    std::optional<新特征匹配定义快照> 捕获特征匹配定义(
        稳定编码 特征概念节点) const noexcept;
    std::optional<新特征匹配实例快照> 捕获特征匹配实例(
        稳定编码 特征节点) const noexcept;
    // 只用传入快照，不读取当前E/F/EC/FC或调用业务归属/集合提供者。
    新特征值域关系比较结果 比较捕获特征值域(
        const 新特征匹配实例快照& 实例,
        const 新特征匹配定义快照& 目标) const noexcept;

    bool 是特征节点(稳定编码 节点) const noexcept;

    // 仅供同次私有创建或明确未采用回收：核验初始单值I64完整私有闭包。
    // 只读；不符时工程终止，不授予删除权限，也不保留跨调用锁。
    void 核验初建I64特征可回收(
        稳定编码 特征节点, 稳定编码 预期特征概念节点,
        std::int64_t 初始值,
        const std::vector<稳定编码>& 允许外部入向字段关系组) const noexcept;

    // 调用方必须先解除业务持有者对该特征节点的内部字段引用。
    // 本函数递归删除归组子特征，并同步减少正式特征概念引用。
    bool 删除特征(稳定编码 特征节点) noexcept;
    新特征清理准备结果 准备特征组清理(
        const std::vector<稳定编码>& 根特征组,
        const std::vector<稳定编码>& 允许解除入向字段关系组) noexcept;
    void 执行已准备特征组清理(新特征清理准备&) noexcept;

    // 更新当前值；每次调用给该准确值增加一次命中次数，首次出现时加入历史集合。
    // 标准值首次出现时建立；以后只有候选命中次数严格超过当前标准值时才替换。
    // 每次调用都尝试使历史聚合值域与完整历史集合一致，并以实际历史值域
    // 持续修正正式所属概念；概念值域不限制实际值写入。
    新特征值添加结果 添加特征值(
        稳定编码 特征节点,
        const 新特征准确值& 新值) noexcept;

    新特征I64准备结果 准备顶层I64更新(
        稳定编码 特征节点, std::int64_t 新值) noexcept;
    新特征I64提交结果 提交顶层I64更新(
        新特征I64更新准备& 准备,
        const std::vector<基础封存追加片段>& 附加片段组) noexcept;
    新特征值添加结果 完成顶层I64更新(新特征I64更新准备& 准备) noexcept;
    void 结束顶层I64更新(新特征I64更新准备& 准备) noexcept;
    void 取消顶层I64更新(新特征I64更新准备& 准备) noexcept;

    // 使用给定特征节点的材料类型、单位和比较规则比较两个准确值。
    新特征比较结果 比较特征值(
        稳定编码 特征节点,
        const 新特征准确值& 左值,
        const 新特征准确值& 右值) const noexcept;

    // 不要求先建立实例特征。二次特征等派生计算在已经明确特征概念时，
    // 可直接复用该概念的材料类型、单位和比较规则。
    新特征比较结果 按概念比较特征值(
        稳定编码 特征概念节点,
        const 新特征准确值& 左值,
        const 新特征准确值& 右值) const noexcept;

    // 完整读回两个实例特征；只使用准确值、历史聚合值域或概念作比较材料。
    新特征比较结果 比较特征(
        稳定编码 左特征节点,
        稳定编码 右特征节点) const noexcept;

    // 只读比较：准确I64、直接I64闭区间或概念引用；不建立或修改概念。
    // 不建立跨公开读取入口的原子快照；不进行单位换算。
    新特征值域关系比较结果 比较特征值域(
        const 新特征值域比较输入& 左,
        const 新特征值域比较输入& 右) const noexcept;

    // 集合满足与完整材料判等分离；这里只核验类型，成员算法由概念owner唯一提供。
    特征任务集合条件比较结果 比较任务集合条件(
        稳定编码 特征类型概念节点,
        const 新特征准确值& 当前值,
        const 特征概念任务集合值域& 目标) const noexcept;

    // 只读计算后集合中不属于前集合的成员数；不解释任务来源或修改集合。
    // 仅接受先天服务任务集合类型，顺序和重复成员不改变结果。
    新特征比较结果 计算任务集合新增数(稳定编码 特征类型根,
        const 新特征准确值& 前, const 新特征准确值& 后) const noexcept;

    // 三个入口都只接受同一特征类型根内的概念，不处理存在概念组合。
    新特征概念值域关系结果 比较特征概念值域关系(
        稳定编码 左概念节点,
        稳定编码 右概念节点) const noexcept;

    新特征概念相似度结果 计算特征概念值域重叠度(
        稳定编码 左概念节点,
        稳定编码 右概念节点) const noexcept;

    新特征概念相似度结果 计算特征概念材料接近度(
        稳定编码 左概念节点,
        稳定编码 右概念节点) const noexcept;

private:
    新特征I64准备结果 准备顶层准确值更新(
        稳定编码 特征节点, const 新特征准确值& 新值, bool 引用集合) noexcept;
    新特征值添加结果 完成添加值归组(
        const 新特征信息& 原特征, const 新特征准确值& 新值,
        稳定编码 活动记录节点, 新特征值添加结果 结果,
        const void* 联合凭据 = nullptr) noexcept;
    // 只完成已经确定的“子值域概念 -> 子实例特征”建立链。
    // 何时判定需要形成子值域、哪些历史值进入子特征，由后续策略决定。
    稳定编码 建立或取得子特征节点(
        稳定编码 父特征节点,
        const 特征概念值域& 子值域) noexcept;

    新_特征值类& 特征值服务_;
    概念_特征类& 特征概念服务_;
    const 特征存在引用读取接口* 存在引用读取接口_ = nullptr;
};

} // namespace 海中鱼巣
