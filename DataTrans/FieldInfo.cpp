// FieldInfo.cpp : Implementation of CFieldInfo
#include "stdafx.h"
#include "DTran.h"
#include "FieldInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CFieldInfo


STDMETHODIMP CFieldInfo::get_Name(BSTR *pVal)
{
	*pVal = m_bFieldName.copy();
	return S_OK;
}

STDMETHODIMP CFieldInfo::put_Name(BSTR newVal)
{
	m_bFieldName = newVal;
	return S_OK;
}


STDMETHODIMP CFieldInfo::get_Type(FIELDTYPE *pVal)
{
	*pVal = m_nType;
	return S_OK;
}

STDMETHODIMP CFieldInfo::put_Type(FIELDTYPE newVal)
{
	m_nType = newVal;
	return S_OK;
}


STDMETHODIMP CFieldInfo::get_Length(BSTR *pVal)
{
	*pVal = m_bLength.copy();
	return S_OK;
}

STDMETHODIMP CFieldInfo::put_Length(BSTR newVal)
{
	m_bLength = newVal;
	return S_OK;
}

STDMETHODIMP CFieldInfo::get_Start(long *pVal)
{
	*pVal = m_lStartPos;

	return S_OK;
}

STDMETHODIMP CFieldInfo::put_Start(long newVal)
{
	m_lStartPos = newVal;

	return S_OK;
}

STDMETHODIMP CFieldInfo::get_End(long *pVal)
{
	*pVal = m_lEndPos;

	return S_OK;
}

STDMETHODIMP CFieldInfo::put_End(long newVal)
{
	m_lEndPos = newVal;

	return S_OK;
}

STDMETHODIMP CFieldInfo::get_Precision(long *pVal)
{
	*pVal = m_lPrecision;
	return S_OK;
}

STDMETHODIMP CFieldInfo::put_Precision(long newVal)
{
	m_lPrecision = newVal;
	return S_OK;
}

STDMETHODIMP CFieldInfo::get_Scale(long *pVal)
{
	*pVal = m_lScale;
	return S_OK;
}

STDMETHODIMP CFieldInfo::put_Scale(long newVal)
{
	m_lScale = newVal; 
	return S_OK;
}


