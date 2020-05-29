// Process.h : Declaration of the CProcess

#ifndef __PROCESS_H_
#define __PROCESS_H_

#include "resource.h"       // main symbols

#if !defined
#include "Utilities.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcess
class ATL_NO_VTABLE CProcess : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CProcess, &CLSID_Process>,
	public IDispatchImpl<IProcess, &IID_IProcess, &LIBID_DTRANLib>
{
public:
	CProcess()
	{
		m_nAction = vCREATE;
		m_bTraceLogOn = VARIANT_FALSE;
		m_bAddSource = VARIANT_FALSE;
		//m_bTruncationWarning = VARIANT_FALSE;
//		m_bstrErrMsgs = L"";
		m_lRowsLoaded = 0;
		m_bstrLogFileName = L"";

		m_iLoadCnt = 1;
		m_bCreatedFMT = false;
		m_bTableOK = false;
		m_bstrDSN = L"";
		m_bMultiLoad = false;

	}

DECLARE_REGISTRY_RESOURCEID(IDR_PROCESS)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CProcess)
	COM_INTERFACE_ENTRY(IProcess)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IProcess
public:
	STDMETHOD(SQLToXBase)(/*[in]*/ IdeSQL* pSQL, /*[in]*/ IdeXBase* pXBase,  /*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(IsXBaseObject)(/*[in]*/ IDispatch* pIDisp, /*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(XBaseToSQL)(/*[in]*/ IdeXBase* pXBase, /*[in]*/ IdeSQL* pSQL, /*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(get_AddSource)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_AddSource)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(GetNextError)(/*[out]*/ BSTR* pValOut, /*[out, retval]*/ VARIANT_BOOL* pVal);
	STDMETHOD(GetFirstError)(/*[out]*/ BSTR* pValOut, /*[out, retval]*/ VARIANT_BOOL* pVal);
	STDMETHOD(get_ErrMsgList)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(IsDelimFileObject)(/*[in]*/ IDispatch* pIDisp, /*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(SQLToDelim)( /*[in]*/ IdeSQL* pSQL,/*[in]*/ IdeDelim* pDelim, /*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(DelimToSQL)(/*[in]*/ IdeDelim* pDelim, /*[in]*/ IdeSQL* pSQL, /*[out, retval]*/ VARIANT_BOOL* pRetVal);
//	STDMETHOD(get_TruncationOccurred)(/*[out, retval]*/ VARIANT_BOOL *pRetVal);
	STDMETHOD(get_Action)(/*[out, retval]*/ DESTACTION *pVal);
	STDMETHOD(put_Action)(/*[in]*/ DESTACTION newVal);
	STDMETHOD(MultiImportToSQL)(/*[in]*/ IDispatch* pDisp , /*[in]*/ IdeSQL* pSQL, /*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(get_TraceFileName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_TraceFileName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_RowsLoaded)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_RowsLoaded)(/*[in]*/ long newVal);
	STDMETHOD(get_ErrMsgs)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(SQLToFixed)(/*[in]*/IdeSQL *pSQL, /*[in]*/IdeFixed *pFixed, /*[in]*/ long lChunk, /*[in]*/ long lFileNum,  /*[out, retval]*/ VARIANT_BOOL *pRetVal);
	STDMETHOD(FixedToSQL)(/*[in]*/ IdeFixed* pFixed, /*[in]*/ IdeSQL* pSQL,  /*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(IsFixedFileObject)(/*[in]*/ IDispatch *pIDisp, /*[out, retval]*/ VARIANT_BOOL *pRetVal);
	STDMETHOD(IsSQLObject)(/*[in]*/ IDispatch* pIDisp, /*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(put_TraceLogOn)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_TraceLogOn)(/*[in]*/ VARIANT_BOOL* pVal);
	VARIANT_BOOL m_bTraceLogOn;

private:
	//properties
	DESTACTION m_nAction;
//	_bstr_t m_bstrErrMsgs;
	long m_lRowsLoaded;
	_bstr_t m_bstrLogFileName;
	VARIANT_BOOL m_bAddSource;
	//VARIANT_BOOL m_bTruncationWarning;

	//internally used member variables
	_bstr_t m_bstrFileNameBase;
	_bstr_t m_bstrFileNameExt;
		
	//used mainly during multiloading
	int m_iLoadCnt;
	bool m_bCreatedFMT;
	bool m_bTableOK;
	bool m_bMultiLoad;
	_bstr_t m_bstrDSN;

	int m_iErrIndex;
	ERRLIST m_listErrors;

	int TransferErrors(IFormat** pFormat);
	int BuildFieldList(IFormat** pFormat, SQLFIELDLIST* ptrFldArray, bool bGenericColNames);

	private:
//	VARIANT_BOOL ReconcileFieldColl(IFormat** pSrceFormat, IFormat** pTrgtFormat);

};

#endif //__PROCESS_H_
