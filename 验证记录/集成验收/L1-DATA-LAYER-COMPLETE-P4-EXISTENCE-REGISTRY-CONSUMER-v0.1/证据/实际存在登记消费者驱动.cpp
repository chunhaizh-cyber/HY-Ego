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
            "{\"通过\":false,\"阶段\":\"启动\",\"启动状态\":%u,\"世界登记状态\":%u,\"初始场景状态\":%u,\"实际存在登记状态\":%u,\"发布状态\":%u}\n",
            static_cast<unsigned>(启动结果.状态),
            static_cast<unsigned>(启动结果.世界登记业务状态),
            static_cast<unsigned>(启动结果.初始场景业务状态),
            static_cast<unsigned>(启动结果.实际存在登记业务状态),
            static_cast<unsigned>(启动结果.发布状态));
        return 31;
    }

    const auto 存在第一次 = 上下文->读取实际存在登记();
    const auto 存在第二次 = 上下文->读取实际存在登记();
    const auto 场景第一次 = 上下文->读取初始场景();
    const auto 场景第二次 = 上下文->读取初始场景();
    const auto 世界第一次 = 上下文->读取世界登记();
    const auto 世界第二次 = 上下文->读取世界登记();

    if (!存在第一次.成功() || !存在第二次.成功()
        || !场景第一次.成功() || !场景第二次.成功()
        || !世界第一次.成功() || !世界第二次.成功()
        || !存在第一次.登记.has_value() || !存在第二次.登记.has_value()
        || !场景第一次.事实.has_value() || !场景第二次.事实.has_value()
        || !世界第一次.登记.has_value() || !世界第二次.登记.has_value()
        || 存在第一次.状态 != 存在第二次.状态
        || !(*存在第一次.登记 == *存在第二次.登记)
        || 场景第一次.状态 != 场景第二次.状态
        || !(*场景第一次.事实 == *场景第二次.事实)
        || 世界第一次.状态 != 世界第二次.状态
        || !(*世界第一次.登记 == *世界第二次.登记)) {
        std::printf(
            "{\"通过\":false,\"阶段\":\"正式读回\",\"存在第一次\":%u,\"存在第二次\":%u,\"场景第一次\":%u,\"场景第二次\":%u,\"世界第一次\":%u,\"世界第二次\":%u}\n",
            static_cast<unsigned>(存在第一次.状态),
            static_cast<unsigned>(存在第二次.状态),
            static_cast<unsigned>(场景第一次.状态),
            static_cast<unsigned>(场景第二次.状态),
            static_cast<unsigned>(世界第一次.状态),
            static_cast<unsigned>(世界第二次.状态));
        return 32;
    }

    const auto& 存在登记 = *存在第一次.登记;
    const auto& 场景事实 = *场景第一次.事实;
    const auto& 世界登记 = *世界第一次.登记;
    const auto 固定配置 = 读取生产运行期固定配置();

    const bool 字段完整 =
        启动结果.状态 == 生产运行期启动状态::已启动
        && 启动结果.世界登记业务状态 == 世界登记状态::已提交
        && 启动结果.初始场景业务状态 == 世界结构状态::已提交
        && 启动结果.实际存在登记业务状态 == 实际存在状态::已提交
        && 启动结果.发布状态 == 运行期上下文发布状态::已发布
        && 存在第一次.状态 == 实际存在状态::已读取
        && 场景第一次.状态 == 世界结构状态::已读取
        && 世界第一次.状态 == 世界登记状态::已读取
        && 存在登记.合同版本 == L1实际存在合同版本
        && 存在登记.合同版本 == 2
        && 存在登记.规则版本 == L1实际存在规则版本
        && 存在登记.规则版本 == 1
        && 存在登记.首次幂等身份 == 固定配置.实际存在登记幂等身份
        && 存在登记.首次幂等身份.值 == 1
        && 有效(存在登记.服务身份)
        && 有效(存在登记.实际存在资格属性类型)
        && 存在登记.服务身份 != 存在登记.实际存在资格属性类型
        && 实际存在结构登记完整(存在登记)
        && 存在登记.事实截止代次 == 场景事实.事实截止代次
        && 场景事实.事实截止代次 == 世界登记.已验证事实代次
        && 上下文->实际存在登记完整();
    if (!字段完整) {
        std::printf(
            "{\"通过\":false,\"阶段\":\"字段互证\",\"合同版本\":%u,\"规则版本\":%u,\"首次幂等身份\":%llu,\"事实截止\":%llu,\"场景截止\":%llu,\"世界截止\":%llu,\"服务身份\":%llu,\"资格属性类型\":%llu}\n",
            存在登记.合同版本,
            存在登记.规则版本,
            static_cast<unsigned long long>(存在登记.首次幂等身份.值),
            static_cast<unsigned long long>(存在登记.事实截止代次),
            static_cast<unsigned long long>(场景事实.事实截止代次),
            static_cast<unsigned long long>(世界登记.已验证事实代次),
            static_cast<unsigned long long>(存在登记.服务身份.值),
            static_cast<unsigned long long>(存在登记.实际存在资格属性类型.值));
        return 33;
    }

    std::printf(
        "{\"通过\":true,\"启动状态\":%u,\"世界登记启动状态\":%u,\"初始场景启动状态\":%u,\"实际存在登记启动状态\":%u,\"实际存在读回状态\":%u,\"初始场景读回状态\":%u,\"世界登记读回状态\":%u,\"发布状态\":%u,\"合同版本\":%u,\"规则版本\":%u,\"首次幂等身份\":%llu,\"事实截止\":%llu,\"服务身份\":%llu,\"资格属性类型\":%llu}\n",
        static_cast<unsigned>(启动结果.状态),
        static_cast<unsigned>(启动结果.世界登记业务状态),
        static_cast<unsigned>(启动结果.初始场景业务状态),
        static_cast<unsigned>(启动结果.实际存在登记业务状态),
        static_cast<unsigned>(存在第一次.状态),
        static_cast<unsigned>(场景第一次.状态),
        static_cast<unsigned>(世界第一次.状态),
        static_cast<unsigned>(启动结果.发布状态),
        存在登记.合同版本,
        存在登记.规则版本,
        static_cast<unsigned long long>(存在登记.首次幂等身份.值),
        static_cast<unsigned long long>(存在登记.事实截止代次),
        static_cast<unsigned long long>(存在登记.服务身份.值),
        static_cast<unsigned long long>(存在登记.实际存在资格属性类型.值));
    return 0;
}
