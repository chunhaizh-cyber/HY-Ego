//
// 鱼巢_外设_相机Dlg.cpp
//

#include "pch.h"
#include <bit>
#include <chrono>
#include <filesystem>
#include <sstream>
#include <string>
#include <variant>
#include "framework.h"
#include "鱼巢_外设_相机.h"
#include "鱼巢_外设_相机Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

import 基础数据类型模块;
import 主信息定义模块;
import 场景模块;
import 世界树环境模块;
import 特征值环境模块;
import 语素环境模块;
import 语言环境模块;
import 特征类型定义模块;
import 相机观测运行时模块;
import 日志模块;

namespace {
CString 宽转CString_(const std::wstring& 文本)
{
    return CString(文本.c_str());
}

std::wstring UTF8转宽_(const std::string& 文本)
{
    if (文本.empty()) return {};

    const int 宽字符数 = ::MultiByteToWideChar(CP_UTF8, 0, 文本.c_str(), -1, nullptr, 0);
    if (宽字符数 <= 0) {
        return std::wstring(文本.begin(), 文本.end());
    }

    std::wstring 结果(static_cast<std::size_t>(宽字符数 - 1), L'\0');
    ::MultiByteToWideChar(CP_UTF8, 0, 文本.c_str(), -1, 结果.data(), 宽字符数);
    return 结果;
}

std::wstring 节点名称或空_(const 基础信息节点类* 节点)
{
    if (!节点 || !节点->主信息) return {};
    const auto* 基础信息 = dynamic_cast<const 基础信息基类*>(节点->主信息);
    if (!基础信息 || !基础信息->名称) return {};
    return UTF8转宽_(基础信息->名称->获取主键());
}

场景节点类* 查找子场景_按名称(基础信息节点类* 父节点, const std::string& 名称)
{
    if (!父节点) return nullptr;

    for (auto* 子场景 : 世界树.获取子场景(父节点, "鱼巢_外设_相机/查找子场景")) {
        if (!子场景 || !子场景->主信息) continue;
        const auto* 主信息 = dynamic_cast<const 场景节点主信息类*>(子场景->主信息);
        if (!主信息 || !主信息->名称) continue;
        if (主信息->名称->获取主键() == 名称) {
            return 子场景;
        }
    }
    return nullptr;
}

bool 取Vec3I64_(const std::optional<特征快照值>& 快照, I64& x, I64& y, I64& z)
{
    if (!快照 || !std::holds_alternative<VecU句柄>(*快照)) return false;
    const auto 句柄 = std::get<VecU句柄>(*快照);
    const auto* 值 = 世界树.值池().取VecU只读指针(句柄);
    if (!值 || 值->size() < 3) return false;

    x = std::bit_cast<std::int64_t>((*值)[0]);
    y = std::bit_cast<std::int64_t>((*值)[1]);
    z = std::bit_cast<std::int64_t>((*值)[2]);
    return true;
}

std::size_t 取VecU长度_(const std::optional<特征快照值>& 快照)
{
    if (!快照 || !std::holds_alternative<VecU句柄>(*快照)) return 0;
    const auto 句柄 = std::get<VecU句柄>(*快照);
    const auto* 值 = 世界树.值池().取VecU只读指针(句柄);
    return 值 ? 值->size() : 0;
}
} // namespace

struct C鱼巢外设相机Dlg::摄像机运行时包装 {
    相机观测运行时类 实现{};

    bool 启动() { return 实现.启动(); }
    void 停止() { 实现.停止(); }
    bool 是否运行中() const { return 实现.是否正在运行(); }
    结构_相机观测运行时快照 快照() const { return 实现.快照(); }
};

C鱼巢外设相机Dlg::C鱼巢外设相机Dlg(CWnd* pParent)
    : CDialogEx(IDD_鱼巢外设相机_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

C鱼巢外设相机Dlg::~C鱼巢外设相机Dlg()
{
    安全停止全部_();
}

void C鱼巢外设相机Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_RUNTIME_STATUS, 运行状态框_);
    DDX_Control(pDX, IDC_EDIT_OBSERVATION, 观测显示框_);
}

BEGIN_MESSAGE_MAP(C鱼巢外设相机Dlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BUTTON_CAMERA_START, &C鱼巢外设相机Dlg::OnBnClicked启动相机)
    ON_BN_CLICKED(IDC_BUTTON_CAMERA_STOP, &C鱼巢外设相机Dlg::OnBnClicked停止相机)
    ON_BN_CLICKED(IDC_BUTTON_DISPLAY_START, &C鱼巢外设相机Dlg::OnBnClicked启动显示)
    ON_BN_CLICKED(IDC_BUTTON_DISPLAY_STOP, &C鱼巢外设相机Dlg::OnBnClicked停止显示)
    ON_BN_CLICKED(IDC_BUTTON_REFRESH, &C鱼巢外设相机Dlg::OnBnClicked刷新)
END_MESSAGE_MAP()

BOOL C鱼巢外设相机Dlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    确保运行环境已初始化_();
    摄像机运行时_ = std::make_unique<摄像机运行时包装>();

    SetTimer(定时器_刷新, 500, nullptr);
    刷新界面_();
    return TRUE;
}

void C鱼巢外设相机Dlg::确保运行环境已初始化_()
{
    if (运行环境已初始化_) return;

    日志参数 参数{};
    参数.根目录 = std::filesystem::path(std::u8string(u8"./日志"));
    参数.文件前缀 = "鱼巢_外设_相机";
    参数.每条刷新 = true;
    日志::初始化(参数);
    日志::运行("[鱼巢_外设_相机] 日志初始化完成");

    特征值集.初始化();
    语素集.初始化();
    特征类型定义类::初始化特征类型定义模块_依赖语素();
    世界树.初始化默认世界();
    语言集.初始化();

    if (!世界树.虚拟世界) {
        const auto* 虚拟世界名称 = 语素集.添加词性词("鱼巢_外设_相机_虚拟世界", "名词");
        世界树.虚拟世界 = 世界树.取或创建子场景_按名称(
            nullptr,
            虚拟世界名称,
            结构体_时间戳::当前_微秒(),
            "鱼巢_外设_相机/初始化虚拟世界");
    }
    if (!世界树.自我所在场景) {
        世界树.自我所在场景 = 世界树.虚拟世界;
    }

    运行环境已初始化_ = true;
    日志::运行("[鱼巢_外设_相机] 运行环境初始化完成");
}

void C鱼巢外设相机Dlg::安全停止全部_()
{
    if (::IsWindow(GetSafeHwnd())) {
        KillTimer(定时器_刷新);
    }

    if (摄像机运行时_) {
        摄像机运行时_->停止();
    }
}

void C鱼巢外设相机Dlg::刷新界面_()
{
    if (::IsWindow(运行状态框_.GetSafeHwnd())) {
        运行状态框_.SetWindowTextW(生成运行状态文本_());
    }
    if (::IsWindow(观测显示框_.GetSafeHwnd())) {
        观测显示框_.SetWindowTextW(生成观测显示文本_());
    }
    刷新按钮状态_();
}

void C鱼巢外设相机Dlg::刷新按钮状态_()
{
    const bool 相机运行中 = 摄像机运行时_ && 摄像机运行时_->是否运行中();

    if (CWnd* 按钮 = GetDlgItem(IDC_BUTTON_CAMERA_START)) {
        按钮->EnableWindow(!相机运行中);
    }
    if (CWnd* 按钮 = GetDlgItem(IDC_BUTTON_CAMERA_STOP)) {
        按钮->EnableWindow(相机运行中);
    }
    if (CWnd* 按钮 = GetDlgItem(IDC_BUTTON_DISPLAY_START)) {
        按钮->EnableWindow(!显示启用_);
    }
    if (CWnd* 按钮 = GetDlgItem(IDC_BUTTON_DISPLAY_STOP)) {
        按钮->EnableWindow(显示启用_);
    }
}

CString C鱼巢外设相机Dlg::生成运行状态文本_() const
{
    std::wostringstream 输出;
    输出 << L"程序：鱼巢_外设_相机\r\n";
    输出 << L"运行环境：" << (运行环境已初始化_ ? L"已初始化" : L"未初始化") << L"\r\n";
    输出 << L"显示状态：" << (显示启用_ ? L"显示中" : L"未显示") << L"\r\n";

    if (!摄像机运行时_) {
        输出 << L"相机运行时：尚未创建\r\n";
        return 宽转CString_(输出.str());
    }

    const auto 快照 = 摄像机运行时_->快照();
    输出 << L"相机状态：" << (快照.运行中 ? L"运行中" : L"已停止") << L"\r\n";
    输出 << L"相机来源：" << (快照.使用虚拟相机 ? L"虚拟相机" : L"D455 / 实机优先") << L"\r\n";
    输出 << L"已处理帧数：" << 快照.已处理帧数 << L"\r\n";
    输出 << L"最近桥接候选数：" << 快照.最近桥接候选数 << L"\r\n";
    输出 << L"最近桥接时间(us)：" << 快照.最近桥接时间 << L"\r\n";
    输出 << L"最近状态摘要：" << UTF8转宽_(快照.最近状态摘要) << L"\r\n";

    auto* 内部世界 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
    输出 << L"桥接世界：" << (内部世界 ? 节点名称或空_(内部世界) : L"未创建") << L"\r\n";
    输出 << L"桥接输入场景：" << (查找子场景_按名称(内部世界, "主窗口_自我观察输入") ? L"已存在" : L"未发现") << L"\r\n";

    return 宽转CString_(输出.str());
}

CString C鱼巢外设相机Dlg::生成观测显示文本_() const
{
    std::wostringstream 输出;
    if (!显示启用_) {
        输出 << L"显示未启动。\r\n点击“启动显示”后，这里会实时展示相机桥接场景中的候选存在信息。";
        return 宽转CString_(输出.str());
    }

    auto* 内部世界 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
    if (!内部世界) {
        输出 << L"尚未创建虚拟世界。";
        return 宽转CString_(输出.str());
    }

    auto* 输入场景 = 查找子场景_按名称(内部世界, "主窗口_自我观察输入");
    if (!输入场景) {
        输出 << L"尚未收到相机桥接输入场景。";
        return 宽转CString_(输出.str());
    }

    const auto 深度有效万分比 = 世界树.读取特征_I64(
        输入场景,
        语素集.添加词性词("深度有效万分比", "名词"),
        "鱼巢_外设_相机/读深度有效万分比");
    if (深度有效万分比) {
        输出 << L"深度有效万分比：" << *深度有效万分比 << L"\r\n";
    }

    auto* 候选场景 = 查找子场景_按名称(输入场景, "候选存在集");
    if (!候选场景) {
        输出 << L"桥接输入已存在，但当前没有候选存在集。";
        return 宽转CString_(输出.str());
    }

    const auto 候选存在列表 = 世界树.获取子存在(
        候选场景,
        "鱼巢_外设_相机/获取候选存在列表");

    输出 << L"候选存在数：" << 候选存在列表.size() << L"\r\n\r\n";
    if (候选存在列表.empty()) {
        输出 << L"当前没有候选存在。";
        return 宽转CString_(输出.str());
    }

    const auto* 跟踪ID特征 = 语素集.添加词性词("跟踪ID", "名词");

    std::size_t 索引 = 0;
    for (auto* 候选存在 : 候选存在列表) {
        ++索引;
        I64 位置x = 0, 位置y = 0, 位置z = 0;
        I64 尺寸x = 0, 尺寸y = 0, 尺寸z = 0;

        const auto 位置快照 = 世界树.读取特征快照(
            候选存在,
            特征类型定义类::类型_绝对位置,
            "鱼巢_外设_相机/读候选绝对位置");
        const auto 尺寸快照 = 世界树.读取特征快照(
            候选存在,
            特征类型定义类::类型_尺寸,
            "鱼巢_外设_相机/读候选尺寸");
        const auto 轮廓快照 = 世界树.读取特征快照(
            候选存在,
            特征类型定义类::类型_轮廓编码,
            "鱼巢_外设_相机/读候选轮廓");
        const auto 时间戳_us = 世界树.读取特征_I64(
            候选存在,
            特征类型定义类::类型_时间戳_us,
            "鱼巢_外设_相机/读候选时间戳");
        const auto 跟踪ID = 世界树.读取特征_I64(
            候选存在,
            跟踪ID特征,
            "鱼巢_外设_相机/读候选跟踪ID");

        (void)取Vec3I64_(位置快照, 位置x, 位置y, 位置z);
        (void)取Vec3I64_(尺寸快照, 尺寸x, 尺寸y, 尺寸z);

        输出 << L"[" << 索引 << L"] "
             << UTF8转宽_(候选存在->获取主键()) << L"\r\n";
        if (跟踪ID) {
            输出 << L"  跟踪ID: " << *跟踪ID << L"\r\n";
        }
        输出 << L"  位置(mm): (" << 位置x << L", " << 位置y << L", " << 位置z << L")\r\n";
        输出 << L"  尺寸(mm): (" << 尺寸x << L", " << 尺寸y << L", " << 尺寸z << L")\r\n";
        输出 << L"  轮廓编码块数: " << 取VecU长度_(轮廓快照) << L"\r\n";
        if (时间戳_us) {
            输出 << L"  时间戳(us): " << *时间戳_us << L"\r\n";
        }
        输出 << L"\r\n";
    }

    return 宽转CString_(输出.str());
}

void C鱼巢外设相机Dlg::OnPaint()
{
    if (IsIconic()) {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        const int cxIcon = GetSystemMetrics(SM_CXICON);
        const int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        const int x = (rect.Width() - cxIcon + 1) / 2;
        const int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else {
        CDialogEx::OnPaint();
    }
}

HCURSOR C鱼巢外设相机Dlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void C鱼巢外设相机Dlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 定时器_刷新) {
        刷新界面_();
    }
    CDialogEx::OnTimer(nIDEvent);
}

void C鱼巢外设相机Dlg::OnClose()
{
    安全停止全部_();
    CDialogEx::OnCancel();
}

void C鱼巢外设相机Dlg::OnBnClicked启动相机()
{
    确保运行环境已初始化_();
    if (!摄像机运行时_) {
        摄像机运行时_ = std::make_unique<摄像机运行时包装>();
    }

    if (!摄像机运行时_->启动()) {
        AfxMessageBox(_T("启动相机失败。请检查 D455、RealSense 运行时或虚拟相机回退路径。"));
    }
    刷新界面_();
}

void C鱼巢外设相机Dlg::OnBnClicked停止相机()
{
    if (摄像机运行时_) {
        摄像机运行时_->停止();
    }
    刷新界面_();
}

void C鱼巢外设相机Dlg::OnBnClicked启动显示()
{
    显示启用_ = true;
    刷新界面_();
}

void C鱼巢外设相机Dlg::OnBnClicked停止显示()
{
    显示启用_ = false;
    刷新界面_();
}

void C鱼巢外设相机Dlg::OnBnClicked刷新()
{
    刷新界面_();
}

void C鱼巢外设相机Dlg::OnOK()
{
    安全停止全部_();
    CDialogEx::OnOK();
}

void C鱼巢外设相机Dlg::OnCancel()
{
    安全停止全部_();
    CDialogEx::OnCancel();
}
