#pragma once

#include "基础数据集.快照格式.h"

#include <atomic>
#include <map>
#include <string_view>

namespace 海中鱼巣 {

class 基础持久视图 final {
public:
    const 基础数据集& 结构() const noexcept { return *结构_; }
    const std::array<std::uint8_t, 16>& 数据集身份() const noexcept { return 身份_; }
    std::uint64_t 下一个编码() const noexcept { return 下一个编码_; }
    std::vector<基础字段关系> 查询自键标记(std::string_view 值) const;

private:
    friend class 基础持久运行包;
    基础持久视图() = default;

    std::unique_ptr<const 基础数据集> 结构_;
    // 键和值均借用本视图拥有的不可变字段记录。
    std::multimap<std::string_view, const 基础字段关系*, std::less<>> 自键标记_;
    std::array<std::uint8_t, 16> 身份_{};
    std::uint64_t 下一个编码_ = 0;
};

struct 基础自键定义 final {
    std::string_view 标记;
    稳定编码 节点;
};

struct 基础结构自键读回 final {
    基础结构操作状态 状态 = 基础结构操作状态::入口拒绝;
    std::vector<稳定编码> 节点组;
};

// 结构所有者提供本域的固定标记与物理节点。底座只校验和发表直接字段。
基础结构操作状态 发表基础结构自键(
    基础数据集& 数据, std::span<const 基础自键定义> 定义组);
基础结构自键读回 读取基础结构自键(
    const 基础持久视图& 视图, std::span<const std::string_view> 标记组);

class 基础持久首份候选 final {
public:
    const 基础持久视图& 读取() const noexcept { return *视图_; }

private:
    friend class 基础持久运行包;
    基础持久首份候选() = default;

    std::shared_ptr<const int> 会话身份_;
    std::shared_ptr<const 基础持久视图> 视图_;
    std::vector<std::uint8_t> 字节_;
};

struct 基础持久追加结果 final {
    基础持久状态 持久状态 = 基础持久状态::入口拒绝;
    std::optional<基础节点组追加状态> 结构状态;
    std::vector<基础片段发布回执> 回执组;
};

// 持久会话持有唯一当前事实与提交资格。
class 基础持久运行包 final {
    friend class 基础数据集;
public:
    using 视图 = std::shared_ptr<const 基础持久视图>;
    static std::pair<基础持久状态, std::unique_ptr<基础持久运行包>> 打开(
        const std::filesystem::path& 目录);
    static 基础持久状态 绑定恢复权威(基础数据集& 入口, const std::shared_ptr<基础持久运行包>& 会话);
    static std::shared_ptr<基础持久运行包> 读取已绑定会话(const 基础数据集& 入口);
    视图 读取() const noexcept { return 当前_.load(std::memory_order_acquire); }
    基础持久状态 提交(const 视图& 预期, const 基础快照映像& 候选);
    // 仅用于实际普通启动的全新、无运行期消费者引导候选。
    static bool 可开始首份(const 基础数据集& 源);
    std::pair<基础持久状态, std::unique_ptr<const 基础持久首份候选>> 准备首份(
        const 基础数据集& 源) const;
    基础持久状态 提交首份(const 基础持久首份候选& 候选);
    基础片段封存结果 封存追加(const 基础节点组追加请求& 请求) const;
    // 调用方在交接封存片段后释放业务仓库锁。
    基础持久追加结果 提交追加(const 视图& 预期, const std::vector<基础封存追加片段>& 片段组);

private:
    基础持久运行包(std::unique_ptr<基础持久存储> 存储, 视图 初始);
    static std::pair<基础持久状态, 视图> 准备(std::span<const std::uint8_t> 字节);
    static void 建立自键索引(基础持久视图& 视图);
    static 基础持久状态 校验变更(const 基础持久视图& 前, const 基础持久视图& 后);

    基础持久状态 发表已准备(视图 后, std::span<const std::uint8_t> 字节);

    const std::shared_ptr<const int> 封存身份_;
    std::unique_ptr<基础持久存储> 存储_;
    std::atomic<视图> 当前_;
    std::atomic_flag 提交中_{};
    bool 发布未决_ = false; // 仅取得提交资格后使用
};

} // namespace 海中鱼巣
