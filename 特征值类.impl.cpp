// 特征值模块.impl.cpp

module;

#include <afx.h>
#include <cstring>
#include <cmath>
#include <algorithm>

module 特征值模块;

import 模板模块;
import 主信息定义模块;
import 数据仓库模块;
import 基础数据类型模块;

static inline std::uint8_t clamp_u8(long long v)
{
    if (v < 0) return 0;
    if (v > 255) return 255;
    return (std::uint8_t)v;
}

void 特征值类::确保初始化_已加锁()
{
    if (已初始化) return;

    // 为每个“特征值类型”建立一个根节点，挂在 特征值链 的根下。
    // 根节点本身不参与去重索引（主信息类型统一设为 未定义）。
    for (std::size_t i = 0; i < 类型数; ++i) {
        auto* mi = new 主信息类{};
        mi->类型 = 枚举_特征值类型::未定义;
        mi->比较模式 = 枚举_比较模式::有序标量;
        mi->值 = std::monostate{};
        mi->命中次数 = 0;
        mi->可信度 = 0.0;
        mi->粗哈希 = 0xF00D0000ull ^ (std::uint64_t)i; // 仅用于调试区分
        类型根[i] = 特征值链.添加子节点_已加锁(特征值链.根指针, mi);
    }

    已初始化 = true;
}

特征值类::节点类* 特征值类::查找等值_已加锁(const 主信息类& key)
{
    if (key.类型 == 枚举_特征值类型::未定义) return nullptr;
    const auto i = idx(key.类型);

    auto it = 类型索引[i].buckets.find(key.粗哈希);
    if (it == 类型索引[i].buckets.end()) return nullptr;

    for (auto* n : it->second) {
        if (!n || !n->主信息) continue;
        if (等值(*n->主信息, key)) return n;
    }
    return nullptr;
}

void 特征值类::索引插入_已加锁(const 主信息类& mi, 节点类* node)
{
    if (!node || !node->主信息) return;
    if (mi.类型 == 枚举_特征值类型::未定义) return;
    const auto i = idx(mi.类型);
    类型索引[i].buckets[mi.粗哈希].push_back(node);
}

void 特征值类::索引移除_已加锁(const 主信息类& mi, 节点类* node)
{
    if (!node) return;
    if (mi.类型 == 枚举_特征值类型::未定义) return;

    const auto i = idx(mi.类型);
    auto it = 类型索引[i].buckets.find(mi.粗哈希);
    if (it == 类型索引[i].buckets.end()) return;

    auto& vec = it->second;
    vec.erase(std::remove(vec.begin(), vec.end(), node), vec.end());
    if (vec.empty()) {
        类型索引[i].buckets.erase(it);
    }
}

特征值类::节点类* 特征值类::获取或创建(const 主信息类& mi) const
{
    主信息类 key = mi;
    if (key.粗哈希 == 0) 重新计算粗哈希(key);

    auto 锁 = 特征值链.获取锁();
    确保初始化_已加锁();

    if (auto* hit = 查找等值_已加锁(key)) {
        if (hit->主信息) {
            hit->主信息->命中次数 += 1;
            hit->主信息->可信度 = (((1.0) < (hit->主信息->可信度 + 0.01)) ? (1.0) : (hit->主信息->可信度 + 0.01));
        }
        return hit;
    }

    auto* stored = new 主信息类(key);
    stored->命中次数 = std::max<std::uint32_t>(1, stored->命中次数);
    if (stored->可信度 <= 0.0) stored->可信度 = 1.0;

    auto* parent = 类型根[idx(key.类型)];
    auto* node = 特征值链.添加子节点_已加锁(parent, stored);
    索引插入_已加锁(*stored, node);
    return node;
}

特征值类::节点类* 特征值类::获取或创建(主信息类&& mi) const
{
    主信息类 key = std::move(mi);
    if (key.粗哈希 == 0) 重新计算粗哈希(key);

    auto 锁 = 特征值链.获取锁();
    确保初始化_已加锁();

    if (auto* hit = 查找等值_已加锁(key)) {
        if (hit->主信息) {
            hit->主信息->命中次数 += 1;
            hit->主信息->可信度 = (((1.0) < (hit->主信息->可信度 + 0.01)) ? (1.0) : (hit->主信息->可信度 + 0.01));
        }
        return hit;
    }

    auto* stored = new 主信息类(std::move(key));
    stored->命中次数 = std::max<std::uint32_t>(1, stored->命中次数);
    if (stored->可信度 <= 0.0) stored->可信度 = 1.0;

    auto* parent = 类型根[idx(stored->类型)];
    auto* node = 特征值链.添加子节点_已加锁(parent, stored);
    索引插入_已加锁(*stored, node);
    return node;
}

特征值类::节点类* 特征值类::获取或创建标量特征值(void* /*单位*/, std::int64_t 值, 枚举_比较模式 mode) const
{
    主信息类 mi;
    mi.类型 = 枚举_特征值类型::I64;
    mi.比较模式 = mode;
    mi.值 = 值;
    重新计算粗哈希(mi);
    return 获取或创建(std::move(mi));
}

特征值类::节点类* 特征值类::获取或创建无符号标量特征值(void* /*单位*/, std::uint64_t 值, 枚举_比较模式 mode) const
{
    主信息类 mi;
    mi.类型 = 枚举_特征值类型::U64;
    mi.比较模式 = mode;
    mi.值 = 值;
    重新计算粗哈希(mi);
    return 获取或创建(std::move(mi));
}

特征值类::节点类* 特征值类::获取或创建浮点特征值(double 值, 枚举_比较模式 mode) const
{
    主信息类 mi;
    mi.类型 = 枚举_特征值类型::F64;
    mi.比较模式 = mode;
    mi.值 = 值;
    重新计算粗哈希(mi);
    return 获取或创建(std::move(mi));
}

特征值类::节点类* 特征值类::获取或创建矢量特征值(const std::vector<std::int64_t>& 值, 枚举_比较模式 mode) const
{
    主信息类 mi;
    mi.类型 = 枚举_特征值类型::VecI64;
    mi.比较模式 = mode;
    mi.值 = 值;
    重新计算粗哈希(mi);
    return 获取或创建(std::move(mi));
}

特征值类::节点类* 特征值类::获取或创建向量特征值_f32(const std::vector<float>& 值, 枚举_比较模式 mode) const
{
    主信息类 mi;
    mi.类型 = 枚举_特征值类型::VecF32;
    mi.比较模式 = mode;
    mi.值 = 值;
    重新计算粗哈希(mi);
    return 获取或创建(std::move(mi));
}

特征值类::节点类* 特征值类::获取或创建字符串特征值_UTF8(const std::string& utf8, 枚举_比较模式 mode) const
{
    std::vector<std::uint8_t> bytes;
    bytes.assign((const std::uint8_t*)utf8.data(), (const std::uint8_t*)utf8.data() + utf8.size());

    主信息类 mi;
    mi.类型 = 枚举_特征值类型::Bytes;
    mi.比较模式 = mode;
    mi.值 = std::move(bytes);
    重新计算粗哈希(mi);
    return 获取或创建(std::move(mi));
}

特征值类::节点类* 特征值类::获取或创建引用特征值(const void* ptr, 枚举_比较模式 mode) const
{
    主信息类 mi;
    mi.类型 = 枚举_特征值类型::U64;
    mi.比较模式 = mode;
    mi.值 = (std::uint64_t)(std::uintptr_t)ptr;
    重新计算粗哈希(mi);
    return 获取或创建(std::move(mi));
}

特征值类::节点类* 特征值类::创建_不查重(const 主信息类& mi) const
{
    主信息类 key = mi;
    if (key.粗哈希 == 0) 重新计算粗哈希(key);

    auto 锁 = 特征值链.获取锁();
    确保初始化_已加锁();

    auto* stored = new 主信息类(key);
    stored->命中次数 = std::max<std::uint32_t>(1, stored->命中次数);
    if (stored->可信度 <= 0.0) stored->可信度 = 1.0;

    auto* parent = 类型根[idx(stored->类型)];
    auto* node = 特征值链.添加子节点_已加锁(parent, stored);
    索引插入_已加锁(*stored, node);
    return node;
}

特征值类::节点类* 特征值类::创建_不查重(主信息类&& mi) const
{
    主信息类 key = std::move(mi);
    if (key.粗哈希 == 0) 重新计算粗哈希(key);

    auto 锁 = 特征值链.获取锁();
    确保初始化_已加锁();

    auto* stored = new 主信息类(std::move(key));
    stored->命中次数 = std::max<std::uint32_t>(1, stored->命中次数);
    if (stored->可信度 <= 0.0) stored->可信度 = 1.0;

    auto* parent = 类型根[idx(stored->类型)];
    auto* node = 特征值链.添加子节点_已加锁(parent, stored);
    索引插入_已加锁(*stored, node);
    return node;
}

bool 特征值类::覆盖(节点类* 节点, const 主信息类& 新值) const
{
    if (!节点 || !节点->主信息) return false;

    auto 锁 = 特征值链.获取锁();
    确保初始化_已加锁();

    主信息类 old = *节点->主信息;
    主信息类 neu = 新值;
    if (neu.粗哈希 == 0) 重新计算粗哈希(neu);

    // 更新索引：先移除旧 key
    索引移除_已加锁(old, 节点);

    // 覆盖内容
    *节点->主信息 = neu;

    // 再插入新 key
    索引插入_已加锁(neu, 节点);
    return true;
}

bool 特征值类::尝试融合_已加锁(主信息类& inout, const 主信息类& obs, const 融合参数& p)
{
    if (inout.类型 != obs.类型) return false;
    if (inout.比较模式 != obs.比较模式) return false;

    // 命中次数/可信度
    inout.命中次数 += 1;
    inout.可信度 = (((1.0) < (inout.可信度 + 0.02)) ? (1.0) : (inout.可信度 + 0.02));

    const double lr = std::clamp(p.学习率, 0.0, 1.0);

    switch (inout.类型)
    {
    case 枚举_特征值类型::I64:
        if (auto* a = std::get_if<std::int64_t>(&inout.值)) {
            if (auto* b = std::get_if<std::int64_t>(&obs.值)) {
                const double nv = (1.0 - lr) * (double)(*a) + lr * (double)(*b);
                *a = (std::int64_t)std::llround(nv);
                return true;
            }
        }
        return false;

    case 枚举_特征值类型::U64:
        if (auto* a = std::get_if<std::uint64_t>(&inout.值)) {
            if (auto* b = std::get_if<std::uint64_t>(&obs.值)) {
                const double nv = (1.0 - lr) * (double)(*a) + lr * (double)(*b);
                const long long r = (long long)std::llround(nv);
                *a = (std::uint64_t)std::max<long long>(0, r);
                return true;
            }
        }
        return false;

    case 枚举_特征值类型::U8:
        if (auto* a = std::get_if<std::uint8_t>(&inout.值)) {
            if (auto* b = std::get_if<std::uint8_t>(&obs.值)) {
                const double nv = (1.0 - lr) * (double)(*a) + lr * (double)(*b);
                *a = clamp_u8((long long)std::llround(nv));
                return true;
            }
        }
        return false;

    case 枚举_特征值类型::F64:
        if (auto* a = std::get_if<double>(&inout.值)) {
            if (auto* b = std::get_if<double>(&obs.值)) {
                *a = (1.0 - lr) * (*a) + lr * (*b);
                return true;
            }
        }
        return false;

    case 枚举_特征值类型::区间_I64:
        if (auto* a = std::get_if<区间_i64>(&inout.值)) {
            if (auto* b = std::get_if<区间_i64>(&obs.值)) {
                if (p.区间扩张) {
                    a->lo = (((a->lo) < (b->lo)) ? (a->lo) : (b->lo));
                    a->hi = (((a->hi) > (b->hi)) ? (a->hi) : (b->hi));
                }
                else {
                    // 简单 EMA：对 lo/hi 分别收敛
                    a->lo = (std::int64_t)std::llround((1.0 - lr) * (double)a->lo + lr * (double)b->lo);
                    a->hi = (std::int64_t)std::llround((1.0 - lr) * (double)a->hi + lr * (double)b->hi);
                }
                return true;
            }
        }
        return false;

    case 枚举_特征值类型::区间_F64:
        if (auto* a = std::get_if<区间_f64>(&inout.值)) {
            if (auto* b = std::get_if<区间_f64>(&obs.值)) {
                if (p.区间扩张) {
                    a->lo = (((a->lo) < (b->lo)) ? (a->lo) : (b->lo));
                    a->hi = (((a->hi) > (b->hi)) ? (a->hi) : (b->hi));
                }
                else {
                    a->lo = (1.0 - lr) * a->lo + lr * b->lo;
                    a->hi = (1.0 - lr) * a->hi + lr * b->hi;
                }
                return true;
            }
        }
        return false;

    case 枚举_特征值类型::颜色_BGR8:
        if (auto* a = std::get_if<颜色_BGR8>(&inout.值)) {
            if (auto* b = std::get_if<颜色_BGR8>(&obs.值)) {
                a->b = clamp_u8((long long)std::llround((1.0 - lr) * a->b + lr * b->b));
                a->g = clamp_u8((long long)std::llround((1.0 - lr) * a->g + lr * b->g));
                a->r = clamp_u8((long long)std::llround((1.0 - lr) * a->r + lr * b->r));
                return true;
            }
        }
        return false;

    case 枚举_特征值类型::Bits64:
        if (auto* a = std::get_if<Bits64>(&inout.值)) {
            if (auto* b = std::get_if<Bits64>(&obs.值)) {
                if (p.Bits融合策略 == 1)      a->w |= b->w;
                else if (p.Bits融合策略 == 2) a->w &= b->w;
                else {
                    // 不融合，仅计数
                }
                return true;
            }
        }
        return false;

    case 枚举_特征值类型::VecF32:
        if (auto* a = std::get_if<std::vector<float>>(&inout.值)) {
            if (auto* b = std::get_if<std::vector<float>>(&obs.值)) {
                if (a->size() != b->size()) return false;
                for (size_t i = 0; i < a->size(); ++i) {
                    (*a)[i] = (float)((1.0 - lr) * (double)(*a)[i] + lr * (double)(*b)[i]);
                }
                return true;
            }
        }
        return false;

        // VecI64/Bytes：一般视为离散值，不建议融合；这里默认“仅计数”
    case 枚举_特征值类型::VecI64:
    case 枚举_特征值类型::Bytes:
        return true;

    default:
        return false;
    }
}

bool 特征值类::融合(节点类* 旧节点, const 主信息类& 新观测, const 融合参数& p, bool 失败则覆盖) const
{
    if (!旧节点 || !旧节点->主信息) return false;

    auto 锁 = 特征值链.获取锁();
    确保初始化_已加锁();

    主信息类 old = *旧节点->主信息;

    主信息类 obs = 新观测;
    if (obs.粗哈希 == 0) 重新计算粗哈希(obs);

    // 先从旧索引移除
    索引移除_已加锁(old, 旧节点);

    主信息类 merged = old;
    const bool ok = 尝试融合_已加锁(merged, obs, p);
    if (!ok) {
        if (失败则覆盖) {
            merged = obs;
        }
        else {
            // 加回旧索引
            索引插入_已加锁(old, 旧节点);
            return false;
        }
    }

    重新计算粗哈希(merged);
    *旧节点->主信息 = merged;

    // 插入新索引
    索引插入_已加锁(merged, 旧节点);
    return true;
}

bool 特征值类::删除(节点类* 节点) const
{
    if (!节点 || !节点->主信息) return false;

    auto 锁 = 特征值链.获取锁();
    确保初始化_已加锁();

    // 避免误删类型根
    for (auto* r : 类型根) {
        if (节点 == r) return false;
    }

    主信息类 old = *节点->主信息;
    索引移除_已加锁(old, 节点);

    特征值链.删除节点_已加锁(节点);
    return true;
}
