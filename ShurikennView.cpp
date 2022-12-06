
// ShurikennView.cpp : implementation of the CShurikennView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Shurikenn.h"
#endif

#include "ShurikennDoc.h"
#include "ShurikennView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define pi 3.14159


// CShurikennView

IMPLEMENT_DYNCREATE(CShurikennView, CView)

BEGIN_MESSAGE_MAP(CShurikennView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CShurikennView construction/destruction

CShurikennView::CShurikennView() noexcept
{
	sky = new DImage();
	bark = new DImage();
	sky->Load(CString("sky.jpg"));
	bark->Load(CString("bark.jpg"));
	dx = 0;
	angle = 0;
}

CShurikennView::~CShurikennView()
{
	delete sky;
	delete bark;
}
float CShurikennView::ToRad(float angle)
{
	return angle * pi / 180;
}
void CShurikennView::DrawBlade(CDC* pDC, int size)
{
	CPen* old_pen;
	CPen grey_pen(PS_SOLID, 1, RGB(228, 228, 228));
	old_pen = pDC->SelectObject(&grey_pen);

	CBrush* old_brush;
	CBrush dark_grey_brush(RGB(198, 198, 198)), grey_brush(RGB(228, 228, 228));

	old_brush = pDC->SelectObject(&grey_brush);

	CPoint points1[3];
	points1[0].x = 0;
	points1[0].y = 0;

	points1[1].x = size;
	points1[1].y = -size;

	points1[2].x = 4*size;
	points1[2].y = 0;

	pDC->Polygon(points1,3);

	old_brush = pDC->SelectObject(&dark_grey_brush);

	CPoint points2[3];
	points2[0].x = 0;
	points2[0].y = 0;

	points2[1].x = size;
	points2[1].y = size;

	points2[2].x = 4 * size;
	points2[2].y = 0;

	pDC->Polygon(points2, 3);

	
	pDC->SelectObject(&old_brush);
	pDC->SelectObject(&old_pen);

	old_brush->DeleteObject();
	old_pen->DeleteObject();

}
//Krak je dužine 4 * size, visine 2 * size


void CShurikennView::Translate(CDC* pDC, double x, double y)
{
	XFORM t = { 1,0,0,1,x,y };

	pDC->ModifyWorldTransform(&t, MWT_LEFTMULTIPLY);
}

void CShurikennView::Rotate(CDC* pDC, float angle)
{
	XFORM t = { cos(ToRad(angle)),sin(ToRad(angle)),-sin(ToRad(angle)),cos(ToRad(angle)),0,0 };

	pDC->ModifyWorldTransform(&t, MWT_LEFTMULTIPLY);
}

void CShurikennView::Text(CDC* pDC, int size)
{
	CFont* font = new CFont();
	COLORREF clrOld = pDC->SetTextColor(RGB(255, 255, 255));
	int nOldMode = pDC->SetBkMode(TRANSPARENT);
	CString szMsg = (CString("Shuricane"));

	font->CreateFontW(size, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, (CString("Monotype Corsiva")));

	CFont* old = pDC->SelectObject(font);

	pDC->TextOutW(size - 2, -size / 2 - 2, szMsg);

	pDC->SetTextColor(RGB(50, 50, 50));
	pDC->TextOutW(size, -size / 2, szMsg);

	pDC->SetTextColor(clrOld);
	pDC->SetBkMode(nOldMode);

	pDC->SelectObject(old);

	font->DeleteObject();
	delete font;
}
void CShurikennView::DrawStar(CDC* pDC, int size)
{
	DrawBlade(pDC, size);
	Text(pDC, 0.7 * size);
	Rotate(pDC, -90);
	DrawBlade(pDC, size);
	Text(pDC, 0.7 * size);
	Rotate(pDC, -90);
	DrawBlade(pDC, size);
	Text(pDC, 0.7 * size);
	Rotate(pDC, -90);
	DrawBlade(pDC, size);
	Text(pDC, 0.7 * size);
	Rotate(pDC, -90);//vracam se na staro, moglo je i sa modifyworldtransform
}



BOOL CShurikennView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CShurikennView drawing

void CShurikennView::OnDraw(CDC* pDC)
{
	CShurikennDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	XFORM t;
	CRect klijent;
	GetClientRect(&klijent);

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);

	CBitmap b;
	b.CreateCompatibleBitmap(pDC, klijent.Width(), klijent.Height());

	memDC->SelectObject(&b);

	memDC->SetGraphicsMode(GM_ADVANCED);
	sky->Draw(memDC, CRect(0, 0, sky->Width(), sky->Height()), klijent);

	memDC->GetWorldTransform(&t);

	Translate(memDC, 100 + dx, 200);//100,200 je pocetna pozicija
	Rotate(memDC, angle);
	DrawStar(memDC, 240/4); //320 je precnik zvezde, smanjila sam da bude 240

	memDC->SetWorldTransform(&t);
	//drvo se crta nakon shurikena
	bark->Draw(memDC, CRect(0, 0, bark->Width(), bark->Height()), CRect(klijent.right - bark->Width(), 0, klijent.right, klijent.bottom));

	pDC->BitBlt(0, 0, klijent.Width(), klijent.Height(), memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC();
	// TODO: add draw code for native data here
}


// CShurikennView printing

BOOL CShurikennView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CShurikennView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CShurikennView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CShurikennView diagnostics

#ifdef _DEBUG
void CShurikennView::AssertValid() const
{
	CView::AssertValid();
}

void CShurikennView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CShurikennDoc* CShurikennView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CShurikennDoc)));
	return (CShurikennDoc*)m_pDocument;
}

#endif //_DEBUG


// CShurikennView message handlers


void CShurikennView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'W') {
		CRect rect;
		GetClientRect(&rect);

		double ten_percent = 240 / 100; //10% precnika
		double tree_beginning = rect.Width();
		tree_beginning -= bark->Width(); //gde pocinje drvo
		
		if (tree_beginning + ten_percent >= dx + 240*3/4 + 100 ) 
		{
			dx += 20;
			angle += 10;
			Invalidate();
		}
		Invalidate();

	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CShurikennView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}
