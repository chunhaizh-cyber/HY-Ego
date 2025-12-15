

export module 主信息定义模块;

import <string>;
import <vector>;
import <chrono>;
import <limits>;
import <algorithm>;
import <stdexcept>;
import <compare>;
import <unordered_set>;
import <variant>;
import <functional>;
import <cstdint>;
import <bit>;       // std::popcount

import 模板模块;
import 基础数据类型模块;


//======================================================================
// 一、语素族：词 / 词性 / 短语 / 短语子
//======================================================================

export class 语素基类;
export class 特征值基类;
export class 基础信息基类;
export class 高级信息基类;
export class 自然语言成分基类;

export class 词主信息类;
export class 词性主信息类;
export class 短语主信息类;
export class 短语子节点主信息类;

//======================================================================
// 三、基础信息族（上移名称/类型到基类）：指代 / 特征 / 存在 / 场景 / 状态 / 动态 / 二次特征 / 因果
//======================================================================
   
export class 矢量特征值节点主信息类;
export class 非矢量特征值节点主信息类;


export class 指代节点主信息类;
export class 特征节点主信息类;
export class 存在节点主信息类;
export class 场景节点主信息类;
export class 状态节点主信息类;
export class 动态节点主信息类;
export class 二次特征主信息类;
export class 因果主信息类;
//======================================================================
// 四、自然语言成分族：基本句 / 复杂句 / 段落 / 文章
//======================================================================
export class 基本句子主信息类;
export class 复杂句子主信息类;
export class 段落主信息类;
export class 文章主信息类;

export using 特征值节点类 = 链表模板<特征值基类*>::节点类;

export using 基础信息节点类 = 链表模板<基础信息基类*>::节点类;
export using 特征节点类 = 链表模板<基础信息基类*>::节点类;
export using 存在节点类 = 链表模板<基础信息基类*>::节点类;
export using 场景节点类 = 链表模板<基础信息基类*>::节点类;
export using 状态节点类 = 链表模板<基础信息基类*>::节点类;
export using 动态节点类 = 链表模板<基础信息基类*>::节点类;
export using 二次特征节点类 = 链表模板<基础信息基类*>::节点类;
export using 因果节点类 = 链表模板<基础信息基类*>::节点类;

export using 语素节点类 = 链表模板<语素基类*>::节点类;
export using 词节点类 = 链表模板<语素基类*>::节点类;
export using 词性节点类 = 链表模板<语素基类*>::节点类;
export using 短语节点类 = 链表模板<语素基类*>::节点类;

export using 子句节点类 = 链表模板<自然语言成分基类*>::节点类;
export using 自然句节点类 = 链表模板<自然语言成分基类*>::节点类;
export using 段落节点类 = 链表模板<自然语言成分基类*>::节点类;
export using 文章节点类 = 链表模板<自然语言成分基类*>::节点类;

// 基础信息节点通用别名（世界树专用）
export using 基础信息节点类 = 链表模板<基础信息基类*>::节点类;
export using 语素节点类 = 链表模板<语素基类*>::节点类;


export class 语素基类 {
public:
    virtual ~语素基类() = default;  

  virtual  std::int64_t  比较(语素基类* 对象, 枚举_比较字段 字段,枚举_比较条件 条件) const = 0;
};
export class 词主信息类 : public 语素基类 {
public:
    std::string 词;

    explicit 词主信息类(const std::string& w = "") : 词(w) {}

    std::int64_t 比较(语素基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const override {
      
        词主信息类* 词主信息 = dynamic_cast<词主信息类*>(对象);
        switch (字段) {
        case 枚举_比较字段::语素_词_词:
        {
            if (条件 == 枚举_比较条件::相等)
                return this->词 == 词主信息->词;
            if (条件 == 枚举_比较条件::不等于)
                return this->词 != 词主信息->词;
        }
        default: break;
        }
       return -1;
    }  
private:
    std::unordered_set<枚举_词性> 已存在词性集合;

public:
    bool 已存在词性(枚举_词性 词性) const {
        return 已存在词性集合.find(词性) != 已存在词性集合.end();
    }

    void 添加词性到集合(枚举_词性 词性) {
        已存在词性集合.insert(词性);
    }
        void 删除词性在集合(枚举_词性 词性) {
            已存在词性集合.erase(词性);
        }
    
};

export struct 结构体_词场景示例 {
    场景节点类* 场景指针 = nullptr;
    基础信息节点类* 基础信息指针 = nullptr;
};
export class 词性主信息类 : public 语素基类 {
public:
    枚举_词性 词性;                         // 所有词性词用词指针
    基础信息节点类* 对应基础信息指针 = nullptr; // 指向世界树中的默认基础信息节点
    std::vector<结构体_词场景示例> 示例列表;
    词性主信息类() = default;
    explicit 词性主信息类(枚举_词性 词性_) : 词性(词性_) {} 

    void 添加场景示例(场景节点类* 场景, 基础信息节点类* 基础信息) {
        if (!场景 || !基础信息) return;

        // 避免重复
        for (const auto& 示例 : 示例列表) {
            if (示例.场景指针 == 场景 && 示例.基础信息指针 == 基础信息) {
                return;
            }
        }

        示例列表.push_back({ 场景, 基础信息 });

        // 如果“默认基础信息”还没设，可以用第一个示例填上
        if (!对应基础信息指针) {
            对应基础信息指针 = 基础信息;
        }
    }

    // 【新增】根据场景查找更合适的基础信息（简单版本）
    基础信息节点类* 在场景中查找基础信息(场景节点类* 当前场景) const {
        if (!当前场景) return 对应基础信息指针;

        // 1. 优先找场景精确匹配的示例
        for (const auto& 示例 : 示例列表) {
            if (示例.场景指针 == 当前场景) {
                return 示例.基础信息指针;
            }
        }

        // 2. TODO：可以扩展为“考虑上级场景/相似场景”等逻辑

        // 3. 找不到就退回默认
        return 对应基础信息指针;
    }
    std::int64_t 比较(语素基类* 对象,枚举_比较字段 字段,枚举_比较条件 条件) const override {
        auto* 主信息 = dynamic_cast<词性主信息类*>(对象);
        if (!主信息) {
            throw std::invalid_argument("词性主信息类:比较函数,比较对象类型错误");
        }

        switch (字段) {
        case 枚举_比较字段::语素_词性_词性:
        {
            if (条件 == 枚举_比较条件::相等) {
                return (this->词性 == 主信息->词性) ? 1 : 0;
            }
            else if (条件 == 枚举_比较条件::不等于) {
                return (this->词性 != 主信息->词性) ? 1 : 0;
            }
            break;
        }
        case 枚举_比较字段::语素_词性_对应基础信息指针:
        {
            if (条件 == 枚举_比较条件::相等) {
                return (this->对应基础信息指针 == 主信息->对应基础信息指针) ? 1 : 0;
            }
            else if (条件 == 枚举_比较条件::不等于) {
                return (this->对应基础信息指针 != 主信息->对应基础信息指针) ? 1 : 0;
            }
            break;
        }
        default:
            break;
        }

        // 未处理的字段 / 条件，返回一个约定的 “无法比较” 值
        return -1;
    }
};
export class 短语子节点主信息类 : public 语素基类 {
public:
    词节点类* 参照词 = nullptr;
    词节点类* 比较对象词 = nullptr;
    词节点类* 比较词 = nullptr;
    词节点类* 结果词 = nullptr;


    短语子节点主信息类() = default;
    短语子节点主信息类(词节点类* 参照词_, 词节点类* 比较对象词_, 词节点类* 比较词_, 词节点类* 结果词_) : 参照词(参照词_), 比较词(比较词_), 比较对象词(比较对象词_), 结果词(结果词_) {}
    std::int64_t 比较(语素基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const override {

        短语子节点主信息类* 主信息 = dynamic_cast<短语子节点主信息类*>(对象);
        if (!主信息) {
            throw std::invalid_argument("短语子节点主信息类:比较函数,比较对象类型错误");
            switch (字段) {
            case 枚举_比较字段::语素_短语字节点_参照词:
            {
                if (条件 == 枚举_比较条件::相等) {
                    return this->参照词 == 主信息->参照词;
                }
                else if (条件 == 枚举_比较条件::不等于) {
                    return this->参照词 != 主信息->参照词;
                }
            }
            break;
            case 枚举_比较字段::语素_短语字节点_比较对象词:
            {
                if (条件 == 枚举_比较条件::相等) {
                    return this->比较对象词 == 主信息->比较对象词;
                }
                else if (条件 == 枚举_比较条件::不等于) {
                    return this->比较对象词 != 主信息->比较对象词;
                }
            }
            break;
            case 枚举_比较字段::语素_短语子节点_比较词:
            {
                if (条件 == 枚举_比较条件::相等)
                    return this->比较词 == 主信息->比较词;
                if (条件 == 枚举_比较条件::不等于)
                    return this->比较词 != 主信息->比较词;
            }
            break;
            case 枚举_比较字段::语素_短语子节点_结果词:
            {
                if (条件 == 枚举_比较条件::相等)
                    return this->结果词 == 主信息->结果词;
                if (条件 == 枚举_比较条件::不等于)
                    return this->结果词 != 主信息->结果词;
            }
            default: break;
            }
            return -1;
        }
    }
   
};


//======================================================================
// 二、特征值族：矢量/非矢量
//======================================================================


export class 特征值基类 {
public:
    枚举_特征值类型       类型 = 枚举_特征值类型::未定义;
    枚举_特征值比较模式   比较模式 = 枚举_特征值比较模式::有序标量;

    virtual ~特征值基类() = default;

    特征值基类() = default;
    explicit 特征值基类(枚举_特征值类型 类型_, 枚举_特征值比较模式 模式_ = 枚举_特征值比较模式::有序标量)
        : 类型(类型_), 比较模式(模式_) {
    }

    // 统一比较接口（之前已经有）
    virtual std::int64_t 比较(特征值基类* 对象,
        枚举_比较字段 字段,
        枚举_比较条件 条件) const = 0;

    // ★ 新增：尝试把特征值转换为 double 标量，用于“有序标量”的场景（特征概念树里的区间概念）
    // 返回 true 表示 out 中填入了一个有效数值；false = 这个值不是有序标量。
    virtual bool 转为标量(double& out) const {
        // 默认实现：不是有序标量
        (void)out;
        return false;
    }
};


// 内部小工具（不导出）
namespace
{
    inline std::int64_t 比较_整数(std::int64_t 左, std::int64_t 右, 枚举_比较条件 条件)
    {
        switch (条件) {
        case 枚举_比较条件::相等:
            return 左 == 右 ? 1 : 0;
        case 枚举_比较条件::不等于:
            return 左 != 右 ? 1 : 0;
        case 枚举_比较条件::大于:
            return 左 > 右 ? 1 : 0;
        case 枚举_比较条件::小于:
            return 左 < 右 ? 1 : 0;
        case 枚举_比较条件::大于等于:
            return 左 >= 右 ? 1 : 0;
        case 枚举_比较条件::小于等于:
            return 左 <= 右 ? 1 : 0;
        default:
            return -1;
        }
    }

    // 指针比较（主要用于“单位”等指向词性节点的字段）
    template<typename T>
    inline std::int64_t 比较_指针(T* 左, T* 右, 枚举_比较条件 条件)
    {
        switch (条件) {
        case 枚举_比较条件::相等:
            return 左 == 右 ? 1 : 0;
        case 枚举_比较条件::不等于:
            return 左 != 右 ? 1 : 0;
        default:
            return -1;
        }
    }

    inline bool 向量完全相等(const std::vector<std::int64_t>& a,
        const std::vector<std::int64_t>& b)
    {
        if (a.size() != b.size()) return false;
        for (std::size_t i = 0; i < a.size(); ++i) {
            if (a[i] != b[i]) return false;
        }
        return true;
    }
} // unnamed namespace

export class 标量特征值主信息类 : public 特征值基类 {
public:
    词性节点类* 单位 = nullptr;
    std::int64_t 值 = 0;

    标量特征值主信息类()
        : 特征值基类(枚举_特征值类型::标量, 枚举_特征值比较模式::有序标量)
    {
    }

    标量特征值主信息类(std::int64_t v)
        : 特征值基类(枚举_特征值类型::标量, 枚举_特征值比较模式::有序标量),
        值(v)
    {
    }

    标量特征值主信息类(词性节点类* 单位_, std::int64_t v)
        : 特征值基类(枚举_特征值类型::标量, 枚举_特征值比较模式::有序标量),
        单位(单位_), 值(v)
    {
    }

    std::int64_t 比较(特征值基类* 对象,枚举_比较字段 字段,枚举_比较条件 条件) const override
    {
        auto* 主信息 = dynamic_cast<标量特征值主信息类*>(对象);
        if (!主信息) {
            throw std::invalid_argument("标量特征值主信息类::比较 - 对象类型错误");
        }
            return 比较_整数(this->值, 主信息->值, 条件);        
    }

    bool 转为标量(double& out) const override {
        out = static_cast<double>(值);
        return true;
    }
};

// ----------------------------------------------------------------------
// 矢量特征值：多个 int64，只做相等 / 相似度
// 说明：
//   - 这里的“矢量”在你的规则里，就是“不能比较大小”的那类特征值
//   - 比如：轮廓描述、坐标向量、embedding、编码向量等
//----------------------------------------------------------------------

export class 矢量特征值主信息类 : public 特征值基类 {
public:
    std::vector<std::int64_t> 值;

    矢量特征值主信息类()
        : 特征值基类(枚举_特征值类型::矢量, 枚举_特征值比较模式::相等或相似度)
    {
    }

    explicit 矢量特征值主信息类(std::vector<std::int64_t> v)
        : 特征值基类(枚举_特征值类型::矢量, 枚举_特征值比较模式::相等或相似度),
        值(v)
    {
    }

    std::size_t 维度() const noexcept { return 值.size(); }

    std::int64_t 比较(特征值基类* 对象,
        枚举_比较字段 /*字段*/,
        枚举_比较条件 条件) const override
    {
        auto* 右 = dynamic_cast<矢量特征值主信息类*>(对象);
        if (!右) {
            throw std::invalid_argument("矢量特征值主信息类::比较 - 对象类型错误");
        }

        // 当前实现遵守你的原则：
        //   - 不能比较大小 => 不支持 > / < ，只支持 == / !=
        switch (条件) {
        case 枚举_比较条件::相等:
            return 向量完全相等(this->值, 右->值) ? 1 : 0;
        case 枚举_比较条件::不等于:
            return 向量完全相等(this->值, 右->值) ? 0 : 1;
        default:
            // 其它条件不支持（> < >= <=），留给“相似度方法/二次特征”去做
            return -1;
        }
    }

    // 将来如果要做“相似度”，可以额外写一个非虚函数：
    //
    // double 计算相似度(const 矢量特征值主信息类& 另一) const;
    //
    // 由不同的“矢量语义类型”（轮廓/坐标/颜色/embedding）选不同相似度算法，
    // 放在“特征抽象策略”那里实现，这里先保持简单。
};


//----------------------------------------------------------------------
// 字符特征值：字符串编码（代表某个存在 / 概念 / 特征）
// 规则：
//   - 存储层只知道字符串本身
//   - 比较层默认只实现 == / !=
//   - 真正的“聚类 / 概念生长”通过“解码 -> 再按被指向对象聚类”来做
//----------------------------------------------------------------------

export class 字符特征值主信息类 : public 特征值基类 {
public:
    // 这里用 std::string，表示已转换为 UTF-8 的编码；
    // 如果你希望保持 UTF-16/UTF-32，可以改成 std::wstring / std::u32string。
    std::string 值;

    字符特征值主信息类()
        : 特征值基类(枚举_特征值类型::字符串, 枚举_特征值比较模式::相等或相似度)
    {
    }

    explicit 字符特征值主信息类(std::string v)
        : 特征值基类(枚举_特征值类型::字符串, 枚举_特征值比较模式::相等或相似度),
        值(v)
    {
    }

    std::int64_t 比较(特征值基类* 对象,
        枚举_比较字段 /*字段*/,
        枚举_比较条件 条件) const override
    {
        auto* 右 = dynamic_cast<字符特征值主信息类*>(对象);
        if (!右) {
            throw std::invalid_argument("字符特征值主信息类::比较 - 对象类型错误");
        }

        switch (条件) {
        case 枚举_比较条件::相等:
            return 值 == 右->值 ? 1 : 0;
        case 枚举_比较条件::不等于:
            return 值 != 右->值 ? 1 : 0;
        default:
            // 字符串默认也不做大小比较；如果你想支持字典序比较，可以在这里扩展
            return -1;
        }
    }
};



export class 基础信息基类 {
public:
    // ⬇⬇ 统一上移：名称 / 类型
    词性节点类* 名称 = nullptr;
    词性节点类* 类型 = nullptr;
    virtual ~基础信息基类() = default; 
    virtual  std::int64_t  比较(基础信息基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const = 0;
protected:
    基础信息基类() = default;
    explicit 基础信息基类(词性节点类* 名) : 名称(名){}
    explicit 基础信息基类(词性节点类* 名, 词性节点类* 型) : 名称(名), 类型(型) {}
};



export struct 结构体_相对坐标 {
    结构体_空间坐标   坐标偏移{};
    基础信息节点类* 参照物 = nullptr;
    friend bool operator==(const 结构体_相对坐标& a, const 结构体_相对坐标& b) {
        return a.坐标偏移 == b.坐标偏移 && a.参照物 == b.参照物;
    }
};

//—— 指代
export class 指代节点主信息类 : public 基础信息基类 {
public:
    词性节点类* 代词 = nullptr;
    存在节点类* 指代对象 = nullptr;

    指代节点主信息类() = default;
    指代节点主信息类(词性节点类* 名, 词性节点类* 型, 词性节点类* 代, 存在节点类* 对象)
        : 基础信息基类(名, 型), 代词(代), 指代对象(对象) {    }

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const override {
        指代节点主信息类* 主信息 = dynamic_cast<指代节点主信息类*>(对象);
        if (!主信息) {
            throw std::invalid_argument("指代节点主信息类:比较函数,比较对象类型错误");
            switch (字段) {
            case 枚举_比较字段::基础信息基类_名称:
                if (条件 == 枚举_比较条件::相等) {
                    return this->名称 == 主信息->名称;
                }
                else if (条件 == 枚举_比较条件::不等于) {
                    return this->名称 != 主信息->名称;
                }
                break;
            case 枚举_比较字段::基础信息基类_类型:

                if (条件 == 枚举_比较条件::相等) {
                    return this->类型 == 主信息->类型;
                }
                else if (条件 == 枚举_比较条件::不等于) {
                    return this->类型 != 主信息->类型;
                }
                break;
            case 枚举_比较字段::基础信息_指代节点_代词:
                if (条件 == 枚举_比较条件::相等) {
                    return this->代词 == 主信息->代词;
                }
                else if (条件 == 枚举_比较条件::不等于) {
                    return this->代词 != 主信息->代词;
                }
                break;
            case 枚举_比较字段::基础信息_指代节点_指代对象:
            {
                if (条件 == 枚举_比较条件::相等) {
                    return this->指代对象 == 主信息->指代对象;
                }
                else if (条件 == 枚举_比较条件::不等于) {
                    return this->指代对象 != 主信息->指代对象;
                }
            }
            return -1;
            }
        }
    }
  
};

export class 特征节点主信息类 : public 基础信息基类 {
public:
    特征值节点类* 值 = nullptr;
    特征节点主信息类() = default;
    // 便捷构造：仅类型 + 值
    explicit 特征节点主信息类(词性节点类* 特征类型, 特征值节点类* 特征值) {
        this->类型 = 特征类型;
        this->值 = 特征值;
    }
    // 完整构造：名称 + 类型 + 值
    特征节点主信息类(词性节点类* 名, 词性节点类* 型, 特征值节点类* 特征值)
        : 基础信息基类(名, 型), 值(特征值) {
    }
    
    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const override {
        特征节点主信息类* 主信息 = dynamic_cast<特征节点主信息类*>(对象);
        if (!主信息) {
            throw std::invalid_argument("特征节点主信息类:比较函数,比较对象类型错误");
            switch (字段) {
            case 枚举_比较字段::基础信息_特征节点_名称:
                if (条件 == 枚举_比较条件::相等) {
                    return this->名称 == 主信息->名称;
                }
                else if (条件 == 枚举_比较条件::不等于) {
                    return this->名称 != 主信息->名称;
                }
                break;
            case 枚举_比较字段::基础信息_特征节点_类型:

                if (条件 == 枚举_比较条件::相等) {
                    return this->类型 == 主信息->类型;
                }
                else if (条件 == 枚举_比较条件::不等于) {
                    return this->类型 != 主信息->类型;
                }
                break;
            case 枚举_比较字段::基础信息_特征节点_值:
                if (条件 == 枚举_比较条件::相等) {
                    return this->值 == 主信息->值;
                }
                else if (条件 == 枚举_比较条件::不等于) {
                    return this->值 != 主信息->值;
                }    
            }
            return -1;
            
        }
    }
};


//—— 存在（仅保留：绝对坐标 / 相对坐标 / 轮廓）
//—— 存在（只保留“身份信息”，坐标/轮廓下放到子特征）
export class 存在节点主信息类 : public 基础信息基类 {
public:
    // 可选：存在的唯一 ID 和创建时间（如果暂时不用，也可以先不管）
    场景节点类* 内部世界 = nullptr;
    时间戳       创建时间 = 0;
    std::string 取存在ID(const 存在节点类* 节点) const {
        return 节点->主键 ;
    }
    // 当前“代表性特征”的快捷索引：指向挂在该存在节点下面的特征节点
    std::vector<特征节点类*> 特征当前状态;

    存在节点主信息类() = default;

    explicit 存在节点主信息类(词性节点类* 名)
        : 基础信息基类(名) {
    }

    存在节点主信息类(词性节点类* 名, 词性节点类* 型)
        : 基础信息基类(名, 型) {
    }

    存在节点主信息类(词性节点类* 名,
        词性节点类* 型,        
        时间戳        创建时间_ = 0)
        : 基础信息基类(名, 型),       
        创建时间(创建时间_) {
    }

    // 工具：快速获取某维度当前特征（维度名通常对应“特征类型”）
    特征节点类* 获取当前特征(词性节点类* 维度名) const {
        if (!维度名) return nullptr;
        for (auto* f : 特征当前状态) {
            if (!f || !f->主信息) continue;
            if (f->主信息->类型 == 维度名) {
                return f;
            }
        }
        return nullptr;
    }

    std::int64_t 比较(基础信息基类* 对象,
        枚举_比较字段 字段,
        枚举_比较条件 条件) const override {
        auto* 主信息 = dynamic_cast<存在节点主信息类*>(对象);
        if (!主信息) {
            throw std::invalid_argument("存在节点主信息类:比较函数, 比较对象类型错误");
        }

        switch (字段) {
        case 枚举_比较字段::基础信息基类_名称:
            if (条件 == 枚举_比较条件::相等)
                return this->名称 == 主信息->名称 ? 1 : 0;
            if (条件 == 枚举_比较条件::不等于)
                return this->名称 != 主信息->名称 ? 1 : 0;
            return -1;

        case 枚举_比较字段::基础信息基类_类型:
            if (条件 == 枚举_比较条件::相等)
                return this->类型 == 主信息->类型 ? 1 : 0;
            if (条件 == 枚举_比较条件::不等于)
                return this->类型 != 主信息->类型 ? 1 : 0;
            return -1;

        default:
            // 绝对坐标 / 相对坐标 / 轮廓等比较，全部下放到“特征节点”去做
            return -1;
        }
    }
};



//—— 场景（仅保留：状态列表 / 动态列表 / 关系列表）
export class 场景节点主信息类 : public 基础信息基类 {
public:
    存在节点类* 宿主存在 = nullptr;   // 普通世界则为 nullptr
    std::vector<状态节点类*>      状态列表;
    std::vector<动态节点类*>      动态列表;
    std::vector<二次特征节点类*>  关系列表;
    // 新增：这个场景属于哪种“世界”
    枚举_世界类型 世界类型 = 枚举_世界类型::现实世界;

    // 可选：来源文章、来源描述等
    文章节点类* 来源文章 = nullptr;
    场景节点主信息类() = default;
 
    explicit 场景节点主信息类(词性节点类* 名, 枚举_世界类型 型)
        : 基础信息基类(名), 世界类型(型){
    }

    场景节点主信息类(词性节点类* 名, 词性节点类* 型,
        std::vector<状态节点类*> s,
        std::vector<动态节点类*> d = {},
        std::vector<二次特征节点类*> r = {})
        : 基础信息基类(名, 型),
        状态列表(s), 动态列表(d), 关系列表(r) {
    }

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const override {
        场景节点主信息类* 主信息 = dynamic_cast<场景节点主信息类*>(对象);
        if (!主信息) {
            throw std::invalid_argument("场景节点主信息类:比较函数,比较对象类型错误");
            switch (字段) {
          case 枚举_比较字段::基础信息基类_名称:
        //       if (条件 == 枚举_比较条件::相等) {
        //           return this->名称 == 主信息->名称;
        //       }
        //       else if (条件 == 枚举_比较条件::不等于) {
        //           return this->名称 != 主信息->名称;
        //       }
        //       break;
        //   case 枚举_比较字段::基础信息基类_类型:
        //
        //       if (条件 == 枚举_比较条件::相等) {
        //           return this->类型 == 主信息->类型;
        //       }
        //       else if (条件 == 枚举_比较条件::不等于) {
        //           return this->类型 != 主信息->类型;
        //       }
        //       break;
        //   case 枚举_比较字段::基础信息_指代节点_代词:
        //       if (条件 == 枚举_比较条件::相等) {
        //           return this->代词 == 主信息->代词;
        //       }
        //       else if (条件 == 枚举_比较条件::不等于) {
        //           return this->代词 != 主信息->代词;
        //       }
        //       break;
        //   case 枚举_比较字段::基础信息_指代节点_指代对象:
        //   {
        //       if (条件 == 枚举_比较条件::相等) {
        //           return this->指代对象 == 主信息->指代对象;
        //       }
        //       else if (条件 == 枚举_比较条件::不等于) {
        //           return this->指代对象 != 主信息->指代对象;
        //       }
        //   }
            return -1;
            }
        }
    }
};

//—— 状态
export class 状态节点主信息类 : public 基础信息基类 {
public:
    时间戳 收到信息时间戳=结构体_时间戳::当前();
    时间戳 信息发生时间戳=0;
    基础信息节点类* 对应信息节点 = nullptr; // 通常指向“特征节点”

    状态节点主信息类() = default;
    explicit 状态节点主信息类(词性节点类* 名) : 基础信息基类(名, nullptr) {}
    状态节点主信息类(词性节点类* 名, 词性节点类* 型, 基础信息节点类* 信息)
        : 基础信息基类(名, 型), 对应信息节点(信息) {
    }

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const override {
        状态节点主信息类* 主信息 = dynamic_cast<状态节点主信息类*>(对象);
        if (!主信息) {
            throw std::invalid_argument("状态节点主信息类:比较函数,比较对象类型错误");
            switch (字段) {
            case 枚举_比较字段::基础信息基类_名称:
                if (条件 == 枚举_比较条件::相等) {
                    return this->名称 == 主信息->名称;
                }
                else if (条件 == 枚举_比较条件::不等于) {
                    return this->名称 != 主信息->名称;
                }
                break;
            case 枚举_比较字段::基础信息基类_类型:

                if (条件 == 枚举_比较条件::相等) {
                    return this->类型 == 主信息->类型;
                }
                else if (条件 == 枚举_比较条件::不等于) {
                    return this->类型 != 主信息->类型;
                }
                break;
            case 枚举_比较字段::基础信息_状态节点主信息类_发生时间: {
                auto 时间差 = this->收到信息时间戳 - 主信息->收到信息时间戳;
                int64_t 时间数 = static_cast<int64_t>(时间差); 
                return 时间数;
                break;
            }
            case 枚举_比较字段::基础信息_状态节点主信息类_收到时间:
                auto 时间差 = this->收到信息时间戳 - 主信息->收到信息时间戳;
                int64_t 时间数 = static_cast<int64_t>(时间差); 
                return 时间数;
                break;
            return -1;
            }
        }
    }
};

//—— 动态（骨架）
export class 动态节点主信息类 : public 基础信息基类 {
public:
    基础信息节点类* 初始状态 = nullptr;
    基础信息节点类* 结果状态 = nullptr;

    动态节点主信息类() = default;
    动态节点主信息类(词性节点类* 名, 词性节点类* 型,
        基础信息节点类* 初, 基础信息节点类* 终)
        : 基础信息基类(名, 型), 初始状态(初), 结果状态(终) {
    }
    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const override {
        动态节点主信息类* 主信息 = dynamic_cast<动态节点主信息类*>(对象);
        if (!主信息) {
            throw std::invalid_argument("状态节点主信息类:比较函数,比较对象类型错误");
            switch (字段) {
            case 枚举_比较字段::基础信息基类_名称:
                if (条件 == 枚举_比较条件::相等) {
                    return this->名称 == 主信息->名称;
                }
                else if (条件 == 枚举_比较条件::不等于) {
                    return this->名称 != 主信息->名称;
                }
                break;
            case 枚举_比较字段::基础信息基类_类型:

                if (条件 == 枚举_比较条件::相等) {
                    return this->类型 == 主信息->类型;
                }
                else if (条件 == 枚举_比较条件::不等于) {
                    return this->类型 != 主信息->类型;
                }
                break;
       //     case 枚举_比较字段::基础信息_状态节点主信息类_发生时间: {
       //         auto 时间差 = this->收到信息时间戳 - 主信息->收到信息时间戳;
       //         int64_t 时间数 = std::chrono::duration_cast<std::chrono::milliseconds>(时间差).count();
       //         return 时间数;
       //         break;
       //     }
       //     case 枚举_比较字段::基础信息_状态节点主信息类_收到时间:
       //         auto 时间差 = this->收到信息时间戳 - 主信息->收到信息时间戳;
       //         int64_t 时间数 = std::chrono::duration_cast<std::chrono::milliseconds>(时间差).count();
       //         return 时间数;
       //         break;
              return -1;
            }
        }
    }
};

//—— 二次特征
export class 二次特征主信息类 : public 基础信息基类 {
public:
    枚举_二次特征种类 种类 = 枚举_二次特征种类::未定义;
    枚举_比较结果     结果 = 枚举_比较结果::未定义;

    // 被比较对象（可以是存在节点、特征节点、状态节点…）
    基础信息节点类* 左对象 = nullptr;
    基础信息节点类* 右对象 = nullptr;

    // 常用数值信息：差值 / 相似度 等，具体含义由 种类 决定
    double 差值 = 0.0;   // 比如：数值差值 / 距离
    double 相似度 = 0.0;   // 0~1

    二次特征主信息类() = default;

    二次特征主信息类(
        枚举_二次特征种类 种类,
        枚举_比较结果 结果,
        基础信息节点类* 左对象,
        基础信息节点类* 右对象,
        double 差值 = 0.0,
        double 相似度 = 0.0)
        : 种类(种类),
        结果(结果),
        左对象(左对象),
        右对象(右对象),
        差值(差值),
        相似度(相似度)
    {
    }   

    // 等价判定：名称/类型 + 四个核心字段全等，则 equal
    // 说明：若按你的语义，“比较特征==nullptr”表示整体比较，我们这里仍将“是否为空”作为语义的一部分参与等价；
    // 也就是说：一边为空（整体），另一边不为空（仅比较某特征），则不相等。
    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const override {
        动态节点主信息类* 主信息 = dynamic_cast<动态节点主信息类*>(对象);
        if (!主信息) {
            throw std::invalid_argument("状态节点主信息类:比较函数,比较对象类型错误");
            switch (字段) {
            case 枚举_比较字段::基础信息基类_名称:
                if (条件 == 枚举_比较条件::相等) {
                    return this->名称 == 主信息->名称;
                }
                else if (条件 == 枚举_比较条件::不等于) {
                    return this->名称 != 主信息->名称;
                }
                break;
            case 枚举_比较字段::基础信息基类_类型:

                if (条件 == 枚举_比较条件::相等) {
                    return this->类型 == 主信息->类型;
                }
                else if (条件 == 枚举_比较条件::不等于) {
                    return this->类型 != 主信息->类型;
                }
                break;
                //     case 枚举_比较字段::基础信息_状态节点主信息类_发生时间: {
                //         auto 时间差 = this->收到信息时间戳 - 主信息->收到信息时间戳;
                //         int64_t 时间数 = std::chrono::duration_cast<std::chrono::milliseconds>(时间差).count();
                //         return 时间数;
                //         break;
                //     }
                //     case 枚举_比较字段::基础信息_状态节点主信息类_收到时间:
                //         auto 时间差 = this->收到信息时间戳 - 主信息->收到信息时间戳;
                //         int64_t 时间数 = std::chrono::duration_cast<std::chrono::milliseconds>(时间差).count();
                //         return 时间数;
                //         break;
                return -1;
            }
        }
    }
};

//—— 因果
export class 因果主信息类 : public 基础信息基类 {
public:
    枚举_因果来源 来源;       // 文本描述、实验观察、他人报告
    枚举_验证状态 验证状态;   // 未验证、弱验证、强验证、被推翻

    基础信息节点类* 因果场景 = nullptr;
    基础信息节点类* 因信息 = nullptr;
    基础信息节点类* 果信息 = nullptr;
    基础信息节点类* 动作信息 = nullptr;
    std::vector<基础信息节点类*> 过程;
    std::vector<基础信息节点类*> 实例信息_动态节点;

    因果主信息类() = default;
    因果主信息类(词性节点类* 名, 词性节点类* 型,
        基础信息节点类* 场景, 基础信息节点类* 因, 基础信息节点类* 果, 基础信息节点类* 动作,
        std::vector<基础信息节点类*> 过程链 = {},
        std::vector<基础信息节点类*> 动态链 = {})
        : 基础信息基类(名, 型),
        因果场景(场景), 因信息(因), 果信息(果), 动作信息(动作),
        过程(过程链), 实例信息_动态节点(动态链) {
    }

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const override {
        动态节点主信息类* 主信息 = dynamic_cast<动态节点主信息类*>(对象);
        if (!主信息) {
            throw std::invalid_argument("状态节点主信息类:比较函数,比较对象类型错误");
            switch (字段) {
            case 枚举_比较字段::基础信息基类_名称:
                if (条件 == 枚举_比较条件::相等) {
                    return this->名称 == 主信息->名称;
                }
                else if (条件 == 枚举_比较条件::不等于) {
                    return this->名称 != 主信息->名称;
                }
                break;
            case 枚举_比较字段::基础信息基类_类型:

                if (条件 == 枚举_比较条件::相等) {
                    return this->类型 == 主信息->类型;
                }
                else if (条件 == 枚举_比较条件::不等于) {
                    return this->类型 != 主信息->类型;
                }
                break;
                //     case 枚举_比较字段::基础信息_状态节点主信息类_发生时间: {
                //         auto 时间差 = this->收到信息时间戳 - 主信息->收到信息时间戳;
                //         int64_t 时间数 = std::chrono::duration_cast<std::chrono::milliseconds>(时间差).count();
                //         return 时间数;
                //         break;
                //     }
                //     case 枚举_比较字段::基础信息_状态节点主信息类_收到时间:
                //         auto 时间差 = this->收到信息时间戳 - 主信息->收到信息时间戳;
                //         int64_t 时间数 = std::chrono::duration_cast<std::chrono::milliseconds>(时间差).count();
                //         return 时间数;
                //         break;
                return -1;
            }
        }
    }
};

//======================================================================
// 五、高级信息族：需求 / 方法头(基础/复合/外部) / 方法结果 / 任务
//======================================================================
export class 需求主信息类;
export class 基础方法头节点主信息类;
export class 复合头节点主信息类;
export class 外部方法头节点主信息类;
export class 方法结果主信息类;
export class 任务主信息类;

export class 高级信息基类 {
public:
    virtual ~高级信息基类() = default;

    virtual std::strong_ordering 比较到(const 需求主信息类&, 枚举_比较字段) const {
        throw std::invalid_argument("高级信息：不支持与 需求 比较");
    }
    virtual std::strong_ordering 比较到(const 基础方法头节点主信息类&, 枚举_比较字段) const {
        throw std::invalid_argument("高级信息：不支持与 基础方法头 比较");
    }
    virtual std::strong_ordering 比较到(const 复合头节点主信息类&, 枚举_比较字段) const {
        throw std::invalid_argument("高级信息：不支持与 复合方法头 比较");
    }
    virtual std::strong_ordering 比较到(const 外部方法头节点主信息类&, 枚举_比较字段) const {
        throw std::invalid_argument("高级信息：不支持与 外部方法头 比较");
    }
    virtual std::strong_ordering 比较到(const 方法结果主信息类&, 枚举_比较字段) const {
        throw std::invalid_argument("高级信息：不支持与 方法结果 比较");
    }
    virtual std::strong_ordering 比较到(const 任务主信息类&, 枚举_比较字段) const {
        throw std::invalid_argument("高级信息：不支持与 任务 比较");
    }

    virtual std::strong_ordering 多态比较(const 高级信息基类& other, 枚举_比较字段 f) const = 0;
};
export using 高级信息节点类 = 链表模板<高级信息基类*>::节点类;
export using 需求节点类 = 链表模板<高级信息基类*>::节点类;
export using 方法节点类 = 链表模板<高级信息基类*>::节点类;

export class 需求主信息类 : public 高级信息基类 {
public:
    存在节点类* 需求主体 = nullptr;
    存在节点类* 被需求存在 = nullptr;
    状态节点类* 被需求状态 = nullptr;
    二次特征节点类* 满足条件 = nullptr;
    std::chrono::steady_clock::time_point 需求有效时间{};
    std::chrono::steady_clock::time_point 需求生成时间{};
    long long             权重 = 0;

    需求主信息类() = default;
    需求主信息类(存在节点类* 主体, 存在节点类* 对象, 状态节点类* 状态, long long w = 0)
        : 需求主体(主体), 被需求存在(对象), 被需求状态(状态), 权重(w) {
    }

    std::strong_ordering 比较到(const 需求主信息类& rhs, 枚举_比较字段 /*f*/) const override {
        bool eq = (需求主体 == rhs.需求主体) && (被需求存在 == rhs.被需求存在) &&
            (被需求状态 == rhs.被需求状态) && (满足条件 == rhs.满足条件) &&
            (需求有效时间 == rhs.需求有效时间) && (需求生成时间 == rhs.需求生成时间) &&
            (权重 == rhs.权重);
        return eq ? std::strong_ordering::equal : std::strong_ordering::less;
    }
    std::strong_ordering 多态比较(const 高级信息基类& other, 枚举_比较字段 f) const override {
        return other.比较到(*this, f);
    }
};







// 方法执行结果：统一封装，结果节点可以指向任何主信息节点
export struct 结构_方法执行结果 {
    bool      成功 = false;
    void* 结果节点 = nullptr;   // 具体是什么类型，由调用方自己解释
    double    可信度 = 0.0;         // 可选：方法对自己结果的信心
};

// 方法执行上下文：在整个方法树执行过程中传递的信息
export struct 结构_方法上下文 {
    // —— 世界相关 —— //
    存在节点类* 目标存在 = nullptr;   // 方法主要面对的存在
    状态节点类* 目标状态 = nullptr;   // 方法关心的状态（可选）
    场景节点类* 输入场景 = nullptr;   // 当前场景/世界片段

    // —— 语言来源（可选） —— //
  //  句子节点类* 来源语句 = nullptr;  // 若由自然语言触发，则记录该句
    // 这里也可以加：句型实例指针 等

    // —— 任务/调度相关（可选） —— //
    std::uint64_t 上游任务ID = 0;       // 谁触发了这个方法
    // 你也可以在这里挂“执行轨迹”等信息
};

// ======================= 条件基类 =======================

// 条件：对上下文做布尔判断，用于方法树中的“条件节点”
export class 条件基类 {
public:
    virtual ~条件基类() = default;

    // 返回 true 表示条件满足
    virtual bool 检查(const 结构_方法上下文& 上下文) const = 0;
};


// 举例：一个简单的条件实现（可选，用来做示例）
// 检查：是否存在来源语句（即这个方法是由语言触发的）
export class 条件_有来源语句 : public 条件基类 {
public:
    bool 检查(const 结构_方法上下文& 上下文) const override {
        
            //上下文.来源语句 != nullptr;
        return 0;
    }
};

export class 方法头节点主信息类 : public 高级信息基类 {
public:
    // —— 语义类型 —— //
    枚举_方法语义大类 语义大类 = 枚举_方法语义大类::应用层;

    // —— 动作方式：本能 / 子方法 / 无 —— //
    枚举_动作执行方式 动作方式 = 枚举_动作执行方式::无动作;

    // 若动作方式 == 直接本能，则使用本能ID
    枚举_本能函数ID 本能ID = 枚举_本能函数ID::未定义;

    // 若动作方式 == 子方法，则指向一个子方法
    方法节点类* 子方法指针 = nullptr;

    // —— 目标与参数描述（世界相关） —— //
    // 这些通常在方法被实例化/调用时由外部填入上下文，
    // 这里更多是“类型/用途上的约定”，也可以存“期望类型信息”。
    bool 需要目标存在 = false;
    bool 需要目标状态 = false;
    bool 需要输入场景 = false;
    bool 需要来源语句 = false;


    // 可选：为调试/解释准备的名称
    std::string 方法名称;  
    long long 可信度;


    方法头节点主信息类() = default;
    //方法头节点主信息类(long long 信, 枚举_基础方法 法,特征节点类* 条 = nullptr,句子节点类* 描 = nullptr,需求节点类* 目 = nullptr)
   //     : 可信度(信), 基础方法(法), 条件(条), 方法用途描述(描), 目标(目) {
  //  }

   
};

//======================================================================
// 四、自然语言成分族：基本句 / 复杂句 / 段落 / 文章
//======================================================================

export class 自然语言成分基类 {
public:
    virtual ~自然语言成分基类() = default;
};



export class 句子成分主信息类 : public 自然语言成分基类 {
public:
    枚举_句子成分类型 类型 = 枚举_句子成分类型::其它;

    // 指向语素集中的“词性节点”
    词性节点类* 对应词性节点 = nullptr;

    // 在所属自然句的 词序列 中的下标
    std::size_t 在句子中的位置 = 0;

    句子成分主信息类() = default;

    句子成分主信息类(枚举_句子成分类型 t,
        词性节点类* 节点,
        std::size_t          位置)
        : 类型(t), 对应词性节点(节点), 在句子中的位置(位置)
    {
    }
};


// ===================================
//  文章主信息
// ===================================

export class 文章主信息类 : public 自然语言成分基类 {
public:
    std::string 标题;
    std::string 原始文本;  // 可选：整篇原文

    // 可以加：作者、时间、来源等元信息
    std::string 作者;
    std::string 来源;
    // 新增：这篇文章对应的“文本世界”的根场景
    场景节点类* 对应文本世界场景 = nullptr;
    文章主信息类() = default;

    explicit 文章主信息类(std::string 标题_,std::string 原文_ = "")
        : 标题(标题_),
        原始文本(原文_)
    {
    }
};


// ===================================
//  段落主信息
// ===================================

export class 段落主信息类 : public 自然语言成分基类 {
public:
    std::size_t 在文章中的序号 = 0;   // 第几个段落（从 0 或 1 开始，随你）
    std::string 原始文本;            // 该段落的原文（可选）

    段落主信息类() = default;

    段落主信息类(std::size_t 序号,
        std::string 文本 = "")
        : 在文章中的序号(序号),
        原始文本(文本)
    {
    }
};


// ===================================
//  自然句主信息（原始句子）
//  —— 可以拆成多个“原子子句”
// ===================================

export class 自然句主信息类 : public 自然语言成分基类 {
public:
    std::size_t 在段落中的序号 = 0;  // 在段落中的顺序

    // 整个自然句的词序列（来自语素集）
    std::vector<词性节点类*> 词序列;

    // 原始句子文本（可选）
    std::string 原始文本;

    // 可选：针对“主干句子”的成分（不含所有子句）
    词性节点类* 主语 = nullptr;
    词性节点类* 谓语 = nullptr;
    词性节点类* 宾语 = nullptr;

    // 在自然语言树中，可以将子句节点挂在 自然句节点 的子链上，
    // 这里保留一个快速访问的列表（不是必须）
    std::vector<子句节点类*> 子句节点列表;

    自然句主信息类() = default;

    自然句主信息类(std::size_t                    序号,
        std::vector<词性节点类*>  词序列_,
        std::string              文本_ = "")
        : 在段落中的序号(序号),
        词序列(词序列_),
        原始文本(文本_)
    {
    }
};


// ===================================
//  子句主信息（原子结构子句）
//  —— 每个子句对应 17 种原子结构之一
// ===================================

export class 子句主信息类 : public 自然语言成分基类 {
public:
    // 所属自然句（方便回跳）
    自然句主信息类* 所属自然句 = nullptr;

    // 这个子句属于哪一种原子结构
    枚举_原子结构类型 原子结构类型 = 枚举_原子结构类型::未定义;

    // 在所属自然句 词序列 中的起止下标（半开区间：[起, 止)）
    std::size_t 起始词索引 = 0;
    std::size_t 结束词索引 = 0;

    // 可选：将该区间对应的词拷贝一份出来，作为“局部视图”
    std::vector<词性节点类*> 局部词序列;

    // 针对这个子句的句子成分（可能与整个自然句的主语/谓语不同）
    词性节点类* 子句主语 = nullptr;
    词性节点类* 子句谓语 = nullptr;
    词性节点类* 子句宾语 = nullptr;
    // 你也可以扩展：状语、定语、补语等

    // 语义结果：这个子句在世界树里的落脚点
    场景节点类* 对应场景 = nullptr;
    状态节点类* 对应状态 = nullptr;
    动态节点类* 对应动态 = nullptr;
    因果节点类* 对应因果 = nullptr;

    子句主信息类() = default;

    子句主信息类(自然句主信息类* 句子,
        枚举_原子结构类型  类型,
        std::size_t         起始,
        std::size_t         结束)
        : 所属自然句(句子),
        原子结构类型(类型),
        起始词索引(起始),
        结束词索引(结束)
    {
    }

    // 小工具：根据 起始/结束索引 填充 局部词序列
    void 刷新局部词序列()
    {
        局部词序列.clear();
        if (!所属自然句) return;
        if (结束词索引 > 所属自然句->词序列.size()) return;

        for (std::size_t i = 起始词索引; i < 结束词索引; ++i) {
            局部词序列.push_back(所属自然句->词序列[i]);
        }
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////

export class 复合头节点主信息类 : public 高级信息基类 {
public:
    long long      可信度 = 0;
    方法节点类* 方法 = nullptr;

    复合头节点主信息类() = default;
    复合头节点主信息类(long long 信, 方法节点类* 方 = nullptr)
        : 可信度(信), 方法(方) {
    }

    std::strong_ordering 比较到(const 复合头节点主信息类& rhs, 枚举_比较字段 /*f*/) const override {
        bool eq = (可信度 == rhs.可信度) && (方法 == rhs.方法);
        return eq ? std::strong_ordering::equal : std::strong_ordering::less;
    }
    std::strong_ordering 多态比较(const 高级信息基类& other, 枚举_比较字段 f) const override {
        return other.比较到(*this, f);
    }
};
export class 方法结果主信息类 : public 高级信息基类 {
public:
    场景节点类* 结果场景 = nullptr;
    std::vector<存在节点类*>     受影响存在;
    std::vector<特征节点类*>     受影响特征;

    方法结果主信息类() = default;
    方法结果主信息类(场景节点类* 场,
        std::vector<存在节点类*> e = {},
        std::vector<特征节点类*> f = {})
        : 结果场景(场), 受影响存在(e), 受影响特征(f) {
    }

    std::strong_ordering 比较到(const 方法结果主信息类& rhs, 枚举_比较字段 /*f*/) const override {
        bool eq = (结果场景 == rhs.结果场景) &&
            (受影响存在 == rhs.受影响存在) &&
            (受影响特征 == rhs.受影响特征);
        return eq ? std::strong_ordering::equal : std::strong_ordering::less;
    }
    std::strong_ordering 多态比较(const 高级信息基类& other, 枚举_比较字段 f) const override {
        return other.比较到(*this, f);
    }

};

export class 任务主信息类 : public 高级信息基类 {
public:
    需求主信息类* 任务目标 = nullptr;
    std::vector<方法节点类*>    可用方法列表;
    long long                   任务优先级 = 0;
    long long                   任务可执行度 = 0;
    方法节点类* 选定方法实例 = nullptr;
    std::vector<方法节点类*>    执行序列;
    场景节点类* 执行条件 = nullptr;
    场景节点类* 需求条件 = nullptr;
    场景节点类* 现有条件 = nullptr;

    任务主信息类() = default;
    任务主信息类(需求主信息类* 目, long long 优 = 0, long long 可 = 0)
        : 任务目标(目), 任务优先级(优), 任务可执行度(可) {
    }

    std::strong_ordering 比较到(const 任务主信息类& rhs, 枚举_比较字段 /*f*/) const override {
        bool eq = (任务目标 == rhs.任务目标) &&
            (可用方法列表 == rhs.可用方法列表) &&
            (任务优先级 == rhs.任务优先级) &&
            (任务可执行度 == rhs.任务可执行度) &&
            (选定方法实例 == rhs.选定方法实例) &&
            (执行序列 == rhs.执行序列) &&
            (执行条件 == rhs.执行条件) &&
            (需求条件 == rhs.需求条件) &&
            (现有条件 == rhs.现有条件);
        return eq ? std::strong_ordering::equal : std::strong_ordering::less;
    }
    std::strong_ordering 多态比较(const 高级信息基类& other, 枚举_比较字段 f) const override {
        return other.比较到(*this, f);
    }
};


export class 自然语言知识库主信息类 {
public:
//    句子节点类* 句型;
//    句子节点类* 用途描述;
};

///////////////////////////////////////自然语言模块////////////////////////////////////////////////////////////////////
// 1) 本能层：快速、确定


// 2) 方法层：整体理解（可成长）
export struct 结构体_整体理解结果 {
    场景节点类* 临时场景;
    double 全局置信度;
    int 未解析指代数;
    int 冲突数;
};

export struct 结构体_自然语言理解输出 {
    场景节点类* 临时场景;
    double 全局置信度;
    std::vector<基础信息节点类*> 未决分支;
    bool 是否提交;
};
export struct 结构体_NLU评估 {
    double 覆盖率;
    double 指代解析率;
    int    矛盾数;
    double 全局置信度;
    double 时间误差;
    double 空间误差;
    bool   可执行;
};



export struct 结构体_NLU评估值 {
    double 覆盖率 = 0.0;
    double 指代解析率 = 0.0;
    int    冲突数 = 0;
    bool   可执行 = false;
    double 全局置信度 = 0.0;
    double 时间误差 = 0.0;
    double 空间误差 = 0.0;
    double 比较正确率 = 0.0;
    double 否定正确率 = 0.0;
    double 程度正确率 = 0.0;
};

export struct 结构体_NLU阈值 {
    double T_cov = 0.85;
    double T_coref = 0.80;
    int    N_conf_max = 0;
    bool   必须可执行 = true;
    double T_submit = 0.72;
    double E_time_max = 0.5;   // 自定单位
    double E_space_max = 0.5;  // 自定单位
    double T_cmp = 0.75;
    double T_neg = 0.90;
    double T_deg = 0.75;
};

export struct 结构体_NLU评估结果 {
    结构体_NLU评估值 值;
    bool 通过提交 = false;
    bool 局部提交 = false;
    std::vector<基础信息节点类*> 待补证据; // 未决点
};

export struct 结构体_规则_判断远近_Y {
    // 参照设定
    bool 更大为更远 = true;  // false 表示更大为更近
    int64_t 阈值_近 = 0;     // 根据坐标单位设置
};

// 绑定结果（抽象→具体）
export struct 结构体_绑定_判断远近 {
    基础信息节点类* 存在_E1 = nullptr;
    基础信息节点类* 存在_E2 = nullptr;
    特征节点类* 特征_E1_Y = nullptr;
    特征节点类* 特征_E2_Y = nullptr;
};

// 计算产物
export struct 结构体_判断远近输出 {
    二次特征节点类* 二次特征_ΔY = nullptr;
    二次特征节点类* 二次特征_排序 = nullptr;
    基础信息节点类* 结果结论 = nullptr; // 可用特征节点或关系节点表达“远/近/等距”
};


// 运算器：对抽到的值做二元/多元计算，产出“中间证据”（差值/距离/排序/关系枚举…）
export enum class 枚举_运算类型 { 差值, 距离_L2, 比值, 排序, 阈值比较, 方向_Y, 包含, 相交, 自定义 };
export struct 结构体_运算结果 { bool 有效 = false; int64_t 标量 = 0; double 浮点 = 0; int8_t 符号 = 0; int 枚举关系 = 0; /*证据*/ };

// 判据：把运算结果与阈值/标准比较，得到“通过/不通过/等价”等判定
export enum class 枚举_判据 { 大于, 小于, 等于, 绝对小于阈值, 绝对大于阈值, 枚举等于, 自定义 };
export struct 结构体_判定 { bool 通过 = false; double 置信度 = 1.0; };

// 通用“判断规格”


export enum class 枚举_LK用途 {
    存在_陈述, 特征_描述, 事件_叙述, 关系_布局, 比较_排序,
    数量_范围, 否定_排除, 疑问_信息需求, 指令_意图,
    条件_假设, 因果_解释, 时间_体貌, 评价_立场,
    来源_引述, 定义_分类, 角色_标注, 约束_义务, 例外_对照, 单位_度量
};

// 抽象场景模板：占位符+必需特征
export struct 结构体_抽象场景模板 {
    // 抽象存在/特征占位（E1,E2,… Fx …）
    std::vector<基础信息节点类*> 存在占位;   // E1,E2...
    std::vector<特征节点类*>     特征占位;   // Fx...
    // 约束（必须有的字段：名称/类型/坐标/时间/角色等）
    std::vector<int> 必备约束ID; // 简化起见，用ID指代
};

// 语言模板：槽位化的“怎么说”
export struct 结构体_语言模板 {
    枚举_LK用途 用途;
    std::string 模板ID;          // 全局唯一
    // 词序列槽位，如 {主语:E1名, 谓词:动词“有/发生/位于”, 宾语:E2名, 修饰:Fx名…}
    std::vector<词性节点类*> 词序列槽位;
    // 线性化策略（见下：名称组装顺序）
    std::string 线性化策略ID;
};

// 知识条目
export struct 结构体_语言知识条目 {
    结构体_语言模板 语言;
    结构体_抽象场景模板 场景模板;
    // 触发条件：最少输入/上下文要求/缺省单位等
    std::vector<int> 触发条件ID;
    // 生成时需要的“判断/二次特征”配方（可为空）
    std::vector<int> 二次特征配方ID;
    // 输出：形容词 or 短语 or 结构化句子
    bool 输出为短语 = true;
};

export struct 结构体_线性化上下文 {
    场景节点类* 场景;
    std::unordered_map<std::string, 基础信息节点类*> 绑定; // "E1"->节点
};
export using 函数_线性化 = std::function<std::vector<词性节点类*>(const 结构体_语言模板&, const 结构体_线性化上下文&)>;

export struct 结构体_LK生成输入 {
    枚举_LK用途 用途;
    std::string 模板ID;            // 可空：自动选最匹配
    场景节点类* 目标场景;           // 抽象/当前场景
    std::unordered_map<std::string, 基础信息节点类*> 预绑定; // 传入已有绑定（如 E1/E2）
};
export struct 结构体_LK生成输出 {
    短语节点类* 语言节点 = nullptr;
    std::vector<词性节点类*> 词序列;   // 已按策略顺序生成
    std::unordered_map<std::string, 基础信息节点类*> 最终绑定;
    std::vector<二次特征节点类*> 证据链; // 若涉及判断
    bool 成功 = false;
    double 置信度 = 1.0;
};

export struct 结构体_取值器输入 { 基础信息节点类* A; 基础信息节点类* B; 场景节点类* 场景; };
export struct 结构体_取值 { bool 有效 = false; std::vector<int64_t> 标量; /*或*/ 结构体_空间坐标 坐标; /*...*/ };

export using 取值器 = std::function<结构体_取值(结构体_取值器输入)>;

export struct 结构体_判断规格 {
    基础信息节点类* A = nullptr;
    基础信息节点类* B = nullptr;
    取值器 取值器A; 取值器 取值器B;
    枚举_运算类型 运算 = 枚举_运算类型::差值;
    枚举_判据 判据 = 枚举_判据::大于;
    double 阈值_浮 = 0; int64_t 阈值_整 = 0;
    词性节点类* 输出名称 = nullptr;  // 结果名
    语素节点类* 输出类型 = nullptr;  // 结果类型
};

export struct 结构体_判断输出 {
    二次特征节点类* 证据节点1 = nullptr;
    二次特征节点类* 证据节点2 = nullptr;
    基础信息节点类* 结果节点 = nullptr;
    bool 成功 = false;
    double 置信度 = 1.0;
};



export class 概念基类 {
public:    
    枚举_概念类别 概念类别 = 枚举_概念类别::未知;
    // 内部标识名，不一定等于自然语言词（可以是技术名）
    std::string 内部标识名;
    // 这个概念在句子中允许出现的句法类别（名词性/动词性/形容词性…）
    std::vector<枚举_句法类别> 允许句法类别;
    virtual ~概念基类() = default;
};

// 常见几个派生类，先给出基础骨架，将来再慢慢加字段
export class 存在概念类 : public 概念基类 {
public:
    // 典型特征、默认大小、典型颜色等，可以慢慢加
    存在概念类() {
        概念类别 = 枚举_概念类别::存在概念;
    }
};

export class 特征概念类 : public 概念基类 {
public:
    // 比如“颜色”、“温度”、“位置”、“速度”等
    // 可挂：是矢量/非矢量、取值范围等
    特征概念类() {
        概念类别 = 枚举_概念类别::特征概念;
    }
};

export class 动态概念类 : public 概念基类 {
public:
    // 比如“吃”、“移动”、“打开”等
    动态概念类() {
        概念类别 = 枚举_概念类别::动态概念;
    }
};