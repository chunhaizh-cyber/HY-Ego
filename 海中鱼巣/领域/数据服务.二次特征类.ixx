module;

#include <cstdint>
#include <exception>
#include <new>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

export module 海中鱼巣.领域.数据服务.二次特征类;

export import 海中鱼巣.领域.数据服务.存在类;

export namespace 海中鱼巣 {

struct 二次特征节点生成请求 final {
    特征类派生定义新增请求 定义请求;
    friend bool operator==(const 二次特征节点生成请求&,
        const 二次特征节点生成请求&) = default;
};

struct 二次特征计算请求 final {
    特征类比较请求 比较请求;
    friend bool operator==(const 二次特征计算请求&,
        const 二次特征计算请求&) = default;
};

struct 二次特征概念材料 final {
    特征类定义身份 特征身份;
    std::uint32_t 实际阶次 = 0;
    std::vector<特征类派生来源事实> 直接来源;
    特征类派生规则 派生规则;
    特征类比较注册身份 比较注册;
    std::uint32_t 算法版本 = 0;
    std::optional<稳定编码> 宿主存在;
    std::uint64_t 事实截止代次 = 0;
    friend bool operator==(const 二次特征概念材料&,
        const 二次特征概念材料&) = default;
};

struct 二次特征概念材料结果 final {
    特征类派生数据状态 状态 = 特征类派生数据状态::入口拒绝;
    std::uint64_t 事实代次 = 0;
    std::optional<二次特征概念材料> 材料;
    bool 成功() const noexcept {
        return 状态 == 特征类派生数据状态::已读取
            && 事实代次 != 0 && 材料 && 材料->实际阶次 > 1
            && 材料->直接来源.size() == 2
            && 有效(材料->特征身份.值) && 有效(材料->派生规则.规则身份)
            && 材料->派生规则.规则版本 != 0
            && 有效(材料->比较注册.值) && 材料->算法版本 != 0
            && 材料->事实截止代次 == 事实代次;
    }
};

class 二次特征类数据服务 final {
public:
    二次特征类数据服务() = delete;
    二次特征类数据服务(const 二次特征类数据服务&) = delete;
    二次特征类数据服务& operator=(const 二次特征类数据服务&) = delete;
    二次特征类数据服务(二次特征类数据服务&&) = delete;
    二次特征类数据服务& operator=(二次特征类数据服务&&) = delete;

    二次特征类数据服务(const L1事实基座服务& 第一层服务,
        特征类数据服务& 特征服务,
        const 存在类数据服务& 存在服务)
        : 第一层服务_(第一层服务), 特征服务_(特征服务),
          存在服务_(存在服务) {}

    特征类派生定义结果 生成二次特征节点(
        const 二次特征节点生成请求& 请求) {
        const auto G0 = 读取当前代次();
        if (!G0 || *G0 != 请求.定义请求.期望事实代次)
            return 派生失败(特征类派生数据状态::事实代次漂移,
                G0.value_or(0));
        try {
            if (请求.定义请求.宿主存在) {
                const auto 宿主 = 存在服务_.查询存在({存在类数据合同版本,
                    *G0, *请求.定义请求.宿主存在});
                if (!(宿主.状态 == 存在类数据状态::已读取
                        && 宿主.成功() && 宿主.事实代次 == *G0
                        && 宿主.存在结点
                        && 宿主.存在结点->结点 == *请求.定义请求.宿主存在))
                    return 派生失败(映射宿主状态(宿主.状态),
                        宿主.事实代次);
            }
            auto 结果 = 特征服务_.新增派生特征定义(请求.定义请求);
            if (结果.成功() && (!结果.定义 || 结果.定义->实际阶次 <= 1))
                return 派生失败(特征类派生数据状态::内部不一致,
                    结果.事实代次);
            const auto 读后 = 读取当前代次();
            const auto 期望末代次 = 结果.成功() || 结果.状态
                    == 特征类派生数据状态::已可能发布
                ? 结果.事实代次 : *G0;
            if (!读后 || *读后 != 期望末代次)
                return 派生失败(特征类派生数据状态::事实代次漂移,
                    读后.value_or(0));
            return 结果;
        } catch (const std::bad_alloc&) {
            return 派生失败(特征类派生数据状态::资源失败);
        } catch (const std::length_error&) {
            return 派生失败(特征类派生数据状态::资源失败);
        } catch (...) {
            return 派生失败(特征类派生数据状态::内部不一致);
        }
    }

    特征类派生定义结果 查询二次特征节点(
        const 特征类派生定义查询请求& 请求) const {
        const auto G0 = 读取当前代次();
        if (!G0 || *G0 != 请求.期望事实代次)
            return 派生失败(特征类派生数据状态::事实代次漂移,
                G0.value_or(0));
        auto 结果 = 特征服务_.查询派生特征定义(请求);
        if (结果.成功() && (!结果.定义 || 结果.定义->实际阶次 <= 1))
            return 派生失败(特征类派生数据状态::内部不一致,
                结果.事实代次);
        const auto 读后 = 读取当前代次();
        if (!读后 || *读后 != *G0)
            return 派生失败(特征类派生数据状态::事实代次漂移,
                读后.value_or(0));
        return 结果;
    }

    特征类派生定义结果 退出二次特征节点(
        const 特征类派生定义退出请求& 请求) {
        const auto G0 = 读取当前代次();
        if (!G0 || *G0 != 请求.期望事实代次)
            return 派生失败(特征类派生数据状态::事实代次漂移,
                G0.value_or(0));
        auto 结果 = 特征服务_.退出派生特征定义(请求);
        if (结果.成功() && (!结果.定义 || 结果.定义->实际阶次 <= 1))
            return 派生失败(特征类派生数据状态::内部不一致,
                结果.事实代次);
        const auto 读后 = 读取当前代次();
        const auto 期望末代次 = 结果.成功() || 结果.状态
                == 特征类派生数据状态::已可能发布
            ? 结果.事实代次 : *G0;
        if (!读后 || *读后 != 期望末代次)
            return 派生失败(特征类派生数据状态::事实代次漂移,
                读后.value_or(0));
        return 结果;
    }

    特征类比较结果 计算二次特征(const 二次特征计算请求& 请求) const {
        const auto G0 = 读取当前代次();
        if (!G0 || *G0 != 请求.比较请求.期望事实代次)
            return 比较漂移失败(请求.比较请求.请求身份, G0.value_or(0));
        auto 结果 = 特征服务_.比较派生特征(请求.比较请求);
        const auto 读后 = 读取当前代次();
        if (!读后 || *读后 != *G0)
            return 比较漂移失败(请求.比较请求.请求身份,
                读后.value_or(0));
        return 结果;
    }

    二次特征概念材料结果 读取二次特征概念材料(
        const 特征类派生定义查询请求& 请求) const {
        const auto G0 = 读取当前代次();
        if (!G0 || *G0 != 请求.期望事实代次)
            return 概念失败(特征类派生数据状态::事实代次漂移,
                G0.value_or(0));
        try {
            const auto 读取 = 特征服务_.查询派生特征定义(请求);
            if (!读取.成功() || !读取.定义)
                return 概念失败(读取.状态, 读取.事实代次);
            const auto& 定义 = *读取.定义;
            if (定义.实际阶次 <= 1)
                return 概念失败(特征类派生数据状态::内部不一致,
                    读取.事实代次);
            二次特征概念材料 材料{定义.身份, 定义.实际阶次,
                定义.直接来源, 定义.派生规则, 定义.比较注册.身份,
                定义.比较注册.合同.算法版本, 定义.宿主存在, *G0};
            const auto 读后 = 读取当前代次();
            if (!读后 || *读后 != *G0)
                return 概念失败(特征类派生数据状态::事实代次漂移,
                    读后.value_or(0));
            二次特征概念材料结果 结果{
                特征类派生数据状态::已读取, *G0, std::move(材料)};
            return 结果.成功() ? 结果
                : 概念失败(特征类派生数据状态::内部不一致, *G0);
        } catch (const std::bad_alloc&) {
            return 概念失败(特征类派生数据状态::资源失败);
        } catch (const std::length_error&) {
            return 概念失败(特征类派生数据状态::资源失败);
        } catch (...) {
            return 概念失败(特征类派生数据状态::内部不一致);
        }
    }

private:
    std::optional<std::uint64_t> 读取当前代次() const noexcept {
        const auto 读取 = 第一层服务_.读取中性当前事实代次(
            {L1中性CRUD合同版本});
        if (读取.状态 != L1中性读取状态::成功
            || 读取.合同版本 != L1中性CRUD合同版本
            || 读取.事实代次 == 0)
            return std::nullopt;
        return 读取.事实代次;
    }

    static 特征类派生定义结果 派生失败(
        特征类派生数据状态 状态, std::uint64_t 事实代次 = 0) noexcept {
        return {状态, 特征类派生数据合同版本, 事实代次, std::nullopt};
    }

    static 二次特征概念材料结果 概念失败(
        特征类派生数据状态 状态, std::uint64_t 事实代次 = 0) noexcept {
        return {状态, 事实代次, std::nullopt};
    }

    static 特征类比较结果 比较漂移失败(
        std::uint64_t 请求身份, std::uint64_t 事实代次) noexcept {
        特征类比较结果 结果;
        结果.请求身份 = 请求身份;
        结果.状态 = 特征类比较状态::入口拒绝;
        结果.拒绝原因 = 特征类比较拒绝原因::输入版本失效;
        结果.事实代次 = 事实代次;
        return 结果;
    }

    static 特征类派生数据状态 映射宿主状态(
        存在类数据状态 状态) noexcept {
        switch (状态) {
        case 存在类数据状态::未找到:
            return 特征类派生数据状态::宿主未找到;
        case 存在类数据状态::目标已退出:
            return 特征类派生数据状态::宿主已退出;
        case 存在类数据状态::事实代次漂移:
            return 特征类派生数据状态::事实代次漂移;
        case 存在类数据状态::入口拒绝:
            return 特征类派生数据状态::入口拒绝;
        case 存在类数据状态::资源失败:
            return 特征类派生数据状态::资源失败;
        default:
            return 特征类派生数据状态::宿主读取失败;
        }
    }

    const L1事实基座服务& 第一层服务_;
    特征类数据服务& 特征服务_;
    const 存在类数据服务& 存在服务_;
};

} // namespace 海中鱼巣
