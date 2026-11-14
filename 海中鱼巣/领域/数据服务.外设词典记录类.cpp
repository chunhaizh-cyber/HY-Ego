#include "数据服务.外设词典记录类.h"
#include "新_特征值类.h"
#include "../核心/容错检查.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <condition_variable>
#include <limits>
#include <map>
#include <mutex>
#include <set>
#include <shared_mutex>
#include <stdexcept>
#include <thread>
#include <tuple>
#include <type_traits>

namespace 海中鱼巣 {
namespace {
using S = 新词典记录操作状态;
using L = 基础节点组追加状态;
[[noreturn]] void 记录故障(const wchar_t* 原因) noexcept {
    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"外设词典记录", 原因);
}
std::mutex 记录互斥;
std::condition_variable 记录空闲;
std::shared_mutex 记录读取退出互斥;
std::atomic<bool> 记录结构就绪{false};
稳定编码 查询类型, 消息类型, 续接类型;
using 查询键 = std::pair<稳定编码, std::uint64_t>;
std::map<查询键, 稳定编码> 查询索引;
std::map<稳定编码, std::pair<稳定编码, std::size_t>> 消息索引;
std::map<稳定编码, 稳定编码> 续接索引;
std::map<查询键, std::thread::id> 活动查询;

struct 查询占用 final {
    查询键 键;
    explicit 查询占用(查询键 k) : 键(k) {
        std::unique_lock lock(记录互斥);
        if (const auto i = 活动查询.find(k); i != 活动查询.end()
            && i->second == std::this_thread::get_id())
            记录故障(L"同线程不能重入同次词典查询");
        记录空闲.wait(lock, [&] { return !活动查询.contains(k); });
        活动查询.emplace(k, std::this_thread::get_id());
    }
    ~查询占用() noexcept {
        std::lock_guard lock(记录互斥);
        活动查询.erase(键);
        记录空闲.notify_all();
    }
};
基础联合节点快照 读取记录快照(稳定编码 n) {
    const auto r = 全局基础数据集.查询节点组完整结构({n});
    if (r.状态 == 基础结构操作状态::资源失败) throw S::资源失败;
    if (r.状态 == 基础结构操作状态::节点不存在) throw S::未找到;
    if (r.状态 != 基础结构操作状态::已完成 || r.节点组.size() != 1)
        记录故障(L"当前记录结构读取失败");
    if (!r.节点组.front().快照) throw S::未找到;
    return *r.节点组.front().快照;
}
const 基础字段关系* 查字段(const 基础节点结构& n, 稳定编码 key) {
    const 基础字段关系* found = nullptr;
    for (const auto& f : n.字段组) if (f.字段节点 == key) {
        if (found) 记录故障(L"记录字段重复");
        found = &f;
    }
    return found;
}
template<class T> T 解码字段(const 基础字段关系& f) {
    if constexpr (std::is_same_v<T, 稳定编码>) {
        const auto* v = std::get_if<稳定编码>(&f.内容);
        if (!v || !有效(*v)) 记录故障(L"记录引用字段损坏");
        return *v;
    } else {
        const auto* m = std::get_if<基础值>(&f.内容);
        if (!m) 记录故障(L"记录直接值字段损坏");
        if constexpr (std::is_same_v<T, std::uint64_t>) {
            const auto* v = std::get_if<std::vector<std::uint64_t>>(&m->材料);
            if (!v || v->size() != 1) 记录故障(L"调用标识字段损坏");
            return v->front();
        } else {
            const auto* v = std::get_if<T>(&m->材料);
            if (!v) 记录故障(L"记录字段物理类型损坏");
            return *v;
        }
    }
}
template<class T> T 读字段(const 基础节点结构& n, 稳定编码 key) {
    const auto* f = 查字段(n, key);
    if (!f) 记录故障(L"记录必需字段缺失");
    return 解码字段<T>(*f);
}
template<class T> 基础组内字段内容 编码字段(const T& v) {
    if constexpr (std::is_same_v<T, 稳定编码>) return v;
    else if constexpr (std::is_same_v<T, std::uint64_t>) return 基础值{std::vector<std::uint64_t>{v}};
    else return 基础值{v};
}
void 替换准备字段(基础组内新节点& n, 稳定编码 key,
    std::optional<基础组内字段内容> value) {
    const auto i = std::find_if(n.字段组.begin(), n.字段组.end(),
        [&](const auto& f) { return f.字段节点 == key; });
    if (!value) { if (i != n.字段组.end()) n.字段组.erase(i); }
    else if (i == n.字段组.end()) n.字段组.push_back({key, std::move(*value)});
    else i->内容 = std::move(*value);
}
struct 查询身份 final { 稳定编码 节点; };
struct 查询字段投影 final { const 基础节点结构& 结构; };
struct 查询节点准备 final { 基础组内新节点 节点; };
稳定编码 查询类型字段;
[[maybe_unused]] void 构造新增查询类型(查询节点准备& 准备, const 稳定编码& 值) { 准备.节点.字段组.push_back({查询类型字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除查询类型(基础节点组追加请求& 请求, 查询字段投影 当前) { if (const auto* f = 查字段(当前.结构, 查询类型字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改查询类型(查询节点准备& 准备, const 稳定编码& 值) { 替换准备字段(准备.节点, 查询类型字段, 编码字段(值)); }
[[maybe_unused]] 稳定编码 读取查询类型(查询字段投影 当前) { return 读字段<稳定编码>(当前.结构, 查询类型字段); }
稳定编码 查询外设场景字段;
[[maybe_unused]] void 构造新增查询外设场景(查询节点准备& 准备, const 稳定编码& 值) { 准备.节点.字段组.push_back({查询外设场景字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除查询外设场景(基础节点组追加请求& 请求, 查询字段投影 当前) { if (const auto* f = 查字段(当前.结构, 查询外设场景字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改查询外设场景(查询节点准备& 准备, const 稳定编码& 值) { 替换准备字段(准备.节点, 查询外设场景字段, 编码字段(值)); }
[[maybe_unused]] 稳定编码 读取查询外设场景(查询字段投影 当前) { return 读字段<稳定编码>(当前.结构, 查询外设场景字段); }
稳定编码 查询说话者字段;
[[maybe_unused]] void 构造新增查询说话者(查询节点准备& 准备, const 稳定编码& 值) { 准备.节点.字段组.push_back({查询说话者字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除查询说话者(基础节点组追加请求& 请求, 查询字段投影 当前) { if (const auto* f = 查字段(当前.结构, 查询说话者字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改查询说话者(查询节点准备& 准备, const 稳定编码& 值) { 替换准备字段(准备.节点, 查询说话者字段, 编码字段(值)); }
[[maybe_unused]] 稳定编码 读取查询说话者(查询字段投影 当前) { return 读字段<稳定编码>(当前.结构, 查询说话者字段); }
稳定编码 查询调用标识字段;
[[maybe_unused]] void 构造新增查询调用标识(查询节点准备& 准备, const std::uint64_t& 值) { 准备.节点.字段组.push_back({查询调用标识字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除查询调用标识(基础节点组追加请求& 请求, 查询字段投影 当前) { if (const auto* f = 查字段(当前.结构, 查询调用标识字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改查询调用标识(查询节点准备& 准备, const std::uint64_t& 值) { 替换准备字段(准备.节点, 查询调用标识字段, 编码字段(值)); }
[[maybe_unused]] std::uint64_t 读取查询调用标识(查询字段投影 当前) { return 读字段<std::uint64_t>(当前.结构, 查询调用标识字段); }
稳定编码 查询语种字段;
[[maybe_unused]] void 构造新增查询语种(查询节点准备& 准备, const std::string& 值) { 准备.节点.字段组.push_back({查询语种字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除查询语种(基础节点组追加请求& 请求, 查询字段投影 当前) { if (const auto* f = 查字段(当前.结构, 查询语种字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改查询语种(查询节点准备& 准备, const std::string& 值) { 替换准备字段(准备.节点, 查询语种字段, 编码字段(值)); }
[[maybe_unused]] std::string 读取查询语种(查询字段投影 当前) { return 读字段<std::string>(当前.结构, 查询语种字段); }
稳定编码 查询词面字段;
[[maybe_unused]] void 构造新增查询词面(查询节点准备& 准备, const std::string& 值) { 准备.节点.字段组.push_back({查询词面字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除查询词面(基础节点组追加请求& 请求, 查询字段投影 当前) { if (const auto* f = 查字段(当前.结构, 查询词面字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改查询词面(查询节点准备& 准备, const std::string& 值) { 替换准备字段(准备.节点, 查询词面字段, 编码字段(值)); }
[[maybe_unused]] std::string 读取查询词面(查询字段投影 当前) { return 读字段<std::string>(当前.结构, 查询词面字段); }
稳定编码 查询批次摘要字段;
[[maybe_unused]] void 构造新增查询批次摘要(查询节点准备& 准备, const std::string& 值) { 准备.节点.字段组.push_back({查询批次摘要字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除查询批次摘要(基础节点组追加请求& 请求, 查询字段投影 当前) { if (const auto* f = 查字段(当前.结构, 查询批次摘要字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改查询批次摘要(查询节点准备& 准备, const std::string& 值) { 替换准备字段(准备.节点, 查询批次摘要字段, 编码字段(值)); }
[[maybe_unused]] std::string 读取查询批次摘要(查询字段投影 当前) { return 读字段<std::string>(当前.结构, 查询批次摘要字段); }
稳定编码 查询包标识字段;
[[maybe_unused]] void 构造新增查询包标识(查询节点准备& 准备, const std::string& 值) { 准备.节点.字段组.push_back({查询包标识字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除查询包标识(基础节点组追加请求& 请求, 查询字段投影 当前) { if (const auto* f = 查字段(当前.结构, 查询包标识字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改查询包标识(查询节点准备& 准备, const std::string& 值) { 替换准备字段(准备.节点, 查询包标识字段, 编码字段(值)); }
[[maybe_unused]] std::string 读取查询包标识(查询字段投影 当前) { return 读字段<std::string>(当前.结构, 查询包标识字段); }
稳定编码 查询时间语义字段;
[[maybe_unused]] void 构造新增查询时间语义(查询节点准备& 准备, const std::int64_t& 值) { 准备.节点.字段组.push_back({查询时间语义字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除查询时间语义(基础节点组追加请求& 请求, 查询字段投影 当前) { if (const auto* f = 查字段(当前.结构, 查询时间语义字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改查询时间语义(查询节点准备& 准备, const std::int64_t& 值) { 替换准备字段(准备.节点, 查询时间语义字段, 编码字段(值)); }
[[maybe_unused]] std::int64_t 读取查询时间语义(查询字段投影 当前) { return 读字段<std::int64_t>(当前.结构, 查询时间语义字段); }
稳定编码 查询时间纳秒字段;
[[maybe_unused]] void 构造新增查询时间纳秒(查询节点准备& 准备, const std::int64_t& 值) { 准备.节点.字段组.push_back({查询时间纳秒字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除查询时间纳秒(基础节点组追加请求& 请求, 查询字段投影 当前) { if (const auto* f = 查字段(当前.结构, 查询时间纳秒字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改查询时间纳秒(查询节点准备& 准备, const std::int64_t& 值) { 替换准备字段(准备.节点, 查询时间纳秒字段, 编码字段(值)); }
[[maybe_unused]] std::int64_t 读取查询时间纳秒(查询字段投影 当前) { return 读字段<std::int64_t>(当前.结构, 查询时间纳秒字段); }
稳定编码 查询结局字段;
[[maybe_unused]] void 构造新增查询结局(查询节点准备& 准备, const std::int64_t& 值) { 准备.节点.字段组.push_back({查询结局字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除查询结局(基础节点组追加请求& 请求, 查询字段投影 当前) { if (const auto* f = 查字段(当前.结构, 查询结局字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改查询结局(查询节点准备& 准备, const std::int64_t& 值) { 替换准备字段(准备.节点, 查询结局字段, 编码字段(值)); }
[[maybe_unused]] std::int64_t 读取查询结局(查询字段投影 当前) { return 读字段<std::int64_t>(当前.结构, 查询结局字段); }
struct 消息身份 final { 稳定编码 节点; };
struct 消息字段投影 final { const 基础节点结构& 结构; };
struct 消息节点准备 final { 基础组内新节点 节点; };
稳定编码 消息类型字段;
[[maybe_unused]] void 构造新增消息类型(消息节点准备& 准备, const 稳定编码& 值) { 准备.节点.字段组.push_back({消息类型字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除消息类型(基础节点组追加请求& 请求, 消息字段投影 当前) { if (const auto* f = 查字段(当前.结构, 消息类型字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改消息类型(消息节点准备& 准备, const 稳定编码& 值) { 替换准备字段(准备.节点, 消息类型字段, 编码字段(值)); }
[[maybe_unused]] 稳定编码 读取消息类型(消息字段投影 当前) { return 读字段<稳定编码>(当前.结构, 消息类型字段); }
稳定编码 消息汉语释义正文字段;
[[maybe_unused]] void 构造新增消息汉语释义正文(消息节点准备& 准备, const 稳定编码& 值) { 准备.节点.字段组.push_back({消息汉语释义正文字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除消息汉语释义正文(基础节点组追加请求& 请求, 消息字段投影 当前) { if (const auto* f = 查字段(当前.结构, 消息汉语释义正文字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改消息汉语释义正文(消息节点准备& 准备, const 稳定编码& 值) { 替换准备字段(准备.节点, 消息汉语释义正文字段, 编码字段(值)); }
[[maybe_unused]] 稳定编码 读取消息汉语释义正文(消息字段投影 当前) { return 读字段<稳定编码>(当前.结构, 消息汉语释义正文字段); }
稳定编码 消息记录键字段;
[[maybe_unused]] void 构造新增消息记录键(消息节点准备& 准备, const std::string& 值) { 准备.节点.字段组.push_back({消息记录键字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除消息记录键(基础节点组追加请求& 请求, 消息字段投影 当前) { if (const auto* f = 查字段(当前.结构, 消息记录键字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改消息记录键(消息节点准备& 准备, const std::string& 值) { 替换准备字段(准备.节点, 消息记录键字段, 编码字段(值)); }
[[maybe_unused]] std::string 读取消息记录键(消息字段投影 当前) { return 读字段<std::string>(当前.结构, 消息记录键字段); }
稳定编码 消息词头语种字段;
[[maybe_unused]] void 构造新增消息词头语种(消息节点准备& 准备, const std::string& 值) { 准备.节点.字段组.push_back({消息词头语种字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除消息词头语种(基础节点组追加请求& 请求, 消息字段投影 当前) { if (const auto* f = 查字段(当前.结构, 消息词头语种字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改消息词头语种(消息节点准备& 准备, const std::string& 值) { 替换准备字段(准备.节点, 消息词头语种字段, 编码字段(值)); }
[[maybe_unused]] std::string 读取消息词头语种(消息字段投影 当前) { return 读字段<std::string>(当前.结构, 消息词头语种字段); }
稳定编码 消息词面字段;
[[maybe_unused]] void 构造新增消息词面(消息节点准备& 准备, const std::string& 值) { 准备.节点.字段组.push_back({消息词面字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除消息词面(基础节点组追加请求& 请求, 消息字段投影 当前) { if (const auto* f = 查字段(当前.结构, 消息词面字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改消息词面(消息节点准备& 准备, const std::string& 值) { 替换准备字段(准备.节点, 消息词面字段, 编码字段(值)); }
[[maybe_unused]] std::string 读取消息词面(消息字段投影 当前) { return 读字段<std::string>(当前.结构, 消息词面字段); }
稳定编码 消息义项键字段;
[[maybe_unused]] void 构造新增消息义项键(消息节点准备& 准备, const std::string& 值) { 准备.节点.字段组.push_back({消息义项键字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除消息义项键(基础节点组追加请求& 请求, 消息字段投影 当前) { if (const auto* f = 查字段(当前.结构, 消息义项键字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改消息义项键(消息节点准备& 准备, const std::string& 值) { 替换准备字段(准备.节点, 消息义项键字段, 编码字段(值)); }
[[maybe_unused]] std::string 读取消息义项键(消息字段投影 当前) { return 读字段<std::string>(当前.结构, 消息义项键字段); }
稳定编码 消息例句原文字段;
[[maybe_unused]] void 构造新增消息例句原文(消息节点准备& 准备, const std::string& 值) { 准备.节点.字段组.push_back({消息例句原文字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除消息例句原文(基础节点组追加请求& 请求, 消息字段投影 当前) { if (const auto* f = 查字段(当前.结构, 消息例句原文字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改消息例句原文(消息节点准备& 准备, const std::string& 值) { 替换准备字段(准备.节点, 消息例句原文字段, 编码字段(值)); }
[[maybe_unused]] std::string 读取消息例句原文(消息字段投影 当前) { return 读字段<std::string>(当前.结构, 消息例句原文字段); }
稳定编码 消息例句汉译字段;
[[maybe_unused]] void 构造新增消息例句汉译(消息节点准备& 准备, const std::string& 值) { 准备.节点.字段组.push_back({消息例句汉译字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除消息例句汉译(基础节点组追加请求& 请求, 消息字段投影 当前) { if (const auto* f = 查字段(当前.结构, 消息例句汉译字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改消息例句汉译(消息节点准备& 准备, const std::string& 值) { 替换准备字段(准备.节点, 消息例句汉译字段, 编码字段(值)); }
[[maybe_unused]] std::string 读取消息例句汉译(消息字段投影 当前) { return 读字段<std::string>(当前.结构, 消息例句汉译字段); }
稳定编码 消息读音字段;
[[maybe_unused]] void 构造新增消息读音(消息节点准备& 准备, const std::string& 值) { 准备.节点.字段组.push_back({消息读音字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除消息读音(基础节点组追加请求& 请求, 消息字段投影 当前) { if (const auto* f = 查字段(当前.结构, 消息读音字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改消息读音(消息节点准备& 准备, const std::string& 值) { 替换准备字段(准备.节点, 消息读音字段, 编码字段(值)); }
[[maybe_unused]] std::string 读取消息读音(消息字段投影 当前) { return 读字段<std::string>(当前.结构, 消息读音字段); }
稳定编码 消息来源页URL字段;
[[maybe_unused]] void 构造新增消息来源页URL(消息节点准备& 准备, const std::string& 值) { 准备.节点.字段组.push_back({消息来源页URL字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除消息来源页URL(基础节点组追加请求& 请求, 消息字段投影 当前) { if (const auto* f = 查字段(当前.结构, 消息来源页URL字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改消息来源页URL(消息节点准备& 准备, const std::string& 值) { 替换准备字段(准备.节点, 消息来源页URL字段, 编码字段(值)); }
[[maybe_unused]] std::string 读取消息来源页URL(消息字段投影 当前) { return 读字段<std::string>(当前.结构, 消息来源页URL字段); }
稳定编码 消息来源版本字段;
[[maybe_unused]] void 构造新增消息来源版本(消息节点准备& 准备, const std::string& 值) { 准备.节点.字段组.push_back({消息来源版本字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除消息来源版本(基础节点组追加请求& 请求, 消息字段投影 当前) { if (const auto* f = 查字段(当前.结构, 消息来源版本字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改消息来源版本(消息节点准备& 准备, const std::string& 值) { 替换准备字段(准备.节点, 消息来源版本字段, 编码字段(值)); }
[[maybe_unused]] std::string 读取消息来源版本(消息字段投影 当前) { return 读字段<std::string>(当前.结构, 消息来源版本字段); }
稳定编码 消息许可标识字段;
[[maybe_unused]] void 构造新增消息许可标识(消息节点准备& 准备, const std::string& 值) { 准备.节点.字段组.push_back({消息许可标识字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除消息许可标识(基础节点组追加请求& 请求, 消息字段投影 当前) { if (const auto* f = 查字段(当前.结构, 消息许可标识字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改消息许可标识(消息节点准备& 准备, const std::string& 值) { 替换准备字段(准备.节点, 消息许可标识字段, 编码字段(值)); }
[[maybe_unused]] std::string 读取消息许可标识(消息字段投影 当前) { return 读字段<std::string>(当前.结构, 消息许可标识字段); }
稳定编码 消息署名字段;
[[maybe_unused]] void 构造新增消息署名(消息节点准备& 准备, const std::string& 值) { 准备.节点.字段组.push_back({消息署名字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除消息署名(基础节点组追加请求& 请求, 消息字段投影 当前) { if (const auto* f = 查字段(当前.结构, 消息署名字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改消息署名(消息节点准备& 准备, const std::string& 值) { 替换准备字段(准备.节点, 消息署名字段, 编码字段(值)); }
[[maybe_unused]] std::string 读取消息署名(消息字段投影 当前) { return 读字段<std::string>(当前.结构, 消息署名字段); }
稳定编码 消息原始来源定位字段;
[[maybe_unused]] void 构造新增消息原始来源定位(消息节点准备& 准备, const std::string& 值) { 准备.节点.字段组.push_back({消息原始来源定位字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除消息原始来源定位(基础节点组追加请求& 请求, 消息字段投影 当前) { if (const auto* f = 查字段(当前.结构, 消息原始来源定位字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改消息原始来源定位(消息节点准备& 准备, const std::string& 值) { 替换准备字段(准备.节点, 消息原始来源定位字段, 编码字段(值)); }
[[maybe_unused]] std::string 读取消息原始来源定位(消息字段投影 当前) { return 读字段<std::string>(当前.结构, 消息原始来源定位字段); }
struct 续接身份 final { 稳定编码 节点; };
struct 续接字段投影 final { const 基础节点结构& 结构; };
struct 续接节点准备 final { 基础组内新节点 节点; };
稳定编码 续接类型字段;
[[maybe_unused]] void 构造新增续接类型(续接节点准备& 准备, const 稳定编码& 值) { 准备.节点.字段组.push_back({续接类型字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除续接类型(基础节点组追加请求& 请求, 续接字段投影 当前) { if (const auto* f = 查字段(当前.结构, 续接类型字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改续接类型(续接节点准备& 准备, const 稳定编码& 值) { 替换准备字段(准备.节点, 续接类型字段, 编码字段(值)); }
[[maybe_unused]] 稳定编码 读取续接类型(续接字段投影 当前) { return 读字段<稳定编码>(当前.结构, 续接类型字段); }
稳定编码 续接阶段字段;
[[maybe_unused]] void 构造新增续接阶段(续接节点准备& 准备, const std::int64_t& 值) { 准备.节点.字段组.push_back({续接阶段字段, 编码字段(值)}); }
[[maybe_unused]] void 构造删除续接阶段(基础节点组追加请求& 请求, 续接字段投影 当前) { if (const auto* f = 查字段(当前.结构, 续接阶段字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改续接阶段(续接节点准备& 准备, const std::int64_t& 值) { 替换准备字段(准备.节点, 续接阶段字段, 编码字段(值)); }
[[maybe_unused]] std::int64_t 读取续接阶段(续接字段投影 当前) { return 读字段<std::int64_t>(当前.结构, 续接阶段字段); }
稳定编码 续接前状态字段;
[[maybe_unused]] void 构造新增续接前状态(续接节点准备& 准备, const std::optional<稳定编码>& 值) { if (值) 准备.节点.字段组.push_back({续接前状态字段, 编码字段(*值)}); }
[[maybe_unused]] void 构造删除续接前状态(基础节点组追加请求& 请求, 续接字段投影 当前) { if (const auto* f = 查字段(当前.结构, 续接前状态字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改续接前状态(续接节点准备& 准备, const std::optional<稳定编码>& 值) { 替换准备字段(准备.节点, 续接前状态字段, 值 ? std::optional<基础组内字段内容>{编码字段(*值)} : std::nullopt); }
[[maybe_unused]] std::optional<稳定编码> 读取续接前状态(续接字段投影 当前) { const auto* f = 查字段(当前.结构, 续接前状态字段); return f ? std::optional<稳定编码>{解码字段<稳定编码>(*f)} : std::nullopt; }
稳定编码 续接后状态字段;
[[maybe_unused]] void 构造新增续接后状态(续接节点准备& 准备, const std::optional<稳定编码>& 值) { if (值) 准备.节点.字段组.push_back({续接后状态字段, 编码字段(*值)}); }
[[maybe_unused]] void 构造删除续接后状态(基础节点组追加请求& 请求, 续接字段投影 当前) { if (const auto* f = 查字段(当前.结构, 续接后状态字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改续接后状态(续接节点准备& 准备, const std::optional<稳定编码>& 值) { 替换准备字段(准备.节点, 续接后状态字段, 值 ? std::optional<基础组内字段内容>{编码字段(*值)} : std::nullopt); }
[[maybe_unused]] std::optional<稳定编码> 读取续接后状态(续接字段投影 当前) { const auto* f = 查字段(当前.结构, 续接后状态字段); return f ? std::optional<稳定编码>{解码字段<稳定编码>(*f)} : std::nullopt; }
稳定编码 续接变化动态字段;
[[maybe_unused]] void 构造新增续接变化动态(续接节点准备& 准备, const std::optional<稳定编码>& 值) { if (值) 准备.节点.字段组.push_back({续接变化动态字段, 编码字段(*值)}); }
[[maybe_unused]] void 构造删除续接变化动态(基础节点组追加请求& 请求, 续接字段投影 当前) { if (const auto* f = 查字段(当前.结构, 续接变化动态字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改续接变化动态(续接节点准备& 准备, const std::optional<稳定编码>& 值) { 替换准备字段(准备.节点, 续接变化动态字段, 值 ? std::optional<基础组内字段内容>{编码字段(*值)} : std::nullopt); }
[[maybe_unused]] std::optional<稳定编码> 读取续接变化动态(续接字段投影 当前) { const auto* f = 查字段(当前.结构, 续接变化动态字段); return f ? std::optional<稳定编码>{解码字段<稳定编码>(*f)} : std::nullopt; }
稳定编码 续接无需动态原因字段;
[[maybe_unused]] void 构造新增续接无需动态原因(续接节点准备& 准备, const std::optional<std::int64_t>& 值) { if (值) 准备.节点.字段组.push_back({续接无需动态原因字段, 编码字段(*值)}); }
[[maybe_unused]] void 构造删除续接无需动态原因(基础节点组追加请求& 请求, 续接字段投影 当前) { if (const auto* f = 查字段(当前.结构, 续接无需动态原因字段)) 请求.删除字段关系组.push_back(f->编码); }
[[maybe_unused]] void 构造修改续接无需动态原因(续接节点准备& 准备, const std::optional<std::int64_t>& 值) { 替换准备字段(准备.节点, 续接无需动态原因字段, 值 ? std::optional<基础组内字段内容>{编码字段(*值)} : std::nullopt); }
[[maybe_unused]] std::optional<std::int64_t> 读取续接无需动态原因(续接字段投影 当前) { const auto* f = 查字段(当前.结构, 续接无需动态原因字段); return f ? std::optional<std::int64_t>{解码字段<std::int64_t>(*f)} : std::nullopt; }


using 本族快照组 = std::map<稳定编码, 基础联合节点快照>;
bool 属于类型(const 基础节点结构& n, 稳定编码 key, 稳定编码 type) {
    const auto* f = 查字段(n, key);
    return f && std::get_if<稳定编码>(&f->内容) && std::get<稳定编码>(f->内容) == type;
}
std::vector<基础外部关系> 读取查询消息边(const 基础联合节点快照& n) {
    std::vector<基础外部关系> edges;
    for (const auto& e : n.出向外部关系组) {
        if (e.类型 != 基础外部关系类型::父子) continue;
        if (e.角色或顺序 < 0) 记录故障(L"查询组织边顺序为负数");
        if (e.角色或顺序 > 0) edges.push_back(e);
    }
    std::sort(edges.begin(), edges.end(), [](const auto& a, const auto& b) { return a.角色或顺序 < b.角色或顺序; });
    for (std::size_t i = 0; i < edges.size(); ++i)
        if (edges[i].角色或顺序 != static_cast<std::int64_t>(i + 1)) 记录故障(L"义项顺序不连续或重复");
    return edges;
}
std::optional<基础外部关系> 读取查询续接边(const 基础联合节点快照& n) {
    std::optional<基础外部关系> edge;
    for (const auto& e : n.出向外部关系组) if (e.类型 == 基础外部关系类型::父子 && e.角色或顺序 == 0) {
        if (edge) 记录故障(L"查询有重复处理记录");
        edge = e;
    }
    return edge;
}
void 构造新增查询消息边(基础节点组追加请求& r, 查询身份 R, 消息身份 M, std::int64_t order) {
    r.追加外部关系组.push_back({R.节点, M.节点, 基础外部关系类型::父子, order});
}
void 构造删除查询消息边(基础节点组追加请求& r, const 基础外部关系& e) { r.删除外部关系组.push_back(e.编码); }
[[maybe_unused]] void 构造修改查询消息边(基础节点组追加请求& r, 查询身份 R, 消息身份 M, std::int64_t order) {
    for (auto& e : r.追加外部关系组) {
        const auto* source = std::get_if<稳定编码>(&e.源节点);
        const auto* target = std::get_if<稳定编码>(&e.目标节点);
        if (source && target && *source == R.节点 && *target == M.节点) { e.角色或顺序 = order; return; }
    }
    记录故障(L"未封存查询消息边不存在");
}
void 构造新增查询续接边(基础节点组追加请求& r, 查询身份 R, 基础组内节点引用 B) {
    r.追加外部关系组.push_back({R.节点, B, 基础外部关系类型::父子, 0});
}
void 构造删除查询续接边(基础节点组追加请求& r, const 基础外部关系& e) { r.删除外部关系组.push_back(e.编码); }
[[maybe_unused]] void 构造修改查询续接边(基础节点组追加请求& r, 查询身份 R, 基础组内节点引用 B) {
    for (auto& e : r.追加外部关系组) {
        const auto* source = std::get_if<稳定编码>(&e.源节点);
        if (source && *source == R.节点 && e.类型 == 基础外部关系类型::父子 && e.角色或顺序 == 0)
            { e.目标节点 = B; return; }
    }
    记录故障(L"未封存查询续接边不存在");
}
基础外部关系 读取消息所属边(const 基础联合节点快照& M) {
    std::optional<基础外部关系> parent;
    for (const auto& e : M.入向外部关系组) if (e.类型 == 基础外部关系类型::父子 && e.角色或顺序 > 0) {
        if (parent) 记录故障(L"义项消息属于多个查询");
        parent = e;
    }
    if (!parent) 记录故障(L"正式义项消息缺少查询组织边");
    return *parent;
}
查询节点准备 构造新增查询(const 新词典查询事实& f) {
    查询节点准备 n;
    构造新增查询类型(n, 查询类型);
    构造新增查询外设场景(n, f.定位.外设场景);
    构造新增查询说话者(n, f.词典说话者);
    构造新增查询调用标识(n, f.定位.调用来源标识);
    构造新增查询语种(n, f.查询语种);
    构造新增查询词面(n, f.查询词面);
    构造新增查询批次摘要(n, f.清单摘要);
    构造新增查询包标识(n, f.原始包标识);
    构造新增查询时间语义(n, static_cast<std::int64_t>(f.查询完成时间.语义));
    构造新增查询时间纳秒(n, f.查询完成时间.纳秒);
    构造新增查询结局(n, static_cast<std::int64_t>(f.结局));
    return n;
}
[[maybe_unused]] void 修改查询准备(查询节点准备& n, const 新词典查询事实& f) { n = 构造新增查询(f); }
消息节点准备 构造新增义项消息(const 词典义项材料& v, 稳定编码 body) {
    消息节点准备 n;
    构造新增消息类型(n, 消息类型); 构造新增消息汉语释义正文(n, body);
    构造新增消息记录键(n, v.记录键); 构造新增消息词头语种(n, v.词头语种);
    构造新增消息词面(n, v.词面); 构造新增消息义项键(n, v.义项键);
    构造新增消息例句原文(n, v.例句原文); 构造新增消息例句汉译(n, v.例句汉译);
    构造新增消息读音(n, v.读音); 构造新增消息来源页URL(n, v.来源页URL);
    构造新增消息来源版本(n, v.来源版本); 构造新增消息许可标识(n, v.许可标识);
    构造新增消息署名(n, v.署名); 构造新增消息原始来源定位(n, v.原始来源定位);
    return n;
}
[[maybe_unused]] void 修改义项消息准备(消息节点准备& n, const 词典义项材料& v, 稳定编码 body) { n = 构造新增义项消息(v, body); }
续接节点准备 构造新增续接(const 新词典接收处理信息& v) {
    续接节点准备 n;
    构造新增续接类型(n, 续接类型);
    构造新增续接阶段(n, static_cast<std::int64_t>(v.阶段));
    构造新增续接前状态(n, v.前状态); 构造新增续接后状态(n, v.后状态);
    构造新增续接变化动态(n, v.动态);
    构造新增续接无需动态原因(n, v.无需动态原因
        ? std::optional<std::int64_t>{static_cast<std::int64_t>(*v.无需动态原因)} : std::nullopt);
    return n;
}
[[maybe_unused]] void 修改续接准备(续接节点准备& n, const 新词典接收处理信息& v) { n = 构造新增续接(v); }
新词典接收处理信息 读取续接(const 基础联合节点快照& n) {
    续接字段投影 p{n.结构};
    if (读取续接类型(p) != 续接类型) 记录故障(L"查询处理节点类型错误");
    新词典接收处理信息 r{n.结构.节点.编码,
        static_cast<新词典接收处理阶段>(读取续接阶段(p)),
        读取续接前状态(p), 读取续接后状态(p), 读取续接变化动态(p), {}};
    if (const auto cause = 读取续接无需动态原因(p)) r.无需动态原因 = static_cast<新词条动态记录原因>(*cause);
    switch (r.阶段) {
    case 新词典接收处理阶段::接收已发表:
        if (!r.后状态 || r.动态 || r.无需动态原因) 记录故障(L"接收阶段字段不完整");
        break;
    case 新词典接收处理阶段::动态已建立:
        if (!r.前状态 || !r.后状态 || !r.动态 || r.无需动态原因) 记录故障(L"动态阶段字段不完整");
        break;
    case 新词典接收处理阶段::场景已挂接:
        if (r.前状态 || r.后状态 || r.动态 || r.无需动态原因) 记录故障(L"已收束记录仍保留临时引用");
        break;
    case 新词典接收处理阶段::无需动态:
        if (r.前状态 || r.后状态 || r.动态 || !r.无需动态原因) 记录故障(L"无需动态记录不完整");
        break;
    default: 记录故障(L"查询处理阶段未知");
    }
    return r;
}
新词典查询记录信息 读取查询头(const 基础联合节点快照& n) {
    查询字段投影 p{n.结构};
    if (!属于类型(n.结构, 查询类型字段, 查询类型)) throw S::未找到;
    新词典查询记录信息 r;
    r.查询记录 = n.结构.节点.编码;
    r.定位 = {读取查询外设场景(p), 读取查询调用标识(p)};
    r.词典说话者 = 读取查询说话者(p);
    r.查询语种 = 读取查询语种(p); r.查询词面 = 读取查询词面(p);
    r.清单摘要 = 读取查询批次摘要(p); r.原始包标识 = 读取查询包标识(p);
    r.查询完成时间 = {static_cast<新状态时间语义>(读取查询时间语义(p)), 读取查询时间纳秒(p)};
    r.结局 = static_cast<新词典查阅结局>(读取查询结局(p));
    if (!r.定位.调用来源标识 || r.查询完成时间.纳秒 < 0
        || (r.查询完成时间.语义 != 新状态时间语义::实例绝对UTC && r.查询完成时间.语义 != 新状态时间语义::抽象相对)
        || (r.结局 != 新词典查阅结局::命中 && r.结局 != 新词典查阅结局::正式未命中))
        记录故障(L"查询头内容损坏");
    for (const auto& e : 读取查询消息边(n)) r.有序义项消息.push_back(e.目标节点);
    return r;
}
本族快照组 读取查询闭包(稳定编码 R) {
    // 用入口定位身份，返回材料全部取最后一次L1读取，不要求前后快照相等。
    auto seed = 读取记录快照(R);
    for (;;) {
        (void)读取查询头(seed);
        std::vector<稳定编码> ids{R};
        for (const auto& e : 读取查询消息边(seed)) ids.push_back(e.目标节点);
        if (const auto e = 读取查询续接边(seed)) ids.push_back(e->目标节点);
        const auto batch = 全局基础数据集.查询节点组完整结构(ids);
        if (batch.状态 == 基础结构操作状态::资源失败) throw S::资源失败;
        if (batch.状态 == 基础结构操作状态::节点不存在) throw S::未找到;
        if (batch.状态 != 基础结构操作状态::已完成 || batch.节点组.size() != ids.size())
            记录故障(L"查询闭包读取失败");
        本族快照组 out;
        for (const auto& x : batch.节点组) if (x.快照) out.emplace(x.查询节点, *x.快照);
        const auto r = out.find(R);
        if (r == out.end()) throw S::未找到;
        bool needMore = false;
        for (const auto& e : r->second.出向外部关系组)
            if (e.类型 == 基础外部关系类型::父子 && !out.contains(e.目标节点)) {
                if (std::find(ids.begin(), ids.end(), e.目标节点) != ids.end())
                    记录故障(L"同一L1快照中的查询组织引用悬空");
                needMore = true;
            }
        if (needMore) { seed = r->second; continue; }
        return out;
    }
}
新词典查询记录信息 读取查询(const 本族快照组& all, 稳定编码 R) {
    const auto& n = all.at(R);
    auto r = 读取查询头(n);
    if (const auto e = 读取查询续接边(n)) r.接收处理 = 读取续接(all.at(e->目标节点));
    if ((r.结局 == 新词典查阅结局::命中 && (r.有序义项消息.empty() || !r.接收处理))
        || (r.结局 == 新词典查阅结局::正式未命中 && (!r.有序义项消息.empty() || r.接收处理)))
        记录故障(L"查询结局与正式组织不符");
    for (const auto M : r.有序义项消息)
        if (!属于类型(all.at(M).结构, 消息类型字段, 消息类型)) 记录故障(L"查询的义项消息类型错误");
    return r;
}
新词典义项消息信息 读取义项消息内容(const 本族快照组& all,
    const 新词典查询记录信息& R, 稳定编码 M, const 新_特征值类& values) {
    const auto pos = std::find(R.有序义项消息.begin(), R.有序义项消息.end(), M);
    if (pos == R.有序义项消息.end()) throw S::未找到;
    const auto& n = all.at(M);
    消息字段投影 p{n.结构};
    if (读取消息类型(p) != 消息类型) 记录故障(L"义项类型错误");
    新词典义项消息信息 r{M, R.查询记录, R.定位.外设场景, R.词典说话者,
        static_cast<std::size_t>(pos - R.有序义项消息.begin()) + 1, R.查询完成时间, {}};
    auto& v = r.材料;
    v.记录键 = 读取消息记录键(p); v.词头语种 = 读取消息词头语种(p);
    v.词面 = 读取消息词面(p); v.义项键 = 读取消息义项键(p);
    v.例句原文 = 读取消息例句原文(p); v.例句汉译 = 读取消息例句汉译(p);
    v.读音 = 读取消息读音(p); v.来源页URL = 读取消息来源页URL(p);
    v.来源版本 = 读取消息来源版本(p); v.许可标识 = 读取消息许可标识(p);
    v.署名 = 读取消息署名(p); v.原始来源定位 = 读取消息原始来源定位(p);
    const auto body = values.获取特征值(读取消息汉语释义正文(p));
    const auto* text = body ? std::get_if<std::string>(&body->材料) : nullptr;
    if (!text) 记录故障(L"义项正文材料不可读");
    v.汉语释义 = *text;
    return r;
}
特征消息成员读取结果 读取消息身份(稳定编码 M) noexcept {
    using T = 特征概念规则状态;
    try {
        if (!记录结构就绪.load(std::memory_order_acquire)) return {T::读取接口未装配, {}, {}};
        if (!有效(M)) return {T::入口拒绝, {}, {}};
        auto n = 读取记录快照(M);
        if (!属于类型(n.结构, 消息类型字段, 消息类型)) return {T::入口拒绝, {}, {}};
        if (n.结构.字段组.size() != 14) return {T::内部不一致, {}, {}};
        消息字段投影 p{n.结构};
        (void)读取消息汉语释义正文(p);
        (void)读取消息记录键(p); (void)读取消息词头语种(p); (void)读取消息词面(p); (void)读取消息义项键(p);
        (void)读取消息例句原文(p); (void)读取消息例句汉译(p); (void)读取消息读音(p);
        (void)读取消息来源页URL(p); (void)读取消息来源版本(p); (void)读取消息许可标识(p);
        (void)读取消息署名(p); (void)读取消息原始来源定位(p);
        (void)读取消息所属边(n);
        return {T::已完成, M, {std::move(n)}};
    } catch (S status) {
        if (status == S::未找到) return {T::值不存在, {}, {}};
        if (status == S::资源失败) return {T::资源失败, {}, {}};
        return {T::内部不一致, {}, {}};
    } catch (const std::bad_alloc&) { return {T::资源失败, {}, {}}; }
    catch (const std::length_error&) { return {T::资源失败, {}, {}}; }
    catch (...) { 记录故障(L"读取词典消息身份发生工程异常"); }
}
class 稳定消息读取器 final : public 特征消息成员读取接口 {
public:
    特征消息成员读取结果 读取消息集合成员身份(稳定编码 n) const noexcept override { return 读取消息身份(n); }
};
稳定消息读取器 消息读取接口;
} // namespace

外设词典记录类::外设词典记录类(新_特征值类& values) noexcept : 值服务_(values) {}
新词典记录操作状态 外设词典记录类::初始化() noexcept {
    try {
        std::lock_guard lock(记录互斥);
        if (记录结构就绪.load(std::memory_order_acquire)) return S::已复用;
        std::vector<稳定编码*> schema{
            &查询类型, &消息类型, &续接类型,
            &查询类型字段,
            &查询外设场景字段,
            &查询说话者字段,
            &查询调用标识字段,
            &查询语种字段,
            &查询词面字段,
            &查询批次摘要字段,
            &查询包标识字段,
            &查询时间语义字段,
            &查询时间纳秒字段,
            &查询结局字段,
            &消息类型字段,
            &消息汉语释义正文字段,
            &消息记录键字段,
            &消息词头语种字段,
            &消息词面字段,
            &消息义项键字段,
            &消息例句原文字段,
            &消息例句汉译字段,
            &消息读音字段,
            &消息来源页URL字段,
            &消息来源版本字段,
            &消息许可标识字段,
            &消息署名字段,
            &消息原始来源定位字段,
            &续接类型字段,
            &续接阶段字段,
            &续接前状态字段,
            &续接后状态字段,
            &续接变化动态字段,
            &续接无需动态原因字段};
        基础节点组追加请求 request;
        request.新节点组.resize(schema.size());
        const auto result = 全局基础数据集.原子追加节点组(request);
        if (result.状态 == L::资源失败) return S::资源失败;
        if (result.状态 != L::已完成 || result.新节点组.size() != schema.size()) 记录故障(L"外设词典schema创建失败");
        for (std::size_t i = 0; i < schema.size(); ++i) *schema[i] = result.新节点组[i];
        记录结构就绪.store(true, std::memory_order_release);
        return S::已完成;
    } catch (const std::bad_alloc&) { return S::资源失败; }
    catch (const std::length_error&) { return S::资源失败; }
    catch (...) { 记录故障(L"初始化外设词典记录异常"); }
}
新词典查询读取结果 外设词典记录类::读取查询记录(稳定编码 R) const noexcept {
    try {
        if (!记录结构就绪.load(std::memory_order_acquire)) return {S::未装配, {}};
        std::shared_lock lock(记录读取退出互斥);
        const auto all = 读取查询闭包(R);
        return {S::已完成, 读取查询(all, R)};
    } catch (S status) { return {status, {}}; }
    catch (const std::bad_alloc&) { return {S::资源失败, {}}; }
    catch (const std::length_error&) { return {S::资源失败, {}}; }
    catch (...) { 记录故障(L"读取外设词典查询异常"); }
}
新词典义项读取结果 外设词典记录类::读取义项消息(稳定编码 M) const noexcept {
    try {
        if (!记录结构就绪.load(std::memory_order_acquire)) return {S::未装配, {}};
        本族快照组 all;
        稳定编码 R;
        {
            std::shared_lock lock(记录读取退出互斥);
            const auto n = 读取记录快照(M);
            if (!属于类型(n.结构, 消息类型字段, 消息类型)) return {S::未找到, {}};
            R = 读取消息所属边(n).源节点;
            all = 读取查询闭包(R);
        }
        // 正文由值owner读取；不持记录锁跨owner调用。
        return {S::已完成, 读取义项消息内容(all, 读取查询(all, R), M, 值服务_)};
    } catch (S status) { return {status, {}}; }
    catch (const std::bad_alloc&) { return {S::资源失败, {}}; }
    catch (const std::length_error&) { return {S::资源失败, {}}; }
    catch (...) { 记录故障(L"读取外设词典义项异常"); }
}
特征消息成员读取结果 外设词典记录类::读取消息集合成员身份(稳定编码 M) const noexcept { return 读取消息身份(M); }
const 特征消息成员读取接口& 外设词典记录类::取得稳定消息身份读取接口() noexcept { return 消息读取接口; }
新词典查询读取结果 外设词典记录类::按调用来源读取查询记录(const 新词典查询定位& where) const noexcept {
    try {
        if (!记录结构就绪.load(std::memory_order_acquire)) return {S::未装配, {}};
        if (!有效(where.外设场景) || !where.调用来源标识) return {S::输入拒绝, {}};
        const 查询键 key{where.外设场景, where.调用来源标识};
        std::optional<稳定编码> cached;
        { std::lock_guard lock(记录互斥); if (const auto i = 查询索引.find(key); i != 查询索引.end()) cached = i->second; }
        if (cached) {
            auto r = 读取查询记录(*cached);
            if (r.状态 != S::未找到) return r;
            std::lock_guard lock(记录互斥); 查询索引.erase(key);
        }
        std::optional<新词典查询记录信息> found;
        // 限定本场景反向字段索引，不扫描全仓节点。
        for (const auto& f : 全局基础数据集.查询目标字段(where.外设场景, 查询外设场景字段)) {
            const auto r = 读取查询记录(f.所属节点);
            if (r.状态 == S::未找到) continue;
            if (r.状态 != S::已完成 || !r.信息) return r;
            if (r.信息->定位.调用来源标识 != where.调用来源标识) continue;
            if (found) 记录故障(L"同一场景调用来源存在重复查询记录");
            found = r.信息;
        }
        if (!found) return {S::未找到, {}};
        { std::lock_guard lock(记录互斥); 查询索引[key] = found->查询记录; }
        return {S::已完成, std::move(found)};
    } catch (const std::bad_alloc&) { return {S::资源失败, {}}; }
    catch (const std::length_error&) { return {S::资源失败, {}}; }
    catch (...) { 记录故障(L"按调用来源定位词典记录异常"); }
}

namespace {
bool 正常零发表(L status) noexcept {
    return status == L::预期不符 || status == L::节点不存在 || status == L::引用不存在;
}
void 加入字段引用身份(基础节点组追加请求& r) {
    std::set<稳定编码> refs;
    const auto add = [&](const 基础组内字段内容& v) { if (const auto* n = std::get_if<稳定编码>(&v)) refs.insert(*n); };
    for (const auto& f : r.追加字段组) add(f.内容);
    for (const auto& n : r.新节点组) for (const auto& f : n.字段组) add(f.内容);
    for (const auto& n : r.预期联合节点组) refs.erase(n.结构.节点.编码);
    for (const auto n : refs) {
        const auto current = 全局基础数据集.查询节点(n);
        if (!current) throw S::未找到;
        r.只读预期.节点组.push_back(*current);
    }
}
void 追加完整字段(基础节点组追加请求& r, 稳定编码 n, 基础组内新节点 fields) {
    for (auto& f : fields.字段组) r.追加字段组.push_back({n, f.字段节点, std::move(f.内容)});
}
void 清理私有身份(std::vector<稳定编码>& ids) noexcept {
    if (ids.empty()) return;
    try {
        const auto batch = 全局基础数据集.查询节点组完整结构(ids);
        if (batch.状态 != 基础结构操作状态::已完成 || batch.节点组.size() != ids.size())
            记录故障(L"私有查询身份回收读取失败，保留现场");
        基础节点组追加请求 r;
        for (const auto& x : batch.节点组) {
            if (!x.快照 || !x.快照->结构.字段组.empty() || !x.快照->入向字段组.empty()
                || !x.快照->入向外部关系组.empty() || !x.快照->出向外部关系组.empty())
                记录故障(L"不能回收已被使用的查询身份");
            r.预期联合节点组.push_back(*x.快照);
            r.删除节点组.push_back(x.查询节点);
        }
        const auto done = 全局基础数据集.原子追加节点组(r);
        if (done.状态 != L::已完成) 记录故障(L"未发表查询身份回收失败，保留现场");
        ids.clear();
    } catch (...) { 记录故障(L"取消私有查询准备发生工程异常，保留现场"); }
}
void 登记查询索引(const 新词典查询记录信息& r) {
    std::lock_guard lock(记录互斥);
    查询索引[{r.定位.外设场景, r.定位.调用来源标识}] = r.查询记录;
    for (std::size_t i = 0; i < r.有序义项消息.size(); ++i)
        消息索引[r.有序义项消息[i]] = {r.查询记录, i + 1};
    if (r.接收处理) 续接索引[r.查询记录] = r.接收处理->处理记录;
}
struct 查询准备内容 {
    const 外设词典记录类* owner = nullptr;
    std::thread::id 线程 = std::this_thread::get_id();
    std::unique_ptr<查询占用> 占用;
    std::shared_ptr<新词典接收场景提供接口> 提供者;
    新词典查询事实 事实;
    新词典查询记录信息 记录;
    std::vector<稳定编码> 私有身份, 正文;
    bool 已发表 = false, 已取消 = false;
    std::optional<基础片段发布回执> 回执;
    void 核对(const 外设词典记录类* o) const noexcept {
        if (owner != o || 线程 != std::this_thread::get_id()) 记录故障(L"查询准备跨owner或跨线程使用");
    }
    ~查询准备内容() noexcept {
        if (线程 != std::this_thread::get_id()) 记录故障(L"查询准备不能跨线程销毁");
        if (!已发表) 清理私有身份(私有身份);
    }
};
// B和新S同片段；共同发表回执必须先标记整个R/M准备已发表，再允许任何收尾。
class 接收结果参与 final : public 新状态结果引用参与接口 {
public:
    接收结果参与(查询准备内容& p, 新词典接收准备投影 v, 基础联合节点快照 r)
        : 内容(p), 投影(v), 查询宿主(std::move(r)) {}
    新状态结果追加构造 构造同组状态引用(const 新同组状态端点& s) const override {
        if (已构造 || s.被描述节点 != 内容.事实.定位.外设场景 || s.来源特征节点 != 投影.接收特征
            || s.强时间 != 投影.本轮状态时间 || !s.固定准确值
            || *s.固定准确值 != 新特征准确值{投影.新消息集合值})
            记录故障(L"词典接收状态端点与本次准备不符");
        已构造 = true;
        新状态结果追加构造 out;
        新词典接收处理信息 info;
        info.阶段 = 新词典接收处理阶段::接收已发表;
        info.前状态 = 投影.前状态;
        auto B = 构造新增续接(info);
        B.节点.字段组.push_back({续接后状态字段, std::visit([](const auto& v) -> 基础组内字段内容 { return v; }, s.状态定位)});
        B槽 = s.后续新节点起点; S端点 = s.状态定位;
        out.追加请求.新节点组.push_back(std::move(B.节点));
        构造新增查询续接边(out.追加请求, {内容.记录.查询记录}, 基础组内节点引用{B槽});
        out.追加请求.预期联合节点组.push_back(查询宿主);
        out.追加请求.只读预期.节点组.push_back({续接类型});
        if (投影.前状态) out.追加请求.只读预期.节点组.push_back({*投影.前状态});
        out.新增状态保持数 = 1;
        return out;
    }
    void 登记共同发表回执(const 基础片段发布回执& receipt) noexcept override {
        if (!已构造 || 内容.已发表 || B槽 >= receipt.新节点组().size())
            记录故障(L"词典接收共同回执缺少本次B");
        稳定编码 state;
        if (const auto* n = std::get_if<稳定编码>(&S端点)) state = *n;
        else {
            const auto slot = std::get<基础组内节点引用>(S端点).索引;
            if (slot >= receipt.新节点组().size()) 记录故障(L"词典接收共同回执缺少本次S");
            state = receipt.新节点组()[slot];
        }
        内容.回执 = receipt;
        内容.已发表 = true;
        内容.记录.接收处理 = 新词典接收处理信息{receipt.新节点组()[B槽],
            新词典接收处理阶段::接收已发表, 投影.前状态, state, {}, {}};
    }
private:
    查询准备内容& 内容;
    新词典接收准备投影 投影;
    基础联合节点快照 查询宿主;
    mutable bool 已构造 = false;
    mutable std::size_t B槽 = 0;
    mutable std::variant<稳定编码, 基础组内节点引用> S端点;
};
基础节点组追加请求 构造查询身份请求(const 查询准备内容& p) {
    基础节点组追加请求 r;
    const auto batch = 全局基础数据集.查询节点组完整结构(p.私有身份);
    if (batch.状态 == 基础结构操作状态::资源失败) throw S::资源失败;
    if (batch.状态 != 基础结构操作状态::已完成 || batch.节点组.size() != p.私有身份.size())
        记录故障(L"私有查询身份读取失败");
    for (const auto& x : batch.节点组) {
        if (!x.快照 || !x.快照->结构.字段组.empty() || !x.快照->入向字段组.empty()
            || !x.快照->入向外部关系组.empty() || !x.快照->出向外部关系组.empty())
            记录故障(L"查询发表前的身份不再私有");
        r.预期联合节点组.push_back(*x.快照);
    }
    追加完整字段(r, p.记录.查询记录, 构造新增查询(p.事实).节点);
    for (std::size_t i = 0; i < p.事实.义项组.size(); ++i) {
        追加完整字段(r, p.记录.有序义项消息[i], 构造新增义项消息(p.事实.义项组[i], p.正文[i]).节点);
        构造新增查询消息边(r, {p.记录.查询记录}, {p.记录.有序义项消息[i]}, static_cast<std::int64_t>(i + 1));
    }
    加入字段引用身份(r);
    return r;
}
S 查询当前完成状态(const 新词典查询记录信息& r) noexcept {
    return r.接收处理 && (r.接收处理->阶段 == 新词典接收处理阶段::接收已发表
        || r.接收处理->阶段 == 新词典接收处理阶段::动态已建立) ? S::尚需续接 : S::已完成;
}
struct 未发表接收清理 final {
    std::unique_ptr<新词典接收参与准备>& 准备;
    bool 已发表 = false;
    ~未发表接收清理() noexcept { if (准备 && !已发表) 准备->取消未发表准备(); }
};
} // namespace

struct 新词典查询准备::实现 : 查询准备内容 {};
新词典查询准备::新词典查询准备(std::unique_ptr<实现> p) noexcept : 实现_(std::move(p)) {}
新词典查询准备::~新词典查询准备() = default;
新词典查询准备::新词典查询准备(新词典查询准备&&) noexcept = default;
新词典查询准备& 新词典查询准备::operator=(新词典查询准备&&) noexcept = default;

新词典查询准备结果 外设词典记录类::准备查询记录(const 新词典查询事实& f,
    std::shared_ptr<新词典接收场景提供接口> provider) noexcept {
    try {
        if (!记录结构就绪.load(std::memory_order_acquire)) return {S::未装配, {}, {}};
        if (!有效(f.定位.外设场景) || !f.定位.调用来源标识 || !有效(f.词典说话者)
            || f.查询语种.empty() || f.查询词面.empty() || f.清单摘要.empty() || f.原始包标识.empty()
            || f.查询完成时间.纳秒 < 0
            || (f.查询完成时间.语义 != 新状态时间语义::实例绝对UTC
                && f.查询完成时间.语义 != 新状态时间语义::抽象相对)
            || (f.结局 != 新词典查阅结局::命中 && f.结局 != 新词典查阅结局::正式未命中)
            || (f.结局 == 新词典查阅结局::命中) != !f.义项组.empty()) return {S::输入拒绝, {}, {}};
        if (f.结局 == 新词典查阅结局::命中 && !provider) return {S::未装配, {}, {}};
        auto p = std::make_unique<新词典查询准备::实现>();
        p->owner = this;
        p->占用 = std::make_unique<查询占用>(查询键{f.定位.外设场景, f.定位.调用来源标识});
        const auto existing = 按调用来源读取查询记录(f.定位);
        if (existing.状态 == S::已完成 && existing.信息) return {S::已复用, {}, existing.信息};
        if (existing.状态 != S::未找到) return {existing.状态, {}, {}};
        p->事实 = f; p->提供者 = std::move(provider);
        p->记录.定位 = f.定位; p->记录.词典说话者 = f.词典说话者;
        p->记录.查询语种 = f.查询语种; p->记录.查询词面 = f.查询词面;
        p->记录.清单摘要 = f.清单摘要; p->记录.原始包标识 = f.原始包标识;
        p->记录.查询完成时间 = f.查询完成时间; p->记录.结局 = f.结局;
        p->记录.有序义项消息.resize(f.义项组.size());
        p->正文.reserve(f.义项组.size());
        for (const auto& v : f.义项组) {
            const auto body = 值服务_.保存或取得特征值(材料物理类型::UTF8字符串, 新特征值材料{v.汉语释义});
            if (!有效(body)) return {S::资源失败, {}, {}};
            p->正文.push_back(body);
        }
        基础节点组追加请求 reserve;
        reserve.新节点组.resize(f.义项组.size() + 1);
        auto result = 全局基础数据集.原子追加节点组(reserve);
        if (result.状态 == L::资源失败) return {S::资源失败, {}, {}};
        if (result.状态 != L::已完成 || result.新节点组.size() != reserve.新节点组.size())
            记录故障(L"词典查询私有身份预留失败");
        p->私有身份 = std::move(result.新节点组);
        p->记录.查询记录 = p->私有身份.front();
        for (std::size_t i = 0; i < f.义项组.size(); ++i) p->记录.有序义项消息[i] = p->私有身份[i + 1];
        return {S::已完成, 新词典查询准备{std::move(p)}, {}};
    } catch (const std::bad_alloc&) { return {S::资源失败, {}, {}}; }
    catch (const std::length_error&) { return {S::资源失败, {}, {}}; }
    catch (...) { 记录故障(L"准备词典查询记录异常"); }
}
新词典查询发表结果 外设词典记录类::提交查询接收(新词典查询准备& prepared) noexcept {
    try {
        if (!prepared.实现_) return {S::输入拒绝, {}};
        auto& p = *prepared.实现_;
        p.核对(this);
        if (p.已取消) return {S::输入拒绝, {}};
        if (p.已发表) {
            p.占用.reset();
            auto current = 读取查询记录(p.记录.查询记录);
            if (!current.信息) return {current.状态, {}};
            return {查询当前完成状态(*current.信息), std::move(current.信息)};
        }
        for (;;) {
            std::unique_ptr<新词典接收参与准备> receive;
            未发表接收清理 cleanup{receive};
            if (p.事实.结局 == 新词典查阅结局::命中) {
                auto r = p.提供者->准备接收(p.事实.定位.外设场景, p.记录.有序义项消息);
                if (r.状态 != S::已完成) return {r.状态, {}};
                if (!r.准备) 记录故障(L"词典接收准备成功但没有准备");
                receive = std::move(r.准备);
            }
            // 场景提供者此时只持读取材料，尚未取得F资格。
            auto request = 构造查询身份请求(p);
            const auto seal = 全局基础数据集.封存原子追加片段(request);
            if (seal.状态 != L::已完成) {
                if (正常零发表(seal.状态)) continue;
                if (seal.状态 == L::资源失败) return {S::资源失败, {}};
                记录故障(L"词典查询身份封存失败");
            }
            if (!seal.片段) 记录故障(L"词典查询身份封存缺片段");
            if (!receive) {
                const auto done = 全局基础数据集.原子追加封存片段组({*seal.片段});
                if (done.状态 == L::已完成) {
                    if (done.片段回执组.size() != 1) 记录故障(L"未命中查询发表缺回执");
                    p.回执 = done.片段回执组.front(); p.已发表 = true;
                } else {
                    if (正常零发表(done.状态)) continue;
                    if (done.状态 == L::资源失败) return {S::资源失败, {}};
                    记录故障(L"未命中查询发表失败");
                }
            } else {
                const auto projection = receive->读取接收投影();
                const auto host = std::find_if(request.预期联合节点组.begin(), request.预期联合节点组.end(),
                    [&](const auto& n) { return n.结构.节点.编码 == p.记录.查询记录; });
                if (host == request.预期联合节点组.end()) 记录故障(L"本轮私有查询宿主缺失");
                接收结果参与 participant{p, projection, *host};
                新消息待发表凭据 credential{p.记录.有序义项消息, request.预期联合节点组, *seal.片段};
                const auto done = receive->提交接收(std::move(credential), participant);
                if (done.状态 != L::已完成) {
                    if (p.已发表) 记录故障(L"词典共同发表已登记却返回零发表");
                    if (正常零发表(done.状态)) continue;
                    if (done.状态 == L::资源失败) return {S::资源失败, {}};
                    记录故障(L"词典接收共同发表失败");
                }
                if (!p.已发表) 记录故障(L"接收成功返回前未登记查询发表事实");
                cleanup.已发表 = true;
            }
            // 回执已保存；后续资源故障不允许取消或重放查询。
            登记查询索引(p.记录);
            p.占用.reset();
            return {查询当前完成状态(p.记录), p.记录};
        }
    } catch (S status) { return {status, {}}; }
    catch (const std::bad_alloc&) { return {S::资源失败, {}}; }
    catch (const std::length_error&) { return {S::资源失败, {}}; }
    catch (...) { 记录故障(L"提交词典查询异常，保留已发表事实"); }
}
void 外设词典记录类::取消未发表查询(新词典查询准备& prepared) noexcept {
    if (!prepared.实现_) return;
    auto& p = *prepared.实现_; p.核对(this);
    if (p.已发表) { p.占用.reset(); return; }
    清理私有身份(p.私有身份);
    p.已取消 = true; p.占用.reset();
}

namespace {
// 仅收集本次实际删除引用的目标；字段键和目标的其它引用不属于删除闭包。
void 补齐删除引用宿主(基础节点组追加请求& request) {
    std::set<稳定编码> hosts, targets;
    for (const auto& n : request.预期联合节点组) {
        hosts.insert(n.结构.节点.编码);
        for (const auto& f : n.结构.字段组) {
            if (std::find(request.删除字段关系组.begin(), request.删除字段关系组.end(), f.编码)
                == request.删除字段关系组.end()) continue;
            if (const auto* target = std::get_if<稳定编码>(&f.内容)) targets.insert(*target);
        }
    }
    for (const auto n : hosts) targets.erase(n);
    if (targets.empty()) return;
    const auto batch = 全局基础数据集.查询节点组完整结构({targets.begin(), targets.end()});
    if (batch.状态 == 基础结构操作状态::资源失败) throw S::资源失败;
    if (batch.状态 != 基础结构操作状态::已完成 || batch.节点组.size() != targets.size())
        记录故障(L"记录解除引用的目标读取失败");
    for (const auto& n : batch.节点组) {
        if (!n.快照) 记录故障(L"仍受记录保持的引用目标不存在");
        request.预期联合节点组.push_back(*n.快照);
    }
}
void 构造删除查询(基础节点组追加请求& r, const 基础联合节点快照& n) {
    const 查询字段投影 p{n.结构};
    构造删除查询类型(r, p); 构造删除查询外设场景(r, p);
    构造删除查询说话者(r, p); 构造删除查询调用标识(r, p);
    构造删除查询语种(r, p); 构造删除查询词面(r, p);
    构造删除查询批次摘要(r, p); 构造删除查询包标识(r, p);
    构造删除查询时间语义(r, p); 构造删除查询时间纳秒(r, p);
    构造删除查询结局(r, p);
    r.删除节点组.push_back(n.结构.节点.编码);
}
void 构造删除义项消息(基础节点组追加请求& r, const 基础联合节点快照& n) {
    const 消息字段投影 p{n.结构};
    构造删除消息类型(r, p); 构造删除消息汉语释义正文(r, p);
    构造删除消息记录键(r, p); 构造删除消息词头语种(r, p);
    构造删除消息词面(r, p); 构造删除消息义项键(r, p);
    构造删除消息例句原文(r, p); 构造删除消息例句汉译(r, p);
    构造删除消息读音(r, p); 构造删除消息来源页URL(r, p);
    构造删除消息来源版本(r, p); 构造删除消息许可标识(r, p);
    构造删除消息署名(r, p); 构造删除消息原始来源定位(r, p);
    r.删除节点组.push_back(n.结构.节点.编码);
}
void 构造删除接收续接记录(基础节点组追加请求& r, const 基础联合节点快照& n) {
    const 续接字段投影 p{n.结构};
    构造删除续接类型(r, p); 构造删除续接阶段(r, p);
    构造删除续接前状态(r, p); 构造删除续接后状态(r, p);
    构造删除续接变化动态(r, p); 构造删除续接无需动态原因(r, p);
    r.删除节点组.push_back(n.结构.节点.编码);
}

class 处理阶段参与 final : public 新词典处理阶段参与接口 {
public:
    explicit 处理阶段参与(const 基础联合节点快照& n) : 当前(n), 信息(读取续接(n)) {
        // 在进入提供者之前读取删除资格；构造及封存期间不再读世界。
        收束请求.预期联合节点组.push_back(n);
        const 续接字段投影 p{n.结构};
        构造删除续接前状态(收束请求, p);
        构造删除续接后状态(收束请求, p);
        构造删除续接变化动态(收束请求, p);
        补齐删除引用宿主(收束请求);
    }
    新词典接收处理信息 读取本轮处理材料() const override { return 信息; }
    基础封存追加片段 构造动态已建立(稳定编码 dynamic) const override {
        if (已构造 || !有效(dynamic) || 信息.阶段 != 新词典接收处理阶段::接收已发表
            || !信息.前状态 || !信息.后状态 || 信息.动态)
            记录故障(L"不能从本次处理阶段构造动态已建立");
        已构造 = true;
        基础节点组追加请求 r;
        r.预期联合节点组.push_back(当前);
        r.只读预期.节点组.push_back({dynamic});
        r.追加字段组.push_back({信息.处理记录, 续接变化动态字段, dynamic});
        r.替换字段组.push_back({查字段(当前.结构, 续接阶段字段)->编码,
            基础值{static_cast<std::int64_t>(新词典接收处理阶段::动态已建立)}});
        return 封存(std::move(r));
    }
    基础封存追加片段 构造处理收束(新词典接收处理阶段 stage,
        std::optional<新词条动态记录原因> reason) const override {
        if (已构造 || (stage != 新词典接收处理阶段::场景已挂接 && stage != 新词典接收处理阶段::无需动态)
            || (stage == 新词典接收处理阶段::场景已挂接
                && (信息.阶段 != 新词典接收处理阶段::动态已建立 || reason))
            || (stage == 新词典接收处理阶段::无需动态
                && (信息.阶段 != 新词典接收处理阶段::接收已发表 || !reason
                    || (*reason != 新词条动态记录原因::无值变化 && *reason != 新词条动态记录原因::无前态
                        && *reason != 新词条动态记录原因::时间不递增))))
            记录故障(L"词典接收收束阶段或原因不符");
        已构造 = true;
        auto r = 收束请求;
        r.替换字段组.push_back({查字段(当前.结构, 续接阶段字段)->编码,
            基础值{static_cast<std::int64_t>(stage)}});
        if (reason) r.追加字段组.push_back({信息.处理记录, 续接无需动态原因字段,
            基础值{static_cast<std::int64_t>(*reason)}});
        return 封存(std::move(r));
    }
    void 登记共同发表回执(const 基础片段发布回执& receipt) noexcept override {
        if (!片段 || 回执 || !receipt.对应片段(*片段))
            记录故障(L"处理阶段回执与本次片段不符");
        回执 = receipt;
    }
    bool 已发表() const noexcept { return 回执.has_value(); }
private:
    基础封存追加片段 封存(基础节点组追加请求 r) const {
        const auto sealed = 全局基础数据集.封存原子追加片段(r);
        if (sealed.状态 == L::资源失败) throw S::资源失败;
        if (sealed.状态 != L::已完成 || !sealed.片段) 记录故障(L"处理阶段纯构造不能封存");
        片段 = *sealed.片段;
        return *片段;
    }
    基础联合节点快照 当前;
    新词典接收处理信息 信息;
    基础节点组追加请求 收束请求;
    mutable bool 已构造 = false;
    mutable std::optional<基础封存追加片段> 片段;
    std::optional<基础片段发布回执> 回执;
};
} // namespace

新词典查询读取结果 外设词典记录类::续接查询变化(
    稳定编码 R, 新词典接收场景提供接口& provider) noexcept {
    try {
        auto first = 读取查询记录(R);
        if (!first.信息) return first;
        查询占用 claim({first.信息->定位.外设场景, first.信息->定位.调用来源标识});
        for (;;) {
            新词典查询记录信息 info;
            std::unique_ptr<处理阶段参与> participant;
            {
                std::shared_lock lock(记录读取退出互斥);
                const auto all = 读取查询闭包(R);
                info = 读取查询(all, R);
                if (查询当前完成状态(info) == S::已完成) return {S::已完成, std::move(info)};
                participant = std::make_unique<处理阶段参与>(all.at(info.接收处理->处理记录));
            }
            const auto result = provider.续接变化(info.定位.外设场景, *participant);
            if (result.状态 == L::已完成) {
                if (!participant->已发表()) 记录故障(L"处理阶段成功但未登记回执");
                // 单次调用最多发表一个阶段；丢弃旧B材料，重新读取同一R再继续。
                continue;
            }
            if (participant->已发表()) 记录故障(L"处理阶段已发表却返回零发表");
            if (正常零发表(result.状态)) continue;
            if (result.状态 == L::引用仍存在) return {S::仍被引用, std::move(info)};
            if (result.状态 == L::资源失败) return {S::资源失败, std::move(info)};
            记录故障(L"词典接收续接失败，保留原记录及保持责任");
        }
    } catch (S status) { return {status, {}}; }
    catch (const std::bad_alloc&) { return {S::资源失败, {}}; }
    catch (const std::length_error&) { return {S::资源失败, {}}; }
    catch (...) { 记录故障(L"续接词典查询发生工程异常，保留已发表事实"); }
}

struct 新词典记录退出准备::实现 final {
    const 外设词典记录类* owner = nullptr;
    std::thread::id 线程 = std::this_thread::get_id();
    std::unique_ptr<查询占用> 占用;
    基础节点组追加请求 请求;
    std::optional<基础封存追加片段> 片段;
    std::optional<基础片段发布回执> 回执;
    bool 已封存 = false, 已结束 = false;
    void 核对(const 外设词典记录类* o) const noexcept {
        if (owner != o || 线程 != std::this_thread::get_id()) 记录故障(L"记录退出跨owner或线程使用");
    }
    void 清理索引() noexcept {
        if (!回执 || 已结束) return;
        std::lock_guard lock(记录互斥);
        for (auto i = 查询索引.begin(); i != 查询索引.end(); ) {
            if (std::find(请求.删除节点组.begin(), 请求.删除节点组.end(), i->second) != 请求.删除节点组.end())
                i = 查询索引.erase(i);
            else ++i;
        }
        for (const auto n : 请求.删除节点组) { 消息索引.erase(n); 续接索引.erase(n); }
        已结束 = true;
    }
    ~实现() noexcept {
        if (线程 != std::this_thread::get_id()) 记录故障(L"记录退出准备不能跨线程销毁");
        清理索引();
    }
};
新词典记录退出准备::新词典记录退出准备(std::unique_ptr<实现> p) noexcept : 实现_(std::move(p)) {}
新词典记录退出准备::~新词典记录退出准备() = default;
新词典记录退出准备::新词典记录退出准备(新词典记录退出准备&&) noexcept = default;
新词典记录退出准备& 新词典记录退出准备::operator=(新词典记录退出准备&&) noexcept = default;

新词典记录退出准备结果 外设词典记录类::准备查询记录退出(稳定编码 R,
    新词典记录退出用途 use, const 新词典隔离退出读取接口* reader) noexcept {
    try {
        if (use != 新词典记录退出用途::独立记录退出 && use != 新词典记录退出用途::隔离整样例退出)
            return {S::输入拒绝, {}};
        const auto first = 读取查询记录(R);
        if (!first.信息) return {first.状态, {}};
        auto p = std::make_unique<新词典记录退出准备::实现>();
        p->owner = this;
        p->占用 = std::make_unique<查询占用>(查询键{first.信息->定位.外设场景, first.信息->定位.调用来源标识});
        // 不持记录互斥调用隔离owner；同R资格阻止续接和另一次退出。
        if (use == 新词典记录退出用途::隔离整样例退出) {
            if (!reader) return {S::未装配, {}};
            const auto status = reader->读取本样例外设场景(first.信息->定位.外设场景);
            if (status != S::已完成 && status != S::已复用) return {status, {}};
        }
        std::unique_lock lock(记录读取退出互斥);
        const auto all = 读取查询闭包(R);
        const auto info = 读取查询(all, R);
        if (use == 新词典记录退出用途::独立记录退出 && 查询当前完成状态(info) != S::已完成)
            return {S::尚需续接, {}};
        for (const auto& [id, n] : all) { (void)id; p->请求.预期联合节点组.push_back(n); }
        const auto& root = all.at(R);
        for (const auto& e : 读取查询消息边(root)) {
            构造删除义项消息(p->请求, all.at(e.目标节点));
            构造删除查询消息边(p->请求, e);
        }
        if (const auto e = 读取查询续接边(root)) {
            构造删除接收续接记录(p->请求, all.at(e->目标节点));
            构造删除查询续接边(p->请求, *e);
        }
        构造删除查询(p->请求, root);
        补齐删除引用宿主(p->请求);
        // 不因其它owner本轮尚未移除的引用提前拒绝，交组合发表统一判断。
        return {S::已完成, 新词典记录退出准备{std::move(p)}};
    } catch (S status) { return {status, {}}; }
    catch (const std::bad_alloc&) { return {S::资源失败, {}}; }
    catch (const std::length_error&) { return {S::资源失败, {}}; }
    catch (...) { 记录故障(L"准备词典记录退出异常"); }
}
基础封存追加片段 外设词典记录类::封存查询记录退出(新词典记录退出准备& prepared) const {
    if (!prepared.实现_) throw S::输入拒绝;
    auto& p = *prepared.实现_; p.核对(this);
    if (p.已封存 || p.已结束) throw S::输入拒绝;
    p.已封存 = true;
    const auto sealed = 全局基础数据集.封存原子追加片段(p.请求);
    if (sealed.状态 == L::资源失败) throw S::资源失败;
    if (sealed.状态 != L::已完成 || !sealed.片段) 记录故障(L"记录退出纯构造不能封存");
    p.片段 = *sealed.片段;
    return *p.片段;
}
void 外设词典记录类::登记查询记录退出发表(
    新词典记录退出准备& prepared, const 基础片段发布回执& receipt) noexcept {
    if (!prepared.实现_) 记录故障(L"退出回执缺少准备");
    auto& p = *prepared.实现_; p.核对(this);
    if (p.已结束 || p.回执 || !p.片段 || !receipt.对应片段(*p.片段))
        记录故障(L"记录退出回执不对应本轮准备");
    p.回执 = receipt;
}
void 外设词典记录类::完成查询记录退出(新词典记录退出准备& prepared) noexcept {
    if (!prepared.实现_) return;
    auto& p = *prepared.实现_; p.核对(this);
    if (p.已结束) return;
    if (!p.回执) 记录故障(L"未发表退出不能完成");
    p.清理索引(); p.占用.reset();
}
void 外设词典记录类::取消查询记录退出(新词典记录退出准备& prepared) noexcept {
    if (!prepared.实现_) return;
    auto& p = *prepared.实现_; p.核对(this);
    if (p.已结束) return;
    if (p.回执) 记录故障(L"已发表记录退出必须完成，不能取消");
    p.已结束 = true;
    p.占用.reset();
}
} // namespace 海中鱼巣
