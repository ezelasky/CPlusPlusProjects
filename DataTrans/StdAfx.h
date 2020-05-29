// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__D47A00DA_AE23_4153_940E_954CECB01B5F__INCLUDED_)
#define AFX_STDAFX_H__D47A00DA_AE23_4153_940E_954CECB01B5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;

#include <atlcom.h>

#define  _CRTDBG_MAP_ALLOC 
#include <stdlib.h>
#include <crtdbg.h>

#include <COMDEF.h>
#include "stdio.h"
#include <iostream>
#include <fstream>
//#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <io.h>
#include <string.h>
#include <windows.h>


#define MAX_FILE_LEN 1024
#define MAX_ERR_LEN  1024
#define MAX_QUERY_LEN  2048
#define BUFF_LARGE  2048
#define BUFF_MED 1024
#define BUFF_LEN 512
#define BUFF_SMALL 100
#define ERR_LIMIT 50
#define BUFF_VERY_LARGE 64000
#define SQLBATCHSIZE 100000
#define MAXERRMSGS 100
#define MAXHEAPSIZE 1000000
#define NUMDBFFIELDS 100
#define DBFFIELDLEN 11
#define FLOAT_DISPLAYLEN 10
#define INT_DISPLAYLEN 5
#define BIGINT_DISPAYLEN 10
#define TINYINT_DISPLAYLEN 2
#define DATETIME_DISPALYLEN 24
#define BIT_DISPALYLEN 1
#define MONEY_DISPLAYLEN 8
#define FIELDPOSARRAYNUM 7
//#define FILE_DEBUG

typedef struct {
		char Name[DBFFIELDLEN];
		char Type;
		int	Length;
		int	Precision;
		int	Scale;
} DBFFIELD;


typedef struct {
		_bstr_t Name;		//for now this is all that is needed...
		//char Type;
		//int	Length;
		//int	Precision;
		//int	Scale;
} SQLFIELD;

#include <list>
typedef std::list<_bstr_t> SQLFIELDLIST;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D47A00DA_AE23_4153_940E_954CECB01B5F__INCLUDED)
