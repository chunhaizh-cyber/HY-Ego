module;

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <atomic>
#include <chrono>
#include <cstdint>
#include <functional>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <vector>
export module 后台前端通道模块;

import 日志模块;

export inline constexpr wchar_t HYEgo后台管道名[] = L"\\\\.\\pipe\\HYEgo.Backend.V1";

export struct 结构_后台前端快照 {
    bool 后台在线 = false;
    bool 宿主运行中 = false;
    bool 请求停止 = false;
    bool 自我线程运行中 = false;
    bool 初始化完成 = false;
    bool 健康运行 = false;
    bool 请求安全重启 = false;
    std::uint32_t 致命错误计数 = 0;
    std::uint32_t 已执行重启次数 = 0;
    bool 相机已启用 = false;
    bool 相机运行中 = false;
    bool 相机使用虚拟回退 = false;
    std::uint64_t 相机已处理帧数 = 0;
    std::uint64_t 最近桥接候选数 = 0;
    std::uint64_t 世界根子节点数 = 0;
    std::uint64_t 需求数 = 0;
    std::uint64_t 任务数 = 0;
    std::uint64_t 安全任务数 = 0;
    std::uint64_t 服务任务数 = 0;
    std::uint64_t 叶子任务数 = 0;
    std::uint64_t 方法数 = 0;
    std::uint64_t 状态数 = 0;
    std::uint64_t 动态数 = 0;
    std::uint64_t 事件动态数 = 0;
    std::uint64_t 因果数 = 0;
    std::int64_t 当前安全值 = 0;
    bool 有当前安全值 = false;
    std::int64_t 当前服务值 = 0;
    bool 有当前服务值 = false;
    std::int64_t 快照生成UTC毫秒 = 0;
    std::uint64_t 最近输入序号 = 0;
    std::int64_t 主要需求权重 = 0;
    bool 主要需求已满足 = false;
    std::string 主要需求类型;
    std::string 最近输入文本;
    std::string 最近命令摘要;
    std::string 最近故障摘要;
    std::string 宿主故障摘要;
    std::string 相机状态摘要;
    std::string 因果链摘要;
};

export struct 结构_后台自我场景存在快照 {
    std::string 存在主键;
    bool 有位置 = false;
    std::int64_t 中心X_mm = 0;
    std::int64_t 中心Y_mm = 0;
    std::int64_t 中心Z_mm = 0;
    bool 有尺寸 = false;
    std::int64_t 尺寸X_mm = 0;
    std::int64_t 尺寸Y_mm = 0;
    std::int64_t 尺寸Z_mm = 0;
    std::uint64_t 轮廓块数 = 0;
    std::uint32_t 连续命中帧 = 0;
    std::uint32_t 连续未命中帧 = 0;
    std::uint32_t 连续静止帧 = 0;
};

export struct 结构_后台自我场景快照 {
    bool 后台在线 = false;
    bool 再现运行中 = false;
    std::uint64_t 帧序号 = 0;
    std::string 场景主键;
    std::string 状态摘要;
    std::vector<结构_后台自我场景存在快照> 存在列表{};
};

namespace 后台前端通道模块_内部 {
    struct 结构_响应 {
        bool 成功 = false;
        std::string 消息;
        结构_后台前端快照 快照{};
        结构_后台自我场景快照 自我场景{};
    };

    class 管道安全属性包装 {
    public:
        管道安全属性包装() {
            属性_.nLength = sizeof(属性_);
            属性_.bInheritHandle = FALSE;
            if (::InitializeSecurityDescriptor(&安全描述符_, SECURITY_DESCRIPTOR_REVISION) &&
                ::SetSecurityDescriptorDacl(&安全描述符_, TRUE, nullptr, FALSE)) {
                属性_.lpSecurityDescriptor = &安全描述符_;
            }
        }

        ~管道安全属性包装() = default;

        SECURITY_ATTRIBUTES* 取() {
            return 属性_.lpSecurityDescriptor ? &属性_ : nullptr;
        }

    private:
        SECURITY_ATTRIBUTES 属性_{};
        SECURITY_DESCRIPTOR 安全描述符_{};
    };

    inline std::string 转义(std::string_view 输入) {
        std::string 输出;
        输出.reserve(输入.size());
        for (char c : 输入) {
            switch (c) {
            case '\\': 输出 += "\\\\"; break;
            case '\n': 输出 += "\\n"; break;
            case '\r': 输出 += "\\r"; break;
            default: 输出.push_back(c); break;
            }
        }
        return 输出;
    }

    inline std::string 反转义(std::string_view 输入) {
        std::string 输出;
        输出.reserve(输入.size());
        bool 转义态 = false;
        for (char c : 输入) {
            if (!转义态) {
                if (c == '\\') {
                    转义态 = true;
                }
                else {
                    输出.push_back(c);
                }
                continue;
            }

            switch (c) {
            case 'n': 输出.push_back('\n'); break;
            case 'r': 输出.push_back('\r'); break;
            case '\\': 输出.push_back('\\'); break;
            default: 输出.push_back(c); break;
            }
            转义态 = false;
        }
        if (转义态) 输出.push_back('\\');
        return 输出;
    }

    inline void 追加字段(std::string& 输出, const char* 键, std::string_view 值) {
        输出 += 键;
        输出.push_back('=');
        输出 += 转义(值);
        输出.push_back('\n');
    }

    inline std::string 序列化(const 结构_后台前端快照& 快照) {
        std::string 输出;
        输出.reserve(4096);
        追加字段(输出, "backend_online", 快照.后台在线 ? "1" : "0");
        追加字段(输出, "host_running", 快照.宿主运行中 ? "1" : "0");
        追加字段(输出, "stop_requested", 快照.请求停止 ? "1" : "0");
        追加字段(输出, "self_running", 快照.自我线程运行中 ? "1" : "0");
        追加字段(输出, "initialized", 快照.初始化完成 ? "1" : "0");
        追加字段(输出, "healthy", 快照.健康运行 ? "1" : "0");
        追加字段(输出, "restart_requested", 快照.请求安全重启 ? "1" : "0");
        追加字段(输出, "fatal_count", std::to_string(快照.致命错误计数));
        追加字段(输出, "restart_count", std::to_string(快照.已执行重启次数));
        追加字段(输出, "camera_enabled", 快照.相机已启用 ? "1" : "0");
        追加字段(输出, "camera_running", 快照.相机运行中 ? "1" : "0");
        追加字段(输出, "camera_virtual", 快照.相机使用虚拟回退 ? "1" : "0");
        追加字段(输出, "camera_frames", std::to_string(快照.相机已处理帧数));
        追加字段(输出, "camera_candidates", std::to_string(快照.最近桥接候选数));
        追加字段(输出, "world_count", std::to_string(快照.世界根子节点数));
        追加字段(输出, "need_count", std::to_string(快照.需求数));
        追加字段(输出, "task_count", std::to_string(快照.任务数));
        追加字段(输出, "safe_task_count", std::to_string(快照.安全任务数));
        追加字段(输出, "service_task_count", std::to_string(快照.服务任务数));
        追加字段(输出, "leaf_task_count", std::to_string(快照.叶子任务数));
        追加字段(输出, "method_count", std::to_string(快照.方法数));
        追加字段(输出, "state_count", std::to_string(快照.状态数));
        追加字段(输出, "dynamic_count", std::to_string(快照.动态数));
        追加字段(输出, "event_dynamic_count", std::to_string(快照.事件动态数));
        追加字段(输出, "causality_count", std::to_string(快照.因果数));
        追加字段(输出, "self_safety_value", std::to_string(快照.当前安全值));
        追加字段(输出, "has_self_safety", 快照.有当前安全值 ? "1" : "0");
        追加字段(输出, "self_service_value", std::to_string(快照.当前服务值));
        追加字段(输出, "has_self_service", 快照.有当前服务值 ? "1" : "0");
        追加字段(输出, "snapshot_utc_ms", std::to_string(快照.快照生成UTC毫秒));
        追加字段(输出, "last_input_seq", std::to_string(快照.最近输入序号));
        追加字段(输出, "top_need_weight", std::to_string(快照.主要需求权重));
        追加字段(输出, "top_need_satisfied", 快照.主要需求已满足 ? "1" : "0");
        追加字段(输出, "top_need_type", 快照.主要需求类型);
        追加字段(输出, "last_input_text", 快照.最近输入文本);
        追加字段(输出, "last_command", 快照.最近命令摘要);
        追加字段(输出, "recent_fault", 快照.最近故障摘要);
        追加字段(输出, "host_fault", 快照.宿主故障摘要);
        追加字段(输出, "camera_status", 快照.相机状态摘要);
        追加字段(输出, "causality_detail", 快照.因果链摘要);
        return 输出;
    }

    inline std::unordered_map<std::string, std::string> 解析字段表(std::string_view 文本) {
        std::unordered_map<std::string, std::string> 字段;
        std::size_t 起点 = 0;
        while (起点 < 文本.size()) {
            const std::size_t 换行 = 文本.find('\n', 起点);
            const std::size_t 行尾 = (换行 == std::string_view::npos) ? 文本.size() : 换行;
            const std::string_view 行 = 文本.substr(起点, 行尾 - 起点);
            if (!行.empty()) {
                const std::size_t 等号 = 行.find('=');
                if (等号 != std::string_view::npos) {
                    字段.emplace(
                        std::string(行.substr(0, 等号)),
                        反转义(行.substr(等号 + 1)));
                }
            }
            if (换行 == std::string_view::npos) break;
            起点 = 换行 + 1;
        }
        return 字段;
    }

    inline bool 解析布尔(const std::unordered_map<std::string, std::string>& 字段, const char* 键, bool 默认值 = false) {
        const auto it = 字段.find(键);
        if (it == 字段.end()) return 默认值;
        return it->second == "1" || it->second == "true" || it->second == "yes";
    }

    inline std::uint64_t 解析非负64位(const std::unordered_map<std::string, std::string>& 字段, const char* 键, std::uint64_t 默认值 = 0) {
        const auto it = 字段.find(键);
        if (it == 字段.end()) return 默认值;
        try {
            return static_cast<std::uint64_t>(std::stoull(it->second));
        }
        catch (...) {
            return 默认值;
        }
    }

    inline std::int64_t 解析I64(const std::unordered_map<std::string, std::string>& 字段, const char* 键, std::int64_t 默认值 = 0) {
        const auto it = 字段.find(键);
        if (it == 字段.end()) return 默认值;
        try {
            return static_cast<std::int64_t>(std::stoll(it->second));
        }
        catch (...) {
            return 默认值;
        }
    }

    inline std::string 解析文本(const std::unordered_map<std::string, std::string>& 字段, const char* 键) {
        const auto it = 字段.find(键);
        return it == 字段.end() ? std::string{} : it->second;
    }

    inline bool 反序列化(std::string_view 文本, 结构_后台前端快照* 输出) {
        if (!输出) return false;
        const auto 字段 = 解析字段表(文本);

        输出->后台在线 = 解析布尔(字段, "backend_online", true);
        输出->宿主运行中 = 解析布尔(字段, "host_running");
        输出->请求停止 = 解析布尔(字段, "stop_requested");
        输出->自我线程运行中 = 解析布尔(字段, "self_running");
        输出->初始化完成 = 解析布尔(字段, "initialized");
        输出->健康运行 = 解析布尔(字段, "healthy");
        输出->请求安全重启 = 解析布尔(字段, "restart_requested");
        输出->致命错误计数 = static_cast<std::uint32_t>(解析非负64位(字段, "fatal_count"));
        输出->已执行重启次数 = static_cast<std::uint32_t>(解析非负64位(字段, "restart_count"));
        输出->相机已启用 = 解析布尔(字段, "camera_enabled");
        输出->相机运行中 = 解析布尔(字段, "camera_running");
        输出->相机使用虚拟回退 = 解析布尔(字段, "camera_virtual");
        输出->相机已处理帧数 = 解析非负64位(字段, "camera_frames");
        输出->最近桥接候选数 = 解析非负64位(字段, "camera_candidates");
        输出->世界根子节点数 = 解析非负64位(字段, "world_count");
        输出->需求数 = 解析非负64位(字段, "need_count");
        输出->任务数 = 解析非负64位(字段, "task_count");
        输出->安全任务数 = 解析非负64位(字段, "safe_task_count");
        输出->服务任务数 = 解析非负64位(字段, "service_task_count");
        输出->叶子任务数 = 解析非负64位(字段, "leaf_task_count");
        输出->方法数 = 解析非负64位(字段, "method_count");
        输出->状态数 = 解析非负64位(字段, "state_count");
        输出->动态数 = 解析非负64位(字段, "dynamic_count");
        输出->事件动态数 = 解析非负64位(字段, "event_dynamic_count");
        输出->因果数 = 解析非负64位(字段, "causality_count");
        输出->当前安全值 = 解析I64(字段, "self_safety_value");
        输出->有当前安全值 = 解析布尔(字段, "has_self_safety");
        输出->当前服务值 = 解析I64(字段, "self_service_value");
        输出->有当前服务值 = 解析布尔(字段, "has_self_service");
        输出->快照生成UTC毫秒 = 解析I64(字段, "snapshot_utc_ms");
        输出->最近输入序号 = 解析非负64位(字段, "last_input_seq");
        输出->主要需求权重 = 解析I64(字段, "top_need_weight");
        输出->主要需求已满足 = 解析布尔(字段, "top_need_satisfied");
        输出->主要需求类型 = 解析文本(字段, "top_need_type");
        输出->最近输入文本 = 解析文本(字段, "last_input_text");
        输出->最近命令摘要 = 解析文本(字段, "last_command");
        输出->最近故障摘要 = 解析文本(字段, "recent_fault");
        输出->宿主故障摘要 = 解析文本(字段, "host_fault");
        输出->相机状态摘要 = 解析文本(字段, "camera_status");
        输出->因果链摘要 = 解析文本(字段, "causality_detail");
        return true;
    }

    inline std::string 序列化响应(const 结构_响应& 响应) {
        std::string 输出;
        输出.reserve(2048);
        追加字段(输出, "ok", 响应.成功 ? "1" : "0");
        追加字段(输出, "message", 响应.消息);
        输出 += 序列化(响应.快照);
        追加字段(输出, "scene_running", 响应.自我场景.再现运行中 ? "1" : "0");
        追加字段(输出, "scene_frame", std::to_string(响应.自我场景.帧序号));
        追加字段(输出, "scene_key", 响应.自我场景.场景主键);
        追加字段(输出, "scene_status", 响应.自我场景.状态摘要);
        追加字段(输出, "scene_count", std::to_string(响应.自我场景.存在列表.size()));
        for (std::size_t i = 0; i < 响应.自我场景.存在列表.size(); ++i) {
            const auto& 项 = 响应.自我场景.存在列表[i];
            const std::string 前缀 = "scene_exist_" + std::to_string(i) + "_";
            追加字段(输出, (前缀 + "key").c_str(), 项.存在主键);
            追加字段(输出, (前缀 + "has_pos").c_str(), 项.有位置 ? "1" : "0");
            追加字段(输出, (前缀 + "cx").c_str(), std::to_string(项.中心X_mm));
            追加字段(输出, (前缀 + "cy").c_str(), std::to_string(项.中心Y_mm));
            追加字段(输出, (前缀 + "cz").c_str(), std::to_string(项.中心Z_mm));
            追加字段(输出, (前缀 + "has_size").c_str(), 项.有尺寸 ? "1" : "0");
            追加字段(输出, (前缀 + "sx").c_str(), std::to_string(项.尺寸X_mm));
            追加字段(输出, (前缀 + "sy").c_str(), std::to_string(项.尺寸Y_mm));
            追加字段(输出, (前缀 + "sz").c_str(), std::to_string(项.尺寸Z_mm));
            追加字段(输出, (前缀 + "contour").c_str(), std::to_string(项.轮廓块数));
            追加字段(输出, (前缀 + "hit").c_str(), std::to_string(项.连续命中帧));
            追加字段(输出, (前缀 + "miss").c_str(), std::to_string(项.连续未命中帧));
            追加字段(输出, (前缀 + "still").c_str(), std::to_string(项.连续静止帧));
        }
        return 输出;
    }

    inline bool 反序列化响应(std::string_view 文本, 结构_响应* 输出) {
        if (!输出) return false;
        const auto 字段 = 解析字段表(文本);
        输出->成功 = 解析布尔(字段, "ok");
        输出->消息 = 解析文本(字段, "message");
        if (!反序列化(文本, &输出->快照)) {
            return false;
        }

        输出->自我场景.后台在线 = 输出->快照.后台在线;
        输出->自我场景.再现运行中 = 解析布尔(字段, "scene_running");
        输出->自我场景.帧序号 = 解析非负64位(字段, "scene_frame");
        输出->自我场景.场景主键 = 解析文本(字段, "scene_key");
        输出->自我场景.状态摘要 = 解析文本(字段, "scene_status");
        const auto 场景存在数 = 解析非负64位(字段, "scene_count");
        输出->自我场景.存在列表.clear();
        输出->自我场景.存在列表.reserve(static_cast<std::size_t>(场景存在数));
        for (std::size_t i = 0; i < static_cast<std::size_t>(场景存在数); ++i) {
            const std::string 前缀 = "scene_exist_" + std::to_string(i) + "_";
            结构_后台自我场景存在快照 项{};
            项.存在主键 = 解析文本(字段, (前缀 + "key").c_str());
            项.有位置 = 解析布尔(字段, (前缀 + "has_pos").c_str());
            项.中心X_mm = 解析I64(字段, (前缀 + "cx").c_str());
            项.中心Y_mm = 解析I64(字段, (前缀 + "cy").c_str());
            项.中心Z_mm = 解析I64(字段, (前缀 + "cz").c_str());
            项.有尺寸 = 解析布尔(字段, (前缀 + "has_size").c_str());
            项.尺寸X_mm = 解析I64(字段, (前缀 + "sx").c_str());
            项.尺寸Y_mm = 解析I64(字段, (前缀 + "sy").c_str());
            项.尺寸Z_mm = 解析I64(字段, (前缀 + "sz").c_str());
            项.轮廓块数 = 解析非负64位(字段, (前缀 + "contour").c_str());
            项.连续命中帧 = static_cast<std::uint32_t>(解析非负64位(字段, (前缀 + "hit").c_str()));
            项.连续未命中帧 = static_cast<std::uint32_t>(解析非负64位(字段, (前缀 + "miss").c_str()));
            项.连续静止帧 = static_cast<std::uint32_t>(解析非负64位(字段, (前缀 + "still").c_str()));
            输出->自我场景.存在列表.push_back(std::move(项));
        }
        return true;
    }

    inline bool 读取完整消息(HANDLE 管道, std::string* 输出) {
        if (!输出) return false;
        输出->clear();
        char 缓冲[1024];
        while (true) {
            DWORD 已读 = 0;
            if (::ReadFile(管道, 缓冲, static_cast<DWORD>(sizeof(缓冲)), &已读, nullptr)) {
                if (已读 == 0) break;
                输出->append(缓冲, 缓冲 + 已读);
                if (已读 < sizeof(缓冲)) break;
                continue;
            }

            const DWORD 错误 = ::GetLastError();
            if (错误 == ERROR_MORE_DATA) {
                if (已读 > 0) 输出->append(缓冲, 缓冲 + 已读);
                continue;
            }
            return false;
        }
        return true;
    }

    inline bool 写入完整消息(HANDLE 管道, std::string_view 消息) {
        const char* 数据 = 消息.data();
        std::size_t 剩余 = 消息.size();
        while (剩余 > 0) {
            DWORD 已写 = 0;
            if (!::WriteFile(管道, 数据, static_cast<DWORD>(剩余), &已写, nullptr)) {
                return false;
            }
            if (已写 == 0) return false;
            数据 += 已写;
            剩余 -= 已写;
        }
        return ::FlushFileBuffers(管道) != FALSE;
    }
}

export class 后台前端服务类 {
public:
    using 命令回调 = std::function<void(
        const std::unordered_map<std::string, std::string>&,
        结构_后台前端快照*,
        bool*,
        std::string*,
        结构_后台自我场景快照*)>;

    explicit 后台前端服务类(命令回调 回调) : 回调_(std::move(回调)) {}
    ~后台前端服务类() { 停止(); }

    bool 启动() {
        bool 需要回收旧线程 = false;
        {
            std::lock_guard<std::mutex> 锁(互斥_);
            if (运行中_) return true;
            需要回收旧线程 = 工作线程_.joinable();
        }
        if (需要回收旧线程) {
            等待线程结束();
        }
        {
            std::lock_guard<std::mutex> 锁(互斥_);
            if (运行中_) return true;
            运行中_ = true;
            工作线程_ = std::thread([this] { 服务线程入口(); });
        }
        日志::运行("[后台前端服务] 启动");
        return true;
    }

    void 请求退出() {
        {
            std::lock_guard<std::mutex> 锁(互斥_);
            if (!运行中_) return;
            运行中_ = false;
        }
        私有_唤醒阻塞连接();
    }

    void 等待线程结束() {
        if (!工作线程_.joinable()) return;
        if (std::this_thread::get_id() == 工作线程_.get_id()) return;
        工作线程_.join();
    }

    void 停止() {
        请求退出();
        if (工作线程_.joinable()) {
            等待线程结束();
        }
    }

    bool 是否运行() const {
        return 运行中_.load();
    }

private:
    void 服务线程入口() {
        后台前端通道模块_内部::管道安全属性包装 安全属性;
        DWORD 最近建管错误 = ERROR_SUCCESS;
        日志::运行("[后台前端服务] 线程启动");
        while (运行中_.load()) {
            HANDLE 管道 = ::CreateNamedPipeW(
                HYEgo后台管道名,
                PIPE_ACCESS_DUPLEX,
                PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                1,
                8192,
                8192,
                0,
                安全属性.取());
            if (管道 == INVALID_HANDLE_VALUE) {
                const DWORD 错误 = ::GetLastError();
                if (错误 != 最近建管错误) {
                    最近建管错误 = 错误;
                    日志::运行_错误(
                        "[后台前端服务] CreateNamedPipeW 失败: err=" + std::to_string(错误));
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                continue;
            }
            最近建管错误 = ERROR_SUCCESS;

            const BOOL 已连接 = ::ConnectNamedPipe(管道, nullptr)
                ? TRUE
                : (::GetLastError() == ERROR_PIPE_CONNECTED);
            if (!已连接) {
                ::CloseHandle(管道);
                if (!运行中_.load()) break;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

            std::string 请求;
            if (后台前端通道模块_内部::读取完整消息(管道, &请求)) {
                后台前端通道模块_内部::结构_响应 响应{};
                if (回调_) {
                    const auto 字段 = 后台前端通道模块_内部::解析字段表(请求);
                    回调_(字段, &响应.快照, &响应.成功, &响应.消息, &响应.自我场景);
                }
                const std::string 响应文本 = 后台前端通道模块_内部::序列化响应(响应);
                (void)后台前端通道模块_内部::写入完整消息(管道, 响应文本);
            }

            ::DisconnectNamedPipe(管道);
            ::CloseHandle(管道);
        }
        日志::运行("[后台前端服务] 线程结束");
    }

    void 私有_唤醒阻塞连接() {
        HANDLE 客户端 = ::CreateFileW(
            HYEgo后台管道名,
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            0,
            nullptr);
        if (客户端 == INVALID_HANDLE_VALUE) return;
        const std::string 消息 = "STOP\n";
        (void)后台前端通道模块_内部::写入完整消息(客户端, 消息);
        ::CloseHandle(客户端);
    }

private:
    mutable std::mutex 互斥_;
    std::atomic_bool 运行中_{ false };
    命令回调 回调_{};
    std::thread 工作线程_{};
};

export class 后台前端客户端类 {
public:
    bool 查询快照(结构_后台前端快照* 输出, DWORD 超时毫秒 = 500) const {
        if (!输出) return false;
        输出->后台在线 = false;

        后台前端通道模块_内部::结构_响应 响应{};
        if (!私有_发送请求("command=snapshot\n", &响应, 超时毫秒)) {
            return false;
        }
        *输出 = 响应.快照;
        return 响应.成功 && 输出->后台在线;
    }

    bool 提交人类输入(std::string_view 文本, std::string* 错误 = nullptr, DWORD 超时毫秒 = 1000) const {
        std::string 请求;
        后台前端通道模块_内部::追加字段(请求, "command", "submit_input");
        后台前端通道模块_内部::追加字段(请求, "text", 文本);
        后台前端通道模块_内部::结构_响应 响应{};
        const bool ok = 私有_发送请求(请求, &响应, 超时毫秒) && 响应.成功;
        if (!ok && 错误) *错误 = 响应.消息.empty() ? "后台未响应提交输入请求" : 响应.消息;
        return ok;
    }

    bool 请求后台安全停机(std::string* 错误 = nullptr, DWORD 超时毫秒 = 1000) const {
        后台前端通道模块_内部::结构_响应 响应{};
        const bool ok = 私有_发送请求("command=request_stop\n", &响应, 超时毫秒) && 响应.成功;
        if (!ok && 错误) *错误 = 响应.消息.empty() ? "后台未响应停机请求" : 响应.消息;
        return ok;
    }

    bool 启动相机观测(std::string* 错误 = nullptr, DWORD 超时毫秒 = 1000) const {
        后台前端通道模块_内部::结构_响应 响应{};
        const bool ok = 私有_发送请求("command=camera_start\n", &响应, 超时毫秒) && 响应.成功;
        if (!ok && 错误) *错误 = 响应.消息.empty() ? "后台未响应相机启动请求" : 响应.消息;
        return ok;
    }

    bool 停止相机观测(std::string* 错误 = nullptr, DWORD 超时毫秒 = 1000) const {
        后台前端通道模块_内部::结构_响应 响应{};
        const bool ok = 私有_发送请求("command=camera_stop\n", &响应, 超时毫秒) && 响应.成功;
        if (!ok && 错误) *错误 = 响应.消息.empty() ? "后台未响应相机停止请求" : 响应.消息;
        return ok;
    }

    bool 启动自我场景再现(std::string* 错误 = nullptr, DWORD 超时毫秒 = 1000) const {
        后台前端通道模块_内部::结构_响应 响应{};
        const bool ok = 私有_发送请求("command=scene_replay_start\n", &响应, 超时毫秒) && 响应.成功;
        if (!ok && 错误) *错误 = 响应.消息.empty() ? "后台未响应自我场景再现启动请求" : 响应.消息;
        return ok;
    }

    bool 停止自我场景再现(std::string* 错误 = nullptr, DWORD 超时毫秒 = 1000) const {
        后台前端通道模块_内部::结构_响应 响应{};
        const bool ok = 私有_发送请求("command=scene_replay_stop\n", &响应, 超时毫秒) && 响应.成功;
        if (!ok && 错误) *错误 = 响应.消息.empty() ? "后台未响应自我场景再现停止请求" : 响应.消息;
        return ok;
    }

    bool 查询自我场景快照(结构_后台自我场景快照* 输出, std::string* 错误 = nullptr, DWORD 超时毫秒 = 500) const {
        if (!输出) return false;
        输出->后台在线 = false;

        后台前端通道模块_内部::结构_响应 响应{};
        const bool ok = 私有_发送请求("command=scene_replay_snapshot\n", &响应, 超时毫秒) && 响应.成功;
        if (!ok) {
            if (错误) *错误 = 响应.消息.empty() ? "后台未响应自我场景快照请求" : 响应.消息;
            return false;
        }
        *输出 = std::move(响应.自我场景);
        输出->后台在线 = 响应.快照.后台在线;
        return 输出->后台在线;
    }

    bool 锁定观测目标(std::string_view 存在主键, std::string* 错误 = nullptr, DWORD 超时毫秒 = 1000) const {
        if (存在主键.empty()) {
            if (错误) *错误 = "观测目标主键为空";
            return false;
        }

        std::string 请求;
        后台前端通道模块_内部::追加字段(请求, "command", "scene_observe_lock");
        后台前端通道模块_内部::追加字段(请求, "target_key", 存在主键);
        后台前端通道模块_内部::结构_响应 响应{};
        const bool ok = 私有_发送请求(请求, &响应, 超时毫秒) && 响应.成功;
        if (!ok && 错误) *错误 = 响应.消息.empty() ? "后台未响应观测目标锁定请求" : 响应.消息;
        return ok;
    }

    bool 解除观测锁定(std::string* 错误 = nullptr, DWORD 超时毫秒 = 1000) const {
        后台前端通道模块_内部::结构_响应 响应{};
        const bool ok = 私有_发送请求("command=scene_observe_unlock\n", &响应, 超时毫秒) && 响应.成功;
        if (!ok && 错误) *错误 = 响应.消息.empty() ? "后台未响应观测目标解锁请求" : 响应.消息;
        return ok;
    }

private:
    bool 私有_发送请求(
        std::string_view 请求,
        后台前端通道模块_内部::结构_响应* 输出,
        DWORD 超时毫秒) const {
        if (!输出) return false;
        // 1240/1230：前端查询必须具备明确超时，不能让 UI 或连接线程无限卡在命名管道读写上。
        std::string 响应缓冲(256 * 1024, '\0');
        DWORD 已读 = 0;
        const BOOL 调用成功 = ::CallNamedPipeW(
            HYEgo后台管道名,
            const_cast<char*>(请求.data()),
            static_cast<DWORD>(请求.size()),
            响应缓冲.data(),
            static_cast<DWORD>(响应缓冲.size()),
            &已读,
            超时毫秒);
        if (!调用成功) {
            return false;
        }
        if (已读 == 0) {
            return false;
        }

        响应缓冲.resize(已读);
        return 后台前端通道模块_内部::反序列化响应(响应缓冲, 输出);
    }
};
