module;

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

export module 海中鱼巣.领域.服务.L1完整自我存在准入;

export import 海中鱼巣.核心.合同.L1中性CRUD;
export import 海中鱼巣.领域.服务.世界登记;
export import 海中鱼巣.领域.服务.L1实际存在;
export import 海中鱼巣.领域.服务.L1自我结构登记;

export {
#define L1_COMPLETE_SELF_NO_INCLUDES
#define L1完整自我数据已导入中性CRUD
#define L1完整自我数据已导入世界登记
#define L1完整自我数据已导入场景结构
#define L1完整自我数据已导入实际存在
#define L1完整自我数据已导入自我结构登记
#include "L1完整自我.数据.h"
#undef L1完整自我数据已导入自我结构登记
#undef L1完整自我数据已导入实际存在
#undef L1完整自我数据已导入场景结构
#undef L1完整自我数据已导入世界登记
#undef L1完整自我数据已导入中性CRUD
#undef L1_COMPLETE_SELF_NO_INCLUDES
}

namespace 海中鱼巣::完整自我存在准入内部 {

// 诊断责任：无适用错误分支；纯值身份碰撞判断。
bool 身份互异(const 完整自我存在准入请求& 请求) noexcept {
    const std::array<稳定编码, 9> 身份组{
        请求.世界登记.世界根场景,
        请求.世界登记.服务身份,
        请求.世界登记.场景标记属性类型,
        请求.世界登记.直接父场景关系类型,
        请求.世界登记.直接成员关系类型,
        请求.实际存在规格.服务身份.编码,
        请求.实际存在规格.资格属性类型.编码,
        请求.自我结构登记.服务身份,
        请求.自我结构登记.自我结构关系类型};
    for (std::size_t i = 0; i < 身份组.size(); ++i) {
        for (std::size_t j = i + 1; j < 身份组.size(); ++j)
            if (身份组[i] == 身份组[j]) return false;
    }
    return true;
}

} // namespace 海中鱼巣::完整自我存在准入内部

export namespace 海中鱼巣 {

class L1完整自我存在准入服务 final {
public:
    L1完整自我存在准入服务() = default;
    L1完整自我存在准入服务(const L1完整自我存在准入服务&) = delete;
    L1完整自我存在准入服务& operator=(const L1完整自我存在准入服务&) = delete;
    L1完整自我存在准入服务(L1完整自我存在准入服务&&) = delete;
    L1完整自我存在准入服务& operator=(L1完整自我存在准入服务&&) = delete;

    // 诊断责任：向上送出；所有拒绝与矛盾均以结构化结果返回组合服务。
    完整自我存在准入结果 形成完整自我存在准入(
        const 完整自我存在准入请求& 请求) const noexcept {
        if (!完整自我存在准入请求有效(请求)) return {};
        if (请求.世界登记.已验证事实代次 != 请求.共同事实截止代次
            || 请求.实际存在规格.事实截止代次 != 请求.共同事实截止代次
            || 请求.自我结构登记.事实截止代次 != 请求.共同事实截止代次) {
            return {完整自我存在准入状态::事实代次漂移, std::nullopt};
        }
        if (!完整自我存在准入内部::身份互异(请求)) {
            return {完整自我存在准入状态::内部不一致, std::nullopt};
        }

        完整自我存在准入规格 规格;
        规格.共同事实截止代次 = 请求.共同事实截止代次;
        规格.世界根 = {请求.世界登记.世界根场景, 节点种类::普通};
        规格.实际存在资格 = 请求.实际存在规格;
        规格.自我结构登记服务 = {
            请求.自我结构登记.服务身份, 节点种类::普通};
        规格.自我结构关系类型 = {
            请求.自我结构登记.自我结构关系类型, 节点种类::普通};
        if (!完整自我存在准入规格完整(规格)) {
            return {完整自我存在准入状态::内部不一致, std::nullopt};
        }
        return {完整自我存在准入状态::已批准, 规格};
    }
};

} // namespace 海中鱼巣
