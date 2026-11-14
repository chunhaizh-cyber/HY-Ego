#pragma once
#include <memory>
#include <functional>
#include <string>
#include "新_方法参数场景.h"
#include "方法问题反馈.数据.h"
#include "本能交互.运行接口.h"
#include "本能需求.运行接口.h"
#include "本能字段询问.运行接口.h"
#include "数据服务.词典材料类.h"

namespace 海中鱼巣 {
extern 稳定编码 模拟世界树根节点;
enum class 新模拟特征初值方式 : std::uint8_t { 常规输入=0, 复制准确=1, 复制未知=2, 概念构造未知=3 };
struct 新模拟执行主体输入 final {
    稳定编码 来源存在, 专属概念;
    std::vector<基础联合节点快照> 来源身份保护;
    std::uint32_t 同源存在输入=0;
};
enum class 新模拟需求执行预检状态 : std::uint8_t { 已就绪=1, 规格未支持, 依赖未就绪, 前提已变化 };
struct 新模拟需求执行预检结果 final {
    新模拟需求执行预检状态 状态=新模拟需求执行预检状态::依赖未就绪;
    std::string 诊断;
};
struct 新模拟内容读取记录 final {
    std::uint8_t 类别=0;
    稳定编码 节点;
    std::vector<std::uint32_t> 参数位置组;
    std::variant<新模拟需求信息,服务要求理解信息,服务要求确认信息,
        新部分服务要求材料,新模拟语言场景信息,新特征信息> 投影;
    bool 动作前=true;
};
enum class 新模拟世界状态 : std::uint8_t { 已完成 = 1, 入口拒绝, 结构不一致, 资源失败, 定义前提已变化, 能力未支持 = 6, 需重新组织输入 = 7 };
struct 新模拟世界建立结果 final {
    新模拟世界状态 状态 = 新模拟世界状态::入口拒绝;
    std::optional<稳定编码> 场景节点;
};
struct 新模拟有序参数输入 final {
    新方法有序参数规格 规格;
    std::uint32_t 所属存在参数=0;
    std::optional<新特征准确值> 值;
    std::optional<新特征值材料> 材料;
    std::uint32_t 同一参数=0;
    std::vector<std::uint32_t> 集合成员参数组;
    std::uint32_t 引用存在参数=0;
    // 仅复制来源映射元数据，绝不作为模拟图中的可写引用发布。
    std::optional<稳定编码> 来源存在;
    bool 复制上下文=false;
    std::vector<稳定编码> 来源任务成员组;
    // 调用期构造收窄，不改变本能首次定型的参数规格。
    std::optional<稳定编码> 构造存在概念;
    std::vector<std::uint32_t> 历史引用存在参数组;
    std::uint32_t 引用需求输入=0;
    新模拟特征初值方式 初值方式=新模拟特征初值方式::常规输入;
    std::optional<新模拟完整特征复制输入> 完整复制特征;
    std::uint32_t 所属语言场景输入=0, 引用语言场景输入=0;
    // 参数本体为本次图中的D，不是F.current引用。
    std::uint32_t 需求参数输入=0;
    // 正式W的不可变键及来源映射；发布localW时不写入来源W引用。
    std::optional<新词条信息> 复制词条;
};
struct 新模拟词典材料复制输入 final {
    新词典材料信息 材料;
    // 可以引用六参数之后的只读来源载体，不增加方法参数。
    std::uint32_t 来源存在输入=0;
};
struct 新模拟场景事实输入 final {
    新方法初始场景事实限定 限定;
    新消息连接输入构成 构成;
};
struct 新模拟任务治理输入 final {
    新任务虚拟存在特征组 治理特征;
};
struct 新模拟输入事件 final {
    std::uint32_t 目标参数=0;
    std::int64_t 值=0;
};
struct 新模拟消息载荷输入 final {
    std::uint32_t 消息序号=0;
    std::string UTF8正文;
};
struct 新模拟消息方案输入 final {
    std::uint32_t 方案序号=0;
    std::vector<新模拟消息载荷输入> 消息组;
};

struct 新模拟任务成员读取 final {
    std::uint32_t 集合参数序号 = 0;
    新任务虚拟存在特征组 动作前;
};

struct 新模拟参数动作请求 final {
    std::uint32_t 主体位置=0, 目标存在位置=0, 目标特征位置=0;
};
struct 新模拟队列提交请求 final {
    std::uint32_t 窗口位置=0, 对端位置=0, 输出特征位置=0;
    稳定编码 动作{};
    std::string UTF8正文;
};

class 动作方法定义只读提供者;
class 概念_动态类;
class 新_模拟世界类 final : public 新方法模拟运行接口,
    public 新本能交互运行接口, public 新方法隔离执行接口,
    public 新本能需求运行接口, public 正式信息参数读取接口,
    public 新本能字段询问运行接口, public 新词典材料场景读取接口 {
public:
    explicit 新_模拟世界类(新_方法类&) noexcept;
    新_模拟世界类(新_方法类&,const 动作方法定义只读提供者&,新_动态类&,const 概念_动态类&) noexcept;
    新_模拟世界类(新_方法类&,const 动作方法定义只读提供者&,新_动态类&,const 概念_动态类&,
        const 新方法场景事实构成读取接口&) noexcept;
    ~新_模拟世界类();
    void 配置二次特征计算(const 新_二次特征类&);
    void 配置词典材料(词典材料类&);
    新词典材料场景读取结果 读取材料所属场景(稳定编码) const noexcept override;
    新词典材料读取结果 读取本地词典材料(稳定编码) const noexcept;
    void 配置需求执行读取(const 新_存在类&,const 概念_存在类&,
        const 概念_特征类&,const 概念_二次特征类&);
    void 设置字段询问语言(std::shared_ptr<const 新字段询问语言接口>,
        std::optional<新字段询问登记配置>);
    新本能字段询问运行接口* 获取字段询问运行接口() noexcept override;
    新字段询问材料读回 读取询问材料(const 方法参数场景&) const override;
    新字段询问文字结果 生成询问文字(const 新正式字段询问目标&) const override;
    稳定编码 建立输出动作(const 方法参数场景&,const 新字段询问材料&) override;
    新出站表达留档状态 留档出站具体表达(const 方法参数场景&,
        const 新字段询问材料&,稳定编码,const 新字段询问文字结果&) override;
    新字段询问入列读回 提交询问输出(const 方法参数场景&,
        const 新字段询问材料&,稳定编码,std::string_view) override;
    稳定编码 建立参数动作(const 方法参数场景&,const 新模拟参数动作请求&);
    新隔离提交结果 提交本地输出队列(const 方法参数场景&,const 新模拟队列提交请求&);
    新_模拟世界类(const 新_模拟世界类&) = delete;
    新_模拟世界类& operator=(const 新_模拟世界类&) = delete;
    新_模拟世界类(新_模拟世界类&&) = delete;
    新_模拟世界类& operator=(新_模拟世界类&&) = delete;

    新模拟世界建立结果 建立有序样例(稳定编码 M,
        const std::vector<新模拟有序参数输入>&,
        const std::vector<新模拟输入事件>&,
        const std::vector<新模拟任务治理输入>& 任务组 = {},
        const std::vector<新模拟场景事实输入>& 场景输入 = {},
        std::optional<稳定编码> 执行分支 = {},
        const std::optional<新模拟消息方案输入>& 消息输入 = {},
        const 新模拟需求输入图& 需求图 = {},
        const std::optional<新模拟执行主体输入>& 执行主体输入 = {},
        const std::vector<新模拟词典材料复制输入>& 词典材料输入 = {});
    新模拟需求执行预检结果 预检需求执行(const 方法参数场景&) const;
    新本能需求运行接口* 获取需求运行接口() noexcept override;
    新本能需求材料读回 读取新需求材料(const 方法参数场景&) const override;
    新本能需求材料状态 复核新需求材料(const 方法参数场景&,const 新本能需求材料&) const override;
    稳定编码 开始本次需求动作(const 方法参数场景&) override;
    新本能需求发布读回 发布新需求(const 方法参数场景&,const 新本能需求材料&,稳定编码) override;
    新本能已发布需求核验结果 核验本次需求执行结果(
        const 方法参数场景&,稳定编码,const 新本能已发布需求结果&) const noexcept;
    新模拟需求读回 读取模拟需求(稳定编码) const override;
    正式信息参数读取结果 读取正式信息参数保护(稳定编码,稳定编码) const noexcept override;
    新模拟需求关联读回<服务要求理解信息> 读取模拟理解(稳定编码) const override;
    新模拟需求关联读回<服务要求确认信息> 读取模拟确认(稳定编码) const override;
    新模拟需求关联读回<新部分服务要求材料> 读取模拟要求(稳定编码) const override;
    新模拟需求关联读回<新模拟语言场景信息> 读取模拟语言场景(稳定编码) const override;
    std::optional<稳定编码> 读取模拟需求来源(稳定编码) const;
    std::vector<新模拟内容读取记录> 读取本次内容读取组() const;
    std::vector<稳定编码> 读取本地实例身份组() const;
    std::optional<新外设消息信息> 读取模拟消息(稳定编码 消息节点) const;
    std::vector<新外设消息信息> 读取模拟消息组(稳定编码 消息特征节点) const;
    新本能交互运行接口* 获取交互运行接口() noexcept override;
    新方法隔离执行接口* 获取隔离执行接口() noexcept override;
    新隔离调用身份 读取当前调用身份() const override;
    稳定编码 建立阶段动作(const 方法参数场景&,const 新隔离阶段动作请求&) override;
    新隔离提交结果 提交输出(const 方法参数场景&,const 新隔离输出请求&) override;
    新隔离推进状态 推进输出() override;
    std::uint64_t 读取进展序号() const override;
    bool 请求停止已成立() override;
    std::optional<新隔离调用证据> 读取已关闭调用证据() const override;
    std::optional<新状态信息> 读取本地状态(稳定编码) const override;
    std::optional<新状态信息> 读取最新特征状态(稳定编码,稳定编码,稳定编码) const override;
    新二次特征计算结果 比较接收数量状态(const 新状态信息&,const 新状态信息&) const override;
    std::optional<新状态信息> 读取固定消息状态(稳定编码,稳定编码) const override;
    std::optional<新动态信息> 读取本地动作(稳定编码) const override;
    std::optional<新动态信息> 读取本地变化(稳定编码) const override;
    std::optional<新外设消息信息> 按提交读取发送消息(std::uint64_t) const override;
    bool 提交已完整写出(std::uint64_t) const override;
    std::optional<新外设消息信息> 读取消息(稳定编码 n) const override { return 读取模拟消息(n); }
    std::vector<新外设消息信息> 读取消息组(稳定编码 n) const override { return 读取模拟消息组(n); }
    新场景事实构成读取结果 读取连接构成() const override;
    新交互写入读回 写入本次连接交互者(const 方法参数场景&,const 新隔离连接完成材料&) override;
    新方法词条参数读取结果 读取模拟词条挂接(稳定编码) const noexcept override;
    新方法参数特征读取结果 读取模拟存在参数特征(std::uint32_t,稳定编码) const noexcept override;
    std::vector<新本能函数条件角色材料> 读取本次存在参数特征读取组() const;
    std::optional<新方法初始必要条件定义> 读取本次必要条件(const 方法参数场景&) const override;
    新交互求值状态 读取绑定环境(const 方法参数场景&,新交互绑定&) override;
    新存在概念匹配状态 核验绑定存在概念(稳定编码,稳定编码) override;
    std::optional<新特征信息> 读取交互者(稳定编码) override;
    新交互求值状态 读取场景事实(const 新方法初始场景事实限定&,const std::vector<新方法参数角色绑定>&) const override;
    新交互求值状态 读取本次结果场景事实(const 新方法本次场景事实引用&,
        const std::vector<新方法参数角色绑定>&, 稳定编码, 稳定编码,
        新交互连接过程& 输出) const override;
    稳定编码 建立本次动作(const 方法参数场景&,const 新交互绑定&) override;
    新交互写入读回 写入交互者(const 新交互绑定&,稳定编码) override;
    bool 已请求停止() const noexcept override;
    std::vector<新方法初始场景事实限定> 读取本次场景事实组() const;
    void 释放有序样例();
    // false仅表示真实闭包外引用仍在；不清除样例和退出责任。
    bool 尝试释放有序样例();
    void 停止样例后继发布() noexcept;
    std::optional<稳定编码> 读取模拟存在来源(稳定编码 本地存在) const;
    std::optional<稳定编码> 读取模拟任务来源(稳定编码 本地任务) const;
    std::optional<新任务虚拟存在引用> 查找任务虚拟存在引用(稳定编码) const noexcept override;
    std::optional<新任务虚拟存在特征组> 读取任务虚拟存在特征组(const 新任务虚拟存在引用&) const noexcept override;
    void 开始记录复制上下文读取(const std::function<bool()>& 停止检查 = {});
    bool 结束记录复制上下文读取();
    // 只返回已关闭调用窗口中最后由方法实际提交的F身份；结果值仍须正式读回。
    std::vector<稳定编码> 读取本次写回特征组() const;
    std::optional<新任务虚拟存在特征组> 读取模拟集合任务治理(std::uint32_t, 稳定编码) const noexcept override;
    std::vector<新模拟任务成员读取> 读取任务成员记录() const;
    std::optional<std::vector<稳定编码>> 读取模拟存在特征组(稳定编码) const noexcept override;
    std::optional<稳定编码> 读取模拟所属场景(稳定编码) const noexcept override;
    稳定编码 建立模拟动作() override;
    新模拟操作状态 模拟递增计数(稳定编码,稳定编码) override;
    新模拟操作状态 推进模拟输入() override;
    新模拟操作状态 模拟集合加入成员(稳定编码,稳定编码,稳定编码) override;

    std::optional<std::vector<新方法参数角色绑定>> 读取参数绑定() const noexcept;
    // 读取第位置个有序输入（1起，含N之后私有追加输入）的本地节点；样例未建立时为空。
    std::optional<稳定编码> 读取有序输入节点(std::uint32_t 位置) const noexcept;
    // 不可变的真实执行材料。来源编码按值保存，不对现实节点取得引用。
    稳定编码 模拟根节点() const noexcept override;
    bool 核验模拟参数(稳定编码, const std::vector<新方法参数角色绑定>&) const noexcept override;
    std::optional<新特征信息> 读取模拟特征(稳定编码) const noexcept override;
    std::optional<新特征值信息> 读取模拟特征值材料(稳定编码) const noexcept override;
    std::optional<新任务虚拟存在读回> 读取任务虚拟存在引用(const 新任务虚拟存在引用&) const noexcept override;
private:
    bool 复制词典材料初值(const std::vector<新模拟有序参数输入>&,
        const std::vector<新模拟词典材料复制输入>&,const std::vector<std::size_t>&,
        const std::vector<稳定编码>&,稳定编码);
    稳定编码 建立本地参数动作(const 方法参数场景&,
        const 新隔离阶段动作请求&,std::uint32_t 主体位置);
    新本能需求材料读回 读取本地需求材料(const 方法参数场景&) const;
    新本能已发布需求核验结果 核验本地需求结果(
        const 方法参数场景&,稳定编码,const 新本能已发布需求结果&,bool 要求关闭) const;
    void 发布模拟数值变化(稳定编码 特征,std::int64_t 新值,bool 环境输入);
    新交互求值状态 读取隔离连接过程(稳定编码 调用,稳定编码 主动作,
        const 新交互绑定&,const 新消息连接输入构成&,新交互连接过程&) const;
    void 发布模拟消息(const 新方法模拟消息输入定义&,const std::string&,std::int64_t 触发时间);
    新隔离提交结果 发布本地消息(稳定编码 F,稳定编码 W,稳定编码 对端,
        新外设消息方向,const std::string&,std::int64_t,std::optional<std::uint64_t>,
        std::optional<稳定编码> 动作, bool 方法写入,
        const std::function<void(基础节点组追加请求&,稳定编码,std::size_t,std::size_t,std::uint64_t)>& 附加 = {});
    新交互写入读回 发布本地队列(稳定编码 F,const std::vector<std::uint64_t>&,
        std::optional<稳定编码> 动作,bool 方法写入,
        const std::function<void(基础节点组追加请求&,std::size_t,std::size_t)>& 附加 = {});
    std::optional<新特征信息> 读取模拟特征内部(稳定编码,bool 正式定位) const noexcept;
    struct 实现;
    std::unique_ptr<实现> 实现_;
};
} // namespace 海中鱼巣
