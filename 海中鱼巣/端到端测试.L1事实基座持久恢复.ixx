// DATA-L1-FINAL-CLOSURE：只经公开持久运行包、签发器、写端口与读服务验证。
module;

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <bcrypt.h>

#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <span>
#include <string>
#include <system_error>
#include <variant>
#include <vector>

#pragma comment(lib, "bcrypt.lib")

export module 海中鱼巣.端到端测试.L1事实基座持久恢复;

import 海中鱼巣.核心.服务.L1事实基座;
import 海中鱼巣.核心.合同.L1事实基座;

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

稳定编码 映射(const L1所有者范围写入结果& 结果,
    std::uint32_t 本地键) noexcept {
    for (const auto& [键, 编码] : 结果.新编码映射)
        if (键.值 == 本地键) return 编码;
    return {};
}

bool 已提交(const L1所有者范围写入结果& 结果) noexcept {
    return 结果.状态 == L1所有者范围写入状态::成功
        && 结果.事实代次 != 0 && 结果.是否形成内存权威发布;
}

std::optional<std::vector<std::uint8_t>> 读文件(
    const std::filesystem::path& 路径) {
    std::ifstream 输入(路径, std::ios::binary);
    if (!输入) return std::nullopt;
    std::vector<std::uint8_t> 数据((std::istreambuf_iterator<char>(输入)), {});
    return 数据;
}

bool 写文件(const std::filesystem::path& 路径,
    const std::vector<std::uint8_t>& 数据) {
    std::ofstream 输出(路径, std::ios::binary | std::ios::trunc);
    if (!输出) return false;
    输出.write(reinterpret_cast<const char*>(数据.data()),
        static_cast<std::streamsize>(数据.size()));
    return static_cast<bool>(输出);
}

std::optional<std::array<std::uint8_t, 32>> SHA256(
    std::span<const std::uint8_t> 数据) noexcept {
    BCRYPT_ALG_HANDLE 算法 = nullptr;
    BCRYPT_HASH_HANDLE 哈希 = nullptr;
    std::vector<std::uint8_t> 对象;
    std::array<std::uint8_t, 32> 摘要{};
    const auto 收尾 = [&]() noexcept {
        if (哈希) BCryptDestroyHash(哈希);
        if (算法) BCryptCloseAlgorithmProvider(算法, 0);
    };
    DWORD 对象长度 = 0, 摘要长度 = 0, 返回长度 = 0;
    if (BCryptOpenAlgorithmProvider(&算法, BCRYPT_SHA256_ALGORITHM,
            nullptr, 0) < 0
        || BCryptGetProperty(算法, BCRYPT_OBJECT_LENGTH,
            reinterpret_cast<PUCHAR>(&对象长度), sizeof(对象长度),
            &返回长度, 0) < 0
        || BCryptGetProperty(算法, BCRYPT_HASH_LENGTH,
            reinterpret_cast<PUCHAR>(&摘要长度), sizeof(摘要长度),
            &返回长度, 0) < 0 || 摘要长度 != 摘要.size()) {
        收尾(); return std::nullopt;
    }
    对象.resize(对象长度);
    if (BCryptCreateHash(算法, &哈希, 对象.data(), 对象长度,
            nullptr, 0, 0) < 0
        || BCryptHashData(哈希, const_cast<PUCHAR>(数据.data()),
            static_cast<ULONG>(数据.size()), 0) < 0
        || BCryptFinishHash(哈希, 摘要.data(),
            static_cast<ULONG>(摘要.size()), 0) < 0) {
        收尾(); return std::nullopt;
    }
    收尾(); return 摘要;
}

void 写U32(std::vector<std::uint8_t>& 数据, std::size_t 位置,
    std::uint32_t 值) noexcept {
    for (std::size_t i = 0; i != 4; ++i)
        数据[位置 + i] = static_cast<std::uint8_t>(值 >> (i * 8));
}

void 写U64(std::vector<std::uint8_t>& 数据, std::size_t 位置,
    std::uint64_t 值) noexcept {
    for (std::size_t i = 0; i != 8; ++i)
        数据[位置 + i] = static_cast<std::uint8_t>(值 >> (i * 8));
}

std::optional<std::filesystem::path> 活动载荷路径(
    const std::filesystem::path& 根) {
    const auto 清单 = 读文件(根 / L"manifest.bin");
    if (!清单 || 清单->size() != 69 || ((*清单)[28] != 1 && (*清单)[28] != 2))
        return std::nullopt;
    return 根 / ((*清单)[28] == 1 ? L"snapshot-a.bin" : L"snapshot-b.bin");
}

bool 同步清单(const std::filesystem::path& 根) {
    auto 清单 = 读文件(根 / L"manifest.bin");
    const auto 载荷路径 = 活动载荷路径(根);
    if (!清单 || !载荷路径) return false;
    const auto 载荷 = 读文件(*载荷路径);
    if (!载荷) return false;
    const auto 摘要 = SHA256(*载荷);
    if (!摘要) return false;
    写U64(*清单, 29, 载荷->size());
    std::copy(摘要->begin(), 摘要->end(), 清单->begin() + 37);
    return 写文件(根 / L"manifest.bin", *清单);
}

bool 复制根(const std::filesystem::path& 源,
    const std::filesystem::path& 目标) {
    std::filesystem::create_directories(目标.parent_path());
    std::filesystem::copy(源, 目标, std::filesystem::copy_options::recursive);
    return true;
}

bool 非成功且无运行包(const L1事实基座持久运行包建立结果_v1& 结果,
    L1事实基座持久恢复状态_v1 状态) noexcept {
    return 结果.恢复.状态 == 状态 && !结果.运行包;
}

} // namespace 海中鱼巣::L1事实基座持久恢复测试内部

namespace 海中鱼巣 {

int 运行L1事实基座持久恢复端到端测试() noexcept {
    using namespace L1事实基座持久恢复测试内部;
    try {
        const auto 本轮根 = std::filesystem::path{L"D:\\TEMP"}
            / L"海中鱼巣" / L"DATA-L1-FINAL-CLOSURE" / L"recovery"
            / (std::to_wstring(GetCurrentProcessId()) + L"-"
                + std::to_wstring(GetTickCount64()));
        隔离根清理 清理本轮{本轮根};
        const auto 权威根 = 本轮根 / L"authoritative";
        const L1事实基座持久存储配置_v1 配置{
            L1事实基座持久恢复合同版本_v1, 权威根};
        const L1所有者范围建立幂等身份 建立身份{
            0x4c31'4643'4c53'0001ULL};
        constexpr std::uint64_t 首写键 = 0x4c31'4643'4c53'0101ULL;
        L1结构所有者身份 所有者{};
        稳定编码 A{}, B{}, RT{}, PT{}, R1{}, V1{}, R2{}, V2{}, C{}, R3{}, V3{};
        std::uint64_t G首写 = 0, G清理 = 0;

        {
            auto 首会话 = 建立L1事实基座持久运行包_v1(配置);
            if (!首会话.成功() || !首会话.运行包
                || 首会话.恢复.状态 != L1事实基座持久恢复状态_v1::已建立空仓)
                return 失败("空根建立");
            auto 交付 = 首会话.运行包->所有者范围签发器().建立所有者范围(
                {L1所有者范围CRUD合同版本, 建立身份,
                    L1所有者范围种类::独占结构范围});
            if (交付.建立结果.状态 != L1所有者范围管理状态::成功
                || !交付.建立结果.所有者事实 || !交付.写入端口)
                return 失败("owner建立");
            所有者 = 交付.建立结果.所有者事实->所有者;
            auto& 端口 = *交付.写入端口;

            L1所有者范围写集请求 首写;
            首写.期望事实代次 = 交付.建立结果.事实代次;
            首写.写入幂等身份 = {首写键};
            首写.节点 = {{{1}, 节点种类::普通, std::nullopt},
                {{2}, 节点种类::普通, std::nullopt},
                {{3}, 节点种类::普通, std::nullopt},
                {{4}, 节点种类::属性类型, L1所有者范围值表示种类::I64}};
            首写.关系 = {{{5}, L1所有者范围写集本地键{1},
                L1所有者范围写集本地键{2},
                L1所有者范围写集本地键{3}, 1}};
            首写.值 = {{{6}, L1所有者范围写集本地键{1},
                L1所有者范围写集本地键{4}, std::int64_t{41},
                L1所有者范围写集本地键{2}}};
            首写.属性槽变更 = {{L1所有者范围写集本地键{1},
                L1所有者范围写集本地键{4}, {6}}};
            const auto G1 = 端口.提交所有者范围中性写集(首写);
            if (!已提交(G1) || G1.新编码映射.size() != 6)
                return 失败("三类事实首写");
            G首写 = G1.事实代次;
            A=映射(G1,1); B=映射(G1,2); RT=映射(G1,3); PT=映射(G1,4);
            R1=映射(G1,5); V1=映射(G1,6);

            L1所有者范围写集请求 替换;
            替换.期望事实代次 = G1.事实代次;
            替换.写入幂等身份 = {0x4c31'4643'4c53'0102ULL};
            替换.关系 = {{{1}, A, B, RT, 2}};
            替换.值 = {{{2}, A, PT, std::int64_t{42}, B}};
            替换.属性槽变更 = {{A, PT, {2}}};
            替换.退出事实 = {R1, V1};
            const auto G2 = 端口.提交所有者范围中性写集(替换);
            if (!已提交(G2) || G2.新编码映射.size() != 2)
                return 失败("关系值替换");
            R2=映射(G2,1); V2=映射(G2,2);

            L1所有者范围写集请求 候选;
            候选.期望事实代次 = G2.事实代次;
            候选.写入幂等身份 = {0x4c31'4643'4c53'0103ULL};
            候选.节点 = {{{1}, 节点种类::普通, std::nullopt}};
            候选.关系 = {{{2}, L1所有者范围写集本地键{1},
                L1所有者范围写集本地键{1}, RT, 3}};
            候选.值 = {{{3}, L1所有者范围写集本地键{1}, PT,
                std::int64_t{43}, L1所有者范围写集本地键{1}}};
            const auto G3 = 端口.提交所有者范围中性写集(候选);
            if (!已提交(G3) || G3.新编码映射.size() != 3)
                return 失败("清理候选建立");
            C=映射(G3,1); R3=映射(G3,2); V3=映射(G3,3);

            L1所有者范围写集请求 退关系值;
            退关系值.期望事实代次=G3.事实代次;
            退关系值.写入幂等身份={0x4c31'4643'4c53'0104ULL};
            退关系值.退出事实={R3,V3};
            const auto G4=端口.提交所有者范围中性写集(退关系值);
            if(!已提交(G4)) return 失败("退出清理关系值");
            L1所有者范围写集请求 退节点;
            退节点.期望事实代次=G4.事实代次;
            退节点.写入幂等身份={0x4c31'4643'4c53'0105ULL};
            退节点.退出事实={C};
            const auto G5=端口.提交所有者范围中性写集(退节点);
            if(!已提交(G5)) return 失败("退出清理节点");

            L1物理清理请求 物理清理;
            物理清理.期望事实代次=G5.事实代次;
            物理清理.幂等身份={0x4c31'4643'4c53'0201ULL};
            物理清理.待清理事实身份组={{L1物理清理事实种类::节点,C},
                {L1物理清理事实种类::关系,R3},{L1物理清理事实种类::值,V3}};
            const auto 清理结果=首会话.运行包->读取服务().执行L1物理清理(物理清理);
            if(!清理结果.成功() || 清理结果.墓碑组.size()!=3)
                return 失败("三类事实物理清理");
            G清理=清理结果.物理清理事实代次;
        }

        {
            auto 恢复=建立L1事实基座持久运行包_v1(配置);
            if(!恢复.成功() || !恢复.运行包 || !恢复.恢复.恢复见证
                || 恢复.恢复.恢复见证->事实代次!=G清理)
                return 失败("第二会话恢复");
            auto 重入=恢复.运行包->所有者范围签发器().重新签发所有者范围写端口(
                {L1所有者范围CRUD合同版本,所有者,建立身份});
            if(重入.重入结果.状态!=L1所有者范围管理状态::成功 || !重入.写入端口)
                return 失败("端口重签发");
            const auto& 读=恢复.运行包->读取服务();
            for(auto 编码:{A,B,RT,PT}) {
                const auto 结果=读.读取所有者范围当前节点({L1所有者范围CRUD合同版本,编码});
                if(结果.状态!=L1所有者范围读取状态::成功 || !结果.事实)
                    return 失败("当前节点恢复");
            }
            const auto 当前R=读.读取所有者范围当前关系({L1所有者范围CRUD合同版本,R2});
            const auto 当前V=读.读取所有者范围当前值({L1所有者范围CRUD合同版本,V2});
            const auto 历史R=读.读取所有者范围历史事实({L1所有者范围CRUD合同版本,R1});
            const auto 历史V=读.读取所有者范围历史事实({L1所有者范围CRUD合同版本,V1});
            if(当前R.状态!=L1所有者范围读取状态::成功 || !当前R.事实
                || 当前V.状态!=L1所有者范围读取状态::成功 || !当前V.事实
                || 历史R.状态!=L1所有者范围读取状态::成功 || !历史R.事实
                || 历史V.状态!=L1所有者范围读取状态::成功 || !历史V.事实)
                return 失败("当前历史恢复");
            for(const auto [编码,种类]:std::vector<std::pair<稳定编码,L1所有者范围物理清理事实种类>>{
                    {C,L1所有者范围物理清理事实种类::节点},{R3,L1所有者范围物理清理事实种类::关系},{V3,L1所有者范围物理清理事实种类::值}}) {
                const auto 结果=读.读取所有者范围历史事实({L1所有者范围CRUD合同版本,编码});
                if(结果.状态!=L1所有者范围读取状态::历史材料已清理 || !结果.物理清理墓碑
                    || 结果.物理清理墓碑->事实种类!=种类 || 结果.物理清理墓碑->物理清理事实代次!=G清理)
                    return 失败("三类墓碑恢复");
            }
            const auto 源组=读.读取所有者范围当前源关系组({L1所有者范围CRUD合同版本,A,RT});
            const auto 目标组=读.读取所有者范围当前目标关系组({L1所有者范围CRUD合同版本,B,RT});
            const auto 历史关系组=读.读取所有者范围历史关系组({L1所有者范围CRUD合同版本,L1所有者范围关系端点方向::源,A,RT,G首写});
            const auto 历史属性组=读.读取所有者范围历史属性值组({L1所有者范围CRUD合同版本,A,G首写});
            if(源组.状态!=L1所有者范围读取状态::成功 || 源组.关系组.size()!=1
                || 目标组.状态!=L1所有者范围读取状态::成功 || 目标组.关系组.size()!=1
                || 历史关系组.状态!=L1所有者范围读取状态::成功 || 历史关系组.关系组.size()!=1
                || 历史属性组.状态!=L1所有者范围读取状态::成功 || 历史属性组.属性值组.size()!=1)
                return 失败("关系与属性历史组恢复");

            L1所有者范围一致当前读取请求 一致;
            一致.期望事实代次=G清理; 一致.所有者={所有者};
            一致.节点={A,B,RT,PT}; 一致.关系={R2}; 一致.值={V2};
            一致.属性值={{A,PT}}; 一致.源关系组={{A,RT}}; 一致.目标关系组={{B,RT}};
            const auto 投影=读.尝试读取所有者范围一致当前投影(一致);
            if(投影.状态!=L1所有者范围一致当前读取状态::成功 || 投影.读取事实代次!=G清理
                || 投影.节点.size()!=4 || 投影.关系.size()!=1 || 投影.值.size()!=1 || 投影.属性值.size()!=1)
                return 失败("一致投影恢复");
            const auto 首次=重入.写入端口->读取首次写入材料({L1所有者范围首次写入读取合同版本,{首写键}});
            if(首次.状态!=L1所有者范围读取状态::成功 || !首次.首次规范化写集
                || 重入.写入端口->提交所有者范围中性写集(*首次.首次规范化写集).状态!=L1所有者范围写入状态::精确重复)
                return 失败("首次材料与重放");
            L1所有者范围写集请求 后继;
            后继.期望事实代次=G清理; 后继.写入幂等身份={0x4c31'4643'4c53'0106ULL};
            后继.节点={{{1},节点种类::普通,std::nullopt}};
            const auto G后继=重入.写入端口->提交所有者范围中性写集(后继);
            const auto 新编码=映射(G后继,1);
            if(!已提交(G后继) || G后继.事实代次!=G清理+1 || !有效(新编码)
                || 新编码==C || 新编码==R3 || 新编码==V3)
                return 失败("后继代次与编码不复用");
        }

        const auto 验证失败=[&](const wchar_t* 名,
            L1事实基座持久恢复状态_v1 预期,
            const auto& 变更) {
            const auto 根=本轮根/名;
            复制根(权威根,根); 变更(根);
            const auto 结果=建立L1事实基座持久运行包_v1({L1事实基座持久恢复合同版本_v1,根});
            return 非成功且无运行包(结果,预期);
        };
        if(!验证失败(L"missing-manifest",L1事实基座持久恢复状态_v1::材料不完整,[](const auto& 根){std::filesystem::remove(根/L"manifest.bin");}))
            return 失败("清单缺失分账");
        if(!验证失败(L"missing-slot",L1事实基座持久恢复状态_v1::材料不完整,[](const auto& 根){const auto p=活动载荷路径(根);if(p)std::filesystem::remove(*p);}))
            return 失败("活动槽缺失分账");
        if(!验证失败(L"length",L1事实基座持久恢复状态_v1::摘要不一致,[](const auto& 根){auto b=读文件(根/L"manifest.bin");if(b){写U64(*b,29,0xffff); 写文件(根/L"manifest.bin",*b);}}))
            return 失败("长度分账");
        if(!验证失败(L"sha",L1事实基座持久恢复状态_v1::摘要不一致,[](const auto& 根){const auto p=活动载荷路径(根);if(p){auto b=读文件(*p);if(b&&!b->empty()){(*b)[0]^=1;写文件(*p,*b);}}}))
            return 失败("SHA分账");
        if(!验证失败(L"unknown-version",L1事实基座持久恢复状态_v1::格式不支持,[](const auto& 根){const auto p=活动载荷路径(根);if(p){auto b=读文件(*p);if(b&&b->size()>12){写U32(*b,8,99);写文件(*p,*b);同步清单(根);}}}))
            return 失败("未知载荷版本分账");
        if(!验证失败(L"unknown-tag",L1事实基座持久恢复状态_v1::编码或所有者冲突,[](const auto& 根){const auto p=活动载荷路径(根);if(p){auto b=读文件(*p);if(b&&!b->empty()){b->back()=0xff;写文件(*p,*b);同步清单(根);}}}))
            return 失败("已知格式未知标签分账");
        {
            auto 占用=建立L1事实基座持久运行包_v1(配置);
            auto 第二=建立L1事实基座持久运行包_v1(配置);
            if(!占用.成功() || !非成功且无运行包(第二,L1事实基座持久恢复状态_v1::存储占用))
                return 失败("存储占用分账");
        }
        {
            const auto 根=本轮根/L"inactive-slot"; 复制根(权威根,根);
            const auto 活动=活动载荷路径(根);
            const auto 非活动=根/(活动&&活动->filename()==L"snapshot-a.bin"?L"snapshot-b.bin":L"snapshot-a.bin");
            auto b=读文件(非活动); if(!b||b->empty()) return 失败("非活动槽准备");
            (*b)[0]^=0xff; 写文件(非活动,*b);
            const auto 结果=建立L1事实基座持久运行包_v1({L1事实基座持久恢复合同版本_v1,根});
            if(!结果.成功() || !结果.运行包) return 失败("只采用清单活动槽");
        }
        std::cout << "[L1持久恢复测试] 三类事实、当前/历史/墓碑、组/一致投影、幂等与 fail-closed 通过\n";
        return 0;
    } catch(const std::exception& e) {
        std::cout << "[L1持久恢复测试] 异常：" << e.what() << "\n";
        return 1;
    } catch(...) { return 失败("未分类异常"); }
}

} // namespace 海中鱼巣
