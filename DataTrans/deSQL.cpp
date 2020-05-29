// deSQL.cpp : Implementation of CdeSQL
#include "stdafx.h"
#include "DTran.h"
#include "deSQL.h"


#if !defined
#include "ADO.h"
#include "Utilities.h"
#endif



/////////////////////////////////////////////////////////////////////////////


STDMETHODIMP CdeSQL::put_TraceLogOn(VARIANT_BOOL newVal)
{
	m_bTraceLogOn = newVal;
	return S_OK;
}

STDMETHODIMP CdeSQL::get_TraceLogOn(VARIANT_BOOL* pVal)
{
	*pVal = m_bTraceLogOn;
	return S_OK;
}

STDMETHODIMP CdeSQL::get_Server(BSTR *pVal)
{
	*pVal = m_bstrServer.copy(); 

	return S_OK;
}

STDMETHODIMP CdeSQL::put_Server(BSTR newVal)
{
	m_bstrServer = newVal;

	return S_OK;
}

STDMETHODIMP CdeSQL::get_Database(BSTR *pVal)
{
	*pVal = m_bstrDatabase.copy();

	return S_OK;
}

STDMETHODIMP CdeSQL::put_Database(BSTR newVal)
{
	m_listErrors.clear();
	if (m_bDropTable) {
		
		if (wcscmp(m_bstrServer, L"")	!= 0	&&
			wcscmp(m_bstrDatabase, L"") != 0	&&
			wcscmp(m_bstrTable, L"")	!= 0		)  
		{
			CADO objADO(L"", L"");
			if (SUCCEEDED(objADO.DropTable(m_bstrServer, m_bstrDatabase, m_bstrTable)))
				m_bDropTable = false;
			else
				Utilities::TransferErrors(objADO.m_listErrors, m_listErrors);
		}
	}
	m_pFormatObj->Clear();	

	m_bstrDatabase = newVal;

	return S_OK;
}

STDMETHODIMP CdeSQL::get_Table(BSTR *pVal)
{
	*pVal = m_bstrTable.copy();

	return S_OK;
}

STDMETHODIMP CdeSQL::put_Table(BSTR newVal)
{
	m_listErrors.clear();
	
	if (m_bDropTable) {
		
		if (wcscmp(m_bstrServer, L"")	!= 0	&&
			wcscmp(m_bstrDatabase, L"") != 0	&&
			wcscmp(m_bstrTable, L"")	!= 0		)  
		{

			CADO objADO(L"", L"");
			if (SUCCEEDED(objADO.DropTable(m_bstrServer, m_bstrDatabase, m_bstrTable)))
				m_bDropTable = false;
			else
				Utilities::TransferErrors(objADO.m_listErrors, m_listErrors);
		}
	}

	m_bstrExportQuery = L"";
	m_pFormatObj->Clear();	

	m_bstrTable = newVal;
	return S_OK;
}

STDMETHODIMP CdeSQL::get_ID_Name(BSTR *pVal)
{
	*pVal = m_bstrIDName.copy();
	return S_OK;
}

STDMETHODIMP CdeSQL::put_ID_Name(BSTR newVal)
{
	m_bstrIDName = newVal;
	return S_OK;
}

STDMETHODIMP CdeSQL::get_ID_Definition(BSTR *pVal)
{
	*pVal = m_bstrIDDef.copy();
	return S_OK;
}

STDMETHODIMP CdeSQL::put_ID_Definition(BSTR newVal)
{
	m_bstrIDDef = newVal;
	return S_OK;
}

STDMETHODIMP CdeSQL::get_UserID(BSTR *pVal)
{
	*pVal = m_bstrUserID.copy();

	return S_OK;
}

STDMETHODIMP CdeSQL::put_UserID(BSTR newVal)
{
	m_bstrUserID = newVal;

	return S_OK;
}

STDMETHODIMP CdeSQL::get_Password(BSTR *pVal)
{
	*pVal = m_bstrPassword.copy();

	return S_OK;
}

STDMETHODIMP CdeSQL::put_Password(BSTR newVal)
{
	m_bstrPassword = newVal;

	return S_OK;
}


STDMETHODIMP CdeSQL::get_RowsLoaded(long *pVal)
{
	*pVal = m_lRowsLoaded;

	return S_OK;
}

STDMETHODIMP CdeSQL::get_ErrMsgs(BSTR *pVal)
{
	_bstr_t bstrErrors = L"";
	try {

		bstrErrors = Utilities::GetErrorList(m_listErrors, false);
	}
		catch(_com_error &e) {
		bstrErrors = Utilities::ConstructErrMsg(L"COM ERROR: CdeSQL::get_ErrMsgs catch statement.", e.Source(), e.Description(), e.ErrorMessage());
	}
	catch (...) {
		bstrErrors = L"DTRAN ERROR: CdeSQL::get_ErrMsgs catch statement.";
	}

	*pVal = bstrErrors.copy();
	return S_OK;
}

STDMETHODIMP CdeSQL::put_Format(IFormat* pObj)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_pFormatObj->CopyFormat(pObj, &bSuccess);

//	m_pFormatObj = NULL;
//	m_pFormatObj = pObj;
	return S_OK;
}

STDMETHODIMP CdeSQL::get_Format(IFormat** pObj)
{
	m_pFormatObj->AddRef();
	*pObj = m_pFormatObj;
	return S_OK;
}

STDMETHODIMP CdeSQL::SetFormat(IFormat* pObj, VARIANT_BOOL* bSuccess)
{
	m_listErrors.clear();
	try {
		m_pFormatObj->CopyFormat(pObj, bSuccess);	
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeSQL::SetFormat() - caught by catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch(...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::SetFormat() - caught by catch statement.");
		VARIANT_BOOL bRet = VARIANT_FALSE;
		*bSuccess = bRet;

	}
	return S_OK;
}



STDMETHODIMP CdeSQL::Import(IDispatch *pIn, DESTACTION action, VARIANT_BOOL *bRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_listErrors.clear();
	m_lRowsLoaded = 0;
	IProcess* pProcess = NULL;

#ifdef FILE_DEBUG
		TCHAR BufDir[MAX_FILE_LEN];
        GetCurrentDirectory(MAX_FILE_LEN, BufDir);
		_tcscat(BufDir, _T("\\debug_delim.txt"));
		FILE* ptrLog;
		ptrLog = fopen(BufDir, "a");
		if (ptrLog != NULL){
			fprintf(ptrLog, "\r\n*********************** CdeSQL::Import ********************************************\r\n");
			fflush(ptrLog);
		}
#endif
	try {
		CoCreateInstance(CLSID_Process, NULL, CLSCTX_INPROC_SERVER, IID_IProcess, (void**) &pProcess);
		if (pProcess != NULL)
		{
#ifdef FILE_DEBUG
			if (ptrLog != NULL){
				fprintf(ptrLog, "Interface IProcess successfully created. \r\n");
				fflush(ptrLog);
			}
#endif
			pProcess->put_Action(action);
			pProcess->put_TraceLogOn(m_bTraceLogOn);
			
			VARIANT_BOOL bTextObj = VARIANT_FALSE;
			VARIANT_BOOL bDelimObj = VARIANT_FALSE;
			VARIANT_BOOL bXBaseObj = VARIANT_FALSE;
#ifdef FILE_DEBUG
			if (ptrLog != NULL){
				fprintf(ptrLog, "Test if Fixed object.\r\n");
				fflush(ptrLog);
			}
#endif
			HRESULT hr = pProcess->IsFixedFileObject(pIn, &bTextObj);
#ifdef FILE_DEBUG
			if (ptrLog != NULL){
				fprintf(ptrLog, "Test if Delim object.\r\n");
				fflush(ptrLog);
			}
#endif
			hr = pProcess->IsDelimFileObject(pIn, &bDelimObj);
#ifdef FILE_DEBUG
			if (ptrLog != NULL){
				fprintf(ptrLog, "Test if XBase object.\r\n");
				fflush(ptrLog);
			}
#endif
			hr = pProcess->IsXBaseObject(pIn, &bXBaseObj);
		
			if (bTextObj == VARIANT_TRUE)
			{
				IdeFixed* pFixed = NULL;
				pIn->QueryInterface(IID_IdeFixed, (void**) &pFixed);
				if (pFixed != NULL) {
					pProcess->FixedToSQL(pFixed, this, &bSuccess);
					pFixed->Release();
					pFixed = NULL;
				}
				else
					m_listErrors.push_back( L"DTRAN ERROR:  CdeSQL::Import() -- Can not get deFixed interface pointer from input IDispatch pointer.");
			}
			
			if (bDelimObj == VARIANT_TRUE)
			{
				IdeDelim* pDelim = NULL;
				pIn->QueryInterface(IID_IdeDelim, (void**) &pDelim);
				if (pDelim != NULL) {
#ifdef FILE_DEBUG
				if (ptrLog != NULL){
					fprintf(ptrLog, "Prior to call DelimToSQL \r\n");
					fflush(ptrLog);
					fclose(ptrLog);
				}
#endif
					pProcess->DelimToSQL(pDelim, this, &bSuccess);
#ifdef FILE_DEBUG
				ptrLog = fopen(BufDir, "a");
				if (ptrLog != NULL){
					fprintf(ptrLog, "Completed to call DelimToSQL \r\n");
					fflush(ptrLog);
				}
#endif
					pDelim->Release();
					pDelim = NULL;
				}
				else {
#ifdef FILE_DEBUG
					if (ptrLog != NULL){
						fprintf(ptrLog, "pDelim was not created \r\n");
						fflush(ptrLog);
					}
#endif				
					m_listErrors.push_back( L"DTRAN ERROR:  CdeSQL::Import() -- Can not get deDelim interface pointer from input IDispatch pointer.");
				}
			}

			if (bXBaseObj == VARIANT_TRUE)
			{
				IdeXBase* pXBase = NULL;
				pIn->QueryInterface(IID_IdeXBase, (void**) &pXBase);
				if (pXBase != NULL) {
					pProcess->XBaseToSQL(pXBase, this, &bSuccess);
					pXBase->Release();
					pXBase = NULL;
				}
				else
					m_listErrors.push_back( L"DTRAN ERROR:  CdeSQL::Import() -- Can not get deXBase interface pointer from input IDispatch pointer.");
			}
			
			if (bTextObj || bDelimObj || bXBaseObj) 
			{
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
				m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::Import() -- Object passed in is not a deFixed or deDelim or deXBase object.");
		}
		else
			m_listErrors.push_back( L"DTRAN ERROR: CdeSQL::Import( -- Internally used process object is NULL.");
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeSQL::Import() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::Import() catch statement.");
		bSuccess = VARIANT_FALSE;
	}

#ifdef FILE_DEBUG
		if (ptrLog != NULL){
			fprintf(ptrLog, "Release pointer to interface IProcess. \r\n");
			fflush(ptrLog);
		}
#endif
	if (pProcess != NULL) 
		pProcess->Release();
	pProcess = NULL;
	*bRetVal = bSuccess;
#ifdef FILE_DEBUG
		if (ptrLog != NULL){
			fprintf(ptrLog, "********************** Exit CdeSQL::Import ********************************************\r\n");
			fflush(ptrLog);
			fclose(ptrLog);
		}
#endif
	return S_OK;
}

STDMETHODIMP CdeSQL::Export(IDispatch *pIn, DESTACTION action, VARIANT_BOOL *bRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_listErrors.clear();
	m_lRowsLoaded = 0;
	IProcess* pProcess = NULL;
	try {
		CoCreateInstance(CLSID_Process, NULL, CLSCTX_INPROC_SERVER, IID_IProcess, (void**) &pProcess);
		if (pProcess != NULL)
		{
			pProcess->put_Action(action);
			pProcess->put_TraceLogOn(m_bTraceLogOn);

			VARIANT_BOOL bTextObj = VARIANT_FALSE;
			VARIANT_BOOL bXBaseObj = VARIANT_FALSE;
			VARIANT_BOOL bDelimObj = VARIANT_FALSE;
			HRESULT hr = pProcess->IsFixedFileObject(pIn, &bTextObj);
			hr = pProcess->IsXBaseObject(pIn, &bXBaseObj);
			hr = pProcess->IsDelimFileObject(pIn, &bDelimObj);

			if (bDelimObj == VARIANT_TRUE)
			{
				IdeDelim* pDelim = NULL;
				pIn->QueryInterface(IID_IdeDelim, (void**) &pDelim);
				if (pDelim != NULL) {
					pProcess->SQLToDelim(this, pDelim, &bSuccess);
                    pDelim->Release();
					pDelim = NULL;
				}
				else
					m_listErrors.push_back( L"DTRAN ERROR:  CdeSQL::Export() -- Can not get deDelim interface pointer from input IDispatch pointer.");
			}
			
			if (bXBaseObj == VARIANT_TRUE)
			{
				IdeXBase* pXBase = NULL;
				pIn->QueryInterface(IID_IdeXBase, (void**) &pXBase);
				if (pXBase != NULL) {
					pProcess->SQLToXBase(this, pXBase, &bSuccess);
					pXBase->Release();
					pXBase = NULL;
				}
				else
					m_listErrors.push_back( L"DTRAN ERROR:  CdeSQL::Export() -- Can not get deXBase interface pointer from input IDispatch pointer.");
			}

			if (bTextObj == VARIANT_TRUE)
			{
				IdeFixed* pFixed = NULL;
				pIn->QueryInterface(IID_IdeFixed, (void**) &pFixed);
				if (pFixed != NULL) {
					pProcess->SQLToFixed(this, pFixed, 0, 0, &bSuccess);
					pFixed->Release();
					pFixed = NULL;
				}
				else
					m_listErrors.push_back( L"DTRAN ERROR:  CdeSQL::Export() -- Can not get deFixed interface pointer from input IDispatch pointer.");


/*				VARIANT_BOOL bTruncation = VARIANT_FALSE;
				pProcess->get_TruncationOccurred(&bTruncation);
				if (bTruncation) {
					FieldResize(vCORRECT, &bSuccess);  //correct the field collection lengths...
				if (bSuccess) {
					pProcess->put_Action(vDROPADD);  //we created a file in the first pass that needs to be overwritten
					pProcess->SQLToFixed(this, pFixed, 0, 0, &bSuccess); 
					}
				}
				
*/			
			}

			if (bTextObj || bXBaseObj || bDelimObj) 
			{
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
				m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::Export() -- Object passed in is not a deFixed or deXBase or deDelim object.");
		}
		else
			m_listErrors.push_back( L"DTRAN ERROR: CdeSQL::Export() -- Internally used process object is NULL.");
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeSQL::Export() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch(...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::Export() catch statement.");
		bSuccess = VARIANT_FALSE;
	}
	
	if (pProcess != NULL) 
			pProcess->Release();
	pProcess = NULL;
	*bRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CdeSQL::ExportMulti(IDispatch* pIn, DESTACTION action, SPLITRULE rule, long lValue, VARIANT_BOOL* pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_listErrors.clear();
	IProcess* pProcess = NULL;
	try {

		long lRecChunk = 0;
		long lFileNum = 0;
		m_lRowsLoaded = 0;

		if (rule == vBYFILE) {
			if (lValue == 0) 
				m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::ExportMulti() -- Number of files must be specified.");
			else
				lFileNum = lValue;
		}
		else {
			if (lValue == 0) 
				m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::ExportMulti() -- Number of rows must be specified.");
			else
				lRecChunk = lValue;
		}

		CoCreateInstance(CLSID_Process, NULL, CLSCTX_INPROC_SERVER, IID_IProcess, (void**) &pProcess);
		if (pProcess != NULL && m_listErrors.size() == 0 )
		{
			VARIANT_BOOL bTextObj = VARIANT_FALSE;
			HRESULT hr = pProcess->IsFixedFileObject(pIn, &bTextObj);

			if (bTextObj == VARIANT_TRUE)
			{
				IdeFixed* pFixed = NULL;
				pIn->QueryInterface(IID_IdeFixed, (void**) &pFixed);
				if (pFixed != NULL) {
					pProcess->put_Action(action);
					pProcess->put_TraceLogOn(m_bTraceLogOn);
					pProcess->SQLToFixed(this, pFixed, lRecChunk, lFileNum,  &bSuccess);
					pFixed->Release();
					pFixed = NULL;
				}
				else
					m_listErrors.push_back( L"DTRAN ERROR:  CdeSQL::ExportMulti() -- Can not get deFixed interface pointer from input IDispatch pointer.");


/*				VARIANT_BOOL bTruncation = VARIANT_FALSE;
				pProcess->get_TruncationOccurred(&bTruncation);
				if (bTruncation) {
					FieldResize(vCORRECT, &bSuccess);  //correct the field collection lengths...
				if (bSuccess) {
					pProcess->put_Action(vDROPADD);
					pProcess->SQLToFixed(this, pFixed, lRecChunk, lFileNum, &bSuccess); 
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
			}
			else
				m_listErrors.push_back(L"DTRAN ERROR:CdeSQL::ExportMulti() -- Object passed in is not a deFixed object.");
			pProcess = NULL;
		}
		else
			m_listErrors.push_back( L"DTRAN ERROR: CdeSQL::ExportMulti() -- Internally used process object is NULL.");
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeSQL::ExportMulti() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::ExportMulti() catch statement.");
		bSuccess = VARIANT_FALSE;
	}
	if (pProcess != NULL) 
			pProcess->Release();
	pProcess = NULL;
	*pRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CdeSQL::ImportMulti(IDispatch *pIn, DESTACTION action, VARIANT_BOOL AddSource, VARIANT_BOOL *pRetVal)
{
//	_CrtMemState s1, s2, s3;
//	_CrtMemCheckpoint(&s1);
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_lRowsLoaded = 0;
	m_listErrors.clear();
	IProcess* pProcess = NULL;
	
#ifdef FILE_DEBUG
		TCHAR BufDir[MAX_FILE_LEN];
        GetCurrentDirectory(MAX_FILE_LEN, BufDir);
		_tcscat(BufDir, _T("\\debug_delim.txt"));
		FILE* ptrLog;
		ptrLog = fopen(BufDir, "a");
		if (ptrLog != NULL){
			fprintf(ptrLog, "\r\n*********************** CdeSQL::ImportMulti ********************************************\r\n");
			fflush(ptrLog);
		}
#endif
	try {

		CoCreateInstance(CLSID_Process, NULL, CLSCTX_INPROC_SERVER, IID_IProcess, (void**) &pProcess);
		if (pProcess != NULL)
		{
#ifdef FILE_DEBUG
			if (ptrLog != NULL){
				fprintf(ptrLog, "Interface IProcess successfully created. \r\n");
				fflush(ptrLog);
			}
#endif
			VARIANT_BOOL bFixedObj = VARIANT_FALSE;
			VARIANT_BOOL bDelimObj = VARIANT_FALSE;
#ifdef FILE_DEBUG
			if (ptrLog != NULL){
				fprintf(ptrLog, "Test if Fixed object.\r\n");
				fflush(ptrLog);
			}
#endif
			HRESULT hr = pProcess->IsFixedFileObject(pIn, &bFixedObj);
#ifdef FILE_DEBUG
			if (ptrLog != NULL){
				fprintf(ptrLog, "Test if Delim object.\r\n");
				fflush(ptrLog);
			}
#endif
			hr = pProcess->IsDelimFileObject(pIn, &bDelimObj);

			if (bFixedObj == VARIANT_TRUE || bDelimObj == VARIANT_TRUE)
			{
#ifdef FILE_DEBUG
				if (ptrLog != NULL){
					fprintf(ptrLog, "Prior to call MultiImportToSQL \r\n");
					fflush(ptrLog);
					fclose(ptrLog);
				}
#endif
				pProcess->put_Action(action);
				pProcess->put_TraceLogOn(m_bTraceLogOn);
				pProcess->put_AddSource(AddSource);
				pProcess->MultiImportToSQL(pIn, this, &bSuccess);
#ifdef FILE_DEBUG
				ptrLog = fopen(BufDir, "a");
				if (ptrLog != NULL){
					fprintf(ptrLog, "Completed call to MultiImportToSQL. \r\n");
					fflush(ptrLog);
				}
#endif
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

				long lRows;
				pProcess->get_RowsLoaded(&lRows);
				m_lRowsLoaded = lRows;

			}
			else {
				m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::ImportMulti() -- Object passed in is not a deFixed object.");
#ifdef FILE_DEBUG
			if (ptrLog != NULL){
				fprintf(ptrLog, "Neither Fixed or Delim object.\r\n");
				fflush(ptrLog);
			}
#endif
			}
		}
		else
			m_listErrors.push_back( L"DTRAN ERROR: CdeSQL::ImportMulti() -- Internally used process object is NULL.");
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeSQL::ImportMulti() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::ImportMulti() catch statement.");
		bSuccess = VARIANT_FALSE;
	}
	if (pProcess != NULL) {
		pProcess->Release();


#ifdef FILE_DEBUG
		if (ptrLog != NULL){
			fprintf(ptrLog, "Release pointer to interface IProcess. \r\n");
			fflush(ptrLog);
		}
#endif

	}
	pProcess = NULL;
	*pRetVal = bSuccess;
#ifdef FILE_DEBUG
		if (ptrLog != NULL){
			fprintf(ptrLog, "*******************************Exit CdeSQL::ImportMulti******************************\r\n");
			fflush(ptrLog);
			fclose(ptrLog);
		}
#endif
//_CrtMemCheckpoint(&s2);
//if (_CrtMemDifference(&s3, &s1, &s2) ) {
//		_CrtMemDumpStatistics(&s3);
//		_CrtMemDumpAllObjectsSince(&s3);
	return S_OK;
}


STDMETHODIMP CdeSQL::GetCreateTableSQL(BSTR *pRetVal)
{
	_bstr_t bstrSQL = L"CREATE TABLE ";
	bstrSQL += m_bstrOwner;
	bstrSQL += ".[";
	bstrSQL += m_bstrTable;
	bstrSQL += L"] (";

	IFieldInfo* pField = NULL;
	m_pFormatObj->First(&pField);

	long lCnt = 0;
	long lTotalFields = 0;
	m_pFormatObj->get_FieldCount(&lTotalFields);

	while (pField != NULL) 
	{
		lCnt++; 
		BSTR Temp;
		pField->get_Name(&Temp);
		_bstr_t bstrFieldName = Temp;
		SysFreeString(Temp);

		pField->get_Length(&Temp);
		_bstr_t bstrLength = Temp;
		SysFreeString(Temp);

		FIELDTYPE nType; 
		pField->get_Type(&nType);

		bstrSQL += "[";
		bstrSQL += bstrFieldName;
		bstrSQL += "]";
		bool bAddEndParan = false;

		long lTemp  = 0;
		_bstr_t bstrTemp = L"";
			
		pField->get_Precision(&lTemp);
		TCHAR szPrec[64]; memset(szPrec, 0, 64);
		_itoa((int) lTemp, szPrec, 10);
		_bstr_t bstrPrec(szPrec);
		
		pField->get_Scale(&lTemp);
		TCHAR szScale[64]; memset(szScale, 0, 64);
		_itoa((int) lTemp, szScale, 10);
		_bstr_t bstrScale(szScale);

		switch (nType) {
		case vBINARY:
			bstrSQL += L" BINARY"; 
			break;
		case vBIT:
			bstrSQL += L" BIT";
			break;
		case vCHAR:
			bstrSQL += L" CHAR(";
			bstrSQL += bstrLength;
			bAddEndParan = true;
			break;
		case vDATETIME:
			bstrSQL += L" DATETIME";
			break;
		case vDECIMAL: 
			bstrSQL += L" NUMERIC("; 
			bstrSQL += bstrPrec;
			bstrSQL += L", ";
			bstrSQL += bstrScale;
			bAddEndParan = true;
			break;
		case vFLOAT:
			bstrSQL += L" FLOAT";
			break;
		case vINT:
			bstrSQL += L" INT"; 
			break;
		case vMONEY:
			bstrSQL += L" MONEY";
			break;
		case vNUMERIC:
			bstrSQL += L" NUMERIC(";
			bstrSQL += bstrPrec;
			bstrSQL += L", ";
			bstrSQL += bstrScale;
			bAddEndParan = true;
			break;
		case vREAL:
			bstrSQL += L" REAL";
			break;
		case vSMALLDATETIME:
			bstrSQL += L" SMALLDATETIME";
			break;
		case vSMALLINT:
			bstrSQL += L" SMALLINT";
			break;
		case vSMALLMONEY:
			bstrSQL += L" SMALLMONEY";
			break;
		case vTEXT:
			bstrSQL += L" TEXT";
			break;
		case vTIMESTAMP:
			bstrSQL += L" TIMESTAMP";
			break;
		case vTINYINT:
			bstrSQL += L" TINYINT";
			break;
		case vVARCHAR:
			bstrSQL += L" VARCHAR(";
			bstrSQL += bstrLength;
			bAddEndParan = true;
			break;
		default:
			bstrSQL += L" VARCHAR(254)";
			break;
		}
		
		if (bAddEndParan) {
			if (lCnt != lTotalFields) 
				bstrSQL += L"), ";
			else
				bstrSQL += L")";
		}
		else if (lCnt != lTotalFields) 
			bstrSQL += L", ";

		pField->Release();
		pField = NULL;
		m_pFormatObj->Next(&pField);
	}
	//identity field ....
	if (wcscmp(m_bstrIDName, L"") != 0 ) {
		bstrSQL += L", [";
		bstrSQL += m_bstrIDName;
		bstrSQL += L"] ";
		bstrSQL += m_bstrIDDef;
	}
	bstrSQL += " )";
	*pRetVal = bstrSQL.copy();
	return S_OK;
}


STDMETHODIMP CdeSQL::Exists(VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_listErrors.clear();
	USES_CONVERSION;

	try {

		CADO objADO(m_bstrServer, m_bstrDatabase);
		if ( SUCCEEDED(objADO.OpenTrustedConnection()) ) {

			ADODB::_RecordsetPtr spRecordset = NULL;
			LPTSTR lpTemp =  Utilities::ConstructInfoSchemaSQL(m_bstrTable, m_bstrOwner);
			_bstr_t bQry = A2W(lpTemp);
		
			HRESULT hr = E_FAIL;
			hr = objADO.GetRecordSet((IDispatch**) &spRecordset, bQry);
	
			if (SUCCEEDED(hr) ) {

				if ( spRecordset != NULL) {
					int iRecCnt = spRecordset->RecordCount;
					if (iRecCnt != 0 )
						bSuccess = VARIANT_TRUE;
				}
			}

			objADO.CloseConnection();
		}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeSQL::Exists() - caught by catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::Exists() - caught by catch statement.");
		bSuccess = VARIANT_FALSE;
	}
	
	*pRetVal = bSuccess;

	return S_OK;
}

STDMETHODIMP CdeSQL::get_Version(BSTR *pVal)
{
	*pVal = m_bstrVersion.copy();

	return S_OK;
}

STDMETHODIMP CdeSQL::LoadFormat(VARIANT_BOOL *pRetVal)
{
	USES_CONVERSION;
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	HRESULT hrRet = E_FAIL;
	m_listErrors.clear();

	try {
		//clear collection
		m_pFormatObj->Clear();

		CADO objADO(m_bstrServer, m_bstrDatabase);
		if ( SUCCEEDED(objADO.OpenTrustedConnection()) ) {

			if (wcscmp(m_bstrExportQuery, L"") != 0 ) {
				TCHAR pzTableName[BUFF_LEN];  memset(pzTableName, 0, BUFF_LEN);
				if (SUCCEEDED(objADO.CreateTableFromQuery(m_bstrExportQuery, pzTableName )))  {
					_bstr_t _bstrTable(pzTableName);
					put_Table(_bstrTable);  // this action will delete any prior temporary tables..
					if (m_listErrors.size() == 0 ) {
						bSuccess = VARIANT_TRUE;
						m_bDropTable = true;
					}
				}
				else {
					m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::LoadFormat() Can not create table from query: " + m_bstrExportQuery);
					Utilities::TransferErrors(objADO.m_listErrors, m_listErrors);
				}
			}
			else
				bSuccess = VARIANT_TRUE;

			if (bSuccess) {

				bSuccess = VARIANT_FALSE;
				ADODB::_RecordsetPtr spRecordset;
				LPTSTR lpTemp = Utilities::ConstructInfoSchemaSQL(m_bstrTable, m_bstrOwner);
				_bstr_t bQry = A2W(lpTemp);
				int iColCount = 0;
				hrRet = objADO.GetRecordSet((IDispatch**) &spRecordset, bQry);
				if (spRecordset != NULL)  {

					int iRecCount = spRecordset->GetRecordCount();
					if (iRecCount > 0)
					{
						spRecordset->MoveFirst();
						long lStartPos = 1;
						long lEndPos = 0;
						long lLastEndPos = 0; 
						while (!spRecordset->adoEOF)
						{
							iColCount++;
							_variant_t vName, vType, vColLength, vPrecision, vScale;
							long lPrec, lScale;
							lPrec = 0; 
							lScale = 0;

							vName = spRecordset->Fields->GetItem("COLUMN_NAME")->Value;
							vType = spRecordset->Fields->GetItem("DATA_TYPE")->Value;
							vColLength = spRecordset->Fields->GetItem("CHARACTER_MAXIMUM_LENGTH")->Value;
							vPrecision = spRecordset->Fields->GetItem("NUMERIC_PRECISION")->Value;
							vScale = spRecordset->Fields->GetItem("NUMERIC_SCALE")->Value;

							if ( (vName.vt != VT_NULL && vName.vt != VT_EMPTY)  &&
								 (vType.vt != VT_NULL && vType.vt != VT_EMPTY)       )  
							{

								vName.ChangeType(VT_BSTR);
								vType.ChangeType(VT_BSTR);
								//vColLength.ChangeType(VT_I4);

								_bstr_t bstrName = vName.bstrVal;
								_variant_t vLen;
								vLen.vt = VT_I4;
								vLen.intVal = 25;

								if (vPrecision.vt == VT_NULL || vPrecision.vt == VT_EMPTY) {
									vPrecision.vt = VT_I4;
									vPrecision.intVal = 0;
								}

								if (vScale.vt == VT_NULL || vScale.vt == VT_EMPTY) {
									vScale.vt = VT_I4;
									vScale.intVal = 0;
								}

								FIELDTYPE nType;
								if (wcscmp(vType.bstrVal, L"numeric") == 0 	) {								
									vLen.intVal = vPrecision.intVal + 2;
									lPrec = vPrecision.intVal;
									lScale = vScale.intVal;
									nType = vNUMERIC;
								}
								else if (wcscmp(vType.bstrVal, L"decimal") == 0 )  {
									vLen.intVal = vPrecision.intVal + 2;
									lPrec = vPrecision.intVal;
									lScale = vScale.intVal;
									nType = vDECIMAL;
								}
								else if (wcscmp(vType.bstrVal, L"float")   == 0 ) {
									vLen.intVal = FLOAT_DISPLAYLEN;
									nType = vFLOAT;
								}
								else if ( wcscmp(vType.bstrVal, L"int")	  == 0 ) {
									vLen.intVal = INT_DISPLAYLEN;
									nType = vINT;
								}
								else if ( wcscmp(vType.bstrVal, L"bigint")  == 0 ) {
									vLen.intVal = BIGINT_DISPAYLEN;
									nType = vBIGINT;
								}
								else if ( wcscmp(vType.bstrVal, L"tinyint") == 0 ) {
									vLen.intVal = TINYINT_DISPLAYLEN;
									nType = vTINYINT;
								}
								else if ( wcscmp(vType.bstrVal, L"tinyint") == 0 ) {
									vLen.intVal = INT_DISPLAYLEN;
									nType = vSMALLINT;
								}
								else if (wcscmp(vType.bstrVal, L"datetime") == 0 ) {
									vLen.intVal = DATETIME_DISPALYLEN;
									nType = vDATETIME;
								}
								else if ( wcscmp(vType.bstrVal, L"bit") == 0 ) {
									vLen.intVal = BIT_DISPALYLEN;
									nType = vBIT;
								}
								else if ( wcscmp(vType.bstrVal, L"money") == 0 ) {
									vLen.intVal = MONEY_DISPLAYLEN;
									nType = vMONEY;
								}
								else if(wcscmp(vType.bstrVal, L"char") == 0 ) {
									vLen.intVal = vColLength.intVal;
									nType = vCHAR;
								}
								else if ( wcscmp(vType.bstrVal, L"varchar") == 0   ) {
									vLen.intVal = vColLength.intVal;
									nType = vVARCHAR;
								}
								else {
									nType = vVARCHAR;
									vLen.intVal = 25;
								}

								//replace spaces with underscore in name..because BCP will bomb
								TCHAR szBuffer[MAX_FILE_LEN]; memset(szBuffer, 0, MAX_FILE_LEN);
								_bstr_t bTemp = vName.bstrVal;
								TCHAR szTemp[BUFF_MED]; memset(szTemp, 0, BUFF_MED);
								int iStrLen =  SysStringLen(bTemp);
								WideCharToMultiByte(CP_ACP, 0, bTemp, iStrLen, szTemp, iStrLen+1, NULL, NULL);
								szTemp[iStrLen]= '\0';
								Utilities::ReplaceChar(szTemp, szBuffer, TCHAR(' '), TCHAR('_'));

								//cal start and end positions
								lStartPos = lLastEndPos + 1;
								lEndPos = lStartPos + vLen.intVal - 1;
								lLastEndPos = lEndPos;

								//add the field to the field collection....
								vLen.ChangeType(VT_BSTR);
								_bstr_t bstrBuffer(szBuffer);
								m_pFormatObj->AddFieldExt(bstrBuffer, vLen.bstrVal, nType, lStartPos, lEndPos, lPrec, lScale, &bSuccess);
							}
							spRecordset->MoveNext();
						}

						long lFieldCnt = 0;
						m_pFormatObj->get_FieldCount(&lFieldCnt);
						if (iColCount != 0 && lFieldCnt == iColCount )
							bSuccess = VARIANT_TRUE; 
						else
							bSuccess = VARIANT_FALSE;
					} //while
					else
						m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::LoadFormat() Table does not exist.");
				}
			}
			objADO.CloseConnection();

			if (!bSuccess)
				m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::LoadFormat() -- Field collection was not generated.");
		}
		else
			m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::LoadFormat() Database connection could not be opened.");
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeSQL::LoadFormat() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::LoadFormat() catch statement.");
		bSuccess = VARIANT_FALSE;
	}

	*pRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CdeSQL::FieldResize(RESIZEOPTION option, VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	HRESULT hrRet = E_FAIL;
	_bstr_t bstrSubErr = L"";
	m_listErrors.clear();

	try {
		
		long lFldCount = 0;
		m_pFormatObj->get_FieldCount(&lFldCount);

		if (lFldCount == 0) 
			bstrSubErr = L"Field collection does not exist.  Resizing is not possible.";
		else
			bSuccess = VARIANT_TRUE;

/*		if (lFldCount == 0)  //regenerate field collection if not there
			LoadFormat(&bSuccess);
		else 
			bSuccess = VARIANT_TRUE;
*/
		if (bSuccess)
		{
			CADO objADO(m_bstrServer, L"Utilities");
			if ( SUCCEEDED(objADO.OpenTrustedConnection()) ) 
			{
				ADODB::_RecordsetPtr spRecordset;
				_bstr_t bQry = L"usp_GetDataLength '";
				bQry += m_bstrDatabase;
				bQry += ".dbo.";
				bQry += m_bstrTable;
				bQry += "'";

				_variant_t vFieldCnt;
				vFieldCnt.vt = VT_I2;
				vFieldCnt.iVal = 0;
				_variant_t varIndex;
				varIndex.vt = VT_I2;
				hrRet = objADO.GetRecordSet((IDispatch**) &spRecordset, bQry);
				if (spRecordset != NULL)
				{
					int iRecCount = spRecordset->GetRecordCount();
					long lFieldCollCnt = 0;
					m_pFormatObj->get_FieldCount(&lFieldCollCnt);
					int iRecFieldCnt = spRecordset->Fields->Count;
					if (iRecCount > 0 && lFieldCollCnt == iRecFieldCnt)
					{
						spRecordset->MoveFirst();
						IFieldInfo* pField = NULL;
						m_pFormatObj->First(&pField); 
						while (vFieldCnt.iVal < iRecFieldCnt)
						{
							_variant_t vValue = spRecordset->Fields->Item[vFieldCnt]->Value;
												
							if (pField != NULL) 
							{
								BSTR bTemp;
								pField->get_Length(&bTemp);
								_bstr_t bSize = bTemp;
								SysFreeString(bTemp);

								if (vValue.vt == VT_NULL) {
									vValue.vt = VT_I2;
									vValue.iVal = 1;
								}

								if (vValue.iVal < 1 ) {
									bSuccess = VARIANT_FALSE;
									bstrSubErr = L"DTRAN ERROR: Method FieldResize() -- A field length returned from usp_GetDataLength was less than 1.";
									break;
								}

								if (option == vCORRECT) 
								{
									//only increase size
									_variant_t vOrig(bSize);
									vOrig.ChangeType(VT_I2);
									if (vValue.iVal > vOrig.iVal) {
										vValue.ChangeType(VT_BSTR);
										pField->put_Length(vValue.bstrVal);
									}
								} 
								else  
								{  //use optimal values
									vValue.ChangeType(VT_BSTR);
									pField->put_Length(vValue.bstrVal);
								}
								pField->Release();
								pField = NULL;
								vFieldCnt.iVal++;
							}
							m_pFormatObj->Next(&pField);
						}
					}
					else {
						bSuccess = VARIANT_FALSE;
						bstrSubErr = L"Query field count does not equal field collection count.";
					}

					if (vFieldCnt.iVal != lFieldCollCnt) {
						bSuccess = VARIANT_FALSE;
						bstrSubErr += L"Not all fields were in field collection were processed.";
					}
				}
				else {
					bSuccess = VARIANT_FALSE;
					bstrSubErr = L"Record set pointer is NULL.";
				}
				objADO.CloseConnection();
			}
			else {
				bSuccess = VARIANT_FALSE;
				bstrSubErr = L"Can not connection to database: Utilities on server: ";
				bstrSubErr += m_bstrServer;
		
			}
		}

		if (!bSuccess) {
			_bstr_t bstrErrMsgs = L"DTRAN ERROR: CdeSQL::FieldResize() encountered a problem. ";
			bstrErrMsgs += bstrSubErr;
			m_listErrors.push_back(bstrErrMsgs);
		}
		else {
			m_pFormatObj->ReCalcLengths(&bSuccess);
			if (!bSuccess )
				m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::FieldResize() encountered a problem when trying to recalculate lengths.");
		}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeSQL::ResizeField() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::FieldResize() catch statement.");
		bSuccess = VARIANT_FALSE;

	}

	*pRetVal = bSuccess;
	return S_OK;
}


STDMETHODIMP CdeSQL::get_ExportQuery(BSTR *pVal)
{
	*pVal = m_bstrExportQuery.copy(); 
	return S_OK;
}


STDMETHODIMP CdeSQL::put_ExportQuery(BSTR newVal)
{
	m_listErrors.clear();

	if (m_bDropTable) {
		
		if (wcscmp(m_bstrServer, L"")	!= 0	&&
			wcscmp(m_bstrDatabase, L"") != 0	&&
			wcscmp(m_bstrTable, L"")	!= 0		)  
		{
			CADO objADO(L"", L"");
			if (SUCCEEDED(objADO.DropTable(m_bstrServer, m_bstrDatabase, m_bstrTable)))
				m_bDropTable = false;
			else
				Utilities::TransferErrors(objADO.m_listErrors, m_listErrors);
		}
	}

	m_bstrTable = L"";		//these two should never both be set
	m_pFormatObj->Clear();

	m_bstrExportQuery = newVal;
	return S_OK;
}

STDMETHODIMP CdeSQL::get_Owner(BSTR *pVal)
{
	*pVal = m_bstrOwner.copy();

	return S_OK;
}

STDMETHODIMP CdeSQL::put_Owner(BSTR newVal)
{
	m_bstrOwner = newVal;

	return S_OK;
}

STDMETHODIMP CdeSQL::get_ErrMsgList(BSTR *pVal)
{
	_bstr_t bstrErrors = L"";
	try {

		bstrErrors = Utilities::GetErrorList(m_listErrors, true);
	}
		catch(_com_error &e) {
		bstrErrors = Utilities::ConstructErrMsg(L"COM ERROR: CdeSQL::get_ErrMsgList() catch statement.", e.Source(), e.Description(), e.ErrorMessage());
	}
	catch (...) {
		bstrErrors = L"DTRAN ERROR: CdeSQL::get_ErrMsgList() catch statement.";
	}

	*pVal = bstrErrors.copy();
	return S_OK;
}

STDMETHODIMP CdeSQL::put_ErrMsgList(BSTR newVal)
{
		return S_OK;

}

STDMETHODIMP CdeSQL::Query(BSTR bstrQuery, IDispatch* *pRecSet)
{
	TCHAR szError[MAX_ERR_LEN];
	memset(szError, 0, MAX_ERR_LEN);
	m_listErrors.clear();
	*pRecSet = NULL;

	try {

		CADO objADO(m_bstrServer, m_bstrDatabase);
		if ( SUCCEEDED(objADO.OpenTrustedConnection()) ) {

			ADODB::_RecordsetPtr spRecordset = NULL;
			HRESULT hr = E_FAIL;
			hr = objADO.GetRecordSet((IDispatch**) &spRecordset, bstrQuery);
	
			if (SUCCEEDED(hr) ) {
				if ( spRecordset != NULL) {
					int iRecCnt = spRecordset->RecordCount;
					*pRecSet = IDispatchPtr(spRecordset).Detach();
					//*pp_Recordset= IDispatchPtr(spRecordset).Detach();
				}
			}
			else {
				Utilities::TransferErrors(objADO.m_listErrors, m_listErrors);
				objADO.CloseConnection();
			}
		}
		else  {
				TCHAR szServer[BUFF_MED]; memset(szServer, 0, BUFF_MED);
				int iStrLen =  SysStringLen(m_bstrServer);
				WideCharToMultiByte(CP_ACP, 0, m_bstrServer, iStrLen, szServer, iStrLen+1, NULL, NULL);
				szServer[iStrLen]= '\0';

				TCHAR szDatabase[BUFF_MED]; memset(szDatabase, 0, BUFF_MED);
				iStrLen =  SysStringLen(m_bstrDatabase);
				WideCharToMultiByte(CP_ACP, 0, m_bstrDatabase, iStrLen, szDatabase, iStrLen+1, NULL, NULL);
				szDatabase[iStrLen]= '\0';

				_stprintf(szError, _T("DTRAN ERROR: A connection could not be made on server: %s with database: %s"), szServer, szDatabase);
				m_listErrors.push_back(szError);
		}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeSQL::ExecuteQuery() - caught by catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::ExecuteQuery() - caught by catch statement.");
	}

	return S_OK;
}

STDMETHODIMP CdeSQL::AddDatabase(BSTR DataBase, BSTR Path, BSTR DatSize, BSTR LogSize, BSTR DatGrowth, BSTR LogGrowth, BSTR DatMaxSize, BSTR LogMaxSize, VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	_bstr_t bstrDataBase = DataBase;
	_bstr_t bstrPathIn = Path;
	_bstr_t bstrPath = L"";
	_bstr_t bstrDatSize = DatSize;
	_bstr_t bstrLogSize = LogSize;
	_bstr_t bstrDatGrowth = DatGrowth;
	_bstr_t bstrLogGrowth = LogGrowth;
	_bstr_t bstrDatMaxSize = DatMaxSize;
	_bstr_t bstrLogMaxSize = LogMaxSize;

	TCHAR szPath[MAX_FILE_LEN]; memset(szPath, 0, MAX_FILE_LEN);
	TCHAR szPath1[MAX_FILE_LEN]; memset(szPath1, 0, MAX_FILE_LEN);

	TCHAR szError[MAX_ERR_LEN];
	memset(szError, 0, MAX_ERR_LEN);
	m_listErrors.clear();

	USES_CONVERSION;

	try {

		TCHAR szCurDir[512];
		GetCurrentDirectory(512, szCurDir);

		if ( wcscmp(bstrDataBase, L"") == 0 )
			m_listErrors.push_back(L"DTRAN ERROR: Data base name must be suppplied.");
		else if ( wcscmp(m_bstrServer, L"") == 0 )
			m_listErrors.push_back(L"DTRAN ERROR: Server name must be suppplied.");
		else {
			DatabaseExists(m_bstrServer, bstrDataBase, &bSuccess);
			if (bSuccess) 
				m_listErrors.push_back(L"DTRAN ERROR: Data base already exists. Can not create.");
		}

		TCHAR szPathIn[BUFF_MED]; memset(szPathIn, 0, BUFF_MED);
		int iStrLen =  SysStringLen(bstrPathIn);
		WideCharToMultiByte(CP_ACP, 0, bstrPathIn, iStrLen, szPathIn, iStrLen+1, NULL, NULL);
		szPathIn[iStrLen]= '\0';

		Utilities::RemoveLastSlash(szPathIn, szPath);
		_bstr_t bstrTemp = Utilities::ConstructFullPath(szCurDir, szPath);
		memset(szPath, 0, MAX_FILE_LEN);

		TCHAR szTemp[BUFF_MED]; memset(szTemp, 0, BUFF_MED);
		iStrLen =  SysStringLen(bstrTemp);
		WideCharToMultiByte(CP_ACP, 0, bstrTemp, iStrLen, szTemp, iStrLen+1, NULL, NULL);
		szPathIn[iStrLen]= '\0';

		Utilities::RemoveLastSlash(szTemp, szPath);
		Utilities::ReplaceChar(szPath, szPath1, '/', '\\');
		

		if ( wcscmp(bstrDatSize, L"") == 0 )
			m_listErrors.push_back(L"DTRAN ERROR: DatFileSize name must be suppplied.");

		if ( wcscmp(bstrLogSize, L"") == 0 )
			m_listErrors.push_back(L"DTRAN ERROR: rLogFileSize name must be suppplied.");

		if ( wcscmp(bstrDatGrowth, L"") == 0 )
			m_listErrors.push_back(L"DTRAN ERROR: DatFileMaxSize name must be suppplied.");

		if ( wcscmp(bstrDatMaxSize, L"") == 0 )
			m_listErrors.push_back(L"DTRAN ERROR: DatFileMaxSize name must be suppplied.");

		if ( wcscmp(bstrLogMaxSize, L"") == 0 )
			m_listErrors.push_back(L"DTRAN ERROR: LogFileMaxSize name must be suppplied.");

		if ( wcscmp(bstrDatGrowth, L"") == 0 )
			m_listErrors.push_back(L"DTRAN ERROR: DatFileGrowth name must be suppplied.");

		if ( wcscmp(bstrLogGrowth, L"") == 0 )
			m_listErrors.push_back(L"DTRAN ERROR: m_bstrLogFileGrowth name must be suppplied.");

		if ( m_listErrors.size() == 0 ) {

			CADO objADO(m_bstrServer, L"master");
			if ( SUCCEEDED(objADO.OpenTrustedConnection()) ) {

				HRESULT hr = E_FAIL;
				bSuccess = VARIANT_FALSE;

				TCHAR szQuery[MAX_QUERY_LEN];  memset(szQuery, 0, MAX_QUERY_LEN);
				_stprintf(szQuery, "CREATE DATABASE %s ON ( NAME = %s_dat, FILENAME = '%s\\%sdat.mdf', SIZE = %s, MAXSIZE = %s, FILEGROWTH = %s ) LOG ON ( NAME = '%s_log', FILENAME = '%s\\%slog.ldf', SIZE = %s, MAXSIZE = %s, FILEGROWTH = %s )", 
							W2A(bstrDataBase), W2A(bstrDataBase), szPath1, W2A(bstrDataBase), W2A(bstrDatSize), W2A(bstrDatMaxSize), W2A(bstrDatGrowth),
												  W2A(bstrDataBase), szPath1, W2A(bstrDataBase), W2A(bstrLogSize), W2A(bstrLogMaxSize), W2A(bstrLogGrowth) );
			
/*			FILE* fpLog;
				fpLog = fopen("c:\\createsql.txt", "a");
				if (fpLog) {
					fprintf(fpLog, szQuery);
					fflush(fpLog);
					fclose(fpLog);
				}
*/
				hr = objADO.ExecuteQuery(szQuery);
				if (SUCCEEDED(hr) ) 
					bSuccess = VARIANT_TRUE;
				else 
					Utilities::TransferErrors(objADO.m_listErrors, m_listErrors);

				objADO.CloseConnection();
			}
			else {
				_stprintf(szError, _T("DTRAN ERROR: A connection could not be made on server: %s with the master database to create data base: %s"), W2A(m_bstrServer), W2A(bstrDataBase));
				m_listErrors.push_back(szError);
			}
		}
		else
			bSuccess = VARIANT_FALSE;
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeSQL::AddDatabase - caught by catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::AddDatabase - caught by catch statement.");
	}
	*pRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CdeSQL::DatabaseExists(BSTR Server, BSTR DataBase, VARIANT_BOOL *pRetVal)
{
	
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	_bstr_t bstrDataBase = DataBase;
	_bstr_t bstrServer = Server;

	TCHAR szError[MAX_ERR_LEN];
	memset(szError, 0, MAX_ERR_LEN);
	m_listErrors.clear();

	TCHAR szDatabase[BUFF_MED]; memset(szDatabase, 0, BUFF_MED);
	int iStrLen =  SysStringLen(bstrDataBase);
	WideCharToMultiByte(CP_ACP, 0, bstrDataBase, iStrLen, szDatabase, iStrLen+1, NULL, NULL);
	szDatabase[iStrLen]= '\0';

	try { 

		if ( wcscmp(bstrDataBase, L"") == 0 )
			m_listErrors.push_back(L"DTRAN ERROR: Database name must be suppplied.");

		if ( wcscmp(bstrServer, L"") == 0 )
			m_listErrors.push_back(L"DTRAN ERROR: bstrServer name must be suppplied.");

		if ( m_listErrors.size() == 0 ) {

			CADO objADO(bstrServer, L"master");
			if ( SUCCEEDED(objADO.OpenTrustedConnection()) ) {

				HRESULT hr = E_FAIL;
				TCHAR szQuery[MAX_QUERY_LEN];
				memset(szQuery, 0, MAX_QUERY_LEN);
			
				ADODB::_RecordsetPtr spRecordset = NULL;
				_stprintf(szQuery, "select * from master..sysdatabases where name = '%s'", szDatabase );
				
				_bstr_t bstrQuery(szQuery);
				hr = objADO.GetRecordSet((IDispatch**) &spRecordset, bstrQuery);
				if (SUCCEEDED(hr) ) {

					if ( spRecordset != NULL) {
						int iRecCnt = spRecordset->RecordCount;
						if (iRecCnt != 0 )
							bSuccess = VARIANT_TRUE;
					}
				}
				else 
					Utilities::TransferErrors(objADO.m_listErrors, m_listErrors);

				objADO.CloseConnection();
			}
			else {
				TCHAR szServer[BUFF_MED]; memset(szServer, 0, BUFF_MED);
				int iStrLen =  SysStringLen(bstrServer);
				WideCharToMultiByte(CP_ACP, 0, bstrServer, iStrLen, szServer, iStrLen+1, NULL, NULL);
				szServer[iStrLen]= '\0';
				_stprintf(szError, _T("DTRAN ERROR: A connection could not be made on server: %s with the master data base to test data base: %s existance."), szServer, szDatabase);
				m_listErrors.push_back(szError);
			}
		}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeSQL::DatabaseExists - caught by catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::DatabaseExists - caught by catch statement.");
	}

	*pRetVal = bSuccess;
	return S_OK;
}



STDMETHODIMP CdeSQL::Execute(BSTR Query, VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	TCHAR szError[MAX_ERR_LEN];
	memset(szError, 0, MAX_ERR_LEN);
	m_listErrors.clear();

	try {

		CADO objADO(m_bstrServer, m_bstrDatabase);
		if ( SUCCEEDED(objADO.OpenTrustedConnection()) ) {

			HRESULT hr = E_FAIL;
			_bstr_t bstrQuery = Query;
			hr = objADO.ExecuteQuery(bstrQuery);
	
			if (!SUCCEEDED(hr) ){
				Utilities::TransferErrors(objADO.m_listErrors, m_listErrors);
			}
			else
				bSuccess = VARIANT_TRUE;

			objADO.CloseConnection();

		}
		else  {
				TCHAR szDatabase[BUFF_MED]; memset(szDatabase, 0, BUFF_MED);
				int iStrLen =  SysStringLen(m_bstrDatabase);
				WideCharToMultiByte(CP_ACP, 0, m_bstrDatabase, iStrLen, szDatabase, iStrLen+1, NULL, NULL);
				szDatabase[iStrLen]= '\0';

				TCHAR szServer[BUFF_MED]; memset(szServer, 0, BUFF_MED);
				iStrLen =  SysStringLen(m_bstrServer);
				WideCharToMultiByte(CP_ACP, 0, m_bstrServer, iStrLen, szServer, iStrLen+1, NULL, NULL);
				szServer[iStrLen]= '\0';

				_stprintf(szError, _T("DTRAN ERROR: A connection could not be made on server: %s with database: %s"), szServer, szDatabase);
				m_listErrors.push_back(szError);
		}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeSQL::ExecuteQuery() - caught by catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::ExecuteQuery() - caught by catch statement.");
	}

	*pRetVal = bSuccess;

	return S_OK;
}

STDMETHODIMP CdeSQL::CreateTable(/*[out, retval]*/ VARIANT_BOOL* pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	TCHAR szError[MAX_ERR_LEN];
	memset(szError, 0, MAX_ERR_LEN);
	m_listErrors.clear();

	try {
		if (wcscmp(m_bstrServer, L"")	!= 0	&&
			wcscmp(m_bstrDatabase, L"") != 0	&&
			wcscmp(m_bstrTable, L"")	!= 0		)  
		{
			CADO objADO(m_bstrServer, m_bstrDatabase);
			if ( SUCCEEDED(objADO.OpenTrustedConnection(m_bstrServer, m_bstrDatabase)) )
			{
				if ( !objADO.TableExists(m_bstrServer, m_bstrDatabase, m_bstrTable) )
				{
					long lFieldCnt = 0;
					m_pFormatObj->get_FieldCount(&lFieldCnt);
					if (lFieldCnt > 0 )
					{
						BSTR bTemp;
						GetCreateTableSQL(&bTemp);
						_bstr_t bstrSQLQry = bTemp;
						SysFreeString(bTemp);

						TCHAR szTable[BUFF_MED]; memset(szTable, 0, BUFF_MED);
						int iStrLen =  SysStringLen(m_bstrTable);
						WideCharToMultiByte(CP_ACP, 0, m_bstrTable, iStrLen, szTable, iStrLen+1, NULL, NULL);
						szTable[iStrLen]= '\0';
					
						HRESULT hr  = objADO.CreateTableFromQuery(bstrSQLQry, szTable);
						if (SUCCEEDED(objADO.ExecuteQuery(bstrSQLQry)))
							bSuccess = VARIANT_TRUE;
						else 
							Utilities::TransferErrors(objADO.m_listErrors, m_listErrors);
					}
					else
						m_listErrors.push_back( L"DTRAN ERROR:  CdeSQL::CreateTable() -- Can not create table, field collection is empty.");
				}
				else 
					m_listErrors.push_back( L"DTRAN ERROR:  CdeSQL::CreateTable() -- Can not create table, already exists.");
			}
			else 
				m_listErrors.push_back( L"DTRAN ERROR:  CdeSQL::CreateTable() -- Can not create table, database connection can not be established.");

		}
		else 				
			m_listErrors.push_back( L"DTRAN ERROR: CdeSQL::CreateTable() - Can not create table, pertinent information either server, database or table name is missing.");
	}
	catch(_com_error &e) {
			m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CdeSQL::CreateTable() - caught by catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CdeSQL::CreateTable() - caught by catch statement.");
	}

	*pRetVal = bSuccess;
	return S_OK;

}

STDMETHODIMP CdeSQL::get_ImportBatchSize(long *pVal)
{
	*pVal = m_lBatchSize;

	return S_OK;
}

STDMETHODIMP CdeSQL::put_ImportBatchSize(long newVal)
{
	m_lBatchSize = newVal;

	return S_OK;
}

STDMETHODIMP CdeSQL::get_Err_MaxMessages(long *pVal)
{
    *pVal = m_lMaxErrMsgs;
	return S_OK;
}

STDMETHODIMP CdeSQL::put_Err_MaxMessages(long newVal)
{
	m_lMaxErrMsgs = newVal; 
	return S_OK;
}

STDMETHODIMP CdeSQL::get_Err_NoInfo(VARIANT_BOOL *pVal)
{
	*pVal = m_bNoInfo;
	return S_OK;
}

STDMETHODIMP CdeSQL::put_Err_NoInfo(VARIANT_BOOL newVal)
{
	m_bNoInfo = newVal;
	return S_OK;
}
