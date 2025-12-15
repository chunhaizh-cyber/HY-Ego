export module 评估与提交策略;

import 主信息定义模块;

export struct 结构体_NLU评估值 {
    double 覆盖率 = 0.0, 指代解析率 = 0.0, 全局置信度 = 0.0;
    int 冲突数 = 0; bool 可执行 = false;
    double 时间误差 = 0.0, 空间误差 = 0.0;
    double 比较正确率 = 0.0, 否定正确率 = 0.0, 程度正确率 = 0.0;
};

export struct 结构体_NLU阈值 {
    double T_cov = 0.85, T_coref = 0.80, T_submit = 0.72;
    int N_conf_max = 0; bool 必须可执行 = true;
    double E_time_max = 0.5, E_space_max = 0.5;
    double T_cmp = 0.75, T_neg = 0.90, T_deg = 0.75;
};

export 结构体_NLU评估值 评估_自然语言理解(场景节点类* /*临时场景*/) {
    // TODO: 从场景统计并回填评估值（可调用通用判断生成器）
    return {};
}

export bool 判定_是否提交(const 结构体_NLU评估值& v, const 结构体_NLU阈值& t) {
    const bool 硬 =
        v.覆盖率 >= t.T_cov && v.指代解析率 >= t.T_coref &&
        v.冲突数 <= t.N_conf_max && (!t.必须可执行 || v.可执行) &&
        v.全局置信度 >= t.T_submit && v.时间误差 <= t.E_time_max && v.空间误差 <= t.E_space_max;
    const bool 软 =
        v.比较正确率 >= t.T_cmp && v.否定正确率 >= t.T_neg && v.程度正确率 >= t.T_deg;
    return 硬 && 软;
}
