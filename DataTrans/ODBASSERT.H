#ifndef	OLEDB_ASSERT_CHARLIE
#define	OLEDB_ASSERT_CHARLIE
	#ifdef _DEBUG
		#ifdef __BORLANDC__		//Borland C++

			__inline int OLEDBPRO_ASSERTE_Helper(char* strExpr, char *strFile, int line, char* strErrMsg)
			{
				char strMsg[256*2]={0};
				::sprintf(strMsg, _T("ASSERT(%s) failed at the line %d of the file %s.\n%s"), strExpr, line, strFile, strErrMsg);
				::MessageBeep(MB_ICONEXCLAMATION);
				return ::MessageBox(NULL, strMsg, "OleDBPro Debug Version -- Programming Error", MB_RETRYCANCEL|MB_ICONEXCLAMATION);
			}
			#define OleDBPro_ASSERT(expr, strErrorInfo) do { \
				if(!(expr) && OLEDBPRO_ASSERTE_Helper(#expr, __FILE__, __LINE__, strErrorInfo) == IDCANCEL)  \
					::exit(1); \
			} while (0) 
		#else	//Visual C++		
			#define OleDBPro_ASSERT(expr, strErrorInfo) do{if(!(expr)&&(1==_CrtDbgReport(_CRT_ASSERT,__FILE__,__LINE__,"OleDBPro Debug Version","%s%s",#expr,strErrorInfo)))_CrtDbgBreak();}while(0)
		#endif
	#else
		#define OleDBPro_ASSERT(expr, strErrorInfo)// do {;} while (0)
	#endif
#endif

