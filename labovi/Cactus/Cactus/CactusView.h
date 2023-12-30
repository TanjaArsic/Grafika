
// CactusView.h : interface of the CCactusView class
//

#pragma once


class CCactusView : public CView
{
protected: // create from serialization only
	CCactusView() noexcept;
	DECLARE_DYNCREATE(CCactusView)

	HENHMETAFILE dark_emf, light_emf;
	int dim;
	int numOfSquares;
	int grid;
	bool drGrid;
	float radius;
	double angle;
	double angle2;

	enum type { fat, norm, skinny };
	enum color { dark, light };

// Attributes
public:
	CCactusDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	double ToDegrees(double rad);
	double ToRadians(double deg);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawBackground(CDC* pDC);
	void DrawCactus(CDC* pDC);
	void DrawCircle(CDC* pDC);
	void DrawPartCactus(CDC* pDC, type t, color color);
	void DrawGrid(CDC* pDC);
	void DrawPot(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CCactusView();
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
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in CactusView.cpp
inline CCactusDoc* CCactusView::GetDocument() const
   { return reinterpret_cast<CCactusDoc*>(m_pDocument); }
#endif

