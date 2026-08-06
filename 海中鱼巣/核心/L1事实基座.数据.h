#ifndef L1_FACT_BASE_NO_INCLUDES
#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <optional>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
#endif

#ifndef L1_PUBLIC_FACT_NO_INCLUDES
#include "L1公共事实.数据.h"
#endif

namespace 海中鱼巣 {

struct 写集本地键 final {
    std::uint32_t 值 = 0;
    friend bool operator==(const 写集本地键&, const 写集本地键&) = default;
    friend bool operator<(const 写集本地键& 左, const 写集本地键& 右) noexcept { return 左.值 < 右.值; }
};

struct 写集幂等键 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const 写集幂等键&, const 写集幂等键&) = default;
};

struct 独立材料引用 final {
    稳定编码 编码;
    friend bool operator==(const 独立材料引用&, const 独立材料引用&) = default;
};

using 事实引用 = std::variant<稳定编码, 写集本地键>;
using 原始值材料 = std::variant<std::int64_t, std::vector<std::int64_t>,
    std::vector<std::uint64_t>, 独立材料引用>;

enum class 值表示种类 : std::uint8_t { I64 = 1, I64组 = 2, U64组 = 3, 独立材料引用 = 4 };

struct 属性槽 final {
    稳定编码 属性类型节点;
    稳定编码 当前值;
    friend bool operator==(const 属性槽&, const 属性槽&) = default;
};

struct 节点事实 final {
    稳定编码 编码;
    节点种类 种类 = 节点种类::普通;
    std::optional<值表示种类> 属性类型表示;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    std::vector<属性槽> 当前属性;
    friend bool operator==(const 节点事实&, const 节点事实&) = default;
};

struct 关系事实 final {
    稳定编码 编码;
    稳定编码 源节点;
    稳定编码 目标节点;
    稳定编码 关系类型节点;
    std::int64_t 角色或顺序 = 0;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    friend bool operator==(const 关系事实&, const 关系事实&) = default;
};

struct 值事实 final {
    稳定编码 编码;
    稳定编码 所属节点;
    稳定编码 属性类型节点;
    原始值材料 材料;
    稳定编码 来源节点;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    friend bool operator==(const 值事实&, const 值事实&) = default;
};

struct 节点新建项 final {
    写集本地键 本地键;
    节点种类 种类 = 节点种类::普通;
    std::optional<值表示种类> 属性类型表示;
    friend bool operator==(const 节点新建项&, const 节点新建项&) = default;
};
struct 关系新建项 final {
    写集本地键 本地键;
    事实引用 源节点;
    事实引用 目标节点;
    事实引用 关系类型节点;
    std::int64_t 角色或顺序 = 0;
    friend bool operator==(const 关系新建项&, const 关系新建项&) = default;
};
struct 值新建项 final {
    写集本地键 本地键;
    事实引用 所属节点;
    事实引用 属性类型节点;
    原始值材料 材料;
    事实引用 来源节点;
    friend bool operator==(const 值新建项&, const 值新建项&) = default;
};
struct 属性槽变更项 final {
    事实引用 所属节点;
    事实引用 属性类型节点;
    写集本地键 新当前值;
    friend bool operator==(const 属性槽变更项&, const 属性槽变更项&) = default;
};

inline constexpr std::uint32_t L1幂等摘要合同版本 = 1;
inline constexpr std::uint32_t L1确定性结果摘要版本 = 1;
inline constexpr std::uint32_t L1通用发布后读回计划版本 = 1;

struct L1确定性摘要 final {
    std::array<std::uint8_t, 32> 字节{};
    friend bool operator==(const L1确定性摘要& 左, const L1确定性摘要& 右) noexcept {
        for (std::size_t i = 0; i < 左.字节.size(); ++i) if (左.字节[i] != 右.字节[i]) return false;
        return true;
    }
};

inline bool 完整(const L1确定性摘要& 摘要) noexcept {
    return std::any_of(摘要.字节.begin(), 摘要.字节.end(),
        [](std::uint8_t 字节) noexcept { return 字节 != 0; });
}

inline constexpr std::uint32_t L1执行证据材料版本 = 1;
inline constexpr std::uint32_t L1请求意图格式版本 = 1;

struct L1领域意图凭证 final {
    std::uint8_t 请求意图组 = 0;
    std::uint32_t 请求意图格式版本 = L1请求意图格式版本;
    std::uint32_t 请求意图操作标签 = 0;
    L1确定性摘要 请求意图摘要;
    L1确定性摘要 领域结果计划摘要;
    friend bool operator==(const L1领域意图凭证&, const L1领域意图凭证&) = default;
};

inline constexpr std::uint32_t L1领域结果计划格式版本 = 1;
inline constexpr std::uint32_t L1领域结果见证摘要版本 = 1;
inline constexpr std::uint32_t L1确定性结果摘要版本E01 = 2;

struct L1领域结果字段计划 final {
    std::uint32_t 字段标签 = 0;
    写集本地键 写集本地键值;
    节点种类 期望对象种类 = 节点种类::普通;
    std::optional<值表示种类> 期望属性类型表示;
    friend bool operator==(const L1领域结果字段计划& 左, const L1领域结果字段计划& 右) {
        return 左.字段标签 == 右.字段标签 && 左.写集本地键值 == 右.写集本地键值
            && 左.期望对象种类 == 右.期望对象种类
            && 左.期望属性类型表示.has_value() == 右.期望属性类型表示.has_value()
            && (!左.期望属性类型表示 || *左.期望属性类型表示 == *右.期望属性类型表示);
    }
};
struct L1领域结果见证计划 final {
    std::uint32_t 格式版本 = L1领域结果计划格式版本;
    std::uint32_t 业务操作标签 = 0;
    std::uint32_t 领域合同版本 = 0;
    std::uint32_t 领域规则版本 = 0;
    std::uint8_t 入口变体 = 0;
    std::vector<L1领域结果字段计划> 字段组;
    friend bool operator==(const L1领域结果见证计划&, const L1领域结果见证计划&) = default;
};
struct L1领域结果见证字段 final {
    std::uint32_t 字段标签 = 0;
    写集本地键 写集本地键值;
    节点种类 对象种类 = 节点种类::普通;
    std::optional<值表示种类> 属性类型表示;
    稳定编码 本次新编码;
    friend bool operator==(const L1领域结果见证字段& 左, const L1领域结果见证字段& 右) {
        return 左.字段标签 == 右.字段标签 && 左.写集本地键值 == 右.写集本地键值
            && 左.对象种类 == 右.对象种类 && 左.本次新编码 == 右.本次新编码
            && 左.属性类型表示.has_value() == 右.属性类型表示.has_value()
            && (!左.属性类型表示 || *左.属性类型表示 == *右.属性类型表示);
    }
};
struct L1领域结果见证段 final {
    std::uint32_t 格式版本 = L1领域结果见证摘要版本;
    std::uint32_t 业务操作标签 = 0;
    std::uint32_t 领域合同版本 = 0;
    std::uint32_t 领域规则版本 = 0;
    std::uint8_t 入口变体 = 0;
    std::uint64_t 发布事实代次 = 0;
    写集幂等键 幂等键;
    std::vector<L1领域结果见证字段> 字段组;
    friend bool operator==(const L1领域结果见证段&, const L1领域结果见证段&) = default;
};
struct L1领域结果见证记录 final {
    写集幂等键 幂等键;
    std::uint64_t 发布事实代次 = 0;
    L1领域结果见证段 结果见证;
    L1确定性摘要 结果见证摘要;
    friend bool operator==(const L1领域结果见证记录&, const L1领域结果见证记录&) = default;
};

inline bool L1领域意图凭证完整(const L1领域意图凭证& 凭证) noexcept {
    return 凭证.请求意图组 <= 8
        && 凭证.请求意图格式版本 == L1请求意图格式版本
        && 凭证.请求意图操作标签 != 0 && 完整(凭证.请求意图摘要);
}

enum class L1执行证据模式 : std::uint8_t {
    空仓纯G0 = 1, 事实互证 = 2, 空仓领域首次 = 3
};

struct L1提供者身份凭证 final {
    稳定编码 稳定身份{0x4C31464200000001ULL};
    std::uint32_t 合同版本 = 2;
    std::uint32_t 规则版本 = 0;
    friend bool operator==(const L1提供者身份凭证&, const L1提供者身份凭证&) = default;
};

struct L1提供者版本见证 final {
    稳定编码 提供者身份;
    std::uint32_t 合同版本 = 0;
    std::uint32_t 规则版本 = 0;
    std::uint32_t 调用次数 = 0;
    friend bool operator==(const L1提供者版本见证&, const L1提供者版本见证&) = default;
};

struct L1写前事实见证 final {
    std::uint8_t 对象种类 = 0;
    稳定编码 身份;
    std::uint64_t 观察事实代次 = 0;
    L1确定性摘要 事实摘要;
    friend bool operator==(const L1写前事实见证&, const L1写前事实见证&) = default;
};

struct L1执行证据材料 final {
    std::uint32_t 材料版本 = L1执行证据材料版本;
    std::uint32_t 合同版本 = 2;
    std::uint32_t 摘要合同版本 = L1幂等摘要合同版本;
    L1执行证据模式 执行证据模式 = L1执行证据模式::事实互证;
    std::uint32_t 业务操作标签 = 0;
    写集幂等键 幂等键;
    std::uint64_t 共同事实截止代次 = 0;
    std::vector<L1提供者版本见证> 提供者版本组;
    std::vector<L1写前事实见证> 写前事实见证组;
    L1确定性摘要 规范化写集摘要;
    L1确定性摘要 材料摘要;
    friend bool operator==(const L1执行证据材料&, const L1执行证据材料&) = default;
};

enum class L1通用读回对象种类 : std::uint8_t { 节点 = 1, 关系 = 2, 值 = 3, 属性槽 = 4 };
enum class L1通用读回当前性 : std::uint8_t { 当前 = 1, 本次发布退出 = 2 };
enum class L1通用读回代次种类 : std::uint8_t { 固定 = 1, 本次发布 = 2 };

struct L1通用读回代次期望 final {
    L1通用读回代次种类 种类 = L1通用读回代次种类::固定;
    std::uint64_t 固定代次 = 0;
    friend bool operator==(const L1通用读回代次期望&, const L1通用读回代次期望&) = default;
};
struct L1节点读回期望 final {
    事实引用 身份;
    节点种类 种类 = 节点种类::普通;
    std::optional<值表示种类> 属性类型表示;
    L1通用读回代次期望 创建代次;
    L1通用读回当前性 当前性 = L1通用读回当前性::当前;
    friend bool operator==(const L1节点读回期望&, const L1节点读回期望&) = default;
};
struct L1关系读回期望 final {
    事实引用 身份;
    事实引用 源节点;
    事实引用 目标节点;
    事实引用 关系类型节点;
    std::int64_t 角色或顺序 = 0;
    L1通用读回代次期望 创建代次;
    L1通用读回当前性 当前性 = L1通用读回当前性::当前;
    friend bool operator==(const L1关系读回期望&, const L1关系读回期望&) = default;
};
struct L1值读回期望 final {
    事实引用 身份;
    事实引用 所属节点;
    事实引用 属性类型节点;
    原始值材料 材料;
    事实引用 来源节点;
    L1通用读回代次期望 创建代次;
    L1通用读回当前性 当前性 = L1通用读回当前性::当前;
    friend bool operator==(const L1值读回期望&, const L1值读回期望&) = default;
};
struct L1属性槽读回期望 final {
    事实引用 所属节点;
    事实引用 属性类型节点;
    事实引用 当前值;
    friend bool operator==(const L1属性槽读回期望&, const L1属性槽读回期望&) = default;
};
using L1通用发布后读回期望 = std::variant<L1节点读回期望, L1关系读回期望,
    L1值读回期望, L1属性槽读回期望>;
struct L1通用发布后读回计划 final {
    std::uint32_t 计划版本 = L1通用发布后读回计划版本;
    std::vector<L1通用发布后读回期望> 项目组;
    friend bool operator==(const L1通用发布后读回计划&, const L1通用发布后读回计划&) = default;
};

struct L1写集请求 final {
    std::uint32_t 合同版本 = 2;
    std::uint32_t 摘要合同版本 = L1幂等摘要合同版本;
    std::uint64_t 期望事实代次 = 0;
    写集幂等键 幂等键;
    L1领域意图凭证 领域意图凭证;
    L1确定性摘要 请求意图摘要;
    L1确定性摘要 执行证据摘要;
    L1通用发布后读回计划 发布后读回计划;
    std::vector<节点新建项> 节点;
    std::vector<关系新建项> 关系;
    std::vector<值新建项> 值;
    std::vector<属性槽变更项> 属性槽变更;
    std::vector<稳定编码> 退出事实;
    std::optional<L1执行证据材料> 执行证据材料;
    std::optional<L1领域结果见证计划> 领域结果见证计划;
    friend bool operator==(const L1写集请求&, const L1写集请求&) = default;
};

struct L1属性读取副本 final {
    稳定编码 节点;
    稳定编码 属性类型节点;
    稳定编码 当前值;
    friend bool operator==(const L1属性读取副本&, const L1属性读取副本&) = default;
};

enum class L1通用发布后读回状态 : std::uint8_t { 成功 = 1, 未执行 = 2, 内部不一致 = 3 };
using L1通用发布后读回副本 = std::variant<节点事实, 关系事实, 值事实, L1属性读取副本>;
struct L1通用发布后读回项目 final {
    L1通用读回对象种类 种类 = L1通用读回对象种类::节点;
    L1通用发布后读回副本 副本;
    friend bool operator==(const L1通用发布后读回项目&, const L1通用发布后读回项目&) = default;
};
struct L1通用发布后读回结果 final {
    L1通用发布后读回状态 状态 = L1通用发布后读回状态::未执行;
    std::uint64_t 发布事实代次 = 0;
    std::vector<L1通用发布后读回项目> 项目组;
    friend bool operator==(const L1通用发布后读回结果&, const L1通用发布后读回结果&) = default;
};

struct L1失败见证身份 final {
    写集幂等键 幂等键;
    std::uint64_t 审计事件序号 = 0;
    friend bool operator==(const L1失败见证身份&, const L1失败见证身份&) = default;
};
enum class L1发布后读回失败原因 : std::uint8_t {
    映射缺失 = 1, 映射重复 = 2, 对象缺失 = 3, 对象种类不符 = 4,
    中性字段不符 = 5, 生命周期不符 = 6, 发布代次不符 = 7,
    计划覆盖不符 = 8, 内部异常 = 9
};
struct L1发布后读回失败见证 final {
    L1失败见证身份 身份;
    std::uint64_t 发布事实代次 = 0;
    std::uint32_t 失败项目索引 = 0;
    L1通用读回对象种类 对象种类 = L1通用读回对象种类::节点;
    L1发布后读回失败原因 原因 = L1发布后读回失败原因::内部异常;
    std::uint64_t 计划证据摘要 = 0;
    std::uint64_t 实际证据摘要 = 0;
    friend bool operator==(const L1发布后读回失败见证&, const L1发布后读回失败见证&) = default;
};

enum class L1写入状态 : std::uint8_t {
    成功 = 1, 精确重复 = 2, 入口拒绝 = 3, 未找到 = 4, 已退出 = 5,
    事实代次漂移 = 6, 幂等冲突 = 7, 资源失败 = 8, 内部不一致 = 9
};
struct L1写入结果 final {
    L1写入状态 状态 = L1写入状态::入口拒绝;
    std::uint64_t 事实代次 = 0;
    std::vector<std::pair<写集本地键, 稳定编码>> 新编码映射;
    L1通用发布后读回结果 发布后读回;
    std::optional<L1失败见证身份> 失败见证身份;
    std::uint32_t 确定性结果摘要版本 = L1确定性结果摘要版本;
    L1确定性摘要 确定性结果摘要;
    std::optional<L1领域结果见证段> 领域结果见证;
    friend bool operator==(const L1写入结果&, const L1写入结果&) = default;
};

enum class L1读取状态 : std::uint8_t {
    成功 = 1, 入口拒绝 = 2, 未找到 = 3, 已退出 = 4,
    属性未设置 = 5, 事实代次漂移 = 6, 资源失败 = 7, 内部不一致 = 8,
    许可拒绝 = 9
};
using L1事实副本 = std::variant<节点事实, 关系事实, 值事实>;
struct L1读取结果 final {
    L1读取状态 状态 = L1读取状态::入口拒绝;
    std::uint64_t 读取事实代次 = 0;
    std::optional<L1事实副本> 事实;
};
struct L1属性读取结果 final {
    L1读取状态 状态 = L1读取状态::入口拒绝;
    std::uint64_t 读取事实代次 = 0;
    std::optional<L1属性读取副本> 属性;
};
struct L1历史事实副本 final {
    稳定编码 查询编码;
    L1事实副本 事实;
    bool 当前有效 = false;
};
struct L1历史读取结果 final {
    L1读取状态 状态 = L1读取状态::入口拒绝;
    std::optional<L1历史事实副本> 历史;
};

enum class L1审计事件 : std::uint8_t {
    提交成功 = 1, 精确重复 = 2, 幂等冲突 = 3, 候选撤销 = 4,
    恢复候选建立 = 5, 恢复候选撤销 = 6, 恢复发布 = 7, 恢复拒绝 = 8,
    发布后读回失败隔离 = 9
};
struct L1审计记录 final {
    写集幂等键 幂等键;
    std::uint64_t 事件序号 = 0;
    L1审计事件 事件 = L1审计事件::候选撤销;
    L1写入状态 结果状态 = L1写入状态::入口拒绝;
    std::uint64_t 事实代次 = 0;
    std::vector<std::pair<写集本地键, 稳定编码>> 新编码映射;
    std::uint32_t 摘要合同版本 = L1幂等摘要合同版本;
    L1确定性摘要 请求意图摘要;
    L1确定性摘要 首次执行证据摘要;
    L1确定性摘要 确定性结果摘要;
    std::optional<L1发布后读回失败见证> 失败见证;
    std::optional<L1执行证据材料> 首次执行证据材料;
    std::uint32_t 确定性结果摘要版本 = L1确定性结果摘要版本;
    L1确定性摘要 领域结果见证摘要;
    std::optional<L1领域结果见证段> 首次领域结果见证;
    friend bool operator==(const L1审计记录&, const L1审计记录&) = default;
};
struct L1审计读取结果 final {
    L1读取状态 状态 = L1读取状态::入口拒绝;
    写集幂等键 幂等键;
    std::vector<L1审计记录> 记录组;
};
struct L1完整快照 final {
    std::uint64_t 事实代次 = 0;
    std::vector<节点事实> 当前节点;
    std::vector<关系事实> 当前关系;
    std::vector<值事实> 当前值;
    std::vector<稳定编码> 永久占用编码;
    friend bool operator==(const L1完整快照&, const L1完整快照&) = default;
};
struct L1完整快照结果 final {
    L1读取状态 状态 = L1读取状态::入口拒绝;
    std::optional<L1完整快照> 快照;
};
struct L1事实代次读取结果 final {
    L1读取状态 状态 = L1读取状态::入口拒绝;
    std::uint64_t 事实截止代次 = 0;
    // 诊断责任：无适用错误分支；默认比较只比较纯值结果。
    friend bool operator==(const L1事实代次读取结果&,
        const L1事实代次读取结果&) = default;
};
struct L1幂等账记录 final {
    写集幂等键 幂等键;
    std::uint32_t 摘要合同版本 = L1幂等摘要合同版本;
    L1确定性摘要 请求意图摘要;
    L1确定性摘要 首次执行证据摘要;
    std::uint64_t 首次发布事实代次 = 0;
    L1写集请求 首次规范化写集;
    std::vector<std::pair<写集本地键, 稳定编码>> 首次新编码映射;
    L1通用发布后读回结果 首次完整读回;
    L1确定性摘要 确定性结果摘要;
    std::optional<L1失败见证身份> 发布后失败见证;
    std::optional<L1执行证据材料> 首次执行证据材料;
    std::uint32_t 确定性结果摘要版本 = L1确定性结果摘要版本;
    L1确定性摘要 领域结果见证摘要;
    std::optional<L1领域结果见证段> 首次领域结果见证;
    friend bool operator==(const L1幂等账记录&, const L1幂等账记录&) = default;
};
struct L1恢复材料 final {
    L1完整快照 当前快照;
    std::vector<节点事实> 历史节点;
    std::vector<关系事实> 历史关系;
    std::vector<值事实> 历史值;
    std::vector<L1审计记录> 审计记录;
    std::vector<L1幂等账记录> 幂等账;
    std::vector<L1发布后读回失败见证> 发布后读回失败见证组;
    std::optional<L1失败见证身份> 当前隔离见证身份;
    std::vector<L1领域结果见证记录> 领域结果见证记录组;
    friend bool operator==(const L1恢复材料&, const L1恢复材料&) = default;
};
enum class L1恢复材料导出状态 : std::uint8_t {
    成功 = 1, 入口拒绝 = 2, 资源失败 = 3, 内部不一致 = 4
};
struct L1恢复材料导出结果 final {
    L1恢复材料导出状态 状态 = L1恢复材料导出状态::入口拒绝;
    std::uint64_t 事实代次 = 0;
    bool 当前隔离 = false;
    std::optional<L1恢复材料> 材料;
};
enum class L1恢复状态 : std::uint8_t {
    候选已建立 = 1, 恢复已发布 = 2, 候选已撤销 = 3, 无候选 = 4,
    入口拒绝 = 5, 材料不完整 = 6, 事实代次漂移 = 7,
    资源失败 = 8, 内部不一致 = 9
};
struct L1恢复结果 final {
    L1恢复状态 状态 = L1恢复状态::入口拒绝;
    std::uint64_t 事实代次 = 0;
};

inline bool 有效(写集本地键 键) noexcept { return 键.值 != 0; }
inline bool 有效(写集幂等键 键) noexcept { return 键.值 != 0; }

inline bool 原始材料完整(const 原始值材料& 材料) noexcept {
    return std::visit([](const auto& 值) noexcept -> bool {
        using T = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<T, std::int64_t>) return true;
        else if constexpr (std::is_same_v<T, 独立材料引用>) return 有效(值.编码);
        else return !值.empty();
    }, 材料);
}

inline bool 属性排序唯一(const std::vector<属性槽>& 槽组) noexcept {
    for (std::size_t i = 0; i < 槽组.size(); ++i) {
        if (!有效(槽组[i].属性类型节点) || !有效(槽组[i].当前值)) return false;
        if (i != 0 && !(槽组[i - 1].属性类型节点 < 槽组[i].属性类型节点)) return false;
    }
    return true;
}

namespace L1确定性编码内部 {
inline constexpr std::array<std::uint32_t, 64> 常量 = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};
inline std::uint32_t 右转(std::uint32_t 值, unsigned 位) noexcept { return (值 >> 位) | (值 << (32U - 位)); }
inline L1确定性摘要 哈希(std::vector<std::uint8_t> 字节) {
    const std::uint64_t 位数 = static_cast<std::uint64_t>(字节.size()) * 8ULL;
    字节.push_back(0x80);
    while ((字节.size() % 64) != 56) 字节.push_back(0);
    for (int i = 7; i >= 0; --i) 字节.push_back(static_cast<std::uint8_t>(位数 >> (i * 8)));
    std::array<std::uint32_t, 8> h = {0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
        0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
    for (std::size_t 偏移 = 0; 偏移 < 字节.size(); 偏移 += 64) {
        std::array<std::uint32_t, 64> w{};
        for (std::size_t i = 0; i < 16; ++i) w[i] =
            (static_cast<std::uint32_t>(字节[偏移 + i * 4]) << 24)
            | (static_cast<std::uint32_t>(字节[偏移 + i * 4 + 1]) << 16)
            | (static_cast<std::uint32_t>(字节[偏移 + i * 4 + 2]) << 8)
            | static_cast<std::uint32_t>(字节[偏移 + i * 4 + 3]);
        for (std::size_t i = 16; i < 64; ++i) {
            const auto s0 = 右转(w[i-15],7) ^ 右转(w[i-15],18) ^ (w[i-15] >> 3);
            const auto s1 = 右转(w[i-2],17) ^ 右转(w[i-2],19) ^ (w[i-2] >> 10);
            w[i] = w[i-16] + s0 + w[i-7] + s1;
        }
        auto a=h[0],b=h[1],c=h[2],d=h[3],e=h[4],f=h[5],g=h[6],hh=h[7];
        for (std::size_t i = 0; i < 64; ++i) {
            const auto s1=右转(e,6)^右转(e,11)^右转(e,25);
            const auto ch=(e&f)^((~e)&g);
            const auto t1=hh+s1+ch+常量[i]+w[i];
            const auto s0=右转(a,2)^右转(a,13)^右转(a,22);
            const auto maj=(a&b)^(a&c)^(b&c);
            const auto t2=s0+maj;
            hh=g; g=f; f=e; e=d+t1; d=c; c=b; b=a; a=t1+t2;
        }
        h[0]+=a;h[1]+=b;h[2]+=c;h[3]+=d;h[4]+=e;h[5]+=f;h[6]+=g;h[7]+=hh;
    }
    L1确定性摘要 结果;
    for (std::size_t i=0;i<8;++i) for (std::size_t j=0;j<4;++j)
        结果.字节[i*4+j]=static_cast<std::uint8_t>(h[i]>>(24-j*8));
    return 结果;
}
inline void 写U8(std::vector<std::uint8_t>& 出, std::uint8_t 值) { 出.push_back(值); }
inline void 写U16(std::vector<std::uint8_t>& 出, std::uint16_t 值) { for (int i=1;i>=0;--i) 出.push_back(static_cast<std::uint8_t>(值>>(i*8))); }
inline void 写U32(std::vector<std::uint8_t>& 出, std::uint32_t 值) { for (int i=3;i>=0;--i) 出.push_back(static_cast<std::uint8_t>(值>>(i*8))); }
inline void 写U64(std::vector<std::uint8_t>& 出, std::uint64_t 值) { for (int i=7;i>=0;--i) 出.push_back(static_cast<std::uint8_t>(值>>(i*8))); }
inline void 写I64(std::vector<std::uint8_t>& 出, std::int64_t 值) { 写U64(出, static_cast<std::uint64_t>(值)); }
inline void 写稳定编码(std::vector<std::uint8_t>& 出, 稳定编码 值) { 写U64(出, 值.值); }
inline void 写字段头(std::vector<std::uint8_t>& 出, std::uint16_t 编号,
    std::uint8_t 类型, std::uint32_t 长度) {
    写U16(出, 编号); 写U8(出, 类型); 写U32(出, 长度);
}
inline void 写引用(std::vector<std::uint8_t>& 出, const 事实引用& 引用) {
    std::visit([&](const auto& 值) { using T=std::decay_t<decltype(值)>;
        写U8(出, std::is_same_v<T,稳定编码> ? 1 : 2); 写U64(出, 值.值); }, 引用);
}
inline void 写材料(std::vector<std::uint8_t>& 出, const 原始值材料& 材料) {
    std::visit([&](const auto& 值) { using T=std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<T,std::int64_t>) { 写U8(出,1); 写U64(出,static_cast<std::uint64_t>(值)); }
        else if constexpr (std::is_same_v<T,std::vector<std::int64_t>>) { 写U8(出,2); 写U64(出,值.size()); for(auto v:值) 写U64(出,static_cast<std::uint64_t>(v)); }
        else if constexpr (std::is_same_v<T,std::vector<std::uint64_t>>) { 写U8(出,3); 写U64(出,值.size()); for(auto v:值) 写U64(出,v); }
        else { 写U8(出,4); 写U64(出,值.编码.值); } }, 材料);
}
inline void 写写集(std::vector<std::uint8_t>& 出, const L1写集请求& 请求) {
    写U32(出,请求.合同版本); 写U32(出,请求.摘要合同版本);
    写U64(出,请求.幂等键.值); 写U64(出,请求.期望事实代次);
    写U64(出,请求.节点.size()); for(const auto& 项:请求.节点){写U32(出,项.本地键.值);写U8(出,static_cast<std::uint8_t>(项.种类));写U8(出, 项.属性类型表示 ? 1 : 0);if(项.属性类型表示)写U8(出,static_cast<std::uint8_t>(*项.属性类型表示));}
    写U64(出,请求.关系.size()); for(const auto& 项:请求.关系){写U32(出,项.本地键.值);写引用(出,项.源节点);写引用(出,项.目标节点);写引用(出,项.关系类型节点);写U64(出,static_cast<std::uint64_t>(项.角色或顺序));}
    写U64(出,请求.值.size()); for(const auto& 项:请求.值){写U32(出,项.本地键.值);写引用(出,项.所属节点);写引用(出,项.属性类型节点);写材料(出,项.材料);写引用(出,项.来源节点);}
    写U64(出,请求.属性槽变更.size()); for(const auto& 项:请求.属性槽变更){写引用(出,项.所属节点);写引用(出,项.属性类型节点);写U32(出,项.新当前值.值);}
    写U64(出,请求.退出事实.size()); for(const auto 编码:请求.退出事实)写U64(出,编码.值);
}
inline void 写计划(std::vector<std::uint8_t>& 出, const L1通用发布后读回计划& 计划) {
    写U32(出,计划.计划版本); 写U64(出,计划.项目组.size());
    for(const auto& 项:计划.项目组) std::visit([&](const auto& 期望){using T=std::decay_t<decltype(期望)>;
        if constexpr(std::is_same_v<T,L1节点读回期望>){写U8(出,1);写引用(出,期望.身份);写U8(出,static_cast<std::uint8_t>(期望.种类));写U8(出, 期望.属性类型表示 ? 1 : 0);if(期望.属性类型表示)写U8(出,static_cast<std::uint8_t>(*期望.属性类型表示));写U8(出,static_cast<std::uint8_t>(期望.创建代次.种类));写U64(出,期望.创建代次.固定代次);写U8(出,static_cast<std::uint8_t>(期望.当前性));}
        else if constexpr(std::is_same_v<T,L1关系读回期望>){写U8(出,2);写引用(出,期望.身份);写引用(出,期望.源节点);写引用(出,期望.目标节点);写引用(出,期望.关系类型节点);写U64(出,static_cast<std::uint64_t>(期望.角色或顺序));写U8(出,static_cast<std::uint8_t>(期望.创建代次.种类));写U64(出,期望.创建代次.固定代次);写U8(出,static_cast<std::uint8_t>(期望.当前性));}
        else if constexpr(std::is_same_v<T,L1值读回期望>){写U8(出,3);写引用(出,期望.身份);写引用(出,期望.所属节点);写引用(出,期望.属性类型节点);写材料(出,期望.材料);写引用(出,期望.来源节点);写U8(出,static_cast<std::uint8_t>(期望.创建代次.种类));写U64(出,期望.创建代次.固定代次);写U8(出,static_cast<std::uint8_t>(期望.当前性));}
        else {写U8(出,4);写引用(出,期望.所属节点);写引用(出,期望.属性类型节点);写引用(出,期望.当前值);}
    },项);
}
inline void 写执行证据材料正文(std::vector<std::uint8_t>& 出, const L1执行证据材料& 材料) {
    写U32(出, 材料.材料版本);
    写U32(出, 材料.合同版本);
    写U32(出, 材料.摘要合同版本);
    写U8(出, static_cast<std::uint8_t>(材料.执行证据模式));
    写U32(出, 材料.业务操作标签);
    写U64(出, 材料.幂等键.值);
    写U64(出, 材料.共同事实截止代次);
    写U64(出, 材料.提供者版本组.size());
    for (const auto& 见证 : 材料.提供者版本组) {
        写U64(出, 见证.提供者身份.值);
        写U32(出, 见证.合同版本);
        写U32(出, 见证.规则版本);
        写U32(出, 见证.调用次数);
    }
    写U64(出, 材料.写前事实见证组.size());
    for (const auto& 见证 : 材料.写前事实见证组) {
        写U8(出, 见证.对象种类);
        写U64(出, 见证.身份.值);
        写U64(出, 见证.观察事实代次);
        出.insert(出.end(), 见证.事实摘要.字节.begin(), 见证.事实摘要.字节.end());
    }
    出.insert(出.end(), 材料.规范化写集摘要.字节.begin(), 材料.规范化写集摘要.字节.end());
}
}

inline L1确定性摘要 形成L1规范化写集摘要(const L1写集请求& 请求) {
    L1写集请求 规范化 = 请求;
    std::sort(规范化.节点.begin(), 规范化.节点.end(),
        [](const auto& 左, const auto& 右) { return 左.本地键 < 右.本地键; });
    std::sort(规范化.关系.begin(), 规范化.关系.end(),
        [](const auto& 左, const auto& 右) { return 左.本地键 < 右.本地键; });
    std::sort(规范化.值.begin(), 规范化.值.end(),
        [](const auto& 左, const auto& 右) { return 左.本地键 < 右.本地键; });
    const auto 引用排序键 = [](const 事实引用& 引用) {
        return std::visit([](const auto& 值) -> std::uint64_t {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, 稳定编码>) return 值.值;
            else return (1ULL << 63) | 值.值;
        }, 引用);
    };
    std::sort(规范化.属性槽变更.begin(), 规范化.属性槽变更.end(),
        [&](const auto& 左, const auto& 右) {
            const auto 左所属 = 引用排序键(左.所属节点);
            const auto 右所属 = 引用排序键(右.所属节点);
            if (左所属 != 右所属) return 左所属 < 右所属;
            const auto 左类型 = 引用排序键(左.属性类型节点);
            const auto 右类型 = 引用排序键(右.属性类型节点);
            if (左类型 != 右类型) return 左类型 < 右类型;
            return 左.新当前值 < 右.新当前值;
        });
    std::sort(规范化.退出事实.begin(), 规范化.退出事实.end());
    std::vector<std::uint8_t> 编码{'H','Z','Y','-','L','1','W','S'};
    L1确定性编码内部::写U32(编码, 1);
    L1确定性编码内部::写写集(编码, 规范化);
    return L1确定性编码内部::哈希(std::move(编码));
}

inline L1确定性摘要 形成L1执行证据材料摘要(const L1执行证据材料& 材料) {
    std::vector<std::uint8_t> 编码{'H','Z','Y','-','L','1','E','M'};
    L1确定性编码内部::写执行证据材料正文(编码, 材料);
    return L1确定性编码内部::哈希(std::move(编码));
}

inline L1确定性摘要 形成L1写前事实摘要(const L1事实副本& 副本) {
    std::vector<std::uint8_t> 编码{'H','Z','Y','-','L','1','P','F'};
    std::visit([&](const auto& 事实) {
        using T = std::decay_t<decltype(事实)>;
        if constexpr (std::is_same_v<T, 节点事实>) {
            L1确定性编码内部::写U8(编码, 1);
            L1确定性编码内部::写U64(编码, 事实.编码.值);
            L1确定性编码内部::写U8(编码, static_cast<std::uint8_t>(事实.种类));
            L1确定性编码内部::写U8(编码, 事实.属性类型表示 ? 1 : 0);
            if (事实.属性类型表示) L1确定性编码内部::写U8(编码,
                static_cast<std::uint8_t>(*事实.属性类型表示));
            L1确定性编码内部::写U64(编码, 事实.创建事实代次);
            L1确定性编码内部::写U64(编码, 事实.退出事实代次.value_or(0));
            L1确定性编码内部::写U64(编码, 事实.当前属性.size());
            for (const auto& 槽 : 事实.当前属性) {
                L1确定性编码内部::写U64(编码, 槽.属性类型节点.值);
                L1确定性编码内部::写U64(编码, 槽.当前值.值);
            }
        } else if constexpr (std::is_same_v<T, 关系事实>) {
            L1确定性编码内部::写U8(编码, 2);
            L1确定性编码内部::写U64(编码, 事实.编码.值);
            L1确定性编码内部::写U64(编码, 事实.源节点.值);
            L1确定性编码内部::写U64(编码, 事实.目标节点.值);
            L1确定性编码内部::写U64(编码, 事实.关系类型节点.值);
            L1确定性编码内部::写U64(编码, static_cast<std::uint64_t>(事实.角色或顺序));
            L1确定性编码内部::写U64(编码, 事实.创建事实代次);
            L1确定性编码内部::写U64(编码, 事实.退出事实代次.value_or(0));
        } else {
            L1确定性编码内部::写U8(编码, 3);
            L1确定性编码内部::写U64(编码, 事实.编码.值);
            L1确定性编码内部::写U64(编码, 事实.所属节点.值);
            L1确定性编码内部::写U64(编码, 事实.属性类型节点.值);
            L1确定性编码内部::写材料(编码, 事实.材料);
            L1确定性编码内部::写U64(编码, 事实.来源节点.值);
            L1确定性编码内部::写U64(编码, 事实.创建事实代次);
            L1确定性编码内部::写U64(编码, 事实.退出事实代次.value_or(0));
        }
    }, 副本);
    return L1确定性编码内部::哈希(std::move(编码));
}

inline L1确定性摘要 形成L1领域结果计划摘要(const L1领域结果见证计划& 计划) {
    std::vector<std::uint8_t> 编码{'H','Z','Y','-','L','1','-','R','P'};
    L1确定性编码内部::写U32(编码, 计划.格式版本);
    L1确定性编码内部::写U32(编码, 计划.业务操作标签);
    L1确定性编码内部::写U32(编码, 计划.领域合同版本);
    L1确定性编码内部::写U32(编码, 计划.领域规则版本);
    L1确定性编码内部::写U8(编码, 计划.入口变体);
    L1确定性编码内部::写U64(编码, 计划.字段组.size());
    for (const auto& 字段 : 计划.字段组) {
        L1确定性编码内部::写U32(编码, 字段.字段标签);
        L1确定性编码内部::写U32(编码, 字段.写集本地键值.值);
        L1确定性编码内部::写U8(编码, static_cast<std::uint8_t>(字段.期望对象种类));
        L1确定性编码内部::写U8(编码, 字段.期望属性类型表示 ? 1 : 0);
        if (字段.期望属性类型表示) L1确定性编码内部::写U8(编码,
            static_cast<std::uint8_t>(*字段.期望属性类型表示));
    }
    return L1确定性编码内部::哈希(std::move(编码));
}

inline L1确定性摘要 形成L1领域结果见证摘要(const L1领域结果见证段& 见证) {
    std::vector<std::uint8_t> 编码{'H','Z','Y','-','L','1','-','R','W'};
    L1确定性编码内部::写U32(编码, 见证.格式版本);
    L1确定性编码内部::写U32(编码, 见证.业务操作标签);
    L1确定性编码内部::写U32(编码, 见证.领域合同版本);
    L1确定性编码内部::写U32(编码, 见证.领域规则版本);
    L1确定性编码内部::写U8(编码, 见证.入口变体);
    L1确定性编码内部::写U64(编码, 见证.发布事实代次);
    L1确定性编码内部::写U64(编码, 见证.幂等键.值);
    L1确定性编码内部::写U64(编码, 见证.字段组.size());
    for (const auto& 字段 : 见证.字段组) {
        L1确定性编码内部::写U32(编码, 字段.字段标签);
        L1确定性编码内部::写U32(编码, 字段.写集本地键值.值);
        L1确定性编码内部::写U8(编码, static_cast<std::uint8_t>(字段.对象种类));
        L1确定性编码内部::写U8(编码, 字段.属性类型表示 ? 1 : 0);
        if (字段.属性类型表示) L1确定性编码内部::写U8(编码,
            static_cast<std::uint8_t>(*字段.属性类型表示));
        L1确定性编码内部::写U64(编码, 字段.本次新编码.值);
    }
    return L1确定性编码内部::哈希(std::move(编码));
}

inline bool L1领域结果计划完整(const L1领域结果见证计划& 计划,
    const L1写集请求& 请求, std::uint32_t 操作标签) {
    if (计划.格式版本 != L1领域结果计划格式版本 || 计划.业务操作标签 != 操作标签
        || 计划.领域合同版本 == 0 || 计划.入口变体 == 0 || 计划.字段组.empty()) return false;
    for (std::size_t i = 0; i < 计划.字段组.size(); ++i) {
        const auto& 字段 = 计划.字段组[i];
        if (字段.字段标签 == 0 || !有效(字段.写集本地键值)
            || (i != 0 && 计划.字段组[i - 1].字段标签 >= 字段.字段标签)) return false;
        const auto 节点 = std::find_if(请求.节点.begin(), 请求.节点.end(), [&](const auto& 项) {
            return 项.本地键 == 字段.写集本地键值;
        });
        if (节点 == 请求.节点.end() || 节点->种类 != 字段.期望对象种类
            || 节点->属性类型表示 != 字段.期望属性类型表示) return false;
        for (std::size_t j = 0; j < i; ++j)
            if (计划.字段组[j].写集本地键值 == 字段.写集本地键值) return false;
    }
    return 请求.领域意图凭证.领域结果计划摘要 == 形成L1领域结果计划摘要(计划);
}

inline std::optional<L1领域结果见证段> 形成L1领域结果见证(
    const L1领域结果见证计划& 计划, const L1写集请求& 请求,
    const std::vector<std::pair<写集本地键, 稳定编码>>& 映射, std::uint64_t 发布代次) {
    if (!L1领域结果计划完整(计划, 请求, 计划.业务操作标签)
        || 发布代次 == 0 || !有效(请求.幂等键)) return std::nullopt;
    L1领域结果见证段 结果{L1领域结果见证摘要版本, 计划.业务操作标签,
        计划.领域合同版本, 计划.领域规则版本, 计划.入口变体,
        发布代次, 请求.幂等键, {}};
    for (const auto& 字段 : 计划.字段组) {
        std::optional<稳定编码> 编码;
        for (const auto& [键, 值] : 映射) if (键 == 字段.写集本地键值) {
            if (编码) return std::nullopt;
            编码 = 值;
        }
        if (!编码 || !有效(*编码)) return std::nullopt;
        结果.字段组.push_back({字段.字段标签, 字段.写集本地键值,
            字段.期望对象种类, 字段.期望属性类型表示, *编码});
    }
    return 结果;
}

inline bool L1执行证据材料排序唯一(const L1执行证据材料& 材料) noexcept {
    for (std::size_t i = 0; i < 材料.提供者版本组.size(); ++i) {
        const auto& 当前 = 材料.提供者版本组[i];
        if (!有效(当前.提供者身份) || 当前.合同版本 == 0 || 当前.调用次数 == 0) return false;
        if (i != 0 && !(材料.提供者版本组[i - 1].提供者身份 < 当前.提供者身份)) return false;
    }
    for (std::size_t i = 0; i < 材料.写前事实见证组.size(); ++i) {
        const auto& 当前 = 材料.写前事实见证组[i];
        if (当前.对象种类 == 0 || !有效(当前.身份) || 当前.观察事实代次 == 0
            || !完整(当前.事实摘要)) return false;
        if (i != 0) {
            const auto& 前 = 材料.写前事实见证组[i - 1];
            if (前.对象种类 > 当前.对象种类
                || (前.对象种类 == 当前.对象种类 && !(前.身份 < 当前.身份))) return false;
        }
    }
    return true;
}

inline bool L1执行证据材料基本完整(const L1执行证据材料& 材料,
    const L1写集请求& 请求) {
    if (材料.材料版本 != L1执行证据材料版本 || 材料.合同版本 != 请求.合同版本
        || 材料.摘要合同版本 != 请求.摘要合同版本 || 材料.业务操作标签 == 0
        || 材料.幂等键 != 请求.幂等键 || !完整(材料.规范化写集摘要)
        || 材料.规范化写集摘要 != 形成L1规范化写集摘要(请求)
        || 材料.材料摘要 != 形成L1执行证据材料摘要(材料)
        || !L1执行证据材料排序唯一(材料)) return false;
    if (材料.执行证据模式 == L1执行证据模式::空仓纯G0)
        return 请求.期望事实代次 == 0 && 材料.共同事实截止代次 == 0
            && 材料.提供者版本组.empty() && 材料.写前事实见证组.empty();
    if (材料.执行证据模式 == L1执行证据模式::空仓领域首次)
        return 请求.期望事实代次 == 0 && 材料.共同事实截止代次 == 0
            && 材料.提供者版本组.empty() && 材料.写前事实见证组.empty()
            && 请求.领域意图凭证.请求意图组 != 0;
    if (材料.执行证据模式 != L1执行证据模式::事实互证
        || 材料.写前事实见证组.empty()) return false;
    return std::all_of(材料.写前事实见证组.begin(), 材料.写前事实见证组.end(),
        [&](const auto& 见证) { return 见证.观察事实代次 == 材料.共同事实截止代次; });
}

inline L1确定性摘要 形成L1写集摘要(std::uint32_t 操作标签,
    const L1写集请求& 请求, bool 执行证据) {
    std::vector<std::uint8_t> 编码{'H','Z','Y','-','L','1','I','D'};
    L1确定性编码内部::写U32(编码,1);
    L1确定性编码内部::写U32(编码,操作标签);
    const auto 模式 = 执行证据 && 请求.执行证据材料
        ? static_cast<std::uint8_t>(请求.执行证据材料->执行证据模式)
        : static_cast<std::uint8_t>(1);
    L1确定性编码内部::写U8(编码, 执行证据 ? 模式 : 1);
    if(执行证据) {
        L1确定性编码内部::写U8(编码, 请求.领域意图凭证.请求意图组);
        L1确定性编码内部::写U32(编码, 请求.领域意图凭证.请求意图格式版本);
        L1确定性编码内部::写U32(编码, 请求.领域意图凭证.请求意图操作标签);
        编码.insert(编码.end(),请求.请求意图摘要.字节.begin(),请求.请求意图摘要.字节.end());
        const auto G0 = 形成L1规范化写集摘要(请求);
        编码.insert(编码.end(), G0.字节.begin(), G0.字节.end());
    }
    L1确定性编码内部::写写集(编码,请求);
    if(执行证据) {
        L1确定性编码内部::写计划(编码,请求.发布后读回计划);
        if (请求.执行证据材料)
            编码.insert(编码.end(), 请求.执行证据材料->材料摘要.字节.begin(),
                请求.执行证据材料->材料摘要.字节.end());
    }
    return L1确定性编码内部::哈希(std::move(编码));
}

inline bool 准备L1写集v2(L1写集请求& 请求, std::uint32_t 操作标签) {
    请求.合同版本 = 2;
    请求.摘要合同版本 = L1幂等摘要合同版本;
    请求.发布后读回计划 = {};
    const L1通用读回代次期望 本次{L1通用读回代次种类::本次发布,0};
    for(const auto& 项:请求.节点) 请求.发布后读回计划.项目组.emplace_back(
        L1节点读回期望{项.本地键,项.种类,项.属性类型表示,本次,L1通用读回当前性::当前});
    for(const auto& 项:请求.关系) 请求.发布后读回计划.项目组.emplace_back(
        L1关系读回期望{项.本地键,项.源节点,项.目标节点,项.关系类型节点,项.角色或顺序,本次,L1通用读回当前性::当前});
    for(const auto& 项:请求.值) 请求.发布后读回计划.项目组.emplace_back(
        L1值读回期望{项.本地键,项.所属节点,项.属性类型节点,项.材料,项.来源节点,本次,L1通用读回当前性::当前});
    for(const auto& 项:请求.属性槽变更) 请求.发布后读回计划.项目组.emplace_back(
        L1属性槽读回期望{项.所属节点,项.属性类型节点,项.新当前值});
    if (!L1领域意图凭证完整(请求.领域意图凭证)
        || 请求.领域意图凭证.请求意图组 == 0) {
        请求.领域意图凭证 = {0, L1请求意图格式版本, 操作标签,
            形成L1写集摘要(操作标签, 请求, false)};
    }
    if (!L1领域意图凭证完整(请求.领域意图凭证)
        || 请求.领域意图凭证.请求意图操作标签 != 操作标签) return false;
    请求.请求意图摘要 = 请求.领域意图凭证.请求意图摘要;
    请求.执行证据摘要 = 请求.执行证据材料
        ? 形成L1写集摘要(操作标签, 请求, true) : L1确定性摘要{};
    return true;
}

inline bool 完成L1执行证据(L1写集请求& 请求, L1执行证据材料 材料) {
    材料.规范化写集摘要 = 形成L1规范化写集摘要(请求);
    材料.材料摘要 = {};
    材料.材料摘要 = 形成L1执行证据材料摘要(材料);
    请求.执行证据材料 = std::move(材料);
    if (!L1执行证据材料基本完整(*请求.执行证据材料, 请求)) return false;
    请求.执行证据摘要 = 形成L1写集摘要(
        请求.执行证据材料->业务操作标签, 请求, true);
    return 完整(请求.执行证据摘要);
}

inline L1确定性摘要 形成L1确定性结果摘要(const L1写集请求& 请求,
    const L1写入结果& 结果, std::uint32_t 版本 = L1确定性结果摘要版本,
    L1确定性摘要 领域结果见证摘要 = {}) {
    std::vector<std::uint8_t> 编码{'H','Z','Y','-','L','1','R','S'};
    L1确定性编码内部::写U32(编码,版本);
    L1确定性编码内部::写U32(编码,请求.合同版本);
    L1确定性编码内部::写U32(编码,请求.摘要合同版本);
    L1确定性编码内部::写U64(编码,请求.幂等键.值);
    编码.insert(编码.end(),请求.请求意图摘要.字节.begin(),请求.请求意图摘要.字节.end());
    编码.insert(编码.end(),请求.执行证据摘要.字节.begin(),请求.执行证据摘要.字节.end());
    L1确定性编码内部::写U64(编码,结果.事实代次);
    L1确定性编码内部::写U64(编码,结果.新编码映射.size());
    for(const auto& [键,值]:结果.新编码映射){L1确定性编码内部::写U64(编码,键.值);L1确定性编码内部::写U64(编码,值.值);}
    L1确定性编码内部::写U64(编码,结果.发布后读回.项目组.size());
    for(const auto& 项:结果.发布后读回.项目组){L1确定性编码内部::写U8(编码,static_cast<std::uint8_t>(项.种类));std::visit([&](const auto& f){using T=std::decay_t<decltype(f)>;if constexpr(std::is_same_v<T,L1属性读取副本>){L1确定性编码内部::写U64(编码,f.节点.值);L1确定性编码内部::写U64(编码,f.属性类型节点.值);L1确定性编码内部::写U64(编码,f.当前值.值);}else{L1确定性编码内部::写U64(编码,f.编码.值);L1确定性编码内部::写U64(编码,f.创建事实代次);L1确定性编码内部::写U64(编码,f.退出事实代次.value_or(0));}},项.副本);}
    if (版本 == L1确定性结果摘要版本E01) {
        if (!完整(领域结果见证摘要)) return {};
        编码.insert(编码.end(), 领域结果见证摘要.字节.begin(), 领域结果见证摘要.字节.end());
    } else if (版本 != L1确定性结果摘要版本) return {};
    return L1确定性编码内部::哈希(std::move(编码));
}

inline L1幂等账记录 形成L1恢复幂等账记录(const L1写集请求& 请求,
    const L1写入结果& 结果) {
    return {请求.幂等键,请求.摘要合同版本,请求.请求意图摘要,请求.执行证据摘要,
        结果.事实代次,请求,结果.新编码映射,结果.发布后读回,
        形成L1确定性结果摘要(请求,结果),结果.失败见证身份,请求.执行证据材料,
        结果.确定性结果摘要版本,
        结果.领域结果见证 ? 形成L1领域结果见证摘要(*结果.领域结果见证) : L1确定性摘要{},
        结果.领域结果见证};
}

} // namespace 海中鱼巣
