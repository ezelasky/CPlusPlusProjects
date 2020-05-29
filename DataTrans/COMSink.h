#pragma once
#import "dtspkg.dll"  named_guids rename("EOF", "dtsEOF")  no_namespace

#if !defined
#include	<initguid.h>
#include "COMDEF.h"
#include "TCHAR.h"
#include <iostream>
#include <ole2.h>
#include <conio.h>
#include <io.h>
#include	<olectl.h>
#endif


#if !defined
#include "Utilities.h"
#endif

#define		BAD_COOKIE 99999


/*  CCOMSink -- This is a COM sink class, which is a call back interface to the 
	DTS COM coclass and allows the the coclass to fire events as they occur, which 
	allows us to grab intermeidiate error messages occurring at the step level and if we choose 
	to handle the errorrs.  Currently all errors are collected and passed back to the client.
*/

class CCOMSink :  public PackageEvents
{
public:
	CCOMSink(void);
	virtual ~CCOMSink(void);
	ERRLIST	m_listErrors;

	// IUnknown
    STDMETHOD(QueryInterface)(REFIID riid, LPVOID FAR* ppvObj)
	{
		if (riid == IID_IUnknown || riid == IID_IDispatch || riid == __uuidof(PackageEvents)) 
		{
			AddRef();
			*ppvObj = this;
			return NOERROR;
		} 
		*ppvObj = NULL;
		return E_NOINTERFACE;
	}

	STDMETHOD_(ULONG, AddRef)(void)
	{
		return InterlockedIncrement((LPLONG)&m_uiRefCount);
	}

	STDMETHOD_(ULONG, Release)(void)
	{
		if (InterlockedDecrement((LPLONG)&m_uiRefCount) == 0) 
		{
			delete this;
			return 0;
		}
		return m_uiRefCount;
	}

	// IDispatch not implemented, and not required by Package.
    STDMETHOD(GetTypeInfoCount)(UINT FAR* pcTypeInfo)
	{
		*pcTypeInfo = 0;
		return E_NOTIMPL;
	}

    STDMETHOD(GetTypeInfo)(UINT iTypeInfo, LCID lcid, ITypeInfo FAR* FAR* ppTypeInfo)
	{
		*ppTypeInfo = NULL;
		return E_NOTIMPL;
	}

	STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR FAR* rgszNames, UINT cNames, LCID lcid, DISPID FAR* rgdispid)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags,	DISPPARAMS FAR* pdispparams, LPVARIANT pVarResult, LPEXCEPINFO pexcepinfo, UINT FAR* puArgErr)
	{
		return E_NOTIMPL;
	}

	// EventSink methods.
	//
	STDMETHOD(raw_OnStart)(BSTR EventSource) { 
		_bstr_t bSource = EventSource;
		return NOERROR; }

	STDMETHOD(raw_OnFinish)(BSTR EventSource){ 
		_bstr_t bSource = EventSource;
		return NOERROR; }

	STDMETHOD(raw_OnError)(BSTR EventSource,
		IN long ErrorCode,
		IN BSTR Source,
		IN BSTR Description,
		IN BSTR HelpFile,
		IN long HelpContext,
		IN BSTR IDofInterfaceWithError,
		OUT VARIANT_BOOL *pbCancel)
	{
		// We don't want to cancel in this utility.
		//
		_bstr_t bstrError = Utilities::ConstructErrMsg(L"DTS ERROR:", Source, Description, L"");
		m_listErrors.push_back(bstrError); 
		if (pbCancel)
		{
			*pbCancel = VARIANT_FALSE;
		}

		return NOERROR;
	}

	STDMETHOD(raw_OnProgress)(BSTR EventSource,
		IN BSTR ProgressDescription,
		IN long PercentComplete,
		IN long ProgressCountLow,
		IN long ProgressCountHigh)
	{
		//LARGE_INTEGER liProgressCount;
		//liProgressCount.LowPart = ProgressCountLow;
		//liProgressCount.HighPart = ProgressCountHigh;
		
		return NOERROR;
	}

	STDMETHOD(raw_OnQueryCancel)(BSTR EventSource,
		OUT VARIANT_BOOL *pbCancel)
	{
		if (pbCancel)
		{
			*pbCancel = VARIANT_FALSE;
		}
		return NOERROR;
	}

	private:
	UINT m_uiRefCount;
};
