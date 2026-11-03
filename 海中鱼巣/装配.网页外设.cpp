#include "装配.网页外设.h"
#include "核心/容错检查.h"
#include <chrono>
#include <limits>
#include <map>
#include <mutex>
#include <stdexcept>
#include <utility>

namespace 海中鱼巣 {
namespace {

bool 会话格式有效(std::string_view 标识) {
    return !标识.empty() && 新_语言类::严格UTF8字节串有效(std::string(标识));
}

bool 目标格式有效(const 网页对话框目标& 目标) {
    const auto& 标识 = 目标.对话框标识;
    if (!会话格式有效(目标.公开会话标识) || 标识.size() != 36 || 标识[14] != '4'
        || (标识[19] != '8' && 标识[19] != '9' && 标识[19] != 'a' && 标识[19] != 'b'))
        return false;
    for (std::size_t 序号 = 0; 序号 < 标识.size(); ++序号) {
        if (序号 == 8 || 序号 == 13 || 序号 == 18 || 序号 == 23) {
            if (标识[序号] != '-') return false;
        } else if (!((标识[序号] >= '0' && 标识[序号] <= '9')
            || (标识[序号] >= 'a' && 标识[序号] <= 'f'))) return false;
    }
    return true;
}

新状态强时间 取得正式发布时间() {
    const auto 计数 = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    if (计数 < 0 || static_cast<std::uintmax_t>(计数)
        > static_cast<std::uintmax_t>((std::numeric_limits<std::int64_t>::max)()))
        throw std::runtime_error("peripheral publication UTC nanoseconds unavailable");
    return {新状态时间语义::实例绝对UTC, static_cast<std::int64_t>(计数)};
}

bool 事实发布完成(新外设事实状态 状态) noexcept {
    return 状态 == 新外设事实状态::已完成 || 状态 == 新外设事实状态::无变化;
}

网页输出队列回执 转换队列回执(新外设输出队列结果 原,
    const 网页对话框目标& 目标, const 新外设对话框定位& 定位) {
    网页输出队列回执 结果;
    switch (原.状态) {
    case 新外设输出队列状态::已入列: 结果.状态 = 网页输出队列状态::已入列; break;
    case 新外设输出队列状态::已复用: 结果.状态 = 网页输出队列状态::已复用; break;
    case 新外设输出队列状态::已领取: 结果.状态 = 网页输出队列状态::已领取; break;
    case 新外设输出队列状态::已移除: 结果.状态 = 网页输出队列状态::已移除; break;
    case 新外设输出队列状态::已清空: 结果.状态 = 网页输出队列状态::已清空; break;
    case 新外设输出队列状态::队列为空: 结果.状态 = 网页输出队列状态::空; break;
    case 新外设输出队列状态::不在队列: 结果.状态 = 网页输出队列状态::不在队列; break;
    default: throw std::runtime_error("peripheral queue formal operation did not complete");
    }
    const bool 必须有消息 = 原.状态 == 新外设输出队列状态::已入列
        || 原.状态 == 新外设输出队列状态::已复用
        || 原.状态 == 新外设输出队列状态::已领取
        || 原.状态 == 新外设输出队列状态::已移除
        || 原.状态 == 新外设输出队列状态::不在队列;
    if (必须有消息 != 原.消息.has_value())
        throw std::runtime_error("peripheral queue receipt payload inconsistent");
    if (原.消息) {
        auto& 消息 = *原.消息;
        if (消息.来源.方向 != 新外设消息方向::输出
            || 消息.来源.公开宿主会话 != 目标.公开会话标识
            || 消息.来源.对话框标识 != 目标.对话框标识
            || 消息.场景节点 != 定位.场景节点 || 消息.窗口节点 != 定位.窗口节点)
            throw std::runtime_error("peripheral queue receipt identity or text invalid");
        结果.项.emplace(网页输出队列项{目标, 消息.来源.来源序号,
            消息.消息节点.值, std::move(消息.UTF8正文)});
    }
    return 结果;
}

} // namespace

struct 网页外设运行模块::实现 final {
    explicit 实现(新_外设世界树类& 外设世界) : 世界(&外设世界) {}

    // 同锁覆盖全部在途world操作；关闭拿到锁后即可撤销借用。
    std::mutex 互斥;
    新_外设世界树类* 世界;
    bool 就绪 = false, 已关闭 = false;
    std::optional<std::string> 活跃会话;
    std::map<std::string, 新外设对话框定位, std::less<>> 页面目录;

    bool 会话匹配(std::string_view 会话) const noexcept {
        return !已关闭 && 就绪 && 世界 && 活跃会话 && *活跃会话 == 会话;
    }

    const 新外设对话框定位& 建立页面(const 网页对话框目标& 目标) {
        const auto 正式 = 世界->建立或取得对话框场景(目标.公开会话标识, 目标.对话框标识);
        if (!事实发布完成(正式.状态) || !正式.定位
            || !正式.定位->发送数量特征 || !正式.定位->发送内容特征
            || !正式.定位->接收消息特征 || !正式.定位->发送消息特征
            || !正式.定位->输出队列特征)
            throw std::runtime_error("peripheral page formal structure incomplete");
        const auto [位置, 新增] = 页面目录.emplace(目标.对话框标识, *正式.定位);
        if (!新增 && 位置->second != *正式.定位)
            throw std::runtime_error("peripheral registered page identity changed");
        return 位置->second;
    }

    const 新外设对话框定位& 取得页面(const 网页对话框目标& 目标) {
        if (!目标格式有效(目标) || !会话匹配(目标.公开会话标识))
            throw std::runtime_error("peripheral callback outside active session lifetime");
        const auto 页面 = 页面目录.find(目标.对话框标识);
        if (页面 == 页面目录.end())
            throw std::runtime_error("peripheral callback page not registered");
        const auto 正式 = 世界->查找对话框场景(目标.公开会话标识, 目标.对话框标识);
        if (正式.状态 != 新外设事实状态::已完成 || !正式.定位 || *正式.定位 != 页面->second)
            throw std::runtime_error("peripheral registered page formal location unavailable");
        return 页面->second;
    }

    bool 开始会话(std::string_view 标识) noexcept {
        try {
            std::lock_guard 锁(互斥);
            if (已关闭 || !就绪 || !世界 || !会话格式有效(标识)) return false;
            if (活跃会话) return *活跃会话 == 标识;
            活跃会话.emplace(标识);
            return true;
        } catch (const std::exception& 异常) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"开始网页外设会话", L"独立外设会话控制状态建立失败。", 异常.what());
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"开始网页外设会话", L"独立外设会话控制状态建立失败。");
        }
    }

    void 结束会话(std::string_view 标识) noexcept {
        try {
            std::lock_guard 锁(互斥);
            if (!会话匹配(标识)) return;
            页面目录.clear();
            活跃会话.reset();
        } catch (const std::exception& 异常) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"结束网页外设会话", L"独立外设会话控制状态回收失败。", 异常.what());
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"结束网页外设会话", L"独立外设会话控制状态回收失败。");
        }
    }

    bool 确认页面(const 网页对话框目标& 目标) noexcept {
        try {
            std::lock_guard 锁(互斥);
            if (!目标格式有效(目标) || !会话匹配(目标.公开会话标识)) return false;
            (void)建立页面(目标);
            return true;
        } catch (const std::exception& 异常) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"确认网页外设页面", L"正式外设页面建立或读回失败。", 异常.what());
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"确认网页外设页面", L"正式外设页面建立或读回失败。");
        }
    }

    bool 接收消息(std::string_view 会话, std::string_view 页面,
        std::uint64_t 序号, std::string_view 正文) noexcept {
        try {
            std::lock_guard 锁(互斥);
            if (!会话匹配(会话) || !序号 || 正文.empty()
                || !新_语言类::严格UTF8字节串有效(std::string(正文))) return false;
            const 网页对话框目标 目标{std::string(会话), std::string(页面)};
            if (!目标格式有效(目标)) return false;
            const auto& 定位 = 建立页面(目标);
            const auto 时间 = 取得正式发布时间();
            if (!事实发布完成(世界->更新接收(定位, 序号, 正文, 时间)))
                throw std::runtime_error("peripheral received fact publication incomplete");
            return true;
        } catch (const std::exception& 异常) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"接收网页外设事实", L"真实输入发布或正式读回失败，不返回可重发的正常否定。", 异常.what());
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"接收网页外设事实", L"真实输入发布或正式读回失败，不返回可重发的正常否定。");
        }
    }

    void 发送完成(const 网页定向发送事实& 事实) noexcept {
        try {
            std::lock_guard 锁(互斥);
            if (!事实.提交序号 || 事实.UTF8正文.empty()
                || !新_语言类::严格UTF8字节串有效(事实.UTF8正文))
                throw std::runtime_error("peripheral completed send fact malformed");
            const auto& 定位 = 取得页面(事实.目标);
            const auto 时间 = 取得正式发布时间();
            if (!事实发布完成(世界->更新发送(定位, 事实.提交序号, 事实.UTF8正文, 时间)))
                throw std::runtime_error("peripheral completed send publication incomplete");
        } catch (const std::exception& 异常) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"记录网页外设发送事实", L"完整写出后寿命、定位或正式发布失败，保留事实，不再次发送。", 异常.what());
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"记录网页外设发送事实", L"完整写出后寿命、定位或正式发布失败，保留事实，不再次发送。");
        }
    }

    enum class 队列操作 { 入列, 领取, 取消, 清空 };

    网页输出队列回执 操作队列(队列操作 操作, const 网页对话框目标& 目标,
        std::uint64_t 序号 = 0, std::string_view 正文 = {}) noexcept {
        try {
            std::lock_guard 锁(互斥);
            const auto& 定位 = 取得页面(目标);
            新外设输出队列结果 原;
            switch (操作) {
            case 队列操作::入列:
                if (正文.empty() || !新_语言类::严格UTF8字节串有效(std::string(正文)))
                    throw std::runtime_error("peripheral queued output text invalid");
                原 = 世界->提交输出({定位, std::string(正文)});
                if (原.状态 != 新外设输出队列状态::已入列)
                    throw std::runtime_error("peripheral new output identity not published");
                break;
            case 队列操作::领取:
                原 = 世界->领取输出队首(定位);
                break;
            case 队列操作::取消:
                if (!序号) throw std::runtime_error("peripheral cancellation source sequence invalid");
                原 = 世界->移除待发输出(定位, 序号);
                break;
            case 队列操作::清空:
                原 = 世界->清空待发输出(定位);
                break;
            default: throw std::runtime_error("peripheral queue operation invalid");
            }
            return 转换队列回执(std::move(原), 目标, 定位);
        } catch (const std::exception& 异常) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"操作网页外设输出队列", L"正式队列操作、来源或回调寿命不成立，不伪装空队列。", 异常.what());
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"操作网页外设输出队列", L"正式队列操作、来源或回调寿命不成立，不伪装空队列。");
        }
    }
};

网页外设运行模块::网页外设运行模块(新_外设世界树类& 世界)
    : 实现_(std::make_shared<实现>(世界)) {}

网页外设运行模块::~网页外设运行模块() { 关闭(); }

新世界初始化结果 网页外设运行模块::初始化() noexcept {
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->已关闭 || !实现_->世界) return {新世界初始化状态::依赖初始化失败};
        const auto 结果 = 实现_->世界->初始化();
        实现_->就绪 = 结果.状态 == 新世界初始化状态::已完成
            || 结果.状态 == 新世界初始化状态::无变化;
        return 结果;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化网页外设模块", L"外设模块初始化发生工程异常。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化网页外设模块", L"外设模块初始化发生工程异常。");
    }
}

网页外设运行回调组 网页外设运行模块::取得运行回调() {
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->已关闭) return {};
        if (!实现_->就绪 || !实现_->世界)
            throw std::runtime_error("peripheral callbacks requested before initialization");
        const auto 当前 = 实现_;
        网页外设运行回调组 结果;
        结果.会话开始 = [当前](std::string_view 会话) { return 当前->开始会话(会话); };
        结果.会话结束 = [当前](std::string_view 会话) { 当前->结束会话(会话); };
        结果.确认页面 = [当前](const 网页对话框目标& 目标) { return 当前->确认页面(目标); };
        结果.接收消息 = [当前](std::string_view 会话, std::string_view 页面,
            std::uint64_t 序号, std::string_view 正文) { return 当前->接收消息(会话, 页面, 序号, 正文); };
        结果.发送完成 = [当前](const 网页定向发送事实& 事实) { 当前->发送完成(事实); };
        结果.输出队列.入列 = [当前](const 网页输出入列请求& 请求) {
            return 当前->操作队列(实现::队列操作::入列, 请求.目标, 0, 请求.UTF8正文);
        };
        结果.输出队列.领取 = [当前](const 网页对话框目标& 目标) {
            return 当前->操作队列(实现::队列操作::领取, 目标);
        };
        结果.输出队列.取消 = [当前](const 网页对话框目标& 目标, std::uint64_t 序号) {
            return 当前->操作队列(实现::队列操作::取消, 目标, 序号);
        };
        结果.输出队列.清空 = [当前](const 网页对话框目标& 目标) {
            return 当前->操作队列(实现::队列操作::清空, 目标);
        };
        return 结果;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"取得网页外设运行回调", L"模块尚未就绪或回调构造失败。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"取得网页外设运行回调", L"模块尚未就绪或回调构造失败。");
    }
}

void 网页外设运行模块::关闭() noexcept {
    try {
        std::lock_guard 锁(实现_->互斥);
        实现_->已关闭 = true;
        实现_->就绪 = false;
        实现_->页面目录.clear();
        实现_->活跃会话.reset();
        实现_->世界 = nullptr;
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"关闭网页外设模块", L"在途回调收束或world借用解除失败。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"关闭网页外设模块", L"在途回调收束或world借用解除失败。");
    }
}

} // namespace 海中鱼巣
