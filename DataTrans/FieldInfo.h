// FieldInfo.h : Declaration of the CFieldInfo

#ifndef __FIELDINFO_H_
#define __FIELDINFO_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CFieldInfo
class ATL_NO_VTABLE CFieldInfo : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFieldInfo, &CLSID_FieldInfo>,
	public IDispatchImpl<IFieldInfo, &IID_IFieldInfo, &LIBID_DTRANLib>
{
public:
	CFieldInfo()
	{
		m_bLength = L"";
		m_bFieldName = L"";
		m_lStartPos = 0;
		m_lEndPos = 0;
		m_lPrecision = 20;
		m_lScale = 10;
		m_nType = vVARCHAR;

	}

DECLARE_REGISTRY_RESOURCEID(IDR_FIELDINFO)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CFieldInfo)
	COM_INTERFACE_ENTRY(IFieldInfo)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IFieldInfo
public:
	STDMETHOD(get_Scale)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Scale)(/*[in]*/ long newVal);
	STDMETHOD(get_Precision)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Precision)(/*[in]*/ long newVal);
	STDMETHOD(get_End)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_End)(/*[in]*/ long newVal);
	STDMETHOD(get_Start)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Start)(/*[in]*/ long newVal);
	STDMETHOD(get_Length)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Length)(/*[in]*/ BSTR newVal);

	STDMETHOD(get_Type)(/*[out, retval]*/ FIELDTYPE *pVal);
	STDMETHOD(put_Type)(/*[in]*/ FIELDTYPE newVal);

	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Name)(/*[in]*/ BSTR newVal);

	FIELDTYPE m_nType;
	_bstr_t m_bLength;
	_bstr_t m_bFieldName;
	long m_lStartPos;
	long m_lEndPos;
	long m_lPrecision;
	long m_lScale;
};

#endif //__FIELDINFO_H_
