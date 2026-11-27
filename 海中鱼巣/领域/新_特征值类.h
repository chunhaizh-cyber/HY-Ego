#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "../核心/基础数据集.h"

namespace 海中鱼巣 {

// 这里只区分材料的物理形状，不承载业务特征类型、单位、区间或概念。
enum class 材料物理类型 : std::int64_t {
    U64数组 = 1,
    I64数组 = 2,
    UTF8字符串 = 3,
    二维二值格 = 4,
    三维二值格 = 5
};

using 新特征值材料 = std::variant<
    std::vector<std::uint64_t>,
    std::vector<std::int64_t>,
    std::string>;

struct 新特征值信息 final {
    稳定编码 节点;
    材料物理类型 物理类型 = 材料物理类型::U64数组;
    新特征值材料 材料;
    friend bool operator==(const 新特征值信息&, const 新特征值信息&) = default;
};

enum class 新特征值快照解码状态 : std::uint8_t {
    已解码 = 1, 未初始化, 非本类节点, 结构不可解码
};
struct 新特征值快照解码结果 final {
    新特征值快照解码状态 状态 = 新特征值快照解码状态::未初始化;
    std::optional<新特征值信息> 信息;
    // 与已解码材料来自同一结构，供组合原子发表使用。
    基础只读预期 窄预期;
};

enum class 新特征值保护读取状态 : std::uint8_t {
    已读取 = 1, 输入拒绝, 未初始化, 节点不存在, 非本类节点, 结构不一致, 资源失败
};

struct 新特征值保护读取结果 final {
    新特征值保护读取状态 状态 = 新特征值保护读取状态::输入拒绝;
    std::optional<新特征值信息> 信息;
    基础只读预期 窄预期;
};

enum class 新服务权限值状态 : std::uint8_t {
    已读取 = 1, 保存失败, 节点不可读, 材料形状不相容, 结构不一致, 资源失败
};

struct 新服务权限值结果 final {
    新服务权限值状态 状态 = 新服务权限值状态::节点不可读;
    稳定编码 节点;
    std::optional<std::uint64_t> 数值;
    基础只读预期 窄预期;
};

// 由对应的特征值 owner 在初始化后取得；仅在该 owner 及其数据集存活期间使用。
struct 新特征值快照解码结构 final {
    稳定编码 节点类型字段, 特征值类型节点, 材料物理类型字段, 原始材料字段;
};

class 新_特征值类 final {
public:
    // 基础数据集必须比本对象存活更久。
    explicit 新_特征值类(基础数据集& 数据集);
    ~新_特征值类();

    新_特征值类(const 新_特征值类&) = delete;
    新_特征值类& operator=(const 新_特征值类&) = delete;
    新_特征值类(新_特征值类&&) = delete;
    新_特征值类& operator=(新_特征值类&&) = delete;

    // 建立本类共享的类型节点与字段节点；重复调用不会重复建立。
    bool 初始化() noexcept;

    // 二维按 y * 边长 + x、三维按 (z * 边长 + y) * 边长 + x 排列位，x 最快变化。
    // 二维二值格先按占用包围框等比例居中，再由原图按面积覆盖率生成各层；三维二值格保留逐级占用汇总。
    // 返回完整原始材料节点，失败返回空编码。
    稳定编码 保存或取得特征值(
        材料物理类型 物理类型, const 新特征值材料& 材料) noexcept;

    std::optional<新特征值信息> 获取特征值(稳定编码 节点) const noexcept;
    // 材料和窄预期来自同一次正式结构读取；发布方仍须复核预期并建立正式引用。
    新特征值保护读取结果 读取特征值保护(稳定编码 节点) const;
    // 只解码调用方提供的结构，不读取底层或判断快照当前性。
    新特征值快照解码结果 解码特征值快照(const 基础节点结构&) const;
    std::optional<新特征值快照解码结构> 读取快照解码结构() const;
    static 新特征值快照解码结果 解码特征值快照(
        const 基础节点结构&, const 新特征值快照解码结构&);
    std::vector<稳定编码> 查询特征值(
        材料物理类型 物理类型, const 新特征值材料& 材料) const noexcept;
    // 只保存准确值材料。派号、权限归属及正式引用须由领域owner另行联合发表。
    新服务权限值结果 保存或取得服务权限值(std::uint64_t 数值);
    新服务权限值结果 读取服务权限值(稳定编码 节点) const;

    bool 是特征值节点(稳定编码 节点) const noexcept;

private:
    class 实现;
    std::unique_ptr<实现> 实现_;
};

} // namespace 海中鱼巣
