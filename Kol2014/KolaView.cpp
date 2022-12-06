
// KolaView.cpp : implementation of the CKolaView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kola.h"
#endif

#include "KolaDoc.h"
#include "KolaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include"math.h"

#define pi 3.14159
// CKolaView

IMPLEMENT_DYNCREATE(CKolaView, CView)

BEGIN_MESSAGE_MAP(CKolaView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

// CKolaView construction/destruction

CKolaView::CKolaView() noexcept
{
	// TODO: add construction code here
	wheel = new DImage();
	wheel->Load(CString("Wheel.png"));

	clio = GetEnhMetaFile(CString("clio.emf"));
	//GetEnhMetaFileHeader(clio, sizeof(ENHMETAHEADER), &clio_header);
	this->angle = 10;
	poz = 0;
	wheel_angle = 0;
}

CKolaView::~CKolaView()
{
	delete wheel;
	DeleteEnhMetaFile(clio);
}

BOOL CKolaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

float CKolaView::toRad(float angle)
{
	return angle * pi / 180;
}

// CKolaView drawing

//DrawGround(CDC* pDC, float angle), koja iscrtava podlogu
//po kojoj se kreće automobil.Podloga je uzdignuta sleva udesno pod uglom
//angle zadatom u stepenima(počinje u donjem levom uglu prozora).Okvir je
//iscrtan olovkom debljine 3 braon boje, a ispuna šrafurom braon - svetlo - zelene
//boje.Ostatak ekrana obojen je svetlo plavo.[10 poena]
void CKolaView::DrawGround(CDC* pDC, float angle)
{
	CPen* old_pen; 
	CBrush* old_brush;
	CPen pen(PS_SOLID, 3, RGB(101, 65, 30)); //braon
	CBrush brush(HS_FDIAGONAL, RGB(0, 255, 0)); //zeleno
	old_pen = pDC->SelectObject(&pen); //nova je stara
	old_brush = pDC->SelectObject(&brush);
	COLORREF old_background = pDC->SetBkColor(RGB(220, 255, 192)); //svetloplavo


	CRect klijent;
	GetClientRect(&klijent); //klijenCki prozor

	CPoint points[3];
	points[0].x = klijent.left; 
	points[0].y = klijent.bottom; //dole levo

	points[1].x = klijent.right; 
	points[1].y = klijent.bottom; //dole desno

	points[2].x = klijent.right; //na desnoj str.
	points[2].y = klijent.bottom - klijent.Width() * tan(toRad(angle)); 
	//tan(angle) je y - dy deo klijentskog ekrana podeljeno s x klijentskog ekrana

	pDC->Polygon(points, 3);
	pDC->SetBkColor(old_background);
	pDC->SelectObject(old_pen);
	old_pen->DeleteObject();
	pDC->SelectObject(old_brush);
	old_brush->DeleteObject();

	

}
void CKolaView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM t = { 1,0,0,1,dX,dY };
	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKolaView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM t = { cos(toRad(angle)),sin(toRad(angle)),-sin(toRad(angle)),cos(toRad(angle)),0,0 };
	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKolaView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM t = { sX,0,0,sY,0,0 };
	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKolaView::DrawCar(CDC* pDC, int x, int y, int w, int h) //prikazuje metafajl clio.emf sa centrom u tački(x, y), širine w i visine h
{
	pDC->PlayMetaFile(clio, CRect(	x - w / 2,  //gore levo
									y - h / 2, 
									x + w / 2, //dole desno
									y + h / 2)); 
	//pDC->PlayMetaFile(clio, CRect(x - w / 2, y - h / 2, x + w / 2, y + h / 2));

}
void CKolaView::DrawWheel(CDC* pDC, int x, int y, int r, float angle)
{
	XFORM oldT;
	pDC->GetWorldTransform(&oldT);
	Translate(pDC, x, y, false);
	Rotate(pDC, angle, false);
	wheel->DrawTransparent(pDC, CRect(52, 15, 376 + 52, 376 + 15), CRect(-r, -r, r, r), NULL);
	
	pDC->SetWorldTransform(&oldT);

}
//Wheel.png sa centrom u tački(x, y), poluprečnika r i zarotiran za ugao angle u stepenima.
//sa gornjim levim uglom na poziciji(52, 15) i širine / visinejednake 376 piksela.
//Iscrtavanje ograničiti samo na dati okvirni pravougaonik. i koristiti funkciju DrawTransparent klase DImage

void CKolaView::OnDraw(CDC* pDC)
{
	CKolaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CBitmap b;
	XFORM t;
	CRect k;
	CBrush* stara;
	CBrush plava(RGB(200, 220, 255));
	GetClientRect(&k);
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	b.CreateCompatibleBitmap(pDC, k.Width(), k.Height());
	memDC->SelectObject(&b);
	memDC->SetGraphicsMode(GM_ADVANCED);
	memDC->GetWorldTransform(&t);
	stara = memDC->SelectObject(&plava);
	memDC->Rectangle(k);

	DrawGround(memDC, this->angle);
	Translate(memDC, 0, k.Height(), false); //na dno ekrana levo
	Rotate(memDC, -this->angle, false);
	Translate(memDC, 300, - 107, false); //lupljene koordinate, vrv ima neka logika
	Translate(memDC, -poz, 0, false);
	Scale(memDC, -1, 1, false);
	DrawCar(memDC, 0, 0, 450, 450/2.5);
	Scale(memDC, 1, 1, false);

	wheel_angle = poz / (2 * pi);

	DrawWheel(memDC, 155, 70, 38, wheel_angle);
	DrawWheel(memDC, -135, 70, 38, wheel_angle); //ne znam je l tockovi treba da se obrcu


	stara->DeleteObject();
	memDC->SetWorldTransform(&t);
	pDC->BitBlt(0, 0, k.Width(), k.Height(), memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC();
}

//Korišćenjem prethodnih funkcija iscrtati telo automobila širine 450 jedinica i visine 2.5 puta
//manje od širine, i dva točka poluprečnika 38 na odgovarajućim pozicijama(središta oba
//	točka su spuštena za 70 jedinica u odnosu na centar tela automobila, levi je pomeren -
//	155, a desni + 135 po X - osi).Čitav automobil postaviti na početku strme ravni, tako da
//	točkovi dodiruju strmu ravan(vidi sliku).Prilikom kretanja automobila, točkovi se rotiraju u
//	skladu sa pređenim putem.

// CKolaView printing

BOOL CKolaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKolaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKolaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKolaView diagnostics

#ifdef _DEBUG
void CKolaView::AssertValid() const
{
	CView::AssertValid();
}

void CKolaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKolaDoc* CKolaView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKolaDoc)));
	return (CKolaDoc*)m_pDocument;
}
#endif //_DEBUG


// CKolaView message handlers


BOOL CKolaView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}


void CKolaView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RIGHT) {
		poz -= 35;
		//wheel_angle += 10;
		Invalidate();

	}
	if (nChar == VK_LEFT) {
		poz += 35;
		//wheel_angle -= 10;
		Invalidate();

	}
	if (nChar == VK_UP) {
		poz = 0;
		this->angle += 10;
		if (this->angle > 80) this->angle = 80;
		Invalidate();

	}
	if (nChar == VK_DOWN) {
		poz = 0;
		this->angle -= 10;
		if (this->angle < -10) this->angle = -10;
		Invalidate();

	}
	
	// TODO: Add your message handler code here and/or call default
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}
