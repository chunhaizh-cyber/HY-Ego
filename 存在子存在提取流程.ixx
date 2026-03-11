module;

#include <vector>
#include <unordered_map>
#include <memory>

import 世界树环境模块;
import 世界树模块;
import 基础数据类型模块;
import 存在模块;
import 存在提取类模块;
import 子存在提取模块;
import 三维场景管理类模块;

import <string>;
import <functional>;

export module 存在子存在提取流程模块;

export class 存在子存在提取流程类 {
public:
    struct 流程参数 {
        bool 启用子存在提取 = true;
        bool 仅在稳态轮廓变化时更新 = true;
        std::int32_t 最大子存在数量 = 24;
        子存在提取参数 子存在提取参数{};
        观测提取参数 子存在观测参数{};
    };

public:
    存在子存在提取流程类(流程参数 p = {}) : 参数_(std::move(p)) {}

    // 从父存在提取子存在并更新到内部世界
    bool 提取并更新子存在(存在节点类* 父存在, 点簇增强结果& 父点簇增强) {
        if (!父存在) return false;
        if (!参数_.启用子存在提取) return false;

        // 1. 从父点簇增强提取子点簇增强结果
        子存在提取类 提取器;
        auto 子点簇增强结果列表 = 提取器.从父点簇增强提取子点簇增强(父点簇增强, 参数_.子存在提取参数);

        if (子点簇增强结果列表.empty()) return false;

        // 2. 限制子存在数量
        if (子点簇增强结果列表.size() > (std::size_t)参数_.最大子存在数量) {
            // 按像素数排序，保留最大的N个
            std::sort(子点簇增强结果列表.begin(), 子点簇增强结果列表.end(),
                [](const 点簇增强结果& a, const 点簇增强结果& b) {
                    return a.簇.size() > b.簇.size();
                });
            子点簇增强结果列表.resize((std::size_t)参数_.最大子存在数量);
        }

        // 3. 将子点簇增强结果转换为存在观测
        存在提取类 存在提取器;
        std::vector<结构体_存在观测> 子存在观测列表;
        子存在观测列表.reserve(子点簇增强结果列表.size());

        for (const auto& 子点簇增强 : 子点簇增强结果列表) {
            结构体_存在观测 子存在观测;
            if (存在提取器.从点簇增强提取存在观测(子点簇增强, 子存在观测, 参数_.子存在观测参数)) {
                子存在观测列表.push_back(std::move(子存在观测));
            }
        }

        if (子存在观测列表.empty()) return false;

        // 4. 获取或创建父存在的内部世界场景
        auto* 内部世界场景 = 三维场景管理类::获取或创建内部世界(父存在);
        if (!内部世界场景) return false;

        // 5. 更新子存在到内部世界
        for (std::size_t i = 0; i < 子存在观测列表.size(); ++i) {
            const auto& 子存在观测 = 子存在观测列表[i];
            const auto& 子点簇增强 = 子点簇增强结果列表[i];

            // 生成唯一的PartKey（使用子点簇增强的哈希或索引）
            std::uint64_t partKey = i + 1; // 简单实现，实际可以使用更复杂的哈希

            // 在内部世界中查找或创建子存在
            auto* 子存在 = 三维场景管理类::获取或创建子存在_按PartKey(内部世界场景, partKey);
            if (!子存在) continue;

            // 更新子存在的观测信息
            三维场景管理类::更新存在_按观测(子存在, 子存在观测);

            // 可以在这里添加更多子存在特有的信息，如与父存在的关系等
        }

        return true;
    }

    // 设置流程参数
    void 设置参数(流程参数 p) {
        参数_ = std::move(p);
    }

    // 获取当前参数
    const 流程参数& 获取参数() const {
        return 参数_;
    }

private:
    流程参数 参数_;

    // 可以添加缓存来存储最近的父点簇增强结果，避免重复计算
    std::unordered_map<const 存在节点类*, 点簇增强结果> 最近父点簇增强缓存_;
};

// 全局工具函数
export inline bool 提取存在的子存在信息(
    存在节点类* 父存在,
    点簇增强结果& 父点簇增强,
    存在子存在提取流程类::流程参数 p = {}) {
    
    存在子存在提取流程类 流程(p);
    return 流程.提取并更新子存在(父存在, 父点簇增强);
}
