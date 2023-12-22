
// Klk(Zastava)View.cpp : implementation of the CKlkZastavaView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk(Zastava).h"
#endif

#include "Klk(Zastava)Doc.h"
#include "Klk(Zastava)View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define pi 3.14159
// CKlkZastavaView

IMPLEMENT_DYNCREATE(CKlkZastavaView, CView)

BEGIN_MESSAGE_MAP(CKlkZastavaView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CKlkZastavaView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CKlkZastavaView construction/destruction

CKlkZastavaView::CKlkZastavaView() noexcept
{
	// TODO: add construction code here
	flag = new DImage(); flag->Load(CString("flag.png"));

}

CKlkZastavaView::~CKlkZastavaView()
{
	if (flag) delete flag; flag = nullptr;
}

BOOL CKlkZastavaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlkZastavaView drawing

void CKlkZastavaView::OnDraw(CDC* pDC)
{
	CKlkZastavaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM old;
	pDC->GetWorldTransform(&old);
	DrawFlags(pDC, flag, 300, 300, 200, 80, 16, 20.0, 6);
	pDC->SetWorldTransform(&old);

	// TODO: add draw code for native data here
}


// CKlkZastavaView printing
void CKlkZastavaView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM mat = { 1,0,0,1,dX,dY };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKlkZastavaView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	angle *= pi / 180;
	XFORM mat = { cos(angle), sin(angle), -sin(angle), cos(angle), 0, 0 };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void CKlkZastavaView::Shear(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM mat = { 1,sX,sY,1,0,0 };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKlkZastavaView::DrawFlagPart(CDC* pDC, DImage* img, double t1, double t2, int x1, int y1, int x2, int y2, int h)
{
	t1 *= img->Width();
	t2 *= img->Width();
	float off = static_cast<float>(y2 - y1) / (x2 - x1);

	XFORM part;
	pDC->GetWorldTransform(&part);
	Translate(pDC, x1, y1, false);
	Shear(pDC, off, 0, false);
	Translate(pDC, -x1, -y1, false);

	img->Draw(pDC, CRect(t1, 0, t2, img->Height()), CRect(x1, y1, x2, y1 + h));
	pDC->SetWorldTransform(&part);
}

void CKlkZastavaView::DrawFlag(CDC* pDC, DImage* img, int x, int y, int w, int h, int nSeg, double a)
{
	XFORM flag;
	pDC->GetWorldTransform(&flag);
	Translate(pDC, x, y, false);
	float angle = 2 * pi / nSeg; //za kolko se rot
	for (int i = 0; i < nSeg; i++)
	{
		double t1 = static_cast<double>(i) / nSeg; // normalizovanje
		double t2 = (i + static_cast<double>(1)) / nSeg;
		int x1 = x + t1 * w;
		int x2 = x + t2 * w;
		int y1 = y + a * sin(angle * i);
		int y2 = y + a * sin(angle * (i + 1));

		DrawFlagPart(pDC, img, t1, t2, x1, y1, x2, y2, h);
	}
	pDC->SetWorldTransform(&flag);


}
//
//void CKlkZastavaView::DrawFlags(CDC* pDC, DImage* img, int cx, int cy, int w, int h, int nSeg, double a, int nFlag)
//{
//	Translate(pDC, cx, cy, false);
//	float angle = 2 * pi / nFlag; 
//	int x = h / (2 * tan(angle / 2));
//	for (int i = 0; i < nFlag; i++)
//	{
//		XFORM flags;
//		pDC->GetWorldTransform(&flags);
//		Rotate(pDC, angle*i, false);
//		DrawFlag(pDC, img, x, -h / 2, w, h, nSeg, a);
//		//pDC->SetWorldTransform(&flags);
//
//	}
//}
void CKlkZastavaView::DrawFlags(CDC* pDC, DImage* img, int cx, int cy, int w, int h, int nSeg, double a, int nFlag)
{
	float angle = 360.0f / nFlag; 
	float radius = (w / 6.0f) / tan((angle * pi / 180.0f) / 2.0f);
	for (int i = 0; i < nFlag; i++)
	{
		XFORM flags;
		pDC->GetWorldTransform(&flags);

		float currentAngle = i * angle; 
		float x = cx + radius * cos(currentAngle * pi / 180.0f); // x koordinata
		float y = cy + radius * sin(currentAngle * pi / 180.0f); // y koordinata za zastavu

		Translate(pDC, x, y, false); 
		Rotate(pDC, currentAngle, false); 
		DrawFlag(pDC, img, 0, 0, w, h, nSeg, a); 
		pDC->SetWorldTransform(&flags); 
	}
}




void CKlkZastavaView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CKlkZastavaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlkZastavaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlkZastavaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CKlkZastavaView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CKlkZastavaView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CKlkZastavaView diagnostics

#ifdef _DEBUG
void CKlkZastavaView::AssertValid() const
{
	CView::AssertValid();
}

void CKlkZastavaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlkZastavaDoc* CKlkZastavaView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlkZastavaDoc)));
	return (CKlkZastavaDoc*)m_pDocument;
}
#endif //_DEBUG


// CKlkZastavaView message handlers
