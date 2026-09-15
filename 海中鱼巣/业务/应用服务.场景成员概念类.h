#pragma once

#include "应用服务.特征概念类.h"
#include <algorithm>
#include <cstdint>
#include <new>
#include <optional>
#include <stdexcept>
#include <utility>
#include <variant>
#include <vector>

#include "应用服务.存在概念树类.h"

namespace 海中鱼巣 {
struct 存在当前采用意图_v2 final {
    std::uint32_t 版本=2;
    L1所有者范围写入幂等身份 幂等身份{};
    std::optional<存在当前采用事实> 原采用预期;
    std::uint64_t 关系预算=0;
    friend bool operator==(const 存在当前采用意图_v2&,const 存在当前采用意图_v2&)=default;
};
struct 场景成员概念请求_v2 final {
    std::uint32_t 版本=2;
    概念树场景引用 S{};
    概念树存在引用 E{};
    std::uint64_t Gread=0;
    std::uint64_t H=0;
    特征概念观察请求 观察;
    std::optional<存在当前采用意图_v2> 采用;
    std::uint64_t 场景关系预算=0;
    稳定编码 场景成员关系{};
    friend bool operator==(const 场景成员概念请求_v2&,const 场景成员概念请求_v2&)=default;
};
enum class 场景成员概念继续阶段_v2 : std::uint8_t {
    特征应用=1,存在采用=2,最终读回=3
};
struct 场景成员概念继续材料_v2 final {
    std::uint32_t 版本=2;
    场景成员概念继续阶段_v2 阶段=场景成员概念继续阶段_v2::特征应用;
    场景成员概念请求_v2 原请求;
    std::optional<存在当前采用写请求> 采用原请求;
    std::optional<定位特征结果> 已确认定位;
    friend bool operator==(const 场景成员概念继续材料_v2& a,const 场景成员概念继续材料_v2& b) {
        if(a.版本!=b.版本||a.阶段!=b.阶段||a.原请求!=b.原请求||a.采用原请求!=b.采用原请求||a.已确认定位.has_value()!=b.已确认定位.has_value())return false;
        if(!a.已确认定位)return true;const auto& x=*a.已确认定位;const auto& y=*b.已确认定位;
        return x.版本==y.版本&&x.状态==y.状态&&x.Gread==y.Gread&&x.首次发布H==y.首次发布H&&x.事实==y.事实&&x.候选F==y.候选F&&x.原请求==y.原请求;
    }
};
struct 场景特征概念结果_v2 final {
    std::uint32_t 版本=2;
    std::uint64_t Gread=0;
    std::uint64_t H=0;
    特征概念处理结果 处理;
    std::optional<准确特征读取事实> 已确认F;
    std::optional<特征概念事实> 已确认C;
    std::optional<特征名称事实> 已确认名称;
    bool 成功() const noexcept;
};
struct 场景组合投影_v2 final {
    std::uint32_t 版本=2;
    std::uint64_t Gread=0;
    std::uint64_t H=0;
    概念树场景引用 S{};
    概念树存在引用 E{};
    场景直接包含事实 场景成员;
    准确特征读取事实 F;
    特征概念事实 C;
    特征名称事实 名称;
    std::optional<存在当前采用事实> 当前采用;
    定位特征事实 定位;
    bool 完整() const noexcept;
};
enum class 场景成员概念状态_v2 : std::uint8_t {
    完成=1,需选择准确F,待特征应用收敛,待存在采用收敛,
    准确F已成立名称未完成,当前采用未完成,入口拒绝,未找到,规则缺失,
    类型不相容,引用冲突,多义,事实代次漂移,历史材料不可用,数量预算不足,
    幂等冲突,旧格式不支持,不支持,资源失败,内部不一致,已可能发布,
    定位拒绝=22,定位待原请求确认=23,准确F已定位概念未完成=24
};
struct 场景成员概念结果_v2 final {
    std::uint32_t 版本=2;
    场景成员概念状态_v2 状态=场景成员概念状态_v2::入口拒绝;
    std::uint64_t Gread=0;
    std::uint64_t H=0;
    概念树场景引用 S{};
    概念树存在引用 E{};
    场景特征概念结果_v2 特征概念;
    std::optional<存在当前采用结果> 当前采用;
    std::optional<场景成员概念继续材料_v2> 继续;
    std::optional<场景组合投影_v2> 最终投影;
    bool 成功() const noexcept;
};
class 场景成员概念应用服务 final {
    概念树类数据服务& concepts_;
    const 场景类数据服务& scenes_;
    存在类数据服务& existences_;
    特征概念应用服务& features_;
    存在概念树应用服务& existenceConcepts_;
    void 验证入口(const 场景成员概念请求_v2&) const;
    场景直接包含事实 读取成员(const 场景成员概念请求_v2&,std::uint64_t,std::uint64_t) const;
    void 补全特征(场景特征概念结果_v2&,const 场景成员概念请求_v2&,std::uint64_t) const;
    void 完成投影(场景成员概念结果_v2&,const 场景成员概念请求_v2&) const;
    void 处理采用(场景成员概念结果_v2&,const 场景成员概念请求_v2&);
    bool 消费特征结果(场景成员概念结果_v2&,const 场景成员概念请求_v2&,特征概念处理结果,
        const 场景成员概念继续材料_v2* =nullptr);
public:
    场景成员概念应用服务(概念树类数据服务& c,const 场景类数据服务& s,存在类数据服务& e,
        特征概念应用服务& f,存在概念树应用服务& ec)
        :concepts_(c),scenes_(s),existences_(e),features_(f),existenceConcepts_(ec) {
        if(!c.使用同一场景基座(s)||!s.使用存在提供者(e)||!ec.使用存在服务(e)
            ||!f.使用概念服务(c)||!ec.使用概念服务(c)||!f.使用定位参与者(e,s))
            throw std::invalid_argument("scene concept bindings");
    }
    场景成员概念结果_v2 处理场景成员概念(const 场景成员概念请求_v2&);
    场景成员概念结果_v2 收敛场景成员概念(const 场景成员概念继续材料_v2&);
};
}
