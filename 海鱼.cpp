
// 海鱼.cpp: 定义应用程序的类行为。
//

#include "pch.h"
#include "framework.h"
#include "海鱼.h"
#include "海鱼Dlg.h"

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
}


// C海鱼App 初始化

BOOL C海鱼App::InitInstance()
{
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

int C海鱼App::ExitInstance()
{
	return CWinApp::ExitInstance();
}
