
 
export module 动作模块;

import <stdexcept>;
import <vector>;
import <utility>;
import <atlbase.h>; // 用于CString
import <iostream>;
import <vector>;
import <stdexcept>;
import <string>;
import <cstdint>;
// import 全局模块;
import 方法模块;
import 主信息定义模块;
import 语素模块;
import 基础数据类型模块;

export class 基础动作类 {
public:
    
    std::int64_t 执行方法(方法节点类* 方法);


private:
private:
    // 将NLP客户端作为成员变量避免重复构造

    枚举_主信息类型 映射_词性到主信息类型(枚举_词性 词性)
    {
        switch (词性)
        {
        case 枚举_词性::名词:
        case 枚举_词性::专有名词:
            return 枚举_主信息类型::存在信息;

        case 枚举_词性::代词:
            return 枚举_主信息类型::存在信息_指代;

        case 枚举_词性::动词:
            return 枚举_主信息类型::动态信息;

        case 枚举_词性::形容词:
            return 枚举_主信息类型::特征信息;

        case 枚举_词性::副词:
            return 枚举_主信息类型::二次特征_修饰;

        case 枚举_词性::数词:
        case 枚举_词性::量词:
            return 枚举_主信息类型::特征信息_数量;

        case 枚举_词性::介词:
        case 枚举_词性::连词:
            return 枚举_主信息类型::关系信息;
        case 枚举_词性::助词:
        case 枚举_词性::语气词:
            return 枚举_主信息类型::情绪调节信息;

        default:
            return 枚举_主信息类型::语言信息_仅记录;
        }
    }
 //   
 //   存在节点主信息类* 生成_存在主信息(const 结构体_分词& 分词)
 //   {
 //       词性节点类* 词节点指针 = 全局变量::语素集.添加词性词(分词.词, 分词.词性);
 //       词性节点类* 词性节点指针= 全局变量::语素集.添加词性词(分词.词性, "抽象名词");
 //       auto* 节点 = new 存在节点主信息类();
 //       节点->名称 = 词节点指针;      // 指向“苹果”“桌子”这种词性词
 //     //  节点->来源 = 枚举_基础信息来源::自然语言;
 //      
 //       // 其他：可以初始化一个空的子链，用于未来挂特征/子存在
 //       return 节点;
 //   }
 //

   



    // 复杂方法头
    struct 结构体_自然语言理解输出 {
        场景节点类* 临时场景;
        double 全局置信度;
        std::vector<基础信息节点类*> 未决分支;
        bool 是否提交;
    };
   // 结构体_自然语言理解输出 方法_自然语言理解_复杂(const std::string& 文本, 场景节点类* 当前场景, /* 可选：历史上下文句柄 */);
  //  结构体_自然语言理解输出 方法_自然语言理解_复杂(const std::string& 文本,场景节点类* 当前场景);

    // 典型简单方法
    bool 方法_分词(const std::string& 文本, std::vector<语素节点类*>& 输出语素);
    bool 方法_词性标注(const std::vector<语素节点类*>& 语素, std::vector<词性节点类*>& 输出词性);
    bool 方法_短语句法(const std::vector<词性节点类*>& 词性, 短语节点类*& 输出短语根);
    bool 方法_语素落地映射(短语节点类* 短语根, 场景节点类* 临时场景);
    bool 方法_指代消解(场景节点类* 临时场景);
    bool 方法_时间体貌解析(场景节点类* 临时场景);
    bool 方法_生成二次特征(场景节点类* 临时场景);
    bool 方法_场景对齐与合并(场景节点类* 临时场景, 场景节点类* 当前场景);
    bool 方法_冲突与置信度融合(场景节点类* 临时场景, double& 全局置信度);
    bool 方法_提交策略(场景节点类* 临时场景, double 全局置信度, bool& 是否提交);
















  
};



bool 基础动作类::方法_分词(const std::string& 文本, std::vector<语素节点类*>& 输出语素)
{
    return false;
}

bool 基础动作类::方法_词性标注(const std::vector<语素节点类*>& 语素, std::vector<词性节点类*>& 输出词性)
{
    return false;
}

bool 基础动作类::方法_短语句法(const std::vector<词性节点类*>& 词性, 短语节点类*& 输出短语根)
{
    return false;
}

bool 基础动作类::方法_语素落地映射(短语节点类* 短语根, 场景节点类* 临时场景)
{
    return false;
}

bool 基础动作类::方法_指代消解(场景节点类* 临时场景)
{
    return false;
}

bool 基础动作类::方法_时间体貌解析(场景节点类* 临时场景)
{
    return false;
}

bool 基础动作类::方法_生成二次特征(场景节点类* 临时场景)
{
    return false;
}

bool 基础动作类::方法_场景对齐与合并(场景节点类* 临时场景, 场景节点类* 当前场景)
{
    return false;
}

bool 基础动作类::方法_冲突与置信度融合(场景节点类* 临时场景, double& 全局置信度)
{
    return false;
}

bool 基础动作类::方法_提交策略(场景节点类* 临时场景, double 全局置信度, bool& 是否提交)
{
    return false;
}
