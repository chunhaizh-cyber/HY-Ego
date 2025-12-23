// 概念_存在树模块.ixx
export module 存在概念树模块;

import 主信息定义模块;
import 数据仓库模块;
import 语素环境模块;
import 特征值环境模块;
import 特征类型定义模块;

import <string>;
import <vector>;
import <unordered_map>;
import <unordered_set>;
import <algorithm>;

using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;

namespace {

    inline 存在节点主信息类* 取存在主信息(存在节点类* n) {
        return n ? dynamic_cast<存在节点主信息类*>((基础信息基类*)n->主信息) : nullptr;
    }

    inline 特征节点主信息类* 取特征主信息(特征节点类* f) {
        return f ? dynamic_cast<特征节点主信息类*>((基础信息基类*)f->主信息) : nullptr;
    }

    inline 特征节点类* 在存在中查找特征(存在节点类* n, 词性节点类* type) {
        if (!n || !type) return nullptr;
        if (auto* mi = 取存在主信息(n)) {
            for (auto* f : mi->特征索引) {
                auto* fmi = 取特征主信息(f);
                if (fmi && fmi->类型 == type) return f;
            }
        }
        return nullptr;
    }

    inline 特征节点类* 确保特征占位(存在节点类* n, 词性节点类* type) {
        if (!n || !type) return nullptr;
        if (auto* exist = 在存在中查找特征(n, type)) return exist;

        auto* emi = 取存在主信息(n);
        if (!emi) return nullptr;

        auto* fmi = new 特征节点主信息类(type);
        基础信息基类* bi = fmi;
        auto* f = static_cast<特征节点类*>(世界链.添加子节点(n, bi));

        // 兜底维护索引（世界树那边如果也维护，这里 push 重复也不会太大，但我们避免重复）
        emi->特征索引.push_back(f);
        emi->特征当前状态.push_back(f);
        return f;
    }

    inline void 为存在设置特征类型集合(存在节点类* n, const vector<词性节点类*>& types) {
        if (!n) return;
        for (auto* t : types) {
            if (!t) continue;
            确保特征占位(n, t);
        }
    }

    inline void 设置概念标量I64(存在节点类* n, 词性节点类* type, std::int64_t v) {
        auto* f = 确保特征占位(n, type);
        if (!f) return;
        auto* fmi = 取特征主信息(f);
        if (!fmi) return;
        fmi->值 = 特征值集.获取或创建标量特征值(nullptr, v);
    }

    inline 存在节点类* 创建配置子存在(存在节点类* parent, const string& key, const vector<词性节点类*>& featureTypes) {
        if (!parent) return nullptr;
        auto* mi = new 存在节点主信息类();
        基础信息基类* bi = mi;
        auto* n = static_cast<存在节点类*>(世界链.添加子节点(parent, bi));
        n->主键 = key;

        if (auto* pmi = 取存在主信息(parent)) {
            pmi->子存在索引.push_back(n);
        }

        为存在设置特征类型集合(n, featureTypes);
        return n;
    }

} // namespace

export class 存在概念树类 {
public:
    using 节点类型 = 存在节点类;

private:
    节点类型* 根_存在概念 = nullptr;  // 概念_存在树的根（也在 世界链 上）

    // 显示模块用的抽象配置节点（也放在世界链上）
    节点类型* 概念_显示参数 = nullptr;

    // 用于概念去重：规范化 Key -> 概念节点
    unordered_map<string, 节点类型*> Key到概念节点;
    // 便于遍历/选父概念
    vector<节点类型*> 概念节点列表;

public:
    存在概念树类() = default;
    ~存在概念树类() = default;

    存在概念树类(const 存在概念树类&) = delete;
    存在概念树类& operator=(const 存在概念树类&) = delete;
    存在概念树类(存在概念树类&&) = default;
    存在概念树类& operator=(存在概念树类&&) = default;

    // 在 世界链 上建立一棵“存在概念树”的根
    void 初始化默认存在概念() {
        根_存在概念 = nullptr;
        概念_显示参数 = nullptr;
        Key到概念节点.clear();
        概念节点列表.clear();

        // 1) 根概念（特征集合视为 ∅）
        {
            auto* mi = new 存在节点主信息类();
            基础信息基类* bi = mi;
            根_存在概念 = static_cast<节点类型*>(世界链.添加子节点(世界链.根指针, bi));
            根_存在概念->主键 = "概念_存在_根";

            // 根概念的模板特征集合视为 ∅
            mi->特征索引.clear();
            mi->特征当前状态.clear();

            Key到概念节点[""] = 根_存在概念;
            概念节点列表.push_back(根_存在概念);
        }

        // 2) 一个“显示需要的存在特征集合”概念（给显示/调试时快速定位与校验）
        {
            std::vector<词性节点类*> types = {
                // 坐标
                类型_相对位置X, 类型_相对位置Y, 类型_相对位置Z,
                类型_位置X, 类型_位置Y, 类型_位置Z,

                // 尺寸 + 朝向
                类型_相对尺寸X, 类型_相对尺寸Y, 类型_相对尺寸Z,
                类型_尺寸左右, 类型_尺寸上下, 类型_尺寸前后,
                类型_主方向1, 类型_主方向2, 类型_主方向3,

                // 外观
                类型_平均颜色,
                类型_轮廓, 类型_轮廓_8x8,
                类型_轮廓金字塔16, 类型_轮廓金字塔32, 类型_轮廓金字塔64,
                类型_轮廓3D, 类型_边界点序列,

                // 质量
                类型_质量, 类型_可信度, 类型_有效点数, 类型_有效点比例,
                类型_深度噪声_MAD, 类型_PCA残差, 类型_颜色一致性,
            };

            创建或查找概念_并设置别名主键(types, "概念_显示特征信息集合");
        }

        // 3) 显示参数（抽象配置）
        初始化显示参数概念();
    }

    节点类型* 获取根概念() const { return 根_存在概念; }
    节点类型* 获取显示参数概念() const { return 概念_显示参数; }

    // —— 核心：给一组特征类型集合，创建或查找存在概念 ——
    节点类型* 创建或查找概念(const vector<词性节点类*>& 特征类型集合) {
        vector<词性节点类*> 规范集合 = 去重并排序(特征类型集合);
        string key = 生成集合Key(规范集合);

        auto it = Key到概念节点.find(key);
        if (it != Key到概念节点.end()) return it->second;

        return 创建概念节点_内部(规范集合, key);
    }

    // 常用：创建/查找后给概念一个稳定“别名主键”（便于显示模块定位）
    节点类型* 创建或查找概念_并设置别名主键(
        const vector<词性节点类*>& 特征类型集合,
        const string& 别名主键)
    {
        auto* n = 创建或查找概念(特征类型集合);
        if (n && !别名主键.empty()) n->主键 = 别名主键;
        return n;
    }

    节点类型* 按特征类型集合查找(const vector<词性节点类*>& 特征类型集合) const {
        vector<词性节点类*> 规范集合 = 去重并排序(特征类型集合);
        string key = 生成集合Key(规范集合);

        auto it = Key到概念节点.find(key);
        if (it == Key到概念节点.end()) return nullptr;
        return it->second;
    }

    // 便于外部按“概念主键别名”定位（例如："概念_显示参数"）
    节点类型* 按概念主键查找(const string& 概念主键) const {
        if (概念主键.empty()) return nullptr;
        for (auto* n : 概念节点列表) {
            if (n && n->主键 == 概念主键) return n;
        }
        return nullptr;
    }

private:
    void 初始化显示参数概念() {
        if (!根_存在概念) return;

        // 抽象“显示参数”本身也是一个存在节点：
        // - 它的特征：用于保存 UI 开关/数值参数
        // - 它的子存在：用于声明“显示所依赖的特征类型集合”（坐标/尺寸/朝向/外观/质量...）

        // 1) 根节点
        {
            auto* mi = new 存在节点主信息类();
            基础信息基类* bi = mi;
            概念_显示参数 = static_cast<节点类型*>(世界链.添加子节点(根_存在概念, bi));
            概念_显示参数->主键 = "概念_显示参数";

            // 放进列表，便于外部按主键查找
            概念节点列表.push_back(概念_显示参数);

            // 2) 声明“显示选项”的类型（并写入默认值）
            vector<词性节点类*> optionTypes = {
                类型_显示_开关_三维背景,
                类型_显示_开关_坐标轴,
                类型_显示_开关_网格地面,
                类型_显示_开关_实时点云,
                类型_显示_开关_存在框,
                类型_显示_开关_存在大小 ,
                类型_显示_开关_存在标签 ,                
                类型_显示_开关_存在ID,
                类型_显示_开关_存在名称,
                类型_显示_开关_存在坐标,
                类型_显示_开关_存在轨迹,
                类型_显示_开关_质量信息,
                类型_显示_开关_视野外淡化,

                类型_显示_坐标模式,
                类型_显示_点云采样步长,
                类型_显示_FOV水平_百分度,
                类型_显示_FOV垂直_百分度,
                类型_显示_近裁剪_mm,
                类型_显示_远裁剪_mm,
                类型_显示_记忆淡化_ms,
                类型_显示_轨迹长度,
            };

            为存在设置特征类型集合(概念_显示参数, optionTypes);

            // 默认值（你后面接 UI 时，直接改这些特征值即可）
            设置概念标量I64(概念_显示参数, 类型_显示_开关_三维背景, 1);
            设置概念标量I64(概念_显示参数, 类型_显示_开关_坐标轴, 1);
            设置概念标量I64(概念_显示参数, 类型_显示_开关_网格地面, 1);
            设置概念标量I64(概念_显示参数, 类型_显示_开关_实时点云, 1);

            设置概念标量I64(概念_显示参数, 类型_显示_开关_存在框, 1);
            设置概念标量I64(概念_显示参数, 类型_显示_开关_存在大小, 1);
            设置概念标量I64(概念_显示参数, 类型_显示_开关_存在标签, 1);
            设置概念标量I64(概念_显示参数, 类型_显示_开关_存在ID, 1);
            设置概念标量I64(概念_显示参数, 类型_显示_开关_存在名称, 0);
            设置概念标量I64(概念_显示参数, 类型_显示_开关_存在坐标, 1);
            设置概念标量I64(概念_显示参数, 类型_显示_开关_存在轨迹, 0);
            设置概念标量I64(概念_显示参数, 类型_显示_开关_质量信息, 0);
            设置概念标量I64(概念_显示参数, 类型_显示_开关_视野外淡化, 1);

            // 0=相对坐标（默认），1=绝对坐标，2=双显
            设置概念标量I64(概念_显示参数, 类型_显示_坐标模式, 0);

            // 点云采样步长（像素步长）
            设置概念标量I64(概念_显示参数, 类型_显示_点云采样步长, 4);

            // 视野判定：D455 典型近似（度*100）
            设置概念标量I64(概念_显示参数, 类型_显示_FOV水平_百分度, 8600);
            设置概念标量I64(概念_显示参数, 类型_显示_FOV垂直_百分度, 5700);
            // 近/远裁剪（mm）
            设置概念标量I64(概念_显示参数, 类型_显示_近裁剪_mm, 150);
            设置概念标量I64(概念_显示参数, 类型_显示_远裁剪_mm, 5000);

            // 记忆淡化（ms）
            设置概念标量I64(概念_显示参数, 类型_显示_记忆淡化_ms, 15000);

            // 轨迹长度（点数）
            设置概念标量I64(概念_显示参数, 类型_显示_轨迹长度, 60);
        }

        // 2) 子存在：声明显示依赖的“特征类型集合”
        if (!概念_显示参数) return;

        // 坐标
        创建配置子存在(概念_显示参数, "显示_坐标_相对", { 类型_相对位置X, 类型_相对位置Y, 类型_相对位置Z });
        创建配置子存在(概念_显示参数, "显示_坐标_绝对", { 类型_位置X, 类型_位置Y, 类型_位置Z });

        // 尺寸
        创建配置子存在(概念_显示参数, "显示_尺寸_相对", { 类型_相对尺寸X, 类型_相对尺寸Y, 类型_相对尺寸Z });
        创建配置子存在(概念_显示参数, "显示_尺寸_绝对", { 类型_尺寸左右, 类型_尺寸上下, 类型_尺寸前后 });

        // 朝向（OBB/姿态）
        创建配置子存在(概念_显示参数, "显示_朝向_PCA", { 类型_主方向1, 类型_主方向2, 类型_主方向3 });

        // 外观/形状
        创建配置子存在(概念_显示参数, "显示_外观", {
            类型_平均颜色,
            类型_轮廓, 类型_轮廓_8x8,
            类型_轮廓金字塔16, 类型_轮廓金字塔32, 类型_轮廓金字塔64,
            类型_轮廓3D,
            类型_边界点序列
            });

        // 质量/可信度
        创建配置子存在(概念_显示参数, "显示_质量", {
            类型_质量,
            类型_可信度,
            类型_有效点数,
            类型_有效点比例,
            类型_深度噪声_MAD,
            类型_PCA残差,
            类型_颜色一致性
            });

        // 运动学（可选）
        创建配置子存在(概念_显示参数, "显示_运动", {
            类型_速度X, 类型_速度Y, 类型_速度Z,
            类型_加速度X, 类型_加速度Y, 类型_加速度Z
            });

        // 点簇缓存（可选）
        创建配置子存在(概念_显示参数, "显示_点簇", {
            类型_点簇点云,
            类型_点簇法线,
            类型_点簇颜色
            });
    }

private:
    static vector<词性节点类*> 去重并排序(const vector<词性节点类*>& 原始) {
        vector<词性节点类*> v = 原始;
        v.erase(std::remove(v.begin(), v.end(), nullptr), v.end());

        std::sort(v.begin(), v.end(),
            [](词性节点类* a, 词性节点类* b) {
                return a->主键 < b->主键;
            });

        v.erase(std::unique(v.begin(), v.end(),
            [](词性节点类* a, 词性节点类* b) {
                return a->主键 == b->主键;
            }), v.end());

        return v;
    }

    static string 生成集合Key(const vector<词性节点类*>& 集合) {
        string key;
        bool 第一 = true;
        for (auto* t : 集合) {
            if (!t) continue;
            if (!第一) key.push_back('|');
            key += t->主键;
            第一 = false;
        }
        return key;
    }

    节点类型* 创建概念节点_内部(const vector<词性节点类*>& 规范集合, const string& key) {
        if (!根_存在概念) {
            初始化默认存在概念();
        }

        // 1) 找“最具体的父概念”：S(父) ⊆ S_new 且 |S(父)| 最大
        节点类型* 父概念 = 选择父概念(规范集合);

        // 2) 在 世界链 上，作为父概念的子节点插入
        auto* mi = new 存在节点主信息类();
        基础信息基类* bi = mi;

        节点类型* 新节点 = static_cast<节点类型*>(世界链.添加子节点(父概念 ? 父概念 : 根_存在概念, bi));

        // 3) 关键：把“模板特征类型集合”实体化成“特征占位节点”
        为存在设置特征类型集合(新节点, 规范集合);

        // 4) 注册去重表 + 记录列表
        Key到概念节点[key] = 新节点;
        概念节点列表.push_back(新节点);

        return 新节点;
    }

    节点类型* 选择父概念(const vector<词性节点类*>& S_new) const {
        if (!根_存在概念) return nullptr;

        unordered_set<string> S_new_key;
        S_new_key.reserve(S_new.size());
        for (auto* t : S_new) {
            if (!t) continue;
            S_new_key.insert(t->主键);
        }

        节点类型* 最佳父 = 根_存在概念;
        std::size_t 最佳父特征数 = 0;

        for (auto* node : 概念节点列表) {
            if (!node) continue;
            auto* mi = dynamic_cast<存在节点主信息类*>((基础信息基类*)node->主信息);
            if (!mi) continue;
            const auto S_parent = mi->获取特征类型集合();

            bool 是子集 = true;
            for (auto* t : S_parent) {
                if (!t) continue;
                if (S_new_key.find(t->主键) == S_new_key.end()) {
                    是子集 = false;
                    break;
                }
            }
            if (!是子集) continue;

            std::size_t 当前特征数 = S_parent.size();
            if (当前特征数 > 最佳父特征数) {
                最佳父特征数 = 当前特征数;
                最佳父 = node;
            }
        }

        return 最佳父 ? 最佳父 : 根_存在概念;
    }
};
