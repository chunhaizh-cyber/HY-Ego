// 数据仓库模块.ixx
module;
#define _WIN32_WINNT 0x0600  // 启用Windows Vista及以上版本的API
// Windows头文件
#include <windows.h>
#include <wct.h>
#include <tlhelp32.h> // 新增ToolHelp头文件
#include <psapi.h> // 新增进程和线程API头文件
export module 数据仓库模块;

import 模板模块;
import 主信息定义模块;
import 日志模块;

import <mutex>;
import <shared_mutex>;
import <vector>;
import <algorithm>;
import <cstdint>;
import <cassert>;

export namespace 数据仓库模块 {
 //   using 命名空间模板;


/*
锁顺序总规矩（多锁同时持有时必须遵循）：
  (A) 线程内部锁：消息队列 -> 帧 -> 外设 -> 参数
  (B) 数据仓库锁：语素 -> 自然语言 -> 高级信息 -> 世界 -> 特征值
  (C) OpenGL 资源锁（尽量避免与(B)同持）

建议：锁内只 swap/copy 句柄，重计算在锁外；严禁拿着队列/帧锁写世界树。
*/



// ====== 五条“链式数据仓库” ======
export using 世界链类型 = 链表模板<基础信息基类*>;
export inline 世界链类型 世界链{};

export using 特征值链类型 = 链表模板<特征值主信息类*>;
export inline 特征值链类型 特征值链{};

export using 语素链类型 = 链表模板<语素基类*>;
export inline 语素链类型 语素链{};

export using 自然语言链类型 = 链表模板<自然语言成分基类*>;
export inline 自然语言链类型 语言链{};

export using 需求信息链类型 = 链表模板<需求主信息类*>;
export inline 需求信息链类型 需求链{};

export using 任务信息链类型 = 链表模板<任务信息基类*>;
export inline 任务信息链类型 任务链{};

export using 方法信息链类型 = 链表模板<方法信息基类*>;
export inline 方法信息链类型 方法链{};

export struct 双写锁 {
    std::unique_lock<std::shared_mutex> a;
    std::unique_lock<std::shared_mutex> b;
    双写锁(std::shared_mutex& A, std::shared_mutex& B)
        : a(A, std::defer_lock), b(B, std::defer_lock) {
        std::lock(a, b);
    }
};

export enum class 枚举_锁域 : std::int32_t {
    世界链 = 10,
    特征值链 = 20,
    语素链 = 30,
    语言链 = 40,
    需求链 = 50,
    任务链 = 60,
    方法链 = 70,

    帧锁 = 100,
    消息队列锁 = 110,
    OpenGL锁 = 120,
    参数锁 = 130,
    缓存锁 = 140,
    缓存队列锁= 150,
    其它 = 200
};

export enum class 枚举_锁模式 : std::int32_t {
    互斥 = 0,
    读 = 1,
    写 = 2
};

// 锁顺序轨迹记录结构
export struct 锁轨迹记录 {
    std::uint64_t 线程ID;
    std::string 锁名;
    std::uint64_t 时间戳;
    std::string 调用点;
};

// 环形缓冲类，用于存储锁顺序轨迹
export class 锁轨迹环形缓冲 {
private:
    static constexpr size_t 缓冲大小 = 1024;
    锁轨迹记录 缓冲[缓冲大小];
    size_t 写入位置 = 0;
    std::mutex 缓冲锁;

public:
    void 记录(const 锁轨迹记录& 记录) {
        std::lock_guard<std::mutex> lk(缓冲锁);
        缓冲[写入位置] = 记录;
        写入位置 = (写入位置 + 1) % 缓冲大小;
    }

    std::vector<锁轨迹记录> 获取最近记录(size_t 数量) {
        std::lock_guard<std::mutex> lk(缓冲锁);
        std::vector<锁轨迹记录> 结果;
        结果.reserve(数量);
        
        size_t 起始位置 = (写入位置 >= 数量) ? (写入位置 - 数量) : 0;
        size_t 实际数量 = std::min(数量, 写入位置);
        
        for (size_t i = 0; i < 实际数量; ++i) {
            size_t 位置 = (起始位置 + i) % 缓冲大小;
            结果.push_back(缓冲[位置]);
        }
        
        return 结果;
    }
};

// 全局锁轨迹环形缓冲
export inline 锁轨迹环形缓冲 g_锁轨迹缓冲{};

export struct 锁请求 {
    枚举_锁域 域{};
    枚举_锁模式 模式{};
    void* 锁指针{};
    void(*lock_fn)(void*) = nullptr;
    void(*unlock_fn)(void*) = nullptr;
    std::string 锁名{};
    std::string 调用点{};

    static 锁请求 互斥(std::mutex& m, 枚举_锁域 d, const std::string& 锁名 = "", const std::string& 调用点 = "") {
        锁请求 r{}; r.域 = d; r.模式 = 枚举_锁模式::互斥; r.锁指针 = &m;
        r.锁名 = 锁名;
        r.调用点 = 调用点;
        r.lock_fn = [](void* p) { static_cast<std::mutex*>(p)->lock(); };
        r.unlock_fn = [](void* p) { static_cast<std::mutex*>(p)->unlock(); };
        return r;
    }
    static 锁请求 读(std::shared_mutex& m, 枚举_锁域 d, const std::string& 锁名 = "", const std::string& 调用点 = "") {
        锁请求 r{}; r.域 = d; r.模式 = 枚举_锁模式::读; r.锁指针 = &m;
        r.锁名 = 锁名;
        r.调用点 = 调用点;
        r.lock_fn = [](void* p) { static_cast<std::shared_mutex*>(p)->lock_shared(); };
        r.unlock_fn = [](void* p) { static_cast<std::shared_mutex*>(p)->unlock_shared(); };
        return r;
    }
    static 锁请求 写(std::shared_mutex& m, 枚举_锁域 d, const std::string& 锁名 = "", const std::string& 调用点 = "") {
        锁请求 r{}; r.域 = d; r.模式 = 枚举_锁模式::写; r.锁指针 = &m;
        r.锁名 = 锁名;
        r.调用点 = 调用点;
        r.lock_fn = [](void* p) { static_cast<std::shared_mutex*>(p)->lock(); };
        r.unlock_fn = [](void* p) { static_cast<std::shared_mutex*>(p)->unlock(); };
        return r;
    }
};

export class 锁调度器守卫 {
    std::vector<锁请求> reqs_;
    bool locked_ = false;
    static thread_local std::vector<std::int32_t> _stack;
    static std::int32_t top_level() {
        return _stack.empty() ? std::int32_t(-1) : _stack.back();
    }
public:
    锁调度器守卫() = default;
    explicit 锁调度器守卫(std::vector<锁请求> reqs) : reqs_(std::move(reqs)) { lock(); }
    锁调度器守卫(const 锁调度器守卫&) = delete;
    锁调度器守卫& operator=(const 锁调度器守卫&) = delete;
    锁调度器守卫(锁调度器守卫&& o) noexcept { *this = std::move(o); }
    锁调度器守卫& operator=(锁调度器守卫&& o) noexcept {
        if (this != &o) {
            unlock();
            reqs_ = std::move(o.reqs_);
            locked_ = o.locked_;
            o.locked_ = false;
        }
        return *this;
    }
    ~锁调度器守卫() { unlock(); }

    // 获取当前线程ID
    static std::uint64_t 获取线程ID() {
        return GetCurrentThreadId();
    }

    // 获取当前时间戳（微秒）
    static std::uint64_t 获取时间戳() {
        LARGE_INTEGER freq, counter;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&counter);
        return (counter.QuadPart * 1000000) / freq.QuadPart;
    }

    void lock() {
        if (locked_) return;
        // sort by domain, then mode (write before read), then address
        std::sort(reqs_.begin(), reqs_.end(), [](const 锁请求& a, const 锁请求& b) {
            if ((std::int32_t)a.域 != (std::int32_t)b.域) return (std::int32_t)a.域 < (std::int32_t)b.域;
            if ((std::int32_t)a.模式 != (std::int32_t)b.模式) return (std::int32_t)a.模式 > (std::int32_t)b.模式;
            return a.锁指针 < b.锁指针;
            });

        const std::int32_t held = top_level();
        const std::int32_t wantMax = reqs_.empty() ? held : (std::int32_t)reqs_.back().域;
        if (held >= 0 && !reqs_.empty()) {
            // if trying to acquire an earlier domain while holding a later domain => potential ABBA
            if ((std::int32_t)reqs_.front().域 < held) {
#ifdef _DEBUG
                // keep it as assert in debug; in release we still lock in sorted order.
                assert(false && "lock scheduler: potential ABBA (domain inversion)");
#endif
            }
        }

        // 开始获取锁，记录时间
        for (auto& r : reqs_) {
            if (!r.lock_fn) continue;
            
            const std::uint64_t 开始时间 = 获取时间戳();
            const std::uint64_t 线程ID = 获取线程ID();
            
            // 执行锁获取
            r.lock_fn(r.锁指针);
            
            const std::uint64_t 结束时间 = 获取时间戳();
            const std::uint64_t 耗时_us = 结束时间 - 开始时间;
            
            // 检查是否超过100ms
            if (耗时_us > 100000) { // 100ms = 100000us
                日志::运行f("[锁警告] 线程ID:{0}, 锁名:{1}, 调用点:{2}, 耗时:{3}ms",
                       线程ID, r.锁名.c_str(), r.调用点.c_str(), 耗时_us / 1000.0);
            }
            
            // 记录锁顺序轨迹
            锁轨迹记录 记录;
            记录.线程ID = 线程ID;
            记录.锁名 = r.锁名;
            记录.时间戳 = 结束时间;
            记录.调用点 = r.调用点;
            g_锁轨迹缓冲.记录(记录);
        }
        
        locked_ = true;
        _stack.push_back(std::max(held, wantMax));
    }

    void unlock() {
        if (!locked_) return;
        for (std::size_t i = reqs_.size(); i-- > 0; ) {
            auto& r = reqs_[i];
            if (r.unlock_fn) r.unlock_fn(r.锁指针);
        }
        locked_ = false;
        if (!_stack.empty()) _stack.pop_back();
    }
};

thread_local std::vector<std::int32_t> 锁调度器守卫::_stack;

// Windows Wait Chain Traversal (WCT) 工具类
export class WCT工具类 {
private:
    HANDLE hWCT = nullptr;

public:
    WCT工具类() {
        // 初始化WCT
        // 修正：使用头文件中定义的标志名并匹配当前 OpenThreadWaitChainSession 函数签名（Flags, callback）
        hWCT = OpenThreadWaitChainSession(WCT_OUT_OF_PROC_FLAG, nullptr);
    }

    ~WCT工具类() {
        if (hWCT) {
            CloseThreadWaitChainSession(hWCT);
        }
    }

    // 分析线程等待链
    bool 分析线程等待链(DWORD 线程ID) {
        if (!hWCT) return false;

        DWORD 节点Count = WCT_MAX_NODE_COUNT;
        WAITCHAIN_NODE_INFO* 节点Info = nullptr;
        BOOL isCycle = FALSE;

        // 为节点数组分配内存
        节点Info = (WAITCHAIN_NODE_INFO*)LocalAlloc(LPTR, sizeof(WAITCHAIN_NODE_INFO) * 节点Count);
        if (!节点Info) {
            日志::运行("[WCT] 分配内存失败");
            return false;
        }

        // 获取线程等待链（注意参数顺序与 wct.h 中声明一致）
        BOOL 成功 = GetThreadWaitChain(
            hWCT,
            0,                          // Context（这里不使用）
            WCTP_GETINFO_ALL_FLAGS,     // Flags
            线程ID,                     // ThreadId
            &节点Count,                 // NodeCount（输入/输出）
            节点Info,                   // NodeInfoArray
            &isCycle                    // IsCycle
        );

        if (!成功) {
            日志::异常f("[WCT] 获取线程等待链失败，错误码:{0}", GetLastError());
            LocalFree(节点Info);
            return false;
        }

        // 打印等待链
        日志::运行f("[WCT] 线程{0}的等待链 (节点数:{1})", 线程ID, 节点Count);
        for (DWORD i = 0; i < 节点Count; ++i) {
            const auto& 节点 = 节点Info[i];
            
            日志::运行f("  节点:{0}", i);
            // 使用头文件中定义的枚举值名称（Wct...）
            switch (节点.ObjectType) {
            case WctMutexType:
                日志::运行("互斥量");
                break;
            case WctThreadType:
                printf("线程");
                break;
            case WctProcessWaitType:
                日志::运行("进程/等待");
                break;
            case WctUnknownType:
                日志::运行("未知类型");
                break;
            default:
                日志::运行f("类型:{0}", static_cast<int>(节点.ObjectType));
                break;
            }

            if (节点.ObjectType == WctThreadType && 节点.ThreadObject.ThreadId != 0) {
                日志::运行f(" (线程ID:{0})", 节点.ThreadObject.ThreadId);
            } else {
                日志::运行("-------------");
            }
            
        }

        // 释放内存
        LocalFree(节点Info);
        return true;
    }

    // 分析所有线程的等待链
    void 分析所有线程等待链() {
        // 使用ToolHelp API获取当前进程的所有线程ID
        DWORD 当前进程ID = GetCurrentProcessId();
        std::vector<DWORD> 线程IDs;

        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) {
            日志::运行f("[WCT] CreateToolhelp32Snapshot失败，错误码:{0}", GetLastError());
            return;
        }

        THREADENTRY32 te32 = {};
        te32.dwSize = sizeof(THREADENTRY32);

        if (Thread32First(hSnapshot, &te32)) {
            do {
                if (te32.th32OwnerProcessID == 当前进程ID) {
                    线程IDs.push_back(te32.th32ThreadID);
                }
            } while (Thread32Next(hSnapshot, &te32));
        } else {
            日志::运行f("[WCT] Thread32First失败，错误码:{0}", GetLastError());
            CloseHandle(hSnapshot);
            return;
        }
        CloseHandle(hSnapshot);

        if (线程IDs.empty()) {
            日志::运行("[WCT] 没有找到线程");
            return;
        }

        // 分析每个线程的等待链
        for (DWORD tid : 线程IDs) {
            分析线程等待链(tid);
        }
    }
};

}//namespace 数据仓库模块

