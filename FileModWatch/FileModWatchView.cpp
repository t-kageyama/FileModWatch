
// FileModWatchView.cpp : implementation of the CFileModWatchView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "FileModWatch.h"
#endif

#include "FileModWatchDoc.h"
#include "FileModWatchView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFileModWatchView

IMPLEMENT_DYNCREATE(CFileModWatchView, CFormView)

BEGIN_MESSAGE_MAP(CFileModWatchView, CFormView)
	ON_BN_CLICKED(IDC_BROWSE_BUTTON, &CFileModWatchView::OnBnClickedBrowseButton)
	ON_BN_CLICKED(IDC_BROWSETO_BUTTON, &CFileModWatchView::OnBnClickedBrowsetoButton)
	ON_BN_CLICKED(IDC_STOP_BUTTON, &CFileModWatchView::OnBnClickedStopButton)
	ON_BN_CLICKED(IDC_START_BUTTON, &CFileModWatchView::OnBnClickedStartButton)
	ON_EN_CHANGE(IDC_DIRECTORY_EDIT, &CFileModWatchView::OnEnChangeDirectoryEdit)
	ON_EN_CHANGE(IDC_WATCH_EDIT, &CFileModWatchView::OnEnChangeWatchEdit)
	ON_EN_CHANGE(IDC_COPYTO_EDIT, &CFileModWatchView::OnEnChangeCopytoEdit)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, &CFileModWatchView::OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &CFileModWatchView::OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CFileModWatchView::OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, &CFileModWatchView::OnUpdateFileSaveAs)
	ON_UPDATE_COMMAND_UI_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, &CFileModWatchView::OnUpdateFileMRU)
END_MESSAGE_MAP()

// CFileModWatchView construction/destruction

CFileModWatchView::CFileModWatchView()
	: CFormView(CFileModWatchView::IDD), m_editing(FALSE)
{
	// TODO: add construction code here
}

CFileModWatchView::~CFileModWatchView()
{
}

void CFileModWatchView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CFileModWatchView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CFileModWatchView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
}

void CFileModWatchView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	m_editing = TRUE;
	CFileModWatchDoc *const doc = (CFileModWatchDoc *)GetDocument();
	GetDlgItem(IDC_DIRECTORY_EDIT)->SetWindowTextW(doc->m_watchDir);
	CString watch;
	if (doc->m_watchFiles.GetCount() > 0) {
		watch = doc->m_watchFiles.GetAt(0);
		for (int i = 1; i < doc->m_watchFiles.GetCount(); i++) {
			watch += TEXT(";");
			watch += doc->m_watchFiles.GetAt(i);
		}
	}
	GetDlgItem(IDC_WATCH_EDIT)->SetWindowTextW(watch);
	GetDlgItem(IDC_COPYTO_EDIT)->SetWindowTextW(doc->m_copyToDir);
	m_editing = FALSE;

	CFileModWatchApp *const app = (CFileModWatchApp *)AfxGetApp();
	if (app->IsStartUp()) {
		CWnd *const mainWnd = AfxGetMainWnd();
		if (mainWnd != NULL) {
			mainWnd->PostMessageW(WM_SYSCOMMAND, SC_MINIMIZE);
			OnBnClickedStartButton();	// start watch.
		}
	}
}

// CFileModWatchView diagnostics

#ifdef _DEBUG
void CFileModWatchView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFileModWatchView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CFileModWatchDoc* CFileModWatchView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFileModWatchDoc)));
	return (CFileModWatchDoc*)m_pDocument;
}
#endif //_DEBUG


// CFileModWatchView message handlers


void CFileModWatchView::OnBnClickedBrowseButton()
{
	// TODO: Add your control notification handler code here
	CFileModWatchDoc *const doc = (CFileModWatchDoc *)GetDocument();
	CString folder = doc->m_watchDir;
	if (folder.GetLength() < 1)  {
		TCHAR my_documents[MAX_PATH];
		HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);
		if (result == S_OK) {
			folder = my_documents;
		}
	}
	CFolderPickerDialog dialog(folder);
	if (dialog.DoModal() == IDOK) {
		m_editing = TRUE;
		doc->m_watchDir = dialog.GetPathName();
		doc->SetModifiedFlag(TRUE);
		GetDlgItem(IDC_DIRECTORY_EDIT)->SetWindowTextW(doc->m_watchDir);
		m_editing = FALSE;
	}
}


void CFileModWatchView::OnBnClickedBrowsetoButton()
{
	// TODO: Add your control notification handler code here
	CFileModWatchDoc *const doc = (CFileModWatchDoc *)GetDocument();
	CString folder = doc->m_copyToDir;
	if (folder.GetLength() < 1)  {
		TCHAR my_documents[MAX_PATH];
		HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);
		if (result == S_OK) {
			folder = my_documents;
		}
	}
	CFolderPickerDialog dialog(folder);
	if (dialog.DoModal() == IDOK) {
		m_editing = TRUE;
		doc->m_copyToDir = dialog.GetPathName();
		doc->SetModifiedFlag(TRUE);
		GetDlgItem(IDC_COPYTO_EDIT)->SetWindowTextW(doc->m_copyToDir);
		m_editing = FALSE;
	}
}


void CFileModWatchView::OnBnClickedStopButton()
{
	// TODO: Add your control notification handler code here
	CFileModWatchApp *const app = (CFileModWatchApp*)AfxGetApp();
	app->StopEatch();

	GetDlgItem(IDC_START_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_DIRECTORY_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_WATCH_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_COPYTO_EDIT)->EnableWindow(TRUE);
}


void CFileModWatchView::OnBnClickedStartButton()
{
	// TODO: Add your control notification handler code here
	CFileModWatchApp *const app = (CFileModWatchApp*)AfxGetApp();
	if (app->StartWatch(GetDocument())) {
		GetDlgItem(IDC_START_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_STOP_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_DIRECTORY_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_WATCH_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COPYTO_EDIT)->EnableWindow(FALSE);
	}
}


void CFileModWatchView::OnEnChangeDirectoryEdit()
{
	if (m_editing) {
		return;
	}
	CFileModWatchDoc *const doc = (CFileModWatchDoc *)GetDocument();
	CString str;
	GetDlgItem(IDC_DIRECTORY_EDIT)->GetWindowTextW(str);
	doc->m_watchDir = str;
	doc->SetModifiedFlag(TRUE);
}

void CFileModWatchView::OnEnChangeWatchEdit()
{
	if (m_editing) {
		return;
	}
	CFileModWatchDoc *const doc = (CFileModWatchDoc *)GetDocument();
	CString str;
	GetDlgItem(IDC_WATCH_EDIT)->GetWindowTextW(str);

	UINT nPrev = 0;
	UINT nLength = str.GetLength();
	if (nLength < 1) {
		return;
	}
	if (str.GetAt(nLength - 1) != _T(';')) {
		str += _T(';');
		nLength++;
	}

	doc->m_watchFiles.RemoveAll();
	LPTSTR lpStr = static_cast<LPTSTR>(str.GetBuffer());
	for (UINT i = 0; i < nLength; i++) {
		if (lpStr[i] == _T(';')) {
			CString word;
			UINT nWordLength = i - nPrev;

			if (nWordLength > 0) {
				word.SetString(&lpStr[nPrev], nWordLength);
				doc->m_watchFiles.Add(word);
				nPrev = i + 1;//
				i++;
			}
		}
	}
	doc->SetModifiedFlag(TRUE);
}

void CFileModWatchView::OnEnChangeCopytoEdit()
{
	if (m_editing) {
		return;
	}
	CFileModWatchDoc *const doc = (CFileModWatchDoc *)GetDocument();
	CString str;
	GetDlgItem(IDC_COPYTO_EDIT)->GetWindowTextW(str);
	doc->m_copyToDir = str;
	doc->SetModifiedFlag(TRUE);
}

void CFileModWatchView::OnUpdateFileNew(CCmdUI *pCmdUI)
{
	CFileModWatchApp *const app = (CFileModWatchApp*) AfxGetApp();
	pCmdUI->Enable(app->IsWatchRunning() == FALSE);
}

void CFileModWatchView::OnUpdateFileOpen(CCmdUI *pCmdUI)
{
	CFileModWatchApp *const app = (CFileModWatchApp*) AfxGetApp();
	pCmdUI->Enable(app->IsWatchRunning() == FALSE);
}

void CFileModWatchView::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	CFileModWatchApp *const app = (CFileModWatchApp*)AfxGetApp();
	pCmdUI->Enable(app->IsWatchRunning() == FALSE);
}

void CFileModWatchView::OnUpdateFileSaveAs(CCmdUI *pCmdUI)
{
	CFileModWatchApp *const app = (CFileModWatchApp*)AfxGetApp();
	pCmdUI->Enable(app->IsWatchRunning() == FALSE);
}

void CFileModWatchView::OnUpdateFileMRU(CCmdUI *pCmdUI)
{
	CFileModWatchApp *const app = (CFileModWatchApp*)AfxGetApp();
	pCmdUI->Enable(app->IsWatchRunning() == FALSE);
}
