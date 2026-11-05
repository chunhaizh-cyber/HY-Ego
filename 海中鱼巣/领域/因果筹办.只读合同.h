#pragma once
#include "新_因果公共类型.h"
#include "新_需求类.h"
#include "新_内部治理对象引用.h"
#include "概念_特征类.h"
#include "概念_二次特征类.h"
#include "因果反向查询.数据.h"
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace 海中鱼巣 {
enum class 新因果条件读回状态 : std::uint8_t {
    满足=1, 尚未具备, 来源已变化
};
enum class 新因果候选读取状态 : std::uint8_t {
    已读取=1, 来源已变化, 等待同源工作, 等待配置, 需求信息待完善
};
enum class 新因果筹办读取状态 : std::uint8_t {
    已逆推=1, 无已知路径, 材料已变化
};
enum class 新因果筹办分支状态 : std::uint8_t {
    有实际候选=1, 任务角色待绑定, 数值约束无交集,
    既有消费待收尾, 既有消费已完成, 形成依赖环
};

struct 新因果条件读回 final {
    std::int64_t 条件序号=0;
    新先天因果条件种类 种类{};
    稳定编码 关系概念;
    新因果条件读回状态 状态=新因果条件读回状态::尚未具备;
    std::vector<稳定编码> 正式依据节点组;
    friend bool operator==(const 新因果条件读回&,const 新因果条件读回&)=default;
};

// 以下是task正式证明的只读投影，不是第二个证明owner或资格算法。
struct 新因果人类锚点证据 final {
    稳定编码 任务节点,需求节点,完成结算节点;
    std::vector<稳定编码> 服务确认节点组;
    friend bool operator==(const 新因果人类锚点证据&,const 新因果人类锚点证据&)=default;
};
struct 新因果贡献证据 final {
    稳定编码 任务节点,完成结算节点;
    std::optional<稳定编码> 人类服务锚点任务节点;
    std::vector<稳定编码> 直接子接收节点组;
    friend bool operator==(const 新因果贡献证据&,const 新因果贡献证据&)=default;
};
struct 新因果既有消费证据 final {
    稳定编码 准备节点,消费节点;
    std::optional<稳定编码> 收尾阶段节点;
    std::int64_t 前值=0,后值=0;
    bool 结果已正式读回=false;
    bool 收尾已正式完成=false;
    friend bool operator==(const 新因果既有消费证据&,const 新因果既有消费证据&)=default;
};
struct 新因果候选事实 final {
    新因果候选读取状态 状态=新因果候选读取状态::来源已变化;
    稳定编码 任务节点;
    std::optional<新任务虚拟存在引用> 任务引用;
    std::optional<稳定编码> 原需求节点,原目标存在节点;
    std::optional<新需求目标定义> 原目标;
    std::optional<新需求活动状态> 原需求活动状态;
    std::optional<std::int64_t> 当前任务状态,当前服务值;
    std::optional<稳定编码> 执行实例节点,影响评估节点,完成结算节点;
    std::vector<稳定编码> 直接子接收节点组;
    std::vector<新因果人类锚点证据> 人类确认锚点组;
    std::vector<新因果贡献证据> 下层贡献证明组;
    std::optional<新任务结算边界证据> 治理边界;
    std::optional<新因果既有消费证据> 既有消费;
    std::optional<稳定编码> 治理世界根节点,治理场景节点;
    std::vector<新因果条件读回> 条件组;
    friend bool operator==(const 新因果候选事实&,const 新因果候选事实&)=default;
};

struct 新因果普通存在绑定 final {
    稳定编码 存在节点,世界根节点,场景节点;
    friend bool operator==(const 新因果普通存在绑定&,const 新因果普通存在绑定&)=default;
};
struct 新因果任务信息绑定 final {
    稳定编码 任务节点;
    新任务虚拟存在引用 虚拟存在引用;
    稳定编码 治理世界根节点,治理场景节点;
    friend bool operator==(const 新因果任务信息绑定&,const 新因果任务信息绑定&)=default;
};
struct 新因果筹办角色绑定 final {
    std::int64_t 角色序号=0;
    新先天因果角色种类 种类{};
    稳定编码 范围概念;
    std::variant<std::monostate,新因果普通存在绑定,新因果任务信息绑定> 实际绑定;
    friend bool operator==(const 新因果筹办角色绑定&,const 新因果筹办角色绑定&)=default;
};
struct 新因果任务等待边 final {
    稳定编码 等待任务节点,被等待任务节点;
    friend bool operator==(const 新因果任务等待边&,const 新因果任务等待边&)=default;
};
struct 新因果结算筹办上下文 final {
    新先天因果信息 所依据先天规则;
    稳定编码 世界根节点,目标场景节点,自我存在节点,目标特征节点;
    std::int64_t 当前目标值=0;
    稳定编码 服务任务集合特征节点,服务任务集合类型,集合材料节点;
    新特征准确值 正式集合当前值;
    std::vector<稳定编码> 正式任务成员组;
    bool 成员枚举完整=false;
    稳定编码 任务范围概念;
    std::vector<新因果候选事实> 候选事实组;
    friend bool operator==(const 新因果结算筹办上下文&,const 新因果结算筹办上下文&)=default;
};
// 调用期的精确派生源约束与关系读回；不建立世界事实或改变原需求目标。
enum class 新派生源反推状态 : std::uint8_t {
    已形成=1, 不是派生目标, 算法未支持, 输入关系未支持,
    缺少源基准, 数学无解, 材料已变化, 源配置未装配
};
struct 新因果投影对应项 final {
    稳定编码 需求节点;
    std::optional<稳定编码> 父需求节点,任务节点;
    std::optional<新需求根角色> 根角色;
    friend bool operator==(const 新因果投影对应项&,const 新因果投影对应项&)=default;
};
struct 新因果正式投影上下文 final {
    稳定编码 自我存在节点,自我概念,自我场景节点,自我场景概念;
    稳定编码 自我世界根节点;
    // 自我所在场景及其全部祖先场景（至世界根）的专属概念，按由近到远排列；
    // 用于判定作用场景范围（INTERACTION-SCENE-LAYERING Q1=A）。
    std::vector<稳定编码> 自我场景链概念组;
    std::vector<稳定编码> 自我正式特征节点组;
    稳定编码 服务根需求节点,安全根需求节点,排除需求节点,需求范围概念;
    稳定编码 源特征节点,源特征类型;
    std::vector<新因果投影对应项> 完整对应项组;
    std::vector<稳定编码> 正式投影成员组;
    friend bool operator==(const 新因果正式投影上下文&,const 新因果正式投影上下文&)=default;
};
struct 新因果派生基准 final {
    稳定编码 持有者节点,源特征节点,源特征类型,源特征概念,基准材料节点;
    特征概念信息 源类型定义;
    std::vector<稳定编码> 基准成员组;
    friend bool operator==(const 新因果派生基准&,const 新因果派生基准&)=default;
};
enum class 新因果派生来源状态 : std::uint8_t {
    已读取=1, 算法未支持, 输入关系未支持, 源配置未装配,
    源特征尚未形成, 源当前值未形成, 材料已变化
};
struct 新因果派生源上下文 final {
    新因果派生来源状态 构造状态{};
    std::optional<二次特征概念信息> 正式派生定义;
    std::optional<新因果派生基准> 基准;
    std::optional<新因果正式投影上下文> 正式投影;
    friend bool operator==(const 新因果派生源上下文&,const 新因果派生源上下文&)=default;
};
struct 新因果派生源约束 final {
    稳定编码 原派生类型,原目标域,目标存在节点,源特征节点,源特征类型,基准材料节点;
    二次特征概念信息 派生定义;
    std::vector<稳定编码> 基准成员组;
    std::vector<特征概念I64闭区间> 允许新增区间组;
    std::optional<新需求新增成员见证要求> 新增成员见证要求;
    friend bool operator==(const 新因果派生源约束&,const 新因果派生源约束&)=default;
};
struct 新因果需求信息绑定 final {
    稳定编码 需求节点;
    friend bool operator==(const 新因果需求信息绑定&,const 新因果需求信息绑定&)=default;
};
struct 新关系筹办角色绑定 final {
    std::int64_t 角色序号=0;
    新关系先天角色类别 类别{};
    稳定编码 范围概念;
    std::variant<std::monostate,新因果普通存在绑定,新因果需求信息绑定> 实际绑定;
    friend bool operator==(const 新关系筹办角色绑定&,const 新关系筹办角色绑定&)=default;
};
enum class 新关系筹办条件状态 : std::uint8_t { 满足=1, 尚未具备, 角色未绑定, 本叶未支持 };
struct 新关系筹办条件读回 final {
    新关系先天条件 正式条件;
    新关系筹办条件状态 状态{};
    std::vector<稳定编码> 正式依据节点组;
    friend bool operator==(const 新关系筹办条件读回&,const 新关系筹办条件读回&)=default;
};
enum class 新关系筹办边界 : std::uint8_t {
    前提尚未具备=1, 后继投影成员目标未具备, 结果与源约束无交集,
    本叶不支持关系族, 绑定缺失, 绑定不唯一, 没有匹配关系规则
};
struct 新关系筹办缺口 final {
    新关系筹办边界 种类{};
    std::optional<稳定编码> 规则节点;
    std::optional<std::int64_t> 条件序号;
    friend bool operator==(const 新关系筹办缺口&,const 新关系筹办缺口&)=default;
};
struct 新关系筹办规则读回 final {
    稳定编码 规则节点;
    新关系先天因果定义 正式定义;
    std::vector<新关系筹办角色绑定> 角色绑定组;
    std::vector<新关系筹办条件读回> 全部条件;
    std::optional<std::vector<稳定编码>> 当前正式投影成员组;
    std::optional<std::int64_t> 当前正式投影新增数;
    std::optional<bool> 当前正式投影满足源约束;
    std::vector<新关系筹办缺口> 缺口组;
    friend bool operator==(const 新关系筹办规则读回&,const 新关系筹办规则读回&)=default;
};
struct 新派生关系筹办读回 final {
    新派生源反推状态 状态{};
    std::optional<新因果派生来源状态> 来源构造状态;
    std::optional<新因果派生源约束> 源约束;
    std::vector<新关系筹办规则读回> 关系分支组;
    std::vector<新关系筹办缺口> 缺口组;
    friend bool operator==(const 新派生关系筹办读回&,const 新派生关系筹办读回&)=default;
};
// 来源前驱是调用期部分筹办材料；工程保护只用于当前联合核验，不属于语义身份。
enum class 新来源前驱缺口 : std::uint8_t {
    人类未绑定=1,人类多义,内容未取得,正式请求未取得,自我采用未确认,
    唯一任务未形成,投影未成立,新增见证未成立,回复同需求提供者未接入,路径待选择,
    没有匹配规则,配置未就绪
};
enum class 新来源前驱前端种类 : std::uint8_t { 取得服务对象=1,既有需求内容,待实例化需求内容 };
enum class 新来源前驱材料状态 : std::uint8_t { 已读取=1,未形成,前提已变化,依赖未就绪 };
struct 新来源前驱交互者材料 final {
    新来源前驱材料状态 状态{};
    稳定编码 自我存在节点;
    std::vector<稳定编码> 正式特征节点组;
    std::optional<稳定编码> 交互者特征节点;
    std::optional<新特征准确值> 当前准确值;
    std::vector<稳定编码> 已验证人类组;
    std::vector<基础联合节点快照> 完整保护;
    friend bool operator==(const 新来源前驱交互者材料& a,const 新来源前驱交互者材料& b) {
        return a.状态==b.状态&&a.自我存在节点==b.自我存在节点
            &&a.正式特征节点组==b.正式特征节点组&&a.交互者特征节点==b.交互者特征节点
            &&a.当前准确值==b.当前准确值&&a.已验证人类组==b.已验证人类组;
    }
};
struct 新来源前驱需求材料 final {
    稳定编码 H,D;
    新需求治理信息 需求;
    std::vector<稳定编码> H正式特征节点组;
    std::optional<稳定编码> 主体需求特征节点;
    std::optional<新特征准确值> 主体需求当前值;
    新需求内容判定结果 完整内容判定;
    std::optional<新行动请求来源材料> 正式请求;
    std::optional<新需求信息> 同源自我需求;
    std::optional<新行动请求筹办资格保护结果> 同源筹办资格;
    std::optional<新因果任务信息绑定> 唯一任务;
    bool H当前需求特征引用本D=false;
    std::vector<基础联合节点快照> 完整保护;
    friend bool operator==(const 新来源前驱需求材料& a,const 新来源前驱需求材料& b) {
        const bool 同请求=a.正式请求.has_value()==b.正式请求.has_value()
            &&(!a.正式请求||(a.正式请求->来源==b.正式请求->来源&&a.正式请求->目标==b.正式请求->目标));
        return a.H==b.H&&a.D==b.D&&a.需求==b.需求&&a.H正式特征节点组==b.H正式特征节点组
            &&a.主体需求特征节点==b.主体需求特征节点&&a.主体需求当前值==b.主体需求当前值
            &&a.完整内容判定.状态==b.完整内容判定.状态
            &&a.完整内容判定.必要项目组==b.完整内容判定.必要项目组
            &&a.完整内容判定.分支组==b.完整内容判定.分支组&&同请求
            &&a.同源自我需求==b.同源自我需求&&a.同源筹办资格==b.同源筹办资格&&a.唯一任务==b.唯一任务
            &&a.H当前需求特征引用本D==b.H当前需求特征引用本D;
    }
};
struct 新来源前驱当前集合材料 final {
    新来源前驱材料状态 状态=新来源前驱材料状态::依赖未就绪;
    稳定编码 自我存在节点,源特征类型;
    std::optional<稳定编码> 源特征节点,源特征概念;
    std::optional<新特征准确值> 当前准确值;
    std::optional<新特征值信息> 当前完整材料;
    std::vector<稳定编码> 当前成员组;
    std::vector<基础联合节点快照> 完整保护;
    friend bool operator==(const 新来源前驱当前集合材料& a,const 新来源前驱当前集合材料& b) {
        return a.状态==b.状态&&a.自我存在节点==b.自我存在节点&&a.源特征类型==b.源特征类型
            &&a.源特征节点==b.源特征节点&&a.源特征概念==b.源特征概念
            &&a.当前准确值==b.当前准确值&&a.当前完整材料==b.当前完整材料&&a.当前成员组==b.当前成员组;
    }
};
struct 新任务筹办观测诊断 final {
    std::optional<稳定编码> 任务节点;
    std::string_view 阶段,读取用途,结果类型,状态类型,状态名称;
    std::optional<std::int64_t> 状态编码;
    std::optional<bool> 判定值,依赖存在;
};
struct 新来源前驱查询上下文 final {
    新服务取得来源前驱配置 配置;
    新来源前驱交互者材料 交互者;
    新来源前驱当前集合材料 当前集合;
    稳定编码 完成服务父需求,服务根需求,排除投影需求;
    std::vector<新来源前驱需求材料> 完整候选组;
    std::optional<新服务取得目标实例信息> 本槽既有实例;
    std::vector<基础联合节点快照> 完整保护;
    friend bool operator==(const 新来源前驱查询上下文& a,const 新来源前驱查询上下文& b) {
        // 构造诊断只供人读，不参与原来源上下文的业务等价。
        return a.配置==b.配置&&a.交互者==b.交互者&&a.当前集合==b.当前集合&&a.完成服务父需求==b.完成服务父需求
            &&a.服务根需求==b.服务根需求&&a.排除投影需求==b.排除投影需求
            &&a.完整候选组==b.完整候选组&&a.本槽既有实例==b.本槽既有实例;
    }
    std::optional<新任务筹办观测诊断> 构造诊断;
};
struct 新来源前驱消息绑定 final {
    稳定编码 M;
    friend bool operator==(const 新来源前驱消息绑定&,const 新来源前驱消息绑定&)=default;
};
struct 新来源前驱含义绑定 final {
    稳定编码 U;
    friend bool operator==(const 新来源前驱含义绑定&,const 新来源前驱含义绑定&)=default;
};
struct 新来源前驱人类身份绑定 final {
    稳定编码 人类存在节点;
    friend bool operator==(const 新来源前驱人类身份绑定&,const 新来源前驱人类身份绑定&)=default;
};
using 新来源前驱绑定值=std::variant<std::monostate,新因果普通存在绑定,
    新来源前驱人类身份绑定,新因果需求信息绑定,新因果任务信息绑定,新来源前驱消息绑定,
    新来源前驱含义绑定,新接收认知来源键>;
struct 新来源前驱角色绑定 final {
    新来源前驱角色 角色{};
    稳定编码 范围概念;
    新来源前驱求值阶段 绑定阶段{};
    新来源前驱绑定值 实际绑定;
    friend bool operator==(const 新来源前驱角色绑定&,const 新来源前驱角色绑定&)=default;
};
enum class 新来源前驱条件状态 : std::uint8_t {
    满足=1,尚未具备,角色未绑定,未支持,前提已变化,接纳未确认,待动作结果
};
struct 新来源前驱条件读回 final {
    新来源前驱条件 正式条件;
    新来源前驱条件状态 状态{};
    std::vector<新来源前驱角色绑定> 参与角色;
    std::vector<稳定编码> 正式依据节点组;
    std::vector<基础联合节点快照> 完整保护;
    friend bool operator==(const 新来源前驱条件读回& a,const 新来源前驱条件读回& b) {
        return a.正式条件==b.正式条件&&a.状态==b.状态&&a.参与角色==b.参与角色
            &&a.正式依据节点组==b.正式依据节点组;
    }
};
struct 新来源前驱规则读回 final {
    新来源前驱规则信息 规则;
    std::vector<新来源前驱角色绑定> 动作前输入角色,动作后结果角色;
    std::vector<新来源前驱条件读回> 输入条件组,接纳核验组,结果校验组;
    bool 来源处理已确认=false,本次发生分支已确认=false,当前直接结果已读回=false;
    std::vector<稳定编码> 来源处理依据组,当前直接结果依据组;
    friend bool operator==(const 新来源前驱规则读回& a,const 新来源前驱规则读回& b) {
        // 真实发生统计不是静态路径身份，不因统计前进撤销同一正式规则。
        return a.规则.节点==b.规则.节点&&a.规则.定义==b.规则.定义
            &&a.动作前输入角色==b.动作前输入角色&&a.动作后结果角色==b.动作后结果角色
            &&a.输入条件组==b.输入条件组&&a.接纳核验组==b.接纳核验组&&a.结果校验组==b.结果校验组
            &&a.来源处理已确认==b.来源处理已确认&&a.本次发生分支已确认==b.本次发生分支已确认
            &&a.当前直接结果已读回==b.当前直接结果已读回&&a.来源处理依据组==b.来源处理依据组
            &&a.当前直接结果依据组==b.当前直接结果依据组;
    }
};
enum class 新来源前驱规则族 : std::uint8_t { 来源前驱=1,原投影 };
enum class 新来源连接槽 : std::uint8_t { 规则角色=1,投影对应项需求,投影对应项任务 };
struct 新来源前驱角色端点 final {
    新来源前驱规则族 规则族{};
    稳定编码 规则节点;
    新来源前驱求值阶段 阶段{};
    新来源连接槽 槽{};
    std::variant<新来源前驱角色,std::int64_t> 角色;
    friend bool operator==(const 新来源前驱角色端点&,const 新来源前驱角色端点&)=default;
};
struct 新来源前驱连接 final {
    稳定编码 上游规则;
    新来源前驱关系结果 上游结果{};
    新来源前驱规则族 下游族{};
    稳定编码 下游规则;
    新来源前驱求值阶段 下游阶段=新来源前驱求值阶段::动作前输入;
    std::int64_t 下游条件序号=0;
    稳定编码 下游关系概念;
    std::vector<std::pair<新来源前驱角色端点,新来源前驱角色端点>> 映射;
    bool 仅覆盖下游部分条件=true;
    friend bool operator==(const 新来源前驱连接&,const 新来源前驱连接&)=default;
};
struct 新来源前驱具名缺口 final {
    新来源前驱缺口 种类{};
    新来源前驱规则族 规则族{};
    std::optional<稳定编码> 规则节点;
    std::optional<std::int64_t> 条件序号;
    std::optional<新来源前驱求值阶段> 阶段;
    std::vector<新来源前驱角色> 未绑定角色;
    std::vector<稳定编码> 正式依据节点组;
    friend bool operator==(const 新来源前驱具名缺口&,const 新来源前驱具名缺口&)=default;
};
struct 新来源前驱链条件读回 final {
    新来源前驱链条件定义 正式条件;
    新来源前驱条件状态 状态{};
    std::vector<新来源前驱角色绑定> 参与角色;
    std::vector<稳定编码> 正式依据节点组;
    std::vector<基础联合节点快照> 完整保护;
    friend bool operator==(const 新来源前驱链条件读回& a,const 新来源前驱链条件读回& b) {
        return a.正式条件==b.正式条件&&a.状态==b.状态
            &&a.参与角色==b.参与角色&&a.正式依据节点组==b.正式依据节点组;
    }
};
struct 新来源前驱OR路径 final {
    std::vector<新来源前驱规则读回> 来源规则组;
    新关系筹办规则读回 原投影分支;
    std::vector<新来源前驱连接> 连接组;
    std::vector<新来源前驱链条件读回> 链条件组;
    std::vector<新来源前驱具名缺口> 缺口组;
    friend bool operator==(const 新来源前驱OR路径& a,const 新来源前驱OR路径& b) {
        return a.来源规则组==b.来源规则组&&a.原投影分支==b.原投影分支
            &&a.连接组==b.连接组&&a.链条件组==b.链条件组&&a.缺口组==b.缺口组;
    }
};
struct 新来源前驱内容模板 final {
    新来源前驱前端种类 种类{};
    std::vector<std::size_t> 支持OR路径序号组;
    稳定编码 目标槽类型,要求概念;
    std::optional<稳定编码> H,D;
    std::vector<稳定编码> 正式根路径;
    friend bool operator==(const 新来源前驱内容模板&,const 新来源前驱内容模板&)=default;
};
struct 新来源前驱路径读回 final {
    std::optional<新来源前驱元数据读取结果> 元数据读回;
    std::vector<新来源前驱OR路径> OR路径组;
    std::vector<新来源前驱具名缺口> 未连接缺口组;
    std::vector<新来源前驱内容模板> 行动前端组;
    std::vector<基础联合节点快照> 完整保护;
    friend bool operator==(const 新来源前驱路径读回& a,const 新来源前驱路径读回& b) {
        return a.元数据读回==b.元数据读回&&a.OR路径组==b.OR路径组
            &&a.未连接缺口组==b.未连接缺口组&&a.行动前端组==b.行动前端组;
    }
};
struct 新因果筹办请求 final {
    稳定编码 父任务节点,父需求节点,目标存在节点;
    新需求目标定义 目标;
    反向因果查询请求 通用查询;
    std::optional<新因果结算筹办上下文> 结算上下文;
    std::vector<新因果任务等待边> 当前等待边组;
    std::optional<新因果派生源上下文> 派生源上下文;
    std::optional<新来源前驱查询上下文> 来源前驱上下文;
    friend bool operator==(const 新因果筹办请求&,const 新因果筹办请求&)=default;
};
struct 新因果筹办分支读回 final {
    新因果筹办分支状态 状态=新因果筹办分支状态::任务角色待绑定;
    稳定编码 规则节点;
    std::int64_t 分支序号=0;
    新先天因果定义 定义;
    std::vector<新因果筹办角色绑定> 角色绑定组;
    std::vector<特征概念I64闭区间> 需要服务值区间组;
    std::vector<新因果条件读回> 全部条件;
    std::optional<新因果候选事实> 候选事实;
    新因果动作触发方式 触发方式=新因果动作触发方式::尚未确定;
    std::vector<std::int64_t> 未绑定角色序号组;
    std::vector<新因果任务等待边> 环边组;
    friend bool operator==(const 新因果筹办分支读回&,const 新因果筹办分支读回&)=default;
};
struct 新因果主动方法参考 final {
    std::size_t 图分支编号=0;
    稳定编码 因果节点,动作概念节点;
    std::vector<反向因果图角色绑定> 角色绑定组;
    std::vector<稳定编码> 正式方法节点组;
    friend bool operator==(const 新因果主动方法参考&,const 新因果主动方法参考&)=default;
};
struct 新因果静态状态缺口 final {
    std::size_t 图节点编号=0;
    反向因果参与者键 参与者;
    反向因果状态条件 需要状态;
    friend bool operator==(const 新因果静态状态缺口&,const 新因果静态状态缺口&)=default;
};
enum class 新因果能力缺口种类 : std::uint8_t {
    未知动作=1, 触发方式未确定, 主动动作无对应方法,
    状态目标尚无路径, 未绑定参与者, 值域暂不能转换
};
struct 新因果能力缺口 final {
    新因果能力缺口种类 种类=新因果能力缺口种类::状态目标尚无路径;
    std::optional<std::size_t> 图节点编号,图分支编号;
    std::optional<稳定编码> 因果节点,动作概念节点;
    std::optional<反向因果状态条件> 需要状态;
    std::optional<新需求变化方向> 目标方向;
    friend bool operator==(const 新因果能力缺口&,const 新因果能力缺口&)=default;
};
struct 新因果筹办读回 final {
    新因果筹办读取状态 状态=新因果筹办读取状态::无已知路径;
    反向因果查询结果 通用反查图;
    std::vector<新因果主动方法参考> 主动方法参考组;
    std::vector<新因果静态状态缺口> 静态状态缺口组;
    std::vector<新因果能力缺口> 能力缺口组;
    std::vector<新因果筹办分支读回> 分支组;
    std::optional<新派生关系筹办读回> 派生关系读回;
    std::optional<新来源前驱路径读回> 来源前驱读回;
    friend bool operator==(const 新因果筹办读回&,const 新因果筹办读回&)=default;
};
class 新因果筹办读取接口 {
public:
    virtual ~新因果筹办读取接口()=default;
    virtual std::vector<新先天因果信息> 读取先天结果规则(稳定编码 目标特征类型) const=0;
    virtual 新因果筹办读回 逆推(const 新因果筹办请求&) const=0;
    virtual 新来源前驱元数据读取结果 读取服务取得来源前驱元数据(
        const 新服务取得来源前驱配置&) const=0;
    virtual 新来源前驱规则读取结果 读取来源前驱规则保护(稳定编码) const=0;
    virtual 新来源前驱规则查询结果 查询来源前驱规则保护(
        稳定编码 服务任务类型,新来源前驱关系结果) const=0;
};
}
