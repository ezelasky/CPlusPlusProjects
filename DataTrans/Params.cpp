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

CCmndBase::CCmndBase()
{
	m_pICommandText=NULL;
}

CCmndBase::~CCmndBase()
{
	if(HasCmnd()) 
		CloseCmnd();
}

void CCmndBase::CloseCmnd()
{
	if(m_pICommandText)	
	{
		m_pICommandText->Release();
		m_pICommandText=NULL;
	}
}

bool CCmndBase::OpenCmnd(IOpenRowset *pIOpenRowset)
{
	OleDBPro_ASSERT(pIOpenRowset, "\nMake sure that pIOpenRowset is wired with a TSession object");
	if(HasCmnd()) CloseCmnd();

	CComPtr<IDBCreateCommand> pIDBCreateCommand;
	m_hr=pIOpenRowset->QueryInterface(__uuidof(IDBCreateCommand), (void**)&pIDBCreateCommand);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIOpenRowset, __uuidof(IOpenRowset), &m_strError);
		return false;
	}
	m_hr =pIDBCreateCommand->CreateCommand(NULL, __uuidof(ICommandText), (IUnknown **)&m_pICommandText);
	if(FAILED(m_hr))
	{	
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIDBCreateCommand, __uuidof(IDBCreateCommand), &m_strError);
		return false;
	}
	OnTCommandCreated(m_pICommandText);
	return true;
}
/*
bool CCmndBase::ExecuteSQL(LPCOLESTR strSQL)
{
	OleDBPro_ASSERT(HasCmnd(), "\nMake sure that a command is available!");
	OleDBPro_ASSERT(strSQL, "\nstrSQL can't be NULL!");
	OleDBPro_ASSERT(wcslen(strSQL), "\nstrSQL can't be zero in length!");
	
	m_hr=m_pICommandText->SetCommandText(DBGUID_DEFAULT, strSQL);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pICommandText, __uuidof(ICommandText), &m_strError);
		return false;
	}
	m_hr=m_pICommandText->Execute(NULL, IID_NULL, NULL, NULL, NULL);
	if (FAILED(m_hr))
	{
		if(m_pGetErrorInfo)
			m_pGetErrorInfo(m_pICommandText, __uuidof(ICommandText), &m_strError);
		return false;
	}
	return true;	
}*/

bool CCmndBase::GetSession(COSession *pOSession)
{
	OleDBPro_ASSERT(pOSession, "\nMake sure that pOSession is pointing to a COSession object and can't be NULL!");
	OleDBPro_ASSERT(HasCmnd(), "\nMake sure that this object is already opened!");
	if(pOSession->IsOpen()) 
		pOSession->Close();
   pOSession->m_hr=m_pICommandText->GetDBSession(pOSession->GetBaseIID(), pOSession->GetBaseInterfacePPtr());
   if(FAILED(pOSession->m_hr))
   {
      if(pOSession->m_pGetErrorInfo) 
			pOSession->m_pGetErrorInfo(m_pICommandText, __uuidof(ICommandText), &(pOSession->m_strError));
      return false;
   }
   return true;
}

bool	CCmndBase::Cancel()
{
	if(!m_pICommandText) 
		return true;
	m_hr=m_pICommandText->Cancel();
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pICommandText, __uuidof(ICommandText), &m_strError);
		return false;
	}
	return true;
}

void	CCmndBase::OnTCommandCreated(ICommandText	*pICommandText)
{

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

