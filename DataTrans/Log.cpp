//--------------------------------------------------------------------
//  class CLog
//--------------------------------------------------------------------
#include "stdafx.h"

//#include "afx.h" .... may have to change for console app
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <TCHAR.H>
#include "log.h"

//TEST svn

CLog::CLog()
{
	fLog = NULL;
	m_ptrLogName = NULL;
	bNoTimeStamp = false;
	m_iLineLength = 80;
	bWriteHeader = true;
	bWriteFooter = true;
}

CLog::~CLog()
{
	_ftprintf(fLog, "************************ File End ************************\n");
	fflush(fLog);
	Close();
}

int CLog::Open(TCHAR* pszName, TCHAR* pszDirName, TCHAR* pszLogFileLoc)
{
	time_t ltime;
	struct tm* today;  
	time (&ltime);   //get system time
	today = localtime (&ltime);

	if ( m_ptrLogName == NULL )
	{
		if (pszLogFileLoc == NULL)
		{
			TCHAR * pszTemp = new TCHAR [ _tcslen(pszName) + _tcslen(pszDirName) + 13 ];
			_stprintf(pszTemp, "%s\\%s.%02d%02d%02d.log",
			(const TCHAR*) pszDirName,
			(const TCHAR*) pszName,
			today->tm_hour,
			today->tm_min,
			today->tm_sec);

			m_ptrLogName = new TCHAR [ _tcslen(pszTemp) + 1 ] ;
			memset(m_ptrLogName, 0, _tcslen(pszTemp) + 1);
			memcpy(m_ptrLogName, pszTemp, _tcslen(pszTemp));

			if (pszTemp)
				delete pszTemp;
		}
		else
		{
			m_ptrLogName = new TCHAR [ _tcslen(pszLogFileLoc) + 1 ] ;
			memset(m_ptrLogName, 0, _tcslen(pszLogFileLoc) + 1);
			memcpy(m_ptrLogName, pszLogFileLoc, _tcslen(pszLogFileLoc));

		}
	}

	if ( (fLog = _tfopen(m_ptrLogName, _T("a") ) ) == NULL )
		return 0;

	if (bWriteHeader)
	{
		_ftprintf(fLog, "\r\n************************ Log file for the date  %02d:%02d:%04d ************************\n",
			(today->tm_mon)+1,
			today->tm_mday,
			(today->tm_year)+1900 );
		WriteBlankLine();

	}

	return 1;

}


void CLog::Close()
{

	if (bWriteFooter)
	{
		_ftprintf(fLog, "******************************************************************************************\n");
		WriteBlankLine();
	}

	if (m_ptrLogName)
	{
		delete m_ptrLogName;
		m_ptrLogName = NULL;
	}

	fLog = NULL;
}

void CLog::WriteString(TCHAR* pszText)
{
	time_t ltime;
	struct tm* today;  
	time (&ltime);   //get system time
	today = localtime (&ltime);

	int iLength =  _tcslen(pszText);
	int iOutLength = min(m_iLineLength, iLength);
	int iNumLines = 0;

	if ( fLog != NULL )
	{
		fseek(fLog, 0, SEEK_END);

		if ( !bNoTimeStamp )
		{
				_ftprintf(fLog, "%02d:%02d:%02d    ",
				today->tm_hour,
				today->tm_min,
				today->tm_sec  );
				if ( iLength > m_iLineLength - 12 )
					iOutLength -= 12;
		}

		while ( iLength > 0 )
		{
			fwrite(pszText, 1, iOutLength, fLog);
			_ftprintf(fLog, "\n");
			iLength -= iOutLength;
			pszText += iOutLength;
			iOutLength = min(m_iLineLength, iLength);
			iNumLines++;

		}

		if (iNumLines > 1 ) _ftprintf(fLog, "\n");
		fflush(fLog);
	}

}

void CLog::WriteBlankLine()
{
	if ( fLog != NULL )
	{
		fseek(fLog, 0, SEEK_END);
		_ftprintf(fLog, "\n");
	}
}