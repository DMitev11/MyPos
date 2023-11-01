// StudentBirthdaysDlg.cpp : implementation file
//

#include "pch.h"
#include "Dnevnik.h"
#include "StudentBirthdaysDlg.h"
#include "afxdialogex.h"
#include "DatabaseSets.h"


// StudentBirthdaysDlg dialog

IMPLEMENT_DYNAMIC(StudentBirthdaysDlg, CDialogEx)

StudentBirthdaysDlg::StudentBirthdaysDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_StudentsBirthdays, pParent)
{

}

StudentBirthdaysDlg::~StudentBirthdaysDlg()
{
}

void StudentBirthdaysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_list);
}

BEGIN_MESSAGE_MAP(StudentBirthdaysDlg, CDialogEx) 
END_MESSAGE_MAP()

BOOL StudentBirthdaysDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	auto students = database::StudentDatabase::GetAllStudentsBirthdayToday(); 
	
	m_list.InsertColumn(0, L"id", 0, 0, 0);
	m_list.InsertColumn(1, L"class", 0, 100, 0);
	m_list.InsertColumn(2, L"name", 0, 250, 0);
	m_list.InsertColumn(3, L"date", 0, 100, 0);

	for (int i = 0; i < students.size(); i++) {
		m_list.InsertItem(i, std::to_wstring(students[i].id.Data1).c_str());
		m_list.SetItemText(i, 0, std::to_wstring(students[i].id.Data1).c_str());
		m_list.SetItemText(i, 1, std::to_wstring(students[i].class_number).c_str());
		m_list.SetItemText(i, 2, students[i].name.c_str());
		m_list.SetItemText(i, 3, 
			(std::to_wstring(students[i].date.year) + 
			L"-" + 
			std::to_wstring(students[i].date.month) + 
			L"-" +
			std::to_wstring(students[i].date.day)).c_str());
	}

	UpdateData(false);
	return true;
}
