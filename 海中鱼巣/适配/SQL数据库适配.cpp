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
#include <limits>
#include <sstream>
#include <string_view>
#include <utility>

#pragma comment(lib, "odbc32.lib")

namespace 海中鱼巣 {

namespace {

constexpr std::size_t 最大配置字段长度 = 128;
constexpr std::size_t 最大来源入口长度 = 128;
constexpr std::size_t 最大查询记录数 = 100;

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
    return {false, true, false, 0, std::move(阶段), std::move(诊断)};
}

数据库操作结果 追根因失败(std::wstring 阶段, std::wstring 诊断, std::uint64_t 审计编号 = 0) {
    return {false, false, true, 审计编号, std::move(阶段), std::move(诊断)};
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

bool 快照可写入(const 结构统计快照& 快照) {
    return 快照.命名空间.类型 == 缓存类型::结构统计
        && 快照.命名空间.来源 == 来源结构类型::结构仓库集合
        && 快照.命名空间.来源版本 > 0
        && 快照.命名空间.规则版本 > 0
        && 快照.命名空间.值类型 == 缓存值类型::结构数量
        && 快照.命名空间.可持久化;
}

bool 数量可写入(std::uint64_t 数量) {
    return 数量 <= static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max());
}

bool 读取文本列(SQLHSTMT 语句, SQLUSMALLINT 列号, std::wstring& 文本) {
    std::array<wchar_t, 512> 缓冲{};
    SQLLEN 指示 = 0;
    const auto 结果 = SQLGetData(
        语句,
        列号,
        SQL_C_WCHAR,
        缓冲.data(),
        static_cast<SQLLEN>(缓冲.size() * sizeof(wchar_t)),
        &指示);
    if (!ODBC成功(结果) || 指示 == SQL_NULL_DATA) {
        return false;
    }
    文本.assign(缓冲.data());
    return true;
}

bool 读取整数列(SQLHSTMT 语句, SQLUSMALLINT 列号, std::uint64_t& 数值) {
    std::int64_t 临时值 = 0;
    SQLLEN 指示 = 0;
    const auto 结果 = SQLGetData(语句, 列号, SQL_C_SBIGINT, &临时值, sizeof(临时值), &指示);
    if (!ODBC成功(结果) || 指示 == SQL_NULL_DATA || 临时值 < 0) {
        return false;
    }
    数值 = static_cast<std::uint64_t>(临时值);
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

    ODBC连接 数据连接;
    if (!数据连接.打开(配置_, 配置_.数据库, 诊断)) {
        return 追根因失败(L"连接审计数据库", std::move(诊断));
    }
    std::wstring 建表SQL = LR"SQL(
IF OBJECT_ID(N'dbo.结构统计审计', N'U') IS NULL
BEGIN
    CREATE TABLE dbo.结构统计审计(
        审计编号 bigint IDENTITY(1,1) NOT NULL CONSTRAINT PK_结构统计审计 PRIMARY KEY,
        记录时间 datetime2(3) NOT NULL CONSTRAINT DF_结构统计审计_记录时间 DEFAULT SYSDATETIME(),
        来源入口 nvarchar(128) NOT NULL,
        来源版本 bigint NOT NULL,
        规则版本 bigint NOT NULL,
        节点数 bigint NOT NULL,
        关系数 bigint NOT NULL,
        索引数 bigint NOT NULL,
        CONSTRAINT CK_结构统计审计_版本 CHECK (来源版本 > 0 AND 规则版本 > 0),
        CONSTRAINT CK_结构统计审计_数量 CHECK (节点数 >= 0 AND 关系数 >= 0 AND 索引数 >= 0)
    );
END;
IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE object_id = OBJECT_ID(N'dbo.结构统计审计') AND name = N'IX_结构统计审计_记录时间')
BEGIN
    CREATE INDEX IX_结构统计审计_记录时间 ON dbo.结构统计审计(记录时间 DESC, 审计编号 DESC);
END;
)SQL";
    if (!执行无结果SQL(数据连接.句柄(), std::move(建表SQL), 诊断)) {
        return 追根因失败(L"初始化审计表", std::move(诊断));
    }
    return {true, false, false, 0, {}, {}};
}

数据库操作结果 SQL数据库适配::写入结构统计审计(const 结构统计快照& 快照, std::wstring 来源入口) const {
    if (!快照可写入(快照) || 来源入口.empty() || 来源入口.size() > 最大来源入口长度
        || !数量可写入(快照.节点数) || !数量可写入(快照.关系数) || !数量可写入(快照.索引数)) {
        return 逻辑内失败(L"结构统计审计写入准入", L"快照命名空间、来源入口或结构数量不符合写入约束。");
    }
    const auto 初始化结果 = 初始化数据库();
    if (!初始化结果.成功) {
        return 初始化结果;
    }

    std::wstring 诊断;
    ODBC连接 连接;
    if (!连接.打开(配置_, 配置_.数据库, 诊断)) {
        return 逻辑内失败(L"连接审计数据库", std::move(诊断));
    }
    ODBC语句 语句(连接.句柄());
    if (!语句.可用()) {
        return 追根因失败(L"分配审计写入语句", L"无法分配 ODBC 语句句柄。");
    }

    std::wstring SQL文本 = LR"SQL(
SET NOCOUNT ON;
INSERT INTO dbo.结构统计审计(来源入口, 来源版本, 规则版本, 节点数, 关系数, 索引数)
VALUES (?, ?, ?, ?, ?, ?);
SELECT CAST(SCOPE_IDENTITY() AS bigint) AS 审计编号;
)SQL";
    if (!ODBC成功(SQLPrepareW(语句.句柄(), reinterpret_cast<SQLWCHAR*>(SQL文本.data()), SQL_NTS))) {
        return 追根因失败(L"准备审计写入语句", 读取ODBC诊断(SQL_HANDLE_STMT, 语句.句柄()));
    }

    SQLLEN 来源指示 = SQL_NTS;
    std::array<std::int64_t, 5> 数值组{
        static_cast<std::int64_t>(快照.命名空间.来源版本),
        static_cast<std::int64_t>(快照.命名空间.规则版本),
        static_cast<std::int64_t>(快照.节点数),
        static_cast<std::int64_t>(快照.关系数),
        static_cast<std::int64_t>(快照.索引数)
    };
    std::array<SQLLEN, 5> 数值指示{};
    auto 绑定结果 = SQLBindParameter(
        语句.句柄(), 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR,
        最大来源入口长度, 0, 来源入口.data(),
        static_cast<SQLLEN>((来源入口.size() + 1) * sizeof(wchar_t)), &来源指示);
    for (SQLUSMALLINT 索引 = 0; ODBC成功(绑定结果) && 索引 < 数值组.size(); ++索引) {
        绑定结果 = SQLBindParameter(
            语句.句柄(),
            static_cast<SQLUSMALLINT>(索引 + 2),
            SQL_PARAM_INPUT,
            SQL_C_SBIGINT,
            SQL_BIGINT,
            19,
            0,
            &数值组[索引],
            sizeof(std::int64_t),
            &数值指示[索引]);
    }
    if (!ODBC成功(绑定结果)) {
        return 追根因失败(L"绑定审计写入参数", 读取ODBC诊断(SQL_HANDLE_STMT, 语句.句柄()));
    }
    if (!ODBC成功(SQLExecute(语句.句柄()))) {
        return 追根因失败(L"执行审计写入", 读取ODBC诊断(SQL_HANDLE_STMT, 语句.句柄()));
    }

    SQLSMALLINT 列数 = 0;
    while (ODBC成功(SQLNumResultCols(语句.句柄(), &列数)) && 列数 == 0) {
        const auto 更多结果 = SQLMoreResults(语句.句柄());
        if (更多结果 == SQL_NO_DATA || !ODBC成功(更多结果)) {
            return 追根因失败(L"读取审计编号", 读取ODBC诊断(SQL_HANDLE_STMT, 语句.句柄()));
        }
    }
    const auto 获取审计编号结果 = SQLFetch(语句.句柄());
    if (!ODBC成功(获取审计编号结果)) {
        return 追根因失败(L"读取审计编号", 读取ODBC诊断(SQL_HANDLE_STMT, 语句.句柄()));
    }
    std::uint64_t 审计编号 = 0;
    if (!读取整数列(语句.句柄(), 1, 审计编号) || 审计编号 == 0) {
        return 追根因失败(L"读取审计编号", L"写入后没有读到有效审计编号。");
    }

    const auto 读回 = 读取最近结构统计审计(1);
    if (!读回.操作.成功 || 读回.记录组.size() != 1
        || 读回.记录组.front().审计编号 != 审计编号
        || 读回.记录组.front().节点数 != 快照.节点数
        || 读回.记录组.front().关系数 != 快照.关系数
        || 读回.记录组.front().索引数 != 快照.索引数) {
        return 追根因失败(L"审计写入后读回", 读回.操作.诊断.empty() ? L"最近审计记录与本次写入不一致。" : 读回.操作.诊断, 审计编号);
    }
    return {true, false, false, 审计编号, {}, {}};
}

数据库查询结果 SQL数据库适配::读取最近结构统计审计(std::size_t 数量上限) const {
    数据库查询结果 返回结果;
    if (数量上限 == 0 || 数量上限 > 最大查询记录数) {
        返回结果.操作 = 逻辑内失败(L"审计查询准入", L"查询数量上限必须在 1 到 100 之间。");
        return 返回结果;
    }
    const auto 初始化结果 = 初始化数据库();
    if (!初始化结果.成功) {
        返回结果.操作 = 初始化结果;
        return 返回结果;
    }

    std::wstring 诊断;
    ODBC连接 连接;
    if (!连接.打开(配置_, 配置_.数据库, 诊断)) {
        返回结果.操作 = 逻辑内失败(L"连接审计数据库", std::move(诊断));
        return 返回结果;
    }
    ODBC语句 语句(连接.句柄());
    if (!语句.可用()) {
        返回结果.操作 = 追根因失败(L"分配审计查询语句", L"无法分配 ODBC 语句句柄。");
        return 返回结果;
    }
    std::wostringstream SQL文本流;
    SQL文本流 << L"SELECT TOP (" << 数量上限 << L") "
        << L"审计编号, CONVERT(nvarchar(23), 记录时间, 121), 来源入口, 来源版本, 规则版本, 节点数, 关系数, 索引数 "
        << L"FROM dbo.结构统计审计 ORDER BY 审计编号 DESC;";
    std::wstring SQL文本 = SQL文本流.str();
    if (!ODBC成功(SQLExecDirectW(语句.句柄(), reinterpret_cast<SQLWCHAR*>(SQL文本.data()), SQL_NTS))) {
        返回结果.操作 = 追根因失败(L"执行审计查询", 读取ODBC诊断(SQL_HANDLE_STMT, 语句.句柄()));
        return 返回结果;
    }

    while (true) {
        const auto 获取结果 = SQLFetch(语句.句柄());
        if (获取结果 == SQL_NO_DATA) {
            break;
        }
        if (!ODBC成功(获取结果)) {
            返回结果.操作 = 追根因失败(L"读取审计查询结果", 读取ODBC诊断(SQL_HANDLE_STMT, 语句.句柄()));
            返回结果.记录组.clear();
            return 返回结果;
        }
        数据库审计记录 记录;
        if (!读取整数列(语句.句柄(), 1, 记录.审计编号)
            || !读取文本列(语句.句柄(), 2, 记录.记录时间)
            || !读取文本列(语句.句柄(), 3, 记录.来源入口)
            || !读取整数列(语句.句柄(), 4, 记录.来源版本)
            || !读取整数列(语句.句柄(), 5, 记录.规则版本)
            || !读取整数列(语句.句柄(), 6, 记录.节点数)
            || !读取整数列(语句.句柄(), 7, 记录.关系数)
            || !读取整数列(语句.句柄(), 8, 记录.索引数)) {
            返回结果.操作 = 追根因失败(L"转换审计查询结果", L"数据库列为空、为负数或类型不匹配。");
            返回结果.记录组.clear();
            return 返回结果;
        }
        返回结果.记录组.push_back(std::move(记录));
    }
    返回结果.操作 = {true, false, false, 0, {}, {}};
    return 返回结果;
}

}
