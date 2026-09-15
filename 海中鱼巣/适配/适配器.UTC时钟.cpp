#include "适配器.UTC时钟.h"

#include <chrono>
#include <utility>

namespace 海中鱼巣 {

UTC时间证据读取结果_v1 UTC时钟适配器::读取当前UTC时间证据_v1(
    const UTC时间证据读取请求_v1& 请求) const noexcept {
    if (请求.合同版本 != UTC时间证据合同版本_v1
        || 请求.期望时间源版本 == 0) {
        return {UTC时间证据读取状态_v1::请求拒绝, std::nullopt};
    }
    if (请求.期望时间源版本 != 系统UTC时间源版本_v1) {
        return {UTC时间证据读取状态_v1::时间源版本漂移, std::nullopt};
    }
    try {
        const auto 计数 = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        if (!std::in_range<std::int64_t>(计数)) {
            return {UTC时间证据读取状态_v1::计数不可表示, std::nullopt};
        }
        const auto UTC纳秒 = static_cast<std::int64_t>(计数);
        if (UTC纳秒 <= 0) {
            return {UTC时间证据读取状态_v1::内部不一致, std::nullopt};
        }
        return {UTC时间证据读取状态_v1::已读取,
            可信UTC时间证据_v1{
                UTC时间证据合同版本_v1, UTC纳秒,
                系统UTC时间源版本_v1}};
    } catch (...) {
        return {UTC时间证据读取状态_v1::内部不一致, std::nullopt};
    }
}

} // namespace 海中鱼巣
