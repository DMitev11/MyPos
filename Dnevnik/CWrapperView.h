#pragma once 
#include "DnevnikDoc.h"
class CInPlaceEdit : public CEdit
{
public:
    CInPlaceEdit(int iItem, int iSubItem, CString sInitText, int code)
        : m_item(iItem), m_subItem(iSubItem), m_initText(sInitText), m_code(code)
    {
    }

    ~CInPlaceEdit()
    {
    }
     
protected:
    int m_item;
    int m_subItem;
    CString m_initText;
    int m_code;

    // Generated message map functions
protected:
    //{{AFX_MSG(CInPlaceEdit) 
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnNcDestroy();  
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

class CWrapperView : public CFormView
{
	DECLARE_DYNCREATE(CWrapperView)

protected:
	CWrapperView();           // protected constructor used by dynamic creation
	virtual ~CWrapperView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CWrapperView };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	virtual void OnDraw(CDC* pDC);
	virtual void OnSize(UINT nType, int cx, int cy);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CDnevnikDoc* GetDocument();

	int m_itemFocus = -1;
	int m_subitemFocus = -1;
	CInPlaceEdit* m_editControl = nullptr;
	DECLARE_MESSAGE_MAP()
public: 
	afx_msg void OnEditDeleteStudent(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditClickStudent(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditSaveStudent(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnEditClickSubject(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditSaveSubject(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnEditClickGrade(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditSaveGrade(NMHDR* pNMHDR, LRESULT* pResult);
	CListCtrl m_studentList;
	CListCtrl m_subjectList;
	CListCtrl m_gradeList;
	afx_msg void OnButtonCreateStudent();
	afx_msg void OnButtonCreateSubject();
	afx_msg void OnButtonCreateGrade();
	afx_msg void OnButtonAllBirthdays();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};


