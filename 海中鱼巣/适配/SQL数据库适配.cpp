// 文件规则：数据库失败只形成局部适配结果；不得回滚、修复或覆盖运行期内存仓库。
#include "SQL数据库适配.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>

#include <array>
#include <cstdlib>
#include <sstream>
#include <string_view>
#include <utility>

#pragma comment(lib, "odbc32.lib")

namespace 海中鱼巣 {

namespace {

constexpr std::size_t 最大配置字段长度 = 128;

bool ODBC成功(SQLRETURN 结果) {
    return SQL_SUCCEEDED(结果) != 0;
}

std::wstring 读取ODBC诊断(SQLSMALLINT 句柄类型, SQLHANDLE 句柄) {
    std::wostringstream 文本;
    SQLSMALLINT 记录编号 = 1;
    while (true) {
        std::array<SQLWCHAR, 6> 状态{};
        SQLINTEGER 本机错误 = 0;
        std::array<SQLWCHAR, 1024> 消息{};
        SQLSMALLINT 消息长度 = 0;
        const auto 结果 = SQLGetDiagRecW(
            句柄类型,
            句柄,
            记录编号,
            状态.data(),
            &本机错误,
            消息.data(),
            static_cast<SQLSMALLINT>(消息.size()),
            &消息长度);
        if (结果 == SQL_NO_DATA) {
            break;
        }
        if (!ODBC成功(结果)) {
            break;
        }
        if (记录编号 > 1) {
            文本 << L" | ";
        }
        文本 << reinterpret_cast<const wchar_t*>(状态.data())
            << L"/" << 本机错误 << L": "
            << reinterpret_cast<const wchar_t*>(消息.data());
        ++记录编号;
    }
    return 文本.str();
}

bool 配置字段可用(std::wstring_view 字段) {
    if (字段.empty() || 字段.size() > 最大配置字段长度) {
        return false;
    }
    for (const wchar_t 字符 : 字段) {
        if (字符 == L';' || 字符 == L'{' || 字符 == L'}' || 字符 == L'\r' || 字符 == L'\n') {
            return false;
        }
    }
    return true;
}

std::wstring 读取环境字段(const wchar_t* 名称) {
    wchar_t* 缓冲 = nullptr;
    std::size_t 长度 = 0;
    if (_wdupenv_s(&缓冲, &长度, 名称) != 0 || 缓冲 == nullptr) {
        return {};
    }
    std::wstring 结果(缓冲);
    std::free(缓冲);
    return 结果;
}

std::wstring 转义SQL字符串(std::wstring_view 文本) {
    std::wstring 结果;
    结果.reserve(文本.size());
    for (const wchar_t 字符 : 文本) {
        结果.push_back(字符);
        if (字符 == L'\'') {
            结果.push_back(L'\'');
        }
    }
    return 结果;
}

std::wstring 转义SQL标识符(std::wstring_view 文本) {
    std::wstring 结果;
    结果.reserve(文本.size());
    for (const wchar_t 字符 : 文本) {
        结果.push_back(字符);
        if (字符 == L']') {
            结果.push_back(L']');
        }
    }
    return 结果;
}

数据库操作结果 逻辑内失败(std::wstring 阶段, std::wstring 诊断) {
    return {false, true, false, std::move(阶段), std::move(诊断)};
}

数据库操作结果 追根因失败(std::wstring 阶段, std::wstring 诊断) {
    return {false, false, true, std::move(阶段), std::move(诊断)};
}

class ODBC连接 {
public:
    ODBC连接() = default;

    ~ODBC连接() {
        关闭();
    }

    ODBC连接(const ODBC连接&) = delete;
    ODBC连接& operator=(const ODBC连接&) = delete;

    bool 打开(const SQL数据库配置& 配置, std::wstring_view 数据库, std::wstring& 诊断) {
        关闭();
        if (!ODBC成功(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &环境_))) {
            诊断 = L"无法分配 ODBC 环境句柄。";
            return false;
        }
        if (!ODBC成功(SQLSetEnvAttr(环境_, SQL_ATTR_ODBC_VERSION,
                reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0))) {
            诊断 = 读取ODBC诊断(SQL_HANDLE_ENV, 环境_);
            return false;
        }

        const std::array<std::wstring_view, 2> 驱动组{
            L"ODBC Driver 18 for SQL Server",
            L"ODBC Driver 17 for SQL Server"
        };
        std::wstring 合并诊断;
        for (const auto 驱动 : 驱动组) {
            if (!ODBC成功(SQLAllocHandle(SQL_HANDLE_DBC, 环境_, &连接_))) {
                合并诊断 = L"无法分配 ODBC 连接句柄。";
                continue;
            }
            const SQLUINTEGER 超时秒 = 配置.连接超时秒;
            (void)SQLSetConnectAttrW(
                连接_,
                SQL_LOGIN_TIMEOUT,
                reinterpret_cast<SQLPOINTER>(static_cast<ULONG_PTR>(超时秒)),
                0);

            std::wostringstream 连接文本;
            连接文本 << L"DRIVER={" << 驱动 << L"};"
                << L"SERVER=" << 配置.服务器 << L";"
                << L"DATABASE=" << 数据库 << L";"
                << L"Trusted_Connection=Yes;"
                << L"Encrypt=No;"
                << L"TrustServerCertificate=Yes;"
                << L"Connection Timeout=" << 配置.连接超时秒 << L";";
            std::wstring 可写连接文本 = 连接文本.str();
            std::array<SQLWCHAR, 1024> 输出连接文本{};
            SQLSMALLINT 输出长度 = 0;
            const auto 结果 = SQLDriverConnectW(
                连接_,
                nullptr,
                reinterpret_cast<SQLWCHAR*>(可写连接文本.data()),
                SQL_NTS,
                输出连接文本.data(),
                static_cast<SQLSMALLINT>(输出连接文本.size()),
                &输出长度,
                SQL_DRIVER_NOPROMPT);
            if (ODBC成功(结果)) {
                已连接_ = true;
                return true;
            }
            if (!合并诊断.empty()) {
                合并诊断 += L" | ";
            }
            合并诊断 += std::wstring(驱动) + L": " + 读取ODBC诊断(SQL_HANDLE_DBC, 连接_);
            SQLFreeHandle(SQL_HANDLE_DBC, 连接_);
            连接_ = SQL_NULL_HDBC;
        }
        诊断 = std::move(合并诊断);
        return false;
    }

    SQLHDBC 句柄() const {
        return 连接_;
    }

private:
    void 关闭() {
        if (连接_ != SQL_NULL_HDBC) {
            if (已连接_) {
                SQLDisconnect(连接_);
            }
            SQLFreeHandle(SQL_HANDLE_DBC, 连接_);
            连接_ = SQL_NULL_HDBC;
            已连接_ = false;
        }
        if (环境_ != SQL_NULL_HENV) {
            SQLFreeHandle(SQL_HANDLE_ENV, 环境_);
            环境_ = SQL_NULL_HENV;
        }
    }

    SQLHENV 环境_ = SQL_NULL_HENV;
    SQLHDBC 连接_ = SQL_NULL_HDBC;
    bool 已连接_ = false;
};

class ODBC语句 {
public:
    explicit ODBC语句(SQLHDBC 连接) {
        if (!ODBC成功(SQLAllocHandle(SQL_HANDLE_STMT, 连接, &语句_))) {
            语句_ = SQL_NULL_HSTMT;
        }
    }

    ~ODBC语句() {
        if (语句_ != SQL_NULL_HSTMT) {
            SQLFreeHandle(SQL_HANDLE_STMT, 语句_);
        }
    }

    ODBC语句(const ODBC语句&) = delete;
    ODBC语句& operator=(const ODBC语句&) = delete;

    bool 可用() const {
        return 语句_ != SQL_NULL_HSTMT;
    }

    SQLHSTMT 句柄() const {
        return 语句_;
    }

private:
    SQLHSTMT 语句_ = SQL_NULL_HSTMT;
};

bool 执行无结果SQL(SQLHDBC 连接, std::wstring SQL文本, std::wstring& 诊断) {
    ODBC语句 语句(连接);
    if (!语句.可用()) {
        诊断 = L"无法分配 ODBC 语句句柄。";
        return false;
    }
    const auto 结果 = SQLExecDirectW(
        语句.句柄(),
        reinterpret_cast<SQLWCHAR*>(SQL文本.data()),
        SQL_NTS);
    if (!ODBC成功(结果)) {
        诊断 = 读取ODBC诊断(SQL_HANDLE_STMT, 语句.句柄());
        return false;
    }
    return true;
}

}

SQL数据库适配::SQL数据库适配(SQL数据库配置 配置)
    : 配置_(std::move(配置)) {
}

SQL数据库配置 SQL数据库适配::从环境创建配置() {
    SQL数据库配置 配置;
    if (const auto 服务器 = 读取环境字段(L"HY_EGO_SQL_SERVER"); !服务器.empty()) {
        配置.服务器 = 服务器;
    }
    if (const auto 数据库 = 读取环境字段(L"HY_EGO_SQL_DATABASE"); !数据库.empty()) {
        配置.数据库 = 数据库;
    }
    return 配置;
}

const SQL数据库配置& SQL数据库适配::读取配置() const {
    return 配置_;
}

数据库操作结果 SQL数据库适配::初始化数据库() const {
    if (!配置字段可用(配置_.服务器) || !配置字段可用(配置_.数据库)
        || 配置_.连接超时秒 == 0 || 配置_.连接超时秒 > 30) {
        return 逻辑内失败(L"数据库配置准入", L"服务器、数据库名称或连接超时不符合入口约束。");
    }

    std::wstring 诊断;
    {
        ODBC连接 主连接;
        if (!主连接.打开(配置_, L"master", 诊断)) {
            return 逻辑内失败(L"连接本地 SQL Server", std::move(诊断));
        }
        const std::wstring 数据库字符串 = 转义SQL字符串(配置_.数据库);
        const std::wstring 数据库标识符 = 转义SQL标识符(配置_.数据库);
        std::wstring 建库SQL = L"IF DB_ID(N'" + 数据库字符串
            + L"') IS NULL EXEC(N'CREATE DATABASE [" + 数据库标识符 + L"]');";
        if (!执行无结果SQL(主连接.句柄(), std::move(建库SQL), 诊断)) {
            return 追根因失败(L"初始化数据库", std::move(诊断));
        }
    }

    return {true, false, false, {}, {}};
}


}
