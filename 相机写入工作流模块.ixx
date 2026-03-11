module;
#include <afx.h>

export module 相机写入工作流模块;

import 外设模块;
import 基础数据类型模块;
import 点簇分割模块;
import 存在提取模块;
import <vector>;
import <memory>;
import <thread>;
import <atomic>;
import <chrono>;
import <utility>;
import <exception>;
import <stdexcept>;
import <unordered_map>;
import <cmath>;
import <algorithm>;

// ============================================================================
// 相机写入工作流模块
// 目的：把“相机帧 -> 点簇 -> (存在观测列表 + ROI列表)”封装成可复用的内部流水线，
//      可选：在此处做“单帧雕刻”（仅用于驱动显示/缓存；稳定写入由独立线程负责）。
// ============================================================================

export struct 相机写入参数 {
    // A) 深度预处理（建议开：空间 + 时间 + 洞填补）
    bool 启用深度预处理 = true;
    深度滤波参数 深度滤波{};

    // B) 点簇分割
    点簇分割参数 分割{};

    // C) 观测提取
    观测提取参数 提取{};

    // D) 会话线程
    int 等待帧超时毫秒 = 50;
    int 空转睡眠毫秒 = 1;


};



// ============================================================================
// “帧 -> 观测列表”的纯处理器（无世界树依赖）
// ============================================================================
export class 相机帧处理器 {
public:
    explicit 相机帧处理器(相机写入参数 p = {}) : p_(std::move(p)) {}

    void 设置参数(const 相机写入参数& p) { p_ = p; }
    const 相机写入参数& 参数() const noexcept { return p_; }

    bool 处理一帧(帧处理结果& out)
    {
        out.存在观测列表.clear();
        out.点簇增强结果列表.clear();
        out.轮廓观测列表.clear();

        if (!out.原始场景帧) return false;

        auto sp可写帧 = std::const_pointer_cast<结构体_原始场景帧>(out.原始场景帧);
        auto& 帧 = *sp可写帧;

        if (!帧.有效深度()) return false;

        if (p_.启用深度预处理) {
            深度滤波链_.处理到帧(帧, p_.深度滤波);
        }


        // 2) 点簇分割 + 增强
        out.点簇增强结果列表 = 分割器_.分割点簇_增强(帧, p_.分割);

        // 3) 观测 + ROI（严格 1:1 对齐，供下游雕刻/写入）
        提取器_.从点簇增强列表提取观测与轮廓观测(
            out.点簇增强结果列表,
            out.存在观测列表,
            out.轮廓观测列表,
            p_.提取,
            out.原始场景帧
        );

        // 3.1) 共享原始帧：避免每个 obs 拷贝一整帧（深度/颜色/点云）
        {
            std::shared_ptr<const 结构体_原始场景帧> 帧只读 = out.原始场景帧;
            for (auto& obs : out.存在观测列表) {
                obs.原始场景帧 = 帧只读;
            }
        }


        if (out.存在观测列表.empty()) return false;





        return true;
    }

private:
    相机写入参数 p_{};
    深度滤波链类 深度滤波链_{};
    点簇分割类   分割器_{};
    存在提取类   提取器_{};
};

// ============================================================================
// 写入调用：避免 if constexpr，靠重载/约束选择“最匹配的签名”
// ============================================================================
template<class 写入器>
static inline void 调用写入(写入器& w, const 外设数据包头& head, 帧处理结果& out)
    requires requires { w(head, out); }
{
    w(head, out);
}

template<class 写入器>
static inline void 调用写入(写入器& w, const 外设数据包头& head, 帧处理结果& out)
    requires (!requires { w(head, out); }) && requires { w(head, out.存在观测列表); }
{
    w(head, out.存在观测列表);
}

template<class 写入器>
static inline void 调用写入(写入器& w, const 外设数据包头& head, 帧处理结果& out)
    requires (!requires { w(head, out); }) && (!requires { w(head, out.存在观测列表); }) && requires { w(head, out.存在观测列表, out.原始场景帧.get(), &out.轮廓观测列表); }
{
    w(head, out.存在观测列表, out.原始场景帧.get(), &out.轮廓观测列表);
}

// ============================================================================
// 外设会话：拉帧 + 处理 + 写入
// ============================================================================
export template<class 写入器>
class 相机会话 {
public:
    相机会话(外设接口<结构体_原始场景帧>& cam, 写入器 writer, 相机写入参数 p = {})
        : cam_(cam), writer_(std::move(writer)), processor_(std::move(p)) {
    }

    ~相机会话() { 停止(); }

    void 启动() {
        bool expected = false;
        if (!running_.compare_exchange_strong(expected, true)) return;
        worker_ = std::thread([this] { 循环_(); });
    }

    void 停止() {
        running_.store(false);
        if (worker_.joinable()) worker_.join();
    }

    相机帧处理器& 处理器() noexcept { return processor_; }

private:
    void 循环_() {
        外设数据包<结构体_原始场景帧> pkg;
        帧处理结果 out;

        while (running_.load()) {
            const auto r = cam_.等待下一包(pkg, processor_.参数().等待帧超时毫秒);
            if (r == 枚举_取数结果::成功) {
                // 避免每帧拷贝大数组（深度/颜色/点云）。
                out.原始场景帧 = std::make_shared<结构体_原始场景帧>(std::move(pkg.数据));

                try {
                    if (!processor_.处理一帧(out)) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(processor_.参数().空转睡眠毫秒));
                        continue;
                    }

                    调用写入(writer_, pkg.头, out);
                }
                catch (const std::exception& e) {
                    TRACE("[相机会话] 异常：%hs\n", e.what());
                    std::this_thread::sleep_for(std::chrono::milliseconds(processor_.参数().空转睡眠毫秒));
                    continue;
                }
                catch (...) {
                    TRACE("[相机会话] 未知异常\n");
                    std::this_thread::sleep_for(std::chrono::milliseconds(processor_.参数().空转睡眠毫秒));
                    continue;
                }
                continue;
            }

            if (r == 枚举_取数结果::已停止 || r == 枚举_取数结果::故障) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(processor_.参数().空转睡眠毫秒));
        }
    }

private:
    外设接口<结构体_原始场景帧>& cam_;
    写入器 writer_;
    相机帧处理器 processor_;

    std::atomic<bool> running_{ false };
    std::thread worker_;
};
