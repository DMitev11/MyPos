// StudentsOverviewDlg.cpp : implementation file
//

#include "pch.h"
#include "Dnevnik.h"
#include "StudentsOverviewDlg.h"
#include "afxdialogex.h"

// StudentsOverviewDlg dialog

IMPLEMENT_DYNAMIC(StudentsOverviewDlg, CDialogEx)

StudentsOverviewDlg::StudentsOverviewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_StudentsOverview, pParent)
{

}

StudentsOverviewDlg::~StudentsOverviewDlg()
{
}

void StudentsOverviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(StudentsOverviewDlg, CDialogEx)
END_MESSAGE_MAP()

// DDX/DDV support
BOOL StudentsOverviewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	std::vector<database::Student> students;

	auto students_set = database::StudentDatabase::GetStudentSet();
	students_set.NextRow();
	while (students_set.IsAvailable())
	{
		students.push_back(students_set.GetRow());
		students_set.NextRow();
	}

	m_list.InsertColumn(0, L"class number", 0, 60, 0);
	m_list.InsertColumn(1, L"name", 0, 200, 1);
	m_list.InsertColumn(2, L"date", 0, 100, 2);
	
	for (int i = 0; i < m_students.size(); i++) {
		const auto& student = m_students[i];
		auto student_data = std::find_if(students.begin(), students.end(), [&](const database::Student& lhs) {
			return	lhs.id == student;
			});
		if (student_data == students.end()) 
		{
			continue; 
		}
		auto [id, class_number, name, date] = (*student_data).data();
		m_list.InsertItem(i, id.c_str());
		m_list.SetItemText(i, 0, class_number.c_str());
		m_list.SetItemText(i, 1, name.c_str());
		m_list.SetItemText(i, 2, date.c_str());
	}

	return true;
}
// StudentsOverviewDlg message handlers
