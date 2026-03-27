module;
#include <mutex>
#include <string>

export module 语言环境模块;

import 自然语言树模块;
import 因果信息模块;
import 特征值环境模块;
import 语素环境模块;
import 世界树环境模块;
import 特征类型定义模块;

export inline 自然语言树类 语言集{};

inline std::once_flag 基础数据初始化_once_{};
inline std::once_flag 世界骨架初始化_once_{};

export inline void 初始化基础数据环境(
    const std::string& 调用点 = "语言环境模块::初始化基础数据环境")
{
    std::call_once(基础数据初始化_once_, [调用点] {
        (void)调用点;
        特征值集.初始化();
        语素集.初始化();
        特征类型定义类::初始化特征类型定义模块_依赖语素();
        语言集.初始化();
    });
}

export inline void 初始化世界骨架环境(
    const std::string& 调用点 = "语言环境模块::初始化世界骨架环境")
{
    初始化基础数据环境(调用点 + "/基础数据");
    std::call_once(世界骨架初始化_once_, [调用点] {
        世界树.初始化(调用点 + "/世界树");
        世界树.初始化默认世界();
        因果集.初始化因果根();
    });
}
