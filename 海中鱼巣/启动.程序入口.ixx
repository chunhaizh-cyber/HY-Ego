module;

#include "启动选项.数据.h"
#include "程序运行结果.数据.h"

#include <string_view>

export module 海中鱼巣.启动.程序入口;

export namespace 海中鱼巣 {

启动选项解析结果 解析并验证启动选项(
    int 参数数量,
    char* const 参数组[]) noexcept {
    if (参数数量 < 1) {
        return {启动选项解析状态::参数数量无效, {}, 0};
    }
    if (参数组 == nullptr) {
        return {启动选项解析状态::参数数组为空, {}, 0};
    }

    bool 已见模式 = false;
    std::string_view 已见参数;
    启动模式 模式 = 启动模式::普通控制面板;
    for (int 参数索引 = 1; 参数索引 < 参数数量; ++参数索引) {
        if (参数组[参数索引] == nullptr) {
            return {启动选项解析状态::参数指针为空, {}, static_cast<std::size_t>(参数索引)};
        }
        const std::string_view 参数 = 参数组[参数索引];
        启动模式 当前模式{};
        if (参数 == "--headless") {
            当前模式 = 启动模式::无窗口常驻;
        } else if (参数 == "--runtime-context") {
            当前模式 = 启动模式::生产运行期;
        } else {
            return {启动选项解析状态::未知参数, {}, static_cast<std::size_t>(参数索引)};
        }

        if (已见模式) {
            const auto 状态 = 参数 == 已见参数
                ? 启动选项解析状态::重复模式参数
                : 启动选项解析状态::模式组合冲突;
            return {状态, {}, static_cast<std::size_t>(参数索引)};
        }
        已见模式 = true;
        已见参数 = 参数;
        模式 = 当前模式;
    }

    return {启动选项解析状态::已接受, {模式}, 0};
}

int 映射进程退出码(
    const 启动选项解析结果& 解析,
    const 程序运行结果* 运行) noexcept {
    if (!解析.成功()) {
        return 2;
    }
    if (运行 == nullptr) {
        return 1;
    }
    return 运行->成功() ? 0 : 1;
}

}
