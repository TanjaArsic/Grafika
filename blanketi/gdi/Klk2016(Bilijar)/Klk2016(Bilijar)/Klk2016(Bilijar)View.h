
// Klk2016(Bilijar)View.h : interface of the CKlk2016BilijarView class
//

#pragma once
#include"DImage.h"

class CKlk2016BilijarView : public CView
{
protected: // create from serialization only
	CKlk2016BilijarView() noexcept;
	DECLARE_DYNCREATE(CKlk2016BilijarView)

// Attributes
public:
	CKlk2016BilijarDoc* GetDocument() const;
	DImage* felt2, * wood;
	float ballRotat, stickRotat, moveStick, moveBall;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawStick(CDC* pDC, int w);
	void DrawBall(CDC* pDC, int w);
	void DrawTable(CDC* pDC, CRect rect);
	void DrawBorder(CDC* pDC, CRect rect, int w);
	void DrawHoles(CDC* pDC, CRect rect, int size); 
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKlk2016BilijarView();
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

#ifndef _DEBUG  // debug version in Klk2016(Bilijar)View.cpp
inline CKlk2016BilijarDoc* CKlk2016BilijarView::GetDocument() const
   { return reinterpret_cast<CKlk2016BilijarDoc*>(m_pDocument); }
#endif

