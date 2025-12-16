
// 海鱼Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "海鱼.h"
#include "海鱼Dlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
import 世界树环境模块;
import 外设模块;
import 场景实时显示线程模块;
import 基础数据类型模块;


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
	世界树.初始化默认世界();
	自我线程.启动();

	启动外设();
	//启动场景实时显示();
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
	if constexpr (requires(自我线程类& t) { t.停止(); }) {
		自我线程.停止();
	} else if constexpr (requires(自我线程类& t) { t.请求退出(); }) {
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
	if constexpr (requires(自我线程类& t) { t.停止(); }) {
		自我线程.停止();
	} else if constexpr (requires(自我线程类& t) { t.请求退出(); }) {
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
	if constexpr (requires(自我线程类& t) { t.停止(); }) {
		自我线程.停止();
	} else if constexpr (requires(自我线程类& t) { t.请求退出(); }) {
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

	if (外设采集中.load()) {
		return;
	}

	if (!外设) {
		外设 = std::make_unique<外设类>();
	}

	外设回调允许.store(true);
	外设采集中.store(true);

	// 统一回调：外设线程把“帧 + 观测列表”推入显示线程
	auto cb = [this](std::shared_ptr<结构体_原始场景帧> f,
					 std::shared_ptr<std::vector<结构体_存在观测>> o)
	{
		if (!外设回调允许.load()) return;
		this->提交场景显示(std::move(f), std::move(o));
	};

	// 兼容两种外设接口：
	//  1) 外设.启动(cb)  —— 外设内部自带线程
	//  2) 外设.相机开始获取信息_阻塞(cb) —— 需要我们自己开线程
	auto start_impl = [&]<typename T>(T& dev)
	{
		if constexpr (requires(T& x) { x.启动(cb); }) {
			dev.启动(cb);
		}
		else if constexpr (requires(T& x) { x.相机开始获取信息_阻塞(cb); }) {
			外设采集线程 = std::thread([this, &dev, cb]() mutable {
				try {
					dev.相机开始获取信息_阻塞(cb);
				} catch (...) {}
				外设采集中.store(false);
			});
		}
		else {
			static_assert(sizeof(T) == 0, "外设类缺少启动接口：需要 启动(cb) 或 相机开始获取信息_阻塞(cb)");
		}
	};

	start_impl(*外设);
}


void C海鱼Dlg::停止外设()
{
	外设回调允许.store(false);

	if (!外设) {
		// 仍然可能有线程
		if (外设采集线程.joinable()) {
			外设采集线程.join();
		}
		外设采集中.store(false);
		return;
	}

	// 尝试通知外设退出（如果外设提供接口）
	if constexpr (requires(外设类& d) { d.请求退出(); }) {
		外设->请求退出();
	}
	else if constexpr (requires(外设类& d) { d.停止(); }) {
		外设->停止();
	}

	if (外设采集线程.joinable()) {
		外设采集线程.join();
	}

	外设采集中.store(false);
	外设.reset();
}

void C海鱼Dlg::启动场景实时显示()
{
	if (!场景显示线程) {
		场景显示线程 = std::make_unique<场景显示线程类>();
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
	if (场景显示线程) {
		// 新接口：直接传递已有的 shared_ptr（零拷贝，共享所有权）
		// 第二个参数固定为世界树当前场景根（用于渲染持久记忆中的存在）
		场景显示线程->提交(帧, 世界树.自我所在场景);
	}
}
