
// Klk2018(Bagercina)View.cpp : implementation of the CKlk2018BagercinaView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk2018(Bagercina).h"
#endif

#include "Klk2018(Bagercina)Doc.h"
#include "Klk2018(Bagercina)View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define pi 3.14159

// CKlk2018BagercinaView

IMPLEMENT_DYNCREATE(CKlk2018BagercinaView, CView)

BEGIN_MESSAGE_MAP(CKlk2018BagercinaView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKlk2018BagercinaView construction/destruction

CKlk2018BagercinaView::CKlk2018BagercinaView() noexcept
{
	// TODO: add construction code here
	arm1 = new DImage();
	arm1->Load(CString("arm1.png"));
	arm2 = new DImage();
	arm2->Load(CString("arm2.png"));
	bkground = new DImage();
	bkground->Load(CString("pozadina.png"));
	bager = new DImage();
	bager->Load(CString("bager.png"));
	fork = GetEnhMetaFile(CString("viljuska.emf"));
	GetEnhMetaFileHeader(fork, sizeof(ENHMETAHEADER), &forkHeader);
	arm1Rotat = arm2Rotat = forkRotat = -90.0f;
	bagerMove = 0.0f;

}

CKlk2018BagercinaView::~CKlk2018BagercinaView()
{
	if (arm1) delete arm1; arm1 = nullptr;
	if (arm2) delete arm2; arm2 = nullptr;
	if (bager) delete bager; bager = nullptr;
	if (bkground) delete bkground; bkground = nullptr;

}

BOOL CKlk2018BagercinaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlk2018BagercinaView drawing

void CKlk2018BagercinaView::OnDraw(CDC* pDC)
{
	CKlk2018BagercinaDoc* pDoc = GetDocument();
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
	XFORM oldForm;
	memDC->GetWorldTransform(&oldForm);
	// TODO: add draw code for native data here
	DrawBackground(memDC);
	Translate(memDC, c.Width() - bager->Width() + bagerMove, c.Height() - bager->Height(), false);
	DrawExcavator(memDC);

	memDC->SetWorldTransform(&oldForm);
	pDC->BitBlt(0, 0, c.Width(), c.Height(), memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC();
}


void CKlk2018BagercinaView::Shear(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM mat = { 1,sX,sY,1,0,0 };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKlk2018BagercinaView::DrawBackground(CDC* pDC)
{
	/*koja iscrtava sliku pozadina.png, tako da
je uvek centrirana po X-osi, prilikom promene veličine prozora, a donja ivica se poklapa sa
donjom ivicom klijentskog dela prozora*/
	CRect rect;
	GetClientRect(&rect);

	CRect picRect(0,0,bkground->Width(), bkground->Height());
	bkground->Draw(pDC, CRect(picRect.CenterPoint().x - bkground->Width() / 2, picRect.bottom - bkground->Height(), picRect.CenterPoint().x + bkground->Width() / 2, picRect.bottom), rect);
		
	//CRect(rect.CenterPoint().x - bkground->Width()/ 2, rect.bottom - bkground->Height(), rect.CenterPoint().x + bkground->Width() / 2, rect.bottom));
}

void CKlk2018BagercinaView::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	pImage->DrawTransparent(pDC, CRect(0, 0, pImage->Width(), pImage->Height()), CRect(0, 0, pImage->Width(), pImage->Height()), NULL);
}

void CKlk2018BagercinaView::DrawBody(CDC* pDC)
{
	Shear(pDC, -1, 0, false);
	DrawImgTransparent(pDC, bager);
	Translate(pDC, 0, 108, false); //ovo 108 je lupeno
}

void CKlk2018BagercinaView::DrawArm1(CDC* pDC)
{
	////Deo se
	////	rotira oko centra većeg kruga, koji je na poziciji(58, 61) u slici.Centar manjeg kruga je na
	////	(309, 61).U okviru ove funkcije definisati rotaciju za ugao koji je atribut View klase i menja
	////	se na pritisak tastera.
	Translate(pDC, 58, 61, false);
	Rotate(pDC, arm1Rotat, false);
	Translate(pDC, -58, -61, false);
	DrawImgTransparent(pDC, arm1);
	Translate(pDC, 309, 61, false);
}

void CKlk2018BagercinaView::DrawArm2(CDC* pDC)
{
	Rotate(pDC, arm2Rotat, false);
	Translate(pDC, -36, -40, false);
	DrawImgTransparent(pDC, arm2);
	Translate(pDC, 272, 40, false);
}

void CKlk2018BagercinaView::DrawFork(CDC* pDC)
{
	Rotate(pDC, forkRotat, false);
	Scale(pDC, 2.5, 2.5, false);
	Translate(pDC, -14, -20, false);

	CRect forkRect( 0, 0, forkHeader.rclBounds.right - forkHeader.rclBounds.left,
		forkHeader.rclBounds.bottom - forkHeader.rclBounds.top);
	pDC->PlayMetaFile(fork,forkRect);

}

void CKlk2018BagercinaView::DrawExcavator(CDC* pDC)
{
	DrawBody(pDC);
	DrawArm1(pDC);
	DrawArm2(pDC);
	DrawFork(pDC);
}


// CKlk2018BagercinaView printing

BOOL CKlk2018BagercinaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlk2018BagercinaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlk2018BagercinaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKlk2018BagercinaView diagnostics

#ifdef _DEBUG
void CKlk2018BagercinaView::AssertValid() const
{
	CView::AssertValid();
}

void CKlk2018BagercinaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlk2018BagercinaDoc* CKlk2018BagercinaView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlk2018BagercinaDoc)));
	return (CKlk2018BagercinaDoc*)m_pDocument;
}
#endif //_DEBUG


// CKlk2018BagercinaView message handlers


BOOL CKlk2018BagercinaView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}


void CKlk2018BagercinaView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	switch (nChar)
	{
	case '1':	arm1Rotat -= 10; break;
	case '2':	arm1Rotat += 10; break;
	case '3':	arm2Rotat -= 10; break;
	case '4':	arm2Rotat += 10; break;
	case '5':	forkRotat -= 10; break;
	case '6':	forkRotat += 10; break;
	case VK_RIGHT:	bagerMove += 10; break;
	case VK_LEFT:	bagerMove -= 10; break;
	}
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
