#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "新_语言类.h"
#include "新_场景类.h"
#include "词条.数据.h"

namespace 海中鱼巣 {

extern 稳定编码 词组织根节点;
extern 稳定编码 严格UTF8字节精确规范化合同节点;

struct 新词条建立请求 final {
    稳定编码 语言节点;
    稳定编码 规范化合同节点;
    std::string 规范化词面;
    std::optional<稳定编码> 创建来源节点;
};

struct 新词条信息 final {
    稳定编码 节点;
    稳定编码 语言节点;
    稳定编码 规范化合同节点;
    std::string 规范化词面;
    std::optional<稳定编码> 创建来源节点;
    friend bool operator==(const 新词条信息&, const 新词条信息&) = default;
};

enum class 新词条建立状态 : std::uint8_t {
    已建立 = 1,
    已复用 = 2,
    输入不合法 = 3,
    依赖不存在 = 4,
    结构不一致 = 5,
    资源失败 = 6,
    依赖已变化 = 7
};

enum class 新词条查询状态 : std::uint8_t {
    已找到 = 1,
    未找到 = 2,
    输入不合法 = 3,
    结构不一致 = 4,
    资源失败 = 5,
    特征未建立 = 6
};

struct 新词条建立结果 final {
    新词条建立状态 状态 = 新词条建立状态::输入不合法;
    std::optional<稳定编码> 词条节点;
};

struct 新词条查询结果 final {
    新词条查询状态 状态 = 新词条查询状态::输入不合法;
    std::optional<新词条信息> 信息;
};

struct 新词条列表查询结果 final {
    新词条查询状态 状态 = 新词条查询状态::输入不合法;
    std::vector<稳定编码> 词条节点组;
};

struct 新词条特征信息 final {
    新词条信息 词条;
    稳定编码 所属自然语言场景;
    std::vector<新词条特征定位> 特征组;
};
struct 新词条特征读取结果 final {
    新词条查询状态 状态 = 新词条查询状态::未找到;
    std::optional<新词条特征信息> 信息;
};
struct 新词条特征定位结果 final {
    新词条查询状态 状态 = 新词条查询状态::未找到;
    std::optional<新词条特征定位> 特征;
};
struct 新词条挂接发布结果 final {
    基础节点组追加状态 状态 = 基础节点组追加状态::入口拒绝;
    std::vector<基础片段发布回执> 附加片段回执组;
    std::optional<稳定编码> 词条, 新状态, 变化动态;
    新词条动态记录原因 动态原因 = 新词条动态记录原因::无值变化;
};

enum class 新词条保护状态 : std::uint8_t {
    已读取 = 1, 未找到, 输入拒绝, 前提已变化, 资源失败
};
struct 新词条保护结果 final {
    新词条保护状态 状态 = 新词条保护状态::输入拒绝;
    std::optional<新词条信息> 信息;
    std::vector<基础联合节点快照> 完整保护;
};

// 词条只登记语言、规范化合同和规范化词面。它不分词、不规范化输入，
// 也不建立词义、概念或名称关系。
class 新_词类 final {
public:
    explicit 新_词类(新_语言类& 语言服务) noexcept;

    bool 初始化() noexcept;

    新词条建立结果 建立或取得词条(
        const 新词条建立请求& 请求) noexcept;
    新词条查询结果 获取词条(稳定编码 词条节点) const noexcept;
    新词条特征读取结果 读取词条特征信息(稳定编码 词条节点) const noexcept;
    新词条特征定位结果 按类型读取词条特征(
        稳定编码 词条节点, 稳定编码 特征类型) const noexcept;
    新词条查询结果 按规范化词面查询(
        稳定编码 语言节点,
        稳定编码 规范化合同节点,
        const std::string& 规范化词面) const noexcept;
    新词条保护结果 读取词条保护(稳定编码 词条) const noexcept;
    新词条保护结果 按规范化词面查询保护(
        稳定编码 语言, 稳定编码 规范化合同,
        const std::string& 规范化词面) const noexcept;
    新词条列表查询结果 查询语言词条(
        稳定编码 语言节点) const noexcept;
    bool 是词条节点(稳定编码 节点) const noexcept;
    const 新_语言类& 取得语言服务() const noexcept;

private:
    friend class 概念_名称关系类;
    friend class 新_自然语言世界树类;
    std::vector<稳定编码> 读取启动词条组() const;
    void 装配概念挂接(新_特征类&, 新_特征值类&, 新_状态类&, 新_动态类&,
        稳定编码 类型根, const 新词条场景参与绑定&);
    基础节点组追加状态 补齐词条概念挂接(稳定编码, const std::vector<稳定编码>&);
    bool 已装配概念挂接() const;
    void 准备名称集合材料(const std::vector<稳定编码>&);
    新词条挂接发布结果 发布名称挂接变化(稳定编码,
        const std::vector<稳定编码>&, const std::vector<基础封存追加片段>&);
    新词条挂接发布结果 发布词条初值已加锁(const 新词条建立请求&,
        std::optional<稳定编码>, const std::vector<稳定编码>&);
    std::optional<新词条特征信息> 读取特征信息已加锁(稳定编码) const;
    新_语言类& 语言服务_;
    新_特征类* 特征服务_ = nullptr;
    新_特征值类* 特征值服务_ = nullptr;
    新_状态类* 状态服务_ = nullptr;
    新_动态类* 动态服务_ = nullptr;
    稳定编码 挂接类型根_;
    std::optional<新词条场景参与绑定> 场景绑定_;
    std::optional<新词条状态构造绑定> 状态绑定_;
};

} // namespace 海中鱼巣
