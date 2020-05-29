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
#include "sqloledb.h"

char	g_strOleDBProKey[40]={0};

void	GetOLEErrorInfo(IUnknown* pObjectWithError, REFIID IID_InterfaceWithError, BSTR	*pstrErrInfo);

IDataInitialize	 *g_pIDataInitialize=NULL;
IDBPromptInitialize	*g_pIDBPromptInitialize=NULL;
ULONG	g_nODataSourceCount=0;

const ULONG g_nSChar=sizeof(signed char);
const ULONG	g_nShort=sizeof(SHORT);
const ULONG	g_nLong=sizeof(LONG);
const ULONG g_nLInteger=sizeof(LARGE_INTEGER);
const ULONG g_nByte=sizeof(BYTE);
const ULONG	g_nUShort=sizeof(unsigned short);
const ULONG	g_nUInt=sizeof(unsigned int);
const ULONG	g_nULInteger=sizeof(ULARGE_INTEGER);
const ULONG g_nFloat=sizeof(float);
const ULONG	g_nDouble=sizeof(double);
const ULONG g_nCy=sizeof(CY);
const ULONG	g_nDecimal=sizeof(DECIMAL);
const ULONG	g_nNumeric=sizeof(DB_NUMERIC);
const ULONG	g_nBool=sizeof(VARIANT_BOOL);
const ULONG	g_nVariant=sizeof(VARIANT);
const ULONG g_nGuid=sizeof(GUID);
const ULONG g_nDate=sizeof(DATE);
const ULONG	g_nDBDate=sizeof(DBDATE);
const ULONG g_nDBTime=sizeof(DBTIME);
const ULONG g_nDBTimeStamp=sizeof(DBTIMESTAMP);
const ULONG g_nBstr=sizeof(BSTR);
const ULONG g_nFileTime=sizeof(FILETIME);
const ULONG g_nHChapter=sizeof(HCHAPTER);
const ULONG g_nPropVariant=sizeof(PROPVARIANT);
const ULONG	g_nByRef=sizeof(void*);
const ULONG	g_nArray=sizeof(SAFEARRAY*);
const ULONG g_nVector=sizeof(DBVECTOR);

const ULONG	g_nSQLVariant=sizeof(SSVARIANT); //sql server specific data type

bool GetIDataInitialize()
{
	if(!g_pIDataInitialize)
	{
		HRESULT hr = ::CoCreateInstance(__uuidof(MSDAINITIALIZE), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDataInitialize), (void**)&g_pIDataInitialize);
		if(FAILED(hr))
			return false;
	}
	return true;
}

bool GetIDBPromptInitialize()
{
	if(!g_pIDBPromptInitialize)
	{
		HRESULT hr = CoCreateInstance(__uuidof(DataLinks), NULL, CLSCTX_INPROC_SERVER, __uuidof(IDBPromptInitialize), (void**)&g_pIDBPromptInitialize);
		if(FAILED(hr))
			return false;
	}
	return true;
}

void UninitOLEDBCore()
{
	if(g_pIDataInitialize)
	{
		g_pIDataInitialize->Release();
		g_pIDataInitialize=NULL;
	}
	if(g_pIDBPromptInitialize)
	{
		g_pIDBPromptInitialize->Release();
		g_pIDBPromptInitialize=NULL;
	}
}

CRtnInfo::CRtnInfo()
{
	m_hr=S_OK;
	m_strError=NULL;
	m_pGetErrorInfo=&GetOLEErrorInfo;
}

CRtnInfo::~CRtnInfo()
{
	if(m_strError) 
		::SysFreeString(m_strError);
}

CODataSource::CODataSource()
{
	g_nODataSourceCount++;
	Initilize();
}

CODataSource::~CODataSource()
{
	g_nODataSourceCount--;
	if(g_nODataSourceCount==0)
		UninitOLEDBCore();
	if(IsOpen()) 
		Close();
}

REFIID	CODataSource::GetBaseIID()
{
	return __uuidof(IDBInitialize);
}

void CODataSource::Initilize()
{
	m_pIDBInitialize=0;
	COPropSet	OPropSet(DBPROPSET_DBINIT);
	m_DBPropSet=OPropSet;
}

void	CODataSource::SetPWD(LPCOLESTR strPWD)
{

	m_DBPropSet.AddProperty(DBPROP_AUTH_PASSWORD, strPWD);
}

void	CODataSource::SetUID(LPCOLESTR strUID)
{
	m_DBPropSet.AddProperty(DBPROP_AUTH_USERID, strUID);
}

void	CODataSource::SetLocation(LPCOLESTR strLocation) 
{
	m_DBPropSet.AddProperty(DBPROP_INIT_LOCATION, strLocation);
}

void	CODataSource::SetServer(LPCOLESTR strServer)
{
	m_DBPropSet.AddProperty(DBPROP_INIT_DATASOURCE, strServer);
}

bool CODataSource::SetPropEx(tagDBPROPSET *pDBPropEx)
{
	OleDBPro_ASSERT(pDBPropEx, "\npDBPropEx must point to a valid DBPROPSET structure!");
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this object is already opened!");
	
	CComPtr<IDBProperties>		pIDBProperties;
	if(FAILED(m_hr = m_pIDBInitialize->QueryInterface(__uuidof(IDBProperties), (void **)&pIDBProperties)))
	{	
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(m_pIDBInitialize, __uuidof(IDBInitialize), &m_strError);
		return false;
	}
	if(FAILED(m_hr = pIDBProperties->SetProperties(1, pDBPropEx)))
	{	
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(pIDBProperties, __uuidof(IDBProperties), &m_strError);
		return false;
	}
	return true;
}

void	CODataSource::SetProviderString(LPCOLESTR strProviderString)
{
	m_DBPropSet.AddProperty(DBPROP_INIT_PROVIDERSTRING, strProviderString);
}

void	CODataSource::SetDatabase(LPCOLESTR strDatabase) 
{
	m_DBPropSet.AddProperty(DBPROP_INIT_CATALOG, strDatabase);
}

void CODataSource::Close()
{
	if(m_pIDBInitialize) 
		m_pIDBInitialize->Release();
	Initilize();
}

bool	CODataSource::CreateDataSource(LPCOLESTR strProvider, LPCOLESTR strServer)
{
	OleDBPro_ASSERT(strProvider, "\nstrProvider can't be NULL!");
	OleDBPro_ASSERT(wcslen(strProvider), "\nstrProvider can't be zero in length!");
	OleDBPro_ASSERT(strServer, "\nstrServer can't be NULL!");
	OleDBPro_ASSERT(wcslen(strServer), "\nstrServer can't be zero in length!");

	CComPtr<IDBDataSourceAdmin> pIDBDataSourceAdmin;
	CComPtr<IOpenRowset> pIOpenRowset;
	CComPtr<IDBInitialize> pIDBInitialize;

	COPropSet DBPropSet(DBPROPSET_DBINIT);

	if(!GetIDataInitialize())
		return false;

	HRESULT hr = g_pIDataInitialize->GetDataSource(NULL, CLSCTX_INPROC_SERVER, strProvider, __uuidof(IDBInitialize), (IUnknown**)&pIDBInitialize);
	if(FAILED(hr)) return false;
	
	hr=pIDBInitialize->Initialize();

	hr=pIDBInitialize->QueryInterface(__uuidof(IDBDataSourceAdmin), (void**)&pIDBDataSourceAdmin);
	if(FAILED(hr)) return false;

	DBPropSet.AddProperty(DBPROP_INIT_DATASOURCE, strServer);
	hr=pIDBDataSourceAdmin->CreateDataSource(1, &DBPropSet, NULL, __uuidof(IOpenRowset), (IUnknown**)&pIOpenRowset);
	if(FAILED(hr)) return false;
	
	return true;
}

bool CODataSource::PostProcess()
{
	OleDBPro_ASSERT(IsOpen(), "\nCall this function after a data source object is already created!");
	CComPtr<IDBInitialize> pIDBInitialize;
	m_hr = (*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IDBInitialize), (void **)&pIDBInitialize);
	if(FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(*GetBaseInterfacePPtr(), GetBaseIID(), &m_strError);
		return false;
	}
	m_hr = pIDBInitialize->Initialize();
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(pIDBInitialize, __uuidof(IDBInitialize), &m_strError);
		return false;
	}
	return true;
}

bool CODataSource::Open(LPCOLESTR strConnection, bool bUseCCE, bool bPooling, short nPrompt)
{
	OleDBPro_ASSERT(strConnection, "\nstrConnection can't be NULL!");
	OleDBPro_ASSERT(wcslen(strConnection), "\nstrConnection can't be zero in length!");

	if(IsOpen()) 
		Close();
	
	CComPtr<IDBProperties>		pIDBProperties;
	LONG	nServices=DBPROPVAL_OS_ENABLEALL;

	if(!GetIDataInitialize())
	{
		m_hr=E_FAIL;
		if(m_strError) 
			::SysFreeString(m_strError);
		m_strError = ::SysAllocString(L"OleDBPro is not initialized or MS Data Link Object not available!");
		return false;
	}
	
	m_hr = g_pIDataInitialize->GetDataSource(NULL, CLSCTX_INPROC_SERVER, strConnection, GetBaseIID(), GetBaseInterfacePPtr());
	if (FAILED(m_hr)) 
	{
		if(m_strError) 
			::SysFreeString(m_strError);
		m_strError = ::SysAllocString(L"Can't create Data Source Object");
		return false;
	}
	m_hr = (*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IDBProperties), (void **)&pIDBProperties);
	if(FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(*GetBaseInterfacePPtr(), GetBaseIID(), &m_strError);
		Close();
		return false;
	}
	
	if(!bPooling) 
		nServices=(nServices &~DBPROPVAL_OS_RESOURCEPOOLING);
	else
		nPrompt=DBPROMPT_NOPROMPT;
		
	if(!bUseCCE)
		nServices=(nServices &~ DBPROPVAL_OS_CLIENTCURSOR);
	
	COPropSet	OPropSet(DBPROPSET_DBINIT);
	OPropSet.AddProperty(DBPROP_INIT_OLEDBSERVICES, nServices);
	OPropSet.AddProperty(DBPROP_INIT_PROMPT, nPrompt);
	for(ULONG nProp=0; nProp<m_DBPropSet.cProperties; nProp++)
	{
		OPropSet.AddProperty(m_DBPropSet.rgProperties[nProp].dwPropertyID, m_DBPropSet.rgProperties[nProp].vValue);
	}
	m_hr=pIDBProperties->SetProperties(1, &OPropSet);
/*	if(FAILED(m_hr))
	{	
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(pIDBProperties, __uuidof(IDBProperties), &m_strError);
		Close();
		return false;
	}*/
	return PostProcess();
}

bool CODataSource::Open(HWND hWnd, DBPROMPTOPTIONS dwPromptOptions)
{
	if(IsOpen())
		Close();
	if (!GetIDBPromptInitialize())
	{
		m_hr=E_FAIL;
		if(m_strError) 
			::SysFreeString(m_strError);
		m_strError = ::SysAllocString(L"Can't get the interface IDBPromptInitialize!");
		return false;
	}

	m_hr = g_pIDBPromptInitialize->PromptDataSource(NULL, hWnd, dwPromptOptions, 0, NULL, NULL, GetBaseIID(), GetBaseInterfacePPtr());
	if (FAILED(m_hr))
	{
		if(m_strError) 
			::SysFreeString(m_strError);
		m_strError = ::SysAllocString(L"The user canceled the dialog or a provider-specific error occurred.");
		return false;
	}
	return PostProcess();
}

bool CODataSource::Open(const CLSID& gCLSID, bool bUseCCE, bool bPooling, short nPrompt)
{
	OleDBPro_ASSERT(gCLSID!=IID_NULL, "\nMake sure the passed clsidProvider is valid!");
	CComBSTR	bstrConnect=L"Provider=";
	LPOLESTR lpsz=0;
	m_hr=::ProgIDFromCLSID(gCLSID, &lpsz); 
	if(lpsz==NULL || FAILED(m_hr))
	{
		if(m_strError) 
			::SysFreeString(m_strError);
		m_strError = ::SysAllocString(L"Cann't open registry and find a class ID");
		return false;
	}
	bstrConnect +=lpsz;	
	CoTaskMemFree(lpsz);
	bstrConnect +=L";";

	return Open(bstrConnect, bUseCCE, bPooling, nPrompt);
}

bool CODataSource::GetProperty(const GUID& guid, DBPROPID propid, VARIANT* pVariant)
{
	OleDBPro_ASSERT(pVariant, "\nMake sure that pVariant is pointing to a valid VARIANT data and can't be NULL!");
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this object is already opened!");
	bool	bSuccess=true;
	m_hr=S_OK;
	CComPtr<IDBProperties>	pIDBProperties;
	if (!SUCCEEDED(m_hr = (*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IDBProperties), (void **)&pIDBProperties)))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(*GetBaseInterfacePPtr(), GetBaseIID(), &m_strError);
		return false;
	}

	DBPROPIDSET	DBPropIDSet;
	DBPropIDSet.guidPropertySet=guid;
	DBPropIDSet.cPropertyIDs=1;
	DBPropIDSet.rgPropertyIDs=&propid;
	DBPROPSET* pPropSet = NULL;
	ULONG ulPropSet = 0;
	m_hr = pIDBProperties->GetProperties(1, &DBPropIDSet, &ulPropSet, &pPropSet);
	if (FAILED(m_hr)) 
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(*GetBaseInterfacePPtr(), GetBaseIID(), &m_strError);
		bSuccess=false;
	}

	if(pPropSet && pPropSet->rgProperties) 
	{
		VariantCopy(pVariant, &pPropSet->rgProperties[0].vValue);
		VariantClear(&(pPropSet->rgProperties[0].vValue));
	}
	if(pPropSet && pPropSet->rgProperties) 
		CoTaskMemFree(pPropSet->rgProperties);
	if(pPropSet) 
		CoTaskMemFree(pPropSet);

	return bSuccess;
}

DBPROPFLAGS CODataSource::GetDBPropFlags(const GUID& guidPropertySet, DBPROPID nPropertyID, VARTYPE *pnVarType)
{
	if(pnVarType) 
		*pnVarType=DBTYPE_EMPTY;
	m_hr=S_OK;
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that the data source is already opened!");

	CComPtr<IDBProperties>		pIDBProperties;
	if (!SUCCEEDED(m_hr = (*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IDBProperties), (void **)&pIDBProperties)))
	{	
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(*GetBaseInterfacePPtr(), GetBaseIID(), &m_strError);
		return DBPROPFLAGS_NOTSUPPORTED;
	}
	
	ULONG		nNumOfInfoSets=0;	
	DBPROPINFOSET		*pDBPropInfoSet=NULL;
	DBPROPFLAGS			nFlags=DBPROPFLAGS_NOTSUPPORTED;

	m_hr=pIDBProperties->GetPropertyInfo(0, 0, &nNumOfInfoSets, &pDBPropInfoSet, 0);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(*GetBaseInterfacePPtr(), GetBaseIID(), &m_strError);
	}
	for(ULONG nIndex=0; nIndex<nNumOfInfoSets; nIndex++)
	{
		if(memcmp(&guidPropertySet, &(pDBPropInfoSet[nIndex].guidPropertySet), sizeof(GUID))==0)
		{
			for(ULONG nP=0; nP<pDBPropInfoSet[nIndex].cPropertyInfos; nP++)
			{
				if(pDBPropInfoSet[nIndex].rgPropertyInfos[nP].dwPropertyID==nPropertyID)
				{
					nFlags=pDBPropInfoSet[nIndex].rgPropertyInfos[nP].dwFlags;
					if(pnVarType) *pnVarType=pDBPropInfoSet[nIndex].rgPropertyInfos[nP].vtType;
					break;
				}	
			}
		}
		if(pDBPropInfoSet[nIndex].rgPropertyInfos)
			CoTaskMemFree(pDBPropInfoSet[nIndex].rgPropertyInfos);
	}
	if(pDBPropInfoSet)
		CoTaskMemFree(pDBPropInfoSet);

	return nFlags;
}

COSession::COSession()
{
	m_pIOpenRowset=NULL;
}

COSession::~COSession()
{
	if(IsOpen()) 
		Close();
}

bool COSession::GetDataSource(CODataSource *pODataSource)
{
	OleDBPro_ASSERT(pODataSource, "\nMake sure that pODataSource is pointing to a CODataSource object and can't be NULL!");
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this object is already opened!");
	
	CComPtr<IGetDataSource> pIGetDataSource;
	if(pODataSource->IsOpen())
		pODataSource->Close();
	pODataSource->m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IGetDataSource), (void**)&pIGetDataSource);
	if(FAILED(pODataSource->m_hr))
	{	
		if(pODataSource->m_pGetErrorInfo)
			pODataSource->m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &(pODataSource->m_strError));
		return false;
	}
	pODataSource->m_hr=pIGetDataSource->GetDataSource(pODataSource->GetBaseIID(), pODataSource->GetBaseInterfacePPtr());
	if(FAILED(pODataSource->m_hr))
	{
		if(pODataSource->m_pGetErrorInfo)
			pODataSource->m_pGetErrorInfo(pIGetDataSource, __uuidof(IGetDataSource), &(pODataSource->m_strError));
		return false;
	}
	return true;
}

bool	COSession::Open(IDBInitialize *pIDBInitialize)
{
	OleDBPro_ASSERT(pIDBInitialize, "\nMake sure that pIDBInitialize is wired with a TDataSource object and can't be NULL!");
	CComPtr<IDBCreateSession>	pIDBCreateSession;
	if(IsOpen())
		Close();
	m_hr =pIDBInitialize->QueryInterface(__uuidof(IDBCreateSession), (void **)&pIDBCreateSession);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(pIDBInitialize, __uuidof(IDBInitialize), &m_strError);
		return false;
	}
	m_hr=pIDBCreateSession->CreateSession(NULL, GetBaseIID(), GetBaseInterfacePPtr());	
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(pIDBCreateSession, __uuidof(IDBCreateSession), &m_strError);
		return false;
	}
	return PostProcess();
}

void COSession::Close()
{
	if(m_pIOpenRowset)
	{
		m_pIOpenRowset->Release();
		m_pIOpenRowset=NULL;
	}
}

REFIID	COSession::GetBaseIID()
{
	return __uuidof(IOpenRowset);
}

bool COSession::ExecuteSQL(LPCOLESTR strSQL)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this object is already opened!");
	OleDBPro_ASSERT(strSQL, "\nstrSQL can't be NULL!");
	OleDBPro_ASSERT(wcslen(strSQL), "\nstrSQL can't be zero in length!");
	CComPtr<IDBCreateCommand>	pIDBCreateCommand;
	CComPtr<ICommandText>		pICommandText;
	m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IDBCreateCommand), (void**)&pIDBCreateCommand);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}
	m_hr = pIDBCreateCommand->CreateCommand(NULL, __uuidof(ICommandText), (IUnknown**)&pICommandText);
	if (FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(pIDBCreateCommand, __uuidof(IDBCreateCommand), &m_strError);
		return false;
	}
	m_hr=pICommandText->SetCommandText(DBGUID_DEFAULT, strSQL);
	if (FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(pICommandText, __uuidof(ICommandText), &m_strError);
		return false;
	}
	m_hr=pICommandText->Execute(NULL, IID_NULL, NULL, NULL, NULL);
	if (FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(pICommandText, __uuidof(ICommandText), &m_strError);
		return false;
	}
	return true;	
}

bool COSession::BeginTrans(ISOLEVEL nIsoLevel, ITransactionOptions* pITOptions, ULONG* pnTLevel)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this object is already opened!");
	CComPtr<ITransactionLocal> pITransactionLocal;
	m_hr = (*GetBaseInterfacePPtr())->QueryInterface(__uuidof(ITransactionLocal), (void**)&pITransactionLocal);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}

	m_hr=pITransactionLocal->StartTransaction(nIsoLevel, 0, pITOptions, pnTLevel);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(pITransactionLocal, __uuidof(ITransactionLocal), &m_strError);
		return false;
	}	
	return true;
}

bool	COSession::Commit(BOOL bRetaining, DWORD nXACTTC)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this object is already opened!");
	CComPtr<ITransaction> pITransaction;

	m_hr = (*GetBaseInterfacePPtr())->QueryInterface(__uuidof(ITransaction), (void**)&pITransaction);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}

	m_hr = pITransaction->Commit(bRetaining, nXACTTC, 0);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(pITransaction, __uuidof(ITransaction), &m_strError);
		return false;
	}
	return true;
}

bool COSession::Rollback(BOID* pboidReason, BOOL bRetaining, BOOL bAsync)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this object is already opened!");
	CComPtr<ITransaction> pITransaction;

	m_hr = (*GetBaseInterfacePPtr())->QueryInterface(__uuidof(ITransaction), (void**)&pITransaction);

	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}
	m_hr = pITransaction->Abort(pboidReason, bRetaining, bAsync);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(pITransaction, __uuidof(ITransaction), &m_strError);
		return false;
	}
	return true;
}

bool	COSessionEx::CreateIndex(LPOLESTR strTblName, ULONG nIdxInfos, const DBINDEXCOLUMNDESC pIdxInfo[], LPOLESTR strIdxName, DBPROPSET *pDBPropSet, DBID **ppIndexID)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this object is already opened!");
	OleDBPro_ASSERT(strTblName, "\nstrTblName can't be NULL!");
	OleDBPro_ASSERT(wcslen(strTblName), "\nstrTblName can't be zero in length!");
	OleDBPro_ASSERT(nIdxInfos, "\nnIdxInfos can't be zero!");
	OleDBPro_ASSERT(pIdxInfo, "\npIdxInfo can't be NULL!");

	CComPtr<IIndexDefinition>	pIIndexDefinition;
	m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IIndexDefinition), (void**)&pIIndexDefinition);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}
	DBID	dbidTable;
	DBID	dbidIndex;
	memset(&dbidTable, 0, sizeof(DBID));
	memset(&dbidIndex, 0, sizeof(DBID));
	dbidTable.eKind=DBKIND_NAME;
	dbidIndex.eKind=DBKIND_NAME;
	dbidTable.uName.pwszName=strTblName;
	dbidIndex.uName.pwszName=strIdxName;
	m_hr=pIIndexDefinition->CreateIndex(&dbidTable, (strIdxName ? &dbidIndex : NULL), nIdxInfos, pIdxInfo, (pDBPropSet ? 1 : 0), pDBPropSet, ppIndexID);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(pIIndexDefinition, __uuidof(IIndexDefinition), &m_strError);
		return false;
	}
	return true;
}

bool	COSessionEx::DropIndex(LPOLESTR strTblName, LPOLESTR strIdxName)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this object is already opened!");
	OleDBPro_ASSERT(strTblName, "\nstrTblName can't be NULL!");
	OleDBPro_ASSERT(wcslen(strTblName), "\nstrTblName can't be zero in length!");
	OleDBPro_ASSERT(strIdxName, "\nstrIdxName can't be NULL!");
	OleDBPro_ASSERT(wcslen(strIdxName), "\nstrIdxName can't be zero in length!");

	DBID		dbidTbl;
	DBID		dbidIdx;
	CComPtr<IIndexDefinition>	pIIndexDefinition;
	m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IIndexDefinition), (void**)&pIIndexDefinition);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}
	memset(&dbidTbl, 0, sizeof(DBID));
	memset(&dbidIdx, 0, sizeof(DBID));
	dbidTbl.eKind=DBKIND_NAME;
	dbidTbl.uName.pwszName=strTblName;
	dbidIdx.eKind=DBKIND_NAME;
	dbidIdx.uName.pwszName=strIdxName;
	m_hr=pIIndexDefinition->DropIndex(&dbidTbl, &dbidIdx);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(pIIndexDefinition, __uuidof(IIndexDefinition), &m_strError);
		return false;
	}
	return true;
}

bool	COSessionEx::DropColumn(LPOLESTR strTblName, LPOLESTR strColName)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this object is already opened!");
	OleDBPro_ASSERT(strTblName, "\nstrTblName can't be NULL!");
	OleDBPro_ASSERT(wcslen(strTblName), "\nstrTblName can't be zero in length!");
	OleDBPro_ASSERT(strColName, "\nstrColName can't be NULL!");
	OleDBPro_ASSERT(wcslen(strColName), "\nstrColName can't be zero in length!");

	DBID		dbidTbl;
	DBID		dbidCol;
	CComPtr<ITableDefinition>	pITableDefinition;
	m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(ITableDefinition), (void**)&pITableDefinition);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}
	memset(&dbidTbl, 0, sizeof(DBID));
	memset(&dbidCol, 0, sizeof(DBID));
	dbidTbl.eKind=DBKIND_NAME;
	dbidTbl.uName.pwszName=strTblName;
	dbidCol.eKind=DBKIND_NAME;
	dbidCol.uName.pwszName=strColName;
	m_hr=pITableDefinition->DropColumn(&dbidTbl, &dbidCol);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(pITableDefinition, __uuidof(ITableDefinition), &m_strError);
		return false;
	}
	return true;
}

bool	COSessionEx::DropTable(LPOLESTR strTblName)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this object is already opened!");
	OleDBPro_ASSERT(strTblName, " \nstrTblName can't be NULL!");
	OleDBPro_ASSERT(wcslen(strTblName), "\nstrTblName can't be zero in length!");

	CComPtr<ITableDefinition>	pITableDefinition;
	m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(ITableDefinition), (void**)&pITableDefinition);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}
	DBID	dbidTable;
	memset(&dbidTable, 0, sizeof(dbidTable));
	dbidTable.eKind=DBKIND_NAME;
	dbidTable.uName.pwszName=strTblName;
	m_hr=pITableDefinition->DropTable(&dbidTable);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(pITableDefinition, __uuidof(ITableDefinition), &m_strError);
		return false;
	}
	return true;
}

bool	COSessionEx::CreateTable(LPOLESTR strTblName, ULONG nCols, const CColInfo pColInfo[], DBID **ppTableID)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this object is already opened!");
	OleDBPro_ASSERT(strTblName, "\nstrTblName can't be NULL!");
	OleDBPro_ASSERT(wcslen(strTblName), "\nstrTblName can't be zero in length!");
	OleDBPro_ASSERT(nCols, "\nnCols can't be zero!");
	OleDBPro_ASSERT(pColInfo, "\npColInfo can't be NULL!");

	CComPtr<ITableDefinition>	pITableDefinition;
	m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(ITableDefinition), (void**)&pITableDefinition);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}

	DBID	dbidTable;
	memset(&dbidTable, 0, sizeof(dbidTable));
	dbidTable.eKind=DBKIND_NAME;
	dbidTable.uName.pwszName=strTblName;

	DBCOLUMNDESC	*pDBColDesc=(DBCOLUMNDESC*)CoTaskMemAlloc(sizeof(DBCOLUMNDESC)*nCols);
	ATLASSERT(pDBColDesc);
	memset(pDBColDesc, 0, sizeof(DBCOLUMNDESC)*nCols);
	for(ULONG n=0; n<nCols; n++)
	{
		pDBColDesc[n].pwszTypeName=pColInfo[n].m_strTypeName;
		pDBColDesc[n].bPrecision=pColInfo[n].m_nPrecision;
		pDBColDesc[n].bScale=pColInfo[n].m_nScale;
		OleDBPro_ASSERT(pColInfo[n].m_nDBType<DBTYPE_VECTOR, "\nColumn data type must be valid!");
		pDBColDesc[n].wType=pColInfo[n].m_nDBType;
		if(pDBColDesc[n].wType==DBTYPE_STR || pDBColDesc[n].wType==DBTYPE_WSTR || pDBColDesc[n].wType==DBTYPE_BYTES)
		{
			pDBColDesc[n].ulColumnSize=pColInfo[n].m_nChars;
		}
		OleDBPro_ASSERT(pColInfo[n].m_strColName, "\npColInfo[n].m_strColName can't be NULL!");
		OleDBPro_ASSERT(wcslen(pColInfo[n].m_strColName), "\npColInfo[n].m_strColName can't be zero in length!");
		pDBColDesc[n].dbcid.eKind=DBKIND_NAME;
		pDBColDesc[n].dbcid.uName.pwszName=pColInfo[n].m_strColName;
		if(pColInfo[n].m_pDBPropSet)
		{
			pDBColDesc[n].cPropertySets=1;
			pDBColDesc[n].rgPropertySets=pColInfo[n].m_pDBPropSet;
		}
	}
	m_hr=pITableDefinition->CreateTable(NULL, &dbidTable, nCols, pDBColDesc, IID_NULL, 0, NULL, ppTableID, NULL);
	CoTaskMemFree(pDBColDesc);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(pITableDefinition, __uuidof(ITableDefinition), &m_strError);
		return false;
	}
	return true;
}

bool	COSessionEx::AddColumn(LPOLESTR strTblName, const CColInfo& ColInfo, DBID **ppColID)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this object is already opened!");
	OleDBPro_ASSERT(strTblName, "\nstrTblName can't be NULL!");
	OleDBPro_ASSERT(wcslen(strTblName), "\nstrTblName can't be zero in length!");

	CComPtr<ITableDefinition>	pITableDefinition;
	m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(ITableDefinition), (void**)&pITableDefinition);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}

	DBID	dbidTable;
	memset(&dbidTable, 0, sizeof(dbidTable));
	dbidTable.eKind=DBKIND_NAME;
	dbidTable.uName.pwszName=strTblName;
	
	DBCOLUMNDESC	DBColDesc;
	memset(&DBColDesc, 0, sizeof(DBCOLUMNDESC));
	DBColDesc.pwszTypeName=ColInfo.m_strTypeName;
	DBColDesc.bPrecision=ColInfo.m_nPrecision;
	DBColDesc.bScale=ColInfo.m_nScale;
	DBColDesc.dbcid.eKind=DBKIND_NAME;
	OleDBPro_ASSERT(ColInfo.m_strColName, "\nColInfo.m_strColName can't be NULL!");
	OleDBPro_ASSERT(wcslen(ColInfo.m_strColName), "\nColInfo.m_strColName can't be zero in length!");
	DBColDesc.dbcid.uName.pwszName=ColInfo.m_strColName;
	OleDBPro_ASSERT(ColInfo.m_nDBType<DBTYPE_VECTOR, "\nMake sure a valid column data type is given!");
	DBColDesc.wType=ColInfo.m_nDBType;
	if(DBColDesc.wType==DBTYPE_STR||DBColDesc.wType==DBTYPE_WSTR||DBColDesc.wType==DBTYPE_BYTES)
	{
		DBColDesc.ulColumnSize=ColInfo.m_nChars;
	}
	if(ColInfo.m_pDBPropSet)
	{
		DBColDesc.cPropertySets=1;
		DBColDesc.rgPropertySets=ColInfo.m_pDBPropSet;
	}
	m_hr=pITableDefinition->AddColumn(&dbidTable, &DBColDesc, ppColID);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(pITableDefinition, __uuidof(ITableDefinition), &m_strError);
		return false;
	}
	return true;
}

bool	COSessionEx::AddConstraint(const CRefInfo &ThisInfo, const CRefInfo &RefInfo, LPOLESTR strKeyName, DBCONSTRAINTTYPE nConstraintType, DBUPDELRULE nUpdateRule, DBUPDELRULE nDeleteRule, DBMATCHTYPE nMatchType)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this object is already opened!");
	ULONG n;
	DBCONSTRAINTDESC		DBConstraintDesc;
	CComPtr<ITableDefinitionWithConstraints>	pITableDefinitionWithConstraints;
	m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(ITableDefinitionWithConstraints), (void**)&pITableDefinitionWithConstraints);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}
	memset(&DBConstraintDesc, 0, sizeof(DBCONSTRAINTDESC));
	OleDBPro_ASSERT(ThisInfo.m_strTblName, "\nThisInfo.m_strTblName can't be NULL!");
	OleDBPro_ASSERT(wcslen(ThisInfo.m_strTblName), "\nThisInfo.m_strTblName can't be zero in length!");
	DBID	dbidTbl;
	memset(&dbidTbl, 0, sizeof(DBID));
	dbidTbl.eKind=DBKIND_NAME;
	dbidTbl.uName.pwszName=ThisInfo.m_strTblName;
	
	DBID		dbidConstraintID;
	memset(&dbidConstraintID, 0, sizeof(DBID));
	dbidConstraintID.eKind=DBKIND_NAME;
	dbidConstraintID.uName.pwszName=strKeyName;
	
	DBID		dbidRefTbl;
	memset(&dbidRefTbl, 0, sizeof(DBID));
	dbidRefTbl.eKind=DBKIND_NAME;
	dbidRefTbl.uName.pwszName=RefInfo.m_strTblName;

	DBID		*pSrcTbl=(DBID*)CoTaskMemAlloc(sizeof(DBID)*ThisInfo.m_nCols);
	ATLASSERT(pSrcTbl);

	memset(pSrcTbl, 0, sizeof(DBID)*ThisInfo.m_nCols);
	for(n=0; n<ThisInfo.m_nCols; n++)
	{
		pSrcTbl[n].eKind=DBKIND_NAME;
		OleDBPro_ASSERT(((ThisInfo.m_pColNames)[n]), "\n((ThisInfo.m_pColNames)[n]) can't be NULL!");
		OleDBPro_ASSERT(wcslen(((ThisInfo.m_pColNames)[n])), "\nwcslen(((ThisInfo.m_pColNames)[n])) can't be zero in length!");
		pSrcTbl[n].uName.pwszName=((ThisInfo.m_pColNames)[n]);
	}
	DBID		*pRefTbl=(DBID*)CoTaskMemAlloc(sizeof(DBID)*RefInfo.m_nCols);
	ATLASSERT(pRefTbl);

	memset(pRefTbl, 0, sizeof(DBID)*RefInfo.m_nCols);
	for(n=0; n<RefInfo.m_nCols; n++)
	{
		pRefTbl[n].eKind=DBKIND_NAME;
		OleDBPro_ASSERT(((RefInfo.m_pColNames)[n]), "\n((RefInfo.m_pColNames)[n]) can't be NULL!");
		OleDBPro_ASSERT(wcslen(((RefInfo.m_pColNames)[n])), "\nwcslen(((RefInfo.m_pColNames)[n])) can't be zero in length!");
		pRefTbl[n].uName.pwszName=((RefInfo.m_pColNames)[n]);
	}
	if(strKeyName)
		DBConstraintDesc.pConstraintID=&dbidConstraintID;
	else
		DBConstraintDesc.pConstraintID=NULL;
	DBConstraintDesc.ConstraintType=nConstraintType;
	
	DBConstraintDesc.cColumns=ThisInfo.m_nCols;
	DBConstraintDesc.rgColumnList=pSrcTbl;
	
	if(RefInfo.m_nCols)
		DBConstraintDesc.pReferencedTableID=&dbidRefTbl;
	else
		DBConstraintDesc.pReferencedTableID=NULL;
	DBConstraintDesc.cForeignKeyColumns=RefInfo.m_nCols;
	DBConstraintDesc.rgForeignKeyColumnList=pRefTbl;
	
	if(nConstraintType==DBCONSTRAINTTYPE_CHECK)
		DBConstraintDesc.pwszConstraintText=strKeyName;
	else
		DBConstraintDesc.pwszConstraintText=NULL;
	DBConstraintDesc.UpdateRule=nUpdateRule;
	DBConstraintDesc.DeleteRule=nDeleteRule;
	DBConstraintDesc.MatchType=nMatchType;

	DBConstraintDesc.Deferrability=0;

	m_hr=pITableDefinitionWithConstraints->AddConstraint(&dbidTbl, &DBConstraintDesc);
	CoTaskMemFree(pRefTbl);
	CoTaskMemFree(pSrcTbl);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(pITableDefinitionWithConstraints, __uuidof(ITableDefinitionWithConstraints), &m_strError);
		return false;
	}
	return true;
}

bool COSessionEx::DropConstraint(LPOLESTR strTblName, LPOLESTR strKeyName)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this object is already opened!");
	OleDBPro_ASSERT(strTblName, "\nstrTblName can't be NULL!");
	OleDBPro_ASSERT(wcslen(strTblName), "\nstrTblName can't be zero in length!");
	OleDBPro_ASSERT(strKeyName, "\nstrKeyName can't be NULL!");
	OleDBPro_ASSERT(wcslen(strKeyName), "\nstrKeyName can't be zero in length!");

	CComPtr<ITableDefinitionWithConstraints>	pITableDefinitionWithConstraints;
	m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(ITableDefinitionWithConstraints), (void**)&pITableDefinitionWithConstraints);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}
	DBID	dbidTbl;
	DBID	dbidKey;
	memset(&dbidTbl, 0, sizeof(DBID));
	memset(&dbidKey, 0, sizeof(DBID));
	dbidTbl.eKind=DBKIND_NAME;
	dbidKey.eKind=DBKIND_NAME;
	dbidTbl.uName.pwszName=strTblName;
	dbidKey.uName.pwszName=strKeyName;
	m_hr=pITableDefinitionWithConstraints->DropConstraint(&dbidTbl, &dbidKey);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(pITableDefinitionWithConstraints, __uuidof(ITableDefinitionWithConstraints), &m_strError);
		return false;
	}
	return true;
}



CPureBase::CPureBase()
{
	m_nRowsAffected=0;
}

LPOLESTR CPureBase::GetDataSourceType(DBTYPE nDBType)
{
	LPOLESTR	bstr=0;
	switch (nDBType)
	{
	case DBTYPE_I1|DBTYPE_BYREF:
	case DBTYPE_I1:
		bstr=L"DBTYPE_I1";
		break;
	case DBTYPE_I2|DBTYPE_BYREF:
	case DBTYPE_I2:
		bstr=L"DBTYPE_I2";
		break;
	case DBTYPE_I4:
	case DBTYPE_I4|DBTYPE_BYREF:
		bstr=L"DBTYPE_I4";
		break;
	case DBTYPE_I8:
	case DBTYPE_I8|DBTYPE_BYREF:
		bstr=L"DBTYPE_I8";
		break;
	case DBTYPE_UI1:
	case DBTYPE_UI1|DBTYPE_BYREF:
		bstr=L"DBTYPE_UI1";
		break;
	case DBTYPE_UI2:
	case DBTYPE_UI2|DBTYPE_BYREF:
		bstr=L"DBTYPE_UI2";
		break;
	case DBTYPE_UI4:
	case DBTYPE_UI4|DBTYPE_BYREF:
		bstr=L"DBTYPE_UI4";
		break;
	case DBTYPE_UI8:
	case DBTYPE_UI8|DBTYPE_BYREF:
		bstr=L"DBTYPE_UI8";
		break;
	case DBTYPE_R4:
	case DBTYPE_R4|DBTYPE_BYREF:
		bstr=L"DBTYPE_R4";
		break;
	case DBTYPE_R8:
	case DBTYPE_R8|DBTYPE_BYREF:
		bstr=L"DBTYPE_R8";
		break;
	case DBTYPE_CY:
	case DBTYPE_CY|DBTYPE_BYREF:
		bstr=L"DBTYPE_CY";
		break;
	case DBTYPE_DECIMAL:
	case DBTYPE_DECIMAL|DBTYPE_BYREF:
		bstr=L"DBTYPE_DECIMAL";
		break;
	case DBTYPE_NUMERIC:
	case DBTYPE_NUMERIC|DBTYPE_BYREF:
		bstr=L"DBTYPE_NUMERIC";
		break;
	case DBTYPE_BOOL:
	case DBTYPE_BOOL|DBTYPE_BYREF:
		bstr=L"DBTYPE_BOOL";
		break;
	case DBTYPE_UDT:
	case DBTYPE_UDT|DBTYPE_BYREF:
		bstr=L"DBTYPE_UDT";
		break;
	case DBTYPE_VARIANT:
	case DBTYPE_VARIANT|DBTYPE_BYREF:
		bstr=L"DBTYPE_VARIANT";
		break;
	case DBTYPE_GUID:
	case DBTYPE_GUID|DBTYPE_BYREF:
		bstr=L"DBTYPE_GUID";
		break;
	case DBTYPE_DATE:
	case DBTYPE_DATE|DBTYPE_BYREF:
		bstr=L"DBTYPE_DATE";
		break;
	case DBTYPE_DBDATE:
	case DBTYPE_DBDATE|DBTYPE_BYREF:
		bstr=L"DBTYPE_DBDATE";
		break;
	case DBTYPE_DBTIME:
	case DBTYPE_DBTIME|DBTYPE_BYREF:
		bstr=L"DBTYPE_DBTIME";
		break;
	case DBTYPE_DBTIMESTAMP:
	case DBTYPE_DBTIMESTAMP|DBTYPE_BYREF:
		bstr=L"DBTYPE_DBTIMESTAMP";
		break;
    case DBTYPE_FILETIME:
    case DBTYPE_FILETIME|DBTYPE_BYREF:
        bstr=L"DBTYPE_FILETIME";
        break;
	case DBTYPE_BSTR:
	case DBTYPE_BSTR|DBTYPE_BYREF:
		bstr=L"DBTYPE_BSTR";
		break;
	case DBTYPE_STR:
	case DBTYPE_STR|DBTYPE_BYREF:
		bstr=L"DBTYPE_VARCHAR";
		break;
	case DBTYPE_WSTR:
	case DBTYPE_WSTR|DBTYPE_BYREF:
		bstr=L"DBTYPE_WVARCHAR";
		break;
	case DBTYPE_BYTES:
	case DBTYPE_BYTES|DBTYPE_BYREF:
		bstr=L"DBTYPE_VARBINARY";
		break;
	case DBTYPE_VARNUMERIC:
	case DBTYPE_VARNUMERIC|DBTYPE_BYREF:
		bstr=L"DBTYPE_VARNUMERIC";
		break;
	case DBTYPE_IUNKNOWN:
	case DBTYPE_IUNKNOWN|DBTYPE_BYREF:
		bstr=L"DBTYPE_IUNKNOWN";
		break;
	case DBTYPE_SQLVARIANT:
	case DBTYPE_SQLVARIANT|DBTYPE_BYREF:
		bstr=L"DBTYPE_SQLVARIANT";
		break;
	default:
		OleDBPro_ASSERT(false, "\nOleDBPro doesn't support this data type!");
		break;
	}
	return bstr;
}

const ULONG	*CPureBase::GetFixLen(DBTYPE nDBType)
{
	switch (nDBType)
	{
	case DBTYPE_I1:
		return &g_nSChar;
	case DBTYPE_I2:
		return &g_nShort;
	case DBTYPE_I4:
		return &g_nLong;
	case DBTYPE_I8:
		return &g_nLInteger;
	case DBTYPE_UI1:
		return &g_nByte;
	case DBTYPE_UI2:
		return &g_nUShort;
	case DBTYPE_UI4:
		return &g_nUInt;
	case DBTYPE_UI8:
		return &g_nULInteger;
	case DBTYPE_R4:
		return &g_nFloat;
	case DBTYPE_R8:
		return &g_nDouble;
	case DBTYPE_CY:
		return &g_nCy;
	case DBTYPE_DECIMAL:
		return &g_nDecimal;
	case DBTYPE_NUMERIC:
		return &g_nNumeric;
	case DBTYPE_BOOL:
		return &g_nBool;
	case DBTYPE_VARIANT:
		return &g_nVariant;
	case DBTYPE_GUID:
		return &g_nGuid;
	case DBTYPE_DATE:
		return &g_nDate;
	case DBTYPE_DBDATE:
		return &g_nDBDate;
	case DBTYPE_DBTIME:
		return &g_nDBTime;
	case DBTYPE_DBTIMESTAMP:
		return &g_nDBTimeStamp;
	case DBTYPE_BSTR:
		return &g_nBstr;
	case DBTYPE_FILETIME:
		return &g_nFileTime;
	case DBTYPE_HCHAPTER:
		return &g_nHChapter;
	case DBTYPE_PROPVARIANT:
		return &g_nPropVariant;
	case DBTYPE_IUNKNOWN:
		return &g_nByRef;
	case DBTYPE_SQLVARIANT:	//sql server 2k specific data type
		return &g_nSQLVariant; 
	default:
		if((nDBType & DBTYPE_BYREF)==DBTYPE_BYREF)
			return &g_nByRef;
		else if((nDBType & DBTYPE_ARRAY)==DBTYPE_ARRAY)
			return &g_nArray;
		else if ((nDBType & DBTYPE_VECTOR)==DBTYPE_VECTOR)
			return &g_nVector;
		else
			return NULL;
		break;
	}
	return NULL;
}

void GetOLEErrorInfo(IUnknown* pObjectWithError, REFIID IID_InterfaceWithError, BSTR	*pstrErrInfo)
{
#ifdef _DEBUG
	USES_CONVERSION;
#endif
	HRESULT				hr=S_OK;
	CComPtr<ISupportErrorInfo>	pISupportErrorInfo;
	CComPtr<IErrorInfo>			pIErrorInfo;
	CComPtr<IErrorRecords>		pIErrorRecords;
	
	CComBSTR		strErr;
	if(pObjectWithError)
	{
		hr = pObjectWithError->QueryInterface(__uuidof(ISupportErrorInfo), (void**)&pISupportErrorInfo);
		if (FAILED(hr)) 
		{
			strErr=L"Interface ISupportErrorInfo not available";
			goto Cleanup;
		}
		if(IID_InterfaceWithError!=IID_NULL)
		{
			hr = pISupportErrorInfo->InterfaceSupportsErrorInfo(IID_InterfaceWithError);
			if (FAILED(hr)) 
			{
				strErr=L"Interface doesn't support error objects";
				goto Cleanup;
			}
		}
	}

	if (GetErrorInfo(0,	&pIErrorInfo)!=S_OK) 
	{
		strErr=L"Unknown error";
		goto Cleanup;
	}

	hr=pIErrorInfo->QueryInterface(__uuidof(IErrorRecords), (void**) &pIErrorRecords);
	if(FAILED(hr) || pIErrorRecords==0)
	{
		CComBSTR	strDescription;
		pIErrorInfo->GetSource(&strErr);
		strErr +=CComBSTR(L": ");
		pIErrorInfo->GetDescription(&strDescription);
		strErr +=strDescription;
	}
	else
	{
		ERRORINFO			ErrorInfo;
		ULONG					i, ulNumErrorRecs=0;
		CComPtr<IErrorInfo>	pIErrorInfoRec;
		LCID 		MYLOCALEID=GetSystemDefaultLCID();
		hr=pIErrorRecords->GetRecordCount(&ulNumErrorRecs);
		for (i = 0; i < ulNumErrorRecs; i++) 
		{
			CComBSTR	bstrDescription;
			CComBSTR bstrSource;
			CComPtr<ISQLErrorInfo>	pISQLErrorInfo;
			
			pIErrorRecords->GetBasicErrorInfo(i, &ErrorInfo);
			pIErrorRecords->GetErrorInfo(i, MYLOCALEID, &pIErrorInfoRec);
		
			pIErrorInfoRec->GetDescription(&bstrDescription); 
			pIErrorInfoRec->GetSource(&bstrSource);
			if(strErr.Length()) strErr +=L"; ";
			strErr +=bstrSource;
			strErr +=": ";
			strErr +=bstrDescription;
			hr=pIErrorRecords->GetCustomErrorObject(i, __uuidof(ISQLErrorInfo), (IUnknown**)&(pISQLErrorInfo.p));
			if(!FAILED(hr) && pISQLErrorInfo)
			{
				char			strNum[11]={0};
				LONG			nNativeError=0;
				CComBSTR		bstrSQLState;
				pISQLErrorInfo->GetSQLInfo(&bstrSQLState, &nNativeError);
				strErr +=CComBSTR(L"\\");
				strErr +=bstrSQLState;
				strErr +=CComBSTR(L" = ");
				ltoa(nNativeError, strNum, 10);
				strErr +=CComBSTR(strNum);
			}
			pISQLErrorInfo.Release();
			pIErrorInfoRec.Release();
		}
	}
Cleanup:
#ifdef _DEBUG
	{
		CComBSTR bstrErrTrace=strErr;
		bstrErrTrace +=CComBSTR(L"     ");
		bstrErrTrace +=CComBSTR(IID_InterfaceWithError);
		bstrErrTrace +=CComBSTR(L"\n");
		if(bstrErrTrace.Length()<255)
			ATLTRACE(W2A(bstrErrTrace));
	}
#endif
	if(*pstrErrInfo) 
		::SysFreeString(*pstrErrInfo);
	*pstrErrInfo=strErr.m_str;
	strErr.m_str=NULL;
}

void	PrepareProps(COPropSet &DBPropSet, int nCursor, bool bReadOnly, bool bBatch, bool bScrollback, bool bBookmark)
{
	OleDBPro_ASSERT(nCursor>=ForwardOnly, "\nMake sure a valid cursor is given!");
	OleDBPro_ASSERT(nCursor<=Dynamic, "\nMake sure a valid cursor is given!");
	DBPropSet.guidPropertySet=DBPROPSET_ROWSET;
	if(nCursor!=ForwardOnly)
	{
		if(bScrollback) DBPropSet.AddProperty(DBPROP_CANSCROLLBACKWARDS, true);
		if(!bReadOnly) DBPropSet.AddProperty(DBPROP_IRowsetChange, true);
		if(bBatch) 
		{
			DBPropSet.AddProperty(DBPROP_IRowsetUpdate, true);
		}
		if(!bReadOnly || bBatch) 
			DBPropSet.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE|DBPROPVAL_UP_DELETE|DBPROPVAL_UP_INSERT);
	}
	switch (nCursor)
	{
	case ForwardOnly:
		break;
	case Static:
		if(bBookmark) 
		{
			DBPropSet.AddProperty(DBPROP_BOOKMARKS, true);
			DBPropSet.AddProperty(DBPROP_IRowsetLocate, true);
		}
		break;
	case Keyset:
		if(bBookmark) 
		{
			DBPropSet.AddProperty(DBPROP_BOOKMARKS, true);
			DBPropSet.AddProperty(DBPROP_IRowsetLocate, true);
		}
		DBPropSet.AddProperty(DBPROP_OTHERUPDATEDELETE, true);
		break;
	case Dynamic:
		DBPropSet.AddProperty(DBPROP_OTHERUPDATEDELETE, true);
		DBPropSet.AddProperty(DBPROP_OTHERINSERT, true);
		break;
	default:
		OleDBPro_ASSERT(false, "\nMake sure a valid cursor is given!");
	}
	DBPropSet.AddProperty(DBPROP_ISupportErrorInfo, true);
	DBPropSet.AddProperty(DBPROP_ISequentialStream, true);
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