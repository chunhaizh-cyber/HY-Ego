#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>

import 海中鱼巣.启动.生产运行期;

int main() {
    using namespace 海中鱼巣;

    生产运行期会话 会话;
    const auto 启动结果 = 会话.启动({生产运行期启动请求当前版本});
    const auto* 上下文 = 启动结果.租约.读取();
    if (!启动结果.成功() || 上下文 == nullptr) {
        std::printf(
            "{\"通过\":false,\"阶段\":\"启动\",\"启动状态\":%u,\"世界登记状态\":%u,\"初始场景状态\":%u,\"发布状态\":%u}\n",
            static_cast<unsigned>(启动结果.状态),
            static_cast<unsigned>(启动结果.世界登记业务状态),
            static_cast<unsigned>(启动结果.初始场景业务状态),
            static_cast<unsigned>(启动结果.发布状态));
        return 21;
    }

    const auto 场景第一次 = 上下文->读取初始场景();
    const auto 场景第二次 = 上下文->读取初始场景();
    const auto 登记第一次 = 上下文->读取世界登记();
    const auto 登记第二次 = 上下文->读取世界登记();
    if (!场景第一次.成功() || !场景第二次.成功()
        || !登记第一次.成功() || !登记第二次.成功()
        || !场景第一次.事实.has_value() || !场景第二次.事实.has_value()
        || !登记第一次.登记.has_value() || !登记第二次.登记.has_value()
        || 场景第一次.状态 != 场景第二次.状态
        || !(*场景第一次.事实 == *场景第二次.事实)
        || 登记第一次.状态 != 登记第二次.状态
        || !(*登记第一次.登记 == *登记第二次.登记)
        || !上下文->初始场景完整()) {
        std::printf(
            "{\"通过\":false,\"阶段\":\"正式读回\",\"场景第一次\":%u,\"场景第二次\":%u,\"登记第一次\":%u,\"登记第二次\":%u}\n",
            static_cast<unsigned>(场景第一次.状态),
            static_cast<unsigned>(场景第二次.状态),
            static_cast<unsigned>(登记第一次.状态),
            static_cast<unsigned>(登记第二次.状态));
        return 22;
    }

    const auto& 场景 = *场景第一次.事实;
    const auto& 登记 = *登记第一次.登记;
    const auto 固定配置 = 读取生产运行期固定配置();
    const std::array<std::uint64_t, 8> 编码组{
        登记.服务身份.值,
        登记.场景标记属性类型.值,
        登记.直接父场景关系类型.值,
        登记.直接成员关系类型.值,
        登记.世界根场景.值,
        场景.初始场景.编码.值,
        场景.场景标记.编码.值,
        场景.直接父关系.编码.值};
    bool 编码唯一且非零 = true;
    for (std::size_t i = 0; i < 编码组.size(); ++i) {
        if (编码组[i] == 0) 编码唯一且非零 = false;
        for (std::size_t j = i + 1; j < 编码组.size(); ++j)
            if (编码组[i] == 编码组[j]) 编码唯一且非零 = false;
    }

    const bool 字段完整 = 编码唯一且非零
        && 启动结果.状态 == 生产运行期启动状态::已启动
        && 启动结果.世界登记业务状态 == 世界登记状态::已提交
        && 启动结果.初始场景业务状态 == 世界结构状态::已提交
        && 启动结果.发布状态 == 运行期上下文发布状态::已发布
        && 场景第一次.状态 == 世界结构状态::已读取
        && 登记第一次.状态 == 世界登记状态::已读取
        && 场景.合同版本 == L1场景结构合同版本
        && 场景.世界规则版本 == 世界登记规则版本
        && 场景.事实截止代次 == 登记.已验证事实代次
        && 场景.世界根.编码 == 登记.世界根场景
        && 场景.世界根.类型 == 节点种类::普通
        && 场景.初始场景.类型 == 节点种类::普通
        && 场景.场景标记.所属节点 == 场景.初始场景
        && 场景.场景标记.属性类型.编码 == 登记.场景标记属性类型
        && 场景.场景标记.属性类型.类型 == 节点种类::属性类型
        && 场景.场景标记.来源.编码 == 登记.服务身份
        && 场景.场景标记.I64值 == 1
        && 场景.场景标记.创建事实代次 != 0
        && 场景.场景标记.创建事实代次 <= 场景.事实截止代次
        && 场景.直接父关系.关系类型.编码 == 登记.直接父场景关系类型
        && 场景.直接父关系.源端 == 场景.世界根
        && 场景.直接父关系.目标端 == 场景.初始场景
        && 场景.直接父关系.角色或顺序 == 0
        && 固定配置.初始场景幂等身份.值 == 1;
    if (!字段完整) {
        std::printf(
            "{\"通过\":false,\"阶段\":\"字段互证\",\"事实截止\":%llu,\"登记截止\":%llu,\"初始场景\":%llu,\"关系\":%llu}\n",
            static_cast<unsigned long long>(场景.事实截止代次),
            static_cast<unsigned long long>(登记.已验证事实代次),
            static_cast<unsigned long long>(场景.初始场景.编码.值),
            static_cast<unsigned long long>(场景.直接父关系.编码.值));
        return 23;
    }

    std::printf(
        "{\"通过\":true,\"启动状态\":%u,\"世界登记启动状态\":%u,\"初始场景启动状态\":%u,\"正式场景读回状态\":%u,\"正式登记读回状态\":%u,\"发布状态\":%u,\"事实截止\":%llu,\"世界根\":%llu,\"初始场景\":%llu,\"场景标记值\":%llu,\"直接父关系\":%llu}\n",
        static_cast<unsigned>(启动结果.状态),
        static_cast<unsigned>(启动结果.世界登记业务状态),
        static_cast<unsigned>(启动结果.初始场景业务状态),
        static_cast<unsigned>(场景第一次.状态),
        static_cast<unsigned>(登记第一次.状态),
        static_cast<unsigned>(启动结果.发布状态),
        static_cast<unsigned long long>(场景.事实截止代次),
        static_cast<unsigned long long>(场景.世界根.编码.值),
        static_cast<unsigned long long>(场景.初始场景.编码.值),
        static_cast<unsigned long long>(场景.场景标记.编码.值),
        static_cast<unsigned long long>(场景.直接父关系.编码.值));
    return 0;
}
