module;

#include <chrono>
#include <cstdint>
#include <optional>
#include <utility>

export module 海中鱼巣.适配.适配器.UTC时钟;

export namespace 海中鱼巣 {

inline constexpr std::uint32_t UTC时间证据合同版本_v1 = 1;
inline constexpr std::uint32_t 系统UTC时间源版本_v1 = 1;

struct UTC时间证据读取请求_v1 final {
    std::uint32_t 合同版本 = UTC时间证据合同版本_v1;
    std::uint32_t 期望时间源版本 = 系统UTC时间源版本_v1;
};

enum class UTC时间证据读取状态_v1 : std::uint8_t {
    已读取 = 1,
    请求拒绝 = 2,
    时间源版本漂移 = 3,
    计数不可表示 = 4,
    内部不一致 = 5
};

struct 可信UTC时间证据_v1 final {
    std::uint32_t 合同版本 = UTC时间证据合同版本_v1;
    std::int64_t UTC纳秒 = 0;
    std::uint32_t 时间源版本 = 系统UTC时间源版本_v1;
};

struct UTC时间证据读取结果_v1 final {
    UTC时间证据读取状态_v1 状态 = UTC时间证据读取状态_v1::请求拒绝;
    std::optional<可信UTC时间证据_v1> 证据;

    bool 成功() const noexcept {
        return 状态 == UTC时间证据读取状态_v1::已读取
            && 证据
            && 证据->合同版本 == UTC时间证据合同版本_v1
            && 证据->UTC纳秒 > 0
            && 证据->时间源版本 == 系统UTC时间源版本_v1;
    }
};

class UTC时钟适配器 final {
public:
    UTC时间证据读取结果_v1 读取当前UTC时间证据_v1(
        const UTC时间证据读取请求_v1& 请求) const noexcept {
        if (请求.合同版本 != UTC时间证据合同版本_v1
            || 请求.期望时间源版本 == 0) {
            return {UTC时间证据读取状态_v1::请求拒绝, std::nullopt};
        }
        if (请求.期望时间源版本 != 系统UTC时间源版本_v1) {
            return {UTC时间证据读取状态_v1::时间源版本漂移,
                std::nullopt};
        }
        try {
            const auto 计数 = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
            if (!std::in_range<std::int64_t>(计数)) {
                return {UTC时间证据读取状态_v1::计数不可表示,
                    std::nullopt};
            }
            const auto UTC纳秒 = static_cast<std::int64_t>(计数);
            if (UTC纳秒 <= 0) {
                return {UTC时间证据读取状态_v1::内部不一致,
                    std::nullopt};
            }
            return {UTC时间证据读取状态_v1::已读取,
                可信UTC时间证据_v1{
                    UTC时间证据合同版本_v1, UTC纳秒,
                    系统UTC时间源版本_v1}};
        } catch (...) {
            return {UTC时间证据读取状态_v1::内部不一致, std::nullopt};
        }
    }
};

} // namespace 海中鱼巣
