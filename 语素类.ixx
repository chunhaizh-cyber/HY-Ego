// 词模块接口实现：统一词结构、树形组织、词性子节点、多实体引用

 
module;

export module 语素模块;
import <iostream>;
import <random>;
import<cassert>;
import <vector>;
import <string>;
import <cstdint>;  // for int64_t
import <chrono>;   // for steady_clock
import <unordered_map>;
import 模板模块; // 假设提供了节点模板和链表模板
import 主信息定义模块;
import 基础数据类型模块;
//import 世界树模块;


// 词类：提供添加词、查找词、管理词性子节点的接口
export class 语素类 {
public:
    ~语素类() = default;
    语素类() {}
    // 为某个场景中的基础信息，绑定一个词和词性
    using 节点类 = 链表模板<语素基类*>::节点类;

    词节点类* 生成语素节点(结构体_分词 分词信息);
    // =============== 新增/补全：添加词（若已存在则直接返回原节点） ===============
    // 便捷重载：支持 const 引用
    词节点类* 添加词(const std::string& 词字符串_const);

   词节点类* 查找词节点(const std::string& 词字符串);
      
   词节点类* 获取词性词指针(const std::string& 词字符串);

   词节点类* 查找词性节点(词节点类* 词节点, 枚举_词性 词性值);
    词节点类* 查找词性节点(节点类* 词节点, std::string 词性值);
    词节点类* 查找词性节点(结构体_分词 分词信息);

    // 在词节点中添加一个词性子节点
    词节点类* 添加词性词(词节点类* 词节点, 枚举_词性 词性); 
    词节点类* 添加词性词(std::string 词值, std::string 词性值);
    词节点类* 添加词性词(结构体_分词 词信息);
    //////////////////////////////////////////////////////////////////////////////////////////////////
    词性节点类* 绑定词性词到场景基础信息(const std::string& 词字符串, 枚举_词性 词性, 场景节点类* 场景, 基础信息节点类* 基础信息节点);
    // 通过“词 + 场景(+可选词性)”找到更合适的基础信息
    std::vector<基础信息节点类*> 通过词和场景查找基础信息(const std::string& 词字符串,场景节点类* 当前场景,std::optional<枚举_词性> 词性 = std::nullopt)
    {
        std::vector<基础信息节点类*> 结果;

        词节点类* 词节点 = 查找词节点(词字符串);
        if (!词节点) return 结果;

        // 遍历该词的所有词性节点
        词性节点类* 词性根节点 = 词节点->子;
        词性节点类* 游标 = 词性根节点;
        do {
            词性主信息类* 主信息 = dynamic_cast<词性主信息类*>(游标->主信息);
            if (!主信息) continue;

            if (词性.has_value() && 主信息->词性 != *词性) {
                continue;
            }

            // 关键：让词性主信息根据“当前场景”选最合适的基础信息
            基础信息节点类* 基础 = 主信息->在场景中查找基础信息(当前场景);
            if (基础) {
                结果.push_back(基础);
            }
            游标 = 游标->下;
        } while (游标 != 词性根节点);     
        return 结果;
    }
    
    词节点类* 添加二次特征词(语素节点类* 参照物, 语素节点类* 比较对象, 语素节点类* 比较类型);
    int64_t 添加对应基础信息(词性节点类* 词节点, 基础信息节点类* 基础信息);
    std::string 获取词(词性节点类* 词性词指针);
private:
    // 纯查找：存在则返回节点，不存在返回 nullptr（不会创建）
    词节点类* 查找词(const std::string& 词字符串);
  
    枚举_主信息类型 映射_词性到主信息类型(枚举_词性 词性);
    
};



export class 枚举_词性_工厂 {
private:
    // 静态映射表：文本到枚举值（用于反向查找）
static const std::unordered_map<std::string, 枚举_词性> 文本到枚举映射表;

    // 动态映射表：允许运行时添加新的映射关系
    static std::unordered_map<枚举_词性, std::string> 枚举到文本动态映射表;

    // 初始化静态映射表
    static void 初始化静态映射表();

public:
    // 混合方案核心函数
    static std::string 根据枚举类型获取文本(枚举_词性 词性值) {
        // 首先尝试switch语句（最高优先级）
        switch (词性值) {
            // 基础词性 - 使用详细中文描述
        case 枚举_词性::名词: return "名词";
        case 枚举_词性::动词: return "动词";
        case 枚举_词性::形容词: return "形容词";
        case 枚举_词性::副词: return "副词";
        case 枚举_词性::连词: return "连词";
        case 枚举_词性::介词: return "介词";
        case 枚举_词性::代词: return "代词";
        case 枚举_词性::数词: return "数词";
        case 枚举_词性::量词: return "量词";
        case 枚举_词性::助词: return "助词";
        case 枚举_词性::叹词: return "叹词";
        case 枚举_词性::拟声词: return "拟声词";
        case 枚举_词性::标点符号: return "标点符号";
        case 枚举_词性::语气词: return "语气词";
        case 枚举_词性::外文字符: return "外文字符";
        case 枚举_词性::未定义: return "未定义";

            // 专有名词相关
        case 枚举_词性::专有名词: return "专有名词";
        case 枚举_词性::抽象名词: return "抽象名词";

            // 其他重要词性
        case 枚举_词性::时间词: return "时间词";
        case 枚举_词性::方位词: return "方位词";
        case 枚举_词性::状态词: return "状态词";
        case 枚举_词性::习用语: return "习用语";
        case 枚举_词性::简称略语: return "简称略语";
        case 枚举_词性::非语素字: return "非语素字";
        case 枚举_词性::地名: return "地名";

        default: break; // 不在switch中的枚举值，继续后续查找
        }

        // 其次尝试动态映射表（中等优先级）
        auto 动态查找 = 枚举到文本动态映射表.find(词性值);
        if (动态查找 != 枚举到文本动态映射表.end()) {
            return 动态查找->second;
        }

        // 最后返回默认值
        return "未知词性";
    }

    // 反向查找：文本到枚举值
    static 枚举_词性 根据文本获取枚举值(const std::string& 文本) {
        auto it = 文本到枚举映射表.find(文本);
        return (it != 文本到枚举映射表.end()) ? it->second : 枚举_词性::未定义;
    }

    // 动态添加映射关系（运行时扩展）
    static void 添加动态映射(枚举_词性 词性值, const std::string& 文本描述) {
        枚举到文本动态映射表[词性值] = 文本描述;
    }

    // 批量添加动态映射
    static void 批量添加动态映射(const std::vector<std::pair<枚举_词性, std::string>>& 映射列表) {
        for (const auto& 映射项 : 映射列表) {
            添加动态映射(映射项.first, 映射项.second);
        }
    }

    // 清除动态映射（用于重置或更新）
    static void 清除动态映射() {
        枚举到文本动态映射表.clear();
    }
};

void 枚举_词性_工厂::初始化静态映射表()
{
}
