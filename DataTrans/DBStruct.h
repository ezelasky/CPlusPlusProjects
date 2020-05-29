// This is a part of the OleDBPro module.
// Copyright (C) 2000-2000 UDAParts 
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
// Copyright (c) 2000 by Yuancai (Charlie) Ye
// http://www.geocities.com/OleDBPro/index.htm
// yekerui@yahoo.com

#ifndef	OLEDB_STRUCTURES_H
#define	OLEDB_STRUCTURES_H

#include <oledb.h>
#include <msdaguid.h>
#include <msdasc.h>

#define	DEFAULT_ROWSET_SIZE		20
#define DEFAULT_BATCH_SIZE		DEFAULT_ROWSET_SIZE
#define	OLEDB_STRING_END		(~0)
#define	DEFAULT_OUPUT_LEN		255
#define	OLEDB_PARAM_BLOB		OLEDB_STRING_END
#define	STRING_NULL_TERMINATD	OLEDB_STRING_END
			
class CRefInfo
{
public:
	CRefInfo()
	{
		m_strTblName=NULL;
		m_nCols=0;
		m_pColNames=NULL;
	}
	LPOLESTR	m_strTblName;	//table name
	ULONG		m_nCols;				
	LPOLESTR	*m_pColNames;	//pointing to m_nCols column names
};

class CColInfo
{
public:
	CColInfo()
	{
		memset(this, 0, sizeof(CColInfo));
	}
	LPOLESTR	m_strColName;
	ULONG		m_nChars;		//in chars for DBTYPE_STR, DBTYPE_WSTR and DBTYPE_BYTES only, not in bytes
	DBPROPSET	*m_pDBPropSet;
	DBTYPE		m_nDBType;		//must be set
	BYTE		m_nPrecision;	//DBTYPE_NUMERIC or DBTYPE_VARNUMERIC only		
	BYTE		m_nScale;		//DBTYPE_NUMERIC or DBTYPE_VARNUMERIC or DBTYPE_DECIMAL only	
	LPOLESTR	m_strTypeName;
};

class CPInfo
{
public:
	CPInfo()
	{
		m_nDBType=DBTYPE_EMPTY;
		m_nDBParamIO=DBPARAMIO_INPUT;
		m_nLen=0;
		m_strPName=NULL;
		m_nPrecision=0;
		m_nScale=0;
	}
public:
	DBTYPE		m_nDBType;		//must be set										
	DBPARAMIO	m_nDBParamIO;	//must be set, default to be DBPARAMIO_INPUT
	ULONG		m_nLen;			//used only for variable-length data types, in bytes
	LPOLESTR	m_strPName;		//name of the parameter. 
	BYTE		m_nPrecision;	//DBTYPE_NUMERIC or DBTYPE_VARNUMERIC only
	BYTE		m_nScale;		//DBTYPE_NUMERIC or DBTYPE_VARNUMERIC or DBTYPE_DECIMAL only
};

class CPInfoEx : public CPInfo
{
public:
	CPInfoEx ()
	{
		m_pData=NULL;
	}
public:
	void	*m_pData;			
};

class CPData
{
public:
	CPData()
	{
		m_pData=NULL;                          
		m_nLen=OLEDB_STRING_END;
		m_nDBStatus=DBSTATUS_S_OK;		
	}

public:
	ULONG		m_nLen;			//If data type is fixed-length, it is ignored.
								//If data type is DBTYPE_BYTES or DBTYPE_VARNUMERIC, must set m_nLen.
								//If data type is DBTYPE_STR or DBTYPE_WSTR, prefers setting m_nLen. 
	DBSTATUS	m_nDBStatus;	//Reserved!!!
	void		*m_pData;				
};

class CKeyInfo : public CPData
{
public:
	CKeyInfo()
	{
		m_nCol=0;
		m_nDBType=DBTYPE_NULL;
		m_nPrecision=0;
		m_nScale=0;
	}
	ULONG	m_nCol;			//1-indexed
	DBTYPE	m_nDBType;		//must be set
	BYTE	m_nPrecision;	//DBTYPE_NUMERIC or DBTYPE_VARNUMERIC only
	BYTE	m_nScale;		//DBTYPE_NUMERIC or DBTYPE_VARNUMERIC or DBTYPE_DECIMAL only
};

class CFindInfo : public CKeyInfo
{
public:
	CFindInfo()
	{
		m_nOp=DBCOMPAREOPS_EQ;
	}
	DBCOMPAREOP		m_nOp;
};

class CFilterInfo	: public CFindInfo
{
public:	
	CFilterInfo()
	{
		m_bAnd=true;
	}
	bool	m_bAnd;	//AND or OR
};

#endif

// This is a part of the OleDBPro module.
// Copyright (C) 2000-2000 UDAParts 
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
// Copyright (c) 2000 by Yuancai (Charlie) Ye
// http://www.geocities.com/OleDBPro/index.htm
// yekerui@yahoo.com