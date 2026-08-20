module;

#include <new>
#include <stdexcept>
#include <utility>

export module 海中鱼巣.领域.服务.L2任务方法路径结构;

import 海中鱼巣.领域.服务.L2方法结构;
import 海中鱼巣.领域.服务.L2任务结构;

export namespace 海中鱼巣 {

// 任务方法路径的合法结构入口：校验权威方法引用后，复用任务 owner 内的唯一原子承载。
class L2任务方法路径结构服务 final {
public:
    L2任务方法路径结构服务(
        const L2方法结构服务& 方法服务,
        L2任务方法路径写入端口&& 写入端口)
        : 方法服务_(方法服务), 写入端口_(std::move(写入端口)) {
        if (!写入端口_.有效())
            throw std::runtime_error("L2 task method path write port is invalid");
    }

    L2任务方法路径结构服务(const L2任务方法路径结构服务&) = delete;
    L2任务方法路径结构服务& operator=(
        const L2任务方法路径结构服务&) = delete;

    // 诊断责任：向上送出；同代读取当前方法并精确核对内容版本，零默认成功。
    L2新增任务方法路径结果 新增任务方法路径(
        L2新增任务方法路径请求 请求) noexcept;

private:
    const L2方法结构服务& 方法服务_;
    L2任务方法路径写入端口 写入端口_;
};

L2新增任务方法路径结果
L2任务方法路径结构服务::新增任务方法路径(
    L2新增任务方法路径请求 请求) noexcept {
    const auto 失败 = [](L2结构状态 状态,
        std::uint64_t 截止 = 0) noexcept {
        L2新增任务方法路径结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 = 状态;
        结果.结果头.事实截止代次 = 截止;
        return 结果;
    };
    if (!L2新增任务方法路径请求有效(请求))
        return 失败(L2结构状态::入口拒绝);
    try {
        L2普通方法读取请求 方法请求;
        方法请求.请求头 = 请求.请求头;
        方法请求.读取类别 = L2读取类别::当前;
        方法请求.方法 = 请求.来源方法;
        const auto 方法读取 = 方法服务_.读取普通方法(方法请求);
        if (!方法读取.成功() || !方法读取.方法)
            return 失败(方法读取.结果头.状态,
                方法读取.结果头.事实截止代次);
        if (方法读取.方法->方法 != 请求.来源方法
            || 方法读取.方法->内容版本.值
                != 请求.来源方法内容版本.值)
            return 失败(L2结构状态::引用冲突,
                方法读取.结果头.事实截止代次);
        return 写入端口_.写入已验证任务方法路径(std::move(请求));
    } catch (const std::bad_alloc&) {
        return 失败(L2结构状态::资源失败);
    } catch (const std::length_error&) {
        return 失败(L2结构状态::资源失败);
    } catch (...) {
        return 失败(L2结构状态::内部不一致);
    }
}

} // namespace 海中鱼巣
