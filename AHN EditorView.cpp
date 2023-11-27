// AHN EditorView.cpp : implementation of the CAHNEditorView class
//
// Author: M.A. Nischalke
// Copyright 2001, MANSoft
// 
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AHN EditorView.h"
#include "mainfrm.h"
#include <list>

CAHNEditorView::CAHNEditorView() : m_strHeadline(_T("")),
								 m_strAbstract(_T("")),
								 m_strSource(_T("")),
								 m_pMainFrame(NULL)
{

}

CAHNEditorView::~CAHNEditorView()
{
	if(m_pMainFrame)
		m_pMainFrame = NULL;
}

BOOL CAHNEditorView::PreTranslateMessage(MSG* pMsg)
{
	return IsDialogMessage(pMsg);
}

///////////////////////////
// Message handlers

LRESULT CAHNEditorView::OnInitDialog(UINT /*uMsg*/, WPARAM /* wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	DoDataExchange(FALSE);

	m_Keywords.Attach(GetDlgItem(IDC_KEYWORDS));
	m_Date.Attach(GetDlgItem(IDC_DATE));

	// Load up the Keywords list box
	GetMainFrame()->LoadKeywords(m_Keywords);

	return 0;
}

LRESULT CAHNEditorView::OnVScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	ATLASSERT(::IsWindow(m_hWnd));
	DoScroll(SB_VERT, (int)(short)LOWORD(wParam), (int&)m_ptOffset.y, m_sizeAll.cy, m_sizePage.cy, m_sizeLine.cy);
	return 0;
}

LRESULT CAHNEditorView::OnHScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	ATLASSERT(::IsWindow(m_hWnd));
	DoScroll(SB_HORZ, (int)(short)LOWORD(wParam), (int&)m_ptOffset.x, m_sizeAll.cx, m_sizePage.cx, m_sizeLine.cx);
	return 0;
}

LRESULT CAHNEditorView::OnEnter(WORD /* wNotifyCode*/, WORD /* wID*/, HWND /* hWndCtl*/, BOOL& /* bHandled*/)
{
	DoDataExchange(TRUE);
	
	// Verify that at least one keyword has been selected.
	if( m_Keywords.GetSelCount() == 0 )
	{
		MessageBox(_T("You must select at least one keyword"), _T("AHN Editor") );
		::SetFocus(GetDlgItem(IDC_KEYWORDS));
		return 1;
	}
	
	// Insert the data into the document
	GetMainFrame()->InsertElement();

	// Clear fields
	m_Keywords.SelItemRange(FALSE, 0, m_Keywords.GetCount());
	m_strHeadline.Empty();
	m_strAbstract.Empty();
	m_strSource.Empty();

	DoDataExchange(FALSE);
	
	// Set the focus on the headline edit box
	// to continue with the next element
	::SetFocus(GetDlgItem(IDC_HEADLINE));

	return 1;
}

HWND CAHNEditorView::CreateEx(HWND hWnd, CMainFrame* pFrame)
{
	m_pMainFrame = pFrame;
	
	return Create(hWnd);
}

/********************************************
// These functions are used by the mainframe
// to reflect accelerator keys.
********************************************/
BOOL CAHNEditorView::Paste()
{
	HWND hWnd = GetFocus();
	ATLASSERT(hWnd);
	return SendMessage(hWnd, WM_PASTE, 0, 0L);
}

BOOL CAHNEditorView::Copy()
{
	HWND hWnd = GetFocus();
	ATLASSERT(hWnd);
	return SendMessage(hWnd, WM_COPY, 0, 0L);
}

BOOL CAHNEditorView::Cut()
{
	HWND hWnd = GetFocus();
	ATLASSERT(hWnd);
	return SendMessage(hWnd, WM_CUT, 0, 0L);
}

BOOL CAHNEditorView::Undo()
{
	HWND hWnd = GetFocus();
	ATLASSERT(hWnd);
	return SendMessage(hWnd, WM_UNDO, 0, 0L);
}

/*******************************************
// Get all of the selected keywords and format
// them into a string to be returned
*******************************************/
CString CAHNEditorView::GetKeywords()
{
	CString strKeywords;
	
	// Get the selected count and iterate through the list
	// appending each one to the string to be returned
	int iCount(m_Keywords.GetSelCount());
	int* Selected = new int[iCount];	
	if( LB_ERR != m_Keywords.GetSelItems(iCount, Selected) )
	{
		for( int x = 0; x < iCount; x++ )	
		{
			TCHAR* szTemp = new TCHAR[25];
			m_Keywords.GetText(Selected[x], szTemp);
			strKeywords += szTemp;
			strKeywords += _T(";");
			delete szTemp;
		}
	}
	delete[] Selected;
	return strKeywords;
}

/*****************************************
// Get the date and return it in a string format
*****************************************/
CString CAHNEditorView::GetDate()
{
	SYSTEMTIME time;

	m_Date.GetSystemTime(&time);
	
	CString strDate;
	strDate.Format(_T("%d/%d/%d"), time.wMonth, time.wDay, time.wYear);

	return strDate;
}