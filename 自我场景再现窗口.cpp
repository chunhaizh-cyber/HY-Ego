#include "pch.h"
#include "framework.h"
#include "海鱼.h"
#include "海鱼Dlg.h"
#include "自我场景再现窗口.h"
#include "afxdialogex.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace {
CString UTF8到CString_(const std::string& 文本)
{
    return CString(CA2W(文本.c_str(), CP_UTF8));
}

CString 存在短标签_(const std::string& 主键)
{
    CString 文本 = UTF8到CString_(主键);
    if (文本.GetLength() > 14) {
        文本 = 文本.Left(14) + _T("...");
    }
    return 文本;
}

CString 布尔短文本_(bool 值)
{
    return 值 ? _T("是") : _T("否");
}

COLORREF 存在颜色_(const 结构_后台自我场景存在快照& 存在)
{
    if (存在.连续未命中帧 > 0) return RGB(180, 100, 100);
    if (存在.轮廓块数 > 0) return RGB(60, 180, 120);
    return RGB(80, 140, 220);
}

struct 三维点_ {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

struct 三维投影参数_ {
    double yaw弧度 = 0.0;
    double pitch弧度 = 0.0;
    double camera距离 = 2200.0;
    double focal像素 = 600.0;
    CPoint 画面原点{};
};

double 角度转弧度_(double 角度)
{
    return 角度 * 3.14159265358979323846 / 180.0;
}

三维点_ 旋转视角_(const 三维点_& 点, const 三维投影参数_& 参数)
{
    const double cosYaw = std::cos(参数.yaw弧度);
    const double sinYaw = std::sin(参数.yaw弧度);
    const double x1 = 点.x * cosYaw - 点.z * sinYaw;
    const double z1 = 点.x * sinYaw + 点.z * cosYaw;

    const double cosPitch = std::cos(参数.pitch弧度);
    const double sinPitch = std::sin(参数.pitch弧度);
    const double y2 = 点.y * cosPitch - z1 * sinPitch;
    const double z2 = 点.y * sinPitch + z1 * cosPitch;
    return { x1, y2, z2 };
}

bool 投影到屏幕_(const 三维点_& 世界点,
    const 三维点_& 场景中心,
    const 三维投影参数_& 参数,
    CPoint* 屏幕点,
    double* 深度)
{
    const 三维点_ 相对点{
        世界点.x - 场景中心.x,
        世界点.y - 场景中心.y,
        世界点.z - 场景中心.z
    };
    const 三维点_ 视角点 = 旋转视角_(相对点, 参数);
    const double 有效深度 = 视角点.z + 参数.camera距离;
    if (有效深度 < 120.0) {
        return false;
    }

    if (屏幕点) {
        屏幕点->x = 参数.画面原点.x + static_cast<LONG>(std::lround(视角点.x * 参数.focal像素 / 有效深度));
        屏幕点->y = 参数.画面原点.y - static_cast<LONG>(std::lround(视角点.y * 参数.focal像素 / 有效深度));
    }
    if (深度) {
        *深度 = 有效深度;
    }
    return true;
}

COLORREF 调亮颜色_(COLORREF 颜色, int 增量)
{
    const int r = std::clamp(static_cast<int>(GetRValue(颜色)) + 增量, 0, 255);
    const int g = std::clamp(static_cast<int>(GetGValue(颜色)) + 增量, 0, 255);
    const int b = std::clamp(static_cast<int>(GetBValue(颜色)) + 增量, 0, 255);
    return RGB(r, g, b);
}
}

C自我场景再现窗口::C自我场景再现窗口(C海鱼Dlg* 主窗口)
    : CDialogEx(IDD_DIALOG_SELF_SCENE_REPLAY, 主窗口),
      主窗口_(主窗口)
{
    图标_ = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

C自我场景再现窗口::~C自我场景再现窗口()
{
    if (::IsWindow(GetSafeHwnd())) {
        KillTimer(定时器_刷新);
    }
    if (标题字体_.GetSafeHandle()) 标题字体_.DeleteObject();
    if (正文字体_.GetSafeHandle()) 正文字体_.DeleteObject();
}

void C自我场景再现窗口::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(C自我场景再现窗口, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL C自我场景再现窗口::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetIcon(图标_, TRUE);
    SetIcon(图标_, FALSE);
    SetWindowTextW(_T("自我场景三维再现"));

    LOGFONTW 标题字体信息{};
    if (CFont* 默认字体 = GetFont()) {
        默认字体->GetLogFont(&标题字体信息);
    }
    else {
        ::SystemParametersInfoW(SPI_GETICONTITLELOGFONT, sizeof(LOGFONTW), &标题字体信息, 0);
    }
    标题字体信息.lfWeight = FW_BOLD;
    标题字体信息.lfHeight = -20;
    标题字体_.CreateFontIndirectW(&标题字体信息);

    LOGFONTW 正文字体信息 = 标题字体信息;
    正文字体信息.lfWeight = FW_NORMAL;
    正文字体信息.lfHeight = -14;
    正文字体_.CreateFontIndirectW(&正文字体信息);

    重置视角_();
    SetTimer(定时器_刷新, 250, nullptr);
    刷新快照_();
    return TRUE;
}

void C自我场景再现窗口::PostNcDestroy()
{
    CDialogEx::PostNcDestroy();
    if (主窗口_ && ::IsWindow(主窗口_->GetSafeHwnd())) {
        ::PostMessageW(主窗口_->GetSafeHwnd(), C海鱼Dlg::WM_APP_自我场景再现窗口关闭, 0, 0);
    }
}

void C自我场景再现窗口::刷新快照_()
{
    最近错误_.Empty();
    if (!主窗口_) {
        最近错误_ = _T("未绑定主窗口");
        最近快照_ = {};
        最近拾取项_.clear();
        return;
    }

    CString 错误文本;
    结构_后台自我场景快照 新快照{};
    if (!主窗口_->查询远程自我场景快照(&新快照, &错误文本)) {
        最近错误_ = 错误文本;
        if (最近错误_.IsEmpty()) {
            最近错误_ = _T("后台未返回自我场景快照");
        }
        最近快照_ = {};
        最近拾取项_.clear();
        return;
    }
    最近快照_ = std::move(新快照);

    if (!当前选中存在主键_.empty()) {
        const bool 仍存在 = std::any_of(
            最近快照_.存在列表.begin(),
            最近快照_.存在列表.end(),
            [&](const auto& 项) { return 项.存在主键 == 当前选中存在主键_; });
        if (!仍存在) {
            更新选中存在_("");
        }
    }
}

void C自我场景再现窗口::重置视角_()
{
    视角Yaw度_ = -34.0;
    视角Pitch度_ = 18.0;
    视角缩放倍数_ = 1.0;
}

bool C自我场景再现窗口::命中场景区_(CPoint 点) const
{
    return 最近场景区有效_ && 最近场景区_.PtInRect(点);
}

bool C自我场景再现窗口::是当前选中存在_(const std::string& 存在主键) const
{
    return !当前选中存在主键_.empty() && 当前选中存在主键_ == 存在主键;
}

void C自我场景再现窗口::更新选中存在_(const std::string& 存在主键)
{
    if (存在主键 == 当前选中存在主键_) {
        return;
    }

    当前选中存在主键_ = 存在主键;
    当前已请求跟随锁定_ = !当前选中存在主键_.empty();
    if (!主窗口_) {
        return;
    }

    if (当前选中存在主键_.empty()) {
        主窗口_->解除锁定观测();
    }
    else {
        主窗口_->锁定观测目标(当前选中存在主键_);
    }
}

bool C自我场景再现窗口::尝试按点击选中存在_(CPoint 点)
{
    const 结构_场景拾取项* 最优项 = nullptr;
    for (const auto& 项 : 最近拾取项_) {
        if (!项.包围框.PtInRect(点)) continue;
        if (!最优项 || 项.平均深度 < 最优项->平均深度) {
            最优项 = &项;
        }
    }
    if (!最优项) {
        if (!当前选中存在主键_.empty()) {
            更新选中存在_("");
            return true;
        }
        return false;
    }

    更新选中存在_(最优项->存在主键);
    return true;
}

void C自我场景再现窗口::绘制状态区_(CDC& dc, const CRect& 区域)
{
    CRect 标题区域 = 区域;
    标题区域.bottom = 标题区域.top + 28;
    CFont* 旧字体 = dc.SelectObject(&标题字体_);
    dc.SetTextColor(RGB(20, 20, 20));
    dc.DrawTextW(_T("自我场景三维再现"), 标题区域, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    dc.SelectObject(&正文字体_);
    CRect 文本区域 = 区域;
    文本区域.top = 标题区域.bottom + 4;

    CString 状态文本;
    if (!最近错误_.IsEmpty()) {
        状态文本.Format(_T("状态：%s"), 最近错误_.GetString());
    }
    else {
        const CString 后台在线文本 = 布尔短文本_(最近快照_.后台在线);
        const CString 再现运行文本 = 布尔短文本_(最近快照_.再现运行中);
        const CString 选中存在文本 = 当前选中存在主键_.empty()
            ? CString(_T("无"))
            : UTF8到CString_(当前选中存在主键_);
        const CString 跟随状态文本 = 当前已请求跟随锁定_ ? _T("已请求") : _T("未请求");
        状态文本.Format(
            _T("后台在线：%s  再现运行：%s\r\n状态：%s\r\n场景：%s\r\n帧序号：%llu，存在数：%u（单位：mm）\r\n选中存在：%s  跟随：%s\r\n视角：Yaw=%.1f° Pitch=%.1f° Zoom=%.2fx（拖拽旋转/滚轮缩放/双击重置，单击对象高亮并锁定）"),
            后台在线文本.GetString(),
            再现运行文本.GetString(),
            UTF8到CString_(最近快照_.状态摘要).GetString(),
            UTF8到CString_(最近快照_.场景主键).GetString(),
            static_cast<unsigned long long>(最近快照_.帧序号),
            static_cast<unsigned>(最近快照_.存在列表.size()),
            选中存在文本.GetString(),
            跟随状态文本.GetString(),
            视角Yaw度_,
            视角Pitch度_,
            视角缩放倍数_);
    }
    dc.DrawTextW(状态文本, 文本区域, DT_LEFT | DT_TOP | DT_WORDBREAK);
    dc.SelectObject(旧字体);
}

void C自我场景再现窗口::绘制场景区_(CDC& dc, const CRect& 区域)
{
    最近拾取项_.clear();
    dc.FillSolidRect(&区域, RGB(238, 243, 250));
    CRect 边框区域 = 区域;
    dc.DrawEdge(&边框区域, BDR_SUNKENOUTER, BF_RECT);

    if (!最近错误_.IsEmpty()) {
        dc.SetBkMode(TRANSPARENT);
        dc.SetTextColor(RGB(160, 40, 40));
        dc.SelectObject(&正文字体_);
        CRect 提示区域 = 区域;
        dc.DrawTextW(最近错误_, 提示区域, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
        return;
    }

    if (!最近快照_.再现运行中) {
        dc.SetBkMode(TRANSPARENT);
        dc.SetTextColor(RGB(90, 90, 90));
        dc.SelectObject(&正文字体_);
        CRect 提示区域 = 区域;
        dc.DrawTextW(_T("自我场景三维再现未运行。\r\n请在主窗口点击“开始自我场景再现”。"),
            提示区域, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
        return;
    }

    std::vector<const 结构_后台自我场景存在快照*> 可绘制存在;
    可绘制存在.reserve(最近快照_.存在列表.size());
    double 最小X = std::numeric_limits<double>::max();
    double 最大X = std::numeric_limits<double>::lowest();
    double 最小Y = std::numeric_limits<double>::max();
    double 最大Y = std::numeric_limits<double>::lowest();
    double 最小Z = std::numeric_limits<double>::max();
    double 最大Z = std::numeric_limits<double>::lowest();

    for (const auto& 存在 : 最近快照_.存在列表) {
        if (!存在.有位置) continue;
        可绘制存在.push_back(&存在);
        const double 中心X = static_cast<double>(存在.中心X_mm);
        const double 中心Y = -static_cast<double>(存在.中心Y_mm);
        const double 中心Z = static_cast<double>(存在.中心Z_mm);
        const double 尺寸X = static_cast<double>(存在.有尺寸 ? std::max<std::int64_t>(存在.尺寸X_mm, 120) : 260);
        const double 尺寸Y = static_cast<double>(存在.有尺寸 ? std::max<std::int64_t>(存在.尺寸Y_mm, 120) : 260);
        const double 尺寸Z = static_cast<double>(存在.有尺寸 ? std::max<std::int64_t>(存在.尺寸Z_mm, 120) : 260);
        const double 半X = 尺寸X * 0.5;
        const double 半Y = 尺寸Y * 0.5;
        const double 半Z = 尺寸Z * 0.5;
        最小X = std::min(最小X, 中心X - 半X);
        最大X = std::max(最大X, 中心X + 半X);
        最小Y = std::min(最小Y, 中心Y - 半Y);
        最大Y = std::max(最大Y, 中心Y + 半Y);
        最小Z = std::min(最小Z, 中心Z - 半Z);
        最大Z = std::max(最大Z, 中心Z + 半Z);
    }

    dc.SetBkMode(TRANSPARENT);
    dc.SelectObject(&正文字体_);
    if (可绘制存在.empty()) {
        dc.SetTextColor(RGB(90, 90, 90));
        CRect 提示区域 = 区域;
        dc.DrawTextW(_T("当前没有带位置的存在可供再现"), 提示区域, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
        return;
    }

    const double 跨度X = std::max(最大X - 最小X, 1.0);
    const double 跨度Y = std::max(最大Y - 最小Y, 1.0);
    const double 跨度Z = std::max(最大Z - 最小Z, 1.0);
    const double 场景跨度 = std::max({ 跨度X, 跨度Y, 跨度Z, 600.0 });
    const 三维点_ 场景中心{
        (最小X + 最大X) * 0.5,
        (最小Y + 最大Y) * 0.5,
        (最小Z + 最大Z) * 0.5
    };

    三维投影参数_ 投影参数{};
    投影参数.yaw弧度 = 角度转弧度_(视角Yaw度_);
    投影参数.pitch弧度 = 角度转弧度_(视角Pitch度_);
    投影参数.camera距离 = std::max(1600.0, 场景跨度 * 2.8);
    投影参数.focal像素 = std::max(380.0,
        static_cast<double>(std::min(区域.Width(), 区域.Height())) * 1.05 * 视角缩放倍数_);
    投影参数.画面原点 = CPoint(
        区域.left + 区域.Width() / 2,
        区域.top + static_cast<int>(区域.Height() * 0.72));

    CRect 提示区域 = 区域;
    提示区域.DeflateRect(10, 6, 10, 0);
    提示区域.bottom = 提示区域.top + 18;
    dc.SetTextColor(RGB(72, 72, 82));
    dc.DrawTextW(_T("三维视角: X左右 / Y高低 / Z远近 (mm)，红点=自我，橙线=视锥"), 提示区域, DT_LEFT | DT_TOP | DT_SINGLELINE);

    const double 网格步长 = std::clamp(场景跨度 / 6.0, 220.0, 1200.0);
    const int 网格半数 = 6;
    const double 地面Y = 最小Y;
    CPen 网格笔(PS_DOT, 1, RGB(186, 196, 212));
    CPen* 旧笔 = dc.SelectObject(&网格笔);
    for (int i = -网格半数; i <= 网格半数; ++i) {
        const double 偏移 = i * 网格步长;
        const 三维点_ 线起点X{
            场景中心.x + 偏移,
            地面Y,
            场景中心.z - 网格半数 * 网格步长
        };
        const 三维点_ 线终点X{
            场景中心.x + 偏移,
            地面Y,
            场景中心.z + 网格半数 * 网格步长
        };
        CPoint p1{}, p2{};
        if (投影到屏幕_(线起点X, 场景中心, 投影参数, &p1, nullptr) &&
            投影到屏幕_(线终点X, 场景中心, 投影参数, &p2, nullptr)) {
            dc.MoveTo(p1);
            dc.LineTo(p2);
        }

        const 三维点_ 线起点Z{
            场景中心.x - 网格半数 * 网格步长,
            地面Y,
            场景中心.z + 偏移
        };
        const 三维点_ 线终点Z{
            场景中心.x + 网格半数 * 网格步长,
            地面Y,
            场景中心.z + 偏移
        };
        if (投影到屏幕_(线起点Z, 场景中心, 投影参数, &p1, nullptr) &&
            投影到屏幕_(线终点Z, 场景中心, 投影参数, &p2, nullptr)) {
            dc.MoveTo(p1);
            dc.LineTo(p2);
        }
    }
    dc.SelectObject(旧笔);

    const auto 绘制坐标轴_ = [&](const 三维点_& 起点, const 三维点_& 终点, COLORREF 颜色, const wchar_t* 标签) {
        CPoint p0{}, p1{};
        if (!投影到屏幕_(起点, 场景中心, 投影参数, &p0, nullptr) ||
            !投影到屏幕_(终点, 场景中心, 投影参数, &p1, nullptr)) {
            return;
        }
        CPen 轴笔(PS_SOLID, 2, 颜色);
        CPen* 旧轴笔 = dc.SelectObject(&轴笔);
        dc.MoveTo(p0);
        dc.LineTo(p1);
        dc.SelectObject(旧轴笔);

        CRect 标签区域(p1.x + 4, p1.y - 10, p1.x + 34, p1.y + 10);
        dc.SetTextColor(颜色);
        dc.DrawTextW(标签, 标签区域, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    };

    const double 坐标轴长度 = std::max(400.0, 场景跨度 * 0.35);
    const 三维点_ 轴原点{ 场景中心.x, 地面Y, 场景中心.z };
    绘制坐标轴_(轴原点, 三维点_{ 轴原点.x + 坐标轴长度, 轴原点.y, 轴原点.z }, RGB(210, 80, 70), L"X");
    绘制坐标轴_(轴原点, 三维点_{ 轴原点.x, 轴原点.y + 坐标轴长度, 轴原点.z }, RGB(70, 150, 80), L"Y");
    绘制坐标轴_(轴原点, 三维点_{ 轴原点.x, 轴原点.y, 轴原点.z + 坐标轴长度 }, RGB(70, 110, 200), L"Z");

    // 叠加“自我”观测锚点与前向视锥，明确当前位置和朝向关系。
    const 三维点_ 自我位置{ 0.0, 0.0, 0.0 };
    CPoint 自我屏幕点{};
    double 自我深度 = 0.0;
    if (投影到屏幕_(自我位置, 场景中心, 投影参数, &自我屏幕点, &自我深度)) {
        const int 半径 = std::clamp(static_cast<int>(std::lround(1500.0 / std::max(420.0, 自我深度))), 5, 11);
        CPen 外圈笔(PS_SOLID, 2, RGB(220, 80, 80));
        CBrush 外圈刷(RGB(255, 225, 225));
        CPen* 旧点笔 = dc.SelectObject(&外圈笔);
        CBrush* 旧点刷 = dc.SelectObject(&外圈刷);
        dc.Ellipse(自我屏幕点.x - 半径, 自我屏幕点.y - 半径, 自我屏幕点.x + 半径, 自我屏幕点.y + 半径);

        CBrush 内圈刷(RGB(226, 50, 50));
        dc.SelectObject(&内圈刷);
        const int 内半径 = std::max(2, 半径 / 2);
        dc.Ellipse(自我屏幕点.x - 内半径, 自我屏幕点.y - 内半径, 自我屏幕点.x + 内半径, 自我屏幕点.y + 内半径);
        dc.SelectObject(旧点刷);
        dc.SelectObject(旧点笔);

        CRect 自我标签(自我屏幕点.x + 8, 自我屏幕点.y - 14, 自我屏幕点.x + 72, 自我屏幕点.y + 6);
        dc.SetTextColor(RGB(190, 50, 50));
        dc.DrawTextW(_T("SELF"), 自我标签, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

        const double 视锥长度 = std::max(300.0, 场景跨度 * 0.24);
        const double 视锥半宽 = std::max(100.0, 视锥长度 * 0.34);
        const double 视锥半高 = std::max(60.0, 视锥长度 * 0.20);
        const std::array<三维点_, 4> 视锥角点{ {
            { 自我位置.x - 视锥半宽, 自我位置.y - 视锥半高, 自我位置.z + 视锥长度 },
            { 自我位置.x + 视锥半宽, 自我位置.y - 视锥半高, 自我位置.z + 视锥长度 },
            { 自我位置.x + 视锥半宽, 自我位置.y + 视锥半高, 自我位置.z + 视锥长度 },
            { 自我位置.x - 视锥半宽, 自我位置.y + 视锥半高, 自我位置.z + 视锥长度 }
        } };
        std::array<CPoint, 4> 视锥角点屏幕{};
        std::array<bool, 4> 视锥角点有效{};
        for (std::size_t i = 0; i < 视锥角点.size(); ++i) {
            视锥角点有效[i] = 投影到屏幕_(视锥角点[i], 场景中心, 投影参数, &视锥角点屏幕[i], nullptr);
        }

        CPen 视锥笔(PS_SOLID, 1, RGB(238, 130, 48));
        CPen* 旧视锥笔 = dc.SelectObject(&视锥笔);
        for (int i = 0; i < 4; ++i) {
            if (!视锥角点有效[i]) continue;
            dc.MoveTo(自我屏幕点);
            dc.LineTo(视锥角点屏幕[i]);
        }
        for (int i = 0; i < 4; ++i) {
            const int j = (i + 1) % 4;
            if (!视锥角点有效[i] || !视锥角点有效[j]) continue;
            dc.MoveTo(视锥角点屏幕[i]);
            dc.LineTo(视锥角点屏幕[j]);
        }
        dc.SelectObject(旧视锥笔);
    }

    struct 结构_可绘制盒 {
        const 结构_后台自我场景存在快照* 源 = nullptr;
        std::array<CPoint, 8> 顶点{};
        std::array<bool, 8> 顶点有效{};
        CPoint 标签点{};
        bool 标签有效 = false;
        CRect 包围框{};
        bool 包围框有效 = false;
        double 平均深度 = 0.0;
        COLORREF 颜色 = RGB(100, 150, 210);
    };
    std::vector<结构_可绘制盒> 盒列表;
    盒列表.reserve(可绘制存在.size());

    for (const auto* 存在 : 可绘制存在) {
        const double 中心X = static_cast<double>(存在->中心X_mm);
        const double 中心Y = -static_cast<double>(存在->中心Y_mm);
        const double 中心Z = static_cast<double>(存在->中心Z_mm);
        const double 尺寸X = static_cast<double>(存在->有尺寸 ? std::max<std::int64_t>(存在->尺寸X_mm, 120) : 260);
        const double 尺寸Y = static_cast<double>(存在->有尺寸 ? std::max<std::int64_t>(存在->尺寸Y_mm, 120) : 260);
        const double 尺寸Z = static_cast<double>(存在->有尺寸 ? std::max<std::int64_t>(存在->尺寸Z_mm, 120) : 260);
        const double 半X = 尺寸X * 0.5;
        const double 半Y = 尺寸Y * 0.5;
        const double 半Z = 尺寸Z * 0.5;

        const std::array<三维点_, 8> 世界顶点{ {
            { 中心X - 半X, 中心Y - 半Y, 中心Z - 半Z },
            { 中心X + 半X, 中心Y - 半Y, 中心Z - 半Z },
            { 中心X + 半X, 中心Y + 半Y, 中心Z - 半Z },
            { 中心X - 半X, 中心Y + 半Y, 中心Z - 半Z },
            { 中心X - 半X, 中心Y - 半Y, 中心Z + 半Z },
            { 中心X + 半X, 中心Y - 半Y, 中心Z + 半Z },
            { 中心X + 半X, 中心Y + 半Y, 中心Z + 半Z },
            { 中心X - 半X, 中心Y + 半Y, 中心Z + 半Z }
        } };

        结构_可绘制盒 盒{};
        盒.源 = 存在;
        盒.颜色 = 存在颜色_(*存在);
        double 深度累计 = 0.0;
        int 深度计数 = 0;
        LONG minX = std::numeric_limits<LONG>::max();
        LONG minY = std::numeric_limits<LONG>::max();
        LONG maxX = std::numeric_limits<LONG>::lowest();
        LONG maxY = std::numeric_limits<LONG>::lowest();
        for (std::size_t i = 0; i < 世界顶点.size(); ++i) {
            CPoint 屏幕点{};
            double 深度 = 0.0;
            if (投影到屏幕_(世界顶点[i], 场景中心, 投影参数, &屏幕点, &深度)) {
                盒.顶点[i] = 屏幕点;
                盒.顶点有效[i] = true;
                深度累计 += 深度;
                ++深度计数;
                minX = std::min(minX, 屏幕点.x);
                minY = std::min(minY, 屏幕点.y);
                maxX = std::max(maxX, 屏幕点.x);
                maxY = std::max(maxY, 屏幕点.y);
            }
        }
        if (深度计数 <= 0) {
            continue;
        }
        盒.平均深度 = 深度累计 / static_cast<double>(深度计数);
        if (minX <= maxX && minY <= maxY) {
            盒.包围框 = CRect(minX - 2, minY - 2, maxX + 3, maxY + 3);
            盒.包围框有效 = true;
        }
        const 三维点_ 标签锚点{ 中心X, 中心Y + 半Y + 80.0, 中心Z };
        if (投影到屏幕_(标签锚点, 场景中心, 投影参数, &盒.标签点, nullptr)) {
            盒.标签有效 = true;
        }
        盒列表.push_back(std::move(盒));
    }

    if (盒列表.empty()) {
        dc.SetTextColor(RGB(90, 90, 90));
        CRect 空区域 = 区域;
        dc.DrawTextW(_T("存在已接入，但当前三维视角中没有可投影对象"), 空区域, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
        return;
    }

    std::sort(盒列表.begin(), 盒列表.end(),
        [](const 结构_可绘制盒& 左, const 结构_可绘制盒& 右) { return 左.平均深度 > 右.平均深度; });

    最近拾取项_.clear();
    最近拾取项_.reserve(盒列表.size());
    for (const auto& 盒 : 盒列表) {
        if (!盒.包围框有效 || !盒.源) continue;
        最近拾取项_.push_back({ 盒.源->存在主键, 盒.包围框, 盒.平均深度 });
    }

    constexpr int 边索引[12][2] = {
        {0,1},{1,2},{2,3},{3,0},
        {4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
    };

    for (const auto& 盒 : 盒列表) {
        const bool 当前选中 = 盒.源 && 是当前选中存在_(盒.源->存在主键);
        const COLORREF 基础颜色 = 当前选中 ? RGB(255, 196, 62) : 盒.颜色;
        const bool 顶面可绘 = 盒.顶点有效[3] && 盒.顶点有效[2] && 盒.顶点有效[6] && 盒.顶点有效[7];
        if (顶面可绘) {
            CPoint 顶面[4] = { 盒.顶点[3], 盒.顶点[2], 盒.顶点[6], 盒.顶点[7] };
            CBrush 顶面刷(调亮颜色_(基础颜色, 当前选中 ? 48 : 92));
            CPen 无边框笔(PS_NULL, 0, RGB(0, 0, 0));
            CBrush* 旧刷 = dc.SelectObject(&顶面刷);
            CPen* 旧面笔 = dc.SelectObject(&无边框笔);
            dc.Polygon(顶面, 4);
            dc.SelectObject(旧面笔);
            dc.SelectObject(旧刷);
        }

        const bool 侧面可绘 = 盒.顶点有效[1] && 盒.顶点有效[2] && 盒.顶点有效[6] && 盒.顶点有效[5];
        if (侧面可绘) {
            CPoint 侧面[4] = { 盒.顶点[1], 盒.顶点[2], 盒.顶点[6], 盒.顶点[5] };
            CBrush 侧面刷(调亮颜色_(基础颜色, 当前选中 ? 22 : 58));
            CPen 无边框笔(PS_NULL, 0, RGB(0, 0, 0));
            CBrush* 旧刷 = dc.SelectObject(&侧面刷);
            CPen* 旧面笔 = dc.SelectObject(&无边框笔);
            dc.Polygon(侧面, 4);
            dc.SelectObject(旧面笔);
            dc.SelectObject(旧刷);
        }

        CPen 轮廓笔(PS_SOLID, 当前选中 ? 3 : 2, 调亮颜色_(基础颜色, 当前选中 ? -20 : 0));
        CPen* 旧轮廓笔 = dc.SelectObject(&轮廓笔);
        for (const auto& 边 : 边索引) {
            const int a = 边[0];
            const int b = 边[1];
            if (!盒.顶点有效[a] || !盒.顶点有效[b]) continue;
            dc.MoveTo(盒.顶点[a]);
            dc.LineTo(盒.顶点[b]);
        }
        dc.SelectObject(旧轮廓笔);

        if (盒.标签有效) {
            dc.SetTextColor(当前选中 ? RGB(168, 90, 10) : RGB(26, 26, 32));
            CRect 标签区域(盒.标签点.x - 70, 盒.标签点.y - 20, 盒.标签点.x + 70, 盒.标签点.y - 2);
            dc.DrawTextW(存在短标签_(盒.源->存在主键), 标签区域,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
        }
    }
}

void C自我场景再现窗口::绘制列表区_(CDC& dc, const CRect& 区域)
{
    dc.FillSolidRect(&区域, RGB(252, 252, 252));
    CRect 边框区域 = 区域;
    dc.DrawEdge(&边框区域, BDR_SUNKENOUTER, BF_RECT);
    dc.SetBkMode(TRANSPARENT);
    dc.SelectObject(&正文字体_);

    CString 文本;
    if (!最近错误_.IsEmpty()) {
        文本 = 最近错误_;
    }
    else if (!最近快照_.再现运行中) {
        文本 = _T("自我场景再现未运行，请在主窗口先启动。");
    }
    else if (最近快照_.存在列表.empty()) {
        文本 = _T("当前没有可列出的存在。");
    }
    else {
        std::wostringstream 输出;
        const std::size_t 显示数量 = std::min<std::size_t>(最近快照_.存在列表.size(), 8);
        for (std::size_t i = 0; i < 显示数量; ++i) {
            const auto& 项 = 最近快照_.存在列表[i];
            const bool 选中 = 是当前选中存在_(项.存在主键);
            输出 << (选中 ? L"[*] " : L"[ ] ");
            输出 << L"[" << (i + 1) << L"] " << UTF8到CString_(项.存在主键).GetString() << L"\r\n";
            if (项.有位置) {
                输出 << L"  位置(mm)=(" << 项.中心X_mm << L"," << 项.中心Y_mm << L"," << 项.中心Z_mm << L")";
            }
            if (项.有尺寸) {
                输出 << L"  尺寸(mm)=(" << 项.尺寸X_mm << L"," << 项.尺寸Y_mm << L"," << 项.尺寸Z_mm << L")";
            }
            输出 << L"  轮廓块=" << 项.轮廓块数
                << L"  命中=" << 项.连续命中帧
                << L"  未命中=" << 项.连续未命中帧
                << L"  静止=" << 项.连续静止帧
                << L"\r\n";
        }
        if (最近快照_.存在列表.size() > 显示数量) {
            输出 << L"... 其余 " << (最近快照_.存在列表.size() - 显示数量) << L" 个存在已省略";
        }
        文本 = 输出.str().c_str();
    }
    dc.SetTextColor(RGB(50, 50, 50));
    CRect 文本区域 = 区域;
    dc.DrawTextW(文本, 文本区域, DT_LEFT | DT_TOP | DT_WORDBREAK);
}

void C自我场景再现窗口::OnPaint()
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
        dc.DrawIcon(x, y, 图标_);
        return;
    }

    CPaintDC dc(this);
    CRect 客户区;
    GetClientRect(&客户区);
    dc.FillSolidRect(&客户区, RGB(235, 239, 245));

    CRect 状态区 = 客户区;
    状态区.DeflateRect(12, 12, 12, 12);
    状态区.bottom = 状态区.top + 114;

    CRect 场景区 = 客户区;
    场景区.DeflateRect(12, 0, 12, 12);
    场景区.top = 状态区.bottom + 10;
    场景区.bottom = 客户区.bottom - 130;
    最近场景区_ = 场景区;
    最近场景区有效_ = (最近场景区_.Width() > 0 && 最近场景区_.Height() > 0);

    CRect 列表区 = 客户区;
    列表区.DeflateRect(12, 0, 12, 12);
    列表区.top = 场景区.bottom + 10;

    绘制状态区_(dc, 状态区);
    绘制场景区_(dc, 场景区);
    绘制列表区_(dc, 列表区);
}

HCURSOR C自我场景再现窗口::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(图标_);
}

void C自我场景再现窗口::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 定时器_刷新) {
        刷新快照_();
        Invalidate(FALSE);
    }
    CDialogEx::OnTimer(nIDEvent);
}

void C自我场景再现窗口::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (命中场景区_(point)) {
        正在拖拽视角_ = true;
        上次拖拽点_ = point;
        场景按下候选点击_ = true;
        场景按下点_ = point;
        SetCapture();
    }
    else {
        场景按下候选点击_ = false;
    }
    CDialogEx::OnLButtonDown(nFlags, point);
}

void C自我场景再现窗口::OnLButtonUp(UINT nFlags, CPoint point)
{
    const bool 可判定点击 = 场景按下候选点击_;
    if (正在拖拽视角_) {
        正在拖拽视角_ = false;
        if (::GetCapture() == m_hWnd) {
            ::ReleaseCapture();
        }
    }
    场景按下候选点击_ = false;

    if (可判定点击 && 命中场景区_(point)) {
        const int dx = std::abs(point.x - 场景按下点_.x);
        const int dy = std::abs(point.y - 场景按下点_.y);
        if (dx <= 4 && dy <= 4) {
            if (尝试按点击选中存在_(point)) {
                Invalidate(FALSE);
            }
        }
    }
    CDialogEx::OnLButtonUp(nFlags, point);
}

void C自我场景再现窗口::OnMouseMove(UINT nFlags, CPoint point)
{
    if (正在拖拽视角_ && (nFlags & MK_LBUTTON)) {
        const int dx = point.x - 上次拖拽点_.x;
        const int dy = point.y - 上次拖拽点_.y;
        if (dx != 0 || dy != 0) {
            if (场景按下候选点击_) {
                const int 拖拽累计X = std::abs(point.x - 场景按下点_.x);
                const int 拖拽累计Y = std::abs(point.y - 场景按下点_.y);
                if (拖拽累计X > 4 || 拖拽累计Y > 4) {
                    场景按下候选点击_ = false;
                }
            }
            视角Yaw度_ += static_cast<double>(dx) * 0.35;
            视角Pitch度_ = std::clamp(视角Pitch度_ - static_cast<double>(dy) * 0.28, -30.0, 78.0);
            上次拖拽点_ = point;
            Invalidate(FALSE);
        }
    }
    CDialogEx::OnMouseMove(nFlags, point);
}

BOOL C自我场景再现窗口::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    CPoint 客户点 = pt;
    ScreenToClient(&客户点);
    if (!命中场景区_(客户点)) {
        return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
    }

    const double 缩放因子 = std::pow(1.1, static_cast<double>(zDelta) / static_cast<double>(WHEEL_DELTA));
    视角缩放倍数_ = std::clamp(视角缩放倍数_ * 缩放因子, 0.45, 3.20);
    Invalidate(FALSE);
    return TRUE;
}

void C自我场景再现窗口::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if (命中场景区_(point)) {
        重置视角_();
        Invalidate(FALSE);
        场景按下候选点击_ = false;
        return;
    }
    CDialogEx::OnLButtonDblClk(nFlags, point);
}

void C自我场景再现窗口::OnClose()
{
    if (正在拖拽视角_) {
        正在拖拽视角_ = false;
        if (::GetCapture() == m_hWnd) {
            ::ReleaseCapture();
        }
    }
    if (::IsWindow(GetSafeHwnd())) {
        KillTimer(定时器_刷新);
    }
    DestroyWindow();
}

void C自我场景再现窗口::OnCancel()
{
    OnClose();
}
