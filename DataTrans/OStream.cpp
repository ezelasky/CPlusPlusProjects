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

COStream::COStream()
{
	m_pISequentialStream=NULL;
}

COStream::~COStream()
{
	if(IsOpen())
		Close();
}

bool COStream::GetSourceRow(CORow *pORow)
{
	OleDBPro_ASSERT(pORow, "\npORow can't be NULL!");
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this stream object is already opened!");
	CComPtr<IGetSourceRow>		pIGetSourceRow;
	if(pORow->IsOpen())
		pORow->Close();
	pORow->m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IGetSourceRow), (void**)&pIGetSourceRow);
	if(FAILED(pORow->m_hr))
	{
		if(pORow->m_pGetErrorInfo) 
			pORow->m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &(pORow->m_strError));
		return false;
	}
	pORow->m_hr=pIGetSourceRow->GetSourceRow(pORow->GetBaseIID(), pORow->GetBaseInterfacePPtr());
	if(FAILED(pORow->m_hr))
	{
		if(pORow->m_pGetErrorInfo) 
			pORow->m_pGetErrorInfo(pIGetSourceRow, __uuidof(IGetSourceRow), &(pORow->m_strError));
		return false;
	}
	return true;
}

bool	COStream::GetSession(COSession *pOSession)
{
	OleDBPro_ASSERT(pOSession, "\npORow can't be NULL!");
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this stream object is already opened!");
	CORow	ORow;
	if(pOSession)
		pOSession->Close();
	if(!GetSourceRow(&ORow))
	{
		pOSession->m_hr=ORow.m_hr;
		if(pOSession->m_strError)
			::SysFreeString(pOSession->m_strError);
		pOSession->m_strError=ORow.m_strError;
		ORow.m_strError=NULL;
		return false;
	}
	return ORow.GetSession(pOSession);
}

void	COStream::Close()
{
	if(m_pISequentialStream)
	{
		m_pISequentialStream->Release();
		m_pISequentialStream=NULL;
	}
}
	
bool	COStream::GetData(void* pStreamBuffer, ULONG *pnGet)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this stream object is already opened!");
	OleDBPro_ASSERT(pStreamBuffer, "\npStreamBuffer can't be NULL!");
	OleDBPro_ASSERT(pnGet, "\npnGet can't be NULL!");

	m_hr=m_pISequentialStream->Read(pStreamBuffer, *pnGet, pnGet);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pISequentialStream, __uuidof(ISequentialStream), &m_strError);
		return false;
	}
	return true;
}

REFIID COStream::GetBaseIID()
{
	return __uuidof(ISequentialStream);
}

bool COStream::Commit(DWORD nCommitFlags)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this stream object is already opened!");
	CComPtr<IStream>	pIStream;
	m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IStream), (void**)&pIStream);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &m_strError);
		return false;
	}
	m_hr=pIStream->Commit(nCommitFlags);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(pIStream, __uuidof(IStream), &m_strError);
		return false;
	}
	return true;
}

bool COStream::PostProcess()
{
	return true;
}

bool	COStream::PutData(void* pStreamBuffer, ULONG* pnPut)
{
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this stream object is already opened!");
	OleDBPro_ASSERT(pStreamBuffer, "\npStreamBuffer can't be NULL!");
	OleDBPro_ASSERT(pnPut, "\npnPut can't be NULL!");
	
	m_hr=m_pISequentialStream->Write(pStreamBuffer, *pnPut, pnPut);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pISequentialStream, __uuidof(ISequentialStream), &m_strError);
		return false;
	}
	return true;
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