// Format.h : Declaration of the CFormat

#ifndef __FORMAT_H_
#define __FORMAT_H_

#include "resource.h"       // main symbols

#if !defined
#include "Utilities.h"
#endif

#include <vector>
//#include <list>
#include <map>

typedef enum {nNAME=0, nTYPE, nSTART, nEND, nLENGTH, nPRECISION, nSCALE, nNONE } FIELDNAME;

/////////////////////////////////////////////////////////////////////////////
// CFormat
class ATL_NO_VTABLE CFormat : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFormat, &CLSID_Format>,
	public IDispatchImpl<IFormat, &IID_IFormat, &LIBID_DTRANLib>
{
public:
	CFormat()
	{
//		m_bstrErrMsgs = L"";
		m_bstrFileName = L"";
		m_bstrErrMsgList = L"";
		m_bstrFileIn = L"";
		m_bCalledInternally = false;
		m_bstrTextQualifier = L"";

		m_bstrRowDelim = L"\n";
		m_bstrFldDelim = L",";

		//rule - store directories w/out backslashes.....
		TCHAR buffer[512];
		memset(buffer, 0, 512);
		GetCurrentDirectory(512, buffer);
		_bstr_t bstrDir(buffer);
		m_bstrDirectory = bstrDir;

		m_iCurrIndex = -1;
		m_bstrBCPFormatLoc = L"";
		m_bCarRetInBCP = VARIANT_TRUE;
		m_bBCPDelim = VARIANT_FALSE;

		m_iFieldPos[nNAME] = -99;	
		m_iFieldPos[nTYPE] = -99;  
		m_iFieldPos[nSTART] = -99;	
		m_iFieldPos[nEND] = -99;  
		m_iFieldPos[nLENGTH] = -99;	
		m_iFieldPos[nPRECISION] = -99;   
		m_iFieldPos[nSCALE] = -99;   

	}

DECLARE_REGISTRY_RESOURCEID(IDR_FORMAT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CFormat)
	COM_INTERFACE_ENTRY(IFormat)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

// IFormat
public:
	STDMETHOD(LoadCust)(/*[in]*/ BSTR FormatFileName, /*[in]*/ BSTR FormatSpec, /*[out,retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(ReplaceColNames)(/*[out,retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(put_TextQualifier)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_TextQualifier)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(RecordLength)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_CarRetInBCP)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(ReCalcLengths)(/*[out, retval]*/ VARIANT_BOOL * pRetVal);
	STDMETHOD(put_BCPDelim)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_FldDelim)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_FldDelim)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_RowDelim)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_RowDelim)(/*[in]*/ BSTR newVal);
	STDMETHOD(GetNextError)(/*[out]*/ BSTR* pValOut, /*[out, retval]*/ VARIANT_BOOL* pVal);
	STDMETHOD(GetFirstError)(/*[out]*/ BSTR* pValOut, /*[out, retval]*/ VARIANT_BOOL* pVal);
	STDMETHOD(get_ErrMsgList)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(CopyFormat)(/*[in]*/ IFormat* pFormat, /*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(get_BCPFormatFileLoc)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_BCPFormatFileLoc)(/*[in]*/ BSTR newVal);
	STDMETHOD(Write)(/*[in]*/ BSTR FileName, /*[in]*/ FORMATOPTION option, /*[out,retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(GetByPos)(/*[in]*/ long Position, /*[out, retval]*/ IFieldInfo** pObj);
	STDMETHOD(GetByName)(/*[in]*/ BSTR Name, /*[out, retval]*/ IFieldInfo** pObj);
	STDMETHOD(Prev)(/*[out, retval]*/ IFieldInfo** pObj);
	STDMETHOD(Last)(/*[out, retval]*/ IFieldInfo** pObj);
	STDMETHOD(Next)(/*[out, retval]*/ IFieldInfo** pObj);
	STDMETHOD(First)(/*[out, retval]*/ IFieldInfo** pObj);
	STDMETHOD(DeleteByPos)(/*[in]*/ long Position, /*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(DeleteByName)(/*[in]*/ BSTR Name, /*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(DeleteCurr)(/*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(get_Directory)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Directory)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_FileName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_FileName)(/*[in]*/ BSTR newVal);
	STDMETHOD(Clear)();
	STDMETHOD(get_FieldCount)(/*[out, retval]*/ long *pVal);
	STDMETHOD(AddField)(/*[in]*/ BSTR Name, /*[in]*/ BSTR Length, /*[in]*/ FIELDTYPE Type, /*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(AddFieldExt)(/*[in]*/ BSTR Name, /*[in]*/ BSTR Length, /*[in]*/ FIELDTYPE Type, /*[in]*/ long lStartPos, /*[in]*/ long lEndPos, /*[in]*/ long lPrecision, /*[in]*/ long lScale, /*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(Load)(/*[in]*/ BSTR FormatFileName, /*[in]*/ FORMATOPTION option, /*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(get_ErrMsgs)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(ValidFieldCollection)(/*[out, retval]*/ VARIANT_BOOL* pRetVal);
	STDMETHOD(TotalFieldLength)(/*[out, retval]*/ long *pVal);

//	_bstr_t m_bstrErrMsgs;
	_bstr_t m_bstrErrMsgList;
	_bstr_t m_bstrDirectory;
	_bstr_t m_bstrFileName;
	_bstr_t m_bstrBCPFormatLoc;
	bool m_bCalledInternally;

	void FinalRelease()
	{
		for (int i = 0; i < m_vecFieldInfo.size(); i++) 
		{
			IFieldInfo* pFieldInfo = m_vecFieldInfo[i];
			if (pFieldInfo != NULL)
			{
				pFieldInfo->Release();
				pFieldInfo = NULL;
			}
		}
	}

private:
	int m_iCurrIndex;
	std::vector<IFieldInfo*> m_vecFieldInfo;

//	typedef std::list<_bstr_t> ERRLIST;
	ERRLIST m_listErrors;
	int m_iErrIndex;

	_bstr_t m_bstrFileIn;
	_bstr_t m_bstrRowDelim;
	_bstr_t m_bstrFldDelim;
	_bstr_t m_bstrTextQualifier;
	VARIANT_BOOL m_bCarRetInBCP;
	VARIANT_BOOL m_bBCPDelim;
	
	HRESULT DeleteItem(int iIndex);

	VARIANT_BOOL WriteBCPFormatFile(_bstr_t FileName);
	VARIANT_BOOL WriteTNSorDELIMFormatFile(_bstr_t FileName, FORMATOPTION option);
	VARIANT_BOOL WriteCustFormatFile(TCHAR* szFileName);
	VARIANT_BOOL LoadBCPFormatFile(_bstr_t FileName);
	VARIANT_BOOL LoadTNSFormatFile(_bstr_t FileName);
	VARIANT_BOOL LoadDELIMFormatFile(_bstr_t FileName);
	VARIANT_BOOL LoadFormatFromFile(LPTSTR pzFile, int iBeginLine);
	VARIANT_BOOL CalcStartandEndPositions(bool bStartExists=false);
	LPSTR MapFieldTypeToText(FIELDTYPE nType, LPTSTR lpOut);

	int m_iFieldPos[FIELDPOSARRAYNUM];  //Field position array -- depicts order of format fields in the format file
	void InitializeFieldPosArray();
	void SetFieldPosArray_TNS();
	void SetFieldPosArray_BCP();
	bool FieldPosArrayWasSet();
	void SetNamePos(int i) {m_iFieldPos[nNAME] = i;}
	void SetTypePos(int i) {m_iFieldPos[nTYPE] = i;}
	void SetStartPos(int i) {m_iFieldPos[nSTART] = i;}
	void SetEndPos(int i) {m_iFieldPos[nEND] = i;}
	void SetLengthPos(int i) {m_iFieldPos[nLENGTH] = i;}
	void SetPrecisionPos(int i) {m_iFieldPos[nPRECISION] = i;}
	void SetScalePos(int i) {m_iFieldPos[nSCALE] = i;}
	bool NamePosSet()	{return ((m_iFieldPos[nNAME] != -99) ? true : false); }
	bool TypePosSet()	{return ((m_iFieldPos[nTYPE] != -99) ? true : false); }
	bool StartPosSet()  {return ((m_iFieldPos[nSTART] != -99) ? true : false); }
	bool EndPosSet()	{return ((m_iFieldPos[nEND] != -99) ? true : false); }
	bool LengthPosSet() {return ((m_iFieldPos[nLENGTH] != -99) ? true : false); }
	bool PrecisionPosSet() {return ((m_iFieldPos[nPRECISION] != -99) ? true : false); }
	bool ScalePosSet() {return ((m_iFieldPos[nSCALE] != -99) ? true : false); }


	FIELDNAME GetFieldNameFromOrder(int iOrderPos);
	int GetOutputFieldCount();
	
};

#endif //__FORMAT_H_
