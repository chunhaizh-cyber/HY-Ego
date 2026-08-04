#ifndef L1_FACT_BASE_NO_INCLUDES
#include <algorithm>
#include <cstdint>
#include <limits>
#include <optional>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
#endif

namespace 海中鱼巣 {

struct 稳定编码 final {
    std::uint64_t 值 = 0;
    friend bool operator==(const 稳定编码&, const 稳定编码&) = default;
    friend bool operator<(const 稳定编码& 左, const 稳定编码& 右) noexcept { return 左.值 < 右.值; }
};

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

enum class 节点种类 : std::uint8_t { 普通 = 1, 属性类型 = 2 };
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
struct L1写集请求 final {
    std::uint64_t 期望事实代次 = 0;
    写集幂等键 幂等键;
    std::vector<节点新建项> 节点;
    std::vector<关系新建项> 关系;
    std::vector<值新建项> 值;
    std::vector<属性槽变更项> 属性槽变更;
    std::vector<稳定编码> 退出事实;
    friend bool operator==(const L1写集请求&, const L1写集请求&) = default;
};

enum class L1写入状态 : std::uint8_t {
    成功 = 1, 精确重复 = 2, 入口拒绝 = 3, 未找到 = 4, 已退出 = 5,
    事实代次漂移 = 6, 幂等冲突 = 7, 资源失败 = 8, 内部不一致 = 9
};
struct L1写入结果 final {
    L1写入状态 状态 = L1写入状态::入口拒绝;
    std::uint64_t 事实代次 = 0;
    std::vector<std::pair<写集本地键, 稳定编码>> 新编码映射;
    friend bool operator==(const L1写入结果&, const L1写入结果&) = default;
};

enum class L1读取状态 : std::uint8_t {
    成功 = 1, 入口拒绝 = 2, 未找到 = 3, 已退出 = 4,
    属性未设置 = 5, 事实代次漂移 = 6, 资源失败 = 7, 内部不一致 = 8
};
using L1事实副本 = std::variant<节点事实, 关系事实, 值事实>;
struct L1读取结果 final {
    L1读取状态 状态 = L1读取状态::入口拒绝;
    std::uint64_t 读取事实代次 = 0;
    std::optional<L1事实副本> 事实;
};
struct L1属性读取副本 final {
    稳定编码 节点;
    稳定编码 属性类型节点;
    稳定编码 当前值;
    friend bool operator==(const L1属性读取副本&, const L1属性读取副本&) = default;
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
    恢复候选建立 = 5, 恢复候选撤销 = 6, 恢复发布 = 7, 恢复拒绝 = 8
};
struct L1审计记录 final {
    写集幂等键 幂等键;
    std::uint64_t 事件序号 = 0;
    L1审计事件 事件 = L1审计事件::候选撤销;
    L1写入状态 结果状态 = L1写入状态::入口拒绝;
    std::uint64_t 事实代次 = 0;
    std::vector<std::pair<写集本地键, 稳定编码>> 新编码映射;
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
struct L1幂等账记录 final {
    写集幂等键 幂等键;
    L1写集请求 规范化写集;
    L1写入结果 首次结果;
};
struct L1恢复材料 final {
    L1完整快照 当前快照;
    std::vector<节点事实> 历史节点;
    std::vector<关系事实> 历史关系;
    std::vector<值事实> 历史值;
    std::vector<L1审计记录> 审计记录;
    std::vector<L1幂等账记录> 幂等账;
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

inline bool 有效(稳定编码 编码) noexcept { return 编码.值 != 0; }
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

} // namespace 海中鱼巣
