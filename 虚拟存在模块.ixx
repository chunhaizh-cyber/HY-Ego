module;

#include <windows.h>

// VirtualExistenceModule_v5.ixx
// 收敛目标：虚拟存在使用链表模板的“添加/删除”来保证主键唯一与结构一致性。
// 共识更新：特征值节点复用全局 特征值链（特征值环境模块::特征值集），虚拟存在不托管特征值节点生命周期。
// 因此：
// - 释放_拥有：只需要删除“虚拟存在树的节点结构”（存在/特征等），不删除特征值节点
// - 特征节点主信息中引用的 当前值/值列表 只是指针引用，删除特征节点时无需释放这些值节点

#include <string>
#include <string_view>
#include <sstream>
#include <cstdint>
export module 虚拟存在模块;

import 主信息定义模块;
import 语素环境模块;
import 日志模块;



namespace 虚拟存在模块 {  

  

  
}

