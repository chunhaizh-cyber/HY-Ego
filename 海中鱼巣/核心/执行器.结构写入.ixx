// 文件规则：结构写入执行器只拥有一次独占许可和同步会话边界，不承载领域业务准入。
module;

#include "结构事务接线.数据.h"
#include "主信息仓库.h"
#include "节点仓库.h"
#include "关系仓库.h"
#include "索引仓库.h"
#include "容错检查.h"

#include <functional>
#include <type_traits>
#include <utility>

export module 海中鱼巣.核心.执行器.结构写入;

export import 海中鱼巣.核心.会话.结构写入;

export namespace 海中鱼巣 {

class 结构写入事务参与者 {
public:
    virtual ~结构写入事务参与者() = default;

protected:
    结构写入事务参与者() = default;

private:
    friend class 结构写入执行器;

    virtual 结构写入结果 准备提交(const 结构提交准备只读视图& 视图) = 0;
    virtual 结构写入结果 确认待发布() = 0;
    virtual void 完成发布() noexcept = 0;
    virtual 结构写入结果 完成撤销() = 0;
};

#ifdef HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST
struct 结构写入故障自检配置 {
    结构写入确认故障点 确认故障点 = 结构写入确认故障点::无;
    std::size_t 确认故障序号 = 0;
    结构写入撤销故障点 撤销故障点 = 结构写入撤销故障点::无;
    bool 最终发布前故障 = false;
    bool 完成发布观察 = false;
};
#endif

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
            return 撤销完成(撤销)
                ? 结构写入结果{结构写入状态::内部不一致, 0, 0, 0}
                : 隔离后内部不一致(许可);
        }

        if (会话.有失败()) {
            const auto 首次失败 = 会话.读取首次失败();
            const auto 撤销 = 会话.完成撤销();
            return 撤销完成(撤销)
                ? 首次失败
                : 隔离后内部不一致(许可);
        }
        if (会话.已请求提交()) {
            const auto 确认 = 会话.完成确认();
            if (!确认.成功()) {
                const auto 撤销 = 会话.完成撤销();
                return 撤销完成(撤销) ? 确认 : 隔离后内部不一致(许可);
            }
            会话.完成发布();
            return 确认;
        }

        const bool 显式撤销 = 会话.已请求撤销();
        const auto 撤销 = 会话.完成撤销();
        if (!撤销完成(撤销)) return 隔离后内部不一致(许可);
        return 显式撤销 ? 撤销 : 结构写入结果{结构写入状态::入口拒绝, 0, 0, 0};
    }

    template <typename 参与者类型>
    结构写入结果 执行(
        const std::function<void(结构写入会话&)>& 回调,
        参与者类型& 参与者) const {
        static_assert(std::is_base_of_v<结构写入事务参与者, 参与者类型>);
        if (!有效() || !回调) return {结构写入状态::入口拒绝, 0, 0, 0};
        auto 许可 = 接线_.取得独占许可(接线_.运行期状态);
        if (!许可.有效()) return {结构写入状态::许可拒绝, 0, 0, 0};

        结构写入会话 会话(*主信息_, *节点_, *关系_, *索引_, 许可.读取令牌());
        try {
            回调(会话);
        } catch (...) {
            const auto 参与者撤销 = 安全完成参与者撤销(参与者);
            const auto 会话撤销 = 会话.完成撤销();
            return 汇总撤销结果(
                许可, 参与者撤销, 会话撤销,
                {结构写入状态::内部不一致, 0, 0, 0});
        }

        if (会话.有失败()) {
            const auto 首次失败 = 会话.读取首次失败();
            const auto 参与者撤销 = 安全完成参与者撤销(参与者);
            const auto 会话撤销 = 会话.完成撤销();
            return 汇总撤销结果(许可, 参与者撤销, 会话撤销, 首次失败);
        }

        if (!会话.已请求提交()) {
            const bool 显式撤销 = 会话.已请求撤销();
            const auto 参与者撤销 = 安全完成参与者撤销(参与者);
            const auto 会话撤销 = 会话.完成撤销();
            if (!参与者撤销.成功() || !撤销完成(会话撤销)) return 隔离后内部不一致(许可);
            return 显式撤销
                ? 会话撤销
                : 结构写入结果{结构写入状态::入口拒绝, 0, 0, 0};
        }

        const auto 准备结果 = 安全准备参与者(会话, 参与者);
        if (!准备结果.成功()) {
            const auto 参与者撤销 = 安全完成参与者撤销(参与者);
            const auto 会话撤销 = 会话.完成撤销();
            return 汇总撤销结果(许可, 参与者撤销, 会话撤销, 准备结果);
        }

        const auto 会话确认 = 会话.完成确认();
        if (!会话确认.成功()) {
            const auto 参与者撤销 = 安全完成参与者撤销(参与者);
            const auto 会话撤销 = 会话.完成撤销();
            return 汇总撤销结果(许可, 参与者撤销, 会话撤销, 会话确认);
        }

        const auto 参与者确认 = 安全确认参与者(参与者);
        if (!参与者确认.成功()) {
            const auto 参与者撤销 = 安全完成参与者撤销(参与者);
            const auto 会话撤销 = 会话.完成撤销();
            return 汇总撤销结果(许可, 参与者撤销, 会话撤销, 参与者确认);
        }

        会话.完成发布();
        完成参与者发布(参与者);
        return 会话确认;
    }

#ifdef HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST
    template <typename 参与者类型>
    结构写入结果 执行确认前故障自检(
        const std::function<void(结构写入会话&)>& 业务回调,
        参与者类型& 参与者,
        const std::function<void()>& 观察回调) const {
        return 执行原子收口故障自检(
            业务回调,
            参与者,
            {结构写入确认故障点::无, 0, 结构写入撤销故障点::无, true, false},
            观察回调);
    }

    template <typename 参与者类型>
    结构写入结果 执行发布可见性自检(
        const std::function<void(结构写入会话&)>& 业务回调,
        参与者类型& 参与者,
        const std::function<void()>& 观察回调) const {
        if (!观察回调) return {结构写入状态::入口拒绝, 0, 0, 0};
        return 执行原子收口故障自检(
            业务回调,
            参与者,
            {结构写入确认故障点::无, 0, 结构写入撤销故障点::无, false, true},
            观察回调);
    }

    template <typename 参与者类型>
    结构写入结果 执行原子收口故障自检(
        const std::function<void(结构写入会话&)>& 业务回调,
        参与者类型& 参与者,
        const 结构写入故障自检配置& 配置,
        const std::function<void()>& 观察回调 = {}) const {
        static_assert(std::is_base_of_v<结构写入事务参与者, 参与者类型>);
        const bool 已配置故障 = 配置.确认故障点 != 结构写入确认故障点::无
            || 配置.撤销故障点 != 结构写入撤销故障点::无
            || 配置.最终发布前故障 || 配置.完成发布观察;
        if (!有效() || !业务回调 || !已配置故障) {
            return {结构写入状态::入口拒绝, 0, 0, 0};
        }
        auto 许可 = 接线_.取得独占许可(接线_.运行期状态);
        if (!许可.有效()) return {结构写入状态::许可拒绝, 0, 0, 0};

        结构写入会话 会话(*主信息_, *节点_, *关系_, *索引_, 许可.读取令牌());
        try {
            业务回调(会话);
        } catch (...) {
            const auto 参与者撤销 = 安全完成参与者撤销(参与者);
            const auto 会话撤销 = 会话.完成撤销();
            return 汇总撤销结果(
                许可, 参与者撤销, 会话撤销,
                {结构写入状态::内部不一致, 0, 0, 0});
        }

        if (会话.有失败()) {
            const auto 首次失败 = 会话.读取首次失败();
            const auto 参与者撤销 = 安全完成参与者撤销(参与者);
            const auto 会话撤销 = 会话.完成撤销();
            return 汇总撤销结果(许可, 参与者撤销, 会话撤销, 首次失败);
        }

        if (!会话.已请求提交()) {
            const bool 显式撤销 = 会话.已请求撤销();
            const auto 参与者撤销 = 安全完成参与者撤销(参与者);
            const auto 会话撤销 = 会话.完成撤销();
            if (!参与者撤销.成功() || !撤销完成(会话撤销)) return 隔离后内部不一致(许可);
            return 显式撤销
                ? 会话撤销
                : 结构写入结果{结构写入状态::入口拒绝, 0, 0, 0};
        }

        const auto 准备结果 = 安全准备参与者(会话, 参与者);
        if (!准备结果.成功()) {
            const auto 参与者撤销 = 安全完成参与者撤销(参与者);
            const auto 会话撤销 = 会话.完成撤销();
            return 汇总撤销结果(许可, 参与者撤销, 会话撤销, 准备结果);
        }

        const auto 会话确认 = 配置.确认故障点 == 结构写入确认故障点::无
            ? 会话.完成确认()
            : 会话.完成确认故障自检(配置.确认故障点, 配置.确认故障序号);
        if (!会话确认.成功()) {
            const auto 参与者撤销 = 安全完成参与者撤销(参与者);
            const auto 会话撤销 = 配置.撤销故障点 == 结构写入撤销故障点::无
                ? 会话.完成撤销() : 会话.完成撤销故障自检(配置.撤销故障点);
            return 汇总撤销结果(许可, 参与者撤销, 会话撤销, 会话确认);
        }
        const auto 参与者确认 = 安全确认参与者(参与者);
        if (!参与者确认.成功()) {
            const auto 参与者撤销 = 安全完成参与者撤销(参与者);
            const auto 会话撤销 = 配置.撤销故障点 == 结构写入撤销故障点::无
                ? 会话.完成撤销() : 会话.完成撤销故障自检(配置.撤销故障点);
            return 汇总撤销结果(许可, 参与者撤销, 会话撤销, 参与者确认);
        }
        if (观察回调) {
            try { 观察回调(); } catch (...) {}
        }
        if (配置.完成发布观察) {
            会话.完成发布();
            完成参与者发布(参与者);
            return 会话确认;
        }
        if (!配置.最终发布前故障 && 配置.撤销故障点 == 结构写入撤销故障点::无) {
            return {结构写入状态::入口拒绝, 0, 0, 0};
        }
        const auto 参与者撤销 = 安全完成参与者撤销(参与者);
        const auto 会话撤销 = 配置.撤销故障点 == 结构写入撤销故障点::无
            ? 会话.完成撤销() : 会话.完成撤销故障自检(配置.撤销故障点);
        return 汇总撤销结果(
            许可,
            参与者撤销,
            会话撤销,
            {结构写入状态::内部不一致, 0, 0, 0});
    }
#endif

private:
    static bool 撤销完成(const 结构写入结果& 结果) noexcept {
        return 结果.状态 == 结构写入状态::候选已撤销;
    }

    bool 标记撤销失败隔离(const 结构事务许可& 许可) const noexcept {
        return 许可.有效() && 接线_.标记撤销失败隔离 != nullptr
            && 接线_.标记撤销失败隔离(接线_.运行期状态, 许可.读取令牌());
    }

    结构写入结果 隔离后内部不一致(const 结构事务许可& 许可) const noexcept {
        const bool 已隔离 = 标记撤销失败隔离(许可);
        海中鱼巣容错检查(false, L"结构写入撤销未完整成功，当前事务域已请求隔离。");
        return {结构写入状态::内部不一致, 0, 0, 已隔离 ? 0U : 1U};
    }

    结构写入结果 汇总撤销结果(
        const 结构事务许可& 许可,
        const 结构写入结果& 参与者撤销,
        const 结构写入结果& 会话撤销,
        const 结构写入结果& 原结果) const noexcept {
        return 参与者撤销.成功() && 撤销完成(会话撤销)
            ? 原结果 : 隔离后内部不一致(许可);
    }

    template <typename 参与者类型>
    static 结构写入结果 安全准备参与者(
        const 结构写入会话& 会话,
        参与者类型& 参与者) noexcept {
        static_assert(std::is_base_of_v<结构写入事务参与者, 参与者类型>);
        try {
            const 结构提交准备只读视图 视图(会话);
            return static_cast<结构写入事务参与者&>(参与者).准备提交(视图);
        } catch (...) {
            return {结构写入状态::内部不一致, 0, 0, 0};
        }
    }

    template <typename 参与者类型>
    static 结构写入结果 安全完成参与者撤销(参与者类型& 参与者) noexcept {
        static_assert(std::is_base_of_v<结构写入事务参与者, 参与者类型>);
        try {
            return static_cast<结构写入事务参与者&>(参与者).完成撤销();
        } catch (...) {
            return {结构写入状态::内部不一致, 0, 0, 0};
        }
    }

    template <typename 参与者类型>
    static 结构写入结果 安全确认参与者(参与者类型& 参与者) noexcept {
        static_assert(std::is_base_of_v<结构写入事务参与者, 参与者类型>);
        try {
            return static_cast<结构写入事务参与者&>(参与者).确认待发布();
        } catch (...) {
            return {结构写入状态::内部不一致, 0, 0, 0};
        }
    }

    template <typename 参与者类型>
    static void 完成参与者发布(参与者类型& 参与者) noexcept {
        static_assert(std::is_base_of_v<结构写入事务参与者, 参与者类型>);
        static_cast<结构写入事务参与者&>(参与者).完成发布();
    }

    结构事务接线 接线_;
    主信息仓库* 主信息_ = nullptr;
    节点仓库* 节点_ = nullptr;
    关系仓库* 关系_ = nullptr;
    索引仓库* 索引_ = nullptr;
};

}
