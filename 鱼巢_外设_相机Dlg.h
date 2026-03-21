//
// 鱼巢_外设_相机Dlg.h
//

#pragma once

#include <memory>

class C鱼巢外设相机Dlg : public CDialogEx
{
public:
    explicit C鱼巢外设相机Dlg(CWnd* pParent = nullptr);
    virtual ~C鱼巢外设相机Dlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_鱼巢外设相机_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX) override;
    virtual BOOL OnInitDialog() override;

    DECLARE_MESSAGE_MAP()

private:
    struct 摄像机运行时包装;

    HICON m_hIcon = nullptr;
    std::unique_ptr<摄像机运行时包装> 摄像机运行时_{};
    bool 运行环境已初始化_ = false;
    bool 显示启用_ = false;

    CEdit 运行状态框_{};
    CEdit 观测显示框_{};

private:
    static constexpr UINT_PTR 定时器_刷新 = 1;

private:
    void 确保运行环境已初始化_();
    void 安全停止全部_();
    void 刷新界面_();
    void 刷新按钮状态_();
    CString 生成运行状态文本_() const;
    CString 生成观测显示文本_() const;

    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnClose();
    afx_msg void OnBnClicked启动相机();
    afx_msg void OnBnClicked停止相机();
    afx_msg void OnBnClicked启动显示();
    afx_msg void OnBnClicked停止显示();
    afx_msg void OnBnClicked刷新();
    virtual void OnOK() override;
    virtual void OnCancel() override;
};

