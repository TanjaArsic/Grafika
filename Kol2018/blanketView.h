
// blanketView.h : interface of the CblanketView class
//

#pragma once
#include"DImage.h"

class CblanketView : public CView
{
protected: // create from serialization only
	CblanketView() noexcept;
	DECLARE_DYNCREATE(CblanketView)

// Attributes
public:
	CblanketDoc* GetDocument() const;
	DImage* bager;
	DImage* arm1;
	DImage* arm2;
	DImage* pozadina;

	float arm1Rotate;
	float arm2Rotate;
	float forkRotate;
	float gas;

	HENHMETAFILE fork;
	ENHMETAHEADER header_fork;


// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	float torad(float angle);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
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
	virtual ~CblanketView();
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

#ifndef _DEBUG  // debug version in blanketView.cpp
inline CblanketDoc* CblanketView::GetDocument() const
   { return reinterpret_cast<CblanketDoc*>(m_pDocument); }
#endif

