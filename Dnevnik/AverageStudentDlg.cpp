// AverageStudentDlg.cpp : implementation file
//

#include "pch.h"
#include "Dnevnik.h"
#include "AverageStudentDlg.h"
#include "afxdialogex.h"


// AverageStudentDlg dialog

IMPLEMENT_DYNAMIC(AverageStudentDlg, CDialogEx)

AverageStudentDlg::AverageStudentDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AverageStudent, pParent)
{

}

AverageStudentDlg::~AverageStudentDlg()
{
}

void AverageStudentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_studentBox);
	DDX_Control(pDX, IDC_COMBO1, m_subjectBox);
	DDX_Control(pDX, IDC_EDIT1, m_resultBox);
	DDX_Control(pDX, IDC_BUTTON1, m_submitBtn);
}


BEGIN_MESSAGE_MAP(AverageStudentDlg, CDialogEx) 
	ON_BN_CLICKED(IDC_BUTTON1, &AverageStudentDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL AverageStudentDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	auto students = database::StudentDatabase::GetStudentSet();
	students.NextRow();
	while (students.IsAvailable()) 
	{
		m_students.push_back(students.GetRow());
		students.NextRow();
	}

	auto subjects = database::SubjectDatabase::GetSubjectSet();
	subjects.NextRow();
	while (subjects.IsAvailable()) 
	{
		m_subjects.push_back(subjects.GetRow());
		subjects.NextRow();
	}

	for (const auto& student : m_students) 
	{
		m_studentBox.AddString(student.name.c_str());
	}

	for (const auto& subject : m_subjects) 
	{
		m_subjectBox.AddString(subject.name.c_str());
	} 

	UpdateData(false);
	return true;
}


void AverageStudentDlg::OnBnClickedButton1()
{
	UpdateData(false);

	int student_index = m_studentBox.GetCurSel(); 
	if (student_index == -1) return;
	auto student = m_students[student_index];
	
	int subject_index = m_subjectBox.GetCurSel();

	if (subject_index == -1) 
	{
		auto result = database::GradeDatabase::AverageGrade(student.id);
		m_resultBox.SetWindowTextW(std::to_wstring(result).c_str()); 
	}
	else
	{
		auto subject = m_subjects[subject_index];
		auto result = database::GradeDatabase::AverageGrade(student.id, subject.name.c_str());
		m_resultBox.SetWindowTextW(std::to_wstring(result).c_str()); 
	}

	UpdateData(false);
	return;
}
