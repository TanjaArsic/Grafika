
// IND_17549View.cpp : implementation of the CIND17549View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_17549.h"
#endif

#include "IND_17549Doc.h"
#include "IND_17549View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.14159


// CIND17549View

IMPLEMENT_DYNCREATE(CIND17549View, CView)

BEGIN_MESSAGE_MAP(CIND17549View, CView)
	ON_WM_KEYUP()
END_MESSAGE_MAP()

// CIND17549View construction/destruction

CIND17549View::CIND17549View() noexcept
{
	this->keyPressed = true;
	this->dimenzija = 500/25;
	// TODO: add construction code here
	light = GetEnhMetaFile(CString("cactus_part_light.emf"));
	dark = GetEnhMetaFile(CString("cactus_part.emf"));

}

CIND17549View::~CIND17549View()
{
}

BOOL CIND17549View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}


void CIND17549View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM forma = {
		1,
		0,
		0,
		1,
		dX,
		dY
	};

	if (rightMultiply) {
		pDC->ModifyWorldTransform(&forma, MWT_RIGHTMULTIPLY);
	}
	else {
		pDC->ModifyWorldTransform(&forma, MWT_LEFTMULTIPLY);
	}
}

void CIND17549View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM forma = {
		sX,
		0,
		0,
		sY,
		0,
		0
	};

	if (rightMultiply) {
		pDC->ModifyWorldTransform(&forma, MWT_RIGHTMULTIPLY);
	}
	else {
		pDC->ModifyWorldTransform(&forma, MWT_LEFTMULTIPLY);
	}
}

void CIND17549View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM forma = {
		cos(angle),
		sin(angle),
		-sin(angle),
		cos(angle),
		0,
		0
	};

	if (rightMultiply) {
		pDC->ModifyWorldTransform(&forma, MWT_RIGHTMULTIPLY);
	}
	else {
		pDC->ModifyWorldTransform(&forma, MWT_LEFTMULTIPLY);
	}
}

void CIND17549View::DrawFigure(CDC* pDC)
{
	//TODO Def kaktus struct
	//LEGENDA	u - up
	//			l - left
	//			r - right
	kaktusNode root;
	root.numOfBranches = 1;
	kaktusNode u;
	u.numOfBranches = 3;
	u.angles[2] = &angle1;

	kaktusNode uu;
	uu.numOfBranches = 2;

	kaktusNode ur;
	ur.numOfBranches = 3;

	kaktusNode uuu;
	uuu.numOfBranches = 1;

	kaktusNode url;
	url.numOfBranches = 1;
	url.angles[0] = &angle2;

	root.decendants[0] = &u;
	u.decendants[1] = &uu;
	u.decendants[2] = &ur;
	uu.decendants[1] = &uuu;
	ur.decendants[0] = &url;
	
	
	Translate(pDC, 10 * this->dimenzija, 10 * this->dimenzija, false);
	Rotate(pDC, DegToRad(-90), false);
	Translate(pDC, -10 * this->dimenzija, -10 * this->dimenzija, false);
	Translate(pDC, 10 * this->dimenzija, 17 * this->dimenzija, false);
	Kaktus(pDC, root);

	Translate(pDC, -10 * this->dimenzija, -17 * this->dimenzija, false);
}


void CIND17549View::DrawPot(CDC* pDC)
{
	CBrush* brush = new CBrush(RGB(222, 148, 0));  
	CBrush* oldBrush = pDC->SelectObject(brush); 
	CPoint* bottomPart = new CPoint[4](); 
	bottomPart[0] = CPoint(8 * this->dimenzija, 18 * this->dimenzija);
	bottomPart[1] = CPoint(12 * this->dimenzija, 18 * this->dimenzija);
	bottomPart[2] = CPoint(11.6 * this->dimenzija, 19.99 * this->dimenzija);
	bottomPart[3] = CPoint(8.4 * this->dimenzija, 19.99 * this->dimenzija);
	pDC->Polygon(bottomPart, 4);  
	pDC->Rectangle(7.6 * this->dimenzija, 17.2 * this->dimenzija, 12.5 * this->dimenzija, 18.1 * this->dimenzija);
	pDC->SelectObject(oldBrush); 
	delete[] bottomPart;
	delete brush;
}

void CIND17549View::DrawBackground(CDC* pDC)
{
	CPen* providniPen = new CPen(PS_NULL, 1, RGB(128, 128, 128));
	CPen* stariprovidniPen = pDC->SelectObject(providniPen);
	CBrush* pozadinaBrush = new CBrush(RGB(136, 206, 236));
	CBrush* oldBrush = pDC->SelectObject(pozadinaBrush);

	CPoint* pozadina = new CPoint[4];
	pozadina[0] = CPoint(0 * this->dimenzija, 0 * this->dimenzija);
	pozadina[1] = CPoint(20 * this->dimenzija, 0 * this->dimenzija);
	pozadina[2] = CPoint(20 * this->dimenzija, 20 * this->dimenzija);
	pozadina[3] = CPoint(0 * this->dimenzija, 20 * this->dimenzija);

	pDC->Polygon(pozadina, 4);
	pDC->SelectObject(oldBrush);
	delete[] pozadina;
	delete pozadinaBrush;
	pDC->SelectObject(stariprovidniPen);
	delete providniPen;
}
void CIND17549View::DrawFont(CDC* pDC)
{
	Translate(pDC, 10 * this->dimenzija, 10 * this->dimenzija, false);
	
	CString* ime = new CString("17549 Tanja Arsic");
	CFont* font = new CFont();
	font->CreateFontW(25, 13, 0, 0, 600, 0, 0, 0, 0, 0, 0, 0, 0, CString("Times New Roman"));

	pDC->SelectObject(font);

	CPoint pozicija(-9.1 * this->dimenzija, -9.1 * this->dimenzija);
	pDC->SetBkMode(TRANSPARENT);

	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->TextOutW(pozicija.x + 1, pozicija.y + 1, *ime);
	pDC->SetTextColor(RGB(255, 255, 0));
	pDC->TextOutW(pozicija.x, pozicija.y, *ime);

	Translate(pDC, -10 * this->dimenzija, -10 * this->dimenzija, false); //vracanje transformacije zbog grida
}

// CIND17549View drawing

void CIND17549View::OnDraw(CDC* pDC)
{
	CIND17549Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);

	DrawBackground(pDC);
	DrawFigure(pDC);
	DrawPot(pDC);
	DrawFont(pDC);


	// TODO: add draw code for native data here

	if (this->keyPressed)
	{
		CPen* gridPen = new CPen(PS_SOLID, 1, RGB(240, 240, 240));
		CPen* stariPen = pDC->SelectObject(gridPen);

		/*vertikalne linije*/
		for (int x = 0; x <= this->dimenzija * 20; x += this->dimenzija) { //20 kvadratica

			pDC->MoveTo(x, 0); //od (x,0)
			pDC->LineTo(x, this->dimenzija * 20); //do (x,500) 
		}
		//za 25px se povecava x dok ne dodje do 500 po y-osi, isto tako i y

		/*horizontalne linije*/
		for (int y = 0; y <= this->dimenzija * 20; y += this->dimenzija) {
			pDC->MoveTo(0, y);
			pDC->LineTo(this->dimenzija * 20, y);
		}

		pDC->SelectObject(stariPen);
		delete gridPen;
	}
}


// CIND17549View diagnostics

#ifdef _DEBUG
void CIND17549View::AssertValid() const
{
	CView::AssertValid();
}

void CIND17549View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIND17549Doc* CIND17549View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND17549Doc)));
	return (CIND17549Doc*)m_pDocument;
}
#endif //_DEBUG


void CIND17549View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	float step = 30;
	// TODO: Add your message handler code here and/or call default
	if (nChar == 'T') //grid pritiskom na T 
	{
		this->keyPressed = !this->keyPressed;
		Invalidate();
	}
	if (nChar == 'Z')
	{
		angle1 += step;
		Invalidate();
	}
	if (nChar == 'X')
	{
		angle1 -= step;
		Invalidate();
	}
	if (nChar == 'C')
	{
		angle2 += step;
		Invalidate();
	}
	if (nChar == 'V')
	{
		angle2 -= step;
		Invalidate();
	}
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

float CIND17549View::DegToRad(float deg)
{
	return (deg * PI) / 180;
}

void CIND17549View::Kaktus(CDC* pDC, kaktusNode node)
{
	
	if (node.numOfBranches == 3)
	{
		Rotate(pDC, DegToRad(-45), false);
		KaktusPart(pDC, node.numOfBranches, node.decendants[0], node.angles[0]);

		Rotate(pDC, DegToRad(45), false);

		KaktusPart(pDC, node.numOfBranches, node.decendants[1], node.angles[1]);
		Rotate(pDC, DegToRad(45), false);

		KaktusPart(pDC, node.numOfBranches, node.decendants[2], node.angles[2]);
		Rotate(pDC, DegToRad(-45), false);
	}
	if (node.numOfBranches == 2)
	{
		Rotate(pDC, DegToRad(-45), false);
		KaktusPart(pDC, node.numOfBranches, node.decendants[0], node.angles[0]);

		Rotate(pDC, DegToRad(90), false);

		KaktusPart(pDC, node.numOfBranches, node.decendants[1], node.angles[1]);
		Rotate(pDC, DegToRad(-45), false);
	}
	if (node.numOfBranches == 1)
	{
		KaktusPart(pDC, node.numOfBranches, node.decendants[0], node.angles[0]);
	}

	CBrush* greenBrush = new CBrush(RGB(0, 200, 0));
	pDC->SelectObject(greenBrush);
	pDC->Ellipse(-10, -10, 10, 10);
	
}

void CIND17549View::KaktusPart(CDC* pDC, int branchScale, kaktusNode* next, float* angle)
{
	int width = 60/branchScale;
	int height = 60;
	CPoint a, b;
	a.x = -width / 2;
	a.y = -height / 2;
	b.x = width / 2;
	b.y = height / 2;

	//Scale(pDC, 1 / branchScale, 0, false); 
	if (angle != nullptr)
	{
		Rotate(pDC, DegToRad(-(*angle)), false);
		Translate(pDC, 0, -height / 2, false);
		pDC->PlayMetaFile(light, CRect(a, b));
	}
	else
	{
		Translate(pDC, 0, -height / 2, false);
		pDC->PlayMetaFile(dark, CRect(a, b));
	}

	if (next != nullptr)
	{
		Translate(pDC, 0, -height / 2, false);
		Kaktus(pDC, *next); 
		Translate(pDC, 0, height / 2, false);
	}
	Translate(pDC, 0, height / 2, false);

	if (angle != nullptr)
		Rotate(pDC, DegToRad(*angle), false);
	
}
