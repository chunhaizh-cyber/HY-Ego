module;
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cmath>
#include <algorithm>

module 特征值模块;

import 基础数据类型模块;
import 主信息定义模块;
import 数据仓库模块;


using namespace 数据仓库模块;

namespace {
    inline 锁调度器守卫 特征值链写锁守卫_(const std::string& 调用点) {
        return 锁调度器守卫({
            锁请求::写(特征值链.链表锁, 枚举_锁域::特征值链, "特征值链", 调用点)
            });
    }
}

static inline std::size_t ceil_sqrt_size(std::size_t x) {
    if (x <= 1) return x;
    long double r = std::sqrt((long double)x);
    std::size_t n = (std::size_t)r;
    while (n * n < x) ++n;
    return n;
}
static inline std::size_t ceil_cbrt_size(std::size_t x) {
    if (x <= 1) return x;
    long double r = std::cbrt((long double)x);
    std::size_t n = (std::size_t)r;
    auto cube = [](std::size_t t) { return t * t * t; };
    while (cube(n) < x) ++n;
    return n;
}

std::uint64_t 特征值类::FNV1a64_Bytes(const void* data, std::size_t len) noexcept {
    const std::uint8_t* p = (const std::uint8_t*)data;
    std::uint64_t h = 1469598103934665603ull;
    for (std::size_t i = 0; i < len; ++i) {
        h ^= (std::uint64_t)p[i];
        h *= 1099511628211ull;
    }
    return h;
}

std::uint64_t 特征值类::FNV1a64_混入64位值(std::uint64_t h, std::uint64_t x) noexcept {
    for (int i = 0; i < 8; ++i) {
        const std::uint8_t b = (std::uint8_t)((x >> (i * 8)) & 0xFF);
        h ^= (std::uint64_t)b;
        h *= 1099511628211ull;
    }
    return h;
}

std::uint64_t 特征值类::哈希VecIU64(const VecIU64& v) noexcept {
    std::uint64_t h = 1469598103934665603ull;
    h = FNV1a64_混入64位值(h, (std::uint64_t)v.size());
    for (auto x : v) h = FNV1a64_混入64位值(h, x);
    return h;
}

void 特征值类::初始化() const {
    auto lk = 特征值链写锁守卫_("特征值类::初始化");
    私有_重建缓存_已加锁();
}

void 特征值类::私有_确保缓存_已加锁() const {
    if (!缓存已构建_) 私有_重建缓存_已加锁();
}

void 特征值类::私有_重建缓存_已加锁() const {
    哈希到候选_.clear();

    auto* root = 特征值链.根指针;
    if (!root) { 缓存已构建_ = true; return; }

    for (auto* it = root->链下; it && it != root; it = it->链下) {
        if (!it->主信息) continue;
        auto* mi = it->主信息;

        if (mi->内容哈希 == 0) mi->内容哈希 = 哈希VecIU64(mi->值);
        哈希到候选_[mi->内容哈希].push_back(it);
    }

    缓存已构建_ = true;
}

// ==================== 唯一性（绝对相同） ====================

特征值节点类* 特征值类::查找(const VecIU64& v) const {
    auto lk = 特征值链写锁守卫_("特征值类::查找");
    私有_确保缓存_已加锁();
    return 查找_已加锁(v);
}


特征值节点类* 特征值类::查找(const 特征值主信息类& mi) const {
    return 查找(mi.值);
}

特征值节点类* 特征值类::查找_已加锁(const VecIU64& v) const {
    const std::uint64_t h = 哈希VecIU64(v);
    auto it = 哈希到候选_.find(h);
    if (it == 哈希到候选_.end()) return nullptr;

    for (auto* n : it->second) {
        if (!n || !n->主信息) continue;
        if (n->主信息->值 == v) return n;
    }
    return nullptr;
}


特征值节点类* 特征值类::查找_已加锁(const 特征值主信息类& mi) const {
    return 查找_已加锁(mi.值);
}

特征值节点类* 特征值类::获取或创建(VecIU64 v, 枚举_轮廓维度 dim) const {
    auto lk = 特征值链写锁守卫_("特征值类::获取或创建");
    私有_确保缓存_已加锁();
    return 获取或创建_已加锁(std::move(v), dim);
}


特征值节点类* 特征值类::获取或创建(特征值主信息类&& mi, 枚举_轮廓维度 dim) const {
    // mi 当前仅包含 VecIU64；这里仍按“唯一性+可选分层”处理
    return 获取或创建(std::move(mi.值), dim);
}

VecU句柄 特征值类::获取或创建句柄(VecIU64 v, 枚举_轮廓维度 dim) const {
    auto* n = 获取或创建(std::move(v), dim);
    VecU句柄 h{};
    h.主信息指针 = n && n->主信息 ? (std::uintptr_t)n->主信息 : 0;
    return h;
}

特征值节点类* 特征值类::获取或创建_已加锁(VecIU64&& v, 枚举_轮廓维度 dim) const {
    // 1) 唯一性查找（锁内原子）
    if (auto* hit = 查找_已加锁(v)) return hit;

    // 2) 创建主信息并添加（创建节点入链 + 入唯一性缓存 + 可选分层）
    auto* mi = new 特征值主信息类(std::move(v));
    mi->内容哈希 = 哈希VecIU64(mi->值);
    return 添加_分层_已加锁(mi, dim);
}


特征值节点类* 特征值类::获取或创建_已加锁(特征值主信息类&& mi, 枚举_轮廓维度 dim) const {
    return 获取或创建_已加锁(std::move(mi.值), dim);
}


特征值节点类* 特征值类::添加_分层_已加锁(特征值主信息类* mi, 枚举_轮廓维度 dim) const {
    if (!mi) return nullptr;

    // 节点入链
    auto* node = 特征值链.添加节点_已加锁(nullptr, mi);

    // 入唯一性缓存
    if (mi->内容哈希 == 0) mi->内容哈希 = 哈希VecIU64(mi->值);
    哈希到候选_[mi->内容哈希].push_back(node);

    // 分层登记（仅组织）
    if (dim != 枚举_轮廓维度::未定义) {
        std::vector<VecIU64> 层{};
        std::vector<I64> tokens;
        if (dim == 枚举_轮廓维度::二维) tokens = 生成二维轮廓金字塔(mi->值, 层);
        else tokens = 生成三维轮廓金字塔(mi->值, 层);
        分层_登记叶子_已加锁(dim, tokens, node);
    }
    return node;
}

// ==================== 分层索引（组织用） ====================

特征值类::分层索引节点* 特征值类::分层_取或创建路径_已加锁(分层索引节点*& root, const std::vector<I64>& tokens) const {
    if (!root) {
        索引池_.emplace_back(std::make_unique<分层索引节点>());
        root = 索引池_.back().get();
    }
    auto* cur = root;
    for (auto t : tokens) {
        auto it = cur->子.find(t);
        if (it == cur->子.end()) {
            索引池_.emplace_back(std::make_unique<分层索引节点>());
            auto* nn = 索引池_.back().get();
            cur->子.emplace(t, nn);
            cur = nn;
        }
        else {
            cur = it->second;
        }
    }
    return cur;
}

void 特征值类::分层_登记叶子_已加锁(枚举_轮廓维度 dim, const std::vector<I64>& tokens, 特征值节点类* leaf) const {
    if (!leaf) return;
    auto*& root = (dim == 枚举_轮廓维度::二维) ? 根2D_ : 根3D_;
    auto* path = 分层_取或创建路径_已加锁(root, tokens);
    path->叶子.push_back(leaf);
}

// ==================== 2D/3D 金字塔（64*N*N / 64*N*N*N） ====================
//
// 规则：
// - 输入 VecIU64 的长度将补齐到：
//     * 2D：64*N*N  => side=8*N => pad to side^2
//     * 3D：64*N^3  => side=4*N => pad to side^3
// - 每层通过按位 OR 做 group 合并：
//     * 2D：2x2 -> 1（group=4）
//     * 3D：2x2x2 -> 1（group=8）
// - tokens：对每一层做 FNV-1a 64 哈希，返回从粗到细（top->...->base）
//
// 说明：tokens 只用于分层组织，不用于唯一性判定。

static inline std::uint64_t FNV1a64_混入64位值_local(std::uint64_t h, std::uint64_t x) noexcept {
    for (int i = 0; i < 8; ++i) {
        const std::uint8_t b = (std::uint8_t)((x >> (i * 8)) & 0xFF);
        h ^= (std::uint64_t)b;
        h *= 1099511628211ull;
    }
    return h;
}

static inline std::uint64_t 哈希层(const VecIU64& layer) {
    std::uint64_t h = 1469598103934665603ull;
    h = FNV1a64_混入64位值_local(h, (std::uint64_t)layer.size());
    for (auto x : layer) h = FNV1a64_混入64位值_local(h, x);
    return h;
}

std::vector<I64> 特征值类::生成二维轮廓金字塔(VecIU64 base, std::vector<VecIU64>& 输出层) {
    输出层.clear();
    std::vector<I64> tokens{};
    if (base.empty()) return tokens;

    std::size_t cnt = base.size();
    std::size_t side = ceil_sqrt_size(cnt);
    if (side % 8 != 0) side = ((side + 7) / 8) * 8;
    std::size_t target = side * side;

    if (base.size() < target) base.resize(target, 0);
    输出层.push_back(std::move(base));

    std::size_t W = side, H = side;
    auto idx2 = [&](std::size_t x, std::size_t y, std::size_t w) -> std::size_t { return y * w + x; };

    while (W > 1 || H > 1) {
        const auto& prev = 输出层.back();
        std::size_t newW = (W + 1) / 2;
        std::size_t newH = (H + 1) / 2;

        VecIU64 cur;
        cur.resize(newW * newH, 0);

        auto pick = [&](std::size_t x, std::size_t y) -> std::uint64_t {
            if (x >= W || y >= H) return 0;
            return prev[idx2(x, y, W)];
            };

        for (std::size_t y = 0; y < newH; ++y) {
            for (std::size_t x = 0; x < newW; ++x) {
                std::uint64_t a = pick(x * 2 + 0, y * 2 + 0);
                std::uint64_t b = pick(x * 2 + 1, y * 2 + 0);
                std::uint64_t c = pick(x * 2 + 0, y * 2 + 1);
                std::uint64_t d = pick(x * 2 + 1, y * 2 + 1);
                cur[idx2(x, y, newW)] = a | b | c | d;
            }
        }

        输出层.push_back(std::move(cur));
        W = newW; H = newH;
    }

    // tokens：粗->细
    tokens.reserve(输出层.size());
    for (auto it = 输出层.rbegin(); it != 输出层.rend(); ++it) {
        tokens.push_back((I64)哈希层(*it));
    }
    return tokens;
}

std::vector<I64> 特征值类::生成三维轮廓金字塔(VecIU64 base, std::vector<VecIU64>& 输出层) {
    输出层.clear();
    std::vector<I64> tokens{};
    if (base.empty()) return tokens;

    std::size_t cnt = base.size();
    std::size_t side = ceil_cbrt_size(cnt);
    if (side % 4 != 0) side = ((side + 3) / 4) * 4;
    std::size_t target = side * side * side;

    if (base.size() < target) base.resize(target, 0);
    输出层.push_back(std::move(base));

    std::size_t W = side, H = side, D = side;
    auto idx3 = [&](std::size_t x, std::size_t y, std::size_t z, std::size_t w, std::size_t h) -> std::size_t {
        return (z * h + y) * w + x; // (z,y,x)
        };

    while (W > 1 || H > 1 || D > 1) {
        const auto& prev = 输出层.back();
        std::size_t newW = (W + 1) / 2;
        std::size_t newH = (H + 1) / 2;
        std::size_t newD = (D + 1) / 2;

        VecIU64 cur;
        cur.resize(newW * newH * newD, 0);

        auto pick = [&](std::size_t x, std::size_t y, std::size_t z) -> std::uint64_t {
            if (x >= W || y >= H || z >= D) return 0;
            return prev[idx3(x, y, z, W, H)];
            };

        for (std::size_t z = 0; z < newD; ++z) {
            for (std::size_t y = 0; y < newH; ++y) {
                for (std::size_t x = 0; x < newW; ++x) {
                    std::uint64_t v = 0;
                    v |= pick(x * 2 + 0, y * 2 + 0, z * 2 + 0);
                    v |= pick(x * 2 + 1, y * 2 + 0, z * 2 + 0);
                    v |= pick(x * 2 + 0, y * 2 + 1, z * 2 + 0);
                    v |= pick(x * 2 + 1, y * 2 + 1, z * 2 + 0);

                    v |= pick(x * 2 + 0, y * 2 + 0, z * 2 + 1);
                    v |= pick(x * 2 + 1, y * 2 + 0, z * 2 + 1);
                    v |= pick(x * 2 + 0, y * 2 + 1, z * 2 + 1);
                    v |= pick(x * 2 + 1, y * 2 + 1, z * 2 + 1);

                    cur[idx3(x, y, z, newW, newH)] = v;
                }
            }
        }

        输出层.push_back(std::move(cur));
        W = newW; H = newH; D = newD;
    }

    tokens.reserve(输出层.size());
    for (auto it = 输出层.rbegin(); it != 输出层.rend(); ++it) {
        tokens.push_back((I64)哈希层(*it));
    }
    return tokens;
}
