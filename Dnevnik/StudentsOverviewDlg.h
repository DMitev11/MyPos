#pragma once
#include <vector>
#include "DatabaseSets.h"
// StudentsOverviewDlg dialog

class StudentsOverviewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(StudentsOverviewDlg)

public:
	std::vector<SQLGUID> students;
	
	StudentsOverviewDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~StudentsOverviewDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog(); 

	DECLARE_MESSAGE_MAP()
};
