// deFixed.cpp : Implementation of CdeFixed
#include "stdafx.h"
#include "DTran.h"
#include "deFixed.h"
#include "stdio.h"
#include "Utilities.h"
#if !defined
#include <fstream>
#endif

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CdeFixed


STDMETHODIMP CdeFixed::Export(IDispatch *pIn, DESTACTION action, VARIANT_BOOL *bRetVal)
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
					pProcess->FixedToSQL(this, pSQL, &bSuccess);
					pSQL->Release();
					pSQL = NULL;
				}
				else
					m_listErrors.push_back( L"DTRAN ERROR:  CdeFixed::Export() -- Can not get deSQL interface pointer from input IDispatch pointer.");
			
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
				else 
					pProcess->get_RowsLoaded(&m_lRowsLoaded);

			}
			else
				m_listErrors.push_back( L"DTRAN ERROR:  CdeFixed::Export() -- Object passed in is not a deSQL object.");
		}
		else
			m_listErrors.push_back( L"DTRAN ERROR:  CdeFixed::Export() -- Internally used process object is NULL.");
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeFixed::Export() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back( L"DTRAN ERROR:  CdeFixed::Export() catch statement.");
		bSuccess = VARIANT_FALSE;
	}
	if (pProcess != NULL) 
		pProcess->Release();
	pProcess = NULL;
	*bRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CdeFixed::Import(IDispatch *pIn, DESTACTION action, VARIANT_BOOL *bRetVal)
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

			//SQL Table-->Fixed File
			HRESULT hr = pProcess->IsSQLObject(pIn, &bTextObj);
			if (bTextObj == VARIANT_TRUE)
			{
				IdeSQL* pSQL = NULL;
				pIn->QueryInterface(IID_IdeSQL, (void**) &pSQL);
				if (pSQL != NULL) {			
					pProcess->put_Action(action);
					pProcess->put_TraceLogOn(m_bTraceLogOn);
					pProcess->SQLToFixed(pSQL, this, 0, 0, &bSuccess);
					pSQL->Release();
					pSQL = NULL;
				}
				else
					m_listErrors.push_back( L"DTRAN ERROR:  CdeFixed::Import() -- Can not get deSQL interface pointer from input IDispatch pointer.");

	
/*			VARIANT_BOOL bTruncation = VARIANT_FALSE;
			pProcess->get_TruncationOccurred(&bTruncation);
			if (bTruncation) {
				pSQL->FieldResize(vCORRECT, &bSuccess);  //correct the field collection lengths...
				if (bSuccess) {
					pProcess->put_Action(vDROPADD);  //we created a file in the first pass that needs to be overwritten
					pProcess->SQLToFixed(pSQL, this, 0, 0, &bSuccess); 
				}
			}	
*/
				
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
				else 
					pProcess->get_RowsLoaded(&m_lRowsLoaded);

			//pSQL->Release();
				pSQL = NULL;
			}
			else
				m_listErrors.push_back( L"DTRAN ERROR:  CdeFixed::Import() -- Object passed in is not a deSQL object.");
			pProcess = NULL;
		}
		else
			m_listErrors.push_back( L"DTRAN ERROR:  CdeFixed::Import() -- Internally used process object is NULL.");
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeFixed::Import() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeFixed::Import() catch statement.");
		bSuccess = VARIANT_FALSE;
	}
	if (pProcess != NULL) 
			pProcess->Release();
	pProcess = NULL;
	*bRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CdeFixed::SetFormat(IFormat* pObj, VARIANT_BOOL* bSuccess)
{
	m_pFormatObj->CopyFormat(pObj, bSuccess);	
	return S_OK;
}

STDMETHODIMP CdeFixed::LoadFormat(VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_listErrors.clear();
	USES_CONVERSION;

	try {
			TCHAR szErr[MAX_ERR_LEN];  memset(szErr, 0, MAX_ERR_LEN);
			TCHAR szFormatFile[MAX_FILE_LEN];  memset(szFormatFile, MAX_FILE_LEN, 0);

			_bstr_t bstrFile = Utilities::ConstructFullPath(m_bstrDirectory, m_bstrFileName);
			TCHAR szFile[BUFF_MED]; memset(szFile, 0, BUFF_MED);
			int iStrLen =  SysStringLen(bstrFile);
			WideCharToMultiByte(CP_ACP, 0, bstrFile, iStrLen, szFile, iStrLen+1, NULL, NULL);
			szFile[iStrLen]= '\0';
			Utilities::ChangeFileExtension(szFile, _T("FMT"), szFormatFile);
			if (Utilities::FileCanBeRead(W2A(bstrFile), szErr) )
			{
				m_pFormatObj->Clear();
				_bstr_t bstrFormatFile(szFormatFile);
				m_pFormatObj->Load(bstrFormatFile, vTNSFORMAT, &bSuccess);
				if (!bSuccess)
					TransferErrors();
			}
			else {
				m_listErrors.push_back(szErr);
			}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeFixed::LoadFormat() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeFixed::LoadFormat() catch statement.");
		bSuccess = VARIANT_FALSE;
	}

	*pRetVal = bSuccess;
	return S_OK;
}


STDMETHODIMP CdeFixed::ValidateFile(BSTR FileName, long TestRecNum, long MaxErrors, VARIANT_BOOL* pRetVal)
{
	USES_CONVERSION;
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_listErrors.clear();

	try 
	{
		bool bMultiFile = false;
		int iTotalErrors = 0;
        TCHAR szErr[MAX_ERR_LEN];  memset(szErr, 0, MAX_ERR_LEN);
		_bstr_t bstrFile = FileName;
		if (bstrFile.length() == 0) 
		{
            bstrFile = Utilities::ConstructFullPath(m_bstrDirectory, m_bstrFileName);
		}

		//multifile validation file name should look like "directory\filename*.txt"
		wchar_t* p = wcschr(bstrFile, L'*');
		if (p != NULL)
		{
			bMultiFile = true;
			TCHAR szTempFile[MAX_FILE_LEN]; memset(szTempFile, 0, MAX_FILE_LEN);
			Utilities::RemoveChar(W2A(bstrFile), _T('*'), szTempFile);
			bstrFile = A2W(szTempFile);
		}

		//construct error file 
		TCHAR szDir[MAX_FILE_LEN];  memset(szDir, 0, MAX_FILE_LEN);
		TCHAR szName[MAX_FILE_LEN];  memset(szName, 0, MAX_FILE_LEN);
		TCHAR szExt[BUFF_LEN];  memset(szExt, 0, BUFF_LEN);
		Utilities::BreakUpFileName(bstrFile, szDir, szName, szExt);

		TCHAR szCurrentDir[MAX_FILE_LEN]; memset(szCurrentDir, 0, MAX_FILE_LEN);
		GetCurrentDirectory(MAX_FILE_LEN, szCurrentDir);

		TCHAR szErrorFile[MAX_FILE_LEN]; memset(szErrorFile, 0, MAX_FILE_LEN);
		_sntprintf(szErrorFile, MAX_FILE_LEN, "%s\\%s.%s.err", szCurrentDir, szName, szExt);
		szErrorFile[_tcslen(szErrorFile)+1] = '\0';

		FILE* ptrErrorFile = NULL;
		DeleteFile(szErrorFile);

		ptrErrorFile = _tfopen(szErrorFile, "a");
		
		HANDLE          hFind = 0;
		WIN32_FIND_DATA wfd = {0};
		SetCurrentDirectory(szDir);
		TCHAR szFileBuffer[BUFF_LEN]; memset(szFileBuffer, 0, BUFF_LEN);
		if (bMultiFile)
			_sntprintf(szFileBuffer, BUFF_LEN, "%s*.%s", szName, szExt);
		else
			_sntprintf(szFileBuffer, BUFF_LEN, "%s.%s", szName, szExt);
		if ( _tcslen(szFileBuffer) > 0) 
		{
			if ((hFind = FindFirstFile(szFileBuffer, &wfd)) != INVALID_HANDLE_VALUE) {
				int iFileCnt = 0;			
				bool bFileFound = true;
				TCHAR szFullFilePath[MAX_FILE_LEN]; 
				TCHAR szBuffer[BUFF_MED]; 
				if (ptrErrorFile && bMultiFile)
					_ftprintf(ptrErrorFile, _T("\r\n ************************ Multifile Validation ************************ "));
				while (bFileFound == true)
				{
					memset(szFileBuffer, 0, BUFF_LEN);
					_tcscpy(szFileBuffer, wfd.cFileName);
					memset(szFullFilePath, 0, MAX_FILE_LEN);
					_sntprintf(szFullFilePath, MAX_FILE_LEN, "%s\\%s", szDir, szFileBuffer);
					if (ptrErrorFile) {
						_sntprintf(szBuffer, BUFF_MED,_T("\r\n Data File Name: %s \r\n"), szFullFilePath);
						_ftprintf(ptrErrorFile, szBuffer);
					}
					iFileCnt++;
					int iErrors = ValidateFile(A2W(szFullFilePath), TestRecNum, MaxErrors, ptrErrorFile, szErrorFile);
					if (iErrors == 0) _ftprintf(ptrErrorFile, _T("\r\n *** No errors found. *** \r\n") );
					iTotalErrors += iErrors;
					if (FindNextFile(hFind, &wfd) == TRUE) {
						memset(szFileBuffer, 0, BUFF_LEN);
						_tcscpy(szFileBuffer, wfd.cFileName);
					}
					else
						bFileFound = false;
				}  //while
			}
			else {
				m_listErrors.push_back(L"DTRAN ERROR: CdeDelim::ValidateFile() FindFirstFile failed.");
			}
			FindClose(hFind);
		}
		else {
			m_listErrors.push_back(L"DTRAN ERROR: CdeDelim::ValidateFile() File name could not be parsed out of input argument.");
		}
		SetCurrentDirectory(szCurrentDir);
		if (iTotalErrors == 0) {
			if (ptrErrorFile) 
				fclose(ptrErrorFile);
			DeleteFile(szErrorFile);
			bSuccess = VARIANT_TRUE;
		}
		else {
			TCHAR szBuffer[BUFF_MED];
			memset(szBuffer, 0, BUFF_MED);
			_sntprintf(szBuffer, BUFF_MED,_T("\r\n See Error File: %s \r\n"), szErrorFile);
			m_listErrors.push_back(szBuffer);
			if (ptrErrorFile) {
				fflush(ptrErrorFile);
				fclose(ptrErrorFile);
			}
		}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeDelim::ValidateFile() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeDelim::ValidateFile() catch statement.");
		bSuccess = VARIANT_FALSE;
	}
	*pRetVal = bSuccess;
	return S_OK;
}

int CdeFixed::ValidateFile(_bstr_t bstrFile, long lTestRecNum, long lMaxErrors, FILE* ptrErrorFile, LPTSTR ErrorFile)
{
	USES_CONVERSION;
	long lErrorCnt = 0;
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	TCHAR szErr[MAX_ERR_LEN];  memset(szErr, 0, MAX_ERR_LEN);

	if (Utilities::FileCanBeRead(W2A(bstrFile), szErr) ) 
		{
			m_pFormatObj->ValidFieldCollection(&bSuccess);
			if (bSuccess) 
			{
				bSuccess = VARIANT_FALSE;
				long lFieldCnt, lTotalLength;
				m_pFormatObj->get_FieldCount(&lFieldCnt);
				m_pFormatObj->TotalFieldLength(&lTotalLength);

				ifstream fileData(W2A(bstrFile));
				if (fileData.is_open()) 
				{
					long lRecCnt = 1;
					TCHAR* pzBuffer = new TCHAR[lTotalLength+1000];
					memset(pzBuffer, 0, lTotalLength+1000);
					TCHAR* pzBufPrior = new TCHAR[lTotalLength+1000];
					memset(pzBufPrior, 0, lTotalLength+1000);

					fileData.getline(pzBuffer, lTotalLength+1000);

					bool bPrintRec = false;
					bool bContinue = true;
					pzBuffer[_tcslen(pzBuffer)+1] = '\0';
	
					while (bContinue) 
					{
						if (pzBuffer && fileData) 
						{
							TCHAR* pzBufTemp = pzBuffer;
							if (*pzBufTemp) {
								if (bPrintRec ) {
									if (ptrErrorFile != NULL) {
										_ftprintf(ptrErrorFile, pzBufTemp);
										_ftprintf(ptrErrorFile, _T("\r\n"));
										_ftprintf(ptrErrorFile, _T("\r\n"));
										fflush(ptrErrorFile);
										bPrintRec = false;
									}
								}
							}

							WORD dwRecSize = fileData.gcount();
							if (dwRecSize != lTotalLength+1) //add one to account for carriage return in dwRecSize
							{
								lErrorCnt++;
								TCHAR buffer[BUFF_LARGE]; memset(buffer, 0, BUFF_LARGE);
								sprintf(buffer, _T("DTRAN ERROR - Validation Error: Format Object total field length: %d does not match record #: %d   length: %d.\r\n"), lTotalLength, lRecCnt, (long)dwRecSize);
								if (lErrorCnt == 1)
									m_listErrors.push_back(A2W(buffer));

								if (ptrErrorFile != NULL) {
									_ftprintf(ptrErrorFile, buffer);
									_ftprintf(ptrErrorFile, "\r\n");
									if (_tcslen(pzBufPrior) != 0 ) {
										_ftprintf(ptrErrorFile,	pzBufPrior);			//print prior
										_ftprintf(ptrErrorFile, "\r\n");
									}
									_ftprintf(ptrErrorFile,	pzBuffer);			//print current
									_ftprintf(ptrErrorFile, "\r\n");
									bPrintRec = true;
								}
								
							}
							pzBufTemp = NULL;
							_tcscpy(pzBufPrior, pzBuffer);
						}
						else {
							if (lRecCnt == 1)
								m_listErrors.push_back(L"Input file is empty.  CdeFixed::ValidateFile() can not continue.\r\n");
							break;
						}
						memset(pzBuffer, 0, lTotalLength+1000);
						fileData.getline(pzBuffer, lTotalLength+1000); 
						if (pzBuffer == NULL) 
							bContinue = false;
						else if (lRecCnt == lTestRecNum )
							bContinue = false;
						if (lMaxErrors == lErrorCnt && lMaxErrors != 0)  //MaxErrors == 0 means take all errors
							bContinue = false;
						lRecCnt++;
					}
					if ( pzBuffer )
						delete pzBuffer;
					pzBuffer = NULL;
					if (pzBufPrior)
						delete pzBufPrior;
					pzBufPrior = NULL;
					fileData.close();
					if (lErrorCnt != 0 ) 
					{
						TCHAR buffer[BUFF_LARGE]; memset(buffer, 0, BUFF_LARGE);
						_sntprintf(buffer, BUFF_LARGE, _T("Validation errors were found in %d record(s) for file %s.\r\n"), lErrorCnt, W2A(bstrFile));
						m_listErrors.push_back(A2W(buffer));
						memset(buffer, 0, BUFF_LARGE);
						_sntprintf(buffer, BUFF_LARGE, _T("\r\nTotal records with validation errors: %d. Number of records evaluated: %d\r\n"), lErrorCnt, lRecCnt);
						_ftprintf(ptrErrorFile, buffer);
					}
					if (m_listErrors.size() == 0) bSuccess = VARIANT_TRUE;
				}
				else
					m_listErrors.push_back(L"DTRAN ERROR: File could not be opened.  CdeFixed::ValidateFile() can not continue.");
			}
			else
				TransferErrors();
		}
		else 
			m_listErrors.push_back(A2W(szErr));

	return lErrorCnt;
}

STDMETHODIMP CdeFixed::Exists(VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_listErrors.clear(); 

	try {

		_bstr_t bstrFileLoc = Utilities::ConstructFullPath(m_bstrDirectory, m_bstrFileName);

		TCHAR szErr[MAX_ERR_LEN];
		memset(szErr, 0, MAX_ERR_LEN);

		TCHAR szFileLoc[BUFF_MED]; memset(szFileLoc, 0, BUFF_MED);
		int iStrLen =  SysStringLen(bstrFileLoc);
		WideCharToMultiByte(CP_ACP, 0, bstrFileLoc, iStrLen, szFileLoc, iStrLen+1, NULL, NULL);
		szFileLoc[iStrLen]= '\0';

		if ( !Utilities::FileCanBeRead( szFileLoc, szErr ) ) {
			_bstr_t bstrTemp(szErr);
			m_listErrors.push_back(bstrTemp);
		}
		else
			bSuccess = VARIANT_TRUE;
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeFixed::Exists() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeFixed::Exists() catch statement.");
		bSuccess = VARIANT_FALSE;
	}

	*pRetVal = bSuccess;

	return S_OK;
}

STDMETHODIMP CdeFixed::get_Format(IFormat **pObj)
{
	m_pFormatObj->AddRef();
	*pObj = m_pFormatObj;

	return S_OK;
}

STDMETHODIMP CdeFixed::put_Format(IFormat *pObj)
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

STDMETHODIMP CdeFixed::put_TraceLogOn(VARIANT_BOOL newVal)
{
	m_bTraceLogOn = newVal;
	return S_OK;
}

STDMETHODIMP CdeFixed::get_TraceLogOn(VARIANT_BOOL* pVal)
{
	*pVal = m_bTraceLogOn;
	return S_OK;
}

STDMETHODIMP CdeFixed::get_Directory(BSTR *pVal)
{
	*pVal = m_bstrDirectory.copy();

	return S_OK;
}

STDMETHODIMP CdeFixed::put_Directory(BSTR newVal)
{
	_bstr_t bstrDir = newVal;

/*	if ( Utilities::IsSubDirPath((TCHAR*) bstrDir) ) {
		m_bstrDirectory += L"\\";  
		m_bstrDirectory += bstrDir;
	}
	else
		m_bstrDirectory = newVal;
*/

	if (wcscmp(bstrDir, L"") != 0 )
		m_bstrDirectory = newVal;

	//as a rule the directory path is stored w/out a backslash
	TCHAR bufOut[512];
	memset(bufOut, 5122, 0);

	TCHAR szDir[BUFF_MED]; memset(szDir, 0, BUFF_MED);
	int iStrLen =  SysStringLen(m_bstrDirectory);
	WideCharToMultiByte(CP_ACP, 0, m_bstrDirectory, iStrLen, szDir, iStrLen+1, NULL, NULL);
	szDir[iStrLen]= '\0';

	if (Utilities::RemoveLastSlash(szDir, bufOut) > 0 ) {
		_bstr_t bstrTemp(bufOut);
		m_bstrDirectory = bstrTemp;
	}
	return S_OK;
}

STDMETHODIMP CdeFixed::get_FileName(BSTR *pVal)
{
	*pVal = m_bstrFileName.copy();

	return S_OK;
}

STDMETHODIMP CdeFixed::put_FileName(BSTR newVal)
{
	m_bstrFileName = newVal;

	return S_OK;
}

STDMETHODIMP CdeFixed::get_RowsLoaded(long *pVal)
{
	*pVal = m_lRowsLoaded;

	return S_OK;
}


STDMETHODIMP CdeFixed::get_Version(BSTR *pVal)
{
	*pVal = m_bstrVersion.copy();
	return S_OK;
}

STDMETHODIMP CdeFixed::get_FirstRow(long *pVal)
{
	*pVal = m_lFirstRow;

	return S_OK;
}

STDMETHODIMP CdeFixed::put_FirstRow(long newVal)
{
	m_lFirstRow = newVal; 

	return S_OK;
}

STDMETHODIMP CdeFixed::get_LastRow(long *pVal)
{
	*pVal = m_lLastRow; 

	return S_OK;
}

STDMETHODIMP CdeFixed::put_LastRow(long newVal)
{
	m_lLastRow = newVal; 

	return S_OK;
}

STDMETHODIMP CdeFixed::get_ErrMsgList(BSTR *pVal)
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

STDMETHODIMP CdeFixed::get_ErrMsgs(BSTR *pVal)
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

int CdeFixed::TransferErrors() 
{
	int iErrCnt = 0;
	BSTR Temp;
	VARIANT_BOOL bMoreErrs = VARIANT_FALSE;
	
	m_pFormatObj->GetFirstError(&Temp, &bMoreErrs);
	_bstr_t bErr = Temp;
	SysFreeString(Temp);

	if (wcscmp(bErr, L"") != 0 ) 
	{
		m_listErrors.push_back(bErr);
		while (bMoreErrs && m_listErrors.size() < ERR_LIMIT) 
		{
			m_pFormatObj->GetNextError(&Temp, &bMoreErrs);
			bErr = Temp;
			SysFreeString(Temp);
			if (wcscmp(bErr, L"") != 0 ) 
				m_listErrors.push_back(bErr);
		}
	}

	return iErrCnt;
}