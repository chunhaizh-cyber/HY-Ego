// 特征值模块.ixx
//
// 目标：
// 1) 特征值存放在“全局唯一仓库（特征值链）”里，所有引用者共享同一节点（共享语义）。
// 2) 为便于快速查找：按“特征值类型”建立索引（粗哈希 -> 候选列表），同时把节点挂到“类型根节点”下形成树形组织。
// 3) 不再使用 特征值基类/派生类，完全以 主信息定义模块 中的 特征值主信息类 为准。
//
module;

#include <afx.h>
#include <array>
#include <vector>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <cmath>
#include <algorithm>

export module 特征值模块;

import 模板模块;
import 主信息定义模块;
import 数据仓库模块;

export class 特征值类 {
public:
    using 节点类 = 特征值节点类;
    using 主信息类 = 特征值主信息类;

    struct 融合参数 {
        // 0~1，越大越“相信新观测”
        double 学习率 = 0.25;

        // 区间类型：是否扩张区间来覆盖新观测
        bool 区间扩张 = true;

        // Bits64：融合策略（0=不融合；1=按位或；2=按位与）
        int Bits融合策略 = 0;
    };

public:
    // ===========================
    // 获取 / 创建（查重）
    // ===========================

    节点类* 获取或创建(const 主信息类& mi) const;
    节点类* 获取或创建(主信息类&& mi) const;

    // 常用快捷函数（与 特征类.ixx 里的调用保持一致；单位参数当前不参与去重，可先传 nullptr）
    节点类* 获取或创建标量特征值(void* 单位, std::int64_t 值,
        枚举_比较模式 mode = 枚举_比较模式::有序标量) const;

    节点类* 获取或创建无符号标量特征值(void* 单位, std::uint64_t 值,
        枚举_比较模式 mode = 枚举_比较模式::有序标量) const;

    节点类* 获取或创建浮点特征值(double 值,
        枚举_比较模式 mode = 枚举_比较模式::有序标量) const;

    节点类* 获取或创建矢量特征值(const std::vector<std::int64_t>& 值,
        枚举_比较模式 mode = 枚举_比较模式::字典序向量) const;

    节点类* 获取或创建向量特征值_f32(const std::vector<float>& 值,
        枚举_比较模式 mode = 枚举_比较模式::字典序向量) const;

    // 文本：按 UTF-8 bytes 存入 Bytes（std::vector<uint8_t>）
    节点类* 获取或创建字符串特征值_UTF8(const std::string& utf8,
        枚举_比较模式 mode = 枚举_比较模式::字典序向量) const;

    // 引用：约定用 U64 保存地址（仅运行期有效；持久化请另存“主键/路径”）
    节点类* 获取或创建引用特征值(const void* ptr,
        枚举_比较模式 mode = 枚举_比较模式::有序标量) const;

public:
    // ===========================
    // 创建（不查重）
    // ===========================

    节点类* 创建_不查重(const 主信息类& mi) const;
    节点类* 创建_不查重(主信息类&& mi) const;

public:
    // ===========================
    // 修改 / 融合 / 删除
    // ===========================

    // 覆盖：把节点的主信息内容直接替换为新值（会自动更新索引与粗哈希）
    bool 覆盖(节点类* 节点, const 主信息类& 新值) const;

    // 融合：把“新观测”融合到“旧节点”里（共享语义）
    bool 融合(节点类* 旧节点, const 主信息类& 新观测,
        const 融合参数& p = 融合参数{}, bool 失败则覆盖 = false) const;

    bool 删除(节点类* 节点) const;

private:
    // ===========================
    // 索引与类型根
    // ===========================

    static constexpr std::size_t 类型数 = (std::size_t)枚举_特征值类型::Bytes + 1;

    struct 桶索引 {
        std::unordered_map<std::uint64_t, std::vector<节点类*>> buckets;
    };

    inline static bool 已初始化 = false;
    inline static std::array<节点类*, 类型数> 类型根{};   // 每个特征值类型一个“根节点”
    inline static std::array<桶索引, 类型数>  类型索引{}; // 每个类型一个粗哈希索引

private:
    static std::size_t idx(枚举_特征值类型 t) { return (std::size_t)t; }

    static bool 等值(const 主信息类& a, const 主信息类& b)
    {
        // 严格比较已同时比较“类型/比较模式/载体”
        return 严格比较_用于排序(a, b) == std::strong_ordering::equal;
    }

    static void 确保初始化_已加锁();

    static 节点类* 查找等值_已加锁(const 主信息类& key);

    static void 索引插入_已加锁(const 主信息类& mi, 节点类* node);
    static void 索引移除_已加锁(const 主信息类& mi, 节点类* node);

    static bool 尝试融合_已加锁(主信息类& inout, const 主信息类& obs, const 融合参数& p);

    static void 重新计算粗哈希(主信息类& mi)
    {
        mi.粗哈希 = 计算粗哈希(mi);
    }
};
