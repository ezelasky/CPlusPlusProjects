// Process.cpp : Implementation of CProcess
#include "stdafx.h"
#include "DTran.h"
#include "ADO.h"
#include "Process.h"
#include "BaseTransfer.h"
//#include <time.h>

#include <list>

#include "Utilities.h"
/////////////////////////////////////////////////////////////////////////////
// Helper Functions
/*
_bstr_t ConstructBCPFileName()
{
	_bstr_t bstrFormatLoc = L"";
	
	TCHAR szBufDir[512];
	GetCurrentDirectory(512, szBufDir);
	bstrFormatLoc = (_bstr_t) szBufDir;
	bstrFormatLoc += L"\\";
	
	srand((unsigned)time( NULL ));
	int i = rand();
	TCHAR unique[20];
	itoa(i, unique, 10);
	unique[strlen(unique)+1] = '\0';

	bstrFormatLoc += (_bstr_t)unique;

	bstrFormatLoc += L"BCPTemp.fmt";
			
	return bstrFormatLoc;
}
*/

/////////////////////////////////////////////////////////////////////////////
// CProcess


STDMETHODIMP CProcess::put_TraceLogOn(VARIANT_BOOL newVal)
{
	m_bTraceLogOn = newVal;
	return S_OK;
}

STDMETHODIMP CProcess::get_TraceLogOn(VARIANT_BOOL* pVal)
{
	*pVal = m_bTraceLogOn;
	return S_OK;
}

STDMETHODIMP CProcess::IsSQLObject(IDispatch *pIDisp, VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	IdeSQL* pSQL = NULL;
	pIDisp->QueryInterface(IID_IdeSQL, (void**) &pSQL);				
	if (pSQL != NULL) {
		bSuccess = VARIANT_TRUE;
		pSQL->Release();
	}

	pSQL = NULL;
	*pRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CProcess::IsDelimFileObject(IDispatch *pIDisp, VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	IdeDelim* pDelim = NULL;
	pIDisp->QueryInterface(IID_IdeDelim, (void**) &pDelim);				
	if (pDelim != NULL) {
		bSuccess = VARIANT_TRUE;
		pDelim->Release();
	}
	pDelim = NULL;
	*pRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CProcess::IsFixedFileObject(IDispatch *pIDisp, VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	IdeFixed* pFixed = NULL;
	pIDisp->QueryInterface(IID_IdeFixed, (void**) &pFixed);				
	if (pFixed != NULL) {
		bSuccess = VARIANT_TRUE;
		pFixed->Release();
	}
	pFixed = NULL;
	*pRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CProcess::IsXBaseObject(IDispatch *pIDisp, VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	IdeXBase* pXBase = NULL;
	pIDisp->QueryInterface(IID_IdeXBase, (void**) &pXBase);				
	if (pXBase != NULL) {
		bSuccess = VARIANT_TRUE;
		pXBase->Release();
	}
	pXBase = NULL;
	*pRetVal = bSuccess;
	return S_OK;
}


STDMETHODIMP CProcess::MultiImportToSQL(IDispatch* pIDisp, IdeSQL* pSQL, VARIANT_BOOL* pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;

	TCHAR szQuery[MAX_QUERY_LEN];   memset(szQuery, 0, MAX_QUERY_LEN);
	TCHAR szError[MAX_ERR_LEN]; 	memset(szError, 0, MAX_ERR_LEN);
	BSTR bTemp;

	if (pIDisp == NULL || pSQL == NULL )  {
		m_listErrors.push_back(L"ERROR: CProcess::MultiImportToSQL() -- Either input source object or deSQL object is NULL.  Processing can not continue.");
		return S_OK;
	}

	IdeFixed* pFixed = NULL;
	IdeDelim* pDelim = NULL;
	pIDisp->QueryInterface(IID_IdeFixed, (void**) &pFixed);
	pIDisp->QueryInterface(IID_IdeDelim, (void**) &pDelim);
	if (pFixed == NULL && pDelim == NULL ){
		m_listErrors.push_back(L"ERROR: CProcess::MultiImportToSQL() -- Input source object is neither deFixed or deDelim object. Processing can not continue.");
		return S_OK;
	}

	if (pFixed != NULL)
		pFixed->get_FileName(&bTemp);
	else 
		pDelim->get_FileName(&bTemp);

	_bstr_t bstrFileName = bTemp;
	SysFreeString(bTemp);

	if (pFixed != NULL)
		pFixed->get_Directory(&bTemp);
	else 
		pDelim->get_Directory(&bTemp);
	_bstr_t	bstrDir = bTemp;
	_bstr_t bstrDirOrig = bstrDir;
	SysFreeString(bTemp);

	TCHAR szDirPath[512];
	memset(szDirPath, 0, 512);

	if (Utilities::GetDirectoryPath((TCHAR*) bstrFileName, szDirPath) != 0 )  {  

		//filename can contain full path or partial path name
		if (Utilities::IsFullPath((TCHAR*) bstrFileName) )
			bstrDir = (_bstr_t) szDirPath;
		else 
			bstrDir = Utilities::ConstructFullPath(bstrDir, (_bstr_t)szDirPath);
		if (pFixed != NULL)
			pFixed->put_Directory(bstrDir);  
		else
			pDelim->put_Directory(bstrDir);  
	}

	TCHAR szDirOrig[512];
	GetCurrentDirectory(512, szDirOrig);

	std::list<_bstr_t> listFileName;
	int iFileCnt = 0;

	HANDLE          hFind = 0;
    WIN32_FIND_DATA wfd = {0};
	SetCurrentDirectory((char*)bstrDir);
	if (pFixed != NULL)
		pFixed->put_Directory(bstrDir);
	else
		pDelim->put_Directory(bstrDir);

	TCHAR szFileBuffer[1000]; memset(szFileBuffer, 0, 1000);
	_bstr_t bstrFile = bstrFileName;
	if ( Utilities::ParseOutFileName((TCHAR*) bstrFileName, szFileBuffer) > 0)
		bstrFile = (_bstr_t) szFileBuffer;

	if ((hFind = FindFirstFile((char*)bstrFile, &wfd)) != INVALID_HANDLE_VALUE) {

		BOOL bResult = FALSE;
		iFileCnt++;
		listFileName.push_back((_bstr_t) wfd.cFileName);
		
		while ( FindNextFile(hFind, &wfd) == TRUE) {
			listFileName.push_back((_bstr_t) wfd.cFileName);
			iFileCnt++;
		}
		FindClose(hFind);
	}

	//set back to original directory for processing
	SetCurrentDirectory(szDirOrig);

	long lRows = 0;
//	_bstr_t bstrErrors = L"";
	m_iLoadCnt = iFileCnt;
	m_bMultiLoad = true;
	
	//added sourcecolumn to table...
	CADO objADO(L"", L"");
	bool bAddTable = true;
	bool bAddCol = true;

	pSQL->get_Owner(&bTemp);
	_bstr_t bstrOwner = bTemp;
	SysFreeString(bTemp);

	pSQL->get_Table(&bTemp);
	_bstr_t bstrTable = bTemp;			//target table
	SysFreeString(bTemp);
	_bstr_t bstrSrceTable = bstrTable;  //source table 
	bstrSrceTable += L"_Source";
	int iSrceCnt = 0;
	LPTSTR pzUpdateSQL = _T("UPDATE %s.[%s] SET DTran_Source = '%d' WHERE DTran_Source IS NULL");	 //update target table
	LPTSTR pzInsertSQL = _T("INSERT INTO  %s.[%s] (SourceID, Source) VALUES (%d, '%s')");			 //insert into source table
	LPTSTR pzAlterSQL = _T("ALTER TABLE %s ADD DTran_Source int");

	if (m_bAddSource) 
	{
		pSQL->get_Server(&bTemp);
		_bstr_t bstrServer = bTemp;
		SysFreeString(bTemp);

		pSQL->get_Database(&bTemp);
		_bstr_t bstrDatabase = bTemp;
		SysFreeString(bTemp);

		if ( SUCCEEDED(objADO.OpenTrustedConnection(bstrServer, bstrDatabase)) ) 
		{
			//does Source tracking table exist?
			if ( objADO.TableExists(bstrServer, bstrDatabase, bstrSrceTable) ) 
			{
				if (m_nAction == vAPPEND) 
				{
					//get last select MAX(SourceID) from source table
					memset(szQuery, 0, MAX_QUERY_LEN);
					_stprintf(szQuery, _T("SELECT MAX(SourceID) from %s"), (TCHAR*)bstrSrceTable);
					ADODB::_RecordsetPtr spRecordset = NULL;
					if (SUCCEEDED(objADO.GetRecordSet((IDispatch**) &spRecordset, (_bstr_t) szQuery)) ) 
					{
						if ( spRecordset != NULL) 
						{
							if (spRecordset->RecordCount != 0 ) 
							{
								spRecordset->MoveFirst();
								if (!spRecordset->adoEOF)
								{
									_variant_t varIndex; varIndex.vt = VT_I2; varIndex.iVal = 0; 
									_variant_t varValue = spRecordset->Fields->Item[varIndex]->Value;
									varValue.ChangeType(VT_I4);
									iSrceCnt = varValue.intVal;
								}
							}
						}
					}
					else {
						Utilities::TransferErrors(objADO.m_listErrors, m_listErrors);
					}
					bAddTable = false;
				}
				else if (m_nAction == vCREATE || m_nAction == vDROPADD || m_nAction == vOVERWRITE)
				{
					memset(szQuery, 0, MAX_QUERY_LEN);
					_stprintf(szQuery, _T("DROP TABLE %s"), (TCHAR*)bstrSrceTable);
					if (!SUCCEEDED(objADO.ExecuteQuery(szQuery))) 
					{
						Utilities::TransferErrors(objADO.m_listErrors, m_listErrors);
						m_bAddSource = VARIANT_FALSE; //turn this off, something is amiss...
					}
				}
			}
		
			//does column exist in table?
			if (m_nAction == vAPPEND || m_nAction == vOVERWRITE) //won't be there on vCREATE or vOVERWRITE
			{
				bool bFillInNulls = false;
				if (objADO.ColumnExists(bstrTable, L"DTran_Source") ) 
				{
					if (m_nAction == vAPPEND) 
						bFillInNulls = true;  //place zero in any NULL values in DTran_Source, probably won't be any, but just in case
					bAddCol = false;
				}
				else 
				{
					if (m_nAction == vAPPEND)
					{	//add the column  
						_stprintf(szQuery, pzAlterSQL, (TCHAR*) bstrTable);
						if (!SUCCEEDED(objADO.ExecuteQuery(szQuery))) 
						{
							Utilities::TransferErrors(objADO.m_listErrors, m_listErrors);
							m_bAddSource = VARIANT_FALSE; //turn this off, something is amiss...
						}
						else 
						{
							bAddCol = false;
							bFillInNulls = true; 
						}
					}
				}

				if (bFillInNulls) 
				{
					memset(szQuery, 0, MAX_QUERY_LEN);
					_stprintf(szQuery, pzUpdateSQL, (TCHAR*) bstrOwner, (TCHAR*)bstrTable, 0);
					if (!SUCCEEDED(objADO.ExecuteQuery(szQuery)))
						Utilities::TransferErrors(objADO.m_listErrors, m_listErrors);
				}
			}
		}
		else
			m_bAddSource = VARIANT_FALSE;
	}

	std::list<_bstr_t>::iterator iterList;
	for (iterList = listFileName.begin(); iterList != listFileName.end(); iterList++)  {
		
		_bstr_t bstrFile = (_bstr_t)(*iterList);
		bTemp = SysAllocString(bstrFile);
		if (pFixed != NULL)
			pFixed->put_FileName(bTemp);
		else
            pDelim->put_FileName(bTemp);
		SysFreeString(bTemp);

		m_lRowsLoaded = 0;
		if (pFixed != NULL)
			FixedToSQL(pFixed, pSQL, &bSuccess);
		else 
			DelimToSQL(pDelim, pSQL, &bSuccess);
	
		lRows += m_lRowsLoaded;
//		bstrErrors += m_bstrErrMsgs;
		m_nAction = vAPPEND;  //always true from here on out regardless of initial action

		//stop once an error is encountered of any type....
		if (bSuccess == VARIANT_FALSE) 
			break;
		else 
		{
			if (m_bAddSource ) 
			{
				if (bAddTable) 
				{
					//add the table, then add first entry --> SourceID = 0, Source = 'missing'
					memset(szQuery, 0, MAX_QUERY_LEN);
					_stprintf(szQuery, "CREATE TABLE %s.[%s] (SourceID int, Source varchar(500) )", (TCHAR*) bstrOwner, (TCHAR*) bstrSrceTable );
					if (!SUCCEEDED(objADO.ExecuteQuery(szQuery))) {
						Utilities::TransferErrors(objADO.m_listErrors, m_listErrors);
						m_bAddSource = VARIANT_FALSE; //turn this off, something is amiss...
					}
					
					memset(szQuery, 0, MAX_QUERY_LEN);
					_stprintf(szQuery, pzInsertSQL, (TCHAR*) bstrOwner, (TCHAR*) bstrSrceTable, 0, _T("missing") );
					if (!SUCCEEDED(objADO.ExecuteQuery(szQuery))) {
						Utilities::TransferErrors(objADO.m_listErrors, m_listErrors);
						m_bAddSource = VARIANT_FALSE; //turn this off, something is amiss...
					}

					bAddTable = false;
				}

				if (bAddCol) {
					//add the column
					_stprintf(szQuery, pzAlterSQL, (TCHAR*) bstrTable);
					if (!SUCCEEDED(objADO.ExecuteQuery(szQuery))) {
						Utilities::TransferErrors(objADO.m_listErrors, m_listErrors);
						m_bAddSource = VARIANT_FALSE; //turn this off, something is amiss...
					}
					bAddCol = false;
				}

				iSrceCnt++;

				//add entry -- source id and source file name to source tracking table
				memset(szQuery, 0, MAX_QUERY_LEN);
				_stprintf(szQuery, pzInsertSQL, (TCHAR*) bstrOwner, (TCHAR*) bstrSrceTable, iSrceCnt, (TCHAR*) bstrFile );
				if (!SUCCEEDED(objADO.ExecuteQuery(szQuery)))
						Utilities::TransferErrors(objADO.m_listErrors, m_listErrors);

				//update target table with source id 
				memset(szQuery, 0, MAX_QUERY_LEN);
				_stprintf(szQuery, pzUpdateSQL, (TCHAR*) bstrOwner, (TCHAR*)bstrTable,  iSrceCnt);
				if (!SUCCEEDED(objADO.ExecuteQuery(szQuery)))
						Utilities::TransferErrors(objADO.m_listErrors, m_listErrors);
			}
		}
	}

	if (pFixed != NULL) {
		pFixed->put_FileName(bstrFileName);
		pFixed->put_Directory(bstrDirOrig);  
		pFixed->Release();
		pFixed = NULL;
	}
	else {
		pDelim->put_FileName(bstrFileName);
		pDelim->put_Directory(bstrDirOrig);  
		pDelim->Release();
		pDelim = NULL;
	}

	objADO.CloseConnection();
	m_lRowsLoaded = lRows;

	if (iFileCnt == 0) {
		_stprintf(szError, _T("ERROR: CProcess::MultiFixedToSQL() -- No files were found based on directory:  %s and file name: %s specified in input deFixed object."), (TCHAR*) bstrDir, (TCHAR*) bstrFile);
		m_listErrors.push_back((_bstr_t) (TCHAR*) szError);
	}

	*pRetVal = bSuccess;
	return S_OK;
}


STDMETHODIMP CProcess::FixedToSQL(IdeFixed *pFixed, IdeSQL *pSQL,  VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	BSTR bTemp;
	m_iLoadCnt--;
	*pRetVal = bSuccess;
	TCHAR szError[MAX_ERR_LEN];
	memset(szError, 0, MAX_ERR_LEN);


	if (pFixed == NULL || pSQL == NULL )  {
		m_listErrors.push_back(L"ERROR: CProcess::FixedToSQL() -- Either deFixed object or deSQL object is NULL.  Processing can not continue.");
			return S_OK;
	}

	IFormat* pFixedFormat = NULL;
	pFixed->get_Format(&pFixedFormat);
	if (pFixedFormat == NULL) {
		m_listErrors.push_back(L"ERROR: CProcess::FixedToSQL() -- Format object in deFixed object is NULL.  Processing can not continue.");
		return S_OK;
	}

	IFormat* pSQLFormat = NULL;
	pSQL->get_Format(&pSQLFormat);
	if (pSQLFormat == NULL) {
		m_listErrors.push_back(L"ERROR: CProcess::FixedToSQL() -- Format object in deSQL object is NULL.  Processing can not continue.");
		return S_OK;
	}

	bool bLoadSQL = false;
	bool bLoadFixed = false;
	long lFixedFldCnt = 0;
	long lSQLFldCnt = 0;
	pFixedFormat->get_FieldCount(&lFixedFldCnt);
	pSQLFormat->get_FieldCount(&lSQLFldCnt);

	if (lFixedFldCnt == 0 && lSQLFldCnt == 0) 
	{
		pFixed->LoadFormat(&bSuccess);   
		if (!bSuccess)
			m_listErrors.push_back(L"ERROR: CProcess::FixedToSQL() -- Failure loading field collection via LoadFormat.");  //for now....
		pFixedFormat->get_FieldCount(&lFixedFldCnt);
		if (lFixedFldCnt == 0)
			m_listErrors.push_back(L"ERROR: CProcess::FixedToSQL() -- Field collection is empty.Processing can not continue.");
		else
			bLoadSQL = true;
	}
	else 
	{   //if one of these has zero fields then copy the other to it, which we know is populated
		if (lSQLFldCnt == 0) 
			bLoadSQL = true;
		else if (lFixedFldCnt == 0) 
			bLoadFixed = true;
		else
			bSuccess = VARIANT_TRUE;
	}

	if (bLoadSQL) {
		pSQLFormat->CopyFormat(pFixedFormat, &bSuccess);
		if (!bSuccess) {
			m_listErrors.push_back(L"ERROR: CProcess::FixedToSQL() -- Attempt to copy deDelim format into deSQL object failed.");
			TransferErrors(&pSQLFormat);
		}
	}
	
	if (bLoadFixed) {
		pFixedFormat->CopyFormat(pSQLFormat, &bSuccess);
		if (!bSuccess) {
			m_listErrors.push_back(L"ERROR: CProcess::FixedToSQL() -- Attempt to copy deDelim format into deSQL object failed.");
			TransferErrors(&pFixedFormat);
		}
	}

	if (bSuccess) 
	{
		pFixedFormat->ValidFieldCollection(&bSuccess);
		if (!bSuccess)
			TransferErrors(&pFixedFormat);
	}

	if (bSuccess) {

		//BCP transfer .... set pertinant data variables....from objects
		CBCPTransfer objBCP(true);
		objBCP.m_nAction = (ACTION)m_nAction;
	
		pSQL->get_Database(&bTemp);
		objBCP.m_bstrSQLDatabase = bTemp;
		SysFreeString(bTemp);

		pSQL->get_Server(&bTemp);
		objBCP.m_bstrSQLServer = bTemp;
		SysFreeString(bTemp);

		pSQL->get_Table(&bTemp);
		objBCP.m_bstrSQLTable = bTemp;
		SysFreeString(bTemp);

		pSQL->get_Owner(&bTemp);
		objBCP.m_bstrSQLOwner = bTemp;
		SysFreeString(bTemp);

		long lTemp = SQLBATCHSIZE;
		pSQL->get_ImportBatchSize(&lTemp);
		objBCP.m_lBatchSize = lTemp;

		lTemp = MAXERRMSGS;
		pSQL->get_Err_MaxMessages(&lTemp);
		objBCP.m_lMaxErrMsgs = lTemp;

		VARIANT_BOOL vbTemp = VARIANT_TRUE;
		pSQL->get_Err_NoInfo(&vbTemp);
		objBCP.m_bMinErrInfo = (vbTemp == VARIANT_TRUE ? true :false);

		pFixed->get_Directory(&bTemp);
		objBCP.m_bDir= bTemp;
		SysFreeString(bTemp);

		pFixed->get_FileName(&bTemp);
		objBCP.m_bFile = bTemp; 
		SysFreeString(bTemp);

		pFixed->get_FirstRow(&lTemp);
		objBCP.m_lFirst = lTemp;
		pFixed->get_LastRow(&lTemp);
		objBCP.m_lLast = lTemp;

		pFixedFormat->get_BCPFormatFileLoc(&bTemp);
		_bstr_t bstrFormatLoc = bTemp;
		SysFreeString(bTemp);

		if (wcscmp(bstrFormatLoc, L"") == 0 ) {
	
			bstrFormatLoc  = (_bstr_t) Utilities::ConstructFileName("BCPFormat", "FMT");
			pFixedFormat->Write(bstrFormatLoc, vBCPFORMAT, &bSuccess);
			if (bSuccess) {
				pFixedFormat->get_BCPFormatFileLoc(&bTemp);
				bstrFormatLoc = bTemp;
				SysFreeString(bTemp);
				m_bCreatedFMT = true;
			}
		}
		objBCP.m_bFormatFileLoc = bstrFormatLoc;

		//multiloading will reuse these....instead of creating new on each one...check to see if it is there...
		objBCP.m_bDSN = m_bstrDSN;
		objBCP.m_bstrTraceFileName = m_bstrLogFileName; 

		HRESULT hr = objBCP.Init(((m_bTraceLogOn == VARIANT_TRUE) ? true : false));
		if (!objBCP.ErrorsOccurred() )	{
	
			m_bstrDSN = objBCP.m_bDSN;
			m_bstrLogFileName = objBCP.m_bstrTraceFileName;

			if ( !m_bTableOK ) {

				pSQL->GetCreateTableSQL(&bTemp);
				_bstr_t bstrSQL = bTemp;
				SysFreeString(bTemp);
				hr = objBCP.ConfirmSQLTable((ACTION)m_nAction, bstrSQL);
			}
	
			if (SUCCEEDED(hr)  && !objBCP.ErrorsOccurred()) {
				m_bTableOK = true;
				hr = objBCP.Transfer();
			}
			m_lRowsLoaded = objBCP.m_lRowsLoaded;
		}

		if ( m_iLoadCnt == 0 || !SUCCEEDED(hr) || objBCP.ErrorsOccurred() ) {	//last file to load or cleanup if multiload is going to abort
		
			objBCP.Cleanup(m_bCreatedFMT); 
			if ( m_bCreatedFMT ) {
				if (pFixedFormat)
					pFixedFormat->put_BCPFormatFileLoc(L"");
			}

			m_bCreatedFMT = false;
			m_bstrDSN = L"";
		}

		if( objBCP.ErrorsOccurred() )  {
			Utilities::TransferErrors(objBCP.m_listErrors, m_listErrors);
			bSuccess = VARIANT_FALSE;
		}
		else
			bSuccess = VARIANT_TRUE;

	}

	if (pSQLFormat) {
		pSQLFormat->Release();
		pSQLFormat = NULL;
	}

	if (pFixedFormat) {
		pFixedFormat->Release();
		pFixedFormat = NULL;
	}

	*pRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CProcess::SQLToFixed(IdeSQL *pSQL, IdeFixed *pFixed, long lChunk, long lFileNum, VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	BSTR bTemp;
	*pRetVal = bSuccess;
	TCHAR szError[MAX_ERR_LEN];
	memset(szError, 0, MAX_ERR_LEN);

	bool bCreatedFMT = false;
//	bool bExportQuery = false;

	if (pFixed == NULL || pSQL == NULL )  {
		m_listErrors.push_back(L"ERROR: CProcess::SQLToFixed() -- Either deFixed object or deSQL object is NULL.  Processing can not continue.");
		return S_OK;
	}

	IFormat* pFixedFormat = NULL;
	pFixed->get_Format(&pFixedFormat);
	if (pFixedFormat == NULL) {
		m_listErrors.push_back(L"ERROR: CProcess::SQLToFixed() -- Format object in deFixed object is NULL.  Processing can not continue.");
		return S_OK;
	}

	IFormat* pSQLFormat = NULL;
	pSQL->get_Format(&pSQLFormat);
	if (pSQLFormat == NULL) {
		m_listErrors.push_back(L"ERROR: CProcess::SQLToFixed() -- Format object in deSQL object is NULL.  Processing can not continue.");
		return S_OK;
	}

	CBCPTransfer objBCP(false);
	objBCP.m_lChunkSize = lChunk; 
	objBCP.m_lFileNum = lFileNum;
	objBCP.m_nAction = (ACTION)m_nAction;

	//set up for export query....we just need to reset this at the end, there are a lot of behind the scenes interactions...
//	pSQL->get_ExportQuery(&bTemp);
//	_bstr_t bstrExportQuery = bTemp;
//	SysFreeString(bTemp);
//	if(wcscmp(bstrExportQuery, L"") != 0 ) 
//		bExportQuery = true;

	bool bLoadSQL = false;
	bool bLoadFixed = false;
	long lFixedFldCnt = 0;
	long lSQLFldCnt = 0;
	pFixedFormat->get_FieldCount(&lFixedFldCnt);
	pSQLFormat->get_FieldCount(&lSQLFldCnt);
	if (lFixedFldCnt == 0 && lSQLFldCnt == 0) 
	{
		pSQL->LoadFormat(&bSuccess);   
		if (!bSuccess)
			m_listErrors.push_back(L"ERROR: CProcess::SQLToFixed() -- Failure loading field collection via LoadFormat.");  //for now....
		pSQLFormat->get_FieldCount(&lSQLFldCnt);
		if (lSQLFldCnt == 0)
			m_listErrors.push_back(L"ERROR: CProcess::SQLToFixed() -- Field collection is empty.Processing can not continue.");
		else
			bLoadFixed = true;
	}
	else 
	{   //if one of these has zero fields then copy the other to it, which we know is populated
		if (lSQLFldCnt == 0) 
			bLoadSQL = true;
		else if (lFixedFldCnt == 0) 
			bLoadFixed = true;
		else
			bSuccess = VARIANT_TRUE;
	}

	if (bLoadSQL) {
		pSQLFormat->CopyFormat(pFixedFormat, &bSuccess);
		if (!bSuccess) {
			m_listErrors.push_back(L"ERROR: CProcess::SQLToFixed() -- Attempt to copy deDelim format into deSQL object failed.");
			TransferErrors(&pSQLFormat);
		}
	}
	
	if (bLoadFixed) {
		pFixedFormat->CopyFormat(pSQLFormat, &bSuccess);
		if (!bSuccess) {
			m_listErrors.push_back(L"ERROR: CProcess::SQLToFixed() -- Attempt to copy deDelim format into deSQL object failed.");
			TransferErrors(&pFixedFormat);
		}
	}

	if (bSuccess) 
	{
		pSQLFormat->ValidFieldCollection(&bSuccess);
		if (!bSuccess)
			TransferErrors(&pSQLFormat);
	}

	pSQL->get_Database(&bTemp);
	objBCP.m_bstrSQLDatabase = bTemp;
	SysFreeString(bTemp);

	pSQL->get_Server(&bTemp);
	objBCP.m_bstrSQLServer = bTemp;
	SysFreeString(bTemp);

	pSQL->get_Owner(&bTemp);
//	_bstr_t bstrOrigOwner = bTemp;
	objBCP.m_bstrSQLOwner = bTemp;
	SysFreeString(bTemp);

	pSQL->get_Table(&bTemp);
	_bstr_t bstrOrigTable = bTemp;
	objBCP.m_bstrSQLTable = bTemp;
	SysFreeString(bTemp);

	long lTemp = SQLBATCHSIZE;
	pSQL->get_ImportBatchSize(&lTemp);
	objBCP.m_lBatchSize = lTemp;

	lTemp = MAXERRMSGS;
	pSQL->get_Err_MaxMessages(&lTemp);
	objBCP.m_lMaxErrMsgs = lTemp;

	VARIANT_BOOL vbTemp = VARIANT_TRUE;
	pSQL->get_Err_NoInfo(&vbTemp);
	objBCP.m_bMinErrInfo = (vbTemp == VARIANT_TRUE ? true :false);

	pSQLFormat->get_BCPFormatFileLoc(&bTemp);
	_bstr_t bstrFormatLoc = bTemp;
	SysFreeString(bTemp);

	pFixed->get_Directory(&bTemp);
	objBCP.m_bDir= bTemp;
	SysFreeString(bTemp);

	pFixed->get_FileName(&bTemp);
	objBCP.m_bFile = bTemp; 
	SysFreeString(bTemp);

	_bstr_t bstrExtraField = (_bstr_t) sz_ExtraFieldName;
	if ( bSuccess ) {

		IFieldInfo* pField = NULL;
		pSQLFormat->Last(&pField);
		if (pField) 
		{	//this is a work around for the ragged edge problem...carridge return is not in same position 
			//when last field contains NULL or varchar, this causes problems in downstream geocoding of fixed file
			pField->get_Name(&bTemp);
			_bstr_t bstrField(bTemp);
			SysFreeString(bTemp);
			if (objBCP.m_bstrSQLServer.length() != 0 && objBCP.m_bstrSQLDatabase.length() != 0 &&  objBCP.m_bstrSQLTable.length() != 0 && bstrField.length() != 0 ) 
			{
				CADO objADO("", "");
				if (objADO.ColumnTextLengthVaries(objBCP.m_bstrSQLTable, bstrField, objBCP.m_bstrSQLServer, objBCP.m_bstrSQLDatabase) )
				{
					pSQLFormat->AddField(bstrExtraField, L"2", vCHAR, &bSuccess);
					if (bSuccess) {
						objBCP.m_bVariedTextLenQuery = true;
						pSQLFormat->put_CarRetInBCP(VARIANT_FALSE);
					}
				}
			}
			pField->Release();
		}
		pSQLFormat->RecordLength(&objBCP.m_lRecLen);
	}
	
	if ( bSuccess ) {
		bstrFormatLoc  = (_bstr_t) Utilities::ConstructFileName("BCPFormat", "FMT");
		pSQLFormat->Write(bstrFormatLoc, vBCPFORMAT, &bSuccess);
		if (bSuccess) {
			pSQLFormat->get_BCPFormatFileLoc(&bTemp);
			bstrFormatLoc = bTemp;
			SysFreeString(bTemp);
			m_bCreatedFMT = true;
			
			if (objBCP.m_bVariedTextLenQuery) 
				pSQLFormat->DeleteByName(bstrExtraField, &bSuccess);  //just needed to add this to get the BCP format correct for this ragged edge thing
		}
	}
	objBCP.m_bFormatFileLoc = bstrFormatLoc;
	objBCP.m_bDSN = m_bstrDSN;
	objBCP.m_bstrTraceFileName = m_bstrLogFileName; 

	if (bSuccess) 
	{
		HRESULT hr = objBCP.Init(((m_bTraceLogOn == VARIANT_TRUE) ? true : false));
		m_bstrLogFileName = objBCP.m_bstrTraceFileName;
		if (!objBCP.ErrorsOccurred() )	{

			//hr = objBCP.ConfirmSQLTable(nCONFIRM, L"");
	
			if (SUCCEEDED(hr) && !objBCP.ErrorsOccurred())
					hr = objBCP.Transfer();

			if (SUCCEEDED(hr) && !objBCP.ErrorsOccurred() ) {
			
				TCHAR szFormatName[MAX_FILE_LEN]; memset(szFormatName, 0, MAX_FILE_LEN);
				if (Utilities::ChangeFileExtension(objBCP.m_bFileLoc, _T("fmt"), szFormatName) > 0 )  {
					//pSQLFormat->Write((_bstr_t) szFormatName, vTNSFORMAT, &bSuccess);
					pSQLFormat->Write((_bstr_t) szFormatName, vCUSTFORMAT, &bSuccess);
				}
				m_lRowsLoaded = objBCP.m_lRowsLoaded;
				//m_bTruncationWarning = objBCP.TruncationOccurred();
			}
		}

		if( objBCP.ErrorsOccurred() )  {
			Utilities::TransferErrors(objBCP.m_listErrors, m_listErrors);
			bSuccess = VARIANT_FALSE;
		}
		else
			bSuccess = VARIANT_TRUE;

	}

	//clean up
	if ( m_bCreatedFMT ) 
			pSQLFormat->put_BCPFormatFileLoc(L"");
	objBCP.Cleanup(m_bCreatedFMT); 
	
//	if(bExportQuery) 
//		pSQL->put_ExportQuery(bstrExportQuery);  //there is a lot of behind the scenes logic in setting these properties...so we need to set this 
												 //to delete the temporary table if it was created....
	if (pSQLFormat) {
		pSQLFormat->Release();
		pSQLFormat = NULL;
	}

	if (pFixedFormat) {
		pFixedFormat->Release();
		pFixedFormat = NULL;
	}

	*pRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CProcess::get_ErrMsgs(BSTR *pVal)
{
	_bstr_t bstrErrors = L"";
	try {

		bstrErrors = Utilities::GetErrorList(m_listErrors, false);
	}
		catch(_com_error &e) {
		bstrErrors = Utilities::ConstructErrMsg(L" CFormat::get_ErrMsgList() catch statement.", e.Source(), e.Description(), e.ErrorMessage());
	}
	catch (...) {
		bstrErrors = L"ERROR: CFormat::get_ErrMsgList() catch statement.";
	}

	*pVal = bstrErrors.copy();
	return S_OK;
}

STDMETHODIMP CProcess::get_RowsLoaded(long *pVal)
{
	*pVal = m_lRowsLoaded;
	return S_OK;
}

STDMETHODIMP CProcess::put_RowsLoaded(long newVal)
{
	m_lRowsLoaded = newVal;
	return S_OK;
}

STDMETHODIMP CProcess::get_TraceFileName(BSTR *pVal)
{
	*pVal = m_bstrLogFileName.copy();

	return S_OK;
}

STDMETHODIMP CProcess::put_TraceFileName(BSTR newVal)
{
	m_bstrLogFileName = newVal;

	return S_OK;
}



STDMETHODIMP CProcess::get_Action(DESTACTION *pVal)
{
	*pVal = m_nAction;

	return S_OK;
}

STDMETHODIMP CProcess::put_Action(DESTACTION newVal)
{
	m_nAction = newVal;

	return S_OK;
}

/*
STDMETHODIMP CProcess::get_TruncationOccurred(VARIANT_BOOL *pRetVal)
{
	*pRetVal = m_bTruncationWarning;

	return S_OK;
}
*/

STDMETHODIMP CProcess::SQLToDelim(IdeSQL *pSQL, IdeDelim *pDelim, VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	BSTR bTemp;
	*pRetVal = bSuccess;

	if (pDelim == NULL || pSQL == NULL )  {
		m_listErrors.push_back(L"ERROR: CProcess::SQLToDelim() --  Either deFixed object or deSQL object is NULL.  Processing can not continue.");
			return S_OK;
	}

	IFormat* pDelimFormat = NULL;
	pDelim->get_Format(&pDelimFormat);
	if (pDelimFormat == NULL) {
		m_listErrors.push_back(L"ERROR: CProcess::SQLToDelim() -- Format object in deDelim object is NULL.  Processing can not continue.");
		return S_OK;
	}

	IFormat* pSQLFormat = NULL;
	pSQL->get_Format(&pSQLFormat);
	if (pSQLFormat == NULL) {
		m_listErrors.push_back(L"ERROR: CProcess::SQLToDelim() -- Format object in deDelim object is NULL.  Processing can not continue.");
		return S_OK;
	}

	bool bLoadSQL = false;
	bool bLoadDelim = false;
	long lDelimFldCnt = 0;
	long lSQLFldCnt = 0;
	pDelimFormat->get_FieldCount(&lDelimFldCnt);
	pSQLFormat->get_FieldCount(&lSQLFldCnt);

	if (lDelimFldCnt == 0 && lSQLFldCnt == 0) 
	{
		pSQL->LoadFormat(&bSuccess);   
		if (!bSuccess)
			m_listErrors.push_back(L"ERROR: CProcess::SQLToDelim() -- Failure loading field collection via LoadFormat.");  //for now....
		pSQLFormat->get_FieldCount(&lSQLFldCnt);
		if (lSQLFldCnt == 0)
			m_listErrors.push_back(L"ERROR: CProcess::SQLToDelim() -- Field collection is empty.Processing can not continue.");
		else
			bLoadDelim = true;
	}
	else 
	{   //if one of these has zero fields then copy the other to it, which we know is populated
		if (lSQLFldCnt == 0) 
			bLoadSQL = true;
		else if (lDelimFldCnt == 0) 
			bLoadDelim = true;
		else 
			bSuccess = VARIANT_TRUE;
	}

	if (bLoadSQL) {
		pSQLFormat->CopyFormat(pDelimFormat, &bSuccess);
		if (!bSuccess) {
			m_listErrors.push_back(L"ERROR: CProcess::SQLToDelim() -- Attempt to copy deDelim format into deSQL object failed.");
			TransferErrors(&pSQLFormat);
		}
	}
	
	if (bLoadDelim) {
		pDelimFormat->CopyFormat(pSQLFormat, &bSuccess);
		if (!bSuccess) {
			m_listErrors.push_back(L"ERROR: CProcess::SQLToDelim() -- Attempt to copy deDelim format into deSQL object failed.");
			TransferErrors(&pDelimFormat);
		}
	}

	if (bSuccess) {
		pSQLFormat->ValidFieldCollection(&bSuccess);
		if (!bSuccess)
			TransferErrors(&pSQLFormat);
	}

	if (bSuccess) 
	{
		//DTS transfer 
		CDTSDelim objDTS(false);
		objDTS.m_nAction = (ACTION)m_nAction;
	
		pSQL->get_Database(&bTemp);
		objDTS.m_bstrSQLDatabase = bTemp;
		SysFreeString(bTemp);

		pSQL->get_Server(&bTemp);
		objDTS.m_bstrSQLServer = bTemp;
		SysFreeString(bTemp);

		pSQL->get_Table(&bTemp);
		objDTS.m_bstrSQLTable = bTemp;
		SysFreeString(bTemp);

		pSQL->get_Owner(&bTemp);
		objDTS.m_bstrSQLOwner = bTemp;
		SysFreeString(bTemp);

		pDelim->get_Directory(&bTemp);
		objDTS.m_bDir= bTemp;
		SysFreeString(bTemp);

		pDelim->get_FileName(&bTemp);
		objDTS.m_bFile = bTemp; 
		SysFreeString(bTemp);
	
		pDelim->get_Delimiter(&bTemp);
		objDTS.m_bstrDelim = bTemp; 
		SysFreeString(bTemp);

		pDelim->get_RowDelimiter(&bTemp);
		objDTS.m_bstrRowDelim = bTemp; 
		SysFreeString(bTemp);

		pDelim->get_TextQualifier(&bTemp);
		objDTS.m_bstrTextQual = bTemp; 
		SysFreeString(bTemp);

		long lTemp;
		pDelim->get_FirstRow(&lTemp);
		objDTS.m_lFirst = lTemp;
		pDelim->get_LastRow(&lTemp);
		objDTS.m_lLast = lTemp;
	
		VARIANT_BOOL boolTemp = VARIANT_FALSE;
		pDelim->get_Header(&boolTemp);
		objDTS.m_bHeaderLine = (boolTemp == VARIANT_TRUE ? true : false);

		HRESULT hr = objDTS.Init(((m_bTraceLogOn == VARIANT_TRUE) ? true : false));
		if (!objDTS.ErrorsOccurred() )	{
			SQLFIELDLIST fldList;
			objDTS.m_ptrSrceFldList = &fldList; //for now just set this as empty....
			objDTS.m_ptrTrgtFldList = &fldList;
			hr = objDTS.Transfer();
			m_lRowsLoaded = objDTS.m_lRowsLoaded;
		}
		if( objDTS.ErrorsOccurred() )  {
			Utilities::TransferErrors(objDTS.m_listErrors, m_listErrors);
			bSuccess = VARIANT_FALSE;
		}
		else
			bSuccess = VARIANT_TRUE;
		objDTS.Cleanup();
	}
	if (pDelimFormat) {
		pDelimFormat->Release();
		pDelimFormat = NULL;
	}
	if (pSQLFormat) {
		pSQLFormat->Release();
		pSQLFormat = NULL;
	}
	*pRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CProcess::DelimToSQL(IdeDelim *pDelim, IdeSQL *pSQL, VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	BSTR bTemp;
	m_iLoadCnt--;
	*pRetVal = bSuccess;

#ifdef FILE_DEBUG
		TCHAR BufDir[MAX_FILE_LEN];
        GetCurrentDirectory(MAX_FILE_LEN, BufDir);
		_tcscat(BufDir, _T("\\debug_delim.txt"));
		FILE* ptrLog;
		ptrLog = fopen(BufDir, "a");
		if (ptrLog != NULL){
			fprintf(ptrLog, "\r\n\r\n\t\t----------------------- CProcess::DelimToSQL \r\n");
			fflush(ptrLog);
		}
#endif

	if (pDelim == NULL || pSQL == NULL )  {
		m_listErrors.push_back(L"ERROR: CProcess::DelimToSQL() --  Either deFixed object or deSQL object is NULL.  Processing can not continue.");
			return S_OK;
	}
#ifdef FILE_DEBUG
	else {
			if (ptrLog != NULL){
				fprintf(ptrLog, "\t\tpSQL && pDelim are not NULL.\r\n");
				fprintf(ptrLog, "\t\tget pDelimFormat object\r\n");
				fflush(ptrLog);
			}
	}
#endif

	IFormat* pDelimFormat = NULL;
	pDelim->get_Format(&pDelimFormat);
	if (pDelimFormat == NULL) {
		m_listErrors.push_back(L"ERROR: CProcess::DelimToSQL() -- Format object in deDelim object is NULL.  Processing can not continue.");
		return S_OK;
	}

#ifdef FILE_DEBUG
	if (ptrLog != NULL){
		fprintf(ptrLog, "\t\tget pSQLFormat object\r\n");
		fflush(ptrLog);
	}
#endif
	IFormat* pSQLFormat = NULL;
	pSQL->get_Format(&pSQLFormat);
	if (pSQLFormat == NULL) {
		m_listErrors.push_back(L"ERROR: CProcess::DelimToSQL() -- Format object in deDelim object is NULL.  Processing can not continue.");
		return S_OK;
	}

	bool bLoadSQL = false;
	bool bLoadDelim = false;
	long lDelimFldCnt = 0;
	long lSQLFldCnt = 0;
	pDelimFormat->get_FieldCount(&lDelimFldCnt);
	pSQLFormat->get_FieldCount(&lSQLFldCnt);
	if (lSQLFldCnt > 0 && lDelimFldCnt > 0) bSuccess = VARIANT_TRUE;
	
	VARIANT_BOOL bHeader = VARIANT_FALSE;
	pDelim->get_Header(&bHeader);
	if (lDelimFldCnt == 0 && bHeader == VARIANT_TRUE) {  //load collection from header information
#ifdef FILE_DEBUG
	if (ptrLog != NULL){
		fprintf(ptrLog, "\t\tpDelim->LoadFormat\r\n");
		fflush(ptrLog);
	}
#endif
		pDelim->LoadFormat(&bSuccess);   
		if (!bSuccess)
			m_listErrors.push_back(L"ERROR: CProcess::DelimToSQL() -- Failure loading field collection via pDelim->LoadFormat.");  //for now....
		pDelimFormat->get_FieldCount(&lDelimFldCnt);
	}

	if (lSQLFldCnt == 0  ) {
		if ( m_nAction == vAPPEND || m_nAction == vOVERWRITE ) {
#ifdef FILE_DEBUG
	if (ptrLog != NULL){
		fprintf(ptrLog, "\t\tpSQL->LoadFormat\r\n");
		fflush(ptrLog);
	}
#endif
			pSQL->LoadFormat(&bSuccess);   
			if (!bSuccess)
				m_listErrors.push_back(L"ERROR: CProcess::DelimToSQL() -- Failure loading field collection via pSQL->LoadFormat.");  //for now....
			pSQLFormat->get_FieldCount(&lSQLFldCnt);
			if (lSQLFldCnt == 0)
				m_listErrors.push_back(L"ERROR: CProcess::DelimToSQL() -- SQL Field collection is empty.Processing can not continue.");
		}
		else {
			if (lDelimFldCnt != 0 ) {
#ifdef FILE_DEBUG
	if (ptrLog != NULL){
		fprintf(ptrLog, "\t\tpSQL->CopyFormat\r\n");
		fflush(ptrLog);
	}
#endif
				pSQLFormat->CopyFormat(pDelimFormat, &bSuccess);
				if (!bSuccess) {
					m_listErrors.push_back(L"ERROR: CProcess::DelimToSQL() -- Attempt to copy deDelim format into deSQL object failed.");
					TransferErrors(&pSQLFormat);
				}
			}
		}
	}

	if (lDelimFldCnt == 0 && bHeader == VARIANT_FALSE && lSQLFldCnt != 0 ) {  //lastly...
#ifdef FILE_DEBUG
	if (ptrLog != NULL){
		fprintf(ptrLog, "\t\tpDelimFormat->CopyFormat\r\n");
		fflush(ptrLog);
	}
#endif
		pDelimFormat->CopyFormat(pSQLFormat, &bSuccess);
		if (!bSuccess) {
			m_listErrors.push_back(L"ERROR: CProcess::DelimToSQL() -- Attempt to copy deDelim format into deSQL object failed.");
			TransferErrors(&pDelimFormat);
		}
	} 
	if (bSuccess) {
#ifdef FILE_DEBUG
		if (ptrLog != NULL){
			fprintf(ptrLog, "\t\tpDelimFormat->ValidFieldCollection\r\n");
			fflush(ptrLog);
		}
#endif
		pDelimFormat->ValidFieldCollection(&bSuccess);
		if (!bSuccess)
			TransferErrors(&pDelimFormat);
#ifdef FILE_DEBUG
		if (ptrLog != NULL){
			fprintf(ptrLog, "\t\tpSQLFormat->ValidFieldCollection\r\n");
			fflush(ptrLog);
		}
#endif
		pSQLFormat->ValidFieldCollection(&bSuccess);
		if (!bSuccess)
			TransferErrors(&pSQLFormat);
	}
	
/*
	if (bSuccess) 
	{
		//DTS transfer 
#ifdef FILE_DEBUG
		if (ptrLog != NULL){
			fprintf(ptrLog, "\t\tbegin setting up for transfer\r\n");
			fflush(ptrLog);
		}
#endif
		CDTSDelim objDTS(true);
		objDTS.m_nAction = (ACTION)m_nAction;
	
		pSQL->get_Database(&bTemp);
		objDTS.m_bstrSQLDatabase = bTemp;
		SysFreeString(bTemp);

		pSQL->get_Server(&bTemp);
		objDTS.m_bstrSQLServer = bTemp;
		SysFreeString(bTemp);

		pSQL->get_Table(&bTemp);
		objDTS.m_bstrSQLTable = bTemp;
		SysFreeString(bTemp);

		pSQL->get_Owner(&bTemp);
		objDTS.m_bstrSQLOwner = bTemp;
		SysFreeString(bTemp);

		pSQL->get_ID_Name(&bTemp);
		bool bAddIdentity = false;
		if (wcslen(bTemp) != 0) bAddIdentity = true;
		SysFreeString(bTemp);

		long lBatchSize = SQLBATCHSIZE;
		pSQL->get_ImportBatchSize(&lBatchSize);
		objDTS.m_lBatchSize = lBatchSize;

		pDelim->get_Directory(&bTemp);
		objDTS.m_bDir= bTemp;
		SysFreeString(bTemp);

		pDelim->get_FileName(&bTemp);
		objDTS.m_bFile = bTemp; 
		SysFreeString(bTemp);
		
		pDelim->get_FileName(&bTemp);
		objDTS.m_bFile = bTemp; 
		SysFreeString(bTemp);

		pDelim->get_Delimiter(&bTemp);
		objDTS.m_bstrDelim = bTemp; 
		SysFreeString(bTemp);

		pDelim->get_RowDelimiter(&bTemp);
		objDTS.m_bstrRowDelim = bTemp; 
		SysFreeString(bTemp);

		pDelim->get_TextQualifier(&bTemp);
		objDTS.m_bstrTextQual = bTemp; 
		SysFreeString(bTemp);

		long lTemp;
		pDelim->get_FirstRow(&lTemp);
		objDTS.m_lFirst = lTemp;
		pDelim->get_LastRow(&lTemp);
		objDTS.m_lLast = lTemp;
	
		VARIANT_BOOL boolTemp = VARIANT_FALSE;
		pDelim->get_Header(&boolTemp);
		objDTS.m_bHeaderLine = (boolTemp == VARIANT_TRUE ? true : false);

		objDTS.m_bstrTraceFileName = m_bstrLogFileName; //for multiloading
		HRESULT hr = objDTS.Init(((m_bTraceLogOn == VARIANT_TRUE) ? true : false));
		if (!objDTS.ErrorsOccurred() )	{
			m_bstrLogFileName = objDTS.m_bstrTraceFileName;  //set for multiloading
			pSQL->GetCreateTableSQL(&bTemp);
			_bstr_t bstrSQL = bTemp;
			SysFreeString(bTemp);
#ifdef FILE_DEBUG
			if (ptrLog != NULL){
				fprintf(ptrLog, "\t\tobjDTS.ConfirmSQLTable\r\n");
				fflush(ptrLog);
			}
#endif
			hr = objDTS.ConfirmSQLTable((ACTION)m_nAction, bstrSQL);
#ifdef FILE_DEBUG
			if (ptrLog != NULL){
				fprintf(ptrLog, "\t\tset up fldListSrce & fldListTrgt\r\n");
				fflush(ptrLog);
			}
#endif	
			if (SUCCEEDED(hr)  && !objDTS.ErrorsOccurred()) {
				SQLFIELDLIST fldListSrce;
				SQLFIELDLIST fldListTrgt;
				if ( m_bAddSource == VARIANT_TRUE || bAddIdentity) {
					BuildFieldList(&pDelimFormat, &fldListSrce, !bHeader);  //lets only do this if necessary, a populated fldList will trigger DTS mapping and there is some risk...
					BuildFieldList(&pSQLFormat, &fldListTrgt, false);
				}
				objDTS.m_ptrSrceFldList = &fldListSrce;
				objDTS.m_ptrTrgtFldList = &fldListTrgt;
#ifdef FILE_DEBUG
			if (ptrLog != NULL){
				fprintf(ptrLog, "\t\tbegin transfer\r\n");
				fflush(ptrLog);
			}
#endif	
				hr = objDTS.Transfer();
#ifdef FILE_DEBUG
			if (ptrLog != NULL){
				fprintf(ptrLog, "\t\tend transfer\r\n");
				fflush(ptrLog);
			}
#endif	
			}
			m_lRowsLoaded = objDTS.m_lRowsLoaded;
		}

		if( objDTS.ErrorsOccurred() )  {
			Utilities::TransferErrors(objDTS.m_listErrors, m_listErrors);
			bSuccess = VARIANT_FALSE;
		}
		else
			bSuccess = VARIANT_TRUE;

		objDTS.Cleanup();

	} */

	if (pDelimFormat) {
		pDelimFormat->Release();
		pDelimFormat = NULL;
	}

	if (pSQLFormat) {
		pSQLFormat->Release();
		pSQLFormat = NULL;
	}
#ifdef FILE_DEBUG
		if (ptrLog != NULL){
			fprintf(ptrLog, "\t\t----------------------- Exit CProcess::DelimToSQL  \r\n\r\n");
			fflush(ptrLog);
			fclose(ptrLog);
		}
#endif
	*pRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CProcess::XBaseToSQL(IdeXBase *pXBase, IdeSQL *pSQL, VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_iLoadCnt--;
	BSTR bTemp;
	bool bMultiLoadAbort = false;
	*pRetVal = bSuccess;
	TCHAR szError[MAX_ERR_LEN];
	memset(szError, 0, MAX_ERR_LEN);

	if (pXBase == NULL || pSQL == NULL )  {
		m_listErrors.push_back(L"ERROR: CProcess::XBaseToSQL() -- Either deFixed object or deSQL object is NULL.  Processing can not continue.");
			return S_OK;
	}

	IFormat* pXBaseFormat = NULL;
	pXBase->get_Format(&pXBaseFormat);
	if (pXBaseFormat == NULL) {
		m_listErrors.push_back(L"ERROR: CProcess::XBaseToSQL() -- Format object in deFixed object is NULL.  Processing can not continue.");
		return S_OK;
	}

	IFormat* pSQLFormat = NULL;
	pSQL->get_Format(&pSQLFormat);
	if (pSQLFormat == NULL) {
		m_listErrors.push_back(L"ERROR: CProcess::XBaseToSQL() -- Format object in deSQL object is NULL.  Processing can not continue.");
		return S_OK;
	}

	bool bLoadSQL = false;
	bool bLoadXBase = false;
	long lXBaseFldCnt = 0;
	long lSQLFldCnt = 0;
	pXBaseFormat->get_FieldCount(&lXBaseFldCnt);
	pSQLFormat->get_FieldCount(&lSQLFldCnt);

	if (lXBaseFldCnt == 0 ) {
		pXBase->LoadFormat(&bSuccess);
		if (!bSuccess) {
			m_listErrors.push_back(L"ERROR: CProcess::XBaseToSQL() -- Failure loading field collection via pXBase->LoadFormat."); 
			//TransferErrors(&pXBase);
		}
	}
		
	pXBaseFormat->get_FieldCount(&lXBaseFldCnt);
	if (lSQLFldCnt == 0  )  {
		if (m_nAction == vAPPEND || m_nAction == vOVERWRITE) {
			pSQL->LoadFormat(&bSuccess);
			if (!bSuccess) {
				m_listErrors.push_back(L"ERROR: CProcess::XBaseToSQL() -- Failure loading field collection via pSQL->LoadFormat.");  
				//TransferErrors(&pSQL);
			}
		}
		else {
			if (lXBaseFldCnt != 0) {
				pSQLFormat->CopyFormat(pXBaseFormat, &bSuccess);
				if (!bSuccess) {
					m_listErrors.push_back(L"ERROR: CProcess::XBaseToSQL() -- Attempt to copy XBase format into deSQL object failed.");
					TransferErrors(&pSQLFormat);
				}
			}
		}
	}
	
	if (bSuccess)  {
		pXBaseFormat->ValidFieldCollection(&bSuccess);
		if (!bSuccess)
			TransferErrors(&pXBaseFormat);
		pSQLFormat->ValidFieldCollection(&bSuccess);
		if (!bSuccess)
			TransferErrors(&pSQLFormat);
	}

	if (bSuccess) {
		CDTSDBF objDTS(true);
		objDTS.m_nAction = (ACTION)m_nAction;
	
		pSQL->get_Database(&bTemp);
		objDTS.m_bstrSQLDatabase = bTemp;
		SysFreeString(bTemp);

		pSQL->get_Server(&bTemp);
		objDTS.m_bstrSQLServer = bTemp;
		SysFreeString(bTemp);

		pSQL->get_Table(&bTemp);
		objDTS.m_bstrSQLTable = bTemp;
		SysFreeString(bTemp);

		pSQL->get_Owner(&bTemp);
		objDTS.m_bstrSQLOwner = bTemp;
		SysFreeString(bTemp);

		pSQL->get_ID_Name(&bTemp);
		bool bAddIdentity = false;
		if (wcslen(bTemp) != 0) bAddIdentity = true;
		SysFreeString(bTemp);

		long lBatchSize = SQLBATCHSIZE;
		pSQL->get_ImportBatchSize(&lBatchSize);
		objDTS.m_lBatchSize = lBatchSize;

		pXBase->get_Directory(&bTemp);
		objDTS.m_bDir= bTemp;
		SysFreeString(bTemp);

		pXBase->get_Table(&bTemp);
		objDTS.m_bFile = bTemp; 
		SysFreeString(bTemp);

		XBASETYPE nXBaseType = vDBASE;
		pXBase->get_TableType(&nXBaseType);
		objDTS.m_nXBaseType = (XBASE) nXBaseType;

		long lTemp;
		pXBase->get_FirstRow(&lTemp);
		objDTS.m_lFirst = lTemp;
		pXBase->get_LastRow(&lTemp);
		objDTS.m_lLast = lTemp;

		//multiloading will reuse these....instead of creating new on each one...check to see if it is there...
		//objDTS.m_bDSN = m_bstrDSN;
		//objDTS.m_bstrTraceFileName = m_bstrLogFileName; 

		HRESULT hr = objDTS.Init(((m_bTraceLogOn == VARIANT_TRUE) ? true : false));
		if (!objDTS.ErrorsOccurred() )	{
			//multiloading reuse ... not implemented
			//m_bstrDSN = objDTS.m_bDSN;
			//m_bstrLogFileName = objDTS.m_bstrTraceFileName;

			if ( !m_bTableOK ) {

				pSQL->GetCreateTableSQL(&bTemp);
				_bstr_t bstrSQL = bTemp;
				SysFreeString(bTemp);
				hr = objDTS.ConfirmSQLTable((ACTION)m_nAction, bstrSQL);
			}
	
			if (SUCCEEDED(hr)  && !objDTS.ErrorsOccurred()) {
				m_bTableOK = true;
				SQLFIELDLIST fldListSrce;
				SQLFIELDLIST fldListTrgt;
				if (bAddIdentity) {
					BuildFieldList(&pXBaseFormat, &fldListSrce, false);  //lets only do this if necessary, a populated fldList will trigger DTS mapping and there is some risk...
					BuildFieldList(&pSQLFormat, &fldListTrgt, false); 
				}
				objDTS.m_ptrSrceFldList = &fldListSrce; 
				objDTS.m_ptrTrgtFldList = &fldListTrgt; 
				hr = objDTS.Transfer();
			}
			m_lRowsLoaded = objDTS.m_lRowsLoaded;
		}

/*      //again multi loading....
		if ( m_iLoadCnt == 0 || !SUCCEEDED(hr) || objDTS.ErrorsOccurred() ) {	//last file to load or cleanup if multiload is going to abort
			objDTS.Cleanup(); 
			m_bstrDSN = L"";
		} */

		objDTS.Cleanup(); 

		if( objDTS.ErrorsOccurred() )  {
			Utilities::TransferErrors(objDTS.m_listErrors, m_listErrors);
			bSuccess = VARIANT_FALSE;
		}
		else
			bSuccess = VARIANT_TRUE;
	}

	if (pSQLFormat) {
		pSQLFormat->Release();
		pSQLFormat = NULL;
	}

	if (pXBaseFormat) {
		pXBaseFormat->Release();
		pXBaseFormat = NULL;
	}

	*pRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CProcess::SQLToXBase(IdeSQL *pSQL, IdeXBase *pXBase, VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	BSTR bTemp;
	//m_iLoadCnt--;
	bool bMultiLoadAbort = false;
	*pRetVal = bSuccess;
	TCHAR szError[MAX_ERR_LEN];
	memset(szError, 0, MAX_ERR_LEN);

	if (pXBase == NULL || pSQL == NULL )  {
		m_listErrors.push_back(L"ERROR: CProcess::SQLToXBase() -- Either deFixed object or deSQL object is NULL.  Processing can not continue.");
			return S_OK;
	}

	IFormat* pXBaseFormat = NULL;
	pXBase->get_Format(&pXBaseFormat);
	if (pXBaseFormat == NULL) {
		m_listErrors.push_back(L"ERROR: CProcess::SQLToXBase() -- Format object in deFixed object is NULL.  Processing can not continue.");
		return S_OK;
	}

	IFormat* pSQLFormat = NULL;
	pSQL->get_Format(&pSQLFormat);
	if (pSQLFormat == NULL) {
		m_listErrors.push_back(L"ERROR: CProcess::SQLToXBase() -- Format object in deSQL object is NULL.  Processing can not continue.");
		return S_OK;
	}

	bool bLoadSQL = false;
	bool bLoadXBase = false;
	long lXBaseFldCnt = 0;
	long lSQLFldCnt = 0;
	pXBaseFormat->get_FieldCount(&lXBaseFldCnt);
	pSQLFormat->get_FieldCount(&lSQLFldCnt);

	if (lXBaseFldCnt == 0 && lSQLFldCnt == 0) 
	{
		pSQL->LoadFormat(&bSuccess);   
		if (!bSuccess)
			m_listErrors.push_back(L"ERROR: CProcess::SQLToXBase() -- Failure loading field collection via LoadFormat.");  //for now....
		pSQLFormat->get_FieldCount(&lSQLFldCnt);
		if (lSQLFldCnt == 0)
			m_listErrors.push_back(L"ERROR: CProcess::SQLToXBase() -- Field collection is empty.Processing can not continue.");
		else
			bLoadXBase = true;
	}
	else 
	{   //if one of these has zero fields then copy the other to it, which we know is populated -- 
		//note: SQL to XBase transfers are not as as particular as XBase to SQL due to the DTS mapping, which the names must match
		if (lSQLFldCnt == 0) 
			bLoadSQL = true;
		else if (lXBaseFldCnt == 0) 
			bLoadXBase = true;
		else
			bSuccess = VARIANT_TRUE;
	}

	if (bLoadSQL) {
		pSQLFormat->CopyFormat(pXBaseFormat, &bSuccess);
		if (!bSuccess) {
			m_listErrors.push_back(L"ERROR: CProcess::SQLToXBase() -- Attempt to copy deXBase format into deSQL object failed.");
			TransferErrors(&pSQLFormat);
		}
	}
	
	if (bLoadXBase) {
		pXBaseFormat->CopyFormat(pSQLFormat, &bSuccess);
		if (!bSuccess) {
			m_listErrors.push_back(L"ERROR: CProcess::SQLToXBase() -- Attempt to copy deXBase format into deSQL object failed.");
			TransferErrors(&pXBaseFormat);
		}
	}

	if (bSuccess) 
	{
		pXBaseFormat->ValidFieldCollection(&bSuccess);
		if (!bSuccess)
			TransferErrors(&pXBaseFormat);
	}

	XBASETYPE nXBaseType = vDBASE;
	pXBase->get_TableType(&nXBaseType); 
	if (nXBaseType != vDBASE ) {
		bSuccess = VARIANT_FALSE;
		m_listErrors.push_back(L"ERROR: CProcess::SQLToXBase() -- SQL exports to FoxPro tables are not supported.  Processing can not continue.");
	}

	if (bSuccess) {
		CDTSDBF objDTS(false);
		objDTS.m_nAction = (ACTION)m_nAction;
		objDTS.m_nXBaseType = (XBASE) nXBaseType;

		pSQL->get_Database(&bTemp);
		objDTS.m_bstrSQLDatabase = bTemp;
		SysFreeString(bTemp);

		pSQL->get_Server(&bTemp);
		objDTS.m_bstrSQLServer = bTemp;
		SysFreeString(bTemp);

		pSQL->get_Table(&bTemp);
		objDTS.m_bstrSQLTable = bTemp;
		SysFreeString(bTemp);

		pSQL->get_Owner(&bTemp);
		objDTS.m_bstrSQLOwner = bTemp;
		SysFreeString(bTemp);

		pXBase->get_Directory(&bTemp);
		objDTS.m_bDir= bTemp;
		SysFreeString(bTemp);

		pXBase->get_Table(&bTemp);
		objDTS.m_bFile = bTemp; 
		SysFreeString(bTemp);

		long lTemp;
		pXBase->get_FirstRow(&lTemp);
		objDTS.m_lFirst = lTemp;
		pXBase->get_LastRow(&lTemp);
		objDTS.m_lLast = lTemp;

		//multiloading will reuse these....instead of creating new on each one...check to see if it is there...
		//objDTS.m_bDSN = m_bstrDSN;
		//objDTS.m_bstrTraceFileName = m_bstrLogFileName; 

		HRESULT hr = objDTS.Init(((m_bTraceLogOn == VARIANT_TRUE) ? true : false));
		if (!objDTS.ErrorsOccurred() )	{
	
			m_bstrDSN = objDTS.m_bDSN;
			m_bstrLogFileName = objDTS.m_bstrTraceFileName;

			if ( !m_bTableOK ) {

				//Field FldsArray[NUMDBFFIELDS];
				IFieldInfo* pField = NULL;
				long lFldCntTemp = 0;
				pXBaseFormat->get_FieldCount(&lFldCntTemp);
				DBFFIELD* FldsArray = new DBFFIELD[lFldCntTemp];
				long lFldCnt = 0;

				pXBaseFormat->First(&pField);
				while (pField != NULL) {
					pField->get_Name(&bTemp);
					memset(FldsArray[lFldCnt].Name, 0, DBFFIELDLEN);
					wcstombs(FldsArray[lFldCnt].Name, bTemp, DBFFIELDLEN-1);
					SysFreeString(bTemp);
                    FldsArray[lFldCnt].Name[strlen(FldsArray[lFldCnt].Name)] = '\0';

					TCHAR szCh = _T('C');
					FIELDTYPE nType;
					long lPrec = 18;  //the dbf create code adds two to precision, so make two less here
					long lScale = 8;
					int iVarCharLen = 0;
					_bstr_t bstrLen = L"";
					pField->get_Type(&nType);
                    switch (nType) {
						case vBIGINT:		//DBTYPE_I8
						case vINT:			//DBTYPE_I4
						case vTINYINT:		//DBTYPE_I2
							if (nXBaseType == vDBASE) {  //note: DTS export of SQL->DBASE && microsoft dbase V driver requires integer types to be defined as numeric in order to work
								FldsArray[lFldCnt].Type = _T('N');
								FldsArray[lFldCnt].Precision = lPrec;
								FldsArray[lFldCnt].Scale = 5;
							}
							else
								FldsArray[lFldCnt].Type = _T('I');
							break;
						case vBIT:			//DBTYPE_BOOL:
							FldsArray[lFldCnt].Type = _T('L');
							break;
						case vCHAR:			//DBTYPE_STR:
						case vVARCHAR:		
							pField->get_Length(&bTemp);
							bstrLen = bTemp;
							SysFreeString(bTemp);
							iVarCharLen = _wtoi((WCHAR*)bstrLen);
							if (iVarCharLen > 254 ) iVarCharLen = 254;
							FldsArray[lFldCnt].Length = iVarCharLen;
							FldsArray[lFldCnt].Type = _T('C');
							break;
						case vDATETIME:		//DBTYPE_DBTIMESTAMP:
							if (nXBaseType == vDBASE)
								FldsArray[lFldCnt].Type = _T('D');  //dbase doesn't support 'T'
							else
								FldsArray[lFldCnt].Type = _T('T');
							break;
						case vFLOAT:		//DBTYPE_R8
							FldsArray[lFldCnt].Type = _T('F');
							FldsArray[lFldCnt].Precision = lPrec;
							FldsArray[lFldCnt].Scale = lScale;
							break;
						case vMONEY:		//DBTYPE_CY:
							if (nXBaseType == vDBASE) {  //note: DTS export of SQL->DBASE && microsoft dbase V driver requires curruency to be defined as numeric in order to work
								FldsArray[lFldCnt].Type = _T('N');
								FldsArray[lFldCnt].Precision = lPrec;
								FldsArray[lFldCnt].Scale = 10;
							}
							else
								FldsArray[lFldCnt].Type = _T('Y');
							break;
						case vNUMERIC:		//DBTYPE_NUMERIC
						case vDECIMAL:		//DBTYPE_DECIMAL
							pField->get_Precision(&lPrec);
							pField->get_Scale(&lScale);
							if (lPrec > 20) lPrec = 18;  
							if (lScale > 18) lScale = 16;
							FldsArray[lFldCnt].Precision = (int) lPrec;
							FldsArray[lFldCnt].Scale = (int) lScale;
							FldsArray[lFldCnt].Type = _T('N');
							break;
						default:
							break;
					}
					pField->Release();
					pXBaseFormat->Next(&pField);
					lFldCnt++;
				}

				if (lFldCnt != 0 ) 
					hr = objDTS.ConfirmDBFTable((ACTION)m_nAction, FldsArray);
				else
					m_listErrors.push_back(L"ERROR: CProcess::SQLToXBase() No fields in deXBase Object."); //shouldn't get here but just in case...
			
				if (FldsArray) delete [] FldsArray;
			}
	
			if (SUCCEEDED(hr)  && !objDTS.ErrorsOccurred()) {
				m_bTableOK = true;
				SQLFIELDLIST fldList;
				objDTS.m_ptrSrceFldList = &fldList; //for now just set these to empty...
				objDTS.m_ptrTrgtFldList = &fldList;
				hr = objDTS.Transfer();
			}
			m_lRowsLoaded = objDTS.m_lRowsLoaded;
		}
		objDTS.Cleanup();
	//	if ( m_iLoadCnt == 0 || !SUCCEEDED(hr) || objOle.ErrorsOccurred() ) {	//last file to load or cleanup if multiload is going to abort
	//		objDTS.Cleanup(); 
	//		m_bstrDSN = L"";
	//	}

		if( objDTS.ErrorsOccurred() )  {
			Utilities::TransferErrors(objDTS.m_listErrors, m_listErrors);
			bSuccess = VARIANT_FALSE;
		}
		else
			bSuccess = VARIANT_TRUE;
	}

	if (pSQLFormat) {
		pSQLFormat->Release();
		pSQLFormat = NULL;
	}

	if (pXBaseFormat) {
		pXBaseFormat->Release();
		pXBaseFormat = NULL;
	}

	*pRetVal = bSuccess;
	return S_OK;
}

int CProcess::BuildFieldList(IFormat** pFormat, SQLFIELDLIST* ptrFldsList, bool bGenericColNames)
{
	if ((*pFormat) == NULL || ptrFldsList == NULL) return 0;
	int iColCnt = 0;
	if (bGenericColNames) {
		//on a delimited file if no header exists w/ field names in the flat file and 
		//DTS mapping is done the DTS requires generic column names (e.g. Col001, Col002, Col003...)
		long lFieldCnt = 0;
		(*pFormat)->get_FieldCount(&lFieldCnt);
        for (iColCnt = 1; iColCnt <= (int) lFieldCnt; iColCnt++) {
			TCHAR szBuffer[10]; memset(szBuffer, 0, 10);
			if ( iColCnt <= 9 ) 
				sprintf(szBuffer, _T("Col00%d"), iColCnt);
			else if ( iColCnt >= 10 && iColCnt <= 99 )
					sprintf(szBuffer, _T("Col0%d"), iColCnt);
			else if ( iColCnt >= 100 )
					sprintf(szBuffer, _T("Col%d"), iColCnt);
			szBuffer[_tcslen(szBuffer)+1] = '\0';
			_bstr_t bstrName(szBuffer);
			ptrFldsList->push_back(bstrName);	
		}
	}
	else {
		IFieldInfo* pField = NULL;
		(*pFormat)->First(&pField);
		while (pField != NULL) {
			iColCnt++;
			BSTR bTemp;
			pField->get_Name(&bTemp);
			_bstr_t bstrName = bTemp;
			SysFreeString(bTemp);
			ptrFldsList->push_back(bstrName);
			pField->Release();
			(*pFormat)->Next(&pField);
		}
	}
	return iColCnt;
}


STDMETHODIMP CProcess::get_ErrMsgList(BSTR *pVal)
{
	_bstr_t bstrErrors = L"";
	try {

		bstrErrors = Utilities::GetErrorList(m_listErrors, true);
	}
		catch(_com_error &e) {
		bstrErrors = Utilities::ConstructErrMsg(L" CFormat::get_ErrMsgList() catch statement.", e.Source(), e.Description(), e.ErrorMessage());
	}
	catch (...) {
		bstrErrors = L"ERROR: CFormat::get_ErrMsgList() catch statement.";
	}

	*pVal = bstrErrors.copy();
	return S_OK;
}

int CProcess::TransferErrors(IFormat** pFormat) 
{
	int iErrCnt = 0;
	BSTR Temp;
	VARIANT_BOOL bMoreErrs = VARIANT_FALSE;
	
	(*pFormat)->GetFirstError(&Temp, &bMoreErrs);
	_bstr_t bErr = Temp;
	SysFreeString(Temp);

	if (wcscmp(bErr, L"") != 0 ) 
	{
		m_listErrors.push_back(bErr);
		while (bMoreErrs && m_listErrors.size() < ERR_LIMIT) 
		{
			(*pFormat)->GetNextError(&Temp, &bMoreErrs);
			bErr = Temp;
			SysFreeString(Temp);
			if (wcscmp(bErr, L"") != 0 ) 
				m_listErrors.push_back(bErr);
		}
	}

	return iErrCnt;
}


/*
VARIANT_BOOL CProcess::ReconcileFieldColl(IFormat** pSrceFormat, IFormat** pTrgtFormat)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;

	bool bLoadTrgt = false;
	bool bLoadSrce = false;
	long lSrceFldCnt = 0;
	long lTrgtFldCnt = 0;
	(*pSrceFormat)->get_FieldCount(&lSrceFldCnt);
	(*pTrgtFormat)->get_FieldCount(&lTrgtFldCnt);

	if (lSrceFldCnt == 0 && lTrgtFldCnt == 0) 
	{
		(*pSrceFormat)->LoadFormat(&bSuccess);   
		if (!bSuccess)
			m_listErrors.push_back(L"ERROR: CProcss::ReconcileFieldColl -- Failure loading field collection via LoadFormat.");  //for now....
		(*pSrceFormat)->get_FieldCount(&lSrceFldCnt);
		if (lSrceFldCnt == 0)
			m_listErrors.push_back(L"ERROR: CProcss::ReconcileFieldColl -- Field collection is empty.Processing can not continue.");
		else
			bLoadSrce = true;
	}
	else 
	{   //if one of these has zero fields then copy the other to it, which we know is populated
		if (lSrceFldCnt == 0) bLoadSrce = true;
		if (lTrgtFldCnt == 0) bLoadTrgt = true;
	}

	if (bLoadTrgt) {
		(*pTrgtFormat)->CopyFormat(	(*pSrceFormat), &bSuccess);
		if (!bSuccess) {
			m_listErrors.push_back(L"ERROR: CProcss::ReconcileFieldColl  -- Attempt to copy deDelim format into deSQL object failed.");
			TransferErrors(&(*pTrgtFormat));
		}
	}
	
	if (bLoadSrce) {
		(*pSrceFormat)->CopyFormat((*pTrgtFormat), &bSuccess);
		if (!bSuccess) {
			m_listErrors.push_back(L"ERROR: CProcss::ReconcileFieldColl  -- Attempt to copy deDelim format into deSQL object failed.");
			TransferErrors(&(*pSrceFormat));
		}
	}

	if (bSuccess) 
	{
		(*pSrceFormat)->ValidFieldCollection(&bSuccess);
		if (!bSuccess)
			TransferErrors(&(*pSrceFormat));
	}
	return bSuccess;
}
*/

STDMETHODIMP CProcess::GetFirstError(BSTR *pValOut, VARIANT_BOOL *pVal)
{
	VARIANT_BOOL bMore = VARIANT_FALSE;
	_bstr_t bstrErr = L"";
	m_iErrIndex = 0;

	if (m_listErrors.size() > 0 ) {
		bstrErr = Utilities::GetError(m_iErrIndex++, m_listErrors);
		if (m_iErrIndex < (int) m_listErrors.size())
			bMore = VARIANT_TRUE;
	}

	*pValOut = bstrErr.copy();
	*pVal = bMore;

	return S_OK;
}

STDMETHODIMP CProcess::GetNextError(BSTR *pValOut, VARIANT_BOOL *pVal)
{
	VARIANT_BOOL bMore = VARIANT_FALSE;
	_bstr_t bstrErr = L"";

	if (m_iErrIndex < (int) m_listErrors.size()) {
		bstrErr = Utilities::GetError(m_iErrIndex, m_listErrors);
		if (m_iErrIndex < (int) m_listErrors.size()) {
			bMore = VARIANT_TRUE;
			m_iErrIndex++;
		}
	}

	*pValOut = bstrErr.copy();
	*pVal = bMore;
	return S_OK;
}

STDMETHODIMP CProcess::get_AddSource(VARIANT_BOOL *pVal)
{
	*pVal = m_bAddSource;

	return S_OK;
}

STDMETHODIMP CProcess::put_AddSource(VARIANT_BOOL newVal)
{
	m_bAddSource = newVal;

	return S_OK;
}



