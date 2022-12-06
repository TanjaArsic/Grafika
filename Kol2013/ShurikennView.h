
// ShurikennView.h : interface of the CShurikennView class
//

#pragma once
#include"DImage.h"


class CShurikennView : public CView
{
protected: // create from serialization only
	CShurikennView() noexcept;
	DECLARE_DYNCREATE(CShurikennView)

// Attributes
public:
	CShurikennDoc* GetDocument() const;
	DImage* sky;
	DImage* bark;
	double dx;
	double angle;

// Operations
public:

// Overrides
public:
	void Text(CDC* pDC, int size);
	void DrawBlade(CDC* pDC, int size);
	float ToRad(float angle);
	void Rotate(CDC* pDC, float angle);
	void Translate(CDC* pDC, double x, double y);
	void DrawStar(CDC* pDC, int size);

	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CShurikennView();
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

#ifndef _DEBUG  // debug version in ShurikennView.cpp
inline CShurikennDoc* CShurikennView::GetDocument() const
   { return reinterpret_cast<CShurikennDoc*>(m_pDocument); }
#endif

