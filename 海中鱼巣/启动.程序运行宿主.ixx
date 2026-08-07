module;

#include "程序运行结果.数据.h"
#include "领域/控制面板服务.h"
#include "界面/控制面板窗口.h"

#include <chrono>
#include <csignal>
#include <cstdint>
#include <functional>
#include <memory>
#include <thread>
#include <utility>

export module 海中鱼巣.启动.程序运行宿主;

import 海中鱼巣.装配.普通应用;
import 海中鱼巣.领域.初始化.系统;
import 海中鱼巣.界面.投影.控制面板启动;
import 海中鱼巣.适配.审计.数据库启动;

export namespace 海中鱼巣 {

using 程序信号处理函数 = void (*)(int);
using 程序信号安装函数 =
    std::function<程序信号处理函数(int, 程序信号处理函数)>;

}

export namespace 海中鱼巣 {

enum class 停止信号安装状态 : std::uint8_t {
    已安装 = 0,
    SIGINT失败 = 1,
    SIGTERM失败 = 2,
    SIGBREAK失败 = 3
};

struct 停止信号安装结果;
void 接收程序停止信号(int);

class 停止信号租约 {
public:
    停止信号租约() = default;
    ~停止信号租约() {
        if (安装函数) {
#ifdef _WIN32
            if (SIGBREAK已安装) {
                (void)安装函数(SIGBREAK, SIGBREAK原处理);
            }
#endif
            if (SIGTERM已安装) {
                (void)安装函数(SIGTERM, SIGTERM原处理);
            }
            if (SIGINT已安装) {
                (void)安装函数(SIGINT, SIGINT原处理);
            }
        }
        if (当前停止信号租约 == this) {
            当前停止信号租约 = nullptr;
        }
    }

    停止信号租约(const 停止信号租约&) = delete;
    停止信号租约& operator=(const 停止信号租约&) = delete;
    停止信号租约(停止信号租约&&) = delete;
    停止信号租约& operator=(停止信号租约&&) = delete;

    volatile std::sig_atomic_t 停止请求 = 0;

private:
    friend struct 停止信号安装结果;
    friend 停止信号安装结果 安装程序停止信号(程序信号安装函数) noexcept;
    friend void 接收程序停止信号(int);
    static inline 停止信号租约* 当前停止信号租约 = nullptr;
    程序信号安装函数 安装函数;
    程序信号处理函数 SIGINT原处理 = SIG_DFL;
    程序信号处理函数 SIGTERM原处理 = SIG_DFL;
#ifdef _WIN32
    程序信号处理函数 SIGBREAK原处理 = SIG_DFL;
#endif
    bool SIGINT已安装 = false;
    bool SIGTERM已安装 = false;
    bool SIGBREAK已安装 = false;
};

struct 停止信号安装结果 {
    停止信号安装状态 状态 = 停止信号安装状态::SIGINT失败;
    std::unique_ptr<停止信号租约> 租约;

    bool 成功() const noexcept {
        return 状态 == 停止信号安装状态::已安装 && 租约 != nullptr;
    }
};

}

namespace 海中鱼巣 {

void 接收程序停止信号(int) {
    if (停止信号租约::当前停止信号租约 != nullptr) {
        停止信号租约::当前停止信号租约->停止请求 = 1;
    }
}

}

export namespace 海中鱼巣 {

停止信号安装结果 安装程序停止信号(
    程序信号安装函数 安装函数 = 程序信号安装函数{&std::signal}) noexcept {
    if (!安装函数 || 停止信号租约::当前停止信号租约 != nullptr) {
        return {};
    }
    try {
        auto 租约 = std::make_unique<停止信号租约>();
        租约->安装函数 = std::move(安装函数);
        租约->SIGINT原处理 = 租约->安装函数(SIGINT, 接收程序停止信号);
        if (租约->SIGINT原处理 == SIG_ERR) {
            return {停止信号安装状态::SIGINT失败, nullptr};
        }
        租约->SIGINT已安装 = true;
        租约->SIGTERM原处理 = 租约->安装函数(SIGTERM, 接收程序停止信号);
        if (租约->SIGTERM原处理 == SIG_ERR) {
            return {停止信号安装状态::SIGTERM失败, nullptr};
        }
        租约->SIGTERM已安装 = true;
#ifdef _WIN32
        租约->SIGBREAK原处理 = 租约->安装函数(SIGBREAK, 接收程序停止信号);
        if (租约->SIGBREAK原处理 == SIG_ERR) {
            return {停止信号安装状态::SIGBREAK失败, nullptr};
        }
        租约->SIGBREAK已安装 = true;
#endif
        停止信号租约::当前停止信号租约 = 租约.get();
        return {停止信号安装状态::已安装, std::move(租约)};
    } catch (...) {
        return {};
    }
}

程序运行结果 运行控制面板宿主(
    普通应用上下文& 上下文,
    const 系统初始化结果& 初始化,
    const 控制面板启动投影结果& 投影,
    const 数据库启动审计结果& 审计,
    停止信号租约& 信号) {
    if (!初始化.成功() || !投影.成功()) {
        return {启动模式::普通控制面板, 程序运行状态::宿主失败,
            程序失败阶段::控制面板窗口};
    }
    const auto& 根需求 = 初始化.自我初始化->根需求;
    控制面板服务 控制面板(
        上下文.显示,
        上下文.节点,
        上下文.概念图,
        上下文.语素,
        上下文.需求,
        上下文.任务,
        上下文.方法,
        上下文.状态,
        投影.世界树,
        投影.语素树,
        {根需求.安全根需求.根需求, 根需求.服务根需求.根需求});
    控制面板窗口 窗口(控制面板, 上下文.数据库, 审计.快照);
    const auto 窗口结果 = 窗口.运行(&信号.停止请求);
    return 窗口结果.成功
        ? 程序运行结果{启动模式::普通控制面板, 程序运行状态::已完成,
            程序失败阶段::无}
        : 程序运行结果{启动模式::普通控制面板, 程序运行状态::宿主失败,
            程序失败阶段::控制面板窗口};
}

程序运行结果 运行无窗口宿主(
    普通应用上下文&,
    停止信号租约& 信号) {
    while (信号.停止请求 == 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return {启动模式::无窗口常驻, 程序运行状态::已完成,
        程序失败阶段::无};
}

}
