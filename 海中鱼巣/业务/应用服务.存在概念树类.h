#pragma once

#include "../领域/数据服务.概念树类.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstddef>
#include <exception>
#include <mutex>
#include <limits>
#include <optional>
#include <set>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
namespace 海中鱼巣 {
struct 存在概念应用预算 final {
    概念树共享预算 数据;
    std::uint64_t 最大派发步骤=0;
    std::uint64_t 最大保留尝试=0;
    std::uint64_t 最大遍历节点=0;
    friend bool operator==(const 存在概念应用预算&,const 存在概念应用预算&)=default;
};
struct 存在概念场景限制 final {
    概念树场景引用 S{};
    概念树存在引用 E{};
    std::uint64_t 原场景H=0;
    稳定编码 成员关系{};
    friend bool operator==(const 存在概念场景限制&,const 存在概念场景限制&)=default;
};
enum class 存在概念业务状态 : std::uint8_t {
    完成=1,待继续,被后继覆盖,需显式治理,不支持,入口拒绝,数量预算不足,历史材料不可用,资源失败,内部不一致
};
enum class 存在概念阶段状态 : std::uint8_t {未派发=1,部分确认,先前阶段待核验,全部确认};

struct 存在概念特征见证 final {
    概念树概念身份 模板;
    概念树特征引用 实例;
    稳定编码 值事实{},成员关系{};
    friend bool operator==(const 存在概念特征见证&,const 存在概念特征见证&)=default;
};
struct 存在概念形成请求 final {
    std::uint32_t 版本=1;
    std::uint64_t Gread=0,H=0;
    概念树存在引用 E;
    std::vector<存在概念特征见证>见证;
    std::optional<概念树概念身份>显式目标;
    std::uint64_t 概念key=0,支持key=0;
    存在概念应用预算 预算;
    std::optional<存在概念场景限制>场景限制;
    friend bool operator==(const 存在概念形成请求&,const 存在概念形成请求&)=default;
};
struct 存在概念上位请求 final {
    std::uint32_t 版本=1;
    std::uint64_t Gread=0,H=0;
    概念树概念身份 下位;
    std::vector<概念树概念身份>保留模板;
    std::uint64_t 概念key=0,接边key=0;
    存在概念应用预算 预算;
    friend bool operator==(const 存在概念上位请求&,const 存在概念上位请求&)=default;
};
enum class 存在概念步骤阶段 :std::uint8_t {概念=1,支持,接边};
using 存在概念步骤请求=std::variant<概念树存在建立请求_v2,概念树应用写请求,概念树存在治理请求_v2>;
using 存在概念步骤回执=std::variant<概念树存在写入结果_v2,概念树应用写入结果>;
struct 存在概念步骤 final {
    存在概念步骤阶段 阶段=存在概念步骤阶段::概念;
    存在概念步骤请求 原请求;
    std::optional<std::uint64_t>已知首次H;
    std::optional<存在概念步骤回执>回执提示;
};
struct 存在概念继续材料 final {
    std::uint32_t 版本=1;
    std::variant<存在概念形成请求,存在概念上位请求>原业务;
    std::uint64_t 初始G=0;
    std::optional<概念树存在定义>冻结定义;
    std::optional<概念树概念身份>已定位EC,已定位父;
    std::vector<存在概念步骤>步骤,已确认未绑定旧尝试;
};
struct 存在概念继续请求 final {
    存在概念继续材料 材料;
    std::uint64_t Gread=0;
    存在概念应用预算 本次读取预算;
};
using 存在概念确认事实=std::variant<概念树应用定义事实,概念树支持事实,概念树直接上位事实>;
struct 存在概念操作结果 final {
    std::uint32_t 版本=1;
    存在概念业务状态 业务状态=存在概念业务状态::入口拒绝;
    std::uint64_t Gread=0,H=0;
    存在概念阶段状态 阶段=存在概念阶段状态::未派发;
    bool 可能已发布=false;
    std::vector<存在概念确认事实>已确认;
    std::optional<存在概念继续材料>继续;
    bool 成功()const noexcept;
};
class 存在概念树应用服务 final {
    概念树类数据服务 &concepts_;
    const 特征类数据服务 &features_;
    const 存在类数据服务 &existences_;
    // 仅保存已核验根的进程内引用；根事实仍由数据服务持有。
    const 概念树概念身份 root_;
    inline static 概念树概念身份 存在概念根节点{};
    inline static std::mutex 根节点互斥_;
    inline static const 概念树类数据服务 *根绑定服务_ = nullptr;
    inline static std::size_t 根绑定实例数_ = 0;

    void 绑定全局根() {
        std::lock_guard lock(根节点互斥_);
        if (!根绑定实例数_) {
            if (根绑定服务_ || 有效(存在概念根节点.值))
                throw std::logic_error("inconsistent concept root binding");
            存在概念根节点 = root_;
            根绑定服务_ = &concepts_;
            根绑定实例数_ = 1;
            return;
        }
        if (根绑定服务_ != &concepts_ || 存在概念根节点 != root_)
            throw std::invalid_argument("concept root is bound to another provider");
        if (根绑定实例数_ == std::numeric_limits<std::size_t>::max())
            throw std::length_error("concept root binding count exhausted");
        ++根绑定实例数_;
    }
    void 释放全局根() noexcept {
        std::lock_guard lock(根节点互斥_);
        if (!根绑定实例数_ || 根绑定服务_ != &concepts_ || 存在概念根节点 != root_)
            std::terminate();
        if (--根绑定实例数_ == 0) {
            存在概念根节点 = {};
            根绑定服务_ = nullptr;
        }
    }

    概念树存在定义 形成见证(const 存在概念形成请求&,std::uint64_t,const 概念树共享预算&)const;
    std::optional<概念树存在定义>上位见证(const 存在概念上位请求&,std::uint64_t,const 概念树共享预算&)const;
    概念树应用定义事实 定义(概念树概念身份,std::uint64_t,std::uint64_t,const 概念树共享预算&)const;
    std::optional<概念树应用定义事实>查找(const std::optional<概念树存在定义>&,std::uint64_t,std::uint64_t,const 概念树共享预算&)const;
    void 核验材料(const 存在概念继续材料&,std::uint64_t,const 存在概念应用预算&)const;
    存在概念操作结果 推进(存在概念继续材料,std::uint64_t,const 存在概念应用预算&,const 场景类数据服务*);
public:
    static std::optional<概念树概念身份>读取存在概念根节点(){std::lock_guard lock(根节点互斥_);if(!根绑定实例数_)return std::nullopt;return 存在概念根节点;}
    ~存在概念树应用服务()noexcept{释放全局根();}
    存在概念树应用服务()=delete;
    存在概念树应用服务(const 存在概念树应用服务&)=delete;
    存在概念树应用服务&operator=(const 存在概念树应用服务&)=delete;
    存在概念树应用服务(存在概念树应用服务&&)=delete;
    存在概念树应用服务&operator=(存在概念树应用服务&&)=delete;
    存在概念树应用服务(概念树类数据服务&,const 特征类数据服务&,const 存在类数据服务&);
    bool 使用存在服务(const 存在类数据服务&e)const noexcept{return &e==&existences_;}
    bool 使用概念服务(const 概念树类数据服务&c)const noexcept{return &c==&concepts_;}
    存在概念操作结果 形成或复用存在概念(const 存在概念形成请求&);
    存在概念操作结果 形成上位存在概念(const 存在概念上位请求&);
    存在概念操作结果 继续存在概念操作(const 存在概念继续请求&);
    存在概念操作结果 形成或复用存在概念(const 存在概念形成请求&,const 存在概念场景限制&,const 场景类数据服务&);
    存在概念操作结果 继续存在概念操作(const 存在概念继续请求&,const 存在概念场景限制&,const 场景类数据服务&);
    概念树应用读取结果<概念树应用图事实>读取存在概念树(const 概念树应用图读取请求&)const;
    概念树应用写入结果 退出存在支持(const 概念树应用写请求&);
    概念树存在写入结果_v2 迁移存在概念(const 概念树存在治理请求_v2&);
    概念树存在写入结果_v2 退出存在概念(const 概念树存在治理请求_v2&);
    概念树存在写入结果_v2 释放存在抽象依据(const 概念树存在依据释放请求_v2&);
};
} // namespace 海中鱼巣
