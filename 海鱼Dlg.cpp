// 海鱼Dlg.cpp: 实现文件
//

#include "pch.h"
#include <mutex>
#include <string>
#include <sstream>
#include <filesystem>
#include <windows.h>
#include <librealsense2/rs.hpp>
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
//import 轮廓颜色缓存线程模块;
import 相机外设基类模块;	
import 日志模块;
import 需求环境模块;
import 自我线程模块;

// ===== 调试：打印 RealSense IMU profiles（输出到 VS 输出窗口） =====

static inline void 调试输出A_(const std::string& s) {
	::OutputDebugStringA(s.c_str());
}

static inline const char* rs_stream_name_(rs2_stream s) {
	switch (s) {
	case RS2_STREAM_GYRO:  return "GYRO";
	case RS2_STREAM_ACCEL: return "ACCEL";
	case RS2_STREAM_DEPTH: return "DEPTH";
	case RS2_STREAM_COLOR: return "COLOR";
	default:               return "OTHER";
	}
}

static inline const char* rs_format_name_(rs2_format f) {
	switch (f) {
	case RS2_FORMAT_MOTION_XYZ32F: return "MOTION_XYZ32F";
	case RS2_FORMAT_Z16:           return "Z16";
	case RS2_FORMAT_RGB8:          return "RGB8";
	case RS2_FORMAT_BGR8:          return "BGR8";
	case RS2_FORMAT_RGBA8:         return "RGBA8";
	case RS2_FORMAT_BGRA8:         return "BGRA8";
	case RS2_FORMAT_YUYV:          return "YUYV";
	case RS2_FORMAT_UYVY:          return "UYVY";
	case RS2_FORMAT_ANY:           return "ANY";
	default:                       return "OTHER";
	}
}

static void 打印IMUProfiles_到输出窗口_() {
#if defined(_DEBUG)
	try {
		rs2::context ctx;
		auto devs = ctx.query_devices();
		if (devs.size() == 0) {
			调试输出A_("[IMU] no RealSense device\n");
			return;
		}

		std::ostringstream os;
		os << "[IMU] devices=" << devs.size() << "\n";
		调试输出A_(os.str());

		for (auto&& dev : devs) {
			std::ostringstream ds;
			const char* name = dev.supports(RS2_CAMERA_INFO_NAME) ? dev.get_info(RS2_CAMERA_INFO_NAME) : "?";
			const char* sn = dev.supports(RS2_CAMERA_INFO_SERIAL_NUMBER) ? dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER) : "?";
			ds << "[IMU] device: " << name << " | SN=" << sn << "\n";
			调试输出A_(ds.str());

			for (auto&& s : dev.query_sensors()) {
				std::ostringstream ss;
				const char* sname = s.supports(RS2_CAMERA_INFO_NAME) ? s.get_info(RS2_CAMERA_INFO_NAME) : "sensor";
				ss << "  sensor: " << sname;
				if (s.is<rs2::motion_sensor>()) ss << " (motion)";
				ss << "\n";
				调试输出A_(ss.str());

				if (!s.is<rs2::motion_sensor>()) continue;

				auto ps = s.get_stream_profiles();
				for (auto&& p : ps) {
					rs2_stream st = p.stream_type();
					rs2_format fm = p.format();
					int fps = p.fps();
					std::ostringstream ls;
					ls << "    " << rs_stream_name_(st)
						<< " fmt=" << rs_format_name_(fm)
						<< " fps=" << fps << "\n";
					调试输出A_(ls.str());
				}
			}
		}
	}
	catch (const rs2::error& e) {
		std::ostringstream es;
		es << "[IMU] rs2::error: " << e.what() << "\n"
			<< "      func: " << e.get_failed_function() << "\n"
			<< "      args: " << e.get_failed_args() << "\n";
		调试输出A_(es.str());
	}
	catch (const std::exception& e) {
		std::ostringstream es;
		es << "[IMU] std::exception: " << e.what() << "\n";
		调试输出A_(es.str());
	}
#endif
}

namespace {
	constexpr UINT_PTR 定时器_标签页刷新 = 1;
	constexpr UINT IDC_标签内容查看框 = 0x52A1;

	CString UTF8到CString_(const std::string& s) {
		return CString(CA2W(s.c_str(), CP_UTF8));
	}

	std::string 词键_(const 词性节点类* 词) {
		return 词 ? 词->获取主键() : std::string{};
	}

	std::string 快照文本_(const 特征快照值& v) {
		if (std::holds_alternative<I64>(v)) return std::to_string(std::get<I64>(v));
		if (std::holds_alternative<VecU句柄>(v)) return std::string("VecU");
		return std::string("<值>");
	}

	template<class TNode, class Fn>
	void 枚举根子节点_(TNode* 根, Fn&& fn) {
		if (!根 || !根->子) return;
		auto* first = static_cast<TNode*>(根->子);
		auto* it = first;
		do {
			fn(it);
			it = static_cast<TNode*>(it->下);
		} while (it && it != first);
	}

	const char* 任务状态文本_(枚举_任务状态 s) {
		switch (s) {
		case 枚举_任务状态::未启动: return "未启动";
		case 枚举_任务状态::运行中: return "运行中";
		case 枚举_任务状态::挂起: return "挂起";
		case 枚举_任务状态::完成: return "完成";
		case 枚举_任务状态::失败: return "失败";
		case 枚举_任务状态::取消: return "取消";
		case 枚举_任务状态::超时: return "超时";
		case 枚举_任务状态::就绪: return "就绪";
		case 枚举_任务状态::执行中: return "执行中";
		case 枚举_任务状态::筹办中: return "筹办中";
		case 枚举_任务状态::排队中: return "排队中";
		case 枚举_任务状态::等待中: return "等待中";
		default: return "未定义";
		}
	}

	const char* 方法来源文本_(枚举_方法沉淀来源 s) {
		switch (s) {
		case 枚举_方法沉淀来源::本能: return "本能";
		case 枚举_方法沉淀来源::反推: return "反推";
		case 枚举_方法沉淀来源::组合: return "组合";
		case 枚举_方法沉淀来源::外部导入: return "外部导入";
		case 枚举_方法沉淀来源::运行期临时: return "运行期临时";
		default: return "未定义";
		}
	}
}
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
	ON_MESSAGE(WM_APP_显示参数, &C海鱼Dlg::OnApp显示参数)
	ON_MESSAGE(C海鱼Dlg::WM_APP_观察换角度, &C海鱼Dlg::OnApp观察换角度)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// C海鱼Dlg 消息处理程序

BOOL C海鱼Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
#if defined(_DEBUG)
	打印IMUProfiles_到输出窗口_();
#endif
	// 强制初始尺寸为800x600
	CRect rect1(0, 0, 1024, 768);
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
	// 日志：运行/异常分文件按日滚动
	日志参数 lp{};
	lp.根目录 = std::filesystem::path("./日志");
	lp.文件前缀 = "海鱼";
	lp.每条刷新 = true;
	日志::初始化(lp);
	日志::运行("程序启动：日志初始化完成");
	特征值集.初始化();
	语素集.初始化();
	世界树.初始化默认世界();
	语言集.初始化();
	bool 跳过外设模式 = false;
	自我线程.启动();


	char* skipIoRaw = nullptr;
	size_t skipIoLen = 0;
	if (_dupenv_s(&skipIoRaw, &skipIoLen, "HY_ACCEPTANCE_SKIP_IO") == 0 && skipIoRaw) {
		std::string skipIo(skipIoRaw);
		std::free(skipIoRaw);
		if (skipIo == "1") {
			跳过外设模式 = true;
		}
	}

	if (!跳过外设模式) {
		启动场景实时显示();
		启动外设();
	}
	else {
		日志::运行("[验收安全链路] 外设跳过模式：跳过场景显示与外设启动");
	}

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
	变量_基础信息界面.Create(IDD_DIALOG3, GetDlgItem(IDC_TAB1));
	变量_交互界面.Create(IDD_DIALOG2, GetDlgItem(IDC_TAB1));
	变量_配置及测试窗口.Create(IDD_DIALOG1, GetDlgItem(IDC_TAB1));

	RECT rect;
	this->GetClientRect(&rect);
	rect.top = rect.top;
	变量_TAB1.MoveWindow(&rect, TRUE);
	// 变量_TAB1.GetClientRect(&rect);
	rect.bottom -= 1;
	rect.top += 22;

	变量_交互界面.MoveWindow(&rect, 1);
	变量_配置及测试窗口.MoveWindow(&rect, 1);
	变量_基础信息界面.MoveWindow(&rect, 1);
	变量_信息查看框.CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""),
		WS_CHILD | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
		rect, this, IDC_标签内容查看框);
	变量_信息查看框.SetFont(GetFont());
	//////////////////////////////////////////////////
	变量_TAB1.SetCurSel(0);
	变量_基础信息界面.ShowWindow(SW_HIDE);
	变量_交互界面.ShowWindow(SW_SHOW);
	变量_配置及测试窗口.ShowWindow(SW_HIDE);
	变量_信息查看框.ShowWindow(SW_HIDE);
	SetTimer(定时器_标签页刷新, 1000, nullptr);
	刷新当前标签内容();
	////////////////////////////////////////////


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C海鱼Dlg::更新信息查看框布局()
{
	if (!::IsWindow(变量_TAB1.GetSafeHwnd()) || !::IsWindow(变量_信息查看框.GetSafeHwnd())) return;
	CRect rect;
	变量_TAB1.GetClientRect(&rect);
	rect.top += 24;
	rect.left += 4;
	rect.right -= 4;
	rect.bottom -= 4;
	变量_信息查看框.MoveWindow(&rect, TRUE);
}

bool C海鱼Dlg::标签页使用信息查看框(int 标签索引) const
{
	switch (标签索引) {
	case 0:
	case 7:
	case 8:
		return false;
	default:
		return true;
	}
}

CString C海鱼Dlg::生成标签页文本(int 标签索引) const
{
	std::ostringstream os;
	os << "标签索引: " << 标签索引 << "\r\n\r\n";

	if (标签索引 == 1) {
		size_t count = 0;
		os << "任务信息\r\n\r\n";
		枚举根子节点_(static_cast<任务节点类*>(数据仓库模块::任务链.根指针), [&](任务节点类* n) {
			auto* mi = n ? n->主信息 : nullptr;
			if (!mi) return;
			++count;
			os << "[" << count << "] "
			   << (词键_(mi->名称).empty() ? "<未命名任务>" : 词键_(mi->名称))
			   << " | 状态=" << 任务状态文本_(mi->状态)
			   << " | 优先级=" << mi->调度优先级
			   << " | 内部存在=" << (mi->任务虚拟存在 ? "是" : "否")
			   << "\r\n";
		});
		os << "\r\n总数: " << count;
	}
	else if (标签索引 == 2) {
		size_t count = 0;
		os << "方法信息\r\n\r\n";
		枚举根子节点_(static_cast<方法节点类*>(数据仓库模块::方法链.根指针), [&](方法节点类* n) {
			auto* mi = n && n->主信息 ? dynamic_cast<方法首节点主信息类*>(n->主信息) : nullptr;
			if (!mi) return;
			++count;
			size_t 条件数 = 0, 结果数 = 0;
			std::function<void(方法节点类*)> 统计方法子树_ = [&](方法节点类* first) {
				if (!first) return;
				auto* it = first;
				do {
					auto* bmi = it && it->主信息 ? dynamic_cast<方法信息基类*>(it->主信息) : nullptr;
					if (bmi) {
						if (bmi->节点种类 == 枚举_方法节点种类::方法条件节点) ++条件数;
						if (bmi->节点种类 == 枚举_方法节点种类::方法结果节点) ++结果数;
					}
					if (it && it->子) {
						统计方法子树_(static_cast<方法节点类*>(it->子));
					}
					it = static_cast<方法节点类*>(it->下);
				} while (it && it != first);
			};
			if (n->子) {
				统计方法子树_(static_cast<方法节点类*>(n->子));
			}
			os << "[" << count << "] "
			   << (词键_(mi->动作名).empty() ? "<未命名方法>" : 词键_(mi->动作名))
			   << " | 来源=" << 方法来源文本_(mi->来源)
			   << " | 条件=" << 条件数
			   << " | 结果=" << 结果数
			   << " | 影响签名=" << mi->影响度量签名.size()
			   << "\r\n";
		});
		os << "\r\n总数: " << count;
	}
	else if (标签索引 == 5 || 标签索引 == 6) {
		size_t worldCount = 0, needCount = 0, taskCount = 0, methodCount = 0;
		枚举根子节点_(static_cast<基础信息节点类*>(数据仓库模块::世界链.根指针), [&](基础信息节点类*) { ++worldCount; });
		枚举根子节点_(static_cast<需求节点类*>(数据仓库模块::需求链.根指针), [&](需求节点类*) { ++needCount; });
		枚举根子节点_(static_cast<任务节点类*>(数据仓库模块::任务链.根指针), [&](任务节点类*) { ++taskCount; });
		枚举根子节点_(static_cast<方法节点类*>(数据仓库模块::方法链.根指针), [&](方法节点类*) { ++methodCount; });
		os << (标签索引 == 5 ? "环境 / 自我状态" : "学习 / 自我状态") << "\r\n\r\n";
		os << "自我线程运行: " << (自我线程.是否正在运行() ? "是" : "否") << "\r\n";
		os << "世界根子节点数: " << worldCount << "\r\n";
		os << "需求数: " << needCount << "\r\n";
		os << "任务数: " << taskCount << "\r\n";
		os << "方法数: " << methodCount << "\r\n";
	}
	else if (标签索引 == 9) {
		size_t count = 0;
		os << "需求列表\r\n\r\n";
		枚举根子节点_(static_cast<需求节点类*>(数据仓库模块::需求链.根指针), [&](需求节点类* n) {
			auto* mi = n ? n->主信息 : nullptr;
			if (!mi) return;
			++count;
			os << "[" << count << "] "
			   << (词键_(mi->类型).empty() ? "<未命名需求>" : 词键_(mi->类型))
			   << " | 权重=" << mi->权重
			   << " | 目标状态=" << (mi->被需求状态 ? "有" : "无")
			   << " | 相关任务=" << (mi->相关任务 ? "有" : "无")
			   << "\r\n";
		});
		os << "\r\n总数: " << count;
	}
	else if (标签索引 == 10) {
		需求节点类* best = nullptr;
		需求主信息类* bestMi = nullptr;
		枚举根子节点_(static_cast<需求节点类*>(数据仓库模块::需求链.根指针), [&](需求节点类* n) {
			auto* mi = n ? n->主信息 : nullptr;
			if (!mi) return;
			if (!bestMi || mi->权重 > bestMi->权重) { best = n; bestMi = mi; }
		});
		os << "需求窗口\r\n\r\n";
		if (!best || !bestMi) {
			os << "当前没有需求。";
		}
		else {
			auto 判断 = 需求集.判断需求状态(best, "C海鱼Dlg::生成标签页文本");
			os << "类型: " << (词键_(bestMi->类型).empty() ? "<未命名需求>" : 词键_(bestMi->类型)) << "\r\n";
			os << "权重: " << bestMi->权重 << "\r\n";
			os << "有主体: " << (bestMi->需求主体 ? "是" : "否") << "\r\n";
			os << "有目标状态: " << (bestMi->被需求状态 ? "是" : "否") << "\r\n";
			os << "已满足: " << (判断.已满足 ? "是" : "否") << "\r\n";
			os << "有现状: " << (判断.有现状 ? "是" : "否") << "\r\n";
			os << "现状值: " << (判断.有现状 ? 快照文本_(判断.现状值) : std::string("<无>")) << "\r\n";
			os << "目标值: " << (判断.有目标 ? 快照文本_(判断.目标值) : std::string("<无>")) << "\r\n";
			os << "调整方向: " << 判断.调整方向 << "\r\n";
			os << "目标差值: " << 判断.目标差值 << "\r\n";
		}
	}
	else {
		size_t needCount = 0, taskCount = 0, methodCount = 0;
		枚举根子节点_(static_cast<需求节点类*>(数据仓库模块::需求链.根指针), [&](需求节点类*) { ++needCount; });
		枚举根子节点_(static_cast<任务节点类*>(数据仓库模块::任务链.根指针), [&](任务节点类*) { ++taskCount; });
		枚举根子节点_(static_cast<方法节点类*>(数据仓库模块::方法链.根指针), [&](方法节点类*) { ++methodCount; });
		os << "当前标签页暂未单独细化，先显示系统摘要。\r\n\r\n";
		os << "自我线程运行: " << (自我线程.是否正在运行() ? "是" : "否") << "\r\n";
		os << "需求数: " << needCount << "\r\n";
		os << "任务数: " << taskCount << "\r\n";
		os << "方法数: " << methodCount << "\r\n";
	}

	return UTF8到CString_(os.str());
}

void C海鱼Dlg::刷新当前标签内容()
{
	if (!::IsWindow(变量_TAB1.GetSafeHwnd())) return;
	const int 标签索引 = 变量_TAB1.GetCurSel();
	const bool 用信息框 = 标签页使用信息查看框(标签索引);
	if (::IsWindow(变量_信息查看框.GetSafeHwnd())) {
		变量_信息查看框.ShowWindow(用信息框 ? SW_SHOW : SW_HIDE);
		if (用信息框) {
			更新信息查看框布局();
			变量_信息查看框.SetWindowTextW(生成标签页文本(标签索引));
		}
	}
}

void C海鱼Dlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 定时器_标签页刷新) {
		刷新当前标签内容();
	}
	CDialogEx::OnTimer(nIDEvent);
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
	变量_基础信息界面.MoveWindow(&rect, 1);
	变量_交互界面.MoveWindow(&rect, 1);
	变量_配置及测试窗口.MoveWindow(&rect, 1);
	变量_基础信息界面.MoveWindow(&rect, 1);
}


void C海鱼Dlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	int iIndex = 变量_TAB1.GetCurSel();

	变量_交互界面.ShowWindow(SW_HIDE);
	变量_配置及测试窗口.ShowWindow(SW_HIDE);
	变量_基础信息界面.ShowWindow(SW_HIDE);
	if (::IsWindow(变量_信息查看框.GetSafeHwnd())) {
		变量_信息查看框.ShowWindow(SW_HIDE);
	}

	switch (iIndex)
	{
	case 0:
		变量_交互界面.ShowWindow(SW_SHOW);
		break;
	case 7:
		变量_配置及测试窗口.ShowWindow(SW_SHOW);
		break;
	case 8:
		变量_基础信息界面.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}

	刷新当前标签内容();
	*pResult = 0;
}

C海鱼Dlg* 主窗口指针;

// ============================================================================
// D455 相机外设（把 D455_相机实现 适配到 外设运行框架）
// ============================================================================
namespace {
	class D455相机外设 final : public 相机外设基类 {
	public:
		D455相机外设() = default;
		// 重要：在派生析构里先停掉外设，避免基类析构阶段再触发 stop/close 导致卡死
		~D455相机外设() override { try { this->停止(); } catch (...) {} }

	protected:
		bool 启动设备(const 外设启动参数& p) override {
			if (!cam_) cam_ = std::make_unique<D455_相机实现>();
			const bool ok = cam_->打开(p);
			opened_ = ok;
			if (!ok) {
				// 打开失败：不要调用 cam_->关闭()（部分驱动在未完全打开时关闭会卡住）
				cam_.reset();
			}
			return ok;
		}

		void 停止设备() override {
			if (!cam_) { opened_ = false; return; }
			if (!opened_) {
				// 未成功打开的对象：直接释放即可（避免关闭时卡死）
				cam_.reset();
				return;
			}
			cam_->关闭();
			cam_.reset();
			opened_ = false;
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
		bool opened_{ false };
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
		外设启动参数 p{};
		p.设备ID = "";         // 自动选择
		p.期望FPS = 30;
		p.宽 = 848;
		p.高 = 480;
		p.扩展.push_back({ "align_to_color", true });

		// 优先 D455
		外设 = std::make_unique<D455相机外设>();
		bool ok = 外设->启动(p);
		if (!ok) {
			// 回退到虚拟相机
			外设.reset();
			外设 = std::make_unique<虚拟相机外设类>();
			ok = 外设->启动(p);
		}
		if (!ok) {
			// 两者都失败：不要启动 worker，避免空转/假死
			running.store(false);
			外设.reset();
			return false;
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
	void 循环_(C海鱼Dlg& dlg)
	{
		外设数据包<结构体_原始场景帧> pkg{};
		时间戳 时间1,时间2,D455时长,点簇分割时长,存在提取时长,三维场景处理时长;
		D455时长= 点簇分割时长=存在提取时长= 三维场景处理时长=0;
		日志::运行f("主窗口摄像机采集线程启动...");
		while (running.load())
		{
			auto r = 外设 ? 外设->等待下一包(pkg, 50) : 枚举_取数结果::已停止;

			if (r != 枚举_取数结果::成功) {
				if (r == 枚举_取数结果::已停止 || r == 枚举_取数结果::故障)
					break;
				continue;
			}

			// ----------------------------
			// 1) 处理：帧 -> (观测列表 + ROI列表)
			// ----------------------------
			时间1= 结构体_时间戳::当前_微秒();
			// 新版：处理一帧(帧处理结果&)
			if constexpr (requires(存在提取类 & x, 帧处理结果 & o) { 处理器.处理一帧(o); })
			{
				帧处理结果 out{};
				out.原始场景帧 = std::make_shared<结构体_原始场景帧>(std::move(pkg.数据)); // 共享指针，避免深度/点云拷贝

				if (!处理器.处理一帧(out)) {
					// 处理失败：继续下一帧
					continue;
				}

				// ----------------------------
				// 2) 写入世界树（B2：一次性加锁）
				//    注意：要雕刻必须传 frame+roiList
				// ----------------------------
				时间2 = 结构体_时间戳::当前_微秒();
				点簇分割时长 = 时间2 - 时间1 + 点簇分割时长;
				if (!out.轮廓观测列表.empty() && out.轮廓观测列表.size() == out.存在观测列表.size())
				{
					三维场景管理类::匹配参数 p{};
					// 能力1：观察导演
				//	p.仅处理主观测 = true;
				//	p.主观测_无增量阈值帧 = 30;
				//	p.主观测_无可观测阈值帧 = 20;
				//	p.主观测_提示冷却帧 = 150;
				//	场景管理.处理观测列表(out.存在观测列表, p);

					// 若需要提示更换观测角度，转到 UI 线程弹窗
				//	if (场景管理.取并清空_需要更换观测角度提示()) {
				//		dlg.PostMessage(C海鱼Dlg::WM_APP_观察换角度, 0, 0);
				//	}

					// 性能/融合统计日志（每30帧一次）
					static std::uint64_t 统计计数 = 0;
					++统计计数;
				//	auto st = 场景管理.获取最近世界轮廓融合统计();
				//	if (统计计数 % 30 == 0) {
				//		const auto& f = *out.原始场景帧;
				//		const std::uint32_t fn = f.时间戳.深度帧号;
				//		const double rate = (st.总数 > 0) ? (double)st.成功数 / (double)st.总数 : 0.0;
				//		日志::运行f("帧={} 融合={}/{}({:.2f}) 抖动:dt={:.1f}mm dR={:.2f}deg", fn, st.成功数, st.总数, rate, st.平移抖动_mm, st.旋转抖动_deg);
					}
				}



				// ----------------------------
				// 3) 推送显示（把帧和观测 move 给 UI，零拷贝共享）
				// ----------------------------
				时间1 = 结构体_时间戳::当前_微秒();
		//		auto 帧Ptr = out.原始场景帧;
		//		auto 观测Ptr = std::make_shared<std::vector<结构体_存在观测>>(std::move(out.存在观测列表));

		//		dlg.提交场景显示(std::move(帧Ptr), std::move(观测Ptr));

				// 如果你希望 UI 也能画 2D ROI/轮廓，可扩展 dlg 接口：
				// auto roiPtr = std::make_shared<std::vector<结构体_轮廓观测>>(std::move(out.轮廓观测列表));
				// dlg.提交轮廓显示(std::move(roiPtr));
			}

		}
	//	日志::运行f("主窗口摄像机采集线程退出...");
//	}

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
	//启动场景实时显示();

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


void C海鱼Dlg::锁定观测目标(const std::string& 存在主键)
{
	if (摄像机_) {
	//	摄像机_->场景管理.锁定观测目标(存在主键);
	}
}

void C海鱼Dlg::解除锁定观测()
{
	if (摄像机_) {
	//	摄像机_->场景管理.解除锁定观测();
	}
}


void C海鱼Dlg::启动场景实时显示()
{
	if (!场景显示线程) {
		场景显示线程 = std::make_unique<场景实时显示线程类>();
		变量_配置及测试窗口.绑定显示线程(场景显示线程.get());
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

	auto pkg = std::make_shared<帧处理结果>();

	// ✅ 零拷贝：shared_ptr<T> -> shared_ptr<const T> 可隐式转换
	pkg->原始场景帧 = std::move(帧);

	// ✅ 必须提交：彩色轮廓优先来自 结构体_存在观测
	// 当前帧处理结果里是 vector，所以这里会拷贝一次（推荐且安全）
	if (观测) {
		pkg->存在观测列表 = *观测;   // 拷贝 vector 内容
	}
	else {
		pkg->存在观测列表.clear();
	}

//	场景显示线程->更新帧(std::move(pkg));
}



LRESULT C海鱼Dlg::OnApp显示参数(WPARAM wParam, LPARAM lParam)
{
	const bool on = (lParam != 0);

	// 你这里换成你真正持有的渲染器对象
	//if (!场景实时显示器) return 0;
	// 关键：把配置窗口绑定到显示线程


	//场景显示线程->更新显示参数([&](场景显示参数& p) {
	//	switch ((枚举_显示参数命令)wParam)
	//	{
	//	case 枚举_显示参数命令::显示坐标轴:         p.显示坐标轴 = on; break;
	//	case 枚举_显示参数命令::显示地面网格:       p.显示网格地面 = on; break;
	//	case 枚举_显示参数命令::显示实时点云:       p.显示实时点云 = on; break;
	//	case 枚举_显示参数命令::显示存在框:         p.显示存在框 = on; break;
	//	case 枚举_显示参数命令::显示存在主轴:       p.显示存在主轴 = on; break;
	//	case 枚举_显示参数命令::显示存在点云簇:     p.显示存在点云簇 = on; break;
	//	case 枚举_显示参数命令::显示体素网格:       p.显示体素网格 = on; break;
	//	case 枚举_显示参数命令::网格仅表面:         p.网格仅表面 = on; break;
	//	case 枚举_显示参数命令::使用相机坐标系映射: p.使用相机坐标系映射 = on; break;
	//	case 枚举_显示参数命令::点云使用颜色:       p.点云使用颜色 = on; break;
	//	case 枚举_显示参数命令::坐标模式_绝对:      p.坐标模式 = on ? 1 : 0; break;
	//	default: break;
	//	}
	//	});

	return 0;
}

LRESULT C海鱼Dlg::OnApp观察换角度(WPARAM, LPARAM)
{
	AfxMessageBox(L"需要更换观测角度");
	return 0;
}












