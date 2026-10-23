#pragma once
#include "新_因果公共类型.h"
#include "新_需求类.h"
#include "新_内部治理对象引用.h"
#include "概念_特征类.h"
#include "因果反向查询.数据.h"
#include <cstdint>
#include <optional>
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
struct 新因果筹办请求 final {
    稳定编码 父任务节点,父需求节点,目标存在节点;
    新需求目标定义 目标;
    反向因果查询请求 通用查询;
    std::optional<新因果结算筹办上下文> 结算上下文;
    std::vector<新因果任务等待边> 当前等待边组;
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
    friend bool operator==(const 新因果筹办读回&,const 新因果筹办读回&)=default;
};
class 新因果筹办读取接口 {
public:
    virtual ~新因果筹办读取接口()=default;
    virtual std::vector<新先天因果信息> 读取先天结果规则(稳定编码 目标特征类型) const=0;
    virtual 新因果筹办读回 逆推(const 新因果筹办请求&) const=0;
};
}
