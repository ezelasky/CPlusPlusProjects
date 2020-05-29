// BaseTransfer.cpp: implementation of the CBaseTransfer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "COMSink.h"
#include "BaseTransfer.h"

using namespace std;
#define SMALL_BUF_SIZE 1000

//////////////////////////////////////////////////////////////////////
//
// CBaseTransfer  -- abstract class for all transfers
//
//////////////////////////////////////////////////////////////////////

CBaseTransfer::CBaseTransfer(_bstr_t bstrSQLServer,
				             _bstr_t bstrSQLDatabase,
				             _bstr_t bstrSQLTable,
							 bool bImportToSQL)
{
	m_bstrSQLServer = bstrSQLServer;
	m_bstrSQLDatabase = bstrSQLDatabase;
	m_bstrSQLTable = bstrSQLTable;
	
	m_bInitCalled = false;
	m_bFatalErrs = false;
	//m_bTrunWarn = false;

	m_bstrSQLOwner = L"dbo";
	m_bstrTraceFileName = L"";
	m_bstrFatalErrMsgs = L"";
	m_lRowsLoaded = 0;
	m_lMaxErrMsgs = 100;
	m_lFirst = 1;
	m_lLast = 0;
	m_bImportToSQL = bImportToSQL;
	m_bDSN = L"";

	m_nAction = nCREATE;
	ptrLog = NULL;
	ptrADO =  new CADO();
}  //CBaseTransfer::CBaseTransfer

CBaseTransfer::~CBaseTransfer()
{
	/*	ptrLog->Close();
	if (ptrLog)
		delete ptrLog;
	ptrLog = NULL;

	*/

	if (ptrADO != NULL) {
		ptrADO->CloseConnection();
		delete ptrADO;
	}

	ptrADO = NULL;
} //CBaseTransfer::~CBaseTransfer

HRESULT CBaseTransfer::Init(bool bTrace)
{
	HRESULT hr = E_FAIL;
	m_bInitCalled = true;

	try
	{
		if (bTrace) 
		{
			ptrLog = new CLog();
			ptrLog->SetLineLength(150);

			if ( wcscmp(m_bstrTraceFileName, L"") == 0 )
			{

				TCHAR BufDir[512];
				GetCurrentDirectory(512, BufDir);

				if ( ptrLog->Open("DTran_trace", BufDir, NULL) == 0)
					ptrLog = NULL;
				else
					m_bstrTraceFileName = ptrLog->GetLogName(); //for multifile load....
			}
			else
			{
				TCHAR szTraceFileName[BUFF_MED]; memset(szTraceFileName, 0, BUFF_MED);
				int iStrLen =  SysStringLen(m_bstrTraceFileName);
				WideCharToMultiByte(CP_ACP, 0, m_bstrTraceFileName, iStrLen, szTraceFileName, iStrLen+1, NULL, NULL);
				szTraceFileName[iStrLen]= '\0';

				if ( ptrLog->Open(NULL, NULL, szTraceFileName) == 0)
					ptrLog = NULL;

			}

		}
	}
	catch(...)
	{
		SetFatalError(L"DTRAN ERROR:  Log File could not be opened.");
	}

	m_bFileLoc = Utilities::ConstructFullPath(m_bDir, m_bFile);
	LogInputParams();
	hr = CheckInputParams();
	if (SUCCEEDED(hr))
		hr = _CheckInputParams();
	
	if (SUCCEEDED(hr)) {

		hr = E_FAIL;
		if (ptrADO != NULL)
			hr = ptrADO->OpenTrustedConnection(m_bstrSQLServer, m_bstrSQLDatabase);
		else
			SetFatalError(L"DTRAN ERROR: Something is amiss with ptrADO.");
	
		if (!SUCCEEDED(hr)) {
			_bstr_t bstrErr = L"DTRAN ERROR: Database ";
			bstrErr += m_bstrSQLDatabase;
			bstrErr += L" does not exist on server: ";
			bstrErr += m_bstrSQLServer;
			SetFatalError(bstrErr);
		}

	}

	if (!m_bImportToSQL && !ErrorsOccurred())  { 
		ConfirmSQLTable(nCONFIRM, L""); 
	}

	return hr;
} //CBaseTransfer::Init()


void CBaseTransfer::SetTime(bool bStartTime)
{
	time_t ltime;
	struct tm* today;  
	time (&ltime);   //get system time
	today = localtime (&ltime);

	if (bStartTime)
	{
		m_sHour = today->tm_hour;
		m_sMin = today->tm_min;
		m_sSec = today->tm_sec;
	}
	else
	{
		m_eHour = today->tm_hour;
		m_eMin = today->tm_min;
		m_eSec = today->tm_sec;

	}

} //CBaseTransfer::SetTime

void CBaseTransfer::SetandWriteTime(bool bStartTime)
{
	char buffer[BUFF_SMALL];
	memset(buffer, 0, BUFF_SMALL);

	if (ptrLog)  {

		if (bStartTime) {
			
			SetTime();
			sprintf(buffer, "Start Time --  Hours: %02d Minutes: %02d Seconds: %02d",
					m_sHour,
					m_sMin,
					m_sSec);

			ptrLog->WriteString(buffer);
		}
		else {
			
			SetTime(false);
			sprintf(buffer, "End Time --  Hours: %02d Minutes: %02d Seconds: %02d",
					m_eHour,
					m_eMin,
					m_eSec);

			ptrLog->WriteString(buffer);

			//now caluclate time difference and write this too

			if (m_sSec > m_eSec) {
				m_eSec += 60;
				if(m_eMin != 0) {
					m_eMin--; 
				}
				else {
					m_eMin += 59; m_eHour--;
				}
			}

			if (m_sMin > m_eMin) {
				m_eMin += 60;
				m_eHour--;
			}

			memset(buffer, 0, BUFF_SMALL);
			sprintf(buffer, "Time Difference --  Hours: %03d Minutes: %03d Seconds: %03d",
				(m_eHour - m_sHour),
				(m_eMin - m_sMin),
				(m_eSec - m_sSec) );
			ptrLog->WriteString(buffer);
			ptrLog->WriteBlankLine();

		}
	
	}

} //CBaseTransfer::SetTime

HRESULT CBaseTransfer::ConfirmSQLTable(ACTION nAction, _bstr_t bstrCreateSQL)
{
	USES_CONVERSION;
	bool bExists = false;
	HRESULT hr = E_FAIL;
	if (ptrADO == NULL) {
		SetFatalError(L"DTRAN ERROR: Something is amiss with ptrADO.");
		return hr;
	}

	ADODB::_RecordsetPtr spRecordset = NULL;
	LPTSTR ptrTemp = Utilities::ConstructInfoSchemaSQL(m_bstrSQLTable, m_bstrSQLOwner);
	_bstr_t bQry = A2W(ptrTemp);

	//HRESULT hr = GetRecordSetByConnection((IDispatch**) &spRecordset, bQry, m_bstrSQLServer, m_bstrSQLDatabase);
	hr = ptrADO->GetRecordSet((IDispatch**) &spRecordset,  bQry);

	if (SUCCEEDED(hr) ) {

		if ( spRecordset != NULL)
		{
			int iRecCnt = spRecordset->RecordCount;
			if (iRecCnt == 0 )
				bExists = false;
			else
				bExists = true;
		}
	}

	switch (nAction)
	{
		case nCREATE:
			if (bExists) 
					SetFatalError(L"DTRAN ERROR:  Table can not be created, already exists.");
			else {
					//hr = ExecQueryByConnection(bstrCreateSQL, m_bstrSQLServer, m_bstrSQLDatabase);
					hr = ptrADO->ExecuteQuery(bstrCreateSQL);
					if (!SUCCEEDED(hr)) {
						_bstr_t bstrTemp = L"DTRAN ERROR: CBaseTransfer::ConfirmSQLTable -- Problem occurred when creating table.  ";
						bstrTemp += bstrCreateSQL;
						SetFatalError(bstrTemp);
					}
			}
			break;
		case nOVERWRITE:
			if (bExists)
			{
				bQry = L"truncate table "; 
				bQry += m_bstrSQLOwner;
				bQry += ".[";
				bQry += m_bstrSQLTable;
				bQry += "]";
				hr = ptrADO->ExecuteQuery(bQry);
				if (!SUCCEEDED(hr)) {
					_bstr_t bstrTemp = L"DTRAN ERROR: CBaseTransfer::ConfirmSQLTable -- Problem occurred when emptying table.  ";
					bstrTemp += bQry;
					SetFatalError(bstrTemp);
				}
				//hr = ExecQueryByConnection(bQry, m_bstrSQLServer, m_bstrSQLDatabase);
			}
			else
				SetFatalError(L"DTRAN ERROR:  Table can not be overwritten, does not exist.");
			break;
		case nAPPEND:
			if (!bExists)
				SetFatalError(L"DTRAN ERROR:  Table can not be appended, does not exist.");
			break;
		case nDROPADD:
			if (bExists)
			{
				bQry = L"drop table "; 
				bQry += m_bstrSQLOwner;
				bQry += ".[";
				bQry += m_bstrSQLTable;
				bQry += "]";
				//hr = ExecQueryByConnection(bQry, m_bstrSQLServer, m_bstrSQLDatabase);
				hr = ptrADO->ExecuteQuery(bQry);
			}
			if (SUCCEEDED(hr)) {
				
				//hr = ExecQueryByConnection(bstrCreateSQL, m_bstrSQLServer, m_bstrSQLDatabase);
				hr = ptrADO->ExecuteQuery(bstrCreateSQL);
				if (!SUCCEEDED(hr)) {
					_bstr_t bstrTemp = L"DTRAN ERROR:  CBaseTransfer::ConfirmSQLTable -- Can not create table. Create table SQL:   ";
					bstrTemp += bstrCreateSQL;
					SetFatalError(bstrTemp);
				}
			}
			else {
				_bstr_t bstrTemp = L"DTRAN ERROR: CBaseTransfer::ConfirmSQLTable -- Problem occurred when dropping the table.  ";
				bstrTemp += bQry;
				SetFatalError(bstrTemp);
			}

			break;
		default:
			if (!bExists)
				SetFatalError(L"DTRAN ERROR:  Table does not exist");
			break;
	}

	return hr;
}  //CBaseTransfer::ConfirmSQLTable

HRESULT CBaseTransfer::DeleteDSN()
{
	if (wcscmp(m_bDSN, L"") == 0) return S_OK;

	DWORD pfErrorCode;
	char lpszErrorMessage[SQL_MAX_MESSAGE_LENGTH +1];  //SQL_MAX_MESSAGE_LENGTH =256
	WORD iErr = 1;

	LPCSTR szDriver = "SQL Server";

	TCHAR szDSN[BUFF_LEN]; memset(szDSN, 0, BUFF_LEN);
	int iStrLen =  SysStringLen(m_bDSN);
	WideCharToMultiByte(CP_ACP, 0, m_bDSN, iStrLen, szDSN, iStrLen+1, NULL, NULL);
	szDSN[iStrLen]= '\0';

	TCHAR szAttributes[BUFF_SMALL];
	memset(szAttributes, 0, BUFF_SMALL);
	_tcscpy(szAttributes, "DSN=");
	_tcscat(szAttributes, szDSN);
	int ilen = _tcslen(szAttributes)+1;
	szAttributes[ilen] ='\0';
	szAttributes[ilen+1] = '\0';

	RETCODE retcode = SQLConfigDataSource(NULL, ODBC_REMOVE_SYS_DSN, szDriver, szAttributes);
	if ( retcode == FAIL ) {
		while ( SQLInstallerError(iErr++,&pfErrorCode,lpszErrorMessage,SQL_MAX_MESSAGE_LENGTH,NULL) != SQL_NO_DATA)
		{
			//if this fails do we want to set a fatal error message?
			LPCSTR lpTemp = lpszErrorMessage;
			if ( iErr > m_lMaxErrMsgs ) break;
		}
	}

	return S_OK;
}  //CBaseTransfer::DeleteDSN

void CBaseTransfer::Close()
{
	if (ptrLog)
	{
		ptrLog->Close();
//		delete ptrLog;
	}
	ptrLog = NULL;

} //CBaseTransfer::Close


void CBaseTransfer::SetFatalError(_bstr_t bErr)
{
	m_bFatalErrs = true; 
	m_listErrors.push_back(bErr);

	TCHAR szError[BUFF_MED]; memset(szError, 0, BUFF_MED);
	int iStrLen =  SysStringLen(bErr);
	WideCharToMultiByte(CP_ACP, 0, bErr, iStrLen, szError, iStrLen+1, NULL, NULL);
	szError[iStrLen]= '\0';

	if (ptrLog != NULL)
		ptrLog->WriteString(szError);


}  //CBaseTransfer::SetFatalError

_bstr_t CBaseTransfer::GetErrorMsg()
{
	_bstr_t bstrErrors = L"";
	bstrErrors = Utilities::GetErrorList(m_listErrors, false);
	return bstrErrors;
}

void CBaseTransfer::_LogInputParams()
{
	char buffer[SMALL_BUF_SIZE + 1];
	if (ptrLog == NULL) return;
	LPTSTR pzText = NULL;

	switch (m_nAction)
	{
		case nCREATE:
			pzText = (char*) "vCreate";
			break;
		case nOVERWRITE:
			pzText = (char*) "vOverWrite";
			break;
		case nAPPEND:
			pzText = (char*) "vAppend";
			break;
		case nDROPADD:
			pzText = (char*) "vDropAdd";
			break;
		default:
			pzText = (char*) "Value is not set";
			break;
	}

	sprintf(buffer, "Target Destination:\t\t%s", pzText);
	ptrLog->WriteString(buffer);

	TCHAR szDir[BUFF_MED]; memset(szDir, 0, BUFF_MED);
	int iStrLen =  SysStringLen(m_bDir);
	WideCharToMultiByte(CP_ACP, 0, m_bDir, iStrLen, szDir, iStrLen+1, NULL, NULL);
	szDir[iStrLen]= '\0';
	memset(buffer, 0, SMALL_BUF_SIZE);
	sprintf(buffer, "File Directory:\t\t\t%s (input)", ((_tcslen(szDir) == 0 ) ? (char*) "Text file directory was not specified." : szDir ) );
	ptrLog->WriteString(buffer);

	TCHAR szFile[BUFF_MED]; memset(szFile, 0, BUFF_MED);
	iStrLen =  SysStringLen(m_bFile);
	WideCharToMultiByte(CP_ACP, 0, m_bFile, iStrLen, szFile, iStrLen+1, NULL, NULL);
	szFile[iStrLen]= '\0';
	memset(buffer, 0, SMALL_BUF_SIZE);
	sprintf(buffer, "File Name:\t\t\t%s (input)", ((_tcslen(szFile) == 0 ) ? (char*) "File name was not specified." : szFile ) );
	ptrLog->WriteString(buffer);

	TCHAR szFileLoc[BUFF_MED]; memset(szFileLoc, 0, BUFF_MED);
	iStrLen =  SysStringLen(m_bFileLoc);
	WideCharToMultiByte(CP_ACP, 0, m_bFileLoc, iStrLen, szFileLoc, iStrLen+1, NULL, NULL);
	szFileLoc[iStrLen]= '\0';
	memset(buffer, 0, SMALL_BUF_SIZE);
	sprintf(buffer, "File Location:\t\t\t%s (constructed from file name & driectory)", ((_tcslen(szFileLoc) == 0 ) ? (char*) "Not able to construct file location." : szFileLoc ) );
	ptrLog->WriteString(buffer);

	TCHAR szSQLServer[BUFF_MED]; memset(szSQLServer, 0, BUFF_MED);
	iStrLen =  SysStringLen(m_bstrSQLServer);
	WideCharToMultiByte(CP_ACP, 0, m_bstrSQLServer, iStrLen, szSQLServer, iStrLen+1, NULL, NULL);
	szSQLServer[iStrLen]= '\0';
	memset(buffer, 0, SMALL_BUF_SIZE);
	sprintf(buffer, "Server Name:\t\t\t%s", szSQLServer); 
	ptrLog->WriteString(buffer);
		
	TCHAR szSQLDatabase[BUFF_MED]; memset(szSQLDatabase, 0, BUFF_MED);
	iStrLen =  SysStringLen(m_bstrSQLDatabase);
	WideCharToMultiByte(CP_ACP, 0, m_bstrSQLDatabase, iStrLen, szSQLDatabase, iStrLen+1, NULL, NULL);
	szSQLDatabase[iStrLen]= '\0';
	memset(buffer, 0, SMALL_BUF_SIZE);
	sprintf(buffer, "Database Name:\t\t\t%s", szSQLDatabase); 
	ptrLog->WriteString(buffer);	

	TCHAR szSQLTable[BUFF_MED]; memset(szSQLTable, 0, BUFF_MED);
	iStrLen =  SysStringLen(m_bstrSQLTable);
	WideCharToMultiByte(CP_ACP, 0, m_bstrSQLTable, iStrLen, szSQLTable, iStrLen+1, NULL, NULL);
	szSQLTable[iStrLen]= '\0';
	memset(buffer, 0, SMALL_BUF_SIZE);
	sprintf(buffer, "Table Name:\t\t\t%s", (char*) m_bstrSQLTable); 
	ptrLog->WriteString(buffer);
	ptrLog->WriteBlankLine();

	memset(buffer, 0, SMALL_BUF_SIZE);
	sprintf(buffer, "First Row:\t\t\t%d", m_lFirst);
	ptrLog->WriteString(buffer);

	memset(buffer, 0, SMALL_BUF_SIZE);
	sprintf(buffer, "Last Row:\t\t\t%d", m_lLast);
	ptrLog->WriteString(buffer);

}  //CBaseTransfer::LogInputParams

HRESULT CBaseTransfer::_CheckInputParams()
{
	HRESULT hr = S_OK;
	_bstr_t bstrTemp = L"";

	TCHAR szErr[MAX_ERR_LEN];
	memset(szErr, 0, MAX_ERR_LEN);

	if (ptrLog != NULL) {
		ptrLog->WriteBlankLine();
		ptrLog->WriteString(_T("Input parameter checks"));
		ptrLog->WriteString(_T("----------------------"));
	}
	
	if (m_bFileLoc.length() == 0)  {
		SetFatalError(L"DTRAN ERROR:  File location was not constructed.");
	}
	else  {

		TCHAR szFileLoc[BUFF_MED]; memset(szFileLoc, 0, BUFF_MED);
		int iStrLen =  SysStringLen(m_bFileLoc);
		WideCharToMultiByte(CP_ACP, 0, m_bFileLoc, iStrLen, szFileLoc, iStrLen+1, NULL, NULL);
		szFileLoc[iStrLen]= '\0';

		if ( !Utilities::FileCanBeRead( szFileLoc, szErr ) ){

			if (m_bImportToSQL) {
				_bstr_t bstrErr(szErr);
				SetFatalError(bstrErr);
			}
			else {
				//test path
				if (!Utilities::PathCanBeAccessed(szFileLoc, szErr, true)) {
					_bstr_t bstrErr(szErr);
					SetFatalError(bstrErr);
				}
			}
		}
		else {
			
			if(!m_bImportToSQL && m_nAction == nCREATE) {
				_stprintf(szErr, _T("File: %s exists but DestAction is set to vCreate."), szFileLoc);
				_bstr_t bstrErr(szErr);
				SetFatalError(bstrErr);

			}
		}
	}

	if (m_bstrSQLDatabase.length() == 0)  {
		SetFatalError(L"DTRAN ERROR:  Database Name must be specified.");
	}

	if (m_bstrSQLTable.length() == 0)  {
		SetFatalError(L"DTRAN ERROR:  Table Name must be specified.");
	}

/*	if (m_nAction == nAPPEND && !m_bImportToSQL)  {
		SetFatalError(L"DTRAN ERROR:  vAppend is NOT valid DestAction for SQL Table Exports to Files.");
	}

	if (m_nAction == nOVERWRITE && !m_bImportToSQL)  {
		SetFatalError(L"DTRAN ERROR:  vOverWrite is NOT valid DestAction for SQL Table Exports to Files.");
	}

	if (m_nAction != nAPPEND && m_nAction != nOVERWRITE && m_nAction != nCREATE && m_nAction != nDROPADD)  {
		SetFatalError(L"DTRAN ERROR:  DestAction is invalid. Must be set to either: vCreate, vAppend, vOverWrite, vDropAdd.");
	}
*/	
	if (m_bFatalErrs)
	{
		hr = E_FAIL;
		if (ptrLog != NULL)
			ptrLog->WriteString("FATAL Errors were found.  Processing can not proceed.");

	}
	else {
		if (ptrLog != NULL) {
			ptrLog->WriteString("No Input Parameter Errors were found.");
			ptrLog->WriteBlankLine();
			ptrLog->WriteBlankLine();
			ptrLog->WriteString("Other Messages");
			ptrLog->WriteString("--------------");
		}

	}

	if (ptrLog != NULL)
		ptrLog->WriteBlankLine();

	return hr;

}  //CBaseTransfer::CheckInputParams


//////////////////////////////////////////////////////////////////////
//
// CBCPTransfer  -- used for BCP transfers
//
//////////////////////////////////////////////////////////////////////
CBCPTransfer::CBCPTransfer(_bstr_t bstrSQLServer,
						   _bstr_t bstrSQLDatabase,
						   _bstr_t bstrSQLTable,
						   bool bImportToSQL)
		  
           : CBaseTransfer(bstrSQLServer, bstrSQLDatabase, bstrSQLTable, bImportToSQL)
{
	m_lBatchSize = SQLBATCHSIZE;
    m_lMaxErrMsgs = MAXERRMSGS;
	m_bMinErrInfo = true;
	m_lRecLen = 0;

	m_lChunkSize = 0;
	m_lFileNum = 0;
	m_lRecCnt = 0;


} //CBCPTransfer::	CBCPTransfer


CBCPTransfer::CBCPTransfer(bool bImportToSQL )
		  
           : CBaseTransfer(L"", L"", L"", bImportToSQL)
{
	m_lBatchSize = SQLBATCHSIZE;
	m_bVariedTextLenQuery = false;
	m_lRecLen = 0;

	m_lChunkSize = 0;
	m_lFileNum = 0;
	m_lRecCnt = 0;

} //CBCPTransfer::	CBCPTransfer


CBCPTransfer::~CBCPTransfer()
{

}  //CBCPTransfer::~CBCPTransfer

HRESULT CBCPTransfer::Init(bool bTrace)
{
	HRESULT hr = CBaseTransfer::Init(bTrace);
	if (SUCCEEDED(hr)  ) {
		m_bstrBCPFileLoc = m_bFileLoc;
		if (m_lChunkSize != 0 && !m_bImportToSQL) {
			//create temporary name & save old name
			TCHAR szFile[BUFF_LEN];				memset(szFile, 0, BUFF_LEN);
			TCHAR szExt[BUFF_LEN];				memset(szExt, 0, BUFF_LEN);
			TCHAR szFileTemp[BUFF_LEN];			memset(szFileTemp, 0, BUFF_LEN);
			TCHAR szFullPathTemp[BUFF_LEN];		memset(szFullPathTemp, 0, BUFF_LEN);
		
			if ( Utilities::BreakupFileName(m_bFileLoc, szFile, szExt)	 > 0   )
			{
				LPTSTR pzFileTemp = Utilities::ConstructUniqueName(szFile);
				_stprintf(szFullPathTemp, "%s.%s", pzFileTemp, szExt);
				_bstr_t bstrTemp(szFullPathTemp);
				m_bstrBCPFileLoc = bstrTemp;
			}

		}

		//if DSN is already present then don't create another
		if (wcscmp(m_bDSN, L"") == 0 && !ErrorsOccurred() ) {
			m_bDSN = Utilities::CreateDSN("", m_bstrSQLServer, m_bstrSQLDatabase);
			if ( wcscmp(m_bDSN, L"") == 0)
				hr = E_FAIL;
			else
				hr = S_OK;
			//hr = CreateDSN(L"");
		}

	}

	return hr;
} //CBCPTransfer::Init

HRESULT CBCPTransfer::InitMultiParams()  
{
	HRESULT hr = S_OK;
	//not currently in use...this calculation is done in ExportMulti

	if (m_lChunkSize == 0 && m_lFileNum == 0 )
		return S_OK;

	//get record total  -- if zero error out(?)
	ADODB::_RecordsetPtr spRecordset = NULL;
	
	_bstr_t bQry = L"SELECT COUNT(*) as rec_count FROM ";
	bQry += m_bstrSQLOwner;
	bQry += ".";
	bQry += m_bstrSQLTable;

	_variant_t vCnt;
	vCnt.vt = VT_I4;
	vCnt.intVal = 0;

	if (ptrADO != NULL)
		hr = ptrADO->GetRecordSet((IDispatch**) &spRecordset, bQry);

//	hr = GetRecordSetByConnection((IDispatch**) &spRecordset, bQry, m_bstrSQLServer, m_bstrSQLDatabase);
	if (SUCCEEDED(hr) ) {

		if ( spRecordset != NULL)
		{
			int iRecCnt = spRecordset->RecordCount;
			if (iRecCnt > 0 ) {
			
				spRecordset->MoveFirst();
				if (!spRecordset->adoEOF)
					vCnt = spRecordset->Fields->GetItem("rec_count")->Value;

				m_lRecCnt = vCnt.intVal;
				if(m_lRecCnt != 0 ) {
					if (m_lFileNum > 0)
						m_lChunkSize = m_lRecCnt / m_lFileNum;  
				}
				else {
					SetFatalError(L"DTRAN ERROR:  Source table record count is zero.");
				}
			}
		}
	}
	//setfatalerror here?????

	return hr;
}  //CBCPTransfer::InitMultiParams

void CBCPTransfer::ExtractBCPErrors(HDBC hdbc1, TCHAR* pzText, bool bWarningSrch)
{
	RETCODE			retcode = SQL_SUCCESS;
	RETCODE			retcode1 = SQL_SUCCESS;
	int irec = 1;

	if (ptrLog)
		ptrLog->WriteString(pzText);

/*	SQLINTEGER		iRownumber = 0;
	retcode1 =  SQLGetDiagField(SQL_HANDLE_DBC, hdbc1, (SQLSMALLINT) 0, SQL_DIAG_NUMBER, (void*)iRownumber, SQL_IS_INTEGER, NULL);
	if (ptrLog != NULL) {
			TCHAR buffer[BUFF_LARGE]; memset(buffer, 0, BUFF_LARGE);
			sprintf(buffer, _T("SQLGetDiagField results: retcode: %d  Status Record Count: %d "), retcode1, iRownumber);
			ptrLog->WriteString(buffer);
			ptrLog->WriteBlankLine();
	}
*/
	while (retcode != SQL_NO_DATA )
	{
		int iSQLBuffLen = 256;
		SQLINTEGER		iNativeErr = 0;
		SQLSMALLINT		iTextLength = 0;
		SDWORD			wNativeError = 0L;
		SWORD			wErrorMsgLen = 0;

		char			szState[255];    memset(szState, 0, iSQLBuffLen-1);
		char			szErrorMsg[255]; memset(szErrorMsg, 0, iSQLBuffLen-1);
	//	retcode = SQLGetDiagRec(SQL_HANDLE_DBC, hdbc1, irec++, szState, NULL,  (unsigned char*) szErrorMsg, SQL_MAX_MESSAGE_LENGTH, NULL);
	//	retcode = SQLGetDiagRec(SQL_HANDLE_DBC, hdbc1, irec++, szState, &iNativeErr,  (unsigned char*) szErrorMsg, SQL_MAX_MESSAGE_LENGTH, &iTextLength);
		retcode = SQLGetDiagRec(SQL_HANDLE_DBC, hdbc1, irec, (unsigned char*) szState, &wNativeError, (unsigned char*) szErrorMsg, iSQLBuffLen - 1, &wErrorMsgLen);
		
		bool bLogMsg = true;
		if ( m_bMinErrInfo &&  !( Utilities::FindSegment( szErrorMsg, _T("Warning")) || 
			                     Utilities::FindSegment( szErrorMsg, _T("Error"))       )  )
			bLogMsg = false;

		if (bLogMsg && retcode != SQL_NO_DATA ) {
		
			TCHAR buffer[BUFF_LARGE]; memset(buffer, 0, BUFF_LARGE);
			sprintf(buffer, _T("BCP ERROR: retcode: %d; Record Num: %d; State: %s; Native Err: %d; Error Message: %s; Error Length %d"), retcode, irec, szState, wNativeError, szErrorMsg, wErrorMsgLen );
			
			
			//char* szTemp = szErrorMsg;
			//_bstr_t bTemp((char*)szTemp);
			//char buffer[SQL_MAX_MESSAGE_LENGTH+1];
			//int iLen = strlen((char*)szErrorMsg);
			//szErrorMsg[iLen+1] = '\0';
			//memcpy(buffer, szErrorMsg, iLen);
/*			wchar_t szWide[SQL_MAX_MESSAGE_LENGTH+1];  memset(szWide, 0, 2*((SQL_MAX_MESSAGE_LENGTH+1)));
			//mbstowcs(szWide, szErrorMsg, strlen(szErrorMsg));
			int iLen = strlen(szErrorMsg);
			for (int i = 0; i < iLen; i++)
				mbstowcs(&szWide[i], &szErrorMsg[i], 1);
			szWide[wcslen(szWide) + 1 ] = L'\0';
			_bstr_t bErr = szWide;
*/
			_bstr_t bErr(buffer);
			if (bWarningSrch) {  //for now this is always false
				bool bFound1 = Utilities::FindSegment(buffer, _T("Warning: Server data"));
				bool bFound2  = Utilities::FindSegment(buffer, _T("exceeds host-file field length"));
				
				if(bFound1 && bFound2 ) {
					if (ptrLog)
						ptrLog->WriteString(_T("Warning: Server data exceeds host-file field length."));
					SetFatalError(bErr);
					//m_bTrunWarn = true;
					//break;
				}
			}
			else {

				if (wNativeError != -122) //not really an error...so don't set ... some message about # rows loaded...
					SetFatalError(bErr);
			}
		}

		irec++;
		if ( irec > m_lMaxErrMsgs ) break;
	
	}
}  //CBCPTransfer::ExtractBCPErrors

HRESULT CBCPTransfer::Transfer()
{
	HRESULT hr = E_FAIL;
	char buffer[BUFF_MED];
	memset(buffer, 0, BUFF_MED);
	m_lRowsLoaded = 0;

	SQLHENV      henv = SQL_NULL_HENV;
	HDBC         hdbc1 = SQL_NULL_HDBC;    

	RETCODE      retcode;
	SDWORD		cRows = 0;

	if (ptrLog != NULL)
	{
		ptrLog->WriteString("Start of BCP transfer from CBCPTransfer::Transfer() ");
		ptrLog->WriteBlankLine();
	}

	SetandWriteTime(true);

	// Allocate the ODBC environment and save handle.
	retcode = SQLAllocHandle (SQL_HANDLE_ENV, NULL, &henv);
	if (SQL_SUCCEEDED(retcode)) 
	{
		// Notify ODBC that this is an ODBC 3.0 app.
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION,
                     (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER); 
		if (SQL_SUCCEEDED(retcode)) 
		{
			// Allocate ODBC connection handle, set BCP mode, and connect.
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc1);
			if (SQL_SUCCEEDED(retcode))
			{
				retcode = SQLSetConnectAttr(hdbc1, SQL_COPT_SS_BCP, (void *)SQL_BCP_ON, SQL_IS_INTEGER);
				if (SQL_SUCCEEDED(retcode))  
				{
					//connect via DSN
					LPCSTR lpTemp = (char*)m_bDSN;
					retcode = SQLConnect(hdbc1, (SQLCHAR*)lpTemp, SQL_NTS, (SQLCHAR*)"", SQL_NTS, (SQLCHAR*)"", SQL_NTS);
					
				}
				else {
					SetFatalError(L"DTRAN ERROR:  Errors occurred in SQLSetConnectAttr");
				}
			}
			else {
				SetFatalError(L"DTRAN ERROR:  Errors occurred in SQLAllocHandle");
			}
		}
		else {
			SetFatalError(L"DTRAN ERROR:  Errors occurred in SQLSetEnvAttr");
		}
	}
	else {
		SetFatalError(L"DTRAN ERROR:  Errors occurred in SQLAllocHandle");
	}

	
	if (SQL_SUCCEEDED(retcode) )  //SQL_SUCCESS && SQL_SUCCESS_WITH_INFO
	{
		//TBD: temp tables should not have database name -- owned by tempdb
		_bstr_t bSource = m_bstrSQLDatabase;
//		bSource += L"..";
		bSource += L".";
		bSource += m_bstrSQLOwner;
		bSource += L".[";
		bSource += m_bstrSQLTable;
		bSource += L"]";
		_bstr_t bstrLast = L"";

		TCHAR lpErrFile[MAX_FILE_LEN];
		memset(lpErrFile, 0, MAX_FILE_LEN);
		Utilities::ConstructFileName("BCPErrors_", "txt", lpErrFile);

		//single BCP export or import
		bstrLast = L"bcp_init ";
		TCHAR szBCPFileLoc[BUFF_MED]; memset(szBCPFileLoc, 0, BUFF_MED);
		int iStrLen =  SysStringLen(m_bstrBCPFileLoc);
		WideCharToMultiByte(CP_ACP, 0, m_bstrBCPFileLoc, iStrLen, szBCPFileLoc, iStrLen+1, NULL, NULL);
		szBCPFileLoc[iStrLen]= '\0';

		TCHAR szSource[BUFF_MED]; memset(szSource, 0, BUFF_MED);
		iStrLen =  SysStringLen(bSource);
		WideCharToMultiByte(CP_ACP, 0, bSource, iStrLen, szSource, iStrLen+1, NULL, NULL);
		szSource[iStrLen]= '\0';

		if ( m_bImportToSQL ) 
			retcode = bcp_init(hdbc1, szSource, szBCPFileLoc, lpErrFile, DB_IN);
		else {
			if (m_bVariedTextLenQuery)
				retcode = bcp_init(hdbc1, NULL, szBCPFileLoc, lpErrFile, DB_OUT);
			else
				retcode = bcp_init(hdbc1, szSource, szBCPFileLoc, lpErrFile, DB_OUT);
		}

		if (retcode == SUCCEED)
		{	//control statements...
			if (m_lFirst > 0 && m_lLast >= m_lFirst )
			{	//partial export/import
				retcode = bcp_control(hdbc1, BCPFIRST, (void*) m_lFirst);
				retcode = bcp_control(hdbc1, BCPLAST,  (void*) m_lLast);
			}
				
			if (retcode == SUCCEED) 
				retcode = bcp_control(hdbc1, BCPBATCH, (void*) m_lBatchSize);

			if (retcode == SUCCEED && m_bVariedTextLenQuery) 
			{
				TCHAR szSQLDatabase[BUFF_MED]; memset(szSQLDatabase, 0, BUFF_MED);
				int iStrLen =  SysStringLen(m_bstrSQLDatabase);
				WideCharToMultiByte(CP_ACP, 0, m_bstrSQLDatabase, iStrLen, szSQLDatabase, iStrLen+1, NULL, NULL);
				szSQLDatabase[iStrLen]= '\0';

				TCHAR szSQLOwner[BUFF_MED]; memset(szSQLOwner, 0, BUFF_MED);
				iStrLen =  SysStringLen(m_bstrSQLOwner);
				WideCharToMultiByte(CP_ACP, 0, m_bstrSQLOwner, iStrLen, szSQLOwner, iStrLen+1, NULL, NULL);
				szSQLOwner[iStrLen]= '\0';

				TCHAR szSQLTable[BUFF_MED]; memset(szSQLTable, 0, BUFF_MED);
				iStrLen =  SysStringLen(m_bstrSQLTable);
				WideCharToMultiByte(CP_ACP, 0, m_bstrSQLTable, iStrLen, szSQLTable, iStrLen+1, NULL, NULL);
				szSQLTable[iStrLen]= '\0';

				TCHAR szQuery[MAX_QUERY_LEN];  memset(szQuery, 0, MAX_QUERY_LEN);
				_stprintf(szQuery, sz_VariedQuery, szSQLDatabase, szSQLOwner, szSQLTable);
				retcode = bcp_control(hdbc1, BCPHINTS, (void*) szQuery);
			}
					
			if (retcode == SUCCEED) 
			{	//read the format file
				bstrLast = L"bcp_readfmt ";

				TCHAR szFormatFileLoc[BUFF_MED]; memset(szFormatFileLoc, 0, BUFF_MED);
				int iStrLen =  SysStringLen(m_bFormatFileLoc);
				WideCharToMultiByte(CP_ACP, 0, m_bFormatFileLoc, iStrLen, szFormatFileLoc, iStrLen+1, NULL, NULL);
				szFormatFileLoc[iStrLen]= '\0';

				retcode = bcp_readfmt(hdbc1, szFormatFileLoc);
			
				if (retcode == SUCCEED)
				{
					bstrLast = L"bcp_exec ";
					try {
						retcode = bcp_exec(hdbc1, &cRows);  //single bcp 
						if (retcode == SUCCEED)
						{
							memset(buffer, 0, BUFF_MED);
							sprintf(buffer, szLog_RowsCopied, cRows);
							if (ptrLog != NULL)
								ptrLog->WriteString(buffer);
							m_lRowsLoaded = (long) cRows;
							hr = S_OK;

							if (retcode == SQL_SUCCESS_WITH_INFO) {
								//these are warning messages ....
								ExtractBCPErrors(hdbc1, "BCP completed process with: SQL_SUCCESS_WITH_INFO", false);
							}

							if ( (m_lChunkSize != 0 || m_lFileNum != 0 ) && !m_bImportToSQL) {
								MultiExport();

								TCHAR szBCPFileLoc[BUFF_MED]; memset(szBCPFileLoc, 0, BUFF_MED);
								int iStrLen =  SysStringLen(m_bstrBCPFileLoc);
								WideCharToMultiByte(CP_ACP, 0, m_bstrBCPFileLoc, iStrLen, szBCPFileLoc, iStrLen+1, NULL, NULL);
								szBCPFileLoc[iStrLen]= '\0';
								DeleteFile(szBCPFileLoc);
							}

						}
					}
					catch (...) {
						SetFatalError(L"DTRAN ERROR:Caught by catch statement in CBCPTransfer::Transfer() on bcp_exec()"); 
					}
				}
			}
		}

		if (!SQL_SUCCEEDED(retcode) )  {
			_bstr_t bstrErrorMsg = bstrLast + L"FAILED";
			ExtractBCPErrors(hdbc1, bstrErrorMsg);
		}
		
		TCHAR pzJunk[MAX_ERR_LEN]; memset(pzJunk, 0, MAX_ERR_LEN); //don't care about this error
		if (Utilities::FileCanBeRead(lpErrFile, pzJunk) ) 
		{		
			ifstream fileBCP(lpErrFile);
			if (fileBCP.is_open()) {

				TCHAR pzBuffer[BUFF_MED];
				memset(pzBuffer, 0, BUFF_MED);
				_bstr_t bstrTemp = L"BCP ERROR:  ";
				bool bHasText = false;

				fileBCP.getline(pzBuffer, BUFF_MED);  
				while (fileBCP) 
				{
					_bstr_t bstrBuffer(pzBuffer);
					bstrTemp += bstrBuffer;
					memset(pzBuffer, 0,BUFF_MED);
					fileBCP.getline(pzBuffer, BUFF_MED);  
					bHasText = true;
				}

				if (bHasText) 
					SetFatalError(bstrTemp);
				fileBCP.close();
			}

			DeleteFile(lpErrFile);
		}
	}
	else if (retcode == SQL_ERROR ) {
		ExtractBCPErrors(hdbc1, _T("SQLConnect returned SQL_ERROR."));
	}
	else if (retcode == SQL_INVALID_HANDLE) {
		ExtractBCPErrors(hdbc1, _T("SQLConnect returned SQL_INVALID_HANDLE."));
	}
	else {
		ExtractBCPErrors(hdbc1, _T("Errors occurred on SQL ODBC calls"));
	}

	SQLDisconnect(hdbc1);

	SQLFreeHandle(SQL_HANDLE_DBC, hdbc1);
	SQLFreeHandle(SQL_HANDLE_ENV, henv);

	SetandWriteTime(false);
	return hr;

} //CBCPTransfer::Transfer

void CBCPTransfer::MultiExport() 
{
	//basically this just chops up the export file into n files
	TCHAR szBufferOut[BUFF_MED];
	memset(szBufferOut, 0, BUFF_MED);
	DWORD dwReadSize = 0;  
	DWORD dwRecSize = 0;
	DWORD dwBytesRead = 0;
	DWORD64 dwFileSize = 0;
	DWORD64 dwTotalRead = 0;
	DWORD64 dwTotalWrote = 0;
	int iFileCnt = 0;

	TCHAR szBCPFileLoc[BUFF_MED]; memset(szBCPFileLoc, 0, BUFF_MED);
	int iStrLen =  SysStringLen(m_bstrBCPFileLoc);
	WideCharToMultiByte(CP_ACP, 0, m_bstrBCPFileLoc, iStrLen, szBCPFileLoc, iStrLen+1, NULL, NULL);
	szBCPFileLoc[iStrLen]= '\0';

	ifstream fileBCP(szBCPFileLoc);
	if (fileBCP.is_open()) {
		TCHAR* pzBuffer = new TCHAR[m_lRecLen+100];
		memset(pzBuffer, 0, m_lRecLen+100);
		if (pzBuffer) {
			fileBCP.getline(pzBuffer, m_lRecLen+100);  
			dwRecSize = fileBCP.gcount();
			dwRecSize++;	//add 1 
			delete pzBuffer;
			pzBuffer = NULL;
		}
	}
	fileBCP.close();

	TCHAR pzBase[BUFF_LEN]; 
	TCHAR pzExt[BUFF_LEN]; 
	int i = Utilities::BreakupFileName(m_bFileLoc, pzBase, pzExt);
	if (_tcslen(pzExt) == 0)
		_tcscpy(pzExt, _T("txt"));

	if (ptrLog != NULL) {
		_stprintf(szBufferOut, _T("Begin MultiFile Export File splitting for file: %s"), szBCPFileLoc);
		ptrLog->WriteString(szBufferOut);
	}

	HANDLE hSpoolFileIn = CreateFile(szBCPFileLoc, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);
	try {

		if (hSpoolFileIn != INVALID_HANDLE_VALUE) 
		{ 
			LARGE_INTEGER pLg1;
			pLg1.QuadPart = 0;
			PLARGE_INTEGER pLg = &pLg1;
			BOOL bRet = GetFileSizeEx(hSpoolFileIn, pLg);
			if (bRet)
			 dwFileSize = pLg->QuadPart;
			else  {
				SetFatalError(L"DTRAN ERROR: CBCPTransfer::MutliExport -- An error resulted in a GetFileSizeEx operation, processing can not continue");
				goto EndProcessing;
			}

			DWORD64 dwTotRecCnt =  dwFileSize/dwRecSize;
			DWORD64 dwChunkSize = 0;
			if (m_lFileNum != 0 ) {  //dividing by number of files to get chunk size here...
				dwChunkSize = dwTotRecCnt / m_lFileNum;
			}
			else 
				dwChunkSize = m_lChunkSize;

/*			if (ptrLog != NULL) {
				_stprintf(szBufferOut, _T("Begin MultiExport: %s"), pszBCPFileLoc);
				ptrLog->WriteString(szBufferOut);
				ptrLog->WriteBlankLine();
				 _stprintf(szBufferOut, _T("\n\r STATISTICS dwFileSize: %I64i bytes, dwRecSize: %I64i bytes, lTotRecCnt: %I64i, m_lChunkSize: %I64i bytes, m_lFileNum: %I64i"),  dwFileSize, dwRecSize, lTotRecCnt, m_lChunkSize, m_lFileNum);
			   ptrLog->WriteString(szBufferOut);
			}
*/
			DWORD64 dwSubFileSize =  dwRecSize  * dwChunkSize;  
			if (dwSubFileSize >= MAXHEAPSIZE) 
				dwReadSize = (long) MAXHEAPSIZE/dwRecSize * dwRecSize;
			else
				dwReadSize = (DWORD32) dwSubFileSize; //if we land here then we will be < 8 bytes in length

			TCHAR* pzBuf = new TCHAR[(int)dwReadSize+1];
			memset(pzBuf, 0, (int) dwReadSize+1);
			BOOL bResultIn = ReadFile(hSpoolFileIn, pzBuf, dwReadSize, &dwBytesRead, NULL);
			dwTotalRead += dwBytesRead;
			
			while ( bResultIn && dwBytesRead != 0  ) 
			{
				//construct the new file name 
				TCHAR pzFileNew[MAX_FILE_LEN];  memset(pzFileNew, 0, MAX_FILE_LEN);
				TCHAR szErr[MAX_ERR_LEN];  memset(szErr, 0, MAX_ERR_LEN);
				sprintf(pzFileNew, "%s%03d.%s", pzBase, ++iFileCnt, pzExt);
				
				//test to make sure it doesn't exist
				if ( Utilities::FileCanBeRead( pzFileNew, szErr ) && m_nAction == nCREATE ) {
					_stprintf(szErr, _T("DTRAN ERROR: CBCPTransfer::MutliExport -- Text file: %s exists but DestAction is set to vCreate."), pzFileNew);
					_bstr_t bstrErr(szErr);
					SetFatalError(bstrErr);
					break;
				}
				
				DWORD64 dwSubFileSizeOut = 0;
				HANDLE hSpoolFileOut = CreateFile(pzFileNew, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);
				if (hSpoolFileOut != INVALID_HANDLE_VALUE) 
				{
					//calculate subfilesize -- this may change on the tail end of the processing..
					DWORD64 dwDelta = dwFileSize - dwTotalWrote;
					if ( m_lFileNum == 0 )   //vBYREC calculations...we don't want to overread...
					{
						if (dwDelta < dwSubFileSize)
							dwSubFileSize = dwDelta;
					}
					else {  //vBYFILE calculations -- for uneven distributions, makes the last file larger instead of having
							//n+1 file and the last one being very small						
						if ( (dwDelta < 2* dwSubFileSize) ||						//normal curicumstances
							 (dwDelta <= 2 * dwSubFileSize && m_lFileNum == 2)  )   //special case when have 3 records and 2 files....
							                                  
							dwSubFileSize = dwDelta; 
					}

					//write out first segment read...
					DWORD dwBytesWrote = 0;
					BOOL bRetOut = WriteFile(hSpoolFileOut, pzBuf, dwBytesRead, (LPDWORD) &dwBytesWrote, NULL);
					dwSubFileSizeOut =  dwBytesWrote;

					int iLineCnt = 0;
					while (dwSubFileSizeOut < dwSubFileSize   )
					{
						DWORD dwSegmentSize = dwReadSize;
						if (dwSubFileSize -  dwSubFileSizeOut < dwReadSize )   //read just enough ...or what we grab will be in the next file
							dwSegmentSize =  (DWORD32) (dwSubFileSize -  dwSubFileSizeOut);  //if we land here then we will be < 8 bytes in length
						memset(pzBuf, 0, (int) dwReadSize);
						bResultIn = ReadFile(hSpoolFileIn, pzBuf, dwSegmentSize, &dwBytesRead, NULL);
						dwTotalRead += dwBytesRead;
						pzBuf[dwBytesRead] = '\0';
						if (bResultIn && dwBytesRead != 0 ) {
							WriteFile(hSpoolFileOut, pzBuf, dwBytesRead, &dwBytesWrote, NULL);
							dwSubFileSizeOut +=  dwBytesWrote;
						}
						else {
							SetFatalError(L"DTRAN ERROR: CBCPTransfer::MutliExport -- An error resulted in a ReadFile operation, processing can not continue");
							goto EndProcessing;
						}
					}
				}
				
				if (hSpoolFileOut != INVALID_HANDLE_VALUE)
					CloseHandle(hSpoolFileOut);
				
				dwTotalWrote += dwSubFileSizeOut;
				if (ptrLog != NULL) {
					ptrLog->WriteBlankLine();
					memset(szBufferOut, 0, BUFF_MED);
					_stprintf(szBufferOut, "File:  %s  rows split out:  %d.", pzFileNew, dwSubFileSizeOut/dwRecSize);
					ptrLog->WriteString(szBufferOut);
				}

				//read the next chunck for the new file
				memset(pzBuf, 0, (int) dwReadSize+1);
				dwBytesRead = 0;
				if (dwFileSize - dwTotalWrote > 0 ) {
					//do delta calculations here
					DWORD64 dwSegSize = dwReadSize;
					if ( dwFileSize - dwTotalWrote < dwReadSize ) 
						dwSegSize = dwFileSize - dwTotalWrote;
						
					bResultIn = ReadFile(hSpoolFileIn, pzBuf, (DWORD32) dwSegSize, &dwBytesRead, NULL);
					pzBuf[dwBytesRead] = '\0';
					dwTotalRead += dwBytesRead;
				}
			}

EndProcessing:
			if (pzBuf)
				delete [] pzBuf;

			if (hSpoolFileIn != INVALID_HANDLE_VALUE)
				CloseHandle(hSpoolFileIn);
		
		}
			
	}
	catch (...) {
		SetFatalError(L"DTRAN ERROR: CBCPTransfer::MutliExport -- In catch statement.");
	}

	if (ptrLog != NULL) {
		ptrLog->WriteBlankLine();
		memset(szBufferOut, 0, BUFF_MED);
		_stprintf(szBufferOut, "Bytes read: %I64i  Bytes written: %I64i  Total rows: %I64i.  End MultiFile Export splitting file.", dwTotalRead, dwTotalWrote, dwTotalWrote/dwRecSize);
		ptrLog->WriteString(szBufferOut);
	}

}  //CBCPTransfer::MutliExport



void CBCPTransfer::Cleanup(bool bCreated)
{
	TCHAR szFormatFileLoc[BUFF_MED]; memset(szFormatFileLoc, 0, BUFF_MED);
	int iStrLen =  SysStringLen(m_bFormatFileLoc);
	WideCharToMultiByte(CP_ACP, 0, m_bFormatFileLoc, iStrLen, szFormatFileLoc, iStrLen+1, NULL, NULL);
	szFormatFileLoc[iStrLen]= '\0';

	if (bCreated)
			DeleteFile(szFormatFileLoc);
	DeleteDSN();
}  //CBCPTransfer::Cleanup



void CBCPTransfer::LogInputParams()
{
	char buffer[SMALL_BUF_SIZE + 1];
	if (ptrLog == NULL) return;

	LPTSTR pzText = NULL;
	ptrLog->SetTimeStamp(true);
	ptrLog->WriteBlankLine();
	ptrLog->WriteString(_T("Input parameters for Data Transfer"));
	ptrLog->WriteString(_T("----------------------------------"));
	ptrLog->WriteBlankLine();

	if (m_bImportToSQL ) 
		pzText = (char*) "Fixed Length Text File Import to SQL Server Table";
	else if ( !m_bImportToSQL ) {
		if (m_lFileNum != 0 || m_lChunkSize != 0 )
			pzText = (char*) "SQL Server Table Export to Multiple Fixed Length Text Files.";
		else
			pzText = (char*) "SQL Server Table Export to Fixed Length Text File.";
	}
	ptrLog->WriteString(pzText);
	_LogInputParams();

	if (m_lFileNum != 0 || m_lChunkSize != 0 ) {
	
		memset(buffer, 0, SMALL_BUF_SIZE);
		if (m_lChunkSize != 0)
			sprintf(buffer, "Multi export record count:\t\t%d", m_lChunkSize); 

		ptrLog->WriteString(buffer);
		memset(buffer, 0, SMALL_BUF_SIZE);
		if (m_lFileNum != 0)
			sprintf(buffer, "Multi export file count:\t\t%d", m_lFileNum); 
		ptrLog->WriteString(buffer);
	}

	memset(buffer, 0, SMALL_BUF_SIZE);
	sprintf(buffer, "BCP Batch Size:\t\t\t%d (default is %d)", m_lBatchSize, SQLBATCHSIZE );
	ptrLog->WriteString(buffer);

	memset(buffer, 0, SMALL_BUF_SIZE);
	sprintf(buffer, "Maximum Error Messages:\t\t%d (default is %d)", m_lMaxErrMsgs, MAXERRMSGS );
	ptrLog->WriteString(buffer);

	memset(buffer, 0, SMALL_BUF_SIZE);
	sprintf(buffer, "No Error Info:\t\t\t%s (default is true)", ( (m_bMinErrInfo == true ) ? "true" : "false") );
	ptrLog->WriteString(buffer);

	TCHAR szFormatFileLoc[BUFF_MED]; memset(szFormatFileLoc, 0, BUFF_MED);
	int iStrLen =  SysStringLen(m_bFormatFileLoc);
	WideCharToMultiByte(CP_ACP, 0, m_bFormatFileLoc, iStrLen, szFormatFileLoc, iStrLen+1, NULL, NULL);
	szFormatFileLoc[iStrLen]= '\0';
	memset(buffer, 0, SMALL_BUF_SIZE);
	sprintf(buffer, "Format File Location:\t\t%s", (  ( _tcslen(szFormatFileLoc) == 0 ) ? (char*) "Format file was not specified." : szFormatFileLoc ) );
	ptrLog->WriteString(buffer);

}  //CBCPTransfer::LogInputParams

HRESULT CBCPTransfer::CheckInputParams()
{
	HRESULT hr = S_OK;
	_bstr_t bstrTemp = L"";
	TCHAR szErr[MAX_ERR_LEN];
	memset(szErr, 0, MAX_ERR_LEN);

	if (m_bFormatFileLoc.length() == 0) {
		SetFatalError(L"DTRAN ERROR:  Format file location must be specified.");
	}
	else
	{
		TCHAR szFormatFileLoc[BUFF_MED]; memset(szFormatFileLoc, 0, BUFF_MED);
		int iStrLen =  SysStringLen(m_bFormatFileLoc);
		WideCharToMultiByte(CP_ACP, 0, m_bFormatFileLoc, iStrLen, szFormatFileLoc, iStrLen+1, NULL, NULL);
		szFormatFileLoc[iStrLen]= '\0';

		if ( !Utilities::FileCanBeRead(szFormatFileLoc, szErr ) ) {
			_bstr_t bstrErr(szErr);
			SetFatalError(bstrErr);
		}
	
	}
	
	if (m_nAction == nAPPEND && !m_bImportToSQL)  {
		SetFatalError(L"DTRAN ERROR:  vAppend is NOT valid DestAction for SQL Table Exports to Files.");
	}

	if (m_nAction == nOVERWRITE && !m_bImportToSQL)  {
		SetFatalError(L"DTRAN ERROR:  vOverWrite is NOT valid DestAction for SQL Table Exports to Files.");
	}

	if (m_nAction != nAPPEND && m_nAction != nOVERWRITE && m_nAction != nCREATE && m_nAction != nDROPADD)  {
		SetFatalError(L"DTRAN ERROR:  DestAction is invalid. Must be set to either: vCreate, vAppend, vOverWrite, vDropAdd.");
	}
//	_CheckInputParams();
	return hr;
} //CBCPTransfer::CheckInputParams

//////////////////////////////////////////////////////////////////////
//
// CDTSTransfer  -- abstract base class for DTS transfers
//
//////////////////////////////////////////////////////////////////////

CDTSTransfer::CDTSTransfer(_bstr_t bstrSQLServer,
						   _bstr_t bstrSQLDatabase,
						   _bstr_t bstrSQLTable,
						   bool bImportToSQL)
		  
           : CBaseTransfer(bstrSQLServer, bstrSQLDatabase, bstrSQLTable, bImportToSQL)
{
	m_lSkipRows = 0; 
	m_iSQLConnID = 1;
	m_iOtherConnID = 2;
	m_bstrSQLConnName = L"SQL Connection";
	m_bstrOtherConnName = L"Other Connection";
} //CDTSTransfer::CDTSTransfer

CDTSTransfer::CDTSTransfer(bool bImportToSQL )
           : CBaseTransfer(L"", L"", L"", bImportToSQL)
{
	m_lSkipRows = 0;
	m_iSQLConnID = 1;
	m_iOtherConnID = 2;
	m_bstrSQLConnName = L"SQL Connection";
	m_bstrOtherConnName = L"Other Connection";
	m_ptrSrceFldList = NULL;
} //CDTSTransfer::CDTSTransfer

CDTSTransfer::~CDTSTransfer()
{
}  //CDTSTransfer::~CDTSTransfer

HRESULT CDTSTransfer::Init(bool bTrace)
{
	HRESULT hr = CBaseTransfer::Init(bTrace);
	m_bstrSQLObjectName =  L"[";
	m_bstrSQLObjectName += m_bstrSQLDatabase;
	m_bstrSQLObjectName += L"].[";
	m_bstrSQLObjectName += m_bstrSQLOwner;
	m_bstrSQLObjectName += L"].[";
	m_bstrSQLObjectName += m_bstrSQLTable;
	m_bstrSQLObjectName += "]";

	if (SUCCEEDED(hr)) {
		if (m_lFirst < 0 ) m_lFirst = 0;		//something is wrong, just take the entire file....
		if (m_lFirst > m_lLast ) m_lLast = 0; 
		if (m_lFirst != 0 && m_lLast == 0) 
			m_lSkipRows = m_lFirst - 1; 
	}
	return hr;
} //CDTSTransfer::Init

HRESULT CDTSTransfer::Transfer()
{
	//for now this is a one-way import of a delimted text file to SQL table
	HRESULT hr = E_FAIL;
	char buffer[BUFF_MED];
	memset(buffer, 0, BUFF_MED);
	m_lRowsLoaded = 0;
	if (ptrLog != NULL)
	{
		ptrLog->WriteString("Start of DTS transfer from CDTSTransfer::Transfer() ");
		ptrLog->WriteBlankLine();
	}
	SetandWriteTime(true);

	//CoInitialize(NULL);  //?
	try
	{
		LPTSTR ptrName = "Transfer";
		_variant_t vTemp; vTemp.vt = VT_BOOL;  vTemp.boolVal = true;

		//create smart pointers to package 
		_Package2Ptr pkg(__uuidof(Package2));
		pkg->Name = "DTS_Import_ex";
		pkg->Description = "DTS package description";
		pkg->FailOnError = false;

		//*** connection object -- this is for the nonSQL source/target
		ConnectionPtr connOther;
		TCHAR* ptrProgID = GetOtherProgID();
		connOther = pkg->Connections->New(ptrProgID);	
		if (connOther == NULL) {
			SetFatalError("DTRAN ERROR: CDTSTransfer::Transfer - ConnectionPtr connOther is NULL");
			return hr;
		}
		InitOtherConnection(connOther); 
		connOther->Name = "Connection Other";
		connOther->ID = m_iOtherConnID;
		pkg->Connections->Add(connOther);

		//*** connection object for SQL source/target 
		ConnectionPtr connSQL;
		connSQL = pkg->Connections->New("SQLOLEDB");
		connSQL->ConnectionProperties->Item("Integrated Security")->PutValue("SSPI");
        connSQL->ConnectionProperties->Item("Persist Security Info")->PutValue(vTemp.boolVal);
		connSQL->ConnectionProperties->Item("Initial Catalog")->PutValue(m_bstrSQLDatabase);
		connSQL->ConnectionProperties->Item("Data Source")->PutValue(m_bstrSQLServer);
        connSQL->ConnectionProperties->Item("Application Name")->PutValue("DTS  Import/Export Wizard");
		connSQL->Name = "Connection SQL";
		connSQL->ID = m_iSQLConnID;
		connSQL->DataSource = m_bstrSQLServer;
		connSQL->ConnectionTimeout = 60;
		connSQL->Catalog = m_bstrSQLDatabase;
		connSQL->UseTrustedConnection = true;
		pkg->Connections->Add(connSQL);

		// Create the Step Object
		Step2Ptr ptrStep;
		ptrStep = pkg->Steps->New();
		ptrStep->Name = ptrName;
		ptrStep->Description = ptrName;
		ptrStep->TaskName = ptrName;
		ptrStep->FailPackageOnError = false;
		ptrStep->ExecuteInMainThread = true;
		ptrStep->RollbackFailure = false;
       	pkg->Steps->Add(ptrStep);  // Add the Step Object to a package

		// Create Task Object
		TaskPtr ptrTask1;
		ptrTask1 = pkg->Tasks->New("DTSDataPumpTask");
	
		//Create custom task object from task object
		DataPumpTask2Ptr ptrCustomTask;
		ptrCustomTask = ptrTask1->CustomTask;
		ptrCustomTask->Name = ptrName;
		ptrCustomTask->Description = ptrName;
		ptrCustomTask->SourceConnectionID = ( (m_bImportToSQL == true)  ? m_iOtherConnID : m_iSQLConnID);
		ptrCustomTask->SourceObjectName = ( (m_bImportToSQL == true)  ? GetOtherObjectName() : m_bstrSQLObjectName);
		ptrCustomTask->DestinationConnectionID = ( (m_bImportToSQL == true)  ? m_iSQLConnID : m_iOtherConnID);
		ptrCustomTask->DestinationObjectName =  ( (m_bImportToSQL == true)  ? m_bstrSQLObjectName : GetOtherObjectName());
		long lCommitSize = m_lBatchSize;
		ptrCustomTask->InsertCommitSize = lCommitSize;
		ptrCustomTask->FastLoadOptions = (DTSFastLoadOptions) 2;
		ptrCustomTask->UseFastLoad = VARIANT_TRUE;

		if (m_lFirst > 0 && m_lLast > 0) {
			ptrCustomTask->FirstRow = m_lFirst;
			ptrCustomTask->LastRow = m_lLast;
			long lCommitSize2 = (m_lLast - m_lFirst)/2; 
			if (lCommitSize2 < lCommitSize) ptrCustomTask->InsertCommitSize;
		}
		AddMoreTaskProperties(ptrCustomTask);
		
		//Create transformation pointer from custom task pointer
		Transformation2Ptr ptrTrans;
		ptrTrans = ptrCustomTask->Transformations->New("DTS.DataPumpTransformCopy");
		ptrTrans->Name = "DirectCopyXform";

		PropertiesPtr ptrProp;			//don't really use this, but need to add it....
		ptrProp = ptrTrans->Properties;
		ptrTrans->TransformFlags = 63;
		ptrTrans->ForceSourceBlobsBuffered = (DTSForceMode) 0;
		ptrTrans->ForceBlobsInMemory = false;
		ptrTrans->InMemoryBlobSize = 1048576;
		ptrTrans->TransformPhases = 4;
		
		//column mapping...if list is empty then mapping is not needed...
		SQLFIELDLIST::iterator iterList;
		int iCnt = 1;
		for (iterList = m_ptrSrceFldList->begin(); iterList != m_ptrSrceFldList->end(); iterList++)  {
			_bstr_t bstrNameSrce = (*iterList);
			ColumnPtr ptrSrcCol = NULL;
			ptrSrcCol = ptrTrans->SourceColumns->New(bstrNameSrce, iCnt);
			ptrTrans->SourceColumns->Add(ptrSrcCol);
			iCnt++;
		}

		iCnt = 1;
		for (iterList = m_ptrTrgtFldList->begin(); iterList != m_ptrTrgtFldList->end(); iterList++)  {
			_bstr_t bstrNameTrgt = (*iterList);
			ColumnPtr ptrDestCol = NULL;
			ptrDestCol = ptrTrans->DestinationColumns->New(bstrNameTrgt, iCnt);
			ptrTrans->DestinationColumns->Add(ptrDestCol);
			iCnt++;
		}
	
		ptrCustomTask->Transformations->Add(ptrTrans);
		pkg->Tasks->Add(ptrTask1);  // Add the Task to the Package

		DWORD  dwCookie = 0;
		LPCONNECTIONPOINT	pCP = NULL;
		LPCONNECTIONPOINTCONTAINER pCPC = NULL;
		CCOMSink* pEvents = NULL;
        try { 
			//set up events via sink class
			hr = pkg.QueryInterface(IID_IConnectionPointContainer, (void**) &pCPC);
			if (pCPC != NULL && SUCCEEDED(hr) ) {
				hr = pCPC->FindConnectionPoint(__uuidof(PackageEvents), &pCP);
				if ( pCPC != NULL && SUCCEEDED(hr) ) {
					pEvents = new CCOMSink();
					if (pEvents != NULL) {
						hr = pCP->Advise(pEvents, &dwCookie);
						if (BAD_COOKIE == dwCookie)
							SetFatalError(L"DTRAN ERROR: CDTSTransfer::Transfer() -- Cookie returned from connection point method is bad.");
						pCPC->Release();
						pCPC = NULL;
						// End - Setup events
					}
					else
						SetFatalError(L"DTRAN ERROR: CDTSTransfer::Transfer() -- Pointer to Sink class is bad.");

				}
				else 
					SetFatalError(L"DTRAN ERROR: CDTSTransfer::Transfer() -- Pointer to connection point can not be found.");
			}
			else
				SetFatalError(L"DTRAN ERROR: CDTSTransfer::Transfer() -- Interface pointer, IConnectionPointContainer, is bad.");

			hr = pkg->Execute();
			Utilities::TransferErrors(pEvents->m_listErrors, m_listErrors);

			// Begin - Cleanup events
/*			hr = pCP->Unadvise(dwCookie);
			pCP->Release();
			pCP = NULL;
			pEvents = NULL;
			delete pEvents;
*/			// End - Cleanup events
			ExtractDTSStepErrors(pkg);
			if (SUCCEEDED(hr)) {
				_variant_t vRowsLoaded = ptrCustomTask->RowsComplete;
				_variant_t vRowsInErr = ptrCustomTask->RowsInError;
				if ( !Utilities::IsNullOrEmpty(vRowsLoaded) && !Utilities::IsNullOrEmpty(vRowsInErr) )  
				{
					m_lRowsLoaded = vRowsLoaded.intVal - vRowsInErr.intVal;
					TCHAR buffer[BUFF_MED];  memset(buffer, 0, BUFF_MED);
					sprintf(buffer, szLog_RowsCopied, m_lRowsLoaded);
					if (ptrLog != NULL)
						ptrLog->WriteString(buffer);
					if (vRowsInErr.intVal != 0)  {
						vRowsInErr.ChangeType(VT_BSTR);
						_bstr_t bstrErr = L"DTRAN ERROR: Not all rows loaded properly. Rows in error: ";
						bstrErr += vRowsInErr.bstrVal;
						SetFatalError(bstrErr);
					}
				}
			}
		}
		catch (...) {
			ExtractDTSStepErrors(pkg);
		}
		//DTSStepExecResult enumResult = ptrStep->GetExecutionResult();
		if ( ptrStep->GetExecutionResult() == DTSStepExecResult_Failure) {
			SetFatalError(L"DTS Step ExecutionResult was DTSStepExecResult_Failure.");
		}

		//finish cleanup
		if (pCP != NULL) {
			hr = pCP->Unadvise(dwCookie);
			pCP->Release();
			pCP = NULL;
		}
		if (pEvents != NULL) {
			pEvents = NULL;
			delete pEvents;
		}
	}
	catch(_com_error &e)  {
		_bstr_t bstrError = Utilities::ConstructErrMsg(L"COM ERROR: Caught by catch statement in CDTSTransfer::Transfer().", e.Source(), e.Description(), e.ErrorMessage());
		SetFatalError(bstrError);
	}
	SetandWriteTime(false);  
	return hr;
} //CDTSTransfer::Transfer

void CDTSTransfer::ExtractDTSStepErrors(_Package2Ptr spPackage)
{
	try
	{
		HRESULT hr;
		StepsPtr steps = spPackage->GetSteps();

		for (long i = 1; i <= steps->GetCount(); i++)
		{
			StepPtr step = steps->Item(i);
			if (hr = step->GetExecutionStatus() == DTSStepExecStat_Completed)
			{
				if (hr = step->GetExecutionResult() == DTSStepExecResult_Failure)
				{
					long	lErrorCode = 0;
					BSTR	bSource;
					BSTR	bDescription;
					BSTR	bHelpFile;
					long	lHelpContext = 0;
					BSTR	bIDofInterfaceWithError;

					hr = step->GetExecutionErrorInfo(&lErrorCode, &bSource, &bDescription, &bHelpFile,
						&lHelpContext, &bIDofInterfaceWithError);
					
					_bstr_t bstrDescp = bDescription; 
					_bstr_t bstrSrc = bSource;
					_bstr_t bstrError = Utilities::ConstructErrMsg(L"DTS STEP ERROR:", bstrSrc, bstrDescp, L"");
					SetFatalError(bstrError);
					SysFreeString(bSource);
					SysFreeString(bDescription);
				}
			}
		}
	}
	catch(_com_error e)
	{
		_bstr_t bstrError = Utilities::ConstructErrMsg(L"COM ERROR: Caught by catch statement in CDTSTransfer::ExtractDTSStepErrors().", e.Source(), e.Description(), e.ErrorMessage());
		SetFatalError(bstrError);
	}

}  //CDTSTransfer::ExtractDTSStepErrors


//////////////////////////////////////////////////////////////////////
//
// CDTSDelim  -- used for Delimited Transfers...
//
//////////////////////////////////////////////////////////////////////

CDTSDelim::CDTSDelim(_bstr_t bstrSQLServer,
					 _bstr_t bstrSQLDatabase,
					 _bstr_t bstrSQLTable,
					 bool bImportToSQL)
		  
           : CDTSTransfer(bstrSQLServer, bstrSQLDatabase, bstrSQLTable, bImportToSQL)
{
	m_bstrDelim = L",";
	m_bstrRowDelim = L"\r\n";
	m_bstrTextQual = L"";
	m_bHeaderLine = false;
} //CDTSDelim::CDTSDelim

CDTSDelim::CDTSDelim(bool bImportToSQL )
		  
           : CDTSTransfer(L"", L"", L"", bImportToSQL)
{
	m_bstrDelim = L",";
	m_bstrRowDelim = L"\r\n";
	m_bstrTextQual = L"";
	m_bHeaderLine = false;

} //CDTSDelim::CDTSTransfer

CDTSDelim::~CDTSDelim()
{
}  //CDTSDelim::~CDTSDelim


void CDTSDelim::InitOtherConnection(ConnectionPtr ptrOther)
{
	ptrOther->DataSource = m_bFileLoc;
	ptrOther->ConnectionProperties->Item("Data Source")->PutValue(m_bFileLoc);
	ptrOther->ConnectionProperties->Item("Mode")->PutValue(_variant_t((long)1));				
    ptrOther->ConnectionProperties->Item("Row Delimiter")->PutValue(m_bstrRowDelim);
    ptrOther->ConnectionProperties->Item("File Format")->PutValue (_variant_t((long)1));		
	ptrOther->ConnectionProperties->Item("Column Delimiter")->PutValue(m_bstrDelim);
	ptrOther->ConnectionProperties->Item("First Row Column Name")->PutValue(m_bHeaderLine);
	ptrOther->ConnectionProperties->Item("Skip Rows")->PutValue (_variant_t((long)m_lSkipRows));
	if(wcscmp(m_bstrTextQual, L"") != 0) 
		ptrOther->ConnectionProperties->Item("Text Qualifier")->PutValue(m_bstrTextQual);
	if (!m_bImportToSQL) {
		//get these from database ... for now...
		//_bstr_t bstrColumnNames = L"char1,varchar1,decimal1,float1,int1,bigint1,tinyint1,datetime1,bit1,char2,numeric1,decimal2,decimal3,money1,xxx";
		int iColNum = 0;
		_bstr_t bstrColumnNames = ptrADO->GetListofFieldNames(L"", L"", m_bstrSQLTable, iColNum);
		if (m_bHeaderLine == true) 
			ptrOther->ConnectionProperties->Item("Column Names")->PutValue(bstrColumnNames);
        ptrOther->ConnectionProperties->Item("Number of Column")->PutValue(_variant_t((long)iColNum));
	}
}

void CDTSDelim::LogInputParams()
{
	char buffer[SMALL_BUF_SIZE + 1];
	if (ptrLog == NULL) return;

	LPTSTR pzText = NULL;
	ptrLog->SetTimeStamp(true);
	ptrLog->WriteBlankLine();
	ptrLog->WriteString(_T("Input parameters for Data Transfer"));
	ptrLog->WriteString(_T("----------------------------------"));
	ptrLog->WriteBlankLine();

	if (m_bImportToSQL ) 
		pzText = (char*) "Delimited Text File Import to SQL Server Table";
	else 
		pzText = (char*) "SQL Server Table Export to Delimited Text File";
	ptrLog->WriteString(pzText);

	_LogInputParams();

	TCHAR szDelim[SMALL_BUF_SIZE]; memset(szDelim, 0, SMALL_BUF_SIZE);
	int iStrLen =  SysStringLen(m_bstrDelim);
	WideCharToMultiByte(CP_ACP, 0, m_bstrDelim, iStrLen, szDelim, iStrLen+1, NULL, NULL);
	szDelim[iStrLen]= '\0';
	memset(buffer, 0, SMALL_BUF_SIZE);
	sprintf(buffer, "Delimiter:\t\t%s", (_tcslen(szDelim) == 0  ? (char*) "A delimiter was not specified." : szDelim ) );
	ptrLog->WriteString(buffer);

	TCHAR szRowDelim[SMALL_BUF_SIZE]; memset(szRowDelim, 0, SMALL_BUF_SIZE);
	iStrLen =  SysStringLen(m_bstrDelim);
	WideCharToMultiByte(CP_ACP, 0, m_bstrDelim, iStrLen, szRowDelim, iStrLen+1, NULL, NULL);
	szRowDelim[iStrLen]= '\0';
	memset(buffer, 0, SMALL_BUF_SIZE);
	sprintf(buffer, "Row Delimiter:\t\t\t\t\t%s", ( _tcsclen(szRowDelim) == 0 ? (char*) "A row delimiter was not specified." : (_tcscmp(szRowDelim, _T("\n")) == 0 ? _T("\\n") : (_tcscmp(szRowDelim, _T("\r")) == 0 ? _T("\\r") : szRowDelim ))));
	ptrLog->WriteString(buffer);

	TCHAR szTextQual[SMALL_BUF_SIZE]; memset(szTextQual, 0, SMALL_BUF_SIZE);
	iStrLen =  SysStringLen(m_bstrTextQual);
	WideCharToMultiByte(CP_ACP, 0, m_bstrTextQual, iStrLen, szTextQual, iStrLen+1, NULL, NULL);
	szTextQual[iStrLen]= '\0';
	memset(buffer, 0, SMALL_BUF_SIZE);
	sprintf(buffer, "Text qualifier:\t\t\t%s", (_tcscmp(szTextQual, _T("")) == 0  ? (char*) "A text qualifier was not specified." : szTextQual) );
	ptrLog->WriteString(buffer);

	memset(buffer, 0, SMALL_BUF_SIZE);
	sprintf(buffer, "Header line information:\t%s", ((m_bHeaderLine  ) ? (char*) "included" : (char*) "not included" ) );
	ptrLog->WriteString(buffer);

}  //CDTSDelim::LogInputParams

HRESULT CDTSDelim::CheckInputParams()
{
	HRESULT hr = S_OK;
	_bstr_t bstrTemp = L"";
	FILE *pTemp = NULL;

	if (wcscmp(m_bstrDelim, L"") == 0)
		SetFatalError(L"DTRAN ERROR:  Delimeter was not specified.");
	
	if (m_nAction == nAPPEND && !m_bImportToSQL)  {
		SetFatalError(L"DTRAN ERROR:  vAppend is NOT valid DestAction for SQL Table Exports to Files.");
	}

	if (m_nAction == nOVERWRITE && !m_bImportToSQL)  {
		SetFatalError(L"DTRAN ERROR:  vOverWrite is NOT valid DestAction for SQL Table Exports to Files.");
	}

	if (m_nAction != nAPPEND && m_nAction != nOVERWRITE && m_nAction != nCREATE && m_nAction != nDROPADD)  {
		SetFatalError(L"DTRAN ERROR:  DestAction is invalid. Must be set to either: vCreate, vAppend, vOverWrite, vDropAdd.");
	}

//	if (m_bstrRowDelim.length() > 1 && m_iFirst > 0 && m_iLast > 0 ) 
//		SetFatalError(L"DTRAN ERROR: Row delimiter must be only one character long for partial file loads.");

//	_CheckInputParams();
	return hr;
} //CDTSDelim::CheckInputParams

//////////////////////////////////////////////////////////////////////
//
// CDTSDBF  -- used for DBF Transfers...
//
//////////////////////////////////////////////////////////////////////

CDTSDBF::CDTSDBF(_bstr_t bstrSQLServer,
					 _bstr_t bstrSQLDatabase,
					 _bstr_t bstrSQLTable,
					 bool bImportToSQL)
	    : CDTSTransfer(bstrSQLServer, bstrSQLDatabase, bstrSQLTable, bImportToSQL)
{
	m_nXBaseType = nFOXPRO;
	m_bDBFName = L"";
	m_bDBFDir = L"";

} //CDTSDelim::CDTSDelim

CDTSDBF::CDTSDBF(bool bImportToSQL )
        : CDTSTransfer(L"", L"", L"", bImportToSQL)
{
	m_nXBaseType = nDBASE;
	m_bDBFName = L"";
	m_bDBFDir = L"";
} //CDTSDBF::CDTSDBF

CDTSDBF::~CDTSDBF()
{
}  //CDTSDBF::~CDTSDBF


HRESULT CDTSDBF::Init(bool bTrace)
{
	USES_CONVERSION;
	TCHAR szDirPath[BUFF_LARGE]; memset(szDirPath, 0, BUFF_LARGE);
	TCHAR szDBFName[BUFF_MED];    memset(szDBFName, 0, BUFF_MED);
	TCHAR szDBFTable[BUFF_MED];    memset(szDBFTable, 0, BUFF_MED);
	TCHAR szDBFExt[BUFF_MED];    memset(szDBFExt, 0, BUFF_MED);
	TCHAR szFileLoc[BUFF_MED]; memset(szFileLoc, 0, BUFF_MED);
	
	m_bFileLoc = Utilities::ConstructFullPath(m_bDir, m_bFile);
	int iStrLen =  SysStringLen(m_bFileLoc);
	WideCharToMultiByte(CP_ACP, 0, m_bFileLoc, iStrLen, szFileLoc, iStrLen+1, NULL, NULL);
	szFileLoc[iStrLen]= '\0';
	Utilities::GetDirectoryPath(szFileLoc, szDirPath);
	_bstr_t bstrPath(szDirPath);
	m_bDBFDir = bstrPath;
	Utilities::ParseOutFileName(szFileLoc, szDBFName);
	_bstr_t bstrDBFName(szDBFName);
	Utilities::BreakupFileName(bstrDBFName, szDBFTable, szDBFExt);
	_bstr_t bstrDBFTable(szDBFTable);
	m_bDBFName = bstrDBFTable;
	
	HRESULT hr = CDTSTransfer::Init(bTrace);
	if (SUCCEEDED(hr)  ) {
		//create DSN for foxpro...if DSN is already present then don't create another
		if ( m_nXBaseType == nFOXPRO && wcscmp(m_bDSN, L"") == 0 && !ErrorsOccurred() ) {
			m_bDSN = Utilities::CreateDSN(W2A(m_bDBFDir), L"", L"");
			if ( wcscmp(m_bDSN, L"") == 0)
				hr = E_FAIL;
			else
				hr = S_OK;
			//hr = CreateDSN(m_bDBFDir);
		}
	}
	return hr;
} //CDTSDBF::Init

void CDTSDBF::Cleanup(bool bCreated)
{
	DeleteDSN();

} //CDTSDBF::Cleanup()

void CDTSDBF::InitOtherConnection(ConnectionPtr ptrOther)
{
	if ( m_nXBaseType == nFOXPRO ) {
		ptrOther->DataSource = m_bDSN;
		ptrOther->ConnectionProperties->Item("Persist Security Info")->PutValue((variant_t)true);  //?? VARIANT_TRUE
		ptrOther->ConnectionProperties->Item("Mode")->PutValue(_variant_t((long)1));				
		ptrOther->ConnectionProperties->Item("Data Source")->PutValue(m_bDSN);
	}
	else {
		ptrOther->DataSource = m_bDBFDir;
		ptrOther->ConnectionProperties->Item("Data Source")->PutValue((variant_t)m_bDBFDir);
        ptrOther->ConnectionProperties->Item("Extended Properties")->PutValue((variant_t)"dBase 5.0");
    }
	ptrOther->ConnectionTimeout = 60;

} //CDTSDBF::InitOtherConnection



void CDTSDBF::AddMoreTaskProperties(DataPumpTask2Ptr ptrCustomTask)
{
	TCHAR szSelectStmt[BUFF_MED]; memset(szSelectStmt, 0, BUFF_MED);
	if (m_bImportToSQL) {
		TCHAR szDBFName[BUFF_MED]; memset(szDBFName, 0, BUFF_MED);
		int iStrLen =  SysStringLen(m_bDBFName);
		WideCharToMultiByte(CP_ACP, 0, m_bDBFName, iStrLen, szDBFName, iStrLen+1, NULL, NULL);
		szDBFName[iStrLen]= '\0';
		_stprintf(szSelectStmt, sz_SQLSelectStmt, szDBFName);
	}
	else {
		TCHAR szSQLObjectName[BUFF_MED]; memset(szSQLObjectName, 0, BUFF_MED);
		int iStrLen =  SysStringLen(m_bstrSQLObjectName);
		WideCharToMultiByte(CP_ACP, 0, m_bstrSQLObjectName, iStrLen, szSQLObjectName, iStrLen+1, NULL, NULL);
		szSQLObjectName[iStrLen]= '\0';
		_stprintf(szSelectStmt, sz_SQLSelectStmt, szSQLObjectName);
	}

	ptrCustomTask->SourceSQLStatement = szSelectStmt;

}

void CDTSDBF::LogInputParams()
{
	TCHAR buffer[SMALL_BUF_SIZE + 1];
	if (ptrLog == NULL) return;
	ptrLog->SetTimeStamp(true);
	ptrLog->WriteBlankLine();
	ptrLog->WriteString(_T("Input parameters for Data Transfer"));
	ptrLog->WriteString(_T("----------------------------------"));
	ptrLog->WriteBlankLine();
	if (m_bImportToSQL ) 
		sprintf(buffer, _T("%s file import to SQL Server Table"), (m_nXBaseType == nDBASE ? sz_DBase : sz_FoxPro ) );
	else
		sprintf(buffer, _T("SQL Server Table Export to %s file"), (m_nXBaseType == nDBASE ? sz_DBase : sz_FoxPro) );
	ptrLog->WriteString(buffer);
	_LogInputParams();
}  //CDTSDBF::LogInputParams

HRESULT CDTSDBF::CheckInputParams()
{
	HRESULT hr = S_OK;
	_bstr_t bstrTemp = L"";
	FILE *pTemp = NULL;
	//_CheckInputParams();
	if (m_bDBFName.length() == 0)  {
		SetFatalError(L"DTRAN ERROR:  Not able to parse database name from input information.");
	}
	if (m_bDBFDir.length() == 0)  {
		SetFatalError(L"DTRAN ERROR:  Not able to parse full directory path name from input information.");
	}
	return hr;
} //CDTSDBF::CheckInputParams

HRESULT CDTSDBF::ConfirmDBFTable(ACTION nAction, DBFFIELD Fields[])
{
	bool bExists = false;
	HRESULT hr = E_FAIL;

	//does table exist?
	TCHAR szJunk[MAX_ERR_LEN];  memset(szJunk, 0, MAX_ERR_LEN);
	TCHAR szFileLoc[BUFF_MED]; memset(szFileLoc, 0, BUFF_MED);
	int iStrLen =  SysStringLen(m_bFileLoc);
	WideCharToMultiByte(CP_ACP, 0, m_bFileLoc, iStrLen, szFileLoc, iStrLen+1, NULL, NULL);
	szFileLoc[iStrLen]= '\0';

	if (Utilities::FileCanBeRead(szFileLoc, szJunk) )
		bExists = true;
	
	switch (nAction)
	{
		case nCREATE:
			if (bExists) 
					SetFatalError(L"DTRAN ERROR:  Table can not be created, already exists.");
			else {
				Utilities::CreateDBF((m_nXBaseType == nDBASE ? 1 : 0 ), Fields, szFileLoc);
				//test to see if it is there
				if (!Utilities::FileCanBeRead(szFileLoc, szJunk) )
					SetFatalError(L"DTRAN ERROR:  CDTSDBF::ConfirmDBFTable -- Can not create table.");
			}
			break;
		case nOVERWRITE:
			if (bExists)
			{
				//figure out a way to delete records...
			}
			else
				SetFatalError(L"DTRAN ERROR:  Table can not be overwritten, does not exist.");
			break;
		case nAPPEND:
			if (!bExists)
				SetFatalError(L"DTRAN ERROR:  Table can not be appended, does not exist.");
			break;
		case nDROPADD:
			if (bExists) {
				if (DeleteFile(szFileLoc) != 0 ) {
					Utilities::CreateDBF((m_nXBaseType == nDBASE ? 1 : 0 ), Fields, szFileLoc);
					//test to see if it is there
					if (!Utilities::FileCanBeRead(szFileLoc, szJunk) )
						SetFatalError(L"DTRAN ERROR:  CDTSDBF::ConfirmDBFTable -- Can not create table.");
				}
				else {
					_bstr_t bstrTemp = L"DTRAN ERROR: CDTSDBF::ConfirmDBFTable -- Problem occurred when dropping the table.";
					SetFatalError(bstrTemp);
				}
			}
			else
				SetFatalError(L"DTRAN ERROR:  Can not DropAdd table, table does not exist.");

			break;
		default:
			if (!bExists)
				SetFatalError(L"DTRAN ERROR:  Table does not exist");
			break;
	}

	if (!ErrorsOccurred())
		hr = S_OK;
	return hr;
} //CDTSDBF::ConfirmDBFTable