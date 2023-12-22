
// Klk2019(Transformers)View.h : interface of the CKlk2019TransformersView class
//

#pragma once
#include"DImage.h"

class CKlk2019TransformersView : public CView
{
protected: // create from serialization only
	CKlk2019TransformersView() noexcept;
	DECLARE_DYNCREATE(CKlk2019TransformersView)

// Attributes
public:
	CKlk2019TransformersDoc* GetDocument() const;
	DImage* body1, * arm1, * arm2, * leg1, * leg2, * background;
	float rotArm1, rotArm2, rotLeg1, rotLeg2, rotBody;
	int move;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawBackground(CDC* pDC, CRect rc);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	float ToRad(float angle);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawLeg1(CDC* pDC);
	void DrawLeg2(CDC* pDC);
	void DrawBody1(CDC* pDC);
	void DrawTransformer(CDC* pDC);
	void DrawCenter(CDC* pDC);

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKlk2019TransformersView();
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

#ifndef _DEBUG  // debug version in Klk2019(Transformers)View.cpp
inline CKlk2019TransformersDoc* CKlk2019TransformersView::GetDocument() const
   { return reinterpret_cast<CKlk2019TransformersDoc*>(m_pDocument); }
#endif

