
// Klk2016(Bilijar)View.cpp : implementation of the CKlk2016BilijarView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk2016(Bilijar).h"
#endif

#include "Klk2016(Bilijar)Doc.h"
#include "Klk2016(Bilijar)View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define pi 3.14159


// CKlk2016BilijarView

IMPLEMENT_DYNCREATE(CKlk2016BilijarView, CView)

BEGIN_MESSAGE_MAP(CKlk2016BilijarView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKlk2016BilijarView construction/destruction

CKlk2016BilijarView::CKlk2016BilijarView() noexcept
{
	// TODO: add construction code here
	felt2 = new DImage();
	felt2->Load(CString("felt2.jpg"));
	wood = new DImage();
	wood->Load(CString("wood.jpg"));
	ballRotat = moveStick = moveBall = 0.0f;

}

CKlk2016BilijarView::~CKlk2016BilijarView()
{
	if (felt2) delete felt2; felt2 = nullptr;
	if (wood) delete wood; wood = nullptr;

}

BOOL CKlk2016BilijarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlk2016BilijarView drawing

void CKlk2016BilijarView::OnDraw(CDC* pDC)
{
	CKlk2016BilijarDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect clientRect;
	GetClientRect(&clientRect);
	CBitmap bmp;
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, clientRect.Width(), clientRect.Height());
	memDC->SelectObject(&bmp);
	XFORM oldForm;
	memDC->GetWorldTransform(&oldForm);

	memDC->SetGraphicsMode(GM_ADVANCED);
	DrawTable(memDC, clientRect);
	DrawBorder(memDC, clientRect, 40);
	DrawHoles(memDC, clientRect, 35);

	/*Nacrtati kuglu prečnika 30, sa centrom u (600, 400) i štap tako da dodiruje kuglu i usmeren
je ka njenom centru. Pritiskom na kursorske tastere  i  omogućiti rotaciju štapa oko
centra kugle, a tasterima  i  pomeranje po uzdužnoj osi. Kada štap dodirne kuglu (pri
pritisku ), on se zaustavlja, a kugla nastavlja da se kreće u pravcu štapa (uz dalje
pritiskanje ). Štap može nakon toga da se rotira, ali kugla mora da nastavi zadatim
pravcem. Pritiskom na , kugla se vraća ka početnoj poziciji. Kada je dostigne, prestaje da
se kreće, a štap nastavlja da se udaljava od nje (vidi aplikaciju)*/
	Translate(memDC, 600, 400, false);
	Rotate(memDC, ballRotat, false);
	Translate(pDC, moveBall, moveBall, false);
	DrawBall(memDC, 30);
	Translate(memDC, -600 - 15 + moveStick, 0, false);
	//Scale(memDC, 1, -1, false);
	DrawStick(memDC, 600);
	Translate(memDC, 600 + 15, 0, false);
	memDC->SetWorldTransform(&oldForm);

	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC();
	// TODO: add draw code for native data here
}

void CKlk2016BilijarView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float radAngle = angle * pi / 180;
	XFORM t = { cos(radAngle), sin(radAngle), -sin(radAngle), cos(radAngle),0,0 };

	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKlk2016BilijarView::Translate(CDC * pDC, float dX, float dY, bool rightMultiply)
{
	XFORM t = { 1,0,0,1 ,dX,dY };

	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}
void CKlk2016BilijarView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM t = { sX,0,0,sY ,0,0 };

	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CKlk2016BilijarView::DrawStick(CDC* pDC, int w)
{
	CBrush brownBrush(RGB(64, 32, 0));
	CBrush yellowBrush(RGB(255, 190, 128));
	CPen brownPen(PS_SOLID, 1, RGB(64, 32, 0));

	CBrush* oldBrush = pDC->SelectObject(&brownBrush);
	CPen* oldPen = pDC->SelectObject(&brownPen);

	POINT brownPart[] = { 
		CPoint(0,-w / 100), 
		CPoint(0,w / 100), 
		CPoint(w / 3, 2 * w / 300),
		CPoint(w / 3, -2 * w / 300) 
	};
	pDC->Polygon(brownPart, 4);
	pDC->Ellipse(CRect(-w / 100, -w / 100, w / 100, w / 100));

	pDC->SelectObject(yellowBrush);
	POINT yellowPart[] = {
		CPoint(w / 3, 2 * w / 300),
		CPoint(w / 3, -2 * w / 300),
		CPoint(w,-w / 200),
		CPoint(w,w / 200)
	};
	pDC->Polygon(yellowPart, 4);

	pDC->SelectStockObject(WHITE_PEN);
	int offset = 2;
	pDC->MoveTo(0 + offset, -w / 100 + offset);
	pDC->LineTo(w - offset, -w / 200 + offset);

	// dealoc
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
	brownBrush.DeleteObject(); 
	yellowBrush.DeleteObject(); 
	brownPen.DeleteObject();
}
void CKlk2016BilijarView::DrawBall(CDC* pDC, int w)
{
	CBrush red(RGB(128, 0, 0));
	CPen darkRed(PS_SOLID, 1, RGB(74, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(&red);
	CPen* oldPen = pDC->SelectObject(&darkRed);

	pDC->Ellipse(CRect(-w / 2, -w / 2, w / 2, w / 2));

	pDC->SelectStockObject(WHITE_PEN);
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->Ellipse(CRect(0, -w / 3, w / 3, 0));
	// dealoc
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
	red.DeleteObject();
	darkRed.DeleteObject();
}
void CKlk2016BilijarView::DrawTable(CDC* pDC, CRect rect)
{
	/*DrawTable(CDC* pDC, CRect rect), koja iscrtava površinu bilijarskog stola
korišćenjem bitmape felt2.jpg. Bitmapu učitati klasom DImage. Dimenzije slike su znatno
manje od dimenzija prozora, zato je neophodno višestruko iscrtati istu sliku, bez rastezanja.
Voditi računa da se slika učitava samo jednom, a ne pri svakom osvežavanju prikaza.*/

	CRect rcImg(0, 0, felt2->Width(), felt2->Height());
	int w = ceil(static_cast<double>(rect.Width()) / felt2->Width());
	int h = ceil(static_cast<double>(rect.Height()) / felt2->Height());
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			felt2->Draw(pDC, rcImg,CRect(i * felt2->Width(), j * felt2->Width(), (i + 1) * felt2->Width(), (j + 1) * felt2->Height()));
		}
	}
}
void CKlk2016BilijarView::DrawBorder(CDC* pDC, CRect rect, int w)
{
	wood->Draw(pDC, CRect(0, 0, rect.Width(), w),		CRect(0, 0, rect.Width(), w));
	wood->Draw(pDC, CRect(0, w, w, rect.Height() - w), CRect(0, w, w, rect.Height() - w));
	wood->Draw(pDC, CRect(0, rect.Height() - w, rect.Width(), rect.Height()), CRect(0, rect.Height() - w, rect.Width(), rect.Height()));
	wood->Draw(pDC, CRect(rect.Width() - w, w, rect.Width(), rect.Height() - w), CRect(rect.Width() - w, w, rect.Width(), rect.Height() - w));
}
//void CKlk2016BilijarView::DrawTable(CDC* pDC, CRect rect)
//{
//
//	int pieceW = rect.Width() / felt2->Width();
//	int pieceH = rect.Height() / felt2->Height();
//	
//
//	for (int i = 0; i <= pieceW; i++)
//	{
//		for (int j = 0; j <= pieceH; j++)
//		{
//			felt2->Draw(pDC, CRect(0, 0, felt2->Width(), felt2->Height()),
//				CRect(i * felt2->Width(), j * felt2->Height(), (i + 1) * felt2->Width(), (j + 1) * felt2->Height()));
//		}
//	}
//	
//}



//void CKlk2016BilijarView::DrawBorder(CDC* pDC, CRect rect, int w)
//{
//	//int wide = rect.Width() - wood->Width(); wide++;// koliko ide u sirinu
//	//int hi = rect.Height() - wood->Height(); hi++;
//	//for (int i = 0; i < wide; i++)
//	//{
//	//	wood->Draw(pDC, CRect(0, 0, wood->Width(), wood->Height()),
//	//		CRect(i * wood->Width(), 0, (i + 1) * wood->Width(), w)); //gornja
//	//	wood->Draw(pDC, CRect(0, 0, wood->Width(), wood->Height()),
//	//		CRect(i * wood->Width(), rect.Width() - w, (i + 1) * wood->Width(), rect.Width()));
//	//}
//
//	CRect topBorder(0, 0, rect.Width(), w);
//	CRect bottomBorder(0, rect.Height() - w, rect.Width(), rect.Height());
//	CRect leftBorder(0, w, w, rect.Height() - w);
//	CRect rightBorder(rect.Width() - w, w, rect.Width(), rect.Height() - w);
//	wood->Draw(pDC, topBorder, topBorder);
//	wood->Draw(pDC, bottomBorder, bottomBorder);
//	wood->Draw(pDC, leftBorder, leftBorder);
//	wood->Draw(pDC, rightBorder, rightBorder);
//
//}

void CKlk2016BilijarView::DrawHoles(CDC* pDC, CRect rect, int size)
{
	//iscrtava 6 rupa na stolu, prečnika size, odmaknutih za dužinu POLUprečnika od ivica stola(tj.prozora)
	pDC->SelectStockObject(BLACK_BRUSH);
	
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
}




// CKlk2016BilijarView printing

BOOL CKlk2016BilijarView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlk2016BilijarView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlk2016BilijarView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKlk2016BilijarView diagnostics

#ifdef _DEBUG
void CKlk2016BilijarView::AssertValid() const
{
	CView::AssertValid();
}

void CKlk2016BilijarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlk2016BilijarDoc* CKlk2016BilijarView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlk2016BilijarDoc)));
	return (CKlk2016BilijarDoc*)m_pDocument;
}
#endif //_DEBUG


// CKlk2016BilijarView message handlers


void CKlk2016BilijarView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	bool touched = moveStick >= 0;
	// TODO: Add your message handler code here and/or call default




	if (nChar == VK_RIGHT)
	{
		ballRotat += 10;
		Invalidate();
	}
	if (nChar == VK_LEFT)
	{
		ballRotat -= 10;
		Invalidate();
	}
	if (nChar == VK_UP)
	{
		if(!touched)
			moveStick += 10;
		if (touched)
		{
			moveBall += 20;

		}
		
		Invalidate();
	}
	if (nChar == VK_DOWN)
	{
		if (moveStick>0)
		{
			moveBall -= 10;
		}
		/*else
		{
			moveBall -= 10;
			moveStick += 10;
		}*/
		moveStick -= 10;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CKlk2016BilijarView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}
