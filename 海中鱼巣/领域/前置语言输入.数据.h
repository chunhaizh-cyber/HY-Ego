#pragma once

#include "接收认知来源.数据.h"
#include <cstdint>
#include <optional>
#include <stop_token>
#include <string>
#include <vector>

namespace 海中鱼巣 {
struct 新前置语言输入定位 final {
    新接收认知来源键 来源;
    稳定编码 前置来源节点, 固定文本材料, 语言节点;
    稳定编码 来源人类节点, 自我存在节点, 现实根场景;
    std::string 公开宿主会话;
    friend bool operator==(const 新前置语言输入定位&,const 新前置语言输入定位&)=default;
};
enum class 新前置语言缺口种类 : std::uint8_t {
    词面未登记=1, 名称材料不足, 对象未唯一绑定, 语言规则不足, 解释多义, 非本入口构式
};
struct 新前置语言缺口 final {
    新前置语言缺口种类 种类;
    std::uint64_t 起始标量=0, 结束标量=0;
    std::vector<稳定编码> 已取得相关节点;
    friend bool operator==(const 新前置语言缺口&,const 新前置语言缺口&)=default;
};
struct 新前置语言输入材料 final {
    新前置语言输入定位 定位;
    std::vector<新前置语言缺口> 缺口组;
    bool 可重入原认知=false;
    friend bool operator==(const 新前置语言输入材料&,const 新前置语言输入材料&)=default;
};
enum class 新前置语言读取状态 : std::uint8_t {
    已读取=1, 来源停用, 输入不适用, 前提已变化, 停止取消, 资源失败
};
struct 新前置语言输入读取结果 final {
    新前置语言读取状态 状态=新前置语言读取状态::输入不适用;
    std::optional<新前置语言输入材料> 材料;
};
class 新前置语言输入读取接口 {
public:
    virtual ~新前置语言输入读取接口()=default;
    virtual 新前置语言输入读取结果 读取前置语言输入材料(
        const 新前置语言输入定位&,std::stop_token) const=0;
};
}
