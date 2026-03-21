module;
#ifdef HY_HEADLESS_NO_MFC
#include "hy_windows.h"
#else
#include <afx.h>
#endif
//#include <vector>
//#include <variant>

#include <variant>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <array>
#include <cmath>
export module 基础数据类型模块;



/////////////////////////////////////////////////////////////
/////          一. 枚举结构定义                         /////
/////////////////////////////////////////////////////////////



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

    //==================== 特征值 ====================
    特征值_值,

    特征值_类型,
    特征值_命中次数,
    特征值_可信度,


    //==================== 特征节点 ====================
    基础信息_特征节点_类型,
    基础信息_特征节点_值,
    基础信息_特征节点_名称,
    基础信息_特征节点_当前值,
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

//============================ 通用枚举（保留） ============================
// 注意：这是多态类区分主信息用
// 约定：
//  - 需要的主信息基类中定义字段:"主信息类型"
export enum class 枚举_主信息类型 {
    指代, 非矢量特征值, 矢量特征值, 短语子, 基础信息基类, 语素基类,
    词, 词性, 短语, 短语子节点, 特征, 特征_特征, 特征_存在, 存在, 状态,
    场景, 状态的特征, 动态的特征, 状态_特征, 状态_存在, 状态_场景,
    动态, 因果, 关系, 其它, 存在信息, 存在信息_指代, 动态信息, 特征信息,
    二次特征_修饰, 特征信息_数量, 关系信息, 情绪特征信息, 情绪调节信息, 语言信息_仅记录, 未定义
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

export enum class 枚举_语言用途 {
    存在_陈述, 特征_描述, 事件_叙述, 关系_布局, 比较_排序,
    数量_范围, 否定_排除, 疑问_信息需求, 指令_意图,
    条件_假设, 因果_解释, 时间_体貌, 评价_立场,
    来源_引述, 定义_分类, 角色_标注, 约束_义务, 例外_对照, 单位_度量
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

export using 置信度 = std::uint64_t;
export using 需求强度 = std::uint64_t;
export using 特征值 = std::uint64_t;
export using 时间戳 = std::uint64_t;

export struct 结构体_时间戳 {
    时间戳 值 = 0; // 约定：单位 = 微秒 us（steady_clock）

    // 当前时间戳（微秒）
    static 时间戳 当前_微秒() noexcept {
        using clock = std::chrono::steady_clock;
        using us = std::chrono::microseconds;
        return static_cast<时间戳>(
            std::chrono::duration_cast<us>(clock::now().time_since_epoch()).count()
            );
    }

    // 当前时间戳（纳秒，可选）
    static 时间戳 当前_纳秒() noexcept {
        using clock = std::chrono::steady_clock;
        using ns = std::chrono::nanoseconds;
        return static_cast<时间戳>(
            std::chrono::duration_cast<ns>(clock::now().time_since_epoch()).count()
            );
    }

    // 计算时间差（微秒，返回无符号；如果担心顺序，提供一个有符号版本更安全）
    static 时间戳 时间差_微秒(时间戳 t0, 时间戳 t1) noexcept {
        return (t1 >= t0) ? (t1 - t0) : (t0 - t1);
    }

    // 更推荐：有符号差值（允许 t1 < t0 的情况）
    static std::int64_t 时间差_微秒_有符号(时间戳 t0, 时间戳 t1) noexcept {
        return static_cast<std::int64_t>(t1) - static_cast<std::int64_t>(t0);
    }
};


//////////////////////////////////////////////////////////////////////////
            /////// ====== 特征值相关======/////////////////// 
//////////////////////////////////////////////////////////////////////////

export enum class 枚举_特征值类型 : std::uint8_t {
    未定义 = 0,
    I64 = 1,
    VecIU64 = 2,
    指针 = 3,
};

export enum class 枚举_比较模式 : std::uint8_t {
    //约定：树排序永远用 严格比较_用于排序，相似度永远走 计算距离 / 相似度，不要混用。
    // ====== 1) 可用于排序（严格弱序）======
    // 用途：放入 map/set 或树结构时做稳定排序；也可用于“严格相等”判断
    // 要求：必须满足可传递、可重复、无随机性

    有序标量 = 0,
    // 适用：I64
    // 语义：按数值大小比较（<,>,==），用于可排序物理量（长度、坐标轴分量、计数等）

    字典序向量 = 1,
    // 适用：VecI64 / VecIU64（仅当你希望“按内容排序”而非相似度）,严格比较_用于排序
    // 语义：逐元素字典序比较（先比第0维，若相等再比第1维...）
    // 注意：这不是“距离”，更像“稳定排序键”；不适合相似匹配打分
    字符串 = 3,

    // ====== 2) 相似度/距离（用于识别/聚类/命中打分）======
    // 用途：两值“像不像”，返回距离或相似度；常用于枚举池命中、聚合、投票
    // 注意：这类模式通常不提供严格弱序（不适合直接当树排序键）

    海明距离 = 10,
    // 适用：VecIU64（位签名、体素块、轮廓块等）
    // 语义：按 bit 计数差异（xor 后 popcount）；越小越相似
    // 常见：Bits64签名、体素占据签名、二值轮廓编码

    L1距离 = 11,
    // 适用：I64（标量时=|a-b|）、VecI64/VecIU64（逐维绝对差求和）
    // 语义：曼哈顿距离；对异常值比 L2 更“稳”，适合粗匹配/鲁棒聚合

    L2距离 = 12,
    // 适用：I64（标量时=(a-b)^2 或 |a-b| 再开方取决于实现）、VecI64/VecIU64
    // 语义：欧氏距离；更强调大偏差，适合几何量的连续变化度量

    余弦相似度 = 13,
    // 适用：VecI64/VecIU64（先转浮点/归一化再算点积）
    // 语义：比较“方向/形状比例”相似，不关心整体幅值（适合方向向量、分布向量）
    // 注意：你已经去浮点，通常需要在实现里做定点近似或临时浮点计算

    // ====== 3) 特化模式（语义更强）======

    颜色距离_BGR = 20,
    // 适用：颜色_BGR8 或 VecI64(3维)/VecIU64(3维) 的颜色向量表示
    // 语义：在 BGR 空间的距离（常用 L1 或 L2）；用于颜色聚合/命中
    // 备注：如果你保留了 颜色_BGR8 类型，推荐直接针对它实现更省分支

    区间关系 = 30
    // 适用：区间摘要（特征节点的区间 side-car），或值节点自带区间时
    // 语义：判断区间包含/相交/相离/相等（用于“命中”与“约束匹配”）
    // 备注：对不可排序签名型值，不建议用值域区间；可选用“距离到原型区间”
};

// =========================
// 2) 载体类型（OpenCV 对齐 BGR8）
// =========================
export using I64 = std::int64_t;
export using VecI64 = std::vector<I64>;
export using VecIU64 = std::vector<std::uint64_t>;
export using 字符串UTF8 = std::string;


// ============================================================
// VecU 句柄 + 特征快照值（Int 内联 + VecU 证据池）
// ------------------------------------------------------------
// 约束：
// - VecU 的真实数据存放在“特征值链（证据池）”，特征节点只持有句柄。
// - Int（I64）作为特征的快照/统计，直接内联在特征节点主信息中。
// - 指针（Ptr）只用于运行期句柄，不建议持久化；本轮先不纳入快照值。
// ============================================================


export constexpr I64 I64_MIN = (std::numeric_limits<I64>::min)();
export constexpr I64 I64_MAX = (std::numeric_limits<I64>::max)();

//////////////////////////////////////////////////////////////////////////////////////////
export struct I64区间 {
    std::int64_t 低值 = 0;
    std::int64_t 高值 = 0;

    constexpr bool 有效() const noexcept { return 低值 <= 高值; }
    constexpr bool 是否点() const noexcept { return 低值 == 高值; }
    constexpr std::int64_t 点值() const noexcept { return 低值; }
    constexpr bool 包含(std::int64_t v) const noexcept { return v >= 低值 && v <= 高值; }

    static constexpr I64区间 点(std::int64_t v) noexcept { return I64区间{ v, v }; }
};



export struct I64多维区间 {


    std::vector<I64区间> 维度{};  // 多维区间：每个维度一个 {lo,hi} 标量区间：维度.size()==1
    bool 有效() const noexcept {
        if (维度.empty()) return false;
        for (const auto& d : 维度) if (d.低值 > d.高值) return false;
        return true;
    }
    std::size_t 维度数() const noexcept { return 维度.size(); }
};




export enum class 枚举_区间语义 : std::uint8_t {
    未定义 = 0,
    值域 = 1,          // 可排序值：真实取值范围（通常用于 I64 标量）
    距离到原型 = 2,     // 不可排序值：与主原型的“差异度/距离”范围（越小越好，通常由 diff() 给出）
    相似度阈值 = 3      // 不可排序值：与主原型的“相似度Q10000”阈值/范围（越大越好，通常由 sim() 给出）
};


//////////////////////////////////////////////////////////////////////////
            /////// ====== 特征值相关截至======/////////////////// 
//////////////////////////////////////////////////////////////////////////



export struct 点阵图
{
    std::int32_t 宽 = 0;
    std::int32_t 高 = 0;
    // 所有像素按行优先展平，每 64 个像素压缩成一个 64 位整数
    std::vector<std::uint64_t> 数据;
};


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



// ===== 基础类型 =====
export struct Color {
    uint8_t r = 255, g = 255, b = 255;
};
export struct 颜色_BGR8 {
    std::uint8_t b{}, g{}, r{};
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

// ============================ 姿态/位姿（相机状态） ============================
// 约定：
// - 四元数按 (w,x,y,z) 存储。
// - 本工程“视角键 ViewKey”只依赖量化后的姿态，因此只要同一工程内一致即可。

export struct Quaternion {
    double w = 1.0;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    static Quaternion 单位() noexcept { return {}; }

    double 范数() const noexcept { return std::sqrt(w * w + x * x + y * y + z * z); }
    void 归一化() noexcept {
        const double n = 范数();
        if (n <= 0.0) { w = 1.0; x = y = z = 0.0; return; }
        w /= n; x /= n; y /= n; z /= n;
    }

    Quaternion 共轭() const noexcept { return { w, -x, -y, -z }; }

    // 复合旋转：this ⊗ r
    Quaternion operator*(const Quaternion& r) const noexcept {
        return {
            w * r.w - x * r.x - y * r.y - z * r.z,
            w * r.x + x * r.w + y * r.z - z * r.y,
            w * r.y - x * r.z + y * r.w + z * r.x,
            w * r.z + x * r.y - y * r.x + z * r.w
        };
    }

    // 旋转向量：v' = q ⊗ (0,v) ⊗ q*  (注意：取决于你对 q 的语义约定；此处提供常用形式)
    Vector3D 旋转(const Vector3D& v) const noexcept {
        Quaternion p{ 0.0, v.x, v.y, v.z };
        Quaternion qn = *this;
        qn.归一化();
        Quaternion out = (qn * p) * qn.共轭();
        return { out.x, out.y, out.z };
    }
};

export struct 结构体_相机状态 {
    // 设备时间戳（推荐使用 RealSense 设备时间域转换到微秒；若无则可用 0）
    时间戳 设备时间_微秒 = 0;
    Quaternion 姿态{};
    Vector3D 位置{ 0,0,0 }; // 目前可先为 0；后续 VIO/SLAM 再补
    bool 有效 = true;
};
export enum class 运行状态 : std::int8_t {
    停止= 0,
    启动中 = 1,
    运行中 = 2,
    停止中 = 3,
    故障 = 4
};
// ===== 额外：非成员函数版本，支持 double  * Vector3D（如 0.5 * vec）=====
export constexpr Vector3D operator*(double  scalar, const Vector3D& vec) {
    return vec * scalar;
}

export constexpr Vector3D operator/(double  scalar, const Vector3D& vec) {
    return Vector3D(scalar / vec.x, scalar / vec.y, scalar / vec.z); // 注意：语义不同，通常不常用
}
////处理一帧 函数带出数据参数//////////////////////////////
export struct 结构体_轮廓观测 {
    int 帧内序号 = 0;

    // ROI（在“对齐后的深度/彩色图”坐标系中）
    int x = 0, y = 0, w = 0, h = 0;

    // 轮廓掩膜：按 ROI 原始尺寸存储（0/1），后续“特征值”阶段再按规则缩放/编码
    // size = w*h
    std::vector<std::uint8_t> 掩膜;

    // 轮廓对应的原始彩色裁剪图（与掩膜同尺寸）
    // size = w*h
    std::vector<Color> 裁剪颜色;  //彩色截图

    // 对应点云簇（相机坐标系，单位：毫米）
    // size ~= mask 中有效像素数（仅统计有效深度点）
    std::vector<Vector3D> 点云簇;

    // 可选：边界点（全图坐标），打包为 (y<<32 | x)
    std::vector<std::int64_t> 边界点;
    // 统计信息（相机坐标系，单位：毫米）
    Vector3D 中心{ 0,0,0 };
    Vector3D 尺寸{ 0,0,0 };
    Color    平均颜色{ 255,255,255 };
    int      像素数 = 0;
    std::int64_t 跟踪ID = -1;

    // 彩色截图（按方案1：在轮廓观测构建阶段生成）
    std::vector<std::uint8_t> 彩色截图;                  // 压缩后的彩色截图字节流
    std::vector<std::int64_t> 彩色截图元信息;            // 元信息：[宽度, 高度, 格式, 步长, 有无alpha, 压缩格式]

    bool 有效 = false;
};


export enum class 枚举_时间域 : std::uint8_t {
    未知 = 0,
    设备硬件时钟 = 1,   // e.g. HARDWARE_CLOCK
    系统时钟 = 2,       // e.g. SYSTEM_TIME
    全局时间 = 3        // e.g. GLOBAL_TIME
};

export struct 结构体_帧时间戳 {
    // 设备时间：适合做帧间/IMU同步（高精度）
    std::uint64_t 设备时间_us = 0;

    // 系统到达时间：适合跨线程排队、跨设备对齐、日志定位
    std::uint64_t 系统到达时间_us = 0;

    枚举_时间域 域 = 枚举_时间域::未知;

    // 帧号：用于查丢帧、串帧（若设备支持该 metadata）
    std::uint32_t 深度帧号 = 0;
    std::uint32_t 彩色帧号 = 0;
};

export struct 结构体_帧元数据 {
    bool 有效 = false;

    // 这些字段取不到时保持默认值即可（不要硬编）
    int 曝光_us = -1;
    int 增益 = -1;
    int 白平衡_K = -1;

    // 深度相关
    int 激光功率 = -1;

    double 温度_C = 0.0;
};

export struct 结构体_IMU样本 {
    // 建议与深度帧使用同一设备时间域，便于融合
    std::uint64_t 设备时间_us = 0;

    // IMU 的物理单位建议保持常见 SI：m/s^2 与 rad/s
    Vector3D 加速度_m_s2{};
    Vector3D 角速度_rad_s{};
};
export struct 结构体_相机内参 {
    // 像素坐标系内参（与 帧.宽度/高度 对应）
    double fx = 0.0;
    double fy = 0.0;
    double cx = 0.0;
    double cy = 0.0;
    int 宽度 = 0;
    int 高度 = 0;
    bool 有效 = false;
};
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
    // 单位约定：几何量/深度/点云 = mm，像素坐标 = px
    结构体_帧时间戳 时间戳;

    int 宽度 = 0;
    int 高度 = 0;
    // 深度内参（用于世界模型投影/轮廓融合等）
    结构体_相机内参 深度内参;
    // 这一帧的对齐约定：所有按像素展开的数组，是否处于“彩色坐标系”
    bool 深度已对齐到彩色 = true;

    // 深度单位（可选但强烈建议）：1个原始深度单位对应多少 mm
    // 你如果已经输出的是 mm 深度，这个字段用于复现/校验
    double 深度单位_mm = 0.0;

    // 深度图 Z（mm），idx = v*宽度 + u
    std::vector<double> 深度;

    // 深度有效掩膜（可选）：1=有效，0=无效/孔洞/被过滤
    // 用于“每个mask点都要三维坐标”的验收与调试
    std::vector<std::uint8_t> 深度有效;

    // RGB 图（可选），size==深度.size() 时有效
    std::vector<Color> 颜色;

    // IR（可选）：若你启用左右红外流，可放这里（具体格式按你采集设置）
    std::vector<std::uint8_t> 红外1;
    std::vector<std::uint8_t> 红外2;

    // 点云（可选），size==深度.size() 时表示“每像素点云”
    std::vector<Vector3D> 点云;

    // 每帧的传感器元数据（可选）
    结构体_帧元数据 元数据_深度;
    结构体_帧元数据 元数据_彩色;

    // 这一帧期间收集到的 IMU 原始样本（可选但建议）
    std::vector<结构体_IMU样本> IMU样本;

    // 同步采集到的相机状态（姿态、视角键等你已有内容）
    结构体_相机状态 相机;

    // 可选：前景提示掩膜（如果你还保留背景差分流程）
    // 1=前景提示，0=非前景。注意它不等于“存在”，只是提示信息。
    std::vector<std::uint8_t> 前景提示;

    std::vector<结构体_轮廓观测> 轮廓观测列表;

    [[nodiscard]] bool 有效深度() const noexcept {
        return 宽度 > 0 && 高度 > 0
            && 深度.size() == static_cast<std::size_t>(宽度) * static_cast<std::size_t>(高度);
    }
    [[nodiscard]] bool 有效颜色() const noexcept { return 颜色.size() == 深度.size(); }
    [[nodiscard]] bool 有效点云() const noexcept { return 点云.size() == 深度.size(); }

    [[nodiscard]] bool 有效深度掩膜() const noexcept { return 深度有效.size() == 深度.size(); }
    [[nodiscard]] bool 有效前景提示() const noexcept { return 前景提示.size() == 深度.size(); }

    [[nodiscard]] inline std::size_t 索引(int u, int v) const noexcept {
        return static_cast<std::size_t>(v) * static_cast<std::size_t>(宽度) + static_cast<std::size_t>(u);
    }

    [[nodiscard]] inline double 深度_at(int u, int v) const noexcept { return 深度[索引(u, v)]; }

    [[nodiscard]] inline bool 深度有效_at(int u, int v) const noexcept {
        return 有效深度掩膜() ? (深度有效[索引(u, v)] != 0) : (深度_at(u, v) > 0.0);
    }

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


// ===== 点索引 / 点簇 =====
export struct 点索引 {
    int u = 0;
    int v = 0;
    点索引(int uu, int vv) : u(uu), v(vv) {}
    点索引() {}
};
export using 点簇 = std::vector<点索引>;
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


export struct 结构体_存在观测 {
    // ===== 基础标识 =====
    bool 轮廓观测有效 = false;
    时间戳   时间 = 0;
    枚举_坐标系 坐标系 = 枚举_坐标系::相机坐标系;

    // 相机位姿快照（方案C：用于cam→world坐标转换）
    Quaternion 相机_q_world_cam{};        // 相机姿态（world←cam）
    Vector3D 相机_t_world_cam{};          // 相机位置（world←cam）
    bool 相机位姿有效 = false;           // 相机位姿是否有效

    // 彩色截图（按方案1：在轮廓观测构建阶段生成）
    std::vector<std::uint8_t> 彩色截图;                  // 压缩后的彩色截图字节流
    std::vector<std::int64_t> 彩色截图元信息;            // 元信息：[宽度, 高度, 格式, 步长, 有无alpha, 压缩格式]

    // ===== 几何摘要（轻量，长期保留）=====
    Vector3D 中心坐标{};        // 质心（毫米, mm；建议用鲁棒质心：截尾均值/中位数）
    bool 坐标有效 = 0;
    double   距离 = 0.0;        // ||中心||（毫米, mm）
    Vector3D 尺寸{};            // OBB 尺寸（沿主方向的宽高深）或 AABB 尺寸（毫米, mm）
    Vector3D 主方向1{};         // PCA 主轴（单位向量）
    Vector3D 主方向2{};
    Vector3D 主方向3{};
    std::int64_t 主方向角差_deg = 0;
    std::int64_t 主方角度_万分度 = 0; // 主方向1 在 XY 平面的方位角（万分度，0~35999）

    // 快速匹配缓存：AABB（相机系），用于粗筛
    Vector3D AABB_min{};
    Vector3D AABB_max{};

    // ===== 形状摘要（可选保留）=====
    std::vector<Vector3D> 轮廓3D;        //带深度数据8*8*n*n的封闭内部的图块
    VecIU64 轮廓编码;                // 8×8×n×n 的封闭内部二值图块
    std::vector<颜色_BGR8> 轮廓编码点颜色;
    // 平时不填（empty），仅当本帧触发雕刻时填
    VecIU64 三维模型_体素立方体;     // blocks
    std::vector<std::uint32_t> 三维模型_体素颜色_RGBA;   // AARRGGBB
    std::int32_t 体素_nb = 0;
    std::int32_t 体素边长_mm = 0;
    bool 本帧包含三维模型更新 = false;


    int 轮廓尺度N = 0;		  // 轮廓编码的尺度 n（对应 n×n 块数），建议 1~8 范围内
    int 轮廓标准边长 = 0; // 轮廓编码时对应的实际边长（标准尺度 8*n*n）   
    int 轮廓最大边长 = 0;        // 轮廓点总数（可选）
    int 轮廓最大N = 0;
    //   Vector3D 轮廓尺寸{ 0.0,0.0,0.0 };


    double 质量分;

    // 可选：边界点序列（全图坐标 pack：((uint64)y<<32)|x）
    std::vector<std::uint64_t> 边界点序列;
    std::vector<std::uint64_t> 边界点颜色序列;

    // ===== 外观摘要（轻量，长期保留）=====
    颜色_BGR8 平均颜色{};                 // 仅 r,g,b
    bool 平均颜色可信 = 1;
    // 可选升级：你后续想更稳，可以再加 “颜色方差/主色”等质量字段

    // ===== 质量/置信度（强烈建议加，融合会聪明很多）=====
    std::uint32_t 有效点数 = 0;
    float 深度噪声_MAD = 0.0f;        // 深度中位绝对偏差（毫米, mm；稳）
    float PCA残差 = 0.0f;             // 拟合残差或最小特征值比例
    float 轮廓完整度 = 1.0f;          // 0~1（缺口越多越低）
    float 颜色一致性 = 1.0f;          // 0~1（颜色方差越大越低）
    float 可见率 = 1.0f;              // 0~1，估算未被遮挡比例
    int   Z顺序层级 = 0;              // 粗深度排序用（可由距离/中心.z 量化得到）
    float 点云覆盖率 = 0.0;
    float 点云覆盖率有效 = 0.0;

    bool 主方向右手系 = 1;
    std::vector<Vector3D>         点云簇;
    std::vector<Vector3D>        点云簇3D;


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
    ///////////////////////////////////////////////////////////////////////////////////
    std::shared_ptr<const 结构体_原始场景帧> 原始场景帧;
    std::vector<结构体_轮廓观测> 轮廓观测;
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

    std::int64_t 跟踪ID = -1;
};

// ====== 追加：表面类型（粗分类）======
export enum class 枚举_表面类型 : int {
    未知 = 0,
    光滑 = 1,
    粗糙 = 2,
    透明 = 3,
    金属 = 4,
};

// ====== 追加：抓取建议 ======
export enum class 枚举_抓取类型 : int {
    未知 = 0,
    侧抓 = 1,
    顶抓 = 2,
    吸盘 = 3,
};

export struct 抓取位姿建议 {
    Vector3D        法向量{ 0,0,1 };          // 建议抓取面法向（单位向量）
    Vector3D        相对中心偏移{ 0,0,0 };     // 相对物体中心的抓取点偏移（mm）
    double          评分 = 0.0;                // 0~1，越大越推荐
    枚举_抓取类型   类型 = 枚举_抓取类型::未知;
};


// ===== 参数 =====

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
    点簇                     簇;           //核心数据：簇内所有像素索引（u,v），用于还原/调试/精细像素级处理
    点簇边界框               边界;         //簇的2D包围盒（像素坐标），用于快速裁剪/碰撞检测/ROI提取
    std::vector<Vector3D>    点云簇;       //簇内所有有效3D点（毫米），用于PCL / Open3D处理 / 三维重建 / 抓取规划  

    // ===== 3D 几何信息（必填）=====
    Vector3D                  中心{ 0,0,0 };           // 3D质心（毫米, mm）
    Vector3D                  尺寸{ 0,0,0 };           // OBB尺寸（沿PCA主方向的宽高深，毫米, mm）
    Vector3D                  主方向1{ 1,0,0 };        // PCA 第一主轴（单位向量）
    Vector3D                  主方向2{ 0,1,0 };        // PCA 第二主轴
    Vector3D                  主方向3{ 0,0,1 };        // PCA 第三主轴
    // bin = (Hbin<<5) | (Sbin<<2) | Vbin
    std::vector<int> 颜色直方图;   // HSV直方图（bin编码），用于颜色特征匹配/分类

    // ===== 轮廓与形状摘要 =====
    VecIU64 轮廓编码;                                   // 原始轮廓编码，小于8*8的忽略
    std::vector<Vector3D>    轮廓3D;                   // 深度边缘反投影得到的3D轮廓点（可选，调试/精细匹配用）

    VecIU64 边界点序列;                            // 可选：全图坐标打包的边缘像素序列 (v<<32|u)

    // ===== 外观信息 =====
    颜色_BGR8                     平均颜色{ 255,255,255 };      // 边缘降权后的鲁棒平均颜色    
    颜色_BGR8                     颜色;
    double                    颜色一致性 = 0.0;           // 0~1，越高越均匀

    // ===== 观测质量信息（强烈推荐保留，用于后续融合加权）=====
    std::size_t               有效点数 = 0;               // 有效3D点数量
    double                    有效点比例 = 0.0;           // 有效点数 / 簇总点数

    double                    深度噪声_MAD = 0.0;         // 深度中位绝对偏差（毫米, mm）
    double                    中位深度 = 0.0;             // 簇中位深度（毫米, mm；用于噪声归一化）

    double                    PCA残差 = 1.0;              // PCA拟合残差（越小越好，已归一化）   
    double                    边界噪声 = 0.0;             // 深度边缘梯度统计（越小越好）
    double                    轮廓完整度 = 0.0;           // 轮廓的完整性和封闭性（0~1，越高越完整）

    double                    质量分 = 0.0;               // 综合质量 0~1（用于世界树融合加权）  

    // ===== 裁剪输出（可选，大内存，按需开启）=====
    // 说明：裁剪窗口为 bbox 外扩 参数.裁剪边距 后的实际矩形。
    // 为了让下游(轮廓观测/雕刻)能正确把裁剪掩码映射回全图坐标，必须同时保存裁剪窗口左上角。
    int                       裁剪x = 0;               // 裁剪窗口左上角 u0（像素）
    int                       裁剪y = 0;               // 裁剪窗口左上角 v0（像素）
    int                       裁剪宽 = 0;
    int                       裁剪高 = 0;
    std::vector<Color>        裁剪颜色;                  // bbox+边距内的原彩色图
    std::vector<std::uint8_t> 裁剪掩码;                  // 0/1 二值掩码（已膨胀+填洞）
    int 轮廓尺度N = 0;
    int 轮廓标准边长 = 0;
    Vector3D 轮廓尺寸{ 0.0,0.0,0.0 };

    // 彩色截图（按方案1：在轮廓观测构建阶段生成）
    std::vector<std::uint8_t> 彩色截图;                  // 压缩后的彩色截图字节流
    std::vector<std::int64_t> 彩色截图元信息;            // 元信息：[宽度, 高度, 格式, 步长, 有无alpha, 压缩格式]
    // 格式说明：
    // - 宽度：截图宽度（像素）
    // - 高度：截图高度（像素）
    // - 格式：0=RGB8, 1=RGBA8, 2=BGR8, 3=BGRA8
    // - 步长：每行字节数（可能为0表示连续存储）
    // - 有无alpha：0=无，1=有
    // - 压缩格式：0=未压缩, 1=PNG, 2=JPEG, 3=其他


};
// ============================================================================
// 帧处理结果：把“一帧”处理链路的所有中间产物一次性带出，
// 让上层（主窗口/写入器）可以选择：只更新存在、或把结果投喂异步写入线程。

export struct 帧处理结果 {
    std::shared_ptr<结构体_原始场景帧> 原始场景帧;                 // W/H + 点云(W*H)
    std::vector<结构体_存在观测> 存在观测列表;  // 每个存在的中心/尺寸/方向/质量等
    std::vector<点簇增强结果> 点簇增强结果列表;
    std::vector<结构体_轮廓观测> 轮廓观测列表;  // 每个存在对应的 ROI(x,y,w,h + 掩膜)
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


    int 最大轮廓尺度 = 0;
    int 最大轮廓边长 = 0;
    int 最大轮廓尺度N = 0;
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



    // 平均颜色的采样步长：1=全采样；2/3=加速
    int 颜色采样步长 = 2;

    // 观测中心必须在相机前方：中心.z < 最小中心Z => 无效
    double  最小中心Z = 50.0;  // 毫米 (mm)

    // 尺寸下限（防止 0 或极小噪声簇）：任一轴 < 最小尺寸 => 无效
    double  最小尺寸 = 10.0;   // 毫米 (mm)

    double  最小点云像素覆盖率 = 0.2;


    /////////////////X////////////////////////////////

    bool 生成边缘图 = true;
    bool 生成法线图 = true;
    bool 生成置信度图 = true;
    bool 生成多尺度轮廓 = true;  // 消耗较大，慎开


};




/////////////////////////////////////////////////////////////////////////////////
//相机模块参数         截至                                            //////////
/////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////
//          窗口_配置及测试类                                          //////////
/////////////////////////////////////////////////////////////////////////////////
export constexpr UINT WM_APP_显示参数 = WM_APP + 301;

export enum class 枚举_显示参数命令 : WPARAM {
    显示坐标轴 = 1,
    显示地面网格,
    显示实时点云,
    显示存在框,
    显示存在主轴,
    显示存在点云簇,
    显示体素网格,
    网格仅表面,
    使用相机坐标系映射,
    点云使用颜色,
    坐标模式_绝对   // true=绝对(1), false=相对(0)
};





////////////////////////////////////////////////////////////////////////////////////////////////
   // 虚拟存在特征读写原语（Step1）
   // 说明：
   //  - 只提供 4 类原语：计数、集合、字符串、派生评估包
   //  - 计数只写入“计数类特征值”（通常为 I64），率由派生生成，不需要字段/全局map
   ////////////////////////////////////////////////////////////////////////////////////////////////

export enum class 枚举_评估阶段 : std::uint8_t {
    未知 = 0,
    证据不足 = 1,
    可用 = 2,
    需学习 = 3,
    禁用 = 4,
};
export struct 结构_评估包 {
    std::int64_t 率_Q10000 = 0;
    枚举_评估阶段 阶段 = 枚举_评估阶段::未知;

    // 可多值，便于叠加更多判断逻辑
    std::vector<std::string> 坏因;
    std::vector<std::string> 缺口;

    // 便于调试与上层策略
    std::int64_t 运行次数 = 0;
    std::int64_t 成功次数 = 0;
    std::int64_t 失败次数 = 0;
};
