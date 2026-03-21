module;
#include <memory>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include <chrono>
#include <limits>
#include <bit>
#include <variant>
#include <unordered_set>

export module 自我本能方法_相机认知模块;

// ============================================================================
// 自我本能方法_相机认知模块
// ----------------------------------------------------------------------------
// 你给出的自我本能方法：
//   读取帧特征信息 / 获取画面识别率 / 查找存在 / 生成存在 / 更新存在
//
// 本模块实现要点：
//   - 输入输出仍用“场景”，以便和任务/方法体系对齐。
//   - 查找/生成/更新都围绕 世界树（现实映射）进行；
//   - 本模块不直接调参；调参属于外设本能方法，且应由更高层自组织策略触发。
//   - Step1：确保方法首节点存在，并把“真正必需的参数类型”写入首节点条件模板。
// ============================================================================

import 基础数据类型模块;
import 本能动作管理模块;
import 本能动作模块;
import 方法环境模块;
import 世界树环境模块;
import 语素环境模块;
import 特征类型定义模块;
import 主信息定义模块;
import 外设本能方法_相机模块;   // 相机帧仓库_取只读()
import 相机写入工作流模块;       // 相机帧处理器（点簇分割/存在提取）
import 日志模块;
import 通用函数模块;

import 场景模块;
import 数据仓库模块;

export class 自我本能方法类_相机认知模块 {
public:
    自我本能方法类_相机认知模块() {}
    ~自我本能方法类_相机认知模块() {}

private:
    static inline 时间戳 当前_微秒() noexcept { return 结构体_时间戳::当前_微秒(); }
    static inline 外设本能方法类_相机模块 外设本能方法类_相机模块_{};
    // ----------------------------------------------------------------------------
    // Step1 通用：先查注册表里是否已有方法首节点指针；没有则创建，并回填到注册表
    // ----------------------------------------------------------------------------
    static 方法节点类* 私有_取或创建_方法首节点_并回填(枚举_本能动作ID id, const char* 调用点, 时间戳 now)
    {
        if (auto q = 本能集.查询(id)) {
            if (q->方法信息首节点) {
                方法集.初始化方法虚拟存在信息(q->方法信息首节点, now, 调用点);
                return q->方法信息首节点;
            }
        }

        auto* head = 方法集.查找或创建_本能方法首节点(static_cast<std::uint64_t>(id), now, 调用点);
        if (head) {
            方法集.初始化方法虚拟存在信息(head, now, 调用点);
            (void)本能集.补全方法信息(id, head);
        }
        return head;
    }

    struct 结构体_方法调用守卫 {
        方法节点类* 方法首节点 = nullptr;
        场景节点类* 输出场景 = nullptr;
        时间戳 now = 0;
        std::string 调用点{};
        bool 成功 = false;
        I64 错误码 = 0;

        结构体_方法调用守卫(方法节点类* head, 场景节点类* 输入场景, 场景节点类* out, 时间戳 ts, std::string fn)
            : 方法首节点(head), 输出场景(out), now(ts), 调用点(std::move(fn))
        {
            if (方法首节点) 方法集.标记方法调用开始(方法首节点, 输入场景, now, 调用点);
        }

        void 设置结果(bool ok, I64 err = 0)
        {
            成功 = ok;
            错误码 = err;
        }

        ~结构体_方法调用守卫()
        {
            if (方法首节点) 方法集.标记方法调用结束(方法首节点, 输出场景, 成功, 错误码, now, 调用点);
        }
    };

    static void 私有_确保方法形参(方法节点类* 方法首节点, std::initializer_list<const 词性节点类*> 需要类型)
    {
        if (!方法首节点) return;
        std::vector<const 词性节点类*> xs;
        xs.reserve(需要类型.size());
        for (auto* t : 需要类型) if (t) xs.push_back(t);
        if (xs.empty()) return;
        本能动作类::方法首节点_确保条件参数类型(方法首节点, xs);
    }

    // Vec3(mm) 编码到 VecIU64：按 I64 截断到 mm，再 bitcast 到 uint64_t
    static inline std::uint64_t bitcast_i64_to_u64(I64 x) noexcept { return std::bit_cast<std::uint64_t>(x); }
    static inline I64 bitcast_u64_to_i64(std::uint64_t x) noexcept { return std::bit_cast<I64>(x); }

    static inline VecIU64 编码Vec3I64(const Vector3D& v)
    {
        VecIU64 xs;
        xs.reserve(3);
        const I64 x = (I64)std::llround(v.x);
        const I64 y = (I64)std::llround(v.y);
        const I64 z = (I64)std::llround(v.z);
        xs.push_back(bitcast_i64_to_u64(x));
        xs.push_back(bitcast_i64_to_u64(y));
        xs.push_back(bitcast_i64_to_u64(z));
        return xs;
    }

    static inline bool 解码Vec3I64(const VecU句柄& h, Vector3D& out)
    {
        if (!h.有效()) return false;
        const VecIU64* xs = 世界树.值池().取VecU只读指针(h);
        if (!xs || xs->size() < 3) return false;
        out.x = (double)bitcast_u64_to_i64((*xs)[0]);
        out.y = (double)bitcast_u64_to_i64((*xs)[1]);
        out.z = (double)bitcast_u64_to_i64((*xs)[2]);
        return true;
    }

    static const 特征节点主信息类* 取特征主信息(const 基础信息节点类* n) {
        return (n && n->主信息) ? dynamic_cast<const 特征节点主信息类*>(n->主信息) : nullptr;
    }

    static bool 读I64_按类型(场景节点类* 输入, const 词性节点类* 类型, I64& out)
    {
        if (!输入 || !类型 || !输入->子) return false;
        auto* start = static_cast<基础信息节点类*>(输入->子);
        auto* cur = start;
        do {
            if (cur && cur->主信息 && cur->主信息->类型 == 类型) {
                auto* f = 取特征主信息(cur);
                if (!f) return false;
                if (!std::holds_alternative<I64>(f->当前快照)) return false;
                out = std::get<1>(f->当前快照);
                return true;
            }
            cur = cur ? static_cast<基础信息节点类*>(cur->下) : nullptr;
        } while (cur && cur != start);
        return false;
    }

    static void 写回执(场景节点类* 输出, bool 成功, I64 错误码)
    {
        if (!输出) return;
        static const 词性节点类* tOK = nullptr;
        static const 词性节点类* tEC = nullptr;
        static const 词性节点类* tTS = nullptr;
        if (!tOK) tOK = 语素集.添加词性词("回执_成功", "名词");
        if (!tEC) tEC = 语素集.添加词性词("回执_错误码", "名词");
        if (!tTS) tTS = 语素集.添加词性词("回执_时间戳", "名词");
        (void)世界树.写入特征_I64(输出, tOK, 成功 ? 1 : 0, {}, "自我本能方法_相机认知模块::写回执");
        (void)世界树.写入特征_I64(输出, tEC, 错误码, {}, "自我本能方法_相机认知模块::写回执");
        (void)世界树.写入特征_I64(输出, tTS, (I64)当前_微秒(), {}, "自我本能方法_相机认知模块::写回执");
    }

    // 读取存在的某个特征快照（I64 或 VecU句柄）
    static const 特征节点主信息类* 取存在特征主信息(存在节点类* e, const 词性节点类* t)
    {
        if (!e || !t) return nullptr;
        auto* fn = 世界树.查找子特征_按类型(e, t, "自我本能方法_相机认知模块::取存在特征");
        if (!fn || !fn->主信息) return nullptr;
        return dynamic_cast<const 特征节点主信息类*>(fn->主信息);
    }

    static bool 候选读取_轮廓句柄(存在节点类* cand, VecU句柄& out)
    {
        const auto* t = 特征类型定义类::类型_轮廓编码;
        auto* mi = 取存在特征主信息(cand, t);
        if (!mi) return false;
        if (!std::holds_alternative<VecU句柄>(mi->当前快照)) return false;
        out = std::get<2>(mi->当前快照);
        return true;
    }

    static bool 候选读取_中心(存在节点类* cand, Vector3D& out)
    {
        const auto* t = 特征类型定义类::类型_绝对位置;
        auto* mi = 取存在特征主信息(cand, t);
        if (!mi) return false;
        if (!std::holds_alternative<VecU句柄>(mi->当前快照)) return false;
        return 解码Vec3I64(std::get<2>(mi->当前快照), out);
    }

    static const 词性节点类* 私有_类型_跟踪ID()
    {
        static const 词性节点类* t跟踪ID = nullptr;
        if (!t跟踪ID) t跟踪ID = 语素集.添加词性词("跟踪ID", "名词");
        return t跟踪ID;
    }

    static bool 候选读取_跟踪ID(存在节点类* cand, I64& out)
    {
        auto* mi = 取存在特征主信息(cand, 私有_类型_跟踪ID());
        if (!mi) return false;
        if (!std::holds_alternative<I64>(mi->当前快照)) return false;
        out = std::get<1>(mi->当前快照);
        return true;
    }

    struct 结构_识别轮廓评估结果 {
        I64 候选总数 = 0;
        I64 参考总数 = 0;
        I64 命中数 = 0;
        I64 效率万分比 = 0;
        I64 完整度万分比 = 0;
        I64 综合识别率万分比 = 0;
        double 平均IoU = 0.0;
    };

    static bool 私有_轮廓向量转点阵图(const VecIU64& blocks, 点阵图& out)
    {
        out = {};
        if (blocks.empty()) return false;
        const std::size_t 像素数 = blocks.size() * 64ull;
        const auto 边长 = static_cast<std::int32_t>(std::llround(std::sqrt(static_cast<double>(像素数))));
        if (边长 <= 0) return false;
        if (static_cast<std::size_t>(边长) * static_cast<std::size_t>(边长) != 像素数) return false;
        out.宽 = 边长;
        out.高 = 边长;
        out.数据 = blocks;
        return true;
    }

    static bool 私有_轮廓句柄转点阵图(const VecU句柄& h, 点阵图& out)
    {
        if (!h.有效()) return false;
        const VecIU64* xs = 世界树.值池().取VecU只读指针(h);
        if (!xs) return false;
        return 私有_轮廓向量转点阵图(*xs, out);
    }

    static bool 私有_读取存在轮廓点阵图(存在节点类* e, 点阵图& out)
    {
        auto* mi = 取存在特征主信息(e, 特征类型定义类::类型_轮廓编码);
        if (!mi || !std::holds_alternative<VecU句柄>(mi->当前快照)) return false;
        return 私有_轮廓句柄转点阵图(std::get<2>(mi->当前快照), out);
    }

    static 结构_识别轮廓评估结果 私有_评估自我识别轮廓(
        场景节点类* candScene,
        const std::vector<存在节点类*>& 参考存在)
    {
        结构_识别轮廓评估结果 out{};
        if (!candScene) return out;

        std::vector<点阵图> 候选轮廓集{};
        for (auto* cand : 场景类::获取子存在(candScene)) {
            点阵图 图{};
            if (!私有_读取存在轮廓点阵图(cand, 图)) continue;
            候选轮廓集.push_back(std::move(图));
        }

        std::vector<点阵图> 参考轮廓集{};
        for (auto* e : 参考存在) {
            点阵图 图{};
            if (!私有_读取存在轮廓点阵图(e, 图)) continue;
            参考轮廓集.push_back(std::move(图));
        }

        out.候选总数 = static_cast<I64>(候选轮廓集.size());
        out.参考总数 = static_cast<I64>(参考轮廓集.size());
        if (候选轮廓集.empty() || 参考轮廓集.empty()) return out;

        double iou累计 = 0.0;
        for (const auto& refContour : 参考轮廓集) {
            double bestIou = 0.0;
            for (const auto& candContour : 候选轮廓集) {
                const double iou = 通用函数模块::计算_轮廓_相似度(refContour, candContour, true);
                if (iou > bestIou) bestIou = iou;
            }
            if (bestIou > 0.0) {
                out.命中数 += 1;
                iou累计 += bestIou;
            }
        }

        out.效率万分比 = (out.参考总数 > 0) ? (out.命中数 * 10000 / out.参考总数) : 0;
        out.完整度万分比 = (out.参考总数 > 0)
            ? static_cast<I64>(std::llround((iou累计 * 10000.0) / static_cast<double>(out.参考总数)))
            : 0;
        out.平均IoU = (out.命中数 > 0) ? (iou累计 / static_cast<double>(out.命中数)) : 0.0;
        out.综合识别率万分比 = (out.效率万分比 + out.完整度万分比) / 2;
        return out;
    }

    static 场景节点类* 私有_确保观察存在集场景(时间戳 now, const std::string& 调用点)
    {
        auto* root = 世界树.世界根();
        if (!root) return nullptr;
        auto* name = 语素集.添加词性词("观察存在集", "名词");
        auto* scene = 世界树.取或创建子场景_按名称(root, name, now, 调用点);
        if (scene) {
            世界树.自我所在场景 = scene;
        }
        return scene;
    }

    static void 私有_递归收集存在(基础信息节点类* parent, std::vector<存在节点类*>& out)
    {
        if (!parent) return;
        auto xs = 场景类::获取子存在(parent);
        out.insert(out.end(), xs.begin(), xs.end());
        for (auto* sc : 场景类::获取子场景(parent)) {
            私有_递归收集存在(sc, out);
        }
    }

    static std::vector<存在节点类*> 私有_枚举观察存在候选()
    {
        std::vector<存在节点类*> out{};
        auto* scene = 私有_确保观察存在集场景(当前_微秒(), "自我本能方法_相机认知模块::枚举观察存在候选");
        if (!scene) return out;
        私有_递归收集存在(scene, out);
        return out;
    }

    static 场景节点类* 私有_查找候选存在集场景(基础信息节点类* parent)
    {
        if (!parent) return nullptr;
        auto 子场景 = 场景类::获取子场景(parent);
        if (子场景.empty()) return nullptr;

        const auto* 名_候选集 = 语素集.添加词性词("候选存在集", "名词");
        const auto* 型_候选集场景 = 语素集.添加词性词("候选集场景", "名词");

        for (auto* sc : 子场景) {
            if (!sc || !sc->主信息) continue;
            auto* mi = dynamic_cast<场景节点主信息类*>(sc->主信息);
            if (!mi) continue;
            if (mi->名称 == 名_候选集 || mi->类型 == 型_候选集场景) return sc;
        }

        for (auto* sc : 子场景) {
            if (!sc) continue;
            if (!场景类::获取子存在(sc).empty()) return sc;
        }
        return 子场景[0];
    }

    static std::uint32_t 私有_观察存在允许连续未命中帧() noexcept
    {
        return 10;
    }

    static std::size_t 私有_衰减并清理未命中观察存在(
        const std::vector<存在节点类*>& 已有存在,
        const std::unordered_set<存在节点类*>& 本帧命中,
        const std::string& 调用点)
    {
        std::size_t 删除数 = 0;
        for (auto* e : 已有存在) {
            if (!e || !e->主信息) continue;
            if (本帧命中.find(e) != 本帧命中.end()) continue;
            auto* mi = dynamic_cast<存在节点主信息类*>(e->主信息);
            if (!mi) continue;
            mi->记录未命中();
            if (mi->连续未命中帧 >= 私有_观察存在允许连续未命中帧()) {
                if (世界树.删除节点(e, 调用点)) {
                    ++删除数;
                }
            }
        }
        return 删除数;
    }

    static 基础信息节点类* 私有_创建指代(基础信息节点类* parent, 指代节点主信息类* mi, const std::string& 调用点)
    {
        if (!parent || !mi) return nullptr;
        using namespace 数据仓库模块;
        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点) });
        return 世界链.添加子节点_已加锁(parent, static_cast<基础信息基类*>(mi));
    }



    // ============================================================================
    // 自我本能 1：读取帧特征信息
    // 输入：特征类型定义类::类型_深度帧句柄（I64）
    // 输出：
    //   - 写：宽/高/深度有效万分比
    //   - 写：候选存在集场景（子场景），其中每个候选存在挂上：绝对位置/尺寸/轮廓编码/跟踪ID
    // ============================================================================
public:
    bool 本能_读取帧特征信息(场景节点类* 输入场景, 场景节点类* 输出场景)
    {
        const 时间戳 now = 当前_微秒();
        特征类型定义类::初始化特征类型定义模块_依赖语素();

        auto* 方法首节点 = 私有_取或创建_方法首节点_并回填(
            枚举_本能动作ID::自我_读取帧特征信息, "本能_读取帧特征信息", now);
        结构体_方法调用守卫 守卫(方法首节点, 输入场景, 输出场景, now, "本能_读取帧特征信息");
        私有_确保方法形参(方法首节点, { 特征类型定义类::类型_深度帧句柄 });

        I64 h = 0;
        if (!读I64_按类型(输入场景, 特征类型定义类::类型_深度帧句柄, h) || h <= 0) {
            守卫.设置结果(false, -10);
            写回执(输出场景, false, -10);
            return false;
        }

        auto sp = 外设本能方法类_相机模块_.相机帧仓库_取只读(h);
        if (!sp) {
            守卫.设置结果(false, -11);
            写回执(输出场景, false, -11);
            return false;
        }

        const I64 W = (I64)sp->宽度;
        const I64 H = (I64)sp->高度;
        const I64 total = W * H;
        I64 valid = 0;
        if (!sp->深度有效.empty()) {
            for (auto v : sp->深度有效) valid += (v ? 1 : 0);
        }
        const I64 validRate = (total > 0) ? (valid * 10000 / total) : 0;

        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("帧_宽", "名词"), W, {}, "本能_读取帧特征信息");
        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("帧_高", "名词"), H, {}, "本能_读取帧特征信息");
        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("深度有效万分比", "名词"), validRate, {}, "本能_读取帧特征信息");

        相机帧处理器 proc{};
        帧处理结果 out{};
        out.原始场景帧 = std::const_pointer_cast<结构体_原始场景帧>(sp);

        const bool ok = proc.处理一帧(out);
        if (!ok) {
            守卫.设置结果(false, -20);
            写回执(输出场景, false, -20);
            return false;
        }

        static const 词性节点类* 名_候选集 = nullptr;
        static const 词性节点类* 型_候选集场景 = nullptr;
        static const 词性节点类* 名_候选存在 = nullptr;
        static const 词性节点类* 型_候选存在 = nullptr;
        if (!名_候选集) 名_候选集 = 语素集.添加词性词("候选存在集", "名词");
        if (!型_候选集场景) 型_候选集场景 = 语素集.添加词性词("候选集场景", "名词");
        if (!名_候选存在) 名_候选存在 = 语素集.添加词性词("候选存在", "名词");
        if (!型_候选存在) 型_候选存在 = 语素集.添加词性词("候选存在类型", "名词");

        auto* miSc = new 场景节点主信息类();
        miSc->名称 = 名_候选集;
        miSc->类型 = 型_候选集场景;
        miSc->最后观测时间 = now;
        auto* candScene = 世界树.创建场景(输出场景, miSc, "本能_读取帧特征信息/创建候选集场景");

        static const 词性节点类* t跟踪ID = nullptr;
        if (!t跟踪ID) t跟踪ID = 语素集.添加词性词("跟踪ID", "名词");

        for (const auto& obs : out.存在观测列表) {
            auto* miE = new 存在节点主信息类();
            miE->名称 = 名_候选存在;
            miE->类型 = 型_候选存在;
            miE->最后观测时间 = now;
            auto* e = 世界树.创建存在(candScene, miE, "本能_读取帧特征信息/创建候选存在");
            if (!e) continue;
            (void)世界树.写入特征_VecU(e, 特征类型定义类::类型_绝对位置, 编码Vec3I64(obs.中心坐标), {}, "本能_读取帧特征信息");
            (void)世界树.写入特征_VecU(e, 特征类型定义类::类型_尺寸, 编码Vec3I64(obs.尺寸), {}, "本能_读取帧特征信息");
            if (!obs.轮廓编码.empty()) {
                (void)世界树.写入特征_VecU(e, 特征类型定义类::类型_轮廓编码, obs.轮廓编码, {}, "本能_读取帧特征信息");
            }
            (void)世界树.写入特征_I64(e, t跟踪ID, (I64)obs.跟踪ID, {}, "本能_读取帧特征信息");
        }

        守卫.设置结果(true, 0);
        写回执(输出场景, true, 0);
        return true;
    }

    // ============================================================================
    // 自我本能 2：获取画面识别率
    // 输入：读取帧特征信息 的输出场景
    // 输出：识别率_万分比（I64）
    // ============================================================================
    bool 本能_获取画面识别率(场景节点类* 输入场景, 场景节点类* 输出场景)
    {
        const 时间戳 now = 当前_微秒();
        auto* tValid = 语素集.添加词性词("深度有效万分比", "名词");
        auto* t识别率 = 语素集.添加词性词("识别率_万分比", "名词");
        auto* t识别效率 = 语素集.添加词性词("识别效率_万分比", "名词");
        auto* t识别完整度 = 语素集.添加词性词("识别完整度_万分比", "名词");
        auto* t识别命中 = 语素集.添加词性词("识别命中数量", "名词");
        auto* t识别参考 = 语素集.添加词性词("识别参考数量", "名词");
        auto* t识别候选 = 语素集.添加词性词("识别候选数量", "名词");
        auto* 方法首节点 = 私有_取或创建_方法首节点_并回填(
            枚举_本能动作ID::自我_获取画面识别率, "本能_获取画面识别率", now);
        结构体_方法调用守卫 守卫(方法首节点, 输入场景, 输出场景, now, "本能_获取画面识别率");
        私有_确保方法形参(方法首节点, { tValid });

        I64 validRate = 0;
        (void)读I64_按类型(输入场景, tValid, validRate);
        场景节点类* candScene = 私有_查找候选存在集场景(输入场景);

        const auto 参考存在 = 私有_枚举观察存在候选();
        const auto contourEval = 私有_评估自我识别轮廓(candScene, 参考存在);
        const I64 recog = (validRate * contourEval.综合识别率万分比) / 10000;

        (void)世界树.写入特征_I64(输出场景, t识别率, recog, {}, "本能_获取画面识别率");
        (void)世界树.写入特征_I64(输出场景, t识别效率, contourEval.效率万分比, {}, "本能_获取画面识别率");
        (void)世界树.写入特征_I64(输出场景, t识别完整度, contourEval.完整度万分比, {}, "本能_获取画面识别率");
        (void)世界树.写入特征_I64(输出场景, t识别命中, contourEval.命中数, {}, "本能_获取画面识别率");
        (void)世界树.写入特征_I64(输出场景, t识别参考, contourEval.参考总数, {}, "本能_获取画面识别率");
        (void)世界树.写入特征_I64(输出场景, t识别候选, contourEval.候选总数, {}, "本能_获取画面识别率");
        守卫.设置结果(true, 0);
        写回执(输出场景, true, 0);
        return true;
    }

    // ============================================================================
    // 自我本能 3：查找存在
    // 输入：
    //   - 输入场景的子存在：候选存在（至少含 绝对位置/轮廓编码）
    // 输出：
    //   - 指代节点（指向匹配到的真实存在）
    //   - 匹配分（I64）
    // ============================================================================
    bool 本能_查找存在(场景节点类* 输入场景, 场景节点类* 输出场景)
    {
        const 时间戳 now = 当前_微秒();
        特征类型定义类::初始化特征类型定义模块_依赖语素();

        auto* 方法首节点 = 私有_取或创建_方法首节点_并回填(
            枚举_本能动作ID::自我_查找存在, "本能_查找存在", now);
        结构体_方法调用守卫 守卫(方法首节点, 输入场景, 输出场景, now, "本能_查找存在");
        私有_确保方法形参(方法首节点, {
            特征类型定义类::类型_绝对位置,
            特征类型定义类::类型_轮廓编码
            });

        auto cands = 场景类::获取子存在(输入场景);
        if (cands.empty()) { 守卫.设置结果(false, -10); 写回执(输出场景, false, -10); return false; }
        auto* cand = cands[0];

        Vector3D cPos{};
        VecU句柄 cContour{};
        I64 cTrackId = 0;
        const bool hasPos = 候选读取_中心(cand, cPos);
        const bool hasContour = 候选读取_轮廓句柄(cand, cContour);
        const bool hasTrackId = 候选读取_跟踪ID(cand, cTrackId);
        auto 已观测存在 = 私有_枚举观察存在候选();
        if (已观测存在.empty()) { 守卫.设置结果(false, -11); 写回执(输出场景, false, -11); return false; }

        I64 bestScore = std::numeric_limits<I64>::min();
        存在节点类* best = nullptr;
        for (auto* e : 已观测存在) {
            if (!e || !e->主信息) continue;

            I64 score = 0;
            I64 eTrackId = 0;
            if (hasTrackId && 候选读取_跟踪ID(e, eTrackId) && eTrackId == cTrackId) {
                score += 200000;
            }

            auto* emi = 取存在特征主信息(e, 特征类型定义类::类型_轮廓编码);
            if (hasContour && emi && std::holds_alternative<VecU句柄>(emi->当前快照)) {
                const auto eh = std::get<2>(emi->当前快照);
                if (eh.主信息指针 == cContour.主信息指针) score += 100000;
                else {
                    const VecIU64* eContourVec = 世界树.值池().取VecU只读指针(eh);
                    const VecIU64* cContourVec = 世界树.值池().取VecU只读指针(cContour);
                    if (eContourVec && cContourVec) {
                        if (*eContourVec == *cContourVec) {
                            score += 60000;
                        }
                        else {
                            点阵图 eMask{};
                            点阵图 cMask{};
                            if (私有_轮廓向量转点阵图(*eContourVec, eMask) &&
                                私有_轮廓向量转点阵图(*cContourVec, cMask)) {
                                const double iou = 通用函数模块::计算_轮廓_相似度(eMask, cMask, true);
                                const double sim = 通用函数模块::计算_轮廓_相似度(eMask, cMask, false);
                                score += static_cast<I64>(std::llround(iou * 80000.0));
                                score += static_cast<I64>(std::llround(sim * 20000.0));
                            }
                        }
                    }
                }
            }
            if (hasPos) {
                auto* pmi = 取存在特征主信息(e, 特征类型定义类::类型_绝对位置);
                if (pmi && std::holds_alternative<VecU句柄>(pmi->当前快照)) {
                    Vector3D ePos{};
                    if (解码Vec3I64(std::get<2>(pmi->当前快照), ePos)) {
                        const double dx = ePos.x - cPos.x;
                        const double dy = ePos.y - cPos.y;
                        const double dz = ePos.z - cPos.z;
                        const double d2 = dx * dx + dy * dy + dz * dz;
                        score += (I64)std::max<double>(0.0, 50000.0 - d2 * 0.05);
                    }
                }
            }
            if (score > bestScore) { bestScore = score; best = e; }
        }

        if (!best || bestScore < 10000) { 守卫.设置结果(false, -20); 写回执(输出场景, false, -20); return false; }

        auto* miRef = new 指代节点主信息类();
        miRef->名称 = 语素集.添加词性词("匹配存在", "名词");
        miRef->类型 = 语素集.添加词性词("指代", "名词");
        miRef->指代对象 = best;
        (void)私有_创建指代(输出场景, miRef, "本能_查找存在/输出指代");
        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("匹配分", "名词"), bestScore, {}, "本能_查找存在");
        守卫.设置结果(true, 0);
        写回执(输出场景, true, 0);
        return true;
    }

    // ============================================================================
    // 自我本能 4：生成存在
    // 输入：候选存在（子存在）
    // 输出：指代节点（指向新创建的真实存在）
    // ============================================================================
    bool 本能_生成存在(场景节点类* 输入场景, 场景节点类* 输出场景)
    {
        const 时间戳 now = 当前_微秒();
        特征类型定义类::初始化特征类型定义模块_依赖语素();

        auto* 方法首节点 = 私有_取或创建_方法首节点_并回填(
            枚举_本能动作ID::自我_生成存在, "本能_生成存在", now);
        结构体_方法调用守卫 守卫(方法首节点, 输入场景, 输出场景, now, "本能_生成存在");
        私有_确保方法形参(方法首节点, {
            特征类型定义类::类型_绝对位置,
            特征类型定义类::类型_尺寸,
            特征类型定义类::类型_轮廓编码
            });

        auto cands = 场景类::获取子存在(输入场景);
        if (cands.empty()) { 守卫.设置结果(false, -10); 写回执(输出场景, false, -10); return false; }
        auto* cand = cands[0];

        auto* observeRoot = 私有_确保观察存在集场景(now, "本能_生成存在/确保观察存在集场景");
        if (!observeRoot) { 守卫.设置结果(false, -11); 写回执(输出场景, false, -11); return false; }

        static const 词性节点类* 名_外部存在 = nullptr;
        static const 词性节点类* 型_外部存在 = nullptr;
        if (!名_外部存在) 名_外部存在 = 语素集.添加词性词("外部存在", "名词");
        if (!型_外部存在) 型_外部存在 = 语素集.添加词性词("外部存在类型", "名词");

        auto* miE = new 存在节点主信息类();
        miE->名称 = 名_外部存在;
        miE->类型 = 型_外部存在;
        miE->最后观测时间 = now;
        auto* e = 世界树.创建存在(observeRoot, miE, "本能_生成存在");
        if (!e) { 守卫.设置结果(false, -20); 写回执(输出场景, false, -20); return false; }

        if (auto* mi = 取存在特征主信息(cand, 特征类型定义类::类型_绝对位置)) {
            if (std::holds_alternative<VecU句柄>(mi->当前快照)) {
                const auto h = std::get<2>(mi->当前快照);
                if (const VecIU64* xs = 世界树.值池().取VecU只读指针(h)) {
                    (void)世界树.写入特征_VecU(e, 特征类型定义类::类型_绝对位置, *xs, {}, "本能_生成存在");
                }
            }
        }
        if (auto* mi = 取存在特征主信息(cand, 特征类型定义类::类型_尺寸)) {
            if (std::holds_alternative<VecU句柄>(mi->当前快照)) {
                const auto h = std::get<2>(mi->当前快照);
                if (const VecIU64* xs = 世界树.值池().取VecU只读指针(h)) {
                    (void)世界树.写入特征_VecU(e, 特征类型定义类::类型_尺寸, *xs, {}, "本能_生成存在");
                }
            }
        }
        if (auto* mi = 取存在特征主信息(cand, 特征类型定义类::类型_轮廓编码)) {
            if (std::holds_alternative<VecU句柄>(mi->当前快照)) {
                const auto h = std::get<2>(mi->当前快照);
                if (const VecIU64* xs = 世界树.值池().取VecU只读指针(h)) {
                    (void)世界树.写入特征_VecU(e, 特征类型定义类::类型_轮廓编码, *xs, {}, "本能_生成存在");
                }
            }
        }
        if (auto* mi = 取存在特征主信息(cand, 私有_类型_跟踪ID())) {
            if (std::holds_alternative<I64>(mi->当前快照)) {
                (void)世界树.写入特征_I64(e, 私有_类型_跟踪ID(), std::get<1>(mi->当前快照), {}, "本能_生成存在");
            }
        }
        (void)世界树.写入特征_I64(e, 特征类型定义类::类型_时间戳_us, (I64)now, {}, "本能_生成存在");

        auto* miRef = new 指代节点主信息类();
        miRef->名称 = 语素集.添加词性词("新存在", "名词");
        miRef->类型 = 语素集.添加词性词("指代", "名词");
        miRef->指代对象 = e;
        (void)私有_创建指代(输出场景, miRef, "本能_生成存在/输出指代");
        守卫.设置结果(true, 0);
        写回执(输出场景, true, 0);
        return true;
    }

    // ============================================================================
    // 自我本能 5：更新存在
    // 输入：
    //   - 一个指代节点（指向要更新的真实存在）
    //   - 一个候选存在（子存在）
    // 输出：回执
    // ============================================================================
    bool 本能_更新存在(场景节点类* 输入场景, 场景节点类* 输出场景)
    {
        const 时间戳 now = 当前_微秒();
        特征类型定义类::初始化特征类型定义模块_依赖语素();

        auto* 方法首节点 = 私有_取或创建_方法首节点_并回填(
            枚举_本能动作ID::自我_更新存在, "本能_更新存在", now);
        结构体_方法调用守卫 守卫(方法首节点, 输入场景, 输出场景, now, "本能_更新存在");

        static const 词性节点类* t存在指代 = nullptr;
        if (!t存在指代) t存在指代 = 语素集.添加词性词("存在指代", "名词");

        私有_确保方法形参(方法首节点, {
            t存在指代,
            特征类型定义类::类型_绝对位置,
            特征类型定义类::类型_尺寸,
            特征类型定义类::类型_轮廓编码
            });

        存在节点类* target = nullptr;
        if (输入场景 && 输入场景->子) {
            auto* start = static_cast<基础信息节点类*>(输入场景->子);
            auto* cur = start;
            do {
                if (cur && cur->主信息) {
                    if (auto* ref = dynamic_cast<指代节点主信息类*>(cur->主信息)) {
                        target = ref->指代对象;
                        break;
                    }
                }
                cur = cur ? static_cast<基础信息节点类*>(cur->下) : nullptr;
            } while (cur && cur != start);
        }
        if (!target) { 守卫.设置结果(false, -10); 写回执(输出场景, false, -10); return false; }

        auto cands = 场景类::获取子存在(输入场景);
        if (cands.empty()) { 守卫.设置结果(false, -11); 写回执(输出场景, false, -11); return false; }
        auto* cand = cands[0];

        auto copyVecU = [&](const 词性节点类* t) {
            auto* mi = 取存在特征主信息(cand, t);
            if (!mi || !std::holds_alternative<VecU句柄>(mi->当前快照)) return;
            const VecU句柄 h = std::get<2>(mi->当前快照);
            if (const VecIU64* xs = 世界树.值池().取VecU只读指针(h)) {
                (void)世界树.写入特征_VecU(target, t, *xs, {}, "本能_更新存在");
            }
        };

        copyVecU(特征类型定义类::类型_绝对位置);
        copyVecU(特征类型定义类::类型_尺寸);
        copyVecU(特征类型定义类::类型_轮廓编码);
        auto* 跟踪ID特征 = 取存在特征主信息(cand, 私有_类型_跟踪ID());
        if (跟踪ID特征 && std::holds_alternative<I64>(跟踪ID特征->当前快照)) {
            (void)世界树.写入特征_I64(target, 私有_类型_跟踪ID(), std::get<1>(跟踪ID特征->当前快照), {}, "本能_更新存在");
        }
        if (auto* targetMi = dynamic_cast<存在节点主信息类*>(target->主信息)) {
            targetMi->记录观测(now);
        }
        (void)世界树.写入特征_I64(target, 特征类型定义类::类型_时间戳_us, (I64)now, {}, "本能_更新存在");
        守卫.设置结果(true, 0);
        写回执(输出场景, true, 0);
        return true;
    }

    static 存在节点类* 私有_读取指代对象(场景节点类* 输入场景)
    {
        if (!输入场景 || !输入场景->子) return nullptr;
        auto* start = static_cast<基础信息节点类*>(输入场景->子);
        auto* cur = start;
        do {
            if (cur && cur->主信息) {
                if (auto* ref = dynamic_cast<指代节点主信息类*>(cur->主信息)) return ref->指代对象;
            }
            cur = cur ? static_cast<基础信息节点类*>(cur->下) : nullptr;
        } while (cur && cur != start);
        return nullptr;
    }

    static 存在节点类* 私有_复制候选存在到场景(存在节点类* src, 场景节点类* dst, 时间戳 now, const std::string& 调用点)
    {
        if (!src || !dst || !src->主信息) return nullptr;
        auto* srcMi = dynamic_cast<存在节点主信息类*>(src->主信息);
        auto* mi = new 存在节点主信息类();
        if (srcMi) {
            mi->名称 = srcMi->名称;
            mi->类型 = srcMi->类型;
            mi->最后观测时间 = now;
        }
        auto* out = 世界树.创建存在(dst, mi, 调用点);
        if (!out) return nullptr;

        auto copyVecU = [&](const 词性节点类* t) {
            auto* f = 取存在特征主信息(src, t);
            if (!f || !std::holds_alternative<VecU句柄>(f->当前快照)) return;
            const auto h = std::get<2>(f->当前快照);
            if (const VecIU64* xs = 世界树.值池().取VecU只读指针(h)) {
                (void)世界树.写入特征_VecU(out, t, *xs, {}, 调用点);
            }
        };
        auto copyI64 = [&](const 词性节点类* t) {
            auto* f = 取存在特征主信息(src, t);
            if (!f || !std::holds_alternative<I64>(f->当前快照)) return;
            (void)世界树.写入特征_I64(out, t, std::get<1>(f->当前快照), {}, 调用点);
        };

        copyVecU(特征类型定义类::类型_绝对位置);
        copyVecU(特征类型定义类::类型_尺寸);
        copyVecU(特征类型定义类::类型_轮廓编码);
        copyI64(语素集.添加词性词("跟踪ID", "名词"));
        return out;
    }

    bool 本能_观察场景(场景节点类* 输入场景, 场景节点类* 输出场景)
    {
        const 时间戳 now = 当前_微秒();
        特征类型定义类::初始化特征类型定义模块_依赖语素();

        auto* 方法首节点 = 私有_取或创建_方法首节点_并回填(
            枚举_本能动作ID::自我_观察场景, "本能_观察场景", now);
        结构体_方法调用守卫 守卫(方法首节点, 输入场景, 输出场景, now, "本能_观察场景");
        私有_确保方法形参(方法首节点, {});

        auto 建场景 = [&](const char* 名称, const char* 类型, const std::string& 调用点) {
            auto* mi = new 场景节点主信息类();
            mi->名称 = 语素集.添加词性词(名称, "名词");
            mi->类型 = 语素集.添加词性词(类型, "名词");
            mi->最后观测时间 = now;
            return 世界树.创建场景(输出场景, mi, 调用点);
        };

        场景节点类* 预置候选输入 = nullptr;
        if (输入场景 && 私有_查找候选存在集场景(输入场景)) {
            预置候选输入 = 输入场景;
        }

        场景节点类* 读帧输出 = 预置候选输入
            ? 预置候选输入
            : 建场景("观察_候选事实输出", "观察过程场景", "本能_观察场景/候选事实输出");

        bool gotFacts = (预置候选输入 != nullptr);
        if (!gotFacts) {
            gotFacts = 读帧输出
                && 本能集.有(枚举_本能动作ID::外设_提取前景存在候选事实)
                && 本能集.调用(枚举_本能动作ID::外设_提取前景存在候选事实, 输入场景, 读帧输出);
        }
        if (!gotFacts) {
            auto* 取帧输出 = 建场景("观察_取帧输出", "观察过程场景", "本能_观察场景/取帧输出");
            bool gotFrame = 取帧输出 && 本能集.调用(枚举_本能动作ID::外设_获取帧信息, 输入场景, 取帧输出);
            if (!gotFrame) {
                日志::运行(
                    "[自我观察场景] 当前阶段禁止自我主动启动相机：未收到主窗口桥接候选，也未获取到现成帧");
                守卫.设置结果(false, -10);
                写回执(输出场景, false, -10);
                return false;
            }
            if (!gotFrame) { 守卫.设置结果(false, -11); 写回执(输出场景, false, -11); return false; }

            I64 handle = 0;
            if (!读I64_按类型(取帧输出, 特征类型定义类::类型_深度帧句柄, handle) || handle <= 0) {
                auto* t句柄帧 = 语素集.添加词性词("句柄_帧", "名词");
                if (!读I64_按类型(取帧输出, t句柄帧, handle) || handle <= 0) {
                    守卫.设置结果(false, -12);
                    写回执(输出场景, false, -12);
                    return false;
                }
            }

            auto* 读帧输入 = 建场景("观察_读帧输入", "观察过程场景", "本能_观察场景/读帧输入");
            读帧输出 = 建场景("观察_读帧输出", "观察过程场景", "本能_观察场景/读帧输出");
            if (!读帧输入 || !读帧输出) { 守卫.设置结果(false, -13); 写回执(输出场景, false, -13); return false; }
            (void)世界树.写入特征_I64(读帧输入, 特征类型定义类::类型_深度帧句柄, handle, {}, "本能_观察场景");
            if (!本能集.调用(枚举_本能动作ID::自我_读取帧特征信息, 读帧输入, 读帧输出)) {
                守卫.设置结果(false, -14);
                写回执(输出场景, false, -14);
                return false;
            }
        }

        auto* 识别率输出 = 建场景("观察_识别率输出", "观察过程场景", "本能_观察场景/识别率输出");
        I64 recog = 0;
        I64 recogEfficiency = 0;
        I64 recogCompleteness = 0;
        if (识别率输出 && 本能集.调用(枚举_本能动作ID::自我_获取画面识别率, 读帧输出, 识别率输出)) {
            (void)读I64_按类型(识别率输出, 语素集.添加词性词("识别率_万分比", "名词"), recog);
            (void)读I64_按类型(识别率输出, 语素集.添加词性词("识别效率_万分比", "名词"), recogEfficiency);
            (void)读I64_按类型(识别率输出, 语素集.添加词性词("识别完整度_万分比", "名词"), recogCompleteness);
            (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("识别率_万分比", "名词"), recog, {}, "本能_观察场景");
            (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("识别效率_万分比", "名词"), recogEfficiency, {}, "本能_观察场景");
            (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("识别完整度_万分比", "名词"), recogCompleteness, {}, "本能_观察场景");
        }

        场景节点类* candScene = 私有_查找候选存在集场景(读帧输出);
        if (!candScene) {
            (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("观察候选数量", "名词"), 0, {}, "本能_观察场景");
            守卫.设置结果(true, 0);
            写回执(输出场景, true, 0);
            return true;
        }

        I64 observed = 0;
        I64 updated = 0;
        I64 created = 0;
        I64 removed = 0;
        auto 旧观察存在 = 私有_枚举观察存在候选();
        std::unordered_set<存在节点类*> 本帧命中存在{};
        std::unordered_set<存在节点类*> 已输出识别结果{};
        auto* 识别结果场景 = 建场景("观察_识别结果", "观察结果场景", "本能_观察场景/识别结果场景");
        for (auto* cand : 场景类::获取子存在(candScene)) {
            if (!cand) continue;
            observed += 1;
            auto* 查找输入 = 建场景("观察_查找输入", "观察过程场景", "本能_观察场景/查找输入");
            auto* 查找输出 = 建场景("观察_查找输出", "观察过程场景", "本能_观察场景/查找输出");
            if (!查找输入 || !查找输出) continue;
            if (!私有_复制候选存在到场景(cand, 查找输入, now, "本能_观察场景/复制候选到查找输入")) continue;

            const bool found = 本能集.调用(枚举_本能动作ID::自我_查找存在, 查找输入, 查找输出);
            auto* target = found ? 私有_读取指代对象(查找输出) : nullptr;
            if (target) {
                auto* 更新输入 = 建场景("观察_更新输入", "观察过程场景", "本能_观察场景/更新输入");
                auto* 更新输出 = 建场景("观察_更新输出", "观察过程场景", "本能_观察场景/更新输出");
                if (!更新输入 || !更新输出) continue;
                auto* miRef = new 指代节点主信息类();
                miRef->名称 = 语素集.添加词性词("存在指代", "名词");
                miRef->类型 = 语素集.添加词性词("指代", "名词");
                miRef->指代对象 = target;
                (void)私有_创建指代(更新输入, miRef, "本能_观察场景/更新输入指代");
                (void)私有_复制候选存在到场景(cand, 更新输入, now, "本能_观察场景/复制候选到更新输入");
                if (本能集.调用(枚举_本能动作ID::自我_更新存在, 更新输入, 更新输出)) {
                    updated += 1;
                    (void)本帧命中存在.insert(target);
                    if (识别结果场景 && 已输出识别结果.insert(target).second) {
                        auto* outRef = new 指代节点主信息类();
                        outRef->名称 = 语素集.添加词性词("观察识别存在", "名词");
                        outRef->类型 = 语素集.添加词性词("指代", "名词");
                        outRef->指代对象 = target;
                        (void)私有_创建指代(识别结果场景, outRef, "本能_观察场景/输出已更新存在");
                    }
                }
            }
            else {
                auto* 生成输入 = 建场景("观察_生成输入", "观察过程场景", "本能_观察场景/生成输入");
                auto* 生成输出 = 建场景("观察_生成输出", "观察过程场景", "本能_观察场景/生成输出");
                if (!生成输入 || !生成输出) continue;
                if (!私有_复制候选存在到场景(cand, 生成输入, now, "本能_观察场景/复制候选到生成输入")) continue;
                if (本能集.调用(枚举_本能动作ID::自我_生成存在, 生成输入, 生成输出)) {
                    created += 1;
                    auto* newTarget = 私有_读取指代对象(生成输出);
                    if (newTarget) {
                        (void)本帧命中存在.insert(newTarget);
                    }
                    if (识别结果场景 && newTarget && 已输出识别结果.insert(newTarget).second) {
                        auto* outRef = new 指代节点主信息类();
                        outRef->名称 = 语素集.添加词性词("观察识别存在", "名词");
                        outRef->类型 = 语素集.添加词性词("指代", "名词");
                        outRef->指代对象 = newTarget;
                        (void)私有_创建指代(识别结果场景, outRef, "本能_观察场景/输出新建存在");
                    }
                }
            }
        }

        removed = (I64)私有_衰减并清理未命中观察存在(
            旧观察存在,
            本帧命中存在,
            "本能_观察场景/清理失联观察存在");
        const auto 当前观察存在 = 私有_枚举观察存在候选();
        const auto 最终轮廓评估 = 私有_评估自我识别轮廓(candScene, 当前观察存在);
        recogEfficiency = 最终轮廓评估.效率万分比;
        recogCompleteness = 最终轮廓评估.完整度万分比;
        recog = 最终轮廓评估.综合识别率万分比;

        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("观察候选数量", "名词"), observed, {}, "本能_观察场景");
        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("观察更新数量", "名词"), updated, {}, "本能_观察场景");
        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("观察新建数量", "名词"), created, {}, "本能_观察场景");
        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("观察清理数量", "名词"), removed, {}, "本能_观察场景");
        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("观察当前存在数量", "名词"), (I64)当前观察存在.size(), {}, "本能_观察场景");
        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("观察识别存在数量", "名词"), (I64)已输出识别结果.size(), {}, "本能_观察场景");
        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("识别率_万分比", "名词"), recog, {}, "本能_观察场景");
        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("识别效率_万分比", "名词"), recogEfficiency, {}, "本能_观察场景");
        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("识别完整度_万分比", "名词"), recogCompleteness, {}, "本能_观察场景");
        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("识别命中数量", "名词"), 最终轮廓评估.命中数, {}, "本能_观察场景");
        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("识别参考数量", "名词"), 最终轮廓评估.参考总数, {}, "本能_观察场景");
        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("识别候选数量", "名词"), 最终轮廓评估.候选总数, {}, "本能_观察场景");
        日志::运行f(
            "[本能_观察场景] 候选={}, 更新={}, 新建={}, 清理={}, 当前存在={}, 识别存在={}, 识别率={}, 效率={}, 完整度={}",
            observed,
            updated,
            created,
            removed,
            (I64)当前观察存在.size(),
            (I64)已输出识别结果.size(),
            recog,
            recogEfficiency,
            recogCompleteness);
        守卫.设置结果(true, 0);
        写回执(输出场景, true, 0);
        return true;
    }

    // ============================================================================
    // 注册：自我相机认知本能方法
    // ============================================================================
    void 注册默认本能动作_自我相机认知()
    {
        const 时间戳 now = 当前_微秒();

        auto reg = [this, now](const 枚举_本能动作ID id, 本能函数 fn, const char* name) {
            auto* head = 私有_取或创建_方法首节点_并回填(id, "注册默认本能动作_自我相机认知", now);
            if (head) 方法集.初始化方法虚拟存在信息(head, now, "注册默认本能动作_自我相机认知");
            if (!本能集.有(id)) {
                (void)本能集.注册(id, std::move(fn), name, head);
            }
            else {
                (void)本能集.补全方法信息(id, head);
            }
            };

        reg(
            枚举_本能动作ID::自我_观察场景,
            [this](场景节点类* 输入场景, 场景节点类* 输出场景) -> bool {
                return this->本能_观察场景(输入场景, 输出场景);
            },
            "本能_观察场景"
        );

        reg(
            枚举_本能动作ID::自我_读取帧特征信息,
            [this](场景节点类* 输入场景, 场景节点类* 输出场景) -> bool {
                return this->本能_读取帧特征信息(输入场景, 输出场景);
            },
            "本能_读取帧特征信息"
        );

        reg(
            枚举_本能动作ID::自我_获取画面识别率,
            [this](场景节点类* 输入场景, 场景节点类* 输出场景) -> bool {
                return this->本能_获取画面识别率(输入场景, 输出场景);
            },
            "本能_获取画面识别率"
        );

        reg(
            枚举_本能动作ID::自我_查找存在,
            [this](场景节点类* 输入场景, 场景节点类* 输出场景) -> bool {
                return this->本能_查找存在(输入场景, 输出场景);
            },
            "本能_查找存在"
        );

        reg(
            枚举_本能动作ID::自我_生成存在,
            [this](场景节点类* 输入场景, 场景节点类* 输出场景) -> bool {
                return this->本能_生成存在(输入场景, 输出场景);
            },
            "本能_生成存在"
        );

        reg(
            枚举_本能动作ID::自我_更新存在,
            [this](场景节点类* 输入场景, 场景节点类* 输出场景) -> bool {
                return this->本能_更新存在(输入场景, 输出场景);
            },
            "本能_更新存在"
        );
    }
};


