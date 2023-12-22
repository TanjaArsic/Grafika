
// Klk2019(BilijarGLupi)Doc.cpp : implementation of the CKlk2019BilijarGLupiDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Klk2019(BilijarGLupi).h"
#endif

#include "Klk2019(BilijarGLupi)Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CKlk2019BilijarGLupiDoc

IMPLEMENT_DYNCREATE(CKlk2019BilijarGLupiDoc, CDocument)

BEGIN_MESSAGE_MAP(CKlk2019BilijarGLupiDoc, CDocument)
END_MESSAGE_MAP()


// CKlk2019BilijarGLupiDoc construction/destruction

CKlk2019BilijarGLupiDoc::CKlk2019BilijarGLupiDoc() noexcept
{
	// TODO: add one-time construction code here

}

CKlk2019BilijarGLupiDoc::~CKlk2019BilijarGLupiDoc()
{
}

BOOL CKlk2019BilijarGLupiDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CKlk2019BilijarGLupiDoc serialization

void CKlk2019BilijarGLupiDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CKlk2019BilijarGLupiDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CKlk2019BilijarGLupiDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CKlk2019BilijarGLupiDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CKlk2019BilijarGLupiDoc diagnostics

#ifdef _DEBUG
void CKlk2019BilijarGLupiDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CKlk2019BilijarGLupiDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CKlk2019BilijarGLupiDoc commands
