export module 外部方法总接口;

// 单文件：外部方法（External Methods）统一模块接口
// 目标：将“单机 + 互联网”可实现的全部外部方法抽象为稳定的接口层，
//      供世界树 / 场景 / 任务-方法系统以依赖注入方式对接。
// 说明：
//  1) 仅定义抽象接口与轻量数据结构；
//  2) 不引入第三方库；
//  3) 提供可选的“控制台默认实现（仅文本I/O）”，便于快速打通流程；
//  4) 与现有模块的对接点：主信息定义模块（节点类型）等。

import <string>;
import <vector>;
import <optional>;
import <cstdint>;
import <memory>;
import <chrono>;

// 若你的工程已存在以下模块，请取消注释并保持模块名一致
 import 主信息定义模块;     // 提供: 基础信息节点类 / 场景节点类 / 语素节点类 / 特征值节点类 等
import 语素模块;
import 基础数据类型模块;

// --------------------------------------------------------------------------------------------
// 基础数据结构
// --------------------------------------------------------------------------------------------
export namespace 外部方法 {

    using 字符串 = std::string;
    using 时间点 = std::chrono::system_clock::time_point;

    // 通用键值对
    export struct 参数KV {
        字符串 键;
        字符串 值;
    };

    // 统一结果包装
    export template <typename T>
        struct 结果 {
        bool 成功 = false;
        T    数据{};                    // 有值时读取
        字符串 错误信息;                 // 失败时可读
        std::vector<参数KV> 元信息;      // 可选的额外信息（耗时、来源等）
    };

    // 文本块
    export struct 文本块 {
        字符串 内容;
        字符串 语言;     // zh-CN / en-US ...
        std::vector<参数KV> 标签; // 例如: {角色: "system"}
    };

    // 二进制块（音频/图像/文件等）
    export struct 二进制块 {
        std::vector<std::byte> 数据;
        字符串 媒体类型;  // image/png, audio/wav, application/octet-stream
        字符串 名称;      // 文件名或资源名
    };

    // 搜索结果/网页抓取
    export struct 搜索条目 {
        字符串 标题;
        字符串 摘要;
        字符串 链接;
        时间点 抓取时间{};
        std::vector<参数KV> 元信息; // 例如引擎、排名
    };

    // 语言学结构：分词 / 情感
    export struct 分词单元 {
        字符串 词;
        字符串 词性;   // 自定义枚举名或原样字符串
    };

    export struct 情感分析结果 {
        double 积极度 = 0.0; // [-1,1] 或 [0,1] 取决于实现
        std::vector<参数KV> 证据; // 触发片段、权重
    };

    // 语音参数
    export struct TTS参数 {
        字符串 发音人;
        double 语速 = 1.0;
        double 音量 = 1.0;
        double 音高 = 1.0;
    };

    // HTTP 基础（可映射到任意网络库）
    export enum class HTTP方法 { GET, POST, PUT, PATCH, DELETE_ };

    export struct HTTP请求 {
        HTTP方法 方法 = HTTP方法::GET;
        字符串   URL;
        std::vector<参数KV> 头;
        std::vector<参数KV> 查询;
        二进制块  正文{};  // 可空
        std::chrono::milliseconds 超时{ 30000 };
    };

    export struct HTTP响应 {
        int 状态码 = 0;
        std::vector<参数KV> 头;
        二进制块 正文{};
        字符串   错误信息; // 网络层错误
        std::chrono::milliseconds 耗时{ 0 };
    };

    // 数据库占位（接口抽象，不绑定具体驱动）
    export struct DB查询结果行 {
        std::vector<参数KV> 列; // 列名-字符串值（简单化）
    };

    export struct DB配置 {
        字符串 连接串;   // sqlite://file.db, mysql://user:pwd@host/db
        字符串 驱动;     // sqlite / mysql / postgres ... 由实现决定
    };

    // 社交/消息渠道
    export struct 渠道消息 {
        字符串 渠道;     // wechat/telegram/email/http-hook
        字符串 目标;     // 群ID/用户ID/地址
        文本块 文本;     // 文本内容
        std::optional<二进制块> 附件; // 可选
    };

    // 视觉层占位（不绑定OpenCV等）
    export struct 图像帧 {
        int 宽 = 0;
        int 高 = 0;
        int 通道 = 0;                 // 1=灰度,3=RGB,4=RGBA
        std::vector<std::byte> 像素;   // 行主序，具体格式由实现说明
        时间点 时间戳{};
        std::vector<参数KV> 元信息;   // 摄像头ID等
    };

    // ----------------------------------------------------------------------------------------
    // 分类接口（抽象层）
    // ----------------------------------------------------------------------------------------

    // 1) 信息获取（搜索 / 抓取 / 通用HTTP）
    export class 信息获取接口 {
    public:
        virtual ~信息获取接口() = default;
        virtual 结果<std::vector<搜索条目>> 搜索(const 字符串& 关键词, int 限制 = 10) = 0;
        virtual 结果<文本块> 抓取网页(const 字符串& URL) = 0;
        virtual 结果<HTTP响应> 调用HTTP(const HTTP请求& 请求) = 0;
    };

    // 2) 语言处理（NLU/NLG/分词/摘要/翻译/情感）
    export class 语言处理接口 {
    public:
        virtual ~语言处理接口() = default;
        virtual 结果<std::vector<分词单元>> 分词(const 字符串& 句子) = 0;
        virtual 结果<文本块> 摘要(const 文本块& 输入, size_t 目标字数) = 0;
        virtual 结果<文本块> 翻译(const 文本块& 输入, const 字符串& 目标语言) = 0;
        virtual 结果<情感分析结果> 情感分析(const 文本块& 输入) = 0;
        virtual 结果<文本块> 生成文本(const 文本块& 提示) = 0; // NLG
    };

    // 3) 感知接口（音频 / 语音 / 视觉）
    export class 感知接口 {
    public:
        virtual ~感知接口() = default;
        // 语音：ASR & TTS
        virtual 结果<文本块> 语音识别(const 二进制块& 音频) = 0; // ASR
        virtual 结果<二进制块> 语音合成(const 文本块& 文本, const TTS参数& 参 = {}) = 0; // TTS
        // 视觉：摄像头帧/文件 -> 特征或识别结果（此处仅抽象图像输入与文本输出）
        virtual 结果<文本块> 图像识别(const 图像帧& 图像) = 0;
    };

    // 4) 推理&模型（分类/匹配/向量等 —— 可映射到外部AI服务）
    export class 推理模型接口 {
    public:
        virtual ~推理模型接口() = default;
        // 通用推理：输入文本/向量，输出文本/向量（这里用文本块占位）
        virtual 结果<文本块> 推理(const 文本块& 输入) = 0;
        // 相似度/匹配等
        virtual 结果<double> 相似度(const 文本块& A, const 文本块& B) = 0;
    };

    // 5) 数据持久化（文件 / 数据库 / 云存储）
    export class 数据存储接口 {
    public:
        virtual ~数据存储接口() = default;
        // 文件
        virtual 结果<bool> 写文件(const 字符串& 路径, const 二进制块& 内容) = 0;
        virtual 结果<二进制块> 读文件(const 字符串& 路径) = 0;
        // 数据库
        virtual 结果<bool> 连接数据库(const DB配置& 配置) = 0;
        virtual 结果<std::vector<DB查询结果行>> 执行查询(const 字符串& SQL) = 0;
    };

    // 6) 控制&行动（系统/进程/外部程序/网络消息等）
    export class 控制行动接口 {
    public:
        virtual ~控制行动接口() = default;
        virtual 结果<int> 执行命令(const 字符串& 命令行) = 0; // 返回进程退出码
        virtual 结果<bool> 打开资源(const 字符串& 路径或URL) = 0;
        virtual 结果<bool> 发送网络消息(const 字符串& 目的, const 文本块& 内容) = 0; // WebSocket/UDP等（抽象）
    };

    // 7) 人机交互（文本/语音/视觉呈现/共情）
    export class 人机交互接口 {
    public:
        virtual ~人机交互接口() = default;
        // 文本对话
        virtual void 输出文本(const 字符串& 文本) = 0;
        virtual 结果<文本块> 等待文本输入(const 字符串& 提示 = "") = 0;
        // 语音
        virtual 结果<bool> 播放语音(const 二进制块& 音频) = 0;
        // 视觉呈现（抽象）
        virtual 结果<bool> 显示图像(const 图像帧& 图像) = 0;
        // 情绪/共情
        virtual void 表达情绪(const 字符串& 情绪标签 /* happy/sad/busy/... */) = 0;
        virtual 结果<文本块> 共情反馈(const 文本块& 用户输入) = 0;
    };

    // 8) 社交与消息渠道（IM/邮件/群聊/订阅）
    export class 社交消息接口 {
    public:
        virtual ~社交消息接口() = default;
        virtual 结果<bool> 发送(const 渠道消息& 消息) = 0;
        virtual 结果<std::vector<渠道消息>> 拉取(const 字符串& 渠道, int 限制 = 20) = 0;
    };
    export   struct 方法回包 {
        场景节点类* 头节点场景 = nullptr;   // 承载 外部方法头节点主信息类
        场景节点类* 结果节点场景 = nullptr; // 承载 方法结果主信息类
        基础信息节点类* 头节点 = nullptr;       // 如需直接引用节点
        基础信息节点类* 结果节点 = nullptr;
        std::vector<参数KV> 元信息;               // 耗时/来源/版本等
    };

    // 用于把“主信息”真正落在世界树/链表上的工厂接口
    export  class 方法节点工厂 {
    public:
        virtual ~方法节点工厂() = default;

        // 创建头节点（返回含头节点场景/节点的回包）
        virtual 结果<方法回包> 创建头节点(
            long long 可信度,
            枚举_基础方法 基础方法类型) = 0;

        // 创建结果节点（可把已有回包补齐成最终“头+结果”）
        virtual 结果<方法回包> 创建结果节点(
            场景节点类* 结果场景,
            const std::vector<存在节点类*>& 受影响存在,
            const std::vector<特征节点类*>& 受影响特征,
            方法回包 已有 = {}) = 0;
    };
    // ----------------------------------------------------------------------------------------
    // 外部方法中心（Facade/Service Locator）
    // 说明：集中持有各分类接口的实例指针，世界树/方法系统仅依赖此门面对象。
    // ----------------------------------------------------------------------------------------
    export class 外部方法中心 {
    public:
        // 统一的回包：打包“头节点场景/结果节点场景”和节点本体指针，便于任务系统传递
      
        // 注入各子系统实现
        void 设置信息获取(std::shared_ptr<信息获取接口> s) { 信息获取 = (s); }
        void 设定语言处理(std::shared_ptr<语言处理接口> s) { 语言处理 = (s); }
        void 设定感知(std::shared_ptr<感知接口> s) { 感知 = (s); }
        void 设定推理模型(std::shared_ptr<推理模型接口> s) { 推理模型 = (s); }
        void 设定数据存储(std::shared_ptr<数据存储接口> s) { 数据存储 = (s); }
        void 设定控制行动(std::shared_ptr<控制行动接口> s) { 控制行动 = (s); }
        void 设定人机交互(std::shared_ptr<人机交互接口> s) { 人机交互 = (s); }
        void 设定社交消息(std::shared_ptr<社交消息接口> s) { 社交消息 = (s); }

        // 便捷转发（可按需精简）
        std::shared_ptr<信息获取接口>   获取信息获取()   const { return 信息获取; }
        std::shared_ptr<语言处理接口>   获取语言处理()   const { return 语言处理; }
        std::shared_ptr<感知接口>       获取感知()       const { return 感知; }
        std::shared_ptr<推理模型接口>   获取推理模型()   const { return 推理模型; }
        std::shared_ptr<数据存储接口>   获取数据存储()   const { return 数据存储; }
        std::shared_ptr<控制行动接口>   获取控制行动()   const { return 控制行动; }
        std::shared_ptr<人机交互接口>   获取人机交互()   const { return 人机交互; }
        std::shared_ptr<社交消息接口>   获取社交消息()   const { return 社交消息; }

        // === public: 新增 ===
        void 设定方法节点工厂(std::shared_ptr<方法节点工厂> f) { 节点工厂 = (f); }
        std::shared_ptr<方法节点工厂> 获取方法节点工厂() const { return 节点工厂; }

        // 便捷封装：任意外部方法在运行前后调用这两个助手即可形成“方法节点”
        结果<方法回包> 生成方法头(long long 可信度, 枚举_基础方法 基础方法类型) {
            if (!节点工厂) return { false, {}, "未设置 方法节点工厂" };
            return 节点工厂->创建头节点(可信度, 基础方法类型);
        }
        结果<方法回包> 生成方法结果(场景节点类* 结果场景,
            const std::vector<存在节点类*>& e,
            const std::vector<特征节点类*>& f,
            方法回包 已有 = {}) {
            if (!节点工厂) return { false, {}, "未设置 方法节点工厂" };
            return 节点工厂->创建结果节点(结果场景, e, f, (已有));
        }

        // === private: 新增 ===
        std::shared_ptr<方法节点工厂> 节点工厂;

    private:
        std::shared_ptr<信息获取接口> 信息获取;
        std::shared_ptr<语言处理接口> 语言处理;
        std::shared_ptr<感知接口>     感知;
        std::shared_ptr<推理模型接口> 推理模型;
        std::shared_ptr<数据存储接口> 数据存储;
        std::shared_ptr<控制行动接口> 控制行动;
        std::shared_ptr<人机交互接口> 人机交互;
        std::shared_ptr<社交消息接口> 社交消息;
    };

    // ----------------------------------------------------------------------------------------
    // 参考：默认控制台实现（仅用于快速联调；生产环境请替换为真实实现）
    // ----------------------------------------------------------------------------------------

    export class 控制台交互 : public 人机交互接口 {
    public:
        void 输出文本(const 字符串& 文本) override;
        结果<文本块> 等待文本输入(const 字符串& 提示) override;
        结果<bool> 播放语音(const 二进制块&) override;
        结果<bool> 显示图像(const 图像帧&) override;
        void 表达情绪(const 字符串& 情绪标签) override;
        结果<文本块> 共情反馈(const 文本块& 用户输入) override;
    };
}

// --------------------------------------------------------------------------------------------
// 内联实现（仅默认控制台实现）
// --------------------------------------------------------------------------------------------
//module; // 私有部分
//import <iostream>;
//
//using namespace std;
//
//export module 外部方法总接口:impl; // 分区接口实现（可选）
//
//using 外部方法::控制台交互;
//using 外部方法::结果;
//using 外部方法::文本块;
//using 外部方法::二进制块;
//using 外部方法::图像帧;
//using 外部方法::字符串;
//
//void 控制台交互::输出文本(const 字符串& 文本) {
//    wcout << 文本 << endl;
//}
//
//结果<文本块> 控制台交互::等待文本输入(const 字符串& 提示) {
//    if (!提示.empty()) wcout << 提示 << endl;
//    std::string 行; std::getline(wcin, 行);
//    return { true, 文本块{.内容 = 行, .语言 = "zh-CN" }, "", {} };
//}
//
//结果<bool> 控制台交互::播放语音(const 二进制块&) {
//    // 控制台环境无法播音，返回未实现
//    return { false, false, "控制台环境未实现TTS播放" };
//}
//
//结果<bool> 控制台交互::显示图像(const 图像帧&) {
//    // 控制台环境无法显示图像，返回未实现
//    return { false, false, "控制台环境未实现图像显示" };
//}
//
//void 控制台交互::表达情绪(const 字符串& 情绪标签) {
//    wcout << "[情绪] " << 情绪标签 << endl;
//}
//
//结果<文本块> 控制台交互::共情反馈(const 文本块& 用户输入) {
//    文本块 out{ "我理解你的感受：" + 用户输入.内容, "zh-CN" };
//    return { true, out };
//}
//