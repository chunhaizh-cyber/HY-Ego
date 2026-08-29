module;

#include <cstdint>
#include <iostream>

export module 海中鱼巣.端到端测试.本能UTC时间证据;

import 海中鱼巣.适配.适配器.UTC时钟;

export namespace 海中鱼巣 {
int 运行本能UTC时间证据端到端测试() noexcept;
}

module :private;

namespace 海中鱼巣 {

int 运行本能UTC时间证据端到端测试() noexcept {
    const auto 失败 = [](const char* 编号, const char* 步骤) noexcept {
        std::cout << "[INSTINCT-UTC][" << 编号 << "] FAIL: "
                  << 步骤 << '\n';
        return 1;
    };
    const auto 通过 = [](const char* 编号, const char* 步骤) {
        std::cout << "[INSTINCT-UTC][" << 编号 << "] PASS: "
                  << 步骤 << '\n';
    };

    try {
        UTC时钟适配器 适配器;
        const auto 首次 = 适配器.读取当前UTC时间证据_v1({});
        if (!首次.成功() || !首次.证据
            || 首次.证据->合同版本 != UTC时间证据合同版本_v1
            || 首次.证据->UTC纳秒 <= 0
            || 首次.证据->时间源版本 != 系统UTC时间源版本_v1)
            return 失败("U00", "默认请求完整读取");
        通过("U00", "默认请求返回正 UTC 纳秒和完整版本");

        const auto 坏合同 = 适配器.读取当前UTC时间证据_v1({2, 1});
        if (坏合同.状态 != UTC时间证据读取状态_v1::请求拒绝
            || 坏合同.证据 || 坏合同.成功())
            return 失败("U01", "合同版本拒绝");
        通过("U01", "错误合同版本返回请求拒绝和空载荷");

        const auto 零版本 = 适配器.读取当前UTC时间证据_v1({1, 0});
        if (零版本.状态 != UTC时间证据读取状态_v1::请求拒绝
            || 零版本.证据 || 零版本.成功())
            return 失败("U02", "零时间源版本拒绝");
        通过("U02", "零时间源版本返回请求拒绝和空载荷");

        const auto 漂移 = 适配器.读取当前UTC时间证据_v1({1, 2});
        if (漂移.状态 != UTC时间证据读取状态_v1::时间源版本漂移
            || 漂移.证据 || 漂移.成功())
            return 失败("U03", "未知时间源版本漂移");
        通过("U03", "未知非零时间源版本返回漂移和空载荷");

        const auto 再次 = 适配器.读取当前UTC时间证据_v1({});
        if (!再次.成功() || !再次.证据 || 再次.证据->UTC纳秒 <= 0)
            return 失败("U04", "连续合法读取完整");
        通过("U04", "连续读取均完整且不附加伪单调条件");
        return 0;
    } catch (...) {
        return 失败("EX", "未处理异常");
    }
}

} // namespace 海中鱼巣
