// 文件规则：结构写入会话只封装一次独占许可内的窄仓库能力和精确写集，不承载领域业务裁决。
module;

#include "主信息仓库.h"
#include "节点仓库.h"
#include "关系仓库.h"
#include "索引仓库.h"

#include <cstdint>
#include <optional>
#include <thread>
#include <utility>
#include <vector>

export module 海中鱼巣.核心.会话.结构写入;

export namespace 海中鱼巣 {

class 结构写入执行器;

class 结构写入会话 final {
public:
    结构写入会话() = delete;
    结构写入会话(const 结构写入会话&) = delete;
    结构写入会话& operator=(const 结构写入会话&) = delete;
    结构写入会话(结构写入会话&&) = delete;
    结构写入会话& operator=(结构写入会话&&) = delete;
    ~结构写入会话() = default;

    带值结构写入结果<主信息句柄> 创建主信息候选() {
        带值结构写入结果<主信息句柄> 输出;
        if (!可继续写入()) return 输出;
        auto 结果 = 主信息_.结构化创建主信息未发布候选(令牌_);
        输出.操作 = 结果.操作;
        if (!结果.成功()) {
            记录失败(结果.操作);
            return 输出;
        }
        if (!结果.值.has_value()) {
            输出.操作.状态 = 结构写入状态::内部不一致;
            记录失败(输出.操作);
            return 输出;
        }
        auto 候选 = std::move(*结果.值);
        const auto 主信息 = 候选.读取主信息();
        try {
            主信息候选组_.emplace_back(std::move(候选));
        } catch (...) {
            if (候选.完整()) (void)主信息_.结构化撤销未发布候选(候选, 令牌_);
            输出.操作 = {结构写入状态::内部不一致, 主信息.主信息编号, 主信息.版本号, 0};
            记录失败(输出.操作);
            return 输出;
        }
        输出.值.emplace(主信息);
        return 输出;
    }

    带值结构写入结果<节点句柄> 创建节点候选(节点类型 类型, 主信息句柄 主信息) {
        带值结构写入结果<节点句柄> 输出;
        if (!可继续写入()) return 输出;
        auto 结果 = 节点_.结构化创建节点未发布候选(类型, 主信息, 令牌_);
        输出.操作 = 结果.操作;
        if (!结果.成功()) {
            记录失败(结果.操作);
            return 输出;
        }
        if (!结果.值.has_value()) {
            输出.操作.状态 = 结构写入状态::内部不一致;
            记录失败(输出.操作);
            return 输出;
        }
        auto 候选 = std::move(*结果.值);
        const auto 节点 = 候选.读取节点();
        try {
            节点候选组_.emplace_back(std::move(候选));
        } catch (...) {
            if (候选.完整()) (void)节点_.结构化撤销未发布候选(候选, 令牌_);
            输出.操作 = {结构写入状态::内部不一致, 节点.节点编号, 节点.版本号, 0};
            记录失败(输出.操作);
            return 输出;
        }
        输出.值.emplace(节点);
        return 输出;
    }

    带值结构写入结果<关系句柄> 创建关系(
        关系类型 类型,
        节点句柄 源节点,
        节点句柄 目标节点,
        std::int64_t 顺序号 = 0) {
        带值结构写入结果<关系句柄> 输出;
        if (!可继续写入()) return 输出;
        auto 结果 = 关系_.结构化创建关系(类型, 源节点, 目标节点, 顺序号, 令牌_);
        输出 = std::move(结果);
        if (!输出.成功()) {
            记录失败(输出.操作);
            return 输出;
        }
        if (!输出.操作.改变了结构()) return 输出;
        try {
            关系写集_.push_back({*输出.值, false});
        } catch (...) {
            (void)关系_.严格删除关系(*输出.值, 令牌_);
            输出.操作.状态 = 结构写入状态::内部不一致;
            记录失败(输出.操作);
        }
        return 输出;
    }

    结构写入结果 绑定主键(std::uint64_t 主键, 节点句柄 预期节点) {
        if (!可继续写入()) return {};
        auto 结果 = 索引_.结构化绑定主键(主键, 预期节点, 令牌_);
        if (!结果.成功()) {
            记录失败(结果);
            return 结果;
        }
        if (!结果.改变了结构()) return 结果;
        try {
            索引写集_.push_back({主键, 预期节点, false});
        } catch (...) {
            (void)索引_.严格删除主键(主键, 预期节点, 令牌_);
            结果.状态 = 结构写入状态::内部不一致;
            记录失败(结果);
        }
        return 结果;
    }

    结构写入结果 写入候选I64值(
        主信息句柄 主信息,
        std::uint64_t 槽位,
        std::int64_t 预期值) {
        结构写入结果 输出{结构写入状态::入口拒绝, 主信息.主信息编号, 主信息.版本号, 0};
        if (!可继续写入() || !句柄有效(主信息)) return 输出;

        bool 是本会话候选 = false;
        for (const auto& 记录 : 主信息候选组_) {
            if (记录.候选.完整() && 记录.候选.读取主信息() == 主信息) {
                是本会话候选 = true;
                break;
            }
        }
        if (!是本会话候选) return 输出;

        for (const auto& 记录 : 候选I64写集_) {
            if (记录.主信息 != 主信息 || 记录.槽位 != 槽位) continue;
            if (记录.预期值 != 预期值) return 输出;
            return {结构写入状态::幂等读回, 主信息.主信息编号, 主信息.版本号, 主信息.版本号};
        }

        if (!主信息_.写入I64值(主信息, 槽位, 预期值, 令牌_)) {
            输出.状态 = 结构写入状态::内部不一致;
            记录失败(输出);
            return 输出;
        }
        try {
            候选I64写集_.push_back({主信息, 槽位, 预期值, false});
        } catch (...) {
            输出.状态 = 结构写入状态::内部不一致;
            记录失败(输出);
            return 输出;
        }
        return {结构写入状态::已提交, 主信息.主信息编号, 主信息.版本号, 主信息.版本号};
    }

    bool 候选I64值匹配(
        主信息句柄 主信息,
        std::uint64_t 槽位,
        std::int64_t 预期值) {
        if (!可继续写入() || !句柄有效(主信息)) return false;
        for (auto& 记录 : 候选I64写集_) {
            if (记录.主信息 != 主信息 || 记录.槽位 != 槽位) continue;
            if (记录.预期值 != 预期值) return false;
            if (主信息_.读取I64值(主信息, 槽位, 令牌_) != std::optional<std::int64_t>{预期值}) {
                记录失败({结构写入状态::内部不一致, 主信息.主信息编号, 主信息.版本号, 0});
                return false;
            }
            记录.已读回 = true;
            return true;
        }
        return false;
    }

    std::optional<节点类型> 读取节点类型(节点句柄 节点) const {
        if (!可继续写入() || !句柄有效(节点)) return std::nullopt;
        const auto 记录 = 节点_.读取节点(节点, 令牌_);
        return 记录.has_value() ? std::optional<节点类型>{记录->类型} : std::nullopt;
    }

    std::optional<主信息句柄> 读取节点主信息(节点句柄 节点) const {
        if (!可继续写入() || !句柄有效(节点)) return std::nullopt;
        const auto 记录 = 节点_.读取节点(节点, 令牌_);
        return 记录.has_value() ? std::optional<主信息句柄>{记录->主信息} : std::nullopt;
    }

    std::optional<std::int64_t> 读取I64值(主信息句柄 主信息, std::uint64_t 槽位) const {
        if (!可继续写入() || !句柄有效(主信息)) return std::nullopt;
        return 主信息_.读取I64值(主信息, 槽位, 令牌_);
    }

    std::vector<关系记录> 读取关系记录组(节点句柄 源节点, 关系类型 类型) const {
        if (!可继续写入() || !句柄有效(源节点)) return {};
        return 关系_.获取关系记录组(源节点, 类型, 令牌_);
    }

    bool 主信息可读(主信息句柄 主信息) {
        if (!当前线程可访问()) return false;
        const bool 可读 = 主信息_.读取主信息(主信息, 令牌_).has_value();
        if (!可读) {
            记录失败({结构写入状态::内部不一致, 主信息.主信息编号, 主信息.版本号, 0});
            return false;
        }
        for (auto& 记录 : 主信息候选组_) if (记录.候选.读取主信息() == 主信息) 记录.已读回 = true;
        return true;
    }

    bool 节点可读(节点句柄 节点) {
        if (!当前线程可访问()) return false;
        const bool 可读 = 节点_.读取节点(节点, 令牌_).has_value();
        if (!可读) {
            记录失败({结构写入状态::内部不一致, 节点.节点编号, 节点.版本号, 0});
            return false;
        }
        for (auto& 记录 : 节点候选组_) if (记录.候选.读取节点() == 节点) 记录.已读回 = true;
        return true;
    }

    bool 关系可读(关系句柄 关系) {
        if (!当前线程可访问()) return false;
        const bool 可读 = 关系_.读取关系(关系, 令牌_).has_value();
        if (!可读) {
            记录失败({结构写入状态::内部不一致, 关系.关系编号, 关系.版本号, 0});
            return false;
        }
        for (auto& 记录 : 关系写集_) if (记录.关系 == 关系) 记录.已读回 = true;
        return true;
    }

    bool 主键绑定匹配(std::uint64_t 主键, 节点句柄 预期节点) {
        if (!当前线程可访问()) return false;
        const bool 匹配 = 索引_.按主键查节点(主键, 令牌_) == std::optional<节点句柄>{预期节点};
        if (!匹配) {
            记录失败({结构写入状态::内部不一致, 主键, 预期节点.版本号, 0});
            return false;
        }
        for (auto& 记录 : 索引写集_) {
            if (记录.主键 == 主键 && 记录.预期节点 == 预期节点) 记录.已读回 = true;
        }
        return true;
    }

    bool 请求提交() {
        if (!当前线程可访问() || 阶段_ != 阶段::写入中 || 首次失败_.has_value() || !写集已完整读回()
            || (决定_ != 决定::未决定 && 决定_ != 决定::提交)) return false;
        决定_ = 决定::提交;
        return true;
    }

    bool 请求撤销() {
        if (!当前线程可访问() || 阶段_ != 阶段::写入中
            || (决定_ != 决定::未决定 && 决定_ != 决定::撤销)) return false;
        决定_ = 决定::撤销;
        return true;
    }

private:
    friend class 结构写入执行器;

    enum class 阶段 : std::uint8_t { 写入中, 收口中, 已结束 };
    enum class 决定 : std::uint8_t { 未决定, 提交, 撤销 };

    struct 索引绑定记录 {
        std::uint64_t 主键 = 0;
        节点句柄 预期节点;
        bool 已读回 = false;
    };

    struct 候选I64写入记录 {
        主信息句柄 主信息;
        std::uint64_t 槽位 = 0;
        std::int64_t 预期值 = 0;
        bool 已读回 = false;
    };

    struct 主信息候选记录 {
        explicit 主信息候选记录(主信息未发布候选 值) : 候选(std::move(值)) {}
        主信息候选记录(const 主信息候选记录&) = delete;
        主信息候选记录& operator=(const 主信息候选记录&) = delete;
        主信息候选记录(主信息候选记录&&) noexcept = default;
        主信息候选记录& operator=(主信息候选记录&&) = delete;
        主信息未发布候选 候选;
        bool 已读回 = false;
    };

    struct 节点候选记录 {
        explicit 节点候选记录(节点未发布候选 值) : 候选(std::move(值)) {}
        节点候选记录(const 节点候选记录&) = delete;
        节点候选记录& operator=(const 节点候选记录&) = delete;
        节点候选记录(节点候选记录&&) noexcept = default;
        节点候选记录& operator=(节点候选记录&&) = delete;
        节点未发布候选 候选;
        bool 已读回 = false;
    };

    struct 关系写入记录 {
        关系句柄 关系;
        bool 已读回 = false;
    };

    结构写入会话(
        主信息仓库& 主信息,
        节点仓库& 节点,
        关系仓库& 关系,
        索引仓库& 索引,
        结构事务令牌 令牌)
        : 主信息_(主信息), 节点_(节点), 关系_(关系), 索引_(索引),
          令牌_(令牌), 所有线程_(std::this_thread::get_id()) {
    }

    bool 当前线程可访问() const {
        return 阶段_ != 阶段::已结束 && 所有线程_ == std::this_thread::get_id();
    }

    bool 可继续写入() const {
        return 当前线程可访问() && 阶段_ == 阶段::写入中
            && 决定_ == 决定::未决定 && !首次失败_.has_value();
    }

    void 记录失败(const 结构写入结果& 结果) {
        if (!首次失败_.has_value()) 首次失败_ = 结果;
    }

    bool 写集已完整读回() const {
        for (const auto& 记录 : 主信息候选组_) if (!记录.已读回) return false;
        for (const auto& 记录 : 节点候选组_) if (!记录.已读回) return false;
        for (const auto& 记录 : 关系写集_) if (!记录.已读回) return false;
        for (const auto& 记录 : 索引写集_) if (!记录.已读回) return false;
        for (const auto& 记录 : 候选I64写集_) if (!记录.已读回) return false;
        return true;
    }

    bool 已请求提交() const { return 决定_ == 决定::提交; }
    bool 已请求撤销() const { return 决定_ == 决定::撤销; }
    bool 有失败() const { return 首次失败_.has_value(); }
    结构写入结果 读取首次失败() const { return 首次失败_.value_or(结构写入结果{}); }

    结构写入结果 完成提交() {
        if (!当前线程可访问() || 阶段_ != 阶段::写入中 || !已请求提交() || 有失败()) {
            return {结构写入状态::内部不一致, 0, 0, 0};
        }
        阶段_ = 阶段::收口中;
        for (auto& 记录 : 节点候选组_) {
            const auto 结果 = 节点_.结构化确认未发布候选(记录.候选, 令牌_);
            if (!结果.成功()) {
                阶段_ = 阶段::已结束;
                return {结构写入状态::内部不一致, 结果.结构编号, 结果.请求版本, 结果.当前版本};
            }
        }
        for (auto& 记录 : 主信息候选组_) {
            const auto 结果 = 主信息_.结构化确认未发布候选(记录.候选, 令牌_);
            if (!结果.成功()) {
                阶段_ = 阶段::已结束;
                return {结构写入状态::内部不一致, 结果.结构编号, 结果.请求版本, 结果.当前版本};
            }
        }
        阶段_ = 阶段::已结束;
        return {结构写入状态::已提交, 0, 0, 0};
    }

    结构写入结果 完成撤销() {
        if (!当前线程可访问() || 阶段_ != 阶段::写入中) {
            return {结构写入状态::内部不一致, 0, 0, 0};
        }
        阶段_ = 阶段::收口中;
        bool 完整 = true;
        for (auto 位置 = 索引写集_.rbegin(); 位置 != 索引写集_.rend(); ++位置) {
            完整 = 索引_.严格删除主键(位置->主键, 位置->预期节点, 令牌_).成功() && 完整;
        }
        for (auto 位置 = 关系写集_.rbegin(); 位置 != 关系写集_.rend(); ++位置) {
            完整 = 关系_.严格删除关系(位置->关系, 令牌_).成功() && 完整;
        }
        for (auto 位置 = 节点候选组_.rbegin(); 位置 != 节点候选组_.rend(); ++位置) {
            完整 = 节点_.结构化撤销未发布候选(位置->候选, 令牌_).成功() && 完整;
        }
        for (auto 位置 = 主信息候选组_.rbegin(); 位置 != 主信息候选组_.rend(); ++位置) {
            完整 = 主信息_.结构化撤销未发布候选(位置->候选, 令牌_).成功() && 完整;
        }
        阶段_ = 阶段::已结束;
        return {完整 ? 结构写入状态::候选已撤销 : 结构写入状态::内部不一致, 0, 0, 0};
    }

    主信息仓库& 主信息_;
    节点仓库& 节点_;
    关系仓库& 关系_;
    索引仓库& 索引_;
    const 结构事务令牌 令牌_;
    const std::thread::id 所有线程_;
    阶段 阶段_ = 阶段::写入中;
    决定 决定_ = 决定::未决定;
    std::optional<结构写入结果> 首次失败_;
    std::vector<主信息候选记录> 主信息候选组_;
    std::vector<节点候选记录> 节点候选组_;
    std::vector<关系写入记录> 关系写集_;
    std::vector<索引绑定记录> 索引写集_;
    std::vector<候选I64写入记录> 候选I64写集_;
};

}
