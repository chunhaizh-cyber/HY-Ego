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
#include <cstdlib>
#include "百度分词类.h"
#include "窗口_交互界面类.h"	

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


IMPLEMENT_DYNAMIC(窗口_交互界面类, CDialogEx)

namespace {
struct 百度凭据配置 {
	std::string app_id;
	std::string api_key;
	std::string secret_key;

	bool 可用() const {
		return !api_key.empty() && !secret_key.empty();
	}
};

std::string 读取环境变量_若存在(const char* 名称)
{
	char* value = nullptr;
	size_t len = 0;
	if (_dupenv_s(&value, &len, 名称) != 0 || value == nullptr) {
		return "";
	}

	std::string result(value);
	free(value);
	return result;
}

bool 任一凭据已提供(const 百度凭据配置& 凭据)
{
	return !凭据.app_id.empty() || !凭据.api_key.empty() || !凭据.secret_key.empty();
}

百度凭据配置 从环境变量读取百度凭据()
{
	百度凭据配置 凭据;
	凭据.app_id = 读取环境变量_若存在("HY_BAIDU_APP_ID");
	if (凭据.app_id.empty()) 凭据.app_id = 读取环境变量_若存在("BAIDU_APP_ID");

	凭据.api_key = 读取环境变量_若存在("HY_BAIDU_API_KEY");
	if (凭据.api_key.empty()) 凭据.api_key = 读取环境变量_若存在("BAIDU_API_KEY");

	凭据.secret_key = 读取环境变量_若存在("HY_BAIDU_SECRET_KEY");
	if (凭据.secret_key.empty()) 凭据.secret_key = 读取环境变量_若存在("BAIDU_SECRET_KEY");
	return 凭据;
}

bool 从本地配置文件读取百度凭据(const char* 路径, 百度凭据配置& out, std::string& error)
{
	std::ifstream 输入流(路径, std::ios::binary);
	if (!输入流.is_open()) return false;

	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;
	Json::Value root;
	std::string errs;
	if (!Json::parseFromStream(builder, 输入流, &root, &errs)) {
		error = std::string("无法解析本地配置文件 config\\baidu_lexer.local.json: ") + errs;
		return false;
	}
	if (!root.isObject()) {
		error = "本地配置文件 config\\baidu_lexer.local.json 必须是 JSON 对象。";
		return false;
	}

	out.app_id = root.get("app_id", "").asString();
	out.api_key = root.get("api_key", "").asString();
	out.secret_key = root.get("secret_key", "").asString();
	return true;
}

bool 加载百度凭据(百度凭据配置& out, std::string& error)
{
	out = 从环境变量读取百度凭据();
	if (out.可用()) return true;
	if (任一凭据已提供(out)) {
		error = "检测到部分环境变量已设置，但百度凭据不完整。请同时提供 HY_BAIDU_API_KEY 与 HY_BAIDU_SECRET_KEY；app_id 可选。";
		return false;
	}

	百度凭据配置 文件凭据;
	if (从本地配置文件读取百度凭据("config\\baidu_lexer.local.json", 文件凭据, error)) {
		if (文件凭据.可用()) {
			out = 文件凭据;
			return true;
		}
		error = "本地配置文件 config\\baidu_lexer.local.json 缺少 api_key 或 secret_key。";
		return false;
	}
	if (!error.empty()) return false;

	error = "未找到百度凭据。请设置环境变量 HY_BAIDU_API_KEY / HY_BAIDU_SECRET_KEY（可选 HY_BAIDU_APP_ID），或创建本地私有文件 config\\baidu_lexer.local.json。";
	return false;
}
} // namespace

窗口_交互界面类::窗口_交互界面类(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	// 控制面板不再作为 OLE 自动化宿主运行，避免关闭时落入旧向导断言链。
}

void 窗口_交互界面类::初始化百度分词客户端()
{
	百度凭据配置 凭据;
	std::string error;
	if (!加载百度凭据(凭据, error)) {
		百度分词已配置_ = false;
		百度分词配置错误_ = error;
		return;
	}

	nlp_.初始化(凭据.app_id, 凭据.api_key, 凭据.secret_key);
	百度分词已配置_ = true;
	百度分词配置错误_.clear();
}

窗口_交互界面类::~窗口_交互界面类()
{
}

void 窗口_交互界面类::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。  基类将自动
	// 删除该对象。  在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CDialogEx::OnFinalRelease();
}

void 窗口_交互界面类::DoDataExchange(CDataExchange* pDX)
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


BEGIN_MESSAGE_MAP(窗口_交互界面类, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &窗口_交互界面类::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &窗口_交互界面类::OnBnClicked自我状态按钮)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(窗口_交互界面类, CDialogEx)
END_DISPATCH_MAP()

// 注意: 我们添加了对 IID_I窗口_交互界面 的支持来支持类型安全绑定
//  以支持来自 VBA 的类型安全绑定。  此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {34fd6fe4-f559-4973-aeb5-15648dc96396}
static const IID IID_I窗口_交互界面 =
{0x34fd6fe4,0xf559,0x4973,{0xae,0xb5,0x15,0x64,0x8d,0xc9,0x63,0x96}};

BEGIN_INTERFACE_MAP(窗口_交互界面类, CDialogEx)
	INTERFACE_PART(窗口_交互界面类, IID_I窗口_交互界面, Dispatch)
END_INTERFACE_MAP()


// 窗口_交互界面 消息处理程序

void 窗口_交互界面类::OnSize(UINT nType, int cx, int cy)
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

void 窗口_交互界面类::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString 输入文本, 历史文本;
	变量_交互者输入框.GetWindowTextW(输入文本);
	if (输入文本.GetLength() == 0)
	{
		变量_交互者输入框.SetFocus();
		return;
	}
	if (主窗口指针 && 主窗口指针->是否使用远程后台模式())
	{
		CString 错误文本;
		if (!主窗口指针->提交远程人类输入(输入文本, &错误文本)) {
			AfxMessageBox(_T("提交后台失败：\n") + 错误文本);
			变量_交互者输入框.SetFocus();
			return;
		}

		变量_消息输出框.GetWindowTextW(历史文本);
		变量_消息输出框.SetWindowTextW(输入文本 + _T("\r\n") + 历史文本);
		变量_智能输出框.SetWindowTextW(_T("已提交到后台，自我线程会按服务阶段继续处理这条输入。"));
		变量_交互者输入框.SetWindowTextW(_T(""));
		变量_交互者输入框.SetFocus();
		return;
	}
	if (!百度分词已配置_)
	{
		CString msg = _T("未配置百度 NLP 凭据。请设置环境变量 HY_BAIDU_API_KEY / HY_BAIDU_SECRET_KEY（可选 HY_BAIDU_APP_ID），或创建本地私有文件 config\\baidu_lexer.local.json。");
		if (!百度分词配置错误_.empty()) {
			CStringA detailA(百度分词配置错误_.c_str());
			CString detailW(detailA);
			msg += _T("\n\n详细信息：");
			msg += detailW;
		}
		AfxMessageBox(msg);
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
	//		const 词性节点类* 词性节点 = 语素集.添加词性词(词, 词性);
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

void 窗口_交互界面类::OnBnClicked自我状态按钮()
{
	if (主窗口指针 && 主窗口指针->是否使用远程后台模式()) {
		if (主窗口指针->后台是否连接中()) {
			变量_智能输出框.SetWindowTextW(_T("后台正在连接中，请稍候，当前无需重复点击。"));
			刷新远程后台状态();
			return;
		}
		CString 错误文本;
		if (主窗口指针->后台是否在线()) {
			if (!主窗口指针->请求远程后台安全停机(&错误文本)) {
				AfxMessageBox(_T("后台停机请求失败：\n") + 错误文本);
				return;
			}
			变量_智能输出框.SetWindowTextW(_T("已请求后台安全停机。需要时可再次点击按钮重新启动后台。"));
		}
		else {
			if (!主窗口指针->启动远程后台(&错误文本)) {
				AfxMessageBox(_T("后台启动失败：\n") + 错误文本);
				return;
			}
			变量_智能输出框.SetWindowTextW(_T("已开始连接后台，前端会在连接完成后自动切换为可提交状态。"));
		}
		刷新远程后台状态();
		return;
	}

	AfxMessageBox(_T("当前是本地内核模式；请在主窗口摘要页查看自我线程状态。"));
}

void 窗口_交互界面类::刷新远程后台状态()
{
	if (!::IsWindow(m_hWnd)) return;
	if (主窗口指针 && 主窗口指针->是否使用远程后台模式()) {
		if (主窗口指针->后台是否连接中()) {
			变量_自我状态按钮.SetWindowTextW(_T("连接中..."));
			变量_自我状态按钮.EnableWindow(FALSE);
			变量_自我状态文本条.SetWindowTextW(主窗口指针->取远程后台状态摘要());
			当前状态提示颜色_ = RGB(180, 120, 0);
		}
		else {
			变量_自我状态按钮.SetWindowTextW(主窗口指针->后台是否在线() ? _T("停止后台") : _T("启动后台"));
			变量_自我状态按钮.EnableWindow(TRUE);
			if (主窗口指针->后台是否在线()) {
				变量_自我状态文本条.SetWindowTextW(主窗口指针->取远程后台状态摘要());
				当前状态提示颜色_ = RGB(0, 120, 50);
			}
			else {
				变量_自我状态文本条.SetWindowTextW(主窗口指针->取远程后台状态摘要());
				当前状态提示颜色_ = RGB(180, 40, 40);
			}
		}
	}
	else {
		变量_自我状态按钮.SetWindowTextW(_T("自我状态"));
		变量_自我状态按钮.EnableWindow(TRUE);
		变量_自我状态文本条.SetWindowTextW(_T("本地内核模式"));
		当前状态提示颜色_ = RGB(40, 90, 160);
	}
	变量_自我状态文本条.Invalidate();
}


BOOL 窗口_交互界面类::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	初始化百度分词客户端();
	LOGFONT 字体信息{};
	if (CFont* 当前字体 = 变量_自我状态文本条.GetFont()) {
		当前字体->GetLogFont(&字体信息);
	}
	else {
		::SystemParametersInfoW(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &字体信息, 0);
	}
	字体信息.lfWeight = FW_BOLD;
	字体信息.lfHeight = -14;
	(void)变量_状态提示字体.CreateFontIndirect(&字体信息);
	变量_自我状态文本条.SetFont(&变量_状态提示字体);
	变量_自我状态文本条.ModifyStyle(0, SS_CENTER);
	if (主窗口指针 && 主窗口指针->是否使用远程后台模式()) {
		变量_智能输出框.SetWindowTextW(_T("当前是远程后台模式：界面会先显示，后台连接完成后即可直接提交输入到鱼巢。"));
	}
	刷新远程后台状态();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

HBRUSH 窗口_交互界面类::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd && pWnd->GetSafeHwnd() == 变量_自我状态文本条.GetSafeHwnd()) {
		pDC->SetTextColor(当前状态提示颜色_);
		pDC->SetBkMode(TRANSPARENT);
		return static_cast<HBRUSH>(::GetSysColorBrush(COLOR_3DFACE));
	}
	return hbr;
}
