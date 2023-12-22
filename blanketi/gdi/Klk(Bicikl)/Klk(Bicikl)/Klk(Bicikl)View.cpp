
// Klk(Bicikl)View.cpp : implementation of the CKlkBiciklView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk(Bicikl).h"
#endif

#include "Klk(Bicikl)Doc.h"
#include "Klk(Bicikl)View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define pi 3.14159


// CKlkBiciklView

IMPLEMENT_DYNCREATE(CKlkBiciklView, CView)

BEGIN_MESSAGE_MAP(CKlkBiciklView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CKlkBiciklView construction/destruction

CKlkBiciklView::CKlkBiciklView() noexcept
{
	// TODO: add construction code here
	moveBicycle = 0.0f;
	timer = 1;
	timeStart = clock();

}

CKlkBiciklView::~CKlkBiciklView()
{
}

BOOL CKlkBiciklView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlkBiciklView drawing

void CKlkBiciklView::OnDraw(CDC* pDC)
{
	CKlkBiciklDoc* pDoc = GetDocument();
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
	COLORREF clrSky(RGB(129, 205, 255));
	COLORREF clrGrass(RGB(156, 202, 64));
	COLORREF clrTrack(RGB(222, 222, 222));	
	COLORREF clr = RGB(0, 90, 200);
	int r = 100;
	int n = 18;
	int d = 5;
	float beta = 30;
	clock_t timeEnd = clock();
	clock_t timeDiff = timeEnd - timeStart;
	float time = ((float)timeDiff) / CLOCKS_PER_SEC;
	CString strText("Kurac picka govno sisa");
	DrawBicycleOnTrack(memDC, c, r, n, d, beta, clr, clrSky, clrGrass, clrTrack, strText, 100, this->time);
	memDC->SetWorldTransform(&xform);
	pDC->BitBlt(0, 0, c.Width(), c.Height(), memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC();
}

void CKlkBiciklView::DrawCycle(CDC* pDC, int r, int n, int d, COLORREF clr)
{
	CPen tire(PS_SOLID, d, clr);
	CPen spone(PS_SOLID, 1, clr);
	CBrush sponeBrush(clr);
	
	CGdiObject* oldBrush = pDC->SelectStockObject(NULL_BRUSH);
	CPen* oldPen = pDC->SelectObject(&tire);
	pDC->Ellipse(-r, -r, r, r);
	pDC->SelectObject(&sponeBrush);
	pDC->Ellipse(-2*d, -2 * d, 2 * d, 2 * d);
	float angle = 360.0 / n;

	pDC->SelectObject(&spone);
	pDC->SelectObject(oldBrush);
	for (int i = 0; i < n; i++)
	{
		pDC->MoveTo(0, 0);
		pDC->LineTo(0, -r);
		Rotate(pDC, -angle, false); // vraca se na pocetak
	}
	pDC->SelectObject(oldBrush); sponeBrush.DeleteObject();
	pDC->SelectObject(oldPen);	tire.DeleteObject();	spone.DeleteObject();
}

void CKlkBiciklView::DrawFrame(CDC* pDC, int r, int d, COLORREF clr)
{
	CPen tire(PS_SOLID, d, clr);
	CPen* oldPen = pDC->SelectObject(&tire);

	// veci luk
	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->Arc(CRect(-1.2 * r, -1.2 * r, 1.2 * r, 1.2 * r), CPoint(0, -1.2 * r), CPoint(1.2 * r, 0));
	// oslonac za penjanje
	pDC->MoveTo(1.2 * r, 0); 
	pDC->LineTo(1.2 * r + 0.2 * r, 0);
	// manji luk
	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->Arc(CRect(1.2 * r, -0.75 * r, 1.2 * r + 1.5 * r, -0.75 * r + 1.5 * r), CPoint(1.2 * r, 0), CPoint(1.2 * r + 0.75 * r, 0.75 * r));
	//manji krug centar je na 1.2 * r + 0.75 * r, 0.75 * r
	// upravljac
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, -1.4 * r);
	pDC->LineTo(0.2 * r, -1.4 * r);

	pDC->SelectObject(oldPen);	tire.DeleteObject();	

}

void CKlkBiciklView::DrawSeat(CDC* pDC, int r, int d, float beta, COLORREF clr)
{
	CPen tire(PS_SOLID, d, clr);
	CPen* oldPen = pDC->SelectObject(&tire);
	//nosac pocinje od rama koji je na rastojanju 1.2*r od centra veceg tocka pod uglom beta u odnosu na centar
	// to cu preko transf.
	XFORM seat;
	pDC->GetWorldTransform(&seat);
	Rotate(pDC, beta, false);
	Translate(pDC, 0, -1.2 * r, false);
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, -0.2 * r);
	Rotate(pDC, -beta, false);
	pDC->MoveTo(-0.1 * r, -0.2 * r);
	pDC->LineTo(0.2 * r, -0.2 * r); // mrzi me oko ovog da se bakcem

	pDC->SetWorldTransform(&seat);

	pDC->SelectObject(oldPen);	tire.DeleteObject();
}

void CKlkBiciklView::DrawPedals(CDC* pDC, int r, int d, float alpha, COLORREF clr)
{
	CPen tire(PS_SOLID, d, clr);
	CPen* oldPen = pDC->SelectObject(&tire);
	XFORM pedal;
	pDC->GetWorldTransform(&pedal);
	Rotate(pDC, alpha, false);
	pDC->MoveTo(0, 0.3 * r);
	pDC->LineTo(0, -0.3 * r);

	Translate(pDC, 0, 0.3 * r, false);
	Rotate(pDC, -alpha, false);
	pDC->MoveTo(-0.1 * r,0 );
	pDC->LineTo(0.1 * r, 0);
	Rotate(pDC, alpha, false);

	Translate(pDC, 0, -0.6 * r, false);
	Rotate(pDC, -alpha, false);
	pDC->MoveTo(-0.1 * r, 0);
	pDC->LineTo(0.1 * r, 0);

	pDC->SetWorldTransform(&pedal);

	// horizontalni deo me mrzi
	pDC->SelectObject(oldPen);	tire.DeleteObject();
}

void CKlkBiciklView::DrawBicycle(CDC* pDC, int r, int n, int d, float beta, COLORREF clr, float omega, float time)
{
	//float angle = (360/n) / r;
	float angle = - omega * time;
	//tocak veci
	Rotate(pDC, angle, false);
	DrawCycle(pDC, r, n, d, clr);
	DrawPedals(pDC, r, d, beta, clr);
	Rotate(pDC, -angle, false);

	Translate(pDC, 1.2 * r + 0.75 * r, 0.75 * r, false);
	//tocak manji
	Rotate(pDC, angle, false);
	DrawCycle(pDC, 0.25 * r, n, d, clr);
	Rotate(pDC, -angle, false);

	Translate(pDC, -1.2 * r - 0.75 * r, -0.75 * r, false);

	DrawFrame(pDC, r, d, clr);
	DrawSeat(pDC, r, d, beta, clr);
	//pedale
	/*Rotate(pDC, angle, false);
	DrawPedals(pDC, r, d, beta, clr);
	Rotate(pDC, -beta, false);*/
	//DrawPedals(pDC, r, d, beta, clr);


}

void CKlkBiciklView::DrawSignature(CDC* pDC, CRect rcTrack, int fontHeight, CString strText, COLORREF clrText)
{
	
	CFont font;
	font.CreateFontW(fontHeight, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, CString("Arial"));
	CFont* oldFont = pDC->SelectObject(&font);
	COLORREF oldClr = pDC->SetTextColor(clrText);
	pDC->SetTextAlign(TA_BASELINE | TA_RIGHT);
	pDC->SetBkMode(TRANSPARENT);
	XFORM f;
	pDC->GetWorldTransform(&f);
	Translate(pDC, rcTrack.Width(), rcTrack.Height()/2, false);
	Rotate(pDC, -90, false);
	pDC->TextOutW(0,0, strText);

	pDC->SelectObject(oldFont); pDC->SetTextColor(oldClr);
	pDC->SetWorldTransform(&f);

}

void CKlkBiciklView::DrawTrack(CDC* pDC, CRect rcTrack, int lineWidth, COLORREF clrSky, COLORREF clrGrass, COLORREF clrTrack)
{
	CBrush sky(clrSky);
	CBrush grass(clrGrass);
	CBrush track(clrTrack);
	CPen pen(PS_SOLID, lineWidth, RGB(255, 255, 255));
	CGdiObject* oldPen = pDC->SelectStockObject(NULL_PEN);
	CBrush* oldBrush = pDC->SelectObject(&sky);
	pDC->Rectangle(0, 0, rcTrack.Width(), 0.4 * rcTrack.Height());
	pDC->SelectObject(&grass);
	pDC->Rectangle(0, 0.4 * rcTrack.Height(), rcTrack.Width(), (0.4 + 0.2)* rcTrack.Height());
	pDC->SelectObject(&track);
	pDC->Rectangle(0, (0.4 + 0.2) * rcTrack.Height(), rcTrack.Width(), rcTrack.Height()); //ovde kae 0.4 al naur 
	pDC->SelectObject(&pen);
	pDC->MoveTo(0, (0.4 + 0.2) * rcTrack.Height());
	pDC->LineTo(rcTrack.Width(), (0.4 + 0.2) * rcTrack.Height());
	
	float h = (rcTrack.Height() + (0.4 + 0.2) * rcTrack.Height())/2;
	
	int oldMode = pDC->SetBkMode(TRANSPARENT);
	LOGBRUSH logBrush;
	logBrush.lbColor = RGB(255, 255, 255);
	logBrush.lbStyle = BS_SOLID;
	CPen* penDash = new CPen(PS_GEOMETRIC | PS_DASH | PS_ENDCAP_FLAT, lineWidth, &logBrush);
	pDC->SelectObject(penDash);
	pDC->MoveTo(0, h);
	pDC->LineTo(rcTrack.Width(), h);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush); 
	pDC->SetBkMode(oldMode);
	sky.DeleteObject();
	grass.DeleteObject();
	track.DeleteObject();
	pen.DeleteObject();
	delete penDash;

}

void CKlkBiciklView::DrawBicycleOnTrack(CDC* pDC, CRect rcTrack, int r, int n, int d, float beta, COLORREF clr, COLORREF clrSky, COLORREF clrGrass, COLORREF clrTrack, CString strText, float omega, float time)
{
	DrawTrack(pDC, rcTrack, rcTrack.Height() / 30, clrSky, clrGrass, clrTrack);
	//DrawSignature(pDC, rcTrack, rcTrack.Height() / 15, strText, clr);
	float len = (omega * pi / 180) * time * r;
	Translate(pDC, rcTrack.Width() - 1.2/2 * r - 1.5 * r - len,  rcTrack.Height() - rcTrack.Height()/12 - r, false);
	DrawBicycle(pDC, r, n, d, beta, clr, omega, time);



}

void CKlkBiciklView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM mat = { 1,0,0,1,dX,dY };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKlkBiciklView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	angle *= pi / 180;
	XFORM mat = { cos(angle), sin(angle), -sin(angle), cos(angle), 0, 0};
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}


// CKlkBiciklView printing

BOOL CKlkBiciklView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlkBiciklView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlkBiciklView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKlkBiciklView diagnostics

#ifdef _DEBUG
void CKlkBiciklView::AssertValid() const
{
	CView::AssertValid();
}

void CKlkBiciklView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlkBiciklDoc* CKlkBiciklView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlkBiciklDoc)));
	return (CKlkBiciklDoc*)m_pDocument;
}
#endif //_DEBUG


// CKlkBiciklView message handlers


BOOL CKlkBiciklView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return true;
}


void CKlkBiciklView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_LEFT)
	{
		this->time += 0.1;
		Invalidate();
	}
	if (nChar == VK_RIGHT)
	{
		this->time -= 0.1;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


int CKlkBiciklView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	if (timeStart == 0.0)
	{
		timeStart = clock();
		timer = SetTimer(1, 10, NULL);
	}
	return 0;
}


void CKlkBiciklView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == timer)
	{
		Invalidate();
	}

	CView::OnTimer(nIDEvent);
}
