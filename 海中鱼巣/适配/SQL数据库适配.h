// 文件规则：SQL 数据库适配只维护节点直接持久证据所需的数据库连接前置；不得承载业务事实解释。
#pragma once

#include <cstdint>
#include <string>

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
    std::wstring 失败阶段;
    std::wstring 诊断;
};

class SQL数据库适配 {
public:
    explicit SQL数据库适配(SQL数据库配置 配置);

    static SQL数据库配置 从环境创建配置();

    const SQL数据库配置& 读取配置() const;
    数据库操作结果 初始化数据库() const;

private:
    SQL数据库配置 配置_;
};

}
