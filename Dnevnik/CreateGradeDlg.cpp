// CreateGradeDlg.cpp : implementation file
//

#include "pch.h"
#include "Dnevnik.h"
#include "CreateGradeDlg.h"
#include "afxdialogex.h"
#include "DatabaseSets.h"


// CreateGradeDlg dialog

IMPLEMENT_DYNAMIC(CreateGradeDlg, CDialogEx)

CreateGradeDlg::CreateGradeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CreateGrade, pParent)
	, m_tutor(_T(""))
	, m_subject(_T(""))
	, m_student(_T(""))
	, m_date(_T(""))
	, m_grade(_T(""))
{

}

CreateGradeDlg::~CreateGradeDlg()
{
}

void CreateGradeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_tutor);
	DDX_Control(pDX, IDC_COMBO1, m_subjectBox);
	DDX_CBString(pDX, IDC_COMBO1, m_subject);
	DDX_Control(pDX, IDC_COMBO2, m_studentBox);
	DDX_CBString(pDX, IDC_COMBO2, m_student);
	DDX_Text(pDX, IDC_EDIT2, m_date);
	DDX_Text(pDX, IDC_EDIT4, m_grade);
}


BEGIN_MESSAGE_MAP(CreateGradeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CreateGradeDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CreateGradeDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();  

	auto students = database::StudentDatabase::GetStudentSet();
	
	students.NextRow();
	while (students.IsAvailable()) {
		m_students.push_back(students.GetRow());
		students.NextRow();
	}

	auto subjects = database::SubjectDatabase::GetSubjectSet();
	subjects.NextRow();
	while (subjects.IsAvailable()) {
		m_subjects.push_back(subjects.GetRow());
		subjects.NextRow();
	}

	for (const auto& student : m_students) {
		m_studentBox.AddString(student.name.c_str());
	}

	for (const auto& subject : m_subjects) {
		m_subjectBox.AddString(subject.name.c_str());
	}

	UpdateData(false);
	return true;
}

void CreateGradeDlg::OnBnClickedOk()
{
	UpdateData(true);
	
	int student_index = m_studentBox.GetCurSel();
	int subject_index = m_subjectBox.GetCurSel(); 
	if (student_index == -1 || 
		subject_index == -1 || 
		std::stof(m_grade.GetString()) < 2 ||
		std::stof(m_grade.GetString()) > 6) {
		CDialogEx::OnOK();
		return;
	}

	database::GradeDatabase::InsertRow(
		m_students[student_index].id,
		m_subjects[subject_index].name,
		m_date.GetString(),
		std::stof(m_grade.GetString()));

	CDialogEx::OnOK();
}
