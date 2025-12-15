export module 方法模块;
import 基础数据类型模块;
import 模板模块;
import 主信息定义模块;

import 世界树模块;
import 二次特征模块;
import 需求模块;
import 状态模块;
import 自然语言树模块;


import<cassert>;
import <vector>;
import <string>;
import <cstdint>;  // for int64_t

import <string>;
import <vector>;
import <map>;
import <memory>;
import <optional>;
import <functional>;

import 主信息定义模块;       // 场景/存在/特征/状态/动态节点别名 & 高级信息基类/比较字段/基础方法枚举
//import 外部方法总接口;       // 外部方法中心、方法回包/结果模板 等
import 内部方法总接口;       // 内部方法中心

export namespace 方法模块 {
	export class 方法类 :链表模板<高级信息基类*> {
	public:
		方法节点类* 添加方法(方法节点类* 头节点, 方法节点类* 结果节点);
		方法节点类* 添加结果(方法节点类* 头节点, 方法节点类* 结果节点);
		方法节点类* 查找方法(自然句节点类* 方法描述);
		方法节点类* 执行本能方法(自然句节点类* 方法描述);
	};

}

方法模块::方法类 方法集;