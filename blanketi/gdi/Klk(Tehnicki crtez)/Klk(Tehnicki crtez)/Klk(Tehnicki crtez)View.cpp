
// Klk(Tehnicki crtez)View.cpp : implementation of the CKlkTehnickicrtezView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk(Tehnicki crtez).h"
#endif

#include "Klk(Tehnicki crtez)Doc.h"
#include "Klk(Tehnicki crtez)View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define pi 3.14159


// CKlkTehnickicrtezView

IMPLEMENT_DYNCREATE(CKlkTehnickicrtezView, CView)

BEGIN_MESSAGE_MAP(CKlkTehnickicrtezView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKlkTehnickicrtezView construction/destruction

CKlkTehnickicrtezView::CKlkTehnickicrtezView() noexcept
{
	// TODO: add construction code here
	clicked = false;

}

CKlkTehnickicrtezView::~CKlkTehnickicrtezView()
{
}

BOOL CKlkTehnickicrtezView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlkTehnickicrtezView drawing

void CKlkTehnickicrtezView::OnDraw(CDC* pDC)
{
	CKlkTehnickicrtezDoc* pDoc = GetDocument();
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
	memDC->Rectangle(c);
	XFORM x;
	memDC->GetWorldTransform(&x);

	Translate(memDC, 300, 300);
	if (clicked) 
		Rotate(memDC, 90);
	DrawDrawing(memDC, 200, 100, 50);
	if (clicked)
		OnClick(memDC, bmp);
	DImage save(bmp);
	save.Save(CString("Saved.bmp"));
	// TODO: add draw code for native data here
	memDC->SetWorldTransform(&x);
	pDC->BitBlt(0, 0, c.Width(), c.Height(), memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC();
}

void CKlkTehnickicrtezView::DrawDrawing(CDC* pDC, float a, float b, float r)
{
	CPen borderPen;
	borderPen.CreatePen(PS_SOLID, 3, RGB(0, 0, 128));
	
	CBrush hatchBrush, hatchBrush2;
	LOGBRUSH lb;
	lb.lbStyle = BS_HATCHED;
	lb.lbColor = RGB(0, 0, 128); 
	lb.lbHatch = HS_BDIAGONAL;
	//hatchBrush.CreateHatchBrush(HS_BDIAGONAL, RGB(0, 0, 128));
	hatchBrush.CreateBrushIndirect(&lb);

	CPen* oldPen = pDC->SelectObject(&borderPen);
	CBrush* oldBrush = pDC->SelectObject(&hatchBrush);
	pDC->SetBkColor(RGB(255, 255, 0));
	
	pDC->Rectangle(0, 0, a, b);
	pDC->SetBkColor(RGB(255, 128, 0));
	lb.lbHatch = HS_DIAGCROSS;
	hatchBrush2.CreateBrushIndirect(&lb);
	pDC->SelectObject(&hatchBrush2);
	pDC->Ellipse(r / 2, b / 2 - r / 2, r / 2 + r, b / 2 + r / 2);

	pDC->SelectObject(oldPen);
	int offset = 20;

	// donje
	XFORM aa;
	pDC->GetWorldTransform(&aa);
	pDC->MoveTo(0, b);
	pDC->LineTo(0, b + offset);
	pDC->MoveTo(a, b);
	pDC->LineTo(a, b + offset);
	pDC->MoveTo(-5, b + offset - 5);
	pDC->LineTo(a + 5, b + offset - 5);
	Translate(pDC, a / 2, b + offset - 5);
	DrawMeasurements(pDC, a);
	pDC->SetWorldTransform(&aa);

	// desno
	XFORM ab;
	pDC->GetWorldTransform(&ab);
	pDC->MoveTo(a, 0);
	pDC->LineTo(a + offset, 0);
	pDC->MoveTo(a, b);
	pDC->LineTo(a + offset, b);
	pDC->MoveTo(a + offset - 5, -5);
	pDC->LineTo(a + offset - 5, b + 5);
	Translate(pDC, a + offset - 5, b/2);
	Rotate(pDC, -90);
	DrawMeasurements(pDC, b);
	pDC->SetWorldTransform(&ab);

	XFORM rb;
	pDC->GetWorldTransform(&rb);
	pDC->MoveTo(r, b / 2 - r / 2);
	pDC->LineTo(-offset, b / 2 - r / 2);
	pDC->MoveTo(r, b / 2 + r / 2);
	pDC->LineTo(-offset, b / 2 + r / 2);
	pDC->MoveTo(-offset + 5, b / 2 - r / 2 - 5);
	pDC->LineTo(-offset + 5, b / 2 + r / 2 + 5);
	Translate(pDC, -offset + 5, b / 2);
	Rotate(pDC, -90);
	DrawMeasurements(pDC, r);
	pDC->SetWorldTransform(&rb);
}

void CKlkTehnickicrtezView::DrawMeasurements(CDC* pDC, float mm)
{
	CString str;
	str.Format(_T("%d mm"), (int)mm);
	CFont font;
	font.CreateFontW(12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CString("Verdana"));
	pDC->SetTextAlign(TA_CENTER | TA_BASELINE);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&font);
	pDC->TextOutW(0,0, str);
}

void CKlkTehnickicrtezView::OnClick(CDC* pDC, CBitmap& bitmap)
{
	BITMAP bm;
	bitmap.GetBitmap(&bm);
	long dwCount = bm.bmWidth * bm.bmHeight * 4;
	unsigned char* lpBits = new unsigned char[dwCount];
	ZeroMemory(lpBits, dwCount);
	bitmap.GetBitmapBits(dwCount, lpBits);

	for (long i = 0; i < dwCount / 4; i++) {
		int offset = i * 4;
		unsigned char& R = lpBits[offset + 2]; // Crvena
		unsigned char& G = lpBits[offset + 1]; // Zelena
		unsigned char& B = lpBits[offset];     // Plava

		unsigned char temp = R;
		R = B;
		B = temp;		
	}

	bitmap.SetBitmapBits(dwCount, lpBits);
	delete[] lpBits;
}


void CKlkTehnickicrtezView::Translate(CDC* pDC, float dX, float dY)
{
	XFORM mat = { 1,0,0,1,dX,dY };
	pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}

void CKlkTehnickicrtezView::Rotate(CDC* pDC, float angle)
{
	angle *= pi / 180;
	XFORM mat = { cos(angle),sin(angle),-sin(angle),cos(angle),0,0 };
	pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}


// CKlkTehnickicrtezView printing

BOOL CKlkTehnickicrtezView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlkTehnickicrtezView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlkTehnickicrtezView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKlkTehnickicrtezView diagnostics

#ifdef _DEBUG
void CKlkTehnickicrtezView::AssertValid() const
{
	CView::AssertValid();
}

void CKlkTehnickicrtezView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlkTehnickicrtezDoc* CKlkTehnickicrtezView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlkTehnickicrtezDoc)));
	return (CKlkTehnickicrtezDoc*)m_pDocument;
}
#endif //_DEBUG


// CKlkTehnickicrtezView message handlers


void CKlkTehnickicrtezView::OnLButtonDown(UINT nFlags, CPoint point)
{

	CView::OnLButtonDown(nFlags, point);
	CRect clientRect;
	GetClientRect(&clientRect);

	if (clientRect.PtInRect(point))
	{
		if (!clicked)
			clicked = true;
		else
			clicked = false;
		Invalidate();
	}
}


void CKlkTehnickicrtezView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CKlkTehnickicrtezView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}
