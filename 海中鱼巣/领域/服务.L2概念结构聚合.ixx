export module 海中鱼巣.领域.服务.L2概念结构聚合;

import 海中鱼巣.领域.服务.L2概念结构;

export namespace 海中鱼巣 {

class L2概念结构聚合服务 final {
public:
    // 诊断责任：无适用错误分支；只绑定调用方已经装配的唯一概念结构服务。
    explicit L2概念结构聚合服务(L2概念结构服务& 概念结构服务) noexcept
        : 概念结构服务_(概念结构服务) {}

    L2概念结构聚合服务() = delete;
    L2概念结构聚合服务(const L2概念结构聚合服务&) = delete;
    L2概念结构聚合服务& operator=(const L2概念结构聚合服务&) = delete;
    L2概念结构聚合服务(L2概念结构聚合服务&&) = delete;
    L2概念结构聚合服务& operator=(L2概念结构聚合服务&&) = delete;

    // 诊断责任：无适用错误分支；返回绑定的唯一概念结构服务同实例引用。
    L2概念结构服务& 取得L2概念结构服务() noexcept {
        return 概念结构服务_;
    }

    // 诊断责任：无适用错误分支；返回绑定的唯一概念结构服务同实例只读引用。
    const L2概念结构服务& 取得L2概念结构服务() const noexcept {
        return 概念结构服务_;
    }

private:
    L2概念结构服务& 概念结构服务_;
};

}
