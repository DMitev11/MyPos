// CWrapperView.cpp : implementation file
//

#include "pch.h"
#include "CWrapperView.h" 
#include "Dnevnik.h" 
#include "CreateStudentDlg.h"
#include "CreateSubjectDlg.h"
#include "CreateGradeDlg.h"
#include "StudentBirthdaysDlg.h"
#include "AverageStudentDlg.h"
#include "AverageBelowDlg.h"
 
BEGIN_MESSAGE_MAP(CInPlaceEdit, CEdit)
	//{{AFX_MSG_MAP(CInPlaceEdit)
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_WM_CHAR()
	ON_WM_CREATE()
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CInPlaceEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	CString str;
	GetWindowText(str);

	// Send Notification to parent of ListView ctrl
	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = 1000000 + m_code;
	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = m_item;
	dispinfo.item.iSubItem = m_subItem;
	dispinfo.item.pszText = LPTSTR((LPCTSTR)str);
	dispinfo.item.cchTextMax = str.GetLength();
	 
	GetParent()->GetParent()->SendMessage(WM_NOTIFY, (WPARAM)GetDlgCtrlID(), (LPARAM)&dispinfo);

	DestroyWindow();
}

void CInPlaceEdit::OnNcDestroy()
{
	CEdit::OnNcDestroy();

	// To prevent memory leaks, delete the object when it is destroyed.
	delete this;
} 
// CWrapperView

IMPLEMENT_DYNCREATE(CWrapperView, CFormView)

CWrapperView::CWrapperView()
	: CFormView(IDD_CWrapperView)
{

}

CWrapperView::~CWrapperView()
{
}

void CWrapperView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_studentList);
	DDX_Control(pDX, IDC_LIST2, m_subjectList);
	DDX_Control(pDX, IDC_LIST3, m_gradeList);

	ListView_SetExtendedListViewStyle(m_studentList, LVS_EX_GRIDLINES);
	ListView_SetExtendedListViewStyle(m_subjectList, LVS_EX_GRIDLINES);
	ListView_SetExtendedListViewStyle(m_gradeList, LVS_EX_GRIDLINES);

	m_studentList.InsertColumn(0, L"id", 0, 50, 0);
	m_studentList.InsertColumn(1, L"class", 0, 50, 1);
	m_studentList.InsertColumn(2, L"name", 0, 120, 2);
	m_studentList.InsertColumn(3, L"date", 0, 80, 3);

	m_subjectList.InsertColumn(0, L"id", 0, 50, 0);
	m_subjectList.InsertColumn(1, L"name", 0, 80, 1);
	m_subjectList.InsertColumn(2, L"tutor", 0, 80, 2);
	m_subjectList.InsertColumn(3, L"room number", 0, 100, 3);
	
	m_gradeList.InsertColumn(0, L"id", 0, 50, 0);
	m_gradeList.InsertColumn(1, L"name", 0, 80, 1);
	m_gradeList.InsertColumn(2, L"subject", 0, 90, 2);
	m_gradeList.InsertColumn(3, L"date", 0, 60, 3);
	m_gradeList.InsertColumn(4, L"grade", 0, 60, 4);

	CFrameWnd* pFrame = GetParentFrame();
	if (pFrame != NULL)
	{
		CRect rect(0, 0, 1085, 650); // Set desired size
		pFrame->MoveWindow(&rect);
	}
}

BEGIN_MESSAGE_MAP(CWrapperView, CFormView) 
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CWrapperView::OnEditClickStudent)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST1, &CWrapperView::OnEditDeleteStudent)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CWrapperView::OnEditClickStudent)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CWrapperView::OnEditClickSubject)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST3, &CWrapperView::OnEditClickGrade)
	ON_NOTIFY(1000000, IDC_LIST1, &CWrapperView::OnEditSaveStudent)
	ON_NOTIFY(1000001, IDC_LIST2, &CWrapperView::OnEditSaveSubject)
	ON_NOTIFY(1000002, IDC_LIST3, &CWrapperView::OnEditSaveGrade)
	ON_BN_CLICKED(IDC_BUTTON5, &CWrapperView::OnButtonCreateStudent)
	ON_BN_CLICKED(IDC_BUTTON6, &CWrapperView::OnButtonCreateSubject)
	ON_BN_CLICKED(IDC_BUTTON7, &CWrapperView::OnButtonCreateGrade)
	ON_BN_CLICKED(IDC_BUTTON8, &CWrapperView::OnButtonAllBirthdays)
	ON_BN_CLICKED(IDC_BUTTON1, &CWrapperView::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CWrapperView::OnBnClickedButton2)
END_MESSAGE_MAP()


// CWrapperView diagnostics

#ifdef _DEBUG
void CWrapperView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CWrapperView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CWrapperView::OnSize(UINT nType, int cx, int cy) {
	CFormView::OnSize(nType, cx, cy);

	// Only adjust the view if the window has been resized to a normal state
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		// Get the client area of the frame window
		CRect rect;
		GetClientRect(&rect);

		// Adjust the size of the view
		// (This part depends on what you have in your view and how you want to adjust it)
		SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOMOVE);
	}
} 

CDnevnikDoc* CWrapperView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDnevnikDoc)));
	auto doc = (CDnevnikDoc*)m_pDocument;
	doc->UpdateDocument();
	return doc;
}

void CWrapperView::OnDraw(CDC* /*pDC*/)
{
	CDnevnikDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_studentList.DeleteAllItems();
	m_subjectList.DeleteAllItems();
	m_gradeList.DeleteAllItems();

	for (int i = 0; i < pDoc->m_students.size(); i++) {
		const auto [id, class_number, name, date] = pDoc->m_students[i].data();
		m_studentList.InsertItem(i, id.c_str(), 0);
		m_studentList.SetItemText(i, 0, id.c_str());
		m_studentList.SetItemText(i, 1, class_number.c_str());
		m_studentList.SetItemText(i, 2, name.c_str());
		m_studentList.SetItemText(i, 3, date.c_str());
	}

	for (int i = 0; i < pDoc->m_subjects.size(); i++) {
		const auto [id, name, tutor, room_number] = pDoc->m_subjects[i].data();
		m_subjectList.InsertItem(i, id.c_str(), 0);
		m_subjectList.SetItemText(i, 0, id.c_str());
		m_subjectList.SetItemText(i, 1, name.c_str());
		m_subjectList.SetItemText(i, 2, tutor.c_str());
		m_subjectList.SetItemText(i, 3, room_number.c_str());
	}

	for (int i = 0; i < pDoc->m_grades.size(); i++) {
		const auto grade_data = pDoc->m_grades[i];
		const auto [id, student_id, subject, date, grade] = pDoc->m_grades[i].data();
		const auto student = std::find_if(
			pDoc->m_students.begin(),
			pDoc->m_students.end(),
			[&](const database::Student& lhs)
			{
				return lhs.id == grade_data.student_id;
			});
		if (student == pDoc->m_students.end()) 
		{
			continue;
		}
		m_gradeList.InsertItem(i, id.c_str(), 0);
		m_gradeList.SetItemText(i, 0, id.c_str()); 
		m_gradeList.SetItemText(i, 1, (*student).name.c_str());
		m_gradeList.SetItemText(i, 2, subject.c_str());
		m_gradeList.SetItemText(i, 3, date.c_str());
		m_gradeList.SetItemText(i, 4, grade.c_str());
	} 
}

void CWrapperView::OnEditDeleteStudent(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMItemActivate = (NM_LISTVIEW*)(pNMHDR); 
	auto nItem = pNMItemActivate->iItem;
	auto pDoc = GetDocument();
	database::StudentDatabase::DeleteRow(pDoc->m_students[nItem].id);
	pDoc->UpdateDocument();

	*pResult = 0;

	Invalidate();
	UpdateWindow();
}

void CWrapperView::OnEditClickStudent(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMItemActivate = (NM_LISTVIEW*)(pNMHDR); 
	if (pNMItemActivate->iItem != -1) {
		m_itemFocus = pNMItemActivate->iItem;
		return;
	}
	m_subitemFocus = pNMItemActivate->iSubItem;

	if (m_itemFocus != -1 && m_subitemFocus != -1)
	{
		CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1); 
		CRect rect;
		pList->GetSubItemRect(m_itemFocus, m_subitemFocus, LVIR_LABEL, rect);
		m_editControl = new CInPlaceEdit(m_itemFocus, m_subitemFocus, pList->GetItemText(m_itemFocus, m_subitemFocus), 0);
		m_editControl->Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, rect, pList, 1001);
		m_editControl->SetFocus();
	}

	*pResult = 0;
}

void CWrapperView::OnEditSaveStudent(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR); 
	CListCtrl* pListCtrl = reinterpret_cast<CListCtrl*>(GetDlgItem(IDC_LIST1));
	if (pListCtrl)
	{
		auto pDoc = GetDocument();
		auto student = pDoc->m_students[pDispInfo->item.iItem]; 
		switch (pDispInfo->item.iSubItem) {
		case 1: {
			student.class_number = std::stoi((wchar_t*)pDispInfo->item.pszText);
			break;
		}
		case 2: {
			student.name = std::wstring((wchar_t*)pDispInfo->item.pszText);
			break; 
		}
		case 3: { 
			auto date = std::wstring((wchar_t*)pDispInfo->item.pszText);
			student.date.year = std::stoi(date.substr(0, 4));
			student.date.month = std::stoi(date.substr(5, 2));
			student.date.day = std::stoi(date.substr(8, 4));
			break;
		}
		}
		database::StudentDatabase::EditRow(student);
		pListCtrl->SetItemText(pDispInfo->item.iItem, pDispInfo->item.iSubItem, pDispInfo->item.pszText);
		pDoc->UpdateDocument(); 
	}

	*pResult = 0;

	Invalidate();
	UpdateWindow();
}

void CWrapperView::OnEditClickSubject(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMItemActivate = (NM_LISTVIEW*)(pNMHDR);
	if (pNMItemActivate->iItem != -1) {
		m_itemFocus = pNMItemActivate->iItem;
		return;
	}
	m_subitemFocus = pNMItemActivate->iSubItem;

	if (m_itemFocus != -1 && m_subitemFocus != -1)
	{
		CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
		CRect rect;
		pList->GetSubItemRect(m_itemFocus, m_subitemFocus, LVIR_LABEL, rect);
		m_editControl = new CInPlaceEdit(m_itemFocus, m_subitemFocus, pList->GetItemText(m_itemFocus, m_subitemFocus), 1);
		m_editControl->Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, rect, pList, 1001);
		m_editControl->SetFocus();
	}

	*pResult = 0;
}

void CWrapperView::OnEditSaveSubject(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	CListCtrl* pListCtrl = reinterpret_cast<CListCtrl*>(GetDlgItem(IDC_LIST2));
	if (pListCtrl)
	{
		pListCtrl->SetItemText(pDispInfo->item.iItem, pDispInfo->item.iSubItem, pDispInfo->item.pszText);
	}

	*pResult = 0;

	Invalidate();
	UpdateWindow();
}

void CWrapperView::OnEditClickGrade(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMItemActivate = (NM_LISTVIEW*)(pNMHDR);
	if (pNMItemActivate->iItem != -1) {
		m_itemFocus = pNMItemActivate->iItem;
		return;
	}
	m_subitemFocus = pNMItemActivate->iSubItem;

	if (m_itemFocus != -1 && m_subitemFocus != -1)
	{
		CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST1);
		CRect rect;
		pList->GetSubItemRect(m_itemFocus, m_subitemFocus, LVIR_LABEL, rect);
		m_editControl = new CInPlaceEdit(m_itemFocus, m_subitemFocus, pList->GetItemText(m_itemFocus, m_subitemFocus), 2);
		m_editControl->Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, rect, pList, 1001);
		m_editControl->SetFocus();
	}

	*pResult = 0;
}

void CWrapperView::OnEditSaveGrade(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	CListCtrl* pListCtrl = reinterpret_cast<CListCtrl*>(GetDlgItem(IDC_LIST3));
	if (pListCtrl)
	{
		pListCtrl->SetItemText(pDispInfo->item.iItem, pDispInfo->item.iSubItem, pDispInfo->item.pszText);
	}

	*pResult = 0;

	Invalidate();
	UpdateWindow();
}


void CWrapperView::OnButtonCreateStudent()
{
	CreateStudentDlg dlg;
	dlg.DoModal();  
	Invalidate();
	UpdateWindow(); 
}


void CWrapperView::OnButtonCreateSubject()
{
	CreateSubjectDlg dlg;
	dlg.DoModal(); 
	Invalidate();
	UpdateWindow(); 
}


void CWrapperView::OnButtonCreateGrade()
{
	CreateGradeDlg dlg;
	dlg.DoModal(); 
	Invalidate();
	UpdateWindow(); 
}


void CWrapperView::OnButtonAllBirthdays()
{
	StudentBirthdaysDlg dlg;
	dlg.DoModal(); 
	Invalidate();
	UpdateWindow(); 
}


void CWrapperView::OnBnClickedButton1()
{
	AverageStudentDlg dlg;
	dlg.DoModal();
	Invalidate();
	UpdateWindow();
}


void CWrapperView::OnBnClickedButton2()
{
	AverageBelowDlg dlg;
	dlg.DoModal();
	Invalidate();
	UpdateWindow();
}
