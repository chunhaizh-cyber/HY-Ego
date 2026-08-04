module;

#include <cstdint>
#include <variant>
#include <vector>

#define L1_PUBLIC_FACT_NO_INCLUDES
#define L1_FACT_BASE_NO_INCLUDES
#define L1_REBUILDABLE_INDEX_NO_INCLUDES

export module 海中鱼巣.核心.合同.L1可重建索引;
export import 海中鱼巣.核心.合同.L1事实基座;

export {
#include "L1可重建索引.数据.h"
}
