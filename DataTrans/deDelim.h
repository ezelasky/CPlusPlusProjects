// deDelim.h : Declaration of the CdeDelim

#ifndef __DEDELIM_H_
#define __DEDELIM_H_

#include "resource.h"       // main symbols

#if !defined
#include "Utilities.h"
#endif


/////////////////////////////////////////////////////////////////////////////
// CdeDelim
class ATL_NO_VTABLE CdeDelim : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CdeDelim, &CLSID_deDelim>,
	public IDispatchImpl<IdeDelim, &IID_IdeDelim, &LIBID_DTRANLib>
{
public:
	CdeDelim()
	{
		m_lRowsLoaded = 0;
		m_bstrDirectory = L"";
		m_bstrDelim = L",";
		m_bstrRowDelim = "\r\n";
		m_bstrTextQual = L"";

		//rule - store directories w/out backslashes.....
		TCHAR buffer[512];
		memset(buffer, 0, 512);
		GetCurrentDirectory(512, buffer);
		_bstr_t bstrDir(buffer);
		m_bstrDirectory = bstrDir;

		m_bstrFileName = L"";
		m_bTraceLogOn = VARIANT_FALSE;
		m_bHeaderLine = VARIANT_FALSE;
		m_bstrVersion = L"v1.0.1";

		m_lFirstRow = 0;
		m_lLastRow = 0;
		
		//create format object...
		IFormat* pFormat = NULL;
		CoCreateInstance(CLSID_Format, NULL, CLSCTX_INPROC_SERVER, IID_IFormat, (void**) &pFormat);

		m_pFormatObj = pFormat;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DEDELIM)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CdeDelim)
	COM_INTERFACE_ENTRY(IdeDelim)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IdeDelim
public:
	STDMETHOD(LoadFormat)(/*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(get_ErrMsgList)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_RowDelimiter)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_RowDelimiter)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Header)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Header)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_TextQualifier)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_TextQualifier)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Delimiter)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Delimiter)(/*[in]*/ BSTR newVal);
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
	_bstr_t m_bstrDirectory;
	_bstr_t m_bstrFileName;
	_bstr_t m_bstrVersion;
	_bstr_t m_bstrDelim;
	_bstr_t m_bstrRowDelim;
	_bstr_t m_bstrTextQual;

	VARIANT_BOOL m_bTraceLogOn;
	VARIANT_BOOL m_bHeaderLine;

	long	m_lRowsLoaded;
	long	m_lFirstRow;
	long	m_lLastRow;

	ERRLIST m_listErrors;
	int TransferErrors();
	int ValidateFile(_bstr_t bstrFile, long TestRecNum, long MaxErrors, FILE* ptrErrorFile, LPTSTR ErrorFile);

	IFormat* m_pFormatObj;
};

#endif //__DEDELIM_H_
