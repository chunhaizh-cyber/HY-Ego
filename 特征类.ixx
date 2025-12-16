// 特征模块.ixx（适配“特征值主信息类 + 全局特征值链 + 索引”新架构）
//
// 约定：
// - 特征值本体存在于 数据仓库模块::特征值链（全局唯一）
// - 特征节点主信息里只存 “特征值节点指针”
// - 特征值的创建/查找/融合/覆盖/删除 统一通过 特征值类 完成
//
module;

#include <afx.h>

#include <string>
#include <vector>
#include <variant>
#include <cstdint>

export module 特征模块;

import 数据仓库模块;
import 主信息定义模块;
import 特征值模块;

export class 特征类
{
public:
    using 特征节点类 = ::特征节点类;
    using 特征值节点类 = ::特征值节点类;
    using 基础信息节点类 = ::基础信息节点类;
    using 词性节点类 = ::词性节点类;

public:
    特征类() = default;

    // ===========================
    // 1) 特征节点：创建 / 查找
    // ===========================
    // 在指定载体的子链中查找指定类型的特征节点
    // 返回第一个匹配的特征节点（nullptr 表示未找到）
    static 特征节点类* 查找特征(基础信息节点类* 载体, 词性节点类* 特征类型)
    {
        if (!载体 || !特征类型) return nullptr;

        // 1) 优先：如果载体是存在节点，且有“当前状态缓存”
        if (auto* emi = dynamic_cast<存在节点主信息类*>(载体->主信息)) {
            if (auto* 当前特征 = emi->查找特征当前(特征类型)) {  // 假设你有此函数
                return 当前特征;
            }
        }

        // 2) 退回：遍历载体的所有子节点（特征节点）
        auto* 子 = 载体->子;
        if (!子) return nullptr;

        auto* 当前 = 子;
        do {
            if (当前->主信息) {
                if (auto* fmi = dynamic_cast<特征节点主信息类*>(当前->主信息)) {
                    if (fmi->类型 == 特征类型) {
                        return 当前;
                    }
                }
            }
            当前 = 当前->下;
            if (当前 == 子) break;  // 环形链表结束
        } while (true);

        return nullptr;
    }
    // 可选重载：返回所有匹配类型的特征节点（用于同类型多值场景，如历史轮廓）
    static std::vector<特征节点类*> 查找所有特征(基础信息节点类* 载体, 词性节点类* 特征类型)
    {
        std::vector<特征节点类*> 结果;

        if (!载体 || !特征类型) return 结果;

        // 优先当前状态（可选加入）
        if (auto* emi = dynamic_cast<存在节点主信息类*>(载体->主信息)) {
            if (auto* 当前特征 = emi->查找特征当前(特征类型)) {
                结果.push_back(当前特征);
            }
        }

        // 遍历全量子链
        auto* 子 = 载体->子;
        if (!子) return 结果;

        auto* 当前 = 子;
        do {
            if (当前->主信息) {
                if (auto* fmi = dynamic_cast<特征节点主信息类*>(当前->主信息)) {
                    if (fmi->类型 == 特征类型) {
                        结果.push_back(当前);
                    }
                }
            }
            当前 = 当前->下;
            if (当前 == 子) break;
        } while (true);

        return 结果;
    }

    // 在“所属节点”的子链上查找(按 类型)；不存在则创建并挂上去
    特征节点类* 找到或创建特征(基础信息节点类* 所属, 词性节点类* 类型) const;

    // 直接创建并挂到所属节点下（不查重）
    特征节点类* 创建并添加特征节点(基础信息节点类* 所属, 词性节点类* 类型,
        特征值节点类* 值节点 = nullptr, 词性节点类* 特征名 = nullptr) const;

    // 兼容接口：以“值主信息”创建特征
    特征节点类* 创建并添加特征节点_带值(基础信息节点类* 所属, 词性节点类* 类型,
        特征值主信息类 值主信息, 词性节点类* 特征名 = nullptr) const;

    // 快捷创建（常用）
    特征节点类* 创建并添加标量特征(基础信息节点类* 所属, 词性节点类* 类型,
        std::int64_t 值, bool 查重 = true) const;

    特征节点类* 创建并添加无符号标量特征(基础信息节点类* 所属, 词性节点类* 类型,
        std::uint64_t 值, bool 查重 = true) const;

    特征节点类* 创建并添加浮点特征(基础信息节点类* 所属, 词性节点类* 类型,
        double 值, bool 查重 = true) const;

    特征节点类* 创建并添加矢量特征(基础信息节点类* 所属, 词性节点类* 类型,
        const std::vector<std::int64_t>& 值, bool 查重 = true) const;

    特征节点类* 创建并添加文本特征_UTF8(基础信息节点类* 所属, 词性节点类* 类型,
        const std::string& utf8, bool 查重 = true) const;

    特征节点类* 创建并添加引用特征(基础信息节点类* 所属, 词性节点类* 类型,
        const void* 指针值, bool 查重 = true) const;

    // ===========================
    // 2) 特征值：覆盖 / 融合
    // ===========================

    bool 设置特征值_覆盖(特征节点类* 特征节点, 特征值节点类* 新值节点) const;

    // 融合：融合到“当前特征值节点”里（全局共享语义）
    bool 设置特征值_融合(特征节点类* 特征节点, 特征值节点类* 新值节点,
        const 特征值类::融合参数& p = {}, bool 失败则覆盖 = true) const;

    // ===========================
    // 3) 读取（从特征节点拿回值）
    // ===========================

    static bool 读取标量特征值(const 特征节点类* 特征节点, std::int64_t& out);
    static bool 读取无符号标量特征值(const 特征节点类* 特征节点, std::uint64_t& out);
    static bool 读取浮点特征值(const 特征节点类* 特征节点, double& out);
    static bool 读取矢量特征值(const 特征节点类* 特征节点, std::vector<std::int64_t>& out);
    static bool 读取文本特征值_UTF8(const 特征节点类* 特征节点, std::string& out);
    static bool 读取引用特征值(const 特征节点类* 特征节点, const void*& out);

private:
    static 特征节点主信息类* 取特征主信息(特征节点类* 节点);
    static const 特征节点主信息类* 取特征主信息(const 特征节点类* 节点);

    static 特征值主信息类* 取值主信息(特征节点类* 节点);
    static const 特征值主信息类* 取值主信息(const 特征节点类* 节点);

    // 索引：把特征节点加入 所属主信息.特征索引（如果存在）
    static void 追加到所属节点特征索引(基础信息节点类* 所属, 特征节点类* 特征节点);
};



// =============================
// 内联实现（模块内）
// =============================

inline 特征节点主信息类* 特征类::取特征主信息(特征节点类* 节点)
{
    return 节点 ? dynamic_cast<特征节点主信息类*>(节点->主信息) : nullptr;
}

inline const 特征节点主信息类* 特征类::取特征主信息(const 特征节点类* 节点)
{
    return 节点 ? dynamic_cast<const 特征节点主信息类*>(节点->主信息) : nullptr;
}

inline 特征值主信息类* 特征类::取值主信息(特征节点类* 节点)
{
    auto* fmi = 取特征主信息(节点);
    if (!fmi || !fmi->值) return nullptr;
    return fmi->值->主信息;
}

inline const 特征值主信息类* 特征类::取值主信息(const 特征节点类* 节点)
{
    auto* fmi = 取特征主信息(节点);
    if (!fmi || !fmi->值) return nullptr;
    return fmi->值->主信息;
}

inline void 特征类::追加到所属节点特征索引(基础信息节点类* 所属, 特征节点类* 特征节点)
{
    if (!所属 || !特征节点) return;
    auto* mi = dynamic_cast<存在节点主信息类*>(所属->主信息);
    if (!mi) return;
    mi->特征索引.push_back(特征节点);
}

inline 特征类::特征节点类* 特征类::找到或创建特征(基础信息节点类* 所属, 词性节点类* 类型) const
{
    if (!所属 || !类型) return nullptr;

    // 遍历所属子链找同类型特征
    auto* root = 所属;
    auto* cur = root->子;
    if (cur) {
        do {
            auto* fmi = dynamic_cast<特征节点主信息类*>(cur->主信息);
            if (fmi && fmi->类型 == 类型) {
                return cur;
            }
            cur = cur->下;
        } while (cur && cur != root->子);
    }

    // 没找到就创建
    return 创建并添加特征节点(所属, 类型, nullptr, /*特征名=*/nullptr);
}

inline 特征类::特征节点类* 特征类::创建并添加特征节点(基础信息节点类* 所属, 词性节点类* 类型,
    特征值节点类* 值节点, 词性节点类* 特征名) const
{
    if (!所属 || !类型) return nullptr;

    auto* mi = new 特征节点主信息类();
    mi->类型 = 类型;
    mi->名称 = 特征名;
    mi->值 = 值节点;

    // 挂到所属节点子链尾部
    auto 链锁 = 世界链.获取锁();
    auto* node = 世界链.添加子节点_已加锁(所属, (基础信息基类*)mi);

    追加到所属节点特征索引(所属, node);
    return node;
}

inline 特征类::特征节点类* 特征类::创建并添加特征节点_带值(基础信息节点类* 所属, 词性节点类* 类型,
    特征值主信息类 值主信息, 词性节点类* 特征名) const
{
    特征值类 值仓库;
    auto* 值节点 = 值仓库.获取或创建(std::move(值主信息));
    return 创建并添加特征节点(所属, 类型, 值节点, 特征名);
}

inline 特征类::特征节点类* 特征类::创建并添加标量特征(基础信息节点类* 所属, 词性节点类* 类型,
    std::int64_t 值, bool 查重) const
{
    特征值类 值仓库;
    auto* v = 查重 ? 值仓库.获取或创建标量特征值(/*单位=*/nullptr, 值)
        : 值仓库.创建_不查重(特征值主信息类{ .类型 = 枚举_特征值类型::I64, .比较模式 = 枚举_比较模式::有序标量, .值 = 值 });
    return 创建并添加特征节点(所属, 类型, v, nullptr);
}

inline 特征类::特征节点类* 特征类::创建并添加无符号标量特征(基础信息节点类* 所属, 词性节点类* 类型,
    std::uint64_t 值, bool 查重) const
{
    特征值类 值仓库;
    auto* v = 查重 ? 值仓库.获取或创建无符号标量特征值(/*单位=*/nullptr, 值)
        : 值仓库.创建_不查重(特征值主信息类{ .类型 = 枚举_特征值类型::U64, .比较模式 = 枚举_比较模式::有序标量, .值 = 值 });
    return 创建并添加特征节点(所属, 类型, v, nullptr);
}

inline 特征类::特征节点类* 特征类::创建并添加浮点特征(基础信息节点类* 所属, 词性节点类* 类型,
    double 值, bool 查重) const
{
    特征值类 值仓库;
    auto* v = 查重 ? 值仓库.获取或创建浮点特征值(值)
        : 值仓库.创建_不查重(特征值主信息类{ .类型 = 枚举_特征值类型::F64, .比较模式 = 枚举_比较模式::有序标量, .值 = 值 });
    return 创建并添加特征节点(所属, 类型, v, nullptr);
}

inline 特征类::特征节点类* 特征类::创建并添加矢量特征(基础信息节点类* 所属, 词性节点类* 类型,
    const std::vector<std::int64_t>& 值, bool 查重) const
{
    特征值类 值仓库;
    auto* v = 查重 ? 值仓库.获取或创建矢量特征值(值)
        : 值仓库.创建_不查重(特征值主信息类{ .类型 = 枚举_特征值类型::VecI64, .比较模式 = 枚举_比较模式::字典序向量, .值 = 值 });
    return 创建并添加特征节点(所属, 类型, v, nullptr);
}

inline 特征类::特征节点类* 特征类::创建并添加文本特征_UTF8(基础信息节点类* 所属, 词性节点类* 类型,
    const std::string& utf8, bool 查重) const
{
    特征值类 值仓库;
    auto* v = 查重 ? 值仓库.获取或创建字符串特征值_UTF8(utf8)
        : 值仓库.创建_不查重(特征值主信息类{ .类型 = 枚举_特征值类型::Bytes, .比较模式 = 枚举_比较模式::字典序向量, .值 = std::vector<std::uint8_t>(utf8.begin(), utf8.end()) });
    return 创建并添加特征节点(所属, 类型, v, nullptr);
}

inline 特征类::特征节点类* 特征类::创建并添加引用特征(基础信息节点类* 所属, 词性节点类* 类型,
    const void* 指针值, bool 查重) const
{
    特征值类 值仓库;
    auto* v = 查重 ? 值仓库.获取或创建引用特征值(指针值)
        : 值仓库.创建_不查重(特征值主信息类{ .类型 = 枚举_特征值类型::U64, .比较模式 = 枚举_比较模式::有序标量, .值 = (std::uint64_t)(std::uintptr_t)指针值 });
    return 创建并添加特征节点(所属, 类型, v, nullptr);
}

inline bool 特征类::设置特征值_覆盖(特征节点类* 特征节点, 特征值节点类* 新值节点) const
{
    auto* fmi = 取特征主信息(特征节点);
    if (!fmi) return false;
    fmi->值 = 新值节点;
    return true;
}

inline bool 特征类::设置特征值_融合(特征节点类* 特征节点, 特征值节点类* 新值节点,
    const 特征值类::融合参数& p, bool 失败则覆盖) const
{
    auto* fmi = 取特征主信息(特征节点);
    if (!fmi) return false;
    if (!新值节点 || !新值节点->主信息) return false;

    // 当前没有值：直接挂
    if (!fmi->值) {
        fmi->值 = 新值节点;
        return true;
    }

    if (!fmi->值->主信息) {
        if (失败则覆盖) {
            fmi->值 = 新值节点;
            return true;
        }
        return false;
    }

    // 融合到“当前值节点”里（共享语义）
    特征值类 值仓库;
    const bool ok = 值仓库.融合(fmi->值, *新值节点->主信息, p, /*失败则覆盖=*/false);
    if (!ok && 失败则覆盖) {
        fmi->值 = 新值节点;
        return true;
    }
    return ok;
}

// ===========================
// 读取
// ===========================

inline bool 特征类::读取标量特征值(const 特征节点类* 特征节点, std::int64_t& out)
{
    auto* vmi = 取值主信息(特征节点);
    if (!vmi) return false;
    if (vmi->类型 != 枚举_特征值类型::I64) return false;
    if (auto* p = std::get_if<std::int64_t>(&vmi->值)) { out = *p; return true; }
    return false;
}

inline bool 特征类::读取无符号标量特征值(const 特征节点类* 特征节点, std::uint64_t& out)
{
    auto* vmi = 取值主信息(特征节点);
    if (!vmi) return false;
    if (vmi->类型 != 枚举_特征值类型::U64) return false;
    if (auto* p = std::get_if<std::uint64_t>(&vmi->值)) { out = *p; return true; }
    return false;
}

inline bool 特征类::读取浮点特征值(const 特征节点类* 特征节点, double& out)
{
    auto* vmi = 取值主信息(特征节点);
    if (!vmi) return false;
    if (vmi->类型 != 枚举_特征值类型::F64) return false;
    if (auto* p = std::get_if<double>(&vmi->值)) { out = *p; return true; }
    return false;
}

inline bool 特征类::读取矢量特征值(const 特征节点类* 特征节点, std::vector<std::int64_t>& out)
{
    auto* vmi = 取值主信息(特征节点);
    if (!vmi) return false;
    if (vmi->类型 != 枚举_特征值类型::VecI64) return false;
    if (auto* p = std::get_if<std::vector<std::int64_t>>(&vmi->值)) { out = *p; return true; }
    return false;
}

inline bool 特征类::读取文本特征值_UTF8(const 特征节点类* 特征节点, std::string& out)
{
    auto* vmi = 取值主信息(特征节点);
    if (!vmi) return false;
    if (vmi->类型 != 枚举_特征值类型::Bytes) return false;
    auto* p = std::get_if<std::vector<std::uint8_t>>(&vmi->值);
    if (!p) return false;
    out.assign((const char*)p->data(), (const char*)p->data() + p->size());
    return true;
}

inline bool 特征类::读取引用特征值(const 特征节点类* 特征节点, const void*& out)
{
    auto* vmi = 取值主信息(特征节点);
    if (!vmi) return false;

    // 约定：引用用 U64 存地址
    if (vmi->类型 != 枚举_特征值类型::U64) return false;
    auto* p = std::get_if<std::uint64_t>(&vmi->值);
    if (!p) return false;
    out = (const void*)(std::uintptr_t)(*p);
    return true;
}
