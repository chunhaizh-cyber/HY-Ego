#include "合同.二次关系概念.h"

#include <algorithm>
#include <functional>
#include <map>
#include <set>

namespace 海中鱼巣 {
namespace {

bool 读取头完整(const 概念树读取头 &h) noexcept {
  return h.合同版本 == 1 && h.Gread && h.H && h.H <= h.Gread;
}

bool 发布见证完整(const 二次关系发布见证 &v) noexcept {
  if (v.版本 != 1 || !v.幂等身份 || !v.发布H || !v.已确认发布)
    return false;
  std::uint64_t prior = 0;
  std::set<std::uint64_t> ids;
  for (const auto &[key, id] : v.新编码映射) {
    if (!key || key <= prior || !有效(id) || !ids.insert(id.值).second)
      return false;
    prior = key;
  }
  return true;
}

bool 生命周期覆盖(const 概念树生命周期 &v, std::uint64_t h) noexcept {
  return v.创建事实代次 && v.创建事实代次 <= h &&
         (!v.退出事实代次 || h < *v.退出事实代次);
}

bool 约束完整(const 二次关系概念约束 &v) noexcept {
  return (v.角色 == 二次关系约束角色::共同 ||
          v.角色 == 二次关系约束角色::A ||
          v.角色 == 二次关系约束角色::B) &&
         有效(v.概念.值);
}

bool 约束小于值(const 二次关系概念约束 &a,
                const 二次关系概念约束 &b) noexcept {
  if(a.角色!=b.角色)
    return static_cast<unsigned>(a.角色)<static_cast<unsigned>(b.角色);
  return a.概念.值.值<b.概念.值.值;
}

bool 约束向量小于(const std::vector<二次关系概念约束> &a,
                  const std::vector<二次关系概念约束> &b) noexcept {
  if(a.size()!=b.size()) return a.size()<b.size();
  return std::lexicographical_compare(a.begin(),a.end(),b.begin(),b.end(),
                                      约束小于值);
}

bool 约束组完整(const 二次关系约束组 &v) noexcept {
  const auto validUnique=[](const auto& group) {
    return std::ranges::all_of(group,约束完整) &&
           std::ranges::is_sorted(group,约束小于值) &&
           std::adjacent_find(group.begin(),group.end())==group.end();
  };
  return validUnique(v.FC) && validUnique(v.EC);
}

bool 原子完整(const 二次关系原子定义 &v) noexcept {
  return v.D.掩码 >= 1 && v.D.掩码 <= 7 && 约束组完整(v.约束) &&
         有效(v.K.编码) && v.输出 == 特征类标量结果角色::差异;
}

bool 规范形完整(const 二次关系规范形 &v) noexcept {
  const auto less=[](const 二次关系原子定义 &a,const 二次关系原子定义 &b) {
    if(a.K.编码!=b.K.编码) return a.K.编码.值<b.K.编码.值;
    if(a.输出!=b.输出) return static_cast<unsigned>(a.输出)<static_cast<unsigned>(b.输出);
    if(a.约束.FC!=b.约束.FC) return 约束向量小于(a.约束.FC,b.约束.FC);
    if(a.约束.EC!=b.约束.EC) return 约束向量小于(a.约束.EC,b.约束.EC);
    return a.D.掩码<b.D.掩码;
  };
  return v.版本 == 1 && !v.原子组.empty() &&
         std::ranges::all_of(v.原子组,原子完整) &&
         std::ranges::is_sorted(v.原子组,less) &&
         std::adjacent_find(v.原子组.begin(),v.原子组.end()) == v.原子组.end();
}

bool 定义完整(const 二次关系定义 &v) noexcept {
  if (const auto *atom=std::get_if<二次关系原子定义>(&v))
    return 原子完整(*atom);
  const auto &all=std::get<二次关系合取定义>(v);
  if (!约束组完整(all.共同约束) || all.条件组.empty()) return false;
  const auto itemLess=[](const 二次关系合取项 &a,const 二次关系合取项 &b) {
    if(a.子RC.值!=b.子RC.值) return a.子RC.值.值<b.子RC.值.值;
    if(a.附加约束.FC!=b.附加约束.FC)
      return 约束向量小于(a.附加约束.FC,b.附加约束.FC);
    return 约束向量小于(a.附加约束.EC,b.附加约束.EC);
  };
  for (const auto &item:all.条件组)
    if (!有效(item.子RC.值) || !约束组完整(item.附加约束))
      return false;
  return std::ranges::is_sorted(all.条件组,itemLess) &&
         std::adjacent_find(all.条件组.begin(),all.条件组.end())==all.条件组.end();
}

bool 来源完整(const 二次关系来源事实 &v, std::uint64_t h) noexcept {
  if (!有效(v.记录) || !有效(v.成员关系) || !有效(v.目标关系) ||
      !有效(v.截止值) || !有效(v.RC.值) || !v.来源.H ||
      v.来源.H > h || !生命周期覆盖(v.生命周期,h))
    return false;
  return std::visit([](const auto &source) {
    if constexpr (requires { source.值; }) return 有效(source.值);
    else if constexpr (requires { source.编码; }) return 有效(source.编码);
    else return false;
  },v.来源.来源);
}

bool 用途完整(const 二次关系用途事实 &v, std::uint64_t h) noexcept {
  return 有效(v.记录) && 有效(v.成员关系) && 有效(v.概念关系) &&
         有效(v.依据关系) && 有效(v.RC.值) && v.业务标识 &&
         v.用途角色 >= 1 && v.用途角色 <= 4 && v.时间 >= 0 &&
         有效(v.业务依据) && v.证据H && v.证据H <= h &&
         生命周期覆盖(v.生命周期,h);
}

bool 事实头完整(const 二次关系概念事实 &v) noexcept {
  if (!(v.Gread && v.H && v.H <= v.Gread && 有效(v.身份.值) &&
         v.类别 == 相关概念类别::特征 && 有效(v.规则.值) &&
         定义完整(v.定义) && 规范形完整(v.规范形) &&
         生命周期覆盖(v.生命周期,v.H) &&
         (v.治理状态 == 概念树生命周期状态::活跃 ||
          v.治理状态 == 概念树生命周期状态::冷却 ||
          v.治理状态 == 概念树生命周期状态::退役)))
    return false;
  std::set<std::uint64_t> owned;
  const auto own=[&](稳定编码 id,const 概念树生命周期 &life) {
    return 有效(id) && owned.insert(id.值).second && 生命周期覆盖(life,v.H);
  };
  for (const auto &node:v.自有节点)
    if (!own(node.编码,node.生命周期)) return false;
  for (const auto &edge:v.自有关系)
    if (!own(edge.编码,edge.生命周期) || !有效(edge.源) ||
        !有效(edge.目标) || !有效(edge.类型)) return false;
  for (const auto &value:v.自有值)
    if (!own(value.编码,value.生命周期) || !有效(value.所属节点) ||
        !有效(value.属性类型) || !有效(value.来源)) return false;
  std::uint64_t prior=0;
  for (const auto &ref:v.定义引用) {
    if (!有效(ref.记录) || !有效(ref.所属关系) || !有效(ref.引用关系) ||
        ref.记录.值 <= prior ||
        (ref.角色 != 二次关系约束角色::共同 &&
         ref.角色 != 二次关系约束角色::A &&
         ref.角色 != 二次关系约束角色::B))
      return false;
    bool shape=false;
    switch(ref.种类) {
    case 二次关系引用种类::FC:
    case 二次关系引用种类::EC:
      shape=ref.约束顺序 &&
            std::holds_alternative<概念树概念身份>(ref.目标) &&
            有效(std::get<概念树概念身份>(ref.目标).值);
      break;
    case 二次关系引用种类::K:
      shape=!ref.项顺序 && !ref.约束顺序 &&
            ref.角色==二次关系约束角色::共同 &&
            std::holds_alternative<特征I64比较绑定身份>(ref.目标) &&
            有效(std::get<特征I64比较绑定身份>(ref.目标).编码);
      break;
    case 二次关系引用种类::子RC:
      shape=ref.项顺序 && !ref.约束顺序 &&
            ref.角色==二次关系约束角色::共同 &&
            std::holds_alternative<概念树概念身份>(ref.目标) &&
            有效(std::get<概念树概念身份>(ref.目标).值);
      break;
    }
    if (!shape) return false;
    prior=ref.记录.值;
  }
  return true;
}

bool 读成功状态(二次关系数据状态 s) noexcept {
  return s == 二次关系数据状态::已读取 ||
         s == 二次关系数据状态::冷却命中 ||
         s == 二次关系数据状态::退役命中;
}

bool 结构交付形状完整(const 二次关系结构交付 &v) noexcept {
  if (v.版本 != 1 || !有效(v.锚点) || !有效(v.规范化规则.值))
    return false;
  const auto &t=v.类型;
  const std::array<稳定编码,23> ids{
      t.规范化规则归属,t.规则版本,t.定义种类,t.定义格式,t.域掩码,
      t.输出角色,t.固定K,t.约束成员,t.约束FC,t.约束EC,t.合取成员,
      t.子RC,t.来源成员,t.来源F,t.来源B,t.来源概念,t.来源截止,
      t.用途成员,t.用途目标,t.用途业务依据,t.用途业务标识,t.用途角色,
      t.用途时间};
  std::set<std::uint64_t> unique{v.锚点.值,v.规范化规则.值.值};
  if (unique.size()!=2) return false;
  for(const auto id:ids)
    if(!有效(id)||!unique.insert(id.值).second) return false;
  return true;
}

bool 来源组完整(const std::vector<二次关系来源事实> &group,
                std::uint64_t h) noexcept {
  std::uint64_t prior=0;
  for(const auto &item:group) {
    if(!来源完整(item,h)||item.记录.值<=prior) return false;
    prior=item.记录.值;
  }
  return true;
}

bool 用途组完整(const std::vector<二次关系用途事实> &group,
                std::uint64_t h) noexcept {
  std::uint64_t prior=0;
  for(const auto &item:group) {
    if(!用途完整(item,h)||item.记录.值<=prior) return false;
    prior=item.记录.值;
  }
  return true;
}

bool 直接边完整(const 二次关系关系见证 &edge,std::uint64_t h) noexcept {
  return 有效(edge.编码)&&有效(edge.源)&&有效(edge.目标)&&有效(edge.类型)&&
         edge.源!=edge.目标&&edge.角色==1&&生命周期覆盖(edge.生命周期,h);
}

} // namespace

bool 二次关系初始化结果::成功() const noexcept {
  if (版本 != 1 || !Gread || !H || H > Gread || !交付 ||
      !结构交付形状完整(*交付))
    return false;
  if (状态 == 二次关系数据状态::已复用)
    return H == Gread && !正式回执;
  if (状态 != 二次关系数据状态::已创建 &&
      状态 != 二次关系数据状态::精确重放)
    return false;
  return 正式回执 && 发布见证完整(*正式回执) &&
         正式回执->发布H == H &&
         正式回执->精确重放 ==
             (状态 == 二次关系数据状态::精确重放);
}

bool 二次关系定义核验结果::成功() const noexcept {
  return 版本 == 2 && 状态 == 二次关系数据状态::已读取 && Gread &&
         H && H <= Gread && 规范形 && 规范形完整(*规范形);
}

bool 二次关系概念读取结果::成功() const noexcept {
  const bool stateMatches=事实 &&
      ((状态==二次关系数据状态::已读取&&事实->治理状态==概念树生命周期状态::活跃)||
       (状态==二次关系数据状态::冷却命中&&事实->治理状态==概念树生命周期状态::冷却)||
       (状态==二次关系数据状态::退役命中&&事实->治理状态==概念树生命周期状态::退役));
  return 版本 == 2 && 读成功状态(状态) && stateMatches && Gread && H && H <= Gread &&
         事实 && 事实->Gread == Gread && 事实->H == H &&
         事实头完整(*事实);
}

bool 二次关系概念写入结果::成功() const noexcept {
  if (版本 != 2 || !Gread || !事实 || !事实头完整(*事实) ||
      事实->Gread != Gread)
    return false;
  if (状态 == 二次关系数据状态::已复用 ||
      状态 == 二次关系数据状态::冷却命中)
    return !首次H && !正式回执 && 事实->H == Gread &&
           事实->治理状态==(状态==二次关系数据状态::已复用 ?
             概念树生命周期状态::活跃 : 概念树生命周期状态::冷却);
  if ((状态 != 二次关系数据状态::已创建 &&
       状态 != 二次关系数据状态::精确重放) ||
      !首次H || !*首次H || !正式回执 || !建立原请求)
    return false;
  return 事实->治理状态==概念树生命周期状态::活跃 &&
         正式回执->发布H == *首次H && 发布见证完整(*正式回执) &&
         事实->H == *首次H &&
         正式回执->精确重放 ==
             (状态 == 二次关系数据状态::精确重放);
}

bool 二次关系关联结果::成功() const noexcept {
  if (版本 != 2 || !Gread || !H || H > Gread)
    return false;
  const bool sourcesValid=来源组完整(来源组,H);
  const bool usesValid=用途组完整(用途组,H);
  const bool notMixed=来源组.empty() || 用途组.empty();
  if (!sourcesValid || !usesValid || !notMixed) return false;
  std::optional<概念树概念身份> rc;
  std::set<std::pair<unsigned,std::uint64_t>> sourceKeys;
  for(const auto &item:来源组) {
    if(rc&&*rc!=item.RC) return false;
    rc=item.RC;
    const auto key=std::visit([](const auto &source) {
      if constexpr (std::is_same_v<std::decay_t<decltype(source)>,特征信息身份>)
        return std::pair{1u,source.编码.值};
      else if constexpr (std::is_same_v<std::decay_t<decltype(source)>,状态使用绑定身份>)
        return std::pair{2u,source.编码.值};
      else return std::pair{3u,source.值.值};
    },item.来源.来源);
    if(!sourceKeys.insert(key).second) return false;
  }
  std::set<std::tuple<std::uint64_t,std::uint64_t,std::uint32_t>> useKeys;
  for(const auto &item:用途组) {
    if(rc&&*rc!=item.RC) return false;
    rc=item.RC;
    if(!useKeys.emplace(item.业务标识,item.RC.值.值,item.用途角色).second) return false;
  }
  if (状态 == 二次关系数据状态::已读取)
    return !首次H && !正式回执;
  if (状态 == 二次关系数据状态::已复用)
    return H == Gread && !首次H && !正式回执 &&
           (!来源组.empty() != !用途组.empty());
  if (状态 != 二次关系数据状态::已变更 &&
      状态 != 二次关系数据状态::精确重放)
    return false;
  return (!来源组.empty() != !用途组.empty()) && 首次H && 正式回执 &&
         *首次H == H &&
         正式回执->发布H == H && 发布见证完整(*正式回执) &&
         正式回执->精确重放 ==
             (状态 == 二次关系数据状态::精确重放);
}

bool 二次关系图结果::成功() const noexcept {
  if (版本 != 2 || 状态 != 二次关系数据状态::已读取 || !Gread ||
      !H || H > Gread || 类别 != 相关概念类别::特征)
    return false;
  std::uint64_t prior = 0;
  std::map<std::uint64_t,std::size_t> index;
  for (const auto &rc : RC组) {
    if (!事实头完整(rc) || rc.Gread != Gread || rc.H != H ||
        rc.身份.值.值 <= prior)
      return false;
    prior = rc.身份.值.值;
    index.emplace(prior,index.size());
  }
  std::vector<std::vector<std::size_t>> graph(RC组.size());
  prior=0;
  for(const auto &edge:直接边) {
    if(!直接边完整(edge,H)||edge.编码.值<=prior) return false;
    const auto from=index.find(edge.源.值),to=index.find(edge.目标.值);
    if(from==index.end()||to==index.end()) return false;
    graph[from->second].push_back(to->second);
    prior=edge.编码.值;
  }
  std::vector<std::uint8_t> color(graph.size());
  std::function<bool(std::size_t)> acyclic=[&](std::size_t node) {
    if(color[node]==1) return false;
    if(color[node]==2) return true;
    color[node]=1;
    for(const auto next:graph[node]) if(!acyclic(next)) return false;
    color[node]=2;
    return true;
  };
  for(std::size_t i=0;i<graph.size();++i) if(!acyclic(i)) return false;
  return true;
}

bool 二次关系治理结果::成功() const noexcept {
  if (版本 != 2 || !Gread || !H || H > Gread || !首次H ||
      !正式回执 || *首次H != 正式回执->发布H ||
      !发布见证完整(*正式回执) ||
      正式回执->精确重放 !=
          (状态 == 二次关系数据状态::精确重放) ||
      (状态 != 二次关系数据状态::已变更 &&
       状态 != 二次关系数据状态::精确重放))
    return false;
  if (!事实 || !事实头完整(*事实) || 事实->Gread != Gread)
    return false;
  const bool exitShape = *首次H > 0 && 事实->H == *首次H - 1 && H == 事实->H;
  const bool changeShape = 事实->H == *首次H && H == *首次H;
  if (!exitShape && !changeShape) return false;
  std::uint64_t prior=0;
  for(const auto id:已退出事实) {
    if(!有效(id)||id.值<=prior) return false;
    prior=id.值;
  }
  prior=0;
  for(const auto &edge:新直接边) {
    if(!直接边完整(edge,*首次H)||edge.编码.值<=prior) return false;
    prior=edge.编码.值;
  }
  return (!已退出事实.empty() || !新直接边.empty()) &&
         (!exitShape || !已退出事实.empty());
}

} // namespace 海中鱼巣
