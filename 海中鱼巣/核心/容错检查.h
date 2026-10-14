// 可选容错检查只提醒；工程异常出口诊断后终止，均不产生业务事实。
#pragma once

#include "日志系统.h"
#include <atomic>
#include <cstddef>
#include <exception>
#include <intrin.h>
#include <process.h>
#include <sstream>
#include <string>

#pragma comment(lib, "User32.lib")

#define HY_EGO_STRINGIZE_IMPL(值) #值
#define HY_EGO_STRINGIZE(值) HY_EGO_STRINGIZE_IMPL(值)
#define HY_EGO_WIDEN_IMPL(值) L##值
#define HY_EGO_WIDEN(值) HY_EGO_WIDEN_IMPL(值)
#define HY_EGO_WSTRINGIZE(值) HY_EGO_WIDEN(HY_EGO_STRINGIZE(值))

namespace 海中鱼巣 {
namespace 工程诊断内部 {

struct 致命诊断副本 final {
    wchar_t 文件[512]{};
    wchar_t 入口[256]{};
    wchar_t 原因[256]{};
    char 异常说明[512]{};
    int 行号 = 0;
    bool 有截断 = false;
};
inline std::atomic_flag 首故障 = ATOMIC_FLAG_INIT;
inline 致命诊断副本 诊断;

// 仅复制诊断文本；不分配节点、不取得业务锁，也不保留异常对象。
template<typename 字符, std::size_t 容量>
inline bool 复制文本(字符 (&目标)[容量], const 字符* 来源) noexcept {
    if (!来源) { 目标[0] = 0; return false; }
    std::size_t 长度 = 0;
    while (长度 + 1 < 容量 && 来源[长度]) {
        目标[长度] = 来源[长度]; ++长度;
    }
    目标[长度] = 0;
    return 来源[长度] != 0;
}

[[noreturn]] inline void 强制终止() noexcept {
    (void)TerminateProcess(GetCurrentProcess(), ERROR_UNHANDLED_EXCEPTION);
    // API失败或终止尚未完成时也绝不能回到原业务栈。
    __fastfail(FAST_FAIL_FATAL_APP_EXIT);
}

inline void 转换异常说明(const 致命诊断副本& 材料,
    wchar_t (&输出)[512]) noexcept {
    std::size_t 长度 = 0;
    while (长度 < sizeof(材料.异常说明) && 材料.异常说明[长度]) ++长度;
    if (!长度) { (void)复制文本(输出, L"未取得异常what()说明"); return; }
    if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, 材料.异常说明,
            static_cast<int>(长度), 输出, 511) > 0) return;
    // what()没有统一编码合同；非法UTF-8保留受控字节摘要，不猜本地代码页。
    constexpr wchar_t 十六进制[] = L"0123456789ABCDEF";
    const auto 前缀长度 = static_cast<std::size_t>(wcslen(L"非UTF-8字节摘要："));
    (void)复制文本(输出, L"非UTF-8字节摘要：");
    auto 位置 = 前缀长度;
    const auto 摘要长度 = 长度 < 64 ? 长度 : std::size_t{64};
    for (std::size_t i = 0; i < 摘要长度; ++i) {
        const auto 字节 = static_cast<unsigned char>(材料.异常说明[i]);
        输出[位置++] = 十六进制[字节 >> 4];
        输出[位置++] = 十六进制[字节 & 15];
        输出[位置++] = L' ';
    }
    if (长度 > 摘要长度) {
        constexpr wchar_t 截断提示[] = L"（摘要已截断）";
        for (const auto 字符 : 截断提示) 输出[位置++] = 字符;
    } else 输出[位置] = 0;
}

inline unsigned __stdcall 输出致命诊断(void* 参数) noexcept {
    const auto& 材料 = *static_cast<const 致命诊断副本*>(参数);
    try {
        wchar_t 异常说明[512]{};
        转换异常说明(材料, 异常说明);
        wchar_t 文本[2048]{};
        const auto 格式化 = _snwprintf_s(文本, _TRUNCATE,
            L"海中鱼巣代码工程异常\n入口：%ls\n文件：%ls\n行号：%d\n原因：%ls\n异常：%ls\n%ls"
            L"关闭此窗口后终止进程，请修复代码后重新启动。",
            材料.入口, 材料.文件, 材料.行号, 材料.原因, 异常说明,
            材料.有截断 ? L"诊断包含已截断字段。\n" : L"");
        if (格式化 < 0) (void)复制文本(文本,
            L"代码工程异常；诊断文本格式化失败。进程将终止，请修复代码后重新启动。");
        try {
            if (!记录逻辑错误日志(L"工程异常", 材料.入口, 文本))
                OutputDebugStringW(L"工程错误日志写入失败；仍将弹窗并终止。\n");
        } catch (...) { OutputDebugStringW(L"工程错误日志输出异常；仍将弹窗并终止。\n"); }
        OutputDebugStringW(文本);
        (void)MessageBoxW(nullptr, 文本, L"海中鱼巣：代码工程错误", MB_OK | MB_ICONERROR);
    } catch (...) { OutputDebugStringW(L"致命工程诊断失败；立即终止进程。\n"); }
    强制终止();
}
} // namespace 工程诊断内部

[[noreturn]] inline void 工程异常终止(const wchar_t* 文件, int 行号,
    const wchar_t* 入口, const wchar_t* 原因, const char* 异常说明 = nullptr) noexcept {
    using namespace 工程诊断内部;
    if (!首故障.test_and_set(std::memory_order_acq_rel)) {
        诊断.行号 = 行号;
        诊断.有截断 |= 复制文本(诊断.文件, 文件);
        诊断.有截断 |= 复制文本(诊断.入口, 入口);
        诊断.有截断 |= 复制文本(诊断.原因, 原因);
        诊断.有截断 |= 复制文本(诊断.异常说明, 异常说明);
        const auto 线程 = _beginthreadex(nullptr, 0, &输出致命诊断, &诊断, 0, nullptr);
        if (!线程) {
            强制终止();
        }
        (void)CloseHandle(reinterpret_cast<HANDLE>(线程));
    }
    // 外层可能仍持领域锁；只让独立诊断线程输出并终止，不返回或展开业务。
    for (;;) Sleep(INFINITE);
}
} // namespace 海中鱼巣

#ifdef HY_EGO_ENABLE_FAULT_TOLERANCE_CHECK

namespace 海中鱼巣 {

inline void 记录容错检查错误(const wchar_t* 文件, int 行号, const wchar_t* 条件, const wchar_t* 说明) {
    std::wostringstream 内容;
    内容 << L"海中鱼巣容错检查触发" << L'\n'
        << L"文件: " << 文件 << L'\n'
        << L"行号: " << 行号 << L'\n'
        << L"条件: " << 条件 << L'\n'
        << L"说明: " << 说明 << L'\n';

    (void)记录逻辑错误日志(L"容错检查", 条件, 内容.str());

    const std::wstring 文本 = 内容.str();
    OutputDebugStringW(文本.c_str());
    MessageBoxW(nullptr, 文本.c_str(), L"海中鱼巣容错检查", MB_OK | MB_ICONERROR);
}

}

#define 海中鱼巣容错检查(条件, 说明) \
    do { \
        if (!(条件)) { \
            ::海中鱼巣::记录容错检查错误(HY_EGO_WSTRINGIZE(__FILE__), __LINE__, HY_EGO_WSTRINGIZE(条件), 说明); \
        } \
    } while (false)

#else

#define 海中鱼巣容错检查(条件, 说明) do { } while (false)

#endif

namespace 海中鱼巣 {

inline bool 追根因检查(bool 条件, const wchar_t* 说明) {
    海中鱼巣容错检查(条件, 说明);
    return 条件;
}

}
