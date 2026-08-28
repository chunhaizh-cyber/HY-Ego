// 文件规则：本专项只使用 D:\TEMP 隔离根；不接触普通应用生产数据。
module;

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <cstdint>
#include <filesystem>
#include <iostream>
#include <string>
#include <system_error>

export module 海中鱼巣.端到端测试.L1事实基座持久恢复;

import 海中鱼巣.核心.服务.L1事实基座;

export namespace 海中鱼巣 {
int 运行L1事实基座持久恢复端到端测试() noexcept;
}

module :private;

namespace 海中鱼巣::L1事实基座持久恢复测试内部 {

struct 隔离根清理 final {
    std::filesystem::path 根;

    ~隔离根清理() {
        std::error_code 错误;
        std::filesystem::remove_all(根, 错误);
    }
};

int 失败(const char* 步骤) noexcept {
    std::cout << "[L1持久恢复测试] 失败：" << 步骤 << "\n";
    return 1;
}

} // namespace 海中鱼巣::L1事实基座持久恢复测试内部

namespace 海中鱼巣 {

int 运行L1事实基座持久恢复端到端测试() noexcept {
    using namespace L1事实基座持久恢复测试内部;
    try {
        const auto 隔离根 = std::filesystem::path{L"D:\\TEMP"}
            / L"海中鱼巣" / L"DATA-L1-PERSISTENT-RECOVERY-MANAGEMENT-PLANE"
            / (std::to_wstring(GetCurrentProcessId()) + L"-"
                + std::to_wstring(GetTickCount64()));
        隔离根清理 清理{隔离根};
        const L1事实基座持久存储配置_v1 配置{
            L1事实基座持久恢复合同版本_v1, 隔离根};
        const L1所有者范围建立幂等身份 建立身份{
            0x4C31'5052'5354'0001ULL};
        L1结构所有者身份 所有者{};

        {
            auto 首次 = 建立L1事实基座持久运行包_v1(配置);
            if (!首次.成功() || !首次.运行包
                || 首次.恢复.状态
                    != L1事实基座持久恢复状态_v1::已建立空仓
                || 首次.恢复.恢复见证)
                return 失败("空根建立");

            auto 交付 = 首次.运行包->所有者范围签发器().建立所有者范围(
                {L1所有者范围CRUD合同版本, 建立身份,
                    L1所有者范围种类::独占结构范围});
            if (交付.建立结果.状态 != L1所有者范围管理状态::成功
                || !交付.建立结果.所有者事实 || !交付.写入端口)
                return 失败("首次 owner 持久发布");
            所有者 = 交付.建立结果.所有者事实->所有者;
        }

        {
            auto 恢复 = 建立L1事实基座持久运行包_v1(配置);
            if (!恢复.成功() || !恢复.运行包
                || 恢复.恢复.状态 != L1事实基座持久恢复状态_v1::已恢复
                || !恢复.恢复.恢复见证)
                return 失败("第二会话隔离恢复");

            auto 重入 = 恢复.运行包->所有者范围签发器()
                .重新签发所有者范围写端口(
                    {L1所有者范围CRUD合同版本, 所有者, 建立身份});
            if (重入.重入结果.状态 != L1所有者范围管理状态::成功
                || !重入.写入端口)
                return 失败("恢复后重新签发写端口");

            const auto 读回 = 恢复.运行包->读取服务().读取当前结构所有者(
                {L1所有者范围CRUD合同版本, 所有者});
            if (读回.状态 != L1所有者范围读取状态::成功
                || !读回.所有者事实 || 读回.所有者事实->所有者 != 所有者
                || 读回.所有者事实->退出事实代次
                || 读回.读取事实代次
                    != 恢复.恢复.恢复见证->事实代次)
                return 失败("恢复后 owner 正式读回");
        }

        std::cout << "[L1持久恢复测试] 空根、owner发布、恢复与重新签发通过\n";
        return 0;
    } catch (...) {
        return 失败("未分类异常");
    }
}

} // namespace 海中鱼巣
