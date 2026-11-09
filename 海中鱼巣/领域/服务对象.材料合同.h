#pragma once

#include <memory>
#include <string>
#include "新_特征类.h"
#include "新_存在类.h"
#include "新_方法公共类型.h"

namespace 海中鱼巣 {

// 公开会话仅用于本次来源复核，不能携带宿主的私有访问令牌。
struct 新服务对象窗口来源 final {
    稳定编码 窗口存在节点, 所在场景节点, 实际存在节点;
    std::string 公开会话标识;
    std::uint64_t 已接纳消息序号 = 0;
    friend bool operator==(const 新服务对象窗口来源&,
        const 新服务对象窗口来源&) = default;
};

// 调用期观察材料，不是服务关系，也不授权采纳此人为服务对象。
// 属于人类只是全特征匹配状态的投影；材料不足不表示确定不是人类。
struct 新服务对象材料 final {
    稳定编码 实际存在节点, 人类概念节点, 专属存在概念节点;
    bool 属于人类 = false;
    新存在概念匹配状态 概念匹配状态 = 新存在概念匹配状态::材料不足;
    std::optional<稳定编码> 服务对象节点;
    新服务对象窗口来源 来源;
    std::optional<新存在概念匹配采集结果> 概念匹配采集;
    // 与匹配使用同组材料的兼容投影；不包含历史、统计及标准值。
    std::vector<新特征信息> 实际特征快照组;
    friend bool operator==(const 新服务对象材料&, const 新服务对象材料&) = default;
};

struct 新服务对象获取配置 final {
    稳定编码 自我存在节点, 自我概念节点, 通用存在概念节点;
    稳定编码 人类概念节点, 交互者特征类型节点;
    // v3第5参数：自我的服务对象特征类型（结果），与交互者特征分开（R93）。
    稳定编码 服务对象特征类型节点;
    // 现实交互场景、交互者在现实世界的对应存在建立时所用的标识类型（监听在函数内部建立，INTERACT-SCENE-INSTINCT）。
    稳定编码 参与自我类型, 参与交互者类型, 外设存在标识类型;
    稳定编码 外设场景标识类型;
    friend bool operator==(const 新服务对象获取配置&,
        const 新服务对象获取配置&) = default;
};
// 获取服务对象 v4 唯一有序规格（INTERACT-SCENE-INSTINCT，2026-11-09用户裁决：监听内置，不以交互者为条件）：
// 1自我{服务对象}、2服务对象特征/1。监听外设世界新开的窗口，在函数内部建立现实交互场景与交互者对应存在。
inline std::vector<新方法有序参数规格> 获取服务对象有序规格(const 新服务对象获取配置& c) {
    using T = 新方法参数信息类型;
    return {
        整理有序参数规格({1, T::存在, c.自我概念节点, {c.服务对象特征类型节点}, 0}),
        {2, T::特征, c.服务对象特征类型节点, {}, 1}};
}

using 新服务对象窗口来源读取函数 = std::optional<新服务对象窗口来源> (*)() noexcept;
class 方法参数场景;
struct 新方法知识建立请求;
struct 新方法建立结果;
struct 新本能函数本次条件结果对;

class 新服务对象获取本能端口 {
public:
    virtual ~新服务对象获取本能端口() = default;
    virtual std::optional<新服务对象获取配置> 读取登记材料() const noexcept = 0;
    virtual std::optional<稳定编码> 读取初始连接关系概念() const noexcept = 0;
    virtual 新方法建立结果 确保方法登记(const 新方法知识建立请求&) noexcept = 0;
    virtual 新本能函数本次条件结果对 执行任务(
        const 方法参数场景&, 稳定编码 执行实例节点) noexcept = 0;
};
} // namespace 海中鱼巣
