
// KolaView.h : interface of the CKolaView class
//

#pragma once

#include"DImage.h"
class CKolaView : public CView
{
protected: // create from serialization only
	CKolaView() noexcept;
	DECLARE_DYNCREATE(CKolaView)

// Attributes
public:
	CKolaDoc* GetDocument() const;
	DImage* wheel;

	HENHMETAFILE clio;
	ENHMETAHEADER clio_header;

	float angle;
	float poz;
	float wheel_angle;


// Operations
public:

// Overrides
public:
	float toRad(float angle);
	void DrawGround(CDC* pDC, float angle);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawCar(CDC* pDC, int x, int y, int w, int h);
	void DrawWheel(CDC* pDC, int x, int y, int r, float angle);

	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKolaView();
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
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in KolaView.cpp
inline CKolaDoc* CKolaView::GetDocument() const
   { return reinterpret_cast<CKolaDoc*>(m_pDocument); }
#endif

