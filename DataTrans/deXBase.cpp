// deXBase.cpp : Implementation of CdeXBase
#include "stdafx.h"
#include "DTran.h"
#include "deXBase.h"

#define	DBINITCONSTANTS
#include "OLEDB25.h"


/////////////////////////////////////////////////////////////////////////////
// CdeXBase


STDMETHODIMP CdeXBase::get_Directory(BSTR *pVal)
{
	*pVal = m_bstrDirectory.copy(); 
	return S_OK;
}

STDMETHODIMP CdeXBase::put_Directory(BSTR newVal)
{
	m_bstrDirectory = newVal;
	return S_OK;
}

STDMETHODIMP CdeXBase::get_Table(BSTR *pVal)
{
	*pVal = m_bstrTable.copy();
	return S_OK;
}

STDMETHODIMP CdeXBase::put_Table(BSTR newVal)
{
	m_bstrTable = newVal;
	return S_OK;
}


STDMETHODIMP CdeXBase::get_FirstRow(long *pVal)
{
	*pVal = m_lFirstRow;
	return S_OK;
}

STDMETHODIMP CdeXBase::put_FirstRow(long newVal)
{
	m_lFirstRow = newVal;
	return S_OK;
}

STDMETHODIMP CdeXBase::get_LastRow(long *pVal)
{
	*pVal = m_lLastRow;
	return S_OK;
}

STDMETHODIMP CdeXBase::put_LastRow(long newVal)
{
	m_lLastRow = newVal;
	return S_OK;
}

STDMETHODIMP CdeXBase::get_Format(IFormat **pObj)
{
	m_pFormatObj->AddRef();
	*pObj = m_pFormatObj;

	return S_OK;
}



STDMETHODIMP CdeXBase::get_TableType(XBASETYPE *pVal)
{
	*pVal = m_nXBaseType;
	return S_OK;
}

STDMETHODIMP CdeXBase::put_TableType(XBASETYPE newVal)
{
	m_nXBaseType = newVal;
	return S_OK;
}


STDMETHODIMP CdeXBase::put_Format(IFormat *pObj)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_listErrors.clear(); 
	
	try {
		m_pFormatObj->CopyFormat(pObj, &bSuccess);
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeFixed::put_Format() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeFixed::put_Format() catch statement.");
	}

	return S_OK;
}


STDMETHODIMP CdeXBase::get_ErrMsgList(BSTR *pVal)
{
	_bstr_t bstrErrors = L"";
	try {

		bstrErrors = Utilities::GetErrorList(m_listErrors, true);
	}
		catch(_com_error &e) {
		bstrErrors = Utilities::ConstructErrMsg(L"COM ERROR: CFormat::get_ErrMsgList() catch statement.", e.Source(), e.Description(), e.ErrorMessage());
	}
	catch (...) {
		bstrErrors = L"DTRAN ERROR: CFormat::get_ErrMsgList() catch statement.";
	}

	*pVal = bstrErrors.copy();
	return S_OK;
}

STDMETHODIMP CdeXBase::get_ErrMsgs(BSTR *pVal)
{
	_bstr_t bstrErrors = L"";
	try {

		bstrErrors = Utilities::GetErrorList(m_listErrors, false);
	}
		catch(_com_error &e) {
		bstrErrors = Utilities::ConstructErrMsg(L"COM ERROR: CFormat::get_ErrMsgList() catch statement.", e.Source(), e.Description(), e.ErrorMessage());
	}
	catch (...) {
		bstrErrors = L"DTRAN ERROR: CFormat::get_ErrMsgList() catch statement.";
	}

	*pVal = bstrErrors.copy();

	return S_OK;
}

STDMETHODIMP CdeXBase::get_RowsLoaded(long *pVal)
{
	*pVal = m_lRowsLoaded;
	return S_OK;
}

STDMETHODIMP CdeXBase::put_TraceLogOn(VARIANT_BOOL newVal)
{
	m_bTraceLogOn = newVal;
	return S_OK;
}

STDMETHODIMP CdeXBase::get_TraceLogOn(VARIANT_BOOL* pVal)
{
	*pVal = m_bTraceLogOn;
	return S_OK;
}

STDMETHODIMP CdeXBase::get_Version(BSTR *pVal)
{
	*pVal = m_bstrVersion.copy();
	return S_OK;
}


STDMETHODIMP CdeXBase::Import(IDispatch *pIn, DESTACTION action, VARIANT_BOOL *bRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_listErrors.clear(); 
	m_lRowsLoaded = 0;
	IProcess* pProcess = NULL;
	try {
		CoCreateInstance(CLSID_Process, NULL, CLSCTX_INPROC_SERVER, IID_IProcess, (void**) &pProcess);
		if (pProcess != NULL)
		{
			VARIANT_BOOL bTextObj = VARIANT_FALSE;

			//Text-->SQL Table		
			HRESULT hr = pProcess->IsSQLObject(pIn, &bTextObj);
			if (bTextObj == VARIANT_TRUE)
			{
				IdeSQL* pSQL = NULL;
				pIn->QueryInterface(IID_IdeSQL, (void**) &pSQL);
				if (pSQL != NULL) {
					pProcess->put_Action(action);
					pProcess->put_TraceLogOn(m_bTraceLogOn);
					pProcess->SQLToXBase(pSQL, this, &bSuccess);
					pSQL->Release();
					pSQL = NULL;
				}
				else
					m_listErrors.push_back( L"DTRAN ERROR:  CdeXBase::Import() -- Can not get deSQL interface pointer from input IDispatch pointer.");
			
				if(!bSuccess) {
					BSTR Temp;
					VARIANT_BOOL bMoreErrs = VARIANT_FALSE;
					pProcess->GetFirstError(&Temp, &bMoreErrs);
					_bstr_t bErr = Temp;
					SysFreeString(Temp);
					if (wcscmp(bErr, L"") != 0 ) 
					{
						m_listErrors.push_back(bErr);
						while (bMoreErrs && m_listErrors.size() < ERR_LIMIT) 
						{
							pProcess->GetNextError(&Temp, &bMoreErrs);
							bErr = Temp;
							SysFreeString(Temp);
							if (wcscmp(bErr, L"") != 0 ) 
								m_listErrors.push_back(bErr);
						}
					}
				}
				pProcess->get_RowsLoaded(&m_lRowsLoaded);

			}
			else
				m_listErrors.push_back( L"DTRAN ERROR:  CdeXBase::Export -- Object passed in is not a deSQL object.");
		}
		else
			m_listErrors.push_back( L"DTRAN ERROR: CdeXBase::Export() -- Internally used process object is NULL.");
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeXBase::Export() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeXBase::Export() catch statement.");
		bSuccess = VARIANT_FALSE;
	}
	if (pProcess != NULL) 
		pProcess->Release();
	pProcess = NULL;
	*bRetVal = bSuccess;

	return S_OK;
}

STDMETHODIMP CdeXBase::Export(IDispatch *pIn, DESTACTION action, VARIANT_BOOL *bRetVal)
{
VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_listErrors.clear(); 
	m_lRowsLoaded = 0;
	IProcess* pProcess = NULL;
	try {
		CoCreateInstance(CLSID_Process, NULL, CLSCTX_INPROC_SERVER, IID_IProcess, (void**) &pProcess);
		if (pProcess != NULL)
		{
			VARIANT_BOOL bTextObj = VARIANT_FALSE;

			//Text-->SQL Table		
			HRESULT hr = pProcess->IsSQLObject(pIn, &bTextObj);
			if (bTextObj == VARIANT_TRUE)
			{
				IdeSQL* pSQL = NULL;
				pIn->QueryInterface(IID_IdeSQL, (void**) &pSQL);
				if (pSQL != NULL ) {
					pProcess->put_Action(action);
					pProcess->put_TraceLogOn(m_bTraceLogOn);
					pProcess->XBaseToSQL(this, pSQL, &bSuccess);
					pSQL->Release();
					pSQL = NULL;
				}
				else
					m_listErrors.push_back( L"DTRAN ERROR:  CdeXBase::Export() -- Can not get deSQL interface pointer from input IDispatch pointer.");
			
				if(!bSuccess) {
					BSTR Temp;
					VARIANT_BOOL bMoreErrs = VARIANT_FALSE;
					pProcess->GetFirstError(&Temp, &bMoreErrs);
					_bstr_t bErr = Temp;
					SysFreeString(Temp);
					if (wcscmp(bErr, L"") != 0 ) 
					{
						m_listErrors.push_back(bErr);
						while (bMoreErrs && m_listErrors.size() < ERR_LIMIT) 
						{
							pProcess->GetNextError(&Temp, &bMoreErrs);
							bErr = Temp;
							SysFreeString(Temp);
							if (wcscmp(bErr, L"") != 0 ) 
								m_listErrors.push_back(bErr);
						}
					}
				}
				pProcess->get_RowsLoaded(&m_lRowsLoaded);

			}
			else
				m_listErrors.push_back( L"DTRAN ERROR:  CdeXBase::Export -- Object passed in is not a deSQL object.");
		}
		else
			m_listErrors.push_back( L"DTRAN ERROR: CdeXBase::Export() -- Internally used process object is NULL.");
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeXBase::Export() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeXBase::Export() catch statement.");
		bSuccess = VARIANT_FALSE;
	}
	if (pProcess != NULL) 
		pProcess->Release();
	pProcess = NULL;
	*bRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CdeXBase::Exists(VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_listErrors.clear();

	try {

		_bstr_t bstrFileLoc = Utilities::ConstructFullPath(m_bstrDirectory, m_bstrTable);

		TCHAR szErr[MAX_ERR_LEN];
		memset(szErr, 0, MAX_ERR_LEN);

		TCHAR szFile[BUFF_MED]; memset(szFile, 0, BUFF_MED);
		int iStrLen =  SysStringLen(bstrFileLoc);
		WideCharToMultiByte(CP_ACP, 0, bstrFileLoc, iStrLen, szFile, iStrLen+1, NULL, NULL);
		szFile[iStrLen]= '\0';

		if ( !Utilities::FileCanBeRead( szFile, szErr ) ) {
			_bstr_t bstrErr(szErr); //constructor will convert...
			m_listErrors.push_back(bstrErr);
		}
		else
			bSuccess = VARIANT_TRUE;

	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeXBase::Exists() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeXBase::Exists() catch statement.");
		bSuccess = VARIANT_FALSE;
	}
	*pRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CdeXBase::LoadFormat(VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	TCHAR szError[MAX_ERR_LEN];
	memset(szError, 0, MAX_ERR_LEN);
	m_listErrors.clear();

	try {
		TCHAR szDirPath[BUFF_LARGE];	memset(szDirPath, 0, BUFF_LARGE);
		TCHAR szDBFName[BUFF_MED];		memset(szDBFName, 0, BUFF_MED);
		TCHAR szSelectStmt[BUFF_MED];	memset(szSelectStmt, 0, BUFF_MED);
		TCHAR szConnStr[BUFF_MED];		memset(szConnStr, 0, BUFF_MED);
		TCHAR szFileLoc[BUFF_MED];		memset(szFileLoc, 0, BUFF_MED);
		_bstr_t bstrDSN = L"";
		m_pFormatObj->Clear();

		_bstr_t bstrFileLoc = Utilities::ConstructFullPath(m_bstrDirectory, m_bstrTable);
		int iStrLen = SysStringLen(bstrFileLoc);
		WideCharToMultiByte(CP_ACP, 0, bstrFileLoc, iStrLen, szFileLoc, iStrLen+1, NULL, NULL);
		szFileLoc[iStrLen] = '\0';
		
		Utilities::GetDirectoryPath(szFileLoc, szDirPath);
		Utilities::ParseOutFileName(szFileLoc, szDBFName);
		
		if (_tcslen(szDirPath) != 0 && _tcslen(szDBFName) != 0) 
			bSuccess = VARIANT_TRUE;
		else {
			_stprintf(szError, _T("DTRAN ERROR: CdeXBase::LoadFormat() Problems with Directory path: %s or file name: %s."), szDirPath, szDBFName);
			m_listErrors.push_back(szError);
		}

		if (m_nXBaseType == vDBASE ) 
			_stprintf(szConnStr, szDBaseConnStr, szDirPath);
		else {
			//create DSN for FoxPro...
			 bstrDSN = Utilities::CreateDSN(szDirPath, L"", L"");
			 if (wcscmp(bstrDSN, L"") == 0){
				m_listErrors.push_back(L"DTRAN ERROR: CdeXBase::LoadFormat() FoxPro DSN was not created.");
				bSuccess = VARIANT_FALSE;
			 }

			 TCHAR szDSN[BUFF_MED]; memset(szDSN, 0, BUFF_MED);
			 int iStrLen =  SysStringLen(bstrDSN);
		     WideCharToMultiByte(CP_ACP, 0, bstrDSN, iStrLen, szDSN, iStrLen+1, NULL, NULL);
		     szDSN[iStrLen]= '\0';
			 _stprintf(szConnStr, szFoxProConnStr, szDSN);
		}

		if (bSuccess) {
	
			CODataSource		SrcDataSource; 
			COSession			SrcSession;
			CBulkRecord<CRBase>	SrcBulkRecord;	
			bSuccess = VARIANT_FALSE;
			_bstr_t bError = L"";

			if (m_nXBaseType == vDBASE ) {  //only for DBase must we do this...
				_bstr_t bstrDirPath(szDirPath);
				CODataSource::CreateDataSource(L"Provider=Microsoft.Jet.OLEDB.4.0;", bstrDirPath);
				SrcDataSource.SetServer(bstrDirPath);
			}

			_bstr_t bstrConnStr(szConnStr);
			if(!SrcDataSource.Open(bstrConnStr) ) {
				bError = SrcDataSource.m_strError;
			}
			else if(!SrcSession.Open(SrcDataSource.m_pIDBInitialize)) { 
				bError = SrcDataSource.m_strError;
			}
			else {

				_stprintf(szSelectStmt, sz_SQLSelectStmt4, szDBFName);
				_bstr_t bstrSelect(szSelectStmt);
				if (!OpenWithCmnd(SrcSession.m_pIOpenRowset, SrcBulkRecord, bstrSelect, Static, true, true)) {
//				if (!OpenWithCmnd(SrcSession.m_pIOpenRowset, SrcBulkRecord, (_bstr_t) szSelectStmt, Static, true, true)) {
					bError = SrcBulkRecord.m_strError;
				}
				else {
					int nNumOfCols=SrcBulkRecord.GetCols();
					int iAddCnt = 0;
					int nCol = 0;

					for(nCol=1; nCol <= nNumOfCols; nCol++)   
					{
						//if the field is null
						if(SrcBulkRecord.GetStatus(nCol)==DBSTATUS_S_ISNULL)
							;
						else	//if the field is not null
						{
							//one conversion routine for now...may need two when dbase implemented...
							LPCOLESTR oleSTR = SrcBulkRecord.GetColName(nCol);
							_bstr_t bstrName(oleSTR);
							ULONG nLen = SrcBulkRecord.GetMaxLength(nCol);
							nLen--;  //for some reason this is coming through 1 greater....
							_bstr_t bstrLen = L"255";
							_variant_t vTemp = nLen;
							if ( nLen > 8000 )
								bstrLen = L"8000";		//these are memo fields & we need to limit them
							else
								_itow((int)nLen, bstrLen, 10);

							FIELDTYPE nFieldType = vCHAR; 
							
							bool bRet = false;
							BYTE BYTEprec = 0;
							BYTE BYTEscale = 0;
							DBTYPEENUM nType = (DBTYPEENUM) SrcBulkRecord.GetDataType(nCol);
							switch(SrcBulkRecord.GetDataType(nCol))
							{
								case DBTYPE_I2:
									nFieldType = vSMALLINT;
									nLen = (ULONG)TINYINT_DISPLAYLEN;
									break;
								case DBTYPE_I4:
									nFieldType = vINT;
									nLen = (ULONG)INT_DISPLAYLEN;
									break;
								case DBTYPE_I8:
									nFieldType = vINT;
									nLen = (ULONG)BIGINT_DISPAYLEN;
									break;
								case DBTYPE_UI2:  //won't have this, include for completeness...
									nFieldType = vINT; 
									nLen = (ULONG)TINYINT_DISPLAYLEN;
									break;
								case DBTYPE_UI4:  //ditto 
									nFieldType = vINT; 
									nLen = (ULONG)INT_DISPLAYLEN;
									break;
								case DBTYPE_DBTIMESTAMP:
									nFieldType = vDATETIME;
									nLen = (ULONG) DATETIME_DISPALYLEN;
									break;
								case DBTYPE_DATE:  //not sure DBASE will come through as this or DBTYPE_DATE
									nFieldType = vDATETIME;
									nLen = (ULONG) DATETIME_DISPALYLEN;
									break;
								case DBTYPE_DBDATE:
									nFieldType = vDATETIME;
									nLen = (ULONG) DATETIME_DISPALYLEN;
									break;
								case DBTYPE_NUMERIC:
								case DBTYPE_DECIMAL:
									nFieldType = vNUMERIC;
									BYTEprec = SrcBulkRecord.GetDataPrecision(nCol); 
									BYTEscale = SrcBulkRecord.GetDataScale(nCol);
								//	if (BYTEscale > 0) BYTEprec++;
								//	BYTEprec++;
									nLen = (ULONG) BYTEprec;
									break;
								case DBTYPE_R8:
								case DBTYPE_R4:
									nFieldType = vFLOAT;
									BYTEprec = SrcBulkRecord.GetDataPrecision(nCol); 
									BYTEscale = SrcBulkRecord.GetDataScale(nCol); 
									nLen = (ULONG) BYTEprec;
									break;
								case DBTYPE_STR: 
									nFieldType = vCHAR;
									if (nLen > 255) nFieldType = vVARCHAR;
									break;
								case DBTYPE_WSTR:	//this is seen in DBase files
									nFieldType = vCHAR;
									if (nLen > 255) nFieldType = vVARCHAR;
									break;
								case DBTYPE_CY:
									nFieldType = vMONEY;
									nLen = (ULONG) MONEY_DISPLAYLEN;
									break;
								case DBTYPE_BOOL:
									nFieldType = vBIT;
									nLen = (ULONG) BIT_DISPALYLEN;
									break;
								default:
									nFieldType = vCHAR;
									break;
							}
							
							//add the field...
							//replace spaces with underscore in name..because BCP will bomb
							iStrLen = SysStringLen(bstrName);
							TCHAR szName[BUFF_MED]; memset(szName, 0, BUFF_MED);
							WideCharToMultiByte(CP_ACP, 0, bstrName, iStrLen, szName, iStrLen+1, NULL, NULL);
							szName[iStrLen]= '\0';
							TCHAR szBuffer[MAX_FILE_LEN]; memset(szBuffer, 0, MAX_FILE_LEN);
							Utilities::ReplaceChar(szName, szBuffer, TCHAR(' '), TCHAR('_'));

							_bstr_t bstrName2(szBuffer);
							m_pFormatObj->AddField(bstrName2, bstrLen, nFieldType, &bSuccess);
							if (bSuccess) {
								iAddCnt++;
								if (BYTEprec != 0 || BYTEscale != 0 ) {
									IFieldInfo* pField = NULL;
									m_pFormatObj->GetByName(bstrName2, &pField);
									if (pField != NULL) {
										pField->put_Precision((long)BYTEprec);
										pField->put_Scale((long)BYTEscale);
										pField->Release();
										pField = NULL;
									}
								}
							}
						}

						if (iAddCnt != nCol ) {
							bSuccess = VARIANT_FALSE;
							_stprintf(szError, _T("DTRAN ERROR: CdeXBase::LoadFormat() Problems adding FieldInfo objects.  DBF column #: %d. FieldInfo objects #: %d"), nCol, iAddCnt);
							m_listErrors.push_back(szError);
						}
						else
							bSuccess = VARIANT_TRUE;
						
					}  //for
				} 
			}

			if (!bSuccess) {
				TCHAR szErr[BUFF_MED]; memset(szErr, 0, BUFF_MED);
				int iStrLen =  SysStringLen(bError);
				WideCharToMultiByte(CP_ACP, 0, bError, iStrLen, szErr, iStrLen+1, NULL, NULL);
				szErr[iStrLen]= '\0';
				memset(szError, 0, MAX_ERR_LEN);
				_stprintf(szError, _T("DTRAN ERROR: CdeXBase::LoadFormat() Error occurred in OleDB processing steps. %s"), ((_tcslen(szErr) == 0) ? _T("") : szErr) );
				m_listErrors.push_back(szError);
			}

			if (m_nXBaseType != vDBASE) 
				Utilities::DeleteDSN(bstrDSN);
		}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeXBase::LoadFormat() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeXBase::LoadFormat() catch statement.");
		bSuccess = VARIANT_FALSE;
	}

	*pRetVal = bSuccess;

	return S_OK;
}

