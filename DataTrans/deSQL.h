// deSQL.h : Declaration of the CdeSQL

#ifndef __DESQL_H_
#define __DESQL_H_

#include "resource.h"       // main symbols

#if !defined
#include "ADO.h"
#include "Utilities.h"
#endif


/////////////////////////////////////////////////////////////////////////////
// CdeSQL
class ATL_NO_VTABLE CdeSQL : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CdeSQL, &CLSID_deSQL>,
	public IDispatchImpl<IdeSQL, &IID_IdeSQL, &LIBID_DTRANLib>
{
public:
	CdeSQL()
	{
//		m_bstrErrMsgs = L"";
		m_lRowsLoaded = 0;
		m_bstrPassword = L"";
		m_bstrUserID = L"";
		m_bstrTable = L"";
		m_bstrDatabase = L"";
		m_bstrServer = L"";
		m_bstrOwner = L"dbo";
		m_bstrExportQuery = L"";
		m_pFormatObj = NULL;
		m_bTraceLogOn = VARIANT_FALSE;
		m_bstrVersion = L"v1.0.1";
		m_bDropTable = false;
		m_lBatchSize = SQLBATCHSIZE;
		m_bNoInfo = VARIANT_TRUE;
	    m_lMaxErrMsgs = MAXERRMSGS;
		m_bstrIDName = L"";
		m_bstrIDDef = L"bigint identity(1,1)";

		//create format object...
		IFormat* pFormat = NULL;
		CoCreateInstance(CLSID_Format, NULL, CLSCTX_INPROC_SERVER, IID_IFormat, (void**) &pFormat);

		m_pFormatObj = pFormat;
	}


DECLARE_REGISTRY_RESOURCEID(IDR_DESQL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CdeSQL)
	COM_INTERFACE_ENTRY(IdeSQL)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IdeSQL
public:
	STDMETHOD(get_ID_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ID_Name)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_ID_Definition)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ID_Definition)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Err_NoInfo)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Err_NoInfo)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Err_MaxMessages)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Err_MaxMessages)(/*[in]*/ long newVal);
	STDMETHOD(get_ImportBatchSize)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_ImportBatchSize)(/*[in]*/ long newVal);
	STDMETHOD(Execute)(/*[in]*/ BSTR Query, /*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(DatabaseExists)(/*[in]*/ BSTR Server, /*[in]*/ BSTR DataBase, /*[out, retval]*/ VARIANT_BOOL* pRetVal);
	//database related operations
	STDMETHOD(AddDatabase)(/*[in]*/ BSTR DataBase, /*[in]*/ BSTR Path, /*[in]*/ BSTR DatSize, /*[in]*/ BSTR LogSize, /*[in]*/ BSTR DatGrowth, /*[in]*/ BSTR LogGrowth, /*[in]*/ BSTR DatMaxSize, /*[in]*/ BSTR LogMaxSize,  /*[out, retval]*/ VARIANT_BOOL *pRetVal);
	STDMETHOD(Query)(/*[in]*/ BSTR bstrQuery, /*[out, retval]*/ IDispatch* *pRecSet);
	
	//import and export related operations
	STDMETHOD(get_ErrMsgList)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ErrMsgList)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Owner)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Owner)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_ExportQuery)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_ExportQuery)(/*[in]*/ BSTR newVal);
	STDMETHOD(FieldResize)(/*[in]*/ RESIZEOPTION option, /*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(LoadFormat)(/*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(get_Version)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(Exists)(/*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(ExportMulti)(/*[in]*/ IDispatch* pIn, /*[in]*/ DESTACTION action, /*[in]*/ SPLITRULE rule, /*[in]*/ long lValue, /*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(ImportMulti)(/*[in]*/ IDispatch* pIn, /*[in]*/ DESTACTION action, /*[in]*/ VARIANT_BOOL AddSource, /*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(Export)(/*[in]*/ IDispatch* pIn, /*[in]*/ DESTACTION action, /*[out, retval]*/ VARIANT_BOOL *bRetVal);
	STDMETHOD(GetCreateTableSQL)(/*[out, retval]*/ BSTR* pRetVal);
	STDMETHOD(Import)(/*[in]*/ IDispatch* pIn, /*[in]*/ DESTACTION action, /*[out, retval]*/ VARIANT_BOOL* bRetVal);
	STDMETHOD(get_ErrMsgs)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_RowsLoaded)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_Password)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Password)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_UserID)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_UserID)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Table)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Table)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Database)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Database)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Server)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Server)(/*[in]*/ BSTR newVal);
	STDMETHOD(put_Format)(/*[in]*/ IFormat* pObj);
	STDMETHOD(get_Format)(/*[in, out]*/ IFormat** pObj);
	STDMETHOD(SetFormat)(/*[in]*/ IFormat* pObj, /*[out, retval]*/ VARIANT_BOOL* bSuccess);
	STDMETHOD(put_TraceLogOn)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_TraceLogOn)(/*[in]*/ VARIANT_BOOL* pVal);
	STDMETHOD(CreateTable)(/*[out, retval]*/ VARIANT_BOOL* pRetVal);

	void FinalRelease()
	{
		if (m_bDropTable) {
			CADO objADO(L"", L"");
			objADO.DropTable(m_bstrServer, m_bstrDatabase, m_bstrTable);
		}
		if (m_pFormatObj)
			m_pFormatObj->Release();
		m_pFormatObj = NULL;
	}

private:
	long		m_lRowsLoaded;
	_bstr_t		m_bstrPassword;
	_bstr_t		m_bstrUserID;
	_bstr_t		m_bstrTable;
	_bstr_t		m_bstrDatabase;
	_bstr_t		m_bstrServer;
	_bstr_t		m_bstrVersion;
	_bstr_t		m_bstrExportQuery;
	_bstr_t		m_bstrOwner;
	_bstr_t		m_bstrIDName;
	_bstr_t		m_bstrIDDef;
	VARIANT_BOOL m_bTraceLogOn;
	VARIANT_BOOL m_bNoInfo;
	long		m_lMaxErrMsgs;
	long	m_lBatchSize;

	bool m_bDropTable;

	ERRLIST m_listErrors;
	IFormat* m_pFormatObj;
};

#endif //__DESQL_H_
