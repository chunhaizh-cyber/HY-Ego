#pragma once

#include "新_世界树类.h"
#include <string_view>

namespace 海中鱼巣 {

struct 新外设对话框定位 final {
    稳定编码 场景节点, 窗口节点;
    稳定编码 接收数量特征, 接收内容特征;
    friend bool operator==(const 新外设对话框定位&, const 新外设对话框定位&) = default;
};
enum class 新外设事实状态 : std::uint8_t {
    已完成, 无变化, 入口拒绝, 未初始化, 结构不一致, 资源失败
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
    新外设对话框建立结果 建立或取得对话框场景(
        std::string_view 公开宿主会话, std::string_view 对话框标识) noexcept;
    新外设事实状态 更新接收(
        const 新外设对话框定位&, std::string_view 实际UTF8正文,
        const 新状态强时间& 接收时间) noexcept;
private:
    std::optional<新状态信息> 读取最近特征状态(稳定编码 场景节点, 稳定编码 存在节点, 稳定编码 特征节点) const;
    新外设事实状态 更新窗口特征并形成动态(const 新外设对话框定位&, 稳定编码 特征节点,
        const 新特征准确值&, const 新状态强时间&);
    void 核验并处理已发布动态(稳定编码 场景节点, 稳定编码 动态节点);
    新外设对话框定位 读取窗口定位(稳定编码 场景节点) const;
    稳定编码 读取类型特征(const std::vector<稳定编码>& 特征组, 稳定编码 类型根) const;
    std::string 读取文本特征(稳定编码 特征) const;
    新_世界树类& 世界_;
    新世界事实服务引用 服务_;
    新_场景类 场景_;
};

} // namespace 海中鱼巣
