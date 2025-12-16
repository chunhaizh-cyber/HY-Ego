
export module 场景模块;

import 基础数据类型模块;
import 主信息定义模块;
import 数据仓库模块;   // for 世界链
import 存在模块;
import 特征模块;
//import 特征值模块;
//import 二次特征模块;

import <vector>;
import <string>;
import <cstdint>;
import <algorithm>;
import <functional>;
import <optional>;
import <variant>;
import <limits>;
import <cmath>;
import <stdexcept>;

//////////////////////////////////////////////////////////////////
// 场景节点模块：提供
// 1) 结构体_AABB3D：3D 包围盒工具
// 2) 场景节点存档：仅存主键与引用主键（用于存档/重载）
// 3) 场景类：场景节点的创建/挂接/删除/索引维护/空间查找
//////////////////////////////////////////////////////////////////

// ---------- 3D 盒工具（空间匹配核心） ----------
export struct 结构体_AABB3D
{
    Vector3D 最小{};
    Vector3D 最大{};

    static 结构体_AABB3D 由中心尺寸(const Vector3D& 中心, const Vector3D& 尺寸, double 放大倍数 = 1.0)
    {
        Vector3D 半径{
            (尺寸.x * 0.5) * 放大倍数,
            (尺寸.y * 0.5) * 放大倍数,
            (尺寸.z * 0.5) * 放大倍数
        };
        return {
            { 中心.x - 半径.x, 中心.y - 半径.y, 中心.z - 半径.z },
            { 中心.x + 半径.x, 中心.y + 半径.y, 中心.z + 半径.z }
        };
    }

    bool 包含点(const Vector3D& 点, double 容差 = 0.0) const
    {
        return (点.x >= 最小.x - 容差 && 点.x <= 最大.x + 容差) &&
            (点.y >= 最小.y - 容差 && 点.y <= 最大.y + 容差) &&
            (点.z >= 最小.z - 容差 && 点.z <= 最大.z + 容差);
    }

    bool 相交(const 结构体_AABB3D& b) const
    {
        return (最小.x <= b.最大.x && 最大.x >= b.最小.x) &&
            (最小.y <= b.最大.y && 最大.y >= b.最小.y) &&
            (最小.z <= b.最大.z && 最大.z >= b.最小.z);
    }
};

// ---------- 存档快照（只存 ID，重载时再二次解析指针） ----------
export struct 场景节点存档
{
    std::string 主键;

    // 预留：若以后你要给“场景概念模板”单独挂钩，可用此字段
    std::string 概念模板ID;

    std::string 宿主存在ID;
    std::string 父场景ID;

    时间戳 创建时间 = 0;
    时间戳 最后观测时间 = 0;
    std::uint32_t 命中次数 = 0;

    std::vector<std::string> 子场景IDs;
    std::vector<std::string> 子存在IDs;
    std::vector<std::string> 特征IDs;
};

// ---------- 场景：空间匹配参数 ----------
// 兼容两种建模：
// A) 一个“位置向量(Vector3D)”特征 + 一个“尺寸向量(Vector3D)”特征
// B) 位置/尺寸按轴拆成 6 个标量特征（X/Y/Z + 宽/高/深）
export struct 结构体_场景存在匹配参数
{
    // 优先：向量型（Vector3D）
    词性节点类* 位置向量特征类型 = nullptr;
    词性节点类* 尺寸向量特征类型 = nullptr;

    // 兼容：标量型（int64_t）
    词性节点类* 位置X特征类型 = nullptr;
    词性节点类* 位置Y特征类型 = nullptr;
    词性节点类* 位置Z特征类型 = nullptr;

    词性节点类* 尺寸X特征类型 = nullptr; // 左右 / 宽
    词性节点类* 尺寸Y特征类型 = nullptr; // 上下 / 高
    词性节点类* 尺寸Z特征类型 = nullptr; // 前后 / 深

    // 匹配控制
    double 放大倍数 = 1.0; // 用于把“存在自身AABB”扩大一点以增强鲁棒性
    double 容差 = 0.0;     // AABB 包含点容差
};

// ---------- 场景类（管理器：创建/挂接/删除/索引维护/空间查询） ----------
export class 场景类
{
private:
    static  inline 存在类 存在集{};
    static inline 特征类 特征集{};
public:
    //============================================================
    // 1) 创建/挂接：场景
    //============================================================

    // 创建一个场景节点并挂到父场景（父场景可为 nullptr，表示挂到宇宙根层）
    场景节点类* 创建并添加场景(场景节点类* 父场景, 场景节点主信息类* 场景主信息, 时间戳 now = 0) const
    {
        if (!场景主信息) return nullptr;

        if (now != 0) 场景主信息->记录观测(now);

        auto* 新节点 = static_cast<场景节点类*>(
            世界链.添加子节点(父场景, static_cast<基础信息基类*>(场景主信息))
            );

        // 更新父场景索引（方案C）
        if (父场景) {
            if (auto* pmi = 取场景主信息(父场景)) {
                pmi->添加子场景(新节点);
            }
        }
        return 新节点;
    }

    // 便捷：直接用 名称+世界类型 创建主信息
    场景节点类* 创建并添加场景(场景节点类* 父场景, 词性节点类* 名称, 枚举_世界类型 世界类型, 时间戳 now = 0) const
    {
        if (父场景 == nullptr)
        {
            父场景 = 世界链.根指针;
        }
        auto* mi = new 场景节点主信息类(名称, 世界类型);
        return 创建并添加场景(父场景, mi, now);
    }

    // 便捷：创建“内部世界”（默认挂到宇宙根层；并写回 宿主存在->内部世界）
    场景节点类* 找到或创建内部世界(存在节点类* 宿主存在, 词性节点类* 场景名 = nullptr, 枚举_世界类型 世界类型 = 枚举_世界类型::现实世界, 时间戳 now = 0) const
    {
        if (!宿主存在 || !宿主存在->主信息) return nullptr;

        auto* emi = dynamic_cast<存在节点主信息类*>(宿主存在->主信息);
        if (!emi) return nullptr;

        if (emi->内部世界) return emi->内部世界;

        auto* mi = new 场景节点主信息类(场景名, 世界类型);
        mi->宿主存在 = 宿主存在;
        auto* 场景 = 创建并添加场景(/*父场景=*/nullptr, mi, now);
        emi->内部世界 = 场景;
        return 场景;
    }

    // 兼容：将“已分配但未挂接”的场景节点挂到父场景
    场景节点类* 添加场景(场景节点类* 父场景, 场景节点类* 场景节点) const
    {
        if (!场景节点) return nullptr;

        auto* 新节点 = static_cast<场景节点类*>(世界链.添加子节点(父场景, 场景节点));

        if (父场景) {
            if (auto* pmi = 取场景主信息(父场景)) {
                pmi->添加子场景(新节点);
            }
        }
        return 新节点;
    }

    //============================================================
    // 2) 场景内容：添加/删除 存在 与 特征（索引同步）
    //============================================================

    // 把“新存在主信息”创建为存在节点并挂到场景
    存在节点类* 创建并添加存在(场景节点类* 场景, 存在节点主信息类* 存在主信息) const
    {
        // 存在模块里已经包含：世界链挂接 + 维护 场景->存在索引
        return 存在集.创建并添加存在(场景, 存在主信息);
    }

    // 兼容：把“已存在节点”挂到场景
    存在节点类* 添加存在(场景节点类* 场景, 存在节点类* 存在节点) const
    {
        return 存在集.添加存在(场景, 存在节点);
    }

    // 删除场景里的一个存在（只删节点，不删“值节点”共享资源）
    bool 删除存在(场景节点类* 场景, 存在节点类* 存在节点) const
    {
        if (!场景 || !存在节点) return false;
        if (存在节点->父 != 场景) return false;

        auto* smi = 取场景主信息(场景);
        if (!smi) return false;

        修正父节点子指针_在删除前(存在节点);
        smi->移除存在(存在节点);

        世界链.删除节点(存在节点);
        return true;
    }

    // 为场景添加特征（值节点可空）
    特征节点类* 为场景添加特征(场景节点类* 场景, 词性节点类* 特征类型, 特征值节点类* 值节点 = nullptr, 词性节点类* 特征名 = nullptr) const
    {
        return 特征集.创建并添加特征节点(场景, 特征类型, 值节点, 特征名);
    }

    // 删除场景的某个特征节点
    bool 删除场景特征(场景节点类* 场景, 特征节点类* 特征节点) const
    {
        if (!场景 || !特征节点) return false;
        if (特征节点->父 != 场景) return false;

        auto* smi = 取场景主信息(场景);
        if (!smi) return false;

        修正父节点子指针_在删除前(特征节点);
        smi->移除特征(特征节点);

        世界链.删除节点(特征节点);
        return true;
    }

    //============================================================
    // 3) 删除：子场景（会递归删其子链）
    //============================================================

    bool 删除子场景(场景节点类* 父场景, 场景节点类* 子场景) const
    {
        if (!父场景 || !子场景) return false;
        if (子场景->父 != 父场景) return false;

        auto* pmi = 取场景主信息(父场景);
        if (!pmi) return false;

        // 若这是某存在的内部世界，断开宿主指针
        if (auto* smi = 取场景主信息(子场景)) {
            if (smi->宿主存在 && smi->宿主存在->主信息) {
                if (auto* emi = dynamic_cast<存在节点主信息类*>(smi->宿主存在->主信息)) {
                    if (emi->内部世界 == 子场景) emi->内部世界 = nullptr;
                }
            }
        }

        修正父节点子指针_在删除前(子场景);
        pmi->移除子场景(子场景);
        世界链.删除节点(子场景);
        return true;
    }

    //============================================================
    // 4) 索引维护：从子链重建（容灾/批量修改后）
    //============================================================

    void 重建索引_从子链(场景节点类* 场景) const
    {
        if (!场景) return;
        auto* smi = 取场景主信息(场景);
        if (!smi) return;

        smi->子场景索引.clear();
        smi->存在索引.clear();
        smi->特征索引.clear();

        if (!场景->子) return;

        auto* 起 = static_cast<基础信息节点类*>(场景->子);
        auto* 游 = 起;
        do {
            if (!游 || !游->主信息) break;

            if (dynamic_cast<场景节点主信息类*>(游->主信息)) {
                smi->子场景索引.push_back(static_cast<场景节点类*>(游));
            }
            else if (dynamic_cast<存在节点主信息类*>(游->主信息)) {
                smi->存在索引.push_back(static_cast<存在节点类*>(游));
            }
            else if (dynamic_cast<特征节点主信息类*>(游->主信息)) {
                smi->特征索引.push_back(static_cast<特征节点类*>(游));
            }

            游 = static_cast<基础信息节点类*>(游->下);
        } while (游 && 游 != 起);

        smi->整理();
    }

    //============================================================
    // 5) 空间查找：在场景中按“中心+尺寸”找同一存在
    //============================================================

    // 返回：最匹配（中心距离最小）的存在；若都无法提取空间信息则返回 nullptr
    存在节点类* 查找存在_按中心尺寸(
        场景节点类* 场景,
        const Vector3D& 目标中心,
        const Vector3D& 目标尺寸,
        const 结构体_场景存在匹配参数& p,
        bool 使用AABB相交 = true) const
    {
        if (!场景) return nullptr;
        auto* smi = 取场景主信息(场景);
        if (!smi) return nullptr;

        // 优先使用索引；若索引为空且子链非空，建议调用 重建索引_从子链
        const auto& 候选 = smi->存在索引;

        const auto 目标盒 = 结构体_AABB3D::由中心尺寸(目标中心, 目标尺寸, 1.0);

        存在节点类* best = nullptr;
        double bestDist2 = std::numeric_limits<double>::infinity();

        for (auto* e : 候选) {
            Vector3D c{}, s{};
            if (!取存在空间_中心尺寸(e, p, c, s)) continue;

            const auto 盒 = 结构体_AABB3D::由中心尺寸(c, s, std::max(1.0, p.放大倍数));

            bool ok = 使用AABB相交 ? 盒.相交(目标盒) : 盒.包含点(目标中心, p.容差);
            if (!ok) continue;

            const double dx = c.x - 目标中心.x;
            const double dy = c.y - 目标中心.y;
            const double dz = c.z - 目标中心.z;
            const double d2 = dx * dx + dy * dy + dz * dz;

            if (d2 < bestDist2) { bestDist2 = d2; best = e; }
        }

        return best;
    }
    std::vector<存在节点类*>  查找存在_按观测信息(场景节点类* 场景, 存在节点主信息类* 观测MI) const
    {
        std::vector<存在节点类*> out;
        if (!场景 || !观测MI) return out;

        auto* smi = 取场景主信息(场景);
        if (!smi) return out;

        // 索引为空时兜底重建（避免“场景里有子节点但索引没维护”）
        if (smi->存在索引.empty() && 场景->子) {
            重建索引_从子链(场景);
        }
        const auto& 候选存在 = smi->存在索引;
        if (候选存在.empty()) return out;

        //============================================================
        // 0) 收集观测存在的特征（从“当前状态”优先）
        //============================================================
        struct 观测标量 { 词性节点类* t = nullptr; double v = 0; };
        struct 观测向量3 { 词性节点类* t = nullptr; Vector3D v{}; };
        struct 观测矢量 { 词性节点类* t = nullptr; const std::vector<std::int64_t>* v = nullptr; };

        std::vector<观测标量> obsS;
        std::vector<观测向量3> obsV3;
        std::vector<观测矢量>  obsVec;

        const auto& obsFeatures = (!观测MI->特征当前状态.empty())
            ? 观测MI->特征当前状态
            : 观测MI->特征索引;

        for (auto* f : obsFeatures) {
            if (!f || !f->主信息) continue;
            auto* fmi = dynamic_cast<特征节点主信息类*>(f->主信息);
            if (!fmi || !fmi->类型) continue;
            Vector3D v3{};
            if (读取向量3(f, v3)) {
                obsV3.push_back({ fmi->类型, v3 });
                continue;
            }

            double s = 0.0;
            if (读取标量为double(f, s)) {
                obsS.push_back({ fmi->类型, s });
                continue;
            }

            const std::vector<std::int64_t>* vv = nullptr;
            if (读取矢量I64指针(f, vv) && vv) {
                obsVec.push_back({ fmi->类型, vv });
                continue;
            }
        }

        //============================================================
        // 1) 构造“观测位置/观测尺寸”的候选表达（向量优先，标量三元兜底）
        //============================================================
        struct 观测向量表达
        {
            enum class Kind { Vec3Type, Scalar3Types };
            Kind kind = Kind::Vec3Type;

            // Vec3Type
            词性节点类* t = nullptr;

            // Scalar3Types
            词性节点类* tx = nullptr;
            词性节点类* ty = nullptr;
            词性节点类* tz = nullptr;

            Vector3D v{};
            bool 尺寸更像 = false; // 用于偏好（全非负 + “看起来像尺寸”）
        };

        auto isFinite3 = [](const Vector3D& a) {
            return std::isfinite(a.x) && std::isfinite(a.y) && std::isfinite(a.z);
            };
        auto allNonNeg = [](const Vector3D& a) {
            return a.x >= 0 && a.y >= 0 && a.z >= 0;
            };
        auto norm2 = [](const Vector3D& a) {
            return a.x * a.x + a.y * a.y + a.z * a.z;
            };

        std::vector<观测向量表达> posCands;
        std::vector<观测向量表达> sizeCands;

        // 1.1 向量特征：全部先纳入（后面组合时会自动淘汰不合适的）
        for (const auto& v3 : obsV3) {
            if (!v3.t || !isFinite3(v3.v)) continue;
            观测向量表达 k;
            k.kind = 观测向量表达::Kind::Vec3Type;
            k.t = v3.t;
            k.v = v3.v;
            k.尺寸更像 = allNonNeg(v3.v); // 尺寸通常全非负
            posCands.push_back(k);
            if (k.尺寸更像) sizeCands.push_back(k);
        }

        // 1.2 标量三元兜底：当没有任何 Vec3 时，尝试用三标量拼 Vector3D
        auto 生成标量三元候选 = [&](bool 偏向尺寸) -> std::vector<观测向量表达>
            {
                std::vector<观测向量表达> r;
                if (obsS.size() < 3) return r;

                // 简单排序：偏向尺寸=>优先小的非负；偏向位置=>优先绝对值大的
                std::vector<观测标量> tmp = obsS;
                if (偏向尺寸) {
                    tmp.erase(std::remove_if(tmp.begin(), tmp.end(), [](const 观测标量& x) { return x.v < 0; }), tmp.end());
                    std::sort(tmp.begin(), tmp.end(), [](auto& a, auto& b) { return a.v < b.v; });
                }
                else {
                    std::sort(tmp.begin(), tmp.end(), [](auto& a, auto& b) { return std::abs(a.v) > std::abs(b.v); });
                }

                const std::size_t N = std::min<std::size_t>(tmp.size(), 8); // 控制组合爆炸
                if (N < 3) return r;

                // 组合 + 6种排列
                std::size_t 产出上限 = 60;
                for (std::size_t i = 0; i < N && r.size() < 产出上限; ++i)
                    for (std::size_t j = i + 1; j < N && r.size() < 产出上限; ++j)
                        for (std::size_t k = j + 1; k < N && r.size() < 产出上限; ++k)
                        {
                            const auto& a = tmp[i];
                            const auto& b = tmp[j];
                            const auto& c = tmp[k];
                            if (!a.t || !b.t || !c.t) continue;
                            if (a.t == b.t || a.t == c.t || b.t == c.t) continue;

                            auto pushPerm = [&](const 观测标量& X, const 观测标量& Y, const 观测标量& Z)
                                {
                                    观测向量表达 e;
                                    e.kind = 观测向量表达::Kind::Scalar3Types;
                                    e.tx = X.t; e.ty = Y.t; e.tz = Z.t;
                                    e.v = Vector3D{ X.v, Y.v, Z.v };
                                    e.尺寸更像 = allNonNeg(e.v);
                                    if (!isFinite3(e.v)) return;
                                    r.push_back(e);
                                };

                            pushPerm(a, b, c);
                            pushPerm(a, c, b);
                            pushPerm(b, a, c);
                            pushPerm(b, c, a);
                            pushPerm(c, a, b);
                            pushPerm(c, b, a);
                        }
                return r;
            };

        if (posCands.empty()) {
            auto extraPos = 生成标量三元候选(false);
            posCands.insert(posCands.end(), extraPos.begin(), extraPos.end());
        }
        if (sizeCands.empty()) {
            auto extraSize = 生成标量三元候选(true);
            sizeCands.insert(sizeCands.end(), extraSize.begin(), extraSize.end());
        }

        if (posCands.empty()) return out; // 没有任何“位置”可用，无法查找

        //============================================================
        // 2) 取候选存在的值：按“观测的类型”去取（类型对齐）
        //============================================================
        auto 取存在向量3_按表达 = [&](存在节点类* e, const 观测向量表达& k, Vector3D& outV) -> bool
            {
                if (!e) return false;

                if (k.kind == 观测向量表达::Kind::Vec3Type) {
                    auto* f = 查找特征_不创建(e, k.t);
                    if (!f) return false;
                    Vector3D v{};
                    if (!读取向量3(f, v)) return false;
                    outV = v;
                    return isFinite3(outV);
                }
                else {
                    auto get1 = [&](词性节点类* t, double& v) -> bool {
                        if (!t) return false;
                        auto* f = 查找特征_不创建(e, t);
                        if (!f) return false;
                        if (!读取标量为double(f, v)) return false;
                        return std::isfinite(v);
                        };
                    double x = 0, y = 0, z = 0;
                    if (!get1(k.tx, x) || !get1(k.ty, y) || !get1(k.tz, z)) return false;
                    outV = Vector3D{ x, y, z };
                    return isFinite3(outV);
                }
            };

        auto 计算矢量相似度_汉明 = [](const std::vector<std::int64_t>& A, const std::vector<std::int64_t>& B) -> double
            {
                const std::size_t n = std::min(A.size(), B.size());
                const std::size_t m = std::max(A.size(), B.size());
                if (m == 0) return 0.0;

                std::size_t diff = 0;
                for (std::size_t i = 0; i < n; ++i) if (A[i] != B[i]) ++diff;
                diff += (m - n); // 长度差也计入差异
                const double sim = 1.0 - (static_cast<double>(diff) / static_cast<double>(m));
                return std::clamp(sim, 0.0, 1.0);
            };

        //============================================================
        // 3) 遍历候选存在：先“空间范围”过滤，再“尺寸/轮廓”打分排序
        //============================================================
        struct 候选评分 { 存在节点类* e = nullptr; double score = 0.0; };
        std::vector<候选评分> scored;
        scored.reserve(候选存在.size());

        // 参数（你后续可以挪到配置结构体里）
        const double AABB放大倍数 = 1.20;  // 候选存在自身AABB放大一点（鲁棒）
        const double 通过阈值 = 2.0;        // 越小越严格（建议 1.0~3.0）
        const std::size_t 最大返回 = 8;

        for (auto* e : 候选存在) {
            if (!e || !e->主信息) continue;
            auto* emi = dynamic_cast<存在节点主信息类*>(e->主信息);
            if (!emi) continue;

            double bestScore = std::numeric_limits<double>::infinity();
            bool   bestOk = false;

            for (const auto& posK : posCands) {
                Vector3D obsPos{}, candPos{};
                if (!isFinite3(posK.v)) continue;
                obsPos = posK.v;

                if (!取存在向量3_按表达(e, posK, candPos)) continue;

                // size：允许缺失（缺失时就用“距离阈值”筛）
                bool hasSize = false;
                Vector3D obsSize{}, candSize{};

                // 让 size 候选优先走 “更像尺寸”的（全非负）表达
                for (int pass = 0; pass < 2 && !hasSize; ++pass) {
                    for (const auto& sizeK : sizeCands) {
                        if (pass == 0 && !sizeK.尺寸更像) continue;
                        if (pass == 1 && sizeK.尺寸更像) continue;

                        obsSize = sizeK.v;
                        if (!isFinite3(obsSize) || !allNonNeg(obsSize)) continue;

                        if (!取存在向量3_按表达(e, sizeK, candSize)) continue;
                        if (!allNonNeg(candSize)) continue;

                        hasSize = true;
                        break;
                    }
                }

                // ---------- 先做空间过滤 ----------
                bool 空间通过 = false;
                double posDist = std::sqrt(norm2(Vector3D{ candPos.x - obsPos.x, candPos.y - obsPos.y, candPos.z - obsPos.z }));

                if (hasSize) {
                    auto obsBox = 结构体_AABB3D::由中心尺寸(obsPos, obsSize, 1.0);
                    auto candBox = 结构体_AABB3D::由中心尺寸(candPos, candSize, std::max(1.0, AABB放大倍数));
                    空间通过 = candBox.相交(obsBox);
                }
                else {
                    // 没尺寸：退化为“距离阈值”
                    空间通过 = (posDist <= 0.20); // 单位依你坐标系（米/厘米/毫米）调整
                }
                if (!空间通过) continue;

                // ---------- 再打分 ----------
                // 位置归一化：用尺寸尺度归一化（尺寸缺失则直接用距离）
                double posN = posDist;
                if (hasSize) {
                    const double s = std::max(1e-6, std::min({ obsSize.x, obsSize.y, obsSize.z, candSize.x, candSize.y, candSize.z }));
                    posN = posDist / s;
                }

                // 尺寸差（归一化）
                double sizeN = 0.0;
                if (hasSize) {
                    const Vector3D d{ candSize.x - obsSize.x, candSize.y - obsSize.y, candSize.z - obsSize.z };
                    const double dn = std::sqrt(norm2(d));
                    const double sn = std::sqrt(std::max(1e-6, norm2(obsSize)));
                    sizeN = dn / sn;
                }

                // 轮廓相似度：取“观测轮廓”与“存在历史轮廓(同类型)”的最大相似度
                double contourCost = 0.0;
                bool hasContour = false;
                if (!obsVec.empty() && !emi->特征索引.empty()) {
                    double bestSim = 0.0;

                    for (const auto& ov : obsVec) {
                        if (!ov.t || !ov.v) continue;

                        for (auto* f : emi->特征索引) {
                            auto* fmi = f ? dynamic_cast<特征节点主信息类*>(f->主信息) : nullptr;
                            if (!fmi || fmi->类型 != ov.t) continue;

                            const std::vector<std::int64_t>* vv = nullptr;
                            if (!读取矢量I64指针(f, vv) || !vv) continue;

                            bestSim = std::max(bestSim, 计算矢量相似度_汉明(*ov.v, *vv));
                            hasContour = true;
                        }
                    }

                    if (hasContour) contourCost = 1.0 - bestSim;
                }

                // 权重：位置最重要；尺寸次之；轮廓用于“同位置多物体”细分
                double wPos = 0.65;
                double wSize = hasSize ? 0.25 : 0.0;
                double wCon = hasContour ? 0.10 : 0.0;
                const double wSum = std::max(1e-9, wPos + wSize + wCon);
                wPos /= wSum; wSize /= wSum; wCon /= wSum;

                const double score = wPos * posN + wSize * sizeN + wCon * contourCost;

                if (score < bestScore) {
                    bestScore = score;
                    bestOk = true;
                }
            }

            if (bestOk && bestScore < 通过阈值) {
                scored.push_back({ e, bestScore });
            }
        }

        if (scored.empty()) return out;

        std::sort(scored.begin(), scored.end(),
            [](const 候选评分& a, const 候选评分& b) { return a.score < b.score; });

        const std::size_t take = std::min<std::size_t>(scored.size(), 最大返回);
        out.reserve(take);
        for (std::size_t i = 0; i < take; ++i) out.push_back(scored[i].e);
        return out;
    }
    // 返回：按“最可能同一存在”排序的候选列表（score 越小越靠前）
    std::vector<存在节点类*> 查找存在_按观测信息_抽象存在版(场景节点类* 场景, 存在节点主信息类* 观测MI) const
    {
        std::vector<存在节点类*> out;
        if (!场景 || !观测MI) return out;

        auto* smi = 取场景主信息(场景);
        if (!smi) return out;

        // 索引为空时兜底
        if (smi->存在索引.empty() && 场景->子) {
            重建索引_从子链(场景);
        }
        if (smi->存在索引.empty()) return out;

        //============================================================
        // 0) 找到“观测特征”模板，并读出其子链里的“观测特征类型列表（按顺序）”
        //============================================================

        auto 取词字符串 = [](词性节点类* 词节点) -> std::string {
            if (!词节点 || !词节点->主信息) return {};
            if (auto* w = dynamic_cast<词主信息类*>(词节点->主信息)) return w->词;
            return {};
            };

        // 从某个父节点的子链里找“名称 == 观测特征”的节点；找不到就用“结构特征”兜底
        auto 找观测特征节点 = [&](存在节点类* 抽象存在) -> 基础信息节点类*
            {
                if (!抽象存在 || !抽象存在->子) return nullptr;

                基础信息节点类* bestByStructure = nullptr;

                auto* cur = static_cast<基础信息节点类*>(抽象存在->子);
                do {
                    if (!cur || !cur->主信息) break;

                    // 先按名字精确找
                    if (auto* bmi = dynamic_cast<基础信息基类*>(cur->主信息)) {
                        if (bmi->名称 && 取词字符串(bmi->名称) == "观测特征") {
                            return cur;
                        }
                    }

                    // 结构兜底：子链里大多是“特征节点(值为空)”
                    if (cur->子) {
                        int total = 0, featNull = 0;
                        auto* c2 = static_cast<基础信息节点类*>(cur->子);
                        do {
                            ++total;
                            if (auto* fmi = dynamic_cast<特征节点主信息类*>(c2->主信息)) {
                                if (fmi->值 == nullptr && fmi->类型 != nullptr) ++featNull;
                            }
                            c2 = static_cast<基础信息节点类*>(c2->下);
                        } while (c2 && c2 != cur->子);

                        if (total >= 2 && featNull >= total) bestByStructure = cur;
                    }

                    cur = static_cast<基础信息节点类*>(cur->下);
                } while (cur && cur != 抽象存在->子);

                return bestByStructure;
            };

        std::vector<词性节点类*> 观测类型顺序; // [绝对坐标, 相对坐标, 尺寸, ...轮廓/颜色等]
        {
            存在节点类* 抽象存在 = 观测MI->概念模板; // 你说初始化时定义的“抽象存在节点”，建议挂在这里
            基础信息节点类* 观测特征节点 = 找观测特征节点(抽象存在);

            if (观测特征节点 && 观测特征节点->子) {
                auto* f = static_cast<基础信息节点类*>(观测特征节点->子);
                do {
                    if (auto* fmi = dynamic_cast<特征节点主信息类*>(f->主信息)) {
                        if (fmi->类型) 观测类型顺序.push_back(fmi->类型);
                    }
                    f = static_cast<基础信息节点类*>(f->下);
                } while (f && f != 观测特征节点->子);
            }
        }

        // 模板缺失：退化为“直接用观测MI里现有特征”——但仍按“先位置后尺寸后轮廓”处理
        if (观测类型顺序.size() < 3) {
            // 尽量保证前 3 个是：abs/rel/size（如果你观测时就是按这个顺序挂的，这里就能工作）
            for (auto* f : (!观测MI->特征当前状态.empty() ? 观测MI->特征当前状态 : 观测MI->特征索引)) {
                auto* fmi = f ? dynamic_cast<特征节点主信息类*>(f->主信息) : nullptr;
                if (fmi && fmi->类型) 观测类型顺序.push_back(fmi->类型);
            }
        }

        if (观测类型顺序.size() < 3) return out;

        //============================================================
        // 1) 决定用“绝对坐标 or 相对坐标”
        //============================================================

        const bool 场景是内部世界 = (smi->宿主存在 != nullptr);
        词性节点类* 绝对坐标类型 = 观测类型顺序[0];
        词性节点类* 相对坐标类型 = 观测类型顺序[1];
        词性节点类* 尺寸类型 = 观测类型顺序[2];

        词性节点类* 使用坐标类型 = 场景是内部世界 ? 相对坐标类型 : 绝对坐标类型;

        //============================================================
        // 2) 从“观测MI”取出：坐标/尺寸/轮廓(可选)
        //============================================================

        auto 取观测特征节点 = [&](词性节点类* t) -> 特征节点类*
            {
                if (!t) return nullptr;
                if (auto* f = 观测MI->查找特征当前(t)) return f;
                for (auto* x : 观测MI->特征索引) {
                    auto* xmi = x ? dynamic_cast<特征节点主信息类*>(x->主信息) : nullptr;
                    if (xmi && xmi->类型 == t) return x;
                }
                return nullptr;
            };

        auto 取观测向量3 = [&](词性节点类* t, Vector3D& outV) -> bool
            {
                auto* f = 取观测特征节点(t);
                if (!f) return false;
                Vector3D v{};
                if (!读取向量3(f, v)) return false;
                outV = v;
                return std::isfinite(outV.x) && std::isfinite(outV.y) && std::isfinite(outV.z);
            };

        auto 取观测矢量 = [&](词性节点类* t, const std::vector<std::int64_t>*& outPtr) -> bool
            {
                auto* f = 取观测特征节点(t);
                if (!f) return false;
                const std::vector<std::int64_t>* v = nullptr;
                if (!读取矢量I64指针(f, v)) return false;
                outPtr = v;
                return true;
            };

        Vector3D obsPos{}, obsSize{};
        bool hasPos = 取观测向量3(使用坐标类型, obsPos);

        // 坐标缺失时：用另一套坐标兜底（避免某些帧只填了 abs 或 rel）
        if (!hasPos) {
            词性节点类* 备用 = (使用坐标类型 == 绝对坐标类型) ? 相对坐标类型 : 绝对坐标类型;
            hasPos = 取观测向量3(备用, obsPos);
        }

        bool hasSize = 取观测向量3(尺寸类型, obsSize);

        if (!hasPos || !hasSize) return out;

        // 观测轮廓/其它矢量特征：从“尺寸之后”开始收集（按模板顺序）
        struct ObsContour { 词性节点类* t; const std::vector<std::int64_t>* v; };
        std::vector<ObsContour> obsContours;
        for (std::size_t i = 3; i < 观测类型顺序.size(); ++i) {
            const std::vector<std::int64_t>* ptr = nullptr;
            if (取观测矢量(观测类型顺序[i], ptr) && ptr) {
                obsContours.push_back({ 观测类型顺序[i], ptr });
            }
        }

        //============================================================
        // 3) 遍历场景存在：AABB 门控 + 打分排序
        //============================================================

        auto norm2 = [](const Vector3D& a) { return a.x * a.x + a.y * a.y + a.z * a.z; };
        auto dist = [&](const Vector3D& a, const Vector3D& b) {
            const Vector3D d{ a.x - b.x, a.y - b.y, a.z - b.z };
            return std::sqrt(norm2(d));
            };

        auto hammingSim = [](const std::vector<std::int64_t>& A, const std::vector<std::int64_t>& B) -> double
            {
                const std::size_t n = std::min(A.size(), B.size());
                const std::size_t m = std::max(A.size(), B.size());
                if (m == 0) return 0.0;
                std::size_t diff = 0;
                for (std::size_t i = 0; i < n; ++i) if (A[i] != B[i]) ++diff;
                diff += (m - n);
                double sim = 1.0 - (static_cast<double>(diff) / static_cast<double>(m));
                if (sim < 0.0) sim = 0.0;
                if (sim > 1.0) sim = 1.0;
                return sim;
            };

        // 参数（可以后抽成配置）
        const double candExpand = 1.20;     // 候选存在 AABB 放大倍数
        const double scoreTh = 2.0;      // 分数阈值（越小越严格）
        const std::size_t topK = 8;

        struct Cand { 存在节点类* e; double score; };
        std::vector<Cand> scored;
        scored.reserve(smi->存在索引.size());

        for (auto* e : smi->存在索引) {
            if (!e || !e->主信息) continue;
            if (e->主信息 == 观测MI) continue; // 避免“把自己匹配到自己”（如果观测节点已挂到场景）

            auto* emi = dynamic_cast<存在节点主信息类*>(e->主信息);
            if (!emi) continue;

            // 取候选存在的坐标/尺寸（按同一特征类型）
            Vector3D candPos{}, candSize{};
            bool gotPos = false, gotSize = false;

            if (auto* f = 查找特征_不创建(e, 使用坐标类型)) {
                Vector3D v{};
                if (读取向量3(f, v)) { candPos = v; gotPos = true; }
            }
            if (!gotPos) {
                // 兜底：另一套坐标
                词性节点类* 备用 = (使用坐标类型 == 绝对坐标类型) ? 相对坐标类型 : 绝对坐标类型;
                if (auto* f = 查找特征_不创建(e, 备用)) {
                    Vector3D v{};
                    if (读取向量3(f, v)) { candPos = v; gotPos = true; }
                }
            }
            if (auto* f = 查找特征_不创建(e, 尺寸类型)) {
                Vector3D v{};
                if (读取向量3(f, v)) { candSize = v; gotSize = true; }
            }

            if (!gotPos || !gotSize) continue;

            // ---------- 门控：AABB 相交 ----------
            const auto obsBox = 结构体_AABB3D::由中心尺寸(obsPos, obsSize, 1.0);
            const auto candBox = 结构体_AABB3D::由中心尺寸(candPos, candSize, std::max(1.0, candExpand));
            if (!candBox.相交(obsBox)) continue;

            // ---------- 打分 ----------
            const double dp = dist(obsPos, candPos);

            // 用“尺寸尺度”归一化位置距离
            const double sScale = std::max(1e-6, std::sqrt(std::max(1e-12, norm2(obsSize))));
            const double posCost = dp / sScale;

            const Vector3D dS{ candSize.x - obsSize.x, candSize.y - obsSize.y, candSize.z - obsSize.z };
            const double sizeCost = std::sqrt(norm2(dS)) / sScale;

            // 轮廓：观测轮廓 vs 候选存在历史轮廓（同类型多值取 max sim）
            double contourCost = 0.0;
            bool hasContour = false;
            if (!obsContours.empty() && emi->特征索引.size() > 0) {
                double bestSimAll = 0.0;
                for (const auto& oc : obsContours) {
                    double bestSimType = 0.0;
                    for (auto* fx : emi->特征索引) {
                        auto* fxmi = fx ? dynamic_cast<特征节点主信息类*>(fx->主信息) : nullptr;
                        if (!fxmi || fxmi->类型 != oc.t) continue;
                        const std::vector<std::int64_t>* vv = nullptr;
                        if (!读取矢量I64指针(fx, vv) || !vv) continue;
                        bestSimType = std::max(bestSimType, hammingSim(*oc.v, *vv));
                    }
                    bestSimAll = std::max(bestSimAll, bestSimType);
                }
                hasContour = true;
                contourCost = 1.0 - bestSimAll;
            }

            double wPos = 0.65;
            double wSize = 0.25;
            double wCon = hasContour ? 0.10 : 0.0;
            const double wSum = std::max(1e-9, wPos + wSize + wCon);
            wPos /= wSum; wSize /= wSum; wCon /= wSum;

            double score = wPos * posCost + wSize * sizeCost + wCon * contourCost;

            // 小幅偏好“更稳定/命中更多”的存在（可选但很实用）
            score *= (1.0 - std::min(0.20, static_cast<double>(emi->命中次数) * 0.001));

            if (score < scoreTh) {
                scored.push_back({ e, score });
            }
        }

        if (scored.empty()) return out;

        std::sort(scored.begin(), scored.end(),
            [](const Cand& a, const Cand& b) { return a.score < b.score; });

        const std::size_t take = std::min(topK, scored.size());
        out.reserve(take);
        for (std::size_t i = 0; i < take; ++i) out.push_back(scored[i].e);
        return out;
    }
    存在节点类* 查找存在_存在主信息(场景节点类* 场景, 存在节点主信息类* 待查主信息)
    {
        if (!场景 || !待查主信息) return nullptr;

        auto* smi = 取场景主信息(场景);
        if (!smi) return nullptr;

        // 索引为空时兜底重建（避免“场景里有子节点但索引没维护”）
        if (smi->存在索引.empty() && 场景->子) {
            重建索引_从子链(场景);
        }
        if (smi->存在索引.empty()) return nullptr;

        //============================================================
        // 0) 收集“待查主信息”的特征（优先当前状态）
        //============================================================
        struct Q标量 { 词性节点类* t = nullptr; double v = 0.0; };
        struct QVector3 { 词性节点类* t = nullptr; Vector3D v{}; };
        struct Q矢量 { 词性节点类* t = nullptr; const std::vector<std::int64_t>* v = nullptr; };

        std::vector<Q标量> qS;
        std::vector<QVector3> qV3;
        std::vector<Q矢量>  qVec;

        const auto& qFeatures = (!待查主信息->特征当前状态.empty())
            ? 待查主信息->特征当前状态
            : 待查主信息->特征索引;

        for (auto* f : qFeatures) {
            if (!f || !f->主信息) continue;
            auto* fmi = dynamic_cast<特征节点主信息类*>(f->主信息);
            if (!fmi || !fmi->类型) continue;
            Vector3D v3{};
            if (读取向量3(f, v3)) {
                if (std::isfinite(v3.x) && std::isfinite(v3.y) && std::isfinite(v3.z)) {
                    qV3.push_back({ fmi->类型, v3 });
                }
                continue;
            }

            double s = 0.0;
            if (读取标量为double(f, s)) {
                qS.push_back({ fmi->类型, s });
                continue;
            }

            const std::vector<std::int64_t>* vv = nullptr;
            if (读取矢量I64指针(f, vv) && vv) {
                qVec.push_back({ fmi->类型, vv });
                continue;
            }
        }

        // 没有任何可用于对齐的特征
        if (qS.empty() && qV3.empty() && qVec.empty()) return nullptr;

        //============================================================
        // 1) 相似度/代价工具
        //============================================================
        auto norm2 = [](const Vector3D& a) { return a.x * a.x + a.y * a.y + a.z * a.z; };

        auto dist = [&](const Vector3D& a, const Vector3D& b) {
            const Vector3D d{ a.x - b.x, a.y - b.y, a.z - b.z };
            return std::sqrt(norm2(d));
            };

        auto hammingSim = [](const std::vector<std::int64_t>& A, const std::vector<std::int64_t>& B) -> double
            {
                const std::size_t n = std::min(A.size(), B.size());
                const std::size_t m = std::max(A.size(), B.size());
                if (m == 0) return 0.0;

                std::size_t diff = 0;
                for (std::size_t i = 0; i < n; ++i) if (A[i] != B[i]) ++diff;
                diff += (m - n);

                double sim = 1.0 - (static_cast<double>(diff) / static_cast<double>(m));
                return std::clamp(sim, 0.0, 1.0);
            };

        //============================================================
        // 2) 遍历候选存在：对齐类型 -> 计算 score -> 取最小
        //============================================================
        struct Cand { 存在节点类* e = nullptr; double score = 0.0; };
        Cand best{ nullptr, std::numeric_limits<double>::infinity() };

        // 你可以按需要调整阈值：越小越严格
        const double scoreTh = 2.0;

        for (auto* e : smi->存在索引) {
            if (!e || !e->主信息) continue;

            // 避免“自己匹配到自己”（如果待查主信息就是场景里某存在的主信息）
            if (e->主信息 == 待查主信息) return e;

            auto* emi = dynamic_cast<存在节点主信息类*>(e->主信息);
            if (!emi) continue;

            // 命中次数：用于“偏好稳定对象”
            const double hit = static_cast<double>(emi->命中次数);

            // 统计匹配情况
            int matched = 0;
            int missing = 0;

            double sumCostV3 = 0.0; int cntV3 = 0;
            double sumCostS = 0.0; int cntS = 0;
            double sumCostVec = 0.0; int cntVec = 0;

            // ---- Vec3 特征：用归一化距离 ----
            for (const auto& q : qV3) {
                if (!q.t) continue;
                auto* f = 查找特征_不创建(e, q.t);
                if (!f) { ++missing; continue; }

                Vector3D cand{};
                if (!读取向量3(f, cand)) { ++missing; continue; }

                if (!std::isfinite(cand.x) || !std::isfinite(cand.y) || !std::isfinite(cand.z)) { ++missing; continue; }

                const double d = dist(q.v, cand);

                // 归一化尺度：用“待查向量长度”兜底（避免不同单位时太敏感）
                const double scale = std::max(1e-6, std::sqrt(std::max(1e-12, norm2(q.v))));
                const double cost = d / scale;

                sumCostV3 += cost;
                ++cntV3;
                ++matched;
            }

            // ---- 标量特征：用相对差 ----
            for (const auto& q : qS) {
                if (!q.t) continue;
                auto* f = 查找特征_不创建(e, q.t);
                if (!f) { ++missing; continue; }

                double cand = 0.0;
                if (!读取标量为double(f, cand)) { ++missing; continue; }

                const double denom = std::max(1.0, (std::abs(q.v) + std::abs(cand)) * 0.5); // 相对尺度
                const double cost = std::abs(q.v - cand) / denom;

                sumCostS += cost;
                ++cntS;
                ++matched;
            }

            // ---- 矢量(int64数组)：用汉明相似度 ----
            for (const auto& q : qVec) {
                if (!q.t || !q.v) continue;
                auto* f = 查找特征_不创建(e, q.t);
                if (!f) { ++missing; continue; }

                const std::vector<std::int64_t>* vv = nullptr;
                if (!读取矢量I64指针(f, vv) || !vv) { ++missing; continue; }

                const double sim = hammingSim(*q.v, *vv);
                const double cost = 1.0 - sim;

                sumCostVec += cost;
                ++cntVec;
                ++matched;
            }

            if (matched <= 0) continue; // 一个类型都对不上，直接跳过

            // 平均代价
            const double avgV3 = (cntV3 ? (sumCostV3 / cntV3) : 0.0);
            const double avgS = (cntS ? (sumCostS / cntS) : 0.0);
            const double avgVec = (cntVec ? (sumCostVec / cntVec) : 0.0);

            // 三类权重：Vec3 更重要；矢量用于细分；标量用于补强
            double wV3 = (cntV3 ? 0.60 : 0.0);
            double wS = (cntS ? 0.25 : 0.0);
            double wVe = (cntVec ? 0.15 : 0.0);
            const double wSum = std::max(1e-9, wV3 + wS + wVe);
            wV3 /= wSum; wS /= wSum; wVe /= wSum;

            double score = wV3 * avgV3 + wS * avgS + wVe * avgVec;

            // 缺失惩罚：缺失越多越差（避免仅凭少量特征误匹配）
            if (missing > 0) {
                const double missRatio = static_cast<double>(missing) / static_cast<double>(missing + matched);
                score += 0.75 * missRatio; // 你可按实际效果调 0.5~1.5
            }

            // 命中次数加权：命中多的更“可信/稳定”，让 score 略微变小
            // 这里用 log1p 防止 hit 特别大时过度偏置
            const double prefer = std::min(0.25, std::log1p(std::max(0.0, hit)) * 0.03);
            score *= (1.0 - prefer);

            if (score < best.score) {
                best = { e, score };
            }
        }

        if (!best.e) return nullptr;
        if (best.score >= scoreTh) return nullptr;
        return best.e;
    }

    //============================================================
    // 6) 存档/重载：导出/修复（只处理指针引用与索引；节点树结构由外部负责重建）
    //============================================================

    场景节点存档 导出存档(const 场景节点类* 场景) const
    {
        场景节点存档 out{};
        if (!场景) return out;

        out.主键 = 场景->主键;
        out.父场景ID = (场景->父 ? 场景->父->主键 : std::string{});

        auto* smi = 取场景主信息(场景);
        if (!smi) return out;

        out.创建时间 = smi->创建时间;
        out.最后观测时间 = smi->最后观测时间;
        out.命中次数 = smi->命中次数;
        out.宿主存在ID = (smi->宿主存在 ? smi->宿主存在->主键 : std::string{});

        // 子场景/存在/特征：优先用索引；索引为空时可扫描子链补齐
        if (!smi->子场景索引.empty()) {
            for (auto* x : smi->子场景索引) if (x) out.子场景IDs.push_back(x->主键);
        }
        if (!smi->存在索引.empty()) {
            for (auto* x : smi->存在索引) if (x) out.子存在IDs.push_back(x->主键);
        }
        if (!smi->特征索引.empty()) {
            for (auto* x : smi->特征索引) if (x) out.特征IDs.push_back(x->主键);
        }

        if (out.子场景IDs.empty() && out.子存在IDs.empty() && out.特征IDs.empty() && 场景->子) {
            // 兜底：扫子链
            auto* cur = static_cast<基础信息节点类*>(场景->子);
            do {
                if (cur && cur->主信息) {
                    if (dynamic_cast<场景节点主信息类*>(cur->主信息)) out.子场景IDs.push_back(cur->主键);
                    else if (dynamic_cast<存在节点主信息类*>(cur->主信息)) out.子存在IDs.push_back(cur->主键);
                    else if (dynamic_cast<特征节点主信息类*>(cur->主信息)) out.特征IDs.push_back(cur->主键);
                }
                cur = static_cast<基础信息节点类*>(cur->下);
            } while (cur && cur != 场景->子);
        }

        return out;
    }

    // 注意：此函数只“修复指针/索引缓存”，不负责“把子节点重新挂回子链”
    void 从存档修复引用(场景节点类* 场景, const 场景节点存档& in) const
    {
        if (!场景 || !场景->主信息) return;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return;

        smi->创建时间 = in.创建时间;
        smi->最后观测时间 = in.最后观测时间;
        smi->命中次数 = in.命中次数;

        // 宿主存在
        if (!in.宿主存在ID.empty()) {
            if (auto* n = 世界链.查找主键(in.宿主存在ID)) {
                smi->宿主存在 = static_cast<存在节点类*>(n);
                // 写回宿主->内部世界（若一致）
                if (smi->宿主存在 && smi->宿主存在->主信息) {
                    if (auto* emi = dynamic_cast<存在节点主信息类*>(smi->宿主存在->主信息)) {
                        if (!emi->内部世界) emi->内部世界 = 场景;
                    }
                }
            }
        }
        else {
            smi->宿主存在 = nullptr;
        }

        // 索引恢复（按ID解析；失败就跳过）
        smi->子场景索引.clear();
        smi->存在索引.clear();
        smi->特征索引.clear();

        for (const auto& id : in.子场景IDs) {
            if (id.empty()) continue;
            if (auto* n = 世界链.查找主键(id)) smi->子场景索引.push_back(static_cast<场景节点类*>(n));
        }
        for (const auto& id : in.子存在IDs) {
            if (id.empty()) continue;
            if (auto* n = 世界链.查找主键(id)) smi->存在索引.push_back(static_cast<存在节点类*>(n));
        }
        for (const auto& id : in.特征IDs) {
            if (id.empty()) continue;
            if (auto* n = 世界链.查找主键(id)) smi->特征索引.push_back(static_cast<特征节点类*>(n));
        }

        smi->整理();
    }

//private:
    //============================================================
    // 内部工具
    //============================================================

    static 场景节点主信息类* 取场景主信息(const 场景节点类* n)
    {
        if (!n || !n->主信息) return nullptr;
        return dynamic_cast<场景节点主信息类*>(n->主信息);
    }

    static 特征节点主信息类* 取特征主信息(const 特征节点类* n)
    {
        if (!n || !n->主信息) return nullptr;
        return dynamic_cast<特征节点主信息类*>(n->主信息);
    }

    static 特征值主信息类* 取特征值主信息(const 特征节点类* n)
    {
        auto* fmi = 取特征主信息(n);
        if (!fmi || !fmi->值 || !fmi->值->主信息) return nullptr;
        return fmi->值->主信息;
    }

    static bool 读取矢量I64指针(const 特征节点类* n, const std::vector<std::int64_t>*& outPtr)
    {
        outPtr = nullptr;
        auto* vmi = 取特征值主信息(n);
        if (!vmi) return false;
        if (vmi->类型 != 枚举_特征值类型::VecI64) return false;

        auto* p = std::get_if<std::vector<std::int64_t>>(&vmi->值);
        if (!p) return false;

        outPtr = p;
        return true;
    }

    static bool 读取向量3(const 特征节点类* n, Vector3D& outV)
    {
        outV = Vector3D{};
        const std::vector<std::int64_t>* p = nullptr;
        if (!读取矢量I64指针(n, p) || !p) return false;
        if (p->size() < 3) return false;

        outV.x = static_cast<double>((*p)[0]);
        outV.y = static_cast<double>((*p)[1]);
        outV.z = static_cast<double>((*p)[2]);

        return std::isfinite(outV.x) && std::isfinite(outV.y) && std::isfinite(outV.z);
    }

    static bool 读取标量为double(const 特征节点类* n, double& out)
    {
        out = 0.0;

        std::int64_t i = 0;
        if (特征集.读取标量特征值(n, i)) {
            out = static_cast<double>(i);
            return std::isfinite(out);
        }

        std::uint64_t u = 0;
        if (特征集.读取无符号标量特征值(n, u)) {
            out = static_cast<double>(u);
            return std::isfinite(out);
        }

        double d = 0.0;
        if (特征集.读取浮点特征值(n, d)) {
            out = d;
            return std::isfinite(out);
        }

        return false;
    }

    static 特征节点类* 查找特征_不创建(基础信息节点类* 所属, 词性节点类* 类型)
    {
        if (!所属 || !所属->主信息 || !类型) return nullptr;

        auto findInList = [&](const std::vector<特征节点类*>& list) -> 特征节点类*
            {
                for (auto* f : list) {
                    auto* fmi = f ? dynamic_cast<特征节点主信息类*>(f->主信息) : nullptr;
                    if (fmi && fmi->类型 == 类型) return f;
                }
                return nullptr;
            };

        if (auto* emi = dynamic_cast<存在节点主信息类*>(所属->主信息)) {
            if (auto* f = emi->查找特征当前(类型)) return f;
            if (auto* f = findInList(emi->特征索引)) return f;
        }

        if (auto* smi = dynamic_cast<场景节点主信息类*>(所属->主信息)) {
            if (auto* f = findInList(smi->特征索引)) return f;
        }

        // 兜底：扫描子链（可能未维护索引）
        if (!所属->子) return nullptr;
        auto* cur = static_cast<基础信息节点类*>(所属->子);
        do {
            auto* fmi = cur ? dynamic_cast<特征节点主信息类*>(cur->主信息) : nullptr;
            if (fmi && fmi->类型 == 类型) return static_cast<特征节点类*>(cur);
            cur = cur ? static_cast<基础信息节点类*>(cur->下) : nullptr;
        } while (cur && cur != 所属->子);

        return nullptr;
    }


    static bool 取存在空间_中心尺寸(const 存在节点类* e, const 结构体_场景存在匹配参数& p, Vector3D& out中心, Vector3D& out尺寸)
    {
        if (!e || !e->主信息) return false;

        bool gotPos = false;
        if (p.位置向量特征类型) {
            if (auto* f = 查找特征_不创建(const_cast<存在节点类*>(e), p.位置向量特征类型)) {
                Vector3D v{};
                if (读取向量3(f, v)) { out中心 = v; gotPos = true; }
            }
        }
        if (!gotPos) {
            if (!取标量三元(e, p.位置X特征类型, p.位置Y特征类型, p.位置Z特征类型, out中心)) return false;
        }

        bool gotSize = false;
        if (p.尺寸向量特征类型) {
            if (auto* f = 查找特征_不创建(const_cast<存在节点类*>(e), p.尺寸向量特征类型)) {
                Vector3D v{};
                if (读取向量3(f, v)) { out尺寸 = v; gotSize = true; }
            }
        }
        if (!gotSize) {
            // 尺寸缺失：可视为 0 尺寸，但这样 AABB 相交几乎退化；这里选择失败
            if (!取标量三元(e, p.尺寸X特征类型, p.尺寸Y特征类型, p.尺寸Z特征类型, out尺寸)) return false;
        }

        return true;
    }

    static bool 取标量三元(const 存在节点类* e, 词性节点类* tx, 词性节点类* ty, 词性节点类* tz, Vector3D& out)
    {
        if (!tx || !ty || !tz) return false;

        auto get1 = [&](词性节点类* t, double& v) -> bool {
            if (!t) return false;
            auto* f = 查找特征_不创建(const_cast<存在节点类*>(e), t);
            if (!f) return false;
            if (!读取标量为double(f, v)) return false;
            return true;
            };

        double x = 0, y = 0, z = 0;
        if (!get1(tx, x) || !get1(ty, y) || !get1(tz, z)) return false;
        out = Vector3D{ x, y, z };
        return true;
    }

    // 重要：链表模板::删除节点 不会自动维护 parent->子 指针。
    static void 修正父节点子指针_在删除前(基础信息节点类* target)
    {
        if (!target) return;
        auto* parent = target->父;
        if (!parent) return;
        if (parent->子 != target) return;

        // 目标是唯一子节点
        if (target->下 == target) {
            parent->子 = nullptr;
            return;
        }

        // 还有其它兄弟：让 parent->子 指向一个仍存活的兄弟
        parent->子 = target->下;
    }
};


