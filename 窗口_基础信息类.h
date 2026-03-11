#pragma once
#include "afxdialogex.h"
#include <afxcmn.h>
#include <vector>
#include <cmath>
#include <memory>

class 二值图显示类;

class 窗口_基础信息类 : public CDialogEx
{
	DECLARE_DYNAMIC(窗口_基础信息类)

public:
	窗口_基础信息类(CWnd* pParent = nullptr);
	virtual ~窗口_基础信息类();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	void 刷新世界树();

private:
	enum class 树项类型 : unsigned char {
		无 = 0,
		基础节点,
		方法节点,
		方法配对
	};

	struct 树项负载 {
		树项类型 类型 = 树项类型::无;
		void* 指针 = nullptr;
	};

	void 填充根节点();
	void 添加占位子项(HTREEITEM hParent);
	bool 需要展开加载(HTREEITEM hItem) const;
	void 展开加载(HTREEITEM hItem);
	void 添加节点子项(HTREEITEM hParent, void* node);
	HTREEITEM 插入树项(HTREEITEM hParent, const CString& 文本, 树项类型 类型, void* 指针, bool 可展开 = false);
	树项负载* 取树项负载(HTREEITEM hItem) const;
	void 清理树项负载();
	void 添加基础节点子项(HTREEITEM hParent, void* nodePtr);
	void 添加方法节点子项(HTREEITEM hParent, void* nodePtr);
	void 添加方法配对子项(HTREEITEM hParent, void* nodePtr);
	static inline CTreeCtrl* m_tree = nullptr;
	static constexpr DWORD_PTR kPlaceholderData = (DWORD_PTR)0x1;
	std::vector<std::unique_ptr<树项负载>> 树项负载池_{};

	void 显示当前值的二值图(HTREEITEM hItem);
	std::vector<uint8_t> 提取二值图数据(void* nodePtr, int& outWidth, int& outHeight);
	void 显示轮廓图片(HTREEITEM hItem);
	std::vector<uint8_t> 生成轮廓复合图像(void* nodePtr, int& outWidth, int& outHeight);
	bool 是轮廓节点(HTREEITEM hItem) const;

public:
	afx_msg void OnTvnItemexpandingTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
};
