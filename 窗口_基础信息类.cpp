#include "pch.h"
#include "海鱼.h"
#include "afxdialogex.h"
#include "窗口_基础信息类.h"
#include "二值图显示类.h"
#include <afxcmn.h>
#include <string>
#include <vector>
#include <variant>
#include <sstream>

import 主信息定义模块;
import 世界树模块;
import 世界树环境模块;
import 数据仓库模块;

namespace {
	using namespace 数据仓库模块;

	template<class Node, class Fn>
	void 遍历同层(Node* first, Fn&& fn)
	{
		if (!first) return;
		auto* cur = first;
		do {
			fn(cur);
			cur = static_cast<Node*>(cur->下);
		} while (cur && cur != first);
	}

	static bool 计算平方边长(std::size_t n, std::size_t& outSide)
	{
		if (n == 0) { outSide = 0; return false; }
		auto s = (std::size_t)std::sqrt((long double)n);
		while (s * s < n) ++s;
		outSide = s;
		return (s * s == n);
	}

	static std::vector<std::uint8_t> 解码二维块图(const std::vector<std::uint64_t>& blocks, int& outW, int& outH)
	{
		outW = outH = 0;
		std::size_t side = 0;
		if (!计算平方边长(blocks.size(), side) || side == 0) return {};

		const int w = (int)(side * 8);
		const int h = (int)(side * 8);
		std::vector<std::uint8_t> img((std::size_t)w * (std::size_t)h, 0);

		for (std::size_t by = 0; by < side; ++by) {
			for (std::size_t bx = 0; bx < side; ++bx) {
				const std::uint64_t mask = blocks[by * side + bx];
				if (mask == 0ull) continue;
				for (int ly = 0; ly < 8; ++ly) {
					for (int lx = 0; lx < 8; ++lx) {
						const int bit = ly * 8 + lx;
						const bool on = ((mask >> bit) & 1ull) != 0ull;
						img[(by * 8 + (std::size_t)ly) * (std::size_t)w + (bx * 8 + (std::size_t)lx)] = on ? 255 : 0;
					}
				}
			}
		}

		outW = w;
		outH = h;
		return img;
	}

	template <class TDlg>
	static void 兼容设置二值图数据(TDlg& dlg, const std::vector<std::uint8_t>& data, int w, int h)
	{
		if constexpr (requires(TDlg d) { d.设置二值图数据(data, w, h); }) {
			dlg.设置二值图数据(data, w, h);
		}
		else if constexpr (requires(TDlg d) { d.SetBinaryImage(data, w, h); }) {
			dlg.SetBinaryImage(data, w, h);
		}
		else if constexpr (requires(TDlg d) { d.SetImage(data, w, h); }) {
			dlg.SetImage(data, w, h);
		}
		else if constexpr (requires(TDlg d) { d.设置数据(data, w, h); }) {
			dlg.设置数据(data, w, h);
		}
		else {
			AfxMessageBox(L"二值图显示类未提供可识别的设置数据接口（请补充 设置二值图数据/SetBinaryImage/SetImage/设置数据）。");
		}
	}

	static CString Utf8ToCString(const std::string& s)
	{
		if (s.empty()) return CString();
		int lenW = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), nullptr, 0);
		if (lenW <= 0) return CString();
		CString out;
		wchar_t* buf = out.GetBuffer(lenW);
		MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), buf, lenW);
		out.ReleaseBuffer(lenW);
		return out;
	}

	static CString 主键转CString(const std::string& k)
	{
		return Utf8ToCString(k);
	}

	static std::string 取词(const 词性节点类* t)
	{
		if (!t || !t->父 || !t->父->主信息) return {};
		auto* w = dynamic_cast<词主信息类*>(t->父->主信息);
		return w ? w->词 : std::string{};
	}

	static CString 格式化快照(const 特征快照值& v)
	{
		if (std::holds_alternative<std::monostate>(v)) return L"(空)";
		if (auto* p = std::get_if<I64>(&v)) {
			CString s; s.Format(L"%lld", (long long)*p); return s;
		}
		if (auto* p = std::get_if<VecU句柄>(&v)) {
			CString s; s.Format(L"VecU@0x%llX", (unsigned long long)p->主信息指针); return s;
		}
		if (auto* p = std::get_if<指针句柄>(&v)) {
			CString s; s.Format(L"Ptr@0x%llX", (unsigned long long)p->指针); return s;
		}
		return L"(未知)";
	}

	static CString 格式化基础节点标题(基础信息节点类* node)
	{
		if (!node) return L"(空节点)";
		CString key = 主键转CString(node->获取主键());
		if (!node->主信息) {
			CString s; s.Format(L"节点[%s]", (LPCTSTR)key); return s;
		}

		if (auto* smi = dynamic_cast<场景节点主信息类*>(node->主信息)) {
			CString name = Utf8ToCString(取词(smi->名称));
			CString type = Utf8ToCString(取词(smi->类型));
			CString s; s.Format(L"场景 %s %s", (LPCTSTR)name, (LPCTSTR)type); return s;
		}
		if (auto* emi = dynamic_cast<存在节点主信息类*>(node->主信息)) {
			CString name = Utf8ToCString(取词(emi->名称));
			CString type = Utf8ToCString(取词(emi->类型));
			CString s; s.Format(L"存在 %s %s 命中=%u", (LPCTSTR)name, (LPCTSTR)type, (unsigned)emi->命中次数); return s;
		}
		if (auto* fmi = dynamic_cast<特征节点主信息类*>(node->主信息)) {
			CString type = Utf8ToCString(取词(fmi->类型));
			CString value = 格式化快照(fmi->当前快照);
			CString s; s.Format(L"特征 %s 当前=%s", (LPCTSTR)type, (LPCTSTR)value); return s;
		}

		CString s; s.Format(L"节点[%s]", (LPCTSTR)key); return s;
	}

	static int 统计直接结果数(方法节点类* 条件节点)
	{
		int n = 0;
		if (!条件节点 || !条件节点->子) return 0;
		遍历同层(static_cast<方法节点类*>(条件节点->子), [&](方法节点类* ch) {
			if (ch && dynamic_cast<方法结果节点主信息类*>(ch->主信息)) ++n;
		});
		return n;
	}

	static CString 格式化方法节点标题(方法节点类* node)
	{
		if (!node || !node->主信息) return L"(空方法节点)";
		auto* mi = dynamic_cast<方法信息基类*>(node->主信息);
		if (!mi) return L"(未知方法节点)";

		if (auto* hmi = dynamic_cast<方法首节点主信息类*>(mi)) {
			CString name = Utf8ToCString(取词(hmi->动作名));
						int 条件数 = 0;
			int 结果数 = 0;
			if (node->子) {
				遍历同层(static_cast<方法节点类*>(node->子), [&](方法节点类* ch) {
					if (dynamic_cast<方法条件节点主信息类*>(ch->主信息)) ++条件数;
					else if (dynamic_cast<方法结果节点主信息类*>(ch->主信息)) ++结果数;
				});
			}
			CString s; s.Format(L"方法 %s 条件=%d 结果=%d", (LPCTSTR)name, 条件数, 结果数);
			return s;
		}
		if (auto* cmi = dynamic_cast<方法条件节点主信息类*>(mi)) {
			int resultCount = 统计直接结果数(node);
			CString s; s.Format(L"条件节点 条件索引=%u 结果=%d", (unsigned)cmi->条件判定索引.size(), resultCount);
			return s;
		}
		if (auto* rmi = dynamic_cast<方法结果节点主信息类*>(mi)) {
			CString s; s.Format(L"结果节点 摘要=%u", (unsigned)rmi->结果摘要索引.size());
			return s;
		}
		return L"方法节点";
	}

	static bool 方法场景根有内容(const 方法信息基类* mi)
	{
		return mi && mi->场景根节点.子 != nullptr;
	}
}

IMPLEMENT_DYNAMIC(窗口_基础信息类, CDialogEx)

窗口_基础信息类::窗口_基础信息类(CWnd* pParent)
	: CDialogEx(IDD_DIALOG3, pParent)
{
}

窗口_基础信息类::~窗口_基础信息类()
{
}

void 窗口_基础信息类::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(窗口_基础信息类, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &窗口_基础信息类::OnTvnSelchangedTree1)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE1, &窗口_基础信息类::OnTvnItemexpandingTree1)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &窗口_基础信息类::OnNMDblclkTree1)
	ON_BN_CLICKED(IDC_BUTTON1, &窗口_基础信息类::OnBnClickedButton1)
END_MESSAGE_MAP()

void 窗口_基础信息类::OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	(void)pNMHDR;
	*pResult = 0;
}

void 窗口_基础信息类::OnTvnItemexpandingTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNM = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if (pNM && (pNM->action == TVE_EXPAND)) {
		展开加载(pNM->itemNew.hItem);
	}
	*pResult = 0;
}

BOOL 窗口_基础信息类::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_tree = (CTreeCtrl*)GetDlgItem(IDC_TREE1);
	if (m_tree) {
		m_tree->ModifyStyle(0, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT);
	}
	刷新世界树();
	return TRUE;
}

HTREEITEM 窗口_基础信息类::插入树项(HTREEITEM hParent, const CString& 文本, 树项类型 类型, void* 指针, bool 可展开)
{
	if (!m_tree) return nullptr;
	HTREEITEM item = m_tree->InsertItem(文本, hParent, TVI_LAST);
	if (类型 != 树项类型::无 || 指针 != nullptr) {
		auto payload = std::make_unique<树项负载>();
		payload->类型 = 类型;
		payload->指针 = 指针;
		m_tree->SetItemData(item, reinterpret_cast<DWORD_PTR>(payload.get()));
		树项负载池_.push_back(std::move(payload));
	}
	else {
		m_tree->SetItemData(item, 0);
	}
	if (可展开) 添加占位子项(item);
	return item;
}

窗口_基础信息类::树项负载* 窗口_基础信息类::取树项负载(HTREEITEM hItem) const
{
	if (!m_tree || !hItem) return nullptr;
	auto raw = m_tree->GetItemData(hItem);
	if (raw == 0 || raw == kPlaceholderData) return nullptr;
	return reinterpret_cast<树项负载*>(raw);
}

void 窗口_基础信息类::清理树项负载()
{
	树项负载池_.clear();
}

void 窗口_基础信息类::添加占位子项(HTREEITEM hParent)
{
	if (!m_tree || !hParent) return;
	HTREEITEM h = m_tree->InsertItem(L"(展开以加载)", hParent);
	m_tree->SetItemData(h, kPlaceholderData);
}

bool 窗口_基础信息类::需要展开加载(HTREEITEM hItem) const
{
	if (!m_tree || !hItem) return false;
	HTREEITEM child = m_tree->GetChildItem(hItem);
	if (!child) return false;
	if (m_tree->GetNextSiblingItem(child) != nullptr) return false;
	return m_tree->GetItemData(child) == kPlaceholderData;
}

void 窗口_基础信息类::填充根节点()
{
	if (!m_tree) return;

	HTREEITEM worldRoot = m_tree->InsertItem(L"世界树", TVI_ROOT, TVI_LAST);
	m_tree->SetItemData(worldRoot, 0);
	auto addWorld = [&](const wchar_t* label, 场景节点类* scene) {
		if (!scene) return;
		CString title = 格式化基础节点标题(scene);
		CString txt; txt.Format(L"%s: %s", label, (LPCTSTR)title);
		插入树项(worldRoot, txt, 树项类型::基础节点, scene, scene->子 != nullptr);
	};
	addWorld(L"现实世界", 世界树.现实世界);
	addWorld(L"文本世界", 世界树.文本世界);
	addWorld(L"想象世界", 世界树.想象世界);
	addWorld(L"记忆世界", 世界树.记忆世界);
	addWorld(L"推理世界", 世界树.推理世界);
	addWorld(L"虚拟世界", 世界树.虚拟世界);
	m_tree->Expand(worldRoot, TVE_EXPAND);

	HTREEITEM methodRoot = m_tree->InsertItem(L"方法树", TVI_ROOT, TVI_LAST);
	m_tree->SetItemData(methodRoot, 0);
	if (方法链.根指针 && 方法链.根指针->子) {
		遍历同层(static_cast<方法节点类*>(方法链.根指针->子), [&](方法节点类* head) {
			CString txt = 格式化方法节点标题(head);
			bool expandable = false;
			if (auto* mi = dynamic_cast<方法信息基类*>(head->主信息)) {
				expandable = 方法场景根有内容(mi) || head->子 != nullptr;
			}
			插入树项(methodRoot, txt, 树项类型::方法节点, head, expandable);
		});
	}
	m_tree->Expand(methodRoot, TVE_EXPAND);
}

void 窗口_基础信息类::刷新世界树()
{
	if (!m_tree) return;
	m_tree->DeleteAllItems();
	清理树项负载();
	填充根节点();
}

void 窗口_基础信息类::添加基础节点子项(HTREEITEM hParent, void* nodePtr)
{
	auto* node = reinterpret_cast<基础信息节点类*>(nodePtr);
	if (!m_tree || !node || !node->主信息) return;

	if (auto* fmi = dynamic_cast<特征节点主信息类*>(node->主信息)) {
		CString current; current.Format(L"当前值: %s", (LPCTSTR)格式化快照(fmi->当前快照));
		插入树项(hParent, current, 树项类型::无, nullptr, false);
		if (fmi->区间摘要 && fmi->区间摘要->有效() && fmi->区间摘要->维度.size() == 1) {
			const auto& d = fmi->区间摘要->维度[0];
			CString range; range.Format(L"范围: [%lld, %lld]", (long long)d.低值, (long long)d.高值);
			插入树项(hParent, range, 树项类型::无, nullptr, false);
		}
	}

	if (!node->子) return;
	遍历同层(static_cast<基础信息节点类*>(node->子), [&](基础信息节点类* ch) {
		CString txt = 格式化基础节点标题(ch);
		插入树项(hParent, txt, 树项类型::基础节点, ch, ch->子 != nullptr);
	});
}

void 窗口_基础信息类::添加方法配对子项(HTREEITEM hParent, void* nodePtr)
{
	auto* 条件节点 = reinterpret_cast<方法节点类*>(nodePtr);
	if (!m_tree || !条件节点 || !条件节点->主信息) return;
	{
		CString condTitle = 格式化方法节点标题(条件节点);
		插入树项(hParent, condTitle, 树项类型::方法节点, 条件节点, true);
	}
	if (!条件节点->子) return;
	遍历同层(static_cast<方法节点类*>(条件节点->子), [&](方法节点类* ch) {
		if (!dynamic_cast<方法结果节点主信息类*>(ch->主信息)) return;
		CString txt = 格式化方法节点标题(ch);
		插入树项(hParent, txt, 树项类型::方法节点, ch, true);
	});
}

void 窗口_基础信息类::添加方法节点子项(HTREEITEM hParent, void* nodePtr)
{
	auto* node = reinterpret_cast<方法节点类*>(nodePtr);
	if (!m_tree || !node || !node->主信息) return;
	auto* mi = dynamic_cast<方法信息基类*>(node->主信息);
	if (!mi) return;

	if (auto* hmi = dynamic_cast<方法首节点主信息类*>(mi)) {
		CString action; action.Format(L"动作: %s", (LPCTSTR)Utf8ToCString(取词(hmi->动作名)));
		插入树项(hParent, action, 树项类型::无, nullptr, false);
		CString impact; impact.Format(L"影响签名数: %u", (unsigned)hmi->影响度量签名.size());
		插入树项(hParent, impact, 树项类型::无, nullptr, false);
		if (方法场景根有内容(mi)) {
			插入树项(hParent, L"条件模板", 树项类型::基础节点, &mi->场景根节点, true);
		}
		int condIndex = 0;
		int looseResultIndex = 0;
		if (node->子) {
			遍历同层(static_cast<方法节点类*>(node->子), [&](方法节点类* ch) {
				if (dynamic_cast<方法条件节点主信息类*>(ch->主信息)) {
					++condIndex;
					CString pairLabel;
					pairLabel.Format(L"条件-结果对 %d", condIndex);
					插入树项(hParent, pairLabel, 树项类型::方法配对, ch, true);
				}
				else if (dynamic_cast<方法结果节点主信息类*>(ch->主信息)) {
					++looseResultIndex;
					CString label;
					label.Format(L"默认结果 %d", looseResultIndex);
					HTREEITEM group = 插入树项(hParent, label, 树项类型::无, nullptr, false);
					if (group) {
						CString txt = 格式化方法节点标题(ch);
						插入树项(group, txt, 树项类型::方法节点, ch, true);
					}
				}
			});
		}
		return;
	}

	if (auto* cmi = dynamic_cast<方法条件节点主信息类*>(mi)) {
		CString info; info.Format(L"条件索引数: %u", (unsigned)cmi->条件判定索引.size());
		插入树项(hParent, info, 树项类型::无, nullptr, false);
		if (!cmi->条件场景模板主键.empty()) {
			CString key; key.Format(L"条件键: %s", (LPCTSTR)Utf8ToCString(cmi->条件场景模板主键));
			插入树项(hParent, key, 树项类型::无, nullptr, false);
		}
		if (方法场景根有内容(mi)) {
			插入树项(hParent, L"条件场景", 树项类型::基础节点, &mi->场景根节点, true);
		}
		return;
	}

	if (auto* rmi = dynamic_cast<方法结果节点主信息类*>(mi)) {
		CString info; info.Format(L"结果摘要项: %u", (unsigned)rmi->结果摘要索引.size());
		插入树项(hParent, info, 树项类型::无, nullptr, false);
		if (方法场景根有内容(mi)) {
			插入树项(hParent, L"结果场景", 树项类型::基础节点, &mi->场景根节点, true);
		}
		for (const auto& sec : rmi->结果摘要索引) {
			auto* smi = sec.主信息 ? dynamic_cast<二次特征主信息类*>(sec.主信息) : nullptr;
			if (!smi) continue;
			CString line;
			line.Format(L"摘要: %s = %lld", (LPCTSTR)Utf8ToCString(smi->度量签名_链键), (long long)smi->标量值);
			插入树项(hParent, line, 树项类型::无, nullptr, false);
		}
	}
}

void 窗口_基础信息类::添加节点子项(HTREEITEM hParent, void* nodePtr)
{
	auto* payload = 取树项负载(hParent);
	if (!payload) return;
	switch (payload->类型) {
	case 树项类型::基础节点:
		添加基础节点子项(hParent, nodePtr);
		break;
	case 树项类型::方法节点:
		添加方法节点子项(hParent, nodePtr);
		break;
	case 树项类型::方法配对:
		添加方法配对子项(hParent, nodePtr);
		break;
	default:
		break;
	}
}

void 窗口_基础信息类::展开加载(HTREEITEM hItem)
{
	if (!m_tree || !hItem || !需要展开加载(hItem)) return;
	HTREEITEM ph = m_tree->GetChildItem(hItem);
	if (ph && m_tree->GetItemData(ph) == kPlaceholderData) {
		m_tree->DeleteItem(ph);
	}
	auto* payload = 取树项负载(hItem);
	if (!payload || !payload->指针) return;
	添加节点子项(hItem, payload->指针);
}

void 窗口_基础信息类::OnBnClickedButton1()
{
	刷新世界树();
}

void 窗口_基础信息类::OnNMDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	CPoint point;
	GetCursorPos(&point);
	m_tree->ScreenToClient(&point);
	UINT flags = 0;
	HTREEITEM hItem = m_tree->HitTest(point, &flags);
	if (hItem && (flags & TVHT_ONITEMLABEL)) {
		if (是轮廓节点(hItem)) {
			显示轮廓图片(hItem);
		}
		else {
			显示当前值的二值图(hItem);
		}
	}
	*pResult = 0;
}

bool 窗口_基础信息类::是轮廓节点(HTREEITEM hItem) const
{
	auto* payload = 取树项负载(hItem);
	if (!payload || payload->类型 != 树项类型::基础节点 || !payload->指针) return false;
	auto* node = reinterpret_cast<基础信息节点类*>(payload->指针);
	auto* fmi = node && node->主信息 ? dynamic_cast<特征节点主信息类*>(node->主信息) : nullptr;
	if (!fmi) return false;
	std::string type = 取词(fmi->类型);
	return type.find("轮廓") != std::string::npos;
}

void 窗口_基础信息类::显示当前值的二值图(HTREEITEM hItem)
{
	if (!m_tree || !hItem) return;
	CString itemText = m_tree->GetItemText(hItem);
	if (itemText.Find(L"当前值:") != 0) return;
	HTREEITEM hParent = m_tree->GetParentItem(hItem);
	if (!hParent) return;
	auto* payload = 取树项负载(hParent);
	if (!payload || payload->类型 != 树项类型::基础节点 || !payload->指针) return;
	int width = 0, height = 0;
	auto binaryData = 提取二值图数据(payload->指针, width, height);
	if (!binaryData.empty() && width > 0 && height > 0) {
		二值图显示类 dlg;
		兼容设置二值图数据(dlg, binaryData, width, height);
		dlg.DoModal();
	}
}

void 窗口_基础信息类::显示轮廓图片(HTREEITEM hItem)
{
	auto* payload = 取树项负载(hItem);
	if (!payload || payload->类型 != 树项类型::基础节点 || !payload->指针) return;
	int width = 0, height = 0;
	auto imageData = 生成轮廓复合图像(payload->指针, width, height);
	if (!imageData.empty() && width > 0 && height > 0) {
		二值图显示类 dlg;
		兼容设置二值图数据(dlg, imageData, width, height);
		dlg.DoModal();
	}
}

std::vector<uint8_t> 窗口_基础信息类::生成轮廓复合图像(void* nodePtr, int& outWidth, int& outHeight)
{
	outWidth = outHeight = 0;
	auto* node = reinterpret_cast<基础信息节点类*>(nodePtr);
	if (!node || !node->主信息) return {};
	return {};
}

std::vector<uint8_t> 窗口_基础信息类::提取二值图数据(void* nodePtr, int& outWidth, int& outHeight)
{
	outWidth = outHeight = 0;
	auto* node = reinterpret_cast<基础信息节点类*>(nodePtr);
	if (!node || !node->主信息) return {};
	return {};
}



