module;

#include <windows.h>
// 主信息定义模块.ixx（更新版）
// 目标：
// 1) 按“方案C（混合子链+索引）”补齐 场景/存在 的索引字段
// 2) 保留“文本特征值”，并新增“引用特征值”以替代“用字符串主键当引用”的旧写法
// 3) 补齐/收敛 枚举_比较字段；未实现字段允许比较函数返回 -1（不支持）
// 4) 修复原文件中多处 switch/throw 花括号错误导致的不可达/缺 return 问题

#include <string>
#include <vector>
#include <deque>
#include <chrono>
#include <limits>
#include <algorithm>
#include <stdexcept>
#include <compare>
#include <unordered_set>
#include <variant>
#include <optional>
#include <functional>
#include <cstdint>
#include <utility>
#include <cmath>
#include <array>
#include <iosfwd>
#include <bit>
#include <type_traits>
#include <ostream>
#include <istream>
#include <typeinfo>
export module 主信息定义模块;



import 模板模块;
import 基础数据类型模块;
import 通用函数模块;
//======================================================================
// 1) 前置声明 & 节点别名
//======================================================================
using namespace 通用函数模块;
export class 语素基类;
export class 特征值主信息类;
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
export class 因果实例主信息类;
export class 因果模板主信息类;
export class 因果链主信息类;
export class 因果兼容主信息类;
export using 因果主信息类 = 因果兼容主信息类;

export class 基本句子主信息类;
export class 复杂句子主信息类;
export class 段落主信息类;
export class 文章主信息类;

export class 需求主信息类;
export class 任务信息基类;
export class 方法信息基类;



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
export using 因果实例节点类 = 链表模板<基础信息基类*>::节点类;
export using 因果模板节点类 = 链表模板<基础信息基类*>::节点类;
export using 因果链节点类 = 链表模板<基础信息基类*>::节点类;

export using 需求节点类 = 链表模板<需求主信息类*>::节点类;
export using 任务节点类 = 链表模板<任务信息基类*>::节点类;
export using 方法节点类 = 链表模板<方法信息基类*>::节点类;

export using 语素节点类 = 链表模板<语素基类*>::节点类;
export using 词节点类 = 链表模板<语素基类*>::节点类;
export using 词性节点类 = 链表模板<语素基类*>::节点类;
export using 短语节点类 = 链表模板<语素基类*>::节点类;

export using 子句节点类 = 链表模板<自然语言成分基类*>::节点类;
export using 自然句节点类 = 链表模板<自然语言成分基类*>::节点类;
export using 段落节点类 = 链表模板<自然语言成分基类*>::节点类;
export using 文章节点类 = 链表模板<自然语言成分基类*>::节点类;

export enum class 枚举_动态来源类型 : std::uint8_t;
export enum class 枚举_动作事件相位 : std::uint8_t;

//======================================================================
// 2) 语素族
//======================================================================

export class 语素基类 {
public:
    virtual ~语素基类() = default;
    枚举_主信息类型 主信息类型 = 枚举_主信息类型::语素基类;
    virtual std::int64_t 比较(语素基类* 对象, 枚举_比较字段 字段) const = 0;

    // ===== 序列化/反序列化（用于链表模板存档）=====
   // virtual void 序列化(std::ostream& os) const;
   // static 语素基类* 反序列化(std::istream& is);

    //  void 收集存档字段(结构体_存在存档字段& out) const;
    //  void 修复存档字段(const 结构体_存在存档字段& in, const 结构体_引用解析上下文& ctx);
};

//—— 词
export class 词主信息类 : public 语素基类 {
public:
    const std::string 词;

    explicit 词主信息类(const std::string& w = "") : 词(w) {
        主信息类型 = 枚举_主信息类型::词;
    }

    std::int64_t 比较(语素基类* 对象, 枚举_比较字段 字段) const override {
        auto* 右 = dynamic_cast<词主信息类*>(对象);
        if (!右) throw std::invalid_argument("词主信息类::比较 - 对象类型错误");

        if (字段 != 枚举_比较字段::语素_词_词) return -1;

        // 默认相等比较，使用减法
        return this->词.compare(右->词);
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
    explicit 词性主信息类(枚举_词性 词性_) : 词性(词性_) {
        主信息类型 = 枚举_主信息类型::词性;
    }


    std::int64_t 比较(语素基类* 对象, 枚举_比较字段 字段) const override {
        auto* 右 = dynamic_cast<词性主信息类*>(对象);
        if (!右) throw std::invalid_argument("词性主信息类::比较 - 对象类型错误");

        switch (字段) {
        case 枚举_比较字段::语素_词性_词性:
            // 默认相等比较，使用减法
            return static_cast<std::int64_t>(this->词性) - static_cast<std::int64_t>(右->词性);
        case 枚举_比较字段::语素_词性_对应基础信息指针:
            // 默认相等比较，使用减法
            return reinterpret_cast<std::intptr_t>(this->对应基础信息指针) - reinterpret_cast<std::intptr_t>(右->对应基础信息指针);
        default:
            return -1;
        }
    }
};

//—— 短语（简化：只保留你当前比较所需字段）
export class 短语主信息类 : public 语素基类 {
public:
    const 词性节点类* 参照词 = nullptr;
    const 词性节点类* 比较对象词 = nullptr;


    短语主信息类(const 词性节点类* a, const 词性节点类* b) : 参照词(a), 比较对象词(b) {
        主信息类型 = 枚举_主信息类型::短语;
    }

    std::int64_t 比较(语素基类* 对象, 枚举_比较字段 字段) const override {
        auto* 右 = dynamic_cast<短语主信息类*>(对象);
        if (!右) throw std::invalid_argument("短语主信息类::比较 - 对象类型错误");

        switch (字段) {
        case 枚举_比较字段::语素_短语字节点_参照词:
            // 默认相等比较，使用减法
            return reinterpret_cast<std::intptr_t>(this->参照词) - reinterpret_cast<std::intptr_t>(右->参照词);
        case 枚举_比较字段::语素_短语字节点_比较对象词:
            // 默认相等比较，使用减法
            return reinterpret_cast<std::intptr_t>(this->比较对象词) - reinterpret_cast<std::intptr_t>(右->比较对象词);
        default:
            return -1;
        }
    }
};

export class 短语子节点主信息类 : public 语素基类 {
public:
    const 词性节点类* 比较词 = nullptr;
    const 词性节点类* 结果词 = nullptr;

    短语子节点主信息类(const 词性节点类* a, const 词性节点类* b) : 比较词(a), 结果词(b) {
        主信息类型 = 枚举_主信息类型::短语子;
    }

    std::int64_t 比较(语素基类* 对象, 枚举_比较字段 字段) const override {
        auto* 右 = dynamic_cast<短语子节点主信息类*>(对象);
        if (!右) throw std::invalid_argument("短语子节点主信息类::比较 - 对象类型错误");

        switch (字段) {
        case 枚举_比较字段::语素_短语子节点_比较词:
            // 默认相等比较，使用减法
            return reinterpret_cast<std::intptr_t>(this->比较词) - reinterpret_cast<std::intptr_t>(右->比较词);
        case 枚举_比较字段::语素_短语子节点_结果词:
            // 默认相等比较，使用减法
            return reinterpret_cast<std::intptr_t>(this->结果词) - reinterpret_cast<std::intptr_t>(右->结果词);
        default:
            return -1;
        }
    }
};


//======================================================================
// 3) 特征值族（仅 VecIU64）
//======================================================================
//
// 说明：
// - 特征值链只存储“大块证据”（VecIU64），用于去重共享。
// - I64（颜色/坐标签名/计数/阈值等）与“指针引用”都内联在特征节点中，不入特征值链。
// - 特征节点引用 VecIU64 证据时，不直接依赖链节点类型，只保存主信息指针句柄（std::uintptr_t）。
//

export class 特征值主信息类 {
public:
    VecIU64 值{};
    std::uint64_t 内容哈希 = 0;     // 可由特征值模块写入/缓存
    std::uint64_t 命中次数 = 0;
    时间戳 最后命中 = 0;

    特征值主信息类() = default;
    explicit 特征值主信息类(VecIU64 v) : 值(std::move(v)) {}

    bool 绝对相同(const 特征值主信息类& rhs) const noexcept { return 值 == rhs.值; }
    void 序列化(std::ostream& os) const;
    static 特征值主信息类* 反序列化(std::istream& is);
};

export struct VecU句柄 {
    std::uintptr_t 主信息指针 = 0; // reinterpret_cast<std::uintptr_t>(特征值主信息类*)
    bool 有效() const noexcept { return 主信息指针 != 0; }
};

export struct 指针句柄 {
    std::uintptr_t 指针 = 0; // 任意运行期指针（存在节点/特征节点/...）
    bool 有效() const noexcept { return 指针 != 0; }
};

export inline bool operator==(const VecU句柄& a, const VecU句柄& b) noexcept { return a.主信息指针 == b.主信息指针; }
export inline bool operator!=(const VecU句柄& a, const VecU句柄& b) noexcept { return !(a == b); }
export inline bool operator==(const 指针句柄& a, const 指针句柄& b) noexcept { return a.指针 == b.指针; }
export inline bool operator!=(const 指针句柄& a, const 指针句柄& b) noexcept { return !(a == b); }

// 特征快照值：特征节点对外的“当前值/候选值”载体。
// - I64：内联标量（颜色/坐标签名/计数/阈值等）
// - VecU句柄：指向特征值链中的 VecIU64 证据主信息（共享）
// - 指针句柄：运行期引用（不入特征值链）
export using 特征快照值 = std::variant<std::monostate, I64, VecU句柄, 指针句柄>;

export inline bool 特征快照有值(const 特征快照值& v) noexcept {
    return !std::holds_alternative<std::monostate>(v);
}

export inline bool 特征快照相等(const 特征快照值& a, const 特征快照值& b) noexcept
{
    if (a.index() != b.index()) return false;
    if (std::holds_alternative<std::monostate>(a)) return true;
    if (const auto* av = std::get_if<I64>(&a)) {
        const auto* bv = std::get_if<I64>(&b);
        return bv && (*av == *bv);
    }
    if (const auto* av = std::get_if<VecU句柄>(&a)) {
        const auto* bv = std::get_if<VecU句柄>(&b);
        return bv && (*av == *bv);
    }
    if (const auto* av = std::get_if<指针句柄>(&a)) {
        const auto* bv = std::get_if<指针句柄>(&b);
        return bv && (*av == *bv);
    }
    return false;
}

export enum class 枚举_状态域 : std::uint8_t {
    未定义 = 0,
    内部状态 = 1,
    世界状态 = 2
};

export struct 运行时状态主信息记录 {
    枚举_状态域 状态域 = 枚举_状态域::未定义;
    时间戳 收到时间 = 0;
    时间戳 发生时间 = 0;
    基础信息节点类* 状态主体 = nullptr;
    特征节点类* 状态特征 = nullptr;
    特征快照值 状态值{};
    基础信息节点类* 对应信息节点 = nullptr;
    bool 是否变化 = false;
    std::string 变化原因类别{};
    std::string 变化原因说明{};
};

export struct 运行时动态主信息记录 {
    std::optional<运行时状态主信息记录> 初始状态{};
    std::optional<运行时状态主信息记录> 结果状态{};
    基础信息节点类* 动态主体 = nullptr;
    特征节点类* 动态特征 = nullptr;
    时间戳 开始时间 = 0;
    时间戳 结束时间 = 0;
    枚举_动态来源类型 来源类型 = static_cast<枚举_动态来源类型>(0);
    方法节点类* 来源方法首节点 = nullptr;
    const 词性节点类* 来源动作名 = nullptr;
    枚举_动作事件相位 来源动作相位 = static_cast<枚举_动作事件相位>(0);
    场景节点类* 来源输入场景 = nullptr;
    场景节点类* 来源输出场景 = nullptr;
    bool 来源执行成功 = false;
    I64 来源错误码 = 0;
    std::vector<运行时状态主信息记录> 事件列表{};
    std::vector<运行时状态主信息记录> 状态路径列表{};
    std::uint64_t 动态路径签名 = 0;
};

//======================================================================
// 四. 基础信息族
//======================================================================



export class 基础信息基类 {
public:
    const 词性节点类* 名称 = nullptr;      //自身名称  
    const 词性节点类* 类型 = nullptr;      // 自身抽象后归类名称
    std::uint64_t 命中次数 = 0;             //被找到的次数，用于可信度计算    
private:
    const 枚举_主信息类型 主信息类型 = 枚举_主信息类型::基础信息基类;
public:
    virtual ~基础信息基类() = default;
    virtual std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段) const = 0;
    // ===== 序列化/反序列化（用于链表模板存档）=====
  //  virtual void 序列化(std::ostream& os) const;
  //  static 基础信息基类* 反序列化(std::istream& is);
protected:
    基础信息基类() = default;
    基础信息基类(枚举_主信息类型  主信息类型_) : 主信息类型(主信息类型_) {}
    explicit 基础信息基类(const 词性节点类* 名, 枚举_主信息类型  主信息类型_) : 名称(名), 主信息类型(主信息类型_) {}
    基础信息基类(const 词性节点类* 名, const 词性节点类* 型, 枚举_主信息类型 主信息类型_) :名称(名), 类型(型), 主信息类型(主信息类型_) {}
    //   void 收集存档字段(结构体_存在存档字段& out) const;
    //   void 修复存档字段(const 结构体_存在存档字段& in, const 结构体_引用解析上下文& ctx);
};

//1)—— 指代
export class 指代节点主信息类 : public 基础信息基类 {
public:
    const 词性节点类* 代词 = nullptr;
    存在节点类* 指代对象 = nullptr;

    // 存档：仅存主键，指针在加载后可由外部修复
    std::string 代词主键_存档{};
    std::string 指代对象主键_存档{};

    指代节点主信息类() :基础信息基类(枚举_主信息类型::指代) {}
    指代节点主信息类(const 词性节点类* 名, const 词性节点类* 型, const 词性节点类* 代, 存在节点类* 对象)
        : 基础信息基类(名, 型, 枚举_主信息类型::指代), 代词(代), 指代对象(对象) {
    }

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段) const override {
        auto* 右 = dynamic_cast<指代节点主信息类*>(对象);
        if (!右) throw std::invalid_argument("指代节点主信息类::比较 - 对象类型错误");

        return 0;
    }
};

export template<class T, std::size_t N>
struct 环形缓冲 {
    static_assert(N >= 2);
    T data[N]{};
    std::size_t head = 0;
    std::size_t size = 0;

    void clear() { head = 0; size = 0; }

    void push(const T& v)
    {
        data[head] = v;
        head = (head + 1) % N;
        if (size < N) ++size;
    }

    // 0 表示最新，1 表示前一项...
    const T* get_from_latest(std::size_t i) const
    {
        if (i >= size) return nullptr;
        std::size_t idx = (head + N - 1 - i) % N;
        return &data[idx];
    }
};

// ========= 稳态原型：点 / 区间 =========
// - 点：用于 VecU（轮廓/颜色/签名）等不可值域聚合的情况，或 I64 早期尚未形成区间。
// - 区间：统一用“1 维标量区间”表达：
//     * 值域（I64 标量）：box=[lo,hi] 表示真实取值范围。
//     * 距离到原型（VecU / 不可排序值）：box=[lo,hi] 表示 diff(rep,v) 的允许范围（rep 必须存在）。
//     * 相似度阈值（VecU / 不可排序值）：box=[lo,hi] 表示 sim(rep,v) 的允许范围（rep 必须存在）。
//   说明：多维连续量若要走区间，请优先拆成多个 I64 特征；
//         不可排序多维量（轮廓/颜色/签名等）请编码为 VecIU64，再用“距离/相似度阈值”表达稳态。

export struct 稳态点原型
{
    特征快照值 点{};                    // I64 或 VecU句柄
    std::uint64_t 命中次数 = 0;
    时间戳 最后命中 = 0;

    bool 有效() const noexcept { return 特征快照有值(点); }
};

export struct 稳态多维区间原型
{
    I64多维区间 box{};                  // 每个维度一个 [lo,hi]
    枚举_区间语义 语义 = 枚举_区间语义::值域;

    std::uint64_t 命中次数 = 0;
    时间戳 最后命中 = 0;

    // 可选：用于解释/回溯/做 medoid（I64 或 VecU句柄）
    std::optional<特征快照值> 代表点{};

    bool 有效() const noexcept {
        if (!box.有效()) return false;
        if (box.维度.size() != 1) return false; // 当前区间统一为 1 维标量区间
        if (语义 == 枚举_区间语义::值域) return true;
        // 距离/相似度阈值：必须有代表点
        return 代表点.has_value() && 特征快照有值(*代表点);
    }
};

export using 稳态原型 = std::variant<稳态点原型, 稳态多维区间原型>;

export class 特征节点主信息类 : public 基础信息基类 {
public:
    // 当前快照：I64 内联；VecU 通过句柄引用证据池。
    特征快照值 当前快照{};

    // 当前“确认的稳态”（用索引指向 稳态表；-1 表示暂无）
    int 当前稳态索引 = -1;

    static constexpr std::size_t 候选窗口 = 60;
    环形缓冲<特征快照值, 候选窗口> 近帧候选{};
    环形缓冲<时间戳, 候选窗口>     近帧时间{};
    std::vector<稳态原型> 稳态表;        // 只存稳态（点/区间）,当特征在变化时存储出现次数或保持时间最长的值,

    // 统计
    std::uint64_t 命中次数 = 0;
    std::uint64_t 可信度 = 10000;       // Q10000

    // 区间摘要（可选）：统一为 1 维标量区间。
    // - 值域：I64 标量真实取值范围。
    // - 距离到原型：diff(区间代表点, 当前快照) 的范围（用于 VecU 等不可排序值）。
    // - 相似度阈值：sim(区间代表点, 当前快照) 的范围（用于 VecU 等不可排序值）。
    std::optional<I64多维区间> 区间摘要{};
    枚举_区间语义 区间语义 = 枚举_区间语义::未定义;
    std::optional<特征快照值> 区间代表点{}; // 仅对 距离/相似度 语义有效
    I64区间 连续测量误差区间{ 0,-1 }; //（无效表示不启用）//差异度/距离（越小越像）落入 [0, eps] 视为等价/命中 //存储测量误差造成范围值

    特征节点主信息类() :基础信息基类(枚举_主信息类型::特征) {};
    explicit 特征节点主信息类(const 词性节点类* 特征类型)
        : 基础信息基类(nullptr, 特征类型, 枚举_主信息类型::特征) {
    }

    特征节点主信息类(const 词性节点类* 特征类型, const 特征快照值& v)
        : 基础信息基类(nullptr, 特征类型, 枚举_主信息类型::特征) {
        当前快照 = v;
    }

    inline bool 有区间() const noexcept {
        return 区间摘要.has_value() && 区间摘要->有效();
    }

    // 约定：1 成立，0 不成立，-1 不支持/无法比较
    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 /*字段*/) const override {
        auto* 右 = dynamic_cast<特征节点主信息类*>(对象);
        if (!右) throw std::invalid_argument("特征节点主信息类::比较 - 对象类型错误");

        const bool 左有 = 特征快照有值(当前快照) || (区间摘要.has_value() && 区间摘要->有效());
        const bool 右有 = 特征快照有值(右->当前快照) || (右->区间摘要.has_value() && 右->区间摘要->有效());
        if (!左有 && !右有) return -1;

        bool 命中 = false;

        // (1) 当前快照严格相等
        if (!命中 && 特征快照有值(当前快照) && 特征快照有值(右->当前快照) && 快照严格相等(当前快照, 右->当前快照))
            命中 = true;

        // (2) 区间命中（只在 值域 语义下；当前仅支持 维度==1 的标量区间）
        const bool 左区间可用 = (区间语义 == 枚举_区间语义::值域) && 区间摘要 && 区间摘要->有效();
        const bool 右区间可用 = (右->区间语义 == 枚举_区间语义::值域) && 右->区间摘要 && 右->区间摘要->有效();

        if (!命中 && 左区间可用 && 右区间可用) {
            if (多维区间相交(*区间摘要, *右->区间摘要)) 命中 = true;
        }

        if (!命中 && 左区间可用) {
            if (快照值落入区间(右->当前快照, *区间摘要)) 命中 = true;
        }

        if (!命中 && 右区间可用) {
            if (快照值落入区间(当前快照, *右->区间摘要)) 命中 = true;
        }

        return 命中 ? 1 : 0;
    }

private:
    static bool 多维区间相交(const I64多维区间& A, const I64多维区间& B) noexcept {
        if (!A.有效() || !B.有效()) return false;
        if (A.维度.size() != B.维度.size()) return false;
        for (size_t i = 0; i < A.维度.size(); ++i) {
            const auto& a = A.维度[i];
            const auto& b = B.维度[i];
            if (!(a.低值 <= b.高值 && b.低值 <= a.高值)) return false;
        }
        return true;
    }

    static bool 快照值落入区间(const 特征快照值& v, const I64多维区间& R) noexcept {
        if (!R.有效()) return false;
        if (R.维度.size() != 1) return false; // 当前仅支持标量区间

        const auto* p = std::get_if<I64>(&v);
        if (!p) return false;
        const auto& d = R.维度[0];
        return (*p >= d.低值 && *p <= d.高值);
    }

    static bool 快照严格相等(const 特征快照值& a, const 特征快照值& b) noexcept {
        if (a.index() != b.index()) return false;
        if (auto* pa = std::get_if<I64>(&a)) return *pa == std::get<I64>(b);
        if (auto* ha = std::get_if<VecU句柄>(&a)) return ha->主信息指针 == std::get<VecU句柄>(b).主信息指针;
        if (auto* hp = std::get_if<指针句柄>(&a)) return hp->指针 == std::get<指针句柄>(b).指针;
        return true; // monostate
    }
};


export class 存在节点主信息类 : public 基础信息基类 {
public:

    const 存在节点类* 概念模板 = nullptr;    // 挂钩到“存在概念模板”（可选，用于抽象/识别）    
    const  场景节点类* 内部世界 = nullptr;   // 内部世界可空：仅当需要“内部坐标系/内部环境特征/复用场景算法”时再懒创建        
    std::string 概念模板主键_存档{};
    std::string 内部世界主键_存档{};
    时间戳 创建时间 = 0;
    时间戳 最后观测时间 = 0;

    std::int64_t 可信度 = 10000;  //Q10000

    // 帧间稳定统计（用于关联与过期清理）
    std::uint32_t 连续命中帧 = 0;
    std::uint32_t 连续未命中帧 = 0;

    // ===== 运动/静止辅助（用于存在融合） =====
    // 约定：单位=mm（与 类型_相对位置 写入的 mm 一致）
    bool 有位置历史 = false;
    Vector3D 上次观测位置_mm{ 0,0,0 };
    Vector3D 最近观测位置_mm{ 0,0,0 };
    std::int64_t 最近位移_mm = 0;      // |最近-上次| 的近似长度（mm）
    std::uint32_t 连续静止帧 = 0;
    std::vector<因果实例节点类*> 实例因果列表{};
    std::vector<std::string> 实例因果主键列表_存档{};
    std::vector<因果链节点类*> 因果链列表{};
    std::vector<std::string> 因果链主键列表_存档{};

public:
    存在节点主信息类() : 基础信息基类(nullptr, nullptr, 枚举_主信息类型::存在) {};
    explicit 存在节点主信息类(const 词性节点类* 名) : 基础信息基类(名, nullptr, 枚举_主信息类型::存在) {}
    存在节点主信息类(const 词性节点类* 名, const 词性节点类* 型) : 基础信息基类(名, 型, 枚举_主信息类型::存在) {}
    存在节点主信息类(const 词性节点类* 名, const 词性节点类* 型, 时间戳 t)
        : 基础信息基类(名, 型, 枚举_主信息类型::存在), 创建时间(t) {
    }
    // =====================================================================
// 比较：多态比较接口（约定：1=成立，0=不成立，-1=不支持/无法比较）
// 说明：
//  - “绝对坐标”字段会尝试从该存在的特征里读取：
//      优先：类型_绝对位置X/Y/Z
//      退化：类型_位置X/Y/Z
//    （通过“特征类型词字符串”匹配，不依赖 特征类型定义模块，避免模块循环依赖）
// =====================================================================
    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段) const override
    {
        auto* 右 = dynamic_cast<存在节点主信息类*>(对象);
        if (!右) throw std::invalid_argument("存在节点主信息类::比较 - 对象类型错误");



        switch (字段)
        {
            // ---- 通用：名称/类型（兼容两套枚举名）----        
        case 枚举_比较字段::基础信息基类_名称:
            return this->名称 == 右->名称;

        case 枚举_比较字段::类型:
        case 枚举_比较字段::基础信息基类_类型:
            return this->类型 == 右->类型;

            // ---- 指标：全局置信度（给“评估”域一个默认落点）----
        case 枚举_比较字段::评估_全局置信度:
            return this->可信度 - 右->可信度;

        default:
            return -1;
        }
    }
    std::string 取存在ID(const 存在节点类* 节点) const {
        return 节点 ? 节点->获取主键() : std::string{};
    }


    // =====================================================================
    // 命中/观测/衰减/过期
    // =====================================================================
    void 记录命中() {
        ++命中次数;
        ++连续命中帧;
        连续未命中帧 = 0;
        可信度 = std::min(10000, (int)(可信度 * 1.005));
    }

    void 记录未命中(std::int64_t 每帧衰减 = 100) {
        ++连续未命中帧;
        连续命中帧 = 0;
        可信度 = std::max((std::int64_t)0, 可信度 - 每帧衰减);
    }

    // now 约定：传入 0 表示“无效时间”
    void 记录观测(时间戳 now) {
        if (now == 0) return;
        if (创建时间 == 0) 创建时间 = now;
        最后观测时间 = now;
        记录命中();
    }

    void 重置统计() {
        命中次数 = 0;
        连续命中帧 = 0;
        连续未命中帧 = 0;
        可信度 = 10000;
    }

    bool 是否过期(时间戳 now, 时间戳 允许失联时长) const {
        if (now == 0) return false;
        if (最后观测时间 == 0) return false;
        return (now > 最后观测时间) && ((now - 最后观测时间) > 允许失联时长);
    }

    bool 含有实例因果(const 因果实例节点类* c) const {
        return std::find(实例因果列表.begin(), 实例因果列表.end(), c) != 实例因果列表.end();
    }

    void 添加实例因果(因果实例节点类* c) {
        if (!c) return;
        if (!含有实例因果(c)) 实例因果列表.push_back(c);
    }

    bool 移除实例因果(const 因果实例节点类* c) {
        auto it = std::remove(实例因果列表.begin(), 实例因果列表.end(), c);
        if (it == 实例因果列表.end()) return false;
        实例因果列表.erase(it, 实例因果列表.end());
        return true;
    }

    bool 含有因果链(const 因果链节点类* c) const {
        return std::find(因果链列表.begin(), 因果链列表.end(), c) != 因果链列表.end();
    }

    void 添加因果链(因果链节点类* c) {
        if (!c) return;
        if (!含有因果链(c)) 因果链列表.push_back(c);
    }

    bool 移除因果链(const 因果链节点类* c) {
        auto it = std::remove(因果链列表.begin(), 因果链列表.end(), c);
        if (it == 因果链列表.end()) return false;
        因果链列表.erase(it, 因果链列表.end());
        return true;
    }


};


export class 场景节点主信息类 : public 基础信息基类
{
public:
    // ---------------- 基本归属 ----------------
    const  存在节点类* 宿主存在 = nullptr;   // 现实世界场景通常为 nullptr；若非空，则该场景是某存在的“内部世界”
    枚举_世界类型 世界类型 = 枚举_世界类型::现实世界;
    const  文章节点类* 来源文章 = nullptr;

    // 存档：主键缓存（反序列化后可由外部修复指针）
    std::string 宿主存在主键_存档{};
    std::string 来源文章主键_存档{};

    // ---------------- 生命周期/统计 ----------------
    时间戳 创建时间 = 0;
    时间戳 最后观测时间 = 0;
    std::uint32_t 命中次数 = 0;

    // ---------------- 逻辑内容（方案A/B） ----------------
    std::vector<状态节点类*>      状态列表;
    std::deque<运行时状态主信息记录> 运行时状态列表{};
    std::vector<动态节点类*>      动态列表;
    std::vector<动态节点类*>      事件动态列表; // 保留近期原始动态事件，供时序/因果分析使用
    std::deque<运行时动态主信息记录> 运行时动态列表{};
    std::vector<二次特征节点类*>  关系列表;
    std::vector<因果实例节点类*>  实例因果列表;
    std::vector<std::string>      实例因果主键列表_存档{};
    std::vector<因果链节点类*>    因果链列表;
    std::vector<std::string>      因果链主键列表_存档{};




public:
    场景节点主信息类() :基础信息基类(nullptr, nullptr, 枚举_主信息类型::场景) {}

    explicit 场景节点主信息类(const 词性节点类* 名, 枚举_世界类型 世界型)
        : 基础信息基类(名, nullptr, 枚举_主信息类型::场景), 世界类型(世界型)
    {
    }

    场景节点主信息类(const 词性节点类* 名, const 词性节点类* 型,
        std::vector<状态节点类*> s,
        std::vector<动态节点类*> d = {},
        std::vector<二次特征节点类*> r = {})
        : 基础信息基类(名, 型, 枚举_主信息类型::场景),
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
        std::unordered_set<T*> seen;
        std::vector<T*> out;
        out.reserve(v.size());
        for (auto* p : v) {
            if (!p) continue;
            if (!seen.insert(p).second) continue;
            out.push_back(p);
        }
        v = std::move(out);
    }

public:
    // =====================================================================
    // 状态 / 动态 / 关系：增删查
    // =====================================================================
    bool 含有状态(const 状态节点类* s) const { return 包含(状态列表, s); }
    void 添加状态(状态节点类* s) { if (!s) return; 添加唯一(状态列表, s); }
    bool 移除状态(const 状态节点类* s) { return 移除(状态列表, s); }
    std::size_t 状态总数() const noexcept { return 状态列表.size() + 运行时状态列表.size(); }

    bool 含有动态(const 动态节点类* d) const { return 包含(动态列表, d); }
    void 添加动态(动态节点类* d) { 添加唯一(动态列表, d); }
    bool 移除动态(const 动态节点类* d) { return 移除(动态列表, d); }
    std::size_t 动态总数() const noexcept { return 动态列表.size() + 运行时动态列表.size(); }

    bool 含有事件动态(const 动态节点类* d) const { return 包含(事件动态列表, d); }
    void 添加事件动态(动态节点类* d) { 添加唯一(事件动态列表, d); }
    bool 移除事件动态(const 动态节点类* d) { return 移除(事件动态列表, d); }
    std::size_t 事件动态总数() const noexcept { return 事件动态列表.size() + 运行时动态列表.size(); }

    bool 含有关联(const 二次特征节点类* r) const { return 包含(关系列表, r); }
    void 添加关联(二次特征节点类* r) { 添加唯一(关系列表, r); }
    bool 移除关联(const 二次特征节点类* r) { return 移除(关系列表, r); }

    bool 含有实例因果(const 因果实例节点类* c) const { return 包含(实例因果列表, c); }
    void 添加实例因果(因果实例节点类* c) { 添加唯一(实例因果列表, c); }
    bool 移除实例因果(const 因果实例节点类* c) { return 移除(实例因果列表, c); }
    bool 含有因果链(const 因果链节点类* c) const { return 包含(因果链列表, c); }
    void 添加因果链(因果链节点类* c) { 添加唯一(因果链列表, c); }
    bool 移除因果链(const 因果链节点类* c) { return 移除(因果链列表, c); }



    // =====================================================================
    // 统一整理：清理空指针 + 去重（建议：每帧更新后/存档前调用）
    // =====================================================================
    void 清理空指针()
    {
        清理空(状态列表);
        清理空(动态列表);
        清理空(事件动态列表);
        清理空(关系列表);
        清理空(实例因果列表);
        清理空(因果链列表);

    }

    void 索引去重()
    {
        去重(状态列表);
        去重(动态列表);
        去重(事件动态列表);
        去重(关系列表);
        去重(实例因果列表);
        去重(因果链列表);

    }

    void 整理()
    {
        清理空指针();
        索引去重();
    }

    void 清空内容()
    {
        状态列表.clear();
        运行时状态列表.clear();
        动态列表.clear();
        事件动态列表.clear();
        运行时动态列表.clear();
        关系列表.clear();
        实例因果列表.clear();
        因果链列表.clear();

    }

    // =====================================================================
    // 比较：保留你原逻辑（只比较名称/类型）
    // =====================================================================
    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段) const override
    {
        auto* 右 = dynamic_cast<场景节点主信息类*>(对象);
        if (!右) throw std::invalid_argument("场景节点主信息类::比较 - 对象类型错误");

        switch (字段) {
        case 枚举_比较字段::名称: {

        }

                        return -1;
        }
        return 0;
    }



};

//—— 状态 / 动态 / 二次特征 / 因果：先给最小可编译骨架（你后续可在对应模块进一步细化）
export class 状态节点主信息类 : public 基础信息基类 {
public:
    // 状态域：
    // - 内部状态：自我系统内部虚拟存在的状态，如任务/需求/方法等
    // - 世界状态：世界树中某主体在某时刻的特征值
    枚举_状态域 状态域 = 枚举_状态域::未定义;
    时间戳 收到时间 = 0;
    时间戳 发生时间 = 0;
    基础信息节点类* 状态主体 = nullptr;     // “谁”的状态；内部状态常指虚拟存在，世界状态常指世界树中的主体节点
    特征节点类* 状态特征 = nullptr;         // 状态落在哪个特征槽上；没有具体特征时可为空
    特征快照值 状态值{};                    // 该时刻的统一状态值快照
    基础信息节点类* 对应信息节点 = nullptr; // 兼容旧逻辑；通常等于 状态特征 或相关主体节点
    bool 是否变化 = false;
    std::string 变化原因类别{};
    std::string 变化原因说明{};


    状态节点主信息类() : 基础信息基类(nullptr, nullptr, 枚举_主信息类型::状态) {}
    状态节点主信息类(const 词性节点类* 名, const 词性节点类* 型) : 基础信息基类(名, 型, 枚举_主信息类型::状态) {}

    bool 是否内部状态() const noexcept { return 状态域 == 枚举_状态域::内部状态; }
    bool 是否世界状态() const noexcept { return 状态域 == 枚举_状态域::世界状态; }

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段) const override {
        auto* 右 = dynamic_cast<状态节点主信息类*>(对象);
        if (!右) throw std::invalid_argument("状态节点主信息类::比较 - 对象类型错误");

        switch (字段) {
        case 枚举_比较字段::名称: {
            return -1;
        }
                        return 0;
        }
        return -1;
    }
};

export enum class 枚举_动态来源类型 : std::uint8_t {
    未定义 = 0,
    方法调用 = 1,
    内部机制动作 = 2,
};

export enum class 枚举_动作事件相位 : std::uint8_t {
    未定义 = 0,
    开始运行 = 1,
    完成 = 2,
    失败 = 3,
};

export class 动态节点主信息类 : public 基础信息基类 {
public:
    基础信息节点类* 初始状态 = nullptr;
    基础信息节点类* 结果状态 = nullptr;
    基础信息节点类* 动态主体 = nullptr;
    特征节点类* 动态特征 = nullptr;
    时间戳 开始时间 = 0;
    时间戳 结束时间 = 0;
    枚举_动态来源类型 来源类型 = 枚举_动态来源类型::未定义;
    方法节点类* 来源方法首节点 = nullptr;
    const 词性节点类* 来源动作名 = nullptr;
    枚举_动作事件相位 来源动作相位 = 枚举_动作事件相位::未定义;
    场景节点类* 来源输入场景 = nullptr;
    场景节点类* 来源输出场景 = nullptr;
    bool 来源执行成功 = false;
    I64 来源错误码 = 0;
    std::vector<状态节点类*> 事件列表{};
    std::vector<状态节点类*> 状态路径列表{};
    std::uint64_t 动态路径签名 = 0;
    动态节点主信息类() : 基础信息基类(nullptr, nullptr, 枚举_主信息类型::动态) {}
    动态节点主信息类(const 词性节点类* 名, const 词性节点类* 型) : 基础信息基类(名, 型, 枚举_主信息类型::动态) {}

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 /*字段*/) const override {
        if (!dynamic_cast<动态节点主信息类*>(对象)) throw std::invalid_argument("动态节点主信息类::比较 - 对象类型错误");
        return -1;
    }
};

//======================================================================
// 二次特征：比较结果区间（用于“概念模板/schema”）
// 约定：
// - 实例节点的比较结果永远来自：比较结果 = 左值 - 右值
// - 概念模板节点用“结果区间”描述该概念接受/命名的结果范围。
// - 区间端点默认闭区间 [下限, 上限]。
//======================================================================


export class 二次特征主信息类 : public 基础信息基类 {
public:
    二次特征主信息类()
        : 基础信息基类(nullptr, nullptr, 枚举_主信息类型::二次特征_修饰) {
    }

    // ============================================================
    // 0) 形态：schema / condition / state
    //   - schema：概念刻度节点（区间 -> 名称）
    //   - condition：绑定对象（引用概念模板，不携带方向性/阈值解释）
    //   - state：纯结果（规约标量 + 命中概念 + 统计）
    // ============================================================
    enum class 枚举_二次特征形态 : std::uint8_t {
        未定义 = 0,
        概念模板_schema = 1,
        条件_condition = 2,
        实例_state = 3
    };

    枚举_二次特征形态 形态 = 枚举_二次特征形态::未定义;

    // ============================================================
    // 1) 种类：比较对象的域（特征/存在/状态/动态/文本...）
    // ============================================================
    枚举_二次特征种类 种类 = 枚举_二次特征种类::未定义;

    // ============================================================
    // 2) ChainKey（度量签名_链键）：一条链的“根键”
    //   规则：只描述“度量定义”，不携带实例/阈值/窗口/文本。
    // ============================================================
    std::string 度量签名_链键;

    // ============================================================
    // 3) schema：概念刻度（标量区间）
    //   说明：区间端点默认闭区间 [下限, 上限]。
    // ============================================================

    // 指向概念模板（世界树中的二次特征树节点）
    // - condition：引用一个 schema 叶子（或大区间节点）
    // - state：记录本次命中的 schema 节点
    二次特征节点类* 概念模板 = nullptr;

    // schema 节点：自身代表的“区间定义”
    // condition/state：可置为无效区间 {0,-1}，靠 概念模板 指向来表达
    I64区间 标量区间{ 0, -1 };

    // 这个区间是“大区间还是小区间”（用于优先命中策略）
    enum class 枚举_区间层级 : std::uint8_t { 未定义 = 0, 大区间 = 1, 小区间 = 2 };
    枚举_区间层级 区间层级 = 枚举_区间层级::未定义;

    // schema 节点用：概念名称（例如 远/近/大/小/高/低...）
    const 词性节点类* 概念名称 = nullptr;

    // ============================================================
    // 4) condition：绑定对象（不带具体值/方向性）
    // ============================================================

    场景节点类* 所属场景 = nullptr;

    // 主体/客体：用于“参照策略”与关系语义（如 距离@主体-客体）
    基础信息节点类* 主体 = nullptr;
    基础信息节点类* 客体 = nullptr;

    // 被比较的对象（通常是特征节点/状态节点/存在节点）
    基础信息节点类* 左对象 = nullptr;
    基础信息节点类* 右对象 = nullptr;

    // ============================================================
    // 5) state：纯结果（规约标量）
    //   标量值的量纲由 度量签名_链键 对应模板决定（如 Q10000/计数/距离mm等）。
    // ============================================================

    std::int64_t 标量值 = 0;

    // 可选：当你把“条件判定输出”也缓存到 state 时使用（严格来说属于条件语义）
    bool 是否满足 = false;

    std::int64_t 置信度 = 0;

    // 观察/学习统计（可选）
    结构体_观察统计 观察{};

    // ============================================================
    // 6) 复合特征 / 多次特征（不让它引爆 ChainKey）
    // ============================================================
    enum class 枚举_度量复杂度 : std::uint8_t { 未定义 = 0, 原子 = 1, 复合 = 2, 多次 = 3 };
    枚举_度量复杂度 度量复杂度 = 枚举_度量复杂度::未定义;

    std::vector<std::string> 依赖度量签名表;

    enum class 枚举_时间窗 : std::uint8_t { 未定义 = 0, 短 = 1, 中 = 2, 长 = 3 };
    枚举_时间窗 时间窗 = 枚举_时间窗::未定义;

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 /*字段*/) const override {
        auto* o = dynamic_cast<二次特征主信息类*>(对象);
        if (!o) throw std::invalid_argument("二次特征主信息类::比较 - 对象类型错误");

        if (度量签名_链键 != o->度量签名_链键) {
            return 度量签名_链键 < o->度量签名_链键 ? -1 : 1;
        }
        if (标量值 == o->标量值) return 0;
        return 标量值 < o->标量值 ? -1 : 1;
    }
};


export enum class 枚举_因果锚点类型 : std::uint8_t {
    未定义 = 0,
    场景 = 1,
    存在 = 2
};

export enum class 枚举_因果来源类型 : std::uint8_t {
    未定义 = 0,
    观察生成 = 1,
    执行生成 = 2,
    推断生成 = 3
};

export enum class 枚举_因果校验结果 : std::uint8_t {
    合法 = 0,
    条件为空 = 1,
    结果为空 = 2,
    锚点不一致 = 3,
    层级不一致 = 4,
    时间逆序 = 5,
    动作语义缺失 = 6,
    跨层直连 = 7,
    状态面不一致 = 8
};

export class 因果实例主信息类 : public 基础信息基类 {
public:
    枚举_因果锚点类型 锚点类型 = 枚举_因果锚点类型::未定义;
    std::uint64_t 锚点主键 = 0;
    std::string 锚点主键_存档{};
    std::uint32_t 状态层级 = 0;
    std::uint64_t 状态面签名 = 0;
    基础信息节点类* 锚点信息 = nullptr;

    时间戳 条件时间 = 0;
    时间戳 动作时间 = 0;
    时间戳 结果时间 = 0;

    std::vector<状态节点类*> 条件状态列表{};
    std::vector<状态节点主信息类> 条件状态主信息列表{};
    std::vector<std::string> 条件状态主键列表_存档{};
    基础信息节点类* 动作主体 = nullptr;
    std::string 动作主体主键_存档{};
    基础信息节点类* 动作语义 = nullptr;
    std::string 动作语义主键_存档{};
    std::string 动作语义键{};
    基础信息节点类* 动作信息 = nullptr;
    动态节点类* 动作动态 = nullptr;
    动态节点类* 触发动态 = nullptr;
    std::string 触发动态主键_存档{};

    std::vector<状态节点类*> 结果状态列表{};
    std::vector<状态节点主信息类> 结果状态主信息列表{};
    std::vector<std::string> 结果状态主键列表_存档{};
    状态节点类* 主结果状态 = nullptr;
    std::vector<状态节点主信息类> 主结果状态主信息列表{};
    std::string 主结果状态主键_存档{};
    std::vector<动态节点类*> 结果动态列表{};
    std::vector<std::string> 结果动态主键列表_存档{};
    动态节点类* 主结果动态 = nullptr;
    std::string 主结果动态主键_存档{};

    枚举_因果来源类型 来源类型 = 枚举_因果来源类型::未定义;
    std::uint64_t 来源任务主键 = 0;
    std::string 来源任务主键_存档{};
    std::uint64_t 来源方法主键 = 0;
    std::string 来源方法主键_存档{};
    std::int64_t 置信度 = 0;
    bool 已验证 = false;

public:
    因果实例主信息类() : 基础信息基类(nullptr, nullptr, 枚举_主信息类型::因果) {}
    因果实例主信息类(const 词性节点类* 名, const 词性节点类* 型) : 基础信息基类(名, 型, 枚举_主信息类型::因果) {}

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 /*字段*/) const override {
        if (!dynamic_cast<因果实例主信息类*>(对象)) throw std::invalid_argument("因果实例主信息类::比较 - 对象类型错误");
        return -1;
    }
};

export class 因果模板主信息类 : public 基础信息基类 {
public:
    枚举_因果锚点类型 适用锚点类型 = 枚举_因果锚点类型::未定义;
    std::uint32_t 适用层级 = 0;
    std::uint64_t 适用状态面签名 = 0;

    std::vector<二次特征节点类*> 条件模板列表{};
    std::vector<std::string> 条件模板主键列表_存档{};
    基础信息节点类* 动作模板 = nullptr;
    std::string 动作模板主键_存档{};
    std::vector<二次特征节点类*> 结果模板列表{};
    std::vector<std::string> 结果模板主键列表_存档{};
    std::vector<因果实例节点类*> 证据实例列表{};
    std::vector<std::string> 证据实例主键列表_存档{};
    std::vector<因果实例节点类*> 代表证据列表{};
    std::vector<std::string> 代表证据主键列表_存档{};
    std::vector<因果实例节点类*> 反例实例列表{};
    std::vector<std::string> 反例主键列表_存档{};

    std::uint64_t 证据次数 = 0;
    std::uint64_t 成立次数 = 0;
    std::uint64_t 失败次数 = 0;
    时间戳 最近命中时间 = 0;
    时间戳 最近失败时间 = 0;
    std::int64_t 稳定度 = 0;

    std::string 典型条件方向摘要{};
    std::string 典型动作语义摘要{};
    std::string 典型结果方向摘要{};
    std::string 最常见失败边界摘要{};

public:
    因果模板主信息类() : 基础信息基类(nullptr, nullptr, 枚举_主信息类型::因果) {}
    因果模板主信息类(const 词性节点类* 名, const 词性节点类* 型) : 基础信息基类(名, 型, 枚举_主信息类型::因果) {}

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 /*字段*/) const override {
        if (!dynamic_cast<因果模板主信息类*>(对象)) throw std::invalid_argument("因果模板主信息类::比较 - 对象类型错误");
        return -1;
    }
};

export class 因果链主信息类 : public 基础信息基类 {
public:
    std::vector<因果实例节点类*> 因果实例序列{};
    std::vector<std::string> 因果实例主键序列_存档{};
    std::uint64_t 起点锚点主键 = 0;
    std::string 起点锚点主键_存档{};
    std::uint64_t 终点锚点主键 = 0;
    std::string 终点锚点主键_存档{};
    时间戳 起始时间 = 0;
    时间戳 结束时间 = 0;

public:
    因果链主信息类() : 基础信息基类(nullptr, nullptr, 枚举_主信息类型::因果) {}
    因果链主信息类(const 词性节点类* 名, const 词性节点类* 型) : 基础信息基类(名, 型, 枚举_主信息类型::因果) {}

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 /*字段*/) const override {
        if (!dynamic_cast<因果链主信息类*>(对象)) throw std::invalid_argument("因果链主信息类::比较 - 对象类型错误");
        return -1;
    }
};

// 兼容旧实现：当前运行链路仍直接读取这些字段。
// 这层只负责过渡，新的核心定义请以上面三个主信息类为准。
export enum class 枚举_因果结构类型 : std::uint8_t {
    未定义 = 0,
    实例 = 1,
    模板 = 2,
    链 = 3
};

export class 因果兼容主信息类 : public 因果实例主信息类 {
public:
    枚举_因果结构类型 结构类型 = 枚举_因果结构类型::实例;
    std::string 因果累计键_存档{};
    时间戳 首次发生时间 = 0;
    时间戳 最近发生时间 = 0;
    std::int64_t 观察次数 = 0;
    std::int64_t 稳定度Q = 0;
    std::int64_t 安全归因Q = 0;
    std::int64_t 服务归因Q = 0;
    bool 是否原始观测实例 = false;
    std::int64_t 原始观测序号 = 0;
    基础信息节点类* 归纳因果节点 = nullptr;

    动态节点类* 因信息 = nullptr;
    动态节点类* 果信息 = nullptr;
    动态节点类* 锚点动态 = nullptr;
    std::vector<二次特征节点类*> 条件方向二次特征列表{};
    std::vector<std::int64_t> 条件方向值列表{};
    std::vector<std::uint8_t> 条件方向可比较列表{};
    std::vector<动态节点类*> 过程动态列表{};
    状态节点类* 结果状态 = nullptr;
    二次特征节点类* 结果方向二次特征 = nullptr;
    std::string 结果方向链键{};
    std::int64_t 结果方向值 = 0;
    bool 结果方向可比较 = false;
    std::vector<状态节点类*> 事件分段列表{};
    时间戳 分段开始时间 = 0;
    时间戳 分段结束时间 = 0;
    基础信息节点类* 因果主体 = nullptr;

public:
    因果兼容主信息类() : 因果实例主信息类() {}
    因果兼容主信息类(const 词性节点类* 名, const 词性节点类* 型) : 因果实例主信息类(名, 型) {}

    std::int64_t 比较(基础信息基类* 对象, 枚举_比较字段 字段) const override {
        auto* 右 = dynamic_cast<因果兼容主信息类*>(对象);
        if (!右) throw std::invalid_argument("因果兼容主信息类::比较 - 对象类型错误");
        return 因果实例主信息类::比较(右, 字段);
    }
};


//======================================================================
// 五、高级信息族：需求 / 任务 / 方法（方法树：首-条件-结果）
// 约束：
// - 高级信息只引用现有节点（场景/存在/特征类型/特征值节点等），不引入新“专用数据结构”
// - 动作通过“动作名+动作句柄”标识，真实可执行体由外部注册表/调度器映射
// - 结果节点只保存“写入效果指针+摘要”，不保存大对象（ROI/掩膜/图像/点云）
//======================================================================




// -------------------- 工具：比较 --------------------
// 已迁移到 通用函数类（比较指针_条件 / 比较I64_条件 / 比较非负64位_条件）。


// -------------------- 高级信息基类 --------------------
// 需求/任务/方法三者共用：
// - 名称/类型：用于索引（词性节点）
// - 命中次数：统计/淘汰/热度


// =========================================================
// 需求主信息（高级）
// - 只引用现有节点：存在/状态/二次特征 等
// =========================================================

export class 需求主信息类 {
public:
    const 自然句节点类* 描述信息 = nullptr;       //需求信息的自然语言描述信息，用于自然语言理解与解释
    const 词性节点类* 类型 = nullptr;              //需求类型，用于分类和索引
    std::uint64_t 命中次数 = 0;    存在节点类* 需求主体 = nullptr;
    场景节点类* 需求场景 = nullptr;     //需求发生的场景,可选,如果不空则指向世界树中的一个场景节点,如果空则表示��需求是“抽象的”或“全局的”
    存在节点类* 被需求存在 = nullptr;     //单个节点,一个需求只服务于一个存在
    状态节点类* 被需求状态 = nullptr;  //被需求状态可以是一个节点也可以是一个链表,甚至可以是一棵树,但都是被需求存在的状态,此处存根指针

    存在节点类* 需求虚拟存在 = nullptr; // 内部状态/统计锚点；需求本身的长期信息优先落这里
    时间戳 需求生成时间 = 0;
    时间戳 需求有效截止 = 0;
    std::int64_t 权重 = 0;            //来源于父级需求,最根本处为服务值或安全值,此处为0表示未知
    任务节点类* 相关任务 = nullptr;          //指向一个任务节点,表示这个需求由哪个任务负责实现,此处存根指针
    std::vector<std::string> 需求签名;              //由需求类型和被需求状态的特征值组合生成的字符串,用于快速匹配和索引
    需求主信息类() = default;
    需求主信息类(const 自然句节点类* 描述信息_, const 词性节点类* 型) :描述信息(描述信息_), 类型(型) {}
    需求主信息类(存在节点类* 主体, 存在节点类* 对象, 状态节点类* 状态, std::int64_t w = 0)
        : 需求主体(主体), 被需求存在(对象), 被需求状态(状态), 权重(w) {
    }

    std::int64_t 比较(需求主信息类* 对象, 枚举_比较字段 字段) const {
        auto* 右 = 对象;
        if (!右) throw std::invalid_argument("需求主信息类::比较 - 对象类型错误");


        return -1;
    }

    // ===== 序列化/反序列化（用于链表模板存档）=====
  //  void 序列化(std::ostream& os) const;
  //  static 需求主信息类* 反序列化(std::istream& is);

};




// =========================
// 任务主信息：最小干净版
// - 步骤 = 子任务引用（可递归分解）
// - 运行统计/派生评估：走“任务虚拟存在树”
// =========================

export enum class 枚举_任务节点种类 : std::uint8_t {
    未定义 = 0,
    头结点 = 1,
    步骤节点 = 2,
    结果节点 = 3,
};

export enum class 枚举_任务状态 : std::uint8_t {
    未定义 = 0,
    未启动 = 1,
    运行中 = 2,
    挂起 = 3,
    完成 = 4,
    失败 = 5,
    取消 = 6,
    超时 = 7,
    就绪 = 8,
    执行中 = 9,
    筹办中 = 10,
    排队中 = 11,
    等待中 = 12,
    无法执行 = 13,
    待重筹办 = 14,
};

export enum class 枚举_需求运行状态 : std::uint8_t {
    未知 = 0,
    待执行 = 1,
    已满足 = 2,
    已取消 = 3,
    失败 = 4,
};

export enum class 枚举_任务返回消息类型 : std::uint8_t {
    未定义 = 0,
    步骤完成 = 1,
    步骤失败 = 2,
    步骤无法执行 = 3,
    生成子任务 = 4,
    子任务完成 = 5,
    子任务失败 = 6,
    子任务无法执行 = 7,
};

export enum class 枚举_任务执行记账结果 : std::uint8_t {
    未定义 = 0,
    未完成 = 1,
    成功 = 2,
    失败 = 3,
};

export struct 结构_任务返回消息 {
    枚举_任务返回消息类型 类型 = 枚举_任务返回消息类型::未定义;
    任务节点类* 来源任务 = nullptr;
    任务节点类* 来源步骤 = nullptr;
    任务节点类* 相关子任务 = nullptr;
    需求节点类* 对应需求 = nullptr;
    bool 需求已满足 = false;
    bool 已进入下一阶段 = false;
    bool 需要重筹办 = false;
    std::string 摘要{};
};

// 任务树类型：用于执行器决定“遇到缺口时优先分解还是执行”
export enum class 枚举_任务树类型 : std::uint8_t {
    未定义 = 0,
    编排任务 = 1,   // 主要职责：分解/组织/调度子任务
    叶子任务 = 2,   // 主要职责：直接选方法执行并产出结果
};

// 结果命中后的默认动作
export enum class 枚举_分支选择动作 : std::uint8_t {
    未定义 = 0,
    继续下一步 = 1,
    回跳重试 = 2,
    生成补条件子任务 = 3,
    转入尝试学习 = 4,
    挂起等待 = 5,
};

export enum class 枚举_任务结果角色 : std::uint8_t {
    未定义 = 0,
    预测结果 = 1,
    实际结果 = 2,
};

// ======= 任务节点主信息基类（实例运行信息）=======
export class 任务信息基类 {
public:
    // 基本标识
    const 词性节点类* 名称 = nullptr;
    const 自然句节点类* 描述信息 = nullptr;
    const 词性节点类* 类型 = nullptr;
    枚举_任务节点种类 节点种类 = 枚举_任务节点种类::未定义;

    // 执行元信息
    // - 状态字段仅作为运行期镜像/缓存；权威状态以任务虚拟存在中的特征为准
    枚举_任务状态 状态 = 枚举_任务状态::未定义;
    时间戳 创建时间 = 0;

    // 优先级规则：
    // - 基准优先级：来自主任务需求，是整棵任务树共享的基础值
    // - 局部优先级偏移：步骤命中分、并列候选分等局部调度修正
    // - 调度优先级：运行期实际排队值，由执行器按当前上下文刷新
    std::int64_t 基准优先级 = 0;
    std::int64_t 局部优先级偏移 = 0;
    std::int64_t 调度优先级 = 0;

    // 场景锚点（每个节点只有一个；语义由 节点种类 决定）
    // - 头结点：任务全局条件场景
    // - 步骤节点：该步骤的局部条件/执行场景
    // - 结果节点：该步骤的预测结果或实际结果场景
    场景节点类* 场景 = nullptr;

    // 任务虚拟存在锚点：运行状态与统计写在这里
    存在节点类* 任务虚拟存在 = nullptr;

    枚举_任务树类型 任务树类型 = 枚举_任务树类型::未定义;

    // 执行流水账镜像：热路径先落到内存，再由统一入口冲刷到任务虚拟存在。
    std::int64_t 运行次数 = 0;
    std::int64_t 成功次数 = 0;
    std::int64_t 失败次数 = 0;
    std::int64_t 连续失败次数 = 0;
    std::int64_t 转入学习次数 = 0;
    std::int64_t 最近错误码 = 0;
    时间戳 最近开始时间 = 0;
    时间戳 最近结束时间 = 0;
    时间戳 最近调度时间 = 0;
    方法节点类* 当前方法首节点 = nullptr;
    方法节点类* 最近方法首节点 = nullptr;

    virtual ~任务信息基类() = default;
};

// ======= 头结点主信息：需求 + 任务全局条件 =======
export class 任务头结点信息 final : public 任务信息基类 {
public:
    需求节点类* 需求 = nullptr;
    bool 是否真根任务 = false;
    bool 是否常驻任务 = false;
    bool 等待学习唤醒 = false;
    方法节点类* 等待学习方法首节点 = nullptr;
    任务节点类* 父任务头结点 = nullptr;
    任务节点类* 来源父结果节点 = nullptr;
    任务节点类* 来源父步骤节点 = nullptr;
    任务节点类* 当前步骤节点 = nullptr;
    任务节点类* 当前子任务头结点 = nullptr;
    任务节点类* 最近实际结果节点 = nullptr;
    std::vector<任务节点类*> 直属子任务头结点列表{};
    std::vector<std::string> 目标维度签名{};
};

export class 任务步骤节点信息 final : public 任务信息基类 {
public:
    任务节点类* 所属任务头结点 = nullptr;
    std::vector<方法节点类*> 可用方法首节点列表{};
    方法节点类* 当前选中方法首节点 = nullptr;
    std::vector<任务节点类*> 并发子任务头结点列表{};
    任务节点类* 当前子任务头结点 = nullptr;
    std::int32_t 步骤序号 = 0;
    std::int32_t 已重试次数 = 0;
    std::int32_t 允许重试次数 = 0;
    时间戳 超时截止时间 = 0;
    bool 允许切换方法 = true;
};

// ======= 结果节点主信息：步骤下的预测结果/实际结果 =======
export class 任务结果节点信息 final : public 任务信息基类 {
public:
    枚举_任务结果角色 结果角色 = 枚举_任务结果角色::未定义;
    任务节点类* 所属任务头结点 = nullptr;
    任务节点类* 来源步骤节点 = nullptr;
    std::vector<任务节点类*> 派生子任务头结点列表{};
    方法节点类* 来源方法首节点 = nullptr;
    方法节点类* 对应方法结果节点 = nullptr;
    任务节点类* 命中预测结果节点 = nullptr;
    枚举_分支选择动作 命中后动作 = 枚举_分支选择动作::未定义;
    bool 是否预测外结果 = false;
    std::int64_t 置信度_Q10000 = 0;
    std::int64_t 错误码 = 0;
    存在节点类* 评估摘要存在 = nullptr;
};

// =========================================================
// 方法树节点（高级）：首节点 / 条件节点 / 结果节点
// =========================================================


export enum class 枚举_方法节点种类 : std::uint8_t {
    未定义 = 0,
    方法首节点 = 1,
    方法条件节点 = 2,
    方法结果节点 = 3
};

// ============================================================
// 0) 额外枚举：方法来源（学习/稳定阶段已迁移到虚拟存在评估包）
// ============================================================
export enum class 枚举_方法沉淀来源 : std::uint8_t {
    未定义 = 0,
    本能 = 1,     // 本能函数自动沉淀
    反推 = 2,     // 因果反推得到的新方法
    组合 = 3,      // 多方法组合/树
    外部导入 = 4,
    运行期临时 = 5
};

// ============================================================
// 1) 方法信息基类（所有方法节点主信息基类）
// ============================================================
export enum class 枚举_动作句柄类型 : std::uint8_t {
    未设置 = 0,
    本能函数ID = 1,     // id 为枚举/整数
    外部实现主键 = 2,   // 外部插件/API/脚本等
    动作序列主键 = 3,   // 组合动作（动作线程解释执行）
};
export struct 结构体_动作句柄 {
    枚举_动作句柄类型 类型 = 枚举_动作句柄类型::未设置;

    // 三选一使用：
    I64 本能ID = 0;               // 当 类型==本能函数ID
    std::string 外部实现主键;            // 当 类型==外部实现主键
    std::string 动作序列主键;            // 当 类型==动作序列主键

    bool 有效() const {
        switch (类型) {
        case 枚举_动作句柄类型::本能函数ID: return 本能ID != 0;
        case 枚举_动作句柄类型::外部实现主键: return !外部实现主键.empty();
        case 枚举_动作句柄类型::动作序列主键: return !动作序列主键.empty();
        default: return false;
        }
    }
};


/// <方法类summary>//////////////////////////////////
/// //////////////////////////
/// </summary>///////////////////////////////////////
export struct 结构体_节点评估原始量 {
    // ===== 计数类 =====
    I64 节点_运行次数 = 0;
    I64 节点_成功次数 = 0;
    I64 节点_失败次数 = 0;

    I64 节点_样本数 = 0;
    I64 节点_新结果次数 = 0;
    I64 节点_连续成功次数 = 0;
    I64 节点_连续失败次数 = 0;

    // ===== 时间类 =====
    I64 节点_最后运行时间_微秒 = 0;
    I64 节点_最后成功时间_微秒 = 0;
    I64 节点_最后失败时间_微秒 = 0;
    I64 节点_累计耗时_微秒 = 0;

    // ===== 集合类（用于 |集合| 派生风险/缺口/副作用强度）=====
    std::unordered_set<std::string> 节点_坏因集合;
    std::unordered_set<std::string> 节点_缺口集合;
    std::unordered_set<std::string> 节点_副作用集合;

    // ===== 字符串类（可选）=====
    std::string 节点_阶段标记;
};


export class 方法信息基类 {
public:
    const 自然句节点类* 描述信息 = nullptr;
    结构体_节点评估原始量 评估原始量;
    场景节点类 场景根节点{};   //头结点中作为条件节点模板,条件节点中存储实际条件,结果节点中存储结果
    // 节点种类：方法树节点（首/条件/结果）
    枚举_方法节点种类 节点种类 = 枚举_方法节点种类::未定义;
    存在节点类* 方法虚拟存在根; // 存储方法相关的二次特征信息（例如成功率等），可选填充
    // 统一统计：对于首节点=命中次数；对于条件节点=样本数；对于结果节点=命中次数
    // 运行态统计/学习状态 已迁移到 虚拟存在特征（节点特征更新 + 评估包派生）
    方法信息基类() = default;
    方法信息基类(const 自然句节点类* 描述) : 描述信息(描述) {}
    virtual ~方法信息基类() = default;
    virtual std::int64_t 比较(方法信息基类* 对象, 枚举_比较字段 字段) const {
        return -1;
    }

    // ===== 序列化/反序列化（用于链表模板存档）=====
  //  virtual void 序列化(std::ostream& os) const;
  //  static 方法信息基类* 反序列化(std::istream& is);

};

// ============================================================
// 3) 方法首节点：动作标签（一个动作/实现）
//    - 负责：
// ============================================================

export class 方法首节点主信息类 : public 方法信息基类 {
public:
    // 动作名：注册本能函数名 或 其它方法名（词性节点表达）
    const 词性节点类* 动作名 = nullptr;
    结构体_动作句柄 动作句柄; //可以是本能ID或外部实现主键等,查找动作时使用 
    // 学习/稳定阶段：由虚拟存在特征 + 评估包派生给出（主信息不再存字段）
    枚举_方法沉淀来源 来源 = 枚举_方法沉淀来源::未定义;
    std::vector<std::string> 影响度量签名;
    方法首节点主信息类() { 节点种类 = 枚举_方法节点种类::方法首节点; }
    方法首节点主信息类(const 词性节点类* 动作, 结构体_动作句柄  动作句柄_) : 动作名(动作), 动作句柄(动作句柄_) { 节点种类 = 枚举_方法节点种类::方法首节点; }

    std::int64_t 比较(方法信息基类* 对象, 枚举_比较字段 字段) const override {
        auto* 右 = dynamic_cast<方法首节点主信息类*>(对象);
        if (!右) throw std::invalid_argument("方法首节点主信息类::比较 - 对象类型错误");

        // 由于枚举_比较字段中没有方法相关的字段，直接返回-1表示不支持
        return -1;
    }
};

// ============================================================
// 4) 方法条件节点：条件簇与条件模板
//    - 场景模板是长期资产（属于本节点），执行时克隆
//    - 条件因子向量用于：condSig生成、相似匹配、条件融合/细化、覆盖统计
// ============================================================
export class 方法条件节点主信息类 : public 方法信息基类 {
public:


    // 条件判定/检索索引：二次特征区间指向表
    // 解释：当前场景 -> 提取对应二次特征 -> 命中这些区间 => 条件满足
    std::vector<二次特征节点类> 条件判定索引;        //这里二次特征是一次性生成,独立存在,本节点专有
    时间戳 时间起 = 0;
    时间戳 时间止 = 0;
    std::string 条件场景模板主键{}; // 条件场景模板主键，用于去重
    方法条件节点主信息类() { 节点种类 = 枚举_方法节点种类::方法条件节点; }
    std::int64_t 比较(方法信息基类* 对象, 枚举_比较字段 字段) const override {
        auto* 右 = dynamic_cast<方法条件节点主信息类*>(对象);
        if (!右) throw std::invalid_argument("方法条件节点主信息类::比较 - 对象类型错误");

        return -1;
    }
};


// ============================================================
// 5) 方法结果节点：结果模板 + 显性条目（显性查询） + token差分（快速评估）
// ============================================================
export enum class 枚举_方法作用对象 : std::uint8_t {
    任意 = 0,
    执行者 = 1,
    目标 = 2,
    环境 = 3,
    第三方 = 4
};

// 一个“结果项”就是：哪个对象(角色) 的 哪种特征 在什么区间内（区间可空表示“只要类型命中”）

export class 方法结果节点主信息类 : public 方法信息基类 {
public:


    std::vector<二次特征节点类> 结果摘要索引; //这里二次特征是一次性生成, 独立存在,本节点专有
    方法结果节点主信息类() { 节点种类 = 枚举_方法节点种类::方法结果节点; }

    std::int64_t 比较(方法信息基类* 对象, 枚举_比较字段 字段) const override {
        auto* 右 = dynamic_cast<方法结果节点主信息类*>(对象);
        if (!右) throw std::invalid_argument("方法结果节点主信息类::比较 - 对象类型错误");
        return  -1;
    }
};






//======================================================================
// 6) 自然语言成分族（最小骨架：如你已在别处实现更完整版本，可保持一致）
//======================================================================

export class 自然语言成分基类 {
public:
    virtual ~自然语言成分基类() = default;
    // ===== 序列化/反序列化（用于链表模板存档）=====
 //   virtual void 序列化(std::ostream& os) const;
 //   static 自然语言成分基类* 反序列化(std::istream& is);

};

export class 句子成分主信息类 : public 自然语言成分基类 {
public:
    枚举_句子成分类型 类型 = 枚举_句子成分类型::其它;

    // 指向语素集中的“词性节点”
    const 词性节点类* 对应词性节点 = nullptr;

    // 在所属自然句的 词序列 中的下标
    std::size_t 在句子中的位置 = 0;

    句子成分主信息类() = default;

    句子成分主信息类(枚举_句子成分类型 t,
        const 词性节点类* 节点,
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
    std::vector<const 词性节点类*> 词序列;

    // 原始句子文本（可选）
    std::string 原始文本;

    // 可选：针对“主干句子”的成分（不含所有子句）
    const 词性节点类* 主语 = nullptr;
    const 词性节点类* 谓语 = nullptr;
    const 词性节点类* 宾语 = nullptr;

    // 在自然语言树中，可以将子句节点挂在 自然句节点 的子链上，
    // 这里保留一个快速访问的列表（不是必须）
    std::vector<子句节点类*> 子句节点列表;

    自然句主信息类() = default;

    自然句主信息类(std::size_t                    序号,
        std::vector<const 词性节点类*>  词序列_,
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
    std::vector<const 词性节点类*> 局部词序列;

    // 针对这个子句的句子成分（可能与整个自然句的主语/谓语不同）
    const 词性节点类* 子句主语 = nullptr;
    const 词性节点类* 子句谓语 = nullptr;
    const 词性节点类* 子句宾语 = nullptr;
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






// 运算器：对抽到的值做二元/多元计算，产出“中间证据”（差值/距离/排序/关系枚举…）
export enum class 枚举_运算类型 { 差值, 距离_L2, 比值, 排序, 阈值比较, 方向_Y, 包含, 相交, 自定义 };
export struct 结构体_运算结果 { bool 有效 = false; int64_t 标量 = 0; double 浮点 = 0; int8_t 符号 = 0; int 枚举关系 = 0; /*证据*/ };

// ======================= 二次特征：数值区间（用于“概念=关系”固化） =======================
// 约定：
// - 二次特征把“两个对象的关系”固化为：左值 - 右值 = 比较结果。
// - 概念节点（schema）通常用区间表达允许/代表的关系范围（例如：结果 in [0,0] 表示“相同”）。
// - 实例节点（state）可用点区间 [v,v] 表示一次具体观测。


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
    std::vector<const 词性节点类*> 词序列槽位;
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
export using 函数_线性化 = std::function<std::vector<const 词性节点类*>(const 结构体_语言模板&, const 结构体_线性化上下文&)>;

export struct 结构体_LK生成输入 {
    枚举_LK用途 用途;
    std::string 模板ID;            // 可空：自动选最匹配
    场景节点类* 目标场景;           // 抽象/当前场景
    std::unordered_map<std::string, 基础信息节点类*> 预绑定; // 传入已有绑定（如 E1/E2）
};
export struct 结构体_LK生成输出 {
    短语节点类* 语言节点 = nullptr;
    std::vector<const 词性节点类*> 词序列;   // 已按策略顺序生成
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
    const 词性节点类* 输出名称 = nullptr;  // 结果名
    语素节点类* 输出类型 = nullptr;  // 结果类型
};

export struct 结构体_判断输出 {
    二次特征节点类* 证据节点1 = nullptr;
    二次特征节点类* 证据节点2 = nullptr;
    基础信息节点类* 结果节点 = nullptr;
    bool 成功 = false;
    double 置信度 = 1.0;
};


// ======================= 主信息序列化辅助函数 =======================
// 说明：
// - 这些函数用于 链表模板 的保存/读取。
// - 指针型字段无法在反序列化阶段直接复原（缺少"主键->节点指针"的全局映射）。
//   本模块只保存/读回"主键缓存"，由上层（例如 世界树类 / 数据仓库类）在加载完所有链后统一修复。
// - 注意：在模块接口中无法直接对 std::ostream/std::istream 调用成员方法，
//   因此这里只提供声明，具体实现在模块的实现部分。
/*
namespace 主信息定义模块_存档_detail
{
    template<class T>
    inline void 写POD(std::ostream& os, const T& v);

    template<class T>
    inline bool 读POD(std::istream& is, T& v);

    inline void 写字符串(std::ostream& os, const std::string& s);

    inline bool 读字符串(std::istream& is, std::string& out, std::uint32_t 上限 = 16u * 1024u * 1024u);

    template<class NodePtr>
    inline std::string 节点主键(NodePtr p);

    template<class Enum>
    inline void 写枚举(std::ostream& os, Enum e);

    template<class Enum>
    inline bool 读枚举(std::istream& is, Enum& e);
}

// ----------------------- 特征值主信息类 -----------------------
inline void 特征值主信息类::序列化(std::ostream& os) const
{
    using namespace 主信息定义模块_存档_detail;
    const std::uint16_t ver = 2;
    写POD(os, ver);

    const std::uint32_t n = (std::uint32_t)值.size();
    写POD(os, n);
    for (auto x : 值) 写POD(os, x);

    写POD(os, 内容哈希);
    写POD(os, 命中次数);
    写POD(os, 最后命中);
}

inline 特征值主信息类* 特征值主信息类::反序列化(std::istream& is)
{
    using namespace 主信息定义模块_存档_detail;
    std::uint16_t ver = 0;
    if (!读POD(is, ver)) return nullptr;

    auto* out = new 特征值主信息类{};

    if (ver == 2) {
        std::uint32_t n{};
        if (!读POD(is, n)) { delete out; return nullptr; }
        out->值.resize(n);
        for (std::uint32_t i = 0; i < n; ++i) {
            if (!读POD(is, out->值[i])) { delete out; return nullptr; }
        }
        if (!读POD(is, out->内容哈希)) { delete out; return nullptr; }
        if (!读POD(is, out->命中次数)) { delete out; return nullptr; }
        if (!读POD(is, out->最后命中)) { delete out; return nullptr; }
        return out;
    }

    if (ver == 1) {
    // 兼容旧存档：旧版特征值链可能混入 I64/字符串等值。
        // 新结构只保留 VecIU64，其他载荷读取后丢弃，避免反序列化直接失败。
        枚举_主信息类型 legacyType{};
        std::uint64_t legacyHits = 0;
        std::int64_t legacyTrust = 0;
        std::uint8_t tag = 0;

        if (!读枚举(is, legacyType)) { delete out; return nullptr; }
        if (!读POD(is, legacyHits)) { delete out; return nullptr; }
        if (!读POD(is, legacyTrust)) { delete out; return nullptr; }
        if (!读POD(is, tag)) { delete out; return nullptr; }

        out->命中次数 = legacyHits;
        out->内容哈希 = 0;
        out->最后命中 = 0;

        switch (tag)
        {
        case 0:
            break;
        case 1: {
            std::int64_t v{};
            if (!读POD(is, v)) { delete out; return nullptr; }
            break;
        }
        case 2: {
            std::uint64_t v{};
            if (!读POD(is, v)) { delete out; return nullptr; }
            break;
        }
        case 3: {
            std::uint32_t n{};
            if (!读POD(is, n)) { delete out; return nullptr; }
            for (std::uint32_t i = 0; i < n; ++i) {
                std::int64_t item{};
                if (!读POD(is, item)) { delete out; return nullptr; }
            }
            break;
        }
        case 4: {
            std::uint32_t n{};
            if (!读POD(is, n)) { delete out; return nullptr; }
            out->值.resize(n);
            for (std::uint32_t i = 0; i < n; ++i) {
                if (!读POD(is, out->值[i])) { delete out; return nullptr; }
            }
            break;
        }
        case 5: {
            std::string s;
            if (!读字符串(is, s)) { delete out; return nullptr; }
            break;
        }
        default:
            delete out;
            return nullptr;
        }

        return out;
    }

    delete out;
    return nullptr;
}

// ----------------------- 语素基类 -----------------------
inline void 语素基类::序列化(std::ostream& os) const
{
    using namespace 主信息定义模块_存档_detail;
    const std::uint16_t ver = 1;
    写POD(os, ver);

    写枚举(os, 主信息类型);

    switch (主信息类型)
    {
    case 枚举_主信息类型::词: {
        auto* p = dynamic_cast<const 词主信息类*>(this);
        写字符串(os, p ? p->词 : std::string{});
        break;
    }
    case 枚举_主信息类型::词性: {
        auto* p = dynamic_cast<const 词性主信息类*>(this);
        写枚举(os, p ? p->词性 : 枚举_词性::未定义);
        // 对应基础信息：存主键（加载后外部修复）
        写字符串(os, p ? 节点主键(p->对应基础信息指针) : std::string{});
        break;
    }
    case 枚举_主信息类型::短语: {
        auto* p = dynamic_cast<const 短语主信息类*>(this);
        写字符串(os, p ? 节点主键(p->参照词) : std::string{});
        写字符串(os, p ? 节点主键(p->比较对象词) : std::string{});
        break;
    }
    case 枚举_主信息类型::短语子节点: {
        auto* p = dynamic_cast<const 短语子节点主信息类*>(this);
        写字符串(os, p ? 节点主键(p->比较词) : std::string{});
        写字符串(os, p ? 节点主键(p->结果词) : std::string{});
        break;
    }
    default:
        // 未知语素：无内容
        break;
    }
}

inline 语素基类* 语素基类::反序列化(std::istream& is)
{
    using namespace 主信息定义模块_存档_detail;
    std::uint16_t ver = 0;
    if (!读POD(is, ver)) return nullptr;
    if (ver != 1) return nullptr;

    枚举_主信息类型 tp = 枚举_主信息类型::未定义;
    if (!读枚举(is, tp)) return nullptr;

    switch (tp)
    {
    case 枚举_主信息类型::词: {
        std::string w;
        if (!读字符串(is, w)) return nullptr;
        return new 词主信息类(std::move(w));
    }
    case 枚举_主信息类型::词性: {
        枚举_词性 pos = 枚举_词性::未定义;
        if (!读枚举(is, pos)) return nullptr;
        std::string _对应基础信息主键;
        if (!读字符串(is, _对应基础信息主键)) return nullptr;
        // 此处无法解析主键->指针，先置空
        auto* out = new 词性主信息类(pos);
        out->对应基础信息指针 = nullptr;
        return out;
    }
    case 枚举_主信息类型::短语: {
        std::string _k1, _k2;
        if (!读字符串(is, _k1)) return nullptr;
        if (!读字符串(is, _k2)) return nullptr;
        // const 指针成员无法后补，先置空
        return new 短语主信息类(nullptr, nullptr);
    }
    case 枚举_主信息类型::短语子节点: {
        std::string _k1, _k2;
        if (!读字符串(is, _k1)) return nullptr;
        if (!读字符串(is, _k2)) return nullptr;
        return new 短语子节点主信息类(nullptr, nullptr);
    }
    default:
        return nullptr;
    }
}

// ----------------------- 基础信息基类 -----------------------
inline void 基础信息基类::序列化(std::ostream& os) const
{
    using namespace 主信息定义模块_存档_detail;
    // ver=2：二次特征主信息的存档格式升级（加入 左值/右值/比较结果 + 结果区间 + 指针主键缓存）
    // ver=3：因果主信息升级为规范化存档（实例/模板/链）
    // ver=4：因果实例补充动作语义键存档
    // ver=5：因果实例补充主结果状态/主结果动态存档
    // ver=6：场景/存在补充实例因果列表，因果模板补充主结果特征与证据引用
    // ver=7：抽象因果节点瘦身，改为统计证据 + 引用证据 + 摘要证据
    // ver=8：场景/存在补充因果链列表
    const std::uint16_t ver = 8;
    写POD(os, ver);

    // 运行时类型标签
    枚举_主信息类型 tag = 枚举_主信息类型::未定义;
    if (dynamic_cast<const 场景节点主信息类*>(this)) tag = 枚举_主信息类型::场景;
    else if (dynamic_cast<const 存在节点主信息类*>(this)) tag = 枚举_主信息类型::存在;
    else if (dynamic_cast<const 特征节点主信息类*>(this)) tag = 枚举_主信息类型::特征;
    else if (dynamic_cast<const 指代节点主信息类*>(this)) tag = 枚举_主信息类型::指代;
    else if (dynamic_cast<const 状态节点主信息类*>(this)) tag = 枚举_主信息类型::状态;
    else if (dynamic_cast<const 动态节点主信息类*>(this)) tag = 枚举_主信息类型::动态;
    else if (dynamic_cast<const 二次特征主信息类*>(this)) tag = 枚举_主信息类型::二次特征_修饰;
    else if (dynamic_cast<const 因果实例主信息类*>(this)
        || dynamic_cast<const 因果模板主信息类*>(this)
        || dynamic_cast<const 因果链主信息类*>(this)) tag = 枚举_主信息类型::因果;

    写枚举(os, tag);

    // 通用字段：名称/类型 的“主键缓存” + 命中次数
    const std::string dummy_name_key = "dummy";
    const std::string dummy_type_key = "dummy";
    写字符串(os, dummy_name_key);
    写字符串(os, dummy_type_key);
    写POD(os, 命中次数);

    auto 写字符串列表 = [&](const std::vector<std::string>& 列表) {
        const std::uint32_t n = static_cast<std::uint32_t>(列表.size());
        写POD(os, n);
        for (const auto& s : 列表) 写字符串(os, s);
    };

    auto 编码运行期主键 = [](std::uint64_t 主键) -> std::string {
        return 主键 == 0 ? std::string{} : (std::string{"hash:"} + std::to_string(主键));
    };

    auto 节点主键或缓存 = [&](const auto* 节点, const std::string& 存档键, std::uint64_t 运行期主键 = 0ull) -> std::string {
        if (节点) return 节点主键(节点);
        if (!存档键.empty()) return 存档键;
        return 编码运行期主键(运行期主键);
    };

    auto 节点列表主键或缓存 = [&](const auto& 节点列表, const std::vector<std::string>& 存档列表) -> std::vector<std::string> {
        std::vector<std::string> out{};
        if (!节点列表.empty()) {
            out.reserve(节点列表.size());
            for (auto* 节点 : 节点列表) {
                if (节点) out.push_back(节点主键(节点));
            }
        }
        if (out.empty()) out = 存档列表;
        return out;
    };

    // 派生字段
    switch (tag)
    {
    case 枚举_主信息类型::指代: {
        auto* p = dynamic_cast<const 指代节点主信息类*>(this);
        const std::string k1 = p ? (p->代词 ? 节点主键(p->代词) : p->代词主键_存档) : std::string{};
        const std::string k2 = p ? (p->指代对象 ? 节点主键(p->指代对象) : p->指代对象主键_存档) : std::string{};
        写字符串(os, k1);
        写字符串(os, k2);
        break;
    }
    case 枚举_主信息类型::特征: {
        auto* p = dynamic_cast<const 特征节点主信息类*>(this);
        const std::uint64_t 命中 = p ? p->命中次数 : 0;
        写POD(os, 命中);

        const std::uint64_t c = p ? p->可信度 : 10000;
        写POD(os, c);

        写枚举(os, p ? p->区间语义 : 枚举_区间语义::未定义);
        // 区间摘要
        const bool has = p ? p->区间摘要.has_value() : false;
        const std::uint8_t b = has ? 1u : 0u;
        写POD(os, b);
        if (has) {
            const auto& interval = *p->区间摘要;
            const std::uint32_t dims = (std::uint32_t)interval.维度数();
            写POD(os, dims);
            for (const auto& seg : interval.维度) {
                写POD(os, seg.低值);
                写POD(os, seg.高值);
            }
        }

        写POD(os, p ? p->当前稳态索引 : -1);

        // 当前值/值列表：只存主键
        const std::string curK = p ? (p->当前值 ? 节点主键(p->当前值) : p->当前值->获取主键()) : std::string{};
        写字符串(os, curK);
        std::vector<std::string> keys;
        if (p) {
                keys.reserve(p->值列表.size());
                for (auto* v : p->值列表) keys.push_back(v->获取主键());

        }
        const std::uint32_t n = (std::uint32_t)keys.size();
        写POD(os, n);
        for (auto& s : keys) 写字符串(os, s);
        break;
    }
    case 枚举_主信息类型::存在: {
        auto* p = dynamic_cast<const 存在节点主信息类*>(this);
        // 指向其他节点的指针只存主键
        写字符串(os, p ? 节点主键或缓存(p->概念模板, p->概念模板主键_存档) : std::string{});
        写字符串(os, p ? 节点主键或缓存(p->内部世界, p->内部世界主键_存档) : std::string{});

        写POD(os, p ? p->创建时间 : 0);
        写POD(os, p ? p->最后观测时间 : 0);
        写POD(os, p ? p->命中次数 : 0u);
        写POD(os, p ? p->可信度 : 10000ll);
        写字符串列表(p ? 节点列表主键或缓存(p->实例因果列表, p->实例因果主键列表_存档) : std::vector<std::string>{});
        写字符串列表(p ? 节点列表主键或缓存(p->因果链列表, p->因果链主键列表_存档) : std::vector<std::string>{});
        // 其他统计/历史较大，这里先不做全量存档（可按需扩展）
        break;
    }
    case 枚举_主信息类型::场景: {
        auto* p = dynamic_cast<const 场景节点主信息类*>(this);
        写字符串(os, p ? 节点主键或缓存(p->宿主存在, p->宿主存在主键_存档) : std::string{});
        写枚举(os, p ? p->世界类型 : 枚举_世界类型::现实世界);
        写字符串(os, p ? 节点主键或缓存(p->来源文章, p->来源文章主键_存档) : std::string{});

        写POD(os, p ? p->创建时间 : 0);
        写POD(os, p ? p->最后观测时间 : 0);
        写POD(os, p ? p->命中次数 : 0u);
        写字符串列表(p ? 节点列表主键或缓存(p->实例因果列表, p->实例因果主键列表_存档) : std::vector<std::string>{});
        写字符串列表(p ? 节点列表主键或缓存(p->因果链列表, p->因果链主键列表_存档) : std::vector<std::string>{});
        break;
    }
    case 枚举_主信息类型::状态: {
        auto* p = dynamic_cast<const 状态节点主信息类*>(this);
        写POD(os, p ? p->收到时间 : 0);
        写POD(os, p ? p->发生时间 : 0);
        写字符串(os, p ? 节点主键(p->对应信息节点) : std::string{});
        const std::uint8_t b = (p && p->是否变化) ? 1u : 0u;
        写POD(os, b);
        写字符串(os, p ? p->变化原因类别 : std::string{});
        写字符串(os, p ? p->变化原因说明 : std::string{});
        break;
    }
    case 枚举_主信息类型::动态: {
        auto* p = dynamic_cast<const 动态节点主信息类*>(this);
        写字符串(os, p ? 节点主键(p->初始状态) : std::string{});
        写字符串(os, p ? 节点主键(p->结果状态) : std::string{});
        break;
    }
    case 枚举_主信息类型::二次特征_修饰: {
        auto* p = dynamic_cast<const 二次特征主信息类*>(this);
        写枚举(os, p ? p->种类 : 枚举_二次特征种类::未定义);
        // 概念模板(schema)：用差值区间定义概念边界；实例(state)：用差值落地。
        写POD(os, p ? p->差值区间.低值 : 0);
        写POD(os, p ? p->差值区间.高值 : 0);

        // 指针字段存“主键缓存”：指针存在则用节点主键，否则用 *_主键_存档
    //    const std::string kTmpl = p ? (p->概念模板 ? 节点主键(p->概念模板) : p->概念模板主键_存档) : std::string{};
        const std::string kScene = p ? (p->所属场景 ? 节点主键(p->所属场景) :p->所属场景->获取主键() ) : std::string{};
        const std::string kSub = p ? (p->主体 ? 节点主键(p->主体) : p->主体->获取主键()) : std::string{};
        const std::string kObj = p ? (p->客体 ? 节点主键(p->客体) : p->客体->获取主键()) : std::string{};
        const std::string kLObj = p ? (p->左对象 ? 节点主键(p->左对象) : p->左对象->获取主键()) : std::string{};
        const std::string kRObj = p ? (p->右对象 ? 节点主键(p->右对象) : p->右对象->获取主键()) : std::string{};

    //    写字符串(os, kTmpl);
        写字符串(os, kScene);
        写字符串(os, kSub);
        写字符串(os, kObj);
        写字符串(os, kLObj);
        写字符串(os, kRObj);

        // 核心量：左值/右值/差值（全部量化为 int64）
        写POD(os, p ? p->左值 : (std::int64_t)0);
        写POD(os, p ? p->右值 : (std::int64_t)0);
        写POD(os, p ? p->差值 : (std::int64_t)0);
        写POD(os, p ? p->左值区间.低值 : 0);
        写POD(os, p ? p->左值区间.高值 : -1);
        写POD(os, p ? p->右值区间.低值 : 0);
        写POD(os, p ? p->右值区间.高值 : -1);
        写POD(os, p ? p->差值区间.低值 : 0);
        写POD(os, p ? p->差值区间.高值 : -1);

        // 兼容占位：历史格式里这里写了一个 double（相似度）。
        // 现在二次特征以 int64 的 左值/右值/差值 为唯一认知入口，仍写入占位以保持存档格式稳定。
        写POD(os, p ? p->相似度 : 0.0);
        // 观察统计（简单存）
        if (p) {
            //      写POD(os, p->观察.样本数);
            //      写POD(os, p->观察.均值);
            //      写POD(os, p->观察.方差);
            //      写POD(os, p->观察.最小值);
            //      写POD(os, p->观察.最大值);
            //      写POD(os, p->观察.最后更新时间);
        }
        else {
            //      结构体_观察统计 tmp{};
            //      写POD(os, tmp.样本数);
            //      写POD(os, tmp.均值);
            //      写POD(os, tmp.方差);
            //      写POD(os, tmp.最小值);
            //      写POD(os, tmp.最大值);
            //      写POD(os, tmp.最后更新时间);
        }
        break;
    }
    case 枚举_主信息类型::因果: {
        枚举_因果结构类型 结构类型 = 枚举_因果结构类型::实例;
        if (auto* p = dynamic_cast<const 因果兼容主信息类*>(this)) {
            结构类型 = p->结构类型;
        }
        else if (dynamic_cast<const 因果模板主信息类*>(this)) {
            结构类型 = 枚举_因果结构类型::模板;
        }
        else if (dynamic_cast<const 因果链主信息类*>(this)) {
            结构类型 = 枚举_因果结构类型::链;
        }
        写枚举(os, 结构类型);

        switch (结构类型)
        {
        case 枚举_因果结构类型::模板: {
            auto* p = dynamic_cast<const 因果模板主信息类*>(this);
            写枚举(os, p ? p->适用锚点类型 : 枚举_因果锚点类型::未定义);
            写POD(os, p ? p->适用层级 : 0u);
            写POD(os, p ? p->适用状态面签名 : 0ull);
            写字符串列表(p ? 节点列表主键或缓存(p->条件模板列表, p->条件模板主键列表_存档) : std::vector<std::string>{});
            写字符串(os, p ? 节点主键或缓存(p->动作模板, p->动作模板主键_存档) : std::string{});
            写字符串列表(p ? 节点列表主键或缓存(p->结果模板列表, p->结果模板主键列表_存档) : std::vector<std::string>{});
            写字符串列表(p ? 节点列表主键或缓存(p->证据实例列表, p->证据实例主键列表_存档) : std::vector<std::string>{});
            写字符串列表(p ? 节点列表主键或缓存(p->代表证据列表, p->代表证据主键列表_存档) : std::vector<std::string>{});
            写字符串列表(p ? 节点列表主键或缓存(p->反例实例列表, p->反例主键列表_存档) : std::vector<std::string>{});
            写POD(os, p ? p->证据次数 : 0ull);
            写POD(os, p ? p->成立次数 : 0ull);
            写POD(os, p ? p->失败次数 : 0ull);
            写POD(os, p ? p->最近命中时间 : 0);
            写POD(os, p ? p->最近失败时间 : 0);
            写POD(os, p ? p->稳定度 : 0ll);
            写字符串(os, p ? p->典型条件方向摘要 : std::string{});
            写字符串(os, p ? p->典型动作语义摘要 : std::string{});
            写字符串(os, p ? p->典型结果方向摘要 : std::string{});
            写字符串(os, p ? p->最常见失败边界摘要 : std::string{});
            break;
        }
        case 枚举_因果结构类型::链: {
            auto* p = dynamic_cast<const 因果链主信息类*>(this);
            写字符串列表(p ? 节点列表主键或缓存(p->因果实例序列, p->因果实例主键序列_存档) : std::vector<std::string>{});
            写字符串(os, p ? (!p->起点锚点主键_存档.empty() ? p->起点锚点主键_存档 : 编码运行期主键(p->起点锚点主键)) : std::string{});
            写字符串(os, p ? (!p->终点锚点主键_存档.empty() ? p->终点锚点主键_存档 : 编码运行期主键(p->终点锚点主键)) : std::string{});
            写POD(os, p ? p->起始时间 : 0);
            写POD(os, p ? p->结束时间 : 0);
            break;
        }
        case 枚举_因果结构类型::未定义:
        case 枚举_因果结构类型::实例:
        default: {
            auto* p = dynamic_cast<const 因果实例主信息类*>(this);
            auto* pc = dynamic_cast<const 因果兼容主信息类*>(this);
            写枚举(os, p ? p->锚点类型 : 枚举_因果锚点类型::未定义);
            写字符串(os, p ? 节点主键或缓存(p->锚点信息, p->锚点主键_存档, p->锚点主键) : std::string{});
            写POD(os, p ? p->状态层级 : 0u);
            写POD(os, p ? p->状态面签名 : 0ull);
            写POD(os, p ? p->条件时间 : 0);
            写POD(os, p ? p->动作时间 : 0);
            写POD(os, p ? p->结果时间 : 0);
            写字符串列表(p ? 节点列表主键或缓存(p->条件状态列表, p->条件状态主键列表_存档) : std::vector<std::string>{});
            写字符串(os, p ? 节点主键或缓存(p->动作主体, p->动作主体主键_存档) : std::string{});
            写字符串(os, p ? 节点主键或缓存(p->动作语义, p->动作语义主键_存档) : std::string{});
            写字符串(os, p ? 节点主键或缓存(p->触发动态, p->触发动态主键_存档) : std::string{});
            写字符串列表(p ? 节点列表主键或缓存(p->结果状态列表, p->结果状态主键列表_存档) : std::vector<std::string>{});
            写字符串列表(p ? 节点列表主键或缓存(p->结果动态列表, p->结果动态主键列表_存档) : std::vector<std::string>{});
            写枚举(os, p ? p->来源类型 : 枚举_因果来源类型::未定义);
            写字符串(os, p ? p->来源方法主键_存档 : std::string{});
            写字符串(os, p ? p->来源任务主键_存档 : std::string{});
            写POD(os, p ? p->置信度 : 0ll);
            const std::uint8_t 已验证 = (p && p->已验证) ? 1u : 0u;
            写POD(os, 已验证);
            写字符串(os, pc ? pc->因果累计键_存档 : std::string{});
            写POD(os, pc ? pc->首次发生时间 : 0);
            写POD(os, pc ? pc->最近发生时间 : 0);
            写POD(os, pc ? pc->观察次数 : 0ll);
            写POD(os, pc ? pc->稳定度Q : 0ll);
            写POD(os, pc ? pc->安全归因Q : 0ll);
            写POD(os, pc ? pc->服务归因Q : 0ll);
            const std::uint8_t 是否原始观测实例 = (pc && pc->是否原始观测实例) ? 1u : 0u;
            写POD(os, 是否原始观测实例);
            写POD(os, pc ? pc->原始观测序号 : 0ll);
            写字符串(os, p ? p->动作语义键 : std::string{});
            写字符串(os, p ? 节点主键或缓存(p->主结果状态, p->主结果状态主键_存档) : std::string{});
            写字符串(os, p ? 节点主键或缓存(p->主结果动态, p->主结果动态主键_存档) : std::string{});
            break;
        }
        }
        break;
    }
    default:
        break;
    }
}

inline 基础信息基类* 基础信息基类::反序列化(std::istream& is)
{
    using namespace 主信息定义模块_存档_detail;
    std::uint16_t ver = 0;
    if (!读POD(is, ver)) return nullptr;
    if (ver != 1 && ver != 2 && ver != 3 && ver != 4 && ver != 5 && ver != 6 && ver != 7 && ver != 8) return nullptr;

    枚举_主信息类型 tag = 枚举_主信息类型::未定义;
    if (!读枚举(is, tag)) return nullptr;

    std::string 名称k, 类型k;
    std::uint64_t hit{};
    // 跳过不存在的字段：名称、类型、命中次数
    if (!读字符串(is, 名称k)) return nullptr;
    if (!读字符串(is, 类型k)) return nullptr;
    if (!读POD(is, hit)) return nullptr;

    auto 设置通用 = [&](基础信息基类* b) {
        // 这些字段在自然语言成分基类中不存在
         b->名称 = nullptr;
         b->类型 = nullptr;
         b->命中次数 = hit;
        (void)名称k; (void)类型k; (void)hit; // 暂不保存缓存字段
        };

    auto 读字符串列表 = [&](std::vector<std::string>& 列表) -> bool {
        std::uint32_t n = 0;
        if (!读POD(is, n)) return false;
        列表.clear();
        列表.reserve(n);
        for (std::uint32_t i = 0; i < n; ++i) {
            std::string s;
            if (!读字符串(is, s)) return false;
            列表.push_back(std::move(s));
        }
        return true;
    };

    auto 解析运行期主键 = [](const std::string& 主键文本) -> std::uint64_t {
        if (主键文本.empty()) return 0;
        if (主键文本.rfind("hash:", 0) == 0) {
            try {
                return static_cast<std::uint64_t>(std::stoull(主键文本.substr(5)));
            }
            catch (...) {
                return 0;
            }
        }
        return static_cast<std::uint64_t>(std::hash<std::string>{}(主键文本));
    };

    switch (tag)
    {
    case 枚举_主信息类型::指代: {
        std::string k1, k2;
        if (!读字符串(is, k1)) return nullptr;
        if (!读字符串(is, k2)) return nullptr;
        auto* out = new 指代节点主信息类();
        设置通用(out);
        out->代词 = nullptr;
        out->指代对象 = nullptr;
        out->代词主键_存档 = std::move(k1);
        out->指代对象主键_存档 = std::move(k2);
        return out;
    }
    case 枚举_主信息类型::特征: {
        std::uint32_t 命中2{};
        std::int64_t w{}, c{};
        枚举_区间语义 sem = 枚举_区间语义::未定义;
        std::uint8_t has{};
        if (!读POD(is, 命中2)) return nullptr;
        if (!读POD(is, w)) return nullptr;
        if (!读POD(is, c)) return nullptr;
        if (!读枚举(is, sem)) return nullptr;
        if (!读POD(is, has)) return nullptr;
        std::optional<I64多维区间> summary;
        if (has) {
            std::uint32_t dims{};
            if (!读POD(is, dims)) return nullptr;
            I64多维区间 v;
            v.维度.resize(dims);
            for (std::uint32_t i = 0; i < dims; ++i) {
                if (!读POD(is, v.维度[i].低值)) return nullptr;
                if (!读POD(is, v.维度[i].高值)) return nullptr;
            }
            summary = std::move(v);
        }
        std::int64_t idx{};
        if (!读POD(is, idx)) return nullptr;

        std::string curK;
        if (!读字符串(is, curK)) return nullptr;
        std::uint32_t n{};
        if (!读POD(is, n)) return nullptr;
        std::vector<std::string> keys;
        keys.reserve(n);
        for (std::uint32_t i = 0; i < n; ++i) {
            std::string s;
            if (!读字符串(is, s)) return nullptr;
            keys.push_back(std::move(s));
        }

        auto* out = new 特征节点主信息类();
        设置通用(out);
        out->命中次数 = 命中2;
        out->可信度 = c;
        out->区间语义 = sem;
        out->区间摘要 = std::move(summary);
        out->当前稳态索引 = idx;
        out->当前值 = nullptr;

        return out;
    }
    case 枚举_主信息类型::存在: {
        std::string kConcept, kInner;
        if (!读字符串(is, kConcept)) return nullptr;
        if (!读字符串(is, kInner)) return nullptr;
        时间戳 createT{}, lastT{};
        std::uint32_t hit{};
        std::int64_t credibility = 10000;
        if (!读POD(is, createT)) return nullptr;
        if (!读POD(is, lastT)) return nullptr;
        if (!读POD(is, hit)) return nullptr;
        if (!读POD(is, credibility)) return nullptr;
        std::vector<std::string> 实例因果主键列表{};
        if (ver >= 6) {
            std::uint32_t n = 0;
            if (!读POD(is, n)) return nullptr;
            实例因果主键列表.reserve(n);
            for (std::uint32_t i = 0; i < n; ++i) {
                std::string s{};
                if (!读字符串(is, s)) return nullptr;
                实例因果主键列表.push_back(std::move(s));
            }
        }
        std::vector<std::string> 因果链主键列表{};
        if (ver >= 8) {
            std::uint32_t n = 0;
            if (!读POD(is, n)) return nullptr;
            因果链主键列表.reserve(n);
            for (std::uint32_t i = 0; i < n; ++i) {
                std::string s{};
                if (!读字符串(is, s)) return nullptr;
                因果链主键列表.push_back(std::move(s));
            }
        }

        auto* out = new 存在节点主信息类();
        设置通用(out);
        out->概念模板 = nullptr;
        out->内部世界 = nullptr;
        out->概念模板主键_存档 = std::move(kConcept);
        out->内部世界主键_存档 = std::move(kInner);
        out->创建时间 = createT;
        out->最后观测时间 = lastT;
        out->命中次数 = hit;
        out->可信度 = credibility;
        out->实例因果主键列表_存档 = std::move(实例因果主键列表);
        out->因果链主键列表_存档 = std::move(因果链主键列表);
        return out;
    }
    case 枚举_主信息类型::场景: {
        std::string kHost;
        if (!读字符串(is, kHost)) return nullptr;
        枚举_世界类型 wt = 枚举_世界类型::现实世界;
        if (!读枚举(is, wt)) return nullptr;
        std::string kArticle;
        if (!读字符串(is, kArticle)) return nullptr;
        时间戳 createT{}, lastT{};
        std::uint32_t hit{};
        if (!读POD(is, createT)) return nullptr;
        if (!读POD(is, lastT)) return nullptr;
        if (!读POD(is, hit)) return nullptr;
        std::vector<std::string> 实例因果主键列表{};
        if (ver >= 6) {
            std::uint32_t n = 0;
            if (!读POD(is, n)) return nullptr;
            实例因果主键列表.reserve(n);
            for (std::uint32_t i = 0; i < n; ++i) {
                std::string s{};
                if (!读字符串(is, s)) return nullptr;
                实例因果主键列表.push_back(std::move(s));
            }
        }
        std::vector<std::string> 因果链主键列表{};
        if (ver >= 8) {
            std::uint32_t n = 0;
            if (!读POD(is, n)) return nullptr;
            因果链主键列表.reserve(n);
            for (std::uint32_t i = 0; i < n; ++i) {
                std::string s{};
                if (!读字符串(is, s)) return nullptr;
                因果链主键列表.push_back(std::move(s));
            }
        }

        auto* out = new 场景节点主信息类();
        设置通用(out);
        out->宿主存在 = nullptr;
        out->来源文章 = nullptr;
        out->宿主存在主键_存档 = std::move(kHost);
        out->来源文章主键_存档 = std::move(kArticle);
        out->世界类型 = wt;
        out->创建时间 = createT;
        out->最后观测时间 = lastT;
        out->命中次数 = hit;
        out->实例因果主键列表_存档 = std::move(实例因果主键列表);
        out->因果链主键列表_存档 = std::move(因果链主键列表);
        return out;
    }
    case 枚举_主信息类型::状态: {
        时间戳 t1{}, t2{};
        std::string k;
        std::uint8_t b{};
        std::string 原因类别;
        std::string 原因说明;
        if (!读POD(is, t1)) return nullptr;
        if (!读POD(is, t2)) return nullptr;
        if (!读字符串(is, k)) return nullptr;
        if (!读POD(is, b)) return nullptr;
        if (!读字符串(is, 原因类别)) return nullptr;
        if (!读字符串(is, 原因说明)) return nullptr;
        auto* out = new 状态节点主信息类();
        设置通用(out);
        out->收到时间 = t1;
        out->发生时间 = t2;
        out->对应信息节点 = nullptr;
        out->是否变化 = (b != 0);
        out->变化原因类别 = 原因类别;
        out->变化原因说明 = 原因说明;
        return out;
    }
    case 枚举_主信息类型::动态: {
        std::string k1, k2;
        if (!读字符串(is, k1)) return nullptr;
        if (!读字符串(is, k2)) return nullptr;
        auto* out = new 动态节点主信息类();
        设置通用(out);
        out->初始状态 = nullptr;
        out->结果状态 = nullptr;
        return out;
    }
    case 枚举_主信息类型::二次特征_修饰: {
        枚举_二次特征种类 kind = 枚举_二次特征种类::未定义;
        if (!读枚举(is, kind)) return nullptr;

        // ver=1：旧格式（枚举结果 + diff(double) + sim(double)）
        // ver=2：新格式（结果区间 + 左值/右值/比较结果(int64) + sim(double) + 主键缓存）
        if (ver == 1) {
            // 旧：结果枚举（这里不再使用，直接读出占位字节）
            std::uint8_t res_raw = 0;
            if (!读POD(is, res_raw)) return nullptr;

            std::string kTmpl, kScene, kSub, kObj, kLObj, kRObj;
            if (!读字符串(is, kTmpl)) return nullptr;
            if (!读字符串(is, kScene)) return nullptr;
            if (!读字符串(is, kSub)) return nullptr;
            if (!读字符串(is, kObj)) return nullptr;
            if (!读字符串(is, kLObj)) return nullptr;
            if (!读字符串(is, kRObj)) return nullptr;

            double diff_d{}, sim{};
            if (!读POD(is, diff_d)) return nullptr;
            if (!读POD(is, sim)) return nullptr;

            auto* out = new 二次特征主信息类();
            设置通用(out);
            out->种类 = kind;
            out->差值区间 = I64区间{};

         //   out->概念模板 = nullptr;
            out->所属场景 = nullptr;
            out->主体 = nullptr;
            out->客体 = nullptr;
            out->左对象 = nullptr;
            out->右对象 = nullptr;



            out->左值 = 0;
            out->右值 = 0;
            out->差值 = (std::int64_t)diff_d;
            (void)sim; // 历史占位字段
            (void)res_raw; // 旧字段不再使用
            return out;
        }

        // ver==2
        I64区间 差值区间{};
        if (!读POD(is, 差值区间.低值)) return nullptr;
        if (!读POD(is, 差值区间.高值)) return nullptr;

        std::string kTmpl, kScene, kSub, kObj, kLObj, kRObj;
        if (!读字符串(is, kTmpl)) return nullptr;
        if (!读字符串(is, kScene)) return nullptr;
        if (!读字符串(is, kSub)) return nullptr;
        if (!读字符串(is, kObj)) return nullptr;
        if (!读字符串(is, kLObj)) return nullptr;
        if (!读字符串(is, kRObj)) return nullptr;

        std::int64_t lv{}, rv{}, diff{};
        if (!读POD(is, lv)) return nullptr;
        if (!读POD(is, rv)) return nullptr;
        if (!读POD(is, diff)) return nullptr;

        // 读取区间信息
        I64区间 左值区间{}, 右值区间{};
        if (!读POD(is, 左值区间.低值)) return nullptr;
        if (!读POD(is, 左值区间.高值)) return nullptr;
        if (!读POD(is, 右值区间.低值)) return nullptr;
        if (!读POD(is, 右值区间.高值)) return nullptr;
        if (!读POD(is, 差值区间.低值)) return nullptr;
        if (!读POD(is, 差值区间.高值)) return nullptr;

        std::int64_t sim{};
        if (!读POD(is, sim)) return nullptr;

        auto* out = new 二次特征主信息类();
        设置通用(out);
        out->种类 = kind;

  //      out->概念模板 = nullptr;
        out->所属场景 = nullptr;
        out->主体 = nullptr;
        out->客体 = nullptr;
        out->左对象 = nullptr;
        out->右对象 = nullptr;



        out->左值 = lv;
        out->右值 = rv;
        out->差值 = diff;
        out->左值区间 = 左值区间;
        out->右值区间 = 右值区间;
        out->差值区间 = 差值区间;
        out->相似度 = sim;
        return out;
    }
    case 枚举_主信息类型::因果: {
        if (ver < 3) {
            auto* out = new 因果主信息类();
            设置通用(out);
            return out;
        }

        枚举_因果结构类型 kind = 枚举_因果结构类型::实例;
        if (!读枚举(is, kind)) return nullptr;

        switch (kind)
        {
        case 枚举_因果结构类型::模板: {
            auto* out = new 因果模板主信息类();
            设置通用(out);
            if (!读枚举(is, out->适用锚点类型)) { delete out; return nullptr; }
            if (!读POD(is, out->适用层级)) { delete out; return nullptr; }
            if (!读POD(is, out->适用状态面签名)) { delete out; return nullptr; }
            std::string 旧版主结果特征主键{};
            std::string 旧版抽象键{};
            if (ver >= 6 && ver < 7) {
                if (!读字符串(is, 旧版主结果特征主键)) { delete out; return nullptr; }
            }
            if (!读字符串列表(out->条件模板主键列表_存档)) { delete out; return nullptr; }
            if (!读字符串(is, out->动作模板主键_存档)) { delete out; return nullptr; }
            if (!读字符串列表(out->结果模板主键列表_存档)) { delete out; return nullptr; }
            if (ver >= 6) {
                if (!读字符串列表(out->证据实例主键列表_存档)) { delete out; return nullptr; }
            }
            if (ver >= 7) {
                if (!读字符串列表(out->代表证据主键列表_存档)) { delete out; return nullptr; }
                if (!读字符串列表(out->反例主键列表_存档)) { delete out; return nullptr; }
            }
            else {
                out->代表证据主键列表_存档 = out->证据实例主键列表_存档;
            }
            if (ver >= 6 && ver < 7) {
                if (!读字符串(is, 旧版抽象键)) { delete out; return nullptr; }
            }
            if (!读POD(is, out->证据次数)) { delete out; return nullptr; }
            if (!读POD(is, out->成立次数)) { delete out; return nullptr; }
            if (!读POD(is, out->失败次数)) { delete out; return nullptr; }
            if (!读POD(is, out->最近命中时间)) { delete out; return nullptr; }
            if (ver >= 7) {
                if (!读POD(is, out->最近失败时间)) { delete out; return nullptr; }
            }
            if (!读POD(is, out->稳定度)) { delete out; return nullptr; }
            if (ver >= 7) {
                if (!读字符串(is, out->典型条件方向摘要)) { delete out; return nullptr; }
                if (!读字符串(is, out->典型动作语义摘要)) { delete out; return nullptr; }
                if (!读字符串(is, out->典型结果方向摘要)) { delete out; return nullptr; }
                if (!读字符串(is, out->最常见失败边界摘要)) { delete out; return nullptr; }
            }
            (void)旧版主结果特征主键;
            (void)旧版抽象键;
            out->动作模板 = nullptr;
            return out;
        }
        case 枚举_因果结构类型::链: {
            auto* out = new 因果链主信息类();
            设置通用(out);
            if (!读字符串列表(out->因果实例主键序列_存档)) { delete out; return nullptr; }
            if (!读字符串(is, out->起点锚点主键_存档)) { delete out; return nullptr; }
            if (!读字符串(is, out->终点锚点主键_存档)) { delete out; return nullptr; }
            if (!读POD(is, out->起始时间)) { delete out; return nullptr; }
            if (!读POD(is, out->结束时间)) { delete out; return nullptr; }
            out->起点锚点主键 = 解析运行期主键(out->起点锚点主键_存档);
            out->终点锚点主键 = 解析运行期主键(out->终点锚点主键_存档);
            return out;
        }
        case 枚举_因果结构类型::未定义:
        case 枚举_因果结构类型::实例:
        default: {
            auto* out = new 因果主信息类();
            设置通用(out);
            out->结构类型 = (kind == 枚举_因果结构类型::未定义)
                ? 枚举_因果结构类型::实例
                : kind;
            if (!读枚举(is, out->锚点类型)) { delete out; return nullptr; }
            if (!读字符串(is, out->锚点主键_存档)) { delete out; return nullptr; }
            if (!读POD(is, out->状态层级)) { delete out; return nullptr; }
            if (!读POD(is, out->状态面签名)) { delete out; return nullptr; }
            if (!读POD(is, out->条件时间)) { delete out; return nullptr; }
            if (!读POD(is, out->动作时间)) { delete out; return nullptr; }
            if (!读POD(is, out->结果时间)) { delete out; return nullptr; }
            if (!读字符串列表(out->条件状态主键列表_存档)) { delete out; return nullptr; }
            if (!读字符串(is, out->动作主体主键_存档)) { delete out; return nullptr; }
            if (!读字符串(is, out->动作语义主键_存档)) { delete out; return nullptr; }
            if (!读字符串(is, out->触发动态主键_存档)) { delete out; return nullptr; }
            if (!读字符串列表(out->结果状态主键列表_存档)) { delete out; return nullptr; }
            if (!读字符串列表(out->结果动态主键列表_存档)) { delete out; return nullptr; }
            if (!读枚举(is, out->来源类型)) { delete out; return nullptr; }
            if (!读字符串(is, out->来源方法主键_存档)) { delete out; return nullptr; }
            if (!读字符串(is, out->来源任务主键_存档)) { delete out; return nullptr; }
            if (!读POD(is, out->置信度)) { delete out; return nullptr; }
            std::uint8_t 已验证 = 0;
            if (!读POD(is, 已验证)) { delete out; return nullptr; }
            if (!读字符串(is, out->因果累计键_存档)) { delete out; return nullptr; }
            if (!读POD(is, out->首次发生时间)) { delete out; return nullptr; }
            if (!读POD(is, out->最近发生时间)) { delete out; return nullptr; }
            if (!读POD(is, out->观察次数)) { delete out; return nullptr; }
            if (!读POD(is, out->稳定度Q)) { delete out; return nullptr; }
            if (!读POD(is, out->安全归因Q)) { delete out; return nullptr; }
            if (!读POD(is, out->服务归因Q)) { delete out; return nullptr; }
            std::uint8_t 是否原始观测实例 = 0;
            if (!读POD(is, 是否原始观测实例)) { delete out; return nullptr; }
            if (!读POD(is, out->原始观测序号)) { delete out; return nullptr; }
            if (ver >= 4) {
                if (!读字符串(is, out->动作语义键)) { delete out; return nullptr; }
            }
            if (ver >= 5) {
                if (!读字符串(is, out->主结果状态主键_存档)) { delete out; return nullptr; }
                if (!读字符串(is, out->主结果动态主键_存档)) { delete out; return nullptr; }
            }

            out->锚点主键 = 解析运行期主键(out->锚点主键_存档);
            out->来源方法主键 = 解析运行期主键(out->来源方法主键_存档);
            out->来源任务主键 = 解析运行期主键(out->来源任务主键_存档);
            out->已验证 = (已验证 != 0);
            out->是否原始观测实例 = (是否原始观测实例 != 0);
            out->锚点信息 = nullptr;
            out->动作主体 = nullptr;
            out->动作语义 = nullptr;
            out->动作信息 = nullptr;
            out->动作动态 = nullptr;
            out->触发动态 = nullptr;
            out->主结果状态 = nullptr;
            out->主结果动态 = nullptr;
            out->归纳因果节点 = nullptr;
            return out;
        }
        }
    }
    default:
        return nullptr;
    }
}

// ----------------------- 需求主信息类 -----------------------
inline void 需求主信息类::序列化(std::ostream& os) const
{
    using namespace 主信息定义模块_存档_detail;
    const std::uint16_t ver = 1;
    写POD(os, ver);

    // 仅保存最核心的统计字段 + 指针主键（不做解析）
    写POD(os, 命中次数);
    写POD(os, 需求生成时间);
    写POD(os, 需求有效截止);
    写POD(os, 权重);

    写字符串(os, 节点主键(描述信息));
    写字符串(os, 节点主键(类型));
    写字符串(os, 节点主键(需求主体));
    写字符串(os, 节点主键(被需求存在));
    写字符串(os, 节点主键(被需求状态));
}

inline 需求主信息类* 需求主信息类::反序列化(std::istream& is)
{
    using namespace 主信息定义模块_存档_detail;
    std::uint16_t ver = 0;
    if (!读POD(is, ver)) return nullptr;
    if (ver != 1) return nullptr;

    auto* out = new 需求主信息类();
    if (!读POD(is, out->命中次数)) { delete out; return nullptr; }
    if (!读POD(is, out->需求生成时间)) { delete out; return nullptr; }
    if (!读POD(is, out->需求有效截止)) { delete out; return nullptr; }
    if (!读POD(is, out->权重)) { delete out; return nullptr; }

    // 读回主键但不解析
    std::string tmp;
    for (int i = 0; i < 5; ++i) {
        if (!读字符串(is, tmp)) { delete out; return nullptr; }
    }
    out->描述信息 = nullptr;
    out->类型 = nullptr;
    out->需求主体 = nullptr;
    out->被需求存在 = nullptr;
    out->被需求状态 = nullptr;
    return out;
}

// ----------------------- 方法信息基类 -----------------------
inline void 方法信息基类::序列化(std::ostream& os) const
{
    using namespace 主信息定义模块_存档_detail;
    const std::uint16_t ver = 1;
    写POD(os, ver);

    // kind: 1=首 2=条件 3=结果 0=未知
    std::uint8_t kind = 0;
    if (dynamic_cast<const 方法首节点主信息类*>(this)) kind = 1;
    else if (dynamic_cast<const 方法条件节点主信息类*>(this)) kind = 2;
    else if (dynamic_cast<const 方法结果节点主信息类*>(this)) kind = 3;

    写POD(os, kind);

    写POD(os, 命中次数);
    写POD(os, 最后更新时间);
    写枚举(os, 学习状态);
    写字符串(os, 节点主键(描述信息));

    if (auto* p = dynamic_cast<const 方法首节点主信息类*>(this)) {
        写字符串(os, 节点主键(p->动作名));
        写POD(os, p->实现ID);
        写枚举(os, p->来源);
        // 用途索引表（只存数量，避免膨胀；你可按需扩展为保存 key 列表）

        return;
    }
    if (auto* p = dynamic_cast<const 方法条件节点主信息类*>(this)) {
        写POD(os, p->时间起);
        写POD(os, p->时间止);
        写POD(os, p->不同结果数);
        写字符串(os, p->条件场景模板主键);
        return;
    }
    if (auto* p = dynamic_cast<const 方法结果节点主信息类*>(this)) {
        写字符串(os, p->Episode文档主键);
        return;
    }
}

inline 方法信息基类* 方法信息基类::反序列化(std::istream& is)
{
    using namespace 主信息定义模块_存档_detail;
    std::uint16_t ver = 0;
    if (!读POD(is, ver)) return nullptr;
    if (ver != 1) return nullptr;

    std::uint8_t kind = 0;
    if (!读POD(is, kind)) return nullptr;

    auto* base = (方法信息基类*)nullptr;

    if (kind == 1) base = new 方法首节点主信息类();
    else if (kind == 2) base = new 方法条件节点主信息类();
    else if (kind == 3) base = new 方法结果节点主信息类();
    else base = new 方法信息基类();

    if (!读POD(is, base->命中次数)) { delete base; return nullptr; }
    if (!读POD(is, base->最后更新时间)) { delete base; return nullptr; }
    if (!读枚举(is, base->学习状态)) { delete base; return nullptr; }
    std::string _descKey;
    if (!读字符串(is, _descKey)) { delete base; return nullptr; }
    base->描述信息 = nullptr;

    if (auto* p = dynamic_cast<方法首节点主信息类*>(base)) {
        std::string _actionKey;
        if (!读字符串(is, _actionKey)) { delete base; return nullptr; }
        if (!读POD(is, p->实现ID)) { delete base; return nullptr; }
        if (!读枚举(is, p->来源)) { delete base; return nullptr; }
        std::uint32_t n{};
        if (!读POD(is, n)) { delete base; return nullptr; }
        // 不解析动作名/用途索引表
        p->动作名 = nullptr;

        return base;
    }
    if (auto* p = dynamic_cast<方法条件节点主信息类*>(base)) {
        if (!读POD(is, p->时间起)) { delete base; return nullptr; }
        if (!读POD(is, p->时间止)) { delete base; return nullptr; }
        if (!读POD(is, p->不同结果数)) { delete base; return nullptr; }
        if (!读字符串(is, p->条件场景模板主键)) { delete base; return nullptr; }
        return base;
    }
    if (auto* p = dynamic_cast<方法结果节点主信息类*>(base)) {
        if (!读字符串(is, p->Episode文档主键)) { delete base; return nullptr; }
        return base;
    }

    return base;
}

// ----------------------- 自然语言成分基类 -----------------------
inline void 自然语言成分基类::序列化(std::ostream& os) const
{
    using namespace 主信息定义模块_存档_detail;
    const std::uint16_t ver = 1;
    写POD(os, ver);

    // kind: 1=文章 2=段落 3=自然句 4=子句 5=句子成分
    std::uint8_t kind = 0;
    if (dynamic_cast<const 文章主信息类*>(this)) kind = 1;
    else if (dynamic_cast<const 段落主信息类*>(this)) kind = 2;
    else if (dynamic_cast<const 自然句主信息类*>(this)) kind = 3;
    else if (dynamic_cast<const 子句主信息类*>(this)) kind = 4;
    else if (dynamic_cast<const 句子成分主信息类*>(this)) kind = 5;

    写POD(os, kind);

    // 通用字段（名称/类型）仅存主键
    // 跳过不存在的字段：名称、类型、命中次数
    std::string dummy_name_key, dummy_type_key;
    写字符串(os, dummy_name_key);
    写字符串(os, dummy_type_key);
    std::uint64_t dummy_hit_count = 0;
    写POD(os, dummy_hit_count);

    if (auto* p = dynamic_cast<const 句子成分主信息类*>(this)) {
        // 跳过不存在的字段：句子成分类型
        枚举_句子成分类型 dummy_sentence_component_type = 枚举_句子成分类型::其它;
        写枚举(os, dummy_sentence_component_type);
        写字符串(os, 节点主键(p->对应词性节点));
        写POD(os, p->在句子中的位置);
        return;
    }
    if (auto* p = dynamic_cast<const 文章主信息类*>(this)) {
        写字符串(os, p->标题);
        写字符串(os, p->原始文本);
        写字符串(os, p->作者);
        写字符串(os, p->来源);
        写字符串(os, 节点主键(p->对应文本世界场景));
        return;
    }
    if (auto* p = dynamic_cast<const 段落主信息类*>(this)) {
        写POD(os, p->在文章中的序号);
        写字符串(os, p->原始文本);
        return;
    }
    if (auto* p = dynamic_cast<const 自然句主信息类*>(this)) {
        写POD(os, p->在段落中的序号);
        // 词序列（主键）
        const std::uint32_t n = static_cast<std::uint32_t>(std::min(static_cast<std::size_t>(std::numeric_limits<std::uint32_t>::max()), p->词序列.size()));
        写POD(os, n);
        for (auto* w : p->词序列) 写字符串(os, 节点主键(w));
        写字符串(os, p->原始文本);
        写字符串(os, 节点主键(p->主语));
        写字符串(os, 节点主键(p->谓语));
        写字符串(os, 节点主键(p->宾语));
        return;
    }
    if (auto* p = dynamic_cast<const 子句主信息类*>(this)) {
        写枚举(os, p->原子结构类型);
        写POD(os, p->起始词索引);
        写POD(os, p->结束词索引);
        // 局部词序列
        const std::uint32_t n = static_cast<std::uint32_t>(std::min(static_cast<std::size_t>(std::numeric_limits<std::uint32_t>::max()), p->局部词序列.size()));
        写POD(os, n);
        for (auto* w : p->局部词序列) 写字符串(os, 节点主键(w));

        写字符串(os, 节点主键(p->子句主语));
        写字符串(os, 节点主键(p->子句谓语));
        写字符串(os, 节点主键(p->子句宾语));

        写字符串(os, 节点主键(p->对应场景));
        写字符串(os, 节点主键(p->对应状态));
        写字符串(os, 节点主键(p->对应动态));
        写字符串(os, 节点主键(p->对应因果));
        return;
    }
}

inline 自然语言成分基类* 自然语言成分基类::反序列化(std::istream& is)
{
    using namespace 主信息定义模块_存档_detail;
    std::uint16_t ver = 0;
    if (!读POD(is, ver)) return nullptr;
    if (ver != 1) return nullptr;

    std::uint8_t kind = 0;
    if (!读POD(is, kind)) return nullptr;

    // 通用字段
    std::string 名称k, 类型k;
    std::uint64_t hit{};
    // 跳过不存在的字段：名称、类型、命中次数
    if (!读字符串(is, 名称k)) return nullptr;
    if (!读字符串(is, 类型k)) return nullptr;
    if (!读POD(is, hit)) return nullptr;

    auto 设置通用 = [&](自然语言成分基类* b) {
        // 这些字段在自然语言成分基类中不存在
        // b->名称 = nullptr;
        // b->类型 = nullptr;
        // b->命中次数 = hit;
        (void)名称k; (void)类型k; (void)hit; // 暂不保存缓存字段
        };

    switch (kind)
    {
    case 5: {
        auto* out = new 句子成分主信息类();
        设置通用(out);
        // 跳过不存在的字段：句子成分类型
        枚举_句子成分类型 dummy_sentence_component_type;
        if (!读枚举(is, dummy_sentence_component_type)) { delete out; return nullptr; }
        std::string _k;
        if (!读字符串(is, _k)) { delete out; return nullptr; }
        out->对应词性节点 = nullptr;
        if (!读POD(is, out->在句子中的位置)) { delete out; return nullptr; }
        return out;
    }
    case 1: {
        auto* out = new 文章主信息类();
        设置通用(out);
        if (!读字符串(is, out->标题)) { delete out; return nullptr; }
        if (!读字符串(is, out->原始文本)) { delete out; return nullptr; }
        if (!读字符串(is, out->作者)) { delete out; return nullptr; }
        if (!读字符串(is, out->来源)) { delete out; return nullptr; }
        std::string _k;
        if (!读字符串(is, _k)) { delete out; return nullptr; }
        out->对应文本世界场景 = nullptr;
        return out;
    }
    case 2: {
        auto* out = new 段落主信息类();
        设置通用(out);
        if (!读POD(is, out->在文章中的序号)) { delete out; return nullptr; }
        if (!读字符串(is, out->原始文本)) { delete out; return nullptr; }
        return out;
    }
    case 3: {
        auto* out = new 自然句主信息类();
        设置通用(out);
        if (!读POD(is, out->在段落中的序号)) { delete out; return nullptr; }
        std::uint32_t n{};
        if (!读POD(is, n)) { delete out; return nullptr; }
        out->词序列.clear();
        out->词序列.reserve(n);
        for (std::uint32_t i = 0; i < n; ++i) {
            std::string _k;
            if (!读字符串(is, _k)) { delete out; return nullptr; }
            out->词序列.push_back(nullptr);
        }
        if (!读字符串(is, out->原始文本)) { delete out; return nullptr; }
        std::string _k;
        if (!读字符串(is, _k)) { delete out; return nullptr; }
        if (!读字符串(is, _k)) { delete out; return nullptr; }
        if (!读字符串(is, _k)) { delete out; return nullptr; }
        out->主语 = nullptr; out->谓语 = nullptr; out->宾语 = nullptr;
        return out;
    }
    case 4: {
        auto* out = new 子句主信息类();
        设置通用(out);
        if (!读枚举(is, out->原子结构类型)) { delete out; return nullptr; }
        if (!读POD(is, out->起始词索引)) { delete out; return nullptr; }
        if (!读POD(is, out->结束词索引)) { delete out; return nullptr; }
        std::uint32_t n{};
        if (!读POD(is, n)) { delete out; return nullptr; }
        out->局部词序列.clear();
        out->局部词序列.reserve(n);
        for (std::uint32_t i = 0; i < n; ++i) {
            std::string _k;
            if (!读字符串(is, _k)) { delete out; return nullptr; }
            out->局部词序列.push_back(nullptr);
        }
        std::string _k;
        for (int i = 0; i < 7; ++i) {
            if (!读字符串(is, _k)) { delete out; return nullptr; }
        }
        out->所属自然句 = nullptr;
        out->子句主语 = nullptr; out->子句谓语 = nullptr; out->子句宾语 = nullptr;
        out->对应场景 = nullptr; out->对应状态 = nullptr; out->对应动态 = nullptr; out->对应因果 = nullptr;
        return out;
    }
    default:
        return nullptr;
    }
}

*/












