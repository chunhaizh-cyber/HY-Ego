export module 三维场景管理模块;

import <vector>;
import <cstdint>;
import <cmath>;
import <limits>;
import <algorithm>;
import <mutex>;
import <optional>;
import <variant>;

import 世界树环境模块;
import 特征值环境模块;
import 主信息定义模块;
import 数据仓库模块;
import 特征类型定义模块;
import 基础数据类型模块;
import 存在提取模块; // 结构体_存在观测

// 设计目标（2025-12）：
// 1) 三维场景管理只负责“相机系/自我系”的相对坐标(P_rel)融合与存在匹配，不再负责绝对坐标(P_abs)建立/刷新。
// 2) 绝对坐标与锚点策略交给 自我线程/自我模块 维护。
// 3) 用法B：支持外部一次性锁住 世界链 + 特征值链 后调用 *_已加锁 写入（避免重复加锁）。

export class 三维场景管理类
{
public:
    // ================= 单位转换 =================
    static constexpr double MM_PER_M = 1000.0;
    static std::int64_t 米到毫米(double m) { return static_cast<std::int64_t>(std::llround(m * MM_PER_M)); }
    static double 毫米到米(std::int64_t mm) { return static_cast<double>(mm) / MM_PER_M; }

    // ================= 配置参数 =================
    struct 匹配参数
    {
        std::int64_t 位置容差_mm = 100;     // 10cm
        std::int64_t 尺寸容差_mm = 150;     // 尺寸偏差容忍
        double       最小质量阈值 = 0.30;  // 低于此质量不参与“新建”（可命中既存）

        // 匹配评分：score = dpos*位置权重 + dsize*尺寸权重
        double 位置权重 = 1.0;
        double 尺寸权重 = 0.35;

        // 兼容字段（绝对坐标锚点策略已迁移到自我线程，这里不再使用）
        bool 锚点规则_启用 = false;
    };

private:
    场景节点类* 当前场景 = nullptr;
    匹配参数   默认匹配参数{};

    // 本帧可见存在缓存（仅用于输出/票据生成；引用有效期要求：必须在锁保护下使用）
    std::vector<存在节点类*> 本帧可见存在缓存;

    // --- 立体模型（体素占据金字塔）配置 ---
    词性节点类* 三维模型特征类型 = nullptr;
    std::uint8_t  三维模型最大层 = 2;
    double        三维模型匹配阈值 = 0.28;
    std::uint32_t 三维模型最大原型数 = 12;


private:
    // ================= 工具函数（已加锁/无锁混用） =================
    static bool 含有(const std::vector<存在节点类*>& list, const 存在节点类* x)
    {
        for (auto* p : list) if (p == x) return true;
        return false;
    }

    // —— 统一写特征值：存在已有关联特征则覆盖其值，否则新建特征节点 ——
    static 特征节点类* 在存在中查找特征类型_已加锁(存在节点类* 存在, 词性节点类* 特征类型)
    {
        if (!存在 || !特征类型) return nullptr;
        auto* emi = dynamic_cast<存在节点主信息类*>(存在->主信息);
        if (!emi) return nullptr;

        if (auto* f = emi->查找特征当前(特征类型)) return f;
        for (auto* f : emi->特征索引) {
            if (!f || !f->主信息) continue;
            auto* fmi = dynamic_cast<特征节点主信息类*>(f->主信息);
            if (fmi && fmi->类型 == 特征类型) return f;
        }
        return nullptr;
    }

    static 存在节点类* 为场景添加存在_已加锁(场景节点类* 场景, 存在节点主信息类* mi)
    {
        if (!场景 || !mi) return nullptr;
        auto* node = static_cast<存在节点类*>(世界链.添加子节点_已加锁(场景, mi));
        if (auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息)) {
            smi->存在索引.push_back(node);
        }
        return node;
    }

    static 特征节点类* 为存在添加特征_已加锁(存在节点类* 存在, 特征节点主信息类* fmi)
    {
        if (!存在 || !fmi) return nullptr;
        auto* node = static_cast<特征节点类*>(世界链.添加子节点_已加锁(存在, fmi));
        if (auto* emi = dynamic_cast<存在节点主信息类*>(存在->主信息)) {
            emi->特征索引.push_back(node);
            if (!emi->查找特征当前(fmi->类型)) {
                emi->特征当前状态.push_back(node);
            }
        }
        return node;
    }

    static void 写特征值_覆盖或创建_已加锁(存在节点类* 存在, 词性节点类* 特征类型, 特征值节点类* 值)
    {
        if (!存在 || !特征类型 || !值) return;

        if (auto* f = 在存在中查找特征类型_已加锁(存在, 特征类型)) {
            auto* fmi = dynamic_cast<特征节点主信息类*>(f->主信息);
            if (fmi) fmi->值 = 值;
            return;
        }

        auto* fmi = new 特征节点主信息类(特征类型, 值);
        为存在添加特征_已加锁(存在, fmi);
    }

    static void 写I64_已加锁(存在节点类* 存在, 词性节点类* 类型, std::int64_t v)
    {
        if (!存在 || !类型) return;
        auto* n = 特征值集.获取或创建标量特征值_已加锁(nullptr, v, 枚举_比较模式::有序标量);
        写特征值_覆盖或创建_已加锁(存在, 类型, n);
    }

    static void 写U64_已加锁(存在节点类* 存在, 词性节点类* 类型, std::uint64_t v)
    {
        if (!存在 || !类型) return;
        auto* n = 特征值集.获取或创建无符号标量特征值_已加锁(nullptr, v, 枚举_比较模式::有序标量);
        写特征值_覆盖或创建_已加锁(存在, 类型, n);
    }

    static void 写颜色_BGR8_已加锁(存在节点类* 存在, 词性节点类* 类型, const 颜色_BGR8& c)
    {
        if (!存在 || !类型) return;
        auto* n = 特征值集.获取或创建颜色特征值_已加锁(c, 枚举_比较模式::有序标量);
        写特征值_覆盖或创建_已加锁(存在, 类型, n);
    }

    static void 写Bits64_已加锁(存在节点类* 存在, 词性节点类* 类型, const Bits64& bits)
    {
        if (!存在 || !类型) return;
        auto* n = 特征值集.获取或创建位图特征值_已加锁(bits, 枚举_比较模式::海明距离);
        写特征值_覆盖或创建_已加锁(存在, 类型, n);
    }

    static void 写VecI64_已加锁(存在节点类* 存在, 词性节点类* 类型, const std::vector<std::int64_t>& v, 枚举_比较模式 mode)
    {
        if (!存在 || !类型) return;
        auto* n = 特征值集.获取或创建向量特征值_已加锁(v, mode);
        写特征值_覆盖或创建_已加锁(存在, 类型, n);
    }

    static 体素占据金字塔 构建体素占据金字塔_从点集(
        const std::vector<Vector3D>& pts,
        const Vector3D& center,
        const Vector3D& size,
        std::uint8_t maxLevel)
    {
        体素占据金字塔 out;
        out.最大层 = maxLevel;

        const double edge_m = std::max({ size.x, size.y, size.z });
        if (edge_m <= 0.0) {
            return out;
        }

        const double half = edge_m * 0.5;
        const Vector3D minp{ center.x - half, center.y - half, center.z - half };

        out.立方体边长_mm = (std::int64_t)std::llround(edge_m * 1000.0);
        out.min_x_mm = (std::int64_t)std::llround(minp.x * 1000.0);
        out.min_y_mm = (std::int64_t)std::llround(minp.y * 1000.0);
        out.min_z_mm = (std::int64_t)std::llround(minp.z * 1000.0);

        // 1 + 8 + 64 + ... + 8^L
        std::size_t totalBlocks = 0;
        for (std::uint8_t l = 0; l <= maxLevel; ++l) {
            totalBlocks += (std::size_t)1ull << (3u * l);
        }
        out.块.assign(totalBlocks, 0ull);

        auto clamp01 = [](double v) {
            if (v <= 0.0) return 0.0;
            if (v >= 1.0) return 0.999999999; // 防止落到 g
            return v;
            };

        std::size_t offset = 0;
        for (std::uint8_t l = 0; l <= maxLevel; ++l) {
            const int g = 4 << l;                      // 4,8,16,...
            const std::size_t blocksThis = (std::size_t)1ull << (3u * l); // 1,8,64...

            for (const auto& p : pts) {
                const double ux = clamp01((p.x - minp.x) / edge_m);
                const double uy = clamp01((p.y - minp.y) / edge_m);
                const double uz = clamp01((p.z - minp.z) / edge_m);

                int xi = (int)std::floor(ux * g);
                int yi = (int)std::floor(uy * g);
                int zi = (int)std::floor(uz * g);
                xi = std::clamp(xi, 0, g - 1);
                yi = std::clamp(yi, 0, g - 1);
                zi = std::clamp(zi, 0, g - 1);

                const std::size_t bit = (std::size_t)((zi * g + yi) * g + xi);
                const std::size_t bi = bit >> 6;
                const std::size_t bb = bit & 63u;
                if (bi < blocksThis) {
                    out.块[offset + bi] |= (1ull << bb);
                }
            }

            offset += blocksThis;
        }

        return out;
    }

    static std::int64_t 取轴特征值_mm_已加锁(存在节点类* 存在, 词性节点类* 类型, std::int64_t 默认值)
    {
        if (!存在 || !类型) return 默认值;
        auto* f = 在存在中查找特征类型_已加锁(存在, 类型);
        if (!f) return 默认值;
        auto* info = dynamic_cast<特征节点主信息类*>(f->主信息);
        if (!info || !info->值 || !info->值->主信息) return 默认值;

        // 兼容“值可能是一个链头”的写法：按环查找最接近默认值的 I64。
        std::int64_t best = 默认值;
        std::uint64_t bestDist = std::numeric_limits<std::uint64_t>::max();

        auto* head = info->值;
        auto* cur = head;
        do {
            if (!cur || !cur->主信息) break;
            const auto* mi = cur->主信息;
            if (auto* pi = std::get_if<std::int64_t>(&mi->值)) {
                const std::uint64_t d = (std::uint64_t)std::llabs(*pi - 默认值);
                if (d < bestDist) { bestDist = d; best = *pi; }
            }
            cur = cur->下;
        } while (cur && cur != head);

        return best;
    }

    static int 比较匹配度_已加锁(存在节点类* e,
        std::int64_t cx, std::int64_t cy, std::int64_t cz,
        std::int64_t sx, std::int64_t sy, std::int64_t sz,
        const 匹配参数& p)
    {
        if (!e) return -1;

        const std::int64_t ex = 取轴特征值_mm_已加锁(e, 类型_相对位置X, std::numeric_limits<std::int64_t>::min());
        const std::int64_t ey = 取轴特征值_mm_已加锁(e, 类型_相对位置Y, std::numeric_limits<std::int64_t>::min());
        const std::int64_t ez = 取轴特征值_mm_已加锁(e, 类型_相对位置Z, std::numeric_limits<std::int64_t>::min());
        if (ex == std::numeric_limits<std::int64_t>::min()) return -1;

        const std::int64_t esx = 取轴特征值_mm_已加锁(e, 类型_尺寸左右, 1);
        const std::int64_t esy = 取轴特征值_mm_已加锁(e, 类型_尺寸上下, 1);
        const std::int64_t esz = 取轴特征值_mm_已加锁(e, 类型_尺寸前后, 1);

        auto absd = [](std::int64_t a, std::int64_t b) { return (std::uint64_t)std::llabs(a - b); };
        const std::uint64_t dpos = absd(ex, cx) + absd(ey, cy) + absd(ez, cz);
        const std::uint64_t dsize = absd(esx, sx) + absd(esy, sy) + absd(esz, sz);

        const double score = (double)dpos * p.位置权重 + (double)dsize * p.尺寸权重;
        const double s = std::max(0.0, 1e12 - score);
        return (int)std::min(1e12, s);
    }

public:
    explicit 三维场景管理类() = default;

    void 设置当前场景(场景节点类* 场景) { 当前场景 = 场景; }

    // ===== 可选：写入“三维模型”特征（体素占据金字塔） =====
    // 若不调用绑定函数，则默认不写该特征。
    void 绑定三维模型特征类型(词性节点类* t) { 三维模型特征类型 = t; }
    void 设置三维模型最大层(std::uint8_t L) { 三维模型最大层 = L; }
    void 设置三维模型匹配阈值(double v) { 三维模型匹配阈值 = v; }
    void 设置三维模型最大原型数(std::uint32_t n) { 三维模型最大原型数 = n; }


    // 兼容接口：绝对坐标锚点策略已迁移到自我线程，这里保留空实现以避免上层改动造成编译失败。
    [[deprecated("绝对坐标锚点已迁移到自我线程：三维场景管理仅维护相对坐标(P_rel)")]]
    void 设置绝对坐标锚点(存在节点类* /*锚点*/) {}

    [[deprecated("绝对坐标锚点已迁移到自我线程：三维场景管理仅维护相对坐标(P_rel)")]]
    存在节点类* 获取绝对坐标锚点() const { return nullptr; }

    // ===== 获取本帧可见存在（在锁保护下读取引用，或让调用方拷贝） =====
    const std::vector<存在节点类*>& 获取本帧可见存在_已加锁() const { return 本帧可见存在缓存; }

    // ================= 核心：找到或创建存在（已加锁） =================
    // 约定：调用前外部已持有 世界链.链表锁 与 特征值链.链表锁（用法B可一次性锁整帧）。
    存在节点类* 找到或创建存在_已加锁(const 结构体_存在观测& 观测, const 匹配参数& p = {})
    {
        if (!当前场景) 当前场景 = 世界树.自我所在场景;
        if (!当前场景) return nullptr;

        // 质量过低：允许“命中既存”，但不允许“新建”（避免噪声存在爆炸）
        const bool 允许新建 = !(观测.质量分 > 0.0 && 观测.质量分 < p.最小质量阈值);

        const 匹配参数& param = (p.位置容差_mm == 0 && p.尺寸容差_mm == 0) ? 默认匹配参数 : p;

        const std::int64_t cx = 米到毫米(观测.中心坐标.x);
        const std::int64_t cy = 米到毫米(观测.中心坐标.y);
        const std::int64_t cz = 米到毫米(观测.中心坐标.z);

        const std::int64_t sx = std::max<std::int64_t>(1, 米到毫米(观测.尺寸.x));
        const std::int64_t sy = std::max<std::int64_t>(1, 米到毫米(观测.尺寸.y));
        const std::int64_t sz = std::max<std::int64_t>(1, 米到毫米(观测.尺寸.z));

        const std::int64_t hx = sx / 2;
        const std::int64_t hy = sy / 2;
        const std::int64_t hz = sz / 2;

        const std::int64_t xMin = cx - hx - param.位置容差_mm;
        const std::int64_t xMax = cx + hx + param.位置容差_mm;
        const std::int64_t yMin = cy - hy - param.位置容差_mm;
        const std::int64_t yMax = cy + hy + param.位置容差_mm;
        const std::int64_t zMin = cz - hz - param.位置容差_mm;
        const std::int64_t zMax = cz + hz + param.位置容差_mm;

        auto* smi = dynamic_cast<场景节点主信息类*>(当前场景->主信息);
        if (!smi) return nullptr;

        存在节点类* best = nullptr;
        int bestScore = -1;

        for (auto* e : smi->存在索引) {
            if (!e) continue;

            const std::int64_t ex = 取轴特征值_mm_已加锁(e, 类型_相对位置X, std::numeric_limits<std::int64_t>::min());
            const std::int64_t ey = 取轴特征值_mm_已加锁(e, 类型_相对位置Y, std::numeric_limits<std::int64_t>::min());
            const std::int64_t ez = 取轴特征值_mm_已加锁(e, 类型_相对位置Z, std::numeric_limits<std::int64_t>::min());
            if (ex == std::numeric_limits<std::int64_t>::min()) continue;

            if (ex < xMin || ex > xMax) continue;
            if (ey < yMin || ey > yMax) continue;
            if (ez < zMin || ez > zMax) continue;

            const int s = 比较匹配度_已加锁(e, cx, cy, cz, sx, sy, sz, param);
            if (s > bestScore) { bestScore = s; best = e; }
        }

        auto 写入观测到存在 = [&](存在节点类* e)
            {
                if (!e) return;

                if (auto* emi = dynamic_cast<存在节点主信息类*>(e->主信息)) {
                    emi->记录命中();
                    if (emi->创建时间 == 0) emi->创建时间 = 观测.时间;
                    emi->最后观测时间 = 观测.时间;
                }

                // 相对坐标（核心）
                写I64_已加锁(e, 类型_相对位置X, cx);
                写I64_已加锁(e, 类型_相对位置Y, cy);
                写I64_已加锁(e, 类型_相对位置Z, cz);

                // 尺寸（兼容写两套字段）
                写I64_已加锁(e, 类型_尺寸左右, sx);
                写I64_已加锁(e, 类型_尺寸上下, sy);
                写I64_已加锁(e, 类型_尺寸前后, sz);

                // 相对尺寸（三轴）
                写I64_已加锁(e, 类型_相对尺寸X, sx);
                写I64_已加锁(e, 类型_相对尺寸Y, sy);
                写I64_已加锁(e, 类型_相对尺寸Z, sz);

                // 颜色
                if (观测.平均颜色.r + 观测.平均颜色.g + 观测.平均颜色.b > 30) {
                    颜色_BGR8 c{ 观测.平均颜色.b, 观测.平均颜色.g, 观测.平均颜色.r };
                    写颜色_BGR8_已加锁(e, 类型_平均颜色, c);
                }

                // 轮廓（原始：可变尺寸 bit-block；兼容：单块可直接映射 Bits64）
                if (!观测.轮廓编码.empty()) {
                    if (观测.轮廓编码.size() == 1) {
                        Bits64 bits{}; bits.w = (std::uint64_t)观测.轮廓编码[0];
                        写Bits64_已加锁(e, 类型_轮廓, bits);
                    }
                    else {
                        std::vector<std::int64_t> blocks;
                        blocks.reserve(观测.轮廓编码.size());
                        for (auto w : 观测.轮廓编码) blocks.push_back((std::int64_t)(std::uint64_t)w);
                        写VecI64_已加锁(e, 类型_轮廓, blocks, 枚举_比较模式::海明距离);
                    }
                }

                // 3D 模型（体素占据金字塔）：写入多原型（稳定姿态原型的“立体状态”）
                写三维模型_已加锁(e, 观测);

                // 质量相关（缩放为整数，方便后续比较/阈值）
                写I64_已加锁(e, 类型_观测质量分, (std::int64_t)std::llround(观测.质量分 * 1000.0));
                写I64_已加锁(e, 类型_深度噪声_MAD, 米到毫米(观测.深度噪声_MAD));
                写I64_已加锁(e, 类型_PCA残差, 米到毫米(观测.PCA残差));
                写I64_已加锁(e, 类型_颜色一致性, (std::int64_t)std::llround(观测.颜色一致性 * 1000.0));



            };

        if (best) {
            写入观测到存在(best);
            return best;
        }

        if (!允许新建) return nullptr;

        // 新建存在
        auto* emi = new 存在节点主信息类();
        auto* e = 为场景添加存在_已加锁(当前场景, emi);
        if (!e) return nullptr;

        写入观测到存在(e);
        return e;
    }
    // --- 三维模型特征：体素占据金字塔（顶层 4x4x4=64bit，层间 *8）---
    void 写三维模型_已加锁(存在节点类* e, const 结构体_存在观测& 观测)
    {
        if (!e) return;
        if (!三维模型特征类型) return;
        if (观测.轮廓3D.empty()) return;

        // 1) 构建金字塔值
        const std::int64_t cx_mm = 米到毫米(观测.中心坐标.x);
        const std::int64_t cy_mm = 米到毫米(观测.中心坐标.y);
        const std::int64_t cz_mm = 米到毫米(观测.中心坐标.z);

        const std::int64_t sx_mm = std::max<std::int64_t>(1, 米到毫米(观测.尺寸.x));
        const std::int64_t sy_mm = std::max<std::int64_t>(1, 米到毫米(观测.尺寸.y));
        const std::int64_t sz_mm = std::max<std::int64_t>(1, 米到毫米(观测.尺寸.z));
        const std::int64_t edge_mm = std::max(sx_mm, std::max(sy_mm, sz_mm));

        体素占据金字塔 pyr;
        pyr.最大层 = 三维模型最大层;
        pyr.立方体边长_mm = edge_mm;
        pyr.min_x_mm = cx_mm - edge_mm / 2;
        pyr.min_y_mm = cy_mm - edge_mm / 2;
        pyr.min_z_mm = cz_mm - edge_mm / 2;

        auto 总块数 = [](std::uint8_t L)->std::size_t {
            std::size_t sum = 0;
            std::size_t bc = 1;
            for (std::uint8_t i = 0; i <= L; ++i) { sum += bc; bc *= 8; }
            return sum;
            };
        pyr.块.assign(总块数(pyr.最大层), 0ull);

        auto 偏移块 = [](std::uint8_t level)->std::size_t {
            std::size_t off = 0;
            std::size_t bc = 1;
            for (std::uint8_t i = 0; i < level; ++i) { off += bc; bc *= 8; }
            return off;
            };

        auto clamp01 = [](double x) {
            if (x < 0.0) return 0.0;
            if (x >= 1.0) return std::nextafter(1.0, 0.0);
            return x;
            };

        const double edge_m = 毫米到米(edge_mm);
        const double min_x_m = 毫米到米(pyr.min_x_mm);
        const double min_y_m = 毫米到米(pyr.min_y_mm);
        const double min_z_m = 毫米到米(pyr.min_z_mm);

        // 2) 逐点写入各层体素占据
        for (const auto& p3 : 观测.轮廓3D) {
            const double ux = clamp01((p3.x - min_x_m) / edge_m);
            const double uy = clamp01((p3.y - min_y_m) / edge_m);
            const double uz = clamp01((p3.z - min_z_m) / edge_m);

            for (std::uint8_t L = 0; L <= pyr.最大层; ++L) {
                const int g = 4 << L; // 4,8,16,...
                const int xi = std::min(g - 1, std::max(0, int(ux * g)));
                const int yi = std::min(g - 1, std::max(0, int(uy * g)));
                const int zi = std::min(g - 1, std::max(0, int(uz * g)));

                const std::size_t bit = (std::size_t(zi) * g + std::size_t(yi)) * g + std::size_t(xi);
                const std::size_t blk = bit >> 6;
                const std::size_t sh = bit & 63;

                const std::size_t off = 偏移块(L);
                if (off + blk < pyr.块.size()) {
                    pyr.块[off + blk] |= (1ull << sh);
                }
            }
        }

        // 3) 写入特征值节点
        特征值主信息类 mi = 创建体素占据金字塔(std::move(pyr), 枚举_比较模式::海明距离);
        auto* val = 特征值集.获取或创建_已加锁(mi);
        if (!val) return;

        // 4) 多原型管理：特征节点主信息升级为 三维模型特征主信息类
        auto* f = 在存在中查找特征类型_已加锁(e, 三维模型特征类型);
        if (!f) {
            auto* fmi = new 三维模型特征主信息类(三维模型特征类型, val);
            fmi->最大原型数 = 三维模型最大原型数;
            为存在添加特征_已加锁(e, fmi);
            return;
        }

        auto* model = dynamic_cast<三维模型特征主信息类*>(f->主信息);
        if (!model) {
            auto* base = dynamic_cast<特征节点主信息类*>(f->主信息);
            auto* fmi = new 三维模型特征主信息类(三维模型特征类型, base ? base->值 : nullptr);
            fmi->最大原型数 = 三维模型最大原型数;
            if (base && base->值) {
                fmi->状态原型.clear();
                fmi->状态命中.clear();
                fmi->状态原型.push_back(base->值);
                fmi->状态命中.push_back(1);
                fmi->值 = base->值;
            }
            delete f->主信息;
            f->主信息 = fmi;
            model = fmi;
        }

        // 5) 选择最相近原型
        std::size_t best_i = (std::size_t)-1;
        double best_d = 1e9;
        for (std::size_t i = 0; i < model->状态原型.size(); ++i) {
            auto* pv = model->状态原型[i];
            if (!pv || !pv->主信息 || !val->主信息) continue;
            const double d = 距离或差异度(*pv->主信息, *val->主信息);
            if (d < best_d) { best_d = d; best_i = i; }
        }

        if (best_i != (std::size_t)-1 && best_d <= 三维模型匹配阈值) {
            model->值 = model->状态原型[best_i];
            if (best_i >= model->状态命中.size()) model->状态命中.resize(model->状态原型.size(), 0);
            model->状态命中[best_i] += 1;
        }
        else {
            model->状态原型.push_back(val);
            model->状态命中.push_back(1);
            model->值 = val;

            // 简易裁剪：超过上限时剔除命中最小的（尽量不剔除当前）
            while (model->状态原型.size() > model->最大原型数 && model->状态原型.size() > 1) {
                std::size_t kill = 0;
                std::uint32_t best = 0xffffffffu;
                for (std::size_t i = 0; i < model->状态原型.size(); ++i) {
                    if (model->状态原型[i] == model->值) continue;
                    const std::uint32_t h = (i < model->状态命中.size()) ? model->状态命中[i] : 0;
                    if (h < best) { best = h; kill = i; }
                }
                if (model->状态原型[kill] == model->值) break;
                model->状态原型.erase(model->状态原型.begin() + kill);
                if (kill < model->状态命中.size()) model->状态命中.erase(model->状态命中.begin() + kill);
            }
        }
    }
    // ================= 批量处理（已加锁） =================
    // out可见：可选输出“本帧可见存在”（用于自我线程票据）
    void 处理观测列表_已加锁(const std::vector<结构体_存在观测>& 观测列表,
        const 匹配参数& p = {},
        std::vector<存在节点类*>* out可见 = nullptr)
    {
        本帧可见存在缓存.clear();
        本帧可见存在缓存.reserve(观测列表.size());

        for (const auto& obs : 观测列表) {
            if (auto* e = 找到或创建存在_已加锁(obs, p)) {
                if (!含有(本帧可见存在缓存, e)) {
                    本帧可见存在缓存.push_back(e);
                }
            }
        }

        if (out可见) {
            *out可见 = 本帧可见存在缓存;
        }

        // 注意：这里不刷新/不写入绝对坐标。
        // 绝对坐标（锚点、rel->abs 变换、缺失检测）由 自我线程 维护。
    }

    // ================= 兼容：无锁接口（内部加锁） =================
    存在节点类* 找到或创建存在(const 结构体_存在观测& 观测, const 匹配参数& p = {})
    {
        std::scoped_lock lock(世界链.链表锁, 特征值链.链表锁);
        return 找到或创建存在_已加锁(观测, p);
    }

    void 处理观测列表(const std::vector<结构体_存在观测>& 观测列表,
        const 匹配参数& p = {},
        std::vector<存在节点类*>* out可见 = nullptr)
    {
        std::scoped_lock lock(世界链.链表锁, 特征值链.链表锁);
        处理观测列表_已加锁(观测列表, p, out可见);
    }

    // ================================
    // B2 终极形态：外部一次性锁住 世界链 + 特征值链，然后调用 处理观测列表_已加锁
    // 这里提供一个“内部加锁”的便捷版本。
    // ================================
    void 处理观测列表_B2(const std::vector<结构体_存在观测>& 观测列表, const 匹配参数& p = {},
        std::vector<存在节点类*>* out可见 = nullptr)
    {
        std::scoped_lock lock(世界链.链表锁, 特征值链.链表锁);
        处理观测列表_已加锁(观测列表, p, out可见);
    }
};