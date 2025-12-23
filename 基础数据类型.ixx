module;
#include <afx.h>
//#include <vector>
//#include <variant>

export module 基础数据类型模块;
import <variant>;
 import <string>;
import <vector>;
import <chrono>;
import <array>;

export enum class 枚举_链域 : std::uint8_t {
    未定义 = 0,
    世界链 = 1,    // 基础信息节点类
    特征值链 = 2,  // 特征值节点类
    语素集 = 3,    // 词/词性/短语节点
    语言集 = 4,    // 自然语言成分节点
    行动信息集 = 5 // 高级信息节点
};
export struct 结构体_节点引用 {
    枚举_链域 域 = 枚举_链域::未定义;
    std::string 主键; // 节点->主键
    bool 空() const { return 域 == 枚举_链域::未定义 || 主键.empty(); }
};
struct 结构体_存在存档字段 {
    结构体_节点引用 名称;
    结构体_节点引用 类型;
    结构体_节点引用 概念模板;
    结构体_节点引用 内部世界;
    //时间戳 创建时间{};
    std::uint32_t 命中次数{};
};


export enum class 枚举_比较条件 : std::uint8_t {
    未定义 = 0,
    相等,
    不等于,
    大于,
    小于,
    大于等于,
    小于等于
};

// 注意：这是“比较字段枚举”，用于各类主信息的多态比较。
// 约定：
//  - 未实现的字段比较允许返回 -1（不支持）
//  - 这里尽量“按域分组”，方便扩展与检索
export enum class 枚举_比较字段 : std::uint16_t {
    未定义 = 0,

    //==================== 通用（谨慎使用） ====================
    名称,
    类型,
    值,
    单位,
    实例信息,
    词性词指针,
    原始短语,
    词指针,

    //==================== 语素类 ====================
    语素_词_词,

    语素_词性_词性,
    语素_词性_对应基础信息指针,

    语素_短语字节点_参照词,
    语素_短语字节点_比较对象词,
    语素_短语子节点_比较词,
    语素_短语子节点_结果词,

    //==================== 基础信息基类 ====================
    基础信息基类_名称,
    基础信息基类_类型,

    //==================== 指代节点 ====================
    基础信息_指代节点_代词,
    基础信息_指代节点_指代对象,

    //==================== 特征值（细分类型） ====================
    基础信息_矢量特征值_类型,
    基础信息_矢量特征值_单位,
    基础信息_矢量特征值_值,

    基础信息_非矢量特征值_类型,
    基础信息_非矢量特征值_单位,
    基础信息_非矢量特征值_值,

    基础信息_字符串特征值_单位,
    基础信息_字符串特征值_值,

    基础信息_引用特征值_目标,

    //==================== 特征节点 ====================
    基础信息_特征节点_类型,
    基础信息_特征节点_值,
    基础信息_特征节点_名称,

    //==================== 存在 / 场景（常用几何字段） ====================
    基础信息_存在节点_绝对坐标_x轴,
    基础信息_存在节点_绝对坐标_y轴,
    基础信息_存在节点_绝对坐标_z轴,

    基础信息_场景节点_绝对坐标_x轴,
    基础信息_场景节点_绝对坐标_y轴,
    基础信息_场景节点_绝对坐标_z轴,

    //==================== 状态节点 ====================
    基础信息_状态节点主信息类_收到时间,
    基础信息_状态节点主信息类_发生时间,
    基础信息_状态节点主信息类_是否变化,

    //==================== 状态类型（你原枚举中提到的三类） ====================
    状态类型_特征,
    状态类型_存在,
    状态类型_场景,

    //==================== 需求（占位：后续需求树/任务树会用） ====================
    产生需求主体,
    需求对象,
    需求状态,

    //==================== 评估/度量（自然语言理解/融合质量） ====================
    评估_覆盖率,
    评估_指代解析率,
    评估_一致性_冲突数,
    评估_可执行性,
    评估_全局置信度,
    评估_时间对齐误差,
    评估_空间对齐误差,
    评估_比较正确率,
    评估_否定正确率,
    评估_程度正确率
};
export enum class 枚举_任务类型{普通任务,思考任务,尝试任务,学习任务};
export enum class 枚举_基础方法 {添加存在,添加特征,添加场景,未定义, 外部调用};

//============================ 通用枚举（保留） ============================
export enum class 枚举_主信息类型 {
    指代, 非矢量特征值, 矢量特征值, 短语子, 基础信息基类, 语素基类,
    词, 词性, 短语, 特征, 特征_特征, 特征_存在, 存在,
    场景, 状态的特征, 动态的特征, 状态_特征, 状态_存在, 状态_场景,
    动态, 因果, 关系, 其它, 存在信息, 存在信息_指代, 动态信息, 特征信息,
    二次特征_修饰, 特征信息_数量, 关系信息, 情绪特征信息, 情绪调节信息, 语言信息_仅记录
};

export enum class 枚举_词性 {
    // 原有枚举项
    名词, 动词, 形容词, 副词, 连词, 介词, 专有名词, 抽象名词, 代词, 数词, 量词, 助词, 叹词,
    拟声词, 标点符号, 时间词, 状态词, 方位词, 习用语, 简称略语, 非语素字, 语气词, 外文字符, 未定义,
    地名,
    // 新增词性（按照你提供的格式对应）
    n, // 普通名词
    f, // 方位名词
    s, // 处所名词
    t, // 时间名词
    nr, // 人名
    ns, // 地名（与原有“地名”重复概念，实际使用中可能需要处理冲突，这里保留原样展示）
    nt, // 机构团体名
    nw, // 作品名
    nz, // 其他专名
    v, // 普通动词
    vd, // 动副词
    vn, // 名动词
    a, // 形容词
    ad, // 副形词
    an, // 名形词
    d, // 副词
    m, // 数量词
    q, // 量词
    r, // 代词
    p, // 介词
    c, // 连词
    u, // 助词
    xc, // 其他虚词
    w, // 标点符号

    // 专名识别缩略词
    PER, // 人名
    LOC, // 地名
    ORG, // 机构名
    TIME  // 时间
};
export struct 结构体_分词 {
    std::string 词;
    std::string  词性;
    结构体_分词() = default;
    结构体_分词(std::string 词_, std::string  词性_)
        :词(词_), 词性(词性_) {
    }

};
//============================ 空间/几何 ============================
export struct 结构体_空间坐标 {
    long long x = 0, y = 0, z = 0;
    friend bool operator==(const 结构体_空间坐标& a, const 结构体_空间坐标& b) {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }
    结构体_空间坐标 operator-(const 结构体_空间坐标& o) const {
        return { x - o.x, y - o.y, z - o.z };
    }
};
export enum class 结构体_轮廓类型 { 盒子, 球体, 圆柱, 自定义 };
export struct 结构体_简单轮廓信息 {
    结构体_轮廓类型 类型 = 结构体_轮廓类型::盒子;
    结构体_空间坐标 中心坐标{};
    结构体_空间坐标 尺寸{};
    long long 半径 = 0;
    friend bool operator==(const 结构体_简单轮廓信息& a, const 结构体_简单轮廓信息& b) {
        return a.类型 == b.类型 && a.中心坐标 == b.中心坐标 && a.尺寸 == b.尺寸 && a.半径 == b.半径;
    }
    结构体_简单轮廓信息 operator-(const 结构体_简单轮廓信息& o) const {
        auto 类型_ = (类型 == o.类型) ? 类型 : 结构体_轮廓类型::自定义;
        return { 类型_, 中心坐标 - o.中心坐标, 尺寸 - o.尺寸, 半径 - o.半径 };
    }
};


//============================ 概念类/概念信息模块 ============================
export enum class 枚举_句法类别 {
    未知 = 0,
    名词性,
    动词性,
    形容词性,
    副词性,
    功能词性,   // 连词 / 介词 / 助词 / 语气词 等
};
export enum class 枚举_概念类别 {
    未知 = 0,
    存在概念,      // “苹果”、“银行”、“门”
    特征概念,      // “颜色”、“温度”、“速度”
    特征值概念,    // “红色”、“蓝色”、“冷”、“热”
    动态概念,      // “移动”、“吃”、“打开”
    因果规则概念,  // “加热 -> 温度升高” 这种规则模板
    方法概念,      // “识别”、“归纳”等内部方法
};
// 外部分词器/词典的POS标签（可选，抄Baidu / 其它工具）
export enum class 枚举_外部词性 {
    未知 = 0,
    // 下面只是示例，你可以按 Baidu 的标签表细化：
    名词_N,
    动词_V,
    形容词_ADJ,
    副词_ADV,
    代词_PRON,
    数词_NUM,
    介词_PREP,
    连词_CONJ,
    助词_PART,
    语气词_MODAL,
    标点_PUNC,
};

// 你自己的「统一简化后的用法类别」
export enum class 枚举_词用法类别 {
    未知 = 0,
    名词用法,
    动词用法,
    形容词用法,
    副词用法,
    功能词用法,   // 连词/介词/助词/语气词等
};



// === 消息定义 ===
export namespace 消息类型 {
    enum 消息代码 {

        新任务 = WM_USER + 200,
        任务完成 = WM_USER + 250,
        空任务= WM_USER + 251,
        任务不可执行 = WM_USER + 252,       
        任务无实例化方法 = WM_USER + 253,

        分词请求 = WM_USER + 260,
        分词完成 = WM_USER + 261,
        ///////////////////////////////////////////////////      
        新任务_完善词信息 = WM_USER + 300,

        ///////////////////////////////////////////////////
        自我_安全值变化= WM_USER + 900,
        消息_退出= WM_USER + 1,

    };
 
    constexpr const  char* 名称(消息代码 消息) noexcept {
        switch (消息) {
        case 新任务: return "新任务";
        case 任务完成: return "任务完成";
        case 空任务: return "空任务";
        case 任务不可执行: return "任务不可执行";
        case 任务无实例化方法: return "任务不可执行";            
            
        case 分词请求: return "分词请求";
        case 分词完成: return "分词完成";
        case 新任务_完善词信息: return "新任务_完善词信息";
        case 自我_安全值变化: return "自我_安全值变化";
        case 消息_退出: return "消息_退出";
        default: return "未知消息";
        }
    }
}
// === 消息参数 ===
export struct 消息参数 {
    std::variant<int64_t, std::string, std::vector<std::string>, void*> 数据;

    消息参数(int64_t 值) : 数据(值) {}
    消息参数(std::string 值) : 数据(值) {}
    消息参数(std::vector<std::string> 值) : 数据(值) {}
    消息参数(void* 值) : 数据(值) {}

    template <typename T>
    消息参数(T) = delete;

    bool 是整数()  const { return std::holds_alternative<int64_t>(数据); }
//    int64_t 取整数()  { return std::get<int64_t>(数据); }
};
// 消息结构体
//export struct 消息 {
//    消息类型::消息代码 消息类型;   
//    std::vector<std::variant<int64_t, std::string, std::vector<std::string>, void*>> 参数列表;
//    消息(消息类型::消息代码 消息类型_, std::variant<int64_t, std::string, std::vector<std::string>, void*> 数据) {
//        消息类型 = 消息类型_;
//        参数列表.push_back(数据);
//    }
//    消息() {}
//    
//};

export enum class 枚举_语言用途 {
    存在_陈述, 特征_描述, 事件_叙述, 关系_布局, 比较_排序,
    数量_范围, 否定_排除, 疑问_信息需求, 指令_意图,
    条件_假设, 因果_解释, 时间_体貌, 评价_立场,
    来源_引述, 定义_分类, 角色_标注, 约束_义务, 例外_对照, 单位_度量
};

export enum class 枚举_因果来源{ 文本描述,实验观察,他人报告 };
export enum class  枚举_验证状态 {未验证,弱验证,强验证,被推翻 };
export enum class  枚举_方法来源 { 文本理解, 自我实践, 他人经验 };
export enum class  枚举_实践状态 { 未验证, 部分验证, 已验证, 已否定 };

///////////////////////////////方法类相关///////////////////////////////////////////////////
export enum class 枚举_本能函数ID : std::uint16_t {
    未定义 = 0,
    // 例如：
    比较特征_时间,
    比较特征_对象,
    阈值判断,
    空间拓扑判断,
    时间顺序判断,
    存在判断,
    // ...
};

// 方法属于哪一层/哪种语义用途
export enum class 枚举_方法语义大类 : std::uint8_t {
    描述层,     // 场景/状态/动态构建
    判断层,     // 二次特征/判据计算
    应用层,     // 任务/决策/执行
    语言解析,   // 语言 -> 场景
    语言生成,   // 场景 -> 语言
    学习层      // 参数更新/方法抽象
};

// 头节点动作的执行方式
export enum class 枚举_动作执行方式 : std::uint8_t {
    无动作,      // 纯分发/决策
    直接本能,    // 直接调用某个本能函数
    子方法       // 调用另一个方法树
};
////////////////////////////////////////////////////////////////////////////////////
// ==================== 纯类型别名（不涉及链表） ====================
export using 时间戳 = std::uint64_t;
export using 置信度 = double;
export using 需求强度 = double;
export using 特征值 = double;
export struct 结构体_时间戳 {
    std::uint64_t 值 = 0;

    // 显式类型转换解决收缩转换问题
    static 时间戳 当前() {
        return {
            static_cast<std::uint64_t>( // 明确转换意图
                std::chrono::steady_clock::now()
                    .time_since_epoch()
                    .count()
            )
        };
    }
};









export struct 点阵图
{
    std::int32_t 宽 = 0;
    std::int32_t 高 = 0;
    // 所有像素按行优先展平，每 64 个像素压缩成一个 64 位整数
    std::vector<std::uint64_t> 数据;
};




namespace {


    inline int 统计位数(std::uint64_t x)
    {
#if defined(__cpp_lib_bitops)
        return static_cast<int>(std::popcount(x));
#else
        int c = 0;
        while (x)
        {
            x &= (x - 1);
            ++c;
        }
        return c;
#endif
    }
}
// 从你的 vector<int64_t> 原始数据构造 点阵图
export 点阵图 构造点阵图(std::int32_t 宽, std::int32_t 高, const std::vector<std::int64_t>& 原始数据)
{
    点阵图 图;
    图.宽 = 宽;
    图.高 = 高;

    if (宽 <= 0 || 高 <= 0)
    {
        // 空图
        图.数据.clear();
        return 图;
    }

    const std::size_t N = static_cast<std::size_t>(宽) * static_cast<std::size_t>(高);
    const std::size_t 期望块数 = (N + 63u) / 64u;

    图.数据.assign(期望块数, 0);

    const std::size_t 实际块数 = 原始数据.size();
    const std::size_t 拷贝块数 = 实际块数 < 期望块数 ? 实际块数 : 期望块数;

    for (std::size_t i = 0; i < 拷贝块数; ++i)
    {
        图.数据[i] = static_cast<std::uint64_t>(原始数据[i]);
    }

    return 图;
}

// 汉明相似度：1 = 完全相同，0 = 完全相反
export double 计算_轮廓_汉明相似度(const 点阵图& A, const 点阵图& B)
{
    if (A.宽 != B.宽 || A.高 != B.高)
    {
        throw std::invalid_argument("计算_轮廓_汉明相似度: 分辨率不一致");
    }
    if (A.数据.size() != B.数据.size())
    {
        throw std::invalid_argument("计算_轮廓_汉明相似度: 数据块数量不一致");
    }

    const std::size_t N = static_cast<std::size_t>(A.宽) * static_cast<std::size_t>(A.高);
    if (N == 0)
    {
        return 1.0; // 约定：空图视为完全相同
    }

    int diff = 0;
    for (std::size_t i = 0; i < A.数据.size(); ++i)
    {
        diff += 统计位数(A.数据[i] ^ B.数据[i]);
    }

    const double ratio = static_cast<double>(diff) / static_cast<double>(N);
    return 1.0 - ratio;
}

// IoU: 交并比 = |A∩B| / |A∪B|
export double 计算_轮廓_IoU(const 点阵图& A, const 点阵图& B)
{
    if (A.宽 != B.宽 || A.高 != B.高)
    {
        throw std::invalid_argument("计算_轮廓_IoU: 分辨率不一致");
    }
    if (A.数据.size() != B.数据.size())
    {
        throw std::invalid_argument("计算_轮廓_IoU: 数据块数量不一致");
    }

    int 交数 = 0;
    int 并数 = 0;

    for (std::size_t i = 0; i < A.数据.size(); ++i)
    {
        const std::uint64_t 交 = A.数据[i] & B.数据[i];
        const std::uint64_t 并 = A.数据[i] | B.数据[i];

        交数 += 统计位数(交);
        并数 += 统计位数(并);
    }

    if (并数 == 0)
    {
        // 两个都是全空图，视为完全相同
        return 1.0;
    }

    return static_cast<double>(交数) / static_cast<double>(并数);
}

// 主信息定义模块.ixx 里（建议放在比较靠前的位置）

export enum class 枚举_世界类型 {
    现实世界,
    文本世界,
    想象世界,
    记忆世界,
    推理世界,
    内部世界,
    虚拟世界,
    其它
};



// ===================================
//  句子成分类型枚举（主语/谓语/宾语/...）
// ===================================

export enum class 枚举_句子成分类型 {
    主语,
    谓语,
    宾语,
    定语,
    状语,
    补语,
    主题,     // 话题（比如“至于小明，他……”里的“小明”）
    其它
};


// ===================================
//  17 种原子结构类型枚举
//  （名称你可以按自己的最终版本调整）
// ===================================

export enum class 枚举_原子结构类型 {
    未定义 = 0,

    存在与命名,        // “有一个 A”，“这叫 B”
    属性与状态,        // “A 是 红色的”，“天气 很冷”
    动态与事件,        // “A 走到 B”，“发生了 爆炸”
    时间定位,          // “昨天”，“在 2025 年”，“三分钟后”
    空间位置,          // “在 桌子上”，“在 房间里”
    数量与范围,        // “有 三个”，“不少于 10”，“在 1 到 5 之间”
    比较与排序,        // “比……更……”，“最高”，“最先”
    否定与排除,        // “不是”，“从来没有”，“除了 A 以外”
    疑问与信息需求,    // “谁”，“什么”，“为什么”
    条件与假设,        // “如果……就……”，“只要……就……”
    因果与解释,        // “因为……所以……”，“导致……”，“于是”
    定义与分类,        // “A 是 一种 B”，“A 属于 C 类”
    角色与参与者,      // “A 是 B 的 老师/学生/主人/部分”
    评价与立场,        // “很好”，“很危险”，“我认为……”
    约束与义务,        // “必须”，“应该”，“不允许”
    来源与引述,        // “他说”，“据报道”，“根据……”
    指代与省略恢复     // “他/她/它”，“那里”，“前面提到的 那个”
};

export enum class 枚举_比较结果 : std::int8_t {
    未定义 = 0,
    小于 = -1,
    等于 = 0,
    大于 = 1,
    无法比较 = 2
};
export enum class 枚举_二次特征种类 : std::int16_t {
    未定义 = 0,
    存在比较,      // 这整个节点代表 “存在 A 与 B 的综合比较”
    特征比较,      // 代表某一种特征类型的比较（颜色/位置/尺寸…）
    状态比较,
    动态比较,
    因果强度比较,
    // ... 后面你可以继续加
    // ===== 关系类（建议从 100 起）=====
    观察关系 = 100,   // (主体, 客体) 在 场景 中的观察/认知统计实例
    // 以后可加：拥有关系 = 101, 遮挡关系 = 102, 接触关系 = 103 ...
};


// 为了以后扩展清晰，加一个比较模式标记
export enum class 枚举_特征值比较模式 {
    
    有序标量 = 0,      // 支持 <、>、范围、差值
    相等,
    相等或相似度       // 只支持 == / != 或相似度，不支持大小关系
};

export struct 结构体_观察统计 {
    std::uint32_t 观察次数 = 0;
    时间戳 最近观察时间 = 0;
    时间戳 最近看见时间 = 0;

    float 熟悉度 = 0.0f;     // 0~1
    float 不确定度 = 1.0f;   // 0~1
    float 变化评分 = 0.0f;   // 0~1

    std::uint32_t 连续命中帧 = 0;
    std::uint32_t 连续丢失帧 = 0;
};

/////////////////////////////////////////二次特征结束//////////////////////////////////////////////////////


// 简单的颜色结构体，替代OpenCV的viz::Color
// ===== 基础类型 =====
export struct Color {
    uint8_t r = 255, g = 255, b = 255;
};


/////////////////////////////////////////////////////////////////////////////////
// 相机模块参数
// 在 Vector3D 结构体定义后添加（同一模块内）

export struct Vector3D {
    double  x = 0.0f;
    double  y = 0.0f;
    double  z = 0.0f;

    // 可选：构造函数
    Vector3D() = default;
    constexpr Vector3D(double  xx, double  yy, double  zz) : x(xx), y(yy), z(zz) {}

    // ===== 关键：标量乘法（成员函数）=====
    constexpr Vector3D operator*(double  scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }

    // ===== 推荐同时添加：标量除法 =====
    constexpr Vector3D operator/(double  scalar) const {
        if (scalar == 0.0f) return *this; // 防除零，或抛异常
        return Vector3D(x / scalar, y / scalar, z / scalar);
    }

    // ===== 推荐添加：向量加法/减法 =====
    constexpr Vector3D operator+(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }

    constexpr Vector3D operator-(const Vector3D& other) const {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    // ===== 推荐添加：复合赋值运算符 =====
    Vector3D& operator*=(double  scalar) {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }

    Vector3D& operator/=(double  scalar) {
        if (scalar != 0.0f) {
            x /= scalar; y /= scalar; z /= scalar;
        }
        return *this;
    }

    Vector3D& operator+=(const Vector3D& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    Vector3D& operator-=(const Vector3D& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }
};

// ===== 额外：非成员函数版本，支持 double  * Vector3D（如 0.5 * vec）=====
export constexpr Vector3D operator*(double  scalar, const Vector3D& vec) {
    return vec * scalar;
}

export constexpr Vector3D operator/(double  scalar, const Vector3D& vec) {
    return Vector3D(scalar / vec.x, scalar / vec.y, scalar / vec.z); // 注意：语义不同，通常不常用
}



// ===== 原始场景帧 =====
template<class T>
struct RawImage {
    int w = 0, h = 0;
    std::vector<T> data;

    bool valid() const { return w > 0 && h > 0 && (int)data.size() == w * h; }

    T& at(int x, int y) { return data[(size_t)y * (size_t)w + (size_t)x]; }
    const T& at(int x, int y) const { return data[(size_t)y * (size_t)w + (size_t)x]; }
};
export struct 结构体_原始场景帧 {
    std::uint64_t 时间 = 0;
    int 宽度 = 0;
    int 高度 = 0;

    // 深度图 Z (米)，按行优先展平：idx = v*宽度 + u
    std::vector<double> 深度;

    // RGB 图（可选），size==深度.size() 时有效
    std::vector<Color> 颜色;

    // 相机坐标系下点云（可选），size==深度.size() 时表示“每像素点云”
    std::vector<Vector3D> 点云;

    // 点云轮廓编码（可选）
    std::vector<std::int64_t> 轮廓;

    [[nodiscard]] bool 有效深度() const noexcept {
        return 宽度 > 0 && 高度 > 0
            && 深度.size() == static_cast<std::size_t>(宽度) * static_cast<std::size_t>(高度);
    }
    [[nodiscard]] bool 有效颜色() const noexcept { return 颜色.size() == 深度.size(); }
    [[nodiscard]] bool 有效点云() const noexcept { return 点云.size() == 深度.size(); }

    [[nodiscard]] inline std::size_t 索引(int u, int v) const noexcept {
        return static_cast<std::size_t>(v) * static_cast<std::size_t>(宽度) + static_cast<std::size_t>(u);
    }

    [[nodiscard]] inline double 深度_at(int u, int v) const noexcept { return 深度[索引(u, v)]; }
    [[nodiscard]] inline Color 颜色_at(int u, int v) const noexcept {
        return 有效颜色() ? 颜色[索引(u, v)] : Color{ 255, 255, 255 };
    }
    [[nodiscard]] inline Vector3D 点云_at(int u, int v) const noexcept {
        return 有效点云() ? 点云[索引(u, v)] : Vector3D{};
    }
};

export enum class 枚举_坐标系 : std::uint8_t {
    未定义 = 0,
    相机坐标系 = 1,
    世界坐标系 = 2,
};

export enum class 枚举_存在观测缓冲 : std::uint32_t {
    无 = 0,
    裁剪BGR = 1u << 0,
    裁剪掩膜 = 1u << 1,
    边缘图 = 1u << 2,
    法线图 = 1u << 3,
    置信度图 = 1u << 4,
    融合权重图 = 1u << 5,
};

export inline 枚举_存在观测缓冲 operator|(枚举_存在观测缓冲 a, 枚举_存在观测缓冲 b) {
    return (枚举_存在观测缓冲)((std::uint32_t)a | (std::uint32_t)b);
}
export inline bool Has(枚举_存在观测缓冲 mask, 枚举_存在观测缓冲 bit) {
    return (((std::uint32_t)mask) & ((std::uint32_t)bit)) != 0;
}

// 统一的 ROI 描述（全图坐标）
export struct 结构体_ROI {
    int x = 0, y = 0;
    int w = 0, h = 0;
};

// 统一的图像缓冲：u8（BGR、mask、edge、confidence 都能放）
// 约定：data 为行优先连续存储，stride_bytes 可选（0=自动= w*c）
export struct 结构体_图像缓冲_u8 {
    int w = 0, h = 0, c = 0;
    int stride_bytes = 0;
    std::vector<std::uint8_t> data;

    inline bool 有效() const {
        return w > 0 && h > 0 && c > 0 && !data.empty();
    }
    inline std::size_t 期望字节数() const {
        std::size_t stride = (stride_bytes > 0) ? (std::size_t)stride_bytes : (std::size_t)(w * c);
        return stride * (std::size_t)h;
    }
    inline void 清空() { w = h = c = stride_bytes = 0; data.clear(); data.shrink_to_fit(); }
};

// 浮点缓冲：用于法线/权重（建议 float，double 太肥且没必要）
export struct 结构体_图像缓冲_f32 {
    int w = 0, h = 0, c = 0;       // 法线 c=3, 权重 c=1
    std::vector<float> data;

    inline bool 有效() const { return w > 0 && h > 0 && c > 0 && !data.empty(); }
    inline std::size_t 期望元素数() const { return (std::size_t)w * (std::size_t)h * (std::size_t)c; }
    inline void 清空() { w = h = c = 0; data.clear(); data.shrink_to_fit(); }
};
// 轮廓编码/占据签名：用 uint64_t 表达 bit-block 更贴切
export using 轮廓块64 = std::uint64_t;

export struct 结构体_存在观测 {
    // ===== 基础标识 =====
    时间戳   时间 = 0;
    枚举_坐标系 坐标系 = 枚举_坐标系::相机坐标系;

    // ===== 几何摘要（轻量，长期保留）=====
    Vector3D 中心坐标{};        // 质心（建议用鲁棒质心：截尾均值/中位数）
    double   距离 = 0.0;        // ||中心||
    Vector3D 尺寸{};            // OBB 尺寸（沿主方向的宽高深）或 AABB 尺寸
    Vector3D 主方向1{};         // PCA 主轴（单位向量）
    Vector3D 主方向2{};
    Vector3D 主方向3{};

    // 快速匹配缓存：AABB（相机系），用于粗筛
    Vector3D AABB_min{};
    Vector3D AABB_max{};

    // ===== 形状摘要（可选保留）=====
    std::vector<Vector3D> 轮廓3D;        // 若内存压力大：可改为 float 或改存稀疏关键点
    std::vector<轮廓块64>  轮廓编码;      // 8×8×n×n 的封闭内部二值图块（具体打包规则你自定义）

    // 多尺度轮廓金字塔：建议每层都知道它的分辨率
    // 0:8x8, 1:16x16, 2:32x32, 3:64x64（也可不满 4 层）
    std::array<std::vector<轮廓块64>, 4> 多尺度轮廓金字塔;
    std::array<int, 4> 金字塔_w{ 8,16,32,64 };
    std::array<int, 4> 金字塔_h{ 8,16,32,64 };
    double 质量分;

    // 可选：边界点序列（全图坐标 pack：((uint64)y<<32)|x）
    std::vector<std::uint64_t> 边界点序列;

    // ===== 外观摘要（轻量，长期保留）=====
    Color 平均颜色{};                 // 仅 r,g,b
    // 可选升级：你后续想更稳，可以再加 “颜色方差/主色”等质量字段

    // ===== 质量/置信度（强烈建议加，融合会聪明很多）=====
    std::uint32_t 有效点数 = 0;
    float 深度噪声_MAD = 0.0f;        // 深度中位绝对偏差（稳）
    float PCA残差 = 0.0f;             // 拟合残差或最小特征值比例
    float 轮廓完整度 = 1.0f;          // 0~1（缺口越多越低）
    float 颜色一致性 = 1.0f;          // 0~1（颜色方差越大越低）
    float 可见率 = 1.0f;              // 0~1，估算未被遮挡比例
    int   Z顺序层级 = 0;              // 粗深度排序用（可由距离/中心.z 量化得到）

    // ===== 与抽象挂钩（语义占位）=====
    int 存在类型候选ID = -1;

    // ===== ROI 与可选原始缓冲（大内存，按需保留）=====
    结构体_ROI ROI{};
    枚举_存在观测缓冲 缓冲掩码 = 枚举_存在观测缓冲::无;

    结构体_图像缓冲_u8 裁剪BGR;     // c=3
    结构体_图像缓冲_u8 裁剪掩膜;    // c=1, 0/1 或 0/255
    结构体_图像缓冲_u8 边缘图;      // c=1, 0~255
    结构体_图像缓冲_u8 置信度图;    // c=1, 0~255

    结构体_图像缓冲_f32 法线图;     // c=3 (nx,ny,nz)
    结构体_图像缓冲_f32 融合权重图; // c=1 (0~1)

    // ===== 轻量工具函数 =====
    inline void 重新计算距离() { 距离 = std::sqrt(中心坐标.x * 中心坐标.x + 中心坐标.y * 中心坐标.y + 中心坐标.z * 中心坐标.z); }

    inline void 清理大缓冲() {
        缓冲掩码 = 枚举_存在观测缓冲::无;
        裁剪BGR.清空();
        裁剪掩膜.清空();
        边缘图.清空();
        置信度图.清空();
        法线图.清空();
        融合权重图.清空();
    }
};

// ===== 点索引 / 点簇 =====
export struct 点索引 {
    int u = 0;
    int v = 0;
};
export using 点簇 = std::vector<点索引>;

// ===== 参数 =====
export struct 点簇分割参数 {
    // ===== 深度有效性过滤 =====
    double  最小深度 = 0.15;          // 米
    double  最大深度 = 8.0;           // 米
    bool    忽略无效点 = true;        // z<=0 或 NaN 的点直接丢弃

    // ===== 连通性判断 =====
    double  邻域最大三维距离 = 0.04;  // 米，点云可用时优先使用
    double  邻域最大深度差 = 0.06;  // 米，点云不可用时的退化判断
    bool    使用8邻域 = true;         // false=4邻域

    // ===== 性能与噪声过滤 =====
    int     采样步长 = 1;             // >1 时跳点扫描加速
    int     最小点数 = 80;            // 小于此点数的簇直接丢弃

    // ===== 裁剪输出控制 =====
    bool    输出裁剪图 = true;      // 输出 bbox+边距 的原彩图
    bool    输出裁剪掩码 = true;      // 输出二值掩码
    int     裁剪边距 = 2;         // bbox 外扩像素数
    bool    掩码膨胀一次 = true;      // 修复稀疏点断裂
    bool    掩码填洞 = true;      // 让轮廓更封闭
    int     最大裁剪像素 = 256 * 256; // 面积超大时跳过裁剪（防爆内存）

    // ===== 轮廓与外观增强控制 =====
    double  边缘颜色权重 = 0.35;      // 0~1，计算平均颜色时边缘像素权重（越小边缘影响越小，越鲁棒）

    // ===== 质量计算开关（默认全开，必要时可关闭加速）=====
    bool    计算PCA主方向 = true;
    bool    计算质量分 = true;
    bool    计算颜色一致性 = true;
    bool    计算深度噪声 = true;
    bool    计算边界噪声 = true;

    // ===== 未来扩展预留 =====
    bool    输出边界点序列 = false;   // 是否填充 边界点序列（用于精确匹配）
    bool    输出轮廓3D点 = false;   // 是否反投影得到 轮廓3D（调试/高精度匹配用）
};

// ===== 输出结构 =====
export struct 点簇边界框 {
    int umin = 0, umax = 0;
    int vmin = 0, vmax = 0;
};

export struct 点簇结果 {
    点簇        簇;
    点簇边界框  边界;
};

// ===== 增强版输出（推荐使用）=====
export struct 点簇增强结果 {
    点簇                     簇;
    点簇边界框               边界;

    // ===== 3D 几何信息（必填）=====
    Vector3D                  中心{ 0,0,0 };           // 3D质心（米）
    Vector3D                  尺寸{ 0,0,0 };           // OBB尺寸（沿PCA主方向的宽高深，米）

    Vector3D                  主方向1{ 1,0,0 };        // PCA 第一主轴（单位向量）
    Vector3D                  主方向2{ 0,1,0 };        // PCA 第二主轴
    Vector3D                  主方向3{ 0,0,1 };        // PCA 第三主轴

    // ===== 轮廓与形状摘要 =====
    std::vector<std::int64_t> 轮廓编码;                     // 8×8 = 64位封闭二值图（必填）
    std::vector<Vector3D>     轮廓3D;                       // 深度边缘反投影得到的3D轮廓点（可选，调试/精细匹配用）

    std::vector<std::uint64_t> 边界点序列;                  // 可选：全图坐标打包的边缘像素序列 (v<<32|u)

    // ===== 外观信息 =====
    Color                     平均颜色{ 255,255,255 };      // 边缘降权后的鲁棒平均颜色
    // 别名（兼容不同命名习惯）
    Color                     平均色{ 255,255,255 };

    double                    颜色一致性 = 0.0;           // 0~1，越高越均匀

    // ===== 观测质量信息（强烈推荐保留，用于后续融合加权）=====
    std::size_t               有效点数 = 0;               // 有效3D点数量
    double                    有效点比例 = 0.0;           // 有效点数 / 簇总点数

    double                    深度噪声_MAD = 0.0;         // 深度中位绝对偏差（米）
    double                    中位深度 = 0.0;             // 簇中位深度（用于噪声归一化）

    double                    PCA残差 = 1.0;              // PCA拟合残差（越小越好，已归一化） 
    double                    PCA_残差 = 1.0;
    double                    边界噪声 = 0.0;             // 深度边缘梯度统计（越小越好）

    double                    质量分 = 0.0;               // 综合质量 0~1（用于世界树融合加权）
    // 别名（兼容不同项目命名）
    double                    可信度 = 0.0;
    double                    质量 = 0.0;

    // ===== 裁剪输出（可选，大内存，按需开启）=====
    int                       裁剪宽 = 0;
    int                       裁剪高 = 0;
    std::vector<Color>        裁剪颜色;                  // bbox+边距内的原彩色图
    std::vector<std::uint8_t> 裁剪掩码;                  // 0/1 二值掩码（已膨胀+填洞）

};

// ==============================
// 存在观测（与三维场景管理模块同步）
// ==============================


// ==============================
// 参数
// ==============================
export struct 观测提取参数 {
    // ===== 基础门槛 =====
    std::size_t 最小有效点数 = 80;

    // 尺寸计算（暂不使用 PCA）
    bool 使用PCA主轴尺寸 = false;

    // 轮廓采样（旧流程用，现可忽略）
    int 最大轮廓点数 = 128;
    bool 轮廓使用8邻域 = true;

    // 点云无效点（z<=0）处理
    bool 忽略无效点 = true;

    // =========================================================
    // ✅ 补全：输出结果校验与“必须项”控制（你上条需求需要这些）
    // =========================================================

    // 颜色是否为必须项：true 时算不出平均颜色 => 该观测整体无效
    bool 要求颜色 = true;

    // 轮廓编码是否为必须项：true 时轮廓编码为空 => 该观测整体无效
    bool 要求轮廓编码 = true;

    // 轮廓编码维度是否必须为 64（8×8）
    bool 严格轮廓维度64 = true;

    // 平均颜色的采样步长：1=全采样；2/3=加速
    int 颜色采样步长 = 2;

    // 观测中心必须在相机前方：中心.z < 最小中心Z => 无效
    double  最小中心Z = 0.05f;

    // 尺寸下限（防止 0 或极小噪声簇）：任一轴 < 最小尺寸 => 无效
    double  最小尺寸 = 0.01f;


 /////////////////X////////////////////////////////

    bool 生成边缘图 = true;
    bool 生成法线图 = true;
    bool 生成置信度图 = true;
    bool 生成多尺度轮廓 = false;  // 消耗较大，慎开

    
};




/////////////////////////////////////////////////////////////////////////////////
//相机模块参数         截至                                            //////////
/////////////////////////////////////////////////////////////////////////////////