export module 语言理解方法;

import <string>;
import <vector>;
import 主信息定义模块;
import 语言知识库模块;
import 二次特征_通用判断;
import 评估与提交策略;

export struct 结构体_句子理解输出 {
    场景节点类* 临时场景 = nullptr;
    std::vector<二次特征节点类*> 评估证据;
    double 全局置信度 = 0.0;
    bool 是否提交 = false;
};

export struct 结构体_段落理解输出 {
    场景节点类* 段落场景 = nullptr;
    std::vector<二次特征节点类*> 评估证据;
    double 全局置信度 = 0.0;
    bool 是否提交 = false;
};

export struct 结构体_文章理解输出 {
    场景节点类* 文档场景 = nullptr;
    std::vector<二次特征节点类*> 评估证据;
    double 全局置信度 = 0.0;
    bool 是否提交 = false;
};

// —— 句子层入口（最小骨架） ——
export 结构体_句子理解输出 方法_句子理解_复杂(
    const std::string& /*文本*/, 场景节点类* /*上下文场景*/)
{
    结构体_句子理解输出 out;
    // TODO: 本能层→LK检索与绑定→证据生成→子图装配→评估与提交
    return out;
}

// —— 段落层入口（最小骨架） ——
//export 结构体_段落理解输出 方法_段落理解_复杂(
//    const std::vector<场景节点类*>& /*句子场景序列*/, 段落节点类* /*段落容器*/)
//{
//    结构体_段落理解输出 out;
//    // TODO: 跨句共指、时间线融合、主题聚类、评估与提交
//    return out;
//}
//
//// —— 文章层入口（最小骨架） ——
//export 结构体_文章理解输出 方法_文章理解_复杂(
//    const std::vector<场景节点类*>& /*段落场景序列*/, 文章节点类* /*文章容器*/)
//{
//    结构体_文章理解输出 out;
//    // TODO: 篇章关系、全文时间线、来源一致性、评估与提交
//    return out;
//}

// —— 子方法占位（你可逐步补齐实现） ——
export bool 方法_句法成分绑定(场景节点类* /*临时场景*/) { return true; }
export bool 方法_指代消解(场景节点类* /*临时场景*/) { return true; }
export bool 方法_时间体貌解析(场景节点类* /*临时场景*/) { return true; }
export bool 方法_生成二次特征(场景节点类* /*临时场景*/) { return true; }
export bool 方法_冲突与置信度融合(场景节点类* /*临时场景*/, double& /*置信度*/) { return true; }
export bool 方法_提交或回滚(场景节点类* /*临时场景*/, double /*全局置信度*/, double /*阈值*/) { return true; }
