// 子轮廓提取类.ixx
// 目的：从父轮廓二值掩膜中提取“子轮廓”（层级轮廓），并可选计算平均颜色。
// 约束：不对图像做任何“美化/重采样/平滑”。仅做轮廓提取与统计。
// 依赖：OpenCV



module;

#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

#include <cstdint>
#include <vector>
#include <algorithm>

export module 子轮廓提取模块;

export struct 结构体_平均颜色_BGR {
    std::uint8_t b = 0, g = 0, r = 0;
};

export struct 子轮廓条目 {
    // 轮廓点（OpenCV坐标：x=列, y=行）
    std::vector<cv::Point> 轮廓;

    // 在输入mask上的外接矩形
    cv::Rect 边界{};

    // 填充后的二值掩膜（与输入mask同尺寸，0/255）
    cv::Mat 掩膜;

    // 面积（像素）
    double 面积 = 0.0;

    // 平均颜色（可选，若未提供彩色图则为0）
    结构体_平均颜色_BGR 平均颜色{};

    // 层级信息（findContours 的索引）
    int 自索引 = -1;
    int 父索引 = -1;
};

export struct 子轮廓提取结果 {
    // 外轮廓（一级）：hierarchy[i].parent == -1
    std::vector<子轮廓条目> 外轮廓;

    // 子轮廓（包括孔洞/内部轮廓）：hierarchy[i].parent >= 0
    std::vector<子轮廓条目> 子轮廓;

    // 可选：父平均颜色（排除其子轮廓区域后的平均色；若无彩色图则为0）
    结构体_平均颜色_BGR 父平均颜色_排除子{};
};

export class 子轮廓提取类 {
public:
    // mask：CV_8UC1，0/255（建议“填充区域”的前景mask；如果是边界线也能工作但层级意义会弱）
    // bgr ：CV_8UC3，可选；用于计算平均颜色（按mask区域求均值）
    // 最小面积像素：过滤极小轮廓（噪点）
    static 子轮廓提取结果 提取(
        const cv::Mat& mask,
        const cv::Mat* bgr = nullptr,
        int 最小面积像素 = 64)
    {
        子轮廓提取结果 out{};
        if (mask.empty()) return out;
        if (mask.type() != CV_8UC1) return out;
        if (bgr && (!bgr->empty()) && bgr->type() != CV_8UC3) return out;
        if (bgr && (!bgr->empty()) && (bgr->rows != mask.rows || bgr->cols != mask.cols)) return out;

        // findContours 会修改输入，所以copy
        cv::Mat m;
        mask.copyTo(m);

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;

        // 选择 RETR_TREE：拿到完整层级
        cv::findContours(m, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

        if (contours.empty() || hierarchy.size() != contours.size()) return out;

        // 提取条目
        out.外轮廓.reserve(contours.size());
        out.子轮廓.reserve(contours.size());

        for (int i = 0; i < (int)contours.size(); ++i) {
            const auto& c = contours[i];
            if (c.size() < 3) continue;

            double area = std::fabs(cv::contourArea(c));
            if (area < (double)最小面积像素) continue;

            子轮廓条目 e{};
            e.轮廓 = c;
            e.面积 = area;
            e.边界 = cv::boundingRect(c);
            e.自索引 = i;
            e.父索引 = hierarchy[i][3];

            // 生成与输入同尺寸的掩膜（0/255）
            e.掩膜 = cv::Mat::zeros(mask.size(), CV_8UC1);
            cv::drawContours(e.掩膜, contours, i, cv::Scalar(255), cv::FILLED);

            // 平均颜色（可选）
            if (bgr && !bgr->empty()) {
                e.平均颜色 = 计算平均颜色_按掩膜(*bgr, e.掩膜);
            }

            if (e.父索引 < 0) out.外轮廓.push_back(std::move(e));
            else out.子轮廓.push_back(std::move(e));
        }

        // 计算父平均颜色（排除子轮廓），只针对“最大外轮廓”做一个代表值（更符合“父存在”语义）
        if (bgr && !bgr->empty() && !out.外轮廓.empty()) {
            // 选择面积最大的外轮廓作为父
            int best = 0;
            for (int i = 1; i < (int)out.外轮廓.size(); ++i) {
                if (out.外轮廓[i].面积 > out.外轮廓[best].面积) best = i;
            }

            cv::Mat parentMask = out.外轮廓[best].掩膜.clone();

            // 从 parentMask 中扣除属于该外轮廓子层级的区域
            // 注意：out.子轮廓 里包含所有内部轮廓，我们只扣除 parent 的直接/间接子孙
            // 这里用 hierarchy 父链判断（需要原hierarchy/contours索引），我们用自索引/父索引重建祖先链。
            const int parentIdx = out.外轮廓[best].自索引;

            // 构建：i -> parent index 的映射（仅用于 filtered entries 也能工作，但更稳用原hierarchy）
            // 这里直接用 hierarchy 原表判断祖先
            for (int i = 0; i < (int)contours.size(); ++i) {
                if (hierarchy[i][3] < 0) continue; // 外轮廓
                if (std::fabs(cv::contourArea(contours[i])) < (double)最小面积像素) continue;

                if (是祖先(parentIdx, i, hierarchy)) {
                    cv::drawContours(parentMask, contours, i, cv::Scalar(0), cv::FILLED); // 扣掉子区域
                }
            }

            out.父平均颜色_排除子 = 计算平均颜色_按掩膜(*bgr, parentMask);
        }

        return out;
    }

private:
    static bool 是祖先(int ancestor, int node, const std::vector<cv::Vec4i>& hierarchy) {
        int p = hierarchy[node][3];
        while (p >= 0) {
            if (p == ancestor) return true;
            p = hierarchy[p][3];
        }
        return false;
    }

    static 结构体_平均颜色_BGR 计算平均颜色_按掩膜(const cv::Mat& bgr, const cv::Mat& mask) {
        结构体_平均颜色_BGR out{};
        // mean: BGR order, returns Scalar(b,g,r,?)
        cv::Scalar m = cv::mean(bgr, mask);
        auto clampU8 = [](double x) -> std::uint8_t {
            if (x < 0.0) return 0;
            if (x > 255.0) return 255;
            return (std::uint8_t)(x + 0.5);
            };
        out.b = clampU8(m[0]);
        out.g = clampU8(m[1]);
        out.r = clampU8(m[2]);
        return out;
    }
};
