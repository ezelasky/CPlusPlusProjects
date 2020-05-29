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

extern DBOBJECT  g_ObjectStruct;


CParamBase::CParamBase()
{
	Initialize();
}

CParamBase::~CParamBase()
{
	if(HasCmnd()) CloseCmnd();
}

void CParamBase::CloseCmnd()
{
	if(m_pIParamsAccessor) 
	{
		if(m_hParamsAccessor) 
			m_pIParamsAccessor->ReleaseAccessor(m_hParamsAccessor, NULL);
		m_pIParamsAccessor->Release();
	}
	if(m_pParamOrdinals)				
		CoTaskMemFree(m_pParamOrdinals);
	if(m_pDBParamBindInfo)			
		CoTaskMemFree(m_pDBParamBindInfo);
	if(m_pParamsDBBinds)				
		CoTaskMemFree(m_pParamsDBBinds);
	if(m_pParamsData)					
		CoTaskMemFree(m_pParamsData);
	Initialize();
	CCmndBase::CloseCmnd();
}

void CParamBase::Initialize()
{
	m_nParamsBufferSize=0;
	m_nParams=0;
	m_hParamsAccessor=0;
	m_pDBParamBindInfo=0;
	m_pIParamsAccessor=0;
	m_pParamOrdinals=0;
	m_pParamsDBBinds=0;
	m_bBatchable=false;
	m_nNumOutputParams=0;
	m_pParamsData=NULL;
}

bool	CParamBase::OpenParam(CPInfo *pPInfo, bool bPrepared)
{
	OleDBPro_ASSERT(m_pICommandText, "\nm_pICommandText must be wired with a command now!");
	OleDBPro_ASSERT(pPInfo, "\npPInfo can't be NULL!");

	if(!m_nParams) return false;
	m_pParamOrdinals=(ULONG*)(CoTaskMemRealloc(m_pParamOrdinals, sizeof(ULONG)*m_nParams));
	ATLASSERT(m_pParamOrdinals);
	memset(m_pParamOrdinals, 0, m_nParams*sizeof(ULONG));
	m_pDBParamBindInfo=(DBPARAMBINDINFO*)(CoTaskMemRealloc(m_pDBParamBindInfo, sizeof(DBPARAMBINDINFO)*m_nParams));
	ATLASSERT(m_pDBParamBindInfo);
	memset(m_pDBParamBindInfo, 0, m_nParams*sizeof(DBPARAMBINDINFO));
	m_pParamsDBBinds=(DBBINDING*)(CoTaskMemRealloc(m_pParamsDBBinds, sizeof(DBBINDING)*m_nParams));
	ATLASSERT(m_pParamsDBBinds);
	memset(m_pParamsDBBinds, 0, m_nParams*sizeof(DBBINDING));
	
	CreateParamDBBindings(pPInfo);
	
	m_nNumOutputParams=NumOutputParams();

	if(!SendParamsInfo(bPrepared))
	{
		CloseCmnd();
		return false;
	}
	if(!CreateParamAccessor())
	{
		CloseCmnd();
		return false;
	}
	return true;
}

bool	CParamBase::CreateParamAccessor()
{
	OleDBPro_ASSERT(m_pICommandText, "\nm_pICommandText must be wired with a command now!");
	OleDBPro_ASSERT(m_pIParamsAccessor==NULL, "\nm_pIParamsAccessor must be NULL at this time!");
	OleDBPro_ASSERT(m_nParams, "\nMake sure that a parameter is available at least!");
	OleDBPro_ASSERT(m_pParamsDBBinds, "\nm_pParamsDBBinds must be available at this time!");
	OleDBPro_ASSERT(m_nParamsBufferSize, "\nm_nParamsBufferSize can't be zero!");

	m_hr=m_pICommandText->QueryInterface(__uuidof(IAccessor), (void**)&m_pIParamsAccessor);
	if(FAILED(m_hr) || m_pIParamsAccessor==NULL)
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pICommandText, __uuidof(ICommandText), &m_strError);
		return false;
	}
	
	m_hr=m_pIParamsAccessor->CreateAccessor(DBACCESSOR_PARAMETERDATA, m_nParams, m_pParamsDBBinds, m_nParamsBufferSize, &m_hParamsAccessor, NULL);
	if(FAILED(m_hr)|| (!m_hParamsAccessor))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIParamsAccessor, __uuidof(IAccessor), &m_strError);
		return false;
	}
	return true;
}

ULONG	CParamBase::NumOutputParams()
{
	OleDBPro_ASSERT(m_pDBParamBindInfo, "\nm_pDBParamBindInfo can't be NULL!");

	ULONG		nOutputParams=0;
	
	for(ULONG nParam=0; nParam<m_nParams; nParam++)
	{
		nOutputParams +=((m_pDBParamBindInfo[nParam].dwFlags & DBPARAMFLAGS_ISOUTPUT) == DBPARAMFLAGS_ISOUTPUT);
	}
	return nOutputParams;
}

ULONG	CParamBase::ComputeNumOfParams(LPCOLESTR strSQL, WCHAR cParam)
{
	OleDBPro_ASSERT(strSQL, "\nstrSQL can't be NULL!");
	ULONG			nCols=0;
	LPCOLESTR		p=NULL;
	p=strSQL;
	while(p && *p)
	{
		if(*p==cParam) nCols++;
		p++;
	}
	return nCols;
}

bool	CParamBase::SendParamsInfo(bool bPrepared)
{
	CComPtr<ICommandPrepare>			pICommandPrepare;
	CComPtr<ICommandWithParameters>		pICommandWithParameters;

	OleDBPro_ASSERT(m_nParams, "\nMake sure that a parameter is available at least!");
	ATLASSERT(m_pParamOrdinals);
	OleDBPro_ASSERT(m_pDBParamBindInfo, "\nm_pDBParamBindInfo can't be NULL now!");

	m_hr=m_pICommandText->QueryInterface(__uuidof(ICommandWithParameters), (void**)&pICommandWithParameters);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pICommandText, __uuidof(ICommandText), &m_strError);
		return false;
	}
	m_hr=pICommandWithParameters->SetParameterInfo(m_nParams, m_pParamOrdinals, m_pDBParamBindInfo);
    if(bPrepared)
    {
	    m_hr=m_pICommandText->QueryInterface(__uuidof(ICommandPrepare), (void**)&pICommandPrepare);
	    if(pICommandPrepare)
	    {
		    m_hr=pICommandPrepare->Prepare(0);
		    if(FAILED(m_hr))
		    {
			    AtlTrace("Warning: failed in preparing a parameterized statement or stored procedure!\n");
		    }
	    }
    }
	return true;
}

void CParamBase::CreateParamDBBindings(CPInfo *pPInfo)
{
	OleDBPro_ASSERT(m_pParamOrdinals, "\nm_pParamOrdinals can't be NULL at this time!");
	OleDBPro_ASSERT(m_pParamsDBBinds, "\nm_pParamsDBBinds must be available at this time!");
	OleDBPro_ASSERT(m_pDBParamBindInfo, "\nm_pDBParamBindInfo can't be NULL now!");
	OleDBPro_ASSERT(pPInfo, "\npPInfo can't be NULL!");
	OleDBPro_ASSERT(m_nParams, "\nMake sure that a parameter is available at least!");
	
	m_nParamsBufferSize=0;
	for(ULONG nParam=0; nParam<m_nParams; nParam++)
	{
		OleDBPro_ASSERT(pPInfo[nParam].m_nDBType>DBTYPE_NULL, "\nMake sure that the parameter data type is set correctly!");
		OleDBPro_ASSERT(pPInfo[nParam].m_nDBParamIO, "\nMake sure that the parameter type is set correctly!");
		OleDBPro_ASSERT((pPInfo[nParam].m_nDBType & DBTYPE_BYREF)!=DBTYPE_BYREF, "\nMake sure that the parameter data type is set correctly!");
		
		m_pParamsDBBinds[nParam].wType=pPInfo[nParam].m_nDBType;
		m_pParamsDBBinds[nParam].bPrecision=pPInfo[nParam].m_nPrecision;
		m_pDBParamBindInfo[nParam].bPrecision=pPInfo[nParam].m_nPrecision;
		m_pParamsDBBinds[nParam].bScale=pPInfo[nParam].m_nScale;
		m_pDBParamBindInfo[nParam].bScale=pPInfo[nParam].m_nScale;
	
		m_pParamOrdinals[nParam]=nParam+1;
		m_pParamsDBBinds[nParam].iOrdinal = nParam+1; 

		m_pParamsDBBinds[nParam].eParamIO = pPInfo[nParam].m_nDBParamIO;	
		m_pParamsDBBinds[nParam].dwMemOwner = DBMEMOWNER_CLIENTOWNED;
		
		if((m_pParamsDBBinds[nParam].eParamIO & DBPARAMIO_INPUT)==DBPARAMIO_INPUT) 
			m_pDBParamBindInfo[nParam].dwFlags=DBPARAMFLAGS_ISINPUT;
		if((m_pParamsDBBinds[nParam].eParamIO & DBPARAMIO_OUTPUT)==DBPARAMIO_OUTPUT) 
			m_pDBParamBindInfo[nParam].dwFlags |=DBPARAMFLAGS_ISOUTPUT;
		ATLASSERT(m_pDBParamBindInfo[nParam].dwFlags);

		m_pDBParamBindInfo[nParam].pwszDataSourceType=GetDataSourceType(m_pParamsDBBinds[nParam].wType);
		m_pDBParamBindInfo[nParam].pwszName=pPInfo[nParam].m_strPName;
		
		if(m_pParamsDBBinds[nParam].wType==DBTYPE_STR||m_pParamsDBBinds[nParam].wType==DBTYPE_BYTES || m_pParamsDBBinds[nParam].wType==DBTYPE_WSTR || m_pParamsDBBinds[nParam].wType==DBTYPE_IUNKNOWN || m_pParamsDBBinds[nParam].wType==DBTYPE_VARNUMERIC)
			m_pParamsDBBinds[nParam].dwPart=DBPART_VALUE|DBPART_STATUS|DBPART_LENGTH;
		else
			m_pParamsDBBinds[nParam].dwPart=DBPART_VALUE|DBPART_STATUS;
		
		if(m_pParamsDBBinds[nParam].wType==DBTYPE_STR||m_pParamsDBBinds[nParam].wType==DBTYPE_BYTES || m_pParamsDBBinds[nParam].wType==DBTYPE_WSTR)
		{
			if(pPInfo[nParam].m_nLen==0)
			{
				m_pParamsDBBinds[nParam].cbMaxLen=DEFAULT_OUPUT_LEN+1;
                m_pDBParamBindInfo[nParam].ulParamSize=DEFAULT_OUPUT_LEN;
				if(m_pParamsDBBinds[nParam].wType==DBTYPE_WSTR)
					m_pParamsDBBinds[nParam].cbMaxLen=m_pParamsDBBinds[nParam].cbMaxLen*sizeof(WCHAR);
			}
			else if(pPInfo[nParam].m_nLen==OLEDB_PARAM_BLOB)
			{
				m_pDBParamBindInfo[nParam].ulParamSize=OLEDB_PARAM_BLOB;
				if(m_pParamsDBBinds[nParam].wType==DBTYPE_WSTR)
				{
					m_pDBParamBindInfo[nParam].pwszDataSourceType=L"DBTYPE_WLONGVARCHAR";
				}
				else if(m_pParamsDBBinds[nParam].wType==DBTYPE_STR)
				{
					m_pDBParamBindInfo[nParam].pwszDataSourceType=L"DBTYPE_LONGVARCHAR";
				}
				else
				{
					m_pDBParamBindInfo[nParam].pwszDataSourceType=L"DBTYPE_LONGVARBINARY";
				}
				m_pDBParamBindInfo[nParam].dwFlags |=DBPARAMFLAGS_ISLONG;
			}
			else
			{
				if(m_pParamsDBBinds[nParam].wType==DBTYPE_WSTR)
				{
					m_pParamsDBBinds[nParam].cbMaxLen=pPInfo[nParam].m_nLen+sizeof(WCHAR);
					m_pDBParamBindInfo[nParam].ulParamSize=pPInfo[nParam].m_nLen/sizeof(WCHAR);
				}
				else
				{
					m_pParamsDBBinds[nParam].cbMaxLen=pPInfo[nParam].m_nLen+sizeof(char);
					m_pDBParamBindInfo[nParam].ulParamSize=pPInfo[nParam].m_nLen;
				}
			}
		}
		else if (m_pParamsDBBinds[nParam].wType==DBTYPE_VARNUMERIC)
		{
			if(pPInfo[nParam].m_nLen==0	|| pPInfo[nParam].m_nLen==OLEDB_PARAM_BLOB || pPInfo[nParam].m_nLen>(255+3))
				m_pParamsDBBinds[nParam].cbMaxLen=(255+3);
			else
				m_pParamsDBBinds[nParam].cbMaxLen=pPInfo[nParam].m_nLen;
			m_pDBParamBindInfo[nParam].ulParamSize=m_pParamsDBBinds[nParam].cbMaxLen;
		}
		else
		{
			m_pParamsDBBinds[nParam].cbMaxLen=m_pDBParamBindInfo[nParam].ulParamSize=*GetFixLen(m_pParamsDBBinds[nParam].wType);
		}

		if(m_pParamsDBBinds[nParam].eParamIO==DBPARAMIO_INPUT)
		{
			if(m_pParamsDBBinds[nParam].wType==DBTYPE_STR|| m_pParamsDBBinds[nParam].wType==DBTYPE_WSTR || m_pParamsDBBinds[nParam].wType==DBTYPE_BYTES ||m_pParamsDBBinds[nParam].wType==DBTYPE_VARNUMERIC)
			{
				m_pParamsDBBinds[nParam].cbMaxLen=sizeof(void*);
				m_pParamsDBBinds[nParam].wType=(m_pParamsDBBinds[nParam].wType|DBTYPE_BYREF);
			}
		}

		if(pPInfo[nParam].m_nDBType==DBTYPE_IUNKNOWN)
		{
			m_pParamsDBBinds[nParam].wType = DBTYPE_IUNKNOWN;
			m_pParamsDBBinds[nParam].cbMaxLen=sizeof(IUnknown*);
			m_pParamsDBBinds[nParam].pObject=&g_ObjectStruct;
			m_pDBParamBindInfo[nParam].ulParamSize=OLEDB_PARAM_BLOB;
		}

		m_pParamsDBBinds[nParam].obValue=m_nParamsBufferSize;
		m_nParamsBufferSize +=m_pParamsDBBinds[nParam].cbMaxLen;
		if((m_pParamsDBBinds[nParam].dwPart & DBPART_LENGTH)==DBPART_LENGTH) 
		{
			m_pParamsDBBinds[nParam].obLength=m_nParamsBufferSize;
			m_nParamsBufferSize +=sizeof(ULONG);
		}
		m_pParamsDBBinds[nParam].obStatus=m_nParamsBufferSize;
		m_nParamsBufferSize +=sizeof(DWORD);
	}
}

void	CParamBase::CopyToClient(ULONG nCount, CPData *pPData)
{
	DWORD		nStatus;
	ULONG		nRows=nCount/m_nParams;
	for(ULONG nRow=0; nRow<nRows; nRow++)
	{
		for(ULONG nParam=0; nParam<m_nParams; nParam++)
		{
			if((m_pParamsDBBinds[nParam].eParamIO & DBPARAMIO_OUTPUT) == DBPARAMIO_OUTPUT)
			{
				nStatus=*((DWORD*)(m_pParamsData+nRow*m_nParamsBufferSize+m_pParamsDBBinds[nParam].obStatus));
				if(nStatus==DBSTATUS_S_OK || nStatus==DBSTATUS_S_TRUNCATED)
				{
					void	*pData=pPData[nRow*m_nParams+nParam].m_pData;
					if(pData)
					{
						void	*pSrc= m_pParamsData+nRow*m_nParamsBufferSize+m_pParamsDBBinds[nParam].obValue;
						if(m_pParamsDBBinds[nParam].wType==DBTYPE_STR || m_pParamsDBBinds[nParam].wType==DBTYPE_WSTR||m_pParamsDBBinds[nParam].wType==DBTYPE_BYTES||m_pParamsDBBinds[nParam].wType==DBTYPE_VARNUMERIC)
						{
							ULONG		nLen=*((ULONG*)(m_pParamsData+nRow*m_nParamsBufferSize+m_pParamsDBBinds[nParam].obLength));
							if(nLen>pPData[nRow*m_nParams+nParam].m_nLen) 
								nLen=pPData[nRow*m_nParams+nParam].m_nLen;
							else
								memset(pData, 0, pPData[nRow*m_nParams+nParam].m_nLen); 
							memcpy(pData, pSrc, nLen);
						}
						else
							memcpy(pData, pSrc, *GetFixLen(m_pParamsDBBinds[nParam].wType));
					}
				}
			}
		}
	}	
}

void	CParamBase::PrepareData(ULONG nCount, CPData* pPData)
{
	OleDBPro_ASSERT(HasCmnd(), "\nMake sure that a command is available!");
	OleDBPro_ASSERT(m_nParams, "\nMake sure that a parameter is available at least!");
	OleDBPro_ASSERT((nCount%m_nParams)==0, "\nMake sure that the correct count of parameter data is given!");	
	DBSTATUS	*pnDBStatus;
	void		*pValue;
	ULONG		nRows=nCount/m_nParams;
	m_pParamsData=(BYTE*)CoTaskMemRealloc(m_pParamsData, nRows*m_nParamsBufferSize);
	ATLASSERT(m_pParamsData);
	memset(m_pParamsData, 0, nRows*m_nParamsBufferSize);

	for(ULONG nRow=0; nRow<nRows; nRow++)
	{
		for(ULONG nParam=0; nParam<m_nParams; nParam++)
		{
			pnDBStatus=(DBSTATUS*)(m_pParamsData+nRow*m_nParamsBufferSize+m_pParamsDBBinds[nParam].obStatus);
			if(pPData[nRow*m_nParams+nParam].m_pData)
			{
				ULONG		nLen=0;
				*pnDBStatus=DBSTATUS_S_OK;	
				if(pPData[nRow*m_nParams+nParam].m_nDBStatus==DBSTATUS_S_ISNULL && m_pParamsDBBinds[nParam].eParamIO==DBPARAMIO_OUTPUT|DBPARAMIO_INPUT)
					*pnDBStatus=DBSTATUS_S_ISNULL;
				if((m_pParamsDBBinds[nParam].wType &~ DBTYPE_BYREF)==DBTYPE_WSTR)
				{
					if(pPData[nRow*m_nParams+nParam].m_nLen==OLEDB_STRING_END || ((m_pParamsDBBinds[nParam].eParamIO & DBPARAMIO_OUTPUT)==DBPARAMIO_OUTPUT && ((m_pParamsDBBinds[nParam].eParamIO & DBPARAMIO_INPUT)==DBPARAMIO_INPUT)))
						nLen=wcslen((wchar_t*)(pPData[nRow*m_nParams+nParam].m_pData))*sizeof(WCHAR);
					else
						nLen=pPData[nRow*m_nParams+nParam].m_nLen;
				}
				else if((m_pParamsDBBinds[nParam].wType &~ DBTYPE_BYREF) ==DBTYPE_STR)
				{
					if(pPData[nRow*m_nParams+nParam].m_nLen==OLEDB_STRING_END || ((m_pParamsDBBinds[nParam].eParamIO & DBPARAMIO_OUTPUT)==DBPARAMIO_OUTPUT && ((m_pParamsDBBinds[nParam].eParamIO & DBPARAMIO_INPUT)==DBPARAMIO_INPUT)))
						nLen=strlen((char*)(pPData[nRow*m_nParams+nParam].m_pData))*sizeof(char);
					else
						nLen=pPData[nRow*m_nParams+nParam].m_nLen;
				}
				else if((m_pParamsDBBinds[nParam].wType &~ DBTYPE_BYREF)==DBTYPE_BYTES || m_pParamsDBBinds[nParam].wType==DBTYPE_IUNKNOWN) 
				{
					OleDBPro_ASSERT(pPData[nRow*m_nParams+nParam].m_nLen!=OLEDB_STRING_END, "\nMake sure that the length is given for a binary data or storage object!");
					nLen=pPData[nRow*m_nParams+nParam].m_nLen;
				}
				else if((m_pParamsDBBinds[nParam].wType &~ DBTYPE_BYREF)==DBTYPE_VARNUMERIC) 
				{
					OleDBPro_ASSERT(pPData[nRow*m_nParams+nParam].m_nLen!=OLEDB_STRING_END, "\nMake sure that the length is given for a varnumeric data!");
					OleDBPro_ASSERT(pPData[nRow*m_nParams+nParam].m_nLen>=3, "\nMake sure that the length is given for a varnumeric data!");
					nLen=pPData[nRow*m_nParams+nParam].m_nLen;
				}
				else
					nLen=*GetFixLen(m_pParamsDBBinds[nParam].wType);	
				pValue=m_pParamsData+nRow*m_nParamsBufferSize+m_pParamsDBBinds[nParam].obValue;
				if(m_pParamsDBBinds[nParam].eParamIO==DBPARAMIO_INPUT && (m_pParamsDBBinds[nParam].wType==(DBTYPE_STR|DBTYPE_BYREF)||m_pParamsDBBinds[nParam].wType==(DBTYPE_BYTES|DBTYPE_BYREF) || m_pParamsDBBinds[nParam].wType==(DBTYPE_WSTR|DBTYPE_BYREF) || m_pParamsDBBinds[nParam].wType==(DBTYPE_VARNUMERIC|DBTYPE_BYREF)))
					memcpy(pValue, &(pPData[nRow*m_nParams+nParam].m_pData), sizeof(void*));	
				else if(m_pParamsDBBinds[nParam].wType==DBTYPE_IUNKNOWN)
					memcpy(pValue, pPData[nRow*m_nParams+nParam].m_pData, sizeof(IUnknown*));
				else
					memcpy(pValue, pPData[nRow*m_nParams+nParam].m_pData, nLen);

				if((m_pParamsDBBinds[nParam].dwPart & DBPART_LENGTH)==DBPART_LENGTH)
					*((ULONG*)(m_pParamsData+nRow*m_nParamsBufferSize+m_pParamsDBBinds[nParam].obLength))=nLen;	
			}
			else
			{
				if((m_pParamsDBBinds[nParam].eParamIO & DBPARAMIO_OUTPUT)==DBPARAMIO_OUTPUT)
					*pnDBStatus=DBSTATUS_S_OK;
				else
					*pnDBStatus=DBSTATUS_S_ISNULL;	
			}
		}
	}
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