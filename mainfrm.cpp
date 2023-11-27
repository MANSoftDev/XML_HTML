// MainFrm.cpp : implementation of the CMainFrame class
//
// Author: M.A. Nischalke
// Copyright 2001, MANSoft
// 
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mainfrm.h"
#include "NewDlg.h"

CMainFrame::CMainFrame()
{

}

CMainFrame::~CMainFrame()
{

}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	if(m_hAccel != NULL && ::TranslateAccelerator(m_hWnd, m_hAccel, pMsg))
		return TRUE;

	return m_EditorView.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateToolBar();
	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// We need an instance of a DOMDocument for the configuration file
	if( FAILED(m_pConfigDoc.CoCreateInstance(__uuidof(DOMDocument2))) || !m_pConfigDoc )
	{
		MessageBox(_T("Failed to create Configuration document."));
		return -1;
	}

	// Create and load the stylesheet.
	if( !CreateStylesheet() )
	{
		MessageBox(_T("Failed to create Stylesheet document."));
		return -1;
	}
	
	// Load the configuration file
	CComVariant varFile(_T("config.xml"));
	short sSuccess;
	if( FAILED(m_pConfigDoc->load(varFile,&sSuccess)) )
	{
		MessageBox(_T("LoadXML failed."));
		return -1;
	}
	
	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	SetMenu(NULL);

	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);
	
	// Get the rect for the commandbar to use for creating the ComboBox command bar
	CRect rect;
	m_CmdBar.GetWindowRect(&rect);
	
	// Create the combobox command bar and load the initial data
	CRect rc(0,0,200,rect.Height()+7);
	HWND hComboBar = m_ComboBar.Create(m_hWnd, rc, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	if(hComboBar)
	{
		// Load the comboboxes
		LoadSections();
		LoadRegions();
		
		// Select the first item
		m_ComboBar.SetCurSel_Section(0);
		m_ComboBar.SetCurSel_Region(0);
		
		// Now add this commandbar to the rebar
		AddSimpleReBarBand(hComboBar, NULL, TRUE);
	}
	
	CreateSimpleStatusBar();

	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);

    m_hAccel = LoadAccelerators(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME));

	GetClientRect(&rect);
	m_hWndClient = m_Splitter.Create(m_hWnd, rect, NULL, WS_CHILD | WS_VISIBLE );
	
	// The initial HTML page is stored as a resource so wee need to extract it
	TCHAR szModule[_MAX_PATH];
	GetModuleFileName(_Module.GetModuleInstance(), szModule, _MAX_PATH);
	TCHAR szResID[11];
	itoa(IDR_SPLASH, szResID, 10);
	CString strURL;
	strURL.Format(_T("res://%s/%s"), szModule, szResID);

	// Create the HTML view with the initial navigation to the resource extracted above and set it
	// as the main page in the splitter frame.
	if( m_XMLView.Create(m_hWndClient, rcDefault, strURL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL, WS_EX_CLIENTEDGE) )
		m_XMLView.SetMainFrame(this);
	else
	{
		MessageBox(_T("Failed to create HTML view."));
		return -1;
	}

	// Create the editor view
	if( !m_EditorView.CreateEx(m_hWndClient, this) )
	{
		MessageBox(_T("Failed to create editor view."));
		return -1;
	}
	
	// Set the splitter panes and show only the right(XML) view initially
	m_Splitter.SetSplitterPanes(m_EditorView, m_XMLView);
	m_Splitter.SetSplitterPos(-1);
	m_Splitter.SetSinglePaneMode(SPLIT_PANE_RIGHT);
	
	// Set the scroll size for the editor view
	m_EditorView.GetWindowRect(&rc);
	m_EditorView.SetScrollSize(rc.Height(), rc.Width());
	
	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
	
	return 0;
}

LRESULT CMainFrame::OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	USES_CONVERSION;

	//MessageBox(_T("This feature is disabled."));
	
	// Ask for the folder
	CFolderDialog dlg(NULL, _T("Chose the folder you want to save document to:"));
	if( IDOK == dlg.DoModal() )
	{
		CString strTitle;
		strTitle.Format(_T("%s\\test%d_%d.xml"), dlg.GetFolderPath(), m_uVolume, m_uIssue);

		// Save needs a BSTR so create a temporary one initialized to the formated
		// string above and attempt to save the document
		CComVariant bstrSave(strTitle);
		if( FAILED(m_pNewDoc->save(bstrSave) ) )
		{
			MessageBox(_T("ERROR: Document not saved."));
			return 0;
		}

		// If the save succeeded then set the window title to indicate it
		SetWindowText(strTitle);
	}

	return 0;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	USES_CONVERSION;
	
	// Ask for the volume and issue number
	CNewDlg dlg;
	if( IDOK == dlg.DoModal() )
	{
		// Release the previous document if necessary
		if(m_pNewDoc)
			m_pNewDoc.Release();

		// Create new document
		if( !CreateNewDocument(dlg.m_uVolume, dlg.m_uIssue) )
		{
			MessageBox(_T("CreateNewDocument failed."));
			return 0;
		}
		
		// Save off the volume and issue
		m_uVolume = dlg.m_uVolume;
		m_uIssue = dlg.m_uIssue;

		// Set the window title
		CString strTitle;
		strTitle.Format(_T("ahn%d_%d.xml *"), m_uVolume, m_uIssue);
		SetWindowText(strTitle);

		// Set the focus on the first field so we can begin editing
		m_EditorView.SetFocus(IDC_HEADLINE);

		// Reset the splitter so the editor is full screen
		m_Splitter.SetSinglePaneMode(SPLIT_PANE_LEFT);
	}
	return 0;
}

LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

//LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//{
//	CAboutDlg dlg;
//	dlg.DoModal();
//	return 0;
//}

/***************************************************
// Get the Keyword elements in the config.xml file(loaded
// earlier) and insert each item in the listbox passed in.
***************************************************/
void CMainFrame::LoadKeywords(CListBox& Keywords)
{
	USES_CONVERSION;

	CComPtr<IXMLDOMNodeList> pNodeList = NULL;

	// Get all hte Keyword elements
	if( SUCCEEDED(m_pConfigDoc->getElementsByTagName(A2OLE("Keyword"), &pNodeList) ) )
	{
		CComPtr<IXMLDOMNode> pNode;
		
		// Iterate through the collection
		while( SUCCEEDED(pNodeList->nextNode(&pNode)) && pNode )
		{
			CComBSTR bstrText;
			// Get each string and add it to the listbox
			pNode->get_text(&bstrText);
			Keywords.AddString(OLE2A(bstrText));
			
			// Release the node for the next iteration
			pNode.Release();
		}
	}
}

/***************************************************
// Get the Section elements in the config.xml file(loaded
// earlier) and insert each item in the combobar.
***************************************************/
void CMainFrame::LoadSections()
{
	USES_CONVERSION;

	CComPtr<IXMLDOMNodeList> pNodeList = NULL;

	// Get all the Keyword elements
	if( SUCCEEDED(m_pConfigDoc->getElementsByTagName(A2OLE("Section"), &pNodeList) ) )
	{
		CComPtr<IXMLDOMNode> pNode;
		
		// Iterate through the collection
		while( SUCCEEDED(pNodeList->nextNode(&pNode)) && pNode )
		{
			CComBSTR bstrText;
			// Get each string and add it to the listbox
			pNode->get_text(&bstrText);
			m_ComboBar.AddSection(OLE2A(bstrText));
			
			// Release the node for the next iteration
			pNode.Release();
		}
	}
}

/***************************************************
// Get the Region elements in the config.xml file(loaded
// earlier) and insert each item in the combobar.
***************************************************/
void CMainFrame::LoadRegions()
{
	USES_CONVERSION;

	CComPtr<IXMLDOMNodeList> pNodeList = NULL;

	// Get all hte Keyword elements
	if( SUCCEEDED(m_pConfigDoc->getElementsByTagName(A2OLE("Region"), &pNodeList) ) )
	{
		CComPtr<IXMLDOMNode> pNode;
		
		// Iterate through the collection
		while( SUCCEEDED(pNodeList->nextNode(&pNode)) && pNode )
		{
			CComBSTR bstrText;
			// Get each string and add it to the listbox
			pNode->get_text(&bstrText);
			m_ComboBar.AddRegion(OLE2A(bstrText));
			
			// Release the node for the next iteration
			pNode.Release();
		}
	}
}

/***************************************************
// Create an XML document based on the provided
// volume and issue numbers.
***************************************************/
BOOL CMainFrame::CreateNewDocument(int iVolume, int iIssue)
{
	USES_CONVERSION;

	// Create a new DOMDocument instance
	if( FAILED(m_pNewDoc.CoCreateInstance(__uuidof(DOMDocument2))) || !m_pNewDoc )
		return FALSE;

	CComPtr<IXMLDOMElement>	 pRootElement;
	CComPtr<IXMLDOMElement>	 pSectionElement;
	CComPtr<IXMLDOMElement>	 pRegionElement;

	// Create the necessary xml processing tag
	CComPtr<IXMLDOMProcessingInstruction> pPI;
	if( SUCCEEDED(m_pNewDoc->createProcessingInstruction(A2OLE("xml"), A2OLE("version='1.0'"), &pPI) ) ) 
	{
		if( FAILED(m_pNewDoc->appendChild(pPI, NULL) ) )
			return FALSE;
	}

	// Create the root element and add the volume and issue attributes
	if( SUCCEEDED(m_pNewDoc->createElement(A2OLE("AutoHeadlineNews"), &pRootElement) ) )
	{
		if( FAILED(pRootElement->setAttribute(A2OLE("volume"), CComVariant(iVolume) ) ) )
			return FALSE;

		if( FAILED(pRootElement->setAttribute(A2OLE("issue"), CComVariant(iIssue) ) ) )
			return FALSE;

		if( FAILED(m_pNewDoc->appendChild(pRootElement, NULL) ) )
			return FALSE;
	}
	
	// Iterate through the contents of the combo boxes and create
	// elements for them in the XML document
	for( int x = 0; x < m_ComboBar.GetSectionCount(); x++ )
	{
		if( FAILED(m_pNewDoc->createElement(A2OLE("Section"), &pSectionElement) ) )
			return FALSE;
		
		CComVariant var(m_ComboBar.GetSection(x));
		if( FAILED(pSectionElement->setAttribute(A2OLE("name"), var) ) )
			return FALSE;

		for( int x = 0; x < m_ComboBar.GetRegionCount(); x++ )
		{
			if( FAILED(m_pNewDoc->createElement(A2OLE("Region"), &pRegionElement) ) )
				return FALSE;
			
			CComVariant var(m_ComboBar.GetRegion(x));
			if( FAILED(pRegionElement->setAttribute(A2OLE("name"), var) ) )
				return FALSE;

			// Append region to section
			if( FAILED(pSectionElement->appendChild(pRegionElement, NULL) ) )
				return FALSE;
			
			pRegionElement.Release();
		}

		// Append section to root
		if( FAILED(pRootElement->appendChild(pSectionElement, NULL) ) )
			return FALSE;
	
		pSectionElement.Release();
	}

	return TRUE;
}

/***************************************************
// Insert a new element into the document based on
// the current selections in the comboboxes
***************************************************/
BOOL CMainFrame::InsertElement()
{
	USES_CONVERSION;

	CComPtr<IXMLDOMNode> pSection;
	CComPtr<IXMLDOMNode> pRegion;

	// Format a XPATH search string to find the section this 
	// element should be inserted into and try to select the node
	CString strSearch;
	strSearch.Format(_T("/*/*[@name=\"%s\"]"), m_ComboBar.GetSection());
	if( FAILED(m_pNewDoc->selectSingleNode(A2OLE(strSearch), &pSection) ) )
		return FALSE;

	// Format a XPATH search string to find the region this 
	// element should be inserted into and try to select the node
	strSearch.Format(_T("/*/*/*[@name=\"%s\"]"), m_ComboBar.GetRegion());
	if( FAILED(pSection->selectSingleNode(A2OLE(strSearch), &pRegion) ) )
		return FALSE;

	CComPtr<IXMLDOMElement>	 pArticle;
	CComPtr<IXMLDOMElement>	 pTitle;
	CComPtr<IXMLDOMElement>	 pAbstract;
	
	// Create the article element
	if( SUCCEEDED(m_pNewDoc->createElement(A2OLE("Article"), &pArticle) ) )
	{
		// Get the keywords selected in the listbox and create the keyword
		// attribute attached to the article element
		CComVariant var(m_EditorView.GetKeywords());
		if( FAILED(pArticle->setAttribute(A2OLE("keywords"), var) ) )
			return FALSE;

		// Get the contents of source editbox and create the source
		// attribute attached to the article element
		var = m_EditorView.GetSource();
		if( FAILED(pArticle->setAttribute(A2OLE("source"), var) ) )
			return FALSE;

		// Get the date and create the date
		// attribute attached to the article element
		var = m_EditorView.GetDate();
		if( FAILED(pArticle->setAttribute(A2OLE("date"), var) ) )
			return FALSE;
	}
	else
		return FALSE;
	
	// Create and add Title element
	if( SUCCEEDED(m_pNewDoc->createElement(A2OLE("Title"), &pTitle) ) )
	{
		// Put the text from the headline edit box
		if( FAILED(pTitle->put_text(A2OLE(m_EditorView.GetHeadline())) ) )
			return FALSE;
		
		// Append this element to the article
		if( FAILED(pArticle->appendChild(pTitle, NULL) ) )
			return FALSE;
	}
	else
		return FALSE;
	

	// Create and add Abstract element
	if( SUCCEEDED(m_pNewDoc->createElement(A2OLE("Abstract"), &pAbstract)) )
	{
		// Put the text from the abstract edit box
		if( FAILED(pAbstract->put_text(A2OLE(m_EditorView.GetAbstract())) ) )
			return FALSE;

		// Append this element to the article
		if( FAILED(pArticle->appendChild(pAbstract, NULL) ) )
			return FALSE;

		// Append article to region
		if( FAILED(pRegion->appendChild(pArticle, NULL) ) )
			return FALSE;
	}
	else
		return FALSE;

	// Transform the node and insert it into the HTML document
	CComBSTR bstrOutput;
	if( SUCCEEDED(pArticle->transformNode(m_pXSLDoc, &bstrOutput) ) )
	{
		// Now insert theis element into the HTML document
		m_XMLView.InsertElement(OLE2A(bstrOutput));
	
		// Show in split screen now, if not already in that mode
		if( SPLIT_PANE_NONE != m_Splitter.GetSinglePaneMode() )
			m_Splitter.SetSinglePaneMode(SPLIT_PANE_NONE);
	}
	else
		return FALSE;

	return TRUE;
}

/***************************************************
// Create DOMDocument for the stylesheet and load
// the stylesheet string resource into it.
***************************************************/
BOOL CMainFrame::CreateStylesheet()
{
	USES_CONVERSION;
	
	// Create an DOMDocument for the stylesheet
	if( FAILED(m_pXSLDoc.CoCreateInstance(__uuidof(DOMDocument2))) || !m_pXSLDoc )
		return FALSE;
	
	// To reduce the supoort files the XSL is stored as a resource
	// and loaded into the previously created DOMDocument.
	CString strXSL;
	strXSL.LoadString(IDS_XSL);
	
	short bSuccess;
	if( FAILED(m_pXSLDoc->loadXML(A2OLE(strXSL), &bSuccess) ) )
		return FALSE;

	return TRUE;
}
