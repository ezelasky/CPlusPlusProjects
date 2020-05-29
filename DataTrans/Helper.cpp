// Helper.cpp: implementation of the Helper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Helper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Helper::Helper()
{

}

Helper::~Helper()
{

}

/*
int Helper::ConstructFileName(LPTSTR pzBase, LPTSTR pzExt, LPTSTR pzFileName)
{
	
	TCHAR szBufDir[512];
	GetCurrentDirectory(512, szBufDir);
	
	srand((unsigned)time( NULL ));
	int i = rand();
	TCHAR unique[20];
	itoa(i, unique, 10);
	unique[strlen(unique)+1] = '\0';

	TCHAR szTemp[1000]; // = new TCHAR[_tcslen(szBufDir) + _tcslen(pzBase) + _tcslen(unique) + _tcslen(pzExt) + 1];
	memset(szTemp, 0, 1000);

	sprintf(szTemp, "%s\\%s%s.%s", szBufDir, pzBase, unique, pzExt);
	szTemp[_tcslen(szTemp)+1] = '\0';
	_tcscpy(pzFileName, szTemp);

	return _tcslen(pzFileName);

}
*/