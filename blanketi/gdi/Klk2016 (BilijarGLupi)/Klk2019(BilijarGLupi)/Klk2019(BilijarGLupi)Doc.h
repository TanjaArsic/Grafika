
// Klk2019(BilijarGLupi)Doc.h : interface of the CKlk2019BilijarGLupiDoc class
//


#pragma once


class CKlk2019BilijarGLupiDoc : public CDocument
{
protected: // create from serialization only
	CKlk2019BilijarGLupiDoc() noexcept;
	DECLARE_DYNCREATE(CKlk2019BilijarGLupiDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CKlk2019BilijarGLupiDoc();
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
