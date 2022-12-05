
// blanketView.cpp : implementation of the CblanketView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "blanket.h"
#endif

#include "blanketDoc.h"
#include "blanketView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define pi 3.14159

// CblanketView

IMPLEMENT_DYNCREATE(CblanketView, CView)

BEGIN_MESSAGE_MAP(CblanketView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CblanketView construction/destruction

CblanketView::CblanketView() noexcept
{
	//slike su atributi, objekti DImage
	bager = new DImage();
	bager->Load(CString("bager.png"));
	arm1 = new DImage();
	arm1->Load(CString("arm1.png"));
	arm2 = new DImage();
	arm2->Load(CString("arm2.png"));
	pozadina = new DImage();
	pozadina->Load(CString("pozadina.png"));

	arm1Rotate = 0;
	arm2Rotate = 0;
	forkRotate = 0;
	gas = 0;

	//metafajl viljuska.emf
	fork = GetEnhMetaFile(CString("viljuska.emf"));
	GetEnhMetaFileHeader(fork, sizeof(ENHMETAHEADER), &header_fork);
	


}

CblanketView::~CblanketView()
{
	delete bager;
	delete arm1;
	delete arm2;
	delete pozadina;
	DeleteEnhMetaFile(fork);

}

BOOL CblanketView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CblanketView drawing

float CblanketView::torad(float angle)
{
	return angle * pi / 180;
}

void CblanketView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM t = { 1,0,0,1,dX,dY };
	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CblanketView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM t = { cos(torad(angle)),sin(torad(angle)),-sin(torad(angle)),cos(torad(angle)),0,0 };
	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CblanketView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM t = { sX,0,0,sY,0,0 };
	pDC->ModifyWorldTransform(&t, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void CblanketView::DrawBackground(CDC* pDC)
{
	CRect klijent;
	GetClientRect(&klijent);
	pozadina->Draw(pDC, CRect(0, 0, pozadina->Width(), pozadina->Height()), klijent); // Iscrtava sliku u datom DC-j

}
void CblanketView::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	pImage->DrawTransparent(pDC, CRect(0, 0, pImage->Width(), pImage->Height()), CRect(0, 0, pImage->Width(), pImage->Height()), NULL);
}


void CblanketView::DrawBody(CDC* pDC)
{
	DrawImgTransparent(pDC, bager); //crtam bager i 
	Translate(pDC, 0, arm1->Width() / 1.5, false); //pomera se lokalni koordinatni sistem za ruku da se nalazi ispred kabine, otprilike za 1.5*sirina slike ruke1
	Rotate(pDC, -90, false); //obrcem je

}


void CblanketView::DrawArm1(CDC* pDC)
{

	Translate(pDC, 58, 61, false); //centar rotacije oko veceg kruga na poziciji(58, 61) u slici, to pise u zadatku, PRE CRTANJA SLIKE
	Rotate(pDC, arm1Rotate, false);  //trt<3
	Translate(pDC, -58, -61, false); //vracam se
	DrawImgTransparent(pDC, arm1); //crtam ruku

	Translate(pDC, 309, 61, false); //postavljam se na zadati centar manjeg kruga, dato u zadatku
	Rotate(pDC, -90, false);
}

void CblanketView::DrawArm2(CDC* pDC)
{
	Rotate(pDC, arm2Rotate, false);
	Translate(pDC, -36, -40, false); //vracam se u lokalni koord pocetak za crtanje manje ruke

	DrawImgTransparent(pDC, arm2);

	Translate(pDC, 272, 40, false);
	Rotate(pDC, -90, false); //dodatnih pi pola jer je viljushka takwa

}

void CblanketView::DrawFork(CDC* pDC)
{
	Rotate(pDC, forkRotate, false);
	Scale(pDC, 2.5, 2.5, false);
	Translate(pDC, -14, -20, false); //zadali pogresno koordinate a mene zabole da zabadam
	//update nisu nego mi nije radio lepo metafajl
	//CRect kvad(header_fork.rclBounds.left, header_fork.rclBounds.top, header_fork.rclBounds.right, header_fork.rclBounds.bottom);

	CRect rc2 = CRect(0,0, abs(header_fork.rclBounds.right- header_fork.rclBounds.left),
		abs(header_fork.rclBounds.top- header_fork.rclBounds.bottom));

	pDC->PlayMetaFile(fork, rc2);
	Scale(pDC, -2.5, -2.5, false); //vraca se na originalne koordinate

}

void CblanketView::DrawExcavator(CDC* pDC)
{
	XFORM old;
	pDC->GetWorldTransform(&old);
	CRect cli;
	GetClientRect(&cli);
	Translate(pDC, cli.Width() - bager->Width() - gas, cli.Height() - bager->Height(), false); //gas je kretanje po x-osi, bager se inicijalno nalazi u donjem desnom uglu prozora.
	DrawBody(pDC);
	DrawArm1(pDC);
	DrawArm2(pDC);
	DrawFork(pDC);
	pDC->SetWorldTransform(&old); //vracam se na staro i normalno posle onih transformacija
}


void CblanketView::OnDraw(CDC* pDC)
{
	CblanketDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect cr;
	GetClientRect(&cr);
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	CBitmap* bmp = new CBitmap();
	bmp->CreateCompatibleBitmap(pDC, cr.Width(), cr.Height());
	memDC->SelectObject(bmp);
	XFORM oldXform;
	memDC->GetWorldTransform(&oldXform);
	memDC->SetGraphicsMode(GM_ADVANCED);

	DrawBackground(memDC);
	DrawExcavator(memDC);
	pDC->BitBlt(0, 0, cr.Width(), cr.Height(), memDC, 0, 0, SRCCOPY);
	memDC->SetWorldTransform(&oldXform);
	memDC->DeleteDC();
	delete memDC;
	// 
	// TODO: add draw code for native data here
	

}


// CblanketView printing

BOOL CblanketView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CblanketView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CblanketView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CblanketView diagnostics

#ifdef _DEBUG
void CblanketView::AssertValid() const
{
	CView::AssertValid();
}

void CblanketView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CblanketDoc* CblanketView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CblanketDoc)));
	return (CblanketDoc*)m_pDocument;
}
#endif //_DEBUG


// CblanketView message handlers


void CblanketView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	if (nChar == '1')
	{
		arm1Rotate -= 10;
		Invalidate();
		
	}
	if (nChar == '2')
	{
		arm1Rotate += 10;
		Invalidate();

	}
	if (nChar == '3')
	{
		arm2Rotate -= 10;
		Invalidate();

	}
	if (nChar == '4')
	{
		arm2Rotate += 10;
		Invalidate();

	}
	if (nChar == '5')
	{
		forkRotate -=10;
		Invalidate();

	}
	if (nChar == '6')
	{
		forkRotate += 10;
		Invalidate();

	}
	if (nChar == VK_RIGHT)
	{
		gas -= 10;
		Invalidate();

	}
	if (nChar == VK_LEFT)
	{
		gas += 10;
		Invalidate();

	}

	

}


BOOL CblanketView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}
