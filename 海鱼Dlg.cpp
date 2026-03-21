// 海鱼Dlg.cpp: 实现文件
//

#include "pch.h"
#include <mutex>
#include <string>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <variant>
#include <windows.h>
#include <librealsense2/rs2.hpp>
#include "framework.h"
#include "海鱼.h"
#include "海鱼Dlg.h"
#include "自我场景再现窗口.h"
#include "DlgProxy.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
import 特征值环境模块;
import 后台前端通道模块;
import 语素环境模块;
import 世界树环境模块;
import 语言环境模块;
import 特征类型定义模块;
import 本能动作管理模块;

import 外设模块;
import 场景实时显示线程模块;
import 基础数据类型模块;

import 数据仓库模块;
import 虚拟相机外设模块;
import 相机写入工作流模块;
import 相机观测运行时模块;
import 三维场景管理模块;
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
	void 确保窗口落在可见工作区_(CWnd* wnd)
	{
		if (wnd == nullptr || !::IsWindow(wnd->GetSafeHwnd())) return;

		wnd->CenterWindow();

		CRect 当前窗口区域;
		wnd->GetWindowRect(&当前窗口区域);

		MONITORINFO 监视器信息{};
		监视器信息.cbSize = sizeof(监视器信息);
		const HMONITOR 监视器 = ::MonitorFromRect(&当前窗口区域, MONITOR_DEFAULTTONEAREST);
		if (监视器 == nullptr || !::GetMonitorInfoW(监视器, &监视器信息)) return;

		const RECT& 工作区 = 监视器信息.rcWork;
		const int 窗口宽度 = 当前窗口区域.Width();
		const int 窗口高度 = 当前窗口区域.Height();

		int 目标左 = 当前窗口区域.left;
		int 目标上 = 当前窗口区域.top;

		if (窗口宽度 >= (工作区.right - 工作区.left)) {
			目标左 = 工作区.left;
		}
		else {
			if (目标左 < 工作区.left) 目标左 = 工作区.left;
			if (目标左 + 窗口宽度 > 工作区.right) 目标左 = 工作区.right - 窗口宽度;
		}

		if (窗口高度 >= (工作区.bottom - 工作区.top)) {
			目标上 = 工作区.top;
		}
		else {
			if (目标上 < 工作区.top) 目标上 = 工作区.top;
			if (目标上 + 窗口高度 > 工作区.bottom) 目标上 = 工作区.bottom - 窗口高度;
		}

		wnd->SetWindowPos(nullptr, 目标左, 目标上, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		wnd->ShowWindow(SW_SHOWNORMAL);
		wnd->BringWindowToTop();
	}

	void 安全销毁窗口_(CWnd& wnd) noexcept
	{
		if (!::IsWindow(wnd.GetSafeHwnd())) return;
		try {
			wnd.DestroyWindow();
		}
		catch (...) {
		}
	}

	void 隐藏子页面关闭控件_(CWnd& wnd) noexcept
	{
		if (!::IsWindow(wnd.GetSafeHwnd())) return;
		if (CWnd* 取消按钮 = wnd.GetDlgItem(IDCANCEL);
			取消按钮 && ::IsWindow(取消按钮->GetSafeHwnd())) {
			取消按钮->EnableWindow(FALSE);
			取消按钮->ShowWindow(SW_HIDE);
		}
	}

	constexpr UINT_PTR 定时器_标签页刷新 = 1;
	constexpr UINT IDC_标签内容查看框 = 0x52A1;

	void 提示控制面板不直接管理外设_(CWnd* 宿主窗口)
	{
		AfxMessageBox(_T("鱼巢控制面板不再直接启动本地外设、场景显示或自我线程。\n请先连接或启动后台“鱼巢”，再通过后台统一查看和管理运行状态。"));
		if (宿主窗口 && ::IsWindow(宿主窗口->GetSafeHwnd())) {
			宿主窗口->SetForegroundWindow();
		}
	}

	struct 结构_异步后台连接结果 {
		bool 成功 = false;
		结构_后台前端快照 快照{};
		std::string 日志摘要;
		std::wstring 已启动后台路径{};
		HANDLE 已启动进程句柄 = nullptr;
		DWORD 已启动进程PID = 0;
	};

	std::filesystem::path 准备后台运行时副本_(const std::wstring& 后台路径)
	{
		if (后台路径.empty()) {
			return {};
		}

		const std::filesystem::path 源文件路径(后台路径);
		const std::filesystem::path 源目录 = 源文件路径.parent_path();
		const std::filesystem::path 运行时根目录 = 源目录 / L"鱼巢_runtime";

		std::wstringstream 目录名;
		目录名 << L"session_" << ::GetCurrentProcessId() << L"_" << ::GetTickCount64();
		const std::filesystem::path 运行时目录 = 运行时根目录 / 目录名.str();
		std::filesystem::create_directories(运行时目录);

		std::filesystem::copy_file(
			源文件路径,
			运行时目录 / 源文件路径.filename(),
			std::filesystem::copy_options::overwrite_existing);

		for (const auto& entry : std::filesystem::directory_iterator(源目录)) {
			if (!entry.is_regular_file()) continue;
			const auto 扩展名 = entry.path().extension().wstring();
			if (_wcsicmp(扩展名.c_str(), L".dll") == 0 ||
				_wcsicmp(扩展名.c_str(), L".json") == 0) {
				std::filesystem::copy_file(
					entry.path(),
					运行时目录 / entry.path().filename(),
					std::filesystem::copy_options::overwrite_existing);
			}
		}

		return 运行时目录 / 源文件路径.filename();
	}

	bool 按路径拉起后台进程_(
		const std::wstring& 后台路径,
		std::wstring* 实际启动路径 = nullptr,
		HANDLE* 进程句柄 = nullptr,
		DWORD* 进程PID = nullptr)
	{
		if (后台路径.empty()) {
			日志::运行("[前端界面] 未找到鱼巢.exe，无法自动拉起后台");
			return false;
		}

		const std::filesystem::path 运行时后台路径 = 准备后台运行时副本_(后台路径);
		if (运行时后台路径.empty()) {
			日志::运行("[前端界面] 后台运行时副本准备失败");
			return false;
		}

		STARTUPINFOW si{};
		si.cb = sizeof(si);
		PROCESS_INFORMATION pi{};
		std::wstring 命令行 = L"\"" + 运行时后台路径.wstring() + L"\"";
		BOOL ok = ::CreateProcessW(
			nullptr,
			命令行.data(),
			nullptr,
			nullptr,
			FALSE,
			CREATE_NO_WINDOW,
			nullptr,
			nullptr,
			&si,
			&pi);
		if (!ok) {
			日志::运行("[前端界面] 自动拉起后台失败");
			return false;
		}

		::CloseHandle(pi.hThread);
		if (实际启动路径) {
			*实际启动路径 = 运行时后台路径.wstring();
		}
		if (进程句柄) {
			*进程句柄 = pi.hProcess;
		}
		else {
			::CloseHandle(pi.hProcess);
		}
		if (进程PID) {
			*进程PID = pi.dwProcessId;
		}
		日志::运行("[前端界面] 已自动拉起鱼巢 后台进程");
		return true;
	}

	CString UTF8到CString_(const std::string& s) {
		return CString(CA2W(s.c_str(), CP_UTF8));
	}

	std::string 取词文本_(const 词性节点类* 词) {
		if (!词 || !词->父 || !词->父->主信息) return {};
		auto* 词信息 = dynamic_cast<词主信息类*>(词->父->主信息);
		return 词信息 ? 词信息->词 : std::string{};
	}

	std::string 词键_(const 词性节点类* 词) {
		return 词 ? 词->获取主键() : std::string{};
	}

	std::size_t 计数同层节点_(const 基础信息节点类* 头节点) {
		if (!头节点) return 0;
		std::size_t 数量 = 0;
		auto* 当前 = 头节点;
		do {
			++数量;
			当前 = 当前->下;
		} while (当前 && 当前 != 头节点);
		return 数量;
	}

	std::string 主要需求类型显示文本_(std::string 文本) {
		if (文本.empty()) return {};
		const auto 服务 = std::string("服务");
		const auto 安全 = std::string("安全");
		const auto 学习 = std::string("学习");
		const auto 需求后缀 = std::string("需求");

		if (文本.find(服务) != std::string::npos) return 服务;
		if (文本.find(安全) != std::string::npos) return 安全;
		if (文本.find(学习) != std::string::npos) return 学习;
		if (文本.size() > 需求后缀.size()
			&& 文本.compare(文本.size() - 需求后缀.size(), 需求后缀.size(), 需求后缀) == 0) {
			文本.resize(文本.size() - 需求后缀.size());
		}
		return 文本;
	}

	std::string 主要需求摘要文本_(const 结构_后台前端快照& 快照) {
		auto 类型 = 主要需求类型显示文本_(快照.主要需求类型);
		if (类型.empty()) return std::string("无");

		std::ostringstream os;
		os << 类型;
		if (快照.主要需求权重 != 0) {
			os << " (权重=" << 快照.主要需求权重
				<< ", " << (快照.主要需求已满足 ? "已满足" : "未满足") << ")";
		}
		return os.str();
	}

	I64 当前UTC毫秒_() {
		return static_cast<I64>(std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()).count());
	}

	std::string 快照新鲜度文本_(const 结构_后台前端快照& 快照) {
		if (快照.快照生成UTC毫秒 <= 0) return std::string("未知");
		I64 延迟毫秒 = 当前UTC毫秒_() - 快照.快照生成UTC毫秒;
		if (延迟毫秒 < 0) 延迟毫秒 = 0;
		std::ostringstream os;
		if (延迟毫秒 < 1000) {
			os << 延迟毫秒 << "ms";
		}
		else {
			os << (延迟毫秒 / 1000.0) << "s";
		}
		return os.str();
	}

	std::string 远程快照概览摘要UTF8_(const 结构_后台前端快照& 快照) {
		std::ostringstream os;
		os << (快照.自我线程运行中 ? "自我运行中" : "自我未就绪");
		if (快照.初始化完成) {
			os << " | " << (快照.健康运行 ? "健康" : "待恢复");
		}
		if (快照.任务数 > 0 || 快照.需求数 > 0 || 快照.状态数 > 0
			|| 快照.事件动态数 > 0 || 快照.因果数 > 0) {
			os << " | 任务=" << 快照.任务数
				<< " | 需求=" << 快照.需求数
				<< " | 状态=" << 快照.状态数;
			if (快照.事件动态数 > 0 || 快照.动态数 > 0) {
				os << " | 事件=" << 快照.事件动态数;
			}
			if (快照.因果数 > 0) {
				os << " | 因果=" << 快照.因果数;
			}
		}
		if (快照.有当前安全值 || 快照.有当前服务值) {
			os << " | 核心值=";
			if (快照.有当前安全值) {
				os << "安全" << 快照.当前安全值;
			}
			else {
				os << "安全?";
			}
			os << "/";
			if (快照.有当前服务值) {
				os << "服务" << 快照.当前服务值;
			}
			else {
				os << "服务?";
			}
		}
		if (快照.快照生成UTC毫秒 > 0) {
			os << " | 快照延迟=" << 快照新鲜度文本_(快照);
		}
		if (!快照.主要需求类型.empty()) {
			os << " | 主要需求=" << 主要需求类型显示文本_(快照.主要需求类型);
		}
		return os.str();
	}

	template<class TNode, class Fn>
	void 枚举根子节点_(TNode* 根, Fn&& fn);

	任务节点类* 取主任务头节点_(任务节点类* 任务头节点) {
		auto* 当前 = 任务头节点;
		while (当前 && 当前->主信息) {
			auto* 头信息 = dynamic_cast<任务头结点信息*>(当前->主信息);
			if (!头信息 || !头信息->父任务头结点 || 头信息->父任务头结点 == 当前) break;
			当前 = 头信息->父任务头结点;
		}
		return 当前;
	}

	bool 主任务属于安全任务树_(任务节点类* 任务头节点) {
		auto* 主任务头 = 取主任务头节点_(任务头节点);
		auto* 头信息 = (主任务头 && 主任务头->主信息) ? dynamic_cast<任务头结点信息*>(主任务头->主信息) : nullptr;
		if (!头信息) return false;
		return 词键_(头信息->名称).find("安全任务") != std::string::npos;
	}

	bool 主任务属于服务任务树_(任务节点类* 任务头节点) {
		auto* 主任务头 = 取主任务头节点_(任务头节点);
		auto* 头信息 = (主任务头 && 主任务头->主信息) ? dynamic_cast<任务头结点信息*>(主任务头->主信息) : nullptr;
		if (!头信息) return false;
		return 词键_(头信息->名称).find("服务任务") != std::string::npos;
	}

	struct 结构_任务分类统计 {
		std::size_t 总任务数 = 0;
		std::size_t 安全任务数 = 0;
		std::size_t 服务任务数 = 0;
		std::size_t 叶子任务数 = 0;
	};

	结构_任务分类统计 统计任务分类_() {
		结构_任务分类统计 结果{};
		auto 递归枚举子树 = [&](auto&& self, 任务节点类* 父节点) -> void {
			auto* 首子节点 = 父节点 ? static_cast<任务节点类*>(父节点->子) : nullptr;
			if (!首子节点) return;
			auto* 当前 = 首子节点;
			do {
				if (auto* 头信息 = (当前 && 当前->主信息) ? dynamic_cast<任务头结点信息*>(当前->主信息) : nullptr) {
					++结果.总任务数;
					if (头信息->任务树类型 == 枚举_任务树类型::叶子任务) ++结果.叶子任务数;
					if (主任务属于安全任务树_(当前)) ++结果.安全任务数;
					else if (主任务属于服务任务树_(当前)) ++结果.服务任务数;
				}
				self(self, 当前);
				当前 = static_cast<任务节点类*>(当前->下);
			} while (当前 && 当前 != 首子节点);
		};

		枚举根子节点_(static_cast<任务节点类*>(数据仓库模块::任务链.根指针), [&](任务节点类* 顶层节点) {
			if (auto* 头信息 = (顶层节点 && 顶层节点->主信息) ? dynamic_cast<任务头结点信息*>(顶层节点->主信息) : nullptr) {
				++结果.总任务数;
				if (头信息->任务树类型 == 枚举_任务树类型::叶子任务) ++结果.叶子任务数;
				if (主任务属于安全任务树_(顶层节点)) ++结果.安全任务数;
				else if (主任务属于服务任务树_(顶层节点)) ++结果.服务任务数;
			}
			递归枚举子树(递归枚举子树, 顶层节点);
		});

		return 结果;
	}

	std::string 快照文本_(const 特征快照值& v) {
		if (std::holds_alternative<I64>(v)) return std::to_string(std::get<I64>(v));
		if (std::holds_alternative<VecU句柄>(v)) return std::string("VecU");
		return std::string("<值>");
	}

	template<class TNode, class Fn>
	void 枚举根子节点_(TNode* 根, Fn&& fn) {
		if (!根) return;

		if (根->子) {
			auto* first = static_cast<TNode*>(根->子);
			auto* it = first;
			do {
				fn(it);
				it = static_cast<TNode*>(it->下);
			} while (it && it != first);
			return;
		}

		// 顶层需求链/任务链的根节点使用 根->下 维护同层环，而不是挂在 根->子 上。
		auto* it = static_cast<TNode*>(根->下);
		while (it && it != 根) {
			fn(it);
			it = static_cast<TNode*>(it->下);
		}
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
	停止后台连接线程();
	关闭后台进程跟踪句柄();

	// 如果该对话框有自动化代理，则
	//  此对话框的返回指针为 null，所以它知道
	//  此代理知道该对话框已被删除。
	if (m_pAutoProxy != nullptr)
		m_pAutoProxy->m_pDialog = nullptr;
}

void C海鱼Dlg::清理退出子窗口()
{
	if (::IsWindow(GetSafeHwnd())) {
		KillTimer(定时器_标签页刷新);
	}
	安全销毁窗口_(变量_信息查看框);
	安全销毁窗口_(变量_配置及测试窗口);
	安全销毁窗口_(变量_交互界面);
	安全销毁窗口_(变量_基础信息界面);
	安全销毁窗口_(变量_TAB1);
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
	ON_MESSAGE(C海鱼Dlg::WM_APP_后台连接完成, &C海鱼Dlg::OnApp后台连接完成)
	ON_MESSAGE(C海鱼Dlg::WM_APP_自我场景再现窗口关闭, &C海鱼Dlg::OnApp自我场景再现窗口关闭)
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
	确保窗口落在可见工作区_(this);
	//初始化();
	// TODO: 在此添加额外的初始化代码
	主窗口指针 = this;
	// 日志：运行/异常分文件按日滚动
	日志参数 lp{};
	lp.根目录 = std::filesystem::path(std::u8string(u8"./日志"));
	lp.文件前缀 = "鱼巢控制面板";
	lp.每条刷新 = true;
	日志::初始化(lp);
	日志::运行("程序启动：日志初始化完成");
	特征值集.初始化();
	语素集.初始化();
	世界树.初始化默认世界();
	语言集.初始化();
	使用远程后台模式_ = true;
	后台在线_ = false;
	最近后台快照_ = {};
	最近后台快照_.后台在线 = false;
	最近后台快照_.最近命令摘要 = "正在连接后台...";
	最近后台自我场景快照_ = {};
	最近后台自我场景快照_.后台在线 = false;
	最近后台自我场景快照有效_ = false;
	最近后台自我场景错误_.clear();
	日志::运行("[前端界面] 控制面板已固定为远程后台模式：不再启动本地自我线程、外设与场景显示");

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
	隐藏子页面关闭控件_(变量_交互界面);
	隐藏子页面关闭控件_(变量_配置及测试窗口);
	隐藏子页面关闭控件_(变量_基础信息界面);
	变量_信息查看框.CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""),
		WS_CHILD | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
		rect, this, IDC_标签内容查看框);
	变量_信息查看框.SetFont(GetFont());
	//////////////////////////////////////////////////
	变量_TAB1.SetCurSel(0);
	const bool 初始使用信息框 = 标签页使用信息查看框(变量_TAB1.GetCurSel());
	变量_基础信息界面.ShowWindow(SW_HIDE);
	变量_交互界面.ShowWindow(初始使用信息框 ? SW_HIDE : SW_SHOW);
	变量_配置及测试窗口.ShowWindow(SW_HIDE);
	变量_信息查看框.ShowWindow(初始使用信息框 ? SW_SHOW : SW_HIDE);
	SetTimer(定时器_标签页刷新, 1000, nullptr);
	刷新当前标签内容();
	if (使用远程后台模式_) {
		启动异步后台连接();
	}
	////////////////////////////////////////////


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BOOL C海鱼Dlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg &&
		pMsg->message == WM_KEYDOWN &&
		pMsg->wParam == VK_ESCAPE &&
		::IsWindow(变量_TAB1.GetSafeHwnd()) &&
		pMsg->hwnd != nullptr &&
		pMsg->hwnd != GetSafeHwnd() &&
		::IsChild(变量_TAB1.GetSafeHwnd(), pMsg->hwnd)) {
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL C海鱼Dlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(wParam) == IDCANCEL &&
		::IsWindow(变量_TAB1.GetSafeHwnd()) &&
		lParam != 0) {
		const HWND 来源窗口 = reinterpret_cast<HWND>(lParam);
		if (来源窗口 != nullptr &&
			来源窗口 != GetSafeHwnd() &&
			::IsChild(变量_TAB1.GetSafeHwnd(), 来源窗口)) {
			return TRUE;
		}
	}

	return CDialogEx::OnCommand(wParam, lParam);
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
	if (使用远程后台模式_) {
		switch (标签索引) {
		case 0:
		case 7:
		case 8:
			return false;
		default:
			return true;
		}
	}
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
	if (使用远程后台模式_) {
		return 生成远程标签页文本(标签索引);
	}

	std::ostringstream os;
	os << "标签索引: " << 标签索引 << "\r\n\r\n";
	const auto 布尔文本_ = [](bool 值) {
		return 值 ? "是" : "否";
		};
	const auto 追加自我线程生命体征面板_ = [&](std::ostringstream& 输出) {
		const bool 线程活着 = 自我线程 && 自我线程->是否正在运行();
		const bool 初始化完成 = 自我线程 && 自我线程->是否初始化完成();
		const bool 健康运行 = 自我线程 && 自我线程->是否健康运行();
		std::string 最近故障 = "无";
		if (自我线程) {
			auto 摘要 = 自我线程->最近故障摘要();
			if (!摘要.empty()) {
				最近故障 = std::move(摘要);
			}
		}
		输出 << "线程活着: " << 布尔文本_(线程活着) << "\r\n";
		输出 << "初始化完成: " << 布尔文本_(初始化完成) << "\r\n";
		输出 << "健康运行: " << 布尔文本_(健康运行) << "\r\n";
		输出 << "最近故障: " << 最近故障 << "\r\n";
		};
	const auto 追加状态事件因果摘要_ = [&](std::ostringstream& 输出) {
		auto* 内部世界 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
		auto* 场景信息 = (内部世界 && 内部世界->主信息) ? dynamic_cast<场景节点主信息类*>(内部世界->主信息) : nullptr;
		std::size_t 状态数 = 0;
		std::size_t 动态数 = 0;
		std::size_t 事件动态数 = 0;
		if (场景信息) {
			场景信息->清理空指针();
			状态数 = 场景信息->状态列表.size();
			动态数 = 场景信息->动态列表.size();
			事件动态数 = 场景信息->事件动态列表.size();
		}
		输出 << "状态数: " << 状态数 << "\r\n";
		输出 << "聚合动态数: " << 动态数 << "\r\n";
		输出 << "原始事件动态数: " << 事件动态数 << "\r\n";
		输出 << "因果节点数: 请切换到远程后台摘要查看\r\n";
	};

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
	else if (标签索引 == 3) {
		os << "事件信息\r\n\r\n";
		追加自我线程生命体征面板_(os);
		追加状态事件因果摘要_(os);
		os << "\r\n说明: 状态列表记录事实；动态信息描述状态先后顺序；事件动态列表会保留近期原始时间片，即使聚合动态已经收束。";
	}
	else if (标签索引 == 4) {
		os << "因果信息\r\n\r\n";
		追加自我线程生命体征面板_(os);
		追加状态事件因果摘要_(os);
		os << "\r\n说明: 因果提炼以状态列表为事实底座，以事件动态列表提供先后顺序；因果信息最终仍可看作从状态信息归纳出的结果。";
	}
	else if (标签索引 == 5 || 标签索引 == 6) {
		size_t worldCount = 0, needCount = 0, methodCount = 0;
		const auto 任务统计 = 统计任务分类_();
		枚举根子节点_(static_cast<基础信息节点类*>(数据仓库模块::世界链.根指针), [&](基础信息节点类*) { ++worldCount; });
		枚举根子节点_(static_cast<需求节点类*>(数据仓库模块::需求链.根指针), [&](需求节点类*) { ++needCount; });
		枚举根子节点_(static_cast<方法节点类*>(数据仓库模块::方法链.根指针), [&](方法节点类*) { ++methodCount; });
		os << (标签索引 == 5 ? "环境 / 自我状态" : "学习 / 自我状态") << "\r\n\r\n";
		追加自我线程生命体征面板_(os);
		os << "世界根子节点数: " << worldCount << "\r\n";
		os << "需求数: " << needCount << "\r\n";
		os << "总任务数: " << 任务统计.总任务数 << "\r\n";
		os << "安全任务数: " << 任务统计.安全任务数 << "\r\n";
		os << "服务任务数: " << 任务统计.服务任务数 << "\r\n";
		os << "叶子任务数: " << 任务统计.叶子任务数 << "\r\n";
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
		size_t needCount = 0, methodCount = 0;
		const auto 任务统计 = 统计任务分类_();
		枚举根子节点_(static_cast<需求节点类*>(数据仓库模块::需求链.根指针), [&](需求节点类*) { ++needCount; });
		枚举根子节点_(static_cast<方法节点类*>(数据仓库模块::方法链.根指针), [&](方法节点类*) { ++methodCount; });
		os << "当前标签页暂未单独细化，先显示系统摘要。\r\n\r\n";
		追加自我线程生命体征面板_(os);
		os << "需求数: " << needCount << "\r\n";
		os << "总任务数: " << 任务统计.总任务数 << "\r\n";
		os << "安全任务数: " << 任务统计.安全任务数 << "\r\n";
		os << "服务任务数: " << 任务统计.服务任务数 << "\r\n";
		os << "叶子任务数: " << 任务统计.叶子任务数 << "\r\n";
		os << "方法数: " << methodCount << "\r\n";
	}

	return UTF8到CString_(os.str());
}

CString C海鱼Dlg::生成远程标签页文本(int 标签索引) const
{
	const auto& 快照 = 最近后台快照_;
	const auto& 场景快照 = 最近后台自我场景快照_;
	const bool 场景快照有效 = 最近后台自我场景快照有效_;
	const auto 布尔文本_ = [](bool 值) { return 值 ? "是" : "否"; };
	std::ostringstream os;
	os << "前端模式: 远程后台界面\r\n";
	os << "后台在线: " << 布尔文本_(后台在线_) << "\r\n";
	os << "后台连接中: " << 布尔文本_(后台连接中_.load()) << "\r\n";
	if (!后台进程路径_.empty()) {
		os << "后台程序: " << CW2A(后台进程路径_.c_str(), CP_UTF8) << "\r\n";
	}
	os << "\r\n";

	const auto 追加场景摘要_ = [&](std::ostringstream& 输出, std::size_t 最大显示存在数, bool 包含场景统计 = true) {
		输出 << "场景快照可用: " << 布尔文本_(场景快照有效) << "\r\n";
		if (!场景快照有效) {
			if (!最近后台自我场景错误_.empty()) {
				输出 << "场景快照说明: " << 最近后台自我场景错误_ << "\r\n";
			}
			return;
		}

		输出 << "场景再现运行中: " << 布尔文本_(场景快照.再现运行中) << "\r\n";
		输出 << "场景帧序号: " << 场景快照.帧序号 << "\r\n";
		输出 << "场景存在数: " << 场景快照.存在列表.size() << "\r\n";
		if (!场景快照.场景主键.empty()) {
			输出 << "场景主键: " << 场景快照.场景主键 << "\r\n";
		}
		if (!场景快照.状态摘要.empty()) {
			输出 << "场景状态摘要: " << 场景快照.状态摘要 << "\r\n";
		}
		if (包含场景统计 && !场景快照.存在列表.empty()) {
			std::size_t 有位置数 = 0;
			std::size_t 有尺寸数 = 0;
			std::size_t 活跃存在数 = 0;
			std::size_t 静止存在数 = 0;
			std::size_t 未命中存在数 = 0;
			std::uint64_t 轮廓总块数 = 0;
			bool 有深度范围 = false;
			std::int64_t 最小深度Z = 0;
			std::int64_t 最大深度Z = 0;
			for (const auto& 项 : 场景快照.存在列表) {
				if (项.有位置) {
					++有位置数;
					if (!有深度范围) {
						最小深度Z = 项.中心Z_mm;
						最大深度Z = 项.中心Z_mm;
						有深度范围 = true;
					}
					else {
						最小深度Z = std::min(最小深度Z, 项.中心Z_mm);
						最大深度Z = std::max(最大深度Z, 项.中心Z_mm);
					}
				}
				if (项.有尺寸) ++有尺寸数;
				if (项.连续未命中帧 > 0) ++未命中存在数;
				if (项.连续静止帧 > 0) ++静止存在数;
				if (项.连续命中帧 > 0 && 项.连续静止帧 == 0 && 项.连续未命中帧 == 0) ++活跃存在数;
				轮廓总块数 += 项.轮廓块数;
			}
			输出 << "场景统计: 有位置=" << 有位置数
				<< " | 有尺寸=" << 有尺寸数
				<< " | 活跃=" << 活跃存在数
				<< " | 静止=" << 静止存在数
				<< " | 未命中=" << 未命中存在数
				<< " | 轮廓块总数=" << 轮廓总块数 << "\r\n";
			if (有深度范围) {
				输出 << "场景深度范围(mm): Z=[" << 最小深度Z << "," << 最大深度Z << "]\r\n";
			}
		}

		const std::size_t 显示数 = std::min<std::size_t>(最大显示存在数, 场景快照.存在列表.size());
		for (std::size_t i = 0; i < 显示数; ++i) {
			const auto& 项 = 场景快照.存在列表[i];
			输出 << "场景存在[" << (i + 1) << "]: " << 项.存在主键;
			if (项.有位置) {
				输出 << " | pos(mm)=(" << 项.中心X_mm << "," << 项.中心Y_mm << "," << 项.中心Z_mm << ")";
			}
			if (项.有尺寸) {
				输出 << " | size(mm)=(" << 项.尺寸X_mm << "," << 项.尺寸Y_mm << "," << 项.尺寸Z_mm << ")";
			}
			输出 << " | contour=" << 项.轮廓块数
				<< " | hit=" << 项.连续命中帧
				<< " | miss=" << 项.连续未命中帧
				<< " | still=" << 项.连续静止帧
				<< "\r\n";
		}
		if (场景快照.存在列表.size() > 显示数) {
			输出 << "场景存在: 其余 " << (场景快照.存在列表.size() - 显示数) << " 项已省略\r\n";
		}
	};

	const auto 追加公共摘要_ = [&](std::ostringstream& 输出, std::size_t 场景最大显示存在数 = 3, bool 包含场景摘要 = true) {
		输出 << "运行概览: " << 远程快照概览摘要UTF8_(快照) << "\r\n";
		输出 << "结构统计: 世界=" << 快照.世界根子节点数
			<< " | 方法=" << 快照.方法数
			<< " | 状态=" << 快照.状态数
			<< " | 动态=" << 快照.动态数
			<< " | 事件动态=" << 快照.事件动态数
			<< " | 因果=" << 快照.因果数 << "\r\n";
		输出 << "任务统计: 总任务=" << 快照.任务数
			<< " | 安全=" << 快照.安全任务数
			<< " | 服务=" << 快照.服务任务数
			<< " | 叶子=" << 快照.叶子任务数 << "\r\n";
		输出 << "需求统计: 总需求=" << 快照.需求数;
		if (!快照.主要需求类型.empty()) {
			输出 << " | 当前主要需求=" << 主要需求摘要文本_(快照);
		}
		输出 << "\r\n";
		输出 << "核心值: 安全=" << (快照.有当前安全值 ? std::to_string(快照.当前安全值) : std::string("<未知>"))
			<< " | 服务=" << (快照.有当前服务值 ? std::to_string(快照.当前服务值) : std::string("<未知>"))
			<< " | 快照延迟=" << 快照新鲜度文本_(快照) << "\r\n";
		输出 << "\r\n";
		输出 << "宿主运行中: " << 布尔文本_(快照.宿主运行中) << "\r\n";
		输出 << "自我线程运行中: " << 布尔文本_(快照.自我线程运行中) << "\r\n";
		输出 << "初始化完成: " << 布尔文本_(快照.初始化完成) << "\r\n";
		输出 << "健康运行: " << 布尔文本_(快照.健康运行) << "\r\n";
		输出 << "请求安全重启: " << 布尔文本_(快照.请求安全重启) << "\r\n";
		输出 << "致命错误计数: " << 快照.致命错误计数 << "\r\n";
		输出 << "已执行重启次数: " << 快照.已执行重启次数 << "\r\n";
		输出 << "相机已启用: " << 布尔文本_(快照.相机已启用) << "\r\n";
		输出 << "相机运行中: " << 布尔文本_(快照.相机运行中) << "\r\n";
		输出 << "相机虚拟回退: " << 布尔文本_(快照.相机使用虚拟回退) << "\r\n";
		输出 << "相机已处理帧数: " << 快照.相机已处理帧数 << "\r\n";
		输出 << "最近桥接候选数: " << 快照.最近桥接候选数 << "\r\n";
		if (快照.最近输入序号 > 0) {
			输出 << "最近输入序号: " << 快照.最近输入序号 << "\r\n";
		}
		if (!快照.最近输入文本.empty()) {
			输出 << "最近输入文本: " << 快照.最近输入文本 << "\r\n";
		}
		if (!快照.最近命令摘要.empty()) {
			输出 << "最近命令摘要: " << 快照.最近命令摘要 << "\r\n";
		}
		if (!快照.最近故障摘要.empty()) {
			输出 << "最近故障摘要: " << 快照.最近故障摘要 << "\r\n";
		}
		if (!快照.宿主故障摘要.empty()) {
			输出 << "宿主故障摘要: " << 快照.宿主故障摘要 << "\r\n";
		}
		if (!快照.相机状态摘要.empty()) {
			输出 << "相机状态摘要: " << 快照.相机状态摘要 << "\r\n";
		}
		if (包含场景摘要) {
			输出 << "\r\n";
			追加场景摘要_(输出, 场景最大显示存在数, true);
		}
	};

	if (标签索引 == 1) {
		os << "任务信息（远程摘要）\r\n\r\n";
		追加公共摘要_(os);
		os << "总任务数: " << 快照.任务数 << "\r\n";
		os << "安全任务数: " << 快照.安全任务数 << "\r\n";
		os << "服务任务数: " << 快照.服务任务数 << "\r\n";
		os << "叶子任务数: " << 快照.叶子任务数 << "\r\n";
		os << "\r\n详细任务树尚未通过后台通道下发，当前前端先显示汇总状态。";
	}
	else if (标签索引 == 2) {
		os << "方法信息（远程摘要）\r\n\r\n";
		追加公共摘要_(os);
		os << "方法数: " << 快照.方法数 << "\r\n";
		os << "\r\n详细方法树尚未通过后台通道下发，当前前端先显示汇总状态。";
	}
	else if (标签索引 == 3) {
		os << "事件信息（远程摘要）\r\n\r\n";
		追加公共摘要_(os);
		os << "状态数: " << 快照.状态数 << "\r\n";
		os << "聚合动态数: " << 快照.动态数 << "\r\n";
		os << "原始事件动态数: " << 快照.事件动态数 << "\r\n";
		os << "\r\n说明: 状态列表记录事实，事件动态描述状态的先后顺序；当前后台会在保留原始事件动态的同时继续做聚合视图。";
	}
	else if (标签索引 == 4) {
		os << "因果信息（远程摘要）\r\n\r\n";
		追加公共摘要_(os);
		os << "因果节点数: " << 快照.因果数 << "\r\n";
		os << "状态数: " << 快照.状态数 << "\r\n";
		os << "原始事件动态数: " << 快照.事件动态数 << "\r\n";
		os << "\r\n说明: 因果提炼依赖状态列表与事件动态列表；当前前端先显示因果总量与生成基础，详细因果树后续再通过后台通道下发。";
	}
	else if (标签索引 == 5 || 标签索引 == 6) {
		os << (标签索引 == 5 ? "环境 / 自我状态（远程）" : "学习 / 自我状态（远程）") << "\r\n\r\n";
		追加公共摘要_(os);
		os << "世界根子节点数: " << 快照.世界根子节点数 << "\r\n";
		os << "需求数: " << 快照.需求数 << "\r\n";
		os << "总任务数: " << 快照.任务数 << "\r\n";
		os << "安全任务数: " << 快照.安全任务数 << "\r\n";
		os << "服务任务数: " << 快照.服务任务数 << "\r\n";
		os << "叶子任务数: " << 快照.叶子任务数 << "\r\n";
		os << "方法数: " << 快照.方法数 << "\r\n";
	}
	else if (标签索引 == 8) {
		os << "场景信息（远程）\r\n\r\n";
		追加公共摘要_(os, 12, true);
		os << "\r\n说明: 场景信息由相机观测驱动；状态列表形成事件动态，事件动态支持因果提炼。";
	}
	else if (标签索引 == 9 || 标签索引 == 10) {
		os << (标签索引 == 9 ? "需求列表（远程摘要）" : "需求窗口（远程摘要）") << "\r\n\r\n";
		追加公共摘要_(os);
		if (!快照.主要需求类型.empty()) {
			os << "当前主要需求类型: " << 主要需求类型显示文本_(快照.主要需求类型) << "\r\n";
			os << "当前主要需求权重: " << 快照.主要需求权重 << "\r\n";
			os << "当前主要需求已满足: " << 布尔文本_(快照.主要需求已满足) << "\r\n";
		}
		else {
			os << "当前没有可展示的主要需求。\r\n";
		}
	}
	else {
		os << "远程后台系统摘要\r\n\r\n";
		追加公共摘要_(os);
		os << "世界根子节点数: " << 快照.世界根子节点数 << "\r\n";
		os << "需求数: " << 快照.需求数 << "\r\n";
		os << "总任务数: " << 快照.任务数 << "\r\n";
		os << "安全任务数: " << 快照.安全任务数 << "\r\n";
		os << "服务任务数: " << 快照.服务任务数 << "\r\n";
		os << "叶子任务数: " << 快照.叶子任务数 << "\r\n";
		os << "方法数: " << 快照.方法数 << "\r\n";
	}

	return UTF8到CString_(os.str());
}

CString C海鱼Dlg::取远程后台状态摘要() const
{
	if (!使用远程后台模式_) {
		return _T("本地内核模式");
	}

	const auto& 快照 = 最近后台快照_;
	CString 摘要;
	if (后台连接中_.load()) {
		摘要 = _T("后台连接中，等待首份运行快照");
	}
	else if (后台在线_) {
		摘要 = UTF8到CString_(std::string("后台在线 | ") + 远程快照概览摘要UTF8_(快照));
	}
	else {
		摘要 = _T("后台离线");
	}

	if (后台在线_) {
		if (最近后台自我场景快照有效_) {
			std::ostringstream 场景摘要;
			std::string 场景键简写 = 最近后台自我场景快照_.场景主键;
			if (场景键简写.size() > 16) {
				场景键简写 = 场景键简写.substr(0, 16) + "...";
			}
			场景摘要 << " | 场景="
				<< (最近后台自我场景快照_.再现运行中 ? "运行" : "待机")
				<< " | 帧=" << 最近后台自我场景快照_.帧序号
				<< " | 存在=" << 最近后台自我场景快照_.存在列表.size();
			if (!场景键简写.empty()) {
				场景摘要 << " | 键=" << 场景键简写;
			}
			摘要 += UTF8到CString_(场景摘要.str());
		}
		return 摘要;
	}

	CString 附加;
	if (!快照.最近命令摘要.empty()) {
		附加 = UTF8到CString_(快照.最近命令摘要);
	}
	else if (!快照.宿主故障摘要.empty()) {
		附加 = UTF8到CString_(快照.宿主故障摘要);
	}
	else if (!快照.最近故障摘要.empty()) {
		附加 = UTF8到CString_(快照.最近故障摘要);
	}

	附加.Trim();
	if (!附加.IsEmpty()) {
		摘要 += _T("：");
		摘要 += 附加;
	}
	return 摘要;
}

void C海鱼Dlg::刷新当前标签内容()
{
	if (!::IsWindow(变量_TAB1.GetSafeHwnd())) return;
	const int 标签索引 = 变量_TAB1.GetCurSel();
	const bool 用信息框 = 标签页使用信息查看框(标签索引);
	if (::IsWindow(变量_信息查看框.GetSafeHwnd())) {
		变量_信息查看框.ShowWindow(用信息框 ? SW_SHOW : SW_HIDE);
		if (用信息框) {
			变量_交互界面.ShowWindow(SW_HIDE);
			变量_配置及测试窗口.ShowWindow(SW_HIDE);
			变量_基础信息界面.ShowWindow(SW_HIDE);
			更新信息查看框布局();
			变量_信息查看框.SetWindowTextW(生成标签页文本(标签索引));
		}
	}
	if (!用信息框 && 标签索引 == 0 && ::IsWindow(变量_交互界面.GetSafeHwnd())) {
		变量_交互界面.刷新远程后台状态();
	}
}

void C海鱼Dlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 定时器_标签页刷新) {
		if (使用远程后台模式_) {
			(void)尝试刷新后台快照();
		}
		刷新当前标签内容();
	}
	CDialogEx::OnTimer(nIDEvent);
}

LRESULT C海鱼Dlg::OnApp后台连接完成(WPARAM, LPARAM lParam)
{
	std::unique_ptr<结构_异步后台连接结果> 结果(reinterpret_cast<结构_异步后台连接结果*>(lParam));
	后台连接中_.store(false);
	if (后台连接线程_.joinable()) {
		后台连接线程_.join();
	}

	if (结果) {
		if (结果->已启动进程句柄) {
			关闭后台进程跟踪句柄();
			后台进程句柄_ = 结果->已启动进程句柄;
			后台进程PID_ = 结果->已启动进程PID;
			if (!结果->已启动后台路径.empty()) {
				后台进程路径_ = 结果->已启动后台路径;
			}
			结果->已启动进程句柄 = nullptr;
			结果->已启动进程PID = 0;
		}
		if (后台在线_ && !结果->成功) {
			日志::运行("[前端界面] 后台连接线程结束，但前端已通过轮询确认后台在线，保留在线状态");
		}
		else {
			后台在线_ = 结果->成功;
			最近后台快照_ = 结果->快照;
			最近后台快照_.后台在线 = 结果->成功;
			if (!结果->成功 && 最近后台快照_.最近命令摘要.empty()) {
				最近后台快照_.最近命令摘要 = 结果->日志摘要;
			}
			日志::运行("[前端界面] " + 结果->日志摘要);
		}
	}

	if (后台在线_) {
		结构_后台自我场景快照 新场景快照{};
		std::string 场景错误;
		if (后台客户端_.查询自我场景快照(&新场景快照, &场景错误, 200)) {
			最近后台自我场景快照_ = std::move(新场景快照);
			最近后台自我场景快照_.后台在线 = true;
			最近后台自我场景快照有效_ = true;
			最近后台自我场景错误_.clear();
		}
		else {
			最近后台自我场景快照_ = {};
			最近后台自我场景快照_.后台在线 = true;
			最近后台自我场景快照有效_ = false;
			最近后台自我场景错误_ = 场景错误.empty() ? "后台场景快照暂不可用" : 场景错误;
		}
	}
	else {
		最近后台自我场景快照_ = {};
		最近后台自我场景快照_.后台在线 = false;
		最近后台自我场景快照有效_ = false;
		最近后台自我场景错误_ = "后台离线，场景快照不可用";
	}

	if (::IsWindow(变量_交互界面.GetSafeHwnd())) {
		变量_交互界面.刷新远程后台状态();
	}
	刷新当前标签内容();
	return 0;
}

LRESULT C海鱼Dlg::OnApp自我场景再现窗口关闭(WPARAM, LPARAM)
{
	自我场景再现窗口_.reset();
	if (!应用退出中_) {
		CString 错误文本;
		(void)停止远程自我场景再现(&错误文本);
	}
	return 0;
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
	if (应用退出中_) {
		return;
	}
	日志::运行("[主窗口关闭] OnClose 开始");
	if (!CanExit()) {
		日志::运行("[主窗口关闭] OnClose 被 CanExit 拦截");
		return;
	}
	应用退出中_ = true;
	执行主窗口退出清理();
	日志::运行("[主窗口关闭] 调用 CDialogEx::OnCancel");
	CDialogEx::OnCancel();
}

void C海鱼Dlg::OnOK()
{
	if (应用退出中_) {
		return;
	}
	if (!CanExit()) {
		return;
	}
	应用退出中_ = true;
	执行主窗口退出清理();
	CDialogEx::OnOK();
}

void C海鱼Dlg::OnCancel()
{
	if (应用退出中_) {
		return;
	}
	if (!CanExit()) {
		return;
	}
	应用退出中_ = true;
	执行主窗口退出清理();
	CDialogEx::OnCancel();
}

void C海鱼Dlg::执行主窗口退出清理()
{
	日志::运行("[主窗口关闭] 停止后台连接线程开始");
	停止后台连接线程();
	日志::运行("[主窗口关闭] 停止后台连接线程完成");
	日志::运行("[主窗口关闭] 停止自我场景再现开始");
	CString 自我场景错误;
	(void)停止远程自我场景再现(&自我场景错误);
	日志::运行("[主窗口关闭] 停止自我场景再现完成");
	日志::运行("[主窗口关闭] 清理子窗口开始");
	清理退出子窗口();
	日志::运行("[主窗口关闭] 清理子窗口完成");
}

BOOL C海鱼Dlg::CanExit()
{
	if (应用退出中_) {
		return TRUE;
	}

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
	if (::IsWindow(变量_信息查看框.GetSafeHwnd())) {
		更新信息查看框布局();
	}
	刷新场景显示布局();
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
	const bool 用信息框 = 标签页使用信息查看框(iIndex);
	if (用信息框) {
		if (::IsWindow(变量_信息查看框.GetSafeHwnd())) {
			变量_信息查看框.ShowWindow(SW_SHOW);
		}
	}
	else {
		switch (iIndex)
		{
		case 0:
			变量_交互界面.ShowWindow(SW_SHOW);
			break;
		case 7:
			变量_配置及测试窗口.ShowWindow(SW_SHOW);
			刷新场景显示布局();
			break;
		case 8:
			变量_基础信息界面.ShowWindow(SW_SHOW);
			break;
		default:
			break;
		}
	}

	刷新当前标签内容();
	*pResult = 0;
}

void C海鱼Dlg::启动异步后台连接()
{
	if (!使用远程后台模式_ || 后台连接中_.load()) return;

	停止后台连接线程();
	后台连接取消_.store(false);
	后台连接中_.store(true);
	最近后台快照_.最近命令摘要 = "正在连接后台...";
	if (::IsWindow(变量_交互界面.GetSafeHwnd())) {
		变量_交互界面.刷新远程后台状态();
	}
	刷新当前标签内容();

	const HWND hwnd = GetSafeHwnd();
	后台连接线程_ = std::thread([this, hwnd]() {
		auto 结果 = std::make_unique<结构_异步后台连接结果>();
		后台前端客户端类 客户端;

		if (!后台连接取消_.load() && 客户端.查询快照(&结果->快照, 250)) {
			结果->成功 = true;
			结果->快照.后台在线 = true;
			结果->日志摘要 = "已连接远程后台，主窗口切换为独立前端模式";
		}
		else {
			const std::wstring 后台路径 = 取后台可执行路径();
			if (后台路径.empty()) {
				结果->成功 = false;
				结果->快照 = {};
				结果->快照.后台在线 = false;
				结果->快照.最近命令摘要 = "未找到鱼巢.exe，无法自动启动后台";
				结果->日志摘要 = "远程后台当前不可用，保持独立前端模式并允许手动启动后台";
			}
			else if (!后台连接取消_.load() && 后台进程仍在运行()) {
				for (int i = 0; i < 40 && !后台连接取消_.load(); ++i) {
					std::this_thread::sleep_for(std::chrono::milliseconds(250));
					if (客户端.查询快照(&结果->快照, 250)) {
						结果->成功 = true;
						结果->快照.后台在线 = true;
						结果->日志摘要 = "已连接远程后台，主窗口切换为独立前端模式";
						break;
					}
				}
				if (!结果->成功) {
					结果->快照 = {};
					结果->快照.后台在线 = false;
					结果->快照.最近命令摘要 = "后台进程仍在运行，但前端连接尚未建立";
					结果->日志摘要 = "远程后台当前不可用，保持独立前端模式并允许手动启动后台";
				}
			}
			else if (!后台连接取消_.load() && 按路径拉起后台进程_(
				后台路径,
				&结果->已启动后台路径,
				&结果->已启动进程句柄,
				&结果->已启动进程PID)) {
				for (int i = 0; i < 40 && !后台连接取消_.load(); ++i) {
					std::this_thread::sleep_for(std::chrono::milliseconds(250));
					if (客户端.查询快照(&结果->快照, 250)) {
						结果->成功 = true;
						结果->快照.后台在线 = true;
						结果->日志摘要 = "已连接远程后台，主窗口切换为独立前端模式";
						break;
					}
				}
				if (!结果->成功) {
					结果->快照 = {};
					结果->快照.后台在线 = false;
					if (结果->已启动进程句柄 &&
						::WaitForSingleObject(结果->已启动进程句柄, 0) == WAIT_TIMEOUT) {
						结果->快照.最近命令摘要 = "后台进程已启动，但前端连接尚未建立";
					}
					else {
						结果->快照.最近命令摘要 = "后台启动后很快退出，或未能建立前端连接";
					}
					结果->日志摘要 = "远程后台当前不可用，保持独立前端模式并允许手动启动后台";
				}
			}
			else {
				结果->成功 = false;
				结果->快照 = {};
				结果->快照.后台在线 = false;
				结果->快照.最近命令摘要 = "自动拉起后台失败";
				结果->日志摘要 = "远程后台当前不可用，保持独立前端模式并允许手动启动后台";
			}
		}

		if (后台连接取消_.load()) {
			return;
		}

		if (!::IsWindow(hwnd)) {
			return;
		}
		if (!::PostMessage(hwnd, WM_APP_后台连接完成, 0, reinterpret_cast<LPARAM>(结果.release()))) {
			return;
		}
	});
}

void C海鱼Dlg::停止后台连接线程()
{
	后台连接取消_.store(true);
	if (后台连接线程_.joinable()) {
		后台连接线程_.join();
	}
	后台连接中_.store(false);
}

void C海鱼Dlg::关闭后台进程跟踪句柄()
{
	if (后台进程句柄_) {
		::CloseHandle(后台进程句柄_);
		后台进程句柄_ = nullptr;
	}
	后台进程PID_ = 0;
}

bool C海鱼Dlg::后台进程仍在运行() const
{
	if (!后台进程句柄_) {
		return false;
	}
	return ::WaitForSingleObject(后台进程句柄_, 0) == WAIT_TIMEOUT;
}

bool C海鱼Dlg::强制结束已跟踪后台进程(CString* 错误文本)
{
	if (!后台进程句柄_) {
		if (错误文本) *错误文本 = _T("当前没有可回收的后台进程句柄。");
		return false;
	}
	if (!后台进程仍在运行()) {
		关闭后台进程跟踪句柄();
		return true;
	}
	if (!::TerminateProcess(后台进程句柄_, 0)) {
		if (错误文本) {
			错误文本->Format(_T("强制结束后台失败，错误码=%lu。"), ::GetLastError());
		}
		return false;
	}
	(void)::WaitForSingleObject(后台进程句柄_, 3000);
	关闭后台进程跟踪句柄();
	return true;
}

bool C海鱼Dlg::尝试刷新后台快照()
{
	结构_后台前端快照 新快照{};
	if (!后台客户端_.查询快照(&新快照, 250)) {
		后台在线_ = false;
		最近后台快照_.后台在线 = false;
		最近后台自我场景快照_ = {};
		最近后台自我场景快照_.后台在线 = false;
		最近后台自我场景快照有效_ = false;
		最近后台自我场景错误_ = "后台离线，场景快照不可用";
		if (!后台连接中_.load()) {
			if (后台进程仍在运行()) {
				最近后台快照_.最近命令摘要 = "后台进程运行中，但状态查询暂未恢复";
			}
			else {
				关闭后台进程跟踪句柄();
			}
		}
		if (::IsWindow(变量_交互界面.GetSafeHwnd())) {
			变量_交互界面.刷新远程后台状态();
		}
		return false;
	}

	const bool 刚结束连接 = 后台连接中_.exchange(false);
	后台在线_ = true;
	最近后台快照_ = std::move(新快照);
	最近后台快照_.后台在线 = true;
	{
		结构_后台自我场景快照 新场景快照{};
		std::string 场景错误;
		if (后台客户端_.查询自我场景快照(&新场景快照, &场景错误, 200)) {
			最近后台自我场景快照_ = std::move(新场景快照);
			最近后台自我场景快照_.后台在线 = true;
			最近后台自我场景快照有效_ = true;
			最近后台自我场景错误_.clear();
		}
		else {
			最近后台自我场景快照_ = {};
			最近后台自我场景快照_.后台在线 = true;
			最近后台自我场景快照有效_ = false;
			最近后台自我场景错误_ = 场景错误.empty() ? "后台场景快照暂不可用" : 场景错误;
		}
	}
	if (刚结束连接) {
		if (最近后台快照_.最近命令摘要.empty()) {
			最近后台快照_.最近命令摘要 = "已连接远程后台，前端可直接提交输入";
		}
		日志::运行("[前端界面] 已通过定时刷新确认远程后台在线");
	}
	if (::IsWindow(变量_交互界面.GetSafeHwnd())) {
		变量_交互界面.刷新远程后台状态();
	}
	return true;
}

bool C海鱼Dlg::提交远程人类输入(const CString& 文本, CString* 错误文本)
{
	if (!使用远程后台模式_) {
		if (错误文本) *错误文本 = _T("当前不是远程后台模式。");
		return false;
	}
	if (后台连接中_.load()) {
		if (错误文本) *错误文本 = _T("后台正在连接中，请稍候再提交输入。");
		return false;
	}
	if (!后台在线_ && !尝试刷新后台快照()) {
		if (错误文本) *错误文本 = _T("后台当前离线，请先点击“启动后台”。");
		return false;
	}

	CString 清洗后 = 文本;
	清洗后.Trim();
	if (清洗后.IsEmpty()) {
		if (错误文本) *错误文本 = _T("输入为空。");
		return false;
	}

	std::string 错误;
	CW2A utf8转换器(清洗后, CP_UTF8);
	const std::string utf8 = static_cast<const char*>(utf8转换器);
	if (!后台客户端_.提交人类输入(utf8, &错误, 1000)) {
		后台在线_ = false;
		最近后台快照_.后台在线 = false;
		if (错误文本) *错误文本 = UTF8到CString_(错误.empty() ? "后台未响应输入提交请求" : 错误);
		return false;
	}

	(void)尝试刷新后台快照();
	return true;
}

bool C海鱼Dlg::启动远程后台(CString* 错误文本)
{
	if (!使用远程后台模式_) {
		if (错误文本) *错误文本 = _T("当前不是远程后台模式。");
		return false;
	}
	if (后台连接中_.load()) {
		if (错误文本) *错误文本 = _T("后台正在连接中，请稍候。");
		return true;
	}
	if (尝试刷新后台快照()) {
		return true;
	}
	if (后台进程仍在运行()) {
		最近后台快照_.最近命令摘要 = "后台进程仍在运行，正在等待前端重新接入";
		启动异步后台连接();
		if (错误文本) *错误文本 = _T("后台已在运行，正在重新接入，请稍候。");
		return true;
	}
	最近后台快照_.最近命令摘要 = "正在连接后台...";
	启动异步后台连接();
	if (错误文本) *错误文本 = _T("已开始连接后台，请稍候刷新。");
	return true;
}

bool C海鱼Dlg::运行后台任务控制回归套件(CString* 输出文本, CString* 错误文本)
{
    if (输出文本) 输出文本->Empty();
    if (错误文本) 错误文本->Empty();

    const std::wstring 后台路径 = 取后台可执行路径();
    if (后台路径.empty()) {
        if (错误文本) *错误文本 = _T("未找到鱼巢.exe，无法运行任务控制自检。");
        return false;
    }

    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = nullptr;
    sa.bInheritHandle = TRUE;

    HANDLE 读管道 = nullptr;
    HANDLE 写管道 = nullptr;
    if (!::CreatePipe(&读管道, &写管道, &sa, 0)) {
        if (错误文本) *错误文本 = _T("创建输出管道失败。");
        return false;
    }
    ::SetHandleInformation(读管道, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOW si{};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.wShowWindow = SW_HIDE;
    si.hStdInput = ::GetStdHandle(STD_INPUT_HANDLE);
    si.hStdOutput = 写管道;
    si.hStdError = 写管道;

    PROCESS_INFORMATION pi{};
    std::filesystem::path 后台文件路径(后台路径);
    const std::wstring 工作目录 = 后台文件路径.parent_path().wstring();
    std::wstring 命令行 = L"\"" + 后台路径 + L"\" --run-task-control-regression";
    BOOL ok = ::CreateProcessW(
        nullptr,
        命令行.data(),
        nullptr,
        nullptr,
        TRUE,
        CREATE_NO_WINDOW,
        nullptr,
        工作目录.empty() ? nullptr : 工作目录.c_str(),
        &si,
        &pi);

    ::CloseHandle(写管道);
    写管道 = nullptr;

    if (!ok) {
        ::CloseHandle(读管道);
        if (错误文本) *错误文本 = _T("启动鱼巢 回归套件失败。");
        return false;
    }

    const DWORD 等待结果 = ::WaitForSingleObject(pi.hProcess, 30000);
    if (等待结果 == WAIT_TIMEOUT) {
        ::TerminateProcess(pi.hProcess, 124);
        ::CloseHandle(pi.hThread);
        ::CloseHandle(pi.hProcess);
        ::CloseHandle(读管道);
        if (错误文本) *错误文本 = _T("任务控制自检超时（30 秒）。");
        return false;
    }

    std::string 输出UTF8;
    char 缓冲[4096] = {};
    DWORD 已读取 = 0;
    while (::ReadFile(读管道, 缓冲, static_cast<DWORD>(sizeof(缓冲)), &已读取, nullptr) && 已读取 > 0) {
        输出UTF8.append(缓冲, 缓冲 + 已读取);
    }

    DWORD 退出码 = 1;
    ::GetExitCodeProcess(pi.hProcess, &退出码);
    ::CloseHandle(pi.hThread);
    ::CloseHandle(pi.hProcess);
    ::CloseHandle(读管道);

    CString 输出 = UTF8到CString_(输出UTF8);
    输出.Trim();
    if (输出文本) {
        if (!输出.IsEmpty()) {
            *输出文本 = 输出;
        }
        else {
            输出文本->Format(_T("任务控制自检已结束，退出码=%lu。"), 退出码);
        }
    }

    if (退出码 != 0) {
        if (错误文本) {
            if (!输出.IsEmpty()) {
                错误文本->Format(_T("任务控制自检失败，退出码=%lu。"), 退出码);
            }
            else {
                错误文本->Format(_T("任务控制自检失败，且没有返回输出，退出码=%lu。"), 退出码);
            }
        }
        return false;
    }

    return true;
}

bool C海鱼Dlg::请求远程后台安全停机(CString* 错误文本)
{
	if (!使用远程后台模式_) {
		if (错误文本) *错误文本 = _T("当前不是远程后台模式。");
		return false;
	}

	std::string 错误;
	if (!后台客户端_.请求后台安全停机(&错误, 1000)) {
		if (后台进程仍在运行()) {
			CString 兜底错误;
			if (!强制结束已跟踪后台进程(&兜底错误)) {
				if (错误文本) *错误文本 = UTF8到CString_(错误.empty() ? "后台未响应安全停机请求" : 错误) + _T("\n") + 兜底错误;
				return false;
			}
			日志::运行("[前端界面] 后台未响应安全停机请求，已回退为强制结束已跟踪后台进程");
		}
		else {
			if (错误文本) *错误文本 = UTF8到CString_(错误.empty() ? "后台未响应安全停机请求" : 错误);
			return false;
		}
	}
	else {
		for (int i = 0; i < 20; ++i) {
			::Sleep(200);
			if (!尝试刷新后台快照()) {
				break;
			}
		}
		if (后台进程仍在运行()) {
			CString 兜底错误;
			(void)强制结束已跟踪后台进程(&兜底错误);
		}
	}

	后台在线_ = false;
	最近后台快照_.后台在线 = false;
	最近后台快照_.最近命令摘要 = "已由前端请求后台安全停机";
	if (::IsWindow(变量_交互界面.GetSafeHwnd())) {
		变量_交互界面.刷新远程后台状态();
	}
	return true;
}

bool C海鱼Dlg::启动远程相机观测(CString* 错误文本)
{
	if (!使用远程后台模式_) {
		if (错误文本) *错误文本 = _T("当前不是远程后台模式。");
		return false;
	}
	if (后台连接中_.load()) {
		if (错误文本) *错误文本 = _T("后台正在连接中，请稍候。");
		return false;
	}
	if (!后台在线_ && !尝试刷新后台快照()) {
		if (错误文本) *错误文本 = _T("后台当前离线，请先启动鱼巢。");
		return false;
	}

	std::string 错误;
	if (!后台客户端_.启动相机观测(&错误, 1200)) {
		后台在线_ = false;
		最近后台快照_.后台在线 = false;
		最近后台快照_.最近命令摘要 = 错误.empty() ? "后台未响应相机启动请求" : 错误;
		if (错误文本) *错误文本 = UTF8到CString_(最近后台快照_.最近命令摘要);
		if (::IsWindow(变量_交互界面.GetSafeHwnd())) {
			变量_交互界面.刷新远程后台状态();
		}
		return false;
	}

	(void)尝试刷新后台快照();
	最近后台快照_.最近命令摘要 = "已请求后台启动相机";
	if (::IsWindow(变量_交互界面.GetSafeHwnd())) {
		变量_交互界面.刷新远程后台状态();
	}
	return true;
}

bool C海鱼Dlg::停止远程相机观测(CString* 错误文本)
{
	if (!使用远程后台模式_) {
		return true;
	}
	if (后台连接中_.load()) {
		if (错误文本) *错误文本 = _T("后台正在连接中，请稍候。");
		return false;
	}
	if (!后台在线_ && !尝试刷新后台快照()) {
		return true;
	}

	std::string 错误;
	if (!后台客户端_.停止相机观测(&错误, 1200)) {
		后台在线_ = false;
		最近后台快照_.后台在线 = false;
		最近后台快照_.最近命令摘要 = 错误.empty() ? "后台未响应相机停止请求" : 错误;
		if (错误文本) *错误文本 = UTF8到CString_(最近后台快照_.最近命令摘要);
		if (::IsWindow(变量_交互界面.GetSafeHwnd())) {
			变量_交互界面.刷新远程后台状态();
		}
		return false;
	}

	(void)尝试刷新后台快照();
	最近后台快照_.最近命令摘要 = "已请求后台停止相机";
	if (::IsWindow(变量_交互界面.GetSafeHwnd())) {
		变量_交互界面.刷新远程后台状态();
	}
	return true;
}

bool C海鱼Dlg::启动远程自我场景再现(CString* 错误文本)
{
	if (!使用远程后台模式_) {
		if (错误文本) *错误文本 = _T("当前不是远程后台模式。");
		return false;
	}
	if (后台连接中_.load()) {
		if (错误文本) *错误文本 = _T("后台正在连接中，请稍候。");
		return false;
	}
	if (!后台在线_ && !尝试刷新后台快照()) {
		if (错误文本) *错误文本 = _T("后台当前离线，请先启动鱼巢。");
		return false;
	}

	std::string 错误;
	if (!后台客户端_.启动自我场景再现(&错误, 1000)) {
		if (错误文本) *错误文本 = UTF8到CString_(错误.empty() ? "后台未响应自我场景再现启动请求" : 错误);
		return false;
	}
	return true;
}

bool C海鱼Dlg::停止远程自我场景再现(CString* 错误文本)
{
	if (自我场景再现窗口_) {
		if (::IsWindow(自我场景再现窗口_->GetSafeHwnd())) {
			自我场景再现窗口_->DestroyWindow();
		}
		自我场景再现窗口_.reset();
	}

	if (!使用远程后台模式_) {
		return true;
	}
	if (!后台在线_ && !尝试刷新后台快照()) {
		return true;
	}

	std::string 错误;
	if (!后台客户端_.停止自我场景再现(&错误, 1000)) {
		if (错误文本) *错误文本 = UTF8到CString_(错误.empty() ? "后台未响应自我场景再现停止请求" : 错误);
		return false;
	}
	return true;
}

bool C海鱼Dlg::查询远程自我场景快照(结构_后台自我场景快照* 输出, CString* 错误文本)
{
	if (!输出) {
		if (错误文本) *错误文本 = _T("输出缓冲为空。");
		return false;
	}
	if (!使用远程后台模式_) {
		if (错误文本) *错误文本 = _T("当前不是远程后台模式。");
		return false;
	}
	if (后台连接中_.load()) {
		if (错误文本) *错误文本 = _T("后台正在连接中，请稍候。");
		return false;
	}
	if (!后台在线_ && !尝试刷新后台快照()) {
		if (错误文本) *错误文本 = _T("后台当前离线。");
		return false;
	}

	std::string 错误;
	if (!后台客户端_.查询自我场景快照(输出, &错误, 500)) {
		最近后台自我场景快照_ = {};
		最近后台自我场景快照_.后台在线 = 后台在线_;
		最近后台自我场景快照有效_ = false;
		最近后台自我场景错误_ = 错误.empty() ? "后台未响应自我场景快照请求" : 错误;
		if (错误文本) *错误文本 = UTF8到CString_(错误.empty() ? "后台未响应自我场景快照请求" : 错误);
		return false;
	}
	最近后台自我场景快照_ = *输出;
	最近后台自我场景快照_.后台在线 = 后台在线_;
	最近后台自我场景快照有效_ = true;
	最近后台自我场景错误_.clear();
	return true;
}

std::wstring C海鱼Dlg::取后台可执行路径() const
{
	wchar_t 模块路径[MAX_PATH] = {};
	if (::GetModuleFileNameW(nullptr, 模块路径, MAX_PATH) == 0) {
		return {};
	}

	std::filesystem::path 路径(模块路径);
	路径 = 路径.parent_path() / L"鱼巢.exe";
	if (!std::filesystem::exists(路径)) {
		return {};
	}
	return 路径.wstring();
}

bool C海鱼Dlg::尝试拉起后台进程()
{
	后台进程路径_ = 取后台可执行路径();
	return 按路径拉起后台进程_(后台进程路径_);
}

bool C海鱼Dlg::尝试连接或拉起后台()
{
	if (尝试刷新后台快照()) {
		return true;
	}
	if (!尝试拉起后台进程()) {
		return false;
	}
	for (int i = 0; i < 20; ++i) {
		::Sleep(250);
		if (尝试刷新后台快照()) {
			return true;
		}
	}
	return false;
}

C海鱼Dlg* 主窗口指针;

// ============================================================================
// D455 相机外设（把 D455_相机实现 适配到 外设运行框架）
// ============================================================================
struct C海鱼Dlg::摄像机运行时 {
	相机观测运行时类 实现{};

	bool 启动() { return 实现.启动(); }
	void 停止() { 实现.停止(); }
	void 锁定观测目标(const std::string& 存在主键) { 实现.锁定观测目标(存在主键); }
	void 解除锁定观测() { 实现.解除锁定观测(); }
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
	CString 错误文本;
	if (!启动远程相机观测(&错误文本)) {
		AfxMessageBox(_T("启动相机失败：\n") + 错误文本);
		return;
	}
	日志::运行("[前端界面] 已提交远程相机启动请求");
}


void C海鱼Dlg::停止外设()
{
	CString 错误文本;
	if (!停止远程相机观测(&错误文本) && !错误文本.IsEmpty()) {
		AfxMessageBox(_T("停止相机失败：\n") + 错误文本);
		return;
	}
	日志::运行("[前端界面] 已提交远程相机停止请求");
}


void C海鱼Dlg::锁定观测目标(const std::string& 存在主键)
{
	if (存在主键.empty()) {
		日志::运行("[前端界面] 观测锁定请求被忽略：目标主键为空");
		return;
	}

	if (!使用远程后台模式_) {
		if (摄像机_) {
			摄像机_->锁定观测目标(存在主键);
		}
		return;
	}
	if (后台连接中_.load()) {
		最近后台快照_.最近命令摘要 = "后台连接中，暂无法锁定观测目标";
		return;
	}
	if (!后台在线_ && !尝试刷新后台快照()) {
		最近后台快照_.最近命令摘要 = "后台离线，观测目标锁定失败";
		if (::IsWindow(变量_交互界面.GetSafeHwnd())) {
			变量_交互界面.刷新远程后台状态();
		}
		return;
	}

	std::string 错误;
	if (!后台客户端_.锁定观测目标(存在主键, &错误, 1000)) {
		后台在线_ = false;
		最近后台快照_.后台在线 = false;
		最近后台快照_.最近命令摘要 = 错误.empty() ? "后台未响应观测目标锁定请求" : 错误;
		日志::运行("[前端界面] 观测锁定失败：" + 最近后台快照_.最近命令摘要);
	}
	else {
		(void)尝试刷新后台快照();
		最近后台快照_.最近命令摘要 = "已锁定观测目标: " + 存在主键;
		日志::运行("[前端界面] 已提交观测锁定请求: " + 存在主键);
	}

	if (::IsWindow(变量_交互界面.GetSafeHwnd())) {
		变量_交互界面.刷新远程后台状态();
	}
}

void C海鱼Dlg::解除锁定观测()
{
	if (!使用远程后台模式_) {
		if (摄像机_) {
			摄像机_->解除锁定观测();
		}
		return;
	}
	if (后台连接中_.load()) {
		最近后台快照_.最近命令摘要 = "后台连接中，暂无法解除观测锁定";
		return;
	}
	if (!后台在线_ && !尝试刷新后台快照()) {
		最近后台快照_.最近命令摘要 = "后台离线，无法解除观测锁定";
		if (::IsWindow(变量_交互界面.GetSafeHwnd())) {
			变量_交互界面.刷新远程后台状态();
		}
		return;
	}

	std::string 错误;
	if (!后台客户端_.解除观测锁定(&错误, 1000)) {
		后台在线_ = false;
		最近后台快照_.后台在线 = false;
		最近后台快照_.最近命令摘要 = 错误.empty() ? "后台未响应观测目标解锁请求" : 错误;
		日志::运行("[前端界面] 观测解锁失败：" + 最近后台快照_.最近命令摘要);
	}
	else {
		(void)尝试刷新后台快照();
		最近后台快照_.最近命令摘要 = "已解除观测目标锁定";
		日志::运行("[前端界面] 已提交观测解锁请求");
	}

	if (::IsWindow(变量_交互界面.GetSafeHwnd())) {
		变量_交互界面.刷新远程后台状态();
	}
}

void C海鱼Dlg::刷新场景显示布局()
{
	if (!自我场景再现窗口_ || !::IsWindow(自我场景再现窗口_->GetSafeHwnd())) {
		return;
	}
	if (!::IsWindow(变量_配置及测试窗口.GetSafeHwnd())) {
		return;
	}

	CRect 承载区域;
	if (!变量_配置及测试窗口.获取自我场景承载区(&承载区域)) {
		return;
	}
	自我场景再现窗口_->MoveWindow(&承载区域, TRUE);
}


void C海鱼Dlg::启动场景实时显示()
{
	CString 错误文本;
	if (!启动远程自我场景再现(&错误文本)) {
		AfxMessageBox(_T("启动自我场景再现失败：\n") + 错误文本);
		return;
	}

	if (!自我场景再现窗口_) {
		自我场景再现窗口_ = std::make_unique<C自我场景再现窗口>(this);
	}
	if (!::IsWindow(变量_配置及测试窗口.GetSafeHwnd())) {
		AfxMessageBox(_T("控制面板尚未初始化，无法创建内嵌场景窗口。"));
		return;
	}
	if (!::IsWindow(自我场景再现窗口_->GetSafeHwnd())) {
		if (!自我场景再现窗口_->Create(IDD_DIALOG_SELF_SCENE_REPLAY, &变量_配置及测试窗口)) {
			自我场景再现窗口_.reset();
			AfxMessageBox(_T("创建控制面板内嵌场景窗口失败。"));
			return;
		}
	}

	刷新场景显示布局();
	自我场景再现窗口_->ShowWindow(SW_SHOW);
	自我场景再现窗口_->SetWindowPos(
		&wndTop,
		0,
		0,
		0,
		0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	日志::运行("[前端界面] 已启动控制面板内嵌自我场景实时显示");
}

void C海鱼Dlg::停止场景实时显示()
{
	CString 错误文本;
	if (!停止远程自我场景再现(&错误文本) && !错误文本.IsEmpty()) {
		AfxMessageBox(_T("停止自我场景再现失败：\n") + 错误文本);
		return;
	}
	日志::运行("[前端界面] 已停止控制面板内嵌自我场景实时显示");
}

void C海鱼Dlg::提交场景显示(
	std::shared_ptr<结构体_原始场景帧> 帧,
	std::shared_ptr<std::vector<结构体_存在观测>> 观测)
{
	(void)帧;
	(void)观测;
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












