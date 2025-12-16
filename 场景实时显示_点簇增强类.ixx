export module 场景实时显示_点簇增强模块;

import 基础数据类型模块;   // 需有 Vector3D / Color 等（按你项目实际替换）
import 点簇分割模块;       // 需有 点簇增强结果 定义（按你项目实际替换）

import <vector>;
import <string>;
import <unordered_map>;
import <cmath>;
import <algorithm>;

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/viz.hpp>

export struct 点簇显示参数
{
    bool 启用3D = true;
    bool 启用2D面板 = true;

    // 3D
    float 文字缩放 = 0.5f;
    double 坐标轴长度 = 0.25;     // viz坐标轴
    bool 显示坐标轴 = true;

    // 2D面板
    int 面板列数 = 4;            // 网格列数
    int 单元边长 = 220;          // 每个ROI显示单元缩放到正方形边长
    int 单元边距 = 8;
    bool 叠加掩膜 = true;
    double 掩膜透明度 = 0.35;

    // 文本
    bool 显示中心尺寸文本 = true;
};

export class 场景实时显示_点簇增强类
{
public:
    场景实时显示_点簇增强类() = default;

    void 初始化(const 点簇显示参数& p = {})
    {
        参数 = p;

        if (参数.启用3D) {
            if (!vizWin) {
                vizWin = std::make_unique<cv::viz::Viz3d>("3D-点簇增强实时显示");
                vizWin->setBackgroundColor(); // 默认背景
            }
            if (参数.显示坐标轴) {
                cv::viz::WCoordinateSystem axes(参数.坐标轴长度);
                vizWin->showWidget("axes", axes);
            }
        }

        if (参数.启用2D面板) {
            cv::namedWindow("2D-点簇增强面板", cv::WINDOW_NORMAL);
        }
    }

    void 关闭()
    {
        if (vizWin) {
            vizWin->close();
            vizWin.reset();
        }
        if (参数.启用2D面板) {
            cv::destroyWindow("2D-点簇增强面板");
        }
        盒子widgetId.clear();
        文本widgetId.clear();
    }

    bool 是否仍在运行() const
    {
        if (vizWin && vizWin->wasStopped()) return false;
        return true;
    }

    // 每帧调用：直接喂进增强结果列表
    void 显示一帧(const std::vector<点簇增强结果>& 增强列表)
    {
        if (参数.启用3D) {
            更新3D(增强列表);
        }
        if (参数.启用2D面板) {
            更新2D面板(增强列表);
        }

        // viz渲染 + highgui刷新
        if (vizWin) vizWin->spinOnce(1, true);
        if (参数.启用2D面板) cv::waitKey(1);
    }

private:
    点簇显示参数 参数;

    // 3D窗口
    std::unique_ptr<cv::viz::Viz3d> vizWin;

    // 用于稳定更新/复用 widget（避免每帧创建导致卡顿）
    std::unordered_map<int, std::string> 盒子widgetId;
    std::unordered_map<int, std::string> 文本widgetId;

private:
    static cv::Vec3d toVec3d(const Vector3D& p) { return { p.x, p.y, p.z }; }

    static cv::viz::Color toVizColor(const Color& c)
    {
        // 你项目 Color 是 b,g,r（你在早期示例里是这样定义的）
        return cv::viz::Color(c.b, c.g, c.r);
    }

    static cv::Mat 裁剪颜色转Mat_BGR(const std::vector<Color>& 裁剪颜色, int w, int h)
    {
        cv::Mat img(h, w, CV_8UC3);
        if ((int)裁剪颜色.size() != w * h) return img;

        for (int y = 0; y < h; ++y) {
            auto* row = img.ptr<cv::Vec3b>(y);
            for (int x = 0; x < w; ++x) {
                const auto& c = 裁剪颜色[y * w + x];
                row[x] = cv::Vec3b(c.b, c.g, c.r);
            }
        }
        return img;
    }

    static cv::Mat 掩膜转Mat_灰度(const std::vector<std::uint8_t>& mask, int w, int h)
    {
        cv::Mat m(h, w, CV_8UC1, cv::Scalar(0));
        if ((int)mask.size() != w * h) return m;
        for (int y = 0; y < h; ++y) {
            auto* row = m.ptr<std::uint8_t>(y);
            for (int x = 0; x < w; ++x) {
                row[x] = mask[y * w + x] ? 255 : 0;
            }
        }
        return m;
    }

    void 更新3D(const std::vector<点簇增强结果>& 增强列表)
    {
        if (!vizWin) return;

        // 先标记这帧出现过的ID（这里用 index 作为稳定ID；如果你有“簇ID/跟踪ID”，可替换）
        std::vector<int> thisFrameIds;
        thisFrameIds.reserve(增强列表.size());
        for (int i = 0; i < (int)增强列表.size(); ++i) thisFrameIds.push_back(i);

        // 画盒子与文本
        for (int i = 0; i < (int)增强列表.size(); ++i) {
            const auto& e = 增强列表[i];

            // widget 名称稳定：box_i / text_i
            auto& boxName = 盒子widgetId[i];
            if (boxName.empty()) boxName = "box_" + std::to_string(i);

            auto& textName = 文本widgetId[i];
            if (textName.empty()) textName = "text_" + std::to_string(i);

            // 以“中心+尺寸”构建 AABB
            const double hx = std::max(0.001, e.尺寸.x * 0.5);
            const double hy = std::max(0.001, e.尺寸.y * 0.5);
            const double hz = std::max(0.001, e.尺寸.z * 0.5);

            const cv::Vec3d c = toVec3d(e.中心);
            const cv::Point3d pmin(c[0] - hx, c[1] - hy, c[2] - hz);
            const cv::Point3d pmax(c[0] + hx, c[1] + hy, c[2] + hz);

            // 颜色：尽量用“平均色/或ROI平均色”。如果你增强结果里没有平均色字段，就固定色。
            cv::viz::Color color = cv::viz::Color::green();
            if constexpr (requires { e.平均颜色; }) {
                color = toVizColor(e.平均颜色);
            }

            cv::viz::WCube cube(pmin, pmax, true, color);
            cube.setRenderingProperty(cv::viz::LINE_WIDTH, 2.0);
            vizWin->showWidget(boxName, cube);

            if (参数.显示中心尺寸文本) {
                // 文本放在盒子右上方一点
                cv::Point3d txtPos(pmax.x, pmax.y, pmax.z);
                std::string txt = "ID=" + std::to_string(i);

                txt += "  C=(" + fmt3(e.中心.x) + "," + fmt3(e.中心.y) + "," + fmt3(e.中心.z) + ")";
                txt += "  S=(" + fmt3(e.尺寸.x) + "," + fmt3(e.尺寸.y) + "," + fmt3(e.尺寸.z) + ")";

                cv::viz::WText3D wtxt(txt, txtPos, 参数.文字缩放, color);
                vizWin->showWidget(textName, wtxt);
            }
        }

        // 清理“上一帧存在、这一帧不存在”的widget
        清理缺失widget(thisFrameIds);
    }

    void 清理缺失widget(const std::vector<int>& thisFrameIds)
    {
        // thisFrameIds 是连续0..N-1时其实不需要清理；
        // 但为了以后你换成“跟踪ID”也能用，这里写成通用版。

        auto isAlive = [&](int id) -> bool {
            return std::find(thisFrameIds.begin(), thisFrameIds.end(), id) != thisFrameIds.end();
            };

        std::vector<int> toErase;
        toErase.reserve(盒子widgetId.size());

        for (auto& kv : 盒子widgetId) {
            const int id = kv.first;
            if (!isAlive(id)) toErase.push_back(id);
        }
        for (int id : toErase) {
            if (vizWin) {
                if (!盒子widgetId[id].empty()) vizWin->removeWidget(盒子widgetId[id]);
                if (!文本widgetId[id].empty()) vizWin->removeWidget(文本widgetId[id]);
            }
            盒子widgetId.erase(id);
            文本widgetId.erase(id);
        }
    }

    void 更新2D面板(const std::vector<点簇增强结果>& 增强列表)
    {
        const int n = (int)增强列表.size();
        if (n <= 0) {
            cv::Mat blank(240, 320, CV_8UC3, cv::Scalar(20, 20, 20));
            cv::putText(blank, "no clusters", { 20,120 }, cv::FONT_HERSHEY_SIMPLEX, 0.8, { 200,200,200 }, 2);
            cv::imshow("2D-点簇增强面板", blank);
            return;
        }

        const int cols = std::max(1, 参数.面板列数);
        const int rows = (n + cols - 1) / cols;

        const int cell = std::max(64, 参数.单元边长);
        const int pad = std::max(0, 参数.单元边距);

        const int W = cols * cell + (cols + 1) * pad;
        const int H = rows * cell + (rows + 1) * pad;

        cv::Mat panel(H, W, CV_8UC3, cv::Scalar(15, 15, 15));

        for (int i = 0; i < n; ++i) {
            const auto& e = 增强列表[i];
            const int r = i / cols;
            const int c = i % cols;

            const int x0 = pad + c * (cell + pad);
            const int y0 = pad + r * (cell + pad);
            cv::Rect dstRect(x0, y0, cell, cell);

            cv::Mat cellImg(cell, cell, CV_8UC3, cv::Scalar(30, 30, 30));

            // 取ROI裁剪图/掩膜
            cv::Mat roiBgr;
            cv::Mat roiMask;
            if (e.裁剪宽 > 0 && e.裁剪高 > 0) {
                if (!e.裁剪颜色.empty()) roiBgr = 裁剪颜色转Mat_BGR(e.裁剪颜色, e.裁剪宽, e.裁剪高);
                if (!e.裁剪掩码.empty()) roiMask = 掩膜转Mat_灰度(e.裁剪掩码, e.裁剪宽, e.裁剪高);
            }

            if (!roiBgr.empty()) {
                // 缩放到单元格
                cv::Mat resized;
                cv::resize(roiBgr, resized, { cell, cell }, 0, 0, cv::INTER_NEAREST);

                if (参数.叠加掩膜 && !roiMask.empty()) {
                    cv::Mat m2;
                    cv::resize(roiMask, m2, { cell, cell }, 0, 0, cv::INTER_NEAREST);

                    // 用掩膜把背景压暗，前景保留
                    cv::Mat fg = resized.clone();
                    cv::Mat bg;
                    resized.convertTo(bg, -1, 0.35, 0); // 暗背景

                    // 生成3通道mask
                    cv::Mat m3;
                    cv::cvtColor(m2, m3, cv::COLOR_GRAY2BGR);

                    // alpha融合：mask区域用原图，非mask区域用暗背景
                    for (int y = 0; y < cell; ++y) {
                        auto* pf = fg.ptr<cv::Vec3b>(y);
                        auto* pb = bg.ptr<cv::Vec3b>(y);
                        auto* pm = m2.ptr<std::uint8_t>(y);
                        for (int x = 0; x < cell; ++x) {
                            if (pm[x] < 128) pf[x] = pb[x];
                        }
                    }
                    cellImg = fg;
                }
                else {
                    cellImg = resized;
                }
            }
            else {
                cv::putText(cellImg, "no ROI", { 20, cell / 2 }, cv::FONT_HERSHEY_SIMPLEX, 0.7, { 200,200,200 }, 2);
            }

            // 标注：ID + ROI + 尺寸
            {
                std::string line1 = "ID=" + std::to_string(i);
                std::string line2 = "ROI(" + std::to_string(e.边界.umin) + "," + std::to_string(e.边界.vmin) + ") "
                    + std::to_string(e.裁剪宽) + "x" + std::to_string(e.裁剪高);
                std::string line3 = "S(" + fmt2(e.尺寸.x) + "," + fmt2(e.尺寸.y) + "," + fmt2(e.尺寸.z) + ")";

                cv::rectangle(cellImg, { 0,0,cell,cell }, { 220,220,220 }, 1);
                cv::putText(cellImg, line1, { 8,18 }, cv::FONT_HERSHEY_SIMPLEX, 0.5, { 0,0,0 }, 3);
                cv::putText(cellImg, line1, { 8,18 }, cv::FONT_HERSHEY_SIMPLEX, 0.5, { 255,255,255 }, 1);

                cv::putText(cellImg, line2, { 8, cell - 26 }, cv::FONT_HERSHEY_SIMPLEX, 0.45, { 0,0,0 }, 3);
                cv::putText(cellImg, line2, { 8, cell - 26 }, cv::FONT_HERSHEY_SIMPLEX, 0.45, { 255,255,255 }, 1);

                cv::putText(cellImg, line3, { 8, cell - 8 }, cv::FONT_HERSHEY_SIMPLEX, 0.45, { 0,0,0 }, 3);
                cv::putText(cellImg, line3, { 8, cell - 8 }, cv::FONT_HERSHEY_SIMPLEX, 0.45, { 255,255,255 }, 1);
            }

            cellImg.copyTo(panel(dstRect));
        }

        cv::imshow("2D-点簇增强面板", panel);
    }

    static std::string fmt3(double v)
    {
        char buf[64];
        std::snprintf(buf, sizeof(buf), "%.3f", v);
        return buf;
    }
    static std::string fmt2(double v)
    {
        char buf[64];
        std::snprintf(buf, sizeof(buf), "%.2f", v);
        return buf;
    }
};
