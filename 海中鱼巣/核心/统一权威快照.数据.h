// 文件规则：NT-C4 值式快照材料只承载具名强类型段，不取得仓库、锁、令牌或运行期所有权。
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace 海中鱼巣 {

inline constexpr std::uint32_t 统一权威容器格式版本 = 1;
inline constexpr std::uint32_t 统一权威字节序版本 = 1;
inline constexpr std::uint32_t 统一权威容器头字节数 = 104;
inline constexpr std::uint32_t 统一权威段头字节数 = 64;
inline constexpr std::uint32_t 统一权威段清单版本 = 1;
inline constexpr std::uint32_t 统一权威摘要算法版本 = 1;
inline constexpr std::uint32_t 统一权威冻结规则版本 = 1;
inline constexpr std::uint32_t 统一权威命名域ABI版本 = 1;
inline constexpr std::uint64_t 统一权威文件最大字节数 = 8589934592ULL;
inline constexpr std::uint64_t 统一权威单段最大字节数 = 2147483648ULL;
inline constexpr std::uint64_t 统一权威单段最大记录数 = 4194304ULL;
inline constexpr std::uint64_t 统一权威单条最大字节数 = 16777216ULL;
inline constexpr std::uint64_t 统一权威字符串最大字节数 = 1048576ULL;
inline constexpr std::uint64_t 统一权威向量最大项数 = 1048576ULL;

using 规范化权威载荷摘要 = std::array<std::uint8_t, 32>;

enum class 统一权威快照段类型 : std::uint16_t {
    节点直接记录 = 1,
    正式关系记录 = 2,
    特征值类型化记录 = 3,
    状态域记录 = 4,
    动态域记录 = 5,
    任务方法选择记录 = 6,
    概念签名记录 = 7,
    用途观察记录 = 8,
    特征批次发布记录 = 9
};

struct 统一段来源 {
    std::uint64_t 来源运行期域身份 = 0;
    std::uint64_t 来源节点直接仓库身份 = 0;
    std::uint64_t 快照代次 = 0;

    bool 有效() const noexcept {
        return 来源运行期域身份 != 0
            && 来源节点直接仓库身份 != 0
            && 快照代次 != 0;
    }
};

struct 统一权威快照头 {
    std::array<std::uint8_t, 8> 魔数 = { 0x48, 0x5A, 0x59, 0x43, 0x53, 0x4E, 0x50, 0x31 };
    std::uint16_t 容器格式版本 = 1;
    std::uint16_t 字节序标识 = 1;
    std::uint32_t 容器头字节数 = 104;
    std::uint32_t 段头字节数 = 64;
    std::uint32_t 段清单版本 = 1;
    std::uint32_t 摘要算法版本 = 1;
    std::uint32_t 段数量 = 9;
    std::uint64_t 文件总字节数 = 0;
    std::uint64_t 来源运行期域身份 = 0;
    std::uint64_t 来源节点直接仓库身份 = 0;
    std::uint64_t 快照代次 = 0;
    std::uint32_t 冻结规则版本 = 1;
    std::uint32_t 节点稳定主键命名域ABI版本 = 1;
    规范化权威载荷摘要 容器摘要{};
};

struct 统一权威快照段头 {
    std::uint16_t 段类型 = 0;
    std::uint16_t 段格式版本 = 1;
    std::uint32_t 段头字节数 = 64;
    std::uint32_t 段顺序 = 0;
    std::uint32_t 段标志 = 0;
    std::uint64_t 记录数量 = 0;
    std::uint64_t 段载荷字节数 = 0;
    规范化权威载荷摘要 段载荷摘要{};
};

struct 节点直接记录 {
    std::uint64_t 稳定主键高位 = 0;
    std::uint64_t 稳定主键低位 = 0;
    std::uint16_t 节点类型ABI = 0;
    std::uint32_t 版本号 = 0;
    std::uint16_t 状态ABI = 0;
    std::uint64_t 创建序号 = 0;
    std::uint32_t 命名域ABI版本 = 1;
    std::uint64_t 域签发高水位 = 0;
};

struct 正式关系记录 {
    std::uint64_t 关系编号 = 0;
    std::uint32_t 关系版本 = 0;
    std::uint16_t 关系类型ABI = 0;
    std::uint16_t 角色ABI = 0;
    std::uint64_t 源主键高位 = 0;
    std::uint64_t 源主键低位 = 0;
    std::uint64_t 目标主键高位 = 0;
    std::uint64_t 目标主键低位 = 0;
    std::uint64_t 顺序号 = 0;
    std::uint16_t 状态ABI = 0;
    std::uint8_t 历史标记 = 0;
};

struct 特征值类型化记录 {
    std::uint64_t 记录编号 = 0;
    std::uint64_t 槽位主键高位 = 0;
    std::uint64_t 槽位主键低位 = 0;
    std::uint16_t 原始值类型ABI = 0;
    std::uint32_t 原始值版本 = 0;
    std::uint8_t 存在I64值 = 0;
    std::int64_t I64值 = 0;
    std::vector<std::int64_t> VecI64值;
    std::vector<std::uint64_t> VecU64值;
    std::uint8_t 历史标记 = 0;
};

struct 状态域记录 {
    std::uint64_t 记录编号 = 0;
    std::uint64_t 主体主键高位 = 0;
    std::uint64_t 主体主键低位 = 0;
    std::uint64_t 特征主键高位 = 0;
    std::uint64_t 特征主键低位 = 0;
    std::uint64_t 值主键高位 = 0;
    std::uint64_t 值主键低位 = 0;
    std::uint64_t 场景主键高位 = 0;
    std::uint64_t 场景主键低位 = 0;
    std::uint64_t 发生时间 = 0;
    std::uint64_t 接收时间 = 0;
    std::uint64_t 来源主键 = 0;
    std::uint16_t 记录模式ABI = 0;
    std::uint8_t 历史标记 = 0;
};

struct 动态域记录 {
    std::uint64_t 记录编号 = 0;
    std::uint64_t 主体主键高位 = 0;
    std::uint64_t 主体主键低位 = 0;
    std::uint64_t 被改变目标高位 = 0;
    std::uint64_t 被改变目标低位 = 0;
    std::uint64_t 前状态主键 = 0;
    std::uint64_t 后状态主键 = 0;
    std::uint64_t 场景主键高位 = 0;
    std::uint64_t 场景主键低位 = 0;
    std::uint64_t 发生时间 = 0;
    std::uint64_t 来源动作主键 = 0;
    std::uint16_t 动态种类ABI = 0;
    std::uint16_t 聚合规则版本 = 0;
    std::uint8_t 历史标记 = 0;
};

struct 任务方法选择记录 {
    std::uint64_t 幂等材料编号 = 0;
    std::uint64_t 筹办轮次 = 0;
    std::uint32_t 规则版本 = 0;
    std::uint32_t 方法内容版本 = 0;
    std::uint16_t 关系角色ABI = 0;
    std::uint8_t 历史标记 = 0;
};

struct 概念签名记录 {
    std::uint64_t 概念主键高位 = 0;
    std::uint64_t 概念主键低位 = 0;
    std::uint32_t 签名规则版本 = 0;
    std::vector<std::uint64_t> 有序非拓扑约束原始值;
    std::uint32_t 记录版本 = 0;
    std::uint8_t 历史标记 = 0;
};

struct 用途观察记录 {
    std::uint64_t 记录编号 = 0;
    std::uint64_t 所属对象高位 = 0;
    std::uint64_t 所属对象低位 = 0;
    std::string 观察材料UTF8;
    std::uint32_t 观察版本 = 0;
    std::uint8_t 历史标记 = 0;
};

struct 特征批次发布记录 {
    std::uint64_t 批次编号 = 0;
    std::uint64_t 槽位主键高位 = 0;
    std::uint64_t 槽位主键低位 = 0;
    std::uint64_t 新值主键高位 = 0;
    std::uint64_t 新值主键低位 = 0;
    std::uint32_t 顺序号 = 0;
    std::uint32_t 批次规则版本 = 0;
    std::uint8_t 历史标记 = 0;
};

struct 节点直接记录段 { 统一段来源 来源; std::uint64_t 高水位 = 0; std::vector<节点直接记录> 记录组; };
struct 正式关系记录段 { 统一段来源 来源; std::uint64_t 高水位 = 0; std::vector<正式关系记录> 记录组; };
struct 特征值类型化记录段 { 统一段来源 来源; std::uint64_t 高水位 = 0; std::vector<特征值类型化记录> 记录组; };
struct 状态域记录段 { 统一段来源 来源; std::uint64_t 高水位 = 0; std::vector<状态域记录> 记录组; };
struct 动态域记录段 { 统一段来源 来源; std::uint64_t 高水位 = 0; std::vector<动态域记录> 记录组; };
struct 任务方法选择记录段 { 统一段来源 来源; std::uint64_t 高水位 = 0; std::vector<任务方法选择记录> 记录组; };
struct 概念签名记录段 { 统一段来源 来源; std::uint64_t 高水位 = 0; std::vector<概念签名记录> 记录组; };
struct 用途观察记录段 { 统一段来源 来源; std::uint64_t 高水位 = 0; std::vector<用途观察记录> 记录组; };
struct 特征批次发布记录段 { 统一段来源 来源; std::uint64_t 高水位 = 0; std::vector<特征批次发布记录> 记录组; };

struct 统一冻结规格 {
    std::uint64_t 来源运行期域身份 = 0;
    std::uint64_t 来源节点直接仓库身份 = 0;
    std::uint64_t 快照代次 = 0;
    std::uint32_t 冻结规则版本 = 1;
    std::uint32_t 命名域ABI版本 = 1;
};

struct 统一冻结提供者组 {
    std::function<节点直接记录段()> 读取节点直接记录段;
    std::function<正式关系记录段()> 读取正式关系记录段;
    std::function<特征值类型化记录段()> 读取特征值类型化记录段;
    std::function<状态域记录段()> 读取状态域记录段;
    std::function<动态域记录段()> 读取动态域记录段;
    std::function<任务方法选择记录段()> 读取任务方法选择记录段;
    std::function<概念签名记录段()> 读取概念签名记录段;
    std::function<用途观察记录段()> 读取用途观察记录段;
    std::function<特征批次发布记录段()> 读取特征批次发布记录段;
    std::function<bool()> 冻结权仍有效;

    bool 完整() const noexcept {
        return static_cast<bool>(读取节点直接记录段)
            && static_cast<bool>(读取正式关系记录段)
            && static_cast<bool>(读取特征值类型化记录段)
            && static_cast<bool>(读取状态域记录段)
            && static_cast<bool>(读取动态域记录段)
            && static_cast<bool>(读取任务方法选择记录段)
            && static_cast<bool>(读取概念签名记录段)
            && static_cast<bool>(读取用途观察记录段)
            && static_cast<bool>(读取特征批次发布记录段)
            && static_cast<bool>(冻结权仍有效);
    }
};

struct 统一权威冻结材料 {
    统一冻结规格 规格;
    统一权威快照头 头;
    节点直接记录段 节点段;
    正式关系记录段 关系段;
    特征值类型化记录段 特征值段;
    状态域记录段 状态段;
    动态域记录段 动态段;
    任务方法选择记录段 任务方法段;
    概念签名记录段 概念段;
    用途观察记录段 用途段;
    特征批次发布记录段 批次段;
};

enum class 必需段声明组状态 : std::uint8_t { 已形成 = 1, 内部不一致 = 2 };
struct 统一权威段声明 {
    统一权威快照段类型 类型 = 统一权威快照段类型::节点直接记录;
    std::uint16_t 段格式版本 = 1;
    std::uint32_t 段顺序 = 0;
    std::uint64_t 最大记录数 = 统一权威单段最大记录数;
    std::uint64_t 最大载荷字节数 = 统一权威单段最大字节数;
};
struct 必需段声明组读取结果 {
    必需段声明组状态 状态 = 必需段声明组状态::内部不一致;
    std::array<统一权威段声明, 9> 声明组{};
};

enum class 统一冻结结果状态 : std::uint8_t { 已形成 = 1, 入口拒绝 = 2, 资源失败 = 3, 内部不一致 = 4 };
struct 统一冻结结果 {
    统一冻结结果状态 状态 = 统一冻结结果状态::入口拒绝;
    std::optional<统一权威冻结材料> 材料;
};

struct 规范化快照字节材料 {
    std::vector<std::uint8_t> 字节;
    规范化权威载荷摘要 权威载荷摘要{};
};
enum class 规范化编码结果状态 : std::uint8_t { 已形成 = 1, 入口拒绝 = 2, 资源失败 = 3, 内部不一致 = 4 };
struct 规范化编码结果 {
    规范化编码结果状态 状态 = 规范化编码结果状态::入口拒绝;
    std::optional<规范化快照字节材料> 材料;
};

struct 快照文件保存请求 {
    std::filesystem::path 目标路径;
    规范化快照字节材料 材料;
};
enum class 快照文件保存结果状态 : std::uint8_t { 已保存 = 1, 路径拒绝 = 2, 资源失败 = 3, 内部不一致 = 4 };
struct 快照文件保存结果 {
    快照文件保存结果状态 状态 = 快照文件保存结果状态::路径拒绝;
};

inline bool 统一段来源相等(const 统一段来源& 左, const 统一冻结规格& 右) noexcept {
    return 左.来源运行期域身份 == 右.来源运行期域身份
        && 左.来源节点直接仓库身份 == 右.来源节点直接仓库身份
        && 左.快照代次 == 右.快照代次;
}

} // namespace 海中鱼巣
