// MyApplicationTrayWnd.cpp : 実装ファイル
//

#include "stdafx.h"
#include "FileModWatch.h"
#include "MyApplicationTrayWnd.h"

//#include "AboutDlg.h"

// CMyApplicationTrayWnd
UINT CMyApplicationTrayWnd::s_wmTaskbarCreated = 0;

IMPLEMENT_DYNAMIC(CMyApplicationTrayWnd, CTrayWnd)

CMyApplicationTrayWnd::CMyApplicationTrayWnd() :
	m_hIcon_main(NULL)
{
}

CMyApplicationTrayWnd::~CMyApplicationTrayWnd()
{
}

void CMyApplicationTrayWnd::DoLButtonClick()
{
	POINT pt;
	GetCursorPos( &pt );

	CMenu menu;
	menu.LoadMenu(IDM_TRAY_L);

	CMenu *const pPopup = menu.GetSubMenu(0);

	// SetForgroundWindowとPostMessageが必要な理由は、
	// Knowledge Base (Q135788)参照のこと
	SetForegroundWindow();
	pPopup->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
	PostMessage(WM_NULL);
}

void CMyApplicationTrayWnd::DoRButtonClick()
{
	DoLButtonClick();
}

void CMyApplicationTrayWnd::DoLButtonDoubleClick()
{
	OnShowMainWindow();
}

BEGIN_MESSAGE_MAP(CMyApplicationTrayWnd, CTrayWnd)
	ON_COMMAND(ID_APP_EXIT, &CMyApplicationTrayWnd::OnAppExit)
	ON_COMMAND(ID_APP_ABOUT, &CMyApplicationTrayWnd::OnAppAbout)
	ON_COMMAND(ID_SHOW, &CMyApplicationTrayWnd::OnShowMainWindow)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_REGISTERED_MESSAGE(CMyApplicationTrayWnd::s_wmTaskbarCreated, &CMyApplicationTrayWnd::OnTaskbarCreated)
END_MESSAGE_MAP()

// CMyApplicationTrayWnd メッセージ ハンドラ

void CMyApplicationTrayWnd::OnAppExit()
{
	((CFileModWatchApp *)AfxGetApp())->TrayWindowWillClose();
	PostMessage(WM_CLOSE);
	AfxGetMainWnd()->PostMessage(WM_CLOSE);	// close main window.
}

void CMyApplicationTrayWnd::OnAppAbout()
{
	AfxGetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_APP_ABOUT, 0));
}

void CMyApplicationTrayWnd::OnShowMainWindow()
{
	AfxGetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_SHOW, 0));
}

int CMyApplicationTrayWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTrayWnd::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	// TODO:  ここに特定な作成コードを追加してください。
	m_hIcon_main = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	NotifyIcon(NIM_ADD, m_hIcon_main, _T("File Modify Watch"));
	s_wmTaskbarCreated = ::RegisterWindowMessage(_T("TaskbarCreated"));

	return 0;
}

void CMyApplicationTrayWnd::OnDestroy()
{
	CTrayWnd::OnDestroy();

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	NotifyIcon(NIM_DELETE, NULL);	
}

LRESULT CMyApplicationTrayWnd::OnTaskbarCreated(WPARAM wParam, LPARAM lParam)
{
	//::DestroyIcon(m_hIcon_main);
	SetCurrentIcon(NULL);
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	NotifyIcon(NIM_ADD, hIcon, _T("File Modify Watch"));
	return 0L;
}