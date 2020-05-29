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
#include "Oledberr.h"
#include "Odbassert.h"

//STGM_READ, __uuidof(ISequentialStream)
DBOBJECT       g_ObjectStruct={STGM_READ, {0x0c733a30,0x2a1c,0x11ce,{0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d}}};

CRBase::CRBase()
{
	Initialize();
}

CRBase::~CRBase()
{
	if(IsOpen()) 
		Close();
	if(m_pHRow)				
		CoTaskMemFree(m_pHRow);
}

bool CRBase::ReleaseRows()
{
	m_hCurrent=0;
	if(m_pIRowset)
	{
		m_hr=m_pIRowset->ReleaseRows(m_nRowsObtained, m_pHRow, NULL, NULL, NULL);
		if(FAILED(m_hr)) 
		{
			return false;
		}
	}
	return true;
}

bool	CRBase::IsChapter(ULONG nCol)
{
	OleDBPro_ASSERT(nCol && nCol<=m_nCol-IsBookmarked(), "\nMake sure that a column ordinal is correct!");
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	ATLASSERT(m_pDBColumnInfo);
	return ((m_pDBColumnInfo[nCol+IsBookmarked()-1].dwFlags & DBCOLUMNFLAGS_ISCHAPTER)==DBCOLUMNFLAGS_ISCHAPTER);
}


DBTYPE   CRBase::GetBookmarkType()
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	OleDBPro_ASSERT(IsBookmarked(), "\nMake sure that a bookmark is available!");
	return m_pDBColumnInfo[0].wType;
}

ULONG	   CRBase::GetBookmarkLen()
{
   OleDBPro_ASSERT(IsBookmarked(), "\nMake sure that a bookmark is available!");
   if(GetBookmarkType()==DBTYPE_WSTR)
      return m_pDBColumnInfo[0].ulColumnSize*sizeof(WCHAR);
   return m_pDBColumnInfo[0].ulColumnSize;
}

void*	   CRBase::GetBookmark()
{
   OleDBPro_ASSERT(IsBookmarked(), "\nMake sure that a bookmark is available!");
   OleDBPro_ASSERT(m_pIRowset, "\ncall MoveFirst, MoveLast, FindNextRow, GetRowsAt or Seek first!");
   ATLASSERT(m_nCurrentRow>=0);
   OleDBPro_ASSERT(m_nCurrentRow<m_nRowsObtained, "\nCusor position is wrong!");
	if(m_hBookmarkAccessor==0)
	{
		m_pDBBinds[0].dwFlags=DBCOLUMNFLAGS_ISBOOKMARK;
		m_hr=m_pIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, 1, m_pDBBinds, 0, &m_hBookmarkAccessor, NULL);
	}
	ATLASSERT(m_hBookmarkAccessor);
	OleDBPro_ASSERT(m_hCurrent, "\nMake sure that a row handle is valid!");
   m_hr=m_pIRowset->GetData(m_hCurrent, m_hBookmarkAccessor, m_pData);
   if(FAILED(m_hr))
   {
       if(m_pGetErrorInfo) 
			 m_pGetErrorInfo(m_pIRowset, __uuidof(IRowset), &m_strError);
       return NULL;
   }
   if(*((ULONG*)(m_pData + m_pDBBinds[0].obStatus))==DBSTATUS_S_ISNULL)
      return NULL;
   return m_pData + m_pDBBinds[0].obValue;
}

void CRBase::SetChapter(HCHAPTER hChapter)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	if(m_hChapter)
		ReleaseChapter();
	m_hChapter=hChapter;
}

REFIID	CRBase::GetBaseIID()
{
	return __uuidof(IAccessor);
}

void CRBase::SetBatchSize(ULONG nBatchSize)
{
	OleDBPro_ASSERT(nBatchSize && !m_pIRowset, "\nnBachSize can't be zero, and can't set the size of batch after the interface IRowset is queried.");
	m_nRowsetSize=nBatchSize;
	m_pHRow=(HROW*)CoTaskMemRealloc(m_pHRow, sizeof(HROW)*m_nRowsetSize);
	ATLASSERT(m_pHRow);
}

bool	CRBase::PostProcess()
{
	OleDBPro_ASSERT(m_pIAccessor, "\nCalling this function is permitted only after the interface IAccessor is available!");
	ReleaseRows();
	return QueryColsInfo();
}

const DBCOLUMNINFO* CRBase::GetColInfo()
{
	if(IsBookmarked())
		return m_pDBColumnInfo+1;

	return m_pDBColumnInfo;
}

void CRBase::FreeRecordMemory()
{
	DBTYPE		nDBType;
	DBMEMOWNER	nDBMemOwner;
	void		*pData;
	for(ULONG nCol=1; nCol<=(m_nCol-IsBookmarked()); nCol++)
	{
		pData=GetData(nCol);
		nDBType=GetDataType(nCol);
		nDBMemOwner=GetDBMemOwner(nCol);
		if(pData && nDBMemOwner==DBMEMOWNER_CLIENTOWNED)
		{
			if((nDBType & DBTYPE_ARRAY)==DBTYPE_ARRAY)
				SafeArrayDestroy((SAFEARRAY*)pData);
			else if((nDBType & DBTYPE_VECTOR)==DBTYPE_VECTOR)
			{
				if(((DBVECTOR*)pData)->ptr)
				{
					CoTaskMemFree(((DBVECTOR*)pData)->ptr);
					memset(pData, 0, sizeof(DBVECTOR));
				}
			}
			else
			{
				void	*pBuffer=NULL;
				if((nDBType & DBTYPE_BYREF) == DBTYPE_BYREF)
					pBuffer=*((void**)pData);
				else
					pBuffer=pData;
				switch (nDBType &~DBTYPE_BYREF)
				{
				case DBTYPE_BSTR:
					if(*((BSTR*)pBuffer))
					{
						SysFreeString(*((BSTR*)pBuffer));
						*((BSTR*)pBuffer)=NULL;
					}
					break;
				case DBTYPE_VARIANT:
					if(pBuffer)
						VariantClear((VARIANT*)pBuffer);
					break;
				case DBTYPE_HCHAPTER:
					if(*((HCHAPTER*)pBuffer)!=DB_NULL_HCHAPTER)
					{
						ULONG	nCount=0;
						CComPtr<IChapteredRowset> pIChapteredRowset;
						m_pIAccessor->QueryInterface(__uuidof(IChapteredRowset), (void**)&pIChapteredRowset);
						if (pIChapteredRowset != NULL)
							HRESULT hr = pIChapteredRowset->ReleaseChapter(*(HCHAPTER*)pBuffer, &nCount);
						*((HCHAPTER*)pBuffer)=DB_NULL_HCHAPTER;
					}
					break;
				default:
					break;
				}
				if((nDBType & DBTYPE_BYREF) == DBTYPE_BYREF && pBuffer)
				{
					CoTaskMemFree(pBuffer);
					*((void**)pData)=NULL;
				}
			}
		}
	}
}

ULONG CRBase::GetMaxLength(ULONG nCol)
{
	OleDBPro_ASSERT(nCol && nCol<=m_nCol-IsBookmarked(), "\nMake sure that a column ordinal is correct!");
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	ULONG nIndex=FindAnIndexOnOrdinal(nCol);
	if(nIndex!=-1)
	{
		if(IsBLOB(nCol)) return 0;
		return m_pDBBinds[nIndex].cbMaxLen;
	}
	return 0;
}

ULONG	CRBase::FindAnIndexOnOrdinal(ULONG nCol)
{
	OleDBPro_ASSERT(nCol && nCol<=m_nCol-IsBookmarked(), "\nMake sure that a column ordinal is correct!");
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");

	for(ULONG n=IsBookmarked(); n<m_nActualCol; n++)
	{
		if(m_pDBBinds[n].iOrdinal==nCol)
			return n;
	}
	return -1;
}

ULONG	CRBase::FindColOrdinal(LPCOLESTR bstrColName, bool bCaseSensitive)
{
	for(ULONG n=1; n<=m_nCol; n++)
	{
		if(bCaseSensitive)
		{
			if(wcscmp(bstrColName, GetColName(n))==0) return n;
		}
		else
		{
			if(_wcsicmp(bstrColName, GetColName(n))==0) return n;
		}
	}
	return 0;
}

bool CRBase::GetDataFromServer(HROW hRow)
{
	OleDBPro_ASSERT(m_pIRowset, "\ncall MoveFirst, MoveLast, FindNextRow, GetRowsAt or Seek first!");
	ATLASSERT(m_pIAccessor);
	OleDBPro_ASSERT(hRow, "\nMake sure that a row handle is valid!");
	ATLASSERT(m_nBufferSize);
	ATLASSERT(m_nCurrentRow>=0);
	OleDBPro_ASSERT(m_nCurrentRow<m_nRowsObtained, "\nCusor position is wrong!");
/*
#ifdef _DEBUG
	{
		DBTYPE		nDBType;
		DBMEMOWNER	nDBMemOwner;
		void		*pData;
		for(ULONG nCol=1; nCol<=(m_nCol-IsBookmarked()); nCol++)
		{
			pData=GetData(nCol);
			nDBType=GetDataType(nCol);
			nDBMemOwner=GetDBMemOwner(nCol);
			if(pData && nDBMemOwner==DBMEMOWNER_CLIENTOWNED)
			{
				if((nDBType & DBTYPE_ARRAY)==DBTYPE_ARRAY)
				{
					ATLTRACE("Memory leaking, and need to call CRBase::FreeRecordMemory to release provider-allocated buffer at the column %d (DBTYPE_XXX|DBTYPE_ARRAY).\n", nCol);
				}
				else if((nDBType & DBTYPE_VECTOR)==DBTYPE_VECTOR)
				{
					if(((DBVECTOR*)pData)->ptr)
					{
						ATLTRACE("Memory leaking, and need to call CRBase::FreeRecordMemory to release provider-allocated buffer at the column %d (DBTYPE_XXX|DBTYPE_VECTOR).\n", nCol);
					}
				}
				else
				{
					void	*pBuffer=NULL;
					if((nDBType & DBTYPE_BYREF) == DBTYPE_BYREF)
						pBuffer=*((void**)pData);
					else
						pBuffer=pData;
					switch (nDBType &~DBTYPE_BYREF)
					{
					case DBTYPE_BSTR:
						if(*((BSTR*)pBuffer))
						{
							ATLTRACE("Memory leaking, and need to call CRBase::FreeRecordMemory to release provider-allocated buffer at the column %d (BSTR).\n", nCol);
						}
						break;
					case DBTYPE_VARIANT:
						if(pBuffer)
						{
							ATLTRACE("Memory leaking, and need to call CRBase::FreeRecordMemory to release provider-allocated buffer at the column %d (VARIANT).\n", nCol);
						}
						break;
					case DBTYPE_HCHAPTER:
						if(*((HCHAPTER*)pBuffer)!=DB_NULL_HCHAPTER)
						{
							ATLTRACE("Memory leaking, and need to call CRBase::FreeRecordMemory to release provider-allocated buffer at the column %d (HCHAPTER).\n", nCol);
						}
						break;
					default:
						break;
					}
					if((nDBType & DBTYPE_BYREF) == DBTYPE_BYREF && pBuffer)
					{
						ATLTRACE("Memory leaking, and need to call CRBase::FreeRecordMemory to release provider-allocated buffer at the column %d (DBTYPE_XXX|DBTYPE_BYREF).\n", nCol);
					}
				}
			}
		}	
	}
#endif
	*/
	memset(m_pData, 0, m_nBufferSize);
	m_hr=m_pIRowset->GetData(hRow, m_hAccessor, m_pData);
	m_hCurrent=hRow;
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIRowset, __uuidof(IRowset), &m_strError);
		return false;
	}	
	return true;
}

bool	CRBase::OpenRBase(IUnknown *pIUnknownToRowset)
{
	OleDBPro_ASSERT(pIUnknownToRowset, "\npIUnknownToRowset must be wired with a rowset!");
	if(IsOpen()) 
		Close();
	m_hr=pIUnknownToRowset->QueryInterface(GetBaseIID(), (void**)GetBaseInterfacePPtr());
	if(FAILED(m_hr)) 
	{
		if(m_strError)
			SysFreeString(m_strError);
		m_strError=::SysAllocString(L"Can't query interface IAccessor.");
		return false;
	}

	return PostProcess();
}

bool CRBase::GetColumnsInfo()
{
	ATLASSERT(*GetBaseInterfacePPtr());

	m_nActualCol=0;
	m_nCol=0;
	if(m_pDBColumnInfo)
    {
		CoTaskMemFree(m_pDBColumnInfo);
	    m_pDBColumnInfo=NULL;
    }
    
    if(m_strColumnString)
    {
        CoTaskMemFree(m_strColumnString);
        m_strColumnString=NULL;
    }
	CComPtr<IColumnsInfo> pIColumnsInfo;

	m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IColumnsInfo), (void**)&pIColumnsInfo);
	if(FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(*GetBaseInterfacePPtr(), GetBaseIID(), &m_strError);
		return false;
	}

	m_hr=pIColumnsInfo->GetColumnInfo(&m_nCol, &m_pDBColumnInfo, &m_strColumnString);
	if(FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIColumnsInfo, __uuidof(IColumnsInfo), &m_strError);
		m_nCol=0;
		return false;
	}
	m_nActualCol=m_nCol;
	return true;
}

bool CRBase::GetRowsetProperty(DBPROPID propid, VARIANT* pVariant)
{
	bool	bSuccess=true;
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	OleDBPro_ASSERT(pVariant, "\npVariant can't be NULL!");
	m_hr=S_OK;
	CComPtr<IRowsetInfo>		pIRowsetInfo;
	
	m_hr=m_pIAccessor->QueryInterface(__uuidof(IRowsetInfo), (void**)&pIRowsetInfo);
	if (FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
		return false;
	}

	DBPROPIDSET	DBPropIDSet;
	DBPropIDSet.guidPropertySet=DBPROPSET_ROWSET;
	DBPropIDSet.cPropertyIDs=1;
	DBPropIDSet.rgPropertyIDs=&propid;
	
	DBPROPSET* pPropSet = NULL;
	ULONG ulPropSet = 0;
	m_hr = pIRowsetInfo->GetProperties(1, &DBPropIDSet, &ulPropSet, &pPropSet);
	if (FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIRowsetInfo, __uuidof(IRowsetInfo), &m_strError);
		bSuccess=false;
	}
	
	if(pPropSet && pPropSet->rgProperties) 
	{
		VariantCopy(pVariant, &(pPropSet->rgProperties[0].vValue));
		VariantClear(&(pPropSet->rgProperties[0].vValue));
	}
	if(pPropSet) 
		CoTaskMemFree(pPropSet->rgProperties);
	if(pPropSet) 
		CoTaskMemFree(pPropSet);

	return bSuccess;
}

void CRBase::Initialize()
{
	m_hAccessor=0;
	m_hBookmarkAccessor=0;	
	m_hChapter=DB_NULL_HCHAPTER;
	m_hCurrent=0;
	m_hWriteAccessor=0;
	m_nBufferSize=0;
	m_nCol=0;
	m_nActualCol=0;
	m_nCurrentRow=0;
	m_nRowsetSize=DEFAULT_BATCH_SIZE;
	m_nRowsObtained=0;
	m_pData=NULL;
	m_pDBBinds=NULL;
	m_pDBColumnInfo=NULL;
	m_pHRow=NULL;
	m_pIAccessor=NULL;
	m_pIRowset=NULL;
    m_strColumnString=NULL;
}

void CRBase::Close()
{
	if(m_pIAccessor) 
	{
		if(m_hChapter)
			ReleaseChapter();
		if(m_hAccessor)  m_pIAccessor->ReleaseAccessor(m_hAccessor, NULL);
		if(m_hWriteAccessor)	m_pIAccessor->ReleaseAccessor(m_hWriteAccessor, NULL);
		if(m_hBookmarkAccessor) m_pIAccessor->ReleaseAccessor(m_hBookmarkAccessor, NULL);
		m_pIAccessor->Release();
	}
	if(m_pIRowset) 
	{
		ReleaseRows();
		m_pIRowset->Release();
	}
	if(m_pDBColumnInfo)	CoTaskMemFree(m_pDBColumnInfo);
	if(m_pDBBinds)			CoTaskMemFree(m_pDBBinds);
	if(m_pData)				CoTaskMemFree(m_pData);
	if(m_pHRow)				CoTaskMemFree(m_pHRow);
    if(m_strColumnString)   CoTaskMemFree(m_strColumnString);
	Initialize();
}

bool	CRBase::CreateAccessors()
{
	if(m_hAccessor) 
		m_pIAccessor->ReleaseAccessor(m_hAccessor, NULL);
	m_hAccessor=0;
	if(m_hWriteAccessor) 
		m_pIAccessor->ReleaseAccessor(m_hWriteAccessor, NULL);
	m_hWriteAccessor=0;
	m_hr=m_pIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, GetCols(), (m_pDBBinds+IsBookmarked()), 0, &m_hAccessor, NULL);
	if(FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
		return false;
	}
	if(!IsReadOnly())
	{
		ULONG		nIndex;
		DBBINDING	*pDBBinds=(DBBINDING*)CoTaskMemAlloc(m_nCol*sizeof(DBBINDING));
		ATLASSERT(pDBBinds);
		memcpy(pDBBinds, m_pDBBinds, m_nActualCol*sizeof(DBBINDING));
		for(ULONG nCol=1; nCol<=m_nCol-IsBookmarked(); nCol++)
		{
			nIndex=FindAnIndexOnOrdinal(nCol);
			if(nIndex!=-1)
			{
				pDBBinds[nIndex].dwMemOwner=DBMEMOWNER_CLIENTOWNED;
				if(IsBLOB(nCol))
				{
					pDBBinds[nIndex].dwPart=(m_pDBBinds[nIndex].dwPart|DBPART_VALUE);
					if(m_pDBColumnInfo[nCol+IsBookmarked()-1].pTypeInfo)
					{
						pDBBinds[nIndex].pObject=&g_ObjectStruct;
						pDBBinds[nIndex].wType=DBTYPE_IUNKNOWN;
					}
					else
						pDBBinds[nIndex].wType |=DBTYPE_BYREF;
				}
			}
		}
		m_hr=m_pIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, GetCols(), (pDBBinds+IsBookmarked()), 0, &m_hWriteAccessor, NULL);
		CoTaskMemFree(pDBBinds);
		if(FAILED(m_hr)) 
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
			return false;
		}
	}
	return true;
}

bool CRBase::GetLenStatus(ULONG nCol, DBTYPE nDBType, ULONG *pnLen, ULONG *pnDBStatus,  BYTE nPrecision, BYTE nScale)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	OleDBPro_ASSERT(nCol && nCol<=m_nCol-IsBookmarked(), "\nMake sure that a column ordinal is correct!");
	OleDBPro_ASSERT(nDBType>DBTYPE_NULL, "Make sure that the input data type is correct!");

	DBBINDING		Binding;
	CPData			PData;
	HACCESSOR		hAccessor=0;
	memset(&Binding, 0, sizeof(DBBINDING));
	Binding.iOrdinal=nCol;
	Binding.dwPart=DBPART_STATUS|DBPART_LENGTH;
	Binding.wType=nDBType;
	Binding.cbMaxLen=sizeof(void*);
	Binding.dwMemOwner=DBMEMOWNER_CLIENTOWNED;
	Binding.eParamIO=DBPARAMIO_NOTPARAM;
	Binding.bPrecision=nPrecision;
	Binding.bScale=nScale;
	Binding.obStatus=offsetof(CPData, m_nDBStatus);
	Binding.obLength=offsetof(CPData, m_nLen);
	ATLASSERT(m_pIAccessor);
	m_hr=m_pIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, 1, &Binding, 0, &hAccessor, NULL);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
		return false;
	}
	OleDBPro_ASSERT(m_pIRowset, "\ncall MoveFirst, MoveLast, FindNextRow, GetRowsAt or Seek first!");
	OleDBPro_ASSERT(m_hCurrent, "\nMake sure that a row handle is valid!");
	m_hr=m_pIRowset->GetData(m_hCurrent, hAccessor, &PData);
	if(hAccessor)
		m_pIAccessor->ReleaseAccessor(hAccessor, NULL);
	if(pnLen)
		*pnLen=PData.m_nLen;
	if(pnDBStatus)
		*pnDBStatus=PData.m_nDBStatus;
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIRowset, __uuidof(IRowset), &m_strError);
		return false;
	}
	return true;
}

bool	CRBase::SetDataEx(ULONG nCol, void *pData, DBTYPE nDBType, ULONG nLen)
{
	OleDBPro_ASSERT(nCol && nCol<=m_nCol-IsBookmarked(), "\nMake sure that a column ordinal is correct!");
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	OleDBPro_ASSERT((nDBType & DBTYPE_BYREF)!=DBTYPE_BYREF, "\nDon't support data type X|DBTYPE_BYREF currently!");
	
#ifdef _DEBUG
	if(!Writable(nCol))
		AtlTrace("Warning: Column %d is not updateable!\n", nCol);
#endif
	CComPtr<IRowsetChange> pIRowsetChange;
	m_hr=m_pIAccessor->QueryInterface(__uuidof(IRowsetChange), (void**)&pIRowsetChange);
	ATLASSERT(!FAILED(m_hr));

	DBBINDING		Binding;
	HACCESSOR		hAccessor=0;
	CPData			PData;
	PData.m_pData=pData;
	if(pData)
		PData.m_nDBStatus=DBSTATUS_S_OK;
	else
	{
		PData.m_nDBStatus=DBSTATUS_S_ISNULL;
#ifdef _DEBUG
	if(!IsNullable(nCol))
		AtlTrace("Warning: Column %d is not nullable!\n", nCol);
#endif
	}
	memset(&Binding, 0, sizeof(DBBINDING));
	Binding.iOrdinal=nCol;
	PData.m_nLen=nLen;
	Binding.dwPart=DBPART_VALUE|DBPART_STATUS;
	if(pData)
	{
		if(nDBType==DBTYPE_STR)
		{
			if(nLen==OLEDB_STRING_END)
				PData.m_nLen=strlen((char*)pData);
			Binding.dwPart |=DBPART_LENGTH;
		}
		else if(nDBType==DBTYPE_WSTR)
		{
			if(nLen==OLEDB_STRING_END)
				PData.m_nLen=wcslen((WCHAR*)pData)*sizeof(WCHAR);
			Binding.dwPart |=DBPART_LENGTH;
		}
		else if(nDBType==DBTYPE_BYTES)
		{
			ATLASSERT(nLen!=OLEDB_STRING_END);
			Binding.dwPart |=DBPART_LENGTH;
		}
		else if(nDBType==DBTYPE_VARNUMERIC)
		{
			ATLASSERT(nLen>=3);
			ATLASSERT(nLen!=OLEDB_STRING_END);
			Binding.dwPart |=DBPART_LENGTH;
		}
		else if(nDBType==DBTYPE_IUNKNOWN)
		{
			ATLASSERT(nLen!=OLEDB_STRING_END);
			Binding.dwPart |=DBPART_LENGTH;
		}
	}

	if(nDBType==DBTYPE_IUNKNOWN)
	{
		Binding.wType=nDBType;
		Binding.pObject=&g_ObjectStruct;
		PData.m_pData=*((ISequentialStream**)(pData));
	}
	else
		Binding.wType=nDBType|DBTYPE_BYREF;
	Binding.cbMaxLen=sizeof(void*);
	Binding.dwMemOwner=DBMEMOWNER_CLIENTOWNED;
	Binding.eParamIO=DBPARAMIO_NOTPARAM;
	Binding.obValue=offsetof(CPData, m_pData);
	Binding.obStatus=offsetof(CPData, m_nDBStatus);
	Binding.obLength=offsetof(CPData, m_nLen);
	ATLASSERT(m_pIAccessor);
	m_hr=m_pIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, 1, &Binding, 0, &hAccessor, NULL);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
		return false;
	}
	OleDBPro_ASSERT(m_hCurrent, "\nMake sure that a row handle is valid!");
	m_hr=pIRowsetChange->SetData(m_hCurrent, hAccessor, &PData);
	if(hAccessor)
		m_pIAccessor->ReleaseAccessor(hAccessor, NULL);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIRowsetChange, __uuidof(IRowsetChange), &m_strError);
		return false;
	}
	return true;
}


bool CRBase::ReleaseChapter()
{
	ULONG	ulCount;
	CComPtr<IChapteredRowset>	pIChapteredRowset;
	if(!m_hChapter)
		return true;
	m_hr=m_pIAccessor->QueryInterface(__uuidof(IChapteredRowset), (void**)&pIChapteredRowset);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
		return false;
	}
	m_hr=pIChapteredRowset->ReleaseChapter (m_hChapter, &ulCount);
	m_hChapter=0;
#ifdef _DEBUG
	if(m_hr!=S_OK)
		AtlTrace("m_hr = %d, ulCount = %d\n", m_hr, ulCount);
#endif
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIChapteredRowset, __uuidof(IChapteredRowset), &m_strError);
		return false;
	}
	return true;
}

bool	CRBase::GetDataEx(ULONG nCol, void *pBuffer, DBTYPE nDBType, ULONG nSize, BYTE nPrecision, BYTE nScale)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	OleDBPro_ASSERT(nCol && nCol<=m_nCol-IsBookmarked(), "\nMake sure that a column ordinal is correct!");
	OleDBPro_ASSERT(pBuffer, "\npBuffer can't be NULL!");
	OleDBPro_ASSERT(nDBType>DBTYPE_NULL, "\nMake sure that the input data type is correct!");
	OleDBPro_ASSERT(nDBType!=(DBTYPE_IUNKNOWN|DBTYPE_BYREF), "\nCan't use data type (DBTYPE_IUNKNOWN|DBTYPE_BYREF)!");
	HACCESSOR		hAccessor=0;
	DBBINDING		Binding;
	memset(&Binding, 0, sizeof(DBBINDING));
	Binding.iOrdinal=nCol;
	Binding.dwPart=DBPART_VALUE;
	Binding.wType=nDBType;
	Binding.cbMaxLen=nSize;
	Binding.dwMemOwner=DBMEMOWNER_CLIENTOWNED;
	Binding.eParamIO=DBPARAMIO_NOTPARAM;
	Binding.bPrecision=nPrecision;
	Binding.bScale=nScale;
	if(nDBType==DBTYPE_IUNKNOWN)
		Binding.pObject=&g_ObjectStruct;
#ifdef _DEBUG
	{
		if(nDBType==DBTYPE_BYTES||nDBType==DBTYPE_STR||nDBType==DBTYPE_WSTR||nDBType==DBTYPE_VARNUMERIC)
			OleDBPro_ASSERT(nSize, "\nMake sure that the length of the buffer pBuffer is given!");
	}
#endif
	ATLASSERT(m_pIAccessor);
	m_hr=m_pIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, 1, &Binding, 0, &hAccessor, NULL);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
		return false;
	}
	OleDBPro_ASSERT(m_pIRowset, "\ncall MoveFirst, MoveLast, FindNextRow, GetRowsAt or Seek first!");
	m_hr=m_pIRowset->GetData(m_hCurrent, hAccessor, pBuffer);
	if(hAccessor)
		m_pIAccessor->ReleaseAccessor(hAccessor, NULL);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIRowset, __uuidof(IRowset), &m_strError);
		return false;
	}
	return true;
}

bool	CRBase::GetBLOB(ULONG nCol, void* pBlobBuffer, ULONG* pnRead)
{
	ULONG		nBytesLen;
	OleDBPro_ASSERT(nCol && nCol<=m_nCol-IsBookmarked(), "\nMake sure that a column ordinal is correct!");
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	ATLASSERT(!IsEOF());
	OleDBPro_ASSERT(IsBLOB(nCol), "\nThe column must be BLOB!");
	OleDBPro_ASSERT(pBlobBuffer, "\npBlobBuffer can be NULL!");
	
	HACCESSOR		hAccessor=0;
	DBBINDSTATUS	nStatus=0;
	DBBINDING		Binding;
	memset(&Binding, 0, sizeof(DBBINDING));

	Binding.iOrdinal=nCol;
	Binding.dwPart=DBPART_VALUE;
	Binding.wType=DBTYPE_IUNKNOWN;
	Binding.cbMaxLen=sizeof(IUnknown *);
	Binding.dwMemOwner=DBMEMOWNER_CLIENTOWNED;
	Binding.eParamIO=DBPARAMIO_NOTPARAM;
	Binding.pObject=&g_ObjectStruct;
	
	ATLASSERT(m_pIAccessor);
	m_hr=m_pIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, 1, &Binding, 0, &hAccessor, &nStatus);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
		return false;
	}
	BYTE pData[sizeof(IUnknown *)];
	memset(pData, 0, sizeof(IUnknown *));
	CComPtr<ISequentialStream>	pISequentialStream;
	OleDBPro_ASSERT(m_pIRowset, "\ncall MoveFirst, MoveLast, FindNextRow, GetRowsAt or Seek first!");
	OleDBPro_ASSERT(m_hCurrent, "\nMake sure that a row handle is valid!");
	OleDBPro_ASSERT(m_nCurrentRow<m_nRowsObtained, "\nCusor position is wrong!");
	m_hr=m_pIRowset->GetData(m_hCurrent, hAccessor, pData);
	if(hAccessor)
		m_pIAccessor->ReleaseAccessor(hAccessor, NULL);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIRowset, __uuidof(IRowset), &m_strError);
		return false;
	}	
	pISequentialStream.p=(*((ISequentialStream **)pData));
	if(!pISequentialStream) return false;
	if(pnRead)
		nBytesLen=*pnRead;
	else
		nBytesLen=GetLength(nCol);	//assuming caller allocated enough memory
	
	OleDBPro_ASSERT(nBytesLen, "\nCan't track the length of buffer pBlobBuffer in bytes!");
	m_hr=pISequentialStream->Read(pBlobBuffer, nBytesLen, pnRead);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pISequentialStream, __uuidof(ISequentialStream), &m_strError);
		return false;
	}
	return true;
}

bool CRBase::CreateDBBindings()
{
	ATLASSERT(m_nCol);
	ATLASSERT(m_pDBColumnInfo);
	ATLASSERT(m_pDBBinds);
	for(ULONG nCol=0; nCol<m_nCol; nCol++)
	{
		//Ordinal
		m_pDBBinds[nCol].iOrdinal = nCol+(!IsBookmarked()); 
		
		//dwMemOwner
		m_pDBBinds[nCol].dwMemOwner = DBMEMOWNER_CLIENTOWNED;
		
		m_pDBColumnInfo[nCol].pTypeInfo=NULL;

		//wType
		m_pDBBinds[nCol].wType = m_pDBColumnInfo[nCol].wType;

		//eParamIO
		m_pDBBinds[nCol].eParamIO = DBPARAMIO_NOTPARAM;
		
		//bPrecision & bScale
		if(m_pDBColumnInfo[nCol].wType==DBTYPE_DECIMAL || m_pDBColumnInfo[nCol].wType==DBTYPE_NUMERIC || m_pDBColumnInfo[nCol].wType==DBTYPE_VARNUMERIC)
		{
			m_pDBBinds[nCol].bPrecision=m_pDBColumnInfo[nCol].bPrecision;
			m_pDBBinds[nCol].bScale=m_pDBColumnInfo[nCol].bScale;
		} 

		if(m_pDBColumnInfo[nCol].wType==DBTYPE_BYTES||m_pDBColumnInfo[nCol].wType==DBTYPE_WSTR||m_pDBColumnInfo[nCol].wType==DBTYPE_STR||m_pDBColumnInfo[nCol].wType==DBTYPE_VARNUMERIC) //variable type
		{
			if((m_pDBColumnInfo[nCol].dwFlags & DBCOLUMNFLAGS_ISLONG)==DBCOLUMNFLAGS_ISLONG || (m_pDBColumnInfo[nCol].ulColumnSize+1)==0)
			{
				m_pDBBinds[nCol].dwPart = DBPART_LENGTH|DBPART_STATUS;
				m_pDBBinds[nCol].cbMaxLen=sizeof(void*);
			}
			else
			{
				m_pDBBinds[nCol].dwPart=(DBPART_VALUE|DBPART_LENGTH|DBPART_STATUS);
				if(m_pDBColumnInfo[nCol].wType==DBTYPE_BYTES)
					m_pDBBinds[nCol].cbMaxLen=m_pDBColumnInfo[nCol].ulColumnSize+1;	
				else if(m_pDBColumnInfo[nCol].wType==DBTYPE_STR)
					m_pDBBinds[nCol].cbMaxLen=m_pDBColumnInfo[nCol].ulColumnSize+1;
				else if(m_pDBColumnInfo[nCol].wType==DBTYPE_VARNUMERIC)
					m_pDBBinds[nCol].cbMaxLen=m_pDBColumnInfo[nCol].ulColumnSize+3;	//Precison + scale + size
				else
					m_pDBBinds[nCol].cbMaxLen=sizeof(WCHAR)*(m_pDBColumnInfo[nCol].ulColumnSize+1);	
			}
		}
		else	//fixed type
		{
			m_pDBBinds[nCol].dwPart=DBPART_VALUE|DBPART_STATUS;
			m_pDBBinds[nCol].cbMaxLen=*GetFixLen(m_pDBBinds[nCol].wType);
		}
	}
	SetBindingValueLenStatus();
	
	m_pData=(BYTE*)CoTaskMemRealloc(m_pData, m_nBufferSize);
	ATLASSERT(m_pData);
	memset(m_pData, 0, m_nBufferSize);
	
	OnCreatingAccessors();
	if(CreateAccessors())
		return true;
	Close();
	return false;
}

void CRBase::OnCreatingAccessors()
{
	
}

void	CRBase::SetBindingValueLenStatus()
{
	m_nBufferSize=0;
	if(IsBookmarked())
	{
		m_nBufferSize=m_pDBBinds[0].cbMaxLen;
		if((m_pDBBinds[0].dwPart & DBPART_LENGTH)==DBPART_LENGTH) 
		{
			m_pDBBinds[0].obLength=m_nBufferSize;
			m_nBufferSize +=sizeof(ULONG);
		}
		else
			m_pDBBinds[0].obValue=0;
		if((m_pDBBinds[0].dwPart & DBPART_STATUS)==DBPART_STATUS) 
		{
			m_pDBBinds[0].obStatus=m_nBufferSize;
			m_nBufferSize +=sizeof(ULONG);
		}
		else
			m_pDBBinds[0].obStatus=0;
	}
	for(ULONG nCol=IsBookmarked(); nCol<m_nActualCol; nCol++)
	{
		m_pDBBinds[nCol].obValue=m_nBufferSize;
		m_nBufferSize +=m_pDBBinds[nCol].cbMaxLen;
		
		if((m_pDBBinds[nCol].dwPart & DBPART_LENGTH)==DBPART_LENGTH)
		{
			m_pDBBinds[nCol].obLength=m_nBufferSize;
			m_nBufferSize +=sizeof(ULONG);
		}
		else
			m_pDBBinds[nCol].obLength=0;

		if((m_pDBBinds[nCol].dwPart & DBPART_STATUS)==DBPART_STATUS)
		{
			m_pDBBinds[nCol].obStatus=m_nBufferSize;
			m_nBufferSize +=sizeof(DWORD);
		}
		else
			m_pDBBinds[nCol].obStatus=0;
	}
}

bool	CRBase::QueryColsInfo()
{
	m_pHRow=(HROW*)CoTaskMemRealloc(m_pHRow, sizeof(HROW)*m_nRowsetSize);
	ATLASSERT(m_pHRow);
	if(!GetColumnsInfo())
	{
		Close();
		return false;
	}
	m_pDBBinds=(DBBINDING*)(CoTaskMemRealloc(m_pDBBinds, sizeof(DBBINDING)*m_nCol));
	ATLASSERT(m_pDBBinds);
	memset(m_pDBBinds, 0, (m_nCol*sizeof(DBBINDING)));
	return CreateDBBindings();
}

DBPART	CRBase::GetDBPart(ULONG nCol)
{
	OleDBPro_ASSERT(nCol && nCol<=m_nCol-IsBookmarked(), "\nMake sure that a column ordinal is correct!");
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");

	ULONG nIndex=FindAnIndexOnOrdinal(nCol);
	if(nIndex!=-1)
		return m_pDBBinds[nIndex].dwPart;
	return 0;
}

bool	CRBase::SetDBPart(ULONG nCol, DBPART nDBPart)
{
	OleDBPro_ASSERT(nCol && nCol<=m_nCol-IsBookmarked(), "\nMake sure that a column ordinal is correct!");
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	OleDBPro_ASSERT(nDBPart<=(DBPART_LENGTH|DBPART_VALUE|DBPART_STATUS), "\nMake sure that nDBPart value is correct!");
	ULONG nIndex=FindAnIndexOnOrdinal(nCol);
	if(nDBPart)
	{
		if(nIndex==-1)
		{
			nIndex=m_nActualCol;
			m_nActualCol++;
			m_pDBBinds=(DBBINDING*)CoTaskMemRealloc(m_pDBBinds, m_nActualCol*sizeof(DBBINDING));
			ATLASSERT(m_pDBBinds);
			m_pDBBinds[nIndex].iOrdinal=nCol;
			m_pDBBinds[nIndex].bPrecision=m_pDBColumnInfo[nCol+IsBookmarked()-1].bPrecision;
			m_pDBBinds[nIndex].bScale=m_pDBColumnInfo[nCol+IsBookmarked()-1].bScale;
			m_pDBBinds[nIndex].wType=m_pDBColumnInfo[nCol+IsBookmarked()-1].wType;
			m_pDBBinds[nIndex].dwFlags=0;
			m_pDBBinds[nIndex].eParamIO=DBPARAMIO_NOTPARAM;
			m_pDBBinds[nIndex].pBindExt=NULL;
			m_pDBBinds[nIndex].pObject=NULL;
			m_pDBBinds[nIndex].pTypeInfo=NULL;
			m_pDBBinds[nIndex].dwMemOwner=DBMEMOWNER_CLIENTOWNED;
			m_pDBColumnInfo[nCol+IsBookmarked()-1].pTypeInfo=NULL;
            m_pDBBinds[nIndex].dwPart=(DBPART_VALUE|nDBPart);
			if(m_pDBBinds[nIndex].wType==DBTYPE_STR)
				m_pDBBinds[nIndex].cbMaxLen=m_pDBColumnInfo[nCol+IsBookmarked()-1].ulColumnSize+1;
			else if(m_pDBBinds[nIndex].wType==DBTYPE_BYTES)
				m_pDBBinds[nIndex].cbMaxLen=m_pDBColumnInfo[nCol+IsBookmarked()-1].ulColumnSize+1;
			else if (m_pDBBinds[nIndex].wType==DBTYPE_VARNUMERIC)
				m_pDBBinds[nIndex].cbMaxLen=m_pDBColumnInfo[nCol+IsBookmarked()-1].ulColumnSize+3;	//scale + precision + size
			else if(m_pDBBinds[nIndex].wType==DBTYPE_WSTR)
				m_pDBBinds[nIndex].cbMaxLen=sizeof(WCHAR)*(m_pDBColumnInfo[nCol+IsBookmarked()-1].ulColumnSize+1);
			else
			{
				m_pDBBinds[nIndex].cbMaxLen=*GetFixLen(m_pDBBinds[nIndex].wType);
				m_pDBBinds[nIndex].dwPart=(m_pDBBinds[nIndex].dwPart &~ DBPART_LENGTH);
			}
		}
        else
        {
			if(m_pDBBinds[nIndex].wType==DBTYPE_STR || 
                m_pDBBinds[nIndex].wType==DBTYPE_WSTR || 
                m_pDBBinds[nIndex].wType==DBTYPE_BYTES ||
                m_pDBBinds[nIndex].wType==DBTYPE_VARNUMERIC)
            {
                m_pDBBinds[nIndex].dwPart=(DBPART_VALUE|nDBPart);
            }
			else
			{
				m_pDBBinds[nIndex].cbMaxLen=*GetFixLen(m_pDBBinds[nIndex].wType);
				m_pDBBinds[nIndex].dwPart=(m_pDBBinds[nIndex].dwPart &~ DBPART_LENGTH);
			}

        }
		if(IsBLOB(nCol))
		{
			m_pDBBinds[nIndex].cbMaxLen=sizeof(void*);
			m_pDBBinds[nIndex].dwPart=(DBPART_LENGTH|(DBPART_STATUS & nDBPart));
		}
	}
	else
	{
		if(nIndex==-1)
			return true;
		m_nActualCol--;
		ATLASSERT(GetCols());
		DBBINDING	*pDBBinds=(DBBINDING*)CoTaskMemAlloc(m_nActualCol*sizeof(DBBINDING));
		ATLASSERT(pDBBinds);
		for(ULONG n=0; n<=m_nActualCol; n++)
		{
			if(n<nIndex)
				pDBBinds[n]=m_pDBBinds[n];
			else if(n>nIndex)
				pDBBinds[n-1]=m_pDBBinds[n];
			else
			{
			}
		}
		CoTaskMemFree(m_pDBBinds);
		m_pDBBinds=pDBBinds;
	}
	SetBindingValueLenStatus();
	m_pData=(BYTE*)CoTaskMemRealloc(m_pData, m_nBufferSize);
	ATLASSERT(m_pData);
	memset(m_pData, 0, m_nBufferSize);
	return CreateAccessors();
}

bool CRBase::SetDataType(ULONG nCol, DBTYPE nDBType, ULONG nLen, BYTE nPrecision, BYTE nScale, DBMEMOWNER nDBMemOwner)
{
	OleDBPro_ASSERT(nCol && nCol<=m_nCol-IsBookmarked(), "\nMake sure that a column ordinal is correct!");
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	ATLASSERT(m_pDBColumnInfo);
	ATLASSERT(m_pDBBinds);
	ATLASSERT(m_pIAccessor);
	OleDBPro_ASSERT(nDBType !=(DBTYPE_IUNKNOWN|DBTYPE_BYREF), "Make sure that input data type is correct!");
	OleDBPro_ASSERT(nDBType !=DBTYPE_ERROR, "Make sure that input data type is correct!");
	OleDBPro_ASSERT(nDBType !=DBTYPE_IDISPATCH, "Make sure that input data type is correct!");
	OleDBPro_ASSERT(nDBType !=DBTYPE_NULL, "Make sure that input data type is correct!");
	OleDBPro_ASSERT(nDBType !=DBTYPE_EMPTY, "Make sure that input data type is correct!");
	OleDBPro_ASSERT(nDBType !=DBTYPE_VECTOR, "Make sure that input data type is correct!");
	OleDBPro_ASSERT(nDBType !=DBTYPE_RESERVED, "Make sure that input data type is correct!");
	OleDBPro_ASSERT(nDBType !=DBTYPE_UDT, "Make sure that input data type is correct!");
	OleDBPro_ASSERT(nDBType !=DBTYPE_ARRAY, "Make sure that input data type is correct!");

	ULONG		nIndex=FindAnIndexOnOrdinal(nCol);
	if(nIndex==-1)
		return false;

	DBTYPE	nType=m_pDBColumnInfo[nCol+IsBookmarked()-1].wType;
	CComPtr<IConvertType>	pIConvertType;
	m_hr=m_pIAccessor->QueryInterface(__uuidof(IConvertType), (void**)&pIConvertType);
	ATLASSERT((m_hr==S_OK && pIConvertType));
	if(!(IsBLOB(nCol) && nDBType==DBTYPE_IUNKNOWN))
	{
		m_hr=pIConvertType->CanConvert(nType, nDBType, DBCONVERTFLAGS_COLUMN);
		if(FAILED(m_hr)||m_hr==S_FALSE)
		{
			ATLTRACE("Data type conversion not supported at column %d!", nCol);
			return false;
		}
	}
	
	if(IsBLOB(nCol)) 
	{
		if(nDBType==m_pDBBinds[nIndex].wType) 
			return true;
		if(nDBType==DBTYPE_WSTR || nDBType==DBTYPE_STR || nDBType==DBTYPE_BYTES)
		{
			m_pDBBinds[nIndex].wType=nDBType;
			m_pDBColumnInfo[nCol+IsBookmarked()-1].pTypeInfo=NULL;
		}
		else if (nDBType==DBTYPE_IUNKNOWN)
		{
			m_pDBBinds[nIndex].wType=m_pDBColumnInfo[nCol+IsBookmarked()-1].wType;
			m_pDBColumnInfo[nCol+IsBookmarked()-1].pTypeInfo=(ITypeInfo*)&g_ObjectStruct;
		}
		else
			return false;
	}
	else
	{
		ULONG nNewLen;
		m_pDBBinds[nIndex].wType=nDBType;
		m_pDBBinds[nIndex].bPrecision=nPrecision;
		m_pDBBinds[nIndex].bScale=nScale;
		if((nDBType & DBTYPE_BYREF)==DBTYPE_BYREF || nDBType == DBTYPE_BSTR || (nDBType & DBTYPE_VECTOR)==DBTYPE_VECTOR || (nDBType & DBTYPE_ARRAY)==DBTYPE_ARRAY || nDBType == DBTYPE_VARIANT || nDBType == DBTYPE_PROPVARIANT)
			m_pDBBinds[nIndex].dwMemOwner=nDBMemOwner;
		else
			m_pDBBinds[nIndex].dwMemOwner=DBMEMOWNER_CLIENTOWNED;
		switch (nDBType)
		{
		case DBTYPE_BYTES:
		case DBTYPE_STR:
		case DBTYPE_WSTR:
			m_pDBBinds[nIndex].dwPart |=DBPART_LENGTH;
			nNewLen=nLen;
			break;
		case DBTYPE_VARNUMERIC:
			m_pDBBinds[nIndex].dwPart |=DBPART_LENGTH;
			nNewLen=nLen+3; //Precision + Scale + Site
			break;
		default:
			{
				DBTYPE	nType=(nDBType &~ DBTYPE_BYREF);
				if(nType==DBTYPE_WSTR || nType==DBTYPE_STR || nType==DBTYPE_BYTES || nType==DBTYPE_VARNUMERIC)
					m_pDBBinds[nIndex].dwPart |=DBPART_LENGTH;
				else
					m_pDBBinds[nIndex].dwPart = (m_pDBBinds[nIndex].dwPart &~ DBPART_LENGTH);
				nNewLen=*GetFixLen(nDBType);
			}
			break;
		}
		m_pDBBinds[nIndex].cbMaxLen=nNewLen;
		OleDBPro_ASSERT(nNewLen, "\nNeed to input the buffer size!");
	}
	SetBindingValueLenStatus();
	m_pData=(BYTE*)CoTaskMemRealloc(m_pData, m_nBufferSize);
	ATLASSERT(m_pData);
	memset(m_pData, 0, m_nBufferSize);
	return CreateAccessors();
}

bool	CRBase::SetData(ULONG nCol, void *pSrcData, ULONG nSrcLen)
{
	OleDBPro_ASSERT(nCol && nCol<=m_nCol-IsBookmarked(), "\nMake sure that a column ordinal is correct!");
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	
#ifdef _DEBUG
	if(!Writable(nCol))
		AtlTrace("Warning: Column %d is not updateable!\n", nCol);
#endif

	ULONG		nLen;
	DBTYPE		nDBType;
	void		*pData;
	void		*pSource;
	ULONG		*pnStatus=GetStatusPtr(nCol);
	if(pnStatus)
	{
		if(pSrcData==NULL) 
		{
#ifdef _DEBUG
	if(!IsNullable(nCol))
		AtlTrace("Warning: Column %d is not nullable!\n", nCol);
#endif
			*pnStatus=DBSTATUS_S_ISNULL;
			return true;
		}
		else
			*pnStatus=DBSTATUS_S_OK;
	}
	nDBType=GetDataType(nCol);
	if((nDBType & DBTYPE_BYREF)==DBTYPE_BYREF)
		pSource=*((void**)pSrcData);
	else
		pSource=pSrcData;
	ULONG		*pnLen=GetLengthPtr(nCol);
	if(m_pDBColumnInfo[nCol+IsBookmarked()-1].pTypeInfo)
		nDBType=DBTYPE_IUNKNOWN;
    
    switch (nDBType)
	{
	case DBTYPE_STR:
		if(nSrcLen==OLEDB_STRING_END)
			nLen=strlen((char*)pSource);
		else
			nLen=nSrcLen;
        if(pnLen)
		    *pnLen=nLen;
		break;
	case DBTYPE_WSTR:
		if(nSrcLen==OLEDB_STRING_END)
			nLen=wcslen((WCHAR*)pSource)*sizeof(WCHAR);
		else
			nLen=nSrcLen;
        if(pnLen)
		    *pnLen=nLen;
		break;
	case DBTYPE_BYTES:
		OleDBPro_ASSERT(nSrcLen!=OLEDB_STRING_END, "\nMust set the length for a binary column!");	//binary data, length must be given
		nLen=nSrcLen;
        if(pnLen)
		    *pnLen=nLen;
		break;
	case DBTYPE_VARNUMERIC:
		OleDBPro_ASSERT(nSrcLen>=3, "\nMust correctly set the length for a varnumeric column!");
		OleDBPro_ASSERT(nSrcLen!=OLEDB_STRING_END, "\nMust set the length for a varnumeric column!");	//length must be set
		nLen=nSrcLen;
        if(pnLen)
		    *pnLen=nLen;
		break;
	case DBTYPE_NULL:
		return false;
	case DBTYPE_IUNKNOWN:
		OleDBPro_ASSERT(nSrcLen!=OLEDB_STRING_END, "\nMust set the length for a BLOB column!");	//binary data, length must be given
		nLen=nSrcLen;
        if(pnLen)
		    *pnLen=nLen;
	default:
		nLen=*GetFixLen(nDBType);
		break;
	}

#ifdef _DEBUG
	if(nLen>GetMaxLength(nCol) && !IsBLOB(nCol))
		ATLTRACE("Data truncated at Column %d!", nCol);
#endif
	if(IsBLOB(nCol))
	{
		ULONG		nIndex=FindAnIndexOnOrdinal(nCol);
		if(nIndex!=-1)
		{
			if(m_pDBColumnInfo[nCol+IsBookmarked()-1].pTypeInfo)
				memcpy(m_pData + m_pDBBinds[nIndex].obValue, pSrcData, sizeof(IUnknown*));	//for BLOB data, remember source data address only
			else
				memcpy(m_pData + m_pDBBinds[nIndex].obValue, &pSrcData, sizeof(void*));	//for BLOB data, remember source data address only
			return true;
		}
		return false;
	}
	pData=GetData(nCol);
	if(pData)
	{
		if(nDBType==DBTYPE_STR || nDBType==DBTYPE_WSTR || nDBType==DBTYPE_BYTES)
			memset(pData, 0, GetMaxLength(nCol));
		memcpy(pData, pSrcData, nLen);
		return true;
	}
	return false;
}

bool CRBase::MoveLast()
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	ATLASSERT(m_pIAccessor);
	ATLASSERT(m_pData);
	ATLASSERT(m_pHRow);

	if(!m_pIRowset)
	{
		m_hr=m_pIAccessor->QueryInterface(__uuidof(IRowset), (void**)&m_pIRowset);
		if(FAILED(m_hr)) 
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
			return false;
		}
	}
	else
		ReleaseRows();
	m_nCurrentRow=0;
	m_hr=m_pIRowset->RestartPosition(m_hChapter);
	if(FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIRowset, __uuidof(IRowset), &m_strError);
		return false;
	}
	m_hr=m_pIRowset->GetNextRows(m_hChapter, -1, 1, &m_nRowsObtained, &m_pHRow);
	ATLTRACE(_T("Records %d obtained.\n"), m_nRowsObtained);
	if(FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIRowset, __uuidof(IRowset), &m_strError);
		return false;
	}
	if(m_nRowsObtained)
		return GetDataFromServer(m_pHRow[m_nCurrentRow]);
	else
		return true;
}

bool CRBase::MoveFirst()
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	ATLASSERT(m_pIAccessor);
	ATLASSERT(m_pData);
	ATLASSERT(m_pHRow);

	if(!m_pIRowset)
	{
		m_hr=m_pIAccessor->QueryInterface(__uuidof(IRowset), (void**)&m_pIRowset);
		if(FAILED(m_hr)) 
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
			return false;
		}
	}
	else
		ReleaseRows();

	m_nRowsObtained=0;
	m_nCurrentRow=0;
	
	m_hr=m_pIRowset->RestartPosition(m_hChapter);
	if(FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIRowset, __uuidof(IRowset), &m_strError);
		return false;
	}
	
	m_hr=m_pIRowset->GetNextRows(m_hChapter, 0, m_nRowsetSize, &m_nRowsObtained, &m_pHRow);
	if(FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIRowset, __uuidof(IRowset), &m_strError);
		return false;
	}
	if(m_nRowsObtained)
		return GetDataFromServer(m_pHRow[m_nCurrentRow]);
	return true;
}

bool CRBase::MovePrev()
{
	return MoveNext(-2);
}


bool CRBase::MoveNext(LONG nSkipped)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	OleDBPro_ASSERT(m_pIRowset, "\ncall MoveFirst, MoveLast, FindNextRow, GetRowsAt or Seek first!");
	ATLASSERT(m_pIAccessor);
	OleDBPro_ASSERT(m_nCurrentRow<m_nRowsObtained, "\nCusor position is wrong!");
	ATLASSERT(m_nCurrentRow>=0);
	m_nCurrentRow++;
	LONG nJump=m_nCurrentRow+nSkipped;
	if(nJump>=((LONG)m_nRowsObtained))
	{
		ReleaseRows();
		m_nCurrentRow=0;
		m_hr=m_pIRowset->GetNextRows(m_hChapter, (nJump-m_nRowsObtained), m_nRowsetSize, &m_nRowsObtained, &m_pHRow);
		if(FAILED(m_hr)) 
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pIRowset, __uuidof(IRowset), &m_strError);
			return false;
		}
	}
	else if(nJump<0)
	{
		ReleaseRows();
//		m_hr=m_pIRowset->GetNextRows(m_hChapter, 1+nJump-m_nRowsetSize-m_nRowsObtained, m_nRowsetSize, &m_nRowsObtained, &m_pHRow);
		m_hr=m_pIRowset->GetNextRows(m_hChapter, nJump-m_nRowsObtained, 1, &m_nRowsObtained, &m_pHRow);
		if(FAILED(m_hr)) 
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pIRowset, __uuidof(IRowset), &m_strError);
			return false;
		}
		m_nCurrentRow=m_nRowsObtained-1;
	}
	else
	{
		m_nCurrentRow=m_nCurrentRow+nSkipped;
	}
	if(m_nRowsObtained)
		return GetDataFromServer(m_pHRow[m_nCurrentRow]);
	return true;
}

bool CRBase::GetSession(COSession *pOSession)
{
	OleDBPro_ASSERT(pOSession, "\npOSession must point to a valid COSession based object!");
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	CComPtr<IRowsetInfo>    pIRowsetInfo;

   if(pOSession->IsOpen())
      pOSession->Close();
   
   pOSession->m_hr=m_pIAccessor->QueryInterface(__uuidof(IRowsetInfo), (void**)&pIRowsetInfo);
   if(FAILED(pOSession->m_hr) || pIRowsetInfo==NULL)
   {
      if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &(pOSession->m_strError));
      return false; 
   }
   pOSession->m_hr=pIRowsetInfo->GetSpecification(__uuidof(IOpenRowset), (IUnknown**)&(pOSession->m_pIOpenRowset));
   if(FAILED(pOSession->m_hr))
   {
      CComPtr<ICommandText> pICommandText;
		pOSession->m_hr=pIRowsetInfo->GetSpecification(__uuidof(ICommandText), (IUnknown**)&pICommandText);
		if(FAILED(pOSession->m_hr))
		{
			if(pOSession->m_pGetErrorInfo) 
				pOSession->m_pGetErrorInfo(pIRowsetInfo, __uuidof(IRowsetInfo), &(pOSession->m_strError));
			return false;
		}
		pOSession->m_hr=pICommandText->GetDBSession(__uuidof(IOpenRowset), (IUnknown**)&(pOSession->m_pIOpenRowset));
		if(FAILED(pOSession->m_hr))
		{
			if(pOSession->m_pGetErrorInfo) 
				pOSession->m_pGetErrorInfo(pICommandText, __uuidof(ICommandText), &(pOSession->m_strError));
			return false;
		}
   }
   return true;
}


bool	CRBaseEx::GetRowsAt(const BYTE* pBookmark, LONG nRowsOffset, ULONG nBookmarkLen)
{
	OleDBPro_ASSERT(IsBookmarked(), "\nMake sure that a bookmark is available!");
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	OleDBPro_ASSERT(pBookmark, "\npBookmark can't be NULL!");
	
	CComPtr<IRowsetLocate>	pIRowsetLocate;
	m_hr=m_pIAccessor->QueryInterface(__uuidof(IRowsetLocate), (void**)&pIRowsetLocate);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
		return false;
	}
	if(!m_pIRowset)
	{
		m_hr=m_pIAccessor->QueryInterface(__uuidof(IRowset), (void**)&m_pIRowset);
		if(FAILED(m_hr)) 
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
			return false;
		}
	}
	else
		ReleaseRows();
	m_nCurrentRow=0;
	if(!nBookmarkLen && pBookmark)
		nBookmarkLen=GetBookmarkLen();
	m_hr=pIRowsetLocate->GetRowsAt(m_hChapter, NULL, nBookmarkLen, pBookmark, nRowsOffset, m_nRowsetSize, &m_nRowsObtained, &m_pHRow);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIRowsetLocate, __uuidof(IRowsetLocate), &m_strError);
		return false;
	}
	if(m_nRowsObtained)
		return GetDataFromServer(m_pHRow[m_nCurrentRow]);
	return true;
}

bool	CRBaseEx::SetRange(ULONG nStartKeys, CKeyInfo *pStartKeys, ULONG nEndKeys, CKeyInfo *pEndKeys, DBRANGE nRangeOptions)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	ULONG		n;
	ULONG		nTotalKeys=0;
	if(nStartKeys && pStartKeys)
		nTotalKeys +=nStartKeys;
	else
	{
		pStartKeys=NULL;
		nStartKeys=0;
	}
	if(nEndKeys && pEndKeys)
		nTotalKeys +=nEndKeys;
	else
	{
		pEndKeys=NULL;
		nEndKeys=0;
	}
	CComPtr<IRowsetIndex>		pIRowsetIndex;
	m_hr=m_pIAccessor->QueryInterface(__uuidof(IRowsetIndex), (void**)&pIRowsetIndex);
	if(FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
		return false;
	}
	DBBINDING	*pDBBindings=(DBBINDING*)CoTaskMemAlloc(nTotalKeys*sizeof(DBBINDING));
	ATLASSERT(pDBBindings);
	memset(pDBBindings, 0, nTotalKeys*sizeof(DBBINDING));
	for(n=0; n<nStartKeys; n++)
	{
		pDBBindings[n].dwMemOwner=DBMEMOWNER_CLIENTOWNED;
		pDBBindings[n].eParamIO=DBPARAMIO_NOTPARAM;
		pDBBindings[n].cbMaxLen=sizeof(void*);
		pDBBindings[n].dwPart=DBPART_VALUE|DBPART_STATUS;
		ATLASSERT(pStartKeys[n].m_nCol);
		ATLASSERT(pStartKeys[n].m_nDBType>DBTYPE_NULL);	//make sure DBTYPE is correct
		ATLASSERT(pStartKeys[n].m_nDBType<DBTYPE_VECTOR);
		pDBBindings[n].iOrdinal=pStartKeys[n].m_nCol;
		ATLASSERT((pStartKeys[n].m_nDBType & DBTYPE_BYREF)!=DBTYPE_BYREF);
		pDBBindings[n].wType=pStartKeys[n].m_nDBType|DBTYPE_BYREF;
		pDBBindings[n].obLength=n*sizeof(CKeyInfo)+offsetof(CKeyInfo, m_nLen);
		pDBBindings[n].obValue=n*sizeof(CKeyInfo)+offsetof(CKeyInfo, m_pData);
		pDBBindings[n].obStatus=n*sizeof(CKeyInfo)+offsetof(CKeyInfo, m_nDBStatus);
		pDBBindings[n].bPrecision=pStartKeys[n].m_nPrecision;
		pDBBindings[n].bScale=pStartKeys[n].m_nScale;
		ATLASSERT(pStartKeys[n].m_pData);
		pStartKeys[n].m_nDBStatus=DBSTATUS_S_OK;
		if(pStartKeys[n].m_nDBType==DBTYPE_STR)
		{
			pDBBindings[n].dwPart|=DBPART_LENGTH;
			if(pStartKeys[n].m_nLen==OLEDB_STRING_END)
				pStartKeys[n].m_nLen=strlen((char*)(pStartKeys[n].m_pData));
		}
		else if(pStartKeys[n].m_nDBType==DBTYPE_WSTR)
		{
			pDBBindings[n].dwPart|=DBPART_LENGTH;
			if(pStartKeys[n].m_nLen==OLEDB_STRING_END)
				pStartKeys[n].m_nLen=sizeof(WCHAR)*wcslen((wchar_t*)(pStartKeys[n].m_pData));
		}
		else if(pStartKeys[n].m_nDBType==DBTYPE_BYTES)
		{
			pDBBindings[n].dwPart|=DBPART_LENGTH;
			ATLASSERT(pStartKeys[n].m_nLen!=OLEDB_STRING_END);
		}
		else if(pStartKeys[n].m_nDBType==DBTYPE_VARNUMERIC)
		{
			ATLASSERT(pStartKeys[n].m_nLen>=3);
			ATLASSERT(pStartKeys[n].m_nLen!=OLEDB_STRING_END);
			pDBBindings[n].dwPart|=DBPART_LENGTH;
			ATLASSERT(pStartKeys[n].m_nLen!=OLEDB_STRING_END);
		}
		else
		{
		}	
	}
	for(n=0; n<nEndKeys; n++)
	{
		pDBBindings[n+nStartKeys].dwMemOwner=DBMEMOWNER_CLIENTOWNED;
		pDBBindings[n+nStartKeys].eParamIO=DBPARAMIO_NOTPARAM;
		pDBBindings[n+nStartKeys].cbMaxLen=sizeof(void*);
		pDBBindings[n+nStartKeys].dwPart=DBPART_VALUE|DBPART_STATUS;
		ATLASSERT(pEndKeys[n].m_nCol);
		ATLASSERT(pEndKeys[n].m_nDBType>DBTYPE_NULL);	//make sure DBTYPE is correct
		ATLASSERT(pEndKeys[n].m_nDBType<DBTYPE_VECTOR);
		pDBBindings[n+nStartKeys].iOrdinal=pEndKeys[n].m_nCol;
		ATLASSERT((pEndKeys[n].m_nDBType & DBTYPE_BYREF)!=DBTYPE_BYREF);
		pDBBindings[n+nStartKeys].wType=pEndKeys[n].m_nDBType|DBTYPE_BYREF;
		pDBBindings[n+nStartKeys].obLength=n*sizeof(CKeyInfo)+offsetof(CKeyInfo, m_nLen);
		pDBBindings[n+nStartKeys].obValue=n*sizeof(CKeyInfo)+offsetof(CKeyInfo, m_pData);
		pDBBindings[n+nStartKeys].obStatus=n*sizeof(CKeyInfo)+offsetof(CKeyInfo, m_nDBStatus);
		pDBBindings[n+nStartKeys].bPrecision=pEndKeys[n].m_nPrecision;
		pDBBindings[n+nStartKeys].bScale=pEndKeys[n].m_nScale;
		ATLASSERT(pEndKeys[n].m_pData);
		pEndKeys[n].m_nDBStatus=DBSTATUS_S_OK;
		if(pEndKeys[n].m_nDBType==DBTYPE_STR)
		{
			pDBBindings[n+nStartKeys].dwPart|=DBPART_LENGTH;
			if(pEndKeys[n].m_nLen==OLEDB_STRING_END)
				pEndKeys[n].m_nLen=strlen((char*)(pEndKeys[n].m_pData));
		}
		else if(pEndKeys[n].m_nDBType==DBTYPE_WSTR)
		{
			pDBBindings[n+nStartKeys].dwPart|=DBPART_LENGTH;
			if(pEndKeys[n].m_nLen==OLEDB_STRING_END)
				pEndKeys[n].m_nLen=sizeof(WCHAR)*wcslen((wchar_t*)(pEndKeys[n].m_pData));
		}
		else if(pEndKeys[n].m_nDBType==DBTYPE_BYTES)
		{
			pDBBindings[n+nStartKeys].dwPart|=DBPART_LENGTH;
			ATLASSERT(pEndKeys[n].m_nLen!=OLEDB_STRING_END);
		}
		else if(pEndKeys[n].m_nDBType==DBTYPE_VARNUMERIC)
		{
			ATLASSERT(pEndKeys[n].m_nLen!=OLEDB_STRING_END);
			ATLASSERT(pEndKeys[n].m_nLen>=3);
			pDBBindings[n+nStartKeys].dwPart|=DBPART_LENGTH;
		}
		else
		{
		}	
	}
	HACCESSOR		hAccessor=0;
	m_hr=m_pIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, nTotalKeys, pDBBindings, sizeof(CKeyInfo)*nTotalKeys, &hAccessor, NULL);
	CoTaskMemFree(pDBBindings);
	if(FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
		return false;
	}
	ATLASSERT(hAccessor);

	m_hr=pIRowsetIndex->SetRange (hAccessor, nStartKeys, pStartKeys, nEndKeys, pEndKeys, nRangeOptions);
	m_pIAccessor->ReleaseAccessor(hAccessor, NULL);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIRowsetIndex, __uuidof(IRowsetIndex), &m_strError);
		return false;
	}
	return true;
}

bool	CRBaseEx::Seek(ULONG nKeys, CKeyInfo pKeyInfo[], DBSEEK nSeekOptions)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");

	CComPtr<IRowsetIndex>		pIRowsetIndex;
	m_hr=m_pIAccessor->QueryInterface(__uuidof(IRowsetIndex), (void**)&pIRowsetIndex);
	if(FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
		return false;
	}
	
	DBBINDING	*pDBBindings=(DBBINDING*)CoTaskMemAlloc(nKeys*sizeof(DBBINDING));
	ATLASSERT(pDBBindings);
	memset(pDBBindings, 0, nKeys*sizeof(DBBINDING));

	for(ULONG n=0; n<nKeys; n++)
	{
		pDBBindings[n].dwMemOwner=DBMEMOWNER_CLIENTOWNED;
		pDBBindings[n].eParamIO=DBPARAMIO_NOTPARAM;
		pDBBindings[n].dwPart=DBPART_VALUE|DBPART_STATUS;
		ATLASSERT(pKeyInfo[n].m_nCol);
		ATLASSERT(pKeyInfo[n].m_nDBType>DBTYPE_NULL);	//make sure DBTYPE is correct
		ATLASSERT(pKeyInfo[n].m_nDBType<DBTYPE_VECTOR);
		pDBBindings[n].cbMaxLen=sizeof(void*);
		pDBBindings[n].iOrdinal=pKeyInfo[n].m_nCol;
		ATLASSERT((pKeyInfo[n].m_nDBType & DBTYPE_BYREF)!=DBTYPE_BYREF);
		pDBBindings[n].wType=pKeyInfo[n].m_nDBType|DBTYPE_BYREF;
		pDBBindings[n].obLength=n*sizeof(CKeyInfo)+offsetof(CKeyInfo, m_nLen);
		pDBBindings[n].obValue=n*sizeof(CKeyInfo)+offsetof(CKeyInfo, m_pData);
		pDBBindings[n].obStatus=n*sizeof(CKeyInfo)+offsetof(CKeyInfo, m_nDBStatus);
		pDBBindings[n].bPrecision=pKeyInfo[n].m_nPrecision;
		pDBBindings[n].bScale=pKeyInfo[n].m_nScale;
		if(pKeyInfo[n].m_pData)
			pKeyInfo[n].m_nDBStatus=DBSTATUS_S_OK;
		else
			pKeyInfo[n].m_nDBStatus=DBSTATUS_S_ISNULL;
		if(pKeyInfo[n].m_nDBType==DBTYPE_STR)
		{
			pDBBindings[n].dwPart|=DBPART_LENGTH;
			if(pKeyInfo[n].m_nLen==OLEDB_STRING_END)
				pKeyInfo[n].m_nLen=strlen((char*)(pKeyInfo[n].m_pData));
		}
		else if(pKeyInfo[n].m_nDBType==DBTYPE_WSTR)
		{
			pDBBindings[n].dwPart|=DBPART_LENGTH;
			if(pKeyInfo[n].m_nLen==OLEDB_STRING_END)
				pKeyInfo[n].m_nLen=sizeof(WCHAR)*wcslen((wchar_t*)(pKeyInfo[n].m_pData));
		}
		else if(pKeyInfo[n].m_nDBType==DBTYPE_BYTES)
		{
			pDBBindings[n].dwPart|=DBPART_LENGTH;
			ATLASSERT(pKeyInfo[n].m_nLen!=OLEDB_STRING_END);
		}
		else if(pKeyInfo[n].m_nDBType==DBTYPE_VARNUMERIC)
		{
			ATLASSERT(pKeyInfo[n].m_nLen!=OLEDB_STRING_END);
			ATLASSERT(pKeyInfo[n].m_nLen>=3);
			pDBBindings[n].dwPart|=DBPART_LENGTH;
		}
		else
		{
		}
	}
	HACCESSOR		hAccessor=0;
	m_hr=m_pIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, nKeys, pDBBindings, 0, &hAccessor, NULL);
	CoTaskMemFree(pDBBindings);
	if(FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
		return false;
	}
	ATLASSERT(hAccessor);

	m_hr=pIRowsetIndex->Seek(hAccessor, nKeys, pKeyInfo, nSeekOptions);
	m_pIAccessor->ReleaseAccessor(hAccessor, NULL);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIRowsetIndex, __uuidof(IRowsetIndex), &m_strError);
		return false;
	}
	
	if(!m_pIRowset)
	{
		m_hr=m_pIAccessor->QueryInterface(__uuidof(IRowset), (void**)&m_pIRowset);
		if(FAILED(m_hr)) 
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
			return false;
		}
	}
	else
		ReleaseRows();

	m_nCurrentRow=0;
	m_hr=m_pIRowset->GetNextRows(m_hChapter, 0, m_nRowsetSize, &m_nRowsObtained, &m_pHRow);
	if(FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIRowset, __uuidof(IRowset), &m_strError);
		return false;
	}
	if(m_nRowsObtained)
		return GetDataFromServer(m_pHRow[m_nCurrentRow]);
	return true;
}

bool	CRBaseEx::FindNextRow(CFindInfo& FindInfo, LONG nSkip, const BYTE* pBookmark, ULONG nBookmarkLen)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	OleDBPro_ASSERT(FindInfo.m_pData, "\nFindInfo.m_pData can't be NULL!");	
	OleDBPro_ASSERT(FindInfo.m_nDBType>DBTYPE_NULL, "\nMake sure the data type is given!");
	OleDBPro_ASSERT((FindInfo.m_nDBType & DBTYPE_BYREF)!=DBTYPE_BYREF, "\nCurrently, can't be one of X|DBTYPE_BYREF!");
	OleDBPro_ASSERT(FindInfo.m_nCol && FindInfo.m_nCol<=m_nCol, "\nA column ordinal must be correct!");
	CComPtr<IRowsetFind>		pIRowsetFind;
	m_hr=m_pIAccessor->QueryInterface(__uuidof(IRowsetFind), (void**)&pIRowsetFind);
	if(FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
		return false;
	}
	if(!m_pIRowset)
	{
		m_hr=m_pIAccessor->QueryInterface(__uuidof(IRowset), (void**)&m_pIRowset);
		if(FAILED(m_hr)) 
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
			return false;
		}
	}
	else
		ReleaseRows();

	m_nCurrentRow=0;
	
	DBBINDING	DBBinding;
	memset(&DBBinding, 0, sizeof(DBBINDING));
	DBBinding.dwMemOwner=DBMEMOWNER_CLIENTOWNED;
	DBBinding.eParamIO=DBPARAMIO_NOTPARAM;
	DBBinding.dwPart=DBPART_VALUE|DBPART_STATUS;
	DBBinding.cbMaxLen=sizeof(void*);
	ATLASSERT(FindInfo.m_nCol);
	DBBinding.iOrdinal=FindInfo.m_nCol;
	DBBinding.wType=FindInfo.m_nDBType|DBTYPE_BYREF;
	DBBinding.obLength=offsetof(CFindInfo, m_nLen);
	DBBinding.obValue=offsetof(CFindInfo, m_pData);
	DBBinding.obStatus=offsetof(CFindInfo, m_nDBStatus);
	DBBinding.bPrecision=FindInfo.m_nPrecision;
	DBBinding.bScale=FindInfo.m_nScale;
	if(FindInfo.m_pData)
	{
		FindInfo.m_nDBStatus=DBSTATUS_S_OK;
		if(FindInfo.m_nDBType==DBTYPE_STR)
		{
			DBBinding.dwPart|=DBPART_LENGTH;
			if(FindInfo.m_nLen==OLEDB_STRING_END)
				FindInfo.m_nLen=strlen((char*)(FindInfo.m_pData));
		}
		else if(FindInfo.m_nDBType==DBTYPE_WSTR)
		{
			DBBinding.dwPart|=DBPART_LENGTH;
			if(FindInfo.m_nLen==OLEDB_STRING_END)
				FindInfo.m_nLen=sizeof(WCHAR)*wcslen((wchar_t*)(FindInfo.m_pData));
		}
		else if(FindInfo.m_nDBType==DBTYPE_BYTES)
		{
			DBBinding.dwPart|=DBPART_LENGTH;
			OleDBPro_ASSERT(FindInfo.m_nLen!=OLEDB_STRING_END, "\nMust set the length for a binary column!");
		}
		else if(FindInfo.m_nDBType==DBTYPE_VARNUMERIC)
		{
			OleDBPro_ASSERT(FindInfo.m_nLen!=OLEDB_STRING_END, "\nMust set the length for a varnumeric column!");
			OleDBPro_ASSERT(FindInfo.m_nLen>=3, "\nMust correctly set the length for a varnumeric column!");
			DBBinding.dwPart|=DBPART_LENGTH;
		}
		else
		{
		}
	}
	else
		FindInfo.m_nDBStatus=DBSTATUS_S_ISNULL;

	HACCESSOR		hAccessor=0;
	m_hr=m_pIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, 1, &DBBinding, sizeof(FindInfo), &hAccessor, NULL);
	if(FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
		return false;
	}
	ATLASSERT(hAccessor);
	if(!nBookmarkLen && pBookmark)
		nBookmarkLen=GetBookmarkLen();
	m_hr=pIRowsetFind->FindNextRow(m_hChapter, hAccessor, &FindInfo, FindInfo.m_nOp, nBookmarkLen, pBookmark, nSkip, m_nRowsetSize, &m_nRowsObtained, &m_pHRow);
	m_pIAccessor->ReleaseAccessor(hAccessor, NULL);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIRowsetFind, __uuidof(IRowsetFind), &m_strError);
		return false;
	}
	if(m_nRowsObtained)
		return GetDataFromServer(m_pHRow[m_nCurrentRow]);
	else
		return true;
}

bool CRBaseEx::GetRowsAtRatio (ULONG nNumerator, ULONG nDenominator)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	CComPtr<IRowsetScroll>	pIRowsetScroll;
	if(!m_pIRowset)
	{
		m_hr=m_pIAccessor->QueryInterface(__uuidof(IRowset), (void**)&m_pIRowset);
		if(FAILED(m_hr)) 
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
			return false;
		}
	}
	else
		ReleaseRows();
	m_hr=m_pIAccessor->QueryInterface(__uuidof(IRowsetScroll), (void**)&pIRowsetScroll);
	if(FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
		return false;
	}
	m_hr=pIRowsetScroll->GetRowsAtRatio(NULL, m_hChapter, nNumerator, nDenominator, m_nRowsetSize, &m_nRowsObtained, &m_pHRow);
	if(FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIRowsetScroll, __uuidof(IRowsetScroll), &m_strError);
		return false;
	}
	m_nCurrentRow=0;
	if(m_nRowsObtained)
		return GetDataFromServer(m_pHRow[m_nCurrentRow]);
	return true;
}

bool CRBaseEx::GetApproximatePosition(const BYTE *pBookmark, ULONG *pnPosition, ULONG *pnRows, ULONG nBookmarkLen)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that a rowset is available!");
	CComPtr<IRowsetScroll>	pIRowsetScroll;
	m_hr=m_pIAccessor->QueryInterface(__uuidof(IRowsetScroll), (void**)&pIRowsetScroll);
	if(FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
		return false;
	}
	if(!nBookmarkLen && pBookmark)
		nBookmarkLen=GetBookmarkLen();
	m_hr=pIRowsetScroll->GetApproximatePosition(m_hChapter, nBookmarkLen, pBookmark, pnPosition, pnRows);
	if(FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIRowsetScroll, __uuidof(IRowsetScroll), &m_strError);
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
