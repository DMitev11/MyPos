
#pragma once
#include "DatabaseSets.h" 

class CDnevnikDoc : public CDocument
{
	typedef database::StudentDatabase StudentDatabase;
	typedef database::SubjectDatabase SubjectDatabase;
	typedef database::GradeDatabase GradeDatabase;
	typedef database::Student Student;
	typedef database::Subject Subject;
	typedef database::Grade Grade;

protected: // create from serialization only
	CDnevnikDoc() noexcept;
	DECLARE_DYNCREATE(CDnevnikDoc)

	// Attributes
public:  

	std::vector<Student> m_students;
	std::vector<Subject> m_subjects;
	std::vector<Grade> m_grades;

	// Operations
public:


	// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void UpdateDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

	// Implementation
public:
	virtual ~CDnevnikDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
