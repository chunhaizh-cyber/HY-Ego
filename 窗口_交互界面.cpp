// 窗口_交互界面.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "海鱼.h"
#include "海鱼Dlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"
#include <stdio.h>
#include <iostream>
#include "百度分词类.h"

////#include <stdexcept>
//#include <curl/curl.h>
//#include <json/json.h>
//#include <unordered_map>
////#include <codecvt>
//#include <atlbase.h> // 用于CString
//#include <iostream>
//
//
//
//#define CURL_STATICLIB  // libcurl需要
//#pragma comment(lib, "wininet.lib")
//#pragma comment(lib, "Ws2_32.lib")  // libcurl需要
//#pragma comment(lib, "Wldap32.lib")  // libcurl需要
//#pragma comment(lib, "Crypt32.lib")  // libcurl需要
//#pragma comment(lib, "Normaliz.lib")  // libcurl需要

// 窗口_交互界面 对话框
import 语素环境模块;
import 世界树环境模块;


IMPLEMENT_DYNAMIC(窗口_交互界面, CDialogEx)

窗口_交互界面::窗口_交互界面(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

	EnableAutomation();

}

窗口_交互界面::~窗口_交互界面()
{
}

void 窗口_交互界面::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。  基类将自动
	// 删除该对象。  在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CDialogEx::OnFinalRelease();
}

void 窗口_交互界面::DoDataExchange(CDataExchange* pDX)
{

	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, 变量_智能输出文本条);
	DDX_Control(pDX, IDC_STATIC2, 变量_消息输出文本条);
	DDX_Control(pDX, IDC_STATIC3, 变量_交互者输入文本条);
	DDX_Control(pDX, IDCANCEL, 变量_取消按钮);
	DDX_Control(pDX, IDC_EDIT3, 变量_交互者输入框);
	DDX_Control(pDX, IDC_EDIT1, 变量_消息输出框);
	DDX_Control(pDX, IDC_EDIT2, 变量_智能输出框);
	DDX_Control(pDX, IDOK, 变量_确定按钮);
	DDX_Control(pDX, IDC_BUTTON2, 变量_正确按钮);
	DDX_Control(pDX, IDC_BUTTON3, 变量_错误按钮);
	DDX_Control(pDX, IDC_BUTTON4, 变量_不知道按钮);
	DDX_Control(pDX, IDC_STATIC5, 变量_自我状态文本条);
	DDX_Control(pDX, IDC_BUTTON1, 变量_自我状态按钮);
	DDX_Control(pDX, IDC_STATIC4, 变量_右侧固定组框);
	DDX_Control(pDX, IDC_STATIC6, 变量_右下固定组框);
}


BEGIN_MESSAGE_MAP(窗口_交互界面, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &窗口_交互界面::OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(窗口_交互界面, CDialogEx)
END_DISPATCH_MAP()

// 注意: 我们添加了对 IID_I窗口_交互界面 的支持来支持类型安全绑定
//  以支持来自 VBA 的类型安全绑定。  此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {34fd6fe4-f559-4973-aeb5-15648dc96396}
static const IID IID_I窗口_交互界面 =
{0x34fd6fe4,0xf559,0x4973,{0xae,0xb5,0x15,0x64,0x8d,0xc9,0x63,0x96}};

BEGIN_INTERFACE_MAP(窗口_交互界面, CDialogEx)
	INTERFACE_PART(窗口_交互界面, IID_I窗口_交互界面, Dispatch)
END_INTERFACE_MAP()


// 窗口_交互界面 消息处理程序

void 窗口_交互界面::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
 //   // 最小化时不处理
    if (nType == SIZE_MINIMIZED || !::IsWindow(m_hWnd))
        return;
 
   
	// TODO: 在此处添加消息处理程序代码
	if (主窗口指针->变量_交互界面.m_hWnd == NULL)
		return;
	if (变量_交互者输入文本条.m_hWnd == NULL)
		return;
	if (nType == SIZE_MINIMIZED || !::IsWindow(m_hWnd)) return; // 最小化时不处理


	// 直接使用绑定的成员变量
	RECT 右侧固定组框位置变量, 右下固定组框位置变量;
	RECT rect, nrect;

	int 组框宽度 = 260;  // 组框固定宽度
	int 边距 = 10;           // 边距
	主窗口指针->变量_TAB1.GetWindowRect(&rect);

	// 计算新位置和尺寸
	右侧固定组框位置变量.left = cx - 组框宽度 - 边距; // 右侧对齐
	右侧固定组框位置变量.right = 右侧固定组框位置变量.left + 组框宽度;
	右侧固定组框位置变量.top = 边距;                      // 顶部边距
	右侧固定组框位置变量.bottom = cy - 边距;        // 高度填满客户区
	// 调用 MoveWindow 调整位置和大小
	变量_右侧固定组框.MoveWindow(&右侧固定组框位置变量);


	右下固定组框位置变量.left = 右侧固定组框位置变量.left + 5;
	右下固定组框位置变量.right = 右侧固定组框位置变量.right - 5;
	右下固定组框位置变量.top = (右侧固定组框位置变量.bottom - 右侧固定组框位置变量.top) / 2;
	右下固定组框位置变量.bottom = 右侧固定组框位置变量.bottom - 5;
	变量_右下固定组框.MoveWindow(&右下固定组框位置变量);

	int 文本高度, 按钮高度, 可变高度, 按钮宽度;
	变量_交互者输入文本条.GetClientRect(&rect);
	文本高度 = rect.bottom - rect.top;

	变量_确定按钮.GetClientRect(&rect);
	按钮高度 = rect.bottom - rect.top;
	按钮宽度 = rect.right - rect.left;
	可变高度 = cy - 80 - 文本高度 * 3 - 按钮高度;

	rect.left = 边距;
	rect.right = 右侧固定组框位置变量.left - 边距;
	rect.top = 10;
	rect.bottom = rect.top + 文本高度;
	变量_消息输出文本条.MoveWindow(&rect, TRUE);



	//rect = nrect;
	rect.top = rect.bottom + 10;
	rect.bottom = rect.top + 可变高度 / 2;
	变量_消息输出框.MoveWindow(&rect, TRUE);

	nrect = rect;
	nrect.top = nrect.bottom - 50;
	nrect.left = nrect.right + 60;
	nrect.right = nrect.left + 50;
	//变量_心情图标.MoveWindow(&nrect, TRUE);
	rect.top = rect.bottom + 10;
	rect.bottom = rect.top + 文本高度;
	变量_智能输出文本条.MoveWindow(&rect, TRUE);

	rect.top = rect.bottom + 10;
	rect.bottom = rect.top + 可变高度 / 4;
	变量_智能输出框.MoveWindow(&rect, TRUE);


	nrect = rect;



	nrect.left = nrect.left + 15 + 按钮宽度;
	nrect.top = nrect.top + 20;
	nrect.right = nrect.right - 10;
	nrect.bottom = nrect.bottom - 10;
	//	变量_文本框_任务信息.MoveWindow(&nrect, TRUE);
		//	rect = nrect;
	rect.top = rect.bottom + 10;
	rect.bottom = rect.top + 文本高度;
	变量_交互者输入文本条.MoveWindow(&rect, TRUE);

	rect.top = rect.bottom + 10;
	rect.bottom = rect.top + 可变高度 / 4;
	变量_交互者输入框.MoveWindow(&rect, TRUE);

	rect.top = rect.bottom + 10;
	rect.bottom = rect.top + 按钮高度;
	
	rect.left = 边距+(rect.right- rect.left) / 2- 按钮宽度;
	rect.right = rect.left + 按钮宽度;
	变量_确定按钮.MoveWindow(&rect, TRUE);

	rect.left =rect.left+20 + 按钮宽度;
	rect.right = rect.left + 按钮宽度;	
	变量_取消按钮.MoveWindow(&rect, TRUE);



	nrect.left = 右下固定组框位置变量.left + 边距;
	nrect.right = 右下固定组框位置变量.right - 边距;

	nrect.bottom = 右下固定组框位置变量.bottom - 按钮高度 - 边距;
	nrect.top = nrect.bottom - 按钮高度;
	变量_不知道按钮.MoveWindow(&nrect, TRUE);

	nrect.bottom = nrect.top - 边距;
	nrect.top = nrect.bottom - 按钮高度;
	变量_错误按钮.MoveWindow(&nrect, TRUE);

	nrect.bottom = nrect.top - 边距;
	nrect.top = nrect.bottom - 按钮高度;
	变量_正确按钮.MoveWindow(&nrect, TRUE);


	nrect.bottom = 右下固定组框位置变量.top - 边距;
	nrect.top = nrect.bottom - 按钮高度;
	变量_自我状态按钮.MoveWindow(&nrect, TRUE);



	nrect.bottom = nrect.top - 边距;
	nrect.top = nrect.bottom - 按钮高度;
	变量_自我状态文本条.MoveWindow(&nrect, TRUE);
}

void 窗口_交互界面::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString 输入文本, 历史文本;
	变量_交互者输入框.GetWindowTextW(输入文本);
	if (输入文本.GetLength() == 0)
	{
		变量_交互者输入框.SetFocus();
		return;
	}

	变量_消息输出框.GetWindowTextW(历史文本);
	变量_消息输出框.SetWindowTextW(输入文本 + _T("\r\n") +历史文本  );
	变量_交互者输入框.SetWindowTextW(_T(" "));
	变量_交互者输入框.SetFocus();
	
	
	CT2A utf8Converter(输入文本, CP_UTF8);
	std::string utf8Str = utf8Converter.m_psz;
	std::vector<结构体_分词> 结果;
	try {
		std::vector<Token> tokens = nlp_.lexer(utf8Str);
		for (auto& t : tokens) {
			std::string 词 = t.word;
			std::string 词性 = t.pos;

	//		枚举_词性 枚举词性 = 枚举_词性_工厂::根据文本获取枚举值(词性);
	//		词性节点类* 词性节点 = 语素集.添加词性词(词, 词性);
	//		词性主信息类* 词性主信息 = dynamic_cast<词性主信息类*>(词性节点->主信息);
	//		if (词性主信息->对应基础信息指针 == nullptr) {
	//			基础信息节点类* 基础信息节点 = nullptr; //宇宙环境::世界树.世界树_根据词性创建默认基础信息节点(词性节点,枚举词性);
	//			词性主信息->对应基础信息指针 = 基础信息节点;
	//		}

		}

	}
	catch (const std::exception& ex) {
		CStringA msgA(ex.what());
		CString msgW(msgA);
		AfxMessageBox(_T("NLP 调用失败：\n") + msgW); // MFC 弹窗
	}
}


BOOL 窗口_交互界面::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
//	nlp_.初始化(app_id,api_key, secret_key);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
