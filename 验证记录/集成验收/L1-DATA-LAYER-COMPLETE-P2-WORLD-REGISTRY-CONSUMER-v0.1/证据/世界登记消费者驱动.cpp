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
            "{\"通过\":false,\"阶段\":\"启动\",\"启动状态\":%u,\"世界登记状态\":%u,\"发布状态\":%u}\n",
            static_cast<unsigned>(启动结果.状态),
            static_cast<unsigned>(启动结果.世界登记业务状态),
            static_cast<unsigned>(启动结果.发布状态));
        return 11;
    }

    const auto 第一次读回 = 上下文->读取世界登记();
    const auto 第二次读回 = 上下文->读取世界登记();
    const auto 登记逐字段相同 = [](const auto& 左, const auto& 右) {
        return 左.合同版本 == 右.合同版本
            && 左.世界规则版本 == 右.世界规则版本
            && 左.首次幂等身份.值 == 右.首次幂等身份.值
            && 左.首次中性写集幂等键 == 右.首次中性写集幂等键
            && 左.已验证事实代次 == 右.已验证事实代次
            && 左.服务身份.值 == 右.服务身份.值
            && 左.场景标记属性类型.值 == 右.场景标记属性类型.值
            && 左.直接父场景关系类型.值 == 右.直接父场景关系类型.值
            && 左.直接成员关系类型.值 == 右.直接成员关系类型.值
            && 左.世界根场景.值 == 右.世界根场景.值;
    };
    if (!第一次读回.成功() || !第二次读回.成功()
        || !第一次读回.登记.has_value() || !第二次读回.登记.has_value()
        || 第一次读回.状态 != 第二次读回.状态
        || !登记逐字段相同(*第一次读回.登记, *第二次读回.登记)
        || !上下文->世界登记完整()) {
        std::printf(
            "{\"通过\":false,\"阶段\":\"正式读回\",\"第一次状态\":%u,\"第二次状态\":%u}\n",
            static_cast<unsigned>(第一次读回.状态),
            static_cast<unsigned>(第二次读回.状态));
        return 12;
    }

    const auto& 登记 = *第一次读回.登记;
    const auto 固定配置 = 读取生产运行期固定配置();
    const std::uint64_t 编码组[]{
        登记.服务身份.值,
        登记.场景标记属性类型.值,
        登记.直接父场景关系类型.值,
        登记.直接成员关系类型.值,
        登记.世界根场景.值};
    bool 编码唯一且非零 = true;
    for (std::size_t i = 0; i < 5; ++i) {
        if (编码组[i] == 0) 编码唯一且非零 = false;
        for (std::size_t j = i + 1; j < 5; ++j)
            if (编码组[i] == 编码组[j]) 编码唯一且非零 = false;
    }
    const bool 字段完整 = 编码唯一且非零
        && 启动结果.状态 == 生产运行期启动状态::已启动
        && 启动结果.世界登记业务状态 == 世界登记状态::已提交
        && 启动结果.发布状态 == 运行期上下文发布状态::已发布
        && 第一次读回.状态 == 世界登记状态::已读取
        && 登记.合同版本 == 固定配置.世界登记.合同版本
        && 登记.世界规则版本 == 固定配置.世界登记.世界规则版本
        && 登记.首次幂等身份.值 == 固定配置.世界登记.幂等身份.值
        && 登记.首次中性写集幂等键 == 0x0100'0000'0000'0001ULL
        && 登记.已验证事实代次 == 1;
    if (!字段完整) {
        std::printf(
            "{\"通过\":false,\"阶段\":\"字段互证\",\"事实代次\":%llu,\"幂等键\":%llu}\n",
            static_cast<unsigned long long>(登记.已验证事实代次),
            static_cast<unsigned long long>(登记.首次中性写集幂等键));
        return 13;
    }

    std::printf(
        "{\"通过\":true,\"启动状态\":%u,\"世界登记启动状态\":%u,\"正式读回状态\":%u,\"发布状态\":%u,\"事实代次\":%llu,\"幂等键\":%llu,\"服务身份\":%llu,\"场景标记属性类型\":%llu,\"直接父场景关系类型\":%llu,\"直接成员关系类型\":%llu,\"世界根场景\":%llu}\n",
        static_cast<unsigned>(启动结果.状态),
        static_cast<unsigned>(启动结果.世界登记业务状态),
        static_cast<unsigned>(第一次读回.状态),
        static_cast<unsigned>(启动结果.发布状态),
        static_cast<unsigned long long>(登记.已验证事实代次),
        static_cast<unsigned long long>(登记.首次中性写集幂等键),
        static_cast<unsigned long long>(登记.服务身份.值),
        static_cast<unsigned long long>(登记.场景标记属性类型.值),
        static_cast<unsigned long long>(登记.直接父场景关系类型.值),
        static_cast<unsigned long long>(登记.直接成员关系类型.值),
        static_cast<unsigned long long>(登记.世界根场景.值));
    return 0;
}
