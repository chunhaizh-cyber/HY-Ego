module;
#include <afx.h>

export module 度量模板注册表模块;

import 基础数据类型模块;

import <string>;
import <string_view>;
import <vector>;
import <memory>;
import <unordered_map>;
import <shared_mutex>;
import <mutex>;
import <optional>;
import <stdexcept>;
import <cstdint>;

// ============================================================
// 度量模板注册表：
//  - 负责生成/识别 ChainKey（度量签名_链键）
//  - 负责给出“默认概念刻度”(大区间/小区间的初始集合)
//
// 设计精神（与当前项目一致）：
//  1) 二次特征 = 纯结果（标量值）；方向性不在二次特征里
//  2) ChainKey 只描述“度量定义”，不携带阈值/窗口/实例主键
//  3) 模板数量必须有限：
//     - 存在域：固定少量
//     - 特征域：固定模板 + 参数(特征类型主键)
// ============================================================

export class 度量模板注册表类 {
public:
    // ============================================================
    // 0) 枚举：模板族（有限集合）
    // ============================================================
    enum class 枚举_模板族 : std::uint8_t {
        未定义 = 0,
        // ===== 存在域（聚合） =====
        存在_归一化差异度Q = 1,
        存在_总体相似度Q = 2,
        存在_命中特征数 = 3,
        存在_比较特征数 = 4,
        存在_覆盖率Q = 5,
        存在_加权平均差异度Q = 6,
        存在_加权相似度Q = 7,

        // ===== 特征域（明细） =====
        特征_差异度Q = 20,
        特征_权重Q = 21,
        特征_缺失码 = 22,
        特征_贡献度Q = 23
    };

    // ============================================================
    // 1) 概念刻度（schema）定义
    // ============================================================
    struct 结构_刻度片 {
        I64区间 区间{ 0, -1 };
        // 名称为“概念名标签”，上层可映射到 词性节点类*
        std::string 名称{};
        bool 是小区间 = false;
    };

    // ============================================================
    // 2) 参数：特征类模板需要“特征类型主键”
    // ============================================================
    struct 结构_模板参数 {
        // 只允许出现“类型主键/类型签名”，禁止实例主键
        std::string 特征类型主键{};
    };

    // ============================================================
    // 3) 模板定义
    // ============================================================
    class 模板基类 {
    public:
        virtual ~模板基类() = default;

        virtual 枚举_模板族 模板族() const noexcept = 0;

        // 生成 ChainKey：只描述“度量定义”
        virtual std::string 生成链键(const 结构_模板参数& p) const = 0;

        // 默认概念刻度（可为空）
        virtual std::vector<结构_刻度片> 默认刻度(const 结构_模板参数& p) const {
            (void)p;
            return {};
        }

        // 校验参数：避免把实例主键塞进链键
        virtual void 校验参数(const 结构_模板参数& p) const {
            (void)p;
        }

        // 用于由 ChainKey 反推模板族（最小：前缀匹配）
        virtual bool 匹配链键(std::string_view chainKey) const noexcept = 0;
    };

public:
    // ============================================================
    // 4) 单例
    // ============================================================
    static 度量模板注册表类& 实例() {
        static 度量模板注册表类 inst;
        return inst;
    }

    // ============================================================
    // 5) 注册/查找
    // ============================================================
    void 注册(std::unique_ptr<模板基类> t) {
        if (!t) return;
        std::unique_lock lk(mtx_);
        auto k = t->模板族();
        表_.emplace(k, std::move(t));
    }

    const 模板基类* 查找(枚举_模板族 k) const {
        std::shared_lock lk(mtx_);
        auto it = 表_.find(k);
        return it == 表_.end() ? nullptr : it->second.get();
    }

    const 模板基类& 获取或抛(枚举_模板族 k) const {
        auto* p = 查找(k);
        if (!p) throw std::runtime_error("度量模板注册表类：模板未注册");
        return *p;
    }

    // 由 ChainKey 反推模板（用于二次特征链键命中刻度）
    const 模板基类* 由链键查找(std::string_view chainKey) const {
        std::shared_lock lk(mtx_);
        for (const auto& kv : 表_) {
            if (kv.second && kv.second->匹配链键(chainKey)) return kv.second.get();
        }
        return nullptr;
    }

    // ============================================================
    // 6) 常用生成器（上层不用记字符串）
    // ============================================================
    std::string 生成链键_存在(枚举_模板族 k) const {
        结构_模板参数 p{};
        return 获取或抛(k).生成链键(p);
    }

    std::string 生成链键_特征(枚举_模板族 k, std::string_view 特征类型主键) const {
        结构_模板参数 p{};
        p.特征类型主键 = std::string(特征类型主键);
        const auto& t = 获取或抛(k);
        t.校验参数(p);
        return t.生成链键(p);
    }

    std::vector<结构_刻度片> 默认刻度_由链键(std::string_view chainKey, std::string_view 特征类型主键 = {}) const {
        const auto* t = 由链键查找(chainKey);
        if (!t) return {};
        结构_模板参数 p{};
        if (!特征类型主键.empty()) p.特征类型主键 = std::string(特征类型主键);
        return t->默认刻度(p);
    }

    // ============================================================
    // 7) 一次性注册默认模板（建议在世界树初始化时调用）
    // ============================================================
    void 注册默认模板() {
        // 存在域
        注册(std::make_unique<模板_存在_归一化差异度Q>());
        注册(std::make_unique<模板_存在_总体相似度Q>());
        注册(std::make_unique<模板_存在_命中特征数>());
        注册(std::make_unique<模板_存在_比较特征数>());
        注册(std::make_unique<模板_存在_覆盖率Q>());
        注册(std::make_unique<模板_存在_加权平均差异度Q>());
        注册(std::make_unique<模板_存在_加权相似度Q>());

        // 特征域
        注册(std::make_unique<模板_特征_差异度Q>());
        注册(std::make_unique<模板_特征_权重Q>());
        注册(std::make_unique<模板_特征_缺失码>());
        注册(std::make_unique<模板_特征_贡献度Q>());
    }

private:
    度量模板注册表类() = default;

    // ============================================================
    // 8) 内置模板实现（有限集合）
    // ============================================================

    // ---------- 存在域：归一化差异度Q（0..10000，越小越像） ----------
    class 模板_存在_归一化差异度Q : public 模板基类 {
    public:
        枚举_模板族 模板族() const noexcept override { return 枚举_模板族::存在_归一化差异度Q; }
        std::string 生成链键(const 结构_模板参数&) const override { return "存在|归一化差异度Q"; }
        bool 匹配链键(std::string_view k) const noexcept override { return k == "存在|归一化差异度Q"; }
        std::vector<结构_刻度片> 默认刻度(const 结构_模板参数&) const override {
            return {
                { I64区间{0, 3333}, "小差异", false },
                { I64区间{3334, 6666}, "中差异", false },
                { I64区间{6667, 10000}, "大差异", false }
            };
        }
    };

    // ---------- 存在域：总体相似度Q（0..10000，越大越像） ----------
    class 模板_存在_总体相似度Q : public 模板基类 {
    public:
        枚举_模板族 模板族() const noexcept override { return 枚举_模板族::存在_总体相似度Q; }
        std::string 生成链键(const 结构_模板参数&) const override { return "存在|总体相似度Q"; }
        bool 匹配链键(std::string_view k) const noexcept override { return k == "存在|总体相似度Q"; }
        std::vector<结构_刻度片> 默认刻度(const 结构_模板参数&) const override {
            return {
                { I64区间{0, 3333}, "低相似", false },
                { I64区间{3334, 6666}, "中相似", false },
                { I64区间{6667, 10000}, "高相似", false }
            };
        }
    };

    class 模板_存在_命中特征数 : public 模板基类 {
    public:
        枚举_模板族 模板族() const noexcept override { return 枚举_模板族::存在_命中特征数; }
        std::string 生成链键(const 结构_模板参数&) const override { return "存在|命中特征数"; }
        bool 匹配链键(std::string_view k) const noexcept override { return k == "存在|命中特征数"; }
    };

    class 模板_存在_比较特征数 : public 模板基类 {
    public:
        枚举_模板族 模板族() const noexcept override { return 枚举_模板族::存在_比较特征数; }
        std::string 生成链键(const 结构_模板参数&) const override { return "存在|比较特征数"; }
        bool 匹配链键(std::string_view k) const noexcept override { return k == "存在|比较特征数"; }
    };

    class 模板_存在_覆盖率Q : public 模板基类 {
    public:
        枚举_模板族 模板族() const noexcept override { return 枚举_模板族::存在_覆盖率Q; }
        std::string 生成链键(const 结构_模板参数&) const override { return "存在|覆盖率Q"; }
        bool 匹配链键(std::string_view k) const noexcept override { return k == "存在|覆盖率Q"; }
        std::vector<结构_刻度片> 默认刻度(const 结构_模板参数&) const override {
            return {
                { I64区间{0, 3333}, "覆盖低", false },
                { I64区间{3334, 6666}, "覆盖中", false },
                { I64区间{6667, 10000}, "覆盖高", false }
            };
        }
    };

    class 模板_存在_加权平均差异度Q : public 模板基类 {
    public:
        枚举_模板族 模板族() const noexcept override { return 枚举_模板族::存在_加权平均差异度Q; }
        std::string 生成链键(const 结构_模板参数&) const override { return "存在|加权平均差异度Q"; }
        bool 匹配链键(std::string_view k) const noexcept override { return k == "存在|加权平均差异度Q"; }
    };

    class 模板_存在_加权相似度Q : public 模板基类 {
    public:
        枚举_模板族 模板族() const noexcept override { return 枚举_模板族::存在_加权相似度Q; }
        std::string 生成链键(const 结构_模板参数&) const override { return "存在|加权相似度Q"; }
        bool 匹配链键(std::string_view k) const noexcept override { return k == "存在|加权相似度Q"; }
        std::vector<结构_刻度片> 默认刻度(const 结构_模板参数&) const override {
            return {
                { I64区间{0, 3333}, "低相似", false },
                { I64区间{3334, 6666}, "中相似", false },
                { I64区间{6667, 10000}, "高相似", false }
            };
        }
    };

    // ---------- 特征域：差异度Q（0..10000，越小越像） ----------
    class 模板_特征_差异度Q : public 模板基类 {
    public:
        枚举_模板族 模板族() const noexcept override { return 枚举_模板族::特征_差异度Q; }

        void 校验参数(const 结构_模板参数& p) const override {
            if (p.特征类型主键.empty()) throw std::invalid_argument("模板_特征_差异度Q：特征类型主键为空");
        }

        std::string 生成链键(const 结构_模板参数& p) const override {
            return std::string("特征|差异度Q|T=") + p.特征类型主键;
        }

        bool 匹配链键(std::string_view k) const noexcept override {
            return k.rfind("特征|差异度Q|T=", 0) == 0;
        }

        std::vector<结构_刻度片> 默认刻度(const 结构_模板参数&) const override {
            return {
                { I64区间{0, 3333}, "小差异", false },
                { I64区间{3334, 6666}, "中差异", false },
                { I64区间{6667, 10000}, "大差异", false }
            };
        }
    };

    // ---------- 特征域：权重Q（0..10000） ----------
    class 模板_特征_权重Q : public 模板基类 {
    public:
        枚举_模板族 模板族() const noexcept override { return 枚举_模板族::特征_权重Q; }
        void 校验参数(const 结构_模板参数& p) const override {
            if (p.特征类型主键.empty()) throw std::invalid_argument("模板_特征_权重Q：特征类型主键为空");
        }
        std::string 生成链键(const 结构_模板参数& p) const override {
            return std::string("特征|权重Q|T=") + p.特征类型主键;
        }
        bool 匹配链键(std::string_view k) const noexcept override {
            return k.rfind("特征|权重Q|T=", 0) == 0;
        }
    };

    // ---------- 特征域：缺失码（0=都有，1=缺A，2=缺B，3=都缺） ----------
    class 模板_特征_缺失码 : public 模板基类 {
    public:
        枚举_模板族 模板族() const noexcept override { return 枚举_模板族::特征_缺失码; }
        void 校验参数(const 结构_模板参数& p) const override {
            if (p.特征类型主键.empty()) throw std::invalid_argument("模板_特征_缺失码：特征类型主键为空");
        }
        std::string 生成链键(const 结构_模板参数& p) const override {
            return std::string("特征|缺失码|T=") + p.特征类型主键;
        }
        bool 匹配链键(std::string_view k) const noexcept override {
            return k.rfind("特征|缺失码|T=", 0) == 0;
        }
        std::vector<结构_刻度片> 默认刻度(const 结构_模板参数&) const override {
            return {
                { I64区间{0, 0}, "齐全", false },
                { I64区间{1, 1}, "缺A", false },
                { I64区间{2, 2}, "缺B", false },
                { I64区间{3, 3}, "都缺", false }
            };
        }
    };

    // ---------- 特征域：贡献度Q（差异度Q * 权重Q / 10000） ----------
    class 模板_特征_贡献度Q : public 模板基类 {
    public:
        枚举_模板族 模板族() const noexcept override { return 枚举_模板族::特征_贡献度Q; }
        void 校验参数(const 结构_模板参数& p) const override {
            if (p.特征类型主键.empty()) throw std::invalid_argument("模板_特征_贡献度Q：特征类型主键为空");
        }
        std::string 生成链键(const 结构_模板参数& p) const override {
            return std::string("特征|贡献度Q|T=") + p.特征类型主键;
        }
        bool 匹配链键(std::string_view k) const noexcept override {
            return k.rfind("特征|贡献度Q|T=", 0) == 0;
        }
    };

private:
    mutable std::shared_mutex mtx_{};
    std::unordered_map<枚举_模板族, std::unique_ptr<模板基类>> 表_{};
};