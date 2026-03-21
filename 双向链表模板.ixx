module;
#include <windows.h>
#include <string>
#include <vector>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <memory>
#include <type_traits>
#include <fstream>
#include <unordered_map>
#include <cstdint>
#include <limits>
export  module 模板模块;

import 基础数据类型模块;

// 双向链表模板（环形）
// - 同层：用 上/下 构成环
// - 全链：用 链上/链下 把整棵树的所有节点串成一个环，便于全局扫描/序列化
// - 线程安全：默认对外接口会加锁（链表锁）。
//   为避免递归删除/批量操作时重复加锁，提供 *_已加锁 版本：调用者需已持有链表锁。

export template<typename 主信息类型>
class 链表模板 {
public:
    struct 节点类 {


        节点类* 上 = nullptr;
        节点类* 下 = nullptr;

        节点类* 父 = nullptr;
        节点类* 子 = nullptr;

        节点类* 根 = nullptr;

        // 全链环（把整棵树所有节点串起来）
        节点类* 链上 = nullptr;
        节点类* 链下 = nullptr;

        主信息类型 主信息{};

        std::int64_t 子节点数量 = 0;

        void 变更子节点数量(std::int64_t delta) { 子节点数量 += delta; }
        std::string 获取主键() const { return 主键; }
        void 设置主键(const std::string& 主键_) {
            this->主键 = 主键_;
        }
    private:
        std::string  主键 = "";


        friend class 链表模板;
    };

    // =========================
    // 并发策略（线程友好版）
    // - 读：shared_lock（允许多读并发）
    // - 写：unique_lock（独占）
    // - 递归/批量：由外部一次性拿 unique_lock，然后调用 *_已加锁 版本
    //   （避免 shared_mutex 非递归带来的重复加锁问题）
    // =========================
public:
    using 写锁守卫 = std::unique_lock<std::shared_mutex>;
    using 读锁守卫 = std::shared_lock<std::shared_mutex>;
    using 锁守卫 = 写锁守卫;
    mutable std::shared_mutex 链表锁;

    //private:
    节点类* 根指针 = nullptr;


public:
    链表模板()
    {
        根指针 = new 节点类{};
        根指针->上 = 根指针;
        根指针->下 = 根指针;
        根指针->父 = nullptr;
        根指针->子 = nullptr;
        根指针->根 = 根指针;
        根指针->链上 = 根指针;
        根指针->链下 = 根指针;

    }

    ~链表模板() { 删除链表(); }

    // 获取写锁（批量/递归时强烈建议：一次性拿锁，再调用 *_已加锁）
    锁守卫 获取锁() const { return 锁守卫(链表锁); }
    // 获取读锁（多线程读友好）
    读锁守卫 获取读锁() const { return 读锁守卫(链表锁); }

private:
    // ====================== 主键生成 ======================
    std::string 获取新主键(const std::string& 前一主键) const
    {
        constexpr char 字符集[] =
            "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        constexpr std::size_t 字符集大小 = sizeof(字符集) - 1;

        std::string 新主键 = 前一主键;

        // 从末位开始进位
        for (int i = static_cast<int>(新主键.size()) - 1; i >= 0; --i) {
            std::size_t pos = 0;
            while (pos < 字符集大小 && 字符集[pos] != 新主键[i]) ++pos;

            if (pos + 1 < 字符集大小) {
                新主键[i] = 字符集[pos + 1];
                return 新主键;
            }
            新主键[i] = 字符集[0];
        }

        // 全部溢出：前面补 1
        新主键.insert(新主键.begin(), 字符集[1]);
        return 新主键;
    }

    // 把“新节点”追加到“全链环”尾部（在 根链 的链上/链下 上维护）
    void 更新链指针_已加锁(节点类* 根链, 节点类* 新节点)
    {
        if (!根链 || !新节点) return;

        // 插到 根链->链上 (尾) 与 根链 (头) 之间
        新节点->链上 = 根链->链上;
        新节点->链下 = 根链;

        根链->链上->链下 = 新节点;
        根链->链上 = 新节点;
    }

public:
    // =============================================================
    // 1) 添加节点：同层环插入 + 全链环登记
    // =============================================================

    // *_已加锁：调用者需已持有 链表锁
    节点类* 添加节点_已加锁(节点类* 位置节点, 节点类* 新节点)
    {
        if (!新节点) return nullptr;

        if (!位置节点) {
            位置节点 = 根指针;
        }

        // 继承父/根
        新节点->根 = 位置节点->根;
        新节点->父 = 位置节点->父;

        // 插入同层环：插到 位置节点 与 位置节点->下 之间
        新节点->上 = 位置节点;
        新节点->下 = 位置节点->下;

        位置节点->下->上 = 新节点;
        位置节点->下 = 新节点;

        // 主键递增（使用根链的主键种子）
        根指针->主键 = 新节点->主键 = 获取新主键(根指针->主键);

        // 维护父节点数量
        if (新节点->父) {
            新节点->父->变更子节点数量(1);
        }

        更新链指针_已加锁(根指针, 新节点);
        return 新节点;
    }

    节点类* 添加节点(节点类* 位置节点, 节点类* 新节点)
    {
        写锁守卫 lock(链表锁);
        return 添加节点_已加锁(位置节点, 新节点);
    }

    节点类* 添加节点_已加锁(节点类* 位置节点, 主信息类型 主信息)
    {
        auto* 新节点 = new 节点类{};
        新节点->主信息 = 主信息;
        return 添加节点_已加锁(位置节点, 新节点);
    }

    节点类* 添加节点(节点类* 位置节点, 主信息类型 主信息)
    {
        写锁守卫 lock(链表锁);
        return 添加节点_已加锁(位置节点, 主信息);
    }

    // =============================================================
    // 2) 添加子节点：挂到 父节点 的 子链（同层环）+ 全链环登记
    // =============================================================

    节点类* 添加子节点_已加锁(节点类* 父节点, 节点类* 子节点)
    {
        if (!父节点 || !子节点) return nullptr;

        子节点->父 = 父节点;

        // 父节点第一次拥有子链
        if (!父节点->子) {
            父节点->子 = 子节点;

            子节点->上 = 子节点;
            子节点->下 = 子节点;
            子节点->根 = 子节点;

            根指针->主键 = 子节点->主键 = 获取新主键(根指针->主键);

            父节点->变更子节点数量(1);
            更新链指针_已加锁(根指针, 子节点);
            return 子节点;
        }

        // 已有子链：插入到 父节点->子 的同层环里
        return 添加节点_已加锁(父节点->子, 子节点);
    }

    节点类* 添加子节点(节点类* 父节点, 节点类* 子节点)
    {
        写锁守卫 lock(链表锁);
        return 添加子节点_已加锁(父节点, 子节点);
    }

    节点类* 添加子节点_已加锁(节点类* 父节点, 主信息类型 主信息)
    {
        auto* 子节点 = new 节点类{};
        子节点->主信息 = 主信息;
        return 添加子节点_已加锁(父节点, 子节点);
    }

    节点类* 添加子节点(节点类* 父节点, 主信息类型 主信息)
    {
        写锁守卫 lock(链表锁);
        return 添加子节点_已加锁(父节点, 主信息);
    }

    // =============================================================
    // 3) 查找（全链扫描）
    // =============================================================
    std::string 获取主键(节点类* 节点) const {
        if (!节点) return "";
        return 节点->获取主键();
    }

    // *_已加锁：调用者需已持有 读锁/写锁（通常用于批量扫描或上层统一管锁）
    节点类* 查找主键_已加锁(const std::string& 主键) const
    {
        if (!根指针) return nullptr;
        for (节点类* it = 根指针->链下; it && it != 根指针; it = it->链下) {
            if (it->主键 == 主键) return it;
        }
        return nullptr;
    }

    // 默认线程安全：自动加“读锁”
    节点类* 查找主键(const std::string& 主键) const
    {
        读锁守卫 lk(链表锁);
        return 查找主键_已加锁(主键);
    }

    // 当前链查找：只在同层环（上/下）里找，不跨全链（链上/链下）
    节点类* 查找主键_当前链_已加锁(节点类* 当前节点, const std::string& 主键) const
    {
        if (!当前节点) return nullptr;

        // 定位“当前链”的根哨兵：子链根或全局根
        节点类* root = 当前节点->根 ? 当前节点->根 : 当前节点;
        if (!root) return nullptr;

        // 如果 root 是全局根哨兵，则从根的第一个元素开始遍历
        节点类* it = (root == 根指针) ? 根指针->下 : root;
        if (!it) return nullptr;

        // 同层环遍历（下 指针）
        节点类* stop = (root == 根指针) ? 根指针 : root;
        while (it && it != stop) {
            if (it->主键 == 主键) return it;
            it = it->下;
        }
        return nullptr;
    }

    节点类* 查找主键_当前链(节点类* 当前节点, const std::string& 主键) const
    {
        读锁守卫 lk(链表锁);
        return 查找主键_当前链_已加锁(当前节点, 主键);
    }
    // =============================================================
    // 4) 修改节点信息
    // =============================================================

    // *_已加锁：调用者需已持有 链表锁
    void 修改节点信息_已加锁(节点类* 节点, 主信息类型 新信息)
    {
        if (!节点 || 节点 == 根指针) return;

        // 仅当主信息为“指针类型”时，认为节点独占其主信息，需要释放旧对象
        if constexpr (std::is_pointer_v<主信息类型>) {
            if (节点->主信息 != 新信息) {
                delete 节点->主信息;
                节点->主信息 = 新信息;
            }
        }
        else {
            节点->主信息 = 新信息;
        }
    }

    // 通过“节点指针”修改
    void 修改节点信息(节点类* 节点, 主信息类型 新信息)
    {
        写锁守卫 lock(链表锁);
        修改节点信息_已加锁(节点, 新信息);
    }

    // 通过“主键”修改
    void 修改节点信息(const std::string& 主键, 主信息类型 新信息)
    {
        写锁守卫 lock(链表锁);
        节点类* 节点 = 查找主键(主键);
        修改节点信息_已加锁(节点, 新信息);
    }

    // =============================================================
        // 5) 删除（支持递归删子链）
        // =============================================================

    void 删除节点_已加锁(节点类* 被删除节点)
    {
        if (!被删除节点 || 被删除节点 == 根指针) return;

        // 先递归删除子链（不会重复加锁）
        if (被删除节点->子) {
            删除子链_已加锁(被删除节点);
            被删除节点->子 = nullptr;
        }

        // 若父节点的 子 指针正指向该节点，需要提前修正
        if (被删除节点->父 && 被删除节点->父->子 == 被删除节点) {
            if (被删除节点->下 != 被删除节点) {
                被删除节点->父->子 = 被删除节点->下;
            }
            else {
                被删除节点->父->子 = nullptr;
            }
        }

        // 从同层环摘除
        被删除节点->上->下 = 被删除节点->下;
        被删除节点->下->上 = 被删除节点->上;

        // 从全链环摘除
        被删除节点->链上->链下 = 被删除节点->链下;
        被删除节点->链下->链上 = 被删除节点->链上;

        if (被删除节点->父) {
            被删除节点->父->变更子节点数量(-1);
        }

        if constexpr (std::is_pointer_v<主信息类型>) {
            delete 被删除节点->主信息;
            被删除节点->主信息 = nullptr;
        }

        delete 被删除节点;
    }

    void 删除节点(节点类* 被删除节点)
    {
        写锁守卫 lock(链表锁);
        删除节点_已加锁(被删除节点);
    }

    void 删除子链_已加锁(节点类* 父节点)
    {
        if (!父节点 || !父节点->子) return;

        // 用“父->子 逐个前移”的方式安全删除环形子链
        while (父节点->子) {
            节点类* 当前 = 父节点->子;

            if (当前->下 == 当前) {
                // 单节点子链
                删除节点_已加锁(当前);
                父节点->子 = nullptr;
                break;
            }
            else {
                // 先把父->子移到下一个，避免删除时父->子悬空/循环条件失效
                父节点->子 = 当前->下;
                删除节点_已加锁(当前);
            }
        }
    }

    void 删除子链(节点类* 父节点)
    {
        写锁守卫 lock(链表锁);
        删除子链_已加锁(父节点);
    }

    void 删除链表_已加锁()
    {
        if (!根指针) return;

        // 先删除所有顶层节点（会递归删除其子链）
        while (根指针->下 != 根指针) {
            删除节点_已加锁(根指针->下);
        }

        delete 根指针;
        根指针 = nullptr;
    }

    void 删除链表()
    {
        写锁守卫 lock(链表锁);
        删除链表_已加锁();
    }

public:
    // =============================================================
    // 6) 按条件查找（当前链扫描）
    // =============================================================

    // *_已加锁：调用者需已持有 读锁/写锁
    节点类* 查找节点_当前链_已加锁(
        节点类* 当前节点,
        主信息类型 查找内容,
        枚举_比较字段 比较字段 ) const
    {
        if (!当前节点 || !查找内容) return nullptr;

        节点类* root = 当前节点->根 ? 当前节点->根 : 当前节点;
        if (!root) return nullptr;

        // 情况1：root 是全局根哨兵（根指针），遍历根指针->下 ... 直到回到根指针
        if (root == 根指针) {
            for (节点类* it = 根指针->下; it && it != 根指针; it = it->下) {
                auto* mi = dynamic_cast<主信息类型>(it->主信息);
                if (mi && mi->比较(查找内容, 比较字段) == 1) return it;
            }
            return nullptr;
        }

        // 情况2：root 是子链“根节点”（环形），从 root 开始 do-while 扫描整圈
        节点类* it = root;
        do {
            auto* mi = dynamic_cast<主信息类型>(it->主信息);
            if (mi && mi->比较(查找内容, 比较字段) == 1) return it;
            it = it->下;
        } while (it && it != root);

        return nullptr;
    }


    // 默认线程安全：自动加“读锁”
    节点类* 查找节点_当前链(节点类* 当前节点, 主信息类型 查找内容, 枚举_比较字段 比较字段) const
    {
        读锁守卫 lk(链表锁);
        return 查找节点_当前链_已加锁(当前节点, 查找内容, 比较字段);
    }
    // =============================================================
    // 7) 序列化：保存/读取整棵树（包含同层环 + 全链环）
    // 说明：
    // - 采用二进制格式，写入所有节点的：主键/子节点数量/7个指针索引/主信息
    // - 指针全部用“节点索引”表达（按“全链环”顺序），避免地址失效
    // - 若 主信息类型 为指针：会写入空指针标记，并通过 主信息类的 序列化/反序列化 读写内容
    // =============================================================

private:
    static constexpr std::uint32_t _存档魔数 = 0x4C4C4442u; // 'BDLL' little-endian

    template<class T>
    static inline void _写POD(std::ostream& os, const T& v) {
        static_assert(std::is_trivially_copyable_v<T>);
        os.write(reinterpret_cast<const char*>(&v), sizeof(T));
    }

    template<class T>
    static inline bool _读POD(std::istream& is, T& v) {
        static_assert(std::is_trivially_copyable_v<T>);
        is.read(reinterpret_cast<char*>(&v), sizeof(T));
        return (bool)is;
    }

    static inline void _写字符串(std::ostream& os, const std::string& s) {
        const std::uint32_t n = (std::uint32_t)s.size();
        _写POD(os, n);
        if (n) os.write(s.data(), n);
    }

    static inline bool _读字符串(std::istream& is, std::string& out, std::uint32_t 上限 = 16u * 1024u * 1024u) {
        std::uint32_t n = 0;
        if (!_读POD(is, n)) return false;
        if (n > 上限) return false;
        out.assign(n, '\0');
        if (n) is.read(out.data(), n);
        return (bool)is;
    }

    static inline std::uint64_t _索引或空(const std::unordered_map<const 节点类*, std::uint64_t>& mp, const 节点类* p) {
        const std::uint64_t NIL = (std::numeric_limits<std::uint64_t>::max)();
        if (!p) return NIL;
        auto it = mp.find(p);
        return (it == mp.end()) ? NIL : it->second;
    }

    static inline 节点类* _由索引取指针(const std::vector<节点类*>& vec, std::uint64_t idx) {
        const std::uint64_t NIL = (std::numeric_limits<std::uint64_t>::max)();
        if (idx == NIL) return nullptr;
        if (idx >= (std::uint64_t)vec.size()) return nullptr;
        return vec[(std::size_t)idx];
    }


public:
    // =============================================================
    // 8) 遍历同层环（下指针环）
    // - 头==nullptr：直接返回 true（视为遍历完成）
    // - 回调可为：void(节点类*) 或 bool(节点类*)，返回 false 则提前停止
    // =============================================================
    template<class Fn>
    static bool 遍历同层环(节点类* 头, Fn&& 回调)
    {
        if (!头) return true;
        节点类* cur = 头;
        auto&& fn = 回调;

        using Ret = std::invoke_result_t<Fn, 节点类*>;
        if constexpr (std::is_same_v<Ret, bool>) {
            do {
                if (!fn(cur)) return false;
                cur = cur->下;
            } while (cur && cur != 头);
            return true;
        }
        else {
            do {
                fn(cur);
                cur = cur->下;
            } while (cur && cur != 头);
            return true;
        }
    }

    template<class Fn>
    static bool 遍历同层环(const 节点类* 头, Fn&& 回调)
    {
        if (!头) return true;
        const 节点类* cur = 头;
        auto&& fn = 回调;

        using Ret = std::invoke_result_t<Fn, const 节点类*>;
        if constexpr (std::is_same_v<Ret, bool>) {
            do {
                if (!fn(cur)) return false;
                cur = cur->下;
            } while (cur && cur != 头);
            return true;
        }
        else {
            do {
                fn(cur);
                cur = cur->下;
            } while (cur && cur != 头);
            return true;
        }
    }

public:
    // =============================================================
    // 9) 深拷贝树（返回“独立树”，不挂入本链表、不更新本链表主键种子）
    //
    // 独立树含义：
    // - 返回的新根及其所有子孙节点，均为 new 分配的全新节点
    // - 仅维护树内：父/子 关系 + 同层环（上/下）
    // - 不把新节点登记到本链表的“全链环（链上/链下）”
    //   （链上/链下 默认设为自环，避免野指针，但不参与任何全链扫描）
    //
    // 主信息拷贝：
    // - 若 主信息类型 为指针：按“原样引用”规则，直接拷贝指针（不克隆、不 new）
    //   （例如：词性词节点/语素节点等外部共享对象）
    // - 若 主信息类型 为值：要求可拷贝构造，直接拷贝
    // =============================================================
    static 节点类* 深拷贝树_独立(const 节点类* 源根)
    {
        return 私有_深拷贝树_独立_递归(源根);
    }


    // 线程安全包装：对源树加读锁（当源树来自“本链表”时建议用这个）
    节点类* 深拷贝树(const 节点类* 源根) const
    {
        读锁守卫 lk(链表锁);
        return 深拷贝树_独立(源根);
    }

    // 已加锁版本：调用者需已持有 读锁/写锁
    节点类* 深拷贝树_已加锁(const 节点类* 源根) const
    {
        return 深拷贝树_独立(源根);
    }


    // 释放独立树（与 深拷贝树_独立 配套使用）
    // 注意：该树不属于本链表，不能用 删除节点/删除链表 去释放。
    static void 释放树_独立(节点类*& 独立根)
    {
        if (!独立根) return;
        私有_递归释放_独立(独立根);
        独立根 = nullptr;
    }



private:
    static 主信息类型 私有_深拷贝主信息(const 主信息类型& src)
    {
        // 新规则：当 主信息类型 为指针时，不做深度拷贝，原样引用即可。
        // 典型例子：存在节点主信息里引用的 词性节点/语素节点 等“外部共享对象”，不应被深拷贝与释放。
        // 注意：这意味着“独立树”并不拥有这些指针；当原对象生命周期结束时，独立树中的指针将失效。
        if constexpr (std::is_pointer_v<主信息类型>) {
            return src;
        }
        else {
            static_assert(std::is_copy_constructible_v<主信息类型>,
                "链表模板::深拷贝树_独立：主信息为值类型时，需可拷贝构造。");
            return src;
        }
    }

    static void 私有_设置同层环根指针(节点类* 头)
    {
        if (!头) return;
        遍历同层环(头, [&](节点类* n) {
            n->根 = 头;
            return true;
            });
    }

    static 节点类* 私有_深拷贝树_独立_递归(const 节点类* src)
    {
        if (!src) return nullptr;

        auto* dst = new 节点类{};

        // 复制结构（不挂任何外部链）
        dst->主键 = src->主键;
        dst->子节点数量 = src->子节点数量;

        // 同层环先自环
        dst->上 = dst;
        dst->下 = dst;

        // 父/子
        dst->父 = nullptr;
        dst->子 = nullptr;

        // 同层根指针：先指向自己（若随后被插入父子链，会被修正为子链头）
        dst->根 = dst;

        // 全链环：独立树默认自环（不登记到原链表）
        dst->链上 = dst;
        dst->链下 = dst;

        // 主信息深拷贝
        dst->主信息 = 私有_深拷贝主信息(src->主信息);

        // 递归复制子链（同层环）
        if (src->子) {
            const 节点类* src_head = src->子;
            const 节点类* cur = src_head;

            节点类* dst_head = nullptr;
            节点类* dst_tail = nullptr;
            std::int64_t cnt = 0;

            do {
                // 递归得到“子树根”
                节点类* child = 私有_深拷贝树_独立_递归(cur);
                child->父 = dst;

                // 追加到 dst 子链尾部（保持顺序）
                if (!dst_head) {
                    dst_head = child;
                    dst_tail = child;
                    child->上 = child->下 = child;
                }
                else {
                    child->上 = dst_tail;
                    child->下 = dst_head;
                    dst_tail->下 = child;
                    dst_head->上 = child;
                    dst_tail = child;
                }

                ++cnt;
                cur = cur->下;
            } while (cur && cur != src_head);

            dst->子 = dst_head;
            dst->子节点数量 = cnt;

            // 修正：子链同层根指针（所有兄弟的 根 指向 子链头）
            私有_设置同层环根指针(dst_head);
        }

        return dst;
    }


    static void 私有_递归释放_独立(节点类* n)
    {
        if (!n) return;

        // 先释放子链（同层环）
        if (n->子) {
            节点类* head = n->子;
            节点类* cur = head;
            do {
                节点类* nxt = cur->下;
                // 断开同层环，避免重复遍历
                cur->上 = nullptr;
                cur->下 = nullptr;
                私有_递归释放_独立(cur);
                cur = nxt;
            } while (cur && cur != head);
            n->子 = nullptr;
        }

        // 主信息释放策略：
        // - 主信息为指针：按“原样引用”规则，不在独立树中释放（避免误删外部共享对象）
        // - 主信息为值：随节点析构自动释放
        if constexpr (std::is_pointer_v<主信息类型>) {
            n->主信息 = nullptr;
        }

        delete n;
    }

    /*
    public:
        bool 保存数据_已加锁(const std::string& 文件名) const
        {
            std::ofstream ofs(文件名, std::ios::binary);
            if (!ofs) return false;

            const std::uint16_t ver = 1;
            _写POD(ofs, _存档魔数);
            _写POD(ofs, ver);

            // 1) 收集节点：按“全链环”顺序（含 根指针 自身）
            std::vector<节点类*> nodes;
            nodes.reserve(1024);
            nodes.push_back(根指针);
            for (节点类* it = 根指针 ? 根指针->链下 : nullptr; it&& it != 根指针; it = it->链下) {
                nodes.push_back(it);
            }

            const std::uint64_t count = (std::uint64_t)nodes.size();
            _写POD(ofs, count);

            // 2) 建索引表
            std::unordered_map<const 节点类*, std::uint64_t> idx;
            idx.reserve((std::size_t)count * 2);
            for (std::uint64_t i = 0; i < count; ++i) idx[nodes[(std::size_t)i]] = i;

            // 3) 写节点
            for (auto* n : nodes) {
                _写字符串(ofs, n ? n->主键 : std::string{});
                const std::int64_t childCnt = n ? n->子节点数量 : 0;
                _写POD(ofs, childCnt);

                const std::uint64_t i_up = _索引或空(idx, n ? n->上 : nullptr);
                const std::uint64_t i_dn = _索引或空(idx, n ? n->下 : nullptr);
                const std::uint64_t i_pa = _索引或空(idx, n ? n->父 : nullptr);
                const std::uint64_t i_ch = _索引或空(idx, n ? n->子 : nullptr);
                const std::uint64_t i_ro = _索引或空(idx, n ? n->根 : nullptr);
                const std::uint64_t i_cu = _索引或空(idx, n ? n->链上 : nullptr);
                const std::uint64_t i_cd = _索引或空(idx, n ? n->链下 : nullptr);

                _写POD(ofs, i_up); _写POD(ofs, i_dn); _写POD(ofs, i_pa); _写POD(ofs, i_ch);
                _写POD(ofs, i_ro); _写POD(ofs, i_cu); _写POD(ofs, i_cd);

                // 主信息
                if constexpr (std::is_pointer_v<主信息类型>) {
                    const std::uint8_t has = (n && n->主信息) ? 1u : 0u;
                    _写POD(ofs, has);
                    if (has) {
                        n->主信息->序列化(ofs);
                    }
                }
                else if constexpr (requires(const 主信息类型 & x, std::ostream & os) { x.序列化(os); }) {
                    if (n) n->主信息.序列化(ofs);
                    else {
                        主信息类型 tmp{};
                        tmp.序列化(ofs);
                    }
                }
                else if constexpr (std::is_trivially_copyable_v<主信息类型>) {
                    if (n) _写POD(ofs, n->主信息);
                    else {
                        主信息类型 tmp{};
                        _写POD(ofs, tmp);
                    }
                }
                else {
                    static_assert(std::is_trivially_copyable_v<主信息类型>,
                        "链表模板::保存数据：主信息类型 需要可序列化（提供 序列化/反序列化）或可平凡拷贝。");
                }
            }

            return (bool)ofs;
        }

        bool 保存数据(const std::string& 文件名) const
        {
            读锁守卫 lk(链表锁);
            return 保存数据_已加锁(文件名);
        }

        // 兼容旧签名（忽略返回值也能用）
        bool 保存数据(std::string 文件名) const { return 保存数据((const std::string&)文件名); }

        bool 读取数据_已加锁(const std::string& 文件名)
        {
            std::ifstream ifs(文件名, std::ios::binary);
            if (!ifs) return false;

            std::uint32_t magic = 0;
            std::uint16_t ver = 0;
            if (!_读POD(ifs, magic)) return false;
            if (!_读POD(ifs, ver)) return false;
            if (magic != _存档魔数) return false;
            if (ver != 1) return false;

            std::uint64_t count = 0;
            if (!_读POD(ifs, count)) return false;
            if (count == 0 || count > 10'000'000ull) return false; // 基本防护

            struct 索引包 { std::uint64_t up{}, dn{}, pa{}, ch{}, ro{}, cu{}, cd{}; };
            std::vector<节点类*> nodes(count, nullptr);
            std::vector<索引包> links(count);

            // 先读所有节点（不连指针）
            for (std::uint64_t i = 0; i < count; ++i) {
                auto* n = new 节点类{};
                nodes[(std::size_t)i] = n;

                if (!_读字符串(ifs, n->主键)) goto FAIL;
                if (!_读POD(ifs, n->子节点数量)) goto FAIL;

                if (!_读POD(ifs, links[(std::size_t)i].up)) goto FAIL;
                if (!_读POD(ifs, links[(std::size_t)i].dn)) goto FAIL;
                if (!_读POD(ifs, links[(std::size_t)i].pa)) goto FAIL;
                if (!_读POD(ifs, links[(std::size_t)i].ch)) goto FAIL;
                if (!_读POD(ifs, links[(std::size_t)i].ro)) goto FAIL;
                if (!_读POD(ifs, links[(std::size_t)i].cu)) goto FAIL;
                if (!_读POD(ifs, links[(std::size_t)i].cd)) goto FAIL;

                // 主信息
                if constexpr (std::is_pointer_v<主信息类型>) {
                    std::uint8_t has = 0;
                    if (!_读POD(ifs, has)) goto FAIL;
                    if (has) {
                        using Pointee = std::remove_pointer_t<主信息类型>;
                        n->主信息 = Pointee::反序列化(ifs);
                        if (!n->主信息) goto FAIL;
                    }
                    else {
                        n->主信息 = nullptr;
                    }
                }
                else if constexpr (std::is_trivially_copyable_v<主信息类型>) {
                    if (!_读POD(ifs, n->主信息)) goto FAIL;
                }
                else {
                    // 如需支持 value-type 的自定义反序列化，可在此扩展
                    static_assert(std::is_trivially_copyable_v<主信息类型>,
                        "链表模板::读取数据：主信息类型 需要可反序列化（提供 static 反序列化）或可平凡拷贝。");
                }
            }

            // 再连指针
            for (std::uint64_t i = 0; i < count; ++i) {
                auto* n = nodes[(std::size_t)i];
                const auto& L = links[(std::size_t)i];
                n->上 = _由索引取指针(nodes, L.up);
                n->下 = _由索引取指针(nodes, L.dn);
                n->父 = _由索引取指针(nodes, L.pa);
                n->子 = _由索引取指针(nodes, L.ch);
                n->根 = _由索引取指针(nodes, L.ro);
                n->链上 = _由索引取指针(nodes, L.cu);
                n->链下 = _由索引取指针(nodes, L.cd);
            }

            // 替换旧链表
            删除链表_已加锁();
            根指针 = nodes[0];
            return true;

        FAIL:
            for (auto* n : nodes) {
                if (!n) continue;
                if constexpr (std::is_pointer_v<主信息类型>) {
                    delete n->主信息;
                    n->主信息 = nullptr;
                }
                delete n;
            }
            return false;
        }

        bool 读取数据(const std::string& 文件名)
        {
            写锁守卫 lk(链表锁);
            return 读取数据_已加锁(文件名);
        }

        // 兼容旧签名（忽略返回值也能用）
        bool 读取数据(std::string 文件名) { return 读取数据((const std::string&)文件名); }
        */
};
