// 特征值模块.ixx
module;

#include <afx.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <mutex>
#include <algorithm>
#include <cstdint>
#include <cmath>

module 特征值模块;

import 基础数据类型模块;
import 模板模块;
import 主信息定义模块;



    uint64_t 特征值类::计算矢量压缩哈希(const std::vector<int64_t>& 原始矢量, int 目标层级)
    {
        if (原始矢量.empty()) return 0;

        const int 边长 = 8 << 目标层级;           // 8、16、32 … 2048
        const int 总格子数 = 边长 * 边长;
        const double 缩放比例 = static_cast<double>(原始矢量.size()) / 总格子数;

        uint64_t 哈希 = 0;

        for (int i = 0; i < 总格子数 && i < 64; ++i) // 只取前64位就足够区分
        {
            int 源索引 = static_cast<int>(i * 缩放比例);
            if (源索引 < static_cast<int>(原始矢量.size()) && 原始矢量[源索引] != 0)
            {
                哈希 |= (uint64_t(1) << (i % 64));
            }
        }
        return 哈希;
    }

    
    特征值节点类* 特征值类::创建并登记节点(特征值基类* 新主信息)
    {
      

        // 让链表模板负责主键生成和插入
        特征值节点类* 新节点 = 添加节点(根指针, 新主信息);
        if (!新节点) return nullptr;

        // 根据实际类型登记到对应索引
        if (auto* 字符串信息 = dynamic_cast<字符特征值主信息类*>(新主信息))
        {
            std::string 键 = 字符串信息->值;
            // 统一转小写、去除空格
            std::transform(键.begin(), 键.end(), 键.begin(),
                [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            键.erase(std::remove_if(键.begin(), 键.end(), [](unsigned char c) { return std::isspace(c); }), 键.end());

            文本特征值索引[键] = 新节点;
        }
        else if (auto* 矢量信息 = dynamic_cast<矢量特征值主信息类*>(新主信息))
        {
            const auto& 矢量值 = 矢量信息->值;

            for (int 层 = 0; 层 <= 最大层级; ++层)
            {
                uint64_t 哈希 = 计算矢量压缩哈希(矢量值, 层);
                矢量金字塔索引[层][哈希] = 新节点;  // 同哈希后面会被覆盖，只保留最新（实际可用 vector 存多个）
            }
        }
        // 标量特征值数量极少，不建额外索引

        return 新节点;
    }

    特征值节点类* 特征值类::查找标量特征值(语素节点类* 单位指针, int64_t 数值) const
    {
        std::lock_guard<std::mutex> 锁(互斥锁);

        // 线性遍历查找（标量节点很少，几百个以内完全无压力）
        for (特征值节点类* 当前 = 根指针->下; 当前 != 根指针; 当前 = 当前->下)
        {
            if (auto* 信息 = dynamic_cast<标量特征值主信息类*>(当前->主信息))
            {
                if (信息->值 == 数值 && 信息->单位 == 单位指针)
                    return 当前;
            }
        }

        // 没找到就返回 nullptr，不会创建新节点
        return nullptr;
    }

    特征值节点类* 特征值类::获取或创建标量特征值(语素节点类* 单位指针, int64_t 数值)
    {
        std::lock_guard<std::mutex> 锁(互斥锁);

        // 线性遍历查找（标量节点很少，几百个以内完全无压力）
        for (特征值节点类* 当前 = 根指针->下; 当前 != 根指针; 当前 = 当前->下)
        {
            if (auto* 信息 = dynamic_cast<标量特征值主信息类*>(当前->主信息))
            {
                if (信息->值 == 数值 && 信息->单位 == 单位指针)
                    return 当前;
            }
        }

        // 不存在 → 新建
        return 创建并登记节点(new 标量特征值主信息类(单位指针, 数值));
    }

    特征值节点类* 特征值类::查找字符串特征值(const std::string& 原始文本) const
    {
        if (原始文本.empty()) return nullptr;

        std::string 键 = 原始文本;
        std::transform(键.begin(), 键.end(), 键.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        键.erase(
            std::remove_if(键.begin(), 键.end(),
                [](unsigned char c) { return std::isspace(c); }),
            键.end()
        );

        std::lock_guard<std::mutex> 锁(互斥锁);
        auto it = 文本特征值索引.find(键);
        if (it != 文本特征值索引.end())
            return it->second;

        return nullptr;
    }

    特征值节点类* 特征值类::获取或创建字符串特征值(const std::string& 原始文本)
    {
        if (原始文本.empty()) return nullptr;

        std::string 键 = 原始文本;
        std::transform(键.begin(), 键.end(), 键.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        键.erase(std::remove_if(键.begin(), 键.end(), [](unsigned char c) { return std::isspace(c); }), 键.end());

        {
            std::lock_guard<std::mutex> 锁(互斥锁);
            auto 查找结果 = 文本特征值索引.find(键);
            if (查找结果 != 文本特征值索引.end())
                return 查找结果->second;
        }

        // 不存在 → 新建
        return 创建并登记节点(new 字符特征值主信息类(原始文本));
    }

    特征值节点类* 特征值类::查找矢量特征值(const std::vector<int64_t>& 矢量值) const
    {
        if (矢量值.empty()) return nullptr;

        // 先计算最高层级的哈希
        uint64_t 哈希 = 计算矢量压缩哈希(矢量值, 最大层级);

        std::lock_guard<std::mutex> 锁(互斥锁);
        auto it = 矢量金字塔索引[最大层级].find(哈希);
        if (it == 矢量金字塔索引[最大层级].end())
            return nullptr;

        特征值节点类* 候选 = it->second;
        if (auto* 信息 = dynamic_cast<矢量特征值主信息类*>(候选->主信息))
        {
            // 为防止哈希碰撞，再做一次逐元素精确比对
            if (信息->值 == 矢量值)
                return 候选;
        }

        return nullptr;
    }

    特征值节点类* 特征值类::获取或创建矢量特征值(const std::vector<int64_t>& 矢量值)
    {
        if (矢量值.empty()) return nullptr;

        // 使用最粗糙的一层（层级7 = 2048×2048）做快速过滤
        uint64_t 粗哈希 = 计算矢量压缩哈希(矢量值, 最大层级);

        特征值节点类* 候选节点 = nullptr;

        {
            std::lock_guard<std::mutex> 锁(互斥锁);
            auto 查找结果 = 矢量金字塔索引[最大层级].find(粗哈希);
            if (查找结果 != 矢量金字塔索引[最大层级].end())
                候选节点 = 查找结果->second;
        }

        // 精确比对
        if (候选节点)
        {
            if (auto* 信息 = dynamic_cast<矢量特征值主信息类*>(候选节点->主信息))
            {
                if (信息->值 == 矢量值)
                    return 候选节点;   // 完全相等，直接返回
            }
        }

        // 真的没有 → 新建
        return 创建并登记节点(new 矢量特征值主信息类(矢量值));
    }

    
    特征值节点类* 特征值类::获取或创建矢量特征值(int64_t 单值)
    {
		std::vector<int64_t> 单值矢量 = { 单值 };
        return 获取或创建矢量特征值(单值矢量);
    }

   
    特征值类::相似矢量结果 特征值类::查找最相似矢量(const std::vector<int64_t>& 目标矢量, int 使用层级) const
    {
        相似矢量结果 最佳;
        最佳.相似度 = -1.0;

        if (目标矢量.empty() || 使用层级 < 0 || 使用层级 > 最大层级)
            return 最佳;

        uint64_t 哈希 = 计算矢量压缩哈希(目标矢量, 使用层级);

        std::lock_guard<std::mutex> 锁(互斥锁);
        auto 查找结果 = 矢量金字塔索引[使用层级].find(哈希);
        if (查找结果 == 矢量金字塔索引[使用层级].end())
            return 最佳;

        特征值节点类* 候选 = 查找结果->second;
        if (auto* 信息 = dynamic_cast<矢量特征值主信息类*>(候选->主信息)) {
            最佳.相似度 = 计算矢量相似度(目标矢量, 信息->值);
            最佳.节点 = 候选;
        }

        return 最佳;
    }

    // impl.cpp（模块实现）
   
      


    void 特征值类::删除特征值节点(特征值节点类* 待删除节点)
    {
        if (!待删除节点 || 待删除节点 == 根指针) return;

        std::lock_guard<std::mutex> 锁(互斥锁);

        // 从文本索引移除
        if (auto* 字符串信息 = dynamic_cast<字符特征值主信息类*>(待删除节点->主信息))
        {
            std::string 键 = 字符串信息->值;
            std::transform(键.begin(), 键.end(), 键.begin(),
                [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            键.erase(std::remove_if(键.begin(), 键.end(), [](unsigned char c) { return std::isspace(c); }), 键.end());
            文本特征值索引.erase(键);
        }
        // 从矢量金字塔索引移除
        else if (auto* 矢量信息 = dynamic_cast<矢量特征值主信息类*>(待删除节点->主信息))
        {
            const auto& 矢量 = 矢量信息->值;
            for (int 层 = 0; 层 <= 最大层级; ++层)
            {
                uint64_t 哈希 = 计算矢量压缩哈希(矢量, 层);
                矢量金字塔索引[层].erase(哈希);
            }
        }

        // 交给链表模板统一释放内存
        删除节点(待删除节点);
    }
    void 特征值类::重建所有索引()
    {
        std::lock_guard<std::mutex> 锁(互斥锁);

        文本特征值索引.clear();
        for (int 层 = 0; 层 <= 最大层级; ++层)
            矢量金字塔索引[层].clear();

        // 从链表重新扫描一遍
        for (特征值节点类* 当前 = 根指针->下; 当前 != 根指针; 当前 = 当前->下)
        {
            auto* 主 = 当前->主信息;
            if (!主) continue;

            if (auto* 字符串信息 = dynamic_cast<字符特征值主信息类*>(主))
            {
                std::string 键 = 字符串信息->值;
                std::transform(键.begin(), 键.end(), 键.begin(),
                    [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
                键.erase(std::remove_if(键.begin(), 键.end(),
                    [](unsigned char c) { return std::isspace(c); }), 键.end());

                文本特征值索引[键] = 当前;
            }
            else if (auto* 矢量信息 = dynamic_cast<矢量特征值主信息类*>(主))
            {
                const auto& 矢量 = 矢量信息->值;
                for (int 层 = 0; 层 <= 最大层级; ++层)
                {
                    uint64_t 哈希 = 计算矢量压缩哈希(矢量, 层);
                    矢量金字塔索引[层][哈希] = 当前;
                }
            }
            // 标量目前仍不建立额外索引
        }
    }
    double 特征值类::计算矢量相似度(
        const std::vector<int64_t>& A,
        const std::vector<int64_t>& B)
    {
        if (A.empty() || B.empty()) return -1.0;

        size_t n = (A.size() < B.size()) ? A.size() : B.size();
        if (n == 0) return -1.0;

        int 不同计数 = 0;
        for (size_t i = 0; i < n; ++i)
        {
            if (A[i] != B[i])
                ++不同计数;
        }

        return 1.0 - static_cast<double>(不同计数) / n;  // 完全相同 = 1.0
    }
