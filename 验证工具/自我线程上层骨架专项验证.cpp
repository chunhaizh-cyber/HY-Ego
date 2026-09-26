#include "../海中鱼巣/业务/应用服务.自我根需求复核.h"
#include "../海中鱼巣/线程/线程_自我.h"

#include <atomic>
#include <cstdint>
#include <iostream>
#include <string_view>
#include <thread>
#include <vector>

namespace {

using namespace 海中鱼巣;

std::uint64_t 通过数量 = 0;
std::uint64_t 失败数量 = 0;

void 检查(const bool 条件, const std::string_view 名称) {
    if (条件) {
        ++通过数量;
        std::cout << "PASS " << 名称 << '\n';
    }
    else {
        ++失败数量;
        std::cout << "FAIL " << 名称 << '\n';
    }
}

class 假正式上下文端口 final : public 自我线程正式上下文端口_v2 {
public:
    自我线程正式上下文结果_v2 读取正式上下文(
        const 自我线程正式上下文请求_v2&) noexcept override {
        调用数量_.fetch_add(1, std::memory_order_relaxed);
        return {};
    }
    [[nodiscard]] std::uint64_t 调用数量() const noexcept {
        return 调用数量_.load(std::memory_order_relaxed);
    }
private:
    std::atomic<std::uint64_t> 调用数量_{0};
};

class 假根复核端口 final : public 自我线程根需求复核端口 {
public:
    自我线程根需求复核结果_v2 复核双根当前需求(
        const 自我线程根需求复核请求_v2&) const noexcept override {
        return {};
    }
};

class 假任务核心端口 final : public 本能根任务核心端口_v1 {
public:
    本能根任务初始化包结果_v1 签发或恢复不可变初始化包(
        const 本能根任务初始化语义请求_v1&) noexcept override { return {}; }
    本能根任务初始化包按意图读取结果_v1 按初始化意图读取不可变包(
        const 本能根任务初始化包按意图读取请求_v1&) const noexcept override { return {}; }
    本能根任务承接结果_v1 承接或建立任务(
        const 不可变本能根任务初始化包_v1&) noexcept override { return {}; }
    本能根任务承接结果_v1 恢复任务初始化(
        const 不可变本能根任务初始化包_v1&) noexcept override { return {}; }
    本能根任务核心读取结果_v1 按任务读取核心(
        const 本能根任务身份读取请求_v1&) const noexcept override { return {}; }
    本能根任务核心读取结果_v1 按查询锚点读取当前任务(
        const 本能根任务锚点读取请求_v1&) const noexcept override { return {}; }
    本能根任务目标投影结果_v1 按任务读取目标投影(
        const 本能根任务目标投影读取请求_v1&) const noexcept override { return {}; }
    本能根任务当前资格退出结果_v1 退出任务当前资格(
        const 本能根任务当前资格退出请求_v1&) noexcept override { return {}; }
};

class 假sink final : public 自我线程根治理意图接收端口_v1 {
public:
    bool 已就绪() const noexcept override { return false; }
    根治理意图投递结果_v1 提交(
        const 自我到任务管理本能根承接消息_v1& 消息) noexcept override {
        return {根治理意图投递状态_v1::尚未就绪,消息.消息};
    }
};

struct 假依赖 final {
    假根复核端口 根复核;
    假任务核心端口 任务核心;
    假sink sink;
};

[[nodiscard]] 自我线程创建请求_v1 建立请求(const std::uint64_t 自我身份 = 40) {
    自我线程创建请求_v1 请求;
    请求.邮箱容量 = 2;
    请求.世界 = {{30}, {31}, 100};
    请求.自我 = {{自我身份}, {30}, {31}, 101};
    请求.本能根.自我 = {自我身份};
    请求.本能根.安全根 = {{50}, {51}, {52}, {53}};
    请求.本能根.服务根 = {{60}, {61}, {62}, {63}};
    请求.本能根.Gread = 102;
    return 请求;
}

[[nodiscard]] 自我线程根需求复核消息_v2 建立消息(
    const std::uint64_t 消息身份,
    const 自我线程复核触发根 根,
    const std::uint64_t 正式需求,
    const std::uint64_t 原请求 = 1000) {
    自我线程根需求复核消息_v2 消息;
    消息.消息={消息身份};
    消息.原请求={原请求};
    消息.触发根=根;
    消息.正式需求定位={正式需求};
    if(根==自我线程复核触发根::双根||根==自我线程复核触发根::安全根)
        消息.安全根任务意图={{2001}};
    if(根==自我线程复核触发根::双根||根==自我线程复核触发根::服务根)
        消息.服务根任务意图={{2002}};
    return 消息;
}

自我线程创建结果_v1 创建(自我线程& 线程,
    const 自我线程创建请求_v1& 请求,假正式上下文端口& 上下文,
    假依赖& 依赖,const std::uint64_t 等待=2000) {
    return 线程.创建并停在治理运行门(
        请求,上下文,依赖.根复核,依赖.任务核心,依赖.sink,等待);
}

void 验证基础生命周期与邮箱() {
    假正式上下文端口 端口A,端口B;
    假依赖 依赖;
    自我线程 线程;
    auto 无效请求=建立请求();
    无效请求.邮箱容量=0;
    检查(创建(线程,无效请求,端口B,依赖,100).状态==自我线程操作状态::入口拒绝,
        "invalid request rejected");

    const auto 请求=建立请求();
    const auto 首次=创建(线程,请求,端口A,依赖);
    检查(首次.成功(),"valid create reaches closed gate");
    检查(创建(线程,请求,端口A,依赖).状态==自我线程操作状态::精确重复,
        "same selection is exact duplicate");
    检查(创建(线程,请求,端口B,依赖).状态==自我线程操作状态::选择冲突,
        "different context port conflicts");

    检查(线程.提交根需求复核消息(
        建立消息(70,自我线程复核触发根::安全根,60)).状态==
        自我线程操作状态::入口拒绝,"root locator mismatch rejected");
    const auto 一=建立消息(70,自我线程复核触发根::安全根,50);
    检查(线程.提交根需求复核消息(一).状态==自我线程操作状态::成功,
        "first message queued");
    检查(线程.提交根需求复核消息(一).状态==自我线程操作状态::精确重复,
        "queued duplicate recognized");
    auto 异义=一;异义.原请求={1001};
    检查(线程.提交根需求复核消息(异义).状态==自我线程操作状态::消息冲突,
        "queued identity conflict recognized");
    检查(线程.提交根需求复核消息(
        建立消息(71,自我线程复核触发根::双根,0)).状态==
        自我线程操作状态::成功,"second message queued");
    检查(线程.提交根需求复核消息(
        建立消息(72,自我线程复核触发根::服务根,60)).状态==
        自我线程操作状态::队列已满,"bounded mailbox rejects overflow");

    const auto 快照=线程.读取诊断快照();
    检查(快照.生命周期==自我线程生命周期状态::已停门&&
        快照.邮箱数量==2&&!快照.治理运行门开启&&快照.已冻结批次数量==0,
        "closed gate does not consume mailbox");
    检查(线程.请求停止().成功(),"stop accepted");
    检查(线程.等待停止(2000).成功(),"thread joined");
    检查(端口A.调用数量()==0&&端口B.调用数量()==0,
        "closed-gate lifecycle never calls provider");
}

void 验证并发同选择() {
    假正式上下文端口 端口;
    假依赖 依赖;
    自我线程 线程;
    const auto 请求=建立请求(200);
    std::vector<自我线程创建结果_v1> 结果(8);
    std::vector<std::thread> 调用方;
    for(std::size_t i=0;i<结果.size();++i)
        调用方.emplace_back([&,i]{结果[i]=创建(线程,请求,端口,依赖);});
    for(auto& 调用:调用方)调用.join();
    std::uint64_t 首次=0,重复=0;
    for(const auto& r:结果){
        if(r.状态==自我线程操作状态::成功)++首次;
        if(r.状态==自我线程操作状态::精确重复)++重复;
    }
    检查(首次==1&&重复==7,"concurrent create has one physical winner");
    (void)线程.请求停止();
    检查(线程.等待停止(2000).成功(),"concurrent-create thread joins");
}

void 验证析构停止() {
    假正式上下文端口 端口;
    假依赖 依赖;
    {
        自我线程 线程;
        检查(创建(线程,建立请求(300),端口,依赖).成功(),"destructor case created");
    }
    检查(true,"destructor requests stop and joins");
}

} // namespace

int main() {
    验证基础生命周期与邮箱();
    验证并发同选择();
    验证析构停止();
    std::cout<<"SUMMARY pass="<<通过数量<<" fail="<<失败数量<<'\n';
    return 失败数量==0?0:1;
}
