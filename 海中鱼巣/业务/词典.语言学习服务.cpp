#include "词典.语言学习服务.h"
#include "解释.词典释义.h"
#include "../领域/数据服务.词典材料类.h"
#include "../领域/新_任务类.h"
#include "../领域/新_需求类.h"
#include "../领域/新_词类.h"
#include "../领域/新_特征类.h"
#include "../领域/新_自然语言世界树类.h"

#include <algorithm>
#include <limits>
#include <map>
#include <new>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

namespace 海中鱼巣 {
namespace {
std::int64_t 词典语言位置(std::size_t 位置) {
    if (位置 > static_cast<std::size_t>((std::numeric_limits<std::int64_t>::max)()))
        throw std::length_error("词典语言原文位置不可表示");
    return static_cast<std::int64_t>(位置);
}

std::size_t 词典原文标量数(std::string_view 原文) {
    return static_cast<std::size_t>(std::count_if(原文.begin(), 原文.end(), [](unsigned char 字节) {
        return (字节 & 0xc0) != 0x80;
    }));
}

新词典语言词段 词典语言词段(const 词典释义词段& 词段) {
    return {词典语言位置(词段.起始标量), 词典语言位置(词段.结束标量), 词段.规范化词面,
        词段.是词头 ? 新词典词段依据::同批词头表 : 新词典词段依据::单标量回退};
}
} // namespace

struct 词典语言学习服务::实现 final {
    新_任务类& 任务;
    新_需求类& 需求;
    新_词类& 词;
    新_特征类& 特征;
    新_语言类& 语言;
    词典材料类& 材料;
    新_自然语言世界树类& 世界;
    std::filesystem::path 词典根;
    稳定编码 材料类型;
    std::chrono::milliseconds 重试间隔;
    std::map<std::pair<std::string, std::string>, std::shared_ptr<const 离线词典数据源>> 源缓存;

    struct 请求者 final {
        稳定编码 T, D, W;
        bool 已交付 = false;
    };
    struct 原文行 final {
        稳定编码 I;
        std::string 记录键, 原文;
        std::int64_t 行号 = 0, 起始 = 0, 结束 = 0;
    };
    struct 工作 final {
        稳定编码 Q;
        std::vector<请求者> 请求者组;
        std::optional<新词典材料信息> 信息;
        std::shared_ptr<const 离线词典数据源> 源;
        稳定编码 来源场景;
        std::vector<原文行> 行组;
        std::size_t 下一个行 = 0;
        std::vector<稳定编码> 已处理行组;
        bool 已完成 = false;
        std::chrono::steady_clock::time_point 可推进时点{};
    };
    std::map<稳定编码, 工作> 工作组;

    void 接纳(稳定编码 T, 稳定编码 D, 稳定编码 W, 稳定编码 Q) {
        auto [位置, 新增] = 工作组.try_emplace(Q);
        auto& 项 = 位置->second;
        if (新增) 项.Q = Q;
        const auto 已有 = std::find_if(项.请求者组.begin(), 项.请求者组.end(), [&](const auto& 人) {
            return 人.T == T && 人.D == D && 人.W == W;
        });
        if (已有 == 项.请求者组.end()) {
            项.请求者组.push_back({T, D, W});
            项.可推进时点 = std::chrono::steady_clock::now();
        }
    }

    std::shared_ptr<const 离线词典数据源> 取得源(const 新词典材料信息& Q) {
        const auto 键 = std::pair{Q.可信批次标识, Q.原包标识};
        if (const auto 已有 = 源缓存.find(键); 已有 != 源缓存.end()) return 已有->second;
        const auto 打开 = 离线词典数据源::打开指定批次(词典根, Q.可信批次标识);
        if (打开.状态 == 词典操作状态::未配置) return {};
        if (打开.状态 == 词典操作状态::资源失败) throw std::bad_alloc{};
        if (打开.状态 != 词典操作状态::已完成 || !打开.数据源)
            throw std::runtime_error(打开.诊断);
        if (打开.包标识 != Q.原包标识)
            throw std::logic_error("词典语言材料与指定批次原包身份不一致");
        源缓存.emplace(键, 打开.数据源);
        return 打开.数据源;
    }

    void 准备原文行(工作& 项) {
        for (const auto& 义项 : 项.信息->义项组) {
            const std::string_view 全文 = 义项.内容.汉语释义;
            std::size_t 标量起点 = 0;
            std::int64_t 行号 = 0;
            for (std::size_t 起点 = 0; 起点 <= 全文.size(); ++行号) {
                const auto 换行 = 全文.find('\n', 起点);
                const auto 终点 = 换行 == std::string_view::npos ? 全文.size() : 换行;
                const auto 行 = 全文.substr(起点, 终点 - 起点);
                const auto 标量数 = 词典原文标量数(行);
                if (行.find_first_not_of(" \t\r") != std::string_view::npos)
                    项.行组.push_back({义项.节点, 义项.内容.记录键, std::string(行), 行号,
                        词典语言位置(标量起点), 词典语言位置(标量起点 + 标量数)});
                if (换行 == std::string_view::npos) break;
                标量起点 += 标量数 + 1;
                起点 = 换行 + 1;
            }
        }
    }

    enum class 准备状态 { 已就绪, 等待材料, 来源已退出 };
    准备状态 准备工作(工作& 项) {
        if (!项.信息) {
            if (语言.读取词典语言材料组(项.Q)) {
                项.已完成 = true;
                return 准备状态::已就绪;
            }
            const auto Q = 材料.读取材料(项.Q);
            if (Q.状态 == 新词典材料操作状态::资源失败) throw std::bad_alloc{};
            if (Q.状态 == 新词典材料操作状态::未找到) return 准备状态::来源已退出;
            if (!Q.信息) throw std::logic_error("正式词典语言来源读取失败");
            项.信息 = *Q.信息;
        }
        if (!项.源) 项.源 = 取得源(*项.信息);
        if (!项.源) return 准备状态::等待材料;
        if (!项.来源场景.值) {
            const auto 场景 = 世界.建立或取得词典来源场景(项.Q);
            if (场景.状态 == 新来源子场景状态::资源失败) throw std::bad_alloc{};
            if (场景.状态 == 新来源子场景状态::前提已变化 || 场景.状态 == 新来源子场景状态::未建立)
                return 准备状态::等待材料;
            if (!场景.信息) throw std::logic_error("词典来源语言场景建立失败");
            项.来源场景 = 场景.信息->场景节点;
            准备原文行(项);
        }
        return 准备状态::已就绪;
    }

    bool 保存下一行(工作& 项) {
        const auto& 行 = 项.行组.at(项.下一个行);
        std::optional<新词条信息> X;
        for (const auto& 人 : 项.请求者组) {
            const auto 当前 = 词.获取词条(人.W);
            if (当前.状态 == 新词条查询状态::资源失败) throw std::bad_alloc{};
            if (当前.信息) { X = *当前.信息; break; }
        }
        if (!X) return false;
        const auto 解析 = 解读词典释义行(*项.源, X->规范化词面, 行.原文,
            项.源->读取记录词性(行.记录键));
        if (解析.状态 == 词典释义解读状态::输入不合法
            || 解析.状态 == 词典释义解读状态::辅助表未配置)
            throw std::logic_error("已准入词典材料不能使用当前同批释义解析器");

        新词典归类材料定义 定义;
        定义.材料Q = 项.Q;
        定义.义项I = 行.I;
        定义.来源场景 = 项.来源场景;
        定义.说话者 = 项.信息->内部词典来源;
        定义.释义语言 = 项.信息->释义语言;
        定义.词条X = X->节点;
        定义.原行号 = 行.行号;
        定义.原文起始标量 = 行.起始;
        定义.原文结束标量 = 行.结束;
        定义.释义原文 = 解析.释义原文;
        定义.规范化释义 = 解析.规范化释义;
        定义.状态 = 解析.状态 == 词典释义解读状态::已解读
            ? 新词典语言理解状态::已理解 : 新词典语言理解状态::缺规则;
        if (解析.结构) 定义.结构编号 = static_cast<std::int64_t>(*解析.结构);
        for (const auto& 词段 : 解析.词段组) {
            定义.词段组.push_back(词典语言词段(词段));
            if (!词段.是词头) continue;
            const auto 准入 = 词.建立或取得词条({定义.释义语言, 严格UTF8字节精确规范化合同节点,
                词段.规范化词面, 行.I});
            if (准入.状态 == 新词条建立状态::资源失败) throw std::bad_alloc{};
            if (!准入.词条节点) throw std::logic_error("已确认词典词头的正式词条准入失败");
            if (解析.中心词 && 词段.起始标量 == 解析.中心词->起始标量
                && 词段.结束标量 == 解析.中心词->结束标量) 定义.词条N = *准入.词条节点;
        }
        if (解析.中心词) 定义.中心词 = 词典语言词段(*解析.中心词);
        if (解析.修饰起始标量) 定义.修饰起始标量 = 词典语言位置(*解析.修饰起始标量);
        if (解析.修饰结束标量) 定义.修饰结束标量 = 词典语言位置(*解析.修饰结束标量);
        定义.诊断 = 解析.诊断;
        const auto 保存 = 语言.保存词典归类材料(定义);
        if (!保存) return false;
        项.已处理行组.push_back(保存->节点);
        ++项.下一个行;
        return true;
    }
};

词典语言学习服务::词典语言学习服务(新_任务类& T, 新_需求类& D, 新_词类& W, 新_特征类& F,
    新_语言类& L, 词典材料类& Q, 新_自然语言世界树类& 世界,
    std::filesystem::path 根, std::shared_ptr<const 离线词典数据源> 启动源,
    std::string 包, 稳定编码 材料类型, std::chrono::milliseconds 间隔)
    : 实现_(std::make_unique<实现>(T, D, W, F, L, Q, 世界, std::move(根), 材料类型, 间隔)) {
    if (启动源) {
        if (const auto 批次 = 启动源->读取可信批次标识())
            实现_->源缓存.emplace(std::pair{*批次, std::move(包)}, std::move(启动源));
    }
}

词典语言学习服务::~词典语言学习服务() = default;

void 词典语言学习服务::接纳当前任务(稳定编码 T) {
    新当前满足需求结果 当前;
    for (;;) {
        当前 = 实现_->任务.读取当前满足需求(T);
        if (当前.状态 == 新当前满足需求状态::资源失败) throw std::bad_alloc{};
        if (当前.状态 == 新当前满足需求状态::无可服务需求) return;
        if (当前.状态 == 新当前满足需求状态::已选定) break;
        if (当前.状态 != 新当前满足需求状态::前提已变化) {
            if (!实现_->任务.获取任务(T)) return;
            throw std::logic_error("词典语言发现不能读取当前任务需求");
        }
        std::this_thread::yield();
    }
    const auto D = 实现_->需求.获取需求(当前.需求节点);
    if (!D) return;
    const auto F = 实现_->词.按类型读取词条特征(D->目标存在节点, 实现_->材料类型);
    if (F.状态 == 新词条查询状态::资源失败) throw std::bad_alloc{};
    if (!F.特征) return;
    const auto 特征 = 实现_->特征.获取特征(F.特征->特征节点);
    if (!特征 || !特征->当前值) return;
    if (const auto* Q = std::get_if<稳定编码>(&*特征->当前值))
        实现_->接纳(T, D->节点, D->目标存在节点, *Q);
}

void 词典语言学习服务::接纳执行材料(稳定编码 T, 稳定编码 X) {
    const auto 读 = 实现_->任务.读取执行词典回执(T, X);
    if (读.状态 == 新词典材料操作状态::资源失败) throw std::bad_alloc{};
    if (!读.回执) return;
    const auto& 回执 = *读.回执;
    if (回执.查阅结局 != 新词典查阅结局::命中 || 回执.采用阶段 != 新词典材料采用阶段::已采用
        || !回执.材料Q || 回执.变化阶段 == 新词典变化处理阶段::待处理) return;
    const auto 执行 = 实现_->任务.获取执行实例(X);
    if (执行) 实现_->接纳(T, 执行->执行服务需求, 回执.词条, *回执.材料Q);
}

std::optional<std::chrono::steady_clock::time_point> 词典语言学习服务::下一推进时点() const noexcept {
    std::optional<std::chrono::steady_clock::time_point> 结果;
    for (const auto& [Q, 项] : 实现_->工作组) {
        (void)Q;
        if (项.已完成 && std::all_of(项.请求者组.begin(), 项.请求者组.end(),
            [](const auto& 人) { return 人.已交付; })) continue;
        if (!结果 || 项.可推进时点 < *结果) 结果 = 项.可推进时点;
    }
    return 结果;
}

std::optional<新词典语言交付> 词典语言学习服务::推进一次(std::stop_token 停止) {
    if (停止.stop_requested()) return std::nullopt;
    const auto 现在 = std::chrono::steady_clock::now();
    for (auto 位置 = 实现_->工作组.begin(); 位置 != 实现_->工作组.end(); ++位置) {
        auto& 项 = 位置->second;
        if (项.可推进时点 > 现在) continue;
        if (!项.已完成) {
            const auto 准备 = 实现_->准备工作(项);
            if (准备 == 实现::准备状态::来源已退出) {
                实现_->工作组.erase(位置);
                return std::nullopt;
            }
            if (准备 == 实现::准备状态::等待材料) {
                项.信息.reset();
                项.可推进时点 = 现在 + 实现_->重试间隔;
                continue;
            }
            if (停止.stop_requested()) return std::nullopt;
            if (项.下一个行 < 项.行组.size()) {
                if (!实现_->保存下一行(项)) 项.可推进时点 = 现在 + 实现_->重试间隔;
                return std::nullopt;
            }
            if (!项.已完成) {
                if (!实现_->语言.发表词典语言材料组(项.Q, 项.来源场景, 项.已处理行组)) {
                    项.信息.reset();
                    项.来源场景 = {};
                    项.行组.clear();
                    项.下一个行 = 0;
                    项.已处理行组.clear();
                    项.可推进时点 = 现在 + 实现_->重试间隔;
                    return std::nullopt;
                }
                项.已完成 = true;
            }
            项.信息.reset();
            项.行组.clear();
            项.已处理行组.clear();
        }
        for (auto& 人 : 项.请求者组) {
            if (人.已交付) continue;
            人.已交付 = true;
            return 新词典语言交付{人.T, 人.D, 人.W, 项.Q};
        }
    }
    return std::nullopt;
}

} // namespace 海中鱼巣
