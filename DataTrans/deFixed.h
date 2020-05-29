// deFixed.h : Declaration of the CdeFixed

#ifndef __DEFIXED_H_
#define __DEFIXED_H_

#include "resource.h"       // main symbols

#if !defined
#include "Utilities.h"
#endif


/////////////////////////////////////////////////////////////////////////////
// CdeFixed
class ATL_NO_VTABLE CdeFixed : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CdeFixed, &CLSID_deFixed>,
	public IDispatchImpl<IdeFixed, &IID_IdeFixed, &LIBID_DTRANLib>
{
public:
	CdeFixed()
	{
		m_lRowsLoaded = 0;
		m_bstrDirectory = L"";

		//rule - store directories w/out backslashes.....
		TCHAR buffer[512];
		memset(buffer, 0, 512);
		GetCurrentDirectory(512, buffer);
		_bstr_t bstrDir(buffer);
		m_bstrDirectory = bstrDir;

		m_bstrFileName = L"";
		m_bTraceLogOn = VARIANT_FALSE;
		m_bstrVersion = L"v1.0.1";

		m_lFirstRow = 1;
		m_lLastRow = 0;
		
		//create format object...
		IFormat* pFormat = NULL;
		CoCreateInstance(CLSID_Format, NULL, CLSCTX_INPROC_SERVER, IID_IFormat, (void**) &pFormat);

		m_pFormatObj = pFormat;

	}

DECLARE_REGISTRY_RESOURCEID(IDR_DEFIXED)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CdeFixed)
	COM_INTERFACE_ENTRY(IdeFixed)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IdeFixed
public:
	STDMETHOD(LoadFormat)(/*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(get_ErrMsgList)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_LastRow)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_LastRow)(/*[in]*/ long newVal);
	STDMETHOD(get_FirstRow)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_FirstRow)(/*[in]*/ long newVal);
	STDMETHOD(get_Version)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(Exists)(/*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(get_Format)(/*[out, retval]*/ IFormat* *pObj);
	STDMETHOD(put_Format)(/*[in]*/ IFormat* pObj);
	STDMETHOD(SetFormat)(/*[in]*/ IFormat* pObj, /*[out, retval]*/ VARIANT_BOOL* bSuccess);
	STDMETHOD(Import)(/*[in]*/ IDispatch* pIn, /*[in]*/ DESTACTION action, /*[out, retval]*/ VARIANT_BOOL *bRetVal);
	STDMETHOD(Export)(/*[in]*/ IDispatch* pIn, /*[in]*/ DESTACTION action, /*[out, retval]*/ VARIANT_BOOL *bRetVal);
	STDMETHOD(get_ErrMsgs)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_RowsLoaded)(/*[out, retval]*/ long *pVal);
	STDMETHOD(get_FileName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_FileName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Directory)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Directory)(/*[in]*/ BSTR newVal);
	STDMETHOD(put_TraceLogOn)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_TraceLogOn)(/*[in]*/ VARIANT_BOOL* pVal);
	STDMETHOD(ValidateFile)(/*[in]*/ BSTR FileName, /*[in]*/ long TestRecNum, /*[in]*/ long MaxErrors,  /*[out, retval]*/ VARIANT_BOOL* pRetVal);

	void FinalRelease()
	{
		if (m_pFormatObj)
			m_pFormatObj->Release();
		m_pFormatObj = NULL;
	}
private:
	long	m_lRowsLoaded;
	_bstr_t m_bstrDirectory;
	_bstr_t m_bstrFileName;
	_bstr_t m_bstrVersion;
	VARIANT_BOOL m_bTraceLogOn;
	long m_lFirstRow;
	long m_lLastRow;

	ERRLIST m_listErrors;
	int TransferErrors();
	int ValidateFile(_bstr_t bstrFile, long TestRecNum, long MaxErrors, FILE* ptrErrorFile, LPTSTR ErrorFile);

	IFormat* m_pFormatObj;

};

#endif //__DEFIXED_H_
