
// MonaLisaView.cpp : implementation of the CMonaLisaView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MonaLisa.h"
#endif

#include "MonaLisaDoc.h"
#include "MonaLisaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define pi 3.141592653589793


// CMonaLisaView

IMPLEMENT_DYNCREATE(CMonaLisaView, CView)

BEGIN_MESSAGE_MAP(CMonaLisaView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMonaLisaView construction/destruction

CMonaLisaView::CMonaLisaView() noexcept
{
	// TODO: add construction code here

	this->dim = 500;
	this->numOfSquares = 20;
	this->grid = dim / numOfSquares;
	puzzle = new DImage * [3];
	for (int i = 0; i < 3; i++)
	{
		puzzle[i] = new DImage[3];
	}
	
	// ovoa moze i u petlju
	puzzle[0][0].Load(CString("pic1.dib"));
	puzzle[0][1].Load(CString("pic2.dib"));
	puzzle[0][2].Load(CString("pic3.dib"));
	puzzle[1][0].Load(CString("pic4.dib"));
	puzzle[1][1].Load(CString("pic5.dib"));
	puzzle[1][2].Load(CString("pic6.dib"));
	puzzle[2][0].Load(CString("pic7.dib"));
	puzzle[2][1].Load(CString("pic8.dib"));
	puzzle[2][2].Load(CString("pic9.dib"));

	/*for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			puzzle[i][j].MakeItGrey();*/

}

CMonaLisaView::~CMonaLisaView()
{
	for (int i = 0; i < 3; i++)
		delete[] puzzle[i];
	delete[] puzzle;
}

BOOL CMonaLisaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMonaLisaView drawing
//void CMonaLisaView::DrawTransparent(CDC* pDC, DImage* img)
//{
//	//Kreiramo masku 1 bit po pikselu monohromatska
//	CBitmap mask;
//	mask.CreateBitmap(img->Width(), img->Height(), 1, 1, NULL);
//	//Kopiramo bajtove iz DImage u Bitmapu sa kojom radimo
//	CBitmap image;
//	image.CreateBitmap(img->Width(), img->Height(), 4, 8, NULL);
//	image.SetBitmapBits(img->Width() * img->Height() * img->BPP(), img->GetDIBBits());
//
//	CDC* srcDC = new CDC();
//	CDC* dstDC = new CDC();
//	srcDC->CreateCompatibleDC(pDC);
//	dstDC->CreateCompatibleDC(pDC);
//
//	CBitmap* srcOldBitmap = srcDC->SelectObject(&image);
//	CBitmap* dstOldBitmap = dstDC->SelectObject(&mask);
//
//	//Pravimo masku
//	COLORREF removeColor = srcDC->GetPixel(0, 0);
//	COLORREF srcOldBckColor = srcDC->SetBkColor(removeColor);
//	dstDC->BitBlt(0, 0, img->Width(), img->Height(), srcDC, 0, 0, SRCCOPY);
//
//	//Menjamo boje i za bk i text i odstranjujemo pozadinsku boju
//	COLORREF srcOldTextColor = srcDC->SetTextColor(RGB(255, 255, 255));
//	srcDC->SetBkColor(RGB(0, 0, 0));
//	srcDC->BitBlt(0, 0, img->Width(), img->Height(), dstDC, 0, 0, SRCAND);
//
//	dstDC->SelectObject(dstOldBitmap);
//	delete dstDC;
//
//
//	//Selektujemo masku i ANDUJEMO sa pDC, tj u nasem slucaju tu se prosledjuje memDC
//	srcDC->SelectObject(&mask);
//	pDC->BitBlt(0,0, img->Width(), img->Height(), srcDC, 0, 0, SRCAND);
//	// I sad crtamo sliku tj OR
//	srcDC->SelectObject(&image);
//	pDC->BitBlt(0, 0, img->Width(), img->Height(), srcDC, 0, 0, SRCPAINT);
//	srcDC->SelectObject(srcOldBitmap);
//	delete srcDC;
//}
//
void CMonaLisaView::DrawGrid(CDC* pDC)
{
	CPen newPen(PS_SOLID, 1, RGB(128, 128, 128));
	CPen* oldPen = pDC->SelectObject(&newPen);

	int x = 0, y = 0;
	while (x <= this->dim)
	{
		pDC->MoveTo(x, 0);
		pDC->LineTo(x, this->dim);
		x += this->grid;
	}
	while (y <= this->dim)
	{
		pDC->MoveTo(0, y);
		pDC->LineTo(this->dim, y);
		y += this->grid;
	}
	pDC->SelectObject(oldPen);
	newPen.DeleteObject();


}
//
////void CMonaLisaView::DrawTransparent(CDC* pDC, DImage img)
////{
////	CBitmap bmpImage;
////	CBitmap bmpMask;
////	bmpMask.CreateBitmap(img.Width(), img.Height(), 1, 1, NULL);
////	bmpImage.CreateBitmap(img.Width(), img.Height(), 4, 8, NULL);
////
////	CDC* srcDC = new CDC();
////	srcDC->CreateCompatibleDC(pDC);
////	CDC* dstDC = new CDC();
////	dstDC->CreateCompatibleDC(pDC);
////	CBitmap* srcOldBitmap = srcDC->SelectObject(&bmpImage);
////	CBitmap* dstOldBitmap = dstDC->SelectObject(&bmpMask);
////
////	// popunjavanje monohromatske bitmape
////	COLORREF topLeftCorner = srcDC->GetPixel(0, 0);
////	srcDC->SetBkColor(topLeftCorner);
////	dstDC->BitBlt(0, 0, img.Width(), img.Height(), srcDC, 0, 0, SRCCOPY);
////	
////	srcDC->SetTextColor(RGB(255, 255, 255));
////	srcDC->SetBkColor(RGB(0, 0, 0));
////	srcDC->BitBlt(0, 0, img.Width(), img.Height(), dstDC, 0, 0, SRCAND);
////
////	dstDC->SelectObject(dstOldBitmap);
////	delete dstDC;
////
////	// selektujemo masku i ANDUJEMO sa pDC, tj u nasem slucaju tu se prosledjuje memDC
////	srcDC->SelectObject(&bmpMask);
////	pDC->BitBlt(0,0, img.Width(), img.Height(), srcDC, 0, 0, SRCAND);
////	// sad crtamo sliku tj OR
////	srcDC->SelectObject(bmpImage);
////	pDC->BitBlt(0, 0, img.Width(), img.Height(), srcDC, 0, 0, SRCPAINT);
////	srcDC->SelectObject(srcOldBitmap);
////	delete srcDC;
////
////}
//
////void CMonaLisaView::OnDraw(CDC* pDC)
////{
////	CMonaLisaDoc* pDoc = GetDocument();
////	ASSERT_VALID(pDoc);
////	if (!pDoc)
////		return;
////
////	CRect clientWindow;
////	GetClientRect(&clientWindow);
////	CDC* memDC = new CDC();
////	memDC->CreateCompatibleDC(pDC);
////	CBitmap bmp;
////	bmp.CreateCompatibleBitmap(pDC, clientWindow.Width(), clientWindow.Height());
////
////	memDC->SelectObject(bmp);
////	//====================
////	int oldGMode = memDC->SetGraphicsMode(GM_ADVANCED);
////	XFORM oldForm;
////	memDC->GetWorldTransform(&oldForm);
////	// TODO: add draw code for native data here
////	Translate(memDC, 200, 200, false);
////	DrawTransparent(memDC, puzzle[0][0]);
////
////	memDC->SetGraphicsMode(oldGMode);
////	//====================
////	//Kopiramo MemDC u pDC
////	//MemDCTopDC(memDC, pDC);
////	delete memDC;
////
////
////}
//void CMonaLisaView::OnDraw(CDC* pDC)
//{
//	CMonaLisaDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
//	if (!pDoc)
//		return;
//
//	CBitmap b;
//	XFORM t;
//	CRect k;
//	GetClientRect(&k);
//
//	CDC* memDC = new CDC();
//	memDC->CreateCompatibleDC(pDC);
//	b.CreateCompatibleBitmap(pDC, k.Width(), k.Height());
//	memDC->SelectObject(&b);
//	int oldGMode = memDC->SetGraphicsMode(GM_ADVANCED);
//	memDC->GetWorldTransform(&t);
//
//	DrawGrid(memDC);
//	//prva
//	Translate(memDC, -1 * this->grid, -3.2 * this->grid, false);
//	Rotate(memDC, 9, false);
//	//Mirror(memDC, true, false/, false);
//	for (int i = 0; i < 3; i++)
//		for (int j = 0; j < 3; j++)
//			DrawTransparent(memDC, &puzzle[i][j]);
//	//DrawTransparent(memDC, &puzzle[0][0]);
//		
//	//memDC->SetWorldTransform(&t);
//	//pDC->BitBlt(0, 0, k.Width(), k.Height(), memDC, 0, 0, SRCCOPY);
//	//memDC->DeleteDC();
//}
//
//
//void CMonaLisaView::MemDCTopDC(CDC* memDC, CDC* pDC)
//{
//	CRect clientWindow;
//	GetClientRect(&clientWindow);
//	//Kreiramo masku
//	CBitmap mask;
//	mask.CreateBitmap(clientWindow.Width(), clientWindow.Height(), 1, 1, NULL);
//	CDC* dstDC = new CDC();
//	dstDC->CreateCompatibleDC(memDC);
//	dstDC->SelectObject(&mask);
//	//Postavljamo memDC boje
//	memDC->SetBkColor(RGB(0, 0, 0));
//	memDC->SetTextColor(RGB(255, 255, 255));
//	//pravimo masku 
//	dstDC->BitBlt(0, 0, clientWindow.Width(), clientWindow.Height(), memDC, 0, 0, SRCPAINT);
//	//Radimo AND sa pDC, da napravimo masku gde ce da bude iscrtano
//	pDC->BitBlt(0, 0, clientWindow.Width(), clientWindow.Height(), dstDC, 0, 0, SRCAND);
//	delete dstDC;
//	//Crtamo sliku
//	pDC->BitBlt(0, 0, clientWindow.Width(), clientWindow.Height(), memDC, 0, 0, SRCPAINT);
//
//}

void CMonaLisaView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM matrix = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&matrix, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CMonaLisaView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM matrix = { sX, 0, 0, sY, 0, 0 };
	pDC->ModifyWorldTransform(&matrix, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

float CMonaLisaView::ToRad(float angle)
{
	return angle * pi / 180;
}

void CMonaLisaView::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply)
{
	XFORM form = { mx ? -1 : 1, 0,0,my ? -1 : 1,0,0 };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CMonaLisaView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM matrix = { cos(ToRad(angle)), sin(ToRad(angle)), -sin(ToRad(angle)), cos(ToRad(angle)), 0, 0};
	pDC->ModifyWorldTransform(&matrix, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void CMonaLisaView::DrawPiece(CDC* pDC, DImage* img, int dx, int dy, float angle, bool mx, bool my, bool blueFilter)
{
	XFORM xform;
	pDC->GetWorldTransform(&xform);
	int x = 0;

	CBitmap* bmpImg = new CBitmap();
	bmpImg->CreateBitmap(img->Width(), img->Height(), 1, 32, NULL);//bez ovoga nece
	bmpImg->SetBitmapBits(img->Height() * img->Width() * img->BPP(), img->GetDIBBits());
	BITMAP bm;
	bmpImg->GetBitmap(&bm);//ispunjava bitmap strukturu inf o bitmapi
	//kreiranje maske
	CBitmap bmpMask;
	bmpMask.CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

	//kreiranje device kontexta
	CDC* SrcDc = new CDC();
	CDC* DstDc = new CDC();
	SrcDc->CreateCompatibleDC(pDC);
	DstDc->CreateCompatibleDC(pDC);
	//pamcenje prethodnog sadrzaja
	CBitmap* pOldSrcBmp = SrcDc->SelectObject(bmpImg);
	CBitmap* pOldDstBmp = DstDc->SelectObject(&bmpMask);

	//izvlacenje boje i postavljanje za pozadinu od srcdc
	COLORREF clrTopLeft = SrcDc->GetPixel(0, 0);
	COLORREF clrSaveBk = SrcDc->SetBkColor(clrTopLeft);
	//kopiranje iz src u dst
	DstDc->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, SrcDc, 0, 0, SRCCOPY);


	COLORREF clrSavestText = SrcDc->SetTextColor(RGB(255, 255, 255));
	SrcDc->SetBkColor(RGB(0, 0, 0));
	SrcDc->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, DstDc, 0, 0, SRCAND);

	DstDc->SetTextColor(clrSavestText);
	SrcDc->SetBkColor(clrSaveBk);
	SrcDc->SelectObject(pOldSrcBmp);
	DstDc->SelectObject(pOldDstBmp);
	SrcDc->DeleteDC();
	delete SrcDc;
	DstDc->DeleteDC();
	delete DstDc;


	Translate(pDC, dx, dy, false);
	Mirror(pDC, mx, my, false);
	Rotate(pDC, angle, false);


	CDC* MemDc = new CDC();
	MemDc->CreateCompatibleDC(pDC);
	CBitmap* mbpOldT = MemDc->SelectObject(&bmpMask);
	pDC->BitBlt(-bm.bmWidth / 2, -bm.bmHeight / 2, bm.bmWidth, bm.bmHeight, MemDc, 0, 0, SRCAND);
	if (blueFilter)
		toGrayScale(bmpImg, bm, true);
	else {
	toGrayScale(bmpImg, bm, false);
}
	MemDc->SelectObject(bmpImg);
	pDC->BitBlt(-bm.bmWidth / 2, -bm.bmHeight / 2, bm.bmWidth, bm.bmHeight, MemDc, 0, 0, SRCPAINT);

	MemDc->SelectObject(mbpOldT);
	delete MemDc;

	pDC->SetWorldTransform(&xform);

}

void CMonaLisaView::toGrayScale(CBitmap* bmpImg, BITMAP bm, bool filter)
{
	long dwCount = bm.bmWidth * bm.bmHeight * 4;
	unsigned char* lpBits = new unsigned char[dwCount];
	ZeroMemory(lpBits, dwCount);
	bmpImg->GetBitmapBits(dwCount, lpBits);
	for (long i = 0; i < dwCount / 4; i++) 
	{
		int offset = i * 4;
		unsigned char& R = lpBits[offset + 2]; // Red component
		unsigned char& G = lpBits[offset + 1]; // Green component
		unsigned char& B = lpBits[offset];     // Blue component

		if (filter) 
		{
			R = G = 0;
		}
		else 
		{
			unsigned char gray = (R + G + B) / 3;
			R = G = B = gray;
		}

	}
	bmpImg->SetBitmapBits(dwCount, lpBits);
	delete[] lpBits;
}

void CMonaLisaView::OnDraw(CDC* pDC)
{
	CMonaLisaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	DrawGrid(pDC);
	XFORM xform;
	pDC->GetWorldTransform(&xform);
	pDC->SetGraphicsMode(GM_ADVANCED);
	

	DrawPiece(pDC, &puzzle[0][0], 3.75 * this->grid, 4.2 * this->grid, -81, false, true, false);
	DrawPiece(pDC, &puzzle[0][1], 10.3 * this->grid, 3.9 * this->grid, 96, false, true, false);
	DrawPiece(pDC, &puzzle[0][2], 15.8 * this->grid, 3.75 * this->grid, -168, false, true, false);
	DrawPiece(pDC, &puzzle[1][0], 4.35 * this->grid, 10 * this->grid, 67, false, true, false);
	DrawPiece(pDC, &puzzle[1][1], 10.3 * this->grid, 10.2 * this->grid, 35, false, true, true);
	DrawPiece(pDC, &puzzle[1][2], 15.65 * this->grid, 10.1 * this->grid, 82, true, false, false);
	DrawPiece(pDC, &puzzle[2][0], 4.1 * this->grid, 16.3 * this->grid, -4, false, true, false);
	DrawPiece(pDC, &puzzle[2][1], 10 * this->grid, 15.6 * this->grid, -31, true, false, false);
	DrawPiece(pDC, &puzzle[2][2], 15.75 * this->grid, 16.2 * this->grid, -60, false, true, false);

	pDC->SetWorldTransform(&xform);

}


// CMonaLisaView printing

BOOL CMonaLisaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMonaLisaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMonaLisaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CMonaLisaView diagnostics

#ifdef _DEBUG
void CMonaLisaView::AssertValid() const
{
	CView::AssertValid();
}

void CMonaLisaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMonaLisaDoc* CMonaLisaView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMonaLisaDoc)));
	return (CMonaLisaDoc*)m_pDocument;
}
#endif //_DEBUG


// CMonaLisaView message handlers
