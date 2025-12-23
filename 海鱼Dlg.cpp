
// 海鱼Dlg.cpp: 实现文件
//

#include "pch.h"
#include <mutex>
#include <string>
#include "framework.h"
#include "海鱼.h"
#include "海鱼Dlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
import 特征值环境模块;
import 语素环境模块;
import 世界树环境模块;
import 语言环境模块;

import 外设模块;
import 场景实时显示线程模块;
import 基础数据类型模块;

import 数据仓库模块;
import 虚拟相机外设模块;
import 相机写入工作流模块;
import 三维场景管理模块;
import D455相机模块;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// C海鱼Dlg 对话框


IMPLEMENT_DYNAMIC(C海鱼Dlg, CDialogEx);

C海鱼Dlg::C海鱼Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MY_DIALOG, pParent)
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = nullptr;
}

C海鱼Dlg::~C海鱼Dlg()
{
	停止外设();
	停止场景实时显示();

	// 如果该对话框有自动化代理，则
	//  此对话框的返回指针为 null，所以它知道
	//  此代理知道该对话框已被删除。
	if (m_pAutoProxy != nullptr)
		m_pAutoProxy->m_pDialog = nullptr;
}

void C海鱼Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, 变量_TAB1);
}

BEGIN_MESSAGE_MAP(C海鱼Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &C海鱼Dlg::OnSelchangeTab1)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// C海鱼Dlg 消息处理程序

BOOL C海鱼Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 强制初始尺寸为800x600
	CRect rect1(0, 0, 800, 600);
	CalcWindowRect(&rect1); // 转换为客户区坐标到窗口坐标
	SetWindowPos(NULL, 0, 0, rect1.Width(), rect1.Height(), SWP_NOZORDER | SWP_NOMOVE);
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	// SetWindowPos(NULL, 0, 0, 800, 600, SWP_NOZORDER | SWP_NOMOVE);
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	//初始化();
	// TODO: 在此添加额外的初始化代码
	主窗口指针 = this;
	特征值集.初始化();
	语素集.初始化();
	世界树.初始化默认世界();
	语言集.初始化();
	自我线程.启动();

	启动外设();
	启动场景实时显示();
	变量_TAB1.InsertItem(0, _T("交互界面"));
	变量_TAB1.InsertItem(1, _T("任务信息"));
	变量_TAB1.InsertItem(2, _T("方法信息"));
	变量_TAB1.InsertItem(3, _T("事件信息"));
	变量_TAB1.InsertItem(4, _T("因果信息"));
	变量_TAB1.InsertItem(5, _T("环境信息"));
	变量_TAB1.InsertItem(6, _T("学习信息"));
	变量_TAB1.InsertItem(7, _T("配置级测试"));
	变量_TAB1.InsertItem(8, _T("基础信息"));
	变量_TAB1.InsertItem(9, _T("需求列表窗口"));
	变量_TAB1.InsertItem(10, _T("需求窗口"));


	/////////////////////////////////////////////
	变量_交互界面.Create(IDD_DIALOG2, GetDlgItem(IDC_TAB1));


	RECT rect;
	this->GetClientRect(&rect);
	rect.top = rect.top;
	变量_TAB1.MoveWindow(&rect, TRUE);
	// 变量_TAB1.GetClientRect(&rect);
	rect.bottom -= 1;
	rect.top += 22;

	变量_交互界面.MoveWindow(&rect, 1);

	//////////////////////////////////////////////////
	变量_交互界面.ShowWindow(SW_SHOW);
	////////////////////////////////////////////


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C海鱼Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C海鱼Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C海鱼Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 当用户关闭 UI 时，如果控制器仍保持着它的某个
//  对象，则自动化服务器不应退出。  这些
//  消息处理程序确保如下情形: 如果代理仍在使用，
//  则将隐藏 UI；但是在关闭对话框时，
//  对话框仍然会保留在那里。

void C海鱼Dlg::OnClose()
{
	if (!CanExit()) {
		return;
	}
	停止外设();
	停止场景实时显示();
	// 视情况停止自我线程（如果提供了接口）
	if constexpr (requires(自我线程类 & t) { t.停止(); }) {
		自我线程.停止();
	}
	else if constexpr (requires(自我线程类 & t) { t.请求退出(); }) {
		自我线程.请求退出();
	}
	CDialogEx::OnClose();
}

void C海鱼Dlg::OnOK()
{
	if (!CanExit()) {
		return;
	}
	停止外设();
	停止场景实时显示();
	// 视情况停止自我线程（如果提供了接口）
	if constexpr (requires(自我线程类 & t) { t.停止(); }) {
		自我线程.停止();
	}
	else if constexpr (requires(自我线程类 & t) { t.请求退出(); }) {
		自我线程.请求退出();
	}
	CDialogEx::OnOK();
}

void C海鱼Dlg::OnCancel()
{
	if (!CanExit()) {
		return;
	}
	停止外设();
	停止场景实时显示();
	// 视情况停止自我线程（如果提供了接口）
	if constexpr (requires(自我线程类 & t) { t.停止(); }) {
		自我线程.停止();
	}
	else if constexpr (requires(自我线程类 & t) { t.请求退出(); }) {
		自我线程.请求退出();
	}
	CDialogEx::OnCancel();
}

BOOL C海鱼Dlg::CanExit()
{
	// 如果代理对象仍保留在那里，则自动化
	//  控制器仍会保持此应用程序。
	//  使对话框保留在那里，但将其 UI 隐藏起来。
	if (m_pAutoProxy != nullptr)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}


void C海鱼Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	if (变量_交互界面.m_hWnd == NULL)
		return;
	if (变量_TAB1.m_hWnd == NULL)
		return;      // Return if window is not created yet.
	RECT rect;

	/////////// TAB控件变量
	rect.bottom = cy - 18;
	rect.top = 1;
	rect.left = 1;
	rect.right = cx - 18;
	//	变量_TAB1.AdjustRect(FALSE, &rect);
		// Move the tab control to the new position and size.
	变量_TAB1.MoveWindow(&rect, TRUE);
	////////////////////////////////////////////////////
	rect.bottom -= 1;
	rect.top += 22;
	变量_交互界面.MoveWindow(&rect, 1);
}


void C海鱼Dlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int iIndex = 变量_TAB1.GetCurSel();
	switch (iIndex)
	{
	case -1:
	{
		break;
	}
	case 0:
	{
		变量_交互界面.ShowWindow(SW_SHOW);
		//		变量_测试窗口.ShowWindow(SW_HIDE);
		//		变量_任务窗口.ShowWindow(SW_HIDE);
		//		变量_方法窗口.ShowWindow(SW_HIDE);
		//		变量_事件窗口.ShowWindow(SW_HIDE);
		//		变量_环境窗口.ShowWindow(SW_HIDE);
		//		变量_基础信息窗口.ShowWindow(SW_HIDE);
		//		变量_因果窗口.ShowWindow(SW_HIDE);
		//		变量_学习窗口.ShowWindow(SW_HIDE);
		//		变量_需求列表窗口.ShowWindow(SW_HIDE);
		//		变量_需求窗口.ShowWindow(SW_HIDE);
				//  变量_交互界面.ShowWindow(SW_HIDE);
				//  变量_交互界面.ShowWindow(SW_HIDE);
		break;
	}
	case 1:
	{
		//		变量_基础信息窗口.ShowWindow(SW_HIDE);
		//		变量_测试窗口.ShowWindow(SW_HIDE);
		//		变量_任务窗口.ShowWindow(SW_SHOW);
		//		变量_方法窗口.ShowWindow(SW_HIDE);
		//		变量_事件窗口.ShowWindow(SW_HIDE);
		//		变量_环境窗口.ShowWindow(SW_HIDE);
		变量_交互界面.ShowWindow(SW_HIDE);
		//		变量_因果窗口.ShowWindow(SW_HIDE);
		//		变量_学习窗口.ShowWindow(SW_HIDE);
		//		变量_需求列表窗口.ShowWindow(SW_HIDE);
		//		变量_需求窗口.ShowWindow(SW_HIDE);
				//  变量_交互界面.ShowWindow(SW_HIDE);
				//  变量_交互界面.ShowWindow(SW_HIDE);
		break;
	}
	case 2:
	{
		//		变量_基础信息窗口.ShowWindow(SW_HIDE);
		//		变量_测试窗口.ShowWindow(SW_HIDE);
		//		变量_任务窗口.ShowWindow(SW_HIDE);
		//		变量_方法窗口.ShowWindow(SW_SHOW);
		//		变量_事件窗口.ShowWindow(SW_HIDE);
		//		变量_环境窗口.ShowWindow(SW_HIDE);
		变量_交互界面.ShowWindow(SW_HIDE);
		//		变量_因果窗口.ShowWindow(SW_HIDE);
		//		变量_学习窗口.ShowWindow(SW_HIDE);
		//		变量_需求列表窗口.ShowWindow(SW_HIDE);
		//		变量_需求窗口.ShowWindow(SW_HIDE);
				//  变量_交互界面.ShowWindow(SW_HIDE);
				//  变量_交互界面.ShowWindow(SW_HIDE);
		break;
	}
	case 3:
	{
		//		变量_基础信息窗口.ShowWindow(SW_HIDE);
		//		变量_测试窗口.ShowWindow(SW_HIDE);
		//		变量_任务窗口.ShowWindow(SW_HIDE);
		//		变量_方法窗口.ShowWindow(SW_HIDE);
		//		变量_事件窗口.ShowWindow(SW_SHOW);
		//		变量_环境窗口.ShowWindow(SW_HIDE);
		变量_交互界面.ShowWindow(SW_HIDE);
		//		变量_因果窗口.ShowWindow(SW_HIDE);
		//		变量_学习窗口.ShowWindow(SW_HIDE);
		//		变量_需求列表窗口.ShowWindow(SW_HIDE);
		//		变量_需求窗口.ShowWindow(SW_HIDE);
				//  变量_交互界面.ShowWindow(SW_HIDE);
				//  变量_交互界面.ShowWindow(SW_HIDE);
		break;
	}
	case 4:
	{
		//		变量_基础信息窗口.ShowWindow(SW_HIDE);
		//		变量_测试窗口.ShowWindow(SW_HIDE);
		//		变量_任务窗口.ShowWindow(SW_HIDE);
		//		变量_方法窗口.ShowWindow(SW_HIDE);
		//		变量_事件窗口.ShowWindow(SW_HIDE);
		//		变量_因果窗口.ShowWindow(SW_SHOW);
		//		变量_环境窗口.ShowWindow(SW_HIDE);
		变量_交互界面.ShowWindow(SW_HIDE);
		//		变量_学习窗口.ShowWindow(SW_HIDE);
		//		变量_需求列表窗口.ShowWindow(SW_HIDE);
		//		变量_需求窗口.ShowWindow(SW_HIDE);
				//  变量_交互界面.ShowWindow(SW_HIDE);
				//  变量_交互界面.ShowWindow(SW_HIDE);

		break;
	}
	case 5:
	{
		//		变量_基础信息窗口.ShowWindow(SW_HIDE);
		//		变量_任务窗口.ShowWindow(SW_HIDE);
		//		变量_方法窗口.ShowWindow(SW_HIDE);
		//		变量_事件窗口.ShowWindow(SW_HIDE);
		//		变量_因果窗口.ShowWindow(SW_HIDE);
		//		变量_环境窗口.ShowWindow(SW_SHOW);
		//		变量_测试窗口.ShowWindow(SW_HIDE);
		变量_交互界面.ShowWindow(SW_HIDE);
		//		变量_学习窗口.ShowWindow(SW_HIDE);
		//		变量_需求列表窗口.ShowWindow(SW_HIDE);
		//		变量_需求窗口.ShowWindow(SW_HIDE);
				//  变量_交互界面.ShowWindow(SW_HIDE);
				//  变量_交互界面.ShowWindow(SW_HIDE);
		break;
	}
	case 6:
	{
		//		变量_基础信息窗口.ShowWindow(SW_HIDE);
		//		变量_任务窗口.ShowWindow(SW_HIDE);
		//		变量_方法窗口.ShowWindow(SW_HIDE);
		//		变量_事件窗口.ShowWindow(SW_HIDE);
		//		变量_因果窗口.ShowWindow(SW_HIDE);
		//		变量_环境窗口.ShowWindow(SW_HIDE);
		//		变量_测试窗口.ShowWindow(SW_HIDE);
		变量_交互界面.ShowWindow(SW_HIDE);
		//		变量_学习窗口.ShowWindow(SW_SHOW);
		//		变量_需求列表窗口.ShowWindow(SW_HIDE);
		//		变量_需求窗口.ShowWindow(SW_HIDE);
				//  变量_交互界面.ShowWindow(SW_HIDE);
				//  变量_交互界面.ShowWindow(SW_HIDE);
		break;
	}
	case 7:
	{
		//		变量_基础信息窗口.ShowWindow(SW_HIDE);
		//		变量_任务窗口.ShowWindow(SW_HIDE);
		//		变量_方法窗口.ShowWindow(SW_HIDE);
		//		变量_事件窗口.ShowWindow(SW_HIDE);
		//		变量_因果窗口.ShowWindow(SW_HIDE);
		//		变量_环境窗口.ShowWindow(SW_HIDE);
		//		变量_测试窗口.ShowWindow(SW_SHOW);
		变量_交互界面.ShowWindow(SW_HIDE);
		//		变量_学习窗口.ShowWindow(SW_HIDE);
		//		变量_需求列表窗口.ShowWindow(SW_HIDE);
		//		变量_需求窗口.ShowWindow(SW_HIDE);
				//  变量_交互界面.ShowWindow(SW_HIDE);
				//  变量_交互界面.ShowWindow(SW_HIDE);
		break;
	}

	case 8:
	{
		变量_交互界面.ShowWindow(SW_HIDE);
		//		变量_任务窗口.ShowWindow(SW_HIDE);
		//		变量_方法窗口.ShowWindow(SW_HIDE);
		//		变量_事件窗口.ShowWindow(SW_HIDE);
		//		变量_因果窗口.ShowWindow(SW_HIDE);
		//		变量_环境窗口.ShowWindow(SW_HIDE);
		//		变量_测试窗口.ShowWindow(SW_HIDE);
		//		变量_基础信息窗口.ShowWindow(SW_SHOW);
		//		变量_学习窗口.ShowWindow(SW_HIDE);
		//		变量_需求列表窗口.ShowWindow(SW_HIDE);
		//		变量_需求窗口.ShowWindow(SW_HIDE);
		//		//  变量_交互界面.ShowWindow(SW_HIDE);
				//  变量_交互界面.ShowWindow(SW_HIDE);
		break;

	case 9:
	{
		//		变量_基础信息窗口.ShowWindow(SW_HIDE);
		//		变量_任务窗口.ShowWindow(SW_HIDE);
		//		变量_方法窗口.ShowWindow(SW_HIDE);
		//		变量_事件窗口.ShowWindow(SW_HIDE);
		//		变量_因果窗口.ShowWindow(SW_HIDE);
		//		变量_环境窗口.ShowWindow(SW_HIDE);
		//		变量_测试窗口.ShowWindow(SW_HIDE);
		变量_交互界面.ShowWindow(SW_HIDE);
		//		变量_学习窗口.ShowWindow(SW_HIDE);
		//		变量_需求列表窗口.ShowWindow(SW_SHOW);
		//		变量_需求窗口.ShowWindow(SW_HIDE);
				//  变量_交互界面.ShowWindow(SW_HIDE);
				//  变量_交互界面.ShowWindow(SW_HIDE);
		break;
	}
	case 10:
	{
		//		变量_基础信息窗口.ShowWindow(SW_HIDE);
		//		变量_任务窗口.ShowWindow(SW_HIDE);
		//		变量_方法窗口.ShowWindow(SW_HIDE);
		//		变量_事件窗口.ShowWindow(SW_HIDE);
		//		变量_因果窗口.ShowWindow(SW_HIDE);
		//		变量_环境窗口.ShowWindow(SW_HIDE);
		//		变量_测试窗口.ShowWindow(SW_HIDE);
		变量_交互界面.ShowWindow(SW_HIDE);
		//		变量_学习窗口.ShowWindow(SW_HIDE);
		//		变量_需求列表窗口.ShowWindow(SW_HIDE);
		//		变量_需求窗口.ShowWindow(SW_SHOW);
				//  变量_交互界面.ShowWindow(SW_HIDE);
				//  变量_交互界面.ShowWindow(SW_HIDE);
		break;
	}
	default:
	{
		break;
	}
	}
	*pResult = 0;
	}
}



C海鱼Dlg* 主窗口指针;

// ============================================================================
// D455 相机外设（把 D455_相机实现 适配到 外设运行框架）
// ============================================================================
namespace {
	class D455相机外设 final : public 相机外设基类 {
	public:
		D455相机外设() = default;
		~D455相机外设() override { try { 停止设备(); } catch (...) {} }

	protected:
		bool 启动设备(const 外设启动参数& p) override {
			if (!cam_) cam_ = std::make_unique<D455_相机实现>();
			return cam_->打开(p);
		}

		void 停止设备() override {
			if (cam_) {
				cam_->关闭();
				cam_.reset();
			}
		}

		外设指令回执 处理指令_设备(const 外设指令& cmd) override {
			if (!cam_) return { false, -1, "D455 未启动" };
			return cam_->处理外设指令(cmd);
		}

		bool 抓取数据_阻塞(结构体_原始场景帧& out) override {
			if (!cam_) return false;
			return cam_->采集一帧(out);
		}

		时间戳 取设备时间戳(const 结构体_原始场景帧& d) const override {
			if (cam_) return cam_->获取最后设备时间_微秒();
			return 相机外设基类::取设备时间戳(d);
		}

	private:
		std::unique_ptr<D455_相机实现> cam_{};
	};
} // namespace

// ============================================================================
// C海鱼Dlg::摄像机运行时（采集/处理/写入世界树 + 推送显示）
// ============================================================================
struct C海鱼Dlg::摄像机运行时 {
	三维场景管理类 场景管理{};
	相机帧处理器   处理器{};

	std::unique_ptr<相机外设基类> 外设{};
	std::thread worker{};
	std::atomic_bool running{ false };

	bool 启动(C海鱼Dlg& dlg) {
		if (running.exchange(true)) return false;

		// 1) 创建外设（优先 D455，失败则退化为虚拟相机）
		外设 = std::make_unique<D455相机外设>();

		外设启动参数 p{};
		p.设备ID = "";         // 自动选择
		p.期望FPS = 30;
		p.宽 = 640;
		p.高 = 480;
		p.扩展.push_back({ "align_to_color", true });

		if (!外设->启动(p)) {
			外设.reset();
			外设 = std::make_unique<虚拟相机外设类>();
			(void)外设->启动(p);
		}

		// 2) 处理线程：等帧 -> 提取观测 -> 写入世界树 -> 推送显示
		worker = std::thread([this, &dlg] { 循环_(dlg); });
		return true;
	}

	void 停止() {
		running.store(false);

		if (外设) {
			外设->停止();
		}
		if (worker.joinable()) {
			worker.join();
		}
		外设.reset();
	}

private:
	void 循环_(C海鱼Dlg& dlg) {
		外设数据包<结构体_原始场景帧> pkg{};

		while (running.load()) {
			auto r = 外设 ? 外设->等待下一包(pkg, 50) : 枚举_取数结果::已停止;

			if (r != 枚举_取数结果::成功) {
				if (r == 枚举_取数结果::已停止 || r == 枚举_取数结果::故障) break;
				continue;
			}

			// 把帧转成 shared_ptr，便于显示线程共享（零拷贝移动）
			auto 帧 = std::make_shared<结构体_原始场景帧>(std::move(pkg.数据));

			// 处理：帧 -> 观测列表
			std::vector<结构体_存在观测> obs{};
			(void)处理器.处理一帧(*帧, obs);

			// 写入世界树（B2：一次性加锁）
			{
				std::scoped_lock lk(世界链.链表锁, 特征值链.链表锁);
				场景管理.处理观测列表(obs);
			}

			auto 观测 = std::make_shared<std::vector<结构体_存在观测>>(std::move(obs));

			// 推送显示
			dlg.提交场景显示(std::move(帧), std::move(观测));
		}
	}
};

void C海鱼Dlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	lpMMI->ptMinTrackSize.x = 800; // 最小宽度
	lpMMI->ptMinTrackSize.y = 600; // 最小高度
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void C海鱼Dlg::启动外设()
{
	// 先确保显示线程存在（否则提交无意义）
	启动场景实时显示();

	if (!摄像机_) {
		摄像机_ = std::make_unique<摄像机运行时>();
	}
	摄像机_->启动(*this);
}


void C海鱼Dlg::停止外设()
{
	if (摄像机_) {
		摄像机_->停止();
		摄像机_.reset();
	}
}

void C海鱼Dlg::启动场景实时显示()
{
	if (!场景显示线程) {
		场景显示线程 = std::make_unique<场景实时显示线程类>();
		场景显示线程->启动();
	}

}

void C海鱼Dlg::停止场景实时显示()
{
	if (场景显示线程) {
		场景显示线程->停止();
		场景显示线程.reset();
	}
}


void C海鱼Dlg::提交场景显示(
	std::shared_ptr<结构体_原始场景帧> 帧,
	std::shared_ptr<std::vector<结构体_存在观测>> 观测)
{
	if (!场景显示线程) return;

	// 直接调用唯一的重载版本，解引用 shared_ptr 传递引用
	场景显示线程->更新帧(*帧);
}

