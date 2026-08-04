module;

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <variant>
#include <vector>

export module 海中鱼巣.领域.自检.世界登记;

import 海中鱼巣.领域.服务.世界登记;

export namespace 海中鱼巣 {

struct 世界登记自检报告 final {
    std::vector<bool> 验收;
    std::size_t 失败数量 = 0;
    bool 总通过 = false;
};

} // namespace 海中鱼巣

namespace 海中鱼巣::世界登记自检内部 {

世界登记建立请求 请求(std::uint64_t 键) {
    世界登记建立请求 结果;
    结果.幂等键 = {键};
    return 结果;
}

bool 项目1() {
    L1事实基座服务 L1;
    世界登记服务 服务(L1);
    const auto 登记 = 服务.读取当前登记();
    const auto 快照 = L1.读取完整快照();
    return 登记.状态 == 世界登记状态::登记未加载
        && !登记.登记.has_value() && 快照.状态 == L1读取状态::成功
        && 快照.快照.has_value() && 快照.快照->事实代次 == 0
        && 快照.快照->当前节点.empty()
        && 快照.快照->当前关系.empty()
        && 快照.快照->当前值.empty()
        && 快照.快照->永久占用编码.empty();
}

bool 项目2() {
    L1事实基座服务 L1;
    世界登记服务 服务(L1);
    auto 合同 = 请求(1); 合同.合同版本 = 2;
    auto 规则 = 请求(2); 规则.世界规则版本 = 2;
    const auto A = 服务.建立空世界(合同);
    const auto B = 服务.建立空世界(规则);
    const auto C = 服务.建立空世界({});
    const auto 快照 = L1.读取完整快照();
    return A.状态 == 世界登记状态::入口拒绝
        && B.状态 == 世界登记状态::入口拒绝
        && C.状态 == 世界登记状态::入口拒绝
        && !A.登记.has_value() && !B.登记.has_value()
        && !C.登记.has_value() && 快照.快照.has_value()
        && 快照.快照->事实代次 == 0
        && 快照.快照->当前节点.empty();
}

bool 项目3() {
    L1事实基座服务 L1;
    世界登记服务 服务(L1);
    const auto 结果 = 服务.建立空世界(请求(3));
    return 结果.状态 == 世界登记状态::已提交
        && 结果.登记.has_value()
        && 结果.登记->合同版本 == 1
        && 结果.登记->世界规则版本 == 1
        && 有效(结果.登记->首次幂等键)
        && 有效(结果.登记->服务身份)
        && 有效(结果.登记->世界根场景)
        && 结果.登记->已验证事实代次 == 1;
}

bool 项目4() {
    L1事实基座服务 L1;
    世界登记服务 服务(L1);
    if (!服务.建立空世界(请求(4)).登记.has_value()) return false;
    const auto 结果 = L1.读取完整快照();
    return 结果.状态 == L1读取状态::成功 && 结果.快照.has_value()
        && 结果.快照->事实代次 == 1
        && 结果.快照->当前节点.size() == 5
        && 结果.快照->当前关系.empty()
        && 结果.快照->当前值.size() == 1
        && 结果.快照->永久占用编码.size() == 6;
}

bool 项目5() {
    L1事实基座服务 L1;
    世界登记服务 服务(L1);
    const auto 登记结果 = 服务.建立空世界(请求(5));
    const auto 快照结果 = L1.读取完整快照();
    if (!登记结果.登记.has_value() || !快照结果.快照.has_value()
        || 快照结果.快照->当前值.size() != 1) return false;
    const auto& 登记 = *登记结果.登记;
    const auto& 值 = 快照结果.快照->当前值[0];
    const 节点事实* 根 = nullptr;
    const 节点事实* 类型 = nullptr;
    for (const auto& 节点 : 快照结果.快照->当前节点) {
        if (节点.编码 == 登记.世界根场景) 根 = &节点;
        if (节点.编码 == 登记.场景标记属性类型) 类型 = &节点;
    }
    return 根 != nullptr && 类型 != nullptr && 根->当前属性.size() == 1
        && 根->当前属性[0].属性类型节点 == 登记.场景标记属性类型
        && 根->当前属性[0].当前值 == 值.编码
        && 类型->种类 == 节点种类::属性类型
        && 类型->属性类型表示.has_value()
        && *类型->属性类型表示 == 值表示种类::I64
        && 值.所属节点 == 登记.世界根场景
        && 值.属性类型节点 == 登记.场景标记属性类型
        && 值.来源节点 == 登记.服务身份
        && 值.创建事实代次 == 登记.已验证事实代次
        && !值.退出事实代次.has_value()
        && std::holds_alternative<std::int64_t>(值.材料)
        && std::get<std::int64_t>(值.材料) == 1;
}

bool 项目6() {
    L1事实基座服务 L1;
    世界登记服务 服务(L1);
    const auto 首次 = 服务.建立空世界(请求(6));
    const auto 读回 = 服务.读取当前登记();
    if (!首次.登记.has_value() || !读回.登记.has_value()) return false;
    const auto& A = *首次.登记;
    const auto& B = *读回.登记;
    return 读回.状态 == 世界登记状态::已读取
        && A.首次幂等键 == B.首次幂等键
        && A.已验证事实代次 == B.已验证事实代次
        && A.世界根场景 == B.世界根场景
        && A.服务身份 == B.服务身份;
}

bool 项目7() {
    L1事实基座服务 L1;
    世界登记服务 服务(L1);
    const auto 请求值 = 请求(7);
    const auto 首次 = 服务.建立空世界(请求值);
    const auto 前 = L1.读取完整快照();
    const auto 重复 = 服务.建立空世界(请求值);
    const auto 后 = L1.读取完整快照();
    return 首次.登记.has_value() && 重复.状态 == 世界登记状态::幂等读回
        && 重复.登记.has_value() && 前.快照.has_value() && 后.快照.has_value()
        && 前.快照->事实代次 == 后.快照->事实代次
        && 前.快照->当前节点.size() == 后.快照->当前节点.size();
}

bool 项目8() {
    L1事实基座服务 L1;
    世界登记服务 服务(L1);
    const auto 请求值 = 请求(8);
    (void)服务.建立空世界(请求值);
    auto 异义 = 请求值; 异义.期望事实代次 = 1;
    const auto 前 = L1.读取完整快照();
    const auto 结果 = 服务.建立空世界(异义);
    const auto 后 = L1.读取完整快照();
    return 结果.状态 == 世界登记状态::幂等冲突
        && !结果.登记.has_value() && 前.快照.has_value() && 后.快照.has_value()
        && 前.快照->事实代次 == 后.快照->事实代次;
}

bool 项目9() {
    L1事实基座服务 L1;
    L1写集请求 写集;
    写集.幂等键 = {9};
    写集.节点 = {{{1}, 节点种类::普通, std::nullopt}};
    const auto 写入 = L1.提交写集(写集);
    世界登记服务 服务(L1);
    const auto 前 = L1.读取完整快照();
    const auto 结果 = 服务.建立空世界(请求(10));
    const auto 后 = L1.读取完整快照();
    return 写入.状态 == L1写入状态::成功
        && 结果.状态 == 世界登记状态::入口拒绝
        && !结果.登记.has_value() && 前.快照.has_value() && 后.快照.has_value()
        && 前.快照->事实代次 == 后.快照->事实代次
        && 前.快照->当前节点.size() == 后.快照->当前节点.size();
}

bool 项目10() {
    L1事实基座服务 L1;
    世界登记服务 服务(L1);
    (void)服务.建立空世界(请求(11));
    世界登记服务 新服务(L1);
    const auto 前 = L1.读取完整快照();
    const auto 结果 = 新服务.读取当前登记();
    const auto 后 = L1.读取完整快照();
    return 结果.状态 == 世界登记状态::登记未加载
        && !结果.登记.has_value() && 前.快照.has_value() && 后.快照.has_value()
        && 前.快照->事实代次 == 后.快照->事实代次;
}

世界登记自检报告 执行() {
    const std::array<bool, 10> 结果{
        项目1(), 项目2(), 项目3(), 项目4(), 项目5(),
        项目6(), 项目7(), 项目8(), 项目9(), 项目10()};
    世界登记自检报告 报告;
    报告.验收.assign(结果.begin(), 结果.end());
    for (const bool 通过 : 报告.验收) if (!通过) ++报告.失败数量;
    报告.总通过 = 报告.失败数量 == 0;
    return 报告;
}

} // namespace 海中鱼巣::世界登记自检内部

export namespace 海中鱼巣 {

世界登记自检报告 运行世界登记自检() {
    return 世界登记自检内部::执行();
}

} // namespace 海中鱼巣
