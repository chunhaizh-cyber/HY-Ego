#pragma once
#include <string>
#include "../领域/新_任务类.h"
#include "../领域/新_需求类.h"
#include "../领域/模拟需求.数据.h"

namespace 海中鱼巣 {
class 结构化服务要求服务;
enum class 新模拟业务来源状态 : std::uint8_t {
    已读取=1, 来源不存在, 来源已变化, 尚缺依赖提供者
};
enum class 新模拟业务引用类别 : std::uint8_t {
    存在=1, 特征, 特征值, 任务, 任务虚拟存在, 需求,
    方法, 方法实例, 执行组织, 执行实例, 影响评估, 完成结算,
    需求承接, 服务要求确认, 语言理解, 服务要求, 状态,
    其它待识别实例, 候选方法材料
};
struct 新模拟业务待展开引用 final {
    新模拟业务引用类别 类别;
    稳定编码 来源身份;
    std::string 字段角色;
};
struct 新模拟任务来源投影 final {
    新任务信息 任务;
    新任务虚拟存在信息 虚拟存在;
    新任务虚拟存在特征组 治理特征;
    std::vector<新模拟业务待展开引用> 实例引用组;
};
struct 新模拟需求来源投影 final {
    新需求治理信息 治理;
    std::optional<新需求信息> 完整需求;
    std::vector<新模拟业务待展开引用> 实例引用组;
};
template<class T> struct 新模拟业务来源读回 final {
    新模拟业务来源状态 状态=新模拟业务来源状态::来源不存在;
    std::optional<T> 信息;
    std::string 诊断;
    std::vector<基础联合节点快照> 完整保护;
};
// 仅导出值副本和待展开引用，不授予现实写权，也不交付可执行模拟对象。
class 模拟任务需求来源读取接口 {
public:
    virtual ~模拟任务需求来源读取接口() = default;
    virtual 新模拟业务来源读回<新模拟任务来源投影> 读取任务来源(稳定编码) const = 0;
    virtual 新模拟业务来源读回<新模拟需求来源投影> 读取需求来源(稳定编码) const = 0;
    virtual 新模拟需求来源结果<服务要求理解信息> 读取模拟理解来源(稳定编码) const = 0;
    virtual 新模拟需求来源结果<服务要求确认信息> 读取模拟确认来源(稳定编码) const = 0;
    virtual 新模拟需求来源结果<新部分服务要求材料> 读取模拟要求来源(稳定编码) const = 0;
    virtual 新模拟需求来源结果<新模拟需求构造依据> 读取模拟构造依据() const = 0;
    virtual 新自然语言场景复制结果 读取语言场景来源(稳定编码) const = 0;
};
class 模拟任务需求来源服务 final : public 模拟任务需求来源读取接口 {
public:
    模拟任务需求来源服务(const 新_任务类&,const 新_需求类&,
        const 结构化服务要求服务&,const 新_自然语言世界树类&) noexcept;
    新模拟业务来源读回<新模拟任务来源投影> 读取任务来源(稳定编码) const override;
    新模拟业务来源读回<新模拟需求来源投影> 读取需求来源(稳定编码) const override;
    新模拟需求来源结果<服务要求理解信息> 读取模拟理解来源(稳定编码) const override;
    新模拟需求来源结果<服务要求确认信息> 读取模拟确认来源(稳定编码) const override;
    新模拟需求来源结果<新部分服务要求材料> 读取模拟要求来源(稳定编码) const override;
    新模拟需求来源结果<新模拟需求构造依据> 读取模拟构造依据() const override;
    新自然语言场景复制结果 读取语言场景来源(稳定编码) const override;
private:
    const 新_任务类& 任务_;
    const 新_需求类& 需求_;
    const 结构化服务要求服务& 服务要求_;
    const 新_自然语言世界树类& 语言_;
};
}
