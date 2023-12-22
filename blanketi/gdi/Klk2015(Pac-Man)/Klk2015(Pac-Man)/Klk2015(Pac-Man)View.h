
// Klk2015(Pac-Man)View.h : interface of the CKlk2015PacManView class
//

#pragma once
#include"DImage.h"


class CKlk2015PacManView : public CView
{
protected: // create from serialization only
	CKlk2015PacManView() noexcept;
	DECLARE_DYNCREATE(CKlk2015PacManView)

// Attributes
public:
	CKlk2015PacManDoc* GetDocument() const;
	DImage* blue;
	float angle, moveX, moveY, direction;
	int turn;
	bool touched;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawPacman(CDC* pDC, CRect rect, float angle);
	void DrawGhost(CDC* pDC, CRect rect);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKlk2015PacManView();
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

#ifndef _DEBUG  // debug version in Klk2015(Pac-Man)View.cpp
inline CKlk2015PacManDoc* CKlk2015PacManView::GetDocument() const
   { return reinterpret_cast<CKlk2015PacManDoc*>(m_pDocument); }
#endif

