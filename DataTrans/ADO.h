// ADO.h: interface for the CADO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADO_H__CB28982F_FBE2_4180_BC16_66D6F5159265__INCLUDED_)
#define AFX_ADO_H__CB28982F_FBE2_4180_BC16_66D6F5159265__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#import <msado15.dll>  named_guids rename("EOF", "adoEOF") rename("Recordset", "adoRecordset")

#if !defined
#include "Utilities.h"
#endif

class CADO  
{
public:
	CADO() {m_bstrServer = L""; m_bstrDatabase = L""; }
	CADO(_bstr_t bstrServer, _bstr_t bstrDatabase);
	virtual ~CADO();

	HRESULT GetTrustedConnection(IDispatch **pp_Connection, _bstr_t bstrSQLServer = L"", _bstr_t bstrSQLDatabase = L"");

	HRESULT OpenTrustedConnection(_bstr_t bstrSQLServer = L"", _bstr_t bstrSQLDatabase = L"");
	HRESULT ExecuteQuery(_bstr_t sQry);
	HRESULT GetRecordSet(IDispatch **pp_Recordset, _bstr_t sQry);
	HRESULT CreateTableFromQuery(_bstr_t bstrQuery, LPTSTR pzTableName, bool bCreateTemp = false);
	HRESULT CloseConnection();

	//self contained -- creates/closes connection
	bool TableExists(_bstr_t bstrSQLServer, _bstr_t bstrSQLDatabase, _bstr_t bstrTable);
	HRESULT DropTable(_bstr_t bstrSQLServer, _bstr_t bstrSQLDatabase, _bstr_t bstrTable);
	bool ColumnExists(_bstr_t bstrTable, _bstr_t bstrColumn, _bstr_t bstrSQLServer = L"", _bstr_t bstrSQLDatabase = L"");
	bool ColumnTextLengthVaries(_bstr_t bstrTable, _bstr_t bstrColumn, _bstr_t bstrSQLServer, _bstr_t bstrSQLDatabase );
	_bstr_t GetListofFieldNames(bstr_t bstrSQLServer, _bstr_t bstrSQLDatabase, _bstr_t bstrTable, int& iNumFields);

	ERRLIST m_listErrors;

private:
	ADODB::_ConnectionPtr m_spConnection;
	_bstr_t m_bstrServer;
	_bstr_t m_bstrDatabase;

	int LoadErrorsFromConnection();
	int LoadErrorsFromConnection(ADODB::_ConnectionPtr spConnection);


};

#endif // !defined(AFX_ADO_H__CB28982F_FBE2_4180_BC16_66D6F5159265__INCLUDED_)
