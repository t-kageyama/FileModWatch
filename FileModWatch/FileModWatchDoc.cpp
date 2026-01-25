
// FileModWatchDoc.cpp : implementation of the CFileModWatchDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "FileModWatch.h"
#endif

#include "FileModWatchDoc.h"
#include "FileModWatchView.h"

#include <propkey.h>

#define FILE_HEADER		MAKELONG(MAKEWORD('F', 'M'), MAKEWORD('D', 'W'))
#define FILE_VERSION	MAKELONG(MAKEWORD(1, 0), MAKEWORD(0, 0))

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFileModWatchDoc

IMPLEMENT_DYNCREATE(CFileModWatchDoc, CDocument)

BEGIN_MESSAGE_MAP(CFileModWatchDoc, CDocument)
END_MESSAGE_MAP()


// CFileModWatchDoc construction/destruction

CFileModWatchDoc::CFileModWatchDoc()
{
	// TODO: add one-time construction code here

}

CFileModWatchDoc::~CFileModWatchDoc()
{
}

BOOL CFileModWatchDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	m_watchDir = _T("");
	m_copyToDir = _T("");
	m_watchFiles.RemoveAll();

	UpdateAllViews(NULL);
	return TRUE;
}

BOOL CFileModWatchDoc::IsReady() const
{
	if (m_watchDir.GetLength() < 1) {
		return FALSE;
	}
	if (m_copyToDir.GetLength() < 1) {
		return FALSE;
	}
	return TRUE;
}

// CFileModWatchDoc serialization

void CFileModWatchDoc::Serialize(CArchive& ar)
{
	DWORD header = FILE_HEADER;
	DWORD version = FILE_VERSION;
	DWORD count = (DWORD) m_watchFiles.GetCount();
	if (ar.IsStoring())
	{
		// add storing code here
		ar << header;
		ar << version;
		ar << m_watchDir;
		ar << m_copyToDir;
		ar << count;
		for (DWORD i = 0; i < count; i++) {
			CString file = m_watchFiles.GetAt(i);
			ar << file;
		}

		//POSITION pos = GetFirstViewPosition();
		//CFileModWatchView *const view = (CFileModWatchView*) GetNextView(pos);
		//if (view != NULL) {
		//
		//}
		UpdateAllViews(NULL);
	}
	else
	{
		// add loading code here
		m_watchFiles.RemoveAll();
		ar >> header;
		if (header != FILE_HEADER) {
			// invalid file.
			AfxThrowArchiveException(CArchiveException::badIndex);
		}
		ar >> version;
		if (version != FILE_VERSION) {
			// invalid version.
			AfxThrowArchiveException(CArchiveException::badIndex);
		}
		ar >> m_watchDir;
		ar >> m_copyToDir;
		ar >> count;
		for (DWORD i = 0; i < count; i++) {
			CString file;
			ar >> file;
			m_watchFiles.Add(file);
		}
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CFileModWatchDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CFileModWatchDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CFileModWatchDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CFileModWatchDoc diagnostics

#ifdef _DEBUG
void CFileModWatchDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFileModWatchDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFileModWatchDoc commands
