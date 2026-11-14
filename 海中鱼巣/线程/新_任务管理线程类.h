#pragma once

#include <cstdint>
#include <functional>
#include <future>
#include <memory>
#include <optional>
#include <stop_token>
#include <variant>
#include <vector>
#include "../领域/新_任务类.h"
#include "新_任务工作线程类.h"
#include "../领域/服务值结算.本能合同.h"
#include "../业务/模拟学习.服务.h"

namespace 海中鱼巣 {
struct 新前置语言筹办接收结果 final {
    新任务工作接收状态 状态=新任务工作接收状态::入口拒绝;
    std::optional<新任务工作请求> 工作;
    std::shared_future<新任务工作处理结果> 处理结果;
};

class 新_世界树类;

struct 新任务管理运行配置 final {
    std::uint64_t 邮箱容量 = 0;
    std::uint64_t 资源重试间隔毫秒 = 0;
    std::uint64_t 调度复核间隔毫秒 = 1000;
    friend bool operator==(const 新任务管理运行配置&,
        const 新任务管理运行配置&) = default;
};

enum class 新任务管理生命周期 : std::uint8_t {
    未启动, 启动中, 等待消息, 处理消息, 停止中, 已停止, 启动失败, 内部错误
};

enum class 新任务管理操作状态 : std::uint8_t {
    已完成 = 1, 无变化, 入口拒绝, 已在运行, 尚未启动,
    等待超时, 不能等待自身, 服务未就绪, 所有者冲突, 资源失败, 内部错误
};

struct 新任务管理操作结果 final {
    新任务管理操作状态 状态 = 新任务管理操作状态::入口拒绝;
    新任务管理生命周期 生命周期 = 新任务管理生命周期::未启动;
};

enum class 新任务承接依据 : std::uint8_t {
    显式请求 = 1,
    自我首次复核 = 2,
    自我状态变化复核 = 3,
    自我全树扫描 = 4,
    自我条件子需求 = 5,
    外部服务要求 = 7,
    行动请求 = 8
};


// 唯一业务用途是承接需求；由自我决议的调用链提供具体需求。
struct 新任务承接消息 final {
    std::uint64_t 消息编号 = 0;
    稳定编码 需求节点;
    新任务承接依据 依据 = 新任务承接依据::显式请求;
    // 只有“自我状态变化复核”携带正式状态节点；它同时作为该根的
    // 单调决议身份，防止旧状态通知在任务终结后重新启用任务。
    std::optional<稳定编码> 来源状态节点;
    std::optional<稳定编码> 外部要求节点;
    std::optional<新行动请求承接来源> 行动请求来源{};
    friend bool operator==(const 新任务承接消息&, const 新任务承接消息&) = default;
};

enum class 新任务承接处理状态 : std::uint8_t {
    已承接 = 1, 已重新启用, 既有终结, 需求当前已满足,
    入口拒绝, 服务未就绪, 需核查, 停止取消, 内部错误
};

struct 新任务承接处理结果 final {
    新任务承接消息 原消息;
    新任务承接处理状态 状态 = 新任务承接处理状态::内部错误;
    std::optional<新任务治理承接结果> 承接;
};

enum class 新任务承接接收状态 : std::uint8_t {
    已接收 = 1, 精确重复, 过期决议, 身份冲突, 邮箱已满, 尚未启动,
    已停止接收, 入口拒绝, 资源失败, 内部错误
};

struct 新任务承接接收结果 final {
    新任务承接接收状态 状态 = 新任务承接接收状态::入口拒绝;
    // 接纳不等于已建任务。结果交付后容量释放，由调用者持有共享结果。
    std::shared_future<新任务承接处理结果> 处理结果;
};

struct 新任务管理快照 final {
    新任务管理生命周期 生命周期 = 新任务管理生命周期::未启动;
    std::uint64_t 活动消息数 = 0;
    std::uint64_t 活动工作数 = 0;
    std::uint64_t 待反馈任务数 = 0;
    std::uint64_t 自我筹办裁决数 = 0;
    std::uint64_t 执行结果数 = 0;
    bool 停止已请求 = false;
    bool 线程已完成 = false;
    std::optional<稳定编码> 服务值结算需求;
    std::optional<稳定编码> 服务值结算任务;
    bool 服务值结算方法已配置 = false;
    bool 服务值结算治理已配置 = false;
    std::uint64_t 等待复核任务数 = 0;
    bool 调度扫描在途 = false;
    std::uint64_t 就绪工作数 = 0;
};

// self初始化后、manager启动前锁定。它只描述长期内部治理任务使用的
// 正式世界身份，不建立第二份服务值或任务事实。
struct 新长期服务值结算配置 final {
    新_世界树类* 世界树服务 = nullptr;
    稳定编码 自我所在场景;
    稳定编码 自我存在;
    稳定编码 服务值特征;
    稳定编码 完成服务需求;
    稳定编码 服务根需求;
    稳定编码 自我概念节点, 服务值特征类型节点;
    新结算能力引用 能力;
    新服务值结算确认接口* 工程诊断端口 = nullptr;

    bool 完整() const noexcept;
    friend bool operator==(const 新长期服务值结算配置&,
        const 新长期服务值结算配置&) = default;
};

enum class 新任务筹办处理状态 : std::uint8_t {
    有候选并进入条件检查 = 1,
    完整无候选并进入等待,
    条件筹办已完成,
    任务状态已变化,
    工作未完成,
    独立读回失败,
    状态推进失败,
    停止取消,
    内部错误,
    组织能力暂不支持, 复合目标需自我确认, 内容结果定位未支持, 未执行目标已满足
};

struct 新任务筹办处理结果 final {
    新任务工作请求 原工作;
    新任务筹办处理状态 状态 = 新任务筹办处理状态::内部错误;
    std::optional<新任务工作处理结果> 工作结果;
    std::optional<新任务治理候选消费结果> 独立消费;
    std::optional<新任务治理条件筹办结果> 条件筹办;
    std::optional<新任务治理条件筹办结果> 独立条件筹办;
    std::optional<新任务条件子需求提议> 独立子需求提议;
    std::optional<新任务因果子需求提议> 因果子需求提议;
    std::optional<新任务复合目标分解提议> 复合目标提议;
    std::optional<新任务执行前目标收束结果> 执行前目标收束;
};

// manager只向self交付当前筹办结果的稳定定位；正式筹办内容必须由
// self按任务节点重新读取，并核对完整原工作后才能用于治理裁决。
struct 新任务筹办反馈定位 final {
    稳定编码 任务节点;
    新任务工作请求 原工作;
    bool 完整() const noexcept;
    friend bool operator==(const 新任务筹办反馈定位&,
        const 新任务筹办反馈定位&) = default;
};

enum class 新任务筹办反馈接收状态 : std::uint8_t {
    已接收 = 1, 精确重复, 身份冲突, 邮箱已满, 尚未配置,
    尚未启动, 已停止接收, 入口拒绝, 资源失败, 内部错误
};

struct 新任务等待复核定位 final {
    std::uint64_t 复核编号 = 0;
    新任务调度定位 定位;
    新任务等待原因 原因 = 新任务等待原因::需核查;
    bool 完整() const noexcept;
    friend bool operator==(const 新任务等待复核定位&,
        const 新任务等待复核定位&) = default;
};
enum class 新自我等待复核状态 : std::uint8_t {
    允许重新筹办 = 1, 当前已满足, 需求非活动, 等待材料,
    等待同次执行比较, 定位已变化, 需核查
};
struct 新自我等待复核决议 final {
    新任务等待复核定位 定位;
    新自我等待复核状态 状态 = 新自我等待复核状态::需核查;
    std::optional<新需求当前满足状态> 当前满足状态;
    std::optional<新需求筹办资格结果> 筹办资格;
    friend bool operator==(const 新自我等待复核决议&,
        const 新自我等待复核决议&) = default;
};
enum class 新任务等待复核反馈接收状态 : std::uint8_t {
    已接收 = 1, 精确重复, 身份冲突, 邮箱已满, 尚未配置,
    尚未启动, 已停止接收, 入口拒绝, 资源失败, 内部错误
};
enum class 新自我等待复核接收状态 : std::uint8_t {
    已接收 = 1, 精确重复, 定位已变化, 身份冲突,
    已停止接收, 入口拒绝, 资源失败, 内部错误, 筹办材料已变化 = 9
};

enum class 新任务结果状态接收状态 : std::uint8_t {
    已接收并触发结果获取 = 1,
    已接收等待更多材料,
    当前无等待执行,
    与等待执行无关,
    已停止接收,
    资源失败,
    内部错误,
    已接收待异步检测
};

enum class 新自我筹办裁决状态 : std::uint8_t {
    可进入执行前判断 = 1,
    等待材料,
    当前无可用方法,
    结构缺口,
    任务状态变化,
    需核查,
    已处理条件子需求,
    已处理因果子需求
};

enum class 新自我执行前判断状态 : std::uint8_t {
    允许执行 = 1,
    暂缓执行,
    拒绝执行,
    重新判断,
    缺少执行依赖,
    需核查
};

struct 新自我执行前判断 final {
    稳定编码 任务节点;
    稳定编码 方法节点;
    稳定编码 配对节点;
    稳定编码 结果节点;
    新自我执行前判断状态 状态 = 新自我执行前判断状态::需核查;
    // 只在“允许执行”时填写；manager按原样形成W3工作，worker重新核验。
    std::optional<新任务治理本能执行请求> 本能执行请求;
    friend bool operator==(const 新自我执行前判断&,
        const 新自我执行前判断&) = default;
};

enum class 新任务执行处理状态 : std::uint8_t {
    已完成影响评估 = 1,
    动作已发出等待结果,
    执行依赖未就绪,
    动作未进入,
    任务状态已变化,
    工作未完成,
    独立读回失败,
    停止取消,
    内部错误,
    工程停止, 未执行目标已满足
};

struct 新任务执行处理结果 final {
    新任务工作请求 原工作;
    新任务执行处理状态 状态 = 新任务执行处理状态::内部错误;
    std::optional<新任务工作处理结果> 工作结果;
    std::optional<新任务治理执行读回结果> 独立读回;
};

struct 新任务结果反馈定位 final {
    稳定编码 任务节点;
    新任务工作请求 原工作;
    稳定编码 执行实例节点;
    稳定编码 影响评估节点;
    // 索引提示不是事实；self仍从同一X正式重读已发布需求。
    std::optional<稳定编码> 已发布需求节点;
    bool 完整() const noexcept;
    friend bool operator==(const 新任务结果反馈定位&,
        const 新任务结果反馈定位&) = default;
};

enum class 新任务结果反馈接收状态 : std::uint8_t {
    已接收 = 1, 精确重复, 身份冲突, 邮箱已满, 尚未配置,
    尚未启动, 已停止接收, 入口拒绝, 资源失败, 内部错误
};

enum class 新自我任务后继接收状态 : std::uint8_t {
    已发布并读回 = 1,
    精确重复,
    定位已变化,
    任务状态已变化,
    状态发布失败,
    已停止接收,
    入口拒绝,
    资源失败,
    内部错误
};

enum class 新任务完成结算确认状态 : std::uint8_t {
    已确认 = 1,
    精确重复,
    尚未结算,
    定位已变化,
    已停止接收,
    入口拒绝,
    资源失败,
    内部错误
};

enum class 新子需求采用状态 : std::uint8_t {
    已建立 = 1, 已复用, 已无需要, 来源过期, 拒绝,
    结构错误, 资源失败, 未处理
};

struct 新子需求采用项 final {
    新任务条件子需求项 原提议;
    新子需求采用状态 状态 = 新子需求采用状态::未处理;
    std::optional<稳定编码> 需求节点;
    std::optional<新来源前驱内容模板> 来源前驱模板;
    std::optional<新服务取得目标实例信息> 来源前驱实例;
    std::optional<新需求完整目标> 来源前驱具体目标;
    friend bool operator==(const 新子需求采用项&,
        const 新子需求采用项&) = default;
};

struct 新子需求采用回执 final {
    新任务条件子需求提议 原提议;
    std::vector<新子需求采用项> 项组;
    bool 已完成处理 = false;
    friend bool operator==(const 新子需求采用回执&,
        const 新子需求采用回执&) = default;
};

struct 新自我筹办裁决 final {
    新任务筹办反馈定位 定位;
    新自我筹办裁决状态 状态 = 新自我筹办裁决状态::需核查;
    // 只有W2中恰好一个具备的条件结果配对时才填写；它们只是
    // 后继执行前判断定位，不表示已经选定、批准或执行方法。
    std::optional<稳定编码> 唯一具备方法节点;
    std::optional<稳定编码> 唯一具备配对节点;
    std::optional<稳定编码> 唯一对应结果节点;
    // 只有完成fresh执行方向核验后才填写；允许只表示self治理判断通过，
    // 不表示本能函数已经注册、调用或产生现实结果。
    std::optional<新自我执行前判断> 执行前判断;
    std::optional<新子需求采用回执> 子需求回执;
    std::optional<新任务本能采用材料> 本能采用;
    struct 因果采用回执 final {
        新任务因果子需求提议 原提议;
        std::vector<新子需求采用项> 项组;
        bool 已完成处理 = false;
        friend bool operator==(const 因果采用回执&, const 因果采用回执&) = default;
    };
    std::optional<因果采用回执> 因果回执;
    friend bool operator==(const 新自我筹办裁决&,
        const 新自我筹办裁决&) = default;
};

enum class 新自我筹办裁决接收状态 : std::uint8_t {
    已接收 = 1, 精确重复, 定位已变化, 身份冲突,
    已停止接收, 入口拒绝, 资源失败, 内部错误
};

// R111：自我对子需求做合法性判定后发给任务管理线程的通知。只提示挂起或恢复，
// 不携带结论以外的业务材料；父任务定位沿用筹办反馈定位。被排除候选的身份取自
// 子需求提议本身：条件子需求为提议的方法/配对/执行分支，复合目标为分解提议，
// 因果子需求为采用的因果依据。
enum class 新子需求不合法候选种类 : std::uint8_t {
    条件方法 = 1, 复合分解, 因果路径
};

struct 新子需求不合法候选 final {
    新子需求不合法候选种类 种类 = 新子需求不合法候选种类::条件方法;
    稳定编码 方法节点;
    稳定编码 配对节点;
    std::optional<稳定编码> 执行分支节点;
    std::optional<新任务复合目标分解提议> 复合分解;
    std::optional<新任务因果筹办依据> 因果依据;
    friend bool operator==(const 新子需求不合法候选&,
        const 新子需求不合法候选&) = default;
};

struct 新子需求合法性通知 final {
    新任务筹办反馈定位 父任务定位;
    std::vector<稳定编码> 子需求节点组;
    新子需求不合法候选 候选;
    bool 完整() const noexcept;
    friend bool operator==(const 新子需求合法性通知&,
        const 新子需求合法性通知&) = default;
};

enum class 新子需求合法性通知接收状态 : std::uint8_t {
    已接收 = 1, 精确重复, 任务状态已变化, 已停止接收, 入口拒绝, 资源失败, 内部错误
};

// 工程停止与完整业务结果分账，不把取消解释成空边或拒绝采用。
using 新因果等待边可停止结果 = std::variant<
    std::vector<新因果任务等待边>, 新任务因果读取取消>;
using 新自我因果筹办提交结果 = std::variant<
    新自我筹办裁决接收状态, 新任务因果读取取消>;

// 当前完成M1承接、W1/W2筹办、self执行前裁决回收，并在self允许后
// 派发W3本能执行；结算任务无明确来源时等待条件，具备来源后同样
// 经W1召回、W2核验和self选择进入worker，初始化不预绑函数指针。
struct 新最终服务工作接收结果 final {
    新任务工作接收状态 状态 = 新任务工作接收状态::入口拒绝;
    std::optional<新任务工作请求> 工作;
    std::shared_future<新任务工作处理结果> 处理结果;
};

struct 新贡献治理工作接收结果 final {
    新任务工作接收状态 状态 = 新任务工作接收状态::入口拒绝;
    std::optional<新任务工作请求> 工作;
    std::shared_future<新任务工作处理结果> 处理结果;
};

class 新_任务管理线程类 final {
public:
    新前置语言筹办接收结果 提交前置语言输入筹办(const 新任务前置语言输入请求&) noexcept;
    std::optional<新任务工作处理结果> 读取前置语言输入筹办结果(const 新任务工作请求&) const noexcept;
    新任务工作操作状态 确认前置语言输入筹办已读回(const 新任务工作请求&) noexcept;
    新贡献治理工作接收结果 提交任务贡献治理工作(
        const void* 自我所有者, const 新任务贡献反馈定位&) noexcept;
    新任务完成结算确认状态 确认任务贡献治理工作(
        const void* 自我所有者, std::uint64_t 工作编号,
        const 新任务贡献反馈定位&) noexcept;
    // 任务服务必须比本对象存活更久；绑定本对象的self必须先回收。
    explicit 新_任务管理线程类(新_任务类& 任务服务) noexcept;
    ~新_任务管理线程类() noexcept;
    新_任务管理线程类(const 新_任务管理线程类&) = delete;
    新_任务管理线程类& operator=(const 新_任务管理线程类&) = delete;
    新_任务管理线程类(新_任务管理线程类&&) = delete;
    新_任务管理线程类& operator=(新_任务管理线程类&&) = delete;

    新任务管理操作结果 启动(
        const 新任务管理运行配置& 配置, std::uint64_t 等待毫秒) noexcept;
    新任务承接接收结果 提交消息(const 新任务承接消息& 消息) noexcept;
    新任务管理操作结果 请求停止() noexcept;
    std::stop_token 读取停止令牌() const noexcept;
    新任务管理操作结果 等待停止(std::uint64_t 等待毫秒) noexcept;
    新任务管理快照 读取快照() const noexcept;
    std::vector<新因果任务等待边> 读取因果等待边() const;
    新因果等待边可停止结果 读取因果等待边(std::stop_token) const;
    std::optional<新任务筹办处理结果> 读取最近筹办结果(
        稳定编码 任务节点) const noexcept;
    std::optional<新自我筹办裁决> 读取最近自我筹办裁决(
        稳定编码 任务节点) const noexcept;
    std::optional<新任务执行处理结果> 读取最近执行结果(
        稳定编码 任务节点) const noexcept;
    // TASK-STATE-CAUSAL 切片4：需求owner在需求字段更新后的就绪通知（以需求节点为键），只提示重读；装配端口投递，不经自我线程。
    新任务结果状态接收状态 提交需求就绪通知(
        稳定编码 需求节点) noexcept;
    // 只提示按来源节点重读，不把场景/入口当作需求或结果状态。
    新任务结果状态接收状态 提交词典来源就绪通知(
        const std::vector<稳定编码>& 节点组) noexcept;
    // ASK-READ-SCENE v0.5 §3.2：语言owner在可能回答某问题的新信息保存后的就绪通知（以问题节点为节点键），
    // 只提示重读；装配端口投递，不经自我线程。
    新任务结果状态接收状态 提交问题答案就绪通知(
        稳定编码 问题节点) noexcept;
    // METHOD-LEARN-STATE（5260、8100）：方法owner形成方法状态后的就绪通知（以方法虚拟存在节点为节点键，
    // 并唤醒“方法知识”键），只提示重读；装配端口投递，不经自我线程。
    新任务结果状态接收状态 提交方法状态就绪通知(
        稳定编码 方法虚拟存在节点) noexcept;

private:
    friend class 新_自我线程类;
    using 筹办反馈接收函数 = std::function<新任务筹办反馈接收状态(
        const 新任务筹办反馈定位&)>;
    using 结果反馈接收函数 = std::function<新任务结果反馈接收状态(
        const 新任务结果反馈定位&)>;
    using 等待复核接收函数 = std::function<新任务等待复核反馈接收状态(
        const 新任务等待复核定位&)>;
    using 贡献反馈接收函数 = std::function<新任务结果反馈接收状态(
        const 新任务贡献反馈定位&)>;
    std::optional<新任务等待复核定位> 读取等待复核定位(
        std::uint64_t 复核编号) const noexcept;
    新自我等待复核接收状态 提交等待复核决议(
        const 新自我等待复核决议&) noexcept;
    bool 使用任务服务(const 新_任务类& 任务服务) const noexcept;
    // 仅收存交接消息；已接收待筹办不代表来源已绑定或服务值已经结算。
    // 由管理循环排空旧筹办、独立读回后串行接纳并触发普通筹办。
    新服务值结算协作状态 接纳服务值结算来源(稳定编码 已完成任务节点) noexcept;
    新最终服务工作接收结果 提交最终服务消费工作(
        const void* 自我所有者, const 新服务值最终来源定位&) noexcept;
    新任务工作操作状态 确认最终服务消费工作(
        const void* 自我所有者, std::uint64_t 工作编号,
        const 新服务值最终来源定位&) noexcept;
    新任务完成结算确认状态 确认直接最终服务消费(
        const 新服务值最终来源定位&, const 新服务值最终消费结果&) noexcept;
    新任务管理操作状态 绑定自我筹办反馈(
        const void* 所有者,
        筹办反馈接收函数 筹办接收,
        结果反馈接收函数 结果接收,
        等待复核接收函数 等待接收,
        贡献反馈接收函数 贡献接收,
        std::function<void()> 最终服务工作唤醒,
        const 新长期服务值结算配置& 服务结算配置) noexcept;
    新任务承接接收结果 提交外部服务需求承接(const 新任务承接消息&) noexcept;
    新任务承接接收结果 提交行动请求需求承接(const 新任务承接消息&) noexcept;
    void 解除自我筹办反馈(const void* 所有者) noexcept;
    新自我筹办裁决接收状态 提交自我筹办裁决(
        const 新自我筹办裁决& 裁决) noexcept;
    新自我因果筹办提交结果 提交自我因果筹办裁决(
        const 新自我筹办裁决&, std::stop_token) noexcept;
    新任务承接接收结果 提交已确认条件子需求(
        const 新任务筹办反馈定位& 父筹办定位,
        const 新任务承接消息& 消息) noexcept;
    新任务操作状态 提交已确认复合子需求(const 新任务筹办反馈定位&,
        const 新任务复合采用请求&) noexcept;
    新任务结果状态接收状态 提交结果状态通知(
        稳定编码 状态节点) noexcept;
    // R111：父任务因子需求不合法挂起；仅接纳父任务处于待查找方法、待检查条件、
    // 等待条件时写入“子需求不合法”，否则按任务状态已变化丢弃。重复通知幂等。
    新子需求合法性通知接收状态 提交子需求不合法通知(
        const 新子需求合法性通知&) noexcept;
    // R111：自我改判子需求合法；移除对应排除候选，全部恢复且父任务仍挂起时写回待查找方法。
    新子需求合法性通知接收状态 提交子需求恢复合法通知(
        const 新子需求合法性通知&) noexcept;
    // R111：自我独立重读因果提议时取用与管理线程相同的排除集（只读副本）。
    std::optional<新任务筹办排除集> 读取筹办排除集(稳定编码 任务节点) const noexcept;
    新自我任务后继接收状态 提交自我任务后继决议(
        const 新任务结果反馈定位& 定位,
        const 新自我任务后继决议& 决议) noexcept;
    // self完成正式任务结算后确认工程交接已经收束。manager只在fresh
    // 读回同一结算后释放结果定位；本入口不建立或修改结算事实。
    新任务完成结算确认状态 确认任务完成结算(
        const 新任务结果反馈定位& 定位,
        稳定编码 结算节点) noexcept;
    新任务完成结算确认状态 确认任务贡献复核(
        const 新任务贡献反馈定位&, const 新任务贡献推进结果&) noexcept;
    struct 实现;
    std::unique_ptr<实现> 实现_;
};

} // namespace 海中鱼巣
