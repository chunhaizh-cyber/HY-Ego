#include "自检.语言词条匹配.h"

#include <cstdio>
#include <stdexcept>
#include <vector>

namespace 海中鱼巣 {
namespace {

void 要求(bool 成立, const char* 说明) {
    if (!成立) throw std::runtime_error(说明);
}

// 与生产 trie 解码独立：已校验 UTF-8 中非续字节的数量即码点数。
std::size_t 码点数量(const std::string& 文本) noexcept {
    std::size_t 数量 = 0;
    for (const unsigned char 字节 : 文本) {
        if ((字节 & 0xC0) != 0x80) ++数量;
    }
    return 数量;
}

struct 只读词库材料 final {
    新语言信息 语言;
    std::vector<稳定编码> 词条组;
    std::vector<新词条信息> 词信息组;
    friend bool operator==(const 只读词库材料&, const 只读词库材料&) = default;
};

只读词库材料 读取材料(const 新_语言类& 语言服务,
    const 新_词类& 词服务, 稳定编码 语言节点) {
    const auto 语言 = 语言服务.获取语言(语言节点);
    要求(语言.状态 == 新语言查询状态::已找到 && 语言.信息
        && 语言.信息->节点 == 语言节点, "读取语言失败");
    const auto 词组 = 词服务.查询语言词条(语言节点);
    要求((词组.状态 == 新词条查询状态::已找到 && !词组.词条节点组.empty())
        || (词组.状态 == 新词条查询状态::未找到 && 词组.词条节点组.empty()),
        "读取词条列表失败");
    只读词库材料 材料{*语言.信息, 词组.词条节点组, {}};
    for (const auto 节点 : 材料.词条组) {
        const auto 词 = 词服务.获取词条(节点);
        要求(词.状态 == 新词条查询状态::已找到 && 词.信息
            && 词.信息->节点 == 节点 && 词.信息->语言节点 == 语言节点
            && 新_语言类::严格UTF8字节串有效(词.信息->规范化词面),
            "逐项读取词条失败");
        材料.词信息组.push_back(*词.信息);
    }
    return 材料;
}

// 小规模只读自检参照：每个合法起点线性遍历 DTO 词面，不复用生产 trie。
std::vector<新语言词条匹配片段> 计算参照片段(
    const std::string& 文本, const std::vector<新词条信息>& 词组) {
    std::vector<新语言词条匹配片段> 片段组;
    std::size_t 字节起点 = 0;
    std::size_t 码点起点 = 0;
    while (字节起点 < 文本.size()) {
        const 新词条信息* 最长 = nullptr;
        for (const auto& 词 : 词组) {
            if (文本.compare(字节起点, 词.规范化词面.size(), 词.规范化词面) == 0
                && (!最长 || 词.规范化词面.size() > 最长->规范化词面.size())) {
                最长 = &词;
            }
        }
        std::size_t 字节长度 = 1;
        if (最长) {
            字节长度 = 最长->规范化词面.size();
        } else {
            while (字节起点 + 字节长度 < 文本.size()
                && (static_cast<unsigned char>(文本[字节起点 + 字节长度]) & 0xC0) == 0x80) {
                ++字节长度;
            }
        }
        const auto 原文 = 文本.substr(字节起点, 字节长度);
        const auto 数量 = 码点数量(原文);
        if (!最长 && !片段组.empty() && !片段组.back().词条节点) {
            auto& 末段 = 片段组.back();
            末段.原文字节长度 += 字节长度;
            末段.原文码点数量 += 数量;
            末段.原文 += 原文;
        } else {
            片段组.push_back({最长 ? std::optional<稳定编码>{最长->节点} : std::nullopt,
                字节起点, 字节长度, 码点起点, 数量, 原文});
        }
        字节起点 += 字节长度;
        码点起点 += 数量;
    }
    return 片段组;
}

void 核对匹配(const 新_语言类& 语言服务, const 新_词类& 词服务,
    稳定编码 语言节点, const std::string& 文本,
    const std::vector<新语言词条匹配片段>& 预期, const char* 项目) {
    const auto 结果 = 语言服务.按已登记词条最长匹配(语言节点, 文本, 词服务);
    要求(结果.状态 == 新语言查询状态::已找到 && 结果.片段组 == 预期, 项目);
    std::string 拼接;
    for (const auto& 片段 : 结果.片段组) 拼接 += 片段.原文;
    要求(拼接 == 文本, "原文覆盖/保留失败");
    std::printf("PASS: %s\n", 项目);
}

int 样本不足(const char* 原因) noexcept {
    std::printf("NOT_RUN: 语言词条匹配自检，%s；未新建任何样本。\n", 原因);
    return 3;
}

} // namespace

int 运行语言词条匹配自检(
    const 新_语言类& 语言服务, const 新_词类& 词服务,
    稳定编码 语言节点, const std::string& 短词面,
    const std::string& 长词面, const std::string& 未登记文本) noexcept {
    try {
        const auto 之前 = 读取材料(语言服务, 词服务, 语言节点);
        if (!新_语言类::严格UTF8字节串有效(短词面)
            || !新_语言类::严格UTF8字节串有效(长词面)
            || !新_语言类::严格UTF8字节串有效(未登记文本)
            || 长词面.size() <= 短词面.size() || !长词面.starts_with(短词面)
            || 码点数量(长词面) == 长词面.size()
            || 码点数量(未登记文本) == 未登记文本.size()) {
            return 样本不足("需要合法短前缀、多字节长词和多字节未知材料");
        }
        std::optional<稳定编码> 短词;
        std::optional<稳定编码> 长词;
        for (const auto& 词 : 之前.词信息组) {
            if (词.规范化词面 == 短词面) 短词 = 词.节点;
            if (词.规范化词面 == 长词面) 长词 = 词.节点;
        }
        if (!短词 || !长词) return 样本不足("短词或长词尚未登记");
        const auto 未知预期 = 计算参照片段(未登记文本, 之前.词信息组);
        if (未知预期.size() != 1 || 未知预期.front().词条节点) {
            return 样本不足("未知材料实际含已登记词");
        }
        std::string 分隔;
        std::size_t 起点 = 0;
        while (起点 < 未登记文本.size()) {
            auto 终点 = 起点 + 1;
            while (终点 < 未登记文本.size()
                && (static_cast<unsigned char>(未登记文本[终点]) & 0xC0) == 0x80) ++终点;
            const auto 候选 = 未登记文本.substr(起点, 终点 - 起点);
            bool 可以分隔 = true;
            for (const auto& 词 : 之前.词信息组) {
                if (词.规范化词面.starts_with(候选)) 可以分隔 = false;
            }
            const auto 混合预期 = 计算参照片段(候选 + 长词面 + 候选, 之前.词信息组);
            if (可以分隔 && 混合预期.size() == 3
                && 混合预期[1].词条节点 == 长词 && 混合预期[1].原文 == 长词面) {
                分隔 = 候选;
                break;
            }
            起点 = 终点;
        }
        if (分隔.empty()) return 样本不足("没有不会引发跨界长词的未知分隔码点");

        核对匹配(语言服务, 词服务, 语言节点, 长词面,
            {{长词, 0, 长词面.size(), 0, 码点数量(长词面), 长词面}},
            "长词优先短前缀/多字节区间");
        核对匹配(语言服务, 词服务, 语言节点, 未登记文本,
            {{std::nullopt, 0, 未登记文本.size(), 0, 码点数量(未登记文本), 未登记文本}},
            "连续未知原样保留/多字节区间");
        核对匹配(语言服务, 词服务, 语言节点, "", {}, "空文本");
        const auto 混合文本 = 分隔 + 长词面 + 分隔;
        核对匹配(语言服务, 词服务, 语言节点, 混合文本,
            {{std::nullopt, 0, 分隔.size(), 0, 1, 分隔},
             {长词, 分隔.size(), 长词面.size(), 1, 码点数量(长词面), 长词面},
             {std::nullopt, 分隔.size() + 长词面.size(), 分隔.size(),
                1 + 码点数量(长词面), 1, 分隔}}, "已登记与未登记混合");
        const auto 重复 = 混合文本 + 长词面;
        核对匹配(语言服务, 词服务, 语言节点, 重复,
            计算参照片段(重复, 之前.词信息组), "重复词条/保留位置");

        const std::vector<std::string> 非法组{
            "\x80", "\xC0\xAF", "\xE4\xB8", "\xED\xA0\x80",
            "\xF4\x90\x80\x80", "\xEF\xBB\xBF", "\xEF\xBF\xBD",
            std::string("x") + "\xEF\xBB\xBF"};
        for (const auto& 非法 : 非法组) {
            const auto 结果 = 语言服务.按已登记词条最长匹配(语言节点, 非法, 词服务);
            要求(结果.状态 == 新语言查询状态::输入不合法 && 结果.片段组.empty(),
                "非法编码必须拒绝且无部分结果");
        }
        std::printf("PASS: 非法/截断/过长/代理/BOM/替换字符拒绝\n");
        const std::vector<std::string> 精确材料组{
            std::string("\0", 1), std::string(" \tAa") + 长词面 + "Ａａ!",
            std::string("\xC3\xA9") + "e\xCC\x81", "\xF0\x9F\x98\x80"};
        for (const auto& 文本 : 精确材料组) {
            核对匹配(语言服务, 词服务, 语言节点, 文本,
                计算参照片段(文本, 之前.词信息组), "NUL/大小写/空白/全角/组合码点/四字节精确处理");
        }
        const auto 无效语言 = 语言服务.按已登记词条最长匹配({}, 长词面, 词服务);
        要求(无效语言.状态 == 新语言查询状态::输入不合法 && 无效语言.片段组.empty(),
            "无效语言编码未拒绝");
        const auto 之后 = 读取材料(语言服务, 词服务, 语言节点);
        要求(之前 == 之后, "读取材料发生漂移，无法证明前后不变");
        std::printf("PASS: 语言/词条列表/词条完整DTO前后一致；全局瞬时写入不在观测范围\n");
        return 0;
    } catch (const std::exception& 错误) {
        std::printf("FAIL: 语言词条匹配自检：%s\n", 错误.what());
        return 1;
    } catch (...) {
        std::printf("FAIL: 语言词条匹配自检：未知工程异常\n");
        return 1;
    }
}

} // namespace 海中鱼巣
