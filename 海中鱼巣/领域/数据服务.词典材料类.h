#pragma once

#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "词典.数据.h"
#include "信息内容.数据.h"

namespace 海中鱼巣 {

class 新_特征值类;
class 概念_存在类;
class 新材料结果引用参与接口;
class 词典材料类;

struct 新词典内部义项输入 final {
    std::uint64_t 外设消息标识 = 0;
    词典义项材料 内容;
};
struct 新词典内部材料建立请求 final {
    稳定编码 内部组织, 内部词典来源, 释义语言;
    新词典查询键 查询键;
    std::string 可信批次标识, 原包标识;
    新词典外设来源标识 外设来源;
    std::vector<新词典内部义项输入> 义项组;
};
struct 新词典内部义项 final {
    稳定编码 节点, 所属材料, 正文材料;
    std::size_t 序号 = 0;
    std::uint64_t 外设消息标识 = 0;
    词典义项材料 内容;
};
struct 新词典材料信息 final {
    稳定编码 节点, 内部组织, 内部词典来源, 释义语言;
    新词典查询键 查询键;
    std::string 可信批次标识, 原包标识;
    新词典外设来源标识 外设来源;
    std::vector<新词典内部义项> 义项组;
};
struct 新词典材料读取结果 final {
    新词典材料操作状态 状态 = 新词典材料操作状态::未找到;
    std::optional<新词典材料信息> 信息;
};
struct 新词典材料身份信息 final {
    稳定编码 节点, 内部组织, 正式材料范围;
};
struct 新词典材料身份读取结果 final {
    新词典材料操作状态 状态 = 新词典材料操作状态::未找到;
    std::optional<新词典材料身份信息> 信息;
};
struct 新词典正式语言归属 final { 稳定编码 自然语言根; };
struct 新词典隔离语言归属 final { 稳定编码 当前样例; };
struct 新词典材料场景身份 final {
    稳定编码 场景;
    std::variant<新词典正式语言归属, 新词典隔离语言归属> 归属;
};
struct 新词典材料场景读取结果 final {
    新词典材料操作状态 状态 = 新词典材料操作状态::未找到;
    std::optional<新词典材料场景身份> 身份;
};
class 新词典材料场景读取接口 {
public:
    virtual ~新词典材料场景读取接口() = default;
    virtual 新词典材料场景读取结果 读取材料所属场景(稳定编码) const noexcept = 0;
};
struct 新词典材料组织信息 final { 稳定编码 节点, 所属语言场景; };
struct 新词典材料组织结果 final {
    新词典材料操作状态 状态 = 新词典材料操作状态::未找到;
    std::optional<新词典材料组织信息> 信息;
};
struct 新词典材料提交结果 final {
    基础节点组追加状态 状态 = 基础节点组追加状态::入口拒绝;
    std::optional<基础片段发布回执> 共同回执;
};
struct 新词典材料建立结果 final {
    新词典材料操作状态 状态 = 新词典材料操作状态::输入拒绝;
    std::optional<稳定编码> 材料节点;
};

// 一次准备只提交一次；移动限创建线程，服务寿命覆盖准备。
class 新词典材料准备 final {
public:
    ~新词典材料准备();
    新词典材料准备(新词典材料准备&&) noexcept;
    新词典材料准备& operator=(新词典材料准备&&) noexcept;
    新词典材料准备(const 新词典材料准备&) = delete;
    新词典材料准备& operator=(const 新词典材料准备&) = delete;
private:
    struct 实现;
    explicit 新词典材料准备(std::unique_ptr<实现>) noexcept;
    std::unique_ptr<实现> 实现_;
    friend class 词典材料类;
};
struct 新词典材料准备结果 final {
    新词典材料操作状态 状态 = 新词典材料操作状态::输入拒绝;
    std::optional<新词典材料准备> 准备;
};

class 新词典材料退出准备 final {
public:
    ~新词典材料退出准备();
    新词典材料退出准备(新词典材料退出准备&&) noexcept;
    新词典材料退出准备& operator=(新词典材料退出准备&&) noexcept;
    新词典材料退出准备(const 新词典材料退出准备&) = delete;
    新词典材料退出准备& operator=(const 新词典材料退出准备&) = delete;
private:
    struct 实现;
    explicit 新词典材料退出准备(std::unique_ptr<实现>) noexcept;
    std::unique_ptr<实现> 实现_;
    friend class 词典材料类;
};
class 新隔离词典材料组准备 final {
public:
    ~新隔离词典材料组准备();
    新隔离词典材料组准备(新隔离词典材料组准备&&) noexcept;
    新隔离词典材料组准备& operator=(新隔离词典材料组准备&&) noexcept;
    新隔离词典材料组准备(const 新隔离词典材料组准备&) = delete;
    新隔离词典材料组准备& operator=(const 新隔离词典材料组准备&) = delete;
private:
    struct 实现;
    explicit 新隔离词典材料组准备(std::unique_ptr<实现>) noexcept;
    std::unique_ptr<实现> 实现_;
    friend class 词典材料类;
};
struct 新词典材料退出准备结果 final {
    新词典材料操作状态 状态 = 新词典材料操作状态::输入拒绝;
    std::unique_ptr<新词典材料退出准备> 准备;
};
struct 新隔离词典材料组准备结果 final {
    新词典材料操作状态 状态 = 新词典材料操作状态::输入拒绝;
    std::unique_ptr<新隔离词典材料组准备> 准备;
};
struct 新隔离词典材料组完成结果 final {
    新词典材料操作状态 状态 = 新词典材料操作状态::输入拒绝;
    std::vector<稳定编码> 材料节点组;
};
class 新隔离材料初值参与接口;

class 词典材料类 final : public 新信息对象身份读取接口 {
public:
    词典材料类(新_特征值类&, 概念_存在类&);
    ~词典材料类();
    词典材料类(const 词典材料类&) = delete;
    词典材料类& operator=(const 词典材料类&) = delete;
    新词典材料操作状态 初始化() noexcept;
    新词典材料组织结果 建立或取得材料组织(
        稳定编码, const 新词典材料场景读取接口&) noexcept;
    新词典材料组织结果 读取材料组织(稳定编码) const noexcept;
    新词典材料准备结果 准备内部材料(const 新词典内部材料建立请求&) noexcept;
    新词典材料提交结果 提交内部材料首次持有(
        新词典材料准备&, 新材料结果引用参与接口&) noexcept;
    新词典材料建立结果 完成内部材料发表(新词典材料准备&) noexcept;
    void 取消内部材料准备(新词典材料准备&) noexcept;
    新词典材料读取结果 读取材料(稳定编码) const noexcept;
    新词典材料读取结果 按外设来源读取材料(
        稳定编码, const 新词典外设来源标识&) const noexcept;
    新词典材料身份读取结果 读取引用身份(稳定编码) const noexcept;
    新信息对象身份读取结果 读取信息对象身份(稳定编码) const noexcept override;
    static const 新信息对象身份读取接口& 取得稳定身份读取接口() noexcept;
    新词典材料操作状态 释放材料闭包(稳定编码) noexcept;
    新词典材料退出准备结果 准备材料闭包退出(稳定编码) noexcept;
    新词典材料退出准备结果 准备隔离材料组织退出(
        稳定编码, const 新词典材料场景读取接口&) noexcept;
    基础封存追加片段 封存材料退出(新词典材料退出准备&) const;
    void 登记材料退出发表(新词典材料退出准备&, const 基础片段发布回执&) noexcept;
    void 完成材料退出(新词典材料退出准备&) noexcept;
    void 取消材料退出(新词典材料退出准备&) noexcept;
    新隔离词典材料组准备结果 准备隔离初值材料组(
        const std::vector<新词典内部材料建立请求>&,
        const 新词典材料场景读取接口&) noexcept;
    新词典材料提交结果 提交隔离初值材料首次持有(
        新隔离词典材料组准备&, 新隔离材料初值参与接口&) noexcept;
    新隔离词典材料组完成结果 完成隔离初值材料发表(新隔离词典材料组准备&) noexcept;
    void 取消隔离初值材料准备(新隔离词典材料组准备&) noexcept;
private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
};

} // namespace 海中鱼巣
