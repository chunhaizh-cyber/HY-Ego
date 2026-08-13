export module 海中鱼巣.领域.服务.L2结构聚合;

import 海中鱼巣.领域.服务.L2场景结构;
import 海中鱼巣.领域.服务.L2存在结构;
import 海中鱼巣.领域.服务.L2特征结构;
import 海中鱼巣.领域.服务.L2状态结构;
import 海中鱼巣.领域.服务.L2动态结构;
import 海中鱼巣.领域.服务.L2因果结构;

export namespace 海中鱼巣 {

class L2结构聚合服务 final {
public:
    explicit L2结构聚合服务(
        L2场景结构服务& 场景服务,
        L2存在结构服务& 存在服务,
        L2特征结构服务& 特征服务,
        L2状态结构服务& 状态服务,
        L2动态结构服务& 动态服务,
        L2因果结构服务& 因果服务) noexcept
        : 场景服务_(场景服务), 存在服务_(存在服务), 特征服务_(特征服务),
          状态服务_(状态服务), 动态服务_(动态服务), 因果服务_(因果服务) {}

    L2结构聚合服务() = delete;
    L2结构聚合服务(const L2结构聚合服务&) = delete;
    L2结构聚合服务& operator=(const L2结构聚合服务&) = delete;
    L2结构聚合服务(L2结构聚合服务&&) = delete;
    L2结构聚合服务& operator=(L2结构聚合服务&&) = delete;

    L2场景结构服务& 取得L2场景结构服务() noexcept { return 场景服务_; }
    const L2场景结构服务& 取得L2场景结构服务() const noexcept { return 场景服务_; }
    L2存在结构服务& 取得L2存在结构服务() noexcept { return 存在服务_; }
    const L2存在结构服务& 取得L2存在结构服务() const noexcept { return 存在服务_; }
    L2特征结构服务& 取得L2特征结构服务() noexcept { return 特征服务_; }
    const L2特征结构服务& 取得L2特征结构服务() const noexcept { return 特征服务_; }
    L2状态结构服务& 取得L2状态结构服务() noexcept { return 状态服务_; }
    const L2状态结构服务& 取得L2状态结构服务() const noexcept { return 状态服务_; }
    L2动态结构服务& 取得L2动态结构服务() noexcept { return 动态服务_; }
    const L2动态结构服务& 取得L2动态结构服务() const noexcept { return 动态服务_; }
    L2因果结构服务& 取得L2因果结构服务() noexcept { return 因果服务_; }
    const L2因果结构服务& 取得L2因果结构服务() const noexcept { return 因果服务_; }

private:
    L2场景结构服务& 场景服务_;
    L2存在结构服务& 存在服务_;
    L2特征结构服务& 特征服务_;
    L2状态结构服务& 状态服务_;
    L2动态结构服务& 动态服务_;
    L2因果结构服务& 因果服务_;
};

}
