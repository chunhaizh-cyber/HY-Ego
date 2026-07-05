// 文件规则：容错检查只做诊断提醒；不得承载机器事实、不得修复结构、不得替代入口前置拒绝。
#pragma once

#ifdef HY_EGO_ENABLE_FAULT_TOLERANCE_CHECK

#include <Windows.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#pragma comment(lib, "User32.lib")

namespace 海中鱼巣 {

inline void 记录容错检查错误(const wchar_t* 文件, int 行号, const wchar_t* 条件, const wchar_t* 说明) {
    std::wostringstream 内容;
    内容 << L"海中鱼巣容错检查触发" << L'\n'
        << L"文件: " << 文件 << L'\n'
        << L"行号: " << 行号 << L'\n'
        << L"条件: " << 条件 << L'\n'
        << L"说明: " << 说明 << L'\n';

    const std::filesystem::path 日志目录 = L"D:\\海中鱼巣\\日志";
    std::error_code 错误码;
    std::filesystem::create_directories(日志目录, 错误码);
    std::wofstream 日志(日志目录 / L"逻辑错误.log", std::ios::app);
    if (日志.is_open()) {
        日志 << 内容.str() << L'\n';
    }

    const std::wstring 文本 = 内容.str();
    OutputDebugStringW(文本.c_str());
    MessageBoxW(nullptr, 文本.c_str(), L"海中鱼巣容错检查", MB_OK | MB_ICONERROR);
}

}

#define HY_EGO_STRINGIZE_IMPL(值) #值
#define HY_EGO_STRINGIZE(值) HY_EGO_STRINGIZE_IMPL(值)
#define HY_EGO_WIDEN_IMPL(值) L##值
#define HY_EGO_WIDEN(值) HY_EGO_WIDEN_IMPL(值)
#define HY_EGO_WSTRINGIZE(值) HY_EGO_WIDEN(HY_EGO_STRINGIZE(值))

#define 海中鱼巣容错检查(条件, 说明) \
    do { \
        if (!(条件)) { \
            ::海中鱼巣::记录容错检查错误(HY_EGO_WSTRINGIZE(__FILE__), __LINE__, HY_EGO_WSTRINGIZE(条件), 说明); \
        } \
    } while (false)

#else

#define 海中鱼巣容错检查(条件, 说明) do { } while (false)

#endif
