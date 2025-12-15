

 
export module 任务模块;
import 基础数据类型模块;
import 模板模块;
import 主信息定义模块;

import 世界树模块;
import 二次特征模块;
import 需求模块;
import 方法模块;
import 状态模块;


import<cassert>;
import<variant>;
import <vector>;
import <string>;
import <cstdint>;  // for int64_t



export class 任务类 :链表模板<高级信息基类*> {
public:
	高级信息节点类* 生成新任务(需求节点类* 需求);
	方法节点类* 方法实例化(存在节点类* 方法执行者,方法节点类* 方法);
	int64_t  任务优先级评估(高级信息节点类* 任务);
	int64_t  任务可行度评估(高级信息节点类* 任务);
	void 任务执行(高级信息节点类* 任务);

};

void 任务类::任务执行(高级信息节点类* 任务)
{
//	if (任务 == nullptr) 线程消息.发送({ 消息类型::空任务,std::vector<std::variant<int64_t, std::string, std::vector<std::string>, void*>>()});
//	if (!任务->主信息->可执行) 线程消息.发送({ 消息类型::任务不可执行,std::vector<std::variant<int64_t, std::string, std::vector<std::string>, void*>>() });
//	if(任务->主信息->实例化方法==nullptr) 线程消息.发送({ 消息类型::任务无实例化方法,std::vector<std::variant<int64_t, std::string, std::vector<std::string>, void*>>() });

}
