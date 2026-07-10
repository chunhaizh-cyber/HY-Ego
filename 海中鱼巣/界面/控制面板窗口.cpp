// 文件规则：原生窗口只读取控制面板服务材料；窗口消息、控件文本和按钮状态不得成为机器事实或动作来源。
#include "控制面板窗口.h"

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
#include <sstream>
#include <string>
#include <utility>

#pragma comment(lib, "Comctl32.lib")

namespace 海中鱼巣 {

namespace {

constexpr int 刷新按钮编号 = 1001;
constexpr int 操作请求按钮编号 = 1002;
constexpr int 关闭按钮编号 = 1003;
constexpr UINT_PTR 停止检查计时器编号 = 1;
constexpr UINT 停止检查间隔毫秒 = 100;
constexpr wchar_t 窗口类名称[] = L"海中鱼巣控制面板窗口类";
constexpr wchar_t 窗口标题[] = L"海中鱼巣控制面板";

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
        return L"世界结构的只读导航入口。第一轮只显示分类根，不生成虚假子节点。";
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

}

struct 控制面板窗口::窗口实现 {
    const 控制面板服务& 控制面板;
    const SQL数据库适配& 数据库;
    控制面板窗口快照 当前快照;
    const volatile std::sig_atomic_t* 停止请求 = nullptr;

    HWND 主窗口 = nullptr;
    HWND 标题文本 = nullptr;
    HWND 边界文本 = nullptr;
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
    HFONT 正文字体 = nullptr;
    HFONT 标题字体 = nullptr;

    std::uint32_t 当前分类索引 = 0;
    bool 已显示窗口 = false;
    bool 追根因错误 = false;
    bool 窗口类由本次注册 = false;
    std::wstring 失败阶段;

    窗口实现(const 控制面板服务& 面板服务, const SQL数据库适配& 数据库适配,
        结构统计快照 启动结构统计)
        : 控制面板(面板服务), 数据库(数据库适配) {
        当前快照.启动结构统计 = 启动结构统计;
    }

    ~窗口实现() {
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
        if (!总览有效 || !树材料有效 || !统计快照有效) {
            标记追根因错误(L"控制面板只读材料或启动结构统计不符合已确认边界");
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

        if (标题文本 == nullptr || 边界文本 == nullptr || 树视图 == nullptr
            || 节点数文本 == nullptr || 关系数文本 == nullptr || 索引数文本 == nullptr
            || 详情文本 == nullptr || 数据库标题文本 == nullptr || 数据库列表 == nullptr
            || 刷新按钮 == nullptr || 操作请求按钮 == nullptr
            || 关闭按钮 == nullptr || 状态栏 == nullptr) {
            标记追根因错误(L"控制面板必需控件创建失败");
            return false;
        }

        SendMessageW(标题文本, WM_SETFONT, reinterpret_cast<WPARAM>(标题字体), TRUE);
        for (HWND 控件 : {边界文本, 树视图, 节点数文本, 关系数文本, 索引数文本,
                 详情文本, 数据库标题文本, 数据库列表, 刷新按钮, 操作请求按钮, 关闭按钮, 状态栏}) {
            SendMessageW(控件, WM_SETFONT, reinterpret_cast<WPARAM>(正文字体), TRUE);
        }

        HTREEITEM 首项 = nullptr;
        for (std::uint32_t 索引 = 0; 索引 < 树分类名称.size(); ++索引) {
            TVINSERTSTRUCTW 插入材料{};
            插入材料.hParent = TVI_ROOT;
            插入材料.hInsertAfter = TVI_LAST;
            插入材料.item.mask = TVIF_TEXT | TVIF_PARAM;
            插入材料.item.pszText = const_cast<wchar_t*>(树分类名称[索引]);
            插入材料.item.lParam = static_cast<LPARAM>(索引);
            const auto 新项 = TreeView_InsertItem(树视图, &插入材料);
            if (新项 == nullptr) {
                标记追根因错误(L"控制面板六类树根创建失败");
                return false;
            }
            if (首项 == nullptr) {
                首项 = 新项;
            }
        }
        TreeView_SelectItem(树视图, 首项);

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
        更新总览控件();
        更新详情控件();
        更新数据库控件();
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
            << L"\r\n\r\n"
            << L"第一轮边界：当前只显示六类树根和启动结构统计。真实领域树子节点由后续只读 DTO 专项承接。";
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
        const int 左栏宽度 = std::clamp(宽度 / 4, 220, 300);
        const int 内容顶部 = 边距 + 标题高度;
        const int 内容底部 = 高度 - 状态栏高度 - 按钮区高度;
        const int 右栏左边 = 边距 + 左栏宽度 + 边距;
        const int 右栏宽度 = std::max(200, 宽度 - 右栏左边 - 边距);
        const int 统计间距 = 10;
        const int 单统计宽度 = std::max(80, (右栏宽度 - 2 * 统计间距) / 3);
        const int 统计高度 = 58;
        const int 数据库标题高度 = 24;

        MoveWindow(标题文本, 边距, 10, 宽度 - 2 * 边距, 32, TRUE);
        MoveWindow(边界文本, 边距, 42, 宽度 - 2 * 边距, 24, TRUE);
        MoveWindow(树视图, 边距, 内容顶部, 左栏宽度, std::max(120, 内容底部 - 内容顶部), TRUE);
        MoveWindow(节点数文本, 右栏左边, 内容顶部, 单统计宽度, 统计高度, TRUE);
        MoveWindow(关系数文本, 右栏左边 + 单统计宽度 + 统计间距, 内容顶部, 单统计宽度, 统计高度, TRUE);
        MoveWindow(索引数文本, 右栏左边 + 2 * (单统计宽度 + 统计间距), 内容顶部,
            右栏宽度 - 2 * (单统计宽度 + 统计间距), 统计高度, TRUE);
        const int 右栏内容顶部 = 内容顶部 + 统计高度 + 12;
        const int 右栏内容高度 = std::max(200, 内容底部 - 右栏内容顶部);
        const int 详情高度 = std::max(100, 右栏内容高度 / 2 - 12);
        const int 数据库标题顶部 = 右栏内容顶部 + 详情高度 + 8;
        const int 数据库列表顶部 = 数据库标题顶部 + 数据库标题高度;
        MoveWindow(详情文本, 右栏左边, 右栏内容顶部, 右栏宽度, 详情高度, TRUE);
        MoveWindow(数据库标题文本, 右栏左边, 数据库标题顶部, 右栏宽度, 数据库标题高度, TRUE);
        MoveWindow(数据库列表, 右栏左边, 数据库列表顶部, 右栏宽度,
            std::max(80, 内容底部 - 数据库列表顶部), TRUE);
        if (数据库列表 != nullptr) {
            const int 编号列宽 = 64;
            const int 时间列宽 = 190;
            const int 数量列宽 = 72;
            const int 来源列宽 = std::max(160, 右栏宽度 - 编号列宽 - 时间列宽 - 3 * 数量列宽 - 20);
            ListView_SetColumnWidth(数据库列表, 0, 编号列宽);
            ListView_SetColumnWidth(数据库列表, 1, 时间列宽);
            ListView_SetColumnWidth(数据库列表, 2, 来源列宽);
            ListView_SetColumnWidth(数据库列表, 3, 数量列宽);
            ListView_SetColumnWidth(数据库列表, 4, 数量列宽);
            ListView_SetColumnWidth(数据库列表, 5, 数量列宽);
        }

        const int 按钮顶部 = 高度 - 状态栏高度 - 42;
        MoveWindow(刷新按钮, 右栏左边, 按钮顶部, 100, 30, TRUE);
        MoveWindow(操作请求按钮, 右栏左边 + 112, 按钮顶部, 148, 30, TRUE);
        MoveWindow(关闭按钮, 宽度 - 边距 - 100, 按钮顶部, 100, 30, TRUE);
    }

    bool 刷新显示材料() {
        if (!读取只读材料()) {
            return false;
        }
        读取数据库审计材料();
        更新总览控件();
        更新详情控件();
        更新数据库控件();
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

    控制面板窗口运行结果 运行(const volatile std::sig_atomic_t* 外部停止请求) {
        停止请求 = 外部停止请求;
        if (!读取只读材料() || !注册窗口类() || !创建主窗口()) {
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
            尺寸材料->ptMinTrackSize.x = 840;
            尺寸材料->ptMinTrackSize.y = 560;
            return 0;
        }
        case WM_SIZE:
            当前实现->布局控件();
            return 0;
        case WM_NOTIFY: {
            const auto* 通知 = reinterpret_cast<NMHDR*>(参数二);
            if (通知 != nullptr && 通知->hwndFrom == 当前实现->树视图 && 通知->code == TVN_SELCHANGEDW) {
                const auto* 树通知 = reinterpret_cast<NMTREEVIEWW*>(参数二);
                const auto 新索引 = static_cast<std::uint32_t>(树通知->itemNew.lParam);
                if (新索引 < 当前实现->当前快照.树视图材料组.size()) {
                    当前实现->当前分类索引 = 新索引;
                    当前实现->更新详情控件();
                    当前实现->设置状态栏文本(L"已切换只读分类");
                }
            }
            return 0;
        }
        case WM_COMMAND:
            switch (LOWORD(参数一)) {
            case 刷新按钮编号:
                if (!当前实现->刷新显示材料()) {
                    DestroyWindow(窗口);
                }
                return 0;
            case 操作请求按钮编号:
                if (!当前实现->生成操作请求材料()) {
                    DestroyWindow(窗口);
                }
                return 0;
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
            PostQuitMessage(0);
            return 0;
        default:
            break;
        }
        return DefWindowProcW(窗口, 消息, 参数一, 参数二);
    }
};

控制面板窗口::控制面板窗口(const 控制面板服务& 控制面板, const SQL数据库适配& 数据库,
    结构统计快照 启动结构统计)
    : 实现(std::make_unique<窗口实现>(控制面板, 数据库, 启动结构统计)) {
}

控制面板窗口::~控制面板窗口() = default;

控制面板窗口运行结果 控制面板窗口::运行(const volatile std::sig_atomic_t* 停止请求) {
    return 实现->运行(停止请求);
}

}
