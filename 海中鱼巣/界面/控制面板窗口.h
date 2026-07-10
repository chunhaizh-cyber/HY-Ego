// 文件规则：控制面板窗口只显示会话期人读材料；不得持有仓库可写引用、写业务事实或承载机器裁决。
#pragma once

#include "../领域/控制面板服务.h"
#include "../领域/统计服务.h"
#include "../适配/SQL数据库适配.h"

#include <array>
#include <csignal>
#include <memory>
#include <string>

namespace 海中鱼巣 {

struct 控制面板窗口快照 {
    控制面板材料 总览材料;
    std::array<控制面板树视图材料, 6> 树视图材料组;
    控制面板树结构材料 世界树结构材料;
    控制面板操作请求材料 操作请求材料;
    结构统计快照 启动结构统计;
    数据库查询结果 数据库审计查询;
};

struct 控制面板窗口运行结果 {
    bool 成功 = false;
    bool 已显示窗口 = false;
    bool 正常关闭 = false;
    bool 逻辑内返回 = false;
    bool 追根因错误 = false;
    std::wstring 失败阶段;
};

class 控制面板窗口 {
public:
    控制面板窗口(const 控制面板服务& 控制面板, const SQL数据库适配& 数据库,
        结构统计快照 启动结构统计, 控制面板树结构材料 世界树结构材料);
    ~控制面板窗口();

    控制面板窗口(const 控制面板窗口&) = delete;
    控制面板窗口& operator=(const 控制面板窗口&) = delete;
    控制面板窗口(控制面板窗口&&) = delete;
    控制面板窗口& operator=(控制面板窗口&&) = delete;

    控制面板窗口运行结果 运行(const volatile std::sig_atomic_t* 停止请求);

private:
    struct 窗口实现;
    std::unique_ptr<窗口实现> 实现;
};

}
