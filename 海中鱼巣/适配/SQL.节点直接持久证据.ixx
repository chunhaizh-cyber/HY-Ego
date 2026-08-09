// 文件规则：本模块唯一拥有节点直接结构事务持久证据的 SQL DDL/DML 与短连接实现。
module;

#include "SQL数据库适配.h"
#include "../核心/节点直接结构合同.数据.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

#include <array>
#include <algorithm>
#include <cstdint>
#include <limits>
#include <mutex>
#include <optional>
#include <span>
#include <sstream>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include <bcrypt.h>
#pragma comment(lib, "odbc32.lib")
#pragma comment(lib, "bcrypt.lib")

export module 海中鱼巣.适配.SQL.节点直接持久证据;

import 海中鱼巣.核心.端口.节点直接持久证据;

namespace 海中鱼巣::节点直接持久证据SQL内部 {

bool ODBC成功(SQLRETURN 值) noexcept { return 值 == SQL_SUCCESS || 值 == SQL_SUCCESS_WITH_INFO; }

std::wstring 读取诊断(SQLSMALLINT 类型, SQLHANDLE 句柄) {
    std::wstring 结果;
    for (SQLSMALLINT 序号 = 1;; ++序号) {
        std::array<SQLWCHAR, 6> 状态{};
        SQLINTEGER 本机错误 = 0;
        std::array<SQLWCHAR, 512> 文本{};
        SQLSMALLINT 长度 = 0;
        if (!ODBC成功(SQLGetDiagRecW(类型, 句柄, 序号, 状态.data(), &本机错误,
                文本.data(), static_cast<SQLSMALLINT>(文本.size()), &长度))) break;
        if (!结果.empty()) 结果 += L" | ";
        结果.append(reinterpret_cast<const wchar_t*>(文本.data()), static_cast<std::size_t>(长度));
    }
    return 结果;
}

class 短连接 final {
public:
    短连接() = default;
    短连接(const 短连接&) = delete;
    短连接& operator=(const 短连接&) = delete;
    ~短连接() { 关闭_(); }

    bool 打开(const SQL数据库配置& 配置) {
        if (!ODBC成功(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &环境_))) return false;
        if (!ODBC成功(SQLSetEnvAttr(环境_, SQL_ATTR_ODBC_VERSION,
                reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0))) return false;
        const std::array<std::wstring_view, 2> 驱动组{
            L"ODBC Driver 18 for SQL Server", L"ODBC Driver 17 for SQL Server"};
        for (const auto 驱动 : 驱动组) {
            if (!ODBC成功(SQLAllocHandle(SQL_HANDLE_DBC, 环境_, &连接_))) continue;
            const SQLUINTEGER 超时 = 配置.连接超时秒;
            (void)SQLSetConnectAttrW(连接_, SQL_LOGIN_TIMEOUT,
                reinterpret_cast<SQLPOINTER>(static_cast<ULONG_PTR>(超时)), 0);
            std::wostringstream 文本;
            文本 << L"DRIVER={" << 驱动 << L"};SERVER=" << 配置.服务器
                << L";DATABASE=" << 配置.数据库
                << L";Trusted_Connection=Yes;Encrypt=No;TrustServerCertificate=Yes;Connection Timeout="
                << 配置.连接超时秒 << L";";
            auto 可写 = 文本.str();
            std::array<SQLWCHAR, 1024> 输出{};
            SQLSMALLINT 输出长度 = 0;
            if (ODBC成功(SQLDriverConnectW(连接_, nullptr,
                    reinterpret_cast<SQLWCHAR*>(可写.data()), SQL_NTS, 输出.data(),
                    static_cast<SQLSMALLINT>(输出.size()), &输出长度, SQL_DRIVER_NOPROMPT))) {
                已连接_ = true;
                return true;
            }
            SQLFreeHandle(SQL_HANDLE_DBC, 连接_);
            连接_ = SQL_NULL_HDBC;
        }
        return false;
    }

    SQLHDBC 句柄() const noexcept { return 连接_; }
    bool 开始事务() {
        if (!ODBC成功(SQLSetConnectAttrW(连接_, SQL_ATTR_AUTOCOMMIT,
                reinterpret_cast<SQLPOINTER>(SQL_AUTOCOMMIT_OFF), 0))) return false;
        事务中_ = true;
        return true;
    }
    bool 提交() {
        if (!事务中_ || !ODBC成功(SQLEndTran(SQL_HANDLE_DBC, 连接_, SQL_COMMIT))) return false;
        事务中_ = false;
        return ODBC成功(SQLSetConnectAttrW(连接_, SQL_ATTR_AUTOCOMMIT,
            reinterpret_cast<SQLPOINTER>(SQL_AUTOCOMMIT_ON), 0));
    }
private:
    void 关闭_() noexcept {
        if (连接_ != SQL_NULL_HDBC) {
            if (事务中_) (void)SQLEndTran(SQL_HANDLE_DBC, 连接_, SQL_ROLLBACK);
            if (已连接_) SQLDisconnect(连接_);
            SQLFreeHandle(SQL_HANDLE_DBC, 连接_);
        }
        if (环境_ != SQL_NULL_HENV) SQLFreeHandle(SQL_HANDLE_ENV, 环境_);
    }
    SQLHENV 环境_ = SQL_NULL_HENV;
    SQLHDBC 连接_ = SQL_NULL_HDBC;
    bool 已连接_ = false;
    bool 事务中_ = false;
};

class 短语句 final {
public:
    explicit 短语句(SQLHDBC 连接) { (void)SQLAllocHandle(SQL_HANDLE_STMT, 连接, &语句_); }
    ~短语句() { if (语句_ != SQL_NULL_HSTMT) SQLFreeHandle(SQL_HANDLE_STMT, 语句_); }
    短语句(const 短语句&) = delete;
    短语句& operator=(const 短语句&) = delete;
    SQLHSTMT 句柄() const noexcept { return 语句_; }
    bool 可用() const noexcept { return 语句_ != SQL_NULL_HSTMT; }
private:
    SQLHSTMT 语句_ = SQL_NULL_HSTMT;
};

bool 准备语句(SQLHSTMT 语句, std::wstring_view 文本) {
    return 语句 != SQL_NULL_HSTMT
        && 文本.size() <= static_cast<std::size_t>(std::numeric_limits<SQLINTEGER>::max())
        && ODBC成功(SQLPrepareW(语句,
            reinterpret_cast<SQLWCHAR*>(const_cast<wchar_t*>(文本.data())),
            static_cast<SQLINTEGER>(文本.size())));
}

template<std::size_t 大小>
bool 绑定定长二进制(SQLHSTMT 语句, SQLUSMALLINT 序号,
    std::array<std::uint8_t, 大小>& 值, SQLLEN& 指示) {
    指示 = static_cast<SQLLEN>(大小);
    return ODBC成功(SQLBindParameter(语句, 序号, SQL_PARAM_INPUT,
        SQL_C_BINARY, SQL_BINARY, static_cast<SQLULEN>(大小), 0,
        值.data(), static_cast<SQLLEN>(大小), &指示));
}

bool 绑定变长二进制(SQLHSTMT 语句, SQLUSMALLINT 序号,
    std::vector<std::uint8_t>& 值, SQLLEN& 指示) {
    if (值.size() > static_cast<std::size_t>(std::numeric_limits<SQLLEN>::max())) return false;
    指示 = static_cast<SQLLEN>(值.size());
    auto* 数据 = 值.empty() ? nullptr : 值.data();
    return ODBC成功(SQLBindParameter(语句, 序号, SQL_PARAM_INPUT,
        SQL_C_BINARY, SQL_LONGVARBINARY, static_cast<SQLULEN>(值.size()), 0,
        数据, 指示, &指示));
}

bool 绑定无符号整数(SQLHSTMT 语句, SQLUSMALLINT 序号,
    SQLUINTEGER& 值, SQLLEN& 指示) {
    指示 = 0;
    return ODBC成功(SQLBindParameter(语句, 序号, SQL_PARAM_INPUT,
        SQL_C_ULONG, SQL_INTEGER, 0, 0, &值, 0, &指示));
}

bool 绑定无符号字节(SQLHSTMT 语句, SQLUSMALLINT 序号,
    SQLCHAR& 值, SQLLEN& 指示) {
    指示 = 0;
    return ODBC成功(SQLBindParameter(语句, 序号, SQL_PARAM_INPUT,
        SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &值, 0, &指示));
}

bool 精确影响一行(SQLHSTMT 语句) {
    SQLLEN 行数 = -1;
    return ODBC成功(SQLRowCount(语句, &行数)) && 行数 == 1;
}

bool 执行(SQLHDBC 连接, std::wstring 文本) {
    短语句 语句(连接);
    return 语句.可用() && ODBC成功(SQLExecDirectW(语句.句柄(),
        reinterpret_cast<SQLWCHAR*>(文本.data()), SQL_NTS));
}

std::array<std::uint8_t, 8> 网络序(std::uint64_t 值) noexcept {
    std::array<std::uint8_t, 8> 输出{};
    for (std::size_t i = 0; i < 输出.size(); ++i)
        输出[i] = static_cast<std::uint8_t>(值 >> ((7 - i) * 8));
    return 输出;
}

std::uint64_t 读取网络序(const std::array<std::uint8_t, 8>& 值) noexcept {
    std::uint64_t 输出 = 0;
    for (const auto 字节 : 值) 输出 = (输出 << 8) | 字节;
    return 输出;
}

std::wstring 十六进制(std::span<const std::uint8_t> 值) {
    static constexpr wchar_t 数字[] = L"0123456789ABCDEF";
    std::wstring 输出 = L"0x";
    输出.reserve(2 + 值.size() * 2);
    for (const auto 字节 : 值) {
        输出.push_back(数字[字节 >> 4]);
        输出.push_back(数字[字节 & 0x0F]);
    }
    return 输出;
}

std::wstring 十六进制(std::uint64_t 值) { const auto 字节 = 网络序(值); return 十六进制(字节); }

template<std::size_t 大小>
std::wstring 十六进制(const std::array<std::uint8_t, 大小>& 值) { return 十六进制(std::span{值}); }

bool 摘要非零(const std::array<std::uint8_t, 32>& 值) noexcept {
    return std::any_of(值.begin(), 值.end(), [](auto 字节) { return 字节 != 0; });
}

std::optional<std::array<std::uint8_t, 32>> 计算摘要(std::span<const std::uint8_t> 材料) {
    BCRYPT_ALG_HANDLE 算法 = nullptr;
    BCRYPT_HASH_HANDLE 哈希 = nullptr;
    std::array<std::uint8_t, 32> 输出{};
    if (!BCRYPT_SUCCESS(BCryptOpenAlgorithmProvider(&算法, BCRYPT_SHA256_ALGORITHM, nullptr, 0))) return std::nullopt;
    const auto 关闭算法 = [&]() { BCryptCloseAlgorithmProvider(算法, 0); };
    if (材料.size() > static_cast<std::size_t>(std::numeric_limits<ULONG>::max())
        || !BCRYPT_SUCCESS(BCryptCreateHash(算法, &哈希, nullptr, 0, nullptr, 0, 0))) {
        关闭算法();
        return std::nullopt;
    }
    const auto 完成 = BCRYPT_SUCCESS(BCryptHashData(哈希,
            const_cast<PUCHAR>(材料.data()), static_cast<ULONG>(材料.size()), 0))
        && BCRYPT_SUCCESS(BCryptFinishHash(哈希, 输出.data(), static_cast<ULONG>(输出.size()), 0));
    BCryptDestroyHash(哈希);
    关闭算法();
    return 完成 ? std::optional{输出} : std::nullopt;
}

std::wstring 身份条件(节点直接事务幂等身份 安装, 节点直接事务幂等身份 事务) {
    return L"安装实例命名域=" + 十六进制(安装.命名域) + L" AND 安装实例键值=" + 十六进制(安装.键值)
        + L" AND 事务命名域=" + 十六进制(事务.命名域) + L" AND 事务键值=" + 十六进制(事务.键值);
}

bool 读取整数(SQLHSTMT 语句, SQLUSMALLINT 列, std::uint64_t& 值) {
    std::array<std::uint8_t, 8> 字节{};
    SQLLEN 长度 = 0;
    if (!ODBC成功(SQLGetData(语句, 列, SQL_C_BINARY, 字节.data(), 字节.size(), &长度)) || 长度 != 8) return false;
    值 = 读取网络序(字节);
    return true;
}

template<std::size_t 大小>
bool 读取定长(SQLHSTMT 语句, SQLUSMALLINT 列, std::array<std::uint8_t, 大小>& 值) {
    SQLLEN 长度 = 0;
    return ODBC成功(SQLGetData(语句, 列, SQL_C_BINARY, 值.data(), 值.size(), &长度))
        && 长度 == static_cast<SQLLEN>(大小);
}

bool 读取字节组(SQLHSTMT 语句, SQLUSMALLINT 列, SQLLEN 期望长度,
    std::vector<std::uint8_t>& 值) {
    if (期望长度 < 0 || 期望长度 > 67'108'864) return false;
    try { 值.resize(static_cast<std::size_t>(期望长度)); } catch (...) { return false; }
    if (期望长度 == 0) return true;
    SQLLEN 实际长度 = 0;
    return ODBC成功(SQLGetData(语句, 列, SQL_C_BINARY,
        值.data(), 期望长度, &实际长度)) && 实际长度 == 期望长度;
}

struct 尝试行 {
    std::uint64_t 序号 = 0;
    std::uint32_t 格式 = 0;
    std::array<std::uint8_t, 32> 请求摘要{};
    std::array<std::uint8_t, 32> 执行摘要{};
    std::vector<std::uint8_t> 材料;
    std::uint8_t 状态 = 0;
    std::optional<std::uint64_t> 发布代次;
    std::optional<std::array<std::uint8_t, 32>> 结果摘要;
};

std::optional<std::vector<尝试行>> 读取尝试(SQLHDBC 连接,
    节点直接事务幂等身份 安装, 节点直接事务幂等身份 事务) {
    短语句 语句(连接);
    if (!语句.可用()) return std::nullopt;
    constexpr std::wstring_view 文本 =
        L"SELECT 尝试序号,材料格式版本,请求意图摘要,执行证据摘要,DATALENGTH(写集材料),写集材料,证据状态,发布代次,结果摘要 "
        L"FROM dbo.节点直接结构事务证据_v1 WITH (UPDLOCK,HOLDLOCK) WHERE "
        L"安装实例命名域=? AND 安装实例键值=? AND 事务命名域=? AND 事务键值=? ORDER BY 尝试序号";
    auto 安装域 = 网络序(安装.命名域);
    auto 安装键 = 网络序(安装.键值);
    auto 事务域 = 网络序(事务.命名域);
    auto 事务键 = 网络序(事务.键值);
    std::array<SQLLEN, 4> 指示{};
    if (!准备语句(语句.句柄(), 文本)
        || !绑定定长二进制(语句.句柄(), 1, 安装域, 指示[0])
        || !绑定定长二进制(语句.句柄(), 2, 安装键, 指示[1])
        || !绑定定长二进制(语句.句柄(), 3, 事务域, 指示[2])
        || !绑定定长二进制(语句.句柄(), 4, 事务键, 指示[3])
        || !ODBC成功(SQLExecute(语句.句柄()))) return std::nullopt;
    std::vector<尝试行> 输出;
    while (true) {
        const auto 获取 = SQLFetch(语句.句柄());
        if (获取 == SQL_NO_DATA) break;
        if (!ODBC成功(获取)) return std::nullopt;
        尝试行 行;
        SQLLEN 长度 = 0;
        SQLUINTEGER 格式 = 0;
        SQLINTEGER 材料长度 = -1;
        SQLCHAR 状态 = 0;
        if (!读取整数(语句.句柄(), 1, 行.序号)
            || !ODBC成功(SQLGetData(语句.句柄(), 2, SQL_C_ULONG, &格式, sizeof(格式), &长度))
            || !读取定长(语句.句柄(), 3, 行.请求摘要)
            || !读取定长(语句.句柄(), 4, 行.执行摘要)
            || !ODBC成功(SQLGetData(语句.句柄(), 5, SQL_C_SLONG, &材料长度, sizeof(材料长度), &长度))
            || !读取字节组(语句.句柄(), 6, 材料长度, 行.材料)
            || !ODBC成功(SQLGetData(语句.句柄(), 7, SQL_C_UTINYINT, &状态, sizeof(状态), &长度))) return std::nullopt;
        行.格式 = 格式;
        行.状态 = 状态;
        std::array<std::uint8_t, 8> 代次字节{};
        const auto 代次结果 = SQLGetData(语句.句柄(), 8, SQL_C_BINARY, 代次字节.data(),代次字节.size(), &长度);
        if (ODBC成功(代次结果) && 长度 == 8)
            行.发布代次 = 读取网络序(代次字节);
        else if (!(ODBC成功(代次结果) && 长度 == SQL_NULL_DATA)) return std::nullopt;
        std::array<std::uint8_t, 32> 摘要{};
        const auto 摘要结果 = SQLGetData(语句.句柄(), 9, SQL_C_BINARY, 摘要.data(), 摘要.size(), &长度);
        if (ODBC成功(摘要结果) && 长度 == 32)
            行.结果摘要 = 摘要;
        else if (!(ODBC成功(摘要结果) && 长度 == SQL_NULL_DATA)) return std::nullopt;
        输出.push_back(std::move(行));
    }
    return 输出;
}

bool 同义(const 尝试行& 行, const 节点直接持久准备请求& 请求) {
    return 行.格式 == 请求.材料格式版本 && 行.请求摘要 == 请求.请求意图摘要
        && 行.执行摘要 == 请求.执行证据摘要 && 行.材料 == 请求.写集材料;
}

} // namespace 海中鱼巣::节点直接持久证据SQL内部

export namespace 海中鱼巣 {

class SQL节点直接持久证据适配 final
    : public 节点直接持久证据写入端口 {
public:
    explicit SQL节点直接持久证据适配(SQL数据库适配& 基础适配) noexcept : 基础适配_(基础适配) {}

    节点直接持久端口结果 初始化证据结构(节点直接事务幂等身份 安装实例身份) {
        using namespace 节点直接持久证据SQL内部;
        std::scoped_lock 锁(互斥量_);
        if (!节点直接事务幂等身份完整(安装实例身份)) return {节点直接持久端口状态::损坏, 0};
        if (!基础适配_.初始化数据库().成功) return {节点直接持久端口状态::资源失败, 0};
        短连接 连接;
        if (!连接.打开(基础适配_.读取配置()) || !连接.开始事务()) return {节点直接持久端口状态::资源失败, 0};
        const auto 安装域 = 十六进制(安装实例身份.命名域);
        const auto 安装键 = 十六进制(安装实例身份.键值);
        std::wstring 脚本 = LR"SQL(
DECLARE @实例表 bit=CASE WHEN OBJECT_ID(N'dbo.节点直接安装实例证据_v1',N'U') IS NULL THEN 0 ELSE 1 END;
DECLARE @事务表 bit=CASE WHEN OBJECT_ID(N'dbo.节点直接结构事务证据_v1',N'U') IS NULL THEN 0 ELSE 1 END;
IF @实例表<>@事务表 THROW 51000,N'节点直接持久证据表只存在一张',1;
IF @实例表=0
BEGIN
 CREATE TABLE dbo.节点直接安装实例证据_v1(安装实例命名域 binary(8) NOT NULL,安装实例键值 binary(8) NOT NULL,实例状态 tinyint NOT NULL,创建时间 datetime2(7) NOT NULL DEFAULT SYSDATETIME(),更新时间 datetime2(7) NOT NULL DEFAULT SYSDATETIME(),CONSTRAINT PK_节点直接安装实例证据_v1 PRIMARY KEY(安装实例命名域,安装实例键值));
 CREATE TABLE dbo.节点直接结构事务证据_v1(安装实例命名域 binary(8) NOT NULL,安装实例键值 binary(8) NOT NULL,事务命名域 binary(8) NOT NULL,事务键值 binary(8) NOT NULL,尝试序号 binary(8) NOT NULL,材料格式版本 int NOT NULL,请求意图摘要 binary(32) NOT NULL,执行证据摘要 binary(32) NOT NULL,写集材料 varbinary(max) NOT NULL,证据状态 tinyint NOT NULL,发布代次 binary(8) NULL,结果摘要 binary(32) NULL,创建时间 datetime2(7) NOT NULL DEFAULT SYSDATETIME(),更新时间 datetime2(7) NOT NULL DEFAULT SYSDATETIME(),CONSTRAINT PK_节点直接结构事务证据_v1 PRIMARY KEY(安装实例命名域,安装实例键值,事务命名域,事务键值,尝试序号));
 INSERT dbo.节点直接安装实例证据_v1(安装实例命名域,安装实例键值,实例状态) VALUES()SQL"
            + 安装域 + L"," + 安装键 + L",1); END ELSE IF NOT EXISTS(SELECT 1 FROM dbo.节点直接安装实例证据_v1 WITH(UPDLOCK,HOLDLOCK) WHERE 安装实例命名域="
            + 安装域 + L" AND 安装实例键值=" + 安装键 + LR"SQL() THROW 51001,N'证据表已存在但安装实例未登记',1;
IF (SELECT COUNT(*) FROM sys.columns WHERE object_id=OBJECT_ID(N'dbo.节点直接安装实例证据_v1'))<>5
 OR (SELECT COUNT(*) FROM sys.columns WHERE object_id=OBJECT_ID(N'dbo.节点直接结构事务证据_v1'))<>14
 THROW 51002,N'节点直接持久证据列数量漂移',1;
IF EXISTS(
 SELECT 名称,类型名,最大长度,可空 FROM (VALUES
  (N'安装实例命名域',N'binary',CONVERT(smallint,8),CONVERT(bit,0)),(N'安装实例键值',N'binary',8,0),
  (N'实例状态',N'tinyint',1,0),(N'创建时间',N'datetime2',8,0),(N'更新时间',N'datetime2',8,0)) E(名称,类型名,最大长度,可空)
 EXCEPT SELECT c.name,t.name,c.max_length,c.is_nullable FROM sys.columns c JOIN sys.types t ON c.user_type_id=t.user_type_id WHERE c.object_id=OBJECT_ID(N'dbo.节点直接安装实例证据_v1'))
 THROW 51003,N'节点直接安装实例证据列结构漂移',1;
IF EXISTS(
 SELECT 名称,类型名,最大长度,可空 FROM (VALUES
  (N'安装实例命名域',N'binary',CONVERT(smallint,8),CONVERT(bit,0)),(N'安装实例键值',N'binary',8,0),
  (N'事务命名域',N'binary',8,0),(N'事务键值',N'binary',8,0),(N'尝试序号',N'binary',8,0),
  (N'材料格式版本',N'int',4,0),(N'请求意图摘要',N'binary',32,0),(N'执行证据摘要',N'binary',32,0),
  (N'写集材料',N'varbinary',CONVERT(smallint,-1),0),(N'证据状态',N'tinyint',1,0),
  (N'发布代次',N'binary',8,1),(N'结果摘要',N'binary',32,1),(N'创建时间',N'datetime2',8,0),(N'更新时间',N'datetime2',8,0)) E(名称,类型名,最大长度,可空)
 EXCEPT SELECT c.name,t.name,c.max_length,c.is_nullable FROM sys.columns c JOIN sys.types t ON c.user_type_id=t.user_type_id WHERE c.object_id=OBJECT_ID(N'dbo.节点直接结构事务证据_v1'))
 THROW 51004,N'节点直接结构事务证据列结构漂移',1;
IF (SELECT COUNT(*) FROM sys.indexes WHERE object_id=OBJECT_ID(N'dbo.节点直接安装实例证据_v1') AND is_primary_key=1)<>1
 OR (SELECT COUNT(*) FROM sys.indexes WHERE object_id=OBJECT_ID(N'dbo.节点直接结构事务证据_v1') AND is_primary_key=1)<>1
 THROW 51005,N'节点直接持久证据主键漂移',1;
DECLARE @状态 tinyint=(SELECT 实例状态 FROM dbo.节点直接安装实例证据_v1 WITH(UPDLOCK,HOLDLOCK) WHERE 安装实例命名域=)SQL"
            + 安装域 + L" AND 安装实例键值=" + 安装键 + L");"
            + L"IF @状态 NOT IN(1,2) THROW 51006,N'安装实例状态损坏',1;"
            + L"IF @状态=1 AND EXISTS(SELECT 1 FROM dbo.节点直接结构事务证据_v1 WHERE 安装实例命名域="
            + 安装域 + L" AND 安装实例键值=" + 安装键 + L") THROW 51007,N'确认从未发布实例存在事务证据',1;";
        if (!执行(连接.句柄(), std::move(脚本)) || !连接.提交()) return {节点直接持久端口状态::损坏, 0};
        return {节点直接持久端口状态::精确同义, 0};
    }

    节点直接持久端口结果 准备(const 节点直接持久准备请求& 请求) override {
        using namespace 节点直接持久证据SQL内部;
        std::scoped_lock 锁(互斥量_);
        if (!节点直接事务幂等身份完整(请求.安装实例身份)
            || !节点直接事务幂等身份完整(请求.事务身份)
            || 请求.材料格式版本 != 2 || !摘要非零(请求.请求意图摘要)
            || !摘要非零(请求.执行证据摘要) || 请求.写集材料.size() > 67'108'864)
            return {节点直接持久端口状态::损坏, 0};
        if (!基础适配_.初始化数据库().成功) return {节点直接持久端口状态::资源失败, 0};
        短连接 连接;
        if (!连接.打开(基础适配_.读取配置()) || !连接.开始事务()) return {节点直接持久端口状态::资源失败, 0};
        const auto 尝试组 = 读取尝试(连接.句柄(), 请求.安装实例身份, 请求.事务身份);
        if (!尝试组) return {节点直接持久端口状态::资源失败, 0};
        bool 已有见证 = false;
        for (std::size_t i = 0; i < 尝试组->size(); ++i) {
            if ((*尝试组)[i].序号 != i + 1 || (*尝试组)[i].状态 < 1 || (*尝试组)[i].状态 > 5)
                return {节点直接持久端口状态::损坏, 0};
            if ((*尝试组)[i].状态 == 2) {
                if (已有见证 || i + 1 != 尝试组->size()) return {节点直接持久端口状态::损坏, 0};
                已有见证 = true;
            }
        }
        if (!尝试组->empty()) {
            const auto& 最后 = 尝试组->back();
            if (已有见证) return {同义(最后, 请求) ? 节点直接持久端口状态::已见证 : 节点直接持久端口状态::冲突, 最后.序号};
            if (最后.状态 == 1 || 最后.状态 == 3)
                return {同义(最后, 请求) ? 节点直接持久端口状态::精确同义 : 节点直接持久端口状态::冲突, 最后.序号};
            if (最后.状态 != 5 || 最后.序号 == std::numeric_limits<std::uint64_t>::max())
                return {节点直接持久端口状态::损坏, 0};
        }
        const std::uint64_t 新序号 = 尝试组->empty() ? 1 : 尝试组->back().序号 + 1;
        auto 安装域 = 网络序(请求.安装实例身份.命名域);
        auto 安装键 = 网络序(请求.安装实例身份.键值);
        auto 事务域 = 网络序(请求.事务身份.命名域);
        auto 事务键 = 网络序(请求.事务身份.键值);
        auto 序号字节 = 网络序(新序号);
        auto 请求摘要 = 请求.请求意图摘要;
        auto 执行摘要 = 请求.执行证据摘要;
        auto 写集材料 = 请求.写集材料;
        SQLUINTEGER 格式 = 请求.材料格式版本;
        SQLCHAR 状态 = 1;
        std::array<SQLLEN, 10> 指示{};
        短语句 插入(连接.句柄());
        constexpr std::wstring_view 插入文本 =
            L"INSERT dbo.节点直接结构事务证据_v1(安装实例命名域,安装实例键值,事务命名域,事务键值,尝试序号,材料格式版本,请求意图摘要,执行证据摘要,写集材料,证据状态) VALUES(?,?,?,?,?,?,?,?,?,?)";
        if (!插入.可用() || !准备语句(插入.句柄(), 插入文本)
            || !绑定定长二进制(插入.句柄(), 1, 安装域, 指示[0])
            || !绑定定长二进制(插入.句柄(), 2, 安装键, 指示[1])
            || !绑定定长二进制(插入.句柄(), 3, 事务域, 指示[2])
            || !绑定定长二进制(插入.句柄(), 4, 事务键, 指示[3])
            || !绑定定长二进制(插入.句柄(), 5, 序号字节, 指示[4])
            || !绑定无符号整数(插入.句柄(), 6, 格式, 指示[5])
            || !绑定定长二进制(插入.句柄(), 7, 请求摘要, 指示[6])
            || !绑定定长二进制(插入.句柄(), 8, 执行摘要, 指示[7])
            || !绑定变长二进制(插入.句柄(), 9, 写集材料, 指示[8])
            || !绑定无符号字节(插入.句柄(), 10, 状态, 指示[9])
            || !ODBC成功(SQLExecute(插入.句柄())) || !精确影响一行(插入.句柄()))
            return {节点直接持久端口状态::资源失败, 0};

        短语句 更新实例(连接.句柄());
        constexpr std::wstring_view 更新实例文本 =
            L"UPDATE dbo.节点直接安装实例证据_v1 SET 实例状态=2,更新时间=SYSDATETIME() WHERE 安装实例命名域=? AND 安装实例键值=? AND 实例状态 IN (1,2)";
        if (!更新实例.可用() || !准备语句(更新实例.句柄(), 更新实例文本)
            || !绑定定长二进制(更新实例.句柄(), 1, 安装域, 指示[0])
            || !绑定定长二进制(更新实例.句柄(), 2, 安装键, 指示[1])
            || !ODBC成功(SQLExecute(更新实例.句柄())) || !精确影响一行(更新实例.句柄()))
            return {节点直接持久端口状态::损坏, 0};
        return 连接.提交() ? 节点直接持久端口结果{节点直接持久端口状态::已准备, 新序号}
            : 节点直接持久端口结果{节点直接持久端口状态::结果未知, 新序号};
    }

    节点直接持久端口结果 标记已发布(const 节点直接持久发布见证请求& 请求) override {
        return 转换状态_(请求);
    }

    节点直接持久端口结果 标记已撤销未发布(const 节点直接持久撤销见证请求& 请求) override {
        return 转换状态_(请求);
    }

private:
    template<class 请求类型>
    节点直接持久端口结果 转换状态_(const 请求类型& 请求) {
        using namespace 节点直接持久证据SQL内部;
        std::scoped_lock 锁(互斥量_);
        if (!节点直接事务幂等身份完整(请求.安装实例身份)
            || !节点直接事务幂等身份完整(请求.事务身份) || 请求.尝试序号 == 0
            || !摘要非零(请求.请求意图摘要) || !摘要非零(请求.执行证据摘要))
            return {节点直接持久端口状态::损坏, 请求.尝试序号};
        if (!基础适配_.初始化数据库().成功) return {节点直接持久端口状态::资源失败, 请求.尝试序号};
        短连接 连接;
        if (!连接.打开(基础适配_.读取配置()) || !连接.开始事务()) return {节点直接持久端口状态::资源失败, 请求.尝试序号};
        const auto 尝试组 = 读取尝试(连接.句柄(), 请求.安装实例身份, 请求.事务身份);
        if (!尝试组) return {节点直接持久端口状态::资源失败, 请求.尝试序号};
        bool 已有见证 = false;
        for (std::size_t 序号 = 0; 序号 < 尝试组->size(); ++序号) {
            const auto& 尝试 = (*尝试组)[序号];
            if (尝试.序号 != 序号 + 1 || 尝试.状态 < 1 || 尝试.状态 > 5)
                return {节点直接持久端口状态::损坏, 请求.尝试序号};
            if (尝试.状态 == 2) {
                if (已有见证 || 序号 + 1 != 尝试组->size())
                    return {节点直接持久端口状态::损坏, 请求.尝试序号};
                已有见证 = true;
            }
        }
        const auto 位置 = std::find_if(尝试组->begin(), 尝试组->end(), [&](const auto& 项) { return 项.序号 == 请求.尝试序号; });
        if (位置 == 尝试组->end() || 位置->请求摘要 != 请求.请求意图摘要 || 位置->执行摘要 != 请求.执行证据摘要)
            return {节点直接持久端口状态::损坏, 请求.尝试序号};
        if (已有见证 && 位置->状态 != 2)
            return {节点直接持久端口状态::损坏, 请求.尝试序号};
        if constexpr (std::is_same_v<请求类型, 节点直接持久发布见证请求>) {
            const auto& 发布请求 = 请求;
            if (发布请求.发布代次 == 0 || !摘要非零(发布请求.结果摘要)) return {节点直接持久端口状态::损坏, 请求.尝试序号};
            if (位置->状态 == 2) return {位置->发布代次 == 发布请求.发布代次 && 位置->结果摘要 == 发布请求.结果摘要 ? 节点直接持久端口状态::精确同义 : 节点直接持久端口状态::损坏, 请求.尝试序号};
            if (位置->状态 != 1 || 位置->发布代次 || 位置->结果摘要) return {节点直接持久端口状态::损坏, 请求.尝试序号};
            auto 发布代次 = 网络序(发布请求.发布代次);
            auto 结果摘要 = 发布请求.结果摘要;
            auto 安装域 = 网络序(请求.安装实例身份.命名域);
            auto 安装键 = 网络序(请求.安装实例身份.键值);
            auto 事务域 = 网络序(请求.事务身份.命名域);
            auto 事务键 = 网络序(请求.事务身份.键值);
            auto 尝试序号 = 网络序(请求.尝试序号);
            std::array<SQLLEN, 7> 指示{};
            短语句 更新(连接.句柄());
            constexpr std::wstring_view 文本 =
                L"UPDATE dbo.节点直接结构事务证据_v1 SET 证据状态=2,发布代次=?,结果摘要=?,更新时间=SYSDATETIME() WHERE 安装实例命名域=? AND 安装实例键值=? AND 事务命名域=? AND 事务键值=? AND 尝试序号=? AND 证据状态=1";
            if (!更新.可用() || !准备语句(更新.句柄(), 文本)
                || !绑定定长二进制(更新.句柄(), 1, 发布代次, 指示[0])
                || !绑定定长二进制(更新.句柄(), 2, 结果摘要, 指示[1])
                || !绑定定长二进制(更新.句柄(), 3, 安装域, 指示[2])
                || !绑定定长二进制(更新.句柄(), 4, 安装键, 指示[3])
                || !绑定定长二进制(更新.句柄(), 5, 事务域, 指示[4])
                || !绑定定长二进制(更新.句柄(), 6, 事务键, 指示[5])
                || !绑定定长二进制(更新.句柄(), 7, 尝试序号, 指示[6])
                || !ODBC成功(SQLExecute(更新.句柄())) || !精确影响一行(更新.句柄()))
                return {节点直接持久端口状态::资源失败, 请求.尝试序号};
        } else {
            if (位置->状态 == 5) return {节点直接持久端口状态::精确同义, 请求.尝试序号};
            const auto 实际摘要 = 计算摘要(位置->材料);
            if (位置->状态 != 1 || 位置->发布代次 || 位置->结果摘要 || !实际摘要 || *实际摘要 != 请求.写集材料摘要)
                return {节点直接持久端口状态::损坏, 请求.尝试序号};
            auto 安装域 = 网络序(请求.安装实例身份.命名域);
            auto 安装键 = 网络序(请求.安装实例身份.键值);
            auto 事务域 = 网络序(请求.事务身份.命名域);
            auto 事务键 = 网络序(请求.事务身份.键值);
            auto 尝试序号 = 网络序(请求.尝试序号);
            std::array<SQLLEN, 5> 指示{};
            短语句 更新(连接.句柄());
            constexpr std::wstring_view 文本 =
                L"UPDATE dbo.节点直接结构事务证据_v1 SET 证据状态=5,更新时间=SYSDATETIME() WHERE 安装实例命名域=? AND 安装实例键值=? AND 事务命名域=? AND 事务键值=? AND 尝试序号=? AND 证据状态=1";
            if (!更新.可用() || !准备语句(更新.句柄(), 文本)
                || !绑定定长二进制(更新.句柄(), 1, 安装域, 指示[0])
                || !绑定定长二进制(更新.句柄(), 2, 安装键, 指示[1])
                || !绑定定长二进制(更新.句柄(), 3, 事务域, 指示[2])
                || !绑定定长二进制(更新.句柄(), 4, 事务键, 指示[3])
                || !绑定定长二进制(更新.句柄(), 5, 尝试序号, 指示[4])
                || !ODBC成功(SQLExecute(更新.句柄())) || !精确影响一行(更新.句柄()))
                return {节点直接持久端口状态::资源失败, 请求.尝试序号};
        }
        constexpr auto 成功状态 = std::is_same_v<请求类型, 节点直接持久发布见证请求>
            ? 节点直接持久端口状态::已见证 : 节点直接持久端口状态::已撤销未发布;
        return 连接.提交() ? 节点直接持久端口结果{成功状态, 请求.尝试序号}
            : 节点直接持久端口结果{节点直接持久端口状态::结果未知, 请求.尝试序号};
    }

    SQL数据库适配& 基础适配_;
    std::mutex 互斥量_;
};

} // namespace 海中鱼巣
