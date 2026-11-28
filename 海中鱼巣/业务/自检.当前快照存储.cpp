#include "自检.当前快照存储.h"
#include "../核心/基础数据集.持久运行包.h"

#include <bit>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <limits>
#include <string_view>
#include <thread>

namespace 海中鱼巣 {
namespace {
using S = 基础持久状态;

基础快照映像 示例(bool 第二版) {
    基础快照映像 r;
    r.数据集身份[0] = 42;
    r.下一个编码 = 第二版 ? 12 : 11;
    r.节点组 = {{{1}}, {{2}}, {{3}}};
    r.外部关系组 = {{{4}, {1}, {2}, 基础外部关系类型::父子, -1}};
    r.字段关系组 = {
        {{5}, {1}, {3}, 稳定编码{2}},
        {{6}, {1}, {3}, 基础值{第二版 ? std::int64_t{30} : (std::numeric_limits<std::int64_t>::min)()}},
        {{7}, {1}, {3}, 基础值{std::vector<std::int64_t>{-1, 0, 1}}},
        {{8}, {1}, {3}, 基础值{std::vector<std::uint64_t>{0, (std::numeric_limits<std::uint64_t>::max)()}}},
        {{9}, {2}, {3}, 基础值{std::string{"A\0B", 3}}}
    };
    return r;
}

bool 验证编解码() {
    const auto 原文 = 示例(false);
    const auto [编码状态, 字节] = 编码基础快照(原文);
    if (编码状态 != S::已读取 || 字节.size() < 4 || 字节[0] != 1 || 字节[1] || 字节[2] || 字节[3])
        return false;
    const auto [状态, 映像] = 解码基础快照(字节);
    if (状态 != S::已读取 || 映像 != 原文) return false;
    auto 破坏 = 字节;
    破坏[0] = 2;
    if (解码基础快照(破坏).first != S::格式不支持) return false;
    破坏 = 字节;
    破坏.pop_back();
    if (解码基础快照(破坏).first != S::材料不完整) return false;
    auto 不合法 = 原文;
    不合法.节点组[1] = 不合法.节点组[0];
    if (编码基础快照(不合法).first != S::结构冲突) return false;
    不合法 = 原文;
    不合法.外部关系组[0].目标节点 = {10};
    if (编码基础快照(不合法).first != S::结构冲突) return false;
    不合法 = 原文;
    不合法.字段关系组[0].内容 = 稳定编码{10};
    if (编码基础快照(不合法).first != S::结构冲突) return false;
    return true;
}

bool 核对运行视图(const 基础持久运行包::视图& 视图, bool 第二版) {
    if (!视图) return false;
    const auto 映像 = 示例(第二版);
    const auto& 结构 = 视图->结构();
    return 视图->数据集身份() == 映像.数据集身份 && 视图->下一个编码() == 映像.下一个编码
        && 结构.查询节点({1}) == 映像.节点组[0]
        && 结构.查询源关系({1}, 基础外部关系类型::父子) == 映像.外部关系组
        && 结构.查询目标关系({2}, 基础外部关系类型::父子) == 映像.外部关系组
        && 结构.查询字段({1}, {3}) == std::vector<基础字段关系>(
            映像.字段关系组.begin(), 映像.字段关系组.begin() + 4)
        && 结构.查询字段({2}, {3}) == std::vector<基础字段关系>{映像.字段关系组.back()}
        && 结构.查询目标字段({2}, {3}) == std::vector<基础字段关系>{映像.字段关系组.front()}
        && 结构.有入向字段引用({2});
}

bool 核对组合视图(const 基础持久运行包::视图& 视图) {
    if (!视图 || 视图->下一个编码() != 13) return false;
    const auto& 结构 = 视图->结构();
    const auto 值 = 结构.查询字段关系({6});
    const auto 关联 = 结构.查询字段({11}, {3});
    return 值 && 值->内容 == 基础字段内容{基础值{std::int64_t{30}}}
        && 结构.查询节点({11}).has_value() && 关联.size() == 1
        && 关联.front().编码 == 稳定编码{12} && 关联.front().内容 == 基础字段内容{稳定编码{1}};
}

bool 验证持久组合(const std::shared_ptr<基础持久运行包>& 会话, std::string_view 操作) {
    using A = 基础节点组追加状态;
    auto& 运行 = *会话;
    const auto 前 = 运行.读取();
    基础数据集 入口, 旧活动;
    if (!有效(旧活动.新建节点()) || 基础持久运行包::绑定恢复权威(旧活动, 会话) != S::入口拒绝
        || 基础持久运行包::绑定恢复权威(入口, 会话) != S::已读取
        || 基础持久运行包::绑定恢复权威(入口, 会话) != S::已读取
        || 入口.查询节点({1}) != std::optional{基础节点{{1}}}
        || 入口.查询字段({1}, {3}) != 前->结构().查询字段({1}, {3})
        || 有效(入口.新建节点()) || 有效(入口.新建节点({1}))
        || 入口.原子建立节点({}).状态 != 基础结构操作状态::入口拒绝
        || 入口.原子建立节点组({基础组内新节点{}}).状态 != 基础结构操作状态::入口拒绝
        || 入口.删除字段({6}) || 入口.删除关系({4})
        || 有效(入口.添加字段节点({1}, {3}, {2}))
        || 入口.修改字段值({6}, 基础值{std::int64_t{999}})) return false;
    if (操作 == "pack-composite-read")
        return 核对组合视图(前) && 入口.查询节点({11}).has_value()
            && 入口.查询字段关系({6}) == 前->结构().查询字段关系({6});
    if (!核对运行视图(前, false)) return false;
    基础节点组追加请求 请求;
    请求.新节点组.push_back({{{{3}, 稳定编码{1}}}});
    const auto 核验 = 前->结构().查询节点组完整结构({{1}});
    if (核验.状态 != 基础结构操作状态::已完成 || 核验.节点组.size() != 1 || !核验.节点组.front().快照)
        return false;
    请求.预期联合节点组.push_back(*核验.节点组.front().快照);
    请求.替换字段组.push_back({{6}, 基础值{std::int64_t{30}}});
    const auto 封存 = 入口.封存原子追加片段(请求);
    if (封存.状态 != A::已完成 || !封存.片段) return false;
    const std::vector<基础封存追加片段> 片段组{*封存.片段};
    if (入口.原子追加节点组(请求).状态 != A::入口拒绝
        || 入口.原子追加封存片段组(片段组).状态 != A::入口拒绝 || 运行.读取() != 前)
        return false;
    if (操作 == "pack-composite-unknown") {
        const auto 不明 = 运行.提交追加(前, 片段组);
        if (不明.持久状态 != S::发布结果未知 || !不明.回执组.empty() || 运行.读取() != 前)
            return false;
        const auto 禁止 = 运行.提交追加(前, 片段组);
        return 禁止.持久状态 == S::发布结果未知 && 禁止.回执组.empty()
            && 运行.提交(前, 示例(false)) == S::发布结果未知;
    }
    if (操作 != "pack-composite-write") return false;
    基础数据集 外部;
    const auto 外部封存 = 外部.封存原子追加片段(请求);
    if (外部封存.状态 != A::已完成 || !外部封存.片段) return false;
    const auto 拒绝外部 = 运行.提交追加(前, {*外部封存.片段});
    if (拒绝外部.持久状态 != S::结构拒绝 || !拒绝外部.回执组.empty()) return false;

    请求.预期联合节点组.front().结构.字段组.back().内容 = 基础值{std::int64_t{88}};
    const auto 旧封存 = 运行.封存追加(请求);
    if (旧封存.状态 != A::已完成 || !旧封存.片段) return false;
    const auto 拒绝旧值 = 运行.提交追加(前, {*旧封存.片段});
    if (拒绝旧值.持久状态 != S::结构拒绝 || 拒绝旧值.结构状态 != A::预期不符
        || !拒绝旧值.回执组.empty() || 运行.读取() != 前) return false;

    const auto 成功 = 运行.提交追加(前, 片段组);
    if (成功.持久状态 != S::已提交 || 成功.结构状态 != A::已完成 || 成功.回执组.size() != 1
        || !成功.回执组.front().对应片段(*封存.片段)
        || 成功.回执组.front().新节点组() != std::vector<稳定编码>{{11}}
        || !核对组合视图(运行.读取()) || !核对运行视图(前, false)
        || !入口.查询节点({11}) || 入口.查询字段关系({6}) != 运行.读取()->结构().查询字段关系({6}))
        return false;
    const auto 旧视图 = 运行.提交追加(前, 片段组);
    return 旧视图.持久状态 == S::前提已变化 && 旧视图.回执组.empty();
}

bool 验证运行包(const std::filesystem::path& 根, std::string_view 操作, std::string_view 案例) {
    if (操作 == "pack-seed-unsupported") {
        auto [状态, 存储] = 基础持久存储::打开(根);
        if (状态 != S::空仓 || !存储) return false;
        auto 输入 = 示例(false);
        输入.技术材料 = {42};
        const auto [格式, 字节] = 编码基础快照(输入);
        return 格式 == S::已读取 && 存储->提交(字节) == S::已提交;
    }
    auto [状态, 运行] = 基础持久运行包::打开(根);
    if (操作 == "pack-reject-format") return 状态 == S::格式不支持 && !运行;
    if (操作 == "pack-reject-digest") return 状态 == S::摘要不一致 && !运行;
    if (!运行) return false;
    if (操作.starts_with("pack-composite-"))
        return 验证持久组合(std::shared_ptr<基础持久运行包>(std::move(运行)), 操作);
    const auto 前 = 运行->读取();
    if (操作 == "pack-empty") return 状态 == S::空仓 && !前;
    if (操作 == "pack-read") return 状态 == S::已读取 && 核对运行视图(前, 案例 == "2");
    if (操作 == "pack-write" && 案例 == "1")
        return 状态 == S::空仓 && !前 && 运行->提交(前, 示例(false)) == S::已提交
            && 核对运行视图(运行->读取(), false);
    if (状态 != S::已读取 || !核对运行视图(前, false)) return false;
    if (操作 == "pack-write" && 案例 == "2") {
        return 运行->提交(前, 示例(true)) == S::已提交
            && 核对运行视图(运行->读取(), true) && 核对运行视图(前, false)
            && 运行->提交(前, 示例(true)) == S::前提已变化;
    }
    if (操作 == "pack-fail") {
        return 运行->提交(前, 示例(true)) == S::资源失败
            && 运行->读取() == 前 && 核对运行视图(前, false);
    }
    if (操作 != "pack-invalid") return false;
    auto 改写 = 示例(false);
    改写.数据集身份[0] = 43;
    if (运行->提交(前, 改写) != S::结构冲突) return false;
    改写 = 示例(false);
    改写.下一个编码 = 10;
    if (运行->提交(前, 改写) != S::结构冲突) return false;
    改写 = 示例(false);
    改写.节点组.push_back({{10}});
    if (运行->提交(前, 改写) != S::结构冲突) return false;
    改写 = 示例(false);
    改写.技术材料 = {42};
    return 运行->提交(前, 改写) == S::格式不支持
        && 运行->提交({}, 示例(false)) == S::前提已变化
        && 运行->提交(前, 示例(false)) == S::无变化
        && 运行->读取() == 前;
}

bool 验证载体(const std::filesystem::path& 根, std::string_view 操作, std::string_view 案例) {
    const auto [状态, 会话] = 基础持久存储::打开(根);
    if (操作 == "reject") {
        if (会话) return false;
        if (案例 == "occupied") return 状态 == S::存储占用;
        if (案例 == "missing") return 状态 == S::材料不完整;
        if (案例 == "digest") return 状态 == S::摘要不一致;
        if (案例 == "format") return 状态 == S::格式不支持;
        return false;
    }
    if (!会话) return false;
    if ((操作 == "hold" || 操作 == "empty") && 案例 == "-") {
        if (状态 != S::空仓 || 会话->读取().first != S::空仓) return false;
        if (操作 == "hold") {
            std::puts("snapshot session held");
            std::fflush(stdout);
            std::this_thread::sleep_for(std::chrono::seconds(8));
        }
        return true;
    }
    if (案例 != "1" && 案例 != "2") return false;
    const bool 第二版 = 案例 == "2";
    const auto 映像 = 示例(第二版);
    const auto 读回 = [&](const 基础快照映像& 预期) {
        const auto [存储状态, 字节] = 会话->读取();
        if (存储状态 != S::已读取) return false;
        const auto [解码状态, 实际] = 解码基础快照(字节);
        return 解码状态 == S::已读取 && 实际 == 预期;
    };
    if (操作 == "read") return 状态 == S::已读取 && 读回(映像);
    if (操作 != "write") return false;
    if (第二版 ? (状态 != S::已读取 || !读回(示例(false))) : (状态 != S::空仓)) return false;
    const auto [编码状态, 字节] = 编码基础快照(映像);
    return 编码状态 == S::已读取 && 会话->提交(字节) == S::已提交 && 读回(映像);
}
}

bool 运行当前快照存储工程验证(const std::array<std::string, 3>& 参数) noexcept {
    try {
        const auto& [操作, 路径, 案例] = 参数;
        const std::filesystem::path 输入{std::u8string(路径.begin(), 路径.end())};
        if (!输入.is_absolute() || 输入 != 输入.lexically_normal()) return false;
        const auto 根 = std::filesystem::canonical(输入);
        const auto 临时 = std::filesystem::canonical(std::filesystem::temp_directory_path());
        if (根.parent_path() != 临时 || !根.filename().wstring().starts_with(L"HY-EGO-CURRENT-SNAPSHOT-VERIFY-"))
            return false;
        const bool 通过 = 操作.starts_with("pack-") ? 验证运行包(根, 操作, 案例)
            : (操作 == "codec" && 案例 == "-" ? 验证编解码() : 验证载体(根, 操作, 案例));
        std::puts(通过 ? "current snapshot verification passed" : "current snapshot verification failed");
        return 通过;
    } catch (...) {
        return false;
    }
}
}
