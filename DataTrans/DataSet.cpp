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

#include "OleDBBas.h"
#include "Odbassert.h"

CODataset::CODataset()
{
	m_pIMDDataset=NULL;
	m_nAxes=0;
   m_pAxisInfo=NULL;
}

CODataset::~CODataset()
{
	if(IsOpen())
		Close();
}

REFIID CODataset::GetBaseIID()
{
	return __uuidof(IMDDataset);
}

void	CODataset::Close()
{
	if(m_pIMDDataset)
	{
		if(m_pAxisInfo)
		{
			ATLASSERT(m_nAxes);
			m_pIMDDataset->FreeAxisInfo(m_nAxes, m_pAxisInfo);
			m_pAxisInfo=NULL;
		}
		m_pIMDDataset->Release();
		m_pIMDDataset=NULL;
	}
	if(CRBase::IsOpen())
		CRBase::Close();
}

bool	CODataset::GetRangeRowset(CRBase *pRBase, ULONG nStartCell, ULONG nEndCell, ULONG cPropSets, DBPROPSET *pPropSets)
{
	OleDBPro_ASSERT(pRBase, "\nstrConnection can't be NULL!");
	OleDBPro_ASSERT(IsOpen(), "\nMake sure that this object is already opened!");
	if(pRBase->IsOpen())
		pRBase->Close();
	CComPtr<IMDRangeRowset> pIMDRangeRowset;
	pRBase->m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IMDRangeRowset), (void**)&pIMDRangeRowset);
	if(FAILED(pRBase->m_hr))
	{
		if(pRBase->m_pGetErrorInfo) 
			pRBase->m_pGetErrorInfo((*GetBaseInterfacePPtr()), GetBaseIID(), &(pRBase->m_strError));
		return false;
	}
	pRBase->m_hr=pIMDRangeRowset->GetRangeRowset(NULL, nStartCell, nEndCell, pRBase->GetBaseIID(), cPropSets, pPropSets, pRBase->GetBaseInterfacePPtr());
	if(FAILED(pRBase->m_hr))
	{
		if(pRBase->m_pGetErrorInfo) 
			pRBase->m_pGetErrorInfo(pIMDRangeRowset, __uuidof(IMDRangeRowset), &(pRBase->m_strError));
		return false;
	}
	return pRBase->PostProcess();
}


bool	CODataset::GetAxisRowset(CRBase *pRBase, ULONG iAxis, ULONG cPropSets, DBPROPSET *pPropSets)
{
	OleDBPro_ASSERT(pRBase, "\npRBase can't be NULL!");
	OleDBPro_ASSERT(m_pIMDDataset, "\nMake sure that this object is already opened!");
	if(pRBase->IsOpen())
		pRBase->Close();
	pRBase->m_hr=m_pIMDDataset->GetAxisRowset(NULL, iAxis, pRBase->GetBaseIID(), cPropSets, pPropSets, pRBase->GetBaseInterfacePPtr());
	if(FAILED(pRBase->m_hr))
	{
		if(pRBase->m_pGetErrorInfo) 
			pRBase->m_pGetErrorInfo(m_pIMDDataset, __uuidof(IMDDataset), &(pRBase->m_strError));
		return false;
	}
	return pRBase->PostProcess();
}

bool CODataset::OpenWithCmnd(IOpenRowset *pIOpenRowset, LPCOLESTR strSelect, DBPROPSET *pDBPropSet, ULONG nPropSets, REFGUID guidDialect)
{
	OleDBPro_ASSERT(pIOpenRowset, "\npIOpenRowset can't be NULL!");
	CComPtr<ICommandProperties>	pICommandProperties;
	if(IsOpen())
		Close();
	if(HasCmnd())
		CloseCmnd();
	if(!OpenCmnd(pIOpenRowset))
		return false;
	m_hr = m_pICommandText->SetCommandText(DBGUID_MDX, strSelect);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pICommandText, __uuidof(ICommandText), &m_strError);
		return false;
	}
	m_hr = m_pICommandText->QueryInterface(__uuidof(ICommandProperties), (void**)&pICommandProperties);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pICommandText, __uuidof(ICommandText), &m_strError);
		return false;
	}
	if(pDBPropSet)
		m_hr=pICommandProperties->SetProperties(nPropSets, pDBPropSet);
	m_hr=m_pICommandText->Execute(NULL, GetBaseIID(), NULL, &m_nRowsAffected, GetBaseInterfacePPtr());
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pICommandText, __uuidof(ICommandText), &m_strError);
		return false;
	}
	return PostProcess();
}

bool CODataset::PostProcess()
{
	OleDBPro_ASSERT(m_pIMDDataset, "\nCalling this function is permitted only after the interface IMDDataset is available!");
	if(m_pAxisInfo)
	{
		m_pIMDDataset->FreeAxisInfo(m_nAxes, m_pAxisInfo);
		m_pAxisInfo=NULL;
	}
	m_hr=m_pIMDDataset->GetAxisInfo(&m_nAxes, &m_pAxisInfo);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIMDDataset, __uuidof(IMDDataset), &m_strError);
		Close();
		return false;
	}
	SetRowsObtained();
	m_nRowsAffected=m_nRowsObtained;
	m_hr=m_pIMDDataset->QueryInterface(CRBase::GetBaseIID(), (void**) CRBase::GetBaseInterfacePPtr());
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIMDDataset, __uuidof(IMDDataset), &m_strError);
		Close();
		return false;
	}
	m_nRowsetSize=1;
	return QueryColsInfo();
}

bool   CODataset::GetSession(COSession *pOSession)
{
	return CCmndBase::GetSession(pOSession);
}

bool	CODataset::MoveTo(ULONG *pnTupleOrdinals)
{
	OleDBPro_ASSERT(pnTupleOrdinals, "\npnTupleOrdinals can't be NULL!");
	ULONG	nProduct=1;
	ULONG	nCellOrdinal=0;
	for(ULONG nAxis=0; nAxis<m_nAxes-1; nAxis++)
	{
		OleDBPro_ASSERT(pnTupleOrdinals[nAxis]<m_pAxisInfo[nAxis].cCoordinates, "\neach of tuple ordinals must be less than the number of the coordinates on an axis!");
		nCellOrdinal +=pnTupleOrdinals[nAxis]*nProduct;
		nProduct *=m_pAxisInfo[nAxis].cCoordinates;
	}
	return MoveTo(nCellOrdinal);
}

bool	CODataset::MoveTo(ULONG nCellOrdinal)
{
	OleDBPro_ASSERT(nCellOrdinal<GetCells(), "\neach of cell ordinals must be less than the number of obtained cells!");
	m_nCurrentRow=nCellOrdinal;
	return GetDataFromServer(m_nCurrentRow);
}

bool	CODataset::GetDataFromServer(HROW hRow)
{
	if(hRow>=m_nRowsObtained)
	{
		m_hCurrent=0;
		return false;
	}
	m_hr=m_pIMDDataset->GetCellData(m_hAccessor, hRow, hRow, m_pData);
	if(FAILED(m_hr))
	{
		if(m_pGetErrorInfo) 
			m_pGetErrorInfo(m_pIMDDataset, __uuidof(IMDDataset), &m_strError);
		return false;
	}
	m_hCurrent=1;
	return true;
}

bool	CODataset::MoveFirst()
{
	m_nCurrentRow=0;
	return GetDataFromServer(m_nCurrentRow);
}

bool	CODataset::MoveLast()
{
	m_nCurrentRow=m_nRowsObtained-1;
	return GetDataFromServer(m_nCurrentRow);
}

bool	CODataset::MovePrev()
{
	m_nCurrentRow--;
	return GetDataFromServer(m_nCurrentRow);
}

bool	CODataset::MoveNext(LONG nSkipped)
{
	m_nCurrentRow +=(nSkipped+1);
	return GetDataFromServer(m_nCurrentRow);
}

void CODataset::SetRowsObtained()
{
	m_nRowsObtained=1;
	for(ULONG nAxis=0; nAxis<(m_nAxes-1); nAxis++)
		m_nRowsObtained *=m_pAxisInfo[nAxis].cCoordinates;
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