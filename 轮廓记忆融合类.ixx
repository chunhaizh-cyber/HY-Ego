// 轮廓记忆融合类.ixx
// 多帧2D轮廓“记忆”融合：把每帧观测到的局部轮廓掩膜对齐到“规范轮廓坐标系”，再做统计累积。
// 目标：得到更完整/更稳定的2D轮廓记忆（并集/稳态），供显示与匹配使用。
// 约束：不做任何图像“美化/平滑/重采样”。只做几何对齐（仿射）+ 计数统计。
// 依赖：OpenCV
module;
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <algorithm>
#include <cmath>

export module 轮廓记忆融合模块;





export struct 轮廓记忆参数 {
    // 规范坐标系大小（像素）：建议 128/192/256。越大越细，但计算更重。
    int 规范边长 = 256;

    // 过滤极小轮廓（像素面积）
    int 最小面积像素 = 64;

    // 稳态阈值：k 次以上命中即为“稳态前景”
    std::uint32_t 稳态阈值_k = 3;

    // 稳态比例阈值：命中次数 >= seen * ratio
    // 若启用，则最终阈值 = max(k, ceil(seen*ratio))
    bool 启用稳态比例 = true;
    float 稳态比例 = 0.20f;

    // 仿射对齐：使用 PCA 主轴（旋转+缩放+居中）到规范方框
    // 若关闭，则仅按外接矩形缩放居中（不旋转）
    bool 启用PCA旋转对齐 = true;
};

export struct 轮廓记忆导出 {
    // 0/255 二值掩膜（规范边长 x 规范边长）
    cv::Mat 并集Mask;     // count>0
    cv::Mat 稳态Mask;     // count>=threshold(seen)

    // 编码（8x8 block packing）：
    // blocks 的布局：按行主序的 block 网格（nb = N/8；共 nb*nb 个 u64）
    std::vector<std::uint64_t> 并集编码;
    std::vector<std::uint64_t> 稳态编码;

    // 元信息（可选）：[N]
    std::vector<std::int64_t> 元信息;
};

export class 轮廓记忆状态 {
public:
    // 计数图：CV_16UC1（避免长期累加溢出）
    cv::Mat 计数图{};
    std::uint32_t 观测次数 = 0;

    // 最近一次更新的签名（便于上层做“版本驱动”）
    std::uint64_t 最近签名 = 0;

    // 初始化（仅在首次使用时）
    void 确保初始化(int N) {
        if (!计数图.empty() && 计数图.rows == N && 计数图.cols == N) return;
        计数图 = cv::Mat::zeros(N, N, CV_16UC1);
        观测次数 = 0;
        最近签名 = 0;
    }
};

export class 轮廓记忆融合类 {
public:
    using Key = const void*; // 推荐传入 const 存在节点类* 作为 key

    explicit 轮廓记忆融合类(轮廓记忆参数 p = {}) : p_(p) {}

    // 更新某个存在的记忆（输入必须是0/255的二值掩膜；建议是“填充轮廓区域”的mask）
    // obsMask：CV_8UC1，与 ROI 尺寸一致
    // 返回：是否发生有效更新
    bool 更新(Key k, const cv::Mat& obsMask) {
        if (!k) return false;
        if (obsMask.empty()) return false;
        if (obsMask.type() != CV_8UC1) return false;

        // 1) 提取轮廓与面积
        std::vector<std::vector<cv::Point>> contours;
        {
            cv::Mat tmp;
            obsMask.copyTo(tmp);
            cv::findContours(tmp, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        }
        if (contours.empty()) return false;

        int best = 0;
        double bestA = 0.0;
        for (int i = 0; i < (int)contours.size(); ++i) {
            double a = std::fabs(cv::contourArea(contours[i]));
            if (a > bestA) { bestA = a; best = i; }
        }
        if (bestA < (double)p_.最小面积像素) return false;

        const auto& c = contours[best];

        // 2) 计算对齐仿射：ROI坐标 -> 规范坐标
        cv::Mat M = 计算对齐仿射_(c, obsMask.size(), p_);

        // 3) 最近邻 warp 到规范mask（不做插值美化）
        cv::Mat warped = cv::Mat::zeros(p_.规范边长, p_.规范边长, CV_8UC1);
        cv::warpAffine(
            obsMask, warped, M, warped.size(),
            cv::INTER_NEAREST, cv::BORDER_CONSTANT, cv::Scalar(0));

        // 4) 更新计数图
        {
            std::scoped_lock lk(m_);
            auto& st = map_[k];
            st.确保初始化(p_.规范边长);

            st.观测次数 += 1;

            // 遍历 mask 前景像素，计数+1（CV_16U饱和）
            for (int y = 0; y < warped.rows; ++y) {
                const std::uint8_t* row = warped.ptr<std::uint8_t>(y);
                std::uint16_t* cnt = st.计数图.ptr<std::uint16_t>(y);
                for (int x = 0; x < warped.cols; ++x) {
                    if (row[x]) {
                        if (cnt[x] != 0xFFFF) cnt[x] += 1;
                    }
                }
            }

            // 计算一次签名（用于上层“版本驱动”）
            // 签名基于并集编码（足够稳定且快）
            auto uni = 导出_并集编码_已加锁_(st);
            st.最近签名 = FNV1a64_vec_u64_(uni);
        }

        return true;
    }

    // 导出：并集/稳态 mask + 编码
    bool 导出(Key k, 轮廓记忆导出& out) const {
        out = {};
        if (!k) return false;

        std::scoped_lock lk(m_);
        auto it = map_.find(k);
        if (it == map_.end()) return false;

        const auto& st = it->second;
        if (st.计数图.empty()) return false;

        const int N = st.计数图.rows;
        out.元信息 = { (std::int64_t)N };

        // 并集
        out.并集Mask = cv::Mat::zeros(N, N, CV_8UC1);
        // 稳态
        out.稳态Mask = cv::Mat::zeros(N, N, CV_8UC1);

        std::uint32_t thr = 计算稳态阈值_(st.观测次数, p_);

        for (int y = 0; y < N; ++y) {
            const std::uint16_t* cnt = st.计数图.ptr<std::uint16_t>(y);
            std::uint8_t* u = out.并集Mask.ptr<std::uint8_t>(y);
            std::uint8_t* s = out.稳态Mask.ptr<std::uint8_t>(y);
            for (int x = 0; x < N; ++x) {
                if (cnt[x] > 0) u[x] = 255;
                if (cnt[x] >= thr) s[x] = 255;
            }
        }

        out.并集编码 = 编码_8x8Blocks_(out.并集Mask);
        out.稳态编码 = 编码_8x8Blocks_(out.稳态Mask);

        return true;
    }

    // 获取最近签名（用于“只有变化才触发下游更新”）
    bool 获取最近签名(Key k, std::uint64_t& sig) const {
        sig = 0;
        if (!k) return false;
        std::scoped_lock lk(m_);
        auto it = map_.find(k);
        if (it == map_.end()) return false;
        sig = it->second.最近签名;
        return true;
    }

    // 清理（存在删除通知时调用，方案A）
    void 清理(Key k) {
        if (!k) return;
        std::scoped_lock lk(m_);
        map_.erase(k);
    }

private:
    轮廓记忆参数 p_;
    mutable std::mutex m_;
    std::unordered_map<Key, 轮廓记忆状态> map_;

private:
    // ---- 对齐仿射：ROI坐标 -> 规范坐标 ----
    static cv::Mat 计算对齐仿射_(const std::vector<cv::Point>& contour, cv::Size roiSize, const 轮廓记忆参数& p) {
        const int N = p.规范边长;

        // 默认：外接矩形缩放居中（不旋转）
        cv::Rect bbox = cv::boundingRect(contour);
        float cx = bbox.x + bbox.width * 0.5f;
        float cy = bbox.y + bbox.height * 0.5f;

        float sx = (bbox.width > 0) ? (float)N / (float)bbox.width : 1.0f;
        float sy = (bbox.height > 0) ? (float)N / (float)bbox.height : 1.0f;
        float s = std::min(sx, sy) * 0.90f; // 留一点边界（不算“美化”，是避免裁切）

        float angleRad = 0.0f;

        if (p.启用PCA旋转对齐 && contour.size() >= 5) {
            // PCA 主轴：取第一特征向量方向
            cv::Mat data((int)contour.size(), 2, CV_64F);
            for (int i = 0; i < (int)contour.size(); ++i) {
                data.at<double>(i, 0) = contour[i].x;
                data.at<double>(i, 1) = contour[i].y;
            }
            cv::PCA pca(data, cv::Mat(), cv::PCA::DATA_AS_ROW);
            cv::Vec2d v(pca.eigenvectors.at<double>(0, 0), pca.eigenvectors.at<double>(0, 1));
            angleRad = (float)std::atan2(v[1], v[0]); // 与x轴夹角
        }

        // 构造：先平移到中心，再旋转，再缩放，再平移到规范中心
        // 目标中心：((N-1)/2, (N-1)/2)
        float tx = (N - 1) * 0.5f;
        float ty = (N - 1) * 0.5f;

        float ca = std::cos(-angleRad);
        float sa = std::sin(-angleRad);

        // 2x3 仿射矩阵：
        // [ s*ca  -s*sa  tx - s*ca*cx + s*sa*cy ]
        // [ s*sa   s*ca  ty - s*sa*cx - s*ca*cy ]
        cv::Mat M(2, 3, CV_32F);
        M.at<float>(0, 0) = s * ca;  M.at<float>(0, 1) = -s * sa;
        M.at<float>(1, 0) = s * sa;  M.at<float>(1, 1) = s * ca;

        M.at<float>(0, 2) = tx - (s * ca) * cx - (-s * sa) * cy;
        M.at<float>(1, 2) = ty - (s * sa) * cx - (s * ca) * cy;

        return M;
    }

    static std::uint32_t 计算稳态阈值_(std::uint32_t seen, const 轮廓记忆参数& p) {
        std::uint32_t thr = p.稳态阈值_k;
        if (p.启用稳态比例 && seen > 0) {
            std::uint32_t r = (std::uint32_t)std::ceil((double)seen * (double)p.稳态比例);
            thr = std::max(thr, r);
        }
        if (thr < 1) thr = 1;
        return thr;
    }

    // ---- 8x8 blocks 编码：mask(0/255) -> VecIU64 ----
    static std::vector<std::uint64_t> 编码_8x8Blocks_(const cv::Mat& mask) {
        std::vector<std::uint64_t> out;
        if (mask.empty() || mask.type() != CV_8UC1) return out;

        const int H = mask.rows;
        const int W = mask.cols;
        if ((W % 8) != 0 || (H % 8) != 0) {
            // 为保持“无再加工”，这里直接拒绝（上层应确保 N 是8的倍数）
            return out;
        }

        const int nbx = W / 8;
        const int nby = H / 8;
        out.resize((std::size_t)nbx * (std::size_t)nby);

        for (int by = 0; by < nby; ++by) {
            for (int bx = 0; bx < nbx; ++bx) {
                std::uint64_t bits = 0;
                // 8x8 内，按行主序填 bit：bit(r*8+c)
                for (int r = 0; r < 8; ++r) {
                    const std::uint8_t* row = mask.ptr<std::uint8_t>(by * 8 + r);
                    for (int c = 0; c < 8; ++c) {
                        if (row[bx * 8 + c]) {
                            bits |= (1ull << (r * 8 + c));
                        }
                    }
                }
                out[(std::size_t)by * (std::size_t)nbx + (std::size_t)bx] = bits;
            }
        }
        return out;
    }

    static std::vector<std::uint64_t> 导出_并集编码_已加锁_(const 轮廓记忆状态& st) {
        // 直接从计数图生成并集编码，避免临时mask分配
        std::vector<std::uint64_t> out;
        if (st.计数图.empty()) return out;
        const int N = st.计数图.rows;
        if ((N % 8) != 0) return out;

        const int nb = N / 8;
        out.resize((std::size_t)nb * (std::size_t)nb);

        for (int by = 0; by < nb; ++by) {
            for (int bx = 0; bx < nb; ++bx) {
                std::uint64_t bits = 0;
                for (int r = 0; r < 8; ++r) {
                    const std::uint16_t* row = st.计数图.ptr<std::uint16_t>(by * 8 + r);
                    for (int c = 0; c < 8; ++c) {
                        if (row[bx * 8 + c] > 0) bits |= (1ull << (r * 8 + c));
                    }
                }
                out[(std::size_t)by * (std::size_t)nb + (std::size_t)bx] = bits;
            }
        }
        return out;
    }

    // ---- FNV1a64 over VecU64 ----
    static std::uint64_t FNV1a64_vec_u64_(const std::vector<std::uint64_t>& v) {
        constexpr std::uint64_t FNV_OFFSET = 1469598103934665603ull;
        constexpr std::uint64_t FNV_PRIME = 1099511628211ull;
        std::uint64_t h = FNV_OFFSET;
        const std::uint8_t* p = (const std::uint8_t*)v.data();
        std::size_t n = v.size() * sizeof(std::uint64_t);
        for (std::size_t i = 0; i < n; ++i) {
            h ^= (std::uint64_t)p[i];
            h *= FNV_PRIME;
        }
        // 也混入长度，避免不同长度前缀冲突
        h ^= (std::uint64_t)v.size();
        h *= FNV_PRIME;
        return h;
    }
};
