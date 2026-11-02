#pragma once
#include "../领域/语言补全.运行接口.h"
#include "../领域/语言含义图.数据.h"
namespace 海中鱼巣 {
class 新_外设世界树类;class 新_自然语言世界树类;class 新_语言类;class 新_词类;
class 概念_名称关系类;class 概念_存在类;class 概念_特征类;class 概念_动态类;class 概念_二次特征类;
class 新_存在类;class 新_特征值类;class 新_特征类;class 新_场景类;class 新_需求类;class 新_方法类;class 新_动态类;
class 接收需求认知服务;
class 语言补全运行服务 final : public 新语言补全运行接口 {
public :
    语言补全运行服务(新_外设世界树类&,新_自然语言世界树类&,新_语言类&,新_词类&,
        概念_名称关系类&,概念_存在类&,概念_特征类&,概念_动态类&,概念_二次特征类&,
        新_存在类&,新_特征值类&,新_特征类&,新_场景类&,新_需求类&,新_方法类&,新_动态类&,接收需求认知服务&);
    ~语言补全运行服务();
    bool 初始化(const 新字段询问登记配置&);
    void 关闭() noexcept;
    bool 记录采用指代(稳定编码 采用含义,稳定编码 说话者,稳定编码 受话者);
    新语言指代结果 消解交互指代(const 新语言指代请求&);
    新语言补全缺口结果 读取缺口(const 新需求字段追问来源&) const override;
    std::optional<稳定编码> 读取方法(新语言补全种类,新语言补全动作) const override;
    新语言补全询问结果 询问(const 方法参数场景&,const 新语言补全目标&,const 新需求字段追问来源&) override;
    新语言补全发送结果 发送(const 方法参数场景&,const 新语言补全候选&,const 新需求字段追问来源&) override;
    新语言补全回答写回结果 回答写回(const 方法参数场景&,const 新语言补全目标&,const 新需求字段追问来源&) override;
private :
    struct 实现;std::unique_ptr<实现> 实现_;
};
}
