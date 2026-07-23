// 文件规则：自我、存在和场景退役适配只接受具名退役规格，不把成员、槽位或聚合投影变成删除事实。
module;

#include "节点直接P3合同.数据.h"

export module 海中鱼巣.领域.适配.自我存在场景退役;

import 海中鱼巣.领域.服务.节点直接存在;
import 海中鱼巣.领域.服务.节点直接场景;
import 海中鱼巣.领域.组合.节点直接自我读取;
import 海中鱼巣.领域.数据操作.节点直接系统角色;

export namespace 海中鱼巣::领域 {

class 自我存在场景退役适配器 final {
public:
    自我存在场景退役适配器() = delete;

    static 自我结构退役规格 规范化自我结构退役规格(const 自我结构退役规格&);
    static 系统角色退役规格 规范化系统角色退役规格(const 系统角色退役规格&);
    static 存在场景增量退役规格 规范化存在场景增量退役规格(
        const 存在场景增量退役规格&);
    static bool 复核自我存在场景边界(const 自我结构退役规格&);
    static bool 复核系统角色退役边界(const 系统角色退役规格&);
    static bool 复核存在场景增量退役边界(const 存在场景增量退役规格&);
};

}
