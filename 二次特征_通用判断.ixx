export module 二次特征_通用判断;

import <string>;
import <vector>;
import <functional>;
import <cmath>;
import 主信息定义模块;
// 2) 方法层：整体理解（可成长）
export struct 结构体_整体理解结果 {
    场景节点类* 临时场景;
    double 全局置信度;
    int 未解析指代数;
    int 冲突数;
};

export struct 结构体_自然语言理解输出 {
    场景节点类* 临时场景;
    double 全局置信度;
    std::vector<基础信息节点类*> 未决分支;
    bool 是否提交;
};
export struct 结构体_NLU评估 {
    double 覆盖率;
    double 指代解析率;
    int    矛盾数;
    double 全局置信度;
    double 时间误差;
    double 空间误差;
    bool   可执行;
};



export struct 结构体_NLU评估值 {
    double 覆盖率 = 0.0;
    double 指代解析率 = 0.0;
    int    冲突数 = 0;
    bool   可执行 = false;
    double 全局置信度 = 0.0;
    double 时间误差 = 0.0;
    double 空间误差 = 0.0;
    double 比较正确率 = 0.0;
    double 否定正确率 = 0.0;
    double 程度正确率 = 0.0;
};

export struct 结构体_NLU阈值 {
    double T_cov = 0.85;
    double T_coref = 0.80;
    int    N_conf_max = 0;
    bool   必须可执行 = true;
    double T_submit = 0.72;
    double E_time_max = 0.5;   // 自定单位
    double E_space_max = 0.5;  // 自定单位
    double T_cmp = 0.75;
    double T_neg = 0.90;
    double T_deg = 0.75;
};

export struct 结构体_NLU评估结果 {
    结构体_NLU评估值 值;
    bool 通过提交 = false;
    bool 局部提交 = false;
    std::vector<基础信息节点类*> 待补证据; // 未决点
};

export struct 结构体_规则_判断远近_Y {
    // 参照设定
    bool 更大为更远 = true;  // false 表示更大为更近
    int64_t 阈值_近 = 0;     // 根据坐标单位设置
};

// 绑定结果（抽象→具体）
export struct 结构体_绑定_判断远近 {
    基础信息节点类* 存在_E1 = nullptr;
    基础信息节点类* 存在_E2 = nullptr;
    特征节点类* 特征_E1_Y = nullptr;
    特征节点类* 特征_E2_Y = nullptr;
};

// 计算产物
export struct 结构体_判断远近输出 {
    二次特征节点类* 二次特征_ΔY = nullptr;
    二次特征节点类* 二次特征_排序 = nullptr;
    基础信息节点类* 结果结论 = nullptr; // 可用特征节点或关系节点表达“远/近/等距”
};




export struct 结构体_判定 { bool 通过 = false; double 置信度 = 1.0; };

// 通用“判断规格”




// 抽象场景模板：占位符+必需特征
export struct 结构体_抽象场景模板 {
    // 抽象存在/特征占位（E1,E2,… Fx …）
    std::vector<基础信息节点类*> 存在占位;   // E1,E2...
    std::vector<特征节点类*>     特征占位;   // Fx...
    // 约束（必须有的字段：名称/类型/坐标/时间/角色等）
    std::vector<int> 必备约束ID; // 简化起见，用ID指代
};

// 语言模板：槽位化的“怎么说”
export struct 结构体_语言模板 {
    枚举_LK用途 用途;
    std::string 模板ID;          // 全局唯一
    // 词序列槽位，如 {主语:E1名, 谓词:动词“有/发生/位于”, 宾语:E2名, 修饰:Fx名…}
    std::vector<词性节点类*> 词序列槽位;
    // 线性化策略（见下：名称组装顺序）
    std::string 线性化策略ID;
};

// 知识条目
export struct 结构体_语言知识条目 {
    结构体_语言模板 语言;
    结构体_抽象场景模板 场景模板;
    // 触发条件：最少输入/上下文要求/缺省单位等
    std::vector<int> 触发条件ID;
    // 生成时需要的“判断/二次特征”配方（可为空）
    std::vector<int> 二次特征配方ID;
    // 输出：形容词 or 短语 or 结构化句子
    bool 输出为短语 = true;
};

export struct 结构体_线性化上下文 {
    场景节点类* 场景;
    std::unordered_map<std::string, 基础信息节点类*> 绑定; // "E1"->节点
};
export using 函数_线性化 = std::function<std::vector<词性节点类*>(const 结构体_语言模板&, const 结构体_线性化上下文&)>;

export struct 结构体_LK生成输入 {
    枚举_LK用途 用途;
    std::string 模板ID;            // 可空：自动选最匹配
    场景节点类* 目标场景;           // 抽象/当前场景
    std::unordered_map<std::string, 基础信息节点类*> 预绑定; // 传入已有绑定（如 E1/E2）
};
export struct 结构体_LK生成输出 {
    短语节点类* 语言节点 = nullptr;
    std::vector<词性节点类*> 词序列;   // 已按策略顺序生成
    std::unordered_map<std::string, 基础信息节点类*> 最终绑定;
    std::vector<二次特征节点类*> 证据链; // 若涉及判断
    bool 成功 = false;
    double 置信度 = 1.0;
};

export struct 结构体_取值器输入 { 基础信息节点类* A; 基础信息节点类* B; 场景节点类* 场景; };
//export struct 结构体_取值 { bool 有效 = false; std::vector<int64_t> 标量; /*或*/ 结构体_空间坐标 坐标; /*...*/ };

export using 取值器 = std::function<结构体_取值(结构体_取值器输入)>;

export struct 结构体_判断规格 {
    基础信息节点类* A = nullptr;
    基础信息节点类* B = nullptr;
    取值器 取值器A; 取值器 取值器B;
    枚举_运算类型 运算 = 枚举_运算类型::差值;
    枚举_判据 判据 = 枚举_判据::大于;
    double 阈值_浮 = 0; int64_t 阈值_整 = 0;
    词性节点类* 输出名称 = nullptr;  // 结果名
    语素节点类* 输出类型 = nullptr;  // 结果类型
};

export struct 结构体_判断输出 {
    二次特征节点类* 证据节点1 = nullptr;
    二次特征节点类* 证据节点2 = nullptr;
    基础信息节点类* 结果节点 = nullptr;
    bool 成功 = false;
    double 置信度 = 1.0;
};
export struct 结构体_运算结果 { bool 有效 = false; int64_t 标量 = 0; double 浮点 = 0; int8_t 符号 = 0; int 枚举关系 = 0; /*证据*/ };

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
