// 文件规则：本模块只承载节点直接类型化值的统一冻结读取值式合同与安全未实现访问器。
module;

#include "节点直接结构合同.数据.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

export module 海中鱼巣.核心.冻结.节点直接类型化值;

import 海中鱼巣.核心.执行器.节点直接身份结构写入;
import 海中鱼巣.核心.仓库.节点直接类型化值;

export namespace 海中鱼巣 {

enum class 节点直接类型化值冻结读取状态 : std::uint8_t {
    已形成 = 1,
    入口拒绝 = 2,
    许可竞争 = 3,
    资源失败 = 4,
    未实现 = 5,
    内部不一致 = 6
};

struct 节点直接类型化值冻结副本 final {
    节点直接统一冻结见证 见证;
    std::uint64_t 结构版本 = 0;
    std::uint64_t 记录数量 = 0;
    std::vector<类型化值读回> 记录组;
};

struct 节点直接类型化值冻结读取结果 final {
    节点直接类型化值冻结读取状态 状态 =
        节点直接类型化值冻结读取状态::未实现;
    std::optional<节点直接类型化值冻结副本> 副本;
};

struct 节点直接类型化值结构观察 final {
    节点直接统一冻结见证 见证;
    std::uint64_t 结构版本 = 0;
    std::uint64_t 记录数量 = 0;
};

struct 节点直接类型化值结构观察结果 final {
    节点直接类型化值冻结读取状态 状态 =
        节点直接类型化值冻结读取状态::未实现;
    std::optional<节点直接类型化值结构观察> 观察;
};

class 节点直接类型化值冻结只读访问器 final {
public:
    explicit 节点直接类型化值冻结只读访问器(
        const 节点直接类型化值仓库& 仓) noexcept
        : 仓_(std::addressof(仓)) {
    }

    节点直接类型化值冻结读取结果 复制全部已发布记录(
        const 节点直接统一冻结许可&) const {
        return {};
    }

    节点直接类型化值结构观察结果 观察已发布结构(
        const 节点直接统一冻结许可&) const {
        return {};
    }

private:
    const 节点直接类型化值仓库* 仓_ = nullptr;
};

} // namespace 海中鱼巣
