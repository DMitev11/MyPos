#pragma once


// StudentBirthdaysDlg dialog

class StudentBirthdaysDlg : public CDialogEx
{
	DECLARE_DYNAMIC(StudentBirthdaysDlg)

public:
	StudentBirthdaysDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~StudentBirthdaysDlg();

	CListCtrl m_list; 
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_StudentBirthdays };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()  
};
