
// Klk(Bicikl)View.h : interface of the CKlkBiciklView class
//

#pragma once


class CKlkBiciklView : public CView
{
protected: // create from serialization only
	CKlkBiciklView() noexcept;
	DECLARE_DYNCREATE(CKlkBiciklView)

// Attributes
public:
	CKlkBiciklDoc* GetDocument() const;
	float moveBicycle;
	clock_t timeStart;
	UINT_PTR timer;
	float time = 0.0f;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void DrawCycle(CDC* pDC, int r, int n, int d, COLORREF clr);
	void DrawFrame(CDC* pDC, int r, int d, COLORREF clr);
	void DrawSeat(CDC* pDC, int r, int d, float beta, COLORREF clr);
	void DrawPedals(CDC* pDC, int r, int d, float alpha, COLORREF clr);
	void DrawBicycle(CDC* pDC, int r, int n, int d, float beta, COLORREF clr, float omega, float time);
	void DrawSignature(CDC* pDC, CRect rcTrack, int fontHeight, CString strText, COLORREF clrText);
	void DrawTrack(CDC* pDC, CRect rcTrack, int lineWidth, COLORREF clrSky, COLORREF clrGrass, COLORREF clrTrack);
	void DrawBicycleOnTrack(CDC* pDC, CRect rcTrack, int r, int n, int d, float beta, COLORREF clr, COLORREF clrSky, COLORREF clrGrass, COLORREF clrTrack, CString strText, float omega, float time);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKlkBiciklView();
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
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // debug version in Klk(Bicikl)View.cpp
inline CKlkBiciklDoc* CKlkBiciklView::GetDocument() const
   { return reinterpret_cast<CKlkBiciklDoc*>(m_pDocument); }
#endif

