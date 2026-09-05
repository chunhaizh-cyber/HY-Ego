module;

#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <map>
#include <mutex>
#include <optional>
#include <set>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.领域.数据服务.概念树类;

export import 海中鱼巣.领域.数据服务.存在类;

export namespace 海中鱼巣 {

struct 概念树概念身份 final {
    稳定编码 值{};
    概念树概念身份() = default;
    explicit 概念树概念身份(稳定编码 编码) : 值(编码) {
    }
    friend bool operator==(const 概念树概念身份 &, const 概念树概念身份 &) = default;
};
struct 概念树规则身份 final {
    稳定编码 值{};
    概念树规则身份() = default;
    explicit 概念树规则身份(稳定编码 编码) : 值(编码) {
    }
    friend bool operator==(const 概念树规则身份 &, const 概念树规则身份 &) = default;
};
struct 概念树存在引用 final {
    稳定编码 值{};
    概念树存在引用() = default;
    explicit 概念树存在引用(稳定编码 编码) : 值(编码) {
    }
    friend bool operator==(const 概念树存在引用 &, const 概念树存在引用 &) = default;
};
struct 概念树特征类型引用 final {
    稳定编码 值{};
    概念树特征类型引用() = default;
    explicit 概念树特征类型引用(稳定编码 编码) : 值(编码) {
    }
    friend bool operator==(const 概念树特征类型引用 &, const 概念树特征类型引用 &) = default;
};
struct 概念树特征引用 final {
    稳定编码 值{};
    概念树特征引用() = default;
    explicit 概念树特征引用(稳定编码 编码) : 值(编码) {
    }
    friend bool operator==(const 概念树特征引用 &, const 概念树特征引用 &) = default;
};
using 概念树世界引用 = std::variant<概念树存在引用, 概念树特征引用>;
using 概念树形成世界引用 = std::variant<概念树存在引用, 概念树特征引用, 概念树特征类型引用>;
using 概念树精确值 = std::variant<std::int64_t, std::vector<std::int64_t>, std::vector<std::uint64_t>>;
struct 概念树I64区间 final {
    std::int64_t 下界 = 0;
    std::int64_t 上界 = 0;
    friend bool operator==(const 概念树I64区间 &, const 概念树I64区间 &) = default;
};
using 概念树特征值域 = std::variant<概念树精确值, 概念树I64区间>;
struct 概念树特征定义 final {
    概念树存在引用 形成宿主;
    概念树特征类型引用 特征类型;
    概念树特征值域 值域;
    friend bool operator==(const 概念树特征定义 &, const 概念树特征定义 &) = default;
};
struct 概念树存在定义 final {
    std::vector<概念树概念身份> 特征模板组;
    friend bool operator==(const 概念树存在定义 &, const 概念树存在定义 &) = default;
};
using 概念树定义 = std::variant<概念树特征定义, 概念树存在定义>;
struct 概念树来源项 final {
    概念树世界引用 世界事实;
    std::uint64_t 证据截止 = 0;
    friend bool operator==(const 概念树来源项 &, const 概念树来源项 &) = default;
};
struct 概念树读取头 final {
    std::uint32_t 合同版本 = 1;
    std::uint64_t Gread = 0;
    std::uint64_t H = 0;
    friend bool operator==(const 概念树读取头 &, const 概念树读取头 &) = default;
};
struct 概念树写入头 final {
    std::uint32_t 合同版本 = 1;
    std::uint64_t 期望事实代次 = 0;
    std::uint64_t 幂等身份 = 0;
    friend bool operator==(const 概念树写入头 &, const 概念树写入头 &) = default;
};
struct 概念树预算 final {
    std::uint64_t 最大概念数 = 0;
    std::uint64_t 最大关系数 = 0;
    std::uint64_t 最大来源数 = 0;
    std::uint64_t 最大支持数 = 0;
    std::uint64_t 最大世界成员数 = 0;
    std::uint64_t 最大特征属性数 = 0;
    friend bool operator==(const 概念树预算 &, const 概念树预算 &) = default;
};
enum class 概念树根角色 : std::uint8_t {
    存在 = 1,
    特征,
    动态,
    因果链
};
enum class 概念树生命周期状态 : std::uint8_t {
    活跃 = 1,
    冷却,
    退役
};
enum class 概念树上位操作 : std::uint8_t {
    新增 = 1,
    替换,
    退出
};
enum class 概念树数据状态 : std::uint8_t {
    已创建 = 1,
    精确重复,
    已读取,
    已添加支持,
    已退出支持,
    已添加上位,
    已替换上位,
    已退出上位,
    已迁移生命周期,
    已退出概念,
    已释放引用,
    无须变更,
    入口拒绝,
    未找到,
    目标已退出,
    引用冲突,
    形成环,
    根不可退出,
    事实代次漂移,
    幂等冲突,
    数量预算不足,
    历史材料不可用,
    资源失败,
    内部不一致,
    已可能发布
};
enum class 概念树发布状态 : std::uint8_t {
    未派发 = 1,
    已确认发布,
    可能已发布
};
struct 概念树根引用组 final {
    概念树概念身份 存在根;
    概念树概念身份 特征根;
    概念树概念身份 动态根;
    概念树概念身份 因果链根;
    friend bool operator==(const 概念树根引用组 &, const 概念树根引用组 &) = default;
};
struct 概念树结构类型 final {
    稳定编码 根成员{};
    稳定编码 规则归属{};
    稳定编码 概念根归属{};
    稳定编码 概念规则{};
    稳定编码 定义成员{};
    稳定编码 来源成员{};
    稳定编码 直接上位{};
    稳定编码 定义宿主{};
    稳定编码 定义特征类型{};
    稳定编码 定义模板{};
    稳定编码 来源存在{};
    稳定编码 来源特征{};
    稳定编码 支持归属{};
    稳定编码 支持概念{};
    稳定编码 支持存在{};
    稳定编码 支持特征{};
    稳定编码 格式版本{};
    稳定编码 规则版本{};
    稳定编码 根角色{};
    稳定编码 概念种类{};
    稳定编码 生命周期{};
    稳定编码 证据截止{};
    稳定编码 精确I64{};
    稳定编码 精确I64组{};
    稳定编码 精确U64组{};
    稳定编码 区间下界{};
    稳定编码 区间上界{};
    稳定编码 类型登记{};
    friend bool operator==(const 概念树结构类型 &, const 概念树结构类型 &) = default;
};
struct 概念树结构交付 final {
    稳定编码 格式锚点{};
    概念树根引用组 根组;
    概念树规则身份 签名规则;
    概念树结构类型 类型;
    friend bool operator==(const 概念树结构交付 &, const 概念树结构交付 &) = default;
};
struct 概念树生命周期 final {
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    friend bool operator==(const 概念树生命周期 &, const 概念树生命周期 &) = default;
};
struct 概念树直接上位事实 final {
    稳定编码 关系{};
    概念树概念身份 上位;
    概念树概念身份 下位;
    概念树生命周期 生命周期;
    friend bool operator==(const 概念树直接上位事实 &, const 概念树直接上位事实 &) = default;
};
struct 概念树来源事实 final {
    稳定编码 记录{};
    稳定编码 成员关系{};
    稳定编码 世界关系{};
    稳定编码 截止值事实{};
    概念树来源项 来源;
    概念树生命周期 记录生命周期;
    bool 当前世界引用已释放 = false;
    friend bool operator==(const 概念树来源事实 &, const 概念树来源事实 &) = default;
};
struct 概念树概念事实 final {
    概念树概念身份 概念;
    概念树根角色 根角色 = 概念树根角色::存在;
    概念树概念身份 本体根;
    bool 是本体根 = false;
    概念树规则身份 规则;
    std::optional<概念树定义> 定义;
    稳定编码 定义记录{};
    稳定编码 生命周期值事实{};
    概念树生命周期状态 治理状态 = 概念树生命周期状态::活跃;
    概念树生命周期 生命周期;
    std::vector<概念树来源事实> 来源组;
    std::vector<概念树直接上位事实> 直接上位组;
    friend bool operator==(const 概念树概念事实 &, const 概念树概念事实 &) = default;
};
struct 概念树支持事实 final {
    稳定编码 记录{};
    稳定编码 归属关系{};
    稳定编码 概念关系{};
    稳定编码 世界关系{};
    稳定编码 截止值事实{};
    概念树概念身份 目标概念;
    概念树世界引用 世界引用;
    std::uint64_t 证据H = 0;
    概念树生命周期 记录生命周期;
    friend bool operator==(const 概念树支持事实 &, const 概念树支持事实 &) = default;
};
struct 概念树形成引用事实 final {
    稳定编码 关系{};
    稳定编码 记录{};
    概念树概念身份 所属概念;
    概念树形成世界引用 世界引用;
    概念树生命周期 关系生命周期;
    friend bool operator==(const 概念树形成引用事实 &, const 概念树形成引用事实 &) = default;
};

struct 概念树概念建立请求 final {
    概念树写入头 写入头;
    概念树定义 定义;
    std::vector<概念树概念身份> 直接上位;
    std::vector<概念树来源项> 来源;
    概念树预算 预算;
    friend bool operator==(const 概念树概念建立请求 &, const 概念树概念建立请求 &) = default;
};
struct 概念树概念读取请求 final {
    概念树读取头 读取头;
    概念树概念身份 目标概念;
    概念树预算 预算;
    friend bool operator==(const 概念树概念读取请求 &, const 概念树概念读取请求 &) = default;
};
struct 概念树定义查找请求 final {
    概念树读取头 读取头;
    概念树定义 定义;
    概念树预算 预算;
    friend bool operator==(const 概念树定义查找请求 &, const 概念树定义查找请求 &) = default;
};
struct 概念树图读取请求 final {
    概念树读取头 读取头;
    概念树根角色 根角色 = 概念树根角色::存在;
    bool 包含支持 = false;
    概念树预算 预算;
    friend bool operator==(const 概念树图读取请求 &, const 概念树图读取请求 &) = default;
};
struct 概念树上位操作请求 final {
    概念树写入头 写入头;
    概念树上位操作 操作 = 概念树上位操作::新增;
    概念树概念身份 下位;
    std::optional<概念树概念身份> 旧上位;
    std::optional<概念树概念身份> 新上位;
    std::optional<稳定编码> 旧关系;
    概念树预算 预算;
    friend bool operator==(const 概念树上位操作请求 &, const 概念树上位操作请求 &) = default;
};
struct 概念树支持建立请求 final {
    概念树写入头 写入头;
    概念树概念身份 目标概念;
    概念树世界引用 世界引用;
    std::uint64_t 证据H = 0;
    概念树预算 预算;
    friend bool operator==(const 概念树支持建立请求 &, const 概念树支持建立请求 &) = default;
};
struct 概念树支持退出请求 final {
    概念树写入头 写入头;
    概念树支持事实 预期支持;
    概念树预算 预算;
    friend bool operator==(const 概念树支持退出请求 &, const 概念树支持退出请求 &) = default;
};
struct 概念树按概念支持读取请求 final {
    概念树读取头 读取头;
    概念树概念身份 目标概念;
    概念树预算 预算;
    friend bool operator==(const 概念树按概念支持读取请求 &, const 概念树按概念支持读取请求 &) = default;
};
struct 概念树按世界支持读取请求 final {
    概念树读取头 读取头;
    概念树世界引用 世界引用;
    概念树预算 预算;
    friend bool operator==(const 概念树按世界支持读取请求 &, const 概念树按世界支持读取请求 &) = default;
};
struct 概念树生命周期迁移请求 final {
    概念树写入头 写入头;
    概念树概念身份 概念;
    稳定编码 当前生命周期值编码{};
    概念树生命周期状态 预期状态 = 概念树生命周期状态::活跃;
    概念树生命周期状态 目标状态 = 概念树生命周期状态::活跃;
    概念树预算 预算;
    friend bool operator==(const 概念树生命周期迁移请求 &, const 概念树生命周期迁移请求 &) = default;
};
struct 概念树概念退出请求 final {
    概念树写入头 写入头;
    概念树概念身份 概念;
    稳定编码 当前退役生命周期值编码{};
    概念树预算 预算;
    friend bool operator==(const 概念树概念退出请求 &, const 概念树概念退出请求 &) = default;
};
struct 概念树形成引用读取请求 final {
    概念树读取头 读取头;
    概念树形成世界引用 世界引用;
    概念树预算 预算;
    friend bool operator==(const 概念树形成引用读取请求 &, const 概念树形成引用读取请求 &) = default;
};
struct 概念树形成引用释放请求 final {
    概念树写入头 写入头;
    概念树形成世界引用 世界引用;
    std::vector<概念树形成引用事实> 预期当前引用;
    概念树预算 预算;
    friend bool operator==(const 概念树形成引用释放请求 &, const 概念树形成引用释放请求 &) = default;
};
using 概念树业务写请求 =
    std::variant<概念树概念建立请求, 概念树上位操作请求, 概念树支持建立请求, 概念树支持退出请求,
                 概念树生命周期迁移请求, 概念树概念退出请求, 概念树形成引用释放请求>;

struct 概念树读取结果 final {
    std::uint32_t 合同版本 = 1;
    概念树数据状态 状态 = 概念树数据状态::入口拒绝;
    std::uint64_t Gread = 0;
    std::uint64_t H = 0;
    std::optional<概念树概念事实> 概念;
    bool 成功() const noexcept;
    friend bool operator==(const 概念树读取结果 &, const 概念树读取结果 &) = default;
};
struct 概念树概念组结果 final {
    std::uint32_t 合同版本 = 1;
    概念树数据状态 状态 = 概念树数据状态::入口拒绝;
    std::uint64_t Gread = 0;
    std::uint64_t H = 0;
    std::vector<概念树概念事实> 概念组;
    bool 成功() const noexcept;
    friend bool operator==(const 概念树概念组结果 &, const 概念树概念组结果 &) = default;
};
struct 概念树图结果 final {
    std::uint32_t 合同版本 = 1;
    概念树数据状态 状态 = 概念树数据状态::入口拒绝;
    std::uint64_t Gread = 0;
    std::uint64_t H = 0;
    概念树概念身份 根身份;
    std::vector<概念树概念事实> 概念组;
    std::vector<概念树直接上位事实> 直接边组;
    std::optional<std::vector<概念树支持事实>> 支持组;
    bool 成功() const noexcept;
    friend bool operator==(const 概念树图结果 &, const 概念树图结果 &) = default;
};
struct 概念树支持组结果 final {
    std::uint32_t 合同版本 = 1;
    概念树数据状态 状态 = 概念树数据状态::入口拒绝;
    std::uint64_t Gread = 0;
    std::uint64_t H = 0;
    std::vector<概念树支持事实> 支持组;
    bool 成功() const noexcept;
    friend bool operator==(const 概念树支持组结果 &, const 概念树支持组结果 &) = default;
};
struct 概念树形成引用组结果 final {
    std::uint32_t 合同版本 = 1;
    概念树数据状态 状态 = 概念树数据状态::入口拒绝;
    std::uint64_t Gread = 0;
    std::uint64_t H = 0;
    std::vector<概念树形成引用事实> 形成引用组;
    bool 成功() const noexcept;
    friend bool operator==(const 概念树形成引用组结果 &, const 概念树形成引用组结果 &) = default;
};
struct 概念树写入结果 final {
    std::uint32_t 合同版本 = 1;
    概念树数据状态 状态 = 概念树数据状态::入口拒绝;
    std::uint64_t 事实代次 = 0;
    概念树发布状态 发布状态 = 概念树发布状态::未派发;
    std::optional<std::uint64_t> 发布截止;
    std::optional<概念树业务写请求> 原请求;
    std::optional<概念树概念事实> 概念;
    std::optional<概念树支持事实> 支持;
    std::vector<概念树直接上位事实> 关系组;
    std::vector<稳定编码> 已退出事实;
    bool 成功() const noexcept;
    friend bool operator==(const 概念树写入结果 &, const 概念树写入结果 &) = default;
};

class 概念树类数据服务 final {
    using S = 概念树数据状态;
    using P = 概念树发布状态;
    using N = L1所有者范围节点事实;
    using E = L1所有者范围关系事实;
    using V = L1所有者范围值事实;
    using Ref = L1所有者范围事实引用;
    using Key = L1所有者范围写集本地键;
    struct 失败 {
        S 状态;
    };
    struct 概念展开 {
        概念树概念事实 事实;
        std::vector<稳定编码> 当前自有事实;
    };
    friend struct 概念树读取结果;
    friend struct 概念树概念组结果;
    friend struct 概念树图结果;
    friend struct 概念树支持组结果;
    friend struct 概念树形成引用组结果;
    friend struct 概念树写入结果;

  public:
    概念树类数据服务(const L1事实基座服务 &, const 特征类数据服务 &, const 存在类数据服务 &,
                     const 特征值类数据服务 &, L1所有者范围写端口 &&, const 概念树结构交付 &);
    概念树类数据服务() = delete;
    概念树类数据服务(const 概念树类数据服务 &) = delete;
    概念树类数据服务 &operator=(const 概念树类数据服务 &) = delete;
    概念树类数据服务(概念树类数据服务 &&) = delete;
    概念树类数据服务 &operator=(概念树类数据服务 &&) = delete;
    概念树读取结果 读取概念(const 概念树概念读取请求 &) const;
    概念树概念组结果 查找完整定义(const 概念树定义查找请求 &) const;
    概念树图结果 读取概念图(const 概念树图读取请求 &) const;
    概念树支持组结果 按概念读取支持(const 概念树按概念支持读取请求 &) const;
    概念树支持组结果 按世界读取支持(const 概念树按世界支持读取请求 &) const;
    概念树形成引用组结果 读取形成来源当前引用(const 概念树形成引用读取请求 &) const;
    概念树写入结果 创建概念(const 概念树概念建立请求 &r) {
        return 执行写入(r);
    }
    概念树写入结果 操作直接上位(const 概念树上位操作请求 &r) {
        return 执行写入(r);
    }
    概念树写入结果 建立世界支持(const 概念树支持建立请求 &r) {
        return 执行写入(r);
    }
    概念树写入结果 退出世界支持(const 概念树支持退出请求 &r) {
        return 执行写入(r);
    }
    概念树写入结果 迁移概念生命周期(const 概念树生命周期迁移请求 &r) {
        return 执行写入(r);
    }
    概念树写入结果 退出概念(const 概念树概念退出请求 &r) {
        return 执行写入(r);
    }
    概念树写入结果 释放形成来源当前引用(const 概念树形成引用释放请求 &r) {
        return 执行写入(r);
    }

  private:
    const L1事实基座服务 &l1_;
    const 特征类数据服务 &features_;
    const 存在类数据服务 &existences_;
    const 特征值类数据服务 &values_;
    L1所有者范围写端口 port_;
    概念树结构交付 layout_;
    mutable std::mutex mutex_;

    void 校验布局(std::uint64_t Gread) const;
    void 守卫代次(std::uint64_t Gread) const;
    概念树定义 规范化定义(const 概念树定义 &) const;
    概念树概念事实 读取概念内部(概念树概念身份, std::uint64_t Gread, std::uint64_t H,
                                const 概念树预算 &) const;
    L1所有者范围写集请求 形成写集(const 概念树业务写请求 &, std::uint64_t Gread, std::uint64_t H) const;
    bool 校验图闭包(const 概念树图结果 &) const;
    概念树写入结果 执行写入(const 概念树业务写请求 &);
    std::uint64_t 当前代次() const;
    N 节点(稳定编码, std::uint64_t g, std::uint64_t h) const;
    L1所有者范围事实副本 原始事实(稳定编码, std::uint64_t g) const;
    std::vector<E> 关系(稳定编码 端点, 稳定编码 类型, bool 入边, std::uint64_t g, std::uint64_t h,
                        std::uint64_t 预算) const;
    std::vector<V> 属性(稳定编码, std::uint64_t g, std::uint64_t h) const;
    E 唯一关系(稳定编码, 稳定编码, std::uint64_t g, std::uint64_t h) const;
    static const V &唯一属性(const std::vector<V> &, 稳定编码);
    std::array<稳定编码, 28> 类型组() const;
    std::array<概念树概念身份, 4> 根组() const;
    概念树概念身份 根(概念树根角色) const;
    概念展开 展开概念(概念树概念身份, std::uint64_t g, std::uint64_t h, const 概念树预算 &) const;
    概念树图结果 图(概念树根角色, bool 支持, std::uint64_t g, std::uint64_t h, const 概念树预算 &) const;
    概念树支持事实 支持记录(稳定编码, std::uint64_t g, std::uint64_t h, const 概念树预算 &) const;
    std::vector<概念树支持事实> 支持组(std::optional<概念树概念身份>, std::optional<概念树世界引用>,
                                       std::uint64_t g, std::uint64_t h, const 概念树预算 &) const;
    std::vector<概念树形成引用事实> 形成引用组(const 概念树形成世界引用 &, std::uint64_t g, std::uint64_t h,
                                               const 概念树预算 &) const;
    void 验证世界(const 概念树形成世界引用 &, std::uint64_t g, std::uint64_t h, const 概念树预算 &) const;
    void 验证特征定义(const 概念树特征定义 &, std::uint64_t g, std::uint64_t h, const 概念树预算 &) const;
    概念树业务写请求 规范写请求(const 概念树业务写请求 &) const;
    概念树写入结果 发布结果(const 概念树业务写请求 &, const L1所有者范围写集请求 &,
                            const L1所有者范围写入结果 &, std::uint64_t g) const;
    static void 预算有效(const 概念树预算 &);
    static std::uint64_t 读取截止(const 概念树读取头 &);
    static void 检查数量(std::size_t n, std::uint64_t max);
    static S 映射(L1所有者范围读取状态);
    static S 映射(特征引用读取状态);
    static bool 编码小于(稳定编码 a, 稳定编码 b) {
        return a.值 < b.值;
    }
    static bool 身份小于(概念树概念身份 a, 概念树概念身份 b) {
        return 编码小于(a.值, b.值);
    }
    template <class W> static 稳定编码 世界编码(const W &w) {
        return std::visit([](const auto &v) { return v.值; }, w);
    }
    static 概念树形成世界引用 扩展世界(const 概念树世界引用 &w) {
        return std::visit([](const auto &x) -> 概念树形成世界引用 { return x; }, w);
    }
    template <class T> static 概念树生命周期 投影生命(const T &v, std::uint64_t h) {
        return {v.创建事实代次, v.退出事实代次 && *v.退出事实代次 <= h ? v.退出事实代次 : std::nullopt};
    }
    template <class T> static bool 有效于(const T &v, std::uint64_t h) {
        return v.创建事实代次 && v.创建事实代次 <= h && (!v.退出事实代次 || h < *v.退出事实代次);
    }
    static bool 生命完整(const 概念树生命周期 &, std::uint64_t h, bool 可退出 = false) noexcept;
    static bool 概念完整(const 概念树概念事实 &, std::uint64_t h, bool 可退出 = false) noexcept;
    static bool 支持完整(const 概念树支持事实 &, std::uint64_t h, bool 可退出 = false) noexcept;
    template <class R, class F> R 读取封装(const 概念树读取头 &头, const 概念树预算 &预算, F &&f) const {
        R out;
        out.Gread = 头.Gread;
        out.H = 头.H ? 头.H : 头.Gread;
        try {
            const auto h = 读取截止(头);
            预算有效(预算);
            守卫代次(头.Gread);
            out = f(头.Gread, h);
            out.状态 = S::已读取;
            out.Gread = 头.Gread;
            out.H = h;
            守卫代次(头.Gread);
            if (!out.成功())
                throw 失败{S::内部不一致};
            return out;
        } catch (const 失败 &e) {
            out = R{};
            out.状态 = e.状态;
        } catch (const std::bad_alloc &) {
            out = R{};
            out.状态 = S::资源失败;
        } catch (const std::length_error &) {
            out = R{};
            out.状态 = S::资源失败;
        } catch (...) {
            out = R{};
            out.状态 = S::内部不一致;
        }
        out.Gread = 头.Gread;
        out.H = 头.H ? 头.H : 头.Gread;
        return out;
    }
};

// 诊断责任：低层状态保留错误种类并向公开边界送出。
概念树数据状态 概念树类数据服务::映射(L1所有者范围读取状态 s) {
    switch (s) {
    case L1所有者范围读取状态::入口拒绝:
        return S::入口拒绝;
    case L1所有者范围读取状态::未找到:
        return S::未找到;
    case L1所有者范围读取状态::已退出:
        return S::目标已退出;
    case L1所有者范围读取状态::事实代次漂移:
        return S::事实代次漂移;
    case L1所有者范围读取状态::资源失败:
        return S::资源失败;
    case L1所有者范围读取状态::历史材料已清理:
        return S::历史材料不可用;
    default:
        return S::内部不一致;
    }
}
概念树数据状态 概念树类数据服务::映射(特征引用读取状态 s) {
    switch (s) {
    case 特征引用读取状态::入口拒绝:
        return S::入口拒绝;
    case 特征引用读取状态::未找到:
        return S::未找到;
    case 特征引用读取状态::目标已退出:
        return S::目标已退出;
    case 特征引用读取状态::事实代次漂移:
        return S::事实代次漂移;
    case 特征引用读取状态::资源失败:
        return S::资源失败;
    case 特征引用读取状态::历史材料不可用:
        return S::历史材料不可用;
    case 特征引用读取状态::数量预算不足:
        return S::数量预算不足;
    case 特征引用读取状态::引用冲突:
        return S::引用冲突;
    default:
        return S::内部不一致;
    }
}
void 概念树类数据服务::预算有效(const 概念树预算 &b) {
    if (!b.最大概念数 || !b.最大关系数 || !b.最大来源数 || !b.最大支持数 || !b.最大世界成员数 ||
        !b.最大特征属性数)
        throw 失败{S::入口拒绝};
}
void 概念树类数据服务::检查数量(std::size_t n, std::uint64_t max) {
    if (n > max)
        throw 失败{S::数量预算不足};
}
std::uint64_t 概念树类数据服务::读取截止(const 概念树读取头 &r) {
    if (r.合同版本 != 1 || !r.Gread || r.H > r.Gread)
        throw 失败{S::入口拒绝};
    return r.H ? r.H : r.Gread;
}
std::uint64_t 概念树类数据服务::当前代次() const {
    const auto r = l1_.读取中性当前事实代次({L1中性CRUD合同版本});
    if (r.状态 != L1中性读取状态::成功)
        throw 失败{r.状态 == L1中性读取状态::资源失败 ? S::资源失败 : S::内部不一致};
    if (r.合同版本 != L1中性CRUD合同版本 || !r.事实代次)
        throw 失败{S::内部不一致};
    return r.事实代次;
}
void 概念树类数据服务::守卫代次(std::uint64_t g) const {
    if (当前代次() != g)
        throw 失败{S::事实代次漂移};
}
L1所有者范围事实副本 概念树类数据服务::原始事实(稳定编码 id, std::uint64_t g) const {
    if (!有效(id))
        throw 失败{S::入口拒绝};
    const auto r = l1_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, id});
    if (r.状态 != L1所有者范围读取状态::成功)
        throw 失败{映射(r.状态)};
    if (r.合同版本 != L1所有者范围CRUD合同版本 || r.查询编码 != id || !r.事实 || r.物理清理墓碑 ||
        r.物理清理事实代次)
        throw 失败{S::内部不一致};
    if (r.读取事实代次 != g)
        throw 失败{S::事实代次漂移};
    std::visit(
        [&](const auto &v) {
            if (v.编码 != id || v.写入所有者 != port_.所有者身份())
                throw 失败{S::引用冲突};
            if (!v.创建事实代次 || v.创建事实代次 > g ||
                (v.退出事实代次 && (*v.退出事实代次 < v.创建事实代次 || *v.退出事实代次 > g)))
                throw 失败{S::内部不一致};
        },
        *r.事实);
    return *r.事实;
}
概念树类数据服务::N 概念树类数据服务::节点(稳定编码 id, std::uint64_t g, std::uint64_t h) const {
    auto f = 原始事实(id, g);
    auto *n = std::get_if<N>(&f);
    if (!n)
        throw 失败{S::引用冲突};
    if (n->创建事实代次 > h)
        throw 失败{S::未找到};
    if (!有效于(*n, h))
        throw 失败{S::目标已退出};
    return *n;
}
std::vector<概念树类数据服务::E> 概念树类数据服务::关系(稳定编码 id, 稳定编码 type, bool incoming,
                                                        std::uint64_t g, std::uint64_t h,
                                                        std::uint64_t budget) const {
    const auto direction = incoming ? L1所有者范围关系端点方向::目标 : L1所有者范围关系端点方向::源;
    const auto r = l1_.读取所有者范围历史关系组({L1所有者范围CRUD合同版本, direction, id, type, h});
    if (r.状态 != L1所有者范围读取状态::成功)
        throw 失败{映射(r.状态)};
    if (r.读取事实代次 != g)
        throw 失败{S::事实代次漂移};
    if (r.合同版本 != L1所有者范围CRUD合同版本 || r.方向 != direction || r.端点节点 != id ||
        r.关系类型节点 != type || r.历史截止事实代次 != h)
        throw 失败{S::内部不一致};
    检查数量(r.关系组.size(), budget);
    auto out = r.关系组;
    std::set<std::uint64_t> ids;
    for (const auto &e : out) {
        if (!有效(e.编码) || !有效(e.源节点) || !有效(e.目标节点) || e.关系类型节点 != type ||
            (incoming ? e.目标节点 : e.源节点) != id || e.写入所有者 != port_.所有者身份() || !有效于(e, h) ||
            !ids.insert(e.编码.值).second)
            throw 失败{S::内部不一致};
    }
    std::sort(out.begin(), out.end(), [](const auto &a, const auto &b) {
        return a.角色或顺序 != b.角色或顺序 ? a.角色或顺序 < b.角色或顺序 : a.编码.值 < b.编码.值;
    });
    return out;
}
std::vector<概念树类数据服务::V> 概念树类数据服务::属性(稳定编码 id, std::uint64_t g, std::uint64_t h) const {
    const auto r = l1_.读取所有者范围历史属性值组({L1所有者范围CRUD合同版本, id, h});
    if (r.状态 != L1所有者范围读取状态::成功)
        throw 失败{映射(r.状态)};
    if (r.读取事实代次 != g)
        throw 失败{S::事实代次漂移};
    if (r.合同版本 != L1所有者范围CRUD合同版本 || r.所属节点 != id || r.历史截止事实代次 != h)
        throw 失败{S::内部不一致};
    std::set<std::uint64_t> slots;
    for (const auto &v : r.属性值组)
        if (!有效(v.编码) || v.所属节点 != id || v.来源节点 != id || v.写入所有者 != port_.所有者身份() ||
            !有效于(v, h) || !slots.insert(v.属性类型节点.值).second)
            throw 失败{S::内部不一致};
    return r.属性值组;
}
概念树类数据服务::E 概念树类数据服务::唯一关系(稳定编码 id, 稳定编码 type, std::uint64_t g,
                                               std::uint64_t h) const {
    const auto r = 关系(id, type, false, g, h, 1);
    if (r.size() != 1 || r.front().角色或顺序 != 1)
        throw 失败{S::内部不一致};
    return r.front();
}
const 概念树类数据服务::V &概念树类数据服务::唯一属性(const std::vector<V> &values, 稳定编码 type) {
    const V *found = nullptr;
    for (const auto &v : values)
        if (v.属性类型节点 == type) {
            if (found)
                throw 失败{S::内部不一致};
            found = &v;
        }
    if (!found)
        throw 失败{S::内部不一致};
    return *found;
}
std::array<稳定编码, 28> 概念树类数据服务::类型组() const {
    const auto &t = layout_.类型;
    return {t.根成员,   t.规则归属,     t.概念根归属, t.概念规则,  t.定义成员, t.来源成员, t.直接上位,
            t.定义宿主, t.定义特征类型, t.定义模板,   t.来源存在,  t.来源特征, t.支持归属, t.支持概念,
            t.支持存在, t.支持特征,     t.格式版本,   t.规则版本,  t.根角色,   t.概念种类, t.生命周期,
            t.证据截止, t.精确I64,      t.精确I64组,  t.精确U64组, t.区间下界, t.区间上界, t.类型登记};
}
std::array<概念树概念身份, 4> 概念树类数据服务::根组() const {
    const auto &r = layout_.根组;
    return {r.存在根, r.特征根, r.动态根, r.因果链根};
}
概念树概念身份 概念树类数据服务::根(概念树根角色 role) const {
    const auto i = static_cast<unsigned>(role);
    if (i < 1 || i > 4)
        throw 失败{S::入口拒绝};
    return 根组()[i - 1];
}

// 构造只消费正式交付，不建立根、规则、类型或 owner。
概念树类数据服务::概念树类数据服务(const L1事实基座服务 &l1, const 特征类数据服务 &features,
                                   const 存在类数据服务 &existences, const 特征值类数据服务 &values,
                                   L1所有者范围写端口 &&port, const 概念树结构交付 &layout)
    : l1_(l1), features_(features), existences_(existences), values_(values), port_(std::move(port)),
      layout_(layout) {
    if (!port_.有效() || !port_.绑定于(l1_) || !features_.绑定于(l1_) || !existences_.绑定于(l1_) ||
        !values_.绑定于(l1_))
        throw std::invalid_argument("concept tree provider binding mismatch");
    try {
        校验布局(当前代次());
    } catch (const 失败 &e) {
        if (e.状态 == S::资源失败)
            throw std::bad_alloc{};
        throw std::invalid_argument("concept tree structure delivery is invalid");
    }
}
void 概念树类数据服务::校验布局(std::uint64_t g) const {
    const auto owner = l1_.读取当前结构所有者({L1所有者范围CRUD合同版本, port_.所有者身份()});
    if (owner.状态 != L1所有者范围读取状态::成功)
        throw 失败{映射(owner.状态)};
    if (owner.读取事实代次 != g)
        throw 失败{S::事实代次漂移};
    if (!owner.所有者事实 || owner.所有者事实->所有者 != port_.所有者身份() ||
        owner.所有者事实->退出事实代次 || owner.所有者事实->范围种类 != L1所有者范围种类::独占结构范围)
        throw 失败{S::引用冲突};
    const auto types = 类型组();
    const auto roots = 根组();
    std::set<std::uint64_t> ids;
    const auto unique = [&](稳定编码 id) {
        if (!有效(id) || !ids.insert(id.值).second)
            throw 失败{S::引用冲突};
    };
    unique(layout_.格式锚点);
    unique(layout_.签名规则.值);
    for (const auto r : roots)
        unique(r.值);
    for (std::size_t i = 0; i < types.size(); ++i) {
        unique(types[i]);
        const auto n = 节点(types[i], g, g);
        const bool attribute = i >= 16 && i < 27;
        using R = L1所有者范围值表示种类;
        const auto rep = i == 21 || i == 24 ? R::U64组 : i == 23 ? R::I64组 : R::I64;
        if (n.种类 != (attribute ? 节点种类::属性类型 : 节点种类::普通) ||
            n.属性类型表示 != (attribute ? std::optional<R>{rep} : std::nullopt) ||
            !属性(types[i], g, g).empty())
            throw 失败{S::引用冲突};
    }
    const auto plain = [&](稳定编码 id) {
        const auto n = 节点(id, g, g);
        if (n.种类 != 节点种类::普通 || n.属性类型表示)
            throw 失败{S::引用冲突};
    };
    plain(layout_.格式锚点);
    plain(layout_.签名规则.值);
    const auto &t = layout_.类型;
    const auto registrations = 关系(layout_.格式锚点, t.类型登记, false, g, g, 28);
    if (registrations.size() != 28)
        throw 失败{S::引用冲突};
    for (std::size_t i = 0; i < 28; ++i)
        if (registrations[i].目标节点 != types[i] || registrations[i].角色或顺序 != i + 1)
            throw 失败{S::引用冲突};
    const auto rootEdges = 关系(layout_.格式锚点, t.根成员, false, g, g, 4);
    if (rootEdges.size() != 4)
        throw 失败{S::引用冲突};
    for (std::size_t i = 0; i < 4; ++i) {
        plain(roots[i].值);
        const auto a = 属性(roots[i].值, g, g);
        if (rootEdges[i].目标节点 != roots[i].值 || rootEdges[i].角色或顺序 != i + 1 || a.size() != 1 ||
            std::get<std::int64_t>(唯一属性(a, t.根角色).材料) != i + 1)
            throw 失败{S::引用冲突};
    }
    if (唯一关系(layout_.格式锚点, t.规则归属, g, g).目标节点 != layout_.签名规则.值)
        throw 失败{S::引用冲突};
    const auto anchorValues = 属性(layout_.格式锚点, g, g);
    const auto ruleValues = 属性(layout_.签名规则.值, g, g);
    if (anchorValues.size() != 1 || ruleValues.size() != 2 ||
        std::get<std::int64_t>(唯一属性(anchorValues, t.格式版本).材料) != 1 ||
        std::get<std::int64_t>(唯一属性(ruleValues, t.格式版本).材料) != 1 ||
        std::get<std::int64_t>(唯一属性(ruleValues, t.规则版本).材料) != 1)
        throw 失败{S::引用冲突};
    守卫代次(g);
}

概念树定义 概念树类数据服务::规范化定义(const 概念树定义 &input) const {
    auto out = input;
    std::visit(
        [&](auto &d) {
            using T = std::decay_t<decltype(d)>;
            if constexpr (std::is_same_v<T, 概念树特征定义>) {
                if (!有效(d.形成宿主.值) || !有效(d.特征类型.值))
                    throw 失败{S::入口拒绝};
                if (auto *interval = std::get_if<概念树I64区间>(&d.值域)) {
                    if (interval->下界 > interval->上界)
                        throw 失败{S::入口拒绝};
                    if (interval->下界 == interval->上界)
                        d.值域 = 概念树精确值{interval->下界};
                }
                if (auto *exact = std::get_if<概念树精确值>(&d.值域))
                    std::visit(
                        [](const auto &v) {
                            if constexpr (!std::is_same_v<std::decay_t<decltype(v)>, std::int64_t>)
                                if (v.empty())
                                    throw 失败{S::入口拒绝};
                        },
                        *exact);
            } else {
                if (d.特征模板组.empty())
                    throw 失败{S::入口拒绝};
                std::sort(d.特征模板组.begin(), d.特征模板组.end(), 身份小于);
                for (std::size_t i = 0; i < d.特征模板组.size(); ++i)
                    if (!有效(d.特征模板组[i].值) || (i && d.特征模板组[i] == d.特征模板组[i - 1]))
                        throw 失败{S::入口拒绝};
            }
        },
        out);
    return out;
}
void 概念树类数据服务::验证世界(const 概念树形成世界引用 &w, std::uint64_t g, std::uint64_t h,
                                const 概念树预算 &b) const {
    if (!h || h > g || !有效(世界编码(w)))
        throw 失败{S::入口拒绝};
    std::visit(
        [&](const auto &x) {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, 概念树存在引用>) {
                const auto r = existences_.读取存在历史事实({1, g, h, x.值, b.最大世界成员数});
                if (!r.成功())
                    throw 失败{映射(r.状态)};
                if (r.Gread != g || r.H != h || r.存在->结点 != x.值)
                    throw 失败{S::内部不一致};
            } else if constexpr (std::is_same_v<T, 概念树特征引用>) {
                const auto r = features_.按实例读取特征历史事实({1, g, h, x.值, b.最大特征属性数});
                if (!r.成功())
                    throw 失败{映射(r.状态)};
                if (r.Gread != g || r.H != h || r.特征->结点 != x.值)
                    throw 失败{S::内部不一致};
            } else {
                const auto r = features_.读取特征类型历史事实({1, g, h, x.值});
                if (!r.成功())
                    throw 失败{映射(r.状态)};
                if (r.Gread != g || r.H != h || r.特征类型 != x.值)
                    throw 失败{S::内部不一致};
            }
        },
        w);
}
void 概念树类数据服务::验证特征定义(const 概念树特征定义 &d, std::uint64_t g, std::uint64_t h,
                                    const 概念树预算 &b) const {
    验证世界(d.形成宿主, g, h, b);
    const auto r = h == g ? features_.确认特征类型({1, g, d.特征类型.值})
                          : features_.读取特征类型历史事实({1, g, h, d.特征类型.值});
    if (!r.成功())
        throw 失败{映射(r.状态)};
    if (r.Gread != g || r.H != h || r.特征类型 != d.特征类型.值)
        throw 失败{S::内部不一致};
    using R = L1所有者范围值表示种类;
    R required = R::I64;
    if (const auto *exact = std::get_if<概念树精确值>(&d.值域))
        required = exact->index() == 1 ? R::I64组 : exact->index() == 2 ? R::U64组 : R::I64;
    if (r.表示 != required)
        throw 失败{S::引用冲突};
}

// 当前结构与不可变记录的形成关系分别按结构 H 与记录创建 H 投影。
概念树类数据服务::概念展开 概念树类数据服务::展开概念(概念树概念身份 id, std::uint64_t g, std::uint64_t h,
                                                      const 概念树预算 &b) const {
    概念展开 out;
    auto &f = out.事实;
    const auto n = 节点(id.值, g, h);
    if (n.种类 != 节点种类::普通 || n.属性类型表示)
        throw 失败{S::引用冲突};
    f.概念 = id;
    f.生命周期 = 投影生命(n, h);
    f.规则 = layout_.签名规则;
    const auto roots = 根组();
    const auto rootIt = std::find(roots.begin(), roots.end(), id);
    const auto &t = layout_.类型;
    if (rootIt != roots.end()) {
        const auto role = static_cast<概念树根角色>(rootIt - roots.begin() + 1);
        const auto a = 属性(id.值, g, h);
        if (a.size() != 1 || std::get<std::int64_t>(唯一属性(a, t.根角色).材料) != static_cast<int>(role))
            throw 失败{S::内部不一致};
        f.是本体根 = true;
        f.本体根 = id;
        f.根角色 = role;
        return out;
    }
    out.当前自有事实.push_back(id.值);
    const auto a = 属性(id.值, g, h);
    if (a.size() != 2)
        throw 失败{S::内部不一致};
    const auto &kindValue = 唯一属性(a, t.概念种类);
    const auto &lifeValue = 唯一属性(a, t.生命周期);
    const auto kind = std::get<std::int64_t>(kindValue.材料);
    const auto state = std::get<std::int64_t>(lifeValue.材料);
    if ((kind != 1 && kind != 2) || state < 1 || state > 3 || kindValue.创建事实代次 != n.创建事实代次)
        throw 失败{S::内部不一致};
    f.根角色 = kind == 1 ? 概念树根角色::特征 : 概念树根角色::存在;
    f.本体根 = 根(f.根角色);
    f.治理状态 = static_cast<概念树生命周期状态>(state);
    f.生命周期值事实 = lifeValue.编码;
    for (const auto &v : a)
        out.当前自有事实.push_back(v.编码);
    const auto rootEdge = 唯一关系(id.值, t.概念根归属, g, h);
    const auto ruleEdge = 唯一关系(id.值, t.概念规则, g, h);
    const auto definitionEdge = 唯一关系(id.值, t.定义成员, g, h);
    if (rootEdge.目标节点 != f.本体根.值 || ruleEdge.目标节点 != layout_.签名规则.值)
        throw 失败{S::引用冲突};
    for (const auto &e : {rootEdge, ruleEdge, definitionEdge}) {
        if (e.创建事实代次 != n.创建事实代次)
            throw 失败{S::内部不一致};
        out.当前自有事实.push_back(e.编码);
    }
    const auto record = 节点(definitionEdge.目标节点, g, h);
    if (record.种类 != 节点种类::普通 || record.属性类型表示 || record.创建事实代次 != n.创建事实代次)
        throw 失败{S::内部不一致};
    f.定义记录 = record.编码;
    out.当前自有事实.push_back(record.编码);
    const auto definitionValues = 属性(record.编码, g, h);
    for (const auto &v : definitionValues) {
        if (v.创建事实代次 != n.创建事实代次)
            throw 失败{S::内部不一致};
        out.当前自有事实.push_back(v.编码);
    }
    const auto formedEdge = [&](稳定编码 recordId, 稳定编码 type, std::uint64_t creation) {
        const auto e = 唯一关系(recordId, type, g, creation);
        if (e.创建事实代次 != creation)
            throw 失败{S::内部不一致};
        const auto current = 关系(recordId, type, false, g, h, 1);
        if (!current.empty()) {
            if (current.front().编码 != e.编码)
                throw 失败{S::内部不一致};
            out.当前自有事实.push_back(e.编码);
        } else if (!e.退出事实代次 || *e.退出事实代次 > h) {
            throw 失败{S::内部不一致};
        }
        return e;
    };
    if (kind == 1) {
        概念树特征定义 d;
        d.形成宿主 = 概念树存在引用{formedEdge(record.编码, t.定义宿主, n.创建事实代次).目标节点};
        d.特征类型 = 概念树特征类型引用{formedEdge(record.编码, t.定义特征类型, n.创建事实代次).目标节点};
        if (!关系(record.编码, t.定义模板, false, g, h, b.最大关系数).empty())
            throw 失败{S::内部不一致};
        if (definitionValues.size() == 2) {
            d.值域 = 概念树I64区间{std::get<std::int64_t>(唯一属性(definitionValues, t.区间下界).材料),
                                   std::get<std::int64_t>(唯一属性(definitionValues, t.区间上界).材料)};
        } else if (definitionValues.size() == 1) {
            const auto &v = definitionValues.front();
            if (v.属性类型节点 == t.精确I64)
                d.值域 = 概念树精确值{std::get<std::int64_t>(v.材料)};
            else if (v.属性类型节点 == t.精确I64组)
                d.值域 = 概念树精确值{std::get<std::vector<std::int64_t>>(v.材料)};
            else if (v.属性类型节点 == t.精确U64组)
                d.值域 = 概念树精确值{std::get<std::vector<std::uint64_t>>(v.材料)};
            else
                throw 失败{S::内部不一致};
        } else
            throw 失败{S::内部不一致};
        if (规范化定义(d) != 概念树定义{d})
            throw 失败{S::内部不一致};
        验证特征定义(d, g, n.创建事实代次, b);
        f.定义 = d;
    } else {
        if (!definitionValues.empty() ||
            !关系(record.编码, t.定义宿主, false, g, n.创建事实代次, 1).empty() ||
            !关系(record.编码, t.定义特征类型, false, g, n.创建事实代次, 1).empty())
            throw 失败{S::内部不一致};
        概念树存在定义 d;
        const auto templates = 关系(record.编码, t.定义模板, false, g, h, b.最大关系数);
        if (templates != 关系(record.编码, t.定义模板, false, g, n.创建事实代次, b.最大关系数))
            throw 失败{S::内部不一致};
        for (const auto &e : templates) {
            if (e.角色或顺序 != d.特征模板组.size() + 1 || e.创建事实代次 != n.创建事实代次)
                throw 失败{S::内部不一致};
            const auto target = 概念树概念身份{e.目标节点};
            // 先检查种类，禁止坏模板链递归成为环。
            const auto targetNode = 节点(target.值, g, h);
            const auto targetValues = 属性(target.值, g, h);
            if (targetNode.种类 != 节点种类::普通 || targetValues.size() != 2 ||
                std::get<std::int64_t>(唯一属性(targetValues, t.概念种类).材料) != 1)
                throw 失败{S::引用冲突};
            const auto targetFact = 读取概念内部(target, g, h, b);
            if (targetFact.是本体根 || targetFact.根角色 != 概念树根角色::特征)
                throw 失败{S::引用冲突};
            d.特征模板组.push_back(target);
            out.当前自有事实.push_back(e.编码);
        }
        if (规范化定义(d) != 概念树定义{d})
            throw 失败{S::内部不一致};
        f.定义 = d;
    }
    const auto sources = 关系(id.值, t.来源成员, false, g, h, b.最大来源数);
    if (sources != 关系(id.值, t.来源成员, false, g, n.创建事实代次, b.最大来源数))
        throw 失败{S::内部不一致};
    if (sources.empty())
        throw 失败{S::内部不一致};
    for (const auto &member : sources) {
        if (member.角色或顺序 != f.来源组.size() + 1 || member.创建事实代次 != n.创建事实代次)
            throw 失败{S::内部不一致};
        const auto sourceNode = 节点(member.目标节点, g, h);
        if (sourceNode.创建事实代次 != n.创建事实代次 || sourceNode.种类 != 节点种类::普通 ||
            sourceNode.属性类型表示)
            throw 失败{S::内部不一致};
        const auto sourceValues = 属性(sourceNode.编码, g, h);
        if (sourceValues.size() != 1)
            throw 失败{S::内部不一致};
        const auto &time = 唯一属性(sourceValues, t.证据截止);
        const auto &times = std::get<std::vector<std::uint64_t>>(time.材料);
        if (times.size() != 1 || !times[0] || times[0] >= n.创建事实代次 ||
            time.创建事实代次 != n.创建事实代次)
            throw 失败{S::内部不一致};
        const auto oldExists = 关系(sourceNode.编码, t.来源存在, false, g, n.创建事实代次, 1);
        const auto oldFeatures = 关系(sourceNode.编码, t.来源特征, false, g, n.创建事实代次, 1);
        if (oldExists.size() + oldFeatures.size() != 1)
            throw 失败{S::内部不一致};
        const auto e =
            formedEdge(sourceNode.编码, oldExists.empty() ? t.来源特征 : t.来源存在, n.创建事实代次);
        概念树来源事实 s;
        s.记录 = sourceNode.编码;
        s.成员关系 = member.编码;
        s.世界关系 = e.编码;
        s.截止值事实 = time.编码;
        s.来源.世界事实 = oldExists.empty() ? 概念树世界引用{概念树特征引用{e.目标节点}}
                                            : 概念树世界引用{概念树存在引用{e.目标节点}};
        s.来源.证据截止 = times[0];
        s.记录生命周期 = 投影生命(sourceNode, h);
        s.当前世界引用已释放 = !有效于(e, h);
        验证世界(扩展世界(s.来源.世界事实), g, times[0], b);
        f.来源组.push_back(s);
        for (auto code : {sourceNode.编码, member.编码, time.编码})
            out.当前自有事实.push_back(code);
    }
    const auto parents = 关系(id.值, t.直接上位, true, g, h, b.最大关系数);
    if (parents.empty())
        throw 失败{S::引用冲突};
    std::set<std::uint64_t> parentIds;
    for (const auto &e : parents) {
        const auto parent = 概念树概念身份{e.源节点};
        if (e.角色或顺序 != 1 || parent == id || !parentIds.insert(e.源节点.值).second)
            throw 失败{S::引用冲突};
        (void)节点(parent.值, g, h);
        if (parent != f.本体根 && 唯一关系(parent.值, t.概念根归属, g, h).目标节点 != f.本体根.值)
            throw 失败{S::引用冲突};
        f.直接上位组.push_back({e.编码, parent, id, 投影生命(e, h)});
    }
    std::sort(f.直接上位组.begin(), f.直接上位组.end(),
              [](const auto &a, const auto &b) { return a.上位.值.值 < b.上位.值.值; });

    return out;
}
概念树概念事实 概念树类数据服务::读取概念内部(概念树概念身份 id, std::uint64_t g, std::uint64_t h,
                                              const 概念树预算 &b) const {
    return 展开概念(id, g, h, b).事实;
}
概念树支持事实 概念树类数据服务::支持记录(稳定编码 id, std::uint64_t g, std::uint64_t h,
                                          const 概念树预算 &b) const {
    const auto n = 节点(id, g, h);
    if (n.种类 != 节点种类::普通 || n.属性类型表示)
        throw 失败{S::引用冲突};
    const auto &t = layout_.类型;
    const auto anchor = 关系(id, t.支持归属, true, g, h, 1);
    if (anchor.size() != 1 || anchor.front().源节点 != layout_.格式锚点 || anchor.front().角色或顺序 != 1)
        throw 失败{S::内部不一致};
    const auto c = 唯一关系(id, t.支持概念, g, h);
    const auto exists = 关系(id, t.支持存在, false, g, h, 1);
    const auto features = 关系(id, t.支持特征, false, g, h, 1);
    if (exists.size() + features.size() != 1)
        throw 失败{S::内部不一致};
    const auto &e = exists.empty() ? features.front() : exists.front();
    const auto values = 属性(id, g, h);
    if (values.size() != 1)
        throw 失败{S::内部不一致};
    const auto &v = 唯一属性(values, t.证据截止);
    const auto &times = std::get<std::vector<std::uint64_t>>(v.材料);
    if (times.size() != 1 || !times[0] || times[0] >= n.创建事实代次 || v.创建事实代次 != n.创建事实代次)
        throw 失败{S::内部不一致};
    for (const auto &edge : {anchor.front(), c, e})
        if (edge.创建事实代次 != n.创建事实代次 || edge.角色或顺序 != 1)
            throw 失败{S::内部不一致};
    概念树支持事实 s{id,
                     anchor.front().编码,
                     c.编码,
                     e.编码,
                     v.编码,
                     概念树概念身份{c.目标节点},
                     exists.empty() ? 概念树世界引用{概念树特征引用{e.目标节点}}
                                    : 概念树世界引用{概念树存在引用{e.目标节点}},
                     times[0],
                     投影生命(n, h)};
    (void)读取概念内部(s.目标概念, g, h, b);
    验证世界(扩展世界(s.世界引用), g, s.证据H, b);
    return s;
}
std::vector<概念树支持事实> 概念树类数据服务::支持组(std::optional<概念树概念身份> conceptId,
                                                     std::optional<概念树世界引用> world, std::uint64_t g,
                                                     std::uint64_t h, const 概念树预算 &b) const {
    if (conceptId)
        (void)读取概念内部(*conceptId, g, h, b);
    if (world)
        验证世界(扩展世界(*world), g, h, b);
    const auto &t = layout_.类型;
    std::vector<E> members;
    if (conceptId)
        members = 关系(conceptId->值, t.支持概念, true, g, h, b.最大支持数);
    else if (world)
        members =
            关系(世界编码(*world), world->index() == 0 ? t.支持存在 : t.支持特征, true, g, h, b.最大支持数);
    else
        members = 关系(layout_.格式锚点, t.支持归属, false, g, h, b.最大支持数);
    std::vector<概念树支持事实> out;
    std::set<std::pair<std::uint64_t, std::uint64_t>> endpoints;
    for (const auto &member : members) {
        const auto id = conceptId || world ? member.源节点 : member.目标节点;
        auto s = 支持记录(id, g, h, b);
        if ((conceptId && s.目标概念 != *conceptId) || (world && s.世界引用 != *world) ||
            !endpoints.insert({世界编码(s.世界引用).值, s.目标概念.值.值}).second)
            throw 失败{S::内部不一致};
        out.push_back(std::move(s));
    }
    std::sort(out.begin(), out.end(), [](const auto &a, const auto &b) {
        if (a.世界引用.index() != b.世界引用.index())
            return a.世界引用.index() < b.世界引用.index();
        if (世界编码(a.世界引用) != 世界编码(b.世界引用))
            return 编码小于(世界编码(a.世界引用), 世界编码(b.世界引用));
        if (a.目标概念 != b.目标概念)
            return 身份小于(a.目标概念, b.目标概念);
        return 编码小于(a.记录, b.记录);
    });
    return out;
}

// 图必须同时满足类型闭包、无环和全部节点从唯一根可达。
bool 概念树类数据服务::校验图闭包(const 概念树图结果 &graph) const {
    std::map<std::uint64_t, std::size_t> degrees;
    std::map<std::uint64_t, std::vector<std::uint64_t>> children;
    std::set<std::pair<std::uint64_t, std::uint64_t>> pairs;
    for (const auto &f : graph.概念组) {
        if (!degrees.emplace(f.概念.值.值, 0).second || f.本体根 != graph.根身份)
            return false;
    }
    if (!degrees.contains(graph.根身份.值.值))
        return false;
    for (const auto &e : graph.直接边组) {
        const auto p = e.上位.值.值, c = e.下位.值.值;
        if (p == c || !degrees.contains(p) || !degrees.contains(c) || !pairs.insert({p, c}).second)
            return false;
        ++degrees[c];
        children[p].push_back(c);
    }
    if (degrees[graph.根身份.值.值])
        return false;
    for (const auto &[id, count] : degrees)
        if (id != graph.根身份.值.值 && !count)
            return false;
    std::vector<std::uint64_t> ready{graph.根身份.值.值};
    for (std::size_t i = 0; i < ready.size(); ++i)
        for (auto c : children[ready[i]])
            if (--degrees[c] == 0)
                ready.push_back(c);
    return ready.size() == degrees.size();
}
概念树图结果 概念树类数据服务::图(概念树根角色 role, bool withSupport, std::uint64_t g, std::uint64_t h,
                                  const 概念树预算 &b) const {
    概念树图结果 out;
    out.状态 = S::已读取;
    out.Gread = g;
    out.H = h;
    out.根身份 = 根(role);
    out.概念组.push_back(读取概念内部(out.根身份, g, h, b));
    const auto members = 关系(out.根身份.值, layout_.类型.概念根归属, true, g, h, b.最大概念数);
    检查数量(members.size() + 1, b.最大概念数);
    std::set<std::uint64_t> edges;
    std::uint64_t sources = 0;
    for (const auto &member : members) {
        if (member.角色或顺序 != 1)
            throw 失败{S::内部不一致};
        auto f = 读取概念内部(概念树概念身份{member.源节点}, g, h, b);
        if (f.本体根 != out.根身份 || f.是本体根)
            throw 失败{S::内部不一致};
        if (f.来源组.size() > b.最大来源数 - sources)
            throw 失败{S::数量预算不足};
        sources += f.来源组.size();
        for (const auto &e : f.直接上位组) {
            if (!edges.insert(e.关系.值).second)
                throw 失败{S::内部不一致};
            out.直接边组.push_back(e);
            检查数量(out.直接边组.size(), b.最大关系数);
        }
        out.概念组.push_back(std::move(f));
    }
    // 出边反查包含未知下位的坏边，不能仅扫描已知下位而漏掉孤立结构。
    std::set<std::uint64_t> outgoing;
    for (const auto &f : out.概念组)
        for (const auto &e : 关系(f.概念.值, layout_.类型.直接上位, false, g, h, b.最大关系数))
            if (e.角色或顺序 != 1 || !outgoing.insert(e.编码.值).second)
                throw 失败{S::内部不一致};
    if (outgoing != edges || !校验图闭包(out))
        throw 失败{S::内部不一致};
    std::sort(out.概念组.begin(), out.概念组.end(),
              [](const auto &a, const auto &z) { return 身份小于(a.概念, z.概念); });
    std::sort(out.直接边组.begin(), out.直接边组.end(), [](const auto &a, const auto &z) {
        if (a.上位 != z.上位)
            return 身份小于(a.上位, z.上位);
        if (a.下位 != z.下位)
            return 身份小于(a.下位, z.下位);
        return 编码小于(a.关系, z.关系);
    });
    if (withSupport) {
        out.支持组.emplace();
        for (const auto &f : out.概念组) {
            auto supports = 支持组(f.概念, std::nullopt, g, h, b);
            if (supports.size() > b.最大支持数 - out.支持组->size())
                throw 失败{S::数量预算不足};
            out.支持组->insert(out.支持组->end(), supports.begin(), supports.end());
        }
        std::sort(out.支持组->begin(), out.支持组->end(), [](const auto &a, const auto &z) {
            if (a.世界引用.index() != z.世界引用.index())
                return a.世界引用.index() < z.世界引用.index();
            if (世界编码(a.世界引用) != 世界编码(z.世界引用))
                return 编码小于(世界编码(a.世界引用), 世界编码(z.世界引用));
            if (a.目标概念 != z.目标概念)
                return 身份小于(a.目标概念, z.目标概念);
            return 编码小于(a.记录, z.记录);
        });
    }
    return out;
}
std::vector<概念树形成引用事实> 概念树类数据服务::形成引用组(const 概念树形成世界引用 &world, std::uint64_t g,
                                                             std::uint64_t h, const 概念树预算 &b) const {
    if (!有效(世界编码(world)))
        throw 失败{S::入口拒绝};
    std::vector<概念树形成引用事实> out;
    const auto &t = layout_.类型;
    const std::array<稳定编码, 4> types{t.定义宿主, t.定义特征类型, t.来源存在, t.来源特征};
    for (std::size_t i = 0; i < types.size(); ++i) {
        if ((i == 0 || i == 2) ? world.index() != 0 : i == 1 ? world.index() != 2 : world.index() != 1)
            continue;
        for (const auto &edge : 关系(世界编码(world), types[i], true, g, h, b.最大关系数)) {
            if (edge.角色或顺序 != 1)
                throw 失败{S::内部不一致};
            auto parent = 关系(edge.源节点, i < 2 ? t.定义成员 : t.来源成员, true, g, h, b.最大关系数);
            if (parent.size() != 1)
                throw 失败{S::内部不一致};
            auto f = 展开概念(概念树概念身份{parent[0].源节点}, g, h, b);
            if (std::find(f.当前自有事实.begin(), f.当前自有事实.end(), edge.编码) == f.当前自有事实.end())
                throw 失败{S::内部不一致};
            if (i < 2 && (!f.事实.定义 || !std::holds_alternative<概念树特征定义>(*f.事实.定义)))
                throw 失败{S::内部不一致};
            out.push_back({edge.编码, edge.源节点, f.事实.概念, world, 投影生命(edge, h)});
            检查数量(out.size(), b.最大关系数);
        }
    }
    std::sort(out.begin(), out.end(), [](const auto &a, const auto &z) { return 编码小于(a.关系, z.关系); });
    return out;
}
概念树读取结果 概念树类数据服务::读取概念(const 概念树概念读取请求 &r) const {
    return 读取封装<概念树读取结果>(r.读取头, r.预算, [&](auto g, auto h) {
        概念树读取结果 out;
        out.概念 = 读取概念内部(r.目标概念, g, h, r.预算);
        return out;
    });
}
概念树概念组结果 概念树类数据服务::查找完整定义(const 概念树定义查找请求 &r) const {
    return 读取封装<概念树概念组结果>(r.读取头, r.预算, [&](auto g, auto h) {
        const auto d = 规范化定义(r.定义);
        auto graph = 图(d.index() == 0 ? 概念树根角色::特征 : 概念树根角色::存在, false, g, h, r.预算);
        概念树概念组结果 out;
        for (auto &f : graph.概念组)
            if (f.定义 && *f.定义 == d)
                out.概念组.push_back(std::move(f));
        if (out.概念组.size() > 1)
            throw 失败{S::内部不一致};
        return out;
    });
}
概念树图结果 概念树类数据服务::读取概念图(const 概念树图读取请求 &r) const {
    return 读取封装<概念树图结果>(r.读取头, r.预算,
                                  [&](auto g, auto h) { return 图(r.根角色, r.包含支持, g, h, r.预算); });
}
概念树支持组结果 概念树类数据服务::按概念读取支持(const 概念树按概念支持读取请求 &r) const {
    return 读取封装<概念树支持组结果>(r.读取头, r.预算, [&](auto g, auto h) {
        概念树支持组结果 out;
        out.支持组 = 支持组(r.目标概念, std::nullopt, g, h, r.预算);
        return out;
    });
}
概念树支持组结果 概念树类数据服务::按世界读取支持(const 概念树按世界支持读取请求 &r) const {
    return 读取封装<概念树支持组结果>(r.读取头, r.预算, [&](auto g, auto h) {
        概念树支持组结果 out;
        out.支持组 = 支持组(std::nullopt, r.世界引用, g, h, r.预算);
        return out;
    });
}
概念树形成引用组结果 概念树类数据服务::读取形成来源当前引用(const 概念树形成引用读取请求 &r) const {
    return 读取封装<概念树形成引用组结果>(r.读取头, r.预算, [&](auto g, auto h) {
        if (r.读取头.H)
            throw 失败{S::入口拒绝};
        概念树形成引用组结果 out;
        out.形成引用组 = 形成引用组(r.世界引用, g, h, r.预算);
        return out;
    });
}

bool 概念树类数据服务::生命完整(const 概念树生命周期 &life, std::uint64_t h, bool exited) noexcept {
    return h && life.创建事实代次 && life.创建事实代次 <= h &&
           (!life.退出事实代次 ||
            (exited && *life.退出事实代次 > life.创建事实代次 && *life.退出事实代次 <= h));
}
bool 概念树类数据服务::概念完整(const 概念树概念事实 &f, std::uint64_t h, bool exited) noexcept {
    if ((f.定义 && f.定义->valueless_by_exception()) || !有效(f.概念.值) || !有效(f.本体根.值) ||
        !有效(f.规则.值) || !生命完整(f.生命周期, h, exited) || static_cast<unsigned>(f.根角色) < 1 ||
        static_cast<unsigned>(f.根角色) > 4 || static_cast<unsigned>(f.治理状态) < 1 ||
        static_cast<unsigned>(f.治理状态) > 3)
        return false;
    if (f.是本体根)
        return f.概念 == f.本体根 && !f.定义 && !有效(f.定义记录) && !有效(f.生命周期值事实) &&
               f.来源组.empty() && f.直接上位组.empty() && f.治理状态 == 概念树生命周期状态::活跃 &&
               !f.生命周期.退出事实代次;
    if (f.概念 == f.本体根 || !f.定义 || !有效(f.定义记录) || !有效(f.生命周期值事实) || f.来源组.empty() ||
        f.直接上位组.empty())
        return false;
    if (const auto *d = std::get_if<概念树特征定义>(&*f.定义)) {
        if (d->值域.valueless_by_exception() ||
            (std::holds_alternative<概念树精确值>(d->值域) &&
             std::get<概念树精确值>(d->值域).valueless_by_exception()) ||
            f.根角色 != 概念树根角色::特征 || !有效(d->形成宿主.值) || !有效(d->特征类型.值))
            return false;
        if (auto p = std::get_if<概念树I64区间>(&d->值域)) {
            if (p->下界 >= p->上界)
                return false;
        } else if (!std::visit(
                       [](const auto &v) {
                           using T = std::decay_t<decltype(v)>;
                           if constexpr (std::is_same_v<T, std::int64_t>)
                               return true;
                           else
                               return !v.empty();
                       },
                       std::get<概念树精确值>(d->值域)))
            return false;
    } else {
        const auto &ids = std::get<概念树存在定义>(*f.定义).特征模板组;
        if (f.根角色 != 概念树根角色::存在 || ids.empty())
            return false;
        for (std::size_t i = 0; i < ids.size(); ++i)
            if (!有效(ids[i].值) || (i && !身份小于(ids[i - 1], ids[i])))
                return false;
    }
    for (std::size_t i = 0; i < f.来源组.size(); ++i) {
        const auto &s = f.来源组[i];
        if (s.来源.世界事实.valueless_by_exception() || !有效(s.记录) || !有效(s.成员关系) ||
            !有效(s.世界关系) || !有效(s.截止值事实) || !有效(世界编码(s.来源.世界事实)) ||
            !s.来源.证据截止 || s.来源.证据截止 >= f.生命周期.创建事实代次 ||
            !生命完整(s.记录生命周期, h, exited) || s.记录生命周期.创建事实代次 != f.生命周期.创建事实代次)
            return false;
        for (std::size_t j = 0; j < i; ++j)
            if (s.记录 == f.来源组[j].记录 || s.来源 == f.来源组[j].来源)
                return false;
    }
    for (std::size_t i = 0; i < f.直接上位组.size(); ++i) {
        const auto &e = f.直接上位组[i];
        if (!有效(e.关系) || !有效(e.上位.值) || e.下位 != f.概念 || e.上位 == e.下位 ||
            !生命完整(e.生命周期, h, exited))
            return false;
        for (std::size_t j = 0; j < i; ++j)
            if (e.关系 == f.直接上位组[j].关系 || e.上位 == f.直接上位组[j].上位)
                return false;
    }
    return true;
}
bool 概念树类数据服务::支持完整(const 概念树支持事实 &f, std::uint64_t h, bool exited) noexcept {
    return !f.世界引用.valueless_by_exception() && 有效(f.记录) && 有效(f.归属关系) && 有效(f.概念关系) &&
           有效(f.世界关系) && 有效(f.截止值事实) && 有效(f.目标概念.值) && 有效(世界编码(f.世界引用)) &&
           f.证据H && f.证据H < f.记录生命周期.创建事实代次 && 生命完整(f.记录生命周期, h, exited);
}
bool 概念树读取结果::成功() const noexcept {
    return 合同版本 == 1 && 状态 == 概念树数据状态::已读取 && Gread >= H && 概念 &&
           概念树类数据服务::概念完整(*概念, H);
}
bool 概念树概念组结果::成功() const noexcept {
    if (合同版本 != 1 || 状态 != 概念树数据状态::已读取 || !H || Gread < H)
        return false;
    for (std::size_t i = 0; i < 概念组.size(); ++i) {
        if (!概念树类数据服务::概念完整(概念组[i], H))
            return false;
        for (std::size_t j = 0; j < i; ++j)
            if (概念组[i].概念 == 概念组[j].概念)
                return false;
    }
    return true;
}
bool 概念树支持组结果::成功() const noexcept {
    if (合同版本 != 1 || 状态 != 概念树数据状态::已读取 || !H || Gread < H)
        return false;
    for (std::size_t i = 0; i < 支持组.size(); ++i) {
        if (!概念树类数据服务::支持完整(支持组[i], H))
            return false;
        for (std::size_t j = 0; j < i; ++j)
            if (支持组[i].记录 == 支持组[j].记录 ||
                (支持组[i].目标概念 == 支持组[j].目标概念 && 支持组[i].世界引用 == 支持组[j].世界引用))
                return false;
    }
    return true;
}
bool 概念树形成引用组结果::成功() const noexcept {
    if (合同版本 != 1 || 状态 != 概念树数据状态::已读取 || !H || Gread != H)
        return false;
    for (std::size_t i = 0; i < 形成引用组.size(); ++i) {
        const auto &f = 形成引用组[i];
        if (f.世界引用.valueless_by_exception() || !有效(f.关系) || !有效(f.记录) || !有效(f.所属概念.值) ||
            !有效(概念树类数据服务::世界编码(f.世界引用)) || !概念树类数据服务::生命完整(f.关系生命周期, H) ||
            (i && !概念树类数据服务::编码小于(形成引用组[i - 1].关系, f.关系)))
            return false;
    }
    return true;
}
bool 概念树图结果::成功() const noexcept {
    if (合同版本 != 1 || 状态 != 概念树数据状态::已读取 || !H || Gread < H || !有效(根身份.值) ||
        概念组.empty())
        return false;
    std::size_t roots = 0;
    for (std::size_t i = 0; i < 概念组.size(); ++i) {
        const auto &f = 概念组[i];
        if (!概念树类数据服务::概念完整(f, H) || f.本体根 != 根身份)
            return false;
        if (f.是本体根)
            ++roots;
        for (std::size_t j = 0; j < i; ++j)
            if (f.概念 == 概念组[j].概念)
                return false;
        for (const auto &e : f.直接上位组)
            if (std::count(直接边组.begin(), 直接边组.end(), e) != 1)
                return false;
    }
    if (roots != 1)
        return false;
    for (std::size_t i = 0; i < 直接边组.size(); ++i) {
        const auto &e = 直接边组[i];
        auto p = std::find_if(概念组.begin(), 概念组.end(), [&](const auto &f) { return f.概念 == e.上位; });
        auto c = std::find_if(概念组.begin(), 概念组.end(), [&](const auto &f) { return f.概念 == e.下位; });
        if (p == 概念组.end() || c == 概念组.end() ||
            std::count(c->直接上位组.begin(), c->直接上位组.end(), e) != 1)
            return false;
        for (std::size_t j = 0; j < i; ++j)
            if (e.关系 == 直接边组[j].关系)
                return false;
    }
    if (支持组)
        for (const auto &s : *支持组) {
            if (!概念树类数据服务::支持完整(s, H) ||
                std::none_of(概念组.begin(), 概念组.end(),
                             [&](const auto &f) { return f.概念 == s.目标概念; }))
                return false;
        }
    return true;
}

概念树业务写请求 概念树类数据服务::规范写请求(const 概念树业务写请求 &input) const {
    auto out = input;
    std::visit(
        [&](auto &r) {
            using T = std::decay_t<decltype(r)>;
            if (r.写入头.合同版本 != 1 || !r.写入头.期望事实代次 || !r.写入头.幂等身份)
                throw 失败{S::入口拒绝};
            预算有效(r.预算);
            if constexpr (std::is_same_v<T, 概念树概念建立请求>) {
                r.定义 = 规范化定义(r.定义);
                if (r.直接上位.empty() || r.来源.empty())
                    throw 失败{S::入口拒绝};
                // 每类本地键最多 0xFFFF 项，先用减法界限拒绝不可表示的请求。
                if (r.来源.size() > 0xFFFF - 2 || r.直接上位.size() > 0xFFFF - 3)
                    throw 失败{S::数量预算不足};
                const std::size_t definitionEdges =
                    r.定义.index() == 0 ? 2 : std::get<概念树存在定义>(r.定义).特征模板组.size();
                if (definitionEdges > 0xFFFF - 3 - r.直接上位.size() ||
                    r.来源.size() > (0xFFFF - 3 - r.直接上位.size() - definitionEdges) / 2)
                    throw 失败{S::数量预算不足};
                检查数量(r.直接上位.size(), r.预算.最大关系数);
                检查数量(r.来源.size(), r.预算.最大来源数);
                std::sort(r.直接上位.begin(), r.直接上位.end(), 身份小于);
                for (std::size_t i = 0; i < r.直接上位.size(); ++i)
                    if (!有效(r.直接上位[i].值) || (i && r.直接上位[i] == r.直接上位[i - 1]))
                        throw 失败{S::入口拒绝};
                std::sort(r.来源.begin(), r.来源.end(), [](const auto &a, const auto &z) {
                    if (a.世界事实.index() != z.世界事实.index())
                        return a.世界事实.index() < z.世界事实.index();
                    if (世界编码(a.世界事实) != 世界编码(z.世界事实))
                        return 编码小于(世界编码(a.世界事实), 世界编码(z.世界事实));
                    return a.证据截止 < z.证据截止;
                });
                for (std::size_t i = 0; i < r.来源.size(); ++i)
                    if (!有效(世界编码(r.来源[i].世界事实)) || !r.来源[i].证据截止 ||
                        r.来源[i].证据截止 > r.写入头.期望事实代次 || (i && r.来源[i] == r.来源[i - 1]))
                        throw 失败{S::入口拒绝};
            } else if constexpr (std::is_same_v<T, 概念树上位操作请求>) {
                if (!有效(r.下位.值))
                    throw 失败{S::入口拒绝};
                const bool old = r.旧上位 && r.旧关系 && 有效(r.旧上位->值) && 有效(*r.旧关系);
                const bool fresh = r.新上位 && 有效(r.新上位->值);
                if (r.操作 == 概念树上位操作::新增) {
                    if (r.旧上位 || r.旧关系 || !fresh)
                        throw 失败{S::入口拒绝};
                } else if (r.操作 == 概念树上位操作::替换) {
                    if (!old || !fresh || r.旧上位 == r.新上位)
                        throw 失败{S::入口拒绝};
                } else if (r.操作 == 概念树上位操作::退出) {
                    if (!old || r.新上位)
                        throw 失败{S::入口拒绝};
                } else
                    throw 失败{S::入口拒绝};
            } else if constexpr (std::is_same_v<T, 概念树支持建立请求>) {
                if (!有效(r.目标概念.值) || !有效(世界编码(r.世界引用)) || !r.证据H ||
                    r.证据H > r.写入头.期望事实代次)
                    throw 失败{S::入口拒绝};
            } else if constexpr (std::is_same_v<T, 概念树支持退出请求>) {
                if (!支持完整(r.预期支持, r.写入头.期望事实代次))
                    throw 失败{S::入口拒绝};
            } else if constexpr (std::is_same_v<T, 概念树生命周期迁移请求>) {
                if (!有效(r.概念.值) || !有效(r.当前生命周期值编码) ||
                    static_cast<unsigned>(r.预期状态) < 1 || static_cast<unsigned>(r.预期状态) > 3 ||
                    static_cast<unsigned>(r.目标状态) < 1 || static_cast<unsigned>(r.目标状态) > 3)
                    throw 失败{S::入口拒绝};
            } else if constexpr (std::is_same_v<T, 概念树概念退出请求>) {
                if (!有效(r.概念.值) || !有效(r.当前退役生命周期值编码))
                    throw 失败{S::入口拒绝};
            } else {
                if (!有效(世界编码(r.世界引用)))
                    throw 失败{S::入口拒绝};
                检查数量(r.预期当前引用.size(), r.预算.最大关系数);
                for (std::size_t i = 0; i < r.预期当前引用.size(); ++i) {
                    const auto &f = r.预期当前引用[i];
                    if (f.世界引用.valueless_by_exception() || !有效(f.关系) || !有效(f.记录) ||
                        !有效(f.所属概念.值) || f.世界引用 != r.世界引用 ||
                        !生命完整(f.关系生命周期, r.写入头.期望事实代次) ||
                        (i && !编码小于(r.预期当前引用[i - 1].关系, f.关系)))
                        throw 失败{S::入口拒绝};
                }
            }
        },
        out);
    return out;
}
L1所有者范围写集请求 概念树类数据服务::形成写集(const 概念树业务写请求 &request, std::uint64_t g,
                                                std::uint64_t h) const {
    L1所有者范围写集请求 out;
    const auto &t = layout_.类型;
    std::uint32_t nk = 1, rk = 0x10000, vk = 0x20000;
    auto newNode = [&]() -> Ref {
        if (nk > 0xFFFF)
            throw 失败{S::数量预算不足};
        const Key key{nk++};
        out.节点.push_back({key, 节点种类::普通, std::nullopt});
        return key;
    };
    auto newEdge = [&](Ref source, Ref target, 稳定编码 type, std::int64_t role = 1) {
        if (rk >= 0x1FFFF)
            throw 失败{S::数量预算不足};
        out.关系.push_back({Key{rk++}, source, target, Ref{type}, role});
    };
    auto newValue = [&](Ref node, 稳定编码 type, L1所有者范围原始值材料 value) {
        if (vk >= 0x2FFFF)
            throw 失败{S::数量预算不足};
        const Key key{vk++};
        out.值.push_back({key, node, Ref{type}, std::move(value), node});
        out.属性槽变更.push_back({node, Ref{type}, key});
    };
    auto path = [](const 概念树图结果 &graph, 概念树概念身份 from, 概念树概念身份 to) {
        std::set<std::uint64_t> seen{from.值.值};
        std::vector<概念树概念身份> queue{from};
        for (std::size_t i = 0; i < queue.size(); ++i) {
            if (queue[i] == to)
                return true;
            for (const auto &e : graph.直接边组)
                if (e.上位 == queue[i] && seen.insert(e.下位.值.值).second)
                    queue.push_back(e.下位);
        }
        return false;
    };
    std::visit(
        [&](const auto &r) {
            using T = std::decay_t<decltype(r)>;
            out.期望事实代次 = r.写入头.期望事实代次;
            out.写入幂等身份 = {r.写入头.幂等身份};
            const auto &b = r.预算;
            if constexpr (std::is_same_v<T, 概念树概念建立请求>) {
                const auto role = r.定义.index() == 0 ? 概念树根角色::特征 : 概念树根角色::存在;
                const auto graph = 图(role, false, g, h, b);
                std::size_t matches = 0;
                for (const auto &f : graph.概念组)
                    if (f.定义 && *f.定义 == r.定义)
                        ++matches;
                if (matches > 1)
                    throw 失败{S::内部不一致};
                if (matches)
                    return;
                检查数量(graph.概念组.size() + 1, b.最大概念数);
                if (r.直接上位.size() > b.最大关系数 - graph.直接边组.size())
                    throw 失败{S::数量预算不足};
                std::uint64_t oldSources = 0;
                for (const auto &f : graph.概念组) {
                    if (f.来源组.size() > b.最大来源数 - oldSources)
                        throw 失败{S::数量预算不足};
                    oldSources += f.来源组.size();
                }
                if (r.来源.size() > b.最大来源数 - oldSources)
                    throw 失败{S::数量预算不足};
                for (auto parent : r.直接上位) {
                    const auto f = 读取概念内部(parent, g, h, b);
                    if (f.本体根 != 根(role))
                        throw 失败{S::引用冲突};
                }
                for (const auto &source : r.来源) {
                    验证世界(扩展世界(source.世界事实), g, source.证据截止, b);
                    验证世界(扩展世界(source.世界事实), g, h, b);
                }
                if (auto d = std::get_if<概念树特征定义>(&r.定义))
                    验证特征定义(*d, g, h, b);
                else
                    for (auto id : std::get<概念树存在定义>(r.定义).特征模板组) {
                        const auto f = 读取概念内部(id, g, h, b);
                        if (!f.定义 || !std::holds_alternative<概念树特征定义>(*f.定义))
                            throw 失败{S::引用冲突};
                    }
                const auto cn = newNode(), dn = newNode();
                std::vector<Ref> sourceNodes;
                for (std::size_t i = 0; i < r.来源.size(); ++i)
                    sourceNodes.push_back(newNode());
                newEdge(cn, Ref{根(role).值}, t.概念根归属);
                newEdge(cn, Ref{layout_.签名规则.值}, t.概念规则);
                newEdge(cn, dn, t.定义成员);
                for (auto parent : r.直接上位)
                    newEdge(Ref{parent.值}, cn, t.直接上位);
                if (auto d = std::get_if<概念树特征定义>(&r.定义)) {
                    newEdge(dn, Ref{d->形成宿主.值}, t.定义宿主);
                    newEdge(dn, Ref{d->特征类型.值}, t.定义特征类型);
                } else {
                    std::int64_t order = 0;
                    for (auto id : std::get<概念树存在定义>(r.定义).特征模板组)
                        newEdge(dn, Ref{id.值}, t.定义模板, ++order);
                }
                for (std::size_t i = 0; i < r.来源.size(); ++i) {
                    newEdge(cn, sourceNodes[i], t.来源成员, static_cast<std::int64_t>(i + 1));
                    newEdge(sourceNodes[i], Ref{世界编码(r.来源[i].世界事实)},
                            r.来源[i].世界事实.index() == 0 ? t.来源存在 : t.来源特征);
                }
                newValue(cn, t.概念种类, std::int64_t{r.定义.index() == 0 ? 1 : 2});
                newValue(cn, t.生命周期, std::int64_t{1});
                if (auto d = std::get_if<概念树特征定义>(&r.定义)) {
                    if (auto exact = std::get_if<概念树精确值>(&d->值域)) {
                        const auto type = exact->index() == 0   ? t.精确I64
                                          : exact->index() == 1 ? t.精确I64组
                                                                : t.精确U64组;
                        std::visit([&](const auto &v) { newValue(dn, type, v); }, *exact);
                    } else {
                        const auto interval = std::get<概念树I64区间>(d->值域);
                        newValue(dn, t.区间下界, interval.下界);
                        newValue(dn, t.区间上界, interval.上界);
                    }
                }
                for (std::size_t i = 0; i < r.来源.size(); ++i)
                    newValue(sourceNodes[i], t.证据截止, std::vector<std::uint64_t>{r.来源[i].证据截止});
            } else if constexpr (std::is_same_v<T, 概念树上位操作请求>) {
                const auto child = 读取概念内部(r.下位, g, h, b);
                if (child.是本体根)
                    throw 失败{S::根不可退出};
                auto graph = 图(child.根角色, false, g, h, b);
                if (r.旧关系) {
                    auto old = std::find_if(graph.直接边组.begin(), graph.直接边组.end(), [&](const auto &e) {
                        return e.关系 == *r.旧关系 && e.上位 == *r.旧上位 && e.下位 == r.下位;
                    });
                    if (old == graph.直接边组.end())
                        throw 失败{S::引用冲突};
                    out.退出事实.push_back(old->关系);
                    graph.直接边组.erase(old);
                }
                if (r.新上位) {
                    const auto parent = 读取概念内部(*r.新上位, g, h, b);
                    if (parent.本体根 != child.本体根)
                        throw 失败{S::引用冲突};
                    if (path(graph, r.下位, *r.新上位))
                        throw 失败{S::形成环};
                    if (std::any_of(graph.直接边组.begin(), graph.直接边组.end(),
                                    [&](const auto &e) { return e.上位 == *r.新上位 && e.下位 == r.下位; }))
                        throw 失败{S::引用冲突};
                    newEdge(Ref{r.新上位->值}, Ref{r.下位.值}, t.直接上位);
                    graph.直接边组.push_back({{}, *r.新上位, r.下位, {h, std::nullopt}});
                }
                if (!校验图闭包(graph))
                    throw 失败{S::引用冲突};
                检查数量(graph.直接边组.size(), b.最大关系数);
            } else if constexpr (std::is_same_v<T, 概念树支持建立请求>) {
                (void)读取概念内部(r.目标概念, g, h, b);
                auto supports = 支持组(r.目标概念, std::nullopt, g, h, b);
                if (std::any_of(supports.begin(), supports.end(),
                                [&](const auto &s) { return s.世界引用 == r.世界引用; }))
                    return;
                检查数量(supports.size() + 1, b.最大支持数);
                检查数量(支持组(std::nullopt, r.世界引用, g, h, b).size() + 1, b.最大支持数);
                验证世界(扩展世界(r.世界引用), g, r.证据H, b);
                验证世界(扩展世界(r.世界引用), g, h, b);
                const auto n = newNode();
                newEdge(Ref{layout_.格式锚点}, n, t.支持归属);
                newEdge(n, Ref{r.目标概念.值}, t.支持概念);
                newEdge(n, Ref{世界编码(r.世界引用)}, r.世界引用.index() == 0 ? t.支持存在 : t.支持特征);
                newValue(n, t.证据截止, std::vector<std::uint64_t>{r.证据H});
            } else if constexpr (std::is_same_v<T, 概念树支持退出请求>) {
                const auto s = 支持记录(r.预期支持.记录, g, h, b);
                if (s != r.预期支持)
                    throw 失败{S::引用冲突};
                out.退出事实 = {s.记录, s.归属关系, s.概念关系, s.世界关系, s.截止值事实};
            } else if constexpr (std::is_same_v<T, 概念树生命周期迁移请求>) {
                const auto f = 读取概念内部(r.概念, g, h, b);
                if (f.是本体根)
                    throw 失败{S::根不可退出};
                if (f.治理状态 != r.预期状态 || f.生命周期值事实 != r.当前生命周期值编码)
                    throw 失败{S::引用冲突};
                if (r.预期状态 == r.目标状态)
                    return;
                using L = 概念树生命周期状态;
                if (!((r.预期状态 == L::活跃 && r.目标状态 == L::冷却) ||
                      (r.预期状态 == L::冷却 && (r.目标状态 == L::活跃 || r.目标状态 == L::退役)) ||
                      (r.预期状态 == L::退役 && r.目标状态 == L::活跃)))
                    throw 失败{S::入口拒绝};
                out.退出事实.push_back(r.当前生命周期值编码);
                newValue(Ref{r.概念.值}, t.生命周期, static_cast<std::int64_t>(r.目标状态));
            } else if constexpr (std::is_same_v<T, 概念树概念退出请求>) {
                const auto expanded = 展开概念(r.概念, g, h, b);
                const auto &f = expanded.事实;
                if (f.是本体根)
                    throw 失败{S::根不可退出};
                if (f.治理状态 != 概念树生命周期状态::退役 || f.生命周期值事实 != r.当前退役生命周期值编码)
                    throw 失败{S::引用冲突};
                if (!支持组(r.概念, std::nullopt, g, h, b).empty() ||
                    !关系(r.概念.值, t.定义模板, true, g, h, b.最大关系数).empty())
                    throw 失败{S::引用冲突};
                auto graph = 图(f.根角色, false, g, h, b);
                std::vector<概念树概念身份> parents, children;
                out.退出事实 = expanded.当前自有事实;
                for (const auto &e : graph.直接边组) {
                    if (e.下位 == r.概念)
                        parents.push_back(e.上位);
                    if (e.上位 == r.概念)
                        children.push_back(e.下位);
                    if (e.上位 == r.概念 || e.下位 == r.概念)
                        out.退出事实.push_back(e.关系);
                }
                std::erase_if(graph.概念组, [&](const auto &n) { return n.概念 == r.概念; });
                std::erase_if(graph.直接边组,
                              [&](const auto &e) { return e.上位 == r.概念 || e.下位 == r.概念; });
                std::sort(parents.begin(), parents.end(), 身份小于);
                std::sort(children.begin(), children.end(), 身份小于);
                if (!children.empty() && parents.size() > b.最大关系数 / children.size())
                    throw 失败{S::数量预算不足};
                for (auto parent : parents)
                    for (auto child : children) {
                        if (path(graph, parent, child))
                            continue;
                        if (path(graph, child, parent))
                            throw 失败{S::形成环};
                        newEdge(Ref{parent.值}, Ref{child.值}, t.直接上位);
                        graph.直接边组.push_back({{}, parent, child, {h, std::nullopt}});
                    }
                if (!校验图闭包(graph))
                    throw 失败{S::内部不一致};
                检查数量(graph.直接边组.size(), b.最大关系数);
            } else {
                const auto actual = 形成引用组(r.世界引用, g, h, b);
                if (actual != r.预期当前引用)
                    throw 失败{S::引用冲突};
                for (const auto &f : actual)
                    out.退出事实.push_back(f.关系);
            }
            检查数量(out.关系.size(), b.最大关系数);
            std::sort(out.退出事实.begin(), out.退出事实.end(), 编码小于);
            if (std::adjacent_find(out.退出事实.begin(), out.退出事实.end()) != out.退出事实.end())
                throw 失败{S::内部不一致};
        },
        request);
    // L1 首次材料以所属节点、属性类型排序槽变更；此处使用同一中性顺序。
    auto refLess = [](const Ref &a, const Ref &z) {
        if (a.index() != z.index())
            return a.index() < z.index();
        return std::visit([](const auto &x) { return static_cast<std::uint64_t>(x.值); }, a) <
               std::visit([](const auto &x) { return static_cast<std::uint64_t>(x.值); }, z);
    };
    std::sort(out.属性槽变更.begin(), out.属性槽变更.end(), [&](const auto &a, const auto &z) {
        if (a.所属节点 != z.所属节点)
            return refLess(a.所属节点, z.所属节点);
        return refLess(a.属性类型节点, z.属性类型节点);
    });
    return out;
}

// 首次写集和编码映射共同重建结果；任何提交后读回失败都由外层保留发布可能性。
概念树写入结果 概念树类数据服务::发布结果(const 概念树业务写请求 &request, const L1所有者范围写集请求 &ws,
                                          const L1所有者范围写入结果 &committed, std::uint64_t g) const {
    const auto h = committed.事实代次;
    if (committed.合同版本 != L1所有者范围CRUD合同版本 || committed.所有者 != port_.所有者身份() ||
        committed.写入幂等身份 != ws.写入幂等身份 || !h || h <= ws.期望事实代次 || h > g ||
        !committed.是否形成内存权威发布 ||
        (committed.状态 != L1所有者范围写入状态::成功 && committed.状态 != L1所有者范围写入状态::精确重复) ||
        (committed.重试边界 != L1所有者范围重试边界::不适用 &&
         committed.重试边界 != L1所有者范围重试边界::原幂等身份读回收敛))
        throw 失败{S::内部不一致};
    if (committed.新编码映射.size() != ws.节点.size() + ws.关系.size() + ws.值.size())
        throw 失败{S::内部不一致};
    std::map<std::uint32_t, 稳定编码> mappings;
    std::set<std::uint64_t> ids;
    for (const auto &[key, id] : committed.新编码映射)
        if (!key.值 || !有效(id) || !mappings.emplace(key.值, id).second || !ids.insert(id.值).second)
            throw 失败{S::内部不一致};
    auto mapped = [&](const Ref &ref) -> 稳定编码 {
        if (auto id = std::get_if<稳定编码>(&ref))
            return *id;
        const auto i = mappings.find(std::get<Key>(ref).值);
        if (i == mappings.end())
            throw 失败{S::内部不一致};
        return i->second;
    };
    for (const auto &expected : ws.节点) {
        const auto actual = 节点(mapped(expected.本地键), g, h);
        if (actual.创建事实代次 != h || actual.种类 != expected.种类 ||
            actual.属性类型表示 != expected.属性类型表示)
            throw 失败{S::内部不一致};
    }
    for (const auto &expected : ws.关系) {
        const auto raw = 原始事实(mapped(expected.本地键), g);
        const auto *actual = std::get_if<E>(&raw);
        if (!actual || !有效于(*actual, h) || actual->创建事实代次 != h ||
            actual->源节点 != mapped(expected.源节点) || actual->目标节点 != mapped(expected.目标节点) ||
            actual->关系类型节点 != mapped(expected.关系类型节点) ||
            actual->角色或顺序 != expected.角色或顺序)
            throw 失败{S::内部不一致};
    }
    for (const auto &expected : ws.值) {
        const auto raw = 原始事实(mapped(expected.本地键), g);
        const auto *actual = std::get_if<V>(&raw);
        if (!actual || !有效于(*actual, h) || actual->创建事实代次 != h ||
            actual->所属节点 != mapped(expected.所属节点) ||
            actual->属性类型节点 != mapped(expected.属性类型节点) ||
            actual->来源节点 != mapped(expected.来源节点) || actual->材料 != expected.材料)
            throw 失败{S::内部不一致};
    }
    for (auto id : ws.退出事实) {
        const auto raw = 原始事实(id, g);
        if (!std::visit([&](const auto &f) { return f.退出事实代次 && *f.退出事实代次 == h; }, raw))
            throw 失败{S::内部不一致};
    }
    概念树写入结果 out;
    out.事实代次 = g;
    out.发布状态 = P::已确认发布;
    out.发布截止 = h;
    out.原请求 = request;
    out.已退出事实 = ws.退出事实;
    for (const auto &edge : ws.关系)
        if (edge.关系类型节点 == Ref{layout_.类型.直接上位})
            out.关系组.push_back({mapped(edge.本地键),
                                  概念树概念身份{mapped(edge.源节点)},
                                  概念树概念身份{mapped(edge.目标节点)},
                                  {h, std::nullopt}});
    std::visit(
        [&](const auto &r) {
            using T = std::decay_t<decltype(r)>;
            if constexpr (std::is_same_v<T, 概念树概念建立请求>) {
                out.状态 = S::已创建;
                out.概念 = 读取概念内部(概念树概念身份{mapped(Key{1})}, g, h, r.预算);
                if (out.概念->定义 != std::optional<概念树定义>{r.定义})
                    throw 失败{S::内部不一致};
                (void)图(out.概念->根角色, false, g, h, r.预算);
            } else if constexpr (std::is_same_v<T, 概念树上位操作请求>) {
                out.状态 = r.操作 == 概念树上位操作::新增   ? S::已添加上位
                           : r.操作 == 概念树上位操作::替换 ? S::已替换上位
                                                            : S::已退出上位;
                out.概念 = 读取概念内部(r.下位, g, h, r.预算);
                (void)图(out.概念->根角色, false, g, h, r.预算);
            } else if constexpr (std::is_same_v<T, 概念树支持建立请求>) {
                out.状态 = S::已添加支持;
                out.支持 = 支持记录(mapped(Key{1}), g, h, r.预算);
                const auto a = 支持组(r.目标概念, std::nullopt, g, h, r.预算);
                const auto b = 支持组(std::nullopt, r.世界引用, g, h, r.预算);
                if (std::count(a.begin(), a.end(), *out.支持) != 1 ||
                    std::count(b.begin(), b.end(), *out.支持) != 1)
                    throw 失败{S::内部不一致};
            } else if constexpr (std::is_same_v<T, 概念树支持退出请求>) {
                out.状态 = S::已退出支持;
                out.支持 = 支持记录(r.预期支持.记录, g, r.写入头.期望事实代次, r.预算);
                out.支持->记录生命周期.退出事实代次 = h;
                const auto a = 支持组(r.预期支持.目标概念, std::nullopt, g, h, r.预算);
                const auto b = 支持组(std::nullopt, r.预期支持.世界引用, g, h, r.预算);
                for (const auto *list : {&a, &b})
                    if (std::any_of(list->begin(), list->end(),
                                    [&](const auto &f) { return f.记录 == r.预期支持.记录; }))
                        throw 失败{S::内部不一致};
            } else if constexpr (std::is_same_v<T, 概念树生命周期迁移请求>) {
                out.状态 = S::已迁移生命周期;
                out.概念 = 读取概念内部(r.概念, g, h, r.预算);
                if (out.概念->治理状态 != r.目标状态 || out.概念->生命周期值事实 != mapped(Key{0x20000}))
                    throw 失败{S::内部不一致};
            } else if constexpr (std::is_same_v<T, 概念树概念退出请求>) {
                out.状态 = S::已退出概念;
                out.概念 = 读取概念内部(r.概念, g, r.写入头.期望事实代次, r.预算);
                out.概念->生命周期.退出事实代次 = h;
                for (auto &source : out.概念->来源组)
                    source.记录生命周期.退出事实代次 = h;
                for (auto &edge : out.概念->直接上位组)
                    edge.生命周期.退出事实代次 = h;
                (void)图(out.概念->根角色, false, g, h, r.预算);
            } else {
                out.状态 = S::已释放引用;
                const auto remaining = 形成引用组(r.世界引用, g, h, r.预算);
                if (!remaining.empty())
                    throw 失败{S::内部不一致};
                for (const auto &f : r.预期当前引用)
                    (void)读取概念内部(f.所属概念, g, h, r.预算);
            }
        },
        request);
    if (!out.成功())
        throw 失败{S::内部不一致};
    return out;
}
概念树写入结果 概念树类数据服务::执行写入(const 概念树业务写请求 &input) {
    概念树写入结果 out;
    bool keyUnknown = false;
    bool dispatched = false;
    try {
        out.原请求 = input;
        const auto request = 规范写请求(input);
        out.原请求 = request;
        const auto header = std::visit([](const auto &r) { return r.写入头; }, request);
        std::scoped_lock lock(mutex_);
        keyUnknown = true;
        const auto first = port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, {header.幂等身份}});
        if (first.状态 != L1所有者范围读取状态::成功 && first.状态 != L1所有者范围读取状态::未找到)
            throw 失败{映射(first.状态)};
        if (first.合同版本 != L1所有者范围首次写入读取合同版本 || first.所有者 != port_.所有者身份() ||
            first.写入幂等身份.值 != header.幂等身份 || !first.读取事实代次)
            throw 失败{S::内部不一致};
        const auto g = first.读取事实代次;
        out.事实代次 = g;
        if (first.状态 == L1所有者范围读取状态::成功 && first.首次写入结果 && first.首次写入结果->事实代次 &&
            first.首次写入结果->事实代次 <= g)
            out.发布截止 = first.首次写入结果->事实代次;
        守卫代次(g);
        if (first.状态 == L1所有者范围读取状态::成功) {
            dispatched = true;
            if (!first.首次规范化写集 || !first.首次写入结果)
                throw 失败{S::内部不一致};
            const auto &saved = *first.首次写入结果;
            if (saved.事实代次 && saved.事实代次 <= g)
                out.发布截止 = saved.事实代次;
            if (header.期望事实代次 != first.首次规范化写集->期望事实代次) {
                dispatched = false;
                keyUnknown = false;
                throw 失败{S::幂等冲突};
            }
            L1所有者范围写集请求 expected;
            try {
                expected = 形成写集(request, g, header.期望事实代次);
            } catch (const 失败 &e) {
                if (e.状态 == S::引用冲突 || e.状态 == S::入口拒绝 || e.状态 == S::形成环 ||
                    e.状态 == S::未找到 || e.状态 == S::目标已退出 || e.状态 == S::根不可退出) {
                    dispatched = false;
                    keyUnknown = false;
                    throw 失败{S::幂等冲突};
                }
                throw;
            }
            if (expected != *first.首次规范化写集) {
                dispatched = false;
                keyUnknown = false;
                throw 失败{S::幂等冲突};
            }
            out = 发布结果(request, expected, saved, g);
            out.状态 = S::精确重复;
            守卫代次(g);
            if (!out.成功())
                throw 失败{S::内部不一致};
            return out;
        }
        if (first.首次规范化写集 || first.首次写入结果)
            throw 失败{S::内部不一致};
        keyUnknown = false;
        if (g != header.期望事实代次)
            throw 失败{S::事实代次漂移};
        const auto ws = 形成写集(request, g, g);
        if (ws.节点.empty() && ws.关系.empty() && ws.值.empty() && ws.退出事实.empty()) {
            out.状态 = S::无须变更;
            std::visit(
                [&](const auto &r) {
                    using T = std::decay_t<decltype(r)>;
                    if constexpr (std::is_same_v<T, 概念树概念建立请求>) {
                        const auto graph = 图(r.定义.index() == 0 ? 概念树根角色::特征 : 概念树根角色::存在,
                                              false, g, g, r.预算);
                        for (const auto &f : graph.概念组)
                            if (f.定义 && *f.定义 == r.定义)
                                out.概念 = f;
                    } else if constexpr (std::is_same_v<T, 概念树支持建立请求>) {
                        for (const auto &f : 支持组(r.目标概念, std::nullopt, g, g, r.预算))
                            if (f.世界引用 == r.世界引用)
                                out.支持 = f;
                    } else if constexpr (std::is_same_v<T, 概念树生命周期迁移请求>)
                        out.概念 = 读取概念内部(r.概念, g, g, r.预算);
                },
                request);
            守卫代次(g);
            if (!out.成功())
                throw 失败{S::内部不一致};
            return out;
        }
        守卫代次(g);
        dispatched = true;
        const auto committed = port_.提交所有者范围中性写集(ws);
        if (committed.是否形成内存权威发布 && committed.事实代次 > g)
            out.发布截止 = committed.事实代次;
        const auto after = 当前代次();
        out.事实代次 = after;
        out = 发布结果(request, ws, committed, after);
        守卫代次(after);
        return out;
    } catch (const 失败 &e) {
        out.状态 = e.状态;
    } catch (const std::bad_alloc &) {
        out.状态 = S::资源失败;
    } catch (const std::length_error &) {
        out.状态 = S::资源失败;
    } catch (...) {
        out.状态 = S::内部不一致;
    }
    out.概念.reset();
    out.支持.reset();
    out.关系组.clear();
    out.已退出事实.clear();
    if (dispatched || keyUnknown) {
        out.状态 = S::已可能发布;
        out.发布状态 = P::可能已发布;
    } else {
        out.发布状态 = out.发布截止 ? P::可能已发布 : P::未派发;
    }
    return out;
}
bool 概念树写入结果::成功() const noexcept {
    using C = 概念树类数据服务;
    using S = 概念树数据状态;
    if (合同版本 != 1 || !事实代次 || !原请求 || 原请求->valueless_by_exception())
        return false;
    const bool noChange = 状态 == S::无须变更;
    if (noChange) {
        if (发布状态 != 概念树发布状态::未派发 || 发布截止 || !关系组.empty() || !已退出事实.empty())
            return false;
    } else if (发布状态 != 概念树发布状态::已确认发布 || !发布截止 || !*发布截止 || *发布截止 > 事实代次)
        return false;
    const auto h = noChange ? 事实代次 : *发布截止;
    if (概念 && !C::概念完整(*概念, h, 概念->生命周期.退出事实代次.has_value()))
        return false;
    if (支持 && !C::支持完整(*支持, h, 支持->记录生命周期.退出事实代次.has_value()))
        return false;
    for (std::size_t i = 0; i < 已退出事实.size(); ++i)
        if (!有效(已退出事实[i]) || (i && !C::编码小于(已退出事实[i - 1], 已退出事实[i])))
            return false;
    for (std::size_t i = 0; i < 关系组.size(); ++i) {
        const auto &e = 关系组[i];
        if (!有效(e.关系) || !有效(e.上位.值) || !有效(e.下位.值) || e.上位 == e.下位 ||
            !C::生命完整(e.生命周期, h) || e.生命周期.创建事实代次 != h)
            return false;
        for (std::size_t j = 0; j < i; ++j)
            if (e.关系 == 关系组[j].关系)
                return false;
    }
    return std::visit(
        [&](const auto &r) {
            using T = std::decay_t<decltype(r)>;
            if (r.写入头.合同版本 != 1 || !r.写入头.幂等身份 || !r.写入头.期望事实代次 ||
                (noChange ? r.写入头.期望事实代次 != h : r.写入头.期望事实代次 >= h))
                return false;
            const bool replay = 状态 == S::精确重复;
            if constexpr (std::is_same_v<T, 概念树概念建立请求>) {
                return (noChange || replay || 状态 == S::已创建) && 概念 && !支持 &&
                       !概念->生命周期.退出事实代次 && 概念->定义 && *概念->定义 == r.定义 &&
                       已退出事实.empty() && (noChange || 概念->生命周期.创建事实代次 == h);
            } else if constexpr (std::is_same_v<T, 概念树上位操作请求>) {
                const auto expected = r.操作 == 概念树上位操作::新增   ? S::已添加上位
                                      : r.操作 == 概念树上位操作::替换 ? S::已替换上位
                                                                       : S::已退出上位;
                return !noChange && (replay || 状态 == expected) && 概念 && !支持 && 概念->概念 == r.下位 &&
                       !概念->生命周期.退出事实代次 && 关系组.size() == (r.新上位 ? 1 : 0) &&
                       已退出事实.size() == (r.旧关系 ? 1 : 0) && (!r.旧关系 || 已退出事实[0] == *r.旧关系) &&
                       (!r.新上位 || (关系组[0].上位 == *r.新上位 && 关系组[0].下位 == r.下位));
            } else if constexpr (std::is_same_v<T, 概念树支持建立请求>) {
                return (noChange || replay || 状态 == S::已添加支持) && 支持 && !概念 && 关系组.empty() &&
                       已退出事实.empty() && 支持->目标概念 == r.目标概念 && 支持->世界引用 == r.世界引用 &&
                       !支持->记录生命周期.退出事实代次 &&
                       (noChange || (支持->记录生命周期.创建事实代次 == h && 支持->证据H == r.证据H));
            } else if constexpr (std::is_same_v<T, 概念树支持退出请求>) {
                return !noChange && (replay || 状态 == S::已退出支持) && 支持 && !概念 && 关系组.empty() &&
                       已退出事实.size() == 5 && 支持->记录 == r.预期支持.记录 &&
                       支持->记录生命周期.退出事实代次 == 发布截止;
            } else if constexpr (std::is_same_v<T, 概念树生命周期迁移请求>) {
                return (noChange || replay || 状态 == S::已迁移生命周期) && 概念 && !支持 && 关系组.empty() &&
                       概念->概念 == r.概念 && 概念->治理状态 == r.目标状态 && !概念->生命周期.退出事实代次 &&
                       (noChange ? r.预期状态 == r.目标状态
                                 : 已退出事实.size() == 1 && 已退出事实[0] == r.当前生命周期值编码);
            } else if constexpr (std::is_same_v<T, 概念树概念退出请求>) {
                return !noChange && (replay || 状态 == S::已退出概念) && 概念 && !支持 &&
                       概念->概念 == r.概念 && 概念->治理状态 == 概念树生命周期状态::退役 &&
                       概念->生命周期.退出事实代次 == 发布截止 &&
                       std::find(已退出事实.begin(), 已退出事实.end(), r.概念.值) != 已退出事实.end();
            } else {
                if (!(noChange || replay || 状态 == S::已释放引用) || 概念 || 支持 || !关系组.empty() ||
                    已退出事实.size() != r.预期当前引用.size())
                    return false;
                for (std::size_t i = 0; i < 已退出事实.size(); ++i)
                    if (已退出事实[i] != r.预期当前引用[i].关系)
                        return false;
                return noChange ? r.预期当前引用.empty() : !r.预期当前引用.empty();
            }
        },
        *原请求);
}

} // namespace 海中鱼巣
