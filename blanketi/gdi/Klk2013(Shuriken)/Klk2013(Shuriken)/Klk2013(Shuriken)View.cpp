
// Klk2013(Shuriken)View.cpp : implementation of the CKlk2013ShurikenView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk2013(Shuriken).h"
#endif

#include "Klk2013(Shuriken)Doc.h"
#include "Klk2013(Shuriken)View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define pi 3.14159


// CKlk2013ShurikenView

IMPLEMENT_DYNCREATE(CKlk2013ShurikenView, CView)

BEGIN_MESSAGE_MAP(CKlk2013ShurikenView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKlk2013ShurikenView construction/destruction

CKlk2013ShurikenView::CKlk2013ShurikenView() noexcept
{
	// TODO: add construction code here
	sky = new DImage();
	sky->Load(CString("sky.jpg"));
	bark = new DImage();
	bark->Load(CString("bark.jpg"));
	moveShuriken = rotateShuriken = 0.0f;

}

CKlk2013ShurikenView::~CKlk2013ShurikenView()
{
	if (sky) delete sky; sky = nullptr;
	if (bark) delete bark; bark = nullptr;
}

BOOL CKlk2013ShurikenView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlk2013ShurikenView drawing

void CKlk2013ShurikenView::OnDraw(CDC* pDC)
{
	CKlk2013ShurikenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CBitmap bmp;
	CRect client;
	GetClientRect(&client);
	bmp.CreateCompatibleBitmap(pDC, client.Width(), client.Height());

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	memDC->SelectObject(&bmp);

	XFORM xf;
	memDC->SetGraphicsMode(GM_ADVANCED);
	memDC->GetWorldTransform(&xf);

	sky->Draw(memDC, CRect(0, 0, sky->Width(), sky->Height()), client);
	//bark->Draw(memDC, CRect(0, 0, bark->Width(), bark->Height()), CRect(client.Width() - bark->Width(), 0, client.Width(), client.Height()));

	float len = 320; 
	float tenPercent = 320 * 0.1;
	hit = 100 + moveShuriken + len/2 - tenPercent >= client.Width() - bark->Width();
	XFORM shur;
	memDC->GetWorldTransform(&shur);
	Translate(memDC, 100 + moveShuriken, 200);
	Rotate(memDC, rotateShuriken);
	DrawStar(memDC, len/8);
	memDC->SetWorldTransform(&shur);
	
	bark->Draw(memDC, CRect(0, 0, bark->Width(), bark->Height()), CRect(client.right - bark->Width(), client.top, client.right, client.bottom));

	// TODO: add draw code for native data here
	memDC->SetWorldTransform(&xf);
	pDC->BitBlt(0, 0, client.Width(), client.Height(), memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC();
}

void CKlk2013ShurikenView::Translate(CDC* pDC, double x, double y)
{
	XFORM mat = { 1,0,0,1,x,y };
	pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}

void CKlk2013ShurikenView::Rotate(CDC* pDC, float angle)
{
	angle *= pi / 180;
	XFORM mat = { cos(angle),sin(angle),-sin(angle),cos(angle),0,0 };
	pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}


void CKlk2013ShurikenView::DrawBlade(CDC* pDC, int size)
{
	//Jedna polovina kraka je sive a drugi tamno sive boje.
	// Okvirna linija je sive boje. Krak je dužine 4 * size, visine 2 * size
	CBrush grayBrush(RGB(224, 224, 224)); 
	CBrush dgrayBrush(RGB(192, 192, 192));
	CPen grayPen(PS_SOLID, 1, RGB(224, 224, 224));
	CBrush* oldBrush = pDC->SelectObject(&grayBrush);
	CPen* oldPen = pDC->SelectObject(&grayPen);

	POINT upperPart[] = { CPoint(0,0), CPoint(size,-size), CPoint(4 * size,0) };
	POINT lowerPart[] = { CPoint(0,0), CPoint(size,size), CPoint(4 * size,0) };
	pDC->Polygon(upperPart, 3);
	pDC->SelectObject(&dgrayBrush);
	pDC->Polygon(lowerPart, 3);

	// font
	CFont font;
	font.CreateFontW(0.7 * size, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CString("Monotype Corsiva"));
	CFont* oldFont = pDC->SelectObject(&font);
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);

	COLORREF oldTextColor = pDC->SetTextColor(RGB(255, 255, 255));
	pDC->TextOutW(size - 1, -(0.7 * size) / 2 - 1, CString("Shuricane"));

	pDC->SetTextColor(RGB(85, 85, 85));
	pDC->TextOutW(size, -(0.7 * size) / 2, CString("Shuricane"));

	pDC->SelectObject(oldPen);	grayPen.DeleteObject();
	pDC->SelectObject(oldBrush);	grayBrush.DeleteObject();	dgrayBrush.DeleteObject();
	pDC->SelectObject(oldFont);	font.DeleteObject();
	pDC->SetTextColor(oldTextColor);
	pDC->SetBkMode(oldBkMode);
}

void CKlk2013ShurikenView::DrawStar(CDC* pDC, int size)
{
	for (int i = 0; i < 4; i++)
	{
		DrawBlade(pDC, size);
		Rotate(pDC, -90);
	}
}



// CKlk2013ShurikenView printing

BOOL CKlk2013ShurikenView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlk2013ShurikenView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlk2013ShurikenView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKlk2013ShurikenView diagnostics

#ifdef _DEBUG
void CKlk2013ShurikenView::AssertValid() const
{
	CView::AssertValid();
}

void CKlk2013ShurikenView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlk2013ShurikenDoc* CKlk2013ShurikenView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlk2013ShurikenDoc)));
	return (CKlk2013ShurikenDoc*)m_pDocument;
}
#endif //_DEBUG


// CKlk2013ShurikenView message handlers


BOOL CKlk2013ShurikenView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}


void CKlk2013ShurikenView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CRect client;
	GetClientRect(&client);
	// TODO: Add your message handler code here and/or call default
	if (nChar == 'W' && !hit)
	{
		//if (100 + moveShuriken + 32 <= client.Width() - bark->Width())
		{
			moveShuriken += 20;
			rotateShuriken += 10;
			Invalidate();
		}
		
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
