export module 语言学习模块;

import <string>;
import <vector>;
import <unordered_map>;
import 主信息定义模块;
import 语言知识库模块;
import 二次特征_通用判断;
import 评估与提交策略;

export enum class 枚举_交互类型 { 指代确认, 角色确认, 参照系, 单位量纲, 阈值校准, 时间顺序, 意图确认, 关系确认 };

export struct 结构体_句型样本 { /* 同上定义，略 */ };
export struct 结构体_句型候选 { /* 同上定义，略 */ };

export struct 结构体_交互问题 {
    枚举_交互类型 类型;
    std::string 问题文本;
    std::vector<std::string> 选项;
    std::string 期望槽位;
    double 信息增益估计 = 0.0;
};

export struct 结构体_交互答案 {
    std::string 期望槽位;    // 如 "E1"
    std::string 值;          // 文本或枚举，如 "小车" / "厘米" / "先A后B"
};

export 结构体_句型候选 归纳_句型候选(
    const 结构体_句型样本& 样本);

export std::vector<结构体_交互问题> 生成_最小问题集(
    const 结构体_句型候选& 候选, int 上限个数 = 3);

export bool 应用_交互答案(
    结构体_句型候选& 候选,
    const std::vector<结构体_交互答案>& 答案);

export bool 评估并提交_进入LK(
    const 结构体_句型候选& 候选, double& 提交置信度);

// 批量学习：从一批句子样本 → LK模板
export void 批量_句型学习(
    const std::vector<结构体_句型样本>& 样本集);
