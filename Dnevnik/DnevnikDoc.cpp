
// DnevnikDoc.cpp : implementation of the CDnevnikDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Dnevnik.h"
#endif

#include "DnevnikDoc.h"
#include "CWrapperView.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDnevnikDoc

IMPLEMENT_DYNCREATE(CDnevnikDoc, CDocument)

BEGIN_MESSAGE_MAP(CDnevnikDoc, CDocument)
END_MESSAGE_MAP()


// CDnevnikDoc construction/destruction

CDnevnikDoc::CDnevnikDoc() noexcept
{
	// TODO: add one-time construction code here
}

CDnevnikDoc::~CDnevnikDoc()
{
}

BOOL CDnevnikDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	UpdateDocument();
// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	
	return TRUE;
}

void CDnevnikDoc::UpdateDocument()
{ 
	std::vector<Student> students;
	auto student_set = StudentDatabase::GetStudentSet();
	student_set.NextRow();
	while (student_set.IsAvailable()) {
		students.push_back(student_set.GetRow());
		student_set.NextRow();
	}
	m_students = students;

	std::vector<Subject> subjects;
	auto subject_set = SubjectDatabase::GetSubjectSet();
	subject_set.NextRow();
	while (subject_set.IsAvailable()) {
		subjects.push_back(subject_set.GetRow());
		subject_set.NextRow();
	}
	m_subjects = subjects;

	std::vector<Grade> grades;
	auto grade_set = GradeDatabase::GetGradeSet();
	grade_set.NextRow();
	while (grade_set.IsAvailable()) {
		grades.push_back(grade_set.GetRow());
		grade_set.NextRow();
	}
	m_grades = grades; 
}




// CDnevnikDoc serialization

void CDnevnikDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CDnevnikDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CDnevnikDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CDnevnikDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CDnevnikDoc diagnostics

#ifdef _DEBUG
void CDnevnikDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDnevnikDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDnevnikDoc commands
