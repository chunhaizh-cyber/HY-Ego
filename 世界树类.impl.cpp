module;

#include <afx.h>

// 世界树类.impl.cpp
module 世界树模块;

import <vector>;
import <cstdint>;

import 主信息定义模块;
import 场景模块;
import 存在模块;
import 特征模块;
import 特征值模块;

namespace {

    template<class T>
    void 尝试记录观测(T* mi, 时间戳 now)
    {
        if (!mi || now == 0) return;
        if constexpr (requires { mi->记录观测(now); }) {
            mi->记录观测(now);
        }
    }

    inline void 递增命中次数(存在节点类* e)
    {
        auto* mi = dynamic_cast<存在节点主信息类*>(e ? e->主信息 : nullptr);
        if (!mi) return;
        mi->命中次数 += 1;
    }

    inline 特征节点类* 写入标量特征(
        特征类& 特征集,
        基础信息节点类* 所属,
        词性节点类* 类型,
        std::int64_t 值,
        bool 使用融合,
        const 特征值类::融合参数& 融合参数)
    {
        if (!所属 || !类型) return nullptr;

        // 1) 确保特征节点
        auto* f = 特征集.找到或创建特征(所属, 类型);

        // 2) 确保特征值节点（全局唯一仓库）
        特征值类 值仓库;
        auto* v = 值仓库.获取或创建标量特征值(/*单位=*/nullptr, 值);

        // 3) 写入
        if (使用融合) {
            特征集.设置特征值_融合(f, v, 融合参数, /*失败则覆盖=*/true);
        }
        else {
            特征集.设置特征值_覆盖(f, v);
        }
        return f;
    }


    inline 特征节点类* 写入矢量特征(
        特征类& 特征集,
        基础信息节点类* 所属,
        词性节点类* 类型,
        const std::vector<std::int64_t>& 值,
        bool 使用融合,
        const 特征值类::融合参数& 融合参数)
    {
        if (!所属 || !类型) return nullptr;

        // 1) 确保特征节点
        auto* f = 特征集.找到或创建特征(所属, 类型);

        // 2) 确保特征值节点（全局唯一仓库）
        特征值类 值仓库;
        auto* v = 值仓库.获取或创建矢量特征值(值);

        // 3) 写入
        if (使用融合) {
            特征集.设置特征值_融合(f, v, 融合参数, /*失败则覆盖=*/true);
        }
        else {
            特征集.设置特征值_覆盖(f, v);
        }
        return f;
    }




} // namespace

场景节点类* 世界树类::为存在添加内部世界(存在节点类* 存在, 场景节点主信息类* 内部世界主信息)
{
    if (!存在) return nullptr;

    auto* emi = dynamic_cast<存在节点主信息类*>(存在->主信息);
    if (!emi) return nullptr;
    if (emi->内部世界) return emi->内部世界;

    // 若未提供主信息则创建默认
    if (!内部世界主信息) {
        内部世界主信息 = new 场景节点主信息类();
    }

    // 由场景模块负责创建并挂到宇宙根层，同时写回 宿主存在
    auto* 场景 = 场景集.找到或创建内部世界(存在, 内部世界主信息->名称, 内部世界主信息->世界类型, /*now=*/内部世界主信息->最后观测时间);
    if (!场景) {
        // 兜底：直接创建
        内部世界主信息->宿主存在 = 存在;
        场景 = 场景集.创建并添加场景(/*父=*/nullptr, 内部世界主信息, /*now=*/内部世界主信息->最后观测时间);
        emi->内部世界 = 场景;
    }
    return 场景;
}

存在节点类* 世界树类::为场景添加存在(场景节点类* 场景, 存在节点主信息类* 存在主信息)
{
    if (!场景) 场景 = 自我所在场景;
    if (!场景) 场景 = 现实世界;
    if (!场景) return nullptr;

    if (!存在主信息) 存在主信息 = new 存在节点主信息类();
    return 场景集.创建并添加存在(场景, 存在主信息);
}

存在节点类* 世界树类::在场景中新建存在(场景节点类* 所在场景, 存在节点主信息类* 主信息)
{
    if (!所在场景) 所在场景 = 自我所在场景;
    if (!所在场景) 所在场景 = 现实世界;
    if (!所在场景) return nullptr;
    if (!主信息) 主信息 = new 存在节点主信息类();
    return 存在集.创建并添加存在(所在场景, 主信息);
}

存在节点类* 世界树类::在场景查找存在(场景节点类* 所在场景, 存在节点主信息类* 主信息)
{
    if (!所在场景) 所在场景 = 自我所在场景;
    if (!所在场景 || !主信息) return nullptr;

    // 复用场景模块的“观测信息匹配”
    auto list = 场景集.查找存在_按观测信息_抽象存在版(所在场景, 主信息);
    if (list.empty()) return nullptr;
    return list.front();
}

存在节点类* 世界树类::在场景查找存在(场景节点类* 所在场景, 存在节点类* 存在节点指针)
{
    if (!所在场景) 所在场景 = 自我所在场景;
    if (!所在场景 || !存在节点指针) return nullptr;

    // 快速判断：父指针一致即属于该场景
    if (存在节点指针->父 == 所在场景) return 存在节点指针;

    // 退化：按主信息查找
    auto* mi = dynamic_cast<存在节点主信息类*>(存在节点指针->主信息);
    if (!mi) return nullptr;
    return 在场景查找存在(所在场景, mi);
}

特征节点类* 世界树类::为存在添加特征(存在节点类* 存在, 特征节点主信息类* 特征主信息)
{
    if (!存在 || !特征主信息) return nullptr;
    return 存在集.为存在添加特征(存在, 特征主信息);
}

特征节点类* 世界树类::查找特征节点_类型加值(存在节点类* 存在, 词性节点类* 特征类型, 特征值节点类* 目标特征值) const
{
    if (!存在 || !特征类型 || !目标特征值) return nullptr;
    auto* emi = dynamic_cast<存在节点主信息类*>(存在->主信息);
    if (!emi) return nullptr;

    for (auto* f : emi->特征索引) {
        auto* fmi = dynamic_cast<特征节点主信息类*>(f ? f->主信息 : nullptr);
        if (!fmi || fmi->类型 != 特征类型) continue;

        // 这里“精确匹配”的定义：值节点指针相同（值节点由值树/仓库复用）
        if (fmi->值 == 目标特征值) return f;
    }
    return nullptr;
}

bool 世界树类::是否存在该特征值(存在节点类* 存在, 词性节点类* 特征类型, 特征值节点类* 目标特征值) const
{
    return 查找特征节点_类型加值(存在, 特征类型, 目标特征值) != nullptr;
}

特征节点类* 世界树类::在存在中查找特征类型(存在节点类* 存在, 词性节点类* 特征类型) const
{
    if (!存在 || !特征类型) return nullptr;
    return 特征集.查找特征(存在, 特征类型);
}

std::vector<特征节点类*> 世界树类::查找所有特征节点(存在节点类* 存在, 词性节点类* 特征类型) const
{
    std::vector<特征节点类*> out;
    if (!存在 || !特征类型) return out;

    auto* emi = dynamic_cast<存在节点主信息类*>(存在->主信息);
    if (!emi) return out;

    for (auto* f : emi->特征索引) {
        auto* fmi = dynamic_cast<特征节点主信息类*>(f ? f->主信息 : nullptr);
        if (fmi && fmi->类型 == 特征类型) out.push_back(f);
    }
    return out;
}

存在节点类* 世界树类::观测到存在(
    场景节点类* 场景,
    const Vector3D& 中心坐标,
    const Vector3D& 尺寸坐标,
    const std::vector<std::int64_t>& 轮廓编码,
    const 结构体_观测融合参数& p,
    时间戳 now)
{
    if (!场景) 场景 = 自我所在场景;
    if (!场景) 场景 = 现实世界;
    if (!场景) return nullptr;

    // 1) 先按空间查找
    存在节点类* e = 场景集.查找存在_按中心尺寸(场景, 中心坐标, 尺寸坐标, p.匹配参数, /*使用AABB相交=*/true);

    // 2) 找不到则创建
    if (!e) {
        auto* mi = new 存在节点主信息类();
        e = 场景集.创建并添加存在(场景, mi);
    }

    // 3) 命中次数 + 时间
    递增命中次数(e);
    尝试记录观测(dynamic_cast<存在节点主信息类*>(e->主信息), now);

    // 4) 写入位置/尺寸（优先向量类型，其次标量三轴）
    if (p.匹配参数.位置向量特征类型) {
        // 若你把位置/尺寸以 Vector3D 挂在“矢量特征值”，此处可扩展为写入三元。
        // 当前先按三轴写入（你项目里约定更多使用 X/Y/Z 标量）。
    }

    if (p.匹配参数.位置X特征类型) 写入标量特征(特征集, e, p.匹配参数.位置X特征类型, static_cast<std::int64_t>(中心坐标.x), p.位置尺寸使用融合, p.融合参数);
    if (p.匹配参数.位置Y特征类型) 写入标量特征(特征集, e, p.匹配参数.位置Y特征类型, static_cast<std::int64_t>(中心坐标.y), p.位置尺寸使用融合, p.融合参数);
    if (p.匹配参数.位置Z特征类型) 写入标量特征(特征集, e, p.匹配参数.位置Z特征类型, static_cast<std::int64_t>(中心坐标.z), p.位置尺寸使用融合, p.融合参数);

    if (p.匹配参数.尺寸X特征类型) 写入标量特征(特征集, e, p.匹配参数.尺寸X特征类型, static_cast<std::int64_t>(尺寸坐标.x), p.位置尺寸使用融合, p.融合参数);
    if (p.匹配参数.尺寸Y特征类型) 写入标量特征(特征集, e, p.匹配参数.尺寸Y特征类型, static_cast<std::int64_t>(尺寸坐标.y), p.位置尺寸使用融合, p.融合参数);
    if (p.匹配参数.尺寸Z特征类型) 写入标量特征(特征集, e, p.匹配参数.尺寸Z特征类型, static_cast<std::int64_t>(尺寸坐标.z), p.位置尺寸使用融合, p.融合参数);

    // 5) 写入轮廓编码（可选）
    if (p.轮廓特征类型 && !轮廓编码.empty()) {
        写入矢量特征(特征集, e, p.轮廓特征类型, 轮廓编码, p.轮廓使用融合, p.融合参数);
    }

    return e;
}
