// 文件规则：SQL 数据库适配只写审计投影和读取人读材料；不得裁决或恢复运行期机器事实。
#pragma once

#include "../领域/统计服务.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace 海中鱼巣 {

struct SQL数据库配置 {
    std::wstring 服务器 = L".\\SQLEXPRESS";
    std::wstring 数据库 = L"海中鱼巣";
    std::uint32_t 连接超时秒 = 3;
};

struct 数据库操作结果 {
    bool 成功 = false;
    bool 逻辑内返回 = false;
    bool 追根因错误 = false;
    std::uint64_t 审计编号 = 0;
    std::wstring 失败阶段;
    std::wstring 诊断;
};

struct 数据库审计记录 {
    std::uint64_t 审计编号 = 0;
    std::wstring 记录时间;
    std::wstring 来源入口;
    std::uint64_t 来源版本 = 0;
    std::uint64_t 规则版本 = 0;
    std::uint64_t 节点数 = 0;
    std::uint64_t 关系数 = 0;
    std::uint64_t 索引数 = 0;
};

struct 数据库查询结果 {
    数据库操作结果 操作;
    std::vector<数据库审计记录> 记录组;
};

class SQL数据库适配 {
public:
    explicit SQL数据库适配(SQL数据库配置 配置);

    static SQL数据库配置 从环境创建配置();

    const SQL数据库配置& 读取配置() const;
    数据库操作结果 初始化数据库() const;
    数据库操作结果 写入结构统计审计(const 结构统计快照& 快照, std::wstring 来源入口) const;
    数据库查询结果 读取最近结构统计审计(std::size_t 数量上限 = 50) const;

private:
    SQL数据库配置 配置_;
};

}
