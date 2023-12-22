
// MonaLisaView.h : interface of the CMonaLisaView class
//

#pragma once
//#include "DImage.h"

class CMonaLisaView : public CView
{
protected: // create from serialization only
	CMonaLisaView() noexcept;
	DECLARE_DYNCREATE(CMonaLisaView)

// Attributes
	int numOfSquares, grid, dim;
	DImage** puzzle;
public:
	CMonaLisaDoc* GetDocument() const;

// Operations
public:
// Overrides
public:
	void DrawTransparent(CDC* pDC, DImage* img);
	void DrawGrid(CDC* pDC);
	void toGrayScale(CBitmap* bmpImg, BITMAP bm, bool filter);
	void MakeItGrey(DImage img);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void MemDCTopDC(CDC* memDC, CDC* pDC);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	float ToRad(float angle);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void DrawPiece(CDC* pDC, DImage* img, int dx, int dy, float angle, bool mx, bool my, bool blueFilter);
	void Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMonaLisaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MonaLisaView.cpp
inline CMonaLisaDoc* CMonaLisaView::GetDocument() const
   { return reinterpret_cast<CMonaLisaDoc*>(m_pDocument); }
#endif

