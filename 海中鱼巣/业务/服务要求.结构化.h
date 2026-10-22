#pragma once

#include "../领域/新_需求类.h"
#include "../领域/新_自然语言世界树类.h"
#include "../领域/服务要求.材料合同.h"
#include <memory>
#include <mutex>
#include <string>
#include <string_view>

namespace 海中鱼巣 {

enum class 结构化服务要求状态 : std::uint8_t {
    已建立 = 1, 已复用, 已读取, 未初始化, 会话未绑定, 会话已失效,
    目标不完整, 目标不合法, 来源不合法, 理解不存在,
    确认对象不匹配, 结构不一致, 资源失败, 需重新确认, 语义快照暂不支持
};

struct 服务要求特征语义快照 {
    稳定编码 概念节点;
    特征概念材料物理类型 材料类型 = 特征概念材料物理类型::I64标量;
    std::optional<稳定编码> 单位;
    特征概念比较规则 比较规则 = 特征概念比较规则::I64数值;
    friend bool operator==(const 服务要求特征语义快照&, const 服务要求特征语义快照&) = default;
};
struct 服务要求值域语义快照 {
    服务要求特征语义快照 特征语义;
    特征概念值域 值域;
    friend bool operator==(const 服务要求值域语义快照&, const 服务要求值域语义快照&) = default;
};
struct 服务要求比较语义快照 {
    稳定编码 比较概念节点;
    稳定编码 类型根节点;
    二次特征算法 算法 = 二次特征算法::有向特征差值;
    std::vector<二次特征概念输入定义> 输入定义组;
    std::optional<稳定编码> 参照概念节点;
    服务要求值域语义快照 输出语义;
    std::vector<二次特征结果域种类> 结果域种类组;
    friend bool operator==(const 服务要求比较语义快照&, const 服务要求比较语义快照&) = default;
};
struct 服务要求目标语义快照 {
    服务要求特征语义快照 目标类型;
    std::optional<服务要求值域语义快照> 目标域;
    std::optional<服务要求比较语义快照> 比较;
    friend bool operator==(const 服务要求目标语义快照&, const 服务要求目标语义快照&) = default;
};
struct 服务要求理解请求 {
    std::string 公开会话标识;
    稳定编码 来源人类节点;
    稳定编码 目标存在节点;
    新需求目标定义 目标;
    std::optional<稳定编码> 来源要求节点;
};
struct 服务要求理解信息 {
    稳定编码 节点;
    稳定编码 语言场景节点;
    std::string 公开会话标识;
    稳定编码 来源人类节点;
    稳定编码 目标存在节点;
    新需求目标定义 目标;
    服务要求目标语义快照 固定语义;
    std::optional<稳定编码> 来源要求节点;
};
struct 服务要求确认信息 {
    稳定编码 节点;
    服务要求理解信息 理解;
    bool 需重新确认 = false;
};
struct 服务要求理解结果 {
    结构化服务要求状态 状态 = 结构化服务要求状态::未初始化;
    std::optional<服务要求理解信息> 理解;
};
struct 服务要求确认结果 {
    结构化服务要求状态 状态 = 结构化服务要求状态::未初始化;
    std::optional<服务要求确认信息> 确认;
};
struct 服务要求确认查询结果 {
    结构化服务要求状态 状态 = 结构化服务要求状态::未初始化;
    std::vector<服务要求确认信息> 确认组;
};

struct 部分服务要求提交 {
    std::string 公开会话标识, 来源要求标识;
    稳定编码 来源人类节点;
    std::optional<稳定编码> 要求节点;
    新部分服务要求目标 补充目标;
};
enum class 部分服务要求状态 : std::uint8_t {
    已建立=1, 已补充, 精确重复, 已读取, 来源不匹配,
    已知值冲突, 输入不合法, 要求不存在, 需重新确认, 工程故障
};
struct 部分服务要求结果 {
    部分服务要求状态 状态 = 部分服务要求状态::工程故障;
    std::optional<新部分服务要求材料> 材料;
    std::vector<新服务要求缺项> 缺项, 冲突项;
    std::optional<服务要求理解信息> 理解;
};

// 语言材料的唯一业务owner；不创建自我需求、任务或现实结果。
// 注入服务的寿命须覆盖本对象；活跃会话权限由装配在调用写入口前核验。
class 结构化服务要求服务 final : public 新服务要求材料读取接口,
    public 新部分服务要求读取接口 {
public:
    结构化服务要求服务(新_自然语言世界树类&, 新_存在类&, 新_特征值类&,
        概念_存在类&, 概念_特征类&, 概念_二次特征类&) noexcept;
    ~结构化服务要求服务();
    结构化服务要求状态 初始化(稳定编码 人类概念节点) noexcept;
    服务要求理解结果 建立或取得理解(const 服务要求理解请求&) noexcept;
    服务要求理解结果 读取理解(稳定编码 理解节点) const noexcept;
    // 输入拒绝为业务结果；结构/资源异常向装配工程边界送出。
    部分服务要求结果 提交或补充要求(const 部分服务要求提交&) noexcept;
    部分服务要求结果 查询部分要求(std::string_view 会话,
        稳定编码 人类, 稳定编码 要求) const noexcept;
    // 同一来源键的只读定位；没有匹配时不建立材料。
    部分服务要求结果 查询部分要求(std::string_view 会话,
        稳定编码 人类, std::string_view 来源要求标识) const noexcept;
    // schema启动前冻结；不取得业务mutex，供持有需求锁的读取者调用。
    新部分服务要求读取结果 读取部分服务要求(稳定编码 要求节点) const noexcept override;
    新服务要求分项读取结果 读取服务要求分项(
        const 新服务要求分项引用&) const noexcept override;
    服务要求确认结果 确认理解(std::string_view 公开会话标识,
        稳定编码 来源人类节点, 稳定编码 理解节点) noexcept;
    服务要求确认查询结果 查询已确认要求(稳定编码 来源人类节点) const noexcept;
    新服务要求确认读取结果 读取服务要求确认身份(稳定编码) const noexcept override;
    新服务要求确认读取结果 复核服务要求确认语义(稳定编码) const noexcept override;
    新服务要求确认保护结果 准备服务要求确认语义保护(稳定编码) const noexcept override;
    新服务要求确认同义结果 比较服务要求确认固定语义(
        稳定编码, 稳定编码) const noexcept override;
private:
    struct 实现;
    新_自然语言世界树类& 语言_;
    新_存在类& 存在_;
    新_特征值类& 特征值_;
    概念_存在类& 存在概念_;
    概念_特征类& 特征概念_;
    概念_二次特征类& 二次概念_;
    mutable std::mutex 互斥_;
    std::unique_ptr<实现> 实现_;
};

} // namespace 海中鱼巣
