// 文件规则：状态动态退役适配只保持状态和动态强类型分账，不复制状态/动态记录或关系端点。
module;

#include "节点直接P3合同.数据.h"

export module 海中鱼巣.领域.适配.状态动态退役;

import 海中鱼巣.领域.服务.节点直接状态;
import 海中鱼巣.领域.服务.节点直接动态;

export namespace 海中鱼巣::领域 {

class 状态动态退役适配器 final {
public:
    状态动态退役适配器() = delete;

    static 状态退役规格 规范化状态退役规格(const 状态退役规格&);
    static 动态退役规格 规范化动态退役规格(const 动态退役规格&);
    static bool 复核新域状态身份(const 状态退役规格&);
    static bool 复核新域动态身份(const 动态退役规格&);
};

}
