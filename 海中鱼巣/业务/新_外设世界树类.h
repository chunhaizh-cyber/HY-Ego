#pragma once

#include "新_世界树类.h"
#include "../领域/消息交互.数据.h"
#include <string>
#include <string_view>
#include <functional>

namespace 海中鱼巣 {

struct 新外设对话框定位 final {
    稳定编码 场景节点, 窗口节点;
    稳定编码 接收数量特征, 接收内容特征;
    std::optional<稳定编码> 发送数量特征, 发送内容特征;
    std::optional<稳定编码> 接收消息特征, 发送消息特征;
    std::optional<稳定编码> 输出队列特征;
    friend bool operator==(const 新外设对话框定位&, const 新外设对话框定位&) = default;
};
struct 新外设对话框来源 final {
    新外设对话框定位 定位;
    std::string 公开宿主会话, 对话框标识;
};
enum class 新外设输出队列状态 : std::uint8_t {
    已入列=1, 已复用, 已领取, 已移除, 已清空, 已读取,
    队列为空, 不在队列, 来源不存在, 目标不符, 未初始化, 前提已变化
};
struct 新外设输出入列请求 final {
    新外设对话框定位 定位;
    std::uint64_t 提交序号 = 0;
    std::string UTF8正文;
};
struct 新外设输出提交请求 final {
    新外设对话框定位 定位;
    std::string UTF8正文;
    std::optional<稳定编码> 来源动作;
};
struct 新外设输出队列结果 final {
    新外设输出队列状态 状态;
    std::optional<新外设消息信息> 消息;
    std::optional<稳定编码> 入列状态, 入列动态;
};
struct 新外设原子发送请求 final {
    新外设对话框定位 定位;
    稳定编码 执行场景, 执行主体, 动作定义, 动作来源;
    std::string UTF8正文;
    std::vector<基础联合节点快照> 完整保护组;
};
struct 新外设原子发送结果 final {
    新外设输出队列结果 输出{新外设输出队列状态::前提已变化, {}};
    std::optional<稳定编码> 动作节点;
};
struct 新外设输出队列读取结果 final {
    新外设输出队列状态 状态;
    std::optional<稳定编码> 特征节点;
    std::vector<新外设消息信息> 消息组;
};
enum class 新外设事实状态 : std::uint8_t {
    已完成, 无变化, 入口拒绝, 未初始化, 结构不一致, 资源失败, 未找到
};
struct 新外设接收记录完整结果 final {
    新外设事实状态 状态 = 新外设事实状态::未初始化;
    std::vector<新外设消息信息> 消息组;
};
struct 新外设消息保护结果 final {
    新外设事实状态 状态 = 新外设事实状态::未初始化;
    std::optional<新外设消息信息> 消息;
    std::vector<基础联合节点快照> 完整保护;
};
struct 新外设对话框建立结果 final {
    新外设事实状态 状态 = 新外设事实状态::入口拒绝;
    std::optional<新外设对话框定位> 定位;
};

// 借用共享事实服务，只拥有独立外设根及其对话框组织；不解释对方身份。
class 新_外设世界树类 final {
public:
    explicit 新_外设世界树类(新_世界树类&) noexcept;
    新世界初始化结果 初始化() noexcept;
    std::optional<稳定编码> 获取输出队列特征类型() const noexcept;
    新外设对话框建立结果 建立或取得对话框场景(
        std::string_view 公开宿主会话, std::string_view 对话框标识) noexcept;
    新外设对话框建立结果 查找对话框场景(
        std::string_view 公开宿主会话, std::string_view 对话框标识) const noexcept;
    std::optional<新外设对话框来源> 读取对话框来源(稳定编码 窗口节点) const noexcept;
    const 新_场景类& 获取场景只读视图() const noexcept { return 场景_; }
    新外设事实状态 更新发送(
        const 新外设对话框定位&, std::uint64_t 来源序号, std::string_view 实际UTF8正文,
        const 新状态强时间& 发布时间) noexcept;
    新外设事实状态 更新接收(
        const 新外设对话框定位&, std::uint64_t 来源序号, std::string_view 实际UTF8正文,
        const 新状态强时间& 接收时间) noexcept;
    std::optional<新外设消息信息> 读取消息(稳定编码 消息节点) const noexcept;
    新外设接收记录完整结果 完整读取仍保留的接收记录() const noexcept;
    新外设消息保护结果 读取消息保护(稳定编码 消息节点) const noexcept;
    std::optional<新外设消息信息> 查找消息(const 新外设消息来源键&) const noexcept;
    std::vector<新外设消息信息> 读取当前消息(
        稳定编码 窗口节点, 新外设消息方向) const noexcept;
    新外设事实状态 移除当前消息引用(
        稳定编码 窗口节点, 新外设消息方向, 稳定编码 消息节点,
        const 新状态强时间&) noexcept;
    新外设输出队列结果 输出入列(const 新外设输出入列请求&) noexcept;
    新外设输出队列结果 提交输出(const 新外设输出提交请求&) noexcept;
    // 语言读集、正式消息、独立动作及队列核心只提交一次；变化治理在成功后进行。
    新外设原子发送结果 原子提交发送(const 新外设原子发送请求&,
        const std::function<bool()>& 复核来源) noexcept;
    新外设输出队列结果 领取输出队首(const 新外设对话框定位&) noexcept;
    新外设输出队列结果 移除待发输出(
        const 新外设对话框定位&, std::uint64_t 提交序号) noexcept;
    新外设输出队列结果 清空待发输出(const 新外设对话框定位&) noexcept;
    新外设输出队列读取结果 读取输出队列(
        const 新外设对话框定位&) const noexcept;
private:
    struct 特征变化读回 final {
        新外设事实状态 状态;
        std::optional<稳定编码> 状态节点, 动态节点;
        operator 新外设事实状态() const noexcept { return 状态; }
    };
    新外设输出队列结果 输出入列_已加锁(const 新外设输出入列请求&, std::optional<稳定编码> 来源动作={});
    新外设对话框定位 补齐消息集合(const 新外设对话框定位&);
    新外设事实状态 记录消息(const 新外设对话框定位&, 新外设消息方向,
        std::uint64_t, std::string_view, const 新状态强时间&);
    std::vector<稳定编码> 读取消息成员(稳定编码 特征节点) const;
    std::vector<新外设消息信息> 读取输出队列成员(const 新外设对话框定位&) const;
    特征变化读回 更新输出队列(const 新外设对话框定位&,
        const std::vector<新外设消息信息>&, std::optional<稳定编码> 来源动作={});
    新外设消息信息 建立输出消息(const 新外设对话框定位&,
        const 新外设消息来源键&, std::string_view);
    void 完成消息集合变化(const 新外设对话框定位&, 稳定编码 特征节点,
        稳定编码 新材料, const 新特征值添加结果&,
        const std::optional<新状态信息>& 前, const 新状态强时间&);
    std::optional<新状态信息> 读取最近特征状态(稳定编码 场景节点, 稳定编码 存在节点, 稳定编码 特征节点) const;
    特征变化读回 更新窗口特征并形成动态(const 新外设对话框定位&, 稳定编码 特征节点,
        const 新特征准确值&, const 新状态强时间&, std::optional<稳定编码> 来源动作={});
    void 核验并处理已发布动态(稳定编码 场景节点, 稳定编码 动态节点);
    新外设对话框定位 读取窗口定位(稳定编码 场景节点) const;
    std::optional<新外设对话框定位> 查找正式窗口(
        std::string_view 公开宿主会话, std::string_view 对话框标识) const;
    稳定编码 读取类型特征(const std::vector<稳定编码>& 特征组, 稳定编码 类型根) const;
    std::string 读取文本特征(稳定编码 特征) const;
    新_世界树类& 世界_;
    新世界事实服务引用 服务_;
    新_场景类 场景_;
};

} // namespace 海中鱼巣
