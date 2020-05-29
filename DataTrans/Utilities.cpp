// Utilities.cpp: implementation of the Utilities class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Utilities.h"

#if !defined
	#include <stdio.h>
	#include <time.h>
	#include <io.h>
#endif

extern TCHAR sz_CreateTableFromQuery[] =	_T("Select * into %s from ( %s ) Alias1");
extern TCHAR sz_SchemaForColumnSQL[] =		_T("SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = '%s' and COLUMN_NAME = '%s'");
extern TCHAR sz_InfoSchemaOnTable[] =			_T("SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = '%s' order by ORDINAL_POSITION");
extern TCHAR sz_ExtraFieldName[] =          _T("TQ_787");
extern TCHAR sz_VariedQuery[] =				_T("SELECT *, TQ_787 = '\r\n' from %s.%s.%s");
extern TCHAR sz_SQLSelectStmt[] =			_T("Select * from %s");
extern TCHAR sz_SQLSelectStmt2[] =			_T("Select * from %s.%s");
extern TCHAR sz_SQLSelectStmt3[] =			_T("Select * from [%s].[%s].[%s]");
extern TCHAR sz_SQLSelectStmt4[] =			_T("Select * from %s where 1 = 0");
extern TCHAR sz_DBase[] =					_T("DBase");
extern TCHAR sz_FoxPro[] =					_T("FoxPro");
extern TCHAR szDBaseConnStr[] =				_T("Provider=Microsoft.Jet.OLEDB.4.0; Data Source = %s; Extended Properties=DBASE 5.0;");
extern TCHAR szFoxProConnStr[] =			_T("Provider=MSDASQL;DSN=%s");
extern TCHAR szAccessConnStr[] =			_T("Provider=Microsoft.Jet.OLEDB.4.0; Data Source = %s");
extern TCHAR szSQLConnStr1[] =				_T("Provider=SQLOLEDB; Integrated Security='SSPI'");
extern TCHAR szLog_RowsCopied[] =			_T("Total Number of rows bulk copied in = %d.");
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Utilities::Utilities()
{

}

Utilities::~Utilities()
{

}

int GetRandomNumber()
{
	int sHour, sMin, sSec;
	time_t ltime;
	struct tm* today;  
	time (&ltime);   //get system time
	today = localtime (&ltime);
	sHour = today->tm_hour;
	sMin = today->tm_min;
	sSec = today->tm_sec;

	srand((unsigned) sSec*sMin);
	return rand();
}

LPTSTR Utilities::ConstructFileName(LPTSTR pzBase, LPTSTR pzExt)
{
	TCHAR szBufDir[MAX_FILE_LEN];
	GetCurrentDirectory(MAX_FILE_LEN, szBufDir);
		
	TCHAR unique[20];
	itoa(GetRandomNumber(), unique, 10);
	unique[strlen(unique)+1] = '\0';

	TCHAR szTemp[MAX_FILE_LEN]; // = new TCHAR[_tcslen(szBufDir) + _tcslen(pzBase) + _tcslen(unique) + _tcslen(pzExt) + 1];
	memset(szTemp, 0, MAX_FILE_LEN);

	_stprintf(szTemp, "%s\\%s%s.%s", szBufDir, pzBase, unique, pzExt);
	szTemp[_tcslen(szTemp)+1] = '\0';

	
	TCHAR szTemp2[MAX_FILE_LEN]; // = new TCHAR[_tcslen(szBufDir) + _tcslen(pzBase) + _tcslen(unique) + _tcslen(pzExt) + 1];
	memset(szTemp2, 0, MAX_FILE_LEN);

	return _tcscpy(szTemp2, szTemp);
}

LPTSTR Utilities::ConstructUniqueName(LPTSTR pzBase)
{
	TCHAR unique[20];
	itoa(GetRandomNumber(), unique, 10);
	unique[strlen(unique)+1] = '\0';

	time_t ltime;
	struct tm* today;  
	time (&ltime);   //get system time
	today = localtime (&ltime);

	TCHAR szTemp[MAX_FILE_LEN]; // = new TCHAR[_tcslen(szBufDir) + _tcslen(pzBase) + _tcslen(unique) + _tcslen(pzExt) + 1];
	memset(szTemp, 0, MAX_FILE_LEN);
	::Sleep(1000);  //sleep for 1 sec here, a problem has occurred where multiple scripts running will actually contruct the same table name, so sleep just a bit
	_sntprintf(szTemp,MAX_FILE_LEN, "%s%d_%d_%d_%d", pzBase, today->tm_mday, today->tm_hour, today->tm_min, today->tm_sec);
	szTemp[_tcslen(szTemp)+1] = '\0';

	TCHAR szTemp2[MAX_FILE_LEN]; // = new TCHAR[_tcslen(szBufDir) + _tcslen(pzBase) + _tcslen(unique) + _tcslen(pzExt) + 1];
	memset(szTemp2, 0, MAX_FILE_LEN);

	return 	_tcscpy(szTemp2, szTemp);
}


int Utilities::ConstructFileName(LPTSTR pzBase, LPTSTR pzExt, LPTSTR pzFileName)
{
	
	TCHAR szBufDir[512];
	GetCurrentDirectory(512, szBufDir);
	
	TCHAR unique[20];
	itoa(GetRandomNumber(), unique, 10);
	unique[strlen(unique)+1] = '\0';

	TCHAR szTemp[MAX_FILE_LEN]; // = new TCHAR[_tcslen(szBufDir) + _tcslen(pzBase) + _tcslen(unique) + _tcslen(pzExt) + 1];
	memset(szTemp, 0, MAX_FILE_LEN);

	_stprintf(szTemp, "%s\\%s%s.%s", szBufDir, pzBase, unique, pzExt);
	szTemp[_tcslen(szTemp)+1] = '\0';
	_tcscpy(pzFileName, szTemp);

	return _tcslen(pzFileName);

}

int Utilities::BreakupFileName(_bstr_t bstrFileName, LPTSTR pzBase, LPTSTR pzExt)
{
	int iFileCnt = 0;
	bool bFound = false;

	TCHAR szDirCurr[512];
	GetCurrentDirectory(512, szDirCurr);

	TCHAR szFileName[BUFF_MED]; memset(szFileName, 0, BUFF_MED);
	int iStrLen =  SysStringLen(bstrFileName);
	WideCharToMultiByte(CP_ACP, 0, bstrFileName, iStrLen, szFileName, iStrLen+1, NULL, NULL);
	szFileName[iStrLen]= '\0';
	LPTSTR lpFileOrig = szFileName;
	int iLen = strlen(lpFileOrig);
//	*(lpFileOrig + iLen) = '\0';

	TCHAR Name[MAX_FILE_LEN];
	memset(Name, 0, MAX_FILE_LEN);
	TCHAR Ext[BUFF_LEN];
	memset(Ext, 0, BUFF_LEN);

	TCHAR ch1 = _T('.');
	int iIndex1 = 0;

	//search for "." and split
	//get the first half of the file 
	while (*lpFileOrig) {
		
		TCHAR ch = (*lpFileOrig);
		if (ch != ch1)  {
			Name[iIndex1++] = ch;
		    lpFileOrig++;
		}
		else {
			bFound = true;
			break;
		}
	}

	lpFileOrig++;
	iLen = strlen(Name)+1;
	Name[iLen] = '\0';

	_tcscpy(pzBase, Name);

	if (bFound) {
		//get the extension
		iLen = strlen(lpFileOrig);
		for (iIndex1 = 0; iIndex1 < iLen;  iIndex1++)  {
			Ext[iIndex1] = *(lpFileOrig++);
		}
	}

	iLen = strlen(Ext)+1;
	Ext[iLen] = '\0';

	_tcscpy(pzExt, Ext);

	return _tcslen(Name) + _tcslen(Ext);
}

int Utilities::BreakUpFileName(_bstr_t bstrFileName, LPTSTR pzDir, LPTSTR pzBaseName, LPTSTR pzExt)
{
	int iFileCnt = 0;
	bool bFound = false;
	USES_CONVERSION;

	TCHAR szDirCurr[512];
	GetCurrentDirectory(512, szDirCurr);

	LPTSTR lpFileOrig = W2A(bstrFileName);
//	int iLen = strlen(lpFileOrig);
//	*(lpFileOrig + iLen) = '\0';

	TCHAR Dir[MAX_FILE_LEN];
	memset(Dir, 0, MAX_FILE_LEN);
	TCHAR Name[MAX_FILE_LEN];
	memset(Name, 0, MAX_FILE_LEN);
	TCHAR Ext[BUFF_LEN];
	memset(Ext, 0, BUFF_LEN);

	//find when directory path ends
	int iPathLen = _tcslen(lpFileOrig);
	for (iPathLen; iPathLen > 0; iPathLen--) {
		TCHAR ch0 = lpFileOrig[iPathLen];
		if (ch0 == _T('\\') || ch0 == _T('/') )
			break;
	}

	//grab directory path
	for (int i = 0; i < iPathLen; i++) {
		Dir[i] = *(lpFileOrig++);
	}
	int iLen = _tcslen(Dir)+1;
	Dir[iLen] = '\0';
	_tcscpy(pzDir, Dir);
	
	//grab file name
	int j = 0;
	int k = 0;
	bool bGrabExt = false;
	lpFileOrig++;
	while ( *(lpFileOrig) ) {
		TCHAR ch0 = *(lpFileOrig);
		if (ch0 == _T('.')  ) {
			bGrabExt = true;
			lpFileOrig++; //skip dot
		}
		else if (bGrabExt) {
			Ext[j++] = *(lpFileOrig++);
		}
		else  {
			Name[k++] = *(lpFileOrig++);
		}
	}
	iLen = _tcslen(Name)+1;
	Name[iLen] = '\0';
	_tcscpy(pzBaseName, Name);

	iLen = _tcslen(Ext)+1;
	Ext[iLen] = '\0';
	_tcscpy(pzExt, Ext);

	return _tcslen(Dir) + _tcslen(Name) + _tcslen(Ext);
}


bool Utilities::IsSubDirPath(LPTSTR pzPath)
{
	LPTSTR pzTemp = pzPath;
	//is this a path or a dir?
	if (_tcslen(pzPath) > 1 ) {
			
		TCHAR ch1 = (*pzTemp++);
		TCHAR ch2 = (*pzTemp++);
		if ( ch1 == '\\' && ch2 == '\\')  //begins with machine name.
			return false;
	}

	pzTemp = pzPath;
	while (*pzTemp) {
		TCHAR ch = (*pzTemp++);
		if ( ch == '\\' || ch == '/' )  
			return true;
		if (ch == ':')
			return false;
	}


	return false;
}

bool Utilities::IsLastCharSlash(LPTSTR pzPath)
{
	int iLen = _tcslen(pzPath);
	TCHAR ch = *(pzPath+iLen-1);

	if (ch == '\\' || ch == '/')
		return true;

	return false;
}

bool Utilities::IsFirstCharSlash(LPTSTR pzPath)
{
	int iLen = _tcslen(pzPath);
	TCHAR ch = (*pzPath);

	if (ch == '\\' || ch == '/')
		return true;

	return false;
}

int Utilities::RemoveLastSlash(LPTSTR pzIn, LPTSTR pzOut)
{
	//remove last backslash
	TCHAR buf_out[MAX_FILE_LEN];
	memset(buf_out, 0, MAX_FILE_LEN);

	if (_tcslen(pzIn) > 0 ) {
		
		int iLen = _tcslen(pzIn);
		if (IsLastCharSlash(pzIn) ) 
			iLen--;
		for (int ii = 0; ii < iLen; ii++) {
			buf_out[ii] = *(pzIn++);
		}

		buf_out[_tcslen(buf_out)+1] = '\0';
	}

	_tcscpy(pzOut, buf_out);

	return _tcslen(pzOut);
}

bool Utilities::IsFullPath(LPTSTR pzPath)
{
	//check for double backslash or drive colon
	////check for base.ext??

	if (_tcslen(pzPath) > 1 ) {
			
		TCHAR ch1 = (*pzPath++);
		TCHAR ch2 = (*pzPath++);
		TCHAR ch3 = (*pzPath++);

		if ( (ch1 == '\\' && ch2 == '\\') ||(ch1 == '/' && ch2 == '/') || (ch2 == ':' && ch3 == '\\' ) || (ch2 == ':' && ch3 == '/')  )
			return true;
	}

	return false;
}

_bstr_t Utilities::ConstructFullPath(_bstr_t bstrDir, _bstr_t bstrFile)
{
	USES_CONVERSION;
	_bstr_t bstrNew = L"";

	if ( IsFullPath(W2A(bstrFile)) )  {
		bstrNew = bstrFile;
	}
	else {

		if (IsFirstCharSlash(W2A(bstrFile)) )
			bstrNew = bstrDir + bstrFile;
		else {
			
			bstrNew = bstrDir;
			bstrNew += L"\\";
			bstrNew += bstrFile;
		}
	}

	return bstrNew;

}

int Utilities::GetDirectoryPath(LPTSTR pzPathIn, LPTSTR pzPathOut)
{
	//parses out the directory path from a full path name or partial directory path
	if (!IsFullPath(pzPathIn) && !IsSubDirPath(pzPathIn) )
		return 0;

	TCHAR szOrig[MAX_FILE_LEN]; 
	memset(szOrig, 0, MAX_FILE_LEN);
	_tcscpy(szOrig, pzPathIn);

	int iPathLen = _tcslen(szOrig);
	for (iPathLen; iPathLen > 0; iPathLen--) {
		TCHAR ch0 = szOrig[iPathLen];
		if (ch0 == _T('\\') || ch0 == _T('/') )
			break;
	}

	TCHAR strNewPath[MAX_FILE_LEN];
	memset(strNewPath, 0, MAX_FILE_LEN);
	for (int i = 0; i < iPathLen; i++) {
		strNewPath[i] = szOrig[i];
	}
	strNewPath[(_tcslen(strNewPath))] = '\0';

	_tcscpy(pzPathOut, strNewPath);

	return _tcslen(pzPathOut);

}

int Utilities::ChangeFileExtension(LPTSTR pzFileName, LPTSTR pzNewExt, LPTSTR pzNameOut)
{
	int iLen = 0;
	TCHAR szBase[MAX_FILE_LEN];  memset(szBase, 0, MAX_FILE_LEN);
	TCHAR szExt[BUFF_LEN];	memset(szExt, 0, BUFF_LEN);
	
	if ( BreakupFileName(pzFileName, szBase, szExt) > 0 ) {

		TCHAR szNewFile[MAX_FILE_LEN]; memset(szNewFile, 0, MAX_FILE_LEN);
		_tcscpy(szNewFile, szBase);
		_tcscat(szNewFile, _T("."));
		_tcscat(szNewFile, pzNewExt);
		szNewFile[_tcslen(szNewFile)] = '\0';
	
		_tcscpy(pzNameOut, szNewFile);
		iLen = _tcslen(pzNameOut);
	}
	return iLen;

}

int Utilities::AddExtension(LPTSTR pzFileName, LPTSTR pzNewExt, LPTSTR pzNameOut)
{
	//if no extension then add ".fmt"
	int iLen = 0;
	TCHAR szBase[MAX_FILE_LEN];  memset(szBase, 0, MAX_FILE_LEN);
	TCHAR szExt[BUFF_LEN];	memset(szExt, 0, BUFF_LEN);
	
	if ( BreakupFileName(pzFileName, szBase, szExt) > 0 ) 
	{
		if (_tcslen(szExt) != 0 )
			_tcscpy(pzNameOut, pzFileName); //has extension so keep it
		else 
		{
			TCHAR szNewFile[MAX_FILE_LEN]; memset(szNewFile, 0, MAX_FILE_LEN);
			_tcscpy(szNewFile, szBase);
			_tcscat(szNewFile, _T("."));
			_tcscat(szNewFile, pzNewExt);
			szNewFile[_tcslen(szNewFile)] = '\0';
	
			_tcscpy(pzNameOut, szNewFile);
		}
		iLen = _tcslen(pzNameOut);
	}
	return iLen;
}


int Utilities::RemoveChar(LPTSTR pzStrIn, TCHAR ch, LPTSTR pzOut)
{
	int j = 0;
	TCHAR szStrNew[MAX_FILE_LEN]; memset(szStrNew, 0, MAX_FILE_LEN);
	while (*pzStrIn) {
		if (*pzStrIn != ch)
			szStrNew[j++] = *pzStrIn;
		pzStrIn++;
	}
	szStrNew[_tcslen(szStrNew)+1] = '\0';
	_tcscpy(pzOut, szStrNew);
	return _tcslen(pzOut);
}

int Utilities::ReplaceChar(LPTSTR pzStrIn, LPTSTR pzStrOut, TCHAR ch1, TCHAR ch2)
{
	int iLen = 0;
	TCHAR szStrNew[MAX_FILE_LEN]; memset(szStrNew, 0, MAX_FILE_LEN);

	int iStrLen = _tcslen(pzStrIn);
	for (int i = 0; i < iStrLen; i++ ) {

		if (*pzStrIn == ch1)
			szStrNew[i] = ch2;
		else
			szStrNew[i] = *(pzStrIn);
		pzStrIn++;
	}

	szStrNew[_tcslen(szStrNew)] = '\0';
	_tcscpy(pzStrOut, szStrNew);

	return _tcslen(pzStrOut);
}

int Utilities::ParseOutFileName(LPTSTR pzPath, LPTSTR pzFile)
{
	//handles cases: full paths, partial paths and plain file names

	int iLen = _tcslen(pzPath);
	int iLast = 0;
	for (int i =0; i < iLen; i++) {
		TCHAR ch1 = '\\';
		TCHAR ch2 = '/';
		if (pzPath[i] == ch1 || pzPath[i] == ch2)
			iLast = i;
	}

	int iStart = 0;
	if (iLast > 0 ) iStart = iLast + 1;

	int j = 0;
	TCHAR szOut[MAX_FILE_LEN]; memset(szOut, 0, MAX_FILE_LEN);
	for (i = iStart; i < iLen; i++) {
		szOut[j++] = pzPath[i];
	}

	szOut[_tcslen(szOut)] = '\0';
	_tcscpy(pzFile, szOut);

	return _tcslen(pzFile);

}

bool Utilities::FindSegment(LPSTR ptrStrOrig, LPSTR ptrSeg)
{
	//finds a segment in a string...convert to lower case first...
	bool bFound = false;
	TCHAR buffer1[BUFF_LARGE];
	TCHAR buffer2[BUFF_LARGE];

	_tcscpy(buffer1, ptrStrOrig);
	_tcscpy(buffer2, ptrSeg);

	LPSTR ptrStr = buffer1;
	LPSTR ptrTestSeg = buffer2;
	ptrStr = _tcslwr(ptrStr);
	ptrTestSeg = _tcslwr(ptrTestSeg);

	while ((*ptrStr) && (*ptrTestSeg) && !bFound ) {
		if ( (*ptrStr++) == (*ptrTestSeg++) ) {
			bool bContinue = true;
			while ( (*ptrTestSeg) && (*ptrStr) && bContinue) {
				 
				if( (*ptrStr++) == (*ptrTestSeg++) )
					;
				else
					bContinue = false;
			}
			if ( !(*ptrTestSeg))
				bFound = true;
		}
		ptrTestSeg = buffer2;
	}

	return bFound;
}

int Utilities::TrimLast(LPTSTR pzBufferIn, LPTSTR pzBufferOut)
{
	if (pzBufferIn == NULL) return 0;
	int iBufLen = _tcslen(pzBufferIn);
	TCHAR* pzTemp = new TCHAR[iBufLen+1];
	memset(pzTemp, 0,  iBufLen+1);
	bool bCharFound = false;
	int iSpaceCnt = 0;
	int i = 1;
	while (i < iBufLen) {
		if ( isspace(*(pzBufferIn+(iBufLen-i++)) ) )
			iSpaceCnt++;
		else 
			break;
	}

	i = 0;
	int iNewLen = iBufLen - iSpaceCnt;
	while (i < iNewLen) {
		*(pzTemp+i) = *(pzBufferIn++);
		i++;
	}

	*(pzTemp+(_tcslen(pzTemp))) = '\0';
	_tcscpy(pzBufferOut, pzTemp);
	if (pzTemp)
		delete [] pzTemp;
	
	return _tcslen(pzBufferOut);
}

int Utilities::TrimFirst(LPTSTR pzBufferIn, LPTSTR pzBufferOut)
{
	if (pzBufferIn == NULL) return 0;
	TCHAR* pzTemp = new TCHAR[_tcslen(pzBufferIn)+1];
	memset(pzTemp, 0,  _tcslen(pzBufferIn)+1);
	bool bCharFound = false;
	int i = 0;

	while( *pzBufferIn ) {
		if (isspace(*pzBufferIn) && !bCharFound)
			(pzBufferIn++);
		else {
			bCharFound = true;
			*(pzTemp+i) = *(pzBufferIn++);
			i++;
		}
	}
	*(pzTemp+(_tcslen(pzTemp))) = '\0';
	_tcscpy(pzBufferOut, pzTemp);
	if (pzTemp) 
		delete [] pzTemp;
	
	return _tcslen(pzBufferOut);
}


LPTSTR Utilities::ConstructInfoSchemaSQL(LPTSTR pzTableName, LPTSTR pzOwnerName)
{
	TCHAR szTemp[MAX_QUERY_LEN]; 
	memset(szTemp, 0, MAX_QUERY_LEN);
	
	LPTSTR pzNameIn = pzTableName;
	if ((*pzNameIn) == '#' )
		_stprintf(szTemp, "select * from tempdb.information_schema.columns where table_name = '%s' and table_schema = '%s'", pzTableName, pzOwnerName);
	else
		_stprintf(szTemp, "select * from information_schema.columns where table_name = '%s' and table_schema = '%s'", pzTableName, pzOwnerName);

	szTemp[_tcslen(szTemp)+1] = '\0';

	TCHAR szTemp2[MAX_QUERY_LEN]; 
	memset(szTemp2, 0, MAX_QUERY_LEN);

	return _tcscpy(szTemp2, szTemp);

}

bool Utilities::IsNullOrEmpty(_variant_t vTemp) 
{
	if (vTemp.vt == VT_NULL || vTemp.vt == VT_EMPTY)
		return true;
	else
		return false;
}

_bstr_t Utilities::ConstructErrMsg(_bstr_t bstrText, _bstr_t bstrSource, _bstr_t bstrDesc, _bstr_t bstrMsg)
{
	_bstr_t bstrError = L"";
	bstrError += bstrText;
	bstrError += L" SOURCE: ";
	if (wcscmp(bstrSource, L"") == 0 )
		bstrSource = L"None provided.";
	bstrError += bstrSource;
	bstrError += L" DESCRIPTION: ";
	if (wcscmp(bstrDesc, L"") == 0 )
		bstrDesc = L"None provided.";
	bstrError += bstrDesc;
	bstrError += L" MESSAGE: ";
	if (wcscmp(bstrMsg, L"") == 0 )
		bstrMsg = L"None provided.";
	bstrError += bstrMsg;

	return bstrError;
}

bool Utilities::FileCanBeRead(LPTSTR pzFileName, LPTSTR pzErr)
{
	bool bSuccess = false;
	TCHAR buffer[MAX_ERR_LEN];
	memset(buffer, 0, MAX_ERR_LEN);

	if( _taccess(pzFileName, 0 ) == -1 ) {   //first test if we can access
		_stprintf(buffer, "DTRAN ERROR: File: %s does not exist.", pzFileName);
	}
	else if (_taccess(pzFileName, 4 ) == -1 ) {  //now test if we can read it
		_stprintf(buffer, "DTRAN ERROR: File: %s does not have read permissions.", pzFileName);
	}
	else 
		bSuccess = true;

	buffer[_tcslen(buffer)] = '\0';
	_tcscpy(pzErr, buffer); 

	return bSuccess;
}

bool Utilities::PathCanBeAccessed(LPTSTR pzPathName, LPTSTR pzErr, bool bTestWriteAccess)
{
	bool bSuccess = false;
	TCHAR buffer[MAX_ERR_LEN];
	memset(buffer, 0, MAX_ERR_LEN);

	//make sure we have just the directory path
	TCHAR szDirPath[MAX_FILE_LEN]; memset(szDirPath, 0, MAX_FILE_LEN);
	if ( GetDirectoryPath(pzPathName, szDirPath) != 0 ) 
	{
		if( _taccess(szDirPath, 0 ) == -1 ) {   //first test if we can access
			_stprintf(buffer, "DTRAN ERROR: Path: %s does not exist.", szDirPath);
		}
		else if (bTestWriteAccess) {
			if (_taccess(szDirPath, 2 ) == -1 ) {  //now test write permissions on this path 		
				_stprintf(buffer, "DTRAN ERROR: Path: %s does not have write permissions.", szDirPath);
			}
			else bSuccess = true;
		}
		else
			bSuccess = true;

	}
	else 
		_stprintf(buffer, "DTRAN ERROR: Error in accessing path: %s access.", pzPathName);

	buffer[_tcslen(buffer)] = '\0';
	_tcscpy(pzErr, buffer); 
	return bSuccess;
}

_bstr_t Utilities::GetErrorList(ERRLIST listErrors, bool bTabs)
{
	_bstr_t bstrList = L"";
	ERRLIST::iterator listIt;
	int iCnt = 0;

	for (listIt = listErrors.begin(); listIt != listErrors.end(); listIt++) {
		if (iCnt > 0 ) {

			if (bTabs)
				bstrList += L"\r\n";
			else
				bstrList += L" ";

			bstrList +=  (*listIt);
		}
		else
			bstrList = (*listIt);
		iCnt++;
	}

	return bstrList;
}

int Utilities::TransferErrors(ERRLIST listFrom, ERRLIST &listTo)
{
	ERRLIST::iterator listIt;
	int iCnt = 0;

	for (listIt = listFrom.begin(); listIt != listFrom.end(); listIt++) {
		listTo.push_back((*listIt)); 
		iCnt++;
	}

	return iCnt;
}

_bstr_t Utilities::GetError(int iErrIndex, ERRLIST &listErr)
{
	ERRLIST::iterator listIt;
	_bstr_t bstrErr = L"";
	int iCnt = 0;
	
	if ( iErrIndex < (int) listErr.size()) {
		for (listIt = listErr.begin(); listIt != listErr.end(); listIt++) {
			if(iErrIndex == iCnt)
				bstrErr = (*listIt); 
			iCnt++;
		}
	}

	return bstrErr;
}

//***************************************************************************
//* raw code to create dbf file

int pack4(int j, char linn[])
{
	int	i, i1, i2[31], i3;

        struct bbb {
		unsigned j8 : 1;
		unsigned j7 : 1;
		unsigned j6 : 1;
		unsigned j5 : 1;
		unsigned j4 : 1;
		unsigned j3 : 1;
		unsigned j2 : 1;
		unsigned j1 : 1;
	};
	union aaa {
		char a1;
		struct bbb b2;
	};
	union aaa a1;


	i1 = j;
	i3 = 1073741824;
	for(i=30; i>=0; i--){
		i2[i]=0;
		if(i1>=i3){
			i2[i]=1;
			i1 -= i3;
		}
		i3 /= 2;
	}


	a1.a1=0;
	a1.b2.j1 = i2[7];
	a1.b2.j2 = i2[6];
	a1.b2.j3 = i2[5];
	a1.b2.j4 = i2[4];
	a1.b2.j5 = i2[3];
	a1.b2.j6 = i2[2];
	a1.b2.j7 = i2[1];
	a1.b2.j8 = i2[0];
	linn[0]=a1.a1;

	a1.a1=0;
	a1.b2.j1 = i2[15];
	a1.b2.j2 = i2[14];
	a1.b2.j3 = i2[13];
	a1.b2.j4 = i2[12];
	a1.b2.j5 = i2[11];
	a1.b2.j6 = i2[10];
	a1.b2.j7 = i2[9];
	a1.b2.j8 = i2[8];
	linn[1]=a1.a1;

	a1.a1=0;
	a1.b2.j1 = i2[23];
	a1.b2.j2 = i2[22];
	a1.b2.j3 = i2[21];
	a1.b2.j4 = i2[20];
	a1.b2.j5 = i2[19];
	a1.b2.j6 = i2[18];
	a1.b2.j7 = i2[17];
	a1.b2.j8 = i2[16];
	linn[2]=a1.a1;


	a1.a1=0;
	a1.b2.j2 = i2[30];
	a1.b2.j3 = i2[29];
	a1.b2.j4 = i2[28];
	a1.b2.j5 = i2[27];
	a1.b2.j6 = i2[26];
	a1.b2.j7 = i2[25];
	a1.b2.j8 = i2[24];
	linn[3]=a1.a1;
	linn[4]='\0';

	return(0);
}

_bstr_t Utilities::CreateDSN(_bstr_t bstrSrceDBase, _bstr_t bstrSQLServer, _bstr_t bstrSQLDatabase)
{
	HRESULT hr = S_OK;
	RETCODE retcode;
	LPCSTR szDriver = "SQL Server";
	
	//DWORD pfErrorCode;
//	char lpszErrorMessage[SQL_MAX_MESSAGE_LENGTH +1];  //SQL_MAX_MESSAGE_LENGTH =256
	WORD iErr = 1;
	//create name
	srand((unsigned)time( NULL ));
	int i = rand();
	TCHAR buffer[20];
	itoa(i, buffer, 10);
	buffer[strlen(buffer)+1] = '\0';

	TCHAR szDSN[BUFF_LEN]; memset(szDSN, 0, BUFF_LEN);
	_sntprintf(szDSN, BUFF_LEN, "TestTool_%s", buffer);
	szDSN[_tcslen(szDSN)] = '\0';
	_bstr_t bstrDSN(szDSN);

	//replace forward slashes with backward
	TCHAR ch1 = '/';
	TCHAR ch2 = '\\';
	TCHAR pzSrceDBase[BUFF_MED];  memset(pzSrceDBase, 0, BUFF_MED);
	TCHAR pzSrceDBaseIn[BUFF_MED];  memset(pzSrceDBaseIn, 0, BUFF_MED);
	int iStrLen = SysStringLen(bstrSrceDBase);
	WideCharToMultiByte(CP_ACP, 0, bstrSrceDBase, iStrLen, pzSrceDBaseIn, iStrLen+1, NULL, NULL);
	pzSrceDBaseIn[iStrLen] = '\0';
	LPTSTR pzTemp = pzSrceDBaseIn;

	i = 0;
	while ( i < iStrLen ) {
		if ( (*pzTemp) == ch1 )
			pzSrceDBase[i++] = ch2;  
		else 
			pzSrceDBase[i] = pzSrceDBaseIn[i];
		i++;
	}
	pzSrceDBase[_tcslen(pzSrceDBase)] = '\0';

	//set up attributes
	char szAttributes[BUFF_MED];
	strcpy(szAttributes, "DSN=");
	strcat(szAttributes, szDSN);
	int ilen = strlen(szAttributes)+1;

	strcpy(szAttributes+ilen, "DESCRIPTION=");
	ilen += strlen("DESCRIPTION=");
	strcat(szAttributes+ilen, "Temp DSN for tool");
	ilen += strlen("Temp DSN for tool") + 1;

//	if (wcscmp(bstrSrceDBase, L"") != 0 ) {
	if ( _tcslen(pzSrceDBase) != 0 ) {
		//attributes for either a foxpro DSN
		strcpy(szAttributes+ilen, "SourceType=");
		ilen += strlen("SourceType=");
		strcat(szAttributes + ilen, "DBF");
		ilen += strlen((char*)"DBF") + 1;

		strcpy(szAttributes+ilen, "SourceDB=");
		ilen += strlen("SourceDB=");
		strcat(szAttributes + ilen, pzSrceDBase);
		ilen += strlen(pzSrceDBase) + 1;

		strcpy(szAttributes+ilen, "Exclusive=No");
		ilen += strlen("Exclusive=No") + 1;

		szDriver = "Microsoft Visual FoxPro Driver";
	}
	else {
		//attributes for SQL server DSN
		TCHAR szSQLServer[BUFF_MED];  memset(szSQLServer, 0, BUFF_MED);
		int iStrLen = SysStringLen(bstrSQLServer);
		WideCharToMultiByte(CP_ACP, 0, bstrSQLServer, iStrLen, szSQLServer, iStrLen+1, NULL, NULL);
		szSQLServer[iStrLen] = '\0';

		TCHAR szSQLDatabase[BUFF_MED];  memset(szSQLDatabase, 0, BUFF_MED);
		iStrLen = SysStringLen(bstrSQLDatabase);
		WideCharToMultiByte(CP_ACP, 0, bstrSQLDatabase, iStrLen, szSQLDatabase, iStrLen+1, NULL, NULL);
		szSQLDatabase[iStrLen] = '\0';

		strcpy(szAttributes+ilen, "SERVER=");
		ilen += strlen("SERVER=");
		strcat(szAttributes + ilen, szSQLServer);
		ilen += strlen(szSQLServer) + 1;

		strcpy(szAttributes+ilen, "DATABASE=");
		ilen += strlen("DATABASE=");
		strcat(szAttributes + ilen, szSQLDatabase);
		ilen += strlen(szSQLDatabase) + 1;

		strcpy(szAttributes+ilen, "TRUSTED_CONNECTION=YES");
		ilen += strlen("TRUSTED_CONNECTION=YES") + 1;
	}

	szAttributes[ilen] = '\0';
	szAttributes[ilen+1] = '\0';

	retcode = SQLConfigDataSource(NULL, ODBC_ADD_SYS_DSN, szDriver, szAttributes);

	//SQLInstallerError returns error or status information for the ODBC installer functions.

/*	if ( retcode == FAIL ) {
		while ( SQLInstallerError(iErr++,&pfErrorCode,lpszErrorMessage,SQL_MAX_MESSAGE_LENGTH,NULL) != SQL_NO_DATA)
		{
			_bstr_t bError(lpszErrorMessage);
			SetFatalError(bError);
			hr=E_FAIL;
			if ( iErr > m_lMaxErrMsgs ) break;
		}
	}
*/	
	return bstrDSN;
}  //Utilities::CreateDSN

HRESULT Utilities::DeleteDSN(_bstr_t bstrDSN)
{
	if (wcscmp(bstrDSN, L"") == 0) return S_OK;

//	DWORD pfErrorCode;
//	char lpszErrorMessage[SQL_MAX_MESSAGE_LENGTH +1];  //SQL_MAX_MESSAGE_LENGTH =256
	WORD iErr = 1;
	LPCSTR szDriver = "SQL Server";
	TCHAR szDSN[BUFF_LEN];  memset(szDSN, 0, BUFF_LEN);
	int iStrLen = SysStringLen(bstrDSN);
	WideCharToMultiByte(CP_ACP, 0, bstrDSN, iStrLen, szDSN, iStrLen+1, NULL, NULL);
	szDSN[iStrLen] = '\0';

	TCHAR szAttributes[BUFF_SMALL];
	memset(szAttributes, 0, BUFF_SMALL);
	_tcscpy(szAttributes, "DSN=");
	_tcscat(szAttributes, szDSN);
	int ilen = _tcslen(szAttributes)+1;
	szAttributes[ilen] ='\0';
	szAttributes[ilen+1] = '\0';

	RETCODE retcode = SQLConfigDataSource(NULL, ODBC_REMOVE_SYS_DSN, szDriver, szAttributes);
/*	if ( retcode == FAIL ) {
		while ( SQLInstallerError(iErr++,&pfErrorCode,lpszErrorMessage,SQL_MAX_MESSAGE_LENGTH,NULL) != SQL_NO_DATA)
		{
			//if this fails do we want to set a fatal error message?
			LPCSTR lpTemp = lpszErrorMessage;
			if ( iErr > m_lMaxErrMsgs ) break;
		}
	}
*/
	return S_OK;
}  //Utilities::DeleteDSN

/**********************************************************************************/
int Utilities::CreateDBF(int dbaseIV, DBFFIELD input[], char* fileName)
{
/***************


same in dbase IV and V:
	C Character All OEM code page characters - padded with blanks to the width of the field. 
	D Date 8 bytes - date stored as a string in the format YYYYMMDD. 
	F Float Number stored as a string, right justified, and padded with blanks to the width of the field.  
	N Numeric Number stored as a string, right justified, and padded with blanks to the width of the field.  
	L Logical 1 byte - initialized to 0x20 (space) otherwise T or F. 
	I Long 4 bytes. Leftmost bit used to indicate sign, 0 negative. 

Dbase IV only:
	B Binary, a string 10 digits representing a .DBT block number. The number is stored as a string, right justified and padded with blanks. 
	M Memo, a string 10 digits (bytes) representing a .DBT block number. The number is stored as a string, right justified and padded with blanks. 
	@ Timestamp 8 bytes - two longs, first for date, second for time.  The date is the number of days since  01/01/4713 BC. Time is hours * 3600000L + minutes * 60000L + Seconds * 1000L 
	+ Autoincrement Same as a Long 
	O Double 8 bytes - no conversions, stored as a double. 
	G OLE 10 digits (bytes) representing a .DBT block number. The number is stored as a string, right justified and padded with blanks. 

Dbase V only:
	M Memo 4 byte char
	G General 4 byte char
	T Timedate
	B Double 8 bytes & can specify # decimals
	Y currency ... let this go through for IV or V

	left out these dbase V's:
		C character binary
		M memo binary

so for dbaseV, input O will be output as B, input @ will be output as T, B will be a double

and for dbaseIV, input T will be output as @, B will be char


**********************/

	FILE	*outfile;
	char	packed[10], pack2[10], setChar;
	int	i, j, k, lineLength, startpos, setLength, ncols, setScale;
	int	year, month, day;

	if ((outfile = fopen(fileName, "wb")) == NULL) return(1);

	pack4(48-45*dbaseIV, packed);
	fprintf(outfile, "%c", packed[0]);	/* first comes a 48; use 3 for dbaseIV */

	year=5;	/* creation date dummy */
	month=3;
	day=10;

	pack4(year, packed);
	fprintf(outfile, "%c", packed[0]);
	pack4(month, packed);
	fprintf(outfile, "%c", packed[0]);
	pack4(day, packed);
	fprintf(outfile, "%c", packed[0]);

	pack4(0, packed);			/* # records */
	fprintf(outfile, "%c%c%c%c", packed[0], packed[1], packed[2], packed[3]);

	ncols=0;
	while((input[ncols].Name[0]>='a' && input[ncols].Name[0]<='z') || input[ncols].Name[0]=='_' || (input[ncols].Name[0]>='A' && input[ncols].Name[0]<='Z'))ncols++;



k = 32 + 32*ncols + 263*(1-dbaseIV);

//printf("start: %d\n", k);

	pack4(k, packed);	/* start position of data, + 263 for vfp */
	fprintf(outfile, "%c%c", packed[0], packed[1]);

	lineLength=1;
	for(i=0; i<ncols; i++){
		setLength = input[i].Length;

		if(input[i].Type=='D')setLength = 8;
		if(input[i].Type=='L')setLength = 1;
		if(setLength<=0 && input[i].Type=='I')setLength = 4;
		if(input[i].Type=='+')setLength = 4;
		if(input[i].Type=='T')setLength = 8;
		if(input[i].Type=='@')setLength = 8;
		if(input[i].Type=='Y')setLength = 8;

		if(input[i].Type=='B' && dbaseIV==0)setLength = 10;
		if(input[i].Type=='B' && dbaseIV==1)setLength = 8;
		if(input[i].Type=='O')setLength = 8;

		if(input[i].Type=='M' && dbaseIV==0)setLength = 4;
		if(input[i].Type=='G' && dbaseIV==0)setLength = 4;
		if(input[i].Type=='M' && dbaseIV==1)setLength = 10;
		if(input[i].Type=='G' && dbaseIV==1)setLength = 10;

		if(setLength<0){
			setLength = 1+input[i].Precision;
			if(input[i].Scale>0)setLength++;
		}
		if(setLength<0)setLength=1;

		lineLength += setLength;
	}

	pack4(lineLength, packed);
	fprintf(outfile, "%c%c%c%c", packed[0], packed[1], packed[2], packed[3]);

	for(i=0; i<15; i++)fprintf(outfile, "%c", 0);	/* filler? */
	pack4(3, packed);	fprintf(outfile, "%c", packed[0]);
	for(i=0; i<2; i++)fprintf(outfile, "%c", 0);

	startpos = 1;	/* running start pos */

	for(i=0; i<ncols; i++){

		setChar=input[i].Type;
		if(setChar=='O' && dbaseIV==0)setChar='B';
		if(setChar=='T' && dbaseIV==1)setChar='@';
		if(setChar=='@' && dbaseIV==0)setChar='T';

		k = strlen(input[i].Name);	/* pad name with zeroes & add coltype */

		for(j=0; j<k; j++)fprintf(outfile, "%c", input[i].Name[j]);
		for(j=k; j<11; j++)fprintf(outfile, "%c", 0);
		fprintf(outfile, "%c", setChar);

		pack4(startpos, packed);
		fprintf(outfile, "%c%c%c%c", packed[0], packed[1], packed[2], packed[3]);

		setLength = input[i].Length;

		if(input[i].Type=='D')setLength = 8;
		if(input[i].Type=='L')setLength = 1;
		if(setLength<=0 && input[i].Type=='I')setLength = 4;
		if(input[i].Type=='+')setLength = 4;
		if(input[i].Type=='T')setLength = 8;
		if(input[i].Type=='@')setLength = 8;
		if(input[i].Type=='Y')setLength = 8;

		if(input[i].Type=='B' && dbaseIV==0)setLength = 10;
		if(input[i].Type=='B' && dbaseIV==1)setLength = 8;
		if(input[i].Type=='O')setLength = 8;

		if(input[i].Type=='M' && dbaseIV==0)setLength = 4;
		if(input[i].Type=='G' && dbaseIV==0)setLength = 4;
		if(input[i].Type=='M' && dbaseIV==1)setLength = 10;
		if(input[i].Type=='G' && dbaseIV==1)setLength = 10;

		if(setLength<0){
			setLength = 1+input[i].Precision;
			if(input[i].Scale>0)setLength++;
		}
		if(setLength<0)setLength=1;

		startpos += setLength;

		pack4(setLength, packed);

		if(setChar=='L' || setChar=='C' || setChar=='D' || setChar=='M' || setChar=='G' || (setChar=='B' && dbaseIV==1)){
			fprintf(outfile, "%c%c%c%c", packed[0], packed[1], packed[2], packed[3]);
		}

		if(setChar=='I' || input[i].Type=='+' || setChar=='T' || setChar=='@' || setChar=='O'){
			pack4(4, pack2);
			fprintf(outfile, "%c%c%c%c", packed[0], packed[1], pack2[0], pack2[1]);
		}

		if(setChar=='N' || setChar=='F'){
			setScale = input[i].Scale;
			if(setScale<0)setScale=0;
			pack4(setScale, pack2);
			fprintf(outfile, "%c%c%c%c", packed[0], pack2[0], 0, 0);
		}
		if(setChar=='B' && dbaseIV==0){
			setScale = input[i].Scale;
			if(setScale<0)setScale=0;
			pack4(setScale, pack2);
			fprintf(outfile, "%c%c%c", packed[0], pack2[0],0);
			pack4(4, pack2);
			fprintf(outfile, "%c", packed[0]);
		}
		if(setChar=='Y'){
			pack4(4, pack2);
			fprintf(outfile, "%c%c%c%c", packed[0], packed[1], pack2[0], pack2[0]);
		}

		for(j=0; j<12; j++)fprintf(outfile, "%c", 0);
	}

	pack4(13, packed); fprintf(outfile, "%c", packed[0]);

	if(dbaseIV==0){for(i=0; i<263; i++)fprintf(outfile, "%c", 0); }	/* these exist in foxpro files */

	pack4(26, packed);
	fprintf(outfile, "%c", packed[0]);	/* eof char - finish the file if have header but no data */

	fclose(outfile);

	return(0);
}

/*


*/