export module 因果信息模块;

import<cassert>;
import <vector>;
import <string>;
import <cstdint>;  // for int64_t
import <chrono>;   // for steady_clock

import 模板模块;
import 基础数据类型模块;
import 主信息定义模块;



export class 因果类 :public 链表模板<基础信息基类*> {
public:
	基础信息节点类* 获取因果信息(基础信息节点类* 动态根指针);

};

