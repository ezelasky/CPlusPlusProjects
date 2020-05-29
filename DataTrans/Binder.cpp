// This is a part of the OleDBPro module.
// Copyright (C) 2000-2001 UDAParts 
// All rights reserved.
//
// This source code is only intended as a supplement to the
// OleDBPro module Classes Reference and related
// electronic documentation provided with the Module.
// See these sources for detailed information regarding this
// UDAParts product.

// Please don't disclose any source code of the software to any person or entity,
//
// Please don't decompile, disassemble, or reverse engineer any object code of 
// any portion of the software.
//  
// Copyright (c) 2001 by Yuancai (Charlie) Ye
// http://www.geocities.com/OleDBPro/index.htm
// yekerui@yahoo.com

#include "OLEDBBas.h"
#include "Odbassert.h"

COBinder::COBinder()
{
	m_pIBindResource=NULL;
}

COBinder::~COBinder()
{
	if(IsOpen())
		Close();
}

bool	COBinder::SetProperties(ULONG nCount, DBPROPSET *pDBPropSet)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this binder object is already opened!");
	CComPtr<IDBBinderProperties> pIDBBinderProperties;
	m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IDBBinderProperties), (void**)&pIDBBinderProperties);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}
	m_hr=pIDBBinderProperties->SetProperties(nCount, pDBPropSet);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIDBBinderProperties, __uuidof(IDBBinderProperties), &m_strError);
		return false;
	}
	return true;
}

REFIID	COBinder::GetBaseIID()
{
	return __uuidof(IBindResource);
}

bool	COBinder::Open(CLSID clsidBinder)
{
	m_hr=CoCreateInstance(clsidBinder, NULL, CLSCTX_INPROC_SERVER, GetBaseIID(), (void**)GetBaseInterfacePPtr());
	if(FAILED(m_hr))
	{
		if(m_strError)
			::SysFreeString(m_strError);
		m_strError=NULL;
		m_strError=::SysAllocString(OLESTR("Failed in creating binder object."));
		return false;
	}
	return true;
}

void	COBinder::Close()
{
	if(m_pIBindResource)
		m_pIBindResource->Release();
	m_pIBindResource=NULL;
}

// This is a part of the OleDBPro module.
// Copyright (C) 2000-2001 UDAParts 
// All rights reserved.
//
// This source code is only intended as a supplement to the
// OleDBPro module Classes Reference and related
// electronic documentation provided with the Module.
// See these sources for detailed information regarding this
// UDAParts product.

// Please don't disclose any source code of the software to any person or entity,
//
// Please don't decompile, disassemble, or reverse engineer any object code of 
// any portion of the software.
//  
// Copyright (c) 2001 by Yuancai (Charlie) Ye
// http://www.geocities.com/OleDBPro/index.htm
// yekerui@yahoo.com