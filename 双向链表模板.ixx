export module 模板模块;

import 基础数据类型模块;

import <string>;
import <vector>;
import <mutex>;
import <cassert>;
import <type_traits>;
import <stdexcept>;

export template <typename 主信息类型>
class 链表模板 {
private:
    static constexpr char 字符集[62] = {
        L'0',L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9',
        L'A',L'B',L'C',L'D',L'E',L'F',L'G',L'H',L'I',L'J',
        L'K',L'L',L'M',L'N',L'O',L'P',L'Q',L'R',L'S',L'T',
        L'U',L'V',L'W',L'X',L'Y',L'Z',
        L'a',L'b',L'c',L'd',L'e',L'f',L'g',L'h',L'i',L'j',
        L'k',L'l',L'm',L'n',L'o',L'p',L'q',L'r',L's',L't',
        L'u',L'v',L'w',L'x',L'y',L'z'
    };



public:
    class 节点类 {
    public:
        节点类()
            : 上(nullptr), 下(nullptr), 父(nullptr), 子(nullptr),
            根(nullptr), 链上(nullptr), 链下(nullptr),
            主键("0"), 子节点数量(0) {
        }

        virtual ~节点类() = default;

        std::string 主键;
        主信息类型  主信息;

        // 同层环
        节点类* 上;
        节点类* 下;

        // 全局“链”环
        节点类* 链上;
        节点类* 链下;

        // 树关系
        节点类* 子;
        节点类* 父;
        节点类* 根;

        // 计数器
        int64_t 获取节点被引用次数() const { return 节点被引用次数; }
        int64_t 变更节点被引用次数(int64_t 变更数量) {
            节点被引用次数 += 变更数量;
            return 节点被引用次数;
        }
        int64_t 获取子节点数量() const { return 子节点数量; }
        int64_t 变更子节点数量(int64_t 变更数量) {
            子节点数量 += 变更数量;
            return 子节点数量;
        }
    private:
        int64_t 子节点数量;
        int64_t 节点被引用次数 = 0;
    };

public:
    节点类* 根指针;
    mutable std::mutex 链表锁;

    链表模板() {
        根指针 = new 节点类();
        根指针->上 = 根指针->下 = 根指针->链上 = 根指针->链下 = 根指针->根 = 根指针;
        根指针->主键 = "0";
    }
    // 在 链表模板 类里加这两行（只允许移动，不允许拷贝）
    链表模板(链表模板&&) noexcept = default;
    链表模板& operator=(链表模板&&) noexcept = default;

    链表模板(const 链表模板&) = delete;              // 仍然禁止拷贝
    链表模板& operator=(const 链表模板&) = delete;    // 仍然禁止拷贝
    ~链表模板() {
        删除链表();
    }

private:
    static int 字符转值(char ch) {
        for (int i = 0; i < 62; ++i) {
            if (字符集[i] == ch) return i;
        }
        assert(false && "无效字符");
        return 0;
    }

    static std::string 获取新主键(std::string& 当前主键) {
        if (当前主键.empty()) return "0";
        std::string 结果 = 当前主键;
        int 位置 = static_cast<int>(结果.size()) - 1;

        while (位置 >= 0) {
            int 值 = 字符转值(结果[位置]);
            if (值 < 61) { 结果[位置] = 字符集[值 + 1]; return 结果; }
            结果[位置] = 字符集[0];
            --位置;
        }
        // 溢出，前面补 '0'
        return std::string(1, 字符集[0]) + 结果;
    }

    void 更新链指针(节点类* 新节点) {
        新节点->链下 = 根指针;
        新节点->链上 = 根指针->链上;
        根指针->链上->链下 = 新节点;
        根指针->链上 = 新节点;
    }

public:
    // —— 只读计数 —— 
    int64_t 获取子节点数量(节点类* 节点_) const {
        assert(节点_);
        return 节点_->获取子节点数量();
    }

    // —— 插入（位置 + 新节点） —— 
    节点类* 添加节点(节点类* 位置节点, 节点类* 新节点) {
        if (!新节点 || !位置节点) return nullptr;
        std::lock_guard<std::mutex> lock(链表锁);

        根指针->主键 = 新节点->主键 = 获取新主键(根指针->主键);
        新节点->上 = 位置节点->上;
        新节点->下 = 位置节点;
        新节点->根 = 位置节点->根;
        新节点->父 = 位置节点->父;

        位置节点->上->下 = 新节点;
        位置节点->上 = 新节点;

        if (新节点->父) 新节点->父->变更子节点数量(1);
        更新链指针(新节点);
        return 新节点;
    }

    // —— 插入（只给新节点） —— 
//    节点类* 添加节点(节点类* 新节点) {
//        if (!新节点) return nullptr;
//        return 添加节点(根指针, 新节点);
//    }


    节点类* 添加节点(节点类* 位置节点, 主信息类型 主信息变量) {
        if (!位置节点) 位置节点 = 根指针;
        节点类* 新节点 = new 节点类();
        新节点->主信息 = 主信息变量;
        return 添加节点(位置节点, 新节点);
    }
    // —— 插入（给载荷） —— 
    节点类* 添加节点(主信息类型 主信息变量) {
        return 添加节点(根指针, 主信息变量);
    }
  
    // —— 添加子节点 —— 
    节点类* 添加子节点(节点类* 父节点, 节点类* 子节点) {
        assert(子节点);
        if (父节点 == nullptr) return 添加节点(根指针, 子节点);

        if (!父节点->子) {
            std::lock_guard<std::mutex> lock(链表锁);
            父节点->子 = 子节点;
            子节点->主键 = 根指针->主键 = 获取新主键(根指针->主键);
            子节点->上 = 子节点->下 = 子节点->根 = 子节点;
            子节点->父 = 父节点;
            更新链指针(子节点);
            父节点->变更子节点数量(1);
            return 子节点;
        }
        // 子环尾插
        return 添加节点(父节点->子, 子节点);
    }

    节点类* 添加子节点(节点类* 父节点, 主信息类型 主信息变量) {
        节点类* 新子节点 = new 节点类();
        新子节点->主信息 = 主信息变量;
        return 添加子节点(父节点, 新子节点);
    }

    // —— 删除节点（含从全链摘除，必要时递归删子链） —— 
    void 删除节点(节点类* 被删除节点) {
        if (!被删除节点 || 被删除节点 == 根指针) return;
        std::lock_guard<std::mutex> lock(链表锁);

        if (被删除节点->子) 删除子链(被删除节点);

        被删除节点->上->下 = 被删除节点->下;
        被删除节点->下->上 = 被删除节点->上;

        被删除节点->链上->链下 = 被删除节点->链下;
        被删除节点->链下->链上 = 被删除节点->链上;

        if (被删除节点->父) 被删除节点->父->变更子节点数量(-1);

        // 仅当主信息为“指针类型”时才 delete
        if constexpr (std::is_pointer_v<主信息类型>) {
            delete 被删除节点->主信息;
            被删除节点->主信息 = nullptr;
        }
        delete 被删除节点;
    }

    void 删除子链(节点类* 父节点) {
        if (!父节点 || !父节点->子) return;

        节点类* 当前 = 父节点->子;
        if (当前 == 当前->下) {
            删除节点(当前);
            父节点->子 = nullptr;
            return;
        }
        节点类* 下一个;
        do {
            下一个 = 当前->下;
            删除节点(当前);
            当前 = 下一个;
        } while (当前 != 父节点->子);

        父节点->子 = nullptr;
    }

    void 删除链表() {
        if (!根指针) return;
        节点类* 当前 = 根指针->下;
        while (当前 != 根指针) {
            节点类* 临时 = 当前;
            当前 = 当前->下;
            删除节点(临时);
        }
        delete 根指针;
        根指针 = nullptr;
    }

    // —— 修改节点 —— 
    void 修改节点信息(std::string& 主键, 主信息类型& 新主信息) {
        std::lock_guard<std::mutex> lock(链表锁);
        节点类* 节点 = 查找主键(主键);
        if (!节点) return;

        if constexpr (std::is_pointer_v<主信息类型>) {
            delete 节点->主信息;
            // 对指针载荷，假设传入的是“新对象指针”
            节点->主信息 = 新主信息;
        }
        else {
            节点->主信息 = 新主信息;
        }
    }

    // —— 通过主键查找（全链） —— 
    节点类* 查找主键(const std::string& 主键) const {
        节点类* 游标 = 根指针->链下;
        while (游标 != 根指针) {
            if (游标->主键 == 主键) return 游标;
            游标 = 游标->链下;
        }
        return nullptr;
    }

    // ========================= 关键改造：查找（可注入比较器） =========================

    // 当前链查找：从“当前节点”开始，向后扫描，遇到哨兵/回到起点即止。
    // 比较器签名：bool(const 被比较对象&, const 查找对象&, 枚举_比较字段)

    节点类* 查找节点_当前链(节点类* 当前节点, const 主信息类型& 查找内容, 枚举_比较字段 字段选择, 枚举_比较条件 条件)
    {
        if (!当前节点) return nullptr;

        if (当前节点 == 根指针) {
            当前节点 = 当前节点->下;
            if (当前节点 == 根指针) return nullptr;
        }

        节点类* 起点 = 当前节点;
        do {
            if (dynamic_cast<主信息类型>(当前节点->主信息))
            {
                if (当前节点->主信息->比较(查找内容, 字段选择, 条件))
                    return 当前节点;
            }
            else {
                throw std::invalid_argument("逻辑错误,查找节点_当前链,主信息类型不符 ");
            }
            if (!当前节点) return nullptr;         // 保护：破链
            if (当前节点 == 根指针) return nullptr; // 到达哨兵，结束
        } while (当前节点 != 起点);

        return nullptr;
    }
};
  
 