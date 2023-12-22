
// Klk2019(BilijarGLupi)View.h : interface of the CKlk2019BilijarGLupiView class
//

#pragma once
#include"DImage.h"

class CKlk2019BilijarGLupiView : public CView
{
protected: // create from serialization only
	CKlk2019BilijarGLupiView() noexcept;
	DECLARE_DYNCREATE(CKlk2019BilijarGLupiView)

// Attributes
public:
	CKlk2019BilijarGLupiDoc* GetDocument() const;
	DImage* felt2, * wood;
	float stickRotat, stickMove, ballMove, ballRotat;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawStick(CDC* pDC, int w);
	void DrawBall(CDC* pDC, int w);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
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
	virtual ~CKlk2019BilijarGLupiView();
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

#ifndef _DEBUG  // debug version in Klk2019(BilijarGLupi)View.cpp
inline CKlk2019BilijarGLupiDoc* CKlk2019BilijarGLupiView::GetDocument() const
   { return reinterpret_cast<CKlk2019BilijarGLupiDoc*>(m_pDocument); }
#endif

