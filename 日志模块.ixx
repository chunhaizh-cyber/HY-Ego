module;

#include <chrono>
#include <cstdio>
#include <ctime>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream>
#include <format>
#include <mutex>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <utility>

export module 日志模块;

// 说明：
// - 运行日志与异常日志分两个文件。
// - 每天按日期自动切换新文件（YYYYMMDD）。
// - 线程安全；失败静默（避免日志写入导致业务崩溃）。
/*
用法（最小接入）

程序初始化（比如 OnInitDialog() 或主线程启动处）加：

import 日志模块;

void 初始化日志()
{
    日志参数 p{};
    p.根目录 = "./日志";
    p.文件前缀 = "海鱼";
    p.每条刷新 = true;      // 崩溃也尽量能落盘；想更快可设 false
    日志::初始化(p);
}


写运行日志：

日志::运行("系统启动");
日志::运行f("帧号={}, 点数={}", frameId, n);
日志::运行_警告("深度无效比例偏高");


写异常日志：

try {
    // ...
} catch (const std::exception& e) {
    日志::记录异常(e, "点簇分割失败");
}


关闭（可选，程序退出时）：

日志::关闭();
注意:日志模块内部保证线程安全（至少保护队列/文件写入）。
不要在持有大锁时做 heavy 日志：
大锁区内只做：LOG_MIN("xxx", a,b,c) 这种轻量入队
复杂内容：先拷贝数据到局部变量，退出锁区后再格式化/输出
日志里别调用会拿世界树锁的函数（尤其是 to_string(节点*) 这类“看着无害”的东西）。
必要时加“限流/采样”：例如同一类日志每秒最多N条，否则日志本身就是性能炸弹。
日志锁顺序固定：如果你一定要同步输出（方案A），规定“永远先拿日志锁，再拿其他锁”，并且最好只在调试模式这样做。
*/
export enum class 枚举_日志类别 : std::int8_t {
    运行 = 0,
    异常 = 1,
};

export enum class 枚举_日志级别 : std::int8_t {
    调试 = 0,
    信息 = 1,
    警告 = 2,
    错误 = 3,
    致命 = 4,
};

export struct 日志参数 {
    // 日志根目录（默认 ./日志）
    std::filesystem::path 根目录 = std::filesystem::path("./日志");
    // 文件名前缀（默认 海鱼）
    std::string 文件前缀 = "海鱼";
    // 写入后是否每条都 flush（默认 true，便于崩溃前落盘；追求性能可关）
    bool 每条刷新 = true;
    // 是否同时输出到调试器（OutputDebugStringA 风格）；这里不直接依赖 WinAPI，默认 false
    bool 输出到调试器 = false;
};

export namespace 日志 {
    // 初始化/反初始化
    void 初始化(const 日志参数& p = 日志参数{});
    void 关闭();

    // 基础写入（字符串）
    void 写(枚举_日志类别 类别, 枚举_日志级别 级别, std::string_view msg);

    // 便捷：运行日志
    inline void 运行(std::string_view msg) { 写(枚举_日志类别::运行, 枚举_日志级别::信息, msg); }
    inline void 运行_警告(std::string_view msg) { 写(枚举_日志类别::运行, 枚举_日志级别::警告, msg); }
    inline void 运行_错误(std::string_view msg) { 写(枚举_日志类别::运行, 枚举_日志级别::错误, msg); }

    // 便捷：异常日志
    inline void 异常(std::string_view msg) { 写(枚举_日志类别::异常, 枚举_日志级别::错误, msg); }
    inline void 异常_致命(std::string_view msg) { 写(枚举_日志类别::异常, 枚举_日志级别::致命, msg); }

    // format 版本（C++20 std::format）
    template <class... Args>
    inline void 运行f(std::format_string<Args...> fmt, Args&&... args) {
        写(枚举_日志类别::运行, 枚举_日志级别::信息, std::format(fmt, std::forward<Args>(args)...));
    }
    template <class... Args>
    inline void 异常f(std::format_string<Args...> fmt, Args&&... args) {
        写(枚举_日志类别::异常, 枚举_日志级别::错误, std::format(fmt, std::forward<Args>(args)...));
    }

    // 便捷：记录 std::exception
    void 记录异常(const std::exception& e, std::string_view 上下文 = {});
}

// =========================
// 实现（放在同一模块，便于直接 import 使用）
// =========================

export namespace 日志::detail {

    struct 单文件日志器 {
        std::mutex mtx;
        std::ofstream ofs;
        std::string 当前日期; // YYYYMMDD
        std::filesystem::path 根目录;
        std::string 文件前缀;
        std::string 文件标识; // "run" / "exception"
        bool 每条刷新 = true;
        bool 输出到调试器 = false;

        static std::string 取日期_YYYYMMDD() {
            using namespace std::chrono;
            auto now = system_clock::now();
            std::time_t tt = system_clock::to_time_t(now);
            std::tm tm{};
#if defined(_WIN32)
            localtime_s(&tm, &tt);
#else
            localtime_r(&tt, &tm);
#endif
            char buf[16]{};
            std::snprintf(buf, sizeof(buf), "%04d%02d%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
            return std::string(buf);
        }

        static std::string 取时间戳_YYYYMMDD_hhmmss_mmm() {
            using namespace std::chrono;
            auto now = system_clock::now();
            auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
            std::time_t tt = system_clock::to_time_t(now);
            std::tm tm{};
#if defined(_WIN32)
            localtime_s(&tm, &tt);
#else
            localtime_r(&tt, &tm);
#endif
            char buf[64]{};
            std::snprintf(buf, sizeof(buf),
                "%04d-%02d-%02d %02d:%02d:%02d.%03d",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                tm.tm_hour, tm.tm_min, tm.tm_sec,
                (int)ms.count());
            return std::string(buf);
        }

        static const char* 级别字符串(枚举_日志级别 lv) {
            switch (lv) {
            case 枚举_日志级别::调试: return "DBG";
            case 枚举_日志级别::信息: return "INF";
            case 枚举_日志级别::警告: return "WRN";
            case 枚举_日志级别::错误: return "ERR";
            case 枚举_日志级别::致命: return "FTL";
            default: return "UNK";
            }
        }

        void 配置(const 日志参数& p, std::string 标识) {
            根目录 = p.根目录;
            文件前缀 = p.文件前缀;
            文件标识 = std::move(标识);
            每条刷新 = p.每条刷新;
            输出到调试器 = p.输出到调试器;
        }

        std::filesystem::path 生成文件路径(const std::string& yyyymmdd) const {
            // logs/海鱼_run_20260118.低值g
            std::string name = std::format("{}_{}_{}.低值g", 文件前缀, 文件标识, yyyymmdd);
            return 根目录 / name;
        }

        void 确保打开_已加锁() {
            const std::string today = 取日期_YYYYMMDD();
            if (ofs.is_open() && today == 当前日期) return;

            // 切日 / 首次
            try {
                std::filesystem::create_directories(根目录);
            }
            catch (...) {
                // 目录创建失败：后续写入会静默失败
            }

            try {
                if (ofs.is_open()) ofs.close();
                当前日期 = today;
                auto path = 生成文件路径(today);
                ofs.open(path, std::ios::out | std::ios::app);
            }
            catch (...) {
                // 打开失败：静默
            }
        }

        void 写一行(枚举_日志级别 lv, std::string_view msg) {
            std::scoped_lock lk(mtx);
            确保打开_已加锁();
            if (!ofs.is_open()) return;

            // thread id
            std::ostringstream tid;
            tid << std::this_thread::get_id();

            const std::string ts = 取时间戳_YYYYMMDD_hhmmss_mmm();
            ofs << ts << " [" << 级别字符串(lv) << "]"
                << " [T" << tid.str() << "] "
                << msg << "\n";

            if (每条刷新) ofs.flush();

            // 可选：输出到调试器（不引 WinAPI，留给你接入）。
            (void)输出到调试器;
        }

        void 关闭() {
            std::scoped_lock lk(mtx);
            try {
                if (ofs.is_open()) ofs.close();
            }
            catch (...) {}
            当前日期.clear();
        }
    };

    inline 单文件日志器 g_run;
    inline 单文件日志器 g_ex;
    inline std::mutex g_init_mtx;
    inline bool g_inited = false;
    inline 日志参数 g_param{};

    inline void 确保初始化_已加锁() {
        if (g_inited) return;
        g_run.配置(g_param, "run");
        g_ex.配置(g_param, "exception");
        g_inited = true;
    }
}

namespace 日志 {
    inline void 初始化(const 日志参数& p) {
        std::scoped_lock lk(detail::g_init_mtx);
        detail::g_param = p;
        detail::g_inited = false;
        detail::确保初始化_已加锁();
    }

    inline void 关闭() {
        std::scoped_lock lk(detail::g_init_mtx);
        if (!detail::g_inited) return;
        detail::g_run.关闭();
        detail::g_ex.关闭();
        detail::g_inited = false;
    }

    inline void 写(枚举_日志类别 类别, 枚举_日志级别 级别, std::string_view msg) {
        // 无论是否显式初始化，都能工作：用默认参数懒初始化
        if (!detail::g_inited) {
            // 使用try_lock避免死锁
            if (detail::g_init_mtx.try_lock()) {
                if (!detail::g_inited) detail::确保初始化_已加锁();
                detail::g_init_mtx.unlock();
            } else {
                // 如果无法获取锁，直接返回，避免死锁
                return;
            }
        }

        try {
            if (类别 == 枚举_日志类别::运行) {
                detail::g_run.写一行(级别, msg);
            }
            else {
                detail::g_ex.写一行(级别, msg);
            }
        }
        catch (...) {
            // 日志写入不得影响主流程
        }
    }

    inline void 记录异常(const std::exception& e, std::string_view 上下文) {
        if (上下文.empty()) {
            写(枚举_日志类别::异常, 枚举_日志级别::错误, std::string_view(e.what()));
        }
        else {
            写(枚举_日志类别::异常, 枚举_日志级别::错误, std::format("{} | {}", 上下文, e.what()));
        }
    }
}

