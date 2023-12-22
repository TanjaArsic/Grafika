
// Klk2014(Clio)View.h : interface of the CKlk2014ClioView class
//

#pragma once
#include"DImage.h"


class CKlk2014ClioView : public CView
{
protected: // create from serialization only
	CKlk2014ClioView() noexcept;
	DECLARE_DYNCREATE(CKlk2014ClioView)

// Attributes
public:
	CKlk2014ClioDoc* GetDocument() const;
	float angle, moveCar, wheelAngle;
	DImage* wheel;
	HENHMETAFILE clio;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawGround(CDC* pDC, float angle);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	float ToRad(float angle);
	void DrawCar(CDC* pDC, int x, int y, int w, int h); 
	void DrawWheel(CDC* pDC, int x, int y, int r, float angle);
	void DrawImgTransparent(CDC* pDC, DImage* pImage, CRect srcRc, CRect dstRc);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKlk2014ClioView();
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

#ifndef _DEBUG  // debug version in Klk2014(Clio)View.cpp
inline CKlk2014ClioDoc* CKlk2014ClioView::GetDocument() const
   { return reinterpret_cast<CKlk2014ClioDoc*>(m_pDocument); }
#endif

