module;

#include <functional>


#include <string>
#include <cstdint>
#include <functional>
#include <deque>
#include <unordered_set>
#include <vector>
#include <mutex>
#include <atomic>
#include <type_traits>
#include <cstdint>
export module 学习模块;


import 基础数据类型模块;
import 方法环境模块;
import 本能动作管理模块;
import 方法模块;
import 数据仓库模块;
import 语素环境模块;
import 任务模块;


using namespace 数据仓库模块;
export class 学习类 {
public:
    struct 自我学习配置 {
        // 单次扫描最多入队多少个（防止一次把队列塞爆）
        std::size_t 单次扫描入队上限 = 1024;

        // 每个本能函数最多尝试次数（避免失败死循环）
        std::uint32_t 每ID最大尝试次数 = 3;

        // 调度一次最多处理多少个队列项（你可以每 tick 调一次）
        std::size_t 单次调度预算 = 1;
    };

    struct 尝试学习步输出 {
        bool 有执行 = false;
        枚举_本能动作ID id = 枚举_本能动作ID::未定义;

        // 本次调用的执行结果
        结构_方法执行结果 结果{};

        // 是否成功写入方法树（记录本能经验返回了方法首节点）
        bool 已沉淀 = false;

        // 沉淀后的方法首节点（可能为 nullptr）
        方法节点类* 方法首节点 = nullptr;

        // 本ID累计尝试次数
        std::uint32_t 本ID已尝试次数 = 0;
    };


  

  

    class 尝试学习器 {
    public:


    };
};

