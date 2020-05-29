// deXBase.h : Declaration of the CdeXBase

#ifndef __DEXBASE_H_
#define __DEXBASE_H_

#include "resource.h"       // main symbols

#if !defined
#include "Utilities.h"
#endif


/////////////////////////////////////////////////////////////////////////////
// CdeXBase
class ATL_NO_VTABLE CdeXBase : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CdeXBase, &CLSID_deXBase>,
	public IDispatchImpl<IdeXBase, &IID_IdeXBase, &LIBID_DTRANLib>
{
public:
	CdeXBase()
	{
		m_lRowsLoaded = 0;
		m_bstrTable = L"";   //this is really a filename
		m_bstrDirectory = L"";
		m_pFormatObj = NULL;
		m_bTraceLogOn = VARIANT_FALSE;
		m_bstrVersion = L"v1.0.1";
		m_lFirstRow = 0;
		m_lLastRow = 0;
		m_nXBaseType = vFOXPRO;

		//rule - store directories w/out backslashes.....
		TCHAR buffer[512];
		memset(buffer, 0, 512);
		GetCurrentDirectory(512, buffer);
		_bstr_t bstrDir(buffer);
		m_bstrDirectory = bstrDir;

		//create format object...
		IFormat* pFormat = NULL;
		CoCreateInstance(CLSID_Format, NULL, CLSCTX_INPROC_SERVER, IID_IFormat, (void**) &pFormat);

		m_pFormatObj = pFormat;

	}

DECLARE_REGISTRY_RESOURCEID(IDR_DEXBASE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CdeXBase)
	COM_INTERFACE_ENTRY(IdeXBase)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IdeXBase
public:
	STDMETHOD(LoadFormat)(VARIANT_BOOL *pRetVal);
	STDMETHOD(get_TableType)(/*[out, retval]*/ XBASETYPE *pVal);
	STDMETHOD(put_TableType)(/*[in]*/ XBASETYPE newVal);
//	STDMETHOD(get_TraceOn)(/*[out, retval]*/ VARIANT_BOOL *pVal);
//	STDMETHOD(put_TraceOn)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_LastRow)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_LastRow)(/*[in]*/ long newVal);
	STDMETHOD(get_FirstRow)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_FirstRow)(/*[in]*/ long newVal);
	STDMETHOD(Exists)(/*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(Import)(/*[in]*/ IDispatch* pIn, /*[in]*/ DESTACTION action, /*[out, retval]*/ VARIANT_BOOL *bRetVal);
	STDMETHOD(Export)(/*[in]*/ IDispatch* pIn, /*[in]*/ DESTACTION action, /*[out, retval]*/ VARIANT_BOOL *bRetVal);
	STDMETHOD(get_Version)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_TraceLogOn)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_TraceLogOn)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_RowsLoaded)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_ErrMsgs)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_ErrMsgList)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Table)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Table)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Directory)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Directory)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Format)(/*[out, retval]*/ IFormat* *pObj);
	STDMETHOD(put_Format)(/*[in]*/ IFormat* pObj);

	void FinalRelease()
	{
		if (m_pFormatObj)
			m_pFormatObj->Release();
		m_pFormatObj = NULL;
	}

private:
	XBASETYPE m_nXBaseType;
	long	 m_lRowsLoaded;
	long     m_lFirstRow;
	long	 m_lLastRow;

	_bstr_t m_bstrTable;
	_bstr_t m_bstrDirectory;
	_bstr_t m_bstrVersion;
	VARIANT_BOOL m_bTraceLogOn;


	ERRLIST m_listErrors;
	IFormat* m_pFormatObj;
};

#endif //__DEXBASE_H_
