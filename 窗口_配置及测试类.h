#pragma once
#include "afxdialogex.h"
 
 

// 前向声明：来自 C++20 模块 `场景实时显示线程模块`
import 场景实时显示线程模块;
// 前向声明主对话框类
class C海鱼Dlg;

// 窗口_配置及测试类 对话框
class 窗口_配置及测试类 : public CDialogEx
{
    DECLARE_DYNAMIC(窗口_配置及测试类)

public:
    窗口_配置及测试类(CWnd* pParent = nullptr);
    virtual ~窗口_配置及测试类();

    virtual void OnFinalRelease();

    // 绑定到“显示线程”（推荐：因为 OpenGL 显示器通常在显示线程内部创建）
    void 绑定显示线程(场景实时显示线程类* p) noexcept { 显示线程 = p; }

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG1 };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog() override;

    DECLARE_MESSAGE_MAP()
    DECLARE_DISPATCH_MAP()
    DECLARE_INTERFACE_MAP()

private:
    场景实时显示线程类* 显示线程 = nullptr;

    bool 读取Check(int idc) const;
    void 写Check(int idc, bool on);
    void 同步UI_从线程();

public:
    afx_msg void OnBnClickedStatic1();
    afx_msg void OnBnClickedCheck1();
    afx_msg void OnBnClickedCheck2();
    afx_msg void OnBnClickedCheck3();
    afx_msg void OnBnClickedCheck4();
    afx_msg void OnBnClickedCheck5();
    afx_msg void OnBnClickedCheck6();
    afx_msg void OnBnClickedCheck7();
    afx_msg void OnBnClickedCheck9();
    afx_msg void OnBnClickedCheck10();
    afx_msg void OnBnClickedCheck11();
    afx_msg void OnNMCustomdrawSlider8(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnBnClickedCheck8();
    afx_msg void OnBnClickedCheck12();
    afx_msg void OnBnClickedCheck13();
    afx_msg void OnBnClickedCheck14();
    afx_msg void OnBnClickedCheck15();
    afx_msg void OnBnClickedCheck16();
    afx_msg void OnBnClickedButtonStartDisplay();
    afx_msg void OnBnClickedButtonStopDisplay();
    afx_msg void OnBnClickedButtonStartCamera();
    afx_msg void OnBnClickedButtonStopCamera();
};
