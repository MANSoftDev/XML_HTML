// ComboBar.h: interface for the CComboBar class.
//
// Author: M.A. Nischalke
// Copyright 2001, MANSoft
// 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMBOBAR_H__38D82BB2_6AF3_11D5_94B2_000102848731__INCLUDED_)
#define AFX_COMBOBAR_H__38D82BB2_6AF3_11D5_94B2_000102848731__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CComboBar : public CCommandBarCtrl
{
private:
	CContainedWindowT<CComboBox> m_cbSection;
	CContainedWindowT<CComboBox> m_cbRegion;

public:

	DECLARE_WND_SUPERCLASS(NULL, CCommandBarCtrl::GetWndClassName())

	CComboBar() : m_cbSection(NULL, this, 1 ), m_cbRegion(NULL, this, 1 )
	{
	};
	virtual ~CComboBar() {};
	
	BEGIN_MSG_MAP(CComboBar)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
	ALT_MSG_MAP(1)
//		MESSAGE_HANDLER(CBN_SELCHANGE, OnSelectChange)
//		MESSAGE_HANDLER(CBN_DROPDOWN, OnDropDown)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /* uMsg*/, WPARAM /* wParam*/, LPARAM /* lParam*/, BOOL& /* bHandled*/)
	{
		int iLabelWidth = 70;
		int iComboWidth = 150;
		
		// TODO: Get font metric and set width
		CRect rc(0,0,iLabelWidth,300);
		if( !m_Label1.Create(m_hWnd, rc, NULL, WS_CHILD | WS_VISIBLE) ) 
			return -1;
		m_Label1.SetWindowText(_T("Section:"));		
		
		rc.left += iLabelWidth;
		rc.right += iComboWidth;

		if( !m_cbSection.Create(this, 1, m_hWnd, &rc, NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST ) )
			return -1;

		rc.left += iComboWidth + 10;
		rc.right += iLabelWidth;

		if( !m_Label2.Create(m_hWnd, rc, NULL, WS_CHILD | WS_VISIBLE) ) 
			return -1;
		m_Label2.SetWindowText(_T("Region:"));		

		rc.left += iLabelWidth;
		rc.right += iComboWidth;

		if( !m_cbRegion.Create(this, 1, m_hWnd, &rc, NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST ) )
			return -1;

		return 0;
	}

//	LRESULT OnSelectChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//	{
//		ATLTRACE(_T("OnSelectChange\n"));
//		bHandled = FALSE;
//		return 0;
//	}
//
//	LRESULT OnDropDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//	{
//		ATLTRACE(_T("OnDropDown\n"));
//		bHandled = FALSE;
//		return 0;
//	}

	BOOL AddSection(CString strAdd)
	{
		return m_cbSection.AddString(strAdd);
	}

	BOOL AddRegion(CString strAdd)
	{
		return m_cbRegion.AddString(strAdd);
	}

	BOOL SetCurSel_Section(int iID)
	{
		return m_cbSection.SetCurSel(iID);
	}

	BOOL SetCurSel_Region(int iID)
	{
		return m_cbRegion.SetCurSel(iID);
	}

	CString GetSection() const
	{
		TCHAR* strText = new TCHAR[25];
		if( CB_ERR != m_cbSection.GetLBText(m_cbSection.GetCurSel(), strText) )
			return strText;
		else
			return _T("");
	}

	CString GetRegion() const
	{
		TCHAR* strText = new TCHAR[25];
		if( CB_ERR != m_cbRegion.GetLBText(m_cbRegion.GetCurSel(), strText) )
			return strText;
		else
			return _T("");
	}

	CString GetSection(int iIndex) const
	{
		TCHAR* strText = new TCHAR[25];
		if( CB_ERR != m_cbSection.GetLBText(iIndex, strText) )
			return strText;
		else
			return _T("");
	}

	CString GetRegion(int iIndex) const
	{
		TCHAR* strText = new TCHAR[25];
		if( CB_ERR != m_cbRegion.GetLBText(iIndex, strText) )
			return strText;
		else
			return _T("");
	}
	
	int GetSectionCount() const { return m_cbSection.GetCount(); };
	int GetRegionCount() const { return m_cbRegion.GetCount(); };

private:
	CStatic m_Label1;
	CStatic m_Label2;
};

#endif // !defined(AFX_COMBOBAR_H__38D82BB2_6AF3_11D5_94B2_000102848731__INCLUDED_)
