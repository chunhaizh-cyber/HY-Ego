// 文件规则：运行期只读查询适配门面只持上下文租约并按次返回值；不得缓存上下文或组合器指针。
module;

#include <utility>

export module 海中鱼巣.适配.查询.运行期只读;

import 海中鱼巣.启动.运行期上下文;
import 海中鱼巣.领域.组合.运行期只读查询;

export namespace 海中鱼巣 {

class 运行期只读查询适配器 final {
public:
    explicit 运行期只读查询适配器(运行期上下文租约 租约)
        : 租约_(std::move(租约)) {
    }

    运行期概念命名查询结果 查询概念命名(
        const 运行期概念命名查询请求& 请求) const {
        const auto* 上下文 = 租约_.读取();
        if (上下文 == nullptr) return {};
        const auto* 查询 = 上下文->读取候选只读查询();
        return 查询 == nullptr ? 运行期概念命名查询结果{}
            : 查询->查询概念命名(请求);
    }

    运行期任务结果复核结果 复核任务结果(
        const 运行期任务结果复核请求& 请求) const {
        const auto* 上下文 = 租约_.读取();
        if (上下文 == nullptr) {
            运行期任务结果复核结果 结果;
            结果.回执 = 请求.回执;
            return 结果;
        }
        const auto* 查询 = 上下文->读取候选只读查询();
        if (查询 != nullptr) return 查询->复核任务结果(请求);
        运行期任务结果复核结果 结果;
        结果.回执 = 请求.回执;
        return 结果;
    }

private:
    运行期上下文租约 租约_;
};

}
