// This is a part of the OleDBPro module.
// Copyright (C) 2000-2001 UDAParts 
// All rights reserved.
//
// This source code is only intended as a supplement to the
// OleDBPro module Classes Reference and related
// electronic documentation provided with the Module.
// See these sources for detailed information regarding this
// UDAParts product.

// Please don't disclose any source code of the software to any person or entity,
//
// Please don't decompile, disassemble, or reverse engineer any object code of 
// any portion of the software.
//  
// Copyright (c) 2001 by Yuancai (Charlie) Ye
// http://www.geocities.com/OleDBPro/index.htm
// yekerui@yahoo.com

#include "OLEDBBas.h"
#include "Odbassert.h"

COView::COView()
{
	m_pIUnknown=NULL;
	m_IID=__uuidof(IViewChapter);
}

COView::~COView()
{
	if(IsOpen())
		Close();
}

void	COView::Close()
{
	if(m_pIUnknown)
	{
		m_pIUnknown->Release();
		m_pIUnknown=NULL;
	}
}

void COView::SetBaseIID(const GUID& guidBase)
{
	OleDBPro_ASSERT(!IsOpen(), "\nCan't set a guidBase after opening a view object!"); 
	m_IID=guidBase;
}

bool	COView::GetSession(COSession *pOSession)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that the view object is already opened!");
	OleDBPro_ASSERT(pOSession, "\npOSession can't be NULL!");
	CComPtr<IAccessor>		pIAccessor;
	CComPtr<IViewChapter>	pIViewChapter;
	CRBase						RBase;
	if(pOSession->IsOpen())
		pOSession->Close();
	pOSession->m_hr=m_pIUnknown->QueryInterface(__uuidof(IRowsetChange), (void**)&pIViewChapter);
	if(FAILED(pOSession->m_hr))
	{
		if(pOSession->m_pGetErrorInfo) 
			pOSession->m_pGetErrorInfo(m_pIUnknown, m_IID, &(pOSession->m_strError));
		return false;
	}
	pOSession->m_hr=pIViewChapter->GetSpecification(__uuidof(IAccessor), (IUnknown**)&pIAccessor);
	if(FAILED(pOSession->m_hr))
	{
		if(pOSession->m_pGetErrorInfo) 
			pOSession->m_pGetErrorInfo(pIViewChapter, __uuidof(IRowsetChange), &(pOSession->m_strError));
		return false;
	}
	if(!RBase.OpenRBase(pIAccessor.p))
	{
		return false;
	}
	if(!RBase.GetSession(pOSession))
	{
		return false;
	}
	return true;
}

bool	COView::OpenWithCmnd(IOpenRowset *pIOpenRowset, LPCOLESTR strSelect, DBPROPSET *pDBPropSet, ULONG nPropSets, REFGUID guidDialect)
{
	if(IsOpen())
		Close();
	OleDBPro_ASSERT(pIOpenRowset, "\nMake sure that pIOpenRowset is wired with a session!");
	CComPtr<IDBCreateCommand>		pIDBCreateCommand;
	CComPtr<ICommandText>			pICommandText;
	CComPtr<ICommandProperties>	pICommandProperties;
	m_hr=pIOpenRowset->QueryInterface(__uuidof(IDBCreateCommand), (void**)&pIDBCreateCommand);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIOpenRowset, __uuidof(IOpenRowset), &m_strError);
		return false;
	}
	m_hr =pIDBCreateCommand->CreateCommand(NULL, __uuidof(ICommandText), (IUnknown **)&pICommandText);
	if(FAILED(m_hr))
	{	
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIDBCreateCommand, __uuidof(IDBCreateCommand), &m_strError);
		return false;
	}
	m_hr = pICommandText->SetCommandText(guidDialect, strSelect);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pICommandText, __uuidof(ICommandText), &m_strError);
		return false;
	}
	m_hr = pICommandText->QueryInterface(__uuidof(ICommandProperties), (void**)&pICommandProperties);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pICommandText, __uuidof(ICommandText), &m_strError);
		return false;
	}
	if(pDBPropSet)
		m_hr=pICommandProperties->SetProperties(nPropSets, pDBPropSet);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pICommandProperties, __uuidof(ICommandProperties), &m_strError);
		return false;
	}
	m_hr=pICommandText->Execute(NULL, GetBaseIID(), NULL, &m_nRowsAffected, GetBaseInterfacePPtr());
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pICommandText, __uuidof(ICommandText), &m_strError);
		return false;
	}
	return PostProcess();
}

bool	COView::OpenView(CRBase *pRBase)
{
	OleDBPro_ASSERT(pRBase, "\npRBase can't be NULL!");
	OleDBPro_ASSERT(pRBase->IsOpen(), "\nMake sure that this view object is already opened!");
	ATLASSERT(*(pRBase->GetBaseInterfacePPtr()));
	if(IsOpen())
		Close();
	CComPtr<IRowsetView>		pIRowsetView;
	m_hr=(*(pRBase->GetBaseInterfacePPtr()))->QueryInterface(__uuidof(IRowsetView), (void**)&pIRowsetView);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(*(pRBase->GetBaseInterfacePPtr()), __uuidof(IAccessor), &m_strError);
		return false;
	}
	m_hr=pIRowsetView->CreateView(NULL, m_IID, &m_pIUnknown);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIRowsetView, __uuidof(IRowsetView), &m_strError);
		return false;
	}
	return PostProcess();
}

bool	COView::SetSortOrder(ULONG nCols, const ULONG pnCol[], const DBSORT pnSort[])
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this view object is already opened!");
	CComPtr<IViewSort>		pIViewSort;
	m_hr=m_pIUnknown->QueryInterface(__uuidof(IViewSort), (void**)&pIViewSort);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIUnknown, m_IID, &m_strError);
		return false;
	}
	m_hr=pIViewSort->SetSortOrder(nCols, pnCol, pnSort);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIViewSort, __uuidof(IViewSort), &m_strError);
		return false;
	}
	return true;
}

bool	COView::OpenRowset(CRBase *pRBase)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this view object is already opened!");
	OleDBPro_ASSERT(pRBase, "\npRBase can't be NULL!");
	CComPtr<IAccessor>		pIAccessor;
	CComPtr<IViewRowset>		pIViewRowset;
	if(pRBase->IsOpen())
		pRBase->Close();
	pRBase->m_hr=m_pIUnknown->QueryInterface(__uuidof(IViewRowset), (void**)&pIViewRowset);
	if(FAILED(pRBase->m_hr))
	{
		if(pRBase->m_pGetErrorInfo) 
			pRBase->m_pGetErrorInfo(m_pIUnknown, m_IID, &(pRBase->m_strError));
		return false;
	}
	pRBase->m_hr=pIViewRowset->OpenViewRowset(NULL, __uuidof(IAccessor), (IUnknown**)&(pIAccessor.p));
	if(FAILED(pRBase->m_hr))
	{
		if(pRBase->m_pGetErrorInfo) 
			pRBase->m_pGetErrorInfo(pIViewRowset, __uuidof(IViewRowset), &(pRBase->m_strError));
		return false;
	}
	return pRBase->OpenRBase(pIAccessor.p);
}

bool	COView::ApplyView(CRBase *pRBase, HCHAPTER hChapter)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this view object is already opened!");
	OleDBPro_ASSERT(pRBase, "\npRBase can't be NULL!");
	HCHAPTER	hChildChapter;	
	if(pRBase->IsOpen())
		pRBase->Close();
	CComPtr<IViewChapter>	pIViewChapter;
	CComPtr<IAccessor>		pIAccessor;
	pRBase->m_hr=m_pIUnknown->QueryInterface(__uuidof(IViewChapter), (void**)&pIViewChapter);
	if(FAILED(pRBase->m_hr))
	{
		if(pRBase->m_pGetErrorInfo) 
			pRBase->m_pGetErrorInfo(m_pIUnknown, m_IID, &(pRBase->m_strError));
		return false;
	}
	pRBase->m_hr=pIViewChapter->GetSpecification(__uuidof(IAccessor), (IUnknown**)&pIAccessor);
	if(FAILED(pRBase->m_hr))
	{
		if(pRBase->m_pGetErrorInfo) 
			pRBase->m_pGetErrorInfo(pIViewChapter, __uuidof(IViewChapter), &(pRBase->m_strError));
		return false;
	}
	if(!pRBase->OpenRBase(pIAccessor.p))
		return false;
	pRBase->m_hr=pIViewChapter->OpenViewChapter(hChapter, &hChildChapter);
	if(FAILED(pRBase->m_hr))
	{
		if(pRBase->m_pGetErrorInfo) 
			pRBase->m_pGetErrorInfo(pIViewChapter, __uuidof(IViewChapter), &(pRBase->m_strError));
		return false;
	}
	pRBase->SetChapter(hChildChapter);
	return true;
}

bool	COView::SetFilter(ULONG nCols, CFilterInfo pFilterInfo[])
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this view object is already opened!");
	OleDBPro_ASSERT(nCols, "\nnCols can't be zero!");
	OleDBPro_ASSERT(pFilterInfo, "\npFilterInfo can't be NULL!");

	ULONG			nCol;
	HACCESSOR	hAccessor=0;
	ULONG			nRows;
	ULONG			nRow;
	CComPtr<IAccessor>		pIAccessor;
	CComPtr<IViewFilter>		pIViewFilter;
	CComPtr<IViewChapter>	pIViewChapter;
	m_hr=m_pIUnknown->QueryInterface(__uuidof(IViewFilter), (void**)&pIViewFilter);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIUnknown, m_IID, &m_strError);
		return false;
	}
	m_hr=pIViewFilter->QueryInterface(__uuidof(IAccessor), (void**)&pIAccessor);
	if(FAILED(m_hr))
	{
		m_hr=m_pIUnknown->QueryInterface(__uuidof(IViewChapter), (void**)&pIViewChapter);
		if(FAILED(m_hr))
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pIUnknown, m_IID, &m_strError);
			return false;
		}
		m_hr=pIViewChapter->GetSpecification(__uuidof(IAccessor), (IUnknown**)&pIAccessor);
		if(FAILED(m_hr))
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(pIViewChapter, __uuidof(IViewChapter), &m_strError);
			return false;
		}
	}
	nRows=1;
	for(nCol=0; nCol<nCols; nCol++)
	{
		if(nCol && (!pFilterInfo[nCol].m_bAnd))
			nRows++;
	}
	DBBINDING		*pDBBinds=(DBBINDING	*)CoTaskMemAlloc(nCols*sizeof(DBBINDING));
	ATLASSERT(pDBBinds);
	memset(pDBBinds, 0, nCols*sizeof(DBBINDING));
	CPData			*pData=(CPData*)CoTaskMemAlloc(nCols*nRows*sizeof(CPData));
	ATLASSERT(pData);
	DBCOMPAREOP		*pDBOps=(DBCOMPAREOP*)CoTaskMemAlloc(nCols*nRows*sizeof(DBCOMPAREOP));
	ATLASSERT(pDBOps);
	for(nCol=0; nCol<nCols; nCol++)
	{
		pDBBinds[nCol].dwMemOwner=DBMEMOWNER_CLIENTOWNED;
		pDBBinds[nCol].eParamIO=DBPARAMIO_NOTPARAM;
		pDBBinds[nCol].dwPart=DBPART_VALUE|DBPART_STATUS;
		pDBBinds[nCol].cbMaxLen=sizeof(void*);
		OleDBPro_ASSERT(pFilterInfo[nCol].m_nCol, "\npFilterInfo[nCol].m_nCol can't be zero!");
		pDBBinds[nCol].iOrdinal=pFilterInfo[nCol].m_nCol;
		OleDBPro_ASSERT((pFilterInfo[nCol].m_nDBType & DBTYPE_BYREF)!=DBTYPE_BYREF, "\nMake sure each of filter column data types is correct!");
		pDBBinds[nCol].wType=(pFilterInfo[nCol].m_nDBType|DBTYPE_BYREF);
		pDBBinds[nCol].obLength=nCol*sizeof(CPData)+offsetof(CPData, m_nLen);
		pDBBinds[nCol].obValue=nCol*sizeof(CPData)+offsetof(CPData, m_pData);
		pDBBinds[nCol].obStatus=nCol*sizeof(CPData)+offsetof(CPData, m_nDBStatus);
		pDBBinds[nCol].bPrecision=pFilterInfo[nCol].m_nPrecision;
		pDBBinds[nCol].bScale=pFilterInfo[nCol].m_nScale;
		
		if(pFilterInfo[nCol].m_pData)
		{
			pFilterInfo[nCol].m_nDBStatus=DBSTATUS_S_OK;
			if(pFilterInfo[nCol].m_nDBType==DBTYPE_STR)
			{
				pDBBinds[nCol].dwPart |=DBPART_LENGTH;
				if(pFilterInfo[nCol].m_nLen==OLEDB_STRING_END)
					pFilterInfo[nCol].m_nLen=strlen((char*)(pFilterInfo[nCol].m_pData));
			}
			else if(pFilterInfo[nCol].m_nDBType==DBTYPE_WSTR)
			{
				pDBBinds[nCol].dwPart |=DBPART_LENGTH;
				if(pFilterInfo[nCol].m_nLen==OLEDB_STRING_END)
					pFilterInfo[nCol].m_nLen=sizeof(WCHAR)*wcslen((wchar_t*)(pFilterInfo[nCol].m_pData));
			}
			else if(pFilterInfo[nCol].m_nDBType==DBTYPE_BYTES)
			{
				pDBBinds[nCol].dwPart |=DBPART_LENGTH;					
				OleDBPro_ASSERT(pFilterInfo[nCol].m_nLen!=OLEDB_STRING_END, "\nMust set the length for a binary data!");
			}
			else if(pFilterInfo[nCol].m_nDBType==DBTYPE_VARNUMERIC)
			{
				OleDBPro_ASSERT(pFilterInfo[nCol].m_nLen!=OLEDB_STRING_END, "\nMust set the length for a varnumeric data!");
				OleDBPro_ASSERT(pFilterInfo[nCol].m_nLen>=3, "\nMust set the length for a varnumeric data correctly!");
				pDBBinds[nCol].dwPart |=DBPART_LENGTH;
			}
			else
			{
//				pFilterInfo[nCol].m_nLen=*GetFixLen(pFilterInfo[nCol].m_nDBType);
			}
		}
		else
			pFilterInfo[nCol].m_nDBStatus=DBSTATUS_S_ISNULL;
	}

	for(nCol=0; nCol<nCols*nRows; nCol++)
	{
		pDBOps[nCol]=DBCOMPAREOPS_IGNORE;
		pData[nCol].m_nDBStatus=DBSTATUS_S_ISNULL;
	}

	nRow=0;
	for(nCol=0; nCol<nCols; nCol++)
	{
		if(nCol && (!pFilterInfo[nCol].m_bAnd))
			nRow++;
		pDBOps[nCols*nRow+nCol]=pFilterInfo[nCol].m_nOp;
		pData[nCols*nRow+nCol].m_nDBStatus=pFilterInfo[nCol].m_nDBStatus;
		pData[nCols*nRow+nCol].m_pData=pFilterInfo[nCol].m_pData;
		pData[nCols*nRow+nCol].m_nLen=pFilterInfo[nCol].m_nLen;
	}
	m_hr=pIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, nCols, pDBBinds, nCols*sizeof(CPData), &hAccessor, NULL);
	CoTaskMemFree(pDBBinds);
	if(FAILED(m_hr) || (!hAccessor))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIAccessor, __uuidof(IAccessor), &m_strError);
		CoTaskMemFree(pDBOps);
		CoTaskMemFree(pData);
		return false;
	}
	m_hr=pIViewFilter->SetFilter(hAccessor, nRows, pDBOps, pData);
	CoTaskMemFree(pDBOps);
	CoTaskMemFree(pData);
	pIAccessor->ReleaseAccessor(hAccessor, NULL);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIViewFilter, __uuidof(IViewFilter), &m_strError);
		return false;
	}
	return true;
}

// This is a part of the OleDBPro module.
// Copyright (C) 2000-2001 UDAParts 
// All rights reserved.
//
// This source code is only intended as a supplement to the
// OleDBPro module Classes Reference and related
// electronic documentation provided with the Module.
// See these sources for detailed information regarding this
// UDAParts product.

// Please don't disclose any source code of the software to any person or entity,
//
// Please don't decompile, disassemble, or reverse engineer any object code of 
// any portion of the software.
//  
// Copyright (c) 2001 by Yuancai (Charlie) Ye
// http://www.geocities.com/OleDBPro/index.htm
// yekerui@yahoo.com