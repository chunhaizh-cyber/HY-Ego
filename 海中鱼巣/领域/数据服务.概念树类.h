#pragma once

#include "数据服务.绑定存在.h"
#include "数据服务.存在类.h"
#include "数据服务.场景类.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "../核心/容错检查.h"

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


#include "合同.相关概念添加参与.h"

namespace 海中鱼巣 {

struct 概念树规则身份 final {
    稳定编码 值{};
    概念树规则身份() = default;
    explicit 概念树规则身份(稳定编码 编码) : 值(编码) {
    }
    friend bool operator==(const 概念树规则身份 &, const 概念树规则身份 &) = default;
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
enum class 概念树根角色 : std::uint8_t {
    存在 = 1,
    特征,
    动态,
    因果链
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

struct 概念树截止结果 final {
    std::uint32_t 合同版本 = 1;
    概念树数据状态 状态 = 概念树数据状态::入口拒绝;
    std::uint64_t Gread = 0;
    bool 成功() const noexcept {
        return 合同版本 == 1 && 状态 == 概念树数据状态::已读取 && Gread;
    }
};
using 概念树业务写请求 = std::variant<概念树概念建立请求, 概念树上位操作请求, 概念树支持建立请求,
                                      概念树支持退出请求, 概念树生命周期迁移请求, 概念树概念退出请求,
                                      概念树形成引用释放请求>;

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
struct 特征概念出生使用结构交付 final {
    稳定编码 锚点{};
    std::uint32_t 格式=1;
    稳定编码 F到FCv出生使用关系类型{};
    friend bool operator==(const 特征概念出生使用结构交付&,const 特征概念出生使用结构交付&)=default;
};
struct 特征概念出生使用结构登记请求 final {
    std::uint32_t 版本=1; std::uint64_t G0=0;
    L1所有者范围写入幂等身份 幂等键{};
    纯概念结构交付_v2 纯概念结构{}; std::uint64_t 最大首次材料项数=0;
    friend bool operator==(const 特征概念出生使用结构登记请求&,const 特征概念出生使用结构登记请求&)=default;
};
struct 特征概念出生使用结构首次材料读回 final {
    L1所有者范围节点事实 锚点;
    L1所有者范围节点事实 F到FCv出生使用关系类型;
    L1所有者范围关系事实 类型登记关系;
    L1所有者范围值事实 格式值;
    friend bool operator==(const 特征概念出生使用结构首次材料读回&,const 特征概念出生使用结构首次材料读回&)=default;
};
struct 特征概念出生使用结构登记结果 final {
    std::uint32_t 版本=1;
    纯概念状态 状态=纯概念状态::入口拒绝;
    纯概念发布状态 发布=纯概念发布状态::未进入;
    std::uint64_t Gread=0;
    std::optional<std::uint64_t> 首次发布H;
    std::optional<特征概念出生使用结构登记请求> 原请求;
    std::optional<特征概念出生使用结构交付> 交付;
    std::optional<特征概念出生使用结构首次材料读回> 首次材料;
    bool 成功(const 特征概念出生使用结构登记请求&) const noexcept;
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
using 概念树应用模板事实 = std::variant<特征概念事实>;
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
using 概念树应用业务操作 = std::variant<概念树支持建立请求, 概念树支持退出请求,
    概念树上位操作请求, 概念树生命周期迁移请求, 概念树概念退出请求, 概念树形成引用释放请求>;
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
    概念树形成引用读取请求>;
struct 概念树应用关联请求 final {
    std::uint32_t 版本 = 1;
    概念树应用关联操作 操作;
    概念树共享预算 预算;
};
using 概念树应用关联事实 = std::variant<std::vector<概念树支持事实>, std::vector<概念树形成引用事实>>;
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

// 4180 §5.2：仅供已知 FC 的窄读；不枚举概念树，也不承担值域比较。
enum class 特征概念值域基础读取状态_v1 : std::uint8_t {
    已读取 = 1, 未找到, 目标已退出, 类别冲突, 规则缺失, 未实现,
    事实代次漂移, 数量预算不足, 历史材料不可用, 资源失败,
    内部不一致, 入口拒绝
};
struct 特征概念值域基础读取请求_v1 final {
    std::uint32_t 版本 = 1;
    std::uint64_t Gread = 0, H = 0;
    概念树概念身份 FC;
    概念树预算 预算;
};
struct 特征概念值域基础事实_v1 final {
    概念树概念身份 FC;
    特征类型身份 FT;
    特征值表示类型 表示 = 特征值表示类型::I64;
    std::optional<特征规范I64域> I64域;
    概念树生命周期 生命周期;
    std::uint64_t Gread = 0, H = 0;
};
struct 特征概念值域基础读取结果_v1 final {
    std::uint32_t 版本 = 1;
    特征概念值域基础读取状态_v1 状态 = 特征概念值域基础读取状态_v1::入口拒绝;
    std::uint64_t Gread = 0, H = 0;
    std::optional<特征概念值域基础事实_v1> 事实;
    bool 成功(const 特征概念值域基础读取请求_v1&) const noexcept;
};

class 概念树类数据服务 final : public 相关概念添加参与者,
                              public 已发布概念引用参与者,
                              public 原子I64特征概念参与者 {
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
    struct 相关失败 {
        相关概念参与状态 状态;
    };
    struct 纯失败 {
        纯概念状态 状态;
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

  public:
    static 纯概念结构登记结果_v2 登记纯概念结构_v2(
        const L1事实基座服务 &, L1所有者范围写端口 &,
        const 纯概念结构登记请求_v2 &) noexcept;
    static 特征概念出生使用结构登记结果 登记特征概念出生使用结构(
        const L1事实基座服务 &, L1所有者范围写端口 &,
        const 特征概念出生使用结构登记请求 &) noexcept;
    static 存在概念两组结构登记结果_v1 登记存在概念两组结构_v1(
        const L1事实基座服务 &, L1所有者范围写端口 &,
        const 存在概念两组结构登记请求_v1 &) noexcept;
    概念树类数据服务(const L1事实基座服务 &, const 特征类数据服务 &,
                     const 存在类数据服务 &, const 特征值类数据服务 &,
                     const 场景类数据服务 &, L1所有者范围写端口 &&,
                     const 纯概念结构交付_v2 &,
                     const 特征概念出生使用结构交付 &,
                     const 存在概念两组结构交付_v1 &);
    概念树类数据服务(const L1事实基座服务 &, const 特征类数据服务 &, const 存在类数据服务 &,
                     const 特征值类数据服务 &, L1所有者范围写端口 &&, const 概念树结构交付 &,
                     const 概念树共享结构交付 &);
    bool 绑定于(const L1事实基座服务 &x) const noexcept override { return &l1_ == &x; }
    相关概念参与片段 准备相关概念片段(const 相关概念参与请求 &,
                                         std::uint64_t Gread,
                                         L1有限N分区原子参与者身份_v3) const noexcept override;
    相关概念组合提交结果 提交相关概念组合事务(
        const 相关概念组合提交请求 &,
        std::span<L1所有者范围写端口 *const>) noexcept override;
    相关概念参与读回 读取相关概念结果(const 相关概念参与请求 &,
                                         std::uint64_t Gread,
                                         std::uint64_t H) const noexcept override;
    概念树类数据服务(const L1事实基座服务 &, const 特征类数据服务 &,
                     const 存在类数据服务 &, const 特征值类数据服务 &,
                     const 场景类数据服务 &, L1所有者范围写端口 &&,
                     const 相关概念结构交付 &);
    概念树类数据服务(const L1事实基座服务 &, const 特征类数据服务 &,
                     const 存在类数据服务 &, const 特征值类数据服务 &,
                     const 场景类数据服务 &, L1所有者范围写端口 &&,
                     const 概念树结构交付 &, const 相关概念结构交付 &);
    bool 使用特征存在服务(const 特征类数据服务 &f, const 存在类数据服务 &e) const noexcept {
        return &features_ == &f && &existences_ == &e;
    }
    bool 使用特征服务(const 特征类数据服务& f) const noexcept { return &features_==&f; }
    bool 共享应用已启用() const noexcept { return feature_layout_.has_value(); }
    概念树类数据服务(const L1事实基座服务&, const 特征类数据服务&, const 存在类数据服务&,
        const 特征值类数据服务&, L1所有者范围写端口&&, const 概念树结构交付&, const 特征概念结构交付&);
    概念树类数据服务(const L1事实基座服务&, const 特征类数据服务&, const 存在类数据服务&,
        const 特征值类数据服务&, L1所有者范围写端口&&, const 概念树结构交付&,
        const 特征概念结构交付&, const 概念树存在扩展交付&);
    bool 使用同一场景基座(const 场景类数据服务 &s) const noexcept { return s.绑定于(l1_); }
    概念树概念身份 特征根引用() const noexcept { return layout_ ? layout_->根组.特征根 : 概念树概念身份{}; }
    概念树概念身份 存在根引用() const noexcept { return layout_ ? layout_->根组.存在根 : 概念树概念身份{}; }
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


    概念树概念身份 动态根引用() const noexcept {
        return layout_ ? layout_->根组.动态根 : 概念树概念身份{};
    }
    概念树截止结果 读取当前事实代次() const;
    存在概念两组规范化结果_v1 规范化存在概念两组定义(
        const 存在概念两组规范化请求_v1&) const noexcept;
    存在概念两组枚举结果_v1 枚举存在概念候选(
        const 存在概念两组枚举请求_v1&) const noexcept;
    存在概念两组查询结果_v1 精确查询存在概念(
        const 存在概念两组查询请求_v1&) const noexcept;
    存在概念两组写入结果_v1 创建或复用存在概念(
        const 存在概念两组创建请求_v1&) noexcept;
    存在概念两组读取结果_v1 读取存在概念两组定义(
        const 存在概念两组读取请求_v1&) const noexcept;
    纯概念查询结果 精确查询纯概念(const 纯概念查询请求&) const noexcept;
    I64特征概念组织读取结果 读取当前I64特征概念(
        const I64特征概念组织读取请求&) const noexcept;
    特征概念值域基础读取结果_v1 读取特征概念值域基础(
        const 特征概念值域基础读取请求_v1&) const noexcept;
    纯概念写入结果 创建或复用纯概念(const 纯概念创建请求&) noexcept;
    纯概念创建恢复结果 读取纯概念创建首次结果(
        const 纯概念创建恢复请求&) const noexcept;
    纯概念读取结果 读取纯概念(const 纯概念读取请求&) const noexcept;
    纯概念生命周期结果 迁移纯概念生命周期(
        const 纯概念生命周期请求&) noexcept;
    纯概念退出结果 退出无引用叶概念(const 纯概念退出请求&) noexcept;
    存在概念使用读取结果 读取存在概念使用(
        const 存在概念使用读取请求&) const noexcept override;
    const L1事实基座服务& 原子I64底座() const noexcept override;
    L1所有者范围写端口& 原子I64端口() noexcept override;
    bool 原子I64结构已就绪() const noexcept override;
    原子I64特征参与结果<L1有限N分区原子参与者写集_v3>
    准备原子I64出生片段(const 原子I64特征出生请求&,std::uint64_t) const override;
    原子I64特征出生使用读取结果 读取原子I64出生使用(
        const 原子I64特征出生使用读取请求&) const override;
    原子I64特征参与结果<L1有限N分区原子参与者写集_v3>
    准备原子I64出生使用退出片段(
        const 原子I64特征出生使用退出片段请求&,std::uint64_t) const override;


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
        const bool existence = std::holds_alternative<概念树存在定义>(r.定义);
        if (existence && r.直接上位.empty()) {
            try {
                追根因检查(false, L"普通存在概念创建前必须明确直接上位绑定节点");
            } catch (...) {}
        }
        auto result = 执行写入(r);
        if (existence && !r.直接上位.empty() && !result.成功()) {
            try {
                追根因检查(false, L"普通存在概念创建失败：必须先明确合法直接上位绑定节点；请核对结构化结果原因");
            } catch (...) {
                // 诊断失败不得改写已经确定的机器结果。
            }
        }
        return result;
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
    std::optional<概念树结构交付> layout_;
    const 场景类数据服务 *scenes_ = nullptr;
    std::optional<相关概念结构交付> related_layout_;
    std::optional<纯概念结构交付_v2> pure_layout_;
    std::optional<特征概念出生使用结构交付> feature_birth_layout_;
    std::optional<存在概念两组结构交付_v1> two_group_definition_layout_;
    纯概念定义 规范化纯概念定义(const 纯概念定义&, std::uint64_t,
                              std::uint64_t, const 概念树预算&) const;
    纯概念事实 读取纯概念内部(概念树概念身份, std::uint64_t,
                            std::uint64_t, const 概念树预算&) const;
    void 核验纯概念无保护引用(const 纯概念事实&, std::uint64_t,
                               const 概念树预算&) const;
    L1所有者范围写集请求 形成纯概念写集(
        const 纯概念创建请求&, const 纯概念定义&) const;
    存在概念两组定义_v3 规范化两组定义内部(
        const 存在概念两组定义_v3&, std::uint64_t, std::uint64_t,
        const 存在概念两组预算_v3&) const;
    存在概念两组事实_v3 读取两组定义内部(
        概念树概念身份, std::uint64_t, std::uint64_t,
        const 存在概念两组预算_v3&) const;
    L1所有者范围写端口& 借用存在概念引用写端口() noexcept override {
        return port_;
    }
    已发布概念引用片段结果 准备存在概念引用片段(
        const 已发布存在概念引用准备请求&, std::uint64_t) const noexcept override;
    已发布概念引用首次材料结果 读取存在概念引用首次材料(
        L1所有者范围写入幂等身份) const noexcept override;
    const 概念树结构交付 &旧布局() const {
        if (!layout_) throw 失败{S::旧格式不支持};
        return *layout_;
    }
    void 校验相关概念布局(std::uint64_t) const;
    struct 相关概念展开 final {
        概念树概念身份 概念;
        相关概念类别 类别 = 相关概念类别::存在;
        概念树定义 定义;
        std::vector<概念树直接上位事实> 直接上位;
        std::vector<概念树形成引用事实> 挂接;
        概念树生命周期 生命周期;
    };
    相关概念展开 展开相关概念(概念树概念身份, std::uint64_t,
                                  std::uint64_t, const 相关概念读取预算 &) const;
    相关概念完整事实 读取相关概念内部(const 相关概念参与请求 &,
                                      概念树概念身份, std::uint64_t,
                                      std::uint64_t) const;
    相关概念参与状态 映射相关状态(S) const noexcept;
    void 验证相关世界(const 相关概念世界挂靠 &, std::uint64_t,
                      const 相关概念读取预算 &) const;
    L1有限N分区原子参与者写集_v3 形成相关概念写集(
        const 相关概念参与请求 &, std::uint64_t,
        L1有限N分区原子参与者身份_v3,
        const std::optional<相关概念完整事实> &) const;

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
    struct 共享读取上下文 {
        std::uint64_t G = 0, H = 0;
        概念树共享预算 预算;
        std::uint64_t 来源数 = 0, 关系数 = 0, 首次材料数 = 0;
        std::uint64_t 特征记录扫描数 = 0;
        概念树应用读取用量 用量;
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
    稳定编码 支持关系类型(const 概念树世界引用 &) const;
    template <class T> static const 概念树写入头 &请求写头(const T &r) { return r.写入头; }
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
    template <class T> static 概念树生命周期 投影纯生命(const T &v, std::uint64_t g) {
        return {v.创建事实代次, v.退出事实代次 && *v.退出事实代次 <= g ? v.退出事实代次 : std::nullopt};
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

} // namespace 海中鱼巣
