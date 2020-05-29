// This is a part of the OleDBPro module.
// Copyright (C) 2000-2001 UDAParts 
// All rights reserved.
//
// This source code is only intended as a supplement to the
// OleDBPro module Classes Reference and related
// electronic documentation provided with the Module.
// See these sources for detailed information regarding this
// UDAParts product.

// Please don't disclose any source code of the software to any person or entity,
//
// Please don't decompile, disassemble, or reverse engineer any object code of 
// any portion of the software.
//  
// Copyright (c) 2001 by Yuancai (Charlie) Ye
// http://www.geocities.com/OleDBPro/index.htm
// yekerui@yahoo.com

#ifndef __OLE_DATABASE25_H__
#define __OLE_DATABASE25_H__

#include "DBStruct.h"
#include <atlbase.h>
#define DONT_NEED_OLEDBPRO_DLL  //using original source code and not oledbr.dll  //need this when building release MinDependancy in .NET
#ifdef DONT_NEED_OLEDBPRO_DLL	//need original source codes 		
	#define	OLEDB_EXPORT25
#else
	#ifdef	BUILD_OLEDB25
		#define	OLEDB_EXPORT25 __declspec(dllexport)
	#else
		#define	OLEDB_EXPORT25 __declspec(dllimport)
	#endif
#endif


#if !defined(BUILD_OLEDB25) && !defined(DONT_NEED_OLEDBPRO_DLL)
	#ifdef __BORLANDC__		//Borland compiler
		#ifdef _DEBUG
			#pragma comment(lib, "OleDBPBD.lib")
//			#pragma message("Built with debug version of OleDBPro module for Borland C++ Builder")
		#else
			#pragma comment(lib, "OleDBPBR.lib")
//			#pragma message("Built with release version of OleDBPro module for Borland C++ Builder")
		#endif
	#else					//Visual C++
		#ifdef _DEBUG
			#pragma comment(lib, "OleDB25D.lib")
//			#pragma message("Built with debug version of OleDBPro module for MS Visual C++")
		#else
			#pragma comment(lib, "OleDB25R.lib")
//          #pragma message("Built with release version of OleDBPro module for MS Visual C++")
		#endif
	#endif 
#endif

typedef void (*GETERRORINFO) (IUnknown* pIObjetWithError, REFIID IID_InterfaceWithError, BSTR	*pstrErrInfo);

//unlock the OleDBPro
extern OLEDB_EXPORT25 char	g_strOleDBProKey[40];

//release MS Core service components
OLEDB_EXPORT25 void UninitOLEDBCore();

//usually, you will not use the following exports.
//you may use the following exports at particular cases
extern OLEDB_EXPORT25 DBOBJECT g_ObjectStruct;
extern OLEDB_EXPORT25 IDataInitialize	 *g_pIDataInitialize;
extern OLEDB_EXPORT25 IDBPromptInitialize	*g_pIDBPromptInitialize;
OLEDB_EXPORT25 bool GetIDataInitialize();
OLEDB_EXPORT25 bool GetIDBPromptInitialize();



class OLEDB_EXPORT25 COPropSet : public tagDBPROPSET
{
public:
	COPropSet();
	COPropSet(const GUID& guid);
	COPropSet(const COPropSet& OPropSet);
	~COPropSet();

public:
	COPropSet& operator=(const COPropSet& OPropSet);
	void AddProperty(DWORD nPropertyID, VARIANT& vtData);
	void AddProperty(DWORD nPropertyID, LPCWSTR strValue);
	void AddProperty(DWORD nPropertyID, LPCSTR strValue);
	void AddProperty(DWORD nPropertyID, bool bValue);
	void AddProperty(DWORD nPropertyID, short nValue);
	void AddProperty(DWORD nPropertyID, BYTE bValue);
	void AddProperty(DWORD nPropertyID, long nValue);

protected:
	void Add();
	void Copy(const COPropSet& propset);

private:
	void ReleaseMemories();
};

enum CursorType
{
	ForwardOnly=0,	//default and fastest forward and readonly	
	Static,			//cursor can scrollback, snapshot
	Keyset,			//some providers don't support this cursor
	Dynamic,			//most of providers don't support this cursor
};

void	OLEDB_EXPORT25 PrepareProps(COPropSet &DBPropSet, int nCursor=Static, bool bReadOnly=false, bool bBatch=false, bool bScrollback=false, bool bBookmark=false);

class OLEDB_EXPORT25 CRtnInfo
{
public:
	CRtnInfo();
	virtual ~CRtnInfo();

public:
	HRESULT			m_hr;
	BSTR			m_strError;	
	GETERRORINFO	m_pGetErrorInfo;	
};

class OLEDB_EXPORT25 CODataSource : public CRtnInfo
{
public:
	CODataSource();
	virtual ~CODataSource();

public:
	virtual bool	Open(LPCOLESTR strConnection, bool bUseCCE=true, bool bPooling=true, short nPrompt=DBPROMPT_COMPLETEREQUIRED);
	virtual bool	Open(const CLSID& clsidProvider, bool bUseCCE=true, bool bPooling=true, short nPrompt=DBPROMPT_COMPLETEREQUIRED);
	virtual bool	Open(HWND hWnd = GetActiveWindow(), DBPROMPTOPTIONS nPromptOptions = DBPROMPTOPTIONS_PROPERTYSHEET);
	virtual void	Close();
	bool			IsOpen(){return (m_pIDBInitialize ? true : false);}
	
	//IDBProperties
	bool		SetPropEx(tagDBPROPSET *pDBPropEx);
	DBPROPFLAGS	GetDBPropFlags(const GUID& guidPropertySet, DBPROPID nPropertyID, VARTYPE *pnVarType=NULL);
	bool		GetProperty(const GUID& guidPropertySet, DBPROPID nPropID, VARIANT* pVariant);
	
	//IDBDataSourceAdmin
	static bool	CreateDataSource(LPCOLESTR strProvider, LPCOLESTR strServer);
	
	void	SetServer(LPCOLESTR strServer);
	void	SetLocation(LPCOLESTR strLocation);
	void	SetDatabase(LPCOLESTR strDatabase);
	void	SetUID(LPCOLESTR strUID);
	void	SetPWD(LPCOLESTR strPWD);
	void	SetProviderString(LPCOLESTR strProviderString);

	//mainly used by various templates
	static REFGUID	GetRefGuid(){return DBGUID_DSO;}
	virtual REFIID	GetBaseIID();
	virtual IUnknown**	GetBaseInterfacePPtr(){return ((IUnknown**)&m_pIDBInitialize);}
	virtual bool PostProcess();
private:
	void		Initilize();

public:
	IDBInitialize	*m_pIDBInitialize;

protected:
	COPropSet		m_DBPropSet;			//holding information for accessing a database
};

class OLEDB_EXPORT25 COSession  : public CRtnInfo
{
public:
	COSession();
	virtual ~COSession();
	
public:
	virtual bool	Open(IDBInitialize *pIDBInitialize);
	virtual void	Close();

	//ICommandText 
	virtual bool	ExecuteSQL(LPCOLESTR strSQL);

	//ITransactionLocal
	virtual bool	BeginTrans(ISOLEVEL nIsoLevel=ISOLATIONLEVEL_READCOMMITTED, ITransactionOptions* pITOptions=NULL, ULONG* pnTLevel=NULL);
	
	//ITransaction
	virtual bool	Commit(BOOL bRetaining=FALSE, DWORD nXACTTC=XACTTC_SYNC);
	virtual bool	Rollback(BOID* pboidReason=NULL, BOOL bRetaining = FALSE, BOOL bAsync = FALSE);
   
	//IGetDataSource
	virtual bool	GetDataSource(CODataSource *pODataSource);
	
	virtual bool	IsOpen(){return m_pIOpenRowset ? true : false;}
	
	//mainly used by various templates
	static REFGUID	GetRefGuid(){return DBGUID_SESSION;}
	virtual REFIID	GetBaseIID();
	virtual IUnknown**	GetBaseInterfacePPtr(){return ((IUnknown**)&m_pIOpenRowset);}
	virtual bool	PostProcess(){return true;}

	//members
public:
	IOpenRowset	*m_pIOpenRowset;
};

class OLEDB_EXPORT25 COSessionEx	: public COSession
{
public:
	//IIndexDefinition
	virtual bool	CreateIndex(LPOLESTR strTblName, ULONG nIdxInfos, const DBINDEXCOLUMNDESC pIdxInfo[], LPOLESTR strIdxName=NULL, DBPROPSET *pDBPropSet=NULL, DBID **ppIndexID=NULL);
	virtual bool	DropIndex(LPOLESTR strTblName, LPOLESTR strIdxName);

	//ITableDefinition
	virtual bool	CreateTable(LPOLESTR strTblName, ULONG nCols, const CColInfo pColInfo[], DBID **ppTableID=NULL);
	virtual bool	DropTable(LPOLESTR strTblName);
	virtual bool	AddColumn(LPOLESTR strTblName, const CColInfo& ColInfo, DBID **ppColID=NULL); 
	virtual bool	DropColumn(LPOLESTR strTblName, LPOLESTR strColName);
	
	//ITableDefinitionWithConstraints
	virtual bool	AddConstraint(const CRefInfo &ThisInfo, const CRefInfo &RefInfo, LPOLESTR strKeyName, DBCONSTRAINTTYPE nConstraintType, DBUPDELRULE nUpdateRule=DBUPDELRULE_CASCADE, DBUPDELRULE nDeleteRule=DBUPDELRULE_CASCADE, DBMATCHTYPE nMatchType=DBMATCHTYPE_FULL);
	virtual bool	DropConstraint(LPOLESTR strTblName, LPOLESTR strKeyName);	
};

class OLEDB_EXPORT25 CPureBase : virtual public CRtnInfo
{
public:
	CPureBase();

public:
	virtual bool	GetSession(COSession *pOSession)=0;
	static LPOLESTR	GetDataSourceType(DBTYPE nDBType);
	static const ULONG	*GetFixLen(DBTYPE nDBType);

public:
	LONG	m_nRowsAffected;
};

class OLEDB_EXPORT25	CCmndBase : virtual public CPureBase
{
public:
	CCmndBase();
	virtual ~CCmndBase();
public:
	virtual bool	Cancel();
	virtual void	CloseCmnd();	
	virtual bool	HasCmnd(){return m_pICommandText ? true : false;}
	virtual void	OnTCommandCreated(ICommandText *pICommandText);

	//ICommand
	virtual bool	GetSession(COSession *pOSession);

protected:
	virtual bool	OpenCmnd(IOpenRowset *pIOpenRowset);

protected:
	ICommandText	*m_pICommandText;
};

class OLEDB_EXPORT25 CParamBase : virtual public CCmndBase
{
public:
	CParamBase();
	virtual ~CParamBase();

public:
	virtual void	CloseCmnd();	
	virtual bool	HasCmnd(){return (CCmndBase::HasCmnd()||m_pIParamsAccessor) ? true : false;}
	bool			IsBatchable(){return m_bBatchable;}
	
protected:
	ULONG			NumOutputParams();
	virtual ULONG	ComputeNumOfParams(LPCOLESTR strSQL, WCHAR cParam);
	virtual bool	OpenParam(CPInfo *pPInfo, bool bPrepared);
	virtual void	PrepareData(ULONG nCount, CPData *pPData);
	virtual void	CopyToClient(ULONG nCount, CPData *pPData);

private:
	bool	SendParamsInfo(bool bPrepared);
	void	CreateParamDBBindings(CPInfo *pPInfo);
	bool	CreateParamAccessor();
	void	Initialize();

protected:
	ULONG			m_nParams;
	BYTE			*m_pParamsData;
	DBBINDING		*m_pParamsDBBinds;
	DBPARAMBINDINFO	*m_pDBParamBindInfo;
	HACCESSOR		m_hParamsAccessor;
	ULONG			m_nNumOutputParams;
	ULONG			*m_pParamOrdinals;
	bool			m_bBatchable;
	IAccessor		*m_pIParamsAccessor;
	ULONG			m_nParamsBufferSize;
};


class CORow;
class OLEDB_EXPORT25 CRBase : virtual public CPureBase
{
public:
   CRBase();
   virtual ~CRBase();

public:
	//IRowset
	virtual bool	MoveFirst();
	virtual bool	MoveLast();
	virtual bool	MovePrev();
	virtual bool	MoveNext(LONG nSkipped=0);
	
	//IRowsetInfo
	virtual bool	GetSession(COSession *pOSession);

	virtual bool	OpenRBase(IUnknown *pIUnknownToRowset);
	virtual void	Close();	
	virtual bool	IsOpen(){return (m_pIAccessor || m_pIRowset);}
	virtual bool	IsReadOnly(){return true;}
	virtual void	OnCreatingAccessors();

	//inline functions
	inline LPCOLESTR GetColName(ULONG nCol)
	{
		ATLASSERT(IsOpen());	//make sure that a rowset is available!
		ATLASSERT(nCol && nCol<=m_nCol-IsBookmarked()); //make sure that a column ordinal is correct!
		return m_pDBColumnInfo[nCol+IsBookmarked()-1].pwszName;
	}
	inline bool IsBLOB(ULONG nCol)
	{
		ATLASSERT(IsOpen());	//make sure that a rowset is available!
		ATLASSERT(nCol && nCol<=m_nCol-IsBookmarked()); //make sure that a column ordinal is correct!
		ATLASSERT(m_pDBColumnInfo);
		return ((m_pDBColumnInfo[nCol+IsBookmarked()-1].dwFlags & DBCOLUMNFLAGS_ISLONG)==DBCOLUMNFLAGS_ISLONG || (m_pDBColumnInfo[nCol+IsBookmarked()-1].ulColumnSize+1)==0);
	}
	inline bool IsNullable(ULONG nCol)
	{
		ATLASSERT(IsOpen());	//make sure that a rowset is available!
		ATLASSERT(nCol && nCol<=m_nCol-IsBookmarked()); //make sure that a column ordinal is correct!
		ATLASSERT(m_pDBColumnInfo);
		return ((m_pDBColumnInfo[nCol+IsBookmarked()-1].dwFlags & DBCOLUMNFLAGS_ISNULLABLE)==DBCOLUMNFLAGS_ISNULLABLE);
	}
	inline bool Writable(ULONG nCol)
	{
		ATLASSERT(IsOpen());	//make sure that a rowset is available!
		ATLASSERT(nCol && nCol<=m_nCol-IsBookmarked()); //make sure that a column ordinal is correct!
		ATLASSERT(m_pDBColumnInfo);
		return ((m_pDBColumnInfo[nCol+IsBookmarked()-1].dwFlags & DBCOLUMNFLAGS_WRITE)==DBCOLUMNFLAGS_WRITE || (m_pDBColumnInfo[nCol+IsBookmarked()-1].dwFlags & DBCOLUMNFLAGS_WRITEUNKNOWN)==DBCOLUMNFLAGS_WRITEUNKNOWN);
	}
	inline bool IsBookmarked()
	{
		ATLASSERT(IsOpen());	//make sure that a rowset is available!
		ATLASSERT(m_pDBColumnInfo);
		return (m_pDBColumnInfo[0].iOrdinal==0);
	}
	inline ULONG	GetCols() {return (m_nActualCol-IsBookmarked());}
	inline bool		IsEOF(){return m_nRowsObtained ? false : true;}
	inline ULONG	GetRowsObtained(){return m_nRowsObtained;}
	inline ULONG	GetLength(ULONG nCol)
	{
		ATLASSERT(IsOpen());	//make sure that a rowset is available!
		ATLASSERT(nCol && nCol<=m_nCol-IsBookmarked()); //make sure that a column ordinal is correct!
		ULONG	*pnLen=GetLengthPtr(nCol);
		if(pnLen==NULL) 
			return 0;
		return *pnLen;
	}
    inline DBTYPE GetDataType(ULONG nCol)
	{
		ATLASSERT(IsOpen());	//make sure that a rowset is available!
		ATLASSERT(nCol && nCol<=m_nCol-IsBookmarked()); //make sure that a column ordinal is correct!
		ATLASSERT(m_pDBBinds);
		ULONG nIndex=FindAnIndexOnOrdinal(nCol);
		if(nIndex!=-1)
			return m_pDBBinds[nIndex].wType;
		return DBTYPE_NULL;
	}
		//** modified code....these next two functions....
	inline BYTE GetDataPrecision(ULONG nCol)
	{
		ATLASSERT(IsOpen());	//make sure that a rowset is available!
		ATLASSERT(nCol && nCol<=m_nCol-IsBookmarked()); //make sure that a column ordinal is correct!
		ATLASSERT(m_pDBBinds);
		ULONG nIndex=FindAnIndexOnOrdinal(nCol);
		if(nIndex!=-1)
			return m_pDBBinds[nIndex].bPrecision;
		return 0;
	}
	inline BYTE GetDataScale(ULONG nCol)
	{
		ATLASSERT(IsOpen());	//make sure that a rowset is available!
		ATLASSERT(nCol && nCol<=m_nCol-IsBookmarked()); //make sure that a column ordinal is correct!
		ATLASSERT(m_pDBBinds);
		ULONG nIndex=FindAnIndexOnOrdinal(nCol);
		if(nIndex!=-1)
			return m_pDBBinds[nIndex].bScale;
		return 0;
	}
	//** end of modified code

	inline DBTYPE GetRawDataType(ULONG nCol)
	{
		ATLASSERT(IsOpen());	//make sure that a rowset is available!
		ATLASSERT(nCol && nCol<=m_nCol-IsBookmarked()); //make sure that a column ordinal is correct!
		ATLASSERT(m_pDBColumnInfo);
		return m_pDBColumnInfo[nCol+IsBookmarked()-1].wType;
	}
	inline DBMEMOWNER GetDBMemOwner(ULONG nCol)
	{
		ATLASSERT(IsOpen());	//make sure that a rowset is available!
		ATLASSERT(nCol && nCol<=m_nCol-IsBookmarked()); //make sure that a column ordinal is correct!
		ATLASSERT(m_pDBBinds);
		ULONG nIndex=FindAnIndexOnOrdinal(nCol);
		if(nIndex==-1)
			return DBMEMOWNER_CLIENTOWNED;
		return m_pDBBinds[nIndex].dwMemOwner;
	}
	inline ULONG* GetLengthPtr(ULONG nCol)
	{
		ATLASSERT(IsOpen());	//make sure that a rowset is available!
		ATLASSERT(nCol && nCol<=m_nCol-IsBookmarked()); //make sure that a column ordinal is correct!
		ATLASSERT(m_pDBBinds);
		DBTYPE	nDBType;
		ULONG nIndex=FindAnIndexOnOrdinal(nCol);
		if(nIndex==-1)
			return NULL;
		if((m_pDBBinds[nIndex].dwPart & DBPART_LENGTH)==DBPART_LENGTH)
			return (ULONG*)(m_pData + m_pDBBinds[nIndex].obLength);
		nDBType=m_pDBBinds[nIndex].wType;
		return ((ULONG*)GetFixLen(nDBType));
	}
	inline ULONG	*GetStatusPtr(ULONG nCol)
	{
		ATLASSERT(IsOpen());	//make sure that a rowset is available!
		ATLASSERT(nCol && nCol<=m_nCol-IsBookmarked()); //make sure that a column ordinal is correct!
		ATLASSERT(m_pDBBinds);
		ULONG nIndex=FindAnIndexOnOrdinal(nCol);
		if(nIndex==-1 || (m_pDBBinds[nIndex].dwPart & DBPART_STATUS)!=DBPART_STATUS)
			return NULL;
		return (ULONG*)(m_pData + m_pDBBinds[nIndex].obStatus);
	}
	inline ULONG	GetStatus(ULONG nCol)
	{
		ULONG		*pnStatus=GetStatusPtr(nCol);
		if(pnStatus)
			return *pnStatus;
		return 0;
	}
	inline void* GetData(ULONG nCol)
	{
		ATLASSERT(IsOpen());	//make sure that a rowset is available!
		ATLASSERT(nCol && nCol<=m_nCol-IsBookmarked()); //make sure that a column ordinal is correct!
		ATLASSERT(m_pDBBinds);
		ULONG	nIndex=FindAnIndexOnOrdinal(nCol);
		if(nIndex==-1)
			return NULL;
		if(IsBLOB(nCol)) 
		{
			void **p=(void**)(m_pData + m_pDBBinds[nIndex].obValue);
			return (*p);
		}
		return (m_pData + m_pDBBinds[nIndex].obValue);
	}

    inline HROW GetCurrentRowHandle() 
    {
        ATLASSERT(m_hCurrent); 
        ATLASSERT(IsOpen()); 
        return m_hCurrent;
    }
    
	//other functions
	ULONG	FindColOrdinal(LPCOLESTR strColName, bool bCaseSensitive=true);
	ULONG	GetMaxLength(ULONG nCol);
	bool	IsChapter(ULONG nCol);
	bool	GetDataEx(ULONG nCol, void *pBuffer, DBTYPE nDBType, ULONG nSize=0, BYTE nPrecision=0, BYTE nScale=0);
	bool	GetBLOB(ULONG nCol, void *pBlobBuffer, ULONG* pnRead=NULL);
	bool	SetDataEx(ULONG nCol, void *pData, DBTYPE nDBType, ULONG nLen=OLEDB_STRING_END);
	bool	SetData(ULONG nCol, void *pSrcData, ULONG nSrcLen=OLEDB_STRING_END);
	bool	GetLenStatus(ULONG nCol, DBTYPE nDBType, ULONG *pnLen, ULONG *pnDBStatus,  BYTE nPrecision=0, BYTE nScale=0);
	DBPART	GetDBPart(ULONG nCol);
	bool	SetDBPart(ULONG nCol, DBPART nDBPart=DBPART_LENGTH|DBPART_VALUE|DBPART_STATUS);
	bool	SetDataType(ULONG nCol, DBTYPE nDBType, ULONG nLen=0, BYTE nPrecision=0, BYTE nScale=0, DBMEMOWNER nDBMemOwner=DBMEMOWNER_CLIENTOWNED);
	void	SetBatchSize(ULONG nBatchSize=DEFAULT_BATCH_SIZE);
	const	DBCOLUMNINFO* GetColInfo();
	DBTYPE	GetBookmarkType();
	ULONG	GetBookmarkLen();
	void*	GetBookmark();
	void	SetChapter(HCHAPTER hChapter);
	void	FreeRecordMemory();
	
	//IRowsetInfo
	bool	GetRowsetProperty(DBPROPID propid, VARIANT* pVariant);

	//mainly used by various templates
	static REFGUID	GetRefGuid(){return DBGUID_ROWSET;}
	virtual REFIID	GetBaseIID();
	virtual IUnknown**	GetBaseInterfacePPtr(){return ((IUnknown**)&m_pIAccessor);}
	virtual bool	PostProcess();

protected:
	virtual bool	GetDataFromServer(HROW hRow);
	virtual bool	QueryColsInfo();
	virtual bool	ReleaseRows();

private:
	bool	GetColumnsInfo();
	void	SetBindingValueLenStatus();
	bool	CreateAccessors();
	bool	CreateDBBindings();
	void	Initialize();
	inline ULONG	FindAnIndexOnOrdinal(ULONG nCol);
	//IChapteredRowset 
	bool	ReleaseChapter();

public:
	

protected:
	HCHAPTER		m_hChapter;
	HROW			*m_pHRow;
	BYTE			*m_pData;
	DBBINDING		*m_pDBBinds;
	ULONG			m_nCurrentRow;
	ULONG			m_nRowsObtained;
	LONG			m_nRowsetSize;
	ULONG			m_nBufferSize;
	HROW			m_hCurrent;
	ULONG			m_nCol;
	ULONG			m_nActualCol;
	DBCOLUMNINFO	*m_pDBColumnInfo;
	HACCESSOR		m_hAccessor;
	HACCESSOR		m_hWriteAccessor;
	//TRowset
	IRowset			*m_pIRowset;
	IAccessor		*m_pIAccessor;
private:
	HACCESSOR		m_hBookmarkAccessor;
    LPOLESTR        m_strColumnString;
	friend			CORow;
};

class OLEDB_EXPORT25 CRBaseEx : public CRBase
{
public:
	//IRowsetFind
	virtual bool	FindNextRow(CFindInfo& FindInfo, LONG nSkip=0, const BYTE* pBookmark=NULL, ULONG nBookmarkLen=0);
	
	//IRowsetIndex
	virtual bool	Seek(ULONG nKeys, CKeyInfo pKeyInfo[], DBSEEK nSeekOptions=DBSEEK_FIRSTEQ);
	virtual bool	SetRange(ULONG nStartKeys=0, CKeyInfo *pStartKeys=NULL, ULONG nEndKeys=0, CKeyInfo *pEndKeys=NULL, DBRANGE nRangeOptions=DBRANGE_INCLUSIVESTART|DBRANGE_INCLUSIVEEND);
	
	//IRowsetLocate
	virtual bool	GetRowsAt(const BYTE* pBookmark, LONG nRowsOffset=0, ULONG nBookmarkLen=0);

	//IRowsetScroll
	virtual bool	GetRowsAtRatio(ULONG nNumerator, ULONG nDenominator);
	virtual bool	GetApproximatePosition(const BYTE *pBookmark, ULONG *pnPosition, ULONG *pnRows, ULONG nBookmarkLen=0);
};

enum XMLDataType
{
	XML_String=0,	//no conversion, as determined by OLEDB data conversion library
	XML_Bin_hex,	//see XML data types defined
	XML_Boolean,
	XML_Char,
	XML_Date,
	XML_DateTime,
	XML_Fixed_14_4,
	XML_Number,
	XML_Time,
	XML_I1,
	XML_I2,
	XML_Int,
	XML_Ui1,
	XML_Ui2,
	XML_Ui4,
	XML_I4,
	XML_Float,
	XML_R4,
	XML_R8,
	XML_Uri,
	XML_Uuid,
	XML_DateTime_tz,
	XML_Time_tz
};

class OLEDB_EXPORT25 CToXML : virtual public CRtnInfo
{
public:
	CToXML();
	virtual ~CToXML();

public:
	virtual DBTYPE	GetRawDataType(ULONG nCol)=0;
	virtual ULONG	GetCols()=0;
	virtual bool	GetDataEx(ULONG nCol, void *pBuffer, DBTYPE nDBType, ULONG nSize=0, BYTE nPrecision=0, BYTE nScale=0)=0;
	bool	SetXMLDataType(ULONG nCol, XMLDataType nXMLDataType);
	XMLDataType GetXMLDataType(ULONG nCol);
	const char* GetXMLStringDataType(ULONG nCol);

protected:
	//handle XML Reserved characters such as <, &, >, ", and '.
	virtual void Chars2DT(char **pstrDT);
	virtual void WChars2DT(WCHAR **pstrDT);

	virtual void CreateArrayOfXMLDataTypes();

	//OLEDB fixed data types to XML data types
	virtual void ODB2XMLConversion(ULONG nCol, char* const strData); 

	//!!!!may need to overwrite a following function 
	//if a data type conversion is not fit to your requirement!!!!
	virtual void ToBin_hex(ULONG nCol, char *const strInn);
	virtual void ToBoolean(ULONG nCol, char *const strIn);
	virtual void ToChar(ULONG nCol, char *const strIn);
	virtual void ToDate(ULONG nCol, char *const strIn);
	virtual void ToDateTime(ULONG nCol, char *const strIn);
	virtual void ToDateTime_tz(ULONG nCol, char *const strIn);
	virtual void ToFixed_14_4(ULONG nCol, char *const strIn);
	virtual void ToNumber(ULONG nCol, char *const strIn);
	virtual void ToInt(ULONG nCol, char *const strIn);
	virtual void ToTime(ULONG nCol, char *const strIn);
	virtual void ToTime_tz(ULONG nCol, char *const strIn);
	virtual void ToI1(ULONG nCol, char *const strIn);
	virtual void ToI2(ULONG nCol, char *const strIn);
	virtual void ToUi1(ULONG nCol, char *const strIn);
	virtual void ToUi2(ULONG nCol, char *const strIn);
	virtual void ToUi4(ULONG nCol, char *const strInn);
	virtual void ToUri(ULONG nCol, char *const strIn);
	virtual void ToUuid(ULONG nCol, char *const strIn);
	virtual void ToI4(ULONG nCol, char *const strIn);
	virtual void ToFloat(ULONG nCol, char *const strIn);
	virtual void ToR4(ULONG nCol, char *const strIn);
	virtual void ToR8(ULONG nCol, char *const strIn);

private:
	bool	IsReasonable(ULONG nCol, XMLDataType nXMLDataType);

private:
	XMLDataType	*m_pXMLDataType;
};

class OLEDB_EXPORT25 CODataset : public CRBase, public CCmndBase
{
public:
	CODataset();
	virtual ~CODataset();

public:
	virtual bool	IsOpen(){return (m_pIMDDataset || CRBase::IsOpen()) ? true : false;}
	virtual void	Close();
	virtual bool	OpenWithCmnd(IOpenRowset *pIOpenRowset, LPCOLESTR strMDX, DBPROPSET *pDBPropSet=NULL, ULONG nPropSets=1, REFGUID guidDialect=DBGUID_DEFAULT);
	virtual bool	GetSession(COSession *pOSession);

	//IMDDataset::GetCellData(......)
    virtual bool	MoveFirst();
	virtual bool	MoveLast();
	virtual bool	MovePrev();
	virtual bool	MoveNext(LONG nSkipped=0);
	virtual bool	MoveTo(ULONG nCellOrdinal);
	virtual bool	MoveTo(ULONG *pnTupleOrdinals);

	//IMDDataset::GetAxisRowset(......)
	virtual bool	GetAxisRowset(CRBase *pRBase, ULONG nAxis, ULONG nPropSets=0, DBPROPSET *pPropSets=NULL);
	
	//IMDRangeRowset::GetRangeRowset(......)
	virtual bool	GetRangeRowset(CRBase *pRBase, ULONG nStartCell, ULONG nEndCell, ULONG nPropSets=0, DBPROPSET *pPropSets=NULL);
   
	inline bool		IsEOF(){return m_hCurrent ? false : true;}
	ULONG			GetCells(){return m_nRowsObtained;}
	ULONG			GetAxes(){return m_nAxes-1;}
	const MDAXISINFO*	GetAxisInfo(){return m_pAxisInfo;}
	
	//mainly used by various templates
	virtual REFIID		GetBaseIID();
	virtual IUnknown**	GetBaseInterfacePPtr(){return ((IUnknown**)&m_pIMDDataset);}
	virtual bool		PostProcess();

protected:
	virtual bool	GetDataFromServer(HROW hRow);
	void			SetRowsObtained();

protected:
	IMDDataset		*m_pIMDDataset;
	ULONG			m_nAxes;
   MDAXISINFO		*m_pAxisInfo;
};

class OLEDB_EXPORT25 COView : public CPureBase
{
public:
	COView();
	virtual ~COView();

public:
	virtual bool	IsOpen(){return m_pIUnknown ? true : false;}
	virtual void	Close();
	virtual bool	OpenView(CRBase *pRBase);
	virtual bool	OpenWithCmnd(IOpenRowset *pIOpenRowset, LPCOLESTR strSelect, DBPROPSET *pDBPropSet=NULL, ULONG nPropSets=1, REFGUID guidDialect=DBGUID_DEFAULT);
	
	//IViewChapter
	virtual bool	GetSession(COSession *pOSession);
	virtual bool	ApplyView(CRBase *pRBase, HCHAPTER hChapter=DB_NULL_HCHAPTER);

	//IViewSort
	virtual bool	SetSortOrder(ULONG nCols, const ULONG pnCol[], const DBSORT pnSort[]);
	
	//IViewFilter
	virtual bool	SetFilter(ULONG nCols, CFilterInfo pFilterInfo[]);

	//IViewRowset
	virtual bool	OpenRowset(CRBase *pRBase);

	void			SetBaseIID(const GUID& guidBase);
	
	////mainly used by various templates
	REFIID	GetBaseIID(){return m_IID;}
	virtual REFGUID	GetRefGuid(){return DBGUID_ROWSET;}
	virtual IUnknown**	GetBaseInterfacePPtr(){return &m_pIUnknown;}
	virtual bool	PostProcess(){return true;}

protected:
	GUID			m_IID;	
	IUnknown		*m_pIUnknown;
};

class CORow;
class OLEDB_EXPORT25 COStream : virtual public CRtnInfo
{
public:
	COStream();
	virtual ~COStream();

public:
	virtual bool	IsOpen(){return m_pISequentialStream ? true : false;}
	virtual void	Close();
	
	//IGetSourceRow
	virtual bool	GetSourceRow(CORow *pORow);
	virtual bool	GetSession(COSession *pOSession);

	//ISequentialStream
	virtual bool	GetData(void* pStreamBuffer, ULONG* pnGet);
	virtual bool	PutData(void* pStreamBuffer, ULONG* pnPut);

	//IStream
	virtual bool	Commit(DWORD nCommitFlags=STGC_DEFAULT);

	//mainly used by various templates
	static REFGUID	GetRefGuid(){return DBGUID_STREAM;}
	virtual REFIID	GetBaseIID();
	virtual IUnknown**	GetBaseInterfacePPtr(){return ((IUnknown**)&m_pISequentialStream);}
	virtual bool PostProcess();

protected:
	ISequentialStream	*m_pISequentialStream;
};

class OLEDB_EXPORT25 CORow : public CPureBase
{
public:
	CORow();
	virtual ~CORow();

public:
	bool	IsOpen(){return m_pIRow ? true : false;}
	virtual bool	Open(CRBase	*pRBase);
	virtual void	Close();

	//IGetSession
	virtual bool	GetSession(COSession *pOSession);

	//IRow
	virtual bool	GetColumns(ULONG nCols, DBCOLUMNACCESS pDBColAccess[]);
	virtual bool	GetSourceRowset(CRBase *pRBase);
	
	//IRowChange
	virtual bool	SetColumns(ULONG nCols, DBCOLUMNACCESS pDBColAccess[]);

	//IRowSchemaChange
	virtual bool	AddColumns(ULONG nCols, const DBCOLUMNINFO pNewColInfo[], DBCOLUMNACCESS *pDBColAccess=NULL);
	virtual bool	DeleteColumns(ULONG nCols, const DBID pColIDs[], DBSTATUS *pnStatus=NULL);
	
	//IScopedOperations
	virtual bool	Delete(LPCOLESTR strURLs[], DBSTATUS *pnStatus, ULONG nRows=1, DBDELETEFLAGS nDeleteFlags=DBDELETE_ATOMIC);
	virtual bool	Copy(LPCOLESTR strSourceURLs[], LPCOLESTR strDestURLs[], DBSTATUS *pnStatus, ULONG nRows=1, LPOLESTR *strNewURLs=NULL, OLECHAR **ppStringsBuffer=NULL, DBCOPYFLAGS nCopyFlags=DBCOPY_ALLOW_EMULATION|DBCOPY_REPLACE_EXISTING, IAuthenticate *pIAuthenticate=NULL);
	virtual bool	Move(LPCOLESTR strSourceURLs[], LPCOLESTR strDestURLs[], DBSTATUS *pnStatus, ULONG nRows=1, LPOLESTR *strNewURLs=NULL, OLECHAR **ppStringsBuffer=NULL, DBMOVEFLAGS nMoveFlags=DBMOVE_ALLOW_EMULATION|DBMOVE_REPLACE_EXISTING, IAuthenticate *pIAuthenticate=NULL);
	virtual bool	OpenRowset(CRBase *pRBase, LPOLESTR strTblName, LPOLESTR strIdxName=NULL, DBPROPSET *pDBPropSet=NULL, ULONG nPropSets=1);
	virtual bool	OpenRowset(LPOLESTR strTblName, CRBase *pRBase, LPOLESTR strIdxName=NULL, int nCursor=Static, bool bReadOnly=false, bool bBatch=false, bool bScrollback=false, bool bBookmark=false, const DBPROPSET* pPropSet=NULL);

	//IColumnsInfo
	virtual bool	GetColumnInfo(ULONG *pnCols, DBCOLUMNINFO **ppDBColInfo, OLECHAR **ppStringsBuffer);
	
	virtual bool	OpenWithCmnd(IOpenRowset *pIOpenRowset, LPCOLESTR strSelect, DBPROPSET *pDBPropSet=NULL, ULONG nPropSets=1, REFGUID guidDialect=DBGUID_DEFAULT);
	
	//mainly used by various templates
	static REFGUID	GetRefGuid(){return DBGUID_ROW;}
	virtual REFIID	GetBaseIID();
	virtual IUnknown**	GetBaseInterfacePPtr(){return ((IUnknown**)&m_pIRow);}
	virtual bool	PostProcess(){return true;}

protected:
	IRow			*m_pIRow;
};

class OLEDB_EXPORT25 COBinder : public CRtnInfo
{
public:
	COBinder();
	virtual ~COBinder();

public:
	virtual bool	IsOpen(){return m_pIBindResource ? true : false;}
	virtual bool	Open(CLSID clsidBinder=__uuidof(RootBinder));
	virtual void	Close();

	//IDBBinderProperties
	virtual bool	SetProperties(ULONG nCount, DBPROPSET *pDBPropSet);

	//used by global template functions, bool BindObject(...) and bool CreateObject(...)
	virtual REFIID	GetBaseIID();
	virtual IUnknown**	GetBaseInterfacePPtr(){return ((IUnknown**)&m_pIBindResource);}

protected:
	IBindResource	*m_pIBindResource;
};

#endif

// This is a part of the OleDBPro module.
// Copyright (C) 2000-2001 UDAParts 
// All rights reserved.
//
// This source code is only intended as a supplement to the
// OleDBPro module Classes Reference and related
// electronic documentation provided with the Module.
// See these sources for detailed information regarding this
// UDAParts product.

// Please don't disclose any source code of the software to any person or entity,
//
// Please don't decompile, disassemble, or reverse engineer any object code of 
// any portion of the software.
//  
// Copyright (c) 2001 by Yuancai (Charlie) Ye
// http://www.geocities.com/OleDBPro/index.htm
// yekerui@yahoo.com