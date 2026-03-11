module;

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <limits>
#include <cmath>
#include <mutex>

export module 三维场景管理模块;

// 新逻辑：
//  - 世界树是唯一入口（写特征 I64 / 写轮廓 VecIU64）
//  - 不再使用 VecI64 / Vec3I64 / 特征值节点指针
//  - 连续测量误差由“特征主信息里的误差区间”解决（仅 I64 特征），三维场景管理只负责写入测量

import 世界树环境模块;
import 基础数据类型模块;
import 主信息定义模块;
import 数据仓库模块;
import 特征模块;         // 写入参数/稳态策略
import 特征类型定义模块;   // 复用 类型_轮廓编码
import 语素环境模块;       // 用于创建 X/Y/Z 等 I64 特征类型（懒初始化）
import 存在提取模块;       // 结构体_存在观测

// 说明：
//  - 本类只做“存在关联 + 观测写入 + 过期清理”。
//  - 轮廓相似度等高阶计算应由世界树封装为标量接口（不泄露 VecU）。

export class 三维场景管理类 {
public:
    using I64 = std::int64_t;

    struct 匹配参数 {
        bool 启用跟踪 = true;

        // 粗匹配：中心距离阈值（mm）
        I64 最大中心距离_mm = 800;

        // AABB 相交容差（mm）
        I64 AABB容差_mm = 80;

        // 若跟踪ID丢失，允许兜底最近距离匹配
        bool 允许兜底最近匹配 = true;

        // 幽灵存在清理：连续未命中帧阈值（0 表示不清理）
        std::uint32_t 幽灵_连续未命中阈值 = 60;

        // 写入轮廓时的稳态策略（控制原型数量等）
        稳态策略 轮廓稳态{};

        匹配参数() {
            // 默认让轮廓稳态比较保守
            轮廓稳态.连续N = 2;
            轮廓稳态.阈值_判稳 = 1200; // Q10000 diff 门限（越小越像；具体量纲取决于世界树默认差异度实现）
            轮廓稳态.Kmax_points = 16;
       //     轮廓稳态.允许点到区间 = false;
        }
    };

    struct 帧处理统计 {
        std::uint64_t 帧号 = 0;
        std::size_t 观测数 = 0;
        std::size_t 命中数 = 0;
        std::size_t 新建数 = 0;
        std::size_t 删除数 = 0;
    };

public:
    三维场景管理类() {
        初始化特征类型_懒();
    }

    // ============================================================
    // 主入口：把一帧存在观测写入到世界树
    // - parentScene：这帧写入的目标场景节点（世界树中的场景）
    // ============================================================
    帧处理统计 处理观测列表(
        场景节点类* parentScene,
        const std::vector<结构体_存在观测>& list,
        const 匹配参数& p = {})
    {
        初始化特征类型_懒();

        ++帧序号_;
        帧处理统计 st{};
        st.帧号 = 帧序号_;
        st.观测数 = list.size();

        // 标记本帧命中
        for (auto& kv : 跟踪表_) {
            kv.second.本帧命中 = false;
        }

        // 本帧用于“非跟踪匹配”的候选集合：从当前场景枚举子存在
        std::vector<存在节点类*> sceneExists;
        if (parentScene) {
            sceneExists = 世界树.获取子存在(parentScene, "三维场景管理类::处理观测列表::枚举子存在");
        }

        for (const auto& obs : list) {
            if (!obs.坐标有效) continue;

            // 1) 找/建存在
            存在节点类* e = nullptr;
            if (p.启用跟踪 && obs.跟踪ID >= 0) {
                e = 私有_按跟踪ID命中(obs.跟踪ID);
            }

            if (!e && p.允许兜底最近匹配) {
                e = 私有_按空间粗匹配(parentScene, sceneExists, obs, p);
            }

            if (!e) {
                e = 私有_创建新存在(parentScene, obs);
                if (e) st.新建数++;
            }
            else {
                st.命中数++;
            }

            if (!e) continue;

            // 2) 回写跟踪表
            if (p.启用跟踪 && obs.跟踪ID >= 0) {
                auto& rec = 跟踪表_[obs.跟踪ID];
                rec.存在 = e;
                rec.最后时间 = obs.时间;
                rec.连续未命中 = 0;
                rec.本帧命中 = true;
                rec.中心_mm = { 量化_mm(obs.中心坐标.x), 量化_mm(obs.中心坐标.y), 量化_mm(obs.中心坐标.z) };
                rec.AABB_min_mm = { 量化_mm(obs.AABB_min.x), 量化_mm(obs.AABB_min.y), 量化_mm(obs.AABB_min.z) };
                rec.AABB_max_mm = { 量化_mm(obs.AABB_max.x), 量化_mm(obs.AABB_max.y), 量化_mm(obs.AABB_max.z) };
            }

            // 3) 更新存在主信息中的观测时间/运动统计（不依赖特征值节点）
            私有_更新存在主信息_运动(e, obs);

            // 4) 写入 I64 几何快照（x/y/z & w/h/d）
            写入参数 wp{};
            wp.ts = obs.时间;
            wp.仅更新当前值 = true;  // 几何量只作快照，不做稳态原型
            wp.允许创建稳态 = false;

            世界树.写入特征_I64(e, 类型_中心X, 量化_mm(obs.中心坐标.x), wp, "三维场景管理类::写中心X");
            世界树.写入特征_I64(e, 类型_中心Y, 量化_mm(obs.中心坐标.y), wp, "三维场景管理类::写中心Y");
            世界树.写入特征_I64(e, 类型_中心Z, 量化_mm(obs.中心坐标.z), wp, "三维场景管理类::写中心Z");

            世界树.写入特征_I64(e, 类型_尺寸X, 量化_mm(obs.尺寸.x), wp, "三维场景管理类::写尺寸X");
            世界树.写入特征_I64(e, 类型_尺寸Y, 量化_mm(obs.尺寸.y), wp, "三维场景管理类::写尺寸Y");
            世界树.写入特征_I64(e, 类型_尺寸Z, 量化_mm(obs.尺寸.z), wp, "三维场景管理类::写尺寸Z");

            // 5) 写入轮廓编码（VecIU64 证据池 + 句柄）
            if (!obs.轮廓编码.empty() && 特征类型定义类::类型_轮廓编码) {
                写入参数 wp2{};
                wp2.ts = obs.时间;
                wp2.仅更新当前值 = false;
                wp2.允许创建稳态 = true;
                wp2.稳态 = p.轮廓稳态;

                世界树.写入特征_VecU(e, 特征类型定义类::类型_轮廓编码, obs.轮廓编码, wp2, "三维场景管理类::写轮廓编码");
            }
        }

        // 6) 幽灵清理：对跟踪表里未命中的条目递增 miss，超过阈值则删除
        if (p.幽灵_连续未命中阈值 > 0) {
            std::vector<I64> toErase;
            for (auto& kv : 跟踪表_) {
                auto& rec = kv.second;
                if (rec.本帧命中) continue;
                rec.连续未命中++;
                if (rec.连续未命中 >= p.幽灵_连续未命中阈值) {
                    if (rec.存在) {
                        世界树.删除节点(rec.存在, "三维场景管理类::幽灵清理");
                        st.删除数++;
                    }
                    toErase.push_back(kv.first);
                }
            }
            for (auto id : toErase) 跟踪表_.erase(id);
        }

        return st;
    }

private:
    struct _Vec3I64 { I64 x = 0, y = 0, z = 0; };

    struct 跟踪记录 {
        存在节点类* 存在 = nullptr;
        时间戳 最后时间 = 0;
        std::uint32_t 连续未命中 = 0;
        bool 本帧命中 = false;
        _Vec3I64 中心_mm{};
        _Vec3I64 AABB_min_mm{};
        _Vec3I64 AABB_max_mm{};
    };

    std::unordered_map<I64, 跟踪记录> 跟踪表_;
    std::uint64_t 帧序号_ = 0;

    // ===== I64 特征类型（组件化） =====
    static inline const 词性节点类* 类型_中心X = nullptr;
    static inline const 词性节点类* 类型_中心Y = nullptr;
    static inline const 词性节点类* 类型_中心Z = nullptr;

    static inline const 词性节点类* 类型_尺寸X = nullptr;
    static inline const 词性节点类* 类型_尺寸Y = nullptr;
    static inline const 词性节点类* 类型_尺寸Z = nullptr;

    static inline std::once_flag g_onceTypes_;

    static void 初始化特征类型_懒() {
        std::call_once(g_onceTypes_, [] {
            // 词性节点用于“特征类型”，避免 VecI64 类型；全部用 I64 分量类型
            if (!类型_中心X) 类型_中心X = 语素集.添加词性词("中心X", "名词");
            if (!类型_中心Y) 类型_中心Y = 语素集.添加词性词("中心Y", "名词");
            if (!类型_中心Z) 类型_中心Z = 语素集.添加词性词("中心Z", "名词");

            if (!类型_尺寸X) 类型_尺寸X = 语素集.添加词性词("尺寸X", "名词");
            if (!类型_尺寸Y) 类型_尺寸Y = 语素集.添加词性词("尺寸Y", "名词");
            if (!类型_尺寸Z) 类型_尺寸Z = 语素集.添加词性词("尺寸Z", "名词");

            // 确保特征类型定义模块也初始化（轮廓编码等）
            特征类型定义类::初始化特征类型定义模块_依赖语素();
            });
    }

    static I64 量化_mm(double x) noexcept {
        if (!std::isfinite(x)) return 0;
        // 观测以 mm 为单位，统一四舍五入到整数 mm
        return (I64)std::llround(x);
    }

    static I64 abs_i64(I64 v) noexcept {
        if (v == (std::numeric_limits<I64>::min)()) return (std::numeric_limits<I64>::max)();
        return v < 0 ? -v : v;
    }

    static I64 近似距离_mm(const _Vec3I64& a, const _Vec3I64& b) noexcept {
        // L1 近似（更快，且用于粗筛足够）
        return abs_i64(a.x - b.x) + abs_i64(a.y - b.y) + abs_i64(a.z - b.z);
    }

    static bool AABB相交_容差(const _Vec3I64& aMin, const _Vec3I64& aMax, const _Vec3I64& bMin, const _Vec3I64& bMax, I64 tol) noexcept {
        return (aMin.x - tol) <= bMax.x && (aMax.x + tol) >= bMin.x &&
            (aMin.y - tol) <= bMax.y && (aMax.y + tol) >= bMin.y &&
            (aMin.z - tol) <= bMax.z && (aMax.z + tol) >= bMin.z;
    }

    存在节点类* 私有_按跟踪ID命中(I64 trackId) const {
        auto it = 跟踪表_.find(trackId);
        if (it == 跟踪表_.end()) return nullptr;
        return it->second.存在;
    }

    存在节点类* 私有_按空间粗匹配(
        场景节点类* parentScene,
        const std::vector<存在节点类*>& sceneExists,
        const 结构体_存在观测& obs,
        const 匹配参数& p)
    {
        (void)parentScene;

        const _Vec3I64 cObs{ 量化_mm(obs.中心坐标.x), 量化_mm(obs.中心坐标.y), 量化_mm(obs.中心坐标.z) };
        const _Vec3I64 aMinObs{ 量化_mm(obs.AABB_min.x), 量化_mm(obs.AABB_min.y), 量化_mm(obs.AABB_min.z) };
        const _Vec3I64 aMaxObs{ 量化_mm(obs.AABB_max.x), 量化_mm(obs.AABB_max.y), 量化_mm(obs.AABB_max.z) };

        // 1) 先优先用跟踪表中的 AABB 缓存做匹配（更便宜）
        I64 bestDist = (std::numeric_limits<I64>::max)();
        存在节点类* best = nullptr;

        for (auto& kv : 跟踪表_) {
            const auto& rec = kv.second;
            if (!rec.存在) continue;
            if (!AABB相交_容差(rec.AABB_min_mm, rec.AABB_max_mm, aMinObs, aMaxObs, p.AABB容差_mm)) continue;

            const I64 d = 近似距离_mm(rec.中心_mm, cObs);
            if (d < bestDist) {
                bestDist = d;
                best = rec.存在;
            }
        }

        if (best && bestDist <= p.最大中心距离_mm * 3) { // L1≈3*L2
            return best;
        }

        // 2) 兜底：枚举当前场景子存在（即使没有 trackId 也能匹配）
        //    这里只做“中心距离最小”粗筛，不读取特征值节点。
        for (auto* e : sceneExists) {
            if (!e || !e->主信息) continue;
            auto* mi = dynamic_cast<存在节点主信息类*>(e->主信息);
            if (!mi) continue;

            // 若有位置历史，则用最近观测位置
            _Vec3I64 cE{};
            if (mi->有位置历史) {
                cE = { 量化_mm(mi->最近观测位置_mm.x), 量化_mm(mi->最近观测位置_mm.y), 量化_mm(mi->最近观测位置_mm.z) };
            }
            else {
                continue;
            }

            const I64 d = 近似距离_mm(cE, cObs);
            if (d < bestDist) {
                bestDist = d;
                best = e;
            }
        }

        if (best && bestDist <= p.最大中心距离_mm * 3) return best;
        return nullptr;
    }

    存在节点类* 私有_创建新存在(场景节点类* parentScene, const 结构体_存在观测& obs) {
        auto* mi = new 存在节点主信息类();
        mi->创建时间 = obs.时间;
        mi->最后观测时间 = obs.时间;
        mi->可信度 = 10000;

        auto* e = 世界树.创建存在(parentScene, mi, "三维场景管理类::创建新存在");
        return e;
    }

    void 私有_更新存在主信息_运动(存在节点类* e, const 结构体_存在观测& obs) {
        if (!e || !e->主信息) return;
        auto* mi = dynamic_cast<存在节点主信息类*>(e->主信息);
        if (!mi) return;

        mi->最后观测时间 = obs.时间;

        // 更新位置历史（仍使用 Vector3D 存储，单位 mm）
        const Vector3D pos{ obs.中心坐标.x, obs.中心坐标.y, obs.中心坐标.z };
        if (!mi->有位置历史) {
            mi->有位置历史 = true;
            mi->上次观测位置_mm = pos;
            mi->最近观测位置_mm = pos;
            mi->最近位移_mm = 0;
            mi->连续静止帧 = 0;
        }
        else {
            mi->上次观测位置_mm = mi->最近观测位置_mm;
            mi->最近观测位置_mm = pos;

            const I64 dx = 量化_mm(mi->最近观测位置_mm.x - mi->上次观测位置_mm.x);
            const I64 dy = 量化_mm(mi->最近观测位置_mm.y - mi->上次观测位置_mm.y);
            const I64 dz = 量化_mm(mi->最近观测位置_mm.z - mi->上次观测位置_mm.z);
            const I64 move = abs_i64(dx) + abs_i64(dy) + abs_i64(dz);
            mi->最近位移_mm = move;
            if (move <= 40) mi->连续静止帧++; else mi->连续静止帧 = 0;
        }
    }
};
