
// IND_17549View.h : interface of the CIND17549View class
//

#pragma once

struct kaktusNode {
	// 3 kao magican broj = max grana po nivou
	int numOfBranches;
	float* angles[3] = { nullptr,nullptr,nullptr };
	kaktusNode* decendants[3] = { nullptr,nullptr,nullptr };//ovako mora inace je glup da skonta sam da je nullpointer
};

class CIND17549View : public CView
{
protected: // create from serialization only
	CIND17549View() noexcept;
	DECLARE_DYNCREATE(CIND17549View)
	bool keyPressed;
	float angle1 = 0;
	float angle2 = 0;
	int dimenzija;
	HENHMETAFILE light, dark;


// Attributes
public:
	CIND17549Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
protected:

// Implementation
public:
	virtual ~CIND17549View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void Kaktus(CDC* pDC, kaktusNode node);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void DrawFigure(CDC* pDC);
	void DrawPot(CDC* pDC);
	void DrawBackground(CDC* pDC);
	void DrawFont(CDC* pDC);
	float DegToRad(float deg);
	void KaktusPart(CDC* pDC, int branchScale, kaktusNode* next, float* angle);
protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in IND_17549View.cpp
inline CIND17549Doc* CIND17549View::GetDocument() const
   { return reinterpret_cast<CIND17549Doc*>(m_pDocument); }
#endif

