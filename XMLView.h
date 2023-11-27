// XMLView.h: interface for the CXMLView class.
//
// Author: M.A. Nischalke
// Copyright 2001, MANSoft
// 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLVIEW_H__F85E6390_2FC4_4672_8674_DF6F9C6B362D__INCLUDED_)
#define AFX_XMLVIEW_H__F85E6390_2FC4_4672_8674_DF6F9C6B362D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame;

class CXMLView : public CWindowImpl<CXMLView, CAxWindow>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CAxWindow::GetWndClassName())

	CXMLView();
	~CXMLView();

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		if((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
		   (pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST))
			return FALSE;

		// give HTML page a chance to translate this message
		return (BOOL)SendMessage(WM_FORWARDMSG, 0, (LPARAM)pMsg);
	}

	BEGIN_MSG_MAP(CHTMLView)
	END_MSG_MAP()

	void InsertElement(CString strElement);
	inline void SetMainFrame(CMainFrame* pWnd) { m_pMainFrame = pWnd; };
	inline CMainFrame* GetMainFrame() { return m_pMainFrame; };

private:
	CMainFrame*	m_pMainFrame;
	CComPtr<IHTMLElement> m_pBody;
	CComPtr<IHTMLDocument2> m_pDoc;
	
	BOOL GetBody();
	void ToggleElement(IHTMLElementCollection* pColl);

};

#endif // !defined(AFX_XMLVIEW_H__F85E6390_2FC4_4672_8674_DF6F9C6B362D__INCLUDED_)
