module;
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <memory>

export module 特征值模块;

// 说明：
// - 本模块仅服务于 特征模块：负责 VecIU64（大块证据）的全局去重与可选分层索引。
// - I64 与 指针值 不入特征值链，直接内联在 特征节点主信息类::当前快照 中。
// - “查找”只用于唯一性：仅排除绝对相同（VecIU64 内容完全一致）。
//
// 分层索引：
// - 仅用于组织（例如：轮廓2D/轮廓3D 的金字塔路径），不参与唯一性判定。
// - 读档后可按需重建（当前阶段不强制）。

import 模板模块;
import 基础数据类型模块;
import 主信息定义模块;
import 数据仓库模块;


export using 特征值节点类 = 数据仓库模块::特征值链类型::节点类;

export enum class 枚举_轮廓维度 : std::uint8_t {
    未定义 = 0,
    二维 = 1,
    三维 = 2,
};

export class 特征值类 {
public:
    // 初始化：扫描特征值链，重建“哈希->候选”缓存（唯一性索引用）
    void 初始化() const;

    // ===== 唯一性（绝对相同） =====
    // 兼容：以 主信息 形式传入（主信息目前仅包含 VecIU64）
    特征值节点类* 查找(const 特征值主信息类& mi) const;
    特征值节点类* 获取或创建(特征值主信息类&& mi, 枚举_轮廓维度 dim = 枚举_轮廓维度::未定义) const;

    特征值节点类* 查找(const VecIU64& v) const;
    特征值节点类* 获取或创建(VecIU64 v, 枚举_轮廓维度 dim = 枚举_轮廓维度::未定义) const;

    // 便捷：直接返回句柄（可写入 特征快照值）
    VecU句柄 获取或创建句柄(VecIU64 v, 枚举_轮廓维度 dim = 枚举_轮廓维度::未定义) const;
    VecU句柄 获取或创建VecU(VecIU64 v, 枚举_轮廓维度 dim = 枚举_轮廓维度::未定义) const {
        return 获取或创建句柄(static_cast<VecIU64&&>(v), dim);
    }
    const VecIU64* 取VecU只读指针(VecU句柄 h) const noexcept {
        if (!h.有效()) return nullptr;
        auto* mi = reinterpret_cast<const 特征值主信息类*>(h.主信息指针);
        return mi ? &mi->值 : nullptr;
    }

    // ===== 已加锁版本（调用者需先通过 锁调度器守卫 持有 特征值链锁）=====
    // - 只读查询路径：至少持有读锁
    // - 可能改写缓存/链结构的路径：持有写锁
    特征值节点类* 查找_已加锁(const 特征值主信息类& mi) const;
    特征值节点类* 获取或创建_已加锁(特征值主信息类&& mi, 枚举_轮廓维度 dim) const;
    特征值节点类* 查找_已加锁(const VecIU64& v) const;
    特征值节点类* 获取或创建_已加锁(VecIU64&& v, 枚举_轮廓维度 dim) const;

private:
    // ---------- 唯一性缓存 ----------
    mutable bool 缓存已构建_ = false;
    mutable std::unordered_map<std::uint64_t, std::vector<特征值节点类*>> 哈希到候选_{};

    // ---------- 分层索引（组织用） ----------
    struct 分层索引节点 {
        std::unordered_map<I64, 分层索引节点*> 子;
        std::vector<特征值节点类*> 叶子;
    };
    mutable std::vector<std::unique_ptr<分层索引节点>> 索引池_{};
    mutable 分层索引节点* 根2D_ = nullptr;
    mutable 分层索引节点* 根3D_ = nullptr;

private:
    void 私有_确保缓存_已加锁() const;
    void 私有_重建缓存_已加锁() const;

    // FNV-1a 64
    static std::uint64_t FNV1a64_Bytes(const void* data, std::size_t len) noexcept;
    static std::uint64_t FNV1a64_混入64位值(std::uint64_t h, std::uint64_t x) noexcept;

    static std::uint64_t 哈希VecIU64(const VecIU64& v) noexcept;

    // 2D/3D 金字塔：输入为 VecIU64（长度将补齐到 64*N*N 或 64*N*N*N）
    // 输出每层 flattened 数组（层0 为补齐后的 base），返回 tokens（从粗到细）
    static std::vector<I64> 生成二维轮廓金字塔(VecIU64 base, std::vector<VecIU64>& 输出层);
    static std::vector<I64> 生成三维轮廓金字塔(VecIU64 base, std::vector<VecIU64>& 输出层);

    // 分层索引：取或创建路径节点
    分层索引节点* 分层_取或创建路径_已加锁(分层索引节点*& root, const std::vector<I64>& tokens) const;

    // 插入分层（仅组织，不参与唯一性）
    void 分层_登记叶子_已加锁(枚举_轮廓维度 dim, const std::vector<I64>& tokens, 特征值节点类* leaf) const;

    // 添加：创建节点入链 + 入唯一性缓存 +（可选）分层登记
    特征值节点类* 添加_分层_已加锁(特征值主信息类* mi, 枚举_轮廓维度 dim) const;
};

export inline 特征值类 特征值仓库{};


