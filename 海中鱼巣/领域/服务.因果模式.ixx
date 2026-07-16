// 文件规则：因果模式服务只做纯值入口编排；不得读取仓库、调用数据操作或发布稳定因果结构。
module;

export module 海中鱼巣.领域.服务.因果模式;

import 海中鱼巣.领域.材料.因果模式;
import 海中鱼巣.领域.算法.因果模式;

export namespace 海中鱼巣 {

class 因果模式业务服务 final {
public:
    static 因果模式结果 形成因果概率候选(const 因果模式请求& 请求) {
        return 执行因果模式(请求);
    }
};

}
