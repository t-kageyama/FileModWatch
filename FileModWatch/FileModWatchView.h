
// FileModWatchView.h : interface of the CFileModWatchView class
//

#pragma once

#include "resource.h"


class CFileModWatchView : public CFormView
{
protected: // create from serialization only
	CFileModWatchView();
	DECLARE_DYNCREATE(CFileModWatchView)

public:
	enum{ IDD = IDD_FILEMODWATCH_FORM };

// Attributes
public:
	CFileModWatchDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CFileModWatchView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

protected:

private:
	BOOL m_editing;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBrowseButton();
	afx_msg void OnBnClickedBrowsetoButton();
	afx_msg void OnBnClickedStopButton();
	afx_msg void OnBnClickedStartButton();
	afx_msg void OnEnChangeDirectoryEdit();
	afx_msg void OnEnChangeWatchEdit();
	afx_msg void OnEnChangeCopytoEdit();
	afx_msg void OnUpdateFileNew(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileMRU(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in FileModWatchView.cpp
inline CFileModWatchDoc* CFileModWatchView::GetDocument() const
   { return reinterpret_cast<CFileModWatchDoc*>(m_pDocument); }
#endif

