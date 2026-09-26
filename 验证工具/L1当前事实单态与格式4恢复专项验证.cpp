#include "../海中鱼巣/核心/服务.L1事实基座.h"

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {
using namespace 海中鱼巣;

std::uint64_t 通过数量 = 0;

void 检查(bool 条件, const char* 名称) {
    if (!条件) throw std::runtime_error(名称);
    ++通过数量;
    std::cout << "PASS " << 名称 << '\n';
}

std::uint64_t 当前代次(const L1事实基座服务& 服务) {
    const auto 结果 = 服务.读取中性当前事实代次({});
    检查(结果.状态 == L1中性读取状态::成功, "current-generation-read");
    return 结果.事实代次;
}

稳定编码 映射编码(const L1中性写入结果& 结果, std::uint32_t 本地键) {
    for (const auto& [键, 编码] : 结果.新编码映射)
        if (键.值 == 本地键) return 编码;
    return {};
}

L1事实基座持久运行包建立结果_v1 建立(const std::filesystem::path& 根) {
    return 建立L1事实基座持久运行包_v1({
        L1事实基座持久恢复合同版本_v1, 根});
}
} // namespace

int wmain(int 参数数量, wchar_t** 参数) {
    try {
        std::cout << std::unitbuf;
        if (参数数量 != 2) throw std::runtime_error("controlled-root-required");
        const std::filesystem::path 根 = 参数[1];
        if (!根.is_absolute()) throw std::runtime_error("controlled-root-not-absolute");

        L1中性写集请求 首次请求;
        首次请求.期望事实代次 = 0;
        首次请求.幂等键 = {0xC0110001ULL};
        首次请求.节点 = {{{1}, 节点种类::普通, std::nullopt},
                           {{2}, 节点种类::普通, std::nullopt}};

        L1中性写集请求 删除请求;
        稳定编码 保留节点{}, 删除节点{};
        std::uint64_t 删除后代次 = 0;

        {
            auto 建立结果 = 建立(根);
            检查(建立结果.恢复.状态 == L1事实基座持久恢复状态_v1::已建立空仓,
                 "format4-seed-empty-store");
            检查(static_cast<bool>(建立结果.运行包), "format4-seed-package-delivered");
            auto& 服务 = 建立结果.运行包->读取服务();

            const auto 首次结果 = 服务.提交中性写集(首次请求);
            检查(首次结果.状态 == L1中性写入状态::成功 &&
                     首次结果.是否形成内存权威发布,
                 "current-create-published");
            保留节点 = 映射编码(首次结果, 1);
            删除节点 = 映射编码(首次结果, 2);
            检查(有效(保留节点) && 有效(删除节点) && 保留节点 < 删除节点,
                 "stable-codes-monotonic");

            删除请求.期望事实代次 = 首次结果.事实代次;
            删除请求.幂等键 = {0xC0110002ULL};
            删除请求.退出事实 = {删除节点};
            const auto 删除结果 = 服务.提交中性写集(删除请求);
            检查(删除结果.状态 == L1中性写入状态::成功 &&
                     删除结果.是否形成内存权威发布,
                 "current-delete-published");
            删除后代次 = 删除结果.事实代次;
            const auto 删除后读取 = 服务.读取中性当前节点({1, 删除节点});
            检查(删除后读取.状态 == L1中性读取状态::未找到 && !删除后读取.事实,
                 "deleted-fact-not-found");

            const auto 删除重放 = 服务.提交中性写集(删除请求);
            检查(删除重放.状态 == L1中性写入状态::精确重复 &&
                     删除重放.事实代次 == 删除后代次,
                 "deleted-write-first-material-replay");
        }

        稳定编码 重启后节点{};
        {
            auto 恢复结果 = 建立(根);
            检查(恢复结果.恢复.状态 == L1事实基座持久恢复状态_v1::已恢复,
                 "format4-first-recover");
            检查(恢复结果.恢复.恢复见证 &&
                     恢复结果.恢复.恢复见证->格式版本 ==
                         L1事实基座持久快照格式版本_v1,
                 "format4-recovery-witness");
            检查(static_cast<bool>(恢复结果.运行包), "format4-recover-package-delivered");
            auto& 服务 = 恢复结果.运行包->读取服务();
            检查(当前代次(服务) == 删除后代次, "format4-generation-restored");
            const auto 保留读取 = 服务.读取中性当前节点({1, 保留节点});
            检查(保留读取.状态 == L1中性读取状态::成功 && 保留读取.事实,
                 "retained-fact-restored");
            const auto 删除读取 = 服务.读取中性当前节点({1, 删除节点});
            检查(删除读取.状态 == L1中性读取状态::未找到 && !删除读取.事实,
                 "deleted-fact-stays-absent-after-recover");

            const auto 首次重放 = 服务.提交中性写集(首次请求);
            检查(首次重放.状态 == L1中性写入状态::精确重复,
                 "create-first-material-replay-after-recover");
            const auto 首次账 = 服务.读取中性写入首次结果({1, 首次请求.幂等键});
            检查(首次账.状态 == L1中性写入首次结果读取状态::已读取 &&
                     首次账.首次规范请求等价材料 == 首次请求 &&
                     首次账.首次状态 == L1中性写入状态::成功,
                 "first-material-readback-after-recover");

            L1中性写集请求 新增请求;
            新增请求.期望事实代次 = 当前代次(服务);
            新增请求.幂等键 = {0xC0110003ULL};
            新增请求.节点 = {{{1}, 节点种类::普通, std::nullopt}};
            const auto 新增结果 = 服务.提交中性写集(新增请求);
            检查(新增结果.状态 == L1中性写入状态::成功,
                 "post-recover-create-published");
            重启后节点 = 映射编码(新增结果, 1);
            检查(重启后节点.值 > 删除节点.值, "deleted-high-code-not-reused");
        }

        {
            auto 二次恢复 = 建立(根);
            检查(二次恢复.恢复.状态 == L1事实基座持久恢复状态_v1::已恢复 &&
                     二次恢复.运行包,
                 "format4-second-recover");
            const auto 新节点读取 = 二次恢复.运行包->读取服务().读取中性当前节点(
                {1, 重启后节点});
            检查(新节点读取.状态 == L1中性读取状态::成功 && 新节点读取.事实,
                 "post-recover-node-restored");
        }

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
