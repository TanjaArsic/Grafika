
// Klk2018(Bager)View.h : interface of the CKlk2018BagerView class
//

#pragma once
#include"DImage.h"

class CKlk2018BagerView : public CView
{
protected: // create from serialization only
	CKlk2018BagerView() noexcept;
	DECLARE_DYNCREATE(CKlk2018BagerView)

// Attributes
public:
	CKlk2018BagerDoc* GetDocument() const;
	DImage* bager, *arm1, *arm2, *bkground;
	HENHMETAFILE fork; 
	ENHMETAHEADER forkRect;
	float smolCrcl, bigCrcl, forkRotat, moveDx;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	float ToRad(float angle);
	void Rotate(CDC* pDC, float angle, bool rightMultiply); 
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawBackground(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawBody(CDC* pDC);
	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawFork(CDC* pDC);
	void DrawExcavator(CDC* pDC);

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKlk2018BagerView();
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

#ifndef _DEBUG  // debug version in Klk2018(Bager)View.cpp
inline CKlk2018BagerDoc* CKlk2018BagerView::GetDocument() const
   { return reinterpret_cast<CKlk2018BagerDoc*>(m_pDocument); }
#endif

