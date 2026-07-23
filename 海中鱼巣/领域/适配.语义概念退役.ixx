// 文件规则：语义概念退役适配只消费概念签名和关系 9—12 的强类型材料，不读取活动快照或旧登记数组。
module;

#include "节点直接P3合同.数据.h"

export module 海中鱼巣.领域.适配.语义概念退役;

import 海中鱼巣.领域.数据操作.节点直接概念结构;

export namespace 海中鱼巣::领域 {

class 语义概念退役适配器 final {
public:
    语义概念退役适配器() = delete;

    static 语义概念退役规格 规范化语义概念退役规格(const 语义概念退役规格&);
    static bool 复核概念签名和关系分账(const 语义概念退役规格&);
};

}
