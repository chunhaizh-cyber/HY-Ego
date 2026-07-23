#pragma once

// 文件规则：节点直接切换映射只保存 P4 施工库存，不承载运行期机器事实。
#include "节点直接P3合同.数据.h"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace 海中鱼巣 {

enum class 节点直接切换映射可达状态 : std::uint8_t {
    不可达 = 0,
    候选可达 = 1
};

struct 节点直接切换映射记录 {
    std::string 调用点文件;
    std::string 调用点模块;
    std::string 调用点类;
    std::string 调用点函数;
    std::string 旧目标模块;
    std::string 旧目标入口;
    std::string 目标模块;
    std::string 目标入口;
    std::string 目标完整签名;
    std::vector<std::string> 请求字段映射;
    std::vector<std::string> 结果状态映射;
    std::vector<std::string> 结构依赖;
    std::vector<std::string> 静态零命中模式;
    节点直接切换映射可达状态 生产可达 = 节点直接切换映射可达状态::不可达;
    std::string 最终动作;
    std::optional<std::string> 退回原因;

    bool 字段完整() const noexcept {
        return !调用点文件.empty()
            && !调用点模块.empty()
            && !调用点类.empty()
            && !调用点函数.empty()
            && !旧目标模块.empty()
            && !旧目标入口.empty()
            && !目标模块.empty()
            && !目标入口.empty()
            && !目标完整签名.empty()
            && !请求字段映射.empty()
            && !结果状态映射.empty()
            && !结构依赖.empty()
            && !静态零命中模式.empty()
            && !最终动作.empty();
    }
};

struct 节点直接切换映射库存 {
    std::vector<节点直接切换映射记录> 记录组;

    bool 全部字段完整() const noexcept {
        for (const auto& 记录 : 记录组) {
            if (!记录.字段完整()) return false;
        }
        return true;
    }
};

}
