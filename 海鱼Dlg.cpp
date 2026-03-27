// 海鱼Dlg.cpp: 实现文件
//

#include "pch.h"
#include <mutex>
#include <string>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <variant>
#include <unordered_map>
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
import 自治宿主模块;
import 因果信息模块;

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

	constexpr UINT IDC_标签内容查看框 = 0x52A1;
	constexpr UINT IDC_因果树查看框 = 0x52A2;

	void 提示控制面板不直接管理外设_(CWnd* 宿主窗口)
	{
		AfxMessageBox(_T("鱼巢控制面板不再直接启动本地外设、场景显示或自我线程。\n请先连接或启动后台“鱼巢”，再通过后台统一查看和管理运行状态。"));
		if (宿主窗口 && ::IsWindow(宿主窗口->GetSafeHwnd())) {
			宿主窗口->SetForegroundWindow();
		}
	}

	std::string 裁剪摘要文本_(std::string 文本, std::size_t 最大长度 = 120)
	{
		if (文本.size() <= 最大长度) return 文本;
		if (最大长度 < 3) return 文本.substr(0, 最大长度);
		return 文本.substr(0, 最大长度 - 3) + "...";
	}

	const 词性节点类* 取状态特征类型_(const 状态节点类* 状态);
	std::size_t 取因果条件状态数_(const 因果主信息类* 信息);

	bool 尝试取当前自我特征I64_(
		const 词性节点类* 特征类型,
		I64* 输出值)
	{
		if (!输出值 || !特征类型) return false;
		auto* 自我存在 = 世界树.自我指针;
		if (!自我存在) return false;

		const auto 快照 = 世界树.读取特征快照(
			自我存在,
			特征类型,
			"海鱼Dlg::尝试取当前自我特征I64");
		if (!快照.has_value()) return false;

		if (const auto* 值 = std::get_if<I64>(&*快照)) {
			*输出值 = *值;
			return true;
		}
		return false;
	}

	场景节点类* 查找直系子场景_按名称_(场景节点类* 父场景, const char* 名称)
	{
		if (!父场景 || !名称 || !*名称) return nullptr;
		const auto* 目标名称 = 语素集.添加词性词(名称, "名词");
		for (auto* 场景 : 世界树.获取子场景(父场景, "C海鱼Dlg::查找直系子场景_按名称_")) {
			auto* 信息 = (场景 && 场景->主信息) ? dynamic_cast<场景节点主信息类*>(场景->主信息) : nullptr;
			if (信息 && 信息->名称 == 目标名称) return 场景;
		}
		return nullptr;
	}

	场景节点类* 确保人类输入桥场景_(const char* 名称, const char* 类型, 时间戳 now)
	{
		auto* 父场景 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
		if (!父场景) return nullptr;
		if (auto* 已有 = 查找直系子场景_按名称_(父场景, 名称)) {
			if (auto* 信息 = 已有->主信息 ? dynamic_cast<场景节点主信息类*>(已有->主信息) : nullptr) {
				信息->最后观测时间 = now;
			}
			return 已有;
		}

		auto* 信息 = new 场景节点主信息类();
		信息->名称 = 语素集.添加词性词(名称, "名词");
		信息->类型 = 语素集.添加词性词(类型, "名词");
		信息->最后观测时间 = now;
		return 世界树.创建场景(父场景, 信息, std::string("C海鱼Dlg::确保人类输入桥场景_/") + 名称);
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

	std::string 快照文本_(const 特征快照值& v);

	std::string 取词文本_(const 词性节点类* 词) {
		if (!词 || !词->父 || !词->父->主信息) return {};
		auto* 词信息 = dynamic_cast<词主信息类*>(词->父->主信息);
		return 词信息 ? 词信息->词 : std::string{};
	}

	std::string 词键_(const 词性节点类* 词) {
		return 词 ? 词->获取主键() : std::string{};
	}

	std::string 词显示文本_(const 词性节点类* 词) {
		auto 文本 = 取词文本_(词);
		if (!文本.empty()) return 文本;
		return 词键_(词);
	}

	const 词性节点类* 取特征节点类型_(const 特征节点类* 特征) {
		auto* 特征信息 = (特征 && 特征->主信息)
			? dynamic_cast<特征节点主信息类*>(特征->主信息)
			: nullptr;
		return 特征信息 ? 特征信息->类型 : nullptr;
	}

	const 词性节点类* 取状态特征类型_(const 状态节点类* 状态) {
		auto* 状态信息 = (状态 && 状态->主信息)
			? dynamic_cast<状态节点主信息类*>(状态->主信息)
			: nullptr;
		return 状态信息 ? 取特征节点类型_(状态信息->状态特征) : nullptr;
	}

	const 词性节点类* 取状态特征类型_(const 状态节点主信息类* 状态信息) {
		return 状态信息 ? 取特征节点类型_(状态信息->状态特征) : nullptr;
	}

	const 词性节点类* 取动态特征类型_(const 动态节点类* 动态) {
		auto* 动态信息 = (动态 && 动态->主信息)
			? dynamic_cast<动态节点主信息类*>(动态->主信息)
			: nullptr;
		return 动态信息 ? 取特征节点类型_(动态信息->动态特征) : nullptr;
	}

	std::string 取状态特征显示文本_(const 状态节点类* 状态) {
		auto 文本 = 词显示文本_(取状态特征类型_(状态));
		return 文本.empty() ? std::string("<未知特征>") : 文本;
	}

	std::string 取状态特征显示文本_(const 状态节点主信息类* 状态信息) {
		auto 文本 = 词显示文本_(取状态特征类型_(状态信息));
		return 文本.empty() ? std::string("<未知特征>") : 文本;
	}

	std::string 取动态特征显示文本_(const 动态节点类* 动态) {
		auto 文本 = 词显示文本_(取动态特征类型_(动态));
		return 文本.empty() ? std::string("<未知特征>") : 文本;
	}

	std::string 节点显示文本_(const 基础信息节点类* 节点) {
		if (!节点) return "<未知主体>";
		const auto 主键 = 节点->获取主键();
		if (!主键.empty()) return 裁剪摘要文本_(主键, 64);
		return "<无主键主体>";
	}

	std::string 取状态主体显示文本_(const 状态节点类* 状态) {
		auto* 状态信息 = (状态 && 状态->主信息)
			? dynamic_cast<状态节点主信息类*>(状态->主信息)
			: nullptr;
		return 状态信息 ? 节点显示文本_(状态信息->状态主体) : std::string("<未知主体>");
	}

	std::string 取状态主体显示文本_(const 状态节点主信息类* 状态信息) {
		return 状态信息 ? 节点显示文本_(状态信息->状态主体) : std::string("<未知主体>");
	}

	std::string 生成状态描述键_(const 状态节点类* 状态) {
		if (!状态) return "<none>";
		return 取状态主体显示文本_(状态) + "." + 取状态特征显示文本_(状态);
	}

	std::string 生成状态描述键_(const 状态节点主信息类* 状态信息) {
		if (!状态信息) return "<none>";
		return 取状态主体显示文本_(状态信息) + "." + 取状态特征显示文本_(状态信息);
	}

	std::string 生成状态标准描述_(const 状态节点类* 状态) {
		auto* 状态信息 = (状态 && 状态->主信息)
			? dynamic_cast<状态节点主信息类*>(状态->主信息)
			: nullptr;
		if (!状态信息) return "<none>";
		return 生成状态描述键_(状态) + "=" + 快照文本_(状态信息->状态值);
	}

	std::string 生成状态标准描述_(const 状态节点主信息类* 状态信息) {
		if (!状态信息) return "<none>";
		return 生成状态描述键_(状态信息) + "=" + 快照文本_(状态信息->状态值);
	}

	void 追加唯一文本_(std::vector<std::string>& 列表, const std::string& 文本) {
		if (文本.empty()) return;
		if (std::find(列表.begin(), 列表.end(), 文本) == 列表.end()) {
			列表.push_back(文本);
		}
	}

	std::string 拼接文本列表_(const std::vector<std::string>& 列表, const char* 分隔符) {
		if (列表.empty()) return "<none>";
		std::string out;
		for (std::size_t i = 0; i < 列表.size(); ++i) {
			if (i != 0) out += 分隔符;
			out += 列表[i];
		}
		return out;
	}

	std::vector<std::string> 收集状态特征名称列表_(const std::vector<状态节点类*>& 状态列表) {
		std::vector<std::string> out{};
		for (auto* 状态 : 状态列表) {
			追加唯一文本_(out, 生成状态描述键_(状态));
		}
		return out;
	}

	std::vector<std::string> 收集状态特征详情列表_(const std::vector<状态节点类*>& 状态列表) {
		struct 结构_特征值摘要 {
			时间戳 最近时间 = 0;
			std::string 值文本{};
			bool 已命中 = false;
		};

		std::vector<std::string> 顺序{};
		std::unordered_map<std::string, 结构_特征值摘要> 最新值{};
		for (auto* 状态 : 状态列表) {
			auto* 状态信息 = (状态 && 状态->主信息)
				? dynamic_cast<状态节点主信息类*>(状态->主信息)
				: nullptr;
			if (!状态信息) continue;

			const auto 状态键 = 生成状态描述键_(状态);
			if (状态键.empty()) continue;
			追加唯一文本_(顺序, 状态键);

			const 时间戳 当前时间 = 状态信息->发生时间 ? 状态信息->发生时间 : 状态信息->收到时间;
			auto& 摘要 = 最新值[状态键];
			if (!摘要.已命中 || 当前时间 >= 摘要.最近时间) {
				摘要.已命中 = true;
				摘要.最近时间 = 当前时间;
				摘要.值文本 = 快照文本_(状态信息->状态值);
			}
		}

		std::vector<std::string> out{};
		for (const auto& 状态键 : 顺序) {
			auto it = 最新值.find(状态键);
			if (it == 最新值.end() || !it->second.已命中) {
				out.push_back(状态键);
				continue;
			}
			out.push_back(状态键 + "=" + it->second.值文本);
		}
		return out;
	}

	std::vector<std::string> 收集动态路径特征列表_(const std::vector<动态节点类*>& 动态列表) {
		std::vector<std::string> out{};
		for (auto* 动态 : 动态列表) {
			const auto 特征名 = 取动态特征显示文本_(动态);
			if (!out.empty() && out.back() == 特征名) continue;
			out.push_back(特征名);
		}
		return out;
	}

	bool 是显式动作动态_(const 动态节点类* 动态) {
		auto* 动态信息 = (动态 && 动态->主信息)
			? dynamic_cast<动态节点主信息类*>(动态->主信息)
			: nullptr;
		if (!动态信息) return false;
		if (动态信息->来源类型 != 枚举_动态来源类型::未定义) return true;
		if (动态信息->来源动作名 || 动态信息->来源方法首节点) return true;
		return 动态信息->初始状态 == nullptr
			&& 动态信息->结果状态 == nullptr
			&& 动态信息->动态主体 != nullptr
			&& 动态信息->动态特征 != nullptr;
	}

	动态节点类* 取因动作动态_(const 因果主信息类* 信息) {
		if (!信息) return nullptr;
		auto 打分 = [&](const 动态节点类* 动态) -> int {
			if (!是显式动作动态_(动态)) return -1;
			auto* 动态信息 = (动态 && 动态->主信息)
				? dynamic_cast<动态节点主信息类*>(动态->主信息)
				: nullptr;
			if (!动态信息) return -1;
			switch (动态信息->来源动作相位) {
			case 枚举_动作事件相位::开始运行:
				return 3;
			case 枚举_动作事件相位::未定义:
				return 2;
			case 枚举_动作事件相位::完成:
			case 枚举_动作事件相位::失败:
				return 1;
			default:
				return 0;
			}
		};
		动态节点类* 最优 = nullptr;
		int 最优分 = -1;
		auto 更新最优 = [&](动态节点类* 动态) {
			const int 分 = 打分(动态);
			if (分 > 最优分) {
				最优 = 动态;
				最优分 = 分;
			}
		};
		更新最优(信息->因信息);
		for (auto* 动态 : 信息->过程动态列表) {
			更新最优(动态);
		}
		if (最优) return 最优;
		if (信息->因信息) return 信息->因信息;
		return 信息->过程动态列表.empty() ? nullptr : 信息->过程动态列表.front();
	}

	std::string 动作事件相位显示文本_(枚举_动作事件相位 相位) {
		switch (相位) {
		case 枚举_动作事件相位::开始运行:
			return "开始运行";
		case 枚举_动作事件相位::完成:
			return "完成";
		case 枚举_动作事件相位::失败:
			return "失败";
		default:
			return {};
		}
	}

	std::vector<动态节点类*> 提取过程动态_不含因动作_(const 因果主信息类* 信息) {
		std::vector<动态节点类*> out{};
		if (!信息) return out;
		auto* 因动作 = 取因动作动态_(信息);
		bool 已跳过因动作 = false;
		for (auto* 动态 : 信息->过程动态列表) {
			if (!已跳过因动作 && 动态 == 因动作) {
				已跳过因动作 = true;
				continue;
			}
			out.push_back(动态);
		}
		return out;
	}

	std::string 取动态来源动作显示文本_(const 动态节点类* 动态) {
		auto* 动态信息 = (动态 && 动态->主信息)
			? dynamic_cast<动态节点主信息类*>(动态->主信息)
			: nullptr;
		if (!动态信息) return {};
		if (动态信息->来源动作名) {
			return 词显示文本_(动态信息->来源动作名);
		}
		if (动态信息->来源方法首节点 && 动态信息->来源方法首节点->主信息) {
			if (auto* 方法首信息 = dynamic_cast<const 方法首节点主信息类*>(动态信息->来源方法首节点->主信息)) {
				if (方法首信息->动作名) {
					return 词显示文本_(方法首信息->动作名);
				}
			}
		}
		return {};
	}

	std::string 汇总因动作详情_(const 因果主信息类* 信息) {
		auto* 动作 = 取因动作动态_(信息);
		if (!动作) return "<none>";
		if (!是显式动作动态_(动作)) {
			auto 文本 = 取动态特征显示文本_(动作);
			return 文本.empty() ? "<none>" : 文本;
		}
		auto 文本 = 取动态来源动作显示文本_(动作);
		if (文本.empty()) {
			文本 = 取动态特征显示文本_(动作);
		}
		return 文本.empty() ? "<none>" : 文本;
	}

	std::string 取因动作标签_(const 因果主信息类* 信息) {
		auto* 动作 = 取因动作动态_(信息);
		if (!动作) return {};
		return 是显式动作动态_(动作) ? "动作" : "状态变化";
	}

	std::string 生成因动作模式键_(const 因果主信息类* 信息) {
		auto* 动作 = 取因动作动态_(信息);
		auto* 动态信息 = (动作 && 动作->主信息)
			? dynamic_cast<动态节点主信息类*>(动作->主信息)
			: nullptr;
		if (!动态信息) return "none";
		return 取动态特征显示文本_(动作) + "|path=" + std::to_string(动态信息->动态路径签名);
	}

	std::string 汇总因果状态特征_(const std::vector<状态节点类*>& 状态列表) {
		return 拼接文本列表_(收集状态特征名称列表_(状态列表), "、");
	}

	std::string 汇总因果动态特征_(const std::vector<动态节点类*>& 动态列表) {
		std::vector<std::string> 文本列表{};
		for (auto* 动态 : 动态列表) {
			auto 文本 = 取动态来源动作显示文本_(动态);
			if (文本.empty()) {
				文本 = 取动态特征显示文本_(动态);
			}
			if (!文本.empty()) {
				文本列表.push_back(文本);
			}
		}
		return 拼接文本列表_(文本列表, " -> ");
	}

	std::string 单状态值文本_(const 状态节点类* 状态) {
		auto* 状态信息 = (状态 && 状态->主信息)
			? dynamic_cast<状态节点主信息类*>(状态->主信息)
			: nullptr;
		return 状态信息 ? 快照文本_(状态信息->状态值) : std::string("<none>");
	}

	std::string 单状态值文本_(const 状态节点主信息类* 状态信息) {
		return 状态信息 ? 快照文本_(状态信息->状态值) : std::string("<none>");
	}

	std::string 方向自然语言文本_(bool 可比较, std::int64_t 方向值);

	std::string 取状态原因类别_(const 状态节点类* 状态) {
		auto* 状态信息 = (状态 && 状态->主信息)
			? dynamic_cast<状态节点主信息类*>(状态->主信息)
			: nullptr;
		return 状态信息 ? 状态信息->变化原因类别 : std::string{};
	}

	std::string 取状态原因类别_(const 状态节点主信息类* 状态信息) {
		return 状态信息 ? 状态信息->变化原因类别 : std::string{};
	}

	std::string 取状态原因说明_(const 状态节点类* 状态) {
		auto* 状态信息 = (状态 && 状态->主信息)
			? dynamic_cast<状态节点主信息类*>(状态->主信息)
			: nullptr;
		return 状态信息 ? 状态信息->变化原因说明 : std::string{};
	}

	std::string 取状态原因说明_(const 状态节点主信息类* 状态信息) {
		return 状态信息 ? 状态信息->变化原因说明 : std::string{};
	}

	std::string 取状态原因显示文本_(const 状态节点类* 状态) {
		auto 文本 = 取状态原因说明_(状态);
		if (!文本.empty()) return 文本;
		return 取状态原因类别_(状态);
	}

	std::string 取状态原因显示文本_(const 状态节点主信息类* 状态信息) {
		auto 文本 = 取状态原因说明_(状态信息);
		if (!文本.empty()) return 文本;
		return 取状态原因类别_(状态信息);
	}

	std::string 单状态静态短语_(const 状态节点类* 状态, bool 强调保持 = false) {
		if (!状态) return "<none>";
		auto out = 生成状态标准描述_(状态);
		if (强调保持) out += "（保持）";
		return out;
	}

	std::string 单状态静态短语_(const 状态节点主信息类* 状态信息, bool 强调保持 = false) {
		if (!状态信息) return "<none>";
		auto out = 生成状态标准描述_(状态信息);
		if (强调保持) out += "（保持）";
		return out;
	}

	std::string 生成状态方向自然语言短语_(
		const 状态节点类* 状态,
		bool 可比较,
		std::int64_t 方向值) {
		if (!状态) return "<none>";
		auto out = 生成状态标准描述_(状态);
		if (可比较 && 方向值 != 0) {
			return out + "（" + 方向自然语言文本_(可比较, 方向值) + "）";
		}
		if (可比较) {
			return 单状态静态短语_(状态, true);
		}
		return 单状态静态短语_(状态, false);
	}

	std::string 生成状态方向自然语言短语_(
		const 状态节点主信息类* 状态信息,
		bool 可比较,
		std::int64_t 方向值) {
		if (!状态信息) return "<none>";
		auto out = 生成状态标准描述_(状态信息);
		if (可比较 && 方向值 != 0) {
			return out + "（" + 方向自然语言文本_(可比较, 方向值) + "）";
		}
		if (可比较) {
			return 单状态静态短语_(状态信息, true);
		}
		return 单状态静态短语_(状态信息, false);
	}

	std::string 方向显示文本_(bool 可比较, std::int64_t 方向值) {
		if (!可比较) return "不可比较";
		if (方向值 > 0) return "上升(+1)";
		if (方向值 < 0) return "下降(-1)";
		return "持平(0)";
	}

	std::string 方向自然语言文本_(bool 可比较, std::int64_t 方向值) {
		if (!可比较) return "发生变化";
		if (方向值 > 0) return "变大";
		if (方向值 < 0) return "变小";
		return "保持不变";
	}

	std::string 生成状态结果描述_(const 状态节点类* 状态, bool 可比较, std::int64_t 方向值) {
		if (!状态) return "<none>";
		auto out = 生成状态标准描述_(状态);
		if (可比较) {
			out += "（" + 方向自然语言文本_(可比较, 方向值) + "）";
		}
		return out;
	}

	std::string 生成状态结果描述_(const 状态节点主信息类* 状态信息, bool 可比较, std::int64_t 方向值) {
		if (!状态信息) return "<none>";
		auto out = 生成状态标准描述_(状态信息);
		if (可比较) {
			out += "（" + 方向自然语言文本_(可比较, 方向值) + "）";
		}
		return out;
	}

	std::string 果方向显示文本_(const 因果主信息类* 信息) {
		if (!信息) return "<未定义>";
		return 方向显示文本_(信息->结果方向可比较, 信息->结果方向值);
	}

	std::string 条件方向显示文本_(const 因果主信息类* 信息, std::size_t 序号) {
		if (!信息 || 序号 >= 取因果条件状态数_(信息)) return "<未定义>";
		const bool 可比较 = 序号 < 信息->条件方向可比较列表.size()
			&& 信息->条件方向可比较列表[序号] != 0;
		const std::int64_t 方向值 = 序号 < 信息->条件方向值列表.size()
			? 信息->条件方向值列表[序号]
			: 0;
		return 方向显示文本_(可比较, 方向值);
	}

	std::string 条件方向自然语言文本_(const 因果主信息类* 信息, std::size_t 序号) {
		if (!信息 || 序号 >= 取因果条件状态数_(信息)) return "发生变化";
		const bool 可比较 = 序号 < 信息->条件方向可比较列表.size()
			&& 信息->条件方向可比较列表[序号] != 0;
		const std::int64_t 方向值 = 序号 < 信息->条件方向值列表.size()
			? 信息->条件方向值列表[序号]
			: 0;
		return 方向自然语言文本_(可比较, 方向值);
	}

	状态节点类* 取因果主结果状态_(const 因果主信息类* 信息) {
		if (!信息) return nullptr;
		return 信息->主结果状态 ? 信息->主结果状态 : 信息->结果状态;
	}

	const 状态节点主信息类* 取因果主结果状态主信息_(const 因果主信息类* 信息) {
		if (!信息) return nullptr;
		if (!信息->主结果状态主信息列表.empty()) return &信息->主结果状态主信息列表.back();
		if (信息->主结果状态 && 信息->主结果状态->主信息) {
			return dynamic_cast<状态节点主信息类*>(信息->主结果状态->主信息);
		}
		if (!信息->结果状态主信息列表.empty()) {
			return &信息->结果状态主信息列表.back();
		}
		auto* 结果状态 = 取因果主结果状态_(信息);
		return (结果状态 && 结果状态->主信息)
			? dynamic_cast<状态节点主信息类*>(结果状态->主信息)
			: nullptr;
	}

	std::size_t 取因果条件状态数_(const 因果主信息类* 信息) {
		if (!信息) return 0;
		return !信息->条件状态主信息列表.empty()
			? 信息->条件状态主信息列表.size()
			: 信息->条件状态列表.size();
	}

	template<class F>
	void 遍历因果条件状态主信息_(const 因果主信息类* 信息, F&& 访问) {
		if (!信息) return;
		if (!信息->条件状态主信息列表.empty()) {
			for (const auto& 状态 : 信息->条件状态主信息列表) {
				访问(状态);
			}
			return;
		}
		for (auto* 状态 : 信息->条件状态列表) {
			auto* 状态信息 = (状态 && 状态->主信息)
				? dynamic_cast<状态节点主信息类*>(状态->主信息)
				: nullptr;
			if (状态信息) 访问(*状态信息);
		}
	}

	std::string 汇总因果结果特征_(const 因果主信息类* 信息) {
		auto* 结果状态信息 = 取因果主结果状态主信息_(信息);
		if (!结果状态信息) return "<none>";
		return 取状态特征显示文本_(结果状态信息);
	}

	std::string 汇总因果结果详情_(const 因果主信息类* 信息) {
		auto* 结果状态信息 = 取因果主结果状态主信息_(信息);
		if (!结果状态信息) return "<none>";
		return 生成状态结果描述_(结果状态信息, 信息->结果方向可比较, 信息->结果方向值)
			+ " | 方向=" + 果方向显示文本_(信息);
	}

	std::string 汇总因果条件详情_(const 因果主信息类* 信息) {
		if (!信息 || 取因果条件状态数_(信息) == 0) return "<none>";
		std::vector<std::string> out{};
		std::size_t i = 0;
		遍历因果条件状态主信息_(信息, [&](const 状态节点主信息类& 状态信息) {
			out.push_back(
				生成状态标准描述_(&状态信息)
				+ " | 方向=" + 条件方向显示文本_(信息, i));
			++i;
		});
		return 拼接文本列表_(out, "、");
	}

	struct 结构_因果条件短语分组 {
		std::vector<std::string> 变化条件{};
		std::vector<std::string> 背景条件{};
	};

	结构_因果条件短语分组 收集因果条件短语分组_(const 因果主信息类* 信息) {
		结构_因果条件短语分组 out{};
		if (!信息) return out;
		遍历因果条件状态主信息_(信息, [&](const 状态节点主信息类& 状态信息) {
			out.背景条件.push_back(生成状态标准描述_(&状态信息));
		});
		return out;
	}

	std::string 汇总因果条件方向短语_(const 因果主信息类* 信息) {
		if (!信息 || 取因果条件状态数_(信息) == 0) return "<none>";
		const auto 分组 = 收集因果条件短语分组_(信息);
		std::vector<std::string> out = 分组.背景条件;
		out.insert(out.end(), 分组.变化条件.begin(), 分组.变化条件.end());
		return 拼接文本列表_(out, "、");
	}

	std::string 汇总因果结果方向短语_(const 因果主信息类* 信息) {
		auto* 结果状态信息 = 取因果主结果状态主信息_(信息);
		if (!结果状态信息) return "<none>";
		return 生成状态结果描述_(结果状态信息, 信息->结果方向可比较, 信息->结果方向值);
	}

	std::string 生成因果方向摘要_(const 因果主信息类* 信息) {
		if (!信息) return "<未定义>";
		const auto 动作详情 = 汇总因动作详情_(信息);
		return std::string("条件{") + 汇总因果条件方向短语_(信息)
			+ "} + 动作{" + 动作详情
			+ "} = 结果{" + 汇总因果结果方向短语_(信息) + "}";
	}

	bool 因果包含指定特征文本_(const 因果主信息类* 信息, const char* 特征文本) {
		if (!信息 || !特征文本 || !*特征文本) return false;
		const std::string 目标 = 特征文本;
		bool 命中 = false;
		遍历因果条件状态主信息_(信息, [&](const 状态节点主信息类& 状态信息) {
			if (!命中 && 取状态特征显示文本_(&状态信息) == 目标) 命中 = true;
		});
		if (命中) return true;
		if (auto* 结果状态信息 = 取因果主结果状态主信息_(信息)) {
			if (取状态特征显示文本_(结果状态信息) == 目标) return true;
		}
		const auto 动作摘要 = 汇总因动作详情_(信息);
		return 动作摘要.find(目标) != std::string::npos;
	}

	bool 是否安全服务相关因果_(const 因果主信息类* 信息) {
		return 因果包含指定特征文本_(信息, "安全值")
			|| 因果包含指定特征文本_(信息, "服务值");
	}

	bool 条件方向表示变化_(const 因果主信息类* 信息, std::size_t 序号) {
		if (!信息 || 序号 >= 取因果条件状态数_(信息)) return false;
		const bool 可比较 = 序号 < 信息->条件方向可比较列表.size()
			&& 信息->条件方向可比较列表[序号] != 0;
		if (!可比较) return false;
		const std::int64_t 方向值 = 序号 < 信息->条件方向值列表.size()
			? 信息->条件方向值列表[序号]
			: 0;
		return 方向值 != 0;
	}

	std::string 生成因果抽象组键_(const 因果主信息类* 信息) {
		auto* 结果状态 = 取因果主结果状态主信息_(信息);
		if (!结果状态) return {};
		std::string key = "result:";
		key += 取状态特征显示文本_(结果状态);
		key += "|dir:";
		key += 信息->结果方向可比较 ? std::to_string(信息->结果方向值) : std::string{ "na" };
		key += "|reason:";
		{
			const auto 原因类别 = 取状态原因类别_(结果状态);
			key += 原因类别.empty() ? std::string{ "none" } : 原因类别;
		}
		std::size_t i = 0;
		遍历因果条件状态主信息_(信息, [&](const 状态节点主信息类& 状态信息) {
			key += "|cause:";
			key += 取状态特征显示文本_(&状态信息);
			key += "#reason:";
			{
			const auto 原因类别 = 取状态原因类别_(&状态信息);
			key += 原因类别.empty() ? std::string{ "none" } : 原因类别;
			}
			if (条件方向表示变化_(信息, i)) {
				key += "@dynamic";
			}
			else {
				key += "@static:";
				key += 单状态值文本_(&状态信息);
			}
			++i;
		});
		key += "|action:";
		key += 生成因动作模式键_(信息);
		return key;
	}

	std::string 生成因果条件模式键_(const 因果主信息类* 信息) {
		if (!信息) return {};
		std::string key = "action:" + 生成因动作模式键_(信息);
		std::size_t i = 0;
		遍历因果条件状态主信息_(信息, [&](const 状态节点主信息类& 状态信息) {
			key += "|";
			key += "reason:";
			{
				const auto 原因类别 = 取状态原因类别_(&状态信息);
				key += 原因类别.empty() ? std::string{ "none" } : 原因类别;
			}
			key += "#";
			if (条件方向表示变化_(信息, i)) {
				const std::int64_t 方向值 = i < 信息->条件方向值列表.size()
					? 信息->条件方向值列表[i]
					: 0;
				key += "dynamic:";
				key += std::to_string(方向值);
			}
			else {
				const bool 可比较 = i < 信息->条件方向可比较列表.size()
					&& 信息->条件方向可比较列表[i] != 0;
				key += 可比较 ? "hold:" : "static:";
				key += 单状态值文本_(&状态信息);
			}
			++i;
		});
		return key;
	}

	struct 结构_因果状态判定结果 {
		std::unordered_map<const 基础信息节点类*, std::string> 节点标签{};
		std::size_t 稳定数 = 0;
		std::size_t 冲突数 = 0;
		std::size_t 待细化数 = 0;
	};

	结构_因果状态判定结果 构建因果状态判定结果_(const std::vector<基础信息节点类*>& 节点列表) {
		结构_因果状态判定结果 out{};
		std::unordered_map<std::string, std::unordered_map<std::string, std::size_t>> 分组模式频次{};
		for (auto* 节点 : 节点列表) {
			auto* 信息 = (节点 && 节点->主信息) ? dynamic_cast<因果主信息类*>(节点->主信息) : nullptr;
			if (!信息 || 取因果条件状态数_(信息) == 0 || !取因果主结果状态主信息_(信息)) continue;
			++分组模式频次[生成因果抽象组键_(信息)][生成因果条件模式键_(信息)];
		}

		for (auto* 节点 : 节点列表) {
			auto* 信息 = (节点 && 节点->主信息) ? dynamic_cast<因果主信息类*>(节点->主信息) : nullptr;
			std::string 标签 = "待细化";
			if (信息 && 取因果条件状态数_(信息) != 0 && 取因果主结果状态主信息_(信息)) {
				const auto 分组键 = 生成因果抽象组键_(信息);
				const auto 分组it = 分组模式频次.find(分组键);
				const bool 是否冲突 = (分组it != 分组模式频次.end())
					&& 分组it->second.size() > 1;
				if (是否冲突) {
					标签 = "冲突";
				}
				else if (信息->观察次数 >= 2) {
					标签 = "稳定";
				}
			}
			out.节点标签[节点] = 标签;
			if (标签 == "稳定") ++out.稳定数;
			else if (标签 == "冲突") ++out.冲突数;
			else ++out.待细化数;
		}
		return out;
	}

	std::string 查询因果状态标签_(
		const 基础信息节点类* 节点,
		const 结构_因果状态判定结果* 判定结果) {
		if (!节点 || !判定结果) return "待细化";
		auto it = 判定结果->节点标签.find(节点);
		return it == 判定结果->节点标签.end() ? "待细化" : it->second;
	}

	std::string 判断因果展示类型_(const 因果主信息类* 信息) {
		if (!信息) return "未分类";
		const auto 条件数 = 取因果条件状态数_(信息);
		if (条件数 == 0 || !取因果主结果状态主信息_(信息)) return "未分类";
		return 条件数 == 1 ? "单因单果" : "多因单果";
	}

	std::string 生成因果自然语言摘要_(const 因果主信息类* 信息) {
		if (!信息) return "当前因果信息不完整。";

		const auto 条件分组 = 收集因果条件短语分组_(信息);
		const auto 条件短语 = 拼接文本列表_(条件分组.背景条件, "、");
		const auto 结果短语 = 汇总因果结果方向短语_(信息);
		const auto 动作标签 = 取因动作标签_(信息);
		const auto 动作短语 = 汇总因动作详情_(信息);
		const bool 有动作短语 = !动作标签.empty() && 动作短语 != "<none>";
		if (有动作短语) {
			return "当状态{" + 条件短语 + "}成立时，发生" + 动作标签 + "{" + 动作短语 + "}，结果状态为" + 结果短语 + "。";
		}
		return "当状态{" + 条件短语 + "}成立时，结果状态为" + 结果短语 + "。";
	}

	struct 结构_因果展示统计 {
		std::size_t 同特征延续数 = 0;
		std::size_t 跨特征关联数 = 0;
		std::size_t 多特征联动数 = 0;
		std::size_t 稳定数 = 0;
		std::size_t 冲突数 = 0;
		std::size_t 待细化数 = 0;
		std::vector<std::pair<std::string, std::size_t>> 方向统计{};
	};

	结构_因果展示统计 统计因果展示信息_(
		const std::vector<基础信息节点类*>& 节点列表,
		const 结构_因果状态判定结果* 判定结果 = nullptr) {
		结构_因果展示统计 结果{};
		std::unordered_map<std::string, std::size_t> 方向频次{};

		for (auto* 节点 : 节点列表) {
			auto* 信息 = (节点 && 节点->主信息) ? dynamic_cast<因果主信息类*>(节点->主信息) : nullptr;
			if (!信息) continue;

		const auto 类型 = 判断因果展示类型_(信息);
		if (类型 == "单因单果") ++结果.同特征延续数;
			else if (类型 == "多因单果") ++结果.跨特征关联数;
			else ++结果.多特征联动数;

			const auto 状态标签 = 查询因果状态标签_(节点, 判定结果);
			if (状态标签 == "稳定") ++结果.稳定数;
			else if (状态标签 == "冲突") ++结果.冲突数;
			else ++结果.待细化数;

			++方向频次[生成因果方向摘要_(信息)];
		}

		结果.方向统计.assign(方向频次.begin(), 方向频次.end());
		std::sort(结果.方向统计.begin(), 结果.方向统计.end(), [](const auto& a, const auto& b) {
			if (a.second != b.second) return a.second > b.second;
			return a.first < b.first;
		});
		return 结果;
	}

	std::vector<基础信息节点类*> 收集因果节点列表_() {
		return 因果集.获取实例因果列表();
	}

	std::vector<基础信息节点类*> 收集抽象因果模板节点列表_() {
		return 因果集.获取抽象因果列表();
	}

	std::string 动作模板显示文本_(const 基础信息节点类* 动作模板);

	std::string 生成抽象因果表达式_(const 因果模板主信息类* 信息) {
		if (!信息) return "条件{<none>} + 动作{<none>} = 结果{<none>}";

		std::string 条件文本 = !信息->典型条件方向摘要.empty()
			? 信息->典型条件方向摘要
			: (信息->条件模板列表.empty() ? std::string("<无前置条件>") : ("条件模板=" + std::to_string(信息->条件模板列表.size())));
		std::string 动作文本 = 动作模板显示文本_(信息->动作模板);
		if (动作文本.empty() || 动作文本 == "<空>") {
			动作文本 = 信息->典型动作语义摘要;
		}
		std::string 结果文本 = !信息->典型结果方向摘要.empty()
			? 信息->典型结果方向摘要
			: (信息->结果模板列表.empty() ? std::string("<无结果模板>") : ("结果模板=" + std::to_string(信息->结果模板列表.size())));

		if (条件文本.empty()) 条件文本 = "<none>";
		if (动作文本.empty()) 动作文本 = "<none>";
		if (结果文本.empty()) 结果文本 = "<none>";

		return std::string("条件{") + 条件文本
			+ "} + 动作{" + 动作文本
			+ "} = 结果{" + 结果文本 + "}";
	}

	std::vector<基础信息节点类*> 收集因果链节点列表_() {
		return 因果集.获取因果链列表();
	}

	enum class 枚举_控制面板因果分组 : std::uint8_t {
		自我 = 0,
		自我方法 = 1,
		自我任务 = 2
	};

	std::string 因果锚点类型显示文本_(枚举_因果锚点类型 类型) {
		switch (类型) {
		case 枚举_因果锚点类型::场景: return "场景";
		case 枚举_因果锚点类型::存在: return "存在";
		default: return "未定义";
		}
	}

	std::string 主键信息显示文本_(std::uint64_t 运行期主键, const std::string& 存档主键) {
		if (!存档主键.empty()) return 裁剪摘要文本_(存档主键, 72);
		if (运行期主键 != 0) return std::to_string(运行期主键);
		return "<none>";
	}

	std::string 取特征显示文本_(const 特征节点类* 特征) {
		auto 文本 = 词显示文本_(取特征节点类型_(特征));
		return 文本.empty() ? std::string("<未知特征>") : 文本;
	}

	std::string 动作模板显示文本_(const 基础信息节点类* 动作模板) {
		if (!动作模板) return "<空>";
		std::string 文本{};
		if (动作模板->主信息) {
			if (auto* 特征信息 = dynamic_cast<const 特征节点主信息类*>(动作模板->主信息)) {
				文本 = 词显示文本_(特征信息->类型);
			}
			else if (auto* 首信息 = dynamic_cast<const 方法首节点主信息类*>(动作模板->主信息)) {
				文本 = 词显示文本_(首信息->动作名);
			}
		}
		const auto 主键 = 动作模板->获取主键();
		const auto 主键摘要 = 主键.empty() ? std::string("<无主键>") : 裁剪摘要文本_(主键, 96);
		if (文本.empty()) return 主键摘要;
		return 文本;
	}

	枚举_控制面板因果分组 判定控制面板因果分组_(const 因果主信息类* 信息) {
		if (!信息) return 枚举_控制面板因果分组::自我;
		if (信息->来源任务主键 != 0 || !信息->来源任务主键_存档.empty()) {
			return 枚举_控制面板因果分组::自我任务;
		}
		if (信息->来源方法主键 != 0 || !信息->来源方法主键_存档.empty()) {
			return 枚举_控制面板因果分组::自我方法;
		}
		return 枚举_控制面板因果分组::自我;
	}

	void 追加因果链详细信息_(
		std::ostringstream& 输出,
		const std::vector<基础信息节点类*>& 实例节点列表,
		const std::vector<基础信息节点类*>& 抽象节点列表,
		const std::vector<基础信息节点类*>& 链节点列表,
		std::size_t 展示上限 = 24) {
		输出 << "实例因果节点数: " << 实例节点列表.size() << "\r\n";
		输出 << "抽象因果节点数: " << 抽象节点列表.size() << "\r\n";
		输出 << "因果链节点数: " << 链节点列表.size() << "\r\n";
		if (实例节点列表.empty() && 抽象节点列表.empty() && 链节点列表.empty()) {
			输出 << "\r\n当前没有因果信息。";
			return;
		}

		if (!实例节点列表.empty()) {
			const auto 判定结果 = 构建因果状态判定结果_(实例节点列表);
			const auto 统计 = 统计因果展示信息_(实例节点列表, &判定结果);
			输出 << "\r\n[实例因果]\r\n";
			输出 << "类型分布: 单因单果=" << 统计.同特征延续数
				<< " | 多因单果=" << 统计.跨特征关联数
				<< " | 未分类=" << 统计.多特征联动数 << "\r\n";
			输出 << "状态分布: 稳定=" << 统计.稳定数
				<< " | 冲突=" << 统计.冲突数
				<< " | 待细化=" << 统计.待细化数 << "\r\n";
			输出 << "排序方式: 优先按稳定度Q，其次按最近发生时间\r\n";

			if (!统计.方向统计.empty()) {
				输出 << "主要方向:\r\n";
				const auto 方向展示数 = (std::min)(static_cast<std::size_t>(6), 统计.方向统计.size());
				for (std::size_t i = 0; i < 方向展示数; ++i) {
					输出 << "[" << (i + 1) << "] "
						<< 统计.方向统计[i].first
						<< " | 数量=" << 统计.方向统计[i].second << "\r\n";
				}
			}

			const auto 实际展示数 = (std::min)(展示上限, 实例节点列表.size());
			for (std::size_t i = 0; i < 实际展示数; ++i) {
				auto* 节点 = 实例节点列表[i];
				auto* 信息 = (节点 && 节点->主信息) ? dynamic_cast<因果主信息类*>(节点->主信息) : nullptr;
				if (!信息) continue;

				输出 << "\r\n因果[" << (i + 1) << "] " << 生成因果方向摘要_(信息) << "\r\n";
				输出 << "类型: " << 判断因果展示类型_(信息)
					<< " | 状态=" << 查询因果状态标签_(节点, &判定结果)
					<< " | 观察次数=" << 信息->观察次数
					<< " | 稳定度Q=" << 信息->稳定度Q << "\r\n";
				输出 << "锚点: " << 因果锚点类型显示文本_(信息->锚点类型)
					<< " | 来源方法=" << 主键信息显示文本_(信息->来源方法主键, 信息->来源方法主键_存档)
					<< " | 来源任务=" << 主键信息显示文本_(信息->来源任务主键, 信息->来源任务主键_存档) << "\r\n";
				输出 << "窗口: 因状态=" << 取因果条件状态数_(信息)
					<< " | 因动作=" << (取因动作动态_(信息) ? 1 : 0)
					<< " | 过程动态=" << 提取过程动态_不含因动作_(信息).size()
					<< " | 果状态=" << (取因果主结果状态主信息_(信息) ? 1 : 0) << "\r\n";
				输出 << "因: 状态集={" << 汇总因果条件详情_(信息) << "} | 动作={" << 汇总因动作详情_(信息) << "}\r\n";
				输出 << "过程: " << 汇总因果动态特征_(提取过程动态_不含因动作_(信息)) << "\r\n";
				输出 << "果: " << 汇总因果结果详情_(信息) << "\r\n";
				输出 << "说明: " << 生成因果自然语言摘要_(信息) << "\r\n";
			}
			if (实例节点列表.size() > 实际展示数) {
				输出 << "\r\n其余 " << (实例节点列表.size() - 实际展示数) << " 条实例因果已省略。\r\n";
			}
		}

		if (!抽象节点列表.empty()) {
			输出 << "\r\n[抽象因果]\r\n";
			const auto 实际展示数 = (std::min)(展示上限, 抽象节点列表.size());
			for (std::size_t i = 0; i < 实际展示数; ++i) {
				auto* 节点 = 抽象节点列表[i];
				auto* 信息 = (节点 && 节点->主信息) ? dynamic_cast<因果模板主信息类*>(节点->主信息) : nullptr;
				if (!信息) continue;
				输出 << "抽象[" << (i + 1) << "] "
					<< 生成抽象因果表达式_(信息)
					<< " | 证据=" << 信息->证据次数
					<< " | 稳定度=" << 信息->稳定度 << "\r\n";
			}
			if (抽象节点列表.size() > 实际展示数) {
				输出 << "其余 " << (抽象节点列表.size() - 实际展示数) << " 条抽象因果已省略。\r\n";
			}
		}

		if (!链节点列表.empty()) {
			输出 << "\r\n[因果链]\r\n";
			const auto 实际展示数 = (std::min)(展示上限, 链节点列表.size());
			for (std::size_t i = 0; i < 实际展示数; ++i) {
				auto* 节点 = 链节点列表[i];
				auto* 信息 = (节点 && 节点->主信息) ? dynamic_cast<因果链主信息类*>(节点->主信息) : nullptr;
				if (!信息) continue;
				输出 << "链[" << (i + 1) << "] 长度=" << 信息->因果实例序列.size()
					<< " | 时间=" << 信息->起始时间 << "->" << 信息->结束时间
					<< " | 起点锚点=" << 主键信息显示文本_(信息->起点锚点主键, 信息->起点锚点主键_存档)
					<< " | 终点锚点=" << 主键信息显示文本_(信息->终点锚点主键, 信息->终点锚点主键_存档) << "\r\n";
				const auto 链内展示数 = (std::min)(static_cast<std::size_t>(6), 信息->因果实例序列.size());
				for (std::size_t j = 0; j < 链内展示数; ++j) {
					auto* 实例 = 信息->因果实例序列[j];
					auto* 实例信息 = (实例 && 实例->主信息) ? dynamic_cast<因果主信息类*>(实例->主信息) : nullptr;
					if (!实例信息) continue;
					输出 << "  -> " << 生成因果方向摘要_(实例信息) << "\r\n";
				}
				if (信息->因果实例序列.size() > 链内展示数) {
					输出 << "  其余 " << (信息->因果实例序列.size() - 链内展示数) << " 跳已省略\r\n";
				}
			}
			if (链节点列表.size() > 实际展示数) {
				输出 << "其余 " << (链节点列表.size() - 实际展示数) << " 条因果链已省略。\r\n";
			}
		}
	}

	std::string 节点主键摘要_(const 基础信息节点类* 节点) {
		if (!节点) return "<空>";
		const auto 主键 = 节点->获取主键();
		if (!主键.empty()) return 裁剪摘要文本_(主键, 96);
		return "<无主键>";
	}

	std::string 节点主键摘要_(const 方法节点类* 节点) {
		if (!节点) return "<空>";
		const auto 主键 = 节点->获取主键();
		if (!主键.empty()) return 裁剪摘要文本_(主键, 96);
		return "<无主键>";
	}

	std::string 动态来源类型显示文本_(枚举_动态来源类型 来源类型) {
		switch (来源类型) {
		case 枚举_动态来源类型::方法调用:
			return "方法调用";
		default:
			return "未定义";
		}
	}

	std::string 动态来源动作名_(const 动态节点主信息类* 动态信息) {
		if (!动态信息) return {};
		std::string 动作名{};
		if (动态信息->来源动作名) {
			动作名 = 词显示文本_(动态信息->来源动作名);
		}
		if (动作名.empty()) {
			auto* 方法首信息 = (动态信息->来源方法首节点 && 动态信息->来源方法首节点->主信息)
				? dynamic_cast<const 方法首节点主信息类*>(动态信息->来源方法首节点->主信息)
				: nullptr;
			if (方法首信息 && 方法首信息->动作名) {
				动作名 = 词显示文本_(方法首信息->动作名);
			}
		}
		if (动作名.empty()) return {};
		const auto 相位 = 动作事件相位显示文本_(动态信息->来源动作相位);
		return 相位.empty() ? 动作名 : (动作名 + 相位);
	}

	std::string 状态域显示文本_(const 状态节点主信息类* 状态信息) {
		if (!状态信息) return "<未知状态域>";
		if (状态信息->是否内部状态()) return "内部状态";
		if (状态信息->是否世界状态()) return "世界状态";
		return "未定义状态域";
	}

	std::string 生成状态树项文本_(const 状态节点类* 状态, std::size_t 序号) {
		auto* 状态信息 = (状态 && 状态->主信息)
			? dynamic_cast<状态节点主信息类*>(状态->主信息)
			: nullptr;
		if (!状态信息) {
			return "[" + std::to_string(序号) + "] <状态信息缺失>";
		}

		std::ostringstream 输出;
		输出 << "[" << 序号 << "] "
			<< 取状态特征显示文本_(状态)
			<< "=" << 快照文本_(状态信息->状态值)
			<< " | " << 状态域显示文本_(状态信息);
		const 时间戳 t = 状态信息->发生时间 ? 状态信息->发生时间 : 状态信息->收到时间;
		if (t != 0) {
			输出 << " | t=" << t;
		}
		if (状态信息->是否变化) {
			输出 << " | 变化";
		}
		if (!状态信息->变化原因说明.empty()) {
			输出 << " | 原因=" << 状态信息->变化原因说明;
		}
		else if (!状态信息->变化原因类别.empty()) {
			输出 << " | 原因=" << 状态信息->变化原因类别;
		}
		return 输出.str();
	}

	std::string 生成状态树项文本_(const 状态节点主信息类* 状态信息, std::size_t 序号) {
		if (!状态信息) {
			return "[" + std::to_string(序号) + "] <状态信息缺失>";
		}

		std::ostringstream 输出;
		输出 << "[" << 序号 << "] "
			<< 取状态特征显示文本_(状态信息)
			<< "=" << 快照文本_(状态信息->状态值)
			<< " | " << 状态域显示文本_(状态信息);
		const 时间戳 t = 状态信息->发生时间 ? 状态信息->发生时间 : 状态信息->收到时间;
		if (t != 0) {
			输出 << " | t=" << t;
		}
		if (状态信息->是否变化) {
			输出 << " | 变化";
		}
		if (!状态信息->变化原因说明.empty()) {
			输出 << " | 原因=" << 状态信息->变化原因说明;
		}
		else if (!状态信息->变化原因类别.empty()) {
			输出 << " | 原因=" << 状态信息->变化原因类别;
		}
		return 输出.str();
	}

	std::string 生成动态树项文本_(const 动态节点类* 动态, std::size_t 序号) {
		auto* 动态信息 = (动态 && 动态->主信息)
			? dynamic_cast<动态节点主信息类*>(动态->主信息)
			: nullptr;
		if (!动态信息) {
			return "[" + std::to_string(序号) + "] <动态信息缺失>";
		}

		const auto 来源动作名 = 动态来源动作名_(动态信息);
		const bool 是方法调用动态 = 动态信息->来源类型 == 枚举_动态来源类型::方法调用;
		std::ostringstream 输出;
		输出 << "[" << 序号 << "] "
			<< (来源动作名.empty() ? 取动态特征显示文本_(动态) : 来源动作名)
			<< " | 开始=" << 动态信息->开始时间
			<< " | 结束=" << 动态信息->结束时间;
		if (是方法调用动态) {
			输出 << " | 来源=" << 动态来源类型显示文本_(动态信息->来源类型)
				<< " | 输入=" << 节点主键摘要_(动态信息->来源输入场景)
				<< " | 输出=" << 节点主键摘要_(动态信息->来源输出场景)
				<< " | 结果=" << (动态信息->来源执行成功 ? "成功" : "失败");
			if (!动态信息->来源执行成功) {
				输出 << "(" << 动态信息->来源错误码 << ")";
			}
		}
		if (!动态信息->事件列表.empty()) {
			输出 << " | 事件=" << 动态信息->事件列表.size();
		}
		if (!动态信息->状态路径列表.empty()) {
			输出 << " | 状态路径=" << 动态信息->状态路径列表.size();
		}
		return 输出.str();
	}

	std::string 生成因果树标题_(
		const 因果主信息类* 信息,
		std::size_t 序号,
		const std::string& 状态标签 = "待细化") {
		if (!信息) {
			return "因果[" + std::to_string(序号) + "] <信息缺失>";
		}

		std::ostringstream 输出;
		输出 << "因果[" << 序号 << "] "
			<< 生成因果方向摘要_(信息)
			<< " | " << 状态标签
			<< " | " << 判断因果展示类型_(信息);
		输出 << " | 累计观察=" << 信息->观察次数;
		输出 << " | Q=" << 信息->稳定度Q;
		return 输出.str();
	}

	std::string 规范化编辑框换行_(const std::string& 文本) {
		std::string out;
		out.reserve(文本.size() + 16);
		for (std::size_t i = 0; i < 文本.size(); ++i) {
			const char c = 文本[i];
			if (c == '\n') {
				if (i == 0 || 文本[i - 1] != '\r') {
					out += "\r\n";
				}
				else {
					out.push_back('\n');
				}
				continue;
			}
			out.push_back(c);
		}
		return out;
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

	template<class TNode, class Fn>
	void 枚举同层节点_(TNode* 首节点, Fn&& fn) {
		if (!首节点) return;
		auto* 当前 = 首节点;
		do {
			fn(当前);
			当前 = static_cast<TNode*>(当前->下);
		} while (当前 && 当前 != 首节点);
	}

	template<class TNode>
	std::size_t 计数同层节点_(TNode* 首节点) {
		std::size_t 数量 = 0;
		枚举同层节点_(首节点, [&](TNode*) { ++数量; });
		return 数量;
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

	const char* 方法节点种类显示文本_(枚举_方法节点种类 s) {
		switch (s) {
		case 枚举_方法节点种类::方法首节点: return "方法首节点";
		case 枚举_方法节点种类::方法条件节点: return "方法条件节点";
		case 枚举_方法节点种类::方法结果节点: return "方法结果节点";
		default: return "未定义";
		}
	}

	std::string 动作句柄显示文本_(const 结构体_动作句柄& v) {
		switch (v.类型) {
		case 枚举_动作句柄类型::本能函数ID:
			return v.本能ID != 0 ? ("本能ID:" + std::to_string(v.本能ID)) : std::string("本能ID:<空>");
		case 枚举_动作句柄类型::外部实现主键:
			return v.外部实现主键.empty() ? std::string("外部实现:<空>") : ("外部实现:" + 裁剪摘要文本_(v.外部实现主键, 72));
		case 枚举_动作句柄类型::动作序列主键:
			return v.动作序列主键.empty() ? std::string("动作序列:<空>") : ("动作序列:" + 裁剪摘要文本_(v.动作序列主键, 72));
		default:
			return "未设置";
		}
	}

	bool 方法模板场景有内容_(const 方法信息基类* 信息) {
		return 信息 && 信息->场景根节点.子 != nullptr;
	}

	void 统计方法子树节点分布_(方法节点类* 首节点, std::size_t& 条件数, std::size_t& 结果数) {
		枚举同层节点_(首节点, [&](方法节点类* 节点) {
			auto* 信息 = (节点 && 节点->主信息) ? dynamic_cast<方法信息基类*>(节点->主信息) : nullptr;
			if (!信息) return;
			if (信息->节点种类 == 枚举_方法节点种类::方法条件节点) ++条件数;
			if (信息->节点种类 == 枚举_方法节点种类::方法结果节点) ++结果数;
			if (节点->子) {
				统计方法子树节点分布_(static_cast<方法节点类*>(节点->子), 条件数, 结果数);
			}
		});
	}

	std::string 生成方法节点树标题_(const 方法节点类* 节点) {
		auto* 信息 = (节点 && 节点->主信息) ? dynamic_cast<方法信息基类*>(节点->主信息) : nullptr;
		if (!信息) return "<方法信息缺失>";

		std::ostringstream 输出;
		if (auto* 首信息 = dynamic_cast<方法首节点主信息类*>(信息)) {
			std::size_t 条件数 = 0;
			std::size_t 结果数 = 0;
			if (节点->子) {
				统计方法子树节点分布_(static_cast<方法节点类*>(节点->子), 条件数, 结果数);
			}
			输出 << (词显示文本_(首信息->动作名).empty() ? "<未命名方法>" : 词显示文本_(首信息->动作名))
				<< " | 来源=" << 方法来源文本_(首信息->来源)
				<< " | 条件=" << 条件数
				<< " | 结果=" << 结果数
				<< " | 影响签名=" << 首信息->影响度量签名.size();
			return 输出.str();
		}

		if (auto* 条件信息 = dynamic_cast<方法条件节点主信息类*>(信息)) {
			输出 << "条件节点"
				<< " | 判定=" << 条件信息->条件判定索引.size();
			if (条件信息->时间起 != 0 || 条件信息->时间止 != 0) {
				输出 << " | 时间=" << 条件信息->时间起 << "->" << 条件信息->时间止;
			}
			if (!条件信息->条件场景模板主键.empty()) {
				输出 << " | 条件键=" << 裁剪摘要文本_(条件信息->条件场景模板主键, 48);
			}
			return 输出.str();
		}

		if (auto* 结果信息 = dynamic_cast<方法结果节点主信息类*>(信息)) {
			输出 << "结果节点"
				<< " | 摘要=" << 结果信息->结果摘要索引.size();
			return 输出.str();
		}

		输出 << 方法节点种类显示文本_(信息->节点种类);
		return 输出.str();
	}

	const char* 需求运行状态文本_(枚举_需求运行状态 s) {
		switch (s) {
		case 枚举_需求运行状态::待执行: return "待执行";
		case 枚举_需求运行状态::已满足: return "已满足";
		case 枚举_需求运行状态::失败: return "失败";
		case 枚举_需求运行状态::已取消: return "已取消";
		default: return "未知";
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

struct C海鱼Dlg::摄像机运行时 {
	相机观测运行时类 实现{};

	bool 启动() { return 实现.启动(); }
	void 停止() { 实现.停止(); }
	bool 是否正在运行() const { return 实现.是否正在运行(); }
	结构_相机观测运行时快照 快照() const { return 实现.快照(); }
	std::vector<结构_相机桥接存在快照> 取最近桥接存在列表() const { return 实现.取最近桥接存在列表(); }
	void 锁定观测目标(const std::string& 存在主键) { 实现.锁定观测目标(存在主键); }
	void 解除锁定观测() { 实现.解除锁定观测(); }
};

C海鱼Dlg::~C海鱼Dlg()
{
	停止后台连接线程();
	关闭后台进程跟踪句柄();
	if (摄像机_) {
		摄像机_->停止();
		摄像机_.reset();
	}
	if (本地宿主_) {
		本地宿主_->停止();
		本地宿主_.reset();
	}

	// 如果该对话框有自动化代理，则
	//  此对话框的返回指针为 null，所以它知道
	//  此代理知道该对话框已被删除。
	if (m_pAutoProxy != nullptr)
		m_pAutoProxy->m_pDialog = nullptr;
}

void C海鱼Dlg::清理退出子窗口()
{
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
	DDX_Control(pDX, IDC_BUTTON_REFRESH_MAIN, 变量_刷新按钮);
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
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_MAIN, &C海鱼Dlg::OnBnClicked刷新当前页)
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
	初始化世界骨架环境("C海鱼Dlg::OnInitDialog");
	使用远程后台模式_ = false;
	后台在线_ = false;
	最近后台快照_ = {};
	最近后台快照_.后台在线 = true;
	最近后台快照_.最近命令摘要 = "正在启动本地内核...";
	最近后台自我场景快照_ = {};
	最近后台自我场景快照_.后台在线 = true;
	最近后台自我场景快照有效_ = false;
	最近后台自我场景错误_.clear();
	日志::运行("[前端界面] 控制面板已切换为本地一体化模式：直接在本进程内启动宿主、自我线程与外设");

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
	隐藏子页面关闭控件_(变量_交互界面);
	隐藏子页面关闭控件_(变量_配置及测试窗口);
	隐藏子页面关闭控件_(变量_基础信息界面);
	变量_信息查看框.CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""),
		WS_CHILD | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
		rect, this, IDC_标签内容查看框);
	变量_信息查看框.SetFont(GetFont());
	变量_因果树控件.CreateEx(
		WS_EX_CLIENTEDGE,
		WS_CHILD | WS_TABSTOP | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS,
		rect,
		this,
		IDC_因果树查看框);
	变量_因果树控件.SetFont(GetFont());
	变量_刷新按钮.SetFont(GetFont());
	更新主界面布局_();
	//////////////////////////////////////////////////
	变量_TAB1.SetCurSel(0);
	const bool 初始使用信息框 = 标签页使用信息查看框(变量_TAB1.GetCurSel());
	const bool 初始使用因果树 = 标签页使用因果树控件_(变量_TAB1.GetCurSel());
	变量_基础信息界面.ShowWindow(SW_HIDE);
	变量_交互界面.ShowWindow(初始使用信息框 ? SW_HIDE : SW_SHOW);
	变量_配置及测试窗口.ShowWindow(SW_HIDE);
	变量_信息查看框.ShowWindow(初始使用信息框 ? SW_SHOW : SW_HIDE);
	变量_因果树控件.ShowWindow(初始使用因果树 ? SW_SHOW : SW_HIDE);
	刷新当前界面_(false);
	if (使用远程后台模式_) {
		启动异步后台连接();
	}
	else {
		CString 错误文本;
		if (!启动本地内核_(&错误文本) && !错误文本.IsEmpty()) {
			AfxMessageBox(_T("启动本地内核失败：\n") + 错误文本);
		}
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
	if (!::IsWindow(变量_TAB1.GetSafeHwnd())) return;
	CRect rect;
	变量_TAB1.GetClientRect(&rect);
	rect.top += 24;
	rect.left += 4;
	rect.right -= 4;
	rect.bottom -= 4;
	if (::IsWindow(变量_信息查看框.GetSafeHwnd())) {
		变量_信息查看框.MoveWindow(&rect, TRUE);
	}
	if (::IsWindow(变量_因果树控件.GetSafeHwnd())) {
		变量_因果树控件.MoveWindow(&rect, TRUE);
	}
}

void C海鱼Dlg::更新主界面布局_()
{
	if (!::IsWindow(GetSafeHwnd()) || !::IsWindow(变量_TAB1.GetSafeHwnd())) return;

	CRect 客户区;
	GetClientRect(&客户区);

	constexpr int 边距 = 8;
	constexpr int 按钮宽度 = 88;
	constexpr int 按钮高度 = 24;
	constexpr int 按钮与标签间距 = 8;

	CRect 刷新按钮区域(
		客户区.right - 边距 - 按钮宽度,
		客户区.bottom - 边距 - 按钮高度,
		客户区.right - 边距,
		客户区.bottom - 边距);
	if (::IsWindow(变量_刷新按钮.GetSafeHwnd())) {
		变量_刷新按钮.MoveWindow(&刷新按钮区域, TRUE);
	}

	CRect 标签区域(
		客户区.left + 边距,
		客户区.top + 边距,
		客户区.right - 边距,
		刷新按钮区域.top - 按钮与标签间距);
	if (标签区域.bottom <= 标签区域.top) {
		标签区域.bottom = 客户区.bottom - 边距;
	}
	变量_TAB1.MoveWindow(&标签区域, TRUE);

	CRect 子页区域 = 标签区域;
	子页区域.bottom -= 1;
	子页区域.top += 22;
	if (::IsWindow(变量_交互界面.GetSafeHwnd())) {
		变量_交互界面.MoveWindow(&子页区域, TRUE);
	}
	if (::IsWindow(变量_配置及测试窗口.GetSafeHwnd())) {
		变量_配置及测试窗口.MoveWindow(&子页区域, TRUE);
	}
	if (::IsWindow(变量_基础信息界面.GetSafeHwnd())) {
		变量_基础信息界面.MoveWindow(&子页区域, TRUE);
	}
	if (::IsWindow(变量_信息查看框.GetSafeHwnd())) {
		更新信息查看框布局();
	}
	刷新场景显示布局();
}

void C海鱼Dlg::刷新当前界面_(bool 主动拉取远程快照)
{
	if (使用远程后台模式_ && 主动拉取远程快照) {
		(void)尝试刷新后台快照();
	}

	刷新当前标签内容();

	if (!::IsWindow(变量_TAB1.GetSafeHwnd())) return;
	if (变量_TAB1.GetCurSel() == 8 && ::IsWindow(变量_基础信息界面.GetSafeHwnd())) {
		变量_基础信息界面.刷新世界树();
	}
}

bool C海鱼Dlg::标签页使用因果树控件_(int 标签索引) const
{
	return !使用远程后台模式_ && (标签索引 == 2 || 标签索引 == 4);
}

bool C海鱼Dlg::标签页使用信息查看框(int 标签索引) const
{
	if (标签页使用因果树控件_(标签索引)) {
		return false;
	}
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

void C海鱼Dlg::刷新因果树控件_()
{
	if (::IsWindow(变量_TAB1.GetSafeHwnd()) && 变量_TAB1.GetCurSel() == 2) {
		刷新方法树控件_();
		return;
	}

	if (!::IsWindow(变量_因果树控件.GetSafeHwnd())) return;

	变量_因果树控件.SetRedraw(FALSE);
	变量_因果树控件.DeleteAllItems();

	const auto 插入树项 = [&](HTREEITEM 父项, const std::string& 文本) {
		return 变量_因果树控件.InsertItem(UTF8到CString_(文本), 父项, TVI_LAST);
	};

	if (使用远程后台模式_) {
		HTREEITEM 根项 = 插入树项(TVI_ROOT, "因果信息（远程摘要）");
		插入树项(根项, "当前远程模式只下发摘要文本，尚未提供完整树形因果对象。");
		插入树项(根项, "如果要查看全部因果，请在本地一体化模式下打开控制面板。");
		变量_因果树控件.Expand(根项, TVE_EXPAND);
		变量_因果树控件.SetRedraw(TRUE);
		变量_因果树控件.Invalidate();
		return;
	}

	if (本地内核是否停止中()) {
		HTREEITEM 根项 = 插入树项(TVI_ROOT, "因果树");
		插入树项(根项, "本地内核正在停止中，因果树已暂停读取。");
		if (!本地最近命令摘要_.empty()) {
			插入树项(根项, "最近状态: " + 本地最近命令摘要_);
		}
		变量_因果树控件.Expand(根项, TVE_EXPAND);
		变量_因果树控件.SetRedraw(TRUE);
		变量_因果树控件.Invalidate();
		return;
	}

	HTREEITEM 根项 = 插入树项(TVI_ROOT, "因果树");
	插入树项(根项, "说明: 这里只显示独立因果树；实例因果仍按锚点挂在世界树里。");

	auto* 因果根 = 因果集.获取因果根();
	if (!因果根) {
		插入树项(根项, "因果根未初始化");
		变量_因果树控件.Expand(根项, TVE_EXPAND);
		变量_因果树控件.SetRedraw(TRUE);
		变量_因果树控件.Invalidate();
		return;
	}

	const auto 计数同层基础节点_ = [](基础信息节点类* 首节点) -> std::size_t {
		if (!首节点) return 0;
		std::size_t 数量 = 0;
		auto* 当前 = 首节点;
		do {
			++数量;
			当前 = 当前->下 ? static_cast<基础信息节点类*>(当前->下) : nullptr;
		} while (当前 && 当前 != 首节点);
		return 数量;
	};

	const auto 生成因果树节点标题_ = [&](基础信息节点类* 节点) -> std::string {
		if (!节点) return "<空节点>";
		if (!节点->主信息) return 节点主键摘要_(节点);

		if (auto* 模板信息 = dynamic_cast<因果模板主信息类*>(节点->主信息)) {
			std::ostringstream os;
			os << "抽象因果 | " << 生成抽象因果表达式_(模板信息)
				<< " | 证据=" << 模板信息->证据次数
				<< " | 稳定度=" << 模板信息->稳定度;
			return os.str();
		}

		if (auto* 链信息 = dynamic_cast<因果链主信息类*>(节点->主信息)) {
			std::ostringstream os;
			os << "因果链 | 长度=" << 链信息->因果实例序列.size()
				<< " | 时间=" << 链信息->起始时间 << "->" << 链信息->结束时间;
			return os.str();
		}

		if (auto* 实例信息 = dynamic_cast<因果主信息类*>(节点->主信息)) {
			return 生成因果树标题_(实例信息, 0, {});
		}

		return 节点主键摘要_(节点);
	};

	std::function<void(HTREEITEM, 基础信息节点类*)> 追加因果树节点_;
	追加因果树节点_ = [&](HTREEITEM 父项, 基础信息节点类* 节点) {
		if (!父项 || !节点) return;
		HTREEITEM 节点项 = 插入树项(父项, 生成因果树节点标题_(节点));

		if (auto* 模板信息 = 节点->主信息 ? dynamic_cast<因果模板主信息类*>(节点->主信息) : nullptr) {
			HTREEITEM 详情项 = 插入树项(节点项, "详情");
			插入树项(详情项, "表达式: " + 生成抽象因果表达式_(模板信息));
			插入树项(详情项, "证据次数: " + std::to_string(模板信息->证据次数));
			插入树项(详情项, "成立次数: " + std::to_string(模板信息->成立次数));
			插入树项(详情项, "失败次数: " + std::to_string(模板信息->失败次数));
			插入树项(详情项, "稳定度: " + std::to_string(模板信息->稳定度));
			if (!模板信息->典型条件方向摘要.empty()) {
				插入树项(详情项, "条件摘要: " + 模板信息->典型条件方向摘要);
			}
			if (!模板信息->典型动作语义摘要.empty()) {
				插入树项(详情项, "动作摘要: " + 模板信息->典型动作语义摘要);
			}
			if (!模板信息->典型结果方向摘要.empty()) {
				插入树项(详情项, "结果摘要: " + 模板信息->典型结果方向摘要);
			}
			变量_因果树控件.Expand(详情项, TVE_EXPAND);
		}
		else if (auto* 实例信息 = 节点->主信息 ? dynamic_cast<因果主信息类*>(节点->主信息) : nullptr) {
			HTREEITEM 详情项 = 插入树项(节点项, "详情");
			插入树项(详情项, "表达式: " + 生成因果方向摘要_(实例信息));
			插入树项(详情项, "说明: " + 生成因果自然语言摘要_(实例信息));
			插入树项(详情项, "类型: " + 判断因果展示类型_(实例信息));
			插入树项(详情项, "累计观察: " + std::to_string(实例信息->观察次数));
			插入树项(详情项, "稳定度Q: " + std::to_string(实例信息->稳定度Q));
			插入树项(详情项, "来源方法: " + 主键信息显示文本_(实例信息->来源方法主键, 实例信息->来源方法主键_存档));
			插入树项(详情项, "来源任务: " + 主键信息显示文本_(实例信息->来源任务主键, 实例信息->来源任务主键_存档));

			HTREEITEM 条件项 = 插入树项(节点项, "条件状态 (" + std::to_string(取因果条件状态数_(实例信息)) + ")");
			std::size_t 条件序号 = 1;
			遍历因果条件状态主信息_(实例信息, [&](const 状态节点主信息类& 状态信息) {
				插入树项(条件项, 生成状态树项文本_(&状态信息, 条件序号++));
			});
			if (条件序号 == 1) {
				插入树项(条件项, "<无条件状态>");
			}

			HTREEITEM 动作项 = 插入树项(节点项, "动作");
			插入树项(动作项, "摘要: " + 汇总因动作详情_(实例信息));
			插入树项(动作项, "标签: " + 取因动作标签_(实例信息));
			if (实例信息->动作主体) {
				插入树项(动作项, "主体: " + 节点主键摘要_(实例信息->动作主体));
			}
			if (实例信息->动作语义) {
				插入树项(动作项, "语义: " + 节点主键摘要_(实例信息->动作语义));
			}
			if (实例信息->动作动态) {
				auto* 动态信息 = 实例信息->动作动态->主信息
					? dynamic_cast<动态节点主信息类*>(实例信息->动作动态->主信息)
					: nullptr;
				if (动态信息) {
					插入树项(动作项, "动作动态: " + 动态来源动作名_(动态信息));
				}
			}

			HTREEITEM 结果项 = 插入树项(节点项, "结果状态");
			if (auto* 结果状态信息 = 取因果主结果状态主信息_(实例信息)) {
				插入树项(结果项, 生成状态树项文本_(结果状态信息, 1));
			}
			else {
				插入树项(结果项, "<无结果状态>");
			}

			变量_因果树控件.Expand(详情项, TVE_EXPAND);
			变量_因果树控件.Expand(条件项, TVE_EXPAND);
			变量_因果树控件.Expand(动作项, TVE_EXPAND);
			变量_因果树控件.Expand(结果项, TVE_EXPAND);
		}

		auto* 首子 = 节点->子 ? static_cast<基础信息节点类*>(节点->子) : nullptr;
		if (!首子) return;
		auto* 当前 = 首子;
		do {
			追加因果树节点_(节点项, 当前);
			当前 = 当前->下 ? static_cast<基础信息节点类*>(当前->下) : nullptr;
		} while (当前 && 当前 != 首子);
	};

	std::ostringstream 根标题;
	根标题 << "因果根";
	const auto 根主键 = 节点主键摘要_(因果根);
	if (!根主键.empty() && 根主键 != "<空>" && 根主键 != "<无主键>") {
		根标题 << " {" << 根主键 << "}";
	}
	根标题 << " | 直接子节点=" << 计数同层基础节点_(因果根->子 ? static_cast<基础信息节点类*>(因果根->子) : nullptr);
	HTREEITEM 因果根项 = 插入树项(根项, 根标题.str());

	auto* 首子 = 因果根->子 ? static_cast<基础信息节点类*>(因果根->子) : nullptr;
	if (!首子) {
		插入树项(因果根项, "(因果根当前没有子节点)");
	}
	else {
		auto* 当前 = 首子;
		do {
			追加因果树节点_(因果根项, 当前);
			当前 = 当前->下 ? static_cast<基础信息节点类*>(当前->下) : nullptr;
		} while (当前 && 当前 != 首子);
	}

	变量_因果树控件.Expand(根项, TVE_EXPAND);
	变量_因果树控件.Expand(因果根项, TVE_EXPAND);
	变量_因果树控件.SetRedraw(TRUE);
	变量_因果树控件.Invalidate();
}

void C海鱼Dlg::刷新方法树控件_()
{
	if (!::IsWindow(变量_因果树控件.GetSafeHwnd())) return;

	变量_因果树控件.SetRedraw(FALSE);
	变量_因果树控件.DeleteAllItems();

	const auto 插入树项 = [&](HTREEITEM 父项, const std::string& 文本) {
		return 变量_因果树控件.InsertItem(UTF8到CString_(文本), 父项, TVI_LAST);
	};

	if (使用远程后台模式_) {
		HTREEITEM 根项 = 插入树项(TVI_ROOT, "方法树（远程摘要）");
		插入树项(根项, "当前远程模式只下发方法摘要，尚未提供完整树形方法对象。");
		变量_因果树控件.Expand(根项, TVE_EXPAND);
		变量_因果树控件.SetRedraw(TRUE);
		变量_因果树控件.Invalidate();
		return;
	}

	if (本地内核是否停止中()) {
		HTREEITEM 根项 = 插入树项(TVI_ROOT, "方法树");
		插入树项(根项, "本地内核正在停止中，方法树已暂停读取。");
		if (!本地最近命令摘要_.empty()) {
			插入树项(根项, "最近状态: " + 本地最近命令摘要_);
		}
		变量_因果树控件.Expand(根项, TVE_EXPAND);
		变量_因果树控件.SetRedraw(TRUE);
		变量_因果树控件.Invalidate();
		return;
	}

	const auto 顶层方法数 = [&]() -> std::size_t {
		std::size_t 数量 = 0;
		枚举根子节点_(static_cast<方法节点类*>(数据仓库模块::方法链.根指针), [&](方法节点类* 节点) {
			if (节点 && 节点->主信息) ++数量;
		});
		return 数量;
	}();

	HTREEITEM 根项 = 插入树项(TVI_ROOT, "方法树（" + std::to_string(顶层方法数) + " 个方法首节点）");
	if (顶层方法数 == 0) {
		插入树项(根项, "当前没有方法。");
		变量_因果树控件.Expand(根项, TVE_EXPAND);
		变量_因果树控件.SetRedraw(TRUE);
		变量_因果树控件.Invalidate();
		return;
	}

	std::function<void(HTREEITEM, 方法节点类*)> 追加方法节点树_;
	追加方法节点树_ = [&](HTREEITEM 父项, 方法节点类* 节点) {
		if (!父项 || !节点) return;
		auto* 信息 = 节点->主信息 ? dynamic_cast<方法信息基类*>(节点->主信息) : nullptr;
		HTREEITEM 节点项 = 插入树项(父项, 生成方法节点树标题_(节点));
		if (!信息) {
			插入树项(节点项, "<主信息缺失>");
			return;
		}

		HTREEITEM 详情项 = 插入树项(节点项, "字段详情");
		插入树项(详情项, "主键: " + 节点主键摘要_(节点));
		插入树项(详情项, "节点种类: " + std::string(方法节点种类显示文本_(信息->节点种类)));
		插入树项(详情项, "模板场景子节点数: " + std::to_string(计数同层节点_(static_cast<基础信息节点类*>(信息->场景根节点.子))));
		插入树项(详情项, "运行次数: " + std::to_string(信息->评估原始量.节点_运行次数));
		插入树项(详情项, "成功次数: " + std::to_string(信息->评估原始量.节点_成功次数));
		插入树项(详情项, "失败次数: " + std::to_string(信息->评估原始量.节点_失败次数));
		插入树项(详情项, "样本数: " + std::to_string(信息->评估原始量.节点_样本数));
		插入树项(详情项, "缺口数: " + std::to_string(信息->评估原始量.节点_缺口集合.size()));
		插入树项(详情项, "副作用数: " + std::to_string(信息->评估原始量.节点_副作用集合.size()));
		if (信息->方法虚拟存在根) {
			插入树项(详情项, "方法虚拟存在: " + 节点主键摘要_(static_cast<基础信息节点类*>(信息->方法虚拟存在根)));
		}
		if (方法模板场景有内容_(信息)) {
			插入树项(详情项, "模板场景: 有内容");
		}

		if (auto* 首信息 = dynamic_cast<方法首节点主信息类*>(信息)) {
			插入树项(详情项, "动作名: " + (词显示文本_(首信息->动作名).empty() ? std::string("<未命名方法>") : 词显示文本_(首信息->动作名)));
			插入树项(详情项, "动作句柄: " + 动作句柄显示文本_(首信息->动作句柄));
			插入树项(详情项, "来源: " + std::string(方法来源文本_(首信息->来源)));
			插入树项(详情项, "影响签名数: " + std::to_string(首信息->影响度量签名.size()));
		}
		else if (auto* 条件信息 = dynamic_cast<方法条件节点主信息类*>(信息)) {
			插入树项(详情项, "条件判定数: " + std::to_string(条件信息->条件判定索引.size()));
			if (!条件信息->条件场景模板主键.empty()) {
				插入树项(详情项, "条件键: " + 裁剪摘要文本_(条件信息->条件场景模板主键, 72));
			}
			if (条件信息->时间起 != 0 || 条件信息->时间止 != 0) {
				插入树项(详情项, "时间窗口: " + std::to_string(条件信息->时间起) + " -> " + std::to_string(条件信息->时间止));
			}
		}
		else if (auto* 结果信息 = dynamic_cast<方法结果节点主信息类*>(信息)) {
			插入树项(详情项, "结果摘要项: " + std::to_string(结果信息->结果摘要索引.size()));
		}

		if (节点->子) {
			HTREEITEM 子节点项 = 插入树项(节点项, "子节点 (" + std::to_string(计数同层节点_(static_cast<方法节点类*>(节点->子))) + ")");
			枚举同层节点_(static_cast<方法节点类*>(节点->子), [&](方法节点类* 子节点) {
				追加方法节点树_(子节点项, 子节点);
			});
		}
	};

	枚举根子节点_(static_cast<方法节点类*>(数据仓库模块::方法链.根指针), [&](方法节点类* 节点) {
		if (!节点 || !节点->主信息) return;
		追加方法节点树_(根项, 节点);
	});

	变量_因果树控件.Expand(根项, TVE_EXPAND);
	变量_因果树控件.SetRedraw(TRUE);
	变量_因果树控件.Invalidate();
}

CString C海鱼Dlg::生成标签页文本(int 标签索引) const
{
	if (使用远程后台模式_) {
		return 生成远程标签页文本(标签索引);
	}

	if (本地内核是否停止中()) {
		const char* 标题 = "系统信息";
		switch (标签索引) {
		case 1: 标题 = "任务信息"; break;
		case 2: 标题 = "方法信息"; break;
		case 3: 标题 = "事件信息"; break;
		case 4: 标题 = "因果信息"; break;
		case 5: 标题 = "环境 / 自我状态"; break;
		case 6: 标题 = "学习 / 自我状态"; break;
		case 8: 标题 = "场景信息"; break;
		case 9: 标题 = "需求列表"; break;
		case 10: 标题 = "需求窗口"; break;
		default: break;
		}

		std::ostringstream 停机文本;
		停机文本 << 标题 << "\r\n\r\n";
		停机文本 << "本地内核正在停止中。\r\n";
		停机文本 << "为避免停机阶段继续读取任务、方法、状态、因果等全量数据导致界面卡顿，当前页已暂停详细刷新。\r\n";
		if (!本地最近命令摘要_.empty()) {
			停机文本 << "最近状态: " << 本地最近命令摘要_ << "\r\n";
		}
		停机文本 << "停机完成后可再次启动内核，再查看完整信息。";
		return UTF8到CString_(停机文本.str());
	}

	std::ostringstream os;
	os << "标签索引: " << 标签索引 << "\r\n\r\n";
	const auto 布尔文本_ = [](bool 值) {
		return 值 ? "是" : "否";
		};
	const auto 追加自我线程生命体征面板_ = [&](std::ostringstream& 输出) {
		const auto 宿主快照 = 本地宿主_ ? 本地宿主_->快照() : 结构_自治宿主快照{};
		const bool 线程活着 = 宿主快照.自我线程运行中;
		const bool 初始化完成 = 宿主快照.初始化完成;
		const bool 健康运行 = 宿主快照.健康运行;
		std::string 最近故障 = 宿主快照.最近故障摘要.empty() ? "无" : 宿主快照.最近故障摘要;
		输出 << "线程活着: " << 布尔文本_(线程活着) << "\r\n";
		输出 << "初始化完成: " << 布尔文本_(初始化完成) << "\r\n";
		输出 << "健康运行: " << 布尔文本_(健康运行) << "\r\n";
		输出 << "最近故障: " << 最近故障 << "\r\n";
		};
	const auto 统计根链节点数_ = [](auto* 根节点) -> std::size_t {
		std::size_t 数量 = 0;
		枚举根子节点_(根节点, [&](auto*) { ++数量; });
		return 数量;
		};
	const auto 追加状态事件因果摘要_ = [&](std::ostringstream& 输出) {
		auto* 内部世界 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
		auto* 场景信息 = (内部世界 && 内部世界->主信息) ? dynamic_cast<场景节点主信息类*>(内部世界->主信息) : nullptr;
		std::size_t 状态数 = 0;
		std::size_t 动态数 = 0;
		std::size_t 事件动态数 = 0;
		if (场景信息) {
			状态数 = 场景信息->状态总数();
			动态数 = 场景信息->动态总数();
			事件动态数 = 场景信息->事件动态总数();
		}
		输出 << "状态数: " << 状态数 << "\r\n";
		输出 << "聚合动态数: " << 动态数 << "\r\n";
		输出 << "原始事件动态数: " << 事件动态数 << "\r\n";
		输出 << "因果统计: 已移到“因果信息”页单独查看，避免切页时同步全量扫描导致卡顿\r\n";
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
		追加因果链详细信息_(
			os,
			收集因果节点列表_(),
			收集抽象因果模板节点列表_(),
			收集因果链节点列表_(),
			24);
	}
	else if (标签索引 == 5 || 标签索引 == 6) {
		const auto worldCount = 统计根链节点数_(static_cast<基础信息节点类*>(数据仓库模块::世界链.根指针));
		const auto needCount = 统计根链节点数_(static_cast<需求节点类*>(数据仓库模块::需求链.根指针));
		const auto methodCount = 统计根链节点数_(static_cast<方法节点类*>(数据仓库模块::方法链.根指针));
		const auto topTaskCount = 统计根链节点数_(static_cast<任务节点类*>(数据仓库模块::任务链.根指针));
		os << (标签索引 == 5 ? "环境 / 自我状态" : "学习 / 自我状态") << "\r\n\r\n";
		追加自我线程生命体征面板_(os);
		os << "世界根子节点数: " << worldCount << "\r\n";
		os << "顶层需求数: " << needCount << "\r\n";
		os << "顶层任务数: " << topTaskCount << "\r\n";
		os << "方法数: " << methodCount << "\r\n";
		追加状态事件因果摘要_(os);
		os << "\r\n提示: 该页已改成轻量摘要；任务树、因果树的全量统计请分别到“任务信息”“因果信息”页查看。";
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
			auto* 目标状态信息 = (bestMi->被需求状态 && bestMi->被需求状态->主信息)
				? dynamic_cast<状态节点主信息类*>(bestMi->被需求状态->主信息)
				: nullptr;
			auto* 目标特征信息 = (目标状态信息 && 目标状态信息->状态特征 && 目标状态信息->状态特征->主信息)
				? dynamic_cast<特征节点主信息类*>(目标状态信息->状态特征->主信息)
				: nullptr;
			枚举_需求运行状态 缓存状态 = 枚举_需求运行状态::未知;
			const bool 有缓存状态 = 需求状态管理器.尝试查询缓存需求状态(best, &缓存状态);
			os << "类型: " << (词键_(bestMi->类型).empty() ? "<未命名需求>" : 词键_(bestMi->类型)) << "\r\n";
			os << "权重: " << bestMi->权重 << "\r\n";
			os << "有主体: " << (bestMi->需求主体 ? "是" : "否") << "\r\n";
			os << "有目标状态: " << (bestMi->被需求状态 ? "是" : "否") << "\r\n";
			os << "缓存状态: " << (有缓存状态 ? 需求运行状态文本_(缓存状态) : "未缓存") << "\r\n";
			os << "已满足: " << ((有缓存状态 && 缓存状态 == 枚举_需求运行状态::已满足) ? "是" : "否") << "\r\n";
			os << "相关任务: " << (bestMi->相关任务 ? "有" : "无") << "\r\n";
			os << "目标特征: " << (目标特征信息 ? 词键_(目标特征信息->类型) : std::string("<无>")) << "\r\n";
			os << "目标值: " << (目标状态信息 ? 快照文本_(目标状态信息->状态值) : std::string("<无>")) << "\r\n";
			os << "\r\n";
			os << "提示: 该页面已改为使用缓存摘要，避免点开时在界面线程里现场计算需求现状而卡住。\r\n";
		}
	}
	else {
		const auto needCount = 统计根链节点数_(static_cast<需求节点类*>(数据仓库模块::需求链.根指针));
		const auto methodCount = 统计根链节点数_(static_cast<方法节点类*>(数据仓库模块::方法链.根指针));
		const auto topTaskCount = 统计根链节点数_(static_cast<任务节点类*>(数据仓库模块::任务链.根指针));
		os << "当前标签页暂未单独细化，先显示系统摘要。\r\n\r\n";
		追加自我线程生命体征面板_(os);
		os << "顶层需求数: " << needCount << "\r\n";
		os << "顶层任务数: " << topTaskCount << "\r\n";
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
		os << "因果信息（远程）\r\n\r\n";
		os << "因果节点数: " << 快照.因果数 << "\r\n";
		os << "\r\n";
		if (!快照.因果链摘要.empty()) {
			os << 规范化编辑框换行_(快照.因果链摘要);
		}
		else {
			os << "后台尚未提供详细因果链信息。";
		}
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
		return 取本地内核状态摘要_();
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

bool C海鱼Dlg::本地内核是否停止中() const
{
	return 本地宿主停止状态_.停止中.load();
}

void C海鱼Dlg::刷新当前标签内容()
{
	if (!::IsWindow(变量_TAB1.GetSafeHwnd())) return;
	const int 标签索引 = 变量_TAB1.GetCurSel();
	const bool 用因果树 = 标签页使用因果树控件_(标签索引);
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
	if (::IsWindow(变量_因果树控件.GetSafeHwnd())) {
		变量_因果树控件.ShowWindow(用因果树 ? SW_SHOW : SW_HIDE);
		if (用因果树) {
			变量_交互界面.ShowWindow(SW_HIDE);
			变量_配置及测试窗口.ShowWindow(SW_HIDE);
			变量_基础信息界面.ShowWindow(SW_HIDE);
			更新信息查看框布局();
			刷新因果树控件_();
		}
	}
	if (!用信息框 && 标签索引 == 0 && ::IsWindow(变量_交互界面.GetSafeHwnd())) {
		变量_交互界面.刷新远程后台状态();
	}
}

void C海鱼Dlg::OnBnClicked刷新当前页()
{
	刷新当前界面_(true);
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
	退出清理完成标记_.store(false);
	安排应用退出兜底_();
	日志::运行("[主窗口关闭] 停止后台连接线程开始");
	停止后台连接线程();
	日志::运行("[主窗口关闭] 停止后台连接线程完成");
	日志::运行("[主窗口关闭] 停止自我场景再现开始");
	CString 自我场景错误;
	(void)停止远程自我场景再现(&自我场景错误);
	日志::运行("[主窗口关闭] 停止自我场景再现完成");
	日志::运行("[主窗口关闭] 停止相机观测开始");
	CString 相机错误;
	(void)停止远程相机观测(&相机错误);
	日志::运行("[主窗口关闭] 停止相机观测完成");
	日志::运行("[主窗口关闭] 停止本地/远程内核开始");
	CString 内核错误;
	(void)请求远程后台安全停机(&内核错误);
	日志::运行("[主窗口关闭] 停止本地/远程内核完成");
	日志::运行("[主窗口关闭] 清理子窗口开始");
	清理退出子窗口();
	日志::运行("[主窗口关闭] 清理子窗口完成");
	退出清理完成标记_.store(true);
}

void C海鱼Dlg::安排应用退出兜底_()
{
	::OutputDebugStringA("[主窗口关闭] 已禁用兜底退出线程，避免干扰调试泄漏定位\n");
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
	if (变量_交互界面.m_hWnd == NULL || 变量_TAB1.m_hWnd == NULL)
		return;
	更新主界面布局_();
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
	if (::IsWindow(变量_因果树控件.GetSafeHwnd())) {
		变量_因果树控件.ShowWindow(SW_HIDE);
	}
	const bool 用因果树 = 标签页使用因果树控件_(iIndex);
	const bool 用信息框 = 标签页使用信息查看框(iIndex);
	if (用信息框) {
		if (::IsWindow(变量_信息查看框.GetSafeHwnd())) {
			变量_信息查看框.ShowWindow(SW_SHOW);
		}
	}
	else if (用因果树) {
		if (::IsWindow(变量_因果树控件.GetSafeHwnd())) {
			变量_因果树控件.ShowWindow(SW_SHOW);
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

	刷新当前界面_(false);
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
		auto* 已发布结果 = 结果.get();
		if (!::PostMessage(hwnd, WM_APP_后台连接完成, 0, reinterpret_cast<LPARAM>(已发布结果))) {
			return;
		}
		(void)结果.release();
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

结构_后台前端快照 C海鱼Dlg::生成本地快照_() const
{
	结构_后台前端快照 结果{};
	结果.快照生成UTC毫秒 = 当前UTC毫秒_();
	if (本地内核是否停止中()) {
		结果.后台在线 = false;
		结果.宿主运行中 = false;
		结果.自我线程运行中 = false;
		结果.初始化完成 = false;
		结果.健康运行 = false;
		结果.最近命令摘要 = 本地最近命令摘要_.empty() ? "本地内核正在停止" : 本地最近命令摘要_;
		return 结果;
	}

	const auto 宿主快照 = 本地宿主_ ? 本地宿主_->快照() : 结构_自治宿主快照{};
	结果.后台在线 = 本地宿主_ && 本地宿主_->是否正在运行();
	结果.宿主运行中 = 宿主快照.宿主运行中;
	结果.请求停止 = 宿主快照.请求停止;
	结果.自我线程运行中 = 宿主快照.自我线程运行中;
	结果.初始化完成 = 宿主快照.初始化完成;
	结果.健康运行 = 宿主快照.健康运行;
	结果.请求安全重启 = 宿主快照.请求安全重启;
	结果.致命错误计数 = 宿主快照.致命错误计数;
	结果.已执行重启次数 = 宿主快照.已执行重启次数;
	结果.最近故障摘要 = 宿主快照.最近故障摘要;
	结果.宿主故障摘要 = 宿主快照.宿主故障摘要;

	结果.相机已启用 = (摄像机_ != nullptr);
	if (摄像机_) {
		const auto 相机快照 = 摄像机_->快照();
		结果.相机运行中 = 相机快照.运行中;
		结果.相机使用虚拟回退 = 相机快照.使用虚拟相机;
		结果.相机已处理帧数 = 相机快照.已处理帧数;
		结果.最近桥接候选数 = 相机快照.最近桥接候选数;
		结果.相机状态摘要 = 相机快照.最近状态摘要;
	}

	结果.最近输入序号 = 本地最近输入序号_;
	结果.最近输入文本 = 本地最近输入文本_;
	结果.最近命令摘要 = 本地最近命令摘要_;

	枚举根子节点_(static_cast<基础信息节点类*>(数据仓库模块::世界链.根指针), [&](基础信息节点类*) {
		++结果.世界根子节点数;
	});
	枚举根子节点_(static_cast<需求节点类*>(数据仓库模块::需求链.根指针), [&](需求节点类* n) {
		++结果.需求数;
		auto* mi = n ? n->主信息 : nullptr;
		if (!mi) return;
		if (结果.需求数 == 1 || static_cast<std::int64_t>(mi->权重) > 结果.主要需求权重) {
			结果.主要需求权重 = static_cast<std::int64_t>(mi->权重);
			结果.主要需求类型 = 主要需求类型显示文本_(词键_(mi->类型));
		}
	});

	枚举根子节点_(static_cast<任务节点类*>(数据仓库模块::任务链.根指针), [&](任务节点类*) {
		++结果.任务数;
	});

	枚举根子节点_(static_cast<方法节点类*>(数据仓库模块::方法链.根指针), [&](方法节点类*) {
		++结果.方法数;
	});

	auto* 内部世界 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
	auto* 内部场景信息 = (内部世界 && 内部世界->主信息) ? dynamic_cast<场景节点主信息类*>(内部世界->主信息) : nullptr;
	if (内部场景信息) {
		结果.状态数 = static_cast<std::uint64_t>(内部场景信息->状态总数());
		结果.动态数 = static_cast<std::uint64_t>(内部场景信息->动态总数());
		结果.事件动态数 = static_cast<std::uint64_t>(内部场景信息->事件动态总数());
	}
	I64 当前值 = 0;
	if (尝试取当前自我特征I64_(特征类型定义类::类型_自我_安全值, &当前值)) {
		结果.有当前安全值 = true;
		结果.当前安全值 = 当前值;
	}
	if (尝试取当前自我特征I64_(特征类型定义类::类型_自我_服务值, &当前值)) {
		结果.有当前服务值 = true;
		结果.当前服务值 = 当前值;
	}

	if (结果.需求数 > 0) {
		需求节点类* 最佳需求 = nullptr;
		I64 最佳权重 = std::numeric_limits<I64>::min();
		枚举根子节点_(static_cast<需求节点类*>(数据仓库模块::需求链.根指针), [&](需求节点类* n) {
			auto* mi = n ? n->主信息 : nullptr;
			if (!mi) return;
			const auto 当前权重 = static_cast<I64>(mi->权重);
			if (!最佳需求 || 当前权重 > 最佳权重) {
				最佳需求 = n;
				最佳权重 = 当前权重;
			}
		});
		if (最佳需求) {
			枚举_需求运行状态 缓存状态 = 枚举_需求运行状态::未知;
			if (需求状态管理器.尝试查询缓存需求状态(最佳需求, &缓存状态)) {
				结果.主要需求已满足 = (缓存状态 == 枚举_需求运行状态::已满足);
			}
		}
	}

	return 结果;
}

CString C海鱼Dlg::取本地内核状态摘要_() const
{
	if (本地内核是否停止中()) {
		return _T("本地内核 | 正在停止，界面不会再等待内核完全退出");
	}
	const auto 宿主快照 = 本地宿主_ ? 本地宿主_->快照() : 结构_自治宿主快照{};
	const bool 宿主运行中 = 本地宿主_ && 本地宿主_->是否正在运行();
	bool 有当前安全值 = false;
	bool 有当前服务值 = false;
	I64 当前安全值 = 0;
	I64 当前服务值 = 0;
	if (尝试取当前自我特征I64_(特征类型定义类::类型_自我_安全值, &当前安全值)) {
		有当前安全值 = true;
	}
	if (尝试取当前自我特征I64_(特征类型定义类::类型_自我_服务值, &当前服务值)) {
		有当前服务值 = true;
	}

	bool 相机已启用 = (摄像机_ != nullptr);
	bool 相机运行中 = false;
	if (摄像机_) {
		const auto 相机快照 = 摄像机_->快照();
		相机运行中 = 相机快照.运行中;
	}

	std::ostringstream os;
	os << "本地内核";
	os << " | 宿主=" << (宿主运行中 ? "运行" : "停止");
	os << " | 自我=" << (宿主快照.自我线程运行中 ? "运行" : "待机");
	if (宿主快照.初始化完成) {
		os << " | " << (宿主快照.健康运行 ? "健康" : "待恢复");
	}
	if (有当前安全值 || 有当前服务值) {
		os << " | ";
		if (有当前安全值) {
			os << "安全" << 当前安全值;
		}
		if (有当前安全值 && 有当前服务值) {
			os << " ";
		}
		if (有当前服务值) {
			os << "服务" << 当前服务值;
		}
	}
	if (相机已启用) {
		os << " | 相机=" << (相机运行中 ? "运行" : "停止");
	}
	if (!本地最近命令摘要_.empty()) {
		os << " | " << 本地最近命令摘要_;
	}
	if (!宿主快照.最近故障摘要.empty()) {
		os << " | 故障=" << 宿主快照.最近故障摘要;
	}
	return UTF8到CString_(os.str());
}

bool C海鱼Dlg::启动本地内核_(CString* 错误文本)
{
	if (错误文本) 错误文本->Empty();
	try {
		if (本地内核是否停止中()) {
			if (错误文本) *错误文本 = _T("本地内核正在停止，请稍候再启动。");
			return false;
		}
		if (!本地宿主_) {
			本地宿主_ = std::make_unique<自治宿主类>();
		}
		if (!本地宿主_->是否正在运行()) {
			本地宿主_->启动();
			本地最近命令摘要_ = "本地内核已启动";
			日志::运行("[前端界面] 已启动本地一体化宿主");
		}
		else {
			本地最近命令摘要_ = "本地内核已在运行";
		}
		后台在线_ = true;
		return true;
	}
	catch (const std::exception& ex) {
		if (错误文本) {
			*错误文本 = UTF8到CString_(std::string("启动本地内核异常: ") + ex.what());
		}
		日志::运行_错误("[前端界面] 启动本地内核异常");
		return false;
	}
	catch (...) {
		if (错误文本) *错误文本 = _T("启动本地内核时发生未知异常。");
		日志::运行_错误("[前端界面] 启动本地内核发生未知异常");
		return false;
	}
}

void C海鱼Dlg::异步回收本地宿主_(std::unique_ptr<自治宿主类> 待停止宿主)
{
	if (!待停止宿主) return;
	本地宿主停止状态_.停止中.store(true);
	try {
		待停止宿主->停止();
		日志::运行("[前端界面] 本地一体化宿主已完成同步停机回收");
	}
	catch (const std::exception& ex) {
		日志::运行_错误(std::string("[前端界面] 本地一体化宿主同步停机异常: ") + ex.what());
	}
	catch (...) {
		日志::运行_错误("[前端界面] 本地一体化宿主同步停机发生未知异常");
	}
	本地宿主停止状态_.停止中.store(false);
}

bool C海鱼Dlg::停止本地内核_(CString* 错误文本)
{
	if (错误文本) 错误文本->Empty();
	try {
		if (本地内核是否停止中()) {
			本地最近命令摘要_ = "本地内核正在停止";
			return true;
		}
		本地自我场景再现已启用_ = false;
		本地自我场景再现状态摘要_ = "自我场景再现已停止";
		本地自我场景再现帧序号_ = 0;
		if (摄像机_) {
			摄像机_->停止();
			摄像机_.reset();
		}
		auto 待停止宿主 = std::move(本地宿主_);
		if (待停止宿主) {
			待停止宿主->请求停止();
			异步回收本地宿主_(std::move(待停止宿主));
			日志::运行("[前端界面] 已发出本地一体化宿主停机请求，转入异步回收");
		}
		后台在线_ = false;
		本地最近命令摘要_ = 本地内核是否停止中() ? "本地内核正在停止" : "本地内核已停止";
		return true;
	}
	catch (const std::exception& ex) {
		if (错误文本) {
			*错误文本 = UTF8到CString_(std::string("停止本地内核异常: ") + ex.what());
		}
		日志::运行_错误("[前端界面] 停止本地内核异常");
		return false;
	}
	catch (...) {
		if (错误文本) *错误文本 = _T("停止本地内核时发生未知异常。");
		日志::运行_错误("[前端界面] 停止本地内核发生未知异常");
		return false;
	}
}

bool C海鱼Dlg::提交本地人类输入_(const CString& 文本, CString* 错误文本)
{
	if (错误文本) 错误文本->Empty();
	if (!本地宿主_ || !本地宿主_->是否正在运行()) {
		if (错误文本) *错误文本 = _T("本地内核未运行，请先启动内核。");
		return false;
	}

	CString 清洗后 = 文本;
	清洗后.Trim();
	if (清洗后.IsEmpty()) {
		if (错误文本) *错误文本 = _T("输入为空。");
		return false;
	}

	const CW2A utf8转换器(清洗后, CP_UTF8);
	const std::string utf8 = static_cast<const char*>(utf8转换器);
	const auto 序号 = 本地下一个输入序号_++;
	const auto 提交时间 = 结构体_时间戳::当前_微秒();
	auto* 输入场景 = 确保人类输入桥场景_("主窗口_人类输入桥输入", "主窗口人类输入桥场景", 提交时间);
	if (!输入场景) {
		if (错误文本) *错误文本 = _T("创建本地人类输入桥场景失败。");
		return false;
	}

	本地最近输入对象_ = std::make_shared<std::string>(utf8);
	本地最近输入序号_ = 序号;
	本地最近输入文本_ = 裁剪摘要文本_(utf8);
	本地最近命令摘要_ = "已接收本地输入 #" + std::to_string(序号);

	(void)世界树.写入特征_I64(
		输入场景,
		语素集.添加词性词("人类输入_存在", "名词"),
		1,
		{},
		"C海鱼Dlg::提交本地人类输入_/写存在");
	(void)世界树.写入特征_I64(
		输入场景,
		语素集.添加词性词("人类输入_序号", "名词"),
		static_cast<I64>(序号),
		{},
		"C海鱼Dlg::提交本地人类输入_/写序号");
	(void)世界树.写入特征_I64(
		输入场景,
		语素集.添加词性词("人类输入_来源码", "名词"),
		1001,
		{},
		"C海鱼Dlg::提交本地人类输入_/写来源码");
	(void)世界树.写入特征_指针(
		输入场景,
		语素集.添加词性词("人类输入_对象指针", "名词"),
		reinterpret_cast<std::uintptr_t>(本地最近输入对象_.get()),
		{},
		"C海鱼Dlg::提交本地人类输入_/写对象指针");

	日志::运行f(
		"[前端界面] 本地输入已桥接: 序号={}, 字节数={}",
		static_cast<unsigned long long>(序号),
		static_cast<unsigned long long>(utf8.size()));
	return true;
}

bool C海鱼Dlg::查询本地自我场景快照_(结构_后台自我场景快照* 输出, CString* 错误文本)
{
	if (!输出) {
		if (错误文本) *错误文本 = _T("输出缓冲为空。");
		return false;
	}
	if (错误文本) 错误文本->Empty();

	if (本地内核是否停止中()) {
		*输出 = {};
		输出->后台在线 = true;
		输出->再现运行中 = false;
		输出->帧序号 = 本地自我场景再现帧序号_;
		输出->状态摘要 = "本地内核正在停止，自我场景再现已暂停刷新";
		return true;
	}

	const auto 本地快照 = 生成本地快照_();
	if (!本地快照.宿主运行中) {
		if (错误文本) *错误文本 = _T("本地内核未运行。");
		*输出 = {};
		return false;
	}

	输出->后台在线 = true;
	输出->再现运行中 = 本地自我场景再现已启用_;
	输出->帧序号 = 本地自我场景再现帧序号_;
	输出->状态摘要 = 本地自我场景再现状态摘要_;
	if (!本地自我场景再现已启用_) {
		return true;
	}

	if (!摄像机_) {
		本地自我场景再现状态摘要_ = "当前未启用相机桥接缓存";
		输出->状态摘要 = 本地自我场景再现状态摘要_;
		return true;
	}

	const auto 相机快照 = 摄像机_->快照();
	const auto 桥接存在列表 = 摄像机_->取最近桥接存在列表();
	输出->场景主键 = "主窗口_自我观察输入/候选存在集";
	输出->存在列表.clear();
	输出->存在列表.reserve(桥接存在列表.size());
	for (const auto& 存在 : 桥接存在列表) {
		结构_后台自我场景存在快照 项{};
		项.存在主键 = "bridge_candidate_" + std::to_string(static_cast<long long>(存在.跟踪ID));
		项.有位置 = true;
		项.中心X_mm = 存在.中心X_mm;
		项.中心Y_mm = 存在.中心Y_mm;
		项.中心Z_mm = 存在.中心Z_mm;
		项.有尺寸 = true;
		项.尺寸X_mm = 存在.尺寸X_mm;
		项.尺寸Y_mm = 存在.尺寸Y_mm;
		项.尺寸Z_mm = 存在.尺寸Z_mm;
		项.轮廓块数 = 存在.轮廓块数;
		输出->存在列表.push_back(std::move(项));
	}

	输出->帧序号 = ++本地自我场景再现帧序号_;
	if (!相机快照.运行中) {
		本地自我场景再现状态摘要_ = "相机运行时未运行";
	}
	else if (相机快照.已处理帧数 == 0) {
		本地自我场景再现状态摘要_ = "相机已启动，尚未处理到可回放帧";
	}
	else if (输出->存在列表.empty()) {
		本地自我场景再现状态摘要_ = "相机桥接已运行，但当前没有存在";
	}
	else {
		本地自我场景再现状态摘要_ = "已提取 " + std::to_string(输出->存在列表.size()) + " 个存在（来自相机桥接）";
	}
	输出->状态摘要 = 本地自我场景再现状态摘要_;
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
		日志::运行("[前端界面] 已通过界面刷新确认远程后台在线");
	}
	if (::IsWindow(变量_交互界面.GetSafeHwnd())) {
		变量_交互界面.刷新远程后台状态();
	}
	return true;
}

bool C海鱼Dlg::提交远程人类输入(const CString& 文本, CString* 错误文本)
{
	if (!使用远程后台模式_) {
		return 提交本地人类输入_(文本, 错误文本);
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
		return 启动本地内核_(错误文本);
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
		return 停止本地内核_(错误文本);
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
		if (错误文本) 错误文本->Empty();
		if (!本地宿主_ || !本地宿主_->是否正在运行()) {
			if (错误文本) *错误文本 = _T("本地内核未运行，请先启动内核。");
			return false;
		}
		if (!摄像机_) {
			摄像机_ = std::make_unique<摄像机运行时>();
		}
		if (!摄像机_->是否正在运行() && !摄像机_->启动()) {
			if (错误文本) *错误文本 = _T("本地相机启动失败。");
			return false;
		}
		本地最近命令摘要_ = "已启动本地相机";
		日志::运行("[前端界面] 已启动本地相机观测运行时");
		return true;
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
		if (错误文本) 错误文本->Empty();
		if (摄像机_) {
			摄像机_->停止();
			摄像机_.reset();
		}
		本地最近命令摘要_ = "已停止本地相机";
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
		if (错误文本) 错误文本->Empty();
		if (!本地宿主_ || !本地宿主_->是否正在运行()) {
			if (错误文本) *错误文本 = _T("本地内核未运行，请先启动内核。");
			return false;
		}
		本地自我场景再现已启用_ = true;
		本地自我场景再现状态摘要_ = "自我场景再现已启动";
		本地最近命令摘要_ = "已启动本地自我场景再现";
		return true;
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
		本地自我场景再现已启用_ = false;
		本地自我场景再现帧序号_ = 0;
		本地自我场景再现状态摘要_ = "自我场景再现已停止";
		本地最近命令摘要_ = "已停止本地自我场景再现";
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
		return 查询本地自我场景快照_(输出, 错误文本);
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












