
// Klk(Mrezni Saobracaj)View.cpp : implementation of the CKlkMrezniSaobracajView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk(Mrezni Saobracaj).h"
#endif

#include "Klk(Mrezni Saobracaj)Doc.h"
#include "Klk(Mrezni Saobracaj)View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define pi 3.14159


// CKlkMrezniSaobracajView

IMPLEMENT_DYNCREATE(CKlkMrezniSaobracajView, CView)

BEGIN_MESSAGE_MAP(CKlkMrezniSaobracajView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKlkMrezniSaobracajView construction/destruction

CKlkMrezniSaobracajView::CKlkMrezniSaobracajView() noexcept
{
	// TODO: add construction code here
	router = new DImage();
	router->Load(CString("router.jpg"));
	pos = 0;
}

CKlkMrezniSaobracajView::~CKlkMrezniSaobracajView()
{
	if (router) delete router; router = nullptr;
}

BOOL CKlkMrezniSaobracajView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlkMrezniSaobracajView drawing

void CKlkMrezniSaobracajView::OnDraw(CDC* pDC)
{
	CKlkMrezniSaobracajDoc* pDoc = GetDocument();
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

	XFORM oldForm;
	memDC->SetGraphicsMode(GM_ADVANCED);
	memDC->GetWorldTransform(&oldForm);

	memDC->FillSolidRect(&client, RGB(255, 255, 255));
	// TODO: add draw code for native data here
	CPoint* pts = new CPoint[2];
	pts[0] = CPoint(100, 100);
	pts[1] = CPoint(500, 200);

	DrawRouters(memDC, pts, 2);
	DrawSerialLink(memDC, pts[0], pts[1]);
	int size = 32;
	DrawPacket(memDC, CPoint(9 * size, 1.5 * size), size);
	DrawMessage(memDC, pts[0], pts[1], pos, CSize(50, 30));

	memDC->SetWorldTransform(&oldForm);
	pDC->BitBlt(0, 0, client.Width(), client.Height(), memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC();

}

void CKlkMrezniSaobracajView::DrawRouters(CDC* pDC, CPoint* pts, int count)
{
	for (int i = 0; i < count; i++)
	{
		CRect routerRect(pts[i].x - router->Width() / 6, pts[i].y - router->Height() / 6, pts[i].x + router->Width() / 6, pts[i].y + router->Height() / 6);
		//Scale(pDC, 0.3, 0.3, false);
		router->DrawTransparent(pDC, CRect(0, 0, router->Width(), router->Height()), routerRect, NULL);
		//Scale(pDC, 1, 1, false);
	}
}

void CKlkMrezniSaobracajView::Translate(CDC* pDC, float dx, float dy, bool rightMultiply)
{
	XFORM mat = { 1,0,0,1,dx,dy };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

float CKlkMrezniSaobracajView::ToRad(float angle)
{
	return angle * pi / 180;
}

void CKlkMrezniSaobracajView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	angle *= ToRad(angle);
	XFORM mat = { cos(angle),sin(angle),-sin(angle),cos(angle),0,0 };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKlkMrezniSaobracajView::Scale(CDC* pDC, float sx, float sy, bool rightMultiply)
{
	XFORM mat = { sx,0,0,sy,0,0 };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKlkMrezniSaobracajView::DrawSerialLink(CDC* pDC, CPoint pt1, CPoint pt2)
{
	CPen redPen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen fatredPen;
	pDC->SetMiterLimit(10.0f);
	LOGBRUSH logBrush;
	logBrush.lbColor = RGB(255, 0, 0);
	logBrush.lbStyle = BS_SOLID;
	fatredPen.CreatePen(PS_GEOMETRIC | PS_JOIN_MITER, 7, &logBrush);
	CPen* oldPen = pDC->SelectObject(&redPen);
	pDC->MoveTo(pt1);
	pDC->LineTo(CPoint((pt2.x + pt1.x) / 2 + 10, (pt2.y + pt1.y) / 2 - 5)); //malo vise od pola ka gore
	pDC->SelectObject(&fatredPen);
	pDC->LineTo(CPoint((pt2.x + pt1.x) / 2 - 10, (pt2.y + pt1.y) / 2 + 5));
	pDC->SelectObject(&redPen);
	pDC->LineTo(pt2);

	pDC->SelectObject(oldPen);

}

void CKlkMrezniSaobracajView::DrawPacket(CDC* pDC, CPoint pt, int h)
{
	CBrush yellow(RGB(255, 255, 0));
	CBrush* oldBrush = pDC->SelectObject(&yellow);
	// centar je u tacki pt
	pDC->Rectangle(pt.x - 8 * h, pt.y - h / 2, pt.x + 8 * h, pt.y + h / 2 ); 
	pDC->MoveTo(pt.x - 4 * h, pt.y - h / 2);
	pDC->LineTo(pt.x - 4 * h, pt.y + h / 2);
	pDC->MoveTo(pt.x, pt.y - h / 2);
	pDC->LineTo(pt.x, pt.y + h / 2);

	CFont font1, font2;
	font1.CreateFontW(0.7 * h, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CString("Arial"));
	font2.CreateFontW(0.8 * h, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CString("Arial"));

	int oldBk = pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextAlign(TA_CENTER);

	CFont* oldFont = pDC->SelectObject(&font1);
	pDC->TextOutW(pt.x - 8 * h, pt.y - h / static_cast<double>(2) - 0.7 * h, CString("0"));
	pDC->TextOutW(pt.x - 4 * h, pt.y - h / static_cast<double>(2) - 0.7 * h, CString("8"));
	pDC->TextOutW(pt.x, pt.y - h / static_cast<double>(2) - 0.7 * h, CString("16"));
	pDC->TextOutW(pt.x + 8 * h, pt.y - h / static_cast<double>(2) - 0.7 * h, CString("32"));

	pDC->SelectObject(&font2);
	pDC->TextOutW(pt.x - 6 * h, pt.y - h / 2 + h / 10, CString("Type: 0x8"));
	pDC->TextOutW(pt.x - 2 * h, pt.y - h / 2 + h / 10, CString("Code: 0x0"));
	pDC->TextOutW(pt.x + 4 * h, pt.y - h / 2 + h / 10, CString("Checksum: 02F41"));

	// dealoc
	pDC->SelectObject(oldFont); font1.DeleteObject(); font2.DeleteObject();
	pDC->SelectObject(oldBrush); yellow.DeleteObject();
	pDC->SetBkMode(oldBk);

}
void CKlkMrezniSaobracajView::DrawMessage(CDC* pDC, CPoint pt1, CPoint pt2, int pos, CSize size)
{
	CBrush yellow(RGB(255, 255, 0));
	CBrush* oldBrush = pDC->SelectObject(&yellow);

	float angle = atan2(static_cast<float>(pt1.y - pt2.y), static_cast<float>(pt1.x - pt2.x));
	float w2 = size.cx / 2;
	float h2 = size.cy / 2;

	int inc_x = static_cast<int>((pt2.x - pt1.x) * pos / 100.0); // inkrementira x po 'pos' udelu
	int inc_y = static_cast<int>((pt2.y - pt1.y) * pos / 100.0); // inkrementira y po 'pos' udelu

	pt1.x += inc_x;
	pt1.y += inc_y;

	POINT points[] =
	{
		CPoint(
			pt1.x + static_cast<int>((w2 * cos(angle)) - (h2 * sin(angle))),
			pt1.y + static_cast<int>((w2 * sin(angle)) + (h2 * cos(angle)))
		),
		CPoint(
			pt1.x - static_cast<int>((w2 * cos(angle)) + (h2 * sin(angle))),
			pt1.y - static_cast<int>((w2 * sin(angle)) - (h2 * cos(angle)))
		),
		CPoint(
			pt1.x - static_cast<int>((w2 * cos(angle)) - (h2 * sin(angle))),
			pt1.y - static_cast<int>((w2 * sin(angle)) + (h2 * cos(angle)))
		),
		CPoint(
			pt1.x + static_cast<int>((w2 * cos(angle)) + (h2 * sin(angle))),
			pt1.y + static_cast<int>((w2 * sin(angle)) - (h2 * cos(angle)))
		)
	};
	pDC->Polygon(points, 4);

	pDC->MoveTo(pt1.x + (w2 * cos(angle) - h2 * sin(angle)), pt1.y + (w2 * sin(angle) + h2 * cos(angle)));
	pDC->LineTo(pt1.x, pt1.y);
	pDC->LineTo(pt1.x - (w2 * cos(angle) + h2 * sin(angle)), pt1.y - (w2 * sin(angle) - h2 * cos(angle))); // Draw a line to the top-right corner
}


// CKlkMrezniSaobracajView printing

BOOL CKlkMrezniSaobracajView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlkMrezniSaobracajView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlkMrezniSaobracajView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKlkMrezniSaobracajView diagnostics

#ifdef _DEBUG
void CKlkMrezniSaobracajView::AssertValid() const
{
	CView::AssertValid();
}

void CKlkMrezniSaobracajView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlkMrezniSaobracajDoc* CKlkMrezniSaobracajView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlkMrezniSaobracajDoc)));
	return (CKlkMrezniSaobracajDoc*)m_pDocument;
}
#endif //_DEBUG


// CKlkMrezniSaobracajView message handlers


BOOL CKlkMrezniSaobracajView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}


void CKlkMrezniSaobracajView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == 'Q')
	{
		if (pos < 100) {
			pos += 10;
			Invalidate();
		}
	}
	if (nChar == 'W')
	{
		if (pos > 0) 
		{
			pos -= 10;
			Invalidate();
		}
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
