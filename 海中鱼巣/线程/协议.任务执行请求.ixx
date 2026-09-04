// 文件规则：任务执行请求协议只转出 L4 强类型执行请求值合同与值式复核；不得调用领域入口或承载事务能力。
module;

export module 海中鱼巣.线程.协议.任务执行请求;

export import 海中鱼巣.领域.内部治理.服务.任务执行派发门控;

export namespace 海中鱼巣 {

__declspec(noinline) bool 强类型任务执行请求相同(
    const 任务强类型执行请求& 左,
    const 任务强类型执行请求& 右) noexcept {
    return 左 == 右;
}

__declspec(noinline) bool 强类型任务执行请求完整(
    const 任务强类型执行请求& 请求) noexcept {
    return 任务执行强类型请求基本有效(请求);
}

} // namespace 海中鱼巣
