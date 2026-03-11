#include "pch.h"
#include <mutex>
#include <string>
#include "framework.h"
#include "海鱼.h"
#include "海鱼Dlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"
#include "窗口_配置及测试类.h"

// 关键：导入显示线程模块（已 re-export 场景实时显示模块）
 import 场景实时显示模块;
IMPLEMENT_DYNAMIC(窗口_配置及测试类, CDialogEx)

窗口_配置及测试类::窗口_配置及测试类(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG1, pParent)
{
#ifndef _WIN32_WCE
    EnableActiveAccessibility();
#endif
    EnableAutomation();
}

窗口_配置及测试类::~窗口_配置及测试类() {}

void 窗口_配置及测试类::OnFinalRelease()
{
    CDialogEx::OnFinalRelease();
}

void 窗口_配置及测试类::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BOOL 窗口_配置及测试类::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    同步UI_从线程();
    return TRUE;
}

bool 窗口_配置及测试类::读取Check(int idc) const
{
    auto* w = const_cast<窗口_配置及测试类*>(this)->GetDlgItem(idc);
    if (!w) return false;
    auto* btn = reinterpret_cast<CButton*>(w);
    return btn->GetCheck() == BST_CHECKED;
}

void 窗口_配置及测试类::写Check(int idc, bool on)
{
    auto* w = GetDlgItem(idc);
    if (!w) return;
    auto* btn = reinterpret_cast<CButton*>(w);
    btn->SetCheck(on ? BST_CHECKED : BST_UNCHECKED);
}

void 窗口_配置及测试类::同步UI_从线程()
{
//   if (!显示线程) return;
//
//   const auto p = 显示线程->获取显示参数();
//
//   写Check(IDC_CHECK1, p.显示网格地面);
//   写Check(IDC_CHECK2, p.显示实时点云);
//   写Check(IDC_CHECK3, p.显示存在框);
//   写Check(IDC_CHECK4, p.显示存在主轴);
//   写Check(IDC_CHECK5, p.显示存在点云簇);
//   写Check(IDC_CHECK6, p.显示体素网格);
//   写Check(IDC_CHECK7, p.网格仅表面);
//   写Check(IDC_CHECK9, p.使用相机坐标系映射);
//   写Check(IDC_CHECK10, p.点云使用颜色);
//   写Check(IDC_CHECK11, (p.坐标模式 != 0)); // 0=相对, 1=绝对
//   写Check(IDC_CHECK11, (p.坐标模式 != 0));  
//	写Check(IDC_CHECK12, p.显示体素立方体);
//	写Check(IDC_CHECK13, p.显示彩色截图);
//	写Check(IDC_CHECK14, p.显示轮廓编码);
//	写Check(IDC_CHECK15, p.显示边界序列点);
//	写Check(IDC_CHECK16, p.自动选择显示模式);
       
}

BEGIN_MESSAGE_MAP(窗口_配置及测试类, CDialogEx)
    // 静态控件：建议资源里给 IDC_STATIC1 设置 SS_NOTIFY
    ON_STN_CLICKED(IDC_STATIC1, &窗口_配置及测试类::OnBnClickedStatic1)

    ON_BN_CLICKED(IDC_CHECK1, &窗口_配置及测试类::OnBnClickedCheck1)
    ON_BN_CLICKED(IDC_CHECK2, &窗口_配置及测试类::OnBnClickedCheck2)
    ON_BN_CLICKED(IDC_CHECK3, &窗口_配置及测试类::OnBnClickedCheck3)
    ON_BN_CLICKED(IDC_CHECK4, &窗口_配置及测试类::OnBnClickedCheck4)
    ON_BN_CLICKED(IDC_CHECK5, &窗口_配置及测试类::OnBnClickedCheck5)
    ON_BN_CLICKED(IDC_CHECK6, &窗口_配置及测试类::OnBnClickedCheck6)
    ON_BN_CLICKED(IDC_CHECK7, &窗口_配置及测试类::OnBnClickedCheck7)
    ON_BN_CLICKED(IDC_CHECK9, &窗口_配置及测试类::OnBnClickedCheck9)
    ON_BN_CLICKED(IDC_CHECK10, &窗口_配置及测试类::OnBnClickedCheck10)
    ON_BN_CLICKED(IDC_CHECK11, &窗口_配置及测试类::OnBnClickedCheck11)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER8, &窗口_配置及测试类::OnNMCustomdrawSlider8)
    ON_BN_CLICKED(IDC_CHECK8, &窗口_配置及测试类::OnBnClickedCheck8)
    ON_BN_CLICKED(IDC_CHECK12, &窗口_配置及测试类::OnBnClickedCheck12)
    ON_BN_CLICKED(IDC_CHECK13, &窗口_配置及测试类::OnBnClickedCheck13)
    ON_BN_CLICKED(IDC_CHECK14, &窗口_配置及测试类::OnBnClickedCheck14)
    ON_BN_CLICKED(IDC_CHECK15, &窗口_配置及测试类::OnBnClickedCheck15)
    ON_BN_CLICKED(IDC_CHECK16, &窗口_配置及测试类::OnBnClickedCheck16)
    ON_BN_CLICKED(IDC_BUTTON_START_DISPLAY, &窗口_配置及测试类::OnBnClickedButtonStartDisplay)
    ON_BN_CLICKED(IDC_BUTTON_STOP_DISPLAY, &窗口_配置及测试类::OnBnClickedButtonStopDisplay)
    ON_BN_CLICKED(IDC_BUTTON_START_CAMERA, &窗口_配置及测试类::OnBnClickedButtonStartCamera)
    ON_BN_CLICKED(IDC_BUTTON_STOP_CAMERA, &窗口_配置及测试类::OnBnClickedButtonStopCamera)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(窗口_配置及测试类, CDialogEx)
END_DISPATCH_MAP()

// {38b98a81-3cb8-4309-9251-c5d47fbed6b3}
static const IID IID_I窗口_配置及测试类 =
{ 0x38b98a81,0x3cb8,0x4309,{0x92,0x51,0xc5,0xd4,0x7f,0xbe,0xd6,0xb3} };

BEGIN_INTERFACE_MAP(窗口_配置及测试类, CDialogEx)
    INTERFACE_PART(窗口_配置及测试类, IID_I窗口_配置及测试类, Dispatch)
END_INTERFACE_MAP()

// -------------------- 消息处理：把 UI 勾选写进 场景显示参数 --------------------

void 窗口_配置及测试类::OnBnClickedStatic1()
{
 //   if (!显示线程) return;
 //
 //   // “静态控件点击”想稳定触发：资源里给 IDC_STATIC1 设置 SS_NOTIFY
 //   const auto cur = 显示线程->获取显示参数();
 //   const bool on = !cur.显示坐标轴;
 //
 //   显示线程->修改显示参数([&](场景显示参数& p) {
 //       p.显示坐标轴 = on;
 //       });
}

void 窗口_配置及测试类::OnBnClickedCheck1()
{
    if (!显示线程) return;
    const bool on = 读取Check(IDC_CHECK1);
 //   显示线程->修改显示参数([&](场景显示参数& p) { p.显示坐标轴 = on; });
}

void 窗口_配置及测试类::OnBnClickedCheck2()
{
    if (!显示线程) return;
    const bool on = 读取Check(IDC_CHECK2);
 //   显示线程->修改显示参数([&](场景显示参数& p) { p.显示网格地面 = on; });
}

void 窗口_配置及测试类::OnBnClickedCheck3()
{
    if (!显示线程) return;
    const bool on = 读取Check(IDC_CHECK3);
 //   显示线程->修改显示参数([&](场景显示参数& p) { p.显示实时点云 = on; });
}

void 窗口_配置及测试类::OnBnClickedCheck4()
{
    if (!显示线程) return;
    const bool on = 读取Check(IDC_CHECK4);
 //   显示线程->修改显示参数([&](场景显示参数& p) { p.显示存在框 = on; });
}

void 窗口_配置及测试类::OnBnClickedCheck5()
{
    if (!显示线程) return;
    const bool on = 读取Check(IDC_CHECK5);
 //   显示线程->修改显示参数([&](场景显示参数& p) { p.显示存在主轴 = on; });
}

void 窗口_配置及测试类::OnBnClickedCheck6()
{
    if (!显示线程) return;
    const bool on = 读取Check(IDC_CHECK6);
 //   显示线程->修改显示参数([&](场景显示参数& p) { p.显示存在点云簇 = on; });
}

void 窗口_配置及测试类::OnBnClickedCheck7()
{
    if (!显示线程) return;
    const bool on = 读取Check(IDC_CHECK7);
   // 显示线程->修改显示参数([&](场景显示参数& p) { p.显示体素网格 = on; });
}
void 窗口_配置及测试类::OnBnClickedCheck8()
{
    if (!显示线程) return;
    const bool on = 读取Check(IDC_CHECK7);
  //  显示线程->修改显示参数([&](场景显示参数& p) { p.网格仅表面 = on; });
}
void 窗口_配置及测试类::OnBnClickedCheck9()
{
    if (!显示线程) return;
    const bool on = 读取Check(IDC_CHECK9);
  //  显示线程->修改显示参数([&](场景显示参数& p) { p.使用相机坐标系映射 = on; });
}

void 窗口_配置及测试类::OnBnClickedCheck10()
{
    if (!显示线程) return;
    const bool on = 读取Check(IDC_CHECK10);
  //  显示线程->修改显示参数([&](场景显示参数& p) { p.点云使用颜色 = on; });
}

void 窗口_配置及测试类::OnBnClickedCheck11()
{
    if (!显示线程) return;
    const bool on = 读取Check(IDC_CHECK11);
 //   显示线程->修改显示参数([&](场景显示参数& p) {
 //       p.坐标模式 = on ? 1 : 0; // 0=相对, 1=绝对
  //      });
}

void 窗口_配置及测试类::OnNMCustomdrawSlider8(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
}



void 窗口_配置及测试类::OnBnClickedCheck12()
{
    if (!显示线程) return;
    const bool on = 读取Check(IDC_CHECK12);
 //   显示线程->修改显示参数([&](场景显示参数& p) { p.显示体素立方体 = on; });
}

void 窗口_配置及测试类::OnBnClickedCheck13()
{
    if (!显示线程) return;
    const bool on = 读取Check(IDC_CHECK13);
 //   显示线程->修改显示参数([&](场景显示参数& p) { p.显示彩色截图 = on; });
}

void 窗口_配置及测试类::OnBnClickedCheck14()
{
    if (!显示线程) return;
    const bool on = 读取Check(IDC_CHECK14);
  //  显示线程->修改显示参数([&](场景显示参数& p) { p.显示轮廓编码 = on; });
}

void 窗口_配置及测试类::OnBnClickedCheck15()
{
    if (!显示线程) return;
    const bool on = 读取Check(IDC_CHECK15);
 //   显示线程->修改显示参数([&](场景显示参数& p) { p.显示边界序列点 = on; });
}

void 窗口_配置及测试类::OnBnClickedCheck16()
{
    if (!显示线程) return;
    const bool on = 读取Check(IDC_CHECK16);
 //   显示线程->修改显示参数([&](场景显示参数& p) { p.自动选择显示模式 = on; });
}

void 窗口_配置及测试类::OnBnClickedButtonStartDisplay()
{
    // 获取主对话框指针并调用启动外设函数
    C海鱼Dlg* pMainDlg = dynamic_cast<C海鱼Dlg*>(AfxGetMainWnd());
    if (pMainDlg) {
        pMainDlg->启动场景实时显示();
    }
}

void 窗口_配置及测试类::OnBnClickedButtonStopDisplay()
{
    // 获取主对话框指针并调用停止外设函数
    C海鱼Dlg* pMainDlg = dynamic_cast<C海鱼Dlg*>(AfxGetMainWnd());
    if (pMainDlg) {
        pMainDlg->停止场景实时显示();
    }
}

void 窗口_配置及测试类::OnBnClickedButtonStartCamera()
{
    // 获取主对话框指针并调用启动摄像头函数
    C海鱼Dlg* pMainDlg = dynamic_cast<C海鱼Dlg*>(AfxGetMainWnd());
    if (pMainDlg) {
        pMainDlg->启动外设();
    }
}

void 窗口_配置及测试类::OnBnClickedButtonStopCamera()
{
    // 获取主对话框指针并调用停止摄像头函数
    C海鱼Dlg* pMainDlg = dynamic_cast<C海鱼Dlg*>(AfxGetMainWnd());
    if (pMainDlg) {
        pMainDlg->停止外设();
    }
}
