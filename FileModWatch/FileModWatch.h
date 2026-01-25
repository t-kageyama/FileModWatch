
// FileModWatch.h : main header file for the FileModWatch application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include "DirectoryWatch.h"
#include "TrayWnd.h"

// CFileModWatchApp:
// See FileModWatch.cpp for the implementation of this class
//

class CFileModWatchApp : public CWinApp
{
public:
	CFileModWatchApp();
	BOOL StartWatch(CDocument* doc);
	void StopEatch();
	BOOL IsWatchRunning();
	BOOL IsStartUp() { return m_startup; }

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void TrayWindowWillClose() { m_trayWnd = NULL; }

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_startup;
	CTrayWnd* m_trayWnd;
	CDirectoryWatch* m_dirWatch;
};

extern CFileModWatchApp theApp;
