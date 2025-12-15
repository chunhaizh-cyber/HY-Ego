
// DlgProxy.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "海鱼.h"
#include "DlgProxy.h"
#include "海鱼Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C海鱼DlgAutoProxy

IMPLEMENT_DYNCREATE(C海鱼DlgAutoProxy, CCmdTarget)

C海鱼DlgAutoProxy::C海鱼DlgAutoProxy()
{
	EnableAutomation();

	// 为使应用程序在自动化对象处于活动状态时一直保持
	//	运行，构造函数调用 AfxOleLockApp。
	AfxOleLockApp();

	// 通过应用程序的主窗口指针
	//  来访问对话框。  设置代理的内部指针
	//  指向对话框，并设置对话框的后向指针指向
	//  该代理。
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(C海鱼Dlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(C海鱼Dlg)))
		{
			m_pDialog = reinterpret_cast<C海鱼Dlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

C海鱼DlgAutoProxy::~C海鱼DlgAutoProxy()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	//	析构函数调用 AfxOleUnlockApp。
	//  除了做其他事情外，这还将销毁主对话框
	if (m_pDialog != nullptr)
		m_pDialog->m_pAutoProxy = nullptr;
	AfxOleUnlockApp();
}

void C海鱼DlgAutoProxy::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。  基类将自动
	// 删除该对象。  在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(C海鱼DlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(C海鱼DlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// 注意: 我们添加了对 IID_IMy 的支持来支持类型安全绑定
//  以支持来自 VBA 的类型安全绑定。  此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {7656fa3a-4221-460a-8f2c-c2291cb2ce1b}
static const IID IID_IMy =
{0x7656fa3a,0x4221,0x460a,{0x8f,0x2c,0xc2,0x29,0x1c,0xb2,0xce,0x1b}};

BEGIN_INTERFACE_MAP(C海鱼DlgAutoProxy, CCmdTarget)
	INTERFACE_PART(C海鱼DlgAutoProxy, IID_IMy, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 宏是在此项目的 pch.h 中定义的
// {7dd0adaa-c857-4017-8a73-a9d37cb8577c}
IMPLEMENT_OLECREATE2(C海鱼DlgAutoProxy, "My.Application", 0x7dd0adaa,0xc857,0x4017,0x8a,0x73,0xa9,0xd3,0x7c,0xb8,0x57,0x7c)


// C海鱼DlgAutoProxy 消息处理程序
