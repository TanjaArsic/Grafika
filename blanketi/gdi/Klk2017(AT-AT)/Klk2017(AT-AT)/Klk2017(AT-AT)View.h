
// Klk2017(AT-AT)View.h : interface of the CKlk2017ATATView class
//

#pragma once
#include"DImage.h"


class CKlk2017ATATView : public CView
{
protected: // create from serialization only
	CKlk2017ATATView() noexcept;
	DECLARE_DYNCREATE(CKlk2017ATATView)

// Attributes
public:
	CKlk2017ATATDoc* GetDocument() const;
	DImage* leg1, * leg2, * leg3, * body, * back2;
	float enlarge, position;
	int movement[8] = { -20, -10, 0, 10, 20 , 10, 0, -10 };
	int dx, topCorner, leftCorner;
// Operations
public:
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void LoadIdentity(CDC* pDC);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	float ToRad(float angle);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawLeg(CDC* pDC, double alpha, double dx, double dy);
	void DrawImgTransparent(CDC* pDC, DImage* pImage, CRect srcRc, CRect dstRc);
	void DrawTransparent(CDC* pDC, DImage* pImage, CRect srcRc, CRect dstRc);
	void DrawScene(CDC* pDC, CRect rect);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKlk2017ATATView();
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

#ifndef _DEBUG  // debug version in Klk2017(AT-AT)View.cpp
inline CKlk2017ATATDoc* CKlk2017ATATView::GetDocument() const
   { return reinterpret_cast<CKlk2017ATATDoc*>(m_pDocument); }
#endif

