
// Klk2017(AT-AT)View.cpp : implementation of the CKlk2017ATATView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk2017(AT-AT).h"
#endif
#include "Klk2017(AT-AT)Doc.h"
#include "Klk2017(AT-AT)View.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define pi 3.14159


// CKlk2017ATATView

IMPLEMENT_DYNCREATE(CKlk2017ATATView, CView)

BEGIN_MESSAGE_MAP(CKlk2017ATATView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKlk2017ATATView construction/destruction

CKlk2017ATATView::CKlk2017ATATView() noexcept
{
	// TODO: add construction code here
	leg1 = new DImage();
	leg1->Load(CString("Leg1.png"));
	leg2 = new DImage();
	leg2->Load(CString("Leg2.png"));
	leg3 = new DImage();
	leg3->Load(CString("Leg3.png"));
	body = new DImage();
	body->Load(CString("Body.png"));
	back2 = new DImage();
	back2->Load(CString("Back2.jpg"));

	enlarge = 1;
	position = dx = topCorner = leftCorner = 0;

}

CKlk2017ATATView::~CKlk2017ATATView()
{
	if (leg1) delete leg1; leg1 = nullptr;
	if (leg2) delete leg2; leg2 = nullptr;
	if (leg3) delete leg3; leg3 = nullptr;
	if (back2) delete back2; back2 = nullptr;
	if (body) delete body; body = nullptr;

}

BOOL CKlk2017ATATView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlk2017ATATView drawing

void CKlk2017ATATView::OnDraw(CDC* pDC)
{
	CKlk2017ATATDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientRect;
	GetClientRect(&clientRect);

	CBitmap bmp;
	XFORM stara;

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, clientRect.Width(), clientRect.Height());
	memDC->SelectObject(&bmp);

	memDC->SetGraphicsMode(GM_ADVANCED);
	DrawScene(memDC, clientRect);

	pDC->BitBlt(0, 0, clientRect.right, clientRect.bottom, memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC();
}

void CKlk2017ATATView::LoadIdentity(CDC* pDC)
{
	XFORM mat = { 1,0,0,1,0,0 };
	pDC->SetWorldTransform(&mat);
}

void CKlk2017ATATView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM mat = { 1,0,0,1,dX,dY };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

float CKlk2017ATATView::ToRad(float angle)
{
	return angle * pi / 180;
}
 
void CKlk2017ATATView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM mat = { cos(ToRad(angle)), sin(ToRad(angle)), -sin(ToRad(angle)), cos(ToRad(angle)) };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKlk2017ATATView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM mat = { sX, 0, 0, sY, 0 ,0 };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void CKlk2017ATATView::DrawScene(CDC* pDC, CRect rect)
{
	//back2->Draw(pDC, CRect(0, 0, back2->Width(), back2->Height()),
	//	CRect(leftCorner, topCorner, rect.right + leftCorner, rect.bottom + topCorner));
	DrawImgTransparent(pDC, back2, CRect(0, 0, back2->Width(), back2->Height()),
		CRect(leftCorner, topCorner, rect.right + leftCorner, rect.bottom + topCorner));

	XFORM atat;
	pDC->GetWorldTransform(&atat);
	Translate(pDC, 100 + position, 100, false);
	Scale(pDC, enlarge, enlarge, false);
	body->DrawTransparent(pDC, CRect(0, 0, body->Width(), body->Height()), CRect(0, 0, body->Width(), body->Height()));
	//-20 -10 0 10 20 10 0 - 10 
	DrawLeg(pDC, movement[(dx + 2) % 8], 55, 168); //72 kako da ne
	DrawLeg(pDC, movement[(dx + 6) % 8], 55, 168);
	DrawLeg(pDC, movement[(dx + 4) % 8], 270, 168);
	DrawLeg(pDC, movement[dx % 8], 270, 168);

	pDC->SetWorldTransform(&atat);

}
void CKlk2017ATATView::DrawLeg(CDC* pDC, double alpha, double dx, double dy)
{
	XFORM legs;
	pDC->GetWorldTransform(&legs);
	Translate(pDC, dx + 28, dy + 28, false);
	Rotate(pDC, alpha, false);
	Translate(pDC, - 28, - 28, false);

	//leg1->DrawTransparent(pDC, CRect(0, 0, leg1->Width(), leg1->Height()), CRect(0, 0, leg1->Width(), leg1->Height()), NULL);
	DrawTransparent(pDC, leg1, CRect(0, 0, leg1->Width(), leg1->Height()), CRect(0, 0, leg1->Width(), leg1->Height()));

	Translate(pDC, 29, 139 + 29, false);
	Rotate(pDC, -alpha, false);
	Translate(pDC, -29, -29, false);
	//leg2->DrawTransparent(pDC, CRect(0, 0, leg2->Width(), leg2->Height()), CRect(0, 0, leg2->Width(), leg2->Height()), NULL);
	DrawImgTransparent(pDC, leg2, CRect(0, 0, leg2->Width(), leg2->Height()), CRect(0, 0, leg2->Width(), leg2->Height()));

	Translate(pDC, -34, 95, false);
	
	leg3->DrawTransparent(pDC, CRect(0, 0, leg3->Width(), leg3->Height()), CRect(0, 0, leg3->Width(), leg3->Height()));
	//DrawImgTransparent(pDC, leg3, CRect(0, 0, leg3->Width(), leg3->Height()), CRect(0, 0, leg3->Width(), leg3->Height()));
	pDC->SetWorldTransform(&legs);
	
}

void CKlk2017ATATView::DrawImgTransparent(CDC* pDC, DImage* pImage, CRect srcRc, CRect dstRc)
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

}

void CKlk2017ATATView::DrawTransparent(CDC* pDC, DImage* pImage, CRect srcRc, CRect dstRc)
{
	int w = pImage->Width();
	int h = pImage->Height();
	BITMAP bm;
	CBitmap bitmap, bitmapMask;
	bitmap.CreateCompatibleBitmap(pDC, w, h);
	bitmapMask.CreateBitmap(w, h, 1, 1, NULL);
	bitmap.GetBitmap(&bm);

	CDC* srcDC = new CDC(); srcDC->CreateCompatibleDC(pDC);
	CDC* dstDC = new CDC();	dstDC->CreateCompatibleDC(pDC);
	srcDC->SelectObject(&bitmap);
	dstDC->SelectObject(&bitmapMask);
	pImage->Draw(srcDC, srcRc, dstRc);
	COLORREF topleft = srcDC->GetPixel(0, 0);
	COLORREF oldbkclr = srcDC->SetBkColor(topleft);	
	COLORREF oldtxtclr = srcDC->SetTextColor(RGB(255, 255, 255));
	dstDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, srcDC, 0, 0, SRCCOPY);
	srcDC->SetBkColor(RGB(0, 0, 0));
	srcDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, dstDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, dstDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, srcDC, 0, 0, SRCPAINT);

	dstDC->SetTextColor(oldtxtclr);
	srcDC->SetBkColor(oldbkclr);
	srcDC->DeleteDC();
	dstDC->DeleteDC();
	
}






//void CKlk2017ATATView::DrawScene(CDC* pDC, CRect rect)
//{
//	XFORM oldScene;
//	pDC->GetWorldTransform(&oldScene);
//	
//	CRect srcrect(leftCorner, topCorner, back2->Width() + leftCorner, back2->Height() + topCorner);
//	back2->Draw(pDC, srcrect, rect);
//	int i = 1;
//	Translate(pDC, 100 + position, 100, false);
//	Scale(pDC, enlarge, enlarge, false);
//	body->Draw(pDC, CRect(0, 0, body->Width(), body->Height()), CRect(0, 0, body->Width(), body->Height()));
//	DrawLeg(pDC, movement[(i * 0 + dx) % 8], 27.5, 89);
//	DrawLeg(pDC, movement[(i * 4 + dx) % 8], 27.5, 89);
//	DrawLeg(pDC, movement[(i * 6 + dx) % 8], 135, 89);
//	DrawLeg(pDC, movement[(i * 2 + dx) % 8], 135, 89);
//
//
//	pDC->SetWorldTransform(&oldScene);
//
//
//}




// CKlk2017ATATView printing

BOOL CKlk2017ATATView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlk2017ATATView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlk2017ATATView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKlk2017ATATView diagnostics

#ifdef _DEBUG
void CKlk2017ATATView::AssertValid() const
{
	CView::AssertValid();
}

void CKlk2017ATATView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlk2017ATATDoc* CKlk2017ATATView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlk2017ATATDoc)));
	return (CKlk2017ATATDoc*)m_pDocument;
}
#endif //_DEBUG


// CKlk2017ATATView message handlers


void CKlk2017ATATView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	CRect clientRect;
	GetClientRect(&clientRect);

	if (nChar == VK_UP)
	{
		enlarge += 0.1;
		Invalidate();
	}
	if (nChar == VK_DOWN)
	{
		if (enlarge > 0) enlarge -= 0.1;
		Invalidate();
	}
	if (nChar == VK_RIGHT)
	{
		position += 10;
		dx++;
		Invalidate();
	}
	/*if (nChar == VK_LEFT)
	{
		position -= 10;
		dx--;
		Invalidate();
	}*/
	/*if (nChar == 'A')
	{
		if (leftCorner >= 200)
			leftCorner -= 200;
		Invalidate();
	}
	if (nChar == 'D')
	{
		if (clientRect.Width() + leftCorner + 200 <= back2->Width())
			leftCorner += 200;
		Invalidate();
	}
	if (nChar == 'S')
	{
		if (clientRect.Height() + topCorner + 200 <= back2->Height())
			topCorner += 200;
		Invalidate();
	}
	if (nChar == 'W')
	{
		if (topCorner >= 200)
			topCorner -= 200;

		Invalidate();
	}*/

	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	
	//pomerati pozadinsku sliku 200 pixela
	CRect c;
	GetClientRect(&c);
	if (nChar == 'S')
	{
		topCorner += 200;
		Invalidate();
	}
	if (nChar == 'W')
	{
		if (topCorner > 0)
		{
			topCorner -= 200;
			Invalidate();
		}
	}
	if (nChar == 'A')
	{
		if (leftCorner > 0)
		{
			leftCorner -= 200;
			Invalidate();
		}
	}
	if (nChar == 'D')
	{
		leftCorner += 200;
		Invalidate();
	}

}


BOOL CKlk2017ATATView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return true;
	
}
