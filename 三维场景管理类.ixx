
export module 三维场景管理模块;

// 统一单位：毫米（mm）
// 说明：
// - 观测输入（double）假定为“米”
// - 世界树/特征内部全部存储 int64 毫米
// - 显示阶段再 mm -> m

import <vector>;
import <cstdint>;
import <cmath>;
import <limits>;
import <algorithm>;
import <variant>;

import 世界树环境模块;
import 特征值环境模块;
import 主信息定义模块;
import 特征类型定义模块;




// ================= 三维场景管理类 =================
export class 三维场景管理类 {
public:
    // ================= 单位工具 =================
    static std::int64_t 米到毫米(double m) {
        return static_cast<std::int64_t>(std::llround(m * 1000.0));
    }
    static double 毫米到米(std::int64_t mm) {
        return static_cast<double>(mm) / 1000.0;
    }

    // ================= 特征辅助（毫米） =================
    // ================= 特征辅助（毫米） =================
    // 注意：新特征值系统使用「特征值主信息类(variant)」，不再依赖 标量/矢量/文本 等派生主信息类。
    static bool 轴特征值在范围内_mm(存在节点类* 存在, 词性节点类* 类型, std::int64_t minV, std::int64_t maxV)
    {
        if (!存在 || !类型) return false;
        特征节点类* f = 世界树.在存在中查找特征类型(存在, 类型);
        if (!f) return false;

        auto* info = dynamic_cast<特征节点主信息类*>(f->主信息);
        if (!info || !info->值) return false;

        // 若该特征积累了多次观测，值链上可能有多个历史值：逐个检查
        特征值节点类* v = info->值;
        do {
            auto* vmi = (v ? v->主信息 : nullptr);
            if (vmi && vmi->类型 == 枚举_特征值类型::I64) {
                if (auto* p = std::get_if<std::int64_t>(&vmi->值)) {
                    const std::int64_t val = *p; // mm
                    if (val >= minV && val <= maxV) return true;
                }
            }
            v = v->下;
        } while (v && v != info->值);

        return false;
    }

    static std::int64_t 轴取最近值_mm(存在节点类* 存在, 词性节点类* 类型, std::int64_t 目标)
    {
        if (!存在 || !类型) return 目标;
        特征节点类* f = 世界树.在存在中查找特征类型(存在, 类型);
        if (!f) return 目标;

        auto* info = dynamic_cast<特征节点主信息类*>(f->主信息);
        if (!info || !info->值) return 目标;

        std::int64_t bestDist = (std::numeric_limits<std::int64_t>::max)();
        std::int64_t bestVal = 目标;

        特征值节点类* v = info->值;
        do {
            auto* vmi = (v ? v->主信息 : nullptr);
            if (vmi && vmi->类型 == 枚举_特征值类型::I64) {
                if (auto* p = std::get_if<std::int64_t>(&vmi->值)) {
                    const std::int64_t val = *p;
                    const std::int64_t d = (val >= 目标) ? (val - 目标) : (目标 - val);
                    if (d < bestDist) { bestDist = d; bestVal = val; }
                }
            }
            v = v->下;
        } while (v && v != info->值);

        return bestVal;
    }
    场景节点类* 当前场景 = nullptr;

    explicit 三维场景管理类() {}

    存在节点类* 找到或创建存在(const 结构体_存在观测& 观测) {
        if (!当前场景)
            当前场景 = 世界树.自我所在场景;
        if (!当前场景)
            return nullptr;

        // ===== 0) 统一为毫米 =====
        const std::int64_t cx = 米到毫米(观测.中心坐标.x);
        const std::int64_t cy = 米到毫米(观测.中心坐标.y);
        const std::int64_t cz = 米到毫米(观测.中心坐标.z);

        const std::int64_t sx = std::max<std::int64_t>(1, 米到毫米(观测.尺寸.x));
        const std::int64_t sy = std::max<std::int64_t>(1, 米到毫米(观测.尺寸.y));
        const std::int64_t sz = std::max<std::int64_t>(1, 米到毫米(观测.尺寸.z));

        const std::int64_t hx = sx / 2;
        const std::int64_t hy = sy / 2;
        const std::int64_t hz = sz / 2;

        constexpr std::int64_t 容差_mm = 50; // 5cm

        const std::int64_t xMin = cx - hx - 容差_mm;
        const std::int64_t xMax = cx + hx + 容差_mm;
        const std::int64_t yMin = cy - hy - 容差_mm;
        const std::int64_t yMax = cy + hy + 容差_mm;
        const std::int64_t zMin = cz - hz - 容差_mm;
        const std::int64_t zMax = cz + hz + 容差_mm;

        // ===== 1) 候选 =====
        std::vector<存在节点类*> 候选;
        场景节点主信息类* 场景主信息 = dynamic_cast<场景节点主信息类*>(当前场景->主信息);
        for (auto* cur : 场景主信息->存在索引) {
            if (!cur) continue;

            bool ok =
                轴特征值在范围内_mm(cur, 类型_位置X, xMin, xMax) &&
                轴特征值在范围内_mm(cur, 类型_位置Y, yMin, yMax) &&
                轴特征值在范围内_mm(cur, 类型_位置Z, zMin, zMax);

            if (ok)
                候选.push_back(cur);
        }

        // ===== 2) 最近邻 =====
        if (!候选.empty()) {
            存在节点类* 最佳 = nullptr;
            std::int64_t 最佳距2 = (std::numeric_limits<std::int64_t>::max)();

            for (auto* e : 候选) {
                const std::int64_t ex = 轴取最近值_mm(e, 类型_位置X, cx);
                const std::int64_t ey = 轴取最近值_mm(e, 类型_位置Y, cy);
                const std::int64_t ez = 轴取最近值_mm(e, 类型_位置Z, cz);

                const std::int64_t dx = ex - cx;
                const std::int64_t dy = ey - cy;
                const std::int64_t dz = ez - cz;
                const std::int64_t d2 = dx * dx + dy * dy + dz * dz;

                if (d2 < 最佳距2) {
                    最佳距2 = d2;
                    最佳 = e;
                }
            }

            if (最佳) {
                return 最佳;
            }
        }

        // ===== 3) 新建 =====
        auto* 主信息 = new 存在节点主信息类();
        存在节点类* 新存在 = 世界树.为场景添加存在(当前场景, 主信息);

        auto 添加标量_mm = [&](词性节点类* 类型, std::int64_t v_mm) {
            世界树.为存在添加特征(新存在, new 特征节点主信息类(类型, 类型,
                特征值集.获取或创建标量特征值(nullptr, v_mm)
            )
            );
            };

        添加标量_mm(类型_位置X, cx);
        添加标量_mm(类型_位置Y, cy);
        添加标量_mm(类型_位置Z, cz);

        添加标量_mm(类型_尺寸左右, sx);
        添加标量_mm(类型_尺寸上下, sy);
        添加标量_mm(类型_尺寸前后, sz);

        if (!观测.轮廓编码.empty()) {
            auto* v = 特征值集.获取或创建矢量特征值(观测.轮廓编码);
            世界树.为存在添加特征(
                新存在,
                new 特征节点主信息类(类型_轮廓, 类型_轮廓, v)
            );
        }

        return 新存在;
    }
    // 辅助：轴特征范围检查
    // 辅助：轴特征范围检查（米）
    static bool 轴特征值在范围内(存在节点类* 存在, 词性节点类* 类型, double minV, double maxV) {
        return 轴特征值在范围内_mm(存在, 类型, 米到毫米(minV), 米到毫米(maxV));
    }

    static void 轴取最近值(存在节点类* 存在, 词性节点类* 类型, double 目标, double& 出值) {
        出值 = 毫米到米(轴取最近值_mm(存在, 类型, 米到毫米(目标)));
    }
};