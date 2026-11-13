#pragma once

#include <string>

namespace 海中鱼巣 {

// 来自只读词典的值式义项材料；不依赖加载服务或执行身份。
struct 词典义项材料 final {
    std::string 记录键, 词头语种, 词面, 义项键, 汉语释义;
    std::string 例句原文, 例句汉译, 读音;
    std::string 来源页URL, 来源版本, 许可标识, 署名, 原始来源定位;
};

} // namespace 海中鱼巣
