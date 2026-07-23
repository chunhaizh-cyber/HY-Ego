// 文件规则：退役审计只读取已发布权威结构并形成值式审计/后验结果，投影失败不得改写权威退役回执。
module;

#include "节点直接P3合同.数据.h"

#include <cstdint>
#include <optional>
#include <vector>

export module 海中鱼巣.领域.审计.节点直接退役;

export namespace 海中鱼巣::领域 {

struct 权威退役回执 {
    std::uint64_t 请求编号 = 0;
    节点句柄 目标{};
    std::uint64_t 原节点版本 = 0;
    std::vector<正式关系值式材料> 关系历史组;
    std::vector<节点直接领域记录退役审计材料> 领域记录审计组;
    std::vector<索引审计值式材料> 索引审计组;
    节点审计值式材料 删除节点审计{};
    std::uint64_t 发布版本 = 0;
};

struct 节点直接退役审计读取请求 {
    节点直接P3请求头 头{};
    节点句柄 目标{};
    std::uint64_t 发布版本 = 0;
};

enum class 历史连续性状态 : std::uint16_t {
    连续 = 1,
    不连续 = 2
};

struct 历史连续性结果 {
    历史连续性状态 状态 = 历史连续性状态::不连续;
    std::uint64_t 原节点版本 = 0;
    std::uint64_t 发布版本 = 0;
    std::uint64_t 关系历史数 = 0;
    std::uint64_t 领域记录历史数 = 0;
    std::uint64_t 索引审计数 = 0;
};

struct 节点直接退役审计读取结果 {
    节点直接P3结果头 头{};
    std::optional<权威退役回执> 回执;
    std::optional<历史连续性结果> 历史连续性;
};

enum class 投影种类 : std::uint16_t {
    缓存 = 1,
    活动投影 = 2,
    反向索引投影 = 3,
    统计投影 = 4
};

struct 投影后验项目 {
    投影种类 种类 = 投影种类::缓存;
    std::uint64_t 失效版本 = 0;
    std::uint64_t 重建版本 = 0;
    std::uint64_t 复核版本 = 0;
    节点直接P3结果状态 状态 = 节点直接P3结果状态::请求拒绝;
    std::optional<节点直接P3内部错误材料> 内部错误;
};

struct 节点直接投影后验报告 {
    std::uint32_t 合同版本 = 0x00020000;
    std::uint64_t 发布版本 = 0;
    std::vector<投影后验项目> 项目组;
};

struct 节点直接投影后验请求 {
    节点直接P3请求头 头{};
    权威退役回执 权威回执{};
    std::vector<投影种类> 投影种类组;
};

struct 节点直接投影后验结果 {
    节点直接P3结果头 头{};
    std::optional<节点直接投影后验报告> 报告;
};

class 节点直接退役审计服务 final {
public:
    节点直接退役审计服务() = delete;

    static 节点直接退役审计读取结果 读取节点直接退役审计(
        const 节点直接退役审计读取请求&);
};

class 节点直接投影后验服务 final {
public:
    节点直接投影后验服务() = delete;

    static 节点直接投影后验结果 执行节点直接投影后验(
        const 节点直接投影后验请求&);
};

}
