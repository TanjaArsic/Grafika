
// Klk2019(Transformers)View.cpp : implementation of the CKlk2019TransformersView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk2019(Transformers).h"
#endif

#include "Klk2019(Transformers)Doc.h"
#include "Klk2019(Transformers)View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define pi 3.14159

// CKlk2019TransformersView

IMPLEMENT_DYNCREATE(CKlk2019TransformersView, CView)

BEGIN_MESSAGE_MAP(CKlk2019TransformersView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKlk2019TransformersView construction/destruction

CKlk2019TransformersView::CKlk2019TransformersView() noexcept
{
	// TODO: add construction code here
	body1 = new DImage(); 	body1->Load(CString("body1.png"));
	arm1 = new DImage(); 	arm1->Load(CString("arm1.png"));
	arm2 = new DImage(); 	arm2->Load(CString("arm2.png"));
	leg1 = new DImage(); 	leg1->Load(CString("leg1.png"));
	leg2 = new DImage(); 	leg2->Load(CString("leg2.png"));
	background = new DImage(); 	background->Load(CString("background.jpg"));

	rotArm1 = rotArm2 = rotLeg1 = rotLeg2 = rotBody = 0.0f;
	rotArm2 = 180;
	move = 0;


}

CKlk2019TransformersView::~CKlk2019TransformersView()
{
	if (body1) delete body1; body1 = nullptr;
	if (arm1) delete arm1; arm1 = nullptr;
	if (arm2) delete arm2; arm2 = nullptr;
	if (leg1) delete leg1; leg1 = nullptr;
	if (leg2) delete leg2; leg2 = nullptr;
	if (background) delete background; background = nullptr;
}

BOOL CKlk2019TransformersView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKlk2019TransformersView drawing

void CKlk2019TransformersView::OnDraw(CDC* pDC)
{
	CKlk2019TransformersDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientRect;
	GetClientRect(&clientRect);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, clientRect.Width(), clientRect.Height());

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	memDC->SelectObject(&bmp);
	DrawBackground(memDC, clientRect);
	XFORM oldForm;
	memDC->SetGraphicsMode(GM_ADVANCED);
	memDC->GetWorldTransform(&oldForm);
	// TODO: add draw code for native data here
	
	Translate(memDC, 200+move, 400, false);
	DrawTransformer(memDC);


	memDC->SetWorldTransform(&oldForm);
	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC();
}

void CKlk2019TransformersView::DrawBackground(CDC* pDC, CRect rc)
{
	background->Draw(pDC, CRect(0, 0, background->Width(), background->Height()), rc);
}

void CKlk2019TransformersView::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	CRect imgRect(0, 0, pImage->Width(), pImage->Height());
	pImage->DrawTransparent(pDC, imgRect, imgRect, NULL);

}

void CKlk2019TransformersView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM mat = { 1,0,0,1,dX,dY };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

float CKlk2019TransformersView::ToRad(float angle)
{
	return angle * pi/180;
}

void CKlk2019TransformersView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float radAngle = ToRad(angle);
	XFORM mat = { cos(radAngle),sin(radAngle),-sin(radAngle),cos(radAngle),0,0 };
	pDC->ModifyWorldTransform(&mat, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKlk2019TransformersView::DrawArm1(CDC* pDC)
{
	Translate(pDC, 210, 87, false); // ovo se ubacuje kad se nadovezuje na body
	//Translate(pDC, 34, 31, false);
	Rotate(pDC, rotArm1, false);
	Translate(pDC, -34, -31, false);
	DrawArm2(pDC); // pošto treba druga ruka da se iscrta pre prve, 
	DrawImgTransparent(pDC, arm1);

}

void CKlk2019TransformersView::DrawArm2(CDC* pDC)
{
	// ide prvo pomeraj za 210,102 u tačku rotacije druge ruke
	XFORM secondArm;
	pDC->GetWorldTransform(&secondArm);
	Translate(pDC, 210, 102, false);
	Rotate(pDC, rotArm2, false);
	Translate(pDC, -23, -61, false);  // vraca se na gornji levi ugao i iscrtava drugu ruku
	DrawImgTransparent(pDC, arm2);
	pDC->SetWorldTransform(&secondArm);

}
void CKlk2019TransformersView::DrawLeg1(CDC* pDC)
{

	Translate(pDC, 30, 125, false);
	Rotate(pDC, rotLeg1, false);
	DrawLeg2(pDC); // jer prvo ide druga noga
	Translate(pDC, -30, -125, false);
	DrawImgTransparent(pDC, leg1);
	
	Translate(pDC, 237, 125, false);

}
void CKlk2019TransformersView::DrawLeg2(CDC* pDC)
{
	XFORM secondLeg;
	pDC->GetWorldTransform(&secondLeg);
	Rotate(pDC, rotLeg2, false);
	Translate(pDC, -35, -60, false);
	DrawImgTransparent(pDC, leg2);
	pDC->SetWorldTransform(&secondLeg);

}
void CKlk2019TransformersView::DrawBody1(CDC* pDC)
{
	Rotate(pDC, rotBody, false);
	Translate(pDC, -26, -133, false);
	DrawImgTransparent(pDC, body1);
}
void CKlk2019TransformersView::DrawTransformer(CDC* pDC)
{
	DrawLeg1(pDC);
	DrawBody1(pDC);
	DrawArm1(pDC);

}


//void CKlk2019TransformersView::DrawArm1(CDC* pDC)
//{
//	/*rotira oko tačke sa koordinatama (34,31), a drugi deo ruke se rotira oko tačke
//	(210,102). Menja se na pritisak tastera Q i A.*/
//	Translate(pDC, 34, 31, false);
//	Rotate(pDC, rotArm1, false);
//	Translate(pDC, -34, -31, false);
//	DrawImgTransparent(pDC, arm1);
//
//}

//void CKlk2019TransformersView::DrawArm2(CDC* pDC)
//{
//	/*rotira oko tačke sa koordinatama (23,61) u slici i poklapa se sa 
//	odgovarajućom tačkom na	slici arm1.png. menja se na pritisak tastera T i G.*/
//	XFORM ndarm;
//	pDC->GetWorldTransform(&ndarm);
//	Translate(pDC, 210, 102, false);
//	Rotate(pDC, rotArm2, false);
//	Translate(pDC, -23, -61, false);
//	DrawImgTransparent(pDC, arm2);
//	pDC->SetWorldTransform(&ndarm);
//}

//void CKlk2019TransformersView::DrawLeg1(CDC* pDC)
//{
//	Translate(pDC, 30, 125, false);
//	Rotate(pDC, rotLeg1, false);
//	Translate(pDC, -30, - 125, false);
//
//	DrawImgTransparent(pDC, leg1);
//
//}

//void CKlk2019TransformersView::DrawLeg2(CDC* pDC)
//{
//	XFORM ndleg;
//	pDC->GetWorldTransform(&ndleg);
//	Translate(pDC, 30, 125, false);
//	Rotate(pDC, rotLeg2, false);
//	Translate(pDC, -35, -60, false);
//	DrawImgTransparent(pDC, leg2);
//
//	pDC->SetWorldTransform(&ndleg);
//}

//void CKlk2019TransformersView::DrawBody1(CDC* pDC)
//{	
//
//	Translate(pDC, 237, 125, false);
//	Rotate(pDC, rotBody, false);
//	Translate(pDC, -26, -133, false);
//
//	DrawImgTransparent(pDC, body1);
//
//	Translate(pDC, 178, 54, false); //pozicija za crtanje prve ruke
//
//}

//void CKlk2019TransformersView::DrawTransformer(CDC* pDC)
//{
//	DrawLeg1(pDC);
//	DrawLeg2(pDC);
//	DrawBody1(pDC);
//	DrawArm1(pDC);
//	DrawArm2(pDC);
//
//}



void CKlk2019TransformersView::DrawCenter(CDC* pDC)
{

	CPen dot(PS_SOLID, 3, RGB(255, 255, 255));
	CPen* oldPen = pDC->SelectObject(&dot);
	pDC->MoveTo(-5, -5);
	pDC->LineTo(5, 5);
	pDC->MoveTo(-5, 5);
	pDC->LineTo(5, -5);
	pDC->SelectObject(oldPen);
	dot.DeleteObject();
}


// drugi put

// CKlk2019TransformersView printing

BOOL CKlk2019TransformersView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKlk2019TransformersView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKlk2019TransformersView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKlk2019TransformersView diagnostics

#ifdef _DEBUG
void CKlk2019TransformersView::AssertValid() const
{
	CView::AssertValid();
}

void CKlk2019TransformersView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKlk2019TransformersDoc* CKlk2019TransformersView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKlk2019TransformersDoc)));
	return (CKlk2019TransformersDoc*)m_pDocument;
}
#endif //_DEBUG


// CKlk2019TransformersView message handlers


void CKlk2019TransformersView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == 'Q')
	{
		rotArm1 += 10;
		Invalidate();
	}
	if (nChar == 'A')
	{
		rotArm1 -= 10;
		Invalidate();
	}
	if (nChar == 'T')
	{
		rotArm2 += 10;
		Invalidate();
	}
	if (nChar == 'G')
	{
		rotArm2 -= 10;
		Invalidate();
	}
	if (nChar == 'E')
	{
		rotLeg1 += 10;
		Invalidate();
	}
	if (nChar == 'D')
	{
		rotLeg1 -= 10;
		Invalidate();
	}
	if (nChar == 'R')
	{
		rotLeg2 += 10;
		Invalidate();
	}
	if (nChar == 'F')
	{
		rotLeg2 -= 10;
		Invalidate();
	}
	if (nChar == 'W')
	{
		rotBody += 10;
		Invalidate();
	}
	if (nChar == 'S')
	{
		rotBody -= 10;
		Invalidate();
	}

	if (nChar == VK_LEFT && move > 0)
	{
		rotArm2 += 15; 
		rotArm1 += 15; 
		rotBody -= 1; 
		rotLeg1 += 6; 
		rotLeg2 -= 12;
		move -= 30;
		Invalidate();
	}
	if (nChar == VK_RIGHT && move < 550)
	{
		rotArm2 -= 15;
		rotArm1 -= 15;
		rotBody += 1;
		rotLeg1 -= 6;
		rotLeg2 += 12;
		move += 30;

		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CKlk2019TransformersView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}
