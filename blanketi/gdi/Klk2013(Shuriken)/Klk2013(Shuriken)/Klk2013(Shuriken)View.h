
// Klk2013(Shuriken)View.h : interface of the CKlk2013ShurikenView class
//

#pragma once
#include"DImage.h"

class CKlk2013ShurikenView : public CView
{
protected: // create from serialization only
	CKlk2013ShurikenView() noexcept;
	DECLARE_DYNCREATE(CKlk2013ShurikenView)

// Attributes
public:
	DImage* sky, * bark;
	float moveShuriken, rotateShuriken;
	bool hit = false;
	CKlk2013ShurikenDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void Translate(CDC* pDC, double x, double y);
	void Rotate(CDC* pDC, float angle);
	void DrawBlade(CDC* pDC, int size);
	void DrawStar(CDC* pDC, int size);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKlk2013ShurikenView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in Klk2013(Shuriken)View.cpp
inline CKlk2013ShurikenDoc* CKlk2013ShurikenView::GetDocument() const
   { return reinterpret_cast<CKlk2013ShurikenDoc*>(m_pDocument); }
#endif

