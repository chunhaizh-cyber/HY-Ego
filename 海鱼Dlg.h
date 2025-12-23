
// 海鱼Dlg.h: 头文件
//

#pragma once
#include "窗口_交互界面.h"
#include <memory>
#include <vector>
#include <thread>
#include <atomic>


import 自我线程模块;
import 外设模块;
import 场景实时显示线程模块;
import 世界树环境模块;

class C海鱼DlgAutoProxy;

// 仅 forward declare（避免头文件里 import 一堆类型模块）
struct 结构体_原始场景帧;
struct 结构体_存在观测;

// C海鱼Dlg 对话框
class C海鱼Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(C海鱼Dlg);
	friend class C海鱼DlgAutoProxy;

	// 构造
public:
	C海鱼Dlg(CWnd* pParent = nullptr);	// 标准构造函数
	virtual ~C海鱼Dlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	// 实现
protected:
	C海鱼DlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl 变量_TAB1;
	窗口_交互界面 变量_交互界面;

private:
	自我线程类 自我线程;
	std::unique_ptr<场景实时显示线程类> 场景显示线程;

	struct 摄像机运行时;
	std::unique_ptr<摄像机运行时> 摄像机_;
	
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

	// ===== 场景实时显示（OpenCV viz） =====
	void 启动场景实时显示();
	void 停止场景实时显示();

	// ===== 外设采集（D455） =====
	void 启动外设();
	void 停止外设();

	// ===== 数据提交：外设/自我线程 -> 显示线程 =====
	void 提交场景显示(std::shared_ptr<结构体_原始场景帧> 帧,
		std::shared_ptr<std::vector<结构体_存在观测>> 观测);

	//	void 初始化();
};


extern C海鱼Dlg* 主窗口指针;