#pragma once

#include <vector>
#include "DatabaseSets.h"
// CreateGradeDlg dialog

class CreateGradeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CreateGradeDlg)

public:
	CreateGradeDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CreateGradeDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CreateGrade };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog() override;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
private:
	CString m_tutor;
	CComboBox m_subjectBox;
	CString m_subject;
	CComboBox m_studentBox;
	CString m_student;
	CString m_date;
	CString m_grade;

	std::vector<database::Student> m_students;
	std::vector<database::Subject> m_subjects;
};
