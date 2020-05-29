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
// http://www.geocities.com/oledbpro/index.htm
// yekerui@yahoo.com

#ifndef _ROWSET_TO_XML_TRANSFORMATION_H
#define _ROWSET_TO_XML_TRANSFORMATION_H

#include "OLEDBBas.h"

/*
    Template CR2XML is designed for transforming a rowset or OLAP dataset into 
    a XML file efficiently, generically and flexibly. It automatically takes care of 
	XML reserved chars such as <, &, >, ", and ', and disagreement between OLEDB 
	data conversion and XML data format. The output string class could be CString/MFC, 
	_bstr_t/COM, CComBSTR/COM, AnsiString/VCL, WideString/VCL, and other string classes 
	which can handle char/wchar conversion properly.

    All of the following declarations, as examples, are corect.
    
    //convert an OLAP data set into XML
    CR2XML<CODataset> ODataset2XML;

    //convert a chapter record into XML
    CChapterRecord<COTable<CR2XML<CRBaseEx> > > ChapterRecord2XML;

    //convert multiple rowsets from a parameterized statement/stored procedure
    //into XML
    CMultiBulkRecord<CBulkRecordParam<CR2XML<CRBase> > > MultiBulkRecordParam2XML;
	
    //and many more
    .......

*/

//1. template TString could be 
//   CString, _bstr_t, CComBSTR, AnsiString, WideString and any other string classes 
//   which can handle char/wchar conversion properly.

//2. TRBase can be either CRBase or CODataset derived class.

//3. handle XML Reserved characters such as <, &, >, ", and '.

//4. handle disagreement between OLEDB data conversion and XML data format.

//5. handle children's XML data

template <class TRBase = CRBase, class TToXML = CToXML>
class CR2XML : public TRBase, public TToXML
{ 
public:
    CR2XML()
    {
        m_bElements = true;
		m_bAfter = true;
    }
public:
	virtual DBTYPE	GetRawDataType(ULONG nCol)
	{
		return TRBase::GetRawDataType(nCol);
	}
	virtual ULONG	GetCols()
	{
		return TRBase::GetCols();
	}
	virtual bool	GetDataEx(ULONG nCol, void *pBuffer, DBTYPE nDBType, ULONG nSize=0, BYTE nPrecision=0, BYTE nScale=0)
	{
		return TRBase::GetDataEx(nCol, pBuffer, nDBType, nSize, nPrecision, nScale);
	}
    inline void SetRowsObtained(ULONG nRowsObtained)
    {
        m_nRowsObtained = nRowsObtained;
    }

    //  template TString could be 
    //  CString, _bstr_t, CComBSTR, AnsiString, WideString and any other string classes 
    //  which can handle char/wchar conversion properly.
    template <class TString>
    void GetSchema(TString& strSchema, LPCTSTR strNameAttr=NULL)
    {
        ULONG nCol;
        //must set m_strRowTag first!
        ATLASSERT(m_strRowTag.Length());   
        strSchema = OLESTR("<Schema ");
        if(strNameAttr)
        {
            strSchema += strNameAttr;
            strSchema += OLESTR(" ");
        }
        strSchema += OLESTR("xmlns=\"urn:schemas-microsoft-com:xml-data\" xmlns:dt=\"urn:schemas-microsoft-com:datatypes\">");
        strSchema += OLESTR("<ElementType name=\"");
        strSchema += (LPCOLESTR)m_strRowTag;
        strSchema += OLESTR("\" content=\"");
        if(m_bElements)
            strSchema += OLESTR("eltOnly");
        else
            strSchema += OLESTR("empty");
        strSchema += OLESTR("\" model=\"");
        if(m_bElements)
            strSchema += OLESTR("open");
        else
            strSchema += OLESTR("closed");
        strSchema += OLESTR("\">");
        for(nCol=1; nCol<=(m_nCol-IsBookmarked()); nCol++)
        {
            LPCTSTR strDTType = GetXMLStringDataType(nCol);
            if(GetDBPart(nCol) && strDTType)
            {
                strSchema += OLESTR("<AttributeType name=\"");
                strSchema += GetColName(nCol);
                strSchema += OLESTR("\" dt:type=\"");
                strSchema += strDTType;
                strSchema += OLESTR("\"/>");
            }
        }
        for(nCol=1; nCol<=(m_nCol-IsBookmarked()); nCol++)
        {
            if(GetDBPart(nCol) && GetXMLStringDataType(nCol))
            {
                strSchema += OLESTR("<Attribute Type=\"");
                strSchema += GetColName(nCol);
                strSchema += OLESTR("\"/>");
            }
        }
        strSchema += OLESTR("</ElementType></Schema>"); 
    }

    //  template TString could be 
    //  CString, _bstr_t, CComBSTR, AnsiString, WideString and any other string classes 
    //  which can handle char/wchar conversion properly.
    template <class TString, class TChar>
	bool GetXMLRow(TString& strXMLData, const TChar * strRowAttr, HROW hRow=0) 
	{
        //must set m_strRowTag first!
        ATLASSERT(m_strRowTag.Length());
		
        ULONG   nCol;
        ULONG   nCount=0;
        TString strXMLField;
		TString	strTemp;
        ATLASSERT(IsOpen());
        ATLASSERT(m_pDBColumnInfo);
        if(hRow && !GetDataFromServer(hRow))
            return false;   
		strXMLData = OLESTR("<");
        strXMLData += (LPCWSTR)m_strRowTag;
        if(strRowAttr)
        {
            strXMLData +=OLESTR(" ");
            strXMLData +=strRowAttr;
        }  

        if(m_bElements)
        {
            for(nCol=1; nCol<=(m_nCol-IsBookmarked()); nCol++)
            {
                if(GetDBPart(nCol) && GetStatus(nCol)==DBSTATUS_S_OK && GetDataType(nCol)!=DBTYPE_HCHAPTER)
                {
                    if(!nCount)
					{
                        strXMLData += OLESTR(">");
						if(!m_bAfter && m_strChildren.Length())
							strXMLData += (LPCWSTR)m_strChildren;
						nCount++;
					}
                    if(!GetXMLField(strXMLField, nCol, GetColName(nCol)))
						return false;
                    strTemp += strXMLField;
                }
            }
            if(nCount || m_strChildren.Length() )
            {
				strXMLData +=strTemp;
				if(m_bAfter && m_strChildren.Length())
					strXMLData += (LPCWSTR)m_strChildren;
                strXMLData += OLESTR("</");
                strXMLData += (LPCWSTR)m_strRowTag;
                strXMLData += OLESTR(">");
            }
            else
                strXMLData += OLESTR("/>");
        }
        else
        {
            for(nCol=1; nCol<=(m_nCol-IsBookmarked()); nCol++)
            {
                if(GetDBPart(nCol) && GetStatus(nCol)==DBSTATUS_S_OK && GetDataType(nCol)!=DBTYPE_HCHAPTER)
                {
                    strXMLData +=OLESTR(" ");
                    if(!GetXMLField(strXMLField, nCol, GetColName(nCol)))
						return false;
                    strXMLData += strXMLField;
                }
            }
			if(m_strChildren.Length())
			{
				strXMLData += OLESTR(">");
				strXMLData += (LPCWSTR)m_strChildren;
				strXMLData += OLESTR("</");
                strXMLData += (LPCWSTR)m_strRowTag;
                strXMLData += OLESTR(">");
			}
			else
				strXMLData +=OLESTR("/>");
        }
		return true;
	}
	template <class TString>
    bool GetXMLField(TString& strXMLData, ULONG nCol, LPCOLESTR strTag)
    {
        ATLASSERT(IsOpen());	//make sure that a rowset is available!
		ATLASSERT(nCol && nCol<=m_nCol-IsBookmarked()); //make sure that a column ordinal is correct!
        ATLASSERT(strTag);
        TString strField;
        if(!GetField(strField, nCol))
			return false;
        if(m_bElements)
        {
            strXMLData = OLESTR("<");
            strXMLData += TString(strTag);
            strXMLData += OLESTR(">");
            strXMLData += strField;
            strXMLData += OLESTR("</");
		    strXMLData += TString(strTag);
		    strXMLData += OLESTR(">");
        }
        else
        {
            strXMLData = TString(strTag);
            strXMLData +=OLESTR("=\"");
            strXMLData +=strField;
            strXMLData +=OLESTR("\"");
        }
		return true;
    }
protected:
    template <class TString>
    bool GetField(TString& strField, ULONG nCol)
    {
        strField=OLESTR("");
        DBTYPE  nDBType=GetDataType(nCol);
        ATLASSERT((nDBType & DBTYPE_BYREF) != DBTYPE_BYREF);   //can't use DBTYPE_BYREF
        if(GetDBPart(nCol) && nDBType!=DBTYPE_HCHAPTER)
        { 
            void *pBuffer=GetData(nCol);
            if(IsBLOB(nCol))
            {
                if(GetStatus(nCol)==DBSTATUS_S_OK)
                {
                    ULONG nLen=GetLength(nCol);
                    void *pBLOB;
					DBTYPE nDBType=GetRawDataType(nCol);
                    if(nDBType==DBTYPE_STR)
                    {
                        pBLOB=::CoTaskMemAlloc(nLen+1);
                        if(pBLOB && GetDataEx(nCol, pBLOB, DBTYPE_STR, nLen+1))
                        {
                            char *str = (char*)pBLOB;
                            ((char*)pBLOB)[nLen]=0;
                            Chars2DT(&str);
                            strField = str;
                            if(str != pBLOB)
                                ::CoTaskMemFree(str);
                        }
                        else
							return false;
                    }
                    else if(nDBType==DBTYPE_WSTR)
                    {
                        pBLOB=::CoTaskMemAlloc(nLen+2);
                        if(pBLOB && GetDataEx(nCol, pBLOB, DBTYPE_WSTR, nLen+2))
                        {
                            wchar_t  *str = (wchar_t*)pBLOB;
                            ((wchar_t*)pBLOB)[nLen/sizeof(WCHAR)]=0;
                            WChars2DT(&str);
                            strField = str;
                            if(str != pBLOB)
                                ::CoTaskMemFree(str);
                        }
                        else
							return false;
                    }
                    else    //DBTYPE_BYTES
                    {
                        pBLOB=::CoTaskMemAlloc(nLen*2+1);
                        if(pBLOB && GetDataEx(nCol, pBLOB, DBTYPE_STR, nLen*2+1))
                        {
                            ((char*)pBLOB)[nLen*2]=0;
                            strField = (LPCSTR)pBLOB;
                        }
                        else
							return false;
                    }
                    if(pBLOB)
                        ::CoTaskMemFree(pBLOB);
                }
            }
            else if(nDBType==DBTYPE_STR)
            {
                DBTYPE nOriDBType=GetRawDataType(nCol);
				if (nOriDBType == DBTYPE_STR)
                {
                    char *str = (char*)pBuffer;
                    Chars2DT(&str);
                    strField = str; 
                    if(pBuffer != str)
                        ::CoTaskMemFree(str);
                }
                else
				{
                    ODB2XMLConversion(nCol, (char*)pBuffer);
                    strField = (LPCSTR)pBuffer; 
                }
            }
            else if(nDBType == DBTYPE_WSTR)
            {
                DBTYPE nOriDBType=GetRawDataType(nCol);
                if (nOriDBType == DBTYPE_WSTR)
                {
					wchar_t *str = (wchar_t*)pBuffer;
                    WChars2DT(&str);
					strField = TString((LPCWSTR)str);
                    if(str != pBuffer)
                        ::CoTaskMemFree(str);
                }
                else
                {
                    ATLASSERT(FALSE); //data type wrong!
                }
            }
            else
            {
                //other data types forbidden
                ATLASSERT(FALSE);   
            } 
        }
		return true;
    }
    
    //force OLEDB to do data conversion
    virtual bool QueryColsInfo()
    {
        if(!TRBase::QueryColsInfo())
        {
            Close();
            return false;
        }
		CreateArrayOfXMLDataTypes();
        ULONG nCols=GetCols();
        bool bOK;
		for(ULONG nCol=1; nCol<=nCols; nCol++)
		{
			DBTYPE nDBType=GetDataType(nCol);
			switch (nDBType)
			{
            case DBTYPE_BOOL:		
				SetXMLDataType(nCol, XML_Boolean); //default to xml boolean
				bOK=SetDataType(nCol, DBTYPE_STR, 6);//true or false
				break;
            case DBTYPE_UI1:
				SetXMLDataType(nCol, XML_Ui1);	//default to xml ui1
				bOK=SetDataType(nCol, DBTYPE_STR, 5);
				break;
			case DBTYPE_I1:
				SetXMLDataType(nCol, XML_I1);
                bOK=SetDataType(nCol, DBTYPE_STR, 5);
                break;
			case DBTYPE_I2:
				SetXMLDataType(nCol, XML_I2);
				bOK=SetDataType(nCol, DBTYPE_STR, 7);
				break;
            case DBTYPE_UI2:
				SetXMLDataType(nCol, XML_Ui2);
                bOK=SetDataType(nCol, DBTYPE_STR, 7);
                break;
            case DBTYPE_I4:
				SetXMLDataType(nCol, XML_I4);
				bOK=SetDataType(nCol, DBTYPE_STR, 12);
				break;
            case DBTYPE_UI4:
				SetXMLDataType(nCol, XML_Ui4);
                bOK=SetDataType(nCol, DBTYPE_STR, 12);
                break;
			case DBTYPE_I8:
				SetXMLDataType(nCol, XML_Number);
                bOK=SetDataType(nCol, DBTYPE_STR, 21);
                break;
			case DBTYPE_UI8:
				SetXMLDataType(nCol, XML_Number);
                bOK=SetDataType(nCol, DBTYPE_STR, 22);
                break;
			case DBTYPE_R4:
				SetXMLDataType(nCol, XML_R4);
                bOK=SetDataType(nCol, DBTYPE_STR, 17);
                break;
            case DBTYPE_R8:
				SetXMLDataType(nCol, XML_R8);
                bOK=SetDataType(nCol, DBTYPE_STR, 18);
                break;
			case DBTYPE_CY:
				SetXMLDataType(nCol, XML_Fixed_14_4);
                bOK=SetDataType(nCol, DBTYPE_STR, 20);
                break;
			case DBTYPE_DECIMAL:
				SetXMLDataType(nCol, XML_Number);
                bOK=SetDataType(nCol, DBTYPE_STR, 31);
                break;
			case DBTYPE_NUMERIC:
				SetXMLDataType(nCol, XML_Number);
                bOK=SetDataType(nCol, DBTYPE_STR, 41);
                break;
			case DBTYPE_VARNUMERIC:
				SetXMLDataType(nCol, XML_Number);
				bOK=SetDataType(nCol, DBTYPE_STR, 256);
				break;
			case DBTYPE_DBDATE: 
				SetXMLDataType(nCol, XML_Date);
                bOK=SetDataType(nCol, DBTYPE_STR, 45);
				break;
			case DBTYPE_DBTIME: 
				SetXMLDataType(nCol, XML_Time);
                bOK=SetDataType(nCol, DBTYPE_STR, 45);
				break;
            case DBTYPE_DATE:
			case DBTYPE_FILETIME:
			case DBTYPE_DBTIMESTAMP:
				SetXMLDataType(nCol, XML_DateTime);
				bOK=SetDataType(nCol, DBTYPE_STR, 45);
				break;
			case DBTYPE_GUID:
				SetXMLDataType(nCol, XML_Uuid);
				bOK=SetDataType(nCol, DBTYPE_STR, 40);
				break;
            case DBTYPE_BYTES:
				SetXMLDataType(nCol, XML_Bin_hex);
                if(!IsBLOB(nCol))
                    bOK=SetDataType(nCol, DBTYPE_STR, GetMaxLength(nCol)*2+1); 
                else
                    bOK=true;
                break;
			case DBTYPE_VARIANT:
				SetXMLDataType(nCol, XML_String);
				bOK=SetDataType(nCol, DBTYPE_STR, 256);
				AtlTrace("You may need to set a data type other than DBTYPE_VARIANT at the column %d by overwriting CRBase::OnCreatingAccessors().\n", nCol);
				break;
            //don't need any conversion
            case DBTYPE_WSTR:
			case DBTYPE_STR:
				SetXMLDataType(nCol, XML_String);
			case DBTYPE_HCHAPTER:
                bOK=true;
                break;
            //for others, we will never meet them probably
			default:
				ATLASSERT(FALSE);
                bOK=false;
				break;
			}
            if(!bOK)
            {
                Close();
                return false;
            }
        }
        if(!m_pIRowset && GetBaseIID()==__uuidof(IAccessor)) 
	    {
		    m_hr=(*GetBaseInterfacePPtr())->QueryInterface(__uuidof(IRowset), (void**)&m_pIRowset);
		    if(FAILED(m_hr)) 
		    {
			    if(m_pGetErrorInfo) 
				    m_pGetErrorInfo(*GetBaseInterfacePPtr(), GetBaseIID(), &m_strError);
				m_pIRowset=NULL;
		    }
	    }
		return true;
    }
public:
    //output string format, either element or attribute
    bool        m_bElements; 
    CComBSTR    m_strRowTag;
	
	//put XML data children's records after (true) or before (false)
	//the parent's XML data of a record 
	bool		m_bAfter;
	
	//children's XML data
	CComBSTR	m_strChildren;
};

#endif

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
// http://www.geocities.com/oledbpro/index.htm
// yekerui@yahoo.com