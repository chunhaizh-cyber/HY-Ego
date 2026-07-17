// 文件规则：结构事务接线只承载传统仓库可见的稳定值式边界，不承载锁行为或业务事实。
#pragma once

#include <cstdint>
#include <memory>
#include <utility>

namespace 海中鱼巣 {

enum class 结构许可类型 : std::uint8_t {
    无效,
    共享,
    独占
};

struct 结构事务令牌 {
    std::uint64_t 域编号 = 0;
    std::uint64_t 运行期纪元 = 0;
    std::uint64_t 许可序号 = 0;
    结构许可类型 类型 = 结构许可类型::无效;
};

class 结构事务许可 {
public:
    结构事务许可() = default;
    结构事务许可(const 结构事务许可&) = delete;
    结构事务许可& operator=(const 结构事务许可&) = delete;
    结构事务许可(结构事务许可&& 其它) noexcept { *this = std::move(其它); }
    结构事务许可& operator=(结构事务许可&& 其它) noexcept {
        if (this != &其它) {
            释放();
            运行期状态_ = std::move(其它.运行期状态_);
            令牌_ = 其它.令牌_;
            释放许可_ = 其它.释放许可_;
            其它.令牌_ = {};
            其它.释放许可_ = nullptr;
        }
        return *this;
    }
    ~结构事务许可() { 释放(); }
    bool 有效() const { return 运行期状态_ && 令牌_.域编号 != 0 && 释放许可_ != nullptr; }
    const 结构事务令牌& 读取令牌() const { return 令牌_; }
    static 结构事务许可 创建(
        std::shared_ptr<void> 运行期状态,
        结构事务令牌 令牌,
        void (*释放许可)(const std::shared_ptr<void>&, const 结构事务令牌&) noexcept) {
        结构事务许可 结果;
        结果.运行期状态_ = std::move(运行期状态);
        结果.令牌_ = 令牌;
        结果.释放许可_ = 释放许可;
        return 结果;
    }

private:
    friend struct 结构事务接线;
    std::shared_ptr<void> 运行期状态_;
    结构事务令牌 令牌_;
    void (*释放许可_)(const std::shared_ptr<void>&, const 结构事务令牌&) noexcept = nullptr;
    void 释放() noexcept { if (有效()) { 释放许可_(运行期状态_, 令牌_); 令牌_ = {}; 释放许可_ = nullptr; 运行期状态_.reset(); } }
};

struct 结构事务接线 {
    std::uint64_t 域编号 = 0;
    std::uint64_t 运行期纪元 = 0;
    std::shared_ptr<void> 运行期状态;
    结构事务许可 (*取得共享许可)(const std::shared_ptr<void>&) = nullptr;
    结构事务许可 (*取得独占许可)(const std::shared_ptr<void>&) = nullptr;
    bool (*验证共享路径令牌)(const std::shared_ptr<void>&, const 结构事务令牌&) = nullptr;
    bool (*验证独占令牌)(const std::shared_ptr<void>&, const 结构事务令牌&) = nullptr;
    bool (*标记撤销失败隔离)(const std::shared_ptr<void>&, const 结构事务令牌&) = nullptr;

    bool 已接域() const {
        return 域编号 != 0 && 运行期纪元 != 0 && 运行期状态 && 取得共享许可 && 取得独占许可
            && 验证共享路径令牌 && 验证独占令牌 && 标记撤销失败隔离;
    }
};

}
