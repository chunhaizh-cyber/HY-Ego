// 文件规则：用途事件业务服务只编排纯值冻结与效用裁决；不得持有仓库、许可、会话、锁或持久状态。
module;

export module 海中鱼巣.领域.服务.用途事件;

import 海中鱼巣.领域.材料.用途事件;
import 海中鱼巣.领域.算法.用途事件;

export namespace 海中鱼巣 {

class 用途事件业务服务 final {
public:
    static 用途来源冻结结果 冻结用途来源(const 用途来源冻结请求& 请求) {
        return 执行用途来源冻结(请求);
    }

    static 用途事件结果 形成结构化结果效用(const 用途事件请求& 请求) {
        return 执行结构化结果效用(请求);
    }
};

}
