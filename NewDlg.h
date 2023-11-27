// NewDlg.h: interface for the CNewDlg class.
//
// Author: M.A. Nischalke
// Copyright 2001, MANSoft
// 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWDLG_H__471EB673_6C9F_11D5_94B3_000102848731__INCLUDED_)
#define AFX_NEWDLG_H__471EB673_6C9F_11D5_94B3_000102848731__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNewDlg : public CDialogImpl<CNewDlg>,
				public CWinDataExchange<CNewDlg> 
{
public:
   enum { IDD = IDD_NEW_DLG };

public:
	CNewDlg() : m_uVolume(1), m_uIssue(1) {};
	virtual ~CNewDlg() {};

	BEGIN_MSG_MAP(CNewDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CAHNEditorView)
		DDX_UINT(IDC_VOLUME, m_uVolume)
		DDX_UINT(IDC_ISSUE, m_uIssue)
	END_DDX_MAP() 

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CenterWindow();

		DoDataExchange(FALSE);
		return 1;
	}

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		DoDataExchange(TRUE);
		if( m_uVolume > 0 && m_uIssue > 0 )
			EndDialog(IDOK);
		return 0;
	}

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(IDCANCEL);
		return 0;
	}

	UINT m_uVolume;
	UINT m_uIssue;
};


#endif // !defined(AFX_NEWDLG_H__471EB673_6C9F_11D5_94B3_000102848731__INCLUDED_)
