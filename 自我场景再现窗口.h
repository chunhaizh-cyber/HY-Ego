#pragma once

#include <string>
#include <vector>

import 后台前端通道模块;

class C海鱼Dlg;

class C自我场景再现窗口 : public CDialogEx
{
public:
    explicit C自我场景再现窗口(C海鱼Dlg* 主窗口);
    virtual ~C自我场景再现窗口();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_SELF_SCENE_REPLAY };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX) override;
    virtual BOOL OnInitDialog() override;
    virtual void PostNcDestroy() override;

    DECLARE_MESSAGE_MAP()

private:
    static constexpr UINT_PTR 定时器_刷新 = 1;

    C海鱼Dlg* 主窗口_ = nullptr;
    HICON 图标_ = nullptr;
    CFont 标题字体_{};
    CFont 正文字体_{};
    结构_后台自我场景快照 最近快照_{};
    CString 最近错误_{};
    double 视角Yaw度_ = -34.0;
    double 视角Pitch度_ = 18.0;
    double 视角缩放倍数_ = 1.0;
    bool 正在拖拽视角_ = false;
    CPoint 上次拖拽点_{};
    CRect 最近场景区_{};
    bool 最近场景区有效_ = false;
    bool 场景按下候选点击_ = false;
    CPoint 场景按下点_{};
    std::string 当前选中存在主键_{};
    bool 当前已请求跟随锁定_ = false;

    struct 结构_场景拾取项 {
        std::string 存在主键;
        CRect 包围框{};
        double 平均深度 = 0.0;
    };
    std::vector<结构_场景拾取项> 最近拾取项_{};

private:
    void 刷新快照_();
    void 重置视角_();
    bool 命中场景区_(CPoint 点) const;
    bool 是当前选中存在_(const std::string& 存在主键) const;
    void 更新选中存在_(const std::string& 存在主键);
    bool 尝试按点击选中存在_(CPoint 点);
    void 绘制状态区_(CDC& dc, const CRect& 区域);
    void 绘制场景区_(CDC& dc, const CRect& 区域);
    void 绘制列表区_(CDC& dc, const CRect& 区域);

    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnClose();
    virtual void OnCancel() override;
};
