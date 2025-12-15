export module 语言本能学习桥;
import <string>;
import <vector>;
import <unordered_map>;
import 语言学习模块;
import 语言知识库模块;
import 主信息定义模块;

export 结构体_句型样本 构造_句型样本(
    const std::string& 原文,
    const std::vector<词性节点类*>& 词序列,
    场景节点类* 场景,
    const std::unordered_map<std::string, 基础信息节点类*>& 槽位绑定,
    const std::vector<二次特征节点类*>& 证据链,
    double 置信度, int 冲突数);
