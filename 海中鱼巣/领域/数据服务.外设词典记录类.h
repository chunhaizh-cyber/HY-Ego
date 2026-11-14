#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "词典.数据.h"
#include "词条.数据.h"
#include "数据操作.执行结果参与.h"

namespace 海中鱼巣 {
class 新_特征值类;
class 外设词典记录类;

struct 新词典查询定位 final {
    稳定编码 外设场景;
    std::uint64_t 调用来源标识 = 0;
};
struct 新词典查询事实 final {
    新词典查询定位 定位;
    稳定编码 词典说话者;
    std::string 查询语种, 查询词面, 清单摘要, 原始包标识;
    新状态强时间 查询完成时间;
    新词典查阅结局 结局{};
    std::vector<词典义项材料> 义项组;
};
enum class 新词典接收处理阶段 : std::uint8_t {
    接收已发表 = 1, 动态已建立, 场景已挂接, 无需动态
};
struct 新词典接收处理信息 final {
    稳定编码 处理记录;
    新词典接收处理阶段 阶段{};
    std::optional<稳定编码> 前状态, 后状态, 动态;
    std::optional<新词条动态记录原因> 无需动态原因;
};
struct 新词典查询记录信息 final {
    稳定编码 查询记录;
    新词典查询定位 定位;
    稳定编码 词典说话者;
    std::string 查询语种, 查询词面, 清单摘要, 原始包标识;
    新状态强时间 查询完成时间;
    新词典查阅结局 结局{};
    std::vector<稳定编码> 有序义项消息;
    std::optional<新词典接收处理信息> 接收处理;
};
struct 新词典义项消息信息 final {
    稳定编码 消息, 查询记录, 外设场景, 词典说话者;
    std::size_t 义项顺序 = 0;
    新状态强时间 查询完成时间;
    词典义项材料 材料;
};
enum class 新词典记录操作状态 : std::uint8_t {
    已完成 = 1, 已复用, 未找到, 尚需续接, 仍被引用,
    输入拒绝, 未装配, 资源失败
};
struct 新词典查询读取结果 final {
    新词典记录操作状态 状态 = 新词典记录操作状态::未找到;
    std::optional<新词典查询记录信息> 信息;
};
struct 新词典义项读取结果 final {
    新词典记录操作状态 状态 = 新词典记录操作状态::未找到;
    std::optional<新词典义项消息信息> 信息;
};
struct 新词典查询发表结果 final {
    新词典记录操作状态 状态 = 新词典记录操作状态::输入拒绝;
    std::optional<新词典查询记录信息> 信息;
};
struct 新词典接收准备投影 final {
    稳定编码 接收特征, 新消息集合值;
    std::optional<稳定编码> 前状态;
    新状态强时间 本轮状态时间;
};
struct 新词典接收提交结果 final {
    基础节点组追加状态 状态 = 基础节点组追加状态::入口拒绝;
    std::vector<基础片段发布回执> 回执组;
    std::optional<稳定编码> 新状态;
};
class 新词典接收参与准备 {
public:
    virtual ~新词典接收参与准备() = default;
    virtual 新词典接收准备投影 读取接收投影() const = 0;
    virtual 新词典接收提交结果 提交接收(
        新消息待发表凭据&&, 新状态结果引用参与接口&) noexcept = 0;
    virtual void 取消未发表准备() noexcept = 0;
};
struct 新词典接收准备结果 final {
    新词典记录操作状态 状态 = 新词典记录操作状态::输入拒绝;
    std::unique_ptr<新词典接收参与准备> 准备;
};
class 新词典处理阶段参与接口 : public 新共同发表回执接收接口 {
public:
    virtual 新词典接收处理信息 读取本轮处理材料() const = 0;
    virtual 基础封存追加片段 构造动态已建立(稳定编码) const = 0;
    virtual 基础封存追加片段 构造处理收束(
        新词典接收处理阶段, std::optional<新词条动态记录原因>) const = 0;
};
class 新词典接收场景提供接口 {
public:
    virtual ~新词典接收场景提供接口() = default;
    virtual 新词典接收准备结果 准备接收(
        稳定编码, const std::vector<稳定编码>&) noexcept = 0;
    virtual 新词典接收提交结果 续接变化(
        稳定编码, 新词典处理阶段参与接口&) noexcept = 0;
};
class 新词典查询准备 final {
public:
    ~新词典查询准备();
    新词典查询准备(新词典查询准备&&) noexcept;
    新词典查询准备& operator=(新词典查询准备&&) noexcept;
    新词典查询准备(const 新词典查询准备&) = delete;
    新词典查询准备& operator=(const 新词典查询准备&) = delete;
private:
    struct 实现;
    explicit 新词典查询准备(std::unique_ptr<实现>) noexcept;
    std::unique_ptr<实现> 实现_;
    friend class 外设词典记录类;
};
struct 新词典查询准备结果 final {
    新词典记录操作状态 状态 = 新词典记录操作状态::输入拒绝;
    std::optional<新词典查询准备> 准备;
    std::optional<新词典查询记录信息> 既有记录;
};
enum class 新词典记录退出用途 : std::uint8_t { 独立记录退出 = 1, 隔离整样例退出 };
class 新词典隔离退出读取接口 {
public:
    virtual ~新词典隔离退出读取接口() = default;
    virtual 新词典记录操作状态 读取本样例外设场景(稳定编码) const noexcept = 0;
};
class 新词典记录退出准备 final {
public:
    ~新词典记录退出准备();
    新词典记录退出准备(新词典记录退出准备&&) noexcept;
    新词典记录退出准备& operator=(新词典记录退出准备&&) noexcept;
    新词典记录退出准备(const 新词典记录退出准备&) = delete;
    新词典记录退出准备& operator=(const 新词典记录退出准备&) = delete;
private:
    struct 实现;
    explicit 新词典记录退出准备(std::unique_ptr<实现>) noexcept;
    std::unique_ptr<实现> 实现_;
    friend class 外设词典记录类;
};
struct 新词典记录退出准备结果 final {
    新词典记录操作状态 状态 = 新词典记录操作状态::输入拒绝;
    std::optional<新词典记录退出准备> 准备;
};

class 外设词典记录类 final {
public:
    explicit 外设词典记录类(新_特征值类&) noexcept;
    新词典记录操作状态 初始化() noexcept;
    新词典查询准备结果 准备查询记录(const 新词典查询事实&,
        std::shared_ptr<新词典接收场景提供接口>) noexcept;
    新词典查询发表结果 提交查询接收(新词典查询准备&) noexcept;
    void 取消未发表查询(新词典查询准备&) noexcept;
    新词典查询读取结果 按调用来源读取查询记录(const 新词典查询定位&) const noexcept;
    新词典查询读取结果 读取查询记录(稳定编码) const noexcept;
    新词典义项读取结果 读取义项消息(稳定编码) const noexcept;
    特征消息成员读取结果 读取消息集合成员身份(稳定编码) const noexcept;
    static const 特征消息成员读取接口& 取得稳定消息身份读取接口() noexcept;
    新词典查询读取结果 续接查询变化(稳定编码, 新词典接收场景提供接口&) noexcept;
    新词典记录退出准备结果 准备查询记录退出(稳定编码, 新词典记录退出用途,
        const 新词典隔离退出读取接口* = nullptr) noexcept;
    基础封存追加片段 封存查询记录退出(新词典记录退出准备&) const;
    void 登记查询记录退出发表(新词典记录退出准备&, const 基础片段发布回执&) noexcept;
    void 完成查询记录退出(新词典记录退出准备&) noexcept;
    void 取消查询记录退出(新词典记录退出准备&) noexcept;
private:
    新_特征值类& 值服务_;
};
} // namespace 海中鱼巣
