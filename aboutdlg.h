// aboutdlg.h : interface of the CAboutDlg class
//
// Author: M.A. Nischalke
// Copyright 2001, MANSoft
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABOUTDLG_H__5E832C69_CD4C_46A4_AD29_49A550C75065__INCLUDED_)
#define AFX_ABOUTDLG_H__5E832C69_CD4C_46A4_AD29_49A550C75065__INCLUDED_

class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
	enum { IDD = IDD_ABOUTBOX };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());
		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}
};

#endif // !defined(AFX_ABOUTDLG_H__5E832C69_CD4C_46A4_AD29_49A550C75065__INCLUDED_)
