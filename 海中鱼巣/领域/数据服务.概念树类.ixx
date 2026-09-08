module;

#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <limits>
#include <map>
#include <mutex>
#include <numeric>
#include <optional>
#include <set>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.领域.数据服务.概念树类;

export import 海中鱼巣.领域.数据服务.存在类;
export import 海中鱼巣.领域.数据服务.场景类;

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
struct 概念树场景引用 final {
    稳定编码 值{};
    概念树场景引用() = default;
    explicit 概念树场景引用(稳定编码 id) : 值(id) {
    }
    friend bool operator==(const 概念树场景引用 &, const 概念树场景引用 &) = default;
};
struct 概念树动态引用 final {
    稳定编码 值{};
    概念树动态引用() = default;
    explicit 概念树动态引用(稳定编码 id) : 值(id) {
    }
    friend bool operator==(const 概念树动态引用 &, const 概念树动态引用 &) = default;
};
using 概念树世界引用 = std::variant<概念树存在引用, 概念树特征引用, 概念树动态引用>;
using 概念树形成世界引用 =
    std::variant<概念树存在引用, 概念树特征引用, 概念树特征类型引用, 概念树场景引用, 概念树动态引用>;
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

struct 概念树动态时间槽 final {
    std::int64_t 相对纳秒 = 0;
    std::vector<概念树概念身份> 模板组;
    friend bool operator==(const 概念树动态时间槽 &, const 概念树动态时间槽 &) = default;
};
struct 概念树动态定义 final {
    概念树场景引用 场景;
    概念树存在引用 主体;
    std::vector<概念树动态时间槽> 时间槽组;
    friend bool operator==(const 概念树动态定义 &, const 概念树动态定义 &) = default;
};
using 概念树定义 = std::variant<概念树特征定义, 概念树存在定义, 概念树动态定义>;

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
    std::uint64_t 最大动态槽数 = 0;
    std::uint64_t 最大动态模板数 = 0;
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
    已可能发布,
    已创建共享定义 = 26, 已绑定共享名称, 已退出共享名称, 已记录共享用途, 不支持, 差异不可表示,
    已登记类型观察 = 32, 已登记特征概念命中 = 33, 已变更特征概念组织 = 34,
    旧格式不支持 = 35, 规则缺失 = 36, 类型不相容 = 37, 前次写入待收敛 = 38
};
enum class 概念树发布状态 : std::uint8_t {
    未派发 = 1,
    已确认发布,
    可能已发布,
    已确认未发布 = 4
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

struct 概念树动态结构类型 final {
    稳定编码 定义场景{}, 定义主体{}, 时间槽{}, 槽模板{}, 相对时间{}, 来源动态{}, 支持动态{}, 来源下位概念{};
    friend bool operator==(const 概念树动态结构类型 &, const 概念树动态结构类型 &) = default;
};
struct 概念树动态扩展交付 final {
    稳定编码 锚点{};
    概念树规则身份 签名规则;
    概念树动态结构类型 类型;
    friend bool operator==(const 概念树动态扩展交付 &, const 概念树动态扩展交付 &) = default;
};
enum class 概念树动态证据状态 : std::uint8_t {
    已核验 = 1,
    历史已清理 = 2,
    未展开 = 3
};
struct 概念树动态依据项 final {
    std::variant<概念树动态引用, 概念树概念身份> 目标;
    std::uint64_t H = 0;
    friend bool operator==(const 概念树动态依据项 &, const 概念树动态依据项 &) = default;
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

struct 概念树动态依据事实 final {
    稳定编码 记录{}, 成员关系{}, 引用关系{}, 截止值事实{};
    概念树动态依据项 依据;
    概念树生命周期 生命周期;
    bool 当前引用已释放 = false;
    friend bool operator==(const 概念树动态依据事实 &, const 概念树动态依据事实 &) = default;
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
    std::vector<概念树动态依据事实> 动态依据组;
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
    std::optional<概念树动态证据状态> 动态证据;
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

struct 概念树动态建立请求 final {
    概念树写入头 头;
    概念树动态定义 定义;
    std::vector<概念树概念身份> 直接上位;
    std::vector<概念树动态依据项> 依据;
    概念树预算 预算;
    friend bool operator==(const 概念树动态建立请求 &, const 概念树动态建立请求 &) = default;
};
struct 概念树动态依据释放请求 final {
    概念树写入头 头;
    概念树概念身份 所属概念;
    std::vector<概念树动态依据事实> 预期当前依据;
    概念树预算 预算;
    friend bool operator==(const 概念树动态依据释放请求 &, const 概念树动态依据释放请求 &) = default;
};
struct 概念树动态依据审计请求 final {
    概念树读取头 头;
    概念树概念身份 概念;
    概念树预算 预算;
};
struct 概念树动态依据审计项 final {
    概念树动态依据事实 依据;
    概念树动态证据状态 证据状态 = 概念树动态证据状态::未展开;
};
struct 概念树动态依据审计结果 final {
    std::uint32_t 合同版本 = 1;
    概念树数据状态 状态 = 概念树数据状态::入口拒绝;
    std::uint64_t Gread = 0, H = 0;
    std::vector<概念树动态依据审计项> 依据组;
    bool 成功() const noexcept;
};
struct 概念树动态支持核验请求 final {
    概念树读取头 读取头;
    稳定编码 支持记录{};
    概念树预算 预算;
};
struct 概念树动态支持核验结果 final {
    std::uint32_t 合同版本 = 1;
    概念树数据状态 状态 = 概念树数据状态::入口拒绝;
    std::uint64_t Gread = 0, H = 0;
    std::optional<概念树支持事实> 支持;
    bool 成功() const noexcept;
};
struct 概念树截止结果 final {
    std::uint32_t 合同版本 = 1;
    概念树数据状态 状态 = 概念树数据状态::入口拒绝;
    std::uint64_t Gread = 0;
    bool 成功() const noexcept {
        return 合同版本 == 1 && 状态 == 概念树数据状态::已读取 && Gread;
    }
};
struct 概念树动态模板读取请求 final {
    概念树读取头 头;
    概念树概念身份 模板;
    概念树预算 预算;
};
struct 概念树动态模板事实 final {
    概念树概念身份 模板, 本体根;
    概念树规则身份 规则;
    稳定编码 定义记录{}, 生命周期值事实{};
    概念树特征定义 定义;
    概念树生命周期 生命周期;
    概念树生命周期状态 治理状态 = 概念树生命周期状态::活跃;
    friend bool operator==(const 概念树动态模板事实 &, const 概念树动态模板事实 &) = default;
};
struct 概念树动态模板读取结果 final {
    std::uint32_t 合同版本 = 1;
    概念树数据状态 状态 = 概念树数据状态::入口拒绝;
    std::uint64_t Gread = 0, H = 0;
    std::optional<概念树动态模板事实> 模板;
    bool 成功() const noexcept;
};
using 概念树业务写请求 = std::variant<概念树概念建立请求, 概念树上位操作请求, 概念树支持建立请求,
                                      概念树支持退出请求, 概念树生命周期迁移请求, 概念树概念退出请求,
                                      概念树形成引用释放请求, 概念树动态建立请求, 概念树动态依据释放请求>;

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

struct 概念树共享结构交付 final {
    稳定编码 锚点{};
    std::uint32_t 格式 = 2;
    std::array<稳定编码, 21> 类型{};
    friend bool operator==(const 概念树共享结构交付&, const 概念树共享结构交付&) = default;
};
struct 类型观察键 final {
    特征类型身份 类型; 稳定编码 来源; std::uint64_t 序号 = 0;
    friend bool operator==(const 类型观察键&, const 类型观察键&) = default;
};
struct 概念树存在扩展交付 final {
    稳定编码 抽象依据目标关系类型{};
    friend bool operator==(const 概念树存在扩展交付&, const 概念树存在扩展交付&) = default;
};
struct 类型观察输入 final {
    类型观察键 键; 特征信息身份 F; std::int64_t 时间 = 0;
    friend bool operator==(const 类型观察输入&, const 类型观察输入&) = default;
};
struct 类型观察事实 final {
    稳定编码 记录; 类型观察输入 输入; std::uint64_t 证据H = 0;
    friend bool operator==(const 类型观察事实&, const 类型观察事实&) = default;
};
struct 特征概念定义 final {
    特征类型身份 类型; 特征规范I64域 域;
    friend bool operator==(const 特征概念定义&, const 特征概念定义&) = default;
};
struct 特征概念区间事实 final {
    稳定编码 节点, 成员关系, 下界值事实, 上界值事实;
    特征I64闭区间 区间; 概念树生命周期 生命周期;
    friend bool operator==(const 特征概念区间事实&, const 特征概念区间事实&) = default;
};
struct 特征概念来源事实 final {
    稳定编码 记录, 所属关系, 目标关系, 截止值事实;
    std::variant<特征类型身份, 类型观察事实, 概念树概念身份> 来源;
    std::uint64_t 证据H = 0; 概念树生命周期 生命周期;
    bool 当前引用已释放 = false;
    friend bool operator==(const 特征概念来源事实&, const 特征概念来源事实&) = default;
};
struct 特征概念事实 final {
    概念树概念身份 身份; 特征概念定义 定义;
    概念树概念身份 本体根, 类型根; 概念树规则身份 规则;
    稳定编码 定义记录, 生命周期值事实, 类型关系;
    概念树生命周期 生命周期;
    概念树生命周期状态 治理状态 = 概念树生命周期状态::活跃;
    std::vector<概念树直接上位事实> 直接上位;
    std::vector<特征概念区间事实> 区间事实;
    std::vector<特征概念来源事实> 来源事实;
    friend bool operator==(const 特征概念事实&, const 特征概念事实&) = default;
};
struct 特征名称事实 final {
    稳定编码 关系; 特征信息身份 F; 概念树概念身份 C;
    概念树生命周期 生命周期;
    friend bool operator==(const 特征名称事实&, const 特征名称事实&) = default;
};
struct 特征概念父组变更 final {
    概念树概念身份 子; std::vector<概念树直接上位事实> 预期;
    std::vector<概念树概念身份> 新父组;
    friend bool operator==(const 特征概念父组变更&, const 特征概念父组变更&) = default;
};
struct 特征概念预算 final {
    概念树预算 基础;
    std::uint64_t 最大观察数 = 0, 最大区间数 = 0, 最大命中数 = 0, 最大名称数 = 0, 最大首次材料项数 = 0;
    friend bool operator==(const 特征概念预算&, const 特征概念预算&) = default;
};
struct 特征概念结构交付 final {
    稳定编码 锚点; std::uint32_t 格式 = 1; std::array<稳定编码, 24> 类型{};
    friend bool operator==(const 特征概念结构交付&, const 特征概念结构交付&) = default;
};
struct 概念树共享预算 final {
    概念树预算 基础; 特征概念预算 特征;
    std::uint64_t 最大原子数 = 0, 最大用途数 = 0, 最大名称数 = 0, 最大首次材料项数 = 0;
    friend bool operator==(const 概念树共享预算&, const 概念树共享预算&) = default;
};
struct 概念树共享引用事实 final {
    稳定编码 关系{}, 记录{}, 目标{}; 概念树概念身份 概念;
    std::uint32_t 角色 = 0; 概念树生命周期 生命周期;
    friend bool operator==(const 概念树共享引用事实&, const 概念树共享引用事实&) = default;
};
struct 概念树存在依据事实_v2 final {
    概念树概念身份 所属EC, 来源EC;
    稳定编码 记录{}, 成员关系{}, 引用关系{}, 截止值事实{};
    std::uint64_t 证据H = 0;
    概念树生命周期 记录生命周期, 成员生命周期, 引用生命周期;
    bool 当前引用已释放 = false;
    friend bool operator==(const 概念树存在依据事实_v2 &, const 概念树存在依据事实_v2 &) = default;
};
struct 概念树应用存在事实 final {
    概念树概念身份 概念, 本体根;
    概念树规则身份 规则;
    概念树存在定义 定义;
    稳定编码 定义记录{}, 生命周期值事实{};
    概念树生命周期状态 治理状态 = 概念树生命周期状态::活跃;
    概念树生命周期 生命周期;
    std::vector<概念树直接上位事实> 直接上位组;
    std::vector<概念树来源事实> 世界来源组;
    std::vector<概念树存在依据事实_v2> 抽象依据组;
    friend bool operator==(const 概念树应用存在事实 &, const 概念树应用存在事实 &) = default;
};
using 概念树应用定义事实 = std::variant<概念树概念事实, 特征概念事实, 概念树应用存在事实>;
using 概念树应用模板事实 = std::variant<概念树动态模板事实, 特征概念事实>;
struct 概念树应用图事实 final {
    概念树概念身份 根身份;
    std::vector<概念树应用定义事实> 节点组;
    std::vector<概念树直接上位事实> 直接边组;
    std::optional<std::vector<概念树支持事实>> 支持组;
};
template<class T> struct 概念树应用读取结果 final {
    std::uint32_t 版本 = 1;
    概念树数据状态 状态 = 概念树数据状态::入口拒绝;
    std::uint64_t Gread = 0, H = 0;
    std::optional<T> 数据;
    bool 成功() const noexcept {
        return 版本 == 1 && 状态 == 概念树数据状态::已读取 && H && H <= Gread && 数据.has_value();
    }
};
struct 概念树应用定义读取请求 final {
    std::uint32_t 版本 = 2;
    概念树读取头 读取头;
    概念树概念身份 概念;
    概念树共享预算 预算;
};
struct 概念树应用图读取请求 final {
    std::uint32_t 版本 = 2;
    概念树图读取请求 图请求;
    概念树共享预算 预算;
};
struct 概念树应用模板读取请求 final {
    std::uint32_t 版本 = 2;
    概念树读取头 读取头;
    概念树概念身份 模板;
    概念树共享预算 预算;
};
struct 概念树应用模板判定请求 final {
    std::uint32_t 版本 = 2;
    概念树读取头 读取头;
    概念树概念身份 模板;
    准确特征读取请求 实际;
    概念树共享预算 预算;
};
struct 概念树应用读取用量 final {
    std::uint64_t 概念数=0, 关系数=0, 来源数=0, 支持数=0, 世界成员数=0,
        特征属性数=0, 动态槽数=0, 动态模板数=0, 观察数=0, 区间数=0,
        命中数=0, 名称数=0, 用途数=0, 首次材料项数=0;
};
struct 特征概念域判定回执 final {
    std::uint64_t Gread=0, 模板H=0, 实际H=0;
    准确特征读取事实 实际; 特征规范I64域 域; bool 命中=false;
};
struct 概念树应用模板判定事实 final {
    概念树应用读取用量 用量; 概念树应用模板事实 模板;
    特征概念域判定回执 判定; bool 适用=false;
};
struct 类型观察登记请求 final {
    概念树写入头 写入头; 类型观察输入 输入; std::uint64_t 最大保留观察数=0;
    特征概念预算 预算;
    friend bool operator==(const 类型观察登记请求&,const 类型观察登记请求&)=default;
};
struct 类型观察读取请求 final {
    概念树读取头 读取头; std::variant<类型观察键,特征类型身份> 选择;
    特征概念预算 预算;
    friend bool operator==(const 类型观察读取请求&,const 类型观察读取请求&)=default;
};
struct 特征概念查找请求 final {
    概念树读取头 读取头; std::variant<特征概念定义,特征类型身份> 选择;
    特征概念预算 预算;
    friend bool operator==(const 特征概念查找请求&,const 特征概念查找请求&)=default;
};
struct 特征概念建立请求 final {
    概念树写入头 写入头; 特征概念定义 定义;
    std::variant<特征类型身份,稳定编码,std::vector<概念树概念身份>> 依据;
    std::vector<概念树概念身份> 直接上位; std::vector<特征概念父组变更> 子父替代;
    特征概念预算 预算;
    friend bool operator==(const 特征概念建立请求&,const 特征概念建立请求&)=default;
};
struct 特征名称按特征读取请求 final {
    std::uint32_t 版本 = 2;
    概念树读取头 读取头;
    特征信息身份 F{};
    概念树预算 预算;
    friend bool operator==(const 特征名称按特征读取请求&, const 特征名称按特征读取请求&) = default;
};
struct 特征名称按概念读取请求 final {
    std::uint32_t 版本 = 2;
    概念树读取头 读取头;
    概念树概念身份 C{};
    概念树预算 预算;
    friend bool operator==(const 特征名称按概念读取请求&, const 特征名称按概念读取请求&) = default;
};
struct 特征名称变更请求 final {
    概念树写入头 写入头; 特征信息身份 F;
    std::optional<特征名称事实> 预期; std::optional<概念树概念身份> 新名称;
    特征概念预算 预算;
    friend bool operator==(const 特征名称变更请求&,const 特征名称变更请求&)=default;
};
struct 特征概念命中请求 final {
    概念树写入头 写入头; 稳定编码 观察记录; std::vector<概念树概念身份> 完整命中组;
    特征概念预算 预算;
    friend bool operator==(const 特征概念命中请求&,const 特征概念命中请求&)=default;
};
struct 特征概念组织请求 final {
    概念树写入头 写入头; std::vector<特征概念父组变更> 变更组;
    特征概念预算 预算;
    friend bool operator==(const 特征概念组织请求&,const 特征概念组织请求&)=default;
};
using 特征概念写请求 = std::variant<类型观察登记请求,特征概念建立请求,特征名称变更请求,特征概念命中请求,特征概念组织请求>;
struct 特征概念命中事实 final {
    稳定编码 记录,观察记录; 概念树概念身份 C;
    friend bool operator==(const 特征概念命中事实&,const 特征概念命中事实&)=default;
};
struct 特征概念写入结果 final {
    std::uint32_t 版本=1; 概念树数据状态 状态=概念树数据状态::入口拒绝;
    std::uint64_t Gread=0; std::optional<std::uint64_t> 首次H;
    概念树发布状态 发布状态=概念树发布状态::未派发;
    std::optional<特征概念写请求> 原请求;
    std::optional<类型观察事实> 观察; std::optional<特征概念事实> 概念;
    std::optional<特征名称事实> 名称; std::vector<特征概念命中事实> 命中组;
    std::vector<概念树直接上位事实> 直接边;
    bool 成功() const noexcept;
};
struct 旧特征概念审计关系 final {
    稳定编码 身份,源,目标; std::int64_t 关联值=0; 概念树生命周期 生命周期;
};
struct 旧特征概念审计数值 final {
    稳定编码 身份,所属,属性类型;
    std::variant<std::int64_t,std::vector<std::int64_t>,std::vector<std::uint64_t>> 内容;
    概念树生命周期 生命周期;
};
struct 旧特征概念审计事实 final {
    概念树概念身份 C,本体根; 概念树规则身份 规则;
    稳定编码 定义记录; std::uint32_t 格式=0;
    概念树生命周期 生命周期; 概念树生命周期状态 治理状态=概念树生命周期状态::活跃;
    std::vector<旧特征概念审计关系> 关系记录;
    std::vector<旧特征概念审计数值> 数值记录;
};
struct 概念树共享用途事实 final {
    稳定编码 记录{},登记关系{}; std::uint64_t 业务标识=0,原H=0;
    概念树概念身份 概念; 特征信息身份 F; std::optional<稳定编码> 观察;
    std::uint32_t 用途角色=0; std::int64_t 时间=0;
    std::vector<概念树共享引用事实> 引用; 概念树生命周期 生命周期;
    friend bool operator==(const 概念树共享用途事实&,const 概念树共享用途事实&)=default;
};
struct 概念树共享使用请求 final {
    std::uint32_t 版本=2; 概念树写入头 写入头; std::uint64_t 业务标识=0;
    概念树概念身份 概念; 准确特征读取请求 实际; std::optional<稳定编码> 观察;
    std::uint64_t 原H=0; std::int64_t 时间=0; std::uint32_t 用途角色=0;
    概念树共享预算 预算;
    friend bool operator==(const 概念树共享使用请求&,const 概念树共享使用请求&)=default;
};
struct 概念树共享使用读取请求 final {
    std::uint32_t 版本=2; 概念树读取头 读取头; 概念树概念身份 概念; 概念树共享预算 预算;
};
struct 概念树共享上位请求 final {
    std::uint32_t 版本=2; 概念树写入头 写入头; 概念树概念身份 下位;
    std::vector<概念树直接上位事实> 预期父组; std::vector<概念树概念身份> 新父组;
    概念树共享预算 预算;
    friend bool operator==(const 概念树共享上位请求&,const 概念树共享上位请求&)=default;
};
struct 概念树共享生命周期请求 final {
    std::uint32_t 版本=2; 概念树生命周期迁移请求 操作;
    std::optional<std::variant<类型观察事实,概念树共享用途事实>> 重新采用见证;
    概念树共享预算 预算;
    friend bool operator==(const 概念树共享生命周期请求&,const 概念树共享生命周期请求&)=default;
};
struct 概念树共享子父替代 final {
    概念树概念身份 子节点; std::vector<概念树直接上位事实> 预期父组;
    std::vector<概念树概念身份> 新父组;
    friend bool operator==(const 概念树共享子父替代&,const 概念树共享子父替代&)=default;
};
struct 概念树共享退出请求 final {
    std::uint32_t 版本=2; 概念树概念退出请求 操作;
    std::vector<概念树共享子父替代> 完整替代; 概念树共享预算 预算;
    friend bool operator==(const 概念树共享退出请求&,const 概念树共享退出请求&)=default;
};
struct 概念树共享释放请求 final {
    std::uint32_t 版本=2; 概念树写入头 写入头;
    std::vector<概念树共享引用事实> 完整预期; 概念树共享预算 预算;
    friend bool operator==(const 概念树共享释放请求&,const 概念树共享释放请求&)=default;
};
using 概念树共享写请求 = std::variant<概念树共享使用请求,概念树共享上位请求,
    概念树共享生命周期请求,概念树共享退出请求,概念树共享释放请求>;
struct 概念树存在依据项_v2 final {
    std::variant<概念树存在引用, 概念树概念身份> 来源;
    std::uint64_t H = 0;
    friend bool operator==(const 概念树存在依据项_v2 &, const 概念树存在依据项_v2 &) = default;
};
struct 概念树存在建立请求_v2 final {
    std::uint32_t 版本 = 2;
    概念树写入头 写入头;
    概念树存在定义 定义;
    std::vector<概念树概念身份> 直接上位;
    概念树存在依据项_v2 依据;
    概念树共享预算 预算;
    friend bool operator==(const 概念树存在建立请求_v2 &, const 概念树存在建立请求_v2 &) = default;
};
struct 概念树存在依据释放请求_v2 final {
    std::uint32_t 版本 = 2;
    概念树写入头 写入头;
    概念树概念身份 所属EC;
    std::vector<概念树存在依据事实_v2> 预期;
    概念树共享预算 预算;
    friend bool operator==(const 概念树存在依据释放请求_v2 &, const 概念树存在依据释放请求_v2 &) = default;
};
struct 概念树存在治理请求_v2 final {
    std::uint32_t 版本 = 2;
    std::variant<概念树上位操作请求, 概念树生命周期迁移请求, 概念树概念退出请求> 操作;
    概念树共享预算 预算;
    friend bool operator==(const 概念树存在治理请求_v2 &, const 概念树存在治理请求_v2 &) = default;
};
using 概念树存在写请求_v2 = std::variant<概念树存在建立请求_v2, 概念树存在依据释放请求_v2,
                                      概念树存在治理请求_v2>;
struct 概念树存在写入结果_v2 final {
    std::uint32_t 版本 = 2;
    概念树数据状态 状态 = 概念树数据状态::入口拒绝;
    std::uint64_t Gread = 0;
    概念树发布状态 发布状态 = 概念树发布状态::未派发;
    std::optional<std::uint64_t> 首次H;
    std::optional<概念树存在写请求_v2> 原请求;
    std::optional<概念树应用存在事实> 概念;
    std::vector<概念树直接上位事实> 关系组;
    std::vector<稳定编码> 已退出事实;
    bool 成功() const noexcept;
};
using 概念树应用业务操作 = std::variant<概念树动态建立请求, 概念树支持建立请求, 概念树支持退出请求,
    概念树上位操作请求, 概念树生命周期迁移请求, 概念树概念退出请求, 概念树形成引用释放请求,
    概念树动态依据释放请求>;
struct 概念树应用写请求 final {
    std::uint32_t 版本 = 1;
    概念树应用业务操作 操作;
    概念树共享预算 预算;
    friend bool operator==(const 概念树应用写请求 &, const 概念树应用写请求 &) = default;
};
struct 概念树应用写入结果 final {
    std::uint32_t 版本 = 1;
    概念树数据状态 状态 = 概念树数据状态::入口拒绝;
    std::uint64_t Gread = 0;
    概念树发布状态 发布状态 = 概念树发布状态::未派发;
    std::optional<std::uint64_t> 首次H;
    std::optional<概念树应用写请求> 原请求;
    std::optional<概念树应用定义事实> 概念;
    std::optional<概念树支持事实> 支持;
    std::vector<概念树直接上位事实> 关系组;
    std::vector<稳定编码> 已退出事实;
    std::vector<概念树应用定义事实> 释放所属概念组;
    bool 成功() const noexcept;
};
using 概念树应用关联操作 = std::variant<概念树按概念支持读取请求, 概念树按世界支持读取请求,
    概念树形成引用读取请求, 概念树动态依据审计请求, 概念树动态支持核验请求>;
struct 概念树应用关联请求 final {
    std::uint32_t 版本 = 1;
    概念树应用关联操作 操作;
    概念树共享预算 预算;
};
using 概念树应用关联事实 = std::variant<std::vector<概念树支持事实>, std::vector<概念树形成引用事实>,
    std::vector<概念树动态依据审计项>, 概念树支持事实>;
struct 概念树共享写入结果 final {
    std::uint32_t 版本 = 2;
    概念树数据状态 状态 = 概念树数据状态::入口拒绝;
    std::uint64_t Gread = 0;
    std::optional<std::uint64_t> 首次H;
    概念树发布状态 发布状态 = 概念树发布状态::未派发;
    std::optional<概念树共享写请求> 原请求;
    std::vector<稳定编码> 已退出编码;
    std::optional<特征概念事实> 概念;
    std::optional<概念树共享用途事实> 用途;
    std::vector<概念树直接上位事实> 直接边;
    bool 成功() const noexcept;
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
    friend struct 概念树存在写入结果_v2;
    friend struct 概念树应用写入结果;
    friend struct 概念树动态模板读取结果;
    friend struct 概念树动态支持核验结果;
    friend struct 概念树动态依据审计结果;

  public:
    概念树类数据服务(const L1事实基座服务 &, const 特征类数据服务 &, const 存在类数据服务 &,
                     const 特征值类数据服务 &, L1所有者范围写端口 &&, const 概念树结构交付 &,
                     const 概念树共享结构交付 &);
    概念树类数据服务(const L1事实基座服务 &, const 特征类数据服务 &, const 存在类数据服务 &,
                     const 特征值类数据服务 &, L1所有者范围写端口 &&, const 概念树结构交付 &,
                     const 场景类数据服务 &, const 动态类数据服务 &, const 概念树动态扩展交付 &,
                     const 概念树共享结构交付 &);
    bool 使用特征存在服务(const 特征类数据服务 &f, const 存在类数据服务 &e) const noexcept {
        return &features_ == &f && &existences_ == &e;
    }
    bool 使用特征服务(const 特征类数据服务& f) const noexcept { return &features_==&f; }
    bool 共享应用已启用() const noexcept { return feature_layout_.has_value(); }
    概念树类数据服务(const L1事实基座服务&, const 特征类数据服务&, const 存在类数据服务&,
        const 特征值类数据服务&, L1所有者范围写端口&&, const 概念树结构交付&, const 特征概念结构交付&);
    概念树类数据服务(const L1事实基座服务&, const 特征类数据服务&, const 存在类数据服务&,
        const 特征值类数据服务&, L1所有者范围写端口&&, const 概念树结构交付&,
        const 场景类数据服务&, const 动态类数据服务&, const 概念树动态扩展交付&, const 特征概念结构交付&);
    概念树类数据服务(const L1事实基座服务&, const 特征类数据服务&, const 存在类数据服务&,
        const 特征值类数据服务&, L1所有者范围写端口&&, const 概念树结构交付&,
        const 特征概念结构交付&, const 概念树存在扩展交付&);
    概念树类数据服务(const L1事实基座服务&, const 特征类数据服务&, const 存在类数据服务&,
        const 特征值类数据服务&, L1所有者范围写端口&&, const 概念树结构交付&,
        const 场景类数据服务&, const 动态类数据服务&, const 概念树动态扩展交付&,
        const 特征概念结构交付&, const 概念树存在扩展交付&);
    bool 使用同一场景基座(const 场景类数据服务 &s) const noexcept { return s.绑定于(l1_); }
    概念树概念身份 特征根引用() const noexcept { return layout_.根组.特征根; }
    概念树概念身份 存在根引用() const noexcept { return layout_.根组.存在根; }
    概念树应用读取结果<概念树应用定义事实> 读取应用概念定义(const 概念树应用定义读取请求 &) const;
    概念树应用读取结果<概念树应用图事实> 读取应用概念图(const 概念树应用图读取请求 &) const;
    概念树应用读取结果<概念树应用模板事实> 读取应用特征模板(const 概念树应用模板读取请求 &) const;
    概念树应用读取结果<概念树应用模板判定事实> 判定应用特征模板(const 概念树应用模板判定请求 &) const;
    特征概念写入结果 登记类型观察(const 类型观察登记请求& r);
    概念树应用读取结果<std::vector<类型观察事实>> 读取类型观察(const 类型观察读取请求&) const;
    概念树应用读取结果<std::vector<特征概念事实>> 查找特征概念(const 特征概念查找请求&) const;
    特征概念写入结果 建立特征概念(const 特征概念建立请求& r);
    概念树应用读取结果<std::optional<特征名称事实>> 读取特征当前名称(const 特征名称按特征读取请求&) const;
    概念树应用读取结果<std::vector<特征名称事实>> 读取概念当前名称(const 特征名称按概念读取请求&) const;
    特征概念写入结果 变更特征名称(const 特征名称变更请求& r);
    特征概念写入结果 登记特征概念命中(const 特征概念命中请求& r);
    特征概念写入结果 变更特征概念组织(const 特征概念组织请求& r);
    概念树应用读取结果<概念树写入头> 取得特征概念写入头(const 特征概念预算&) const;
    特征概念写入结果 收敛特征概念写入(const 特征概念写请求&);
    概念树应用读取结果<旧特征概念审计事实> 读取旧特征概念审计(const 概念树应用定义读取请求&) const;
    概念树应用读取结果<std::vector<概念树共享用途事实>> 读取共享概念使用(const 概念树共享使用读取请求&) const;
    概念树共享写入结果 记录共享概念使用(const 概念树共享使用请求 &r);
    概念树共享写入结果 操作共享概念上位(const 概念树共享上位请求 &r);
    概念树共享写入结果 迁移共享概念生命周期(const 概念树共享生命周期请求 &r);
    概念树共享写入结果 退出共享特征概念(const 概念树共享退出请求 &r);
    概念树存在写入结果_v2 创建存在概念(const 概念树存在建立请求_v2 &);
    概念树存在写入结果_v2 释放存在抽象依据(const 概念树存在依据释放请求_v2 &);
    概念树存在写入结果_v2 治理存在概念(const 概念树存在治理请求_v2 &);
    概念树应用写入结果 执行应用概念写入(const 概念树应用写请求 &);
    概念树应用读取结果<概念树应用关联事实> 读取应用概念关联(const 概念树应用关联请求 &) const;
    概念树共享写入结果 释放共享概念引用(const 概念树共享释放请求 &r);
    概念树类数据服务(const L1事实基座服务 &, const 特征类数据服务 &, const 存在类数据服务 &,
                     const 特征值类数据服务 &, L1所有者范围写端口 &&, const 概念树结构交付 &);

    概念树类数据服务(const L1事实基座服务 &, const 特征类数据服务 &, const 存在类数据服务 &,
                     const 特征值类数据服务 &, L1所有者范围写端口 &&, const 概念树结构交付 &,
                     const 场景类数据服务 &, const 动态类数据服务 &, const 概念树动态扩展交付 &);
    bool 使用动态世界服务(const 场景类数据服务 &s, const 存在类数据服务 &e,
                          const 动态类数据服务 &d) const noexcept {
        return dynamic_ && scenes_ == &s && dynamics_ == &d && &existences_ == &e;
    }
    概念树概念身份 动态根引用() const noexcept {
        return layout_.根组.动态根;
    }
    概念树截止结果 读取当前事实代次() const;
    概念树写入结果 创建动态概念(const 概念树动态建立请求 &r) {
        return 执行写入(r);
    }
    概念树写入结果 释放动态形成依据(const 概念树动态依据释放请求 &r) {
        return 执行写入(r);
    }
    概念树动态依据审计结果 读取动态形成依据(const 概念树动态依据审计请求 &) const;
    概念树动态支持核验结果 核验动态支持依据(const 概念树动态支持核验请求 &) const;
    概念树动态模板读取结果 读取动态特征模板(const 概念树动态模板读取请求 &) const;
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

    const 场景类数据服务 *scenes_ = nullptr;
    const 动态类数据服务 *dynamics_ = nullptr;
    std::optional<概念树动态扩展交付> dynamic_;
    std::optional<概念树共享结构交付> shared_;
    void 校验共享扩展(std::uint64_t) const;
    std::optional<特征概念结构交付> feature_layout_;
    std::optional<概念树存在扩展交付> existence_layout_;
    void 校验存在扩展(std::uint64_t) const;
    std::optional<特征概念写请求> feature_pending_;
    std::optional<概念树共享写请求> shared_pending_;
    std::optional<概念树应用写请求> application_pending_;
    void 校验特征扩展(std::uint64_t) const;
    static void 特征预算有效(const 特征概念预算&);
    static 概念树共享预算 组合共享预算(const 特征概念预算&);
    static void 共享预算有效(const 概念树共享预算&);
    static S 共享映射(特征数据错误);
    template<class T> static T 特征结果(特征数据结果<T> r) {
        if (auto* e = std::get_if<特征数据错误>(&r)) throw 失败{共享映射(*e)};
        return std::get<T>(std::move(r));
    }
    概念树共享写入结果 共享执行(const 概念树共享写请求 &);
    static 概念树写入头 共享请求头(const 概念树共享写请求 &);
    概念树共享写请求 规范共享请求(const 概念树共享写请求 &) const;
    struct 共享写集构造 {
        L1所有者范围写集请求 写集;
        Key 节点() {
            if (写集.节点.size() >= 0xFFFF) throw 失败{S::数量预算不足};
            const Key key{static_cast<std::uint32_t>(写集.节点.size() + 1)};
            写集.节点.push_back({key, 节点种类::普通, std::nullopt});
            return key;
        }
        Key 关系(Ref source, Ref target, 稳定编码 type, std::int64_t order = 1) {
            if (写集.关系.size() >= 0xFFFF) throw 失败{S::数量预算不足};
            const Key key{static_cast<std::uint32_t>(0x10000 + 写集.关系.size())};
            写集.关系.push_back({key, source, target, Ref{type}, order});
            return key;
        }
        Key 值(Ref node, 稳定编码 type, L1所有者范围原始值材料 value) {
            if (写集.值.size() >= 0xFFFF) throw 失败{S::数量预算不足};
            const Key key{static_cast<std::uint32_t>(0x20000 + 写集.值.size())};
            写集.值.push_back({key, node, Ref{type}, std::move(value), node});
            写集.属性槽变更.push_back({node, Ref{type}, key});
            return key;
        }
        void 规范() {
            auto less = [](const Ref &a, const Ref &b) {
                if (a.index() != b.index()) return a.index() < b.index();
                return std::visit([](const auto &v) { return static_cast<std::uint64_t>(v.值); }, a) <
                       std::visit([](const auto &v) { return static_cast<std::uint64_t>(v.值); }, b);
            };
            std::sort(写集.属性槽变更.begin(), 写集.属性槽变更.end(), [&](const auto &a, const auto &b) {
                return a.所属节点 != b.所属节点 ? less(a.所属节点, b.所属节点) : less(a.属性类型节点, b.属性类型节点);
            });
            std::sort(写集.退出事实.begin(), 写集.退出事实.end(), 编码小于);
            写集.退出事实.erase(std::unique(写集.退出事实.begin(), 写集.退出事实.end()), 写集.退出事实.end());
        }
    };
    struct 动态历史用量 {
        std::uint64_t 状态数 = 0, 变化数 = 0, 快照数 = 0;
        std::set<std::pair<std::uint64_t, std::uint64_t>> 已核验;
    };
    struct 共享读取上下文 {
        std::uint64_t G = 0, H = 0;
        概念树共享预算 预算;
        std::uint64_t 来源数 = 0, 关系数 = 0, 首次材料数 = 0;
        std::uint64_t 特征记录扫描数 = 0;
        概念树应用读取用量 用量;
        std::uint64_t 动态槽数 = 0, 动态模板数 = 0;
        动态历史用量 动态用量;
        std::set<std::pair<std::uint64_t,std::uint64_t>> 观察计数, 区间计数, 命中计数, 名称计数, 来源计数;
        std::set<std::pair<std::uint64_t,std::uint64_t>> 定义路径;
        std::set<std::pair<std::uint64_t,std::uint64_t>> 关系计数, 概念计数, 特征计数, 用途计数;
    };
    struct 共享展开 {
        特征概念事实 事实;
        std::vector<稳定编码> 自有事实;
    };
    static bool 生命周期迁移有效(概念树生命周期状态 from, 概念树生命周期状态 to) noexcept {
        using L = 概念树生命周期状态;
        return (from == L::活跃 && to == L::冷却)
            || (from == L::冷却 && (to == L::活跃 || to == L::退役))
            || (from == L::退役 && to == L::活跃);
    }
    特征概念定义 规范共享定义(const 特征概念定义&, 共享读取上下文&) const;
    bool 共享包含(const 特征概念定义&, const 特征概念定义&, 共享读取上下文&) const;
    static void 专项计数(std::set<std::pair<std::uint64_t,std::uint64_t>>&,
        稳定编码, std::uint64_t, std::uint64_t);
    类型观察事实 观察事实(稳定编码, 共享读取上下文&) const;
    void 核验外部节点(稳定编码,std::uint64_t,std::uint64_t) const;
    std::vector<类型观察事实> 观察组(特征类型身份, 共享读取上下文&) const;
    std::vector<特征概念命中事实> 命中组(共享读取上下文&) const;
    std::vector<特征名称事实> 名称组(std::variant<特征信息身份,概念树概念身份>,
        共享读取上下文&) const;
    std::vector<特征名称事实> 名称窄读(稳定编码, bool, std::uint64_t, std::uint64_t,
        const 概念树预算&) const;
    struct 特征准备结果 {
        共享写集构造 构造;
        特征概念写入结果 复用;
    };
    特征准备结果 准备特征写入(const 特征概念写请求&, 共享读取上下文&) const;
    特征概念写入结果 特征发布读回(const 特征概念写请求&, const L1所有者范围写集请求&,
        const L1所有者范围写入结果&, 共享读取上下文&) const;
    特征概念写入结果 特征执行(const 特征概念写请求&);
    void 准备父组(const std::vector<特征概念父组变更>&, 共享写集构造&,
        共享读取上下文&, const std::optional<std::pair<Ref,特征概念定义>>& = std::nullopt) const;
    struct 存在展开_v2 {
        概念树应用存在事实 事实;
        std::vector<稳定编码> 自有事实;
    };
    struct 共享截止作用域 {
        共享读取上下文 &上下文;
        std::uint64_t 原H;
        共享截止作用域(共享读取上下文 &c, std::uint64_t h) : 上下文(c), 原H(c.H) { c.H = h; }
        ~共享截止作用域() { 上下文.H = 原H; }
    };
    存在展开_v2 展开存在定义_v2(概念树概念身份, 共享读取上下文 &) const;
    struct 存在准备结果_v2 {
        共享写集构造 构造;
        std::optional<概念树应用存在事实> 复用;
    };
    static 概念树写入头 存在请求头(const 概念树存在写请求_v2 &);
    概念树存在写请求_v2 规范存在请求(const 概念树存在写请求_v2 &) const;
    存在准备结果_v2 准备存在写入(const 概念树存在写请求_v2 &, 共享读取上下文 &) const;
    概念树存在写入结果_v2 存在发布读回(const 概念树存在写请求_v2 &, const L1所有者范围写集请求 &,
        const L1所有者范围写入结果 &, 共享读取上下文 &) const;
    概念树存在写入结果_v2 存在执行(const 概念树存在写请求_v2 &);
    std::vector<E> 共享关系(稳定编码, 稳定编码, bool, std::uint64_t, 共享读取上下文 &) const;
    共享展开 展开共享定义(概念树概念身份, 共享读取上下文 &) const;
    概念树应用定义事实 应用定义(概念树概念身份, 共享读取上下文 &) const;
    概念树应用图事实 应用图(概念树根角色, bool, 共享读取上下文 &) const;
    概念树应用模板事实 应用模板(概念树概念身份, 共享读取上下文 &) const;
    概念树应用模板判定事实 应用判定(概念树概念身份, const 准确特征读取请求&, 共享读取上下文&) const;
    std::vector<特征名称事实> 共享名称组(共享读取上下文&) const;
    void 共享名称交叉核验(稳定编码, bool, const std::vector<特征名称事实>&, 共享读取上下文&) const;
    概念树共享用途事实 共享用途(稳定编码, 共享读取上下文 &) const;
    std::vector<概念树共享用途事实> 共享用途组(共享读取上下文 &) const;
    void 扣首次材料(共享读取上下文 &c, std::uint64_t n) const {
        if (c.首次材料数 > c.预算.最大首次材料项数 || n > c.预算.最大首次材料项数 - c.首次材料数)
            throw 失败{S::数量预算不足};
        c.首次材料数 += n;
    }
    std::map<std::uint32_t, 稳定编码> 共享核验首次(const L1所有者范围写集请求 &,
        const L1所有者范围写入结果 &, 共享读取上下文 &) const;
    struct 共享准备结果 {
        共享写集构造 构造;
        std::optional<特征概念事实> 复用概念;
        std::optional<概念树共享用途事实> 复用用途;
    };
    共享准备结果 准备共享用途(const 概念树共享使用请求 &, 共享读取上下文 &) const;
    共享准备结果 准备共享写入(const 概念树共享写请求 &, 共享读取上下文 &) const;
    共享准备结果 准备共享治理(const 概念树共享写请求 &, 共享读取上下文 &) const;
    void 核验共享释放读回(const 概念树共享释放请求&, std::uint64_t, 共享读取上下文&) const;
    概念树共享写入结果 共享发布读回(const 概念树共享写请求 &, const L1所有者范围写集请求 &,
        const L1所有者范围写入结果 &, 共享读取上下文 &) const;
    template<class T, class F> 概念树应用读取结果<T> 共享读取封装(
        const 概念树读取头 &head, const 概念树共享预算 &budget, F &&action) const {
        概念树应用读取结果<T> out;
        out.Gread = head.Gread;
        out.H = head.H;
        try {
            if(!head.H)throw 失败{S::入口拒绝};
            共享预算有效(budget);
            共享读取上下文 c{head.Gread, 读取截止(head), budget};
            守卫代次(c.G);
            auto data = action(c);
            守卫代次(c.G);
            out.数据 = std::move(data);
            out.状态 = S::已读取;
        } catch (const 失败 &x) {
            out.状态 = x.状态;
            try { 守卫代次(head.Gread); }
            catch (const 失败 &late) { out.状态 = late.状态; }
            catch (const std::bad_alloc &) { out.状态 = S::资源失败; }
            catch (...) { out.状态 = S::内部不一致; }
        } catch (const std::bad_alloc &) { out.状态 = S::资源失败; }
        catch (const std::length_error &) { out.状态 = S::资源失败; }
        catch (...) { out.状态 = S::内部不一致; }
        if (out.状态 != S::已读取) out.数据.reset();
        return out;
    }
    void 校验动态扩展(std::uint64_t) const;
    void 验证动态定义(const 概念树动态定义 &, std::uint64_t, std::uint64_t, const 概念树预算 &, 共享读取上下文 * = nullptr) const;
    void 验证动态依据(const 概念树动态依据项 &, std::uint64_t, const 概念树预算 &,
                      动态历史用量 * = nullptr, 共享读取上下文 * = nullptr) const;
    概念树动态证据状态 审计动态(概念树动态引用, std::uint64_t, std::uint64_t, const 概念树预算 &,
                                bool 当前保护 = false, 动态历史用量 * = nullptr) const;
    概念树动态模板事实 动态模板(概念树概念身份, std::uint64_t, std::uint64_t, const 概念树预算 &) const;
    static bool 动态定义完整(const 概念树动态定义 &) noexcept;
    static bool 动态依据完整(const 概念树动态依据事实 &, std::uint64_t, bool = false) noexcept;
    稳定编码 支持关系类型(const 概念树世界引用 &) const;
    static S 映射(动态类数据状态);
    static S 映射(场景类数据状态);
    template <class T> static const 概念树写入头 &请求写头(const T &r) {
        if constexpr (std::is_same_v<T, 概念树动态建立请求> || std::is_same_v<T, 概念树动态依据释放请求>)
            return r.头;
        else
            return r.写入头;
    }
    mutable std::mutex mutex_;

    void 校验布局(std::uint64_t Gread) const;
    void 守卫代次(std::uint64_t Gread) const;
    概念树定义 规范化定义(const 概念树定义 &) const;
    概念树概念事实 读取概念内部(概念树概念身份, std::uint64_t Gread, std::uint64_t H,
                                const 概念树预算 &, 共享读取上下文 * = nullptr) const;
    L1所有者范围写集请求 形成写集(const 概念树业务写请求 &, std::uint64_t Gread, std::uint64_t H, 共享读取上下文 * = nullptr) const;
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
    概念展开 展开概念(概念树概念身份, std::uint64_t g, std::uint64_t h, const 概念树预算 &,
                      bool 窄模板 = false, bool 应用结构 = false, 共享读取上下文 * = nullptr) const;
    概念树图结果 图(概念树根角色, bool 支持, std::uint64_t g, std::uint64_t h, const 概念树预算 &, 共享读取上下文 * = nullptr) const;
    概念树支持事实 支持记录(稳定编码, std::uint64_t g, std::uint64_t h, const 概念树预算 &, 共享读取上下文 * = nullptr) const;
    std::vector<概念树支持事实> 支持组(std::optional<概念树概念身份>, std::optional<概念树世界引用>,
                                       std::uint64_t g, std::uint64_t h, const 概念树预算 &, 共享读取上下文 * = nullptr) const;
    std::vector<概念树形成引用事实> 形成引用组(const 概念树形成世界引用 &, std::uint64_t g, std::uint64_t h,
                                               const 概念树预算 &, 共享读取上下文 * = nullptr) const;
    void 验证世界(const 概念树形成世界引用 &, std::uint64_t g, std::uint64_t h, const 概念树预算 &) const;
    void 验证特征定义(const 概念树特征定义 &, std::uint64_t g, std::uint64_t h, const 概念树预算 &) const;
    概念树业务写请求 规范写请求(const 概念树业务写请求 &) const;
    概念树写入结果 发布结果(const 概念树业务写请求 &, const L1所有者范围写集请求 &,
                            const L1所有者范围写入结果 &, std::uint64_t g, 共享读取上下文 * = nullptr) const;
    static void 预算有效(const 概念树预算 &);
    static std::uint64_t 读取截止(const 概念树读取头 &);
    static void 检查数量(std::size_t n, std::uint64_t max);
    static S 映射(L1所有者范围读取状态);
    static S 映射(存在历史读取状态);
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
    static bool 应用事实完整(const 概念树应用定义事实 &, std::uint64_t) noexcept;
    static 概念树概念身份 应用身份(const 概念树应用定义事实& f) {
        return std::visit([](const auto& v) {
            if constexpr(std::is_same_v<std::decay_t<decltype(v)>,特征概念事实>)return v.身份;
            else return v.概念;
        },f);
    }
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
概念树数据状态 概念树类数据服务::映射(存在历史读取状态 s) {
    switch (s) {
    case 存在历史读取状态::入口拒绝:
        return S::入口拒绝;
    case 存在历史读取状态::未找到:
        return S::未找到;
    case 存在历史读取状态::目标已退出:
        return S::目标已退出;
    case 存在历史读取状态::事实代次漂移:
        return S::事实代次漂移;
    case 存在历史读取状态::资源失败:
        return S::资源失败;
    case 存在历史读取状态::历史材料不可用:
        return S::历史材料不可用;
    case 存在历史读取状态::数量预算不足:
        return S::数量预算不足;
    case 存在历史读取状态::引用冲突:
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

概念树类数据服务::概念树类数据服务(const L1事实基座服务 &l1, const 特征类数据服务 &f, const 存在类数据服务 &e,
                                   const 特征值类数据服务 &v, L1所有者范围写端口 &&port,
                                   const 概念树结构交付 &base, const 场景类数据服务 &scene,
                                   const 动态类数据服务 &dynamics, const 概念树动态扩展交付 &extension)
    : 概念树类数据服务(l1, f, e, v, std::move(port), base) {
    if (!scene.绑定于(l1) || !dynamics.绑定于(l1))
        throw std::invalid_argument("dynamic world L1 mismatch");
    scenes_ = &scene;
    dynamics_ = &dynamics;
    dynamic_ = extension;
    try {
        校验动态扩展(当前代次());
    } catch (const 失败 &x) {
        if (x.状态 == S::资源失败)
            throw std::bad_alloc{};
        throw std::invalid_argument("dynamic concept extension is invalid");
    }
}
void 概念树类数据服务::校验动态扩展(std::uint64_t g) const {
    if (!dynamic_ || !scenes_ || !dynamics_)
        throw 失败{S::入口拒绝};
    const auto &x = *dynamic_;
    const auto &t = x.类型;
    const std::array<稳定编码, 8> ids{t.定义场景, t.定义主体, t.时间槽,   t.槽模板,
                                      t.相对时间, t.来源动态, t.支持动态, t.来源下位概念};
    std::set<std::uint64_t> used{layout_.格式锚点.值, layout_.签名规则.值.值};
    for (auto id : 类型组())
        used.insert(id.值);
    for (auto id : 根组())
        used.insert(id.值.值);
    auto unique = [&](稳定编码 id) {
        if (!有效(id) || !used.insert(id.值).second)
            throw 失败{S::引用冲突};
    };
    unique(x.锚点);
    unique(x.签名规则.值);
    for (std::size_t i = 0; i < ids.size(); ++i) {
        unique(ids[i]);
        const auto n = 节点(ids[i], g, g);
        if (n.种类 != (i == 4 ? 节点种类::属性类型 : 节点种类::普通) ||
            n.属性类型表示 != (i == 4 ? std::optional{L1所有者范围值表示种类::I64} : std::nullopt) ||
            !属性(ids[i], g, g).empty())
            throw 失败{S::引用冲突};
    }
    for (auto id : {x.锚点, x.签名规则.值}) {
        const auto n = 节点(id, g, g);
        if (n.种类 != 节点种类::普通 || n.属性类型表示)
            throw 失败{S::引用冲突};
    }
    const auto a = 属性(x.锚点, g, g), rv = 属性(x.签名规则.值, g, g);
    if (a.size() != 1 || rv.size() != 2 ||
        std::get<std::int64_t>(唯一属性(a, layout_.类型.格式版本).材料) != 1 ||
        std::get<std::int64_t>(唯一属性(rv, layout_.类型.格式版本).材料) != 1 ||
        std::get<std::int64_t>(唯一属性(rv, layout_.类型.规则版本).材料) != 1 ||
        唯一关系(x.锚点, layout_.类型.规则归属, g, g).目标节点 != x.签名规则.值)
        throw 失败{S::引用冲突};
    const auto regs = 关系(x.锚点, layout_.类型.类型登记, false, g, g, 8);
    if (regs.size() != 8)
        throw 失败{S::引用冲突};
    for (std::size_t i = 0; i < 8; ++i)
        if (regs[i].角色或顺序 != i + 1 || regs[i].目标节点 != ids[i])
            throw 失败{S::引用冲突};
    守卫代次(g);
}
概念树截止结果 概念树类数据服务::读取当前事实代次() const {
    try {
        return {1, S::已读取, 当前代次()};
    } catch (const 失败 &x) {
        return {1, x.状态, 0};
    } catch (const std::bad_alloc &) {
        return {1, S::资源失败, 0};
    } catch (const std::length_error &) {
        return {1, S::资源失败, 0};
    } catch (...) {
        return {1, S::内部不一致, 0};
    }
}
概念树数据状态 概念树类数据服务::映射(动态类数据状态 s) {
    switch (s) {
    case 动态类数据状态::未找到:
        return S::未找到;
    case 动态类数据状态::目标已退出:
        return S::目标已退出;
    case 动态类数据状态::入口拒绝:
        return S::入口拒绝;
    case 动态类数据状态::事实代次漂移:
        return S::事实代次漂移;
    case 动态类数据状态::数量预算不足:
        return S::数量预算不足;
    case 动态类数据状态::资源失败:
        return S::资源失败;
    case 动态类数据状态::历史材料已清理:
        return S::历史材料不可用;
    case 动态类数据状态::引用冲突:
        return S::引用冲突;
    default:
        return S::内部不一致;
    }
}
概念树数据状态 概念树类数据服务::映射(场景类数据状态 s) {
    switch (s) {
    case 场景类数据状态::未找到:
        return S::未找到;
    case 场景类数据状态::目标已退出:
        return S::目标已退出;
    case 场景类数据状态::入口拒绝:
        return S::入口拒绝;
    case 场景类数据状态::事实代次漂移:
        return S::事实代次漂移;
    case 场景类数据状态::数量预算不足:
        return S::数量预算不足;
    case 场景类数据状态::资源失败:
        return S::资源失败;
    case 场景类数据状态::历史材料已清理:
        return S::历史材料不可用;
    case 场景类数据状态::引用冲突:
        return S::引用冲突;
    default:
        return S::内部不一致;
    }
}
bool 概念树类数据服务::动态定义完整(const 概念树动态定义 &d) noexcept {
    if (!有效(d.场景.值) || !有效(d.主体.值) || d.时间槽组.size() < 2 || d.时间槽组.front().相对纳秒 != 0)
        return false;
    for (std::size_t i = 0; i < d.时间槽组.size(); ++i) {
        const auto &q = d.时间槽组[i];
        if (q.相对纳秒 < 0 || q.模板组.empty() || (i && d.时间槽组[i - 1].相对纳秒 > q.相对纳秒))
            return false;
        for (std::size_t j = 0; j < q.模板组.size(); ++j)
            if (!有效(q.模板组[j].值) || (j && !身份小于(q.模板组[j - 1], q.模板组[j])))
                return false;
    }
    return true;
}
bool 概念树类数据服务::动态依据完整(const 概念树动态依据事实 &f, std::uint64_t h, bool exited) noexcept {
    return !f.依据.目标.valueless_by_exception() && 有效(f.记录) && 有效(f.成员关系) && 有效(f.引用关系) &&
           有效(f.截止值事实) && 有效(世界编码(f.依据.目标)) && f.依据.H &&
           f.依据.H < f.生命周期.创建事实代次 && 生命完整(f.生命周期, h, exited);
}
稳定编码 概念树类数据服务::支持关系类型(const 概念树世界引用 &w) const {
    if (w.valueless_by_exception())
        throw 失败{S::入口拒绝};
    if (w.index() == 0)
        return layout_.类型.支持存在;
    if (w.index() == 1)
        return layout_.类型.支持特征;
    if (!dynamic_)
        throw 失败{S::入口拒绝};
    return dynamic_->类型.支持动态;
}
概念树动态模板事实 概念树类数据服务::动态模板(概念树概念身份 id, std::uint64_t g, std::uint64_t h,
                                              const 概念树预算 &b) const {
    if (!dynamic_)
        throw 失败{S::入口拒绝};
    auto f = 展开概念(id, g, h, b, true).事实;
    if (f.是本体根 || f.根角色 != 概念树根角色::特征 || !f.定义 ||
        !std::holds_alternative<概念树特征定义>(*f.定义))
        throw 失败{S::引用冲突};
    return {f.概念,     f.本体根,  f.规则, f.定义记录, f.生命周期值事实, std::get<概念树特征定义>(*f.定义),
            f.生命周期, f.治理状态};
}
概念树动态模板读取结果 概念树类数据服务::读取动态特征模板(const 概念树动态模板读取请求 &r) const {
    return 读取封装<概念树动态模板读取结果>(r.头, r.预算, [&](auto g, auto h) {
        概念树动态模板读取结果 out;
        out.模板 = 动态模板(r.模板, g, h, r.预算);
        return out;
    });
}
void 概念树类数据服务::验证动态定义(const 概念树动态定义 &d, std::uint64_t g, std::uint64_t h,
                                    const 概念树预算 &b, 共享读取上下文 *shared) const {
    if (!dynamic_ || !动态定义完整(d) || !b.最大动态槽数 || !b.最大动态模板数)
        throw 失败{S::入口拒绝};
    检查数量(d.时间槽组.size(), b.最大动态槽数);
    验证世界(d.场景, g, h, b);
    验证世界(d.主体, g, h, b);
    std::uint64_t count = 0;
    for (const auto &q : d.时间槽组) {
        if (q.模板组.size() > b.最大动态模板数 - count)
            throw 失败{S::数量预算不足};
        count += q.模板组.size();
        for (auto id : q.模板组) {
            if (shared) {
                共享截止作用域 at(*shared, h);
                const auto f = 应用模板(id, *shared);
                if (const auto *old = std::get_if<概念树动态模板事实>(&f); old && old->定义.形成宿主 != d.主体)
                    throw 失败{S::引用冲突};
            } else if (动态模板(id, g, h, b).定义.形成宿主 != d.主体) throw 失败{S::引用冲突};
        }
    }
}
void 概念树类数据服务::验证动态依据(const 概念树动态依据项 &a, std::uint64_t g, const 概念树预算 &b,
                                    动态历史用量 *used, 共享读取上下文 *shared) const {
    if (a.目标.valueless_by_exception() || !a.H || a.H > g)
        throw 失败{S::入口拒绝};
    if (shared) used = &shared->动态用量;
    if (const auto *d = std::get_if<概念树动态引用>(&a.目标)) {
        if (审计动态(*d, g, a.H, b, false, used) != 概念树动态证据状态::已核验)
            throw 失败{S::历史材料不可用};
    } else {
        const auto id = std::get<概念树概念身份>(a.目标);
        const auto f = 读取概念内部(id, g, a.H, b, shared);
        if (f.是本体根 || f.根角色 != 概念树根角色::动态 || f.生命周期.创建事实代次 > a.H)
            throw 失败{S::引用冲突};
    }
}
概念树动态证据状态 概念树类数据服务::审计动态(概念树动态引用 d, std::uint64_t g, std::uint64_t h,
                                              const 概念树预算 &b, bool current, 动态历史用量 *used) const {
    if (!dynamic_)
        throw 失败{S::入口拒绝};
    if (used && used->已核验.contains({d.值.值, h}))
        return 概念树动态证据状态::已核验;
    const auto states = b.最大世界成员数 - (used ? used->状态数 : 0);
    const auto changes = b.最大世界成员数 - (used ? used->变化数 : 0);
    const auto snapshots = b.最大特征属性数 - (used ? used->快照数 : 0);
    if (!states || !changes || !snapshots)
        throw 失败{S::数量预算不足};
    auto r = dynamics_->读取动态历史事实({1, g, h, d.值, states, changes, snapshots});
    if (r.状态 == 动态类数据状态::历史材料已清理) {
        if (r.合同版本 != 1 || r.Gread != g || r.H != h || !r.缺失位置)
            throw 失败{S::内部不一致};
        if (current && *r.缺失位置 != 动态类历史缺失位置::状态快照依赖)
            throw 失败{S::内部不一致};
        守卫代次(g);
        return 概念树动态证据状态::历史已清理;
    }
    if (!r.成功())
        throw 失败{映射(r.状态)};
    if (r.Gread != g || r.H != h || r.动态结点->结点 != d.值)
        throw 失败{S::内部不一致};
    if (used) {
        used->状态数 += r.状态组.size();
        used->变化数 += r.变化特征组.size();
        for (const auto &q : r.状态组)
            used->快照数 += q.特征值快照组.size();
        used->已核验.emplace(d.值.值, h);
    }
    return 概念树动态证据状态::已核验;
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
            } else if constexpr (std::is_same_v<T, 概念树存在定义>) {
                if (d.特征模板组.empty())
                    throw 失败{S::入口拒绝};
                std::sort(d.特征模板组.begin(), d.特征模板组.end(), 身份小于);
                for (std::size_t i = 0; i < d.特征模板组.size(); ++i)
                    if (!有效(d.特征模板组[i].值) || (i && d.特征模板组[i] == d.特征模板组[i - 1]))
                        throw 失败{S::入口拒绝};
            } else {
                if (!dynamic_)
                    throw 失败{S::入口拒绝};
                for (auto &q : d.时间槽组)
                    std::sort(q.模板组.begin(), q.模板组.end(), 身份小于);
                if (!动态定义完整(d))
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
                const auto r = 特征结果(features_.读取准确特征事实({1,g,h,{x.值}}));
                if (r.Gread != g || r.H != h || r.信息.身份.编码 != x.值)
                    throw 失败{S::内部不一致};
            } else if constexpr (std::is_same_v<T, 概念树特征类型引用>) {
                const auto r = 特征结果(features_.读取先天I64特征类型事实({1,g,h,{x.值}}));
                if (r.Gread != g || r.H != h || r.数据.身份.编码 != x.值)
                    throw 失败{S::内部不一致};
            } else if constexpr (std::is_same_v<T, 概念树场景引用>) {
                if (!dynamic_)
                    throw 失败{S::入口拒绝};
                const auto r = scenes_->读取场景历史事实({1, g, h, x.值, b.最大世界成员数});
                if (!r.成功())
                    throw 失败{映射(r.状态)};
                if (r.Gread != g || r.H != h || r.场景结点->结点 != x.值)
                    throw 失败{S::内部不一致};
            } else {
                if (审计动态(x, g, h, b) != 概念树动态证据状态::已核验)
                    throw 失败{S::历史材料不可用};
            }
        },
        w);
}
void 概念树类数据服务::验证特征定义(const 概念树特征定义 &d, std::uint64_t g, std::uint64_t h,
                                    const 概念树预算 &b) const {
    验证世界(d.形成宿主, g, h, b);
    const auto r = 特征结果(features_.读取先天I64特征类型事实({1,g,h,{d.特征类型.值}}));
    if (r.Gread != g || r.H != h || r.数据.身份.编码 != d.特征类型.值)
        throw 失败{S::内部不一致};
    using R = L1所有者范围值表示种类;
    R required = R::I64;
    if (const auto *exact = std::get_if<概念树精确值>(&d.值域))
        required = exact->index() == 1 ? R::I64组 : exact->index() == 2 ? R::U64组 : R::I64;
    if (required != R::I64)
        throw 失败{S::不支持};
}

// 当前结构与不可变记录的形成关系分别按结构 H 与记录创建 H 投影。
概念树类数据服务::概念展开 概念树类数据服务::展开概念(概念树概念身份 id, std::uint64_t g, std::uint64_t h,
                                                      const 概念树预算 &b, bool narrow, bool structure, 共享读取上下文 *shared) const {
    auto rel = [&](稳定编码 id, 稳定编码 type, bool incoming, std::uint64_t G, std::uint64_t H, std::uint64_t limit) {
        if (!shared) return 关系(id, type, incoming, G, H, limit);
        if (G != shared->G) throw 失败{S::内部不一致};
        auto edges = 共享关系(id, type, incoming, H, *shared);
        检查数量(edges.size(), limit); return edges;
    };
    auto one = [&](稳定编码 id, 稳定编码 type, std::uint64_t G, std::uint64_t H) {
        auto edges = rel(id, type, false, G, H, 1);
        if (edges.size() != 1 || edges.front().角色或顺序 != 1) throw 失败{S::内部不一致};
        return edges.front();
    };
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
    if (kind == 4) throw 失败{S::不支持};
    if ((kind != 1 && kind != 2 && kind != 3) || state < 1 || state > 3 ||
        kindValue.创建事实代次 != n.创建事实代次)
        throw 失败{S::内部不一致};
    if ((kind == 3 && !dynamic_) || (narrow && kind != 1))
        throw 失败{S::入口拒绝};
    if (kind == 3)
        f.规则 = dynamic_->签名规则;
    f.根角色 = kind == 1 ? 概念树根角色::特征 : kind == 2 ? 概念树根角色::存在 : 概念树根角色::动态;
    f.本体根 = 根(f.根角色);
    f.治理状态 = static_cast<概念树生命周期状态>(state);
    f.生命周期值事实 = lifeValue.编码;
    for (const auto &v : a)
        out.当前自有事实.push_back(v.编码);
    const auto rootEdge = one(id.值, t.概念根归属, g, h);
    const auto ruleEdge = one(id.值, t.概念规则, g, h);
    const auto definitionEdge = one(id.值, t.定义成员, g, h);
    if (rootEdge.目标节点 != f.本体根.值 || ruleEdge.目标节点 != f.规则.值)
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
        const auto e = one(recordId, type, g, creation);
        if (e.创建事实代次 != creation)
            throw 失败{S::内部不一致};
        const auto current = rel(recordId, type, false, g, h, 1);
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
        if (!rel(record.编码, t.定义模板, false, g, h, b.最大关系数).empty())
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

        if (!narrow && !structure)
            验证特征定义(d, g, n.创建事实代次, b);
        else {
            const auto ft = 特征结果(features_.读取先天I64特征类型事实({1,g,n.创建事实代次,{d.特征类型.值}}));
            using R = L1所有者范围值表示种类;
            const auto *exact = std::get_if<概念树精确值>(&d.值域);
            const auto rep = exact && exact->index() == 1   ? R::I64组
                             : exact && exact->index() == 2 ? R::U64组
                                                            : R::I64;
            if (rep != R::I64) throw 失败{S::不支持};
            if (ft.Gread != g || ft.H != n.创建事实代次 || ft.数据.身份.编码 != d.特征类型.值)
                throw 失败{S::内部不一致};
        }
        f.定义 = d;
    } else if (kind == 2) {
        if (!definitionValues.empty() ||
            !rel(record.编码, t.定义宿主, false, g, n.创建事实代次, 1).empty() ||
            !rel(record.编码, t.定义特征类型, false, g, n.创建事实代次, 1).empty())
            throw 失败{S::内部不一致};
        概念树存在定义 d;
        const auto templates = rel(record.编码, t.定义模板, false, g, h, b.最大关系数);
        if (templates != rel(record.编码, t.定义模板, false, g, n.创建事实代次, b.最大关系数))
            throw 失败{S::内部不一致};
        for (const auto &e : templates) {
            if (e.角色或顺序 != d.特征模板组.size() + 1 || e.创建事实代次 != n.创建事实代次)
                throw 失败{S::内部不一致};
            const auto target = 概念树概念身份{e.目标节点};
            // 先检查种类，禁止坏模板链递归成为环。
            const auto targetNode = 节点(target.值, g, h);
            const auto targetValues = 属性(target.值, g, h);
            if (std::get<std::int64_t>(唯一属性(targetValues, t.概念种类).材料) == 4)
                throw 失败{S::不支持};
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
    } else {
        if (!b.最大动态槽数 || !b.最大动态模板数)
            throw 失败{S::入口拒绝};
        const auto &dt = dynamic_->类型;
        if (!definitionValues.empty() ||
            !rel(record.编码, t.定义宿主, false, g, n.创建事实代次, 1).empty() ||
            !rel(record.编码, t.定义特征类型, false, g, n.创建事实代次, 1).empty() ||
            !rel(record.编码, t.定义模板, false, g, h, b.最大关系数).empty())
            throw 失败{S::内部不一致};
        const auto se = formedEdge(record.编码, dt.定义场景, n.创建事实代次);
        const auto ee = formedEdge(record.编码, dt.定义主体, n.创建事实代次);
        概念树动态定义 d{概念树场景引用{se.目标节点}, 概念树存在引用{ee.目标节点}, {}};
        if (有效于(se, h))
            验证世界(d.场景, g, n.创建事实代次, b);
        if (有效于(ee, h))
            验证世界(d.主体, g, n.创建事实代次, b);
        const auto slotLimit = shared ? std::min(b.最大动态槽数, shared->预算.基础.最大动态槽数 - shared->动态槽数) : b.最大动态槽数;
        const auto slots = rel(record.编码, dt.时间槽, false, g, h, slotLimit);
        if (shared) shared->动态槽数 += slots.size();
        if (slots != rel(record.编码, dt.时间槽, false, g, n.创建事实代次, b.最大动态槽数))
            throw 失败{S::内部不一致};
        std::uint64_t count = 0;
        std::set<std::uint64_t> slotIds;
        for (const auto &slot : slots) {
            if (slot.角色或顺序 != d.时间槽组.size() + 1 || slot.创建事实代次 != n.创建事实代次 ||
                !slotIds.insert(slot.目标节点.值).second)
                throw 失败{S::内部不一致};
            const auto sn = 节点(slot.目标节点, g, h);
            const auto sv = 属性(sn.编码, g, h);
            if (sn.种类 != 节点种类::普通 || sn.属性类型表示 || sn.创建事实代次 != n.创建事实代次 ||
                sv.size() != 1)
                throw 失败{S::内部不一致};
            const auto &tv = 唯一属性(sv, dt.相对时间);
            if (tv.创建事实代次 != n.创建事实代次)
                throw 失败{S::内部不一致};
            概念树动态时间槽 q{std::get<std::int64_t>(tv.材料), {}};
            if (count == b.最大动态模板数)
                throw 失败{S::数量预算不足};
            const auto templateLimit = shared ? std::min(b.最大动态模板数 - count, shared->预算.基础.最大动态模板数 - shared->动态模板数) : b.最大动态模板数 - count;
            const auto templates = rel(sn.编码, dt.槽模板, false, g, h, templateLimit);
            if (shared) shared->动态模板数 += templates.size();
            if (templates != rel(sn.编码, dt.槽模板, false, g, n.创建事实代次, b.最大动态模板数 - count))
                throw 失败{S::内部不一致};
            if (templates.size() > b.最大动态模板数 - count)
                throw 失败{S::数量预算不足};
            count += templates.size();
            for (const auto &e : templates) {
                if (e.角色或顺序 != q.模板组.size() + 1 || e.创建事实代次 != n.创建事实代次)
                    throw 失败{S::内部不一致};
                const auto templateId = 概念树概念身份{e.目标节点};
                if (shared) {
                    共享截止作用域 at(*shared, h);
                    const auto ft = 应用模板(templateId, *shared);
                    if (const auto *old = std::get_if<概念树动态模板事实>(&ft); old && old->定义.形成宿主 != d.主体)
                        throw 失败{S::引用冲突};
                } else if (动态模板(templateId, g, h, b).定义.形成宿主 != d.主体) throw 失败{S::引用冲突};
                q.模板组.push_back(templateId);
                out.当前自有事实.push_back(e.编码);
            }
            d.时间槽组.push_back(std::move(q));
            for (auto code : {slot.编码, sn.编码, tv.编码})
                out.当前自有事实.push_back(code);
        }
        if (!动态定义完整(d))
            throw 失败{S::内部不一致};
        f.定义 = std::move(d);
    }
    if (narrow)
        return out;
    const auto sourceLimit = shared ? std::min(b.最大来源数, shared->预算.基础.最大来源数 - shared->来源数) : b.最大来源数;
    const auto sources = rel(id.值, t.来源成员, false, g, h, sourceLimit);
    if (shared) shared->来源数 += sources.size();
    if (sources != rel(id.值, t.来源成员, false, g, n.创建事实代次, b.最大来源数))
        throw 失败{S::内部不一致};
    if (sources.empty())
        throw 失败{S::内部不一致};
    for (const auto &member : sources) {
        if (member.角色或顺序 != f.来源组.size() + f.动态依据组.size() + 1 ||
            member.创建事实代次 != n.创建事实代次)
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
        if (kind == 3) {
            const auto &dt = dynamic_->类型;
            const auto ds = rel(sourceNode.编码, dt.来源动态, false, g, n.创建事实代次, 1);
            const auto cs = rel(sourceNode.编码, dt.来源下位概念, false, g, n.创建事实代次, 1);
            if (ds.size() + cs.size() != 1 ||
                !rel(sourceNode.编码, t.来源存在, false, g, n.创建事实代次, 1).empty() ||
                !rel(sourceNode.编码, t.来源特征, false, g, n.创建事实代次, 1).empty())
                throw 失败{S::内部不一致};
            const auto e =
                formedEdge(sourceNode.编码, ds.empty() ? dt.来源下位概念 : dt.来源动态, n.创建事实代次);
            概念树动态依据项 item;
            if (ds.empty())
                item.目标 = 概念树概念身份{e.目标节点};
            else
                item.目标 = 概念树动态引用{e.目标节点};
            item.H = times[0];
            if (!有效(e.目标节点) || e.目标节点 == id.值)
                throw 失败{S::内部不一致};
            f.动态依据组.push_back({sourceNode.编码, member.编码, e.编码, time.编码, item,
                                    投影生命(sourceNode, h), !有效于(e, h)});
            for (auto code : {sourceNode.编码, member.编码, time.编码})
                out.当前自有事实.push_back(code);
            continue;
        }
        const auto oldExists = rel(sourceNode.编码, t.来源存在, false, g, n.创建事实代次, 1);
        const auto oldFeatures = rel(sourceNode.编码, t.来源特征, false, g, n.创建事实代次, 1);
        if (kind == 2 && (!existence_layout_ || !rel(sourceNode.编码,
            existence_layout_->抽象依据目标关系类型, false,
            g, n.创建事实代次, 1).empty())) throw 失败{S::不支持};
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
        if (!structure) 验证世界(扩展世界(s.来源.世界事实), g, times[0], b);
        f.来源组.push_back(s);
        for (auto code : {sourceNode.编码, member.编码, time.编码})
            out.当前自有事实.push_back(code);
    }
    const auto parents = rel(id.值, t.直接上位, true, g, h, b.最大关系数);
    if (parents.empty())
        throw 失败{S::引用冲突};
    std::set<std::uint64_t> parentIds;
    for (const auto &e : parents) {
        const auto parent = 概念树概念身份{e.源节点};
        if (e.角色或顺序 != 1 || parent == id || !parentIds.insert(e.源节点.值).second)
            throw 失败{S::引用冲突};
        (void)节点(parent.值, g, h);
        if (parent != f.本体根 && one(parent.值, t.概念根归属, g, h).目标节点 != f.本体根.值)
            throw 失败{S::引用冲突};
        f.直接上位组.push_back({e.编码, parent, id, 投影生命(e, h)});
    }
    std::sort(f.直接上位组.begin(), f.直接上位组.end(),
              [](const auto &a, const auto &b) { return a.上位.值.值 < b.上位.值.值; });

    return out;
}
概念树概念事实 概念树类数据服务::读取概念内部(概念树概念身份 id, std::uint64_t g, std::uint64_t h,
                                              const 概念树预算 &b, 共享读取上下文 *shared) const {
    return 展开概念(id, g, h, b, false, shared != nullptr, shared).事实;
}
概念树支持事实 概念树类数据服务::支持记录(稳定编码 id, std::uint64_t g, std::uint64_t h,
                                          const 概念树预算 &b, 共享读取上下文 *shared) const {
    auto rel = [&](稳定编码 id, 稳定编码 type, bool incoming, std::uint64_t G, std::uint64_t H, std::uint64_t limit) {
        if (!shared) return 关系(id, type, incoming, G, H, limit);
        auto edges = 共享关系(id, type, incoming, H, *shared);
        检查数量(edges.size(), limit);
        return edges;
    };
    auto one = [&](稳定编码 id, 稳定编码 type, std::uint64_t G, std::uint64_t H) {
        auto edges = rel(id, type, false, G, H, 1);
        if (edges.size() != 1 || edges.front().角色或顺序 != 1) throw 失败{S::内部不一致};
        return edges.front();
    };
    const auto n = 节点(id, g, h);
    if (n.种类 != 节点种类::普通 || n.属性类型表示)
        throw 失败{S::引用冲突};
    const auto &t = layout_.类型;
    const auto anchor = rel(id, t.支持归属, true, g, h, 1);
    if (anchor.size() != 1 || anchor.front().源节点 != layout_.格式锚点 || anchor.front().角色或顺序 != 1)
        throw 失败{S::内部不一致};
    const auto c = one(id, t.支持概念, g, h);
    const auto exists = rel(id, t.支持存在, false, g, h, 1);
    const auto features = rel(id, t.支持特征, false, g, h, 1);
    const auto dynamics = dynamic_ ? rel(id, dynamic_->类型.支持动态, false, g, h, 1) : std::vector<E>{};
    if (exists.size() + features.size() + dynamics.size() != 1)
        throw 失败{S::内部不一致};
    const auto &e = !exists.empty()     ? exists.front()
                    : !features.empty() ? features.front()
                                        : dynamics.front();
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
    if (shared) { 共享截止作用域 at(*shared, h); (void)应用定义(s.目标概念, *shared); }
    else (void)读取概念内部(s.目标概念, g, h, b);
    if (!dynamics.empty()) {
        s.世界引用 = 概念树动态引用{e.目标节点};
        s.动态证据 = 概念树动态证据状态::未展开;
    } else
        验证世界(扩展世界(s.世界引用), g, s.证据H, b);
    return s;
}
std::vector<概念树支持事实> 概念树类数据服务::支持组(std::optional<概念树概念身份> conceptId,
                                                     std::optional<概念树世界引用> world, std::uint64_t g,
                                                     std::uint64_t h, const 概念树预算 &b, 共享读取上下文 *shared) const {
    auto rel = [&](稳定编码 id, 稳定编码 type, bool incoming, std::uint64_t G, std::uint64_t H, std::uint64_t limit) {
        if (!shared) return 关系(id, type, incoming, G, H, limit);
        auto edges = 共享关系(id, type, incoming, H, *shared);
        检查数量(edges.size(), limit);
        return edges;
    };
    if (conceptId) {
        if (shared) { 共享截止作用域 at(*shared, h); (void)应用定义(*conceptId, *shared); }
        else (void)读取概念内部(*conceptId, g, h, b);
    }
    if (world && world->index() != 2)
        验证世界(扩展世界(*world), g, h, b);
    if (world && world->index() == 2 && (!dynamic_ || !有效(世界编码(*world))))
        throw 失败{S::入口拒绝};
    const auto &t = layout_.类型;
    std::vector<E> members;
    if (conceptId)
        members = rel(conceptId->值, t.支持概念, true, g, h, b.最大支持数);
    else if (world)
        members = rel(世界编码(*world), 支持关系类型(*world), true, g, h, b.最大支持数);
    else
        members = rel(layout_.格式锚点, t.支持归属, false, g, h, b.最大支持数);
    std::vector<概念树支持事实> out;
    std::set<std::pair<std::uint64_t, std::uint64_t>> endpoints;
    for (const auto &member : members) {
        const auto id = conceptId || world ? member.源节点 : member.目标节点;
        auto s = 支持记录(id, g, h, b, shared);
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
                                  const 概念树预算 &b, 共享读取上下文 *shared) const {
    if (shared) {
        共享截止作用域 at(*shared, h);
        const auto actual = 应用图(role, withSupport, *shared);
        概念树图结果 graph;
        graph.状态 = S::已读取; graph.Gread = g; graph.H = h; graph.根身份 = actual.根身份;
        graph.直接边组 = actual.直接边组; graph.支持组 = actual.支持组;
        for (const auto &node : actual.节点组) std::visit([&](const auto &f) {
            using T = std::decay_t<decltype(f)>;
            if constexpr (std::is_same_v<T, 概念树概念事实>) graph.概念组.push_back(f);
            else {
                // 仅供既有图算法的真实拓扑投影；不作为旧公开概念载荷返回。
                概念树概念事实 topology;
                if constexpr(std::is_same_v<T,特征概念事实>)topology.概念=f.身份;
                else topology.概念=f.概念;
                topology.本体根 = f.本体根; topology.根角色 = role;
                topology.规则 = f.规则; topology.生命周期 = f.生命周期; topology.治理状态 = f.治理状态;
                topology.定义记录 = f.定义记录; topology.生命周期值事实 = f.生命周期值事实;
                if constexpr(std::is_same_v<T,特征概念事实>)topology.直接上位组=f.直接上位;
                else topology.直接上位组=f.直接上位组;
                graph.概念组.push_back(std::move(topology));
            }
        }, node);
        return graph;
    }
    auto rel = [&](稳定编码 id, 稳定编码 type, bool incoming, std::uint64_t G, std::uint64_t H, std::uint64_t limit) {
        if (!shared) return 关系(id, type, incoming, G, H, limit);
        if (G != shared->G) throw 失败{S::内部不一致};
        auto edges = 共享关系(id, type, incoming, H, *shared);
        检查数量(edges.size(), limit); return edges;
    };
    概念树图结果 out;
    out.状态 = S::已读取;
    out.Gread = g;
    out.H = h;
    out.根身份 = 根(role);
    out.概念组.push_back(读取概念内部(out.根身份, g, h, b, shared));
    const auto members = rel(out.根身份.值, layout_.类型.概念根归属, true, g, h, b.最大概念数);
    检查数量(members.size() + 1, b.最大概念数);
    std::set<std::uint64_t> edges;
    std::uint64_t sources = 0, slots = 0, templates = 0;
    for (const auto &member : members) {
        if (member.角色或顺序 != 1)
            throw 失败{S::内部不一致};
        auto remaining = b;
        if (role == 概念树根角色::动态) {
            if (slots >= b.最大动态槽数 || templates >= b.最大动态模板数)
                throw 失败{S::数量预算不足};
            remaining.最大动态槽数 -= slots;
            remaining.最大动态模板数 -= templates;
        }
        if (sources >= b.最大来源数)
            throw 失败{S::数量预算不足};
        remaining.最大来源数 -= sources;
        auto f = 读取概念内部(概念树概念身份{member.源节点}, g, h, remaining, shared);
        if (f.本体根 != out.根身份 || f.是本体根)
            throw 失败{S::内部不一致};
        const auto sourceCount = f.来源组.size() + f.动态依据组.size();
        if (sourceCount > b.最大来源数 - sources)
            throw 失败{S::数量预算不足};
        sources += sourceCount;
        if (f.定义 && std::holds_alternative<概念树动态定义>(*f.定义)) {
            const auto &d = std::get<概念树动态定义>(*f.定义);
            if (d.时间槽组.size() > b.最大动态槽数 - slots)
                throw 失败{S::数量预算不足};
            slots += d.时间槽组.size();
            for (const auto &q : d.时间槽组) {
                if (q.模板组.size() > b.最大动态模板数 - templates)
                    throw 失败{S::数量预算不足};
                templates += q.模板组.size();
            }
        }
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
        for (const auto &e : rel(f.概念.值, layout_.类型.直接上位, false, g, h, b.最大关系数))
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
            auto supports = 支持组(f.概念, std::nullopt, g, h, b, shared);
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
                                                             std::uint64_t h, const 概念树预算 &b, 共享读取上下文 *shared) const {
    auto rel = [&](稳定编码 id, 稳定编码 type, bool incoming, std::uint64_t G, std::uint64_t H, std::uint64_t limit) {
        if (!shared) return 关系(id, type, incoming, G, H, limit);
        if (G != shared->G) throw 失败{S::内部不一致};
        auto edges = 共享关系(id, type, incoming, H, *shared);
        检查数量(edges.size(), limit); return edges;
    };
    if (!有效(世界编码(world)))
        throw 失败{S::入口拒绝};
    std::vector<概念树形成引用事实> out;
    const auto &t = layout_.类型;
    std::vector<稳定编码> types{t.定义宿主, t.定义特征类型, t.来源存在, t.来源特征};
    if (dynamic_) {
        types.push_back(dynamic_->类型.定义场景);
        types.push_back(dynamic_->类型.定义主体);
        types.push_back(dynamic_->类型.来源动态);
    }
    if (world.index() >= 3 && !dynamic_)
        throw 失败{S::入口拒绝};
    for (std::size_t i = 0; i < types.size(); ++i) {
        const std::array<std::size_t, 7> indices{0, 2, 0, 1, 3, 0, 4};
        if (world.index() != indices[i])
            continue;
        for (const auto &edge : rel(世界编码(world), types[i], true, g, h, b.最大关系数)) {
            if (edge.角色或顺序 != 1)
                throw 失败{S::内部不一致};
            auto parent = rel(edge.源节点, (i < 2 || i == 4 || i == 5) ? t.定义成员 : t.来源成员, true, g, h,
                               b.最大关系数);
            if (parent.size() != 1)
                throw 失败{S::内部不一致};
            const auto ownerId = 概念树概念身份{parent[0].源节点};
            if (shared) {
                const auto ownerValues = 属性(ownerId.值, g, h);
                if (std::get<std::int64_t>(唯一属性(ownerValues, t.概念种类).材料) == 2) {
                    共享截止作用域 at(*shared, h);
                    const auto ec = 展开存在定义_v2(ownerId, *shared);
                    if (i != 2 || std::find(ec.自有事实.begin(), ec.自有事实.end(), edge.编码) == ec.自有事实.end())
                        throw 失败{S::内部不一致};
                    out.push_back({edge.编码, edge.源节点, ownerId, world, 投影生命(edge, h)});
                    检查数量(out.size(), b.最大关系数); continue;
                }
            }
            auto f = 展开概念(ownerId, g, h, b, false, shared != nullptr, shared);
            if (std::find(f.当前自有事实.begin(), f.当前自有事实.end(), edge.编码) == f.当前自有事实.end())
                throw 失败{S::内部不一致};
            if (i < 2 && (!f.事实.定义 || !std::holds_alternative<概念树特征定义>(*f.事实.定义)))
                throw 失败{S::内部不一致};
            if (i >= 4 && (!f.事实.定义 || !std::holds_alternative<概念树动态定义>(*f.事实.定义)))
                throw 失败{S::内部不一致};
            out.push_back({edge.编码, edge.源节点, f.事实.概念, world, 投影生命(edge, h)});
            检查数量(out.size(), b.最大关系数);
        }
    }
    if (shared && feature_layout_ && std::holds_alternative<概念树特征类型引用>(world)) {
        共享截止作用域 at(*shared, h);
        const auto& x = feature_layout_->类型;
        for (const auto& edge : 共享关系(世界编码(world), x[16], true, h, *shared)) {
            if (edge.角色或顺序 != 1) throw 失败{S::内部不一致};
            const auto owner = 共享关系(edge.源节点, x[15], true, h, *shared);
            if (owner.size() != 1) throw 失败{S::内部不一致};
            const auto f = 展开共享定义(概念树概念身份{owner.front().源节点}, *shared).事实;
            const auto source = std::find_if(f.来源事实.begin(), f.来源事实.end(), [&](const auto& s) {
                const auto* ft = std::get_if<特征类型身份>(&s.来源);
                return ft && ft->编码 == 世界编码(world) && s.记录 == edge.源节点
                    && s.目标关系 == edge.编码 && s.所属关系 == owner.front().编码 && !s.当前引用已释放;
            });
            if (source == f.来源事实.end()) throw 失败{S::内部不一致};
            out.push_back({edge.编码, edge.源节点, f.身份, world, 投影生命(edge, h)});
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
        auto graph = 图(d.index() == 0   ? 概念树根角色::特征
                        : d.index() == 1 ? 概念树根角色::存在
                                         : 概念树根角色::动态,
                        false, g, h, r.预算);
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
               f.来源组.empty() && f.动态依据组.empty() && f.直接上位组.empty() &&
               f.治理状态 == 概念树生命周期状态::活跃 && !f.生命周期.退出事实代次;
    if (f.概念 == f.本体根 || !f.定义 || !有效(f.定义记录) || !有效(f.生命周期值事实) ||
        (f.定义->index() == 2 ? (!f.来源组.empty() || f.动态依据组.empty())
                              : (f.来源组.empty() || !f.动态依据组.empty())) ||
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
    } else if (std::holds_alternative<概念树存在定义>(*f.定义)) {
        const auto &ids = std::get<概念树存在定义>(*f.定义).特征模板组;
        if (f.根角色 != 概念树根角色::存在 || ids.empty())
            return false;
        for (std::size_t i = 0; i < ids.size(); ++i)
            if (!有效(ids[i].值) || (i && !身份小于(ids[i - 1], ids[i])))
                return false;
    } else if (f.根角色 != 概念树根角色::动态 || !动态定义完整(std::get<概念树动态定义>(*f.定义)))
        return false;
    for (std::size_t i = 0; i < f.动态依据组.size(); ++i) {
        const auto &a = f.动态依据组[i];
        if (!动态依据完整(a, h, exited) || a.生命周期.创建事实代次 != f.生命周期.创建事实代次)
            return false;
        for (std::size_t j = 0; j < i; ++j)
            if (a.记录 == f.动态依据组[j].记录 || a.依据 == f.动态依据组[j].依据)
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
           f.证据H && f.证据H < f.记录生命周期.创建事实代次 && 生命完整(f.记录生命周期, h, exited) &&
           (f.世界引用.index() == 2 ? (f.动态证据 && static_cast<unsigned>(*f.动态证据) >= 1 &&
                                       static_cast<unsigned>(*f.动态证据) <= 3)
                                    : !f.动态证据);
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
            if (请求写头(r).合同版本 != 1 || !请求写头(r).期望事实代次 || !请求写头(r).幂等身份)
                throw 失败{S::入口拒绝};
            预算有效(r.预算);
            if constexpr (std::is_same_v<T, 概念树概念建立请求>) {
                if (r.定义.index() > 1)
                    throw 失败{S::入口拒绝};
                for (const auto &a : r.来源)
                    if (a.世界事实.index() > 1)
                        throw 失败{S::入口拒绝};
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
                        r.来源[i].证据截止 > 请求写头(r).期望事实代次 || (i && r.来源[i] == r.来源[i - 1]))
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
                    r.证据H > 请求写头(r).期望事实代次)
                    throw 失败{S::入口拒绝};
            } else if constexpr (std::is_same_v<T, 概念树支持退出请求>) {
                if (!支持完整(r.预期支持, 请求写头(r).期望事实代次))
                    throw 失败{S::入口拒绝};
                if (r.预期支持.世界引用.index() == 2)
                    r.预期支持.动态证据 = 概念树动态证据状态::未展开;
            } else if constexpr (std::is_same_v<T, 概念树生命周期迁移请求>) {
                if (!有效(r.概念.值) || !有效(r.当前生命周期值编码) ||
                    static_cast<unsigned>(r.预期状态) < 1 || static_cast<unsigned>(r.预期状态) > 3 ||
                    static_cast<unsigned>(r.目标状态) < 1 || static_cast<unsigned>(r.目标状态) > 3)
                    throw 失败{S::入口拒绝};
            } else if constexpr (std::is_same_v<T, 概念树概念退出请求>) {
                if (!有效(r.概念.值) || !有效(r.当前退役生命周期值编码))
                    throw 失败{S::入口拒绝};
            } else if constexpr (std::is_same_v<T, 概念树形成引用释放请求>) {
                if (!有效(世界编码(r.世界引用)))
                    throw 失败{S::入口拒绝};
                检查数量(r.预期当前引用.size(), r.预算.最大关系数);
                for (std::size_t i = 0; i < r.预期当前引用.size(); ++i) {
                    const auto &f = r.预期当前引用[i];
                    if (f.世界引用.valueless_by_exception() || !有效(f.关系) || !有效(f.记录) ||
                        !有效(f.所属概念.值) || f.世界引用 != r.世界引用 ||
                        !生命完整(f.关系生命周期, 请求写头(r).期望事实代次) ||
                        (i && !编码小于(r.预期当前引用[i - 1].关系, f.关系)))
                        throw 失败{S::入口拒绝};
                }
            } else if constexpr (std::is_same_v<T, 概念树动态建立请求>) {
                if (!dynamic_ || !r.预算.最大动态槽数 || !r.预算.最大动态模板数)
                    throw 失败{S::入口拒绝};
                r.定义 = std::get<概念树动态定义>(规范化定义(r.定义));
                if (r.直接上位.empty() || r.依据.empty())
                    throw 失败{S::入口拒绝};
                检查数量(r.定义.时间槽组.size(), r.预算.最大动态槽数);
                检查数量(r.依据.size(), r.预算.最大来源数);
                检查数量(r.直接上位.size(), r.预算.最大关系数);
                std::uint64_t nodes = 2, edges = 5, values = 2, templates = 0;
                auto count = [](std::uint64_t &n, std::size_t add) {
                    if (add > 0xFFFF - n)
                        throw 失败{S::数量预算不足};
                    n += add;
                };
                count(nodes, r.定义.时间槽组.size());
                count(nodes, r.依据.size());
                count(edges, r.直接上位.size());
                count(edges, r.定义.时间槽组.size());
                count(edges, r.依据.size());
                count(edges, r.依据.size());
                count(values, r.定义.时间槽组.size());
                count(values, r.依据.size());
                for (const auto &q : r.定义.时间槽组) {
                    count(edges, q.模板组.size());
                    if (q.模板组.size() > r.预算.最大动态模板数 - templates)
                        throw 失败{S::数量预算不足};
                    templates += q.模板组.size();
                }
                检查数量(edges, r.预算.最大关系数);
                std::sort(r.直接上位.begin(), r.直接上位.end(), 身份小于);
                for (std::size_t i = 0; i < r.直接上位.size(); ++i)
                    if (!有效(r.直接上位[i].值) || (i && r.直接上位[i] == r.直接上位[i - 1]))
                        throw 失败{S::入口拒绝};
                for (const auto &a : r.依据)
                    if (a.目标.valueless_by_exception() || !有效(世界编码(a.目标)) || !a.H ||
                        a.H > r.头.期望事实代次)
                        throw 失败{S::入口拒绝};
                std::sort(r.依据.begin(), r.依据.end(), [](const auto &a, const auto &b) {
                    if (a.目标.index() != b.目标.index())
                        return a.目标.index() < b.目标.index();
                    if (世界编码(a.目标) != 世界编码(b.目标))
                        return 编码小于(世界编码(a.目标), 世界编码(b.目标));
                    return a.H < b.H;
                });
                if (std::adjacent_find(r.依据.begin(), r.依据.end()) != r.依据.end())
                    throw 失败{S::入口拒绝};
            } else {
                if (!dynamic_ || !有效(r.所属概念.值))
                    throw 失败{S::入口拒绝};
                检查数量(r.预期当前依据.size(), r.预算.最大来源数);
                for (std::size_t i = 0; i < r.预期当前依据.size(); ++i) {
                    const auto &a = r.预期当前依据[i];
                    if (!动态依据完整(a, r.头.期望事实代次) || a.当前引用已释放 ||
                        (i && !编码小于(r.预期当前依据[i - 1].引用关系, a.引用关系)))
                        throw 失败{S::入口拒绝};
                }
            }
        },
        out);
    return out;
}
L1所有者范围写集请求 概念树类数据服务::形成写集(const 概念树业务写请求 &request, std::uint64_t g,
                                                std::uint64_t h, 共享读取上下文 *shared) const {
    auto rel = [&](稳定编码 id, 稳定编码 type, bool incoming, std::uint64_t G, std::uint64_t H, std::uint64_t limit) {
        if (!shared) return 关系(id, type, incoming, G, H, limit);
        if (G != shared->G) throw 失败{S::内部不一致};
        auto edges = 共享关系(id, type, incoming, H, *shared);
        检查数量(edges.size(), limit); return edges;
    };
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
            out.期望事实代次 = 请求写头(r).期望事实代次;
            out.写入幂等身份 = {请求写头(r).幂等身份};
            const auto &b = r.预算;
            if constexpr (std::is_same_v<T, 概念树概念建立请求>) {
                const auto role = r.定义.index() == 0 ? 概念树根角色::特征 : 概念树根角色::存在;
                const auto graph = 图(role, false, g, h, b, shared);
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
                    const auto f = 读取概念内部(parent, g, h, b, shared);
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
                        const auto f = 读取概念内部(id, g, h, b, shared);
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
                const auto child = 读取概念内部(r.下位, g, h, b, shared);
                if (child.是本体根)
                    throw 失败{S::根不可退出};
                auto graph = 图(child.根角色, false, g, h, b, shared);
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
                    const auto parent = 读取概念内部(*r.新上位, g, h, b, shared);
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
                if (shared) (void)应用定义(r.目标概念, *shared); else (void)读取概念内部(r.目标概念, g, h, b);
                auto supports = 支持组(r.目标概念, std::nullopt, g, h, b, shared);
                if (std::any_of(supports.begin(), supports.end(),
                                [&](const auto &s) { return s.世界引用 == r.世界引用; }))
                    return;
                检查数量(supports.size() + 1, b.最大支持数);
                检查数量(支持组(std::nullopt, r.世界引用, g, h, b, shared).size() + 1, b.最大支持数);
                验证世界(扩展世界(r.世界引用), g, r.证据H, b);
                验证世界(扩展世界(r.世界引用), g, h, b);
                const auto n = newNode();
                newEdge(Ref{layout_.格式锚点}, n, t.支持归属);
                newEdge(n, Ref{r.目标概念.值}, t.支持概念);
                newEdge(n, Ref{世界编码(r.世界引用)}, 支持关系类型(r.世界引用));
                newValue(n, t.证据截止, std::vector<std::uint64_t>{r.证据H});
            } else if constexpr (std::is_same_v<T, 概念树支持退出请求>) {
                const auto s = 支持记录(r.预期支持.记录, g, h, b, shared);
                if (s != r.预期支持)
                    throw 失败{S::引用冲突};
                out.退出事实 = {s.记录, s.归属关系, s.概念关系, s.世界关系, s.截止值事实};
            } else if constexpr (std::is_same_v<T, 概念树生命周期迁移请求>) {
                const auto f = 读取概念内部(r.概念, g, h, b, shared);
                if (f.是本体根)
                    throw 失败{S::根不可退出};
                if (f.治理状态 != r.预期状态 || f.生命周期值事实 != r.当前生命周期值编码)
                    throw 失败{S::引用冲突};
                if (r.预期状态 == r.目标状态)
                    return;
                if (!生命周期迁移有效(r.预期状态, r.目标状态))
                    throw 失败{S::入口拒绝};
                out.退出事实.push_back(r.当前生命周期值编码);
                newValue(Ref{r.概念.值}, t.生命周期, static_cast<std::int64_t>(r.目标状态));
            } else if constexpr (std::is_same_v<T, 概念树概念退出请求>) {
                const auto expanded = 展开概念(r.概念, g, h, b, false, shared != nullptr, shared);
                const auto &f = expanded.事实;
                if (f.是本体根)
                    throw 失败{S::根不可退出};
                if (f.治理状态 != 概念树生命周期状态::退役 || f.生命周期值事实 != r.当前退役生命周期值编码)
                    throw 失败{S::引用冲突};
                if (!支持组(r.概念, std::nullopt, g, h, b, shared).empty() ||
                    !rel(r.概念.值, t.定义模板, true, g, h, b.最大关系数).empty())
                    throw 失败{S::引用冲突};
                if (dynamic_ &&
                    (!rel(r.概念.值, dynamic_->类型.槽模板, true, g, h, b.最大关系数).empty() ||
                     !rel(r.概念.值, dynamic_->类型.来源下位概念, true, g, h, b.最大关系数).empty()))
                    throw 失败{S::引用冲突};
                auto graph = 图(f.根角色, false, g, h, b, shared);
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
            } else if constexpr (std::is_same_v<T, 概念树形成引用释放请求>) {
                const auto actual = 形成引用组(r.世界引用, g, h, b, shared);
                if (actual != r.预期当前引用)
                    throw 失败{S::引用冲突};
                for (const auto &f : actual)
                    out.退出事实.push_back(f.关系);
            } else if constexpr (std::is_same_v<T, 概念树动态建立请求>) {
                const auto graph = 图(概念树根角色::动态, false, g, h, b, shared);
                for (const auto &f : graph.概念组)
                    if (f.定义 && *f.定义 == 概念树定义{r.定义})
                        return;
                检查数量(graph.概念组.size() + 1, b.最大概念数);
                if (r.直接上位.size() > b.最大关系数 - graph.直接边组.size())
                    throw 失败{S::数量预算不足};
                std::uint64_t sources = 0, slots = 0, templates = 0;
                for (const auto &f : graph.概念组) {
                    if (f.动态依据组.size() > b.最大来源数 - sources)
                        throw 失败{S::数量预算不足};
                    sources += f.动态依据组.size();
                    if (f.定义)
                        for (const auto &q : std::get<概念树动态定义>(*f.定义).时间槽组) {
                            if (slots == b.最大动态槽数 || q.模板组.size() > b.最大动态模板数 - templates)
                                throw 失败{S::数量预算不足};
                            ++slots;
                            templates += q.模板组.size();
                        }
                }
                if (r.依据.size() > b.最大来源数 - sources || r.定义.时间槽组.size() > b.最大动态槽数 - slots)
                    throw 失败{S::数量预算不足};
                for (const auto &q : r.定义.时间槽组) {
                    if (q.模板组.size() > b.最大动态模板数 - templates)
                        throw 失败{S::数量预算不足};
                    templates += q.模板组.size();
                }
                for (auto id : r.直接上位)
                    if (读取概念内部(id, g, h, b, shared).本体根 != layout_.根组.动态根)
                        throw 失败{S::引用冲突};
                验证动态定义(r.定义, g, h, b, shared);
                动态历史用量 localSourceUsage;
                auto &sourceUsage = shared ? shared->动态用量 : localSourceUsage;
                for (const auto &a : r.依据) {
                    验证动态依据(a, g, b, &sourceUsage, shared);
                    if (const auto *d = std::get_if<概念树动态引用>(&a.目标)) {
                        if (审计动态(*d, g, h, b, false, &sourceUsage) != 概念树动态证据状态::已核验)
                            throw 失败{S::历史材料不可用};
                    } else {
                        const auto f = 读取概念内部(std::get<概念树概念身份>(a.目标), g, h, b, shared);
                        if (f.是本体根 || f.根角色 != 概念树根角色::动态)
                            throw 失败{S::引用冲突};
                    }
                }
                const auto &dt = dynamic_->类型;
                const auto cn = newNode(), dn = newNode();
                std::vector<Ref> qs, as;
                for (const auto &q : r.定义.时间槽组)
                    qs.push_back(newNode());
                for (const auto &a : r.依据)
                    as.push_back(newNode());
                newEdge(cn, Ref{layout_.根组.动态根.值}, t.概念根归属);
                newEdge(cn, Ref{dynamic_->签名规则.值}, t.概念规则);
                newEdge(cn, dn, t.定义成员);
                for (auto id : r.直接上位)
                    newEdge(Ref{id.值}, cn, t.直接上位);
                newEdge(dn, Ref{r.定义.场景.值}, dt.定义场景);
                newEdge(dn, Ref{r.定义.主体.值}, dt.定义主体);
                for (std::size_t i = 0; i < qs.size(); ++i) {
                    newEdge(dn, qs[i], dt.时间槽, static_cast<std::int64_t>(i + 1));
                    const auto &ids = r.定义.时间槽组[i].模板组;
                    for (std::size_t j = 0; j < ids.size(); ++j)
                        newEdge(qs[i], Ref{ids[j].值}, dt.槽模板, static_cast<std::int64_t>(j + 1));
                }
                for (std::size_t i = 0; i < as.size(); ++i) {
                    newEdge(cn, as[i], t.来源成员, static_cast<std::int64_t>(i + 1));
                    newEdge(as[i], Ref{世界编码(r.依据[i].目标)},
                            r.依据[i].目标.index() == 0 ? dt.来源动态 : dt.来源下位概念);
                }
                newValue(cn, t.概念种类, std::int64_t{3});
                newValue(cn, t.生命周期, std::int64_t{1});
                for (std::size_t i = 0; i < qs.size(); ++i)
                    newValue(qs[i], dt.相对时间, r.定义.时间槽组[i].相对纳秒);
                for (std::size_t i = 0; i < as.size(); ++i)
                    newValue(as[i], t.证据截止, std::vector<std::uint64_t>{r.依据[i].H});
            } else {
                const auto f = 读取概念内部(r.所属概念, g, h, b, shared);
                if (f.是本体根 || f.根角色 != 概念树根角色::动态)
                    throw 失败{S::引用冲突};
                std::vector<概念树动态依据事实> actual;
                for (const auto &a : f.动态依据组)
                    if (!a.当前引用已释放)
                        actual.push_back(a);
                std::sort(actual.begin(), actual.end(),
                          [](const auto &a, const auto &z) { return 编码小于(a.引用关系, z.引用关系); });
                if (actual != r.预期当前依据)
                    throw 失败{S::引用冲突};
                for (const auto &a : actual)
                    out.退出事实.push_back(a.引用关系);
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
                                          const L1所有者范围写入结果 &committed, std::uint64_t g, 共享读取上下文 *shared) const {
    const auto h = committed.事实代次;
    if (committed.合同版本 != L1所有者范围CRUD合同版本 || committed.所有者 != port_.所有者身份() ||
        committed.写入幂等身份 != ws.写入幂等身份 || !h || h <= ws.期望事实代次 || h > g ||
        ((!shared || committed.状态 == L1所有者范围写入状态::成功) && !committed.是否形成内存权威发布) ||
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
    if (shared) (void)共享核验首次(ws, committed, *shared);
    else {
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
            if constexpr (std::is_same_v<T, 概念树概念建立请求> || std::is_same_v<T, 概念树动态建立请求>) {
                out.状态 = S::已创建;
                out.概念 = 读取概念内部(概念树概念身份{mapped(Key{1})}, g, h, r.预算, shared);
                if (out.概念->定义 != std::optional<概念树定义>{r.定义})
                    throw 失败{S::内部不一致};
                (void)图(out.概念->根角色, false, g, h, r.预算, shared);
            } else if constexpr (std::is_same_v<T, 概念树上位操作请求>) {
                out.状态 = r.操作 == 概念树上位操作::新增   ? S::已添加上位
                           : r.操作 == 概念树上位操作::替换 ? S::已替换上位
                                                            : S::已退出上位;
                out.概念 = 读取概念内部(r.下位, g, h, r.预算, shared);
                (void)图(out.概念->根角色, false, g, h, r.预算, shared);
            } else if constexpr (std::is_same_v<T, 概念树支持建立请求>) {
                out.状态 = S::已添加支持;
                out.支持 = 支持记录(mapped(Key{1}), g, h, r.预算, shared);
                const auto a = 支持组(r.目标概念, std::nullopt, g, h, r.预算, shared);
                const auto b = 支持组(std::nullopt, r.世界引用, g, h, r.预算, shared);
                if (std::count(a.begin(), a.end(), *out.支持) != 1 ||
                    std::count(b.begin(), b.end(), *out.支持) != 1)
                    throw 失败{S::内部不一致};
            } else if constexpr (std::is_same_v<T, 概念树支持退出请求>) {
                out.状态 = S::已退出支持;
                out.支持 = 支持记录(r.预期支持.记录, g, 请求写头(r).期望事实代次, r.预算, shared);
                out.支持->记录生命周期.退出事实代次 = h;
                const auto a = 支持组(r.预期支持.目标概念, std::nullopt, g, h, r.预算, shared);
                const auto b = 支持组(std::nullopt, r.预期支持.世界引用, g, h, r.预算, shared);
                for (const auto *list : {&a, &b})
                    if (std::any_of(list->begin(), list->end(),
                                    [&](const auto &f) { return f.记录 == r.预期支持.记录; }))
                        throw 失败{S::内部不一致};
            } else if constexpr (std::is_same_v<T, 概念树生命周期迁移请求>) {
                out.状态 = S::已迁移生命周期;
                out.概念 = 读取概念内部(r.概念, g, h, r.预算, shared);
                if (out.概念->治理状态 != r.目标状态 || out.概念->生命周期值事实 != mapped(Key{0x20000}))
                    throw 失败{S::内部不一致};
            } else if constexpr (std::is_same_v<T, 概念树概念退出请求>) {
                out.状态 = S::已退出概念;
                out.概念 = 读取概念内部(r.概念, g, 请求写头(r).期望事实代次, r.预算, shared);
                out.概念->生命周期.退出事实代次 = h;
                for (auto &source : out.概念->来源组)
                    source.记录生命周期.退出事实代次 = h;
                for (auto &source : out.概念->动态依据组)
                    source.生命周期.退出事实代次 = h;
                for (auto &edge : out.概念->直接上位组)
                    edge.生命周期.退出事实代次 = h;
                (void)图(out.概念->根角色, false, g, h, r.预算, shared);
            } else if constexpr (std::is_same_v<T, 概念树形成引用释放请求>) {
                out.状态 = S::已释放引用;
                const auto remaining = 形成引用组(r.世界引用, g, h, r.预算, shared);
                if (!remaining.empty())
                    throw 失败{S::内部不一致};
                for (const auto &f : r.预期当前引用) {
                    if (shared) (void)应用定义(f.所属概念, *shared);
                    else (void)读取概念内部(f.所属概念, g, h, r.预算);
                }
            } else {
                out.状态 = S::已释放引用;
                const auto f = 读取概念内部(r.所属概念, g, h, r.预算, shared);
                for (const auto &a : f.动态依据组)
                    if (!a.当前引用已释放)
                        throw 失败{S::内部不一致};
            }
        },
        request);
    if (!shared && !out.成功())
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
        const auto header = std::visit([](const auto &r) { return 请求写头(r); }, request);
        std::scoped_lock lock(mutex_);
        if (feature_pending_ || shared_pending_ || application_pending_) throw 失败{S::前次写入待收敛};
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
                    } else if constexpr (std::is_same_v<T, 概念树动态建立请求>) {
                        const auto graph = 图(概念树根角色::动态, false, g, g, r.预算);
                        for (const auto &f : graph.概念组)
                            if (f.定义 && *f.定义 == 概念树定义{r.定义})
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
            if (概念树类数据服务::请求写头(r).合同版本 != 1 || !概念树类数据服务::请求写头(r).幂等身份 ||
                !概念树类数据服务::请求写头(r).期望事实代次 ||
                (noChange ? 概念树类数据服务::请求写头(r).期望事实代次 != h
                          : 概念树类数据服务::请求写头(r).期望事实代次 >= h))
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
            } else if constexpr (std::is_same_v<T, 概念树形成引用释放请求>) {
                if (!(noChange || replay || 状态 == S::已释放引用) || 概念 || 支持 || !关系组.empty() ||
                    已退出事实.size() != r.预期当前引用.size())
                    return false;
                for (std::size_t i = 0; i < 已退出事实.size(); ++i)
                    if (已退出事实[i] != r.预期当前引用[i].关系)
                        return false;
                return noChange ? r.预期当前引用.empty() : !r.预期当前引用.empty();
            } else if constexpr (std::is_same_v<T, 概念树动态建立请求>) {
                return (noChange || replay || 状态 == S::已创建) && 概念 && !支持 && 概念->定义 &&
                       std::holds_alternative<概念树动态定义>(*概念->定义) &&
                       std::get<概念树动态定义>(*概念->定义) == r.定义 && !概念->生命周期.退出事实代次 &&
                       已退出事实.empty() && (noChange || 概念->生命周期.创建事实代次 == h);
            } else {
                if (!(noChange || replay || 状态 == S::已释放引用) || 概念 || 支持 || !关系组.empty() ||
                    已退出事实.size() != r.预期当前依据.size())
                    return false;
                for (std::size_t i = 0; i < 已退出事实.size(); ++i)
                    if (已退出事实[i] != r.预期当前依据[i].引用关系)
                        return false;
                return noChange ? r.预期当前依据.empty() : !r.预期当前依据.empty();
            }
        },
        *原请求);
}

bool 概念树动态模板读取结果::成功() const noexcept {
    using C = 概念树类数据服务;
    if (合同版本 != 1 || 状态 != 概念树数据状态::已读取 || !H || Gread < H || !模板)
        return false;
    const auto &f = *模板;
    if (!有效(f.模板.值) || !有效(f.本体根.值) || f.模板 == f.本体根 || !有效(f.规则.值) ||
        !有效(f.定义记录) || !有效(f.生命周期值事实) || !C::生命完整(f.生命周期, H) ||
        static_cast<unsigned>(f.治理状态) < 1 || static_cast<unsigned>(f.治理状态) > 3 ||
        !有效(f.定义.形成宿主.值) || !有效(f.定义.特征类型.值) || f.定义.值域.valueless_by_exception())
        return false;
    if (const auto *i = std::get_if<概念树I64区间>(&f.定义.值域))
        return i->下界 < i->上界;
    const auto &exact = std::get<概念树精确值>(f.定义.值域);
    if (exact.valueless_by_exception())
        return false;
    return std::visit(
        [](const auto &v) {
            if constexpr (std::is_same_v<std::decay_t<decltype(v)>, std::int64_t>)
                return true;
            else
                return !v.empty();
        },
        exact);
}
bool 概念树动态支持核验结果::成功() const noexcept {
    return 合同版本 == 1 && 状态 == 概念树数据状态::已读取 && H && Gread >= H && 支持 &&
           概念树类数据服务::支持完整(*支持, H) && 支持->世界引用.index() == 2 &&
           (支持->动态证据 == 概念树动态证据状态::已核验 || 支持->动态证据 == 概念树动态证据状态::历史已清理);
}
bool 概念树动态依据审计结果::成功() const noexcept {
    if (合同版本 != 1 || 状态 != 概念树数据状态::已读取 || !H || Gread < H || 依据组.empty())
        return false;
    for (std::size_t i = 0; i < 依据组.size(); ++i) {
        const auto &a = 依据组[i];
        if (!概念树类数据服务::动态依据完整(a.依据, H) ||
            (a.证据状态 != 概念树动态证据状态::已核验 && a.证据状态 != 概念树动态证据状态::历史已清理))
            return false;
        for (std::size_t j = 0; j < i; ++j)
            if (a.依据.记录 == 依据组[j].依据.记录 || a.依据.依据 == 依据组[j].依据.依据)
                return false;
    }
    return true;
}
概念树动态支持核验结果 概念树类数据服务::核验动态支持依据(const 概念树动态支持核验请求 &r) const {
    return 读取封装<概念树动态支持核验结果>(r.读取头, r.预算, [&](auto g, auto h) {
        if (!dynamic_)
            throw 失败{S::入口拒绝};
        auto s = 支持记录(r.支持记录, g, h, r.预算);
        if (s.世界引用.index() != 2)
            throw 失败{S::引用冲突};
        const auto raw = 原始事实(s.世界关系, g);
        const auto *edge = std::get_if<E>(&raw);
        if (!edge)
            throw 失败{S::内部不一致};
        s.动态证据 = 审计动态(std::get<概念树动态引用>(s.世界引用), g, s.证据H, r.预算, !edge->退出事实代次);
        概念树动态支持核验结果 out;
        out.支持 = std::move(s);
        return out;
    });
}
概念树动态依据审计结果 概念树类数据服务::读取动态形成依据(const 概念树动态依据审计请求 &r) const {
    return 读取封装<概念树动态依据审计结果>(r.头, r.预算, [&](auto g, auto h) {
        if (!dynamic_)
            throw 失败{S::入口拒绝};
        const auto f = 读取概念内部(r.概念, g, h, r.预算);
        if (f.是本体根 || f.根角色 != 概念树根角色::动态)
            throw 失败{S::引用冲突};
        概念树动态依据审计结果 out;
        动态历史用量 sourceUsage;
        for (const auto &a : f.动态依据组) {
            auto state = 概念树动态证据状态::已核验;
            if (const auto *d = std::get_if<概念树动态引用>(&a.依据.目标)) {
                const auto raw = 原始事实(a.引用关系, g);
                const auto *edge = std::get_if<E>(&raw);
                if (!edge)
                    throw 失败{S::内部不一致};
                state = 审计动态(*d, g, a.依据.H, r.预算, !edge->退出事实代次, &sourceUsage);
            } else {
                try {
                    验证动态依据(a.依据, g, r.预算);
                } catch (const 失败 &e) {
                    if (e.状态 != S::历史材料不可用)
                        throw;
                    守卫代次(g);
                    state = 概念树动态证据状态::历史已清理;
                }
            }
            out.依据组.push_back({a, state});
        }
        return out;
    });
}

概念树类数据服务::概念树类数据服务(
    const L1事实基座服务 &l1, const 特征类数据服务 &f, const 存在类数据服务 &e,
    const 特征值类数据服务 &v, L1所有者范围写端口 &&port, const 概念树结构交付 &base,
    const 概念树共享结构交付 &extension)
    : 概念树类数据服务(l1, f, e, v, std::move(port), base) {
    shared_ = extension;
    try {
        校验共享扩展(当前代次());
    } catch (const 失败 &x) {
        if (x.状态 == S::资源失败)
            throw std::bad_alloc{};
        throw std::invalid_argument("shared concept extension is invalid");
    }
}

概念树类数据服务::概念树类数据服务(
    const L1事实基座服务 &l1, const 特征类数据服务 &f, const 存在类数据服务 &e,
    const 特征值类数据服务 &v, L1所有者范围写端口 &&port, const 概念树结构交付 &base,
    const 场景类数据服务 &scene, const 动态类数据服务 &dynamics,
    const 概念树动态扩展交付 &dynamicExtension, const 概念树共享结构交付 &extension)
    : 概念树类数据服务(l1, f, e, v, std::move(port), base, scene, dynamics, dynamicExtension) {
    shared_ = extension;
    try {
        校验共享扩展(当前代次());
    } catch (const 失败 &x) {
        if (x.状态 == S::资源失败)
            throw std::bad_alloc{};
        throw std::invalid_argument("shared concept extension is invalid");
    }
}

void 概念树类数据服务::校验共享扩展(std::uint64_t g) const {
    if (!shared_ || shared_->格式 != 2)
        throw 失败{S::入口拒绝};
    std::set<std::uint64_t> used;
    auto unique = [&](稳定编码 id) {
        if (!有效(id) || !used.insert(id.值).second)
            throw 失败{S::引用冲突};
    };
    unique(layout_.格式锚点);
    unique(layout_.签名规则.值);
    for (auto id : 根组()) unique(id.值);
    for (auto id : 类型组()) unique(id);
    if (dynamic_) {
        unique(dynamic_->锚点);
        unique(dynamic_->签名规则.值);
        const auto &d = dynamic_->类型;
        for (auto id : {d.定义场景, d.定义主体, d.时间槽, d.槽模板, d.相对时间,
                        d.来源动态, d.支持动态, d.来源下位概念})
            unique(id);
    }
    const auto &x = *shared_;
    unique(x.锚点);
    const auto anchor = 节点(x.锚点, g, g);
    if (anchor.种类 != 节点种类::普通 || anchor.属性类型表示)
        throw 失败{S::引用冲突};
    const auto av = 属性(x.锚点, g, g);
    if (av.size() != 1 || std::get<std::int64_t>(唯一属性(av, layout_.类型.格式版本).材料) != 2)
        throw 失败{S::引用冲突};
    using R = L1所有者范围值表示种类;
    for (std::size_t i = 0; i < x.类型.size(); ++i) {
        unique(x.类型[i]);
        const auto n = 节点(x.类型[i], g, g);
        std::optional<R> rep;
        switch (i + 1) {
        case 2: case 5: case 15: case 19: rep = R::I64; break;
        case 4: case 10: case 14: rep = R::U64组; break;
        case 6: rep = R::I64组; break;
        default: break;
        }
        if (n.种类 != (rep ? 节点种类::属性类型 : 节点种类::普通) ||
            n.属性类型表示 != rep || !属性(x.类型[i], g, g).empty())
            throw 失败{S::引用冲突};
    }
    const auto registrations = 关系(x.锚点, layout_.类型.类型登记, false, g, g, 21);
    if (registrations.size() != 21)
        throw 失败{S::引用冲突};
    for (std::size_t i = 0; i < registrations.size(); ++i)
        if (registrations[i].角色或顺序 != i + 1 || registrations[i].目标节点 != x.类型[i])
            throw 失败{S::引用冲突};
    守卫代次(g);
}

概念树应用读取结果<旧特征概念审计事实> 概念树类数据服务::读取旧特征概念审计(
    const 概念树应用定义读取请求& r) const {
    return 共享读取封装<旧特征概念审计事实>(r.读取头,r.预算,[&](auto& c) {
        if(r.版本!=2||!有效(r.概念.值))throw 失败{S::入口拒绝};
        if(!shared_||shared_->格式!=2)throw 失败{S::旧格式不支持};
        const auto raw=原始事实(r.概念.值,c.G);const auto* n=std::get_if<N>(&raw);
        if(!n||!n->创建事实代次||n->创建事实代次>c.H)throw 失败{S::未找到};
        if(n->种类!=节点种类::普通||n->属性类型表示)throw 失败{S::引用冲突};
        const auto effective=n->退出事实代次&&*n->退出事实代次<=c.H?*n->退出事实代次-1:c.H;
        const auto& t=layout_.类型;const auto& x=shared_->类型;
        const auto attrs=属性(n->编码,c.G,effective);
        const auto* kind=std::get_if<std::int64_t>(&唯一属性(attrs,t.概念种类).材料);
        const auto* state=std::get_if<std::int64_t>(&唯一属性(attrs,t.生命周期).材料);
        if(attrs.size()!=2||!kind||*kind!=4||!state||*state<1||*state>3)throw 失败{S::旧格式不支持};
        旧特征概念审计事实 out;
        out.C=r.概念;out.本体根=layout_.根组.特征根;out.规则=layout_.签名规则;
        out.格式=2;out.生命周期=投影生命(*n,c.H);out.治理状态=static_cast<概念树生命周期状态>(*state);
        std::set<std::uint64_t> relations,values,nodes;
        auto addRelation=[&](const E& e) {
            if(relations.insert(e.编码.值).second)out.关系记录.push_back(
                {e.编码,e.源节点,e.目标节点,e.角色或顺序,投影生命(e,c.H)});
        };
        auto addNode=[&](稳定编码 id,std::uint64_t h) {
            if(!nodes.insert(id.值).second)return;
            检查数量(nodes.size(),c.预算.基础.最大来源数);
            const auto node=节点(id,c.G,h);
            if(node.种类!=节点种类::普通||node.属性类型表示||node.创建事实代次!=n->创建事实代次)
                throw 失败{S::内部不一致};
            for(const auto& v:属性(id,c.G,h)) {
                if(!values.insert(v.编码.值).second)throw 失败{S::内部不一致};
                检查数量(values.size(),c.预算.基础.最大特征属性数);
                std::variant<std::int64_t,std::vector<std::int64_t>,std::vector<std::uint64_t>> data;
                if(const auto* a=std::get_if<std::int64_t>(&v.材料))data=*a;
                else if(const auto* a=std::get_if<std::vector<std::int64_t>>(&v.材料))data=*a;
                else if(const auto* a=std::get_if<std::vector<std::uint64_t>>(&v.材料))data=*a;
                else throw 失败{S::不支持};
                out.数值记录.push_back({v.编码,v.所属节点,v.属性类型节点,std::move(data),投影生命(v,c.H)});
            }
        };
        addNode(n->编码,effective);
        for(auto type:{t.概念根归属,t.概念规则,t.定义成员}) {
            auto edges=共享关系(n->编码,type,false,n->创建事实代次,c);
            if(edges.size()!=1||edges.front().角色或顺序!=1)throw 失败{S::内部不一致};
            const auto& e=edges.front();addRelation(e);
            if(type==t.概念根归属&&e.目标节点!=out.本体根.值)throw 失败{S::引用冲突};
            if(type==t.概念规则&&e.目标节点!=out.规则.值)throw 失败{S::引用冲突};
            if(type==t.定义成员)out.定义记录=e.目标节点;
        }
        addNode(out.定义记录,n->创建事实代次);
        const auto dv=属性(out.定义记录,c.G,n->创建事实代次);
        if(dv.size()!=1)throw 失败{S::旧格式不支持};
        const auto* form=std::get_if<std::int64_t>(&唯一属性(dv,x[1]).材料);
        if(!form||*form<1||*form>3)throw 失败{S::旧格式不支持};
        for(auto type:{x[0],t.定义特征类型}) {
            const auto edges=共享关系(out.定义记录,type,false,n->创建事实代次,c);
            if(edges.size()!=1)throw 失败{S::内部不一致};
            addRelation(edges.front());
        }
        for(const auto& e:共享关系(out.定义记录,x[2],false,n->创建事实代次,c)) {
            addRelation(e);addNode(e.目标节点,n->创建事实代次);
        }
        for(const auto& m:共享关系(n->编码,t.来源成员,false,n->创建事实代次,c)) {
            addRelation(m);addNode(m.目标节点,n->创建事实代次);
            for(auto type:{x[6],x[16],x[17],x[19],x[20]})
                for(const auto& e:共享关系(m.目标节点,type,false,n->创建事实代次,c))addRelation(e);
        }
        for(const auto& e:共享关系(n->编码,t.直接上位,true,effective,c))addRelation(e);
        std::sort(out.关系记录.begin(),out.关系记录.end(),[](const auto& a,const auto& b){return a.身份.值<b.身份.值;});
        std::sort(out.数值记录.begin(),out.数值记录.end(),[](const auto& a,const auto& b){return a.身份.值<b.身份.值;});
        return out;
    });
}
概念树类数据服务::概念树类数据服务(const L1事实基座服务& l1,const 特征类数据服务& f,
    const 存在类数据服务& e,const 特征值类数据服务& v,L1所有者范围写端口&& port,
    const 概念树结构交付& base,const 特征概念结构交付& x)
    :概念树类数据服务(l1,f,e,v,std::move(port),base) {
    feature_layout_=x;
    try{校验特征扩展(当前代次());}
    catch(const 失败& e){if(e.状态==S::资源失败)throw std::bad_alloc{};throw std::invalid_argument("invalid feature concept layout");}
}
概念树类数据服务::概念树类数据服务(const L1事实基座服务& l1,const 特征类数据服务& f,
    const 存在类数据服务& e,const 特征值类数据服务& v,L1所有者范围写端口&& port,
    const 概念树结构交付& base,const 场景类数据服务& scene,const 动态类数据服务& dynamics,
    const 概念树动态扩展交付& d,const 特征概念结构交付& x)
    :概念树类数据服务(l1,f,e,v,std::move(port),base,scene,dynamics,d) {
    feature_layout_=x;
    try{校验特征扩展(当前代次());}
    catch(const 失败& e){if(e.状态==S::资源失败)throw std::bad_alloc{};throw std::invalid_argument("invalid feature concept layout");}
}
概念树类数据服务::概念树类数据服务(const L1事实基座服务& l1,const 特征类数据服务& f,
    const 存在类数据服务& e,const 特征值类数据服务& v,L1所有者范围写端口&& port,
    const 概念树结构交付& base,const 特征概念结构交付& x,const 概念树存在扩展交付& ec)
    :概念树类数据服务(l1,f,e,v,std::move(port),base,x) {
    existence_layout_=ec;
    try{校验存在扩展(当前代次());}
    catch(const 失败& error){if(error.状态==S::资源失败)throw std::bad_alloc{};
        throw std::invalid_argument("invalid existence concept extension");}
}
概念树类数据服务::概念树类数据服务(const L1事实基座服务& l1,const 特征类数据服务& f,
    const 存在类数据服务& e,const 特征值类数据服务& v,L1所有者范围写端口&& port,
    const 概念树结构交付& base,const 场景类数据服务& scene,const 动态类数据服务& dynamics,
    const 概念树动态扩展交付& d,const 特征概念结构交付& x,const 概念树存在扩展交付& ec)
    :概念树类数据服务(l1,f,e,v,std::move(port),base,scene,dynamics,d,x) {
    existence_layout_=ec;
    try{校验存在扩展(当前代次());}
    catch(const 失败& error){if(error.状态==S::资源失败)throw std::bad_alloc{};
        throw std::invalid_argument("invalid existence concept extension");}
}
void 概念树类数据服务::校验存在扩展(std::uint64_t g) const {
    if(!feature_layout_||!existence_layout_)throw 失败{S::不支持};
    const auto id=existence_layout_->抽象依据目标关系类型;
    if(!有效(id))throw 失败{S::入口拒绝};
    const auto distinct=[&](稳定编码 other){if(id==other)throw 失败{S::引用冲突};};
    distinct(layout_.格式锚点);distinct(layout_.签名规则.值);
    for(auto root:根组())distinct(root.值);
    for(auto type:类型组())distinct(type);
    distinct(feature_layout_->锚点);
    for(auto type:feature_layout_->类型)distinct(type);
    if(dynamic_) {
        distinct(dynamic_->锚点);distinct(dynamic_->签名规则.值);
        const auto& d=dynamic_->类型;
        for(auto type:{d.定义场景,d.定义主体,d.时间槽,d.槽模板,d.相对时间,d.来源动态,d.支持动态,d.来源下位概念})distinct(type);
    }
    const auto n=节点(id,g,g);
    if(n.种类!=节点种类::普通||n.属性类型表示||!属性(id,g,g).empty())throw 失败{S::引用冲突};
    守卫代次(g);
}
void 概念树类数据服务::校验特征扩展(std::uint64_t g) const {
    if(!feature_layout_||feature_layout_->格式!=1)throw 失败{S::入口拒绝};
    const auto& x=*feature_layout_;std::set<std::uint64_t> ids;
    auto unique=[&](稳定编码 id){if(!有效(id)||!ids.insert(id.值).second)throw 失败{S::引用冲突};};
    unique(layout_.格式锚点);unique(layout_.签名规则.值);
    for(auto root:根组())unique(root.值);for(auto t:类型组())unique(t);
    if(dynamic_) {
        unique(dynamic_->锚点);unique(dynamic_->签名规则.值);const auto& d=dynamic_->类型;
        for(auto id:{d.定义场景,d.定义主体,d.时间槽,d.槽模板,d.相对时间,d.来源动态,d.支持动态,d.来源下位概念})unique(id);
    }
    unique(x.锚点);const auto anchor=节点(x.锚点,g,g);const auto av=属性(x.锚点,g,g);
    const auto* format=std::get_if<std::int64_t>(&唯一属性(av,layout_.类型.格式版本).材料);
    if(anchor.种类!=节点种类::普通||anchor.属性类型表示||av.size()!=1||!format||*format!=1)throw 失败{S::引用冲突};
    using R=L1所有者范围值表示种类;
    for(std::size_t i=0;i<x.类型.size();++i) {
        unique(x.类型[i]);const auto n=节点(x.类型[i],g,g);std::optional<R> rep;
        switch(i+1) {
        case 5:case 7:case 18:case 23:rep=R::U64组;break;
        case 6:case 10:case 11:case 24:rep=R::I64;break;
        default:break;
        }
        if(n.种类!=(rep?节点种类::属性类型:节点种类::普通)||n.属性类型表示!=rep
            ||!属性(n.编码,g,g).empty())throw 失败{S::引用冲突};
    }
    const auto es=关系(x.锚点,layout_.类型.类型登记,false,g,g,24);
    if(es.size()!=24)throw 失败{S::引用冲突};
    for(std::size_t i=0;i<es.size();++i)
        if(es[i].目标节点!=x.类型[i]||es[i].角色或顺序!=i+1)throw 失败{S::引用冲突};
    守卫代次(g);
}

void 概念树类数据服务::特征预算有效(const 特征概念预算& b) {
    预算有效(b.基础);
    if (!b.最大观察数 || !b.最大区间数 || !b.最大命中数 || !b.最大名称数 || !b.最大首次材料项数)
        throw 失败{S::入口拒绝};
}
概念树共享预算 概念树类数据服务::组合共享预算(const 特征概念预算& b) {
    return {b.基础, b, b.最大区间数, b.基础.最大来源数, b.最大名称数, b.最大首次材料项数};
}
void 概念树类数据服务::共享预算有效(const 概念树共享预算& b) {
    预算有效(b.基础); 特征预算有效(b.特征);
    if (!b.最大用途数 || !b.最大名称数 || !b.最大首次材料项数) throw 失败{S::入口拒绝};
}
概念树数据状态 概念树类数据服务::共享映射(特征数据错误 s) {
    switch (s) {
    case 特征数据错误::未找到: return S::未找到;
    case 特征数据错误::未设置: case 特征数据错误::规则缺失: return S::规则缺失;
    case 特征数据错误::入口拒绝: return S::入口拒绝;
    case 特征数据错误::类型不相容: return S::类型不相容;
    case 特征数据错误::能力未提供: return S::不支持;
    case 特征数据错误::旧格式不支持: return S::旧格式不支持;
    case 特征数据错误::已退出: return S::目标已退出;
    case 特征数据错误::历史材料不可用: return S::历史材料不可用;
    case 特征数据错误::引用冲突: return S::引用冲突;
    case 特征数据错误::算术不可表示: return S::差异不可表示;
    case 特征数据错误::并发变化: return S::事实代次漂移;
    case 特征数据错误::资源失败: return S::资源失败;
    case 特征数据错误::幂等冲突: return S::幂等冲突;
    case 特征数据错误::数量预算不足: return S::数量预算不足;
    case 特征数据错误::前次写入待收敛: return S::前次写入待收敛;
    default: return S::内部不一致;
    }
}
void 概念树类数据服务::专项计数(std::set<std::pair<std::uint64_t,std::uint64_t>>& seen,
    稳定编码 id, std::uint64_t h, std::uint64_t limit) {
    if (seen.contains({id.值,h})) return;
    if (seen.size() >= limit) throw 失败{S::数量预算不足};
    seen.insert({id.值,h});
}
特征概念定义 概念树类数据服务::规范共享定义(const 特征概念定义& d, 共享读取上下文& c) const {
    auto r = 特征结果(features_.规范化I64特征域({{1,c.G,c.H,d.类型},d.域}));
    if (r.Gread != c.G || r.H != c.H) throw 失败{S::内部不一致};
    检查数量(r.数据.区间.size(),c.预算.特征.最大区间数);
    return {d.类型,std::move(r.数据)};
}
bool 概念树类数据服务::共享包含(const 特征概念定义& a,const 特征概念定义& b,共享读取上下文& c) const {
    if (a.类型 != b.类型) return false;
    const auto r = 特征结果(features_.判定I64域包含({{1,c.G,c.H,a.类型},a.域,b.域}));
    if (r.Gread != c.G || r.H != c.H) throw 失败{S::内部不一致};
    return r.数据;
}
std::vector<概念树类数据服务::E> 概念树类数据服务::共享关系(
    稳定编码 id,稳定编码 type,bool incoming,std::uint64_t h,共享读取上下文& c) const {
    auto out = 关系(id,type,incoming,c.G,h,c.预算.基础.最大关系数);
    for (const auto& e : out) 专项计数(c.关系计数,e.编码,h,c.预算.基础.最大关系数);
    c.关系数 = c.关系计数.size(); c.用量.关系数 = c.关系数;
    return out;
}
void 概念树类数据服务::核验外部节点(稳定编码 id,std::uint64_t g,std::uint64_t h) const {
    const auto r = l1_.读取所有者范围历史事实({L1所有者范围CRUD合同版本,id});
    if (r.读取事实代次 != g) throw 失败{S::事实代次漂移};
    if (r.状态 != L1所有者范围读取状态::成功) throw 失败{映射(r.状态)};
    const auto* n = r.事实 ? std::get_if<N>(&*r.事实) : nullptr;
    if (r.合同版本 != L1所有者范围CRUD合同版本 || r.查询编码 != id || !n || n->编码 != id)
        throw 失败{S::引用冲突};
    if (!n->创建事实代次 || n->创建事实代次 > g) throw 失败{S::内部不一致};
    if (n->创建事实代次 > h) throw 失败{S::未找到};
    if (!有效于(*n,h)) throw 失败{S::目标已退出};
}
类型观察事实 概念树类数据服务::观察事实(稳定编码 id,共享读取上下文& c) const {
    if (!feature_layout_) throw 失败{S::不支持};
    专项计数(c.观察计数,id,c.H,c.预算.特征.最大观察数);
    c.用量.观察数 = c.观察计数.size();
    const auto& x = feature_layout_->类型;
    const auto n = 节点(id,c.G,c.H);
    if (n.种类 != 节点种类::普通 || n.属性类型表示) throw 失败{S::引用冲突};
    auto pair = [&](const auto& f) {
        if (f.创建事实代次 != n.创建事实代次 || f.退出事实代次 != n.退出事实代次) throw 失败{S::内部不一致};
    };
    const auto av = 属性(id,c.G,c.H);
    if (av.size()!=3) throw 失败{S::内部不一致};
    for (const auto& v:av) pair(v);
    auto u64 = [&](稳定编码 type) {
        const auto* v=std::get_if<std::vector<std::uint64_t>>(&唯一属性(av,type).材料);
        if (!v || v->size()!=1 || !v->front()) throw 失败{S::内部不一致};
        return v->front();
    };
    auto one=[&](稳定编码 type) {
        auto es=共享关系(id,type,false,c.H,c);
        if(es.size()!=1 || es.front().角色或顺序!=1) throw 失败{S::内部不一致};
        pair(es.front()); return es.front();
    };
    const auto own=one(x[0]), ft=one(x[1]), f=one(x[2]), source=one(x[3]);
    const auto* time=std::get_if<std::int64_t>(&唯一属性(av,x[5]).材料);
    const auto seq=u64(x[4]), h=u64(x[6]);
    if (own.目标节点!=feature_layout_->锚点 || !time || h>=n.创建事实代次) throw 失败{S::内部不一致};
    const auto exact=特征结果(features_.读取准确特征事实({1,c.G,h,{f.目标节点}}));
    if(exact.Gread!=c.G || exact.H!=h || exact.信息.类型.编码!=ft.目标节点) throw 失败{S::引用冲突};
    核验外部节点(source.目标节点,c.G,h);
    return {id,{{{ft.目标节点},source.目标节点,seq},{f.目标节点},*time},h};
}
std::vector<类型观察事实> 概念树类数据服务::观察组(特征类型身份 ft,共享读取上下文& c) const {
    if(!feature_layout_ || !有效(ft)) throw 失败{S::入口拒绝};
    const auto es=共享关系(ft.编码,feature_layout_->类型[1],true,c.H,c);
    std::vector<类型观察事实> out;
    for(const auto& e:es) {
        auto f=观察事实(e.源节点,c);
        if(f.输入.键.类型!=ft) throw 失败{S::内部不一致};
        if(std::any_of(out.begin(),out.end(),[&](const auto& x){return x.输入.键==f.输入.键;}))
            throw 失败{S::内部不一致};
        out.push_back(std::move(f));
    }
    std::sort(out.begin(),out.end(),[](const auto& a,const auto& b){return a.记录.值<b.记录.值;});
    return out;
}
概念树类数据服务::共享展开 概念树类数据服务::展开共享定义(概念树概念身份 id,共享读取上下文& c) const {
    if(!feature_layout_) throw 失败{S::不支持};
    const auto path=std::pair{id.值.值,c.H};
    if(!c.定义路径.insert(path).second) throw 失败{S::形成环};
    struct Pop { decltype(c.定义路径)& set; decltype(path) key; ~Pop(){set.erase(key);} } pop{c.定义路径,path};
    专项计数(c.概念计数,id.值,c.H,c.预算.基础.最大概念数);
    c.用量.概念数=c.概念计数.size();
    const auto& t=layout_.类型; const auto& x=feature_layout_->类型;
    const auto n=节点(id.值,c.G,c.H);
    if(n.种类!=节点种类::普通 || n.属性类型表示) throw 失败{S::引用冲突};
    const auto av=属性(id.值,c.G,c.H);
    const auto* kind=std::get_if<std::int64_t>(&唯一属性(av,t.概念种类).材料);
    const auto& lv=唯一属性(av,t.生命周期);
    const auto* state=std::get_if<std::int64_t>(&lv.材料);
    if(av.size()!=2 || !kind || *kind!=4 || !state || *state<1 || *state>3) throw 失败{S::旧格式不支持};
    auto paired=[&](const auto& v) {
        if(v.创建事实代次!=n.创建事实代次 || v.退出事实代次!=n.退出事实代次) throw 失败{S::内部不一致};
    };
    auto one=[&](稳定编码 a,稳定编码 type,std::uint64_t h) {
        auto es=共享关系(a,type,false,h,c);
        if(es.size()!=1 || es.front().角色或顺序!=1) throw 失败{S::旧格式不支持};
        return es.front();
    };
    共享展开 out; auto& f=out.事实;
    f.身份=id; f.本体根=layout_.根组.特征根; f.规则=layout_.签名规则;
    f.生命周期=投影生命(n,c.H); f.生命周期值事实=lv.编码;
    f.治理状态=static_cast<概念树生命周期状态>(*state);
    out.自有事实={id.值,唯一属性(av,t.概念种类).编码,lv.编码};
    paired(唯一属性(av,t.概念种类));
    const auto root=one(id.值,t.概念根归属,c.H),rule=one(id.值,t.概念规则,c.H),def=one(id.值,t.定义成员,c.H);
    if(root.目标节点!=f.本体根.值 || rule.目标节点!=f.规则.值) throw 失败{S::引用冲突};
    for(const auto& e:{root,rule,def}){paired(e);out.自有事实.push_back(e.编码);}
    const auto dn=节点(def.目标节点,c.G,c.H);paired(dn);
    if(dn.种类!=节点种类::普通 || dn.属性类型表示 || !属性(dn.编码,c.G,c.H).empty()) throw 失败{S::旧格式不支持};
    f.定义记录=dn.编码;out.自有事实.push_back(dn.编码);
    const auto ft=one(dn.编码,x[7],c.H);paired(ft);
    f.类型关系=ft.编码;f.定义.类型={ft.目标节点};out.自有事实.push_back(ft.编码);
    auto intervals=共享关系(dn.编码,x[8],false,c.H,c);
    if(intervals.empty()) throw 失败{S::内部不一致};
    for(const auto& e:intervals) {
        paired(e);专项计数(c.区间计数,e.目标节点,c.H,c.预算.特征.最大区间数);
        const auto in=节点(e.目标节点,c.G,c.H);paired(in);
        const auto vs=属性(in.编码,c.G,c.H);
        if(in.种类!=节点种类::普通 || in.属性类型表示 || vs.size()!=2
            || e.角色或顺序!=f.区间事实.size()+1) throw 失败{S::内部不一致};
        const auto& a=唯一属性(vs,x[9]);const auto& b=唯一属性(vs,x[10]);paired(a);paired(b);
        const auto* lo=std::get_if<std::int64_t>(&a.材料);const auto* hi=std::get_if<std::int64_t>(&b.材料);
        if(!lo || !hi || *lo>*hi) throw 失败{S::内部不一致};
        f.定义.域.区间.push_back({*lo,*hi});
        f.区间事实.push_back({in.编码,e.编码,a.编码,b.编码,{*lo,*hi},投影生命(in,c.H)});
        out.自有事实.insert(out.自有事实.end(),{in.编码,e.编码,a.编码,b.编码});
    }
    c.用量.区间数=c.区间计数.size();
    if(规范共享定义(f.定义,c)!=f.定义) throw 失败{S::内部不一致};
    特征规范I64域 combined;
    const auto members=共享关系(id.值,x[15],false,c.H,c);
    if(members.empty()) throw 失败{S::内部不一致};
    for(const auto& e:members) {
        paired(e);专项计数(c.来源计数,e.目标节点,c.H,c.预算.基础.最大来源数);
        const auto sn=节点(e.目标节点,c.G,c.H);paired(sn);
        const auto vs=属性(sn.编码,c.G,c.H);
        if(sn.种类!=节点种类::普通 || sn.属性类型表示 || vs.size()!=1
            || e.角色或顺序!=f.来源事实.size()+1) throw 失败{S::内部不一致};
        const auto& hv=唯一属性(vs,x[17]);paired(hv);
        const auto* times=std::get_if<std::vector<std::uint64_t>>(&hv.材料);
        if(!times || times->size()!=1 || !times->front() || times->front()>=n.创建事实代次) throw 失败{S::内部不一致};
        const auto edges=共享关系(sn.编码,x[16],false,n.创建事实代次,c);
        if(edges.size()!=1) throw 失败{S::内部不一致};
        const auto& target=edges.front();
        if(target.创建事实代次!=n.创建事实代次 || target.角色或顺序<1 || target.角色或顺序>3) throw 失败{S::内部不一致};
        特征概念来源事实 source{sn.编码,e.编码,target.编码,hv.编码,特征类型身份{},times->front(),
            投影生命(sn,c.H),!有效于(target,c.H)};
        特征规范I64域 d;
        {
            共享截止作用域 scope(c,source.证据H);
            if(target.角色或顺序==1) {
                source.来源=特征类型身份{target.目标节点};
                if(target.目标节点!=f.定义.类型.编码 || members.size()!=1) throw 失败{S::引用冲突};
                d=特征结果(features_.读取I64类型完整域({1,c.G,c.H,f.定义.类型})).数据;
            } else if(target.角色或顺序==2) {
                auto o=观察事实(target.目标节点,c);source.来源=o;
                if(o.输入.键.类型!=f.定义.类型 || members.size()!=1) throw 失败{S::引用冲突};
                d=特征结果(features_.形成I64特征域({1,c.G,c.H,o.输入.F})).域;
            } else {
                const 概念树概念身份 prior{target.目标节点};source.来源=prior;
                const auto fact=展开共享定义(prior,c).事实;
                if(fact.定义.类型!=f.定义.类型) throw 失败{S::类型不相容};
                d=fact.定义.域;
            }
        }
        combined.区间.insert(combined.区间.end(),d.区间.begin(),d.区间.end());
        检查数量(combined.区间.size(),c.预算.特征.最大区间数);
        f.来源事实.push_back(std::move(source));
        out.自有事实.insert(out.自有事实.end(),{sn.编码,e.编码,hv.编码});
        if(有效于(target,c.H))out.自有事实.push_back(target.编码);
    }
    c.用量.来源数=c.来源计数.size();
    if(规范共享定义({f.定义.类型,std::move(combined)},c)!=f.定义) throw 失败{S::内部不一致};
    const auto full=特征结果(features_.读取I64类型完整域({1,c.G,c.H,f.定义.类型})).数据;
    const bool isRoot=full==f.定义.域;
    const auto parents=共享关系(id.值,t.直接上位,true,c.H,c);
    if(parents.empty())throw 失败{S::内部不一致};
    for(const auto& e:parents) {
        if(e.角色或顺序!=1 || e.源节点==id.值)throw 失败{S::形成环};
        const 概念树概念身份 parent{e.源节点};
        if(isRoot) {
            if(parents.size()!=1 || parent!=f.本体根)throw 失败{S::引用冲突};
            f.类型根=id;
        } else {
            if(parent==f.本体根)throw 失败{S::引用冲突};
            const auto p=展开共享定义(parent,c).事实;
            if(p.定义==f.定义 || !共享包含(p.定义,f.定义,c))throw 失败{S::引用冲突};
            if(有效(f.类型根.值) && f.类型根!=p.类型根)throw 失败{S::引用冲突};
            f.类型根=p.类型根;
        }
        f.直接上位.push_back({e.编码,parent,id,投影生命(e,c.H)});
        out.自有事实.push_back(e.编码);
    }
    std::sort(out.自有事实.begin(),out.自有事实.end(),编码小于);
    if(std::adjacent_find(out.自有事实.begin(),out.自有事实.end())!=out.自有事实.end())throw 失败{S::内部不一致};
    return out;
}

概念树应用定义事实 概念树类数据服务::应用定义(概念树概念身份 id, 共享读取上下文 &c) const {
    const auto roots = 根组();
    if (std::find(roots.begin(), roots.end(), id) == roots.end()) {
        const auto av = 属性(id.值, c.G, c.H);
        const auto kind = std::get<std::int64_t>(唯一属性(av, layout_.类型.概念种类).材料);
        if (kind == 4)
            return 展开共享定义(id, c).事实;
        if (kind == 2)
            return 展开存在定义_v2(id, c).事实;
    }
    return 展开概念(id, c.G, c.H, c.预算.基础, false, true, &c).事实;
}

概念树应用读取结果<概念树应用定义事实> 概念树类数据服务::读取应用概念定义(
    const 概念树应用定义读取请求 &r) const {
    return 共享读取封装<概念树应用定义事实>(r.读取头, r.预算, [&](auto &c) {
        if (r.版本 != 2) throw 失败{S::入口拒绝};
        return 应用定义(r.概念, c);
    });
}

概念树应用图事实 概念树类数据服务::应用图(概念树根角色 role, bool support,
                                              共享读取上下文 &c) const {
    概念树应用图事实 out;
    out.根身份 = 根(role);
    out.节点组.push_back(应用定义(out.根身份, c));
    const auto members = 共享关系(out.根身份.值, layout_.类型.概念根归属, true, c.H, c);
    检查数量(members.size() + 1, c.预算.基础.最大概念数);
    for (const auto &e : members) {
        if (e.角色或顺序 != 1) throw 失败{S::内部不一致};
        auto fact = 应用定义(概念树概念身份{e.源节点}, c);
        if (std::visit([&](const auto &f) { return f.本体根 != out.根身份; }, fact))
            throw 失败{S::内部不一致};
        std::visit([&](const auto &f) {
            const auto& parents = [&]() -> const std::vector<概念树直接上位事实>& {
                if constexpr (std::is_same_v<std::decay_t<decltype(f)>,特征概念事实>) return f.直接上位;
                else return f.直接上位组;
            }();
            out.直接边组.insert(out.直接边组.end(), parents.begin(), parents.end());
        }, fact);
        检查数量(out.直接边组.size(), c.预算.基础.最大关系数);
        out.节点组.push_back(std::move(fact));
    }
    std::map<std::uint64_t, std::size_t> degrees;
    std::map<std::uint64_t, std::vector<std::uint64_t>> children;
    std::set<std::pair<std::uint64_t, std::uint64_t>> endpoints;
    std::set<std::uint64_t> edgeIds;
    for (const auto &f : out.节点组) {
        const auto id = 应用身份(f).值.值;
        if (!degrees.emplace(id, 0).second) throw 失败{S::内部不一致};
    }
    for (const auto &e : out.直接边组) {
        const auto p = e.上位.值.值, child = e.下位.值.值;
        if (p == child || !degrees.contains(p) || !degrees.contains(child) ||
            !endpoints.emplace(p, child).second || !edgeIds.insert(e.关系.值).second)
            throw 失败{S::内部不一致};
        ++degrees[child]; children[p].push_back(child);
    }
    // 从完整根归属范围反查出边，不能只沿可达边遍历而遗漏坏边或孤儿。
    for (const auto &[id, degree] : degrees) {
        for (const auto &e : 共享关系(稳定编码{id}, layout_.类型.直接上位, false, c.H, c))
            if (!edgeIds.contains(e.编码.值) || !degrees.contains(e.目标节点.值) || e.角色或顺序 != 1)
                throw 失败{S::内部不一致};
        if ((id == out.根身份.值.值 && degree) || (id != out.根身份.值.值 && !degree))
            throw 失败{S::内部不一致};
    }
    std::vector<std::uint64_t> ready{out.根身份.值.值};
    for (std::size_t i = 0; i < ready.size(); ++i)
        for (auto child : children[ready[i]]) if (--degrees[child] == 0) ready.push_back(child);
    if (ready.size() != degrees.size()) throw 失败{S::内部不一致};
    std::sort(out.节点组.begin(), out.节点组.end(), [](const auto &a, const auto &b) {
        return 应用身份(a).值.值 <
               应用身份(b).值.值;
    });
    std::sort(out.直接边组.begin(), out.直接边组.end(), [](const auto &a, const auto &b) {
        if (a.上位 != b.上位) return a.上位.值.值 < b.上位.值.值;
        if (a.下位 != b.下位) return a.下位.值.值 < b.下位.值.值;
        return a.关系.值 < b.关系.值;
    });
    if (support) {
        out.支持组.emplace();
        for (const auto &f : out.节点组) {
            const auto id = 应用身份(f);
            auto group = 支持组(id, std::nullopt, c.G, c.H, c.预算.基础, &c);
            if (group.size() > c.预算.基础.最大支持数 - out.支持组->size())
                throw 失败{S::数量预算不足};
            out.支持组->insert(out.支持组->end(), group.begin(), group.end());
        }
        std::sort(out.支持组->begin(), out.支持组->end(), [](const auto &a, const auto &b) {
            if (a.世界引用.index() != b.世界引用.index()) return a.世界引用.index() < b.世界引用.index();
            if (世界编码(a.世界引用) != 世界编码(b.世界引用)) return 世界编码(a.世界引用).值 < 世界编码(b.世界引用).值;
            if (a.目标概念 != b.目标概念) return a.目标概念.值.值 < b.目标概念.值.值;
            return a.记录.值 < b.记录.值;
        });
    }
    return out;
}

概念树应用读取结果<概念树应用图事实> 概念树类数据服务::读取应用概念图(
    const 概念树应用图读取请求 &r) const {
    return 共享读取封装<概念树应用图事实>(r.图请求.读取头, r.预算, [&](auto &c) {
        if (r.版本 != 2) throw 失败{S::入口拒绝};
        return 应用图(r.图请求.根角色, r.图请求.包含支持, c);
    });
}

概念树应用模板事实 概念树类数据服务::应用模板(概念树概念身份 id, 共享读取上下文 &c) const {
    auto definition = 应用定义(id, c);
    if (auto *shared = std::get_if<特征概念事实>(&definition)) return std::move(*shared);
    const auto* generic = std::get_if<概念树概念事实>(&definition);
    if (!generic) throw 失败{S::引用冲突};
    const auto &old = *generic;
    if (old.是本体根 || !old.定义 || !std::holds_alternative<概念树特征定义>(*old.定义))
        throw 失败{S::引用冲突};
    return 动态模板(id, c.G, c.H, c.预算.基础);
}

概念树应用读取结果<概念树应用模板事实> 概念树类数据服务::读取应用特征模板(
    const 概念树应用模板读取请求 &r) const {
    return 共享读取封装<概念树应用模板事实>(r.读取头, r.预算, [&](auto &c) {
        if (r.版本 != 2) throw 失败{S::入口拒绝};
        return 应用模板(r.模板, c);
    });
}

概念树应用读取结果<std::vector<类型观察事实>> 概念树类数据服务::读取类型观察(const 类型观察读取请求& r) const {
    return 共享读取封装<std::vector<类型观察事实>>(r.读取头,组合共享预算(r.预算),[&](auto& c) {
        const auto ft=std::visit([](const auto& choice) {
            if constexpr(std::is_same_v<std::decay_t<decltype(choice)>,类型观察键>)return choice.类型;
            else return choice;
        },r.选择);
        auto out=观察组(ft,c);
        if(const auto* key=std::get_if<类型观察键>(&r.选择)) {
            if(!有效(key->来源)||!key->序号)throw 失败{S::入口拒绝};
            out.erase(std::remove_if(out.begin(),out.end(),[&](const auto& o){return o.输入.键!=*key;}),out.end());
        }
        return out;
    });
}
概念树应用读取结果<std::vector<特征概念事实>> 概念树类数据服务::查找特征概念(const 特征概念查找请求& r) const {
    return 共享读取封装<std::vector<特征概念事实>>(r.读取头,组合共享预算(r.预算),[&](auto& c) {
        if(!feature_layout_)throw 失败{S::不支持};
        const auto ft=std::visit([](const auto& choice) {
            if constexpr(std::is_same_v<std::decay_t<decltype(choice)>,特征概念定义>)return choice.类型;
            else return choice;
        },r.选择);
        if(!有效(ft))throw 失败{S::入口拒绝};
        std::optional<特征概念定义> exact;
        if(const auto* d=std::get_if<特征概念定义>(&r.选择))exact=规范共享定义(*d,c);
        const auto graph=应用图(概念树根角色::特征,false,c);
        std::vector<特征概念事实> out;
        for(const auto& node:graph.节点组)
            if(const auto* f=std::get_if<特征概念事实>(&node);f&&f->定义.类型==ft&&(!exact||f->定义==*exact))
                out.push_back(*f);
        if(exact&&out.size()>1)throw 失败{S::内部不一致};
        return out;
    });
}
概念树应用模板判定事实 概念树类数据服务::应用判定(
    概念树概念身份 id,const 准确特征读取请求& actual,共享读取上下文& c) const {
    if(actual.合同版本!=1||actual.Gread!=c.G||!actual.H||actual.H>c.G||!有效(actual.身份))throw 失败{S::入口拒绝};
    概念树应用模板判定事实 out;out.模板=应用模板(id,c);
    auto f=特征结果(features_.读取准确特征事实(actual));
    if(f.Gread!=c.G||f.H!=actual.H||f.信息.身份!=actual.身份)throw 失败{S::内部不一致};
    专项计数(c.特征计数,actual.身份.编码,actual.H,c.预算.基础.最大特征属性数);
    c.用量.特征属性数=c.特征计数.size();
    特征规范I64域 domain;
    if(const auto* d=std::get_if<特征概念事实>(&out.模板)) {
        if(d->定义.类型!=f.信息.类型)throw 失败{S::类型不相容};
        domain=d->定义.域;
    } else {
        const auto& dynamicDef=std::get<概念树动态模板事实>(out.模板).定义;
        if(dynamicDef.特征类型.值!=f.信息.类型.编码)throw 失败{S::类型不相容};
        if(const auto* range=std::get_if<概念树I64区间>(&dynamicDef.值域))domain.区间.push_back({range->下界,range->上界});
        else {
            const auto* value=std::get_if<std::int64_t>(&std::get<概念树精确值>(dynamicDef.值域));
            if(!value)throw 失败{S::不支持};
            domain.区间.push_back({*value,*value});
        }
    }
    const auto result=特征结果(features_.判定准确特征命中域({actual,domain}));
    if(result.Gread!=c.G||result.H!=actual.H)throw 失败{S::内部不一致};
    out.适用=result.数据;out.判定={c.G,c.H,actual.H,std::move(f),std::move(domain),result.数据};
    out.用量=c.用量;return out;
}
概念树应用读取结果<概念树应用模板判定事实> 概念树类数据服务::判定应用特征模板(const 概念树应用模板判定请求& r) const {
    return 共享读取封装<概念树应用模板判定事实>(r.读取头,r.预算,[&](auto& c) {
        if(r.版本!=2)throw 失败{S::入口拒绝};
        return 应用判定(r.模板,r.实际,c);
    });
}

std::vector<特征名称事实> 概念树类数据服务::名称组(
    std::variant<特征信息身份,概念树概念身份> choice,共享读取上下文& c) const {
    if(!feature_layout_)throw 失败{S::不支持};
    const bool incoming=std::holds_alternative<概念树概念身份>(choice);
    const auto id=std::visit([](const auto& v) {
        if constexpr(std::is_same_v<std::decay_t<decltype(v)>,特征信息身份>)return v.编码;
        else return v.值;
    },choice);
    if(!有效(id))throw 失败{S::入口拒绝};
    const auto es=共享关系(id,feature_layout_->类型[11],incoming,c.H,c);
    if(!incoming&&es.size()>1)throw 失败{S::内部不一致};
    std::vector<特征名称事实> out;
    for(const auto& e:es) {
        if(e.角色或顺序!=1)throw 失败{S::内部不一致};
        专项计数(c.名称计数,e.编码,c.H,c.预算.特征.最大名称数);
        const auto decision=应用判定(概念树概念身份{e.目标节点},{1,c.G,c.H,{e.源节点}},c);
        if(!decision.适用)throw 失败{S::引用冲突};
        out.push_back({e.编码,{e.源节点},概念树概念身份{e.目标节点},{e.创建事实代次,e.退出事实代次}});
    }
    c.用量.名称数=c.名称计数.size();return out;
}
std::vector<特征名称事实> 概念树类数据服务::名称窄读(稳定编码 id,bool incoming,
    std::uint64_t g,std::uint64_t h,const 概念树预算& b) const {
    if(!feature_layout_)throw 失败{S::不支持};
    if(!有效(id))throw 失败{S::入口拒绝};
    // 只核名称关系和端点节点；不展开值域、观察、命中、用途或首次材料。
    const auto conceptNode=[&](稳定编码 c) {
        const auto n=节点(c,g,h);
        if(n.种类!=节点种类::普通||n.属性类型表示)throw 失败{S::引用冲突};
    };
    if(incoming){检查数量(1,b.最大概念数);conceptNode(id);}
    else{检查数量(1,b.最大世界成员数);核验外部节点(id,g,h);}
    const auto relations=关系(id,feature_layout_->类型[11],incoming,g,h,b.最大关系数);
    if(!incoming&&relations.size()>1)throw 失败{S::内部不一致};
    std::set<std::uint64_t> features;
    std::vector<特征名称事实> out;
    for(const auto& e:relations) {
        if(e.角色或顺序!=1)throw 失败{S::内部不一致};
        if(incoming) {
            if(features.insert(e.源节点.值).second) {
                检查数量(features.size(),b.最大世界成员数);
                核验外部节点(e.源节点,g,h);
            }
        } else conceptNode(e.目标节点);
        out.push_back({e.编码,{e.源节点},概念树概念身份{e.目标节点},
            {e.创建事实代次,e.退出事实代次}});
    }
    std::sort(out.begin(),out.end(),[](const auto& a,const auto& b){return a.关系.值<b.关系.值;});
    return out;
}
概念树应用读取结果<std::optional<特征名称事实>> 概念树类数据服务::读取特征当前名称(
    const 特征名称按特征读取请求& r) const {
    using R=概念树应用读取结果<std::optional<特征名称事实>>;
    return 读取封装<R>(r.读取头,r.预算,[&](auto g,auto h) {
        if(r.版本!=2||!r.读取头.H)throw 失败{S::入口拒绝};
        auto group=名称窄读(r.F.编码,false,g,h,r.预算);
        R out;out.数据.emplace(group.empty()?std::optional<特征名称事实>{}:std::optional{std::move(group.front())});
        return out;
    });
}
概念树应用读取结果<std::vector<特征名称事实>> 概念树类数据服务::读取概念当前名称(
    const 特征名称按概念读取请求& r) const {
    using R=概念树应用读取结果<std::vector<特征名称事实>>;
    return 读取封装<R>(r.读取头,r.预算,[&](auto g,auto h) {
        if(r.版本!=2||!r.读取头.H)throw 失败{S::入口拒绝};
        R out;out.数据=名称窄读(r.C.值,true,g,h,r.预算);return out;
    });
}
std::vector<特征名称事实> 概念树类数据服务::共享名称组(共享读取上下文& c) const {
    const auto graph=应用图(概念树根角色::特征,false,c);std::vector<特征名称事实> out;
    for(const auto& node:graph.节点组)if(const auto* f=std::get_if<特征概念事实>(&node)) {
        auto names=名称组(f->身份,c);out.insert(out.end(),names.begin(),names.end());
    }
    std::set<std::uint64_t> fs;
    for(const auto& f:out)if(!fs.insert(f.F.编码.值).second)throw 失败{S::内部不一致};
    return out;
}
std::vector<特征概念命中事实> 概念树类数据服务::命中组(共享读取上下文& c) const {
    if(!feature_layout_)throw 失败{S::不支持};
    const auto& x=feature_layout_->类型;
    const auto own=共享关系(feature_layout_->锚点,x[12],true,c.H,c);
    std::vector<特征概念命中事实> out;std::set<std::pair<std::uint64_t,std::uint64_t>> pairs;
    for(const auto& member:own) {
        if(member.角色或顺序!=1)throw 失败{S::内部不一致};
        const auto n=节点(member.源节点,c.G,c.H);
        if(n.种类!=节点种类::普通||n.属性类型表示||!属性(n.编码,c.G,c.H).empty())throw 失败{S::内部不一致};
        专项计数(c.命中计数,n.编码,c.H,c.预算.特征.最大命中数);
        const auto os=共享关系(n.编码,x[13],false,c.H,c),cs=共享关系(n.编码,x[14],false,c.H,c);
        if(os.size()!=1||cs.size()!=1||os.front().角色或顺序!=1||cs.front().角色或顺序!=1
            ||!pairs.insert({os.front().目标节点.值,cs.front().目标节点.值}).second)throw 失败{S::内部不一致};
        for(const auto& e:{member,os.front(),cs.front()})
            if(e.创建事实代次!=n.创建事实代次||e.退出事实代次!=n.退出事实代次)throw 失败{S::内部不一致};
        const auto o=观察事实(os.front().目标节点,c);
        const auto match=应用判定(概念树概念身份{cs.front().目标节点},{1,c.G,o.证据H,o.输入.F},c);
        if(!match.适用)throw 失败{S::引用冲突};
        out.push_back({n.编码,o.记录,概念树概念身份{cs.front().目标节点}});
    }
    c.用量.命中数=c.命中计数.size();return out;
}

void 概念树类数据服务::准备父组(
    const std::vector<特征概念父组变更>& changes,共享写集构造& b,共享读取上下文& c,
    const std::optional<std::pair<Ref,特征概念定义>>& addition) const {
    检查数量(changes.size(),c.预算.基础.最大概念数);
    std::set<std::uint64_t> children;
    for(const auto& change:changes) {
        if(!有效(change.子.值)||!children.insert(change.子.值.值).second)throw 失败{S::入口拒绝};
        auto f=展开共享定义(change.子,c).事实;
        auto old=change.预期, actual=f.直接上位;
        auto edgeLess=[](const auto& a,const auto& b){return 身份小于(a.上位,b.上位);};
        std::sort(old.begin(),old.end(),edgeLess);std::sort(actual.begin(),actual.end(),edgeLess);
        if(old!=actual)throw 失败{S::引用冲突};
        auto ids=change.新父组;std::sort(ids.begin(),ids.end(),身份小于);
        if((ids.empty()&&!addition)||std::adjacent_find(ids.begin(),ids.end())!=ids.end())
            throw 失败{S::入口拒绝};
        for(auto parent:ids) {
            if(!有效(parent.值)||parent==f.身份)throw 失败{S::形成环};
            if(parent==layout_.根组.特征根) {
                if(f.身份!=f.类型根||addition)throw 失败{S::引用冲突};
            } else {
                auto p=展开共享定义(parent,c).事实;
                if(p.治理状态==概念树生命周期状态::退役||p.定义==f.定义||!共享包含(p.定义,f.定义,c))
                    throw 失败{S::引用冲突};
            }
        }
        if(addition) {
            if(addition->second==f.定义||!共享包含(addition->second,f.定义,c))throw 失败{S::引用冲突};
            b.关系(addition->first,Ref{f.身份.值},layout_.类型.直接上位);
        }
        for(const auto& e:actual)
            if(std::find(ids.begin(),ids.end(),e.上位)==ids.end())b.写集.退出事实.push_back(e.关系);
        for(auto parent:ids)
            if(std::none_of(actual.begin(),actual.end(),[&](const auto& e){return e.上位==parent;}))
                b.关系(Ref{parent.值},Ref{f.身份.值},layout_.类型.直接上位);
    }
    // 每条新父边严格包含子域；严格包含的传递性同时排除跨组环。
    检查数量(b.写集.关系.size(),c.预算.基础.最大关系数);
}
概念树类数据服务::特征准备结果 概念树类数据服务::准备特征写入(
    const 特征概念写请求& request,共享读取上下文& c) const {
    if(!feature_layout_||request.valueless_by_exception())throw 失败{S::不支持};
    特征准备结果 out;auto& b=out.构造;auto& reuse=out.复用;
    reuse.原请求=request;reuse.Gread=c.G;reuse.状态=S::无须变更;
    const auto& x=feature_layout_->类型;const auto& t=layout_.类型;
    auto uniqueIds=[&](auto ids) {
        检查数量(ids.size(),c.预算.基础.最大概念数);
        std::sort(ids.begin(),ids.end(),身份小于);
        if(std::adjacent_find(ids.begin(),ids.end())!=ids.end())throw 失败{S::入口拒绝};
        for(auto id:ids)if(!有效(id.值))throw 失败{S::入口拒绝};
        return ids;
    };
    std::visit([&](const auto& r) {
        using T=std::decay_t<decltype(r)>;
        if constexpr(std::is_same_v<T,类型观察登记请求>) {
            if(!r.最大保留观察数||!r.输入.键.序号||!有效(r.输入.键.来源)||!有效(r.输入.F.编码))
                throw 失败{S::入口拒绝};
            const auto f=特征结果(features_.读取准确特征事实({1,c.G,c.H,r.输入.F}));
            if(f.信息.类型!=r.输入.键.类型)throw 失败{S::类型不相容};
            核验外部节点(r.输入.键.来源,c.G,c.H);
            const auto all=观察组(r.输入.键.类型,c);
            for(const auto& o:all)if(o.输入.键==r.输入.键) {
                if(o.输入!=r.输入)throw 失败{S::幂等冲突};
                reuse.观察=o;return;
            }
            if(all.size()>=r.最大保留观察数)throw 失败{S::数量预算不足};
            const auto node=b.节点();
            b.关系(Ref{feature_layout_->锚点},Ref{node},x[0]);
            b.关系(Ref{node},Ref{r.输入.键.类型.编码},x[1]);
            b.关系(Ref{node},Ref{r.输入.F.编码},x[2]);
            b.关系(Ref{node},Ref{r.输入.键.来源},x[3]);
            b.值(Ref{node},x[4],std::vector<std::uint64_t>{r.输入.键.序号});
            b.值(Ref{node},x[5],r.输入.时间);
            b.值(Ref{node},x[6],std::vector<std::uint64_t>{c.H});
        } else if constexpr(std::is_same_v<T,特征概念建立请求>) {
            const auto d=规范共享定义(r.定义,c);
            if(d!=r.定义)throw 失败{S::入口拒绝};
            auto graph=应用图(概念树根角色::特征,false,c);
            for(const auto& v:graph.节点组)if(const auto* f=std::get_if<特征概念事实>(&v);f&&f->定义==d) {
                if(reuse.概念)throw 失败{S::内部不一致};
                if(f->治理状态==概念树生命周期状态::退役)throw 失败{S::引用冲突};
                reuse.概念=*f;reuse.直接边=f->直接上位;
            }
            if(reuse.概念)return;
            auto parents=uniqueIds(r.直接上位);
            if(parents.empty())throw 失败{S::入口拒绝};
            const auto full=特征结果(features_.读取I64类型完整域({1,c.G,c.H,d.类型})).数据;
            const bool typeRoot=full==d.域;
            if(typeRoot) {
                if(parents.size()!=1||parents.front()!=layout_.根组.特征根)throw 失败{S::引用冲突};
            } else for(auto id:parents) {
                if(id==layout_.根组.特征根)throw 失败{S::引用冲突};
                auto p=展开共享定义(id,c).事实;
                if(p.治理状态==概念树生命周期状态::退役||p.定义==d||!共享包含(p.定义,d,c))
                    throw 失败{S::引用冲突};
            }
            std::vector<std::pair<稳定编码,std::int64_t>> sources;
            特征规范I64域 expected;
            std::visit([&](const auto& why) {
                using W=std::decay_t<decltype(why)>;
                if constexpr(std::is_same_v<W,特征类型身份>) {
                    if(why!=d.类型||!typeRoot)throw 失败{S::引用冲突};
                    expected=full;sources.push_back({why.编码,1});
                } else if constexpr(std::is_same_v<W,稳定编码>) {
                    const auto o=观察事实(why,c);
                    if(o.输入.键.类型!=d.类型)throw 失败{S::类型不相容};
                    expected=特征结果(features_.形成I64特征域({1,c.G,c.H,o.输入.F})).域;
                    sources.push_back({why,2});
                } else {
                    auto ids=uniqueIds(why);
                    if(ids.empty())throw 失败{S::入口拒绝};
                    for(auto id:ids) {
                        auto p=展开共享定义(id,c).事实;
                        if(p.定义.类型!=d.类型||p.治理状态==概念树生命周期状态::退役)throw 失败{S::引用冲突};
                        expected.区间.insert(expected.区间.end(),p.定义.域.区间.begin(),p.定义.域.区间.end());
                        sources.push_back({id.值,3});
                    }
                }
            },r.依据);
            if(规范共享定义({d.类型,expected},c)!=d)throw 失败{S::引用冲突};
            const auto node=b.节点(),def=b.节点();
            b.值(Ref{node},t.概念种类,std::int64_t{4});
            b.值(Ref{node},t.生命周期,static_cast<std::int64_t>(概念树生命周期状态::活跃));
            b.关系(Ref{node},Ref{layout_.根组.特征根.值},t.概念根归属);
            b.关系(Ref{node},Ref{layout_.签名规则.值},t.概念规则);
            b.关系(Ref{node},Ref{def},t.定义成员);
            b.关系(Ref{def},Ref{d.类型.编码},x[7]);
            for(std::size_t i=0;i<d.域.区间.size();++i) {
                const auto member=b.节点();const auto& v=d.域.区间[i];
                b.关系(Ref{def},Ref{member},x[8],static_cast<std::int64_t>(i+1));
                b.值(Ref{member},x[9],v.下界);b.值(Ref{member},x[10],v.上界);
            }
            检查数量(sources.size(),c.预算.基础.最大来源数);
            for(std::size_t i=0;i<sources.size();++i) {
                const auto member=b.节点();
                b.关系(Ref{node},Ref{member},x[15],static_cast<std::int64_t>(i+1));
                b.关系(Ref{member},Ref{sources[i].first},x[16],sources[i].second);
                b.值(Ref{member},x[17],std::vector<std::uint64_t>{c.H});
            }
            for(auto p:parents)b.关系(Ref{p.值},Ref{node},t.直接上位);
            准备父组(r.子父替代,b,c,std::pair{Ref{node},d});
        } else if constexpr(std::is_same_v<T,特征名称变更请求>) {
            if(!有效(r.F.编码)||(!r.预期&&!r.新名称))throw 失败{S::入口拒绝};
            (void)特征结果(features_.读取准确特征事实({1,c.G,c.H,r.F}));
            const auto names=名称组(r.F,c);
            const std::optional<特征名称事实> current=names.empty()?std::nullopt:std::optional{names.front()};
            if(current!=r.预期)throw 失败{S::引用冲突};
            if(r.新名称) {
                auto f=展开共享定义(*r.新名称,c).事实;
                if(f.治理状态==概念树生命周期状态::退役||!应用判定(*r.新名称,{1,c.G,c.H,r.F},c).适用)
                    throw 失败{S::引用冲突};
                if(current&&current->C==*r.新名称){reuse.名称=current;return;}
                b.关系(Ref{r.F.编码},Ref{r.新名称->值},x[11]);
            }
            if(current)b.写集.退出事实.push_back(current->关系);
        } else if constexpr(std::is_same_v<T,特征概念命中请求>) {
            const auto o=观察事实(r.观察记录,c);
            auto ids=uniqueIds(r.完整命中组);
            const auto graph=应用图(概念树根角色::特征,false,c);
            std::vector<概念树概念身份> actual;
            for(const auto& v:graph.节点组)if(const auto* f=std::get_if<特征概念事实>(&v)) {
                if(f->定义.类型==o.输入.键.类型&&f->治理状态!=概念树生命周期状态::退役
                    &&应用判定(f->身份,{1,c.G,o.证据H,o.输入.F},c).适用)actual.push_back(f->身份);
            }
            std::sort(actual.begin(),actual.end(),身份小于);
            if(ids!=actual)throw 失败{S::引用冲突};
            auto hits=命中组(c);
            for(auto id:ids) {
                auto it=std::find_if(hits.begin(),hits.end(),[&](const auto& h){return h.观察记录==o.记录&&h.C==id;});
                if(it!=hits.end())reuse.命中组.push_back(*it);
                else {
                    const auto n=b.节点();
                    b.关系(Ref{feature_layout_->锚点},Ref{n},x[12]);
                    b.关系(Ref{n},Ref{o.记录},x[13]);
                    b.关系(Ref{n},Ref{id.值},x[14]);
                }
            }
        } else {
            准备父组(r.变更组,b,c);
            for(const auto& change:r.变更组) {
                auto f=展开共享定义(change.子,c).事实;
                reuse.直接边.insert(reuse.直接边.end(),f.直接上位.begin(),f.直接上位.end());
            }
        }
        b.写集.合同版本=L1所有者范围CRUD合同版本;
        b.写集.写入幂等身份={r.写入头.幂等身份};
        b.写集.期望事实代次=r.写入头.期望事实代次;
    },request);
    b.规范();检查数量(b.写集.关系.size(),c.预算.基础.最大关系数);
    return out;
}
特征概念写入结果 概念树类数据服务::特征发布读回(
    const 特征概念写请求& request,const L1所有者范围写集请求& ws,
    const L1所有者范围写入结果& saved,共享读取上下文& c) const {
    const auto mapping=共享核验首次(ws,saved,c);
    特征概念写入结果 out;out.原请求=request;out.Gread=c.G;out.首次H=saved.事实代次;out.发布状态=P::已确认发布;
    auto get=[&](std::uint32_t key) {
        const auto it=mapping.find(key);if(it==mapping.end())throw 失败{S::内部不一致};return it->second;
    };
    auto append=[&](const auto& changes,std::optional<概念树概念身份> inserted) {
        for(const auto& change:changes) {
            auto f=展开共享定义(change.子,c).事实;
            auto expected=change.新父组;if(inserted)expected.push_back(*inserted);
            std::sort(expected.begin(),expected.end(),身份小于);
            if(f.直接上位.size()!=expected.size())throw 失败{S::内部不一致};
            for(const auto& e:f.直接上位)
                if(std::find(expected.begin(),expected.end(),e.上位)==expected.end())throw 失败{S::内部不一致};
            out.直接边.insert(out.直接边.end(),f.直接上位.begin(),f.直接上位.end());
        }
    };
    std::visit([&](const auto& r) {
        using T=std::decay_t<decltype(r)>;
        if constexpr(std::is_same_v<T,类型观察登记请求>) {
            out.观察=观察事实(get(1),c);out.状态=S::已登记类型观察;
            if(out.观察->输入!=r.输入||out.观察->证据H!=r.写入头.期望事实代次)throw 失败{S::内部不一致};
        } else if constexpr(std::is_same_v<T,特征概念建立请求>) {
            out.概念=展开共享定义(概念树概念身份{get(1)},c).事实;out.状态=S::已创建共享定义;
            if(out.概念->定义!=r.定义)throw 失败{S::内部不一致};
            out.直接边=out.概念->直接上位;append(r.子父替代,out.概念->身份);
        } else if constexpr(std::is_same_v<T,特征名称变更请求>) {
            auto names=名称组(r.F,c);
            if(r.新名称) {
                if(names.size()!=1||names.front().关系!=get(0x10000)||names.front().C!=*r.新名称)
                    throw 失败{S::内部不一致};
                out.名称=names.front();out.状态=S::已绑定共享名称;
            } else {
                if(!names.empty()||!r.预期)throw 失败{S::内部不一致};
                out.状态=S::已退出共享名称;
            }
        } else if constexpr(std::is_same_v<T,特征概念命中请求>) {
            auto hits=命中组(c);
            for(const auto& h:hits)if(h.观察记录==r.观察记录
                &&std::find(r.完整命中组.begin(),r.完整命中组.end(),h.C)!=r.完整命中组.end())out.命中组.push_back(h);
            if(out.命中组.size()!=r.完整命中组.size())throw 失败{S::内部不一致};
            out.状态=S::已登记特征概念命中;
        } else {
            append(r.变更组,std::nullopt);out.状态=S::已变更特征概念组织;
        }
    },request);
    return out;
}
特征概念写入结果 概念树类数据服务::特征执行(const 特征概念写请求& input) {
    特征概念写入结果 out;bool uncertain=false,dispatched=false,zero=false;
    std::unique_lock<std::mutex> lock(mutex_);
    try {
        if(input.valueless_by_exception())throw 失败{S::入口拒绝};
        out.原请求=input;
        const auto head=std::visit([](const auto& r){return r.写入头;},input);
        const auto budget=std::visit([](const auto& r){return r.预算;},input);
        特征预算有效(budget);
        if(head.合同版本!=1||!head.期望事实代次||!head.幂等身份||(head.幂等身份>>48)==0x4E43)
            throw 失败{S::入口拒绝};
        if(shared_pending_||application_pending_)throw 失败{S::前次写入待收敛};
        if(feature_pending_&&*feature_pending_!=input)throw 失败{S::前次写入待收敛};
        if(!feature_layout_)throw 失败{S::不支持};
        uncertain=true;
        const auto first=port_.读取首次写入材料({L1所有者范围首次写入读取合同版本,{head.幂等身份}});
        if(first.合同版本!=L1所有者范围首次写入读取合同版本||first.所有者!=port_.所有者身份()
            ||first.写入幂等身份.值!=head.幂等身份||!first.读取事实代次)throw 失败{S::内部不一致};
        out.Gread=first.读取事实代次;守卫代次(out.Gread);
        if(first.状态!=L1所有者范围读取状态::成功&&first.状态!=L1所有者范围读取状态::未找到)
            throw 失败{映射(first.状态)};
        if(first.状态==L1所有者范围读取状态::成功) {
            if(!first.首次规范化写集||!first.首次写入结果)throw 失败{S::内部不一致};
            if(first.首次写入结果->事实代次&&first.首次写入结果->事实代次<=out.Gread)
                out.首次H=first.首次写入结果->事实代次;
            if(first.首次规范化写集->期望事实代次!=head.期望事实代次) {
                uncertain=false;throw 失败{S::幂等冲突};
            }
            共享读取上下文 before{out.Gread,head.期望事实代次,组合共享预算(budget)};
            const auto prepared=准备特征写入(input,before);
            if(prepared.构造.写集!=*first.首次规范化写集) {
                uncertain=false;throw 失败{S::幂等冲突};
            }
            before.H=first.首次写入结果->事实代次;
            auto result=特征发布读回(input,prepared.构造.写集,*first.首次写入结果,before);
            result.状态=S::精确重复;守卫代次(before.G);
            if(!result.成功())throw 失败{S::内部不一致};
            feature_pending_.reset();return result;
        }
        if(first.首次规范化写集||first.首次写入结果)throw 失败{S::内部不一致};
        uncertain=false;
        if(out.Gread!=head.期望事实代次) {
            feature_pending_.reset();throw 失败{S::事实代次漂移};
        }
        共享读取上下文 c{out.Gread,out.Gread,组合共享预算(budget)};
        auto prepared=准备特征写入(input,c);auto& ws=prepared.构造.写集;
        if(ws.节点.empty()&&ws.关系.empty()&&ws.值.empty()&&ws.属性槽变更.empty()&&ws.退出事实.empty()) {
            auto result=std::move(prepared.复用);守卫代次(c.G);
            if(!result.成功())throw 失败{S::内部不一致};
            feature_pending_.reset();return result;
        }
        // 保存先于派发；内存分配失败在发布前失败，不丢失未知发布的原请求。
        feature_pending_=input;守卫代次(c.G);dispatched=true;
        const auto saved=port_.提交所有者范围中性写集(ws);
        const bool header=saved.合同版本==L1所有者范围CRUD合同版本&&saved.所有者==port_.所有者身份()
            &&saved.写入幂等身份==ws.写入幂等身份;
        if(header&&!saved.是否形成内存权威发布&&saved.新编码映射.empty()
            &&saved.状态!=L1所有者范围写入状态::成功&&saved.状态!=L1所有者范围写入状态::精确重复
            &&saved.重试边界!=L1所有者范围重试边界::原幂等身份读回收敛) {
            zero=true;feature_pending_.reset();
            switch(saved.状态) {
            case L1所有者范围写入状态::事实代次漂移:throw 失败{S::事实代次漂移};
            case L1所有者范围写入状态::幂等冲突:throw 失败{S::幂等冲突};
            case L1所有者范围写入状态::引用冲突:throw 失败{S::引用冲突};
            case L1所有者范围写入状态::未找到:throw 失败{S::未找到};
            case L1所有者范围写入状态::已退出:throw 失败{S::目标已退出};
            case L1所有者范围写入状态::资源失败:throw 失败{S::资源失败};
            case L1所有者范围写入状态::入口拒绝:case L1所有者范围写入状态::许可拒绝:throw 失败{S::入口拒绝};
            default:throw 失败{S::内部不一致};
            }
        }
        if(!header)throw 失败{S::内部不一致};
        if(saved.事实代次>head.期望事实代次&&(saved.是否形成内存权威发布
            ||saved.状态==L1所有者范围写入状态::精确重复))out.首次H=saved.事实代次;
        out.Gread=当前代次();c.G=out.Gread;c.H=saved.事实代次;
        auto result=特征发布读回(input,ws,saved,c);
        if(saved.状态==L1所有者范围写入状态::精确重复)result.状态=S::精确重复;
        守卫代次(c.G);if(!result.成功())throw 失败{S::内部不一致};
        feature_pending_.reset();return result;
    } catch(const 失败& e){out.状态=e.状态;}
    catch(const std::bad_alloc&){out.状态=S::资源失败;}
    catch(const std::length_error&){out.状态=S::资源失败;}
    catch(...){out.状态=S::内部不一致;}
    if((uncertain||dispatched)&&!zero) {
        out.状态=S::已可能发布;out.发布状态=P::可能已发布;
        // 首次材料查询失败也不允许更换技术键；调用方有完整原请求可重入。
        if(!feature_pending_&&out.原请求) {
            try{feature_pending_=*out.原请求;}catch(...){}
        }
    } else out.发布状态=zero?P::已确认未发布:P::未派发;
    return out;
}
概念树应用读取结果<概念树写入头> 概念树类数据服务::取得特征概念写入头(const 特征概念预算& budget) const {
    概念树应用读取结果<概念树写入头> out;
    try {
        std::scoped_lock lock(mutex_);特征预算有效(budget);
        if(!feature_layout_)throw 失败{S::不支持};
        if(feature_pending_||shared_pending_||application_pending_)throw 失败{S::前次写入待收敛};
        const auto g=当前代次();out.Gread=g;out.H=g;
        auto key=g;
        for(std::uint64_t used=0;used<budget.最大首次材料项数;++used) {
            if(key==std::numeric_limits<std::uint64_t>::max())throw 失败{S::数量预算不足};
            ++key;
            if((key>>48)==0x4E43) {
                key=std::uint64_t{0x4E44}<<48;
            }
            const auto first=port_.读取首次写入材料({L1所有者范围首次写入读取合同版本,{key}});
            if(first.合同版本!=L1所有者范围首次写入读取合同版本||first.所有者!=port_.所有者身份()
                ||first.写入幂等身份.值!=key||first.读取事实代次!=g)throw 失败{S::事实代次漂移};
            if(first.状态==L1所有者范围读取状态::未找到) {
                if(first.首次规范化写集||first.首次写入结果)throw 失败{S::内部不一致};
                守卫代次(g);out.数据=概念树写入头{1,g,key};out.状态=S::已读取;return out;
            }
            if(first.状态!=L1所有者范围读取状态::成功)throw 失败{映射(first.状态)};
            if(!first.首次规范化写集||!first.首次写入结果)throw 失败{S::内部不一致};
        }
        throw 失败{S::数量预算不足};
    }catch(const 失败& e){out.状态=e.状态;}
    catch(const std::bad_alloc&){out.状态=S::资源失败;}
    catch(...){out.状态=S::内部不一致;}
    return out;
}
inline bool 新特征概念事实完整(const 特征概念事实& f,std::uint64_t h,bool exited=false) noexcept {
    auto active=[&](const auto& life) {
        return life.创建事实代次&&life.创建事实代次<=h
            &&(exited||!life.退出事实代次||*life.退出事实代次>h);
    };
    if(!有效(f.身份.值)||!有效(f.定义.类型.编码)||!有效(f.本体根.值)||!有效(f.类型根.值)
        ||!有效(f.规则.值)||!有效(f.定义记录)||!有效(f.生命周期值事实)||!有效(f.类型关系)
        ||!active(f.生命周期)||f.定义.域.区间.empty()||f.区间事实.size()!=f.定义.域.区间.size()
        ||f.来源事实.empty()||f.直接上位.empty())return false;
    for(std::size_t i=0;i<f.区间事实.size();++i) {
        const auto& a=f.区间事实[i];
        if(!有效(a.节点)||!有效(a.成员关系)||!有效(a.下界值事实)||!有效(a.上界值事实)
            ||a.区间!=f.定义.域.区间[i]||a.区间.下界>a.区间.上界||!active(a.生命周期))return false;
        if(i&&(f.区间事实[i-1].区间.上界>=a.区间.下界
            ||(f.区间事实[i-1].区间.上界!=std::numeric_limits<std::int64_t>::max()
                &&f.区间事实[i-1].区间.上界+1==a.区间.下界)))return false;
    }
    for(const auto& s:f.来源事实)
        if(!有效(s.记录)||!有效(s.所属关系)||!有效(s.目标关系)||!有效(s.截止值事实)
            ||!s.证据H||s.证据H>=f.生命周期.创建事实代次||!active(s.生命周期)||s.来源.valueless_by_exception())return false;
    for(const auto& e:f.直接上位)
        if(!有效(e.关系)||!有效(e.上位.值)||e.下位!=f.身份||!active(e.生命周期))return false;
    return true;
}
bool 特征概念写入结果::成功() const noexcept {
    using S=概念树数据状态;using P=概念树发布状态;
    if(版本!=1||!Gread||!原请求||原请求->valueless_by_exception())return false;
    const bool reuse=状态==S::无须变更,replay=状态==S::精确重复;
    if(reuse) {if(首次H||发布状态!=P::未派发)return false;}
    else if(!首次H||!*首次H||*首次H>Gread||发布状态!=P::已确认发布)return false;
    const auto h=reuse?Gread:*首次H;
    if(概念&&!新特征概念事实完整(*概念,h))return false;
    auto group=[&](const auto& changes,std::optional<概念树概念身份> inserted) -> std::optional<std::size_t> {
        std::size_t count=0;
        for(const auto& change:changes) {
            auto has=[&](概念树概念身份 parent) {
                return std::count_if(直接边.begin(),直接边.end(),[&](const auto& e){
                    return e.下位==change.子&&e.上位==parent&&有效(e.关系)&&e.生命周期.创建事实代次
                        &&e.生命周期.创建事实代次<=h&&(!e.生命周期.退出事实代次||*e.生命周期.退出事实代次>h);
                })==1;
            };
            for(auto p:change.新父组){if(!has(p))return std::nullopt;++count;}
            if(inserted){if(!has(*inserted))return std::nullopt;++count;}
        }
        return count;
    };
    return std::visit([&](const auto& r) noexcept {
        using T=std::decay_t<decltype(r)>;
        if(r.写入头.合同版本!=1||!r.写入头.幂等身份||!r.写入头.期望事实代次)return false;
        if constexpr(std::is_same_v<T,类型观察登记请求>) {
            return (reuse||replay||状态==S::已登记类型观察)&&观察&&有效(观察->记录)&&观察->输入==r.输入
                &&观察->证据H&&观察->证据H<=h&&(!首次H||观察->证据H<*首次H)
                &&!概念&&!名称&&命中组.empty()&&直接边.empty();
        } else if constexpr(std::is_same_v<T,特征概念建立请求>) {
            if((!reuse&&!replay&&状态!=S::已创建共享定义)||!概念||概念->定义!=r.定义||观察||名称||!命中组.empty())return false;
            if(reuse)return 直接边==概念->直接上位;
            const auto count=group(r.子父替代,概念->身份);
            return count&&直接边.size()==*count+概念->直接上位.size();
        } else if constexpr(std::is_same_v<T,特征名称变更请求>) {
            if(观察||概念||!命中组.empty()||!直接边.empty())return false;
            if(r.新名称)return(reuse||replay||状态==S::已绑定共享名称)&&名称&&有效(名称->关系)
                &&名称->F==r.F&&名称->C==*r.新名称&&名称->生命周期.创建事实代次
                &&名称->生命周期.创建事实代次<=h&&(!名称->生命周期.退出事实代次||*名称->生命周期.退出事实代次>h);
            return !reuse&&(replay||状态==S::已退出共享名称)&&!名称&&r.预期&&有效(r.预期->关系);
        } else if constexpr(std::is_same_v<T,特征概念命中请求>) {
            if((!reuse&&!replay&&状态!=S::已登记特征概念命中)||观察||概念||名称||!直接边.empty()
                ||命中组.size()!=r.完整命中组.size())return false;
            for(auto id:r.完整命中组)if(std::count_if(命中组.begin(),命中组.end(),[&](const auto& m){
                return 有效(m.记录)&&m.C==id&&m.观察记录==r.观察记录;})!=1)return false;
            return true;
        } else {
            if((!reuse&&!replay&&状态!=S::已变更特征概念组织)||观察||概念||名称||!命中组.empty())return false;
            const auto count=group(r.变更组,std::nullopt);return count&&*count==直接边.size();
        }
    },*原请求);
}
特征概念写入结果 概念树类数据服务::登记类型观察(const 类型观察登记请求& r){return 特征执行(r);}
特征概念写入结果 概念树类数据服务::建立特征概念(const 特征概念建立请求& r){return 特征执行(r);}
特征概念写入结果 概念树类数据服务::变更特征名称(const 特征名称变更请求& r){return 特征执行(r);}
特征概念写入结果 概念树类数据服务::登记特征概念命中(const 特征概念命中请求& r){return 特征执行(r);}
特征概念写入结果 概念树类数据服务::变更特征概念组织(const 特征概念组织请求& r){return 特征执行(r);}
特征概念写入结果 概念树类数据服务::收敛特征概念写入(const 特征概念写请求& r){return 特征执行(r);}
概念树共享用途事实 概念树类数据服务::共享用途(稳定编码 id,共享读取上下文& c) const {
    if(!feature_layout_)throw 失败{S::不支持};
    const auto& x=feature_layout_->类型;const auto n=节点(id,c.G,c.H);
    const auto av=属性(id,c.G,c.H);
    if(n.种类!=节点种类::普通||n.属性类型表示||av.size()!=2)throw 失败{S::内部不一致};
    const auto& kv=唯一属性(av,x[22]);const auto& tv=唯一属性(av,x[23]);
    const auto* key=std::get_if<std::vector<std::uint64_t>>(&kv.材料);
    const auto* time=std::get_if<std::int64_t>(&tv.材料);
    if(!key||key->size()!=3||!(*key)[0]||!(*key)[1]||(*key)[1]>=n.创建事实代次
        ||((*key)[2]!=1&&(*key)[2]!=2)||!time||kv.创建事实代次!=n.创建事实代次||tv.创建事实代次!=n.创建事实代次)
        throw 失败{S::内部不一致};
    const auto regs=共享关系(id,x[18],true,c.H,c);
    if(regs.size()!=1||regs.front().源节点!=feature_layout_->锚点||regs.front().角色或顺序!=1
        ||regs.front().创建事实代次!=n.创建事实代次)throw 失败{S::内部不一致};
    概念树共享用途事实 out;
    out.记录=id;out.登记关系=regs.front().编码;out.业务标识=(*key)[0];out.原H=(*key)[1];
    out.用途角色=static_cast<std::uint32_t>((*key)[2]);out.时间=*time;out.生命周期=投影生命(n,c.H);
    for(std::uint32_t role=20;role<=22;++role) {
        const auto edges=共享关系(id,x[role-1],false,n.创建事实代次,c);
        if(edges.size()!=(role==22?edges.size():1)||edges.size()>1)throw 失败{S::内部不一致};
        if(edges.empty())continue;
        const auto& e=edges.front();
        if(e.角色或顺序!=1||e.创建事实代次!=n.创建事实代次)throw 失败{S::内部不一致};
        if(role==20)out.概念=概念树概念身份{e.目标节点};
        else if(role==21)out.F={e.目标节点};
        else out.观察=e.目标节点;
        out.引用.push_back({e.编码,id,e.目标节点,out.概念,role,投影生命(e,c.H)});
    }
    {
        共享截止作用域 scope(c,out.原H);
        if(!应用判定(out.概念,{1,c.G,c.H,out.F},c).适用)throw 失败{S::内部不一致};
        if(out.观察) {
            const auto o=观察事实(*out.观察,c);
            if(o.输入.F!=out.F)throw 失败{S::引用冲突};
        }
    }
    return out;
}
std::vector<概念树共享用途事实> 概念树类数据服务::共享用途组(共享读取上下文& c) const {
    if(!feature_layout_)throw 失败{S::不支持};
    const auto regs=共享关系(feature_layout_->锚点,feature_layout_->类型[18],false,c.H,c);
    std::vector<概念树共享用途事实> out;std::set<std::array<std::uint64_t,3>> keys;
    for(const auto& e:regs) {
        专项计数(c.用途计数,e.目标节点,c.H,c.预算.最大用途数);
        auto f=共享用途(e.目标节点,c);
        if(f.登记关系!=e.编码||!keys.insert({f.业务标识,f.概念.值.值,f.用途角色}).second)throw 失败{S::内部不一致};
        out.push_back(std::move(f));
    }
    c.用量.用途数=c.用途计数.size();
    std::sort(out.begin(),out.end(),[](const auto& a,const auto& b){return a.记录.值<b.记录.值;});
    return out;
}
概念树应用读取结果<std::vector<概念树共享用途事实>> 概念树类数据服务::读取共享概念使用(
    const 概念树共享使用读取请求& r) const {
    return 共享读取封装<std::vector<概念树共享用途事实>>(r.读取头,r.预算,[&](auto& c) {
        if(r.版本!=2||!有效(r.概念.值))throw 失败{S::入口拒绝};
        // 用途原H保留历史事实；当前C已退出不妨碍其原生用途记录的审计。
        const auto n=原始事实(r.概念.值,c.G);
        if(!std::holds_alternative<N>(n)||std::get<N>(n).创建事实代次>c.H)throw 失败{S::未找到};
        {共享截止作用域 scope(c,std::get<N>(n).创建事实代次);(void)展开共享定义(r.概念,c);}
        auto out=共享用途组(c);
        std::erase_if(out,[&](const auto& f){return f.概念!=r.概念;});
        return out;
    });
}
概念树写入头 概念树类数据服务::共享请求头(const 概念树共享写请求& request) {
    return std::visit([](const auto& r) {
        using T=std::decay_t<decltype(r)>;
        if constexpr(std::is_same_v<T,概念树共享生命周期请求>||std::is_same_v<T,概念树共享退出请求>)
            return r.操作.写入头;
        else return r.写入头;
    },request);
}
概念树共享写请求 概念树类数据服务::规范共享请求(const 概念树共享写请求& input) const {
    if(!feature_layout_||input.valueless_by_exception())throw 失败{S::不支持};
    const auto head=共享请求头(input);
    if(head.合同版本!=1||!head.期望事实代次||!head.幂等身份)throw 失败{S::入口拒绝};
    std::visit([&](const auto& r) {
        using T=std::decay_t<decltype(r)>;
        if(r.版本!=2)throw 失败{S::入口拒绝};
        共享预算有效(r.预算);
        if constexpr(std::is_same_v<T,概念树共享使用请求>) {
            if(!r.业务标识||!有效(r.概念.值)||!r.原H||r.原H>head.期望事实代次
                ||r.实际.合同版本!=1||r.实际.Gread!=head.期望事实代次||r.实际.H!=r.原H
                ||!有效(r.实际.身份.编码)||(r.观察&&!有效(*r.观察))||(r.用途角色!=1&&r.用途角色!=2))
                throw 失败{S::入口拒绝};
        } else if constexpr(std::is_same_v<T,概念树共享释放请求>) {
            检查数量(r.完整预期.size(),r.预算.基础.最大关系数);
            std::set<std::uint64_t> ids;
            for(const auto& f:r.完整预期)
                if(!有效(f.关系)||!有效(f.记录)||!有效(f.目标)||!有效(f.概念.值)
                    ||(f.角色!=17&&f.角色!=20&&f.角色!=21&&f.角色!=22)
                    ||!生命完整(f.生命周期,head.期望事实代次)||!ids.insert(f.关系.值).second)throw 失败{S::入口拒绝};
        } else if constexpr(std::is_same_v<T,概念树共享上位请求>) {
            if(!有效(r.下位.值)||r.新父组.empty())throw 失败{S::入口拒绝};
        } else {
            auto op=r.操作;op.预算=r.预算.基础;
            (void)规范写请求(op);
        }
    },input);
    return input;
}

std::map<std::uint32_t, 稳定编码> 概念树类数据服务::共享核验首次(
    const L1所有者范围写集请求 &ws, const L1所有者范围写入结果 &saved, 共享读取上下文 &c) const {
    for (auto n : {ws.节点.size(), ws.关系.size(), ws.值.size(), ws.属性槽变更.size(),
                    ws.退出事实.size(), saved.新编码映射.size()}) 扣首次材料(c, n);
    const auto h = saved.事实代次;
    if (saved.合同版本 != L1所有者范围CRUD合同版本 || saved.所有者 != port_.所有者身份() ||
        saved.写入幂等身份 != ws.写入幂等身份 || !h || h > c.G || h <= ws.期望事实代次 ||
        ws.合同版本 != L1所有者范围CRUD合同版本 || !ws.期望事实代次 ||
        (saved.状态 != L1所有者范围写入状态::成功 && saved.状态 != L1所有者范围写入状态::精确重复) ||
        (saved.状态 == L1所有者范围写入状态::成功 && !saved.是否形成内存权威发布) ||
        (saved.重试边界 != L1所有者范围重试边界::不适用 &&
         saved.重试边界 != L1所有者范围重试边界::原幂等身份读回收敛) ||
        saved.新编码映射.size() != ws.节点.size() + ws.关系.size() + ws.值.size())
        throw 失败{S::内部不一致};
    std::map<std::uint32_t, 稳定编码> mappings;
    std::set<std::uint64_t> ids;
    for (const auto &[key, id] : saved.新编码映射)
        if (!key.值 || !有效(id) || !mappings.emplace(key.值, id).second || !ids.insert(id.值).second)
            throw 失败{S::内部不一致};
    auto mapped = [&](const Ref &r) {
        if (const auto *id = std::get_if<稳定编码>(&r)) {
            if (!有效(*id)) throw 失败{S::内部不一致};
            return *id;
        }
        const auto it = mappings.find(std::get<Key>(r).值);
        if (it == mappings.end()) throw 失败{S::内部不一致};
        return it->second;
    };
    std::set<std::uint32_t> localKeys;
    for (const auto &n : ws.节点) {
        if (!localKeys.insert(n.本地键.值).second) throw 失败{S::内部不一致};
        扣首次材料(c, 1);
        const auto actual = 节点(mapped(n.本地键), c.G, h);
        if (actual.创建事实代次 != h || actual.种类 != n.种类 || actual.属性类型表示 != n.属性类型表示)
            throw 失败{S::内部不一致};
    }
    for (const auto &e : ws.关系) {
        if (!localKeys.insert(e.本地键.值).second) throw 失败{S::内部不一致};
        扣首次材料(c, 1);
        const auto raw = 原始事实(mapped(e.本地键), c.G);
        const auto *actual = std::get_if<E>(&raw);
        if (!actual || !有效于(*actual, h) || actual->创建事实代次 != h ||
            actual->源节点 != mapped(e.源节点) || actual->目标节点 != mapped(e.目标节点) ||
            actual->关系类型节点 != mapped(e.关系类型节点) || actual->角色或顺序 != e.角色或顺序)
            throw 失败{S::内部不一致};
    }
    for (const auto &v : ws.值) {
        if (!localKeys.insert(v.本地键.值).second) throw 失败{S::内部不一致};
        扣首次材料(c, 1);
        const auto raw = 原始事实(mapped(v.本地键), c.G);
        const auto *actual = std::get_if<V>(&raw);
        if (!actual || !有效于(*actual, h) || actual->创建事实代次 != h ||
            actual->所属节点 != mapped(v.所属节点) || actual->属性类型节点 != mapped(v.属性类型节点) ||
            actual->来源节点 != mapped(v.来源节点) || actual->材料 != v.材料)
            throw 失败{S::内部不一致};
    }
    for (std::size_t i = 0; i < ws.退出事实.size(); ++i) {
        const auto id = ws.退出事实[i];
        if (!有效(id) || (i && !编码小于(ws.退出事实[i - 1], id)) || ids.contains(id.值))
            throw 失败{S::内部不一致};
        扣首次材料(c, 1);
        const auto raw = 原始事实(id, c.G);
        if (!std::visit([&](const auto &f) { return f.退出事实代次 == h && f.创建事实代次 < h; }, raw))
            throw 失败{S::内部不一致};
    }
    守卫代次(c.G);
    return mappings;
}

概念树共享写入结果 概念树类数据服务::共享执行(const 概念树共享写请求& input) {
    概念树共享写入结果 out;bool uncertain=false,dispatched=false,zero=false;
    std::unique_lock<std::mutex> lock(mutex_);
    try {
        if(input.valueless_by_exception())throw 失败{S::入口拒绝};
        out.原请求=input;
        const auto head=共享请求头(input);
        (void)规范共享请求(input);
        const auto budget=std::visit([](const auto& r){return r.预算;},input);
        共享预算有效(budget);
        if(head.合同版本!=1||!head.期望事实代次||!head.幂等身份||(head.幂等身份>>48)==0x4E43)
            throw 失败{S::入口拒绝};
        if(feature_pending_||application_pending_)throw 失败{S::前次写入待收敛};
        if(shared_pending_&&*shared_pending_!=input)throw 失败{S::前次写入待收敛};
        if(!feature_layout_)throw 失败{S::不支持};
        uncertain=true;
        const auto first=port_.读取首次写入材料({L1所有者范围首次写入读取合同版本,{head.幂等身份}});
        if(first.合同版本!=L1所有者范围首次写入读取合同版本||first.所有者!=port_.所有者身份()
            ||first.写入幂等身份.值!=head.幂等身份||!first.读取事实代次)throw 失败{S::内部不一致};
        out.Gread=first.读取事实代次;守卫代次(out.Gread);
        if(first.状态!=L1所有者范围读取状态::成功&&first.状态!=L1所有者范围读取状态::未找到)
            throw 失败{映射(first.状态)};
        if(first.状态==L1所有者范围读取状态::成功) {
            if(!first.首次规范化写集||!first.首次写入结果)throw 失败{S::内部不一致};
            if(first.首次写入结果->事实代次&&first.首次写入结果->事实代次<=out.Gread)
                out.首次H=first.首次写入结果->事实代次;
            if(first.首次规范化写集->期望事实代次!=head.期望事实代次) {
                uncertain=false;throw 失败{S::幂等冲突};
            }
            共享读取上下文 before{out.Gread,head.期望事实代次,budget};
            const auto prepared=准备共享写入(input,before);
            if(prepared.构造.写集!=*first.首次规范化写集) {
                uncertain=false;throw 失败{S::幂等冲突};
            }
            before.H=first.首次写入结果->事实代次;
            auto result=共享发布读回(input,prepared.构造.写集,*first.首次写入结果,before);
            result.状态=S::精确重复;守卫代次(before.G);
            if(!result.成功())throw 失败{S::内部不一致};
            shared_pending_.reset();return result;
        }
        if(first.首次规范化写集||first.首次写入结果)throw 失败{S::内部不一致};
        uncertain=false;
        if(out.Gread!=head.期望事实代次) {
            shared_pending_.reset();throw 失败{S::事实代次漂移};
        }
        共享读取上下文 c{out.Gread,out.Gread,budget};
        auto prepared=准备共享写入(input,c);auto& ws=prepared.构造.写集;
        if(ws.节点.empty()&&ws.关系.empty()&&ws.值.empty()&&ws.属性槽变更.empty()&&ws.退出事实.empty()) {
            auto result=out;result.状态=S::无须变更;
            result.概念=std::move(prepared.复用概念);result.用途=std::move(prepared.复用用途);
            if(std::holds_alternative<概念树共享上位请求>(input)&&result.概念)result.直接边=result.概念->直接上位;
            守卫代次(c.G);
            if(!result.成功())throw 失败{S::内部不一致};
            shared_pending_.reset();return result;
        }
        // 保存先于派发；内存分配失败在发布前失败，不丢失未知发布的原请求。
        shared_pending_=input;守卫代次(c.G);dispatched=true;
        const auto saved=port_.提交所有者范围中性写集(ws);
        const bool header=saved.合同版本==L1所有者范围CRUD合同版本&&saved.所有者==port_.所有者身份()
            &&saved.写入幂等身份==ws.写入幂等身份;
        if(header&&!saved.是否形成内存权威发布&&saved.新编码映射.empty()
            &&saved.状态!=L1所有者范围写入状态::成功&&saved.状态!=L1所有者范围写入状态::精确重复
            &&saved.重试边界!=L1所有者范围重试边界::原幂等身份读回收敛) {
            zero=true;shared_pending_.reset();
            switch(saved.状态) {
            case L1所有者范围写入状态::事实代次漂移:throw 失败{S::事实代次漂移};
            case L1所有者范围写入状态::幂等冲突:throw 失败{S::幂等冲突};
            case L1所有者范围写入状态::引用冲突:throw 失败{S::引用冲突};
            case L1所有者范围写入状态::未找到:throw 失败{S::未找到};
            case L1所有者范围写入状态::已退出:throw 失败{S::目标已退出};
            case L1所有者范围写入状态::资源失败:throw 失败{S::资源失败};
            case L1所有者范围写入状态::入口拒绝:case L1所有者范围写入状态::许可拒绝:throw 失败{S::入口拒绝};
            default:throw 失败{S::内部不一致};
            }
        }
        if(!header)throw 失败{S::内部不一致};
        if(saved.事实代次>head.期望事实代次&&(saved.是否形成内存权威发布
            ||saved.状态==L1所有者范围写入状态::精确重复))out.首次H=saved.事实代次;
        out.Gread=当前代次();c.G=out.Gread;c.H=saved.事实代次;
        auto result=共享发布读回(input,ws,saved,c);
        if(saved.状态==L1所有者范围写入状态::精确重复)result.状态=S::精确重复;
        守卫代次(c.G);if(!result.成功())throw 失败{S::内部不一致};
        shared_pending_.reset();return result;
    } catch(const 失败& e){out.状态=e.状态;}
    catch(const std::bad_alloc&){out.状态=S::资源失败;}
    catch(const std::length_error&){out.状态=S::资源失败;}
    catch(...){out.状态=S::内部不一致;}
    if((uncertain||dispatched)&&!zero) {
        out.状态=S::已可能发布;out.发布状态=P::可能已发布;
        // 首次材料查询失败也不允许更换技术键；调用方有完整原请求可重入。
        if(!shared_pending_&&out.原请求) {
            try{shared_pending_=*out.原请求;}catch(...){}
        }
    } else out.发布状态=zero?P::已确认未发布:P::未派发;
    return out;
}
概念树类数据服务::共享准备结果 概念树类数据服务::准备共享用途(
    const 概念树共享使用请求& r,共享读取上下文& c) const {
    共享准备结果 out;
    const auto f=展开共享定义(r.概念,c).事实;
    if(f.治理状态==概念树生命周期状态::退役)throw 失败{S::引用冲突};
    {
        auto actual=r.实际;actual.Gread=c.G;
        共享截止作用域 scope(c,r.原H);
        if(!应用判定(r.概念,actual,c).适用)throw 失败{S::引用冲突};
        if(r.观察) {
            const auto o=观察事实(*r.观察,c);
            if(o.输入.F!=r.实际.身份||o.输入.键.类型!=f.定义.类型)throw 失败{S::引用冲突};
        }
    }
    for(const auto& u:共享用途组(c))
        if(u.业务标识==r.业务标识&&u.概念==r.概念&&u.用途角色==r.用途角色) {
            if(u.F!=r.实际.身份||u.观察!=r.观察||u.原H!=r.原H||u.时间!=r.时间)throw 失败{S::幂等冲突};
            out.复用用途=u;return out;
        }
    auto& b=out.构造;const auto& x=feature_layout_->类型;
    const auto n=b.节点();
    b.关系(Ref{feature_layout_->锚点},Ref{n},x[18]);
    b.关系(Ref{n},Ref{r.概念.值},x[19]);
    b.关系(Ref{n},Ref{r.实际.身份.编码},x[20]);
    if(r.观察)b.关系(Ref{n},Ref{*r.观察},x[21]);
    b.值(Ref{n},x[22],std::vector<std::uint64_t>{r.业务标识,r.原H,r.用途角色});
    b.值(Ref{n},x[23],r.时间);
    return out;
}
概念树类数据服务::共享准备结果 概念树类数据服务::准备共享治理(
    const 概念树共享写请求& request,共享读取上下文& c) const {
    共享准备结果 out;auto& b=out.构造;
    const auto& x=feature_layout_->类型;const auto& t=layout_.类型;
    if(const auto* r=std::get_if<概念树共享释放请求>(&request)) {
        for(const auto& expected:r->完整预期) {
            const auto raw=原始事实(expected.关系,c.G);const auto* e=std::get_if<E>(&raw);
            if(!e||!有效于(*e,c.H)||e->源节点!=expected.记录||e->目标节点!=expected.目标
                ||e->关系类型节点!=x[expected.角色-1]||投影生命(*e,c.H)!=expected.生命周期)
                throw 失败{S::引用冲突};
            bool found=false;
            if(expected.角色==17) {
                const auto f=展开共享定义(expected.概念,c).事实;
                for(const auto& s:f.来源事实)
                    if(s.记录==expected.记录&&s.目标关系==expected.关系&&!s.当前引用已释放)found=true;
            } else {
                const auto u=共享用途(expected.记录,c);
                if(u.概念!=expected.概念)throw 失败{S::引用冲突};
                found=std::find(u.引用.begin(),u.引用.end(),expected)!=u.引用.end();
            }
            if(!found)throw 失败{S::引用冲突};
            b.写集.退出事实.push_back(expected.关系);
        }
    } else if(const auto* r=std::get_if<概念树共享上位请求>(&request)) {
        准备父组({{r->下位,r->预期父组,r->新父组}},b,c);
        if(b.写集.关系.empty()&&b.写集.退出事实.empty())out.复用概念=展开共享定义(r->下位,c).事实;
    } else if(const auto* r=std::get_if<概念树共享生命周期请求>(&request)) {
        const auto& op=r->操作;auto f=展开共享定义(op.概念,c).事实;
        if(f.生命周期值事实!=op.当前生命周期值编码||f.治理状态!=op.预期状态)throw 失败{S::引用冲突};
        // 冷却、退役和恢复均先完整读取真实用途；读取失败不得解释为零使用。
        (void)共享用途组(c);
        if(op.预期状态==op.目标状态){out.复用概念=f;return out;}
        if(!生命周期迁移有效(op.预期状态,op.目标状态))throw 失败{S::入口拒绝};
        if(op.预期状态==概念树生命周期状态::退役) {
            if(op.目标状态!=概念树生命周期状态::活跃||!r->重新采用见证)throw 失败{S::入口拒绝};
            特征信息身份 actual;
            std::visit([&](const auto& witness) {
                using W=std::decay_t<decltype(witness)>;
                if constexpr(std::is_same_v<W,类型观察事实>) {
                    const auto read=观察事实(witness.记录,c);
                    if(read!=witness)throw 失败{S::引用冲突};actual=read.输入.F;
                } else {
                    const auto read=共享用途(witness.记录,c);
                    if(read!=witness||read.概念!=f.身份)throw 失败{S::引用冲突};actual=read.F;
                }
            },*r->重新采用见证);
            if(!应用判定(f.身份,{1,c.G,c.H,actual},c).适用)throw 失败{S::引用冲突};
        }
        b.写集.退出事实.push_back(f.生命周期值事实);
        b.值(Ref{f.身份.值},t.生命周期,static_cast<std::int64_t>(op.目标状态));
    } else if(const auto* r=std::get_if<概念树共享退出请求>(&request)) {
        auto full=展开共享定义(r->操作.概念,c);const auto& f=full.事实;
        if(f.治理状态!=概念树生命周期状态::退役||f.生命周期值事实!=r->操作.当前退役生命周期值编码)
            throw 失败{S::引用冲突};
        auto names=名称组(f.身份,c);if(!names.empty())throw 失败{S::引用冲突};
        const auto children=共享关系(f.身份.值,t.直接上位,false,c.H,c);
        // FT 全域锚点不是第五本体根，但有保留细分时不能退出。
        if(f.身份==f.类型根&&!children.empty())throw 失败{S::根不可退出};
        if(children.size()!=r->完整替代.size())throw 失败{S::引用冲突};
        std::vector<特征概念父组变更> changes;
        for(const auto& item:r->完整替代) {
            if(std::count_if(children.begin(),children.end(),[&](const auto& e){return e.目标节点==item.子节点.值;})!=1
                ||std::find(item.新父组.begin(),item.新父组.end(),f.身份)!=item.新父组.end())throw 失败{S::引用冲突};
            changes.push_back({item.子节点,item.预期父组,item.新父组});
        }
        准备父组(changes,b,c);
        for(auto type:{x[11],x[14],x[16],x[19],t.定义模板,t.支持概念})
            if(!共享关系(f.身份.值,type,true,c.H,c).empty())throw 失败{S::引用冲突};
        if(dynamic_&&!共享关系(f.身份.值,dynamic_->类型.槽模板,true,c.H,c).empty())throw 失败{S::引用冲突};
        b.写集.退出事实.insert(b.写集.退出事实.end(),full.自有事实.begin(),full.自有事实.end());
    } else throw 失败{S::入口拒绝};
    b.规范();return out;
}
概念树类数据服务::共享准备结果 概念树类数据服务::准备共享写入(
    const 概念树共享写请求& request,共享读取上下文& c) const {
    auto out=std::holds_alternative<概念树共享使用请求>(request)
        ?准备共享用途(std::get<概念树共享使用请求>(request),c):准备共享治理(request,c);
    const auto head=共享请求头(request);auto& ws=out.构造.写集;
    ws.合同版本=L1所有者范围CRUD合同版本;ws.写入幂等身份={head.幂等身份};ws.期望事实代次=head.期望事实代次;
    out.构造.规范();检查数量(ws.关系.size(),c.预算.基础.最大关系数);return out;
}
void 概念树类数据服务::核验共享释放读回(const 概念树共享释放请求& r,
    std::uint64_t published, 共享读取上下文& c) const {
    for(const auto& expected:r.完整预期) {
        const auto raw=原始事实(expected.关系,c.G);
        const auto* edge=std::get_if<E>(&raw);
        if(!edge||edge->退出事实代次!=published)throw 失败{S::内部不一致};
        if(expected.角色==17) {
            const auto f=展开共享定义(expected.概念,c).事实;
            const auto s=std::find_if(f.来源事实.begin(),f.来源事实.end(),[&](const auto& v){return v.目标关系==expected.关系;});
            if(s==f.来源事实.end()||!s->当前引用已释放)throw 失败{S::内部不一致};
        } else {
            const auto u=共享用途(expected.记录,c);
            const auto ref=std::find_if(u.引用.begin(),u.引用.end(),[&](const auto& v){return v.关系==expected.关系;});
            if(ref==u.引用.end()||ref->生命周期.退出事实代次!=published)throw 失败{S::内部不一致};
        }
    }
}
概念树共享写入结果 概念树类数据服务::共享发布读回(
    const 概念树共享写请求& request,const L1所有者范围写集请求& ws,
    const L1所有者范围写入结果& saved,共享读取上下文& c) const {
    const auto mappings=共享核验首次(ws,saved,c);
    概念树共享写入结果 out;out.原请求=request;out.Gread=c.G;out.首次H=saved.事实代次;
    out.发布状态=P::已确认发布;out.已退出编码=ws.退出事实;
    std::visit([&](const auto& r) {
        using T=std::decay_t<decltype(r)>;
        if constexpr(std::is_same_v<T,概念树共享使用请求>) {
            const auto it=mappings.find(1);if(it==mappings.end())throw 失败{S::内部不一致};
            out.用途=共享用途(it->second,c);out.状态=S::已记录共享用途;
        } else if constexpr(std::is_same_v<T,概念树共享释放请求>) {
            if(ws.退出事实.size()!=r.完整预期.size())throw 失败{S::内部不一致};
            核验共享释放读回(r,saved.事实代次,c);
            out.状态=S::已释放引用;
        } else if constexpr(std::is_same_v<T,概念树共享退出请求>) {
            {共享截止作用域 scope(c,ws.期望事实代次);out.概念=展开共享定义(r.操作.概念,c).事实;}
            out.概念->生命周期.退出事实代次=saved.事实代次;
            out.状态=S::已退出概念;
            for(const auto& item:r.完整替代) {
                auto f=展开共享定义(item.子节点,c).事实;
                if(f.直接上位.size()!=item.新父组.size())throw 失败{S::内部不一致};
                for(const auto& e:f.直接上位)
                    if(std::find(item.新父组.begin(),item.新父组.end(),e.上位)==item.新父组.end())throw 失败{S::内部不一致};
                out.直接边.insert(out.直接边.end(),f.直接上位.begin(),f.直接上位.end());
            }
        } else if constexpr(std::is_same_v<T,概念树共享上位请求>) {
            out.概念=展开共享定义(r.下位,c).事实;out.直接边=out.概念->直接上位;out.状态=S::已替换上位;
            if(out.直接边.size()!=r.新父组.size())throw 失败{S::内部不一致};
            for(const auto& e:out.直接边)
                if(std::find(r.新父组.begin(),r.新父组.end(),e.上位)==r.新父组.end())throw 失败{S::内部不一致};
        } else {
            out.概念=展开共享定义(r.操作.概念,c).事实;out.状态=S::已迁移生命周期;
            if(out.概念->治理状态!=r.操作.目标状态)throw 失败{S::内部不一致};
        }
    },request);
    return out;
}
bool 概念树共享写入结果::成功() const noexcept {
    using S=概念树数据状态;using P=概念树发布状态;
    if(版本!=2||!Gread||!原请求||原请求->valueless_by_exception())return false;
    const bool reuse=状态==S::无须变更,replay=状态==S::精确重复;
    if(reuse){if(首次H||发布状态!=P::未派发||!已退出编码.empty())return false;}
    else if(!首次H||!*首次H||*首次H>Gread||发布状态!=P::已确认发布)return false;
    if(概念&&!新特征概念事实完整(*概念,reuse?Gread:*首次H,状态==S::已退出概念
        ||std::holds_alternative<概念树共享退出请求>(*原请求)))return false;
    return std::visit([&](const auto& r) noexcept {
        using T=std::decay_t<decltype(r)>;
        if(r.版本!=2)return false;
        if constexpr(std::is_same_v<T,概念树共享使用请求>) {
            return (reuse||replay||状态==S::已记录共享用途)&&用途&&!概念&&直接边.empty()
                &&有效(用途->记录)&&有效(用途->登记关系)&&用途->业务标识==r.业务标识&&用途->概念==r.概念
                &&用途->F==r.实际.身份&&用途->观察==r.观察&&用途->原H==r.原H&&用途->时间==r.时间
                &&用途->用途角色==r.用途角色&&用途->引用.size()==(r.观察 ? 3 : 2);
        } else if constexpr(std::is_same_v<T,概念树共享释放请求>) {
            if((!reuse&&!replay&&状态!=S::已释放引用)||概念||用途||!直接边.empty()
                ||已退出编码.size()!=r.完整预期.size())return false;
            for(const auto& expected:r.完整预期)
                if(std::count(已退出编码.begin(),已退出编码.end(),expected.关系)!=1)return false;
            return true;
        } else if constexpr(std::is_same_v<T,概念树共享上位请求>) {
            if((!reuse&&!replay&&状态!=S::已替换上位)||!概念||用途||概念->身份!=r.下位
                ||直接边!=概念->直接上位||直接边.size()!=r.新父组.size())return false;
            for(auto p:r.新父组)
                if(std::count_if(直接边.begin(),直接边.end(),[&](const auto& e){return e.上位==p&&e.下位==r.下位;})!=1)return false;
            return true;
        } else if constexpr(std::is_same_v<T,概念树共享生命周期请求>) {
            return(reuse||replay||状态==S::已迁移生命周期)&&概念&&!用途&&概念->身份==r.操作.概念
                &&概念->治理状态==r.操作.目标状态;
        } else return(replay||状态==S::已退出概念)&&概念&&!用途&&概念->身份==r.操作.概念
            &&概念->生命周期.退出事实代次==首次H&&!已退出编码.empty();
    },*原请求);
}
概念树共享写入结果 概念树类数据服务::记录共享概念使用(const 概念树共享使用请求& r){return 共享执行(r);}
概念树共享写入结果 概念树类数据服务::操作共享概念上位(const 概念树共享上位请求& r){return 共享执行(r);}
概念树共享写入结果 概念树类数据服务::迁移共享概念生命周期(const 概念树共享生命周期请求& r){return 共享执行(r);}
概念树共享写入结果 概念树类数据服务::退出共享特征概念(const 概念树共享退出请求& r){return 共享执行(r);}
概念树共享写入结果 概念树类数据服务::释放共享概念引用(const 概念树共享释放请求& r){return 共享执行(r);}
void 概念树类数据服务::共享名称交叉核验(稳定编码 id,bool incoming,
    const std::vector<特征名称事实>& all,共享读取上下文& c) const {
    auto direct=名称组(incoming?std::variant<特征信息身份,概念树概念身份>{概念树概念身份{id}}
        :std::variant<特征信息身份,概念树概念身份>{特征信息身份{id}},c);
    for(const auto& f:direct)if(std::count(all.begin(),all.end(),f)!=1)throw 失败{S::内部不一致};
    for(const auto& f:all)if((incoming?f.C.值:f.F.编码)==id&&std::count(direct.begin(),direct.end(),f)!=1)
        throw 失败{S::内部不一致};
}


概念树类数据服务::存在展开_v2 概念树类数据服务::展开存在定义_v2(
    概念树概念身份 id, 共享读取上下文 &c) const {
    if(!existence_layout_)throw 失败{S::不支持};
    const auto &t = layout_.类型;
    const auto n = 节点(id.值, c.G, c.H);
    const auto born = n.创建事实代次;
    if (n.种类 != 节点种类::普通 || n.属性类型表示 || id == layout_.根组.存在根)
        throw 失败{S::引用冲突};
    const auto av = 属性(id.值, c.G, c.H);
    if (av.size() != 2) throw 失败{S::内部不一致};
    const auto &kind = 唯一属性(av, t.概念种类), &state = 唯一属性(av, t.生命周期);
    const auto life = std::get<std::int64_t>(state.材料);
    if (std::get<std::int64_t>(kind.材料) != 2 || kind.创建事实代次 != born || life < 1 || life > 3)
        throw 失败{S::引用冲突};
    存在展开_v2 out;
    auto &f = out.事实;
    f.概念 = id; f.本体根 = layout_.根组.存在根; f.规则 = layout_.签名规则;
    f.生命周期 = 投影生命(n,c.H); f.生命周期值事实 = state.编码;
    f.治理状态 = static_cast<概念树生命周期状态>(life);
    out.自有事实 = {id.值, kind.编码, state.编码};
    auto one = [&](稳定编码 from, 稳定编码 type, std::uint64_t h) {
        const auto es = 共享关系(from, type, false, h, c);
        if (es.size() != 1 || es[0].角色或顺序 != 1) throw 失败{S::内部不一致};
        return es[0];
    };
    const auto re = one(id.值, t.概念根归属, c.H), se = one(id.值, t.概念规则, c.H),
               de = one(id.值, t.定义成员, c.H);
    if (re.目标节点 != f.本体根.值 || se.目标节点 != f.规则.值)
        throw 失败{S::引用冲突};
    for (const auto &e : {re, se, de}) {
        if (e.创建事实代次 != born || e.退出事实代次 != n.退出事实代次) throw 失败{S::内部不一致};
        out.自有事实.push_back(e.编码);
    }
    const auto dn = 节点(de.目标节点, c.G, c.H);
    if (dn.种类 != 节点种类::普通 || dn.属性类型表示 || dn.创建事实代次 != born ||
        dn.退出事实代次 != n.退出事实代次 || !属性(dn.编码, c.G, c.H).empty() ||
        !共享关系(dn.编码, t.定义宿主, false, born, c).empty() ||
        !共享关系(dn.编码, t.定义特征类型, false, born, c).empty()) throw 失败{S::内部不一致};
    f.定义记录 = dn.编码; out.自有事实.push_back(dn.编码);
    const auto templates = 共享关系(dn.编码, t.定义模板, false, c.H, c);
    if (templates.empty() || templates != 共享关系(dn.编码, t.定义模板, false, born, c))
        throw 失败{S::内部不一致};
    for (const auto &e : templates) {
        if (e.角色或顺序 != f.定义.特征模板组.size() + 1 || e.创建事实代次 != born ||
            e.退出事实代次 != n.退出事实代次) throw 失败{S::内部不一致};
        const auto target = 应用模板(概念树概念身份{e.目标节点}, c);
        const auto targetLife = std::visit([](const auto &x) { return x.生命周期; }, target);
        if (targetLife.创建事实代次 > born) throw 失败{S::内部不一致};
        f.定义.特征模板组.push_back(概念树概念身份{e.目标节点}); out.自有事实.push_back(e.编码);
    }
    if (规范化定义(f.定义) != 概念树定义{f.定义}) throw 失败{S::内部不一致};
    const auto sources = 共享关系(id.值, t.来源成员, false, c.H, c);
    if (sources.empty() || sources != 共享关系(id.值, t.来源成员, false, born, c))
        throw 失败{S::内部不一致};
    if (sources.size() > c.预算.基础.最大来源数 - c.来源数) throw 失败{S::数量预算不足};
    c.来源数 += sources.size();
    for (const auto &member : sources) {
        if (member.角色或顺序 != f.世界来源组.size() + f.抽象依据组.size() + 1 ||
            member.创建事实代次 != born || member.退出事实代次 != n.退出事实代次)
            throw 失败{S::内部不一致};
        const auto sn = 节点(member.目标节点, c.G, c.H);
        const auto sa = 属性(sn.编码, c.G, c.H);
        if (sn.种类 != 节点种类::普通 || sn.属性类型表示 || sn.创建事实代次 != born ||
            sn.退出事实代次 != n.退出事实代次 || sa.size() != 1) throw 失败{S::内部不一致};
        const auto &tv = 唯一属性(sa, t.证据截止);
        const auto &times = std::get<std::vector<std::uint64_t>>(tv.材料);
        if (times.size() != 1 || !times[0] || times[0] >= born || tv.创建事实代次 != born ||
            tv.退出事实代次 != n.退出事实代次) throw 失败{S::内部不一致};
        const auto es = 共享关系(sn.编码, t.来源存在, false, born, c);
        const auto fs = 共享关系(sn.编码, t.来源特征, false, born, c);
        const auto cs = 共享关系(sn.编码, existence_layout_->抽象依据目标关系类型, false, born, c);
        if (es.size() + fs.size() + cs.size() != 1) throw 失败{S::内部不一致};
        const auto &e = !cs.empty() ? cs[0] : !es.empty() ? es[0] : fs[0];
        if (e.创建事实代次 != born || e.角色或顺序 != 1 ||
            (e.退出事实代次 && *e.退出事实代次 <= born)) throw 失败{S::内部不一致};
        const auto current = 共享关系(sn.编码, e.关系类型节点, false, c.H, c);
        const bool released = current.empty();
        if ((!released && (current.size() != 1 || current[0].编码 != e.编码)) ||
            (released && (!e.退出事实代次 || *e.退出事实代次 > c.H))) throw 失败{S::内部不一致};
        if (!released) out.自有事实.push_back(e.编码);
        if (!cs.empty()) {
            const auto target = 概念树概念身份{e.目标节点};
            const auto tn = 节点(target.值, c.G, times[0]);
            if (target == id || target == f.本体根 || tn.创建事实代次 >= born) throw 失败{S::内部不一致};
            概念树应用存在事实 source;
            { 共享截止作用域 at(c, times[0]); source = 展开存在定义_v2(target, c).事实; }
            if (source.本体根 != f.本体根 || f.定义.特征模板组.size() >= source.定义.特征模板组.size() ||
                !std::includes(source.定义.特征模板组.begin(), source.定义.特征模板组.end(),
                    f.定义.特征模板组.begin(), f.定义.特征模板组.end(), 身份小于)) throw 失败{S::内部不一致};
            f.抽象依据组.push_back({id, target, sn.编码, member.编码, e.编码, tv.编码, times[0],
                {born, sn.退出事实代次}, {born, member.退出事实代次}, {born, e.退出事实代次}, released});
        } else {
            f.世界来源组.push_back({sn.编码, member.编码, e.编码, tv.编码,
                {!es.empty() ? 概念树世界引用{概念树存在引用{e.目标节点}} : 概念树世界引用{概念树特征引用{e.目标节点}}, times[0]},
                {born, sn.退出事实代次}, released});
        }
        for (auto code : {sn.编码, member.编码, tv.编码}) out.自有事实.push_back(code);
    }
    if (!f.世界来源组.empty() && !f.抽象依据组.empty()) throw 失败{S::内部不一致};
    if (!f.抽象依据组.empty() && f.抽象依据组.size() != 1) throw 失败{S::内部不一致};
    const auto parents = 共享关系(id.值, t.直接上位, true, c.H, c);
    if (parents.empty()) throw 失败{S::内部不一致};
    std::set<std::uint64_t> seen;
    for (const auto &e : parents) {
        if (e.角色或顺序 != 1 || e.源节点 == id.值 || !seen.insert(e.源节点.值).second)
            throw 失败{S::内部不一致};
        (void)节点(e.源节点, c.G, c.H);
        if (e.源节点 != f.本体根.值 && one(e.源节点, t.概念根归属, c.H).目标节点 != f.本体根.值)
            throw 失败{S::引用冲突};
        f.直接上位组.push_back({e.编码, 概念树概念身份{e.源节点}, id, {e.创建事实代次, e.退出事实代次}});
        out.自有事实.push_back(e.编码);
    }
    std::sort(f.直接上位组.begin(), f.直接上位组.end(), [](const auto &a, const auto &b) { return 身份小于(a.上位, b.上位); });
    return out;
}


概念树写入头 概念树类数据服务::存在请求头(const 概念树存在写请求_v2 &request) {
    return std::visit([](const auto &r) -> 概念树写入头 {
        if constexpr (std::is_same_v<std::decay_t<decltype(r)>, 概念树存在治理请求_v2>)
            return std::visit([](const auto &op) { return op.写入头; }, r.操作);
        else return r.写入头;
    }, request);
}
概念树存在写请求_v2 概念树类数据服务::规范存在请求(const 概念树存在写请求_v2 &input) const {
    if(!existence_layout_)throw 失败{S::不支持};
    if (input.valueless_by_exception()) throw 失败{S::入口拒绝};
    auto request = input;
    std::visit([&](auto &r) {
        using T = std::decay_t<decltype(r)>;
        if (r.版本 != 2) throw 失败{S::入口拒绝};
        共享预算有效(r.预算);
        if constexpr (std::is_same_v<T, 概念树存在建立请求_v2>) {
            r.定义 = std::get<概念树存在定义>(规范化定义(r.定义));
            检查数量(r.直接上位.size(), r.预算.基础.最大关系数);
            if (r.直接上位.empty() || r.依据.来源.valueless_by_exception() || !r.依据.H ||
                r.依据.H > r.写入头.期望事实代次 ||
                !std::visit([](const auto &v) { return 有效(v.值); }, r.依据.来源)) throw 失败{S::入口拒绝};
            std::sort(r.直接上位.begin(), r.直接上位.end(), 身份小于);
            if (std::adjacent_find(r.直接上位.begin(), r.直接上位.end()) != r.直接上位.end()) throw 失败{S::入口拒绝};
        } else if constexpr (std::is_same_v<T, 概念树存在依据释放请求_v2>) {
            检查数量(r.预期.size(), r.预算.基础.最大来源数);
            if (!有效(r.所属EC.值) || r.预期.empty()) throw 失败{S::入口拒绝};
            std::sort(r.预期.begin(), r.预期.end(), [](const auto &a, const auto &b) { return a.引用关系.值 < b.引用关系.值; });
            for (std::size_t i = 0; i < r.预期.size(); ++i)
                if (r.预期[i].所属EC != r.所属EC || !有效(r.预期[i].引用关系) ||
                    (i && r.预期[i-1].引用关系 == r.预期[i].引用关系)) throw 失败{S::入口拒绝};
        } else {
            if (r.操作.valueless_by_exception()) throw 失败{S::入口拒绝};
            std::visit([&](auto &op) {
                using O = std::decay_t<decltype(op)>;
                if (op.预算 != r.预算.基础) throw 失败{S::入口拒绝};
                op = std::get<O>(规范写请求(概念树业务写请求{op}));
            }, r.操作);
        }
    }, request);
    const auto head = 存在请求头(request);
    if (head.合同版本 != 1 || !head.期望事实代次 || !head.幂等身份) throw 失败{S::入口拒绝};
    return request;
}
概念树类数据服务::存在准备结果_v2 概念树类数据服务::准备存在写入(
    const 概念树存在写请求_v2 &request, 共享读取上下文 &c) const {
    存在准备结果_v2 out;
    auto &builder = out.构造;
    const auto &t = layout_.类型;
    const auto head = 存在请求头(request);
    builder.写集.期望事实代次 = head.期望事实代次; builder.写集.写入幂等身份 = {head.幂等身份};
    const auto reachable = [](const auto &edges, 概念树概念身份 from, 概念树概念身份 to) {
        std::set<std::uint64_t> seen{from.值.值}; std::vector<概念树概念身份> q{from};
        for (std::size_t i = 0; i < q.size(); ++i) {
            if (q[i] == to) return true;
            for (const auto &e : edges) if (e.上位 == q[i] && seen.insert(e.下位.值.值).second) q.push_back(e.下位);
        }
        return false;
    };
    auto parent = [&](概念树概念身份 id) {
        const auto fact = 应用定义(id, c);
        if (std::visit([&](const auto &v) { return v.本体根 != layout_.根组.存在根; }, fact)) throw 失败{S::引用冲突};
        if (id != layout_.根组.存在根 && !std::holds_alternative<概念树应用存在事实>(fact)) throw 失败{S::引用冲突};
    };
    std::visit([&](const auto &r) {
        using T = std::decay_t<decltype(r)>;
        if constexpr (std::is_same_v<T, 概念树存在建立请求_v2>) {
            for (auto id : r.定义.特征模板组) (void)应用模板(id, c);
            for (auto id : r.直接上位) parent(id);
            if (const auto *e = std::get_if<概念树存在引用>(&r.依据.来源)) {
                验证世界(*e, c.G, r.依据.H, c.预算.基础);
                验证世界(*e, c.G, c.H, c.预算.基础);
            } else {
                概念树应用存在事实 source;
                { 共享截止作用域 at(c, r.依据.H); source = 展开存在定义_v2(std::get<概念树概念身份>(r.依据.来源), c).事实; }
                if (r.定义.特征模板组.size() >= source.定义.特征模板组.size() ||
                    !std::includes(source.定义.特征模板组.begin(), source.定义.特征模板组.end(),
                        r.定义.特征模板组.begin(), r.定义.特征模板组.end(), 身份小于)) throw 失败{S::引用冲突};
                // 新当前引用必须可建立；历史恢复仍在原写 G 使用相同事实。
                (void)节点(source.概念.值, c.G, c.H);
            }
            const auto graph = 应用图(概念树根角色::存在, false, c);
            std::uint64_t sources = 0;
            for (const auto &node : graph.节点组) if (const auto *ec = std::get_if<概念树应用存在事实>(&node)) {
                if (ec->定义 == r.定义) {
                    if (out.复用) throw 失败{S::内部不一致};
                    out.复用 = *ec;
                }
                sources += ec->世界来源组.size() + ec->抽象依据组.size();
            }
            if (out.复用) return;
            检查数量(graph.节点组.size() + 1, c.预算.基础.最大概念数);
            if (sources >= c.预算.基础.最大来源数 ||
                r.直接上位.size() > c.预算.基础.最大关系数 - graph.直接边组.size()) throw 失败{S::数量预算不足};
            const auto cn = builder.节点(), dn = builder.节点(), sn = builder.节点();
            builder.关系(Ref{cn}, Ref{layout_.根组.存在根.值}, t.概念根归属);
            builder.关系(Ref{cn}, Ref{layout_.签名规则.值}, t.概念规则);
            builder.关系(Ref{cn}, Ref{dn}, t.定义成员);
            for (auto id : r.直接上位) builder.关系(Ref{id.值}, Ref{cn}, t.直接上位);
            std::int64_t order = 0;
            for (auto id : r.定义.特征模板组) builder.关系(Ref{dn}, Ref{id.值}, t.定义模板, ++order);
            builder.关系(Ref{cn}, Ref{sn}, t.来源成员);
            if (const auto *e = std::get_if<概念树存在引用>(&r.依据.来源)) builder.关系(Ref{sn}, Ref{e->值}, t.来源存在);
            else builder.关系(Ref{sn}, Ref{std::get<概念树概念身份>(r.依据.来源).值}, existence_layout_->抽象依据目标关系类型);
            builder.值(Ref{cn}, t.概念种类, std::int64_t{2}); builder.值(Ref{cn}, t.生命周期, std::int64_t{1});
            builder.值(Ref{sn}, t.证据截止, std::vector<std::uint64_t>{r.依据.H});
        } else if constexpr (std::is_same_v<T, 概念树存在依据释放请求_v2>) {
            const auto fact = 展开存在定义_v2(r.所属EC, c).事实;
            auto project = [&](auto v) {
                for (auto *life : {&v.记录生命周期, &v.成员生命周期, &v.引用生命周期})
                    if (life->退出事实代次 && *life->退出事实代次 > c.H) life->退出事实代次.reset();
                v.当前引用已释放 = v.引用生命周期.退出事实代次.has_value(); return v;
            };
            for (const auto &expected : r.预期) {
                const auto found = std::find_if(fact.抽象依据组.begin(), fact.抽象依据组.end(), [&](const auto &v) { return v.引用关系 == expected.引用关系; });
                if (found == fact.抽象依据组.end() || project(*found) != project(expected) || project(*found).当前引用已释放)
                    throw 失败{S::引用冲突};
                builder.写集.退出事实.push_back(found->引用关系);
            }
        } else std::visit([&](const auto &op) {
            using O = std::decay_t<decltype(op)>;
            const auto id = [&] { if constexpr (std::is_same_v<O, 概念树上位操作请求>) return op.下位; else return op.概念; }();
            if (id == layout_.根组.存在根) throw 失败{S::根不可退出};
            const auto expanded = 展开存在定义_v2(id, c); const auto &fact = expanded.事实;
            if constexpr (std::is_same_v<O, 概念树生命周期迁移请求>) {
                if (fact.生命周期值事实 != op.当前生命周期值编码 || fact.治理状态 != op.预期状态) throw 失败{S::引用冲突};
                if (op.预期状态 == op.目标状态) { out.复用 = fact; return; }
                using L = 概念树生命周期状态;
                if (!((op.预期状态 == L::活跃 && (op.目标状态 == L::冷却 || op.目标状态 == L::退役)) ||
                      (op.预期状态 == L::冷却 && (op.目标状态 == L::活跃 || op.目标状态 == L::退役)) ||
                      (op.预期状态 == L::退役 && op.目标状态 == L::活跃))) throw 失败{S::入口拒绝};
                builder.写集.退出事实.push_back(op.当前生命周期值编码);
                builder.值(Ref{id.值}, t.生命周期, static_cast<std::int64_t>(op.目标状态));
            } else {
                auto graph = 应用图(概念树根角色::存在, false, c);
                if constexpr (std::is_same_v<O, 概念树上位操作请求>) {
                    if (op.旧关系) {
                        auto old = std::find_if(graph.直接边组.begin(), graph.直接边组.end(), [&](const auto &e) {
                            return e.关系 == *op.旧关系 && e.上位 == *op.旧上位 && e.下位 == id; });
                        if (old == graph.直接边组.end()) throw 失败{S::引用冲突};
                        builder.写集.退出事实.push_back(old->关系); graph.直接边组.erase(old);
                    }
                    if (op.新上位) {
                        parent(*op.新上位);
                        if (reachable(graph.直接边组, id, *op.新上位)) throw 失败{S::形成环};
                        if (std::any_of(graph.直接边组.begin(), graph.直接边组.end(), [&](const auto &e) { return e.上位 == *op.新上位 && e.下位 == id; })) throw 失败{S::引用冲突};
                        builder.关系(Ref{op.新上位->值}, Ref{id.值}, t.直接上位);
                        graph.直接边组.push_back({{}, *op.新上位, id, {c.H, std::nullopt}});
                    }
                    if (std::none_of(graph.直接边组.begin(), graph.直接边组.end(), [&](const auto &e) { return e.下位 == id; })) throw 失败{S::引用冲突};
                } else {
                    if (fact.治理状态 != 概念树生命周期状态::退役 || fact.生命周期值事实 != op.当前退役生命周期值编码) throw 失败{S::引用冲突};
                    if (!支持组(id, std::nullopt, c.G, c.H, c.预算.基础, &c).empty()) throw 失败{S::引用冲突};
                    for (auto type : {t.定义模板, existence_layout_->抽象依据目标关系类型})
                        if (!共享关系(id.值, type, true, c.H, c).empty()) throw 失败{S::引用冲突};
                    if (dynamic_) for (auto type : {dynamic_->类型.槽模板, dynamic_->类型.来源下位概念})
                        if (!共享关系(id.值, type, true, c.H, c).empty()) throw 失败{S::引用冲突};
                    std::vector<概念树概念身份> parents, children;
                    builder.写集.退出事实 = expanded.自有事实;
                    for (const auto &edge : graph.直接边组) {
                        if (edge.下位 == id) parents.push_back(edge.上位);
                        if (edge.上位 == id) children.push_back(edge.下位);
                        if (edge.下位 == id || edge.上位 == id) builder.写集.退出事实.push_back(edge.关系);
                    }
                    std::erase_if(graph.直接边组, [&](const auto &e) { return e.上位 == id || e.下位 == id; });
                    std::sort(parents.begin(), parents.end(), 身份小于); std::sort(children.begin(), children.end(), 身份小于);
                    for (auto p : parents) for (auto child : children) {
                        if (reachable(graph.直接边组, p, child)) continue;
                        if (reachable(graph.直接边组, child, p)) throw 失败{S::形成环};
                        builder.关系(Ref{p.值}, Ref{child.值}, t.直接上位);
                        graph.直接边组.push_back({{}, p, child, {c.H, std::nullopt}});
                    }
                }
                检查数量(graph.直接边组.size(), c.预算.基础.最大关系数);
            }
        }, r.操作);
    }, request);
    builder.规范(); 检查数量(builder.写集.关系.size(), c.预算.基础.最大关系数);
    return out;
}
概念树存在写入结果_v2 概念树类数据服务::存在执行(const 概念树存在写请求_v2 &input) {
    概念树存在写入结果_v2 out;
    bool 首次未确定 = false, 已派发 = false;
    try {
        out.原请求 = input;
        auto request = 规范存在请求(input);
        out.原请求 = request;
        const auto head = 存在请求头(request);
        const auto budget = std::visit([](const auto &r) { return r.预算; }, request);
        std::scoped_lock lock(mutex_);
        if (feature_pending_ || shared_pending_ || application_pending_) throw 失败{S::前次写入待收敛};
        首次未确定 = true;
        const auto first = port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, {head.幂等身份}});
        if (first.状态 != L1所有者范围读取状态::成功 && first.状态 != L1所有者范围读取状态::未找到)
            throw 失败{映射(first.状态)};
        if (first.合同版本 != L1所有者范围首次写入读取合同版本 || first.所有者 != port_.所有者身份() ||
            first.写入幂等身份.值 != head.幂等身份 || !first.读取事实代次)
            throw 失败{S::内部不一致};
        out.Gread = first.读取事实代次;
        if (first.首次写入结果 && first.首次写入结果->事实代次 && first.首次写入结果->事实代次 <= out.Gread)
            out.首次H = first.首次写入结果->事实代次;
        守卫代次(out.Gread);
        if (first.状态 == L1所有者范围读取状态::未找到) {
            if (first.首次规范化写集 || first.首次写入结果) throw 失败{S::内部不一致};
            首次未确定 = false;
        }
        if (first.状态 == L1所有者范围读取状态::成功) {
            if (!first.首次规范化写集 || !first.首次写入结果 || !out.首次H)
                throw 失败{S::内部不一致};
            if (first.首次规范化写集->期望事实代次 != head.期望事实代次) {
                首次未确定 = false;
                throw 失败{S::幂等冲突};
            }
            共享读取上下文 before{out.Gread, head.期望事实代次, budget};
            存在准备结果_v2 prepared;
            try { prepared = 准备存在写入(request, before); }
            catch (const 失败 &x) {
                if (x.状态 == S::入口拒绝 || x.状态 == S::引用冲突 || x.状态 == S::未找到 ||
                    x.状态 == S::目标已退出 || x.状态 == S::形成环 || x.状态 == S::根不可退出) {
                    首次未确定 = false;
                    throw 失败{S::幂等冲突};
                }
                throw;
            }
            if (prepared.构造.写集 != *first.首次规范化写集) {
                首次未确定 = false;
                throw 失败{S::幂等冲突};
            }
            auto &after = before; after.H = *out.首次H;
            out = 存在发布读回(request, prepared.构造.写集, *first.首次写入结果, after);
            out.状态 = S::精确重复;
            守卫代次(after.G);
            if (!out.成功()) throw 失败{S::内部不一致};
            return out;
        }
        if (first.首次规范化写集 || first.首次写入结果) throw 失败{S::内部不一致};
        首次未确定 = false;
        if (out.Gread != head.期望事实代次) throw 失败{S::事实代次漂移};
        共享读取上下文 c{out.Gread, out.Gread, budget};
        auto prepared = 准备存在写入(request, c);
        auto &ws = prepared.构造.写集;
        if (ws.节点.empty() && ws.关系.empty() && ws.值.empty() && ws.退出事实.empty() && ws.属性槽变更.empty()) {
            out.状态 = S::无须变更;
            out.概念 = std::move(prepared.复用);
            守卫代次(c.G);
            if (!out.成功()) throw 失败{S::内部不一致};
            return out;
        }
        守卫代次(c.G);
        已派发 = true;
        const auto saved = port_.提交所有者范围中性写集(ws);
        if (saved.事实代次 > head.期望事实代次 &&
            (saved.是否形成内存权威发布 || saved.状态 == L1所有者范围写入状态::精确重复))
            out.首次H = saved.事实代次;
        const auto g = 当前代次();
        out.Gread = g;
        auto &after = c; after.动态用量.已核验.clear(); after.G = g; after.H = saved.事实代次;
        out = 存在发布读回(request, ws, saved, after);
        if (saved.状态 == L1所有者范围写入状态::精确重复) out.状态 = S::精确重复;
        守卫代次(g);
        if (!out.成功()) throw 失败{S::内部不一致};
        return out;
    } catch (const 失败 &x) { out.状态 = x.状态; }
    catch (const std::bad_alloc &) { out.状态 = S::资源失败; }
    catch (const std::length_error &) { out.状态 = S::资源失败; }
    catch (...) { out.状态 = S::内部不一致; }
    out.概念.reset(); out.关系组.clear(); out.已退出事实.clear();
    if (首次未确定 || 已派发) {
        out.状态 = S::已可能发布;
        out.发布状态 = P::可能已发布;
    } else out.发布状态 = out.首次H ? P::可能已发布 : P::未派发;
    return out;
}



概念树存在写入结果_v2 概念树类数据服务::存在发布读回(const 概念树存在写请求_v2 &request,
    const L1所有者范围写集请求 &ws, const L1所有者范围写入结果 &saved, 共享读取上下文 &c) const {
    const auto mapped = 共享核验首次(ws, saved, c);
    概念树存在写入结果_v2 out;
    out.原请求 = request; out.Gread = c.G; out.首次H = saved.事实代次; out.发布状态 = P::已确认发布;
    out.已退出事实 = ws.退出事实;
    std::visit([&](const auto &r) {
        using T = std::decay_t<decltype(r)>;
        if constexpr (std::is_same_v<T, 概念树存在建立请求_v2>) {
            out.概念 = 展开存在定义_v2(概念树概念身份{mapped.at(1)}, c).事实;
            if (out.概念->定义 != r.定义 || out.概念->生命周期.创建事实代次 != c.H) throw 失败{S::内部不一致};
            out.状态 = S::已创建;
        } else if constexpr (std::is_same_v<T, 概念树存在依据释放请求_v2>) {
            out.概念 = 展开存在定义_v2(r.所属EC, c).事实;
            for (const auto &expected : r.预期) {
                auto found = std::find_if(out.概念->抽象依据组.begin(), out.概念->抽象依据组.end(), [&](const auto &v) { return v.引用关系 == expected.引用关系; });
                if (found == out.概念->抽象依据组.end() || !found->当前引用已释放 || found->引用生命周期.退出事实代次 != c.H)
                    throw 失败{S::内部不一致};
            }
            out.状态 = S::已释放引用;
        } else std::visit([&](const auto &op) {
            using O = std::decay_t<decltype(op)>;
            const auto id = [&] { if constexpr (std::is_same_v<O, 概念树上位操作请求>) return op.下位; else return op.概念; }();
            if constexpr (std::is_same_v<O, 概念树概念退出请求>) {
                { 共享截止作用域 before(c, op.写入头.期望事实代次); out.概念 = 展开存在定义_v2(id, c).事实; }
                const auto raw = 原始事实(id.值, c.G);
                const auto *n = std::get_if<N>(&raw);
                if (!n || n->退出事实代次 != c.H || std::find(ws.退出事实.begin(), ws.退出事实.end(), id.值) == ws.退出事实.end())
                    throw 失败{S::内部不一致};
                out.概念->生命周期.退出事实代次 = c.H;
                const auto graph = 应用图(概念树根角色::存在, false, c);
                for (const auto &e : graph.直接边组) for (const auto &[local, real] : mapped)
                    if (real == e.关系) out.关系组.push_back(e);
                out.状态 = S::已退出概念;
            } else {
                out.概念 = 展开存在定义_v2(id, c).事实;
                if constexpr (std::is_same_v<O, 概念树生命周期迁移请求>) {
                    if (out.概念->治理状态 != op.目标状态) throw 失败{S::内部不一致};
                    out.状态 = S::已迁移生命周期;
                } else {
                    if (op.旧关系 && std::any_of(out.概念->直接上位组.begin(), out.概念->直接上位组.end(), [&](const auto &e) { return e.关系 == *op.旧关系; })) throw 失败{S::内部不一致};
                    if (op.新上位) {
                        auto e = std::find_if(out.概念->直接上位组.begin(), out.概念->直接上位组.end(), [&](const auto &v) { return v.上位 == *op.新上位; });
                        if (e == out.概念->直接上位组.end()) throw 失败{S::内部不一致};
                        out.关系组.push_back(*e);
                    }
                    out.状态 = op.操作 == 概念树上位操作::新增 ? S::已添加上位 :
                        op.操作 == 概念树上位操作::替换 ? S::已替换上位 : S::已退出上位;
                }
            }
        }, r.操作);
    }, request);
    return out;
}
bool 概念树存在写入结果_v2::成功() const noexcept {
    using S = 概念树数据状态;
    if (版本 != 2 || !Gread || !原请求 || 原请求->valueless_by_exception() || !概念 ||
        !概念->概念.值.值 || !概念->本体根.值.值 || !概念->规则.值.值 || !概念->定义记录.值 ||
        !概念->生命周期值事实.值 || !概念->生命周期.创建事实代次 || 概念->定义.特征模板组.empty() ||
        概念->直接上位组.empty() || (概念->世界来源组.empty() == 概念->抽象依据组.empty()) ||
        (概念->治理状态 != 概念树生命周期状态::活跃 && 概念->治理状态 != 概念树生命周期状态::冷却 &&
         概念->治理状态 != 概念树生命周期状态::退役)) return false;
    if (const auto *govern = std::get_if<概念树存在治理请求_v2>(&*原请求); govern && govern->操作.valueless_by_exception()) return false;
    if (状态 == S::无须变更) {
        if (首次H || 发布状态 != 概念树发布状态::未派发 || !已退出事实.empty()) return false;
    } else if (!首次H || *首次H > Gread || *首次H <= 概念树类数据服务::存在请求头(*原请求).期望事实代次 ||
               发布状态 != 概念树发布状态::已确认发布) return false;
    for (const auto &a : 概念->抽象依据组)
        if (a.所属EC != 概念->概念 || !a.来源EC.值.值 || !a.记录.值 || !a.成员关系.值 || !a.引用关系.值 ||
            !a.截止值事实.值 || !a.证据H || a.证据H >= a.记录生命周期.创建事实代次) return false;
    const bool repeated = 状态 == S::精确重复;
    return std::visit([&](const auto &r) {
        using T = std::decay_t<decltype(r)>;
        if constexpr (std::is_same_v<T, 概念树存在建立请求_v2>) return
            (repeated || 状态 == S::无须变更 || 状态 == S::已创建) && 概念->定义 == r.定义;
        else if constexpr (std::is_same_v<T, 概念树存在依据释放请求_v2>) return
            (repeated || 状态 == S::已释放引用) && 概念->概念 == r.所属EC && 已退出事实.size() == r.预期.size();
        else {
            if (r.操作.valueless_by_exception()) return false;
            return std::visit([&](const auto &op) {
                using O = std::decay_t<decltype(op)>;
                if constexpr (std::is_same_v<O, 概念树生命周期迁移请求>) return
                    (repeated || 状态 == S::无须变更 || 状态 == S::已迁移生命周期) && 概念->概念 == op.概念 && 概念->治理状态 == op.目标状态;
                else if constexpr (std::is_same_v<O, 概念树概念退出请求>) return
                    (repeated || 状态 == S::已退出概念) && 概念->概念 == op.概念 && 概念->生命周期.退出事实代次 == 首次H;
                else return (repeated || 状态 == S::已添加上位 || 状态 == S::已替换上位 || 状态 == S::已退出上位) && 概念->概念 == op.下位;
            }, r.操作);
        }
    }, *原请求);
}
概念树存在写入结果_v2 概念树类数据服务::创建存在概念(const 概念树存在建立请求_v2 &r) { return 存在执行(r); }
概念树存在写入结果_v2 概念树类数据服务::释放存在抽象依据(const 概念树存在依据释放请求_v2 &r) { return 存在执行(r); }
概念树存在写入结果_v2 概念树类数据服务::治理存在概念(const 概念树存在治理请求_v2 &r) { return 存在执行(r); }
概念树应用写入结果 概念树类数据服务::执行应用概念写入(const 概念树应用写请求 &input) {
    概念树应用写入结果 out;
    bool unknown = false, dispatched = false, confirmedZero = false;
    std::unique_lock<std::mutex> lock(mutex_);
    try {
        out.原请求 = input;
        if (input.版本 != 1 || input.操作.valueless_by_exception()) throw 失败{S::入口拒绝};
        const auto head = std::visit([](const auto &r) { return 请求写头(r); }, input.操作);
        if (head.合同版本 != 1 || !head.期望事实代次 || !head.幂等身份) throw 失败{S::入口拒绝};
        if (feature_pending_ || shared_pending_ || (application_pending_ && *application_pending_ != input))
            throw 失败{S::前次写入待收敛};
        unknown = true;
        const auto first = port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, {head.幂等身份}});
        if (first.状态 != L1所有者范围读取状态::成功 && first.状态 != L1所有者范围读取状态::未找到) throw 失败{映射(first.状态)};
        if (first.合同版本 != L1所有者范围首次写入读取合同版本 || first.所有者 != port_.所有者身份() ||
            first.写入幂等身份.值 != head.幂等身份 || !first.读取事实代次) throw 失败{S::内部不一致};
        out.Gread = first.读取事实代次;
        if (first.首次写入结果 && first.首次写入结果->事实代次 && first.首次写入结果->事实代次 <= out.Gread)
            out.首次H = first.首次写入结果->事实代次;
        守卫代次(out.Gread);
        if (first.状态 == L1所有者范围读取状态::未找到) {
            if (first.首次规范化写集 || first.首次写入结果) throw 失败{S::内部不一致};
            unknown = false;
        }
        共享预算有效(input.预算);
        auto request = input;
        std::visit([&](auto &r) {
            using T = std::decay_t<decltype(r)>;
            if (r.预算 != input.预算.基础) throw 失败{S::入口拒绝};
            if constexpr (std::is_same_v<T, 概念树形成引用释放请求>)
                std::sort(r.预期当前引用.begin(), r.预期当前引用.end(), [](const auto &a,const auto &b){return 编码小于(a.关系,b.关系);});
            else if constexpr (std::is_same_v<T, 概念树动态依据释放请求>)
                std::sort(r.预期当前依据.begin(), r.预期当前依据.end(), [](const auto &a,const auto &b){return 编码小于(a.引用关系,b.引用关系);});
        }, request.操作);
        auto oldRequest = 规范写请求(std::visit([](const auto &r) -> 概念树业务写请求 { return r; }, request.操作));
        std::visit([&](const auto &r) {
            if constexpr (!std::is_same_v<std::decay_t<decltype(r)>, 概念树概念建立请求>) request.操作 = r;
            else throw 失败{S::内部不一致};
        }, oldRequest);
        // 外层原请求及 pending 保留调用方输入，规范化仅服务于同一写集形成。
        共享读取上下文 c{out.Gread, head.期望事实代次, request.预算};
        std::optional<概念树共享写请求> featureGovernance;
        std::optional<概念树共享写请求> featureRelease;
        const auto prepare = [&] {
            featureGovernance.reset(); featureRelease.reset();
            std::visit([&](const auto &r) {
                using T = std::decay_t<decltype(r)>;
                if constexpr (std::is_same_v<T, 概念树上位操作请求> || std::is_same_v<T, 概念树生命周期迁移请求> ||
                              std::is_same_v<T, 概念树概念退出请求>) {
                    const auto id = [&] { if constexpr (std::is_same_v<T, 概念树上位操作请求>) return r.下位; else return r.概念; }();
                    const auto raw = 节点(id.值, c.G, c.H);
                    if (id == layout_.根组.特征根 || id == layout_.根组.动态根 || id == layout_.根组.存在根 || id == layout_.根组.因果链根)
                        throw 失败{S::根不可退出};
                    const auto av = 属性(id.值, c.G, c.H);
                    const auto kind = std::get<std::int64_t>(唯一属性(av, layout_.类型.概念种类).材料);
                    if (kind == 4) {
                        if (!feature_layout_) throw 失败{S::不支持};
                        const auto f = 展开共享定义(id, c).事实;
                        if constexpr (std::is_same_v<T, 概念树上位操作请求>) {
                            概念树共享上位请求 op{2, r.写入头, r.下位, f.直接上位, {}, request.预算};
                            for (const auto& e : f.直接上位) {
                                if (r.旧关系 && e.关系 == *r.旧关系) {
                                    if (e.上位 != *r.旧上位) throw 失败{S::引用冲突};
                                } else op.新父组.push_back(e.上位);
                            }
                            if (r.旧关系 && std::none_of(f.直接上位.begin(), f.直接上位.end(),
                                [&](const auto& e) { return e.关系 == *r.旧关系; })) throw 失败{S::引用冲突};
                            if (r.新上位) {
                                if (std::find(op.新父组.begin(), op.新父组.end(), *r.新上位) != op.新父组.end())
                                    throw 失败{S::引用冲突};
                                op.新父组.push_back(*r.新上位);
                            }
                            featureGovernance = std::move(op);
                        } else if constexpr (std::is_same_v<T, 概念树生命周期迁移请求>) {
                            // 通用请求不承载重新采用见证；恢复须使用具名共享入口。
                            featureGovernance = 概念树共享生命周期请求{2, r, std::nullopt, request.预算};
                        } else {
                            概念树共享退出请求 op{2, r, {}, request.预算};
                            auto graph = 应用图(概念树根角色::特征, false, c);
                            std::vector<概念树概念身份> parents, children;
                            for (const auto& e : graph.直接边组) {
                                if (e.下位 == id) parents.push_back(e.上位);
                                if (e.上位 == id) children.push_back(e.下位);
                            }
                            std::sort(parents.begin(), parents.end(), 身份小于);
                            std::sort(children.begin(), children.end(), 身份小于);
                            std::erase_if(graph.直接边组, [&](const auto& e) { return e.上位 == id || e.下位 == id; });
                            auto reachable = [&](概念树概念身份 from, 概念树概念身份 to) {
                                std::vector<概念树概念身份> stack{from}; std::set<std::uint64_t> seen;
                                while (!stack.empty()) {
                                    auto next = stack.back(); stack.pop_back();
                                    if (next == to) return true;
                                    if (!seen.insert(next.值.值).second) continue;
                                    检查数量(seen.size(), request.预算.基础.最大概念数);
                                    for (const auto& e : graph.直接边组) if (e.上位 == next) stack.push_back(e.下位);
                                }
                                return false;
                            };
                            for (auto child : children) {
                                const auto cf = 展开共享定义(child, c).事实;
                                概念树共享子父替代 change{child, cf.直接上位, {}};
                                for (const auto& e : cf.直接上位) if (e.上位 != id) change.新父组.push_back(e.上位);
                                for (auto parent : parents) {
                                    if (reachable(parent, child)) continue;
                                    if (reachable(child, parent)) throw 失败{S::形成环};
                                    change.新父组.push_back(parent);
                                    graph.直接边组.push_back({{}, parent, child, {c.H, std::nullopt}});
                                    检查数量(graph.直接边组.size(), request.预算.基础.最大关系数);
                                }
                                op.完整替代.push_back(std::move(change));
                            }
                            featureGovernance = std::move(op);
                        }
                        return;
                    }
                    if (kind != 1 && kind != 3) throw 失败{S::不支持};
                    const auto root = 唯一关系(id.值, layout_.类型.概念根归属, c.G, c.H);
                    if (root.目标节点 != (kind == 1 ? layout_.根组.特征根.值 : layout_.根组.动态根.值)) throw 失败{S::引用冲突};
                }
            }, request.操作);
            if (featureGovernance) {
                (void)规范共享请求(*featureGovernance);
                return 准备共享写入(*featureGovernance, c).构造.写集;
            }
            auto ws = 形成写集(oldRequest, c.G, c.H, &c);
            if (const auto* r = std::get_if<概念树形成引用释放请求>(&request.操作); r && feature_layout_) {
                概念树共享释放请求 op{2, r->写入头, {}, request.预算};
                for (const auto& expected : r->预期当前引用) {
                    const auto raw = 原始事实(expected.关系, c.G);
                    const auto* e = std::get_if<E>(&raw);
                    if (!e) throw 失败{S::引用冲突};
                    if (e->关系类型节点 == feature_layout_->类型[16])
                        op.完整预期.push_back({expected.关系, expected.记录, 世界编码(r->世界引用),
                            expected.所属概念, 17, expected.关系生命周期});
                }
                if (!op.完整预期.empty()) {
                    featureRelease = std::move(op); (void)规范共享请求(*featureRelease);
                    const auto part = 准备共享写入(*featureRelease, c);
                    // 释放没有新增本地键；共享内核核对同一笔完整预期中的新 C 子集。
                    for (auto id : part.构造.写集.退出事实)
                        if (std::count(ws.退出事实.begin(), ws.退出事实.end(), id) != 1) throw 失败{S::内部不一致};
                }
            }
            return ws;
        };
        const auto readback = [&](const L1所有者范围写集请求 &ws, const L1所有者范围写入结果 &saved) {
            if (featureGovernance) {
                const auto actual = 共享发布读回(*featureGovernance, ws, saved, c);
                if (!actual.成功()) throw 失败{S::内部不一致};
                out.Gread = c.G; out.首次H = saved.事实代次; out.发布状态 = P::已确认发布;
                out.状态 = actual.状态; out.已退出事实 = actual.已退出编码; out.关系组 = actual.直接边;
                if (actual.概念) out.概念 = *actual.概念;
                if (const auto* op = std::get_if<概念树上位操作请求>(&request.操作))
                    out.状态 = op->操作 == 概念树上位操作::新增 ? S::已添加上位
                        : op->操作 == 概念树上位操作::替换 ? S::已替换上位 : S::已退出上位;
                return;
            }
            if (featureRelease) {
                // 完整写集由通用发布读回核对，新 C 子集复用共享释放的业务读回。
                核验共享释放读回(std::get<概念树共享释放请求>(*featureRelease), saved.事实代次, c);
            }
            auto actual = 发布结果(oldRequest, ws, saved, c.G, &c);
            out.Gread = c.G; out.首次H = saved.事实代次; out.发布状态 = P::已确认发布;
            out.状态 = actual.状态; out.支持 = std::move(actual.支持);
            if (actual.概念) out.概念 = std::move(*actual.概念);
            out.关系组 = std::move(actual.关系组); out.已退出事实 = std::move(actual.已退出事实);
        };
        if (first.状态 == L1所有者范围读取状态::成功) {
            if (!first.首次规范化写集 || !first.首次写入结果 || !out.首次H) throw 失败{S::内部不一致};
            if (first.首次规范化写集->期望事实代次 != head.期望事实代次) {
                unknown = false; throw 失败{S::幂等冲突};
            }
            L1所有者范围写集请求 expected;
            try { expected = prepare(); }
            catch (const 失败 &e) {
                if (e.状态 == S::入口拒绝 || e.状态 == S::引用冲突 || e.状态 == S::未找到 ||
                    e.状态 == S::目标已退出 || e.状态 == S::形成环 || e.状态 == S::根不可退出) {
                    unknown = false; throw 失败{S::幂等冲突};
                }
                throw;
            }
            if (expected != *first.首次规范化写集) { unknown = false; throw 失败{S::幂等冲突}; }
            c.H = *out.首次H;
            readback(expected, *first.首次写入结果);
            out.状态 = S::精确重复;
        } else {
            if (first.首次规范化写集 || first.首次写入结果) throw 失败{S::内部不一致};
            unknown = false;
            if (c.G != c.H) { application_pending_.reset(); throw 失败{S::事实代次漂移}; }
            const auto ws = prepare();
            if (ws.节点.empty() && ws.关系.empty() && ws.值.empty() && ws.退出事实.empty() && ws.属性槽变更.empty()) {
                out.状态 = S::无须变更;
                std::visit([&](const auto &r) {
                    using T = std::decay_t<decltype(r)>;
                    if constexpr (std::is_same_v<T, 概念树动态建立请求>) {
                        const auto graph = 应用图(概念树根角色::动态, false, c);
                        for (const auto &node : graph.节点组) if (const auto *f = std::get_if<概念树概念事实>(&node);
                            f && f->定义 && *f->定义 == 概念树定义{r.定义}) {
                            if (out.概念) throw 失败{S::内部不一致}; out.概念 = node;
                        }
                    } else if constexpr (std::is_same_v<T, 概念树支持建立请求>) {
                        for (const auto &f : 支持组(r.目标概念, std::nullopt, c.G, c.H, r.预算, &c))
                            if (f.世界引用 == r.世界引用) { if (out.支持) throw 失败{S::内部不一致}; out.支持 = f; }
                    } else if constexpr (std::is_same_v<T, 概念树生命周期迁移请求>) out.概念 = 应用定义(r.概念, c);
                }, request.操作);
            } else {
                application_pending_ = input;
                守卫代次(c.G); dispatched = true;
                const auto saved = port_.提交所有者范围中性写集(ws);
                const bool header = saved.合同版本 == L1所有者范围CRUD合同版本 && saved.所有者 == port_.所有者身份()
                    && saved.写入幂等身份 == ws.写入幂等身份;
                if (header && !saved.是否形成内存权威发布 && saved.新编码映射.empty()
                    && saved.状态 != L1所有者范围写入状态::成功 && saved.状态 != L1所有者范围写入状态::精确重复
                    && saved.重试边界 != L1所有者范围重试边界::原幂等身份读回收敛) {
                    confirmedZero = true; application_pending_.reset();
                    switch (saved.状态) {
                    case L1所有者范围写入状态::事实代次漂移: throw 失败{S::事实代次漂移};
                    case L1所有者范围写入状态::幂等冲突: throw 失败{S::幂等冲突};
                    case L1所有者范围写入状态::引用冲突: throw 失败{S::引用冲突};
                    case L1所有者范围写入状态::未找到: throw 失败{S::未找到};
                    case L1所有者范围写入状态::已退出: throw 失败{S::目标已退出};
                    case L1所有者范围写入状态::资源失败: throw 失败{S::资源失败};
                    case L1所有者范围写入状态::入口拒绝: case L1所有者范围写入状态::许可拒绝: throw 失败{S::入口拒绝};
                    default: throw 失败{S::内部不一致};
                    }
                }
                if (!header) throw 失败{S::内部不一致};
                if (saved.事实代次 > head.期望事实代次 && (saved.是否形成内存权威发布 || saved.状态 == L1所有者范围写入状态::精确重复))
                    out.首次H = saved.事实代次;
                c.G = 当前代次(); c.H = saved.事实代次; c.动态用量.已核验.clear(); out.Gread = c.G;
                readback(ws, saved);
                if (saved.状态 == L1所有者范围写入状态::精确重复) out.状态 = S::精确重复;
            }
        }
        std::visit([&](const auto &r) {
            using T = std::decay_t<decltype(r)>;
            if constexpr (std::is_same_v<T, 概念树形成引用释放请求> || std::is_same_v<T, 概念树动态依据释放请求>) {
                std::set<std::uint64_t> ids;
                if constexpr (std::is_same_v<T, 概念树形成引用释放请求>)
                    for (const auto &ref : r.预期当前引用) ids.insert(ref.所属概念.值.值);
                else ids.insert(r.所属概念.值.值);
                for (auto id : ids) out.释放所属概念组.push_back(应用定义(概念树概念身份{稳定编码{id}}, c));
            }
        }, request.操作);
        守卫代次(c.G);
        if (!out.成功()) throw 失败{S::内部不一致};
        application_pending_.reset();
        return out;
    } catch (const 失败 &e) { out.状态 = e.状态; }
    catch (const std::bad_alloc &) { out.状态 = S::资源失败; }
    catch (const std::length_error &) { out.状态 = S::资源失败; }
    catch (...) { out.状态 = S::内部不一致; }
    out.概念.reset(); out.支持.reset(); out.关系组.clear(); out.已退出事实.clear(); out.释放所属概念组.clear();
    if ((unknown || dispatched) && !confirmedZero) {
        out.状态 = S::已可能发布; out.发布状态 = P::可能已发布;
        if (!application_pending_) { try { application_pending_ = input; } catch (...) {} }
    } else {
        out.发布状态 = confirmedZero ? P::已确认未发布 : (out.首次H ? P::可能已发布 : P::未派发);
        if (application_pending_ && *application_pending_ == input) application_pending_.reset();
    }
    return out;
}

bool 概念树应用写入结果::成功() const noexcept {
    using S = 概念树数据状态;
    if (版本 != 1 || !Gread || !原请求 || 原请求->版本 != 1 || 原请求->操作.valueless_by_exception() ||
        (概念 && 概念->valueless_by_exception())) return false;
    const auto head = std::visit([](const auto &r) {
        if constexpr (std::is_same_v<std::decay_t<decltype(r)>, 概念树动态建立请求> ||
                      std::is_same_v<std::decay_t<decltype(r)>, 概念树动态依据释放请求>) return r.头;
        else return r.写入头;
    }, 原请求->操作);
    if (head.合同版本 != 1 || !head.期望事实代次 || !head.幂等身份) return false;
    if (状态 == S::无须变更) {
        if (首次H || 发布状态 != 概念树发布状态::未派发 || !关系组.empty() || !已退出事实.empty()) return false;
    } else if (!首次H || *首次H <= head.期望事实代次 || *首次H > Gread || 发布状态 != 概念树发布状态::已确认发布) return false;
    const bool replay = 状态 == S::精确重复;
    return std::visit([&](const auto &r) {
        using T = std::decay_t<decltype(r)>;
        if (r.预算 != 原请求->预算.基础) return false;
        if constexpr (!std::is_same_v<T, 概念树形成引用释放请求> && !std::is_same_v<T, 概念树动态依据释放请求>) {
            if (!释放所属概念组.empty()) return false;
        }
        if constexpr (std::is_same_v<T, 概念树支持建立请求>) return
            (replay || 状态 == S::无须变更 || 状态 == S::已添加支持) && !概念 && 支持 &&
            支持->目标概念 == r.目标概念 && 支持->世界引用 == r.世界引用 && 支持->记录.值 &&
            支持->概念关系.值 && 支持->世界关系.值 && 支持->归属关系.值 && 支持->截止值事实.值 &&
            支持->证据H && 支持->证据H < 支持->记录生命周期.创建事实代次;
        else if constexpr (std::is_same_v<T, 概念树支持退出请求>) return
            (replay || 状态 == S::已退出支持) && !概念 && 支持 && 支持->记录 == r.预期支持.记录 &&
            支持->记录生命周期.退出事实代次 == 首次H && 已退出事实.size() == 5;
        else if constexpr (std::is_same_v<T, 概念树形成引用释放请求> || std::is_same_v<T, 概念树动态依据释放请求>) {
            if ((!replay && 状态 != S::已释放引用 && 状态 != S::无须变更) || 概念 || 支持 || !关系组.empty()) return false;
            const auto &expected = [&]() -> const auto & {
                if constexpr (std::is_same_v<T, 概念树形成引用释放请求>) return r.预期当前引用;
                else return r.预期当前依据;
            }();
            if (已退出事实.size() != expected.size()) return false;
            for (std::size_t i=0;i<已退出事实.size();++i) {
                if (!已退出事实[i].值 || (i && 已退出事实[i-1].值 >= 已退出事实[i].值)) return false;
                const auto count = std::count_if(expected.begin(), expected.end(), [&](const auto &v) {
                    if constexpr (std::is_same_v<T, 概念树形成引用释放请求>) return v.关系 == 已退出事实[i];
                    else return v.引用关系 == 已退出事实[i];
                });
                if (count != 1) return false;
            }
            std::size_t unique=0;
            if constexpr (std::is_same_v<T, 概念树形成引用释放请求>) {
                for (std::size_t i=0;i<expected.size();++i) {
                    if (!expected[i].所属概念.值.值) return false;
                    bool prior=false; for(std::size_t j=0;j<i;++j) if(expected[j].所属概念==expected[i].所属概念) prior=true;
                    if (!prior) ++unique;
                }
            } else unique=1;
            if (释放所属概念组.size()!=unique) return false;
            std::uint64_t previous=0;
            for(const auto &node:释放所属概念组) {
                if (!概念树类数据服务::应用事实完整(node, 首次H.value_or(Gread))) return false;
                const auto id=概念树类数据服务::应用身份(node);
                if(id.值.值<=previous)return false; previous=id.值.值;
                if constexpr (std::is_same_v<T, 概念树形成引用释放请求>) {
                    if(std::none_of(expected.begin(),expected.end(),[&](const auto&v){return v.所属概念==id;}))return false;
                }else if(id!=r.所属概念)return false;
            }
            return true;
        }
        else {
            if (!概念 || 支持) return false;
            if (const auto* feature = std::get_if<特征概念事实>(&*概念)) {
                constexpr bool exiting = std::is_same_v<T, 概念树概念退出请求>;
                if (!新特征概念事实完整(*feature, 首次H.value_or(Gread), exiting)) return false;
                if constexpr (std::is_same_v<T, 概念树上位操作请求>) {
                    const auto expected = r.操作 == 概念树上位操作::新增 ? S::已添加上位
                        : r.操作 == 概念树上位操作::替换 ? S::已替换上位 : S::已退出上位;
                    if ((!replay && 状态 != expected) || feature->身份 != r.下位 || 关系组 != feature->直接上位) return false;
                    if (r.旧关系 && std::any_of(关系组.begin(), 关系组.end(),
                        [&](const auto& e) { return e.关系 == *r.旧关系; })) return false;
                    if (r.旧关系 && std::count(已退出事实.begin(), 已退出事实.end(), *r.旧关系) != 1) return false;
                    return !r.新上位 || std::count_if(关系组.begin(), 关系组.end(),
                        [&](const auto& e) { return e.上位 == *r.新上位 && e.下位 == r.下位; }) == 1;
                } else if constexpr (std::is_same_v<T, 概念树生命周期迁移请求>) {
                    return (replay || 状态 == S::无须变更 || 状态 == S::已迁移生命周期)
                        && feature->身份 == r.概念 && feature->治理状态 == r.目标状态;
                } else if constexpr (exiting) {
                    return (replay || 状态 == S::已退出概念) && feature->身份 == r.概念
                        && feature->生命周期.退出事实代次 == 首次H
                        && std::count(已退出事实.begin(), 已退出事实.end(), feature->身份.值) == 1;
                } else return false;
            }
            const auto *f = std::get_if<概念树概念事实>(&*概念);
            if (!f || f->是本体根 || !f->定义 || f->定义->valueless_by_exception() || !f->概念.值.值 ||
                !f->定义记录.值 || !f->规则.值.值 || !f->生命周期.创建事实代次 || f->直接上位组.empty()) return false;
            if constexpr (std::is_same_v<T, 概念树动态建立请求>) return
                (replay || 状态 == S::无须变更 || 状态 == S::已创建) && std::holds_alternative<概念树动态定义>(*f->定义) &&
                std::get<概念树动态定义>(*f->定义) == r.定义;
            else if constexpr (std::is_same_v<T, 概念树上位操作请求>) return
                (replay || 状态 == S::已添加上位 || 状态 == S::已替换上位 || 状态 == S::已退出上位) && f->概念 == r.下位;
            else if constexpr (std::is_same_v<T, 概念树生命周期迁移请求>) return
                (replay || 状态 == S::无须变更 || 状态 == S::已迁移生命周期) && f->概念 == r.概念 && f->治理状态 == r.目标状态;
            else return (replay || 状态 == S::已退出概念) && f->概念 == r.概念 && f->生命周期.退出事实代次 == 首次H;
        }
    }, 原请求->操作);
}

概念树应用读取结果<概念树应用关联事实> 概念树类数据服务::读取应用概念关联(const 概念树应用关联请求 &input) const {
    概念树读取头 head{};
    if (!input.操作.valueless_by_exception()) head = std::visit([](const auto &r) {
        if constexpr (std::is_same_v<std::decay_t<decltype(r)>, 概念树动态依据审计请求>) return r.头;
        else return r.读取头;
    }, input.操作);
    return 共享读取封装<概念树应用关联事实>(head, input.预算, [&](auto &c) -> 概念树应用关联事实 {
        if (input.版本 != 1 || input.操作.valueless_by_exception()) throw 失败{S::入口拒绝};
        return std::visit([&](const auto &r) -> 概念树应用关联事实 {
            using T = std::decay_t<decltype(r)>;
            if (r.预算 != c.预算.基础) throw 失败{S::入口拒绝};
            if constexpr (std::is_same_v<T, 概念树按概念支持读取请求>) return 支持组(r.目标概念, std::nullopt, c.G, c.H, r.预算, &c);
            else if constexpr (std::is_same_v<T, 概念树按世界支持读取请求>) return 支持组(std::nullopt, r.世界引用, c.G, c.H, r.预算, &c);
            else if constexpr (std::is_same_v<T, 概念树形成引用读取请求>) {
                if (r.读取头.H) throw 失败{S::入口拒绝};
                return 形成引用组(r.世界引用, c.G, c.H, r.预算, &c);
            }
            else if constexpr (std::is_same_v<T, 概念树动态支持核验请求>) {
                if (!dynamic_) throw 失败{S::入口拒绝};
                auto s = 支持记录(r.支持记录, c.G, c.H, r.预算, &c);
                if (s.世界引用.index() != 2) throw 失败{S::引用冲突};
                const auto raw = 原始事实(s.世界关系, c.G); const auto *edge = std::get_if<E>(&raw);
                if (!edge) throw 失败{S::内部不一致};
                s.动态证据 = 审计动态(std::get<概念树动态引用>(s.世界引用), c.G, s.证据H, r.预算, !edge->退出事实代次, &c.动态用量);
                return s;
            } else {
                if (!dynamic_) throw 失败{S::入口拒绝};
                const auto f = 读取概念内部(r.概念, c.G, c.H, r.预算, &c);
                if (f.是本体根 || f.根角色 != 概念树根角色::动态) throw 失败{S::引用冲突};
                std::vector<概念树动态依据审计项> out;
                auto &used = c.动态用量;
                for (const auto &a : f.动态依据组) {
                    auto state = 概念树动态证据状态::已核验;
                    if (const auto *d = std::get_if<概念树动态引用>(&a.依据.目标)) {
                        const auto raw = 原始事实(a.引用关系, c.G); const auto *edge = std::get_if<E>(&raw);
                        if (!edge) throw 失败{S::内部不一致};
                        state = 审计动态(*d, c.G, a.依据.H, r.预算, !edge->退出事实代次, &used);
                    } else try { 验证动态依据(a.依据, c.G, r.预算, &used, &c); }
                    catch (const 失败 &e) {
                        if (e.状态 != S::历史材料不可用) throw;
                        守卫代次(c.G); state = 概念树动态证据状态::历史已清理;
                    }
                    out.push_back({a, state});
                }
                return out;
            }
        }, input.操作);
    });
}

bool 概念树类数据服务::应用事实完整(const 概念树应用定义事实 &node, std::uint64_t h) noexcept {
    if (node.valueless_by_exception() || !h) return false;
    const auto lifeComplete = [](const 概念树生命周期 &life, std::uint64_t H, bool allowExited = false) noexcept {
        return H && life.创建事实代次 && life.创建事实代次 <= H && (!life.退出事实代次 ||
            (*life.退出事实代次 > life.创建事实代次 && (*life.退出事实代次 > H || allowExited)));
    };
    return std::visit([&](const auto &f) {
        using T = std::decay_t<decltype(f)>;
        if constexpr (std::is_same_v<T, 概念树概念事实>) return 概念完整(f,h);
        else if constexpr(std::is_same_v<T,特征概念事实>)return 新特征概念事实完整(f,h);
        else {
            if (!有效(f.概念.值)||!有效(f.本体根.值)||f.概念==f.本体根||!有效(f.规则.值)||
                !有效(f.定义记录)||!有效(f.生命周期值事实)||!lifeComplete(f.生命周期,h)||f.直接上位组.empty()||
                (f.治理状态!=概念树生命周期状态::活跃&&f.治理状态!=概念树生命周期状态::冷却&&f.治理状态!=概念树生命周期状态::退役))return false;
            for(std::size_t i=0;i<f.直接上位组.size();++i){const auto &e=f.直接上位组[i];
                if(!有效(e.关系)||!有效(e.上位.值)||e.下位!=f.概念||e.上位==e.下位||!lifeComplete(e.生命周期,h))return false;
                for(std::size_t j=0;j<i;++j)if(f.直接上位组[j].关系==e.关系||f.直接上位组[j].上位==e.上位)return false;
            }
            {
                if(f.定义.特征模板组.empty()||(f.世界来源组.empty()==f.抽象依据组.empty()))return false;
                for(std::size_t i=0;i<f.定义.特征模板组.size();++i)
                    if(!有效(f.定义.特征模板组[i].值)||(i&&!身份小于(f.定义.特征模板组[i-1],f.定义.特征模板组[i])))return false;
                for(const auto &v:f.世界来源组){
                    if(!有效(v.记录)||!有效(v.成员关系)||!有效(v.世界关系)||!有效(v.截止值事实)||
                        v.来源.世界事实.valueless_by_exception()||!有效(世界编码(v.来源.世界事实))||
                        !v.来源.证据截止||v.来源.证据截止>=f.生命周期.创建事实代次||
                        !lifeComplete(v.记录生命周期,h)||v.记录生命周期.创建事实代次!=f.生命周期.创建事实代次)return false;
                }
                for(const auto &v:f.抽象依据组){
                    if(v.所属EC!=f.概念||!有效(v.来源EC.值)||v.来源EC==f.概念||!有效(v.记录)||!有效(v.成员关系)||
                        !有效(v.引用关系)||!有效(v.截止值事实)||!v.证据H||v.证据H>=f.生命周期.创建事实代次||
                        !lifeComplete(v.记录生命周期,h)||!lifeComplete(v.成员生命周期,h)||
                        !lifeComplete(v.引用生命周期,h,true)||v.记录生命周期.创建事实代次!=f.生命周期.创建事实代次||
                        v.成员生命周期.创建事实代次!=f.生命周期.创建事实代次||v.引用生命周期.创建事实代次!=f.生命周期.创建事实代次||
                        v.当前引用已释放!=(v.引用生命周期.退出事实代次&&*v.引用生命周期.退出事实代次<=h))return false;
                }

            }
            return true;
        }
    },node);
}

} // namespace 海中鱼巣
