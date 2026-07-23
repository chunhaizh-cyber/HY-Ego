// 文件规则：候选调用图自检只形成静态 / 白名单证据，不写仓库、不接线、不改变候选结果。
module;

#include "节点直接P3合同.数据.h"
#include "节点直接切换映射.数据.h"

#include <cstdint>
#include <set>
#include <string>

export module 海中鱼巣.领域.自检.节点直接候选调用图;

export namespace 海中鱼巣 {

struct 节点直接候选调用图自检结果 final {
    std::uint32_t 记录总数 = 0;
    std::uint32_t 字段完整数 = 0;
    std::uint32_t 重复调用点数 = 0;
    std::uint32_t 重复目标数 = 0;
    std::uint32_t 旧域回退数 = 0;
    std::uint32_t 无最终动作数 = 0;
    std::uint32_t 提前生产可达数 = 0;

    bool 候选库存通过() const noexcept {
        return 记录总数 != 0
            && 记录总数 == 字段完整数
            && 重复调用点数 == 0
            && 重复目标数 == 0
            && 旧域回退数 == 0
            && 无最终动作数 == 0
            && 提前生产可达数 == 0;
    }
};

class 节点直接候选调用图自检 final {
public:
    explicit 节点直接候选调用图自检(
        const 节点直接切换映射库存& 库存)
        : 库存_(库存) {}

    节点直接候选调用图自检结果 运行节点直接候选调用图自检() const {
        节点直接候选调用图自检结果 结果;
        std::set<std::string> 调用点集合;
        std::set<std::string> 目标集合;
        for (const auto& 记录 : 库存_.记录组) {
            ++结果.记录总数;
            if (记录.字段完整()) {
                ++结果.字段完整数;
            }
            if (记录.最终动作.empty()) {
                ++结果.无最终动作数;
            }

            const std::string 调用点键 = 记录.调用点文件 + "\n"
                + 记录.调用点模块 + "\n" + 记录.调用点类 + "\n"
                + 记录.调用点函数;
            if (!调用点集合.insert(调用点键).second) {
                ++结果.重复调用点数;
            }

            const std::string 目标键 = 记录.目标模块 + "\n"
                + 记录.目标入口 + "\n" + 记录.目标完整签名;
            if (!目标集合.insert(目标键).second) {
                ++结果.重复目标数;
            }

            if (记录.目标模块.find("旧") != std::string::npos
                || 记录.目标入口.find("旧") != std::string::npos) {
                ++结果.旧域回退数;
            }
            if (记录.生产可达 != 节点直接切换映射可达状态::不可达) {
                ++结果.提前生产可达数;
            }
        }
        return 结果;
    }

private:
    const 节点直接切换映射库存& 库存_;
};

}
