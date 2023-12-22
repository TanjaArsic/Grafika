
// Klk(Tehnicki crtez)View.h : interface of the CKlkTehnickicrtezView class
//

#pragma once
#include"DImage.h"

class CKlkTehnickicrtezView : public CView
{
protected: // create from serialization only
	CKlkTehnickicrtezView() noexcept;
	DECLARE_DYNCREATE(CKlkTehnickicrtezView)

// Attributes
public:
	CKlkTehnickicrtezDoc* GetDocument() const;
	bool clicked;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawDrawing(CDC* pDC, float a, float b, float r);
	void DrawMeasurements(CDC* pDC, float mm);
	void OnClick(CDC* pDC, CBitmap& bitmap);
	void Translate(CDC* pDC, float dX, float dY);
	void Rotate(CDC* pDC, float angle);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKlkTehnickicrtezView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in Klk(Tehnicki crtez)View.cpp
inline CKlkTehnickicrtezDoc* CKlkTehnickicrtezView::GetDocument() const
   { return reinterpret_cast<CKlkTehnickicrtezDoc*>(m_pDocument); }
#endif

