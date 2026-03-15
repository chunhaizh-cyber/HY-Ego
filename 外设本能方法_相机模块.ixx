
export module 外设本能方法_相机模块;

// ============================================================================
// 外设本能方法_相机模块（按规范对齐版）
// ----------------------------------------------------------------------------
// 分层：
//  - 本能动作模块：只放通用函数（形参模板写入 / 模式匹配 / 回执工具）
//  - 本能动作管理模块：注册/查询/运行（不做业务）
//  - 本模块：外设相机相关本能方法的具体实现（启动/关闭/设置参数/获取参数/获取帧）
//
// 关键共识：
//  - 相机是世界树中的真实存在；相机参数是其真实特征（事实）
//  - “想要的参数/候选空间/best/约束/预算/锁”属于方法虚拟存在（由上层方法/任务写）
//  - 本模块只做外设动作与事实回写，不做识别/存在组织等自我认知
//
// 四步语义（本模块每个本能方法内部按四步跑）：
//  Step1：确保方法树中有自己这个动作（方法首节点存在；必要形参类型写入首节点场景根节点模板）
//  Step2：解析输入参数并与形参模板绑定（模式匹配）；生成“条件节点条件场景快照”
//  Step3：执行外设动作（可能产生副作用）
//  Step4：输出回执（含条件场景快照）+ 写回世界树事实
// ============================================================================

import 外设模块;
import 主信息定义模块;
import 基础数据类型模块;
import 相机外设基类模块;
import D455相机外设模块;
import 虚拟相机外设模块;
import 相机写入工作流模块;

import 本能动作管理模块;
import 本能动作模块;        // 通用：形参模板写入/模式匹配/评分
import 方法环境模块;        // export inline 方法类 方法集;
import 世界树环境模块;      // export inline 世界树类 世界树;
import 场景模块;
import 语素环境模块;        // export inline 语素类 语素集;
import 特征类型定义模块;    // 型_外设摄像机 等（若已注册）
import 日志模块;

import 通用函数模块;        // 允许使用部分通用函数（不引入循环依赖：实现模块允许依赖通用模块）

import <memory>;
import <string>;
import <unordered_map>;
import <deque>;
import <mutex>;
import <atomic>;
import <chrono>;
import <utility>;
import <cstdint>;
import <vector>;
import <algorithm>;
import <bit>;
import <cmath>;

export class 外设本能方法类_相机模块 {
private:
   static inline 时间戳 当前_微秒() noexcept { return 结构体_时间戳::当前_微秒(); }
   static inline VecIU64 编码Vec3I64(const Vector3D& v)
   {
       return VecIU64{
           static_cast<U64>(std::bit_cast<std::int64_t>(std::llround(v.x))),
           static_cast<U64>(std::bit_cast<std::int64_t>(std::llround(v.y))),
           static_cast<U64>(std::bit_cast<std::int64_t>(std::llround(v.z)))
       };
   }
   
   static inline 本能动作类 本能动作{};
    static constexpr I64 方法运行状态_未运行 = 0;
    static constexpr I64 方法运行状态_运行中 = 1;
    static constexpr I64 方法运行状态_成功 = 2;
    static constexpr I64 方法运行状态_失败 = 3;

    // ----------------------------
    // 0) 词性/名称：回执字段、条件场景等
    // ----------------------------
     struct 结构体_词性缓存 {
        const 词性节点类* 回执_成功 = nullptr;
        const 词性节点类* 回执_错误码 = nullptr;
        const 词性节点类* 回执_时间戳 = nullptr;
        const 词性节点类* 回执_条件场景 = nullptr;

        const 词性节点类* 句柄_帧 = nullptr;

        const 词性节点类* 名称_相机参数包 = nullptr;

        // 相机真实存在特征类型：运行状态（停止/启动中/运行中/停止中/故障）
        const 词性节点类* 特征_运行状态 = nullptr;
        const 词性节点类* 特征_方法运行状态 = nullptr;
        const 词性节点类* 特征_方法最近成功 = nullptr;
        const 词性节点类* 特征_方法最近错误码 = nullptr;
        const 词性节点类* 特征_方法最近运行时间 = nullptr;
        const 词性节点类* 特征_方法相机可用 = nullptr;
    };

     static inline 结构体_词性缓存 g_t{};

    static void 私有_初始化词性()
    {
        if (g_t.回执_成功) return;
        特征类型定义类::初始化特征类型定义模块_依赖语素();

        g_t.回执_成功 = 语素集.添加词性词("回执_成功", "名词");
        g_t.回执_错误码 = 语素集.添加词性词("回执_错误码", "名词");
        g_t.回执_时间戳 = 语素集.添加词性词("回执_时间戳", "名词");
        g_t.回执_条件场景 = 语素集.添加词性词("回执_条件场景", "名词");

        g_t.句柄_帧 = 语素集.添加词性词("句柄_帧", "名词");

        // 形参容器：相机参数包（其子节点为具体参数键值）
        g_t.名称_相机参数包 = 语素集.添加词性词("相机参数包", "名词");

        // 真实特征类型：运行状态
        g_t.特征_运行状态 = 语素集.添加词性词("运行状态", "名词");
        g_t.特征_方法运行状态 = 语素集.添加词性词("方法_运行状态", "名词");
        g_t.特征_方法最近成功 = 语素集.添加词性词("方法_最近是否成功", "名词");
        g_t.特征_方法最近错误码 = 语素集.添加词性词("方法_最近错误码", "名词");
        g_t.特征_方法最近运行时间 = 语素集.添加词性词("方法_最近运行时间", "名词");
        g_t.特征_方法相机可用 = 语素集.添加词性词("方法_相机可用", "名词");
    }

    // ----------------------------
    // 1) 帧句柄仓库（最小可用）
    // ----------------------------
    struct 结构体_帧仓库项 {
        I64 句柄 = 0;
        std::shared_ptr<结构体_原始场景帧> 帧;
        时间戳 时间 = 0;
    };

    static inline std::mutex g_mu{};
    static inline std::unique_ptr<相机外设基类> g_cam{};   // 单相机实例
    static inline std::deque<结构体_帧仓库项> g_frames{};
    static inline std::atomic<I64> g_nextHandle{ 1 };
    static inline std::size_t g_maxFramesKeep = 8;

    static void 私有_帧仓库_插入_已加锁(std::shared_ptr<结构体_原始场景帧> f, 时间戳 t, I64& out句柄)
    {
        out句柄 = g_nextHandle.fetch_add(1);
        g_frames.push_back({ out句柄, std::move(f), t });
        while (g_frames.size() > g_maxFramesKeep) g_frames.pop_front();
    }

    static std::shared_ptr<const 结构体_原始场景帧> 私有_相机帧仓库_取只读(I64 句柄)
    {
        std::scoped_lock lk(g_mu);
        for (auto it = g_frames.rbegin(); it != g_frames.rend(); ++it) {
            if (it->句柄 == 句柄) return it->帧;
        }
        return {};
    }

    static bool 私有_抓取一帧句柄(方法节点类* 方法首节点, 时间戳 now, const std::string& 调用点, I64& out句柄, I64& out错误码)
    {
        out句柄 = 0;
        out错误码 = 0;

        外设数据包<结构体_原始场景帧> pkg{};
        枚举_取数结果 r = 枚举_取数结果::故障;

        {
            std::scoped_lock lk(g_mu);
            if (!g_cam) { 私有_确保_相机外设实例_已加锁(); }
            私有_同步方法相机特征(方法首节点, g_cam != nullptr, now, 调用点);
            if (g_cam) r = g_cam->等待下一包(pkg, 50);
        }

        if (r != 枚举_取数结果::成功) {
            out错误码 = (I64)r;
            return false;
        }

        {
            std::scoped_lock lk(g_mu);
            auto sp = std::make_shared<结构体_原始场景帧>(std::move(pkg.数据));
            私有_帧仓库_插入_已加锁(std::move(sp), pkg.头.时间_系统 ? pkg.头.时间_系统 : now, out句柄);
        }
        return true;
    }

    static bool 私有_写入前景存在候选事实(场景节点类* 输出场景, I64 句柄, 时间戳 now, const std::string& 调用点, I64& out候选数)
    {
        out候选数 = 0;
        if (!输出场景 || 句柄 <= 0) return false;

        auto sp = 私有_相机帧仓库_取只读(句柄);
        if (!sp) return false;

        const I64 W = (I64)sp->宽度;
        const I64 H = (I64)sp->高度;
        const I64 total = W * H;
        I64 valid = 0;
        if (!sp->深度有效.empty()) {
            for (auto v : sp->深度有效) valid += (v ? 1 : 0);
        }
        const I64 validRate = (total > 0) ? (valid * 10000 / total) : 0;

        (void)世界树.写入特征_I64(输出场景, g_t.句柄_帧, 句柄, {}, 调用点);
        if (特征类型定义类::类型_深度帧句柄) {
            (void)世界树.写入特征_I64(输出场景, 特征类型定义类::类型_深度帧句柄, 句柄, {}, 调用点);
        }
        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("帧_宽", "名词"), W, {}, 调用点);
        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("帧_高", "名词"), H, {}, 调用点);
        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("深度有效万分比", "名词"), validRate, {}, 调用点);

        相机帧处理器 proc{};
        帧处理结果 out{};
        out.原始场景帧 = std::const_pointer_cast<结构体_原始场景帧>(sp);
        if (!proc.处理一帧(out)) {
            return false;
        }

        static const 词性节点类* 名_候选集 = nullptr;
        static const 词性节点类* 型_候选集场景 = nullptr;
        static const 词性节点类* 名_候选存在 = nullptr;
        static const 词性节点类* 型_候选存在 = nullptr;
        static const 词性节点类* t跟踪ID = nullptr;
        if (!名_候选集) 名_候选集 = 语素集.添加词性词("候选存在集", "名词");
        if (!型_候选集场景) 型_候选集场景 = 语素集.添加词性词("候选集场景", "名词");
        if (!名_候选存在) 名_候选存在 = 语素集.添加词性词("候选存在", "名词");
        if (!型_候选存在) 型_候选存在 = 语素集.添加词性词("候选存在类型", "名词");
        if (!t跟踪ID) t跟踪ID = 语素集.添加词性词("跟踪ID", "名词");

        auto* miSc = new 场景节点主信息类();
        miSc->名称 = 名_候选集;
        miSc->类型 = 型_候选集场景;
        miSc->最后观测时间 = now;
        auto* candScene = 世界树.创建场景(输出场景, miSc, 调用点 + "/创建候选集场景");
        if (!candScene) return false;

        for (const auto& obs : out.存在观测列表) {
            auto* miE = new 存在节点主信息类();
            miE->名称 = 名_候选存在;
            miE->类型 = 型_候选存在;
            miE->最后观测时间 = now;
            auto* e = 世界树.创建存在(candScene, miE, 调用点 + "/创建候选存在");
            if (!e) continue;
            (void)世界树.写入特征_VecU(e, 特征类型定义类::类型_绝对位置, 编码Vec3I64(obs.中心坐标), {}, 调用点);
            (void)世界树.写入特征_VecU(e, 特征类型定义类::类型_尺寸, 编码Vec3I64(obs.尺寸), {}, 调用点);
            if (!obs.轮廓编码.empty()) {
                (void)世界树.写入特征_VecU(e, 特征类型定义类::类型_轮廓编码, obs.轮廓编码, {}, 调用点);
            }
            (void)世界树.写入特征_I64(e, t跟踪ID, (I64)obs.跟踪ID, {}, 调用点);
            (void)世界树.写入特征_I64(e, 特征类型定义类::类型_时间戳_us, (I64)now, {}, 调用点);
            ++out候选数;
        }

        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("前景存在候选数量", "名词"), out候选数, {}, 调用点);
        return true;
    }

    // ----------------------------
    // 2) 小工具：遍历/读写特征节点（仅用于“方法条件场景快照”这种模板内树）
    // ----------------------------
    static 特征节点主信息类* 私有_取特征主信息(基础信息节点类* n) {
        return (n && n->主信息) ? dynamic_cast<特征节点主信息类*>(n->主信息) : nullptr;
    }

    static const 特征节点主信息类* 私有_取特征主信息(const 基础信息节点类* n) {
        return (n && n->主信息) ? dynamic_cast<const 特征节点主信息类*>(n->主信息) : nullptr;
    }

    static std::vector<基础信息节点类*> 私有_枚举子节点(基础信息节点类* parent)
    {
        std::vector<基础信息节点类*> out;
        if (!parent || !parent->子) return out;
        auto* start = static_cast<基础信息节点类*>(parent->子);
        auto* cur = start;
        do {
            out.push_back(cur);
            cur = cur ? static_cast<基础信息节点类*>(cur->下) : nullptr;
        } while (cur && cur != start);
        return out;
    }

    static 基础信息节点类* 私有_查找子特征_按类型(基础信息节点类* parent, const 词性节点类* 类型)
    {
        if (!parent || !parent->子 || !类型) return nullptr;
        auto* start = static_cast<基础信息节点类*>(parent->子);
        auto* cur = start;
        do {
            auto* fi = 私有_取特征主信息(cur);
            if (fi && fi->类型 == 类型) return cur;
            cur = cur ? static_cast<基础信息节点类*>(cur->下) : nullptr;
        } while (cur && cur != start);
        return nullptr;
    }

    static void 私有_挂子_仅同层环(基础信息节点类* 父, 基础信息节点类* 子)
    {
        if (!父 || !子) return;
        子->父 = 父;
        子->根 = 父->根 ? 父->根 : 父;

        if (!父->子) {
            父->子 = 子;
            子->上 = 子;
            子->下 = 子;
        }
        else {
            auto* start = static_cast<基础信息节点类*>(父->子);
            auto* last = static_cast<基础信息节点类*>(start->上);
            last->下 = 子;
            子->上 = last;
            子->下 = start;
            start->上 = 子;
        }
    }

    static 基础信息节点类* 私有_场景_写I64特征_模板树(基础信息节点类* 场景根, const 词性节点类* 类型, I64 值)
    {
        if (!场景根 || !类型) return nullptr;
        if (auto* n = 私有_查找子特征_按类型(场景根, 类型)) {
            if (auto* fi = 私有_取特征主信息(n)) {
                fi->当前快照 = 值;
                (void)世界树.确保特征(n, g_t.特征_运行状态, nullptr, "外设本能方法_相机模块::确保相机存在");
                return n;
            }
        }

        auto* info = new 特征节点主信息类();
        info->类型 = 类型;
        info->当前快照 = 值;

        auto* n = new 基础信息节点类();
        n->主信息 = info;
        n->设置主键(std::string("cond_") + 类型->获取主键());

        私有_挂子_仅同层环(场景根, n);
        (void)世界树.确保特征(n, g_t.特征_运行状态, nullptr, "外设本能方法_相机模块::确保相机存在");
        return n;
    }
    // ----------------------------
  //  外设实例：确保相机对象存在（D455 或 虚拟）
  // ----------------------------
    static void 私有_确保_相机外设实例_已加锁()
    {
        if (g_cam) return;

        try {
            g_cam = std::make_unique<D455相机外设类>();
        }
        catch (...) {
            g_cam = std::make_unique<虚拟相机外设类>();
        }
    }



    // ----------------------------
    // 3) 世界树：确保相机真实存在节点
    // ----------------------------
    static 存在节点类* 私有_确保_世界树相机存在()
    {
        私有_初始化词性();

        auto* root = 世界树.世界根();
        if (!root) return nullptr;

        const 词性节点类* 相机类型 = 特征类型定义类::型_外设摄像机;
        const 词性节点类* 相机名 = 特征类型定义类::名_摄像机;
        if (!相机类型) 相机类型 = 语素集.添加词性词("外设摄像机", "名词");
        if (!相机名) 相机名 = 语素集.添加词性词("摄像机", "名词");

        // 查找：世界根下第一个 类型=相机类型 的存在
        auto existings = 场景类::获取子存在(root);
        for (auto* e : existings) {
            if (!e || !e->主信息) continue;
            if (e->主信息->类型 == 相机类型) { (void)世界树.确保特征(e, g_t.特征_运行状态, nullptr, "外设本能方法_相机模块::确保相机存在"); return e; }
        }

        // 创建
        auto* mi = new 存在节点主信息类();
        mi->类型 = 相机类型;

        auto* n = 世界树.添加子存在(root, mi, 当前_微秒(), "外设本能方法_相机模块::确保相机存在");
        (void)世界树.写入名称(n, 相机名, 当前_微秒(), "外设本能方法_相机模块::确保相机存在");
        (void)世界树.确保特征(n, g_t.特征_运行状态, nullptr, "外设本能方法_相机模块::确保相机存在");
        return n;
    }
    // ----------------------------
  // 相机运行状态（真实特征）同步
  // ----------------------------
    static inline 运行状态 私有_映射_外设状态到运行状态(枚举_外设状态 s) noexcept
    {
        switch (s) {
        case 枚举_外设状态::启动中: return 运行状态::启动中;
        case 枚举_外设状态::运行中: return 运行状态::运行中;
        case 枚举_外设状态::暂停中: return 运行状态::运行中;
        case 枚举_外设状态::停止中: return 运行状态::停止中;
        case 枚举_外设状态::故障:   return 运行状态::故障;
        case 枚举_外设状态::离线:   return 运行状态::故障;
        case 枚举_外设状态::已停止: return 运行状态::停止;
        case 枚举_外设状态::未启动: return 运行状态::停止;
        default: return 运行状态::停止;
        }
    }

    static inline void 私有_写相机运行状态_到世界树(运行状态 st, 时间戳 now)
    {
        存在节点类* camExist = 私有_确保_世界树相机存在();
        if (!camExist) return;
        if (!g_t.特征_运行状态) return;

        (void)世界树.写入特征_I64(camExist, g_t.特征_运行状态, (I64)st, {}, "外设本能方法_相机模块::写相机运行状态");
        // 同时把“最后同步时间”写进相机存在（可选：后续再加类型）
    }

    static inline void 私有_同步_相机运行状态_从外设(时间戳 now)
    {
        std::scoped_lock lk(g_mu);
        私有_确保_相机外设实例_已加锁();
        if (!g_cam) return;
        const auto st = g_cam->状态();
        私有_写相机运行状态_到世界树(私有_映射_外设状态到运行状态(st.状态), now);
    }


    static 基础信息节点类* 私有_场景_写特征节点_无值_模板树(基础信息节点类* 场景根, const 词性节点类* 类型)
    {
        if (!场景根 || !类型) return nullptr;

        if (auto* n = 私有_查找子特征_按类型(场景根, 类型)) {
            (void)世界树.确保特征(n, g_t.特征_运行状态, nullptr, "外设本能方法_相机模块::确保相机存在");
            return n;
        }

        auto* info = new 特征节点主信息类();
        info->类型 = 类型;
        info->当前快照 = std::monostate{};

        auto* n = new 基础信息节点类();
        n->主信息 = info;
        n->设置主键(std::string("cond_") + 类型->获取主键());

        私有_挂子_仅同层环(场景根, n);
        (void)世界树.确保特征(n, g_t.特征_运行状态, nullptr, "外设本能方法_相机模块::确保相机存在");
        return n;
    }
    // ----------------------------
    // 4) 本能注册表中的“方法首节点指针”获取/回填
    // ----------------------------
    static 方法节点类* 私有_取或创建_方法首节点_并回填(枚举_本能动作ID id, const std::string& 函数名)
    {
        const 时间戳 now = 当前_微秒();

        // 1) 先看注册表上下文里是否已有首节点指针
        if (auto opt = 本能集.查询(id)) {
            if (opt->方法信息首节点) {
                方法集.初始化方法虚拟存在信息(opt->方法信息首节点, now, 函数名);
                return opt->方法信息首节点;
            }
        }

        // 2) 没有则创建（或在方法链中查找）并回写到注册表
        auto* head = 方法集.查找或创建_本能方法首节点(id, now, 函数名);
        if (head) {
            方法集.初始化方法虚拟存在信息(head, now, 函数名);
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
    static void 私有_写方法运行结果_到虚拟存在(
        方法节点类* 方法首节点,
        I64 运行状态值,
        bool 最近成功,
        I64 最近错误码,
        时间戳 now,
        bool 相机可用,
        const std::string& 调用点)
    {
        if (!方法首节点) return;
        auto* ve = 方法集.取或创建_方法虚拟存在(方法首节点, now, 调用点);
        if (!ve) return;
        (void)世界树.写入特征_I64(ve, g_t.特征_方法运行状态, 运行状态值, {}, 调用点);
        (void)世界树.写入特征_I64(ve, g_t.特征_方法最近成功, 最近成功 ? 1 : 0, {}, 调用点);
        (void)世界树.写入特征_I64(ve, g_t.特征_方法最近错误码, 最近错误码, {}, 调用点);
        (void)世界树.写入特征_I64(ve, g_t.特征_方法最近运行时间, (I64)now, {}, 调用点);
        (void)世界树.写入特征_I64(ve, g_t.特征_方法相机可用, 相机可用 ? 1 : 0, {}, 调用点);
    }

    static void 私有_同步方法相机特征(
        方法节点类* 方法首节点,
        bool 相机可用,
        时间戳 now,
        const std::string& 调用点)
    {
        if (!方法首节点) return;
        (void)方法集.记录方法虚拟存在特征(
            方法首节点,
            g_t.特征_方法相机可用,
            特征快照值{ 相机可用 ? (I64)1 : (I64)0 },
            now,
            调用点);
    }


    // ----------------------------
    // 5) Step4：写回执（含条件场景快照）
    // ----------------------------
    static void 私有_写回执(场景节点类* 输出,
        bool 成功,
        I64 错误码,
        时间戳 now,
        const 场景节点类* 条件场景快照 /*可空*/)
    {
        if (!输出) return;
        私有_初始化词性();

        // 最小字段
        (void)世界树.写入特征_I64(输出, g_t.回执_成功, 成功 ? 1 : 0, {}, "外设本能方法_相机模块::写回执");
        (void)世界树.写入特征_I64(输出, g_t.回执_错误码, 错误码, {}, "外设本能方法_相机模块::写回执");
        (void)世界树.写入特征_I64(输出, g_t.回执_时间戳, (I64)now, {}, "外设本能方法_相机模块::写回执");

        // 条件场景快照：作为输出子场景写入（用于后续完善方法树/条件融合）
        if (条件场景快照) {
            auto* condOut = 世界树.取或创建子场景_按名称(
                输出,
                const_cast<词性节点类*>(g_t.回执_条件场景),
                now,
                "外设本能方法_相机模块::写回执");

            if (condOut) {
                // 复制“条件场景快照”的第一层特征（递归可后续补）
                auto* srcRoot = static_cast<const 基础信息节点类*>(条件场景快照);
                if (srcRoot && srcRoot->子) {
                    auto* start = static_cast<const 基础信息节点类*>(srcRoot->子);
                    auto* cur = start;
                    do {
                        auto* fi = cur ? dynamic_cast<const 特征节点主信息类*>(cur->主信息) : nullptr;
                        if (fi && fi->类型) {
                            if (std::holds_alternative<I64>(fi->当前快照)) {
                                (void)世界树.写入特征_I64(condOut, fi->类型, std::get<1>(fi->当前快照), {}, "外设本能方法_相机模块::写回执");
                            }
                            // VecU句柄等其它类型：此处先不写（避免引入证据池依赖）
                        }
                        cur = cur ? static_cast<const 基础信息节点类*>(cur->下) : nullptr;
                    } while (cur && cur != start);
                }
            }
        }
    }

    // ----------------------------
    // 6) Step2：条件校验（模式匹配）+ 条件场景快照生成
    // ----------------------------
    struct 结构体_条件解析结果 {
        bool ok = true;
        I64 错误码 = 0;

        // 条件节点（用于方法树长出条件样本）
        方法节点类* 条件节点 = nullptr;

        // 条件场景快照（来自条件节点主信息的 场景根节点）
        场景节点类* 条件场景 = nullptr;

        // 对于“设置相机参数”：解析到的参数键值对
        std::vector<std::pair<std::string, I64>> 参数KV{};
    };

    static 结构体_条件解析结果 私有_Step2_解析_设置相机参数(
        方法节点类* 方法首节点,
        场景节点类* 输入场景,
        const std::string& 函数名)
    {
        结构体_条件解析结果 out{};
        if (!方法首节点 || !方法首节点->主信息 || !输入场景) {
            out.ok = false; out.错误码 = 1; return out;
        }
        auto* hmi = dynamic_cast<方法首节点主信息类*>(方法首节点->主信息);
        if (!hmi) { out.ok = false; out.错误码 = 2; return out; }

        static 本能动作类::结构体_场景模式匹配参数 p;
        p.I64容忍误差 = 0;
        p.歧义分差阈值 = 50;

        auto mr = 本能动作.场景_模式匹配(输入场景, &hmi->场景根节点, p);
        if (!mr.匹配成功) { out.ok = false; out.错误码 = 10; return out; }

        方法条件节点主信息类 临时条件信息{};
        auto* 临时条件场景 = &临时条件信息.场景根节点;

        基础信息节点类* inPack = nullptr;
        for (const auto& b : mr.绑定表) {
            auto* pat = b.模式节点;
            auto* inN = b.输入节点;
            if (!pat || !inN || !pat->主信息) continue;
            auto* pfi = dynamic_cast<特征节点主信息类*>(pat->主信息);
            if (pfi && pfi->类型 == g_t.名称_相机参数包) {
                inPack = static_cast<基础信息节点类*>(inN);
                break;
            }
        }
        if (!inPack) { out.ok = false; out.错误码 = 13; return out; }

        auto* condRoot = static_cast<基础信息节点类*>(临时条件场景);
        auto* condPack = 私有_场景_写特征节点_无值_模板树(condRoot, g_t.名称_相机参数包);
        if (!condPack) { out.ok = false; out.错误码 = 14; return out; }

        for (auto* child : 私有_枚举子节点(inPack)) {
            auto* fi = 私有_取特征主信息(child);
            if (!fi || !fi->类型) continue;
            if (!std::holds_alternative<I64>(fi->当前快照)) continue;

            const I64 v = std::get<1>(fi->当前快照);
            (void)私有_场景_写I64特征_模板树(condPack, fi->类型, v);
            out.参数KV.push_back({ fi->类型->获取主键(), v });
        }

        out.条件节点 = 方法集.取或创建_条件节点_按场景(方法首节点, 临时条件场景, 当前_微秒(), 64, {}, 函数名);
        if (!out.条件节点 || !out.条件节点->主信息) { out.ok = false; out.错误码 = 11; return out; }

        auto* cmi = dynamic_cast<方法条件节点主信息类*>(out.条件节点->主信息);
        if (!cmi) { out.ok = false; out.错误码 = 12; return out; }

        out.条件场景 = &cmi->场景根节点;
        return out;
    }

    // Step2：无参动作（启动/关闭/取帧/获取参数）——仅创建条件节点与空快照
    static 结构体_条件解析结果 私有_Step2_解析_无参动作(方法节点类* 方法首节点, const std::string& 函数名)
    {
        结构体_条件解析结果 out{};
        if (!方法首节点) { out.ok = false; out.错误码 = 1; return out; }

        方法条件节点主信息类 临时条件信息{};
        out.条件节点 = 方法集.取或创建_条件节点_按场景(方法首节点, &临时条件信息.场景根节点, 当前_微秒(), 64, {}, 函数名);
        if (!out.条件节点 || !out.条件节点->主信息) { out.ok = false; out.错误码 = 2; return out; }

        auto* cmi = dynamic_cast<方法条件节点主信息类*>(out.条件节点->主信息);
        if (!cmi) { out.ok = false; out.错误码 = 3; return out; }

        out.条件场景 = &cmi->场景根节点;
        return out;
    }

    public:


    // ============================================================================
    // 导出：帧句柄仓库读取（供自我侧在“读取帧特征信息”本能方法中使用）
    // - 注意：只读指针，不暴露可写入口
    // ============================================================================
    std::shared_ptr<const 结构体_原始场景帧> 相机帧仓库_取只读(I64 句柄)
    {
        return 私有_相机帧仓库_取只读(句柄);
    }


    // ============================================================================
    // 本能方法：外设_相机启动
    // ============================================================================
    static bool 本能_外设_相机启动(场景节点类* 输入场景, 场景节点类* 输出场景)
    {
        constexpr 枚举_本能动作ID id = 枚举_本能动作ID::外设_相机启动;
        const std::string fn = "本能_外设_相机启动";
        const 时间戳 now = 当前_微秒();
        私有_初始化词性();

        auto* head = 私有_取或创建_方法首节点_并回填(id, fn);
        结构体_方法调用守卫 守卫(head, 输入场景, 输出场景, now, fn);
        if (head) 本能动作.方法首节点_确保条件参数类型(head, {});

        auto cond = 私有_Step2_解析_无参动作(head, fn);
        if (!cond.ok) {
            守卫.设置结果(false, cond.错误码);
            私有_写回执(输出场景, false, cond.错误码, now, nullptr);
            return false;
        }

        外设启动参数 p{};
        外设指令回执 rc{};
        {
            std::scoped_lock lk(g_mu);
            私有_确保_相机外设实例_已加锁();
            私有_同步方法相机特征(head, g_cam != nullptr, now, fn);
            私有_写相机运行状态_到世界树(运行状态::启动中, now);
            if (!g_cam) {
                rc.成功 = false;
                rc.错误码 = -100;
                rc.消息 = "camera unavailable";
            }
            else {
                rc.成功 = g_cam->启动(p);
                rc.错误码 = rc.成功 ? 0 : -1;
                rc.消息 = rc.成功 ? "ok" : "start failed";
            }
            私有_写相机运行状态_到世界树(rc.成功 ? 运行状态::运行中 : 运行状态::故障, now);
        }

        (void)私有_确保_世界树相机存在();
        守卫.设置结果(rc.成功, (I64)rc.错误码);
        私有_写回执(输出场景, rc.成功, (I64)rc.错误码, now, cond.条件场景);
        return rc.成功;
    }

    // ============================================================================
    // 本能方法：外设_相机关闭
    // ============================================================================
    static bool 本能_外设_相机关闭(场景节点类* 输入场景, 场景节点类* 输出场景)
    {
        constexpr 枚举_本能动作ID id = 枚举_本能动作ID::外设_相机关闭;
        const std::string fn = "本能_外设_相机关闭";
        const 时间戳 now = 当前_微秒();

        auto* head = 私有_取或创建_方法首节点_并回填(id, fn);
        结构体_方法调用守卫 守卫(head, 输入场景, 输出场景, now, fn);
        if (head) 本能动作.方法首节点_确保条件参数类型(head, {});

        auto cond = 私有_Step2_解析_无参动作(head, fn);
        if (!cond.ok) {
            守卫.设置结果(false, cond.错误码);
            私有_写回执(输出场景, false, cond.错误码, now, nullptr);
            return false;
        }

        bool ok = false;
        I64 err = 0;
        {
            std::scoped_lock lk(g_mu);
            if (!g_cam) 私有_确保_相机外设实例_已加锁();
            私有_同步方法相机特征(head, g_cam != nullptr, now, fn);
            if (!g_cam) {
                err = -101;
            }
            else {
                g_cam->停止();
                ok = true;
                私有_写相机运行状态_到世界树(运行状态::停止, now);
            }
        }

        守卫.设置结果(ok, err);
        私有_写回执(输出场景, ok, err, now, cond.条件场景);
        return ok;
    }



    // ============================================================================
    // 本能方法：外设_获取相机状态（同步外设状态到世界树真实相机存在）
    // ----------------------------------------------------------------------------
    // - 不改变外设状态（只读）
    // - 必须把外设状态映射为 运行状态 并写入相机存在的真实特征
    // ============================================================================
        static bool 本能_外设_获取相机状态(场景节点类* 输入场景, 场景节点类* 输出场景)
    {
        constexpr 枚举_本能动作ID id = 枚举_本能动作ID::外设_获取相机状态;
        const std::string fn = "本能_外设_获取相机状态";
        const 时间戳 now = 当前_微秒();
        私有_初始化词性();

        auto* head = 私有_取或创建_方法首节点_并回填(id, fn);
        结构体_方法调用守卫 守卫(head, 输入场景, 输出场景, now, fn);
        if (head) 本能动作.方法首节点_确保条件参数类型(head, {});

        auto cond = 私有_Step2_解析_无参动作(head, fn);
        if (!cond.ok) {
            守卫.设置结果(false, cond.错误码);
            私有_写回执(输出场景, false, cond.错误码, now, nullptr);
            return false;
        }

        私有_同步_相机运行状态_从外设(now);
        {
            std::scoped_lock lk(g_mu);
            私有_同步方法相机特征(head, g_cam != nullptr, now, fn);
        }

        守卫.设置结果(true, 0);
        私有_写回执(输出场景, true, 0, now, cond.条件场景);
        return true;
    }

    // ============================================================================
    // 本能方法：外设_设置相机参数
    // 形参模板：首节点条件模板中要求存在一个“相机参数包”节点（子节点任意 => 允许任意参数集合）
    // 输入实参：输入场景子链包含 “相机参数包” 特征节点，其子节点为具体参数（类型=参数名，值=I64）
    // ============================================================================
        static bool 本能_外设_设置相机参数(场景节点类* 输入场景, 场景节点类* 输出场景)
    {
        constexpr 枚举_本能动作ID id = 枚举_本能动作ID::外设_设置相机参数;
        const std::string fn = "本能_外设_设置相机参数";
        const 时间戳 now = 当前_微秒();
        私有_初始化词性();

        auto* head = 私有_取或创建_方法首节点_并回填(id, fn);
        结构体_方法调用守卫 守卫(head, 输入场景, 输出场景, now, fn);
        if (head) {
            本能动作.方法首节点_确保条件参数类型(head, { g_t.名称_相机参数包 });
        }

        auto cond = 私有_Step2_解析_设置相机参数(head, 输入场景, fn);
        if (!cond.ok) {
            守卫.设置结果(false, cond.错误码);
            私有_写回执(输出场景, false, cond.错误码, now, nullptr);
            return false;
        }

        bool okAll = true;
        I64 err = 0;
        {
            std::scoped_lock lk(g_mu);
            私有_确保_相机外设实例_已加锁();
            私有_同步方法相机特征(head, g_cam != nullptr, now, fn);
            if (!g_cam) {
                okAll = false;
                err = -102;
            }

            for (const auto& kv : cond.参数KV) {
                if (!okAll) break;
                外设指令 cmd{};
                cmd.类型 = 枚举_外设指令::设置参数;
                cmd.参数名 = kv.first;
                cmd.参数值_ = (std::int64_t)kv.second;

                const auto rc = g_cam->指令(cmd);
                if (!rc.成功) { okAll = false; err = (I64)rc.错误码; break; }

                if (auto* camExist = 私有_确保_世界树相机存在()) {
                    const 词性节点类* t = 语素集.添加词性词(kv.first, "名词");
                    (void)世界树.写入特征_I64(camExist, t, kv.second, {}, "外设_设置相机参数");
                }
            }
        }

        守卫.设置结果(okAll, err);
        私有_写回执(输出场景, okAll, err, now, cond.条件场景);
        return okAll;
    }

    // ============================================================================
    // 本能方法：外设_获取相机参数
    // 说明：外设接口不提供“读某参数”的统一指令；本方法返回世界树中记录的“相机真实参数事实”。
    // 输入：可选 “相机参数包” 指定要读哪些参数（子节点类型=参数名）
    // 输出：在输出场景中写入同名特征 I64（若未记录则不写）
    // ============================================================================
        static bool 本能_外设_获取相机参数(场景节点类* 输入场景, 场景节点类* 输出场景)
    {
        constexpr 枚举_本能动作ID id = 枚举_本能动作ID::外设_获取相机参数;
        const std::string fn = "本能_外设_获取相机参数";
        const 时间戳 now = 当前_微秒();
        私有_初始化词性();

        auto* head = 私有_取或创建_方法首节点_并回填(id, fn);
        结构体_方法调用守卫 守卫(head, 输入场景, 输出场景, now, fn);
        if (head) {
            本能动作.方法首节点_确保条件参数类型(head, {});
        }

        auto cond = 私有_Step2_解析_无参动作(head, fn);
        if (!cond.ok) {
            守卫.设置结果(false, cond.错误码);
            私有_写回执(输出场景, false, cond.错误码, now, nullptr);
            return false;
        }

        auto* camExist = 私有_确保_世界树相机存在();
        {
            std::scoped_lock lk(g_mu);
            私有_同步方法相机特征(head, g_cam != nullptr, now, fn);
        }
        if (!camExist) {
            守卫.设置结果(false, -10);
            私有_写回执(输出场景, false, -10, now, cond.条件场景);
            return false;
        }

        std::vector<const 词性节点类*> wantTypes{};
        if (输入场景 && 输入场景->子) {
            基础信息节点类* inRoot = static_cast<基础信息节点类*>(输入场景);
            基础信息节点类* pack = 私有_查找子特征_按类型(inRoot, g_t.名称_相机参数包);
            if (pack) {
                for (auto* c : 私有_枚举子节点(pack)) {
                    auto* fi = 私有_取特征主信息(c);
                    if (fi && fi->类型) wantTypes.push_back(fi->类型);
                }
            }
            else {
                for (auto* c : 私有_枚举子节点(inRoot)) {
                    auto* fi = 私有_取特征主信息(c);
                    if (fi && fi->类型) wantTypes.push_back(fi->类型);
                }
            }
        }

        if (wantTypes.empty()) {
            守卫.设置结果(true, 0);
            私有_写回执(输出场景, true, 0, now, cond.条件场景);
            return true;
        }

        auto* camNode = static_cast<基础信息节点类*>(camExist);
        for (auto* t : wantTypes) {
            if (!t) continue;
            if (auto* n = 私有_查找子特征_按类型(camNode, t)) {
                auto* fi = 私有_取特征主信息(n);
                if (fi && std::holds_alternative<I64>(fi->当前快照)) {
                    (void)世界树.写入特征_I64(输出场景, t, std::get<1>(fi->当前快照), {}, fn);
                }
            }
        }

        守卫.设置结果(true, 0);
        私有_写回执(输出场景, true, 0, now, cond.条件场景);
        return true;
    }

    // ============================================================================
    // 本能方法：外设_获取帧信息
    // 输出：帧句柄（I64），供自我侧后续读取帧特征
    // ============================================================================
    static bool 本能_外设_获取帧信息(场景节点类* 输入场景, 场景节点类* 输出场景)
    {
        constexpr 枚举_本能动作ID id = 枚举_本能动作ID::外设_获取帧信息;
        const std::string fn = "本能_外设_获取帧信息";
        const 时间戳 now = 当前_微秒();
        私有_初始化词性();

        auto* head = 私有_取或创建_方法首节点_并回填(id, fn);
        结构体_方法调用守卫 守卫(head, 输入场景, 输出场景, now, fn);
        if (head) 本能动作.方法首节点_确保条件参数类型(head, {});

        auto cond = 私有_Step2_解析_无参动作(head, fn);
        if (!cond.ok) {
            守卫.设置结果(false, cond.错误码);
            私有_写回执(输出场景, false, cond.错误码, now, nullptr);
            return false;
        }

        I64 handle = 0;
        I64 err = 0;
        if (!私有_抓取一帧句柄(head, now, fn, handle, err)) {
            守卫.设置结果(false, err);
            私有_写回执(输出场景, false, err, now, cond.条件场景);
            return false;
        }

        (void)世界树.写入特征_I64(输出场景, g_t.句柄_帧, handle, {}, fn);
        if (特征类型定义类::类型_深度帧句柄) {
            (void)世界树.写入特征_I64(输出场景, 特征类型定义类::类型_深度帧句柄, handle, {}, fn);
        }
        守卫.设置结果(true, 0);
        私有_写回执(输出场景, true, 0, now, cond.条件场景);
        return true;
    }

    // ============================================================================
    // 本能方法：外设_读取深度相机状态
    // 输出：深度相机可用 / 深度帧可获取 / 可选帧句柄
    // ============================================================================
    static bool 本能_外设_读取深度相机状态(场景节点类* 输入场景, 场景节点类* 输出场景)
    {
        constexpr 枚举_本能动作ID id = 枚举_本能动作ID::外设_读取深度相机状态;
        const std::string fn = "本能_外设_读取深度相机状态";
        const 时间戳 now = 当前_微秒();
        私有_初始化词性();

        auto* head = 私有_取或创建_方法首节点_并回填(id, fn);
        结构体_方法调用守卫 守卫(head, 输入场景, 输出场景, now, fn);
        if (head) 本能动作.方法首节点_确保条件参数类型(head, {});

        auto cond = 私有_Step2_解析_无参动作(head, fn);
        if (!cond.ok) {
            守卫.设置结果(false, cond.错误码);
            私有_写回执(输出场景, false, cond.错误码, now, nullptr);
            return false;
        }

        私有_同步_相机运行状态_从外设(now);

        bool 相机可用 = false;
        {
            std::scoped_lock lk(g_mu);
            if (!g_cam) { 私有_确保_相机外设实例_已加锁(); }
            相机可用 = (g_cam != nullptr);
            私有_同步方法相机特征(head, 相机可用, now, fn);
        }

        I64 handle = 0;
        I64 err = 0;
        const bool 帧可获取 = 私有_抓取一帧句柄(head, now, fn, handle, err);

        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("深度相机可用", "名词"), 相机可用 ? 1 : 0, {}, fn);
        (void)世界树.写入特征_I64(输出场景, 语素集.添加词性词("深度帧可获取", "名词"), 帧可获取 ? 1 : 0, {}, fn);
        if (帧可获取) {
            (void)世界树.写入特征_I64(输出场景, g_t.句柄_帧, handle, {}, fn);
            if (特征类型定义类::类型_深度帧句柄) {
                (void)世界树.写入特征_I64(输出场景, 特征类型定义类::类型_深度帧句柄, handle, {}, fn);
            }
        }

        守卫.设置结果(true, 帧可获取 ? 0 : err);
        私有_写回执(输出场景, true, 帧可获取 ? 0 : err, now, cond.条件场景);
        return true;
    }

    // ============================================================================
    // 本能方法：外设_提取前景存在候选事实
    // 输出：帧宽/帧高/深度有效万分比/候选存在集
    // ============================================================================
    static bool 本能_外设_提取前景存在候选事实(场景节点类* 输入场景, 场景节点类* 输出场景)
    {
        constexpr 枚举_本能动作ID id = 枚举_本能动作ID::外设_提取前景存在候选事实;
        const std::string fn = "本能_外设_提取前景存在候选事实";
        const 时间戳 now = 当前_微秒();
        私有_初始化词性();

        auto* head = 私有_取或创建_方法首节点_并回填(id, fn);
        结构体_方法调用守卫 守卫(head, 输入场景, 输出场景, now, fn);
        if (head) 本能动作.方法首节点_确保条件参数类型(head, {});

        auto cond = 私有_Step2_解析_无参动作(head, fn);
        if (!cond.ok) {
            守卫.设置结果(false, cond.错误码);
            私有_写回执(输出场景, false, cond.错误码, now, nullptr);
            return false;
        }

        I64 handle = 0;
        I64 err = 0;
        if (!私有_抓取一帧句柄(head, now, fn, handle, err)) {
            守卫.设置结果(false, err);
            私有_写回执(输出场景, false, err, now, cond.条件场景);
            return false;
        }

        I64 候选数 = 0;
        if (!私有_写入前景存在候选事实(输出场景, handle, now, fn, 候选数)) {
            守卫.设置结果(false, -20);
            私有_写回执(输出场景, false, -20, now, cond.条件场景);
            return false;
        }

        日志::运行f("[外设_提取前景存在候选事实] 句柄={}, 候选数={}", handle, 候选数);

        守卫.设置结果(true, 0);
        私有_写回执(输出场景, true, 0, now, cond.条件场景);
        return true;
    }

    // ============================================================================
    // 注册入口：由 Self 初始化调用一次
    // - 按规范：注册时先确保方法首节点存在；若注册表中指针为空则回填
    // ============================================================================
    void 注册默认本能动作_外设相机()
    {
        私有_初始化词性();

        const 时间戳 now = 当前_微秒();

        auto reg = [&](枚举_本能动作ID id, 本能函数 fn, const std::string& name,
            const std::vector<const 词性节点类*>& 需要类型) {
                // 先看注册表是否已有首节点指针
                方法节点类* head = nullptr;
                if (auto opt = 本能集.查询(id)) head = opt->方法信息首节点;
                if (!head) head = 方法集.查找或创建_本能方法首节点(id, now, "注册默认本能动作_外设相机");
                if (head) 方法集.初始化方法虚拟存在信息(head, now, "注册默认本能动作_外设相机");

                if (!本能集.有(id)) {
                    本能集.注册(id, fn, name, head);
                }
                else {
                    (void)本能集.补全方法信息(id, head);
                }

                // 补写形参类型到首节点条件模板（幂等）
                if (head) 本能动作.方法首节点_确保条件参数类型(head, 需要类型);
            };

        reg(枚举_本能动作ID::外设_相机启动, 本能_外设_相机启动, "本能_外设_相机启动", {});
        reg(枚举_本能动作ID::外设_相机关闭, 本能_外设_相机关闭, "本能_外设_相机关闭", {});
        reg(枚举_本能动作ID::外设_设置相机参数, 本能_外设_设置相机参数, "本能_外设_设置相机参数", { g_t.名称_相机参数包 });
        reg(枚举_本能动作ID::外设_获取相机参数, 本能_外设_获取相机参数, "本能_外设_获取相机参数", {});
        reg(枚举_本能动作ID::外设_获取相机状态, 本能_外设_获取相机状态, "本能_外设_获取相机状态", {});
        reg(枚举_本能动作ID::外设_获取帧信息, 本能_外设_获取帧信息, "本能_外设_获取帧信息", {});
        reg(枚举_本能动作ID::外设_读取深度相机状态, 本能_外设_读取深度相机状态, "本能_外设_读取深度相机状态", {});
        reg(枚举_本能动作ID::外设_提取前景存在候选事实, 本能_外设_提取前景存在候选事实, "本能_外设_提取前景存在候选事实", {});
    }
};


