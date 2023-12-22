
// Klk(Mrezni Saobracaj)View.h : interface of the CKlkMrezniSaobracajView class
//

#pragma once
#include"DImage.h"


class CKlkMrezniSaobracajView : public CView
{
protected: // create from serialization only
	CKlkMrezniSaobracajView() noexcept;
	DECLARE_DYNCREATE(CKlkMrezniSaobracajView)

// Attributes
public:
	CKlkMrezniSaobracajDoc* GetDocument() const;
	DImage* router;
	int pos;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawRouters(CDC* pDC, CPoint* pts, int count);
	void Translate(CDC* pDC, float dx, float dy, bool rightMultiply);
	float ToRad(float angle);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sx, float sy, bool rightMultiply);
	void DrawSerialLink(CDC* pDC, CPoint pt1, CPoint pt2);
	void DrawPacket(CDC* pDC, CPoint pt, int h);
	void DrawMessage(CDC* pDC, CPoint pt1, CPoint pt2, int pos, CSize size);

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKlkMrezniSaobracajView();
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

#ifndef _DEBUG  // debug version in Klk(Mrezni Saobracaj)View.cpp
inline CKlkMrezniSaobracajDoc* CKlkMrezniSaobracajView::GetDocument() const
   { return reinterpret_cast<CKlkMrezniSaobracajDoc*>(m_pDocument); }
#endif

