export module 节点特征更新模块;

import 基础数据类型模块;
import 主信息定义模块;
import 特征类型定义模块;
import 语素环境模块;
//import 虚拟存在特征原语模块;
import 日志模块;

import <string>;
import <sstream>;
import <cstdint>;
import <optional>;
import <algorithm>;
import <limits>;
import <bit>;
import <cmath>;

// ============================================================
// 节点特征更新模块
// - 目的：把“任务/方法/首节点/条件/结果”的运行态统计，统一写入 世界树.虚拟世界 下的虚拟存在。
// - 注意：
//   1) 只写“统计/调度/诊断”类特征，不进入默认匹配策略表
//   2) 发生缺失特征时按 0 处理（读不到就当 0）
// ============================================================
