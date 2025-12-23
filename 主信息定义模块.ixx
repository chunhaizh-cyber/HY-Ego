
// 主信息定义模块.ixx（更新版）
// 目标：
// 1) 按“方案C（混合子链+索引）”补齐 场景/存在 的索引字段
// 2) 保留“文本特征值”，并新增“引用特征值”以替代“用字符串主键当引用”的旧写法
// 3) 补齐/收敛 枚举_比较字段；未实现字段允许比较函数返回 -1（不支持）
// 4) 修复原文件中多处 switch/throw 花括号错误导致的不可达/缺 return 问题

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
import <utility>;
import <cmath>;
import <array>;
import <type_traits>;

import 模板模块;
import 基础数据类型模块;

//======================================================================
// 1) 前置声明 & 节点别名
//======================================================================

export class 语素基类;
export struct 特征值主信息类;
export class 基础信息基类;
export class 高级信息基类;
export class 自然语言成分基类;

export class 词主信息类;
export class 词性主信息类;
export class 短语主信息类;
export class 短语子节点主信息类;

export class 指代节点主信息类;
export class 特征节点主信息类;
export class 存在节点主信息类;
export class 场景节点主信息类;
export class 状态节点主信息类;
export class 动态节点主信息类;
export class 二次特征主信息类;
export class 因果主信息类;

export class 基本句子主信息类;
export class 复杂句子主信息类;
export class 段落主信息类;
export class 文章主信息类;

// 链表节点别名（你工程里统一用 链表模板<T*>::节点类）


export using 基础信息节点类 = 链表模板<基础信息基类*>::节点类;
export using 特征值节点类 = 链表模板<特征值主信息类*>::节点类;
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

//======================================================================
// 2) 语素族
//======================================================================

export class 语素基类 {
public:
    virtual ~语素基类() = default;
    virtual std::int64_t 比较(语素基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const = 0;
    //  void 收集存档字段(结构体_存在存档字段& out) const;
    //  void 修复存档字段(const 结构体_存在存档字段& in, const 结构体_引用解析上下文& ctx);
};

//—— 词
export class 词主信息类 : public 语素基类 {
public:
    std::string 词;

    explicit 词主信息类(const std::string& w = "") : 词(w) {}

    std::int64_t 比较(语素基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const override {
        auto* 右 = dynamic_cast<词主信息类*>(对象);
        if (!右) throw std::invalid_argument("词主信息类::比较 - 对象类型错误");

        if (字段 != 枚举_比较字段::语素_词_词) return -1;

        switch (条件) {
        case 枚举_比较条件::相等:   return this->词 == 右->词 ? 1 : 0;
        case 枚举_比较条件::不等于: return this->词 != 右->词 ? 1 : 0;
        default: return -1;
        }
    }

private:
    std::unordered_set<枚举_词性> 已存在词性集合;

public:
    bool 已存在词性(枚举_词性 词性) const {
        return 已存在词性集合.find(词性) != 已存在词性集合.end();
    }
    void 添加词性到集合(枚举_词性 词性) { 已存在词性集合.insert(词性); }
    void 删除词性在集合(枚举_词性 词性) { 已存在词性集合.erase(词性); }
};

export struct 结构体_词场景示例 {
    场景节点类* 场景指针 = nullptr;
    基础信息节点类* 基础信息指针 = nullptr;
};

//—— 词性
export class 词性主信息类 : public 语素基类 {
public:
    枚举_词性 词性 = 枚举_词性::未定义;
    基础信息节点类* 对应基础信息指针 = nullptr;
    std::vector<结构体_词场景示例> 示例列表;

    词性主信息类() = default;
    explicit 词性主信息类(枚举_词性 词性_) : 词性(词性_) {}

    void 添加场景示例(场景节点类* 场景, 基础信息节点类* 基础信息) {
        if (!场景 || !基础信息) return;
        for (const auto& 示例 : 示例列表) {
            if (示例.场景指针 == 场景 && 示例.基础信息指针 == 基础信息) return;
        }
        示例列表.push_back({ 场景, 基础信息 });
        if (!对应基础信息指针) 对应基础信息指针 = 基础信息;
    }

    基础信息节点类* 在场景中查找基础信息(场景节点类* 当前场景) const {
        if (!当前场景) return 对应基础信息指针;
        for (const auto& 示例 : 示例列表) {
            if (示例.场景指针 == 当前场景) return 示例.基础信息指针;
        }
        return 对应基础信息指针;
    }

    std::int64_t 比较(语素基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const override {
        auto* 右 = dynamic_cast<词性主信息类*>(对象);
        if (!右) throw std::invalid_argument("词性主信息类::比较 - 对象类型错误");

        switch (字段) {
        case 枚举_比较字段::语素_词性_词性:
            switch (条件) {
            case 枚举_比较条件::相等:   return this->词性 == 右->词性 ? 1 : 0;
            case 枚举_比较条件::不等于: return this->词性 != 右->词性 ? 1 : 0;
            default: return -1;
            }
        case 枚举_比较字段::语素_词性_对应基础信息指针:
            switch (条件) {
            case 枚举_比较条件::相等:   return this->对应基础信息指针 == 右->对应基础信息指针 ? 1 : 0;
            case 枚举_比较条件::不等于: return this->对应基础信息指针 != 右->对应基础信息指针 ? 1 : 0;
            default: return -1;
            }
        default:
            return -1;
        }
    }
};

//—— 短语（简化：只保留你当前比较所需字段）
export class 短语主信息类 : public 语素基类 {
public:
    词性节点类* 参照词 = nullptr;
    词性节点类* 比较对象词 = nullptr;

    短语主信息类() = default;
    短语主信息类(词性节点类* a, 词性节点类* b) : 参照词(a), 比较对象词(b) {}

    std::int64_t 比较(语素基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const override {
        auto* 右 = dynamic_cast<短语主信息类*>(对象);
        if (!右) throw std::invalid_argument("短语主信息类::比较 - 对象类型错误");

        switch (字段) {
        case 枚举_比较字段::语素_短语字节点_参照词:
            return (条件 == 枚举_比较条件::相等) ? (参照词 == 右->参照词) :
                (条件 == 枚举_比较条件::不等于) ? (参照词 != 右->参照词) : -1;
        case 枚举_比较字段::语素_短语字节点_比较对象词:
            return (条件 == 枚举_比较条件::相等) ? (比较对象词 == 右->比较对象词) :
                (条件 == 枚举_比较条件::不等于) ? (比较对象词 != 右->比较对象词) : -1;
        default:
            return -1;
        }
    }
};

export class 短语子节点主信息类 : public 语素基类 {
public:
    词性节点类* 比较词 = nullptr;
    词性节点类* 结果词 = nullptr;

    短语子节点主信息类() = default;
    短语子节点主信息类(词性节点类* a, 词性节点类* b) : 比较词(a), 结果词(b) {}

    std::int64_t 比较(语素基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const override {
        auto* 右 = dynamic_cast<短语子节点主信息类*>(对象);
        if (!右) throw std::invalid_argument("短语子节点主信息类::比较 - 对象类型错误");

        switch (字段) {
        case 枚举_比较字段::语素_短语子节点_比较词:
            return (条件 == 枚举_比较条件::相等) ? (比较词 == 右->比较词) :
                (条件 == 枚举_比较条件::不等于) ? (比较词 != 右->比较词) : -1;
        case 枚举_比较字段::语素_短语子节点_结果词:
            return (条件 == 枚举_比较条件::相等) ? (结果词 == 右->结果词) :
                (条件 == 枚举_比较条件::不等于) ? (结果词 != 右->结果词) : -1;
        default:
            return -1;
        }
    }
};


//======================================================================
// 3) 特征值族
//======================================================================
namespace  主信息定义_内部 {
    inline std::int64_t 比较_整数(std::int64_t l, std::int64_t r, 枚举_比较条件 c) {
        switch (c) {
        case 枚举_比较条件::相等:   return l == r ? 1 : 0;
        case 枚举_比较条件::不等于: return l != r ? 1 : 0;
        case 枚举_比较条件::大于:   return l > r ? 1 : 0;
        case 枚举_比较条件::小于:   return l < r ? 1 : 0;
        case 枚举_比较条件::大于等于: return l >= r ? 1 : 0;
        case 枚举_比较条件::小于等于: return l <= r ? 1 : 0;
        default: return -1;
        }
    }

    template<class T>
    inline std::int64_t 比较_指针(T* l, T* r, 枚举_比较条件 c) {
        switch (c) {
        case 枚举_比较条件::相等:   return l == r ? 1 : 0;
        case 枚举_比较条件::不等于: return l != r ? 1 : 0;
        default: return -1;
        }
    }

    inline bool 向量完全相等(const std::vector<std::int64_t>& a, const std::vector<std::int64_t>& b) {
        if (a.size() != b.size()) return false;
        for (std::size_t i = 0; i < a.size(); ++i) if (a[i] != b[i]) return false;
        return true;
    }
}

// =========================
   // 1) 类型 / 比较模式
   // =========================
export enum class 枚举_特征值类型 : std::uint16_t {
    未定义 = 0,

    // 标量
    I64,
    U64,
    F64,
    U8,

    // OpenCV 对齐颜色
    颜色_BGR8,

    // Bitset / 轮廓 / 占据
    Bits64,     // 8x8
    Bits512,    // 512bit

    // 区间
    区间_I64,
    区间_F64,

    // 向量/原始字节
    VecI64,
    VecF32,
    Bytes,
    //字符串特征值
    StringUTF8,

    //指针特征值
    Ptr,

    // 3D 体素占据金字塔（顶层 64bit，层间 *8）
    体素占据金字塔

};

export enum class 枚举_比较模式 : std::uint8_t {
    // 适合“严格弱序”用于树排序
    有序标量 = 0,
    字典序向量 = 1,

    // 适合“相似度/距离”用于识别打分
    海明距离 = 10,
    L1距离 = 11,
    L2距离 = 12,
    余弦相似度 = 13,

    // 颜色与区间
    颜色距离_BGR = 20,
    区间关系 = 30
};

// =========================
// 2) 载体类型（OpenCV 对齐 BGR8）
// =========================
export struct 颜色_BGR8 {
    std::uint8_t b{}, g{}, r{};
};

export struct 区间_i64 { std::int64_t lo{}, hi{}; };
export struct 区间_f64 { double       lo{}, hi{}; };

export struct Bits64 {
    std::uint64_t w = 0;

    constexpr Bits64() = default;
    constexpr Bits64(const Bits64&) = default;
    constexpr Bits64& operator=(const Bits64&) = default;

    constexpr explicit Bits64(std::uint64_t v) : w(v) {}

    friend constexpr bool operator==(const Bits64&, const Bits64&) = default;
    friend constexpr std::strong_ordering operator<=>(const Bits64& a, const Bits64& b) {
        return a.w <=> b.w;
    }
};

export using Bits512 = std::array<std::uint64_t, 8>;

export struct 体素占据金字塔 {
    // 顶层固定 4x4x4 = 64 bit（1 个 uint64）
    // 每层细分 2x2x2，数据量 *8：L1=512bit(8块), L2=4096bit(64块)...
    std::uint8_t 最大层 = 0;            // 0=仅顶层

    // 本金字塔覆盖的局部立方体范围（毫米，便于跨帧对齐/重建）
    std::int64_t 立方体边长_mm = 0;
    std::int64_t min_x_mm = 0;
    std::int64_t min_y_mm = 0;
    std::int64_t min_z_mm = 0;

    // 拼接块：L0(1) + L1(8) + L2(64) + ...
    std::vector<std::uint64_t> 块;
};


export using 特征值载体 = std::variant<
    std::monostate,
    std::int64_t,
    std::uint64_t,
    double,
    std::uint8_t,
    颜色_BGR8,
    Bits64,
    Bits512,
    体素占据金字塔,
    区间_i64,
    区间_f64,
    std::vector<std::int64_t>,
    std::vector<float>,
    std::vector<std::uint8_t>,
    std::string,
    void*
>;

// =========================
   // 3) 工具：popcount（可换成 std::popcount if <bit> 可用）
   // =========================
inline std::uint32_t popcount_u64(std::uint64_t x) {
#if defined(_MSC_VER)
    return (std::uint32_t)__popcnt64(x);
#elif defined(__GNUG__) || defined(__clang__)
    return (std::uint32_t)__builtin_popcountll(x);
#else
    // fallback
    std::uint32_t c = 0;
    while (x) { x &= (x - 1); ++c; }
    return c;
#endif
}

// =========================
// 4) 颜色：距离 / 桶 key / 排序
// =========================
export inline std::uint16_t 颜色距离_L1_BGR(const 颜色_BGR8& a, const 颜色_BGR8& b) {
    auto absu8 = [](int x) -> std::uint16_t { return (std::uint16_t)(x < 0 ? -x : x); };
    return absu8((int)a.b - (int)b.b)
        + absu8((int)a.g - (int)b.g)
        + absu8((int)a.r - (int)b.r); // 0..765
}

export inline std::uint16_t 颜色桶key_12bit(const 颜色_BGR8& c) {
    // 每通道 16 桶：b>>4,g>>4,r>>4
    return (std::uint16_t)(((c.b >> 4) << 8) | ((c.g >> 4) << 4) | (c.r >> 4));
}

export inline std::strong_ordering 颜色_字典序比较(const 颜色_BGR8& a, const 颜色_BGR8& b) {
    if (a.b != b.b) return a.b <=> b.b;
    if (a.g != b.g) return a.g <=> b.g;
    return a.r <=> b.r;
}

// =========================
// 5) Bitset：海明距离
// =========================
export inline double Bits64_海明距离(const Bits64& a, const Bits64& b) {
    const std::uint32_t diff = popcount_u64(a.w ^ b.w);
    return (double)diff / 64.0;
}

export inline double Bits512_海明距离(const Bits512& a, const Bits512& b) {
    std::uint32_t diff = 0;
    for (int i = 0; i < 8; ++i) diff += popcount_u64(a[i] ^ b[i]);
    return (double)diff / 512.0;
}

// 可选：部分观测 mask（未知不惩罚）
export inline double Bits64_海明距离_掩码(const Bits64& a, const Bits64& b, const Bits64& mask_visible) {
    const std::uint64_t x = (a.w ^ b.w) & mask_visible.w;
    const std::uint32_t diff = popcount_u64(x);
    const std::uint32_t vis = popcount_u64(mask_visible.w);
    if (vis == 0) return 1.0; // 没有可见信息：当作最不确定
    return (double)diff / (double)vis;
}

export inline double Bits512_海明距离_掩码(const Bits512& a, const Bits512& b, const Bits512& mask_visible) {
    std::uint32_t diff = 0, vis = 0;
    for (int i = 0; i < 8; ++i) {
        diff += popcount_u64((a[i] ^ b[i]) & mask_visible[i]);
        vis += popcount_u64(mask_visible[i]);
    }
    if (vis == 0) return 1.0;
    return (double)diff / (double)vis;
}


// =========================
// 5.5) 体素占据金字塔：块数/距离/排序
// =========================
export inline std::uint64_t 体素金字塔_层块数(std::uint8_t level) {
    // L0:1, L1:8, L2:64 ...
    return 1ull << (3ull * level);
}

export inline std::uint64_t 体素金字塔_总块数(std::uint8_t max_level) {
    std::uint64_t n = 0;
    for (std::uint8_t i = 0; i <= max_level; ++i) n += 体素金字塔_层块数(i);
    return n;
}

export inline std::uint64_t 体素金字塔_层起始偏移(std::uint8_t level) {
    // sum_{i=0}^{level-1} 8^i
    std::uint64_t off = 0;
    for (std::uint8_t i = 0; i < level; ++i) off += 体素金字塔_层块数(i);
    return off;
}

export inline std::strong_ordering 体素金字塔_字典序比较(const 体素占据金字塔& a, const 体素占据金字塔& b) {
    if (a.最大层 != b.最大层) return a.最大层 <=> b.最大层;
    if (a.立方体边长_mm != b.立方体边长_mm) return a.立方体边长_mm <=> b.立方体边长_mm;
    if (a.min_x_mm != b.min_x_mm) return a.min_x_mm <=> b.min_x_mm;
    if (a.min_y_mm != b.min_y_mm) return a.min_y_mm <=> b.min_y_mm;
    if (a.min_z_mm != b.min_z_mm) return a.min_z_mm <=> b.min_z_mm;

    const size_t n = std::min(a.块.size(), b.块.size());
    for (size_t i = 0; i < n; ++i) {
        if (a.块[i] != b.块[i]) return a.块[i] <=> b.块[i];
    }
    return a.块.size() <=> b.块.size();
}

export inline double 体素金字塔_海明距离(const 体素占据金字塔& a, const 体素占据金字塔& b) {
    // 加权：顶层权重大，细层权重按 1/8^L 衰减
    const std::uint8_t L = (a.最大层 < b.最大层) ? a.最大层 : b.最大层;
    if (a.块.empty() || b.块.empty()) return 1.0;

    double acc = 0.0;
    double wsum = 0.0;
    for (std::uint8_t lv = 0; lv <= L; ++lv) {
        const std::uint64_t bc = 体素金字塔_层块数(lv);
        const std::uint64_t off = 体素金字塔_层起始偏移(lv);
        if (off + bc > a.块.size() or off + bc > b.块.size()) break;

        std::uint64_t diff = 0;
        for (std::uint64_t i = 0; i < bc; ++i) diff += popcount_u64(a.块[off + i] ^ b.块[off + i]);

        const double dn = (double)diff / (double)(bc * 64ull);
        const double w = 1.0 / (double)(1ull << (3ull * lv)); // 1/8^lv
        acc += w * dn;
        wsum += w;
    }
    if (wsum <= 0.0) return 1.0;
    return acc / wsum;
}
// =========================
// 6) 区间：关系 / 距离
// =========================
export inline bool 区间_重叠(const 区间_i64& a, const 区间_i64& b) {
    return !(a.hi < b.lo || b.hi < a.lo);
}
export inline double 区间_距离(const 区间_i64& a, const 区间_i64& b) {
    if (区间_重叠(a, b)) return 0.0;
    if (a.hi < b.lo) return (double)(b.lo - a.hi);
    return (double)(a.lo - b.hi);
}

export inline bool 区间_重叠(const 区间_f64& a, const 区间_f64& b) {
    return !(a.hi < b.lo || b.hi < a.lo);
}
export inline double 区间_距离(const 区间_f64& a, const 区间_f64& b) {
    if (区间_重叠(a, b)) return 0.0;
    if (a.hi < b.lo) return (b.lo - a.hi);
    return (a.lo - b.hi);
}
// =========================
  // 7) 主信息（值节点）
  // =========================
export struct 特征值主信息类 {
    枚举_特征值类型 类型 = 枚举_特征值类型::未定义;
    枚举_比较模式   比较模式 = 枚举_比较模式::有序标量;

    特征值载体 值;

    uint64_t 粗哈希 = 0;     // 可选：加速去重/入桶
    uint32_t 命中次数 = 0;   // 你体系里非常有用
    float    可信度 = 1.0f;  // 可选
};

// =========================
   // 8) 粗哈希 / 粗key 计算
   // =========================
inline std::uint64_t fnv1a64(const void* data, size_t n) {
    const std::uint8_t* p = (const std::uint8_t*)data;
    std::uint64_t h = 1469598103934665603ull;
    for (size_t i = 0; i < n; ++i) {
        h ^= (std::uint64_t)p[i];
        h *= 1099511628211ull;
    }
    return h;
}
export inline std::uint64_t 计算粗哈希(const 特征值主信息类& v) {
    // 约定：粗哈希用于“去重/入桶”，不要求强抗碰撞，只要快。
    // 对颜色：用 12bit 桶 key
    // 对 bitset：用低 64 或若干片段混合
    // 对标量：直接 hash bytes
    // 对向量/bytes：hash 前若干字节 + size
    std::uint64_t h = 0;

    std::visit([&](auto&& x) {
        using T = std::decay_t<decltype(x)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            h = 0;
        }
        else if constexpr (std::is_same_v<T, 颜色_BGR8>) {
            std::uint16_t key = 颜色桶key_12bit(x);
            h = ((std::uint64_t)key << 16) ^ 0xC0B1A0ull;
        }
        else if constexpr (std::is_same_v<T, Bits64>) {
            // 取低/高混合
            std::uint64_t y = x.w ^ (x.w >> 33);
            h = fnv1a64(&y, sizeof(y));
        }
        else if constexpr (std::is_same_v<T, Bits512>) {
            // 混合前两块 + size
            std::uint64_t y[3] = { x[0], x[1], 512ull };
            h = fnv1a64(y, sizeof(y));
        }
        else if constexpr (std::is_same_v<T, 体素占据金字塔>) {
            std::uint64_t sz = (std::uint64_t)x.块.size();
            // 取前 3 块 + max_level
            std::uint64_t y[5] = { sz, (std::uint64_t)x.最大层, (sz > 0 ? x.块[0] : 0ull), (sz > 1 ? x.块[1] : 0ull), (sz > 2 ? x.块[2] : 0ull) };
            h = fnv1a64(y, sizeof(y));
            // 混入边界（避免不同尺度碰撞）
            std::uint64_t bnd[4] = { (std::uint64_t)x.立方体边长_mm, (std::uint64_t)x.min_x_mm, (std::uint64_t)x.min_y_mm, (std::uint64_t)x.min_z_mm };
            h ^= fnv1a64(bnd, sizeof(bnd));
        }
        else if constexpr (std::is_same_v<T, std::vector<std::int64_t>>) {
            std::uint64_t sz = (std::uint64_t)x.size();
            size_t bytes = x.size() * sizeof(std::int64_t);
            size_t take = bytes > 64 ? 64 : bytes; // 只取前 64 字节
            h = fnv1a64(&sz, sizeof(sz));
            if (take) h ^= fnv1a64(x.data(), take);
        }
        else if constexpr (std::is_same_v<T, std::vector<float>>) {
            std::uint64_t sz = (std::uint64_t)x.size();
            size_t bytes = x.size() * sizeof(float);
            size_t take = bytes > 64 ? 64 : bytes;
            h = fnv1a64(&sz, sizeof(sz));
            if (take) h ^= fnv1a64(x.data(), take);
        }
        else if constexpr (std::is_same_v<T, std::vector<std::uint8_t>>) {
            std::uint64_t sz = (std::uint64_t)x.size();
            size_t take = x.size() > 64 ? 64 : x.size();
            h = fnv1a64(&sz, sizeof(sz));
            if (take) h ^= fnv1a64(x.data(), take);
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            std::uint64_t sz = (std::uint64_t)x.size();
            size_t take = x.size() > 64 ? 64 : x.size();
            h = fnv1a64(&sz, sizeof(sz));
            if (take) h ^= fnv1a64(x.data(), take);
        }
        else {
            // 标量 / 区间 / double / u8 / u64 / i64
            h = fnv1a64(&x, sizeof(T));
        }
        }, v.值);

    // 混入类型与比较模式
    h ^= ((std::uint64_t)v.类型 << 48);
    h ^= ((std::uint64_t)v.比较模式 << 40);
    return h;
}

// =========================
// 9) 严格比较（用于“树排序”）
// =========================
// 约定：NaN 排在最后；NaN 与 NaN 视为相等；+0 与 -0 视为相等
template<class F>
    requires std::is_floating_point_v<F>
inline std::strong_ordering 强序比较_浮点(F a, F b) {
    const bool na = std::isnan(a);
    const bool nb = std::isnan(b);
    if (na || nb) {
        if (na && nb) return std::strong_ordering::equal;
        return na ? std::strong_ordering::greater : std::strong_ordering::less;
    }
    if (a < b) return std::strong_ordering::less;
    if (a > b) return std::strong_ordering::greater;
    return std::strong_ordering::equal;
}

export inline std::strong_ordering 严格比较_用于排序(const 特征值主信息类& a, const 特征值主信息类& b) {
    if (a.类型 != b.类型) return a.类型 <=> b.类型;
    if (a.比较模式 != b.比较模式) return a.比较模式 <=> b.比较模式;

    return std::visit([&](auto&& x, auto&& y) -> std::strong_ordering {
        using X = std::decay_t<decltype(x)>;
        using Y = std::decay_t<decltype(y)>;

        if constexpr (!std::is_same_v<X, Y>) {
            return a.值.index() <=> b.值.index();
        }
        else {
            if constexpr (std::is_same_v<X, std::monostate>) {
                return std::strong_ordering::equal;
            }
            else if constexpr (std::is_same_v<X, 颜色_BGR8>) {
                return 颜色_字典序比较(x, y);
            }
            else if constexpr (std::is_same_v<X, Bits64>) {
                return x <=> y;
            }
            else if constexpr (std::is_same_v<X, Bits512>) {
                for (int i = 0; i < 8; ++i) {
                    if (x[i] != y[i]) return x[i] <=> y[i];
                }
                return std::strong_ordering::equal;
            }
            else if constexpr (std::is_same_v<X, 体素占据金字塔>) {
                return 体素金字塔_字典序比较(x, y);
            }
            else if constexpr (std::is_same_v<X, 区间_i64>) {
                if (x.lo != y.lo) return x.lo <=> y.lo;
                return x.hi <=> y.hi;
            }
            else if constexpr (std::is_same_v<X, 区间_f64>) {
                if (auto r = 强序比较_浮点(x.lo, y.lo); r != 0) return r;
                return 强序比较_浮点(x.hi, y.hi);
            }
            else if constexpr (std::is_same_v<X, std::vector<std::int64_t>>) {
                const size_t n = (x.size() < y.size()) ? x.size() : y.size();
                for (size_t i = 0; i < n; ++i) {
                    if (x[i] != y[i]) return x[i] <=> y[i];
                }
                return x.size() <=> y.size();
            }
            else if constexpr (std::is_same_v<X, std::vector<float>>) {
                const size_t n = (x.size() < y.size()) ? x.size() : y.size();
                for (size_t i = 0; i < n; ++i) {
                    if (auto r = 强序比较_浮点(x[i], y[i]); r != 0) return r;
                }
                return x.size() <=> y.size();
            }
            else if constexpr (std::is_same_v<X, std::vector<std::uint8_t>>) {
                const size_t n = (x.size() < y.size()) ? x.size() : y.size();
                for (size_t i = 0; i < n; ++i) {
                    if (x[i] != y[i]) return x[i] <=> y[i];
                }
                return x.size() <=> y.size();
            }
            else if constexpr (std::is_same_v<X, std::string>) {
                const size_t n = (x.size() < y.size()) ? x.size() : y.size();
                for (size_t i = 0; i < n; ++i) {
                    if (x[i] != y[i]) return (unsigned char)x[i] <=> (unsigned char)y[i];
                }
                return x.size() <=> y.size();
            }
            else {
                // 标量：i64/u64/u8/double/float...
                if constexpr (std::is_floating_point_v<X>) {
                    return 强序比较_浮点(x, y);
                }
                else {
                    return x <=> y;
                }
            }
        }
        }, a.值, b.值);
}

// =========================
// 10) 相似度/距离（用于识别打分）
// 返回值：越小越相似；默认范围尽量归一化到 [0,1]（能归一就归一）
// =========================
export inline double 距离或差异度(const 特征值主信息类& a, const 特征值主信息类& b) {
    if (a.类型 != b.类型) return 1.0; // 类型不同直接认为不相似
    // 比较模式不同也视为不可比（你也可以改成按类型默认）
    if (a.比较模式 != b.比较模式) return 1.0;

    return std::visit([&](auto&& x, auto&& y) -> double {
        using X = std::decay_t<decltype(x)>;
        using Y = std::decay_t<decltype(y)>;
        if constexpr (!std::is_same_v<X, Y>) {
            return 1.0;
        }
        else {
            switch (a.比较模式) {
            case 枚举_比较模式::海明距离:
                if constexpr (std::is_same_v<X, Bits64>)  return Bits64_海明距离(x, y);
                if constexpr (std::is_same_v<X, Bits512>) return Bits512_海明距离(x, y);
                if constexpr (std::is_same_v<X, 体素占据金字塔>) return 体素金字塔_海明距离(x, y);
                if constexpr (std::is_same_v<X, std::vector<std::int64_t>>) {
                    const size_t n = std::min(x.size(), y.size());
                    if (n == 0) return 1.0;
                    std::uint64_t diff = 0;
                    for (size_t i = 0; i < n; ++i) diff += popcount_u64((std::uint64_t)x[i] ^ (std::uint64_t)y[i]);
                    const double dn = (double)diff / (double)(n * 64ull);
                    // 若长度不同：缺失部分记为最大差异
                    if (x.size() != y.size()) {
                        const size_t m = std::max(x.size(), y.size());
                        return std::min(1.0, dn * (double)m / (double)n);
                    }
                    return dn;
                }
                return 1.0;

            case 枚举_比较模式::颜色距离_BGR:
                if constexpr (std::is_same_v<X, 颜色_BGR8>) {
                    const double d = (double)颜色距离_L1_BGR(x, y);
                    return d / 765.0;
                }
                return 1.0;

            case 枚举_比较模式::有序标量:
                // 有序标量不是“距离”，这里给 0/1
                if constexpr (std::is_arithmetic_v<X>) return (x == y) ? 0.0 : 1.0;
                return 1.0;

            case 枚举_比较模式::区间关系:
                if constexpr (std::is_same_v<X, 区间_i64>) {
                    // 归一化：用区间长度尺度（避免爆）
                    const double base = (double)std::max<std::int64_t>(1, (x.hi - x.lo + 1));
                    return 区间_距离(x, y) / base;
                }
                if constexpr (std::is_same_v<X, 区间_f64>) {
                    const double base = std::max(1e-9, (x.hi - x.lo));
                    return 区间_距离(x, y) / base;
                }
                return 1.0;

                // 这里可以继续扩展：L1/L2/余弦 for VecF32 等
            default:
                return 1.0;
            }
        }
        }, a.值, b.值);
}

// =========================
// 11) 初始化/规范化：让 类型 与 variant 内实际载体一致
// =========================
export inline bool 校验载体与类型一致(const 特征值主信息类& v) {
    // 简单校验：按 v.类型 允许的 variant 类型检查
    bool ok = false;
    std::visit([&](auto&& x) {
        using T = std::decay_t<decltype(x)>;
        switch (v.类型) {
        case 枚举_特征值类型::I64:      ok = std::is_same_v<T, std::int64_t>; break;
        case 枚举_特征值类型::U64:      ok = std::is_same_v<T, std::uint64_t>; break;
        case 枚举_特征值类型::F64:      ok = std::is_same_v<T, double>; break;
        case 枚举_特征值类型::U8:       ok = std::is_same_v<T, std::uint8_t>; break;
        case 枚举_特征值类型::颜色_BGR8: ok = std::is_same_v<T, 颜色_BGR8>; break;
        case 枚举_特征值类型::Bits64:   ok = std::is_same_v<T, Bits64>; break;
        case 枚举_特征值类型::Bits512:  ok = std::is_same_v<T, Bits512>; break;
        case 枚举_特征值类型::区间_I64: ok = std::is_same_v<T, 区间_i64>; break;
        case 枚举_特征值类型::区间_F64: ok = std::is_same_v<T, 区间_f64>; break;
        case 枚举_特征值类型::VecI64:   ok = std::is_same_v<T, std::vector<std::int64_t>>; break;
        case 枚举_特征值类型::VecF32:   ok = std::is_same_v<T, std::vector<float>>; break;
        case 枚举_特征值类型::Bytes:    ok = std::is_same_v<T, std::vector<std::uint8_t>>; break;
        case 枚举_特征值类型::StringUTF8: ok = std::is_same_v<T, std::string>; break;
        case 枚举_特征值类型::Ptr:      ok = std::is_same_v<T, void*>; break;
        case 枚举_特征值类型::体素占据金字塔: ok = std::is_same_v<T, 体素占据金字塔>; break;
        default: ok = std::is_same_v<T, std::monostate>; break;
        }
        }, v.值);
    return ok;
}

export inline void 重新计算缓存(特征值主信息类& v) {
    v.粗哈希 = 计算粗哈希(v);
}

// =========================
// 12) 工厂：OpenCV 对齐颜色值
// =========================
export inline 特征值主信息类 创建颜色_RGB8(std::uint8_t b, std::uint8_t g, std::uint8_t r,
    枚举_比较模式 mode = 枚举_比较模式::颜色距离_BGR) {
    特征值主信息类 v;
    v.类型 = 枚举_特征值类型::颜色_BGR8;
    v.比较模式 = mode;
    v.值 = 颜色_BGR8{ b, g, r };
    重新计算缓存(v);
    return v;
}

export inline 特征值主信息类 创建Bits64(Bits64 bits, 枚举_比较模式 mode = 枚举_比较模式::海明距离) {
    特征值主信息类 v;
    v.类型 = 枚举_特征值类型::Bits64;
    v.比较模式 = mode;
    v.值 = bits;
    重新计算缓存(v);
    return v;
}

export inline 特征值主信息类 创建Bits512(const Bits512& bits, 枚举_比较模式 mode = 枚举_比较模式::海明距离) {
    特征值主信息类 v;
    v.类型 = 枚举_特征值类型::Bits512;
    v.比较模式 = mode;
    v.值 = bits;
    重新计算缓存(v);
    return v;
}

export inline 特征值主信息类 创建体素占据金字塔(体素占据金字塔 v0, 枚举_比较模式 mode = 枚举_比较模式::海明距离) {
    特征值主信息类 v;
    v.类型 = 枚举_特征值类型::体素占据金字塔;
    v.比较模式 = mode;
    v.值 = std::move(v0);
    重新计算缓存(v);
    return v;
}

// =========================
// 13) 存档/重载（最小二进制接口示例）
// 你可以替换成你自己的 序列化流/档案类
// =========================
export struct 二进制写入器 {
    virtual ~二进制写入器() = default;
    virtual void 写(const void* p, size_t n) = 0;
};
export struct 二进制读取器 {
    virtual ~二进制读取器() = default;
    virtual void 读(void* p, size_t n) = 0;
};

export inline void 写入(二进制写入器& w, const 特征值主信息类& v) {
    // 版本号（便于未来升级）
    const std::uint16_t ver = 1;
    w.写(&ver, sizeof(ver));

    w.写(&v.类型, sizeof(v.类型));
    w.写(&v.比较模式, sizeof(v.比较模式));
    w.写(&v.命中次数, sizeof(v.命中次数));
    w.写(&v.可信度, sizeof(v.可信度));

    // payload
    std::visit([&](auto&& x) {
        using T = std::decay_t<decltype(x)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            // nothing
        }
        else if constexpr (
            std::is_same_v<T, std::int64_t> ||
            std::is_same_v<T, std::uint64_t> ||
            std::is_same_v<T, double> ||
            std::is_same_v<T, std::uint8_t> ||
            std::is_same_v<T, 颜色_BGR8> ||
            std::is_same_v<T, Bits64> ||
            std::is_same_v<T, Bits512> ||
            std::is_same_v<T, 区间_i64> ||
            std::is_same_v<T, 区间_f64>
            ) {
            w.写(&x, sizeof(T));
        }
        else if constexpr (std::is_same_v<T, std::vector<std::int64_t>>) {
            std::uint64_t n = (std::uint64_t)x.size();
            w.写(&n, sizeof(n));
            if (n) w.写(x.data(), (size_t)n * sizeof(std::int64_t));
        }
        else if constexpr (std::is_same_v<T, std::vector<float>>) {
            std::uint64_t n = (std::uint64_t)x.size();
            w.写(&n, sizeof(n));
            if (n) w.写(x.data(), (size_t)n * sizeof(float));
        }
        else if constexpr (std::is_same_v<T, std::vector<std::uint8_t>>) {
            std::uint64_t n = (std::uint64_t)x.size();
            w.写(&n, sizeof(n));
            if (n) w.写(x.data(), (size_t)n);
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            std::uint64_t n = (std::uint64_t)x.size();
            w.写(&n, sizeof(n));
            if (n) w.写(x.data(), (size_t)n);
        }
        else if constexpr (std::is_same_v<T, 体素占据金字塔>) {
            w.写(&x.最大层, sizeof(x.最大层));
            w.写(&x.立方体边长_mm, sizeof(x.立方体边长_mm));
            w.写(&x.min_x_mm, sizeof(x.min_x_mm));
            w.写(&x.min_y_mm, sizeof(x.min_y_mm));
            w.写(&x.min_z_mm, sizeof(x.min_z_mm));
            std::uint64_t n = (std::uint64_t)x.块.size();
            w.写(&n, sizeof(n));
            if (n) w.写(x.块.data(), (size_t)n * sizeof(std::uint64_t));
        }
        else if constexpr (std::is_same_v<T, void*>) {
            // 指针不保证可跨进程/跨次启动复原，这里仅写入地址做调试用途
            std::uint64_t u = (std::uint64_t)(std::uintptr_t)x;
            w.写(&u, sizeof(u));
        }
        else {
            //   static_assert(!sizeof(T*), "未覆盖的 variant 类型");
        }
        }, v.值);
}

export inline 特征值主信息类 读取(二进制读取器& r) {
    特征值主信息类 v;
    std::uint16_t ver = 0;
    r.读(&ver, sizeof(ver));
    if (ver != 1) throw std::runtime_error("特征值序列化版本不匹配");

    r.读(&v.类型, sizeof(v.类型));
    r.读(&v.比较模式, sizeof(v.比较模式));
    r.读(&v.命中次数, sizeof(v.命中次数));
    r.读(&v.可信度, sizeof(v.可信度));

    // payload：按 v.类型 读取（稳定格式：不依赖 variant.index）
    switch (v.类型) {
    case 枚举_特征值类型::I64: {
        std::int64_t x; r.读(&x, sizeof(x)); v.值 = x; break;
    }
    case 枚举_特征值类型::U64: {
        std::uint64_t x; r.读(&x, sizeof(x)); v.值 = x; break;
    }
    case 枚举_特征值类型::F64: {
        double x; r.读(&x, sizeof(x)); v.值 = x; break;
    }
    case 枚举_特征值类型::U8: {
        std::uint8_t x; r.读(&x, sizeof(x)); v.值 = x; break;
    }
    case 枚举_特征值类型::颜色_BGR8: {
        颜色_BGR8 x; r.读(&x, sizeof(x)); v.值 = x; break;
    }
    case 枚举_特征值类型::Bits64: {
        Bits64 x; r.读(&x, sizeof(x)); v.值 = x; break;
    }
    case 枚举_特征值类型::Bits512: {
        Bits512 x; r.读(x.data(), sizeof(std::uint64_t) * 8); v.值 = x; break;
    }
    case 枚举_特征值类型::区间_I64: {
        区间_i64 x; r.读(&x, sizeof(x)); v.值 = x; break;
    }
    case 枚举_特征值类型::区间_F64: {
        区间_f64 x; r.读(&x, sizeof(x)); v.值 = x; break;
    }
    case 枚举_特征值类型::VecI64: {
        std::uint64_t n = 0; r.读(&n, sizeof(n));
        std::vector<std::int64_t> x; x.resize((size_t)n);
        if (n) r.读(x.data(), (size_t)n * sizeof(std::int64_t));
        v.值 = std::move(x);
        break;
    }
    case 枚举_特征值类型::VecF32: {
        std::uint64_t n = 0; r.读(&n, sizeof(n));
        std::vector<float> x; x.resize((size_t)n);
        if (n) r.读(x.data(), (size_t)n * sizeof(float));
        v.值 = std::move(x);
        break;
    }
    case 枚举_特征值类型::Bytes: {
        std::uint64_t n = 0; r.读(&n, sizeof(n));
        std::vector<std::uint8_t> x; x.resize((size_t)n);
        if (n) r.读(x.data(), (size_t)n);
        v.值 = std::move(x);
        break;
    }
    case 枚举_特征值类型::StringUTF8: {
        std::uint64_t n = 0; r.读(&n, sizeof(n));
        std::string s; s.resize((size_t)n);
        if (n) r.读(s.data(), (size_t)n);
        v.值 = std::move(s);
        break;
    }
    case 枚举_特征值类型::体素占据金字塔: {
        体素占据金字塔 t;
        r.读(&t.最大层, sizeof(t.最大层));
        r.读(&t.立方体边长_mm, sizeof(t.立方体边长_mm));
        r.读(&t.min_x_mm, sizeof(t.min_x_mm));
        r.读(&t.min_y_mm, sizeof(t.min_y_mm));
        r.读(&t.min_z_mm, sizeof(t.min_z_mm));
        std::uint64_t n = 0; r.读(&n, sizeof(n));
        t.块.resize((size_t)n);
        if (n) r.读(t.块.data(), (size_t)n * sizeof(std::uint64_t));
        v.值 = std::move(t);
        break;
    }
    case 枚举_特征值类型::Ptr: {
        std::uint64_t u = 0; r.读(&u, sizeof(u));
        v.值 = (void*)(std::uintptr_t)u;
        break;
    }
    default:
        // 未定义/未知：降级为空
        v.值 = std::monostate{};
        break;
    }

    重新计算缓存(v);
    return v;
}


//======================================================================
// 四. 基础信息族
//======================================================================



export class 基础信息基类 {
public:
    词性节点类* 名称 = nullptr;
    词性节点类* 类型 = nullptr;
    std::uint64_t 命中次数 = 0;
    virtual ~基础信息基类() = default;

    virtual std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const = 0;

protected:
    基础信息基类() = default;
    explicit 基础信息基类(词性节点类* 名) : 名称(名) {}
    基础信息基类(词性节点类* 名, 词性节点类* 型) : 名称(名), 类型(型) {}
    //   void 收集存档字段(结构体_存在存档字段& out) const;
    //   void 修复存档字段(const 结构体_存在存档字段& in, const 结构体_引用解析上下文& ctx);
};

//—— 指代
export class 指代节点主信息类 : public 基础信息基类 {
public:
    词性节点类* 代词 = nullptr;
    存在节点类* 指代对象 = nullptr;

    指代节点主信息类() = default;
    指代节点主信息类(词性节点类* 名, 词性节点类* 型, 词性节点类* 代, 存在节点类* 对象)
        : 基础信息基类(名, 型), 代词(代), 指代对象(对象) {
    }

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const override {
        auto* 右 = dynamic_cast<指代节点主信息类*>(对象);
        if (!右) throw std::invalid_argument("指代节点主信息类::比较 - 对象类型错误");

        switch (字段) {
        case 枚举_比较字段::基础信息基类_名称:
            return 主信息定义_内部::比较_指针(this->名称, 右->名称, 条件);
        case 枚举_比较字段::基础信息基类_类型:
            return 主信息定义_内部::比较_指针(this->类型, 右->类型, 条件);
        case 枚举_比较字段::基础信息_指代节点_代词:
            return 主信息定义_内部::比较_指针(this->代词, 右->代词, 条件);
        case 枚举_比较字段::基础信息_指代节点_指代对象:
            return 主信息定义_内部::比较_指针(this->指代对象, 右->指代对象, 条件);
        default:
            return -1;
        }
    }
};

//—— 特征
export class 特征节点主信息类 : public 基础信息基类 {
public:
    特征值节点类* 值 = nullptr;

    特征节点主信息类() = default;
    explicit 特征节点主信息类(词性节点类* 特征类型) : 基础信息基类(nullptr, 特征类型) {}
    特征节点主信息类(词性节点类* 特征类型, 特征值节点类* 特征值) : 基础信息基类(nullptr, 特征类型), 值(特征值) {}
    特征节点主信息类(词性节点类* 名, 词性节点类* 型, 特征值节点类* 特征值)
        : 基础信息基类(名, 型), 值(特征值) {
    }

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const override {
        auto* 右 = dynamic_cast<特征节点主信息类*>(对象);
        if (!右) throw std::invalid_argument("特征节点主信息类::比较 - 对象类型错误");

        switch (字段) {
        case 枚举_比较字段::基础信息_特征节点_名称:
        case 枚举_比较字段::基础信息基类_名称:
            return 主信息定义_内部::比较_指针(this->名称, 右->名称, 条件);
        case 枚举_比较字段::基础信息_特征节点_类型:
        case 枚举_比较字段::基础信息基类_类型:
            return 主信息定义_内部::比较_指针(this->类型, 右->类型, 条件);
        case 枚举_比较字段::基础信息_特征节点_值:
        case 枚举_比较字段::值:
            return 主信息定义_内部::比较_指针(this->值, 右->值, 条件);
        default:
            return -1;
        }
    }
};


//—— 三维模型特征：一个特征节点下挂多个“立体状态原型”（特征值节点指针）
//   用于“站/坐/卧/行”等稳定姿态原型的比较与选择。
export class 三维模型特征主信息类 : public 特征节点主信息类 {
public:
    std::vector<特征值节点类*> 状态原型;
    std::vector<std::uint32_t>   状态命中;
    std::uint32_t               最大原型数 = 16;

    三维模型特征主信息类() = default;
    explicit 三维模型特征主信息类(词性节点类* 特征类型) : 特征节点主信息类(特征类型) {}
    三维模型特征主信息类(词性节点类* 特征类型, 特征值节点类* 当前) : 特征节点主信息类(特征类型, 当前) {
        if (当前) { 状态原型.push_back(当前); 状态命中.push_back(1); }
    }
    三维模型特征主信息类(词性节点类* 名, 词性节点类* 型, 特征值节点类* 当前) : 特征节点主信息类(名, 型, 当前) {
        if (当前) { 状态原型.push_back(当前); 状态命中.push_back(1); }
    }
};

//—— 存在：作为“容器”，允许在子链上挂 子存在/特征（内部世界可选）
export class 存在节点主信息类 : public 基础信息基类 {
public:
    // 挂钩到“存在概念模板”（可选，用于抽象/识别）
    存在节点类* 概念模板 = nullptr;

    // 兼容旧结构：内部世界可空。你当前可完全不依赖它；
    // 仅当需要“内部坐标系/内部环境特征/复用场景算法”时再懒创建。
    场景节点类* 内部世界 = nullptr;

    时间戳 创建时间 = 0;
    时间戳 最后观测时间 = 0;
    std::uint32_t 命中次数 = 0;
    float 可信度 = 1.0f;
    // 稳定度/命中计数：用于“帧间关联”与“过期清理”


    // 方案C：索引（子链可混放，但用索引加速）
    std::vector<存在节点类*> 子存在索引;
    std::vector<特征节点类*> 特征索引;

    // 每种特征类型的“当前代表节点”（缓存：每类型最多一个）
    std::vector<特征节点类*> 特征当前状态;

    存在节点主信息类() = default;
    explicit 存在节点主信息类(词性节点类* 名) : 基础信息基类(名) {}
    存在节点主信息类(词性节点类* 名, 词性节点类* 型) : 基础信息基类(名, 型) {}
    存在节点主信息类(词性节点类* 名, 词性节点类* 型, 时间戳 t) : 基础信息基类(名, 型), 创建时间(t) {}

    std::string 取存在ID(const 存在节点类* 节点) const { return 节点 ? 节点->主键 : std::string{}; }

    // —— 轻量查询：按类型取“当前特征” —— 
    特征节点类* 查找特征当前(词性节点类* 特征类型) const {
        if (!特征类型) return nullptr;
        for (auto* f : 特征当前状态) {
            if (!f || !f->主信息) continue;
            if (f->主信息->类型 == 特征类型) return f;
        }
        return nullptr;
    }

    // —— 轻量查询：推导特征类型集合（去重）——
    std::vector<词性节点类*> 获取特征类型集合() const {
        std::vector<词性节点类*> out;
        out.reserve(特征索引.size());

        std::unordered_set<词性节点类*> seen;
        for (auto* f : 特征索引) {
            if (!f || !f->主信息) continue;
            auto* fmi = dynamic_cast<特征节点主信息类*>(f->主信息);
            if (!fmi || !fmi->类型) continue;
            if (seen.insert(fmi->类型).second) out.push_back(fmi->类型);
        }
        return out;
    }

    // —— 缓存维护：从 特征索引 重建 特征当前状态 —— 
    // 规则：每类型选一个代表。默认“先到先得”；若旧代表无值、新候选有值，则替换。
    void 重建特征当前状态() {
        特征当前状态.clear();
        特征当前状态.reserve(特征索引.size());

        std::unordered_set<词性节点类*> seen;
        for (auto* f : 特征索引) {
            if (!f || !f->主信息) continue;
            auto* fmi = dynamic_cast<特征节点主信息类*>(f->主信息);
            if (!fmi || !fmi->类型) continue;

            if (seen.insert(fmi->类型).second) {
                特征当前状态.push_back(f);
                continue;
            }

            // 同类型已存在：尝试“有值优先”
            for (auto& cur : 特征当前状态) {
                if (!cur || !cur->主信息) continue;
                auto* cmi = dynamic_cast<特征节点主信息类*>(cur->主信息);
                if (!cmi || cmi->类型 != fmi->类型) continue;
                if (!cmi->值 && fmi->值) cur = f;
                break;
            }
        }
    }

    void 记录命中()
    {
        ++命中次数;
        可信度 = std::min(1.0f, 可信度 + 0.005f);
    }

    // 可选：重置统计
    void 重置统计()
    {
        命中次数 = 0;
        可信度 = 1.0f;
    }

    // 新增：统一记录观测（更新时间 + 命中 + 可信度提升）
    void 记录观测(时间戳 now)
    {
        if (now == 0) return;

        if (创建时间 == 0) 创建时间 = now;
        最后观测时间 = now;
        ++命中次数;

        // 可选：每次观测轻微提升可信度（可调）
        可信度 = std::min(1.0f, 可信度 + 0.01f);
    }
    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const override {
        auto* 右 = dynamic_cast<存在节点主信息类*>(对象);
        if (!右) throw std::invalid_argument("存在节点主信息类::比较 - 对象类型错误");

        switch (字段) {
        case 枚举_比较字段::基础信息基类_名称:
            return 主信息定义_内部::比较_指针(this->名称, 右->名称, 条件);
        case 枚举_比较字段::基础信息基类_类型:
            return 主信息定义_内部::比较_指针(this->类型, 右->类型, 条件);
        default:
            // 坐标/轮廓等统一下放到“特征节点”去比
            return -1;
        }
    }
};

export class 场景节点主信息类 : public 基础信息基类
{
public:
    // ---------------- 基本归属 ----------------
    存在节点类* 宿主存在 = nullptr;   // 现实世界场景通常为 nullptr；若非空，则该场景是某存在的“内部世界”
    枚举_世界类型 世界类型 = 枚举_世界类型::现实世界;
    文章节点类* 来源文章 = nullptr;

    // ---------------- 生命周期/统计 ----------------
    时间戳 创建时间 = 0;
    时间戳 最后观测时间 = 0;
    std::uint32_t 命中次数 = 0;

    // ---------------- 逻辑内容（方案A/B） ----------------
    std::vector<状态节点类*>      状态列表;
    std::vector<动态节点类*>      动态列表;
    std::vector<二次特征节点类*>  关系列表;

    // ---------------- 方案C索引（高频访问缓存） ----------------
    std::vector<场景节点类*> 子场景索引;
    std::vector<存在节点类*> 存在索引;
    std::vector<特征节点类*> 特征索引;

public:
    场景节点主信息类() = default;

    explicit 场景节点主信息类(词性节点类* 名, 枚举_世界类型 世界型)
        : 基础信息基类(名), 世界类型(世界型)
    {
    }

    场景节点主信息类(词性节点类* 名, 词性节点类* 型,
        std::vector<状态节点类*> s,
        std::vector<动态节点类*> d = {},
        std::vector<二次特征节点类*> r = {})
        : 基础信息基类(名, 型),
        状态列表(std::move(s)),
        动态列表(std::move(d)),
        关系列表(std::move(r))
    {
    }

    // ---------------- 基础判断 ----------------
    bool 是否内部世界() const noexcept { return 宿主存在 != nullptr; }
    bool 是否现实世界() const noexcept { return 世界类型 == 枚举_世界类型::现实世界; }

    // ---------------- 命中/观测 ----------------
    void 记录观测(时间戳 now)
    {
        ++命中次数;
        最后观测时间 = now;
        if (创建时间 == 0) 创建时间 = now;
    }
    void 记录命中()
    {
        ++命中次数;
    }
    void 绑定宿主(存在节点类* host) { 宿主存在 = host; }
    void 绑定来源文章(文章节点类* a) { 来源文章 = a; }
    void 设置世界类型(枚举_世界类型 t) { 世界类型 = t; }

    // =====================================================================
    // 内部工具：包含/添加/移除/清理/去重
    // =====================================================================
private:
    template<class T>
    static bool 包含(const std::vector<T*>& v, const T* p)
    {
        return std::find(v.begin(), v.end(), p) != v.end();
    }

    template<class T>
    static void 添加唯一(std::vector<T*>& v, T* p)
    {
        if (!p) return;
        if (!包含(v, p)) v.push_back(p);
    }

    template<class T>
    static bool 移除(std::vector<T*>& v, const T* p)
    {
        auto it = std::remove(v.begin(), v.end(), p);
        if (it == v.end()) return false;
        v.erase(it, v.end());
        return true;
    }

    template<class T>
    static void 清理空(std::vector<T*>& v)
    {
        v.erase(std::remove(v.begin(), v.end(), nullptr), v.end());
    }

    template<class T>
    static void 去重(std::vector<T*>& v)
    {
        清理空(v);
        std::sort(v.begin(), v.end());                 // 指针按地址排序即可
        v.erase(std::unique(v.begin(), v.end()), v.end());
    }

public:
    // =====================================================================
    // 状态 / 动态 / 关系：增删查
    // =====================================================================
    bool 含有状态(const 状态节点类* s) const { return 包含(状态列表, s); }
    void 添加状态(状态节点类* s) { 添加唯一(状态列表, s); }
    bool 移除状态(const 状态节点类* s) { return 移除(状态列表, s); }

    bool 含有动态(const 动态节点类* d) const { return 包含(动态列表, d); }
    void 添加动态(动态节点类* d) { 添加唯一(动态列表, d); }
    bool 移除动态(const 动态节点类* d) { return 移除(动态列表, d); }

    bool 含有关联(const 二次特征节点类* r) const { return 包含(关系列表, r); }
    void 添加关联(二次特征节点类* r) { 添加唯一(关系列表, r); }
    bool 移除关联(const 二次特征节点类* r) { return 移除(关系列表, r); }

    // =====================================================================
    // 方案C索引：子场景 / 存在 / 特征：增删查
    // =====================================================================
    bool 含有子场景(const 场景节点类* s) const { return 包含(子场景索引, s); }
    void 添加子场景(场景节点类* s) { 添加唯一(子场景索引, s); }
    bool 移除子场景(const 场景节点类* s) { return 移除(子场景索引, s); }

    bool 含有存在(const 存在节点类* e) const { return 包含(存在索引, e); }
    void 添加存在(存在节点类* e) { 添加唯一(存在索引, e); }
    bool 移除存在(const 存在节点类* e) { return 移除(存在索引, e); }

    bool 含有特征(const 特征节点类* f) const { return 包含(特征索引, f); }
    void 添加特征(特征节点类* f) { 添加唯一(特征索引, f); }
    bool 移除特征(const 特征节点类* f) { return 移除(特征索引, f); }

    // =====================================================================
    // 统一整理：清理空指针 + 去重（建议：每帧更新后/存档前调用）
    // =====================================================================
    void 清理空指针()
    {
        清理空(状态列表);
        清理空(动态列表);
        清理空(关系列表);
        清理空(子场景索引);
        清理空(存在索引);
        清理空(特征索引);
    }

    void 索引去重()
    {
        去重(状态列表);
        去重(动态列表);
        去重(关系列表);
        去重(子场景索引);
        去重(存在索引);
        去重(特征索引);
    }

    void 整理()
    {
        清理空指针();
        索引去重();
    }

    void 清空内容(bool 是否清空索引 = true)
    {
        状态列表.clear();
        动态列表.clear();
        关系列表.clear();
        if (是否清空索引) {
            子场景索引.clear();
            存在索引.clear();
            特征索引.clear();
        }
    }

    // =====================================================================
    // 比较：保留你原逻辑（只比较名称/类型）
    // =====================================================================
    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const override
    {
        auto* 右 = dynamic_cast<场景节点主信息类*>(对象);
        if (!右) throw std::invalid_argument("场景节点主信息类::比较 - 对象类型错误");

        switch (字段) {
        case 枚举_比较字段::基础信息基类_名称:
            return 主信息定义_内部::比较_指针(this->名称, 右->名称, 条件);
        case 枚举_比较字段::基础信息基类_类型:
            return 主信息定义_内部::比较_指针(this->类型, 右->类型, 条件);
        default:
            return -1;
        }
    }



};

//—— 状态 / 动态 / 二次特征 / 因果：先给最小可编译骨架（你后续可在对应模块进一步细化）
export class 状态节点主信息类 : public 基础信息基类 {
public:
    时间戳 收到时间 = 0;
    时间戳 发生时间 = 0;
    基础信息节点类* 对应信息节点 = nullptr; // 通常指向“特征节点”,也有可能是一个存在
    bool 是否变化 = false;

    状态节点主信息类() = default;
    状态节点主信息类(词性节点类* 名, 词性节点类* 型) : 基础信息基类(名, 型) {}

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段, 枚举_比较条件 条件) const override {
        auto* 右 = dynamic_cast<状态节点主信息类*>(对象);
        if (!右) throw std::invalid_argument("状态节点主信息类::比较 - 对象类型错误");

        switch (字段) {
        case 枚举_比较字段::基础信息_状态节点主信息类_收到时间:
            return 主信息定义_内部::比较_整数((std::int64_t)收到时间, (std::int64_t)右->收到时间, 条件);
        case 枚举_比较字段::基础信息_状态节点主信息类_发生时间:
            return 主信息定义_内部::比较_整数((std::int64_t)发生时间, (std::int64_t)右->发生时间, 条件);
        case 枚举_比较字段::基础信息_状态节点主信息类_是否变化:
            if (条件 == 枚举_比较条件::相等) return 是否变化 == 右->是否变化 ? 1 : 0;
            if (条件 == 枚举_比较条件::不等于) return 是否变化 != 右->是否变化 ? 1 : 0;
            return -1;
        default:
            return -1;
        }
    }
};

export class 动态节点主信息类 : public 基础信息基类 {
public:
    基础信息节点类* 初始状态 = nullptr;
    基础信息节点类* 结果状态 = nullptr;
    动态节点主信息类() = default;
    动态节点主信息类(词性节点类* 名, 词性节点类* 型) : 基础信息基类(名, 型) {}

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 /*字段*/, 枚举_比较条件 /*条件*/) const override {
        if (!dynamic_cast<动态节点主信息类*>(对象)) throw std::invalid_argument("动态节点主信息类::比较 - 对象类型错误");
        return -1;
    }
};

export class 二次特征主信息类 : public 基础信息基类 {
public:
    枚举_二次特征种类 种类 = 枚举_二次特征种类::未定义;
    枚举_比较结果     结果 = 枚举_比较结果::未定义;

    // ===== 关系实例 key（你已经加了，很好）=====
    二次特征节点类* 概念模板 = nullptr;  // schema：指向二次特征概念树里的“观察”概念节点
    场景节点类* 所属场景 = nullptr;
    基础信息节点类* 主体 = nullptr;
    基础信息节点类* 客体 = nullptr;

    // ===== 比较类字段（保留兼容）=====
    基础信息节点类* 左对象 = nullptr;
    基础信息节点类* 右对象 = nullptr;
    double 差值 = 0.0;
    double 相似度 = 0.0;

    // ===== 观察关系 payload（新增）=====
    结构体_观察统计 观察{};

public:
    bool 是否观察关系() const noexcept { return 种类 == 枚举_二次特征种类::观察关系; }

    // “概念节点” vs “实例节点” 的常用判定：
    // 概念节点：概念模板==nullptr 且 主体/客体/所属场景 为空（作为 schema 存在）
    // 实例节点：概念模板!=nullptr 且 主体/客体/所属场景 不为空（作为 state 存在）
    bool 是否实例() const noexcept {
        return 概念模板 != nullptr && 所属场景 && 主体 && 客体;
    }

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 /*字段*/, 枚举_比较条件 /*条件*/) const override {
        if (!dynamic_cast<二次特征主信息类*>(对象)) throw std::invalid_argument("二次特征主信息类::比较 - 对象类型错误");
        return -1;
    }
    inline 二次特征节点类* 查找观察关系实例_在场景(
        场景节点类* 场景,
        二次特征节点类* 观察概念模板,
        基础信息节点类* 主体,
        基础信息节点类* 客体)
    {
        if (!场景 || !观察概念模板 || !主体 || !客体) return nullptr;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return nullptr;

        for (auto* r : smi->关系列表) {
            二次特征主信息类* mi = dynamic_cast<二次特征主信息类*>(r->主信息);
            if (!mi) continue;

            if (mi->种类 != 枚举_二次特征种类::观察关系) continue;
            if (mi->概念模板 != 观察概念模板) continue;
            if (mi->所属场景 != 场景) continue;
            if (mi->主体 != 主体) continue;
            if (mi->客体 != 客体) continue;

            return r;
        }
        return nullptr;
    }
};

export class 因果主信息类 : public 基础信息基类 {
public:
    因果主信息类() = default;
    因果主信息类(词性节点类* 名, 词性节点类* 型) : 基础信息基类(名, 型) {}

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 /*字段*/, 枚举_比较条件 /*条件*/) const override {
        if (!dynamic_cast<因果主信息类*>(对象)) throw std::invalid_argument("因果主信息类::比较 - 对象类型错误");
        return -1;
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
// 6) 自然语言成分族（最小骨架：如你已在别处实现更完整版本，可保持一致）
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

    explicit 文章主信息类(std::string 标题_, std::string 原文_ = "")
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


