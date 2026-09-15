#pragma once

#include "程序运行结果.数据.h"

#include <csignal>
#include <cstdint>
#include <functional>
#include <memory>

namespace 海中鱼巣 {

using 程序信号处理函数 = void (*)(int);
using 程序信号安装函数 =
    std::function<程序信号处理函数(int, 程序信号处理函数)>;

enum class 停止信号安装状态 : std::uint8_t {
    已安装 = 0,
    SIGINT失败 = 1,
    SIGTERM失败 = 2,
    SIGBREAK失败 = 3
};

enum class 程序周期维护状态 : std::uint8_t {
    已完成 = 0,
    稍后重试 = 1,
    内部不一致 = 2
};

using 程序周期维护回调 = std::function<程序周期维护状态()>;

struct 停止信号安装结果;
void 接收程序停止信号(int);

class 停止信号租约 {
public:
    停止信号租约() = default;
    ~停止信号租约();

    停止信号租约(const 停止信号租约&) = delete;
    停止信号租约& operator=(const 停止信号租约&) = delete;
    停止信号租约(停止信号租约&&) = delete;
    停止信号租约& operator=(停止信号租约&&) = delete;

    volatile std::sig_atomic_t 停止请求 = 0;

private:
    friend struct 停止信号安装结果;
    friend 停止信号安装结果 安装程序停止信号(程序信号安装函数) noexcept;
    friend void 接收程序停止信号(int);

    static 停止信号租约* 当前停止信号租约;
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

    bool 成功() const noexcept;
};

停止信号安装结果 安装程序停止信号(
    程序信号安装函数 安装函数 = 程序信号安装函数{&std::signal}) noexcept;

程序运行结果 运行无窗口宿主(停止信号租约& 信号,
    程序周期维护回调 维护回调 = {});

} // namespace 海中鱼巣
