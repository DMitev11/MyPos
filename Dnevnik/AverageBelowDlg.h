#pragma once
#include <vector>
#include "DatabaseSets.h"

// AverageBelowDlg dialog

class AverageBelowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AverageBelowDlg)

public:
	AverageBelowDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AverageBelowDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AverageBelow };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void onButtonRadioSubject();
	afx_msg void onButtonRadioCount(); 

	DECLARE_MESSAGE_MAP()
private:
	CString m_grade;
	CComboBox m_subjectsBox;
	CString m_count;
	CButton m_radio_subject;
	CButton m_radio_count;

	std::vector<database::Subject> m_subjects; 
public:
	afx_msg void OnBnClickedOk();
};
