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
#include <algorithm>
#include <unordered_set>

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

	template<class Node>
	std::size_t 计数同层节点(Node* first)
	{
		std::size_t count = 0;
		遍历同层(first, [&](Node*) { ++count; });
		return count;
	}

	template<class Fn>
	void 遍历根链基础节点(基础信息节点类* root, Fn&& fn)
	{
		if (!root) return;
		auto* first = static_cast<基础信息节点类*>(root->下);
		if (!first || first == root) return;
		遍历同层(first, [&](基础信息节点类* node) {
			if (!node || node == root) return;
			fn(node);
		});
	}

	template<class Fn>
	void 遍历直属基础子节点(基础信息节点类* parent, Fn&& fn)
	{
		if (!parent || !parent->子) return;
		遍历同层(static_cast<基础信息节点类*>(parent->子), [&](基础信息节点类* node) {
			if (!node) return;
			fn(node);
		});
	}

	static std::size_t 计数直属基础子节点(基础信息节点类* parent)
	{
		std::size_t count = 0;
		遍历直属基础子节点(parent, [&](基础信息节点类*) { ++count; });
		return count;
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

	static CString 截断文本(const CString& text, int maxLen = 96)
	{
		if (text.GetLength() <= maxLen) return text;
		if (maxLen <= 3) return text.Left(maxLen);
		return text.Left(maxLen - 3) + L"...";
	}

	static CString 整数文本(std::int64_t v)
	{
		CString s;
		s.Format(L"%lld", (long long)v);
		return s;
	}

	static CString 无符号文本(std::uint64_t v)
	{
		CString s;
		s.Format(L"%llu", (unsigned long long)v);
		return s;
	}

	static CString 布尔文本(bool v)
	{
		return v ? L"是" : L"否";
	}

	static CString 时间文本(std::int64_t t)
	{
		return t == 0 ? CString(L"(无)") : 整数文本(t);
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

	template<class Node>
	static CString 节点主键摘要(const Node* node, int maxLen = 96)
	{
		if (!node) return L"(空)";
		return 截断文本(主键转CString(node->获取主键()), maxLen);
	}

	static std::string 取词(const 词性节点类* t)
	{
		if (!t || !t->父 || !t->父->主信息) return {};
		auto* w = dynamic_cast<词主信息类*>(t->父->主信息);
		return w ? w->词 : std::string{};
	}

	static CString 词显示文本(const 词性节点类* t, const wchar_t* 空文本 = L"(空)")
	{
		if (!t) return 空文本;
		CString out = Utf8ToCString(取词(t));
		out.Trim();
		if (out.IsEmpty()) {
			out = 主键转CString(t->获取主键());
		}
		out.Trim();
		return out.IsEmpty() ? CString(空文本) : out;
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

	static CString 格式化区间(const I64区间& r)
	{
		if (!r.有效()) return L"(无效)";
		CString s;
		s.Format(L"[%lld, %lld]", (long long)r.低值, (long long)r.高值);
		return s;
	}

	static CString 格式化多维区间(const I64多维区间& r)
	{
		if (!r.有效()) return L"(无效)";
		CString out;
		for (std::size_t i = 0; i < r.维度.size(); ++i) {
			if (i != 0) out += L" x ";
			out += 格式化区间(r.维度[i]);
		}
		return out;
	}

	static CString 格式化向量3(const Vector3D& v)
	{
		CString s;
		s.Format(L"(%.0f, %.0f, %.0f)", v.x, v.y, v.z);
		return s;
	}

	static CString 世界类型文本(枚举_世界类型 t)
	{
		switch (t) {
		case 枚举_世界类型::现实世界: return L"现实世界";
		case 枚举_世界类型::文本世界: return L"文本世界";
		case 枚举_世界类型::想象世界: return L"想象世界";
		case 枚举_世界类型::记忆世界: return L"记忆世界";
		case 枚举_世界类型::推理世界: return L"推理世界";
		case 枚举_世界类型::内部世界: return L"内部世界";
		case 枚举_世界类型::虚拟世界: return L"虚拟世界";
		default: return L"其它";
		}
	}

	static CString 状态域文本(枚举_状态域 v)
	{
		switch (v) {
		case 枚举_状态域::内部状态: return L"内部状态";
		case 枚举_状态域::世界状态: return L"世界状态";
		default: return L"未定义";
		}
	}

	static CString 动态来源文本(枚举_动态来源类型 v)
	{
		switch (v) {
		case 枚举_动态来源类型::方法调用: return L"方法调用";
		case 枚举_动态来源类型::内部机制动作: return L"内部机制动作";
		default: return L"未定义";
		}
	}

	static CString 动作相位文本(枚举_动作事件相位 v)
	{
		switch (v) {
		case 枚举_动作事件相位::开始运行: return L"开始运行";
		case 枚举_动作事件相位::完成: return L"完成";
		case 枚举_动作事件相位::失败: return L"失败";
		default: return L"未定义";
		}
	}

	static CString 区间语义文本(枚举_区间语义 v)
	{
		switch (v) {
		case 枚举_区间语义::值域: return L"值域";
		case 枚举_区间语义::距离到原型: return L"距离到原型";
		case 枚举_区间语义::相似度阈值: return L"相似度阈值";
		default: return L"未定义";
		}
	}

	static CString 二次特征形态文本(二次特征主信息类::枚举_二次特征形态 v)
	{
		switch (v) {
		case 二次特征主信息类::枚举_二次特征形态::概念模板_schema: return L"概念模板";
		case 二次特征主信息类::枚举_二次特征形态::条件_condition: return L"条件";
		case 二次特征主信息类::枚举_二次特征形态::实例_state: return L"实例";
		default: return L"未定义";
		}
	}

	static CString 二次特征区间层级文本(二次特征主信息类::枚举_区间层级 v)
	{
		switch (v) {
		case 二次特征主信息类::枚举_区间层级::大区间: return L"大区间";
		case 二次特征主信息类::枚举_区间层级::小区间: return L"小区间";
		default: return L"未定义";
		}
	}

	static CString 二次特征复杂度文本(二次特征主信息类::枚举_度量复杂度 v)
	{
		switch (v) {
		case 二次特征主信息类::枚举_度量复杂度::原子: return L"原子";
		case 二次特征主信息类::枚举_度量复杂度::复合: return L"复合";
		case 二次特征主信息类::枚举_度量复杂度::多次: return L"多次";
		default: return L"未定义";
		}
	}

	static CString 二次特征时间窗文本(二次特征主信息类::枚举_时间窗 v)
	{
		switch (v) {
		case 二次特征主信息类::枚举_时间窗::短: return L"短";
		case 二次特征主信息类::枚举_时间窗::中: return L"中";
		case 二次特征主信息类::枚举_时间窗::长: return L"长";
		default: return L"未定义";
		}
	}

	static CString 因果锚点类型文本(枚举_因果锚点类型 v)
	{
		switch (v) {
		case 枚举_因果锚点类型::场景: return L"场景";
		case 枚举_因果锚点类型::存在: return L"存在";
		default: return L"未定义";
		}
	}

	static CString 因果来源文本(枚举_因果来源类型 v)
	{
		switch (v) {
		case 枚举_因果来源类型::观察生成: return L"观察生成";
		case 枚举_因果来源类型::执行生成: return L"执行生成";
		case 枚举_因果来源类型::推断生成: return L"推断生成";
		default: return L"未定义";
		}
	}

	static CString 方法来源文本(枚举_方法沉淀来源 v)
	{
		switch (v) {
		case 枚举_方法沉淀来源::本能: return L"本能";
		case 枚举_方法沉淀来源::反推: return L"反推";
		case 枚举_方法沉淀来源::组合: return L"组合";
		case 枚举_方法沉淀来源::外部导入: return L"外部导入";
		case 枚举_方法沉淀来源::运行期临时: return L"运行期临时";
		default: return L"未定义";
		}
	}

	static CString 动作句柄文本(const 结构体_动作句柄& v)
	{
		switch (v.类型) {
		case 枚举_动作句柄类型::本能函数ID: {
			CString s;
			s.Format(L"本能函数ID=%lld", (long long)v.本能ID);
			return s;
		}
		case 枚举_动作句柄类型::外部实现主键: {
			CString s(L"外部实现=");
			s += 截断文本(Utf8ToCString(v.外部实现主键), 72);
			return s;
		}
		case 枚举_动作句柄类型::动作序列主键: {
			CString s(L"动作序列=");
			s += 截断文本(Utf8ToCString(v.动作序列主键), 72);
			return s;
		}
		default:
			return L"未设置";
		}
	}

	static CString 方法节点种类文本(枚举_方法节点种类 v)
	{
		switch (v) {
		case 枚举_方法节点种类::方法首节点: return L"方法首节点";
		case 枚举_方法节点种类::方法条件节点: return L"方法条件节点";
		case 枚举_方法节点种类::方法结果节点: return L"方法结果节点";
		default: return L"未定义";
		}
	}

	static CString 节点类型文本(const 基础信息基类* mi)
	{
		if (!mi) return L"空节点";
		if (dynamic_cast<const 场景节点主信息类*>(mi)) return L"场景";
		if (dynamic_cast<const 存在节点主信息类*>(mi)) return L"存在";
		if (dynamic_cast<const 特征节点主信息类*>(mi)) return L"特征";
		if (dynamic_cast<const 状态节点主信息类*>(mi)) return L"状态";
		if (dynamic_cast<const 动态节点主信息类*>(mi)) return L"动态";
		if (dynamic_cast<const 二次特征主信息类*>(mi)) return L"二次特征";
		if (dynamic_cast<const 因果模板主信息类*>(mi)) return L"因果模板";
		if (dynamic_cast<const 因果链主信息类*>(mi)) return L"因果链";
		if (dynamic_cast<const 因果实例主信息类*>(mi)) return L"因果实例";
		if (dynamic_cast<const 指代节点主信息类*>(mi)) return L"指代";
		return L"基础信息";
	}

	static std::uint64_t 取命中次数(const 基础信息基类* mi)
	{
		if (!mi) return 0;
		if (auto* smi = dynamic_cast<const 场景节点主信息类*>(mi)) return smi->命中次数;
		if (auto* fmi = dynamic_cast<const 特征节点主信息类*>(mi)) return fmi->命中次数;
		return mi->命中次数;
	}

	static CString 取名称类型摘要(const 基础信息基类* mi)
	{
		if (!mi) return L"<空>";
		const bool hasName = mi->名称 != nullptr;
		const bool hasType = mi->类型 != nullptr;
		if (hasName && hasType) return 词显示文本(mi->名称) + L" / " + 词显示文本(mi->类型);
		if (hasName) return 词显示文本(mi->名称);
		if (hasType) return 词显示文本(mi->类型);
		return L"<未命名>";
	}

	static CString 取状态特征摘要(const 状态节点主信息类* smi)
	{
		if (!smi || !smi->状态特征 || !smi->状态特征->主信息) return L"<空特征>";
		if (auto* fmi = dynamic_cast<特征节点主信息类*>(smi->状态特征->主信息)) {
			return 词显示文本(fmi->类型, L"<空特征>");
		}
		return 节点主键摘要(smi->状态特征, 48);
	}

	static const 状态节点主信息类* 取状态主信息(const 状态节点类* node)
	{
		if (!node || !node->主信息) return nullptr;
		return dynamic_cast<状态节点主信息类*>(node->主信息);
	}

	static CString 格式化状态主信息摘要(const 状态节点主信息类* smi, int maxLen = 96)
	{
		if (!smi) return L"<空状态>";
		CString s;
		s.Format(L"%s=%s | 主体=%s | 域=%s",
			(LPCTSTR)截断文本(取状态特征摘要(smi), 28),
			(LPCTSTR)截断文本(格式化快照(smi->状态值), 24),
			(LPCTSTR)节点主键摘要(smi->状态主体, 24),
			(LPCTSTR)状态域文本(smi->状态域));
		return 截断文本(s, maxLen);
	}

	static std::size_t 取因果条件状态数(const 因果实例主信息类* cmi)
	{
		if (!cmi) return 0;
		return !cmi->条件状态主信息列表.empty()
			? cmi->条件状态主信息列表.size()
			: cmi->条件状态列表.size();
	}

	static std::size_t 取因果结果状态数(const 因果实例主信息类* cmi)
	{
		if (!cmi) return 0;
		return !cmi->结果状态主信息列表.empty()
			? cmi->结果状态主信息列表.size()
			: cmi->结果状态列表.size();
	}

	template<class Fn>
	void 遍历因果条件状态主信息(const 因果实例主信息类* cmi, Fn&& fn)
	{
		if (!cmi) return;
		if (!cmi->条件状态主信息列表.empty()) {
			for (const auto& 状态 : cmi->条件状态主信息列表) {
				fn(&状态);
			}
			return;
		}
		for (auto* 状态节点 : cmi->条件状态列表) {
			if (auto* 状态主信息 = 取状态主信息(状态节点)) {
				fn(状态主信息);
			}
		}
	}

	template<class Fn>
	void 遍历因果结果状态主信息(const 因果实例主信息类* cmi, Fn&& fn)
	{
		if (!cmi) return;
		if (!cmi->结果状态主信息列表.empty()) {
			for (const auto& 状态 : cmi->结果状态主信息列表) {
				fn(&状态);
			}
			return;
		}
		for (auto* 状态节点 : cmi->结果状态列表) {
			if (auto* 状态主信息 = 取状态主信息(状态节点)) {
				fn(状态主信息);
			}
		}
	}

	static const 状态节点主信息类* 取因果主结果状态主信息(const 因果实例主信息类* cmi)
	{
		if (!cmi) return nullptr;
		if (!cmi->主结果状态主信息列表.empty()) return &cmi->主结果状态主信息列表.back();
		if (!cmi->结果状态主信息列表.empty()) return &cmi->结果状态主信息列表.back();
		if (auto* 主结果状态主信息 = 取状态主信息(cmi->主结果状态)) return 主结果状态主信息;
		if (!cmi->结果状态列表.empty()) {
			return 取状态主信息(cmi->结果状态列表.back());
		}
		return nullptr;
	}

	static CString 取动态动作摘要(const 动态节点主信息类* dmi)
	{
		if (!dmi) return L"<无动作>";
		if (dmi->来源动作名) {
			CString action = 词显示文本(dmi->来源动作名);
			const CString phase = 动作相位文本(dmi->来源动作相位);
			return phase == L"未定义" ? action : (action + L" / " + phase);
		}
		if (dmi->来源方法首节点 && dmi->来源方法首节点->主信息) {
			if (auto* hmi = dynamic_cast<方法首节点主信息类*>(dmi->来源方法首节点->主信息)) {
				CString action = hmi->动作名 ? 词显示文本(hmi->动作名) : 节点主键摘要(dmi->来源方法首节点, 48);
				const CString phase = 动作相位文本(dmi->来源动作相位);
				return phase == L"未定义" ? action : (action + L" / " + phase);
			}
		}
		if (dmi->动态特征 && dmi->动态特征->主信息) {
			if (auto* fmi = dynamic_cast<特征节点主信息类*>(dmi->动态特征->主信息)) {
				return 词显示文本(fmi->类型, L"<空动作>");
			}
		}
		return L"<无动作>";
	}

	static CString 取因果动作摘要(const 因果实例主信息类* cmi)
	{
		if (!cmi) return L"<无动作>";
		if (auto* compat = dynamic_cast<const 因果兼容主信息类*>(cmi)) {
			if (!compat->动作语义键.empty()) {
				return 截断文本(Utf8ToCString(compat->动作语义键), 72);
			}
		}
		if (cmi->动作动态 && cmi->动作动态->主信息) {
			return 取动态动作摘要(dynamic_cast<动态节点主信息类*>(cmi->动作动态->主信息));
		}
		if (cmi->触发动态 && cmi->触发动态->主信息) {
			return 取动态动作摘要(dynamic_cast<动态节点主信息类*>(cmi->触发动态->主信息));
		}
		if (cmi->动作语义) {
			return 节点主键摘要(cmi->动作语义, 72);
		}
		return L"<无动作>";
	}

	static CString 取动作模板摘要(const 基础信息节点类* 节点)
	{
		if (!节点) return L"<空>";
		CString 文本;
		if (节点->主信息) {
			if (auto* 特征信息 = dynamic_cast<特征节点主信息类*>(节点->主信息)) {
				if (特征信息->类型) {
					文本 = 词显示文本(特征信息->类型, L"");
				}
			}
		}
		CString 主键 = 节点主键摘要(节点, 72);
		if (文本.IsEmpty() || 文本 == 主键) return 主键;
		return 文本 + L" {" + 主键 + L"}";
	}

	static CString 格式化基础节点标题(基础信息节点类* node)
	{
		if (!node) return L"(空节点)";
		CString key = 节点主键摘要(node, 48);
		auto* mi = node->主信息;
		if (!mi) {
			CString s;
			s.Format(L"空节点 [%s]", (LPCTSTR)key);
			return s;
		}

		if (auto* smi = dynamic_cast<场景节点主信息类*>(mi)) {
			CString s;
			s.Format(L"场景 %s | 世界=%s | 状态=%llu 动态=%llu 因果=%llu",
				(LPCTSTR)截断文本(取名称类型摘要(smi), 44),
				(LPCTSTR)世界类型文本(smi->世界类型),
				(unsigned long long)smi->状态总数(),
				(unsigned long long)(smi->动态总数() + smi->事件动态总数()),
				(unsigned long long)(smi->实例因果列表.size() + smi->因果链列表.size()));
			return s;
		}
		if (auto* emi = dynamic_cast<存在节点主信息类*>(mi)) {
			CString s;
			s.Format(L"存在 %s | 命中=%llu | 可信=%lld | 因果=%llu",
				(LPCTSTR)截断文本(取名称类型摘要(emi), 44),
				(unsigned long long)emi->命中次数,
				(long long)emi->可信度,
				(unsigned long long)(emi->实例因果列表.size() + emi->因果链列表.size()));
			return s;
		}
		if (auto* fmi = dynamic_cast<特征节点主信息类*>(mi)) {
			CString s;
			s.Format(L"特征 %s | 当前=%s | 稳态=%d | 命中=%llu",
				(LPCTSTR)截断文本(词显示文本(fmi->类型, L"<空特征>"), 36),
				(LPCTSTR)截断文本(格式化快照(fmi->当前快照), 32),
				fmi->当前稳态索引,
				(unsigned long long)fmi->命中次数);
			return s;
		}
		if (auto* smi = dynamic_cast<状态节点主信息类*>(mi)) {
			CString s;
			s.Format(L"状态 %s=%s | 主体=%s | 域=%s",
				(LPCTSTR)截断文本(取状态特征摘要(smi), 28),
				(LPCTSTR)截断文本(格式化快照(smi->状态值), 24),
				(LPCTSTR)节点主键摘要(smi->状态主体, 20),
				(LPCTSTR)状态域文本(smi->状态域));
			return s;
		}
		if (auto* dmi = dynamic_cast<动态节点主信息类*>(mi)) {
			CString s;
			s.Format(L"动态 %s | 开始=%lld 结束=%lld | 事件=%llu",
				(LPCTSTR)截断文本(取动态动作摘要(dmi), 36),
				(long long)dmi->开始时间,
				(long long)dmi->结束时间,
				(unsigned long long)dmi->事件列表.size());
			return s;
		}
		if (auto* bmi = dynamic_cast<二次特征主信息类*>(mi)) {
			CString chainKey = bmi->度量签名_链键.empty()
				? CString(L"<空链键>")
				: 截断文本(Utf8ToCString(bmi->度量签名_链键), 36);
			CString s;
			s.Format(L"二次特征 %s | 值=%lld | 形态=%s",
				(LPCTSTR)chainKey,
				(long long)bmi->标量值,
				(LPCTSTR)二次特征形态文本(bmi->形态));
			return s;
		}
		if (auto* cmi = dynamic_cast<因果模板主信息类*>(mi)) {
			CString s;
			s.Format(L"因果模板 | 条件=%llu 结果=%llu 证据=%llu",
				(unsigned long long)cmi->条件模板列表.size(),
				(unsigned long long)cmi->结果模板列表.size(),
				(unsigned long long)cmi->证据次数);
			return s;
		}
		if (auto* cmi = dynamic_cast<因果链主信息类*>(mi)) {
			CString s;
			s.Format(L"因果链 | 长度=%llu | 时间=%lld->%lld",
				(unsigned long long)cmi->因果实例序列.size(),
				(long long)cmi->起始时间,
				(long long)cmi->结束时间);
			return s;
		}
		if (auto* cmi = dynamic_cast<因果实例主信息类*>(mi)) {
			CString s;
			s.Format(L"因果实例 | 条件=%llu 结果=%llu | 动作=%s",
				(unsigned long long)取因果条件状态数(cmi),
				(unsigned long long)取因果结果状态数(cmi),
				(LPCTSTR)截断文本(取因果动作摘要(cmi), 32));
			return s;
		}
		if (auto* pmi = dynamic_cast<指代节点主信息类*>(mi)) {
			CString s;
			s.Format(L"指代 %s -> %s",
				(LPCTSTR)词显示文本(pmi->代词, L"<空代词>"),
				(LPCTSTR)节点主键摘要(pmi->指代对象, 36));
			return s;
		}

		CString s;
		s.Format(L"%s [%s]", (LPCTSTR)节点类型文本(mi), (LPCTSTR)key);
		return s;
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
			CString name = 词显示文本(hmi->动作名, L"<未命名方法>");
			int 条件数 = 0;
			int 结果数 = 0;
			if (node->子) {
				遍历同层(static_cast<方法节点类*>(node->子), [&](方法节点类* ch) {
					if (dynamic_cast<方法条件节点主信息类*>(ch->主信息)) ++条件数;
					else if (dynamic_cast<方法结果节点主信息类*>(ch->主信息)) ++结果数;
				});
			}
			CString s;
			s.Format(L"方法 %s | 来源=%s | 条件=%d 结果=%d 影响=%llu",
				(LPCTSTR)name,
				(LPCTSTR)方法来源文本(hmi->来源),
				条件数,
				结果数,
				(unsigned long long)hmi->影响度量签名.size());
			return s;
		}
		if (auto* cmi = dynamic_cast<方法条件节点主信息类*>(mi)) {
			int resultCount = 统计直接结果数(node);
			CString s;
			s.Format(L"条件节点 | 条件索引=%llu | 结果=%d",
				(unsigned long long)cmi->条件判定索引.size(),
				resultCount);
			return s;
		}
		if (auto* rmi = dynamic_cast<方法结果节点主信息类*>(mi)) {
			CString s;
			s.Format(L"结果节点 | 摘要=%llu",
				(unsigned long long)rmi->结果摘要索引.size());
			return s;
		}
		return L"方法节点";
	}

	static CString 语素节点类型文本(const 语素基类* mi)
	{
		if (!mi) return L"空语素";
		if (dynamic_cast<const 词主信息类*>(mi)) return L"词";
		if (dynamic_cast<const 词性主信息类*>(mi)) return L"词性";
		if (dynamic_cast<const 短语主信息类*>(mi)) return L"短语";
		if (dynamic_cast<const 短语子节点主信息类*>(mi)) return L"短语子";
		return L"语素";
	}

	static CString 格式化语素节点标题(语素节点类* node)
	{
		if (!node) return L"(空语素节点)";
		CString key = 节点主键摘要(node, 64);
		auto* mi = node->主信息;
		if (!mi) {
			CString s;
			s.Format(L"语素 [%s]", (LPCTSTR)key);
			return s;
		}

		if (auto* wmi = dynamic_cast<词主信息类*>(mi)) {
			CString word = 截断文本(Utf8ToCString(wmi->词), 48);
			if (word.IsEmpty()) word = key;
			return L"词 " + word;
		}
		if (auto* pmi = dynamic_cast<词性主信息类*>(mi)) {
			CString s;
			s.Format(L"词性 %s | 类型值=%d | 示例=%llu",
				(LPCTSTR)词显示文本(reinterpret_cast<词性节点类*>(node), L"<空词>"),
				(int)pmi->词性,
				(unsigned long long)pmi->示例列表.size());
			return s;
		}
		if (auto* phr = dynamic_cast<短语主信息类*>(mi)) {
			CString s;
			s.Format(L"短语 %s / %s",
				(LPCTSTR)词显示文本(phr->参照词, L"<空参照词>"),
				(LPCTSTR)词显示文本(phr->比较对象词, L"<空比较词>"));
			return s;
		}
		if (auto* child = dynamic_cast<短语子节点主信息类*>(mi)) {
			CString s;
			s.Format(L"短语子 %s -> %s",
				(LPCTSTR)词显示文本(child->比较词, L"<空比较词>"),
				(LPCTSTR)词显示文本(child->结果词, L"<空结果词>"));
			return s;
		}

		CString s;
		s.Format(L"%s [%s]", (LPCTSTR)语素节点类型文本(mi), (LPCTSTR)key);
		return s;
	}

	static void 收集语素节点详情(语素节点类* node, std::vector<CString>& out)
	{
		if (!node) return;
		auto addLine = [&](const wchar_t* label, const CString& value) {
			CString line(label);
			line += value;
			out.push_back(line);
		};

		auto* mi = node->主信息;
		addLine(L"主键: ", 节点主键摘要(node, 120));
		addLine(L"节点类型: ", 语素节点类型文本(mi));
		addLine(L"直接子节点数: ", 无符号文本(node->子 ? 计数同层节点(static_cast<语素节点类*>(node->子)) : 0));

		if (auto* wmi = dynamic_cast<词主信息类*>(mi)) {
			addLine(L"词: ", Utf8ToCString(wmi->词));
			return;
		}
		if (auto* pmi = dynamic_cast<词性主信息类*>(mi)) {
			addLine(L"词性值: ", 整数文本((int)pmi->词性));
			if (pmi->对应基础信息指针) addLine(L"对应基础信息: ", 节点主键摘要(pmi->对应基础信息指针, 96));
			addLine(L"示例数: ", 无符号文本((unsigned long long)pmi->示例列表.size()));
			return;
		}
		if (auto* phr = dynamic_cast<短语主信息类*>(mi)) {
			if (phr->参照词) addLine(L"参照词: ", 词显示文本(phr->参照词));
			if (phr->比较对象词) addLine(L"比较对象词: ", 词显示文本(phr->比较对象词));
			return;
		}
		if (auto* child = dynamic_cast<短语子节点主信息类*>(mi)) {
			if (child->比较词) addLine(L"比较词: ", 词显示文本(child->比较词));
			if (child->结果词) addLine(L"结果词: ", 词显示文本(child->结果词));
		}
	}

	static CString 自然语言节点类型文本(const 自然语言成分基类* mi)
	{
		if (!mi) return L"空自然语言节点";
		if (dynamic_cast<const 文章主信息类*>(mi)) return L"文章";
		if (dynamic_cast<const 段落主信息类*>(mi)) return L"段落";
		if (dynamic_cast<const 自然句主信息类*>(mi)) return L"自然句";
		if (dynamic_cast<const 子句主信息类*>(mi)) return L"子句";
		if (dynamic_cast<const 句子成分主信息类*>(mi)) return L"句子成分";
		return L"自然语言";
	}

	static CString 格式化自然语言节点标题(自然句节点类* node)
	{
		if (!node) return L"(空自然语言节点)";
		CString key = 节点主键摘要(node, 64);
		auto* mi = node->主信息;
		if (!mi) {
			CString s;
			s.Format(L"自然语言 [%s]", (LPCTSTR)key);
			return s;
		}

		if (auto* article = dynamic_cast<文章主信息类*>(mi)) {
			CString title = 截断文本(Utf8ToCString(article->标题), 48);
			if (title.IsEmpty()) title = key;
			return L"文章 " + title;
		}
		if (auto* para = dynamic_cast<段落主信息类*>(mi)) {
			CString s;
			s.Format(L"段落 #%llu | 文本=%s",
				(unsigned long long)para->在文章中的序号,
				(LPCTSTR)截断文本(Utf8ToCString(para->原始文本), 36));
			return s;
		}
		if (auto* sent = dynamic_cast<自然句主信息类*>(mi)) {
			CString s;
			s.Format(L"自然句 #%llu | 词=%llu | 文本=%s",
				(unsigned long long)sent->在段落中的序号,
				(unsigned long long)sent->词序列.size(),
				(LPCTSTR)截断文本(Utf8ToCString(sent->原始文本), 36));
			return s;
		}
		if (auto* clause = dynamic_cast<子句主信息类*>(mi)) {
			CString s;
			s.Format(L"子句 [%llu, %llu) | 结构=%d",
				(unsigned long long)clause->起始词索引,
				(unsigned long long)clause->结束词索引,
				(int)clause->原子结构类型);
			return s;
		}
		if (auto* comp = dynamic_cast<句子成分主信息类*>(mi)) {
			CString s;
			s.Format(L"句子成分 | 类型=%d | 位置=%llu",
				(int)comp->类型,
				(unsigned long long)comp->在句子中的位置);
			return s;
		}

		CString s;
		s.Format(L"%s [%s]", (LPCTSTR)自然语言节点类型文本(mi), (LPCTSTR)key);
		return s;
	}

	static void 收集自然语言节点详情(自然句节点类* node, std::vector<CString>& out)
	{
		if (!node) return;
		auto addLine = [&](const wchar_t* label, const CString& value) {
			CString line(label);
			line += value;
			out.push_back(line);
		};

		auto* mi = node->主信息;
		addLine(L"主键: ", 节点主键摘要(node, 120));
		addLine(L"节点类型: ", 自然语言节点类型文本(mi));
		addLine(L"直接子节点数: ", 无符号文本(node->子 ? 计数同层节点(static_cast<自然句节点类*>(node->子)) : 0));

		if (auto* article = dynamic_cast<文章主信息类*>(mi)) {
			addLine(L"标题: ", Utf8ToCString(article->标题));
			if (!article->作者.empty()) addLine(L"作者: ", Utf8ToCString(article->作者));
			if (!article->来源.empty()) addLine(L"来源: ", Utf8ToCString(article->来源));
			if (!article->原始文本.empty()) addLine(L"原始文本: ", 截断文本(Utf8ToCString(article->原始文本), 160));
			if (article->对应文本世界场景) addLine(L"对应文本世界场景: ", 节点主键摘要(article->对应文本世界场景, 96));
			return;
		}
		if (auto* para = dynamic_cast<段落主信息类*>(mi)) {
			addLine(L"在文章中的序号: ", 无符号文本((unsigned long long)para->在文章中的序号));
			if (!para->原始文本.empty()) addLine(L"原始文本: ", 截断文本(Utf8ToCString(para->原始文本), 160));
			return;
		}
		if (auto* sent = dynamic_cast<自然句主信息类*>(mi)) {
			addLine(L"在段落中的序号: ", 无符号文本((unsigned long long)sent->在段落中的序号));
			addLine(L"词序列数: ", 无符号文本((unsigned long long)sent->词序列.size()));
			if (!sent->原始文本.empty()) addLine(L"原始文本: ", 截断文本(Utf8ToCString(sent->原始文本), 160));
			if (sent->主语) addLine(L"主语: ", 词显示文本(sent->主语));
			if (sent->谓语) addLine(L"谓语: ", 词显示文本(sent->谓语));
			if (sent->宾语) addLine(L"宾语: ", 词显示文本(sent->宾语));
			addLine(L"子句数: ", 无符号文本((unsigned long long)sent->子句节点列表.size()));
			return;
		}
		if (auto* clause = dynamic_cast<子句主信息类*>(mi)) {
			addLine(L"原子结构类型: ", 整数文本((int)clause->原子结构类型));
			addLine(L"起始词索引: ", 无符号文本((unsigned long long)clause->起始词索引));
			addLine(L"结束词索引: ", 无符号文本((unsigned long long)clause->结束词索引));
			addLine(L"局部词数: ", 无符号文本((unsigned long long)clause->局部词序列.size()));
			if (clause->子句主语) addLine(L"子句主语: ", 词显示文本(clause->子句主语));
			if (clause->子句谓语) addLine(L"子句谓语: ", 词显示文本(clause->子句谓语));
			if (clause->子句宾语) addLine(L"子句宾语: ", 词显示文本(clause->子句宾语));
			if (clause->对应场景) addLine(L"对应场景: ", 节点主键摘要(clause->对应场景, 96));
			if (clause->对应状态) addLine(L"对应状态: ", 节点主键摘要(clause->对应状态, 96));
			if (clause->对应动态) addLine(L"对应动态: ", 节点主键摘要(clause->对应动态, 96));
			if (clause->对应因果) addLine(L"对应因果: ", 节点主键摘要(clause->对应因果, 96));
			return;
		}
		if (auto* comp = dynamic_cast<句子成分主信息类*>(mi)) {
			addLine(L"成分类型: ", 整数文本((int)comp->类型));
			addLine(L"句内位置: ", 无符号文本((unsigned long long)comp->在句子中的位置));
			if (comp->对应词性节点) addLine(L"对应词性节点: ", 词显示文本(comp->对应词性节点));
		}
	}

	static bool 方法场景根有内容(const 方法信息基类* mi)
	{
		return mi && mi->场景根节点.子 != nullptr;
	}

	enum class 子节点分组 : unsigned char {
		场景 = 0,
		存在,
		特征,
		状态,
		动态,
		二次特征,
		因果实例,
		因果模板,
		因果链,
		指代,
		其它,
		数量
	};

	static 子节点分组 判断子节点分组(const 基础信息节点类* node)
	{
		auto* mi = node ? node->主信息 : nullptr;
		if (!mi) return 子节点分组::其它;
		if (dynamic_cast<场景节点主信息类*>(mi)) return 子节点分组::场景;
		if (dynamic_cast<存在节点主信息类*>(mi)) return 子节点分组::存在;
		if (dynamic_cast<特征节点主信息类*>(mi)) return 子节点分组::特征;
		if (dynamic_cast<状态节点主信息类*>(mi)) return 子节点分组::状态;
		if (dynamic_cast<动态节点主信息类*>(mi)) return 子节点分组::动态;
		if (dynamic_cast<二次特征主信息类*>(mi)) return 子节点分组::二次特征;
		if (dynamic_cast<因果模板主信息类*>(mi)) return 子节点分组::因果模板;
		if (dynamic_cast<因果链主信息类*>(mi)) return 子节点分组::因果链;
		if (dynamic_cast<因果实例主信息类*>(mi)) return 子节点分组::因果实例;
		if (dynamic_cast<指代节点主信息类*>(mi)) return 子节点分组::指代;
		return 子节点分组::其它;
	}

	static const wchar_t* 子节点分组标题(子节点分组 g)
	{
		switch (g) {
		case 子节点分组::场景: return L"子节点 / 场景";
		case 子节点分组::存在: return L"子节点 / 存在";
		case 子节点分组::特征: return L"子节点 / 特征";
		case 子节点分组::状态: return L"子节点 / 状态";
		case 子节点分组::动态: return L"子节点 / 动态";
		case 子节点分组::二次特征: return L"子节点 / 二次特征";
		case 子节点分组::因果实例: return L"子节点 / 因果实例";
		case 子节点分组::因果模板: return L"子节点 / 因果模板";
		case 子节点分组::因果链: return L"子节点 / 因果链";
		case 子节点分组::指代: return L"子节点 / 指代";
		default: return L"子节点 / 其它";
		}
	}

	struct 节点显示分组 {
		CString 标题{};
		std::vector<基础信息节点类*> 节点{};
	};

	template<class T>
	static 基础信息节点类* 转基础节点(T* node)
	{
		return node ? static_cast<基础信息节点类*>(node) : nullptr;
	}

	template<class T>
	static 基础信息节点类* 转基础节点(const T* node)
	{
		return node ? const_cast<基础信息节点类*>(static_cast<const 基础信息节点类*>(node)) : nullptr;
	}

	template<class T>
	static void 追加唯一显示节点(
		std::vector<基础信息节点类*>& out,
		T* node,
		const std::unordered_set<基础信息节点类*>* 排除集 = nullptr)
	{
		auto* 基础节点 = 转基础节点(node);
		if (!基础节点) return;
		if (排除集 && 排除集->find(基础节点) != 排除集->end()) return;
		if (std::find(out.begin(), out.end(), 基础节点) != out.end()) return;
		out.push_back(基础节点);
	}

	template<class T>
	static std::vector<基础信息节点类*> 收集显示节点列表(
		const std::vector<T*>& src,
		const std::unordered_set<基础信息节点类*>* 排除集 = nullptr)
	{
		std::vector<基础信息节点类*> out{};
		out.reserve(src.size());
		for (auto* node : src) {
			追加唯一显示节点(out, node, 排除集);
		}
		return out;
	}

	static void 添加节点显示分组(
		std::vector<节点显示分组>& out,
		const wchar_t* 标题,
		std::vector<基础信息节点类*>&& 节点列表)
	{
		if (!标题 || 节点列表.empty()) return;
		节点显示分组 group{};
		group.标题 = 标题;
		group.节点 = std::move(节点列表);
		out.push_back(std::move(group));
	}

	static void 收集基础节点引用分组(
		基础信息节点类* node,
		const std::unordered_set<基础信息节点类*>& 直接子节点,
		std::vector<节点显示分组>& out)
	{
		if (!node || !node->主信息) return;

		auto addSingle = [&](const wchar_t* 标题, auto* refNode) {
			std::vector<基础信息节点类*> 节点列表{};
			追加唯一显示节点(节点列表, refNode, &直接子节点);
			添加节点显示分组(out, 标题, std::move(节点列表));
		};
		auto addList = [&](const wchar_t* 标题, const auto& refNodes) {
			添加节点显示分组(out, 标题, 收集显示节点列表(refNodes, &直接子节点));
		};

		if (auto* smi = dynamic_cast<场景节点主信息类*>(node->主信息)) {
			addList(L"索引内容 / 状态列表", smi->状态列表);
			addList(L"索引内容 / 动态列表", smi->动态列表);
			addList(L"索引内容 / 事件动态列表", smi->事件动态列表);
			addList(L"索引内容 / 关系列表", smi->关系列表);
			addList(L"索引内容 / 实例因果列表", smi->实例因果列表);
			addList(L"索引内容 / 因果链列表", smi->因果链列表);
			return;
		}

		if (auto* emi = dynamic_cast<存在节点主信息类*>(node->主信息)) {
			addSingle(L"引用内容 / 概念模板", emi->概念模板);
			addSingle(L"引用内容 / 内部世界", emi->内部世界);
			addList(L"索引内容 / 实例因果列表", emi->实例因果列表);
			addList(L"索引内容 / 因果链列表", emi->因果链列表);
			return;
		}

		if (auto* smi = dynamic_cast<状态节点主信息类*>(node->主信息)) {
			addSingle(L"引用内容 / 状态主体", smi->状态主体);
			addSingle(L"引用内容 / 状态特征", smi->状态特征);
			addSingle(L"引用内容 / 对应信息节点", smi->对应信息节点);
			return;
		}

		if (auto* dmi = dynamic_cast<动态节点主信息类*>(node->主信息)) {
			addSingle(L"引用内容 / 动态主体", dmi->动态主体);
			addSingle(L"引用内容 / 动态特征", dmi->动态特征);
			addSingle(L"引用内容 / 初始状态", dmi->初始状态);
			addSingle(L"引用内容 / 结果状态", dmi->结果状态);
			addSingle(L"引用内容 / 来源输入场景", dmi->来源输入场景);
			addSingle(L"引用内容 / 来源输出场景", dmi->来源输出场景);
			return;
		}

		if (auto* bmi = dynamic_cast<二次特征主信息类*>(node->主信息)) {
			addSingle(L"引用内容 / 概念模板", bmi->概念模板);
			addSingle(L"引用内容 / 所属场景", bmi->所属场景);
			addSingle(L"引用内容 / 主体", bmi->主体);
			addSingle(L"引用内容 / 客体", bmi->客体);
			addSingle(L"引用内容 / 左对象", bmi->左对象);
			addSingle(L"引用内容 / 右对象", bmi->右对象);
			return;
		}

		if (auto* cmi = dynamic_cast<因果模板主信息类*>(node->主信息)) {
			addList(L"引用内容 / 条件模板列表", cmi->条件模板列表);
			addSingle(L"引用内容 / 动作模板", cmi->动作模板);
			addList(L"引用内容 / 结果模板列表", cmi->结果模板列表);
			addList(L"引用内容 / 证据实例列表", cmi->证据实例列表);
			addList(L"引用内容 / 代表证据列表", cmi->代表证据列表);
			addList(L"引用内容 / 反例实例列表", cmi->反例实例列表);
			return;
		}

		if (auto* cmi = dynamic_cast<因果链主信息类*>(node->主信息)) {
			addList(L"引用内容 / 因果实例序列", cmi->因果实例序列);
			return;
		}

		if (auto* cmi = dynamic_cast<因果实例主信息类*>(node->主信息)) {
			addSingle(L"引用内容 / 锚点信息", cmi->锚点信息);
			addSingle(L"引用内容 / 动作主体", cmi->动作主体);
			addSingle(L"引用内容 / 动作语义", cmi->动作语义);
			addSingle(L"引用内容 / 动作信息", cmi->动作信息);
			addSingle(L"引用内容 / 动作动态", cmi->动作动态);
			addSingle(L"引用内容 / 触发动态", cmi->触发动态);
			if (cmi->条件状态主信息列表.empty()) {
				addList(L"引用内容 / 条件状态列表", cmi->条件状态列表);
			}
			if (cmi->结果状态主信息列表.empty()) {
				addList(L"引用内容 / 结果状态列表", cmi->结果状态列表);
			}
			if (cmi->主结果状态主信息列表.empty()) {
				addSingle(L"引用内容 / 主结果状态", cmi->主结果状态);
			}
			addList(L"引用内容 / 结果动态列表", cmi->结果动态列表);
			addSingle(L"引用内容 / 主结果动态", cmi->主结果动态);

			if (auto* compat = dynamic_cast<因果兼容主信息类*>(cmi)) {
				addSingle(L"兼容内容 / 归纳因果节点", compat->归纳因果节点);
				addSingle(L"兼容内容 / 因信息", compat->因信息);
				addSingle(L"兼容内容 / 果信息", compat->果信息);
				addSingle(L"兼容内容 / 锚点动态", compat->锚点动态);
				addList(L"兼容内容 / 条件方向二次特征列表", compat->条件方向二次特征列表);
				addList(L"兼容内容 / 过程动态列表", compat->过程动态列表);
				addSingle(L"兼容内容 / 结果状态", compat->结果状态);
				addSingle(L"兼容内容 / 结果方向二次特征", compat->结果方向二次特征);
				addList(L"兼容内容 / 事件分段列表", compat->事件分段列表);
				addSingle(L"兼容内容 / 因果主体", compat->因果主体);
			}
			return;
		}

		if (auto* pmi = dynamic_cast<指代节点主信息类*>(node->主信息)) {
			addSingle(L"引用内容 / 指代对象", pmi->指代对象);
		}
	}

	static bool 基础节点有可显示内容(const 基础信息节点类* node)
	{
		return node && (node->主信息 != nullptr || node->子 != nullptr);
	}

	static bool 方法节点有可显示内容(const 方法节点类* node)
	{
		return node && (node->主信息 != nullptr || node->子 != nullptr);
	}

	static bool 语素节点有可显示内容(const 语素节点类* node)
	{
		return node && (node->主信息 != nullptr || node->子 != nullptr);
	}

	static bool 自然语言节点有可显示内容(const 自然句节点类* node)
	{
		return node && (node->主信息 != nullptr || node->子 != nullptr);
	}

	static void 收集基础节点详情(基础信息节点类* node, std::vector<CString>& out)
	{
		if (!node) return;
		auto addLine = [&](const wchar_t* label, const CString& value) {
			CString line(label);
			line += value;
			out.push_back(line);
		};

		auto* mi = node->主信息;
		addLine(L"主键: ", 节点主键摘要(node, 120));
		addLine(L"节点类型: ", 节点类型文本(mi));
		if (mi && mi->名称) addLine(L"名称: ", 词显示文本(mi->名称));
		if (mi && mi->类型) addLine(L"类型: ", 词显示文本(mi->类型));
		if (mi) addLine(L"命中次数: ", 无符号文本(取命中次数(mi)));
		addLine(L"直接子节点数: ", 无符号文本(计数直属基础子节点(node)));

		if (!mi) return;

		if (auto* smi = dynamic_cast<场景节点主信息类*>(mi)) {
			addLine(L"世界类型: ", 世界类型文本(smi->世界类型));
			if (smi->宿主存在) addLine(L"宿主存在: ", 节点主键摘要(smi->宿主存在, 96));
			if (smi->来源文章) addLine(L"来源文章: ", 节点主键摘要(smi->来源文章, 96));
			addLine(L"创建时间: ", 时间文本(smi->创建时间));
			addLine(L"最后观测时间: ", 时间文本(smi->最后观测时间));
			addLine(L"状态数: ", 无符号文本((unsigned long long)smi->状态总数()));
			addLine(L"动态数: ", 无符号文本((unsigned long long)smi->动态总数()));
			addLine(L"事件动态数: ", 无符号文本((unsigned long long)smi->事件动态总数()));
			addLine(L"关系列表数: ", 无符号文本((unsigned long long)smi->关系列表.size()));
			addLine(L"实例因果数: ", 无符号文本((unsigned long long)smi->实例因果列表.size()));
			addLine(L"因果链数: ", 无符号文本((unsigned long long)smi->因果链列表.size()));
			return;
		}

		if (auto* emi = dynamic_cast<存在节点主信息类*>(mi)) {
			if (emi->概念模板) addLine(L"概念模板: ", 节点主键摘要(emi->概念模板, 96));
			if (emi->内部世界) addLine(L"内部世界: ", 节点主键摘要(emi->内部世界, 96));
			addLine(L"创建时间: ", 时间文本(emi->创建时间));
			addLine(L"最后观测时间: ", 时间文本(emi->最后观测时间));
			addLine(L"可信度(Q10000): ", 整数文本(emi->可信度));
			addLine(L"连续命中帧: ", 无符号文本(emi->连续命中帧));
			addLine(L"连续未命中帧: ", 无符号文本(emi->连续未命中帧));
			addLine(L"连续静止帧: ", 无符号文本(emi->连续静止帧));
			addLine(L"最近位移(mm): ", 整数文本(emi->最近位移_mm));
			if (emi->有位置历史) {
				addLine(L"上次观测位置(mm): ", 格式化向量3(emi->上次观测位置_mm));
				addLine(L"最近观测位置(mm): ", 格式化向量3(emi->最近观测位置_mm));
			}
			addLine(L"实例因果数: ", 无符号文本((unsigned long long)emi->实例因果列表.size()));
			addLine(L"因果链数: ", 无符号文本((unsigned long long)emi->因果链列表.size()));
			return;
		}

		if (auto* fmi = dynamic_cast<特征节点主信息类*>(mi)) {
			addLine(L"当前值: ", 格式化快照(fmi->当前快照));
			addLine(L"当前稳态索引: ", 整数文本(fmi->当前稳态索引));
			addLine(L"稳态数量: ", 无符号文本((unsigned long long)fmi->稳态表.size()));
			addLine(L"可信度(Q10000): ", 无符号文本(fmi->可信度));
			if (fmi->区间摘要 && fmi->区间摘要->有效()) {
				addLine(L"区间摘要: ", 格式化多维区间(*fmi->区间摘要));
				addLine(L"区间语义: ", 区间语义文本(fmi->区间语义));
			}
			if (fmi->区间代表点.has_value()) {
				addLine(L"区间代表点: ", 格式化快照(*fmi->区间代表点));
			}
			if (fmi->连续测量误差区间.有效()) {
				addLine(L"连续测量误差区间: ", 格式化区间(fmi->连续测量误差区间));
			}
			return;
		}

		if (auto* smi = dynamic_cast<状态节点主信息类*>(mi)) {
			addLine(L"状态域: ", 状态域文本(smi->状态域));
			if (smi->状态主体) addLine(L"状态主体: ", 节点主键摘要(smi->状态主体, 96));
			if (smi->状态特征) addLine(L"状态特征: ", 取状态特征摘要(smi));
			addLine(L"状态值: ", 格式化快照(smi->状态值));
			if (smi->对应信息节点) addLine(L"对应信息节点: ", 节点主键摘要(smi->对应信息节点, 96));
			addLine(L"收到时间: ", 时间文本(smi->收到时间));
			addLine(L"发生时间: ", 时间文本(smi->发生时间));
			addLine(L"是否变化: ", 布尔文本(smi->是否变化));
			if (!smi->变化原因类别.empty()) addLine(L"变化原因类别: ", Utf8ToCString(smi->变化原因类别));
			if (!smi->变化原因说明.empty()) addLine(L"变化原因说明: ", Utf8ToCString(smi->变化原因说明));
			return;
		}

		if (auto* dmi = dynamic_cast<动态节点主信息类*>(mi)) {
			addLine(L"动作摘要: ", 取动态动作摘要(dmi));
			if (dmi->动态主体) addLine(L"动态主体: ", 节点主键摘要(dmi->动态主体, 96));
			if (dmi->动态特征 && dmi->动态特征->主信息) {
				if (auto* fmi = dynamic_cast<特征节点主信息类*>(dmi->动态特征->主信息)) {
					addLine(L"动态特征: ", 词显示文本(fmi->类型, L"<空特征>"));
				}
			}
			if (dmi->初始状态) addLine(L"初始状态: ", 节点主键摘要(dmi->初始状态, 96));
			if (dmi->结果状态) addLine(L"结果状态: ", 节点主键摘要(dmi->结果状态, 96));
			addLine(L"开始时间: ", 时间文本(dmi->开始时间));
			addLine(L"结束时间: ", 时间文本(dmi->结束时间));
			addLine(L"来源类型: ", 动态来源文本(dmi->来源类型));
			if (dmi->来源方法首节点) addLine(L"来源方法首节点: ", 节点主键摘要(dmi->来源方法首节点, 96));
			if (dmi->来源动作名) addLine(L"来源动作名: ", 词显示文本(dmi->来源动作名));
			addLine(L"来源动作相位: ", 动作相位文本(dmi->来源动作相位));
			if (dmi->来源输入场景) addLine(L"来源输入场景: ", 节点主键摘要(dmi->来源输入场景, 96));
			if (dmi->来源输出场景) addLine(L"来源输出场景: ", 节点主键摘要(dmi->来源输出场景, 96));
			addLine(L"来源执行成功: ", 布尔文本(dmi->来源执行成功));
			if (dmi->来源错误码 != 0) addLine(L"来源错误码: ", 整数文本(dmi->来源错误码));
			addLine(L"事件数: ", 无符号文本((unsigned long long)dmi->事件列表.size()));
			addLine(L"状态路径数: ", 无符号文本((unsigned long long)dmi->状态路径列表.size()));
			addLine(L"动态路径签名: ", 无符号文本(dmi->动态路径签名));
			return;
		}

		if (auto* bmi = dynamic_cast<二次特征主信息类*>(mi)) {
			addLine(L"形态: ", 二次特征形态文本(bmi->形态));
			addLine(L"种类值: ", 整数文本((int)bmi->种类));
			if (!bmi->度量签名_链键.empty()) addLine(L"度量签名链键: ", 截断文本(Utf8ToCString(bmi->度量签名_链键), 120));
			if (bmi->概念模板) addLine(L"概念模板: ", 节点主键摘要(bmi->概念模板, 96));
			if (bmi->标量区间.有效()) addLine(L"标量区间: ", 格式化区间(bmi->标量区间));
			addLine(L"区间层级: ", 二次特征区间层级文本(bmi->区间层级));
			if (bmi->概念名称) addLine(L"概念名称: ", 词显示文本(bmi->概念名称));
			if (bmi->所属场景) addLine(L"所属场景: ", 节点主键摘要(bmi->所属场景, 96));
			if (bmi->主体) addLine(L"主体: ", 节点主键摘要(bmi->主体, 96));
			if (bmi->客体) addLine(L"客体: ", 节点主键摘要(bmi->客体, 96));
			if (bmi->左对象) addLine(L"左对象: ", 节点主键摘要(bmi->左对象, 96));
			if (bmi->右对象) addLine(L"右对象: ", 节点主键摘要(bmi->右对象, 96));
			addLine(L"标量值: ", 整数文本(bmi->标量值));
			addLine(L"是否满足: ", 布尔文本(bmi->是否满足));
			addLine(L"置信度: ", 整数文本(bmi->置信度));
			addLine(L"度量复杂度: ", 二次特征复杂度文本(bmi->度量复杂度));
			addLine(L"时间窗: ", 二次特征时间窗文本(bmi->时间窗));
			addLine(L"依赖度量数: ", 无符号文本((unsigned long long)bmi->依赖度量签名表.size()));
			return;
		}

		if (auto* cmi = dynamic_cast<因果模板主信息类*>(mi)) {
			addLine(L"适用锚点类型: ", 因果锚点类型文本(cmi->适用锚点类型));
			addLine(L"适用层级: ", 无符号文本(cmi->适用层级));
			addLine(L"适用状态面签名: ", 无符号文本(cmi->适用状态面签名));
			if (cmi->动作模板) addLine(L"动作模板: ", 取动作模板摘要(cmi->动作模板));
			addLine(L"条件模板数: ", 无符号文本((unsigned long long)cmi->条件模板列表.size()));
			addLine(L"结果模板数: ", 无符号文本((unsigned long long)cmi->结果模板列表.size()));
			addLine(L"证据实例数: ", 无符号文本((unsigned long long)cmi->证据实例列表.size()));
			addLine(L"代表证据数: ", 无符号文本((unsigned long long)cmi->代表证据列表.size()));
			addLine(L"反例数: ", 无符号文本((unsigned long long)cmi->反例实例列表.size()));
			addLine(L"证据次数: ", 无符号文本(cmi->证据次数));
			addLine(L"成立次数: ", 无符号文本(cmi->成立次数));
			addLine(L"失败次数: ", 无符号文本(cmi->失败次数));
			addLine(L"最近命中时间: ", 时间文本(cmi->最近命中时间));
			addLine(L"最近失败时间: ", 时间文本(cmi->最近失败时间));
			addLine(L"稳定度: ", 整数文本(cmi->稳定度));
			if (!cmi->典型条件方向摘要.empty()) addLine(L"典型条件摘要: ", Utf8ToCString(cmi->典型条件方向摘要));
			if (!cmi->典型动作语义摘要.empty()) addLine(L"典型动作摘要: ", Utf8ToCString(cmi->典型动作语义摘要));
			if (!cmi->典型结果方向摘要.empty()) addLine(L"典型结果摘要: ", Utf8ToCString(cmi->典型结果方向摘要));
			if (!cmi->最常见失败边界摘要.empty()) addLine(L"失败边界摘要: ", Utf8ToCString(cmi->最常见失败边界摘要));
			return;
		}

		if (auto* cmi = dynamic_cast<因果链主信息类*>(mi)) {
			addLine(L"因果实例序列长度: ", 无符号文本((unsigned long long)cmi->因果实例序列.size()));
			addLine(L"起点锚点主键: ", cmi->起点锚点主键_存档.empty() ? 无符号文本(cmi->起点锚点主键) : Utf8ToCString(cmi->起点锚点主键_存档));
			addLine(L"终点锚点主键: ", cmi->终点锚点主键_存档.empty() ? 无符号文本(cmi->终点锚点主键) : Utf8ToCString(cmi->终点锚点主键_存档));
			addLine(L"起始时间: ", 时间文本(cmi->起始时间));
			addLine(L"结束时间: ", 时间文本(cmi->结束时间));
			return;
		}

		if (auto* cmi = dynamic_cast<因果实例主信息类*>(mi)) {
			addLine(L"锚点类型: ", 因果锚点类型文本(cmi->锚点类型));
			if (cmi->锚点信息) addLine(L"锚点信息: ", 节点主键摘要(cmi->锚点信息, 96));
			addLine(L"状态层级: ", 无符号文本(cmi->状态层级));
			addLine(L"状态面签名: ", 无符号文本(cmi->状态面签名));
			addLine(L"条件时间: ", 时间文本(cmi->条件时间));
			addLine(L"动作时间: ", 时间文本(cmi->动作时间));
			addLine(L"结果时间: ", 时间文本(cmi->结果时间));
			addLine(L"条件状态数: ", 无符号文本((unsigned long long)取因果条件状态数(cmi)));
			if (cmi->动作主体) addLine(L"动作主体: ", 节点主键摘要(cmi->动作主体, 96));
			if (cmi->动作语义) addLine(L"动作语义: ", 节点主键摘要(cmi->动作语义, 96));
			if (cmi->动作信息) addLine(L"动作信息: ", 节点主键摘要(cmi->动作信息, 96));
			if (cmi->动作动态) addLine(L"动作动态: ", 节点主键摘要(cmi->动作动态, 96));
			if (cmi->触发动态) addLine(L"触发动态: ", 节点主键摘要(cmi->触发动态, 96));
			addLine(L"结果状态数: ", 无符号文本((unsigned long long)取因果结果状态数(cmi)));
			if (const auto* 主结果状态主信息 = 取因果主结果状态主信息(cmi)) addLine(L"主结果状态: ", 格式化状态主信息摘要(主结果状态主信息, 96));
			else if (cmi->主结果状态) addLine(L"主结果状态: ", 节点主键摘要(cmi->主结果状态, 96));
			addLine(L"结果动态数: ", 无符号文本((unsigned long long)cmi->结果动态列表.size()));
			if (cmi->主结果动态) addLine(L"主结果动态: ", 节点主键摘要(cmi->主结果动态, 96));
			addLine(L"来源类型: ", 因果来源文本(cmi->来源类型));
			if (!cmi->来源任务主键_存档.empty()) addLine(L"来源任务主键: ", Utf8ToCString(cmi->来源任务主键_存档));
			else if (cmi->来源任务主键 != 0) addLine(L"来源任务主键: ", 无符号文本(cmi->来源任务主键));
			if (!cmi->来源方法主键_存档.empty()) addLine(L"来源方法主键: ", Utf8ToCString(cmi->来源方法主键_存档));
			else if (cmi->来源方法主键 != 0) addLine(L"来源方法主键: ", 无符号文本(cmi->来源方法主键));
			addLine(L"置信度: ", 整数文本(cmi->置信度));
			addLine(L"已验证: ", 布尔文本(cmi->已验证));

			if (auto* compat = dynamic_cast<因果兼容主信息类*>(cmi)) {
				addLine(L"兼容结构类型值: ", 整数文本((int)compat->结构类型));
				if (!compat->因果累计键_存档.empty()) addLine(L"因果累计键: ", Utf8ToCString(compat->因果累计键_存档));
				addLine(L"首次发生时间: ", 时间文本(compat->首次发生时间));
				addLine(L"最近发生时间: ", 时间文本(compat->最近发生时间));
				addLine(L"观察次数: ", 整数文本(compat->观察次数));
				addLine(L"稳定度Q: ", 整数文本(compat->稳定度Q));
				addLine(L"安全归因Q: ", 整数文本(compat->安全归因Q));
				addLine(L"服务归因Q: ", 整数文本(compat->服务归因Q));
				addLine(L"是否原始观测实例: ", 布尔文本(compat->是否原始观测实例));
				if (compat->是否原始观测实例) addLine(L"原始观测序号: ", 整数文本(compat->原始观测序号));
				if (compat->归纳因果节点) addLine(L"归纳因果节点: ", 节点主键摘要(compat->归纳因果节点, 96));
				if (compat->因果主体) addLine(L"因果主体: ", 节点主键摘要(compat->因果主体, 96));
				if (!compat->动作语义键.empty()) addLine(L"动作语义键: ", Utf8ToCString(compat->动作语义键));
			}
			return;
		}

		if (auto* pmi = dynamic_cast<指代节点主信息类*>(mi)) {
			addLine(L"代词: ", 词显示文本(pmi->代词, L"<空代词>"));
			if (pmi->指代对象) addLine(L"指代对象: ", 节点主键摘要(pmi->指代对象, 96));
		}
	}

	static void 收集方法节点详情(方法节点类* node, std::vector<CString>& out)
	{
		if (!node || !node->主信息) return;
		auto addLine = [&](const wchar_t* label, const CString& value) {
			CString line(label);
			line += value;
			out.push_back(line);
		};

		auto* mi = dynamic_cast<方法信息基类*>(node->主信息);
		if (!mi) return;

		addLine(L"主键: ", 节点主键摘要(node, 120));
		addLine(L"节点种类: ", 方法节点种类文本(mi->节点种类));
		if (mi->描述信息) addLine(L"描述信息: ", 节点主键摘要(mi->描述信息, 96));
		if (mi->方法虚拟存在根) addLine(L"方法虚拟存在根: ", 节点主键摘要(mi->方法虚拟存在根, 96));
		addLine(L"模板场景子节点数: ", 无符号文本(计数同层节点(static_cast<基础信息节点类*>(mi->场景根节点.子))));
		addLine(L"运行次数: ", 整数文本(mi->评估原始量.节点_运行次数));
		addLine(L"成功次数: ", 整数文本(mi->评估原始量.节点_成功次数));
		addLine(L"失败次数: ", 整数文本(mi->评估原始量.节点_失败次数));
		addLine(L"样本数: ", 整数文本(mi->评估原始量.节点_样本数));
		addLine(L"连续成功次数: ", 整数文本(mi->评估原始量.节点_连续成功次数));
		addLine(L"连续失败次数: ", 整数文本(mi->评估原始量.节点_连续失败次数));
		if (mi->评估原始量.节点_最后运行时间_微秒 != 0) addLine(L"最后运行时间(微秒): ", 整数文本(mi->评估原始量.节点_最后运行时间_微秒));
		if (mi->评估原始量.节点_最后成功时间_微秒 != 0) addLine(L"最后成功时间(微秒): ", 整数文本(mi->评估原始量.节点_最后成功时间_微秒));
		if (mi->评估原始量.节点_最后失败时间_微秒 != 0) addLine(L"最后失败时间(微秒): ", 整数文本(mi->评估原始量.节点_最后失败时间_微秒));
		if (mi->评估原始量.节点_累计耗时_微秒 != 0) addLine(L"累计耗时(微秒): ", 整数文本(mi->评估原始量.节点_累计耗时_微秒));
		addLine(L"坏因数: ", 无符号文本((unsigned long long)mi->评估原始量.节点_坏因集合.size()));
		addLine(L"缺口数: ", 无符号文本((unsigned long long)mi->评估原始量.节点_缺口集合.size()));
		addLine(L"副作用数: ", 无符号文本((unsigned long long)mi->评估原始量.节点_副作用集合.size()));

		if (auto* hmi = dynamic_cast<方法首节点主信息类*>(mi)) {
			if (hmi->动作名) addLine(L"动作名: ", 词显示文本(hmi->动作名));
			addLine(L"动作句柄: ", 动作句柄文本(hmi->动作句柄));
			addLine(L"来源: ", 方法来源文本(hmi->来源));
			addLine(L"影响度量签名数: ", 无符号文本((unsigned long long)hmi->影响度量签名.size()));
			return;
		}

		if (auto* cmi = dynamic_cast<方法条件节点主信息类*>(mi)) {
			addLine(L"条件判定索引数: ", 无符号文本((unsigned long long)cmi->条件判定索引.size()));
			if (!cmi->条件场景模板主键.empty()) addLine(L"条件场景模板主键: ", Utf8ToCString(cmi->条件场景模板主键));
			if (cmi->时间起 != 0) addLine(L"时间起: ", 时间文本(cmi->时间起));
			if (cmi->时间止 != 0) addLine(L"时间止: ", 时间文本(cmi->时间止));
			return;
		}

		if (auto* rmi = dynamic_cast<方法结果节点主信息类*>(mi)) {
			addLine(L"结果摘要索引数: ", 无符号文本((unsigned long long)rmi->结果摘要索引.size()));
		}
	}

	static HTREEITEM 插入静态树项(CTreeCtrl* tree, HTREEITEM parent, const CString& text)
	{
		if (!tree) return nullptr;
		HTREEITEM item = tree->InsertItem(text, parent, TVI_LAST);
		tree->SetItemData(item, 0);
		return item;
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
	ON_BN_CLICKED(IDC_BUTTON_COPY_WORLD_TREE, &窗口_基础信息类::OnBnClickedButtonCopyWorldTree)
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL 窗口_基础信息类::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg && pMsg->message == WM_KEYDOWN && (GetKeyState(VK_CONTROL) & 0x8000) != 0) {
		if (pMsg->wParam == 'C' || pMsg->wParam == 'c') {
			复制显示树到剪贴板();
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void 窗口_基础信息类::OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	auto* pNM = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if (m_tree && pNM && pNM->itemNew.hItem) {
		auto* payload = 取树项负载(pNM->itemNew.hItem);
		const bool 仅双击展开 = payload && payload->仅双击展开;
		if (!仅双击展开 && 需要展开加载(pNM->itemNew.hItem)) {
			m_tree->Expand(pNM->itemNew.hItem, TVE_EXPAND);
		}
	}
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
	if (CWnd* 确定按钮 = GetDlgItem(IDOK)) {
		确定按钮->ShowWindow(SW_HIDE);
	}
	if (CWnd* 取消按钮 = GetDlgItem(IDCANCEL)) {
		取消按钮->ShowWindow(SW_HIDE);
	}
	更新自适应布局();
	刷新世界树();
	return TRUE;
}

void 窗口_基础信息类::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (nType == SIZE_MINIMIZED || !::IsWindow(m_hWnd)) {
		return;
	}
	更新自适应布局();
}

void 窗口_基础信息类::更新自适应布局()
{
	if (!::IsWindow(m_hWnd)) return;

	CRect 客户区;
	GetClientRect(&客户区);
	if (客户区.Width() <= 0 || 客户区.Height() <= 0) return;

	const int 边距 = 8;
	const int 按钮间距 = 6;
	const int 列间距 = 8;
	const int 最小树宽 = 180;
	int 按钮列宽 = 78;
	int 按钮高度 = 24;

	if (CWnd* 刷新按钮 = GetDlgItem(IDC_BUTTON1); 刷新按钮 && ::IsWindow(刷新按钮->m_hWnd)) {
		CRect 按钮区域;
		刷新按钮->GetWindowRect(&按钮区域);
		ScreenToClient(&按钮区域);
		按钮列宽 = std::max(按钮列宽, 按钮区域.Width());
		按钮高度 = std::max(按钮高度, 按钮区域.Height());
	}
	if (CWnd* 复制按钮 = GetDlgItem(IDC_BUTTON_COPY_WORLD_TREE); 复制按钮 && ::IsWindow(复制按钮->m_hWnd)) {
		CRect 按钮区域;
		复制按钮->GetWindowRect(&按钮区域);
		ScreenToClient(&按钮区域);
		按钮列宽 = std::max(按钮列宽, 按钮区域.Width());
		按钮高度 = std::max(按钮高度, 按钮区域.Height());
	}

	int 最大按钮列宽 = 客户区.Width() - 边距 * 2 - 列间距 - 最小树宽;
	if (最大按钮列宽 < 按钮列宽) {
		按钮列宽 = std::max(0, 最大按钮列宽);
	}

	CRect 树区域(边距, 边距, 客户区.right - 边距, 客户区.bottom - 边距);
	if (按钮列宽 > 0) {
		树区域.right -= 按钮列宽 + 列间距;
	}
	if (树区域.right < 树区域.left + 80) {
		树区域.right = 树区域.left + 80;
	}
	if (树区域.bottom < 树区域.top + 80) {
		树区域.bottom = 树区域.top + 80;
	}

	if (m_tree && ::IsWindow(m_tree->m_hWnd)) {
		m_tree->MoveWindow(&树区域, TRUE);
	}

	int 按钮左 = 树区域.right + 列间距;
	int 按钮顶 = 边距;
	int 实际按钮宽 = std::max(0, static_cast<int>(客户区.right) - 边距 - 按钮左);

	if (CWnd* 刷新按钮 = GetDlgItem(IDC_BUTTON1); 刷新按钮 && ::IsWindow(刷新按钮->m_hWnd)) {
		刷新按钮->MoveWindow(按钮左, 按钮顶, 实际按钮宽, 按钮高度, TRUE);
		刷新按钮->ShowWindow(实际按钮宽 > 0 ? SW_SHOW : SW_HIDE);
		按钮顶 += 按钮高度 + 按钮间距;
	}
	if (CWnd* 复制按钮 = GetDlgItem(IDC_BUTTON_COPY_WORLD_TREE); 复制按钮 && ::IsWindow(复制按钮->m_hWnd)) {
		复制按钮->MoveWindow(按钮左, 按钮顶, 实际按钮宽, 按钮高度, TRUE);
		复制按钮->ShowWindow(实际按钮宽 > 0 ? SW_SHOW : SW_HIDE);
	}
}

HTREEITEM 窗口_基础信息类::插入树项(HTREEITEM hParent, const CString& 文本, 树项类型 类型, void* 指针, bool 可展开, bool 仅双击展开)
{
	if (!m_tree) return nullptr;
	HTREEITEM item = m_tree->InsertItem(文本, hParent, TVI_LAST);
	if (类型 != 树项类型::无 || 指针 != nullptr) {
		auto payload = std::make_unique<树项负载>();
		payload->类型 = 类型;
		payload->指针 = 指针;
		payload->仅双击展开 = 仅双击展开;
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

void 窗口_基础信息类::释放树项负载递归(HTREEITEM hItem)
{
	if (!m_tree || !hItem) return;

	for (HTREEITEM child = m_tree->GetChildItem(hItem); child; child = m_tree->GetNextSiblingItem(child)) {
		释放树项负载递归(child);
	}

	const auto raw = m_tree->GetItemData(hItem);
	if (raw == 0 || raw == kPlaceholderData) return;

	auto* payload = reinterpret_cast<树项负载*>(raw);
	树项负载池_.erase(
		std::remove_if(
			树项负载池_.begin(),
			树项负载池_.end(),
			[payload](const std::unique_ptr<树项负载>& item) {
				return item.get() == payload;
			}),
		树项负载池_.end());
}

void 窗口_基础信息类::清空子项并释放负载(HTREEITEM hParent)
{
	if (!m_tree || !hParent) return;

	for (HTREEITEM child = m_tree->GetChildItem(hParent); child; ) {
		HTREEITEM next = m_tree->GetNextSiblingItem(child);
		释放树项负载递归(child);
		m_tree->DeleteItem(child);
		child = next;
	}
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
	auto* payload = 取树项负载(hItem);
	if (!payload || !payload->指针) return false;
	if ((m_tree->GetItemState(hItem, TVIS_EXPANDED) & TVIS_EXPANDED) != 0) return false;
	HTREEITEM child = m_tree->GetChildItem(hItem);
	if (!child) return false;
	return true;
}

void 窗口_基础信息类::填充根节点()
{
	if (!m_tree) return;

	auto* 世界根节点 = 世界树.世界根();
	CString worldTitle(L"世界树");
	HTREEITEM worldRoot = m_tree->InsertItem(worldTitle, TVI_ROOT, TVI_LAST);
	m_tree->SetItemData(worldRoot, 0);

	if (!世界根节点) {
		插入静态树项(m_tree, worldRoot, L"世界根未初始化");
		m_tree->Expand(worldRoot, TVE_EXPAND);
		return;
	}

	std::size_t firstLayerCount = 0;
	std::unordered_set<基础信息节点类*> insertedNodes{};
	auto 插入首层节点 = [&](基础信息节点类* node) {
		if (!node || !insertedNodes.insert(node).second) return;
		++firstLayerCount;
		CString txt = 格式化基础节点标题(node);
		插入树项(worldRoot, txt, 树项类型::基础节点, node, 基础节点有可显示内容(node));
	};

	遍历根链基础节点(世界根节点, [&](基础信息节点类* node) {
		插入首层节点(node);
	});
	if (firstLayerCount == 0) {
		遍历直属基础子节点(世界根节点, [&](基础信息节点类* node) {
			插入首层节点(node);
		});
	}
	if (firstLayerCount == 0) {
		插入静态树项(m_tree, worldRoot, L"根链为空");
	}
	m_tree->Expand(worldRoot, TVE_EXPAND);
}

void 窗口_基础信息类::刷新世界树()
{
	if (!m_tree) return;
	m_tree->SetRedraw(FALSE);
	m_tree->DeleteAllItems();
	清理树项负载();
	填充根节点();
	m_tree->SetRedraw(TRUE);
	m_tree->Invalidate();
	m_tree->UpdateWindow();
}

void 窗口_基础信息类::插入字段引用_基础节点(HTREEITEM hParent, const CString& 字段名, const void* nodePtr)
{
	auto* node = const_cast<基础信息节点类*>(reinterpret_cast<const 基础信息节点类*>(nodePtr));
	if (!m_tree || !hParent || !node) return;
	CString text;
	text.Format(L"%s: %s", (LPCTSTR)字段名, (LPCTSTR)格式化基础节点标题(node));
	插入树项(hParent, text, 树项类型::基础节点, node, 基础节点有可显示内容(node), true);
}

void 窗口_基础信息类::插入字段引用_方法节点(HTREEITEM hParent, const CString& 字段名, const void* nodePtr)
{
	auto* node = const_cast<方法节点类*>(reinterpret_cast<const 方法节点类*>(nodePtr));
	if (!m_tree || !hParent || !node) return;
	CString text;
	text.Format(L"%s: %s", (LPCTSTR)字段名, (LPCTSTR)格式化方法节点标题(node));
	插入树项(hParent, text, 树项类型::方法节点, node, 方法节点有可显示内容(node), true);
}

void 窗口_基础信息类::插入字段引用_语素节点(HTREEITEM hParent, const CString& 字段名, const void* nodePtr)
{
	auto* node = const_cast<语素节点类*>(reinterpret_cast<const 语素节点类*>(nodePtr));
	if (!m_tree || !hParent || !node) return;
	CString text;
	text.Format(L"%s: %s", (LPCTSTR)字段名, (LPCTSTR)格式化语素节点标题(node));
	插入树项(hParent, text, 树项类型::语素节点, node, 语素节点有可显示内容(node), true);
}

void 窗口_基础信息类::插入字段引用_自然语言节点(HTREEITEM hParent, const CString& 字段名, const void* nodePtr)
{
	auto* node = const_cast<自然句节点类*>(reinterpret_cast<const 自然句节点类*>(nodePtr));
	if (!m_tree || !hParent || !node) return;
	CString text;
	text.Format(L"%s: %s", (LPCTSTR)字段名, (LPCTSTR)格式化自然语言节点标题(node));
	插入树项(hParent, text, 树项类型::自然语言节点, node, 自然语言节点有可显示内容(node), true);
}

void 窗口_基础信息类::添加基础节点指针字段(HTREEITEM hParent, void* nodePtr)
{
	auto* node = reinterpret_cast<基础信息节点类*>(nodePtr);
	if (!m_tree || !hParent || !node || !node->主信息) return;

	HTREEITEM 指针字段组 = nullptr;
	HTREEITEM 快照字段组 = nullptr;
	auto 获取指针字段组 = [&]() -> HTREEITEM {
		if (!指针字段组) {
			指针字段组 = 插入静态树项(m_tree, hParent, L"指针字段");
		}
		return 指针字段组;
	};
	auto 获取快照字段组 = [&]() -> HTREEITEM {
		if (!快照字段组) {
			快照字段组 = 插入静态树项(m_tree, hParent, L"状态快照");
		}
		return 快照字段组;
	};
	auto 添加基础指针 = [&](const wchar_t* 字段名, auto* ptr) {
		if (!ptr) return;
		if (auto group = 获取指针字段组()) {
			插入字段引用_基础节点(group, 字段名, ptr);
		}
	};
	auto 添加方法指针 = [&](const wchar_t* 字段名, auto* ptr) {
		if (!ptr) return;
		if (auto group = 获取指针字段组()) {
			插入字段引用_方法节点(group, 字段名, ptr);
		}
	};
	auto 添加语素指针 = [&](const wchar_t* 字段名, auto* ptr) {
		if (!ptr) return;
		if (auto group = 获取指针字段组()) {
			插入字段引用_语素节点(group, 字段名, ptr);
		}
	};
	auto 添加自然语言指针 = [&](const wchar_t* 字段名, auto* ptr) {
		if (!ptr) return;
		if (auto group = 获取指针字段组()) {
			插入字段引用_自然语言节点(group, 字段名, ptr);
		}
	};
	auto 添加基础列表 = [&](const wchar_t* 字段名, const auto& items) {
		std::unordered_set<基础信息节点类*> 去重{};
		HTREEITEM 列表组 = nullptr;
		std::size_t 索引 = 0;
		for (auto* item : items) {
			auto* 基础节点 = reinterpret_cast<基础信息节点类*>(item);
			if (!基础节点 || !去重.insert(基础节点).second) continue;
			if (!列表组) {
				CString title;
				title.Format(L"%s (%llu)", 字段名, (unsigned long long)items.size());
				列表组 = 插入静态树项(m_tree, 获取指针字段组(), title);
			}
			CString label;
			label.Format(L"[%llu]", (unsigned long long)++索引);
			插入字段引用_基础节点(列表组, label, 基础节点);
		}
	};
	auto 添加状态主信息列表 = [&](const wchar_t* 字段名, const std::vector<状态节点主信息类>& items) {
		if (items.empty()) return;
		HTREEITEM 列表组 = nullptr;
		std::size_t 索引 = 0;
		for (const auto& item : items) {
			if (!列表组) {
				CString title;
				title.Format(L"%s (%llu)", 字段名, (unsigned long long)items.size());
				列表组 = 插入静态树项(m_tree, 获取快照字段组(), title);
			}
			CString text;
			text.Format(L"[%llu] %s", (unsigned long long)++索引, (LPCTSTR)格式化状态主信息摘要(&item, 160));
			插入静态树项(m_tree, 列表组, text);
		}
	};
	auto 添加状态主信息单项 = [&](const wchar_t* 字段名, const 状态节点主信息类* item) {
		if (!item) return;
		if (auto group = 获取快照字段组()) {
			CString text;
			text.Format(L"%s: %s", 字段名, (LPCTSTR)格式化状态主信息摘要(item, 160));
			插入静态树项(m_tree, group, text);
		}
	};

	auto* mi = node->主信息;
	添加语素指针(L"名称", mi->名称);
	添加语素指针(L"类型", mi->类型);

	if (auto* smi = dynamic_cast<场景节点主信息类*>(mi)) {
		添加基础指针(L"宿主存在", smi->宿主存在);
		添加自然语言指针(L"来源文章", smi->来源文章);
		添加基础列表(L"状态列表", smi->状态列表);
		添加基础列表(L"动态列表", smi->动态列表);
		添加基础列表(L"事件动态列表", smi->事件动态列表);
		添加基础列表(L"关系列表", smi->关系列表);
		添加基础列表(L"实例因果列表", smi->实例因果列表);
		添加基础列表(L"因果链列表", smi->因果链列表);
	}
	else if (auto* emi = dynamic_cast<存在节点主信息类*>(mi)) {
		添加基础指针(L"概念模板", emi->概念模板);
		添加基础指针(L"内部世界", emi->内部世界);
		添加基础列表(L"实例因果列表", emi->实例因果列表);
		添加基础列表(L"因果链列表", emi->因果链列表);
	}
	else if (auto* smi = dynamic_cast<状态节点主信息类*>(mi)) {
		添加基础指针(L"状态主体", smi->状态主体);
		添加基础指针(L"状态特征", smi->状态特征);
		添加基础指针(L"对应信息节点", smi->对应信息节点);
	}
	else if (auto* dmi = dynamic_cast<动态节点主信息类*>(mi)) {
		添加基础指针(L"初始状态", dmi->初始状态);
		添加基础指针(L"结果状态", dmi->结果状态);
		添加基础指针(L"动态主体", dmi->动态主体);
		添加基础指针(L"动态特征", dmi->动态特征);
		添加方法指针(L"来源方法首节点", dmi->来源方法首节点);
		添加语素指针(L"来源动作名", dmi->来源动作名);
		添加基础指针(L"来源输入场景", dmi->来源输入场景);
		添加基础指针(L"来源输出场景", dmi->来源输出场景);
		添加基础列表(L"事件列表", dmi->事件列表);
		添加基础列表(L"状态路径列表", dmi->状态路径列表);
	}
	else if (auto* bmi = dynamic_cast<二次特征主信息类*>(mi)) {
		添加基础指针(L"概念模板", bmi->概念模板);
		添加语素指针(L"概念名称", bmi->概念名称);
		添加基础指针(L"所属场景", bmi->所属场景);
		添加基础指针(L"主体", bmi->主体);
		添加基础指针(L"客体", bmi->客体);
		添加基础指针(L"左对象", bmi->左对象);
		添加基础指针(L"右对象", bmi->右对象);
	}
	else if (auto* cmi = dynamic_cast<因果模板主信息类*>(mi)) {
		添加基础列表(L"条件模板列表", cmi->条件模板列表);
		添加基础指针(L"动作模板", cmi->动作模板);
		添加基础列表(L"结果模板列表", cmi->结果模板列表);
		添加基础列表(L"证据实例列表", cmi->证据实例列表);
		添加基础列表(L"代表证据列表", cmi->代表证据列表);
		添加基础列表(L"反例实例列表", cmi->反例实例列表);
	}
	else if (auto* cmi = dynamic_cast<因果链主信息类*>(mi)) {
		添加基础列表(L"因果实例序列", cmi->因果实例序列);
	}
	else if (auto* cmi = dynamic_cast<因果实例主信息类*>(mi)) {
		添加基础指针(L"锚点信息", cmi->锚点信息);
		添加状态主信息列表(L"条件状态快照列表", cmi->条件状态主信息列表);
		if (cmi->条件状态主信息列表.empty()) {
			添加基础列表(L"条件状态列表", cmi->条件状态列表);
		}
		添加基础指针(L"动作主体", cmi->动作主体);
		添加基础指针(L"动作语义", cmi->动作语义);
		添加基础指针(L"动作信息", cmi->动作信息);
		添加基础指针(L"动作动态", cmi->动作动态);
		添加基础指针(L"触发动态", cmi->触发动态);
		添加状态主信息列表(L"结果状态快照列表", cmi->结果状态主信息列表);
		if (cmi->结果状态主信息列表.empty()) {
			添加基础列表(L"结果状态列表", cmi->结果状态列表);
		}
		添加状态主信息单项(L"主结果状态快照", 取因果主结果状态主信息(cmi));
		if (cmi->主结果状态主信息列表.empty()) {
			添加基础指针(L"主结果状态", cmi->主结果状态);
		}
		添加基础列表(L"结果动态列表", cmi->结果动态列表);
		添加基础指针(L"主结果动态", cmi->主结果动态);
		if (auto* compat = dynamic_cast<因果兼容主信息类*>(cmi)) {
			添加基础指针(L"归纳因果节点", compat->归纳因果节点);
			添加基础指针(L"因信息", compat->因信息);
			添加基础指针(L"果信息", compat->果信息);
			添加基础指针(L"锚点动态", compat->锚点动态);
			添加基础列表(L"条件方向二次特征列表", compat->条件方向二次特征列表);
			添加基础列表(L"过程动态列表", compat->过程动态列表);
			添加基础指针(L"结果状态", compat->结果状态);
			添加基础指针(L"结果方向二次特征", compat->结果方向二次特征);
			添加基础列表(L"事件分段列表", compat->事件分段列表);
			添加基础指针(L"因果主体", compat->因果主体);
		}
	}
	else if (auto* pmi = dynamic_cast<指代节点主信息类*>(mi)) {
		添加语素指针(L"代词", pmi->代词);
		添加基础指针(L"指代对象", pmi->指代对象);
	}

	if (指针字段组) {
		m_tree->Expand(指针字段组, TVE_EXPAND);
	}
	if (快照字段组) {
		m_tree->Expand(快照字段组, TVE_EXPAND);
	}
}

void 窗口_基础信息类::添加基础节点子项(HTREEITEM hParent, void* nodePtr)
{
	auto* node = reinterpret_cast<基础信息节点类*>(nodePtr);
	if (!m_tree || !node) return;

	std::vector<CString> details{};
	收集基础节点详情(node, details);
	if (!details.empty()) {
		HTREEITEM detailGroup = 插入静态树项(m_tree, hParent, L"字段详情");
		if (detailGroup) {
			for (const auto& line : details) {
				插入静态树项(m_tree, detailGroup, line);
			}
			m_tree->Expand(detailGroup, TVE_EXPAND);
		}
	}

	添加基础节点指针字段(hParent, node);

	std::unordered_set<基础信息节点类*> insertedNodes{};
	HTREEITEM childGroup = nullptr;
	遍历直属基础子节点(node, [&](基础信息节点类* ch) {
		if (!ch || !insertedNodes.insert(ch).second) return;
		if (!childGroup) {
			childGroup = 插入静态树项(m_tree, hParent, L"子链");
		}
		CString txt = 格式化基础节点标题(ch);
		插入树项(childGroup, txt, 树项类型::基础节点, ch, 基础节点有可显示内容(ch));
	});
	if (childGroup) {
		m_tree->Expand(childGroup, TVE_EXPAND);
	}
}

void 窗口_基础信息类::添加语素节点子项(HTREEITEM hParent, void* nodePtr)
{
	auto* node = reinterpret_cast<语素节点类*>(nodePtr);
	if (!m_tree || !node) return;

	std::vector<CString> details{};
	收集语素节点详情(node, details);
	if (!details.empty()) {
		HTREEITEM detailGroup = 插入静态树项(m_tree, hParent, L"字段详情");
		if (detailGroup) {
			for (const auto& line : details) {
				插入静态树项(m_tree, detailGroup, line);
			}
			m_tree->Expand(detailGroup, TVE_EXPAND);
		}
	}

	HTREEITEM 指针字段组 = nullptr;
	auto 获取指针字段组 = [&]() -> HTREEITEM {
		if (!指针字段组) {
			指针字段组 = 插入静态树项(m_tree, hParent, L"指针字段");
		}
		return 指针字段组;
	};
	auto* mi = node->主信息;
	if (auto* pmi = dynamic_cast<词性主信息类*>(mi)) {
		if (pmi->对应基础信息指针) {
			插入字段引用_基础节点(获取指针字段组(), L"对应基础信息", pmi->对应基础信息指针);
		}
		if (!pmi->示例列表.empty()) {
			HTREEITEM 示例组 = 插入静态树项(m_tree, 获取指针字段组(), L"示例列表");
			std::size_t idx = 0;
			for (const auto& 示例 : pmi->示例列表) {
				CString title;
				title.Format(L"示例 #%llu", (unsigned long long)++idx);
				HTREEITEM item = 插入静态树项(m_tree, 示例组, title);
				if (示例.场景指针) 插入字段引用_基础节点(item, L"场景", 示例.场景指针);
				if (示例.基础信息指针) 插入字段引用_基础节点(item, L"基础信息", 示例.基础信息指针);
			}
		}
	}
	else if (auto* phr = dynamic_cast<短语主信息类*>(mi)) {
		if (phr->参照词) 插入字段引用_语素节点(获取指针字段组(), L"参照词", phr->参照词);
		if (phr->比较对象词) 插入字段引用_语素节点(获取指针字段组(), L"比较对象词", phr->比较对象词);
	}
	else if (auto* child = dynamic_cast<短语子节点主信息类*>(mi)) {
		if (child->比较词) 插入字段引用_语素节点(获取指针字段组(), L"比较词", child->比较词);
		if (child->结果词) 插入字段引用_语素节点(获取指针字段组(), L"结果词", child->结果词);
	}
	if (指针字段组) {
		m_tree->Expand(指针字段组, TVE_EXPAND);
	}

	std::unordered_set<语素节点类*> insertedNodes{};
	HTREEITEM childGroup = nullptr;
	if (node->子) {
		遍历同层(static_cast<语素节点类*>(node->子), [&](语素节点类* ch) {
			if (!ch || !insertedNodes.insert(ch).second) return;
			if (!childGroup) childGroup = 插入静态树项(m_tree, hParent, L"子链");
			CString txt = 格式化语素节点标题(ch);
			插入树项(childGroup, txt, 树项类型::语素节点, ch, 语素节点有可显示内容(ch));
		});
	}
	if (childGroup) {
		m_tree->Expand(childGroup, TVE_EXPAND);
	}
}

void 窗口_基础信息类::添加自然语言节点子项(HTREEITEM hParent, void* nodePtr)
{
	auto* node = reinterpret_cast<自然句节点类*>(nodePtr);
	if (!m_tree || !node) return;

	std::vector<CString> details{};
	收集自然语言节点详情(node, details);
	if (!details.empty()) {
		HTREEITEM detailGroup = 插入静态树项(m_tree, hParent, L"字段详情");
		if (detailGroup) {
			for (const auto& line : details) {
				插入静态树项(m_tree, detailGroup, line);
			}
			m_tree->Expand(detailGroup, TVE_EXPAND);
		}
	}

	HTREEITEM 指针字段组 = nullptr;
	auto 获取指针字段组 = [&]() -> HTREEITEM {
		if (!指针字段组) {
			指针字段组 = 插入静态树项(m_tree, hParent, L"指针字段");
		}
		return 指针字段组;
	};
	auto 添加语素列表 = [&](const wchar_t* 字段名, const auto& items) {
		HTREEITEM 列表组 = nullptr;
		std::unordered_set<const 语素节点类*> 去重{};
		std::size_t 索引 = 0;
		for (auto* item : items) {
			auto* 语素节点 = reinterpret_cast<const 语素节点类*>(item);
			if (!语素节点 || !去重.insert(语素节点).second) continue;
			if (!列表组) {
				CString title;
				title.Format(L"%s (%llu)", 字段名, (unsigned long long)items.size());
				列表组 = 插入静态树项(m_tree, 获取指针字段组(), title);
			}
			CString label;
			label.Format(L"[%llu]", (unsigned long long)++索引);
			插入字段引用_语素节点(列表组, label, 语素节点);
		}
	};
	auto 添加自然语言列表 = [&](const wchar_t* 字段名, const auto& items) {
		HTREEITEM 列表组 = nullptr;
		std::unordered_set<const 自然句节点类*> 去重{};
		std::size_t 索引 = 0;
		for (auto* item : items) {
			auto* 自然语言节点 = reinterpret_cast<const 自然句节点类*>(item);
			if (!自然语言节点 || !去重.insert(自然语言节点).second) continue;
			if (!列表组) {
				CString title;
				title.Format(L"%s (%llu)", 字段名, (unsigned long long)items.size());
				列表组 = 插入静态树项(m_tree, 获取指针字段组(), title);
			}
			CString label;
			label.Format(L"[%llu]", (unsigned long long)++索引);
			插入字段引用_自然语言节点(列表组, label, 自然语言节点);
		}
	};

	auto* mi = node->主信息;
	if (auto* article = dynamic_cast<文章主信息类*>(mi)) {
		if (article->对应文本世界场景) 插入字段引用_基础节点(获取指针字段组(), L"对应文本世界场景", article->对应文本世界场景);
	}
	else if (auto* sent = dynamic_cast<自然句主信息类*>(mi)) {
		添加语素列表(L"词序列", sent->词序列);
		if (sent->主语) 插入字段引用_语素节点(获取指针字段组(), L"主语", sent->主语);
		if (sent->谓语) 插入字段引用_语素节点(获取指针字段组(), L"谓语", sent->谓语);
		if (sent->宾语) 插入字段引用_语素节点(获取指针字段组(), L"宾语", sent->宾语);
		添加自然语言列表(L"子句节点列表", sent->子句节点列表);
	}
	else if (auto* clause = dynamic_cast<子句主信息类*>(mi)) {
		添加语素列表(L"局部词序列", clause->局部词序列);
		if (clause->子句主语) 插入字段引用_语素节点(获取指针字段组(), L"子句主语", clause->子句主语);
		if (clause->子句谓语) 插入字段引用_语素节点(获取指针字段组(), L"子句谓语", clause->子句谓语);
		if (clause->子句宾语) 插入字段引用_语素节点(获取指针字段组(), L"子句宾语", clause->子句宾语);
		if (clause->对应场景) 插入字段引用_基础节点(获取指针字段组(), L"对应场景", clause->对应场景);
		if (clause->对应状态) 插入字段引用_基础节点(获取指针字段组(), L"对应状态", clause->对应状态);
		if (clause->对应动态) 插入字段引用_基础节点(获取指针字段组(), L"对应动态", clause->对应动态);
		if (clause->对应因果) 插入字段引用_基础节点(获取指针字段组(), L"对应因果", clause->对应因果);
	}
	else if (auto* comp = dynamic_cast<句子成分主信息类*>(mi)) {
		if (comp->对应词性节点) 插入字段引用_语素节点(获取指针字段组(), L"对应词性节点", comp->对应词性节点);
	}
	if (指针字段组) {
		m_tree->Expand(指针字段组, TVE_EXPAND);
	}

	std::unordered_set<自然句节点类*> insertedNodes{};
	HTREEITEM childGroup = nullptr;
	if (node->子) {
		遍历同层(static_cast<自然句节点类*>(node->子), [&](自然句节点类* ch) {
			if (!ch || !insertedNodes.insert(ch).second) return;
			if (!childGroup) childGroup = 插入静态树项(m_tree, hParent, L"子链");
			CString txt = 格式化自然语言节点标题(ch);
			插入树项(childGroup, txt, 树项类型::自然语言节点, ch, 自然语言节点有可显示内容(ch));
		});
	}
	if (childGroup) {
		m_tree->Expand(childGroup, TVE_EXPAND);
	}
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

	std::vector<CString> details{};
	收集方法节点详情(node, details);
	if (!details.empty()) {
		HTREEITEM detailGroup = 插入静态树项(m_tree, hParent, L"字段详情");
		if (detailGroup) {
			for (const auto& line : details) {
				插入静态树项(m_tree, detailGroup, line);
			}
			m_tree->Expand(detailGroup, TVE_EXPAND);
		}
	}

	if (auto* hmi = dynamic_cast<方法首节点主信息类*>(mi)) {
		CString action; action.Format(L"动作: %s", (LPCTSTR)词显示文本(hmi->动作名, L"<未命名方法>"));
		插入树项(hParent, action, 树项类型::无, nullptr, false);
		CString impact; impact.Format(L"影响签名数: %llu", (unsigned long long)hmi->影响度量签名.size());
		插入树项(hParent, impact, 树项类型::无, nullptr, false);
		CString handle(L"动作句柄: ");
		handle += 动作句柄文本(hmi->动作句柄);
		插入树项(hParent, handle, 树项类型::无, nullptr, false);
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
		CString info; info.Format(L"条件索引数: %llu", (unsigned long long)cmi->条件判定索引.size());
		插入树项(hParent, info, 树项类型::无, nullptr, false);
		if (!cmi->条件场景模板主键.empty()) {
			CString key; key.Format(L"条件键: %s", (LPCTSTR)Utf8ToCString(cmi->条件场景模板主键));
			插入树项(hParent, key, 树项类型::无, nullptr, false);
		}
		if (cmi->时间起 != 0 || cmi->时间止 != 0) {
			CString timeWindow;
			timeWindow.Format(L"时间窗口: %lld -> %lld", (long long)cmi->时间起, (long long)cmi->时间止);
			插入树项(hParent, timeWindow, 树项类型::无, nullptr, false);
		}
		if (方法场景根有内容(mi)) {
			插入树项(hParent, L"条件场景", 树项类型::基础节点, &mi->场景根节点, true);
		}
		return;
	}

	if (auto* rmi = dynamic_cast<方法结果节点主信息类*>(mi)) {
		CString info; info.Format(L"结果摘要项: %llu", (unsigned long long)rmi->结果摘要索引.size());
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
	case 树项类型::语素节点:
		添加语素节点子项(hParent, nodePtr);
		break;
	case 树项类型::自然语言节点:
		添加自然语言节点子项(hParent, nodePtr);
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
	if (!m_tree || !hItem) return;
	auto* payload = 取树项负载(hItem);
	if (!payload || !payload->指针) return;
	清空子项并释放负载(hItem);
	添加节点子项(hItem, payload->指针);
}

void 窗口_基础信息类::OnBnClickedButton1()
{
	刷新世界树();
}

void 窗口_基础信息类::OnBnClickedButtonCopyWorldTree()
{
	复制显示树到剪贴板();
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
		else if (需要展开加载(hItem)) {
			m_tree->Expand(hItem, TVE_EXPAND);
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
	HTREEITEM cursor = hItem;
	树项负载* payload = nullptr;
	while (cursor) {
		payload = 取树项负载(cursor);
		if (payload && payload->类型 == 树项类型::基础节点 && payload->指针) {
			break;
		}
		cursor = m_tree->GetParentItem(cursor);
	}
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

void 窗口_基础信息类::追加树项文本(HTREEITEM hItem, int depth, std::wstring& out) const
{
	if (!m_tree || !hItem) return;
	if (m_tree->GetItemData(hItem) == kPlaceholderData) return;

	for (int i = 0; i < depth; ++i) {
		out += L"  ";
	}
	out += std::wstring(m_tree->GetItemText(hItem));
	out += L"\r\n";

	for (HTREEITEM child = m_tree->GetChildItem(hItem); child; child = m_tree->GetNextSiblingItem(child)) {
		追加树项文本(child, depth + 1, out);
	}
}

void 窗口_基础信息类::复制显示树到剪贴板()
{
	if (!m_tree) return;

	std::wstring text{};
	for (HTREEITEM item = m_tree->GetRootItem(); item; item = m_tree->GetNextSiblingItem(item)) {
		追加树项文本(item, 0, text);
	}
	if (text.empty()) return;

	const std::size_t bytes = (text.size() + 1) * sizeof(wchar_t);
	HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, bytes);
	if (!hMem) return;

	void* mem = ::GlobalLock(hMem);
	if (!mem) {
		::GlobalFree(hMem);
		return;
	}
	memcpy(mem, text.c_str(), bytes);
	::GlobalUnlock(hMem);

	if (!::OpenClipboard(m_hWnd)) {
		::GlobalFree(hMem);
		return;
	}
	::EmptyClipboard();
	::SetClipboardData(CF_UNICODETEXT, hMem);
	::CloseClipboard();
}



