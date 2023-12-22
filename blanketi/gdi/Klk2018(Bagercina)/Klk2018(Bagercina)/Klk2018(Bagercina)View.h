
// Klk2018(Bagercina)View.h : interface of the CKlk2018BagercinaView class
//

#pragma once
#include"DImage.h"

class CKlk2018BagercinaView : public CView
{
protected: // create from serialization only
	CKlk2018BagercinaView() noexcept;
	DECLARE_DYNCREATE(CKlk2018BagercinaView)

// Attributes
public:
	CKlk2018BagercinaDoc* GetDocument() const;
	DImage* arm1, * arm2, * bager, * bkground;
	HENHMETAFILE fork;
	ENHMETAHEADER forkHeader;
	float arm1Rotat, arm2Rotat, forkRotat, bagerMove;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply); 
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Shear(CDC* pDC, float sX, float sY, bool rightMultiply);
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
	virtual ~CKlk2018BagercinaView();
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

#ifndef _DEBUG  // debug version in Klk2018(Bagercina)View.cpp
inline CKlk2018BagercinaDoc* CKlk2018BagercinaView::GetDocument() const
   { return reinterpret_cast<CKlk2018BagercinaDoc*>(m_pDocument); }
#endif

