#pragma once

#include "TrayWnd.h"

// CMyApplicationTrayWnd

class CMyApplicationTrayWnd : public CTrayWnd
{
private:
	HICON m_hIcon_main;
protected:
	void DoLButtonClick();
	void DoRButtonClick();
	void DoLButtonDoubleClick();

	DECLARE_DYNAMIC(CMyApplicationTrayWnd)

public:
	CMyApplicationTrayWnd();
	virtual ~CMyApplicationTrayWnd();

protected:
	static UINT s_wmTaskbarCreated;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAppExit();
	afx_msg void OnAppAbout();
	afx_msg void OnShowMainWindow();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnTaskbarCreated(WPARAM wParam, LPARAM lParam);
};


