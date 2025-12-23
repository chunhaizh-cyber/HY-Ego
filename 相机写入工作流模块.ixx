
module;
#include <afx.h>

export module 相机写入工作流模块;

import 外设模块;
import 基础数据类型模块;
import 点簇分割模块;
import 存在提取模块;

import <vector>;
import <thread>;
import <atomic>;
import <chrono>;
import <utility>;

// ============================================================================
// 相机写入工作流模块
// 目的：把“相机帧 -> 点簇 -> 存在观测列表”封装成可复用的内部流水线，
//      然后把 “外设数据包 -> 观测 -> 写入器(回调)” 串起来。
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

    bool 处理一帧(const 结构体_原始场景帧& in, std::vector<结构体_存在观测>& out观测) {
        if (!in.有效深度()) return false;

        结构体_原始场景帧 帧 = in;
        if (p_.启用深度预处理) {
            深度滤波链_.处理到帧(帧, p_.深度滤波);
        }

        auto 增强列表 = 分割器_.分割点簇_增强(帧, p_.分割);
        out观测 = 提取器_.从点簇增强列表提取观测(增强列表, p_.提取);
        return !out观测.empty();
    }

private:
    相机写入参数 p_{};
    深度滤波链类 深度滤波链_{};
    点簇分割类   分割器_{};
    存在提取类   提取器_{};
};

// ============================================================================
// 外设会话：拉帧 + 处理 + 写入
// 写入器：writer(header, 观测列表)
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
        std::vector<结构体_存在观测> obs;

        while (running_.load()) {
            const auto r = cam_.等待下一包(pkg, processor_.参数().等待帧超时毫秒);
            if (r == 枚举_取数结果::成功) {
                obs.clear();
                processor_.处理一帧(pkg.数据, obs);
                writer_(pkg.头, obs);
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
