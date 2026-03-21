module;
#include <cstdint>
#include <unordered_map>

export module 特征比较策略模块;

import 基础数据类型模块;
import 主信息定义模块;

export struct 特征比较策略 {
    枚举_特征值类型 值类型 = 枚举_特征值类型::未定义;
    枚举_比较模式   模式 = 枚举_比较模式::有序标量; // 这里作为“距离模式”使用：海明/L1/L2...

    std::int64_t 权重 = 1;     // 命中打分加权（可选）
    std::int64_t 尺度 = 1;     // 距离缩放（dist *= 尺度），保持你原逻辑
    std::int32_t 期望维度 = 0; // 0=不校验；3=位置/尺寸/方向...

    // ★新增：距离归一化尺度
    // dist==0 => sim=1；dist>=归一化K => sim=0
    // 归一化K<=0 表示：不启用相似度（只允许严格相等/不等）
    std::int64_t 归一化K = 0;
};

struct 指针哈希 {
    size_t operator()(const void* p) const noexcept {
        return (size_t)(reinterpret_cast<std::uintptr_t>(p) >> 4);
    }
};

export class 特征比较策略表 {
public:
    void 注册(const 词性节点类* 特征类型, const 特征比较策略& s) {
        if (!特征类型) return;
        表[特征类型] = s;
    }

    const 特征比较策略* 查询(const 词性节点类* 特征类型) const {
        if (!特征类型) return nullptr;
        auto it = 表.find(特征类型);
        return (it == 表.end()) ? nullptr : &it->second;
    }

private:
    std::unordered_map<const 词性节点类*, 特征比较策略, 指针哈希> 表;
};

