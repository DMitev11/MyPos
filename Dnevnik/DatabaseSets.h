#pragma once 
#include <chrono>
#include <iomanip>
#include <sstream>
#include <locale>
#include <tuple> 

#include "Database.h" 
namespace database {
#pragma region student
    struct Student {
        SQLGUID id;
        int class_number;
        std::wstring name;
        SQL_DATE_STRUCT date;

        std::tuple<std::wstring, std::wstring, std::wstring, std::wstring> data() const {
            return std::tuple<std::wstring, std::wstring, std::wstring, std::wstring>{
                std::to_wstring(id.Data1),
                std::to_wstring(class_number),
                name,
                std::to_wstring(date.year) + 
                    L"-" + 
                    std::to_wstring(date.month) + 
                    L"-" + 
                    std::to_wstring(date.day)
            };
        }
    };
     
    class StudentSet: public SelectQuery<Student> { 
    public:
        StudentSet() = delete;
        StudentSet(const SQLHSTMT& handle);
        Student GetRow();
    }; 

    class StudentDatabase {
    private: 
        StudentDatabase() {};

    public:
        ~StudentDatabase() {}; 
        static void DropTable();
        static void InsertRow(const int& class_number, const std::wstring& name, const std::wstring& date);
        static void DeleteRow(const SQLGUID& guid);
        static void EditRow(const Student& student);
        static StudentSet GetStudentSet(const wchar_t* query = L"SELECT * FROM students");
        static std::vector<Student> GetAllStudentsBirthdayToday(); 
        static StudentDatabase SetupTable();
    };
#pragma endregion

#pragma region subject
    struct Subject {
        SQLGUID id;
        std::wstring name;
        std::wstring tutor;
        int room_number;

        std::tuple<std::wstring, std::wstring, std::wstring, std::wstring> data() const {
            return std::tuple<std::wstring, std::wstring, std::wstring, std::wstring>{
                std::to_wstring(id.Data1),
                    name, 
                    tutor, 
                    std::to_wstring(room_number)
            };
        }
    };

    class SubjectSet : public SelectQuery<Subject> {
    public:
        SubjectSet() = delete;
        SubjectSet(const SQLHSTMT& handle);
        Subject GetRow();
    };

    class SubjectDatabase {
    private: 
        SubjectDatabase() {};

    public: 
        ~SubjectDatabase() {};
        static SubjectSet GetSubjectSet(const wchar_t* query = L"SELECT * FROM subjects");
        static void DropTable();
        static void InsertRow(const std::wstring& name, const std::wstring& tutor, const int& room_number);
        static void DeleteRow(const SQLGUID& guid);
        static void EditRow(const Subject& subject); 
        static SubjectDatabase SetupTable();
    };

    struct Grade {
        SQLGUID id;
        SQLGUID student_id;
        std::wstring subject;
        SQL_DATE_STRUCT date;
        float grade;

        std::tuple<std::wstring, std::wstring, std::wstring, std::wstring, std::wstring> data() const {
            return std::tuple<std::wstring, std::wstring, std::wstring, std::wstring, std::wstring>{
                    std::to_wstring(id.Data1),
                    std::to_wstring(student_id.Data1),
                    subject,
                    std::to_wstring(date.day),
                    std::to_wstring(grade)
            };
        }
    };
#pragma endregion

    class GradeSet : public SelectQuery<Grade> {
    public: 
        GradeSet() = delete;
        GradeSet(const SQLHSTMT& handle) : SelectQuery(handle) {}; 
        Grade GetRow() {
            auto id = this->GetData<SQLGUID>(1, SQL_C_GUID); 
            auto student_id = this->GetData<SQLGUID>(2, SQL_C_GUID);
            auto subject = this->GetData<std::wstring>(3, SQL_C_CHAR, 256);            
            auto date = this->GetData<SQL_DATE_STRUCT>(4, SQL_C_TYPE_DATE);
            auto grade = this->GetData<float>(5, SQL_C_FLOAT);
            return Grade{ id, student_id, subject, date, grade };
        } 
    }; 

    struct StudentGrade {
        SQLGUID student_id;
        float grade;
    };

    class StudentGradeSet : public SelectQuery<StudentGrade> {
    public:
        StudentGradeSet() = delete;
        StudentGradeSet(const SQLHSTMT& handle) : SelectQuery(handle) {};
        StudentGrade GetRow() {
            auto student_id = this->GetData<SQLGUID>(1, SQL_C_GUID);
            auto grade = this->GetData<float>(2, SQL_C_FLOAT);
            return StudentGrade{ student_id, grade };
        } 
    };

#pragma region grade
    class GradeDatabase {
    private: 
        GradeDatabase(){};

    public: 
        ~GradeDatabase() {}; 
        static void DropTable();
        static void InsertRow(
            const SQLGUID& student_id,
            const std::wstring& subject,
            const std::wstring& date,
            const float& grade);

        static void DeleteRow(const SQLGUID& guid);
        static void EditRow(const Grade& grade);
        static GradeSet GetGradeSet(const wchar_t* query = L"SELECT * FROM grades");
        static float AverageGrade(const SQLGUID& student_id);
        static float AverageGrade(
            const SQLGUID& student_id,
            const std::wstring& subject);

        static std::vector<SQLGUID> GetAllStudentsGradeBelow(const float& grade);
        static std::vector<SQLGUID> GetAllStudentsGradeBelow(
            const float& grade,
            const std::wstring& subject);
        static std::vector<SQLGUID> GetAllStudentsGradeBelow(
            const float& grade, 
            const int& count);

        static GradeDatabase SetupTable();
    };
#pragma endregion
}