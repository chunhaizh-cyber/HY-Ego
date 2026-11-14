#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "词典.数据.h"
#include "词条.数据.h"

namespace 海中鱼巣 {

enum class 新词典材料采用阶段 : std::uint8_t {
    未取得 = 1, 已取得未采用, 已采用
};

enum class 新词典变化处理阶段 : std::uint8_t {
    待处理 = 1, 已形成, 无需形成
};

// 保存本次动作的真实阶段；当前特征后来变化不改写这份执行材料。
struct 新本能词典读取回执 final {
    稳定编码 动作, 词条, 材料特征;
    新词典外设来源标识 查询来源;
    新词典查阅结局 查阅结局{};
    新词典材料采用阶段 采用阶段 = 新词典材料采用阶段::未取得;
    std::optional<稳定编码> 材料Q;
    bool 本次形成新状态 = false;
    std::optional<稳定编码> 前状态, 后状态, 变化动态;
    std::optional<新词典变化处理阶段> 变化阶段;
    std::optional<新词条动态记录原因> 无需形成原因;
    friend bool operator==(const 新本能词典读取回执&, const 新本能词典读取回执&) = default;
};

struct 新词典执行结果来源 final {
    稳定编码 词条, 材料特征;
    新词典外设来源标识 查询来源;
};
struct 新词典执行回执读取结果 final {
    新词典材料操作状态 状态 = 新词典材料操作状态::未找到;
    std::optional<新本能词典读取回执> 回执;
};

enum class 新词典运行状态 : std::uint8_t {
    已就绪 = 1, 已推进, 已收束, 条件未具备, 需重新组织,
    已停止, 输入拒绝, 未装配, 资源失败
};

struct 新词典本能输入 final {
    稳定编码 词条, 材料特征, 来源存在, 执行主体, 批次特征, 定位特征;
    新词典查询键 键;
    std::string 批次摘要, 原始包标识;
    std::uint64_t 外设场景标识 = 0;
    friend bool operator==(const 新词典本能输入&, const 新词典本能输入&) = default;
};

struct 新词典本能输入读取结果 final {
    新词典运行状态 状态 = 新词典运行状态::输入拒绝;
    std::optional<新词典本能输入> 输入;
    std::vector<新词典学习缺口> 缺口组;
    std::vector<稳定编码> 变化依赖节点组;
};

enum class 新词典推进收束原因 : std::uint8_t {
    当前目标已满足 = 1, 需求不再活动, 输入身份已更换, 主动停止
};

struct 新本能词典运行处置 final {
    新词典运行状态 状态 = 新词典运行状态::输入拒绝;
    std::vector<新词典学习缺口> 缺口组;
    std::vector<稳定编码> 变化依赖节点组;
    std::optional<新词典推进收束原因> 收束原因;
    friend bool operator==(const 新本能词典运行处置&, const 新本能词典运行处置&) = default;
};

enum class 新词典推进收束读取状态 : std::uint8_t {
    已读取 = 1, 尚未收束, 上下文不存在, 资源失败
};

struct 新词典推进收束材料 final {
    稳定编码 执行实例节点;
    新词典推进收束原因 原因{};
    std::optional<新本能词典读取回执> 回执;
};

struct 新词典推进收束读取结果 final {
    新词典推进收束读取状态 状态 = 新词典推进收束读取状态::上下文不存在;
    std::optional<新词典推进收束材料> 材料;
};

} // namespace 海中鱼巣
