//
// 鱼巢_外设_相机.cpp
//

#include "pch.h"
#include "framework.h"
#include "鱼巢_外设_相机.h"
#include "鱼巢_外设_相机Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(C鱼巢外设相机App, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

C鱼巢外设相机App theApp;

C鱼巢外设相机App::C鱼巢外设相机App()
{
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

BOOL C鱼巢外设相机App::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls{};
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    AfxEnableControlContainer();

    CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
    SetRegistryKey(_T("鱼巢"));

    C鱼巢外设相机Dlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();
    return FALSE;
}

int C鱼巢外设相机App::ExitInstance()
{
    return CWinApp::ExitInstance();
}
