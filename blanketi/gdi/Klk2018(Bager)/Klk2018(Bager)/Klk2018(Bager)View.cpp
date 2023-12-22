
// Klk2018(Bager)View.cpp : implementation of the CKlk2018BagerView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk2018(Bager).h"
#endif

#include "Klk2018(Bager)Doc.h"
#include "Klk2018(Bager)View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define pi 3.14159

// CKlk2018BagerView

IMPLEMENT_DYNCREATE(CKlk2018BagerView, CView)

BEGIN_MESSAGE_MAP(CKlk2018BagerView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKlk2018BagerView construction/destruction

CKlk2018BagerView::CKlk2018BagerView() noexcept
{
	// TODO: add construction code here
	bager = new DImage();
	bager->Load(CString("bager.png"));
	arm1 = new DImage();
	arm1->Load(CString("arm1.png"));
	arm2 = new DImage();
	arm2->Load(CString("arm2.png"));
	bkground = new DImage();
	bkground->Load(CString("pozadina.png"));
	
	fork = GetEnhMetaFile(CString("viljuska.emf"));
	GetEnhMetaFileHeader(fork, sizeof(ENHMETAHEADER), &forkRect);
	smolCrcl = bigCrcl = forkRotat = -90.0f;
	moveDx = 0.0f;
}

CKlk2018BagerView::~CKlk2018BagerView()
{
	if (bager) delete bager; bager = nullptr;
	if (arm1) delete arm1; arm1 = nullptr;
	if (arm2) delete arm2; arm2 = nullptr;
	if (bkground) delete bkground; bkground = nullptr;
	 
}

BOOL CKlk2018BagerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlk2018BagerView drawing

void CKlk2018BagerView::OnDraw(CDC* pDC)
{
	CKlk2018BagerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	XFORM oldForm;
	CBitmap bmp;
	CRect clientRect;
	GetClientRect(&clientRect);
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, clientRect.Width(), clientRect.Height());
	memDC->SelectObject(&bmp);
	memDC->GetWorldTransform(&oldForm);
	memDC->SetGraphicsMode(GM_ADVANCED);

	// TODO: add draw code for native data here
	DrawBackground(memDC);
	Translate(memDC, clientRect.Width() - bager->Width() + moveDx, clientRect.Height() - bager->Height(), false);
	DrawExcavator(memDC);

	memDC->SetWorldTransform(&oldForm);

	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC();
	
}

void CKlk2018BagerView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM mat = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

float CKlk2018BagerView::ToRad(float angle)
{
	return angle * pi / 180;
}

void CKlk2018BagerView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float radAngle = ToRad(angle);
	XFORM mat = { cos(radAngle), sin(radAngle), -sin(radAngle), cos(radAngle),0,0};
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKlk2018BagerView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM mat = { sX, 0, 0, sY, 0,0 };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void CKlk2018BagerView::DrawBackground(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	bkground->Draw(pDC, CRect(0, 0, bkground->Width(), bkground->Height()), 
		CRect(rect.CenterPoint().x - bkground->Width() / 2, rect.bottom - bkground->Height(),
			rect.CenterPoint().x + bkground->Width() / 2, rect.bottom));
	
}
void CKlk2018BagerView::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	CRect imageRect(0, 0, pImage->Width(), pImage->Height());
	pImage->DrawTransparent(pDC, imageRect, imageRect, NULL);
}
void CKlk2018BagerView::DrawBody(CDC* pDC)
{
	/*U okviru ove funkcije
pomeriti lokalni koordinatni sistem, tako da se ruka, koja će se naknadno crtati, nalazi ispred
kabine.
*/
	DrawImgTransparent(pDC, bager);
	Translate(pDC, 0, 110,false);
}
void CKlk2018BagerView::DrawArm1(CDC* pDC)
{
	Translate(pDC, 58, 61, false);
	Rotate(pDC, bigCrcl, false);
	Translate(pDC, -58, -61, false);
	DrawImgTransparent(pDC, arm1);
	Translate(pDC, 309, 61, false);
}
void CKlk2018BagerView::DrawArm2(CDC* pDC)
{
	Rotate(pDC, smolCrcl, false);
	Translate(pDC, -36, -40, false);
	DrawImgTransparent(pDC, arm2);	
	Translate(pDC, 272, 40, false);

}
void CKlk2018BagerView::DrawFork(CDC* pDC)
{
	Rotate(pDC, forkRotat, false);
	Scale(pDC, 2.5, 2.5, false);
	Translate(pDC, -14, -20, false);
	CRect rc2 = CRect(0, 0, abs(forkRect.rclBounds.right - forkRect.rclBounds.left),
			abs(forkRect.rclBounds.top - forkRect.rclBounds.bottom));	
	pDC->PlayMetaFile(fork, rc2);
}
void CKlk2018BagerView::DrawExcavator(CDC* pDC)
{
	DrawBody(pDC);
	DrawArm1(pDC);
	DrawArm2(pDC);
	DrawFork(pDC);
}


// CKlk2018BagerView printing

BOOL CKlk2018BagerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlk2018BagerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlk2018BagerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKlk2018BagerView diagnostics

#ifdef _DEBUG
void CKlk2018BagerView::AssertValid() const
{
	CView::AssertValid();
}

void CKlk2018BagerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlk2018BagerDoc* CKlk2018BagerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlk2018BagerDoc)));
	return (CKlk2018BagerDoc*)m_pDocument;
}
#endif //_DEBUG


// CKlk2018BagerView message handlers


BOOL CKlk2018BagerView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}


void CKlk2018BagerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	switch (nChar)
	{
	case '1': bigCrcl -= 10; break;
	case '2': bigCrcl += 10; break;
	case '3': smolCrcl -= 10; break;
	case '4': smolCrcl += 10; break;
	case '5': forkRotat -= 10; break;
	case '6': forkRotat += 10; break;
	case VK_RIGHT: moveDx += 10; break;
	case VK_LEFT: moveDx -= 10; break;
	}
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}