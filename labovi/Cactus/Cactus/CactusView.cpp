
// CactusView.cpp : implementation of the CCactusView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Cactus.h"
#endif

#include "CactusDoc.h"
#include "CactusView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define pi 3.141592653589793


// CCactusView

IMPLEMENT_DYNCREATE(CCactusView, CView)

BEGIN_MESSAGE_MAP(CCactusView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

// CCactusView construction/destruction

CCactusView::CCactusView() noexcept : dim(500), numOfSquares(20)
{
	// TODO: add construction code here
	this->grid = dim/numOfSquares;
	this->drGrid = true;
	this->radius = 1.5 * this->grid;
	this->angle = 0;
	this->angle2 = 45;

	light_emf = GetEnhMetaFile(CString("cactus_part_light.emf"));
	dark_emf = GetEnhMetaFile(CString("cactus_part.emf"));

}

CCactusView::~CCactusView()
{
}

BOOL CCactusView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CCactusView drawing

void CCactusView::OnDraw(CDC* pDC)
{
	CCactusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	DrawBackground(pDC);
	DrawCactus(pDC);
	DrawGrid(pDC);

}
double CCactusView::ToRadians(double deg)
{
	return deg * pi / 180;
}
double CCactusView::ToDegrees(double rad)
{
	return rad * 180 / pi;
}

void CCactusView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM t = { 1,0,0,1,dX,dY };
	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CCactusView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM t = { cos(ToRadians(angle)),sin(ToRadians(angle)),-sin(ToRadians(angle)),cos(ToRadians(angle)),0,0 };
	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CCactusView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM t = { sX,0,0,sY,0,0 };
	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CCactusView::DrawBackground(CDC* pDC)
{
	pDC->SelectStockObject(NULL_PEN);
	CBrush* bkBrush = new CBrush(RGB(136, 206, 236));
	CBrush* oldBrush = pDC->SelectObject(bkBrush);

	POINT dimensions[] = { CPoint(0,0), CPoint(0,this->dim),
		CPoint(500,this->dim), CPoint(this->dim,0) };

	pDC->Polygon(dimensions, 4);

	pDC->SelectObject(oldBrush);
	delete bkBrush;

}

void CCactusView::DrawCactus(CDC* pDC)
{
	// kreiranje olovke i cetke za crtanje kruga
	CPen* drawingPen = new CPen(PS_SOLID, 1, RGB(34, 31, 32));
	CPen* oldPen = pDC->SelectObject(drawingPen);
	CBrush* greenBrush = new CBrush(RGB(51, 204, 51));
	CBrush* oldBrush = pDC->SelectObject(greenBrush);

	// odakle se crta:
	XFORM oldForm, rightSide, middle;
	pDC->GetWorldTransform(&oldForm);

	Translate(pDC, 10 * this->grid, 17 * this->grid, false); // pozicionira se na prvi krug skroz dole
	Translate(pDC, 0, -this->radius, false);
	DrawPartCactus(pDC, fat, dark); // prvi debeli
	Translate(pDC, 0, -this->radius, false);

	Rotate(pDC, -45, false);
	Translate(pDC, 0, -this->radius, false);
	DrawPartCactus(pDC, skinny, dark); // prvi skinny
	Translate(pDC, 0, this->radius, false);
	Rotate(pDC, 45, false);

	/************ sredina ************/
	pDC->GetWorldTransform(&middle);
	Translate(pDC, 0, -this->radius, false);
	DrawPartCactus(pDC, skinny, dark); // drugi skinny
	Translate(pDC, 0, -this->radius, false);

	Rotate(pDC, -45, false);
	Translate(pDC, 0, -this->radius, false);
	DrawPartCactus(pDC, norm, dark); // levi osrednji
	Translate(pDC, 0, this->radius, false);
	Rotate(pDC, 90, false);
	Translate(pDC, 0, -this->radius, false);
	DrawPartCactus(pDC, norm, dark); // desni osrednji
	Translate(pDC, 0, - 2 * this->radius, false);
	DrawPartCactus(pDC, fat, dark); // gornji debeli
	Translate(pDC, 0, this->radius, false);
	DrawCircle(pDC);
	Translate(pDC, 0, 2 * this->radius, false);
	DrawCircle(pDC);

	pDC->SetWorldTransform(&middle);

	/************ desna strana ************/
	pDC->GetWorldTransform(&rightSide);
	Rotate(pDC, this->angle2, false); // po default angle2 = 45°
	//Rotate(pDC, 45, false); // ovo ako je angle2 = 0 u konstruktoru, a ne 45
	Translate(pDC, 0, -this->radius, false);
	DrawPartCactus(pDC, skinny, light); // treci skinny
	Translate(pDC, 0, -this->radius, false);

	Rotate(pDC, 45, false);
	Translate(pDC, 0, -this->radius, false);
	DrawPartCactus(pDC, skinny, dark); // treci treci skinny
	Translate(pDC, 0, this->radius, false);
	Rotate(pDC, -45, false);
	Translate(pDC, 0, -this->radius, false);
	DrawPartCactus(pDC, skinny, dark); // treci drugi skinny
	Translate(pDC, 0, this->radius, false);
	Rotate(pDC, -45, false);
	Translate(pDC, 0, -this->radius, false);
	DrawPartCactus(pDC, skinny, dark); // treci prvi skinny
	Translate(pDC, 0, this->radius, false);
	DrawCircle(pDC);


	Translate(pDC, 0, -2 * this->radius, false);
	Rotate(pDC, this->angle, false);
	Translate(pDC, 0, -this->radius, false);
	DrawPartCactus(pDC, fat, light); //debeli svetli
	Translate(pDC, 0, this->radius, false);
	DrawCircle(pDC);

	pDC->SetWorldTransform(&rightSide);
	DrawCircle(pDC);
	Translate(pDC, 0, 2 * this->radius, false);
	DrawCircle(pDC);

	pDC->SetWorldTransform(&oldForm);

	DrawPot(pDC);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	delete drawingPen, oldBrush;
}

void CCactusView::DrawCircle(CDC* pDC)
{
	float radius = 0.5;
	pDC->Ellipse(
		CRect(CPoint(-radius * this->grid, -radius * this->grid),
			CPoint(radius * this->grid, radius * this->grid))
	);
}

void CCactusView::DrawPartCactus(CDC* pDC, type t, color color)
{
	CRect playRect;
	switch (t)
	{
	case fat:
		playRect = CRect(CPoint(-this->radius, -this->radius),
			CPoint(this->radius, this->radius));
		break;
	case norm:
		playRect = CRect(CPoint((-0.5 * this->radius), -this->radius),
			CPoint((0.5 * this->radius), this->radius));
		break;
	case skinny:
		playRect = CRect(CPoint((-0.3 * this->radius), -this->radius),
			CPoint((0.3 * this->radius), this->radius));
		break;
	}

	switch (color)
	{
	case light:
		pDC->PlayMetaFile(light_emf, playRect);
		break;
	case dark:
		pDC->PlayMetaFile(dark_emf, playRect);
		break;
	}

	//DeleteEnhMetaFile(dark);
}

void CCactusView::DrawGrid(CDC* pDC)
{
	if (drGrid)
	{
		CPen* gridPen = new CPen(PS_SOLID, 1, RGB(238, 238, 238));
		CPen* oldP = pDC->SelectObject(gridPen);
		int x = 0, y = 0;
		while (x < this->dim)
		{
			pDC->MoveTo(x, y);
			pDC->LineTo(x, this->dim);
			x += this->grid;
		}
		while (y < this->dim)
		{
			pDC->MoveTo(0, y);
			pDC->LineTo(this->dim, y);
			y += this->grid;
		}
		pDC->SelectObject(oldP);
		delete gridPen;
	}
}
void CCactusView::DrawPot(CDC* pDC)
{
	CBrush* potBrush = new CBrush(RGB(222, 148, 0));
	CBrush* oldB = pDC->SelectObject(potBrush);

	POINT down[] = {
		CPoint(8 * this->grid, 18 * this->grid), CPoint(12 * this->grid, 18 * this->grid),
		CPoint(11.6 * this->grid, 19.99 * this->grid), CPoint(8.4 * this->grid, 19.99 * this->grid)
	};
	CRect upperPart(7.6 * this->grid, 17.2 * this->grid, 12.4 * this->grid, 18.1 * this->grid);
	pDC->Polygon(down, 4);
	pDC->Rectangle(upperPart);

	pDC->SelectObject(oldB);
	delete potBrush;

}

// CCactusView printing

BOOL CCactusView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCactusView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCactusView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CCactusView diagnostics

#ifdef _DEBUG
void CCactusView::AssertValid() const
{
	CView::AssertValid();
}

void CCactusView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCactusDoc* CCactusView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCactusDoc)));
	return (CCactusDoc*)m_pDocument;
}
#endif //_DEBUG


// CCactusView message handlers


void CCactusView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_SPACE)
	{
		this->drGrid = !this->drGrid;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);

}


void CCactusView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_RIGHT)
	{
		this->angle += 30;
		Invalidate();
	}
	if (nChar == VK_LEFT)
	{
		this->angle -= 30;
		Invalidate();
	}
	if (nChar == 'D')
	{
		this->angle2 += 30;
		Invalidate();
	}
	if (nChar == 'A')
	{
		this->angle2 -= 30;
		Invalidate();
	}
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}
