// MainFrm.h : interface of the CMainFrame class
//
// Author: M.A. Nischalke
// Copyright 2001, MANSoft
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__D675ED27_9F23_4015_B1D2_7A8A8D731987__INCLUDED_)
#define AFX_MAINFRM_H__D675ED27_9F23_4015_B1D2_7A8A8D731987__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "AHN EditorView.h"
#include "XMLView.h"
#include "ComboBar.h"
#include <msxml2.h>

class CMainFrame :	public CFrameWindowImpl<CMainFrame>, 
					public CUpdateUI<CMainFrame>,
					public CMessageFilter, 
					public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)
	
	CMainFrame();
	~CMainFrame();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_FILE_SAVE, OnFileSave)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
//		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_EDIT_PASTE, OnEditPaste)
		COMMAND_ID_HANDLER(ID_EDIT_COPY, OnEditCopy)
		COMMAND_ID_HANDLER(ID_EDIT_CUT, OnEditCut)
		COMMAND_ID_HANDLER(ID_EDIT_UNDO, OnEditUndo)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
//	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void LoadKeywords(CListBox& Keywords);

	/********************************************
	// These functions are used to reflect 
	// accelerator key messages.
	********************************************/
	LRESULT OnEditPaste(WORD /* wNotifyCode*/, WORD /* wID*/, HWND /* hWndCtl*/, BOOL& /* bHandled*/)
	{
		return m_EditorView.Paste();
	}
	LRESULT OnEditCopy(WORD /* wNotifyCode*/, WORD /* wID*/, HWND /* hWndCtl*/, BOOL& /* bHandled*/)
	{
		return m_EditorView.Copy();
	}
	LRESULT OnEditCut(WORD /* wNotifyCode*/, WORD /* wID*/, HWND /* hWndCtl*/, BOOL& /* bHandled*/)
	{
		return m_EditorView.Cut();
	}
	LRESULT OnEditUndo(WORD /* wNotifyCode*/, WORD /* wID*/, HWND /* hWndCtl*/, BOOL& /* bHandled*/)
	{
		return m_EditorView.Undo();
	}
	
	CXMLView* GetXMLView() { return &m_XMLView; };
	
	BOOL InsertElement();

private:
	CSplitterWindow m_Splitter;

	CAHNEditorView m_EditorView;
	CXMLView m_XMLView;

	CCommandBarCtrl m_CmdBar;
	CComboBar	m_ComboBar;

	CComPtr<IXMLDOMDocument2> m_pConfigDoc;
	CComPtr<IXMLDOMDocument2> m_pXSLDoc;
	CComPtr<IXMLDOMDocument2> m_pNewDoc;

	HACCEL m_hAccel;

	UINT m_uVolume;
	UINT m_uIssue;

	BOOL CreateNewDocument(int iVolume, int iIssue);
	BOOL CreateStylesheet();
	void LoadSections();
	void LoadRegions();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__D675ED27_9F23_4015_B1D2_7A8A8D731987__INCLUDED_)
