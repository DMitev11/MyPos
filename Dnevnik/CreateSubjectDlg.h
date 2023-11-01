#pragma once


// CreateSubjectDlg dialog

class CreateSubjectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CreateSubjectDlg)

public:
	CreateSubjectDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CreateSubjectDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CreateSubject };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CString m_tutor;
	CString m_name;
	CString m_room;
public:
	afx_msg void OnBnClickedOk();
};
