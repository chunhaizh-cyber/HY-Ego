#include "自检.限定I64需求目标.h"

#include "新_世界树类.h"
#include "解释.基础语言.h"
#include "../领域/新_需求类.h"
#include "../领域/新_自然语言世界树类.h"

#include <array>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>

namespace 海中鱼巣 {

namespace {
bool 验证限定I64需求目标(新_世界树类& 世界,bool 具名网页前态,
    std::optional<隔离第六组目标选定>* 选定 = nullptr) {
    std::ofstream 输出("i64-demand-target-verification.txt",std::ios::trunc);
    if(!输出)return false;
    const auto 核验=[&](bool 条件,const char* 标识) {
        输出 << 标识 << ' ' << (条件 ? "PASS" : "FAIL") << std::endl;
        if(!条件)throw std::runtime_error(标识);
    };
    try {
        using S=特征概念规则状态;
        using M=I64已知值成员状态;
        std::ifstream 配置("i64-demand-target-setting.txt");
        std::string 合同, 试验, 独立来源, 尾项;
        std::int64_t 设定=0, 对照=0;
        const bool 读取配置=static_cast<bool>(配置>>合同>>试验>>独立来源>>设定>>对照);
        核验(读取配置 && 合同=="ISOLATED-I64-SETTING-V1" && 试验.starts_with("R131-")
            && (具名网页前态 ? 独立来源.starts_with("AUTHORIZED-FIXTURE-") : 独立来源.starts_with("ARCHIVE-"))
            && 设定!=对照 && !(配置>>尾项),"independent-test-configuration");
        输出 << "scope=ISOLATED_TEST_SETTING run=" << 试验 << " reference=" << 独立来源
            << " setting=" << 设定 << " uncovered-probe=" << 对照 << std::endl;
        if(!具名网页前态)(void)世界.初始化();
        const auto 根=世界.获取世界树根场景();
        核验(根.has_value(),"world-owner");
        auto 事实=世界.获取事实服务引用();
        auto& FT=事实.特征概念服务;
        auto& V=事实.特征值服务;
        auto& L=事实.语言服务;
        auto& 支持概念=事实.存在概念服务;
        const std::array<const 正式概念身份读取接口*,2> 读取{&FT,&支持概念};
        if(!具名网页前态)核验(事实.特征服务.装配正式概念引用读取接口(&支持概念,读取),"fixture-owner-readers");
        const auto 温度=事实.特征服务.建立或取得先天温度特征类型();
        核验(温度.信息.has_value(),"formal-temperature-type");
        const auto& 父规格=温度.信息->完整域;
        const auto* 父区间=std::get_if<std::vector<特征概念I64闭区间>>(&父规格.值域);
        核验(父规格.单位 && 父区间 && 父区间->size()==1
            && 父区间->front().下界>(std::numeric_limits<std::int64_t>::min)()
            && 设定>=父区间->front().下界 && 设定<=父区间->front().上界
            && 对照>=父区间->front().下界 && 对照<=父区间->front().上界,"formal-parent-domain");
        auto 父=FT.读取正式I64类型完整域(温度.信息->节点);
        if(!具名网页前态 && !父.概念节点) {
            const auto 定义=FT.建立或取得特征概念强类型(父规格,V);
            核验(定义.概念节点.has_value(),"parent-value-definition");
            父=FT.关联正式I64特征类型(*定义.概念节点,温度.信息->节点,事实.特征服务);
        }
        核验(父.概念节点.has_value(),"formal-parent-association");

        const auto 场景=*根;
        const auto 时间=新状态强时间{新状态时间语义::实例绝对UTC,1};
        std::optional<稳定编码> 受试杯, 子水, 错对象;
        const auto 独立设置对象=事实.场景服务.建立成员存在(场景,{{},{{*父.概念节点,设定}}},时间);
        核验(独立设置对象.存在节点 && 独立设置对象.初始状态节点组.size()==1,"isolated-setting-owner");
        auto 左=独立设置对象;
        新场景成员建立结果 右;
        if(具名网页前态) {
            const auto 标志=FT.建立或取得特征概念(
                {特征概念材料物理类型::I64标量,std::vector<特征概念I64闭区间>{{910700,910701}}},V);
            核验(有效(标志),"isolated-medium-marker-definition");
            const auto 杯=事实.场景服务.建立成员存在(场景,{{},{{标志,std::int64_t{910700}}}},时间);
            const auto 错=事实.场景服务.建立成员存在(场景,{{},{{标志,std::int64_t{910700}}}},时间);
            核验(杯.存在节点 && 错.存在节点,"isolated-cup-and-mismatching-object");
            左=事实.场景服务.建立成员存在(场景,{杯.存在节点,{{标志,std::int64_t{910701}}}},时间);
            右=事实.场景服务.建立成员存在(场景,{{},{{标志,std::int64_t{910701}}}},时间);
            受试杯=杯.存在节点;
            子水=左.存在节点;
            错对象=错.存在节点;
        } else {
            右=事实.场景服务.建立成员存在(场景,{{},{{*父.概念节点,设定}}},时间);
        }
        核验(左.存在节点 && 右.存在节点 && 左.专属存在概念节点 && 右.专属存在概念节点
            && 左.初始状态节点组.size()==1 && 右.初始状态节点组.size()==1,"independent-fixture-existences");
        const auto 分类=支持概念.抽象共同存在概念({*左.专属存在概念节点,*右.专属存在概念节点});
        核验(分类.抽象概念节点 && !分类.共同特征组.empty()
            && 支持概念.是存在概念节点(*分类.抽象概念节点),"formal-fixture-applicability-concept");
        const auto 单独依据=事实.状态服务.获取状态(独立设置对象.初始状态节点组.front());
        核验(单独依据 && 单独依据->被描述存在节点==独立设置对象.存在节点
            && 单独依据->固定准确值==新特征准确值{设定} && 独立设置对象.存在节点!=子水
            && 事实.场景服务.查询所属场景(*独立设置对象.存在节点)==场景,"fixture-setting-exact-state");
        const auto 设定F=事实.特征服务.获取特征(单独依据->来源实例特征节点);
        const auto 源定义=设定F ? FT.读取特征概念定义保护(设定F->特征概念节点) : 特征概念定义保护结果{};
        核验(源定义.信息 && 源定义.信息->实例所属特征类型==*父.概念节点
            && 源定义.信息->定义.单位==父规格.单位,"fixture-setting-formal-unit");
        const auto 采用来源=全局基础数据集.查询节点组完整结构({单独依据->节点});
        核验(采用来源.状态==基础结构操作状态::已完成 && 采用来源.节点组.size()==1
            && 采用来源.节点组.front().快照,"same-run-setting-owner-protection");

        const auto 中文=L.建立或取得语言({"zh-CN",{}});
        核验(中文.语言节点.has_value(),"fixture-language");
        核验(L.初始化语言知识结构()==新语言知识状态::已完成,"fixture-knowledge-schema");
        const 新语言知识来源请求 来源请求{新语言知识来源种类::经确认先天定义,*中文.语言节点,
            "隔离试验温度限定类型：" + 试验 + "；独立设定来源：" + 独立来源};
        const auto 来源=L.建立或取得知识先天来源(来源请求);
        核验(来源.信息.has_value(),"fixture-definition-evidence");
        const auto 来源读=L.读取知识先天来源保护(来源.信息->节点);
        核验(来源读.信息 && 来源读.信息->固定原文==来源请求.固定原文,"fixture-definition-protected-read");
        待完善特征概念结果 待完善;
        if(具名网页前态) {
            const auto 水温定义=L.查询知识先天来源(
                {新语言知识来源种类::经确认先天定义,*中文.语言节点,"水温是水的温度。"});
            核验(水温定义.信息.has_value(),"same-instance-existing-water-temperature-definition");
            待完善=FT.按来源读取待完善特征概念(水温定义.信息->节点);
        } else {
            新_自然语言世界树类 语言世界{FT,支持概念,事实.特征服务,事实.存在服务,事实.状态服务,事实.动态服务};
            (void)语言世界.初始化();
            核验(语言世界.已初始化()
                && 语言世界.接入词条概念挂接(事实.词服务,事实.名称关系服务,V),"fixture-word-scene-owner");
            const auto 词=事实.词服务.建立或取得词条({*中文.语言节点,严格UTF8字节精确规范化合同节点,
                "隔离试验温度限定类型",{}});
            核验(词.词条节点.has_value(),"fixture-only-name");
            待完善=FT.建立或取得待完善特征概念(来源.信息->节点,*词.词条节点,{},*父.概念节点);
        }
        核验(待完善.信息.has_value(),"fixture-type-identity");
        const auto C=待完善.信息->节点;
        const std::array 保护{*采用来源.节点组.front().快照};
        const auto 当前状态=事实.场景服务.查询状态列表(场景);
        auto 发表=FT.完善适用范围I64子类型(C,*父.概念节点,*分类.抽象概念节点,来源.信息->节点,
            特征概念I64已知值集{{{设定,单独依据->节点}}},V,事实.存在服务,{},保护);
        输出 << "typeStatus=" << static_cast<unsigned>(发表.状态) << " type=" << C.值
            << " scopeConcept=" << 分类.抽象概念节点->值 << " parent=" << 父.概念节点->值
            << " sourceS=" << 单独依据->节点.值 << " unit=" << 父规格.单位->值 << std::endl;
        核验(发表.状态==S::已完成 && 发表.概念节点==C,"isolated-member-owner-published");
        const auto 限定=FT.读取适用范围I64子类型(C);
        核验(限定 && 限定->直接上位==*父.概念节点 && 限定->适用存在概念==*分类.抽象概念节点
            && 限定->定义来源==来源.信息->节点,"independent-applicability-and-parent");
        if(具名网页前态) {
            const auto 组成=事实.存在服务.读取存在直接组成保护(*受试杯);
            核验(组成.状态==基础结构操作状态::已完成 && 组成.子存在组==std::vector{*子水},
                "isolated-cup-unique-direct-child");
            const auto 命中=定位语言图限定主体(*受试杯,C,事实.存在服务,FT,true);
            const auto 不符=定位语言图限定主体(*错对象,C,事实.存在服务,FT,true);
            核验(命中.实际主体==子水 && !命中.保护.empty() && !不符.实际主体,"qualified-unique-child-and-mismatch");
            const auto 子=事实.存在服务.获取存在(*子水);
            核验(子.has_value(),"target-child-owner");
            for(const auto id:子->特征节点组) {
                const auto F=事实.特征服务.获取特征(id);
                核验(F.has_value(),"target-child-feature-owner");
                const auto 类型=FT.读取实例所属特征类型(F->特征概念节点);
                核验(类型!=C && 类型!=父.概念节点,"target-child-temperature-unobserved");
            }
            输出<<"cup="<<受试杯->值<<" child="<<子水->值<<" wrong="<<错对象->值
                <<" setting-owner="<<独立设置对象.存在节点->值<<std::endl;
        }
        const auto read=[&](std::optional<std::int64_t> x,std::optional<稳定编码> unit) {
            return 读取首次需求开放单点准入(FT,C,x,unit);
        };
        const auto 收录=read(设定,父规格.单位);
        const auto 未盖=read(对照,父规格.单位);
        const auto 排除=read(父区间->front().下界-1,父规格.单位);
        核验(收录.状态==S::已完成 && 收录.成员状态==M::已收录 && 收录.依据==单独依据->节点
            && !收录.保护组.empty(),"first-demand-covered-and-protected");
        核验(未盖.状态==S::已完成 && 未盖.成员状态==M::未覆盖
            && !未盖.依据,"first-demand-uncovered");
        核验(排除.状态==S::已完成 && 排除.成员状态==M::父域明确排除
            && 排除.依据,"first-demand-parent-exclusion");
        核验(read(设定,{}).状态==S::材料类型不相容 && read({},父规格.单位).状态==S::材料类型不相容,
            "first-demand-unit-and-shape-rejection");
        auto 点定义=父规格;
        点定义.值域=std::vector<特征概念I64闭区间>{{设定,设定}};
        const auto 点=FT.建立或取得特征概念强类型(点定义,V,C);
        const auto 点读=点.概念节点 ? FT.读取特征概念定义保护(*点.概念节点) : 特征概念定义保护结果{};
        核验(点读.信息 && 点读.信息->实例所属特征类型==C && 点读.信息->定义.值域==点定义.值域,
            "first-demand-covered-point-owner-readback");
        点定义.值域=std::vector<特征概念I64闭区间>{{对照,对照}};
        核验(!FT.建立或取得特征概念强类型(点定义,V,C).概念节点,"uncovered-point-owner-reject");
        核验(事实.场景服务.查询状态列表(场景)==当前状态,"no-state-from-target-admission");
        输出 << "point=" << 点.概念节点->值 << " parent-exclusion="
            << (父区间->front().下界-1) << std::endl;
        if (具名网页前态 && 选定) {
            std::ifstream 选择输入("case06-selected-target.txt");
            if (选择输入.is_open()) {
                std::string 版本, 来源试验, 选择身份, 对象角色;
                std::uint64_t 起 = 0, 终 = 0;
                const bool 合格 = static_cast<bool>(选择输入 >> 版本 >> 来源试验 >> 选择身份 >> 对象角色 >> 起 >> 终)
                    && 版本 == "ISOLATED-TARGET-CHOICE-V1" && 来源试验 == 试验
                    && 选择身份.starts_with("CASE06-OPERATOR-") && 对象角色 == "controlled-cup"
                    && 起 < 终 && !(选择输入 >> 尾项);
                核验(合格,"isolated-operator-choice-configuration");
                *选定 = 隔离第六组目标选定{{*受试杯,*子水,C,选择身份},起,终};
            }
        }
        return true;
    } catch(const std::exception& 错误) {
        输出 << 错误.what() << std::endl;
        return false;
    }
}

}

bool 运行限定I64需求目标准入验证() {
    新_世界树类 世界;
    return 验证限定I64需求目标(世界,false);
}

bool 准备隔离第六组水温(新_世界树类& 世界, std::optional<隔离第六组目标选定>& 选定) {
    return 验证限定I64需求目标(世界,true,&选定);
}

} // namespace 海中鱼巣
