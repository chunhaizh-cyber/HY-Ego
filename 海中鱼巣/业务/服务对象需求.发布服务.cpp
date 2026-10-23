#include "服务对象需求.发布服务.h"
#include "../核心/容错检查.h"
#include "../核心/日志系统.h"

#include <algorithm>
#include <condition_variable>
#include <mutex>
#include <set>

namespace 海中鱼巣 {
namespace {
[[noreturn]] void 发布故障(const wchar_t* 原因) noexcept {
    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"服务对象需求特征发布", 原因);
}
void 记录迁移缺口(稳定编码 场景, 稳定编码 主体, const 需求特征迁移缺口& 缺口) noexcept {
    // 仅投影已取得的合法非成功材料；日志设施失败不改变该主体的迁移阻挡或其它主体的就绪。
    try {
        std::wstring 原因 = L"原因文本无法转换，请核对正式读取结果";
        if (缺口.原因.size() <= static_cast<std::size_t>((std::numeric_limits<int>::max)())) {
            const auto 长度 = static_cast<int>(缺口.原因.size());
            const auto 宽长度 = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                缺口.原因.data(), 长度, nullptr, 0);
            if (宽长度 > 0) {
                std::wstring 文本(static_cast<std::size_t>(宽长度), L'\0');
                if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, 缺口.原因.data(),
                    长度, 文本.data(), 宽长度) == 宽长度) 原因 = std::move(文本);
            }
        }
        const auto 可选编码 = [](const std::optional<稳定编码>& 节点) {
            return 节点 ? std::to_wstring(节点->值) : std::wstring(L"无");
        };
        const auto 内容 = L"旧需求特征待迁移；仅阻挡该主体消费，保留旧材料；场景="
            + std::to_wstring(场景.值) + L"；主体=" + std::to_wstring(主体.值)
            + L"；旧特征=" + std::to_wstring(缺口.原特征节点.值)
            + L"；需求=" + 可选编码(缺口.需求节点) + L"；动作=" + 可选编码(缺口.动作节点)
            + L"；状态=" + 可选编码(缺口.状态节点) + L"；原因=" + 原因;
        (void)记录运行日志(L"服务对象需求特征发布", L"初始化旧需求迁移缺口", 内容);
    } catch (...) { /* 尽力诊断失败不改变业务结果。 */ }
}
}

struct 服务对象需求发布服务::实现 final {
    新_世界树类& 世界;
    新_需求类& 需求;
    新世界事实服务引用 事实;
    稳定编码 类型, 旧类型;
    mutable std::mutex 互斥;
    mutable std::condition_variable 等待;
    bool 就绪 = false;
    mutable std::set<稳定编码> 占用;
    实现(新_世界树类& w, 新_需求类& d) : 世界(w), 需求(d), 事实(w.获取事实服务引用()) {}
    服务对象需求发布状态 取得(稳定编码 E) const {
        std::lock_guard 锁(互斥);
        if (!就绪) return 服务对象需求发布状态::已关闭;
        return 占用.insert(E).second ? 服务对象需求发布状态::已读取 : 服务对象需求发布状态::前提已变化;
    }
    void 释放(稳定编码 E) const noexcept {
        std::lock_guard 锁(互斥); 占用.erase(E); 等待.notify_all();
    }
    struct 调用结束 final {
        const 实现& 服务; 稳定编码 主体;
        ~调用结束() { 服务.释放(主体); }
    };
    bool 场景有效(稳定编码 C, 稳定编码 E) const {
        const auto root = 世界.获取世界树根场景();
        return root && 事实.场景服务.是场景节点(C) && 事实.场景服务.存在属于场景范围(*root, C)
            && (C == E || 事实.场景服务.查询所属场景(E) == std::optional<稳定编码>{C});
    }
    std::optional<稳定编码> 找特征(稳定编码 E, 稳定编码 T) const {
        const auto e = 事实.存在服务.获取存在(E); if (!e) return {};
        std::optional<稳定编码> found;
        for (const auto F : e->特征节点组) {
            const auto f = 事实.特征服务.获取特征(F);
            const auto t = f ? 事实.特征服务.捕获特征匹配定义(f->特征概念节点) : std::nullopt;
            if (!f || !t) 发布故障(L"主体持有特征不可读");
            if (t->特征类型根 != T) continue;
            if (found || f->上级特征节点) 发布故障(L"主体需求特征不唯一或不是顶层");
            found = F;
        }
        return found;
    }
    稳定编码 建空特征(稳定编码 E) const {
        if (const auto f = 找特征(E, 类型)) return *f;
        const auto add = 事实.存在服务.添加无当前值特征(E, 类型);
        if (add.状态 != 新存在操作状态::已完成) 发布故障(L"需求空特征建立未完成");
        const auto f = 找特征(E, 类型); if (!f) 发布故障(L"需求空特征读回缺失"); return *f;
    }
    服务对象需求发布状态 确认(const 新需求信息& D) const {
        if (!D.认知来源) 发布故障(L"被认知需求缺少认知来源");
        const auto C = 需求.复核服务要求确认语义(D.认知来源->内容确认节点);
        if (C.状态 == 新服务要求确认状态::需重新确认) return 服务对象需求发布状态::需重新确认;
        if (C.状态 == 新服务要求确认状态::确认不存在) return 服务对象需求发布状态::来源不符;
        if (C.状态 != 新服务要求确认状态::当前有效 || !C.确认) 发布故障(L"需求确认读取失败");
        const auto& c = *C.确认;
        return c.来源人类节点 == D.需求主体存在节点 && c.理解节点 == D.认知来源->理解节点
            && c.目标存在节点 == D.目标存在节点 && c.目标 == D.目标 && c.来源要求节点 == D.认知来源->来源要求节点
            ? 服务对象需求发布状态::已读取 : 服务对象需求发布状态::来源不符;
    }
    std::vector<稳定编码> 集合成员(const 新特征准确值& value) const {
        const auto* n = std::get_if<稳定编码>(&value);
        const auto material = n ? 事实.特征值服务.获取特征值(*n) : std::nullopt;
        const auto* raw = material && material->物理类型 == 材料物理类型::U64数组
            ? std::get_if<std::vector<std::uint64_t>>(&material->材料) : nullptr;
        if (!raw) 发布故障(L"旧需求集合材料损坏");
        std::vector<稳定编码> result;
        for (const auto x : *raw) { if (!x) 发布故障(L"旧需求集合含无效节点"); result.push_back(稳定编码{x}); }
        std::sort(result.begin(), result.end());
        if (std::adjacent_find(result.begin(), result.end()) != result.end()) 发布故障(L"旧需求集合含重复成员");
        return result;
    }
    std::vector<稳定编码> 历史(const 新特征信息& f) const {
        std::vector<稳定编码> result;
        for (const auto& v : f.历史不同值) {
            const auto* d = std::get_if<稳定编码>(&v); if (!d || !有效(*d)) 发布故障(L"需求历史不是合法单节点引用");
            result.push_back(*d);
        }
        std::sort(result.begin(), result.end());
        if (std::adjacent_find(result.begin(), result.end()) != result.end()) 发布故障(L"需求历史身份重复");
        if (f.当前值) {
            const auto* d = std::get_if<稳定编码>(&*f.当前值);
            if (!d || !std::binary_search(result.begin(), result.end(), *d)) 发布故障(L"需求当前值不在历史中");
        } else if (!result.empty() || f.标准值) 发布故障(L"需求无当前值却有历史");
        return result;
    }
    void 核验新关联(稳定编码 C, 稳定编码 E, 稳定编码 F, const 新需求特征发布关联& source) const {
        if (source.场景节点 != C || source.主体存在节点 != E) 发布故障(L"需求发布来源场景或主体不符");
        const auto a = 事实.动态服务.获取动态(source.动作节点);
        const auto ds = 事实.场景服务.查询动态列表(C);
        if (!a || a->种类 != 新动态种类::动作 || a->动作目标种类 != 新动作目标种类::普通存在
            || a->动作目标存在节点 != E || a->动作目标特征节点 != F || !a->动作时间
            || std::count(ds.begin(), ds.end(), source.动作节点) != 1) 发布故障(L"需求发布动作来源不符");
        std::optional<稳定编码> after;
        if (source.首状态节点) {
            if (!a->动作结果组.empty()) 发布故障(L"需求首值伪有变化动态");
            after = source.首状态节点;
        } else {
            if (a->动作结果组.size() != 1 || a->动作结果组.front().类型 != 新动作结果关系类型::导致变化) 发布故障(L"需求发布缺少明确变化来源");
            const auto d = 事实.动态服务.获取动态(a->动作结果组.front().结果动态节点);
            if (!d || d->种类 != 新动态种类::原子 || d->主题存在节点 != E || d->状态节点组.size() != 2
                || std::count(ds.begin(), ds.end(), d->节点) != 1) 发布故障(L"需求变化动态不可读");
            const auto pre = 事实.状态服务.获取状态(d->状态节点组.front());
            if (!pre || pre->被描述存在节点 != E || pre->来源实例特征节点 != F
                || pre->强时间.语义 != 新状态时间语义::实例绝对UTC || a->动作时间->纳秒 <= pre->强时间.纳秒) 发布故障(L"需求前状态来源不符");
            after = d->状态节点组.back();
        }
        const auto s = 事实.状态服务.获取状态(*after);
        const auto ss = 事实.场景服务.查询状态列表(C);
        if (!s || s->被描述存在节点 != E || s->来源实例特征节点 != F
            || s->固定准确值 != 新特征准确值{source.需求节点} || s->强时间.语义 != 新状态时间语义::实例绝对UTC
            || a->动作时间->语义 != 新状态时间语义::实例绝对UTC || s->强时间.纳秒 <= a->动作时间->纳秒
            || std::count(ss.begin(), ss.end(), *after) != 1) 发布故障(L"需求后状态与具体需求不符");
    }

    // 所有旧材料只读；没有唯一完整证据时保留原数据并给出具名缺口。
    服务对象需求发布状态 迁移(稳定编码 C, 稳定编码 E, std::vector<需求特征迁移缺口>& gaps, bool 允许恢复 = false) const {
        using S = 服务对象需求发布状态;
        const auto oldId = 找特征(E, 旧类型); if (!oldId) return S::已读取;
        const auto old = 事实.特征服务.获取特征(*oldId); if (!old) 发布故障(L"旧需求特征不可读");
        std::vector<稳定编码> all;
        std::vector<std::vector<稳定编码>> history;
        for (const auto& v : old->历史不同值) {
            auto members = 集合成员(v); all.insert(all.end(), members.begin(), members.end()); history.push_back(std::move(members));
        }
        std::vector<稳定编码> current;
        if (old->当前值) { current = 集合成员(*old->当前值); all.insert(all.end(), current.begin(), current.end()); }
        std::sort(all.begin(), all.end()); all.erase(std::unique(all.begin(), all.end()), all.end());
        if (all.empty()) return S::已读取;
        const auto gap = [&](const char* why, std::optional<稳定编码> D = {}, std::optional<稳定编码> A = {}, std::optional<稳定编码> state = {}) {
            gaps.push_back({*oldId, D, A, state, why});
        };
        if (!old->当前值) { gap("旧需求特征无当前集合，不能恢复末需求"); return S::旧需求特征待迁移; }
        struct Step { 新需求特征迁移来源项 item; std::vector<稳定编码> before, after; std::int64_t start, end; };
        std::vector<Step> steps;
        for (const auto D : all) {
            const auto d = 需求.获取需求(D);
            if (!d || d->执行域 != 新需求执行域::被认知 || d->需求主体存在节点 != E) 发布故障(L"旧需求集合引用非法需求或异主体");
            const auto origin = 需求.读取主体需求集合发布关联(D);
            if (origin.状态 != 新需求操作状态::已找到) 发布故障(L"旧需求发布来源读取失败");
            if (!origin.关联) { gap("缺少需求原首次发布来源", D); continue; }
            const auto& source = *origin.关联;
            const auto a = 事实.动态服务.获取动态(source.动作节点);
            if (source.主体存在节点 != E || source.场景节点 != C || !a || a->种类 != 新动态种类::动作
                || a->动作目标存在节点 != E || a->动作目标特征节点 != *oldId
                || a->动作结果组.size() != 1 || a->动作结果组.front().类型 != 新动作结果关系类型::导致变化) {
                gap("旧首次发布动作缺少唯一变化结果", D, source.动作节点); continue;
            }
            const auto dynamic = 事实.动态服务.获取动态(a->动作结果组.front().结果动态节点);
            if (!dynamic || dynamic->种类 != 新动态种类::原子 || dynamic->主题存在节点 != E || dynamic->状态节点组.size() != 2) {
                gap("原动作缺少完整两状态变化", D, source.动作节点); continue;
            }
            const auto pre = 事实.状态服务.获取状态(dynamic->状态节点组.front()), post = 事实.状态服务.获取状态(dynamic->状态节点组.back());
            if (!pre || !post || pre->来源实例特征节点 != *oldId || post->来源实例特征节点 != *oldId
                || pre->被描述存在节点 != E || post->被描述存在节点 != E) { gap("原前后状态来源不完整", D, source.动作节点); continue; }
            auto before = 集合成员(pre->固定准确值), after = 集合成员(post->固定准确值), expected = before;
            if (std::binary_search(before.begin(), before.end(), D)) { gap("需求在首次发布前已存在，命中次数不可恢复", D, source.动作节点); continue; }
            expected.push_back(D); std::sort(expected.begin(), expected.end());
            if (after != expected || pre->强时间.语义 != 新状态时间语义::实例绝对UTC
                || post->强时间.语义 != 新状态时间语义::实例绝对UTC || pre->强时间.纳秒 >= post->强时间.纳秒) {
                gap("原变化不是唯一追加该需求或时间非法", D, source.动作节点); continue;
            }
            steps.push_back({{D, source.动作节点, pre->节点, post->节点}, std::move(before), std::move(after), pre->强时间.纳秒, post->强时间.纳秒});
        }
        if (!gaps.empty()) return S::旧需求特征待迁移;
        std::vector<新需求特征迁移来源项> ordered;
        std::vector<稳定编码> prefix;
        std::optional<std::int64_t> time;
        std::set<稳定编码> used;
        while (ordered.size() < steps.size()) {
            const Step* selected = nullptr;
            for (const auto& step : steps) if (!used.contains(step.item.需求节点) && step.before == prefix) {
                if (selected) { gap("旧追加链分叉，不能确定需求时序"); return S::旧需求特征待迁移; }
                selected = &step;
            }
            if (!selected || (time && selected->start < *time)) { gap("旧追加链断裂或时间冲突"); return S::旧需求特征待迁移; }
            used.insert(selected->item.需求节点); ordered.push_back(selected->item); prefix = selected->after; time = selected->end;
        }
        if (prefix != current) { gap("旧追加链末集合不等于当前集合"); return S::旧需求特征待迁移; }
        for (const auto& h : history) {
            const bool empty = h.empty();
            if (!empty && std::none_of(steps.begin(), steps.end(), [&](const auto& step) { return step.after == h; })) {
                gap("历史集合不属于追加链前缀"); return S::旧需求特征待迁移;
            }
        }
        const auto found = 找特征(E, 类型);
        if (!found && !允许恢复) { gap("旧需求需要显式迁移，读取不建立新需求特征"); return S::旧需求特征待迁移; }
        const auto F = found ? *found : 建空特征(E);
        新需求特征事实核验输入 input{新需求特征事实用途::旧集合迁移, C, E, *oldId, F, ordered.back().需求节点, {}, {}, ordered};
        auto proof = 世界.核验需求特征发布事实(input);
        if (proof.状态 == 新需求特征事实核验状态::前提已变化) return S::前提已变化;
        if (proof.状态 != 新需求特征事实核验状态::已核验 || !proof.凭据) { gap("世界未能核验完整旧集合动作状态链"); return S::旧需求特征待迁移; }
        const auto existing = 需求.读取需求特征迁移(F);
        if (existing.状态 != 新需求操作状态::已找到) 发布故障(L"需求迁移映射读取失败");
        if (existing.信息) {
            const auto& m = *existing.信息;
            if (m.主体存在节点 != E || m.场景节点 != C || m.原特征节点 != *oldId || m.新特征节点 != F
                || m.有序来源组 != ordered || m.当前需求节点 != ordered.back().需求节点) 发布故障(L"已有需求迁移映射异义");
            if (!m.迁移当前状态节点) 发布故障(L"需求迁移核心已发表但无直接派生回执，需工程恢复");
            const auto f = 事实.特征服务.获取特征(F); if (!f) 发布故障(L"迁移后需求特征不可读");
            const auto ds = 历史(*f);
            for (const auto D : all) if (!std::binary_search(ds.begin(), ds.end(), D)) 发布故障(L"已迁移需求不在新特征历史中");
            return S::已读取;
        }
        if (!允许恢复) { gap("旧需求尚无已完成迁移映射，读取不执行历史恢复"); return S::旧需求特征待迁移; }
        auto mapping = 需求.准备需求特征迁移(*proof.凭据);
        if (mapping.状态 == 新需求操作状态::前提已变化) return S::前提已变化;
        if (mapping.状态 != 新需求操作状态::已建立 || !mapping.准备) 发布故障(L"需求迁移来源映射准备失败");
        新特征需求历史恢复请求 restore;
        restore.特征节点 = F; restore.当前需求节点 = ordered.back().需求节点; restore.标准需求节点 = ordered.front().需求节点;
        restore.来源保护组 = proof.凭据->依据().完整保护组;
        for (const auto& item : ordered) restore.历史值命中信息.push_back({新特征准确值{item.需求节点}, 1});
        auto prepared = 事实.特征服务.准备顶层需求引用历史恢复(restore);
        if (prepared.状态 == 新特征I64准备状态::预期已变化 || prepared.状态 == 新特征I64准备状态::联合更新占用) return S::前提已变化;
        if (prepared.状态 != 新特征I64准备状态::已准备 || !prepared.准备) 发布故障(L"需求单引用历史恢复准备失败");
        const auto commit = 事实.特征服务.提交顶层需求引用更新(*prepared.准备, {mapping.准备->片段()});
        if (commit.状态 == 基础节点组追加状态::预期不符) { 事实.特征服务.取消顶层需求引用更新(*prepared.准备); return S::前提已变化; }
        if (commit.状态 != 基础节点组追加状态::已完成) 发布故障(L"需求历史恢复与映射联合提交失败");
        std::optional<基础片段发布回执> receipt;
        for (const auto& x : commit.附加片段回执组) if (x.对应片段(mapping.准备->片段())) {
            if (receipt) 发布故障(L"需求迁移映射回执重复"); receipt = x;
        }
        if (!receipt) 发布故障(L"需求迁移映射回执缺失");
        需求.完成需求特征迁移核心(*mapping.准备, *receipt);
        const auto restored = 事实.特征服务.完成顶层需求引用历史恢复(*prepared.准备);
        if (restored.状态 != 新特征写入状态::已完成 || restored.命中次数已增加) 发布故障(L"需求历史恢复完成不符");
        auto fresh = 世界.核验需求特征发布事实(input);
        if (fresh.状态 != 新需求特征事实核验状态::已核验 || !fresh.凭据) 发布故障(L"需求迁移核心已发表但旧链重核验失败");
        auto derive = 需求.准备需求特征迁移派生(F, *fresh.凭据);
        if (derive.状态 != 新需求操作状态::已建立 || !derive.凭据) 发布故障(L"需求迁移派生来源准备失败");
        const auto changed = 世界.完成需求引用迁移派生(*derive.凭据, *prepared.准备);
        if (!changed.新状态节点 || changed.状态 != 新世界特征变化状态::已生成状态但无可连接前状态) 发布故障(L"需求迁移状态派生失败");
        input.用途 = 新需求特征事实用途::迁移当前状态; input.实际状态节点 = changed.新状态节点;
        auto stateProof = 世界.核验需求特征发布事实(input);
        if (stateProof.状态 != 新需求特征事实核验状态::已核验 || !stateProof.凭据) 发布故障(L"需求迁移实际状态核验失败");
        const auto done = 需求.完成需求特征迁移状态(F, *stateProof.凭据);
        if (done != 新需求操作状态::已建立 && done != 新需求操作状态::已复用) 发布故障(L"需求迁移状态映射完成失败");
        const auto map = 需求.读取需求特征迁移(F); const auto f = 事实.特征服务.获取特征(F);
        if (!map.信息 || map.信息->迁移当前状态节点 != changed.新状态节点 || !f
            || f->当前值 != std::optional<新特征准确值>{新特征准确值{ordered.back().需求节点}}
            || f->标准值 != std::optional<新特征准确值>{新特征准确值{ordered.front().需求节点}}
            || 历史(*f) != all || f->历史值命中信息.size() != all.size()) 发布故障(L"需求迁移最终完整读回失败");
        for (const auto& hit : f->历史值命中信息) if (hit.命中次数 != 1) 发布故障(L"需求迁移命中次数不符");
        事实.特征服务.结束顶层需求引用更新(*prepared.准备);
        return S::已读取;
    }

    服务对象需求读取结果 读取(稳定编码 C, 稳定编码 E) const {
        using S = 服务对象需求发布状态;
        服务对象需求读取结果 out;
        if (!场景有效(C, E)) return out;
        out.状态 = 迁移(C, E, out.迁移缺口组); if (out.状态 != S::已读取) return out;
        out.需求特征节点 = 找特征(E, 类型); if (!out.需求特征节点) return out;
        const auto F = *out.需求特征节点;
        const auto f = 事实.特征服务.获取特征(F); if (!f) 发布故障(L"需求特征不可读");
        out.历史需求节点组 = 历史(*f);
        if (f->当前值) out.当前需求节点 = std::get<稳定编码>(*f->当前值);
        const auto mapping = 需求.读取需求特征迁移(F);
        if (mapping.状态 != 新需求操作状态::已找到) 发布故障(L"读取需求迁移映射失败");
        for (const auto n : out.历史需求节点组) {
            const auto d = 需求.获取需求(n);
            if (!d || d->执行域 != 新需求执行域::被认知 || d->需求主体存在节点 != E) 发布故障(L"需求历史含非法或异主体需求");
            const auto source = 需求.读取主体需求特征发布关联(n);
            if (source.状态 != 新需求操作状态::已找到 || !source.关联) 发布故障(L"需求历史缺少首次发布来源");
            const auto oldItem = mapping.信息 ? std::find_if(mapping.信息->有序来源组.begin(), mapping.信息->有序来源组.end(), [&](const auto& x) { return x.需求节点 == n; }) : std::vector<新需求特征迁移来源项>::const_iterator{};
            if (mapping.信息 && oldItem != mapping.信息->有序来源组.end()) {
                if (!mapping.信息->迁移当前状态节点 || oldItem->动作节点 != source.关联->动作节点
                    || source.关联->主体存在节点 != E || source.关联->场景节点 != C) 发布故障(L"迁移需求原发布来源与映射不符");
            } else 核验新关联(C, E, F, *source.关联);
            const auto status = 确认(*d);
            if (status == S::需重新确认) out.需重新确认节点组.push_back(n);
            else if (status != S::已读取) 发布故障(L"需求历史确认来源损坏");
        }
        if (事实.特征服务.获取特征(F) != f) out.状态 = S::前提已变化;
        return out;
    }
};

服务对象需求发布服务::服务对象需求发布服务(新_世界树类& w, 新_需求类& d) noexcept {
    try { 实现_ = std::make_unique<实现>(w, d); } catch (...) { 发布故障(L"构造需求发布服务失败"); }
}
服务对象需求发布服务::~服务对象需求发布服务() { 关闭(); }
bool 服务对象需求发布服务::初始化(稳定编码 T, 稳定编码 oldT) noexcept {
    try {
        const auto innate = 实现_->世界.获取先天特征概念();
        if (!innate || T != innate->需求 || oldT != innate->服务对象需求集合 || T == oldT) return false;
        {
            std::lock_guard lock(实现_->互斥);
            if (实现_->就绪) return 实现_->类型 == T && 实现_->旧类型 == oldT;
            实现_->类型 = T; 实现_->旧类型 = oldT;
        }
        // 初始化是明确迁移入口；所有owner调用均在运行期占用锁外。
        const auto root = 实现_->世界.获取世界树根场景();
        if (!root) return false;
        const auto 范围 = 实现_->事实.场景服务.读取指定场景范围完整(*root);
        if (范围.状态 == 新场景范围成员读取状态::读取已变化) return false;
        if (范围.状态 != 新场景范围成员读取状态::已完成)
            发布故障(L"初始化迁移场景范围未能完整读取，不能当作空范围");
        for (const auto& 场景项 : 范围.场景成员组) {
            const auto C = 场景项.场景节点;
            auto members = 场景项.直接成员存在组;
            members.push_back(C);
            std::sort(members.begin(), members.end()); members.erase(std::unique(members.begin(), members.end()), members.end());
            for (const auto E : members) {
                if (!实现_->找特征(E, oldT)) continue;
                {
                    std::lock_guard lock(实现_->互斥);
                    if (!实现_->占用.insert(E).second) return false;
                }
                实现::调用结束 release{*实现_, E};
                std::vector<需求特征迁移缺口> gaps;
                const auto state = 实现_->迁移(C, E, gaps, true);
                if (state != 服务对象需求发布状态::已读取 && state != 服务对象需求发布状态::旧需求特征待迁移)
                    return false;
                if (state == 服务对象需求发布状态::旧需求特征待迁移)
                    for (const auto& 缺口 : gaps) 记录迁移缺口(C, E, 缺口);
                // 不可还原材料原样保留，已在初始化具名报告；后续正式读取仍返回完整缺口。
            }
        }
        std::lock_guard lock(实现_->互斥); 实现_->就绪 = true; return true;
    } catch (...) { 发布故障(L"初始化需求发布服务失败"); }
}
void 服务对象需求发布服务::关闭() noexcept {
    try {
        if (!实现_) return;
        std::unique_lock lock(实现_->互斥); 实现_->就绪 = false;
        实现_->等待.wait(lock, [&] { return 实现_->占用.empty(); });
    } catch (...) { 发布故障(L"关闭需求发布服务失败"); }
}
服务对象需求读取结果 服务对象需求发布服务::读取主体需求(稳定编码 C, 稳定编码 E) const noexcept {
    try {
        const auto s = 实现_->取得(E);
        if (s != 服务对象需求发布状态::已读取) { 服务对象需求读取结果 r; r.状态 = s; return r; }
        实现::调用结束 end{*实现_, E}; return 实现_->读取(C, E);
    } catch (...) { 发布故障(L"读取主体需求工程异常"); }
}
服务对象需求发布结果 服务对象需求发布服务::发布已确认需求(const 服务对象需求发布请求& q) noexcept {
    try {
        using S = 服务对象需求发布状态;
        服务对象需求发布结果 out; out.状态 = 实现_->取得(q.服务对象节点);
        if (out.状态 != S::已读取) return out;
        实现::调用结束 end{*实现_, q.服务对象节点};
        const auto d = 实现_->需求.获取需求(q.需求节点);
        if (!d) { out.状态 = S::需求不存在; return out; }
        if (d->执行域 != 新需求执行域::被认知) { out.状态 = S::非被认知需求; return out; }
        if (d->需求主体存在节点 != q.服务对象节点) { out.状态 = S::主体不符; return out; }
        out.状态 = 实现_->确认(*d); if (out.状态 != S::已读取) return out;
        if (!实现_->场景有效(q.场景节点, q.服务对象节点)) { out.状态 = S::来源不符; return out; }
        out.状态 = 实现_->迁移(q.场景节点, q.服务对象节点, out.迁移缺口组, true);
        if (out.状态 != S::已读取) return out;
        auto read = 实现_->读取(q.场景节点, q.服务对象节点);
        out.迁移缺口组 = read.迁移缺口组;
        if (read.状态 != S::已读取) { out.状态 = read.状态; return out; }
        const auto F = read.需求特征节点.value_or(稳定编码{});
        const auto target = 有效(F) ? F : 实现_->建空特征(q.服务对象节点);
        out.需求特征节点 = target; out.需求节点 = q.需求节点;
        const auto origin = 实现_->需求.读取主体需求特征发布关联(q.需求节点);
        if (origin.状态 != 新需求操作状态::已找到) 发布故障(L"需求发布来源读取失败");
        const bool present = std::binary_search(read.历史需求节点组.begin(), read.历史需求节点组.end(), q.需求节点);
        if (present != origin.关联.has_value()) 发布故障(L"需求历史与发布来源不一致");
        if (present) { out.状态 = S::已复用; out.动作节点 = origin.关联->动作节点; return out; }
        const auto basis = 实现_->世界.读取需求引用更新依据(q.场景节点, q.服务对象节点, target);
        if (basis.状态 == 新世界需求引用组合状态::前提已变化) { out.状态 = S::前提已变化; return out; }
        if (basis.状态 != 新世界需求引用组合状态::已读取 || !basis.依据) 发布故障(L"需求发布依据不可读");
        auto prepared = 实现_->世界.准备需求引用改变({*basis.依据, q.需求节点, q.动作定义节点, q.动作来源节点, q.执行主体节点, {}});
        out.动作节点 = prepared.已发生动作节点;
        if (prepared.状态 == 新世界需求引用组合状态::前提已变化) { out.状态 = S::前提已变化; return out; }
        if (prepared.状态 == 新世界需求引用组合状态::目标不合法) { out.状态 = S::来源不符; return out; }
        if (prepared.状态 != 新世界需求引用组合状态::已准备 || !prepared.准备 || !out.动作节点) 发布故障(L"需求发布准备失败");
        const auto material = 实现_->世界.读取需求引用组合准备(*prepared.准备);
        auto source = 实现_->需求.准备主体需求特征发布(q.需求节点, q.服务对象节点, target, q.场景节点, *out.动作节点, material.特征保护);
        if (source.状态 == 新需求操作状态::需重新确认) { out.状态 = S::需重新确认; return out; }
        if (source.状态 == 新需求操作状态::前提已变化 || source.状态 == 新需求操作状态::已复用) { out.状态 = S::前提已变化; return out; }
        if (source.状态 != 新需求操作状态::已建立 || !source.准备) 发布故障(L"需求发布来源准备失败");
        const auto published = 实现_->世界.提交需求引用改变并形成状态动态(*prepared.准备, source.准备->片段());
        if (published.状态 == 新世界需求引用组合状态::前提已变化) { out.状态 = S::前提已变化; return out; }
        if (published.状态 != 新世界需求引用组合状态::已完成 || !published.变化 || !published.附加片段回执 || !published.变化->新状态节点) 发布故障(L"需求联合发布未完成");
        out.变化 = published.变化;
        实现_->需求.完成主体需求特征发布(*source.准备, *published.附加片段回执);
        if (!published.变化->前状态节点) {
            新需求特征事实核验输入 input{新需求特征事实用途::首次状态, q.场景节点, q.服务对象节点, {}, target,
                q.需求节点, out.动作节点, published.变化->新状态节点, {}};
            auto proof = 实现_->世界.核验需求特征发布事实(input);
            if (proof.状态 != 新需求特征事实核验状态::已核验 || !proof.凭据) 发布故障(L"需求首状态实际回执核验失败");
            const auto status = 实现_->需求.完成主体需求首状态关联(q.需求节点, *proof.凭据);
            if (status != 新需求操作状态::已建立 && status != 新需求操作状态::已复用) 发布故障(L"需求首状态来源关联失败");
        }
        const auto final = 实现_->读取(q.场景节点, q.服务对象节点);
        if (final.状态 == S::前提已变化) { out.状态 = S::已发布但当前读回变化; return out; }
        if (final.状态 != S::已读取 || final.需求特征节点 != target
            || !std::binary_search(final.历史需求节点组.begin(), final.历史需求节点组.end(), q.需求节点)) 发布故障(L"需求发布最终来源和历史读回失败");
        out.状态 = final.当前需求节点 != q.需求节点
            || std::find(final.需重新确认节点组.begin(), final.需重新确认节点组.end(), q.需求节点) != final.需重新确认节点组.end()
            ? S::已发布但当前读回变化 : S::已发布;
        return out;
    } catch (...) { 发布故障(L"发布已确认需求工程异常"); }
}
}
