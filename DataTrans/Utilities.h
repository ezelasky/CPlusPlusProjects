// Utilities.h: interface for the Utilities class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UTILITIES_H__F3086438_F553_4DE9_82DF_08209C3D5EDC__INCLUDED_)
#define AFX_UTILITIES_H__F3086438_F553_4DE9_82DF_08209C3D5EDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined

#include <sql.h>
#include <sqlext.h>
#include <odbcss.h>
#include <odbcinst.h>

#define	DBINITCONSTANTS
#include "OLEDB25.h"

#endif


typedef std::list<_bstr_t> ERRLIST;

extern TCHAR sz_CreateTableFromQuery[];
extern TCHAR sz_SchemaForColumnSQL[];
extern TCHAR sz_InfoSchemaOnTable[];
extern TCHAR sz_ExtraFieldName[];
extern TCHAR sz_VariedQuery[];
extern TCHAR sz_SQLSelectStmt[];
extern TCHAR sz_SQLSelectStmt2[];
extern TCHAR sz_SQLSelectStmt3[];
extern TCHAR sz_SQLSelectStmt4[];
extern TCHAR sz_DBase[];
extern TCHAR sz_FoxPro[];
extern TCHAR szDBaseConnStr[];
extern TCHAR szFoxProConnStr[];
extern TCHAR szSQLConnStr1[];
extern TCHAR szAccessConnStr[];
extern TCHAR szLog_RowsCopied[];


class Utilities  
{
public:
	Utilities();
	virtual ~Utilities();

	static LPTSTR ConstructFileName(LPTSTR pzBase, LPTSTR pzExt);
	static int ConstructFileName(LPTSTR pzBase, LPTSTR pzExt, LPTSTR pzFileName);
	static int BreakupFileName(_bstr_t bstrFileName, LPTSTR pzBase, LPTSTR pzExt);
	static int BreakUpFileName(_bstr_t bstrFileName, LPTSTR pzDir, LPTSTR pzBaseName, LPTSTR pzExt);
	static bool IsSubDirPath(LPTSTR pzPath);
	static bool IsFullPath(LPTSTR pzPath);
	static bool IsLastCharSlash(LPTSTR pzPath);
	static bool IsFirstCharSlash(LPTSTR pzPath);
	static int RemoveLastSlash(LPTSTR pzIn, LPTSTR pzOut);
	static _bstr_t ConstructFullPath(_bstr_t bstrDir, _bstr_t bstrFile);
	static int GetDirectoryPath(LPTSTR pzFullPath, LPTSTR pzDirPath);
	static int ChangeFileExtension(LPTSTR pzFileName, LPTSTR pzNewExt, LPTSTR pzNameOut);
	static int AddExtension(LPTSTR pzFileName, LPTSTR pzNewExt, LPTSTR pzNameOut);
	static int RemoveChar(LPTSTR pzStrIn, TCHAR ch, LPTSTR pzStrOut);
	static int ParseOutFileName(LPTSTR pzPath, LPTSTR pzFile);
	static bool FindSegment(LPSTR ptrStrOrig, LPSTR ptrSeg);
	static LPTSTR ConstructUniqueName(LPTSTR pzBase);
	static LPTSTR ConstructInfoSchemaSQL(LPTSTR pzTableName, LPTSTR pzOwnerName);
	static bool IsNullOrEmpty(_variant_t vTemp);
	static int ReplaceChar(LPTSTR pzStrIn, LPTSTR pzStrOut, TCHAR ch1, TCHAR ch2);
	static int TrimFirst(LPTSTR pzBufferIn, LPTSTR pzBufferOut);
	static int TrimLast(LPTSTR pzBufferIn, LPTSTR pzBufferOut);

	static bool FileCanBeRead(LPTSTR pzFileName, LPTSTR pzErr);
	static bool PathCanBeAccessed(LPTSTR pzPathName, LPTSTR pzErr, bool bTestWriteAccess);

	static int CreateDBF(int dbaseIV, DBFFIELD input[], char* fileName);
	static _bstr_t CreateDSN(_bstr_t bstrSrceDBase, _bstr_t bstrSQLServer, _bstr_t bstrSQLDatabase);
	static HRESULT DeleteDSN(_bstr_t bstrDSN);

	//Err list manipulations
	static _bstr_t ConstructErrMsg(_bstr_t bstrText, _bstr_t bstrSource, _bstr_t bstrDesc, _bstr_t bstrErrorMsg);
	static _bstr_t GetErrorList(ERRLIST list, bool bTabs);
	static int TransferErrors(ERRLIST listFrom, ERRLIST &listTo);
	static _bstr_t GetError(int iErrIndex, ERRLIST & listErr);
};

#endif // !defined(AFX_UTILITIES_H__F3086438_F553_4DE9_82DF_08209C3D5EDC__INCLUDED_)
