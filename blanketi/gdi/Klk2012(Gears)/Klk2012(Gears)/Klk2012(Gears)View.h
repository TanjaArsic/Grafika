
// Klk2012(Gears)View.h : interface of the CKlk2012GearsView class
//

#pragma once


class CKlk2012GearsView : public CView
{
protected: // create from serialization only
	CKlk2012GearsView() noexcept;
	DECLARE_DYNCREATE(CKlk2012GearsView)

// Attributes
public:
	CKlk2012GearsDoc* GetDocument() const;
	float gearAngle;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void Trapez(CDC* pDC, int a, int b, int h);
	void Translate(CDC* pDC, float x, float y);
	void Rotate(CDC* pDC, float a);
	void DrawGear(CDC* pDC, int spokeCount, int spokeWidth);
	void DrawScene(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKlk2012GearsView();
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

#ifndef _DEBUG  // debug version in Klk2012(Gears)View.cpp
inline CKlk2012GearsDoc* CKlk2012GearsView::GetDocument() const
   { return reinterpret_cast<CKlk2012GearsDoc*>(m_pDocument); }
#endif

