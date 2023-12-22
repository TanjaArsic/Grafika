
// Klk2019(BilijarGLupi)View.cpp : implementation of the CKlk2019BilijarGLupiView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk2019(BilijarGLupi).h"
#endif

#include "Klk2019(BilijarGLupi)Doc.h"
#include "Klk2019(BilijarGLupi)View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define pi 3.14159

// CKlk2019BilijarGLupiView

IMPLEMENT_DYNCREATE(CKlk2019BilijarGLupiView, CView)

BEGIN_MESSAGE_MAP(CKlk2019BilijarGLupiView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKlk2019BilijarGLupiView construction/destruction

CKlk2019BilijarGLupiView::CKlk2019BilijarGLupiView() noexcept
{
	// TODO: add construction code here
	felt2 = new DImage();
	felt2->Load(CString("felt2.jpg"));
	wood = new DImage();
	wood->Load(CString("wood.jpg"));
	stickRotat = stickMove = ballMove = ballRotat= 0.0f;
}

CKlk2019BilijarGLupiView::~CKlk2019BilijarGLupiView()
{
	if (felt2) delete felt2; felt2 = nullptr;
	if (wood) delete wood; wood = nullptr;
}

BOOL CKlk2019BilijarGLupiView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlk2019BilijarGLupiView drawing

void CKlk2019BilijarGLupiView::OnDraw(CDC* pDC)
{
	CKlk2019BilijarGLupiDoc* pDoc = GetDocument();
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
	XFORM xform;
	memDC->GetWorldTransform(&xform);
	// TODO: add draw code for native data here
	DrawTable(memDC, c);
	DrawBorder(memDC, c, 40);
	DrawHoles(memDC, c, 35);

	XFORM ball;
	memDC->GetWorldTransform(&ball);
	Translate(memDC, 600, 400, false);
	Rotate(memDC, ballRotat, false);
	Translate(memDC, ballMove, 0, false);
	DrawBall(memDC, 30);
	memDC->SetWorldTransform(&ball);

	Translate(memDC, 600, 400, false);
	Rotate(memDC, stickRotat, false);
	Translate(memDC, -600 - 15 + stickMove, 0, false);

	DrawStick(memDC, 600);
	Scale(memDC, 1, 1, false);
	memDC->SetWorldTransform(&xform);

	pDC->BitBlt(0, 0, c.Width(), c.Height(), memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC();
}

void CKlk2019BilijarGLupiView::DrawStick(CDC* pDC, int w)
{
	CBrush brownBrush(RGB(64, 32, 0));
	CBrush yellowBrush(RGB(255, 190, 128));
	CPen brownPen(PS_SOLID, 1, RGB(64, 32, 0));

	CBrush* oldBrush = pDC->SelectObject(&brownBrush);
	CPen* oldPen = pDC->SelectObject(&brownPen);

	POINT brownPart[] = { CPoint(0,-w / 100), CPoint(0,w / 100), CPoint(w / 3,2 * w / 300), CPoint(w / 3,-2 * w / 300) };
	pDC->Polygon(brownPart, 4);

	pDC->Ellipse(-w / 100, -w / 100, w / 100, w / 100);
	pDC->SelectObject(&yellowBrush);
	POINT yellowPart[] = { CPoint(w / 3,2 * w / 300), CPoint(w / 3,-2 * w / 300),CPoint(w,-w / 200), CPoint(w,w / 200) };
	pDC->Polygon(yellowPart, 4);

	pDC->SelectStockObject(WHITE_PEN);
	int offset = 2;
	pDC->MoveTo(offset, -w / 100 + offset);
	pDC->LineTo(w - offset, -w / 200 + offset);
	pDC->SelectObject(oldBrush); brownBrush.DeleteObject(); yellowBrush.DeleteObject();
	pDC->SelectObject(oldPen); brownPen.DeleteObject();
	
}

void CKlk2019BilijarGLupiView::DrawBall(CDC* pDC, int w)
{
	CBrush red(RGB(128, 0, 0));
	CPen darkRed(PS_SOLID, 1, RGB(74, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(&red);
	CPen* oldPen = pDC->SelectObject(&darkRed);

	pDC->Ellipse(-w / 2, -w / 2, w / 2, w / 2);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->SelectStockObject(WHITE_PEN);
	w /= 3;
	pDC->Ellipse(0, -w, w, 0);
	pDC->SelectObject(oldPen); darkRed.DeleteObject();
	pDC->SelectObject(oldBrush); red.DeleteObject();

}

void CKlk2019BilijarGLupiView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM mat = { 1,0,0,1,dX,dY };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKlk2019BilijarGLupiView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	angle *= pi / 180;
	XFORM mat = { cos(angle),sin(angle),-sin(angle),cos(angle),0,0 };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKlk2019BilijarGLupiView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM mat = { sX,0,0,sY,0,0 };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKlk2019BilijarGLupiView::DrawTable(CDC* pDC, CRect rect)
{
	int pieceW = rect.Width() / felt2->Width(); 
	int pieceH = rect.Height() / felt2->Height();
	
	for (int i = 0; i <= pieceW; i++)
	{
		for (int j = 0; j <= pieceH; j++)
		{
			felt2->Draw(pDC, CRect(0, 0, felt2->Width(), felt2->Height()),
				CRect(i * felt2->Width(), j * felt2->Height(), (i + 1) * felt2->Width(), (j + 1) * felt2->Height()));

		}
	}
}

void CKlk2019BilijarGLupiView::DrawBorder(CDC* pDC, CRect rect, int w)
{
	CRect topRect(0, 0, rect.Width(), w);
	CRect bottomRect(0, rect.Height() - w, rect.Width(), rect.Height());
	CRect leftRect(0, w, w, rect.Height() - w);
	CRect rightRect(rect.Width() - w, w, rect.Width(), rect.Height() - w);

	wood->Draw(pDC, topRect, topRect);
	wood->Draw(pDC, bottomRect, bottomRect);
	wood->Draw(pDC, leftRect, leftRect);
	wood->Draw(pDC, rightRect, rightRect);
}

void CKlk2019BilijarGLupiView::DrawHoles(CDC* pDC, CRect rect, int size)
{
	//iscrtava 6 rupa na stolu, prečnika size, odmaknutih za dužinu POLUprečnika od ivica stola(tj.prozora)
	CGdiObject* old = pDC->SelectStockObject(BLACK_BRUSH);

	//prva
	//pDC->Ellipse(size * sqrt(2) / 2, size * sqrt(2) / 2, rect.left + 2 * size, rect.top + 2 * size);
	pDC->Ellipse(size / 2, size / 2, size / 2 + size, size / 2 + size);

	//druga
	//pDC->Ellipse(rect.Width() / 2 - size / 2, size, rect.Width() / 2 + size / 2, size + size);
	pDC->Ellipse(rect.Width() / 2 - size / 2, size / 2, rect.Width() / 2 + size / 2, size / 2 + size);

	//treca
	//pDC->Ellipse(rect.Width() - size * sqrt(2) / 2 - size, size * sqrt(2) / 2, rect.Width() - size * sqrt(2) / 2, 2 * size);
	pDC->Ellipse(rect.Width() - size - size / 2, size / 2, rect.Width() - size / 2, size / 2 + size);

	//cetvrta
	pDC->Ellipse(size / 2, rect.Height() - size / 2 - size, size / 2 + size, rect.Height() - size / 2);

	//peta
	pDC->Ellipse(rect.Width() / 2 - size / 2, rect.Height() - size / 2 - size, rect.Width() / 2 + size / 2, rect.Height() - size / 2);

	//sesta
	pDC->Ellipse(rect.Width() - size / 2 - size, rect.Height() - size / 2 - size, rect.Width() - size / 2, rect.Height() - size / 2);
	
	pDC->SelectObject(old);
}



// CKlk2019BilijarGLupiView printing

BOOL CKlk2019BilijarGLupiView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlk2019BilijarGLupiView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlk2019BilijarGLupiView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKlk2019BilijarGLupiView diagnostics

#ifdef _DEBUG
void CKlk2019BilijarGLupiView::AssertValid() const
{
	CView::AssertValid();
}

void CKlk2019BilijarGLupiView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlk2019BilijarGLupiDoc* CKlk2019BilijarGLupiView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlk2019BilijarGLupiDoc)));
	return (CKlk2019BilijarGLupiDoc*)m_pDocument;
}
#endif //_DEBUG


// CKlk2019BilijarGLupiView message handlers


BOOL CKlk2019BilijarGLupiView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}


void CKlk2019BilijarGLupiView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_UP )
	{
		if (stickMove == 0 && ballMove == 0)
			ballRotat = stickRotat;
		//if (stickMove != 0)
		//	stickMove += 10;
		if (stickMove == 0)
			ballMove += 10;
		if (stickMove < 0)
			stickMove += 10;

		Invalidate();
	}
	if (nChar == VK_DOWN)
	{
		if (ballMove == 0)
			stickMove -= 10;
		if (stickMove >= 0)
			ballMove -= 10;

		Invalidate();
	}
	if (nChar == VK_RIGHT )
	{
		stickRotat += 10;
		Invalidate();
	}
	if (nChar == VK_LEFT)
	{
		stickRotat -= 10;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
