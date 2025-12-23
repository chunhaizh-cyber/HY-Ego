module;

#include <opencv2/opencv.hpp>
#include <opencv2/viz.hpp>

export module 场景实时显示模块;

import 相机接口模块;
import 基础数据类型模块;
import 世界树环境模块;
import 三维场景管理模块;
import 主信息定义模块;
import 数据仓库模块;
import 特征类型定义模块;
import 特征值环境模块;
import 特征模块;
import 场景模块;

import <vector>;
import <string>;
import <memory>;
import <unordered_map>;
import <unordered_set>;
import <optional>;
import <sstream>;
import <iomanip>;
import <cmath>;
import <algorithm>;
import <chrono>;
import <variant>;
import <type_traits>;
import <cstdint>;

// ================================
// 1) 参数：仅作为“概念_显示参数”缺失时的兜底
// ================================
export struct 场景显示参数 {
    // ---- 开关（兜底）----
    bool 显示三维背景 = true;
    bool 显示坐标轴 = true;
    bool 显示网格地面 = false;
    bool 显示相机模型 = true;

    bool 显示实时点云 = true;

    bool 显示存在框 = true;
    bool 显示存在标签 = true;
    bool 显示存在ID = true;
    bool 显示存在名称 = true;
    bool 显示存在大小 = false;
    bool 显示存在坐标 = true;
    bool 显示存在轨迹 = true;

    bool 显示质量信息 = false;
    bool 视野外淡化 = true;

    // ---- 参数（兜底）----
    // 坐标模式：0=相对(P_rel)，1=绝对(P_abs)，2=双显（优先abs，括号给rel）
    std::int64_t 坐标模式 = 0;

    std::int64_t 点云采样步长 = 3;

    // FOV（度*100），近/远裁剪（mm）
    std::int64_t FOV水平_百分度 = 8600;
    std::int64_t FOV垂直_百分度 = 5700;
    std::int64_t 近裁剪_mm = 150;
    std::int64_t 远裁剪_mm = 6000;

    // 视野外淡化：ms
    std::int64_t 记忆淡化_ms = 15000;

    // 轨迹长度：点数
    std::int64_t 轨迹长度 = 60;
};

// ================================
// 2) 对外接口
// ================================
export class 场景实时显示类 {
public:
    explicit 场景实时显示类(const 场景显示参数& 参数);
    ~场景实时显示类() = default;

    void 更新显示(场景节点类* 当前场景, const 结构体_原始场景帧* 实时帧);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

// ================================
// 3) 内部实现
// ================================
namespace 场景实时显示_内部 {

    inline 时间戳 当前时间戳_ms_稳态()
    {
        using namespace std::chrono;
        return (时间戳)duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
    }

    inline double clamp01(double x)
    {
        if (x < 0.0) return 0.0;
        if (x > 1.0) return 1.0;
        return x;
    }

    inline double deg100_to_rad(double deg100)
    {
        constexpr double PI = 3.14159265358979323846;
        return (deg100 / 100.0) * PI / 180.0;
    }

    inline cv::Point3d toP3d(const Vector3D& v)
    {
        return cv::Point3d(v.x, v.y, v.z);
    }

    inline bool finite3(const Vector3D& v)
    {
        return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z);
    }

    inline cv::Point3d toP3d_add(const Vector3D& v, const Vector3D& t)
    {
        return cv::Point3d(v.x + t.x, v.y + t.y, v.z + t.z);
    }

    inline cv::viz::Color toVizColor(const 颜色_BGR8& c, double alpha = 1.0)
    {
        alpha = clamp01(alpha);
        auto s = [&](std::uint8_t v) -> std::uint8_t {
            double x = (double)v * alpha;
            if (x < 0.0) x = 0.0;
            if (x > 255.0) x = 255.0;
            return (std::uint8_t)(x + 0.5);
            };
        return cv::viz::Color(s(c.r), s(c.g), s(c.b));
    }

    inline std::string fmt_vec3(const Vector3D& v, int decimals = 2)
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(decimals);
        oss << "(" << v.x << "," << v.y << "," << v.z << ")";
        return oss.str();
    }

    inline double dist3(const Vector3D& a, const Vector3D& b)
    {
        double dx = a.x - b.x;
        double dy = a.y - b.y;
        double dz = a.z - b.z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }

    template<class T>
    inline T median(std::vector<T>& v)
    {
        if (v.empty()) return T{};
        std::size_t mid = v.size() / 2;
        std::nth_element(v.begin(), v.begin() + mid, v.end());
        return v[mid];
    }

} // namespace 场景实时显示_内部

struct 场景实时显示类::Impl {
    // ---- 外部输入兜底 ----
    场景显示参数 p0;

    // ---- OpenCV viz ----
    cv::viz::Viz3d win;
    bool 初始化 = false;

    // ---- 显示参数概念 ----
    存在节点类* 概念_显示参数 = nullptr;

    // 运行时配置（来自概念_显示参数；概念缺失则用 p0）
    场景显示参数 cfg;
    时间戳 上次拉取配置_ms = 0;

    // 依赖特征类型集合（来自 概念_显示参数 的子存在分组）
    struct 依赖类型集合 {
        std::vector<词性节点类*> 坐标_相对;
        std::vector<词性节点类*> 坐标_绝对;
        std::vector<词性节点类*> 尺寸_相对;
        std::vector<词性节点类*> 尺寸_绝对;
        std::vector<词性节点类*> 质量;
    } dep;

    // ---- 轨迹缓存 ----
    struct 轨迹记录 {
        std::vector<cv::Point3d> 点;
        Vector3D 上次位置{};
        bool 有上次位置 = false;
    };
    std::unordered_map<std::uintptr_t, 轨迹记录> 轨迹表;

    Impl(const 场景显示参数& 参数)
        : p0(参数), win("海鱼 | 场景实时显示")
    {
        cfg = p0;
    }

    // ================================
    // 读取/写入：概念_显示参数
    // ================================
    static bool 读标量I64(存在节点类* owner, 词性节点类* type, std::int64_t& out)
    {
        if (!owner || !type) return false;
        auto* f = 特征类::查找特征(owner, type);
        if (!f) return false;
        return 特征类::读取标量特征值(f, out);
    }

    static bool 读数值_转double(存在节点类* owner, 词性节点类* type, double& out)
    {
        if (!owner || !type) return false;
        auto* f = 特征类::查找特征(owner, type);
        if (!f || !f->主信息) return false;
        auto* fmi = dynamic_cast<特征节点主信息类*>(f->主信息);
        if (!fmi || !fmi->值 || !fmi->值->主信息) return false;
        auto* vmi = dynamic_cast<特征值主信息类*>(fmi->值->主信息);
        if (!vmi) return false;

        bool ok = false;
        std::visit([&](auto&& x) {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                ok = false;
            }
            else if constexpr (
                std::is_same_v<T, std::int64_t> ||
                std::is_same_v<T, std::uint64_t> ||
                std::is_same_v<T, double> ||
                std::is_same_v<T, float> ||
                std::is_same_v<T, std::uint8_t> ||
                std::is_same_v<T, std::int32_t> ||
                std::is_same_v<T, std::uint32_t>)
            {
                out = (double)x;
                ok = true;
            }
            else {
                ok = false;
            }
            }, vmi->值);
        return ok;
    }

    static bool 读颜色(存在节点类* owner, 词性节点类* type, 颜色_BGR8& out)
    {
        if (!owner || !type) return false;
        auto* f = 特征类::查找特征(owner, type);
        if (!f || !f->主信息) return false;
        auto* fmi = dynamic_cast<特征节点主信息类*>(f->主信息);
        if (!fmi || !fmi->值 || !fmi->值->主信息) return false;
        auto* vmi = dynamic_cast<特征值主信息类*>(fmi->值->主信息);
        if (!vmi) return false;
        if (auto* c = std::get_if<颜色_BGR8>(&vmi->值)) {
            out = *c;
            return true;
        }
        return false;
    }

    static 存在节点类* 找概念显示参数_已加锁()
    {
        // 约定：概念_显示参数 的主键固定。
        return dynamic_cast<存在节点类*>(世界链.查找主键("概念_显示参数"));
    }

    void 拉取显示配置(时间戳 now_ms)
    {
        // 低频刷新（默认 10Hz），避免每帧频繁扫描概念树。
        if (上次拉取配置_ms != 0 && now_ms - 上次拉取配置_ms < 100) return;
        上次拉取配置_ms = now_ms;

        cfg = p0; // 先铺兜底
        dep = {};

        auto lk = 世界链.获取锁();
        if (!概念_显示参数) {
            概念_显示参数 = 找概念显示参数_已加锁();
        }
        if (!概念_显示参数) return;

        auto 读开关 = [&](词性节点类* t, bool def) -> bool {
            std::int64_t v = 0;
            if (读标量I64(概念_显示参数, t, v)) return v != 0;
            return def;
            };
        auto 读参数 = [&](词性节点类* t, std::int64_t def) -> std::int64_t {
            std::int64_t v = 0;
            if (读标量I64(概念_显示参数, t, v)) return v;
            return def;
            };

        cfg.显示三维背景 = 读开关(类型_显示_开关_三维背景, cfg.显示三维背景);
        cfg.显示坐标轴 = 读开关(类型_显示_开关_坐标轴, cfg.显示坐标轴);
        cfg.显示网格地面 = 读开关(类型_显示_开关_网格地面, cfg.显示网格地面);
        cfg.显示相机模型 = 读开关(类型_显示_开关_相机模型, cfg.显示相机模型);

        cfg.显示实时点云 = 读开关(类型_显示_开关_实时点云, cfg.显示实时点云);

        cfg.显示存在框 = 读开关(类型_显示_开关_存在框, cfg.显示存在框);
        cfg.显示存在标签 = 读开关(类型_显示_开关_存在标签, cfg.显示存在标签);
        cfg.显示存在ID = 读开关(类型_显示_开关_存在ID, cfg.显示存在ID);
        cfg.显示存在名称 = 读开关(类型_显示_开关_存在名称, cfg.显示存在名称);
        cfg.显示存在大小 = 读开关(类型_显示_开关_存在大小, cfg.显示存在大小);
        cfg.显示存在坐标 = 读开关(类型_显示_开关_存在坐标, cfg.显示存在坐标);
        cfg.显示存在轨迹 = 读开关(类型_显示_开关_存在轨迹, cfg.显示存在轨迹);

        cfg.显示质量信息 = 读开关(类型_显示_开关_质量信息, cfg.显示质量信息);
        cfg.视野外淡化 = 读开关(类型_显示_开关_视野外淡化, cfg.视野外淡化);

        cfg.坐标模式 = 读参数(类型_显示_坐标模式, cfg.坐标模式);
        cfg.点云采样步长 = std::max<std::int64_t>(1, 读参数(类型_显示_点云采样步长, cfg.点云采样步长));

        cfg.FOV水平_百分度 = 读参数(类型_显示_FOV水平_百分度, cfg.FOV水平_百分度);
        cfg.FOV垂直_百分度 = 读参数(类型_显示_FOV垂直_百分度, cfg.FOV垂直_百分度);
        cfg.近裁剪_mm = std::max<std::int64_t>(1, 读参数(类型_显示_近裁剪_mm, cfg.近裁剪_mm));
        cfg.远裁剪_mm = std::max<std::int64_t>(cfg.近裁剪_mm + 1, 读参数(类型_显示_远裁剪_mm, cfg.远裁剪_mm));
        cfg.记忆淡化_ms = std::max<std::int64_t>(1, 读参数(类型_显示_记忆淡化_ms, cfg.记忆淡化_ms));
        cfg.轨迹长度 = std::max<std::int64_t>(1, 读参数(类型_显示_轨迹长度, cfg.轨迹长度));

        // 读取“依赖特征类型集合”分组：概念_显示参数/坐标_相对、坐标_绝对、尺寸_相对、尺寸_绝对、质量...
        auto* child = 概念_显示参数->子;
        if (!child) return;

        auto push_group = [&](std::vector<词性节点类*>& dst, 存在节点类* node) {
            if (!node || !node->主信息) return;
            auto* mi = dynamic_cast<存在节点主信息类*>(node->主信息);
            if (!mi) return;
            dst = mi->获取特征类型集合();
            };

        auto* it = child;
        do {
            auto* n = dynamic_cast<存在节点类*>(it);
            if (n && n->主信息) {
                auto* mi = dynamic_cast<存在节点主信息类*>(n->主信息);
                词性节点类* nm = mi ? mi->名称 : nullptr;
                if (nm == 类型_显示_依赖_坐标_相对) push_group(dep.坐标_相对, n);
                else if (nm == 类型_显示_依赖_坐标_绝对) push_group(dep.坐标_绝对, n);
                else if (nm == 类型_显示_依赖_尺寸_相对) push_group(dep.尺寸_相对, n);
                else if (nm == 类型_显示_依赖_尺寸_绝对) push_group(dep.尺寸_绝对, n);
                else if (nm == 类型_显示_依赖_质量) push_group(dep.质量, n);
            }
            it = it->下;
        } while (it && it != child);
    }

    // ================================
    // 写回显示配置（键盘交互：可选）
    // ================================
    bool 设置显示参数_I64(词性节点类* type, std::int64_t value)
    {
        if (!type) return false;
        auto lk = 世界链.获取锁();
        if (!概念_显示参数) 概念_显示参数 = 找概念显示参数_已加锁();
        if (!概念_显示参数) return false;

        auto* emi = dynamic_cast<存在节点主信息类*>(概念_显示参数->主信息);
        if (!emi) return false;

        特征节点类* feat = nullptr;
        for (auto* f : emi->特征索引) {
            if (!f || !f->主信息) continue;
            auto* fmi = dynamic_cast<特征节点主信息类*>(f->主信息);
            if (fmi && fmi->类型 == type) { feat = f; break; }
        }

        // 若缺失则补一个（理论上初始化概念时就有，这里做兜底）
      //  if (!feat) {
      //      auto* v = 特征值集.获取或创建标量特征值(nullptr, value);
      //      auto* fmi = new 特征节点主信息类(type, v);
      //      feat = dynamic_cast<特征节点类*>(世界链.添加节点_已加锁(fmi));
      //      if (!feat) return false;
      //      世界链.添加子节点_已加锁(概念_显示参数, feat);
      //      emi->特征索引.push_back(feat);
      //      emi->重建特征当前状态();
      //  }

        auto* fmi = dynamic_cast<特征节点主信息类*>(feat->主信息);
        if (!fmi) return false;
        fmi->值 = 特征值集.获取或创建标量特征值(nullptr, value);
        emi->重建特征当前状态();

        // 让下一帧立即刷新配置
        上次拉取配置_ms = 0;
        return true;
    }

    bool 切换开关(词性节点类* type)
    {
        std::int64_t v = 0;
        {
            auto lk = 世界链.获取锁();
            if (!概念_显示参数) 概念_显示参数 = 找概念显示参数_已加锁();
            if (!概念_显示参数) return false;
            (void)读标量I64(概念_显示参数, type, v);
        }
        return 设置显示参数_I64(type, v ? 0 : 1);
    }

    // ================================
    // 坐标/尺寸读取
    // ================================
    struct 三轴类型 { 词性节点类* x = nullptr; 词性节点类* y = nullptr; 词性节点类* z = nullptr; };

    static 三轴类型 推断三轴(const std::vector<词性节点类*>& types, 词性节点类* tx, 词性节点类* ty, 词性节点类* tz)
    {
        三轴类型 r;
        for (auto* t : types) {
            if (t == tx) r.x = tx;
            else if (t == ty) r.y = ty;
            else if (t == tz) r.z = tz;
        }
        if (!r.x) r.x = tx;
        if (!r.y) r.y = ty;
        if (!r.z) r.z = tz;
        return r;
    }

    static bool 读三轴_mm(存在节点类* owner, const 三轴类型& t, Vector3D& out_m)
    {
        std::int64_t vx = 0, vy = 0, vz = 0;
        if (!读标量I64(owner, t.x, vx)) return false;
        if (!读标量I64(owner, t.y, vy)) return false;
        if (!读标量I64(owner, t.z, vz)) return false;

        out_m.x = 三维场景管理类::毫米到米(vx);
        out_m.y = 三维场景管理类::毫米到米(vy);
        out_m.z = 三维场景管理类::毫米到米(vz);
        return true;
    }

    // ================================
    // 估计平移：P_abs ≈ P_rel + t
    // ================================
    std::optional<Vector3D> 估计平移_m(场景节点类* 场景)
    {
        if (!场景 || !场景->主信息) return std::nullopt;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return std::nullopt;

        三轴类型 t_rel = 推断三轴(dep.坐标_相对, 类型_相对位置X, 类型_相对位置Y, 类型_相对位置Z);
        三轴类型 t_abs = 推断三轴(dep.坐标_绝对, 类型_绝对位置X, 类型_绝对位置Y, 类型_绝对位置Z);

        std::vector<double> dx, dy, dz;
        dx.reserve(16); dy.reserve(16); dz.reserve(16);

        for (auto* e : smi->存在索引) {
            if (!e) continue;
            Vector3D pr{}, pa{};
            if (!读三轴_mm(e, t_rel, pr)) continue;
            if (!读三轴_mm(e, t_abs, pa)) continue;
            dx.push_back(pa.x - pr.x);
            dy.push_back(pa.y - pr.y);
            dz.push_back(pa.z - pr.z);
        }
        if (dx.empty()) return std::nullopt;

        using namespace 场景实时显示_内部;
        Vector3D t{};
        t.x = median(dx);
        t.y = median(dy);
        t.z = median(dz);
        return t;
    }

    // ================================
    // 视野判定（使用相对坐标）
    // ================================
    bool 在视野内(const Vector3D& p_rel_m) const
    {
        double z = p_rel_m.z;
        if (z <= 0.0) return false;
        double near_m = 三维场景管理类::毫米到米(cfg.近裁剪_mm);
        double far_m = 三维场景管理类::毫米到米(cfg.远裁剪_mm);
        if (z < near_m || z > far_m) return false;

        double hfov = 场景实时显示_内部::deg100_to_rad((double)cfg.FOV水平_百分度);
        double vfov = 场景实时显示_内部::deg100_to_rad((double)cfg.FOV垂直_百分度);

        double ax = std::atan2(p_rel_m.x, z);
        double ay = std::atan2(p_rel_m.y, z);
        if (std::abs(ax) > hfov * 0.5) return false;
        if (std::abs(ay) > vfov * 0.5) return false;
        return true;
    }

    // ================================
    // 初始化 + 回调
    // ================================
    static void 键盘回调(const cv::viz::KeyboardEvent& e, void* cookie)
    {
        if (e.action != cv::viz::KeyboardEvent::Action::KEY_DOWN) return;
        auto* self = static_cast<Impl*>(cookie);
        if (!self) return;

        switch (e.code) {
        case '1': self->切换开关(类型_显示_开关_实时点云); break;
        case '2': self->切换开关(类型_显示_开关_存在框); break;
        case '3': self->切换开关(类型_显示_开关_存在标签); break;
        case '4': self->切换开关(类型_显示_开关_存在坐标); break;
        case '5': self->切换开关(类型_显示_开关_存在大小); break;
        case '6': self->切换开关(类型_显示_开关_存在ID); break;
        case '7': self->切换开关(类型_显示_开关_存在名称); break;
        case '9': self->切换开关(类型_显示_开关_质量信息); break;
        case '0': self->切换开关(类型_显示_开关_视野外淡化); break;
        case '8': {
            // 坐标模式轮换 0/1/2
            std::int64_t mode = self->cfg.坐标模式;
            mode = (mode + 1) % 3;
            self->设置显示参数_I64(类型_显示_坐标模式, mode);
            break;
        }
        case 'B':
        case 'b': self->切换开关(类型_显示_开关_三维背景); break;
        default: break;
        }
    }

    void 确保初始化()
    {
        if (初始化) return;
        初始化 = true;

        win.registerKeyboardCallback(&Impl::键盘回调, this);

        // 默认背景
        win.setBackgroundColor(cv::viz::Color::black());
    }

    // ================================
    // HUD
    // ================================
    void 绘制HUD()
    {
        if (!cfg.显示三维背景) {
            try { win.removeWidget("hud"); }
            catch (...) {}
            return;
        }

        std::ostringstream oss;
        oss << "[1]点云:" << (cfg.显示实时点云 ? "ON" : "OFF")
            << "  [2]框:" << (cfg.显示存在框 ? "ON" : "OFF")
            << "  [3]标签:" << (cfg.显示存在标签 ? "ON" : "OFF")
            << "  [4]坐标:" << (cfg.显示存在坐标 ? "ON" : "OFF")
            << "  [5]大小:" << (cfg.显示存在大小 ? "ON" : "OFF")
            << "  [6]ID:" << (cfg.显示存在ID ? "ON" : "OFF")
            << "  [7]名:" << (cfg.显示存在名称 ? "ON" : "OFF")
            << "  [8]坐标模式:" << cfg.坐标模式
            << "  [9]质量:" << (cfg.显示质量信息 ? "ON" : "OFF")
            << "  [0]淡化:" << (cfg.视野外淡化 ? "ON" : "OFF")
            << "  [B]背景:" << (cfg.显示三维背景 ? "ON" : "OFF");

        cv::viz::WText text(oss.str(), cv::Point(10, 20), 16, cv::viz::Color::white());
        win.showWidget("hud", text);
    }

    // ================================
    // 每帧渲染
    // ================================
    void 更新显示(场景节点类* 当前场景, const 结构体_原始场景帧* 实时帧)
    {
        确保初始化();

        // 当前时间戳：优先用帧时间（更能反映“最后观测时间”的对齐）
        if(当前场景->子节点数量<2){
            return;
		}
        时间戳 now_ms = 0;
        if (实时帧) {
            now_ms = (时间戳)实时帧->时间;
        }
        if (now_ms == 0) now_ms = 场景实时显示_内部::当前时间戳_ms_稳态();

        拉取显示配置(now_ms);

        // 3D 背景
        if (cfg.显示三维背景) {
            if (cfg.显示坐标轴) {
                win.showWidget("axis", cv::viz::WCoordinateSystem(0.5));
            }
            else {
                try { win.removeWidget("axis"); }
                catch (...) {}
            }
        }
        else {
            try { win.removeWidget("axis"); }
            catch (...) {}
        }

        // 估计平移（用于 abs 渲染时将实时点云对齐到 abs）
        Vector3D t_abs_from_rel{};
        bool 有平移 = false;
        if (cfg.坐标模式 != 0) {
            if (auto t = 估计平移_m(当前场景)) {
                t_abs_from_rel = *t;
                有平移 = true;
            }
        }

        // 实时点云（背景）
        if (cfg.显示实时点云 && 实时帧) {
            std::vector<cv::Point3f> pts;
            std::vector<cv::Vec3b> cols;

            const int step = (int)std::max<std::int64_t>(1, cfg.点云采样步长);

            for (std::size_t i = 0; i < 实时帧->点云.size(); i += (std::size_t)step) {
                const auto& p = 实时帧->点云[i];
                if (!std::isfinite(p.x) || !std::isfinite(p.y) || !std::isfinite(p.z)) continue;
                if (p.z <= 0.0f || p.z > 6.0f) continue;

                cv::Point3f q(static_cast<float>(p.x), static_cast<float>(p.y), static_cast<float>(p.z));
                if (cfg.坐标模式 != 0 && 有平移) {
                    q.x = (float)(q.x + t_abs_from_rel.x);
                    q.y = (float)(q.y + t_abs_from_rel.y);
                    q.z = (float)(q.z + t_abs_from_rel.z);
                }

                pts.push_back(q);
                cols.push_back(cv::Vec3b(255, 255, 255));
            }

            cv::viz::WCloud cloud(pts, cols);
            win.showWidget("cloud", cloud);
        }
        else {
            try { win.removeWidget("cloud"); }
            catch (...) {}
        }

        // 当前场景不存在则退出绘制
        if (!当前场景 || !当前场景->主信息) {
            绘制HUD();
            win.spinOnce(1, true);
            return;
        }

        auto* smi = dynamic_cast<场景节点主信息类*>(当前场景->主信息);
        if (!smi) {
            绘制HUD();
            win.spinOnce(1, true);
            return;
        }

        // 用于清理旧 widget
        std::unordered_set<std::string> alive;
        alive.reserve(smi->存在索引.size() * 2 + 8);

        三轴类型 t_rel = 推断三轴(dep.坐标_相对, 类型_相对位置X, 类型_相对位置Y, 类型_相对位置Z);
        三轴类型 t_abs = 推断三轴(dep.坐标_绝对, 类型_绝对位置X, 类型_绝对位置Y, 类型_绝对位置Z);

        三轴类型 s_rel = 推断三轴(dep.尺寸_相对, 类型_相对尺寸X, 类型_相对尺寸Y, 类型_相对尺寸Z);
        三轴类型 s_abs = 推断三轴(dep.尺寸_绝对, 类型_绝对尺寸X, 类型_绝对尺寸Y, 类型_绝对尺寸Z);

        // 遍历所有存在（场景内“记录过的”都要显示）
        for (auto* ex : smi->存在索引) {
            if (!ex || !ex->主信息) continue;
            auto* emi = dynamic_cast<存在节点主信息类*>(ex->主信息);
            if (!emi) continue;

            Vector3D pr{}, pa{}, sr{}, sa{};
            bool has_pr = 读三轴_mm(ex, t_rel, pr);
            bool has_pa = 读三轴_mm(ex, t_abs, pa);
            bool has_sr = 读三轴_mm(ex, s_rel, sr);
            bool has_sa = 读三轴_mm(ex, s_abs, sa);

            // 渲染坐标：根据 cfg.坐标模式 选择
            Vector3D center = pr;
            Vector3D size = sr;

            if (cfg.坐标模式 != 0) {
                if (has_pa) center = pa;
                else if (has_pr && 有平移) {
                    center.x = pr.x + t_abs_from_rel.x;
                    center.y = pr.y + t_abs_from_rel.y;
                    center.z = pr.z + t_abs_from_rel.z;
                }

                if (has_sa) size = sa;
                else size = sr;
            }

            // 防御：viz/VTK 对 NaN/Inf 非常敏感，宁可跳过也不要喂进去导致 abort
            if (!场景实时显示_内部::finite3(center) || !场景实时显示_内部::finite3(size)) {
                continue;
            }
            if (size.x <= 0 || size.y <= 0 || size.z <= 0) {
                continue;
            }

            // 视野判定始终用相对坐标（相机视锥）
            bool in_view = has_pr ? 在视野内(pr) : false;

            // 颜色（有平均色则用平均色，否则用白）
            颜色_BGR8 col{ 255, 255, 255 };
            (void)读颜色(ex, 类型_平均颜色, col);

            // 淡化
            double alpha = 1.0;
            if (!in_view && cfg.视野外淡化) {
                时间戳 last = emi->最后观测时间;
                if (last == 0 || last > now_ms) last = now_ms;
                double age = (double)(now_ms - last);
                alpha = 1.0 - (age / (double)cfg.记忆淡化_ms);
                alpha = 0.05 + 0.95 * 场景实时显示_内部::clamp01(alpha);
            }

            // 盒子
            std::string wid_box = "ex_box_" + ex->主键;
            std::string wid_txt = "ex_txt_" + ex->主键;
            std::string wid_traj = "ex_traj_" + ex->主键;

            if (cfg.显示存在框) {
                Vector3D half{ size.x * 0.5, size.y * 0.5, size.z * 0.5 };
                Vector3D minp{ center.x - half.x, center.y - half.y, center.z - half.z };
                Vector3D maxp{ center.x + half.x, center.y + half.y, center.z + half.z };

                cv::viz::WCube cube(cv::Point3d(minp.x, minp.y, minp.z), cv::Point3d(maxp.x, maxp.y, maxp.z), true, 场景实时显示_内部::toVizColor(col, alpha));
                win.showWidget(wid_box, cube);
                alive.insert(wid_box);
            }
            else {
                try { win.removeWidget(wid_box); }
                catch (...) {}
            }

            // 轨迹
            if (cfg.显示存在轨迹) {
                auto key = (std::uintptr_t)ex;
                auto& tr = 轨迹表[key];
                if (!tr.有上次位置 || 场景实时显示_内部::dist3(tr.上次位置, center) > 0.01) {
                    tr.点.push_back(cv::Point3d(center.x, center.y, center.z));
                    tr.上次位置 = center;
                    tr.有上次位置 = true;
                    if ((std::int64_t)tr.点.size() > cfg.轨迹长度) {
                        tr.点.erase(tr.点.begin(), tr.点.begin() + (tr.点.size() - (std::size_t)cfg.轨迹长度));
                    }
                }

                if (tr.点.size() >= 2) {
                    cv::viz::WPolyLine line(tr.点, 场景实时显示_内部::toVizColor(col, alpha));
                    win.showWidget(wid_traj, line);
                    alive.insert(wid_traj);
                }
            }
            else {
                try { win.removeWidget(wid_traj); }
                catch (...) {}
            }

            // 标签
            if (cfg.显示存在标签) {
                std::ostringstream oss;
                bool first = true;

                if (cfg.显示存在名称) {
                    if (emi->概念模板) {
                        oss << (first ? "" : " ") << "N:" << emi->概念模板->主键;
                        first = false;
                    }
                }
                if (cfg.显示存在ID) {
                    oss << (first ? "" : " ") << "ID:" << ex->主键;
                    first = false;
                }

                if (cfg.显示存在坐标) {
                    if (cfg.坐标模式 == 0) {
                        if (has_pr) {
                            oss << (first ? "" : " ") << "P_rel=" << 场景实时显示_内部::fmt_vec3(pr);
                            first = false;
                        }
                    }
                    else if (cfg.坐标模式 == 1) {
                        if (has_pa) {
                            oss << (first ? "" : " ") << "P_abs=" << 场景实时显示_内部::fmt_vec3(pa);
                            first = false;
                        }
                    }
                    else {
                        if (has_pa) {
                            oss << (first ? "" : " ") << "P_abs=" << 场景实时显示_内部::fmt_vec3(pa);
                            first = false;
                        }
                        if (has_pr) {
                            oss << (first ? "" : " ") << "(rel=" << 场景实时显示_内部::fmt_vec3(pr) << ")";
                            first = false;
                        }
                    }
                }

                if (cfg.显示存在大小) {
                    oss << (first ? "" : " ") << "S=" << 场景实时显示_内部::fmt_vec3(size);
                    first = false;
                }

                if (cfg.显示质量信息) {
                    double q = 0.0;
                    if (读数值_转double(ex, 类型_观测质量分, q) || 读数值_转double(ex, 类型_质量, q)) {
                        oss << (first ? "" : " ") << "Q=" << std::fixed << std::setprecision(2) << q;
                        first = false;
                    }
                }

                // 文本位置：在盒子上方一点
                Vector3D textPos{ center.x, center.y + size.y * 0.6, center.z };
                cv::viz::WText3D text(oss.str(), cv::Point3d(textPos.x, textPos.y, textPos.z), 0.05, true, 场景实时显示_内部::toVizColor(col, alpha));
                win.showWidget(wid_txt, text);
                alive.insert(wid_txt);
            }
            else {
                try { win.removeWidget(wid_txt); }
                catch (...) {}
            }
        }

        // 清理：不再存在于当前场景索引的 widget
        //（注意：场景里“所有记录存在”都显示，因此一般不会频繁删。这里只处理存在被真正移除的情况。）
        {
            std::vector<std::string> to_remove;
            //   for (auto kv : win.getWidgetIterator()) {
            //       (void)kv;
            //   }
        }

        绘制HUD();
        win.spinOnce(1, true);
    }
};

// ================================
// 4) 导出实现
// ================================
场景实时显示类::场景实时显示类(const 场景显示参数& 参数)
    : impl_(std::make_unique<Impl>(参数))
{
}



void 场景实时显示类::更新显示(场景节点类* 当前场景, const 结构体_原始场景帧* 实时帧)
{
    impl_->更新显示(当前场景, 实时帧);
}
