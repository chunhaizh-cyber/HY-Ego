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
    已创建共享定义 = 26, 已绑定共享名称, 已退出共享名称, 已记录共享用途, 不支持, 差异不可表示
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
struct 概念树共享特征定义 final {
    概念树特征类型引用 FT;
    特征类型域内容 类型;
    bool 完整域 = false;
    std::vector<特征类型化原子域> 原子组;
    friend bool operator==(const 概念树共享特征定义&, const 概念树共享特征定义&) noexcept;
};
struct 概念树共享预算 final {
    概念树预算 基础;
    特征长期预算 特征;
    std::uint64_t 最大原子数 = 0;
    std::uint64_t 最大用途数 = 0;
    std::uint64_t 最大名称数 = 0;
    std::uint64_t 最大首次材料项数 = 0;
    friend bool operator==(const 概念树共享预算&, const 概念树共享预算&) = default;
};
struct 概念树共享原子事实 final {
    稳定编码 节点{}, 定义成员{};
    特征类型化原子域 原子;
    std::vector<L1所有者范围值事实> 属性;
    概念树生命周期 生命周期;
    friend bool operator==(const 概念树共享原子事实&, const 概念树共享原子事实&) = default;
};
struct 概念树共享引用事实 final {
    稳定编码 关系{}, 记录{}, 目标{};
    概念树概念身份 概念;
    std::uint32_t 角色 = 0;
    概念树生命周期 生命周期;
    friend bool operator==(const 概念树共享引用事实&, const 概念树共享引用事实&) = default;
};
struct 概念树共享来源事实 final {
    稳定编码 记录{}, 归属关系{};
    std::uint32_t 种类 = 0;
    std::uint64_t H = 0;
    std::vector<概念树共享引用事实> 引用;
    std::vector<L1所有者范围关系事实> 原子贡献;
    概念树生命周期 生命周期;
    friend bool operator==(const 概念树共享来源事实&, const 概念树共享来源事实&) = default;
};
struct 概念树共享定义事实 final {
    概念树概念身份 概念;
    概念树概念身份 本体根;
    概念树规则身份 规则;
    稳定编码 定义记录{}, 生命周期值事实{};
    概念树共享特征定义 共享定义;
    概念树生命周期状态 治理状态 = 概念树生命周期状态::活跃;
    概念树生命周期 生命周期;
    std::vector<概念树直接上位事实> 直接上位组;
    特征类型域身份 类型域身份;
    std::vector<概念树共享原子事实> 原子事实组;
    std::vector<概念树共享来源事实> 来源事实组;
    friend bool operator==(const 概念树共享定义事实&, const 概念树共享定义事实&) = default;
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
using 概念树应用定义事实 = std::variant<概念树概念事实, 概念树共享定义事实, 概念树应用存在事实>;
using 概念树应用模板事实 = std::variant<概念树动态模板事实, 概念树共享定义事实>;
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
    std::uint32_t 版本 = 1;
    概念树读取头 读取头;
    概念树概念身份 概念;
    概念树共享预算 预算;
};
struct 概念树应用图读取请求 final {
    std::uint32_t 版本 = 1;
    概念树图读取请求 图请求;
    概念树共享预算 预算;
};
struct 概念树应用模板读取请求 final {
    std::uint32_t 版本 = 1;
    概念树读取头 读取头;
    概念树概念身份 模板;
    概念树共享预算 预算;
};
struct 概念树应用模板判定请求 final {
    std::uint32_t 版本 = 1;
    概念树读取头 读取头;
    概念树概念身份 模板;
    特征原子实际值见证 实际;
    概念树共享预算 预算;
};
struct 概念树应用模板判定事实 final {
    特征长期只读用量 特征用量;
    概念树应用模板事实 模板;
    std::vector<特征原子域判定事实> 原子回执;
    bool 适用 = false;
};
struct 概念树共享定义查找请求 final {
    std::uint32_t 版本 = 1;
    概念树读取头 读取头;
    概念树共享特征定义 定义;
    概念树共享预算 预算;
};
struct 概念树共享域组请求 final {
    std::uint32_t 版本 = 1;
    概念树读取头 读取头;
    概念树特征类型引用 FT;
    概念树共享预算 预算;
};
struct 概念树类型完整域依据 final {
    特征类型域身份 类型域;
    friend bool operator==(const 概念树类型完整域依据&, const 概念树类型完整域依据&) = default;
};
struct 概念树区间依据 final {
    std::vector<特征区间身份> 区间组;
    std::vector<特征跟踪身份> 跟踪组;
    friend bool operator==(const 概念树区间依据&, const 概念树区间依据&) = default;
};
struct 概念树旧概念依据 final {
    概念树概念身份 概念;
    friend bool operator==(const 概念树旧概念依据&, const 概念树旧概念依据&) = default;
};
struct 概念树并集依据 final {
    std::vector<概念树概念身份> 概念组;
    friend bool operator==(const 概念树并集依据&, const 概念树并集依据&) = default;
};
using 概念树共享来源选择 = std::variant<概念树类型完整域依据, 概念树区间依据, 概念树旧概念依据, 概念树并集依据>;
struct 概念树共享实例见证 final {
    稳定编码 E{}, F{}, 值事实{}, 成员关系{};
    std::uint64_t H = 0, 观察标识 = 0;
    std::int64_t 时间 = 0;
    friend bool operator==(const 概念树共享实例见证&, const 概念树共享实例见证&) = default;
};
struct 概念树共享名称事实 final {
    稳定编码 关系{};
    概念树特征引用 F;
    概念树概念身份 概念;
    概念树生命周期 生命周期;
    friend bool operator==(const 概念树共享名称事实&, const 概念树共享名称事实&) = default;
};
struct 概念树共享用途事实 final {
    稳定编码 记录{}, 登记关系{};
    std::uint64_t 业务标识 = 0, 原H = 0;
    概念树概念身份 概念;
    特征跟踪身份 跟踪;
    特征长期观测身份 观察;
    std::uint32_t 用途角色 = 0;
    std::int64_t 时间 = 0;
    std::vector<概念树共享引用事实> 引用;
    概念树生命周期 生命周期;
    friend bool operator==(const 概念树共享用途事实&, const 概念树共享用途事实&) = default;
};
struct 概念树共享建立请求 final {
    std::uint32_t 版本 = 1;
    概念树写入头 写入头;
    概念树共享特征定义 定义;
    概念树共享来源选择 来源;
    std::vector<概念树概念身份> 直接上位;
    概念树共享预算 预算;
    friend bool operator==(const 概念树共享建立请求&, const 概念树共享建立请求&) = default;
};
struct 概念树共享名称请求 final {
    std::uint32_t 版本 = 1;
    概念树写入头 写入头;
    概念树特征引用 F;
    std::optional<概念树共享名称事实> 预期;
    std::optional<概念树概念身份> 新概念;
    std::optional<概念树共享实例见证> 见证;
    概念树共享预算 预算;
    friend bool operator==(const 概念树共享名称请求&, const 概念树共享名称请求&) = default;
};
struct 概念树共享名称读取请求 final {
    std::uint32_t 版本 = 1;
    概念树读取头 读取头;
    std::variant<概念树特征引用, 概念树概念身份> 选择;
    bool 包含已退出 = false;
    概念树共享预算 预算;
};
struct 概念树共享使用请求 final {
    std::uint32_t 版本 = 1;
    概念树写入头 写入头;
    std::uint64_t 业务标识 = 0;
    概念树概念身份 概念;
    特征跟踪身份 跟踪;
    特征长期观测身份 观察;
    std::uint64_t 原H = 0;
    std::int64_t 时间 = 0;
    std::uint32_t 用途角色 = 0;
    概念树共享预算 预算;
    friend bool operator==(const 概念树共享使用请求&, const 概念树共享使用请求&) = default;
};
struct 概念树共享使用读取请求 final {
    std::uint32_t 版本 = 1;
    概念树读取头 读取头;
    概念树概念身份 概念;
    概念树共享预算 预算;
};
struct 概念树共享上位请求 final {
    std::uint32_t 版本 = 1;
    概念树写入头 写入头;
    概念树概念身份 下位;
    std::vector<概念树直接上位事实> 预期父组;
    std::vector<概念树概念身份> 新父组;
    概念树共享预算 预算;
    friend bool operator==(const 概念树共享上位请求&, const 概念树共享上位请求&) = default;
};
struct 概念树共享生命周期请求 final {
    std::uint32_t 版本 = 1;
    概念树生命周期迁移请求 操作;
    std::optional<概念树共享实例见证> 重新采用见证;
    概念树共享预算 预算;
    friend bool operator==(const 概念树共享生命周期请求&, const 概念树共享生命周期请求&) = default;
};
struct 概念树共享子父替代 final {
    概念树概念身份 子节点;
    std::vector<概念树直接上位事实> 预期父组;
    std::vector<概念树概念身份> 新父组;
    friend bool operator==(const 概念树共享子父替代&, const 概念树共享子父替代&) = default;
};
struct 概念树共享退出请求 final {
    std::uint32_t 版本 = 1;
    概念树概念退出请求 操作;
    std::vector<概念树共享子父替代> 完整替代;
    概念树共享预算 预算;
    friend bool operator==(const 概念树共享退出请求&, const 概念树共享退出请求&) = default;
};
struct 概念树共享释放请求 final {
    std::uint32_t 版本 = 1;
    概念树写入头 写入头;
    std::vector<概念树共享引用事实> 完整预期;
    概念树共享预算 预算;
    friend bool operator==(const 概念树共享释放请求&, const 概念树共享释放请求&) = default;
};
using 概念树共享写请求 = std::variant<概念树共享建立请求, 概念树共享名称请求, 概念树共享使用请求,
    概念树共享上位请求, 概念树共享生命周期请求, 概念树共享退出请求, 概念树共享释放请求>;
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
    std::uint32_t 版本 = 1;
    概念树数据状态 状态 = 概念树数据状态::入口拒绝;
    std::uint64_t Gread = 0;
    std::optional<std::uint64_t> 首次H;
    概念树发布状态 发布状态 = 概念树发布状态::未派发;
    std::optional<概念树共享写请求> 原请求;
    std::vector<稳定编码> 已退出编码;
    std::optional<概念树共享定义事实> 概念;
    std::optional<概念树共享名称事实> 名称;
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
    bool 共享应用已启用() const noexcept { return shared_.has_value(); }
    bool 使用同一场景基座(const 场景类数据服务 &s) const noexcept { return s.绑定于(l1_); }
    概念树概念身份 特征根引用() const noexcept { return layout_.根组.特征根; }
    概念树概念身份 存在根引用() const noexcept { return layout_.根组.存在根; }
    概念树应用读取结果<概念树应用定义事实> 读取应用概念定义(const 概念树应用定义读取请求 &) const;
    概念树应用读取结果<概念树应用图事实> 读取应用概念图(const 概念树应用图读取请求 &) const;
    概念树应用读取结果<概念树应用模板事实> 读取应用特征模板(const 概念树应用模板读取请求 &) const;
    概念树应用读取结果<概念树应用模板判定事实> 判定应用特征模板(const 概念树应用模板判定请求 &) const;
    概念树应用读取结果<std::vector<概念树共享定义事实>> 查找共享特征定义(const 概念树共享定义查找请求 &) const;
    概念树应用读取结果<std::vector<概念树共享定义事实>> 读取共享特征域组(const 概念树共享域组请求 &) const;
    概念树应用读取结果<std::vector<概念树共享名称事实>> 读取共享名称字段(const 概念树共享名称读取请求 &) const;
    概念树应用读取结果<std::vector<概念树共享用途事实>> 读取共享概念使用(const 概念树共享使用读取请求 &) const;
    概念树共享写入结果 创建共享特征概念(const 概念树共享建立请求 &r);
    概念树共享写入结果 操作共享名称字段(const 概念树共享名称请求 &r);
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
    static 概念树共享特征定义 规范共享定义(const 概念树共享特征定义 &, std::uint64_t, std::uint64_t);
    static bool 共享包含(const 概念树共享特征定义 &, const 概念树共享特征定义 &);
    static bool 共享原子小于(const 特征类型化原子域 &, const 特征类型化原子域 &);
    static void 共享预算有效(const 概念树共享预算 &);
    static S 共享映射(特征长期状态);
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
        std::uint64_t 原子数 = 0, 来源数 = 0, 像素数 = 0, 关系数 = 0;
        std::uint64_t 首次材料数 = 0;
        std::uint64_t 特征记录扫描数 = 0;
        特征长期只读用量 判定用量;
        std::uint64_t 动态槽数 = 0, 动态模板数 = 0;
        动态历史用量 动态用量;
        std::map<std::pair<std::uint64_t, std::uint64_t>, std::vector<特征区间事实>> 区间缓存;
    };
    struct 共享展开 {
        概念树共享定义事实 事实;
        std::vector<稳定编码> 自有事实;
    };
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
    const std::vector<特征区间事实> &共享区间组(特征跟踪身份, std::uint64_t, 共享读取上下文 &) const;
    共享展开 展开共享定义(概念树概念身份, 共享读取上下文 &) const;
    概念树应用定义事实 应用定义(概念树概念身份, 共享读取上下文 &) const;
    概念树应用图事实 应用图(概念树根角色, bool, 共享读取上下文 &) const;
    概念树应用模板事实 应用模板(概念树概念身份, 共享读取上下文 &) const;
    概念树应用模板判定事实 应用判定(概念树概念身份, const 特征原子实际值见证 &, 共享读取上下文 &) const;
    static void 扣特征用量(特征长期预算 &, 特征长期只读用量 &, const 特征长期只读用量 &);
    std::vector<概念树共享名称事实> 共享名称组(共享读取上下文 &) const;
    void 共享名称交叉核验(稳定编码, bool, const std::vector<概念树共享名称事实> &, 共享读取上下文 &) const;
    概念树共享用途事实 共享用途(稳定编码, 共享读取上下文 &) const;
    std::vector<概念树共享用途事实> 共享用途组(共享读取上下文 &) const;
    void 扣首次材料(共享读取上下文 &c, std::uint64_t n) const {
        if (c.首次材料数 > c.预算.最大首次材料项数 || n > c.预算.最大首次材料项数 - c.首次材料数)
            throw 失败{S::数量预算不足};
        c.首次材料数 += n;
    }
    std::map<std::uint32_t, 稳定编码> 共享核验首次(const L1所有者范围写集请求 &,
        const L1所有者范围写入结果 &, 共享读取上下文 &) const;
    void 核验共享见证(概念树概念身份, const 概念树共享实例见证 &, 共享读取上下文 &, bool 允许退役 = false) const;
    struct 共享准备结果 {
        共享写集构造 构造;
        std::optional<概念树共享定义事实> 复用概念;
        std::optional<概念树共享名称事实> 复用名称;
        std::optional<概念树共享用途事实> 复用用途;
    };
    共享准备结果 准备共享名称(const 概念树共享名称请求 &, 共享读取上下文 &) const;
    共享准备结果 准备共享创建(const 概念树共享建立请求 &, 共享读取上下文 &) const;
    共享准备结果 准备共享用途(const 概念树共享使用请求 &, 共享读取上下文 &) const;
    共享准备结果 准备共享写入(const 概念树共享写请求 &, 共享读取上下文 &) const;
    共享准备结果 准备共享治理(const 概念树共享写请求 &, 共享读取上下文 &) const;
    概念树共享写入结果 共享发布读回(const 概念树共享写请求 &, const L1所有者范围写集请求 &,
        const L1所有者范围写入结果 &, 共享读取上下文 &) const;
    template<class T, class F> 概念树应用读取结果<T> 共享读取封装(
        const 概念树读取头 &head, const 概念树共享预算 &budget, F &&action) const {
        概念树应用读取结果<T> out;
        out.Gread = head.Gread;
        out.H = head.H ? head.H : head.Gread;
        try {
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
    static bool 应用事实完整(const 概念树应用定义事实 &, std::uint64_t) noexcept;
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
                const auto r = features_.按实例读取特征历史事实({1, g, h, x.值, b.最大特征属性数});
                if (!r.成功())
                    throw 失败{映射(r.状态)};
                if (r.Gread != g || r.H != h || r.特征->结点 != x.值)
                    throw 失败{S::内部不一致};
            } else if constexpr (std::is_same_v<T, 概念树特征类型引用>) {
                const auto r = features_.读取特征类型历史事实({1, g, h, x.值});
                if (!r.成功())
                    throw 失败{映射(r.状态)};
                if (r.Gread != g || r.H != h || r.特征类型 != x.值)
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
            const auto ft = features_.读取特征类型历史事实({1, g, n.创建事实代次, d.特征类型.值});
            if (!ft.成功())
                throw 失败{映射(ft.状态)};
            using R = L1所有者范围值表示种类;
            const auto *exact = std::get_if<概念树精确值>(&d.值域);
            const auto rep = exact && exact->index() == 1   ? R::I64组
                             : exact && exact->index() == 2 ? R::U64组
                                                            : R::I64;
            if (ft.Gread != g || ft.H != n.创建事实代次 || ft.特征类型 != d.特征类型.值 || ft.表示 != rep)
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
        if (kind == 2 && shared_ && !rel(sourceNode.编码, shared_->类型[15], false,
            g, n.创建事实代次, 1).empty()) throw 失败{S::不支持};
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
                topology.概念 = f.概念; topology.本体根 = f.本体根; topology.根角色 = role;
                topology.规则 = f.规则; topology.生命周期 = f.生命周期; topology.治理状态 = f.治理状态;
                topology.定义记录 = f.定义记录; topology.生命周期值事实 = f.生命周期值事实;
                topology.直接上位组 = f.直接上位组;
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
                using L = 概念树生命周期状态;
                if (!((r.预期状态 == L::活跃 && r.目标状态 == L::冷却) ||
                      (r.预期状态 == L::冷却 && (r.目标状态 == L::活跃 || r.目标状态 == L::退役)) ||
                      (r.预期状态 == L::退役 && r.目标状态 == L::活跃)))
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

void 概念树类数据服务::共享预算有效(const 概念树共享预算 &b) {
    预算有效(b.基础);
    if (!b.最大原子数 || !b.最大用途数 || !b.最大名称数 || !b.最大首次材料项数)
        throw 失败{S::入口拒绝};
}

概念树数据状态 概念树类数据服务::共享映射(特征长期状态 s) {
    switch (s) {
    case 特征长期状态::未找到: return S::未找到;
    case 特征长期状态::预算不足: return S::数量预算不足;
    case 特征长期状态::历史材料不可用: return S::历史材料不可用;
    case 特征长期状态::事实代次漂移: return S::事实代次漂移;
    case 特征长期状态::幂等冲突: return S::幂等冲突;
    case 特征长期状态::引用冲突: return S::引用冲突;
    case 特征长期状态::资源失败: return S::资源失败;
    case 特征长期状态::内部不一致: return S::内部不一致;
    case 特征长期状态::差异不可表示: return S::差异不可表示;
    case 特征长期状态::类型不支持: case 特征长期状态::未启用: return S::不支持;
    default: return S::入口拒绝;
    }
}

bool 概念树类数据服务::共享原子小于(const 特征类型化原子域 &a, const 特征类型化原子域 &b) {
    if (a.域.index() != b.域.index()) return a.域.index() < b.域.index();
    if (const auto *x = std::get_if<特征I64闭区间>(&a.域)) {
        const auto &y = std::get<特征I64闭区间>(b.域);
        return x->下界 != y.下界 ? x->下界 < y.下界 : x->上界 < y.上界;
    }
    const auto &x = std::get<特征二值半径域>(a.域);
    const auto &y = std::get<特征二值半径域>(b.域);
    // 调用前已核验同一个完整类型合同，图格式必然相同。
    return x.代表.像素 != y.代表.像素 ? x.代表.像素 < y.代表.像素 : x.半径 < y.半径;
}

概念树共享特征定义 概念树类数据服务::规范共享定义(const 概念树共享特征定义 &r,
                                                        std::uint64_t maxAtoms, std::uint64_t maxPixels) {
    const auto &t = r.类型;
    if (!有效(r.FT.值) || t.FT != r.FT.值 || t.版本 != 1 || !有效(t.单位) || t.缩放 <= 0 ||
        t.完整域.valueless_by_exception() || (r.完整域 && !r.原子组.empty()) ||
        (!r.完整域 && r.原子组.empty()))
        throw 失败{S::入口拒绝};
    检查数量(r.原子组.size(), maxAtoms);
    const bool scalar = t.算法 == 特征长期算法::I64幅度;
    if ((scalar && !std::holds_alternative<特征I64闭区间>(t.完整域)) ||
        (!scalar && (t.算法 != 特征长期算法::二值边界平均 ||
                     !std::holds_alternative<特征二值图格式>(t.完整域))))
        throw 失败{S::入口拒绝};
    if (scalar) {
        const auto &full = std::get<特征I64闭区间>(t.完整域);
        if (full.下界 > full.上界 || t.缩放 != 1 || 有效(t.坐标系)) throw 失败{S::入口拒绝};
    } else {
        const auto &fmt = std::get<特征二值图格式>(t.完整域);
        if (!fmt.宽 || !fmt.高 || fmt.宽 > UINT64_MAX / fmt.高 || !有效(fmt.坐标系) ||
            fmt.坐标系 != t.坐标系 || fmt.轴约定 != 1 || !fmt.分辨率分子 || !fmt.分辨率分母 ||
            std::gcd(fmt.分辨率分子, fmt.分辨率分母) != 1)
            throw 失败{S::入口拒绝};
    }
    std::uint64_t pixels = 0;
    if (!scalar) for (const auto &a : r.原子组) {
        const auto *image = std::get_if<特征二值半径域>(&a.域);
        if (!image) throw 失败{S::入口拒绝};
        if (pixels > maxPixels || image->代表.像素.size() > maxPixels - pixels)
            throw 失败{S::数量预算不足};
        pixels += image->代表.像素.size();
    }
    auto out = r;
    for (const auto &a : out.原子组) {
        if (a.类型 != t || a.域.valueless_by_exception() || a.域.index() != (scalar ? 0 : 1))
            throw 失败{S::入口拒绝};
        if (scalar) {
            const auto &x = std::get<特征I64闭区间>(a.域);
            const auto &full = std::get<特征I64闭区间>(t.完整域);
            if (x.下界 > x.上界 || x.下界 < full.下界 || x.上界 > full.上界)
                throw 失败{S::入口拒绝};
        } else {
            const auto &x = std::get<特征二值半径域>(a.域);
            const auto &fmt = std::get<特征二值图格式>(t.完整域);
            if (x.半径 < 0 || x.代表.格式 != fmt || x.代表.像素.size() != fmt.宽 * fmt.高 ||
                std::any_of(x.代表.像素.begin(), x.代表.像素.end(), [](auto v) { return v > 1; }) ||
                std::none_of(x.代表.像素.begin(), x.代表.像素.end(), [](auto v) { return v == 1; }))
                throw 失败{S::入口拒绝};
        }
    }
    std::sort(out.原子组.begin(), out.原子组.end(), 共享原子小于);
    if (!scalar) {
        out.原子组.erase(std::unique(out.原子组.begin(), out.原子组.end()), out.原子组.end());
        return out;
    }
    std::vector<特征类型化原子域> merged;
    for (const auto &a : out.原子组) {
        const auto &x = std::get<特征I64闭区间>(a.域);
        if (!merged.empty()) {
            auto &last = std::get<特征I64闭区间>(merged.back().域);
            if (x.下界 <= last.上界 ||
                (last.上界 != INT64_MAX && x.下界 == last.上界 + 1)) {
                last.上界 = std::max(last.上界, x.上界);
                continue;
            }
        }
        merged.push_back(a);
    }
    out.原子组 = std::move(merged);
    if (out.原子组.size() == 1 &&
        std::get<特征I64闭区间>(out.原子组.front().域) == std::get<特征I64闭区间>(t.完整域)) {
        out.完整域 = true;
        out.原子组.clear();
    }
    return out;
}

bool 概念树类数据服务::共享包含(const 概念树共享特征定义 &outer,
                                const 概念树共享特征定义 &inner) {
    if (outer.FT != inner.FT || outer.类型 != inner.类型) return false;
    if (outer.完整域) return true;
    if (inner.完整域) return false;
    if (outer.类型.算法 == 特征长期算法::二值边界平均)
        return std::includes(outer.原子组.begin(), outer.原子组.end(),
                             inner.原子组.begin(), inner.原子组.end(), 共享原子小于);
    std::size_t i = 0;
    for (const auto &a : inner.原子组) {
        const auto &x = std::get<特征I64闭区间>(a.域);
        while (i < outer.原子组.size() && std::get<特征I64闭区间>(outer.原子组[i].域).上界 < x.下界) ++i;
        if (i == outer.原子组.size()) return false;
        const auto &y = std::get<特征I64闭区间>(outer.原子组[i].域);
        if (y.下界 > x.下界 || y.上界 < x.上界) return false;
    }
    return true;
}

std::vector<概念树类数据服务::E> 概念树类数据服务::共享关系(
    稳定编码 id, 稳定编码 type, bool incoming, std::uint64_t h, 共享读取上下文 &c) const {
    if (c.关系数 > c.预算.基础.最大关系数) throw 失败{S::数量预算不足};
    auto out = 关系(id, type, incoming, c.G, h, c.预算.基础.最大关系数 - c.关系数);
    c.关系数 += out.size();
    return out;
}

概念树类数据服务::共享展开 概念树类数据服务::展开共享定义(
    概念树概念身份 id, 共享读取上下文 &c) const {
    if (!shared_) throw 失败{S::不支持};
    const auto &t = layout_.类型;
    const auto &x = shared_->类型;
    const auto n = 节点(id.值, c.G, c.H);
    if (n.种类 != 节点种类::普通 || n.属性类型表示) throw 失败{S::引用冲突};
    const auto av = 属性(id.值, c.G, c.H);
    if (av.size() != 2) throw 失败{S::内部不一致};
    const auto &kv = 唯一属性(av, t.概念种类);
    const auto &lv = 唯一属性(av, t.生命周期);
    const auto state = std::get<std::int64_t>(lv.材料);
    if (std::get<std::int64_t>(kv.材料) != 4) throw 失败{S::引用冲突};
    if (state < 1 || state > 3 || kv.创建事实代次 != n.创建事实代次 ||
        kv.退出事实代次 != n.退出事实代次) throw 失败{S::内部不一致};
    共享展开 out;
    auto &f = out.事实;
    f.概念 = id;
    f.本体根 = layout_.根组.特征根;
    f.规则 = layout_.签名规则;
    f.治理状态 = static_cast<概念树生命周期状态>(state);
    f.生命周期 = {n.创建事实代次, n.退出事实代次};
    f.生命周期值事实 = lv.编码;
    out.自有事实 = {id.值, kv.编码, lv.编码};
    auto paired = [&](const auto &item) {
        if (item.创建事实代次 != n.创建事实代次 || item.退出事实代次 != n.退出事实代次)
            throw 失败{S::内部不一致};
    };
    auto one = [&](稳定编码 source, 稳定编码 type, std::uint64_t h) {
        auto edges = 共享关系(source, type, false, h, c);
        if (edges.size() != 1 || edges.front().角色或顺序 != 1) throw 失败{S::内部不一致};
        return edges.front();
    };
    const auto root = one(id.值, t.概念根归属, c.H);
    const auto rule = one(id.值, t.概念规则, c.H);
    const auto def = one(id.值, t.定义成员, c.H);
    for (const auto &e : {root, rule, def}) { paired(e); out.自有事实.push_back(e.编码); }
    if (root.目标节点 != f.本体根.值 || rule.目标节点 != f.规则.值)
        throw 失败{S::内部不一致};
    const auto dn = 节点(def.目标节点, c.G, c.H);
    paired(dn);
    if (dn.种类 != 节点种类::普通 || dn.属性类型表示) throw 失败{S::内部不一致};
    f.定义记录 = dn.编码;
    out.自有事实.push_back(dn.编码);
    const auto dv = 属性(dn.编码, c.G, c.H);
    if (dv.size() != 1) throw 失败{S::内部不一致};
    const auto &formValue = 唯一属性(dv, x[1]);
    paired(formValue);
    out.自有事实.push_back(formValue.编码);
    const auto form = std::get<std::int64_t>(formValue.材料);
    if (form < 1 || form > 3) throw 失败{S::内部不一致};
    const auto type = one(dn.编码, x[0], c.H);
    const auto ft = one(dn.编码, t.定义特征类型, c.H);
    paired(type); paired(ft);
    out.自有事实.push_back(type.编码); out.自有事实.push_back(ft.编码);
    if (!共享关系(dn.编码, t.定义宿主, false, c.H, c).empty() ||
        !共享关系(dn.编码, t.定义模板, false, c.H, c).empty())
        throw 失败{S::内部不一致};
    const auto tr = features_.读取特征类型域({{1, c.G, c.H}, 特征类型域身份{type.目标节点}, c.预算.特征});
    if (!tr.成功()) throw 失败{共享映射(tr.状态)};
    if (tr.Gread != c.G || tr.H != c.H || tr.数据->身份.值 != type.目标节点 ||
        tr.数据->内容.FT != ft.目标节点 || tr.数据->创建G >= n.创建事实代次)
        throw 失败{S::内部不一致};
    f.类型域身份 = tr.数据->身份;
    f.共享定义 = {概念树特征类型引用{ft.目标节点}, tr.数据->内容, form == 1, {}};
    if ((form == 2 && f.共享定义.类型.算法 != 特征长期算法::I64幅度) ||
        (form == 3 && f.共享定义.类型.算法 != 特征长期算法::二值边界平均))
        throw 失败{S::内部不一致};
    const auto atoms = 共享关系(dn.编码, x[2], false, c.H, c);
    if ((form == 1) != atoms.empty() || c.原子数 > c.预算.最大原子数 ||
        atoms.size() > c.预算.最大原子数 - c.原子数)
        throw 失败{atoms.size() > c.预算.最大原子数 - std::min(c.原子数, c.预算.最大原子数)
            ? S::数量预算不足 : S::内部不一致};
    c.原子数 += atoms.size();
    std::set<std::uint64_t> atomIds;
    for (const auto &e : atoms) {
        paired(e);
        if (e.角色或顺序 != f.原子事实组.size() + 1 || !atomIds.insert(e.目标节点.值).second)
            throw 失败{S::内部不一致};
        const auto an = 节点(e.目标节点, c.G, c.H);
        paired(an);
        if (an.种类 != 节点种类::普通 || an.属性类型表示) throw 失败{S::内部不一致};
        auto values = 属性(an.编码, c.G, c.H);
        for (const auto &v : values) { paired(v); out.自有事实.push_back(v.编码); }
        特征类型化原子域 a;
        a.类型 = f.共享定义.类型;
        if (form == 2) {
            if (values.size() != 1) throw 失败{S::内部不一致};
            const auto &range = std::get<std::vector<std::int64_t>>(唯一属性(values, x[5]).材料);
            if (range.size() != 2) throw 失败{S::内部不一致};
            a.域 = 特征I64闭区间{range[0], range[1]};
        } else {
            if (values.size() != 2) throw 失败{S::内部不一致};
            const auto &bits = std::get<std::vector<std::uint64_t>>(唯一属性(values, x[3]).材料);
            if (bits.size() < 10 || bits[0] != 0x42494E4152590001ULL)
                throw 失败{S::内部不一致};
            if (c.像素数 > c.预算.特征.最大像素数 || bits.size() - 9 > c.预算.特征.最大像素数 - c.像素数)
                throw 失败{S::数量预算不足};
            c.像素数 += bits.size() - 9;
            特征二值图 image;
            image.格式 = {bits[1], bits[2], 稳定编码{bits[3]}, bits[4], bits[5], bits[6],
                          std::bit_cast<std::int64_t>(bits[7]), std::bit_cast<std::int64_t>(bits[8])};
            image.像素.assign(bits.begin() + 9, bits.end());
            a.域 = 特征二值半径域{std::move(image), std::get<std::int64_t>(唯一属性(values, x[4]).材料)};
        }
        f.共享定义.原子组.push_back(a);
        f.原子事实组.push_back({an.编码, e.编码, std::move(a), std::move(values), {an.创建事实代次, an.退出事实代次}});
        out.自有事实.push_back(an.编码); out.自有事实.push_back(e.编码);
    }
    try {
        if (规范共享定义(f.共享定义, c.预算.最大原子数, c.预算.特征.最大像素数) != f.共享定义)
            throw 失败{S::内部不一致};
    } catch (const 失败 &e) {
        if (e.状态 == S::入口拒绝) throw 失败{S::内部不一致};
        throw;
    }
    const auto sources = 共享关系(id.值, t.来源成员, false, c.H, c);
    if (sources.empty()) throw 失败{S::内部不一致};
    if (c.来源数 > c.预算.基础.最大来源数 || sources.size() > c.预算.基础.最大来源数 - c.来源数)
        throw 失败{S::数量预算不足};
    c.来源数 += sources.size();
    std::vector<概念树共享特征定义> sourceDomains;
    for (const auto &member : sources) {
        paired(member);
        if (member.角色或顺序 != f.来源事实组.size() + 1) throw 失败{S::内部不一致};
        const auto sn = 节点(member.目标节点, c.G, c.H);
        paired(sn);
        const auto sv = 属性(sn.编码, c.G, c.H);
        if (sn.种类 != 节点种类::普通 || sn.属性类型表示 || sv.size() != 2)
            throw 失败{S::内部不一致};
        const auto &hv = 唯一属性(sv, t.证据截止);
        const auto &sk = 唯一属性(sv, x[18]);
        paired(hv); paired(sk);
        const auto &times = std::get<std::vector<std::uint64_t>>(hv.材料);
        const auto k = std::get<std::int64_t>(sk.材料);
        if (times.size() != 1 || !times[0] || times[0] >= sn.创建事实代次 || k < 1 || k > 4)
            throw 失败{S::内部不一致};
        概念树共享来源事实 source;
        source.记录 = sn.编码; source.归属关系 = member.编码;
        source.种类 = static_cast<std::uint32_t>(k); source.H = times[0];
        source.生命周期 = {sn.创建事实代次, sn.退出事实代次};
        for (const auto role : {7U, 21U, 17U, 18U}) {
            const auto edges = 共享关系(sn.编码, x[role - 1], false, n.创建事实代次, c);
            const bool required = (k == 1 && (role == 7 || role == 21)) ||
                                  (k == 2 && role == 17) || (k == 3 && role == 18);
            if (edges.size() != (required ? 1U : 0U)) throw 失败{S::内部不一致};
            for (const auto &edge : edges) {
                if (edge.角色或顺序 != 1 || edge.创建事实代次 != n.创建事实代次)
                    throw 失败{S::内部不一致};
                source.引用.push_back({edge.编码, sn.编码, edge.目标节点, id, role,
                                      {edge.创建事实代次, edge.退出事实代次}});
                if (有效于(edge, c.H)) out.自有事实.push_back(edge.编码);
            }
        }
        source.原子贡献 = 共享关系(sn.编码, x[19], false, c.H, c);
        std::set<std::uint64_t> targets;
        for (const auto &edge : source.原子贡献) {
            paired(edge);
            if (edge.角色或顺序 != targets.size() + 1 || !atomIds.contains(edge.目标节点.值) ||
                !targets.insert(edge.目标节点.值).second)
                throw 失败{S::内部不一致};
            out.自有事实.push_back(edge.编码);
        }
        if ((k == 4 && !targets.empty()) || (k == 2 && targets != atomIds))
            throw 失败{S::内部不一致};
        if (k == 4) {
            if (!f.共享定义.完整域 || sources.size() != 1) throw 失败{S::内部不一致};
            sourceDomains.push_back(f.共享定义);
        } else {
            概念树共享特征定义 domain;
            if (k == 1) {
                const auto &interval = source.引用[0];
                const auto &track = source.引用[1];
                if (interval.角色 != 7 || track.角色 != 21 ||
                    interval.生命周期 != track.生命周期)
                    throw 失败{S::内部不一致};
                const auto &group = 共享区间组(特征跟踪身份{track.目标}, source.H, c);
                const 特征区间事实 *found = nullptr;
                for (const auto &item : group) {
                    if (item.跟踪.值 != track.目标) throw 失败{S::内部不一致};
                    if (item.身份.值 == interval.目标) {
                        if (found) throw 失败{S::内部不一致};
                        found = &item;
                    }
                }
                if (!found || found->创建G > source.H ||
                    found->原子域.类型 != f.共享定义.类型)
                    throw 失败{S::内部不一致};
                domain = {f.共享定义.FT, f.共享定义.类型, false, {found->原子域}};
                domain = 规范共享定义(domain, c.预算.最大原子数, c.预算.特征.最大像素数);
            } else if (k == 2) {
                const auto target = 概念树概念身份{source.引用.front().目标};
                const auto targetNode = 节点(target.值, c.G, source.H);
                if (targetNode.创建事实代次 >= n.创建事实代次) throw 失败{S::内部不一致};
                const auto old = 展开概念(target, c.G, source.H, c.预算.基础, false, true).事实;
                if (!old.定义 || !std::holds_alternative<概念树特征定义>(*old.定义))
                    throw 失败{S::内部不一致};
                const auto &d = std::get<概念树特征定义>(*old.定义);
                if (d.特征类型 != f.共享定义.FT || f.共享定义.类型.算法 != 特征长期算法::I64幅度)
                    throw 失败{S::内部不一致};
                特征I64闭区间 interval;
                if (const auto *range = std::get_if<概念树I64区间>(&d.值域)) interval = {range->下界, range->上界};
                else {
                    const auto *value = std::get_if<std::int64_t>(&std::get<概念树精确值>(d.值域));
                    if (!value) throw 失败{S::不支持};
                    interval = {*value, *value};
                }
                domain = {f.共享定义.FT, f.共享定义.类型, false, {{f.共享定义.类型, interval}}};
                domain = 规范共享定义(domain, c.预算.最大原子数, c.预算.特征.最大像素数);
            } else {
                const auto target = 概念树概念身份{source.引用.front().目标};
                const auto targetNode = 节点(target.值, c.G, source.H);
                if (targetNode.创建事实代次 >= n.创建事实代次) throw 失败{S::内部不一致};
                const auto h = c.H;
                c.H = source.H;
                try { domain = 展开共享定义(target, c).事实.共享定义; }
                catch (...) { c.H = h; throw; }
                c.H = h;
            }
            if (domain.FT != f.共享定义.FT || domain.类型 != f.共享定义.类型)
                throw 失败{S::内部不一致};
            std::set<std::uint64_t> expectedTargets;
            for (const auto &atom : f.原子事实组) {
                bool contributes = domain.完整域;
                for (const auto &input : domain.原子组) {
                    if (const auto *a = std::get_if<特征I64闭区间>(&atom.原子.域)) {
                        const auto &b = std::get<特征I64闭区间>(input.域);
                        contributes = contributes || (a->下界 <= b.上界 && b.下界 <= a->上界);
                    } else contributes = contributes || atom.原子 == input;
                }
                if (contributes) expectedTargets.insert(atom.节点.值);
            }
            if (targets != expectedTargets) throw 失败{S::内部不一致};
            sourceDomains.push_back(std::move(domain));
        }
        f.来源事实组.push_back(std::move(source));
        for (auto code : {sn.编码, member.编码, hv.编码, sk.编码}) out.自有事实.push_back(code);
    }
    {
        概念树共享特征定义 combined{f.共享定义.FT, f.共享定义.类型, false, {}};
        for (const auto &domain : sourceDomains) {
            if (domain.完整域) { combined.完整域 = true; combined.原子组.clear(); break; }
            if (domain.原子组.size() > c.预算.最大原子数 - combined.原子组.size())
                throw 失败{S::数量预算不足};
            combined.原子组.insert(combined.原子组.end(), domain.原子组.begin(), domain.原子组.end());
        }
        if (规范共享定义(combined, c.预算.最大原子数, c.预算.特征.最大像素数) != f.共享定义)
            throw 失败{S::内部不一致};
    }
    const auto parents = 共享关系(id.值, t.直接上位, true, c.H, c);
    if (parents.empty()) throw 失败{S::内部不一致};
    std::set<std::uint64_t> parentIds;
    for (const auto &edge : parents) {
        if (edge.角色或顺序 != 1 || edge.源节点 == id.值 || !parentIds.insert(edge.源节点.值).second)
            throw 失败{S::内部不一致};
        const auto parent = 概念树概念身份{edge.源节点};
        (void)节点(parent.值, c.G, c.H);
        if (parent != f.本体根 && one(parent.值, t.概念根归属, c.H).目标节点 != f.本体根.值)
            throw 失败{S::内部不一致};
        f.直接上位组.push_back({edge.编码, parent, id, {edge.创建事实代次, edge.退出事实代次}});
    }
    std::sort(f.直接上位组.begin(), f.直接上位组.end(), [](const auto &a, const auto &b) {
        return a.上位.值.值 < b.上位.值.值;
    });
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
        if (r.版本 != 1) throw 失败{S::入口拒绝};
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
            out.直接边组.insert(out.直接边组.end(), f.直接上位组.begin(), f.直接上位组.end());
        }, fact);
        检查数量(out.直接边组.size(), c.预算.基础.最大关系数);
        out.节点组.push_back(std::move(fact));
    }
    std::map<std::uint64_t, std::size_t> degrees;
    std::map<std::uint64_t, std::vector<std::uint64_t>> children;
    std::set<std::pair<std::uint64_t, std::uint64_t>> endpoints;
    std::set<std::uint64_t> edgeIds;
    for (const auto &f : out.节点组) {
        const auto id = std::visit([](const auto &v) { return v.概念.值.值; }, f);
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
        return std::visit([](const auto &x) { return x.概念.值.值; }, a) <
               std::visit([](const auto &x) { return x.概念.值.值; }, b);
    });
    std::sort(out.直接边组.begin(), out.直接边组.end(), [](const auto &a, const auto &b) {
        if (a.上位 != b.上位) return a.上位.值.值 < b.上位.值.值;
        if (a.下位 != b.下位) return a.下位.值.值 < b.下位.值.值;
        return a.关系.值 < b.关系.值;
    });
    if (support) {
        out.支持组.emplace();
        for (const auto &f : out.节点组) {
            const auto id = std::visit([](const auto &x) { return x.概念; }, f);
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
        if (r.版本 != 1) throw 失败{S::入口拒绝};
        return 应用图(r.图请求.根角色, r.图请求.包含支持, c);
    });
}

概念树应用模板事实 概念树类数据服务::应用模板(概念树概念身份 id, 共享读取上下文 &c) const {
    auto definition = 应用定义(id, c);
    if (auto *shared = std::get_if<概念树共享定义事实>(&definition)) return std::move(*shared);
    const auto &old = std::get<概念树概念事实>(definition);
    if (old.是本体根 || !old.定义 || !std::holds_alternative<概念树特征定义>(*old.定义))
        throw 失败{S::引用冲突};
    return 动态模板(id, c.G, c.H, c.预算.基础);
}

概念树应用读取结果<概念树应用模板事实> 概念树类数据服务::读取应用特征模板(
    const 概念树应用模板读取请求 &r) const {
    return 共享读取封装<概念树应用模板事实>(r.读取头, r.预算, [&](auto &c) {
        if (r.版本 != 1) throw 失败{S::入口拒绝};
        return 应用模板(r.模板, c);
    });
}

概念树应用读取结果<std::vector<概念树共享定义事实>> 概念树类数据服务::读取共享特征域组(
    const 概念树共享域组请求 &r) const {
    return 共享读取封装<std::vector<概念树共享定义事实>>(r.读取头, r.预算, [&](auto &c) {
        if (r.版本 != 1 || !有效(r.FT.值) || !shared_) throw 失败{S::入口拒绝};
        auto graph = 应用图(概念树根角色::特征, false, c);
        std::vector<概念树共享定义事实> out;
        for (auto &node : graph.节点组)
            if (auto *s = std::get_if<概念树共享定义事实>(&node); s && s->共享定义.FT == r.FT)
                out.push_back(std::move(*s));
        return out;
    });
}

概念树应用读取结果<std::vector<概念树共享定义事实>> 概念树类数据服务::查找共享特征定义(
    const 概念树共享定义查找请求 &r) const {
    return 共享读取封装<std::vector<概念树共享定义事实>>(r.读取头, r.预算, [&](auto &c) {
        if (r.版本 != 1 || !shared_) throw 失败{S::入口拒绝};
        const auto definition = 规范共享定义(r.定义, c.预算.最大原子数, c.预算.特征.最大像素数);
        auto graph = 应用图(概念树根角色::特征, false, c);
        std::vector<概念树共享定义事实> out;
        for (auto &node : graph.节点组)
            if (auto *s = std::get_if<概念树共享定义事实>(&node); s && s->共享定义 == definition)
                out.push_back(std::move(*s));
        if (out.size() > 1) throw 失败{S::内部不一致};
        return out;
    });
}

void 概念树类数据服务::扣特征用量(特征长期预算 &remaining, 特征长期只读用量 &sum,
                                  const 特征长期只读用量 &u) {
    auto debit = [&](std::uint64_t &limit, std::uint64_t &used, std::uint64_t amount) {
        if (amount > limit || amount > UINT64_MAX - used) throw 失败{S::内部不一致};
        limit -= amount; used += amount;
    };
    debit(remaining.最大记录数, sum.记录数, u.记录数);
    debit(remaining.最大关系数, sum.关系数, u.关系数);
    debit(remaining.最大属性数, sum.属性数, u.属性数);
    debit(remaining.最大样本数, sum.样本数, u.样本数);
    debit(remaining.最大像素数, sum.像素数, u.像素数);
    debit(remaining.最大边界点对数, sum.点对数, u.点对数);
    debit(remaining.最大历史事实数, sum.历史数, u.历史数);
}

概念树应用模板判定事实 概念树类数据服务::应用判定(
    概念树概念身份 id, const 特征原子实际值见证 &actual, 共享读取上下文 &c) const {
    if ((!actual.H || actual.H > c.H) || !有效(actual.F) || !有效(actual.值事实) || !有效(actual.FT))
        throw 失败{S::入口拒绝};
    概念树应用模板判定事实 out;
    out.模板 = 应用模板(id, c);
    auto remaining = c.预算.特征;
    特征长期只读用量 已扣;
    扣特征用量(remaining, 已扣, c.判定用量);
    if (const auto *shared = std::get_if<概念树共享定义事实>(&out.模板)) {
        if (shared->共享定义.FT.值 != actual.FT) throw 失败{S::引用冲突};
        auto decide = [&](const 特征判定域选择 &choice) {
            auto decision = features_.判定特征原子域({1, {1, c.G, actual.H},
                shared->类型域身份, choice, actual, remaining});
            if (!decision.成功()) throw 失败{共享映射(decision.状态)};
            if (decision.Gread != c.G || decision.H != actual.H || decision.数据->实际 != actual ||
                decision.数据->规范域 != choice || decision.数据->类型域事实.身份 != shared->类型域身份 ||
                decision.数据->类型域事实.内容 != shared->共享定义.类型)
                throw 失败{S::内部不一致};
            if (std::holds_alternative<特征完整域标记>(choice) &&
                (!decision.数据->适用 || decision.数据->差异))
                throw 失败{S::内部不一致};
            扣特征用量(remaining, out.特征用量, decision.数据->用量);
            out.适用 = out.适用 || decision.数据->适用;
            out.原子回执.push_back(std::move(*decision.数据));
        };
        if (shared->共享定义.完整域) decide(特征完整域标记{});
        else for (const auto &atom : shared->共享定义.原子组) decide(atom);
    } else {
        const auto &old = std::get<概念树动态模板事实>(out.模板);
        const auto whole = features_.按实例读取特征历史事实({1, c.G, actual.H, actual.F, c.预算.基础.最大特征属性数});
        if (!whole.成功()) throw 失败{映射(whole.状态)};
        if (whole.Gread != c.G || whole.H != actual.H || !whole.特征 ||
            whole.特征->值事实 != actual.值事实 || whole.特征->特征类型 != actual.FT ||
            old.定义.特征类型.值 != actual.FT)
            throw 失败{S::引用冲突};
        const auto *direct = std::get_if<特征直接值>(&whole.特征->特征值);
        if (!direct || direct->valueless_by_exception()) throw 失败{S::不支持};
        if (const auto *range = std::get_if<概念树I64区间>(&old.定义.值域)) {
            const auto *v = std::get_if<std::int64_t>(direct);
            out.适用 = v && *v >= range->下界 && *v <= range->上界;
        } else {
            out.适用 = *direct == std::get<概念树精确值>(old.定义.值域);
        }
    }
    auto totalBudget = c.预算.特征;
    扣特征用量(totalBudget, c.判定用量, out.特征用量);
    return out;
}

概念树应用读取结果<概念树应用模板判定事实> 概念树类数据服务::判定应用特征模板(
    const 概念树应用模板判定请求 &r) const {
    return 共享读取封装<概念树应用模板判定事实>(r.读取头, r.预算, [&](auto &c) {
        if (r.版本 != 1) throw 失败{S::入口拒绝};
        return 应用判定(r.模板, r.实际, c);
    });
}

std::vector<概念树共享名称事实> 概念树类数据服务::共享名称组(共享读取上下文 &c) const {
    if (!shared_) throw 失败{S::不支持};
    const auto &x = shared_->类型;
    const auto regs = 共享关系(shared_->锚点, x[8], false, c.H, c);
    检查数量(regs.size(), c.预算.最大名称数);
    std::set<std::uint64_t> keys, ids, names;
    std::vector<概念树共享名称事实> out;
    for (const auto &reg : regs) {
        const auto locator = 节点(reg.目标节点, c.G, c.H);
        const auto values = 属性(locator.编码, c.G, c.H);
        if (reg.角色或顺序 != 1 || reg.退出事实代次 || locator.退出事实代次 ||
            locator.创建事实代次 != reg.创建事实代次 || locator.种类 != 节点种类::普通 ||
            locator.属性类型表示 || values.size() != 2 || !ids.insert(locator.编码.值).second)
            throw 失败{S::内部不一致};
        const auto &format = 唯一属性(values, layout_.类型.格式版本);
        const auto &keyValue = 唯一属性(values, x[9]);
        const auto &key = std::get<std::vector<std::uint64_t>>(keyValue.材料);
        if (std::get<std::int64_t>(format.材料) != 1 || key.size() != 1 || !key[0] ||
            (key[0] >> 48) == 0x4E43 || !keys.insert(key[0]).second ||
            format.创建事实代次 != locator.创建事实代次 || keyValue.创建事实代次 != locator.创建事实代次 ||
            format.退出事实代次 || keyValue.退出事实代次)
            throw 失败{S::内部不一致};
        const auto first = port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, {key[0]}});
        if (first.读取事实代次 != c.G) throw 失败{S::事实代次漂移};
        if (first.状态 == L1所有者范围读取状态::未找到) throw 失败{S::历史材料不可用};
        if (first.状态 != L1所有者范围读取状态::成功) throw 失败{映射(first.状态)};
        if (first.合同版本 != L1所有者范围首次写入读取合同版本 || first.所有者 != port_.所有者身份() ||
            first.写入幂等身份.值 != key[0] || !first.首次规范化写集 || !first.首次写入结果)
            throw 失败{S::内部不一致};
        const auto &ws = *first.首次规范化写集;
        const auto &saved = *first.首次写入结果;
        for (auto n : {ws.节点.size(), ws.关系.size(), ws.值.size(), ws.属性槽变更.size(),
                        ws.退出事实.size(), saved.新编码映射.size()}) 扣首次材料(c, n);
        const auto born = locator.创建事实代次;
        if (saved.合同版本 != L1所有者范围CRUD合同版本 || saved.所有者 != port_.所有者身份() ||
            saved.写入幂等身份.值 != key[0] || saved.状态 != L1所有者范围写入状态::成功 ||
            !saved.是否形成内存权威发布 || saved.事实代次 != born ||
            saved.重试边界 != L1所有者范围重试边界::不适用 ||
            ws.合同版本 != L1所有者范围CRUD合同版本 || ws.写入幂等身份.值 != key[0] ||
            !ws.期望事实代次 || ws.期望事实代次 >= born || ws.节点.size() != 1 ||
            ws.关系.size() != 2 || ws.值.size() != 2 || ws.属性槽变更.size() != 2 ||
            ws.退出事实.size() > 1 || saved.新编码映射.size() != 5)
            throw 失败{S::内部不一致};
        std::map<std::uint32_t, 稳定编码> map;
        std::set<std::uint64_t> mapped;
        for (const auto &[k, v] : saved.新编码映射)
            if (!k.值 || !有效(v) || !map.emplace(k.值, v).second || !mapped.insert(v.值).second)
                throw 失败{S::内部不一致};
        auto resolve = [&](const Ref &r) {
            if (const auto *v = std::get_if<稳定编码>(&r)) return *v;
            const auto at = map.find(std::get<Key>(r).值);
            if (at == map.end()) throw 失败{S::内部不一致};
            return at->second;
        };
        if (map[1] != locator.编码 || map[0x10001] != reg.编码 || map[0x20000] != format.编码 ||
            map[0x20001] != keyValue.编码 || ws.节点[0].本地键.值 != 1 ||
            ws.节点[0].种类 != 节点种类::普通 || ws.节点[0].属性类型表示)
            throw 失败{S::内部不一致};
        const auto &name = ws.关系[0]; const auto &registration = ws.关系[1];
        if (name.本地键.值 != 0x10000 || name.关系类型节点 != Ref{x[7]} || name.角色或顺序 != 1 ||
            !std::holds_alternative<稳定编码>(name.源节点) || !std::holds_alternative<稳定编码>(name.目标节点) ||
            registration.本地键.值 != 0x10001 || registration.源节点 != Ref{shared_->锚点} ||
            registration.目标节点 != Ref{Key{1}} || registration.关系类型节点 != Ref{x[8]} ||
            registration.角色或顺序 != 1)
            throw 失败{S::内部不一致};
        std::optional<E> business;
        for (const auto &e : ws.关系) {
            扣首次材料(c, 1);
            const auto raw = 原始事实(resolve(e.本地键), c.G);
            const auto *actual = std::get_if<E>(&raw);
            if (!actual || !有效于(*actual, born) || actual->创建事实代次 != born ||
                actual->源节点 != resolve(e.源节点) || actual->目标节点 != resolve(e.目标节点) ||
                actual->关系类型节点 != resolve(e.关系类型节点) || actual->角色或顺序 != e.角色或顺序)
                throw 失败{S::内部不一致};
            if (e.本地键.值 == 0x10000) business = *actual;
        }
        for (std::size_t i = 0; i < ws.值.size(); ++i) {
            const auto &v = ws.值[i];
            const auto type = i == 0 ? layout_.类型.格式版本 : x[9];
            const auto &actual = i == 0 ? format : keyValue;
            扣首次材料(c, 1);
            if (v.本地键.值 != 0x20000 + i || v.所属节点 != Ref{Key{1}} || v.来源节点 != Ref{Key{1}} ||
                v.属性类型节点 != Ref{type} || v.材料 != actual.材料 ||
                std::count_if(ws.属性槽变更.begin(), ws.属性槽变更.end(), [&](const auto &slot) {
                    return slot.所属节点 == Ref{Key{1}} && slot.属性类型节点 == Ref{type} && slot.新当前值 == v.本地键;
                }) != 1)
                throw 失败{S::内部不一致};
        }
        for (const auto retired : ws.退出事实) {
            扣首次材料(c, 1);
            const auto raw = 原始事实(retired, c.G);
            const auto *e = std::get_if<E>(&raw);
            if (!e || e->关系类型节点 != x[7] || e->源节点 != business->源节点 ||
                e->退出事实代次 != born || e->创建事实代次 >= born)
                throw 失败{S::内部不一致};
        }
        if (!business || !names.insert(business->编码.值).second) throw 失败{S::内部不一致};
        out.push_back({business->编码, 概念树特征引用{business->源节点}, 概念树概念身份{business->目标节点},
                       {business->创建事实代次, business->退出事实代次}});
    }
    std::sort(out.begin(), out.end(), [](const auto &a, const auto &b) { return a.关系.值 < b.关系.值; });
    return out;
}

概念树应用读取结果<std::vector<概念树共享名称事实>> 概念树类数据服务::读取共享名称字段(
    const 概念树共享名称读取请求 &r) const {
    return 共享读取封装<std::vector<概念树共享名称事实>>(r.读取头, r.预算, [&](共享读取上下文 &c) {
        if (r.版本 != 1 || r.选择.valueless_by_exception() ||
            !std::visit([](const auto &x) { return 有效(x.值); }, r.选择)) throw 失败{S::入口拒绝};
        const auto all = 共享名称组(c);
        std::visit([&](const auto &id) { 共享名称交叉核验(id.值,
            std::is_same_v<std::decay_t<decltype(id)>, 概念树概念身份>, all, c); }, r.选择);
        std::vector<概念树共享名称事实> out;
        for (const auto &f : all) {
            const bool matches = std::visit([&](const auto &x) {
                if constexpr (std::is_same_v<std::decay_t<decltype(x)>, 概念树特征引用>) return f.F == x;
                else return f.概念 == x;
            }, r.选择);
            if (matches && (r.包含已退出 || !f.生命周期.退出事实代次 || *f.生命周期.退出事实代次 > c.H))
                out.push_back(f);
        }
        if (std::holds_alternative<概念树特征引用>(r.选择) && !r.包含已退出 && out.size() > 1)
            throw 失败{S::内部不一致};
        return out;
    });
}

概念树共享用途事实 概念树类数据服务::共享用途(稳定编码 id, 共享读取上下文 &c) const {
    if (!shared_) throw 失败{S::不支持};
    const auto &x = shared_->类型;
    const auto node = 节点(id, c.G, c.H);
    const auto attrs = 属性(id, c.G, c.H);
    if (node.种类 != 节点种类::普通 || node.属性类型表示 || node.退出事实代次 || attrs.size() != 4)
        throw 失败{S::内部不一致};
    const auto &format = 唯一属性(attrs, layout_.类型.格式版本);
    const auto &time = 唯一属性(attrs, x[14]);
    const auto &location = 唯一属性(attrs, x[13]);
    const auto &original = 唯一属性(attrs, layout_.类型.证据截止);
    for (const auto &v : attrs)
        if (v.创建事实代次 != node.创建事实代次 || v.退出事实代次) throw 失败{S::内部不一致};
    const auto &u = std::get<std::vector<std::uint64_t>>(location.材料);
    const auto &h = std::get<std::vector<std::uint64_t>>(original.材料);
    if (std::get<std::int64_t>(format.材料) != 1 || u.size() != 6 ||
        !u[0] || !u[1] || !u[2] || !u[3] || !u[4] || (u[5] != 1 && u[5] != 2) ||
        u[1] >= node.创建事实代次 || h.size() != 1 || h[0] != u[1] || std::get<std::int64_t>(time.材料) <= 0)
        throw 失败{S::内部不一致};
    const auto reg = 共享关系(id, x[10], true, c.H, c);
    if (reg.size() != 1 || reg[0].源节点 != shared_->锚点 || reg[0].角色或顺序 != 1 ||
        reg[0].创建事实代次 != node.创建事实代次 || reg[0].退出事实代次)
        throw 失败{S::内部不一致};
    概念树共享用途事实 out;
    out.记录 = id; out.登记关系 = reg[0].编码; out.业务标识 = u[0]; out.原H = u[1];
    out.概念 = 概念树概念身份{稳定编码{u[2]}};
    out.跟踪 = 特征跟踪身份{稳定编码{u[3]}};
    out.观察 = 特征长期观测身份{稳定编码{u[4]}};
    out.用途角色 = static_cast<std::uint32_t>(u[5]);
    out.时间 = std::get<std::int64_t>(time.材料);
    out.生命周期 = {node.创建事实代次, node.退出事实代次};
    for (const auto role : {12U, 13U}) {
        const auto edges = 共享关系(id, x[role - 1], false, node.创建事实代次, c);
        const auto target = role == 12 ? out.概念.值 : out.观察.值;
        if (edges.size() != 1 || edges[0].角色或顺序 != 1 || edges[0].目标节点 != target ||
            edges[0].创建事实代次 != node.创建事实代次)
            throw 失败{S::内部不一致};
        out.引用.push_back({edges[0].编码, id, target, out.概念, role,
                           {edges[0].创建事实代次, edges[0].退出事实代次}});
    }
    return out;
}

std::vector<概念树共享用途事实> 概念树类数据服务::共享用途组(共享读取上下文 &c) const {
    if (!shared_) throw 失败{S::不支持};
    const auto regs = 共享关系(shared_->锚点, shared_->类型[10], false, c.H, c);
    检查数量(regs.size(), c.预算.最大用途数);
    std::vector<概念树共享用途事实> out;
    std::set<std::uint64_t> records;
    std::set<std::array<std::uint64_t, 3>> business, observations;
    for (const auto &reg : regs) {
        if (!records.insert(reg.目标节点.值).second) throw 失败{S::内部不一致};
        auto f = 共享用途(reg.目标节点, c);
        if (f.登记关系 != reg.编码 ||
            !business.insert({f.业务标识, f.概念.值.值, f.用途角色}).second ||
            !observations.insert({f.观察.值.值, f.概念.值.值, f.用途角色}).second)
            throw 失败{S::内部不一致};
        out.push_back(std::move(f));
    }
    std::sort(out.begin(), out.end(), [](const auto &a, const auto &b) { return a.记录.值 < b.记录.值; });
    return out;
}

概念树应用读取结果<std::vector<概念树共享用途事实>> 概念树类数据服务::读取共享概念使用(
    const 概念树共享使用读取请求 &r) const {
    return 共享读取封装<std::vector<概念树共享用途事实>>(r.读取头, r.预算, [&](auto &c) {
        if (r.版本 != 1 || !有效(r.概念.值)) throw 失败{S::入口拒绝};
        const auto raw = 原始事实(r.概念.值, c.G);
        const auto *n = std::get_if<N>(&raw);
        if (!n || n->创建事实代次 > c.H) throw 失败{S::未找到};
        const auto a = 属性(r.概念.值, c.G, n->创建事实代次);
        if (std::get<std::int64_t>(唯一属性(a, layout_.类型.概念种类).材料) != 4)
            throw 失败{S::引用冲突};
        auto all = 共享用途组(c);
        all.erase(std::remove_if(all.begin(), all.end(), [&](const auto &f) { return f.概念 != r.概念; }), all.end());
        return all;
    });
}

概念树写入头 概念树类数据服务::共享请求头(const 概念树共享写请求 &request) {
    return std::visit([](const auto &r) {
        using T = std::decay_t<decltype(r)>;
        if constexpr (std::is_same_v<T, 概念树共享生命周期请求> ||
                      std::is_same_v<T, 概念树共享退出请求>) return r.操作.写入头;
        else return r.写入头;
    }, request);
}

概念树共享写请求 概念树类数据服务::规范共享请求(const 概念树共享写请求 &input) const {
    if (!shared_ || input.valueless_by_exception()) throw 失败{S::入口拒绝};
    const auto head = 共享请求头(input);
    if (head.合同版本 != 1 || !head.期望事实代次 || !head.幂等身份) throw 失败{S::入口拒绝};
    auto out = input;
    std::visit([&](auto &r) {
        using T = std::decay_t<decltype(r)>;
        if (r.版本 != 1) throw 失败{S::入口拒绝};
        共享预算有效(r.预算);
        auto ids = [&](auto &v) {
            if (v.empty()) throw 失败{S::入口拒绝};
            检查数量(v.size(), r.预算.基础.最大关系数);
            std::sort(v.begin(), v.end(), [](const auto &a, const auto &b) { return a.值.值 < b.值.值; });
            for (std::size_t i = 0; i < v.size(); ++i)
                if (!有效(v[i].值) || (i && v[i] == v[i - 1])) throw 失败{S::入口拒绝};
        };
        if constexpr (std::is_same_v<T, 概念树共享建立请求>) {
            r.定义 = 规范共享定义(r.定义, r.预算.最大原子数, r.预算.特征.最大像素数);
            ids(r.直接上位);
            if (r.来源.valueless_by_exception()) throw 失败{S::入口拒绝};
            std::visit([&](auto &s) {
                using W = std::decay_t<decltype(s)>;
                if constexpr (std::is_same_v<W, 概念树类型完整域依据>) {
                    if (!有效(s.类型域.值) || !r.定义.完整域) throw 失败{S::入口拒绝};
                } else if constexpr (std::is_same_v<W, 概念树旧概念依据>) {
                    if (!有效(s.概念.值)) throw 失败{S::入口拒绝};
                } else if constexpr (std::is_same_v<W, 概念树并集依据>) {
                    ids(s.概念组);
                    检查数量(s.概念组.size(), r.预算.基础.最大来源数);
                } else {
                    if (s.区间组.empty() || s.区间组.size() != s.跟踪组.size()) throw 失败{S::入口拒绝};
                    检查数量(s.区间组.size(), r.预算.基础.最大来源数);
                    std::vector<std::pair<特征区间身份, 特征跟踪身份>> pairs;
                    for (std::size_t i = 0; i < s.区间组.size(); ++i) {
                        if (!有效(s.区间组[i].值) || !有效(s.跟踪组[i].值)) throw 失败{S::入口拒绝};
                        pairs.emplace_back(s.区间组[i], s.跟踪组[i]);
                    }
                    std::sort(pairs.begin(), pairs.end(), [](const auto &a, const auto &b) { return a.first.值.值 < b.first.值.值; });
                    for (std::size_t i = 0; i < pairs.size(); ++i) {
                        if (i && pairs[i].first == pairs[i - 1].first) throw 失败{S::入口拒绝};
                        s.区间组[i] = pairs[i].first; s.跟踪组[i] = pairs[i].second;
                    }
                }
            }, r.来源);
        } else if constexpr (std::is_same_v<T, 概念树共享名称请求>) {
            if (!有效(r.F.值) || (head.幂等身份 >> 48) == 0x4E43 || (!r.预期 && !r.新概念) ||
                (r.预期 && (r.预期->F != r.F || !有效(r.预期->关系) || !有效(r.预期->概念.值))) ||
                (r.新概念 && (!有效(r.新概念->值) || !r.见证)) || (!r.新概念 && r.见证))
                throw 失败{S::入口拒绝};
            if (r.见证) {
                const auto &w = *r.见证;
                if (!有效(w.E) || w.F != r.F.值 || !有效(w.值事实) || !有效(w.成员关系) ||
                    w.H != head.期望事实代次 || !w.观察标识 || w.时间 <= 0) throw 失败{S::入口拒绝};
            }
        } else if constexpr (std::is_same_v<T, 概念树共享使用请求>) {
            if (!r.业务标识 || !有效(r.概念.值) || !有效(r.跟踪.值) || !有效(r.观察.值) ||
                !r.原H || r.原H > head.期望事实代次 || r.时间 <= 0 || (r.用途角色 != 1 && r.用途角色 != 2))
                throw 失败{S::入口拒绝};
        } else if constexpr (std::is_same_v<T, 概念树共享释放请求>) {
            检查数量(r.完整预期.size(), r.预算.基础.最大关系数);
            std::sort(r.完整预期.begin(), r.完整预期.end(), [](const auto &a, const auto &b) { return a.关系.值 < b.关系.值; });
            for (std::size_t i = 0; i < r.完整预期.size(); ++i) {
                const auto &f = r.完整预期[i];
                if (!有效(f.关系) || !有效(f.记录) || !有效(f.目标) || !有效(f.概念.值) ||
                    (f.角色 != 7 && f.角色 != 12 && f.角色 != 13 && f.角色 != 17 && f.角色 != 18 && f.角色 != 21) ||
                    !生命完整(f.生命周期, head.期望事实代次) || (i && f.关系 == r.完整预期[i - 1].关系))
                    throw 失败{S::入口拒绝};
            }
        } else if constexpr (std::is_same_v<T, 概念树共享上位请求>) {
            if (!有效(r.下位.值) || r.预期父组.empty()) throw 失败{S::入口拒绝};
            检查数量(r.预期父组.size(), r.预算.基础.最大关系数);
            检查数量(r.新父组.size(), r.预算.基础.最大关系数);
            ids(r.新父组);
            std::sort(r.预期父组.begin(), r.预期父组.end(), [](const auto&a,const auto&b){return 身份小于(a.上位,b.上位);});
            for (std::size_t i=0;i<r.预期父组.size();++i) {
                const auto &e=r.预期父组[i];const auto &life=e.生命周期;
                if (!有效(e.关系)||!有效(e.上位.值)||e.下位!=r.下位||e.上位==r.下位||
                    !life.创建事实代次||life.创建事实代次>head.期望事实代次||
                    (life.退出事实代次&&*life.退出事实代次<=head.期望事实代次)||
                    (i&&e.上位==r.预期父组[i-1].上位)) throw 失败{S::入口拒绝};
                for(std::size_t j=0;j<i;++j)if(e.关系==r.预期父组[j].关系)throw 失败{S::入口拒绝};
            }
            if(std::find(r.新父组.begin(),r.新父组.end(),r.下位)!=r.新父组.end())throw 失败{S::入口拒绝};
        } else {
            r.操作.预算 = r.预算.基础;
            using O = std::decay_t<decltype(r.操作)>;
            r.操作 = std::get<O>(规范写请求(r.操作));
            if constexpr (std::is_same_v<T, 概念树共享生命周期请求>) {
                if (r.操作.预期状态 == 概念树生命周期状态::退役 &&
                    (r.操作.目标状态 != 概念树生命周期状态::活跃 || !r.重新采用见证)) throw 失败{S::入口拒绝};
            } else if constexpr (std::is_same_v<T, 概念树共享退出请求>) {
                检查数量(r.完整替代.size(), r.预算.基础.最大概念数);
                std::sort(r.完整替代.begin(), r.完整替代.end(), [](const auto &a, const auto &b) { return a.子节点.值.值 < b.子节点.值.值; });
                for (std::size_t i = 0; i < r.完整替代.size(); ++i) {
                    auto &s = r.完整替代[i];
                    if (!有效(s.子节点.值) || (i && s.子节点 == r.完整替代[i - 1].子节点)) throw 失败{S::入口拒绝};
                    ids(s.新父组);
                    std::sort(s.预期父组.begin(), s.预期父组.end(), [](const auto &a, const auto &b) { return a.上位.值.值 < b.上位.值.值; });
                    for (const auto &e : s.预期父组)
                        if (e.下位 != s.子节点 || !有效(e.关系) || !有效(e.上位.值) || !生命完整(e.生命周期, head.期望事实代次))
                            throw 失败{S::入口拒绝};
                }
            }
        }
    }, out);
    return out;
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

void 概念树类数据服务::核验共享见证(概念树概念身份 id, const 概念树共享实例见证 &w,
                                    共享读取上下文 &c, bool 允许退役) const {
    if (!w.H || w.H > c.H || !有效(w.E) || !有效(w.F) || !有效(w.值事实) ||
        !有效(w.成员关系) || !w.观察标识 || w.时间 <= 0) throw 失败{S::入口拒绝};
    const auto exists = existences_.读取存在历史事实({1, c.G, w.H, w.E, c.预算.基础.最大世界成员数});
    if (!exists.成功()) throw 失败{映射(exists.状态)};
    if (exists.Gread != c.G || exists.H != w.H || !exists.存在 || exists.存在->结点 != w.E ||
        std::count_if(exists.存在->特征组.begin(), exists.存在->特征组.end(), [&](const auto &m) {
            return m.成员关系 == w.成员关系 && m.目标结点 == w.F;
        }) != 1) throw 失败{S::引用冲突};
    const auto member = existences_.读取特征成员历史事实({1, c.G, w.H, w.E, w.成员关系, w.F});
    if (!member.成功()) throw 失败{映射(member.状态)};
    if (member.Gread != c.G || member.H != w.H || member.宿主 != w.E ||
        member.成员->成员关系 != w.成员关系 || member.成员->目标结点 != w.F) throw 失败{S::内部不一致};
    const auto definition = 展开共享定义(id, c).事实;
    if (!允许退役 && definition.治理状态 == 概念树生命周期状态::退役) throw 失败{S::引用冲突};
    const auto match = 应用判定(id, {w.F, w.值事实, definition.共享定义.FT.值, w.H}, c);
    if (!match.适用) throw 失败{S::引用冲突};
}

概念树类数据服务::共享准备结果 概念树类数据服务::准备共享名称(
    const 概念树共享名称请求 &r, 共享读取上下文 &c) const {
    auto names = 共享名称组(c);
    共享名称交叉核验(r.F.值, false, names, c);
    std::optional<概念树共享名称事实> old;
    for (const auto &f : names)
        if (f.F == r.F && (!f.生命周期.退出事实代次 || *f.生命周期.退出事实代次 > c.H)) {
            if (old) throw 失败{S::内部不一致};
            old = f;
        }
    auto expected = r.预期;
    auto projected = old;
    auto project = [&](auto &value) {
        if (value && value->生命周期.退出事实代次 && *value->生命周期.退出事实代次 > c.H)
            value->生命周期.退出事实代次.reset();
    };
    project(expected); project(projected);
    if (expected != projected) throw 失败{S::引用冲突};
    if (r.新概念) 核验共享见证(*r.新概念, *r.见证, c);
    共享准备结果 out;
    auto &builder = out.构造;
    builder.写集.期望事实代次 = r.写入头.期望事实代次;
    builder.写集.写入幂等身份 = {r.写入头.幂等身份};
    if (old && r.新概念 && old->概念 == *r.新概念) { out.复用名称 = *old; return out; }
    if (old) builder.写集.退出事实.push_back(old->关系);
    if (r.新概念) {
        const auto node = builder.节点();
        builder.关系(Ref{r.F.值}, Ref{r.新概念->值}, shared_->类型[7]);
        builder.关系(Ref{shared_->锚点}, Ref{node}, shared_->类型[8]);
        builder.值(Ref{node}, layout_.类型.格式版本, std::int64_t{1});
        builder.值(Ref{node}, shared_->类型[9], std::vector<std::uint64_t>{r.写入头.幂等身份});
    }
    builder.规范();
    return out;
}

概念树类数据服务::共享准备结果 概念树类数据服务::准备共享用途(
    const 概念树共享使用请求 &r, 共享读取上下文 &c) const {
    const auto observations = features_.读取长期观察({{1, c.G, c.H}, r.跟踪, false, c.预算.特征});
    if (!observations.成功()) throw 失败{共享映射(observations.状态)};
    if (observations.Gread != c.G || observations.H != c.H) throw 失败{S::事实代次漂移};
    if (c.特征记录扫描数 > c.预算.特征.最大记录数 ||
        observations.数据->size() > c.预算.特征.最大记录数 - c.特征记录扫描数)
        throw 失败{S::数量预算不足};
    c.特征记录扫描数 += observations.数据->size();
    const 特征长期观测事实 *observation = nullptr;
    for (const auto &o : *observations.数据) if (o.身份 == r.观察) {
        if (observation) throw 失败{S::内部不一致};
        observation = &o;
    }
    if (!observation || observation->跟踪 != r.跟踪 || observation->创建G > c.H ||
        observation->见证.H != r.原H || observation->见证.时间纳秒 != r.时间)
        throw 失败{S::引用冲突};
    const auto &w = observation->见证;
    核验共享见证(r.概念, {w.存在上下文, w.F, w.值事实, w.成员关系, w.H, w.观察标识, w.时间纳秒}, c);
    if (r.用途角色 == 2) {
        const auto names = 共享名称组(c);
        共享名称交叉核验(w.F, false, names, c);
        if (std::count_if(names.begin(), names.end(), [&](const auto &name) {
            return name.F.值 == w.F && name.概念 == r.概念 &&
                   (!name.生命周期.退出事实代次 || *name.生命周期.退出事实代次 > c.H);
        }) != 1) throw 失败{S::引用冲突};
    }
    共享准备结果 out;
    auto &builder = out.构造;
    builder.写集.期望事实代次 = r.写入头.期望事实代次;
    builder.写集.写入幂等身份 = {r.写入头.幂等身份};
    const auto existing = 共享用途组(c);
    for (const auto &use : existing) {
        if (use.概念 != r.概念 || use.用途角色 != r.用途角色 ||
            (use.业务标识 != r.业务标识 && use.观察 != r.观察)) continue;
        if (use.观察 != r.观察 || use.跟踪 != r.跟踪 ||
            use.原H != r.原H || use.时间 != r.时间) throw 失败{S::幂等冲突};
        out.复用用途 = use;
    }
    if (out.复用用途) return out;
    if (w.H != c.H)
        核验共享见证(r.概念, {w.存在上下文, w.F, w.值事实, w.成员关系,
            c.H, w.观察标识, w.时间纳秒}, c);
    检查数量(existing.size() + 1, c.预算.最大用途数);
    const auto node = builder.节点();
    builder.关系(Ref{shared_->锚点}, Ref{node}, shared_->类型[10]);
    builder.关系(Ref{node}, Ref{r.概念.值}, shared_->类型[11]);
    builder.关系(Ref{node}, Ref{r.观察.值}, shared_->类型[12]);
    builder.值(Ref{node}, layout_.类型.格式版本, std::int64_t{1});
    builder.值(Ref{node}, layout_.类型.证据截止, std::vector<std::uint64_t>{r.原H});
    builder.值(Ref{node}, shared_->类型[13], std::vector<std::uint64_t>{r.业务标识, r.原H,
        r.概念.值.值, r.跟踪.值.值, r.观察.值.值, r.用途角色});
    builder.值(Ref{node}, shared_->类型[14], r.时间);
    builder.规范();
    return out;
}

概念树共享写入结果 概念树类数据服务::共享执行(const 概念树共享写请求 &input) {
    概念树共享写入结果 out;
    bool 首次未确定 = false, 已派发 = false;
    try {
        out.原请求 = input;
        if(input.valueless_by_exception())throw 失败{S::入口拒绝};
        const auto head=共享请求头(input);
        if(head.合同版本!=1||!head.期望事实代次||!head.幂等身份)throw 失败{S::入口拒绝};
        std::scoped_lock lock(mutex_);
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
        auto request=规范共享请求(input);out.原请求=request;
        const auto budget=std::visit([](const auto&r){return r.预算;},request);
        if (first.状态 == L1所有者范围读取状态::成功) {
            if (!first.首次规范化写集 || !first.首次写入结果 || !out.首次H)
                throw 失败{S::内部不一致};
            if (first.首次规范化写集->期望事实代次 != head.期望事实代次) {
                首次未确定 = false;
                throw 失败{S::幂等冲突};
            }
            共享读取上下文 before{out.Gread, head.期望事实代次, budget};
            共享准备结果 prepared;
            try { prepared = 准备共享写入(request, before); }
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
            out = 共享发布读回(request, prepared.构造.写集, *first.首次写入结果, after);
            out.状态 = S::精确重复;
            守卫代次(after.G);
            if (!out.成功()) throw 失败{S::内部不一致};
            return out;
        }
        if (first.首次规范化写集 || first.首次写入结果) throw 失败{S::内部不一致};
        首次未确定 = false;
        if (out.Gread != head.期望事实代次) throw 失败{S::事实代次漂移};
        共享读取上下文 c{out.Gread, out.Gread, budget};
        auto prepared = 准备共享写入(request, c);
        auto &ws = prepared.构造.写集;
        if (ws.节点.empty() && ws.关系.empty() && ws.值.empty() && ws.退出事实.empty() && ws.属性槽变更.empty()) {
            out.状态 = S::无须变更;
            out.概念 = std::move(prepared.复用概念);
            out.名称 = std::move(prepared.复用名称);
            out.用途 = std::move(prepared.复用用途);
            if(std::holds_alternative<概念树共享上位请求>(request)&&out.概念)out.直接边=out.概念->直接上位组;
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
        auto &after = c; after.区间缓存.clear(); after.动态用量.已核验.clear(); after.G = g; after.H = saved.事实代次;
        out = 共享发布读回(request, ws, saved, after);
        if (saved.状态 == L1所有者范围写入状态::精确重复) out.状态 = S::精确重复;
        守卫代次(g);
        if (!out.成功()) throw 失败{S::内部不一致};
        return out;
    } catch (const 失败 &x) { out.状态 = x.状态; }
    catch (const std::bad_alloc &) { out.状态 = S::资源失败; }
    catch (const std::length_error &) { out.状态 = S::资源失败; }
    catch (...) { out.状态 = S::内部不一致; }
    out.概念.reset(); out.名称.reset(); out.用途.reset(); out.直接边.clear(); out.已退出编码.clear();
    if (首次未确定 || 已派发) {
        out.状态 = S::已可能发布;
        out.发布状态 = P::可能已发布;
    } else out.发布状态 = out.首次H ? P::可能已发布 : P::未派发;
    return out;
}

概念树类数据服务::共享准备结果 概念树类数据服务::准备共享创建(
    const 概念树共享建立请求 &r, 共享读取上下文 &c) const {
    const auto &t = layout_.类型;
    const auto &x = shared_->类型;
    const auto type = features_.读取特征类型域({{1, c.G, c.H},
        特征长期类型引用{r.定义.FT.值}, c.预算.特征});
    if (!type.成功()) throw 失败{共享映射(type.状态)};
    if (type.Gread != c.G || type.H != c.H || type.数据->内容 != r.定义.类型)
        throw 失败{S::引用冲突};
    struct Source {
        std::uint32_t 种类 = 0;
        稳定编码 目标{}, 跟踪{};
        概念树共享特征定义 域;
    };
    std::vector<Source> sources;
    std::visit([&](const auto &source) {
        using T = std::decay_t<decltype(source)>;
        if constexpr (std::is_same_v<T, 概念树类型完整域依据>) {
            if (source.类型域 != type.数据->身份 || !r.定义.完整域) throw 失败{S::引用冲突};
            sources.push_back({4, {}, {}, r.定义});
        } else if constexpr (std::is_same_v<T, 概念树区间依据>) {
            for (std::size_t i = 0; i < source.区间组.size(); ++i) {
                const auto &group = 共享区间组(source.跟踪组[i], c.H, c);
                const 特征区间事实 *found = nullptr;
                for (const auto &item : group) {
                    if (item.跟踪 != source.跟踪组[i]) throw 失败{S::内部不一致};
                    if (item.身份 == source.区间组[i]) {
                        if (found) throw 失败{S::内部不一致};
                        found = &item;
                    }
                }
                if (!found || found->原子域.类型 != r.定义.类型) throw 失败{S::引用冲突};
                概念树共享特征定义 d{r.定义.FT, r.定义.类型, false, {found->原子域}};
                d = 规范共享定义(d, c.预算.最大原子数, c.预算.特征.最大像素数);
                sources.push_back({1, found->身份.值, source.跟踪组[i].值, std::move(d)});
            }
        } else if constexpr (std::is_same_v<T, 概念树旧概念依据>) {
            const auto old = 展开概念(source.概念, c.G, c.H, c.预算.基础, false, true).事实;
            if (!old.定义 || !std::holds_alternative<概念树特征定义>(*old.定义)) throw 失败{S::引用冲突};
            const auto &d = std::get<概念树特征定义>(*old.定义);
            if (d.特征类型 != r.定义.FT || r.定义.类型.算法 != 特征长期算法::I64幅度)
                throw 失败{S::不支持};
            特征I64闭区间 range;
            if (const auto *a = std::get_if<概念树I64区间>(&d.值域)) range = {a->下界, a->上界};
            else {
                const auto *v = std::get_if<std::int64_t>(&std::get<概念树精确值>(d.值域));
                if (!v) throw 失败{S::不支持};
                range = {*v, *v};
            }
            概念树共享特征定义 domain{r.定义.FT, r.定义.类型, false, {{r.定义.类型, range}}};
            domain = 规范共享定义(domain, c.预算.最大原子数, c.预算.特征.最大像素数);
            sources.push_back({2, source.概念.值, {}, std::move(domain)});
        } else {
            for (auto id : source.概念组) {
                auto f = 展开共享定义(id, c).事实;
                if (f.共享定义.FT != r.定义.FT || f.共享定义.类型 != r.定义.类型)
                    throw 失败{S::引用冲突};
                sources.push_back({3, id.值, {}, std::move(f.共享定义)});
            }
        }
    }, r.来源);
    概念树共享特征定义 combined{r.定义.FT, r.定义.类型, false, {}};
    for (const auto &source : sources) {
        if (source.域.完整域) { combined.完整域 = true; combined.原子组.clear(); break; }
        if (source.域.原子组.size() > c.预算.最大原子数 - combined.原子组.size()) throw 失败{S::数量预算不足};
        combined.原子组.insert(combined.原子组.end(), source.域.原子组.begin(), source.域.原子组.end());
    }
    if (规范共享定义(combined, c.预算.最大原子数, c.预算.特征.最大像素数) != r.定义)
        throw 失败{S::引用冲突};
    auto graph = 应用图(概念树根角色::特征, false, c);
    共享准备结果 out;
    for (const auto &node : graph.节点组) {
        if (const auto *f = std::get_if<概念树共享定义事实>(&node); f && f->共享定义 == r.定义) {
            if (out.复用概念) throw 失败{S::内部不一致};
            out.复用概念 = *f;
        }
    }
    if (out.复用概念) return out;
    if (graph.节点组.size() >= c.预算.基础.最大概念数 ||
        r.直接上位.size() > c.预算.基础.最大关系数 - graph.直接边组.size() ||
        c.来源数 > c.预算.基础.最大来源数 || sources.size() > c.预算.基础.最大来源数 - c.来源数 ||
        c.原子数 > c.预算.最大原子数 || r.定义.原子组.size() > c.预算.最大原子数 - c.原子数)
        throw 失败{S::数量预算不足};
    for (auto parent : r.直接上位) {
        if (parent == layout_.根组.特征根) {
            if (!r.定义.完整域 || r.直接上位.size() != 1) throw 失败{S::引用冲突};
        } else {
            const auto p = 展开共享定义(parent, c).事实;
            if (r.定义.完整域 || p.治理状态 == 概念树生命周期状态::退役 ||
                !共享包含(p.共享定义, r.定义) || p.共享定义 == r.定义)
                throw 失败{S::引用冲突};
        }
    }
    auto &b = out.构造;
    const auto node = b.节点(), def = b.节点();
    std::vector<Key> atoms, records;
    for (const auto &a : r.定义.原子组) atoms.push_back(b.节点());
    for (const auto &source : sources) records.push_back(b.节点());
    b.关系(Ref{node}, Ref{layout_.根组.特征根.值}, t.概念根归属);
    b.关系(Ref{node}, Ref{layout_.签名规则.值}, t.概念规则);
    b.关系(Ref{node}, Ref{def}, t.定义成员);
    for (auto parent : r.直接上位) b.关系(Ref{parent.值}, Ref{node}, t.直接上位);
    b.关系(Ref{def}, Ref{r.定义.FT.值}, t.定义特征类型);
    b.关系(Ref{def}, Ref{type.数据->身份.值}, x[0]);
    for (std::size_t i = 0; i < atoms.size(); ++i) b.关系(Ref{def}, Ref{atoms[i]}, x[2], i + 1);
    for (std::size_t i = 0; i < sources.size(); ++i) {
        const auto &source = sources[i];
        b.关系(Ref{node}, Ref{records[i]}, t.来源成员, i + 1);
        if (source.种类 != 4) {
            const auto role = source.种类 == 1 ? 7U : (source.种类 == 2 ? 17U : 18U);
            b.关系(Ref{records[i]}, Ref{source.目标}, x[role - 1]);
            if (source.种类 == 1) b.关系(Ref{records[i]}, Ref{source.跟踪}, x[20]);
        }
        std::int64_t order = 0;
        for (std::size_t j = 0; j < atoms.size(); ++j) {
            bool contributes = source.域.完整域 || source.种类 == 2;
            for (const auto &input : source.域.原子组) {
                if (const auto *a = std::get_if<特征I64闭区间>(&r.定义.原子组[j].域)) {
                    const auto &v = std::get<特征I64闭区间>(input.域);
                    contributes = contributes || (a->下界 <= v.上界 && v.下界 <= a->上界);
                } else contributes = contributes || input == r.定义.原子组[j];
            }
            if (contributes) b.关系(Ref{records[i]}, Ref{atoms[j]}, x[19], ++order);
        }
    }
    b.值(Ref{node}, t.概念种类, std::int64_t{4});
    b.值(Ref{node}, t.生命周期, std::int64_t{1});
    const auto form = r.定义.完整域 ? 1 : (r.定义.类型.算法 == 特征长期算法::I64幅度 ? 2 : 3);
    b.值(Ref{def}, x[1], static_cast<std::int64_t>(form));
    for (std::size_t i = 0; i < atoms.size(); ++i) {
        if (const auto *range = std::get_if<特征I64闭区间>(&r.定义.原子组[i].域)) {
            b.值(Ref{atoms[i]}, x[5], std::vector<std::int64_t>{range->下界, range->上界});
        } else {
            const auto &a = std::get<特征二值半径域>(r.定义.原子组[i].域);
            const auto &fmt = a.代表.格式;
            std::vector<std::uint64_t> bits{0x42494E4152590001ULL, fmt.宽, fmt.高, fmt.坐标系.值,
                fmt.轴约定, fmt.分辨率分子, fmt.分辨率分母, std::bit_cast<std::uint64_t>(fmt.原点X),
                std::bit_cast<std::uint64_t>(fmt.原点Y)};
            bits.insert(bits.end(), a.代表.像素.begin(), a.代表.像素.end());
            b.值(Ref{atoms[i]}, x[3], std::move(bits));
            b.值(Ref{atoms[i]}, x[4], a.半径);
        }
    }
    for (std::size_t i = 0; i < sources.size(); ++i) {
        b.值(Ref{records[i]}, t.证据截止, std::vector<std::uint64_t>{c.H});
        b.值(Ref{records[i]}, x[18], static_cast<std::int64_t>(sources[i].种类));
    }
    b.规范();
    return out;
}
概念树类数据服务::共享准备结果 概念树类数据服务::准备共享治理(
    const 概念树共享写请求 &request, 共享读取上下文 &c) const {
    共享准备结果 prepared;
    auto &b = prepared.构造;
    const auto &t = layout_.类型;
    const auto &x = shared_->类型;
    auto projected = [&](auto v) {
        if (v.生命周期.退出事实代次 && *v.生命周期.退出事实代次 > c.H)
            v.生命周期.退出事实代次.reset();
        return v;
    };
    auto parentValid = [&](const 概念树共享定义事实 &child, 概念树概念身份 id) {
        if (id == layout_.根组.特征根) {
            if (!child.共享定义.完整域) throw 失败{S::引用冲突};
        } else {
            const auto parent = 展开共享定义(id, c).事实;
            if (child.共享定义.完整域 || parent.治理状态 == 概念树生命周期状态::退役 ||
                parent.共享定义 == child.共享定义 || !共享包含(parent.共享定义, child.共享定义))
                throw 失败{S::引用冲突};
        }
    };
    if (const auto *selected = std::get_if<概念树共享释放请求>(&request)) {
        const auto &r = *selected;
        for (const auto &expected : r.完整预期) {
            const auto raw = 原始事实(expected.关系, c.G);
            const auto *edge = std::get_if<E>(&raw);
            if (!edge || !有效于(*edge, c.H) || edge->源节点 != expected.记录 ||
                edge->目标节点 != expected.目标 || edge->关系类型节点 != x[expected.角色 - 1] ||
                edge->角色或顺序 != 1 || projected(expected).生命周期 !=
                    projected(概念树共享引用事实{edge->编码, edge->源节点, edge->目标节点,
                        expected.概念, expected.角色, {edge->创建事实代次, edge->退出事实代次}}).生命周期)
                throw 失败{S::引用冲突};
            bool found = false;
            if (expected.角色 == 12 || expected.角色 == 13) {
                const auto usage = 共享用途(expected.记录, c);
                if (usage.概念 != expected.概念) throw 失败{S::引用冲突};
                for (const auto &ref : usage.引用) if (projected(ref) == projected(expected)) found = true;
            } else {
                const auto f = 展开共享定义(expected.概念, c).事实;
                for (const auto &source : f.来源事实组) {
                    if (source.记录 != expected.记录) continue;
                    for (const auto &ref : source.引用) if (projected(ref) == projected(expected)) found = true;
                    if (source.种类 == 1) {
                        for (const auto &ref : source.引用) {
                            if ((ref.角色 != 7 && ref.角色 != 21) ||
                                std::none_of(r.完整预期.begin(), r.完整预期.end(), [&](const auto &v) {
                                    return projected(v) == projected(ref);
                                })) throw 失败{S::引用冲突};
                        }
                    }
                }
            }
            if (!found) throw 失败{S::引用冲突};
            b.写集.退出事实.push_back(expected.关系);
        }
    } else {
        概念树概念身份 id;
        if(const auto *r=std::get_if<概念树共享上位请求>(&request))id=r->下位;
        else if(const auto *r=std::get_if<概念树共享生命周期请求>(&request))id=r->操作.概念;
        else if(const auto *r=std::get_if<概念树共享退出请求>(&request))id=r->操作.概念;
        else throw 失败{S::入口拒绝};
        const auto roots = 根组();
        if (std::find(roots.begin(), roots.end(), id) != roots.end()) throw 失败{S::根不可退出};
        auto full = 展开共享定义(id, c);
        const auto &f = full.事实;
        if (const auto *selected = std::get_if<概念树共享生命周期请求>(&request)) {
            const auto &r = *selected;
            const auto &op = r.操作;
            if (f.生命周期值事实 != op.当前生命周期值编码 || f.治理状态 != op.预期状态)
                throw 失败{S::引用冲突};
            if (op.目标状态 == op.预期状态) { prepared.复用概念 = f; return prepared; }
            if (op.预期状态 == 概念树生命周期状态::退役) {
                if (op.目标状态 != 概念树生命周期状态::活跃 || !r.重新采用见证)
                    throw 失败{S::入口拒绝};
                // 重新采用的域证明允许目标仍为退役；当前实例资格独立核验。
                auto witness = *r.重新采用见证;
                if (witness.H != c.H) throw 失败{S::入口拒绝};
                核验共享见证(id, witness, c, true);
            }
            b.写集.退出事实.push_back(f.生命周期值事实);
            b.值(Ref{id.值}, t.生命周期, static_cast<std::int64_t>(op.目标状态));
        } else if (const auto *selected = std::get_if<概念树共享上位请求>(&request)) {
            const auto &r = *selected;
            if(f.直接上位组.size()!=r.预期父组.size())throw 失败{S::引用冲突};
            for(std::size_t i=0;i<f.直接上位组.size();++i)
                if(projected(f.直接上位组[i])!=projected(r.预期父组[i]))throw 失败{S::引用冲突};
            for(auto parent:r.新父组) {
                parentValid(f,parent);
                if(std::none_of(f.直接上位组.begin(),f.直接上位组.end(),[&](const auto&e){return e.上位==parent;}))
                    b.关系(Ref{parent.值},Ref{id.值},t.直接上位);
            }
            for(const auto&e:f.直接上位组)
                if(std::find(r.新父组.begin(),r.新父组.end(),e.上位)==r.新父组.end())b.写集.退出事实.push_back(e.关系);
            const auto graph=应用图(概念树根角色::特征,false,c);
            if(b.写集.退出事实.size()>graph.直接边组.size())throw 失败{S::内部不一致};
            const auto remaining=graph.直接边组.size()-b.写集.退出事实.size();
            if(remaining>c.预算.基础.最大关系数||b.写集.关系.size()>c.预算.基础.最大关系数-remaining)throw 失败{S::数量预算不足};
            if(b.写集.关系.empty()&&b.写集.退出事实.empty())prepared.复用概念=f;
        } else {
            const auto &r = std::get<概念树共享退出请求>(request);
            if (f.治理状态 != 概念树生命周期状态::退役 ||
                f.生命周期值事实 != r.操作.当前退役生命周期值编码 || f.共享定义.完整域)
                throw 失败{S::引用冲突};
            const auto children = 共享关系(id.值, t.直接上位, false, c.H, c);
            if (children.size() != r.完整替代.size()) throw 失败{S::引用冲突};
            for (const auto &childEdge : children) {
                const auto child = 展开共享定义(概念树概念身份{childEdge.目标节点}, c).事实;
                auto replacement = std::find_if(r.完整替代.begin(), r.完整替代.end(), [&](const auto &v) {
                    return v.子节点 == child.概念;
                });
                if (replacement == r.完整替代.end() || replacement->预期父组.size() != child.直接上位组.size())
                    throw 失败{S::引用冲突};
                for (std::size_t i = 0; i < child.直接上位组.size(); ++i)
                    if (projected(child.直接上位组[i]) != projected(replacement->预期父组[i]))
                        throw 失败{S::引用冲突};
                for (auto parent : replacement->新父组) {
                    if (parent == id) throw 失败{S::引用冲突};
                    parentValid(child, parent);
                    const auto found = std::find_if(child.直接上位组.begin(), child.直接上位组.end(),
                        [&](const auto &e) { return e.上位 == parent; });
                    if (found == child.直接上位组.end()) b.关系(Ref{parent.值}, Ref{child.概念.值}, t.直接上位);
                }
                for (const auto &edge : child.直接上位组)
                    if (std::find(replacement->新父组.begin(), replacement->新父组.end(), edge.上位) == replacement->新父组.end())
                        b.写集.退出事实.push_back(edge.关系);
            }
            const auto graph = 应用图(概念树根角色::特征, false, c);
            std::uint64_t removedEdges = 0;
            for (const auto &edge : graph.直接边组)
                if (edge.下位 == id || std::find(b.写集.退出事实.begin(), b.写集.退出事实.end(), edge.关系) !=
                                             b.写集.退出事实.end())
                    ++removedEdges;
            const auto keptEdges = graph.直接边组.size() - removedEdges;
            if (keptEdges > c.预算.基础.最大关系数 ||
                b.写集.关系.size() > c.预算.基础.最大关系数 - keptEdges)
                throw 失败{S::数量预算不足};
            for (auto type : {x[7], x[11], x[16], x[17], t.定义模板, t.支持概念})
                if (!共享关系(id.值, type, true, c.H, c).empty()) throw 失败{S::引用冲突};
            if (dynamic_ && !共享关系(id.值, dynamic_->类型.槽模板, true, c.H, c).empty())
                throw 失败{S::引用冲突};
            b.写集.退出事实.insert(b.写集.退出事实.end(), full.自有事实.begin(), full.自有事实.end());
            for (const auto &edge : f.直接上位组) b.写集.退出事实.push_back(edge.关系);
        }
    }
    b.规范();
    return prepared;
}
概念树类数据服务::共享准备结果 概念树类数据服务::准备共享写入(
    const 概念树共享写请求 &request, 共享读取上下文 &c) const {
    共享准备结果 out;
    if(const auto *r=std::get_if<概念树共享建立请求>(&request))out=准备共享创建(*r,c);
    else if(const auto *r=std::get_if<概念树共享名称请求>(&request))out=准备共享名称(*r,c);
    else if(const auto *r=std::get_if<概念树共享使用请求>(&request))out=准备共享用途(*r,c);
    else out=准备共享治理(request,c);
    auto &ws = out.构造.写集;
    const auto head = 共享请求头(request);
    ws.合同版本 = L1所有者范围CRUD合同版本;
    ws.写入幂等身份 = {head.幂等身份};
    ws.期望事实代次 = head.期望事实代次;
    检查数量(ws.关系.size(), c.预算.基础.最大关系数);
    return out;
}
概念树共享写入结果 概念树类数据服务::共享发布读回(
    const 概念树共享写请求 &request, const L1所有者范围写集请求 &ws,
    const L1所有者范围写入结果 &saved, 共享读取上下文 &c) const {
    const auto map = 共享核验首次(ws, saved, c);
    概念树共享写入结果 out;
    out.Gread = c.G;
    out.首次H = saved.事实代次;
    out.发布状态 = P::已确认发布;
    out.原请求 = request;
    out.已退出编码 = ws.退出事实;
    auto lookup = [&](std::uint32_t key) {
        const auto it = map.find(key);
        if (it == map.end()) throw 失败{S::内部不一致};
        return it->second;
    };
    std::visit([&](const auto &r) {
        using T = std::decay_t<decltype(r)>;
        if constexpr (std::is_same_v<T, 概念树共享建立请求>) {
            out.概念 = 展开共享定义(概念树概念身份{lookup(1)}, c).事实;
            if (out.概念->共享定义 != r.定义) throw 失败{S::内部不一致};
            out.直接边 = out.概念->直接上位组;
            out.状态 = S::已创建共享定义;
        } else if constexpr (std::is_same_v<T, 概念树共享名称请求>) {
            const auto names = 共享名称组(c);
            const auto id = r.新概念 ? lookup(0x10000) : r.预期->关系;
            for (const auto &f : names) if (f.关系 == id) {
                if (out.名称) throw 失败{S::内部不一致};
                out.名称 = f;
            }
            if (!out.名称 || out.名称->F != r.F ||
                (r.新概念 && (out.名称->概念 != *r.新概念 ||
                    (out.名称->生命周期.退出事实代次 && *out.名称->生命周期.退出事实代次 <= c.H))) ||
                (!r.新概念 && out.名称->生命周期.退出事实代次 != saved.事实代次))
                throw 失败{S::内部不一致};
            out.状态 = r.新概念 ? S::已绑定共享名称 : S::已退出共享名称;
        } else if constexpr (std::is_same_v<T, 概念树共享使用请求>) {
            out.用途 = 共享用途(lookup(1), c);
            if (out.用途->概念 != r.概念 || out.用途->业务标识 != r.业务标识 ||
                out.用途->观察 != r.观察 || out.用途->跟踪 != r.跟踪 ||
                out.用途->原H != r.原H || out.用途->时间 != r.时间 || out.用途->用途角色 != r.用途角色)
                throw 失败{S::内部不一致};
            out.状态 = S::已记录共享用途;
        } else if constexpr (std::is_same_v<T, 概念树共享释放请求>) {
            if (ws.退出事实.size() != r.完整预期.size()) throw 失败{S::内部不一致};
            out.状态 = S::已释放引用;
        } else if constexpr (std::is_same_v<T, 概念树共享退出请求>) {
            const auto after = c.H;
            c.H = ws.期望事实代次;
            try { out.概念 = 展开共享定义(r.操作.概念, c).事实; }
            catch (...) { c.H = after; throw; }
            c.H = after;
            const auto raw = 原始事实(r.操作.概念.值, c.G);
            const auto *n = std::get_if<N>(&raw);
            if (!n || n->退出事实代次 != saved.事实代次) throw 失败{S::内部不一致};
            out.概念->生命周期.退出事实代次 = saved.事实代次;
            out.状态 = S::已退出概念;
            for (const auto &v : r.完整替代) {
                const auto child = 展开共享定义(v.子节点, c).事实;
                if (child.直接上位组.size() != v.新父组.size()) throw 失败{S::内部不一致};
                for (std::size_t i = 0; i < v.新父组.size(); ++i)
                    if (child.直接上位组[i].上位 != v.新父组[i]) throw 失败{S::内部不一致};
                out.直接边.insert(out.直接边.end(), child.直接上位组.begin(), child.直接上位组.end());
            }
        } else {
            const auto id = [&] {
                if constexpr (std::is_same_v<T, 概念树共享上位请求>) return r.下位;
                else return r.操作.概念;
            }();
            out.概念 = 展开共享定义(id, c).事实;
            out.直接边 = out.概念->直接上位组;
            if constexpr (std::is_same_v<T, 概念树共享上位请求>) {
                if(out.直接边.size()!=r.新父组.size())throw 失败{S::内部不一致};
                for(std::size_t i=0;i<r.新父组.size();++i)if(out.直接边[i].上位!=r.新父组[i])throw 失败{S::内部不一致};
                out.状态=S::已替换上位;
            }
            else {
                if (out.概念->治理状态 != r.操作.目标状态) throw 失败{S::内部不一致};
                out.状态 = S::已迁移生命周期;
            }
        }
    }, request);
    return out;
}

bool 概念树共享写入结果::成功() const noexcept {
    using S = 概念树数据状态;
    if (版本 != 1 || !Gread || !原请求 || 原请求->valueless_by_exception()) return false;
    const bool reuse = 状态 == S::无须变更;
    const bool replay = 状态 == S::精确重复;
    if (reuse) {
        if (首次H || 发布状态 != 概念树发布状态::未派发 || !已退出编码.empty()) return false;
    } else if (!首次H || !*首次H || *首次H > Gread || 发布状态 != 概念树发布状态::已确认发布) return false;
    if (概念 && (!概念->概念.值.值 || !概念->定义记录.值 || !概念->类型域身份.值.值 ||
        !概念->生命周期.创建事实代次 || 概念->共享定义.类型.完整域.valueless_by_exception() ||
        (概念->共享定义.完整域 != 概念->共享定义.原子组.empty()))) return false;
    return std::visit([&](const auto &r) noexcept {
        using T = std::decay_t<decltype(r)>;
        if (r.版本 != 1) return false;
        if constexpr (std::is_same_v<T, 概念树共享建立请求>) {
            return (reuse || replay || 状态 == S::已创建共享定义) && 概念 &&
                概念->共享定义 == r.定义 && !名称 && !用途;
        } else if constexpr (std::is_same_v<T, 概念树共享名称请求>) {
            return (reuse || replay || 状态 == (r.新概念 ? S::已绑定共享名称 : S::已退出共享名称)) &&
                名称 && 名称->关系.值 && 名称->F == r.F && !概念 && !用途 &&
                (r.新概念 ? 名称->概念 == *r.新概念 : r.预期 && 名称->关系 == r.预期->关系);
        } else if constexpr (std::is_same_v<T, 概念树共享使用请求>) {
            return (reuse || replay || 状态 == S::已记录共享用途) && 用途 &&
                用途->记录.值 && 用途->概念 == r.概念 && 用途->观察 == r.观察 &&
                用途->跟踪 == r.跟踪 && 用途->原H == r.原H && 用途->时间 == r.时间 &&
                用途->用途角色 == r.用途角色 && (reuse || 用途->业务标识 == r.业务标识) && !概念 && !名称;
        } else if constexpr (std::is_same_v<T, 概念树共享释放请求>) {
            if (!replay && !reuse && 状态 != S::已释放引用) return false;
            if (概念 || 名称 || 用途 || 已退出编码.size() != r.完整预期.size()) return false;
            for (std::size_t i = 0; i < 已退出编码.size(); ++i)
                if (已退出编码[i] != r.完整预期[i].关系) return false;
            return true;
        } else if constexpr (std::is_same_v<T, 概念树共享上位请求>) {
            if((!reuse&&!replay&&状态!=S::已替换上位)||!概念||概念->概念!=r.下位||名称||用途||
                直接边.size()!=r.新父组.size()||概念->直接上位组!=直接边||r.新父组.empty())return false;
            std::size_t removed=0;
            for(std::size_t i=0;i<直接边.size();++i){const auto&e=直接边[i];
                if(e.上位!=r.新父组[i]||e.下位!=r.下位||!e.关系.值||
                    (i&&r.新父组[i-1].值.值>=r.新父组[i].值.值))return false;
                const auto old=std::find_if(r.预期父组.begin(),r.预期父组.end(),[&](const auto&p){return p.上位==e.上位;});
                if(old!=r.预期父组.end()&&(old->关系!=e.关系||old->生命周期.创建事实代次!=e.生命周期.创建事实代次))return false;
                if(old==r.预期父组.end()&&(!首次H||e.生命周期.创建事实代次!=*首次H))return false;
            }
            for(const auto&e:r.预期父组)if(std::find(r.新父组.begin(),r.新父组.end(),e.上位)==r.新父组.end()){
                ++removed;if(std::count(已退出编码.begin(),已退出编码.end(),e.关系)!=1)return false;
            }
            return removed==已退出编码.size();
        } else if constexpr (std::is_same_v<T, 概念树共享生命周期请求>) {
            return (reuse || replay || 状态 == S::已迁移生命周期) && 概念 &&
                概念->概念 == r.操作.概念 && 概念->治理状态 == r.操作.目标状态 && !名称 && !用途;
        } else {
            return (replay || 状态 == S::已退出概念) && 概念 && 概念->概念 == r.操作.概念 &&
                概念->生命周期.退出事实代次 == 首次H && !名称 && !用途;
        }
    }, *原请求);
}

概念树共享写入结果 概念树类数据服务::创建共享特征概念(const 概念树共享建立请求 &r) { return 共享执行(r); }
概念树共享写入结果 概念树类数据服务::操作共享名称字段(const 概念树共享名称请求 &r) { return 共享执行(r); }
概念树共享写入结果 概念树类数据服务::记录共享概念使用(const 概念树共享使用请求 &r) { return 共享执行(r); }
概念树共享写入结果 概念树类数据服务::操作共享概念上位(const 概念树共享上位请求 &r) { return 共享执行(r); }
概念树共享写入结果 概念树类数据服务::迁移共享概念生命周期(const 概念树共享生命周期请求 &r) { return 共享执行(r); }
概念树共享写入结果 概念树类数据服务::退出共享特征概念(const 概念树共享退出请求 &r) { return 共享执行(r); }
概念树共享写入结果 概念树类数据服务::释放共享概念引用(const 概念树共享释放请求 &r) { return 共享执行(r); }
bool operator==(const 概念树共享特征定义 &a, const 概念树共享特征定义 &b) noexcept {
    return a.FT == b.FT && a.类型 == b.类型 && a.完整域 == b.完整域 && a.原子组 == b.原子组;
}

void 概念树类数据服务::共享名称交叉核验(稳定编码 id, bool incoming,
    const std::vector<概念树共享名称事实> &all, 共享读取上下文 &c) const {
    const auto edges = 共享关系(id, shared_->类型[7], incoming, c.H, c);
    std::set<std::uint64_t> ids;
    for (const auto &edge : edges) {
        if (edge.角色或顺序 != 1 || !ids.insert(edge.编码.值).second) throw 失败{S::内部不一致};
        if (std::count_if(all.begin(), all.end(), [&](const auto &f) {
            return f.关系 == edge.编码 && f.F.值 == edge.源节点 && f.概念.值 == edge.目标节点 &&
                f.生命周期.创建事实代次 == edge.创建事实代次;
        }) != 1) throw 失败{S::内部不一致};
        if (incoming) 共享名称交叉核验(edge.源节点, false, all, c);
    }
    if (!incoming && edges.size() > 1) throw 失败{S::内部不一致};
    for (const auto &f : all) {
        if ((incoming ? f.概念.值 : f.F.值) != id || f.生命周期.创建事实代次 > c.H ||
            (f.生命周期.退出事实代次 && *f.生命周期.退出事实代次 <= c.H)) continue;
        if (!ids.contains(f.关系.值)) throw 失败{S::内部不一致};
    }
}
const std::vector<特征区间事实> &概念树类数据服务::共享区间组(
    特征跟踪身份 track, std::uint64_t h, 共享读取上下文 &c) const {
    const auto key = std::make_pair(track.值.值, h);
    if (const auto it = c.区间缓存.find(key); it != c.区间缓存.end()) return it->second;
    auto result = features_.读取长期区间({{1, c.G, h}, track, c.预算.特征});
    if (!result.成功()) throw 失败{共享映射(result.状态)};
    if (result.Gread != c.G || result.H != h) throw 失败{S::内部不一致};
    const auto &group = *result.数据;
    if (c.特征记录扫描数 > c.预算.特征.最大记录数 || group.size() > c.预算.特征.最大记录数 - c.特征记录扫描数)
        throw 失败{S::数量预算不足};
    c.特征记录扫描数 += group.size();
    // 只累计公开完整组实际返回的记录和原子材料，不推算 A 的私有展开用量。
    for (const auto &item : group) {
        if (item.跟踪 != track) throw 失败{S::内部不一致};
        if (const auto *image = std::get_if<特征二值半径域>(&item.原子域.域)) {
            if (c.像素数 > c.预算.特征.最大像素数 ||
                image->代表.像素.size() > c.预算.特征.最大像素数 - c.像素数)
                throw 失败{S::数量预算不足};
            c.像素数 += image->代表.像素.size();
        }
    }
    return c.区间缓存.emplace(key, std::move(*result.数据)).first->second;
}


概念树类数据服务::存在展开_v2 概念树类数据服务::展开存在定义_v2(
    概念树概念身份 id, 共享读取上下文 &c) const {
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
    f.生命周期 = {born, n.退出事实代次}; f.生命周期值事实 = state.编码;
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
        const auto cs = shared_ ? 共享关系(sn.编码, shared_->类型[15], false, born, c) : std::vector<E>{};
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
            else builder.关系(Ref{sn}, Ref{std::get<概念树概念身份>(r.依据.来源).值}, shared_->类型[15]);
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
                    for (auto type : {t.定义模板, shared_->类型[15]}) if (!共享关系(id.值, type, true, c.H, c).empty()) throw 失败{S::引用冲突};
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
        auto &after = c; after.区间缓存.clear(); after.动态用量.已核验.clear(); after.G = g; after.H = saved.事实代次;
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
    bool unknown = false, dispatched = false;
    try {
        out.原请求 = input;
        if (input.版本 != 1 || input.操作.valueless_by_exception()) throw 失败{S::入口拒绝};
        if (!shared_) throw 失败{S::不支持};
        const auto head = std::visit([](const auto &r) { return 请求写头(r); }, input.操作);
        if (head.合同版本 != 1 || !head.期望事实代次 || !head.幂等身份) throw 失败{S::入口拒绝};
        std::scoped_lock lock(mutex_);
        unknown = true;
        const auto first = port_.读取首次写入材料({L1所有者范围首次写入读取合同版本, {head.幂等身份}});
        if (first.状态 != L1所有者范围读取状态::成功 && first.状态 != L1所有者范围读取状态::未找到) throw 失败{映射(first.状态)};
        if (first.合同版本 != L1所有者范围首次写入读取合同版本 || first.所有者 != port_.所有者身份() ||
            first.写入幂等身份.值 != head.幂等身份 || !first.读取事实代次) throw 失败{S::内部不一致};
        out.Gread = first.读取事实代次;
        if (first.首次写入结果 && first.首次写入结果->事实代次 && first.首次写入结果->事实代次 <= out.Gread)
            out.首次H = first.首次写入结果->事实代次;
        守卫代次(out.Gread);
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
        out.原请求 = request;
        共享读取上下文 c{out.Gread, head.期望事实代次, request.预算};
        const auto prepare = [&] {
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
                    if (kind != 1 && kind != 3) throw 失败{S::不支持};
                    const auto root = 唯一关系(id.值, layout_.类型.概念根归属, c.G, c.H);
                    if (root.目标节点 != (kind == 1 ? layout_.根组.特征根.值 : layout_.根组.动态根.值)) throw 失败{S::引用冲突};
                }
            }, request.操作);
            return 形成写集(oldRequest, c.G, c.H, &c);
        };
        const auto readback = [&](const L1所有者范围写集请求 &ws, const L1所有者范围写入结果 &saved) {
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
            if (c.G != c.H) throw 失败{S::事实代次漂移};
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
                守卫代次(c.G); dispatched = true;
                const auto saved = port_.提交所有者范围中性写集(ws);
                if (saved.事实代次 > head.期望事实代次 && (saved.是否形成内存权威发布 || saved.状态 == L1所有者范围写入状态::精确重复))
                    out.首次H = saved.事实代次;
                c.G = 当前代次(); c.H = saved.事实代次; c.区间缓存.clear(); c.动态用量.已核验.clear(); out.Gread = c.G;
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
        return out;
    } catch (const 失败 &e) { out.状态 = e.状态; }
    catch (const std::bad_alloc &) { out.状态 = S::资源失败; }
    catch (const std::length_error &) { out.状态 = S::资源失败; }
    catch (...) { out.状态 = S::内部不一致; }
    out.概念.reset(); out.支持.reset(); out.关系组.clear(); out.已退出事实.clear(); out.释放所属概念组.clear();
    if (unknown || dispatched) { out.状态 = S::已可能发布; out.发布状态 = P::可能已发布; }
    else out.发布状态 = out.首次H ? P::可能已发布 : P::未派发;
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
                const auto id=std::visit([](const auto &v){return v.概念;},node);
                if(id.值.值<=previous)return false; previous=id.值.值;
                if constexpr (std::is_same_v<T, 概念树形成引用释放请求>) {
                    if(std::none_of(expected.begin(),expected.end(),[&](const auto&v){return v.所属概念==id;}))return false;
                }else if(id!=r.所属概念)return false;
            }
            return true;
        }
        else {
            if (!概念 || 支持) return false;
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
        if (!shared_) throw 失败{S::不支持};
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
        else {
            if (!有效(f.概念.值)||!有效(f.本体根.值)||f.概念==f.本体根||!有效(f.规则.值)||
                !有效(f.定义记录)||!有效(f.生命周期值事实)||!lifeComplete(f.生命周期,h)||f.直接上位组.empty()||
                (f.治理状态!=概念树生命周期状态::活跃&&f.治理状态!=概念树生命周期状态::冷却&&f.治理状态!=概念树生命周期状态::退役))return false;
            for(std::size_t i=0;i<f.直接上位组.size();++i){const auto &e=f.直接上位组[i];
                if(!有效(e.关系)||!有效(e.上位.值)||e.下位!=f.概念||e.上位==e.下位||!lifeComplete(e.生命周期,h))return false;
                for(std::size_t j=0;j<i;++j)if(f.直接上位组[j].关系==e.关系||f.直接上位组[j].上位==e.上位)return false;
            }
            if constexpr(std::is_same_v<T,概念树应用存在事实>){
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
            }else{
                const auto &d=f.共享定义;
                if(!有效(f.类型域身份.值)||!有效(d.FT.值)||d.类型.FT!=d.FT.值||d.类型.完整域.valueless_by_exception()||
                    (d.类型.算法!=特征长期算法::I64幅度&&d.类型.算法!=特征长期算法::二值边界平均)||
                    (d.完整域 ? !d.原子组.empty() : d.原子组.empty())||d.原子组.size()!=f.原子事实组.size()||f.来源事实组.empty())return false;
                for(std::size_t i=0;i<d.原子组.size();++i){const auto&a=f.原子事实组[i];
                    if(a.原子.域.valueless_by_exception()||a.原子.类型!=d.类型||a.原子!=d.原子组[i]||
                        !有效(a.节点)||!有效(a.定义成员)||!lifeComplete(a.生命周期,h)||a.属性.empty())return false;
                    if(const auto*v=std::get_if<特征I64闭区间>(&a.原子.域)){if(v->下界>v->上界)return false;}
                    else {const auto&binary=std::get<特征二值半径域>(a.原子.域);if(binary.半径<0||binary.代表.像素.empty())return false;}
                }
                for(const auto &v:f.来源事实组){
                    if(!有效(v.记录)||!有效(v.归属关系)||v.种类<1||v.种类>4||!v.H||v.H>=f.生命周期.创建事实代次||
                        !lifeComplete(v.生命周期,h)||v.引用.empty())return false;
                    for(const auto&r:v.引用)if(!有效(r.关系)||r.记录!=v.记录||r.概念!=f.概念||!有效(r.目标)||!lifeComplete(r.生命周期,h,true))return false;
                    if(v.种类==4&&!d.完整域)return false;
                }
            }
            return true;
        }
    },node);
}

} // namespace 海中鱼巣
