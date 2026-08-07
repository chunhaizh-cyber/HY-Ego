#include <cstdint>

import 海中鱼巣.启动.生产运行期;

int main() {
    using namespace 海中鱼巣;

    生产运行期会话 会话;
    const auto 启动结果 = 会话.启动({生产运行期启动请求当前版本});
    const auto* 上下文 = 启动结果.租约.读取();
    if (!启动结果.成功() || 上下文 == nullptr) return 61;

    const auto 世界 = 上下文->读取世界登记();
    if (!世界.成功() || !世界.登记) return 62;

    auto& 服务 = 上下文->读取L1特征定义服务();
    const 特征定义登记请求 请求{
        L1特征定义合同版本,
        L1特征定义规则版本,
        {1},
        世界.登记->已验证事实代次};
    const auto 结果 = 服务.建立登记(请求);
    return 结果.状态 == 特征定义状态::已提交 ? 0 : 63;
}
