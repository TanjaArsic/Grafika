
// Klk(Zastava)View.h : interface of the CKlkZastavaView class
//

#pragma once
#include"DImage.h"

class CKlkZastavaView : public CView
{
protected: // create from serialization only
	CKlkZastavaView() noexcept;
	DECLARE_DYNCREATE(CKlkZastavaView)

// Attributes
public:
	CKlkZastavaDoc* GetDocument() const;
	DImage* flag;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Shear(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawFlagPart(CDC* pDC, DImage* img, double t1, double t2, int x1, int y1, int x2, int y2, int h);
	void DrawFlag(CDC* pDC, DImage* img, int x, int y, int w, int h, int nSeg, double a);
	void DrawFlags(CDC* pDC, DImage* img, int cx, int cy, int w, int h, int nSeg, double a, int nFlag);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKlkZastavaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Klk(Zastava)View.cpp
inline CKlkZastavaDoc* CKlkZastavaView::GetDocument() const
   { return reinterpret_cast<CKlkZastavaDoc*>(m_pDocument); }
#endif

