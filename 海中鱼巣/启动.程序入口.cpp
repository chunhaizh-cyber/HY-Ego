#include "启动.程序入口.h"

#include "启动选项.数据.h"
#include "程序运行结果.数据.h"
#include "领域/新_语言类.h"

#include <string_view>
#include <utility>

namespace 海中鱼巣 {

启动选项解析结果 解析并验证启动选项(
    int 参数数量,
    char* const 参数组[]) noexcept {
    if (参数数量 < 1) {
        return {启动选项解析状态::参数数量无效, {}, 0};
    }
    if (参数组 == nullptr) {
        return {启动选项解析状态::参数数组为空, {}, 0};
    }

    std::size_t 当前参数索引 = 0;
    try {
        bool 已见模式 = false;
        bool 已见网页服务对象 = false;
        std::size_t 网页服务对象参数索引 = 0;
        std::string_view 已见参数;
        启动选项 选项;
        constexpr std::string_view 网页服务对象前缀 =
            "--web-service-object-login=";
        for (int 参数索引 = 1; 参数索引 < 参数数量; ++参数索引) {
            当前参数索引 = static_cast<std::size_t>(参数索引);
            if (参数组[参数索引] == nullptr) {
                return {启动选项解析状态::参数指针为空, {}, 当前参数索引};
            }
            const std::string_view 参数 = 参数组[参数索引];
            if (参数.starts_with(网页服务对象前缀)) {
                if (已见网页服务对象) {
                    return {启动选项解析状态::重复网页服务对象参数,
                        {}, 当前参数索引};
                }
                const auto 登录名 = 参数.substr(网页服务对象前缀.size());
                if (登录名.empty()) {
                    return {启动选项解析状态::网页服务对象登录名为空,
                        {}, 当前参数索引};
                }
                选项.本次网页会话服务对象登录名.assign(登录名);
                if (!新_语言类::严格UTF8字节串有效(
                        选项.本次网页会话服务对象登录名)) {
                    return {启动选项解析状态::网页服务对象登录名无效,
                        {}, 当前参数索引};
                }
                已见网页服务对象 = true;
                网页服务对象参数索引 = 当前参数索引;
                continue;
            }
            启动模式 当前模式{};
            if (参数 == "--headless") {
                当前模式 = 启动模式::无窗口常驻;
            } else if (参数 == "--runtime-context") {
                当前模式 = 启动模式::生产运行期;
            } else {
                return {启动选项解析状态::未知参数, {}, 当前参数索引};
            }

            if (已见模式) {
                const auto 状态 = 参数 == 已见参数
                    ? 启动选项解析状态::重复模式参数
                    : 启动选项解析状态::模式组合冲突;
                return {状态, {}, 当前参数索引};
            }
            已见模式 = true;
            已见参数 = 参数;
            选项.模式 = 当前模式;
        }

        if (已见网页服务对象 && 选项.模式 != 启动模式::普通控制面板) {
            return {启动选项解析状态::网页服务对象模式冲突,
                {}, 网页服务对象参数索引};
        }
        return {启动选项解析状态::已接受, std::move(选项), 0};
    } catch (...) {
        return {启动选项解析状态::资源失败, {}, 当前参数索引};
    }
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
