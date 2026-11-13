#include "解释.词典释义.h"

#include <algorithm>
#include <array>
#include <string_view>

namespace 海中鱼巣 {
namespace {

// 逐标量切分 UTF-8 文本，返回各标量起始字节位置（末尾追加总长度）；非法返回空。
std::optional<std::vector<std::size_t>> 标量边界(std::string_view 文本) {
    std::vector<std::size_t> 边界{0};
    std::size_t 位置 = 0;
    while (位置 < 文本.size()) {
        const auto 首 = static_cast<unsigned char>(文本[位置]);
        std::size_t 长度 = 首 < 0x80 ? 1 : (首 >> 5) == 0x6 ? 2 : (首 >> 4) == 0xE ? 3 : (首 >> 3) == 0x1E ? 4 : 0;
        if (长度 == 0 || 位置 + 长度 > 文本.size()) return std::nullopt;
        for (std::size_t i = 1; i < 长度; ++i)
            if ((static_cast<unsigned char>(文本[位置 + i]) & 0xC0) != 0x80) return std::nullopt;
        位置 += 长度;
        边界.push_back(位置);
    }
    return 边界;
}

bool 包含(std::string_view 文本, std::string_view 片段) noexcept {
    return 文本.find(片段) != std::string_view::npos;
}

template<std::size_t 数>
bool 属于(std::string_view 词, const std::array<std::string_view, 数>& 组) noexcept {
    return std::find(组.begin(), 组.end(), 词) != 组.end();
}

// 结构外的标点：含这些标点的释义不是单个名词短语，本批不读（7250 §2.6）。
constexpr std::array<std::string_view, 26> 结构外标点{
    "。", "，", ",", "；", ";", "：", ":", "（", "(", "）", ")", "「", "」",
    "“", "”", "《", "》", "、", "！", "？", "!", "?", "/", "【", "】", "…"};
// 另行裁决句型的起始词（指、形容、比喻、姓氏地名人名、同／见／通“X”），遇到返回缺规则。
constexpr std::array<std::string_view, 13> 另裁起始{
    "指", "泛指", "专指", "特指", "形容", "比喻", "姓", "地名", "人名", "同", "见", "通", "即"};
// 字词说明：释义说的是词形、读法、称谓等字词信息，不是X的类别（7250 §2.6另行裁决）。
constexpr std::array<std::string_view, 37> 字词说明中心词{
    "写法", "读法", "简称", "别称", "俗称", "旧称", "尊称", "敬称", "美称", "蔑称", "总称", "统称",
    "合称", "通称", "称呼", "称谓", "名称", "形式", "样子", "状态", "声音", "部分", "一部分",
    "异体", "异体字", "简体", "繁体", "转写", "本字", "俗字", "古字", "缩写", "音译", "译名",
    "意译", "略称", "省称"};
constexpr std::array<std::string_view, 4> 字词说明片段{"异体", "简化字", "繁体", "不确定"};
// 中心词前直接是连词时为并列（A或B），中心词不唯一，本批不读。
constexpr std::array<std::string_view, 6> 并列连词{"或", "和", "及", "与", "或者", "以及"};
// 可读释义的原始词性：名词与词典未标词性；其余词性（动词、形容词、成语等）的释义不是名词短语。
constexpr std::array<std::string_view, 2> 可读词性{"noun", "unknown"};

词典释义解读结果 缺规则(词典释义解读结果 结果, std::string_view 诊断) {
    结果.状态 = 词典释义解读状态::缺规则;
    结果.诊断.assign(诊断);
    return 结果;
}

} // namespace

词典释义解读结果 解读词典释义行(const 离线词典数据源& 数据源, std::string_view 被释词面,
    std::string_view 释义行, const std::optional<std::string>& 记录词性) {
    词典释义解读结果 结果;
    结果.释义原文.assign(释义行);
    if (!数据源.释义辅助表就绪()) {
        结果.状态 = 词典释义解读状态::辅助表未配置;
        结果.诊断 = 数据源.释义辅助表诊断();
        return 结果;
    }
    std::string_view 文本 = 释义行;
    while (!文本.empty() && (文本.front() == ' ' || 文本.front() == '\t')) 文本.remove_prefix(1);
    while (!文本.empty() && (文本.back() == ' ' || 文本.back() == '\t' || 文本.back() == '\r')) 文本.remove_suffix(1);
    constexpr std::string_view 句号 = "。";
    if (文本.size() >= 句号.size() && 文本.substr(文本.size() - 句号.size()) == 句号)
        文本.remove_suffix(句号.size());
    if (文本.empty()) { 结果.诊断 = "释义为空"; return 结果; }
    const auto 规范 = 数据源.繁简规范化(文本);
    const auto 被释规范 = 数据源.繁简规范化(被释词面);
    if (!规范 || !被释规范) { 结果.诊断 = "释义或被释词面不是合法 UTF-8"; return 结果; }
    结果.规范化释义 = *规范;
    const auto 边界 = 标量边界(结果.规范化释义);
    if (!边界) { 结果.诊断 = "规范化释义不是合法 UTF-8"; return 结果; }

    if (记录词性 && !属于(*记录词性, 可读词性)) return 缺规则(std::move(结果), "词性不适用：" + *记录词性);
    for (const auto 标点 : 结构外标点)
        if (包含(结果.规范化释义, 标点)) return 缺规则(std::move(结果), "含结构外标点");
    for (const auto 片段 : 字词说明片段)
        if (包含(结果.规范化释义, 片段)) return 缺规则(std::move(结果), "字词说明");
    for (const auto 起始 : 另裁起始)
        if (结果.规范化释义.rfind(起始, 0) == 0) return 缺规则(std::move(结果), "另行裁决句型");

    // 以词典词头表正向最长匹配切分；未匹配的单个标量成段且标为非词头。
    const auto& 位 = *边界;
    const auto 标量总数 = 位.size() - 1;
    const auto 最长 = std::max<std::size_t>(1, 数据源.最长词头标量数());
    for (std::size_t 起 = 0; 起 < 标量总数;) {
        std::size_t 止 = 起 + 1;
        bool 词头 = false;
        for (std::size_t 长 = std::min(最长, 标量总数 - 起); 长 >= 1; --长) {
            const auto 候选 = std::string_view(结果.规范化释义).substr(位[起], 位[起 + 长] - 位[起]);
            if (数据源.是规范化词头(候选)) { 止 = 起 + 长; 词头 = true; break; }
            if (长 == 1) break;
        }
        结果.词段组.push_back({起, 止, 结果.规范化释义.substr(位[起], 位[止] - 位[起]), 词头});
        起 = 止;
    }
    const auto& 段 = 结果.词段组;
    const auto n = 段.size();
    const auto 词 = [&](std::size_t i) -> std::string_view { return 段[i].规范化词面; };
    // 中心词可用：是词头、不是结构固定词、不是字词说明、不与被释词相同、前面不是并列连词。
    const auto 中心可用 = [&](std::size_t i) {
        return 段[i].是词头 && 词(i) != "的" && 词(i) != "一种" && 词(i) != "一类"
            && !属于(词(i), 字词说明中心词) && 词(i) != *被释规范
            && (i == 0 || !属于(词(i - 1), 并列连词));
    };
    const auto 定结构 = [&](词典释义结构 结构, std::size_t 中心, std::size_t 修饰起, std::size_t 修饰止) {
        结果.状态 = 词典释义解读状态::已解读;
        结果.结构 = 结构;
        结果.中心词 = 段[中心];
        if (修饰起 < 修饰止) {
            结果.修饰起始标量 = 段[修饰起].起始标量;
            结果.修饰结束标量 = 段[修饰止 - 1].结束标量;
        }
        return 结果;
    };
    // D02：M? N 的 一种／一类
    if (n >= 3 && (词(n - 1) == "一种" || 词(n - 1) == "一类") && 词(n - 2) == "的") {
        if (!中心可用(n - 3)) return 缺规则(std::move(结果), "N的一种：中心词不可用");
        return 定结构(词典释义结构::N的一种, n - 3, 0, n - 3);
    }
    // D01：一种 M? N
    if (n >= 2 && 词(0) == "一种") {
        if (!中心可用(n - 1)) return 缺规则(std::move(结果), "一种N：中心词不可用");
        return 定结构(词典释义结构::一种N, n - 1, 1, n - 1);
    }
    // D03：M 的 M? N（最后一个“的”之后至少有中心词）
    const bool 含的 = std::any_of(段.begin(), 段.end() - 1, [](const auto& 项) { return 项.规范化词面 == "的"; });
    if (n >= 3 && 含的 && 词(0) != "的") {
        if (!中心可用(n - 1)) return 缺规则(std::move(结果), "定义式：中心词不可用");
        return 定结构(词典释义结构::定义式, n - 1, 0, n - 1);
    }
    return 缺规则(std::move(结果), "无结构匹配");
}

std::vector<词典释义解读结果> 解读词典义项释义(const 离线词典数据源& 数据源, const 词典义项材料& 义项) {
    std::vector<词典释义解读结果> 结果组;
    const auto 词性 = 数据源.释义辅助表就绪() ? 数据源.读取记录词性(义项.记录键) : std::nullopt;
    std::string_view 全文 = 义项.汉语释义;
    for (std::size_t 起 = 0; 起 <= 全文.size();) {
        const auto 止 = std::min(全文.find('\n', 起), 全文.size());
        const auto 行 = 全文.substr(起, 止 - 起);
        if (行.find_first_not_of(" \t\r") != std::string_view::npos)
            结果组.push_back(解读词典释义行(数据源, 义项.词面, 行, 词性));
        起 = 止 + 1;
    }
    return 结果组;
}

} // namespace 海中鱼巣
