module;

#include <cstdint>
#include <optional>
#include <variant>
#include <vector>

#define L2_STRUCTURE_COMMON_NO_INCLUDES

export module 海中鱼巣.领域.合同.L2结构公共;

export import 海中鱼巣.核心.合同.L1公共事实;
export import 海中鱼巣.核心.合同.L1所有者范围CRUD;

export {
#include "L2结构公共.数据.h"
}
