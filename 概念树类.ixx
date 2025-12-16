// ======================== 概念树模块.ixx ========================
export module 概念树模块;

import 模板模块;
import 主信息定义模块;
import 语素模块;
import <string>;
import <vector>;
import <unordered_map>;
import <set>;
import <iostream>;

export class 概念树类 {
public:
    // 为了少写模板长串，给节点起个别名
    using 节点类 = 链表模板<基础信息基类*>::节点类;

private:
    // 五大根（全部是在 世界链表 上的节点）
    节点类* 根_存在 = nullptr;
    节点类* 根_事件 = nullptr;
    节点类* 根_关系 = nullptr;
    节点类* 根_属性 = nullptr;
    节点类* 根_模态 = nullptr;

    // 抽象层级（0 = 五大根）
    std::unordered_map<节点类*, int> 抽象层级;

    // 路线 A 核心：概念名 → 概念主信息（本体只有一份）
    std::unordered_map<std::string, 基础信息基类*> 概念主信息表;

public:
    概念树类();

    // 五大根访问接口
    节点类* 取根_存在() const { return 根_存在; }
    节点类* 取根_事件() const { return 根_事件; }
    节点类* 取根_关系() const { return 根_关系; }
    节点类* 取根_属性() const { return 根_属性; }
    节点类* 取根_模态() const { return 根_模态; }

    int 取抽象层级(节点类* 节点) const {
        if (!节点) return -1;
        if (auto it = 抽象层级.find(节点); it != 抽象层级.end()) {
            return it->second;
        }
        return -1;
    }

    // ======================= 概念本体相关接口 =======================

    // 根据名字获取/创建“概念主信息”（只管基础信息，不建节点）
    // 词性标签：例如 "抽象概念" / "概念根"
    基础信息基类* 获取或创建概念主信息(const std::string& 概念名,const std::string& 词性标签 = "抽象概念");

    // 在概念树里基于某个“概念主信息”挂一个“抽象概念节点”
    节点类* 新建概念节点(基础信息基类* 概念主信息,节点类* 父概念 = nullptr);

    // 一步到位：名字 → 概念主信息 → 概念树节点，并触发“向上抽象”
    节点类* 新建并提交概念(const std::string& 概念名,节点类* 父概念 = nullptr);

    // 触发/重算某个层级的“向上抽象”
    void 尝试向上抽象(节点类* 当前层概念);

private:
    // 初始化五大根
    void 初始化五大根概念();

    // 内部：对当前层概念，根据“子概念共同本质”生成一个更高阶抽象
    void 生成更高阶抽象(节点类* 当前层概念,const std::set<词性节点类*>& 共同本质  );

    // 抽象特征集合：从“某个概念节点”的子链中，提取所有“特征类型”
    std::set<词性节点类*> 获取抽象特征集(节点类* 抽象节点);

    // 简单的 set 交集（用指针比较即可）
    static std::set<词性节点类*> 交集(const std::set<词性节点类*>& a,const std::set<词性节点类*>& b );

    // 在整个“世界链表”中按“概念名”查找概念节点（通过 名称→词面 比较）
    节点类* 查找概念(const std::string& 名称);

public:
    // 工具：通过 词性节点 拿到词面（现在用 std::string）
    static std::string 取词面(词性节点类* 节点);
};

