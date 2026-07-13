// 文件规则：结构写入执行器只拥有一次独占许可和同步会话边界，不承载领域业务准入。
module;

#include "结构事务接线.数据.h"
#include "主信息仓库.h"
#include "节点仓库.h"
#include "关系仓库.h"
#include "索引仓库.h"

#include <functional>
#include <utility>

export module 海中鱼巣.核心.执行器.结构写入;

export import 海中鱼巣.核心.会话.结构写入;

export namespace 海中鱼巣 {

class 结构写入执行器 final {
public:
    结构写入执行器(
        结构事务接线 接线,
        主信息仓库* 主信息,
        节点仓库* 节点,
        关系仓库* 关系,
        索引仓库* 索引)
        : 接线_(std::move(接线)), 主信息_(主信息), 节点_(节点), 关系_(关系), 索引_(索引) {
    }

    bool 有效() const {
        return 接线_.已接域() && 主信息_ != nullptr && 节点_ != nullptr
            && 关系_ != nullptr && 索引_ != nullptr;
    }

    结构写入结果 执行(const std::function<void(结构写入会话&)>& 回调) const {
        if (!有效() || !回调) return {结构写入状态::入口拒绝, 0, 0, 0};
        auto 许可 = 接线_.取得独占许可(接线_.运行期状态);
        if (!许可.有效()) return {结构写入状态::许可拒绝, 0, 0, 0};

        结构写入结果 输出;
        结构写入会话 会话(*主信息_, *节点_, *关系_, *索引_, 许可.读取令牌());
        try {
            回调(会话);
        } catch (...) {
            const auto 撤销 = 会话.完成撤销();
            return {结构写入状态::内部不一致, 0, 0,
                撤销.状态 == 结构写入状态::候选已撤销 ? 0U : 1U};
        }

        if (会话.有失败()) {
            const auto 首次失败 = 会话.读取首次失败();
            const auto 撤销 = 会话.完成撤销();
            return 撤销.状态 == 结构写入状态::候选已撤销
                ? 首次失败
                : 结构写入结果{结构写入状态::内部不一致, 0, 0, 1};
        }
        if (会话.已请求提交()) return 会话.完成提交();

        const bool 显式撤销 = 会话.已请求撤销();
        const auto 撤销 = 会话.完成撤销();
        if (撤销.状态 != 结构写入状态::候选已撤销) return 撤销;
        return 显式撤销 ? 撤销 : 结构写入结果{结构写入状态::入口拒绝, 0, 0, 0};
    }

private:
    结构事务接线 接线_;
    主信息仓库* 主信息_ = nullptr;
    节点仓库* 节点_ = nullptr;
    关系仓库* 关系_ = nullptr;
    索引仓库* 索引_ = nullptr;
};

}
