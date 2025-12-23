
export module 特征值模块;

import <vector>;
import <cstdint>;
import <cmath>;
import <limits>;
import <algorithm>;
import <variant>;
import <numbers>;
import <unordered_map>;
import <array>;
import <compare>;

import 模板模块;
import 主信息定义模块;
import 数据仓库模块;
import 基础数据类型模块;  // 为了颜色_BGR8 和 Bits64

export class 特征值类 {
public:
    using 节点类 = 特征值节点类;
    using 主信息类 = 特征值主信息类;

    struct 融合参数 {
        double 学习率 = 0.25;
        bool   区间扩张 = true;
        int    Bits融合策略 = 0;  // 0=不融合；1=按位或；2=按位与
    };
    void 初始化() {};
public:
    // =========================== 获取 / 创建（查重） ===========================
    节点类* 获取或创建(const 主信息类& mi) const;
    节点类* 获取或创建(主信息类&& mi) const;

    // =========================== 获取 / 创建（查重）已加锁版本 ===========================
    // 约定：调用这些函数前，外部必须已持有 特征值链.链表锁。
    // 目的：批量写入时避免重复加锁（B2 终极形态）。
    节点类* 获取或创建_已加锁(const 主信息类& mi) const
    {
        主信息类 key = mi;
        if (key.粗哈希 == 0) 重新计算粗哈希(key);
        确保初始化_已加锁();

        // 自己做‘等值’判定：使用 主信息定义模块 的严格比较。
        if (key.类型 != 枚举_特征值类型::未定义) {
            const auto i = idx(key.类型);
            auto it = 类型索引[i].buckets.find(key.粗哈希);
            if (it != 类型索引[i].buckets.end()) {
                for (auto* n : it->second) {
                    if (!n || !n->主信息) continue;
                    if (严格比较_用于排序(*n->主信息, key) == std::strong_ordering::equal) {
                        n->主信息->命中次数 += 1;
                        return n;
                    }
                }
            }
        }

        auto* stored = new 主信息类(key);
        if (stored->命中次数 == 0) stored->命中次数 = 1;
        auto* parent = (stored->类型 != 枚举_特征值类型::未定义) ? 类型根[idx(stored->类型)] : nullptr;
        if (!parent) parent = 特征值链.根指针;
        auto* node = 特征值链.添加子节点_已加锁(parent, stored);
        if (stored->类型 != 枚举_特征值类型::未定义) {
            索引插入_已加锁(*stored, node);
        }
        return node;
    }

    节点类* 获取或创建_已加锁(主信息类&& mi) const
    {
        return 获取或创建_已加锁((const 主信息类&)mi);
    }

    节点类* 获取或创建标量特征值_已加锁(void* 单位, std::int64_t 值,
        枚举_比较模式 mode = 枚举_比较模式::有序标量) const
    {
        主信息类 mi;
        mi.类型 = 枚举_特征值类型::I64;
        mi.比较模式 = mode;
        mi.值 = 值;
        重新计算粗哈希(mi);
        return 获取或创建_已加锁(std::move(mi));
    }

    节点类* 获取或创建无符号标量特征值_已加锁(void* 单位, std::uint64_t 值,
        枚举_比较模式 mode = 枚举_比较模式::有序标量) const
    {
        主信息类 mi;
        mi.类型 = 枚举_特征值类型::U64;
        mi.比较模式 = mode;
        mi.值 = 值;
        重新计算粗哈希(mi);
        return 获取或创建_已加锁(std::move(mi));
    }

    节点类* 获取或创建浮点特征值_已加锁(double 值,
        枚举_比较模式 mode = 枚举_比较模式::有序标量) const
    {
        主信息类 mi;
        mi.类型 = 枚举_特征值类型::F64;
        mi.比较模式 = mode;
        mi.值 = 值;
        重新计算粗哈希(mi);
        return 获取或创建_已加锁(std::move(mi));
    }

    节点类* 获取或创建矢量特征值_已加锁(const std::vector<std::int64_t>& 值,
        枚举_比较模式 mode = 枚举_比较模式::字典序向量) const
    {
        主信息类 mi;
        mi.类型 = 枚举_特征值类型::VecI64;
        mi.比较模式 = mode;
        mi.值 = 值;
        重新计算粗哈希(mi);
        return 获取或创建_已加锁(std::move(mi));
    }

    节点类* 获取或创建向量特征值_f32_已加锁(const std::vector<float>& 值,
        枚举_比较模式 mode = 枚举_比较模式::字典序向量) const
    {
        主信息类 mi;
        mi.类型 = 枚举_特征值类型::VecF32;
        mi.比较模式 = mode;
        mi.值 = 值;
        重新计算粗哈希(mi);
        return 获取或创建_已加锁(std::move(mi));
    }

    节点类* 获取或创建字符串特征值_UTF8_已加锁(const std::string& utf8,
        枚举_比较模式 mode = 枚举_比较模式::字典序向量) const
    {
        主信息类 mi;
        mi.类型 = 枚举_特征值类型::StringUTF8;
        mi.比较模式 = mode;
        mi.值 = utf8;
        重新计算粗哈希(mi);
        return 获取或创建_已加锁(std::move(mi));
    }

    节点类* 获取或创建引用特征值_已加锁(const void* ptr,
        枚举_比较模式 mode = 枚举_比较模式::有序标量) const
    {
        主信息类 mi;
        mi.类型 = 枚举_特征值类型::Ptr;
        mi.比较模式 = mode;
        mi.值 = (void*)ptr;
        重新计算粗哈希(mi);
        return 获取或创建_已加锁(std::move(mi));
    }

    节点类* 获取或创建标量特征值(void* 单位, std::int64_t 值,
        枚举_比较模式 mode = 枚举_比较模式::有序标量) const;

    节点类* 获取或创建无符号标量特征值(void* 单位, std::uint64_t 值,
        枚举_比较模式 mode = 枚举_比较模式::有序标量) const;

    节点类* 获取或创建浮点特征值(double 值,
        枚举_比较模式 mode = 枚举_比较模式::有序标量) const;

    节点类* 获取或创建矢量特征值(const std::vector<std::int64_t>& 值,
        枚举_比较模式 mode = 枚举_比较模式::字典序向量) const;
    // 你要的函数：补上它
    特征值节点类* 获取或创建向量特征值_已加锁(const std::vector<std::int64_t>& vec,
        枚举_比较模式 mode = 枚举_比较模式::海明距离,
        词性节点类* 单位 = nullptr);
    节点类* 获取或创建向量特征值_f32(const std::vector<float>& 值,
        枚举_比较模式 mode = 枚举_比较模式::字典序向量) const;

    节点类* 获取或创建字符串特征值_UTF8(const std::string& utf8,
        枚举_比较模式 mode = 枚举_比较模式::字典序向量) const;

    节点类* 获取或创建引用特征值(const void* ptr,
        枚举_比较模式 mode = 枚举_比较模式::有序标量) const;

    // ================ 新增：颜色类型（颜色_BGR8） ================
    节点类* 获取或创建颜色特征值(const 颜色_BGR8& 颜色,
        枚举_比较模式 mode = 枚举_比较模式::有序标量) const
    {
        主信息类 mi;
        mi.类型 = 枚举_特征值类型::颜色_BGR8;
        mi.比较模式 = mode;
        mi.值 = 颜色;
        重新计算粗哈希(mi);
        return const_cast<特征值类*>(this)->获取或创建(std::move(mi));
    }

    // *_已加锁 版本：外部已持有 特征值链.链表锁
    节点类* 获取或创建颜色特征值_已加锁(const 颜色_BGR8& 颜色,
        枚举_比较模式 mode = 枚举_比较模式::有序标量) const
    {
        主信息类 mi;
        mi.类型 = 枚举_特征值类型::颜色_BGR8;
        mi.比较模式 = mode;
        mi.值 = 颜色;
        重新计算粗哈希(mi);
        return 获取或创建_已加锁(std::move(mi));
    }

    // ================ 新增：位图类型（Bits64，轮廓编码） ================
    节点类* 获取或创建位图特征值(const Bits64& bits,
        枚举_比较模式 mode = 枚举_比较模式::有序标量) const
    {
        主信息类 mi;
        mi.类型 = 枚举_特征值类型::Bits64;
        mi.比较模式 = mode;
        mi.值 = bits;
        重新计算粗哈希(mi);
        return const_cast<特征值类*>(this)->获取或创建(std::move(mi));
    }

    // *_已加锁 版本：外部已持有 特征值链.链表锁
    节点类* 获取或创建位图特征值_已加锁(const Bits64& bits,
        枚举_比较模式 mode = 枚举_比较模式::有序标量) const
    {
        主信息类 mi;
        mi.类型 = 枚举_特征值类型::Bits64;
        mi.比较模式 = mode;
        mi.值 = bits;
        重新计算粗哈希(mi);
        return 获取或创建_已加锁(std::move(mi));
    }

    // =========================== 创建（不查重） ===========================
    节点类* 创建_不查重(const 主信息类& mi) const;
    节点类* 创建_不查重(主信息类&& mi) const;

    // =========================== 修改 / 融合 / 删除 ===========================
    bool 覆盖(节点类* 节点, const 主信息类& 新值) const;

    bool 融合(节点类* 旧节点, const 主信息类& 新观测,
        const 融合参数& p = {}, bool 失败则覆盖 = false) const;

    bool 删除(节点类* 节点) const;

private:
    static constexpr std::size_t 类型数 = (std::size_t)枚举_特征值类型::体素占据金字塔 + 1;

    struct 桶索引 {
        std::unordered_map<std::uint64_t, std::vector<节点类*>> buckets;
    };

    inline static bool 已初始化 = false;
    inline static std::array<节点类*, 类型数> 类型根{};
    inline static std::array<桶索引, 类型数>  类型索引{};

    static std::size_t idx(枚举_特征值类型 t) { return (std::size_t)t; }

    static bool 等值(const 主信息类& a, const 主信息类& b);

    static void 确保初始化_已加锁();
    static 节点类* 查找等值_已加锁(const 主信息类& key);
    static void 索引插入_已加锁(const 主信息类& mi, 节点类* node);
    static void 索引移除_已加锁(const 主信息类& mi, 节点类* node);

    static bool 尝试融合_已加锁(主信息类& inout, const 主信息类& obs, const 融合参数& p);

    static void 重新计算粗哈希(主信息类& mi)
    {
        mi.粗哈希 = 计算粗哈希(mi);
    }
};