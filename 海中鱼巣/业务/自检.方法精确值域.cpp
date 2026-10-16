#include "自检.方法精确值域.h"

#include "../启动.程序入口.h"
#include "../启动选项.数据.h"
#include "../领域/新_方法类.h"
#include "../领域/新_场景类.h"
#include "../领域/新_任务类.h"
#include "../领域/概念_二次特征类.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace 海中鱼巣 {
namespace {
using 区间 = 特征概念I64闭区间;
using 整数 = std::int64_t;

static_assert(std::is_same_v<decltype(新特征值域关系比较结果{}.关系编码),
    std::optional<std::int64_t>>);

void 要求(bool 成立, const char* 说明) {
    if (!成立) throw std::runtime_error(说明);
}

bool 评估材料相同(const 新任务影响评估信息& 左,
    const 新任务影响评估信息& 右) {
    return 左.节点 == 右.节点 && 左.任务节点 == 右.任务节点
        && 左.需求节点 == 右.需求节点 && 左.执行实例节点 == 右.执行实例节点
        && 左.方法节点 == 右.方法节点
        && 左.目标需求结果.状态 == 右.目标需求结果.状态
        && 左.目标需求结果.实际特征节点 == 右.目标需求结果.实际特征节点
        && 左.目标需求结果.准确差异 == 右.目标需求结果.准确差异
        && 左.符合预测状态组 == 右.符合预测状态组
        && 左.预计但未发生项组 == 右.预计但未发生项组
        && 左.计划外状态组 == 右.计划外状态组
        && 左.方向验证 == 右.方向验证 && 左.幅度验证 == 右.幅度验证
        && 左.值域验证 == 右.值域验证 && 左.多渠道验证 == 右.多渠道验证
        && 左.禁止结果验证 == 右.禁止结果验证
        && 左.副作用验证 == 右.副作用验证 && 左.归因验证 == 右.归因验证;
}

// 真实服务依赖，仅在独立自检进程中构造；不初始化世界、线程或外设。
struct 自检服务 final {
    新_特征值类 特征值{全局基础数据集};
    概念_特征类 特征概念;
    新_特征类 特征{特征值, 特征概念};
    新_状态类 状态{特征};
    概念_存在类 存在概念{特征概念};
    新_存在类 存在{特征, 存在概念, 状态};
    概念_二次特征类 二次概念{特征概念, 特征值};
    概念_动态类 动态概念{特征概念, 特征值, 二次概念};
    新_动态类 动态{状态, 动态概念, 特征概念};
    稳定编码 场景根;
    新_场景类 场景{场景根, 存在, 状态, 动态};
    新_方法类 方法{存在, 场景, 特征值, 特征, 存在概念, 特征概念};
    稳定编码 数值类型;
    稳定编码 文本类型;
    稳定编码 数值子概念;
    稳定编码 单位;

    自检服务() {
        要求(特征值.初始化() && 特征概念.初始化(特征值)
            && 方法.初始化(), "service initialization");
        单位 = 全局基础数据集.新建节点();
        要求(有效(单位), "unit allocation");
        特征概念定义 数值定义;
        数值定义.值域 = std::vector<区间>{{-100, 100}};
        数值定义.单位 = 单位;
        数值类型 = 特征概念.建立或取得特征概念(数值定义, 特征值);
        数值定义.值域 = std::vector<区间>{{-10, 10}};
        数值子概念 = 特征概念.建立或取得特征概念(
            数值定义, 特征值, 数值类型);
        特征概念定义 文本定义;
        文本定义.材料类型 = 特征概念材料物理类型::UTF8字符串;
        文本定义.值域 = 特征概念全材料值域{};
        文本定义.比较规则 = 特征概念比较规则::完整材料精确;
        文本类型 = 特征概念.建立或取得特征概念(文本定义, 特征值);
        要求(有效(数值类型) && 有效(文本类型) && 有效(数值子概念),
            "feature concept allocation");
    }

    新方法知识建立请求 请求() {
        新方法知识建立请求 请求;
        请求.方法头.涉及特征类型概念组 = {数值类型};
        请求.方法头.可能结果概念组 = {数值类型};
        新方法参数规格 规格;
        规格.参数角色节点 = 全局基础数据集.新建节点();
        要求(有效(规格.参数角色节点), "role allocation");
        规格.特征规格组.push_back({数值类型, std::nullopt, {}});
        请求.参数规格组.push_back(规格);
        新方法条件项定义 条件;
        条件.参数角色节点 = 规格.参数角色节点;
        条件.特征类型概念节点 = 数值类型;
        条件.精确数值域 = 区间{2, 2};
        新方法结果项定义 结果;
        结果.参数角色节点 = 规格.参数角色节点;
        结果.特征类型概念节点 = 数值类型;
        结果.精确数值域 = 区间{3, 3};
        结果.主要结果 = true;
        auto 禁止 = 结果;
        禁止.精确数值域 = 区间{10, 12};
        禁止.主要结果 = false;
        新方法条件结果定义 配对;
        配对.条件.条件项组.push_back(条件);
        配对.结果.结果项组.push_back(结果);
        配对.结果.确认要求.禁止结果项组.push_back(禁止);
        请求.条件结果组.push_back(配对);
        return 请求;
    }

    void 建立并检查复制(const 新方法知识建立请求& 请求) {
        const auto 建立 = 方法.建立方法知识(请求);
        要求(建立.状态 == 新方法操作状态::已建立 && 建立.方法节点,
            "method creation");
        const auto 源 = 方法.获取方法(*建立.方法节点);
        要求(源 && 源->条件结果组.size() == 请求.条件结果组.size(),
            "method readback");
        const auto 校验材料 = [&](const 新方法信息& 信息) {
            要求(信息.方法头 == 请求.方法头
                && 信息.参数规格组 == 请求.参数规格组, "header/spec preservation");
            要求(信息.条件结果组.size() == 请求.条件结果组.size(), "pair count");
            for (std::size_t i = 0; i < 请求.条件结果组.size(); ++i) {
                要求(信息.条件结果组[i].条件 == 请求.条件结果组[i].条件
                    && 信息.条件结果组[i].结果 == 请求.条件结果组[i].结果,
                    "exact domain and confirmation preservation");
            }
        };
        校验材料(*源);
        const auto 再建 = 方法.建立方法知识(请求);
        要求(再建.状态 == 新方法操作状态::已找到 && 再建.方法节点 == 建立.方法节点,
            "equal material deduplication");
        const auto 复制 = 方法.复制方法(源->节点);
        要求(复制.状态 == 新方法复制状态::已复制 && 复制.方法实例节点,
            "method copy");
        const auto 副本 = 方法.获取方法(*复制.方法实例节点);
        要求(副本.has_value(), "copy readback");
        校验材料(*副本);
        要求(副本->来源方法节点 == 源->节点
            && 副本->实例根节点 == 副本->节点
            && 副本->节点 != 源->节点
            && 副本->方法头节点 != 源->方法头节点
            && 副本->方法虚拟存在节点 != 源->方法虚拟存在节点,
            "copy ownership and source");
        for (std::size_t i = 0; i < 源->条件结果组.size(); ++i) {
            const auto& 左 = 源->条件结果组[i];
            const auto& 右 = 副本->条件结果组[i];
            要求(左.配对节点 != 右.配对节点 && 左.条件节点 != 右.条件节点
                && 左.结果节点 != 右.结果节点, "internal pair identities");
        }
        要求(!复制.节点映射组.empty(), "copy mapping");
        for (const auto& 映射 : 复制.节点映射组) {
            要求(映射.源节点 != 映射.实例节点, "independent copied node");
        }
        要求(方法.获取方法(源->节点) == 源, "source unchanged by copy");
    }

    void 检查拒绝(const 新方法知识建立请求& 请求) {
        const auto 前 = 方法.查询候选方法({});
        要求(前.状态 == 新方法操作状态::已找到, "candidate snapshot");
        const auto 结果 = 方法.建立方法知识(请求);
        要求(结果.状态 == 新方法操作状态::条件结果不合法 && !结果.方法节点,
            "invalid domain must be rejected");
        const auto 后 = 方法.查询候选方法({});
        要求(后.状态 == 前.状态 && 后.方法节点组 == 前.方法节点组,
            "invalid request must not publish a method");
    }
};

// 直接消费真实领域公开入口；不用身份锚代替正式需求，也不启动外设或self。
struct 任务值域自检服务 final {
    自检服务& 基础;
    新_需求类 需求;
    新_任务类 任务;
    稳定编码 服务根;
    稳定编码 安全根;
    稳定编码 自我;
    稳定编码 安全类型;

    explicit 任务值域自检服务(自检服务& 服务)
        : 基础(服务), 需求(服务.特征值, 服务.存在, 服务.特征,
              服务.特征概念, 服务.二次概念),
          任务(需求, 服务.方法, 服务.特征值, 服务.特征,
              服务.特征概念, 服务.状态, 服务.场景, 服务.存在,
              服务.二次概念, 服务.存在概念) {
        要求(基础.场景.初始化() && 基础.二次概念.初始化()
            && 需求.初始化() && 任务.初始化治理(), "task owners initialization");
        const auto 根 = 基础.场景.建立根场景();
        要求(根.场景节点.has_value(), "task scene root");
        安全类型 = 新类型();
        const auto 服务类型 = 新类型();
        新存在建立请求 存在请求;
        存在请求.初始特征组 = {{安全类型, 新特征准确值{整数{0}}},
            {服务类型, 新特征准确值{整数{0}}}};
        const auto 存在 = 基础.场景.建立成员存在(*根.场景节点, 存在请求,
            {新状态时间语义::实例绝对UTC, 1});
        要求(存在.存在节点.has_value(), "task self existence");
        自我 = *存在.存在节点;
        const auto 安全域 = 建域(安全类型, {100, 100});
        新需求双根初始化请求 请求;
        请求.安全根 = {自我, 目标(安全类型, 安全域), 100};
        新需求目标定义 增加;
        增加.目标特征类型概念节点 = 服务类型;
        增加.目标形式 = 新需求目标形式::变化方向;
        增加.目标变化方向 = 新需求变化方向::增加;
        请求.服务根 = {自我, 增加, 100};
        const auto 双根 = 需求.建立或取得本能双根(请求);
        要求(双根.安全根 && 双根.服务根, "formal demand roots");
        安全根 = *双根.安全根;
        服务根 = *双根.服务根;
    }

    稳定编码 新类型() {
        特征概念定义 定义;
        定义.单位 = 全局基础数据集.新建节点();
        要求(定义.单位 && 有效(*定义.单位), "isolated type unit");
        定义.值域 = std::vector<区间>{{0, 100}};
        const auto 类型 = 基础.特征概念.建立或取得特征概念(定义, 基础.特征值);
        要求(有效(类型), "isolated task feature type");
        return 类型;
    }

    稳定编码 建域(稳定编码 类型, 区间 域) {
        const auto 根 = 基础.特征概念.获取特征概念(类型);
        要求(根.has_value(), "task domain type");
        特征概念定义 定义;
        定义.单位 = 根->单位;
        定义.值域 = std::vector<区间>{域};
        const auto 概念 = 基础.特征概念.建立或取得特征概念(定义, 基础.特征值, 类型);
        要求(有效(概念), "task domain concept");
        return 概念;
    }

    新需求目标定义 目标(稳定编码 类型, 稳定编码 域,
        二次特征结果域种类 关系 = 二次特征结果域种类::包含于参照值域) {
        const std::vector<二次特征概念输入定义> 输入{
            {0, 二次特征输入角色::左输入, 二次特征输入来源种类::特征准确值, 类型},
            {1, 二次特征输入角色::右输入, 二次特征输入来源种类::特征准确值, 类型}};
        const auto 根 = 基础.二次概念.建立或取得默认二次特征概念(
            二次特征算法::有向特征差值, 输入, 域);
        要求(根.概念节点.has_value(), "target comparison root");
        const auto 子 = 基础.二次概念.建立或取得参照结果域概念(*根.概念节点, 关系);
        要求(子.概念节点.has_value(), "target comparison relation");
        新需求目标定义 定义;
        定义.目标特征类型概念节点 = 类型;
        定义.目标值域概念节点 = 域;
        定义.目标比较关系概念节点 = *子.概念节点;
        return 定义;
    }

    struct 样例 final {
        稳定编码 任务节点;
        稳定编码 场景节点;
        稳定编码 类型;
        稳定编码 方法节点;
    };

    样例 建样例(区间 条件域, 区间 结果域, bool 直接,
        std::optional<整数> 当前值 = 整数{0},
        二次特征结果域种类 目标关系 = 二次特征结果域种类::包含于参照值域,
        bool 禁止直接域 = false,
        std::optional<新需求变化方向> 需求方向 = std::nullopt) {
        样例 材料;
        材料.类型 = 新类型();
        const auto 场景 = 基础.场景.建立场景(基础.场景根);
        要求(场景.场景节点.has_value(), "task child scene");
        材料.场景节点 = *场景.场景节点;
        新存在建立请求 对象请求;
        // 空当前值样例仍以另一真实特征合法建立存在，再添加区间值特征。
        对象请求.初始特征组 = 当前值
            ? std::vector<新存在初始特征>{{材料.类型, 新特征准确值{*当前值}}}
            : std::vector<新存在初始特征>{{安全类型, 新特征准确值{整数{0}}}};
        const auto 对象 = 基础.场景.建立成员存在(材料.场景节点, 对象请求,
            {新状态时间语义::实例绝对UTC, 2});
        要求(对象.存在节点.has_value(), "task target existence");
        if (!当前值) {
            (void)基础.场景.添加场景内存在特征区间值(材料.场景节点,
                *对象.存在节点, 材料.类型, 特征概念值域{std::vector<区间>{{0, 100}}});
            const auto 信息 = 基础.存在.获取存在(*对象.存在节点);
            要求(信息 && 信息->特征节点组.size() == 2, "range-only target feature");
        }
        新普通需求建立请求 请求;
        请求.父需求节点 = 服务根;
        请求.目标存在节点 = *对象.存在节点;
        const auto 目标域 = 目标关系 == 二次特征结果域种类::等于
            ? 区间{50, 50} : 区间{50, 60};
        请求.目标 = 目标(材料.类型, 建域(材料.类型, 目标域), 目标关系);
        if (需求方向) {
            请求.目标.目标形式 = 新需求目标形式::变化方向;
            请求.目标.目标变化方向 = 需求方向;
            请求.目标.目标值域概念节点.reset();
            请求.目标.目标比较关系概念节点.reset();
        }
        请求.来源类型 = 新需求来源类型::本能定义;
        const auto 需求结果 = 需求.建立或取得普通需求(请求);
        要求(需求结果.需求节点.has_value(), "formal value demand");
        const auto 任务结果 = 任务.建立或取得任务(*需求结果.需求节点);
        要求(任务结果.任务节点.has_value(), "formal task creation");
        材料.任务节点 = *任务结果.任务节点;
        新方法知识建立请求 方法请求;
        方法请求.方法头.涉及特征类型概念组 = {材料.类型};
        方法请求.方法头.可能结果概念组 = {材料.类型};
        新方法参数规格 规格;
        规格.参数角色节点 = 材料.类型;
        规格.特征规格组 = {{材料.类型, std::nullopt, {}}};
        方法请求.参数规格组 = {规格};
        新方法条件结果定义 配对;
        新方法条件项定义 条件;
        条件.参数角色节点 = 材料.类型;
        条件.特征类型概念节点 = 材料.类型;
        新方法结果项定义 结果;
        结果.参数角色节点 = 材料.类型;
        结果.特征类型概念节点 = 材料.类型;
        结果.主要结果 = true;
        if (直接) {
            条件.精确数值域 = 条件域;
            结果.精确数值域 = 结果域;
        } else {
            条件.特征值域概念节点 = 建域(材料.类型, 条件域);
            结果.特征值域概念节点 = 建域(材料.类型, 结果域);
        }
        配对.条件.条件项组 = {条件};
        配对.结果.结果项组 = {结果};
        if (禁止直接域) {
            auto 禁止 = 结果;
            禁止.主要结果 = false;
            禁止.特征值域概念节点.reset();
            禁止.精确数值域 = 区间{90, 95};
            配对.结果.确认要求.禁止结果关系规则 = 新方法项关系规则::单项;
            配对.结果.确认要求.禁止结果项组 = {禁止};
        }
        方法请求.条件结果组 = {配对};
        const auto 方法结果 = 基础.方法.建立方法知识(方法请求);
        要求(方法结果.方法节点.has_value(), "task method knowledge");
        材料.方法节点 = *方法结果.方法节点;
        要求(任务.准备任务查找方法(材料.任务节点) == 新任务操作状态::已修改,
            "task prepare recall");
        const auto 候选 = 任务.查询任务方法候选并发布(材料.任务节点);
        要求(候选.候选材料节点.has_value(), "task candidate publication");
        const auto 消费 = 任务.消费任务方法候选(材料.任务节点, *候选.候选材料节点);
        要求(消费.状态 == 新任务治理候选消费状态::已进入条件检查, "task candidate consumption");
        const auto 首次任务 = 任务.获取任务(材料.任务节点);
        const auto 调度 = 任务.读取任务调度材料(材料.任务节点);
        要求(调度.状态 == 新任务调度读取状态::已读取 && 调度.项组.size() == 1
            && 调度.项组.front().状态 == 新任务调度项状态::普通可筹办
            && !调度.项组.front().定位.方法执行组织节点
            && !调度.项组.front().条件核验
            && 任务.获取任务(材料.任务节点) == 首次任务,
            "first candidate must be schedulable before execution organization delivery");
        const auto 交付 = 任务.交付候选方法执行组织(材料.任务节点);
        要求(交付.状态 == 新任务执行组织状态::已建立 && 交付.组织,
            "task execution organization delivery");
        const auto 读回 = 任务.获取任务(材料.任务节点);
        要求(读回 && 读回->方法执行组织节点 == 交付.组织->节点
            && !读回->方法实例节点, "task owns execution organization, not a copied method");
        return 材料;
    }
    // 领域服务级观察样例：公开X入口冻结预测、场景owner生成正式状态、
    // 任务owner生成正式A。不调用本能函数，不伪造动作输出归因或完成事实。
    新任务影响评估结果 形成观察评估结果(const 样例& 材料,
        const std::vector<std::pair<整数, 整数>>& 观察值时间组,
        bool 预测减少 = false) {
        const auto 任务信息 = 任务.获取任务(材料.任务节点);
        要求(任务信息 && 任务信息->方法执行组织节点, "assessment task organization");
        const auto 需求信息 = 需求.获取需求(任务信息->需求节点);
        要求(需求信息.has_value(), "assessment demand");
        const auto 对象 = 基础.存在.获取存在(需求信息->目标存在节点);
        要求(对象 && 对象->特征节点组.size() == 1, "assessment target feature");
        const auto 特征节点 = 对象->特征节点组.front();
        const auto 特征 = 基础.特征.获取特征(特征节点);
        要求(特征 && 特征->当前值, "assessment before value");
        const auto 选择 = 任务.选择任务方法结果(材料.任务节点);
        要求(选择.状态 == 新任务方法选择状态::已选定 && 选择.本能采用,
            "assessment selected source adoption");
        const auto& 采用 = *选择.本能采用;
        新任务预计特征变化 变化;
        变化.作用对象节点 = 对象->节点;
        变化.特征类型概念节点 = 材料.类型;
        变化.预期值域概念节点 = 采用.结果.结果项组.front().特征值域概念节点;
        变化.预期精确数值域 = 采用.结果.结果项组.front().精确数值域;
        if (预测减少) {
            const std::vector<二次特征概念输入定义> 输入{
                {0, 二次特征输入角色::左输入, 二次特征输入来源种类::特征准确值, 材料.类型},
                {1, 二次特征输入角色::右输入, 二次特征输入来源种类::特征准确值, 材料.类型}};
            const auto 根 = 基础.二次概念.建立或取得默认二次特征概念(
                二次特征算法::有向特征差值, 输入);
            要求(根.概念节点.has_value(), "prediction direction root");
            const auto 减少 = 基础.二次概念.建立或取得参照结果域概念(
                *根.概念节点, 二次特征结果域种类::小于);
            要求(减少.概念节点.has_value(), "prediction decrease concept");
            变化.预期变化关系节点 = *减少.概念节点;
            变化.预期方向 = 新任务预期变化方向::减少;
        }
        新任务执行开始请求 开始;
        开始.任务节点 = 材料.任务节点;
        开始.方法节点 = 采用.来源方法节点;
        开始.本能采用 = 采用;
        开始.开始时间 = {新状态时间语义::实例绝对UTC, 10};
        开始.预计变化阶段组 = {{0, {变化}}};
        开始.结果获取方案.渠道方案组 = {新任务结果获取渠道方案{}};
        开始.结果获取方案.目标组 = {{对象->节点, 特征节点, 材料.类型,
            *特征->当前值, true}};
        要求(开始.结果获取方案.完整(), "assessment observation plan");
        要求(任务.确认任务进入待执行(材料.任务节点, 采用)
            == 新任务治理待执行确认状态::已确认, "assessment adoption readiness");
        const auto 执行 = 任务.开始方法执行(开始);
        要求(执行.执行实例节点.has_value(), "assessment owner begin execution");
        std::vector<稳定编码> 状态组;
        整数 结束时刻 = 12;
        for (const auto& [观察值, 时刻] : 观察值时间组) {
            const auto 变化结果 = 基础.场景.更新场景内存在特征值(材料.场景节点,
                对象->节点, 特征节点, 新特征准确值{观察值},
                {新状态时间语义::实例绝对UTC, 时刻});
            要求(变化结果.状态节点.has_value(), "assessment real observed state");
            状态组.push_back(*变化结果.状态节点);
            if (时刻 >= 结束时刻) 结束时刻 = 时刻 + 1;
        }
        const auto 结束 = 任务.结束方法执行({材料.任务节点, *执行.执行实例节点,
            {新状态时间语义::实例绝对UTC, 结束时刻}, 状态组});
        要求(结束 == 新任务操作状态::已结束执行, "assessment owner end execution");
        const auto 评估 = 任务.评估方法执行影响(材料.任务节点, *执行.执行实例节点);
        if (评估.评估) {
            const auto 读回 = 任务.获取影响评估(评估.评估->节点);
            要求(读回 && 评估材料相同(*读回, *评估.评估),
                "assessment official readback");
        } else {
            const auto 读回 = 任务.获取执行实例(*执行.执行实例节点);
            要求(读回 && !读回->影响评估节点
                && 读回->状态 == 新任务执行状态::已结束待评估,
                "rejected assessment left a partial published evaluation");
        }
        return 评估;
    }

    新任务影响评估信息 形成观察评估(const 样例& 材料, 整数 观察后值,
        bool 预测减少 = false) {
        const auto 评估 = 形成观察评估结果(材料, {{观察后值, 11}}, 预测减少);
        要求(评估.评估.has_value(), "assessment owner published evaluation");
        return *评估.评估;
    }
};
} // namespace

int 运行方法精确值域自检() noexcept {
    std::size_t 通过 = 0, 失败 = 0;
    const auto 检查 = [&](const std::string& 名称, const auto& 函数) {
        try {
            函数();
            ++通过;
            std::cout << "PASS " << 名称 << '\n';
        } catch (const std::exception& 异常) {
            ++失败;
            std::cout << "FAIL " << 名称 << ": " << 异常.what() << '\n';
        } catch (...) {
            ++失败;
            std::cout << "FAIL " << 名称 << ": unknown exception\n";
        }
    };
    try {
        检查("startup mode", [] {
            char 程序[] = "self-test", 模式[] = "--self-test-method-domain";
            char 其它[] = "--self-test-simulation";
            char* 参数[] = {程序, 模式, 模式};
            const auto 单项 = 解析并验证启动选项(2, 参数);
            要求(单项.成功() && 单项.选项.模式 == 启动模式::方法精确值域自检,
                "mode accepted");
            要求(解析并验证启动选项(3, 参数).状态 == 启动选项解析状态::重复模式参数,
                "duplicate mode rejected");
            参数[2] = 其它;
            要求(解析并验证启动选项(3, 参数).状态 == 启动选项解析状态::模式组合冲突,
                "mixed mode rejected");
        });
        检查("field indexes preserve updates ordering and atomic publication", [] {
            基础数据集 数据;
            const auto 所属 = 数据.新建节点(), 甲 = 数据.新建节点(), 乙 = 数据.新建节点();
            const auto 目标甲 = 数据.新建节点(), 目标乙 = 数据.新建节点();
            要求(有效(所属) && 有效(甲) && 有效(乙) && 有效(目标甲) && 有效(目标乙), "index test nodes");
            const auto 先 = 数据.添加字段值(所属, 乙, 基础值{整数{1}});
            const auto 中 = 数据.添加字段节点(所属, 甲, 目标甲);
            const auto 后 = 数据.添加字段节点(所属, 乙, 目标甲);
            要求(有效(先) && 有效(中) && 有效(后), "index test fields");
            const auto 正向 = 数据.查询字段(所属, 乙);
            要求(正向.size() == 2 && 正向[0].编码 == 先 && 正向[1].编码 == 后, "multi-field order");
            const auto 完整 = 数据.查询完整节点结构(所属);
            要求(完整.结构 && 完整.结构->字段组.size() == 3
                && 完整.结构->字段组[0].编码 == 先 && 完整.结构->字段组[1].编码 == 中
                && 完整.结构->字段组[2].编码 == 后, "cross-type identity order");
            要求(数据.查询目标字段(目标甲, 甲).size() == 1, "reverse insertion");
            要求(数据.修改字段值(中, 基础值{static_cast<整数>(目标甲.值)})
                && 数据.查询目标字段(目标甲, 甲).empty(), "integer is not a node reference");
            要求(数据.修改字段节点(中, 目标乙) && 数据.修改字段节点(中, 目标乙)
                && 数据.查询目标字段(目标乙, 甲).size() == 1, "value to reference and repeated update");
            要求(数据.修改字段节点(中, 目标甲) && 数据.查询目标字段(目标乙, 甲).empty()
                && 数据.查询目标字段(目标甲, 甲).size() == 1, "reference target replacement");
            要求(数据.删除字段(后) && 数据.查询目标字段(目标甲, 乙).empty()
                && 数据.查询字段(所属, 乙).size() == 1, "ordinary deletion removes both indexes");
            const auto 原字段 = 数据.查询完整节点结构(所属).结构->字段组;
            std::vector<基础组内新节点> 组(2);
            组[0].字段组.push_back({乙, 基础组内节点引用{1}});
            组[0].字段组.push_back({甲, 基础值{整数{9}}});
            组[0].字段组.push_back({甲, 目标甲});
            const auto 发布 = 数据.原子建立节点组(组);
            要求(发布.状态 == 基础结构操作状态::已完成 && 发布.节点组.size() == 2, "atomic index publication");
            const auto 引用 = 数据.查询字段引用节点结构(发布.节点组[1], 乙);
            要求(引用.状态 == 基础结构操作状态::已完成 && 引用.引用字段组.size() == 1
                && 引用.来源节点组.size() == 1 && 引用.来源节点组[0].字段组.size() == 3
                && 引用.来源节点组[0].字段组[0].编码.值 < 引用.来源节点组[0].字段组[1].编码.值
                && 引用.来源节点组[0].字段组[1].编码.值 < 引用.来源节点组[0].字段组[2].编码.值,
                "atomic reverse and complete source read");
            要求(数据.查询目标字段(目标甲, 甲).size() == 2, "atomic external target indexed");
            要求(数据.查询完整节点结构(所属).结构->字段组 == 原字段, "atomic publication preserves old fields");
            const auto 拒绝 = 数据.原子删除节点结构(发布.节点组, {}, {});
            要求(拒绝 == 基础结构操作状态::内部不一致
                && 数据.查询目标字段(发布.节点组[1], 乙).size() == 1, "rejected deletion changes no index");
            std::vector<稳定编码> 删字段;
            for (const auto& 字段 : 引用.来源节点组[0].字段组) 删字段.push_back(字段.编码);
            要求(数据.原子删除节点结构(发布.节点组, 删字段, {}) == 基础结构操作状态::已完成
                && !数据.查询节点(发布.节点组[0]) && !数据.查询节点(发布.节点组[1])
                && 数据.查询完整节点结构(所属).结构->字段组 == 原字段
                && 数据.查询目标字段(目标甲, 甲).size() == 1
                && 数据.查询目标字段(目标甲, 甲)[0].编码 == 中, "atomic delete and unrelated preservation");
            要求(数据.删除字段(中) && 数据.查询目标字段(目标甲, 甲).empty(), "last reverse item removed");
            要求(!数据.修改字段节点(稳定编码{}, 目标甲) && !数据.删除字段(稳定编码{}), "legal missing field rejection");
        });
        自检服务 服务;
        const auto 概念前 = 服务.特征概念.获取特征概念(服务.数值类型);
        const auto 子概念前 = 服务.特征概念.获取特征概念(服务.数值子概念);
        const auto 文本前 = 服务.特征概念.获取特征概念(服务.文本类型);
        要求(概念前 && 子概念前 && 文本前, "concept snapshots");
        检查("singleton and forbidden range roundtrip/copy", [&] {
            服务.建立并检查复制(服务.请求());
        });
        检查("finite intervals roundtrip/copy", [&] {
            auto 请求 = 服务.请求();
            auto& 配对 = 请求.条件结果组.front();
            配对.条件.条件项组.front().精确数值域 = 区间{-9, 8};
            配对.结果.结果项组.front().精确数值域 = 区间{-7, 9};
            服务.建立并检查复制(请求);
        });
        检查("legacy concept domains roundtrip/copy", [&] {
            auto 请求 = 服务.请求();
            auto& 配对 = 请求.条件结果组.front();
            auto 清除 = [&](auto& 项) {
                项.精确数值域.reset();
                项.特征值域概念节点 = 服务.数值子概念;
            };
            清除(配对.条件.条件项组.front());
            清除(配对.结果.结果项组.front());
            清除(配对.结果.确认要求.禁止结果项组.front());
            服务.建立并检查复制(请求);
        });
        检查("finite I64 endpoints preserved without unit conversion", [&] {
            auto 请求 = 服务.请求();
            请求.条件结果组.front().条件.条件项组.front().精确数值域 = 区间{
                (std::numeric_limits<整数>::min)() + 1,
                (std::numeric_limits<整数>::max)() - 1};
            服务.建立并检查复制(请求);
        });
        检查("domain participates in equality and deduplication", [&] {
            auto 请求 = 服务.请求();
            const auto 第一 = 服务.方法.建立方法知识(请求);
            auto 原条件 = 请求.条件结果组.front().条件;
            请求.条件结果组.front().条件.条件项组.front().精确数值域 = 区间{4, 4};
            要求(原条件 != 请求.条件结果组.front().条件, "domain ignored in equality");
            const auto 第二 = 服务.方法.建立方法知识(请求);
            要求(第一.状态 == 新方法操作状态::已建立
                && 第二.状态 == 新方法操作状态::已建立
                && 第一.方法节点 != 第二.方法节点, "different domains deduplicated");
        });
        检查("composite copy preserves child private domains", [&] {
            const auto 子请求 = 服务.请求();
            const auto 子 = 服务.方法.建立方法知识(子请求);
            要求(子.方法节点.has_value(), "child creation");
            auto 父请求 = 服务.请求();
            父请求.实现方式 = 新方法实现方式::组合;
            auto& 配对 = 父请求.条件结果组.front();
            配对.条件.条件项组.front().精确数值域.reset();
            配对.结果.结果项组.front().精确数值域.reset();
            配对.结果.确认要求.禁止结果项组.front().精确数值域.reset();
            const auto 父 = 服务.方法.建立方法知识(父请求);
            要求(父.方法节点.has_value(), "composite creation");
            const auto 步骤 = 服务.方法.添加组合步骤({*父.方法节点, *子.方法节点, 0});
            要求(步骤.节点.has_value(), "composite step");
            要求(服务.方法.设置组合入口步骤(*父.方法节点, *步骤.节点)
                == 新方法操作状态::无变化, "composite first step is already entry");
            const auto 源父 = 服务.方法.获取方法(*父.方法节点);
            const auto 源子 = 服务.方法.获取方法(*子.方法节点);
            const auto 复制 = 服务.方法.复制方法(*父.方法节点);
            要求(复制.状态 == 新方法复制状态::已复制 && 复制.方法实例节点,
                "composite copy");
            const auto 副本 = 服务.方法.获取方法(*复制.方法实例节点);
            要求(副本 && 副本->组合步骤组.size() == 1, "copied composite steps");
            const auto 子副本 = 服务.方法.获取方法(副本->组合步骤组.front().调用方法节点);
            要求(子副本 && 子副本->来源方法节点 == 子.方法节点
                && 子副本->实例根节点 == 副本->节点
                && 子副本->节点 != *子.方法节点
                && 子副本->条件结果组.size() == 1, "copied child identity");
            要求(子副本->条件结果组.front().条件 == 子请求.条件结果组.front().条件
                && 子副本->条件结果组.front().结果 == 子请求.条件结果组.front().结果,
                "child private domains lost in closure copy");
            要求(服务.方法.获取方法(*父.方法节点) == 源父
                && 服务.方法.获取方法(*子.方法节点) == 源子, "source graph changed");
        });
        const char* 位置名[] = {"condition", "result", "forbidden"};
        const char* 错误名[] = {"dual domain", "missing FT", "non-I64 FT",
            "inverted", "MIN sentinel", "MAX sentinel", "non-concept FT"};
        for (int 位置 = 0; 位置 < 3; ++位置) {
            for (int 错误 = 0; 错误 < 7; ++错误) {
                检查(std::string(位置名[位置]) + " rejects " + 错误名[错误], [&] {
                    auto 请求 = 服务.请求();
                    auto 修改 = [&](auto& 项) {
                        switch (错误) {
                        case 0: 项.特征值域概念节点 = 服务.数值子概念; break;
                        case 1: 项.特征类型概念节点.reset(); break;
                        case 2: 项.特征类型概念节点 = 服务.文本类型; break;
                        case 3: 项.精确数值域 = 区间{3, 2}; break;
                        case 4: 项.精确数值域 = 区间{(std::numeric_limits<整数>::min)(), 0}; break;
                        case 5: 项.精确数值域 = 区间{0, (std::numeric_limits<整数>::max)()}; break;
                        case 6: 项.特征类型概念节点 = 服务.单位; break;
                        }
                    };
                    auto& 配对 = 请求.条件结果组.front();
                    if (位置 == 0) 修改(配对.条件.条件项组.front());
                    else if (位置 == 1) 修改(配对.结果.结果项组.front());
                    else 修改(配对.结果.确认要求.禁止结果项组.front());
                    服务.检查拒绝(请求);
                });
            }
        }
        检查("referenced concepts unchanged", [&] {
            要求(服务.特征概念.获取特征概念(服务.数值类型) == 概念前
                && 服务.特征概念.获取特征概念(服务.数值子概念) == 子概念前
                && 服务.特征概念.获取特征概念(服务.文本类型) == 文本前,
                "method operation changed shared feature concepts");
        });

        // 仅通过正式服务构造隔离概念；以下比较不调用任务或修改方法。
        const auto 准确输入 = [&](整数 值) {
            return 新特征值域比较输入{服务.数值类型, 服务.单位,
                新特征准确I64比较材料{值}};
        };
        const auto 区间输入 = [&](整数 下界, 整数 上界) {
            return 新特征值域比较输入{服务.数值类型, 服务.单位,
                新特征直接I64值域比较材料{{下界, 上界}}};
        };
        const auto 概念输入 = [&](稳定编码 类型, 稳定编码 域) {
            const auto 信息 = 服务.特征概念.获取特征概念(类型);
            要求(信息.has_value(), "comparison type readback");
            return 新特征值域比较输入{类型, 信息->单位,
                新特征概念值域比较材料{域}};
        };
        const auto 要求关系 = [&](const 新特征值域比较输入& 左,
            const 新特征值域比较输入& 右, 整数 预期) {
            const auto 结果 = 服务.特征.比较特征值域(左, 右);
            要求(结果.状态 == 新特征比较状态::已完成
                && 结果.关系编码 == 预期, "domain relation code");
        };
        const auto 要求拒绝 = [&](const 新特征值域比较输入& 左,
            const 新特征值域比较输入& 右, 新特征比较状态 预期) {
            const auto 正向 = 服务.特征.比较特征值域(左, 右);
            const auto 反向 = 服务.特征.比较特征值域(右, 左);
            要求(正向.状态 == 预期 && !正向.关系编码
                && 反向.状态 == 预期 && !反向.关系编码,
                "rejection must preserve status without a relation payload");
        };
        const 新特征值域比较输入 三来源[] = {准确输入(0), 区间输入(-10, 10),
            概念输入(服务.数值类型, 服务.数值子概念)};
        const char* 来源名[] = {"exact", "direct", "concept"};
        const 整数 来源关系[3][3] = {{1, 3, 3}, {2, 1, 1}, {2, 1, 1}};
        for (std::size_t 左 = 0; 左 < 3; ++左) {
            for (std::size_t 右 = 0; 右 < 3; ++右) {
                检查(std::string("feature domain sources ") + 来源名[左]
                    + "/" + 来源名[右], [&] {
                    要求关系(三来源[左], 三来源[右], 来源关系[左][右]);
                });
            }
        }
        struct 关系样例 final {
            区间 左;
            区间 右;
            整数 正向;
            整数 反向;
        };
        const 关系样例 五关系[] = {
            {{0, 0}, {0, 0}, 1, 1},
            {{-10, 10}, {-10, -10}, 2, 3},
            {{10, 10}, {-10, 10}, 3, 2},
            {{-10, 0}, {0, 10}, 4, 4},
            {{-10, -1}, {0, 10}, 5, 5}
        };
        for (const auto& 样例 : 五关系) {
            检查("feature domain relation " + std::to_string(样例.正向), [&] {
                const auto 左 = 区间输入(样例.左.下界, 样例.左.上界);
                const auto 右 = 区间输入(样例.右.下界, 样例.右.上界);
                要求关系(左, 右, 样例.正向);
                要求关系(右, 左, 样例.反向);
            });
        }
        const auto 最小 = (std::numeric_limits<整数>::min)();
        const auto 最大 = (std::numeric_limits<整数>::max)();
        检查("feature domain full I64 endpoints and zero", [&] {
            const auto 全域 = 区间输入(最小, 最大);
            for (const auto 值 : {最小, 整数{-1}, 整数{0}, 最大}) {
                要求关系(准确输入(值), 区间输入(值, 值), 1);
                要求关系(全域, 准确输入(值), 2);
                要求关系(准确输入(值), 全域, 3);
            }
            要求关系(准确输入(最小), 准确输入(最大), 5);
            要求关系(准确输入(最大), 准确输入(最小), 5);
            要求关系(全域, 全域, 1);
        });
        检查("feature domain inverted range rejected", [&] {
            要求拒绝(区间输入(1, -1), 准确输入(0), 新特征比较状态::入口拒绝);
        });
        检查("feature domain monostate is not zero or unlimited", [&] {
            新特征值域比较输入 缺失{服务.数值类型, 服务.单位, std::monostate{}};
            要求拒绝(缺失, 准确输入(0), 新特征比较状态::入口拒绝);
            要求拒绝(缺失, 区间输入(最小, 最大), 新特征比较状态::入口拒绝);
            要求拒绝({}, 准确输入(0), 新特征比较状态::入口拒绝);
        });
        检查("feature domain empty type rejected", [&] {
            auto 输入 = 准确输入(0);
            输入.特征类型概念节点 = {};
            要求拒绝(输入, 准确输入(0), 新特征比较状态::入口拒绝);
        });
        检查("feature domain empty domain concept rejected", [&] {
            auto 输入 = 概念输入(服务.数值类型, {});
            要求拒绝(输入, 准确输入(0), 新特征比较状态::入口拒绝);
        });
        检查("feature domain non-concept type rejected", [&] {
            auto 输入 = 准确输入(0);
            输入.特征类型概念节点 = 服务.单位;
            要求拒绝(输入, 准确输入(0), 新特征比较状态::概念不存在);
        });
        检查("feature domain non-concept domain rejected", [&] {
            要求拒绝(概念输入(服务.数值类型, 服务.单位), 准确输入(0),
                新特征比较状态::概念不存在);
        });
        检查("feature domain deleted concept rejected", [&] {
            要求(服务.特征.初始化(), "feature lifecycle service initialization");
            特征概念定义 定义;
            定义.单位 = 服务.单位;
            定义.值域 = std::vector<区间>{{-100, 100}};
            const auto 待删 = 服务.特征概念.建立或取得特征概念(定义, 服务.特征值);
            要求(有效(待删), "deletion concept allocation");
            const auto 实例 = 服务.特征.建立特征(待删);
            要求(有效(实例), "feature instance creation for concept reference");
            要求(服务.特征.删除特征(实例), "feature instance removal");
            要求(!服务.特征概念.获取特征概念(待删),
                "zero-reference leaf concept removal");
            auto 输入 = 准确输入(0);
            输入.特征类型概念节点 = 待删;
            要求拒绝(输入, 准确输入(0), 新特征比较状态::概念不存在);
            要求拒绝(概念输入(服务.数值类型, 待删), 准确输入(0),
                新特征比较状态::概念不存在);
        });
        检查("feature domain distinct roots with identical material rejected", [&] {
            特征概念定义 定义;
            定义.单位 = 服务.单位;
            定义.值域 = std::vector<区间>{{-100, 100}};
            const auto 另一类型 = 服务.特征概念.建立或取得特征概念(定义, 服务.特征值);
            要求(有效(另一类型) && 另一类型 != 服务.数值类型, "independent type root");
            auto 输入 = 准确输入(0);
            输入.特征类型概念节点 = 另一类型;
            要求拒绝(输入, 准确输入(0), 新特征比较状态::概念不相容);
            要求拒绝(概念输入(服务.数值类型, 另一类型), 准确输入(0),
                新特征比较状态::概念不相容);
        });
        检查("feature domain subtype supplies same type identity", [&] {
            auto 输入 = 区间输入(-10, 10);
            输入.特征类型概念节点 = 服务.数值子概念;
            要求关系(输入, 三来源[2], 1);
            要求关系(概念输入(服务.数值子概念, 服务.数值类型), 输入, 2);
        });
        检查("feature domain missing unit rejected", [&] {
            auto 输入 = 准确输入(0);
            输入.单位.reset();
            要求拒绝(输入, 准确输入(0), 新特征比较状态::单位不相容);
        });
        检查("feature domain different explicit unit rejected", [&] {
            auto 输入 = 区间输入(-10, 10);
            输入.单位 = 全局基础数据集.新建节点();
            要求(输入.单位 && 有效(*输入.单位), "other unit allocation");
            要求拒绝(输入, 准确输入(0), 新特征比较状态::单位不相容);
        });
        检查("feature domain values outside concept do not change concepts", [&] {
            // 完整概念信息包含引用数；额外读回双向父子与兄弟关系。
            const 稳定编码 节点组[] = {特征概念树, 服务.数值类型, 服务.数值子概念};
            std::vector<std::optional<特征概念信息>> 概念快照;
            std::vector<std::vector<基础外部关系>> 关系快照;
            const auto 读取关系 = [&] {
                std::vector<std::vector<基础外部关系>> 结果;
                for (const auto 节点 : 节点组) {
                    for (const auto 类型 : {基础外部关系类型::父子, 基础外部关系类型::兄弟}) {
                        结果.push_back(全局基础数据集.查询源关系(节点, 类型));
                        结果.push_back(全局基础数据集.查询目标关系(节点, 类型));
                    }
                }
                return 结果;
            };
            for (const auto 节点 : 节点组) 概念快照.push_back(服务.特征概念.获取特征概念(节点));
            关系快照 = 读取关系();
            auto 左 = 准确输入(1000);
            auto 右 = 区间输入(999, 1001);
            左.特征类型概念节点 = 服务.数值子概念;
            右.特征类型概念节点 = 服务.数值子概念;
            要求关系(左, 右, 3);
            要求关系(右, 左, 2);
            要求关系(右, 三来源[2], 5);
            要求拒绝(区间输入(2, 1), 左, 新特征比较状态::入口拒绝);
            for (std::size_t i = 0; i < std::size(节点组); ++i) {
                要求(服务.特征概念.获取特征概念(节点组[i]) == 概念快照[i],
                    "comparison changed concept contents or reference count");
            }
            要求(读取关系() == 关系快照, "comparison changed concept relationships");
        });
        检查("feature domain non-I64 exact and direct rejected", [&] {
            const auto 文本 = 概念输入(服务.文本类型, 服务.文本类型);
            auto 准确 = 文本;
            准确.材料 = 新特征准确I64比较材料{0};
            auto 直接 = 文本;
            直接.材料 = 新特征直接I64值域比较材料{{-1, 1}};
            要求拒绝(准确, 文本, 新特征比较状态::材料类型不相容);
            要求拒绝(直接, 文本, 新特征比较状态::材料类型不相容);
        });
        const auto 要求新旧一致 = [&](稳定编码 左, 稳定编码 右, 整数 预期) {
            const auto 旧 = 服务.特征.比较特征概念值域关系(左, 右);
            要求(旧.状态 == 新特征比较状态::已完成 && 旧.关系
                && static_cast<整数>(*旧.关系) == 预期, "legacy domain relation");
            要求关系(概念输入(左, 左), 概念输入(右, 右), 预期);
        };
        检查("feature domain legacy scalar entry equivalent", [&] {
            要求新旧一致(服务.数值类型, 服务.数值子概念, 2);
            要求新旧一致(服务.数值子概念, 服务.数值类型, 3);
            要求新旧一致(服务.数值子概念, 服务.数值子概念, 1);
            const auto 空 = 服务.特征.比较特征概念值域关系({}, 服务.数值类型);
            const auto 非概念 = 服务.特征.比较特征概念值域关系(服务.单位, 服务.数值类型);
            要求(空.状态 == 新特征比较状态::入口拒绝 && !空.关系
                && 非概念.状态 == 新特征比较状态::概念不存在 && !非概念.关系,
                "legacy rejection status preserved");
        });
        检查("feature domain legacy non-I64 finite and unlimited equivalent", [&] {
            const auto 文本值 = 服务.特征值.保存或取得特征值(
                材料物理类型::UTF8字符串, std::string("domain comparison"));
            要求(有效(文本值), "text value creation");
            特征概念定义 定义;
            定义.材料类型 = 特征概念材料物理类型::UTF8字符串;
            定义.比较规则 = 特征概念比较规则::完整材料精确;
            定义.值域 = std::vector<稳定编码>{文本值};
            const auto 子 = 服务.特征概念.建立或取得特征概念(
                定义, 服务.特征值, 服务.文本类型);
            要求(有效(子), "finite text domain creation");
            要求新旧一致(服务.文本类型, 子, 2);
            要求新旧一致(子, 服务.文本类型, 3);
            要求新旧一致(子, 子, 1);
        });
        检查("feature domain legacy structured I64 equivalent", [&] {
            const auto 先天 = 服务.特征概念.获取先天特征概念();
            要求(先天.has_value(), "innate concepts available");
            const auto 根 = 服务.特征概念.获取特征概念(先天->RGB颜色);
            要求(根 && std::holds_alternative<特征概念结构化I64值域>(根->值域),
                "structured I64 source");
            特征概念定义 定义;
            定义.材料类型 = 根->材料类型;
            定义.单位 = 根->单位;
            定义.比较规则 = 根->比较规则;
            定义.聚合规则 = 根->聚合规则;
            auto 结构 = std::get<特征概念结构化I64值域>(根->值域);
            要求(!结构.分量.empty(), "structured dimensions");
            结构.分量.front().值域 = {{0, 10}};
            定义.值域 = 结构;
            const auto 子 = 服务.特征概念.建立或取得特征概念(
                定义, 服务.特征值, 根->节点);
            要求(有效(子), "structured child creation");
            要求新旧一致(根->节点, 子, 2);
            要求新旧一致(子, 根->节点, 3);
            要求新旧一致(子, 子, 1);
            auto 直接 = 概念输入(根->节点, 子);
            直接.材料 = 新特征直接I64值域比较材料{{0, 10}};
            要求拒绝(直接, 概念输入(根->节点, 子), 新特征比较状态::材料类型不相容);
        });
        任务值域自检服务 任务服务{服务};
        const auto 检查条件 = [&](const 任务值域自检服务::样例& 材料,
            新任务方法条件项核验状态 预期) {
            const auto 前 = 任务服务.任务.获取任务(材料.任务节点);
            const auto 方法前 = 服务.方法.获取方法(材料.方法节点);
            const auto 结果 = 任务服务.任务.筹办任务条件和参数(材料.任务节点);
            要求(结果.状态 == 新任务治理条件筹办状态::已完成
                && 结果.方法筹办组.size() == 1, "task condition preparation shape");
            const auto& 方法项 = 结果.方法筹办组.front();
            if (预期 == 新任务方法条件项核验状态::材料未到) {
                // 只读核验已形成配对级缺值；筹办包装入口因缺少目标当前值
                // 尚未选定执行配对，将候选已完整核验置false，不能等同具备。
                要求(!结果.候选已完整核验
                    && 结果.方法选择状态 == 新任务方法选择状态::等待条件
                    && 方法项.状态 == 新任务候选方法筹办状态::材料未到
                    && 方法项.条件结果组.size() == 1
                    && 方法项.条件结果组.front().状态 == 预期
                    && std::any_of(方法项.条件缺口组.begin(), 方法项.条件缺口组.end(),
                        [](const auto& 缺口) {
                            return 缺口.种类 == 新任务方法条件缺口种类::参数特征值未取得;
                        }), ("missing current value must retain an unresolved parameter gap; method_state="
                            + std::to_string(static_cast<int>(方法项.状态))
                            + "; complete=" + std::to_string(结果.候选已完整核验)
                            + "; pairs=" + std::to_string(方法项.条件结果组.size())
                            + "; gaps=" + std::to_string(方法项.条件缺口组.size())).c_str());
            } else {
                要求(结果.候选已完整核验 && 方法项.条件结果组.size() == 1
                    && 方法项.条件结果组.front().状态 == 预期,
                    "task current value/domain membership");
            }
            要求(任务服务.任务.获取任务(材料.任务节点) == 前
                && 服务.方法.获取方法(材料.方法节点) == 方法前,
                "read-only preparation changed knowledge or task");
        };
        for (const auto 值 : {整数{0}, 整数{5}, 整数{10}, 整数{11}}) {
            检查("task direct/concept condition membership " + std::to_string(值), [&] {
                for (const bool 直接 : {false, true}) {
                    const auto 材料 = 任务服务.建样例({0, 10}, {50, 60}, 直接, 值);
                    检查条件(材料, 值 <= 10 ? 新任务方法条件项核验状态::具备
                        : 新任务方法条件项核验状态::不具备);
                    const auto 选择 = 任务服务.任务.选择任务方法结果(材料.任务节点);
                    要求(选择.保证满足目标 && 选择.状态 == (值 <= 10
                            ? 新任务方法选择状态::已选定 : 新任务方法选择状态::等待条件),
                        "domain representation changed selection/condition wait");
                }
            });
        }
        struct 结果用例 final {
            const char* 名称;
            区间 域;
            新任务方法选择状态 状态;
            bool 保证;
        };
        const 结果用例 用例组[]{
            {"equal", {50, 60}, 新任务方法选择状态::已选定, true},
            {"contained", {52, 55}, 新任务方法选择状态::已选定, true},
            {"superset", {40, 70}, 新任务方法选择状态::结果定义暂不支持, false},
            {"overlap", {55, 65}, 新任务方法选择状态::结果定义暂不支持, false},
            {"disjoint increasing", {20, 30}, 新任务方法选择状态::已选定, false},
            {"disjoint decreasing", {-20, -10}, 新任务方法选择状态::无适用结果, false}
        };
        for (const auto& 用例 : 用例组) {
            检查(std::string("task result relation ") + 用例.名称, [&] {
                for (const bool 直接 : {false, true}) {
                    // 超类型根的直接域单独检查；不能为了对照而改写类型概念。
                    if (!直接 && 用例.域.下界 < 0) continue;
                    const auto 材料 = 任务服务.建样例({0, 10}, 用例.域, 直接);
                    const auto 前 = 任务服务.任务.获取任务(材料.任务节点);
                    const auto 选择 = 任务服务.任务.选择任务方法结果(材料.任务节点);
                    要求(选择.状态 == 用例.状态 && 选择.保证满足目标 == 用例.保证,
                        "task result relationship policy");
                    if (用例.状态 == 新任务方法选择状态::已选定 && !用例.保证)
                        要求(选择.有利结果端点 == 用例.域.上界, "same-direction progress endpoint");
                    要求(任务服务.任务.获取任务(材料.任务节点) == 前,
                        "read-only result selection changed task");
                }
            });
        }
        检查("task direct domain outside type root remains an explicit domain", [&] {
            const auto 材料 = 任务服务.建样例({-10, 10}, {120, 130}, true);
            检查条件(材料, 新任务方法条件项核验状态::具备);
            const auto 选择 = 任务服务.任务.选择任务方法结果(材料.任务节点);
            要求(选择.状态 == 新任务方法选择状态::已选定
                && !选择.保证满足目标 && 选择.有利结果端点 == 130,
                "direct result was clamped/admitted against type root");
        });
        检查("task missing current value is material-not-arrived", [&] {
            for (const bool 直接 : {false, true}) {
                const auto 材料 = 任务服务.建样例({0, 10}, {50, 60}, 直接, std::nullopt);
                检查条件(材料, 新任务方法条件项核验状态::材料未到);
                要求(任务服务.任务.选择任务方法结果(材料.任务节点).状态
                    == 新任务方法选择状态::等待条件, "missing current value became zero/unlimited");
            }
        });
        检查("task direct domain explicitly rejects non-containment target", [&] {
            const auto 材料 = 任务服务.建样例({0, 10}, {70, 80}, true, 整数{0},
                二次特征结果域种类::高于参照值域);
            要求(任务服务.任务.选择任务方法结果(材料.任务节点).状态
                == 新任务方法选择状态::结果定义暂不支持, "unsupported target became progress candidate");
        });
        检查("task equality target uses direct result domain", [&] {
            // 等于参照域与包含域都通过正式需求语义，不以比较关系编码大小排序。
            const auto 材料 = 任务服务.建样例({0, 10}, {50, 50}, true, 整数{0},
                二次特征结果域种类::等于);
            const auto 选择 = 任务服务.任务.选择任务方法结果(材料.任务节点);
            要求(选择.状态 == 新任务方法选择状态::已选定 && 选择.保证满足目标,
                "equality result domain not accepted");
        });
        检查("task actual current value inside target interval is already satisfied", [&] {
            for (const auto 当前 : {整数{50}, 整数{55}, 整数{60}, 整数{61}}) {
                const auto 材料 = 任务服务.建样例({0, 100}, {50, 60}, true, 当前);
                const auto 前 = 任务服务.任务.获取任务(材料.任务节点);
                const auto 满足 = 任务服务.任务.复核任务需求(材料.任务节点);
                const auto 选择 = 任务服务.任务.选择任务方法结果(材料.任务节点);
                if (当前 <= 60) {
                    要求(满足.状态 == 新需求当前满足状态::已满足
                        && 选择.状态 == 新任务方法选择状态::目标已满足,
                        "actual value inside target was treated as unmet");
                } else {
                    要求(满足.状态 == 新需求当前满足状态::需减少
                        && 选择.状态 == 新任务方法选择状态::已选定 && 选择.保证满足目标,
                        "actual value above target was treated as satisfied");
                }
                要求(任务服务.任务.获取任务(材料.任务节点) == 前,
                    "current-value satisfaction changed task");
            }
        });
        检查("task exact-domain adoption predicts and enters ready without executing", [&] {
            const auto 材料 = 任务服务.建样例({0, 10}, {50, 60}, true);
            const auto 选择 = 任务服务.任务.选择任务方法结果(材料.任务节点);
            要求(选择.状态 == 新任务方法选择状态::已选定 && 选择.方法节点
                && 选择.配对节点 && 选择.结果节点 && 选择.本能采用, "safety gate selected input");
            const auto 任务前 = 任务服务.任务.获取任务(材料.任务节点);
            const auto 需求前 = 任务服务.需求.查询全部需求();
            const auto 场景前 = 服务.场景.获取场景(材料.场景节点);
            const auto 提议 = 任务服务.任务.提议任务条件子需求(材料.任务节点);
            要求(提议.状态 == 新任务条件子需求提议状态::无需子需求
                && 提议.子需求项组.empty(), "satisfied exact-domain condition created a child demand");
            const auto 核验 = 任务服务.任务.核验任务执行方向(材料.任务节点,
                *选择.本能采用,
                任务服务.自我, 任务服务.安全根, 任务服务.服务根);
            要求(核验.状态 == 新任务治理执行方向核验状态::已核验可执行
                && 核验.预计变化阶段组.size() == 1
                && 核验.预计变化阶段组.front().变化组.size() == 1
                && 核验.预计变化阶段组.front().变化组.front().预期精确数值域
                    == std::optional<区间>{{50, 60}}, "exact result domain lost in prediction");
            const auto 需求后 = 任务服务.需求.查询全部需求();
            要求(任务服务.任务.获取任务(材料.任务节点) == 任务前
                && 服务.场景.获取场景(材料.场景节点) == 场景前
                && 需求后.状态 == 需求前.状态 && 需求后.需求节点组 == 需求前.需求节点组,
                "downstream read changed task/demand/scene");
            // 缺少就绪状态时仍零执行，不把支持精确域等同自动执行。
            新任务执行开始请求 开始;
            开始.任务节点 = 材料.任务节点;
            开始.方法节点 = *选择.方法节点;
            开始.本能采用 = 选择.本能采用;
            开始.开始时间 = {新状态时间语义::实例绝对UTC, 3};
            const auto 执行 = 任务服务.任务.开始方法执行(开始);
            要求(执行.状态 == 新任务操作状态::任务状态不允许 && !执行.执行实例节点
                && 任务服务.任务.获取任务(材料.任务节点) == 任务前,
                "not-ready exact-domain task produced execution");
            const auto 确认 = 任务服务.任务.确认任务进入待执行(材料.任务节点,
                *选择.本能采用);
            const auto 就绪 = 任务服务.任务.获取任务(材料.任务节点);
            要求(确认 == 新任务治理待执行确认状态::已确认 && 就绪
                && 就绪->当前状态 == 新任务状态::待执行 && !就绪->最近执行实例节点
                && 服务.场景.获取场景(材料.场景节点) == 场景前,
                "adoption readiness executed an action or changed the scene");
        });
        检查("task forbidden exact domain survives prediction and readiness", [&] {
            const auto 材料 = 任务服务.建样例({0, 10}, {50, 60}, false, 整数{0},
                二次特征结果域种类::包含于参照值域, true);
            const auto 选择 = 任务服务.任务.选择任务方法结果(材料.任务节点);
            要求(选择.状态 == 新任务方法选择状态::已选定 && 选择.方法节点
                && 选择.配对节点 && 选择.结果节点 && 选择.本能采用, "forbidden-domain readonly selection");
            const auto 前 = 任务服务.任务.获取任务(材料.任务节点);
            const auto 核验 = 任务服务.任务.核验任务执行方向(材料.任务节点,
                *选择.本能采用,
                任务服务.自我, 任务服务.安全根, 任务服务.服务根);
            要求(核验.状态 == 新任务治理执行方向核验状态::已核验可执行
                && 核验.结果获取方案 && 核验.结果获取方案->禁止结果组.size() == 1
                && 核验.结果获取方案->禁止结果组.front().禁止精确数值域
                    == std::optional<区间>{{90, 95}}
                && 任务服务.任务.获取任务(材料.任务节点) == 前,
                "forbidden exact domain lost downstream protection");
            const auto 确认 = 任务服务.任务.确认任务进入待执行(材料.任务节点,
                *选择.本能采用);
            const auto 后 = 任务服务.任务.获取任务(材料.任务节点);
            要求(确认 == 新任务治理待执行确认状态::已确认 && 后
                && 后->当前状态 == 新任务状态::待执行 && !后->最近执行实例节点,
                "ready confirmation executed forbidden-domain method");
        });
        // 仅测试公开纯值predicate；这些DTO从不写入基础数据集，不是正式A。
        const auto 完成材料 = [] {
            新任务影响评估信息 材料;
            材料.目标需求结果.状态 = 新需求当前满足状态::已满足;
            材料.多渠道验证 = 新任务影响验证状态::已验证通过;
            材料.禁止结果验证 = 新任务影响验证状态::已验证通过;
            材料.副作用验证 = 新任务影响验证状态::已验证通过;
            材料.归因验证 = 新任务影响验证状态::已验证通过;
            return 材料;
        };
        const 新任务影响验证状态 验证状态组[]{新任务影响验证状态::未验证,
            新任务影响验证状态::已验证通过, 新任务影响验证状态::已验证不通过,
            新任务影响验证状态::不适用, 新任务影响验证状态::冲突};
        检查("completion predicate ignores prediction direction magnitude domain matrix", [&] {
            for (const auto 方向 : 验证状态组)
                for (const auto 幅度 : 验证状态组)
                    for (const auto 值域 : 验证状态组) {
                        auto 材料 = 完成材料();
                        材料.方向验证 = 方向;
                        材料.幅度验证 = 幅度;
                        材料.值域验证 = 值域;
                        const auto 前 = 材料;
                        要求(材料.允许形成完成决议() && 评估材料相同(材料, 前),
                            "prediction score directly blocked satisfied demand");
                    }
        });
        检查("completion predicate still rejects unmet or unknown demand", [&] {
            for (const auto 状态 : {新需求当前满足状态::需增加, 新需求当前满足状态::需减少,
                    新需求当前满足状态::未满足但无有向差值, 新需求当前满足状态::目标特征尚未形成,
                    新需求当前满足状态::当前值不存在, 新需求当前满足状态::比较关系不适用,
                    新需求当前满足状态::双根负差非法, 新需求当前满足状态::入口拒绝,
                    新需求当前满足状态::需求不存在, 新需求当前满足状态::结构不一致,
                    新需求当前满足状态::资源失败, 新需求当前满足状态::需要执行前后比较}) {
                auto 材料 = 完成材料();
                材料.目标需求结果.状态 = 状态;
                要求(!材料.允许形成完成决议(), "unsatisfied/unknown demand completed");
            }
        });
        检查("completion predicate preserves observation prohibition attribution and side-effect gates", [&] {
            const auto 字段组 = {&新任务影响评估信息::多渠道验证,
                &新任务影响评估信息::禁止结果验证,
                &新任务影响评估信息::副作用验证,
                &新任务影响评估信息::归因验证};
            for (const auto 字段 : 字段组) {
                for (const auto 状态 : 验证状态组) {
                    auto 材料 = 完成材料();
                    材料.*字段 = 状态;
                    const bool 可以不适用 = 字段 == &新任务影响评估信息::多渠道验证
                        || 字段 == &新任务影响评估信息::禁止结果验证;
                    const bool 允许 = 状态 == 新任务影响验证状态::已验证通过
                        || (可以不适用 && 状态 == 新任务影响验证状态::不适用);
                    要求(材料.允许形成完成决议() == 允许,
                        "non-prediction safety gate was removed");
                }
            }
            auto 缺项 = 完成材料();
            缺项.预计但未发生项组 = {服务.数值类型};
            要求(!缺项.允许形成完成决议(), "missing expected item gate removed");
            auto 计划外 = 完成材料();
            计划外.计划外状态组 = {服务.数值类型};
            要求(!计划外.允许形成完成决议(), "unplanned state gate removed");
        });
        检查("owner evaluation keeps satisfied value demand despite wrong predicted domain", [&] {
            const auto 材料 = 任务服务.建样例({0, 10}, {70, 80}, false);
            const auto 评估 = 任务服务.形成观察评估(材料, 55);
            要求(评估.目标需求结果.状态 == 新需求当前满足状态::已满足
                && 评估.值域验证 == 新任务影响验证状态::已验证不通过
                && 评估.幅度验证 == 新任务影响验证状态::已验证不通过,
                "observed demand was replaced by predicted-domain conformance");
            要求(评估.归因验证 == 新任务影响验证状态::未验证
                && !评估.允许形成完成决议(), "unattributed observed change completed task");
        });
        检查("owner evaluation compares demand direction independently from predicted direction", [&] {
            const auto 材料 = 任务服务.建样例({0, 10}, {50, 60}, false, 整数{0},
                二次特征结果域种类::包含于参照值域, false, 新需求变化方向::增加);
            const auto 评估 = 任务服务.形成观察评估(材料, 55, true);
            要求(评估.目标需求结果.状态 == 新需求当前满足状态::已满足
                && 评估.方向验证 == 新任务影响验证状态::已验证不通过,
                "demand increase incorrectly used predicted decrease verdict");
            要求(评估.归因验证 == 新任务影响验证状态::未验证
                && !评估.允许形成完成决议(), "unattributed direction change completed task");
        });
        检查("owner direction demand signed observation matrix", [&] {
            for (const auto 方向 : {新需求变化方向::增加, 新需求变化方向::减少}) {
                for (const auto 后值 : {整数{40}, 整数{60}}) {
                    const auto 材料 = 任务服务.建样例({0, 90},
                        方向 == 新需求变化方向::增加 ? 区间{70, 80} : 区间{20, 30}, false, 整数{50},
                        二次特征结果域种类::包含于参照值域, false, 方向);
                    const auto 评估 = 任务服务.形成观察评估(材料, 后值);
                    const bool 符合 = (方向 == 新需求变化方向::增加) == (后值 > 50);
                    const auto 预期 = 符合 ? 新需求当前满足状态::已满足
                        : (方向 == 新需求变化方向::增加
                            ? 新需求当前满足状态::需增加 : 新需求当前满足状态::需减少);
                    要求(评估.目标需求结果.状态 == 预期,
                        "signed actual observation disagrees with demand direction");
                }
            }
        });
        检查("owner direction demand zero net difference is not satisfied", [&] {
            for (const auto 方向 : {新需求变化方向::增加, 新需求变化方向::减少}) {
                const auto 材料 = 任务服务.建样例({0, 90},
                    方向 == 新需求变化方向::增加 ? 区间{70, 80} : 区间{20, 30}, false, 整数{50},
                    二次特征结果域种类::包含于参照值域, false, 方向);
                // 同值写入不造状态；经两次真实变化回到前值，最新观察净差为0。
                const auto 结果 = 任务服务.形成观察评估结果(材料, {{55, 11}, {50, 12}});
                要求(结果.评估 && 结果.评估->目标需求结果.状态
                    == (方向 == 新需求变化方向::增加
                        ? 新需求当前满足状态::需增加 : 新需求当前满足状态::需减少),
                    "zero net change fulfilled a strict direction demand");
            }
        });
        检查("owner direction demand without observation remains undecided", [&] {
            const auto 材料 = 任务服务.建样例({0, 90}, {70, 80}, false, 整数{50},
                二次特征结果域种类::包含于参照值域, false, 新需求变化方向::增加);
            const auto 结果 = 任务服务.形成观察评估结果(材料, {});
            要求(结果.评估 && 结果.评估->目标需求结果.状态
                    == 新需求当前满足状态::需要执行前后比较
                && !结果.评估->允许形成完成决议(),
                "missing observation was converted to successful direction");
        });
        检查("owner direction demand uses latest observation instead of first prediction match", [&] {
            const auto 材料 = 任务服务.建样例({0, 90}, {70, 80}, false, 整数{50},
                二次特征结果域种类::包含于参照值域, false, 新需求变化方向::增加);
            const auto 结果 = 任务服务.形成观察评估结果(材料, {{55, 11}, {45, 12}});
            要求(结果.评估 && 结果.评估->目标需求结果.状态 == 新需求当前满足状态::需增加,
                "first increasing state masked latest decreasing observation");
        });
        检查("owner direction demand rejects conflicting latest-time observations", [&] {
            const auto 材料 = 任务服务.建样例({0, 90}, {70, 80}, false, 整数{50},
                二次特征结果域种类::包含于参照值域, false, 新需求变化方向::增加);
            const auto 结果 = 任务服务.形成观察评估结果(材料, {{55, 11}, {45, 11}});
            要求(结果.状态 == 新任务操作状态::结构不一致 && !结果.评估,
                "conflicting latest observations selected an arbitrary demand verdict");
        });
    } catch (const std::exception& 异常) {
        ++失败;
        std::cout << "FAIL initialization/driver: " << 异常.what() << '\n';
    } catch (...) {
        ++失败;
        std::cout << "FAIL initialization/driver: unknown exception\n";
    }
    std::cout << "NOT_RUN task-level malformed type/unit injection (no public construction path; feature provider rejection tests retained)\n"
        << "NOT_RUN self child-demand publication/actual instinct invocation (owner adoption and observation evaluation are not real actions)\n"
        << "NOT_RUN self message decisions/full attributed business completion (predicate and concept-domain owner evaluation only)\n";
    std::cout << "SUMMARY " << 通过 << " PASS / " << 失败 << " FAIL\n";
    return 失败 == 0 ? 0 : 1;
}
} // namespace 海中鱼巣
