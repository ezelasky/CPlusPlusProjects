// BaseTransfer.h: interface for the CBaseTransfer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASETRANSFER_H__5BCE11EF_413D_44C2_99E0_496A0B0F94B0__INCLUDED_)
#define AFX_BASETRANSFER_H__5BCE11EF_413D_44C2_99E0_496A0B0F94B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Log.h"
#include "COMSink.h"

#include <sql.h>
#include <sqlext.h>
#include <odbcss.h>
#include <odbcinst.h>

#import "dtspkg.dll"  named_guids rename("EOF", "dtsEOF")  no_namespace

#include "ADO.h"

#define	DBINITCONSTANTS
#include "OLEDB25.h"
//#import "dtspkg.dll"  named_guids rename("EOF", "dtsEOF")  no_namespace

#if !defined
#include "Utilities.h"
#endif

typedef enum {nCREATE=0, nOVERWRITE, nAPPEND, nDROPADD, nCONFIRM } ACTION;
typedef enum {nDBASE=0, nFOXPRO } XBASE;

class CBaseTransfer  
{
public:
	CBaseTransfer(_bstr_t bstrSQLServer,
				  _bstr_t bstrSQLDatabase,
				  _bstr_t bstrSQLTable,
				  bool bImportToSQL);

	virtual ~CBaseTransfer();
	virtual HRESULT Init(bool bTrace);
	virtual HRESULT Transfer() = 0;
	virtual void Cleanup(bool bCreated = false) = 0;
	HRESULT ConfirmSQLTable(ACTION nAction, _bstr_t bstrCreateSQL);

	_bstr_t m_bstrSQLServer;
	_bstr_t m_bstrSQLDatabase;
	_bstr_t m_bstrSQLTable;
	_bstr_t m_bstrSQLOwner;

	long m_lRowsLoaded;
	ACTION m_nAction;
	bool m_bImportToSQL;
	long m_lBatchSize;

	_bstr_t m_bFile;
	_bstr_t m_bstrTraceFileName;
	_bstr_t m_bDir;
	_bstr_t m_bFileLoc;
	_bstr_t m_bDSN;

	//partial import
	long m_lFirst;
	long m_lLast;

	_bstr_t GetErrorMsg();   
	bool ErrorsOccurred() {return m_bFatalErrs;}
	_bstr_t m_bstrFatalErrMsgs;
	bool m_bFatalErrs;
	ERRLIST	m_listErrors;
	long m_lMaxErrMsgs;

protected:
	virtual HRESULT CheckInputParams() = 0;
	virtual void LogInputParams() = 0;

	//class helper functions...
	void SetFatalError(_bstr_t bErr); 
	//HRESULT CreateDSN(_bstr_t bstrSrceDBase);
	HRESULT DeleteDSN();
	void _LogInputParams();
	HRESULT _CheckInputParams();
	void SetTime(bool bStartTime = true);
	void SetandWriteTime(bool bStartTime = true);

	CLog* ptrLog;
	CADO* ptrADO;
	bool m_bInitCalled;
private:
	void Close();
	int m_sHour;
	int m_sMin;
	int m_sSec;
	int m_eHour;
	int m_eMin;
	int m_eSec;
};

//********************************************************************
//
//	CBCPTransfer Class -- utilitzes the BCP APIs to transfer to/from
//						  a flat file to SQL server table
//
//********************************************************************

class CBCPTransfer : public CBaseTransfer
{

public:
	CBCPTransfer(_bstr_t bstrSQLServer,
				 _bstr_t bstrSQLDatabase,
				 _bstr_t bstrSQLTable,
				  bool bImportToSQL);

	CBCPTransfer(bool bImportToSQL );
	virtual ~CBCPTransfer();
	HRESULT Transfer();
	virtual HRESULT Init(bool bTransfer);
	virtual void Cleanup(bool bCreated = false);

protected:
	void LogInputParams();
	HRESULT CheckInputParams();

private:
	HRESULT InitMultiParams(); 
	void ExtractBCPErrors(HDBC hdbc1, TCHAR* pzText, bool bWarningSrch = false);
	void MultiExport();
	_bstr_t m_bstrBCPFileLoc;

public:
	_bstr_t m_bFormatFileLoc;
	long m_lRecLen;  //total bytes length of all fields
	bool m_bMinErrInfo;
	bool m_bVariedTextLenQuery;

	//multi export SQLToText
	long m_lChunkSize;  //number of reconrds in a file
	long m_lFileNum;
	long m_lRecCnt;

//	void Cleanup(bool bFMTCreated, _bstr_t bstrFormatFileLoc, _bstr_t bstrDSN);
};

//********************************************************************
//
//	CDTSTransfer Class -- utilitzes the DTS APIs to transfer to/from
//						  a flat file to SQL server table
//
//********************************************************************
class CDTSTransfer : public CBaseTransfer
{
public:
	CDTSTransfer(_bstr_t bstrSQLServer,
				 _bstr_t bstrSQLDatabase,
				 _bstr_t bstrSQLTable,
				  bool bImportToSQL);

	CDTSTransfer(bool bImportToSQL );
	virtual ~CDTSTransfer();
	virtual HRESULT Init(bool bTransfer);
	virtual HRESULT Transfer();
	virtual void Cleanup(bool bCreated = false) {;}

protected:
	virtual void LogInputParams() = 0;
	virtual HRESULT CheckInputParams() = 0;
	virtual _bstr_t GetOtherObjectName() = 0;
	virtual TCHAR* GetOtherProgID() = 0;
	virtual void InitOtherConnection(ConnectionPtr ptrOther) = 0;
	virtual void AddMoreTaskProperties(DataPumpTask2Ptr ptrCustomTask) = 0;
	_bstr_t GetOtherConnName() {return m_bstrOtherConnName;}
	_bstr_t m_bstrSQLObjectName;
	long m_lSkipRows;
	bool m_bTempFile;

private:
	int m_iSQLConnID;
	int m_iOtherConnID;
	_bstr_t m_bstrSQLConnName;
	_bstr_t m_bstrOtherConnName;
	
	void ExtractDTSStepErrors(_Package2Ptr spPackage);

public:
	SQLFIELDLIST* m_ptrSrceFldList;
	SQLFIELDLIST* m_ptrTrgtFldList;
};

//********************************************************************
//
//	CDTSDelim Class -- perfomrs delimited transfers via DTS 
//
//
//********************************************************************

class CDTSDelim : public CDTSTransfer
{
public:
	CDTSDelim(_bstr_t bstrSQLServer,
				 _bstr_t bstrSQLDatabase,
				 _bstr_t bstrSQLTable,
				  bool bImportToSQL);
	CDTSDelim(bool bImportToSQL );
	virtual ~CDTSDelim();

protected:
	void LogInputParams();
	HRESULT CheckInputParams();
	void InitOtherConnection(ConnectionPtr ptrOther);
	_bstr_t GetOtherObjectName(){return m_bFileLoc;}
	TCHAR* GetOtherProgID() {return "DTSFlatFile";}
	void AddMoreTaskProperties(DataPumpTask2Ptr ptrCustomTask) {;}

public:
	_bstr_t m_bstrDelim;
	_bstr_t m_bstrRowDelim;
	_bstr_t m_bstrTextQual;
	bool m_bHeaderLine;
};

//********************************************************************
//
//	CDTSDBF Class -- performs DBF transfers via DTS 
//
//
//********************************************************************

class CDTSDBF : public CDTSTransfer
{
public:
	CDTSDBF(_bstr_t bstrSQLServer,
				 _bstr_t bstrSQLDatabase,
				 _bstr_t bstrSQLTable,
				  bool bImportToSQL);
	CDTSDBF(bool bImportToSQL );
	virtual ~CDTSDBF();
	HRESULT Init(bool bTransfer);
	void Cleanup(bool bCreated = false);
	HRESULT ConfirmDBFTable(ACTION nAction, DBFFIELD Fields[]);
	_bstr_t m_bDBFName;
	_bstr_t m_bDBFDir;
	XBASE m_nXBaseType;

protected:
	void LogInputParams();
	HRESULT CheckInputParams();
	void InitOtherConnection(ConnectionPtr ptrOther);
	_bstr_t GetOtherObjectName() { if (m_bImportToSQL )   return GetOtherConnName(); else return m_bDBFName; }
	TCHAR* GetOtherProgID() { if (m_nXBaseType == nFOXPRO) return "MSDASQL"; else return "Microsoft.Jet.OLEDB.4.0"; }
	void AddMoreTaskProperties(DataPumpTask2Ptr ptrCustomTask);
};


//a basic factory class design pattern -- hands back a ptr to the correct transfer object
//fun to play around with but overkill for what we are doing...does work!
/*
class TransferAgent  
{
public:
	static CBaseTransfer* TransferAgent::getTransfer(int y, bool bImportToSQL)  //getting link error -- not sure why...
	{
		if (y == 0)
			return  new CDTSDBF(bImportToSQL);
		else
			return new CDTSDelim(bImportToSQL);
	}

	TransferAgent();
	virtual ~TransferAgent();
	
	
};
*/
#endif // !defined(AFX_BASETRANSFER_H__5BCE11EF_413D_44C2_99E0_496A0B0F94B0__INCLUDED_)
