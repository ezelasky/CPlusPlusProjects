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

CORow::CORow()
{
	m_pIRow=NULL;
}

CORow::~CORow()
{
	if(IsOpen())
		Close();
}

REFIID	CORow::GetBaseIID()
{
	return __uuidof(IRow);
}

bool	CORow::GetSession(COSession *pOSession)
{
	OleDBPro_ASSERT(pOSession, "\npOSession must point to a COSession based object!");
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a row object is available!");
	CComPtr<IGetSession> pIGetSession;
	if(pOSession)
		pOSession->Close();

	pOSession->m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IGetSession), (void**)&pIGetSession);
	if(FAILED(pOSession->m_hr))
	{
		if(pOSession->m_pGetErrorInfo) 
			pOSession->m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &(pOSession->m_strError));
		return false;
	}
	pOSession->m_hr=pIGetSession->GetSession(__uuidof(IOpenRowset), (IUnknown**)&(pOSession->m_pIOpenRowset));
	if(FAILED(pOSession->m_hr))
	{
		if(pOSession->m_pGetErrorInfo) 
			pOSession->m_pGetErrorInfo(pIGetSession, __uuidof(IGetSession), &(pOSession->m_strError));
		return false;
	}
	return true;
}

void	CORow::Close()
{
	if(m_pIRow)
	{
		m_pIRow->Release();
		m_pIRow=NULL;
	}
}

bool	CORow::GetSourceRowset(CRBase *pRBase)
{
	OleDBPro_ASSERT(pRBase, "\npRBase must point to a valid CRBase based object!");
	OleDBPro_ASSERT(m_pIRow, "\nMake sure that a row object is available!");
	HROW			hRow=0;
	if(pRBase->IsOpen())
		pRBase->Close();
	CComPtr<IAccessor> pIAccessor;
	pRBase->m_hr=m_pIRow->GetSourceRowset(__uuidof(IAccessor), (IUnknown**)&pIAccessor, &hRow);
	if(FAILED(pRBase->m_hr))
	{
		if(pRBase->m_pGetErrorInfo) 
			pRBase->m_pGetErrorInfo(m_pIRow, __uuidof(IRow), &(pRBase->m_strError));
		return false;
	}
	if(pRBase->OpenRBase(pIAccessor.p))
	{
		pRBase->m_hCurrent=hRow;
		pRBase->m_nRowsObtained=1;
		pRBase->m_hr=(*(pRBase->GetBaseInterfacePPtr()))->QueryInterface(__uuidof(IRowset), (void**)&(pRBase->m_pIRowset));
		if(FAILED(pRBase->m_hr)) 
		{
			if(pRBase->m_pGetErrorInfo) 
				pRBase->m_pGetErrorInfo((*(pRBase->GetBaseInterfacePPtr())), pRBase->GetBaseIID(), &(pRBase->m_strError));
			return false;
		}
		return pRBase->GetDataFromServer(hRow);
	}
	return false;
}

bool	CORow::GetColumns(ULONG nCols, DBCOLUMNACCESS pDBColAccess[])
{
	OleDBPro_ASSERT(m_pIRow, "\nMake sure that a row object is available!");
	m_hr=m_pIRow->GetColumns(nCols, pDBColAccess);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIRow, __uuidof(IRow), &m_strError);
		return false;
	}
	return true;
}

bool	CORow::SetColumns(ULONG nCols, DBCOLUMNACCESS pDBColAccess[])
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a row object is available!");
	CComPtr<IRowChange>	pIRowChange;
	m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IRowChange), (void**)&pIRowChange);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}
	m_hr=pIRowChange->SetColumns(nCols, pDBColAccess);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIRowChange, __uuidof(IRowChange), &m_strError);
		return false;
	}
	return true;
}

bool CORow::AddColumns(ULONG nCols, const DBCOLUMNINFO pNewColInfo[], DBCOLUMNACCESS *pDBColAccess)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a row object is available!");
	CComPtr<IRowSchemaChange> pIRowSchemaChange;
	m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IRowSchemaChange), (void**)&pIRowSchemaChange);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}
	m_hr=pIRowSchemaChange->AddColumns(nCols, pNewColInfo, pDBColAccess);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIRowSchemaChange, __uuidof(IRowSchemaChange), &m_strError);
		return false;
	}
	return true;
}

bool	CORow::DeleteColumns(ULONG nCols, const DBID pColIDs[], DBSTATUS *pnStatus)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a row object is available!");
	CComPtr<IRowSchemaChange> pIRowSchemaChange;
	m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IRowSchemaChange), (void**)&pIRowSchemaChange);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}
	m_hr=pIRowSchemaChange->DeleteColumns(nCols, pColIDs, pnStatus);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIRowSchemaChange, __uuidof(IRowSchemaChange), &m_strError);
		return false;
	}
	return true;
}

bool	CORow::Delete(LPCOLESTR strURLs[], DBSTATUS *pnStatus, ULONG nRows, DBDELETEFLAGS nDeleteFlags)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a row object is available!");
	CComPtr<IScopedOperations>		pIScopedOperations;
	m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IScopedOperations), (void**)&pIScopedOperations);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}
	m_hr=pIScopedOperations->Delete(nRows, strURLs, nDeleteFlags, pnStatus);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIScopedOperations, __uuidof(IScopedOperations), &m_strError);
		return false;
	}
	return true;
}


bool	CORow::Copy(LPCOLESTR strSourceURLs[], LPCOLESTR strDestURLs[], DBSTATUS *pnStatus, ULONG nRows, LPOLESTR *strNewURLs, OLECHAR **ppStringsBuffer, DBCOPYFLAGS nCopyFlags, IAuthenticate *pIAuthenticate)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a row object is available!");
	CComPtr<IScopedOperations>		pIScopedOperations;
	m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IScopedOperations), (void**)&pIScopedOperations);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}
	m_hr=pIScopedOperations->Copy(nRows, strSourceURLs, strDestURLs, nCopyFlags, pIAuthenticate, pnStatus, strNewURLs, ppStringsBuffer);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIScopedOperations, __uuidof(IScopedOperations), &m_strError);
		return false;
	}
	return true;
}

bool	CORow::Move(LPCOLESTR strSourceURLs[], LPCOLESTR strDestURLs[], DBSTATUS *pnStatus, ULONG nRows, LPOLESTR *strNewURLs, OLECHAR **ppStringsBuffer, DBMOVEFLAGS nMoveFlags, IAuthenticate *pIAuthenticate)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a row object is available!");
	CComPtr<IScopedOperations>		pIScopedOperations;
	m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IScopedOperations), (void**)&pIScopedOperations);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}
	m_hr=pIScopedOperations->Move(nRows, strSourceURLs, strDestURLs, nMoveFlags, pIAuthenticate, pnStatus, strNewURLs, ppStringsBuffer);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIScopedOperations, __uuidof(IScopedOperations), &m_strError);
		return false;
	}
	return true;
}

bool	CORow::GetColumnInfo(ULONG *pnCols, DBCOLUMNINFO **ppDBColInfo, OLECHAR **ppStringsBuffer)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a row object is available!");
	OleDBPro_ASSERT(ppDBColInfo, "\nppDBColInfo can't be NULL!");
	OleDBPro_ASSERT(pnCols, "\npnCols can't be NULL!");
	CComPtr<IColumnsInfo>	pIColumnsInfo;
	m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IColumnsInfo), (void**)&pIColumnsInfo);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}
	m_hr=pIColumnsInfo->GetColumnInfo(pnCols, ppDBColInfo, ppStringsBuffer);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIColumnsInfo, __uuidof(IColumnsInfo), &m_strError);
		return false;
	}
	return true;
}

bool	CORow::OpenRowset(CRBase *pRBase, LPOLESTR strTblName, LPOLESTR strIdxName, DBPROPSET *pDBPropSet, ULONG nPropSets)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a row object is available!");
	OleDBPro_ASSERT(pRBase, "\npRBase must point to a valid CRBase based object!");
	if(pRBase->IsOpen()) 
		pRBase->Close();
	CComPtr<IAccessor>				pIAccessor;
	CComPtr<IScopedOperations>		pIScopedOperations;
	pRBase->m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IScopedOperations), (void**)&pIScopedOperations);
	if(FAILED(pRBase->m_hr))
	{
		if(pRBase->m_pGetErrorInfo) 
			pRBase->m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &(pRBase->m_strError));
		return false;
	}
	DBID		dbidTbl;
	DBID		dbidIdx;
	memset(&dbidTbl, 0, sizeof(DBID));
	memset(&dbidIdx, 0, sizeof(DBID));
	dbidTbl.eKind=DBKIND_NAME;
	dbidTbl.uName.pwszName=strTblName;
	dbidIdx.eKind=DBKIND_NAME;
	dbidIdx.uName.pwszName=strIdxName;
	pRBase->m_hr=pIScopedOperations->OpenRowset(NULL, (strTblName ? &dbidTbl : NULL), (strIdxName ? &dbidIdx : NULL), __uuidof(IAccessor), (pDBPropSet ? nPropSets : 0), pDBPropSet, (IUnknown **)&pIAccessor);
	if(FAILED(pRBase->m_hr)||pIAccessor.p==NULL)
	{
		if(pRBase->m_pGetErrorInfo) 
			pRBase->m_pGetErrorInfo(pIScopedOperations, __uuidof(IScopedOperations), &(pRBase->m_strError));
		return false;
	}
	return pRBase->OpenRBase(pIAccessor.p);
}

bool	CORow::OpenRowset(LPOLESTR strTblName, CRBase *pRBase, LPOLESTR strIdxName, int nCursor, bool bReadOnly, bool bBatch, bool bScrollback, bool bBookmark, const DBPROPSET* pPropSetEx)
{
	COPropSet		DBPropSet;
	PrepareProps(DBPropSet, nCursor, bReadOnly, bBatch, bScrollback, bBookmark);
	if(pPropSetEx) 
	{
		for(ULONG nIndex=0; nIndex<pPropSetEx->cProperties; nIndex++)
			DBPropSet.AddProperty(pPropSetEx->rgProperties[nIndex].dwPropertyID, pPropSetEx->rgProperties[nIndex].vValue);
	}
	return OpenRowset(pRBase, strTblName, strIdxName, &DBPropSet);
}

bool	CORow::Open(CRBase *pRBase)
{
	OleDBPro_ASSERT(pRBase, "\npRBase must point to a valid CRBase based object!");
	OleDBPro_ASSERT(pRBase->IsOpen(), "\npRBase must point to an opened CRBase based object!");
	OleDBPro_ASSERT(pRBase->m_hCurrent, "\nMake sure that a row handle is valid!");
	if(IsOpen())
		Close();
	CComPtr<IGetRow>	pIGetRow;
	m_hr=(*(pRBase->GetBaseInterfacePPtr()))->QueryInterface(__uuidof(IGetRow), (void**)&pIGetRow);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo((*(pRBase->GetBaseInterfacePPtr())), pRBase->GetBaseIID(), &m_strError);
		return false;
	}
	m_hr=pIGetRow->GetRowFromHROW(NULL, pRBase->m_hCurrent, GetBaseIID(), GetBaseInterfacePPtr());
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIGetRow, __uuidof(IGetRow), &m_strError);
		return false;
	}
	return PostProcess();
}

bool	CORow::OpenWithCmnd(IOpenRowset *pIOpenRowset, LPCOLESTR strSelect, DBPROPSET *pDBPropSet, ULONG nPropSets, REFGUID guidDialect)
{
	if(IsOpen())
		Close();
	OleDBPro_ASSERT(pIOpenRowset, "\npIOpenRowset must be wired with a session object!");
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