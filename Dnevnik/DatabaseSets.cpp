#include "pch.h"
#include "DatabaseSets.h"
 
using namespace database;
#pragma region student
StudentSet::StudentSet(const SQLHSTMT& handle) : SelectQuery<Student>(handle) {};

Student StudentSet::GetRow() {
    auto id = this->GetData<SQLGUID>(1, SQL_C_GUID);
    auto class_number = this->GetData<int>(2, SQL_C_LONG);
    auto name = this->GetData<std::wstring>(3, SQL_C_CHAR, 256);
    auto date = this->GetData<SQL_DATE_STRUCT>(4, SQL_C_TYPE_DATE);
    return Student{ id, class_number, name, date };
}

StudentDatabase StudentDatabase::SetupTable() {
    auto ss = StudentDatabase(); 
    Database::Get().SetupTable(L"students", std::vector<std::wstring>{
        L"id uniqueidentifier PRIMARY KEY, ",
            L"class_number INT, ",
            L"name VARCHAR(255), ",
            L"date_birth DATE"
    }); 
    return ss;
}

StudentSet StudentDatabase::GetStudentSet(const wchar_t* query) {
    SQLHSTMT hStmt = Database::Get().HandleExec((SQLWCHAR*)query);
    return StudentSet(hStmt);
}

void StudentDatabase::DropTable() {
    Database::Get().DropTable(L"students");
}

void StudentDatabase::InsertRow(const int& class_number, const std::wstring& name, const std::wstring& date) {
    SQLHSTMT handle = Database::Get().HandlePrepare(
        L"INSERT INTO students"
        "(id, class_number, name, date_birth)"
        "VALUES(NEWID(), ?, ?, CAST(? AS DATE))");

    SQLBindParameter(handle, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, (SQLPOINTER)&class_number, 0, NULL);
    SQLBindParameter(handle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)name.c_str(), name.size() * sizeof(wchar_t), NULL);
    SQLBindParameter(handle, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_VARCHAR, date.size(), 0, (SQLPOINTER)date.c_str(), date.size() * sizeof(wchar_t), NULL);
    auto ret = SQLExecute(handle);
    if (ret < 0) {
        ErrorHandle(ret, handle);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, handle);
}

void StudentDatabase::DeleteRow(const SQLGUID& guid) {
    SQLHSTMT handle = Database::Get().HandlePrepare(L"DELETE FROM students WHERE id = ?");
    SQLBindParameter(handle, 1, SQL_PARAM_INPUT, SQL_C_GUID, SQL_GUID, 0, 0, (SQLPOINTER)&guid, 0, NULL);
    auto ret = SQLExecute(handle);
    if (ret < 0) {
        ErrorHandle(ret, handle);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, handle);
}

void StudentDatabase::EditRow(const Student& student) {
    SQLHSTMT handle = Database::Get().HandlePrepare(L"UPDATE students SET id = ?, class_number = ?, name = ?, date_birth = ? WHERE id = ?");
    SQLBindParameter(handle, 1, SQL_PARAM_INPUT, SQL_C_GUID, SQL_GUID, 0, 0, (SQLPOINTER)&student.id, 0, NULL);
    SQLBindParameter(handle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, (SQLPOINTER)&student.class_number, 0, NULL);
    SQLBindParameter(handle, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 255, 0, (SQLPOINTER)student.name.c_str(), student.name.size() * sizeof(wchar_t), NULL);
    SQLBindParameter(handle, 4, SQL_PARAM_INPUT, SQL_C_TYPE_DATE, SQL_DATE, 0, 0, (SQLPOINTER)&student.date, 0, NULL);
    SQLBindParameter(handle, 5, SQL_PARAM_INPUT, SQL_C_GUID, SQL_GUID, 0, 0, (SQLPOINTER)&student.id, 0, NULL);
    auto ret = SQLExecute(handle);
    if (ret < 0) {
        ErrorHandle(ret, handle);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, handle);
}

std::vector<Student> StudentDatabase::GetAllStudentsBirthdayToday() {
    SQLHSTMT handle = Database::Get().HandlePrepare(L"SELECT * FROM students WHERE date_birth = CAST(? AS DATE)");
    //chat gpt
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm;
    localtime_s(&now_tm, &now_time_t);
    std::wstringstream wss;
    wss << std::put_time(&now_tm, L"%Y-%m-%d");
    std::wstring time_formatted = wss.str();
    //------
    SQLBindParameter(handle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_CHAR, time_formatted.size(), 0, (SQLPOINTER)time_formatted.c_str(), time_formatted.size() * sizeof(wchar_t), NULL);
    auto ret = SQLExecute(handle);
    if (ret < 0) {
        ErrorHandle(ret, handle);
    }
    auto select_query = StudentSet(handle);
    std::vector<Student> list;
    select_query.NextRow();
    while (select_query.IsAvailable()) {
        list.push_back(select_query.GetRow());
        select_query.NextRow();
    }
    SQLFreeHandle(SQL_HANDLE_STMT, handle);
    return list;

}
#pragma endregion

#pragma region subject
SubjectSet::SubjectSet(const SQLHSTMT& handle) : SelectQuery(handle) {};

Subject SubjectSet::GetRow() {
    auto id = this->GetData<SQLGUID>(1, SQL_C_GUID);
    auto name = this->GetData<std::wstring>(2, SQL_C_CHAR, 256);
    auto tutor = this->GetData<std::wstring>(3, SQL_C_CHAR, 256);
    auto room_number = this->GetData<int>(4, SQL_C_LONG);
    return Subject{ id, name, tutor, room_number };
}

SubjectDatabase SubjectDatabase::SetupTable() {
    auto sb = SubjectDatabase(); 
    Database::Get().SetupTable(L"subjects", std::vector<std::wstring>{
        L"id uniqueidentifier PRIMARY KEY, "
            L"name VARCHAR(255), ",
            L"tutor VARCHAR(255), ",
            L"room_number INT"
    });
    return sb;
}

SubjectSet SubjectDatabase::GetSubjectSet(const wchar_t* query) {
    SQLHSTMT hStmt = Database::Get().HandleExec((SQLWCHAR*)query);
    return SubjectSet(hStmt);
}

void SubjectDatabase::DropTable() {
    Database::Get().DropTable(L"subjects");
}

void SubjectDatabase::InsertRow(
    const std::wstring& name,
    const std::wstring& tutor,
    const int& room_number) {
    SQLHSTMT handle = Database::Get().HandlePrepare(
        L"INSERT INTO subjects"
        "(id, name, tutor, room_number)"
        "VALUES(NEWID(), ?, ?, ?)");

    SQLBindParameter(handle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)name.c_str(), name.size() * sizeof(wchar_t), NULL);
    SQLBindParameter(handle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)tutor.c_str(), tutor.size() * sizeof(wchar_t), NULL);
    SQLBindParameter(handle, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, (SQLPOINTER)&room_number, 0, NULL);
    auto ret = SQLExecute(handle);
    if (ret < 0) {
        ErrorHandle(ret, handle);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, handle);
}

void SubjectDatabase::DeleteRow(const SQLGUID& guid) {
    SQLHSTMT handle = Database::Get().HandlePrepare(L"DELETE FROM subjects WHERE id = ?");
    SQLBindParameter(handle, 1, SQL_PARAM_INPUT, SQL_C_GUID, SQL_GUID, 0, 0, (SQLPOINTER)&guid, 0, NULL);
    auto ret = SQLExecute(handle);
    if (ret < 0) {
        ErrorHandle(ret, handle);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, handle);
}

void SubjectDatabase::EditRow(const Subject& subject) {
    SQLHSTMT handle = Database::Get().HandlePrepare(L"UPDATE subjects SET id = ?, name = ?, tutor = ?, room_number = ? WHERE id = ?");
    SQLBindParameter(handle, 1, SQL_PARAM_INPUT, SQL_C_GUID, SQL_GUID, 0, 0, (SQLPOINTER)&subject.id, 0, NULL);
    SQLBindParameter(handle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 255, 0, (SQLPOINTER)subject.name.c_str(), subject.name.size() * sizeof(wchar_t), NULL);
    SQLBindParameter(handle, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 255, 0, (SQLPOINTER)subject.tutor.c_str(), subject.tutor.size() * sizeof(wchar_t), NULL);
    SQLBindParameter(handle, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, (SQLPOINTER)&subject.room_number, 0, NULL);
    SQLBindParameter(handle, 5, SQL_PARAM_INPUT, SQL_C_GUID, SQL_GUID, 0, 0, (SQLPOINTER)&subject.id, 0, NULL);
    auto ret = SQLExecute(handle);
    if (ret < 0) {
        ErrorHandle(ret, handle);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, handle);
}
#pragma endregion

#pragma region subject
GradeDatabase GradeDatabase::SetupTable() {
    auto gb = GradeDatabase(); 
    Database::Get().SetupTable(L"grades", std::vector<std::wstring>{
        L"id uniqueidentifier PRIMARY KEY, ",
            L"student_id uniqueidentifier, ",
            L"subject VARCHAR(255), ",
            L"date DATE, "
            L"grade FLOAT"
    });
    return gb;
}

GradeSet GradeDatabase::GetGradeSet(const wchar_t* query) {
    SQLHSTMT hStmt = Database::Get().HandleExec((SQLWCHAR*)query);
    return GradeSet(hStmt);
}

void GradeDatabase::DropTable() {
    Database::Get().DropTable(L"grades");
}

void GradeDatabase::InsertRow(
    const SQLGUID& student_id,
    const std::wstring& subject,
    const std::wstring& date,
    const float& grade) {
    SQLHSTMT handle = Database::Get().HandlePrepare(
        L"INSERT INTO grades"
        "(id, student_id, subject, date, grade)"
        "VALUES(NEWID(), ?, ?, CAST(? AS DATE), ?)");

    SQLBindParameter(handle, 1, SQL_PARAM_INPUT, SQL_C_GUID, SQL_GUID, 0, 0, (SQLPOINTER)&student_id, 0, NULL);
    SQLBindParameter(handle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)subject.c_str(), subject.size() * sizeof(wchar_t), NULL);
    SQLBindParameter(handle, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_VARCHAR, date.size(), 0, (SQLPOINTER)date.c_str(), date.size() * sizeof(wchar_t), NULL);
    SQLBindParameter(handle, 4, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, (SQLPOINTER)&grade, 0, NULL);
    auto ret = SQLExecute(handle);
    if (ret < 0) {
        ErrorHandle(ret, handle);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, handle);
}

void GradeDatabase::DeleteRow(const SQLGUID& guid) {
    SQLHSTMT handle = Database::Get().HandlePrepare(L"DELETE FROM grades WHERE id = ?");
    SQLBindParameter(handle, 1, SQL_PARAM_INPUT, SQL_C_GUID, SQL_GUID, 0, 0, (SQLPOINTER)&guid, 0, NULL);
    auto ret = SQLExecute(handle);
    if (ret < 0) {
        ErrorHandle(ret, handle);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, handle);
}

void GradeDatabase::EditRow(const Grade& grade) {
    SQLHSTMT handle = Database::Get().HandlePrepare(L"UPDATE grades SET id = ?, student_id = ?, subject = ?, date = ?, grade = ? WHERE id = ?");
    SQLBindParameter(handle, 1, SQL_PARAM_INPUT, SQL_C_GUID, SQL_GUID, 0, 0, (SQLPOINTER)&grade.id, 0, NULL);
    SQLBindParameter(handle, 2, SQL_PARAM_INPUT, SQL_C_GUID, SQL_GUID, 0, 0, (SQLPOINTER)&grade.student_id, 0, NULL);
    SQLBindParameter(handle, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 255, 0, (SQLPOINTER)grade.subject.c_str(), grade.subject.size() * sizeof(wchar_t), NULL);
    SQLBindParameter(handle, 4, SQL_PARAM_INPUT, SQL_C_TYPE_DATE, SQL_TYPE_DATE, 0, 0, (SQLPOINTER)&grade.date, 0, NULL);
    SQLBindParameter(handle, 5, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, (SQLPOINTER)&grade.grade, 0, NULL);
    SQLBindParameter(handle, 6, SQL_PARAM_INPUT, SQL_C_GUID, SQL_GUID, 0, 0, (SQLPOINTER)&grade.id, 0, NULL);
    auto ret = SQLExecute(handle);
    if (ret < 0) {
        ErrorHandle(ret, handle);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, handle);
}

float GradeDatabase::AverageGrade(const SQLGUID& student_id) {
    SQLHSTMT handle = Database::Get().HandlePrepare(L"SELECT AVG(grade) AS average_grade FROM grades WHERE student_id = ?");
    SQLBindParameter(handle, 1, SQL_PARAM_INPUT, SQL_C_GUID, SQL_GUID, 0, 0, (SQLPOINTER)&student_id, 0, NULL);
    auto ret = SQLExecute(handle);
    if (ret < 0) {
        ErrorHandle(ret, handle);
    }
    float grade;
    //@todo why?
    SQLLEN indicator;
    SQLBindCol(handle, 1, SQL_C_FLOAT, &grade, sizeof(grade), &indicator);
    ret = SQLFetch(handle);
    if (ret < 0) {
        ErrorHandle(ret, handle);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, handle);
    return grade;
}

float GradeDatabase::AverageGrade(
    const SQLGUID& student_id,
    const std::wstring& subject) {
    SQLHSTMT handle = Database::Get().HandlePrepare(L"SELECT AVG(grade) AS average_grade FROM grades WHERE student_id = ? AND subject = ?");
    SQLBindParameter(handle, 1, SQL_PARAM_INPUT, SQL_C_GUID, SQL_GUID, 0, 0, (SQLPOINTER)&student_id, 0, NULL);
    SQLBindParameter(handle, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 255, 0, (SQLPOINTER)subject.c_str(), subject.size() * sizeof(wchar_t), NULL);
    auto ret = SQLExecute(handle);
    if (ret < 0) {
        ErrorHandle(ret, handle);
    }
    float grade = 0.f;
    //@todo why?
    SQLLEN indicator;
    SQLBindCol(handle, 1, SQL_C_FLOAT, &grade, sizeof(grade), &indicator);
    SQLFetch(handle);
    if (ret < 0) {
        ErrorHandle(ret, handle);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, handle);
    return grade;
}

std::vector<SQLGUID> GradeDatabase::GetAllStudentsGradeBelow(const float& grade) {
    SQLHSTMT handle = Database::Get().HandlePrepare(L"SELECT student_id FROM grades WHERE grade < ? GROUP BY student_id");
    SQLBindParameter(handle, 1, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, (SQLPOINTER)&grade, 0, NULL);
    auto ret = SQLExecute(handle);
    if (ret < 0) {
        ErrorHandle(ret, handle);
    }
    auto select_query = SelectQuery<SQLGUID>(handle);
    std::vector<SQLGUID> list;
    select_query.NextRow();
    while (select_query.IsAvailable()) {
        list.push_back(select_query.GetData<SQLGUID>(1, SQL_C_GUID));
        select_query.NextRow();
    }
    SQLFreeHandle(SQL_HANDLE_STMT, handle);
    return list;
}

std::vector<SQLGUID> GradeDatabase::GetAllStudentsGradeBelow(
    const float& grade,
    const std::wstring& subject) {
    SQLHSTMT handle = Database::Get().HandlePrepare(L"SELECT student_id FROM grades WHERE subject = ? GROUP BY student_id HAVING AVG(grade) < ?");
    SQLBindParameter(handle, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 255, 0, (SQLPOINTER)subject.c_str(), subject.size() * sizeof(wchar_t), NULL);
    SQLBindParameter(handle, 2, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, (SQLPOINTER)&grade, 0, NULL);
    auto ret = SQLExecute(handle);
    if (ret < 0) {
        ErrorHandle(ret, handle);
    }
    auto select_query = SelectQuery<SQLGUID>(handle);
    std::vector<SQLGUID> list;
    select_query.NextRow();
    while (select_query.IsAvailable()) {
        list.push_back(select_query.GetData<SQLGUID>(1, SQL_C_GUID));
        select_query.NextRow();
    }
    SQLFreeHandle(SQL_HANDLE_STMT, handle);
    return list;
}

std::vector<SQLGUID> GradeDatabase::GetAllStudentsGradeBelow(
    const float& grade,
    const int& count) {
    SQLHSTMT handle = Database::Get().HandlePrepare(
        L"WITH SubjectAverages AS ("
        L"SELECT student_id, subject, AVG(grade) AS average_grade "
        L"FROM grades "
        L"GROUP BY student_id, subject "
        L"HAVING AVG(grade) < ?) "
        L"SELECT student_id "
        L"FROM SubjectAverages "
        L"GROUP BY student_id "
        L"HAVING COUNT(subject) >= ?; ");
    SQLBindParameter(handle, 1, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, (SQLPOINTER)&grade, 0, NULL);
    SQLBindParameter(handle, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, (SQLPOINTER)&count, 0, NULL);
    auto ret = SQLExecute(handle);
    if (ret < 0) {
        ErrorHandle(ret, handle);
    }
    auto select_query = SelectQuery<SQLGUID>(handle);
    std::vector<SQLGUID> list;
    select_query.NextRow();
    while (select_query.IsAvailable()) {
        list.push_back(select_query.GetData<SQLGUID>(1, SQL_C_GUID));
        select_query.NextRow();
    }
    SQLFreeHandle(SQL_HANDLE_STMT, handle);
    return list;
}

