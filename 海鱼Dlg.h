
// 海鱼Dlg.h: 头文件
//

#pragma once
#include "窗口_交互界面类.h"
#include "窗口_基础信息类.h"
#include "窗口_配置及测试类.h"
#include <memory>
#include <vector>
#include <string>
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
	窗口_交互界面类 变量_交互界面;
	窗口_基础信息类 变量_基础信息界面;
	窗口_配置及测试类 变量_配置及测试窗口;
private:
	std::unique_ptr<自我线程类> 自我线程 = std::make_unique<自我线程类>();
	std::unique_ptr<场景实时显示线程类> 场景显示线程;
	bool 应用退出中_ = false;

	struct 摄像机运行时;
	std::unique_ptr<摄像机运行时> 摄像机_;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg LRESULT OnApp显示参数(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	// 观察导演：提示更换观测角度	
	static constexpr UINT WM_APP_观察换角度 = WM_APP + 502;
	afx_msg LRESULT OnApp观察换角度(WPARAM wParam, LPARAM lParam);
	// ===== 场景实时显示（OpenCV viz） =====
	void 启动场景实时显示();
	void 停止场景实时显示();

	// ===== 外设采集（D455） =====
	void 启动外设();
	void 停止外设();

	// ===== 数据提交：外设/自我线程 -> 显示线程 =====
	void 提交场景显示(std::shared_ptr<结构体_原始场景帧> 帧,
		std::shared_ptr<std::vector<结构体_存在观测>> 观测);

	// ===== 观测调度（UI -> 采集线程）=====
	void 锁定观测目标(const std::string& 存在主键);
	void 解除锁定观测();

	void 更新信息查看框布局();
	void 刷新当前标签内容();
	CString 生成标签页文本(int 标签索引) const;
	bool 标签页使用信息查看框(int 标签索引) const;
	void 清理退出子窗口();
//	void 初始化();

private:
	CEdit 变量_信息查看框;
};


extern C海鱼Dlg* 主窗口指针;





