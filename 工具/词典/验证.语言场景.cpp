#include "../../海中鱼巣/业务/转换.语言场景归类.h"
#include "../../海中鱼巣/领域/新_自然语言世界树类.h"
#include "../../海中鱼巣/领域/新_词类.h"
#include "../../海中鱼巣/领域/新_存在类.h"
#include "../../海中鱼巣/领域/新_动态类.h"
#include "../../海中鱼巣/领域/概念_二次特征类.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
using namespace 海中鱼巣;
namespace {
void 必须(bool v,const char* m){if(!v)throw std::runtime_error(m);}
std::string J(std::string_view s){std::string r="\"";for(unsigned char c:s){switch(c){case '\\':r+="\\\\";break;case '"':r+="\\\"";break;case '\n':r+="\\n";break;case '\r':r+="\\r";break;case '\t':r+="\\t";break;default:if(c<32)r+=' ';else r+=char(c);}}return r+'"';}
std::string 解转义(std::string_view s){std::string r;for(size_t i=0;i<s.size();++i){if(s[i]!='\\')r+=s[i];else {必须(++i<s.size(),"tsv escape");switch(s[i]){case 'n':r+='\n';break;case 'r':r+='\r';break;case 't':r+='\t';break;case '\\':r+='\\';break;default:throw std::runtime_error("escape");}}}return r;}
const char* 状态名(语言场景归类输入状态 s){using S=语言场景归类输入状态;switch(s){case S::已形成且读回:return "已形成且读回";case S::未知词:return "未知词";case S::缺规则:return "缺规则";case S::工程未完成:return "工程未完成";case S::输入拒绝:return "输入拒绝";case S::场景建立未完成:return "场景建立未完成";case S::写后读回未完成:return "写后读回未完成";default:return "其他现有状态";}}
}
int wmain(int argc,wchar_t** argv){
 const auto begin=std::chrono::steady_clock::now();
  std::map<std::string,size_t> counts,examples,preCounts;
  size_t parsedAll=0,selected=0,attempted=0,success=0,negative=0,missing=0,multi=0,preAttempts=0,preSuccess=0,extraMissing=0;
  double conversionSeconds=0;
 bool scanComplete=false;
 auto dump=[&](){
  const auto elapsed=std::chrono::duration<double>(std::chrono::steady_clock::now()-begin).count();
  std::cout<<"{\"summary\":true,\"decoded_all\":"<<parsedAll<<",\"attempted\":"<<attempted<<",\"success\":"<<success<<",\"pre_attempted\":"<<preAttempts<<",\"pre_success\":"<<preSuccess<<",\"extra_missing_rule\":"<<extraMissing<<",\"unknown_before_admission\":"<<negative<<",\"missing_rule_retained\":"<<missing<<",\"multi_sense_heads\":"<<multi<<",\"conversion_seconds\":"<<conversionSeconds<<",\"seconds\":"<<elapsed<<",\"scan_complete\":"<<(scanComplete?"true":"false")<<",\"counts\":{";
  bool first=true;for(const auto& [k,v]:counts){std::cout<<(first?"":",")<<J(k)<<":"<<v;first=false;}std::cout<<"}}"<<std::endl;
 };
 try {
  必须(argc==3,"args: dictionary-directory decoded-line-limit (0=all)");
  const std::filesystem::path dir=argv[1];
  const auto limit=std::stoull(argv[2]);
  const auto opened=离线词典数据源::打开(dir);
  必须(opened.状态==词典操作状态::已完成&&bool(opened.数据源),opened.诊断.c_str());
  const auto& source=*opened.数据源;
  必须(source.释义辅助表就绪(),source.释义辅助表诊断().c_str());
  std::cout<<"{\"package\":"<<J(opened.包标识)<<",\"limit\":"<<limit<<"}"<<std::endl;
  新_特征值类 values(全局基础数据集);概念_特征类 fc;概念_存在类 ec(fc);
  新_特征类 features(values,fc);新_状态类 states(features);新_存在类 exists(features,ec,states);
  概念_二次特征类 sc(fc,values);概念_动态类 dc(fc,values,sc);新_动态类 dynamics(states,dc,fc);
  必须(values.初始化()&&fc.初始化(values)&&ec.初始化()&&features.初始化()&&states.初始化()&&exists.初始化()&&sc.初始化()&&dc.初始化()&&dynamics.初始化(),"domain init");
  新_自然语言世界树类 world(fc,ec,features,exists,states,dynamics);
  必须(world.初始化().成功(),"language world init");
  // 隔离运行时只有这一个词典身份；不调用生产启动/恢复/任务线程。
  const auto dictionary=全局基础数据集.新建节点();
  const auto de=exists.初始化存在节点(dictionary,{},true,{});
  必须(de.状态==新存在操作状态::已完成&&exists.获取存在(dictionary).has_value(),"dictionary existence init/readback");
  新_语言类 language;新_词类 words(language);必须(words.初始化(),"word init");
  const auto lang=language.建立或取得语言({"zh",{}});必须(lang.语言节点.has_value(),"zh init");
  const auto lid=*lang.语言节点;

  std::set<std::pair<std::string,std::string>> seen;
  std::ifstream file(dir/L"词典.简体.tsv",std::ios::binary);必须(file.is_open(),"simplified package missing");
  std::string line;std::getline(file,line);
  auto emit=[&](const char* lane,const 词典义项材料& sense,const 词典释义行转换结果& r){
   const std::string key=std::string(lane)+":"+状态名(r.状态);if(examples[key]++>=20)return;
   const std::string center = r.解读材料.中心词 ? r.解读材料.中心词->规范化词面 : std::string{};
   std::cout<<"{\"lane\":"<<J(lane)<<",\"status\":"<<J(状态名(r.状态))<<",\"record\":"<<J(sense.记录键)<<",\"sense\":"<<J(sense.义项键)<<",\"X\":"<<J(sense.词面)<<",\"line\":"<<J(r.解读材料.释义原文)<<",\"N\":"<<J(center)<<",\"diagnostic\":"<<J(r.诊断.empty() ? r.解读材料.诊断 : r.诊断)<<",\"unknown\":[";
   for(size_t i=0;i<r.未登记词面组.size();++i)std::cout<<(i?",":"")<<J(r.未登记词面组[i]);
   std::cout<<"]";
   if(r.读回){const auto& v=r.读回->内容;std::cout<<",\"node\":"<<r.读回->节点.值<<",\"speaker\":"<<v.说话者词典.值<<",\"source_record\":"<<J(v.来源义项.记录键)<<",\"source_sense\":"<<J(v.来源义项.义项键);
    if(v.归类)std::cout<<",\"X_id\":"<<v.归类->X.值<<",\"N_id\":"<<v.归类->N.值;
    if(v.M起始标量)std::cout<<",\"M_begin\":"<<*v.M起始标量<<",\"M_end\":"<<*v.M结束标量;
   }std::cout<<"}"<<std::endl;
  };
  while(std::getline(file,line)){
   const auto a=line.find('\t'),b=line.find('\t',a+1),c=line.find('\t',b+1);必须(a!=line.npos&&b!=line.npos&&c!=line.npos,"tsv columns");
   auto tag=解转义(std::string_view(line).substr(a+1,b-a-1)),head=解转义(std::string_view(line).substr(b+1,c-b-1));
   if(!seen.emplace(tag,head).second)continue;
   const auto found=source.精确查询({tag,head});必须(found.状态==词典操作状态::已完成,"exact lookup");
   size_t headSelected=0;
   for(const auto& sense:found.义项组){
    const auto parsed=解读词典义项释义(source,sense);
    size_t decoded=0;for(const auto& p:parsed)if(p.状态==词典释义解读状态::已解读)++decoded;
    parsedAll+=decoded;
    词典义项语言消息 message{dictionary,lid,严格UTF8字节精确规范化合同节点,sense};
    if(decoded && (!limit||selected<limit)){
     ++headSelected;selected+=decoded;
     const auto conversionStart=std::chrono::steady_clock::now();
     // 准入前的真实未知词路径；不把这些重复尝试计入主批次。
     const auto before=转换词典义项到语言场景(message,source,world,language,words,exists);
     必须(before.逐行结果.size()==parsed.size(),"negative line count");
     for(const auto& r:before.逐行结果)if(r.解读材料.状态==词典释义解读状态::已解读){
      ++preAttempts;++preCounts[状态名(r.状态)];emit("准入前",sense,r);
      if(r.状态==语言场景归类输入状态::未知词){++negative;必须(r.读回&&!r.读回->内容.归类,"unknown material not retained");}
      else if(r.状态==语言场景归类输入状态::已形成且读回)++preSuccess;
      else throw std::runtime_error("unexpected pre-admission result");
     }
     // 由验证调用方显式准入；边界证据指向已保存的词典词头表材料。
     for(size_t i=0;i<parsed.size();++i)if(parsed[i].状态==词典释义解读状态::已解读){
      必须(before.逐行结果[i].材料节点.has_value(),"admission source missing");
      const auto origin=before.逐行结果[i].材料节点;
      auto admit=[&](const std::string& t){const auto w=words.建立或取得词条({lid,严格UTF8字节精确规范化合同节点,t,origin});必须((w.状态==新词条建立状态::已建立||w.状态==新词条建立状态::已复用)&&w.词条节点.has_value(),"explicit admission failed");};
      const auto x=source.繁简规范化(sense.词面);必须(x.has_value(),"normalize X");admit(*x);
      for(const auto& t:parsed[i].词段组)admit(t.规范化词面);
     }
     const auto after=转换词典义项到语言场景(message,source,world,language,words,exists);
     必须(after.逐行结果.size()==parsed.size(),"line count");
     for(const auto& r:after.逐行结果){
      if(r.解读材料.状态!=词典释义解读状态::已解读){
       if(r.状态==语言场景归类输入状态::缺规则){++extraMissing;必须(r.读回&&!r.读回->内容.归类,"extra missing material");emit("缺规则",sense,r);}continue;
      }
      ++attempted;++counts[状态名(r.状态)];emit("主批次",sense,r);
      if(r.状态==语言场景归类输入状态::已形成且读回){
       必须(r.读回&&r.读回->内容.归类,"claim absent");
       const auto back=language.读取词典语言材料(r.读回->节点);必须(back&&back->内容==r.读回->内容,"independent readback");
       const auto& v=back->内容;const auto x=words.获取词条(v.归类->X),n=words.获取词条(v.归类->N);
       必须(x.信息&&n.信息&&x.信息->规范化词面==*source.繁简规范化(sense.词面)&&n.信息->规范化词面==r.解读材料.中心词->规范化词面
        &&v.来源义项.记录键==sense.记录键&&v.来源义项.义项键==sense.义项键&&v.来源义项.汉语释义==sense.汉语释义&&v.说话者词典==dictionary
        &&v.释义行原文==r.解读材料.释义原文&&v.M起始标量.has_value()==r.解读材料.修饰起始标量.has_value(),"claim fields mismatch");
       if(v.M起始标量)必须(*v.M起始标量==*r.解读材料.修饰起始标量&&*v.M结束标量==*r.解读材料.修饰结束标量,"M mismatch");
       ++success;
      }
     }
     conversionSeconds+=std::chrono::duration<double>(std::chrono::steady_clock::now()-conversionStart).count();
    }else if(!decoded&&missing<20&&!parsed.empty()){
     const auto converted=转换词典义项到语言场景(message,source,world,language,words,exists);
     for(const auto& r:converted.逐行结果)if(r.状态==语言场景归类输入状态::缺规则){++missing;必须(r.读回&&!r.读回->内容.归类,"missing-rule material");emit("缺规则",sense,r);}
    }
   }
   if(headSelected>1)++multi;
  }
  必须(file.eof(),"package scan failed");
  scanComplete=true;dump();
  return attempted&&success==attempted?0:3;
 }catch(const std::exception& e){std::cout<<"{\"error\":"<<J(e.what())<<"}"<<std::endl;dump();std::cerr<<e.what()<<std::endl;return 2;}
}
