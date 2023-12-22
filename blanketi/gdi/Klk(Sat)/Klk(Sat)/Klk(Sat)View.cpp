
// Klk(Sat)View.cpp : implementation of the CKlkSatView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk(Sat).h"
#endif

#include "Klk(Sat)Doc.h"
#include "Klk(Sat)View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define pi 3.14159

// CKlkSatView

IMPLEMENT_DYNCREATE(CKlkSatView, CView)

BEGIN_MESSAGE_MAP(CKlkSatView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKlkSatView construction/destruction

CKlkSatView::CKlkSatView() noexcept
{
	// TODO: add construction code here
	hrRotat = minRotat = secRotat = 0.0f;

}

CKlkSatView::~CKlkSatView()
{
}

BOOL CKlkSatView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlkSatView drawing

void CKlkSatView::OnDraw(CDC* pDC)
{
	CKlkSatDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CBitmap bmp;
	CRect c;
	GetClientRect(&c);
	bmp.CreateCompatibleBitmap(pDC, c.Width(), c.Height());
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	memDC->SelectObject(&bmp);
	memDC->SetGraphicsMode(GM_ADVANCED);
	XFORM x;
	memDC->GetWorldTransform(&x);
	memDC->Rectangle(c);
	Translate(memDC, c.CenterPoint().x, c.CenterPoint().y, false);
	DrawClock(memDC);
	// TODO: add draw code for native data here
	memDC->SetWorldTransform(&x);
	pDC->BitBlt(0, 0, c.Width(), c.Height(), memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC();

}
void CKlkSatView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM mat = { 1,0,0,1,dX,dY };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKlkSatView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	angle *= pi / 180;
	XFORM mat = { cos(angle),sin(angle),-sin(angle),cos(angle),0,0 };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKlkSatView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM mat = { sX,0,0,sY,0,0 };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKlkSatView::DrawClock(CDC* pDC)
{
	
	CBrush grayBrush(RGB(200, 200, 200));
	CBrush* old = pDC->SelectObject(&grayBrush);
	CPen shadow(PS_SOLID, 2, RGB(128, 128, 128)); 
		
	pDC->Rectangle(-100, -100, 100, 100);
	pDC->SelectStockObject(WHITE_BRUSH);
	float angle = 360 / 12;
	for (int i = 12; i >= 1; i--)
	{
		Translate(pDC, 0, -74, false); // nagadjanje koordinata jer volimo
		pDC->Rectangle(-2.5,0, 2.5, 9);
		Translate(pDC, 0, -26, false);
		CString str;
		str.Format(_T("%d"), i);
		DrawNumber(pDC, str);
		Translate(pDC,0, 100, false);
		Rotate(pDC, -angle, false);

	}
	POINT hrPts[] = { CPoint(-5, 0), CPoint(0, 5), CPoint(50, 0), CPoint(0, -5) };
	POINT minPts[] = { CPoint(-5, 0), CPoint(0, 5), CPoint(70, 0), CPoint(0, -5) };
	POINT secPts[] = { CPoint(-1, 1), CPoint(80, 1), CPoint(80, -1), CPoint(-1, -1) };
	CPen* oldPen = pDC->SelectObject(&shadow);

	XFORM hr;
	pDC->GetWorldTransform(&hr);
	Rotate(pDC, 1.5 * angle + hrRotat, false);
	pDC->MoveTo(hrPts[2].x + 1, hrPts[2].y - 1);
	pDC->LineTo(hrPts[3].x + 1, hrPts[3].y - 1);
	pDC->SelectObject(oldPen);
	pDC->Polygon(hrPts,4);
	pDC->SetWorldTransform(&hr);

	XFORM min;
	pDC->GetWorldTransform(&min);
	Rotate(pDC, 3.3 * angle + minRotat, false);
	pDC->SelectObject(&shadow);
	pDC->MoveTo(minPts[2].x + 1, minPts[2].y - 1);
	pDC->LineTo(minPts[3].x + 1, minPts[3].y - 1);
	pDC->SelectObject(oldPen);
	pDC->Polygon(minPts, 4);
	pDC->SetWorldTransform(&min);

	XFORM sec;
	pDC->GetWorldTransform(&sec);
	Rotate(pDC, 5.7 * angle + secRotat, false);
	pDC->SelectObject(&shadow);
	pDC->MoveTo(secPts[2].x + 1, secPts[2].y - 1);
	pDC->LineTo(secPts[3].x + 1, secPts[3].y - 1);
	pDC->SelectObject(oldPen);
	pDC->Polygon(secPts, 4);
	pDC->SetWorldTransform(&sec);

}

void CKlkSatView::DrawNumber(CDC* pDC, CString num)
{
	CFont font;
	font.CreateFontW(16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CString("Zeitgeist Std Condensed"));
	COLORREF col = pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextAlign(TA_CENTER);
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->TextOutW(0, 0, num);

	pDC->SetBkMode(col);

}


// CKlkSatView printing

BOOL CKlkSatView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlkSatView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlkSatView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKlkSatView diagnostics

#ifdef _DEBUG
void CKlkSatView::AssertValid() const
{
	CView::AssertValid();
}

void CKlkSatView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlkSatDoc* CKlkSatView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlkSatDoc)));
	return (CKlkSatDoc*)m_pDocument;
}
#endif //_DEBUG


// CKlkSatView message handlers


void CKlkSatView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar)
	{
	case 'W': hrRotat += 10; break;
	case 'Q': hrRotat -= 10; break;
	case 'S': minRotat += 10; break;
	case 'A': minRotat -= 10; break;
	case 'X': secRotat += 10; break;
	case 'Z': secRotat -= 10; break;
	}

	if (nChar == VK_RIGHT)
	{
		secRotat += 10;
		if ((int)secRotat % 360 == 0 && (int)secRotat != 0) 
		{
			minRotat += 10;

			if ((int)minRotat % 360 == 0 && (int)minRotat != 0)
			{
				hrRotat += 10;
			}
		}
	}

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CKlkSatView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}
