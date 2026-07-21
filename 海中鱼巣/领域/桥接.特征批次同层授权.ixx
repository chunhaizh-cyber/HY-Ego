// 文件规则：本桥只以依赖类型模板连接两个具名同层数据操作，不拥有领域类型、事务或业务裁决。
module;

#include <utility>

export module 海中鱼巣.领域.桥接.特征批次同层授权;

export namespace 海中鱼巣 {

class 特征批次同层授权桥 final {
public:
    template <typename 特征数据操作类型, typename... 参数类型>
    static decltype(auto) 形成参与包(
        特征数据操作类型& 数据操作,
        参数类型&&... 参数) {
        return 数据操作.形成特征批次同层参与包_(
            std::forward<参数类型>(参数)...);
    }

    template <typename 需求数据操作类型, typename 参与包类型, typename 具名候选形成回调类型>
    static decltype(auto) 执行参与包(
        需求数据操作类型& 数据操作,
        参与包类型& 参与包,
        具名候选形成回调类型&& 具名候选形成回调) {
        auto 特征候选形成回调 = [&参与包](auto& 会话) -> decltype(auto) {
            return 参与包.形成候选_(会话);
        };
        return 数据操作.执行特征批次同层参与包_(
            std::forward<具名候选形成回调类型>(具名候选形成回调),
            特征候选形成回调,
            参与包.原始材料参与者_,
            参与包.批次记录参与者_);
    }

    template <typename 参与包类型>
    static decltype(auto) 读取候选结果(const 参与包类型& 参与包) {
        return 参与包.读取候选结果_();
    }
};

}
