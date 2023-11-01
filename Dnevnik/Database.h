#pragma once
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

namespace database { 
    void ErrorHandle(SQLRETURN res, SQLHSTMT handle);

    class HQuery {
    protected:
        SQLHSTMT m_handle;
        HQuery(const SQLHSTMT& handle) : m_handle(handle) {};
    
    public:
        HQuery() = delete;
        ~HQuery() {
            SQLFreeHandle(SQL_HANDLE_STMT, m_handle);
        }
    };

    template<typename U>
    class SelectQuery: public HQuery { 
    private:
        bool isAvailable = true;
    
    public: 
        SelectQuery(const SQLHSTMT& handle) : HQuery(handle) {};
        template<typename T>
        T GetData(
            const int& column,
            const int& sql_type,
            const std::size_t& size = -1) {
            T ret = T();
            auto res = SQLGetData(m_handle, column, sql_type, &ret, size != -1 ? size : sizeof(ret), NULL);
            if (res != SQL_SUCCESS && res != SQL_SUCCESS_WITH_INFO) {
                ErrorHandle(res, m_handle);
                isAvailable = false;
            }

            return ret;
        }

        template<>
        std::wstring GetData(
            const int& column,
            const int& sql_type,
            const std::size_t& size) {
            std::wstring ret = std::wstring();
            char varchar[256];
            SQLLEN indicator;
            auto res = SQLGetData(m_handle, column, sql_type, &varchar, 256, &indicator);
            if (res != SQL_SUCCESS && res != SQL_SUCCESS_WITH_INFO) {
                ErrorHandle(res, m_handle);
                SQLFreeHandle(SQL_HANDLE_STMT, m_handle);
                isAvailable = false;
            }

            for (int i = 0; i < (int)indicator; i++) {
                ret += varchar[i];
            }
            return ret;
        } 

        void NextRow() {
            auto res = SQLFetch(m_handle);
            if (res == SQL_SUCCESS) { return; }
            else {
                ErrorHandle(res, m_handle);
                SQLFreeHandle(SQL_HANDLE_STMT, m_handle);
                isAvailable = false;
                return;
            }            
            return;

        }

        void PreviousRow() {
            auto res = SQLFetchScroll(m_handle, SQL_FETCH_PRIOR, 0);;
            if (res == SQL_SUCCESS) { return; }
            else {
                std::cout << res;
            }
            return;
        };

        bool IsAvailable() const {
            return isAvailable;
        }
    }; 

    class Database {
    private:
        SQLHENV m_hEnv = SQLHENV();
        SQLHDBC m_hDbc = SQLHDBC();

        SQLHSTMT DirectQuery(const std::wstring& query); 
        SQLHSTMT PrepareQuery(const std::wstring& query);

        Database() {};
        Database(const Database& other) {};
        void operator=(const Database& other) {};
        ~Database() {};

        static Database* instance;

    public:
        static Database& Get() {
            if (instance == nullptr) {
                instance = new Database();
            }

            return *instance;
        }

        bool Init(
            const std::string server_host = "localhost",
            const std::string db_name = "master");

        void DropTable(const std::wstring& name);
        void SetupTable(
            const std::wstring& name,
            const std::vector<std::wstring>& rows);

        void InsertRow(
            const std::wstring& name,
            const std::vector<std::wstring>& columns,
            const std::vector<std::wstring>& values);
             
        void Exec(const wchar_t* query); 
        bool Free();
        SQLHSTMT HandleExec(const wchar_t* query);
        SQLHSTMT HandlePrepare(const wchar_t* query);
    };   
 }
