export module 标准特征工具模块;

import 模板模块;

import 主信息定义模块;
import 特征值模块;
import 语素模块;
import 概念树模块;
import 宇宙链模块;
import <string>;
import <vector>;

export namespace 标准特征工具 {

    using 节点类 = 链表模板<基础信息基类*>::节点类;

    // ======== 内部小工具：统一创建「名称词性」和「类型词性」 ========

    inline 词性节点类* 获取或创建特征名(const std::string& 名称) {
        // 语素集内部负责同名去重
        return 语素集.添加词性词(名称, "特征名");
    }

    inline 词性节点类* 获取或创建特征类型(const std::string& 类型名) {
        return 语素集.添加词性词(类型名, "特征类型");
    }

    // ======== 通用：创建特征主信息 + 挂到宇宙链表 ========

    inline 节点类* 添加特征节点(
        节点类* 所属节点,
        词性节点类* 名称词性,
        词性节点类* 类型词性,
        特征值节点类* 值对象
    ) {
        if (!所属节点 || !名称词性 || !类型词性 || !值对象) {
            return nullptr;
        }

        auto* 主信息 = new 特征节点主信息类(名称词性, 类型词性, 值对象);       
        return 宇宙链.添加子节点(所属节点, 主信息);
    }

    // ======== 1. 通用「写入」接口：标量 / 向量 / 字符串 ========

    // 1.1 标量整数特征（可选单位）
    inline 节点类* 添加标量特征(
        节点类* 所属节点,
        const std::string& 特征名,
        const std::string& 特征类型名,
        std::int64_t 数值,
        词性节点类* 单位 = nullptr   // 可以为 nullptr 表示“无单位 / 隐含单位”
    ) {
        auto* 名 = 获取或创建特征名(特征名);
        auto* 型 = 获取或创建特征类型(特征类型名);
        auto* 特征值节点=特征值集.获取或创建标量特征值(单位, 数值);

        return 添加特征节点(所属节点, 名, 型, 特征值节点);
    }

    // 1.2 整数向量特征
    inline 节点类* 添加向量特征(
        节点类* 所属节点,
        const std::string& 特征名,
        const std::string& 特征类型名,
        const std::vector<std::int64_t>& 数据
    ) {
        auto* 名 = 获取或创建特征名(特征名);
        auto* 型 = 获取或创建特征类型(特征类型名);
        auto* 值 = 特征值集.获取或创建矢量特征值(数据);
        return 添加特征节点(所属节点, 名, 型, 值);
    }

    // 1.3 字符串特征
    inline 节点类* 添加字符串特征(
        节点类* 所属节点,
        const std::string& 特征名,
        const std::string& 特征类型名,
        const std::string& 文本值
    ) {
        auto* 名 = 获取或创建特征名(特征名);
        auto* 型 = 获取或创建特征类型(特征类型名);

        auto* 值 = 特征值集.获取或创建字符串特征值(文本值);
        return 添加特征节点(所属节点, 名, 型, 值);
    }

    // ======== 2. 通用「查询特征节点」接口 ========

    struct 特征查询条件 {
        // 为空表示“不限制”
        std::string 名称;        // 精确匹配
        std::string 类型前缀;    // 以某个前缀开头，比如 "空间_" / "存在引用"
    };

    inline bool 匹配特征(const 特征节点主信息类* 主, const 特征查询条件& 条件) {
        if (!主) return false;

        // 名称条件
        if (!条件.名称.empty()) {
            if (!主->名称) return false;
            std::string 名 = 概念树类::取词面(主->名称);
            if (名 != 条件.名称) return false;
        }

        // 类型前缀条件
        if (!条件.类型前缀.empty()) {
            if (!主->类型) return false;
            std::string 型 = 概念树类::取词面(主->类型);
            if (型.rfind(条件.类型前缀, 0) != 0) return false; // 必须前缀匹配
        }

        return true;
    }

    // 查找第一个匹配的特征节点
    inline 节点类* 查找第一个特征节点(
        节点类* 所属节点,
        const 特征查询条件& 条件
    ) {
        if (!所属节点 || !所属节点->子) return nullptr;

        节点类* 起 = 所属节点->子;
        节点类* 游 = 起;

        do {
            auto* 主 = dynamic_cast<特征节点主信息类*>(游->主信息);
            if (主 && 匹配特征(主, 条件)) {
                return 游;
            }
            游 = 游->下;
        } while (游 && 游 != 起);

        return nullptr;
    }

    // 可选：查找所有匹配的特征节点
    inline std::vector<节点类*> 查找所有特征节点(
        节点类* 所属节点,
        const 特征查询条件& 条件
    ) {
        std::vector<节点类*> 结果;
        if (!所属节点 || !所属节点->子) return 结果;

        节点类* 起 = 所属节点->子;
        节点类* 游 = 起;

        do {
            auto* 主 = dynamic_cast<特征节点主信息类*>(游->主信息);
            if (主 && 匹配特征(主, 条件)) {
                结果.push_back(游);
            }
            游 = 游->下;
        } while (游 && 游 != 起);

        return 结果;
    }

    // ======== 3. 通用「读取特征值」接口：标量 / 向量 / 字符串 ========

    inline bool 读取标量特征(
        节点类* 所属节点,
        const std::string& 特征名,
        const std::string& 类型前缀,
        std::int64_t& 输出值,
        词性节点类** 输出单位 = nullptr   // 可选
    ) {
        特征查询条件 条件{ 特征名, 类型前缀 };
        节点类* 特征节点 = 查找第一个特征节点(所属节点, 条件);
        if (!特征节点) return false;

        auto* 主 = dynamic_cast<特征节点主信息类*>(特征节点->主信息);
        if (!主) return false;

        auto* 值 = dynamic_cast<标量特征值主信息类*>(主->值);
        if (!值) return false;

        输出值 = 值->值;
        if (输出单位) {
            *输出单位 = 值->单位;
        }
        return true;
    }

    inline bool 读取向量特征(
        节点类* 所属节点,
        const std::string& 特征名,
        const std::string& 类型前缀,
        std::vector<std::int64_t>& 输出值
    ) {
        特征查询条件 条件{ 特征名, 类型前缀 };
        节点类* 特征节点 = 查找第一个特征节点(所属节点, 条件);
        if (!特征节点) return false;

        auto* 主 = dynamic_cast<特征节点主信息类*>(特征节点->主信息);
        if (!主) return false;

        auto* 值 = dynamic_cast<矢量特征值主信息类*>(主->值);
        if (!值) return false;

        输出值 = 值->值;
        return true;
    }

    inline bool 读取字符串特征(
        节点类* 所属节点,
        const std::string& 特征名,
        const std::string& 类型前缀,
        std::string& 输出文本
    ) {
        特征查询条件 条件{ 特征名, 类型前缀 };
        节点类* 特征节点 = 查找第一个特征节点(所属节点, 条件);
        if (!特征节点) return false;

        auto* 主 = dynamic_cast<特征节点主信息类*>(特征节点->主信息);
        if (!主) return false;

        auto* 值 = dynamic_cast<字符特征值主信息类*>(主->值);
        if (!值) return false;

        输出文本 = 值->值;
        return true;
    }

    // ======== 4. 通用：「存在引用特征」也是通用，不写死关系词 ========

    // 写：from 上挂一个「存在引用」特征，值为 to 的主键字符串
    inline 节点类* 设置存在引用特征(
        节点类* from,
        节点类* to,
        const std::string& 特征名,         // 如 "母亲" / "主人" / "容器"
        const std::string& 特征类型名 = "存在引用"  // 更细可用 "存在引用_母亲" 之类
    ) {
        if (!from || !to) return nullptr;

        const std::string& 目标主键 = to->主键;
        return 添加字符串特征(from, 特征名, 特征类型名, 目标主键);
    }

    // 读：从 from 上解析一个存在引用特征，返回被引用节点（通过主键查找）
    inline 节点类* 读取存在引用特征(
        节点类* from,
        const std::string& 特征名,
        const std::string& 类型前缀 = "存在引用"
    ) {
        if (!from) return nullptr;

        std::string 主键;
        if (!读取字符串特征(from, 特征名, 类型前缀, 主键)) {
            return nullptr;
        }

        return 宇宙链.查找主键(主键);
    }

} // namespace 标准特征工具
