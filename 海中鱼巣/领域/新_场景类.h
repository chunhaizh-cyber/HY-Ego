#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <vector>

#include "新_存在类.h"

namespace 海中鱼巣 {

// 全局基础数据集中的世界树根场景。节点稳定编码本身就是根场景身份。
extern 稳定编码 场景树根节点;

struct 新场景信息 final {
    稳定编码 节点;
    std::optional<稳定编码> 父场景节点;
    稳定编码 专属存在概念节点;
    std::vector<稳定编码> 特征节点组;
    std::vector<稳定编码> 直接成员存在组;
    // 纯引用：边界存在不要求直接归属于本场景。
    std::vector<稳定编码> 边界存在组;
    std::vector<稳定编码> 直接子场景组;
    std::vector<稳定编码> 状态节点组;
    friend bool operator==(const 新场景信息&, const 新场景信息&) = default;
};

enum class 新场景操作状态 : std::uint8_t {
    已完成 = 1,
    无变化 = 2,
    入口拒绝 = 3,
    场景不存在 = 4,
    父场景不合法 = 5,
    存在不存在 = 6,
    存在已属于其它场景 = 7,
    存在建立失败 = 8,
    存在仍有子存在 = 9,
    会形成环 = 10,
    根场景不可操作 = 11,
    场景非空 = 12,
    结构不一致 = 13,
    资源失败 = 14,
    存在仍被边界引用 = 15,
    状态不存在 = 16,
    状态与场景不一致 = 17,
    特征已改变但状态挂接未完成 = 18,
    存在已建立但状态挂接未完成 = 19,
    状态已挂接但专属概念未完成 = 20,
    存在仍被状态引用 = 21
};

struct 新场景建立结果 final {
    新场景操作状态 状态 = 新场景操作状态::入口拒绝;
    std::optional<稳定编码> 场景节点;
    std::optional<稳定编码> 专属存在概念节点;
};

struct 新场景成员建立结果 final {
    新场景操作状态 状态 = 新场景操作状态::入口拒绝;
    std::optional<稳定编码> 存在节点;
    std::optional<稳定编码> 专属存在概念节点;
    std::vector<稳定编码> 初始状态节点组;
};

struct 新场景特征添加结果 final {
    新场景操作状态 状态 = 新场景操作状态::入口拒绝;
    std::optional<新存在特征添加结果> 存在结果;
    std::optional<稳定编码> 状态节点;
};

struct 新场景特征值更新结果 final {
    新场景操作状态 状态 = 新场景操作状态::入口拒绝;
    std::optional<新存在特征值更新结果> 存在结果;
    std::optional<稳定编码> 状态节点;
};

struct 三维毫米坐标 final {
    std::int64_t X = 0;
    std::int64_t Y = 0;
    std::int64_t Z = 0;
    friend bool operator==(const 三维毫米坐标&, const 三维毫米坐标&) = default;
};

struct 二维毫米坐标 final {
    std::int64_t X = 0;
    std::int64_t Y = 0;
    friend bool operator==(const 二维毫米坐标&, const 二维毫米坐标&) = default;
};

// 行主序；将自我局部坐标旋转到场景定位坐标系。
struct 三维朝向矩阵 final {
    std::array<double, 9> 分量{};
    friend bool operator==(const 三维朝向矩阵&, const 三维朝向矩阵&) = default;
};

struct 场景定位坐标读回 final {
    稳定编码 存在节点;
    三维毫米坐标 定位坐标;
    std::uint64_t 定位序号 = 0;
    std::uint64_t 形成时刻 = 0;
    std::uint64_t 有效截止时刻 = 0;
};

// 后继定位/状态服务实现此只读边界；场景类不在刷新过程中修改定位事实。
class 场景定位坐标读取接口 {
public:
    virtual ~场景定位坐标读取接口() = default;
    virtual std::optional<场景定位坐标读回> 读取存在定位坐标(
        稳定编码 场景节点,
        稳定编码 存在节点) const noexcept = 0;
};

struct 自我场景姿态 final {
    稳定编码 自我节点;
    稳定编码 场景节点;
    std::optional<三维毫米坐标> 定位坐标;
    std::optional<三维朝向矩阵> 朝向矩阵;
    std::uint64_t 定位序号 = 0;
    std::uint64_t 姿态序号 = 0;
    std::uint64_t 边界定义序号 = 0;
    std::uint64_t 形成时刻 = 0;
    std::uint64_t 有效截止时刻 = 0;
};

struct 场景存在相对坐标 final {
    稳定编码 存在节点;
    三维毫米坐标 相对坐标;
    std::uint64_t 来源定位序号 = 0;
    std::uint64_t 自我姿态序号 = 0;
    std::uint64_t 边界定义序号 = 0;
    std::uint64_t 形成时刻 = 0;
    std::uint64_t 有效截止时刻 = 0;
    friend bool operator==(const 场景存在相对坐标&, const 场景存在相对坐标&) = default;
};

enum class 场景相对坐标刷新状态 : std::uint8_t {
    已完成 = 1,
    部分完成 = 2,
    场景不存在 = 3,
    存在不属于场景 = 4,
    需要自我定位坐标 = 5,
    需要自我朝向 = 6,
    自我姿态已失效 = 7,
    朝向矩阵不合法 = 8,
    坐标超出范围 = 9,
    结构不一致 = 10,
    资源失败 = 11
};

struct 场景相对坐标刷新结果 final {
    场景相对坐标刷新状态 状态 = 场景相对坐标刷新状态::资源失败;
    std::vector<场景存在相对坐标> 相对坐标组;
    std::vector<稳定编码> 需要获取定位坐标的存在组;
};

struct 场景二维边界候选线段 final {
    std::uint64_t 线段序号 = 0;
    二维毫米坐标 起点;
    二维毫米坐标 终点;
    friend bool operator==(const 场景二维边界候选线段&,
        const 场景二维边界候选线段&) = default;
};

class 场景二维边界几何读取接口 {
public:
    virtual ~场景二维边界几何读取接口() = default;
    virtual std::vector<场景二维边界候选线段> 读取二维边界候选线段(
        稳定编码 场景节点,
        稳定编码 边界存在节点) const noexcept = 0;
};

// 候选面已经位于目标场景的定位坐标系中。面序号只要求在同一存在内
// 稳定且非零，用于把计算结果对应回提供者的具体面。
struct 场景三维边界候选面 final {
    std::uint64_t 面序号 = 0;
    std::vector<三维毫米坐标> 顶点组;
    friend bool operator==(const 场景三维边界候选面&,
        const 场景三维边界候选面&) = default;
};

class 场景三维边界几何读取接口 {
public:
    virtual ~场景三维边界几何读取接口() = default;
    virtual std::vector<场景三维边界候选面> 读取三维边界候选面(
        稳定编码 场景节点,
        稳定编码 边界存在节点) const noexcept = 0;
};

struct 场景二维边界线段 final {
    稳定编码 边界存在节点;
    std::uint64_t 线段序号 = 0;
    bool 边界存在属于当前场景范围 = false;
    二维毫米坐标 起点;
    二维毫米坐标 终点;
    friend bool operator==(const 场景二维边界线段&,
        const 场景二维边界线段&) = default;
};

struct 场景二维边界闭合环 final {
    std::vector<二维毫米坐标> 顶点组;
    std::vector<稳定编码> 来源边界存在组;
    friend bool operator==(const 场景二维边界闭合环&,
        const 场景二维边界闭合环&) = default;
};

struct 场景三维边界线段 final {
    三维毫米坐标 起点;
    三维毫米坐标 终点;
    friend bool operator==(const 场景三维边界线段&,
        const 场景三维边界线段&) = default;
};

struct 场景三维边界面 final {
    稳定编码 边界存在节点;
    std::uint64_t 面序号 = 0;
    bool 边界存在属于当前场景范围 = false;
    std::vector<三维毫米坐标> 顶点组;
    friend bool operator==(const 场景三维边界面&,
        const 场景三维边界面&) = default;
};

enum class 场景边界计算状态 : std::uint8_t {
    已完成 = 1,
    部分完成 = 2,
    场景不存在 = 3,
    边界存在组为空 = 4,
    边界存在不存在 = 5,
    边界几何缺失 = 6,
    边界几何不合法 = 7,
    边界面无法唯一确定 = 8,
    边界线段无法唯一确定 = 9,
    边界未闭合 = 10,
    结构不一致 = 11,
    资源失败 = 12
};

struct 场景二维边界计算结果 final {
    场景边界计算状态 状态 = 场景边界计算状态::资源失败;
    std::vector<场景二维边界线段> 边界线段组;
    std::vector<场景二维边界闭合环> 闭合环组;
    std::vector<稳定编码> 需要获取边界几何的存在组;
};

struct 场景三维边界计算结果 final {
    场景边界计算状态 状态 = 场景边界计算状态::资源失败;
    std::vector<场景三维边界面> 边界面组;
    std::vector<场景三维边界线段> 边界线段组;
    std::vector<稳定编码> 需要获取边界几何的存在组;
};

class 新_场景类 final {
public:
    新_场景类(新_存在类& 存在服务, 新_状态类& 状态服务) noexcept;

    bool 初始化() noexcept;

    // 根场景没有父场景；重复调用返回同一个根场景。
    新场景建立结果 建立根场景() noexcept;
    // 普通场景只需确认直接父场景即可建立为空场景。
    新场景建立结果 建立场景(稳定编码 父场景节点) noexcept;
    新场景操作状态 迁移子场景(
        稳定编码 场景节点,
        稳定编码 新父场景节点) noexcept;
    // 只删除没有直接子场景和成员存在的非根场景；场景自身特征和
    // 专属存在概念由本调用同步清理。
    新场景操作状态 删除空场景(
        稳定编码 场景节点) noexcept;

    std::optional<新场景信息> 获取场景(
        稳定编码 场景节点) const noexcept;
    bool 是场景节点(稳定编码 节点) const noexcept;

    // 普通存在只能通过场景类建立并立即写入一个最小场景的成员字段。
    新场景成员建立结果 建立成员存在(
        稳定编码 场景节点,
        const 新存在建立请求& 请求,
        const 新状态强时间& 初始状态时间) noexcept;
    新场景操作状态 迁移成员存在(
        稳定编码 存在节点,
        稳定编码 目标场景节点) noexcept;
    新场景操作状态 删除成员存在(
        稳定编码 场景节点,
        稳定编码 存在节点) noexcept;

    std::optional<稳定编码> 查询所属场景(
        稳定编码 存在节点) const noexcept;
    std::optional<稳定编码> 查询父场景(
        稳定编码 场景节点) const noexcept;
    std::vector<稳定编码> 查询直接子场景(
        稳定编码 场景节点) const noexcept;
    std::vector<稳定编码> 查询直接成员存在(
        稳定编码 场景节点) const noexcept;
    // 返回当前场景及全部下级场景直接成员的并集，不包含场景节点本身。
    std::vector<稳定编码> 查询场景范围存在(
        稳定编码 场景节点) const noexcept;
    // 场景节点按场景树判断；普通存在按其唯一直接所属场景判断。
    bool 存在属于场景范围(
        稳定编码 场景节点,
        稳定编码 存在节点) const noexcept;

    // 边界存在组只保存引用，不改变存在原有的场景归属。
    新场景操作状态 添加边界存在(
        稳定编码 场景节点,
        稳定编码 边界存在节点) noexcept;
    新场景操作状态 移除边界存在(
        稳定编码 场景节点,
        稳定编码 边界存在节点) noexcept;
    std::vector<稳定编码> 查询边界存在(
        稳定编码 场景节点) const noexcept;

    // 状态列表只保存状态节点引用。状态内容仍由状态类唯一维护。
    新场景操作状态 添加状态(
        稳定编码 场景节点,
        稳定编码 状态节点) noexcept;
    新场景操作状态 移除状态(
        稳定编码 场景节点,
        稳定编码 状态节点) noexcept;
    std::vector<稳定编码> 查询状态列表(
        稳定编码 场景节点) const noexcept;

    // 二维：点连接为线段，线段必须形成一个或多个精确闭合环。
    场景二维边界计算结果 计算二维场景边界(
        const 场景二维边界几何读取接口& 几何读取,
        稳定编码 场景节点,
        const 二维毫米坐标& 场景内部参考点) const noexcept;

    // 三维：候选面顶点依序连线；闭合空间中的每条无向边必须恰好由
    // 两个边界面共享。两个函数都不写入边界结果或补造缺失几何。
    场景三维边界计算结果 计算三维场景边界(
        const 场景三维边界几何读取接口& 几何读取,
        稳定编码 场景节点,
        const 三维毫米坐标& 场景内部参考点) const noexcept;

    // 场景本身具有存在身份；特征操作复用存在类，并持续更新同一个
    // 专属存在概念节点。相对坐标矩阵不进入这里。
    新场景特征添加结果 添加场景特征(
        稳定编码 场景节点,
        稳定编码 特征概念节点,
        const 新特征准确值& 初始值,
        const 新状态强时间& 强时间) noexcept;
    新场景特征值更新结果 更新场景特征值(
        稳定编码 场景节点,
        稳定编码 特征节点,
        const 新特征准确值& 新值,
        const 新状态强时间& 强时间) noexcept;
    新场景特征添加结果 添加场景内存在特征(
        稳定编码 场景节点,
        稳定编码 存在节点,
        稳定编码 特征概念节点,
        const 新特征准确值& 初始值,
        const 新状态强时间& 强时间) noexcept;
    新场景特征值更新结果 更新场景内存在特征值(
        稳定编码 场景节点,
        稳定编码 存在节点,
        稳定编码 特征节点,
        const 新特征准确值& 新值,
        const 新状态强时间& 强时间) noexcept;
    std::vector<稳定编码> 查询场景全部特征(
        稳定编码 场景节点) const noexcept;

    // 返回当前场景直接成员的完整替换结果；结果容器随成员数量扩展。
    // 缺少定位坐标的成员不计算，并进入“需要获取定位坐标”集合。
    场景相对坐标刷新结果 刷新场景存在相对坐标(
        const 场景定位坐标读取接口& 定位读取,
        const 自我场景姿态& 自我姿态,
        std::uint64_t 当前时刻) const noexcept;
    场景相对坐标刷新结果 刷新单个存在相对坐标(
        const 场景定位坐标读取接口& 定位读取,
        const 自我场景姿态& 自我姿态,
        稳定编码 存在节点,
        std::uint64_t 当前时刻) const noexcept;

private:
    新_存在类& 存在服务_;
    新_状态类& 状态服务_;
};

} // namespace 海中鱼巣
