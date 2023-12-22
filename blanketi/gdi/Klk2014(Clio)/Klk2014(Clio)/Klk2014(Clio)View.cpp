
// Klk2014(Clio)View.cpp : implementation of the CKlk2014ClioView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk2014(Clio).h"
#endif

#include "Klk2014(Clio)Doc.h"
#include "Klk2014(Clio)View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define pi 3.14159

// CKlk2014ClioView

IMPLEMENT_DYNCREATE(CKlk2014ClioView, CView)

BEGIN_MESSAGE_MAP(CKlk2014ClioView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKlk2014ClioView construction/destruction

CKlk2014ClioView::CKlk2014ClioView() noexcept
{
	// TODO: add construction code here
	wheel = new DImage();
	wheel->Load(CString("Wheel.png"));
	clio = GetEnhMetaFile(CString("clio.emf"));
	angle = moveCar = wheelAngle = 0.0f;
}

CKlk2014ClioView::~CKlk2014ClioView()
{
	if (wheel) delete wheel; wheel = nullptr;
	DeleteEnhMetaFile(clio);
}

BOOL CKlk2014ClioView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlk2014ClioView drawing

void CKlk2014ClioView::OnDraw(CDC* pDC)
{
	CKlk2014ClioDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect client;
	GetClientRect(&client);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, client.Width(), client.Height());
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	memDC->SelectObject(&bmp);
	memDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldForm;
	memDC->GetWorldTransform(&oldForm);

	DrawGround(memDC, angle);
	Translate(memDC, 0, client.bottom, false);
	Rotate(memDC, -angle, false);
	Translate(memDC, 250 + moveCar, -108, false); // omogucava autu da se krece
	DrawCar(memDC, 0, 0, 450, 450 / 2.5);

	int r = 38;
	//float alpha = (180/pi) * moveCar / (2 * pi);
	float alpha = (moveCar / r) * 180 / pi;
	DrawWheel(memDC, - 155, 70, r, -alpha);
	DrawWheel(memDC, 135, 70, r, -alpha);

	memDC->SetWorldTransform(&oldForm);
	pDC->BitBlt(0, 0, client.Width(), client.Height(), memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC(); 
}

void CKlk2014ClioView::DrawGround(CDC* pDC, float angle)
{
	CRect rect;
	GetClientRect(&rect);
	
	CPen brownPen(PS_SOLID, 3, RGB(99, 39, 38));
	CBrush lightBlue(RGB(200, 220, 255));
	CBrush* oldBrush = pDC->SelectObject(&lightBlue);
	CBrush hatchBrush;
	hatchBrush.CreateHatchBrush(HS_FDIAGONAL, RGB(99, 39, 38));
	CGdiObject* old = pDC->SelectStockObject(NULL_PEN);
	pDC->Rectangle(rect); //plava pozadina

	CPen* oldPen = pDC->SelectObject(&brownPen);
	pDC->SetBkColor(RGB(236, 247, 192));
	pDC->SelectObject(&hatchBrush);
	POINT ground[] = { CPoint(rect.left,rect.bottom), CPoint(rect.right,rect.bottom), CPoint(rect.right, rect.bottom - rect.right * tan(angle * pi / 180)) };
	pDC->Polygon(ground, 3);

	pDC->SelectObject(old); brownPen.DeleteObject();
	pDC->SelectObject(oldBrush); lightBlue.DeleteObject(); hatchBrush.DeleteObject();

}

void CKlk2014ClioView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM mat = { 1,0,0,1,dX,dY };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKlk2014ClioView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	angle = ToRad(angle);
	XFORM mat = { cos(angle),sin(angle),-sin(angle),cos(angle),0,0 };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKlk2014ClioView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM mat = { sX,0,0,sY,0,0 };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

float CKlk2014ClioView::ToRad(float angle)
{
	return angle * pi / 180;
}

void CKlk2014ClioView::DrawCar(CDC* pDC, int x, int y, int w, int h)
{
	XFORM car;
	pDC->GetWorldTransform(&car);
	//centar je u tacki x,y, a prikazuje s sirinom w i visinom h
	Scale(pDC, -1, 1, false);
	pDC->PlayMetaFile(clio, CRect(x - w / 2, y - h / 2, x + w / 2, y + h / 2));
	pDC->SetWorldTransform(&car);

}

void CKlk2014ClioView::DrawWheel(CDC* pDC, int x, int y, int r, float angle)
{
	XFORM wheelF;
	pDC->GetWorldTransform(&wheelF);
	Translate(pDC, x, y, false);
	Rotate(pDC, wheelAngle, false);
	Translate(pDC, -r, -r, false);
	unsigned char* bajts = wheel->GetDIBBits();
	COLORREF color = RGB(bajts[2], bajts[1], bajts[0]);
	//wheel->DrawTransparent(pDC, CRect(52, 15, 52 + 376, 15 + 376), CRect(0, 0, 2 * r, 2 * r), NULL);
	DrawImgTransparent(pDC, wheel, CRect(52, 15, 52 + 376, 15 + 376), CRect(0, 0, 2 * r, 2 * r));
	pDC->SetWorldTransform(&wheelF);

}

void CKlk2014ClioView::DrawImgTransparent(CDC* pDC, DImage* pImage, CRect srcRc, CRect dstRc)
{
	int w = pImage->Width();
	int h = pImage->Height();

	CBitmap* bmpImage = new CBitmap();
	bmpImage->CreateCompatibleBitmap(pDC, w, h);

	CBitmap* bmpMask = new CBitmap();
	bmpMask->CreateBitmap(w, h, 1, 1, NULL);

	BITMAP* bm = new BITMAP();
	bmpImage->GetBitmap(bm);

	CDC* srcDC = new CDC();
	srcDC->CreateCompatibleDC(pDC);

	CDC* dstDC = new CDC();
	dstDC->CreateCompatibleDC(pDC);

	CBitmap* pOldSrcBmp = srcDC->SelectObject(bmpImage);
	CBitmap* pOldDstBmp = dstDC->SelectObject(bmpMask);

	pImage->Draw(srcDC, srcRc, dstRc);

	COLORREF clrTopLeft = srcDC->GetPixel(0, 0);
	COLORREF clrSaveBk = srcDC->SetBkColor(clrTopLeft);
	COLORREF clrSaveDstText = srcDC->SetTextColor(RGB(255, 255, 255));

	dstDC->BitBlt(0, 0, bm->bmWidth, bm->bmHeight, srcDC, 0, 0, SRCCOPY);
	srcDC->SetBkColor(RGB(0, 0, 0));
	srcDC->BitBlt(0, 0, bm->bmWidth, bm->bmHeight, dstDC, 0, 0, SRCAND);

	pDC->BitBlt(0, 0, bm->bmWidth, bm->bmHeight, dstDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, bm->bmWidth, bm->bmHeight, srcDC, 0, 0, SRCPAINT);

	dstDC->SetTextColor(clrSaveDstText);
	srcDC->SetBkColor(clrSaveBk);

	srcDC->SelectObject(pOldSrcBmp);
	dstDC->SelectObject(pOldDstBmp);

	srcDC->DeleteDC();
	dstDC->DeleteDC();
}

//void CKlk2014ClioView::DrawCar(CDC* pDC, int x, int y, int w, int h)
//{
//	//koja prikazuje metafajl
//	//clio.emf sa centrom u tački(x, y), širine w i visine h.Pri iscrtavanju automobil je „okrenut“
//		//na desnu stranu(vidi sliku).
//	Scale(pDC, -1, 1, false);
//	CRect clioRect(CPoint(x - w / 2, y - h / 2), CPoint(x + w / 2, y + h / 2));
//	pDC->PlayMetaFile(clio, clioRect);
//	Scale(pDC, 1, 1, false);
//
//}

//void CKlk2014ClioView::DrawWheel(CDC* pDC, int x, int y, int r, float angle)
//{
//	XFORM transOld;
//	pDC->GetWorldTransform(&transOld);
//	Translate(pDC, x, y, false);
//	Rotate(pDC, angle, false);
//	wheel->DrawTransparent(pDC, CRect(52, 15, 52 + 376, 15 + 376), CRect(r, r, -r, -r), NULL);
//	pDC->SetWorldTransform(&transOld);
//}


// CKlk2014ClioView printing

BOOL CKlk2014ClioView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlk2014ClioView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlk2014ClioView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKlk2014ClioView diagnostics

#ifdef _DEBUG
void CKlk2014ClioView::AssertValid() const
{
	CView::AssertValid();
}

void CKlk2014ClioView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlk2014ClioDoc* CKlk2014ClioView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlk2014ClioDoc)));
	return (CKlk2014ClioDoc*)m_pDocument;
}
#endif //_DEBUG


// CKlk2014ClioView message handlers


BOOL CKlk2014ClioView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}


void CKlk2014ClioView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_UP)
	{
		if (angle < 80) 
			angle += 10;
		Invalidate();
	}
	if (nChar == VK_DOWN)
	{
		if (angle > -10)
			angle -= 10;
		Invalidate();
	}
	if (nChar == VK_RIGHT)
	{
		moveCar += 10;
		wheelAngle += 10;
		Invalidate();
	}
	if (nChar == VK_LEFT)
	{
		moveCar -= 10;
		wheelAngle -= 10;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
