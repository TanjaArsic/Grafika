
// Klk2012(Gears)View.cpp : implementation of the CKlk2012GearsView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk2012(Gears).h"
#endif

#include "Klk2012(Gears)Doc.h"
#include "Klk2012(Gears)View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define pi 3.14159

// CKlk2012GearsView

IMPLEMENT_DYNCREATE(CKlk2012GearsView, CView)

BEGIN_MESSAGE_MAP(CKlk2012GearsView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKlk2012GearsView construction/destruction

CKlk2012GearsView::CKlk2012GearsView() noexcept
{
	// TODO: add construction code here
	gearAngle = 0.0f;

}

CKlk2012GearsView::~CKlk2012GearsView()
{
}

BOOL CKlk2012GearsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlk2012GearsView drawing

void CKlk2012GearsView::OnDraw(CDC* pDC)
{
	CKlk2012GearsDoc* pDoc = GetDocument();
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
	memDC->SetGraphicsMode(GM_ADVANCED);
	memDC->Rectangle(client);
	XFORM xform;
	memDC->GetWorldTransform(&xform);
	Translate(memDC, 300, 300);
	DrawScene(memDC);
	// TODO: add draw code for native data here
	memDC->SetWorldTransform(&xform);
	pDC->BitBlt(0, 0, client.Width(), client.Height(), memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC();

}

void CKlk2012GearsView::Trapez(CDC* pDC, int a, int b, int h)
{
	//koja iscrtava trapez sa centrom u
	//	koordinatnom početku, čija je donja osnovica dužine a, a gornja b.Visina trapeza je h
	POINT pts[] = {
		CPoint(-b / 2,-h / 2),
		CPoint(b / 2,-h / 2),
		CPoint(a / 2,h / 2),
		CPoint(-a / 2,h / 2)
	};
	pDC->Polygon(pts, 4);
	
}

void CKlk2012GearsView::Translate(CDC* pDC, float x, float y)
{
	XFORM mat = { 1,0,0,1,x,y };
	pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}

void CKlk2012GearsView::Rotate(CDC* pDC, float a)
{
	a *= pi / 180;
	XFORM mat = { cos(a),sin(a),-sin(a),cos(a),0,0 };
	pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}

void CKlk2012GearsView::DrawGear(CDC* pDC, int spokeCount, int spokeWidth)
{
	int crc = spokeCount * 2.0 * spokeWidth; //ovo je obim kruga koji je 2r pi, znaci da crtam krug s polupr r
	float r = crc / (2 * pi);

	CPen pen(PS_DOT, 1, RGB(0, 0, 0));
	CPen* oldPen = pDC->SelectObject(&pen);
	CGdiObject* oldBrush = pDC->SelectStockObject(NULL_BRUSH);
	pDC->Ellipse(-r, -r, r, r);

	CPen fatPen(PS_SOLID, 2, RGB(0, 0, 64));
	CBrush blueBrush(RGB(128, 192, 255));
	pDC->SelectObject(&fatPen); 
	pDC->SelectObject(&blueBrush);
	float r2 = r - spokeWidth / 2;


	/*Zupci se crtaju korišćenjem funkcije Trapez(), sa donjom osnovicom 1.4 puta dužom od
		spokeWidth, a gornjom isto toliko puta kraćom.Zupci su takođe svetlo plave boje sa
		tamno plavom okvirnom linijom debljine 2 piksela.Za postavljanje zupčanika koristiti
		funkcije definisane u tačkama 2 i 3*/
	float angle = 360 / spokeCount;
	for (int i = 0; i < spokeCount; i++)
	{
		Rotate(pDC, angle);
		Translate(pDC, 0, -r);
		Trapez(pDC,1.4* spokeWidth, spokeWidth / 1.4, spokeWidth);
		Translate(pDC, 0, r);
	}
	pDC->Ellipse(-r2, -r2, r2, r2);

	//U središtu zupčanika iscrtan je kvadrat stranice spokeWidth, a na sredini njegove donje
	//	stranice nalazi se gornja - središna tačka okvirnog pravougaonika teksta koji se ispisuje.
	//	Tekst se ispisuje fontom Arial, veličine 18, i sadrži broj zubaca datog zupčanika

	pDC->Rectangle(-spokeWidth / 2, -spokeWidth / 2, spokeWidth / 2, spokeWidth / 2);
	CFont font;
	font.CreateFontW(18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CString("Arial"));
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF oldTextColor = pDC->SetTextColor(RGB(0, 0, 64));
	pDC->SetTextAlign(TA_CENTER);
	CString strSpokeCount;
	strSpokeCount.Format(CString("%d zubaca"), spokeCount);

	// Output the text on device context pDC
	pDC->TextOutW(0, spokeWidth / 2, strSpokeCount);

	pDC->SelectObject(oldPen); pen.DeleteObject(); fatPen.DeleteObject();
	pDC->SelectObject(oldBrush); blueBrush.DeleteObject(); 
	pDC->SetBkMode(oldBkMode);
	pDC->SetTextColor(oldTextColor);
}

void CKlk2012GearsView::DrawScene(CDC* pDC)
{
	/*Napisati funkciju DrawScene(CDC* pDC), koja iscrtava 3 zupčanika. Jedan sa 20 zubaca, od
kojih je svaki širine 20, sa centrom u tački (300, 300). Drugi pomeren desno od njega i sa duplo
manjim brojem zubaca (10). Treći zupčanik je pomeren gore-levo u odnosu na centralni i
zarotiran za 8 stepeni, kako bi se uklopili zupci. Rastojanje centara dva zupčanika jednako je
zbiru njihovih efektivnih poluprečnika. Za postavljanje i rotaciju zupčanika koristiti funkcije
definisane u tačkama 2 i 3. [20 poena*/

		
	float distance = (20 * 2.0 * 20 + 20 * 2.0 * 10) / (2 * pi);
	XFORM second;
	pDC->GetWorldTransform(&second);
	Translate(pDC, distance / sqrt(2), distance / sqrt(2));
	Rotate(pDC, gearAngle);
	DrawGear(pDC, 20, 20);
	pDC->SetWorldTransform(&second);

	XFORM third;
	pDC->GetWorldTransform(&third);
	Translate(pDC, distance + distance / sqrt(2), distance / sqrt(2));
	Rotate(pDC, -gearAngle);
	DrawGear(pDC, 10, 20);
	pDC->SetWorldTransform(&third);

	XFORM first;
	pDC->GetWorldTransform(&first);
	Translate(pDC, 0, 0);
	Rotate(pDC, 8 - gearAngle);
	DrawGear(pDC, 10, 20);
	pDC->SetWorldTransform(&first);

}


// CKlk2012GearsView printing

BOOL CKlk2012GearsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlk2012GearsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlk2012GearsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKlk2012GearsView diagnostics

#ifdef _DEBUG
void CKlk2012GearsView::AssertValid() const
{
	CView::AssertValid();
}

void CKlk2012GearsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlk2012GearsDoc* CKlk2012GearsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlk2012GearsDoc)));
	return (CKlk2012GearsDoc*)m_pDocument;
}
#endif //_DEBUG


// CKlk2012GearsView message handlers


void CKlk2012GearsView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_UP)
	{
		gearAngle += 1;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CKlk2012GearsView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}
