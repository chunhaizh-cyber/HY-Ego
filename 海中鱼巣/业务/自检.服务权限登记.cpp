#include "自检.服务权限登记.h"
#include "新_世界树类.h"
#include "../领域/消息交互.数据.h"

#include <algorithm>
#include <array>
#include <fstream>
#include <limits>
#include <stdexcept>

namespace 海中鱼巣 {

bool 运行服务权限登记工程验证() {
    std::ofstream 输出("service-permission-verification.txt", std::ios::trunc);
    if (!输出) return false;
    const auto 核验 = [&](bool 条件, const char* 标识) {
        输出 << 标识 << ' ' << (条件 ? "PASS" : "FAIL") << std::endl;
        if (!条件) throw std::runtime_error(标识);
    };
    try {
        using W = 新活动服务登记状态;
        using V = 新服务权限值状态;
        新_世界树类 世界;
        const auto 初始化 = 世界.初始化();
        核验(初始化.世界树根场景.has_value(), "world-owner");
        const auto 场景 = *初始化.世界树根场景;
        auto 事实 = 世界.获取事实服务引用();
        auto& 值 = 事实.特征值服务;
        auto& FT = 事实.特征概念服务;
        auto& 概念 = 事实.存在概念服务;
        const std::array<const 正式概念身份读取接口*, 2> 读取{&FT, &概念};
        核验(事实.特征服务.装配正式概念引用读取接口(&概念, 读取), "owner-readers");
        const auto 零 = 值.保存或取得服务权限值(0);
        const auto 最大 = 值.保存或取得服务权限值((std::numeric_limits<std::uint64_t>::max)());
        核验(零.状态 == V::已读取 && 零.数值 == 0 && 值.读取服务权限值(零.节点).数值 == 0
            && 值.保存或取得服务权限值(0).节点 == 零.节点, "uint64-zero-exact-reuse");
        核验(最大.状态 == V::已读取 && 最大.数值 == (std::numeric_limits<std::uint64_t>::max)()
            && 最大.节点 != 零.节点 && 值.读取服务权限值(最大.节点).数值 == 最大.数值,
            "uint64-max-exact");
        const auto 极限 = (std::numeric_limits<std::uint64_t>::max)();
        核验(选择下一个服务权限({}) == 0 && 选择下一个服务权限(极限 - 1) == 极限
            && !选择下一个服务权限(极限), "no-permission-wrap-at-cap");
        const auto 错形 = 值.保存或取得特征值(材料物理类型::U64数组, std::vector<std::uint64_t>{0,1});
        核验(值.读取服务权限值(错形).状态 == V::材料形状不相容, "reject-nonsingleton");
        const auto 权限定义 = FT.建立或取得先天特征类型槽({先天特征类型槽::服务权限,
            {特征概念材料物理类型::U64数组, 特征概念全材料值域{}, {},
             特征概念比较规则::完整材料精确}}, 值, 读取);
        核验(权限定义.类型根.has_value(), "permission-owner-slot");
        const auto 许可特征 = FT.建立或取得特征概念(
            {特征概念材料物理类型::I64标量, std::vector<特征概念I64闭区间>{{0,1}}}, 值);
        const auto 时 = 新状态强时间{新状态时间语义::实例绝对UTC, 1};
        const auto 建立 = [&](std::int64_t 测试值) {
            return 事实.场景服务.建立成员存在(场景, {{}, {{许可特征,测试值}}}, 时);
        };
        const auto A = 建立(1), B = 建立(1), N = 建立(0), H = 建立(0);
        核验(A.存在节点 && B.存在节点 && N.存在节点 && H.存在节点
            && A.专属存在概念节点 && B.专属存在概念节点, "isolated-existence-fixtures");
        const auto 分类 = 概念.抽象共同存在概念({*A.专属存在概念节点, *B.专属存在概念节点});
        核验(分类.抽象概念节点 && !分类.共同特征组.empty(), "independent-fixture-concept");
        const auto 服务类型 = FT.建立或取得特征概念({特征概念材料物理类型::存在节点引用,
            特征概念存在引用值域{*分类.抽象概念节点}, {}, 特征概念比较规则::存在身份精确}, 值);
        const auto F = 事实.存在服务.添加无当前值特征(*H.存在节点, 服务类型);
        核验(F.特征节点.has_value(), "service-feature-owner");
        const auto 快照 = [&] { return 事实.特征服务.获取特征(*F.特征节点); };
        const auto 次 = [&](稳定编码 E, std::optional<稳定编码> 前) {
            return 世界.登记活动数据集服务对象(*H.存在节点, *F.特征节点, E, 前);
        };
        const auto 原 = 快照();
        const auto 原状态 = 事实.场景服务.查询状态列表(场景);
        const auto 排除 = 次(*N.存在节点, {});
        核验(排除.状态 == W::资格排除 && 快照() == 原
            && 事实.场景服务.查询状态列表(场景) == 原状态, "negative-no-write");
        const auto 初 = 次(*A.存在节点, {});
        输出 << "A status " << static_cast<int>(初.状态) << std::endl;
        核验(初.状态 == W::已登记 && 初.权限 == 0 && 初.服务状态节点, "first-A-zero");
        const auto 初读 = 快照();
        const auto 初状态 = 事实.场景服务.查询状态列表(场景);
        核验(次(*B.存在节点, {}).状态 == W::前提已变化 && 快照() == 初读
            && 事实.场景服务.查询状态列表(场景) == 初状态, "stale-no-write");
        const auto 后 = 次(*B.存在节点, A.存在节点);
        核验(后.状态 == W::已登记 && 后.权限 == 1 && 后.服务状态节点, "B-next");
        const auto 回 = 次(*A.存在节点, B.存在节点);
        const auto 回读 = 快照();
        核验(回.状态 == W::已登记 && 回.权限 == 0 && 回.服务状态节点 && 回读
            && 回读->历史不同值.size() == 2
            && std::ranges::find(回读->历史不同值, 新特征准确值{*A.存在节点}) != 回读->历史不同值.end()
            && std::ranges::find(回读->历史不同值, 新特征准确值{*B.存在节点}) != 回读->历史不同值.end()
            && 回读->当前值 == 新特征准确值{*A.存在节点}, "A-B-A-owner-readback");
        const auto 独立权限 = [&](稳定编码 E, std::uint64_t 期望) {
            const auto 主体 = 事实.存在服务.获取存在(E);
            if (!主体) return false;
            std::size_t 命中 = 0;
            for (const auto F : 主体->特征节点组) {
                const auto 类型 = 事实.特征服务.读取当前特征类型(F);
                if (类型.特征类型根 != 权限定义.类型根) continue;
                const auto 权限 = 事实.特征服务.获取特征(F);
                const auto* V = 权限 && 权限->当前值 ? std::get_if<稳定编码>(&*权限->当前值) : nullptr;
                if (!V || 值.读取服务权限值(*V).数值 != 期望
                    || 权限->历史不同值 != std::vector<新特征准确值>{*V}
                    || 权限->实例历史聚合值域 != 特征概念值域{std::vector<稳定编码>{*V}})
                    return false;
                ++命中;
            }
            return 命中 == 1;
        };
        核验(独立权限(*A.存在节点, 0) && 独立权限(*B.存在节点, 1), "permission-F-V-owner-readback");
        const auto 状态 = 事实.场景服务.查询状态列表(场景);
        const auto 已当前 = 次(*A.存在节点, A.存在节点);
        核验(已当前.状态 == W::已是当前 && 已当前.权限 == 0 && 快照() == 回读
            && 事实.场景服务.查询状态列表(场景) == 状态
            && 状态.size() == 原状态.size() + 5, "current-no-write-and-five-states");
        const auto 旁证类型 = FT.建立或取得特征概念(
            {特征概念材料物理类型::I64标量, std::vector<特征概念I64闭区间>{{0,1}}}, 值);
        const auto 待 = 事实.场景服务.建立成员存在(场景, {{}, {{旁证类型,std::int64_t{0}}}}, 时);
        核验(待.存在节点.has_value(), "pending-fixture");
        const auto 再前 = 快照();
        核验(次(*待.存在节点, A.存在节点).状态 == W::资格待定 && 快照() == 再前,
            "pending-no-registration");
        const auto 补F = 事实.存在服务.添加无当前值特征(*待.存在节点, 许可特征);
        核验(补F.特征节点.has_value(), "late-fact-owner-target");
        const auto 发布 = 世界.更新存在特征并形成动态(场景, *待.存在节点, *补F.特征节点,
            新特征准确值{std::int64_t{1}}, {新状态时间语义::实例绝对UTC, 2});
        核验(发布.新状态节点.has_value(), "late-fact-owner-state");
        const auto S节点 = *发布.新状态节点;
        const auto 材料 = 世界.读取活动对端续判状态(场景, *待.存在节点, S节点, 1, true);
        核验(材料 && 材料->节点 == S节点 && 材料->来源实例特征节点 == *补F.特征节点,
            "same-owner-late-state-reopened");
        核验(!世界.读取活动对端续判状态(场景, *A.存在节点, S节点, 1, true)
            && !世界.读取活动对端续判状态(*A.存在节点, *待.存在节点, S节点, 1, true)
            && !世界.读取活动对端续判状态(场景, *待.存在节点, S节点, 2, true)
            && !世界.读取活动对端续判状态(场景, *待.存在节点, S节点, 1, false)
            && 快照() == 再前, "wrong-stale-repeat-closed-no-registration");
        const auto 新登记 = 次(*待.存在节点, A.存在节点);
        核验(新登记.状态 == W::已登记 && 新登记.权限 == 2 && 独立权限(*待.存在节点, 2),
            "late-state-then-formal-qualification-and-register");
        输出 << "late E=" << 待.存在节点->值 << " source S=" << S节点.值
            << " service S=" << 新登记.服务状态节点->值 << std::endl;
        输出 << "A=" << A.存在节点->值 << " B=" << B.存在节点->值 << " F=" << F.特征节点->值
            << " states=" << 初.服务状态节点->值 << "," << 后.服务状态节点->值 << "," << 回.服务状态节点->值 << std::endl;
        // 已登记工程入口的独立活动数据集材料，不是网页身份输入。
        const auto 根 = 概念.读取存在根概念保护();
        核验(根.信息.has_value() && 世界.登记自我(*H.存在节点,场景) == 新场景操作状态::已完成,
            "passive-self-owner");
        const auto 参与 = FT.建立或取得特征概念({特征概念材料物理类型::存在节点引用,
            特征概念存在引用值域{根.信息->节点}, {}, 特征概念比较规则::存在身份精确}, 值);
        核验(世界.配置现实交互场景特征(服务类型,参与) == 新场景操作状态::已完成
            && 世界.配置原消息角色事实(参与) == 新原消息角色状态::已完成, "passive-scene-contract");
        const auto 建窗 = [&](std::optional<稳定编码> E) {
            const auto 输入 = 全局基础数据集.原子建立节点组(std::vector<基础组内新节点>(8));
            核验(输入.节点组.size()==8,"passive-fixture-provenance");
            const auto& n = 输入.节点组;
            const 新临时交互场景来源 源{n[0],n[1],n[2],n[3]};
            const auto 窗 = 世界.采用已确认临时交互场景(源);
            核验(窗.信息.has_value(),"passive-formal-window-owner");
            if (!E) return n[0];
            const auto 角色 = 世界.准备原消息场景角色事实({n[4],n[0],n[1],n[5],新外设消息方向::接收});
            核验(角色.状态==新原消息角色状态::已完成 && 角色.片段,"passive-role-source");
            const auto 角色发布 = 全局基础数据集.原子追加封存片段组({*角色.片段});
            核验(角色发布.状态==基础节点组追加状态::已完成 && 角色发布.片段回执组.size()==1,
                "passive-published-role");
            const auto 角色节点 = 角色发布.片段回执组.front().新节点组().front();
            核验(世界.采用本窗身份认知({{{},窗.信息->场景,窗.信息->本窗对端,*E,n[4],角色节点,n[6],n[7]},源,{}}),
                "passive-published-cognition");
            return n[0];
        };
        using 模式 = 新本窗服务登记模式;
        const auto 丙 = 建立(1), 丁 = 建立(1);
        核验(丙.存在节点 && 丁.存在节点,"passive-new-eligible-targets");
        const auto 无窗 = 建窗({}), 否窗 = 建窗(N.存在节点);
        const auto 丙窗 = 建窗(丙.存在节点), 丁窗 = 建窗(丁.存在节点);
        const auto 从窗 = [&](稳定编码 窗, 模式 选择 = 模式::隔离活动数据集验证) {
            return 世界.确认登记本窗服务对象(窗,*H.存在节点,*F.特征节点,选择);
        };
        const auto 前 = 快照();
        核验(从窗(无窗).状态 == W::资格待定 && 从窗(否窗).状态 == W::资格排除
            && 从窗(丙窗,模式::普通待恢复).状态 == W::恢复写入未开放 && 快照()==前,
            "passive-pending-excluded-restore-gate-no-write");
        const auto 丙登记 = 从窗(丙窗), 丁登记 = 从窗(丁窗);
        输出 << "passive C status " << static_cast<unsigned>(丙登记.状态)
            << " D status " << static_cast<unsigned>(丁登记.状态) << std::endl;
        核验(丙登记.状态==W::已登记 && 丙登记.权限==3 && 丁登记.状态==W::已登记 && 丁登记.权限==4
            && 独立权限(*丙.存在节点,3) && 独立权限(*丁.存在节点,4), "passive-two-new-ordered-permissions");
        const auto 动作 = 丙登记.认知来源 ? 世界.读取本窗服务动作(*丙登记.认知来源) : std::nullopt;
        核验(动作 && 动作->被动动作==丙登记.被动动作 && 动作->服务状态节点==丙登记.服务状态节点
            && 动作->认知存在==丙.存在节点,"passive-independent-cognition-action-state");
        const auto 已丁 = 快照();
        核验(从窗(丙窗).状态==W::已处理 && 快照()==已丁,"passive-replay-no-switch");
        const auto 同丁 = 从窗(建窗(丁.存在节点));
        核验(同丁.状态==W::已是当前 && !同丁.被动动作 && 快照()==已丁,"passive-already-current-zero-write");
        const auto 再丙 = 从窗(建窗(丙.存在节点));
        const auto 终 = 快照();
        核验(再丙.状态==W::已登记 && 再丙.权限==3 && 再丙.被动动作 && 终 && 已丁
            && 终->历史不同值==已丁->历史不同值 && 终->当前值==新特征准确值{*丙.存在节点},
            "passive-fresh-window-reuses-reference-and-permission");
        输出 << "passive cognition=" << 丙登记.认知来源->值 << " action=" << 丙登记.被动动作->值
            << " service state=" << 丙登记.服务状态节点->值 << std::endl;
        return true;
    } catch (const std::exception& e) {
        输出 << e.what() << std::endl;
        return false;
    }
}

} // namespace 海中鱼巣
