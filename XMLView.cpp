// XMLView.cpp: implementation of the CXMLView class.
//
// Author: M.A. Nischalke
// Copyright 2001, MANSoft
// 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLView.h"
#include "mainfrm.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLView::CXMLView() : m_pBody(NULL)
{

}

CXMLView::~CXMLView()
{

}

/****************************************************
// Insert the given text into the HTML Document by getting
// the collection of elements and inserting it at the end
****************************************************/
void CXMLView::InsertElement(CString strElement)
{
	USES_CONVERSION;
	
	// If the body has not been stored yet then attempt to find it
	if( !m_pBody )
	{
		if( !GetBody() )
		{
			MessageBox(_T("Body not found.\n\nPlease close application"), _T("Critical Error"), MB_ICONERROR);
			return;
		}
	}
	
	// Get the elements on this page
	CComPtr<IDispatch> pDisp;
	if( SUCCEEDED(m_pBody->get_all(&pDisp)) && pDisp)
	{
		// Get the ElementCollection
		CComQIPtr<IHTMLElementCollection> pColl(pDisp);
		if(pColl)
		{
			// Make sure there are items in the collection
			long lCount;
			if( SUCCEEDED(pColl->get_length(&lCount)) && lCount > 0 )
			{
				// Show/Hide the element
				ToggleElement(pColl);

				// Release the dispatch to be reused and get the 
				// the last item in the collection
				pDisp.Release();
				CComVariant varName, varIndex(lCount);
				pColl->item(varName, varIndex, &pDisp);

				// Get the element returned above and insert the text
				// before the end of it
				CComQIPtr<IHTMLElement> pElement(pDisp);
				if(pElement)
					pElement->insertAdjacentHTML(A2OLE("beforeEnd"), A2OLE(strElement));
			}
		}
	}
}

/************************************************
// Get the body element of the the HTML document
// currently displayed in the HTML control
************************************************/
BOOL CXMLView::GetBody()
{
	// Get the browser interface from the embedded HTML control
	CComPtr<IWebBrowser2> spBrowser;
	if( SUCCEEDED(QueryControl(__uuidof(spBrowser), reinterpret_cast<void**>(&spBrowser))) )
	{
		// Get the document element
		CComPtr<IDispatch> pDisp;
		if( SUCCEEDED(spBrowser->get_Document(&pDisp)) && pDisp )
		{
			// Get the body element
			if( SUCCEEDED(pDisp.QueryInterface(&m_pDoc)) && m_pDoc )
			{
				if( SUCCEEDED(m_pDoc->get_body(&m_pBody)) && m_pBody )
					return TRUE;		
			}
		}
	}
	return FALSE;
}

void CXMLView::ToggleElement(IHTMLElementCollection* pColl)
{
	pColl->AddRef();
	
	CComPtr<IDispatch> pDisp;
	if( SUCCEEDED(pColl->tags(CComVariant(_T("span")), &pDisp)) && pDisp)
	{
		CComQIPtr<IHTMLElementCollection> pTagColl(pDisp);
		if(pTagColl)
		{
			long lCount;
			if( SUCCEEDED(pColl->get_length(&lCount)) && lCount > 0 )
			{
				for( long x = 0; x < lCount; x++ )
				{
					pDisp.Release(); // Reuse IDispatch

					CComVariant varName, varIndex(x);
					if( FAILED(pColl->item(varName, varIndex, &pDisp) ) )
						return;

					CComQIPtr<IHTMLElement> pElement(pDisp);
					if(pElement)
					{
						CComBSTR bstrID;
						pElement->get_innerHTML(&bstrID);
						if( SUCCEEDED(pElement->get_id(&bstrID) ) )
						{
							ATLTRACE("ID %d: %s\n", x, bstrID);
							if( bstrID == _T("img") )
							{
								// Simulate a click and let the embedded
								// JavaScript handle it
								pElement->click();
								return;
							}
						}
					}
					pElement.Release();
				}
			}
		}		
	}

	pColl->Release();
}