#include "OLEDBBas.h"
#include "Odbassert.h"

#include <stdio.h>

CToXML::CToXML()
{
	m_pXMLDataType=NULL;
}

CToXML::~CToXML()
{
	if(m_pXMLDataType)
		::CoTaskMemFree(m_pXMLDataType);	
}

void CToXML::WChars2DT(WCHAR **pstrDT)
{
    ATLASSERT(pstrDT && *pstrDT);
    ULONG nPos;
    ULONG nLen = wcslen(*pstrDT);
    wchar_t *pNewBuffer=*pstrDT;
    for(nPos=0; nPos<nLen; nPos++)
    {
        switch (pNewBuffer[nPos])
        {
        case L'<':
            if(pNewBuffer != *pstrDT)
            {
                pNewBuffer = (wchar_t*)::CoTaskMemRealloc(pNewBuffer, (nLen+4)*sizeof(wchar_t));
                memcpy(pNewBuffer + nPos + 4, pNewBuffer + nPos + 1, (nLen-nPos)*sizeof(wchar_t));
                pNewBuffer[nPos] = L'&';
                pNewBuffer[nPos+1] = L'l';
                pNewBuffer[nPos+2] = L't';
                pNewBuffer[nPos+3] = L';';
            }
            else 
            {
                pNewBuffer = (wchar_t*)::CoTaskMemAlloc((nLen + 4)*sizeof(wchar_t));
                memcpy(pNewBuffer, *pstrDT, nPos*sizeof(wchar_t));
                pNewBuffer[nPos] = L'&';
                pNewBuffer[nPos+1] = L'l';
                pNewBuffer[nPos+2] = L't';
                pNewBuffer[nPos+3] = L';';
                memcpy(pNewBuffer+nPos+4, *pstrDT+nPos+1, (nLen-nPos)*sizeof(wchar_t)); 
            }
            nPos +=3;
            nLen +=3; //&lt;
            break;
        case L'>':   //&gt;
            if(pNewBuffer != *pstrDT)
            {
                pNewBuffer = (wchar_t*)::CoTaskMemRealloc(pNewBuffer, (nLen+4)*sizeof(wchar_t));
                memcpy(pNewBuffer + nPos + 4, pNewBuffer + nPos + 1, (nLen-nPos)*sizeof(wchar_t));
                pNewBuffer[nPos] = L'&';
                pNewBuffer[nPos+1] = L'g';
                pNewBuffer[nPos+2] = L't';
                pNewBuffer[nPos+3] = L';';
            }
            else 
            {
                pNewBuffer = (wchar_t*)::CoTaskMemAlloc((nLen + 4)*sizeof(wchar_t));
                memcpy(pNewBuffer, *pstrDT, nPos*sizeof(wchar_t));
                pNewBuffer[nPos] = L'&';
                pNewBuffer[nPos+1] = L'g';
                pNewBuffer[nPos+2] = L't';
                pNewBuffer[nPos+3] = L';';
                memcpy(pNewBuffer+nPos+4, *pstrDT+nPos+1, (nLen-nPos)*sizeof(wchar_t)); 
            }
            nPos +=3;
            nLen +=3; 
            break;
        case L'&': //&amp;
            if(pNewBuffer != *pstrDT)
            {
                pNewBuffer = (wchar_t*)::CoTaskMemRealloc(pNewBuffer, (nLen+5)*sizeof(wchar_t));
				memcpy(pNewBuffer + nPos + 5, pNewBuffer + nPos + 1, (nLen-nPos)*sizeof(wchar_t));
                pNewBuffer[nPos] = L'&';
                pNewBuffer[nPos+1] = L'a';
                pNewBuffer[nPos+2] = L'm';
                pNewBuffer[nPos+3] = L'p';
                pNewBuffer[nPos+4] = L';';
            }
            else 
            {
                pNewBuffer = (wchar_t*)::CoTaskMemAlloc((nLen + 5)*sizeof(wchar_t));
                memcpy(pNewBuffer, *pstrDT, nPos*sizeof(wchar_t));
                pNewBuffer[nPos] = L'&';
                pNewBuffer[nPos+1] = L'a';
                pNewBuffer[nPos+2] = L'm';
                pNewBuffer[nPos+3] = L'p';
                pNewBuffer[nPos+4] = L';';
                memcpy(pNewBuffer+nPos+5, *pstrDT+nPos+1, (nLen-nPos)*sizeof(wchar_t)); 
            }
            nPos +=4;
            nLen +=4; 
            break;

        case L'"':   //&quot;
            if(pNewBuffer != *pstrDT)
            {
                pNewBuffer = (wchar_t*)::CoTaskMemRealloc(pNewBuffer, (nLen+6)*sizeof(wchar_t));
				memcpy((pNewBuffer + nPos + 6), (pNewBuffer + nPos + 1), (nLen-nPos)*sizeof(wchar_t));
                pNewBuffer[nPos] = L'&';
                pNewBuffer[nPos+1] = L'q';
                pNewBuffer[nPos+2] = L'u';
                pNewBuffer[nPos+3] = L'o';
                pNewBuffer[nPos+4] = L't';
                pNewBuffer[nPos+5] = L';';
            }
            else 
            {
                pNewBuffer = (wchar_t*)::CoTaskMemAlloc((nLen + 6)*sizeof(wchar_t));
                memcpy(pNewBuffer, *pstrDT, nPos*sizeof(wchar_t));
                pNewBuffer[nPos] = L'&';
                pNewBuffer[nPos+1] = L'q';
                pNewBuffer[nPos+2] = L'u';
                pNewBuffer[nPos+3] = L'o';
                pNewBuffer[nPos+4] = L't';
                pNewBuffer[nPos+5] = L';';
                memcpy((pNewBuffer+nPos+6), (*pstrDT+nPos+1), (nLen-nPos)*sizeof(wchar_t)); 
            }
            nPos +=5;
            nLen +=5; 
            break;
        case L'\'':   //'   &apos;
            if(pNewBuffer != *pstrDT)
            {
                pNewBuffer = (wchar_t*)::CoTaskMemRealloc(pNewBuffer, (nLen+6)*sizeof(wchar_t));
                memcpy(pNewBuffer + nPos + 6, pNewBuffer + nPos + 1, (nLen-nPos)*sizeof(wchar_t));
                pNewBuffer[nPos] = L'&';
                pNewBuffer[nPos+1] = L'a';
                pNewBuffer[nPos+2] = L'p';
                pNewBuffer[nPos+3] = L'o';
                pNewBuffer[nPos+4] = L's';
                pNewBuffer[nPos+5] = L';';
            }
            else 
            {
                pNewBuffer = (wchar_t*)::CoTaskMemAlloc((nLen + 6)*sizeof(wchar_t));
                memcpy(pNewBuffer, *pstrDT, nPos*sizeof(wchar_t));
                pNewBuffer[nPos] = L'&';
                pNewBuffer[nPos+1] = L'a';
                pNewBuffer[nPos+2] = L'p';
                pNewBuffer[nPos+3] = L'o';
                pNewBuffer[nPos+4] = L's';
                pNewBuffer[nPos+5] = L';';
                memcpy(pNewBuffer+nPos+6, *pstrDT+nPos+1, (nLen-nPos)*sizeof(wchar_t)); 
            }
            nPos +=5;
            nLen +=5; 
            break;
        default:
            break;
        }  
    }
	if(*pstrDT != pNewBuffer)
        *pstrDT = pNewBuffer;
}

bool CToXML::SetXMLDataType(ULONG nCol, XMLDataType nXMLDataType)
{
	OleDBPro_ASSERT(m_pXMLDataType && GetCols() && GetRawDataType(nCol), "\nMake sure that a CR2XML<T> object is opened!");
	if(!IsReasonable(nCol, nXMLDataType))
	{
		if(m_strError) 
			::SysFreeString(m_strError);
		m_strError = ::SysAllocString(L"The XML data type can't be converted from OleDBPro!");
		m_hr=E_FAIL;
		return false;
	}	
	m_pXMLDataType[nCol-1]=nXMLDataType;
	return true;
}

XMLDataType CToXML::GetXMLDataType(ULONG nCol)
{
	OleDBPro_ASSERT(m_pXMLDataType && GetCols() && GetRawDataType(nCol), "\nMake sure that a CR2XML<T> object is opened!");
	return m_pXMLDataType[nCol-1];
}

const char* CToXML::GetXMLStringDataType(ULONG nCol)
{
	OleDBPro_ASSERT(m_pXMLDataType && GetCols() && GetRawDataType(nCol), "\nMake sure that a CR2XML<T> object is opened!");
	XMLDataType		nXMLDataType=GetXMLDataType(nCol);
	const char* str=NULL;
	switch (nXMLDataType)
	{
		case XML_String:
			str="string";
			break;
		case XML_Bin_hex:
			str="bin.hex";
			break;
		case XML_Boolean:
			str="boolean";
			break;
		case XML_Char:
			str="char";
			break;
		case XML_Date:
			str="date";
			break;
		case XML_DateTime:
			str="dateTime";
			break;
		case XML_DateTime_tz:
			str="dateTime.tz";
			break;
		case XML_Fixed_14_4:
			str="fixed.14.4";
			break;
		case XML_Number:
			str="number";
			break;
		case XML_Time:
			str="time";
			break;
		case XML_Time_tz:
			str="time.tz";
			break;
		case XML_I1:
			str="i1";
			break;
		case XML_I2:
			str="i2";
			break;
		case XML_Int:
			str="int";
			break;
		case XML_Ui1:
			str="ui1";
			break;
		case XML_Ui2:
			str="ui2";
			break;
		case XML_Ui4:
			str="ui4";
			break;
		case XML_Uri:
			str="uri";
			break;
		case XML_Uuid:
			str="uuid";
			break;
		case XML_I4:
			str="i4";
			break;
		case XML_Float:
			str="float";
			break;
		case XML_R4:
			str="r4";
			break;
		case XML_R8:
			str="r8";
			break;
		default:
			break;
	}
	return str;
}

bool CToXML::IsReasonable(ULONG nCol, XMLDataType nXMLDataType)
{
	DBTYPE	nDBType=GetRawDataType(nCol);
	switch (nXMLDataType)
	{
	case XML_String:
		return true;
	case XML_Bin_hex:
		if(nDBType==DBTYPE_BYTES)
			return true;
		if(nDBType==DBTYPE_STR || nDBType==DBTYPE_WSTR || nDBType==DBTYPE_VARIANT || nDBType==DBTYPE_BSTR)
		{
			AtlTrace(_T("Warning: XML data type conversion may be incorrect for the column %d!\n"), nCol);
			return true;
		}
		break;
	case XML_Boolean:
		return true;
	case XML_Char:
		return true;
	case XML_DateTime_tz:
	case XML_DateTime:
	case XML_Date:
		if(nDBType==DBTYPE_DATE || nDBType==DBTYPE_DBDATE || nDBType==DBTYPE_DBTIMESTAMP || DBTYPE_FILETIME)
			return true;
		if(nDBType==DBTYPE_STR || nDBType==DBTYPE_WSTR || nDBType==DBTYPE_VARIANT || nDBType==DBTYPE_BSTR)
		{
			AtlTrace(_T("Warning: XML data type conversion may be incorrect for the column %d!\n"), nCol);
			return true;
		}
		break;
	case XML_Float:
	case XML_R4:
	case XML_R8:
	case XML_Number:
	case XML_Fixed_14_4:
		if( nDBType==DBTYPE_R4		||
			nDBType==DBTYPE_R8		||
			nDBType==DBTYPE_CY		||
			nDBType==DBTYPE_BOOL	||
			nDBType==DBTYPE_I1		||
			nDBType==DBTYPE_I2		||
			nDBType==DBTYPE_I4		||
			nDBType==DBTYPE_UI1		||
			nDBType==DBTYPE_UI2		||
			nDBType==DBTYPE_UI4		||
			nDBType==DBTYPE_I8		||
			nDBType==DBTYPE_UI8		||
			nDBType==DBTYPE_DECIMAL	||
			nDBType==DBTYPE_NUMERIC	||
			nDBType==DBTYPE_VARNUMERIC)
		return true;
		if(nDBType==DBTYPE_STR || nDBType==DBTYPE_WSTR || nDBType==DBTYPE_VARIANT || nDBType==DBTYPE_BSTR)
		{
			AtlTrace(_T("Warning: XML data type conversion may be incorrect for the column %d!\n"), nCol);
			return true;
		}
		break;
	case XML_Time:
	case XML_Time_tz:
		if(nDBType==DBTYPE_DBTIME || nDBType==DBTYPE_DATE || nDBType==DBTYPE_DBTIMESTAMP || DBTYPE_FILETIME)
			return true;
		if(nDBType==DBTYPE_STR || nDBType==DBTYPE_WSTR || nDBType==DBTYPE_VARIANT || nDBType==DBTYPE_BSTR)
		{
			AtlTrace(_T("Warning: XML data type conversion may be incorrect for the column %d!\n"), nCol);
			return true;
		}
		break;
	case XML_I4:
	case XML_Int:
	case XML_I1:
	case XML_I2:
	case XML_Ui1:
	case XML_Ui2:
	case XML_Ui4:
		if( nDBType==DBTYPE_R4		||
			nDBType==DBTYPE_R8		||
			nDBType==DBTYPE_CY		||
			nDBType==DBTYPE_BOOL	||
			nDBType==DBTYPE_I1		||
			nDBType==DBTYPE_I2		||
			nDBType==DBTYPE_I4		||
			nDBType==DBTYPE_UI1		||
			nDBType==DBTYPE_UI2		||
			nDBType==DBTYPE_UI4		||
			nDBType==DBTYPE_I8		||
			nDBType==DBTYPE_UI8		||
			nDBType==DBTYPE_DECIMAL	||
			nDBType==DBTYPE_NUMERIC	||
			nDBType==DBTYPE_VARNUMERIC)
			return true;
		if(nDBType==DBTYPE_STR || nDBType==DBTYPE_WSTR || nDBType==DBTYPE_VARIANT || nDBType==DBTYPE_BSTR)
		{
			AtlTrace(_T("Warning: XML data type conversion may be incorrect for the column %d!\n"), nCol);
			return true;
		}
		break;
	case XML_Uri:
		if(nDBType==DBTYPE_STR || nDBType==DBTYPE_WSTR || nDBType==DBTYPE_VARIANT || nDBType==DBTYPE_BSTR ||nDBType==DBTYPE_BYTES)
		{
			AtlTrace(_T("Warning: XML data type conversion may be incorrect for the column %d!\n"), nCol);
			return true;
		}
		break;
	case XML_Uuid:
		if(nDBType==DBTYPE_GUID)
			return true;
		if(nDBType==DBTYPE_STR || nDBType==DBTYPE_WSTR || nDBType==DBTYPE_VARIANT || nDBType==DBTYPE_BSTR ||nDBType==DBTYPE_BYTES)
		{
			AtlTrace(_T("Warning: XML data type conversion may be incorrect for the column %d!\n"), nCol);
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

void CToXML::ODB2XMLConversion(ULONG nCol, char* const strData)
{
	XMLDataType		nXMLDataType=GetXMLDataType(nCol);
	switch (nXMLDataType)
	{
		case XML_String:
			break;
		case XML_Bin_hex:
			ToBin_hex(nCol, strData);
			break;
		case XML_Boolean:
			ToBoolean(nCol, strData);
			break;
		case XML_Char:
			ToChar(nCol, strData);
			break;
		case XML_Date:
			ToDate(nCol, strData);
			break;
		case XML_DateTime:
			ToDateTime(nCol, strData);
			break;
		case XML_DateTime_tz:
			ToDateTime_tz(nCol, strData);
			break;
		case XML_Fixed_14_4:
			ToFixed_14_4(nCol, strData);
			break;
		case XML_Number:
			ToNumber(nCol, strData);
			break;
		case XML_Time:
			ToTime(nCol, strData);
			break;
		case XML_Time_tz:
			ToTime_tz(nCol, strData);
			break;
		case XML_I1:
			ToI1(nCol, strData);
			break;
		case XML_I2:
			ToI2(nCol, strData);
			break;
		case XML_Int:
			ToInt(nCol, strData);
			break;
		case XML_Ui1:
			ToUi1(nCol, strData);
			break;
		case XML_Ui2:
			ToUi2(nCol, strData);
			break;
		case XML_Ui4:
			ToUi4(nCol, strData);
			break;
		case XML_Uri:
			ToUri(nCol, strData);
			break;
		case XML_Uuid:
			ToUuid(nCol, strData);
			break;
		case XML_I4:
			ToI4(nCol, strData);
			break;
		case XML_Float:
			ToFloat(nCol, strData);
			break;
		case XML_R4:
			ToR4(nCol, strData);
			break;
		case XML_R8:
			ToR8(nCol, strData);
			break;
		default:
			break;
	}	
}

void CToXML::ToBin_hex(ULONG nCol, char *const strIn)
{
}

void CToXML::ToBoolean(ULONG nCol, char *const strIn)
{
	if (strIn[0]==' ' || strIn[0]==0 || strIn[0]=='0')
		strIn[0]='0';
	else
		strIn[0]='1';
	strIn[1]=0;
}

void CToXML::ToChar(ULONG nCol, char *const strIn)
{
	strIn[1]=0;
}

void CToXML::ToDate(ULONG nCol, char *const strIn)
{
	DBTYPE	nDBType=GetRawDataType(nCol);
	if(nDBType==DBTYPE_FILETIME||nDBType==DBTYPE_DATE)
	{
		//convert to dt:type="datetime"
        DBTIMESTAMP dbTimeStamp;
        memset(&dbTimeStamp, 0, sizeof(DBTIMESTAMP));
        GetDataEx(nCol, &dbTimeStamp, DBTYPE_DBTIMESTAMP);
        if(dbTimeStamp.year)
        {
            sprintf(strIn, "%.4d-%.2d-%.2d", dbTimeStamp.year, dbTimeStamp.month, dbTimeStamp.day);
        }
	}
	strIn[10]=0;
}

void CToXML::ToDateTime(ULONG nCol, char *const strIn)
{
	DBTYPE	nDBType=GetRawDataType(nCol);
	if (nDBType==DBTYPE_DBTIMESTAMP)
	{
		strIn[10]='T';    //convert to dt:type="datetime"
		strIn[19]=0;
	}
	else if(nDBType==DBTYPE_DATE || nDBType==DBTYPE_FILETIME)
	{
		//convert to dt:type="datetime"
        DBTIMESTAMP dbTimeStamp;
        memset(&dbTimeStamp, 0, sizeof(DBTIMESTAMP));
        GetDataEx(nCol, &dbTimeStamp, DBTYPE_DBTIMESTAMP);
        if(dbTimeStamp.year)
        {
            sprintf(strIn, "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d", dbTimeStamp.year, 
                    dbTimeStamp.month, dbTimeStamp.day, dbTimeStamp.hour, 
                    dbTimeStamp.minute, dbTimeStamp.second);
            strIn[19]=0;
        }
        else
            strIn[0]=0;
	}
	else if (GetRawDataType(nCol)==DBTYPE_DBDATE)
	{
		strIn[10]=0;    //convert to dt:type="datetime"
	}
}

void CToXML::ToDateTime_tz(ULONG nCol, char *const strIn)
{
	
}

void CToXML::ToFixed_14_4(ULONG nCol, char *const strIn)
{	
	int nPos=0;
	while (strIn[nPos])
	{
		if(strIn[nPos] == '.')
		{
			strIn[nPos+5]=0;	
		}
		nPos++;
	}
}

void CToXML::ToInt(ULONG nCol, char *const strIn)
{
	
}

void CToXML::ToNumber(ULONG nCol, char *const strIn)
{

}

void CToXML::ToTime(ULONG nCol, char *const strIn)
{
	DBTYPE nDBType=GetRawDataType(nCol);
	if(nDBType==DBTYPE_DATE||nDBType==DBTYPE_FILETIME||nDBType==DBTYPE_DBTIMESTAMP)
	{
		//convert to dt:type="datetime"
        DBTIMESTAMP dbTimeStamp;
        memset(&dbTimeStamp, 0, sizeof(DBTIMESTAMP));
        GetDataEx(nCol, &dbTimeStamp, DBTYPE_DBTIMESTAMP);
        if(dbTimeStamp.year)
        {
            sprintf(strIn, "%.2d:%.2d:%.2d", dbTimeStamp.hour, dbTimeStamp.minute, dbTimeStamp.second);
            strIn[8]=0;
        }
	}
}

void CToXML::ToTime_tz(ULONG nCol, char *const strIn)
{
		
}

void CToXML::ToI1(ULONG nCol, char *const strIn)
{

}

void CToXML::ToI2(ULONG nCol, char *const strIn)
{

}

void CToXML::ToUi1(ULONG nCol, char *const strIn)
{
	if(strIn[0]=='-')
		strIn[0]=' ';
}

void CToXML::ToUi2(ULONG nCol, char *const strIn)
{
	if(strIn[0]=='-')
		strIn[0]=' ';
}

void CToXML::ToUi4(ULONG nCol, char *const strIn)
{
	if(strIn[0]=='-')
		strIn[0]=' ';
}

void CToXML::ToUri(ULONG nCol, char *const strIn)
{

}

void CToXML::ToUuid(ULONG nCol, char *const strIn)
{
	memcpy(strIn, (strIn+1), 36);
    strIn[36]=0;
}

void CToXML::ToI4(ULONG nCol, char *const strIn)
{

}

void CToXML::ToFloat(ULONG nCol, char *const strIn)
{

}

void CToXML::ToR4(ULONG nCol, char *const strIn)
{

}

void CToXML::ToR8(ULONG nCol, char *const strIn)
{

}

void CToXML::Chars2DT(char **pstrDT)
{
    ATLASSERT(pstrDT && *pstrDT);
    ULONG nPos;
    ULONG nLen = strlen(*pstrDT);
    char *pNewBuffer=*pstrDT;
    for(nPos=0; nPos<nLen; nPos++)
    {
        switch (pNewBuffer[nPos])
        {
        case '<':
            if(pNewBuffer != *pstrDT)
            {
                pNewBuffer = (char*)::CoTaskMemRealloc(pNewBuffer, nLen+4);
                memcpy(pNewBuffer + nPos + 4, pNewBuffer + nPos + 1, nLen-nPos);
                pNewBuffer[nPos] = '&';
                pNewBuffer[nPos+1] = 'l';
                pNewBuffer[nPos+2] = 't';
                pNewBuffer[nPos+3] = ';';
            }
            else 
            {
                pNewBuffer = (char*)::CoTaskMemAlloc(nLen + 4);
                memcpy(pNewBuffer, *pstrDT, nPos);
                pNewBuffer[nPos] = '&';
                pNewBuffer[nPos+1] = 'l';
                pNewBuffer[nPos+2] = 't';
                pNewBuffer[nPos+3] = ';';
                memcpy(pNewBuffer+nPos+4, *pstrDT+nPos+1, nLen-nPos); 
            }
            nPos +=3;
            nLen +=3; //&lt;
            break;
        case '>':   //&gt;
            if(pNewBuffer != *pstrDT)
            {
                pNewBuffer = (char*)::CoTaskMemRealloc(pNewBuffer, nLen+4);
                memcpy(pNewBuffer + nPos + 4, pNewBuffer + nPos + 1, nLen-nPos);
                pNewBuffer[nPos] = '&';
                pNewBuffer[nPos+1] = 'g';
                pNewBuffer[nPos+2] = 't';
                pNewBuffer[nPos+3] = ';';
            }
            else 
            {
                pNewBuffer = (char*)::CoTaskMemAlloc(nLen + 4);
                memcpy(pNewBuffer, *pstrDT, nPos);
                pNewBuffer[nPos] = '&';
                pNewBuffer[nPos+1] = 'g';
                pNewBuffer[nPos+2] = 't';
                pNewBuffer[nPos+3] = ';';
                memcpy(pNewBuffer+nPos+4, *pstrDT+nPos+1, nLen-nPos); 
            }
            nPos +=3;
            nLen +=3; 
            break;
        case '&': //&amp;
            if(pNewBuffer != *pstrDT)
            {
                pNewBuffer = (char*)::CoTaskMemRealloc(pNewBuffer, nLen+5);
                memcpy(pNewBuffer + nPos + 5, pNewBuffer + nPos + 1, nLen-nPos);
                pNewBuffer[nPos] = '&';
                pNewBuffer[nPos+1] = 'a';
                pNewBuffer[nPos+2] = 'm';
                pNewBuffer[nPos+3] = 'p';
                pNewBuffer[nPos+4] = ';';
            }
            else 
            {
                pNewBuffer = (char*)::CoTaskMemAlloc(nLen + 5);
                memcpy(pNewBuffer, *pstrDT, nPos);
                pNewBuffer[nPos] = '&';
                pNewBuffer[nPos + 1] = 'a';
                pNewBuffer[nPos + 2] = 'm';
                pNewBuffer[nPos + 3] = 'p';
                pNewBuffer[nPos + 4] = ';';
                memcpy(pNewBuffer + nPos + 5, *pstrDT + nPos + 1, nLen - nPos ); 
            }
            nPos += 4;
            nLen += 4; 
            break;
        case '"':   //&quot;
            if(pNewBuffer != *pstrDT)
            {
                pNewBuffer = (char*)::CoTaskMemRealloc(pNewBuffer, nLen+6);
                memcpy(pNewBuffer + nPos + 6, pNewBuffer + nPos + 1, nLen-nPos);
                pNewBuffer[nPos] = '&';
                pNewBuffer[nPos+1] = 'q';
                pNewBuffer[nPos+2] = 'u';
                pNewBuffer[nPos+3] = 'o';
                pNewBuffer[nPos+4] = 't';
                pNewBuffer[nPos+5] = ';';
            }
            else 
            {
                pNewBuffer = (char*)::CoTaskMemAlloc(nLen + 6);
                memcpy(pNewBuffer, *pstrDT, nPos);
                pNewBuffer[nPos] = '&';
                pNewBuffer[nPos+1] = 'q';
                pNewBuffer[nPos+2] = 'u';
                pNewBuffer[nPos+3] = 'o';
                pNewBuffer[nPos+4] = 't';
                pNewBuffer[nPos+5] = ';';
                memcpy(pNewBuffer+nPos+6, *pstrDT+nPos+1, nLen-nPos); 
            }
            nPos +=5;
            nLen +=5; 
            break;
        case '\'':   //'   &apos;
            if(pNewBuffer != *pstrDT)
            {
                pNewBuffer = (char*)::CoTaskMemRealloc(pNewBuffer, nLen+6);
                memcpy(pNewBuffer + nPos + 6, pNewBuffer + nPos + 1, nLen-nPos);
                pNewBuffer[nPos] = '&';
                pNewBuffer[nPos+1] = 'a';
                pNewBuffer[nPos+2] = 'p';
                pNewBuffer[nPos+3] = 'o';
                pNewBuffer[nPos+4] = 's';
                pNewBuffer[nPos+5] = ';';
            }
            else 
            {
                pNewBuffer = (char*)::CoTaskMemAlloc(nLen + 6);
                memcpy(pNewBuffer, *pstrDT, nPos);
                pNewBuffer[nPos] = '&';
                pNewBuffer[nPos+1] = 'a';
                pNewBuffer[nPos+2] = 'p';
                pNewBuffer[nPos+3] = 'o';
                pNewBuffer[nPos+4] = 's';
                pNewBuffer[nPos+5] = ';';
                memcpy(pNewBuffer+nPos+6, *pstrDT+nPos+1, nLen-nPos); 
            }
            nPos +=5;
            nLen +=5; 
            break;
        default:
            break;
        }  
    }
    if(*pstrDT != pNewBuffer)
        *pstrDT = pNewBuffer;
}

void CToXML::CreateArrayOfXMLDataTypes()
{
	OleDBPro_ASSERT(GetCols(), "\nMake sure that a CR2XML<T> object is opened!");
	m_pXMLDataType= (XMLDataType*)::CoTaskMemRealloc(m_pXMLDataType, sizeof(XMLDataType)*GetCols());
	memset(m_pXMLDataType, 0, sizeof(XMLDataType)*GetCols());
}
