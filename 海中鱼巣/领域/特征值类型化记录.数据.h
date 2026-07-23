// 文件规则：本头只定义特征值领域三态记录、值式请求与结果，不保存拓扑、来源端点或 4170 批次业务字段。
#pragma once

namespace 海中鱼巣 {

inline constexpr std::uint32_t 特征值类型化记录当前格式版本 = 1;
inline constexpr std::size_t 特征值最大序列元素数量 = 1U << 20;

enum class 特征值原始类型 : std::uint32_t {
    未建立 = 0,
    I64 = 1,
    VecI64 = 2,
    VecU64 = 3
};

struct 特征值原始值式材料 {
    特征值原始类型 类型 = 特征值原始类型::未建立;
    std::optional<std::int64_t> I64值;
    std::vector<std::int64_t> VecI64值;
    std::vector<std::uint64_t> VecU64值;

    bool 完整() const noexcept {
        switch (类型) {
        case 特征值原始类型::I64:
            return I64值.has_value() && VecI64值.empty() && VecU64值.empty();
        case 特征值原始类型::VecI64:
            return !I64值.has_value() && !VecI64值.empty()
                && VecI64值.size() <= 特征值最大序列元素数量 && VecU64值.empty();
        case 特征值原始类型::VecU64:
            return !I64值.has_value() && VecI64值.empty() && !VecU64值.empty()
                && VecU64值.size() <= 特征值最大序列元素数量;
        default:
            return false;
        }
    }

    bool operator==(const 特征值原始值式材料&) const = default;
};

struct 特征值类型化记录 {
    节点句柄 特征值;
    std::uint32_t 记录格式版本 = 特征值类型化记录当前格式版本;
    std::uint64_t 原始值版本 = 0;
    记录状态 状态 = 记录状态::无效;
    特征值原始值式材料 原始值;

    bool 完整() const noexcept {
        return 句柄有效(特征值)
            && 记录格式版本 == 特征值类型化记录当前格式版本
            && 原始值版本 != 0 && 状态 == 记录状态::有效 && 原始值.完整();
    }

    bool operator==(const 特征值类型化记录&) const = default;
};

struct 特征值类型化记录规格 {
    节点句柄 特征值;
    std::uint64_t 原始值版本 = 0;
    特征值原始值式材料 原始值;

    bool 完整() const noexcept {
        return 句柄有效(特征值) && 原始值版本 != 0 && 原始值.完整();
    }
};

enum class 特征值归属读取口径 : std::uint32_t {
    当前 = 1,
    历史审计 = 2
};

struct 特征值归属值式材料 {
    节点句柄 特征值;
    std::uint32_t 节点版本 = 0;
    std::uint64_t 原始值版本 = 0;
    节点句柄 实例槽;
    节点句柄 抽象定义;
    正式关系记录 定义模板证据;
    正式关系记录 当前或历史值证据;
    特征值原始值式材料 原始值;
};

struct 实例特征槽位值式材料 {
    节点句柄 实例槽;
    节点句柄 抽象定义;
    节点句柄 当前特征值;
    正式关系记录 宿主证据;
    正式关系记录 定义证据;
    正式关系记录 当前值证据;
    std::uint64_t 原始值版本 = 0;
    特征值原始值式材料 原始值;
};

struct 宿主实例特征槽位组值式材料 {
    节点句柄 宿主;
    std::vector<实例特征槽位值式材料> 项目组;
    std::uint64_t 读取结构版本 = 0;
};

struct 特征值退役规格 {
    节点句柄 特征值;
    std::uint32_t 预期节点版本 = 0;
    std::uint64_t 预期原始值版本 = 0;
};

struct 特征值批次引用值式材料 {
    特征批次身份 批次身份;
    std::uint64_t 业务规则版本 = 0;
    std::uint64_t 项目顺序号 = 0;
    节点句柄 宿主;
    节点句柄 定义;
    节点句柄 实例槽;
    节点句柄 新特征值;
    节点句柄 来源;
    std::optional<关系句柄> 写前当前关系;
    std::optional<节点句柄> 写前当前值;
    std::optional<std::uint64_t> 写前原始值版本;
    关系句柄 发布后当前关系;
};

struct 特征值退役材料 {
    节点句柄 特征值;
    std::uint32_t 节点版本 = 0;
    std::uint64_t 原始值版本 = 0;
    std::vector<正式关系记录> 当前或失效关系22组;
    std::vector<正式关系记录> 来源关系组;
    std::vector<特征值批次引用值式材料> 批次引用组;
};

using 特征值归属读取结果 = 合同单项结果<特征值归属值式材料>;
using 宿主实例特征槽位组读取结果 = 合同具名组结果<宿主实例特征槽位组值式材料>;
using 特征值退役规格结果 = 合同单项结果<特征值退役规格>;

struct 新实例槽位发布请求 {
    节点句柄 宿主;
    节点句柄 抽象定义;
    节点稳定主键 实例槽主键;
    节点稳定主键 特征值主键;
    节点句柄 来源;
    特征值原始值式材料 原始值;
};

struct 实例槽位当前值换代请求 {
    节点句柄 实例槽;
    关系句柄 写前当前关系;
    节点句柄 写前当前值;
    std::uint64_t 写前原始值版本 = 0;
    节点稳定主键 新特征值主键;
    节点句柄 来源;
    特征值原始值式材料 新原始值;
};

enum class 特征批次变更种类 : std::uint32_t {
    初始 = 1,
    换代 = 2
};

struct 特征批次变更项目请求 {
    std::uint64_t 项目顺序号 = 0;
    特征批次变更种类 种类 = 特征批次变更种类::初始;
    节点句柄 抽象定义;
    节点稳定主键 实例槽主键;
    节点句柄 实例槽;
    std::optional<关系句柄> 写前当前关系;
    std::optional<节点句柄> 写前当前值;
    std::optional<std::uint64_t> 写前原始值版本;
    节点稳定主键 新特征值主键;
    节点句柄 来源;
    特征值原始值式材料 新原始值;
};

struct 特征批次变更请求 {
    特征批次身份 批次身份;
    std::uint64_t 业务规则版本 = 0;
    节点句柄 宿主;
    std::vector<特征批次变更项目请求> 项目组;
};

struct 特征批次提交值式材料 {
    特征批次身份 批次身份;
    std::uint64_t 业务规则版本 = 0;
    节点句柄 宿主;
    std::vector<实例特征槽位值式材料> 项目组;
};

}
