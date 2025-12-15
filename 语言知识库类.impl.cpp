module;


module 语言知识库模块;
import 模板模块;
import 主信息定义模块;
import <string>;
import <vector>;
import <functional>;
import  <algorithm>;

// ====== 辅助：获取词文本 ======
/*
std::string 自然语言知识库类::取词文本(const 词性节点类* 节点)
{
    if (!节点) return "";
	auto* 词主信息 = dynamic_cast<词主信息类*>(节点->父->主信息);
    return 词主信息->词;
}

bool 自然语言知识库类::文本等于(const 词性节点类* 节点,
    const std::string& 文本)
{
    return 取词文本(节点) == 文本;
}

// ====== 辅助：简单类型判断（全部基于字符串，先粗糙实现） ======

bool 自然语言知识库类::是时间词(const 词性节点类* 节点)
{
    const auto t = 取词文本(节点);
    static const std::string 标记[] = {
        "年", "月", "日", "号", "点", "分", "秒",
        "今天", "昨天", "明天", "现在", "刚才", "刚刚",
        "上午", "中午", "下午", "晚上", "早上"
    };
    for (const auto& m : 标记) {
        if (t.find(m) != std::string::npos) return true;
    }
    return false;
}

bool 自然语言知识库类::是否定词(const 词性节点类* 节点)
{
    const auto t = 取词文本(节点);
    return (t == "不" || t == "没" || t == "没有" ||
        t == "别" || t == "无" || t == "不是");
}

bool 自然语言知识库类::是疑问标记(const 词性节点类* 节点)
{
    const auto t = 取词文本(节点);
    return (t == "吗" || t == "呢" ||
        t == "什么" || t == "哪儿" || t == "哪里" ||
        t == "谁" || t == "几时" || t == "什么时候" ||
        t.find("？") != std::string::npos ||
        t.find("?") != std::string::npos);
}

bool 自然语言知识库类::是条件连词(const 词性节点类* 节点)
{
    const auto t = 取词文本(节点);
    return (t == "如果" || t == "只要" || t == "假如" ||
        t == "倘若" || t == "要是" || t == "当");
}

bool 自然语言知识库类::是因果连词_原因(const 词性节点类* 节点)
{
    const auto t = 取词文本(节点);
    return (t == "因为" || t == "由于" || t == "因" ||
        t == "既然");
}

bool 自然语言知识库类::是因果连词_结果(const 词性节点类* 节点)
{
    const auto t = 取词文本(节点);
    return (t == "所以" || t == "因此" || t == "于是" ||
        t == "从而");
}

bool 自然语言知识库类::是比较标记(const 词性节点类* 节点)
{
    const auto t = 取词文本(节点);
    return (t == "比" || t == "更" || t == "较" || t == "最");
}

bool 自然语言知识库类::是模态词(const 词性节点类* 节点)
{
    const auto t = 取词文本(节点);
    return (t == "必须" || t == "应该" || t == "需要" ||
        t == "可以" || t == "得" || t == "要" || t == "能");
}

bool 自然语言知识库类::是例外标记(const 词性节点类* 节点)
{
    const auto t = 取词文本(节点);
    return (t == "除了" || t == "除" || t == "以外");
}

bool 自然语言知识库类::是单位词(const 词性节点类* 节点)
{
    const auto t = 取词文本(节点);
    static const std::string 单位[] = {
        "米", "厘米", "毫米",
        "公斤", "千克", "克",
        "度", "秒", "分钟", "小时",
        "天", "年", "周", "个月", "个月"
    };
    for (const auto& u : 单位) {
        if (t == u) return true;
    }
    return false;
}

bool 自然语言知识库类::是代词_指代(const 词性节点类* 节点)
{
    const auto t = 取词文本(节点);
    return (t == "他" || t == "她" || t == "它" ||
        t == "他们" || t == "她们" || t == "它们" ||
        t == "那里" || t == "那儿" ||
        t == "这里" || t == "这儿" ||
        t == "这样" || t == "那样");
}

bool 自然语言知识库类::是数字串(const std::string& s)
{
    if (s.empty()) return false;
    bool 有小数点 = false;
    for (char ch : s) {
        if (ch >= L'0' && ch <= L'9') continue;
        if (ch == L'.') {
            if (有小数点) return false;
            有小数点 = true;
            continue;
        }
        return false;
    }
    return true;
}

double 自然语言知识库类::解析数字(const std::string& s)
{
    // 这里只处理纯阿拉伯数字，中文数字后续可扩展
    if (!是数字串(s)) return 0.0;
    std::string narrow(s.begin(), s.end());
    try {
        return std::stod(narrow);
    }
    catch (...) {
        return 0.0;
    }
}
//1. 空间位置：A 在 B（里面/上/下/附近…）
bool 自然语言知识库类::融合_空间位置(const std::vector<词性节点类*>& 词序列,世界树类& 世界树,场景节点类* 场景)
{
    if (词序列.size() < 3) return false;

    int 在索引 = -1;
    for (size_t i = 0; i < 词序列.size(); ++i) {
        if (文本等于(词序列[i], "在")) {
            在索引 = static_cast<int>(i);
            break;
        }
    }
    if (在索引 <= 0 || 在索引 + 1 >= static_cast<int>(词序列.size())) {
        return false; // 不是 “X 在 Y” 结构
    }

    词性节点类* 主体词 = 词序列[在索引 - 1];
    std::string 地点表达;
    for (size_t i = 在索引 + 1; i < 词序列.size(); ++i) {
        auto t = 取词文本(词序列[i]);
        if (!t.empty()) {
            if (!地点表达.empty()) 地点表达 += " ";
            地点表达 += t;
        }
    }
    if (地点表达.empty()) return false;

  //  auto* 主体 = 世界树.确保存在(场景, 主体词);
  //  auto* 场所 = 世界树.确保抽象场所(场景, 地点表达);
  //
  //  世界树.记录空间位置(主体, 场所, "在");
    return true;
}
//2. 时间定位：事件 在 / 发生在 某个时间
bool 自然语言知识库类::融合_时间定位(const std::vector<词性节点类*>& 词序列,世界树类& 世界树,场景节点类* 场景)
{
    if (词序列.empty()) return false;

    int 时间索引 = -1;
    for (size_t i = 0; i < 词序列.size(); ++i) {
        if (是时间词(词序列[i])) {
            时间索引 = static_cast<int>(i);
            break;
        }
    }
    if (时间索引 == -1) return false; // 没有明显时间词

    // 简化：把第一个词当成“事件/存在”的锚点
    词性节点类* 锚词 = 词序列[0];
    auto* 锚存在 = 世界树.确保存在(场景, 锚词);

    std::string 时间表达;
    for (size_t i = 时间索引; i < 词序列.size(); ++i) {
        if (是时间词(词序列[i])) {
            auto t = 取词文本(词序列[i]);
            if (!t.empty()) {
                if (!时间表达.empty()) 时间表达 += " ";
                时间表达 += t;
            }
        }
    }
    if (时间表达.empty()) return false;

    世界树.记录时间定位(锚存在, 时间表达);
    return true;
}
//3. 比较与排序：A 比 B 更 高 / 大 / 快
bool 自然语言知识库类::融合_比较排序(const std::vector<词性节点类*>& 词序列,世界树类& 世界树,场景节点类* 场景)
{
    if (词序列.size() < 3) return false;

    int 比索引 = -1;
    for (size_t i = 0; i < 词序列.size(); ++i) {
        if (文本等于(词序列[i], "比")) {
            比索引 = static_cast<int>(i);
            break;
        }
    }
    if (比索引 <= 0 || 比索引 + 1 >= static_cast<int>(词序列.size())) {
        return false;
    }

    词性节点类* A词 = 词序列[比索引 - 1];
    词性节点类* B词 = 词序列[比索引 + 1];

    // 简单：最后一个词当作“比较维度（形容词）”
    词性节点类* 维度词 = 词序列.back();
    std::string 维度 = 取词文本(维度词);
    if (维度.empty()) 维度 = "";

    auto* A = 世界树.确保存在(场景, A词);
    auto* B = 世界树.确保存在(场景, B词);

    世界树.记录比较关系(A, B, 维度, ">");
    return true;
}
//4. 数量与范围：A 有 三 个 B / A 有 很多 B
bool 自然语言知识库类::融合_数量范围(const std::vector<词性节点类*>& 词序列,世界树类& 世界树,场景节点类* 场景)
{
    if (词序列.size() < 3) return false;

    int 有索引 = -1;
    for (size_t i = 0; i < 词序列.size(); ++i) {
        if (文本等于(词序列[i], "有")) {
            有索引 = static_cast<int>(i);
            break;
        }
    }
    if (有索引 <= 0 || 有索引 + 1 >= static_cast<int>(词序列.size())) {
        return false;
    }

    词性节点类* 主体词 = 词序列[有索引 - 1];
    auto* 主体 = 世界树.确保存在(场景, 主体词);

    double 数值 = 0.0;
    std::string 数量表达;
    std::string 对象名称;

    for (size_t i = 有索引 + 1; i < 词序列.size(); ++i) {
        auto t = 取词文本(词序列[i]);
        if (t.empty()) continue;

        if (是数字串(t)) {
            数值 = 解析数字(t);
            if (!数量表达.empty()) 数量表达 += " ";
            数量表达 += t;
        }
        else {
            if (!对象名称.empty()) 对象名称 += " ";
            对象名称 += t;
        }
    }

    if (数量表达.empty()) return false;

    世界树.记录数量特征(主体, 数值, 数量表达, 对象名称);
    return true;
}
//5. 否定与排除：不 / 没 / 不是 / 没有
bool 自然语言知识库类::融合_否定排除(const std::vector<词性节点类*>& 词序列,世界树类& 世界树,场景节点类* 场景)
{
    int 否定索引 = -1;
    for (size_t i = 0; i < 词序列.size(); ++i) {
        if (是否定词(词序列[i])) {
            否定索引 = static_cast<int>(i);
            break;
        }
    }
    if (否定索引 == -1) return false;

    // 将整句（去掉否定词）当成一个“被否定的事件文本”
    std::string 事件文本;
    for (size_t i = 0; i < 词序列.size(); ++i) {
        if (static_cast<int>(i) == 否定索引) continue;
        auto t = 取词文本(词序列[i]);
        if (!t.empty()) {
            if (!事件文本.empty()) 事件文本 += " ";
            事件文本 += t;
        }
    }

    if (事件文本.empty()) return false;

    auto* 事件 = 世界树.记录文本事件(场景, 事件文本);
    世界树.记录否定信息(事件, 取词文本(词序列[否定索引]));
    return true;
}
//6. 疑问与信息需求：吗 / 什么 / 谁 / 哪儿 / …？
bool 自然语言知识库类::融合_疑问需求(const std::vector<词性节点类*>& 词序列,世界树类& 世界树,场景节点类* 场景)
{
    bool 有疑问 = false;
    bool 是是非问 = false;

    std::string 全句;
    for (auto* 词 : 词序列) {
        auto t = 取词文本(词);
        if (t.empty()) continue;

        if (!全句.empty()) 全句 += " ";
        全句 += t;

        if (是疑问标记(词)) {
            有疑问 = true;
            if (t == "吗" || t == "呢")
                是是非问 = true;
        }
    }

    if (!有疑问) return false;

    std::string 类型 = 是是非问 ? "是非问" : "内容问";
    世界树.添加信息需求(场景, 类型, 全句);
    return true;
}
//7. 条件与假设：如果 A，(就) B
bool 自然语言知识库类::融合_条件假设(const std::vector<词性节点类*>& 词序列,世界树类& 世界树,场景节点类* 场景)
{
    int 条件索引 = -1;
    int 就索引 = -1;

    for (size_t i = 0; i < 词序列.size(); ++i) {
        if (是条件连词(词序列[i])) {
            条件索引 = static_cast<int>(i);
        }
        else if (文本等于(词序列[i], "就")) {
            就索引 = static_cast<int>(i);
        }
    }
    if (条件索引 == -1) return false;

    std::string 条件文本;
    std::string 结果文本;

    // 条件：从 条件连词 后面到 “就” 或句中间
    for (size_t i = 条件索引 + 1;
        i < 词序列.size() && (就索引 == -1 || static_cast<int>(i) < 就索引);
        ++i)
    {
        auto t = 取词文本(词序列[i]);
        if (!t.empty()) {
            if (!条件文本.empty()) 条件文本 += " ";
            条件文本 += t;
        }
    }

    // 结果：从 “就” 或 条件片段之后 到句尾
    size_t 起始 = (就索引 != -1) ? (就索引 + 1) : (条件索引 + 1);
    for (size_t i = 起始; i < 词序列.size(); ++i) {
        auto t = 取词文本(词序列[i]);
        if (!t.empty()) {
            if (!结果文本.empty()) 结果文本 += " ";
            结果文本 += t;
        }
    }

    if (条件文本.empty() || 结果文本.empty()) return false;

    auto* 条件事件 = 世界树.记录文本事件(场景, 条件文本);
    auto* 结果事件 = 世界树.记录文本事件(场景, 结果文本);

    世界树.记录条件关系(条件事件, 结果事件, 取词文本(词序列[条件索引]));
    return true;
}
//8. 因果与解释：因为 A，所以 B
bool 自然语言知识库类::融合_因果解释(const std::vector<词性节点类*>& 词序列,世界树类& 世界树,场景节点类* 场景)
{
    int 因索引 = -1;
    int 果索引 = -1;

    for (size_t i = 0; i < 词序列.size(); ++i) {
        if (是因果连词_原因(词序列[i])) {
            因索引 = static_cast<int>(i);
        }
        else if (是因果连词_结果(词序列[i])) {
            果索引 = static_cast<int>(i);
        }
    }

    if (因索引 == -1 || 果索引 == -1) return false;

    std::string 因文本;
    std::string 果文本;

    for (size_t i = 因索引 + 1; i < 词序列.size() && static_cast<int>(i) < 果索引; ++i) {
        auto t = 取词文本(词序列[i]);
        if (!t.empty()) {
            if (!因文本.empty()) 因文本 += " ";
            因文本 += t;
        }
    }

    for (size_t i = 果索引 + 1; i < 词序列.size(); ++i) {
        auto t = 取词文本(词序列[i]);
        if (!t.empty()) {
            if (!果文本.empty()) 果文本 += " ";
            果文本 += t;
        }
    }

    if (因文本.empty() || 果文本.empty()) return false;

    auto* 因事件 = 世界树.记录文本事件(场景, 因文本);
    auto* 果事件 = 世界树.记录文本事件(场景, 果文本);

    世界树.记录因果关系(因事件, 果事件,
        取词文本(词序列[因索引]) + " - " +
        (果索引 >= 0 ? 取词文本(词序列[果索引]) : ""));
    return true;
}
//9. 时间与体貌：正在 / 已经 / 还没 / 一直 / 刚刚
bool 自然语言知识库类::融合_时间体貌(const std::vector<词性节点类*>& 词序列,世界树类& 世界树,场景节点类* 场景)
{
    std::string 体貌标记;
    for (auto* 词 : 词序列) {
        auto t = 取词文本(词);
        if (t == "正在" || t == "已在") {
            体貌标记 = "进行中";
            break;
        }
        else if (t == "已经" || t == "已") {
            体貌标记 = "已完成";
            break;
        }
        else if (t == "还没" || t == "尚未") {
            体貌标记 = "未发生";
            break;
        }
        else if (t == "刚刚" || t == "刚才") {
            体貌标记 = "刚刚发生";
            break;
        }
        else if (t == "一直") {
            体貌标记 = "持续";
            break;
        }
    }

    if (体貌标记.empty()) return false;

    // 这里简化：用整句文本作为“事件”
    std::string 事件文本;
    for (auto* 词 : 词序列) {
        auto t = 取词文本(词);
        if (!t.empty()) {
            if (!事件文本.empty()) 事件文本 += " ";
            事件文本 += t;
        }
    }

    auto* 事件 = 世界树.记录文本事件(场景, 事件文本);
    世界树.记录体貌(事件, 体貌标记);
    return true;
}
//10. 评价与立场：好 / 坏 / 喜欢 / 讨厌
bool 自然语言知识库类::融合_评价立场(const std::vector<词性节点类*>& 词序列,世界树类& 世界树,场景节点类* 场景)
{
    int 评价索引 = -1;
    int 极性 = 0;

    for (size_t i = 0; i < 词序列.size(); ++i) {
        auto t = 取词文本(词序列[i]);
        if (t == "好" || t == "喜欢" || t == "满意" || t == "棒") {
            评价索引 = static_cast<int>(i);
            极性 = +1;
            break;
        }
        else if (t == "坏" || t == "讨厌" || t == "糟糕" || t == "差") {
            评价索引 = static_cast<int>(i);
            极性 = -1;
            break;
        }
    }

    if (评价索引 == -1) return false;

    // 简化：评价的对象就是评价词左边最近的一个词
    if (评价索引 == 0) return false;
    词性节点类* 对象词 = 词序列[评价索引 - 1];
    auto* 对象存在 = 世界树.确保存在(场景, 对象词);

    世界树.记录评价(对象存在,
        取词文本(词序列[评价索引]),
        极性);
    return true;
}
//11. 来源与引述：A 说 B / 据 A 说，B
bool 自然语言知识库类::融合_来源引述(const std::vector<词性节点类*>& 词序列,世界树类& 世界树,场景节点类* 场景)
{
    int 说索引 = -1;
    for (size_t i = 0; i < 词序列.size(); ++i) {
        auto t = 取词文本(词序列[i]);
        if (t == "说" || t == "表示" || t == "认为") {
            说索引 = static_cast<int>(i);
            break;
        }
    }
    if (说索引 == -1) return false;

    // 左边合并成“来源”，右边合并成“引述内容”
    std::string 来源文本;
    std::string 内容文本;

    for (size_t i = 0; i < 词序列.size(); ++i) {
        auto t = 取词文本(词序列[i]);
        if (t.empty()) continue;

        if (static_cast<int>(i) < 说索引) {
            if (!来源文本.empty()) 来源文本 += " ";
            来源文本 += t;
        }
        else if (static_cast<int>(i) > 说索引) {
            if (!内容文本.empty()) 内容文本 += " ";
            内容文本 += t;
        }
    }

    if (来源文本.empty() || 内容文本.empty()) return false;

    auto* 来源存在 = 世界树.确保抽象来源(场景, 来源文本);
    auto* 事件 = 世界树.记录文本事件(场景, 内容文本);

    世界树.记录信息来源(事件, 来源存在,
        取词文本(词序列[说索引]));
    return true;
}
//12. 定义与分类：A 是 B / A 叫做 B / A 属于 B
bool 自然语言知识库类::融合_定义分类(const std::vector<词性节点类*>& 词序列,世界树类& 世界树,场景节点类* 场景)
{
    int 系词索引 = -1;
    for (size_t i = 0; i < 词序列.size(); ++i) {
        auto t = 取词文本(词序列[i]);
        if (t == "是" || t == "属于" || t == "叫做" || t == "叫") {
            系词索引 = static_cast<int>(i);
            break;
        }
    }
    if (系词索引 <= 0 || 系词索引 + 1 >= static_cast<int>(词序列.size())) {
        return false;
    }

    词性节点类* 个体词 = 词序列[系词索引 - 1];

    std::string 类别文本;
    for (size_t i = 系词索引 + 1; i < 词序列.size(); ++i) {
        auto t = 取词文本(词序列[i]);
        if (!t.empty()) {
            if (!类别文本.empty()) 类别文本 += " ";
            类别文本 += t;
        }
    }

    if (类别文本.empty()) return false;

    auto* 个体 = 世界树.确保存在(场景, 个体词);
    auto* 类别 = 世界树.确保抽象类别(场景, 类别文本);

    世界树.记录定义分类(个体, 类别, 取词文本(词序列[系词索引]));
    return true;
}
//13. 角色标注：非常简化版 A 给 B C
bool 自然语言知识库类::融合_角色标注(const std::vector<词性节点类*>& 词序列,世界树类& 世界树,场景节点类* 场景)
{
    if (词序列.size() < 4) return false;

    int 给索引 = -1;
    for (size_t i = 0; i < 词序列.size(); ++i) {
        if (文本等于(词序列[i], "给")) {
            给索引 = static_cast<int>(i);
            break;
        }
    }
    if (给索引 <= 0 || 给索引 + 2 >= static_cast<int>(词序列.size())) {
        return false;
    }

    词性节点类* 施事词 = 词序列[0];               // 简化：第一个词
    词性节点类* 受益者词 = 词序列[给索引 + 1];
    词性节点类* 主题词 = 词序列.back();           // 简化：最后一个

    auto* 施事 = 世界树.确保存在(场景, 施事词);
    auto* 受益者 = 世界树.确保存在(场景, 受益者词);
    auto* 主题 = 世界树.确保存在(场景, 主题词);

    std::string 事件文本;
    for (auto* 词 : 词序列) {
        auto t = 取词文本(词);
        if (!t.empty()) {
            if (!事件文本.empty()) 事件文本 += " ";
            事件文本 += t;
        }
    }
    auto* 事件 = 世界树.记录文本事件(场景, 事件文本);

    世界树.记录角色关系(事件, "施事", 施事);
    世界树.记录角色关系(事件, "受益者", 受益者);
    世界树.记录角色关系(事件, "主题", 主题);

    return true;
}
//14. 约束与义务：必须 / 应该 / 需要 / 可以
bool 自然语言知识库类::融合_约束义务(const std::vector<词性节点类*>& 词序列,世界树类& 世界树,场景节点类* 场景)
{
    int 模态索引 = -1;
    for (size_t i = 0; i < 词序列.size(); ++i) {
        if (是模态词(词序列[i])) {
            模态索引 = static_cast<int>(i);
            break;
        }
    }
    if (模态索引 == -1) return false;

    // 简化：第一个词为施事
    if (词序列.empty()) return false;
    词性节点类* 主体词 = 词序列[0];
    auto* 主体 = 世界树.确保存在(场景, 主体词);

    std::string 行为文本;
    for (size_t i = 模态索引 + 1; i < 词序列.size(); ++i) {
        auto t = 取词文本(词序列[i]);
        if (!t.empty()) {
            if (!行为文本.empty()) 行为文本 += " ";
            行为文本 += t;
        }
    }
    if (行为文本.empty()) return false;

    auto* 行为事件 = 世界树.记录文本事件(场景, 行为文本);
    世界树.记录义务约束(主体, 行为事件,
        取词文本(词序列[模态索引]));
    return true;
}
//15. 例外与对照：除了 A 以外，B
bool 自然语言知识库类::融合_例外对照(const std::vector<词性节点类*>& 词序列,世界树类& 世界树,场景节点类* 场景)
{
    int 除了索引 = -1;
    int 以外索引 = -1;

    for (size_t i = 0; i < 词序列.size(); ++i) {
        if (文本等于(词序列[i], "除了") || 文本等于(词序列[i], "除")) {
            除了索引 = static_cast<int>(i);
        }
        else if (文本等于(词序列[i], "以外")) {
            以外索引 = static_cast<int>(i);
        }
    }
    if (除了索引 == -1 || 以外索引 == -1 || 以外索引 <= 除了索引) {
        return false;
    }

    // 简化：例外对象是 “除了” 后第一个词，范围对象是 “以外” 后第一个词
    if (除了索引 + 1 >= static_cast<int>(词序列.size())) return false;
    if (以外索引 + 1 >= static_cast<int>(词序列.size())) return false;

    auto* 例外词 = 词序列[除了索引 + 1];
    auto* 范围词 = 词序列[以外索引 + 1];

    auto* 例外存在 = 世界树.确保存在(场景, 例外词);
    auto* 范围存在 = 世界树.确保存在(场景, 范围词);

    世界树.记录例外关系(范围存在, 例外存在, "除了…以外");
    return true;
}
//16. 单位与度量：A 有 3 米 高 / B 重 5 公斤
bool 自然语言知识库类::融合_单位度量(const std::vector<词性节点类*>& 词序列,世界树类& 世界树,场景节点类* 场景)
{
    if (词序列.size() < 3) return false;

    // 找到一个 “数字 + 单位” 组合
    int 数字索引 = -1;
    int 单位索引 = -1;

    for (size_t i = 0; i + 1 < 词序列.size(); ++i) {
        auto 数字词 = 取词文本(词序列[i]);
        if (!是数字串(数字词)) continue;

        if (是单位词(词序列[i + 1])) {
            数字索引 = static_cast<int>(i);
            单位索引 = static_cast<int>(i + 1);
            break;
        }
    }
    if (数字索引 == -1) return false;

    double 数值 = 解析数字(取词文本(词序列[数字索引]));
    std::string 单位 = 取词文本(词序列[单位索引]);

    // 简化：第一个词是被描述的主体
    词性节点类* 主体词 = 词序列[0];
    auto* 主体 = 世界树.确保存在(场景, 主体词);

    // 维度：数字 + 单位 后面的一个词（如果有，比如 “高 / 长 / 重”）
    std::string 维度;
    if (单位索引 + 1 < static_cast<int>(词序列.size())) {
        维度 = 取词文本(词序列[单位索引 + 1]);
    }

    世界树.记录度量特征(主体, 数值, 单位, 维度);
    return true;
}
//17. 指代与省略恢复：他 / 她 / 它 / 那里 / 这样…
bool 自然语言知识库类::融合_指代省略( const std::vector<词性节点类*>& 词序列, 世界树类& 世界树, 场景节点类* 场景)
{
    std::vector<std::string> 代词列表;

    for (auto* 词 : 词序列) {
        if (是代词_指代(词)) {
            auto t = 取词文本(词);
            if (!t.empty()) 代词列表.push_back(t);
        }
    }
    if (代词列表.empty()) return false;

    // 生成一个简单的“指代解析需求”
    std::string 内容 = "需要解析指代：";
    for (size_t i = 0; i < 代词列表.size(); ++i) {
        if (i > 0) 内容 += "，";
        内容 += 代词列表[i];
    }

    世界树.添加信息需求(场景, "指代解析", 内容);
    return true;
}


// ==============================
// 原子结构库 懒加载
// ==============================

std::vector<自然语言知识库类::原子结构条目>& 自然语言知识库类::原子结构库()
{
    static std::vector<原子结构条目> 库;

    if (!库.empty()) {
        return 库;
    }

    // ====== 示例 1：时间定位结构 ======
    {
        原子结构条目 条目;
        条目.类型 = 枚举_原子结构类型::时间定位;
        条目.名称 = "时间定位";

        // 1) 匹配器：在句子里找到所有“时间短语”
        条目.匹配器 = [](const 词序列类型& 词序列,
            std::vector<子句匹配结果>& 输出)
            {
                // 这里只给非常粗糙的示意：
                // 遍历所有词，凡是词性是“时间词”的，就作为一个长度为 1 的时间定位子句。
                for (std::size_t i = 0; i < 词序列.size(); ++i) {
                    词性节点类* 词 = 词序列[i];
                    if (!词) continue;

                    // TODO: 这里用你自己的词性枚举/判断函数替换
                    auto* 词性主信息 = dynamic_cast<词性主信息类*>(词->主信息);
                    if (!词性主信息 ) continue;
                    if (词性主信息->词性 == 枚举_词性::时间词 ) {
                        子句匹配结果 结果;
                        结果.类型 = 枚举_原子结构类型::时间定位;
                        结果.起始 = i;
                        结果.结束 = i + 1;
                        输出.push_back(结果);
                    }
                }
            };

        // 2) 融合器：把时间信息融合到世界树（示意）
        条目.融合器 = [](子句主信息类* 子句,世界树类& 世界树,场景节点类* 默认场景) -> bool
            {
                if (!子句 || !默认场景) return false;

                if (子句->局部词序列.empty()) {
                    子句->刷新局部词序列();
                }
                if (子句->局部词序列.empty()) return false;

                // 直接复用你已经写好的 时间定位 融合逻辑
                bool ok = 自然语言知识库类::融合_时间定位(
                    子句->局部词序列, 世界树, 默认场景);

                if (ok) {
                    子句->对应场景 = 默认场景;
                }
                return ok;
            };


        库.push_back((条目));
    }

    // ====== 示例 2：空间位置结构 ======
    {
        原子结构条目 条目;
        条目.类型 = 枚举_原子结构类型::空间位置;
        条目.名称 = "空间位置";

        条目.融合器 = [](子句主信息类* 子句,
            世界树类& 世界树,
            场景节点类* 默认场景) -> bool
            {
                if (!子句 || !默认场景) return false;

                if (子句->局部词序列.empty()) {
                    子句->刷新局部词序列();
                }
                if (子句->局部词序列.size() < 2) return false;

                bool ok = 自然语言知识库类::融合_空间位置(
                    子句->局部词序列, 世界树, 默认场景);

                if (ok) {
                    子句->对应场景 = 默认场景;
                }
                return ok;
            };


        条目.融合器 = [](子句主信息类* 子句,
            世界树类& 世界树,
            场景节点类* 默认场景) -> bool
            {
                if (!子句 || !默认场景) return false;
                if (子句->局部词序列.empty()) {
                    子句->刷新局部词序列();
                }
                if (子句->局部词序列.size() < 2) return false;

                // TODO: 从局部词序列中抽取“参照存在 + 方位/地点”等信息，
                //       在 默认场景 下为相应存在添加空间特征（坐标/相对位置）
                子句->对应场景 = 默认场景;
                return true;
            };

        库.push_back((条目));
    }

    // TODO: 继续按这个模式，把剩下的 15 种原子结构条目补齐：
    // - 存在与命名
    // - 属性与状态
    // - 动态与事件
    // - 数量与范围
    // - 比较与排序
    // - 否定与排除
    // - 疑问与信息需求
    // - 条件与假设
    // - 因果与解释
    // - 定义与分类
    // - 角色与参与者
    // - 评价与立场
    // - 约束与义务
    // - 来源与引述
    // - 指代与省略恢复
    // 每条都提供一个匹配器 + 一个融合器

    return 库;
}
void 自然语言知识库类::融合整篇文章(文章节点类* 文章, 世界树类& 世界树)
{
    if (!文章) return;

    // 文章没有任何段落，直接返回
    auto* 段落起点 = 文章->子;
    if (!段落起点) return;

    auto* 段落游标 = 段落起点;
    do {
        // 安全：如果这个段落没有任何句子，就跳过
        auto* 句子起点 = 段落游标->子;
        if (句子起点) {
            auto* 句子游标 = 句子起点;
            do {
                // 可选：做一次类型检查，确保真的是“自然句节点”
                if (句子游标 && 句子游标->主信息) {
                    if (dynamic_cast<自然句主信息类*>(句子游标->主信息)) {
                        融合自然句(
                            static_cast<自然句节点类*>(句子游标),
                            世界树,
                           nullptr
                        );
                    }
                }

                句子游标 = 句子游标->下;
            } while (句子游标 && 句子游标 != 句子起点);
        }

        段落游标 = 段落游标->下;
    } while (段落游标 && 段落游标 != 段落起点);
}
// ==============================
// 根据 自然句节点 推断默认场景
// ==============================

场景节点类* 自然语言知识库类::推断默认场景(自然句节点类* 自然句节点,世界树类& 世界树,场景节点类* 显式默认场景)
{
    if (显式默认场景) {
        return 显式默认场景;
    }

    if (!自然句节点) {
        // 退化策略：如果世界树内部有“当前场景”，你可以在这里返回世界树.当前场景()
        return nullptr;
    }

    // 假设：自然句节点 的父节点 是 段落节点，段落节点 的父节点 是 文章节点
    段落节点类* 段落节点 = 自然句节点->父;
    if (!段落节点) return nullptr;

    文章节点类* 文章节点 = 段落节点->父;
    if (!文章节点) return nullptr;

    auto* 文章主信息 = dynamic_cast<文章主信息类*>(文章节点->主信息);
    if (!文章主信息) return nullptr;

    // 如果文章已经挂了“对应文本世界场景”，优先用它
    if (文章主信息->对应文本世界场景) {
        return 文章主信息->对应文本世界场景;
    }

    // 否则，你可以在这里新建一个“文本世界场景”，并挂回文章主信息
    // 伪代码示意：
    /*
    场景主信息类 场景主信息;
    场景主信息.名称      = 文章主信息->标题;
    场景主信息.世界类型  = 枚举_世界类型::文本世界;
    场景主信息.来源文章  = 文章节点;

    场景节点类* 新场景 = 世界树.新建场景(nullptr, 场景主信息);
    文章主信息->对应文本世界场景 = 新场景;
    return 新场景;
   

    return nullptr;
}

// ==============================
// 融合自然句()
// ==============================

bool 自然语言知识库类::融合自然句(自然句节点类* 自然句节点,世界树类& 世界树,场景节点类* 默认场景)
{
    if (!自然句节点) {
        throw std::invalid_argument("融合自然句: 自然句节点 为空!");
    }

    auto* 自然句主信息 = dynamic_cast<自然句主信息类*>(自然句节点->主信息);
    if (!自然句主信息) {
        throw std::runtime_error("融合自然句: 自然句节点 的 主信息 不是 自然句主信息类!");
    }

    词序列类型& 词序列 = 自然句主信息->词序列;
    if (词序列.empty()) {
        return false;
    }

    // 1. 确定这一句对应的“默认场景”（通常是文章对应的文本世界场景）
    场景节点类* 使用场景 = 推断默认场景(自然句节点, 世界树, 默认场景);

    // 2. 收集所有原子结构匹配结果
    std::vector<子句匹配结果> 全部匹配;

    auto& 库 = 原子结构库();
    for (const auto& 条目 : 库) {
        if (!条目.匹配器) continue;
        条目.匹配器(词序列, 全部匹配);
    }

    if (全部匹配.empty()) {
        // 新子句流水线暂时识别不出结构，就回退到老的句子级 17 项融合逻辑
        return 尝试融合(词序列, 世界树, 使用场景);
        
    }

    // 3. 对匹配结果排序（按起始位置优先，其次按区间长度）
    std::sort(全部匹配.begin(), 全部匹配.end(),
        [](const 子句匹配结果& a, const 子句匹配结果& b) {
            if (a.起始 != b.起始) return a.起始 < b.起始;
            return (a.结束 - a.起始) > (b.结束 - b.起始); // 同起点时，长的在前
        });

    // 4. 简单去重 / 去掉完全被覆盖的区间（你也可以实现更复杂的冲突解决策略）
    std::vector<子句匹配结果> 精简匹配;
    for (const auto& 当前 : 全部匹配) {
        bool 被完全覆盖 = false;
        for (const auto& 已选 : 精简匹配) {
            if (当前.起始 >= 已选.起始 && 当前.结束 <= 已选.结束) {
                被完全覆盖 = true;
                break;
            }
        }
        if (!被完全覆盖) {
            精简匹配.push_back(当前);
        }
    }

    bool 全局成功 = false;

    // 方便：给自然句主信息的子句节点列表清空/准备
    自然句主信息->子句节点列表.clear();

    // 5. 对每个子句区间生成 子句主信息 + 子句节点，并调用对应融合器
    for (const auto& 匹配 : 精简匹配) {
        if (匹配.起始 >= 匹配.结束 ||
            匹配.结束 > 词序列.size()) {
            continue;
        }

        // 5.1 新建 子句主信息
        auto* 子句主 = new 子句主信息类(
            自然句主信息,
            匹配.类型,
            匹配.起始,
            匹配.结束
        );

        // 填充局部词序列
        子句主->刷新局部词序列();

        // 5.2 在自然语言树中挂一个 子句节点      
        子句节点类* 子句节点 = 全局变量::语言树.添加子句(自然句节点, 子句主);    
        自然句主信息->子句节点列表.push_back(子句节点);

        // 5.3 找到该类型对应的 原子结构条目，调用融合器
        for (const auto& 条目 : 库) {
            if (条目.类型 != 匹配.类型) continue;
            if (!条目.融合器) continue;

            bool 成功 = 条目.融合器(子句主, 世界树, 使用场景);
            if (成功) {
                全局成功 = true;
            }
        }
    }

    return 全局成功;
}
*/