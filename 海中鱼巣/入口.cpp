// 文件规则：入口只接收参数、调用启动解析与唯一顶层程序并映射退出码。
#include "启动选项.数据.h"
#include "程序运行结果.数据.h"

#include <string_view>

import 海中鱼巣.启动.程序入口;
import 海中鱼巣.启动.应用程序;
import 海中鱼巣.端到端测试.需求服务;

int main(int 参数数量, char* 参数组[]) {
    // 端到端测试模式：直接运行需求服务端到端测试后返回
    for (int 索引 = 1; 索引 < 参数数量; ++索引) {
        if (参数组[索引] != nullptr
            && std::string_view{参数组[索引]} == "--test-需求") {
            return 海中鱼巣::运行需求服务端到端测试();
        }
    }
    const auto 解析 = 海中鱼巣::解析并验证启动选项(参数数量, 参数组);
    if (!解析.成功()) {
        return 海中鱼巣::映射进程退出码(解析, nullptr);
    }
    const auto 运行 = 海中鱼巣::运行海中鱼巣(解析.选项);
    return 海中鱼巣::映射进程退出码(解析, &运行);
}
