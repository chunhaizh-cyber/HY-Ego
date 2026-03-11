// 二值图显示类.cpp: 实现文件
//

#include "pch.h"
#include "海鱼.h"
#include "afxdialogex.h"
#include "二值图显示类.h"
#include <algorithm>


// 二值图显示类 对话框

IMPLEMENT_DYNAMIC(二值图显示类, CDialogEx)

二值图显示类::二值图显示类(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_二值图显示, pParent)
	, m_宽度(0)
	, m_高度(0)
{

}

二值图显示类::~二值图显示类()
{
}

void 二值图显示类::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(二值图显示类, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// 二值图显示类 消息处理程序

void 二值图显示类::设置二值图数据(const std::vector<uint8_t>& data, int width, int height)
{
	m_二值图数据 = data;
	m_宽度 = width;
	m_高度 = height;

	// 如果对话框已经创建，强制重绘
	if (GetSafeHwnd())
	{
		Invalidate(TRUE);
	}
}

void 二值图显示类::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (m_二值图数据.empty() || m_宽度 <= 0 || m_高度 <= 0)
	{
		// 绘制无数据提示
		CString text = L"无二值图数据";
		CRect rect;
		GetClientRect(&rect);
		dc.DrawText(text, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		return;
	}

	// 计算缩放比例以适应窗口
	CRect clientRect;
	GetClientRect(&clientRect);

	double scaleX = static_cast<double>(clientRect.Width()) / m_宽度;
	double scaleY = static_cast<double>(clientRect.Height()) / m_高度;
	double scale = (std::min)(scaleX, scaleY);

	int scaledWidth = static_cast<int>(m_宽度 * scale);
	int scaledHeight = static_cast<int>(m_高度 * scale);

	// 居中显示
	int x = (clientRect.Width() - scaledWidth) / 2;
	int y = (clientRect.Height() - scaledHeight) / 2;

	// 创建内存位图和设备上下文
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, scaledWidth, scaledHeight);
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	// 填充背景
	memDC.FillSolidRect(0, 0, scaledWidth, scaledHeight, RGB(240, 240, 240));

	// 绘制二值图
	for (int row = 0; row < m_高度; row++)
	{
		for (int col = 0; col < m_宽度; col++)
		{
			int index = row * m_宽度 + col;
			if (index < m_二值图数据.size())
			{
				uint8_t value = m_二值图数据[index];
				COLORREF color = (value > 0) ? RGB(0, 0, 0) : RGB(255, 255, 255);

				int scaledCol = static_cast<int>(col * scale);
				int scaledRow = static_cast<int>(row * scale);
				int nextScaledCol = static_cast<int>((col + 1) * scale);
				int nextScaledRow = static_cast<int>((row + 1) * scale);

				memDC.FillSolidRect(scaledCol, scaledRow, 
								   nextScaledCol - scaledCol, 
								   nextScaledRow - scaledRow, 
								   color);
			}
		}
	}

	// 将内存位图绘制到屏幕
	dc.BitBlt(x, y, scaledWidth, scaledHeight, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
}