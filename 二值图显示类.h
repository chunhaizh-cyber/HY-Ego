#pragma once
#include "afxdialogex.h"
#include <vector>	// 包含 std::vector
// 二值图显示类 对话框

class 二值图显示类 : public CDialogEx
{
	DECLARE_DYNAMIC(二值图显示类)

public:
	二值图显示类(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~二值图显示类();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_二值图显示 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 设置二值图数据
	void 设置二值图数据(const std::vector<uint8_t>& data, int width, int height);

private:
	std::vector<uint8_t> m_二值图数据;
	int m_宽度 = 0;
	int m_高度 = 0;

protected:
	afx_msg void OnPaint();
};