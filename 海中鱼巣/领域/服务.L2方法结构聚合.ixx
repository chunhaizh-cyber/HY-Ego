export module 海中鱼巣.领域.服务.L2方法结构聚合;

import 海中鱼巣.领域.服务.L2方法结构;

export namespace 海中鱼巣 {

class L2方法结构聚合服务 final {
public:
    // 诊断责任：无适用错误分支；只绑定调用方已经装配的唯一方法结构服务。
    explicit L2方法结构聚合服务(L2方法结构服务& 方法结构服务) noexcept
        : 方法结构服务_(方法结构服务) {}

    L2方法结构聚合服务() = delete;
    L2方法结构聚合服务(const L2方法结构聚合服务&) = delete;
    L2方法结构聚合服务& operator=(const L2方法结构聚合服务&) = delete;
    L2方法结构聚合服务(L2方法结构聚合服务&&) = delete;
    L2方法结构聚合服务& operator=(L2方法结构聚合服务&&) = delete;

    // 诊断责任：无适用错误分支；返回绑定的唯一方法结构服务同实例引用。
    L2方法结构服务& 取得L2方法结构服务() noexcept {
        return 方法结构服务_;
    }

    // 诊断责任：无适用错误分支；返回绑定的唯一方法结构服务同实例只读引用。
    const L2方法结构服务& 取得L2方法结构服务() const noexcept {
        return 方法结构服务_;
    }

private:
    L2方法结构服务& 方法结构服务_;
};

}
