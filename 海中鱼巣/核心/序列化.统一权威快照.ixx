// 文件规则：只按 NT-C4 固定数值 ABI 形成规范化字节，不写 C++ 内存布局、指针、size_t 或文件。
module;

#include "统一权威快照.数据.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <new>
#include <utility>

export module 海中鱼巣.核心.序列化.统一权威快照;

export namespace 海中鱼巣 {

namespace {

class 摘要计算器 {
public:
    摘要计算器() noexcept
        : 状态_{
            0x6a09e667U, 0xbb67ae85U, 0x3c6ef372U, 0xa54ff53aU,
            0x510e527fU, 0x9b05688cU, 0x1f83d9abU, 0x5be0cd19U } {}

    void 写入(const std::uint8_t* 数据, std::size_t 长度) noexcept {
        总字节数_ += 长度;
        while (长度 != 0) {
            const std::size_t 可复制 = std::min(长度, 缓冲区_.size() - 缓冲字节数_);
            std::copy_n(数据, 可复制, 缓冲区_.begin() + static_cast<std::ptrdiff_t>(缓冲字节数_));
            缓冲字节数_ += 可复制;
            数据 += 可复制;
            长度 -= 可复制;
            if (缓冲字节数_ == 缓冲区_.size()) {
                变换();
                缓冲字节数_ = 0;
            }
        }
    }

    规范化权威载荷摘要 完成() noexcept {
        const std::uint64_t 位数 = 总字节数_ * 8;
        缓冲区_[缓冲字节数_++] = 0x80;
        if (缓冲字节数_ > 56) {
            while (缓冲字节数_ < 64) 缓冲区_[缓冲字节数_++] = 0;
            变换();
            缓冲字节数_ = 0;
        }
        while (缓冲字节数_ < 56) 缓冲区_[缓冲字节数_++] = 0;
        for (std::size_t 偏移 = 0; 偏移 < 8; ++偏移) {
            缓冲区_[56 + 偏移] = static_cast<std::uint8_t>(位数 >> (56 - 偏移 * 8));
        }
        变换();

        规范化权威载荷摘要 摘要{};
        for (std::size_t 索引 = 0; 索引 < 状态_.size(); ++索引) {
            摘要[索引 * 4] = static_cast<std::uint8_t>(状态_[索引] >> 24);
            摘要[索引 * 4 + 1] = static_cast<std::uint8_t>(状态_[索引] >> 16);
            摘要[索引 * 4 + 2] = static_cast<std::uint8_t>(状态_[索引] >> 8);
            摘要[索引 * 4 + 3] = static_cast<std::uint8_t>(状态_[索引]);
        }
        return 摘要;
    }

private:
    static constexpr std::array<std::uint32_t, 64> 常量_ = {
        0x428a2f98U,0x71374491U,0xb5c0fbcfU,0xe9b5dba5U,0x3956c25bU,0x59f111f1U,0x923f82a4U,0xab1c5ed5U,
        0xd807aa98U,0x12835b01U,0x243185beU,0x550c7dc3U,0x72be5d74U,0x80deb1feU,0x9bdc06a7U,0xc19bf174U,
        0xe49b69c1U,0xefbe4786U,0x0fc19dc6U,0x240ca1ccU,0x2de92c6fU,0x4a7484aaU,0x5cb0a9dcU,0x76f988daU,
        0x983e5152U,0xa831c66dU,0xb00327c8U,0xbf597fc7U,0xc6e00bf3U,0xd5a79147U,0x06ca6351U,0x14292967U,
        0x27b70a85U,0x2e1b2138U,0x4d2c6dfcU,0x53380d13U,0x650a7354U,0x766a0abbU,0x81c2c92eU,0x92722c85U,
        0xa2bfe8a1U,0xa81a664bU,0xc24b8b70U,0xc76c51a3U,0xd192e819U,0xd6990624U,0xf40e3585U,0x106aa070U,
        0x19a4c116U,0x1e376c08U,0x2748774cU,0x34b0bcb5U,0x391c0cb3U,0x4ed8aa4aU,0x5b9cca4fU,0x682e6ff3U,
        0x748f82eeU,0x78a5636fU,0x84c87814U,0x8cc70208U,0x90befffaU,0xa4506cebU,0xbef9a3f7U,0xc67178f2U
    };

    static std::uint32_t 右旋(std::uint32_t 值, std::uint32_t 位数) noexcept {
        return (值 >> 位数) | (值 << (32 - 位数));
    }

    void 变换() noexcept {
        std::array<std::uint32_t, 64> 工作{};
        for (std::size_t 索引 = 0; 索引 < 16; ++索引) {
            const std::size_t 偏移 = 索引 * 4;
            工作[索引] = (static_cast<std::uint32_t>(缓冲区_[偏移]) << 24)
                | (static_cast<std::uint32_t>(缓冲区_[偏移 + 1]) << 16)
                | (static_cast<std::uint32_t>(缓冲区_[偏移 + 2]) << 8)
                | static_cast<std::uint32_t>(缓冲区_[偏移 + 3]);
        }
        for (std::size_t 索引 = 16; 索引 < 64; ++索引) {
            const std::uint32_t 小一 = 右旋(工作[索引 - 15], 7) ^ 右旋(工作[索引 - 15], 18) ^ (工作[索引 - 15] >> 3);
            const std::uint32_t 小二 = 右旋(工作[索引 - 2], 17) ^ 右旋(工作[索引 - 2], 19) ^ (工作[索引 - 2] >> 10);
            工作[索引] = 工作[索引 - 16] + 小一 + 工作[索引 - 7] + 小二;
        }

        std::uint32_t a = 状态_[0], b = 状态_[1], c = 状态_[2], d = 状态_[3];
        std::uint32_t e = 状态_[4], f = 状态_[5], g = 状态_[6], h = 状态_[7];
        for (std::size_t 索引 = 0; 索引 < 64; ++索引) {
            const std::uint32_t 大一 = 右旋(e, 6) ^ 右旋(e, 11) ^ 右旋(e, 25);
            const std::uint32_t 选择 = (e & f) ^ ((~e) & g);
            const std::uint32_t 临时一 = h + 大一 + 选择 + 常量_[索引] + 工作[索引];
            const std::uint32_t 大二 = 右旋(a, 2) ^ 右旋(a, 13) ^ 右旋(a, 22);
            const std::uint32_t 多数 = (a & b) ^ (a & c) ^ (b & c);
            const std::uint32_t 临时二 = 大二 + 多数;
            h = g; g = f; f = e; e = d + 临时一; d = c; c = b; b = a; a = 临时一 + 临时二;
        }
        状态_[0] += a; 状态_[1] += b; 状态_[2] += c; 状态_[3] += d;
        状态_[4] += e; 状态_[5] += f; 状态_[6] += g; 状态_[7] += h;
    }

    std::array<std::uint32_t, 8> 状态_{};
    std::array<std::uint8_t, 64> 缓冲区_{};
    std::uint64_t 总字节数_ = 0;
    std::size_t 缓冲字节数_ = 0;
};

class 规范化字节写入器 {
public:
    void 写入8(std::uint8_t 值) { 字节组_.push_back(值); }
    void 写入16(std::uint16_t 值) { 写入8(static_cast<std::uint8_t>(值)); 写入8(static_cast<std::uint8_t>(值 >> 8)); }
    void 写入32(std::uint32_t 值) { for (std::uint32_t 偏移 = 0; 偏移 < 32; 偏移 += 8) 写入8(static_cast<std::uint8_t>(值 >> 偏移)); }
    void 写入64(std::uint64_t 值) { for (std::uint32_t 偏移 = 0; 偏移 < 64; 偏移 += 8) 写入8(static_cast<std::uint8_t>(值 >> 偏移)); }
    void 写入有符号64(std::int64_t 值) { 写入64(static_cast<std::uint64_t>(值)); }
    void 写入摘要(const 规范化权威载荷摘要& 摘要) { 字节组_.insert(字节组_.end(), 摘要.begin(), 摘要.end()); }
    void 写入字节(const std::vector<std::uint8_t>& 字节) { 字节组_.insert(字节组_.end(), 字节.begin(), 字节.end()); }
    std::vector<std::uint8_t> 取走() && { return std::move(字节组_); }
    const std::vector<std::uint8_t>& 读取() const noexcept { return 字节组_; }
private:
    std::vector<std::uint8_t> 字节组_;
};

规范化权威载荷摘要 计算摘要(const std::vector<std::uint8_t>& 字节) noexcept {
    摘要计算器 计算器;
    if (!字节.empty()) 计算器.写入(字节.data(), 字节.size());
    return 计算器.完成();
}

void 写入来源(规范化字节写入器& 写入器, const 统一段来源& 来源) {
    写入器.写入64(来源.来源运行期域身份);
    写入器.写入64(来源.来源节点直接仓库身份);
    写入器.写入64(来源.快照代次);
}

template<class 段类型, class 记录写入函数>
std::optional<std::vector<std::uint8_t>> 形成段载荷(const 段类型& 段, 记录写入函数&& 记录写入函数对象) {
    std::vector<std::vector<std::uint8_t>> 记录字节组;
    记录字节组.reserve(段.记录组.size());
    for (const auto& 记录 : 段.记录组) {
        规范化字节写入器 记录写入器;
        记录写入函数对象(记录写入器, 记录);
        const auto& 记录字节 = 记录写入器.读取();
        if (记录字节.size() > 统一权威单条最大字节数) return std::nullopt;
        记录字节组.push_back(记录字节);
    }
    std::sort(记录字节组.begin(), 记录字节组.end(), [](const auto& 左, const auto& 右) {
        return std::lexicographical_compare(左.begin(), 左.end(), 右.begin(), 右.end());
    });

    规范化字节写入器 写入器;
    写入来源(写入器, 段.来源);
    写入器.写入64(段.高水位);
    for (const auto& 记录字节 : 记录字节组) {
        写入器.写入32(static_cast<std::uint32_t>(记录字节.size()));
        写入器.写入字节(记录字节);
    }
    if (写入器.读取().size() > 统一权威单段最大字节数) return std::nullopt;
    return std::move(写入器).取走();
}

void 写入定长16(std::vector<std::uint8_t>& 字节, std::size_t 偏移, std::uint16_t 值) { 字节[偏移] = static_cast<std::uint8_t>(值); 字节[偏移 + 1] = static_cast<std::uint8_t>(值 >> 8); }
void 写入定长32(std::vector<std::uint8_t>& 字节, std::size_t 偏移, std::uint32_t 值) { for (std::uint32_t 位移 = 0; 位移 < 32; 位移 += 8) 字节[偏移 + 位移 / 8] = static_cast<std::uint8_t>(值 >> 位移); }
void 写入定长64(std::vector<std::uint8_t>& 字节, std::size_t 偏移, std::uint64_t 值) { for (std::uint32_t 位移 = 0; 位移 < 64; 位移 += 8) 字节[偏移 + 位移 / 8] = static_cast<std::uint8_t>(值 >> 位移); }
void 写入定长摘要(std::vector<std::uint8_t>& 字节, std::size_t 偏移, const 规范化权威载荷摘要& 摘要) { std::copy(摘要.begin(), 摘要.end(), 字节.begin() + static_cast<std::ptrdiff_t>(偏移)); }

template<class 段类型, class 记录写入函数>
std::optional<std::vector<std::uint8_t>> 形成段(const 段类型& 段, 统一权威快照段类型 类型, std::uint32_t 顺序, 记录写入函数&& 记录写入函数对象) {
    auto 载荷结果 = 形成段载荷(段, std::forward<记录写入函数>(记录写入函数对象));
    if (!载荷结果.has_value()) return std::nullopt;
    auto 载荷 = std::move(*载荷结果);
    规范化字节写入器 段写入器;
    段写入器.写入16(static_cast<std::uint16_t>(类型));
    段写入器.写入16(1);
    段写入器.写入32(64);
    段写入器.写入32(顺序);
    段写入器.写入32(0);
    段写入器.写入64(static_cast<std::uint64_t>(段.记录组.size()));
    段写入器.写入64(static_cast<std::uint64_t>(载荷.size()));
    段写入器.写入摘要(计算摘要(载荷));
    段写入器.写入字节(载荷);
    return std::move(段写入器).取走();
}

bool 输入上限有效(const 统一权威冻结材料& 材料) noexcept {
    const bool 段数量有效 = 材料.节点段.记录组.size() <= 统一权威单段最大记录数
        && 材料.关系段.记录组.size() <= 统一权威单段最大记录数
        && 材料.特征值段.记录组.size() <= 统一权威单段最大记录数
        && 材料.状态段.记录组.size() <= 统一权威单段最大记录数
        && 材料.动态段.记录组.size() <= 统一权威单段最大记录数
        && 材料.任务方法段.记录组.size() <= 统一权威单段最大记录数
        && 材料.概念段.记录组.size() <= 统一权威单段最大记录数
        && 材料.用途段.记录组.size() <= 统一权威单段最大记录数
        && 材料.批次段.记录组.size() <= 统一权威单段最大记录数;
    if (!段数量有效) return false;
    for (const auto& 记录 : 材料.特征值段.记录组) {
        if (记录.VecI64值.size() > 统一权威向量最大项数 || 记录.VecU64值.size() > 统一权威向量最大项数) return false;
    }
    for (const auto& 记录 : 材料.概念段.记录组) {
        if (记录.有序非拓扑约束原始值.size() > 统一权威向量最大项数) return false;
    }
    for (const auto& 记录 : 材料.用途段.记录组) {
        if (记录.观察材料UTF8.size() > 统一权威字符串最大字节数) return false;
    }
    return true;
}

} // namespace

规范化编码结果 序列化统一权威快照(const 统一权威冻结材料& 材料) {
    规范化编码结果 结果;
    try {
        if (!输入上限有效(材料)) {
            结果.状态 = 规范化编码结果状态::内部不一致;
            return 结果;
        }
        auto 节点结果 = 形成段(材料.节点段, 统一权威快照段类型::节点直接记录, 0, [](规范化字节写入器& 写入器, const 节点直接记录& 记录) {
            写入器.写入64(记录.稳定主键高位); 写入器.写入64(记录.稳定主键低位); 写入器.写入16(记录.节点类型ABI); 写入器.写入32(记录.版本号); 写入器.写入16(记录.状态ABI); 写入器.写入64(记录.创建序号); 写入器.写入32(记录.命名域ABI版本); 写入器.写入64(记录.域签发高水位);
        });
        auto 关系结果 = 形成段(材料.关系段, 统一权威快照段类型::正式关系记录, 1, [](规范化字节写入器& 写入器, const 正式关系记录& 记录) {
            写入器.写入64(记录.关系编号); 写入器.写入32(记录.关系版本); 写入器.写入16(记录.关系类型ABI); 写入器.写入16(记录.角色ABI); 写入器.写入64(记录.源主键高位); 写入器.写入64(记录.源主键低位); 写入器.写入64(记录.目标主键高位); 写入器.写入64(记录.目标主键低位); 写入器.写入64(记录.顺序号); 写入器.写入16(记录.状态ABI); 写入器.写入8(记录.历史标记);
        });
        auto 特征值结果 = 形成段(材料.特征值段, 统一权威快照段类型::特征值类型化记录, 2, [](规范化字节写入器& 写入器, const 特征值类型化记录& 记录) {
            写入器.写入64(记录.记录编号); 写入器.写入64(记录.槽位主键高位); 写入器.写入64(记录.槽位主键低位); 写入器.写入16(记录.原始值类型ABI); 写入器.写入32(记录.原始值版本); 写入器.写入8(记录.存在I64值); if (记录.存在I64值 != 0) 写入器.写入有符号64(记录.I64值); 写入器.写入32(static_cast<std::uint32_t>(记录.VecI64值.size())); for (auto 值 : 记录.VecI64值) 写入器.写入有符号64(值); 写入器.写入32(static_cast<std::uint32_t>(记录.VecU64值.size())); for (auto 值 : 记录.VecU64值) 写入器.写入64(值); 写入器.写入8(记录.历史标记);
        });
        auto 状态结果 = 形成段(材料.状态段, 统一权威快照段类型::状态域记录, 3, [](规范化字节写入器& 写入器, const 状态域记录& 记录) {
            写入器.写入64(记录.记录编号); 写入器.写入64(记录.主体主键高位); 写入器.写入64(记录.主体主键低位); 写入器.写入64(记录.特征主键高位); 写入器.写入64(记录.特征主键低位); 写入器.写入64(记录.值主键高位); 写入器.写入64(记录.值主键低位); 写入器.写入64(记录.场景主键高位); 写入器.写入64(记录.场景主键低位); 写入器.写入64(记录.发生时间); 写入器.写入64(记录.接收时间); 写入器.写入64(记录.来源主键); 写入器.写入16(记录.记录模式ABI); 写入器.写入8(记录.历史标记);
        });
        auto 动态结果 = 形成段(材料.动态段, 统一权威快照段类型::动态域记录, 4, [](规范化字节写入器& 写入器, const 动态域记录& 记录) {
            写入器.写入64(记录.记录编号); 写入器.写入64(记录.主体主键高位); 写入器.写入64(记录.主体主键低位); 写入器.写入64(记录.被改变目标高位); 写入器.写入64(记录.被改变目标低位); 写入器.写入64(记录.前状态主键); 写入器.写入64(记录.后状态主键); 写入器.写入64(记录.场景主键高位); 写入器.写入64(记录.场景主键低位); 写入器.写入64(记录.发生时间); 写入器.写入64(记录.来源动作主键); 写入器.写入16(记录.动态种类ABI); 写入器.写入16(记录.聚合规则版本); 写入器.写入8(记录.历史标记);
        });
        auto 任务方法结果 = 形成段(材料.任务方法段, 统一权威快照段类型::任务方法选择记录, 5, [](规范化字节写入器& 写入器, const 任务方法选择记录& 记录) {
            写入器.写入64(记录.幂等材料编号); 写入器.写入64(记录.筹办轮次); 写入器.写入32(记录.规则版本); 写入器.写入32(记录.方法内容版本); 写入器.写入16(记录.关系角色ABI); 写入器.写入8(记录.历史标记);
        });
        auto 概念结果 = 形成段(材料.概念段, 统一权威快照段类型::概念签名记录, 6, [](规范化字节写入器& 写入器, const 概念签名记录& 记录) {
            写入器.写入64(记录.概念主键高位); 写入器.写入64(记录.概念主键低位); 写入器.写入32(记录.签名规则版本); 写入器.写入32(static_cast<std::uint32_t>(记录.有序非拓扑约束原始值.size())); for (auto 值 : 记录.有序非拓扑约束原始值) 写入器.写入64(值); 写入器.写入32(记录.记录版本); 写入器.写入8(记录.历史标记);
        });
        auto 用途结果 = 形成段(材料.用途段, 统一权威快照段类型::用途观察记录, 7, [](规范化字节写入器& 写入器, const 用途观察记录& 记录) {
            写入器.写入64(记录.记录编号); 写入器.写入64(记录.所属对象高位); 写入器.写入64(记录.所属对象低位); 写入器.写入32(static_cast<std::uint32_t>(记录.观察材料UTF8.size())); for (unsigned char 字符 : 记录.观察材料UTF8) 写入器.写入8(字符); 写入器.写入32(记录.观察版本); 写入器.写入8(记录.历史标记);
        });
        auto 批次结果 = 形成段(材料.批次段, 统一权威快照段类型::特征批次发布记录, 8, [](规范化字节写入器& 写入器, const 特征批次发布记录& 记录) {
            写入器.写入64(记录.批次编号); 写入器.写入64(记录.槽位主键高位); 写入器.写入64(记录.槽位主键低位); 写入器.写入64(记录.新值主键高位); 写入器.写入64(记录.新值主键低位); 写入器.写入32(记录.顺序号); 写入器.写入32(记录.批次规则版本); 写入器.写入8(记录.历史标记);
        });

        if (!节点结果 || !关系结果 || !特征值结果 || !状态结果 || !动态结果 || !任务方法结果 || !概念结果 || !用途结果 || !批次结果) {
            结果.状态 = 规范化编码结果状态::内部不一致;
            return 结果;
        }
        const std::array<std::vector<std::uint8_t>, 9> 段字节组 = {
            std::move(*节点结果), std::move(*关系结果), std::move(*特征值结果),
            std::move(*状态结果), std::move(*动态结果), std::move(*任务方法结果),
            std::move(*概念结果), std::move(*用途结果), std::move(*批次结果)
        };
        std::vector<std::uint8_t> 输出(统一权威容器头字节数, 0);
        写入定长16(输出, 8, 1); 写入定长16(输出, 10, 1); 写入定长32(输出, 12, 104); 写入定长32(输出, 16, 64); 写入定长32(输出, 20, 1); 写入定长32(输出, 24, 1); 写入定长32(输出, 28, 9);
        std::uint64_t 文件字节数 = 104;
        std::vector<std::uint8_t> 所有载荷;
        for (const auto& 段字节 : 段字节组) { 文件字节数 += 段字节.size(); 所有载荷.insert(所有载荷.end(), 段字节.begin() + 64, 段字节.end()); }
        if (文件字节数 > 统一权威文件最大字节数) { 结果.状态 = 规范化编码结果状态::内部不一致; return 结果; }
        写入定长64(输出, 32, 文件字节数); 写入定长64(输出, 40, 材料.规格.来源运行期域身份); 写入定长64(输出, 48, 材料.规格.来源节点直接仓库身份); 写入定长64(输出, 56, 材料.规格.快照代次); 写入定长32(输出, 64, 1); 写入定长32(输出, 68, 1);
        for (const auto& 段字节 : 段字节组) 输出.insert(输出.end(), 段字节.begin(), 段字节.end());
        const auto 容器摘要 = 计算摘要(输出);
        写入定长摘要(输出, 72, 容器摘要);
        规范化快照字节材料 材料结果;
        材料结果.字节 = std::move(输出);
        材料结果.权威载荷摘要 = 计算摘要(所有载荷);
        结果.材料 = std::move(材料结果);
        结果.状态 = 规范化编码结果状态::已形成;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 规范化编码结果状态::资源失败;
        return 结果;
    } catch (...) {
        结果.状态 = 规范化编码结果状态::内部不一致;
        return 结果;
    }
}

} // namespace 海中鱼巣
