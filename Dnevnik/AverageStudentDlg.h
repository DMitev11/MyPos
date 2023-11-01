#pragma once

#include "DatabaseSets.h" 
// AverageStudentDlg dialog

class AverageStudentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AverageStudentDlg)

public:
	AverageStudentDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AverageStudentDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AverageStudent };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();  

	DECLARE_MESSAGE_MAP() 
private:
	CComboBox m_studentBox;
	CComboBox m_subjectBox;
	CEdit m_resultBox;
	CButton m_submitBtn;

	std::vector<database::Student> m_students;
	std::vector<database::Subject> m_subjects;

public:
	afx_msg void OnBnClickedButton1();
};
