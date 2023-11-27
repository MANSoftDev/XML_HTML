// AHN EditorView.h : interface of the CAHNEditorView class
//
// Author: M.A. Nischalke
// Copyright 2001, MANSoft
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AHNEDITORVIEW_H__B562B80D_0A19_46ED_8D3B_F99F7AFB2B7A__INCLUDED_)
#define AFX_AHNEDITORVIEW_H__B562B80D_0A19_46ED_8D3B_F99F7AFB2B7A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "resource.h"

class CMainFrame;

class CAHNEditorView :	public CDialogImpl<CAHNEditorView>,
						public CScrollImpl<CAHNEditorView>,
						public CWinDataExchange<CAHNEditorView> 
{
public:
	CAHNEditorView();
	~CAHNEditorView();

	enum { IDD = IDD_AHNEDITOR_FORM };

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CAHNEditorView)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_VSCROLL, OnVScroll)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		COMMAND_HANDLER(IDC_ENTER, BN_CLICKED, OnEnter)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CAHNEditorView)
		DDX_TEXT(IDC_HEADLINE, m_strHeadline)
		DDX_TEXT(IDC_ABSTRACT, m_strAbstract)
		DDX_TEXT(IDC_SOURCE, m_strSource)
	END_DDX_MAP() 

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnVScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnHScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEnter(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	HWND CreateEx(HWND hWnd, CMainFrame* pFrame);
	BOOL Paste();
	BOOL Copy();
	BOOL Cut();
	BOOL Undo();

	inline CString GetHeadline() const { return m_strHeadline; };
	inline CString GetAbstract() const { return m_strAbstract; };
	inline CString GetSource() const { return m_strSource; };
	CString GetKeywords();
	CString GetDate();
	
	void SetFocus(UINT uID) { ::SetFocus(GetDlgItem(uID)); };

private:
	CString m_strHeadline;
	CString m_strAbstract;
	CString m_strSource;
	
	CDateTimePickerCtrl m_Date;
	CListBox m_Keywords;
	CMainFrame*	m_pMainFrame;

private:
	inline CMainFrame* GetMainFrame() const { return m_pMainFrame; };
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AHNEDITORVIEW_H__B562B80D_0A19_46ED_8D3B_F99F7AFB2B7A__INCLUDED_)
