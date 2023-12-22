
// Klk(Sat)View.h : interface of the CKlkSatView class
//

#pragma once


class CKlkSatView : public CView
{
protected: // create from serialization only
	CKlkSatView() noexcept;
	DECLARE_DYNCREATE(CKlkSatView)

// Attributes
public:
	CKlkSatDoc* GetDocument() const;
	float hrRotat, minRotat, secRotat;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawClock(CDC* pDC);
	void DrawNumber(CDC* pDC, CString num);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKlkSatView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in Klk(Sat)View.cpp
inline CKlkSatDoc* CKlkSatView::GetDocument() const
   { return reinterpret_cast<CKlkSatDoc*>(m_pDocument); }
#endif

