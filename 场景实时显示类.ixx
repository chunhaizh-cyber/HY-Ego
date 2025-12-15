module;

// 只有你定义了 启用_OPENCV_VIZ 才会真的引入 OpenCV/viz
// 否则整个模块会退化为空实现，但仍可编译通过
#include <opencv2/opencv.hpp>
#include  <opencv2/viz.hpp>
#include <opencv2/core.hpp>


export module 场景实时显示模块;

import 相机接口模块;
import 存在提取模块;
import 基础数据类型模块;

import <vector>;
import <string>;
import <memory>;
import <cmath>;
import <algorithm>;
import <cstdint>;

export struct 场景显示参数 {
    // ===== 显示内容开关 =====
    bool 显示坐标轴 = true;
    bool 显示相机方向箭头 = true;

    bool 显示点云 = true;
    int  点云采样步长 = 3;           // 越大越快（建议 2~6）
    bool 点云忽略无效点 = true;

    bool 显示存在占位盒 = true;
    bool 显示存在轮廓点云 = false;    // 需要 观测.轮廓3D 有数据
    float 轮廓点大小 = 2.0f;

    bool 显示存在标签 = false;

    // ===== 相机视角初始化（可选）=====
    float 初始相机距离 = 1.2f;       // 仅用于一个“好看”的默认视角
    float 初始相机俯仰 = -15.0f;     // 度
    float 初始相机偏航 = 25.0f;     // 度
};

export class 场景实时显示器 {
public:
    explicit 场景实时显示器(场景显示参数 p = {});
    ~场景实时显示器() = default;

    // 创建窗口等资源
    bool 初始化();

    // 每帧调用：传入相机帧 + 观测列表
    bool 更新(const 结构体_原始场景帧& 帧, const std::vector<存在观测>& 观测列表);

    // 窗口是否仍在运行（用户点 X 关闭后返回 false）
    bool 仍在运行() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

namespace {
    inline bool 有限(float v) { return std::isfinite((double)v); }

    inline bool 观测有效_用于显示(const 存在观测& o) {
        // 你“存在提取”里至少要有中心/尺寸，否则整条观测无效
        if (!有限(o.中心.x) || !有限(o.中心.y) || !有限(o.中心.z)) return false;
        if (!有限(o.尺寸.x) || !有限(o.尺寸.y) || !有限(o.尺寸.z)) return false;
        if (o.中心.z <= 0.0f) return false;                // 深度相机：z<=0 通常是无效点
        if (o.尺寸.x <= 0.0001f || o.尺寸.y <= 0.0001f || o.尺寸.z <= 0.0001f) return false;
        return true;
    }
}



// =========================
// OpenCV viz 真·实现
// =========================
struct 场景实时显示器::Impl {
    场景显示参数 p;
    cv::viz::Viz3d win{ "Ego Scene (D455)" };
    bool inited = false;

    int 上帧对象数 = 0;

    explicit Impl(场景显示参数 pp) : p(pp) {}

    static cv::Point3f toP(const Vector3D& v) {
        return { (float)v.x, (float)v.y, (float)v.z };
    }

    static cv::viz::Color toVizColor(const Color& c) {
        // 这里按“r,g,b”理解（你 存在观测 注释也是 r,g,b）【:contentReference[oaicite:1]{index=1}】
        return cv::viz::Color(c.r, c.g, c.b);
    }

    static void RemoveWidgetIfExists(cv::viz::Viz3d& w, const std::string& id) noexcept {
        try {
            w.removeWidget(id);
        }
        catch (const cv::Exception&) {
            // ignore: widget not found
        }
    }

    void show_base_widgets() {
        if (p.显示坐标轴) {
            win.showWidget("axis", cv::viz::WCoordinateSystem(0.25));
        }
        if (p.显示相机方向箭头) {
            cv::Point3f o(0, 0, 0);
            cv::Point3f f(0, 0, 0.35f); // +Z 前方
            win.showWidget("cam_dir", cv::viz::WArrow(o, f, 0.01, cv::viz::Color::red()));
        }
    }

    void set_default_view() {
        // 给一个“能看到前方”的默认视角（用户可鼠标拖动）
        const float r = std::max(0.2f, p.初始相机距离);
        const float pitch = p.初始相机俯仰 * 3.1415926f / 180.0f;
        const float yaw = p.初始相机偏航 * 3.1415926f / 180.0f;

        cv::Point3f camPos(
            r * std::cos(pitch) * std::sin(yaw),
            r * std::sin(pitch),
            -r * std::cos(pitch) * std::cos(yaw)
        );
        cv::Point3f camFocal(0, 0, 0.7f);
        cv::Point3f camUp(0, -1, 0); // 你定义 +Y 向下时，用 (0,-1,0) 会更顺手

        const cv::Vec3d camPosD(camPos.x, camPos.y, camPos.z);
        const cv::Vec3d camFocalD(camFocal.x, camFocal.y, camFocal.z);
        const cv::Vec3d camUpD(camUp.x, camUp.y, camUp.z);

        win.setViewerPose(cv::viz::makeCameraPose(camPosD, camFocalD, camUpD));
    }

    void update_cloud(const 结构体_原始场景帧& 帧) {
        if (!p.显示点云) {
            RemoveWidgetIfExists(win, "cloud");
            return;
        }

        const int w = 帧.宽度;
        const int h = 帧.高度;
        if (w <= 0 || h <= 0) return;

        const bool 点云可用 = ((int)帧.点云.size() == w * h);
        if (!点云可用) return;

        const int step = std::max(1, p.点云采样步长);

        std::vector<cv::Point3f> pts;
        pts.reserve((w / step) * (h / step));

        for (int v = 0; v < h; v += step) {
            for (int u = 0; u < w; u += step) {
                const int idx = v * w + u;
                const auto& P = 帧.点云[idx];
                if (p.点云忽略无效点 && P.z <= 0.0f) continue;
                pts.push_back(toP(P));
            }
        }

        if (pts.empty()) return;

        cv::Mat cloudMat((int)pts.size(), 1, CV_32FC3, pts.data());
        cv::viz::WCloud cloud(cloudMat, cv::viz::Color::white());
        win.showWidget("cloud", cloud);
    }

    void update_objects(const std::vector<存在观测>& obsList) {
        // 先移除上帧多余的对象
        for (int i = (int)obsList.size(); i < 上帧对象数; ++i) {
            RemoveWidgetIfExists(win, "obj_box_" + std::to_string(i));
            RemoveWidgetIfExists(win, "obj_cnt_" + std::to_string(i));
            RemoveWidgetIfExists(win, "obj_txt_" + std::to_string(i));
        }
        上帧对象数 = (int)obsList.size();

        for (int i = 0; i < (int)obsList.size(); ++i) {
            const auto& o = obsList[i];
            if (!观测有效_用于显示(o)) continue;

            const std::string nameBox = "obj_box_" + std::to_string(i);
            const std::string nameCnt = "obj_cnt_" + std::to_string(i);
            const std::string nameTxt = "obj_txt_" + std::to_string(i);

            const float cx = (float)o.中心.x, cy = (float)o.中心.y, cz = (float)o.中心.z;
            const float hx = (float)o.尺寸.x * 0.5f, hy = (float)o.尺寸.y * 0.5f, hz = (float)o.尺寸.z * 0.5f;

            const cv::Point3f pmin(cx - hx, cy - hy, cz - hz);
            const cv::Point3f pmax(cx + hx, cy + hy, cz + hz);

            // 1) 占位盒（线框）
            if (p.显示存在占位盒) {
                cv::viz::WCube cube(pmin, pmax, true /*wireframe*/, toVizColor(o.平均颜色));
                cube.setRenderingProperty(cv::viz::LINE_WIDTH, 2.0);
                win.showWidget(nameBox, cube);
            }
            else {
                RemoveWidgetIfExists(win, nameBox);
            }

            // 2) 轮廓（用点云代替轮廓线，最稳，避免 polyline 数据不闭合）
            if (p.显示存在轮廓点云 && !o.轮廓3D.empty()) {
                std::vector<cv::Point3f> cpts;
                cpts.reserve(o.轮廓3D.size());
                for (auto& pp : o.轮廓3D) {
                    if (!有限((float)pp.x) || !有限((float)pp.y) || !有限((float)pp.z)) continue;
                    if (pp.z <= 0.0f) continue;
                    cpts.push_back(toP(pp));
                }
                if (!cpts.empty()) {
                    cv::Mat cm((int)cpts.size(), 1, CV_32FC3, cpts.data());
                    cv::viz::WCloud wc(cm, toVizColor(o.平均颜色));
                    wc.setRenderingProperty(cv::viz::POINT_SIZE, p.轮廓点大小);
                    win.showWidget(nameCnt, wc);
                }
                else {
                    RemoveWidgetIfExists(win, nameCnt);
                }
            }
            else {
                RemoveWidgetIfExists(win, nameCnt);
            }

            // 3) 标签（可选）
            if (p.显示存在标签) {
                std::string txt = "E" + std::to_string(i);
                cv::Point3f pos(cx, cy, cz);
                cv::viz::WText3D t(txt, pos, 0.03, true, cv::viz::Color::yellow());
                win.showWidget(nameTxt, t);
            }
            else {
                RemoveWidgetIfExists(win, nameTxt);
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

    bool tick(const 结构体_原始场景帧& 帧, const std::vector<存在观测>& obsList) {
        if (!inited) init();

        // 1) 点云
        update_cloud(帧);

        // 2) 观测对象
        update_objects(obsList);

        // 3) 刷新窗口
        win.spinOnce(1, true);
        return !win.wasStopped();
    }
};
inline 场景实时显示器::场景实时显示器(场景显示参数 p)
    : impl_(std::make_unique<Impl>(p)) {
}
inline bool 场景实时显示器::初始化() { return impl_ ? impl_->init() : false; }
inline bool 场景实时显示器::更新(const 结构体_原始场景帧& 帧, const std::vector<存在观测>& 观测列表) {
    return impl_ ? impl_->tick(帧, 观测列表) : false;
}
inline bool 场景实时显示器::仍在运行() const {
    return impl_ ? !impl_->win.wasStopped() : false;
}

