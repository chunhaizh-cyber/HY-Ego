#pragma once

#include "特征当前值.语言桥.h"

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace 海中鱼巣 {

class 新_语言类;
class 新_词类;
class 概念_名称关系类;
class 概念_存在类;
class 概念_特征类;
class 新_存在类;
class 新_特征类;

// 实际项目内验证入口：使用正式装配后的公开服务，不另建临时工程。
// 用例材料为云端/CI 合成夹具时必须在条目中标注；不得称为正式业务数据。

struct 特征当前值语言桥自检条目 final {
    std::string 标识;
    bool 通过 = false;
    bool 合成夹具 = false; // true=非正式生产数据
    std::string 说明;
};

struct 特征当前值语言桥自检结果 final {
    std::size_t 通过数 = 0;
    std::size_t 失败数 = 0;
    std::vector<特征当前值语言桥自检条目> 条目组;
    std::string 摘要;
    bool 成功() const noexcept { return 失败数 == 0; }
};

struct 特征当前值语言桥自检依赖 final {
    稳定编码 语言节点{};
    新_语言类* 语言 = nullptr;
    新_词类* 词 = nullptr;
    概念_名称关系类* 名称 = nullptr;
    概念_存在类* 存在概念 = nullptr;
    概念_特征类* 特征概念 = nullptr;
    新_存在类* 存在 = nullptr;
    新_特征类* 特征 = nullptr;
    // 可选：已知带当前值的存在（正式装配对象）；空则只跑不依赖具体 E 的用例。
    std::optional<稳定编码> 候选存在;
};

特征当前值语言桥自检结果 运行特征当前值语言桥自检(
    const 特征当前值语言桥自检依赖&);

} // namespace 海中鱼巣
