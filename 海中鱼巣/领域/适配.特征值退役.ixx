// 文件规则：特征值退役适配只保持特征值对象族的强类型边界，不访问仓库、事务令牌或旧域材料。
module;

#include "节点直接P3合同.数据.h"

export module 海中鱼巣.领域.适配.特征值退役;

import 海中鱼巣.领域.服务.节点直接特征;

export namespace 海中鱼巣::领域 {

class 特征值退役适配器 final {
public:
    特征值退役适配器() = delete;

    static 特征值退役规格 规范化退役规格(const 特征值退役规格&);
    static bool 复核新域退役身份(const 特征值退役规格&);
};

}
