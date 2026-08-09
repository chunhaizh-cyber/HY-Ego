module;

#include <cstdint>
#include <cstddef>
#include <optional>
#include <variant>
#include <vector>

#define L1_PUBLIC_FACT_NO_INCLUDES
#define L1_FACT_BASE_NO_INCLUDES

export module 海中鱼巣.核心.合同.L1事实基座;
export import 海中鱼巣.核心.合同.L1公共事实;

export {
#include "L1事实基座.数据.h"
}
