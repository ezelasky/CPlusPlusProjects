// FormatSpec.cpp : Implementation of CFormatSpec
#include "stdafx.h"
#include "DTran.h"
#include "FormatSpec.h"

/////////////////////////////////////////////////////////////////////////////
// CFormatSpec


STDMETHODIMP CFormatSpec::get_NamePos(BSTR *pVal)
{
	*pVal = m_bNamePos.copy();

	return S_OK;
}

STDMETHODIMP CFormatSpec::put_NamePos(BSTR newVal)
{
	m_bNamePos = newVal;

	return S_OK;
}

STDMETHODIMP CFormatSpec::get_StartPos(BSTR *pVal)
{
	*pVal = m_bStartPos.copy();

	return S_OK;
}

STDMETHODIMP CFormatSpec::put_StartPos(BSTR newVal)
{
	m_bStartPos = newVal;

	return S_OK;
}

STDMETHODIMP CFormatSpec::get_LengthPos(BSTR *pVal)
{
	*pVal = m_bLengthPos.copy();

	return S_OK;
}

STDMETHODIMP CFormatSpec::put_LengthPos(BSTR newVal)
{
	m_bLengthPos = newVal;

	return S_OK;
}

STDMETHODIMP CFormatSpec::get_EndPos(BSTR *pVal)
{
	*pVal = m_bEndPos.copy();

	return S_OK;
}

STDMETHODIMP CFormatSpec::put_EndPos(BSTR newVal)
{
	m_bEndPos = newVal;

	return S_OK;
}

STDMETHODIMP CFormatSpec::get_TypePos(BSTR *pVal)
{
	*pVal = m_bTypePos.copy();

	return S_OK;
}

STDMETHODIMP CFormatSpec::put_TypePos(BSTR newVal)
{
	m_bTypePos = newVal;

	return S_OK;
}

STDMETHODIMP CFormatSpec::get_SQLTypePos(BSTR *pVal)
{
	*pVal = m_bSQLTypePos.copy();

	return S_OK;
}

STDMETHODIMP CFormatSpec::put_SQLTypePos(BSTR newVal)
{
	m_bSQLTypePos = newVal;

	return S_OK;
}
