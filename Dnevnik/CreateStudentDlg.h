#pragma once


// CreateStudentDlg dialog

class CreateStudentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CreateStudentDlg)

public:
	CreateStudentDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CreateStudentDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CreateStudent };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()    
public: 
	afx_msg void OnBnClickedOk();    
private:
	CString m_number;
	CString m_name;
	CString m_birthDate;
};
