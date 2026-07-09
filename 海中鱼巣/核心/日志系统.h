// 文件规则：日志系统只做人读诊断、运行观察、调试定位和审计辅助；不得承载机器事实或修复结构。
#pragma once

#include <chrono>
#include <cstdint>
#include <cwchar>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <string_view>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

namespace 海中鱼巣 {

enum class 日志类别 {
    逻辑错误,
    运行,
    调试,
    事件
};

enum class 事件类别 : std::uint32_t {
    未知 = 0,
    节点结构 = 1,
    主信息结构 = 2,
    关系结构 = 3,
    索引结构 = 4,
    领域结构 = 5
};

enum class 事件写入结果 : std::uint32_t {
    未确认 = 0,
    已确认写入 = 1,
    入口拒绝 = 2,
    业务失败 = 3
};

struct 事件材料 {
    std::wstring 入口名称;
    事件类别 类别 = 事件类别::未知;
    事件写入结果 写入结果 = 事件写入结果::未确认;
    std::uint64_t 仓库编号 = 0;
    std::uint64_t 节点编号 = 0;
    std::uint64_t 节点版本 = 0;
    std::uint64_t 主信息编号 = 0;
    std::uint64_t 主信息版本 = 0;
    std::uint64_t 关系编号 = 0;
    std::uint64_t 关系版本 = 0;
    std::uint64_t 索引键 = 0;
    std::uint64_t 结构事件顺序号 = 0;
    std::uint64_t 结构事件时间戳 = 0;
    std::wstring 摘要;
    bool 请求裁决运行期事实 = false;
    bool 请求恢复运行期结构 = false;
};

inline std::filesystem::path 日志目录路径() {
    return L"D:\\海中鱼巣\\日志";
}

inline bool 调试日志切片名可用(std::wstring_view 切片名) {
    if (切片名.empty()) {
        return false;
    }
    for (const wchar_t 字符 : 切片名) {
        if (字符 == L'\\' || 字符 == L'/' || 字符 == L':' || 字符 == L'*'
            || 字符 == L'?' || 字符 == L'"' || 字符 == L'<' || 字符 == L'>'
            || 字符 == L'|' || 字符 == L'.') {
            return false;
        }
    }
    return true;
}

inline std::filesystem::path 日志文件路径(日志类别 类别, std::wstring_view 调试切片名 = {}) {
    switch (类别) {
    case 日志类别::逻辑错误:
        return 日志目录路径() / L"逻辑错误.log";
    case 日志类别::运行:
        return 日志目录路径() / L"运行.log";
    case 日志类别::事件:
        return 日志目录路径() / L"事件.log";
    case 日志类别::调试:
        if (!调试日志切片名可用(调试切片名)) {
            return {};
        }
        return 日志目录路径() / (std::wstring(L"调试_") + std::wstring(调试切片名) + L".log");
    }
    return {};
}

inline std::wstring 当前日志时间文本() {
    const auto 当前时间点 = std::chrono::system_clock::now();
    const std::time_t 当前时间 = std::chrono::system_clock::to_time_t(当前时间点);
    std::tm 本地时间{};
    localtime_s(&本地时间, &当前时间);

    wchar_t 时间文本[32]{};
    std::wcsftime(时间文本, 32, L"%Y-%m-%d %H:%M:%S", &本地时间);
    return 时间文本;
}

inline const wchar_t* 日志类别文本(日志类别 类别) {
    switch (类别) {
    case 日志类别::逻辑错误:
        return L"逻辑错误";
    case 日志类别::运行:
        return L"运行";
    case 日志类别::调试:
        return L"调试";
    case 日志类别::事件:
        return L"事件";
    }
    return L"未知";
}

inline const wchar_t* 事件类别文本(事件类别 类别) {
    switch (类别) {
    case 事件类别::节点结构:
        return L"节点结构";
    case 事件类别::主信息结构:
        return L"主信息结构";
    case 事件类别::关系结构:
        return L"关系结构";
    case 事件类别::索引结构:
        return L"索引结构";
    case 事件类别::领域结构:
        return L"领域结构";
    case 事件类别::未知:
        return L"未知";
    }
    return L"未知";
}

inline const wchar_t* 事件写入结果文本(事件写入结果 结果) {
    switch (结果) {
    case 事件写入结果::已确认写入:
        return L"已确认写入";
    case 事件写入结果::入口拒绝:
        return L"入口拒绝";
    case 事件写入结果::业务失败:
        return L"业务失败";
    case 事件写入结果::未确认:
        return L"未确认";
    }
    return L"未确认";
}

inline bool 事件类别有效(事件类别 类别) {
    return 类别 == 事件类别::节点结构
        || 类别 == 事件类别::主信息结构
        || 类别 == 事件类别::关系结构
        || 类别 == 事件类别::索引结构
        || 类别 == 事件类别::领域结构;
}

inline bool 事件写入结果有效(事件写入结果 结果) {
    return 结果 == 事件写入结果::已确认写入
        || 结果 == 事件写入结果::入口拒绝
        || 结果 == 事件写入结果::业务失败;
}

inline bool 事件材料含结构材料(const 事件材料& 材料) {
    return 材料.仓库编号 != 0
        || 材料.节点编号 != 0
        || 材料.主信息编号 != 0
        || 材料.关系编号 != 0
        || 材料.索引键 != 0;
}

inline bool 事件材料是否可写入(const 事件材料& 材料) {
    return !材料.入口名称.empty()
        && 事件类别有效(材料.类别)
        && 事件写入结果有效(材料.写入结果)
        && 事件材料含结构材料(材料)
        && !材料.请求裁决运行期事实
        && !材料.请求恢复运行期结构;
}

inline std::string 转为UTF8(std::wstring_view 文本) {
    if (文本.empty() || 文本.size() > static_cast<std::size_t>((std::numeric_limits<int>::max)())) {
        return {};
    }

    const int 输入长度 = static_cast<int>(文本.size());
    const int 输出长度 = WideCharToMultiByte(CP_UTF8, 0, 文本.data(), 输入长度, nullptr, 0, nullptr, nullptr);
    if (输出长度 <= 0) {
        return {};
    }

    std::string 结果(static_cast<std::size_t>(输出长度), '\0');
    WideCharToMultiByte(CP_UTF8, 0, 文本.data(), 输入长度, 结果.data(), 输出长度, nullptr, nullptr);
    return 结果;
}

inline bool 写入日志(日志类别 类别, std::wstring_view 模块, std::wstring_view 入口, std::wstring_view 内容, std::wstring_view 调试切片名 = {}) {
    if (内容.empty()) {
        return false;
    }

    const auto 文件路径 = 日志文件路径(类别, 调试切片名);
    if (文件路径.empty()) {
        return false;
    }

    std::error_code 错误码;
    std::filesystem::create_directories(日志目录路径(), 错误码);
    if (错误码) {
        return false;
    }

    std::wostringstream 日志内容;
    日志内容 << L"[" << 当前日志时间文本() << L"]"
        << L" 类别=" << 日志类别文本(类别)
        << L" 模块=" << 模块
        << L" 入口=" << 入口
        << L'\n'
        << 内容 << L"\n\n";

    const std::string UTF8内容 = 转为UTF8(日志内容.str());
    if (UTF8内容.empty()) {
        return false;
    }

    std::ofstream 日志(文件路径, std::ios::app | std::ios::binary);
    if (!日志.is_open()) {
        return false;
    }

    日志 << UTF8内容;
    return 日志.good();
}

inline bool 记录逻辑错误日志(std::wstring_view 模块, std::wstring_view 入口, std::wstring_view 内容) {
    return 写入日志(日志类别::逻辑错误, 模块, 入口, 内容);
}

inline bool 记录运行日志(std::wstring_view 模块, std::wstring_view 入口, std::wstring_view 内容) {
    return 写入日志(日志类别::运行, 模块, 入口, 内容);
}

inline bool 记录事件日志(std::wstring_view 模块, std::wstring_view 入口, std::wstring_view 内容) {
    return 写入日志(日志类别::事件, 模块, 入口, 内容);
}

inline bool 事件日志可裁决运行期事实(const 事件材料&) {
    return false;
}

inline bool 事件日志可直接恢复运行期结构(const 事件材料&) {
    return false;
}

inline bool 记录结构事件日志(const 事件材料& 材料) {
    if (!事件材料是否可写入(材料)) {
        return false;
    }

    std::wostringstream 内容;
    内容 << L"事件类别=" << 事件类别文本(材料.类别)
        << L"\n写入结果=" << 事件写入结果文本(材料.写入结果)
        << L"\n仓库编号=" << 材料.仓库编号
        << L"\n节点编号=" << 材料.节点编号
        << L"\n节点版本=" << 材料.节点版本
        << L"\n主信息编号=" << 材料.主信息编号
        << L"\n主信息版本=" << 材料.主信息版本
        << L"\n关系编号=" << 材料.关系编号
        << L"\n关系版本=" << 材料.关系版本
        << L"\n索引键=" << 材料.索引键
        << L"\n结构事件顺序号=" << 材料.结构事件顺序号
        << L"\n结构事件时间戳=" << 材料.结构事件时间戳
        << L"\n摘要=" << 材料.摘要;
    return 记录事件日志(L"结构事件", 材料.入口名称, 内容.str());
}

inline bool 记录调试日志(std::wstring_view 调试切片名, std::wstring_view 模块, std::wstring_view 入口, std::wstring_view 内容) {
    return 写入日志(日志类别::调试, 模块, 入口, 内容, 调试切片名);
}

}

#ifdef HY_EGO_DEBUG_LOG_MIN_LOOP
#define 海中鱼巣记录最小闭环调试日志(模块, 入口, 内容) \
    do { \
        (void)::海中鱼巣::记录调试日志(L"MIN_LOOP", 模块, 入口, 内容); \
    } while (false)
#else
#define 海中鱼巣记录最小闭环调试日志(模块, 入口, 内容) do { } while (false)
#endif
