// 文件规则：UTC 时钟适配器只读取系统 UTC 时间证据；不得承载机器事实、线程或领域服务。
#pragma once

#include <cstdint>
#include <optional>

namespace 海中鱼巣 {

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
        const UTC时间证据读取请求_v1& 请求) const noexcept;
};

} // namespace 海中鱼巣
