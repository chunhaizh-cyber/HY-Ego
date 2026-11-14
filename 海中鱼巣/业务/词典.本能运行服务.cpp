#include "词典.本能运行服务.h"
#include "新_外设世界树类.h"
#include "../适配/词典.离线数据源.h"
#include "../适配/适配器.UTC时钟.h"
#include "../线程/新_任务管理线程类.h"
#include "../领域/新_本能函数管理类.h"
#include "../核心/容错检查.h"

#include <algorithm>
#include <array>
#include <condition_variable>
#include <limits>
#include <mutex>
#include <stdexcept>
#include <thread>

namespace 海中鱼巣 {
namespace {
auto 配置身份(const 新词典登记配置& c) {
    return std::array{c.词条全域,c.词典材料全域,c.通用存在范围,
        c.材料特征类型,c.来源包特征类型,c.外设场景标识类型};
}
新状态强时间 来源建立时间() {
    const auto now = UTC时钟适配器{}.读取当前UTC时间证据_v1({});
    if (!now.成功()) throw std::runtime_error("dictionary source UTC unavailable");
    return {新状态时间语义::实例绝对UTC, now.证据->UTC纳秒};
}
}

struct 词典本能运行服务::实现 final {
    新_世界树类& 世界;
    新_外设世界树类& 外设;
    新_方法类& 方法;
    新_特征类& 特征;
    新_特征值类& 值;
    新_任务管理线程类& 管理;
    mutable std::mutex 生命周期锁;
    std::condition_variable 空闲;
    std::mutex 来源锁;
    bool 已关闭 = false;
    std::size_t 活动 = 0;
    std::optional<新词典登记配置> 配置;
    std::shared_ptr<const 离线词典数据源> 来源;
    std::optional<新外设词典来源定位> 外设定位;
    std::shared_ptr<新词典接收场景提供接口> 接收提供者;
    struct 入口 {
        稳定编码 场景, 存在, 定位F;
        std::optional<稳定编码> 批次F;
        bool 可用 = false;
    };
    std::optional<入口> 当前绑定;

    struct 租约 {
        实现& s;
        bool 已取得 = false;
        explicit 租约(实现& v) : s(v) {
            std::lock_guard lock(s.生命周期锁);
            if (!s.已关闭) { ++s.活动; 已取得 = true; }
        }
        ~租约() {
            if (!已取得) return;
            std::lock_guard lock(s.生命周期锁);
            --s.活动; s.空闲.notify_all();
        }
    };

    // 返回当次正式定位；节点正常退出时重读，不比较整组旧快照。
    std::optional<入口> 查找入口(稳定编码 S, std::int64_t C, const 新词典登记配置& cfg) {
        for (;;) {
            { std::lock_guard stateLock(生命周期锁); if (已关闭) return {}; }
            const auto scene = 世界.查询场景(S);
            if (!scene) {
                if (世界.读取自我所在场景() != std::optional{S}) return {};
                throw std::runtime_error("dictionary current scene unreadable");
            }
            std::optional<入口> found;
            bool refresh = false;
            for (const auto E : scene->直接成员存在组) {
                const auto e = 世界.查询存在(E);
                if (!e) {
                    const auto current = 世界.查询场景(S);
                    if (!current || std::find(current->直接成员存在组.begin(), current->直接成员存在组.end(), E)
                        == current->直接成员存在组.end()) { refresh = true; break; }
                    throw std::runtime_error("dictionary scene still contains missing member");
                }
                std::optional<稳定编码> batchF, locationF;
                for (const auto F : e->特征节点组) {
                    const auto type = 特征.读取当前特征类型(F);
                    if (type.状态 == 新特征类型读取状态::特征不存在) {
                        const auto current = 世界.查询存在(E);
                        if (!current || std::find(current->特征节点组.begin(), current->特征节点组.end(), F)
                            == current->特征节点组.end()) { refresh = true; break; }
                        throw std::runtime_error("dictionary member still contains missing feature");
                    }
                    if (type.状态 == 新特征类型读取状态::资源失败) throw std::bad_alloc{};
                    if (type.状态 != 新特征类型读取状态::已读取 || !type.特征类型根)
                        throw std::runtime_error("dictionary source feature type unavailable");
                    auto save = [&](std::optional<稳定编码>& target) {
                        if (target) throw std::runtime_error("dictionary source duplicate feature type");
                        target = F;
                    };
                    if (*type.特征类型根 == cfg.来源包特征类型) save(batchF);
                    if (*type.特征类型根 == cfg.外设场景标识类型) save(locationF);
                }
                if (refresh) break;
                if (!locationF) continue;
                const auto location = 特征.获取特征(*locationF);
                if (!location) { refresh = true; break; }
                if (!location->当前值) continue;
                const auto number = std::get_if<std::int64_t>(&*location->当前值);
                if (!number) throw std::runtime_error("dictionary source location is not I64");
                if (*number != C) continue;
                if (found) throw std::runtime_error("dictionary scene has duplicate source entrances");
                入口 item{S,E,*locationF,batchF,false};
                const auto batch = batchF ? 特征.获取特征(*batchF) : std::nullopt;
                if (batchF && !batch) { refresh = true; break; }
                if (batch && batch->当前值) {
                    const auto V = std::get_if<稳定编码>(&*batch->当前值);
                    if (!V) throw std::runtime_error("dictionary source batch is not material reference");
                    const auto value = 值.获取特征值(*V);
                    if (!value) {
                        const auto latest = 特征.获取特征(*batchF);
                        if (!latest || latest->当前值 != batch->当前值) { refresh = true; break; }
                        throw std::runtime_error("dictionary source batch refers to missing material");
                    }
                    const auto text = std::get_if<std::string>(&value->材料);
                    if (!text || value->物理类型 != 材料物理类型::UTF8字符串)
                        throw std::runtime_error("dictionary source batch material is not UTF8");
                    item.可用 = *text == 外设定位->可信批次标识;
                }
                found = item;
            }
            if (!refresh) return found;
            std::this_thread::yield();
        }
    }
};

词典本能运行服务::词典本能运行服务(新_世界树类& w, 新_外设世界树类& p,
    新_方法类& m, 新_特征类& f, 新_特征值类& v, 新_任务管理线程类& t)
    : 实现_(std::make_unique<实现>(w,p,m,f,v,t)) {}
词典本能运行服务::~词典本能运行服务() = default;

void 词典本能运行服务::初始化来源(const 新词典登记配置& cfg,
    std::shared_ptr<const 离线词典数据源> source) {
    auto& s = *实现_;
    实现::租约 lease(s);
    if (!lease.已取得) throw std::logic_error("dictionary source already closed");
    std::lock_guard lock(s.来源锁);
    for (const auto id : 配置身份(cfg))
        if (!有效(id)) throw std::logic_error("dictionary registration configuration missing");
    if (s.配置 && (配置身份(*s.配置) != 配置身份(cfg) || s.来源 != source))
        throw std::logic_error("dictionary source configuration changed outside assembly");
    s.配置 = cfg;
    s.来源 = std::move(source);
    if (!s.来源) return;
    const auto batch = s.来源->读取可信批次标识();
    if (!batch || batch->empty()) throw std::logic_error("dictionary source is not a trusted batch");
    const auto location = s.外设.建立或取得词典来源场景(*batch);
    if (location.状态 == 新外设事实状态::资源失败) throw std::bad_alloc{};
    if ((location.状态 != 新外设事实状态::已完成 && location.状态 != 新外设事实状态::无变化)
        || !location.定位) throw std::runtime_error("dictionary peripheral source assembly incomplete");
    s.外设定位 = location.定位;
    if (!s.接收提供者) s.接收提供者 = s.外设.创建词典接收场景提供者();
    if (!s.接收提供者) throw std::bad_alloc{};
}

新词典运行状态 词典本能运行服务::保障当前场景词典入口() {
    auto& s = *实现_;
    实现::租约 lease(s);
    if (!lease.已取得) return 新词典运行状态::已停止;
    for (;;) {
        { std::lock_guard stateLock(s.生命周期锁); if (s.已关闭) return 新词典运行状态::已停止; }
        std::unique_lock lock(s.来源锁);
        const auto S = s.世界.读取自我所在场景();
        if (!S || !s.来源) return 新词典运行状态::条件未具备;
        if (!s.配置 || !s.外设定位) throw std::logic_error("dictionary source assembly missing");
        const auto C = s.外设定位->场景节点.值;
        if (!C || C > static_cast<std::uint64_t>((std::numeric_limits<std::int64_t>::max)()))
            throw std::runtime_error("dictionary source location cannot be represented as I64");
        auto entry = s.查找入口(*S, static_cast<std::int64_t>(C), *s.配置);
        { std::lock_guard stateLock(s.生命周期锁); if (s.已关闭) return 新词典运行状态::已停止; }
        if (!entry) {
            if (s.世界.读取自我所在场景() != S) { lock.unlock(); std::this_thread::yield(); continue; }
            const auto V = s.值.保存或取得特征值(材料物理类型::UTF8字符串, s.外设定位->可信批次标识);
            if (!有效(V)) throw std::bad_alloc{};
            const 新存在建立请求 request{std::nullopt, {
                {s.配置->来源包特征类型, V},
                {s.配置->外设场景标识类型, static_cast<std::int64_t>(C)}}};
            const auto made = s.世界.建立场景内存在(*S, request, 来源建立时间());
            // E一旦存在便不能重建。部分发表保留现场走工程处理，不伪装完整入口。
            if (made.存在节点 && made.状态 != 新场景操作状态::已完成)
                throw std::runtime_error("dictionary entrance published but initial state incomplete");
            if (!made.存在节点 && (made.状态 == 新场景操作状态::存在前提已变化
                || made.状态 == 新场景操作状态::存在特征联合更新占用)) {
                lock.unlock(); std::this_thread::yield(); continue;
            }
            if (made.状态 == 新场景操作状态::资源失败) throw std::bad_alloc{};
            if (made.状态 == 新场景操作状态::场景不存在 && s.世界.读取自我所在场景() != S) {
                lock.unlock(); std::this_thread::yield(); continue;
            }
            if (made.状态 != 新场景操作状态::已完成 || !made.存在节点)
                throw std::runtime_error("dictionary entrance creation failed");
            entry = s.查找入口(*S, static_cast<std::int64_t>(C), *s.配置);
            { std::lock_guard stateLock(s.生命周期锁); if (s.已关闭) return 新词典运行状态::已停止; }
            if (!entry) {
                // 完整发表之后入口也可能正常退出或失去本来源定位；重新读当前场景，
                // 不把后来的变化改写为本次发表失败，也不撤销已发生事实。
                lock.unlock(); std::this_thread::yield(); continue;
            }
        }
        s.当前绑定 = entry;
        const auto ready = entry->可用;
        std::vector<稳定编码> nodes{entry->场景,entry->存在,entry->定位F};
        if (entry->批次F) nodes.push_back(*entry->批次F);
        lock.unlock();
        const auto accepted = s.管理.提交词典来源就绪通知(nodes);
        if (accepted == 新任务结果状态接收状态::资源失败) throw std::bad_alloc{};
        if (accepted == 新任务结果状态接收状态::内部错误)
            throw std::runtime_error("dictionary source notification unavailable");
        if (s.世界.读取自我所在场景() == S)
            return ready ? 新词典运行状态::已就绪 : 新词典运行状态::条件未具备;
        // 保障期间迁移：保留旧入口，再处理最新场景；不搬移、不撤销。
        std::this_thread::yield();
    }
}

void 词典本能运行服务::接收自我场景就绪() noexcept {
    try { (void)保障当前场景词典入口(); }
    catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"保障词典来源入口",L"来源入口处理失败，保留已发表事实。",e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"保障词典来源入口",L"来源入口处理发生未知工程错误。");
    }
}

void 词典本能运行服务::关闭() noexcept {
    try {
        auto& s = *实现_;
        std::unique_lock lock(s.生命周期锁);
        s.已关闭 = true;
        s.空闲.wait(lock,[&] { return s.活动 == 0; });
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"关闭词典运行服务",L"在途调用未能正常回收。");
    }
}

std::optional<新词典登记配置> 词典本能运行服务::读取登记材料() const noexcept {
    try {
        auto& s = *实现_;
        实现::租约 lease(s);
        if (!lease.已取得) return {};
        std::lock_guard lock(s.来源锁);
        return s.配置;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取词典登记材料",L"登记材料读取失败。");
    }
}

新方法建立结果 词典本能运行服务::确保方法登记(const 新方法知识建立请求& request) noexcept {
    try {
        auto& s = *实现_;
        实现::租约 lease(s);
        if (!lease.已取得 || !request.条件结果组.empty())
            throw std::logic_error("dictionary registration closed or contains preset pairs");
        return s.方法.确保本能方法登记(std::string(新本能函数登记名_查词典), request);
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"登记查词典方法",L"方法登记失败，不预置条件结果。");
    }
}

} // namespace 海中鱼巣
