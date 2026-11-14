#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "新_方法公共类型.h"
#include "信息内容.数据.h"
#include "新_内部治理对象引用.h"
#include "新_特征类.h"
#include "新_特征值类.h"
#include "服务要求.材料合同.h"
#include "服务值结算.本能合同.h"
#include "模拟需求.数据.h"
#include "新_词类.h"

namespace 海中鱼巣 {

class 新_方法类;
class 新_存在类;
class 新本能交互运行接口;
class 新方法隔离执行接口;
class 新本能需求运行接口;
class 新本能字段询问运行接口;

enum class 新方法参数特征读取状态 : std::uint8_t {
    已读取=1, 参数未绑定, 信息类型不符, 未声明,
    特征未取得, 特征匹配不唯一, 当前值未取得, 定义前提已变化, 能力未支持
};
enum class 新方法词条参数读取状态 : std::uint8_t {
    已读取=1, 未声明, 参数未绑定, 信息类型不符,
    参数已失效, 能力未支持, 依赖未就绪, 资源失败
};
struct 新方法词条参数读取结果 final {
    新方法词条参数读取状态 状态=新方法词条参数读取状态::参数未绑定;
    std::optional<新词条特征信息> 信息;
};
struct 新方法参数特征读取结果 final {
    新方法参数特征读取状态 状态=新方法参数特征读取状态::参数未绑定;
    std::optional<新特征信息> 特征;
    std::optional<新特征值信息> 当前完整材料;
};

enum class 新方法参数场景用途 : std::uint8_t {
    登记 = 1,
    执行 = 2,
    模拟 = 3,
    任务贡献治理 = 4
};

struct 新方法实际特征绑定 final {
    稳定编码 特征节点;
    friend bool operator==(const 新方法实际特征绑定&,
        const 新方法实际特征绑定&) = default;
};

struct 新方法任务成员绑定 final {
    std::uint32_t 集合参数序号 = 0;
    新任务虚拟存在引用 任务引用;
    std::optional<稳定编码> 服务值特征节点;
    friend bool operator==(const 新方法任务成员绑定&,
        const 新方法任务成员绑定&) = default;
};

struct 新方法参数角色绑定 final {
    稳定编码 参数角色节点;
    std::optional<稳定编码> 世界场景节点;
    std::optional<稳定编码> 实际存在节点;
    std::vector<新方法实际特征绑定> 实际特征组;
    std::optional<新任务虚拟存在引用> 任务虚拟存在引用;
    std::optional<新服务要求分项引用> 服务要求分项引用;
    std::optional<新服务要求确认引用> 服务要求确认引用;
    // 新格式只保存此按位引用及可选的世界场景定位；旧业务封套为空。
    std::optional<新方法有序参数引用> 有序引用;
    friend bool operator==(const 新方法参数角色绑定&,
        const 新方法参数角色绑定&) = default;
};

// 一个调用期所有者同时提供只读投影与隔离写端口，不能拼接不同样例。
class 新方法模拟运行接口 : public 新内部治理对象读取接口 {
public:
    virtual 正式信息参数读取结果 读取正式信息参数保护(
        稳定编码,稳定编码) const noexcept { return {}; }
    virtual 新模拟需求读回 读取模拟需求(稳定编码) const = 0;
    virtual 新模拟需求关联读回<服务要求理解信息> 读取模拟理解(稳定编码) const = 0;
    virtual 新模拟需求关联读回<服务要求确认信息> 读取模拟确认(稳定编码) const = 0;
    virtual 新模拟需求关联读回<新部分服务要求材料> 读取模拟要求(稳定编码) const = 0;
    virtual 新模拟需求关联读回<新模拟语言场景信息> 读取模拟语言场景(稳定编码) const = 0;
    virtual 新方法参数特征读取结果 读取模拟存在参数特征(
        std::uint32_t,稳定编码) const noexcept { return {新方法参数特征读取状态::未声明,{},{}}; }
    virtual 新本能交互运行接口* 获取交互运行接口() noexcept { return nullptr; }
    virtual 新方法隔离执行接口* 获取隔离执行接口() noexcept { return nullptr; }
    virtual 新本能需求运行接口* 获取需求运行接口() noexcept { return nullptr; }
    virtual 新本能字段询问运行接口* 获取字段询问运行接口() noexcept { return nullptr; }

    // 无此能力的模拟owner返回不适用，绝不回退现实owner。
    virtual std::optional<std::vector<稳定编码>> 读取模拟存在特征组(稳定编码) const noexcept { return {}; }
    virtual std::optional<新任务虚拟存在特征组> 读取模拟集合任务治理(
        std::uint32_t, 稳定编码) const noexcept { return {}; }
    virtual std::optional<稳定编码> 读取模拟所属场景(稳定编码) const noexcept { return {}; }
    virtual 稳定编码 建立模拟动作() { return {}; }
    virtual 新模拟操作状态 模拟递增计数(稳定编码,稳定编码) { return 新模拟操作状态::输入不适用; }
    virtual 新模拟操作状态 推进模拟输入() { return 新模拟操作状态::输入耗尽; }
    virtual 新模拟操作状态 模拟集合加入成员(稳定编码,稳定编码,稳定编码) { return 新模拟操作状态::输入不适用; }

    virtual 稳定编码 模拟根节点() const noexcept = 0;
    virtual bool 核验模拟参数(稳定编码 方法节点,
        const std::vector<新方法参数角色绑定>&) const noexcept = 0;
    virtual std::optional<新特征信息> 读取模拟特征(稳定编码) const noexcept = 0;
    virtual std::optional<新特征值信息> 读取模拟特征值材料(稳定编码) const noexcept = 0;
};

class 方法参数场景 final {
public:
    新方法词条参数读取结果 读取词条参数挂接(std::uint32_t 参数序号) const noexcept;
    正式信息参数读取结果 读取正式信息参数保护(std::uint32_t 参数序号) const noexcept;
    新模拟需求读回 读取模拟需求参数(std::uint32_t 特征参数序号) const;
    新方法参数特征读取结果 读取存在参数特征(std::uint32_t,稳定编码) const noexcept;
    std::optional<稳定编码> 执行分支节点() const noexcept;
    新本能交互运行接口* 获取交互运行接口() const noexcept;
    新本能需求运行接口* 获取需求运行接口() const noexcept;
    新本能字段询问运行接口* 获取字段询问运行接口() const noexcept;
    新方法模拟运行接口* 获取模拟运行接口() const noexcept;
    稳定编码 方法节点() const noexcept;
    新方法参数场景用途 用途() const noexcept;
    const std::optional<新任务贡献治理参数>& 读取任务贡献治理参数() const noexcept;
    bool 使用治理对象读取接口(const 新内部治理对象读取接口&) const noexcept;
    const std::vector<新方法参数角色绑定>& 全部角色绑定() const noexcept;
    const 新方法参数角色绑定* 查询角色(
        稳定编码 参数角色节点) const noexcept;
    const 新方法有序参数引用* 查询参数(std::uint32_t 序号) const noexcept;
    std::optional<新任务虚拟存在读回> 读取绑定任务虚拟存在(
        稳定编码 参数角色节点) const noexcept;
    std::optional<新任务虚拟存在特征组> 读取绑定任务特征组(
        稳定编码 参数角色节点) const noexcept;
    std::optional<新任务虚拟存在特征组> 读取绑定集合任务特征组(
        std::uint32_t 集合参数位置,稳定编码 任务节点) const noexcept;
    std::optional<新方法任务成员绑定> 读取绑定任务成员(std::uint32_t 变量序号) const noexcept;
    新服务要求分项读取结果 读取绑定服务要求分项(
        稳定编码 参数角色节点) const noexcept;
    新服务要求确认读取结果 读取绑定服务要求确认身份(
        稳定编码 参数角色节点) const noexcept;
    新服务要求确认读取结果 复核绑定服务要求确认语义(
        稳定编码 参数角色节点) const noexcept;
    // 运行服务仅接受由其同一材料owner建立的现实执行视图。
    bool 使用服务要求读取接口(
        const 新服务要求材料读取接口&) const noexcept;
    // 只允许读取本参数场景已经绑定的特征；每次调用都从特征owner
    // 取得当前信息，不保存调用前值的副本。
    std::optional<新特征信息> 读取绑定特征(
        稳定编码 特征节点) const noexcept;
    // 只允许读取被某个绑定特征当前值引用的非标量材料节点。
    std::optional<新特征值信息> 读取绑定特征值材料(
        稳定编码 特征值节点) const noexcept;

private:
    friend class 新_方法类;
    方法参数场景(
        稳定编码 方法节点,
        std::vector<新方法参数角色绑定> 角色绑定组,
        const 新_特征类& 特征服务,
        const 新_特征值类& 特征值服务,
        新方法参数场景用途 用途,
        const 新内部治理对象读取接口* 治理对象读取,
        std::shared_ptr<新方法模拟运行接口> 模拟运行 = {},
        const 新服务要求材料读取接口* 服务要求读取 = nullptr,
        std::optional<新任务贡献治理参数> 任务贡献治理参数 = {},
        std::optional<新方法任务成员绑定> 任务成员绑定 = {},
        std::optional<稳定编码> 执行分支 = {},
        const 新_方法类* 方法读取 = nullptr,
        const 新_存在类* 存在读取 = nullptr,
        const 概念_特征类* 特征概念读取 = nullptr,
        std::shared_ptr<新本能需求运行接口> 需求运行 = {},
        const 正式信息参数读取接口* 正式信息读取 = nullptr,
        std::shared_ptr<新本能字段询问运行接口> 字段询问运行 = {},
        const 新_词类* 词条读取 = nullptr) noexcept;

    稳定编码 方法节点_;
    const 正式信息参数读取接口* 正式信息读取_ = nullptr;
    const 新_方法类* 方法读取_ = nullptr;
    const 新_存在类* 存在读取_ = nullptr;
    const 概念_特征类* 特征概念读取_ = nullptr;
    const 新_词类* 词条读取_ = nullptr;
    std::vector<新方法参数角色绑定> 角色绑定组_;
    const 新_特征类* 特征服务_;
    const 新_特征值类* 特征值服务_;
    新方法参数场景用途 用途_;
    // 非拥有引用；调用方保证读取owner覆盖整个参数场景寿命。
    const 新内部治理对象读取接口* 治理对象读取_;
    std::shared_ptr<新方法模拟运行接口> 模拟运行_;
    std::shared_ptr<新本能需求运行接口> 需求运行_;
    std::shared_ptr<新本能字段询问运行接口> 字段询问运行_;
    // 非拥有只读owner，必须覆盖整个参数场景寿命；模拟不得借此访问现实材料。
    const 新服务要求材料读取接口* 服务要求读取_;
    std::optional<新任务贡献治理参数> 任务贡献治理参数_;
    std::optional<新方法任务成员绑定> 任务成员绑定_;
    std::optional<稳定编码> 执行分支节点_;
};

} // namespace 海中鱼巣
