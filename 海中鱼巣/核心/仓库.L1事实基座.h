#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>

#include "L1事实基座.数据.h"
#include "L1中性CRUD.数据.h"
#include "L1所有者范围CRUD.数据.h"

namespace 海中鱼巣 {

enum class L1事实基座核心持久恢复状态 : std::uint8_t {
    已建立空仓 = 1,
    已恢复 = 2,
    入口拒绝 = 3,
    存储占用 = 4,
    材料不完整 = 5,
    格式不支持 = 6,
    摘要不一致 = 7,
    编码或所有者冲突 = 8,
    资源失败 = 10,
    持久证据未知 = 11,
    内部不一致 = 12
};

struct L1事实基座核心持久恢复见证 final {
    std::uint32_t 格式版本 = 5;
    std::uint64_t 快照序号 = 0;
    std::array<std::uint8_t, 32> 载荷SHA256{};
};

struct L1事实基座核心持久恢复结果 final {
    L1事实基座核心持久恢复状态 状态 =
        L1事实基座核心持久恢复状态::内部不一致;
    std::optional<L1事实基座核心持久恢复见证> 恢复见证;
};

class L1事实基座仓库 final {
public:
    L1事实基座仓库();
    ~L1事实基座仓库();
    L1事实基座仓库(L1事实基座仓库&&) noexcept;
    L1事实基座仓库& operator=(L1事实基座仓库&&) noexcept;
    L1事实基座仓库(const L1事实基座仓库&) = delete;
    L1事实基座仓库& operator=(const L1事实基座仓库&) = delete;

    L1事实基座核心持久恢复结果 初始化持久恢复(
        const std::filesystem::path& 绝对受控根) noexcept;
    L1所有者范围建立结果 建立所有者范围(
        const L1所有者范围建立请求& 请求);
    L1所有者范围重入结果 验证所有者范围重入(
        const L1所有者范围重入请求& 请求) const;
    L1所有者范围退出结果 退出所有者范围(
        const L1所有者范围退出请求& 请求);

    L1中性写入结果 提交中性写集(const L1中性写集请求& 请求);
    L1中性写入首次结果读取结果 读取中性写入首次结果(
        const L1中性写入首次结果读取请求& 请求) const noexcept;
    L1所有者范围写入结果 提交所有者范围中性写集(
        L1结构所有者身份 所有者,
        const L1所有者范围写集请求& 请求);
    L1跨所有者原子事务结果 提交跨所有者原子事务(
        const L1跨所有者原子事务请求& 请求);
    L1三分区原子事务结果 提交三分区原子事务(
        const L1三分区原子事务请求& 请求) noexcept;
    L1有限N分区原子事务结果 提交有限N分区原子事务(
        const L1有限N分区原子事务请求& 请求) noexcept;
    L1所有者范围首次写入读取结果 读取所有者范围首次写入材料(
        L1结构所有者身份 所有者,
        const L1所有者范围首次写入读取请求& 请求) const;

    L1读取结果 读取当前节点(稳定编码 编码) const;
    L1读取结果 读取当前关系(稳定编码 编码) const;
    L1读取结果 读取当前值(稳定编码 编码) const;
    L1属性读取结果 读取当前属性(稳定编码 节点, 稳定编码 类型) const;
    L1中性源关系读取结果 读取中性当前源关系组(
        const L1中性源关系读取请求& 请求) const;
    L1中性目标关系读取结果 读取中性当前目标关系组(
        const L1中性目标关系读取请求& 请求) const;

    L1所有者范围当前读取结果 读取所有者范围当前节点(
        const L1所有者范围事实读取请求& 请求) const;
    L1所有者范围当前读取结果 读取所有者范围当前关系(
        const L1所有者范围事实读取请求& 请求) const;
    L1所有者范围当前读取结果 读取所有者范围当前值(
        const L1所有者范围事实读取请求& 请求) const;
    L1所有者范围当前事实读取结果 读取所有者范围当前事实(
        const L1所有者范围当前事实读取请求& 请求) const noexcept;
    L1所有者范围所属节点当前完整值组读取结果
    读取所有者范围所属节点当前完整值组(
        const L1所有者范围所属节点当前完整值组读取请求& 请求) const noexcept;
    L1所有者范围来源当前完整值组读取结果
    读取所有者范围来源当前完整值组(
        const L1所有者范围来源当前完整值组读取请求& 请求) const noexcept;
    L1所有者范围属性类型当前完整值组读取结果
    读取所有者范围属性类型当前完整值组(
        const L1所有者范围属性类型当前完整值组读取请求& 请求) const noexcept;
    L1所有者范围空域完整读取结果 读取所有者范围完整空域(
        const L1所有者范围空域完整读取请求& 请求) const noexcept;
    L1节点当前完整引用读取结果 读取节点全部当前引用(
        const L1节点当前完整引用读取请求& 请求) const noexcept;
    L1结构所有者当前读取结果 读取当前结构所有者(
        const L1结构所有者读取请求& 请求) const;
    L1所有者范围源关系组读取结果 读取所有者范围当前源关系组(
        const L1所有者范围源关系组读取请求& 请求) const;
    L1所有者范围目标关系组读取结果 读取所有者范围当前目标关系组(
        const L1所有者范围目标关系组读取请求& 请求) const;
    L1中性一致当前读取结果 尝试读取中性一致当前投影(
        const L1中性一致当前读取请求& 请求) const;
    L1所有者范围一致当前读取结果 尝试读取所有者范围一致当前投影(
        const L1所有者范围一致当前读取请求& 请求) const;
    L1所有者范围一致关系类型闭包读取结果
    尝试读取所有者范围一致关系类型闭包投影(
        const L1所有者范围一致关系类型闭包读取请求& 请求) const;

private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
};

} // namespace 海中鱼巣
