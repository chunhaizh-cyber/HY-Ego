export module 表达落地;

import <string>;
import <vector>;
import 主信息定义模块;


export struct 结构体_判断输出 落地_形容词表达(
    场景节点类* /*场景*/,
    基础信息节点类* /*主语*/, 基础信息节点类* /*宾语*/,
    词性节点类* /*形容词词*/,
    const std::vector<二次特征节点类*>& /*证据链*/,
    const std::string& /*来源方法ID*/,
    const std::string& /*模板ID*/,
    int64_t /*参数哈希*/,
    double /*置信度*/)
{
    // TODO: 在场景子链中创建形容词表达节点并连接证据
    return {};
}

export struct 结构体_判断输出 落地_短语比较(
    场景节点类* /*场景*/,
    基础信息节点类* /*主语*/, 基础信息节点类* /*宾语*/,
    词性节点类* /*比较词*/, 词性节点类* /*程度词*/, 词性节点类* /*形容词词*/, 词性节点类* /*参照词*/,
    const std::vector<二次特征节点类*>& /*证据链*/,
    const std::string& /*来源方法ID*/,
    const std::string& /*模板ID*/,
    int64_t /*参数哈希*/,
    double /*置信度*/)
{
    // TODO: 在场景子链中创建短语表达节点并连接证据
    return 结构体_判断输出{};
}
