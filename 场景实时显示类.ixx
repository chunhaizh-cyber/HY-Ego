module;

// OpenCV Viz 实现（仅在需要时引入）
#include <opencv2/opencv.hpp>
#include <opencv2/viz.hpp>
#include <opencv2/core.hpp>

export module 场景实时显示模块;

import 相机接口模块;
import 基础数据类型模块;
import 世界树模块;
import 语素模块;
import 三维场景管模块;   // 用于轴取最近值等辅助函数
import 主信息定义模块; // 用于结构体_原始场景帧
import 宇宙环境模块; // 用于时间戳等辅助函数    
import 宇宙链模块;   // 用于在宇宙链上定位“显示特征信息集合”概念节点

import <vector>;
import <string>;
import <memory>;
import <cmath>;
import <algorithm>;
import <cstdint>;
import <limits>;

export struct 场景显示参数 {
    // ===== 显示内容开关 =====
    bool 显示坐标轴 = true;
    bool 显示相机方向箭头 = true;

    bool 显示实时点云 = true;
    int  点云采样步长 = 3;
    bool 点云忽略无效点 = true;

    bool 显示存在占位盒 = true;
    bool 显示存在标签 = true;

    float 记忆淡化时间秒 = 10.0f;     // 超过此时间后完全透明（可调）

    // ===== 相机视角初始化 =====
    float 初始相机距离 = 1.2f;
    float 初始相机俯仰 = -15.0f;      // 度
    float 初始相机偏航 = 25.0f;       // 度
};

struct 显示用存在信息 {
    Vector3D 中心{ 0,0,0 };
    Vector3D 尺寸{ 0.1f,0.1f,0.1f };
    Color    颜色{ 200, 200, 255 };
    std::string 标签;
    double   最近观测时间秒 = 0.0;
    bool     有效 = false;
};

export class 场景实时显示器 {
public:
    explicit 场景实时显示器(场景显示参数 p = {});
    ~场景实时显示器() = default;

    bool 初始化();

    // 新接口：实时点云可选，存在完全从世界树当前场景读取
    bool 更新(const 结构体_原始场景帧* 实时帧 = nullptr,
        场景节点类* 当前场景根 = nullptr);

    bool 仍在运行() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

namespace {
    inline bool 有限(float v) { return std::isfinite((double)v); }

    inline double 当前时间秒() {
        // 假设你有全局时间戳函数，或用 std::chrono
        // 这里用一个简单示例（单位：秒）
        static auto start = std::chrono::high_resolution_clock::now();
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(now - start).count();
    }
}

struct 场景实时显示器::Impl {
    场景显示参数 p;
    cv::viz::Viz3d win{ "HY-Ego World Memory (D455)" };
    bool inited = false;
    // 新增：每个存在的轨迹点缓存（最近 N 个位置）
   // key 用存在节点指针（uintptr_t 转 int64_t 确保唯一）
    std::map<uint64_t, std::vector<cv::Point3f>> 存在轨迹缓存;

    static constexpr int 最大轨迹点数 = 30;  // 保留最近 30 个位置（约 1 秒 @30fps）
    static constexpr double 轨迹淡化时间秒 = 8.0;  // 超过此时间的老轨迹点变淡

    int 上帧存在数 = 0;

    explicit Impl(场景显示参数 pp) : p(pp) {}

    static cv::Point3f toP(const Vector3D& v) {
        return { (float)v.x, (float)v.y, (float)v.z };
    }

    static cv::viz::Color toVizColor(const Color& c, double alpha = 1.0) {
        return cv::viz::Color(
            (uint8_t)(c.r * alpha),
            (uint8_t)(c.g * alpha),
            (uint8_t)(c.b * alpha)
        );
    }

    static void RemoveWidgetIfExists(cv::viz::Viz3d& w, const std::string& id) noexcept {
        try { w.removeWidget(id); }
        catch (...) {}
    }

    void show_base_widgets() {
        if (p.显示坐标轴) {
            win.showWidget("axis", cv::viz::WCoordinateSystem(0.3));
        }
        if (p.显示相机方向箭头) {
            cv::Point3f o(0, 0, 0);
            cv::Point3f f(0, 0, 0.4f);
            win.showWidget("cam_dir", cv::viz::WArrow(o, f, 0.012, cv::viz::Color::red()));
        }
    }

    void set_default_view() {
        const float r = std::max(0.3f, p.初始相机距离);
        const float pitch = p.初始相机俯仰 * 3.1415926f / 180.0f;
        const float yaw = p.初始相机偏航 * 3.1415926f / 180.0f;

        cv::Point3f camPos(
            r * std::cos(pitch) * std::sin(yaw),
            r * std::sin(pitch),
            -r * std::cos(pitch) * std::cos(yaw)
        );
        cv::Point3f camFocal(0, 0, 0.7f);
        cv::Point3f camUp(0, -1, 0);

        win.setViewerPose(cv::viz::makeCameraPose(
            cv::Vec3d(camPos.x, camPos.y, camPos.z),
            cv::Vec3d(camFocal.x, camFocal.y, camFocal.z),
            cv::Vec3d(camUp.x, camUp.y, camUp.z)
        ));
    }

    void update_cloud(const 结构体_原始场景帧& 帧) {
        if (!p.显示实时点云) {
            RemoveWidgetIfExists(win, "cloud");
            return;
        }

        const int w = 帧.宽度, h = 帧.高度;
        if (w <= 0 || h <= 0 || 帧.点云.size() != static_cast<size_t>(w * h)) {
            RemoveWidgetIfExists(win, "cloud");
            return;
        }

        const int step = std::max(1, p.点云采样步长);
        std::vector<cv::Point3f> pts;
        pts.reserve((w * h) / (step * step));

        for (int v = 0; v < h; v += step) {
            for (int u = 0; u < w; u += step) {
                const auto& P = 帧.点云[v * w + u];
                if (p.点云忽略无效点 && P.z <= 0.0f) continue;
                pts.emplace_back(toP(P));
            }
        }

        if (pts.empty()) {
            RemoveWidgetIfExists(win, "cloud");
            return;
        }

        cv::Mat cloudMat((int)pts.size(), 1, CV_32FC3, pts.data());
        cv::viz::WCloud cloud(cloudMat, cv::viz::Color::gray());
        cloud.setRenderingProperty(cv::viz::POINT_SIZE, 1.0);
        win.showWidget("cloud", cloud);
    }

    // ============================================================
    // 从“概念_显示特征信息集合”读取需要显示的特征类型列表
    // 说明：你现在把 S(C) 存在于 概念存在的 主信息::特征类型集合 中，
    //       显示模块只需要按这个集合来找存在子链上的特征值即可。
    // ============================================================
    static 存在节点类* 宇宙链_按主键查找(const std::string& key)
    {
        if (!宇宙链.根指针) return nullptr;
        存在节点类* root = 宇宙链.根指针;
        存在节点类* cur = root;
        do {
            if (cur && cur->主键 == key) return cur;
            cur = cur ? cur->链下 : nullptr;
        } while (cur && cur != root);
        return nullptr;
    }

    static 存在节点类* 获取显示特征信息集合概念节点()
    {
        // 你初始化时建议把该概念节点主键固定为“概念_显示特征信息集合”
        static 存在节点类* cached = nullptr;
        if (cached) return cached;

        if (auto* n = dynamic_cast<存在节点类*>(宇宙链_按主键查找("概念_显示特征信息集合"))) return cached = n;
        if (auto* n = dynamic_cast<存在节点类*>(宇宙链_按主键查找("显示特征信息集合"))) return cached = n;
        return nullptr;
    }

    static std::vector<词性节点类*> 获取显示特征类型节点集()
    {
        std::vector<词性节点类*> out;

        if (auto* 概念 = 获取显示特征信息集合概念节点()) {
            if (auto* mi = dynamic_cast<存在节点主信息类*>((基础信息基类*)概念->主信息)) {
                for (auto* t : mi->特征类型集合) {
                    if (t) out.push_back(t);
                }
            }
        }

        // 如果概念节点还没初始化出来，则降级使用默认“显示特征集合”
        if (out.empty()) {
            out = {
                语素集.添加词性词("位置坐标X轴", "名词"),
                语素集.添加词性词("位置坐标Y轴", "名词"),
                语素集.添加词性词("位置坐标Z轴", "名词"),
                语素集.添加词性词("尺寸_左右",   "名词"),
                语素集.添加词性词("尺寸_上下",   "名词"),
                语素集.添加词性词("尺寸_前后",   "名词"),
                语素集.添加词性词("平均颜色",   "名词"),
                语素集.添加词性词("最近观测时间", "名词"),
                语素集.添加词性词("观测次数",   "名词")
            };
        }

        // 去重（按主键字典序）保证稳定
        std::sort(out.begin(), out.end(), [](词性节点类* a, 词性节点类* b) {
            if (!a) return true;
            if (!b) return false;
            return a->主键 < b->主键;
            });
        out.erase(std::unique(out.begin(), out.end(), [](词性节点类* a, 词性节点类* b) {
            if (!a || !b) return a == b;
            return a->主键 == b->主键;
            }), out.end());

        return out;
    }

    static bool TryGetScalarFeature(存在节点类* 存在, 词性节点类* 类型, int64_t& out)
    {
        if (!存在 || !类型) return false;
        特征节点类* feat = g_宇宙.世界树.查找特征节点(存在, 类型);
        if (!feat) return false;

        auto* info = dynamic_cast<特征节点主信息类*>(feat->主信息);
        if (!info || !info->值) return false;

        if (auto* scalar = dynamic_cast<标量特征值主信息类*>(info->值->主信息)) {
            out = scalar->值;
            return true;
        }
        return false;
    }

    显示用存在信息 从存在节点提取(存在节点类* 存在) {
        显示用存在信息 info;
        if (!存在) return info;

        const auto 显示类型集 = 获取显示特征类型节点集();

        // 默认值（避免空特征导致显示炸裂）
        double cx = 0, cy = 0, cz = 0;
        double sx = 0.2, sy = 0.5, sz = 0.2;
        double lastTimeMs = 0.0;
        int64_t rgb = 0;
        int64_t obs_count = 0;

        for (auto* T : 显示类型集) {
            if (!T) continue;
            const std::string& 名 = T->主键;

            if (名 == "位置坐标X轴") {
                三维场景管理类::轴取最近值(存在, T, 0.0, cx);
            }
            else if (名 == "位置坐标Y轴") {
                三维场景管理类::轴取最近值(存在, T, 0.0, cy);
            }
            else if (名 == "位置坐标Z轴") {
                三维场景管理类::轴取最近值(存在, T, 0.0, cz);
            }
            else if (名 == "尺寸_左右") {
                double temp = sx;
                if (三维场景管理类::轴特征值在范围内(存在, T, 0.01, 10.0)) {
                    三维场景管理类::轴取最近值(存在, T, sx, temp);
                    sx = temp;
                }
            }
            else if (名 == "尺寸_上下") {
                double temp = sy;
                if (三维场景管理类::轴特征值在范围内(存在, T, 0.01, 10.0)) {
                    三维场景管理类::轴取最近值(存在, T, sy, temp);
                    sy = temp;
                }
            }
            else if (名 == "尺寸_前后") {
                double temp = sz;
                if (三维场景管理类::轴特征值在范围内(存在, T, 0.01, 10.0)) {
                    三维场景管理类::轴取最近值(存在, T, sz, temp);
                    sz = temp;
                }
            }
            else if (名 == "平均颜色") {
                TryGetScalarFeature(存在, T, rgb);
            }
            else if (名 == "最近观测时间") {
                三维场景管理类::轴取最近值(存在, T, 0.0, lastTimeMs);
            }
            else if (名 == "观测次数") {
                TryGetScalarFeature(存在, T, obs_count);
            }
            else {
                // 其它显示特征（轮廓/贴图/子轮廓…）当前显示器暂不直接消费。
            }
        }

        info.中心 = { (float)cx, (float)cy, (float)cz };
        info.尺寸 = { (float)sx, (float)sy, (float)sz };

        if (rgb) {
            uint32_t u = static_cast<uint32_t>(rgb);
            info.颜色 = { (uint8_t)(u >> 16), (uint8_t)(u >> 8), (uint8_t)u };
        }

        if (lastTimeMs > 0.0) {
            info.最近观测时间秒 = lastTimeMs / 1000.0;
        }

        // ===== 生成丰富标签 =====
        uintptr_t id_val = reinterpret_cast<uintptr_t>(存在);
        double age_sec = 0.0;
        if (obs_count > 0 && info.最近观测时间秒 > 0.0) {
            age_sec = (当前时间秒() - info.最近观测时间秒);
            if (age_sec < 0.0) age_sec = 0.0;
        }
        int age_int = static_cast<int>(age_sec);

        char buf[128];
        std::snprintf(buf, sizeof(buf), "ID:%05u | Obs:%lld | Age:%ds",
            static_cast<unsigned int>(id_val % 100000),
            (long long)obs_count,
            age_int);
        info.标签 = buf;

        if (age_sec > p.记忆淡化时间秒 * 0.7) {
            info.颜色 = { 150, 150, 150 };
        }

        info.有效 = (info.中心.z > 0.05f && info.尺寸.x > 0.02f && info.尺寸.y > 0.02f && info.尺寸.z > 0.02f);
        return info;
    }


    void update_memory_objects(场景节点类* 当前场景根) {

        std::vector<存在节点类*> exists;

        std::vector<显示用存在信息> 存在列表;

        if (当前场景根) {

            基础信息节点类* child = 当前场景根->子;
            if (child) {
                基础信息节点类* cur = child;
                do {
                    if (auto* exist = dynamic_cast<存在节点类*>(cur)) {
                        auto info = 从存在节点提取(exist);
                        // if (info.有效) {
                        if (1) {
                            存在列表.push_back(std::move(info));
                        }
                    }
                    cur = cur->下;
                } while (cur != child);
            }
        }

        // 移除多余的旧对象
        for (int i = (int)存在列表.size(); i < 上帧存在数; ++i) {
            RemoveWidgetIfExists(win, "mem_box_" + std::to_string(i));
            RemoveWidgetIfExists(win, "mem_txt_" + std::to_string(i));
        }
        上帧存在数 = (int)存在列表.size();

        double now = 当前时间秒();

        for (int i = 0; i < (int)存在列表.size(); ++i) {
            const auto& o = 存在列表[i];
            const std::string boxId = "mem_box_" + std::to_string(i);
            const std::string txtId = "mem_txt_" + std::to_string(i);

            double age = now - o.最近观测时间秒;
            double alpha = std::max(0.15, 1.0 - age / p.记忆淡化时间秒);

            cv::viz::Color col = toVizColor(o.颜色, alpha);

            float cx = (float)o.中心.x, cy = (float)o.中心.y, cz = (float)o.中心.z;
            float hx = (float)o.尺寸.x * 0.5f, hy = (float)o.尺寸.y * 0.5f, hz = (float)o.尺寸.z * 0.5f;

            cv::Point3f pmin(cx - hx, cy - hy, cz - hz);
            cv::Point3f pmax(cx + hx, cy + hy, cz + hz);

            if (p.显示存在占位盒) {
                cv::viz::WCube cube(pmin, pmax, true /*wireframe*/, col);
                cube.setRenderingProperty(cv::viz::LINE_WIDTH, 2.5);
                cube.setRenderingProperty(cv::viz::OPACITY, alpha);
                win.showWidget(boxId, cube);
            }

            if (p.显示存在标签) {
                cv::Point3f labelPos(cx, cy + hy + 0.08f, cz);  // 稍高一点，避免遮挡盒子

                cv::viz::WText3D txt(o.标签, labelPos, 0.035, true, cv::viz::Color::white());
                txt.setRenderingProperty(cv::viz::OPACITY, alpha);  // 与盒子同步淡化
                //  txt.setRenderingProperty(cv::viz::RENDERING_PROPERTY_FACE_CAMERA, true);//让标签始终朝向相机（更易读）


                win.showWidget(txtId, txt);
            }
            else {
                RemoveWidgetIfExists(win, txtId);
            }
        }
    }

    bool init() {
        if (inited) return true;
        win.setBackgroundColor(cv::viz::Color::black());
        show_base_widgets();
        set_default_view();
        inited = true;
        return true;
    }

    bool tick(const 结构体_原始场景帧* 实时帧, 场景节点类* 当前场景根) {
        if (!inited) init();

        if (实时帧) {
            update_cloud(*实时帧);
        }
        else {
            RemoveWidgetIfExists(win, "cloud");
        }

        update_memory_objects(当前场景根);
        //  double now = 当前时间秒();
          //update_trajectories(存在列表, now);  // 新增这一行

        win.spinOnce(1, true);
        return !win.wasStopped();
    }

    static int64_t 获取观测次数(存在节点类* 存在)
    {
        static 词性节点类* T_观测次数 = nullptr;
        if (!T_观测次数) {
            T_观测次数 = 语素集.添加词性词("观测次数", "名词");
        }

        特征节点类* feat = g_宇宙.世界树.查找特征节点(存在, T_观测次数);
        if (!feat) return 0;

        auto* info = dynamic_cast<特征节点主信息类*>(feat->主信息);
        if (!info || !info->值) return 0;

        if (auto* scalar = dynamic_cast<标量特征值主信息类*>(info->值->主信息)) {
            return scalar->值;
        }
        return 0;
    }

    /*  void update_trajectories(const std::vector<显示用存在信息>& 存在列表, double now)
      {
          // 先清理所有旧轨迹（简单粗暴，每帧重绘）
          for (int i = 0; i < 上帧存在数; ++i) {
              RemoveWidgetIfExists(win, "traj_" + std::to_string(i));
          }

          int idx = 0;
          for (const auto& o : 存在列表) {
              if (!o.有效) continue;

              // 用存在节点地址作为唯一 key（从标签反推或直接传，下面用标签里的 ID）
              uint64_t key = std::stoull(o.标签.substr(3, 10), nullptr, 16);  // 从 "ID:xxxxx" 取数字部分

              auto& traj = 存在轨迹缓存[key];

              // 添加当前最新位置
              cv::Point3f newPt((float)o.中心.x, (float)o.中心.y, (float)o.中心.z);
              traj.push_back(newPt);

              // 限制长度
              if (traj.size() > 最大轨迹点数) {
                  traj.erase(traj.begin());
              }

              // 如果点够多，才画线
              if (traj.size() >= 2) {
                  // 计算每段颜色的透明度（旧点更淡）
                  std::vector<cv::viz::Color> colors;
                  colors.reserve(traj.size());
                  for (size_t k = 0; k < traj.size(); ++k) {
                      double pointAge = now - (info.最近观测时间秒 + k * 0.033);  // 粗估
                      double a = std::max(0.1, 1.0 - pointAge / 轨迹淡化时间秒);
                      colors.emplace_back(255 * a, 200 * a, 50 * a);  // 橙黄色渐淡
                  }

                  cv::Mat pointsMat((int)traj.size(), 1, CV_32FC3, traj.data());
                  cv::Mat colorsMat((int)colors.size(), 1, CV_8UC3, colors.data());

                  cv::viz::WPolyLine line(pointsMat, colorsMat);
                  line.setRenderingProperty(cv::viz::LINE_WIDTH, 3.0);

                  win.showWidget("traj_" + std::to_string(idx), line);
              }

              ++idx;
          }

          // 可选：清理太老的缓存（长时间不动或消失的存在）
          // （省略，实际可加 age 判断清除）
      }*/
};

// =========================
// 外部接口实现
// =========================
inline 场景实时显示器::场景实时显示器(场景显示参数 p)
    : impl_(std::make_unique<Impl>(p)) {
}

inline bool 场景实时显示器::初始化() {
    return impl_ ? impl_->init() : false;
}

inline bool 场景实时显示器::更新(const 结构体_原始场景帧* 实时帧, 场景节点类* 当前场景根) {
    return impl_ ? impl_->tick(实时帧, 当前场景根) : false;
}

inline bool 场景实时显示器::仍在运行() const {
    return impl_ ? !impl_->win.wasStopped() : false;
}