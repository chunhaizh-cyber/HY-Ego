// 文件规则：概念退役服务只形成语义概念强类型退役规格并转交 P3D，不维护活动图、缓存或旧概念登记。
module;

#include "节点直接P3合同.数据.h"

export module 海中鱼巣.领域.服务.节点直接概念退役;

import 海中鱼巣.领域.数据操作.节点直接概念结构;
import 海中鱼巣.领域.退役.节点直接对象;

export namespace 海中鱼巣::领域 {

class 节点直接概念退役服务 final {
public:
    节点直接概念退役服务() = delete;

    static 节点直接对象退役结果 退役节点直接概念(
        const 语义概念退役规格& 规格) {
        return 节点直接对象退役服务::退役节点直接对象(规格);
    }
};

}
