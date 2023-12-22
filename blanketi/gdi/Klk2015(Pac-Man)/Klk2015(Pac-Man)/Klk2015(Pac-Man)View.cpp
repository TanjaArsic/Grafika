
// Klk2015(Pac-Man)View.cpp : implementation of the CKlk2015PacManView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk2015(Pac-Man).h"
#endif

#include "Klk2015(Pac-Man)Doc.h"
#include "Klk2015(Pac-Man)View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define pi 3.14159

// CKlk2015PacManView

IMPLEMENT_DYNCREATE(CKlk2015PacManView, CView)

BEGIN_MESSAGE_MAP(CKlk2015PacManView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKlk2015PacManView construction/destruction

CKlk2015PacManView::CKlk2015PacManView() noexcept
{
	// TODO: add construction code here
	blue = new DImage();
	blue->Load(CString("blue.png"));
	angle = 22.5f;
	moveX = moveY = 0.0f;
	direction = 0.0f;
	turn = 1;
	touched = false;
}

CKlk2015PacManView::~CKlk2015PacManView()
{
	if (blue) delete blue; blue = nullptr;
}

BOOL CKlk2015PacManView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlk2015PacManView drawing

void CKlk2015PacManView::OnDraw(CDC* pDC)
{
	CKlk2015PacManDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CBitmap bmp;
	XFORM xform;
	CRect client;
	GetClientRect(&client);


	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, client.Width(), client.Height());
	memDC->SelectObject(&bmp);
	memDC->SetGraphicsMode(GM_ADVANCED);
	memDC->GetWorldTransform(&xform);

	blue->Draw(memDC, CRect(0, 0, blue->Width(), blue->Height()), client);

	CRect ghostRect(100, 120, 196, 216);
	DrawGhost(memDC, ghostRect);
	
	Translate(memDC, 400 + moveX, 400 + moveY, false);
	int h = 100;
	Translate(memDC, h / 2, h / 2, false);
	Rotate(memDC, direction, false);
	Scale(memDC, 1, turn, false);
	Translate(memDC, -h / 2, -h / 2, false);
	CRect re(0, 0, h, h);
	DrawPacman(memDC, re, angle);
	memDC->SetWorldTransform(&xform);

	touched = (400 + moveX <= 196 && 400 + moveX >= 100) && (400 + moveY <= 216 && 400 + moveY >= 120);
	//touched = !((400 + moveX > 196 && 400 + moveY > 216) || (400 + moveX < 100 && 400 + moveY < 120));
	//touched = ghostRect.PtInRect(CPoint(400 + moveX, 400 + moveY));

	if (touched)
	{
		CFont font;
		font.CreateFontW(72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CString("Arial"));
		CFont* oldFont = memDC->SelectObject(&font);
		COLORREF oldBkColor = memDC->SetTextColor(RGB(255, 255, 255));
		memDC->SetTextAlign(TA_CENTER);
		//int oldBkMode = memDC->SetBkMode(TRANSPARENT);
		memDC->TextOutW(client.CenterPoint().x, client.CenterPoint().y, CString("The End"));

		memDC->SelectObject(oldFont); font.DeleteObject();
		//memDC->SetBkMode(oldBkMode);
		memDC->SetTextColor(oldBkColor);
	}
	// TODO: add draw code for native data here
	pDC->BitBlt(0, 0, client.Width(), client.Height(), memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC();

}
void CKlk2015PacManView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float radAngle = angle * pi / 180;
	XFORM t = { cos(radAngle), sin(radAngle), -sin(radAngle), cos(radAngle),0,0 };

	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKlk2015PacManView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM t = { 1,0,0,1 ,dX,dY };

	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}
void CKlk2015PacManView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM t = { sX,0,0,sY ,0,0 };

	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CKlk2015PacManView::DrawPacman(CDC* pDC, CRect rect, float angle)
{
	/*koja iscrtava Pacman-a žute boje upisanog u okvirni parvougaonik rect, sa ustima otvorenim za ugao 2*angle*/
	CBrush ylw(RGB(255, 255, 0));
	CBrush* oldY = pDC->SelectObject(&ylw);
	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->Pie(rect, 
		CPoint(rect.CenterPoint().x + rect.Width() / 2 * cos(angle * pi / 180), rect.CenterPoint().y + rect.Height() / 2 * sin(- angle * pi / 180)),
		CPoint(rect.CenterPoint().x + rect.Width() / 2 * cos(- angle * pi / 180), rect.CenterPoint().y + rect.Height() / 2 * sin(angle * pi / 180)));
	pDC->SelectStockObject(BLACK_BRUSH);
	pDC->Ellipse(rect.CenterPoint().x + 15, rect.CenterPoint().y - 35, rect.CenterPoint().x + 30, rect.CenterPoint().y - 20);
		
	pDC->SelectObject(oldY); ylw.DeleteObject();
}

void CKlk2015PacManView::DrawGhost(CDC* pDC, CRect rect)
{
	//pDC->BeginPath();

	pDC->SetArcDirection(AD_CLOCKWISE);
	CBrush redBrush(RGB(255, 0, 0));
	CBrush blueBrush(RGB(0, 0, 255));
	CBrush* oldBrush = pDC->SelectObject(&redBrush);
	pDC->SelectStockObject(WHITE_PEN);
	pDC->Arc(rect, CPoint(rect.left, rect.CenterPoint().y), CPoint(rect.right, rect.CenterPoint().y));
	pDC->MoveTo(rect.left, rect.CenterPoint().y);

	pDC->LineTo(rect.left, rect.bottom);
	pDC->MoveTo(rect.right, rect.CenterPoint().y);
	pDC->LineTo(rect.right, rect.bottom);

	//// idu mali lukovi
	float d = rect.Width() / 6.0;
	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->Arc(CRect(rect.left - d/2, rect.bottom - d, rect.left + d/2, rect.bottom), CPoint(rect.left, rect.bottom), CPoint(rect.left + d/2, rect.bottom - d/2));

	for (int i = 0; i < 5; i++)
	{
		i % 2 == 0 ? pDC->SetArcDirection(AD_CLOCKWISE) : pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
		pDC->Arc(CRect(rect.left + d / 2 + i * d, rect.bottom - d, rect.left + d / 2 + d + i * d, rect.bottom), CPoint(rect.left + d / 2 + i * d, rect.bottom - d / 2), CPoint(rect.left + d / 2 + d + i * d, rect.bottom - d / 2));
	}
	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->Arc(CRect(rect.right - d / 2, rect.bottom - d, rect.right + d / 2, rect.bottom), CPoint(rect.right - d / 2, rect.bottom - d / 2), CPoint(rect.right, rect.bottom));
	
	//pDC->EndPath();
	//pDC->StrokeAndFillPath();
	pDC->FloodFill(rect.left + 30, rect.top + 30, RGB(255, 255, 255));


	//// oci
	pDC->SelectStockObject(BLACK_PEN);
	pDC->SelectObject(oldBrush);
	pDC->Ellipse(CRect(rect.left + rect.Width() / 3 + 5, rect.top + rect.Height() / 3, rect.left + rect.Width() / 3 + 30, rect.top + rect.Height() / 3 + 23));
	pDC->Ellipse(CRect(rect.left + rect.Width() / 3 + 35, rect.top + rect.Height() / 3, rect.left + rect.Width() / 3 + 60, rect.top + rect.Height() / 3 + 23));

	pDC->SelectObject(&blueBrush);
	pDC->Ellipse(CRect(rect.left + rect.Width() / 3 + 15, rect.top + rect.Height() / 3 + 6, rect.left + rect.Width() / 3 + 27, rect.top + rect.Height() / 3 + 17));
	pDC->Ellipse(CRect(rect.left + rect.Width() / 3 + 15 + 30, rect.top + rect.Height() / 3 + 6, rect.left + rect.Width() / 3 + 27 + 30, rect.top + rect.Height() / 3 + 17));

	pDC->SelectObject(oldBrush);
	redBrush.DeleteObject();
	blueBrush.DeleteObject();

}


// CKlk2015PacManView printing

BOOL CKlk2015PacManView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlk2015PacManView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlk2015PacManView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKlk2015PacManView diagnostics

#ifdef _DEBUG
void CKlk2015PacManView::AssertValid() const
{
	CView::AssertValid();
}

void CKlk2015PacManView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlk2015PacManDoc* CKlk2015PacManView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlk2015PacManDoc)));
	return (CKlk2015PacManDoc*)m_pDocument;
}
#endif //_DEBUG



BOOL CKlk2015PacManView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}


void CKlk2015PacManView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_UP || nChar == VK_DOWN || nChar == VK_LEFT || nChar == VK_RIGHT)
	{
		if (this->angle != 0)
			angle = 0;
		else if (this->angle == 0)
			angle = 22.5f;
		Invalidate();
	}
	if (nChar == VK_UP)
	{
		turn = 1;
		direction = -90;
		moveY -= 10;
		Invalidate();
	}
	if (nChar == VK_DOWN)
	{
		turn = 1;
		direction = 90;
		moveY += 10;
		Invalidate();
	}
	if (nChar == VK_RIGHT)
	{
		turn = 1;
		direction = 0;
		moveX += 10;
		Invalidate();
	}
	if (nChar == VK_LEFT)
	{
		direction = 180;
		turn = -1;
		moveX -= 10;
		Invalidate();
	}
		

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
