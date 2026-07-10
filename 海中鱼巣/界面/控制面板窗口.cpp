// 文件规则：原生窗口只读取控制面板服务材料；窗口消息、控件文本和按钮状态不得成为机器事实或动作来源。
#include "控制面板窗口.h"
#include "../核心/日志系统.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <CommCtrl.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <sstream>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

#pragma comment(lib, "Comctl32.lib")

namespace 海中鱼巣 {

namespace {

constexpr int 刷新按钮编号 = 1001;
constexpr int 操作请求按钮编号 = 1002;
constexpr int 关闭按钮编号 = 1003;
constexpr int 导航栏编号 = 1004;
constexpr int 分页树菜单编号 = 1101;
constexpr int 分页系统信息菜单编号 = 1102;
constexpr int 分页日志菜单编号 = 1103;
constexpr int 上一分页菜单编号 = 1104;
constexpr int 下一分页菜单编号 = 1105;
constexpr int 树分类菜单编号起点 = 1200;
constexpr UINT_PTR 停止检查计时器编号 = 1;
constexpr UINT 停止检查间隔毫秒 = 100;
constexpr wchar_t 窗口类名称[] = L"海中鱼巣控制面板窗口类";
constexpr wchar_t 窗口标题[] = L"海中鱼巣控制面板";

enum class 控制面板分页 : std::uint32_t {
    树 = 0,
    系统信息 = 1,
    日志 = 2
};

const std::array<const wchar_t*, 3> 分页名称{
    L"树",
    L"系统信息",
    L"日志"
};

const std::array<const wchar_t*, 6> 树分类名称{
    L"世界树",
    L"概念树",
    L"语素树",
    L"需求树",
    L"任务树",
    L"方法树"
};

控制面板请求 创建只读请求() {
    return {true, false, false, false, false, false};
}

const wchar_t* 树分类说明(控制面板树分类 分类) {
    switch (分类) {
    case 控制面板树分类::世界树:
        return L"世界结构的只读导航入口。显示内容来自本次启动时读取的普通父子关系快照。";
    case 控制面板树分类::概念树:
        return L"概念结构的只读导航入口。真实概念 DTO 由后续只读聚合计划承接。";
    case 控制面板树分类::语素树:
        return L"语素结构的只读导航入口。显示文本不参与机器事实裁决。";
    case 控制面板树分类::需求树:
        return L"需求结构的只读导航入口。控制面板不得裁决需求满足。";
    case 控制面板树分类::任务树:
        return L"任务结构的只读导航入口。控制面板不得裁决任务完成。";
    case 控制面板树分类::方法树:
        return L"方法结构的只读导航入口。按钮不得直接执行领域方法。";
    case 控制面板树分类::状态详情:
        return L"状态只能作为详情材料，不是控制面板树分类。";
    case 控制面板树分类::动态详情:
        return L"动态只能作为详情材料，不是控制面板树分类。";
    }
    return L"未知分类。";
}

std::uint32_t 分页索引(控制面板分页 分页) {
    return static_cast<std::uint32_t>(分页);
}

控制面板分页 从分页索引读取分页(std::uint32_t 索引) {
    switch (索引) {
    case 0:
        return 控制面板分页::树;
    case 1:
        return 控制面板分页::系统信息;
    case 2:
        return 控制面板分页::日志;
    default:
        return 控制面板分页::树;
    }
}

int 分页菜单编号(控制面板分页 分页) {
    switch (分页) {
    case 控制面板分页::树:
        return 分页树菜单编号;
    case 控制面板分页::系统信息:
        return 分页系统信息菜单编号;
    case 控制面板分页::日志:
        return 分页日志菜单编号;
    }
    return 分页树菜单编号;
}

void 显示控件(HWND 控件, bool 显示) {
    if (控件 != nullptr) {
        ShowWindow(控件, 显示 ? SW_SHOW : SW_HIDE);
    }
}

std::wstring 日志文件状态文本(日志类别 类别) {
    const auto 路径 = 日志文件路径(类别);
    if (路径.empty()) {
        return L"日志路径不可用";
    }
    std::error_code 错误码;
    const bool 文件存在 = std::filesystem::exists(路径, 错误码);
    if (错误码) {
        return L"状态读取失败";
    }
    if (!文件存在) {
        return L"尚未生成";
    }
    const auto 字节数 = std::filesystem::file_size(路径, 错误码);
    if (错误码) {
        return L"已生成，大小读取失败";
    }
    return L"已生成 | " + std::to_wstring(字节数) + L" 字节";
}

void 追加日志文件行(std::wostringstream& 文本, 日志类别 类别) {
    const auto 路径 = 日志文件路径(类别);
    文本 << 日志类别文本(类别)
        << L"："
        << (路径.empty() ? std::wstring(L"路径不可用") : 路径.wstring())
        << L"\r\n"
        << L"状态：" << 日志文件状态文本(类别)
        << L"\r\n\r\n";
}

std::uint64_t 计算树节点数量(const 控制面板树节点材料& 节点材料) {
    std::uint64_t 数量 = 1;
    for (const auto& 子节点 : 节点材料.子节点组) {
        数量 += 计算树节点数量(子节点);
    }
    return 数量;
}

bool 树节点显示语义有效(const 控制面板树节点材料& 节点材料) {
    if (!句柄有效(节点材料.节点)
        || !句柄有效(节点材料.名称基础信息)
        || !句柄有效(节点材料.类型基础信息)
        || !句柄有效(节点材料.名称语素入口)
        || !句柄有效(节点材料.类型语素入口)
        || 节点材料.显示名称.empty()) {
        return false;
    }
    return std::all_of(
        节点材料.子节点组.begin(),
        节点材料.子节点组.end(),
        [](const 控制面板树节点材料& 子节点) {
            return 树节点显示语义有效(子节点);
        });
}

struct 控制面板树项引用 {
    std::uint32_t 分类索引 = 0;
    const 控制面板树节点材料* 节点材料 = nullptr;
    bool 是结构节点 = false;
};

}

struct 控制面板窗口::窗口实现 {
    const 控制面板服务& 控制面板;
    const SQL数据库适配& 数据库;
    控制面板窗口快照 当前快照;
    const volatile std::sig_atomic_t* 停止请求 = nullptr;

    HWND 主窗口 = nullptr;
    HWND 标题文本 = nullptr;
    HWND 边界文本 = nullptr;
    HWND 导航栏 = nullptr;
    HWND 树视图 = nullptr;
    HWND 节点数文本 = nullptr;
    HWND 关系数文本 = nullptr;
    HWND 索引数文本 = nullptr;
    HWND 详情文本 = nullptr;
    HWND 数据库标题文本 = nullptr;
    HWND 数据库列表 = nullptr;
    HWND 刷新按钮 = nullptr;
    HWND 操作请求按钮 = nullptr;
    HWND 关闭按钮 = nullptr;
    HWND 状态栏 = nullptr;
    HMENU 菜单栏 = nullptr;
    HMENU 分页菜单 = nullptr;
    HMENU 树菜单 = nullptr;
    HACCEL 快捷键表 = nullptr;
    HFONT 正文字体 = nullptr;
    HFONT 标题字体 = nullptr;

    控制面板分页 当前分页 = 控制面板分页::树;
    std::uint32_t 当前分类索引 = 0;
    std::size_t 当前树项引用序号 = 0;
    std::vector<控制面板树项引用> 树项引用组;
    bool 已显示窗口 = false;
    bool 追根因错误 = false;
    bool 窗口类由本次注册 = false;
    std::wstring 失败阶段;

    窗口实现(const 控制面板服务& 面板服务, const SQL数据库适配& 数据库适配,
        结构统计快照 启动结构统计, 控制面板树结构材料 世界树结构材料,
        控制面板树结构材料 语素树结构材料)
        : 控制面板(面板服务), 数据库(数据库适配) {
        当前快照.启动结构统计 = 启动结构统计;
        当前快照.世界树结构材料 = std::move(世界树结构材料);
        当前快照.语素树结构材料 = std::move(语素树结构材料);
    }

    ~窗口实现() {
        销毁快捷键表();
        销毁字体();
        if (窗口类由本次注册) {
            UnregisterClassW(窗口类名称, GetModuleHandleW(nullptr));
        }
    }

    bool 读取只读材料() {
        const auto 总览材料 = 控制面板.生成只读总览材料(创建只读请求());
        const auto 树材料组 = 控制面板.生成六类树视图候选材料();
        const bool 总览有效 =
            总览材料.状态 == 控制面板请求状态::可展示
            && 总览材料.人读材料
            && 总览材料.来源显示服务
            && !总览材料.允许写业务事实;
        const bool 树材料有效 = std::all_of(
            树材料组.begin(),
            树材料组.end(),
            [](const 控制面板树视图材料& 材料) {
                return 材料.状态 == 控制面板请求状态::可展示
                    && 材料.可作为树视图分类
                    && !材料.只作为详情材料;
            });
        const bool 统计快照有效 =
            当前快照.启动结构统计.命名空间.类型 == 缓存类型::结构统计
            && 当前快照.启动结构统计.命名空间.来源 == 来源结构类型::结构仓库集合;
        const bool 世界树材料有效 =
            当前快照.世界树结构材料.状态 == 控制面板请求状态::可展示
            && 当前快照.世界树结构材料.分类 == 控制面板树分类::世界树
            && 当前快照.世界树结构材料.可展示
            && 当前快照.世界树结构材料.只读快照
            && 树节点显示语义有效(当前快照.世界树结构材料.根节点);
        const bool 语素树材料有效 =
            当前快照.语素树结构材料.状态 == 控制面板请求状态::可展示
            && 当前快照.语素树结构材料.分类 == 控制面板树分类::语素树
            && 当前快照.语素树结构材料.可展示
            && 当前快照.语素树结构材料.只读快照
            && 树节点显示语义有效(当前快照.语素树结构材料.根节点);
        if (!总览有效 || !树材料有效 || !统计快照有效 || !世界树材料有效 || !语素树材料有效) {
            标记追根因错误(L"控制面板只读材料、世界树快照、语素树快照或启动结构统计不符合已确认边界");
            return false;
        }
        当前快照.总览材料 = 总览材料;
        当前快照.树视图材料组 = 树材料组;
        return true;
    }

    void 读取数据库审计材料() {
        当前快照.数据库审计查询 = 数据库.读取最近结构统计审计(50);
    }

    bool 注册窗口类() {
        INITCOMMONCONTROLSEX 公共控件初始化{
            sizeof(INITCOMMONCONTROLSEX),
            ICC_TREEVIEW_CLASSES | ICC_BAR_CLASSES
        };
        if (!InitCommonControlsEx(&公共控件初始化)) {
            标记追根因错误(L"Windows 公共控件初始化失败");
            return false;
        }

        WNDCLASSEXW 窗口类{};
        窗口类.cbSize = sizeof(WNDCLASSEXW);
        窗口类.style = CS_HREDRAW | CS_VREDRAW;
        窗口类.lpfnWndProc = 窗口过程;
        窗口类.hInstance = GetModuleHandleW(nullptr);
        窗口类.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
        窗口类.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        窗口类.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        窗口类.lpszClassName = 窗口类名称;
        窗口类.hIconSm = LoadIconW(nullptr, IDI_APPLICATION);

        if (RegisterClassExW(&窗口类) == 0) {
            if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
                标记追根因错误(L"控制面板窗口类注册失败");
                return false;
            }
        } else {
            窗口类由本次注册 = true;
        }
        return true;
    }

    bool 创建主窗口() {
        主窗口 = CreateWindowExW(
            0,
            窗口类名称,
            窗口标题,
            WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            1120,
            720,
            nullptr,
            nullptr,
            GetModuleHandleW(nullptr),
            this);
        if (主窗口 == nullptr) {
            if (!追根因错误) {
                标记追根因错误(L"控制面板主窗口创建失败");
            }
            return false;
        }
        return true;
    }

    bool 创建菜单栏() {
        菜单栏 = CreateMenu();
        分页菜单 = CreatePopupMenu();
        树菜单 = CreatePopupMenu();
        HMENU 操作菜单 = CreatePopupMenu();
        if (菜单栏 == nullptr || 分页菜单 == nullptr || 树菜单 == nullptr || 操作菜单 == nullptr) {
            标记追根因错误(L"控制面板菜单栏创建失败");
            return false;
        }

        AppendMenuW(分页菜单, MF_STRING, 分页树菜单编号, L"树页(&T)\tCtrl+1");
        AppendMenuW(分页菜单, MF_STRING, 分页系统信息菜单编号, L"系统信息页(&S)\tCtrl+2");
        AppendMenuW(分页菜单, MF_STRING, 分页日志菜单编号, L"日志页(&L)\tCtrl+3");
        AppendMenuW(分页菜单, MF_SEPARATOR, 0, nullptr);
        AppendMenuW(分页菜单, MF_STRING, 上一分页菜单编号, L"上一页\tCtrl+Shift+Tab");
        AppendMenuW(分页菜单, MF_STRING, 下一分页菜单编号, L"下一页\tCtrl+Tab");

        for (std::uint32_t 索引 = 0; 索引 < 树分类名称.size(); ++索引) {
            std::wstring 菜单文本 = 树分类名称[索引];
            菜单文本 += L"\tAlt+";
            菜单文本 += std::to_wstring(索引 + 1);
            AppendMenuW(树菜单, MF_STRING, 树分类菜单编号起点 + 索引, 菜单文本.c_str());
        }

        AppendMenuW(操作菜单, MF_STRING, 刷新按钮编号, L"刷新(&R)\tF5");
        AppendMenuW(操作菜单, MF_STRING, 操作请求按钮编号, L"生成操作请求(&G)");
        AppendMenuW(操作菜单, MF_SEPARATOR, 0, nullptr);
        AppendMenuW(操作菜单, MF_STRING, 关闭按钮编号, L"关闭(&C)");

        AppendMenuW(菜单栏, MF_POPUP, reinterpret_cast<UINT_PTR>(分页菜单), L"分页(&P)");
        AppendMenuW(菜单栏, MF_POPUP, reinterpret_cast<UINT_PTR>(树菜单), L"树(&T)");
        AppendMenuW(菜单栏, MF_POPUP, reinterpret_cast<UINT_PTR>(操作菜单), L"操作(&O)");
        if (!SetMenu(主窗口, 菜单栏)) {
            标记追根因错误(L"控制面板菜单栏挂接失败");
            DestroyMenu(菜单栏);
            菜单栏 = nullptr;
            分页菜单 = nullptr;
            树菜单 = nullptr;
            return false;
        }
        更新菜单状态();
        DrawMenuBar(主窗口);
        return true;
    }

    bool 创建快捷键表() {
        const std::array<ACCEL, 12> 快捷键组{{
            {static_cast<BYTE>(FVIRTKEY | FCONTROL), static_cast<WORD>('1'), static_cast<WORD>(分页树菜单编号)},
            {static_cast<BYTE>(FVIRTKEY | FCONTROL), static_cast<WORD>('2'), static_cast<WORD>(分页系统信息菜单编号)},
            {static_cast<BYTE>(FVIRTKEY | FCONTROL), static_cast<WORD>('3'), static_cast<WORD>(分页日志菜单编号)},
            {static_cast<BYTE>(FVIRTKEY | FCONTROL | FSHIFT), static_cast<WORD>(VK_TAB), static_cast<WORD>(上一分页菜单编号)},
            {static_cast<BYTE>(FVIRTKEY | FCONTROL), static_cast<WORD>(VK_TAB), static_cast<WORD>(下一分页菜单编号)},
            {static_cast<BYTE>(FVIRTKEY), static_cast<WORD>(VK_F5), static_cast<WORD>(刷新按钮编号)},
            {static_cast<BYTE>(FVIRTKEY | FALT), static_cast<WORD>('1'), static_cast<WORD>(树分类菜单编号起点 + 0)},
            {static_cast<BYTE>(FVIRTKEY | FALT), static_cast<WORD>('2'), static_cast<WORD>(树分类菜单编号起点 + 1)},
            {static_cast<BYTE>(FVIRTKEY | FALT), static_cast<WORD>('3'), static_cast<WORD>(树分类菜单编号起点 + 2)},
            {static_cast<BYTE>(FVIRTKEY | FALT), static_cast<WORD>('4'), static_cast<WORD>(树分类菜单编号起点 + 3)},
            {static_cast<BYTE>(FVIRTKEY | FALT), static_cast<WORD>('5'), static_cast<WORD>(树分类菜单编号起点 + 4)},
            {static_cast<BYTE>(FVIRTKEY | FALT), static_cast<WORD>('6'), static_cast<WORD>(树分类菜单编号起点 + 5)}
        }};
        快捷键表 = CreateAcceleratorTableW(const_cast<ACCEL*>(快捷键组.data()), static_cast<int>(快捷键组.size()));
        if (快捷键表 == nullptr) {
            标记追根因错误(L"控制面板快捷键表创建失败");
            return false;
        }
        return true;
    }

    LPARAM 追加树项引用(控制面板树项引用 引用) {
        树项引用组.push_back(引用);
        return static_cast<LPARAM>(树项引用组.size());
    }

    const 控制面板树项引用* 读取树项引用(LPARAM 引用序号) const {
        if (引用序号 <= 0) {
            return nullptr;
        }
        const auto 索引 = static_cast<std::size_t>(引用序号 - 1);
        if (索引 >= 树项引用组.size()) {
            return nullptr;
        }
        return &树项引用组[索引];
    }

    const 控制面板树项引用* 读取当前树项引用() const {
        return 读取树项引用(static_cast<LPARAM>(当前树项引用序号));
    }

    HTREEITEM 插入控制面板树节点(HTREEITEM 父项, const 控制面板树节点材料& 节点材料) {
        TVINSERTSTRUCTW 插入材料{};
        插入材料.hParent = 父项;
        插入材料.hInsertAfter = TVI_LAST;
        插入材料.item.mask = TVIF_TEXT | TVIF_PARAM;
        插入材料.item.pszText = const_cast<wchar_t*>(节点材料.显示名称.c_str());
        插入材料.item.lParam = 追加树项引用({当前分类索引, &节点材料, true});
        const auto 新项 = TreeView_InsertItem(树视图, &插入材料);
        if (新项 == nullptr) {
            标记追根因错误(L"控制面板树节点创建失败");
            return nullptr;
        }
        for (const auto& 子节点 : 节点材料.子节点组) {
            if (插入控制面板树节点(新项, 子节点) == nullptr) {
                return nullptr;
            }
        }
        TreeView_Expand(树视图, 新项, TVE_EXPAND);
        return 新项;
    }

    HTREEITEM 插入分类树根(std::uint32_t 分类索引) {
        if (分类索引 >= 树分类名称.size()) {
            标记追根因错误(L"控制面板树分类索引越界");
            return nullptr;
        }
        TVINSERTSTRUCTW 插入材料{};
        插入材料.hParent = TVI_ROOT;
        插入材料.hInsertAfter = TVI_LAST;
        插入材料.item.mask = TVIF_TEXT | TVIF_PARAM;
        插入材料.item.pszText = const_cast<wchar_t*>(树分类名称[分类索引]);
        插入材料.item.lParam = 追加树项引用({分类索引, nullptr, false});
        const auto 新项 = TreeView_InsertItem(树视图, &插入材料);
        if (新项 == nullptr) {
            标记追根因错误(L"控制面板分类树根创建失败");
            return nullptr;
        }
        return 新项;
    }

    bool 重建当前树视图() {
        if (树视图 == nullptr || 当前分类索引 >= 树分类名称.size()) {
            标记追根因错误(L"控制面板树视图重建入口无效");
            return false;
        }
        TreeView_DeleteAllItems(树视图);
        树项引用组.clear();
        当前树项引用序号 = 0;

        HTREEITEM 根项 = nullptr;
        if (当前分类索引 == 0) {
            根项 = 插入控制面板树节点(TVI_ROOT, 当前快照.世界树结构材料.根节点);
        } else if (当前分类索引 == 2) {
            根项 = 插入控制面板树节点(TVI_ROOT, 当前快照.语素树结构材料.根节点);
        } else {
            根项 = 插入分类树根(当前分类索引);
        }
        if (根项 == nullptr || 树项引用组.empty()) {
            return false;
        }
        当前树项引用序号 = 1;
        TreeView_SelectItem(树视图, 根项);
        TreeView_Expand(树视图, 根项, TVE_EXPAND);
        return true;
    }

    void 更新菜单状态() const {
        if (分页菜单 != nullptr) {
            CheckMenuRadioItem(
                分页菜单,
                分页树菜单编号,
                分页日志菜单编号,
                分页菜单编号(当前分页),
                MF_BYCOMMAND);
        }
        if (树菜单 != nullptr) {
            CheckMenuRadioItem(
                树菜单,
                树分类菜单编号起点,
                树分类菜单编号起点 + static_cast<int>(树分类名称.size()) - 1,
                树分类菜单编号起点 + static_cast<int>(当前分类索引),
                MF_BYCOMMAND);
        }
    }

    void 应用分页控件可见性() const {
        const bool 树页 = 当前分页 == 控制面板分页::树;
        const bool 系统信息页 = 当前分页 == 控制面板分页::系统信息;
        const bool 日志页 = 当前分页 == 控制面板分页::日志;
        显示控件(导航栏, 树页);
        显示控件(树视图, 树页);
        显示控件(节点数文本, 树页 || 系统信息页);
        显示控件(关系数文本, 树页 || 系统信息页);
        显示控件(索引数文本, 树页 || 系统信息页);
        显示控件(详情文本, true);
        显示控件(数据库标题文本, 系统信息页 || 日志页);
        显示控件(数据库列表, 系统信息页);
        (void)日志页;
    }

    void 更新系统信息页() const {
        std::wostringstream 文本;
        文本 << L"系统信息"
            << L"\r\n\r\n"
            << L"控制面板模式：只读显示"
            << L"\r\n"
            << L"权威运行期事实来源：内存节点 / 主信息 / 关系 / 索引仓库"
            << L"\r\n"
            << L"显示层边界：不得写业务事实，不得用文本裁决机器事实"
            << L"\r\n"
            << L"数据库审计投影：只作人读审计材料，不裁决运行期事实"
            << L"\r\n\r\n"
            << L"启动结构统计"
            << L"\r\n"
            << L"节点数：" << 当前快照.启动结构统计.节点数
            << L"\r\n"
            << L"关系数：" << 当前快照.启动结构统计.关系数
            << L"\r\n"
            << L"索引数：" << 当前快照.启动结构统计.索引数
            << L"\r\n\r\n"
            << L"树分页分类"
            << L"\r\n";
        for (std::uint32_t 索引 = 0; 索引 < 树分类名称.size(); ++索引) {
            const auto& 树材料 = 当前快照.树视图材料组[索引];
            文本 << L"- " << 树分类名称[索引]
                << L"："
                << (树材料.可作为树视图分类 ? L"可展示" : L"拒绝")
                << L"\r\n";
        }
        文本 << L"\r\n"
            << L"实际树结构"
            << L"\r\n"
            << L"世界树节点数：" << 计算树节点数量(当前快照.世界树结构材料.根节点)
            << L"\r\n"
            << L"语素树节点数：" << 计算树节点数量(当前快照.语素树结构材料.根节点)
            << L"\r\n";
        SetWindowTextW(详情文本, 文本.str().c_str());
    }

    void 更新日志页() const {
        SetWindowTextW(数据库标题文本, L"日志 | 人读诊断材料，不裁决机器事实");
        std::wostringstream 文本;
        文本 << L"日志页"
            << L"\r\n\r\n"
            << L"日志目录：" << 日志目录路径().wstring()
            << L"\r\n"
            << L"用途边界：运行观察、逻辑错误诊断、事件审计辅助和调试定位。"
            << L"\r\n"
            << L"禁止边界：日志不得承载机器事实，不得修复结构，不得恢复运行期事实。"
            << L"\r\n\r\n";
        追加日志文件行(文本, 日志类别::运行);
        追加日志文件行(文本, 日志类别::逻辑错误);
        追加日志文件行(文本, 日志类别::事件);
        文本 << L"调试："
            << (日志目录路径() / L"调试_<切片>.log").wstring()
            << L"\r\n"
            << L"状态：按调试切片宏启用，默认不作为常驻显示依据。";
        SetWindowTextW(详情文本, 文本.str().c_str());
    }

    void 更新当前分页内容() const {
        switch (当前分页) {
        case 控制面板分页::树:
            更新详情控件();
            break;
        case 控制面板分页::系统信息:
            更新系统信息页();
            更新数据库控件();
            break;
        case 控制面板分页::日志:
            更新日志页();
            break;
        }
    }

    bool 切换分页(控制面板分页 新分页) {
        if (分页索引(新分页) >= 分页名称.size()) {
            标记追根因错误(L"控制面板分页索引越界");
            return false;
        }
        当前分页 = 新分页;
        更新菜单状态();
        应用分页控件可见性();
        更新当前分页内容();
        布局控件();
        std::wstring 状态 = L"已切换到";
        状态 += 分页名称[分页索引(当前分页)];
        状态 += L"分页";
        设置状态栏文本(状态.c_str());
        return true;
    }

    bool 切换相邻分页(int 偏移) {
        const int 分页总数 = static_cast<int>(分页名称.size());
        const int 当前索引 = static_cast<int>(分页索引(当前分页));
        const int 新索引 = (当前索引 + 偏移 + 分页总数) % 分页总数;
        return 切换分页(从分页索引读取分页(static_cast<std::uint32_t>(新索引)));
    }

    bool 切换树分类(std::uint32_t 新分类索引) {
        if (新分类索引 >= 树分类名称.size()) {
            标记追根因错误(L"控制面板导航栏分类索引越界");
            return false;
        }
        当前分类索引 = 新分类索引;
        if (导航栏 != nullptr) {
            SendMessageW(导航栏, LB_SETCURSEL, static_cast<WPARAM>(当前分类索引), 0);
        }
        if (!重建当前树视图()) {
            return false;
        }
        当前分页 = 控制面板分页::树;
        更新菜单状态();
        应用分页控件可见性();
        更新详情控件();
        布局控件();
        return true;
    }

    bool 创建控件() {
        标题字体 = CreateFontW(
            -28, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        正文字体 = CreateFontW(
            -18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        if (标题字体 == nullptr || 正文字体 == nullptr) {
            标记追根因错误(L"控制面板字体创建失败");
            return false;
        }

        标题文本 = CreateWindowExW(0, L"STATIC", L"海中鱼巣控制面板",
            WS_CHILD | WS_VISIBLE | SS_LEFT, 0, 0, 0, 0, 主窗口, nullptr, GetModuleHandleW(nullptr), nullptr);
        边界文本 = CreateWindowExW(0, L"STATIC", L"只读模式 | 内存事实：显示服务 | 数据库：审计投影，不裁决事实",
            WS_CHILD | WS_VISIBLE | SS_LEFT, 0, 0, 0, 0, 主窗口, nullptr, GetModuleHandleW(nullptr), nullptr);
        导航栏 = CreateWindowExW(
            WS_EX_CLIENTEDGE, L"LISTBOX", L"",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT,
            0, 0, 0, 0, 主窗口,
            reinterpret_cast<HMENU>(static_cast<INT_PTR>(导航栏编号)), GetModuleHandleW(nullptr), nullptr);
        树视图 = CreateWindowExW(
            WS_EX_CLIENTEDGE, WC_TREEVIEWW, L"",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS,
            0, 0, 0, 0, 主窗口, nullptr, GetModuleHandleW(nullptr), nullptr);
        节点数文本 = CreateWindowExW(WS_EX_CLIENTEDGE, L"STATIC", L"",
            WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 0, 0, 0, 主窗口, nullptr, GetModuleHandleW(nullptr), nullptr);
        关系数文本 = CreateWindowExW(WS_EX_CLIENTEDGE, L"STATIC", L"",
            WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 0, 0, 0, 主窗口, nullptr, GetModuleHandleW(nullptr), nullptr);
        索引数文本 = CreateWindowExW(WS_EX_CLIENTEDGE, L"STATIC", L"",
            WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 0, 0, 0, 主窗口, nullptr, GetModuleHandleW(nullptr), nullptr);
        详情文本 = CreateWindowExW(
            WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
            0, 0, 0, 0, 主窗口, nullptr, GetModuleHandleW(nullptr), nullptr);
        数据库标题文本 = CreateWindowExW(0, L"STATIC", L"数据库审计投影",
            WS_CHILD | WS_VISIBLE | SS_LEFT, 0, 0, 0, 0, 主窗口, nullptr, GetModuleHandleW(nullptr), nullptr);
        数据库列表 = CreateWindowExW(
            WS_EX_CLIENTEDGE, WC_LISTVIEWW, L"",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS,
            0, 0, 0, 0, 主窗口, nullptr, GetModuleHandleW(nullptr), nullptr);
        刷新按钮 = CreateWindowExW(0, L"BUTTON", L"刷新",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
            0, 0, 0, 0, 主窗口,
            reinterpret_cast<HMENU>(static_cast<INT_PTR>(刷新按钮编号)), GetModuleHandleW(nullptr), nullptr);
        操作请求按钮 = CreateWindowExW(0, L"BUTTON", L"生成操作请求",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
            0, 0, 0, 0, 主窗口,
            reinterpret_cast<HMENU>(static_cast<INT_PTR>(操作请求按钮编号)), GetModuleHandleW(nullptr), nullptr);
        关闭按钮 = CreateWindowExW(0, L"BUTTON", L"关闭",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
            0, 0, 0, 0, 主窗口,
            reinterpret_cast<HMENU>(static_cast<INT_PTR>(关闭按钮编号)), GetModuleHandleW(nullptr), nullptr);
        状态栏 = CreateWindowExW(
            0, STATUSCLASSNAMEW, L"控制面板已就绪",
            WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
            0, 0, 0, 0, 主窗口, nullptr, GetModuleHandleW(nullptr), nullptr);

        if (标题文本 == nullptr || 边界文本 == nullptr || 导航栏 == nullptr || 树视图 == nullptr
            || 节点数文本 == nullptr || 关系数文本 == nullptr || 索引数文本 == nullptr
            || 详情文本 == nullptr || 数据库标题文本 == nullptr || 数据库列表 == nullptr
            || 刷新按钮 == nullptr || 操作请求按钮 == nullptr
            || 关闭按钮 == nullptr || 状态栏 == nullptr) {
            标记追根因错误(L"控制面板必需控件创建失败");
            return false;
        }

        SendMessageW(标题文本, WM_SETFONT, reinterpret_cast<WPARAM>(标题字体), TRUE);
        for (HWND 控件 : {边界文本, 导航栏, 树视图, 节点数文本, 关系数文本, 索引数文本,
                 详情文本, 数据库标题文本, 数据库列表, 刷新按钮, 操作请求按钮, 关闭按钮, 状态栏}) {
            SendMessageW(控件, WM_SETFONT, reinterpret_cast<WPARAM>(正文字体), TRUE);
        }

        for (std::uint32_t 索引 = 0; 索引 < 树分类名称.size(); ++索引) {
            if (SendMessageW(导航栏, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(树分类名称[索引])) == LB_ERR) {
                标记追根因错误(L"控制面板导航栏分类创建失败");
                return false;
            }
        }
        SendMessageW(导航栏, LB_SETCURSEL, 0, 0);
        if (!重建当前树视图()) {
            return false;
        }

        ListView_SetExtendedListViewStyle(数据库列表, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
        const std::array<std::pair<const wchar_t*, int>, 6> 数据库列{
            std::pair{L"编号", 72},
            std::pair{L"时间", 170},
            std::pair{L"来源", 160},
            std::pair{L"节点", 80},
            std::pair{L"关系", 80},
            std::pair{L"索引", 80}
        };
        for (std::size_t 索引 = 0; 索引 < 数据库列.size(); ++索引) {
            LVCOLUMNW 列{};
            列.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
            列.pszText = const_cast<wchar_t*>(数据库列[索引].first);
            列.cx = 数据库列[索引].second;
            列.iSubItem = static_cast<int>(索引);
            if (ListView_InsertColumn(数据库列表, static_cast<int>(索引), &列) == -1) {
                标记追根因错误(L"控制面板数据库审计列表列创建失败");
                return false;
            }
        }

        读取数据库审计材料();
        if (!重建当前树视图()) {
            return false;
        }
        更新总览控件();
        更新数据库控件();
        更新当前分页内容();
        更新菜单状态();
        应用分页控件可见性();
        布局控件();
        return true;
    }

    void 更新总览控件() const {
        const auto 节点文本 = std::wstring(L"节点\n") + std::to_wstring(当前快照.启动结构统计.节点数);
        const auto 关系文本 = std::wstring(L"关系\n") + std::to_wstring(当前快照.启动结构统计.关系数);
        const auto 索引文本 = std::wstring(L"索引\n") + std::to_wstring(当前快照.启动结构统计.索引数);
        SetWindowTextW(节点数文本, 节点文本.c_str());
        SetWindowTextW(关系数文本, 关系文本.c_str());
        SetWindowTextW(索引数文本, 索引文本.c_str());
    }

    void 更新详情控件() const {
        const auto& 树材料 = 当前快照.树视图材料组[当前分类索引];
        const auto* 树项 = 读取当前树项引用();
        std::wostringstream 文本;
        文本 << L"当前分类：" << 树分类名称[当前分类索引]
            << L"\r\n\r\n"
            << 树分类说明(树材料.分类)
            << L"\r\n\r\n"
            << L"树视图分类资格：" << (树材料.可作为树视图分类 ? L"可展示" : L"拒绝")
            << L"\r\n"
            << L"材料来源：控制面板服务 / 显示服务"
            << L"\r\n"
            << L"机器事实写入：禁止"
            << L"\r\n"
            << L"状态与动态：只作为详情材料"
            << L"\r\n\r\n";
        if (树项 != nullptr && 树项->是结构节点 && 树项->节点材料 != nullptr) {
            const auto& 节点材料 = *树项->节点材料;
            文本 << L"选中结点：" << 节点材料.显示名称
                << L"\r\n"
                << L"结点仓库：" << 节点材料.节点.仓库编号
                << L"\r\n"
                << L"结点编号：" << 节点材料.节点.节点编号
                << L"\r\n"
                << L"结点版本：" << 节点材料.节点.版本号
                << L"\r\n"
                << L"结点类型枚举：" << static_cast<std::uint32_t>(节点材料.类型)
                << L"\r\n"
                << L"名称基础信息：" << 节点材料.名称基础信息.节点编号
                << L"\r\n"
                << L"类型基础信息：" << 节点材料.类型基础信息.节点编号
                << L"\r\n"
                << L"名称语素入口：" << 节点材料.名称语素入口.节点编号
                << L"\r\n"
                << L"类型语素入口：" << 节点材料.类型语素入口.节点编号
                << L"\r\n"
                << L"子结点数量：" << 节点材料.子节点组.size()
                << L"\r\n\r\n"
                << L"显示文本来源：语素入口与基础信息只读材料。";
        } else if (树材料.分类 == 控制面板树分类::世界树) {
            文本 << L"世界树快照根节点：" << 当前快照.世界树结构材料.根节点.显示名称
                << L"\r\n"
                << L"世界树快照节点数：" << 计算树节点数量(当前快照.世界树结构材料.根节点)
                << L"\r\n"
                << L"结构来源：世界树初始化入口与普通父子关系只读快照";
        } else if (树材料.分类 == 控制面板树分类::语素树) {
            文本 << L"语素树快照根节点：" << 当前快照.语素树结构材料.根节点.显示名称
                << L"\r\n"
                << L"语素树快照节点数：" << 计算树节点数量(当前快照.语素树结构材料.根节点)
                << L"\r\n"
                << L"结构来源：初始化语素入口与语素对应信息 / 概念追溯只读材料";
        } else {
            文本 << L"选中树根：" << 树分类名称[当前分类索引]
                << L"\r\n"
                << L"树形显示：当前树以根结点显示在中间树控件中。"
                << L"\r\n"
                << L"第一轮边界：当前分类只显示树根入口，不生成虚假子结点。真实领域树子结点由后续只读 DTO 专项承接。";
        }
        SetWindowTextW(详情文本, 文本.str().c_str());
    }

    void 更新数据库控件() const {
        ListView_DeleteAllItems(数据库列表);
        const auto& 查询 = 当前快照.数据库审计查询;
        if (!查询.操作.成功) {
            const std::wstring 状态 = L"数据库审计投影不可用 | "
                + (查询.操作.失败阶段.empty() ? std::wstring(L"未知阶段") : 查询.操作.失败阶段);
            SetWindowTextW(数据库标题文本, 状态.c_str());
            return;
        }
        const std::wstring 状态 = L"数据库审计投影 | 最近 " + std::to_wstring(查询.记录组.size())
            + L" 条 | 只读，不裁决运行期事实";
        SetWindowTextW(数据库标题文本, 状态.c_str());
        for (std::size_t 索引 = 0; 索引 < 查询.记录组.size(); ++索引) {
            const auto& 记录 = 查询.记录组[索引];
            std::array<std::wstring, 6> 文本组{
                std::to_wstring(记录.审计编号),
                记录.记录时间,
                记录.来源入口,
                std::to_wstring(记录.节点数),
                std::to_wstring(记录.关系数),
                std::to_wstring(记录.索引数)
            };
            LVITEMW 项{};
            项.mask = LVIF_TEXT;
            项.iItem = static_cast<int>(索引);
            项.pszText = 文本组[0].data();
            const int 项编号 = ListView_InsertItem(数据库列表, &项);
            if (项编号 < 0) {
                continue;
            }
            for (int 列号 = 1; 列号 < static_cast<int>(文本组.size()); ++列号) {
                ListView_SetItemText(数据库列表, 项编号, 列号, 文本组[列号].data());
            }
        }
    }

    void 布局控件() const {
        if (主窗口 == nullptr || 状态栏 == nullptr) {
            return;
        }
        应用分页控件可见性();
        RECT 客户区{};
        GetClientRect(主窗口, &客户区);
        const int 宽度 = 客户区.right - 客户区.left;
        const int 高度 = 客户区.bottom - 客户区.top;
        SendMessageW(状态栏, WM_SIZE, 0, 0);

        RECT 状态栏矩形{};
        GetWindowRect(状态栏, &状态栏矩形);
        const int 状态栏高度 = std::max(24L, 状态栏矩形.bottom - 状态栏矩形.top);
        const int 边距 = 16;
        const int 标题高度 = 58;
        const int 按钮区高度 = 52;
        const int 内容顶部 = 边距 + 标题高度;
        const int 内容底部 = 高度 - 状态栏高度 - 按钮区高度;
        const int 内容宽度 = std::max(260, 宽度 - 2 * 边距);
        const int 统计间距 = 10;
        const int 统计高度 = 58;
        const int 数据库标题高度 = 24;

        MoveWindow(标题文本, 边距, 10, 宽度 - 2 * 边距, 32, TRUE);
        MoveWindow(边界文本, 边距, 42, 宽度 - 2 * 边距, 24, TRUE);

        auto 设置数据库列宽 = [&](int 列表宽度) {
            if (数据库列表 == nullptr) {
                return;
            }
            const int 编号列宽 = 64;
            const int 时间列宽 = 190;
            const int 数量列宽 = 72;
            const int 来源列宽 = std::max(160, 列表宽度 - 编号列宽 - 时间列宽 - 3 * 数量列宽 - 20);
            ListView_SetColumnWidth(数据库列表, 0, 编号列宽);
            ListView_SetColumnWidth(数据库列表, 1, 时间列宽);
            ListView_SetColumnWidth(数据库列表, 2, 来源列宽);
            ListView_SetColumnWidth(数据库列表, 3, 数量列宽);
            ListView_SetColumnWidth(数据库列表, 4, 数量列宽);
            ListView_SetColumnWidth(数据库列表, 5, 数量列宽);
        };

        if (当前分页 == 控制面板分页::树) {
            const int 导航栏宽度 = std::clamp(宽度 / 6, 150, 210);
            const int 右详情宽度 = std::clamp(宽度 / 3, 320, 430);
            const int 导航栏左边 = 边距;
            const int 树左边 = 导航栏左边 + 导航栏宽度 + 边距;
            const int 右栏左边 = 宽度 - 边距 - 右详情宽度;
            const int 树宽度 = std::max(260, 右栏左边 - 树左边 - 边距);
            const int 右栏宽度 = std::max(260, 右详情宽度);
            const int 单统计宽度 = std::max(80, (右栏宽度 - 2 * 统计间距) / 3);
            MoveWindow(导航栏, 导航栏左边, 内容顶部, 导航栏宽度, std::max(120, 内容底部 - 内容顶部), TRUE);
            MoveWindow(树视图, 树左边, 内容顶部, 树宽度, std::max(120, 内容底部 - 内容顶部), TRUE);
            MoveWindow(节点数文本, 右栏左边, 内容顶部, 单统计宽度, 统计高度, TRUE);
            MoveWindow(关系数文本, 右栏左边 + 单统计宽度 + 统计间距, 内容顶部, 单统计宽度, 统计高度, TRUE);
            MoveWindow(索引数文本, 右栏左边 + 2 * (单统计宽度 + 统计间距), 内容顶部,
                右栏宽度 - 2 * (单统计宽度 + 统计间距), 统计高度, TRUE);
            const int 详情顶部 = 内容顶部 + 统计高度 + 12;
            MoveWindow(详情文本, 右栏左边, 详情顶部, 右栏宽度, std::max(180, 内容底部 - 详情顶部), TRUE);
        } else if (当前分页 == 控制面板分页::系统信息) {
            const int 单统计宽度 = std::max(90, (内容宽度 - 2 * 统计间距) / 3);
            MoveWindow(节点数文本, 边距, 内容顶部, 单统计宽度, 统计高度, TRUE);
            MoveWindow(关系数文本, 边距 + 单统计宽度 + 统计间距, 内容顶部, 单统计宽度, 统计高度, TRUE);
            MoveWindow(索引数文本, 边距 + 2 * (单统计宽度 + 统计间距), 内容顶部,
                内容宽度 - 2 * (单统计宽度 + 统计间距), 统计高度, TRUE);
            const int 详情顶部 = 内容顶部 + 统计高度 + 12;
            const int 剩余高度 = std::max(220, 内容底部 - 详情顶部);
            const int 详情高度 = std::max(150, 剩余高度 / 2);
            const int 数据库标题顶部 = 详情顶部 + 详情高度 + 10;
            const int 数据库列表顶部 = 数据库标题顶部 + 数据库标题高度;
            MoveWindow(详情文本, 边距, 详情顶部, 内容宽度, 详情高度, TRUE);
            MoveWindow(数据库标题文本, 边距, 数据库标题顶部, 内容宽度, 数据库标题高度, TRUE);
            MoveWindow(数据库列表, 边距, 数据库列表顶部, 内容宽度, std::max(90, 内容底部 - 数据库列表顶部), TRUE);
            设置数据库列宽(内容宽度);
        } else {
            const int 日志标题顶部 = 内容顶部;
            const int 日志正文顶部 = 日志标题顶部 + 数据库标题高度 + 8;
            MoveWindow(数据库标题文本, 边距, 日志标题顶部, 内容宽度, 数据库标题高度, TRUE);
            MoveWindow(详情文本, 边距, 日志正文顶部, 内容宽度, std::max(180, 内容底部 - 日志正文顶部), TRUE);
        }

        const int 按钮顶部 = 高度 - 状态栏高度 - 42;
        MoveWindow(刷新按钮, 边距, 按钮顶部, 100, 30, TRUE);
        MoveWindow(操作请求按钮, 边距 + 112, 按钮顶部, 148, 30, TRUE);
        MoveWindow(关闭按钮, 宽度 - 边距 - 100, 按钮顶部, 100, 30, TRUE);
    }

    bool 刷新显示材料() {
        if (!读取只读材料()) {
            return false;
        }
        读取数据库审计材料();
        更新总览控件();
        更新当前分页内容();
        设置状态栏文本(当前快照.数据库审计查询.操作.成功
            ? L"只读材料和数据库审计投影已刷新；权威结构未改变"
            : L"内存只读材料已刷新；数据库审计投影不可用；权威结构未改变");
        return true;
    }

    bool 生成操作请求材料() {
        const auto 请求材料 = 控制面板.生成操作请求壳(创建只读请求());
        if (请求材料.状态 != 控制面板请求状态::可展示
            || !请求材料.已生成请求壳
            || 请求材料.已执行动作) {
            标记追根因错误(L"控制面板操作请求壳结果不符合不执行动作边界");
            return false;
        }
        当前快照.操作请求材料 = 请求材料;
        设置状态栏文本(L"操作请求壳已生成；动作未执行");
        return true;
    }

    bool 尝试处理命令编号(WORD 命令编号, bool& 已处理) {
        已处理 = true;
        if (命令编号 >= 树分类菜单编号起点
            && 命令编号 < 树分类菜单编号起点 + static_cast<WORD>(树分类名称.size())) {
            return 切换树分类(static_cast<std::uint32_t>(命令编号 - 树分类菜单编号起点));
        }
        switch (命令编号) {
        case 分页树菜单编号:
            return 切换分页(控制面板分页::树);
        case 分页系统信息菜单编号:
            return 切换分页(控制面板分页::系统信息);
        case 分页日志菜单编号:
            return 切换分页(控制面板分页::日志);
        case 上一分页菜单编号:
            return 切换相邻分页(-1);
        case 下一分页菜单编号:
            return 切换相邻分页(1);
        case 刷新按钮编号:
            return 刷新显示材料();
        case 操作请求按钮编号:
            return 生成操作请求材料();
        case 关闭按钮编号:
            if (主窗口 != nullptr) {
                DestroyWindow(主窗口);
            }
            return true;
        default:
            已处理 = false;
            return true;
        }
    }

    void 设置状态栏文本(const wchar_t* 文本) const {
        if (状态栏 != nullptr) {
            SendMessageW(状态栏, SB_SETTEXTW, 0, reinterpret_cast<LPARAM>(文本));
        }
    }

    void 标记追根因错误(std::wstring 阶段) {
        追根因错误 = true;
        if (失败阶段.empty()) {
            失败阶段 = std::move(阶段);
        }
    }

    void 销毁字体() {
        if (标题字体 != nullptr) {
            DeleteObject(标题字体);
            标题字体 = nullptr;
        }
        if (正文字体 != nullptr) {
            DeleteObject(正文字体);
            正文字体 = nullptr;
        }
    }

    void 销毁快捷键表() {
        if (快捷键表 != nullptr) {
            DestroyAcceleratorTable(快捷键表);
            快捷键表 = nullptr;
        }
    }

    控制面板窗口运行结果 运行(const volatile std::sig_atomic_t* 外部停止请求) {
        停止请求 = 外部停止请求;
        if (!读取只读材料() || !注册窗口类() || !创建主窗口() || !创建菜单栏() || !创建快捷键表()) {
            if (主窗口 != nullptr) {
                DestroyWindow(主窗口);
            }
            return {false, false, false, false, true, 失败阶段};
        }

        ShowWindow(主窗口, SW_SHOW);
        UpdateWindow(主窗口);
        已显示窗口 = true;
        if (SetTimer(主窗口, 停止检查计时器编号, 停止检查间隔毫秒, nullptr) == 0) {
            标记追根因错误(L"控制面板停止信号计时器创建失败");
            DestroyWindow(主窗口);
            return {false, true, false, false, true, 失败阶段};
        }

        MSG 消息{};
        while (true) {
            const int 消息结果 = static_cast<int>(GetMessageW(&消息, nullptr, 0, 0));
            if (消息结果 > 0) {
                if (快捷键表 != nullptr && 主窗口 != nullptr
                    && TranslateAcceleratorW(主窗口, 快捷键表, &消息) != 0) {
                    continue;
                }
                TranslateMessage(&消息);
                DispatchMessageW(&消息);
                continue;
            }
            if (消息结果 < 0) {
                标记追根因错误(L"控制面板消息循环读取失败");
            }
            break;
        }

        销毁字体();
        销毁快捷键表();
        if (追根因错误) {
            return {false, 已显示窗口, false, false, true, 失败阶段};
        }
        return {true, 已显示窗口, true, true, false, {}};
    }

    static LRESULT CALLBACK 窗口过程(HWND 窗口, UINT 消息, WPARAM 参数一, LPARAM 参数二) {
        auto* 当前实现 = reinterpret_cast<窗口实现*>(GetWindowLongPtrW(窗口, GWLP_USERDATA));
        if (消息 == WM_NCCREATE) {
            const auto* 创建材料 = reinterpret_cast<CREATESTRUCTW*>(参数二);
            当前实现 = static_cast<窗口实现*>(创建材料->lpCreateParams);
            SetWindowLongPtrW(窗口, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(当前实现));
            当前实现->主窗口 = 窗口;
        }
        if (当前实现 == nullptr) {
            return DefWindowProcW(窗口, 消息, 参数一, 参数二);
        }

        switch (消息) {
        case WM_CREATE:
            return 当前实现->创建控件() ? 0 : -1;
        case WM_GETMINMAXINFO: {
            auto* 尺寸材料 = reinterpret_cast<MINMAXINFO*>(参数二);
            尺寸材料->ptMinTrackSize.x = 980;
            尺寸材料->ptMinTrackSize.y = 600;
            return 0;
        }
        case WM_SIZE:
            当前实现->布局控件();
            return 0;
        case WM_NOTIFY: {
            const auto* 通知 = reinterpret_cast<NMHDR*>(参数二);
            if (通知 != nullptr && 通知->hwndFrom == 当前实现->树视图 && 通知->code == TVN_SELCHANGEDW) {
                const auto* 树通知 = reinterpret_cast<NMTREEVIEWW*>(参数二);
                const auto* 树项 = 当前实现->读取树项引用(树通知->itemNew.lParam);
                if (树项 != nullptr && 树项->分类索引 < 当前实现->当前快照.树视图材料组.size()) {
                    当前实现->当前分类索引 = 树项->分类索引;
                    当前实现->当前树项引用序号 = static_cast<std::size_t>(树通知->itemNew.lParam);
                    if (当前实现->导航栏 != nullptr) {
                        SendMessageW(当前实现->导航栏, LB_SETCURSEL, static_cast<WPARAM>(当前实现->当前分类索引), 0);
                    }
                    当前实现->更新详情控件();
                    当前实现->设置状态栏文本(树项->是结构节点 ? L"已选择只读树结点" : L"已选择只读树根");
                }
            }
            return 0;
        }
        case WM_COMMAND:
            if (reinterpret_cast<HWND>(参数二) == 当前实现->导航栏 && HIWORD(参数一) == LBN_SELCHANGE) {
                const auto 选中项 = SendMessageW(当前实现->导航栏, LB_GETCURSEL, 0, 0);
                if (选中项 != LB_ERR) {
                    if (!当前实现->切换树分类(static_cast<std::uint32_t>(选中项))) {
                        DestroyWindow(窗口);
                    } else {
                        当前实现->设置状态栏文本(L"已切换导航树");
                    }
                }
                return 0;
            }
            {
                bool 已处理 = false;
                if (!当前实现->尝试处理命令编号(LOWORD(参数一), 已处理)) {
                    DestroyWindow(窗口);
                    return 0;
                }
                if (已处理) {
                    return 0;
                }
            }
            switch (LOWORD(参数一)) {
            case 关闭按钮编号:
                DestroyWindow(窗口);
                return 0;
            default:
                break;
            }
            break;
        case WM_TIMER:
            if (参数一 == 停止检查计时器编号
                && 当前实现->停止请求 != nullptr
                && *当前实现->停止请求 != 0) {
                DestroyWindow(窗口);
            }
            return 0;
        case WM_CLOSE:
            DestroyWindow(窗口);
            return 0;
        case WM_DESTROY:
            KillTimer(窗口, 停止检查计时器编号);
            当前实现->主窗口 = nullptr;
            当前实现->菜单栏 = nullptr;
            当前实现->分页菜单 = nullptr;
            当前实现->树菜单 = nullptr;
            PostQuitMessage(0);
            return 0;
        default:
            break;
        }
        return DefWindowProcW(窗口, 消息, 参数一, 参数二);
    }
};

控制面板窗口::控制面板窗口(const 控制面板服务& 控制面板, const SQL数据库适配& 数据库,
    结构统计快照 启动结构统计, 控制面板树结构材料 世界树结构材料,
    控制面板树结构材料 语素树结构材料)
    : 实现(std::make_unique<窗口实现>(
        控制面板, 数据库, 启动结构统计, std::move(世界树结构材料), std::move(语素树结构材料))) {
}

控制面板窗口::~控制面板窗口() = default;

控制面板窗口运行结果 控制面板窗口::运行(const volatile std::sig_atomic_t* 停止请求) {
    return 实现->运行(停止请求);
}

}
