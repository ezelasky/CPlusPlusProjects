// FormatSpec.h : Declaration of the CFormatSpec

#ifndef __FORMATSPEC_H_
#define __FORMATSPEC_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CFormatSpec
class ATL_NO_VTABLE CFormatSpec : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFormatSpec, &CLSID_FormatSpec>,
	public IDispatchImpl<IFormatSpec, &IID_IFormatSpec, &LIBID_DTRANLib>
{
public:
	CFormatSpec()
	{
		m_bSQLTypePos = L"";
		m_bTypePos = L"";
		m_bEndPos = L"";
		m_bLengthPos = L"";
		m_bStartPos = L"";
		m_bNamePos = L"";
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FORMATSPEC)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CFormatSpec)
	COM_INTERFACE_ENTRY(IFormatSpec)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IFormatSpec
public:
	STDMETHOD(get_SQLTypePos)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_SQLTypePos)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_TypePos)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_TypePos)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_EndPos)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_EndPos)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_LengthPos)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_LengthPos)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_StartPos)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_StartPos)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_NamePos)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_NamePos)(/*[in]*/ BSTR newVal);

	_bstr_t m_bSQLTypePos;
	_bstr_t m_bTypePos;
	_bstr_t m_bEndPos;
	_bstr_t m_bLengthPos;
	_bstr_t m_bStartPos;
	_bstr_t m_bNamePos;
//	_bstr_t m_bPath;
};

#endif //__FORMATSPEC_H_
