export module 三维场景管模块;

import <cmath>;
import <vector>;
import <algorithm>;
import <limits>;
import <cstdint>;
import 基础数据类型模块;
import 主信息定义模块;
import 世界树模块;
import 语素模块;
import 相机接口模块;
import 宇宙环境模块;
import 宇宙链模块;
import 特征值模块;  // 新增：用于特征值集



// ========== 场景3D管理器 ==========
export class 三维场景管理类 {
public:
    三维场景管理类(世界树类& 世界树, 场景节点类* 当前场景)
        : 世界树(世界树), 当前场景(当前场景) {    }

    // 将一帧所有存在观测融合进入场景
    void 融合存在观测列表(const std::vector<存在观测>& 列表) {

        for (const auto& obs : 列表) {
            存在节点类* 存在节点 = 找到或创建存在(obs);
            if (存在节点) {
                更新存在信息(存在节点, obs);
            }
        }
    }

private:
    世界树类& 世界树;
    场景节点类* 当前场景 = nullptr;

    // ========= 查找或创建存在 =========
    存在节点类* 找到或创建存在(const 存在观测& 观测);

    // ========= 更新存在动态信息 =========
    void 更新存在信息(存在节点类* 存在, const 存在观测& 观测);

    // ========= 相似度（预留扩展）=========
    double 计算相似度(const 存在观测& o, 存在节点类* 已知存在);

    // ========= 辅助函数 =========

    // 检查某个轴特征的所有标量值是否至少有一个落在 [min, max] 范围内
    static bool 轴特征值在范围内(存在节点类* 存在, 词性节点类* 类型, double minVal, double maxVal) {
        特征节点类* feat = g_宇宙.世界树.查找特征节点(存在, 类型);
        if (!feat) return false;

        auto* info = dynamic_cast<特征节点主信息类*>(feat->主信息);
        if (!info || !info->值) return false;

        特征值节点类* val = info->值;
        do {
            if (auto* scalar = dynamic_cast<标量特征值主信息类*>(val->主信息)) {
                double v = scalar->值 / 1000.0;  // 存储为毫米整数
                if (v >= minVal && v <= maxVal) return true;
            }
            val = val->下;
        } while (val != info->值);

        return false;
    }

    // 取该轴所有特征值中距离目标最近的一个值
    static void 轴取最近值(存在节点类* 存在, 词性节点类* 类型, double 目标, double& 出值) {
        特征节点类* feat = g_宇宙.世界树.查找特征节点(存在, 类型);
        if (!feat) return;

        auto* info = dynamic_cast<特征节点主信息类*>(feat->主信息);
        if (!info || !info->值) return;

        double bestDiff = std::numeric_limits<double>::infinity();
        特征值节点类* val = info->值;
        do {
            if (auto* scalar = dynamic_cast<标量特征值主信息类*>(val->主信息)) {
                double v = scalar->值 / 1000.0;
                double diff = std::abs(v - 目标);
                if (diff < bestDiff) {
                    bestDiff = diff;
                    出值 = v;
                }
            }
            val = val->下;
        } while (val != info->值);
    }

    // 判断是否存在中是否已有完全相同的轮廓值节点（指针比较）
    static bool 存在已有相同轮廓值(存在节点类* 存在, 词性节点类* 类型, 特征值节点类* 目标轮廓值节点) {
        特征节点类* feat = g_宇宙.世界树.查找特征节点(存在, 类型);
        if (!feat) return false;

        auto* info = dynamic_cast<特征节点主信息类*>(feat->主信息);
        if (!info || !info->值) return false;

        特征值节点类* val = info->值;
        do {
            if (val == 目标轮廓值节点) return true;
            val = val->下;
        } while (val != info->值);

        return false;
    }
};

// ========== 找到或创建存在 实现 ==========
存在节点类* 三维场景管理类::找到或创建存在(const 存在观测& 观测)
{
    // 0) 确保当前场景有效
    if (!当前场景) {
        当前场景 = g_宇宙.世界树.自我所在场景;
    }
    if (!当前场景) {
        return nullptr;
    }

    // =========================================================
    // 1) 静态特征类型词（一次性创建）
    // =========================================================
    static 词性节点类* T_位置X = nullptr;
    static 词性节点类* T_位置Y = nullptr;
    static 词性节点类* T_位置Z = nullptr;
    static 词性节点类* T_尺寸左右 = nullptr;
    static 词性节点类* T_尺寸上下 = nullptr;
    static 词性节点类* T_尺寸前后 = nullptr;
    static 词性节点类* T_轮廓 = nullptr;

    if (!T_位置X) {
        T_位置X = 语素集.添加词性词("位置坐标X轴", "名词");
        T_位置Y = 语素集.添加词性词("位置坐标Y轴", "名词");
        T_位置Z = 语素集.添加词性词("位置坐标Z轴", "名词");
        T_尺寸左右 = 语素集.添加词性词("尺寸_左右", "名词");
        T_尺寸上下 = 语素集.添加词性词("尺寸_上下", "名词");
        T_尺寸前后 = 语素集.添加词性词("尺寸_前后", "名词");
        T_轮廓 = 语素集.添加词性词("轮廓", "名词");
    }

    // =========================================================
    // 2) 计算位置搜索范围（中心 ± 尺寸/2 + 容差）
    // =========================================================
    constexpr double k最小半径_m = 0.0;
    constexpr double k位置容差_m = 0.0;

    const Vector3D halfSize = 观测.尺寸 * 0.5;
    const double radiusX = std::max(halfSize.x + k位置容差_m, k最小半径_m);
    const double radiusY = std::max(halfSize.y + k位置容差_m, k最小半径_m);
    const double radiusZ = std::max(halfSize.z + k位置容差_m, k最小半径_m);

    const double minX = 观测.中心.x - radiusX;
    const double maxX = 观测.中心.x + radiusX;
    const double minY = 观测.中心.y - radiusY;
    const double maxY = 观测.中心.y + radiusY;
    const double minZ = 观测.中心.z - radiusZ;
    const double maxZ = 观测.中心.z + radiusZ;

    // =========================================================
    // 3) 获取轮廓编码（全局查重）
    // =========================================================
    特征值节点类* 轮廓值节点 = nullptr;
    if (!观测.轮廓编码.empty() && 观测.轮廓编码.size() == 64) {
        轮廓值节点 = 特征值集.获取或创建矢量特征值(观测.轮廓编码);
    }

    // =========================================================
    // 4) 收集位置命中的候选存在
    // =========================================================
    std::vector<存在节点类*> 位置命中列表;

    基础信息节点类* child = 当前场景->子;
    if (child) {
        基础信息节点类* cur = child;
        do {
            if (auto* exist = dynamic_cast<存在节点类*>(cur)) {
                if (轴特征值在范围内(exist, T_位置X, minX, maxX) &&
                    轴特征值在范围内(exist, T_位置Y, minY, maxY) &&
                    轴特征值在范围内(exist, T_位置Z, minZ, maxZ)) {
                    位置命中列表.push_back(exist);
                }
            }
            cur = cur->下;
        } while (cur != child);
    }

    // =========================================================
    // 5) （可选）轮廓进一步筛选
    // =========================================================
    std::vector<存在节点类*> 候选集 = 位置命中列表;
    if (轮廓值节点) {
        std::vector<存在节点类*> 轮廓命中;
        for (auto* e : 位置命中列表) {
            if (存在已有相同轮廓值(e, T_轮廓, 轮廓值节点)) {
                轮廓命中.push_back(e);
            }
        }
        if (!轮廓命中.empty()) {
            候选集 = 轮廓命中;
        }
    }

    // =========================================================
    // 6) 选距离最近的作为最佳匹配
    // =========================================================
    存在节点类* 最佳匹配 = nullptr;
    double       最佳距离 = std::numeric_limits<double>::infinity();

    for (auto* e : 候选集) {
        double closestX = 观测.中心.x;
        double closestY = 观测.中心.y;
        double closestZ = 观测.中心.z;

        轴取最近值(e, T_位置X, 观测.中心.x, closestX);
        轴取最近值(e, T_位置Y, 观测.中心.y, closestY);
        轴取最近值(e, T_位置Z, 观测.中心.z, closestZ);

        double dx = closestX - 观测.中心.x;
        double dy = closestY - 观测.中心.y;
        double dz = closestZ - 观测.中心.z;
        double dist = std::sqrt(dx * dx + dy * dy + dz * dz);

        if (dist < 最佳距离) {
            最佳距离 = dist;
            最佳匹配 = e;
        }
    }

    // =========================================================
    // 7) 无匹配 → 创建新存在并写入初始特征
    // =========================================================
    if (!最佳匹配) {
        auto* 新存在主信息 = new 存在节点主信息类();
        最佳匹配 = 世界树.新建存在(当前场景, 新存在主信息);

        // 写入位置（毫米整数）
        世界树.为存在添加特征(最佳匹配,
            new 特征节点主信息类(T_位置X, T_位置X,
                特征值集.获取或创建标量特征值(nullptr, static_cast<int64_t>(观测.中心.x * 1000))));
        世界树.为存在添加特征(最佳匹配,
            new 特征节点主信息类(T_位置Y, T_位置Y,
                特征值集.获取或创建标量特征值(nullptr, static_cast<int64_t>(观测.中心.y * 1000))));
        世界树.为存在添加特征(最佳匹配,
            new 特征节点主信息类(T_位置Z, T_位置Z,
                特征值集.获取或创建标量特征值(nullptr, static_cast<int64_t>(观测.中心.z * 1000))));

        // 写入尺寸
        世界树.为存在添加特征(最佳匹配,
            new 特征节点主信息类(T_尺寸左右, T_尺寸左右,
                特征值集.获取或创建标量特征值(nullptr, static_cast<int64_t>(观测.尺寸.x * 1000))));
        世界树.为存在添加特征(最佳匹配,
            new 特征节点主信息类(T_尺寸上下, T_尺寸上下,
                特征值集.获取或创建标量特征值(nullptr, static_cast<int64_t>(观测.尺寸.y * 1000))));
        世界树.为存在添加特征(最佳匹配,
            new 特征节点主信息类(T_尺寸前后, T_尺寸前后,
                特征值集.获取或创建标量特征值(nullptr, static_cast<int64_t>(观测.尺寸.z * 1000))));

        // 写入轮廓
        if (轮廓值节点) {
            世界树.为存在添加特征(最佳匹配,
                new 特征节点主信息类(T_轮廓, T_轮廓, 轮廓值节点));
        }

        return 最佳匹配;
    }

    // =========================================================
    // 8) 已有匹配 → 引用次数+1，处理轮廓
    // =========================================================
    最佳匹配->变更节点被引用次数(1);

    if (轮廓值节点) {
        特征节点类* 轮廓特征节点 = g_宇宙.世界树.查找特征节点(最佳匹配, T_轮廓);

        if (!轮廓特征节点) {
            // 首次出现轮廓特征
            世界树.为存在添加特征(最佳匹配,
                new 特征节点主信息类(T_轮廓, T_轮廓, 轮廓值节点));
        }
        else {
            // 检查是否已有相同轮廓值
            if (存在已有相同轮廓值(最佳匹配, T_轮廓, 轮廓值节点)) {
                // 相同 → 特征节点引用次数 +1
                轮廓特征节点->变更节点被引用次数(1);
            }
            else {
                // 不同 → 追加到值环
                auto* info = dynamic_cast<特征节点主信息类*>(轮廓特征节点->主信息);
                if (info && info->值) {
                    g_宇宙.世界树.为存在添加特征(最佳匹配, info);
                }
            }
        }
    }

    return 最佳匹配;
}

void 三维场景管理类::更新存在信息(存在节点类* 存在, const 存在观测& 观测)
{
    // 可在此处更新平均颜色、最近观测时间等动态信息
    // 目前预留，未来扩展
}

double 三维场景管理类::计算相似度(const 存在观测& o, 存在节点类* 已知存在)
{
    // 预留：可综合位置、尺寸、轮廓等多特征计算相似度
    return 0.0;
}