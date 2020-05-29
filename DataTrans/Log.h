#ifndef __LOG_H
#define __LOG_H

class CLog
{
private:
	FILE* fLog;
	TCHAR* m_ptrLogName;
	TCHAR LogBuffer[2000];
	BOOL bNoTimeStamp;
	BOOL bWriteHeader;
	BOOL bWriteFooter;
	int m_iLineLength;

public:
	CLog(void);
	~CLog(void);
	int Open(TCHAR* pszName, TCHAR* pszDirName, TCHAR* pszLogFileLoc);
	void Close();
	void WriteString(TCHAR* pszText);
	void WriteBlankLine();
	void SetTimeStamp(BOOL bValue) {bNoTimeStamp = bValue;}
	void SetLineLength(int iValue) {m_iLineLength = iValue;}
	void SetWriteHeader(BOOL bValue) {bWriteHeader = bValue;}
	_bstr_t GetLogName() {_bstr_t bstrLogName(m_ptrLogName); return bstrLogName; }

};

//extern CLog log;

#endif