#include "计算.二次关系求值.h"

#include <algorithm>
#include <new>
#include <set>
#include <stdexcept>
#include <type_traits>

namespace 海中鱼巣 {
namespace {

struct 求值失败 final {
  二次关系判断状态 状态;
};

[[noreturn]] void 失败(二次关系判断状态 状态) { throw 求值失败{状态}; }

二次关系判断状态 映射特征错误(特征数据错误 状态) noexcept {
  switch (状态) {
  case 特征数据错误::未找到:
  case 特征数据错误::未设置:
    return 二次关系判断状态::证据不足;
  case 特征数据错误::已退出:
  case 特征数据错误::历史材料不可用:
    return 二次关系判断状态::历史材料不可用;
  case 特征数据错误::并发变化:
    return 二次关系判断状态::事实代次漂移;
  case 特征数据错误::资源失败:
    return 二次关系判断状态::资源失败;
  case 特征数据错误::类型不相容:
    return 二次关系判断状态::类型不相容;
  case 特征数据错误::能力未提供:
  case 特征数据错误::规则缺失:
  case 特征数据错误::旧格式不支持:
    return 二次关系判断状态::规则不支持;
  default:
    return 二次关系判断状态::内部不一致;
  }
}

二次关系判断状态 映射采用状态(存在当前采用完整读取状态_v2 状态) noexcept {
  switch (状态) {
  case 存在当前采用完整读取状态_v2::未找到:
    return 二次关系判断状态::证据不足;
  case 存在当前采用完整读取状态_v2::目标已退出:
  case 存在当前采用完整读取状态_v2::历史材料不可用:
    return 二次关系判断状态::历史材料不可用;
  case 存在当前采用完整读取状态_v2::事实代次漂移:
    return 二次关系判断状态::事实代次漂移;
  case 存在当前采用完整读取状态_v2::资源失败:
    return 二次关系判断状态::资源失败;
  case 存在当前采用完整读取状态_v2::入口拒绝:
    return 二次关系判断状态::入口拒绝;
  default:
    return 二次关系判断状态::内部不一致;
  }
}

二次关系判断状态 映射存在状态(存在结构身份只读状态 状态) noexcept {
  switch (状态) {
  case 存在结构身份只读状态::未找到:
    return 二次关系判断状态::证据不足;
  case 存在结构身份只读状态::目标已退出:
  case 存在结构身份只读状态::历史材料已清理:
    return 二次关系判断状态::历史材料不可用;
  case 存在结构身份只读状态::事实代次漂移:
    return 二次关系判断状态::事实代次漂移;
  case 存在结构身份只读状态::资源失败:
    return 二次关系判断状态::资源失败;
  case 存在结构身份只读状态::入口拒绝:
    return 二次关系判断状态::入口拒绝;
  case 存在结构身份只读状态::未实现:
    return 二次关系判断状态::规则不支持;
  default:
    return 二次关系判断状态::内部不一致;
  }
}

二次关系判断状态 映射绑定状态(状态使用绑定数据状态 状态) noexcept {
  switch (状态) {
  case 状态使用绑定数据状态::未找到:
  case 状态使用绑定数据状态::场景未找到:
  case 状态使用绑定数据状态::存在未找到:
  case 状态使用绑定数据状态::状态未找到:
    return 二次关系判断状态::证据不足;
  case 状态使用绑定数据状态::目标已退出:
  case 状态使用绑定数据状态::场景已退出:
  case 状态使用绑定数据状态::存在已退出:
  case 状态使用绑定数据状态::状态已退出:
  case 状态使用绑定数据状态::历史材料已清理:
    return 二次关系判断状态::历史材料不可用;
  case 状态使用绑定数据状态::事实代次漂移:
    return 二次关系判断状态::事实代次漂移;
  case 状态使用绑定数据状态::资源失败:
    return 二次关系判断状态::资源失败;
  case 状态使用绑定数据状态::未实现:
    return 二次关系判断状态::规则不支持;
  case 状态使用绑定数据状态::入口拒绝:
    return 二次关系判断状态::入口拒绝;
  default:
    return 二次关系判断状态::内部不一致;
  }
}

二次关系判断状态 映射状态读取(状态类数据状态 状态) noexcept {
  switch (状态) {
  case 状态类数据状态::未找到:
  case 状态类数据状态::正式特征类型未找到:
    return 二次关系判断状态::证据不足;
  case 状态类数据状态::已退出:
  case 状态类数据状态::已清理:
  case 状态类数据状态::目标已退出:
  case 状态类数据状态::正式特征类型已退出:
  case 状态类数据状态::历史材料已清理:
    return 二次关系判断状态::历史材料不可用;
  case 状态类数据状态::事实代次漂移:
    return 二次关系判断状态::事实代次漂移;
  case 状态类数据状态::资源失败:
    return 二次关系判断状态::资源失败;
  case 状态类数据状态::准确值不相容:
    return 二次关系判断状态::类型不相容;
  case 状态类数据状态::旧格式不支持:
    return 二次关系判断状态::规则不支持;
  case 状态类数据状态::入口拒绝:
    return 二次关系判断状态::入口拒绝;
  default:
    return 二次关系判断状态::内部不一致;
  }
}

二次关系判断状态 映射解析状态(特征正式准确I64解析状态_v2 状态) noexcept {
  switch (状态) {
  case 特征正式准确I64解析状态_v2::正式特征类型未找到:
  case 特征正式准确I64解析状态_v2::准确值未找到:
    return 二次关系判断状态::证据不足;
  case 特征正式准确I64解析状态_v2::正式特征类型已退出:
  case 特征正式准确I64解析状态_v2::准确值已退出:
  case 特征正式准确I64解析状态_v2::历史材料不可用:
    return 二次关系判断状态::历史材料不可用;
  case 特征正式准确I64解析状态_v2::准确值不相容:
    return 二次关系判断状态::类型不相容;
  case 特征正式准确I64解析状态_v2::非I64:
    return 二次关系判断状态::规则不支持;
  case 特征正式准确I64解析状态_v2::事实代次漂移:
    return 二次关系判断状态::事实代次漂移;
  case 特征正式准确I64解析状态_v2::资源失败:
    return 二次关系判断状态::资源失败;
  case 特征正式准确I64解析状态_v2::入口拒绝:
    return 二次关系判断状态::入口拒绝;
  default:
    return 二次关系判断状态::内部不一致;
  }
}

二次关系判断状态 映射概念状态(二次关系数据状态 状态) noexcept {
  switch (状态) {
  case 二次关系数据状态::未找到:
  case 二次关系数据状态::同义缺失:
  case 二次关系数据状态::来源不足:
    return 二次关系判断状态::证据不足;
  case 二次关系数据状态::目标已退出:
  case 二次关系数据状态::历史材料不可用:
    return 二次关系判断状态::历史材料不可用;
  case 二次关系数据状态::事实代次漂移:
    return 二次关系判断状态::事实代次漂移;
  case 二次关系数据状态::资源失败:
    return 二次关系判断状态::资源失败;
  case 二次关系数据状态::退役命中:
    return 二次关系判断状态::退役不可用;
  case 二次关系数据状态::格式不支持:
  case 二次关系数据状态::规则不支持:
    return 二次关系判断状态::规则不支持;
  case 二次关系数据状态::旧预算不足:
    return 二次关系判断状态::旧预算不足;
  case 二次关系数据状态::入口拒绝:
    return 二次关系判断状态::入口拒绝;
  default:
    return 二次关系判断状态::内部不一致;
  }
}

二次关系判断状态 映射固定K状态(
    特征I64比较绑定读取状态_v2 状态) noexcept {
  switch (状态) {
  case 特征I64比较绑定读取状态_v2::未找到:
    return 二次关系判断状态::证据不足;
  case 特征I64比较绑定读取状态_v2::目标已退出:
  case 特征I64比较绑定读取状态_v2::历史材料不可用:
    return 二次关系判断状态::历史材料不可用;
  case 特征I64比较绑定读取状态_v2::事实代次漂移:
    return 二次关系判断状态::事实代次漂移;
  case 特征I64比较绑定读取状态_v2::资源失败:
    return 二次关系判断状态::资源失败;
  case 特征I64比较绑定读取状态_v2::格式不支持:
    return 二次关系判断状态::规则不支持;
  case 特征I64比较绑定读取状态_v2::入口拒绝:
    return 二次关系判断状态::入口拒绝;
  default:
    return 二次关系判断状态::内部不一致;
  }
}

二次关系判断状态 映射当前K状态(
    特征I64比较绑定读取状态_v2 状态) noexcept {
  if (状态 == 特征I64比较绑定读取状态_v2::未找到)
    return 二次关系判断状态::规则不支持;
  return 映射固定K状态(状态);
}

bool 命中I64域(const std::vector<概念树I64区间> &域, std::int64_t 值) {
  if (域.empty()) 失败(二次关系判断状态::规则不支持);
  bool 命中 = false;
  std::optional<std::int64_t> 前上界;
  for (const auto &项 : 域) {
    if (项.下界 > 项.上界 || (前上界 && 项.下界 <= *前上界))
      失败(二次关系判断状态::内部不一致);
    if (值 >= 项.下界 && 值 <= 项.上界) 命中 = true;
    前上界 = 项.上界;
  }
  return 命中;
}

bool 见证形状有效(const 二次关系准确来源见证 &v,
                  std::uint64_t g, std::uint64_t h) noexcept {
  if (!有效(v.E) || !有效(v.FT) || v.存在见证.身份 != v.E)
    return false;
  if (const auto *f = std::get_if<准确特征读取事实>(&v.内容)) {
    const auto *source = std::get_if<二次关系准确F来源>(&v.来源);
    return source && f->Gread == g && f->H == h && f->信息.身份 == source->F
        && f->信息.类型 == v.FT && v.采用 && v.采用->E == v.E
        && v.采用->FT == v.FT && v.采用->F == source->F && !v.绑定;
  }
  const auto *s = std::get_if<状态内容事实>(&v.内容);
  const auto *source = std::get_if<二次关系状态端点来源>(&v.来源);
  return s && source && s->Gread == g && s->H == h && s->信息.正式特征类型 == v.FT
      && v.绑定 && v.绑定->信息.身份 == source->B
      && v.绑定->信息.被描述存在 == v.E
      && v.绑定->信息.状态 == s->信息.身份 && !v.采用;
}

void 校验参与者形状(const 二次关系参与者材料 &参与者) {
  if (!有效(参与者.E) || 参与者.来源组.empty())
    失败(二次关系判断状态::入口拒绝);
  const auto branch = 参与者.来源组.front().index();
  std::set<std::uint64_t> identities;
  for (const auto &source : 参与者.来源组) {
    if (source.index() != branch)
      失败(二次关系判断状态::入口拒绝);
    const auto identity = std::visit([](const auto &item) {
      if constexpr (std::is_same_v<std::decay_t<decltype(item)>,
                                   二次关系准确F来源>)
        return item.F.编码.值;
      else
        return item.B.编码.值;
    }, source);
    if (!identity || !identities.insert(identity).second)
      失败(二次关系判断状态::入口拒绝);
  }
}

bool 来源对应(const 二次关系准确来源见证 &witness,
              稳定编码 E, const 二次关系特征来源 &source) noexcept {
  return witness.E == E && witness.来源 == source;
}

bool 来源段对应(const std::vector<二次关系准确来源见证> &sources,
                std::size_t offset,
                const 二次关系参与者材料 &participant) noexcept {
  if (offset > sources.size()
      || participant.来源组.size() > sources.size() - offset)
    return false;
  for (std::size_t i = 0; i < participant.来源组.size(); ++i)
    if (!来源对应(sources[offset + i], participant.E, participant.来源组[i]))
      return false;
  return true;
}

bool 包含来源见证(const std::vector<二次关系准确来源见证> &sources,
                  const 二次关系准确来源见证 &wanted,
                  std::uint64_t g, std::uint64_t h) noexcept {
  return std::any_of(sources.begin(), sources.end(), [&](const auto &item) {
    return item.E == wanted.E && item.来源 == wanted.来源
        && item.FT == wanted.FT && item.值 == wanted.值
        && 见证形状有效(item, g, h);
  });
}

void 核验共同结果(const 二次关系判断结果 &out,
                  std::uint64_t g, std::uint64_t h,
                  std::uint64_t request) {
  if (out.版本 != 2 || (out.状态 != 二次关系判断状态::命中
                        && out.状态 != 二次关系判断状态::不命中)
      || out.Gread != g || out.H != h || out.定义H != g
      || out.请求身份 != request || !g || !h || h > g
      || out.来源组.empty())
    失败(二次关系判断状态::内部不一致);
  for (const auto &source : out.来源组)
    if (!见证形状有效(source, g, h))
      失败(二次关系判断状态::内部不一致);
  std::set<std::uint64_t> concepts;
  for (const auto &conceptIdentity : out.已核验概念)
    if (!有效(conceptIdentity.值)
        || !concepts.insert(conceptIdentity.值.值).second)
      失败(二次关系判断状态::内部不一致);
}

void 核验二参与者来源(const 二次关系判断结果 &out,
                      const 二次关系参与者材料 &a,
                      const 二次关系参与者材料 &b) {
  if (out.来源组.size() != a.来源组.size() + b.来源组.size()
      || !来源段对应(out.来源组, 0, a)
      || !来源段对应(out.来源组, a.来源组.size(), b))
    失败(二次关系判断状态::内部不一致);
}

void 核验单参与者来源(const 二次关系判断结果 &out,
                      const 二次关系参与者材料 &participant) {
  if (out.来源组.size() != participant.来源组.size()
      || !来源段对应(out.来源组, 0, participant))
    失败(二次关系判断状态::内部不一致);
}

void 核验条件结果(const 二次关系判断结果 &out,
                  const std::vector<二次关系原子定义> &atoms) {
  if (atoms.empty() || out.条件组.size() != atoms.size())
    失败(二次关系判断状态::内部不一致);
  bool allHit = true;
  for (std::size_t i = 0; i < atoms.size(); ++i) {
    const auto &condition = out.条件组[i];
    if (condition.定义 != atoms[i] || condition.K.身份 != condition.定义.K
        || !I64绑定事实完整(condition.K, out.Gread)
        || !condition.计算.成功()
        || condition.计算.请求身份 != out.请求身份
        || condition.计算.K != condition.K.身份
        || condition.计算.实际结果位
               != static_cast<std::uint8_t>(特征比较结果要求::差异材料)
        || condition.A.FT != condition.K.定义.输入FT
        || condition.B.FT != condition.K.定义.输入FT
        || !见证形状有效(condition.A, out.Gread, out.H)
        || !见证形状有效(condition.B, out.Gread, out.H)
        || !包含来源见证(out.来源组, condition.A, out.Gread, out.H)
        || !包含来源见证(out.来源组, condition.B, out.Gread, out.H))
      失败(二次关系判断状态::内部不一致);
    const 特征比较执行输出 *difference = nullptr;
    for (const auto &output : condition.计算.输出组)
      if (output.角色 == 特征类标量结果角色::差异) {
        if (difference)
          失败(二次关系判断状态::内部不一致);
        difference = &output;
      }
    if (!difference)
      失败(二次关系判断状态::内部不一致);
    const std::uint8_t bit = difference->值 < 0 ? 1 : difference->值 == 0 ? 2 : 4;
    if (condition.域命中 != ((condition.定义.D.掩码 & bit) != 0))
      失败(二次关系判断状态::内部不一致);
    allHit = condition.FC命中 && condition.EC命中
             && condition.域命中 && allHit;
  }
  if ((out.状态 == 二次关系判断状态::命中) != allHit)
    失败(二次关系判断状态::内部不一致);
}

void 核验概念结果(const 二次关系判断结果 &out,
                  std::uint64_t g, std::uint64_t h,
                  std::uint64_t request, 概念树概念身份 requestedConcept,
                  const 二次关系参与者材料 &participant, bool hit) {
  核验共同结果(out, g, h, request);
  核验单参与者来源(out, participant);
  if (!out.条件组.empty()
      || std::none_of(out.已核验概念.begin(), out.已核验概念.end(),
                      [&](const auto &item) { return item == requestedConcept; })
      || ((out.状态 == 二次关系判断状态::命中) != hit))
    失败(二次关系判断状态::内部不一致);
}

class 求值上下文 final {
public:
  求值上下文(const 概念树类数据服务 &概念,
             const 状态使用绑定只读提供者 &绑定,
             const 状态类数据服务 &状态,
             const 存在类数据服务 &存在,
             const 特征类数据服务 &特征,
             const 有序I64特征比较提供者 &比较,
             std::uint64_t g, std::uint64_t h, std::uint64_t request)
      : 概念_(概念), 绑定_(绑定), 状态_(状态), 存在_(存在), 特征_(特征),
        比较_(比较), g_(g), h_(h), 请求身份_(request) {}

  void 预读FC(概念树概念身份 fc) {
    const auto result = 概念_.读取二次关系约束定义({2, {1, g_, g_}, fc});
    if (!result.成功()) 失败(映射概念状态(result.状态));
    if (result.Gread != g_ || result.H != g_ || !result.定义)
      失败(二次关系判断状态::内部不一致);
    const auto *pure = std::get_if<纯概念事实>(&*result.定义);
    if (!pure || pure->概念 != fc || pure->类别 != 相关概念类别::特征
        || pure->治理状态 == 概念树生命周期状态::退役
        || !std::holds_alternative<纯I64特征概念定义>(pure->定义))
      失败(二次关系判断状态::规则不支持);
  }

  void 预读EC(概念树概念身份 ec) {
    const auto result = 概念_.读取二次关系约束定义({2, {1, g_, g_}, ec});
    if (!result.成功()) 失败(映射概念状态(result.状态));
    if (result.Gread != g_ || result.H != g_ || !result.定义)
      失败(二次关系判断状态::内部不一致);
    if (const auto *pure = std::get_if<纯概念事实>(&*result.定义)) {
      const auto *generic = std::get_if<通用存在概念定义>(&pure->定义);
      if (!generic || pure->概念 != ec || pure->类别 != 相关概念类别::存在
          || generic->规则版本 != 1
          || generic->规则 != 通用存在定义规则::不预设特征
          || !pure->直接上位.empty()
          || pure->治理状态 == 概念树生命周期状态::退役)
        失败(二次关系判断状态::规则不支持);
      return;
    }
    const auto *specific = std::get_if<存在概念两组事实_v3>(&*result.定义);
    if (!specific || specific->概念 != ec
        || specific->治理状态 == 概念树生命周期状态::退役
        || !specific->定义.自身特征组已完整声明
        || !specific->定义.子存在概念组已完整声明)
      失败(二次关系判断状态::规则不支持);
    if (!specific->定义.已知子存在概念组.empty())
      失败(二次关系判断状态::规则不支持);
  }

  std::vector<二次关系准确来源见证> 解析参与者(
      const 二次关系参与者材料 &参与者) const {
    if (!有效(参与者.E) || 参与者.来源组.empty())
      失败(二次关系判断状态::入口拒绝);
    const auto branch = 参与者.来源组.front().index();
    std::set<std::uint64_t> sourceIds;
    std::set<std::uint64_t> featureTypes;
    std::vector<二次关系准确来源见证> out;
    std::optional<稳定编码> commonScene;
    std::optional<状态强时间> commonTime;
    out.reserve(参与者.来源组.size());
    for (const auto &source : 参与者.来源组) {
      if (source.index() != branch) 失败(二次关系判断状态::入口拒绝);
      const auto sourceCode = std::visit([](const auto &x) {
        if constexpr (std::is_same_v<std::decay_t<decltype(x)>, 二次关系准确F来源>)
          return x.F.编码.值;
        else return x.B.编码.值;
      }, source);
      if (!sourceCode || !sourceIds.insert(sourceCode).second)
        失败(二次关系判断状态::入口拒绝);
      二次关系准确来源见证 witness;
      witness.E = 参与者.E;
      witness.来源 = source;
      if (const auto *fSource = std::get_if<二次关系准确F来源>(&source)) {
        if (!有效(fSource->F)) 失败(二次关系判断状态::入口拒绝);
        const 准确特征读取请求 read{1, g_, h_, fSource->F};
        const auto result = 特征_.读取准确特征事实(read);
        const auto *fact = std::get_if<准确特征读取事实>(&result);
        if (!fact) 失败(映射特征错误(std::get<特征数据错误>(result)));
        if (fact->Gread != g_ || fact->H != h_ || fact->信息.身份 != fSource->F)
          失败(二次关系判断状态::内部不一致);
        witness.FT = fact->信息.类型;
        if (const auto *inlineValue = std::get_if<std::int64_t>(&fact->完整值))
          witness.值 = *inlineValue;
        else {
          const auto &stored = std::get<特征值信息>(fact->完整值);
          const auto *value = std::get_if<std::int64_t>(&stored.值内容);
          if (!value) 失败(二次关系判断状态::规则不支持);
          witness.值 = *value;
        }
        const 存在当前采用完整读取请求_v2 adoptionRequest{
            2, g_, h_, 参与者.E, witness.FT};
        const auto adoption = 存在_.读取当前采用完整_v2(adoptionRequest);
        if (!adoption.成功(adoptionRequest)) 失败(映射采用状态(adoption.状态));
        if (!adoption.采用) 失败(二次关系判断状态::证据不足);
        if (adoption.采用->E != 参与者.E || adoption.采用->FT != witness.FT
            || adoption.采用->F != fSource->F)
          失败(二次关系判断状态::内部不一致);
        witness.内容 = *fact;
        witness.采用 = *adoption.采用;
      } else {
        const auto &bSource = std::get<二次关系状态端点来源>(source);
        if (!有效(bSource.B)) 失败(二次关系判断状态::入口拒绝);
        const 状态使用绑定历史读取请求 bindingRequest{1, g_, h_, bSource.B};
        const auto binding = 绑定_.读取状态使用绑定历史(bindingRequest);
        if (!binding.成功()) 失败(映射绑定状态(binding.结果头.状态));
        if (!binding.绑定 || binding.结果头.事实截止代次 != g_
            || binding.绑定->Gread != g_ || binding.绑定->H != h_
            || binding.绑定->信息.身份 != bSource.B
            || binding.绑定->信息.被描述存在 != 参与者.E)
          失败(二次关系判断状态::入口拒绝);
        const 状态历史读取请求 stateRequest{2, g_, h_, binding.绑定->信息.状态};
        const auto state = 状态_.读取状态历史内容(stateRequest);
        if (!state.成功()) 失败(映射状态读取(state.结果头.状态));
        if (!state.内容 || state.结果头.事实截止代次 != g_
            || state.内容->Gread != g_ || state.内容->H != h_
            || state.内容->信息.身份 != binding.绑定->信息.状态)
          失败(二次关系判断状态::内部不一致);
        const 特征正式准确I64解析请求_v2 parseRequest{
            2, g_, h_, state.内容->信息.正式特征类型,
            state.内容->信息.固定准确值};
        const auto parsed = 特征_.解析正式特征类型准确I64_v2(parseRequest);
        if (!parsed.成功(parseRequest)) 失败(映射解析状态(parsed.状态));
        witness.FT = state.内容->信息.正式特征类型;
        witness.值 = parsed.事实->I64;
        witness.内容 = *state.内容;
        witness.绑定 = *binding.绑定;
        if (!commonScene) {
          commonScene = binding.绑定->信息.发生场景;
          commonTime = state.内容->信息.强时间;
        } else if (*commonScene != binding.绑定->信息.发生场景
                   || *commonTime != state.内容->信息.强时间) {
          失败(二次关系判断状态::入口拒绝);
        }
      }
      if (!有效(witness.FT) || !featureTypes.insert(witness.FT.编码.值).second)
        失败(二次关系判断状态::入口拒绝);
      const auto identity = 存在_.读取存在身份来源历史见证(g_, h_, 参与者.E);
      if (!identity.成功(g_, h_, 参与者.E)) 失败(映射存在状态(identity.状态));
      witness.存在见证 = *identity.见证;
      if (!见证形状有效(witness, g_, h_)) 失败(二次关系判断状态::内部不一致);
      out.push_back(std::move(witness));
    }
    return out;
  }

  const 二次关系准确来源见证 &按类型(
      const std::vector<二次关系准确来源见证> &来源,
      特征类型身份 FT) const {
    const 二次关系准确来源见证 *found = nullptr;
    for (const auto &item : 来源) if (item.FT == FT) {
      if (found) 失败(二次关系判断状态::内部不一致);
      found = &item;
    }
    if (!found) 失败(二次关系判断状态::证据不足);
    return *found;
  }

  bool 求值FC(概念树概念身份 fc, const 二次关系准确来源见证 &source) {
    const auto result = 概念_.读取二次关系约束定义({2, {1, g_, g_}, fc});
    if (!result.成功()) 失败(映射概念状态(result.状态));
    if (result.Gread != g_ || result.H != g_ || !result.定义)
      失败(二次关系判断状态::内部不一致);
    const auto *pure = std::get_if<纯概念事实>(&*result.定义);
    if (!pure || pure->概念 != fc || pure->类别 != 相关概念类别::特征
        || pure->治理状态 == 概念树生命周期状态::退役)
      失败(二次关系判断状态::规则不支持);
    const auto *definition = std::get_if<纯I64特征概念定义>(&pure->定义);
    if (!definition) 失败(二次关系判断状态::规则不支持);
    const 特征类型身份 ft{definition->特征类型.值};
    if (source.FT != ft) 失败(二次关系判断状态::类型不相容);
    记录概念(fc);
    return 命中I64域(definition->规范域, source.值);
  }

  bool 求值FC(概念树概念身份 fc,
              const std::vector<二次关系准确来源见证> &sources) {
    const auto result = 概念_.读取二次关系约束定义({2, {1, g_, g_}, fc});
    if (!result.成功()) 失败(映射概念状态(result.状态));
    if (!result.定义) 失败(二次关系判断状态::内部不一致);
    const auto *pure = std::get_if<纯概念事实>(&*result.定义);
    if (!pure) 失败(二次关系判断状态::规则不支持);
    const auto *definition = std::get_if<纯I64特征概念定义>(&pure->定义);
    if (!definition) 失败(二次关系判断状态::规则不支持);
    return 求值FC(fc, 按类型(sources, 特征类型身份{definition->特征类型.值}));
  }

  bool 求值EC(概念树概念身份 ec,
              const std::vector<二次关系准确来源见证> &sources) {
    if (!有效(ec.值) || !ecStack_.insert(ec.值.值).second)
      失败(二次关系判断状态::内部不一致);
    struct StackGuard { std::set<std::uint64_t> &s; std::uint64_t v; ~StackGuard(){s.erase(v);} } guard{ecStack_, ec.值.值};
    const auto result = 概念_.读取二次关系约束定义({2, {1, g_, g_}, ec});
    if (!result.成功()) 失败(映射概念状态(result.状态));
    if (result.Gread != g_ || result.H != g_ || !result.定义)
      失败(二次关系判断状态::内部不一致);
    if (const auto *pure = std::get_if<纯概念事实>(&*result.定义)) {
      const auto *generic = std::get_if<通用存在概念定义>(&pure->定义);
      if (!generic || pure->概念 != ec || pure->类别 != 相关概念类别::存在
          || generic->规则版本 != 1
          || generic->规则 != 通用存在定义规则::不预设特征
          || !pure->直接上位.empty()
          || pure->治理状态 == 概念树生命周期状态::退役)
        失败(二次关系判断状态::规则不支持);
      记录概念(ec);
      return true;
    }
    const auto *specific = std::get_if<存在概念两组事实_v3>(&*result.定义);
    if (!specific || specific->概念 != ec
        || specific->治理状态 == 概念树生命周期状态::退役
        || !specific->定义.自身特征组已完整声明
        || !specific->定义.子存在概念组已完整声明)
      失败(二次关系判断状态::规则不支持);
    if (!specific->定义.已知子存在概念组.empty())
      失败(二次关系判断状态::规则不支持);
    bool match = !(specific->定义.自身特征值域组.empty()
                   && specific->定义.已知子存在概念组.empty());
    for (const auto &item : specific->定义.自身特征值域组)
      match = 求值FC(item.FC, 按类型(sources, item.FT)) && match;
    记录概念(ec);
    return match;
  }

  二次关系条件见证 求值原子(
      const 二次关系原子定义 &atom,
      const std::vector<二次关系准确来源见证> &a,
      const std::vector<二次关系准确来源见证> &b) {
    if (!有效(atom.K) || atom.D.掩码 < 1 || atom.D.掩码 > 7
        || atom.输出 != 特征类标量结果角色::差异)
      失败(二次关系判断状态::规则不支持);
    const 特征I64比较绑定读取请求_v2 fixedRequest{2, g_, g_, atom.K};
    const auto fixed = 特征_.读取I64比较绑定_v2(fixedRequest);
    if (!fixed.成功()) 失败(映射固定K状态(fixed.状态));
    if (!fixed.事实 || fixed.Gread != g_ || fixed.H != g_
        || fixed.事实->身份 != atom.K)
      失败(二次关系判断状态::内部不一致);
    const auto &definition = fixed.事实->定义;
    const 特征I64当前比较绑定读取请求_v2 currentRequest{
        2, g_, definition.输入FT, definition.用途};
    const auto current = 特征_.读取当前I64比较绑定_v2(currentRequest);
    if (!current.成功()) 失败(映射当前K状态(current.状态));
    if (!current.事实 || current.Gread != g_ || current.H != g_
        || current.事实->身份 != atom.K
        || current.事实->定义 != definition)
      失败(二次关系判断状态::规则不支持);
    const auto &sourceA = 按类型(a, definition.输入FT);
    const auto &sourceB = 按类型(b, definition.输入FT);
    bool fcMatch = true, ecMatch = true;
    for (const auto &constraint : atom.约束.FC) {
      switch (constraint.角色) {
      case 二次关系约束角色::共同:
        fcMatch = 求值FC(constraint.概念, a) && fcMatch;
        fcMatch = 求值FC(constraint.概念, b) && fcMatch;
        break;
      case 二次关系约束角色::A:
        fcMatch = 求值FC(constraint.概念, a) && fcMatch;
        break;
      case 二次关系约束角色::B:
        fcMatch = 求值FC(constraint.概念, b) && fcMatch;
        break;
      default: 失败(二次关系判断状态::入口拒绝);
      }
    }
    for (const auto &constraint : atom.约束.EC) {
      switch (constraint.角色) {
      case 二次关系约束角色::共同:
        ecMatch = 求值EC(constraint.概念, a) && ecMatch;
        ecMatch = 求值EC(constraint.概念, b) && ecMatch;
        break;
      case 二次关系约束角色::A:
        ecMatch = 求值EC(constraint.概念, a) && ecMatch;
        break;
      case 二次关系约束角色::B:
        ecMatch = 求值EC(constraint.概念, b) && ecMatch;
        break;
      default: 失败(二次关系判断状态::入口拒绝);
      }
    }
    if ((definition.上下文要求位 & static_cast<std::uint8_t>(特征I64上下文要求::参照)) != 0)
      失败(二次关系判断状态::参照不相容);
    if ((definition.上下文要求位
         & static_cast<std::uint8_t>(特征I64上下文要求::左时间)) != 0
        && !std::holds_alternative<状态内容事实>(sourceB.内容))
      失败(二次关系判断状态::参照不相容);
    if ((definition.上下文要求位
         & static_cast<std::uint8_t>(特征I64上下文要求::右时间)) != 0
        && !std::holds_alternative<状态内容事实>(sourceA.内容))
      失败(二次关系判断状态::参照不相容);
    有序I64比较合同快照 snapshot;
    snapshot.来源 = 特征比较合同来源::当前独立绑定;
    snapshot.K = fixed.事实->身份;
    snapshot.输入FT = definition.输入FT;
    snapshot.用途 = definition.用途;
    snapshot.算法族 = definition.算法族;
    snapshot.算法版本 = definition.算法版本;
    snapshot.左角色 = definition.左角色;
    snapshot.右角色 = definition.右角色;
    snapshot.上下文要求位 = definition.上下文要求位;
    snapshot.输入量化 = definition.输入量化;
    snapshot.误差合同版本 = definition.误差合同版本;
    snapshot.误差预算 = definition.误差预算;
    snapshot.相等容差 = definition.相等容差;
    snapshot.关系编码 = definition.关系编码;
    snapshot.输出组 = fixed.事实->输出组;
    snapshot.Gread = g_;
    snapshot.H = g_;
    const 特征比较执行请求 compareRequest{
        1, 请求身份_, snapshot,
        {{{definition.左角色, sourceB.值}, {definition.右角色, sourceA.值}}},
        static_cast<std::uint8_t>(特征比较结果要求::差异材料)};
    const auto compared = 比较_.比较(compareRequest);
    if (!compared.成功()) {
      if (compared.拒绝原因 == 特征比较拒绝原因::差异不可表示)
        失败(二次关系判断状态::运算溢出);
      if (compared.状态 == 特征比较状态::资源失败)
        失败(二次关系判断状态::资源失败);
      if (compared.拒绝原因 == 特征比较拒绝原因::单位不匹配
          || compared.拒绝原因 == 特征比较拒绝原因::维度不匹配
          || compared.拒绝原因 == 特征比较拒绝原因::误差合同不成立)
        失败(二次关系判断状态::单位量化不相容);
      if (compared.拒绝原因 == 特征比较拒绝原因::原始类型不匹配
          || compared.拒绝原因 == 特征比较拒绝原因::值域不允许)
        失败(二次关系判断状态::类型不相容);
      if (compared.状态 == 特征比较状态::未实现
          || compared.状态 == 特征比较状态::未注册)
        失败(二次关系判断状态::规则不支持);
      失败(二次关系判断状态::内部不一致);
    }
    const 特征比较执行输出 *difference = nullptr;
    for (const auto &output : compared.输出组)
      if (output.角色 == 特征类标量结果角色::差异) {
        if (difference) 失败(二次关系判断状态::内部不一致);
        difference = &output;
      }
    if (!difference) 失败(二次关系判断状态::内部不一致);
    const std::uint8_t bit = difference->值 < 0 ? 1 : difference->值 == 0 ? 2 : 4;
    return 二次关系条件见证{atom, sourceA, sourceB, *fixed.事实, compared,
                             fcMatch, ecMatch, (atom.D.掩码 & bit) != 0};
  }

  void 末尾守卫() const {
    const auto guard = 特征_.核验当前事实代次(g_);
    if (guard.成功()) return;
    if (guard.状态 == 特征类标量状态::事实代次漂移)
      失败(二次关系判断状态::事实代次漂移);
    if (guard.状态 == 特征类标量状态::资源失败)
      失败(二次关系判断状态::资源失败);
    失败(二次关系判断状态::内部不一致);
  }

  const std::vector<概念树概念身份> &已核验概念() const noexcept { return verified_; }

private:
  void 记录概念(概念树概念身份 概念身份) {
    if (std::none_of(verified_.begin(), verified_.end(),
                     [&](auto x) { return x == 概念身份; }))
      verified_.push_back(概念身份);
  }

  const 概念树类数据服务 &概念_;
  const 状态使用绑定只读提供者 &绑定_;
  const 状态类数据服务 &状态_;
  const 存在类数据服务 &存在_;
  const 特征类数据服务 &特征_;
  const 有序I64特征比较提供者 &比较_;
  std::uint64_t g_, h_, 请求身份_;
  std::set<std::uint64_t> ecStack_;
  std::vector<概念树概念身份> verified_;
};

二次关系判断结果 失败结果(bool 截止已确认,
                            std::uint64_t g, std::uint64_t h,
                            std::uint64_t request,
                            二次关系判断状态 state) noexcept {
  二次关系判断结果 out;
  out.状态 = state;
  if (截止已确认) {
    out.Gread = g;
    out.H = h;
    out.定义H = g;
  }
  out.请求身份 = request;
  return out;
}

template <class F>
二次关系判断结果 保护求值(std::uint32_t version,
                           std::uint64_t g, std::uint64_t h,
                           std::uint64_t request, F &&f) noexcept {
  const bool 截止已确认 = version == 2 && g != 0 && h != 0 && h <= g;
  try { return f(); }
  catch (const 求值失败 &e) {
    return 失败结果(截止已确认, g, h, request, e.状态);
  }
  catch (const std::bad_alloc &) {
    return 失败结果(截止已确认, g, h, request,
                    二次关系判断状态::资源失败);
  }
  catch (const std::length_error &) {
    return 失败结果(截止已确认, g, h, request,
                    二次关系判断状态::资源失败);
  }
  catch (...) {
    return 失败结果(截止已确认, g, h, request,
                    二次关系判断状态::内部不一致);
  }
}

void 校验请求头(std::uint32_t version, std::uint64_t g,
                 std::uint64_t h, std::uint64_t request) {
  if (version != 2 || !g || !h || h > g || !request)
    失败(二次关系判断状态::入口拒绝);
}

} // namespace

二次关系求值应用服务::二次关系求值应用服务(
    const L1事实基座服务 &l1, const 概念树类数据服务 &concepts,
    const 状态使用绑定只读提供者 &bindings,
    const 状态类数据服务 &states, const 存在类数据服务 &existences,
    const 特征类数据服务 &features, const 特征值类数据服务 &values,
    const 有序I64特征比较提供者 &comparison)
    : l1_(l1), 概念_(concepts), 绑定_(bindings), 状态_(states),
      存在_(existences), 特征_(features), 特征值_(values), 比较_(comparison) {
  if (!概念_.绑定于(l1_) || !概念_.使用特征服务(特征_)
      || !概念_.使用存在服务(存在_) || !概念_.使用特征值服务(特征值_)
      || !概念_.使用状态使用绑定提供者(绑定_) || !绑定_.绑定于(l1_)
      || !状态_.绑定于(l1_) || !存在_.绑定于(l1_) || !特征_.绑定于(l1_)
      || !特征值_.绑定于(l1_))
    throw std::invalid_argument("二次关系求值服务绑定");
}

bool 二次关系求值应用服务::绑定于(const L1事实基座服务 &l1) const noexcept {
  return &l1_ == &l1;
}

bool 二次关系求值应用服务::使用概念服务(
    const 概念树类数据服务 &concepts) const noexcept {
  return &概念_ == &concepts;
}

二次关系判断结果 二次关系求值应用服务::求值二次关系(
    const 二次关系求值请求 &request) const noexcept {
  return 保护求值(request.版本, request.Gread, request.H,
                  request.请求身份, [&] {
    校验请求头(request.版本, request.Gread, request.H, request.请求身份);
    if (!有效(request.RC.值)) 失败(二次关系判断状态::入口拒绝);
    校验参与者形状(request.A);
    校验参与者形状(request.B);
    const auto read = 概念_.读取二次关系概念(
        {2, {1, request.Gread, request.Gread}, request.RC});
    if (!read.成功()) 失败(映射概念状态(read.状态));
    if (!read.事实 || read.Gread != request.Gread || read.H != request.Gread
        || read.事实->身份 != request.RC)
      失败(二次关系判断状态::内部不一致);
    if (read.事实->治理状态 == 概念树生命周期状态::退役)
      失败(二次关系判断状态::退役不可用);
    求值上下文 context{概念_, 绑定_, 状态_, 存在_, 特征_, 比较_,
                       request.Gread, request.H, request.请求身份};
    auto a = context.解析参与者(request.A);
    auto b = context.解析参与者(request.B);
    bool hit = true;
    std::vector<二次关系条件见证> conditions;
    conditions.reserve(read.事实->规范形.原子组.size());
    if (read.事实->规范形.原子组.empty())
      失败(二次关系判断状态::内部不一致);
    for (const auto &atom : read.事实->规范形.原子组) {
      auto condition = context.求值原子(atom, a, b);
      hit = (condition.FC命中 && condition.EC命中 && condition.域命中) && hit;
      conditions.push_back(std::move(condition));
    }
    context.末尾守卫();
    二次关系判断结果 out;
    out.状态 = hit ? 二次关系判断状态::命中 : 二次关系判断状态::不命中;
    out.Gread = request.Gread; out.H = request.H; out.定义H = request.Gread;
    out.请求身份 = request.请求身份; out.条件组 = std::move(conditions);
    out.来源组 = std::move(a); out.来源组.insert(out.来源组.end(), b.begin(), b.end());
    out.已核验概念 = context.已核验概念();
    核验共同结果(out, request.Gread, request.H, request.请求身份);
    核验二参与者来源(out, request.A, request.B);
    核验条件结果(out, read.事实->规范形.原子组);
    return out;
  });
}

二次关系判断结果 二次关系求值应用服务::求值候选定义(
    const 二次关系候选求值请求 &request) const noexcept {
  return 保护求值(request.版本, request.Gread, request.H,
                  request.请求身份, [&] {
    校验请求头(request.版本, request.Gread, request.H, request.请求身份);
    校验参与者形状(request.A);
    校验参与者形状(request.B);
    const auto normalized = 概念_.规范化二次关系定义(
        {2, {1, request.Gread, request.Gread}, request.定义});
    if (!normalized.成功()) 失败(映射概念状态(normalized.状态));
    if (!normalized.规范形)
      失败(二次关系判断状态::内部不一致);
    求值上下文 context{概念_, 绑定_, 状态_, 存在_, 特征_, 比较_,
                       request.Gread, request.H, request.请求身份};
    auto a = context.解析参与者(request.A);
    auto b = context.解析参与者(request.B);
    bool hit = true;
    std::vector<二次关系条件见证> conditions;
    for (const auto &atom : normalized.规范形->原子组) {
      auto condition = context.求值原子(atom, a, b);
      hit = (condition.FC命中 && condition.EC命中 && condition.域命中) && hit;
      conditions.push_back(std::move(condition));
    }
    if (conditions.empty()) 失败(二次关系判断状态::内部不一致);
    context.末尾守卫();
    二次关系判断结果 out;
    out.状态 = hit ? 二次关系判断状态::命中 : 二次关系判断状态::不命中;
    out.Gread = request.Gread; out.H = request.H; out.定义H = request.Gread;
    out.请求身份 = request.请求身份; out.条件组 = std::move(conditions);
    out.来源组 = std::move(a); out.来源组.insert(out.来源组.end(), b.begin(), b.end());
    out.已核验概念 = context.已核验概念();
    核验共同结果(out, request.Gread, request.H, request.请求身份);
    核验二参与者来源(out, request.A, request.B);
    核验条件结果(out, normalized.规范形->原子组);
    return out;
  });
}

二次关系判断结果 二次关系求值应用服务::求值特征概念(
    const 二次关系FC求值请求 &request) const noexcept {
  return 保护求值(request.版本, request.Gread, request.H,
                  request.请求身份, [&] {
    校验请求头(request.版本, request.Gread, request.H, request.请求身份);
    if (!有效(request.FC.值)) 失败(二次关系判断状态::入口拒绝);
    校验参与者形状(request.参与者);
    if (request.来源下标 >= request.参与者.来源组.size())
      失败(二次关系判断状态::入口拒绝);
    求值上下文 context{概念_, 绑定_, 状态_, 存在_, 特征_, 比较_,
                       request.Gread, request.H, request.请求身份};
    context.预读FC(request.FC);
    auto sources = context.解析参与者(request.参与者);
    const bool hit = context.求值FC(request.FC, sources[static_cast<std::size_t>(request.来源下标)]);
    context.末尾守卫();
    二次关系判断结果 out;
    out.状态 = hit ? 二次关系判断状态::命中 : 二次关系判断状态::不命中;
    out.Gread = request.Gread; out.H = request.H; out.定义H = request.Gread;
    out.请求身份 = request.请求身份; out.来源组 = std::move(sources);
    out.已核验概念 = context.已核验概念();
    核验概念结果(out, request.Gread, request.H, request.请求身份,
                 request.FC, request.参与者, hit);
    return out;
  });
}

二次关系判断结果 二次关系求值应用服务::求值存在概念(
    const 二次关系EC求值请求 &request) const noexcept {
  return 保护求值(request.版本, request.Gread, request.H,
                  request.请求身份, [&] {
    校验请求头(request.版本, request.Gread, request.H, request.请求身份);
    if (!有效(request.EC.值)) 失败(二次关系判断状态::入口拒绝);
    校验参与者形状(request.参与者);
    求值上下文 context{概念_, 绑定_, 状态_, 存在_, 特征_, 比较_,
                       request.Gread, request.H, request.请求身份};
    context.预读EC(request.EC);
    auto sources = context.解析参与者(request.参与者);
    const bool hit = context.求值EC(request.EC, sources);
    context.末尾守卫();
    二次关系判断结果 out;
    out.状态 = hit ? 二次关系判断状态::命中 : 二次关系判断状态::不命中;
    out.Gread = request.Gread; out.H = request.H; out.定义H = request.Gread;
    out.请求身份 = request.请求身份; out.来源组 = std::move(sources);
    out.已核验概念 = context.已核验概念();
    核验概念结果(out, request.Gread, request.H, request.请求身份,
                 request.EC, request.参与者, hit);
    return out;
  });
}

} // namespace 海中鱼巣
