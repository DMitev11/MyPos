#include "pch.h"
#include "Database.h"

using namespace database;
Database* Database::instance = nullptr;

void database::ErrorHandle(SQLRETURN res, SQLHSTMT handle) 
{
    SQLWCHAR sqlstate[6];
    SQLWCHAR errmsg[SQL_MAX_MESSAGE_LENGTH];
    SQLINTEGER nativeerr;
    SQLSMALLINT errmsglen;

    SQLGetDiagRec(SQL_HANDLE_STMT, handle, 1, sqlstate, &nativeerr, errmsg, sizeof(errmsg), &errmsglen);
    printf("Error: %s (%s)\n", errmsg, sqlstate);
}

SQLHSTMT Database::DirectQuery(const std::wstring& query) 
{
    SQLHSTMT hStmt;
    SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &hStmt);
    SQLRETURN ret = SQLExecDirect(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        ErrorHandle(ret, hStmt);
    }
    return hStmt;
}

SQLHSTMT Database::PrepareQuery(const std::wstring& query) 
{
    SQLHSTMT hStmt;
    SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &hStmt);
    SQLPrepare(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);
    return hStmt;
}

bool Database::Init(
    const std::string server_host,
    const std::string db_name)
{

    SQLWCHAR outstr[1024];
    SQLSMALLINT outstrlen;
     
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv); 
    SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0); 
    SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc);

    SQLSetConnectAttr(m_hDbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
    auto ret = SQLDriverConnect(m_hDbc, NULL,
        (SQLWCHAR*)L"DRIVER={ODBC Driver 17 for SQL Server};"
        L"SERVER=localhost;"
        L"DATABASE=dnevnik;"
        L"Trusted_Connection=Yes;",
        SQL_NTS, outstr, sizeof(outstr), &outstrlen, SQL_DRIVER_COMPLETE);
    return true;
}

void Database::DropTable(const std::wstring& name)
{
    auto query = std::wstring(L"DROP TABLE IF EXISTS ");
    query += name;
    query += L";";
    DirectQuery((SQLWCHAR*)query.c_str());
}

void Database::SetupTable(
    const std::wstring& name,
    const std::vector<std::wstring>& rows)
{
    //DropTable(name);
    auto query = std::wstring(L" CREATE TABLE ");
    query += name;
    query += L" (";
    for (auto row : rows) {
        query += row;
    }
    query += L");";
    Exec(query.c_str());
}

void Database::InsertRow(
    const std::wstring& name,
    const std::vector<std::wstring>& columns,
    const std::vector<std::wstring>& values) 
{
    auto query = std::wstring(L"INSERT INTO ");
    query += name;
    query += L" (";
    for (auto column : columns) {
        query += column;
    }
    query += L")";
    query += L" VALUES (";
    for (auto value : values) {
        query += value;
    }
    query += L");";
    Exec(query.c_str());
}

void Database::Exec(const wchar_t* query)
{
    SQLHSTMT hStmt = DirectQuery((SQLWCHAR*)query);
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return;
}

bool Database::Free() 
{
    SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
    SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
    return true;
}

SQLHSTMT Database::HandleExec(const wchar_t* query)
{
    SQLHSTMT hStmt = DirectQuery((SQLWCHAR*)query);
    return hStmt;
}

SQLHSTMT Database::HandlePrepare(const wchar_t* query)
{
    SQLHSTMT hStmt = PrepareQuery((SQLWCHAR*)query);
    return hStmt;
}