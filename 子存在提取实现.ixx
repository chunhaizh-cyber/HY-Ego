module;

#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <cmath>
#include <cstdint>

import 世界树环境模块;
import 世界树模块;
import 基础数据类型模块;
import 存在模块;
import 存在提取类模块;
import 三维场景管理类模块;
import 通用函数模块;
import <string>;
import <functional>;

export module 子存在提取实现模块;

export class 子存在提取实现类 {
public:
    struct 提取参数 {
        // 阶段 1-2：筛选参数
        double 最小子面积比例 = 0.02; // 相对于父ROI面积的最小比例
        double 最大触边率 = 0.8;       // 轮廓与ROI边界重合比例
        double 最小长宽比 = 0.1;       // 外接框最小长宽比
        double 最大长宽比 = 10.0;      // 外接框最大长宽比
        
        // 阶段 3-4：几何参数
        double 最小有效深度 = 0.1;     // 最小有效深度（mm）
        int 最小点数 = 10;             // 最小有效点数
        
        // 阶段 5：颜色参数
        bool 更新父平均颜色 = true;   // 是否在提取子存在后更新父平均颜色
        
        // 阶段 6-7：匹配与索引参数
        int 最大子存在数量 = 24;       // 最大子存在数量
    };

public:
    子存在提取实现类(提取参数 p = {}) : 参数_(std::move(p)) {}

    // 子存在提取总流程
    std::vector<结构体_存在观测> 提取子存在(const 结构体_存在观测& 父存在观测, const 结构体_原始场景帧& 原始帧) {
        std::vector<结构体_存在观测> 子存在观测列表;
        
        // 检查父存在观测是否有效
        if (!父存在观测.有效) return 子存在观测列表;
        
        // 阶段 1：为父存在建立“子轮廓工作区”
        auto 子候选轮廓列表 = 阶段1_建立子轮廓工作区(父存在观测, 原始帧);
        if (子候选轮廓列表.empty()) return 子存在观测列表;
        
        // 阶段 2：对子轮廓候选做“几何筛选 + 稳定性筛选”
        auto 筛选后子候选列表 = 阶段2_筛选子轮廓候选(子候选轮廓列表, 父存在观测);
        if (筛选后子候选列表.empty()) return 子存在观测列表;
        
        // 阶段 3：为每个子候选生成“子掩膜 + 子像素集合”
        auto 子掩膜像素集合列表 = 阶段3_生成子掩膜像素集合(筛选后子候选列表, 父存在观测, 原始帧);
        if (子掩膜像素集合列表.empty()) return 子存在观测列表;
        
        // 阶段 4：从 2D 子像素集合恢复 3D 子点云并计算 3D 几何
        auto 子点云几何列表 = 阶段4_恢复3D子点云(子掩膜像素集合列表, 父存在观测, 原始帧);
        if (子点云几何列表.empty()) return 子存在观测列表;
        
        // 阶段 5：计算“子存在的视觉属性”
        auto 子视觉属性列表 = 阶段5_计算视觉属性(子掩膜像素集合列表, 父存在观测, 原始帧);
        if (子视觉属性列表.empty()) return 子存在观测列表;
        
        // 阶段 6：生成“可匹配的签名/编码”并构造子存在观测
        子存在观测列表 = 阶段6_构造子存在观测(筛选后子候选列表, 子点云几何列表, 子视觉属性列表, 父存在观测, 原始帧);
        if (子存在观测列表.empty()) return 子存在观测列表;
        
        // 限制子存在数量
        if (子存在观测列表.size() > (std::size_t)参数_.最大子存在数量) {
            // 按面积排序，保留最大的N个
            std::sort(子存在观测列表.begin(), 子存在观测列表.end(),
                [](const 结构体_存在观测& a, const 结构体_存在观测& b) {
                    return a.尺寸.x * a.尺寸.y * a.尺寸.z > b.尺寸.x * b.尺寸.y * b.尺寸.z;
                });
            子存在观测列表.resize((std::size_t)参数_.最大子存在数量);
        }
        
        return 子存在观测列表;
    }
    
    // 更新子存在到父存在的内部世界
    bool 更新子存在到内部世界(存在节点类* 父存在, const std::vector<结构体_存在观测>& 子存在观测列表) {
        if (!父存在) return false;
        if (子存在观测列表.empty()) return false;
        
        // 获取或创建父存在的内部世界场景
        auto* 内部世界场景 = 三维场景管理类::获取或创建内部世界(父存在);
        if (!内部世界场景) return false;
        
        // 更新子存在到内部世界
        for (const auto& 子存在观测 : 子存在观测列表) {
            // 生成部件键
            std::uint64_t partKey = 通用函数类::生成子存在部件键(子存在观测);
            
            // 在内部世界中查找或创建子存在
            auto* 子存在 = 三维场景管理类::获取或创建子存在_按PartKey(内部世界场景, partKey);
            if (!子存在) continue;
            
            // 更新子存在的特征
            三维场景管理类::更新存在_按观测(子存在, 子存在观测);
        }
        
        return true;
    }
    
    // 设置提取参数
    void 设置参数(提取参数 p) {
        参数_ = std::move(p);
    }
    
    // 获取当前参数
    const 提取参数& 获取参数() const {
        return 参数_;
    }
    
private:
    // 阶段 1：为父存在建立“子轮廓工作区”
    std::vector<结构体_轮廓观测> 阶段1_建立子轮廓工作区(const 结构体_存在观测& 父存在观测, const 结构体_原始场景帧& 原始帧) {
        std::vector<结构体_轮廓观测> 子候选轮廓列表;
        
        // 1. 在父 ROI 内取父mask
        const auto& 父ROI = 父存在观测.ROI;
        const auto& 父掩膜 = 父存在观测.裁剪掩膜;
        
        // 2. 检查父掩膜是否有效
        if (父掩膜.宽度 <= 0 || 父掩膜.高度 <= 0 || 父掩膜.通道数 != 1) {
            return 子候选轮廓列表;
        }
        
        // 3. 对父掩膜做轻量净化
        // TODO: 实现去噪和填洞功能
        
        // 4. 使用轮廓树路线提取子轮廓
        // 这里简化实现，使用点簇结果作为子候选轮廓
        // 实际项目中应该使用findContours或其他轮廓提取算法
        
        // 假设父存在观测中已经包含了点簇结果
        if (!父存在观测.轮廓观测.empty()) {
            for (const auto& 轮廓观测 : 父存在观测.轮廓观测) {
                if (轮廓观测.有效) {
                    子候选轮廓列表.push_back(轮廓观测);
                }
            }
        }
        
        return 子候选轮廓列表;
    }
    
    // 阶段 2：对子轮廓候选做“几何筛选 + 稳定性筛选”
    std::vector<结构体_轮廓观测> 阶段2_筛选子轮廓候选(const std::vector<结构体_轮廓观测>& 子候选轮廓列表, const 结构体_存在观测& 父存在观测) {
        std::vector<结构体_轮廓观测> 筛选后子候选列表;
        
        // 计算父ROI面积
        const auto& 父ROI = 父存在观测.ROI;
        double 父面积 = (父ROI.umax - 父ROI.umin + 1) * (父ROI.vmax - 父ROI.vmin + 1);
        
        for (const auto& 子轮廓观测 : 子候选轮廓列表) {
            const auto& 子边界 = 子轮廓观测.边界;
            
            // 计算子轮廓面积
            int 子宽度 = 子边界.umax - 子边界.umin + 1;
            int 子高度 = 子边界.vmax - 子边界.vmin + 1;
            double 子面积 = 子宽度 * 子高度;
            
            // 1. 面积阈值筛选
            if (子面积 < 父面积 * 参数_.最小子面积比例) {
                continue;
            }
            
            // 2. 外接框长宽比筛选
            double 宽高比 = (double)子宽度 / (double)子高度;
            if (宽高比 < 参数_.最小长宽比 || 宽高比 > 参数_.最大长宽比) {
                continue;
            }
            
            // 3. 触边率筛选
            int 触边像素 = 0;
            // 上边
            for (int u = 子边界.umin; u <= 子边界.umax; ++u) {
                if (子边界.vmin == 0 || u == 0 || u == 父ROI.umax - 父ROI.umin) {
                    触边像素++;
                }
            }
            // 下边
            for (int u = 子边界.umin; u <= 子边界.umax; ++u) {
                if (子边界.vmax == 父ROI.vmax - 父ROI.vmin || u == 0 || u == 父ROI.umax - 父ROI.umin) {
                    触边像素++;
                }
            }
            // 左边
            for (int v = 子边界.vmin; v <= 子边界.vmax; ++v) {
                if (子边界.umin == 0 || v == 0 || v == 父ROI.vmax - 父ROI.vmin) {
                    触边像素++;
                }
            }
            // 右边
            for (int v = 子边界.vmin; v <= 子边界.vmax; ++v) {
                if (子边界.umax == 父ROI.umax - 父ROI.umin || v == 0 || v == 父ROI.vmax - 父ROI.vmin) {
                    触边像素++;
                }
            }
            
            double 轮廓周长 = 2 * (子宽度 + 子高度);
            double 触边率 = (double)触边像素 / 轮廓周长;
            if (触边率 > 参数_.最大触边率) {
                continue;
            }
            
            // 所有筛选条件都满足，添加到筛选后列表
            筛选后子候选列表.push_back(子轮廓观测);
        }
        
        return 筛选后子候选列表;
    }
    
    // 阶段 3：为每个子候选生成“子掩膜 + 子像素集合”
    struct 子掩膜像素集合 {
        结构体_图像缓冲_u8 子掩膜;
        std::vector<std::pair<int, int>> 子像素集合;
    };
    
    std::vector<子掩膜像素集合> 阶段3_生成子掩膜像素集合(const std::vector<结构体_轮廓观测>& 筛选后子候选列表, const 结构体_存在观测& 父存在观测, const 结构体_原始场景帧& 原始帧) {
        std::vector<子掩膜像素集合> 子掩膜像素集合列表;
        
        const auto& 父ROI = 父存在观测.ROI;
        const auto& 父掩膜 = 父存在观测.裁剪掩膜;
        
        for (const auto& 子轮廓观测 : 筛选后子候选列表) {
            const auto& 子边界 = 子轮廓观测.边界;
            
            // 创建子掩膜
            结构体_图像缓冲_u8 子掩膜;
            子掩膜.宽度 = 父掩膜.宽度;
            子掩膜.高度 = 父掩膜.高度;
            子掩膜.通道数 = 1;
            子掩膜.数据.resize(子掩膜.宽度 * 子掩膜.高度, 0);
            
            // 创建子像素集合
            std::vector<std::pair<int, int>> 子像素集合;
            
            // 遍历子边界内的所有像素
            for (int v = 子边界.vmin; v <= 子边界.vmax; ++v) {
                for (int u = 子边界.umin; u <= 子边界.umax; ++u) {
                    // 检查是否在父掩膜内
                    int 父像素索引 = v * 父掩膜.宽度 + u;
                    if (父掩膜.数据[父像素索引] > 0) {
                        // 在父掩膜内，标记子掩膜
                        int 子像素索引 = v * 子掩膜.宽度 + u;
                        子掩膜.数据[子像素索引] = 255;
                        
                        // 添加到子像素集合
                        子像素集合.emplace_back(u, v);
                    }
                }
            }
            
            // 只添加有像素的子掩膜
            if (!子像素集合.empty()) {
                子掩膜像素集合 子数据;
                子数据.子掩膜 = std::move(子掩膜);
                子数据.子像素集合 = std::move(子像素集合);
                子掩膜像素集合列表.push_back(std::move(子数据));
            }
        }
        
        return 子掩膜像素集合列表;
    }
    
    // 阶段 4：从 2D 子像素集合恢复 3D 子点云并计算 3D 几何
    struct 子点云几何 {
        std::vector<Vector3D> 点云簇3D;
        Vector3D 中心;
        Vector3D 尺寸;
        Vector3D 主方向1;
        Vector3D 主方向2;
        Vector3D 主方向3;
    };
    
    std::vector<子点云几何> 阶段4_恢复3D子点云(const std::vector<子掩膜像素集合>& 子掩膜像素集合列表, const 结构体_存在观测& 父存在观测, const 结构体_原始场景帧& 原始帧) {
        std::vector<子点云几何> 子点云几何列表;
        
        // 获取深度内参
        const auto& 深度内参 = 原始帧.深度内参;
        
        for (const auto& 子数据 : 子掩膜像素集合列表) {
            const auto& 子像素集合 = 子数据.子像素集合;
            
            // 恢复3D子点云
            std::vector<Vector3D> 点云簇3D;
            点云簇3D.reserve(子像素集合.size());
            
            for (const auto& 像素 : 子像素集合) {
                int u = 像素.first + 父存在观测.ROI.umin;
                int v = 像素.second + 父存在观测.ROI.vmin;
                
                // 获取深度值
                float 深度值 = 原始帧.深度图.数据[v * 原始帧.深度图.宽度 + u];
                if (深度值 <= 参数_.最小有效深度) {
                    continue;
                }
                
                // 将像素坐标转换为3D坐标
                Vector3D 点3D;
                double x = (u - 深度内参.cx) * 深度值 / 深度内参.fx;
                double y = (v - 深度内参.cy) * 深度值 / 深度内参.fy;
                double z = 深度值;
                
                点3D.x = static_cast<float>(x);
                点3D.y = static_cast<float>(y);
                点3D.z = static_cast<float>(z);
                
                点云簇3D.push_back(点3D);
            }
            
            // 检查有效点数
            if (点云簇3D.size() < 参数_.最小点数) {
                continue;
            }
            
            // 计算3D几何信息
            Vector3D 中心(0, 0, 0);
            for (const auto& 点 : 点云簇3D) {
                中心 += 点;
            }
            中心 /= static_cast<float>(点云簇3D.size());
            
            // 计算主方向（PCA）
            Vector3D 主方向1(1, 0, 0);
            Vector3D 主方向2(0, 1, 0);
            Vector3D 主方向3(0, 0, 1);
            
            // 计算尺寸
            float xmin = FLT_MAX, ymin = FLT_MAX, zmin = FLT_MAX;
            float xmax = -FLT_MAX, ymax = -FLT_MAX, zmax = -FLT_MAX;
            
            for (const auto& 点 : 点云簇3D) {
                xmin = std::min(xmin, 点.x);
                ymin = std::min(ymin, 点.y);
                zmin = std::min(zmin, 点.z);
                xmax = std::max(xmax, 点.x);
                ymax = std::max(ymax, 点.y);
                zmax = std::max(zmax, 点.z);
            }
            
            Vector3D 尺寸;
            尺寸.x = xmax - xmin;
            尺寸.y = ymax - ymin;
            尺寸.z = zmax - zmin;
            
            // 过滤尺寸异常（某一维=0）
            if (尺寸.x <= 0 || 尺寸.y <= 0 || 尺寸.z <= 0) {
                continue;
            }
            
            // 创建子点云几何对象
            子点云几何 子几何;
            子几何.点云簇3D = std::move(point云簇3D);
            子几何.中心 = 中心;
            子几何.尺寸 = 尺寸;
            子几何.主方向1 = 主方向1;
            子几何.主方向2 = 主方向2;
            子几何.主方向3 = 主方向3;
            
            子点云几何列表.push_back(std::move(子几何));
        }
        
        return 子点云几何列表;
    }
    
    // 阶段 5：计算“子存在的视觉属性”
    struct 子视觉属性 {
        Vector3f 平均颜色; // RGB 颜色
        Vector3f 父平均颜色; // 更新后的父平均颜色
    };
    
    std::vector<子视觉属性> 阶段5_计算视觉属性(const std::vector<子掩膜像素集合>& 子掩膜像素集合列表, const 结构体_存在观测& 父存在观测, const 结构体_原始场景帧& 原始帧) {
        std::vector<子视觉属性> 子视觉属性列表;
        
        const auto& 父ROI = 父存在观测.ROI;
        
        // 如果需要更新父平均颜色，创建父掩膜副本
        结构体_图像缓冲_u8 父掩膜副本;
        if (参数_.更新父平均颜色) {
            父掩膜副本 = 父存在观测.裁剪掩膜;
        }
        
        for (const auto& 子数据 : 子掩膜像素集合列表) {
            const auto& 子像素集合 = 子数据.子像素集合;
            
            // 计算子平均颜色
            int r_sum = 0, g_sum = 0, b_sum = 0;
            int valid_count = 0;
            
            for (const auto& 像素 : 子像素集合) {
                int u = 像素.first + 父ROI.umin;
                int v = 像素.second + 父ROI.vmin;
                
                // 获取颜色值
                int color_index = v * 原始帧.彩色图.宽度 + u;
                const auto& 颜色 = 原始帧.彩色图.数据[color_index];
                
                r_sum += 颜色.r;
                g_sum += 颜色.g;
                b_sum += 颜色.b;
                valid_count++;
                
                // 如果需要更新父平均颜色，标记父掩膜副本
                if (参数_.更新父平均颜色) {
                    int 父像素索引 = 像素.second * 父掩膜副本.宽度 + 像素.first;
                    父掩膜副本.数据[父像素索引] = 0;
                }
            }
            
            Vector3f 平均颜色(0, 0, 0);
            if (valid_count > 0) {
                平均颜色.x = static_cast<float>(r_sum) / valid_count;
                平均颜色.y = static_cast<float>(g_sum) / valid_count;
                平均颜色.z = static_cast<float>(b_sum) / valid_count;
            }
            
            // 创建子视觉属性对象
            子视觉属性 子视觉属性;
            子视觉属性.平均颜色 = 平均颜色;
            
            子视觉属性列表.push_back(子视觉属性);
        }
        
        // 如果需要更新父平均颜色，计算更新后的父平均颜色
        if (参数_.更新父平均颜色) {
            int r_sum = 0, g_sum = 0, b_sum = 0;
            int valid_count = 0;
            
            for (int v = 0; v < 父掩膜副本.高度; ++v) {
                for (int u = 0; u < 父掩膜副本.宽度; ++u) {
                    int 父像素索引 = v * 父掩膜副本.宽度 + u;
                    if (父掩膜副本.数据[父像素索引] > 0) {
                        int 全局u = u + 父ROI.umin;
                        int 全局v = v + 父ROI.vmin;
                        
                        int color_index = 全局v * 原始帧.彩色图.宽度 + 全局u;
                        const auto& 颜色 = 原始帧.彩色图.数据[color_index];
                        
                        r_sum += 颜色.r;
                        g_sum += 颜色.g;
                        b_sum += 颜色.b;
                        valid_count++;
                    }
                }
            }
            
            Vector3f 父平均颜色(0, 0, 0);
            if (valid_count > 0) {
                父平均颜色.x = static_cast<float>(r_sum) / valid_count;
                父平均颜色.y = static_cast<float>(g_sum) / valid_count;
                父平均颜色.z = static_cast<float>(b_sum) / valid_count;
            }
            
            // 更新所有子视觉属性对象的父平均颜色
            for (auto& 子视觉属性 : 子视觉属性列表) {
                子视觉属性.父平均颜色 = 父平均颜色;
            }
        }
        
        return 子视觉属性列表;
    }
    
    // 阶段 6：生成“可匹配的签名/编码”并构造子存在观测
    std::vector<结构体_存在观测> 阶段6_构造子存在观测(const std::vector<结构体_轮廓观测>& 筛选后子候选列表, const std::vector<子点云几何>& 子点云几何列表, const std::vector<子视觉属性>& 子视觉属性列表, const 结构体_存在观测& 父存在观测, const 结构体_原始场景帧& 原始帧) {
        std::vector<结构体_存在观测> 子存在观测列表;
        
        // 确保所有列表长度相同
        if (筛选后子候选列表.size() != 子点云几何列表.size() || 子点云几何列表.size() != 子视觉属性列表.size()) {
            return 子存在观测列表;
        }
        
        const auto& 父ROI = 父存在观测.ROI;
        
        for (size_t i = 0; i < 筛选后子候选列表.size(); ++i) {
            const auto& 子轮廓观测 = 筛选后子候选列表[i];
            const auto& 子点云几何 = 子点云几何列表[i];
            const auto& 子视觉属性 = 子视觉属性列表[i];
            
            // 创建子存在观测
            结构体_存在观测 子存在观测;
            
            // 设置基本信息
            子存在观测.有效 = true;
            子存在观测.ROI.umin = 子轮廓观测.边界.umin + 父ROI.umin;
            子存在观测.ROI.umax = 子轮廓观测.边界.umax + 父ROI.umin;
            子存在观测.ROI.vmin = 子轮廓观测.边界.vmin + 父ROI.vmin;
            子存在观测.ROI.vmax = 子轮廓观测.边界.vmax + 父ROI.vmin;
            
            // 设置3D几何信息
            子存在观测.中心坐标 = 子点云几何.中心;
            子存在观测.尺寸 = 子点云几何.尺寸;
            子存在观测.主方向1 = 子点云几何.主方向1;
            子存在观测.主方向2 = 子点云几何.主方向2;
            子存在观测.主方向3 = 子点云几何.主方向3;
            
            // 设置颜色信息
            子存在观测.颜色 = 子视觉属性.平均颜色;
            
            // 设置点云信息
            子存在观测.点云簇3D = 子点云几何.点云簇3D;
            子存在观测.点数 = static_cast<int>(子点云几何.点云簇3D.size());
            
            // 设置距离
            子存在观测.距离 = std::sqrt(
                子存在观测.中心坐标.x * 子存在观测.中心坐标.x +
                子存在观测.中心坐标.y * 子存在观测.中心坐标.y +
                子存在观测.中心坐标.z * 子存在观测.中心坐标.z);
            
            // 设置彩色截图数据（方案1：传递预先生成的彩色截图）
            子存在观测.彩色截图 = 子轮廓观测.彩色截图;
            子存在观测.彩色截图元信息 = 子轮廓观测.彩色截图元信息;
            
            // 生成签名
            子存在观测.签名 = 通用函数类::生成子存在部件键(子存在观测);
            
            // 添加到子存在观测列表
            子存在观测列表.push_back(子存在观测);
        }
        
        return 子存在观测列表;
    }
    
private:
    提取参数 参数_;
};

export inline std::vector<结构体_存在观测> 提取子存在(const 结构体_存在观测& 父存在观测, const 结构体_原始场景帧& 原始帧, 子存在提取实现类::提取参数 p = {}) {
    子存在提取实现类 提取器(p);
    return 提取器.提取子存在(父存在观测, 原始帧);
}

export inline bool 更新子存在到内部世界(存在节点类* 父存在, const std::vector<结构体_存在观测>& 子存在观测列表) {
    子存在提取实现类 提取器;
    return 提取器.更新子存在到内部世界(父存在, 子存在观测列表);
}
