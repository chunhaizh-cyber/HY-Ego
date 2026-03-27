
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
#include <cstdint>


import 后台前端通道模块;
import 自治宿主模块;
import 自我线程模块;
import 外设模块;
import 场景实时显示线程模块;
import 世界树环境模块;

class C海鱼DlgAutoProxy;
class C自我场景再现窗口;

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
	virtual BOOL PreTranslateMessage(MSG* pMsg) override;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam) override;
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
	struct 本地宿主停止状态 {
		std::atomic_bool 停止中{ false };
	};
	std::unique_ptr<自我线程类> 自我线程;
	std::unique_ptr<自治宿主类> 本地宿主_;
	本地宿主停止状态 本地宿主停止状态_{};
	std::unique_ptr<场景实时显示线程类> 场景显示线程;
	std::unique_ptr<C自我场景再现窗口> 自我场景再现窗口_;
	bool 应用退出中_ = false;
	std::atomic_bool 退出清理完成标记_{ false };
	bool 使用远程后台模式_ = false;
	bool 后台在线_ = false;
	后台前端客户端类 后台客户端_{};
	结构_后台前端快照 最近后台快照_{};
	结构_后台自我场景快照 最近后台自我场景快照_{};
	bool 最近后台自我场景快照有效_ = false;
	std::string 最近后台自我场景错误_{};
	std::wstring 后台进程路径_{};
	HANDLE 后台进程句柄_ = nullptr;
	DWORD 后台进程PID_ = 0;
	std::thread 后台连接线程_{};
	std::atomic_bool 后台连接中_{ false };
	std::atomic_bool 后台连接取消_{ false };
	std::uint64_t 本地下一个输入序号_ = 1;
	std::uint64_t 本地最近输入序号_ = 0;
	std::shared_ptr<std::string> 本地最近输入对象_{};
	std::string 本地最近输入文本_{};
	std::string 本地最近命令摘要_ = "本地内核未启动";
	bool 本地自我场景再现已启用_ = false;
	std::uint64_t 本地自我场景再现帧序号_ = 0;
	std::string 本地自我场景再现状态摘要_ = "自我场景再现未启动";

	struct 摄像机运行时;
	std::unique_ptr<摄像机运行时> 摄像机_;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg LRESULT OnApp显示参数(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClicked刷新当前页();

	// 观察导演：提示更换观测角度	
	static constexpr UINT WM_APP_观察换角度 = WM_APP + 502;
	static constexpr UINT WM_APP_后台连接完成 = WM_APP + 503;
	static constexpr UINT WM_APP_自我场景再现窗口关闭 = WM_APP + 504;
	afx_msg LRESULT OnApp观察换角度(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnApp后台连接完成(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnApp自我场景再现窗口关闭(WPARAM wParam, LPARAM lParam);
	// ===== 场景实时显示（OpenCV viz） =====
	void 启动场景实时显示();
	void 停止场景实时显示();
	void 刷新场景显示布局();

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
	void 更新主界面布局_();
	void 刷新当前界面_(bool 主动拉取远程快照);
	bool 标签页使用因果树控件_(int 标签索引) const;
	void 刷新方法树控件_();
	void 刷新因果树控件_();
	void 刷新当前标签内容();
	CString 生成标签页文本(int 标签索引) const;
	CString 生成远程标签页文本(int 标签索引) const;
	bool 标签页使用信息查看框(int 标签索引) const;
	void 清理退出子窗口();
	bool 尝试刷新后台快照();
	bool 尝试连接或拉起后台();
	bool 尝试拉起后台进程();
	std::wstring 取后台可执行路径() const;
	void 启动异步后台连接();
	void 停止后台连接线程();
	void 关闭后台进程跟踪句柄();
	bool 后台进程仍在运行() const;
	bool 强制结束已跟踪后台进程(CString* 错误文本 = nullptr);
	void 执行主窗口退出清理();
	bool 是否使用远程后台模式() const { return 使用远程后台模式_; }
	bool 后台是否在线() const { return 使用远程后台模式_ ? 后台在线_ : (本地宿主_ && 本地宿主_->是否正在运行()); }
	bool 后台是否连接中() const { return 后台连接中_.load(); }
	bool 本地内核是否停止中() const;
	CString 取远程后台状态摘要() const;
	bool 提交远程人类输入(const CString& 文本, CString* 错误文本 = nullptr);
		bool 启动远程后台(CString* 错误文本 = nullptr);
		bool 请求远程后台安全停机(CString* 错误文本 = nullptr);
		bool 运行后台任务控制回归套件(CString* 输出文本, CString* 错误文本 = nullptr);
		bool 启动远程相机观测(CString* 错误文本 = nullptr);
		bool 停止远程相机观测(CString* 错误文本 = nullptr);
		bool 启动远程自我场景再现(CString* 错误文本 = nullptr);
		bool 停止远程自我场景再现(CString* 错误文本 = nullptr);
		bool 查询远程自我场景快照(结构_后台自我场景快照* 输出, CString* 错误文本 = nullptr);
		bool 启动本地内核_(CString* 错误文本 = nullptr);
		bool 停止本地内核_(CString* 错误文本 = nullptr);
	bool 提交本地人类输入_(const CString& 文本, CString* 错误文本 = nullptr);
	bool 查询本地自我场景快照_(结构_后台自我场景快照* 输出, CString* 错误文本 = nullptr);
	结构_后台前端快照 生成本地快照_() const;
	CString 取本地内核状态摘要_() const;
	void 异步回收本地宿主_(std::unique_ptr<自治宿主类> 待停止宿主);
	void 安排应用退出兜底_();
//	void 初始化();

private:
	CEdit 变量_信息查看框;
	CButton 变量_刷新按钮;
	CTreeCtrl 变量_因果树控件;
};


extern C海鱼Dlg* 主窗口指针;





