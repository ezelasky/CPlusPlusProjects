// Format.cpp : Implementation of CFormat
#include "stdafx.h"
#include "DTran.h"
#include "Format.h"
//#include "Utilities.h"

#if !defined
#include <fstream>
#endif

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CFormat



typedef struct {
	_bstr_t bstrID;
	_bstr_t bstrName;
	FIELDTYPE nType;
	_bstr_t bstrLen;
	long lStartPos;
	long lEndPos;
	long lPrecision;
	long lScale;
	} FieldDef;
	
typedef std::list<FieldDef*> FLDDEFLIST;


VARIANT_BOOL CFormat::LoadFormatFromFile(LPTSTR pzFile, int iBeginLine)
{
	VARIANT_BOOL bSuccess  = VARIANT_FALSE;
	TCHAR szError[MAX_ERR_LEN];
	memset(szError, 0, MAX_ERR_LEN);
	TCHAR pzBuffer[BUFF_LEN];
	memset(pzBuffer, 0, BUFF_LEN);

	FLDDEFLIST lFields;
	ifstream fileFormat(pzFile);
	bool  bFormatLoadErr = false;
	int iLineCnt = 0;
	
	//extract data from file
	if (fileFormat.is_open()) {

		//	throw out any headers...
		for (iLineCnt = 1; iLineCnt < iBeginLine; iLineCnt++)  {
			fileFormat.getline(pzBuffer, BUFF_LEN);  	
		}

		fileFormat.getline (pzBuffer, BUFF_LEN);
		while (fileFormat) {

			iLineCnt++;
			int iSize = lFields.size();
			int iLen1 = _tcslen(pzBuffer);
			pzBuffer[_tcslen(pzBuffer)+1] = '\0';

			TCHAR pzDataBuffer[BUFF_LEN];
			memset(pzDataBuffer, 0, BUFF_LEN);

			int index1 = 0;
			int index2 = 0;
			int iDataItem = 0;
			int iDataCnt = 0;
			_bstr_t bstrName = L"";
			_bstr_t bstrLen = L"254";
			_bstr_t bstrType = L"VARCHAR";
			FIELDTYPE nInput = vVARCHAR;
			long    lStart = 0;
			long    lEnd = 0;
			long    lScale = 0;
			long    lPrecision = 0;
			TCHAR ch = pzBuffer[index1++];
			long l_FieldsLoaded = 0x00000000;
			bool bName = false;
			bool bType = false;
			bool bStart = false;
			bool bEnd = false;
			bool bLength = false;
			bool bPrec = false;
			bool bScale = false;
			while ( ch != '\0' ) {
			
				//go until you have a full item -- no space
				memset(pzDataBuffer, 0, BUFF_LEN);
				index2 = 0;
				while (!_istspace(ch) && ch != '\0') {

					pzDataBuffer[index2++] = ch;
					ch = pzBuffer[index1++];
				}
				
				//now check ... is this an item we care about, if yes then populate structure appropiately
				int iLen = _tcslen(pzDataBuffer);
				if ( iLen > 0 ) {
					
					iDataItem++;
					//what item is this?
					if ( m_iFieldPos[nNAME] == iDataItem ) {			//fieldname
						pzDataBuffer[iLen+1] = '\0';
						//mbstowcs(bstrName, pzDataBuffer, _tcslen(pzDataBuffer));
						_bstr_t bstrBuffer(pzDataBuffer);
						bstrName = bstrBuffer;
						bName = true;
						iDataCnt++;
					}
					else if ( m_iFieldPos[nTYPE] == iDataItem ) {	//fieldtype
						pzDataBuffer[iLen+1] = '\0';
						if (_stricmp(pzDataBuffer, _T("varchar"))== 0 )
							nInput = vVARCHAR;
						else if (_stricmp(pzDataBuffer, _T("bigint"))== 0 )
							nInput = vBIGINT;
						else if (_stricmp(pzDataBuffer, _T("binary")) == 0 )
							nInput = vBINARY;
						else if (_stricmp(pzDataBuffer, _T("bit")) == 0 )
							nInput = vBIT;
						else if (_stricmp(pzDataBuffer, _T("char")) == 0 )
							nInput = vCHAR;
						else if (_stricmp(pzDataBuffer, _T("datetime")) == 0 )
							nInput = vDATETIME;
						else if (_stricmp(pzDataBuffer, _T("decimal"))== 0 )
							nInput = vDECIMAL;
						else if (_stricmp(pzDataBuffer, _T("float"))== 0 )
							nInput = vFLOAT;
						else if (_stricmp(pzDataBuffer, _T("int"))== 0 )
							nInput = vINT;
						else if (_stricmp(pzDataBuffer, _T("money"))== 0 )
							nInput = vMONEY;
						else if (_stricmp(pzDataBuffer, _T("numeric"))== 0 )
							nInput = vNUMERIC;
						else if (_stricmp(pzDataBuffer, _T("real")) == 0)
							nInput = vREAL;
						else if (_stricmp(pzDataBuffer, _T("smalldatetime")) == 0)
							nInput = vSMALLDATETIME;
						else if (_stricmp(pzDataBuffer, _T("smallint")) == 0)
							nInput = vSMALLINT;
						else if (_stricmp(pzDataBuffer, _T("smallmoney")) == 0)
							nInput = vSMALLMONEY;
						else if (_stricmp(pzDataBuffer, _T("text")) == 0)
							nInput = vTEXT;
						else if (_stricmp(pzDataBuffer, _T("timestamp")) == 0)
							nInput = vTIMESTAMP;
						else if (_stricmp(pzDataBuffer, _T("tinyint")) == 0 )
							nInput = vTINYINT;
						else if (_stricmp(pzDataBuffer, _T("SQLCHAR")) == 0 )
							nInput = vVARCHAR;   //BCP format files have this type, not the others
						else {
							bFormatLoadErr = true;
							_stprintf(szError, _T("DTRAN ERROR: Data type: %s is not a stadard data type. Field collection will not be loaded."), pzDataBuffer);
							_bstr_t bstrErr(szError);
							m_listErrors.push_back(bstrErr);
						}
						iDataCnt++;
						bType = true;
					}
					else if ( m_iFieldPos[nSTART] == iDataItem ) {	//start 
						lStart = (long) atoi(pzDataBuffer);
						iDataCnt++;
						bStart = true;
					}
					else if ( m_iFieldPos[nEND] == iDataItem ) {	//end
						lEnd = (long) atoi(pzDataBuffer);
						iDataCnt++;
						bEnd = true;
					}
					else if ( m_iFieldPos[nLENGTH] == iDataItem ) {	//fieldlength
						_bstr_t bstrDataBuffer(pzDataBuffer);
						bstrLen = bstrDataBuffer;
						iDataCnt++;
						bLength = true;
					}
					else if ( m_iFieldPos[nPRECISION] == iDataItem ) {	//precision
						lPrecision = (long) atoi(pzDataBuffer);
						iDataCnt++;
						bPrec = true;
					}
					else if ( m_iFieldPos[nSCALE] == iDataItem ) {	//scale
						lScale = (long) atoi(pzDataBuffer);
						iDataCnt++;
						bScale = true;
					}
				}
				//go again
				ch=pzBuffer[index1++];
			}

				if ( !bName && NamePosSet() ) {
				_stprintf(szError, _T("DTRAN ERROR: Format File Line #: %d -- Field: name was specified but not found in the format file."), iLineCnt);
				m_listErrors.push_back(szError);
				bFormatLoadErr = true;
			}
			if ( bName && !NamePosSet() ) {
				_stprintf(szError, _T("DTRAN ERROR: Format File Line #: %d -- Field: name was loaded but found in format specification."), iLineCnt);
				m_listErrors.push_back(szError);
				bFormatLoadErr = true;
			}

			if ( !bType && TypePosSet() ) {
				_stprintf(szError, _T("DTRAN ERROR: Format File Line #: %d -- Field: type was specified but not found in the format file."), iLineCnt);
				m_listErrors.push_back(szError);
				bFormatLoadErr = true;
			}
			if ( bType && !TypePosSet() ) {
				_stprintf(szError, _T("DTRAN ERROR: Format File Line #: %d -- Field: type was loaded but not found in format specification."), iLineCnt);
				m_listErrors.push_back(szError);
				bFormatLoadErr = true;
			}

			if ( !bStart && StartPosSet() ) {
				_stprintf(szError, _T("DTRAN ERROR: Format File Line #: %d -- Field: start was specified but not found in the format file."), iLineCnt);
				m_listErrors.push_back(szError);
				bFormatLoadErr = true;
			}
			if ( bStart && !StartPosSet() ) {
				_stprintf(szError, _T("DTRAN ERROR: Format File Line #: %d -- Field: start was loaded but not found in format specification."), iLineCnt);
				m_listErrors.push_back(szError);
				bFormatLoadErr = true;
			}
			if ( !bEnd && EndPosSet() ) {
				_stprintf(szError, _T("DTRAN ERROR: Format File Line #: %d -- Field: end was specified but not found in the format file."), iLineCnt);
				m_listErrors.push_back(szError);
				bFormatLoadErr = true;
			}
			if ( bEnd && !EndPosSet() ) {
				_stprintf(szError, _T("DTRAN ERROR: Format File Line #: %d -- Field: end was loaded but not found in format specification."), iLineCnt);
				m_listErrors.push_back(szError);
				bFormatLoadErr = true;
			}
			if ( !bLength && LengthPosSet() ) {
				_stprintf(szError, _T("DTRAN ERROR: Format File Line #: %d -- Field: length was specified but not found in the format file."), iLineCnt);
				m_listErrors.push_back(szError);
				bFormatLoadErr = true;
			}
			if ( bLength && !LengthPosSet() ) {
				_stprintf(szError, _T("DTRAN ERROR: Format File Line #: %d -- Field: length was loaded but not found in format specification."), iLineCnt);
				m_listErrors.push_back(szError);
				bFormatLoadErr = true;
			}

			FieldDef *pFldDef = new FieldDef;
			if (pFldDef && iDataItem != 0 && !bFormatLoadErr) {
				pFldDef->bstrName = bstrName;
				pFldDef->nType = nInput;  
				pFldDef->bstrLen = bstrLen;
				pFldDef->lEndPos = lEnd;
				pFldDef->lStartPos = lStart;
				pFldDef->lPrecision = lPrecision;
				pFldDef->lScale = lScale;
				lFields.push_back(pFldDef);
			}
			fileFormat.getline(pzBuffer, 512);  //next line
		}

	}
	else {
		TCHAR szFileIn[BUFF_MED]; memset(szFileIn, 0, BUFF_MED);
		int iStrLen =  SysStringLen(m_bstrFileIn);
		WideCharToMultiByte(CP_ACP, 0, m_bstrFileIn, iStrLen, szFileIn, iStrLen+1, NULL, NULL);
		szFileIn[iStrLen]= '\0';
		_stprintf(szError, _T("DTRAN ERROR: File : %s can not be opened."), szFileIn);
		_bstr_t bstrTemp(szError);
		m_listErrors.push_back(bstrTemp);
	}

	//if extraction successful load into format objects....
	FLDDEFLIST::iterator listIt;
	if (lFields.size() > 0 && !bFormatLoadErr ) {
		int iAdded = 0;
		int iSize = lFields.size();
		Clear();	//clear current collection

		//load collection
		for (listIt = lFields.begin(); listIt != lFields.end(); listIt++) {
			VARIANT_BOOL bValue = VARIANT_FALSE;
			AddFieldExt((*listIt)->bstrName, 
						(*listIt)->bstrLen, 
						(*listIt)->nType, 
						(*listIt)->lStartPos, 
						(*listIt)->lEndPos,
						(*listIt)->lPrecision,
						(*listIt)->lScale,
						&bValue);
			if ( bValue == VARIANT_TRUE) iAdded++;
		}

		//clean up
		for (listIt = lFields.begin(); listIt != lFields.end(); listIt++) {
		
			FieldDef* pFldDef = (*listIt);
			if (pFldDef)
				delete pFldDef;
			pFldDef = NULL;
		}
		lFields.clear();

		if (iSize == iAdded && iSize ==  m_vecFieldInfo.size())
			bSuccess = VARIANT_TRUE;
		else
			Clear();  //something is amiss, clean out collection
	}
	else {
		TCHAR szFileIn[BUFF_MED]; memset(szFileIn, 0, BUFF_MED);
		int iStrLen =  SysStringLen(m_bstrFileIn);
		WideCharToMultiByte(CP_ACP, 0, m_bstrFileIn, iStrLen, szFileIn, iStrLen+1, NULL, NULL);
		szFileIn[iStrLen]= '\0';
		_stprintf(szError, _T("DTRAN ERROR: Data was not loaded into collection from file : %s.  No data was read or error in reading data."), szFileIn);
		_bstr_t bstrTemp(szError);
		m_listErrors.push_back(bstrTemp);
	}
	return bSuccess;
}

VARIANT_BOOL CFormat::CalcStartandEndPositions(bool bStartExists)
{
	VARIANT_BOOL bSuccess  = VARIANT_TRUE;
    TCHAR szError[1024];  memset(szError, 0, 1024);
	long lastEnd = 0;
	for (int i = 0; i < (int) m_vecFieldInfo.size(); i++) 
	{
		IFieldInfo* pFieldOrig = m_vecFieldInfo[i];
		if (pFieldOrig != NULL)
		{
			BSTR bTemp;
			pFieldOrig->get_Length(&bTemp);
			_bstr_t bstrLen = bTemp;
			SysFreeString(bTemp);

			pFieldOrig->get_Name(&bTemp);
			_bstr_t bstrName = bTemp;
			SysFreeString(bTemp);

			if (wcscmp(bstrLen, L"") != 0 ) {
		
                long lLen = _wtoi((WCHAR*)bstrLen);
				long lStart = 0;
				if (bStartExists) 
					pFieldOrig->get_Start(&lStart);
				else
					lStart = lastEnd + 1;
				long lEnd = lStart + lLen - 1;

				if (!bStartExists)
					pFieldOrig->put_Start(lStart);
				pFieldOrig->put_End(lEnd);
				lastEnd = lEnd;
			}
			else {
				bSuccess = VARIANT_FALSE;
				TCHAR szName[BUFF_MED]; memset(szName, 0, BUFF_MED);
				int iStrLen =  SysStringLen(bstrName);
				WideCharToMultiByte(CP_ACP, 0, bstrName, iStrLen, szName, iStrLen+1, NULL, NULL);
				szName[iStrLen]= '\0';
				_stprintf(szError, _T("DTRAN ERROR: CFormat::CalcStartandEndPositions - Field: %s does not have a valid length."), szName);
				_bstr_t bstrTemp(szError);
				m_listErrors.push_back(bstrTemp);
			}
		}
		else {
			bSuccess = VARIANT_FALSE;
			_stprintf(szError, _T("DTRAN ERROR: CFormat::CalcStartandEndPositions - IFieldInfo pointer can be obtained."));
			_bstr_t bstrTemp(szError);
			m_listErrors.push_back(bstrTemp);
		}
	}
	return bSuccess;
}


STDMETHODIMP CFormat::ReplaceColNames(/*[out,retval]*/ VARIANT_BOOL* pRetVal)
{
	//this method replaces column names with DTS generic names, some of this is required for the DTS transformation mapping
	VARIANT_BOOL bSuccess  = VARIANT_FALSE;
    TCHAR szError[1024];  memset(szError, 0, 1024);
	if (!m_bCalledInternally)
		m_listErrors.clear();
	int iColCnt = 0;

	try {

		if (m_vecFieldInfo.size() == 0 ) {
			m_listErrors.push_back(L"DTRAN ERROR: CFormat::ReplaceColNames() -- Field collection is empty. Column Names can not be replaced.");
		}
		for (int i = 0; i < (int) m_vecFieldInfo.size(); i++) 
		{
			IFieldInfo* pFieldOrig = m_vecFieldInfo[i];
			if (pFieldOrig != NULL)
			{	iColCnt++;
				TCHAR szBuffer[10]; memset(szBuffer, 0, 10);
				if ( iColCnt <= 9 ) 
					sprintf(szBuffer, _T("Col00%d"), iColCnt);
				else if ( iColCnt >= 10 && iColCnt <= 99 )
					sprintf(szBuffer, _T("Col0%d"), iColCnt);
				else if ( iColCnt >= 100 )
					sprintf(szBuffer, _T("Col%d"), iColCnt);
				szBuffer[_tcslen(szBuffer)+1] = '\0';
				_bstr_t bstrNewName(szBuffer);
				//MultiByteToWideChar(CP_ACP, 0, szBuffer, _tcslen(szBuffer), bstrNewName, _tcslen(szBuffer));
				//mbstowcs(bstrNewName, szBuffer, _tcslen(szBuffer));
				pFieldOrig->put_Name(bstrNewName);
			}
		}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CFormat::ReplaceColNames() catch statement." , e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CFormat::ReplaceColNames() catch statement.");
		bSuccess = VARIANT_FALSE;
	}
	if (iColCnt != 0 ) bSuccess = VARIANT_TRUE;
	*pRetVal = bSuccess;
	return S_OK;
}



STDMETHODIMP CFormat::ValidFieldCollection(VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess  = VARIANT_FALSE;
	TCHAR szError[1024];  memset(szError, 0, 1024);
	int iRecordCnt = 0;
	BSTR bTemp;
	bool bCorrupt = false;
	std::map<_bstr_t, _bstr_t> mapNames;
	std::map<_bstr_t, _bstr_t>::iterator mapIt1;
	std::map<long, long> mapPositions;
	std::map<long, long>::iterator mapIt2;
	USES_CONVERSION;

	if (!m_bCalledInternally)
		m_listErrors.clear();

	try {

		if (m_vecFieldInfo.size() == 0 ) {
			m_listErrors.push_back(L"DTRAN ERROR: CFormat::ValidFieldCollection() -- Field collection is empty. Can not be validated.");
		}
		for (int i = 0; i < (int) m_vecFieldInfo.size(); i++) 
		{
			iRecordCnt++;
			IFieldInfo* pFieldOrig = m_vecFieldInfo[i];
			if (pFieldOrig != NULL)
			{
				long lStart, lEnd;
				pFieldOrig->get_Start(&lStart);
				pFieldOrig->get_End(&lEnd);
				long lCalcLen = lEnd - lStart + 1;
				
				pFieldOrig->get_Length(&bTemp);
				_bstr_t bstrLen = bTemp;
				SysFreeString(bTemp);

			//	pFieldOrig->get_Type(&bTemp);
			//	_bstr_t bstrType = bTemp;
			//	SysFreeString(bTemp);

				pFieldOrig->get_Name(&bTemp);
				_bstr_t bstrName = bTemp;
				SysFreeString(bTemp);

				TCHAR szFileIn[BUFF_MED]; memset(szFileIn, 0, BUFF_MED);
				int iStrLen =  SysStringLen(m_bstrFileIn);
				WideCharToMultiByte(CP_ACP, 0, m_bstrFileIn, iStrLen, szFileIn, iStrLen+1, NULL, NULL);
				szFileIn[iStrLen]= '\0';

				//assure data is complete...
				if (lStart == 0 || lEnd == 0 || wcscmp(bstrLen, L"") == 0 )
			//		wcscmp(bstrType, L"") == 0 || wcscmp(bstrName, L"") == 0 )
				{
					if (wcscmp(m_bstrFileIn, L"") != 0 )
						_stprintf(szError, _T("DTRAN ERROR: Incomplete data for record: %d in file: %s"), iRecordCnt, szFileIn);
					else
						_stprintf(szError, _T("DTRAN ERROR: Incomplete data for record: %d in field collection."), iRecordCnt);
					
					_bstr_t bstrError(szError);
					m_listErrors.push_back(bstrError);
				}
				else  
				{	
					//Validate Name for duplicates
					mapIt1 = mapNames.find(bstrName);  //?..what a hack this is..
					if (mapIt1 == mapNames.end() )  {
						mapNames.insert(std::map<_bstr_t, _bstr_t>::value_type(bstrName, bstrName));
					}
					else {
						if (wcscmp(m_bstrFileIn, L"") != 0 )
							_stprintf(szError, _T("DTRAN ERROR: Duplicate name for record: %d in file: %s"), iRecordCnt, szFileIn);
						else
							_stprintf(szError, _T("DTRAN ERROR: Duplicate name for record: %d. "), iRecordCnt);
						
						_bstr_t bstrError(szError);
						m_listErrors.push_back(bstrError);
					}

					//Valdiate start positions for duplicates 
					mapIt2 = mapPositions.find(lStart);
					if (mapIt2 == mapPositions.end() )  {
						mapPositions.insert(std::map<long, long>::value_type(lStart, lStart));
					}
					else {
						if (wcscmp(m_bstrFileIn, L"") != 0 )
							_stprintf(szError, _T("DTRAN ERROR: Duplicate or invalid field start position for record: %d in file: %s"), iRecordCnt, szFileIn);
						else
							_stprintf(szError, _T("DTRAN ERROR: Duplicate or invalid field start position for record: %d."), iRecordCnt);
						
						_bstr_t bstrError(szError);
						m_listErrors.push_back(bstrError);
					}

					//validate length
					long lOrigLen = _wtoi((WCHAR*)bstrLen);
					if (lCalcLen != lOrigLen){
						if (wcscmp(m_bstrFileIn, L"") != 0 )
							_stprintf(szError, _T("DTRAN ERROR: Field length is incorrect for record: %d in file: %s"), iRecordCnt, szFileIn);
						else
							_stprintf(szError, _T("DTRAN ERROR: Field length is incorrect for record: %d."), iRecordCnt);
						
						_bstr_t bstrError(szError);
						m_listErrors.push_back(bstrError);
					}

					//validate data types...need info on acceptable types....
				}	
			}
			else {
				_stprintf(szError, _T("DTRAN ERROR: CFormat::ValidFieldCollection() -- Corrupt pointers in field vector."));
				_bstr_t bstrError(szError);
				m_listErrors.push_back(bstrError);
				bCorrupt = true;
			}

		}

		//sort on start positions.... but only if no errors
		if( (int) m_listErrors.size() == 0 && !bCorrupt  ) 
		{
			int iStart = 0;
			int iLen = m_vecFieldInfo.size() - 1;
			long lStartFirst = 0;
			long lStartNext = 0;
		
			//insertion sort...this should go faster than selection or bubble sort
			//Part I: move the smallest value into the first position to anchor the sort
			for (int i = iLen; i > iStart; i--)
			{
				IFieldInfo* pFirstItem = m_vecFieldInfo[i-1];
				IFieldInfo* pNextItem = m_vecFieldInfo[i];
			
				pFirstItem->get_Start(&lStartFirst);
				pNextItem->get_End(&lStartNext);

				if (lStartNext < lStartFirst) {
					m_vecFieldInfo[i-1] = pNextItem;
					m_vecFieldInfo[i] = pFirstItem;
				}
			}

			//Part II: the bulk of the sort ... move down through the list doing mini-sorts (sorting up) the previously sorted items
			//bringing smaller values to the top ... the vector is sorted when we get to the end
			for (i = iStart + 2; i <= iLen; i++)
			{
				int j = i;
				long lStartBase = 0;
				IFieldInfo* pItemBase = m_vecFieldInfo[i];
				pItemBase->get_Start(&lStartBase);
			
				long lStartCompare = 0;
				IFieldInfo* pItemToCompare = m_vecFieldInfo[j-1];
				pItemToCompare->get_Start(&lStartCompare);

				while (lStartBase < lStartCompare)
				{
					m_vecFieldInfo[j] = pItemToCompare;
					j--;
					pItemToCompare = m_vecFieldInfo[j-1];
					pItemToCompare->get_Start(&lStartCompare);
				}

				m_vecFieldInfo[j] = pItemBase;
			}

			//need to check the sort or something? but for now...

			//this is a cross check using the field lengths to assure the start and stop values are correct
			long lTotalEndLen = 0;
			long lTotalStartLen = 0;
			long lLastFieldLen = 1;
			int iFldcnt = 0;
			for (int i = 0; i < (int) m_vecFieldInfo.size(); i++) 
			{
				iFldcnt++;
				IFieldInfo* pFieldInfo = m_vecFieldInfo[i];
				if (pFieldInfo != NULL)
				{
					BSTR bTemp;
					pFieldInfo->get_Length(&bTemp);
					_bstr_t bstrTemp = bTemp;
					SysFreeString(bTemp);

					TCHAR szTemp[BUFF_LEN]; memset(szTemp, 0, BUFF_LEN);
					int iStrLen =  SysStringLen(bstrTemp);
					WideCharToMultiByte(CP_ACP, 0, bstrTemp, iStrLen, szTemp, iStrLen+1, NULL, NULL);
					szTemp[iStrLen]= '\0';
					long lFieldLen= _ttoi( szTemp );

					pFieldInfo->get_Name(&bTemp);
					_bstr_t bstrName = bTemp;
					SysFreeString(bTemp);
					LPTSTR pzName = W2A(bstrName);

					long lFieldStart = 0;
					pFieldInfo->get_Start(&lFieldStart);
					long lFieldStop = 0;
					pFieldInfo->get_End(&lFieldStop);
					lTotalStartLen = lTotalEndLen + 1;
					if (lTotalStartLen != lFieldStart) {
						_stprintf(szError, _T("DTRAN ERROR: Field start value is incorrect for field: %s."), pzName);
						_bstr_t bstrErr(szError);
						m_listErrors.push_back(bstrErr);
					}
					lTotalEndLen = lTotalStartLen + lFieldLen - 1;
					if (lTotalEndLen != lFieldStop){
						_stprintf(szError, _T("DTRAN ERROR: Field stop value is incorrect for field: %s."), pzName);
						_bstr_t bstrErr(szError);
						m_listErrors.push_back(bstrErr);
					}
					lLastFieldLen = lFieldLen;
				}
			}

			if ( (int) m_listErrors.size() == 0)
				bSuccess = VARIANT_TRUE;
		}

		if  (bCorrupt) 
			bSuccess = VARIANT_FALSE;
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CFormat::ValidFieldCollection() catch statement." , e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CFormat::ValidFieldCollection() catch statement.");
		bSuccess = VARIANT_FALSE;
	}

	*pRetVal = bSuccess;
	return S_OK;
}

VARIANT_BOOL CFormat::LoadDELIMFormatFile(_bstr_t FileName)
{
	VARIANT_BOOL bSuccess  = VARIANT_FALSE;
	TCHAR szError[MAX_ERR_LEN];
	memset(szError, 0, MAX_ERR_LEN);

	//this one works a little different then the other two ....
	//all the info is in one line separated by the delimiter 
	TCHAR szFile[BUFF_MED]; memset(szFile, 0, BUFF_MED);
	int iStrLen =  SysStringLen(FileName);
	WideCharToMultiByte(CP_ACP, 0, FileName, iStrLen, szFile, iStrLen+1, NULL, NULL);
	szFile[iStrLen]= '\0';

	ifstream fileFormat(szFile);
	if (fileFormat.is_open()) 
	{
		Clear(); //clear current collection
		TCHAR* pzBuffer1 = new TCHAR[BUFF_VERY_LARGE]; memset(pzBuffer1, 0, BUFF_VERY_LARGE);
		TCHAR szFieldName[BUFF_LEN]; memset(szFieldName, 0, BUFF_LEN);
		TCHAR szField1[BUFF_LEN]; memset(szField1, 0, BUFF_LEN);
		TCHAR szRowDelim[2];  memset(szRowDelim, 0, 2);
		TCHAR szTextQual[2];  memset(szTextQual, 0, 2);
		if (m_bstrRowDelim.length() == 1) 
			WideCharToMultiByte(CP_ACP, 0, m_bstrRowDelim, 1, szRowDelim, 2, NULL, NULL);
		else 
			szRowDelim[0] = _T('\n');
		szRowDelim[_tcslen(szRowDelim)] = '\0';

		WideCharToMultiByte(CP_ACP, 0, m_bstrTextQualifier, 1, szTextQual, 2, NULL, NULL);
		szTextQual[_tcslen(szTextQual)] = '\0';

		TCHAR szFieldDelim[2]; memset(szFieldDelim, 0, 2);  
		if (m_bstrFldDelim.length() == 1) {
			WideCharToMultiByte(CP_ACP, 0, m_bstrFldDelim, 1, szFieldDelim, 2, NULL, NULL);
		}
		else
			szFieldDelim[0] = _T(',');
		szFieldDelim[_tcslen(szFieldDelim)] = '\0';
		int index = 0;

		fileFormat.getline(pzBuffer1, BUFF_VERY_LARGE, szRowDelim[0]);
		pzBuffer1[_tcslen(pzBuffer1)] = _T('\0');
		TCHAR* pzBufTemp = pzBuffer1;
		while (*pzBufTemp) 
		{
			TCHAR ch = *(pzBufTemp++);
			if (ch == szFieldDelim[0] || ch == _T('\0'))  
			{
				szFieldName[_tcslen(szFieldName)] = _T('\0');
				//remove text qualifiers & trim spaces here
				Utilities::RemoveChar(szFieldName, szTextQual[0], szField1);
				memset(szFieldName, 0, BUFF_LEN);
				//Utilities::TrimFirst(szField1, szFieldName);
				//memset(szField1, 0, BUFF_LEN);
				Utilities::TrimLast(szField1, szFieldName);
				_bstr_t bstrNameTemp(szFieldName);
                AddField(bstrNameTemp, L"255", vVARCHAR, &bSuccess);
				if (!bSuccess)  {
					_stprintf(szError, _T("DTRAN ERROR: CFormat::LoadDELIMFormatFile - Unsuccessfully adding field: %s from delimited header information"), szFieldName);
					_bstr_t bstrTemp(szError);
					m_listErrors.push_back(bstrTemp);
				}
				memset(szFieldName, 0, BUFF_LEN);
				index = 0;
			}
			else
				szFieldName[index++] = ch;
		}

		//add the last one in
		if (_tcsclen(szFieldName) > 0 ) 
		{
			szFieldName[_tcslen(szFieldName)] = _T('\0');
			//remove text qualifiers & trim spaces here
			Utilities::RemoveChar(szFieldName, szTextQual[0], szField1);
			memset(szFieldName, 0, BUFF_LEN);
			//Utilities::TrimFirst(szField1, szFieldName);
			//memset(szField1, 0, BUFF_LEN);
			Utilities::TrimLast(szField1, szFieldName);
			_bstr_t bstrFieldName(szFieldName);
            AddField(bstrFieldName, L"255", vVARCHAR, &bSuccess);
			if (!bSuccess) 	{
					_stprintf(szError, _T("DTRAN ERROR: CFormat::LoadDELIMFormatFile - Unsuccessfully adding field: %s from delimited header information"), szFieldName);
					_bstr_t bstrTemp(szError);
					m_listErrors.push_back(bstrTemp);
			}
		}

		
		if (pzBuffer1)
			delete []pzBuffer1;
	}

	return bSuccess;
}

STDMETHODIMP CFormat::LoadCust(BSTR FormatFileName, BSTR FormatSpec, VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess  = VARIANT_FALSE;
	_bstr_t bstrFileIn = FormatFileName;

	_bstr_t bstrErrMsg = L"";
	TCHAR szError[MAX_ERR_LEN];
	memset(szError, 0, MAX_ERR_LEN);
	m_bCalledInternally = true;
	m_listErrors.clear();

	try {
		_bstr_t bstrFile = Utilities::ConstructFullPath(m_bstrDirectory, bstrFileIn);

		TCHAR szFile[BUFF_MED]; memset(szFile, 0, BUFF_MED);
		int iStrLen =  SysStringLen(bstrFile);
		WideCharToMultiByte(CP_ACP, 0, bstrFile, iStrLen, szFile, iStrLen+1, NULL, NULL);
		szFile[iStrLen]= '\0';

		if ( Utilities::FileCanBeRead( szFile, szError )   ) 
		{
			m_bstrFileIn = bstrFile;
			if (wcscmp(FormatSpec, L"") != 0 ) 
			{
				int iLen = SysStringLen(FormatSpec);
				TCHAR* pzSpec = new TCHAR[iLen+2];
				memset(pzSpec, 0, iLen+1);
				wcstombs(pzSpec, FormatSpec, iLen);
				//int iLenTemp = _tcslen(pzSpec);
				pzSpec[iLen+1] = '\0';
				int i = 0;
				int iItemCnt = 1;
				int iStartPos = 1;
           		TCHAR szFieldID[BUFF_LEN]; memset(szFieldID, 0, BUFF_LEN);
				TCHAR szFieldTemp[BUFF_LEN]; memset(szFieldID, 0, BUFF_LEN);

				//Field position array -- depicts order of format fields in the format file
				InitializeFieldPosArray();

				//first parse the format specicification to find out the order of the fields in the format file
				while (*pzSpec){
					szFieldID[i++] = *(pzSpec++);
					if (*(pzSpec) == _T(',') || *(pzSpec) == _T('=') || *(pzSpec) == _T('\0'))  
					{
						Utilities::TrimLast(szFieldID, szFieldTemp);
						memset(szFieldID, 0, BUFF_LEN);
						Utilities::TrimFirst(szFieldTemp, szFieldID);

						if (_stricmp(szFieldID, _T("name")) == 0) {
							SetNamePos(iItemCnt);
							iItemCnt++;
						}
						else if ( _stricmp(szFieldID, _T("type")) == 0 ) {
							SetTypePos(iItemCnt);
							iItemCnt++;
						}
						else if ( _stricmp(szFieldID, _T("start")) == 0 ) {
							SetStartPos(iItemCnt);
							iItemCnt++;
						}
						else if ( _stricmp(szFieldID, _T("end")) == 0 ) {
							SetEndPos(iItemCnt);
							iItemCnt++;
						}
						else if ( _stricmp(szFieldID, _T("length")) == 0 ) {
							SetLengthPos(iItemCnt);
							iItemCnt++;
						}
						else if ( _stricmp(szFieldID, _T("precision")) == 0 ){
							SetPrecisionPos(iItemCnt);
							iItemCnt++;
						}
						else if ( _stricmp(szFieldID, _T("scale")) == 0 ) {
							SetScalePos(iItemCnt);
							iItemCnt++;
						}
						else if ( _stricmp(szFieldID, _T("firstrow")) == 0) {
							TCHAR szPos[BUFF_LEN]; memset(szPos, 0, BUFF_LEN);
							int j = 0;
							if (*pzSpec) pzSpec++;
							while (*pzSpec){
								szPos[j++] = *(pzSpec++);
								if (*(pzSpec) == _T(',') || *(pzSpec) == _T('\0')) {
									iStartPos = atoi(szPos);
									break;
								}
							}
							//if (*pzSpec) pzSpec++;
						}
						else if (_stricmp(szFieldID, _T("ignore")) == 0)
							iItemCnt++;
						else {
							_stprintf(szError, _T("DTRAN ERROR: Format specification key word %s is not valid."), szFieldID);
							_bstr_t bstrTemp(szError);
							m_listErrors.push_back(bstrTemp);
						}
						i = 0;
						if (*pzSpec) pzSpec++;
						memset(szFieldID, 0, BUFF_LEN);
						memset(szFieldTemp, 0, BUFF_LEN);
					}
				}
				if (pzSpec) {
					pzSpec = NULL;
					delete [] pzSpec;
				}
				//error check
				if (m_iFieldPos[nNAME] == -99 && m_iFieldPos[nTYPE] == -99)
					m_listErrors.push_back(L"Format file specification needs either a name or type field.  Both were missing.");
				
				if (m_listErrors.size() == 0) bSuccess = VARIANT_TRUE;
		
				//load
				if (bSuccess) {
					bSuccess = LoadFormatFromFile(szFile, iStartPos);
				}

				//adjust for rules
				if (bSuccess) {
					if (m_iFieldPos[nNAME] == -99)  //field names will be missing...use Generic Col001, Col002, ...
						ReplaceColNames(&bSuccess);
					if (m_iFieldPos[nSTART] == -99 && m_iFieldPos[nEND] == -99 && bSuccess)  //start and stop positions are missing...adjust
						bSuccess = CalcStartandEndPositions();
					else if (m_iFieldPos[nEND] == -99 && bSuccess)							//just stop is missing...adjust
						bSuccess = CalcStartandEndPositions(true);
				}
				//validate
				if (bSuccess)
					ValidFieldCollection(&bSuccess);
				if (!bSuccess) Clear();
			}
			else {
				_stprintf(szError, _T("DTRAN ERROR: Format Spec field is empty."));
				_bstr_t bstrTemp(szError);
				m_listErrors.push_back(bstrTemp);
			}
		}
		else {
			_stprintf(szError, _T("DTRAN ERROR: File : %s can not be opened."), szFile);
			_bstr_t bstrTemp(szError);
			m_listErrors.push_back(bstrTemp);
		}
	}
	catch(_com_error &e) {
		bstrErrMsg = Utilities::ConstructErrMsg(L"COM ERROR: CFormat::LoadCust() catch statement." , e.Source(), e.Description(), e.ErrorMessage());
		m_listErrors.push_back(bstrErrMsg);
	}
	catch (...) {
		bstrErrMsg = L"DTRAN ERROR: CFormat::LoadCust() catch statement.";
		m_listErrors.push_back(bstrErrMsg);
	}
	if (m_listErrors.size() == 0) 
		bSuccess = VARIANT_TRUE;
	else
		bSuccess = VARIANT_FALSE;

	*pRetVal = bSuccess;
	return S_OK;
}
	
VARIANT_BOOL CFormat::LoadBCPFormatFile(_bstr_t bstrFile)
{
	VARIANT_BOOL bSuccess  = VARIANT_FALSE;
	TCHAR szError[MAX_ERR_LEN];
	memset(szError, 0, MAX_ERR_LEN);

	m_iFieldPos[0] = 7;	//field name position
	m_iFieldPos[1] = 2;    //field type position
	m_iFieldPos[2] = -99;	//start field length position  -- doesn't have this data
	m_iFieldPos[3] = -99;   //end field length position -- doesn't have this data
	m_iFieldPos[4] = 4;	//field length position
	m_iFieldPos[5] = -99;  //precision positon 
	m_iFieldPos[6] = -99;  //scale positon 

	TCHAR szFile[BUFF_MED]; memset(szFile, 0, BUFF_MED);
	int iStrLen =  SysStringLen(bstrFile);
	WideCharToMultiByte(CP_ACP, 0, bstrFile, iStrLen, szFile, iStrLen+1, NULL, NULL);
	szFile[iStrLen]= '\0';

	bSuccess = LoadFormatFromFile(szFile, 3);

	if (bSuccess) 
		bSuccess = CalcStartandEndPositions();
	return bSuccess;
}

VARIANT_BOOL CFormat::LoadTNSFormatFile(_bstr_t bstrFile)
{
	VARIANT_BOOL bSuccess  = VARIANT_FALSE;
	//for now until we get FormatSpec object specified...holds positions in fmt file
	m_iFieldPos[0] = 1;	//field name position
	m_iFieldPos[1] = 2;    //field type position
	m_iFieldPos[2] = 3;	//start field length position
	m_iFieldPos[3] = 4;    //end field length position
	m_iFieldPos[4] = 5;	//field length position
	m_iFieldPos[5] = 6;    //precision position
	m_iFieldPos[6] = 7;	//scale position

	TCHAR szFile[BUFF_MED]; memset(szFile, 0, BUFF_MED);
	int iStrLen =  SysStringLen(bstrFile);
	WideCharToMultiByte(CP_ACP, 0, bstrFile, iStrLen, szFile, iStrLen+1, NULL, NULL);
	szFile[iStrLen]= '\0';

	bSuccess = LoadFormatFromFile(szFile, 1);
	return bSuccess;
}

void CFormat::SetFieldPosArray_TNS()
{
	SetNamePos(1);  //name is first field in TNS format...
	SetTypePos(2);
	SetStartPos(3);
	SetEndPos(4);
	SetLengthPos(5); 
	SetPrecisionPos(6); 
	SetScalePos(7);
}

void CFormat::SetFieldPosArray_BCP()
{
	SetNamePos(7);  //name is seventh field in BCP format...
	SetTypePos(2);
	SetStartPos(-99);
	SetEndPos(-99);
	SetLengthPos(4); 
	SetPrecisionPos(-99); 
	SetScalePos(-99);
}

bool CFormat::FieldPosArrayWasSet() 
{
	bool bRet = false;
	for (int i = 0; i < FIELDPOSARRAYNUM; i++) {
		if (m_iFieldPos[i] != -99)
			bRet = true;
	}
	return bRet;
}

STDMETHODIMP CFormat::Load(BSTR FormatFileName, FORMATOPTION option, VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	_bstr_t bstrFileIn = FormatFileName;
	_bstr_t bstrFile = L"";
	_bstr_t bstrErrMsg = L"";

	m_vecFieldInfo.clear();
	TCHAR szError[MAX_ERR_LEN];
	memset(szError, 0, MAX_ERR_LEN);

	try {

		if (wcscmp(bstrFileIn, L"") != 0 )
			bstrFile = Utilities::ConstructFullPath(m_bstrDirectory, bstrFileIn);
		else 
			bstrFile = Utilities::ConstructFullPath(m_bstrDirectory, m_bstrFileName);

		TCHAR szFile[BUFF_MED]; memset(szFile, 0, BUFF_MED);
		int iStrLen =  SysStringLen(bstrFile);
		WideCharToMultiByte(CP_ACP, 0, bstrFile, iStrLen, szFile, iStrLen+1, NULL, NULL);
		szFile[iStrLen]= '\0';
	
		if ( Utilities::FileCanBeRead( szFile, szError )  ) {
			m_bstrFileIn = bstrFile;
			if (option == vBCPFORMAT) {
				SetFieldPosArray_BCP();
				bSuccess = LoadFormatFromFile(szFile, 3);
				if (bSuccess) 
					bSuccess = CalcStartandEndPositions();
			}
			else if (option == vTNSFORMAT ) {
				SetFieldPosArray_TNS();
				bSuccess = LoadFormatFromFile(szFile, 1);
			}
			else if (option == vDELIMFORMAT){
				bSuccess = LoadDELIMFormatFile(bstrFile);
			}
			else if (option == vCUSTFORMAT) {
				bSuccess = VARIANT_FALSE;
				m_listErrors.push_back(L"DTRAN ERROR: Please use method Format::LoadCust for vLoadCust option");
			}

			if (bSuccess) {
				m_bCalledInternally = true;
				ValidFieldCollection(&bSuccess);
				if (!bSuccess)
					Clear();
			}
		}
		else {
			_stprintf(szError, _T("DTRAN ERROR: File : %s can not be opened."), szFile);
			_bstr_t bstrTemp(szError);
			m_listErrors.push_back(bstrTemp);
		}

	}
	catch(_com_error &e) {
		bstrErrMsg = Utilities::ConstructErrMsg(L"COM ERROR: CFormat::Load() catch statement." , e.Source(), e.Description(), e.ErrorMessage());
		m_listErrors.push_back(bstrErrMsg);
	}
	catch (...) {
		bstrErrMsg = L"DTRAN ERROR: CFormat::Load() catch statement.";
		m_listErrors.push_back(bstrErrMsg);
		bSuccess = VARIANT_FALSE;
	}
	
	*pRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CFormat::AddField(BSTR Name, BSTR Length, FIELDTYPE Type, VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	bool bDuplicate = false;
	_bstr_t bstrName = Name;
	_bstr_t bstrLength = Length;

	try {
		//this method checks for duplicate names
		//it is called only from UI and currently further validation methods are not available
/*		for (int i = 0; i < m_vecFieldInfo.size(); i++) 
		{
			IFieldInfo* pFieldInfo = m_vecFieldInfo[i];
			if (pFieldInfo != NULL)
			{
				BSTR bstrTemp;
				pFieldInfo->get_FieldName(&bstrTemp);
				_bstr_t bstrNameExist = bstrTemp;
				SysFreeString(bstrTemp);
				if (wcscmp(bstrNameExist, bstrName) == 0 ) {
					bDuplicate = true;
					break;
				}
			}
		}

		if (!bDuplicate) 
			AddFieldExt(Name, Length, Type, 0, 0, &bSuccess);
*/
		//calculate start and end length of field
		long lastEnd = 0;
		int iSize = m_vecFieldInfo.size();
		if (iSize != 0) {
			IFieldInfo* pFieldInfo = m_vecFieldInfo[iSize-1];
			if (pFieldInfo != NULL)
				pFieldInfo->get_End(&lastEnd);
		}

		//if type doesn't have length -- supply here based on data type and default length

		long lLen = _wtoi((WCHAR*)bstrLength);
		long lStart = lastEnd + 1;
		long lEnd = lStart + lLen - 1;

		AddFieldExt(Name, Length, Type, lStart, lEnd, 0, 0, &bSuccess);
	}
	catch(_com_error &e) {
		_bstr_t bstrErr = Utilities::ConstructErrMsg(L"COM ERROR: CFormat::AddField() catch statement", e.Source(), e.Description(), e.ErrorMessage());
		m_listErrors.push_back(bstrErr);
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CFormat::AddField() catch statement");
		bSuccess = VARIANT_FALSE;
	}


	*pRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CFormat::AddFieldExt(BSTR Name, BSTR Length, FIELDTYPE Type,  long lStartPos, long lEndPos, long lPrecision, long lScale, VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	_bstr_t bstrName = Name;
	bool bDuplicate = false;

	TCHAR szError[MAX_ERR_LEN];
	memset(szError, 0, MAX_ERR_LEN);
	
	try {
		//create object if we don't already have an object by that name in the vector
		//populate
		IFieldInfo* pFieldInfo = NULL;
		CoCreateInstance(CLSID_FieldInfo, NULL, CLSCTX_INPROC_SERVER, IID_IFieldInfo, (void**) &pFieldInfo);

		//if type doesn't have length -- supply here based on data type and default length

		if (pFieldInfo != NULL)
		{
			pFieldInfo->put_Name(Name); 
			pFieldInfo->put_Length(Length); 
			pFieldInfo->put_Type(Type); 
			pFieldInfo->put_End(lEndPos); 
			pFieldInfo->put_Start(lStartPos); 
			pFieldInfo->put_Precision(lPrecision);
			pFieldInfo->put_Scale(lScale);

			IFieldInfo* pRaw = NULL;
			HRESULT hr = pFieldInfo->QueryInterface(IID_IFieldInfo,(void**)&pRaw);	
			if (SUCCEEDED(hr))
			{
				m_vecFieldInfo.push_back(pRaw);
				if (pFieldInfo != NULL)
					pFieldInfo->AddRef();
				bSuccess = VARIANT_TRUE;
				m_iCurrIndex = m_vecFieldInfo.size()-1;
			}
			else  {
				_stprintf(szError, _T("DTRANERROR: CFormat::AddFieldExt() Can not add field object.  HRESULT:  %d on QueryInterface call."), hr);
				_bstr_t bstrTemp(szError);
				m_listErrors.push_back(bstrTemp);	
			}
		}
		else {
			m_listErrors.push_back(L"DTRAN ERROR: CFormat::AddFieldExt() Can not add field object.  It is is null.");
		}
	}
	catch(_com_error &e) {
		_bstr_t bstrErr = Utilities::ConstructErrMsg(L"COM ERROR: CFormat::AddFieldExt() catch statement", e.Source(), e.Description(), e.ErrorMessage());
		m_listErrors.push_back(bstrErr);
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CFormat::AddFieldExt() catch statement");
		bSuccess = VARIANT_FALSE;
	}
	
	*pRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CFormat::get_FieldCount(long *pVal)
{
	*pVal = m_vecFieldInfo.size();

	return S_OK;
}

STDMETHODIMP CFormat::TotalFieldLength(long *pVal)
{
	USES_CONVERSION;
	IFieldInfo* pFieldInfo = NULL;
	long lTotal = 0;
	for (int i = 0; i < (int) m_vecFieldInfo.size(); i++) 
	{
		pFieldInfo = m_vecFieldInfo[i];
		if (pFieldInfo != NULL)
		{
			BSTR bTemp;
			pFieldInfo->get_Length(&bTemp);
			_bstr_t bstrTemp = bTemp;
			long lLen= _ttoi( W2A(bTemp) );
			SysFreeString(bTemp);
			lTotal += lLen;
		}
	}
	*pVal = lTotal;
	return S_OK;
} 

STDMETHODIMP CFormat::Clear()
{
	IFieldInfo* pFieldInfo = NULL;
	if (!m_bCalledInternally)
		m_listErrors.clear();
	
	try {

		for (int i = 0; i < (int) m_vecFieldInfo.size(); i++) 
		{
			pFieldInfo = m_vecFieldInfo[i];
			if (pFieldInfo != NULL)
			{
				pFieldInfo->Release();
				pFieldInfo = NULL;
			}
		}
		
		m_vecFieldInfo.clear();
		m_iCurrIndex = -1;
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CFormat::Clear() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CFormat::Clear() catch statement.");
	}
	
	return S_OK;
}

STDMETHODIMP CFormat::get_FileName(BSTR *pVal)
{
	// TODO: Add your implementation code here
	*pVal = m_bstrFileName.copy();
	return S_OK;
}

STDMETHODIMP CFormat::put_FileName(BSTR newVal)
{
	m_bstrFileName = newVal;

	return S_OK;
}

STDMETHODIMP CFormat::get_Directory(BSTR *pVal)
{
	*pVal = m_bstrDirectory.copy();

	return S_OK;
}

STDMETHODIMP CFormat::put_Directory(BSTR newVal)
{
	m_listErrors.clear();
	try {
		_bstr_t bstrDir = newVal;

		if (wcscmp(bstrDir, L"") != 0 )
			m_bstrDirectory = newVal;

		//as a rule the directory path is stored w/out a backslash
		TCHAR bufOut[512];
		memset(bufOut, 5122, 0);

		TCHAR szDir[BUFF_MED]; memset(szDir, 0, BUFF_MED);
		int iStrLen =  SysStringLen(m_bstrDirectory);
		WideCharToMultiByte(CP_ACP, 0, m_bstrDirectory, iStrLen, szDir, iStrLen+1, NULL, NULL);
		szDir[iStrLen]= '\0';

		if (Utilities::RemoveLastSlash(szDir, bufOut) > 0 ) {
			_bstr_t bstrTemp(bufOut);
			m_bstrDirectory = bstrTemp;
		}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CFormat::put_Directory() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CFormat::put_Directory() catch statement.");
	}

	return S_OK;
}

HRESULT CFormat::DeleteItem(int iIndex)
{
	HRESULT hr = E_FAIL;
	m_listErrors.clear();

	try {

		if ((int) m_vecFieldInfo.size() != 0  && iIndex != -1 && iIndex != (int) m_vecFieldInfo.size() && !(iIndex > (int) m_vecFieldInfo.size())  )
		{
			IFieldInfo* pField = m_vecFieldInfo[iIndex];
			if (pField != NULL)
				pField->Release();
			pField = NULL;

			int iOrigSize = m_vecFieldInfo.size();
			//now move everyone else up by one
			for (int i = iIndex; i < iOrigSize-1; i++)
				m_vecFieldInfo[i] = m_vecFieldInfo[i+1];

			m_vecFieldInfo.pop_back();
			int iCnt = m_vecFieldInfo.size();

			//reset current if we have deleted the last one added to the vector or no more are left
			if (m_iCurrIndex == m_vecFieldInfo.size() )
				m_iCurrIndex -= 1;
			hr = S_OK;
		}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CFormat::DeleteItem() catch statement", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CFormat::DeleteItem() catch statement.");
	}

	return hr;
}

STDMETHODIMP CFormat::DeleteCurr(VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_listErrors.clear();
	
	try {

		if (SUCCEEDED(DeleteItem(m_iCurrIndex)))
			bSuccess = VARIANT_TRUE;
		else
			m_listErrors.push_back(L"DTRAN ERROR: Current field could not be deleted.");
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CFormat::DeleteCurr() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CFormat::DeleteCurr() catch statement");
	}

	*pRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CFormat::DeleteByName(BSTR Name, VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_listErrors.clear();
	_bstr_t bstrName = Name;
	IFieldInfo* pFieldInfo = NULL;
	
	try {

		for (int i = 0; i < (int) m_vecFieldInfo.size(); i++) 
		{
			pFieldInfo = m_vecFieldInfo[i];
			if (pFieldInfo != NULL)
			{
				BSTR bstrTemp;
				pFieldInfo->get_Name(&bstrTemp);
				_bstr_t bstrNameExist = bstrTemp;
				SysFreeString(bstrTemp);
				if (wcscmp(bstrNameExist, bstrName) == 0 ) {
					if (SUCCEEDED(DeleteItem(i))) {
						bSuccess = VARIANT_TRUE;
						m_iCurrIndex = 0;
					}
					break;
				}
			}
		}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CFormat::DeleteByName() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CFormat::DeleteByName() catch statement.");
	}

	pFieldInfo = NULL;
	*pRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CFormat::DeleteByPos(long Position, VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_listErrors.clear();

	try {
		if (SUCCEEDED(DeleteItem(Position)))
			bSuccess = VARIANT_TRUE;
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CFormat::DeleteByPos() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CFormat::DeleteByPos() catch statement.");
	}

	*pRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CFormat::First(IFieldInfo **pObj)
{
	IFieldInfo* pField = NULL;
	m_listErrors.clear();

	try {	
		if (m_vecFieldInfo.size() != 0 )
		{
			m_iCurrIndex = 0;
			pField = m_vecFieldInfo[m_iCurrIndex];
			if (pField != NULL)
				pField->AddRef();
		}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CFormat::First() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CFormat::First() catch statement.");
	}

	*pObj = pField;

	return S_OK;
}

STDMETHODIMP CFormat::Next(IFieldInfo **pObj)
{
	IFieldInfo* pField = NULL;
	m_listErrors.clear();
	
	try {
		//if we have already given out the last one then hand back NULL
		if ( m_iCurrIndex >= 0 && (m_iCurrIndex+1) != m_vecFieldInfo.size() )
		{
			pField = m_vecFieldInfo[++m_iCurrIndex];
			if (pField != NULL)
				pField->AddRef();
		}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CFormat::Next() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CFormat::Next() catch statement.");
	}

	*pObj = pField;

	return S_OK;
}

STDMETHODIMP CFormat::Last(IFieldInfo **pObj)
{
	IFieldInfo* pField = NULL;
	m_listErrors.clear();
	
	try {
		if ( m_vecFieldInfo.size() != 0 )
		{
			int iSize = m_vecFieldInfo.size();
			m_iCurrIndex = iSize-1;
			pField = m_vecFieldInfo[m_iCurrIndex];
			if (pField != NULL)
				pField->AddRef();
		}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CFormat::Last() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CFormat::Last() catch statement.");
	}

	*pObj = pField;
	return S_OK;
}

STDMETHODIMP CFormat::Prev(IFieldInfo **pObj)
{
	IFieldInfo* pField = NULL;
	m_listErrors.clear();
	
	try {

		//if we have already given out the first then hand back NULL
		if ( m_vecFieldInfo.size() > 0 && m_iCurrIndex > 0 )
		{
			pField = m_vecFieldInfo[--m_iCurrIndex];
			if (pField != NULL)
				pField->AddRef();
		}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CFormat::Prev() catch statement", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CFormat::Prev() catch statement.");
	}

	*pObj = pField;
	return S_OK;
}

STDMETHODIMP CFormat::GetByName(BSTR Name, IFieldInfo **pObj)
{
	_bstr_t bstrName = Name;
	m_listErrors.clear();
	bool bFound = false;
	IFieldInfo* pFieldInfo = NULL;

	try {
		
		for (int i = 0; i < (int) m_vecFieldInfo.size(); i++) 
		{
			pFieldInfo = m_vecFieldInfo[i];
			if (pFieldInfo != NULL)
			{
				BSTR bstrTemp;
				pFieldInfo->get_Name(&bstrTemp);
				_bstr_t bstrNameExist = bstrTemp;
				SysFreeString(bstrTemp);
				if (wcscmp(bstrNameExist, bstrName) == 0 ) {
					pFieldInfo->AddRef();
					m_iCurrIndex = i;
					bFound = true;
					break;
				}
			}
			pFieldInfo = NULL;
		}

		if (!bFound)
			pFieldInfo = NULL;
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CFormat::GetByName() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch(...) {
		m_listErrors.push_back(L"DTRAN ERROR: CFormat::GetByName() catch statement.");
	}

	*pObj = pFieldInfo;
	return S_OK;
}

STDMETHODIMP CFormat::GetByPos(long Position, IFieldInfo **pObj)
{
	IFieldInfo* pField = NULL;
	m_listErrors.clear();

	try {
		
		//if we have already given out the first then hand back NULL
		if ( Position >= 0 && Position < (int) m_vecFieldInfo.size()  )
		{
			pField = m_vecFieldInfo[Position];
			if(pField != NULL)
				pField->AddRef();
			m_iCurrIndex = Position;
		}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CFormat::GetByPos() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch(...) {
		m_listErrors.push_back(L"DTRAN ERROR:CFormat::GetByPos() catch statement.");
	}

	*pObj = pField;
	return S_OK;
}

VARIANT_BOOL CFormat::WriteBCPFormatFile(_bstr_t bstrFile)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_bstrBCPFormatLoc = L"";
	m_listErrors.clear();

	FILE* ptrfile = NULL;
	TCHAR buffer[BUFF_LEN*2];
	memset(buffer, 0, BUFF_LEN*2);
	TCHAR szError[MAX_ERR_LEN];
	memset(szError, 0, MAX_ERR_LEN);

	try 
	{
		if ( ( ptrfile = fopen(bstrFile, "w")) != NULL) 
		{
			//write out header lines
			_stprintf(buffer, "8.0\r\n%d\r\n", m_vecFieldInfo.size());
			_ftprintf(ptrfile, buffer);
			fflush(ptrfile);

			int iColCount = 0;

			for (int index = 0; index < (int) m_vecFieldInfo.size(); index++)
			{	
				iColCount = index + 1;
				IFieldInfo* pField = m_vecFieldInfo[index];

				if (pField != NULL)
				{
					BSTR bTemp;
					pField->get_Length(&bTemp);
					_bstr_t bstrTemp = bTemp;
					SysFreeString(bTemp);

					TCHAR szTemp[BUFF_LEN]; memset(szTemp, 0, BUFF_LEN);
					int iStrLen =  SysStringLen(bstrTemp);
					WideCharToMultiByte(CP_ACP, 0, bstrTemp, iStrLen, szTemp, iStrLen+1, NULL, NULL);
					szTemp[iStrLen]= '\0';
					long lFieldLen= _ttoi( szTemp );

					long lLen= _ttoi( szTemp);
				
					pField->get_Name(&bTemp);
					_bstr_t bstrName = bTemp;
					SysFreeString(bTemp);

					TCHAR szName[BUFF_LEN]; memset(szName, 0, BUFF_LEN);
					iStrLen =  SysStringLen(bstrName);
					WideCharToMultiByte(CP_ACP, 0, bstrName, iStrLen, szName, iStrLen+1, NULL, NULL);
					szName[iStrLen]= '\0';

					if (( index == m_vecFieldInfo.size() - 1 ) && m_bCarRetInBCP == VARIANT_TRUE ) //we are on the last one -- write out last line
					{
						if (m_bBCPDelim == VARIANT_TRUE) 
							_stprintf(buffer, "%d       SQLCHAR       0       0       \"\\r\\n\"                        %d     %s          SQL_Latin1_General_CP1_CI_AS\n", 
							iColCount, iColCount, szName);
						else 
							_stprintf(buffer, "%d       SQLCHAR       0       %d       \"\\r\\n\"                        %d     %s          SQL_Latin1_General_CP1_CI_AS\n", 
							iColCount, lLen, iColCount, szName);
					}
					else
					{
						TCHAR szDelim[BUFF_LEN]; memset(szDelim, 0, BUFF_LEN);
						int iStrLen =  SysStringLen(m_bstrFldDelim);
						WideCharToMultiByte(CP_ACP, 0, m_bstrFldDelim, iStrLen, szDelim, iStrLen+1, NULL, NULL);
						szDelim[iStrLen]= '\0';

						if (m_bBCPDelim == VARIANT_TRUE) 
							_stprintf(buffer, "%d       SQLCHAR       0       0       \"%s\"                        %d     %s          SQL_Latin1_General_CP1_CI_AS\n", 
							iColCount, szDelim, iColCount, szName);
						else 
							_stprintf(buffer, _T("%d       SQLCHAR       0       %d       \"\"                        %d     %s          SQL_Latin1_General_CP1_CI_AS\n"), 
							iColCount, lLen, iColCount, szName);
					}

					fprintf(ptrfile, buffer);
					fflush(ptrfile);
				}
			}

			fclose(ptrfile);

			if (iColCount == m_vecFieldInfo.size() )
			{
				bSuccess = VARIANT_TRUE;
				m_bstrBCPFormatLoc = bstrFile;
			}
			else
				m_listErrors.push_back(L"DTRAN ERROR: CFormat::WriteBCPFormatFile(). Problem with Field objects, not all written to format file.");

		}
		else 
		{
			TCHAR szFile[BUFF_LEN]; memset(szFile, 0, BUFF_LEN);
			int iStrLen =  SysStringLen(bstrFile);
			WideCharToMultiByte(CP_ACP, 0, bstrFile, iStrLen, szFile, iStrLen+1, NULL, NULL);
			szFile[iStrLen]= '\0';

			_stprintf(szError, _T("DTRAN ERROR: CFormat::WriteBCPFormatFile. File:  %s could not be written to."), szFile);
			_bstr_t bstrErr(szError);
			m_listErrors.push_back(bstrErr);	
		}

	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CFormat::WriteBCPFormatFile catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR:CFormat::WriteBCPFormatFile catch statement.");
	}

	return bSuccess;
}

LPTSTR CFormat::MapFieldTypeToText(FIELDTYPE nType, LPTSTR lpOut)
{
	LPTSTR lpType; 
	TCHAR szBuffer[BUFF_LEN]; memset(szBuffer, 0, BUFF_LEN);
	switch (nType)
	{
		case vBIGINT:
			lpType = _T("bigint");
		break;
		case vBINARY:
			lpType = _T("binary");
		break;
		case vBIT:
			lpType = _T("bit");
		break;
		case vCHAR:
			lpType = _T("char");
		break;
		case vDATETIME:
			lpType = _T("datetime");
		break;
		case vDECIMAL:
			lpType = _T("decimal");
		break;
		case vFLOAT:
			lpType = _T("float");
		break;
		case vINT:
			lpType = _T("int");
		break;
		case vMONEY:
			lpType = _T("money");
		break;
		case vNUMERIC:
			lpType = _T("numeric");
		break;
		case vREAL:
			lpType = _T("real");
		break;
		case vSMALLDATETIME:
			lpType = _T("smalldatetime");
		break;
		case vSMALLINT:
			lpType = _T("smallint");
		break;
		case vSMALLMONEY:
			lpType = _T("smallmoney");
		break;
		case vTEXT:
			lpType = _T("text");
		break;
		case vTIMESTAMP:
			lpType = _T("timestamp");
		break;
		case vTINYINT:
			lpType = _T("tinyint");
		break;
		default:
			lpType = _T("varchar");
		break;
	}
	return _tcscpy(lpOut, lpType);
}

void CFormat::InitializeFieldPosArray()
{
	for (int i = 0; i < FIELDPOSARRAYNUM; i++) {
		m_iFieldPos[i] = -99;
	}
}

FIELDNAME CFormat::GetFieldNameFromOrder(int iOrderPos)
{
	//this routine finds the array index of value
	int index = -99;
	for (int i = 0; i < FIELDPOSARRAYNUM; i++) {
		if (m_iFieldPos[i] == iOrderPos)
			return (FIELDNAME) i;
	}
	return nNONE;
 }
int CFormat::GetOutputFieldCount()
{
	int iCnt = 0;
	for (int i = 0; i < FIELDPOSARRAYNUM; i++) {
		if (m_iFieldPos[i] != -99) iCnt++;
	}
	return iCnt;
}

VARIANT_BOOL CFormat::WriteCustFormatFile(TCHAR* szFileName)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_listErrors.clear();

	FILE* ptrfile = NULL;
	TCHAR szError[MAX_ERR_LEN];
	memset(szError, 0, MAX_ERR_LEN);

	TCHAR buffer[BUFF_LEN*2];
	memset(buffer, 0, BUFF_LEN*2);

	if (!FieldPosArrayWasSet())
		SetFieldPosArray_TNS();

	try 
	{
		if ( ( ptrfile = fopen(szFileName, "w")) != NULL) 
		{
			int iColCount = 0;
			for (int index = 0; index < (int) m_vecFieldInfo.size(); index++)
			{	
				TCHAR szBuffer[BUFF_LEN]; memset(szBuffer, 0, BUFF_LEN);
				iColCount = index + 1;
				IFieldInfo* pField = m_vecFieldInfo[index];
				if (pField != NULL)
				{
					BSTR bTemp;
					pField->get_Name(&bTemp);
					_bstr_t bstrName = bTemp;
					SysFreeString(bTemp);

					TCHAR szName[BUFF_LEN]; memset(szName, 0, BUFF_LEN);
					int iStrLen =  SysStringLen(bstrName);
					WideCharToMultiByte(CP_ACP, 0, bstrName, iStrLen, szName, iStrLen+1, NULL, NULL);
					szName[iStrLen]= '\0';
					
					FIELDTYPE nType;
					pField->get_Type(&nType);  
					TCHAR szType[BUFF_LEN]; memset(szType, 0, BUFF_LEN);
					MapFieldTypeToText(nType, szType);
					szType[_tcslen(szType)+1] = '\0';
					
					pField->get_Length(&bTemp);
					_bstr_t bstrLen = bTemp;
					SysFreeString(bTemp);

					TCHAR szLen[BUFF_LEN]; memset(szLen, 0, BUFF_LEN);
					iStrLen =  SysStringLen(bstrLen);
					WideCharToMultiByte(CP_ACP, 0, bstrLen, iStrLen, szLen, iStrLen+1, NULL, NULL);
					szLen[iStrLen]= '\0';

					long lStart, lStop, lScale, lPrec;
					pField->get_Start(&lStart);
					pField->get_End(&lStop);
					pField->get_Precision(&lPrec);
					pField->get_Scale(&lScale);

					TCHAR bufTemp[20]; memset(bufTemp, 0, 20);
					TCHAR szLast[BUFF_LEN]; memset(szLast, 0, BUFF_LEN); szLast[0] = '\0';
					bool bFirst = true;
					for (int i = 1; i <= FIELDPOSARRAYNUM; i++)
					{
						FIELDNAME nFIELD = GetFieldNameFromOrder(i);
						switch (nFIELD)
						{
							case nNAME: 
								if (bFirst)
									_tcscpy(szBuffer, szName);
								else {
									if (_tcslen(szLast) > 7)
										_tcscat(szBuffer, _T("\t"));
									else
										_tcscat(szBuffer, _T("\t\t"));
									_tcscat(szBuffer, szName);
								}
								bFirst = false;
								memset(szLast, 0, BUFF_LEN);
								_tcscpy(szLast, szName);
								szLast[_tcslen(szLast)] = '\0';
							break;
							case nTYPE:
								if (bFirst)
									_tcscpy(szBuffer, szType);
								else {
									if (_tcslen(szLast) > 7)
										_tcscat(szBuffer, _T("\t"));
									else
										_tcscat(szBuffer, _T("\t\t"));
									_tcscat(szBuffer, szType);
								}
								bFirst = false;
								_tcscpy(szLast, szType);
								szLast[_tcslen(szLast)] = '\0';
							break;
							case nLENGTH:
								if (bFirst)
									_tcscpy(szBuffer, szLen);
								else {
									if (_tcslen(szLast) > 7)
										_tcscat(szBuffer, _T("\t"));
									else
										_tcscat(szBuffer, _T("\t\t"));
									_tcscat(szBuffer, szLen);
								}
								bFirst = false;
								_tcscpy(szLast, szLen);
								szLast[_tcslen(szLast)] = '\0';
							break;
							case nSTART:
								memset(bufTemp, 0, 20);
                                _ltoa(lStart, bufTemp, 10);
								bufTemp[strlen(bufTemp)+1] = '\0';
								if (bFirst)
									_tcscpy(szBuffer, bufTemp);
								else {
									if (_tcslen(szLast) > 7)
										_tcscat(szBuffer, _T("\t"));
									else
										_tcscat(szBuffer, _T("\t\t"));
									_tcscat(szBuffer, bufTemp);
								}
								bFirst = false;
								_tcscpy(szLast, bufTemp);
								szLast[_tcslen(szLast)] = '\0';
							break;
							case nEND:
								memset(bufTemp, 0, 20);
                                _ltoa(lStop, bufTemp, 10);
								bufTemp[strlen(bufTemp)+1] = '\0';
								if (bFirst)
									_tcscpy(szBuffer, bufTemp);
								else {
									if (_tcslen(szLast) > 7)
										_tcscat(szBuffer, _T("\t"));
									else
										_tcscat(szBuffer, _T("\t\t"));
									_tcscat(szBuffer, bufTemp);
								}
								bFirst = false;
								_tcscpy(szLast, bufTemp);
								szLast[_tcslen(szLast)] = '\0';
							break;
							case nPRECISION:
								memset(bufTemp, 0, 20);
                                _ltoa(lPrec, bufTemp, 10);
								bufTemp[strlen(bufTemp)+1] = '\0';
								if (bFirst)
									_tcscpy(szBuffer, bufTemp);
								else {
									if (_tcslen(szLast) > 7)
										_tcscat(szBuffer, _T("\t"));
									else
										_tcscat(szBuffer, _T("\t\t"));
									_tcscat(szBuffer, bufTemp);
								}
								bFirst = false;
								_tcscpy(szLast, bufTemp);
								szLast[_tcslen(szLast)] = '\0';
							break;
							case nSCALE:
								memset(bufTemp, 0, 20);
                                _ltoa(lScale, bufTemp, 10);
								bufTemp[strlen(bufTemp)+1] = '\0';
								if (bFirst)
									_tcscpy(szBuffer, bufTemp);
								else {
									if (_tcslen(szLast) > 7)
										_tcscat(szBuffer, _T("\t"));
									else
										_tcscat(szBuffer, _T("\t\t"));
									_tcscat(szBuffer, bufTemp);
								}
								bFirst = false;
								_tcscpy(szLast, bufTemp);
								szLast[_tcslen(szLast)] = '\0';
							break;
							default:

							break;
						}
					}
					_tcscat(szBuffer, (TCHAR*)_T("\n"));
					fprintf(ptrfile, szBuffer);
					fflush(ptrfile);
				}
			}
			fclose(ptrfile);
			if (iColCount == m_vecFieldInfo.size() )
				bSuccess = VARIANT_TRUE;
			else
				m_listErrors.push_back(L"DTRAN ERROR: CFormat::WriteCustFormatFile. Problem with Field objects, not all written to format file.");
		}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CFormat::WriteCustFormatFile catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR:CFormat::WriteCustFormatFile catch statement.");
	}

	return bSuccess;
}


VARIANT_BOOL CFormat::WriteTNSorDELIMFormatFile(_bstr_t bstrFile, FORMATOPTION option)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_listErrors.clear();

	FILE* ptrfile = NULL;
	TCHAR szError[MAX_ERR_LEN];
	memset(szError, 0, MAX_ERR_LEN);

	TCHAR buffer[BUFF_LEN*2];
	memset(buffer, 0, BUFF_LEN*2);

	try 
	{
		if ( ( ptrfile = fopen(bstrFile, "w")) != NULL) 
		{
			int iColCount = 0;
			for (int index = 0; index < (int) m_vecFieldInfo.size(); index++)
			{	
				iColCount = index + 1;
				IFieldInfo* pField = m_vecFieldInfo[index];

				if (pField != NULL)
				{
					BSTR bTemp;
					pField->get_Length(&bTemp);
					_bstr_t bstrTemp = bTemp;
					SysFreeString(bTemp);
					
					TCHAR szTemp[BUFF_LEN]; memset(szTemp, 0, BUFF_LEN);
					int iStrLen =  SysStringLen(bstrTemp);
					WideCharToMultiByte(CP_ACP, 0, bstrTemp, iStrLen, szTemp, iStrLen+1, NULL, NULL);
					szTemp[iStrLen]= '\0';

					long lLen= _ttoi( szTemp );
				
					pField->get_Name(&bTemp);
					_bstr_t bstrName = bTemp;
					SysFreeString(bTemp);

					FIELDTYPE nType;
					pField->get_Type(&nType);  
					TCHAR szType[BUFF_LEN]; memset(szType, 0, BUFF_LEN);
                    MapFieldTypeToText(nType, szType);
					szType[_tcslen(szType)+1] = '\0';
	
					long lStart, lStop, lPrec, lScale;
					lPrec = 0; lScale = 0;
					pField->get_Start(&lStart);
					pField->get_End(&lStop);
					pField->get_Precision(&lPrec);
					pField->get_Scale(&lScale);

					TCHAR szName[BUFF_LEN]; memset(szName, 0, BUFF_LEN);
					iStrLen =  SysStringLen(bstrName);
					WideCharToMultiByte(CP_ACP, 0, bstrName, iStrLen, szName, iStrLen+1, NULL, NULL);
					szName[iStrLen]= '\0';

					if (option == vTNSFORMAT ) {
						_stprintf(buffer, "%s%s%s%s%d\t%d\t%d\t%d\t%d\n", 
							szName, (_tcslen(szName) > 7 ? _T("\t") : _T("\t\t")), 
							szType, (_tcslen(szType) > 7 ? _T("\t") : _T("\t\t")),
							lStart, lStop, lLen, lPrec, lScale);
					}
					else {

						TCHAR szFldDelim[BUFF_LEN]; memset(szFldDelim, 0, BUFF_LEN);
						int iStrLen =  SysStringLen(m_bstrFldDelim);
						WideCharToMultiByte(CP_ACP, 0, m_bstrFldDelim, iStrLen, szFldDelim, iStrLen+1, NULL, NULL);
						szFldDelim[iStrLen]= '\0';

						if (index + 1 == m_vecFieldInfo.size() )
							_stprintf(buffer, "%s\n", szName, szFldDelim);
						else
							_stprintf(buffer, "%s%s ", szName, szFldDelim);
					}

					fprintf(ptrfile, buffer);
					fflush(ptrfile);
				}
			}

			fclose(ptrfile);

			if (iColCount == m_vecFieldInfo.size() )
				bSuccess = VARIANT_TRUE;
			else
				m_listErrors.push_back(L"DTRAN ERROR: CFormat::WriteTNSorDELIMFormatFile. Problem with Field objects, not all written to format file.");
		}
		else 
		{
			TCHAR szFile[BUFF_LEN]; memset(szFile, 0, BUFF_LEN);
			int iStrLen =  SysStringLen(bstrFile);
			WideCharToMultiByte(CP_ACP, 0, bstrFile, iStrLen, szFile, iStrLen+1, NULL, NULL);
			szFile[iStrLen]= '\0';

			_stprintf(szError, _T("DTRAN ERROR: CFormat::WriteTNSorDELIMFormatFile. File:  %s could not be written to."), szFile);
			_bstr_t bstrTemp(szError);
			m_listErrors.push_back(bstrTemp);	
		}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CFormat::WriteTNSorDELIMFormatFile catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CFormat::WriteTNSorDELIMFormatFile catch statement.");
	}

	return bSuccess;
}

STDMETHODIMP CFormat::Write(BSTR FileName, FORMATOPTION option, VARIANT_BOOL *pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	_bstr_t bstrFileIn = FileName;
	_bstr_t bstrFile = L"";
	m_listErrors.clear();

	try {

		if (wcscmp(bstrFileIn, L"") != 0 ) 
		{
			if ( Utilities::IsFullPath(bstrFileIn) || wcscmp(m_bstrDirectory, L"") != 0 )
				bstrFile = Utilities::ConstructFullPath(m_bstrDirectory, bstrFileIn);
		}
		else 
		{
			if ( Utilities::IsFullPath(m_bstrFileName) || wcscmp(m_bstrDirectory, L"") != 0 )
				bstrFile = Utilities::ConstructFullPath(m_bstrDirectory, m_bstrFileName);
		}

		if (wcscmp(bstrFile, L"") != 0 )
		{		
			TCHAR szFileName[MAX_FILE_LEN]; memset(szFileName, 0, MAX_FILE_LEN);
			Utilities::AddExtension(bstrFile, _T("fmt"), szFileName);
			_bstr_t bstrFileName(szFileName);
			if (option == vBCPFORMAT)
				bSuccess = WriteBCPFormatFile(bstrFileName);
			else if (option == vCUSTFORMAT)
				bSuccess = WriteCustFormatFile(szFileName);
			else 
				bSuccess = WriteTNSorDELIMFormatFile(bstrFileName, option);
		}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CFormat::Write() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch(...) {
		m_listErrors.push_back(L"DTRAN ERROR: CFormat::Write() catch statement.");
	}
	
	*pRetVal = bSuccess;
	return S_OK;
}


STDMETHODIMP CFormat::get_BCPFormatFileLoc(BSTR *pVal)
{
	*pVal = m_bstrBCPFormatLoc.copy();

	return S_OK;
}

STDMETHODIMP CFormat::put_BCPFormatFileLoc(BSTR newVal)
{

	m_bstrBCPFormatLoc = newVal;

	return S_OK;
}


STDMETHODIMP CFormat::get_RowDelim(BSTR *pVal)
{
	*pVal = m_bstrRowDelim.copy();
	return S_OK;
}

STDMETHODIMP CFormat::put_RowDelim(BSTR newVal)
{
	m_bstrRowDelim = newVal;
	return S_OK;
}

STDMETHODIMP CFormat::get_FldDelim(BSTR *pVal)
{
	*pVal = m_bstrFldDelim.copy();
	return S_OK;
}

STDMETHODIMP CFormat::put_TextQualifier(BSTR newVal)
{
	m_bstrTextQualifier = newVal;
	return S_OK;
}
STDMETHODIMP CFormat::get_TextQualifier(BSTR *pVal)
{
	*pVal = m_bstrTextQualifier.copy();
	return S_OK;
}

STDMETHODIMP CFormat::put_FldDelim(BSTR newVal)
{
	m_bstrFldDelim = newVal;
	return S_OK;
}

STDMETHODIMP CFormat::put_CarRetInBCP(VARIANT_BOOL newVal)
{
	m_bCarRetInBCP = newVal; 
	return S_OK;
}

STDMETHODIMP CFormat::put_BCPDelim(VARIANT_BOOL newVal)
{
	m_bBCPDelim = newVal; 
	return S_OK;
}

STDMETHODIMP CFormat::RecordLength(long *pVal)
{
	long lLength = 0;
	m_listErrors.clear();

	try {
		for (int index = 0; index < (int) m_vecFieldInfo.size(); index++)
		{	
			IFieldInfo* pField = m_vecFieldInfo[index];

			if (pField != NULL)
			{
				BSTR bTemp;
				pField->get_Length(&bTemp);
				_bstr_t bstrTemp = bTemp;
				SysFreeString(bTemp);
				
				TCHAR szTemp[BUFF_LEN]; memset(szTemp, 0, BUFF_LEN);
				int iStrLen =  SysStringLen(bstrTemp);
				WideCharToMultiByte(CP_ACP, 0, bstrTemp, iStrLen, szTemp, iStrLen+1, NULL, NULL);
				szTemp[iStrLen]= '\0';

				lLength += _ttoi( szTemp );
			}
		}
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CFormat::RecordLength() catch statement.");
	}

	*pVal = lLength;
	return S_OK;
}


STDMETHODIMP CFormat::ReCalcLengths(VARIANT_BOOL * pRetVal)
{
	VARIANT_BOOL bSuccess = VARIANT_TRUE;
	TCHAR szError[MAX_ERR_LEN];
	memset(szError, 0, MAX_ERR_LEN);

	m_listErrors.clear();

	try {

		long lastEnd = 0;
		for (int i = 0; i < (int) m_vecFieldInfo.size(); i++) 
		{
			IFieldInfo* pFieldOrig = m_vecFieldInfo[i];
			if (pFieldOrig != NULL)
			{
				BSTR bTemp;
				pFieldOrig->get_Length(&bTemp);
				_bstr_t bstrLen = bTemp;
				SysFreeString(bTemp);

				pFieldOrig->get_Name(&bTemp);
				_bstr_t bstrName = bTemp;
				SysFreeString(bTemp);

				if (wcscmp(bstrLen, L"") != 0 ) {
	
					long lLen = _wtoi((WCHAR*)bstrLen);
					long lStart = lastEnd + 1;
					long lEnd = lStart + lLen - 1;

					pFieldOrig->put_Start(lStart);
					pFieldOrig->put_End(lEnd);
					lastEnd = lEnd;
				}
				else {
					bSuccess = VARIANT_FALSE;
					TCHAR szName[BUFF_LEN]; memset(szName, 0, BUFF_LEN);
					int iStrLen =  SysStringLen(bstrName);
					WideCharToMultiByte(CP_ACP, 0, bstrName, iStrLen, szName, iStrLen+1, NULL, NULL);
					szName[iStrLen]= '\0';
					_stprintf(szError, _T("DTRAN ERROR: Field: %s does not have a valid length."), szName);
					_bstr_t bstrTemp(szError);
					m_listErrors.push_back(bstrTemp);
				}
			}
			else
				bSuccess = VARIANT_FALSE;
		}
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CFormat::ReCalcLengths() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CFormat::ReCalcLengths() catch statement.");
	}

	*pRetVal = bSuccess;
	return S_OK;
}


STDMETHODIMP CFormat::CopyFormat(IFormat *pFormat, VARIANT_BOOL *pRetVal)
{
	//copy the incoming format object to this format object..this is a deep copy
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	m_listErrors.clear();

	try {

		if (pFormat != NULL)
		{
			BSTR bTemp;
			pFormat->get_Directory(&bTemp);
			m_bstrDirectory = bTemp;
			SysFreeString(bTemp);

			pFormat->get_FileName(&bTemp);
			m_bstrFileName = bTemp;
			SysFreeString(bTemp);

			pFormat->get_BCPFormatFileLoc(&bTemp);
			m_bstrBCPFormatLoc = bTemp;
			SysFreeString(bTemp);

			//copy vector
			Clear();

			IFieldInfo* pField = NULL;
			pFormat->First(&pField);

			while (pField != NULL)
			{
				VARIANT_BOOL bValue = VARIANT_TRUE;
		
				//this is clunky but for now until we ensure that this works...then add CopyFieldInfo function
				pField->get_Name(&bTemp);
				_bstr_t bstrName = bTemp;
				SysFreeString(bTemp);

				pField->get_Length(&bTemp);
				_bstr_t bstrLength = bTemp;
				SysFreeString(bTemp);

				FIELDTYPE nTempType;
				pField->get_Type(&nTempType);

				long lStartPos = 0;
				pField->get_Start(&lStartPos);

				long lEndPos = 0;
				pField->get_End(&lEndPos);

				long lPrecision = 0;
				pField->get_Precision(&lPrecision);

				long lScale = 0;
				pField->get_Scale(&lScale);

				AddFieldExt(bstrName, bstrLength, nTempType, lStartPos, lEndPos, lPrecision, lScale, &bValue);

//?				if (!bValue)
//					bSuccess = VARIANT_FALSE;

				pFormat->Next(&pField);
			}
			bSuccess = VARIANT_TRUE;

		}
		else
			m_listErrors.push_back(L"DTRAN ERROR: Can not copy Format object.  It is NULL.");
	}
	catch(_com_error &e) {
		m_listErrors.push_back(Utilities::ConstructErrMsg(L"COM ERROR: CFormat::CopyFormat() catch statement.", e.Source(), e.Description(), e.ErrorMessage()));
	}
	catch (...) {
		m_listErrors.push_back(L"DTRAN ERROR: CFormat::CopyFormat() catch statement.");
	}

	*pRetVal = bSuccess;
	return S_OK;
}

STDMETHODIMP CFormat::get_ErrMsgList(BSTR *pVal)
{
	_bstr_t bstrErrors = L"";
	try {

		bstrErrors = Utilities::GetErrorList(m_listErrors, true);
	}
		catch(_com_error &e) {
		bstrErrors = Utilities::ConstructErrMsg(L"COM ERROR: CFormat::get_ErrMsgList() catch statement.", e.Source(), e.Description(), e.ErrorMessage());
	}
	catch (...) {
		bstrErrors = L"DTRAN ERROR: CFormat::get_ErrMsgList() catch statement.";
	}

	*pVal = bstrErrors.copy();
	return S_OK;
}

STDMETHODIMP CFormat::GetFirstError(BSTR *pValOut, VARIANT_BOOL *pVal)
{
	VARIANT_BOOL bMore = VARIANT_FALSE;
	_bstr_t bstrErr = L"";
	m_iErrIndex = 0;

	if (m_listErrors.size() > 0 ) {
		bstrErr = Utilities::GetError(m_iErrIndex++, m_listErrors);
		if (m_iErrIndex < (int) m_listErrors.size())
			bMore = VARIANT_TRUE;
	}

	*pValOut = bstrErr.copy();
	*pVal = bMore;

	return S_OK;
}

STDMETHODIMP CFormat::GetNextError(BSTR *pValOut, VARIANT_BOOL *pVal)
{
	VARIANT_BOOL bMore = VARIANT_FALSE;
	_bstr_t bstrErr = L"";

	if (m_iErrIndex < (int) m_listErrors.size()) {
		bstrErr = Utilities::GetError(m_iErrIndex, m_listErrors);
		if (m_iErrIndex < (int) m_listErrors.size()) {
			bMore = VARIANT_TRUE;
			m_iErrIndex++;
		}
	}

	*pValOut = bstrErr.copy();
	*pVal = bMore;
	return S_OK;
}

STDMETHODIMP CFormat::get_ErrMsgs(BSTR *pVal)
{
	_bstr_t bstrErrors = L"";
	try {

		bstrErrors = Utilities::GetErrorList(m_listErrors, false);
	}
		catch(_com_error &e) {
		bstrErrors = Utilities::ConstructErrMsg(L"COM ERROR: CFormat::get_ErrMsgList() catch statement.", e.Source(), e.Description(), e.ErrorMessage());
	}
	catch (...) {
		bstrErrors = L"DTRAN ERROR: CFormat::get_ErrMsgList() catch statement.";
	}

	*pVal = bstrErrors.copy();

	return S_OK;
}

