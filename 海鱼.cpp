
// 海鱼.cpp: 定义应用程序的类行为。
//

#include "pch.h"
#include "framework.h"
#include "海鱼.h"
#include "海鱼Dlg.h"
#include "鱼巢Headless入口.h"

#ifdef _DEBUG
#include <crtdbg.h>
#endif
#include <shellapi.h>

import 数据仓库模块;
import 因果信息模块;
import 世界树环境模块;
import 特征值环境模块;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C海鱼App

BEGIN_MESSAGE_MAP(C海鱼App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// C海鱼App 构造

C海鱼App::C海鱼App()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 C海鱼App 对象

C海鱼App theApp;

const GUID CDECL BASED_CODE _tlid =
		{0x9cf2e7e9,0xbd5a,0x4c2b,{0x83,0xa5,0x1f,0xc0,0xf0,0xee,0x2a,0xc3}};
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

namespace {
#ifdef _DEBUG
void 配置调试堆_()
{
	int 标志 = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	标志 |= _CRTDBG_ALLOC_MEM_DF;
	标志 |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(标志);

	char* raw = nullptr;
	size_t len = 0;
	if (_dupenv_s(&raw, &len, "HY_BREAK_ALLOC") == 0 && raw) {
		char* end = nullptr;
		const long 分配号 = std::strtol(raw, &end, 10);
		std::free(raw);
		if (end && *end == '\0' && 分配号 > 0) {
			_CrtSetBreakAlloc(分配号);
			std::string msg = "[调试堆] 已设置 _CrtSetBreakAlloc = " + std::to_string(分配号) + "\n";
			::OutputDebugStringA(msg.c_str());
		}
	}
}
#endif

bool 运行于受控硬件采集模式_()
{
	char* raw = nullptr;
	size_t len = 0;
	if (_dupenv_s(&raw, &len, "HY_CAPTURE_ONLY") == 0 && raw) {
		std::string value(raw);
		std::free(raw);
		return value == "1";
	}
	return false;
}

struct 命令行参数持有_ {
	int argc = 0;
	LPWSTR* argv = nullptr;

	~命令行参数持有_()
	{
		if (argv) {
			::LocalFree(argv);
		}
	}
};

命令行参数持有_ 取当前命令行参数_()
{
	命令行参数持有_ 参数{};
	参数.argv = ::CommandLineToArgvW(::GetCommandLineW(), &参数.argc);
	if (!参数.argv) {
		参数.argc = 0;
	}
	return 参数;
}

void 绑定标准流到控制台_()
{
	FILE* 文件 = nullptr;
	if (freopen_s(&文件, "CONOUT$", "w", stdout) == 0 && 文件) {
		std::setvbuf(stdout, nullptr, _IONBF, 0);
	}
	文件 = nullptr;
	if (freopen_s(&文件, "CONOUT$", "w", stderr) == 0 && 文件) {
		std::setvbuf(stderr, nullptr, _IONBF, 0);
	}
	文件 = nullptr;
	(void)freopen_s(&文件, "CONIN$", "r", stdin);
	std::ios::sync_with_stdio(false);
}

void 确保Headless控制台可用_()
{
	if (!::GetConsoleWindow()) {
		if (!::AttachConsole(ATTACH_PARENT_PROCESS)) {
			::AllocConsole();
		}
	}
	绑定标准流到控制台_();
}
}


// C海鱼App 初始化

BOOL C海鱼App::InitInstance()
{
#ifdef _DEBUG
	配置调试堆_();
#endif

	// 如果应用程序存在以下情况，Windows XP 上需要 InitCommonControlsEx()
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	auto 命令行参数 = 取当前命令行参数_();
	if (命令行参数.argv && 鱼巢Headless_命令行应接管(命令行参数.argc, 命令行参数.argv))
	{
		if (鱼巢Headless_命令行需要控制台(命令行参数.argc, 命令行参数.argv)) {
			确保Headless控制台可用_();
		}
		记录直接退出码(鱼巢Headless_入口(命令行参数.argc, 命令行参数.argv));
		return FALSE;
	}

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	const bool 受控硬件采集模式 = 运行于受控硬件采集模式_();

	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	// 保留命令行解析，但控制面板不再作为 OLE 自动化服务器运行。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	C海鱼Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

void C海鱼App::记录直接退出码(int 退出码)
{
	使用直接退出码_ = true;
	直接退出码_ = 退出码;
}

int C海鱼App::ExitInstance()
{
	try {
		因果集.调试清空运行期缓存();
		特征值集.调试清空缓存();

		世界树.现实世界 = nullptr;
		世界树.文本世界 = nullptr;
		世界树.想象世界 = nullptr;
		世界树.记忆世界 = nullptr;
		世界树.推理世界 = nullptr;
		世界树.虚拟世界 = nullptr;
		世界树.自我指针 = nullptr;
		世界树.自我所在场景 = nullptr;

		数据仓库模块::方法链.删除链表();
		数据仓库模块::任务链.删除链表();
		数据仓库模块::需求链.删除链表();
		数据仓库模块::语言链.删除链表();
		数据仓库模块::语素链.删除链表();
		数据仓库模块::特征值链.删除链表();
		数据仓库模块::世界链.删除链表();
	}
	catch (...) {
		::OutputDebugStringA("[ExitInstance] 清理全局链/缓存时发生异常，继续退出\n");
	}

	if (使用直接退出码_) {
		return 直接退出码_;
	}
	return CWinApp::ExitInstance();
}
