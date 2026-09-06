module;

#include <cstdint>
#include <new>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

export module 海中鱼巣.领域.数据服务.二次特征类;
export import 海中鱼巣.领域.数据服务.存在类;

export namespace 海中鱼巣 {

struct 二次标量特征生成请求 final {
    特征类标量派生建立请求 定义请求;
};
struct 二次标量特征计算请求 final {
    特征类标量比较请求 比较请求;
};
struct 二次标量特征概念材料结果 final {
    std::uint32_t 版本 = 标量派生合同版本;
    特征类标量状态 状态 = 特征类标量状态::入口拒绝;
    std::uint64_t Gread = 0, H = 0;
    std::optional<特征类标量派生事实> 定义事实;
    std::vector<特征类标量叶回执> 基础叶组;
    std::vector<稳定编码> 左叶组, 右叶组;
    bool 成功() const noexcept {
        return 版本 == 2 && 状态 == 特征类标量状态::已读取 && H && Gread >= H
            && 定义事实 && 定义事实->完整(H)
            && !基础叶组.empty() && !左叶组.empty() && !右叶组.empty();
    }
};

class 二次特征类数据服务 final {
    using S = 特征类标量状态;
    using P = 特征类标量发布确定性;
public:
    二次特征类数据服务() = delete;
    二次特征类数据服务(const 二次特征类数据服务&) = delete;
    二次特征类数据服务& operator=(const 二次特征类数据服务&) = delete;
    二次特征类数据服务(二次特征类数据服务&&) = delete;
    二次特征类数据服务& operator=(二次特征类数据服务&&) = delete;

    二次特征类数据服务(const L1事实基座服务& l1, 特征类数据服务& feature,
        const 存在类数据服务& existence)
        : 第一层服务_(l1), 特征服务_(feature), 存在服务_(existence) {
        if (!feature.绑定于(l1) || !existence.绑定于(l1))
            throw std::invalid_argument("scalar providers must share L1");
    }

    特征类标量派生写结果 生成二次特征节点(const 二次标量特征生成请求& r) {
        return 包装写(r.定义请求);
    }
    特征类标量派生写结果 退出二次特征节点(const 特征类标量派生退出请求& r) {
        return 包装写(r);
    }
    特征类标量派生读取结果 查询二次特征节点(const 特征类标量派生读取请求& r) const {
        auto out = 特征服务_.读取标量派生定义(r);
        if (!out.成功()) return out;
        S 代次失败 = S::内部不一致;
        const auto g = 当前G(代次失败);
        if (!g || *g != r.Gread || out.Gread != r.Gread || out.H != r.H
            || out.定义事实->定义身份 != r.定义身份) {
            特征类标量派生读取结果 fail;
            fail.状态 = !g ? 代次失败 : *g != r.Gread ? S::事实代次漂移 : S::内部不一致;
            fail.Gread = g.value_or(0); fail.H = r.H; return fail;
        }
        return out;
    }
    特征类标量比较结果 计算二次特征(const 二次标量特征计算请求& r) const {
        auto out = 特征服务_.比较派生特征(r.比较请求);
        if (!out.成功()) return out;
        const auto& q = r.比较请求;
        S 代次失败 = S::内部不一致;
        const auto g = 当前G(代次失败);
        if (!g || *g != q.G || out.G != q.G || out.请求身份 != q.请求身份
            || out.根定义 != q.根定义 || out.实际结果位 != q.要求结果位) {
            特征类标量比较结果 fail;
            fail.状态 = !g ? 代次失败 : *g != q.G ? S::事实代次漂移 : S::内部不一致;
            fail.拒绝原因 = fail.状态; fail.G = g.value_or(0); fail.请求身份 = q.请求身份; return fail;
        }
        return out;
    }
    二次标量特征概念材料结果 读取二次特征概念材料(const 特征类标量派生读取请求& r) const {
        auto read = 查询二次特征节点(r);
        二次标量特征概念材料结果 out;
        out.状态 = read.状态; out.Gread = read.Gread; out.H = read.H;
        if (read.成功()) {
            out.定义事实 = std::move(read.定义事实);
            out.基础叶组 = std::move(read.基础叶组);
            out.左叶组 = std::move(read.左叶组);
            out.右叶组 = std::move(read.右叶组);
        }
        return out;
    }
private:
    std::optional<std::uint64_t> 当前G(S& error) const noexcept {
        try {
            const auto r = 第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});
            if (r.状态 == L1中性读取状态::成功 && r.合同版本 == L1中性CRUD合同版本 && r.事实代次)
                return r.事实代次;
            error = r.状态 == L1中性读取状态::资源失败 ? S::资源失败 : S::内部不一致;
        } catch (const std::bad_alloc&) { error = S::资源失败; }
        catch (const std::length_error&) { error = S::资源失败; }
        catch (...) { error = S::内部不一致; }
        return std::nullopt;
    }
    static S 宿主状态(存在类数据状态 s) noexcept {
        switch (s) {
        case 存在类数据状态::未找到: return S::未找到;
        case 存在类数据状态::目标已退出: return S::已退出;
        case 存在类数据状态::事实代次漂移: return S::事实代次漂移;
        case 存在类数据状态::资源失败: return S::资源失败;
        case 存在类数据状态::入口拒绝: return S::入口拒绝;
        default: return S::内部不一致;
        }
    }
    template<class R> 特征类标量派生写结果 包装写(const R& r) {
        特征类标量派生写结果 out;
        bool dispatched = false;
        auto fail = [&](S s) {
            out.定义事实.reset();
            if (dispatched || out.首次发布H || out.发布确定性 == P::可能已发布
                || out.发布确定性 == P::确认已发布) {
                out.状态 = S::已可能发布; out.发布确定性 = P::可能已发布;
            } else out.状态 = s;
            return std::move(out);
        };
        try {
            S 代次失败 = S::内部不一致;
            const auto g = 当前G(代次失败);
            if (!g) return fail(代次失败);
            out.Gread = *g;
            if constexpr (std::is_same_v<R, 特征类标量派生建立请求>) {
                // 原 G 已过去时只让 owner 做原键裁决，不用今天 E 的活动性阻断历史恢复。
                if (*g == r.G && r.宿主E) {
                    const auto e = 存在服务_.查询存在({存在类数据合同版本, r.G, *r.宿主E});
                    if (!e.成功() || e.状态 != 存在类数据状态::已读取)
                        return fail(宿主状态(e.状态));
                    if (e.事实代次 != r.G || !e.存在结点 || e.存在结点->结点 != *r.宿主E)
                        return fail(S::内部不一致);
                }
            }
            dispatched = true;
            if constexpr (std::is_same_v<R, 特征类标量派生建立请求>) out = 特征服务_.建立标量派生定义(r);
            else out = 特征服务_.退出标量派生定义(r);
            // 返回失败也保留 owner 已知的首次 H 和正式 L1 回执。
            if (!out.成功()) return out;
            const auto tail = 当前G(代次失败);
            if (!tail || *tail != out.Gread) return fail(!tail ? 代次失败 : S::事实代次漂移);
            return out;
        } catch (const std::bad_alloc&) { return fail(S::资源失败); }
        catch (const std::length_error&) { return fail(S::资源失败); }
        catch (...) { return fail(S::内部不一致); }
    }
    const L1事实基座服务& 第一层服务_;
    特征类数据服务& 特征服务_;
    const 存在类数据服务& 存在服务_;
};
} // namespace 海中鱼巣
