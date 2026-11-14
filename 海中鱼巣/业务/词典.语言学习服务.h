#pragma once

#include <chrono>
#include <filesystem>
#include <memory>
#include <optional>
#include <stop_token>
#include <string>

#include "../核心/基础数据集.h"

namespace 海中鱼巣 {
class 新_任务类;
class 新_需求类;
class 新_词类;
class 新_特征类;
class 新_语言类;
class 词典材料类;
class 新_自然语言世界树类;
class 离线词典数据源;
class 概念_名称关系类;

struct 新词典语言交付 final {
    稳定编码 任务, 需求, 词条, 材料Q;
};

// 由self单线程调用，所有owner及只读源在最终join后才退出。
class 词典语言学习服务 final {
public:
    词典语言学习服务(新_任务类&, 新_需求类&, 新_词类&, 新_特征类&,
        新_语言类&, 词典材料类&, 新_自然语言世界树类&, const 概念_名称关系类&,
        std::filesystem::path 词典根, std::shared_ptr<const 离线词典数据源> 启动源,
        std::string 原包标识, 稳定编码 材料特征类型, std::chrono::milliseconds 重试间隔);
    ~词典语言学习服务();
    词典语言学习服务(const 词典语言学习服务&) = delete;
    词典语言学习服务& operator=(const 词典语言学习服务&) = delete;
    void 接纳当前任务(稳定编码 T);
    void 接纳执行材料(稳定编码 T, 稳定编码 X);
    std::optional<std::chrono::steady_clock::time_point> 下一推进时点() const noexcept;
    std::optional<新词典语言交付> 推进一次(std::stop_token);
private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
};
} // namespace 海中鱼巣
