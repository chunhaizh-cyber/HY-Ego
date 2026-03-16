#pragma once
#include "afxdialogex.h"

#include <string>
#include <vector>
#include <mutex>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <memory>

#include "百度分词类.h"
import 基础数据类型模块;
import 世界树模块;
import 主信息定义模块;

//import 动作模块;

// 窗口_交互界面 对话框

class 窗口_交互界面类 : public CDialogEx
{
	DECLARE_DYNAMIC(窗口_交互界面类)

public:
	窗口_交互界面类(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~窗口_交互界面类();

	virtual void OnFinalRelease();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CStatic 变量_智能输出文本条;
	CStatic 变量_消息输出文本条;
	CStatic 变量_交互者输入文本条;
	CButton 变量_取消按钮;
	CEdit 变量_交互者输入框;
	CEdit 变量_消息输出框;
	CEdit 变量_智能输出框;
	CButton 变量_确定按钮;
	CButton 变量_正确按钮;
	CButton 变量_错误按钮;
	CButton 变量_不知道按钮;
	CStatic 变量_自我状态文本条;
	CButton 变量_自我状态按钮;
	CStatic 变量_右侧固定组框;
	CStatic 变量_右下固定组框;


	afx_msg void OnSize(UINT nType, int cx, int cy);



	afx_msg void OnBnClickedOk();
private:
//	static std::mutex nlp_mutex;
	const int MAX_RETRY = 3;
	const std::chrono::milliseconds RETRY_DELAY{ 500 };
	BaiduLexerClient nlp_;
	bool 百度分词已配置_ = false;
	std::string 百度分词配置错误_;
	void 初始化百度分词客户端();
	void 等待(int 毫秒);
	// 带重试机制的API调用



	//CString ConvertUTF8ToCString(std::string utf8str);//UTF8格式转宽字符
	void 百度分词(std::string 字符串, std::vector<结构体_分词>& 分词结果);
	
	



	inline size_t onWriteData(void* buffer, size_t size, size_t nmemb, void* userp)
	{
		std::string* str = dynamic_cast<std::string*>((std::string*)userp);
		str->append((char*)buffer, size * nmemb);
		return nmemb;
	}

	int 分词(int argc, char* argv[]);

public:
	virtual BOOL OnInitDialog();
};
