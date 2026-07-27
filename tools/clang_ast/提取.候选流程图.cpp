// 开发工具规则：只读取源文件和 Clang AST，不写入生产代码、规范、计划或正式图谱。
#include <clang-c/Index.h>

#include <cwctype>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace {

struct 位置材料 {
    std::string 文件;
    unsigned 行 = 0;
    unsigned 列 = 0;
    unsigned 偏移 = 0;
};

struct 范围材料 {
    位置材料 开始;
    位置材料 结束;
};

struct 参数记录 {
    std::string 名称;
    std::string 类型;
};

struct 流程节点记录 {
    std::string 编号;
    std::string 父编号;
    unsigned 深度 = 0;
    std::string 游标种类;
    std::string 节点类型;
    std::string 名称;
    范围材料 范围;
};

struct 函数记录 {
    std::string 身份;
    std::string usr;
    std::string 种类;
    std::string 名称;
    std::string 签名;
    std::string 返回类型;
    位置材料 位置;
    范围材料 范围;
    std::vector<参数记录> 参数组;
    std::vector<流程节点记录> 节点组;
};

struct 调用记录 {
    std::string 调用方;
    std::string 调用方名称;
    std::string 被调usr;
    std::string 被调种类;
    std::string 被调名称;
    std::string 被调签名;
    位置材料 位置;
    std::string 解析方式;
};

struct 未解析记录 {
    std::string 调用方;
    std::string 调用方名称;
    std::string 引用种类;
    位置材料 位置;
    std::string 原因;
};

struct 文件结果 {
    std::string 源文件;
    std::string 状态;
    unsigned 解析错误码 = 0;
    std::vector<std::string> 诊断;
    std::map<std::string, 函数记录> 函数组;
    std::vector<调用记录> 调用组;
    std::vector<未解析记录> 未解析组;
    std::set<std::string> 调用键组;
};

struct 遍历上下文 {
    文件结果* 结果 = nullptr;
    std::string 当前函数;
    std::string 当前函数名称;
    std::string 当前父节点;
    unsigned 当前深度 = 0;
    std::filesystem::path 工作目录;
    std::filesystem::path 项目根目录;
    std::filesystem::path 主文件;
    bool 仅主文件 = true;
};

struct 命令行参数 {
    std::vector<std::string> 源文件组;
    std::vector<std::string> 编译参数;
    std::string 编译器 = "clang";
    std::filesystem::path 工作目录 = std::filesystem::current_path();
    std::filesystem::path 项目根目录 = std::filesystem::current_path();
    std::filesystem::path 输出路径;
    std::string 观察提交;
    bool 仅主文件 = true;
    bool 仅显示帮助 = false;
};

std::string 读取字符串(CXString 文本) {
    const char* 指针 = clang_getCString(文本);
    std::string 结果 = 指针 == nullptr ? "" : 指针;
    clang_disposeString(文本);
    return 结果;
}

std::string JSON转义(std::string_view 文本) {
    std::ostringstream 输出;
    for (unsigned char 字符 : 文本) {
        switch (字符) {
        case '\\': 输出 << "\\\\"; break;
        case '"': 输出 << "\\\""; break;
        case '\n': 输出 << "\\n"; break;
        case '\r': 输出 << "\\r"; break;
        case '\t': 输出 << "\\t"; break;
        default:
            if (字符 < 0x20) {
                输出 << "\\u00" << "0123456789abcdef"[(字符 >> 4) & 0x0f]
                    << "0123456789abcdef"[字符 & 0x0f];
            } else {
                输出 << static_cast<char>(字符);
            }
            break;
        }
    }
    return 输出.str();
}

void 写字符串(std::ostream& 输出, std::string_view 文本) {
    输出 << '"' << JSON转义(文本) << '"';
}

std::filesystem::path 规范路径对象(
    const std::string& 原路径,
    const std::filesystem::path& 工作目录) {
    if (原路径.empty()) {
        return {};
    }
    std::filesystem::path 路径 = 原路径;
    if (路径.is_relative()) {
        路径 = 工作目录 / 路径;
    }
    std::error_code 错误;
    auto 规范结果 = std::filesystem::weakly_canonical(路径, 错误);
    return 错误 ? 路径.lexically_normal() : 规范结果;
}

std::string 规范路径(const std::string& 原路径, const std::filesystem::path& 工作目录) {
    return 规范路径对象(原路径, 工作目录).string();
}

位置材料 读取源码位置(CXSourceLocation 源码位置, const std::filesystem::path& 工作目录) {
    CXFile 文件 = nullptr;
    unsigned 行 = 0;
    unsigned 列 = 0;
    unsigned 偏移 = 0;
    clang_getSpellingLocation(源码位置, &文件, &行, &列, &偏移);
    return {
        规范路径(文件 == nullptr ? "" : 读取字符串(clang_getFileName(文件)), 工作目录),
        行,
        列,
        偏移,
    };
}

位置材料 读取位置(CXCursor 游标, const std::filesystem::path& 工作目录) {
    return 读取源码位置(clang_getCursorLocation(游标), 工作目录);
}

范围材料 读取范围(CXCursor 游标, const std::filesystem::path& 工作目录) {
    const auto 范围 = clang_getCursorExtent(游标);
    return {
        读取源码位置(clang_getRangeStart(范围), 工作目录),
        读取源码位置(clang_getRangeEnd(范围), 工作目录),
    };
}

bool 路径相同(
    const std::string& 左路径,
    const std::filesystem::path& 右路径,
    const std::filesystem::path& 工作目录) {
    if (左路径.empty()) {
        return false;
    }
    std::error_code 左错误;
    std::error_code 右错误;
    const auto 左 = std::filesystem::weakly_canonical(
        规范路径对象(左路径, 工作目录), 左错误);
    const auto 右 = std::filesystem::weakly_canonical(右路径, 右错误);
    if (左错误 || 右错误) {
        return false;
    }
#ifdef _WIN32
    auto 左文本 = 左.wstring();
    auto 右文本 = 右.wstring();
    for (auto& 字符 : 左文本) 字符 = static_cast<wchar_t>(::towlower(字符));
    for (auto& 字符 : 右文本) 字符 = static_cast<wchar_t>(::towlower(字符));
    return 左文本 == 右文本;
#else
    return 左 == 右;
#endif
}

bool 属于项目(const std::string& 文件, const std::filesystem::path& 项目根目录) {
    if (文件.empty()) {
        return false;
    }
    std::error_code 文件错误;
    std::error_code 根错误;
    const auto 文件路径 = std::filesystem::weakly_canonical(文件, 文件错误);
    const auto 根路径 = std::filesystem::weakly_canonical(项目根目录, 根错误);
    if (文件错误 || 根错误) {
        return false;
    }
    const auto 相对路径 = 文件路径.lexically_relative(根路径);
    if (相对路径.empty()) {
        return 文件路径 == 根路径;
    }
    const auto 相对文本 = 相对路径.generic_string();
    return 相对文本 != ".."
        && !相对文本.starts_with("../")
        && !相对路径.is_absolute();
}

bool 是函数种类(CXCursorKind 种类) {
    switch (种类) {
    case CXCursor_FunctionDecl:
    case CXCursor_CXXMethod:
    case CXCursor_Constructor:
    case CXCursor_Destructor:
    case CXCursor_ConversionFunction:
    case CXCursor_FunctionTemplate:
        return true;
    default:
        return false;
    }
}

bool 是直接可调用种类(CXCursorKind 种类) {
    return 是函数种类(种类);
}

std::string 节点类型(CXCursorKind 种类) {
    switch (种类) {
    case CXCursor_CallExpr: return "函数调用候选";
    case CXCursor_IfStmt:
    case CXCursor_SwitchStmt:
    case CXCursor_CaseStmt:
    case CXCursor_DefaultStmt:
    case CXCursor_ConditionalOperator:
        return "判断候选";
    case CXCursor_ForStmt:
    case CXCursor_CXXForRangeStmt:
    case CXCursor_WhileStmt:
    case CXCursor_DoStmt:
        return "循环候选";
    case CXCursor_ReturnStmt: return "返回候选";
    case CXCursor_CXXThrowExpr: return "抛出候选";
    case CXCursor_CXXTryStmt:
    case CXCursor_CXXCatchStmt:
        return "异常处理候选";
    case CXCursor_BreakStmt:
    case CXCursor_ContinueStmt:
    case CXCursor_GotoStmt:
        return "循环或跳转控制候选";
    case CXCursor_DeclStmt: return "局部声明候选";
    case CXCursor_BinaryOperator:
    case CXCursor_CompoundAssignOperator:
    case CXCursor_UnaryOperator:
        return "表达式候选";
    case CXCursor_LambdaExpr: return "Lambda候选";
    default:
        return "";
    }
}

std::string 读取游标名称(CXCursor 游标) {
    auto 显示名称 = 读取字符串(clang_getCursorDisplayName(游标));
    if (!显示名称.empty()) {
        return 显示名称;
    }
    return 读取字符串(clang_getCursorSpelling(游标));
}

std::string 读取游标usr(CXCursor 游标) {
    return 读取字符串(clang_getCursorUSR(游标));
}

std::string 读取函数身份(
    const std::string& usr,
    const std::string& 名称,
    const 位置材料& 位置) {
    if (!usr.empty()) {
        return "候选函数:" + usr;
    }
    return "候选函数:" + 位置.文件 + ":" + std::to_string(位置.行) + ":" + 名称;
}

函数记录 形成函数记录(CXCursor 游标, const std::filesystem::path& 工作目录) {
    const auto 位置 = 读取位置(游标, 工作目录);
    const auto usr = 读取游标usr(游标);
    const auto 名称 = 读取游标名称(游标);
    函数记录 结果{
        读取函数身份(usr, 名称, 位置),
        usr,
        读取字符串(clang_getCursorKindSpelling(clang_getCursorKind(游标))),
        名称,
        读取字符串(clang_getTypeSpelling(clang_getCursorType(游标))),
        读取字符串(clang_getTypeSpelling(clang_getCursorResultType(游标))),
        位置,
        读取范围(游标, 工作目录),
        {},
        {},
    };
    const auto 参数数量 = clang_Cursor_getNumArguments(游标);
    if (参数数量 > 0) {
        结果.参数组.reserve(static_cast<std::size_t>(参数数量));
        for (int 索引 = 0; 索引 < 参数数量; ++索引) {
            const auto 参数 = clang_Cursor_getArgument(游标, static_cast<unsigned>(索引));
            结果.参数组.push_back({
                读取字符串(clang_getCursorSpelling(参数)),
                读取字符串(clang_getTypeSpelling(clang_getCursorType(参数))),
            });
        }
    }
    return 结果;
}

std::string 形成节点编号(std::size_t 序号) {
    std::ostringstream 文本;
    文本 << 'N' << std::setw(4) << std::setfill('0') << 序号;
    return 文本.str();
}

std::string 记录流程节点(CXCursor 游标, 遍历上下文& 上下文) {
    auto 函数位置 = 上下文.结果->函数组.find(上下文.当前函数);
    if (函数位置 == 上下文.结果->函数组.end()) {
        return "";
    }
    auto& 节点组 = 函数位置->second.节点组;
    流程节点记录 节点{
        形成节点编号(节点组.size() + 1),
        上下文.当前父节点,
        上下文.当前深度,
        读取字符串(clang_getCursorKindSpelling(clang_getCursorKind(游标))),
        节点类型(clang_getCursorKind(游标)),
        读取游标名称(游标),
        读取范围(游标, 上下文.工作目录),
    };
    节点组.push_back(节点);
    return 节点.编号;
}

void 记录调用(CXCursor 游标, 遍历上下文& 上下文) {
    const auto 位置 = 读取位置(游标, 上下文.工作目录);
    const auto 被调 = clang_getCursorReferenced(游标);
    const auto 被调种类 = clang_getCursorKind(被调);
    const auto 被调名称 = clang_Cursor_isNull(被调) ? "" : 读取游标名称(被调);
    const auto 键 = 上下文.当前函数
        + "|" + 被调名称
        + "|" + 位置.文件
        + "|" + std::to_string(位置.行)
        + "|" + std::to_string(位置.列)
        + "|" + std::to_string(位置.偏移);
    if (!上下文.结果->调用键组.insert(键).second) {
        return;
    }

    if (!clang_Cursor_isNull(被调) && 是直接可调用种类(被调种类)) {
        上下文.结果->调用组.push_back({
            上下文.当前函数,
            上下文.当前函数名称,
            读取游标usr(被调),
            读取字符串(clang_getCursorKindSpelling(被调种类)),
            被调名称,
            读取字符串(clang_getTypeSpelling(clang_getCursorType(被调))),
            位置,
            "direct",
        });
        return;
    }

    上下文.结果->未解析组.push_back({
        上下文.当前函数,
        上下文.当前函数名称,
        clang_Cursor_isNull(被调)
            ? "<null>"
            : 读取字符串(clang_getCursorKindSpelling(被调种类)),
        位置,
        "Clang 未提供可唯一确认的直接函数目标",
    });
}

CXChildVisitResult 访问游标(CXCursor 游标, CXCursor, CXClientData 客户端数据) {
    auto& 上下文 = *static_cast<遍历上下文*>(客户端数据);
    const auto 种类 = clang_getCursorKind(游标);

    if (是函数种类(种类) && clang_isCursorDefinition(游标)) {
        const auto 函数 = 形成函数记录(游标, 上下文.工作目录);
        if (!属于项目(函数.位置.文件, 上下文.项目根目录)) {
            return CXChildVisit_Continue;
        }
        if (上下文.仅主文件
            && !路径相同(函数.位置.文件, 上下文.主文件, 上下文.工作目录)) {
            return CXChildVisit_Continue;
        }
        上下文.结果->函数组.insert_or_assign(函数.身份, 函数);
        遍历上下文 子上下文 = 上下文;
        子上下文.当前函数 = 函数.身份;
        子上下文.当前函数名称 = 函数.名称;
        子上下文.当前父节点.clear();
        子上下文.当前深度 = 0;
        clang_visitChildren(游标, 访问游标, &子上下文);
        return CXChildVisit_Continue;
    }

    if (种类 == CXCursor_CallExpr && !上下文.当前函数.empty()) {
        记录调用(游标, 上下文);
    }

    const auto 类型 = 节点类型(种类);
    if (!上下文.当前函数.empty() && !类型.empty()) {
        const auto 节点编号 = 记录流程节点(游标, 上下文);
        遍历上下文 子上下文 = 上下文;
        子上下文.当前父节点 = 节点编号;
        子上下文.当前深度 = 上下文.当前深度 + 1;
        clang_visitChildren(游标, 访问游标, &子上下文);
        return CXChildVisit_Continue;
    }
    return CXChildVisit_Recurse;
}

void 写位置(std::ostream& 输出, const 位置材料& 位置) {
    输出 << "{\"file\":";
    写字符串(输出, 位置.文件);
    输出 << ",\"line\":" << 位置.行
        << ",\"column\":" << 位置.列
        << ",\"offset\":" << 位置.偏移 << '}';
}

void 写范围(std::ostream& 输出, const 范围材料& 范围) {
    输出 << "{\"start\":";
    写位置(输出, 范围.开始);
    输出 << ",\"end\":";
    写位置(输出, 范围.结束);
    输出 << '}';
}

void 写诊断(std::ostream& 输出, const std::vector<std::string>& 诊断) {
    输出 << '[';
    for (std::size_t 索引 = 0; 索引 < 诊断.size(); ++索引) {
        if (索引 != 0) 输出 << ',';
        写字符串(输出, 诊断[索引]);
    }
    输出 << ']';
}

void 写函数(std::ostream& 输出, const 函数记录& 函数) {
    输出 << "{\"candidate_id\":";
    写字符串(输出, 函数.身份);
    输出 << ",\"usr\":";
    写字符串(输出, 函数.usr);
    输出 << ",\"kind\":";
    写字符串(输出, 函数.种类);
    输出 << ",\"name\":";
    写字符串(输出, 函数.名称);
    输出 << ",\"signature\":";
    写字符串(输出, 函数.签名);
    输出 << ",\"result_type\":";
    写字符串(输出, 函数.返回类型);
    输出 << ",\"location\":";
    写位置(输出, 函数.位置);
    输出 << ",\"extent\":";
    写范围(输出, 函数.范围);
    输出 << ",\"parameters\":[";
    for (std::size_t 索引 = 0; 索引 < 函数.参数组.size(); ++索引) {
        if (索引 != 0) 输出 << ',';
        输出 << "{\"name\":";
        写字符串(输出, 函数.参数组[索引].名称);
        输出 << ",\"type\":";
        写字符串(输出, 函数.参数组[索引].类型);
        输出 << '}';
    }
    输出 << "],\"flow_nodes\":[";
    for (std::size_t 索引 = 0; 索引 < 函数.节点组.size(); ++索引) {
        if (索引 != 0) 输出 << ',';
        const auto& 节点 = 函数.节点组[索引];
        输出 << "{\"id\":";
        写字符串(输出, 节点.编号);
        输出 << ",\"parent_id\":";
        写字符串(输出, 节点.父编号);
        输出 << ",\"depth\":" << 节点.深度;
        输出 << ",\"cursor_kind\":";
        写字符串(输出, 节点.游标种类);
        输出 << ",\"node_type\":";
        写字符串(输出, 节点.节点类型);
        输出 << ",\"name\":";
        写字符串(输出, 节点.名称);
        输出 << ",\"extent\":";
        写范围(输出, 节点.范围);
        输出 << '}';
    }
    输出 << "]}";
}

void 写文件结果(std::ostream& 输出, const 文件结果& 结果) {
    输出 << "{\"source\":";
    写字符串(输出, 结果.源文件);
    输出 << ",\"status\":";
    写字符串(输出, 结果.状态);
    输出 << ",\"parse_error_code\":" << 结果.解析错误码 << ",\"diagnostics\":";
    写诊断(输出, 结果.诊断);

    输出 << ",\"functions\":[";
    bool 首项 = true;
    for (const auto& [身份, 函数] : 结果.函数组) {
        if (!首项) 输出 << ',';
        首项 = false;
        写函数(输出, 函数);
    }
    输出 << ']';

    输出 << ",\"calls\":[";
    for (std::size_t 索引 = 0; 索引 < 结果.调用组.size(); ++索引) {
        if (索引 != 0) 输出 << ',';
        const auto& 调用 = 结果.调用组[索引];
        输出 << "{\"caller\":";
        写字符串(输出, 调用.调用方);
        输出 << ",\"caller_name\":";
        写字符串(输出, 调用.调用方名称);
        输出 << ",\"callee_usr\":";
        写字符串(输出, 调用.被调usr);
        输出 << ",\"callee_kind\":";
        写字符串(输出, 调用.被调种类);
        输出 << ",\"callee_name\":";
        写字符串(输出, 调用.被调名称);
        输出 << ",\"callee_signature\":";
        写字符串(输出, 调用.被调签名);
        输出 << ",\"resolution\":";
        写字符串(输出, 调用.解析方式);
        输出 << ",\"location\":";
        写位置(输出, 调用.位置);
        输出 << '}';
    }
    输出 << ']';

    输出 << ",\"unresolved_calls\":[";
    for (std::size_t 索引 = 0; 索引 < 结果.未解析组.size(); ++索引) {
        if (索引 != 0) 输出 << ',';
        const auto& 未解析 = 结果.未解析组[索引];
        输出 << "{\"caller\":";
        写字符串(输出, 未解析.调用方);
        输出 << ",\"caller_name\":";
        写字符串(输出, 未解析.调用方名称);
        输出 << ",\"referenced_kind\":";
        写字符串(输出, 未解析.引用种类);
        输出 << ",\"reason\":";
        写字符串(输出, 未解析.原因);
        输出 << ",\"location\":";
        写位置(输出, 未解析.位置);
        输出 << '}';
    }
    输出 << "]}";
}

void 写帮助() {
    std::cout
        << "用法: 提取_候选流程图.exe --source 文件 [--source 文件] [选项] -- 编译参数\n"
        << "选项:\n"
        << "  --compiler 路径                   记录所用 clang 路径\n"
        << "  --working-directory 路径          编译工作目录\n"
        << "  --project-root 路径               只保留该目录下的函数定义\n"
        << "  --include-project-dependencies    同时保留主文件之外的项目函数\n"
        << "  --output 路径                     输出 JSON；省略时输出到标准输出\n"
        << "  --observed-commit 文本            记录本次扫描对应的 Git 提交\n";
}

bool 读取参数(int 参数数量, char** 参数组, 命令行参数& 参数) {
    bool 已进入编译参数 = false;
    for (int 索引 = 1; 索引 < 参数数量; ++索引) {
        const std::string 当前 = 参数组[索引];
        if (当前 == "--") {
            已进入编译参数 = true;
            continue;
        }
        if (已进入编译参数) {
            参数.编译参数.push_back(当前);
            continue;
        }
        auto 读取值 = [&](std::string& 目标) {
            if (索引 + 1 >= 参数数量) return false;
            目标 = 参数组[++索引];
            return true;
        };
        if (当前 == "--help" || 当前 == "-h") {
            写帮助();
            参数.仅显示帮助 = true;
            return true;
        }
        if (当前 == "--source") {
            std::string 值;
            if (!读取值(值)) return false;
            参数.源文件组.push_back(值);
        } else if (当前 == "--compiler") {
            if (!读取值(参数.编译器)) return false;
        } else if (当前 == "--working-directory") {
            std::string 值;
            if (!读取值(值)) return false;
            参数.工作目录 = 值;
        } else if (当前 == "--project-root") {
            std::string 值;
            if (!读取值(值)) return false;
            参数.项目根目录 = 值;
        } else if (当前 == "--output") {
            std::string 值;
            if (!读取值(值)) return false;
            参数.输出路径 = 值;
        } else if (当前 == "--observed-commit") {
            if (!读取值(参数.观察提交)) return false;
        } else if (当前 == "--include-project-dependencies") {
            参数.仅主文件 = false;
        } else {
            std::cerr << "未知参数: " << 当前 << '\n';
            return false;
        }
    }
    return !参数.源文件组.empty();
}

文件结果 解析文件(
    CXIndex 索引,
    const std::filesystem::path& 源文件,
    const 命令行参数& 参数) {
    文件结果 结果;
    结果.源文件 = 源文件.string();
    std::vector<const char*> 编译参数;
    编译参数.reserve(参数.编译参数.size());
    for (const auto& 参数文本 : 参数.编译参数) {
        编译参数.push_back(参数文本.c_str());
    }

    CXTranslationUnit 翻译单元 = nullptr;
    const auto 错误码 = clang_parseTranslationUnit2(
        索引,
        源文件.string().c_str(),
        编译参数.data(),
        static_cast<int>(编译参数.size()),
        nullptr,
        0,
        CXTranslationUnit_None,
        &翻译单元);
    结果.解析错误码 = static_cast<unsigned>(错误码);
    if (翻译单元 == nullptr) {
        结果.状态 = "解析失败";
        return 结果;
    }

    const auto 诊断数量 = clang_getNumDiagnostics(翻译单元);
    bool 存在错误 = 错误码 != CXError_Success;
    for (unsigned 诊断索引 = 0; 诊断索引 < 诊断数量; ++诊断索引) {
        CXDiagnostic 诊断 = clang_getDiagnostic(翻译单元, 诊断索引);
        const auto 严重度 = clang_getDiagnosticSeverity(诊断);
        存在错误 = 存在错误 || 严重度 >= CXDiagnostic_Error;
        结果.诊断.push_back(读取字符串(
            clang_formatDiagnostic(诊断, clang_defaultDiagnosticDisplayOptions())));
        clang_disposeDiagnostic(诊断);
    }

    遍历上下文 上下文{
        &结果,
        "",
        "",
        "",
        0,
        参数.工作目录,
        参数.项目根目录,
        源文件,
        参数.仅主文件,
    };
    clang_visitChildren(clang_getTranslationUnitCursor(翻译单元), 访问游标, &上下文);
    结果.状态 = 存在错误 ? "存在诊断" : "成功";
    clang_disposeTranslationUnit(翻译单元);
    return 结果;
}

void 写总结果(std::ostream& 输出, const 命令行参数& 参数, const std::vector<文件结果>& 文件组) {
    输出 << "{\"schema\":\"CLANG-AST-FLOW-CANDIDATE/v0.3\"";
    输出 << ",\"tool\":\"libclang C Index API\"";
    输出 << ",\"observed_commit\":";
    写字符串(输出, 参数.观察提交);
    输出 << ",\"compiler\":";
    写字符串(输出, 参数.编译器);
    输出 << ",\"working_directory\":";
    写字符串(输出, 参数.工作目录.string());
    输出 << ",\"project_root\":";
    写字符串(输出, 参数.项目根目录.string());
    输出 << ",\"main_file_only\":" << (参数.仅主文件 ? "true" : "false");
    输出 << ",\"compile_arguments\":[";
    for (std::size_t 索引 = 0; 索引 < 参数.编译参数.size(); ++索引) {
        if (索引 != 0) 输出 << ',';
        写字符串(输出, 参数.编译参数[索引]);
    }
    输出 << "],\"files\":[";
    for (std::size_t 索引 = 0; 索引 < 文件组.size(); ++索引) {
        if (索引 != 0) 输出 << ',';
        写文件结果(输出, 文件组[索引]);
    }
    输出 << "]}\n";
}

} // namespace

int main(int 参数数量, char** 参数组) {
    命令行参数 参数;
    if (!读取参数(参数数量, 参数组, 参数)) {
        return 2;
    }
    if (参数.仅显示帮助) {
        return 0;
    }
    参数.工作目录 = std::filesystem::weakly_canonical(参数.工作目录);
    参数.项目根目录 = std::filesystem::weakly_canonical(参数.项目根目录);

    CXIndex 索引 = clang_createIndex(0, 0);
    std::vector<文件结果> 文件组;
    for (const auto& 原路径 : 参数.源文件组) {
        auto 源文件 = std::filesystem::path(原路径);
        if (源文件.is_relative()) {
            源文件 = 参数.工作目录 / 源文件;
        }
        文件组.push_back(解析文件(
            索引,
            std::filesystem::weakly_canonical(源文件),
            参数));
    }
    clang_disposeIndex(索引);

    std::ostringstream 文本;
    写总结果(文本, 参数, 文件组);
    if (参数.输出路径.empty()) {
        std::cout << 文本.str();
    } else {
        auto 输出路径 = 参数.输出路径;
        if (输出路径.is_relative()) {
            输出路径 = 参数.工作目录 / 输出路径;
        }
        std::ofstream 输出(输出路径, std::ios::binary);
        if (!输出) {
            std::cerr << "无法写入输出: " << 输出路径.string() << '\n';
            return 3;
        }
        输出 << 文本.str();
    }
    return 0;
}
