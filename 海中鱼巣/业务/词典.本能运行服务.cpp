#include "词典.本能运行服务.h"
#include "新_外设世界树类.h"
#include "查询.词典候选.h"
#include "../领域/数据服务.外设词典记录类.h"
#include "../领域/数据服务.词典材料类.h"
#include "../适配/词典.离线数据源.h"
#include "../适配/适配器.UTC时钟.h"
#include "../线程/新_任务管理线程类.h"
#include "../领域/新_本能函数管理类.h"
#include "../核心/容错检查.h"

#include <algorithm>
#include <array>
#include <condition_variable>
#include <limits>
#include <map>
#include <type_traits>
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
    新词典任务执行依赖 执行;
    外设词典记录类 记录{值};
    struct 调用;
    struct 注册项 {
        std::shared_ptr<调用> 上下文;
        std::shared_ptr<const 方法参数场景> 场景;
    };
    std::mutex 注册锁;
    std::map<稳定编码, std::shared_ptr<注册项>> 调用组;
    std::optional<稳定编码> 登记方法;
    std::string 原包标识;
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
    新_方法类& m, 新_特征类& f, 新_特征值类& v, 新_任务管理线程类& t, 新词典任务执行依赖 d)
    : 实现_(std::make_shared<实现>(w,p,m,f,v,t,d)) {}
词典本能运行服务::~词典本能运行服务() = default;

void 词典本能运行服务::初始化来源(const 新词典登记配置& cfg,
    std::shared_ptr<const 离线词典数据源> source, std::string package) {
    auto& s = *实现_;
    实现::租约 lease(s);
    if (!lease.已取得) throw std::logic_error("dictionary source already closed");
    std::lock_guard lock(s.来源锁);
    for (const auto id : 配置身份(cfg))
        if (!有效(id)) throw std::logic_error("dictionary registration configuration missing");
    if (s.配置 && (配置身份(*s.配置) != 配置身份(cfg) || s.来源 != source || s.原包标识 != package))
        throw std::logic_error("dictionary source configuration changed outside assembly");
    if (source && package.empty()) throw std::logic_error("dictionary original package missing");
    s.原包标识 = std::move(package);
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
        lock.unlock();
        decltype(s.调用组) release;
        { std::lock_guard registry(s.注册锁); release.swap(s.调用组); }
        // 外部场景只持上下文，弱状态和关闭门阻止它再进入被借用owner。
        release.clear();
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
        const auto made = s.方法.确保本能方法登记(std::string(新本能函数登记名_查词典), request);
        if (made.方法节点) {
            std::lock_guard lock(s.来源锁);
            if (s.登记方法 && s.登记方法 != made.方法节点)
                throw std::logic_error("dictionary registered method identity changed");
            s.登记方法 = made.方法节点;
        }
        return made;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"登记查词典方法",L"方法登记失败，不预置条件结果。");
    }
}

namespace {
using 运行状态 = 新词典运行状态;
using 材料状态 = 新词典材料操作状态;
using 记录状态 = 新词典记录操作状态;
void 词典要求(bool ok, const char* why) {
    if (!ok) throw std::logic_error(why);
}
bool 词典零发表变化(基础节点组追加状态 s) {
    return s == 基础节点组追加状态::预期不符 || s == 基础节点组追加状态::节点不存在
        || s == 基础节点组追加状态::引用不存在;
}
bool 接收已收尾(const 新词典查询记录信息& r) {
    return r.结局 == 新词典查阅结局::正式未命中 || (r.接收处理
        && (r.接收处理->阶段 == 新词典接收处理阶段::场景已挂接
            || r.接收处理->阶段 == 新词典接收处理阶段::无需动态));
}
}

// 场景和注册项持有调用；调用不回持场景或注册项，避免环及租约释放丢失查询副本。
struct 词典本能运行服务::实现::调用 final : 新本能词典运行接口 {
    std::weak_ptr<实现> 宿主;
    新本能词典任务参数请求 请求;
    std::optional<新方法任务成员绑定> 成员绑定;
    稳定编码 T, D, 原目标, 原目标类型;
    mutable std::mutex 阶段锁;
    std::optional<新词典本能输入> 实际输入;
    std::optional<新本能函数本次条件> 实际条件;
    std::shared_ptr<const 离线词典数据源> 查询源;
    std::shared_ptr<新词典接收场景提供接口> 接收;
    std::optional<新外设词典来源定位> 原来源;
    std::optional<稳定编码> A, 动作场景;
    bool 已绑定 = false;
    std::optional<词典精确查询结果> 查询副本;
    std::optional<新状态强时间> 查询完成时间;
    std::optional<新词典推进收束原因> 收束;

    调用(const std::shared_ptr<实现>& s, const 新本能词典任务参数请求& q,
        const 新任务执行信息& x, const 新需求信息& d)
        : 宿主(s), 请求(q), 成员绑定(x.本能采用->任务成员绑定), T(x.任务节点), D(x.执行服务需求),
          原目标(d.目标存在节点), 原目标类型(d.目标.目标特征类型概念节点) {}

    void 确认场景(const 方法参数场景& scene) const {
        词典要求(scene.用途() == 新方法参数场景用途::执行 && scene.方法节点() == 请求.方法节点
            && scene.全部角色绑定() == 请求.参数绑定组 && scene.执行分支节点() == 请求.执行分支节点,
            "dictionary invocation scene does not belong to this X");
    }
    bool 已请求停止() const noexcept override {
        if (新_本能函数管理类::本次停止已请求()) return true;
        const auto s = 宿主.lock();
        if (!s) return true;
        std::lock_guard lock(s->生命周期锁);
        return s->已关闭;
    }
    struct 输入快照 {
        新词典本能输入读取结果 结果;
        新本能函数本次条件 条件;
        std::shared_ptr<const 离线词典数据源> 来源;
        std::shared_ptr<新词典接收场景提供接口> 接收;
        std::optional<新外设词典来源定位> 外设定位;
    };
    输入快照 读取当前输入(实现& s, const 方法参数场景& scene) const {
        for (;;) {
            输入快照 out;
            if (已请求停止()) { out.结果.状态 = 运行状态::已停止; return out; }
            std::optional<新词典登记配置> cfg;
            std::string package;
            {
                std::lock_guard lock(s.来源锁);
                cfg = s.配置; out.来源 = s.来源; out.接收 = s.接收提供者;
                out.外设定位 = s.外设定位; package = s.原包标识;
            }
            词典要求(cfg.has_value(), "dictionary registration configuration unavailable");
            std::array<const 新方法有序参数引用*, 6> p{};
            const std::array kinds{新方法参数信息类型::词条, 新方法参数信息类型::特征,
                新方法参数信息类型::存在, 新方法参数信息类型::存在,
                新方法参数信息类型::特征, 新方法参数信息类型::特征};
            词典要求(scene.全部角色绑定().size() == p.size(), "dictionary requires six ordered parameters");
            for (std::size_t i = 0; i < p.size(); ++i) {
                p[i] = scene.查询参数(static_cast<std::uint32_t>(i + 1));
                词典要求(p[i] && p[i]->信息类型 == kinds[i] && 有效(p[i]->信息节点),
                    "dictionary ordered parameter shape invalid");
                out.结果.变化依赖节点组.push_back(p[i]->信息节点);
            }
            if (const auto current = s.世界.读取自我所在场景()) out.结果.变化依赖节点组.push_back(*current);
            const auto 缺口 = [&](新词典学习缺口种类 kind, std::uint32_t position,
                std::optional<稳定编码> type = {}) {
                out.结果.状态 = 运行状态::条件未具备;
                out.结果.缺口组.push_back({kind, position, p[position-1]->信息节点, type, {}});
            };
            if (!out.来源 || !out.外设定位 || !out.接收) {
                缺口(新词典学习缺口种类::来源不可用, 3); return out;
            }
            const auto word = scene.读取词条参数挂接(1);
            if (word.状态 == 新方法词条参数读取状态::资源失败) throw std::bad_alloc{};
            if (word.状态 == 新方法词条参数读取状态::参数已失效) {
                out.结果.状态 = 运行状态::需重新组织; return out;
            }
            词典要求(word.状态 == 新方法词条参数读取状态::已读取 && word.信息,
                "dictionary formal word reader unavailable");
            const auto f = scene.读取存在参数特征(1, cfg->材料特征类型);
            const auto batch = scene.读取存在参数特征(3, cfg->来源包特征类型);
            const auto location = scene.读取存在参数特征(3, cfg->外设场景标识类型);
            const std::array fields{&f, &batch, &location};
            if (std::any_of(fields.begin(), fields.end(), [](const auto* r) {
                return r->状态 == 新方法参数特征读取状态::定义前提已变化;
            })) {
                std::this_thread::yield();
                continue;
            }
            const std::array positions{2u, 5u, 6u};
            const std::array types{cfg->材料特征类型, cfg->来源包特征类型, cfg->外设场景标识类型};
            for (std::size_t i = 0; i < fields.size(); ++i) {
                const auto& r = *fields[i];
                if (r.状态 == 新方法参数特征读取状态::特征未取得) {
                    缺口(新词典学习缺口种类::参数特征未形成, positions[i], types[i]); return out;
                }
                词典要求((r.状态 == 新方法参数特征读取状态::已读取
                    || r.状态 == 新方法参数特征读取状态::当前值未取得) && r.特征,
                    "dictionary parameter feature reader unavailable");
                if (r.特征->节点 != p[positions[i]-1]->信息节点) {
                    out.结果.状态 = 运行状态::需重新组织; return out;
                }
                if (i != 0 && !r.特征->当前值) {
                    缺口(新词典学习缺口种类::参数当前值未取得, positions[i], types[i]); return out;
                }
            }
            const auto batchV = std::get_if<稳定编码>(&*batch.特征->当前值);
            const auto C = std::get_if<std::int64_t>(&*location.特征->当前值);
            词典要求(batchV && C && *C > 0 && batch.当前完整材料,
                "dictionary source batch or peripheral location malformed");
            const auto text = std::get_if<std::string>(&batch.当前完整材料->材料);
            词典要求(text && !package.empty(), "dictionary source batch is not UTF8 or package missing");
            if (A && 实际输入 && (*text != 实际输入->批次摘要
                || static_cast<std::uint64_t>(*C) != 实际输入->外设场景标识)) {
                out.结果.状态 = 运行状态::需重新组织; return out;
            }
            if (*text != out.外设定位->可信批次标识
                || static_cast<std::uint64_t>(*C) != out.外设定位->场景节点.值) {
                // 没有A时是当前缺来源；有A时由推进者按固定输入决定收束。
                缺口(新词典学习缺口种类::来源不可用, 3); return out;
            }
            const auto lang = s.执行.语言.获取语言(word.信息->词条.语言节点);
            if (lang.状态 == 新语言查询状态::资源失败) throw std::bad_alloc{};
            词典要求(lang.状态 == 新语言查询状态::已找到 && lang.信息,
                "dictionary word language unavailable");
            auto key = 新词典查询键{word.信息->词条.语言节点, word.信息->词条.规范化合同节点,
                word.信息->词条.规范化词面, word.信息->词条.规范化词面, {}};
            if (lang.信息->登记键 == "zh-CN") {
                key.源语种 = "zh";
                const auto normalized = out.来源->繁简规范化(key.原规范化词面);
                词典要求(normalized.has_value(), "trusted dictionary normalization unavailable");
                key.查询词面 = *normalized;
            } else if (lang.信息->登记键 == "en") key.源语种 = "en";
            else { 缺口(新词典学习缺口种类::来源不可用, 1); return out; }
            out.结果.输入 = 新词典本能输入{p[0]->信息节点, p[1]->信息节点, p[2]->信息节点,
                p[3]->信息节点, p[4]->信息节点, p[5]->信息节点, std::move(key), *text, package,
                static_cast<std::uint64_t>(*C)};
            out.条件.执行分支节点 = scene.执行分支节点();
            for (const auto& binding : scene.全部角色绑定()) {
                新本能函数条件角色材料 row;
                row.参数角色节点 = binding.参数角色节点;
                row.世界场景节点 = binding.世界场景节点;
                row.有序引用 = binding.有序引用;
                for (std::size_t i = 0; i < positions.size(); ++i)
                    if (binding.有序引用->序号 == positions[i])
                        row.实际特征组.push_back({fields[i]->特征->节点, fields[i]->特征->当前值});
                out.条件.参数角色组.push_back(std::move(row));
            }
            out.结果.状态 = 运行状态::已就绪;
            return out;
        }
    }
    新词典本能输入读取结果 读取查阅输入(const 方法参数场景& scene) const override {
        const auto s = 宿主.lock();
        if (!s) return {运行状态::已停止, {}, {}, {}};
        租约 lease(*s);
        if (!lease.已取得) return {运行状态::已停止, {}, {}, {}};
        std::lock_guard lock(阶段锁);
        确认场景(scene);
        // 已经发生的动作使用原输入；后续来源变化只影响新的实际查询。
        if (A && 实际输入) return {运行状态::已就绪, 实际输入, {}, {}};
        return 读取当前输入(*s, scene).结果;
    }
    // 当前治理只决定后续工作，不拿当前F反证已经取得的回执。
    std::optional<新词典推进收束原因> 读取治理收束(实现& s) const {
        const auto x = s.执行.任务.获取执行实例(请求.执行实例节点);
        if (!x || x->状态 != 新任务执行状态::执行中) return 新词典推进收束原因::需求不再活动;
        if (x->任务节点 != T || x->方法节点 != 请求.方法节点 || x->执行服务需求 != D
            || !x->本能采用 || x->本能采用->参数绑定组 != 请求.参数绑定组
            || x->本能采用->执行分支节点 != 请求.执行分支节点
            || x->本能采用->任务成员绑定 != 成员绑定)
            return 新词典推进收束原因::输入身份已更换;
        const auto d = s.执行.需求.获取需求(D);
        if (!d || d->活动状态 == 新需求活动状态::失活) return 新词典推进收束原因::需求不再活动;
        if (d->目标存在节点 != 原目标 || d->目标.目标特征类型概念节点 != 原目标类型)
            return 新词典推进收束原因::输入身份已更换;
        for (;;) {
            const auto result = s.执行.需求.复核当前满足(D);
            if (result.状态 == 新需求当前满足状态::目标引用前提已变化) {
                if (已请求停止()) return 新词典推进收束原因::主动停止;
                std::this_thread::yield(); continue;
            }
            if (result.状态 == 新需求当前满足状态::资源失败) throw std::bad_alloc{};
            if (result.状态 == 新需求当前满足状态::已满足) return 新词典推进收束原因::当前目标已满足;
            if (result.状态 == 新需求当前满足状态::需求不存在) return 新词典推进收束原因::需求不再活动;
            词典要求(result.状态 != 新需求当前满足状态::结构不一致
                && result.状态 != 新需求当前满足状态::读取接口未装配
                && result.状态 != 新需求当前满足状态::入口拒绝,
                "dictionary task target read failed");
            return {};
        }
    }
    新词典查阅动作结果 动作结果(运行状态 state,
        std::vector<新词典学习缺口> gaps = {}, std::vector<稳定编码> dependencies = {}) const {
        return {state, A, A ? 实际条件 : std::nullopt, A ? 实际输入 : std::nullopt,
            std::move(gaps), std::move(dependencies)};
    }
    新词典查阅动作结果 开始或取得查阅动作(const 方法参数场景& scene) override {
        const auto s = 宿主.lock();
        if (!s) { std::lock_guard lock(阶段锁); 收束 = 新词典推进收束原因::主动停止;
            return 动作结果(运行状态::已停止); }
        租约 lease(*s);
        std::lock_guard lock(阶段锁);
        确认场景(scene);
        if (!lease.已取得 || 已请求停止()) {
            收束 = 新词典推进收束原因::主动停止;
            return 动作结果(运行状态::已停止);
        }
        if (收束) return 动作结果(运行状态::需重新组织);
        if (已绑定) return 动作结果(运行状态::已就绪);
        while (!A) {
            if (const auto why = 读取治理收束(*s)) { 收束 = why; return 动作结果(运行状态::需重新组织); }
            if (!s->方法.获取方法(请求.方法节点)) {
                收束 = 新词典推进收束原因::输入身份已更换;
                return 动作结果(运行状态::需重新组织);
            }
            auto input = 读取当前输入(*s, scene);
            if (input.结果.状态 != 运行状态::已就绪) {
                if (input.结果.状态 == 运行状态::已停止) 收束 = 新词典推进收束原因::主动停止;
                return 动作结果(input.结果.状态, std::move(input.结果.缺口组), std::move(input.结果.变化依赖节点组));
            }
            const auto prior = s->执行.动态.按来源查询动作(请求.执行实例节点);
            if (prior.状态 == 新动态操作状态::资源失败) throw std::bad_alloc{};
            if (prior.状态 == 新动态操作状态::入口拒绝) {
                if (const auto why = 读取治理收束(*s)) {
                    收束 = why; return 动作结果(运行状态::需重新组织);
                }
            }
            词典要求(prior.状态 == 新动态操作状态::已完成 && !prior.动作,
                "dictionary X has an action but its original call context is missing");
            const auto actorScene = s->执行.场景.查询所属场景(input.结果.输入->执行主体);
            if (!actorScene) {
                收束 = 新词典推进收束原因::输入身份已更换;
                return 动作结果(运行状态::需重新组织);
            }
            // 全部可分配材料先保存；A发表后不再用后来值覆盖历史条件。
            实际输入 = std::move(input.结果.输入); 实际条件 = std::move(input.条件);
            查询源 = std::move(input.来源); 接收 = std::move(input.接收); 原来源 = std::move(input.外设定位);
            动作场景 = actorScene;
            const auto made = s->执行.动态.建立词条动作动态({实际输入->执行主体, 请求.方法节点,
                请求.执行实例节点, 实际输入->词条, 实际输入->材料特征, 来源建立时间()});
            if (made.动态节点) A = made.动态节点;
            if (made.状态 == 新动态操作状态::资源失败) throw std::bad_alloc{};
            if (!A && made.状态 == 新动态操作状态::动作前提已变化) {
                实际输入.reset(); 实际条件.reset(); 动作场景.reset();
                查询源.reset(); 接收.reset(); 原来源.reset();
                std::this_thread::yield();
                continue;
            }
            词典要求(made.状态 == 新动态操作状态::已完成 && A,
                "dictionary action could not be established");
        }
        词典要求(实际输入 && 实际条件 && 动作场景, "dictionary action actual materials missing");
        const auto attached = s->执行.场景.添加动态(*动作场景, *A);
        if (attached == 新场景操作状态::资源失败) throw std::bad_alloc{};
        if (attached != 新场景操作状态::已完成 && attached != 新场景操作状态::无变化) {
            if (s->执行.场景.查询所属场景(实际输入->执行主体) != 动作场景
                || attached == 新场景操作状态::动态不存在 || attached == 新场景操作状态::场景不存在) {
                收束 = 新词典推进收束原因::输入身份已更换;
                return 动作结果(运行状态::需重新组织);
            }
            throw std::logic_error("dictionary action scene attachment failed");
        }
        const auto bound = s->执行.任务.记录本能动作身份(T, 请求.执行实例节点, *A, s->执行.动态);
        if (bound == 新任务操作状态::资源失败) throw std::bad_alloc{};
        if (bound == 新任务操作状态::任务状态不允许 || bound == 新任务操作状态::任务不存在
            || bound == 新任务操作状态::执行实例不存在) {
            收束 = 读取治理收束(*s).value_or(新词典推进收束原因::输入身份已更换);
            return 动作结果(运行状态::需重新组织);
        }
        词典要求(bound == 新任务操作状态::已修改 || bound == 新任务操作状态::精确重复,
            "dictionary action binding failed");
        已绑定 = true;
        return 动作结果(运行状态::已就绪);
    }
    std::optional<新本能词典读取回执> 读取回执(实现& s) const {
        const auto r = s.执行.任务.读取执行词典回执(T, 请求.执行实例节点);
        if (r.状态 == 材料状态::资源失败) throw std::bad_alloc{};
        词典要求(r.状态 == 材料状态::已完成 || r.状态 == 材料状态::未找到
            || r.状态 == 材料状态::条件不具备, "dictionary X receipt reader unavailable");
        return r.回执;
    }
    新词典查阅推进结果 推进结果(运行状态 state,
        std::optional<新本能词典读取回执> receipt = {}) const {
        return {state, A, std::move(receipt), {}, {}, 收束};
    }
    新词典查阅推进结果 推进本次查阅(const 方法参数场景& scene) override;
};

新词典查阅推进结果 词典本能运行服务::实现::调用::推进本次查阅(const 方法参数场景& scene) {
    const auto s = 宿主.lock();
    if (!s) { std::lock_guard lock(阶段锁); 收束 = 新词典推进收束原因::主动停止;
        return 推进结果(运行状态::已停止); }
    租约 lease(*s);
    std::lock_guard lock(阶段锁);
    确认场景(scene);
    if (!lease.已取得) { 收束 = 新词典推进收束原因::主动停止; return 推进结果(运行状态::已停止); }
    词典要求(A && 实际输入 && 实际条件, "dictionary continuation lacks actual action input");
    for (;;) {
        auto receipt = 读取回执(*s);
        if (receipt && receipt->采用阶段 == 新词典材料采用阶段::已采用) {
            // 先完成既有采用的责任；后来目标或F变化不撤销、重放它。
            const auto done = s->执行.任务.续接执行词典变化(T, 请求.执行实例节点);
            if (done.状态 == 材料状态::资源失败) throw std::bad_alloc{};
            词典要求(done.状态 == 材料状态::已完成 && done.回执,
                "dictionary adopted change continuation failed");
            return 推进结果(运行状态::已收束, done.回执);
        }
        if (receipt && receipt->查阅结局 == 新词典查阅结局::正式未命中)
            return 推进结果(运行状态::已收束, std::move(receipt));
        if (已请求停止()) { 收束 = 新词典推进收束原因::主动停止; return 推进结果(运行状态::已停止, receipt); }
        if (!收束) 收束 = 读取治理收束(*s);
        if (收束) return 推进结果(运行状态::已收束, receipt);
        词典要求(已绑定 && 查询源 && 原来源 && 接收, "dictionary query attempted before action binding");
        const auto& input = *实际输入;
        // W/F后来更换是当前目标身份退出；来源后来变化则不否定已有副本。
        const auto w = scene.读取词条参数挂接(1);
        if (w.状态 == 新方法词条参数读取状态::资源失败) throw std::bad_alloc{};
        if (w.状态 == 新方法词条参数读取状态::参数已失效
            || (w.信息 && std::none_of(w.信息->特征组.begin(), w.信息->特征组.end(),
                [&](const auto& f) { return f.特征节点 == input.材料特征; }))) {
            收束 = 新词典推进收束原因::输入身份已更换;
            return 推进结果(运行状态::已收束, receipt);
        }
        词典要求(w.状态 == 新方法词条参数读取状态::已读取 && w.信息,
            "dictionary continuation word reader unavailable");
        if (!receipt) {
            const 新词典查询定位 origin{原来源->场景节点, A->值};
            auto r = s->记录.按调用来源读取查询记录(origin);
            if (r.状态 == 记录状态::资源失败) throw std::bad_alloc{};
            词典要求(r.状态 == 记录状态::已完成 || r.状态 == 记录状态::未找到,
                "dictionary query record reader unavailable");
            if (!r.信息) {
                if (!查询副本) {
                    auto current = 读取当前输入(*s, scene);
                    if (current.结果.状态 == 运行状态::需重新组织
                        || (current.结果.输入 && current.结果.输入->键 != input.键)) {
                        收束 = 新词典推进收束原因::输入身份已更换;
                        return 推进结果(运行状态::已收束);
                    }
                    if (current.结果.状态 != 运行状态::已就绪) {
                        if (current.结果.状态 == 运行状态::已停止) 收束 = 新词典推进收束原因::主动停止;
                        return {current.结果.状态, A, {}, std::move(current.结果.缺口组),
                            std::move(current.结果.变化依赖节点组), 收束};
                    }
                    auto result = 词典查询服务{查询源}.查询词义({input.键.源语种, input.键.查询词面});
                    if (result.状态 == 词典操作状态::资源失败) throw std::bad_alloc{};
                    词典要求(result.状态 == 词典操作状态::已完成 || result.状态 == 词典操作状态::未命中,
                        "dictionary query failed; not a formal miss");
                    static_assert(std::is_nothrow_move_constructible_v<词典精确查询结果>);
                    查询副本.emplace(std::move(result));
                    查询完成时间 = 来源建立时间();
                }
                if (已请求停止()) {
                    收束 = 新词典推进收束原因::主动停止;
                    return 推进结果(运行状态::已停止);
                }
                词典要求(查询完成时间.has_value(), "dictionary completed query has no recorded completion time");
                const 新词典查询事实 facts{origin, 原来源->词典说话者, input.键.源语种,
                    input.键.查询词面, input.批次摘要, input.原始包标识, *查询完成时间,
                    查询副本->状态 == 词典操作状态::未命中 ? 新词典查阅结局::正式未命中 : 新词典查阅结局::命中,
                    查询副本->义项组};
                auto prepared = s->记录.准备查询记录(facts, 接收);
                if (prepared.状态 == 记录状态::资源失败) throw std::bad_alloc{};
                if (prepared.状态 == 记录状态::已复用) {
                    词典要求(prepared.既有记录.has_value(), "dictionary reused R missing");
                    r.信息 = std::move(prepared.既有记录);
                } else {
                    词典要求(prepared.状态 == 记录状态::已完成 && prepared.准备,
                        "dictionary receive preparation failed");
                    // 准备绑定本线程。即使返回资源失败也可能已经发表，取消口只清未发表预留。
                    struct 查询清理 {
                        外设词典记录类& owner; 新词典查询准备& p;
                        ~查询清理() { owner.取消未发表查询(p); }
                    } cleanup{s->记录, *prepared.准备};
                    const auto published = s->记录.提交查询接收(*prepared.准备);
                    if (published.状态 == 记录状态::资源失败) throw std::bad_alloc{};
                    词典要求((published.状态 == 记录状态::已完成 || published.状态 == 记录状态::尚需续接)
                        && published.信息, "dictionary R publication failed");
                    return 推进结果(运行状态::已推进);
                }
            }
            词典要求(r.信息.has_value(), "dictionary R missing after read");
            if (!接收已收尾(*r.信息)) {
                const auto continued = s->记录.续接查询变化(r.信息->查询记录, *接收);
                if (continued.状态 == 记录状态::资源失败) throw std::bad_alloc{};
                // B仍保留责任；正常已计数的其它保持不会触发零计数删除冲突。
                词典要求(continued.状态 != 记录状态::仍被引用,
                    "dictionary receive release found references after state count reached zero; B retained");
                词典要求(continued.状态 == 记录状态::已完成 && continued.信息
                    && 接收已收尾(*continued.信息), "dictionary receive continuation did not finish");
                return 推进结果(运行状态::已推进);
            }
            const 新词典执行结果来源 source{input.词条, input.材料特征,
                {origin.外设场景.值, r.信息->查询记录.值}};
            if (r.信息->结局 == 新词典查阅结局::正式未命中) {
                const auto recorded = s->执行.任务.记录执行词典未命中(T, 请求.执行实例节点, source);
                if (recorded == 材料状态::资源失败) throw std::bad_alloc{};
                if (recorded == 材料状态::条件不具备 || recorded == 材料状态::未找到) {
                    收束 = 读取治理收束(*s);
                    词典要求(收束.has_value(), "dictionary miss receipt rejected active X");
                    return 推进结果(运行状态::已收束);
                }
                词典要求(recorded == 材料状态::已完成 || recorded == 材料状态::已复用,
                    "dictionary miss receipt publication failed");
                return 推进结果(运行状态::已收束, 读取回执(*s));
            }
            auto participant = s->执行.任务.准备执行词典材料参与(T, 请求.执行实例节点, source);
            if (participant.状态 == 材料状态::资源失败) throw std::bad_alloc{};
            if (participant.已有回执) continue;
            if (participant.状态 == 材料状态::条件不具备 || participant.状态 == 材料状态::未找到) {
                收束 = 读取治理收束(*s);
                词典要求(收束.has_value(), "dictionary Q participant rejected active X");
                return 推进结果(运行状态::已收束);
            }
            词典要求(participant.状态 == 材料状态::已完成 && participant.准备,
                "dictionary Q participant unavailable");
            struct 参与清理 {
                新词典结果参与准备& p; bool 完成 = false;
                ~参与清理() { if (!完成) p.取消未发表准备(); }
            } cleanup{*participant.准备};
            新词典内部材料建立请求 request{s->执行.材料组织, input.来源存在, s->执行.释义语言,
                input.键, input.批次摘要, input.原始包标识, source.查询来源, {}};
            request.义项组.reserve(r.信息->有序义项消息.size());
            for (const auto M : r.信息->有序义项消息) {
                auto item = s->记录.读取义项消息(M);
                if (item.状态 == 记录状态::资源失败) throw std::bad_alloc{};
                词典要求(item.状态 == 记录状态::已完成 && item.信息,
                    "dictionary R references unreadable sense message");
                request.义项组.push_back({M.值, std::move(item.信息->材料)});
            }
            auto material = s->执行.材料.准备内部材料(request);
            if (material.状态 == 材料状态::资源失败) throw std::bad_alloc{};
            词典要求((material.状态 == 材料状态::已完成 || material.状态 == 材料状态::已复用)
                && material.准备, "dictionary Q preparation unavailable");
            const auto done = s->执行.材料.提交内部材料首次持有(*material.准备, *participant.准备);
            if (done.状态 != 基础节点组追加状态::已完成) {
                s->执行.材料.取消内部材料准备(*material.准备);
                if (词典零发表变化(done.状态)) { std::this_thread::yield(); continue; }
                if (done.状态 == 基础节点组追加状态::资源失败) throw std::bad_alloc{};
                throw std::logic_error("dictionary Q first holding failed");
            }
            // 提交已经登记双方回执；成功后先结束全部发表责任，不响应中途停止。
            cleanup.完成 = true;
            const auto completed = s->执行.材料.完成内部材料发表(*material.准备);
            participant.准备->完成共同发表();
            词典要求(completed.材料节点.has_value(), "dictionary Q completion missing published identity");
            return 推进结果(运行状态::已推进, 读取回执(*s));
        }
        词典要求(receipt->材料Q.has_value(), "dictionary pending adoption has no Q");
        auto word = s->执行.词条.准备词典材料采用(input.词条, input.材料特征, *receipt->材料Q);
        if (word.状态 == 材料状态::资源失败) throw std::bad_alloc{};
        if (word.状态 == 材料状态::未找到 || word.状态 == 材料状态::输入拒绝) {
            // 在新一次循环开头重读W/F；稳定不可适用不能无限重提同一准备。
            const auto current = s->执行.词条.读取词条特征信息(input.词条);
            if (current.状态 == 新词条查询状态::资源失败) throw std::bad_alloc{};
            if (current.状态 == 新词条查询状态::未找到 || (current.信息
                && std::none_of(current.信息->特征组.begin(), current.信息->特征组.end(),
                    [&](const auto& f) { return f.特征节点 == input.材料特征; }))) {
                收束 = 新词典推进收束原因::输入身份已更换;
                return 推进结果(运行状态::已收束, receipt);
            }
            throw std::logic_error("dictionary stable W/F cannot prepare adoption");
        }
        词典要求(word.状态 == 材料状态::已完成 && word.准备, "dictionary word adoption unavailable");
        const auto projection = s->执行.词条.读取词典材料采用投影(*word.准备);
        auto participant = s->执行.任务.准备执行词典采用参与(T, 请求.执行实例节点,
            *receipt->材料Q, projection.前状态);
        if (participant.已有回执) { s->执行.词条.取消词典材料采用准备(*word.准备); continue; }
        if (participant.状态 == 材料状态::资源失败) throw std::bad_alloc{};
        if (participant.状态 == 材料状态::条件不具备 || participant.状态 == 材料状态::未找到) {
            s->执行.词条.取消词典材料采用准备(*word.准备);
            收束 = 读取治理收束(*s);
            词典要求(收束.has_value(), "dictionary adoption participant rejected active X");
            return 推进结果(运行状态::已收束, receipt);
        }
        词典要求(participant.状态 == 材料状态::已完成 && participant.准备,
            "dictionary adoption participant unavailable");
        const auto adopted = s->执行.词条.提交词典材料采用(*word.准备, *participant.准备);
        if (adopted.状态 != 基础节点组追加状态::已完成) {
            s->执行.词条.取消词典材料采用准备(*word.准备);
            participant.准备->取消未发表准备();
            if (词典零发表变化(adopted.状态)) { std::this_thread::yield(); continue; }
            if (adopted.状态 == 基础节点组追加状态::资源失败) throw std::bad_alloc{};
            throw std::logic_error("dictionary word adoption publication failed");
        }
        // 词条提供者已完成X参与，外层不得二次完成或重复命中。
        return 推进结果(运行状态::已推进, 读取回执(*s));
    }
}

bool 词典本能运行服务::支持任务方法(稳定编码 M) const noexcept {
    try {
        auto& s = *实现_; 实现::租约 lease(s);
        if (!lease.已取得) return false;
        std::lock_guard lock(s.来源锁);
        return s.登记方法 == std::optional{M};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"识别词典任务方法", L"词典工厂方法身份读取失败。");
    }
}

新词典任务参数场景结果 词典本能运行服务::建立任务参数场景(
    const 新本能词典任务参数请求& request, const 新内部治理对象读取接口* governance,
    std::optional<新方法任务成员绑定> member) {
    auto& s = *实现_; 实现::租约 lease(s);
    if (!lease.已取得) return {运行状态::已停止, {}, {}, {}};
    { std::lock_guard lock(s.来源锁);
      if (s.登记方法 != std::optional{request.方法节点}) return {运行状态::需重新组织, {}, {}, {}}; }
    词典要求(governance == static_cast<const 新内部治理对象读取接口*>(&s.执行.任务),
        "dictionary factory governance reader differs");
    std::shared_ptr<实现::注册项> entry;
    { std::lock_guard lock(s.注册锁); const auto it = s.调用组.find(request.执行实例节点);
      if (it != s.调用组.end()) entry = it->second; }
    if (!entry) {
        const auto x = s.执行.任务.获取执行实例(request.执行实例节点);
        if (!x || x->状态 != 新任务执行状态::执行中) return {运行状态::需重新组织, {}, {}, {}};
        词典要求(x->本能采用 && x->方法节点 == request.方法节点
            && x->本能采用->来源方法节点 == request.方法节点
            && x->本能采用->参数绑定组 == request.参数绑定组
            && x->本能采用->执行分支节点 == request.执行分支节点
            && x->本能采用->任务成员绑定 == member,
            "dictionary factory request differs from X adoption");
        // 注册锁不跨owner读取；另一调用可能已在上述读取期间登记并推进到A。
        { std::lock_guard lock(s.注册锁); const auto it = s.调用组.find(request.执行实例节点);
          if (it != s.调用组.end()) entry = it->second; }
        if (!entry) {
            词典要求(!x->普通动作节点, "dictionary X action lost its original context");
            const auto d = s.执行.需求.获取需求(x->执行服务需求);
            if (!d) return {运行状态::需重新组织, {}, {}, {}};
            auto candidate = std::make_shared<实现::注册项>();
            candidate->上下文 = std::make_shared<实现::调用>(实现_, request, *x, *d);
            { std::lock_guard lock(s.注册锁);
              entry = s.调用组.emplace(request.执行实例节点, std::move(candidate)).first->second; }
        }
    }
    std::lock_guard phase(entry->上下文->阶段锁);
    auto& context = *entry->上下文;
    词典要求(context.请求.方法节点 == request.方法节点 && context.请求.参数绑定组 == request.参数绑定组
        && context.请求.执行分支节点 == request.执行分支节点 && context.成员绑定 == member,
        "dictionary existing X cannot change ordered invocation identity");
    if (context.已请求停止()) return {运行状态::已停止, {}, {}, {}};
    if (entry->场景) return {运行状态::已就绪, entry->场景, {}, {}};
    for (;;) {
        if (context.已请求停止()) return {运行状态::已停止, {}, {}, {}};
        const auto made = s.方法.建立参数场景(request.方法节点, request.参数绑定组,
            governance, nullptr, member, request.执行分支节点, {}, entry->上下文);
        if (made.参数场景) {
            entry->场景 = made.参数场景;
            return {运行状态::已就绪, entry->场景, {}, {}};
        }
        if (made.状态 == 新方法操作状态::定义前提已变化) { std::this_thread::yield(); continue; }
        if (made.状态 == 新方法操作状态::资源失败) throw std::bad_alloc{};
        if (made.状态 == 新方法操作状态::参数材料未到) {
            std::vector<稳定编码> dependencies;
            for (const auto& b : request.参数绑定组) if (b.有序引用) dependencies.push_back(b.有序引用->信息节点);
            return {运行状态::条件未具备, {}, {{新词典学习缺口种类::方法条件未具备, 0, {}, {}, {}}},
                std::move(dependencies)};
        }
        if (made.状态 == 新方法操作状态::方法不存在 || made.状态 == 新方法操作状态::参数引用失效
            || made.状态 == 新方法操作状态::参数概念不相容)
            return {运行状态::需重新组织, {}, {}, {}};
        throw std::logic_error("dictionary method parameter scene construction failed");
    }
}

新词典任务参数场景结果 词典本能运行服务::取得任务续接参数场景(稳定编码 X) {
    auto& s = *实现_; 实现::租约 lease(s);
    if (!lease.已取得) return {运行状态::已停止, {}, {}, {}};
    std::shared_ptr<实现::注册项> entry;
    { std::lock_guard lock(s.注册锁); const auto it = s.调用组.find(X);
      if (it != s.调用组.end()) entry = it->second; }
    词典要求(entry != nullptr, "dictionary continuation lost original X context");
    std::lock_guard phase(entry->上下文->阶段锁);
    词典要求(entry->场景 != nullptr, "dictionary continuation scene was never established");
    return {运行状态::已就绪, entry->场景, {}, {}};
}

新词典推进收束读取结果 词典本能运行服务::读取任务推进收束(稳定编码 X) const noexcept {
    using S = 新词典推进收束读取状态;
    try {
        auto& s = *实现_; 实现::租约 lease(s);
        if (!lease.已取得) return {S::上下文不存在, {}};
        std::shared_ptr<实现::注册项> entry;
        { std::lock_guard lock(s.注册锁); const auto it = s.调用组.find(X);
          if (it != s.调用组.end()) entry = it->second; }
        if (!entry) return {S::上下文不存在, {}};
        std::lock_guard phase(entry->上下文->阶段锁);
        const auto& c = *entry->上下文;
        if (!c.收束) return {S::尚未收束, {}};
        return {S::已读取, 新词典推进收束材料{X, *c.收束, c.读取回执(s)}};
    } catch (const std::bad_alloc&) { return {S::资源失败, {}}; }
    catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取词典推进收束", L"词典执行收束读取失败。");
    }
}

} // namespace 海中鱼巣
