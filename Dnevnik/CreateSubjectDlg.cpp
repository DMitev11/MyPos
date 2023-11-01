// CreateSubjectDlg.cpp : implementation file
//

#include "pch.h"
#include "Dnevnik.h"
#include "CreateSubjectDlg.h"
#include "afxdialogex.h"
#include "DatabaseSets.h"


// CreateSubjectDlg dialog

IMPLEMENT_DYNAMIC(CreateSubjectDlg, CDialogEx)

CreateSubjectDlg::CreateSubjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CreateSubject, pParent)
	, m_tutor(_T(""))
	, m_name(_T(""))
	, m_room(_T(""))
{

}

CreateSubjectDlg::~CreateSubjectDlg()
{
}

void CreateSubjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_tutor);
	DDX_Text(pDX, IDC_EDIT2, m_name);
	DDX_Text(pDX, IDC_EDIT3, m_room);
}


BEGIN_MESSAGE_MAP(CreateSubjectDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CreateSubjectDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CreateSubjectDlg message handlers


void CreateSubjectDlg::OnBnClickedOk()
{
	UpdateData(true); 
	if (m_name.IsEmpty() ||
		m_tutor.IsEmpty() ||
		m_room.IsEmpty()) return;

	database::SubjectDatabase::InsertRow(m_name.GetString(), m_tutor.GetString(), std::stoi(m_room.GetString()));
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
