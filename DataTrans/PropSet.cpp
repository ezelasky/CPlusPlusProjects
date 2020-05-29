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

COPropSet::COPropSet()
{
	rgProperties    = NULL;
	cProperties     = 0;
	guidPropertySet=GUID_NULL;
}

COPropSet::COPropSet(const GUID& guid)
{
	rgProperties    = NULL;
	cProperties     = 0;
	guidPropertySet = guid;
}

COPropSet::COPropSet(const COPropSet& OPropSet)
{
	rgProperties    = NULL;
	Copy(OPropSet);
}

COPropSet::~COPropSet()
{
	ReleaseMemories();
}

COPropSet& COPropSet::operator=(const COPropSet& OPropSet)
{
	ReleaseMemories();
	Copy(OPropSet);
	return *this;
}

void COPropSet::AddProperty(DWORD nPropertyID, VARIANT& vtData)
{
	Add();
	rgProperties[cProperties].dwPropertyID   = nPropertyID;
	VariantCopy(&(rgProperties[cProperties].vValue), &vtData);
	cProperties++;
}

void COPropSet::AddProperty(DWORD nPropertyID, LPCSTR strValue)
{
	USES_CONVERSION;
	AddProperty(nPropertyID, A2CW(strValue));
}

void COPropSet::AddProperty(DWORD nPropertyID, LPCWSTR strValue)
{
	USES_CONVERSION;
	Add();
	rgProperties[cProperties].dwPropertyID   = nPropertyID;
	rgProperties[cProperties].vValue.vt      = VT_BSTR;
	rgProperties[cProperties].vValue.bstrVal = ::SysAllocString(W2COLE(strValue));
	cProperties++;
}

void COPropSet::AddProperty(DWORD nPropertyID, bool bValue)
{
	Add();
	rgProperties[cProperties].dwPropertyID   = nPropertyID;
	rgProperties[cProperties].vValue.vt      = VT_BOOL;
	rgProperties[cProperties].vValue.boolVal = (bValue ? VARIANT_TRUE : VARIANT_FALSE);
	cProperties++;
}

void COPropSet::AddProperty(DWORD nPropertyID, BYTE bValue)
{
	Add();
	rgProperties[cProperties].dwPropertyID  = nPropertyID;
	rgProperties[cProperties].vValue.vt     = VT_UI1;
	rgProperties[cProperties].vValue.bVal   = bValue;
	cProperties++;
}

void COPropSet::AddProperty(DWORD nPropertyID, short nValue)
{
	Add();
	rgProperties[cProperties].dwPropertyID  = nPropertyID;
	rgProperties[cProperties].vValue.vt     = VT_I2;
	rgProperties[cProperties].vValue.iVal   = nValue;
	cProperties++;
}

void COPropSet::AddProperty(DWORD nPropertyID, long nValue)
{
	Add();
	rgProperties[cProperties].dwPropertyID  = nPropertyID;
	rgProperties[cProperties].vValue.vt     = VT_I4;
	rgProperties[cProperties].vValue.lVal   = nValue;
	cProperties++;
}

void COPropSet::ReleaseMemories()
{
	for (ULONG i = 0; i < cProperties; i++)
		VariantClear(&rgProperties[i].vValue);
	CoTaskMemFree(rgProperties);
	rgProperties=NULL;
	cProperties=0;
}

void COPropSet::Add()
{
	rgProperties = (DBPROP*)CoTaskMemRealloc(rgProperties, (cProperties + 1) * sizeof(DBPROP));
	ATLASSERT(rgProperties);
	rgProperties[cProperties].dwOptions = DBPROPOPTIONS_REQUIRED;
	rgProperties[cProperties].colid     = DB_NULLID;
	rgProperties[cProperties].vValue.vt = VT_EMPTY;
}

void COPropSet::Copy(const COPropSet& OPropSet)
{
	cProperties     = OPropSet.cProperties;
	guidPropertySet = OPropSet.guidPropertySet;
	rgProperties    = (DBPROP*)CoTaskMemRealloc(rgProperties, cProperties * sizeof(DBPROP));
	ATLASSERT(rgProperties);
	memcpy(rgProperties, OPropSet.rgProperties, sizeof(DBPROP)*cProperties);
	for (ULONG i = 0; i < cProperties; i++)
	{
		VariantCopy(&rgProperties[i].vValue, &OPropSet.rgProperties[i].vValue);
	}
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