
// DlgProxy.h: 头文件
//

#pragma once

class C海鱼Dlg;


// C海鱼DlgAutoProxy 命令目标

class C海鱼DlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(C海鱼DlgAutoProxy)

	C海鱼DlgAutoProxy();           // 动态创建所使用的受保护的构造函数

// 特性
public:
	C海鱼Dlg* m_pDialog;

// 操作
public:

// 重写
	public:
	virtual void OnFinalRelease();

// 实现
protected:
	virtual ~C海鱼DlgAutoProxy();

	// 生成的消息映射函数

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(C海鱼DlgAutoProxy)

	// 生成的 OLE 调度映射函数

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

