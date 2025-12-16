module;

#include <opencv2/opencv.hpp>
#include <opencv2/viz.hpp>
#include <opencv2/core.hpp>
// 场景实时显示模块.ixx（完全更新版，适配新特征系统）
export module 场景实时显示模块;



import 相机接口模块;
import 基础数据类型模块;
import 世界树环境模块;
import 语素模块;
import 三维场景管理模块;   // 用于单位转换等
import 主信息定义模块;
import 数据仓库模块;
import 语素环境模块;
import 特征模块;              // 关键：特征类
import 场景模块;

import <vector>;
import <string>;
import <memory>;
import <cmath>;
import <algorithm>;
import <cstdint>;
import <limits>;
import <chrono>;
import <map>;
import <unordered_set>;

export struct 场景显示参数 {
    bool 显示坐标轴 = true;
    bool 显示相机方向箭头 = true;

    bool 显示实时点云 = true;
    int  点云采样步长 = 3;
    bool 点云忽略无效点 = true;

    bool 显示存在占位盒 = true;
    bool 显示存在标签 = true;

    double 记忆淡化时间秒 = 10.0f;

    double 初始相机距离 = 1.2f;
    double 初始相机俯仰 = -15.0f;
    double 初始相机偏航 = 25.0f;
};

struct 显示用存在信息 {
    存在节点类* 节点 = nullptr;
    Vector3D 中心{ 0,0,0 };
    Vector3D 尺寸{ 0.1f,0.1f,0.1f };
    Color    颜色{ 200,200,255 };
    std::string 标签;
    double   最近观测时间秒 = 0.0;
    std::uint32_t 观测次数 = 0;
    bool     有效 = false;
};

export class 场景实时显示器 {
public:
    explicit 场景实时显示器(场景显示参数 p = {});
    ~场景实时显示器() = default;

    bool 初始化();
    bool 更新(const 结构体_原始场景帧* 实时帧 = nullptr, 场景节点类* 当前场景根 = nullptr);
    bool 仍在运行() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

namespace {
    inline double 当前时间秒() {
        static auto start = std::chrono::high_resolution_clock::now();
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(now - start).count();
    }
}

struct 场景实时显示器::Impl {
    场景显示参数 p;
    cv::viz::Viz3d win{ "HY-Ego World Memory (D455)" };
    bool inited = false;

    struct 轨迹记录 {
        std::vector<cv::Point3d> 点;
        double       最近命中时间秒 = 0.0;
        std::uint32_t 最近命中次数 = 0;
    };

    // key = 存在节点指针地址（uintptr_t）
    std::map<uintptr_t, 轨迹记录> 存在缓存;
    std::unordered_set<uintptr_t> 上帧显示Key;
    static constexpr int 最大轨迹点数 = 30;

    // 预定义特征类型（请在初始化时赋值实际词性节点）
    词性节点类* 类型_位置向量 = nullptr;
    词性节点类* 类型_尺寸向量 = nullptr;
    词性节点类* 类型_位置X = nullptr;
    词性节点类* 类型_位置Y = nullptr;
    词性节点类* 类型_位置Z = nullptr;
    词性节点类* 类型_尺寸左右 = nullptr;
    词性节点类* 类型_尺寸上下 = nullptr;
    词性节点类* 类型_尺寸前后 = nullptr;
    词性节点类* 类型_观测次数 = nullptr;

    explicit Impl(场景显示参数 pp) : p(pp) {}

    static cv::Point3d toP(const Vector3D& v) {
        return { v.x, v.y, v.z };
    }

    static cv::viz::Color toVizColor(const Color& c, double alpha = 1.0) {
        return cv::viz::Color(
            static_cast<uint8_t>(c.r * alpha),
            static_cast<uint8_t>(c.g * alpha),
            static_cast<uint8_t>(c.b * alpha)
        );
    }

    static void RemoveWidgetIfExists(cv::viz::Viz3d& w, const std::string& id) noexcept {
        try { w.removeWidget(id); }
        catch (...) {}
    }

    bool init() {
        if (inited) return true;

        win.setWindowSize(cv::Size(1280, 720));
        win.setBackgroundColor(cv::viz::Color::black());

        // 坐标轴
        if (p.显示坐标轴) {
            win.showWidget("coord", cv::viz::WCoordinateSystem(0.2));
        }

        // 初始相机视角（不依赖 cv::viz::Camera 构造，避免不同 OpenCV 版本差异）
        auto pose = cv::viz::makeCameraPose(
            cv::Vec3d(0, 0, p.初始相机距离),
            cv::Vec3d(0, 0, 0),
            cv::Vec3d(0, 1, 0)
        );
        win.setViewerPose(pose);

        inited = true;
        return true;
    }

    // 从存在节点提取显示信息（适配新特征系统）
    显示用存在信息 提取存在信息(存在节点类* 存在, double now) {
        显示用存在信息 info;

        if (!存在 || !存在->主信息) {
            info.有效 = false;
            return info;
        }

        auto* emi = dynamic_cast<存在节点主信息类*>(存在->主信息);
        if (!emi) {
            info.有效 = false;
            return info;
        }

        info.节点 = 存在;

        // 读取位置与尺寸（向量优先 → 标量备选）
        Vector3D 中心{}, 尺寸{};
        bool got = false;

        // 向量型
        if (类型_位置向量) {
            auto* f = 特征类::查找特征(存在, 类型_位置向量);
            std::vector<std::int64_t> vec;
            if (f && 特征类::读取矢量特征值(f, vec) && vec.size() >= 3) {
                中心 = Vector3D{ 三维场景管理类::毫米到米(vec[0]),
                                三维场景管理类::毫米到米(vec[1]),
                                三维场景管理类::毫米到米(vec[2]) };
                got = true;
            }
        }

        if (!got && 类型_位置X && 类型_位置Y && 类型_位置Z) {
            std::int64_t x{}, y{}, z{};
            if (特征类::读取标量特征值(特征类::查找特征(存在, 类型_位置X), x) &&
                特征类::读取标量特征值(特征类::查找特征(存在, 类型_位置Y), y) &&
                特征类::读取标量特征值(特征类::查找特征(存在, 类型_位置Z), z)) {
                中心 = Vector3D{ 三维场景管理类::毫米到米(x),
                                三维场景管理类::毫米到米(y),
                                三维场景管理类::毫米到米(z) };
                got = true;
            }
        }

        if (类型_尺寸向量) {
            auto* f = 特征类::查找特征(存在, 类型_尺寸向量);
            std::vector<std::int64_t> vec;
            if (f && 特征类::读取矢量特征值(f, vec) && vec.size() >= 3) {
                尺寸 = Vector3D{ 三维场景管理类::毫米到米(vec[0]),
                                三维场景管理类::毫米到米(vec[1]),
                                三维场景管理类::毫米到米(vec[2]) };
            }
            else {
                尺寸 = Vector3D{ 0.1, 0.1, 0.1 };
            }
        }
        else if (类型_尺寸左右 && 类型_尺寸上下 && 类型_尺寸前后) {
            std::int64_t sx{}, sy{}, sz{};
            if (特征类::读取标量特征值(特征类::查找特征(存在, 类型_尺寸左右), sx) &&
                特征类::读取标量特征值(特征类::查找特征(存在, 类型_尺寸上下), sy) &&
                特征类::读取标量特征值(特征类::查找特征(存在, 类型_尺寸前后), sz)) {
                尺寸 = Vector3D{ 三维场景管理类::毫米到米(sx),
                                三维场景管理类::毫米到米(sy),
                                三维场景管理类::毫米到米(sz) };
            }
            else {
                尺寸 = Vector3D{ 0.1, 0.1, 0.1 };
            }
        }

        info.中心 = 中心;
        info.尺寸 = 尺寸;
        info.最近观测时间秒 = now;  // 实时显示时都算最新
        info.有效 = got;

        // 观测次数
        if (类型_观测次数) {
            auto* f = 特征类::查找特征(存在, 类型_观测次数);
            std::int64_t cnt = 0;
            if (f && 特征类::读取标量特征值(f, cnt)) {
                info.观测次数 = static_cast<std::uint32_t>(cnt);
            }
        }

        // 标签（主键 + 观测次数）
        info.标签 = "ID:" + 存在->主键.substr(0, 8) +
            " Hits:" + std::to_string(info.观测次数);

        // 颜色随观测次数渐变（新物体亮，老物体淡）
        double ratio = std::min(1.0, info.观测次数 / 10.0);
        info.颜色 = Color{
            static_cast<uint8_t>(200 + 55 * ratio),
            static_cast<uint8_t>(200 + 55 * ratio),
            static_cast<uint8_t>(255)
        };

        return info;
    }

    bool tick(const 结构体_原始场景帧* 实时帧, 场景节点类* 当前场景根) {
        if (!inited) return false;

        const double now = 当前时间秒();

        auto makeId = [](const char* prefix, uintptr_t key) {
            return std::string(prefix) + std::to_string(static_cast<std::uint64_t>(key));
            };
        auto dist = [](const cv::Point3d& a, const cv::Point3d& b) {
            const double dx = a.x - b.x;
            const double dy = a.y - b.y;
            const double dz = a.z - b.z;
            return std::sqrt(dx * dx + dy * dy + dz * dz);
            };

        // 1) 实时点云（尽量与 OpenCV 解耦：只在显示层临时转换）
        if (p.显示实时点云 && 实时帧) {
            RemoveWidgetIfExists(win, "cloud");

            const int w = 实时帧->宽度;
            const int h = 实时帧->高度;
            const std::size_t expect = (w > 0 && h > 0)
                ? (static_cast<std::size_t>(w) * static_cast<std::size_t>(h))
                : 0u;

            const bool perPixel = (expect > 0 && 实时帧->点云.size() == expect);
            const bool hasColor = (实时帧->颜色.size() == 实时帧->点云.size());

            std::vector<cv::Vec3f> pts;
            std::vector<cv::Vec3b> cols;
            const int step = std::max(1, p.点云采样步长);

            if (perPixel) {
                pts.reserve((expect / static_cast<std::size_t>(step * step)) + 1);
                if (hasColor) cols.reserve(pts.capacity());
                for (int v = 0; v < h; v += step) {
                    for (int u = 0; u < w; u += step) {
                        const std::size_t idx = static_cast<std::size_t>(v) * static_cast<std::size_t>(w) + static_cast<std::size_t>(u);
                        const auto& P = 实时帧->点云[idx];
                        if (p.点云忽略无效点 && (!std::isfinite(P.z) || P.z <= 0.0)) continue;
                        pts.emplace_back(static_cast<float>(P.x), static_cast<float>(P.y), static_cast<float>(P.z));
                        if (hasColor) {
                            const auto& c = 实时帧->颜色[idx];
                            cols.emplace_back(c.b, c.g, c.r); // OpenCV 习惯 BGR
                        }
                    }
                }
            }
            else if (!实时帧->点云.empty()) {
                // 非“每像素点云”，视为稀疏点列表
                const std::size_t n = 实时帧->点云.size();
                pts.reserve((n / static_cast<std::size_t>(step)) + 1);
                if (hasColor) cols.reserve(pts.capacity());
                for (std::size_t i = 0; i < n; i += static_cast<std::size_t>(step)) {
                    const auto& P = 实时帧->点云[i];
                    if (p.点云忽略无效点 && (!std::isfinite(P.z) || P.z <= 0.0)) continue;
                    pts.emplace_back(static_cast<float>(P.x), static_cast<float>(P.y), static_cast<float>(P.z));
                    if (hasColor) {
                        const auto& c = 实时帧->颜色[i];
                        cols.emplace_back(c.b, c.g, c.r);
                    }
                }
            }

            if (!pts.empty()) {
                cv::Mat ptsMat((int)pts.size(), 1, CV_32FC3, pts.data());
                if (!cols.empty() && cols.size() == pts.size()) {
                    cv::Mat colMat((int)cols.size(), 1, CV_8UC3, cols.data());
                    cv::viz::WCloud wc(ptsMat, colMat);
                    wc.setRenderingProperty(cv::viz::POINT_SIZE, 2);
                    win.showWidget("cloud", wc);
                }
                else {
                    cv::viz::WCloud wc(ptsMat, cv::viz::Color::white());
                    wc.setRenderingProperty(cv::viz::POINT_SIZE, 2);
                    win.showWidget("cloud", wc);
                }
            }
        }

        // 2) 存在占位盒 + 标签 + 轨迹
        std::unordered_set<uintptr_t> 本帧显示Key;
        if (当前场景根 && p.显示存在占位盒) {
            auto* smi = 场景类::取场景主信息(当前场景根);
            if (smi) {
                for (auto* e : smi->存在索引) {
                    auto info = 提取存在信息(e, now);
                    if (!info.有效 || !info.节点) continue;

                    const uintptr_t key = reinterpret_cast<uintptr_t>(info.节点);
                    auto& cache = 存在缓存[key];
                    if (cache.最近命中时间秒 <= 0.0) {
                        cache.最近命中时间秒 = now;
                        cache.最近命中次数 = info.观测次数;
                    }

                    bool 新命中 = false;
                    if (info.观测次数 > cache.最近命中次数) {
                        cache.最近命中次数 = info.观测次数;
                        cache.最近命中时间秒 = now;
                        新命中 = true;
                    }

                    const double age = now - cache.最近命中时间秒;
                    if (age > p.记忆淡化时间秒 * 2.0) {
                        // 太久没命中，视为“记忆淡出”
                        RemoveWidgetIfExists(win, makeId("box_", key));
                        RemoveWidgetIfExists(win, makeId("text_", key));
                        RemoveWidgetIfExists(win, makeId("traj_", key));
                        continue;
                    }

                    本帧显示Key.insert(key);

                    const double alpha = std::clamp(1.0 - age / std::max(0.001, p.记忆淡化时间秒), 0.08, 1.0);

                    // 盒子
                    cv::viz::WCube cube(
                        toP(info.中心 - info.尺寸 * 0.5),
                        toP(info.中心 + info.尺寸 * 0.5),
                        true,
                        toVizColor(info.颜色, alpha)
                    );
                    cube.setRenderingProperty(cv::viz::LINE_WIDTH, 2);
                    win.showWidget(makeId("box_", key), cube);

                    // 标签
                    if (p.显示存在标签) {
                        double text_scale = info.尺寸.y * 0.08;
                        if (text_scale < 0.02) text_scale = 0.02;

                        cv::viz::WText3D text(
                            info.标签,
                            toP(info.中心 + Vector3D{ 0, info.尺寸.y * 0.6, 0 }),
                            text_scale,
                            /*face_camera=*/true,
                            cv::viz::Color::white()                        
                        );
                        win.showWidget(makeId("text_", key), text);
                    }

                    // 轨迹：只在“新命中”或位置明显变化时追加
                    const cv::Point3d c = toP(info.中心);
                    const bool moved = (!cache.点.empty()) ? (dist(cache.点.back(), c) > 0.002) : true;
                    if (新命中 || moved) {
                        cache.点.push_back(c);
                        if ((int)cache.点.size() > 最大轨迹点数) {
                            cache.点.erase(cache.点.begin());
                        }
                    }

                    if (cache.点.size() >= 2) {
                        cv::Mat points((int)cache.点.size(), 1, CV_64FC3, cache.点.data());
                        cv::viz::WPolyLine line(points, cv::viz::Color::yellow());
                        line.setRenderingProperty(cv::viz::LINE_WIDTH, 3);
                        win.showWidget(makeId("traj_", key), line);
                    }
                    else {
                        RemoveWidgetIfExists(win, makeId("traj_", key));
                    }
                }
            }
        }

        // 清理：本帧不再显示的 widget
        for (auto key : 上帧显示Key) {
            if (本帧显示Key.find(key) == 本帧显示Key.end()) {
                RemoveWidgetIfExists(win, makeId("box_", key));
                RemoveWidgetIfExists(win, makeId("text_", key));
                RemoveWidgetIfExists(win, makeId("traj_", key));
            }
        }
        上帧显示Key = std::move(本帧显示Key);

        // 清理：长期未命中的轨迹缓存
        for (auto it = 存在缓存.begin(); it != 存在缓存.end(); ) {
            if (now - it->second.最近命中时间秒 > p.记忆淡化时间秒 * 4.0) {
                it = 存在缓存.erase(it);
            }
            else {
                ++it;
            }
        }

        win.spinOnce(1, true);
        return true;
    }
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