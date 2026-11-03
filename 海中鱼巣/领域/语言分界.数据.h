#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include "../核心/基础数据集.h"
#include "接收认知来源.数据.h"
#include "消息交互.数据.h"

namespace 海中鱼巣 {

struct 新语言前置来源请求 final { 新接收认知来源键 来源; };
struct 新语言前置来源信息 final {
    稳定编码 节点, 固定文本材料;
    新接收认知来源键 来源;
    std::string 固定UTF8原文;
};
enum class 新语言分界工程状态 : std::uint8_t {
    已完成=1, 未找到, 未初始化, 输入拒绝, 未支持, 前提已变化, 资源失败
};
template<class T> struct 新语言分界调用结果 final {
    新语言分界工程状态 状态=新语言分界工程状态::输入拒绝;
    std::optional<T> 信息;
    std::optional<稳定编码> 本次已发表身份;
};
struct 新分界消息读取结果 final {
    新语言分界工程状态 状态=新语言分界工程状态::输入拒绝;
    std::optional<新外设消息信息> 消息;
    基础只读预期 发布前提;
};
class 新语言分界消息读取接口 {
public:
    virtual ~新语言分界消息读取接口()=default;
    virtual 新分界消息读取结果 读取分界消息正文(稳定编码) const=0;
};

} // namespace 海中鱼巣
