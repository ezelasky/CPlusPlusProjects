// ADO.cpp: implementation of the CADO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ADO.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CADO::CADO(_bstr_t bstrServer, _bstr_t bstrDatabase)
{
	m_bstrServer = bstrServer;
	m_bstrDatabase = bstrDatabase;
}

CADO::~CADO()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//	Connection related methods
//	
//	These methods are concerned with managing the connection object stored in m_spConnection
//
//////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CADO::OpenTrustedConnection(_bstr_t bstrSQLServer, _bstr_t bstrSQLDatabase)
{
	HRESULT hrResult = E_FAIL;
	m_listErrors.clear();
	
	try {

		if (wcscmp(bstrSQLServer, L"") == 0)
			bstrSQLServer = m_bstrServer;

		if(wcscmp(bstrSQLDatabase, L"") == 0)
			bstrSQLDatabase = m_bstrDatabase;

		ADODB::_ConnectionPtr spConnection;
//		_bstr_t bConnStr = "DRIVER={SQL Server}; SERVER=SOCRATES; DATABASE=DataTransTest; Trusted_Connection=Yes;";

		_bstr_t bConnStr = L"DRIVER={SQL Server}; SERVER=";
		bConnStr += bstrSQLServer;
		bConnStr += L"; DATABASE=";
		bConnStr += bstrSQLDatabase;
		bConnStr += L"; Trusted_Connection=Yes;";

		spConnection.CreateInstance(__uuidof(ADODB::Connection));
		spConnection->PutCommandTimeout(0); 
		spConnection->Open( bConnStr, _bstr_t(), _bstr_t(), -1);

		if (spConnection) {
			m_spConnection = spConnection;
			hrResult = S_OK;
		}
	}
	catch(...) {
		hrResult = E_FAIL;
	}

	return hrResult;
}

HRESULT CADO::CloseConnection()
{
	HRESULT hrRetval = S_OK;
	m_listErrors.clear();

	try {

		if (m_spConnection)
			m_spConnection->Close();

		m_spConnection = NULL;
	}
	//catch(_com_error &e)
	catch(...)
	{

		hrRetval = E_FAIL;
	}

	return hrRetval;
}
int CADO::LoadErrorsFromConnection()
{
	ADODB::ErrorsPtr	pErrors = NULL;
	ADODB::ErrorPtr    pError  = NULL;
	
	HRESULT		hr = (HRESULT) 0L;
   	pErrors = m_spConnection->GetErrors();
	int iCount = pErrors->GetCount();
			
	if (pErrors != NULL ) {

		for ( long i = 0; (!FAILED(hr)) && i < iCount; i++ )
		{
			HRESULT hr = pErrors->get_Item( (_variant_t) ((long)i), &pError);
			m_listErrors.push_back((_bstr_t) pError->Description); 
		}
	}

	return iCount;
}

int CADO::LoadErrorsFromConnection(ADODB::_ConnectionPtr spConnection)
{
	ADODB::ErrorsPtr	pErrors = NULL;
	ADODB::ErrorPtr    pError  = NULL;
	
	HRESULT		hr = (HRESULT) 0L;
   	pErrors = spConnection->GetErrors();
	int iCount = pErrors->GetCount();
			
	if (pErrors != NULL ) {

		for ( long i = 0; (!FAILED(hr)) && i < iCount; i++ )
		{
			HRESULT hr = pErrors->get_Item( (_variant_t) ((long)i), &pError);
			m_listErrors.push_back((_bstr_t) pError->Description); 
		}
	}

	return iCount;



}


///////////////////////////////////////////////////////////////////////////////////////////////////
//	Methods working off of m_spConnection 
//	
//	Note: For a variety of reasons, it made sense to allow the calling module to control the 
//	connection, so these methods work off an established connection, m_spConnection.
//
//////////////////////////////////////////////////////////////////////////////////////////////////

HRESULT CADO::ExecuteQuery(_bstr_t sQry)
{
	HRESULT hrRetval = E_FAIL;
	m_listErrors.clear();

	try {
		if ( m_spConnection != NULL ) 
			hrRetval = m_spConnection->Execute(sQry, NULL, -1);
		else 
			m_listErrors.push_back(L"DTRAN ERROR: CADO::ExecuteQuery - the database connection has not been established.");
	}
	catch(...)
	{
		if (LoadErrorsFromConnection() == 0)
			m_listErrors.push_back(L"DTRAN ERROR: CADO::ExecuteQuery - caught in catch statement.  No errors available off of ADO connection.");
	}

	return hrRetval;
}

HRESULT CADO::GetRecordSet(IDispatch** pp_Recordset, _bstr_t sQry)
{
	HRESULT hrRetval = E_FAIL;
	m_listErrors.clear();

	try{
			if ( m_spConnection != NULL ) {
	
			ADODB::_RecordsetPtr spRecordset;
			spRecordset.CreateInstance(__uuidof(ADODB::adoRecordset));
			spRecordset->PutCursorLocation(ADODB::adUseClient);

			spRecordset->PutRefActiveConnection(m_spConnection);
			hrRetval = spRecordset->Open( sQry,
							   _variant_t(DISP_E_PARAMNOTFOUND, VT_ERROR),
							   static_cast <ADODB::CursorTypeEnum> (ADODB::adOpenStatic),
							   static_cast <ADODB::LockTypeEnum> (ADODB::adLockBatchOptimistic),
							    -1);
		
			if(spRecordset->GetRecordCount())
				spRecordset->MoveFirst();

			*pp_Recordset= IDispatchPtr(spRecordset).Detach();

		}
		else 
			m_listErrors.push_back(L"DTRAN ERROR: CADO::GetRecordSet - the database connection has not been established.");
	}
	catch(...)
	{
		if (LoadErrorsFromConnection() == 0)
			m_listErrors.push_back(L"DTRAN ERROR: CADO::GetRecordSet - caught in catch statement.  No errors available off of ADO connection.");
		hrRetval = E_FAIL;
	}

	return hrRetval;
}

HRESULT CADO::CreateTableFromQuery(_bstr_t bstrQuery, LPTSTR pzNameIn, bool bCreateTemp)
{  //should this really be in deSQL??
	HRESULT hrRetval = E_FAIL;
	m_listErrors.clear();
	LPTSTR pzTableName = NULL;
	USES_CONVERSION;

	try{
		if ( m_spConnection != NULL ) {

			if (_tcslen(pzNameIn) == 0 ) {
				LPTSTR pzName = NULL;
				if (bCreateTemp)
					pzName = _T("##Temp_");
				else
					pzName = _T("Temp_");
				LPTSTR ptrName = Utilities::ConstructUniqueName(pzName);
				pzTableName = ptrName;
				_tcscpy(pzNameIn, pzTableName);
			}
			else
				pzTableName = pzNameIn;

			int iLen = bstrQuery.length() + _tcslen(sz_CreateTableFromQuery) + _tcslen(pzTableName) + 1;
			TCHAR* pzQuery = new TCHAR[iLen];
			memset(pzQuery, 0, iLen);
			_sntprintf(pzQuery, iLen, sz_CreateTableFromQuery, pzNameIn, W2T(bstrQuery));

			_bstr_t bstrTemp(pzQuery);
			hrRetval = ExecuteQuery(bstrTemp);
			if (!SUCCEEDED(hrRetval) ) 
				m_listErrors.push_back(L"DTRAN ERROR: CADO::CreateTableFromQuery() -- Export query did not construct table.");

		}
		else 
			m_listErrors.push_back(L"DTRAN ERROR: CADO::ExecuteQuery - the database connection has not been established.");
	}
	catch(...)
	{
		if (LoadErrorsFromConnection() == 0)
			m_listErrors.push_back(L"DTRAN ERROR: CADO::CreateTableFromQuery - caught in catch statement.  No errors available off of ADO connection.");
		hrRetval = E_FAIL;
	}

	return hrRetval;

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//	Methods that open and close a connection and perform a single operation.
//////////////////////////////////////////////////////////////////////////////////////////////////
bool  CADO::ColumnExists(_bstr_t bstrTable, _bstr_t bstrColumn, _bstr_t bstrSQLServer, _bstr_t bstrSQLDatabase )
{
	bool bExists = false;
	bool bNewConn = false;
	m_listErrors.clear();

	try {
		//most of the time this will be called from an open connection, but for now...
		if ( wcscmp(bstrSQLServer, L"") != 0 && wcscmp(bstrSQLDatabase, L"") != 0	 )  {
			if (SUCCEEDED(OpenTrustedConnection(bstrSQLServer, bstrSQLDatabase)))
				bNewConn = true;
		}

		TCHAR szTable[BUFF_MED]; memset(szTable, 0, BUFF_MED);
		int iStrLen =  SysStringLen(bstrTable);
		WideCharToMultiByte(CP_ACP, 0, bstrTable, iStrLen, szTable, iStrLen+1, NULL, NULL);
		szTable[iStrLen]= '\0';

		TCHAR szCol[BUFF_MED]; memset(szCol, 0, BUFF_MED);
		iStrLen =  SysStringLen(bstrColumn);
		WideCharToMultiByte(CP_ACP, 0, bstrColumn, iStrLen, szCol, iStrLen+1, NULL, NULL);
		szCol[iStrLen]= '\0';

		TCHAR szQuery[MAX_QUERY_LEN];  memset(szQuery, 0, MAX_QUERY_LEN);
		_stprintf(szQuery, sz_SchemaForColumnSQL, szTable, szCol);
	
		ADODB::_RecordsetPtr spRecordset = NULL;
		if (SUCCEEDED(GetRecordSet((IDispatch**) &spRecordset, szQuery)) ) {
			if ( spRecordset != NULL) {
				if (spRecordset->RecordCount != 0 )
					bExists = true;
			}
		}

		if (bNewConn)
			CloseConnection();

	}
	catch(...) {
		if (LoadErrorsFromConnection() == 0)
			m_listErrors.push_back(L"DTRAN ERROR: CADO::ColumnExists - caught in catch statement.  No errors available off of ADO connection.");
		if (bNewConn)
			CloseConnection();
	}

	return bExists;
}


bool  CADO::ColumnTextLengthVaries(_bstr_t bstrTable, _bstr_t bstrColumn, _bstr_t bstrSQLServer, _bstr_t bstrSQLDatabase )
{
	bool bVaries = false;
	bool bNewConn = false;
	m_listErrors.clear();

	try {
		//most of the time this will be called from an open connection, but for now...
		if ( wcscmp(bstrSQLServer, L"") != 0 && wcscmp(bstrSQLDatabase, L"") != 0	 )  {
			if (SUCCEEDED(OpenTrustedConnection(bstrSQLServer, bstrSQLDatabase)))
				bNewConn = true;
		}

		TCHAR szTable[BUFF_MED]; memset(szTable, 0, BUFF_MED);
		int iStrLen =  SysStringLen(bstrTable);
		WideCharToMultiByte(CP_ACP, 0, bstrTable, iStrLen, szTable, iStrLen+1, NULL, NULL);
		szTable[iStrLen]= '\0';

		TCHAR szCol[BUFF_MED]; memset(szCol, 0, BUFF_MED);
		iStrLen =  SysStringLen(bstrColumn);
		WideCharToMultiByte(CP_ACP, 0, bstrColumn, iStrLen, szCol, iStrLen+1, NULL, NULL);
		szCol[iStrLen]= '\0';

		TCHAR szQuery[MAX_QUERY_LEN];  memset(szQuery, 0, MAX_QUERY_LEN);
		_stprintf(szQuery, sz_SchemaForColumnSQL, szTable, szCol);
	
		ADODB::_RecordsetPtr spRecordset = NULL;
		if (SUCCEEDED(GetRecordSet((IDispatch**) &spRecordset, szQuery)) ) {
			if ( spRecordset != NULL) {
				if (spRecordset->RecordCount != 0 ) {

					spRecordset->MoveFirst();
					_variant_t vNull = spRecordset->Fields->GetItem("IS_NULLABLE")->Value;
					_variant_t vType = spRecordset->Fields->GetItem("DATA_TYPE")->Value;
					
					if (vType.vt != VT_EMPTY && vType.vt != VT_NULL && 
						vNull.vt != VT_EMPTY && vNull.vt != VT_EMPTY   ) {
						
						if ( wcsnicmp(vType.bstrVal, L"varchar", wcslen(L"varchar") ) == 0 ||
							 wcsnicmp(vNull.bstrVal, L"YES", wcslen(L"YES")         ) == 0 ||
							 wcsnicmp(vType.bstrVal, L"int", wcslen(L"int")         ) == 0   )
								bVaries = true;
					}
				}
			}
		}

		if (bNewConn)
			CloseConnection();

	}
	catch(...) {
		if (LoadErrorsFromConnection() == 0)
			m_listErrors.push_back(L"DTRAN ERROR: CADO::ColumnTextLengthVaries - caught in catch statement.  No errors available off of ADO connection.");
		if (bNewConn)
			CloseConnection();
	}

	return bVaries;
}


bool CADO::TableExists(_bstr_t bstrSQLServer, _bstr_t bstrSQLDatabase, _bstr_t bstrTable)
{
	//TBD ... rewrite this to look like ColumnExists...
	bool bExists = false;
	m_listErrors.clear();

	try {

		ADODB::_ConnectionPtr spConnection = NULL;

		_bstr_t bConnStr = L"DRIVER={SQL Server}; SERVER=";
		bConnStr += bstrSQLServer;
		bConnStr += L"; DATABASE=";
		bConnStr += bstrSQLDatabase;
		bConnStr += L"; Trusted_Connection=Yes;";

		spConnection.CreateInstance(__uuidof(ADODB::Connection));
		spConnection->PutCommandTimeout(0); 
		spConnection->Open( bConnStr, _bstr_t(), _bstr_t(), -1);

		if (spConnection != NULL) {

			ADODB::_RecordsetPtr spRecordset = NULL;

			TCHAR szTable[BUFF_MED]; memset(szTable, 0, BUFF_MED);
			int iStrLen =  SysStringLen(bstrTable);
			WideCharToMultiByte(CP_ACP, 0, bstrTable, iStrLen, szTable, iStrLen+1, NULL, NULL);
			szTable[iStrLen]= '\0';

			TCHAR szQuery[MAX_QUERY_LEN];  memset(szQuery, 0, MAX_QUERY_LEN);
			_stprintf(szQuery, sz_InfoSchemaOnTable, szTable);

			spRecordset.CreateInstance(__uuidof(ADODB::adoRecordset));
			spRecordset->PutCursorLocation(ADODB::adUseClient);

			//..............................................
			// connect recordset to selected connection, 
			// populate and position cursor, then disconnect
			//..............................................

			spRecordset->PutRefActiveConnection(spConnection);
			_bstr_t bstrTemp(szQuery);
			HRESULT hrRetval = spRecordset->Open( bstrTemp,
						   _variant_t(DISP_E_PARAMNOTFOUND, VT_ERROR),
						   static_cast <ADODB::CursorTypeEnum> (ADODB::adOpenStatic),
						   static_cast <ADODB::LockTypeEnum> (ADODB::adLockBatchOptimistic),
						    -1);
			
			if (SUCCEEDED(hrRetval) ) {
				if ( spRecordset != NULL) {
						if (spRecordset->RecordCount != 0 )
							bExists = true;
					}
			}
		}
		spConnection->Close();
	}
	catch (...) {
		bExists = false;

	}
	return bExists;
}

HRESULT CADO::DropTable(_bstr_t bstrSQLServer, _bstr_t bstrSQLDatabase, _bstr_t bstrTable)
{
	HRESULT hrResult = E_FAIL;
	m_listErrors.clear();
	bool bNewConn = false;

	try {
		//most of the time this will be called from an open connection, but for now...
		if ( wcscmp(bstrSQLServer, L"") != 0 && wcscmp(bstrSQLDatabase, L"") != 0	 )  {
			if (SUCCEEDED(OpenTrustedConnection(bstrSQLServer, bstrSQLDatabase)))
				bNewConn = true;
		}

		TCHAR szTable[BUFF_MED]; memset(szTable, 0, BUFF_MED);
		int iStrLen =  SysStringLen(bstrTable);
		WideCharToMultiByte(CP_ACP, 0, bstrTable, iStrLen, szTable, iStrLen+1, NULL, NULL);
		szTable[iStrLen]= '\0';

		TCHAR szQuery[MAX_QUERY_LEN];  memset(szQuery, 0, MAX_QUERY_LEN);
		_stprintf(szQuery, "DROP TABLE %s", szTable);
	
		hrResult = ExecuteQuery(szQuery);
		if (!SUCCEEDED(hrResult) ) 
				m_listErrors.push_back(L"DTRAN ERROR: CADO::DropTable - Table was not dropped.");

		if (bNewConn)
			CloseConnection();

	}
	catch(...) {
		if (LoadErrorsFromConnection() == 0)
			m_listErrors.push_back(L"DTRAN ERROR: CADO::DropTable - caught in catch statement.  No errors available off of ADO connection.");
		if (bNewConn)
			CloseConnection();
	}

	return hrResult;
}

_bstr_t CADO::GetListofFieldNames(bstr_t bstrSQLServer, _bstr_t bstrSQLDatabase, _bstr_t bstrTable, int& iNumFields)
{
	//returns a comma delimited list of field names for input table
	_bstr_t bstrList = L"";
	m_listErrors.clear();
	int iFieldCnt = 0;
	bool bNewConn = false;

	try {
		//most of the time this will be called from an open connection, but for now...
		if ( wcscmp(bstrSQLServer, L"") != 0 && wcscmp(bstrSQLDatabase, L"") != 0	 )  {
			if (SUCCEEDED(OpenTrustedConnection(bstrSQLServer, bstrSQLDatabase)))
				bNewConn = true;
		}

		TCHAR szTable[BUFF_MED]; memset(szTable, 0, BUFF_MED);
		int iStrLen =  SysStringLen(bstrTable);
		WideCharToMultiByte(CP_ACP, 0, bstrTable, iStrLen, szTable, iStrLen+1, NULL, NULL);
		szTable[iStrLen]= '\0';

		TCHAR szQuery[MAX_QUERY_LEN];  memset(szQuery, 0, MAX_QUERY_LEN);
		_stprintf(szQuery, sz_InfoSchemaOnTable, szTable);
	
		ADODB::_RecordsetPtr spRecordset = NULL;
		if (SUCCEEDED(GetRecordSet((IDispatch**) &spRecordset, szQuery)) ) {
			if ( spRecordset != NULL) {
				if (spRecordset->RecordCount != 0 ) {
					spRecordset->MoveFirst();
					while (!spRecordset->adoEOF) {
						_variant_t vName = spRecordset->Fields->GetItem("COLUMN_NAME")->Value;
						_variant_t vType = spRecordset->Fields->GetItem("DATA_TYPE")->Value;
						if (vName.vt != VT_EMPTY && vName.vt != VT_NULL  ) {
							if (iFieldCnt != 0 )
								bstrList += ",";
							bstrList += vName.bstrVal;
							iFieldCnt++;
						}
						spRecordset->MoveNext();
					}
				}
			}
		}

		if (bNewConn)
			CloseConnection();

	}
	catch(...) {
		if (LoadErrorsFromConnection() == 0)
			m_listErrors.push_back(L"DTRAN ERROR: CADO::ColumnTextLengthVaries - caught in catch statement.  No errors available off of ADO connection.");
		if (bNewConn)
			CloseConnection();
	}
	iNumFields = iFieldCnt;
    return bstrList.copy();
}

//*******************************************************************************************
//	not used or tested much 
//*******************************************************************************************
HRESULT CADO::GetTrustedConnection(IDispatch** pp_Connection, _bstr_t bstrSQLServer, _bstr_t bstrSQLDatabase)
{
	HRESULT hrResult = E_FAIL;
	m_listErrors.clear();
	
	try {

		if (wcscmp(bstrSQLServer, L"") == 0)
			bstrSQLServer = m_bstrServer;

		if(wcscmp(bstrSQLDatabase, L"") == 0)
			bstrSQLDatabase = m_bstrDatabase;

		ADODB::_ConnectionPtr spConnection;
//		_bstr_t bConnStr = "DRIVER={SQL Server}; SERVER=SOCRATES; DATABASE=DataTransTest; Trusted_Connection=Yes;";

		_bstr_t bConnStr = L"DRIVER={SQL Server}; SERVER=";
		bConnStr += bstrSQLServer;
		bConnStr += L"; DATABASE=";
		bConnStr += bstrSQLDatabase;
		bConnStr += L"; Trusted_Connection=Yes;";

		spConnection.CreateInstance(__uuidof(ADODB::Connection));
		spConnection->PutCommandTimeout(0); 
		spConnection->Open( bConnStr, _bstr_t(), _bstr_t(), -1);

		if (spConnection) {
			*pp_Connection = IDispatchPtr(spConnection).Detach();
			hrResult = S_OK;
		}
	}
	catch(...) {
		hrResult = E_FAIL;
	}

	return hrResult;

}
