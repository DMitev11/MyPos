// AverageBelowDlg.cpp : implementation file
//

#include "pch.h"
#include "Dnevnik.h"
#include "AverageBelowDlg.h"
#include "afxdialogex.h" 
#include "StudentsOverviewDlg.h"


// AverageBelowDlg dialog

IMPLEMENT_DYNAMIC(AverageBelowDlg, CDialogEx)

AverageBelowDlg::AverageBelowDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AverageBelow, pParent)
	, m_grade(_T(""))
	, m_count(_T(""))
{

}

AverageBelowDlg::~AverageBelowDlg()
{
}

void AverageBelowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_grade);
	DDX_Control(pDX, IDC_COMBO1, m_subjectsBox);
	DDX_Text(pDX, IDC_EDIT2, m_count);
	DDX_Control(pDX, IDC_RADIO1, m_radio_subject);
	DDX_Control(pDX, IDC_RADIO2, m_radio_count);
}


BEGIN_MESSAGE_MAP(AverageBelowDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &AverageBelowDlg::onButtonRadioSubject)
	ON_BN_CLICKED(IDOK, &AverageBelowDlg::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL AverageBelowDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	auto subjects = database::SubjectDatabase::GetSubjectSet();
	subjects.NextRow();
	while (subjects.IsAvailable()) {
		m_subjects.push_back(subjects.GetRow());
		subjects.NextRow();
	}

	for (const auto& subject : m_subjects) {
		m_subjectsBox.AddString(subject.name.c_str());
	}

	return true;
}

void AverageBelowDlg::onButtonRadioSubject()
{
	UpdateData(false);
	m_radio_count.SetCheck(false);
	UpdateData(false);
}

void AverageBelowDlg::onButtonRadioCount()
{
	UpdateData(false);
	m_radio_subject.SetCheck(false);
	UpdateData(false);
}


void AverageBelowDlg::OnBnClickedOk()
{ 
	UpdateData(true);

	if (m_grade.IsEmpty()) return;
	auto grade = std::stof(m_grade.GetString());
	
	std::vector<SQLGUID> students;
	if (m_radio_subject.GetCheck()) 
	{
		auto index = m_subjectsBox.GetCurSel();
		if (index == -1) return;
		auto subject = m_subjects[index];
		students = database::GradeDatabase::GetAllStudentsGradeBelow(grade, subject.name.c_str());
	}
	else if (m_radio_count.GetCheck())
	{ 
		if (m_count.IsEmpty()) return;
		auto count = std::stoi(m_count.GetString());
		students = database::GradeDatabase::GetAllStudentsGradeBelow(grade, count);
	}
	else
	{
		students = database::GradeDatabase::GetAllStudentsGradeBelow(grade);
	}

	StudentsOverviewDlg dlg;
	dlg.m_students = students;
	dlg.DoModal();
	
	UpdateData(true);
}
