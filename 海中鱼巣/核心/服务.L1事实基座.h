#pragma once

#include <memory>
#include <vector>

#include "仓库.L1事实基座.h"
#include "L1事实基座持久恢复.数据.h"

namespace 海中鱼巣 {

struct L1事实基座实例状态;
class L1所有者范围签发器;
class L1所有者范围写端口;
class L1事实基座运行包;
struct L1事实基座持久运行包建立结果;

class L1事实基座服务 final {
public:
    L1事实基座服务() = delete;
    L1事实基座服务(const L1事实基座服务&) = delete;
    L1事实基座服务& operator=(const L1事实基座服务&) = delete;

    L1中性一致当前读取结果 尝试读取中性一致当前投影(
        const L1中性一致当前读取请求& 请求) const;
    L1中性写入结果 提交中性写集(const L1中性写集请求& 请求);
    L1中性写入首次结果读取结果 读取中性写入首次结果(
        const L1中性写入首次结果读取请求& 请求) const noexcept;
    L1中性节点读取结果 读取中性当前节点(
        const L1中性事实读取请求& 请求) const;
    L1中性关系读取结果 读取中性当前关系(
        const L1中性事实读取请求& 请求) const;
    L1中性源关系读取结果 读取中性当前源关系组(
        const L1中性源关系读取请求& 请求) const;
    L1中性目标关系读取结果 读取中性当前目标关系组(
        const L1中性目标关系读取请求& 请求) const;
    L1中性值读取结果 读取中性当前值(
        const L1中性事实读取请求& 请求) const;
    L1中性属性读取结果 读取中性当前属性(
        const L1中性属性读取请求& 请求) const;

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
    L1所有者范围源关系组读取结果 读取所有者范围当前源关系组(
        const L1所有者范围源关系组读取请求& 请求) const;
    L1所有者范围目标关系组读取结果 读取所有者范围当前目标关系组(
        const L1所有者范围目标关系组读取请求& 请求) const;
    L1所有者范围一致当前读取结果 尝试读取所有者范围一致当前投影(
        const L1所有者范围一致当前读取请求& 请求) const;
    L1所有者范围一致关系类型闭包读取结果
    尝试读取所有者范围一致关系类型闭包投影(
        const L1所有者范围一致关系类型闭包读取请求& 请求) const;
    L1结构所有者当前读取结果 读取当前结构所有者(
        const L1结构所有者读取请求& 请求) const;
    L1所有者范围空域完整读取结果 读取所有者范围完整空域(
        const L1所有者范围空域完整读取请求& 请求) const noexcept;
    L1节点当前完整引用读取结果 读取节点全部当前引用(
        const L1节点当前完整引用读取请求& 请求) const noexcept;

private:
    explicit L1事实基座服务(L1事实基座仓库& 仓库) noexcept;
    friend class L1事实基座运行包;
    friend class L1所有者范围写端口;
    L1事实基座仓库& 仓库_;
};

class L1所有者范围写端口 final {
public:
    L1所有者范围写端口() = delete;
    L1所有者范围写端口(const L1所有者范围写端口&) = delete;
    L1所有者范围写端口& operator=(const L1所有者范围写端口&) = delete;
    L1所有者范围写端口(L1所有者范围写端口&& 来源) noexcept;
    L1所有者范围写端口& operator=(L1所有者范围写端口&& 来源) noexcept;
    ~L1所有者范围写端口();

    L1结构所有者身份 所有者身份() const noexcept;
    bool 有效() const noexcept;
    bool 绑定于(const L1事实基座服务& 服务) const noexcept;
    L1所有者范围写入结果 提交所有者范围中性写集(
        const L1所有者范围写集请求& 请求);
    L1跨所有者原子事务结果 提交跨所有者原子事务(
        const L1跨所有者原子事务请求& 请求,
        const L1所有者范围写端口& 另一参与者);
    L1三分区原子事务结果 提交三分区原子事务(
        const L1三分区原子事务请求& 请求,
        const L1所有者范围写端口& 第二参与者,
        const L1所有者范围写端口& 第三参与者) noexcept;
    L1有限N分区原子事务结果 提交有限N分区原子事务(
        const L1有限N分区原子事务请求& 请求,
        const std::vector<const L1所有者范围写端口*>& 其余参与者) noexcept;
    L1所有者范围首次写入读取结果 读取首次写入材料(
        const L1所有者范围首次写入读取请求& 请求) const;

private:
    L1所有者范围写端口(std::weak_ptr<L1事实基座实例状态> 状态,
        L1结构所有者身份 所有者) noexcept;
    void 释放租约() noexcept;
    friend class L1所有者范围签发器;
    std::weak_ptr<L1事实基座实例状态> 状态_;
    L1结构所有者身份 所有者_{};
    bool 持有租约_ = false;
};

struct L1所有者范围交付 final {
    L1所有者范围建立结果 建立结果;
    L1所有者范围重入结果 重入结果;
    std::unique_ptr<L1所有者范围写端口> 写端口;
};

class L1所有者范围签发器 final {
public:
    L1所有者范围签发器() = delete;
    L1所有者范围签发器(const L1所有者范围签发器&) = delete;
    L1所有者范围签发器& operator=(const L1所有者范围签发器&) = delete;
    L1所有者范围交付 建立所有者范围(const L1所有者范围建立请求& 请求);
    L1所有者范围交付 重新签发所有者范围写端口(
        const L1所有者范围重入请求& 请求);
    L1所有者范围退出结果 退出所有者范围(
        const L1所有者范围退出请求& 请求);

private:
    explicit L1所有者范围签发器(
        std::weak_ptr<L1事实基座实例状态> 状态) noexcept;
    friend class L1事实基座运行包;
    std::weak_ptr<L1事实基座实例状态> 状态_;
};

class L1事实基座运行包 final {
public:
    L1事实基座运行包() = delete;
    L1事实基座运行包(const L1事实基座运行包&) = delete;
    L1事实基座运行包& operator=(const L1事实基座运行包&) = delete;
    L1事实基座运行包(L1事实基座运行包&&) noexcept;
    L1事实基座运行包& operator=(L1事实基座运行包&&) noexcept;
    ~L1事实基座运行包();
    L1事实基座服务& 读取服务() noexcept;
    const L1事实基座服务& 读取服务() const noexcept;
    L1所有者范围签发器& 所有者范围签发器() noexcept;

private:
    explicit L1事实基座运行包(std::shared_ptr<L1事实基座实例状态> 状态);
    friend L1事实基座运行包 建立L1事实基座运行包();
    friend L1事实基座持久运行包建立结果
    建立L1事实基座持久运行包(
        const L1事实基座持久存储配置&) noexcept;
    std::shared_ptr<L1事实基座实例状态> 状态_;
    std::unique_ptr<L1事实基座服务> 服务_;
    std::unique_ptr<L1所有者范围签发器> 签发器_;
};

struct L1事实基座持久运行包建立结果 final {
    L1事实基座持久恢复结果 恢复;
    std::unique_ptr<L1事实基座运行包> 运行包;
};

L1事实基座运行包 建立L1事实基座运行包();
L1事实基座持久运行包建立结果 建立L1事实基座持久运行包(
    const L1事实基座持久存储配置& 配置) noexcept;

} // namespace 海中鱼巣
