// 文件规则：只承载启动选项值类型，不保存原始参数指针或人读诊断文本。
#pragma once

#include <cstddef>
#include <cstdint>

namespace 海中鱼巣 {

enum class 启动模式 : std::uint8_t {
    普通控制面板 = 0,
    无窗口常驻 = 1,
    生产运行期 = 2
};

enum class 启动选项解析状态 : std::uint8_t {
    已接受 = 0,
    参数数量无效 = 1,
    参数数组为空 = 2,
    参数指针为空 = 3,
    未知参数 = 4,
    重复模式参数 = 5,
    模式组合冲突 = 6
};

struct 启动选项 {
    启动模式 模式 = 启动模式::普通控制面板;
};

struct 启动选项解析结果 {
    启动选项解析状态 状态 = 启动选项解析状态::参数数量无效;
    启动选项 选项;
    std::size_t 拒绝参数索引 = 0;

    bool 成功() const noexcept {
        return 状态 == 启动选项解析状态::已接受;
    }
};

}
