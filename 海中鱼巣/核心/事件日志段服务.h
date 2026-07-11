// 文件规则：事件日志段只保存可校验的持久化候选；不得裁决运行期事实、恢复仓库或调用领域服务。
#pragma once

#include "日志系统.h"

#include <cstdint>
#include <filesystem>
#include <mutex>
#include <vector>

namespace 海中鱼巣 {

struct 事件段配置 {
    std::filesystem::path 日志根目录;
    std::uint64_t 段编号 = 0;
    std::uint64_t 最大段字节数 = 4ULL * 1024ULL * 1024ULL;
};

enum class 事件段状态 : std::uint32_t {
    未知 = 0,
    已追加 = 1,
    可读取 = 2,
    空段候选 = 3,
    已拒绝 = 4,
    段已满 = 5,
    既有段不可读 = 6,
    临时段冲突 = 7,
    内部写入异常 = 8
};

enum class 事件段拒绝原因 : std::uint32_t {
    无 = 0,
    配置无效 = 1,
    路径越界 = 2,
    材料无效 = 3,
    时间戳无效 = 4,
    调用方预填序号 = 5,
    字段超限 = 6,
    UTF8无效 = 7,
    魔数不匹配 = 8,
    未知格式版本 = 9,
    字节序不匹配 = 10,
    段编号不匹配 = 11,
    段长度不匹配 = 12,
    段头校验不一致 = 13,
    帧长度无效 = 14,
    负载版本不匹配 = 15,
    序号不连续 = 16,
    帧材料无效 = 17,
    帧校验不一致 = 18,
    段已满 = 19,
    临时段冲突 = 20,
    文件读取异常 = 21,
    目录创建异常 = 22,
    临时段写入异常 = 23,
    临时段读回异常 = 24,
    发布替换异常 = 25,
    发布后读回异常 = 26,
    内部不一致 = 27
};

struct 事件段追加结果 {
    事件段状态 状态 = 事件段状态::未知;
    事件段拒绝原因 拒绝原因 = 事件段拒绝原因::无;
    std::uint64_t 段编号 = 0;
    std::uint64_t 分配事件序号 = 0;
    std::uint64_t 段记录数 = 0;
    std::uint64_t 段字节数 = 0;
    bool 旧目标保持可读 = false;
    bool 临时段保留 = false;

    bool 成功() const {
        return 状态 == 事件段状态::已追加 && 拒绝原因 == 事件段拒绝原因::无;
    }
};

struct 事件段读取结果 {
    事件段状态 状态 = 事件段状态::未知;
    事件段拒绝原因 拒绝原因 = 事件段拒绝原因::无;
    std::uint64_t 段编号 = 0;
    std::uint32_t 格式版本 = 0;
    std::uint64_t 首序号 = 0;
    std::uint64_t 末序号 = 0;
    std::uint64_t 段字节数 = 0;
    std::vector<事件材料> 事件组;

    bool 成功() const {
        return (状态 == 事件段状态::可读取 || 状态 == 事件段状态::空段候选)
            && 拒绝原因 == 事件段拒绝原因::无;
    }
};

struct 事件段路径材料 {
    事件段拒绝原因 拒绝原因 = 事件段拒绝原因::无;
    std::filesystem::path 目标路径;
    std::filesystem::path 临时路径;

    bool 可用() const {
        return 拒绝原因 == 事件段拒绝原因::无
            && !目标路径.empty()
            && !临时路径.empty();
    }
};

class 事件日志段服务 {
public:
    static constexpr std::uint32_t 格式版本 = 1;
    static constexpr std::uint64_t 最大允许段字节数 = 4ULL * 1024ULL * 1024ULL;
    static constexpr std::uint32_t 最大入口名称字节数 = 512;
    static constexpr std::uint32_t 最大摘要字节数 = 4096;
    static constexpr std::uint32_t 最大帧字节数 = 8192;

    explicit 事件日志段服务(事件段配置 配置);

    事件日志段服务(const 事件日志段服务&) = delete;
    事件日志段服务& operator=(const 事件日志段服务&) = delete;

    事件段追加结果 追加结构事件(const 事件材料& 材料);
    事件段读取结果 读取事件段() const;
    事件段路径材料 读取段路径材料() const;

private:
    事件段追加结果 追加结构事件_已加锁(const 事件材料& 材料);
    事件段读取结果 读取事件段_已加锁() const;

    事件段配置 配置_;
    mutable std::mutex 服务锁_;
};

}
