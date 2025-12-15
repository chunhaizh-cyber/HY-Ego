module;
#include <afx.h>
//#include <vector>
//#include <variant>

export module 基础数据类型模块;
import <variant>;
 import <string>;
import <vector>;
import <chrono>;

export enum class 枚举_比较条件 { 相等, 大于, 小于, 不等于, 大于等于,小于等于, 包含, 除外,相似,未定义};
export enum class 枚举_比较字段 {
    未定义,
    //特征值类//
    名称,
    类型,
    值,
    //语素类//
    语素_词_词,

    语素_词性_词性,
    语素_词性_对应基础信息指针,

    语素_短语字节点_参照词,
    语素_短语字节点_比较对象词,
    语素_短语子节点_比较词,
    语素_短语子节点_结果词,
    //基础类
    基础信息_矢量特征值_类型,
    基础信息_矢量特征值_单位,
    基础信息_矢量特征值_值,

    基础信息_非矢量特征值_类型,
    基础信息_非矢量特征值_值,
    基础信息_字符串特征值_值,

    基础信息基类_名称,
    基础信息基类_类型,
    基础信息_指代节点_代词,
    基础信息_指代节点_指代对象,

    基础信息_特征节点_类型,
    基础信息_特征节点_值,
    基础信息_特征节点_名称,

    基础信息_存在节点_绝对坐标_z轴,
    基础信息_存在节点_绝对坐标_y轴,
    基础信息_存在节点_绝对坐标_x轴,

    基础信息_场景节点_绝对坐标_z轴,
    基础信息_场景节点_绝对坐标_y轴,
    基础信息_场景节点_绝对坐标_x轴,

    基础信息_状态节点主信息类_收到时间,
    基础信息_状态节点主信息类_发生时间,
    基础信息_状态节点主信息类_是否变化,

   


    实例信息,
    词性词指针,
    原始短语,
    词指针,
    单位,
    状态类型_特征,
    状态类型_存在,
    状态类型_场景,
    产生需求主体,
    需求对象,
    需求状态,
   
    // ...已有字段
    评估_覆盖率, 评估_指代解析率, 评估_一致性_冲突数,
    评估_可执行性, 评估_全局置信度,
    评估_时间对齐误差, 评估_空间对齐误差,
    评估_比较正确率, 评估_否定正确率, 评估_程度正确率


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




export enum class 枚举_特征值类型 : std::int32_t
{
    未定义 = 0,
    矢量,
    标量,
    字符串
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
    附属世界,
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
};


// 为了以后扩展清晰，加一个比较模式标记
export enum class 枚举_特征值比较模式 {
    有序标量 = 0,      // 支持 <、>、范围、差值
    相等或相似度       // 只支持 == / != 或相似度，不支持大小关系
};
// 简单的颜色结构体，替代OpenCV的viz::Color
// ===== 基础类型 =====
export struct Color {
    uint8_t r = 255, g = 255, b = 255;
};


/////////////////////////////////////////////////////////////////////////////////
// 相机模块参数
// 在 Vector3D 结构体定义后添加（同一模块内）

export struct Vector3D {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    // 可选：构造函数
    Vector3D() = default;
    constexpr Vector3D(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

    // ===== 关键：标量乘法（成员函数）=====
    constexpr Vector3D operator*(float scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }

    // ===== 推荐同时添加：标量除法 =====
    constexpr Vector3D operator/(float scalar) const {
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
    Vector3D& operator*=(float scalar) {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }

    Vector3D& operator/=(float scalar) {
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

// ===== 额外：非成员函数版本，支持 float * Vector3D（如 0.5 * vec）=====
export constexpr Vector3D operator*(float scalar, const Vector3D& vec) {
    return vec * scalar;
}

export constexpr Vector3D operator/(float scalar, const Vector3D& vec) {
    return Vector3D(scalar / vec.x, scalar / vec.y, scalar / vec.z); // 注意：语义不同，通常不常用
}



// ===== 原始场景帧 =====
export struct 结构体_原始场景帧 {
    std::uint64_t 时间;
    int 宽度 = 0;
    int 高度 = 0;

    std::vector<float> 深度;       // 深度图 Z (米)
    std::vector<Color> 颜色;       // RGB 图
    std::vector<Vector3D> 点云;    // 相机坐标系下生成的点云（可选）
	std::vector < std::int64_t>  轮廓;   // 点云轮廓编码（可选）
};

export struct 结构体_存在观测 {
    时间戳 时间;
    // 几何信息（相机坐标系）
    Vector3D 中心坐标;          // 所有点平均 / 质心
    Vector3D 尺寸;              // (宽, 高, 深) 或三个主方向长度
    double   距离 = 0.0;        // ||中心||
    // 轮廓可以同时存 2D 和 3D
    std::vector<Vector3D> 轮廓3D;
    Color 平均颜色;  // 仅 r,g,b 三通道
    // 可选：主方向（PCA）
    Vector3D 主方向1;
    Vector3D 主方向2;
    Vector3D 主方向3;  

    // 与“数字生命”的抽象挂钩
    // 存在类型 / 标签先留空，后面由识别模块填写
    int  存在类型候选ID = -1;   
    std::vector<std::int64_t> 轮廓编码;        // 核心：8×8*n*n=64*n*n 维封闭内部二值图
};

// ===== 点索引 / 点簇 =====
export struct 点索引 {
    int u = 0;
    int v = 0;
};
export using 点簇 = std::vector<点索引>;

// ===== 参数 =====
export struct 点簇分割参数 {
    // 只考虑这个距离范围内的点（米）
    float 最小深度 = 0.15f;
    float 最大深度 = 8.0f;

    // 邻域连接阈值（米）
    float 邻域最大三维距离 = 0.04f;

    // 当点云不可用时，用深度差做退化连接判断（米）
    float 邻域最大深度差 = 0.06f;

    // 4 邻域 or 8 邻域
    bool 使用8邻域 = true;

    // 性能优化：>1 时跳点扫描
    int 采样步长 = 1;

    // 过滤噪声：点数太少的簇丢弃
    int 最小点数 = 80;

    // 点云 z==0 的点是否直接忽略
    bool 忽略无效点 = true;

    // ===== 新增：裁剪输出控制 =====
    bool 输出裁剪图 = true;               // 输出 bbox 内原图
    bool 输出裁剪掩码 = true;             // 输出 bbox 内二值掩码
    int  裁剪边距 = 2;                    // bbox 外扩像素
    bool 掩码膨胀一次 = true;             // 修补稀疏点导致的断裂
    bool 掩码填洞 = true;                 // 封闭轮廓填洞
    int  最大裁剪像素 = 256 * 256;        // 裁剪面积过大则跳过（防内存爆）
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
    Vector3D                  中心 = { 0.0f, 0.0f, 0.0f };   // 3D 中心（米）
    Vector3D                  尺寸 = { 0.0f, 0.0f, 0.0f };   // 3D 包围盒尺寸（米）
    std::vector<std::int64_t> 轮廓编码;                     // 8×8 = 64 维二值图（封闭内部已填充）
    std::vector<Vector3D>     轮廓3D;                       // 可选：调试用（不强制填）

    // ===== 新增：按轮廓/掩码裁剪原图输出（bbox 内）=====
    int 裁剪宽 = 0;
    int 裁剪高 = 0;
    std::vector<Color>        裁剪颜色;    // size = 裁剪宽 * 裁剪高
    std::vector<std::uint8_t> 裁剪掩码;    // size = 裁剪宽 * 裁剪高, 0/1
};

// ==============================
// 存在观测（与三维场景管理模块同步）
// ==============================


// ==============================
// 参数
// ==============================
export struct 观测提取参数 {
    // ===== 基础门槛 =====
    int 最小有效点数 = 80;

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
    float 最小中心Z = 0.05f;

    // 尺寸下限（防止 0 或极小噪声簇）：任一轴 < 最小尺寸 => 无效
    float 最小尺寸 = 0.01f;
};




/////////////////////////////////////////////////////////////////////////////////
//相机模块参数         截至                                            //////////
/////////////////////////////////////////////////////////////////////////////////