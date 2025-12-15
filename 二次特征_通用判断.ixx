export module 二次特征_通用判断;

import <string>;
import <vector>;
import <functional>;
import <cmath>;
import 主信息定义模块;


// 取值器：从“存在/特征/状态/场景”抽取可比较的值（标量、向量、集合、轮廓…）


// —— 简单运算器占位实现 ——
inline 结构体_运算结果 运算_差值(const 结构体_取值& a, const 结构体_取值& b) {
    结构体_运算结果 r; if (!(a.有效 && b.有效 && !a.标量.empty() && !b.标量.empty())) return r;
    r.有效 = true; r.标量 = a.标量[0] - b.标量[0]; r.符号 = (r.标量 > 0) - (r.标量 < 0); return r;
}
inline 结构体_运算结果 运算_排序(const 结构体_取值& a, const 结构体_取值& b) {
    结构体_运算结果 r; if (!(a.有效 && b.有效 && !a.标量.empty() && !b.标量.empty())) return r;
    r.有效 = true; r.符号 = (a.标量[0] > b.标量[0]) - (a.标量[0] < b.标量[0]); return r;
}
inline 结构体_运算结果 运算_阈值(const 结构体_取值& a, const 结构体_取值& b) {
    结构体_运算结果 r; if (!(a.有效 && b.有效 && !a.标量.empty() && !b.标量.empty())) return r;
    r.有效 = true; r.标量 = std::llabs(a.标量[0] - b.标量[0]); return r;
}
inline 结构体_判定 判定_比较(const 结构体_运算结果& op, 枚举_判据 p, double τ浮, int64_t τ整) {
    结构体_判定 d; if (!op.有效) return d;
    switch (p) {
    case 枚举_判据::大于: d.通过 = (op.标量 > τ整) || (op.浮点 > τ浮); break;
    case 枚举_判据::小于: d.通过 = (op.标量 < τ整) || (op.浮点 < τ浮); break;
    case 枚举_判据::等于: d.通过 = (op.标量 == τ整) || (std::abs(op.浮点 - τ浮) < 1e-9); break;
    case 枚举_判据::绝对小于阈值: d.通过 = (std::llabs(op.标量) <= τ整) || (std::abs(op.浮点) <= τ浮); break;
    case 枚举_判据::绝对大于阈值: d.通过 = (std::llabs(op.标量) > τ整) || (std::abs(op.浮点) > τ浮); break;
    case 枚举_判据::枚举等于: d.通过 = (op.枚举关系 == (int)τ整); break;
    default: break;
    }
    d.置信度 = d.通过 ? 1.0 : 0.0; return d;
}

// —— 证据/结果节点生成（占位，后续补齐为你的节点创建逻辑） ——
export 二次特征节点类* 生成_证据二次特征(
    场景节点类* /*场景*/, 基础信息节点类* /*A*/, 基础信息节点类* /*B*/,
    枚举_运算类型 /*运算*/, const 结构体_运算结果& /*op*/) {
    return nullptr;
}

export 基础信息节点类* 生成_判断结果(
    场景节点类* /*场景*/, 词性节点类* /*名称*/, 语素节点类* /*类型*/,
    const 结构体_判定& /*判*/, 二次特征节点类* /*证据1*/, 二次特征节点类* /*证据2*/) {
    return nullptr;
}

export 结构体_判断输出 执行_通用判断(场景节点类* 场景, const 结构体_判断规格& 规) {
    结构体_判断输出 out;
    if (!(规.A && 规.B && 规.取值器A && 规.取值器B)) return out;
    结构体_取值 a = 规.取值器A({ 规.A,规.B,场景 });
    结构体_取值 b = 规.取值器B({ 规.A,规.B,场景 });
    结构体_运算结果 op{};
    switch (规.运算) {
    case 枚举_运算类型::差值: op = 运算_差值(a, b); break;
    case 枚举_运算类型::排序: op = 运算_排序(a, b); break;
    case 枚举_运算类型::阈值比较: op = 运算_阈值(a, b); break;
    default: op = 运算_差值(a, b); break;
    }
    auto 判 = 判定_比较(op, 规.判据, 规.阈值_浮, 规.阈值_整);
    out.证据节点1 = 生成_证据二次特征(场景, 规.A, 规.B, 规.运算, op);
    out.结果节点 = 生成_判断结果(场景, 规.输出名称, 规.输出类型, 判, out.证据节点1, nullptr);
    out.成功 = 判.通过; out.置信度 = 判.置信度;
    return out;
}

//// —— 示例取值器（位置Y/矢量标量） ——
//export inline 取值器 取值_位置Y() {
//    return [](结构体_取值器输入 in)->结构体_取值 {
//        结构体_取值 out; if (!in.A) return out;
//        auto* 特A = dynamic_cast<存在节点主信息类*>(in.A->主信息);
//        if (!特A) return out; out.有效 = true; out.标量 = { 特A->绝对坐标.y }; return out;
//        };
//}
//export inline 取值器 取值_矢量特征标量() {
//    return [](结构体_取值器输入 in)->结构体_取值 {
//        结构体_取值 out; if (!in.A) return out;
//        特征节点主信息类* 特A = dynamic_cast<特征节点主信息类*>(in.A->主信息);
//        矢量特征值节点主信息类* 特B = dynamic_cast<矢量特征值节点主信息类*>(特A->值);
//        if (!特A || !特B) return out;               
//            out.有效 = true; out.标量 = { 特B->值 };
//            return out;
//        };
//}
