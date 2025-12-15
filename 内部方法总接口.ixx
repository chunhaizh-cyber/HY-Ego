export module 内部方法总接口;

// 单文件：内部方法（Internal Methods）统一模块接口
// 目标：将“数据组织/结构化/一致性/检索/推导”的内聚能力抽象为稳定接口，
//      与《外部方法总接口.ixx》互补，服务世界树 / 场景 / 任务-方法系统。
// 说明：
//  1) 仅抽象接口 + 轻量数据结构；
//  2) 不依赖第三方库；
//  3) 与工程中的“链表模板/主信息定义模块”对接；

import <string>;
import <vector>;
import <optional>;
import <memory>;
import <cstdint>;
import <chrono>;
import <compare>;

import 主信息定义模块; // 提供 基础信息节点类/特征节点类/存在节点类/场景节点类/状态节点类/动态节点类 等

export namespace 内部方法 {

    using 字符串 = std::string;

    // ----------------------------------- 通用返回封装 -----------------------------------
    export template <typename T>
        struct 结果 {
        bool 成功 = false;
        T 数据{};
        字符串 错误信息;
    };

    export struct 统计信息 {
        uint64_t 变更节点数 = 0;
        uint64_t 新建节点数 = 0;
        uint64_t 删除节点数 = 0;
        uint64_t 索引命中 = 0;
        uint64_t 扫描数量 = 0;
        std::chrono::nanoseconds 耗时{ 0 };
    };

    // ----------------------------------- 事务与一致性 -----------------------------------
    export enum class 隔离级别 { 读已提交, 可重复读, 可串行化 };

    export struct 事务选项 {
        隔离级别 隔离 = 隔离级别::可重复读;
        bool 只读 = false;
    };

    export class 事务 {
    public:
        virtual ~事务() = default;
        virtual bool 提交() = 0;
        virtual void 回滚() = 0;
    };

    export class 事务管理接口 {
    public:
        virtual ~事务管理接口() = default;
        virtual std::unique_ptr<事务> 开启(const 事务选项& 选) = 0;
    };

    // ----------------------------------- 节点工厂与回收 -----------------------------------
    export class 节点工厂接口 {
    public:
        virtual ~节点工厂接口() = default;
        virtual 结果<基础信息节点类*> 新基础信息节点(基础信息基类* 主信息) = 0;
        virtual 结果<特征节点类*>     新特征节点(特征节点主信息类* 主信息) = 0;
        virtual 结果<存在节点类*>     新存在节点(基础信息基类* 主信息) = 0;
        virtual 结果<场景节点类*>     新场景节点(基础信息基类* 主信息) = 0;
        virtual 结果<状态节点类*>     新状态节点(状态节点主信息类* 主信息) = 0;
        virtual 结果<动态节点类*>     新动态节点(动态节点主信息类* 主信息) = 0;

        virtual 结果<bool> 删除节点(基础信息节点类* 节点) = 0;
    };

    // ----------------------------------- 链结构与关系操作 -----------------------------------
    export class 链操作接口 {
    public:
        virtual ~链操作接口() = default;
        // 父子/兄弟插入删除
        virtual 结果<基础信息节点类*> 插入到(基础信息节点类* 位置, 基础信息节点类* 新节点) = 0;
        virtual 结果<基础信息节点类*> 添加子节点(基础信息节点类* 父, 基础信息节点类* 子) = 0;
        virtual 结果<bool> 断开(基础信息节点类* 节点) = 0;

        // 引用计数/子计数维护（映射到链表模板里的计数）
        virtual int64_t 获取引用数(基础信息节点类* 节点) = 0;
        virtual int64_t 变更引用数(基础信息节点类* 节点, int64_t 变更) = 0;
        virtual int64_t 获取子数量(基础信息节点类* 节点) = 0;
    };

    // ----------------------------------- 标识 / 索引 / 查询 -----------------------------------
    export enum class 匹配模式 { 精确, 同类同值, 近似 };

    export struct 查询条件 {
        枚举_比较字段 字段;
        基础信息基类* 基准 = nullptr;
        匹配模式 模式 = 匹配模式::精确;
    };

    export class 索引接口 {
    public:
        virtual ~索引接口() = default;
        virtual 结果<bool> 建立(场景节点类* 场景) = 0;            // 根据场景构建索引
        virtual 结果<bool> 更新(基础信息节点类* 节点) = 0;        // 增量更新
        virtual 结果<bool> 移除(基础信息节点类* 节点) = 0;        // 从索引移除
        virtual 结果<std::vector<基础信息节点类*>> 查询(
            const 查询条件& 条件,
            size_t 限制 = 64) = 0;
    };

    // ----------------------------------- 特征/存在/场景操作 -----------------------------------
    export class 特征操作接口 {
    public:
        virtual ~特征操作接口() = default;
        virtual 结果<特征节点类*> 添加特征(
            基础信息节点类* 位置,
            词性节点类* 类型,
            特征值节点类* 值) = 0;

        virtual 结果<bool> 更新特征值(
            特征节点类* 特征,
            特征值节点类* 新值) = 0;

        virtual 结果<int64_t> 计算差值(
            特征节点类* A,
            特征节点类* B,
            枚举_比较字段 字段) = 0;
    };

    export class 存在场景操作接口 {
    public:
        virtual ~存在场景操作接口() = default;
        virtual 结果<存在节点类*> 添加存在(场景节点类* 场景, 基础信息基类* 主信息) = 0;
        virtual 结果<bool>        移除存在(存在节点类* 存在) = 0;
        virtual 结果<场景节点类*>  建立子场景(场景节点类* 父场景, 基础信息基类* 主信息) = 0;
        virtual 结果<bool>        重建场景索引(场景节点类* 场景) = 0;
    };

    // ----------------------------------- 状态 / 动态 / 队列 -----------------------------------
    export class 状态动态接口 {
    public:
        virtual ~状态动态接口() = default;
        // 状态：写入与对齐
        virtual 结果<状态节点类*> 记录状态(
            存在节点类* 存在,
            状态节点主信息类* 信息) = 0;

        virtual 结果<std::vector<状态节点类*>> 获取状态序列(
            存在节点类* 存在,
            size_t 限制 = 128) = 0;

        // 动态：由两状态推导
        virtual 结果<动态节点类*> 生成动态(
            状态节点类* 初始,
            状态节点类* 结果) = 0;

        virtual 结果<std::vector<动态节点类*>> 抽取动态模式(
            存在节点类* 存在,
            size_t 最小重复 = 2) = 0;
    };

    // ----------------------------------- 关系 / 因果 推导 -----------------------------------
    export class 关系因果接口 {
    public:
        virtual ~关系因果接口() = default;
        virtual 结果<二次特征节点类*> 计算相对关系(
            存在节点类* 基准,
            存在节点类* 对象,
            枚举_比较字段 基准字段) = 0;

        virtual 结果<基础信息节点类*> 记录因果(
            场景节点类* 场景,
            基础信息节点类* 因,
            基础信息节点类* 果,
            基础信息节点类* 动作 = nullptr) = 0;
    };

    // ----------------------------------- 一致性校验 / 规范化 -----------------------------------
    export class 校验规范化接口 {
    public:
        virtual ~校验规范化接口() = default;
        virtual 结果<统计信息> 校验场景(场景节点类* 场景) = 0; // 指针闭环、计数、主键信息
        virtual 结果<统计信息> 规范化场景(场景节点类* 场景) = 0; // 去重、合并、断链修复
    };

    // ----------------------------------- 序列化（内部快照） -----------------------------------
    export class 序列化接口 {
    public:
        virtual ~序列化接口() = default;
        virtual 结果<bool> 保存到文件(场景节点类* 场景, const 字符串& 路径) = 0;
        virtual 结果<场景节点类*> 从文件载入(const 字符串& 路径) = 0;
    };

    // ----------------------------------- 内部方法中心（Facade） -----------------------------------
    export class 内部方法中心 {
    public:
        // 注入实现
        void 设定事务管理(std::shared_ptr<事务管理接口> x) { 事务管理 = (x); }
        void 设定节点工厂(std::shared_ptr<节点工厂接口> x) { 节点工厂 = (x); }
        void 设定链操作(std::shared_ptr<链操作接口> x) { 链操作 = (x); }
        void 设定索引(std::shared_ptr<索引接口> x) { 索引 = (x); }
        void 设定特征操作(std::shared_ptr<特征操作接口> x) { 特征操作 = (x); }
        void 设定存在场景(std::shared_ptr<存在场景操作接口> x) { 存在场景 = (x); }
        void 设定状态动态(std::shared_ptr<状态动态接口> x) { 状态动态 = (x); }
        void 设定关系因果(std::shared_ptr<关系因果接口> x) { 关系因果 = (x); }
        void 设定校验规范化(std::shared_ptr<校验规范化接口> x) { 校验规范化 = (x); }
        void 设定序列化(std::shared_ptr<序列化接口> x) { 序列化 = (x); }

        // 便捷访问
        std::shared_ptr<事务管理接口>       获取事务管理()   const { return 事务管理; }
        std::shared_ptr<节点工厂接口>       获取节点工厂()   const { return 节点工厂; }
        std::shared_ptr<链操作接口>         获取链操作()     const { return 链操作; }
        std::shared_ptr<索引接口>           获取索引()       const { return 索引; }
        std::shared_ptr<特征操作接口>       获取特征操作()   const { return 特征操作; }
        std::shared_ptr<存在场景操作接口>   获取存在场景()   const { return 存在场景; }
        std::shared_ptr<状态动态接口>       获取状态动态()   const { return 状态动态; }
        std::shared_ptr<关系因果接口>       获取关系因果()   const { return 关系因果; }
        std::shared_ptr<校验规范化接口>     获取校验规范化() const { return 校验规范化; }
        std::shared_ptr<序列化接口>         获取序列化()     const { return 序列化; }

    private:
        std::shared_ptr<事务管理接口>       事务管理;
        std::shared_ptr<节点工厂接口>       节点工厂;
        std::shared_ptr<链操作接口>         链操作;
        std::shared_ptr<索引接口>           索引;
        std::shared_ptr<特征操作接口>       特征操作;
        std::shared_ptr<存在场景操作接口>   存在场景;
        std::shared_ptr<状态动态接口>       状态动态;
        std::shared_ptr<关系因果接口>       关系因果;
        std::shared_ptr<校验规范化接口>     校验规范化;
        std::shared_ptr<序列化接口>         序列化;
    };

}