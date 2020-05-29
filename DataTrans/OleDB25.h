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

#ifndef	OLEDB25_TEMPLATES_H
#define	OLEDB25_TEMPLATES_H

#include <oledberr.h>
#include "OLEDBBas.h"
#include <atlbase.h>

/****************************** OleDBPro Template Classes **************************/
template <class TRBase=CRBase>
class COSchema : public TRBase
{
public:
	//IDBSchemaRowset
	virtual bool Open(IOpenRowset *pIOpenRowset, REFGUID guidSchema, ULONG nRestrictions, const VARIANT pvtRestrictions[], DBPROPSET *pPropSet=NULL)
	{
		ATLASSERT(pIOpenRowset);
		CComPtr<IDBSchemaRowset>	pIDBSchemaRowset;
		if(IsOpen()) 
			Close();
		m_hr=pIOpenRowset->QueryInterface(__uuidof(IDBSchemaRowset), (void**)&pIDBSchemaRowset);
		if(FAILED(m_hr))
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(pIOpenRowset, __uuidof(IOpenRowset), &m_strError);
			return false;
		}
		m_hr=pIDBSchemaRowset->GetRowset(NULL, guidSchema, nRestrictions, pvtRestrictions, GetBaseIID(), (pPropSet ? 1 : 0), pPropSet, GetBaseInterfacePPtr());
		if(FAILED(m_hr))
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(pIDBSchemaRowset, __uuidof(IDBSchemaRowset), &m_strError);
			return false;
		}
		return PostProcess();
	}
};

template<class TParamBase=CParamBase>
class CBatchParam : public TParamBase
{
public:
	virtual bool Open(IOpenRowset *pIOpenRowset, LPCOLESTR strSQL, CPInfo arrPInfo[], bool bPrepared=true)
	{
		ATLASSERT(strSQL);
		if(!OpenCmnd(pIOpenRowset)) 
			return false;

		m_hr=m_pICommandText->SetCommandText(DBGUID_DEFAULT, strSQL);
		if(FAILED(m_hr))
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pICommandText, __uuidof(ICommandText), &m_strError);
			CloseCmnd();
			return false;
		}
		m_nParams=ComputeNumOfParams(strSQL, L'?');

		if(!OpenParam(arrPInfo, bPrepared))
			return false;
		EnableBatch(true);
		return true;
	}
	virtual bool DoBatch(UINT nCount, CPData arrPData[])
	{
		ATLASSERT(HasCmnd());
		ATLASSERT(m_nParams);
		ATLASSERT((nCount%m_nParams)==0);
		ATLASSERT(m_nParamsBufferSize);
		ATLASSERT(m_hParamsAccessor);
		bool			bSuccess=false;
		DBPARAMS		DBParams;

		ULONG nRows=nCount/m_nParams;
		PrepareData(nCount, arrPData);

		DBParams.hAccessor=m_hParamsAccessor;
		if(m_bBatchable)
		{
			DBParams.pData=m_pParamsData;
			DBParams.cParamSets=nRows;
			m_hr=m_pICommandText->Execute(NULL, IID_NULL, &DBParams, &m_nRowsAffected, NULL);
			#ifdef _DEBUG
			{
				if(m_nRowsAffected!=(LONG)nRows) 
					AtlTrace("Warning: m_nRowsAffected!=nCount/m_nParams\n");
			}
			#endif
			if(FAILED(m_hr))
			{
				if(m_pGetErrorInfo) 
					m_pGetErrorInfo(m_pICommandText, __uuidof(ICommandText), &m_strError);
				return false;
			}
			else
				bSuccess=true;
		}
		else
		{
			DBParams.cParamSets=1;
			for(ULONG nIndex=0; nIndex<nRows; nIndex++)
			{
				DBParams.pData=m_pParamsData+nIndex*m_nParamsBufferSize;
				m_hr=m_pICommandText->Execute(NULL, IID_NULL, &DBParams, &m_nRowsAffected, NULL);
#ifdef _DEBUG
				{
					if(m_nRowsAffected!=1) 
						AtlTrace("Warning: m_nRowsAffected!=1\n");
				}
#endif
				if(FAILED(m_hr))
				{
					if(m_pGetErrorInfo) 
						m_pGetErrorInfo(m_pICommandText, __uuidof(ICommandText), &m_strError);
				}
				else
					bSuccess=true;
			}
		}
	
		if(m_nNumOutputParams)
			CopyToClient(nCount, arrPData);
		return bSuccess;
	}
	void EnableBatch(bool bBatchable=true)
	{
		ATLASSERT(HasCmnd());
		if(bBatchable)
		{
			VARIANT			vtData;
			memset(&vtData, 0, sizeof(VARIANT));
			COSession		OSession;
			CODataSource	ODataSource;
			GetSession(&OSession);
			if(OSession.IsOpen())
				OSession.GetDataSource(&ODataSource);
			if(ODataSource.IsOpen())
				ODataSource.GetProperty(DBPROPSET_DATASOURCEINFO, DBPROP_MULTIPLEPARAMSETS, &vtData);
			m_bBatchable=(vtData.boolVal ? true : false);
		}
		else
			m_bBatchable=bBatchable;
	}
};

template <class TRBase = CRBase>
class COTable : public TRBase
{
public:
	COTable()
	{
		Initialize();
	}
	virtual ~COTable()
	{
		if(IsOpen())
			Close();
	}

public:
	virtual void   Close()
	{
		ULONG nRef;
		if(m_pIRowsetChange)
			nRef=m_pIRowsetChange->Release();
		if(m_pIRowsetUpdate)
			m_pIRowsetUpdate->Release();
		Initialize();
		TRBase::Close();
	}
	virtual void	BeginUpdate()
	{
		ULONG		*pStatus;
		for(ULONG nCol=1; nCol<=m_nCol-IsBookmarked(); nCol++)
		{
			pStatus=GetStatusPtr(nCol);
			if(pStatus)
				*pStatus=DBSTATUS_S_IGNORE;
		}
	}
	virtual bool	IsReadOnly(){return m_pIRowsetChange ? false : true;}
	virtual bool	Delete(bool bAll=false)
	{
		ATLASSERT(m_nCurrentRow<m_nRowsObtained);
		ATLASSERT(m_pIRowsetChange);
		if(bAll)
		{
			m_hr=m_pIRowsetChange->DeleteRows(m_hChapter, m_nRowsObtained, m_pHRow, NULL);
			if(m_nRowsObtained)
				m_nCurrentRow=m_nRowsObtained-1;
			MoveNext();
		}
		else
			m_hr=m_pIRowsetChange->DeleteRows(m_hChapter, 1, &m_hCurrent, NULL);
		if(FAILED(m_hr))
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pIRowsetChange, __uuidof(IRowsetChange), &m_strError);
			return false;
		}
		return true;
	}
	virtual bool	UpdateBatch()
	{
		ATLASSERT(IsOpen());
		ATLASSERT(m_pIRowsetUpdate);
		m_hr=m_pIRowsetUpdate->Update(DB_NULL_HCHAPTER, 0, NULL, NULL, NULL, NULL);
		if(FAILED(m_hr)) 
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pIRowsetUpdate, __uuidof(IRowsetUpdate), &m_strError);
			return false;
		}
		return true;
	}
	virtual bool CancelBatch()
	{
		ATLASSERT(IsOpen());
		ATLASSERT(m_pIRowsetUpdate);
		m_hr=m_pIRowsetUpdate->Undo(DB_NULL_HCHAPTER, 0, NULL, NULL, NULL, NULL);
		if(FAILED(m_hr)) 
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pIRowsetUpdate, __uuidof(IRowsetUpdate), &m_strError);
			return false;
		}
		return true;
	}
	virtual bool	Update()
	{
		ATLASSERT(m_hWriteAccessor);
		ATLASSERT(m_pIRowsetChange);
		m_hr=m_pIRowsetChange->SetData(m_hCurrent, m_hWriteAccessor, m_pData);
		if(FAILED(m_hr))
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pIRowsetChange, __uuidof(IRowsetChange), &m_strError);
			return false;
		}
		return true;
	}
	virtual bool	Add(bool bNeedNewHandle=false)
	{
		ATLASSERT(m_hWriteAccessor);
		ATLASSERT(m_pIRowsetChange); 
		if(bNeedNewHandle)
			m_hr=m_pIRowsetChange->InsertRow(m_hChapter, m_hWriteAccessor, m_pData, &m_hCurrent);
		else
			m_hr=m_pIRowsetChange->InsertRow(m_hChapter, m_hWriteAccessor, m_pData, NULL);
		if(FAILED(m_hr))
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pIRowsetChange, __uuidof(IRowsetChange), &m_strError);
			return false;
		}
		if(bNeedNewHandle)
		{
			if(IsEOF())
			{
				m_nRowsObtained=1;
				m_nCurrentRow=0;
			}
			return GetDataFromServer(m_hCurrent);
		}
		return true;
	}
	bool	IsDelayUpdate()
   {
		return (m_pIRowsetUpdate ? true : false);
   }
	
protected:
	virtual bool QueryColsInfo()
	{
		if(!m_pIRowsetChange)
			m_hr=m_pIAccessor->QueryInterface(__uuidof(IRowsetChange), (void**)&m_pIRowsetChange);
		if(!m_pIRowsetUpdate)
			m_hr=m_pIAccessor->QueryInterface(__uuidof(IRowsetUpdate), (void**)&m_pIRowsetUpdate);
		return TRBase::QueryColsInfo();
	}
/*	virtual bool	ReleaseRows()
	{
		if(m_pIRowsetUpdate)
			UpdateBatch();
		return TRBase::ReleaseRows();
	}*/
private:
   void	Initialize()
   {
	   m_pIRowsetChange=NULL;;	
	   m_pIRowsetUpdate=NULL;
   }

public:

	//members
protected:
	IRowsetChange	*m_pIRowsetChange;	
	IRowsetUpdate	*m_pIRowsetUpdate;	
};

template <class TOTable, class TCmndBase = CCmndBase>
class CBulkRecord : public TOTable, public TCmndBase
{
public:
	virtual bool OpenWithCmnd(IOpenRowset *pIOpenRowset, LPCOLESTR strSelect, DBPROPSET *pDBPropSet=NULL, ULONG nPropSets=1, REFGUID guidDialect=DBGUID_DEFAULT)
	{
		ATLASSERT(pIOpenRowset);
		ATLASSERT(strSelect);
		if(IsOpen())
			Close();
		if(!TCmndBase::OpenCmnd(pIOpenRowset)) return false;
		m_hr=m_pICommandText->SetCommandText(guidDialect, strSelect);
		if(FAILED(m_hr))
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pICommandText, __uuidof(ICommandText), &m_strError);
			return false;
		}
		if(!ExecuteCmnd(GetBaseIID(), GetBaseInterfacePPtr(), NULL, pDBPropSet, nPropSets)) 
		{
			Close();
			return false;
		}
		if(*GetBaseInterfacePPtr())
			return PostProcess();
		return true;
	}
	virtual bool   GetSession(COSession *pOSession)
   {
		ATLASSERT(pOSession);
      if(m_pICommandText)
		return TCmndBase::GetSession(pOSession);
	  else if(TOTable::IsOpen() && TOTable::GetSession(pOSession))
		  return true;
	  else
		return false;
	  return false;
   }
	bool PostProcess()
	{
		ATLASSERT(*GetBaseInterfacePPtr());
		if(!TOTable::PostProcess())
			return false;
		if(!HasCmnd())
		{
			CComPtr<IRowsetInfo> pIRowsetInfo;
			(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IRowsetInfo), (void**)&pIRowsetInfo);
			if(pIRowsetInfo)
				pIRowsetInfo->GetSpecification(__uuidof(ICommandText), (IUnknown**)&m_pICommandText);
		}
		return true;
	}
protected:
	virtual bool	SetRowsetProps(DBPROPSET *pDBPropSet, ULONG nPropSets)
	{
		ATLASSERT(m_pICommandText);
		CComPtr<ICommandProperties>	pICommandProperties;
		m_hr=m_pICommandText->QueryInterface(__uuidof(ICommandProperties), (void**)&pICommandProperties);
		if(FAILED(m_hr))
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pICommandText, __uuidof(ICommandText), &m_strError);
			return false;
		}
		if(pDBPropSet)
			m_hr=pICommandProperties->SetProperties(nPropSets, pDBPropSet);
		if(m_hr!=S_OK)
		{
#ifdef _DEBUG
            ULONG nPropSet;
            ULONG nProp;
            AtlTrace(_T("Warning: Failed in setting properties. See the following outputs!\n"));
            for(nPropSet=0; nPropSet<nPropSets; nPropSet++)
            {
                for(nProp=0; nProp<pDBPropSet[nPropSet].cProperties; nProp++)
                {
                    if(pDBPropSet[nPropSet].rgProperties[nProp].dwStatus != DBPROPSTATUS_OK)
                        AtlTrace(_T("DBPropID = 0x%x;  DBPropStatus = %d.\n"), pDBPropSet[nPropSet].rgProperties[nProp].dwPropertyID, pDBPropSet[nPropSet].rgProperties[nProp].dwStatus);
                }
            }
#endif      
            if(FAILED(m_hr) && m_pGetErrorInfo) 
				m_pGetErrorInfo(pICommandProperties, __uuidof(ICommandProperties), &m_strError);
		}
		return true;
	}
	virtual bool	ExecuteCmnd(REFIID iidResult, IUnknown** ppIResult, DBPARAMS *pParams=NULL, DBPROPSET *pDBPropSet=NULL, ULONG nPropSets=1)
	{
		if( pDBPropSet && (!SetRowsetProps(pDBPropSet, nPropSets)))
			return false;
		m_hr=m_pICommandText->Execute(NULL, iidResult, pParams, &m_nRowsAffected, ppIResult);
		if(FAILED(m_hr))
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pICommandText, __uuidof(ICommandText), &m_strError);
			return false;
		}
		else if(*ppIResult==NULL)
		{
			if(m_pGetErrorInfo) 
			{
				if(m_strError)
					::SysFreeString(m_strError);
				m_strError=::SysAllocString(L"No target OLEDB object available.");
			}
			return false;
		}
		return true;
	}
};

template <class TOTable>
class CFRowset : public CBulkRecord<TOTable, CCmndBase>
{
public:
	virtual bool OpenWithCmnd(IOpenRowset *pIOpenRowset, LPCOLESTR strMDX, DBPROPSET *pDBPropSet=NULL, ULONG nPropSets=1, REFGUID guidDialect=MDGUID_MDX)
	{
		ATLASSERT(pIOpenRowset);
		ATLASSERT(strMDX);
		if(IsOpen())
			Close();
		if(!CCmndBase::OpenCmnd(pIOpenRowset)) return false;
		m_hr=m_pICommandText->SetCommandText(guidDialect, strMDX);
		if(FAILED(m_hr))
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pICommandText, __uuidof(ICommandText), &m_strError);
			return false;
		}
		if(!ExecuteCmnd(GetBaseIID(), GetBaseInterfacePPtr(), NULL, pDBPropSet, nPropSets)) 
		{
			Close();
			return false;
		}
		return PostProcess();
	}
protected:
	virtual bool	SetRowsetProps(DBPROPSET *pDBPropSet, ULONG nPropSets)
	{
		ATLASSERT(m_pICommandText);
		COPropSet	OPropSet(DBPROPSET_ROWSET);
		CComPtr<ICommandProperties>	pICommandProperties;
		m_hr=m_pICommandText->QueryInterface(__uuidof(ICommandProperties), (void**)&pICommandProperties);
		if(FAILED(m_hr))
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pICommandText, __uuidof(ICommandText), &m_strError);
			return false;
		}
		if(pDBPropSet)
			OPropSet=*((COPropSet*)pDBPropSet);
		OPropSet.AddProperty(DBPROP_IRowset, true);
		m_hr=pICommandProperties->SetProperties(1, &OPropSet);
		if(FAILED(m_hr))
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(pICommandProperties, __uuidof(ICommandProperties), &m_strError);
			return false;
		}
		return true;
	}
};

template <class TOTable, class TParamBase=CParamBase>
class CBulkRecordParam : public CBulkRecord<TOTable, TParamBase>
{
public:
   CBulkRecordParam()
   {
		m_pPData=NULL;
   }
   virtual ~CBulkRecordParam()
   {
		if(IsOpen())
			Close();
   }
public:
	virtual void	Close()
	{
		CBulkRecord<TOTable, TParamBase>::Close();
		if(m_pPData && (!IsOpen()))
		{
			if(m_nNumOutputParams)
				CopyToClient(m_nParams, m_pPData);
			CoTaskMemFree(m_pPData);
			m_pPData=NULL;
		}
	}
   virtual bool	Open(IOpenRowset *pIOpenRowset, LPCOLESTR strSelect, CPInfoEx *pPInfoEx=NULL, DBPROPSET *pDBPropSet=NULL, ULONG nPropSets=1, REFGUID guidDialect=DBGUID_DEFAULT, WCHAR cParam=L'?', bool bPrepared=true)
	{
		ATLASSERT(pIOpenRowset);
		ATLASSERT(strSelect);
		if(IsOpen())
			Close();
		if(!TParamBase::OpenCmnd(pIOpenRowset)) 
			return false;
		m_hr=m_pICommandText->SetCommandText(guidDialect, strSelect);
		if(FAILED(m_hr))
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pICommandText, __uuidof(ICommandText), &m_strError);
			CloseCmnd();
			return false;
		}
		m_nParams=ComputeNumOfParams(strSelect, cParam);
		if(pPInfoEx && m_nParams)
		{
			m_pPData=(CPData*)CoTaskMemAlloc(m_nParams*sizeof(CPData));
			ATLASSERT(m_pPData);
			CPInfo	*pPInfo=(CPInfo*)CoTaskMemAlloc(m_nParams*sizeof(CPInfo));
			ATLASSERT(pPInfo);
			for(ULONG n=0; n<m_nParams; n++)
			{
				pPInfo[n].m_nDBParamIO=pPInfoEx[n].m_nDBParamIO;
				pPInfo[n].m_nDBType=pPInfoEx[n].m_nDBType;
				pPInfo[n].m_nLen=pPInfoEx[n].m_nLen;
				pPInfo[n].m_strPName=pPInfoEx[n].m_strPName;
				pPInfo[n].m_nPrecision=pPInfoEx[n].m_nPrecision;
				pPInfo[n].m_nScale=pPInfoEx[n].m_nScale;
				m_pPData[n].m_pData=pPInfoEx[n].m_pData;
				m_pPData[n].m_nLen=pPInfoEx[n].m_nLen;
			}
			if(!OpenParam(pPInfo, bPrepared))
			{
				CoTaskMemFree(pPInfo);
				return false;
			}
			CoTaskMemFree(pPInfo);
			PrepareData(m_nParams, m_pPData); 
			ATLASSERT(m_pParamsData);
			ATLASSERT(m_hParamsAccessor);
			DBPARAMS DBParams;
			DBParams.pData=m_pParamsData;
			DBParams.hAccessor=m_hParamsAccessor;
			DBParams.cParamSets=1;
			if(!ExecuteCmnd(GetBaseIID(), GetBaseInterfacePPtr(), &DBParams, pDBPropSet, nPropSets)) 
			{
				Close();
				return false;
			}
			if(m_nNumOutputParams)
			{
				VARIANT	vt;
				COSession		OSession;
				CODataSource	ODataSource;
				if(GetSession(&OSession))
					OSession.GetDataSource(&ODataSource);
				memset(&vt, 0, sizeof(VARIANT));
				if(ODataSource.IsOpen())
					ODataSource.GetProperty(DBPROPSET_DATASOURCEINFO, DBPROP_OUTPUTPARAMETERAVAILABILITY, &vt);
				if((vt.lVal & DBPROPVAL_OA_ATEXECUTE) == DBPROPVAL_OA_ATEXECUTE)
				{	
					CopyToClient(m_nParams, m_pPData);
					CoTaskMemFree(m_pPData);
					m_pPData=NULL;
				}
			}
			return PostProcess();
		}
		return CBulkRecord<TOTable, TParamBase>::OpenWithCmnd(pIOpenRowset, strSelect, pDBPropSet, nPropSets, guidDialect);
	}
	virtual bool	Open(LPCOLESTR strSelect, IOpenRowset *pIOpenRowset, CPInfoEx *pPInfoEx=NULL, int nCursor=Static, bool bReadOnly=false, bool bBatch=false, bool bScrollback=false, bool bBookmark=false, const DBPROPSET *pPropSetEx=NULL, bool bPrepared=true)
	{
		COPropSet		DBPropSet(DBPROPSET_ROWSET);
		PrepareProps(DBPropSet, nCursor, bReadOnly, bBatch, bScrollback, bBookmark);
		if(pPropSetEx) 
		{
			for(ULONG nIndex=0; nIndex<pPropSetEx->cProperties; nIndex++)
				DBPropSet.AddProperty(pPropSetEx->rgProperties[nIndex].dwPropertyID, pPropSetEx->rgProperties[nIndex].vValue);
		}
		return Open(pIOpenRowset, strSelect, pPInfoEx, &DBPropSet);	
	}
   virtual bool	GetBulkRecord(CPData pPData[])
   {
      ATLASSERT(m_pICommandText); 
      ATLASSERT(m_hParamsAccessor);
      ATLASSERT(m_nParams);
      ATLASSERT(pPData);
      if(IsOpen())
			Close();
		m_pPData=(CPData*)CoTaskMemRealloc(m_pPData, m_nParams*sizeof(CPData));
		ATLASSERT(m_pPData);
		PrepareData(m_nParams, pPData); 
		memcpy(m_pPData, pPData, m_nParams*sizeof(CPData));
      ATLASSERT(m_pParamsData);
      DBPARAMS DBParams;
      DBParams.pData=m_pParamsData;
      DBParams.hAccessor=m_hParamsAccessor;
      DBParams.cParamSets=1;
	  if(!ExecuteCmnd(GetBaseIID(), GetBaseInterfacePPtr(), &DBParams, NULL) && FAILED(m_hr))
      {
         return false;
      }
		if(m_nNumOutputParams)
		{
			VARIANT	vt;
			COSession		OSession;
			CODataSource	ODataSource;
			if(GetSession(&OSession))
				OSession.GetDataSource(&ODataSource);
			memset(&vt, 0, sizeof(VARIANT));
			if(ODataSource.IsOpen())
				ODataSource.GetProperty(DBPROPSET_DATASOURCEINFO, DBPROP_OUTPUTPARAMETERAVAILABILITY, &vt);
			if((vt.lVal & DBPROPVAL_OA_ATEXECUTE) == DBPROPVAL_OA_ATEXECUTE)
			{	
				CopyToClient(m_nParams, m_pPData);
				CoTaskMemFree(m_pPData);
				m_pPData=NULL;
			}
		}
		if(*GetBaseInterfacePPtr()==NULL)
		{
			if(m_pGetErrorInfo) 
			{
				if(m_strError)
					::SysFreeString(m_strError);
				m_strError=::SysAllocString(L"No OLEDB object available");
			}
			return false;
		}
		return PostProcess();
   }
protected:
	CPData		*m_pPData;
};

template <class TBulkRecord>
class CMultiBulkRecord : public TBulkRecord
{
public:
   CMultiBulkRecord()
   {
	   m_pIMultipleResults=NULL;
   }
   virtual ~CMultiBulkRecord()
   {
	   if(IsOpen())
		   Close();
   }
public:
   virtual void   Close()
   {
      if(m_pIMultipleResults) 
      {
         m_pIMultipleResults->Release();
         m_pIMultipleResults=NULL;
      }
      TBulkRecord::Close();
   }
   virtual bool   IsOpen()
   {
      return (m_pIMultipleResults||TBulkRecord::IsOpen());
   }
	virtual bool	IsRowsetAvailable()
	{
		return TBulkRecord::IsOpen();
	}
   virtual bool	GetNextRowset()
	{
		CleanRecord();
		if(m_pIMultipleResults)
		{
			m_hr=m_pIMultipleResults->GetResult(NULL, NULL, GetBaseIID(), &m_nRowsAffected, GetBaseInterfacePPtr());
			if(m_hr==DB_S_NORESULT)
			{
				Close();
				if(m_strError)
					::SysFreeString(m_strError);
				m_strError=::SysAllocString(L"No more rowset available.");
				return false;
			}
			else if(m_pIAccessor)
				return PostProcess();
			else
			{
				if(FAILED(m_hr))
				{
					if(m_pGetErrorInfo) 
						m_pGetErrorInfo(m_pIMultipleResults, __uuidof(IMultipleResults), &m_strError);
					return false;
				}
				return true;
			}
		}
		else
			Close();
		return false;
	}
	virtual ULONG  IsStatementBatch()
	{
/*		ULONG		nCols=0;
		ATLASSERT(m_pICommandText);
		LPOLESTR		strSQL=NULL;
		LPOLESTR		p=NULL;
		GUID			guid;
		m_hr=m_pICommandText->GetCommandText(&guid, &strSQL);
		p=strSQL;
		while(p && *p)
		{
			if(*p==L';') 
				nCols++;
			p++;
		}
		if(strSQL) 
			CoTaskMemFree(strSQL);
		return nCols;*/
		return 1;
	}
protected:
	virtual bool	ExecuteCmnd(REFIID iidResult, IUnknown** ppIResult, DBPARAMS *pParams=NULL, DBPROPSET *pDBPropSet=NULL, ULONG nPropSets=1)
	{
		if(IsStatementBatch())
		{
			VARIANT			vtMulti;
			memset(&vtMulti, 0, sizeof(VARIANT));
			COSession		OSession;
			CODataSource	ODataSource;
			if(GetSession(&OSession))
				OSession.GetDataSource(&ODataSource);
			if(ODataSource.IsOpen())
				ODataSource.GetProperty(DBPROPSET_DATASOURCEINFO, DBPROP_MULTIPLERESULTS, &vtMulti);
			if((vtMulti.lVal & DBPROPVAL_MR_SUPPORTED)==DBPROPVAL_MR_SUPPORTED)
			{
/*				COPropSet	DBPropSet(DBPROPSET_ROWSET);
				DBPropSet.AddProperty(DBPROP_IMultipleResults, true);
				if(pDBPropSet) 
				{
					for(ULONG nIndex=0; nIndex<pDBPropSet->cProperties; nIndex++)
						DBPropSet.AddProperty(pDBPropSet->rgProperties[nIndex].dwPropertyID, pDBPropSet->rgProperties[nIndex].vValue);
				}*/
				if(!SetRowsetProps(pDBPropSet, nPropSets))
					return false;
				m_hr=m_pICommandText->Execute(NULL, __uuidof(IMultipleResults), pParams, &m_nRowsAffected, (IUnknown**)&m_pIMultipleResults);
				if(FAILED(m_hr))
				{
					if(m_pGetErrorInfo) 
						m_pGetErrorInfo(m_pICommandText, __uuidof(ICommandText), &m_strError);
					return false;
				}
				m_hr=m_pIMultipleResults->GetResult(NULL, NULL, iidResult, &m_nRowsAffected, ppIResult);
				if(FAILED(m_hr))
				{
					if(m_pGetErrorInfo) 
						m_pGetErrorInfo(m_pIMultipleResults, __uuidof(IMultipleResults), &m_strError);
					Close();
					return false;
				}
				else if(*ppIResult==NULL)
				{
					if(m_pGetErrorInfo) 
					{
						if(m_strError)
							::SysFreeString(m_strError);
						m_strError=::SysAllocString(L"No target OLEDB object available.");
					}
					return true;
				}
				return true;
			}
#ifdef _DEBUG
			ATLTRACE("Open only one resultset at a time because this provider doesn't support interface IMultipleResults!\n");
#endif
			return TBulkRecord::ExecuteCmnd(iidResult, ppIResult, pParams, pDBPropSet, nPropSets);
		}
		return TBulkRecord::ExecuteCmnd(iidResult, ppIResult, pParams, pDBPropSet, nPropSets);
	}
	virtual void	CleanRecord()
	{
		TBulkRecord::Close();
	}

protected:
   IMultipleResults	*m_pIMultipleResults;	
};

template <class TRBase=CRBase, class TCmndBase=CCmndBase>
class CChapterRecord : public CBulkRecord<TRBase, TCmndBase> 
{
public:
	CChapterRecord()
	{
		m_pChildRecord=NULL;
	}

public:
	virtual bool GetReferencedRowset(ULONG nColChapter=-1)
	{
		ATLASSERT(m_pIRowset);
		ATLASSERT(m_hCurrent);
		ATLASSERT(m_pIAccessor);
		ATLASSERT(m_pChildRecord);
		ATLASSERT(nColChapter>=1);
		if(nColChapter==-1)
			nColChapter=HaveChapter();
		if(!nColChapter) return false;
		if(!m_pChildRecord->IsOpen())
		{
			CComPtr<IRowsetInfo> pIRowsetInfo;
			m_hr=m_pIAccessor->QueryInterface(__uuidof(IRowsetInfo), (void**)&pIRowsetInfo);
			if(FAILED(m_hr)||pIRowsetInfo==NULL)
			{
				if(m_pGetErrorInfo) 
					m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
				return false;
			}
			
			m_hr=pIRowsetInfo->GetReferencedRowset(nColChapter, m_pChildRecord->GetBaseIID(), m_pChildRecord->GetBaseInterfacePPtr());
			if(FAILED(m_hr)||*(m_pChildRecord->GetBaseInterfacePPtr())==NULL)
			{
				if(m_pGetErrorInfo) 
					m_pGetErrorInfo(pIRowsetInfo, __uuidof(IRowsetInfo), &m_strError);
				return false;
			}
			if(!m_pChildRecord->PostProcess()) 
				return false;
		}
		HCHAPTER	*pHChapter = (HCHAPTER*)GetData(nColChapter);
		m_pChildRecord->SetChapter(*pHChapter);
		*pHChapter = NULL;
		return true;
	}
	virtual bool GetChildRecords(HCHAPTER hChapter=0, ULONG nColChapter=-1)
	{
		ATLASSERT(m_pIAccessor);
		ATLASSERT(m_pChildRecord);
		ATLASSERT(nColChapter>=1);
		if(nColChapter==-1)
			nColChapter=HaveChapter();
		if(!nColChapter) return false;
		if(!m_pChildRecord->IsOpen())
		{
			CComPtr<IParentRowset> pIParentRowset;
			m_hr=m_pIAccessor->QueryInterface(__uuidof(IParentRowset), (void**)&pIParentRowset);
			if(FAILED(m_hr)||pIParentRowset==NULL)
			{
				if(m_pGetErrorInfo) 
					m_pGetErrorInfo(m_pIAccessor, __uuidof(IAccessor), &m_strError);
				return false;
			}
			
			m_hr=pIParentRowset->GetChildRowset(NULL, nColChapter, m_pChildRecord->GetBaseIID(), m_pChildRecord->GetBaseInterfacePPtr());
			if(FAILED(m_hr)||*(m_pChildRecord->GetBaseInterfacePPtr())==NULL)
			{
				if(m_pGetErrorInfo) 
					m_pGetErrorInfo(pIParentRowset, __uuidof(IParentRowset), &m_strError);
				return false;
			}
			else
				m_pIAccessor->AddRef();
			if(!m_pChildRecord->PostProcess()) 
				return false;
		}
		if(hChapter)
			m_pChildRecord->SetChapter(hChapter);
		else
		{
			HCHAPTER *pHChapter = (HCHAPTER*)GetData(nColChapter);
			m_pChildRecord->SetChapter(*pHChapter);
			*pHChapter = NULL;
		}
		return true;
	}

	ULONG	HaveChapter()
	{
		ULONG nCols=m_nCol-IsBookmarked();
		for(ULONG n=1; n<=nCols; n++)
		{
			if(IsChapter(n)) 
				return n;
		}
		return 0;
	}

public:
	CRBase		*m_pChildRecord;
};

template <class T>
class CAsyHandler : public T
{
public:
	CAsyHandler(){m_pIDBAsynchStatus=NULL;}
	virtual ~CAsyHandler()
	{
		if(IsOpen())
			Close();
	}

public:
	virtual bool	IsOpen(){return (T::IsOpen() || m_pIDBAsynchStatus) ? true : false;}
	virtual REFIID	GetBaseIID(){return __uuidof(IDBAsynchStatus);}
	virtual IUnknown**	GetBaseInterfacePPtr(){return ((IUnknown**)&m_pIDBAsynchStatus);}
	virtual void	Close()
	{
		if(m_pIDBAsynchStatus)
		{
			m_pIDBAsynchStatus->Release();
			m_pIDBAsynchStatus=NULL;
		}
		T::Close();
	}
	virtual bool AsyInit()
	{
		ATLASSERT(m_pIDBAsynchStatus);
		CComPtr<IDBInitialize> pIDBInitialize;
		m_pIDBAsynchStatus->QueryInterface(__uuidof(IDBInitialize), (void**)&pIDBInitialize);
		if(pIDBInitialize)
		{
			Initializing();
			m_hr=pIDBInitialize->Initialize();
		}
		return true;
	}
	virtual bool PostProcess()
	{
		return true;
	}
	virtual bool AfterInit()
	{
		if(*(T::GetBaseInterfacePPtr()))
		{
			(*(T::GetBaseInterfacePPtr()))->Release();
			*(T::GetBaseInterfacePPtr())=NULL;
		}
		m_hr=m_pIDBAsynchStatus->QueryInterface(T::GetBaseIID(), (void**)(T::GetBaseInterfacePPtr()));
		if(FAILED(m_hr))
		{
			if(m_pGetErrorInfo) 
				m_pGetErrorInfo(m_pIDBAsynchStatus, __uuidof(IDBAsynchStatus), &m_strError);
			Close();
			return false;
		}
		if(T::GetRefGuid()==DBGUID_DSO)
			return true;
		return T::PostProcess();
	}

	virtual bool GetStatus(ULONG *pnAsynchPhase=NULL, ULONG *pnProgress=NULL, ULONG *pnProgressMax=NULL, ULONG nOperation=DBASYNCHOP_OPEN, LPOLESTR *pStrStatusText=NULL, HCHAPTER hChapter=DB_NULL_HCHAPTER)
	{
		ATLASSERT(m_pIDBAsynchStatus);
		m_hr=m_pIDBAsynchStatus->GetStatus(hChapter, nOperation, pnProgress, pnProgressMax, pnAsynchPhase, pStrStatusText);
		if(FAILED(m_hr))
		{
			if(m_pGetErrorInfo) 
			{
				if(m_strError)
					::SysFreeString(m_strError);
				m_strError=::SysAllocString(L"Failed in returning the status of an asynchronously executing operation!");	
			}
			return false;
		}
		return true;
	}

	virtual bool Abort(ULONG nOperation=DBASYNCHOP_OPEN, HCHAPTER hChapter=DB_NULL_HCHAPTER)
	{
		ATLASSERT(m_pIDBAsynchStatus);
		m_hr=m_pIDBAsynchStatus->Abort(hChapter, nOperation);
		if(FAILED(m_hr))
		{
			if(m_pGetErrorInfo) 
			{
				if(m_strError)
					::SysFreeString(m_strError);
				m_strError=::SysAllocString(L"Failed in canceling an asynchronously executing operation!");	
			}
			return false;
		}
		return true;
	}
protected:
	virtual void	Initializing(){}

protected:
	IDBAsynchStatus	*m_pIDBAsynchStatus;
};


template <class TOleDBProClass, class INotifyImp>
class CTNotification : public TOleDBProClass
{
public:
	CTNotification()
	{
		m_pINotifyImp=NULL;
		m_pIConnectionPointContainer=NULL;	
	}
	virtual ~CTNotification()
	{
		if(IsOpen())
			Close();
	}

public:
	virtual bool	IsOpen(){return (TOleDBProClass::IsOpen() || m_pIConnectionPointContainer) ? true : false;}
	virtual void	Close()
	{
		if(m_pINotifyImp)
		{
			if(m_pIConnectionPointContainer)
			{
				Unadvising();
				m_pINotifyImp->Unadvise(m_pIConnectionPointContainer);
			}
//			m_pINotifyImp->Release();
//			m_pINotifyImp=NULL;
		}
		if(m_pIConnectionPointContainer)
		{
			m_pIConnectionPointContainer->Release();
			m_pIConnectionPointContainer=NULL;
		}
		TOleDBProClass::Close();
	}
	virtual bool	PostProcess()
	{
		ATLASSERT(*GetBaseInterfacePPtr());
		if(m_pIConnectionPointContainer)
		{
			m_pIConnectionPointContainer->Release();
			m_pIConnectionPointContainer=NULL;
		}
		m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&m_pIConnectionPointContainer);
		if(FAILED(m_hr))
		{
			if(m_pGetErrorInfo) 
			{
				if(m_strError)
					::SysFreeString(m_strError);
				m_strError=::SysAllocString(L"Failed in retrieving interface IConnectionPointContainer!");	
			}
			Close();
			return false;
		}
		else
		{
			if(m_pINotifyImp)
			{
				Advising();
//				m_pINotifyImp->AddRef();
				m_hr=m_pINotifyImp->Advise(m_pIConnectionPointContainer);
			}
		}
		return TOleDBProClass::PostProcess();
	}

public:
	INotifyImp		*m_pINotifyImp;

protected:
	virtual void Advising(){}
	virtual void Unadvising(){}

protected:
	IConnectionPointContainer	*m_pIConnectionPointContainer;
};

/**************** Global Template Functions *******************/
//IRow for TRow, TStream or TRowset
template<class T>
bool	Open(CORow *pORow, T& Object, DBID dbidCol=DBROWCOL_DEFAULTSTREAM)
{
	ATLASSERT(pORow);
	ATLASSERT(pORow->IsOpen());
	if(Object.IsOpen())
		Object.Close();
	Object.m_hr=((IRow*)(*(pORow->GetBaseInterfacePPtr())))->Open(NULL, &dbidCol, Object.GetRefGuid(), 0, Object.GetBaseIID(),  Object.GetBaseInterfacePPtr());
	if(FAILED(Object.m_hr))
	{
		if(Object.m_pGetErrorInfo) 
			Object.m_pGetErrorInfo(*(pORow->GetBaseInterfacePPtr()), pORow->GetBaseIID(), &(Object.m_strError));
		return false;
	}
	return Object.PostProcess();
}

//IRow for TRow, TStream or TRowset
template<class T>
bool	Open(CORow *pORow, T& Object, LPOLESTR strColName)
{
	ATLASSERT(strColName);
	ATLASSERT(wcslen(strColName));
	DBID	dbidCol;
	memset(&dbidCol, 0, sizeof(DBID));
	dbidCol.eKind=DBKIND_NAME;
	dbidCol.uName.pwszName=strColName;
	return Open(pORow, Object, dbidCol);
}

//IOpenRowset for TRowset, TRow
template <class T>
bool OpenObject(IOpenRowset *pIOpenRowset, T& Object, LPOLESTR strTblName, LPOLESTR strIdxName=NULL, DBPROPSET* pDBPropSet=NULL, ULONG nPropSets=1)
{
	ATLASSERT(pIOpenRowset);
	if(Object.IsOpen()) 
		Object.Close();
	DBID		dbidTbl;
	DBID		dbidIdx;

	memset(&dbidTbl, 0, sizeof(DBID));
	dbidTbl.eKind=DBKIND_NAME;
	dbidTbl.uName.pwszName=strTblName;
	
	memset(&dbidIdx, 0, sizeof(DBID));
	dbidIdx.eKind=DBKIND_NAME;
	dbidIdx.uName.pwszName=strIdxName;
	
	Object.m_hr=pIOpenRowset->OpenRowset(NULL, (strTblName ? &dbidTbl : NULL), (strIdxName ? &dbidIdx : NULL), Object.GetBaseIID(), (pDBPropSet ? nPropSets : 0), pDBPropSet, Object.GetBaseInterfacePPtr());
	
	if(FAILED(Object.m_hr)||*(Object.GetBaseInterfacePPtr())==NULL)
	{
		if(Object.m_pGetErrorInfo) 
			Object.m_pGetErrorInfo(pIOpenRowset, __uuidof(IOpenRowset), &(Object.m_strError));
		return false;
	}
	return Object.PostProcess();
}

//IOpenRowset for TRowset or TRow
template <class T>
bool	OpenObject(IOpenRowset *pIOpenRowset, LPOLESTR strTblName, T& Object, LPOLESTR strIdxName=NULL, int nCursor=Static, bool bReadOnly=false, bool bBatch=false, bool bScrollback=false, bool bBookmark=false, const DBPROPSET* pPropSetEx=NULL)
{
	COPropSet		DBPropSet;
	PrepareProps(DBPropSet, nCursor, bReadOnly, bBatch, bScrollback, bBookmark);
	if(pPropSetEx) 
	{
		for(ULONG nIndex=0; nIndex<pPropSetEx->cProperties; nIndex++)
			DBPropSet.AddProperty(pPropSetEx->rgProperties[nIndex].dwPropertyID, pPropSetEx->rgProperties[nIndex].vValue);
	}
	return OpenObject(pIOpenRowset, Object, strTblName, strIdxName, &DBPropSet);
}

//IOpenRowset + ICommandText for TRowset, TRow or TDataset
template <class T>
bool OpenWithCmnd(IOpenRowset *pIOpenRowset, T& Object, LPCOLESTR strSelect, int nCursor=Static, bool bReadOnly=false, bool bBatch=false, bool bScrollback=false, bool bBookmark=false, const COPropSet *pOPropSetEx=NULL)
{
	COPropSet	OPropSet;
	if(pOPropSetEx)
		OPropSet=*pOPropSetEx;
	PrepareProps(OPropSet, nCursor, bReadOnly, bBatch, bScrollback, bBookmark);
	return Object.OpenWithCmnd(pIOpenRowset, strSelect, &OPropSet);
}

//IBindResource for TDataSource, TSession, TRow, TRowset or TStream
template <class TObject, class TObjectBinded>
bool	BindObject(TObject& Object, TObjectBinded& ObjectBinded, LPCOLESTR strURL, DBBINDURLFLAG nBindURLFlags=DBBINDURLFLAG_READ, DBIMPLICITSESSION *pImplSession=NULL, DBBINDURLSTATUS *pnBindStatus=NULL, IAuthenticate *pIAuthenticate=NULL)
{
	ATLASSERT(Object.IsOpen());
	ATLASSERT(*(Object.GetBaseInterfacePPtr()));
	if(ObjectBinded.IsOpen())
		ObjectBinded.Close();
	CComPtr<IBindResource>	pIBindResource;
	ObjectBinded.m_hr=(*(Object.GetBaseInterfacePPtr()))->QueryInterface(__uuidof(IBindResource), (void**)&pIBindResource);
	if(FAILED(ObjectBinded.m_hr))
	{
		if(ObjectBinded.m_pGetErrorInfo) 
			ObjectBinded.m_pGetErrorInfo(*(Object.GetBaseInterfacePPtr()), Object.GetBaseIID(), &(ObjectBinded.m_strError));
		return false;
	}
	ObjectBinded.m_hr=pIBindResource->Bind(NULL, strURL, nBindURLFlags, ObjectBinded.GetRefGuid(), ObjectBinded.GetBaseIID(), pIAuthenticate, pImplSession, pnBindStatus, ObjectBinded.GetBaseInterfacePPtr());
	if(FAILED(ObjectBinded.m_hr))
	{
		if(ObjectBinded.m_pGetErrorInfo) 
			ObjectBinded.m_pGetErrorInfo(pIBindResource, __uuidof(IBindResource), &(ObjectBinded.m_strError));
		return false;
	}
	if(ObjectBinded.GetRefGuid()==CODataSource::GetRefGuid())
		return true;
	return ObjectBinded.PostProcess();
}

//ICreateRow for TRow, TStream or TRowset
template <class TObject, class TObjectCreated>
bool	CreateObject(TObject& Object, TObjectCreated& ObjectCreated, LPCOLESTR strURL, LPOLESTR *pStringNewURL=NULL, DBBINDURLFLAG nBindURLFlags=DBBINDURLFLAG_READWRITE, DBIMPLICITSESSION *pImplSession=NULL, DBBINDURLSTATUS *pnBindStatus=NULL, IAuthenticate *pIAuthenticate=NULL)
{
	ATLASSERT(Object.IsOpen());
	ATLASSERT(*(Object.GetBaseInterfacePPtr()));

	if(ObjectCreated.IsOpen())
		ObjectCreated.Close();
	CComPtr<ICreateRow>	pICreateRow;
	ObjectCreated.m_hr=(*(Object.GetBaseInterfacePPtr()))->QueryInterface(__uuidof(ICreateRow), (void**)&pICreateRow);
	if(FAILED(ObjectCreated.m_hr))
	{
		if(ObjectCreated.m_pGetErrorInfo) 
			ObjectCreated.m_pGetErrorInfo(*(Object.GetBaseInterfacePPtr()), Object.GetBaseIID(), &(ObjectCreated.m_strError));
		return false;
	}
	
	ObjectCreated.m_hr=pICreateRow->CreateRow(NULL, strURL, nBindURLFlags, ObjectCreated.GetRefGuid(), ObjectCreated.GetBaseIID(), pIAuthenticate, pImplSession, pnBindStatus, pStringNewURL, ObjectCreated.GetBaseInterfacePPtr());
	if(FAILED(ObjectCreated.m_hr))
	{
		if(ObjectCreated.m_pGetErrorInfo) 
			ObjectCreated.m_pGetErrorInfo(pICreateRow, __uuidof(ICreateRow), &(ObjectCreated.m_strError));
		return false;
	}
	if(ObjectCreated.GetRefGuid()==CODataSource::GetRefGuid())
		return true;
	return ObjectCreated.PostProcess();
}

#endif

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