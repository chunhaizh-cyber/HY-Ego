module;

#include <cstdint>
#include <optional>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#define L1_PUBLIC_FACT_NO_INCLUDES
#define L1_NEUTRAL_CRUD_NO_INCLUDES

export module 海中鱼巣.核心.合同.L1中性CRUD;
export import 海中鱼巣.核心.合同.L1公共事实;

export {
#include "L1中性CRUD.数据.h"
}
