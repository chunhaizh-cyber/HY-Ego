#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <bcrypt.h>
#include <algorithm>
#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <new>
#include <optional>
#include <ranges>
#include <shared_mutex>
#include <stdexcept>
#include <string>
#include <system_error>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#pragma warning(disable : 4240)
#define private public
#include "../海中鱼巣/核心/服务.L1事实基座.h"
#undef private

namespace {
using namespace 海中鱼巣;
using 仓库 = L1事实基座仓库;

std::uint64_t 通过数量 = 0;
void 检查(bool 条件, const std::string& 名称) {
    if (!条件) throw std::runtime_error(名称);
    ++通过数量;
    std::cout << "PASS " << 名称 << '\n';
}

std::uint64_t 当前代次(const L1事实基座服务& 服务) {
    const auto 结果 = 服务.读取中性当前事实代次({});
    if (结果.状态 != L1中性读取状态::成功)
        throw std::runtime_error("current-generation-read");
    return 结果.事实代次;
}

稳定编码 映射编码(
    const std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>>& 映射,
    std::uint32_t 本地键) {
    for (const auto& [键, 编码] : 映射)
        if (键.值 == 本地键) return 编码;
    return {};
}

void 写旧格式(仓库::规范编码器& 出, const L1结构所有者事实& 值) {
    仓库::写(出, 值.所有者);
    仓库::写枚举(出, 值.范围种类);
    出.U64(值.创建事实代次);
    仓库::写可选(出, std::optional<std::uint64_t>{},
        [](auto& e, auto v) { e.U64(v); });
}

void 写旧格式(仓库::规范编码器& 出, const 节点事实& 值) {
    仓库::写(出, 值.编码);
    仓库::写枚举(出, 值.种类);
    仓库::写可选(出, 值.属性类型表示,
        [](auto& e, auto v) { 仓库::写枚举(e, v); });
    出.U64(值.创建事实代次);
    仓库::写可选(出, std::optional<std::uint64_t>{},
        [](auto& e, auto v) { e.U64(v); });
    仓库::写组(出, 值.当前属性,
        [](auto& e, const auto& v) { 仓库::写(e, v); });
    仓库::写(出, 值.写入所有者);
}

void 写旧格式(仓库::规范编码器& 出, const 关系事实& 值) {
    仓库::写(出, 值.编码);
    仓库::写(出, 值.源节点);
    仓库::写(出, 值.目标节点);
    仓库::写(出, 值.关系类型节点);
    出.I64(值.角色或顺序);
    出.U64(值.创建事实代次);
    仓库::写可选(出, std::optional<std::uint64_t>{},
        [](auto& e, auto v) { e.U64(v); });
    仓库::写(出, 值.写入所有者);
}

void 写旧格式(仓库::规范编码器& 出, const 值事实& 值) {
    仓库::写(出, 值.编码);
    仓库::写(出, 值.所属节点);
    仓库::写(出, 值.属性类型节点);
    仓库::写原始材料(出, 值.材料);
    仓库::写(出, 值.来源节点);
    出.U64(值.创建事实代次);
    仓库::写可选(出, std::optional<std::uint64_t>{},
        [](auto& e, auto v) { e.U64(v); });
    仓库::写(出, 值.写入所有者);
}

void 写旧格式(仓库::规范编码器& 出,
    const L1所有者范围建立结果& 值) {
    仓库::写枚举(出, 值.状态);
    出.U32(值.合同版本);
    仓库::写(出, 值.建立幂等身份);
    仓库::写可选(出, 值.所有者事实,
        [](auto& e, const auto& v) { 写旧格式(e, v); });
    出.U64(值.事实代次);
    出.布尔(值.是否形成内存权威发布);
    仓库::写枚举(出, 值.重试边界);
}

struct 种子 final {
    std::unique_ptr<L1事实基座运行包> 包;
    L1中性写集请求 中性请求;
    std::array<L1所有者范围建立请求, 3> 建立请求{};
    std::array<L1结构所有者身份, 3> 所有者{};
    L1所有者范围写集请求 owner请求;
    L1跨所有者原子事务请求 跨请求;
    L1三分区原子事务请求_v2 三分区请求;
    L1有限N分区原子事务请求_v3 有限N请求;
    std::vector<稳定编码> 当前节点;
    std::uint64_t 事实代次 = 0;
    std::uint64_t 下个编码 = 0;
};

L1所有者范围写集请求 普通节点写集(
    std::uint64_t G, std::uint64_t 幂等键) {
    L1所有者范围写集请求 请求;
    请求.期望事实代次 = G;
    请求.写入幂等身份 = {幂等键};
    请求.节点 = {{{1}, 节点种类::普通, std::nullopt}};
    return 请求;
}

L1跨所有者原子写集请求 跨节点写集(
    std::uint64_t G, std::uint64_t 幂等键) {
    L1跨所有者原子写集请求 请求;
    请求.期望事实代次 = G;
    请求.写入幂等身份 = {幂等键};
    请求.节点 = {{{1}, 节点种类::普通, std::nullopt}};
    return 请求;
}

L1三分区原子写集请求_v2 分区节点写集(
    std::uint64_t G, std::uint64_t 幂等键) {
    L1三分区原子写集请求_v2 请求;
    请求.期望事实代次 = G;
    请求.写入幂等身份 = {幂等键};
    请求.节点 = {{{1}, 节点种类::普通, std::nullopt}};
    return 请求;
}

种子 建立种子(std::uint32_t 格式版本) {
    种子 s;
    s.包 = std::make_unique<L1事实基座运行包>(建立L1事实基座运行包());
    auto& 服务 = s.包->读取服务();
    auto& 签发器 = s.包->所有者范围签发器();

    s.中性请求.期望事实代次 = 0;
    s.中性请求.幂等键 = {0x1001};
    s.中性请求.节点 = {{{1}, 节点种类::普通, std::nullopt}};
    const auto 中性结果 = 服务.提交中性写集(s.中性请求);
    检查(中性结果.状态 == L1中性写入状态::成功, "seed-neutral");

    std::array<L1所有者范围交付, 3> 交付;
    for (std::size_t i = 0; i != 3; ++i) {
        s.建立请求[i] = {L1所有者范围CRUD合同版本,
            {0x2001 + static_cast<std::uint64_t>(i)},
            L1所有者范围种类::独占结构范围};
        交付[i] = 签发器.建立所有者范围(s.建立请求[i]);
        检查(交付[i].建立结果.状态 == L1所有者范围管理状态::成功
                && 交付[i].建立结果.所有者事实 && 交付[i].写入端口,
            "seed-owner-" + std::to_string(i + 1));
        s.所有者[i] = 交付[i].建立结果.所有者事实->所有者;
    }

    s.owner请求 = 普通节点写集(当前代次(服务), 0x3001);
    const auto owner结果 = 交付[0].写入端口->提交所有者范围中性写集(s.owner请求);
    检查(owner结果.状态 == L1所有者范围写入状态::成功, "seed-owner-write");

    const auto 跨G = 当前代次(服务);
    s.跨请求.共同期望事实代次 = 跨G;
    s.跨请求.组合写入幂等身份 = {0x4001};
    s.跨请求.状态写集 = {L1跨所有者原子事务参与者序号::状态,
        s.所有者[0], 跨节点写集(跨G, 0x4101)};
    s.跨请求.动态写集 = {L1跨所有者原子事务参与者序号::动态,
        s.所有者[1], 跨节点写集(跨G, 0x4102)};
    const auto 跨结果 = 交付[0].写入端口->提交跨所有者原子事务(
        s.跨请求, *交付[1].写入端口);
    检查(跨结果.状态 == L1跨所有者原子事务状态::已提交, "seed-v1-atomic");

    if (格式版本 >= 2) {
        const auto G = 当前代次(服务);
        s.三分区请求.共同期望事实代次 = G;
        s.三分区请求.组合写入幂等身份 = {0x5001};
        for (std::uint8_t i = 1; i <= 3; ++i)
            s.三分区请求.参与者写集组.push_back(
                {{i}, s.所有者[i - 1], 分区节点写集(G, 0x5100 + i)});
        const auto 结果 = 交付[0].写入端口->提交三分区原子事务_v2(
            s.三分区请求, *交付[1].写入端口, *交付[2].写入端口);
        检查(结果.状态 == L1三分区原子事务状态_v2::已提交,
            "seed-v2-atomic");
    }

    if (格式版本 >= 3) {
        const auto G = 当前代次(服务);
        s.有限N请求.共同期望事实代次 = G;
        s.有限N请求.组合写入幂等身份 = {0x6001};
        for (std::uint8_t i = 1; i <= 2; ++i)
            s.有限N请求.参与者写集组.push_back(
                {{i}, s.所有者[i - 1], 分区节点写集(G, 0x6100 + i)});
        const std::vector<const L1所有者范围写端口*> 其余{
            &*交付[1].写入端口};
        const auto 结果 = 交付[0].写入端口->提交有限N分区原子事务_v3(
            s.有限N请求, 其余);
        检查(结果.状态 == L1有限N分区原子事务状态_v3::已提交,
            "seed-v3-atomic");
    }

    const auto& 状态 = s.包->状态_->仓库.状态_;
    s.事实代次 = 状态.事实代次;
    s.下个编码 = 状态.下个编码;
    for (const auto& [_, 节点] : 状态.当前节点) s.当前节点.push_back(节点.编码);
    std::sort(s.当前节点.begin(), s.当前节点.end());
    return s;
}

std::vector<std::uint8_t> 编码旧格式(
    const 仓库::状态& 值, std::uint32_t 版本) {
    仓库::规范编码器 出;
    出.U64(0x3150414E534C3148ULL); 出.U32(版本);
    出.U64(值.事实代次); 出.U64(值.下个编码);
    仓库::写有序表(出, 值.当前所有者,
        [](auto& e, const auto& v) { 写旧格式(e, v); });
    出.U64(0); // 旧历史owner
    仓库::写有序表(出, 值.当前节点,
        [](auto& e, const auto& v) { 写旧格式(e, v); });
    仓库::写有序表(出, 值.当前关系,
        [](auto& e, const auto& v) { 写旧格式(e, v); });
    仓库::写有序表(出, 值.当前值,
        [](auto& e, const auto& v) { 写旧格式(e, v); });
    出.U64(0); // 旧历史事实
    出.U64(0); // 旧永久占用
    出.U64(0); // 旧墓碑
    出.U64(0); // 旧物理清理账

    std::vector<std::uint64_t> 中性键;
    for (const auto& [k, _] : 值.中性幂等账)
        if (!仓库::是进程维护幂等键(k)) 中性键.push_back(k);
    std::sort(中性键.begin(), 中性键.end());
    出.U64(中性键.size());
    for (const auto k : 中性键) {
        const auto& v = 值.中性幂等账.at(k);
        出.U64(k); 仓库::写(出, v.首次规范化写集);
        出.U64(v.首次发布事实代次);
        仓库::写组(出, v.首次新编码映射,
            [](auto& e, const auto& x) { 仓库::写(e, x.first); 仓库::写(e, x.second); });
        仓库::写(出, v.首次结果);
    }
    仓库::写有序表(出, 值.所有者建立幂等账,
        [](auto& e, const auto& v) {
            仓库::写(e, v.首次请求);
            写旧格式(e, v.首次结果);
        });
    std::vector<std::uint64_t> owner;
    for (const auto& [k, _] : 值.所有者范围幂等账) owner.push_back(k);
    std::sort(owner.begin(), owner.end()); 出.U64(owner.size());
    for (const auto ok : owner) {
        出.U64(ok); const auto& inner = 值.所有者范围幂等账.at(ok);
        std::vector<std::uint64_t> keys;
        for (const auto& [k, _] : inner)
            if (!仓库::是进程维护幂等键(k)) keys.push_back(k);
        std::sort(keys.begin(), keys.end()); 出.U64(keys.size());
        for (const auto k : keys) {
            const auto& v = inner.at(k); 出.U64(k);
            仓库::写(出, v.首次规范化写集); 出.U64(v.首次发布事实代次);
            仓库::写组(出, v.首次新编码映射,
                [](auto& e, const auto& x) { 仓库::写(e, x.first); 仓库::写(e, x.second); });
            仓库::写(出, v.首次结果);
        }
    }
    std::vector<std::uint64_t> 跨键;
    for (const auto& [k, v] : 值.跨所有者原子事务幂等账)
        if (!仓库::是进程维护跨所有者原子事务(k, v)) 跨键.push_back(k);
    std::sort(跨键.begin(), 跨键.end()); 出.U64(跨键.size());
    for (const auto k : 跨键) {
        const auto& v = 值.跨所有者原子事务幂等账.at(k);
        出.U64(k); 仓库::写(出, v.请求); 仓库::写(出, v.结果);
    }
    if (版本 >= 2) {
        std::vector<std::uint64_t> 键组;
        for (const auto& [k, v] : 值.三分区原子事务幂等账_v2)
            if (!仓库::是进程维护三分区原子事务_v2(k, v)) 键组.push_back(k);
        std::sort(键组.begin(), 键组.end()); 出.U64(键组.size());
        for (const auto k : 键组) {
            const auto& v = 值.三分区原子事务幂等账_v2.at(k);
            出.U64(k); 仓库::写(出, v.请求); 仓库::写(出, v.结果);
        }
    }
    if (版本 >= 3) {
        std::vector<std::uint64_t> 键组;
        for (const auto& [k, v] : 值.有限N分区原子事务幂等账_v3)
            if (!仓库::是进程维护有限N分区原子事务_v3(k, v)) 键组.push_back(k);
        std::sort(键组.begin(), 键组.end()); 出.U64(键组.size());
        for (const auto k : 键组) {
            const auto& v = 值.有限N分区原子事务幂等账_v3.at(k);
            出.U64(k); 仓库::写有限N请求_v3(出, v.请求);
            仓库::写有限N结果_v3(出, v.结果);
        }
    }
    仓库::写可选(出, 值.旧共享所有者定位,
        [](auto& e, auto v) { 仓库::写(e, v); });
    return std::move(出.字节);
}

void 写旧快照(const std::filesystem::path& 根,
    const std::vector<std::uint8_t>& 载荷, std::uint64_t G) {
    std::filesystem::create_directories(根);
    std::array<std::uint8_t, 32> 摘要{};
    if (!仓库::SHA256(载荷, 摘要)) throw std::runtime_error("sha256");
    const auto 清单 = 仓库::编码清单(7, G, 1, 载荷.size(), 摘要);
    if (!仓库::写文件并刷新(根 / L"snapshot-a.bin", 载荷)
        || !仓库::写文件并刷新(根 / L"manifest.bin", 清单))
        throw std::runtime_error("write-old-fixture");
}

struct Windows句柄 final {
    HANDLE 值 = INVALID_HANDLE_VALUE;
    ~Windows句柄() {
        if (值 != INVALID_HANDLE_VALUE) CloseHandle(值);
    }
    Windows句柄(const Windows句柄&) = delete;
    Windows句柄& operator=(const Windows句柄&) = delete;
    Windows句柄() = default;
};

void 验证未知格式关闭(const std::filesystem::path& 根) {
    仓库::规范编码器 出;
    出.U64(0x3150414E534C3148ULL);
    出.U32(5);
    写旧快照(根, 出.字节, 1);

    const auto 结果 = 建立L1事实基座持久运行包_v1(
        {L1事实基座持久恢复合同版本_v1, 根});
    检查(结果.恢复.状态 == L1事实基座持久恢复状态_v1::格式不支持,
        "unknown-format-rejected");
    检查(!结果.运行包, "unknown-format-no-package");
}

void 验证损坏载荷关闭(const std::filesystem::path& 根) {
    auto 初建 = 建立L1事实基座持久运行包_v1(
        {L1事实基座持久恢复合同版本_v1, 根});
    检查(初建.恢复.状态 == L1事实基座持久恢复状态_v1::已建立空仓
            && 初建.运行包,
        "damaged-format4-seed-created");
    L1中性写集请求 写入;
    写入.期望事实代次 = 0;
    写入.幂等键 = {0xD401};
    写入.节点 = {{{1}, 节点种类::普通, std::nullopt}};
    检查(初建.运行包->读取服务().提交中性写集(写入).状态 ==
            L1中性写入状态::成功,
        "damaged-format4-seed-published");
    初建.运行包.reset();

    std::vector<std::uint8_t> 清单;
    if (!仓库::读文件(根 / L"manifest.bin", 清单))
        throw std::runtime_error("read-format4-manifest");
    std::uint64_t 序号{}, 代次{}, 长度{};
    std::uint8_t 活动槽编号{};
    std::array<std::uint8_t, 32> 摘要{};
    if (!仓库::解码清单(清单, 序号, 代次, 活动槽编号, 长度, 摘要))
        throw std::runtime_error("decode-format4-manifest");
    std::vector<std::uint8_t> 载荷;
    const auto 活动槽 = 根 /
        (活动槽编号 == 1 ? L"snapshot-a.bin" : L"snapshot-b.bin");
    if (!仓库::读文件(活动槽, 载荷) || 载荷.empty())
        throw std::runtime_error("read-format4-seed");
    载荷.back() ^= 0x01;
    if (!仓库::写文件并刷新(活动槽, 载荷))
        throw std::runtime_error("corrupt-format4-seed");

    const auto 结果 = 建立L1事实基座持久运行包_v1(
        {L1事实基座持久恢复合同版本_v1, 根});
    检查(结果.恢复.状态 == L1事实基座持久恢复状态_v1::摘要不一致,
        "damaged-format4-rejected");
    检查(!结果.运行包, "damaged-format4-no-package");
}

void 验证旧槽删除失败关闭与重试(const std::filesystem::path& 根) {
    auto s = 建立种子(1);
    const auto 旧载荷 = 编码旧格式(s.包->状态_->仓库.状态_, 1);
    s.包.reset();
    写旧快照(根, 旧载荷, s.事实代次);

    Windows句柄 锁定旧槽;
    锁定旧槽.值 = CreateFileW((根 / L"snapshot-a.bin").c_str(), GENERIC_READ,
        FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (锁定旧槽.值 == INVALID_HANDLE_VALUE)
        throw std::runtime_error("open-old-slot-without-delete-share");

    const auto 失败 = 建立L1事实基座持久运行包_v1(
        {L1事实基座持久恢复合同版本_v1, 根});
    检查(失败.恢复.状态 == L1事实基座持久恢复状态_v1::资源失败,
        "old-slot-delete-failure-rejected");
    检查(!失败.运行包, "old-slot-delete-failure-no-package");
    检查(std::filesystem::exists(根 / L"snapshot-a.bin")
            && std::filesystem::exists(根 / L"snapshot-b.bin"),
        "old-and-format4-slots-preserved-after-delete-failure");

    CloseHandle(锁定旧槽.值);
    锁定旧槽.值 = INVALID_HANDLE_VALUE;
    auto 重试 = 建立L1事实基座持久运行包_v1(
        {L1事实基座持久恢复合同版本_v1, 根});
    检查(重试.恢复.状态 == L1事实基座持久恢复状态_v1::已恢复
            && 重试.恢复.恢复见证
            && 重试.恢复.恢复见证->格式版本 == 4
            && 重试.运行包,
        "old-slot-delete-failure-retry-recovers-format4");
    检查(!std::filesystem::exists(根 / L"snapshot-a.bin")
            && std::filesystem::exists(根 / L"snapshot-b.bin"),
        "old-slot-delete-failure-retry-removes-old-slot");
}

void 验证一个版本(const std::filesystem::path& 根, std::uint32_t 版本) {
    auto s = 建立种子(版本);
    const auto 旧载荷 = 编码旧格式(s.包->状态_->仓库.状态_, 版本);
    s.包.reset();
    写旧快照(根, 旧载荷, s.事实代次);

    auto 迁移 = 建立L1事实基座持久运行包_v1(
        {L1事实基座持久恢复合同版本_v1, 根});
    std::cout << "INFO migration-v" << 版本
              << " status=" << static_cast<unsigned>(迁移.恢复.状态)
              << " witness-format="
              << (迁移.恢复.恢复见证
                      ? 迁移.恢复.恢复见证->格式版本
                      : 0U)
              << " package=" << static_cast<bool>(迁移.运行包) << '\n';
    检查(迁移.恢复.状态 == L1事实基座持久恢复状态_v1::已恢复
            && 迁移.恢复.恢复见证 && 迁移.恢复.恢复见证->格式版本 == 4
            && 迁移.运行包,
        "v" + std::to_string(版本) + "-migrated-to-format4");
    检查(!std::filesystem::exists(根 / L"snapshot-a.bin")
            && std::filesystem::exists(根 / L"snapshot-b.bin"),
        "v" + std::to_string(版本) + "-old-slot-removed");
    auto& 服务 = 迁移.运行包->读取服务();
    检查(当前代次(服务) == s.事实代次,
        "v" + std::to_string(版本) + "-generation-restored");
    for (const auto 编码 : s.当前节点) {
        const auto 读取 = 服务.读取中性当前节点({1, 编码});
        检查(读取.状态 == L1中性读取状态::成功 && 读取.事实,
            "v" + std::to_string(版本) + "-current-node-" + std::to_string(编码.值));
    }
    const auto 中性重放 = 服务.提交中性写集(s.中性请求);
    检查(中性重放.状态 == L1中性写入状态::精确重复,
        "v" + std::to_string(版本) + "-neutral-replay");
    const auto 中性账 = 服务.读取中性写入首次结果({1, s.中性请求.幂等键});
    检查(中性账.状态 == L1中性写入首次结果读取状态::已读取
            && 中性账.首次规范请求等价材料 == s.中性请求,
        "v" + std::to_string(版本) + "-neutral-first-material");

    std::array<L1所有者范围交付, 3> 交付;
    for (std::size_t i = 0; i != 3; ++i) {
        交付[i] = 迁移.运行包->所有者范围签发器().建立所有者范围(s.建立请求[i]);
        检查(交付[i].建立结果.状态 == L1所有者范围管理状态::精确重复
                && 交付[i].写入端口
                && 交付[i].写入端口->所有者身份() == s.所有者[i],
            "v" + std::to_string(版本) + "-owner-replay-" + std::to_string(i + 1));
    }
    const auto owner重放 = 交付[0].写入端口->提交所有者范围中性写集(s.owner请求);
    检查(owner重放.状态 == L1所有者范围写入状态::精确重复,
        "v" + std::to_string(版本) + "-owner-write-replay");
    const auto owner账 = 交付[0].写入端口->读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, s.owner请求.写入幂等身份});
    检查(owner账.状态 == L1所有者范围读取状态::成功
            && owner账.首次规范化写集 == s.owner请求,
        "v" + std::to_string(版本) + "-owner-first-material");
    const auto 跨重放 = 交付[0].写入端口->提交跨所有者原子事务(
        s.跨请求, *交付[1].写入端口);
    检查(跨重放.状态 == L1跨所有者原子事务状态::精确重复,
        "v" + std::to_string(版本) + "-v1-atomic-replay");
    if (版本 >= 2) {
        const auto 结果 = 交付[0].写入端口->提交三分区原子事务_v2(
            s.三分区请求, *交付[1].写入端口, *交付[2].写入端口);
        检查(结果.状态 == L1三分区原子事务状态_v2::精确重复,
            "v" + std::to_string(版本) + "-v2-atomic-replay");
    }
    if (版本 >= 3) {
        const std::vector<const L1所有者范围写端口*> 其余{
            &*交付[1].写入端口};
        const auto 结果 = 交付[0].写入端口->提交有限N分区原子事务_v3(
            s.有限N请求, 其余);
        检查(结果.状态 == L1有限N分区原子事务状态_v3::精确重复,
            "v" + std::to_string(版本) + "-v3-atomic-replay");
    }
    检查(当前代次(服务) == s.事实代次,
        "v" + std::to_string(版本) + "-replays-zero-change");

    L1中性写集请求 新增;
    新增.期望事实代次 = 当前代次(服务); 新增.幂等键 = {0x7000 + 版本};
    新增.节点 = {{{1}, 节点种类::普通, std::nullopt}};
    const auto 新增结果 = 服务.提交中性写集(新增);
    检查(新增结果.状态 == L1中性写入状态::成功
            && !新增结果.新编码映射.empty()
            && 新增结果.新编码映射.front().second.值 >= s.下个编码,
        "v" + std::to_string(版本) + "-high-water-preserved");
    交付 = {};
    迁移.运行包.reset();

    auto 二次恢复 = 建立L1事实基座持久运行包_v1(
        {L1事实基座持久恢复合同版本_v1, 根});
    检查(二次恢复.恢复.状态 == L1事实基座持久恢复状态_v1::已恢复
            && 二次恢复.恢复.恢复见证
            && 二次恢复.恢复.恢复见证->格式版本 == 4
            && 二次恢复.运行包,
        "v" + std::to_string(版本) + "-format4-second-recover");
}
} // namespace

int wmain(int 参数数量, wchar_t** 参数) {
    try {
        std::cout << std::unitbuf;
        if (参数数量 != 2) throw std::runtime_error("controlled-root-required");
        const std::filesystem::path 基根 = 参数[1];
        if (!基根.is_absolute()) throw std::runtime_error("controlled-root-not-absolute");
        for (std::uint32_t 版本 = 1; 版本 <= 3; ++版本)
            验证一个版本(基根 / (L"format" + std::to_wstring(版本)), 版本);
        验证未知格式关闭(基根 / L"unknown-format");
        验证损坏载荷关闭(基根 / L"damaged-format4");
        验证旧槽删除失败关闭与重试(基根 / L"old-slot-delete-failure");
        std::cout << "PASS total=" << 通过数量 << '\n';
        return 0;
    } catch (const std::exception& 错误) {
        std::cerr << "FAIL " << 错误.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "FAIL unexpected exception\n";
        return 2;
    }
}
