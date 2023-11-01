// CreateStudentDlg.cpp : implementation file
//

#include "pch.h"
#include "Dnevnik.h"
#include "CreateStudentDlg.h"
#include "afxdialogex.h"
#include "DatabaseSets.h"


// CreateStudentDlg dialog

IMPLEMENT_DYNAMIC(CreateStudentDlg, CDialogEx)

CreateStudentDlg::CreateStudentDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CreateStudent, pParent) 
	, m_name(_T("")) 
	, m_number(_T(""))
	, m_birthDate(_T(""))
{
   
}

CreateStudentDlg::~CreateStudentDlg()
{
}

void CreateStudentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX); 
	DDX_Text(pDX, IDC_EDIT1, m_number);
	DDX_Text(pDX, IDC_EDIT2, m_name);
	DDX_Text(pDX, IDC_EDIT3, m_birthDate);
}


BEGIN_MESSAGE_MAP(CreateStudentDlg, CDialogEx)  
	ON_BN_CLICKED(IDOK, &CreateStudentDlg::OnBnClickedOk)   
END_MESSAGE_MAP()


// CreateStudentDlg message handlers

 


void CreateStudentDlg::OnBnClickedOk()
{
	UpdateData(true); // 
	// TODO: Add your control notification handler code here
	if (m_number.IsEmpty() || 
		m_name.IsEmpty() ||
		m_birthDate.IsEmpty()) return;

	database::StudentDatabase::InsertRow(std::stoi(m_number.GetString()), m_name.GetString(), m_birthDate.GetString());
	CDialogEx::OnOK();
} 