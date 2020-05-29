

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Fri Mar 17 14:16:40 2006
 */
/* Compiler settings for .\DTran.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __DTran_h__
#define __DTran_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IFieldInfo_FWD_DEFINED__
#define __IFieldInfo_FWD_DEFINED__
typedef interface IFieldInfo IFieldInfo;
#endif 	/* __IFieldInfo_FWD_DEFINED__ */


#ifndef __IFormat_FWD_DEFINED__
#define __IFormat_FWD_DEFINED__
typedef interface IFormat IFormat;
#endif 	/* __IFormat_FWD_DEFINED__ */


#ifndef __IdeSQL_FWD_DEFINED__
#define __IdeSQL_FWD_DEFINED__
typedef interface IdeSQL IdeSQL;
#endif 	/* __IdeSQL_FWD_DEFINED__ */


#ifndef __IdeFixed_FWD_DEFINED__
#define __IdeFixed_FWD_DEFINED__
typedef interface IdeFixed IdeFixed;
#endif 	/* __IdeFixed_FWD_DEFINED__ */


#ifndef __IdeDelim_FWD_DEFINED__
#define __IdeDelim_FWD_DEFINED__
typedef interface IdeDelim IdeDelim;
#endif 	/* __IdeDelim_FWD_DEFINED__ */


#ifndef __IdeXBase_FWD_DEFINED__
#define __IdeXBase_FWD_DEFINED__
typedef interface IdeXBase IdeXBase;
#endif 	/* __IdeXBase_FWD_DEFINED__ */


#ifndef __IProcess_FWD_DEFINED__
#define __IProcess_FWD_DEFINED__
typedef interface IProcess IProcess;
#endif 	/* __IProcess_FWD_DEFINED__ */


#ifndef __FieldInfo_FWD_DEFINED__
#define __FieldInfo_FWD_DEFINED__

#ifdef __cplusplus
typedef class FieldInfo FieldInfo;
#else
typedef struct FieldInfo FieldInfo;
#endif /* __cplusplus */

#endif 	/* __FieldInfo_FWD_DEFINED__ */


#ifndef __Format_FWD_DEFINED__
#define __Format_FWD_DEFINED__

#ifdef __cplusplus
typedef class Format Format;
#else
typedef struct Format Format;
#endif /* __cplusplus */

#endif 	/* __Format_FWD_DEFINED__ */


#ifndef __deSQL_FWD_DEFINED__
#define __deSQL_FWD_DEFINED__

#ifdef __cplusplus
typedef class deSQL deSQL;
#else
typedef struct deSQL deSQL;
#endif /* __cplusplus */

#endif 	/* __deSQL_FWD_DEFINED__ */


#ifndef __deFixed_FWD_DEFINED__
#define __deFixed_FWD_DEFINED__

#ifdef __cplusplus
typedef class deFixed deFixed;
#else
typedef struct deFixed deFixed;
#endif /* __cplusplus */

#endif 	/* __deFixed_FWD_DEFINED__ */


#ifndef __Process_FWD_DEFINED__
#define __Process_FWD_DEFINED__

#ifdef __cplusplus
typedef class Process Process;
#else
typedef struct Process Process;
#endif /* __cplusplus */

#endif 	/* __Process_FWD_DEFINED__ */


#ifndef __deDelim_FWD_DEFINED__
#define __deDelim_FWD_DEFINED__

#ifdef __cplusplus
typedef class deDelim deDelim;
#else
typedef struct deDelim deDelim;
#endif /* __cplusplus */

#endif 	/* __deDelim_FWD_DEFINED__ */


#ifndef __deXBase_FWD_DEFINED__
#define __deXBase_FWD_DEFINED__

#ifdef __cplusplus
typedef class deXBase deXBase;
#else
typedef struct deXBase deXBase;
#endif /* __cplusplus */

#endif 	/* __deXBase_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_DTran_0000 */
/* [local] */ 

typedef /* [public][public][public][public][public][public][public][public][public][public][public][public][public][uuid] */  DECLSPEC_UUID("F7465B3E-D219-4f52-BA78-F93D625027EB") 
enum __MIDL___MIDL_itf_DTran_0000_0001
    {	vCREATE	= 0,
	vOVERWRITE	= vCREATE + 1,
	vAPPEND	= vOVERWRITE + 1,
	vDROPADD	= vAPPEND + 1
    } 	DESTACTION;

typedef /* [public][public][uuid] */  DECLSPEC_UUID("AE5F66C0-9BC6-46f2-B0F8-E719B8849E2C") 
enum __MIDL___MIDL_itf_DTran_0000_0002
    {	vBYREC	= 0,
	vBYFILE	= vBYREC + 1
    } 	SPLITRULE;

typedef /* [public][public][public][uuid] */  DECLSPEC_UUID("58105FCB-2420-421f-9275-90F73882E227") 
enum __MIDL___MIDL_itf_DTran_0000_0003
    {	vTNSFORMAT	= 0,
	vBCPFORMAT	= vTNSFORMAT + 1,
	vDELIMFORMAT	= vBCPFORMAT + 1,
	vCUSTFORMAT	= vDELIMFORMAT + 1
    } 	FORMATOPTION;

typedef /* [public][public][uuid] */  DECLSPEC_UUID("36E99AAB-F130-4e51-A818-26B1E820B0A5") 
enum __MIDL___MIDL_itf_DTran_0000_0004
    {	vCORRECT	= 0,
	vOPTIMIZE	= vCORRECT + 1
    } 	RESIZEOPTION;

typedef /* [public][public][public][uuid] */  DECLSPEC_UUID("894F0DB3-9968-4b81-B2C8-3054C1149AD6") 
enum __MIDL___MIDL_itf_DTran_0000_0005
    {	vDBASE	= 0,
	vFOXPRO	= vDBASE + 1
    } 	XBASETYPE;

typedef /* [public][public][public][public][public][uuid] */  DECLSPEC_UUID("F858AF45-B340-4478-AD1A-62441B15A740") 
enum __MIDL___MIDL_itf_DTran_0000_0006
    {	vBIGINT	= 0,
	vBINARY	= vBIGINT + 1,
	vBIT	= vBINARY + 1,
	vCHAR	= vBIT + 1,
	vDATETIME	= vCHAR + 1,
	vDECIMAL	= vDATETIME + 1,
	vFLOAT	= vDECIMAL + 1,
	vINT	= vFLOAT + 1,
	vMONEY	= vINT + 1,
	vNUMERIC	= vMONEY + 1,
	vREAL	= vNUMERIC + 1,
	vSMALLDATETIME	= vREAL + 1,
	vSMALLINT	= vSMALLDATETIME + 1,
	vSMALLMONEY	= vSMALLINT + 1,
	vTEXT	= vSMALLMONEY + 1,
	vTIMESTAMP	= vTEXT + 1,
	vTINYINT	= vTIMESTAMP + 1,
	vVARCHAR	= vTINYINT + 1
    } 	FIELDTYPE;



extern RPC_IF_HANDLE __MIDL_itf_DTran_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_DTran_0000_v0_0_s_ifspec;

#ifndef __IFieldInfo_INTERFACE_DEFINED__
#define __IFieldInfo_INTERFACE_DEFINED__

/* interface IFieldInfo */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IFieldInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("85697DBA-6336-4C66-A80A-01AE41F0D757")
    IFieldInfo : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ FIELDTYPE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Type( 
            /* [in] */ FIELDTYPE newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Length( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Length( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Start( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Start( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_End( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_End( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Precision( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Precision( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Scale( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Scale( 
            /* [in] */ long newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFieldInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFieldInfo * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFieldInfo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFieldInfo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IFieldInfo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IFieldInfo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IFieldInfo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IFieldInfo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IFieldInfo * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Name )( 
            IFieldInfo * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IFieldInfo * This,
            /* [retval][out] */ FIELDTYPE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Type )( 
            IFieldInfo * This,
            /* [in] */ FIELDTYPE newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Length )( 
            IFieldInfo * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Length )( 
            IFieldInfo * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Start )( 
            IFieldInfo * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Start )( 
            IFieldInfo * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_End )( 
            IFieldInfo * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_End )( 
            IFieldInfo * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Precision )( 
            IFieldInfo * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Precision )( 
            IFieldInfo * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Scale )( 
            IFieldInfo * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Scale )( 
            IFieldInfo * This,
            /* [in] */ long newVal);
        
        END_INTERFACE
    } IFieldInfoVtbl;

    interface IFieldInfo
    {
        CONST_VTBL struct IFieldInfoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFieldInfo_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFieldInfo_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFieldInfo_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFieldInfo_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IFieldInfo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IFieldInfo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IFieldInfo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IFieldInfo_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#define IFieldInfo_put_Name(This,newVal)	\
    (This)->lpVtbl -> put_Name(This,newVal)

#define IFieldInfo_get_Type(This,pVal)	\
    (This)->lpVtbl -> get_Type(This,pVal)

#define IFieldInfo_put_Type(This,newVal)	\
    (This)->lpVtbl -> put_Type(This,newVal)

#define IFieldInfo_get_Length(This,pVal)	\
    (This)->lpVtbl -> get_Length(This,pVal)

#define IFieldInfo_put_Length(This,newVal)	\
    (This)->lpVtbl -> put_Length(This,newVal)

#define IFieldInfo_get_Start(This,pVal)	\
    (This)->lpVtbl -> get_Start(This,pVal)

#define IFieldInfo_put_Start(This,newVal)	\
    (This)->lpVtbl -> put_Start(This,newVal)

#define IFieldInfo_get_End(This,pVal)	\
    (This)->lpVtbl -> get_End(This,pVal)

#define IFieldInfo_put_End(This,newVal)	\
    (This)->lpVtbl -> put_End(This,newVal)

#define IFieldInfo_get_Precision(This,pVal)	\
    (This)->lpVtbl -> get_Precision(This,pVal)

#define IFieldInfo_put_Precision(This,newVal)	\
    (This)->lpVtbl -> put_Precision(This,newVal)

#define IFieldInfo_get_Scale(This,pVal)	\
    (This)->lpVtbl -> get_Scale(This,pVal)

#define IFieldInfo_put_Scale(This,newVal)	\
    (This)->lpVtbl -> put_Scale(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IFieldInfo_get_Name_Proxy( 
    IFieldInfo * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IFieldInfo_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IFieldInfo_put_Name_Proxy( 
    IFieldInfo * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IFieldInfo_put_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IFieldInfo_get_Type_Proxy( 
    IFieldInfo * This,
    /* [retval][out] */ FIELDTYPE *pVal);


void __RPC_STUB IFieldInfo_get_Type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IFieldInfo_put_Type_Proxy( 
    IFieldInfo * This,
    /* [in] */ FIELDTYPE newVal);


void __RPC_STUB IFieldInfo_put_Type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IFieldInfo_get_Length_Proxy( 
    IFieldInfo * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IFieldInfo_get_Length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IFieldInfo_put_Length_Proxy( 
    IFieldInfo * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IFieldInfo_put_Length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IFieldInfo_get_Start_Proxy( 
    IFieldInfo * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IFieldInfo_get_Start_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IFieldInfo_put_Start_Proxy( 
    IFieldInfo * This,
    /* [in] */ long newVal);


void __RPC_STUB IFieldInfo_put_Start_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IFieldInfo_get_End_Proxy( 
    IFieldInfo * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IFieldInfo_get_End_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IFieldInfo_put_End_Proxy( 
    IFieldInfo * This,
    /* [in] */ long newVal);


void __RPC_STUB IFieldInfo_put_End_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IFieldInfo_get_Precision_Proxy( 
    IFieldInfo * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IFieldInfo_get_Precision_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IFieldInfo_put_Precision_Proxy( 
    IFieldInfo * This,
    /* [in] */ long newVal);


void __RPC_STUB IFieldInfo_put_Precision_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IFieldInfo_get_Scale_Proxy( 
    IFieldInfo * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IFieldInfo_get_Scale_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IFieldInfo_put_Scale_Proxy( 
    IFieldInfo * This,
    /* [in] */ long newVal);


void __RPC_STUB IFieldInfo_put_Scale_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFieldInfo_INTERFACE_DEFINED__ */


#ifndef __IFormat_INTERFACE_DEFINED__
#define __IFormat_INTERFACE_DEFINED__

/* interface IFormat */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IFormat;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0A53F432-9E65-40BE-8C7A-DF0792D33FE1")
    IFormat : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrMsgs( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Load( 
            /* [in] */ BSTR FormatFileName,
            /* [in] */ FORMATOPTION option,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddField( 
            /* [in] */ BSTR Name,
            /* [in] */ BSTR Length,
            /* [in] */ FIELDTYPE Type,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FieldCount( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clear( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FileName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_FileName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Directory( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Directory( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteCurr( 
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteByName( 
            /* [in] */ BSTR Name,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeleteByPos( 
            /* [in] */ long Position,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE First( 
            /* [retval][out] */ IFieldInfo **pObj) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Next( 
            /* [retval][out] */ IFieldInfo **pObj) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Last( 
            /* [retval][out] */ IFieldInfo **pObj) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Prev( 
            /* [retval][out] */ IFieldInfo **pObj) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetByName( 
            /* [in] */ BSTR Name,
            /* [retval][out] */ IFieldInfo **pObj) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetByPos( 
            /* [in] */ long Position,
            /* [retval][out] */ IFieldInfo **pObj) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Write( 
            /* [in] */ BSTR FileName,
            /* [in] */ FORMATOPTION option,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_BCPFormatFileLoc( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_BCPFormatFileLoc( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CopyFormat( 
            /* [in] */ IFormat *pFormat,
            /* [retval][out] */ VARIANT_BOOL *bSuccess) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddFieldExt( 
            /* [in] */ BSTR Name,
            /* [in] */ BSTR Length,
            /* [in] */ FIELDTYPE Type,
            /* [in] */ long lStartPos,
            /* [in] */ long lEndPos,
            /* [in] */ long lPrecision,
            /* [in] */ long lScale,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrMsgList( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ValidFieldCollection( 
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFirstError( 
            /* [out] */ BSTR *pValOut,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetNextError( 
            /* [out] */ BSTR *pValOut,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RowDelim( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RowDelim( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FldDelim( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_FldDelim( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReCalcLengths( 
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CarRetInBCP( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RecordLength( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_BCPDelim( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TextQualifier( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TextQualifier( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReplaceColNames( 
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadCust( 
            /* [in] */ BSTR FormatFileName,
            /* [in] */ BSTR FormatSpec,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TotalFieldLength( 
            /* [retval][out] */ long *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFormatVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFormat * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFormat * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFormat * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IFormat * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IFormat * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IFormat * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IFormat * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ErrMsgs )( 
            IFormat * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Load )( 
            IFormat * This,
            /* [in] */ BSTR FormatFileName,
            /* [in] */ FORMATOPTION option,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddField )( 
            IFormat * This,
            /* [in] */ BSTR Name,
            /* [in] */ BSTR Length,
            /* [in] */ FIELDTYPE Type,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FieldCount )( 
            IFormat * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clear )( 
            IFormat * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FileName )( 
            IFormat * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FileName )( 
            IFormat * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Directory )( 
            IFormat * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Directory )( 
            IFormat * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteCurr )( 
            IFormat * This,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteByName )( 
            IFormat * This,
            /* [in] */ BSTR Name,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeleteByPos )( 
            IFormat * This,
            /* [in] */ long Position,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *First )( 
            IFormat * This,
            /* [retval][out] */ IFieldInfo **pObj);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Next )( 
            IFormat * This,
            /* [retval][out] */ IFieldInfo **pObj);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Last )( 
            IFormat * This,
            /* [retval][out] */ IFieldInfo **pObj);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Prev )( 
            IFormat * This,
            /* [retval][out] */ IFieldInfo **pObj);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetByName )( 
            IFormat * This,
            /* [in] */ BSTR Name,
            /* [retval][out] */ IFieldInfo **pObj);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetByPos )( 
            IFormat * This,
            /* [in] */ long Position,
            /* [retval][out] */ IFieldInfo **pObj);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Write )( 
            IFormat * This,
            /* [in] */ BSTR FileName,
            /* [in] */ FORMATOPTION option,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BCPFormatFileLoc )( 
            IFormat * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BCPFormatFileLoc )( 
            IFormat * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CopyFormat )( 
            IFormat * This,
            /* [in] */ IFormat *pFormat,
            /* [retval][out] */ VARIANT_BOOL *bSuccess);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddFieldExt )( 
            IFormat * This,
            /* [in] */ BSTR Name,
            /* [in] */ BSTR Length,
            /* [in] */ FIELDTYPE Type,
            /* [in] */ long lStartPos,
            /* [in] */ long lEndPos,
            /* [in] */ long lPrecision,
            /* [in] */ long lScale,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ErrMsgList )( 
            IFormat * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidFieldCollection )( 
            IFormat * This,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFirstError )( 
            IFormat * This,
            /* [out] */ BSTR *pValOut,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetNextError )( 
            IFormat * This,
            /* [out] */ BSTR *pValOut,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RowDelim )( 
            IFormat * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RowDelim )( 
            IFormat * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FldDelim )( 
            IFormat * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FldDelim )( 
            IFormat * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReCalcLengths )( 
            IFormat * This,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CarRetInBCP )( 
            IFormat * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RecordLength )( 
            IFormat * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_BCPDelim )( 
            IFormat * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TextQualifier )( 
            IFormat * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TextQualifier )( 
            IFormat * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReplaceColNames )( 
            IFormat * This,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadCust )( 
            IFormat * This,
            /* [in] */ BSTR FormatFileName,
            /* [in] */ BSTR FormatSpec,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TotalFieldLength )( 
            IFormat * This,
            /* [retval][out] */ long *pVal);
        
        END_INTERFACE
    } IFormatVtbl;

    interface IFormat
    {
        CONST_VTBL struct IFormatVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFormat_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFormat_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFormat_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFormat_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IFormat_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IFormat_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IFormat_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IFormat_get_ErrMsgs(This,pVal)	\
    (This)->lpVtbl -> get_ErrMsgs(This,pVal)

#define IFormat_Load(This,FormatFileName,option,pRetVal)	\
    (This)->lpVtbl -> Load(This,FormatFileName,option,pRetVal)

#define IFormat_AddField(This,Name,Length,Type,pRetVal)	\
    (This)->lpVtbl -> AddField(This,Name,Length,Type,pRetVal)

#define IFormat_get_FieldCount(This,pVal)	\
    (This)->lpVtbl -> get_FieldCount(This,pVal)

#define IFormat_Clear(This)	\
    (This)->lpVtbl -> Clear(This)

#define IFormat_get_FileName(This,pVal)	\
    (This)->lpVtbl -> get_FileName(This,pVal)

#define IFormat_put_FileName(This,newVal)	\
    (This)->lpVtbl -> put_FileName(This,newVal)

#define IFormat_get_Directory(This,pVal)	\
    (This)->lpVtbl -> get_Directory(This,pVal)

#define IFormat_put_Directory(This,newVal)	\
    (This)->lpVtbl -> put_Directory(This,newVal)

#define IFormat_DeleteCurr(This,pRetVal)	\
    (This)->lpVtbl -> DeleteCurr(This,pRetVal)

#define IFormat_DeleteByName(This,Name,pRetVal)	\
    (This)->lpVtbl -> DeleteByName(This,Name,pRetVal)

#define IFormat_DeleteByPos(This,Position,pRetVal)	\
    (This)->lpVtbl -> DeleteByPos(This,Position,pRetVal)

#define IFormat_First(This,pObj)	\
    (This)->lpVtbl -> First(This,pObj)

#define IFormat_Next(This,pObj)	\
    (This)->lpVtbl -> Next(This,pObj)

#define IFormat_Last(This,pObj)	\
    (This)->lpVtbl -> Last(This,pObj)

#define IFormat_Prev(This,pObj)	\
    (This)->lpVtbl -> Prev(This,pObj)

#define IFormat_GetByName(This,Name,pObj)	\
    (This)->lpVtbl -> GetByName(This,Name,pObj)

#define IFormat_GetByPos(This,Position,pObj)	\
    (This)->lpVtbl -> GetByPos(This,Position,pObj)

#define IFormat_Write(This,FileName,option,pRetVal)	\
    (This)->lpVtbl -> Write(This,FileName,option,pRetVal)

#define IFormat_get_BCPFormatFileLoc(This,pVal)	\
    (This)->lpVtbl -> get_BCPFormatFileLoc(This,pVal)

#define IFormat_put_BCPFormatFileLoc(This,newVal)	\
    (This)->lpVtbl -> put_BCPFormatFileLoc(This,newVal)

#define IFormat_CopyFormat(This,pFormat,bSuccess)	\
    (This)->lpVtbl -> CopyFormat(This,pFormat,bSuccess)

#define IFormat_AddFieldExt(This,Name,Length,Type,lStartPos,lEndPos,lPrecision,lScale,pRetVal)	\
    (This)->lpVtbl -> AddFieldExt(This,Name,Length,Type,lStartPos,lEndPos,lPrecision,lScale,pRetVal)

#define IFormat_get_ErrMsgList(This,pVal)	\
    (This)->lpVtbl -> get_ErrMsgList(This,pVal)

#define IFormat_ValidFieldCollection(This,pRetVal)	\
    (This)->lpVtbl -> ValidFieldCollection(This,pRetVal)

#define IFormat_GetFirstError(This,pValOut,pVal)	\
    (This)->lpVtbl -> GetFirstError(This,pValOut,pVal)

#define IFormat_GetNextError(This,pValOut,pVal)	\
    (This)->lpVtbl -> GetNextError(This,pValOut,pVal)

#define IFormat_get_RowDelim(This,pVal)	\
    (This)->lpVtbl -> get_RowDelim(This,pVal)

#define IFormat_put_RowDelim(This,newVal)	\
    (This)->lpVtbl -> put_RowDelim(This,newVal)

#define IFormat_get_FldDelim(This,pVal)	\
    (This)->lpVtbl -> get_FldDelim(This,pVal)

#define IFormat_put_FldDelim(This,newVal)	\
    (This)->lpVtbl -> put_FldDelim(This,newVal)

#define IFormat_ReCalcLengths(This,pRetVal)	\
    (This)->lpVtbl -> ReCalcLengths(This,pRetVal)

#define IFormat_put_CarRetInBCP(This,newVal)	\
    (This)->lpVtbl -> put_CarRetInBCP(This,newVal)

#define IFormat_RecordLength(This,pVal)	\
    (This)->lpVtbl -> RecordLength(This,pVal)

#define IFormat_put_BCPDelim(This,newVal)	\
    (This)->lpVtbl -> put_BCPDelim(This,newVal)

#define IFormat_get_TextQualifier(This,pVal)	\
    (This)->lpVtbl -> get_TextQualifier(This,pVal)

#define IFormat_put_TextQualifier(This,newVal)	\
    (This)->lpVtbl -> put_TextQualifier(This,newVal)

#define IFormat_ReplaceColNames(This,pRetVal)	\
    (This)->lpVtbl -> ReplaceColNames(This,pRetVal)

#define IFormat_LoadCust(This,FormatFileName,FormatSpec,pRetVal)	\
    (This)->lpVtbl -> LoadCust(This,FormatFileName,FormatSpec,pRetVal)

#define IFormat_TotalFieldLength(This,pVal)	\
    (This)->lpVtbl -> TotalFieldLength(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IFormat_get_ErrMsgs_Proxy( 
    IFormat * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IFormat_get_ErrMsgs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_Load_Proxy( 
    IFormat * This,
    /* [in] */ BSTR FormatFileName,
    /* [in] */ FORMATOPTION option,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IFormat_Load_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_AddField_Proxy( 
    IFormat * This,
    /* [in] */ BSTR Name,
    /* [in] */ BSTR Length,
    /* [in] */ FIELDTYPE Type,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IFormat_AddField_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IFormat_get_FieldCount_Proxy( 
    IFormat * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IFormat_get_FieldCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_Clear_Proxy( 
    IFormat * This);


void __RPC_STUB IFormat_Clear_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IFormat_get_FileName_Proxy( 
    IFormat * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IFormat_get_FileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IFormat_put_FileName_Proxy( 
    IFormat * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IFormat_put_FileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IFormat_get_Directory_Proxy( 
    IFormat * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IFormat_get_Directory_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IFormat_put_Directory_Proxy( 
    IFormat * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IFormat_put_Directory_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_DeleteCurr_Proxy( 
    IFormat * This,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IFormat_DeleteCurr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_DeleteByName_Proxy( 
    IFormat * This,
    /* [in] */ BSTR Name,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IFormat_DeleteByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_DeleteByPos_Proxy( 
    IFormat * This,
    /* [in] */ long Position,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IFormat_DeleteByPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_First_Proxy( 
    IFormat * This,
    /* [retval][out] */ IFieldInfo **pObj);


void __RPC_STUB IFormat_First_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_Next_Proxy( 
    IFormat * This,
    /* [retval][out] */ IFieldInfo **pObj);


void __RPC_STUB IFormat_Next_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_Last_Proxy( 
    IFormat * This,
    /* [retval][out] */ IFieldInfo **pObj);


void __RPC_STUB IFormat_Last_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_Prev_Proxy( 
    IFormat * This,
    /* [retval][out] */ IFieldInfo **pObj);


void __RPC_STUB IFormat_Prev_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_GetByName_Proxy( 
    IFormat * This,
    /* [in] */ BSTR Name,
    /* [retval][out] */ IFieldInfo **pObj);


void __RPC_STUB IFormat_GetByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_GetByPos_Proxy( 
    IFormat * This,
    /* [in] */ long Position,
    /* [retval][out] */ IFieldInfo **pObj);


void __RPC_STUB IFormat_GetByPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_Write_Proxy( 
    IFormat * This,
    /* [in] */ BSTR FileName,
    /* [in] */ FORMATOPTION option,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IFormat_Write_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IFormat_get_BCPFormatFileLoc_Proxy( 
    IFormat * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IFormat_get_BCPFormatFileLoc_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IFormat_put_BCPFormatFileLoc_Proxy( 
    IFormat * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IFormat_put_BCPFormatFileLoc_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_CopyFormat_Proxy( 
    IFormat * This,
    /* [in] */ IFormat *pFormat,
    /* [retval][out] */ VARIANT_BOOL *bSuccess);


void __RPC_STUB IFormat_CopyFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_AddFieldExt_Proxy( 
    IFormat * This,
    /* [in] */ BSTR Name,
    /* [in] */ BSTR Length,
    /* [in] */ FIELDTYPE Type,
    /* [in] */ long lStartPos,
    /* [in] */ long lEndPos,
    /* [in] */ long lPrecision,
    /* [in] */ long lScale,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IFormat_AddFieldExt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IFormat_get_ErrMsgList_Proxy( 
    IFormat * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IFormat_get_ErrMsgList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_ValidFieldCollection_Proxy( 
    IFormat * This,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IFormat_ValidFieldCollection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_GetFirstError_Proxy( 
    IFormat * This,
    /* [out] */ BSTR *pValOut,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IFormat_GetFirstError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_GetNextError_Proxy( 
    IFormat * This,
    /* [out] */ BSTR *pValOut,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IFormat_GetNextError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IFormat_get_RowDelim_Proxy( 
    IFormat * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IFormat_get_RowDelim_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IFormat_put_RowDelim_Proxy( 
    IFormat * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IFormat_put_RowDelim_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IFormat_get_FldDelim_Proxy( 
    IFormat * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IFormat_get_FldDelim_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IFormat_put_FldDelim_Proxy( 
    IFormat * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IFormat_put_FldDelim_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_ReCalcLengths_Proxy( 
    IFormat * This,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IFormat_ReCalcLengths_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IFormat_put_CarRetInBCP_Proxy( 
    IFormat * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IFormat_put_CarRetInBCP_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_RecordLength_Proxy( 
    IFormat * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IFormat_RecordLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IFormat_put_BCPDelim_Proxy( 
    IFormat * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IFormat_put_BCPDelim_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IFormat_get_TextQualifier_Proxy( 
    IFormat * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IFormat_get_TextQualifier_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IFormat_put_TextQualifier_Proxy( 
    IFormat * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IFormat_put_TextQualifier_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_ReplaceColNames_Proxy( 
    IFormat * This,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IFormat_ReplaceColNames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_LoadCust_Proxy( 
    IFormat * This,
    /* [in] */ BSTR FormatFileName,
    /* [in] */ BSTR FormatSpec,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IFormat_LoadCust_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFormat_TotalFieldLength_Proxy( 
    IFormat * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IFormat_TotalFieldLength_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFormat_INTERFACE_DEFINED__ */


#ifndef __IdeSQL_INTERFACE_DEFINED__
#define __IdeSQL_INTERFACE_DEFINED__

/* interface IdeSQL */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IdeSQL;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4F35BB42-C24C-4F94-8B73-25F5DA9D0CE8")
    IdeSQL : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Server( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Server( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Database( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Database( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Table( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Table( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_UserID( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_UserID( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Password( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Password( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RowsLoaded( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrMsgs( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Format( 
            /* [in] */ IFormat *pObj) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Format( 
            /* [retval][out] */ IFormat **pObj) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Import( 
            /* [in] */ IDispatch *pIn,
            /* [in] */ DESTACTION action,
            /* [retval][out] */ VARIANT_BOOL *bRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCreateTableSQL( 
            /* [retval][out] */ BSTR *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Export( 
            /* [in] */ IDispatch *pIn,
            /* [in] */ DESTACTION action,
            /* [retval][out] */ VARIANT_BOOL *bRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFormat( 
            /* [in] */ IFormat *pObj,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TraceLogOn( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TraceLogOn( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ImportMulti( 
            /* [in] */ IDispatch *pIn,
            /* [in] */ DESTACTION action,
            /* [in] */ VARIANT_BOOL AddSource,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExportMulti( 
            /* [in] */ IDispatch *pIn,
            /* [in] */ DESTACTION action,
            /* [in] */ SPLITRULE rule,
            /* [in] */ long lValue,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Exists( 
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Version( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadFormat( 
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FieldResize( 
            /* [in] */ RESIZEOPTION option,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ExportQuery( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ExportQuery( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Owner( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Owner( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrMsgList( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ErrMsgList( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Query( 
            /* [in] */ BSTR bstrQuery,
            /* [retval][out] */ IDispatch **pRecSet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddDatabase( 
            /* [in] */ BSTR DataBase,
            /* [in] */ BSTR Path,
            /* [in] */ BSTR DatSize,
            /* [in] */ BSTR LogSize,
            /* [in] */ BSTR DatGrowth,
            /* [in] */ BSTR LogGrowth,
            /* [in] */ BSTR DatMaxSize,
            /* [in] */ BSTR LogMaxSize,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DatabaseExists( 
            /* [in] */ BSTR Server,
            /* [in] */ BSTR DataBase,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Execute( 
            /* [in] */ BSTR Query,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ImportBatchSize( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ImportBatchSize( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Err_MaxMessages( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Err_MaxMessages( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Err_NoInfo( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Err_NoInfo( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ID_Name( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ID_Name( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ID_Definition( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ID_Definition( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateTable( 
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IdeSQLVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IdeSQL * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IdeSQL * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IdeSQL * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IdeSQL * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IdeSQL * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IdeSQL * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IdeSQL * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Server )( 
            IdeSQL * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Server )( 
            IdeSQL * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Database )( 
            IdeSQL * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Database )( 
            IdeSQL * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Table )( 
            IdeSQL * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Table )( 
            IdeSQL * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_UserID )( 
            IdeSQL * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_UserID )( 
            IdeSQL * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Password )( 
            IdeSQL * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Password )( 
            IdeSQL * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RowsLoaded )( 
            IdeSQL * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ErrMsgs )( 
            IdeSQL * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Format )( 
            IdeSQL * This,
            /* [in] */ IFormat *pObj);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Format )( 
            IdeSQL * This,
            /* [retval][out] */ IFormat **pObj);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Import )( 
            IdeSQL * This,
            /* [in] */ IDispatch *pIn,
            /* [in] */ DESTACTION action,
            /* [retval][out] */ VARIANT_BOOL *bRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCreateTableSQL )( 
            IdeSQL * This,
            /* [retval][out] */ BSTR *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Export )( 
            IdeSQL * This,
            /* [in] */ IDispatch *pIn,
            /* [in] */ DESTACTION action,
            /* [retval][out] */ VARIANT_BOOL *bRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFormat )( 
            IdeSQL * This,
            /* [in] */ IFormat *pObj,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TraceLogOn )( 
            IdeSQL * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TraceLogOn )( 
            IdeSQL * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ImportMulti )( 
            IdeSQL * This,
            /* [in] */ IDispatch *pIn,
            /* [in] */ DESTACTION action,
            /* [in] */ VARIANT_BOOL AddSource,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ExportMulti )( 
            IdeSQL * This,
            /* [in] */ IDispatch *pIn,
            /* [in] */ DESTACTION action,
            /* [in] */ SPLITRULE rule,
            /* [in] */ long lValue,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Exists )( 
            IdeSQL * This,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Version )( 
            IdeSQL * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadFormat )( 
            IdeSQL * This,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FieldResize )( 
            IdeSQL * This,
            /* [in] */ RESIZEOPTION option,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ExportQuery )( 
            IdeSQL * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ExportQuery )( 
            IdeSQL * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Owner )( 
            IdeSQL * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Owner )( 
            IdeSQL * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ErrMsgList )( 
            IdeSQL * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ErrMsgList )( 
            IdeSQL * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Query )( 
            IdeSQL * This,
            /* [in] */ BSTR bstrQuery,
            /* [retval][out] */ IDispatch **pRecSet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddDatabase )( 
            IdeSQL * This,
            /* [in] */ BSTR DataBase,
            /* [in] */ BSTR Path,
            /* [in] */ BSTR DatSize,
            /* [in] */ BSTR LogSize,
            /* [in] */ BSTR DatGrowth,
            /* [in] */ BSTR LogGrowth,
            /* [in] */ BSTR DatMaxSize,
            /* [in] */ BSTR LogMaxSize,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DatabaseExists )( 
            IdeSQL * This,
            /* [in] */ BSTR Server,
            /* [in] */ BSTR DataBase,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Execute )( 
            IdeSQL * This,
            /* [in] */ BSTR Query,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ImportBatchSize )( 
            IdeSQL * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ImportBatchSize )( 
            IdeSQL * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Err_MaxMessages )( 
            IdeSQL * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Err_MaxMessages )( 
            IdeSQL * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Err_NoInfo )( 
            IdeSQL * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Err_NoInfo )( 
            IdeSQL * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ID_Name )( 
            IdeSQL * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ID_Name )( 
            IdeSQL * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ID_Definition )( 
            IdeSQL * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ID_Definition )( 
            IdeSQL * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateTable )( 
            IdeSQL * This,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        END_INTERFACE
    } IdeSQLVtbl;

    interface IdeSQL
    {
        CONST_VTBL struct IdeSQLVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IdeSQL_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IdeSQL_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IdeSQL_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IdeSQL_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IdeSQL_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IdeSQL_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IdeSQL_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IdeSQL_get_Server(This,pVal)	\
    (This)->lpVtbl -> get_Server(This,pVal)

#define IdeSQL_put_Server(This,newVal)	\
    (This)->lpVtbl -> put_Server(This,newVal)

#define IdeSQL_get_Database(This,pVal)	\
    (This)->lpVtbl -> get_Database(This,pVal)

#define IdeSQL_put_Database(This,newVal)	\
    (This)->lpVtbl -> put_Database(This,newVal)

#define IdeSQL_get_Table(This,pVal)	\
    (This)->lpVtbl -> get_Table(This,pVal)

#define IdeSQL_put_Table(This,newVal)	\
    (This)->lpVtbl -> put_Table(This,newVal)

#define IdeSQL_get_UserID(This,pVal)	\
    (This)->lpVtbl -> get_UserID(This,pVal)

#define IdeSQL_put_UserID(This,newVal)	\
    (This)->lpVtbl -> put_UserID(This,newVal)

#define IdeSQL_get_Password(This,pVal)	\
    (This)->lpVtbl -> get_Password(This,pVal)

#define IdeSQL_put_Password(This,newVal)	\
    (This)->lpVtbl -> put_Password(This,newVal)

#define IdeSQL_get_RowsLoaded(This,pVal)	\
    (This)->lpVtbl -> get_RowsLoaded(This,pVal)

#define IdeSQL_get_ErrMsgs(This,pVal)	\
    (This)->lpVtbl -> get_ErrMsgs(This,pVal)

#define IdeSQL_put_Format(This,pObj)	\
    (This)->lpVtbl -> put_Format(This,pObj)

#define IdeSQL_get_Format(This,pObj)	\
    (This)->lpVtbl -> get_Format(This,pObj)

#define IdeSQL_Import(This,pIn,action,bRetVal)	\
    (This)->lpVtbl -> Import(This,pIn,action,bRetVal)

#define IdeSQL_GetCreateTableSQL(This,pRetVal)	\
    (This)->lpVtbl -> GetCreateTableSQL(This,pRetVal)

#define IdeSQL_Export(This,pIn,action,bRetVal)	\
    (This)->lpVtbl -> Export(This,pIn,action,bRetVal)

#define IdeSQL_SetFormat(This,pObj,pRetVal)	\
    (This)->lpVtbl -> SetFormat(This,pObj,pRetVal)

#define IdeSQL_get_TraceLogOn(This,pVal)	\
    (This)->lpVtbl -> get_TraceLogOn(This,pVal)

#define IdeSQL_put_TraceLogOn(This,newVal)	\
    (This)->lpVtbl -> put_TraceLogOn(This,newVal)

#define IdeSQL_ImportMulti(This,pIn,action,AddSource,pRetVal)	\
    (This)->lpVtbl -> ImportMulti(This,pIn,action,AddSource,pRetVal)

#define IdeSQL_ExportMulti(This,pIn,action,rule,lValue,pRetVal)	\
    (This)->lpVtbl -> ExportMulti(This,pIn,action,rule,lValue,pRetVal)

#define IdeSQL_Exists(This,pRetVal)	\
    (This)->lpVtbl -> Exists(This,pRetVal)

#define IdeSQL_get_Version(This,pVal)	\
    (This)->lpVtbl -> get_Version(This,pVal)

#define IdeSQL_LoadFormat(This,pRetVal)	\
    (This)->lpVtbl -> LoadFormat(This,pRetVal)

#define IdeSQL_FieldResize(This,option,pRetVal)	\
    (This)->lpVtbl -> FieldResize(This,option,pRetVal)

#define IdeSQL_get_ExportQuery(This,pVal)	\
    (This)->lpVtbl -> get_ExportQuery(This,pVal)

#define IdeSQL_put_ExportQuery(This,newVal)	\
    (This)->lpVtbl -> put_ExportQuery(This,newVal)

#define IdeSQL_get_Owner(This,pVal)	\
    (This)->lpVtbl -> get_Owner(This,pVal)

#define IdeSQL_put_Owner(This,newVal)	\
    (This)->lpVtbl -> put_Owner(This,newVal)

#define IdeSQL_get_ErrMsgList(This,pVal)	\
    (This)->lpVtbl -> get_ErrMsgList(This,pVal)

#define IdeSQL_put_ErrMsgList(This,newVal)	\
    (This)->lpVtbl -> put_ErrMsgList(This,newVal)

#define IdeSQL_Query(This,bstrQuery,pRecSet)	\
    (This)->lpVtbl -> Query(This,bstrQuery,pRecSet)

#define IdeSQL_AddDatabase(This,DataBase,Path,DatSize,LogSize,DatGrowth,LogGrowth,DatMaxSize,LogMaxSize,pRetVal)	\
    (This)->lpVtbl -> AddDatabase(This,DataBase,Path,DatSize,LogSize,DatGrowth,LogGrowth,DatMaxSize,LogMaxSize,pRetVal)

#define IdeSQL_DatabaseExists(This,Server,DataBase,pRetVal)	\
    (This)->lpVtbl -> DatabaseExists(This,Server,DataBase,pRetVal)

#define IdeSQL_Execute(This,Query,pRetVal)	\
    (This)->lpVtbl -> Execute(This,Query,pRetVal)

#define IdeSQL_get_ImportBatchSize(This,pVal)	\
    (This)->lpVtbl -> get_ImportBatchSize(This,pVal)

#define IdeSQL_put_ImportBatchSize(This,newVal)	\
    (This)->lpVtbl -> put_ImportBatchSize(This,newVal)

#define IdeSQL_get_Err_MaxMessages(This,pVal)	\
    (This)->lpVtbl -> get_Err_MaxMessages(This,pVal)

#define IdeSQL_put_Err_MaxMessages(This,newVal)	\
    (This)->lpVtbl -> put_Err_MaxMessages(This,newVal)

#define IdeSQL_get_Err_NoInfo(This,pVal)	\
    (This)->lpVtbl -> get_Err_NoInfo(This,pVal)

#define IdeSQL_put_Err_NoInfo(This,newVal)	\
    (This)->lpVtbl -> put_Err_NoInfo(This,newVal)

#define IdeSQL_get_ID_Name(This,pVal)	\
    (This)->lpVtbl -> get_ID_Name(This,pVal)

#define IdeSQL_put_ID_Name(This,newVal)	\
    (This)->lpVtbl -> put_ID_Name(This,newVal)

#define IdeSQL_get_ID_Definition(This,pVal)	\
    (This)->lpVtbl -> get_ID_Definition(This,pVal)

#define IdeSQL_put_ID_Definition(This,newVal)	\
    (This)->lpVtbl -> put_ID_Definition(This,newVal)

#define IdeSQL_CreateTable(This,pRetVal)	\
    (This)->lpVtbl -> CreateTable(This,pRetVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeSQL_get_Server_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeSQL_get_Server_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeSQL_put_Server_Proxy( 
    IdeSQL * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IdeSQL_put_Server_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeSQL_get_Database_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeSQL_get_Database_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeSQL_put_Database_Proxy( 
    IdeSQL * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IdeSQL_put_Database_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeSQL_get_Table_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeSQL_get_Table_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeSQL_put_Table_Proxy( 
    IdeSQL * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IdeSQL_put_Table_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeSQL_get_UserID_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeSQL_get_UserID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeSQL_put_UserID_Proxy( 
    IdeSQL * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IdeSQL_put_UserID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeSQL_get_Password_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeSQL_get_Password_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeSQL_put_Password_Proxy( 
    IdeSQL * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IdeSQL_put_Password_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeSQL_get_RowsLoaded_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IdeSQL_get_RowsLoaded_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeSQL_get_ErrMsgs_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeSQL_get_ErrMsgs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeSQL_put_Format_Proxy( 
    IdeSQL * This,
    /* [in] */ IFormat *pObj);


void __RPC_STUB IdeSQL_put_Format_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeSQL_get_Format_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ IFormat **pObj);


void __RPC_STUB IdeSQL_get_Format_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeSQL_Import_Proxy( 
    IdeSQL * This,
    /* [in] */ IDispatch *pIn,
    /* [in] */ DESTACTION action,
    /* [retval][out] */ VARIANT_BOOL *bRetVal);


void __RPC_STUB IdeSQL_Import_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeSQL_GetCreateTableSQL_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ BSTR *pRetVal);


void __RPC_STUB IdeSQL_GetCreateTableSQL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeSQL_Export_Proxy( 
    IdeSQL * This,
    /* [in] */ IDispatch *pIn,
    /* [in] */ DESTACTION action,
    /* [retval][out] */ VARIANT_BOOL *bRetVal);


void __RPC_STUB IdeSQL_Export_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeSQL_SetFormat_Proxy( 
    IdeSQL * This,
    /* [in] */ IFormat *pObj,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeSQL_SetFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeSQL_get_TraceLogOn_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IdeSQL_get_TraceLogOn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeSQL_put_TraceLogOn_Proxy( 
    IdeSQL * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IdeSQL_put_TraceLogOn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeSQL_ImportMulti_Proxy( 
    IdeSQL * This,
    /* [in] */ IDispatch *pIn,
    /* [in] */ DESTACTION action,
    /* [in] */ VARIANT_BOOL AddSource,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeSQL_ImportMulti_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeSQL_ExportMulti_Proxy( 
    IdeSQL * This,
    /* [in] */ IDispatch *pIn,
    /* [in] */ DESTACTION action,
    /* [in] */ SPLITRULE rule,
    /* [in] */ long lValue,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeSQL_ExportMulti_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeSQL_Exists_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeSQL_Exists_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeSQL_get_Version_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeSQL_get_Version_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeSQL_LoadFormat_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeSQL_LoadFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeSQL_FieldResize_Proxy( 
    IdeSQL * This,
    /* [in] */ RESIZEOPTION option,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeSQL_FieldResize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeSQL_get_ExportQuery_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeSQL_get_ExportQuery_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeSQL_put_ExportQuery_Proxy( 
    IdeSQL * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IdeSQL_put_ExportQuery_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeSQL_get_Owner_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeSQL_get_Owner_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeSQL_put_Owner_Proxy( 
    IdeSQL * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IdeSQL_put_Owner_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeSQL_get_ErrMsgList_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeSQL_get_ErrMsgList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeSQL_put_ErrMsgList_Proxy( 
    IdeSQL * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IdeSQL_put_ErrMsgList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeSQL_Query_Proxy( 
    IdeSQL * This,
    /* [in] */ BSTR bstrQuery,
    /* [retval][out] */ IDispatch **pRecSet);


void __RPC_STUB IdeSQL_Query_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeSQL_AddDatabase_Proxy( 
    IdeSQL * This,
    /* [in] */ BSTR DataBase,
    /* [in] */ BSTR Path,
    /* [in] */ BSTR DatSize,
    /* [in] */ BSTR LogSize,
    /* [in] */ BSTR DatGrowth,
    /* [in] */ BSTR LogGrowth,
    /* [in] */ BSTR DatMaxSize,
    /* [in] */ BSTR LogMaxSize,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeSQL_AddDatabase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeSQL_DatabaseExists_Proxy( 
    IdeSQL * This,
    /* [in] */ BSTR Server,
    /* [in] */ BSTR DataBase,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeSQL_DatabaseExists_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeSQL_Execute_Proxy( 
    IdeSQL * This,
    /* [in] */ BSTR Query,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeSQL_Execute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeSQL_get_ImportBatchSize_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IdeSQL_get_ImportBatchSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeSQL_put_ImportBatchSize_Proxy( 
    IdeSQL * This,
    /* [in] */ long newVal);


void __RPC_STUB IdeSQL_put_ImportBatchSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeSQL_get_Err_MaxMessages_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IdeSQL_get_Err_MaxMessages_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeSQL_put_Err_MaxMessages_Proxy( 
    IdeSQL * This,
    /* [in] */ long newVal);


void __RPC_STUB IdeSQL_put_Err_MaxMessages_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeSQL_get_Err_NoInfo_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IdeSQL_get_Err_NoInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeSQL_put_Err_NoInfo_Proxy( 
    IdeSQL * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IdeSQL_put_Err_NoInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeSQL_get_ID_Name_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeSQL_get_ID_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeSQL_put_ID_Name_Proxy( 
    IdeSQL * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IdeSQL_put_ID_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeSQL_get_ID_Definition_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeSQL_get_ID_Definition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeSQL_put_ID_Definition_Proxy( 
    IdeSQL * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IdeSQL_put_ID_Definition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeSQL_CreateTable_Proxy( 
    IdeSQL * This,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeSQL_CreateTable_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IdeSQL_INTERFACE_DEFINED__ */


#ifndef __IdeFixed_INTERFACE_DEFINED__
#define __IdeFixed_INTERFACE_DEFINED__

/* interface IdeFixed */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IdeFixed;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A28B384B-80DF-426F-8AAC-28AB9ED28F1F")
    IdeFixed : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Directory( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Directory( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FileName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_FileName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RowsLoaded( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrMsgs( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Export( 
            /* [in] */ IDispatch *pIn,
            /* [in] */ DESTACTION action,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Import( 
            /* [in] */ IDispatch *pIn,
            /* [in] */ DESTACTION action,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFormat( 
            /* [in] */ IFormat *pObj,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Format( 
            /* [retval][out] */ IFormat **pObj) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Format( 
            /* [in] */ IFormat *pObj) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TraceLogOn( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TraceLogOn( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Exists( 
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Version( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FirstRow( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_FirstRow( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LastRow( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_LastRow( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrMsgList( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadFormat( 
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ValidateFile( 
            /* [in] */ BSTR FileName,
            /* [in] */ long TestRecNum,
            /* [in] */ long MaxErrors,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IdeFixedVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IdeFixed * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IdeFixed * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IdeFixed * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IdeFixed * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IdeFixed * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IdeFixed * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IdeFixed * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Directory )( 
            IdeFixed * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Directory )( 
            IdeFixed * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FileName )( 
            IdeFixed * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FileName )( 
            IdeFixed * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RowsLoaded )( 
            IdeFixed * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ErrMsgs )( 
            IdeFixed * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Export )( 
            IdeFixed * This,
            /* [in] */ IDispatch *pIn,
            /* [in] */ DESTACTION action,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Import )( 
            IdeFixed * This,
            /* [in] */ IDispatch *pIn,
            /* [in] */ DESTACTION action,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFormat )( 
            IdeFixed * This,
            /* [in] */ IFormat *pObj,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Format )( 
            IdeFixed * This,
            /* [retval][out] */ IFormat **pObj);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Format )( 
            IdeFixed * This,
            /* [in] */ IFormat *pObj);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TraceLogOn )( 
            IdeFixed * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TraceLogOn )( 
            IdeFixed * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Exists )( 
            IdeFixed * This,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Version )( 
            IdeFixed * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FirstRow )( 
            IdeFixed * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FirstRow )( 
            IdeFixed * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastRow )( 
            IdeFixed * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LastRow )( 
            IdeFixed * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ErrMsgList )( 
            IdeFixed * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadFormat )( 
            IdeFixed * This,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidateFile )( 
            IdeFixed * This,
            /* [in] */ BSTR FileName,
            /* [in] */ long TestRecNum,
            /* [in] */ long MaxErrors,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        END_INTERFACE
    } IdeFixedVtbl;

    interface IdeFixed
    {
        CONST_VTBL struct IdeFixedVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IdeFixed_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IdeFixed_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IdeFixed_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IdeFixed_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IdeFixed_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IdeFixed_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IdeFixed_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IdeFixed_get_Directory(This,pVal)	\
    (This)->lpVtbl -> get_Directory(This,pVal)

#define IdeFixed_put_Directory(This,newVal)	\
    (This)->lpVtbl -> put_Directory(This,newVal)

#define IdeFixed_get_FileName(This,pVal)	\
    (This)->lpVtbl -> get_FileName(This,pVal)

#define IdeFixed_put_FileName(This,newVal)	\
    (This)->lpVtbl -> put_FileName(This,newVal)

#define IdeFixed_get_RowsLoaded(This,pVal)	\
    (This)->lpVtbl -> get_RowsLoaded(This,pVal)

#define IdeFixed_get_ErrMsgs(This,pVal)	\
    (This)->lpVtbl -> get_ErrMsgs(This,pVal)

#define IdeFixed_Export(This,pIn,action,pRetVal)	\
    (This)->lpVtbl -> Export(This,pIn,action,pRetVal)

#define IdeFixed_Import(This,pIn,action,pRetVal)	\
    (This)->lpVtbl -> Import(This,pIn,action,pRetVal)

#define IdeFixed_SetFormat(This,pObj,pRetVal)	\
    (This)->lpVtbl -> SetFormat(This,pObj,pRetVal)

#define IdeFixed_get_Format(This,pObj)	\
    (This)->lpVtbl -> get_Format(This,pObj)

#define IdeFixed_put_Format(This,pObj)	\
    (This)->lpVtbl -> put_Format(This,pObj)

#define IdeFixed_get_TraceLogOn(This,pVal)	\
    (This)->lpVtbl -> get_TraceLogOn(This,pVal)

#define IdeFixed_put_TraceLogOn(This,newVal)	\
    (This)->lpVtbl -> put_TraceLogOn(This,newVal)

#define IdeFixed_Exists(This,pRetVal)	\
    (This)->lpVtbl -> Exists(This,pRetVal)

#define IdeFixed_get_Version(This,pVal)	\
    (This)->lpVtbl -> get_Version(This,pVal)

#define IdeFixed_get_FirstRow(This,pVal)	\
    (This)->lpVtbl -> get_FirstRow(This,pVal)

#define IdeFixed_put_FirstRow(This,newVal)	\
    (This)->lpVtbl -> put_FirstRow(This,newVal)

#define IdeFixed_get_LastRow(This,pVal)	\
    (This)->lpVtbl -> get_LastRow(This,pVal)

#define IdeFixed_put_LastRow(This,newVal)	\
    (This)->lpVtbl -> put_LastRow(This,newVal)

#define IdeFixed_get_ErrMsgList(This,pVal)	\
    (This)->lpVtbl -> get_ErrMsgList(This,pVal)

#define IdeFixed_LoadFormat(This,pRetVal)	\
    (This)->lpVtbl -> LoadFormat(This,pRetVal)

#define IdeFixed_ValidateFile(This,FileName,TestRecNum,MaxErrors,pRetVal)	\
    (This)->lpVtbl -> ValidateFile(This,FileName,TestRecNum,MaxErrors,pRetVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeFixed_get_Directory_Proxy( 
    IdeFixed * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeFixed_get_Directory_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeFixed_put_Directory_Proxy( 
    IdeFixed * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IdeFixed_put_Directory_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeFixed_get_FileName_Proxy( 
    IdeFixed * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeFixed_get_FileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeFixed_put_FileName_Proxy( 
    IdeFixed * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IdeFixed_put_FileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeFixed_get_RowsLoaded_Proxy( 
    IdeFixed * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IdeFixed_get_RowsLoaded_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeFixed_get_ErrMsgs_Proxy( 
    IdeFixed * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeFixed_get_ErrMsgs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeFixed_Export_Proxy( 
    IdeFixed * This,
    /* [in] */ IDispatch *pIn,
    /* [in] */ DESTACTION action,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeFixed_Export_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeFixed_Import_Proxy( 
    IdeFixed * This,
    /* [in] */ IDispatch *pIn,
    /* [in] */ DESTACTION action,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeFixed_Import_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeFixed_SetFormat_Proxy( 
    IdeFixed * This,
    /* [in] */ IFormat *pObj,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeFixed_SetFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeFixed_get_Format_Proxy( 
    IdeFixed * This,
    /* [retval][out] */ IFormat **pObj);


void __RPC_STUB IdeFixed_get_Format_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeFixed_put_Format_Proxy( 
    IdeFixed * This,
    /* [in] */ IFormat *pObj);


void __RPC_STUB IdeFixed_put_Format_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeFixed_get_TraceLogOn_Proxy( 
    IdeFixed * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IdeFixed_get_TraceLogOn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeFixed_put_TraceLogOn_Proxy( 
    IdeFixed * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IdeFixed_put_TraceLogOn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeFixed_Exists_Proxy( 
    IdeFixed * This,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeFixed_Exists_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeFixed_get_Version_Proxy( 
    IdeFixed * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeFixed_get_Version_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeFixed_get_FirstRow_Proxy( 
    IdeFixed * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IdeFixed_get_FirstRow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeFixed_put_FirstRow_Proxy( 
    IdeFixed * This,
    /* [in] */ long newVal);


void __RPC_STUB IdeFixed_put_FirstRow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeFixed_get_LastRow_Proxy( 
    IdeFixed * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IdeFixed_get_LastRow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeFixed_put_LastRow_Proxy( 
    IdeFixed * This,
    /* [in] */ long newVal);


void __RPC_STUB IdeFixed_put_LastRow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeFixed_get_ErrMsgList_Proxy( 
    IdeFixed * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeFixed_get_ErrMsgList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeFixed_LoadFormat_Proxy( 
    IdeFixed * This,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeFixed_LoadFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeFixed_ValidateFile_Proxy( 
    IdeFixed * This,
    /* [in] */ BSTR FileName,
    /* [in] */ long TestRecNum,
    /* [in] */ long MaxErrors,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeFixed_ValidateFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IdeFixed_INTERFACE_DEFINED__ */


#ifndef __IdeDelim_INTERFACE_DEFINED__
#define __IdeDelim_INTERFACE_DEFINED__

/* interface IdeDelim */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IdeDelim;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2E7A8FCB-DACB-4F5C-B2F8-055EF8268E05")
    IdeDelim : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Directory( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Directory( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FileName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_FileName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RowsLoaded( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrMsgs( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Export( 
            /* [in] */ IDispatch *pIn,
            /* [in] */ DESTACTION action,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Import( 
            /* [in] */ IDispatch *pIn,
            /* [in] */ DESTACTION action,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetFormat( 
            /* [in] */ IFormat *pObj,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Format( 
            /* [retval][out] */ IFormat **pObj) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Format( 
            /* [in] */ IFormat *pObj) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TraceLogOn( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TraceLogOn( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Exists( 
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Version( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FirstRow( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_FirstRow( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LastRow( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_LastRow( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Delimiter( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Delimiter( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TextQualifier( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TextQualifier( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Header( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Header( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RowDelimiter( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RowDelimiter( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrMsgList( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadFormat( 
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ValidateFile( 
            /* [in] */ BSTR FileName,
            /* [in] */ long TestRecNum,
            /* [in] */ long MaxErrors,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IdeDelimVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IdeDelim * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IdeDelim * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IdeDelim * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IdeDelim * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IdeDelim * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IdeDelim * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IdeDelim * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Directory )( 
            IdeDelim * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Directory )( 
            IdeDelim * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FileName )( 
            IdeDelim * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FileName )( 
            IdeDelim * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RowsLoaded )( 
            IdeDelim * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ErrMsgs )( 
            IdeDelim * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Export )( 
            IdeDelim * This,
            /* [in] */ IDispatch *pIn,
            /* [in] */ DESTACTION action,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Import )( 
            IdeDelim * This,
            /* [in] */ IDispatch *pIn,
            /* [in] */ DESTACTION action,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetFormat )( 
            IdeDelim * This,
            /* [in] */ IFormat *pObj,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Format )( 
            IdeDelim * This,
            /* [retval][out] */ IFormat **pObj);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Format )( 
            IdeDelim * This,
            /* [in] */ IFormat *pObj);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TraceLogOn )( 
            IdeDelim * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TraceLogOn )( 
            IdeDelim * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Exists )( 
            IdeDelim * This,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Version )( 
            IdeDelim * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FirstRow )( 
            IdeDelim * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FirstRow )( 
            IdeDelim * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastRow )( 
            IdeDelim * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LastRow )( 
            IdeDelim * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Delimiter )( 
            IdeDelim * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Delimiter )( 
            IdeDelim * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TextQualifier )( 
            IdeDelim * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TextQualifier )( 
            IdeDelim * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Header )( 
            IdeDelim * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Header )( 
            IdeDelim * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RowDelimiter )( 
            IdeDelim * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RowDelimiter )( 
            IdeDelim * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ErrMsgList )( 
            IdeDelim * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadFormat )( 
            IdeDelim * This,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ValidateFile )( 
            IdeDelim * This,
            /* [in] */ BSTR FileName,
            /* [in] */ long TestRecNum,
            /* [in] */ long MaxErrors,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        END_INTERFACE
    } IdeDelimVtbl;

    interface IdeDelim
    {
        CONST_VTBL struct IdeDelimVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IdeDelim_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IdeDelim_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IdeDelim_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IdeDelim_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IdeDelim_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IdeDelim_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IdeDelim_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IdeDelim_get_Directory(This,pVal)	\
    (This)->lpVtbl -> get_Directory(This,pVal)

#define IdeDelim_put_Directory(This,newVal)	\
    (This)->lpVtbl -> put_Directory(This,newVal)

#define IdeDelim_get_FileName(This,pVal)	\
    (This)->lpVtbl -> get_FileName(This,pVal)

#define IdeDelim_put_FileName(This,newVal)	\
    (This)->lpVtbl -> put_FileName(This,newVal)

#define IdeDelim_get_RowsLoaded(This,pVal)	\
    (This)->lpVtbl -> get_RowsLoaded(This,pVal)

#define IdeDelim_get_ErrMsgs(This,pVal)	\
    (This)->lpVtbl -> get_ErrMsgs(This,pVal)

#define IdeDelim_Export(This,pIn,action,pRetVal)	\
    (This)->lpVtbl -> Export(This,pIn,action,pRetVal)

#define IdeDelim_Import(This,pIn,action,pRetVal)	\
    (This)->lpVtbl -> Import(This,pIn,action,pRetVal)

#define IdeDelim_SetFormat(This,pObj,pRetVal)	\
    (This)->lpVtbl -> SetFormat(This,pObj,pRetVal)

#define IdeDelim_get_Format(This,pObj)	\
    (This)->lpVtbl -> get_Format(This,pObj)

#define IdeDelim_put_Format(This,pObj)	\
    (This)->lpVtbl -> put_Format(This,pObj)

#define IdeDelim_get_TraceLogOn(This,pVal)	\
    (This)->lpVtbl -> get_TraceLogOn(This,pVal)

#define IdeDelim_put_TraceLogOn(This,newVal)	\
    (This)->lpVtbl -> put_TraceLogOn(This,newVal)

#define IdeDelim_Exists(This,pRetVal)	\
    (This)->lpVtbl -> Exists(This,pRetVal)

#define IdeDelim_get_Version(This,pVal)	\
    (This)->lpVtbl -> get_Version(This,pVal)

#define IdeDelim_get_FirstRow(This,pVal)	\
    (This)->lpVtbl -> get_FirstRow(This,pVal)

#define IdeDelim_put_FirstRow(This,newVal)	\
    (This)->lpVtbl -> put_FirstRow(This,newVal)

#define IdeDelim_get_LastRow(This,pVal)	\
    (This)->lpVtbl -> get_LastRow(This,pVal)

#define IdeDelim_put_LastRow(This,newVal)	\
    (This)->lpVtbl -> put_LastRow(This,newVal)

#define IdeDelim_get_Delimiter(This,pVal)	\
    (This)->lpVtbl -> get_Delimiter(This,pVal)

#define IdeDelim_put_Delimiter(This,newVal)	\
    (This)->lpVtbl -> put_Delimiter(This,newVal)

#define IdeDelim_get_TextQualifier(This,pVal)	\
    (This)->lpVtbl -> get_TextQualifier(This,pVal)

#define IdeDelim_put_TextQualifier(This,newVal)	\
    (This)->lpVtbl -> put_TextQualifier(This,newVal)

#define IdeDelim_get_Header(This,pVal)	\
    (This)->lpVtbl -> get_Header(This,pVal)

#define IdeDelim_put_Header(This,newVal)	\
    (This)->lpVtbl -> put_Header(This,newVal)

#define IdeDelim_get_RowDelimiter(This,pVal)	\
    (This)->lpVtbl -> get_RowDelimiter(This,pVal)

#define IdeDelim_put_RowDelimiter(This,newVal)	\
    (This)->lpVtbl -> put_RowDelimiter(This,newVal)

#define IdeDelim_get_ErrMsgList(This,pVal)	\
    (This)->lpVtbl -> get_ErrMsgList(This,pVal)

#define IdeDelim_LoadFormat(This,pRetVal)	\
    (This)->lpVtbl -> LoadFormat(This,pRetVal)

#define IdeDelim_ValidateFile(This,FileName,TestRecNum,MaxErrors,pRetVal)	\
    (This)->lpVtbl -> ValidateFile(This,FileName,TestRecNum,MaxErrors,pRetVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeDelim_get_Directory_Proxy( 
    IdeDelim * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeDelim_get_Directory_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeDelim_put_Directory_Proxy( 
    IdeDelim * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IdeDelim_put_Directory_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeDelim_get_FileName_Proxy( 
    IdeDelim * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeDelim_get_FileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeDelim_put_FileName_Proxy( 
    IdeDelim * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IdeDelim_put_FileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeDelim_get_RowsLoaded_Proxy( 
    IdeDelim * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IdeDelim_get_RowsLoaded_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeDelim_get_ErrMsgs_Proxy( 
    IdeDelim * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeDelim_get_ErrMsgs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeDelim_Export_Proxy( 
    IdeDelim * This,
    /* [in] */ IDispatch *pIn,
    /* [in] */ DESTACTION action,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeDelim_Export_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeDelim_Import_Proxy( 
    IdeDelim * This,
    /* [in] */ IDispatch *pIn,
    /* [in] */ DESTACTION action,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeDelim_Import_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeDelim_SetFormat_Proxy( 
    IdeDelim * This,
    /* [in] */ IFormat *pObj,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeDelim_SetFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeDelim_get_Format_Proxy( 
    IdeDelim * This,
    /* [retval][out] */ IFormat **pObj);


void __RPC_STUB IdeDelim_get_Format_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeDelim_put_Format_Proxy( 
    IdeDelim * This,
    /* [in] */ IFormat *pObj);


void __RPC_STUB IdeDelim_put_Format_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeDelim_get_TraceLogOn_Proxy( 
    IdeDelim * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IdeDelim_get_TraceLogOn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeDelim_put_TraceLogOn_Proxy( 
    IdeDelim * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IdeDelim_put_TraceLogOn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeDelim_Exists_Proxy( 
    IdeDelim * This,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeDelim_Exists_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeDelim_get_Version_Proxy( 
    IdeDelim * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeDelim_get_Version_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeDelim_get_FirstRow_Proxy( 
    IdeDelim * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IdeDelim_get_FirstRow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeDelim_put_FirstRow_Proxy( 
    IdeDelim * This,
    /* [in] */ long newVal);


void __RPC_STUB IdeDelim_put_FirstRow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeDelim_get_LastRow_Proxy( 
    IdeDelim * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IdeDelim_get_LastRow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeDelim_put_LastRow_Proxy( 
    IdeDelim * This,
    /* [in] */ long newVal);


void __RPC_STUB IdeDelim_put_LastRow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeDelim_get_Delimiter_Proxy( 
    IdeDelim * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeDelim_get_Delimiter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeDelim_put_Delimiter_Proxy( 
    IdeDelim * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IdeDelim_put_Delimiter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeDelim_get_TextQualifier_Proxy( 
    IdeDelim * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeDelim_get_TextQualifier_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeDelim_put_TextQualifier_Proxy( 
    IdeDelim * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IdeDelim_put_TextQualifier_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeDelim_get_Header_Proxy( 
    IdeDelim * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IdeDelim_get_Header_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeDelim_put_Header_Proxy( 
    IdeDelim * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IdeDelim_put_Header_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeDelim_get_RowDelimiter_Proxy( 
    IdeDelim * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeDelim_get_RowDelimiter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeDelim_put_RowDelimiter_Proxy( 
    IdeDelim * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IdeDelim_put_RowDelimiter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeDelim_get_ErrMsgList_Proxy( 
    IdeDelim * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeDelim_get_ErrMsgList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeDelim_LoadFormat_Proxy( 
    IdeDelim * This,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeDelim_LoadFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeDelim_ValidateFile_Proxy( 
    IdeDelim * This,
    /* [in] */ BSTR FileName,
    /* [in] */ long TestRecNum,
    /* [in] */ long MaxErrors,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeDelim_ValidateFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IdeDelim_INTERFACE_DEFINED__ */


#ifndef __IdeXBase_INTERFACE_DEFINED__
#define __IdeXBase_INTERFACE_DEFINED__

/* interface IdeXBase */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IdeXBase;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BADC7E4E-93F6-43DD-8FD7-C0F0EBA54F4C")
    IdeXBase : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Directory( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Directory( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Table( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Table( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrMsgList( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrMsgs( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RowsLoaded( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TraceLogOn( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TraceLogOn( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Version( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Export( 
            /* [in] */ IDispatch *pIn,
            /* [in] */ DESTACTION action,
            /* [retval][out] */ VARIANT_BOOL *bRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Import( 
            /* [in] */ IDispatch *pIn,
            /* [in] */ DESTACTION action,
            /* [retval][out] */ VARIANT_BOOL *bRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Exists( 
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FirstRow( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_FirstRow( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LastRow( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_LastRow( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Format( 
            /* [in] */ IFormat *pObj) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Format( 
            /* [retval][out] */ IFormat **pObj) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TableType( 
            /* [retval][out] */ XBASETYPE *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TableType( 
            /* [in] */ XBASETYPE newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadFormat( 
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IdeXBaseVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IdeXBase * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IdeXBase * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IdeXBase * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IdeXBase * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IdeXBase * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IdeXBase * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IdeXBase * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Directory )( 
            IdeXBase * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Directory )( 
            IdeXBase * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Table )( 
            IdeXBase * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Table )( 
            IdeXBase * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ErrMsgList )( 
            IdeXBase * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ErrMsgs )( 
            IdeXBase * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RowsLoaded )( 
            IdeXBase * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TraceLogOn )( 
            IdeXBase * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TraceLogOn )( 
            IdeXBase * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Version )( 
            IdeXBase * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Export )( 
            IdeXBase * This,
            /* [in] */ IDispatch *pIn,
            /* [in] */ DESTACTION action,
            /* [retval][out] */ VARIANT_BOOL *bRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Import )( 
            IdeXBase * This,
            /* [in] */ IDispatch *pIn,
            /* [in] */ DESTACTION action,
            /* [retval][out] */ VARIANT_BOOL *bRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Exists )( 
            IdeXBase * This,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FirstRow )( 
            IdeXBase * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FirstRow )( 
            IdeXBase * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastRow )( 
            IdeXBase * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LastRow )( 
            IdeXBase * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Format )( 
            IdeXBase * This,
            /* [in] */ IFormat *pObj);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Format )( 
            IdeXBase * This,
            /* [retval][out] */ IFormat **pObj);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TableType )( 
            IdeXBase * This,
            /* [retval][out] */ XBASETYPE *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TableType )( 
            IdeXBase * This,
            /* [in] */ XBASETYPE newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadFormat )( 
            IdeXBase * This,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        END_INTERFACE
    } IdeXBaseVtbl;

    interface IdeXBase
    {
        CONST_VTBL struct IdeXBaseVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IdeXBase_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IdeXBase_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IdeXBase_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IdeXBase_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IdeXBase_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IdeXBase_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IdeXBase_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IdeXBase_get_Directory(This,pVal)	\
    (This)->lpVtbl -> get_Directory(This,pVal)

#define IdeXBase_put_Directory(This,newVal)	\
    (This)->lpVtbl -> put_Directory(This,newVal)

#define IdeXBase_get_Table(This,pVal)	\
    (This)->lpVtbl -> get_Table(This,pVal)

#define IdeXBase_put_Table(This,newVal)	\
    (This)->lpVtbl -> put_Table(This,newVal)

#define IdeXBase_get_ErrMsgList(This,pVal)	\
    (This)->lpVtbl -> get_ErrMsgList(This,pVal)

#define IdeXBase_get_ErrMsgs(This,pVal)	\
    (This)->lpVtbl -> get_ErrMsgs(This,pVal)

#define IdeXBase_get_RowsLoaded(This,pVal)	\
    (This)->lpVtbl -> get_RowsLoaded(This,pVal)

#define IdeXBase_get_TraceLogOn(This,pVal)	\
    (This)->lpVtbl -> get_TraceLogOn(This,pVal)

#define IdeXBase_put_TraceLogOn(This,newVal)	\
    (This)->lpVtbl -> put_TraceLogOn(This,newVal)

#define IdeXBase_get_Version(This,pVal)	\
    (This)->lpVtbl -> get_Version(This,pVal)

#define IdeXBase_Export(This,pIn,action,bRetVal)	\
    (This)->lpVtbl -> Export(This,pIn,action,bRetVal)

#define IdeXBase_Import(This,pIn,action,bRetVal)	\
    (This)->lpVtbl -> Import(This,pIn,action,bRetVal)

#define IdeXBase_Exists(This,pRetVal)	\
    (This)->lpVtbl -> Exists(This,pRetVal)

#define IdeXBase_get_FirstRow(This,pVal)	\
    (This)->lpVtbl -> get_FirstRow(This,pVal)

#define IdeXBase_put_FirstRow(This,newVal)	\
    (This)->lpVtbl -> put_FirstRow(This,newVal)

#define IdeXBase_get_LastRow(This,pVal)	\
    (This)->lpVtbl -> get_LastRow(This,pVal)

#define IdeXBase_put_LastRow(This,newVal)	\
    (This)->lpVtbl -> put_LastRow(This,newVal)

#define IdeXBase_put_Format(This,pObj)	\
    (This)->lpVtbl -> put_Format(This,pObj)

#define IdeXBase_get_Format(This,pObj)	\
    (This)->lpVtbl -> get_Format(This,pObj)

#define IdeXBase_get_TableType(This,pVal)	\
    (This)->lpVtbl -> get_TableType(This,pVal)

#define IdeXBase_put_TableType(This,newVal)	\
    (This)->lpVtbl -> put_TableType(This,newVal)

#define IdeXBase_LoadFormat(This,pRetVal)	\
    (This)->lpVtbl -> LoadFormat(This,pRetVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeXBase_get_Directory_Proxy( 
    IdeXBase * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeXBase_get_Directory_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeXBase_put_Directory_Proxy( 
    IdeXBase * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IdeXBase_put_Directory_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeXBase_get_Table_Proxy( 
    IdeXBase * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeXBase_get_Table_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeXBase_put_Table_Proxy( 
    IdeXBase * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IdeXBase_put_Table_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeXBase_get_ErrMsgList_Proxy( 
    IdeXBase * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeXBase_get_ErrMsgList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeXBase_get_ErrMsgs_Proxy( 
    IdeXBase * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeXBase_get_ErrMsgs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeXBase_get_RowsLoaded_Proxy( 
    IdeXBase * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IdeXBase_get_RowsLoaded_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeXBase_get_TraceLogOn_Proxy( 
    IdeXBase * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IdeXBase_get_TraceLogOn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeXBase_put_TraceLogOn_Proxy( 
    IdeXBase * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IdeXBase_put_TraceLogOn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeXBase_get_Version_Proxy( 
    IdeXBase * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IdeXBase_get_Version_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeXBase_Export_Proxy( 
    IdeXBase * This,
    /* [in] */ IDispatch *pIn,
    /* [in] */ DESTACTION action,
    /* [retval][out] */ VARIANT_BOOL *bRetVal);


void __RPC_STUB IdeXBase_Export_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeXBase_Import_Proxy( 
    IdeXBase * This,
    /* [in] */ IDispatch *pIn,
    /* [in] */ DESTACTION action,
    /* [retval][out] */ VARIANT_BOOL *bRetVal);


void __RPC_STUB IdeXBase_Import_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeXBase_Exists_Proxy( 
    IdeXBase * This,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeXBase_Exists_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeXBase_get_FirstRow_Proxy( 
    IdeXBase * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IdeXBase_get_FirstRow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeXBase_put_FirstRow_Proxy( 
    IdeXBase * This,
    /* [in] */ long newVal);


void __RPC_STUB IdeXBase_put_FirstRow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeXBase_get_LastRow_Proxy( 
    IdeXBase * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IdeXBase_get_LastRow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeXBase_put_LastRow_Proxy( 
    IdeXBase * This,
    /* [in] */ long newVal);


void __RPC_STUB IdeXBase_put_LastRow_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeXBase_put_Format_Proxy( 
    IdeXBase * This,
    /* [in] */ IFormat *pObj);


void __RPC_STUB IdeXBase_put_Format_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeXBase_get_Format_Proxy( 
    IdeXBase * This,
    /* [retval][out] */ IFormat **pObj);


void __RPC_STUB IdeXBase_get_Format_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IdeXBase_get_TableType_Proxy( 
    IdeXBase * This,
    /* [retval][out] */ XBASETYPE *pVal);


void __RPC_STUB IdeXBase_get_TableType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IdeXBase_put_TableType_Proxy( 
    IdeXBase * This,
    /* [in] */ XBASETYPE newVal);


void __RPC_STUB IdeXBase_put_TableType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IdeXBase_LoadFormat_Proxy( 
    IdeXBase * This,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IdeXBase_LoadFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IdeXBase_INTERFACE_DEFINED__ */


#ifndef __IProcess_INTERFACE_DEFINED__
#define __IProcess_INTERFACE_DEFINED__

/* interface IProcess */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IProcess;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1DD63CF9-FAA9-4429-BB2B-B244ABB4A055")
    IProcess : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsSQLObject( 
            /* [in] */ IDispatch *pIDisp,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsFixedFileObject( 
            /* [in] */ IDispatch *pIDisp,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FixedToSQL( 
            /* [in] */ IdeFixed *pFixed,
            /* [in] */ IdeSQL *pSQL,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SQLToFixed( 
            /* [in] */ IdeSQL *pSQL,
            /* [in] */ IdeFixed *pFixed,
            /* [in] */ long lChunk,
            /* [in] */ long lFileNum,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrMsgs( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RowsLoaded( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_RowsLoaded( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TraceLogOn( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TraceLogOn( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TraceFileName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TraceFileName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MultiImportToSQL( 
            /* [in] */ IDispatch *pDisp,
            /* [in] */ IdeSQL *pSQL,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Action( 
            /* [retval][out] */ DESTACTION *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Action( 
            /* [in] */ DESTACTION newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DelimToSQL( 
            /* [in] */ IdeDelim *pDelim,
            /* [in] */ IdeSQL *pSQL,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsDelimFileObject( 
            /* [in] */ IDispatch *pIDisp,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrMsgList( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetFirstError( 
            /* [out] */ BSTR *pValOut,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetNextError( 
            /* [out] */ BSTR *pValOut,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AddSource( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AddSource( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE XBaseToSQL( 
            /* [in] */ IdeXBase *pXBase,
            /* [in] */ IdeSQL *pSQL,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsXBaseObject( 
            /* [in] */ IDispatch *pIDisp,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SQLToXBase( 
            /* [in] */ IdeSQL *pSQL,
            /* [in] */ IdeXBase *pXBase,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SQLToDelim( 
            /* [in] */ IdeSQL *pSQL,
            /* [in] */ IdeDelim *pDelim,
            /* [retval][out] */ VARIANT_BOOL *pRetVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IProcessVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IProcess * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IProcess * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IProcess * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IProcess * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IProcess * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IProcess * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IProcess * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *IsSQLObject )( 
            IProcess * This,
            /* [in] */ IDispatch *pIDisp,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *IsFixedFileObject )( 
            IProcess * This,
            /* [in] */ IDispatch *pIDisp,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FixedToSQL )( 
            IProcess * This,
            /* [in] */ IdeFixed *pFixed,
            /* [in] */ IdeSQL *pSQL,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SQLToFixed )( 
            IProcess * This,
            /* [in] */ IdeSQL *pSQL,
            /* [in] */ IdeFixed *pFixed,
            /* [in] */ long lChunk,
            /* [in] */ long lFileNum,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ErrMsgs )( 
            IProcess * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RowsLoaded )( 
            IProcess * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_RowsLoaded )( 
            IProcess * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TraceLogOn )( 
            IProcess * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TraceLogOn )( 
            IProcess * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TraceFileName )( 
            IProcess * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TraceFileName )( 
            IProcess * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MultiImportToSQL )( 
            IProcess * This,
            /* [in] */ IDispatch *pDisp,
            /* [in] */ IdeSQL *pSQL,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Action )( 
            IProcess * This,
            /* [retval][out] */ DESTACTION *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Action )( 
            IProcess * This,
            /* [in] */ DESTACTION newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DelimToSQL )( 
            IProcess * This,
            /* [in] */ IdeDelim *pDelim,
            /* [in] */ IdeSQL *pSQL,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *IsDelimFileObject )( 
            IProcess * This,
            /* [in] */ IDispatch *pIDisp,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ErrMsgList )( 
            IProcess * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetFirstError )( 
            IProcess * This,
            /* [out] */ BSTR *pValOut,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetNextError )( 
            IProcess * This,
            /* [out] */ BSTR *pValOut,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AddSource )( 
            IProcess * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AddSource )( 
            IProcess * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *XBaseToSQL )( 
            IProcess * This,
            /* [in] */ IdeXBase *pXBase,
            /* [in] */ IdeSQL *pSQL,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *IsXBaseObject )( 
            IProcess * This,
            /* [in] */ IDispatch *pIDisp,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SQLToXBase )( 
            IProcess * This,
            /* [in] */ IdeSQL *pSQL,
            /* [in] */ IdeXBase *pXBase,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SQLToDelim )( 
            IProcess * This,
            /* [in] */ IdeSQL *pSQL,
            /* [in] */ IdeDelim *pDelim,
            /* [retval][out] */ VARIANT_BOOL *pRetVal);
        
        END_INTERFACE
    } IProcessVtbl;

    interface IProcess
    {
        CONST_VTBL struct IProcessVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IProcess_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IProcess_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IProcess_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IProcess_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IProcess_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IProcess_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IProcess_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IProcess_IsSQLObject(This,pIDisp,pRetVal)	\
    (This)->lpVtbl -> IsSQLObject(This,pIDisp,pRetVal)

#define IProcess_IsFixedFileObject(This,pIDisp,pRetVal)	\
    (This)->lpVtbl -> IsFixedFileObject(This,pIDisp,pRetVal)

#define IProcess_FixedToSQL(This,pFixed,pSQL,pRetVal)	\
    (This)->lpVtbl -> FixedToSQL(This,pFixed,pSQL,pRetVal)

#define IProcess_SQLToFixed(This,pSQL,pFixed,lChunk,lFileNum,pRetVal)	\
    (This)->lpVtbl -> SQLToFixed(This,pSQL,pFixed,lChunk,lFileNum,pRetVal)

#define IProcess_get_ErrMsgs(This,pVal)	\
    (This)->lpVtbl -> get_ErrMsgs(This,pVal)

#define IProcess_get_RowsLoaded(This,pVal)	\
    (This)->lpVtbl -> get_RowsLoaded(This,pVal)

#define IProcess_put_RowsLoaded(This,newVal)	\
    (This)->lpVtbl -> put_RowsLoaded(This,newVal)

#define IProcess_get_TraceLogOn(This,pVal)	\
    (This)->lpVtbl -> get_TraceLogOn(This,pVal)

#define IProcess_put_TraceLogOn(This,newVal)	\
    (This)->lpVtbl -> put_TraceLogOn(This,newVal)

#define IProcess_get_TraceFileName(This,pVal)	\
    (This)->lpVtbl -> get_TraceFileName(This,pVal)

#define IProcess_put_TraceFileName(This,newVal)	\
    (This)->lpVtbl -> put_TraceFileName(This,newVal)

#define IProcess_MultiImportToSQL(This,pDisp,pSQL,pRetVal)	\
    (This)->lpVtbl -> MultiImportToSQL(This,pDisp,pSQL,pRetVal)

#define IProcess_get_Action(This,pVal)	\
    (This)->lpVtbl -> get_Action(This,pVal)

#define IProcess_put_Action(This,newVal)	\
    (This)->lpVtbl -> put_Action(This,newVal)

#define IProcess_DelimToSQL(This,pDelim,pSQL,pRetVal)	\
    (This)->lpVtbl -> DelimToSQL(This,pDelim,pSQL,pRetVal)

#define IProcess_IsDelimFileObject(This,pIDisp,pRetVal)	\
    (This)->lpVtbl -> IsDelimFileObject(This,pIDisp,pRetVal)

#define IProcess_get_ErrMsgList(This,pVal)	\
    (This)->lpVtbl -> get_ErrMsgList(This,pVal)

#define IProcess_GetFirstError(This,pValOut,pVal)	\
    (This)->lpVtbl -> GetFirstError(This,pValOut,pVal)

#define IProcess_GetNextError(This,pValOut,pVal)	\
    (This)->lpVtbl -> GetNextError(This,pValOut,pVal)

#define IProcess_get_AddSource(This,pVal)	\
    (This)->lpVtbl -> get_AddSource(This,pVal)

#define IProcess_put_AddSource(This,newVal)	\
    (This)->lpVtbl -> put_AddSource(This,newVal)

#define IProcess_XBaseToSQL(This,pXBase,pSQL,pRetVal)	\
    (This)->lpVtbl -> XBaseToSQL(This,pXBase,pSQL,pRetVal)

#define IProcess_IsXBaseObject(This,pIDisp,pRetVal)	\
    (This)->lpVtbl -> IsXBaseObject(This,pIDisp,pRetVal)

#define IProcess_SQLToXBase(This,pSQL,pXBase,pRetVal)	\
    (This)->lpVtbl -> SQLToXBase(This,pSQL,pXBase,pRetVal)

#define IProcess_SQLToDelim(This,pSQL,pDelim,pRetVal)	\
    (This)->lpVtbl -> SQLToDelim(This,pSQL,pDelim,pRetVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IProcess_IsSQLObject_Proxy( 
    IProcess * This,
    /* [in] */ IDispatch *pIDisp,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IProcess_IsSQLObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IProcess_IsFixedFileObject_Proxy( 
    IProcess * This,
    /* [in] */ IDispatch *pIDisp,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IProcess_IsFixedFileObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IProcess_FixedToSQL_Proxy( 
    IProcess * This,
    /* [in] */ IdeFixed *pFixed,
    /* [in] */ IdeSQL *pSQL,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IProcess_FixedToSQL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IProcess_SQLToFixed_Proxy( 
    IProcess * This,
    /* [in] */ IdeSQL *pSQL,
    /* [in] */ IdeFixed *pFixed,
    /* [in] */ long lChunk,
    /* [in] */ long lFileNum,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IProcess_SQLToFixed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IProcess_get_ErrMsgs_Proxy( 
    IProcess * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IProcess_get_ErrMsgs_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IProcess_get_RowsLoaded_Proxy( 
    IProcess * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IProcess_get_RowsLoaded_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IProcess_put_RowsLoaded_Proxy( 
    IProcess * This,
    /* [in] */ long newVal);


void __RPC_STUB IProcess_put_RowsLoaded_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IProcess_get_TraceLogOn_Proxy( 
    IProcess * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IProcess_get_TraceLogOn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IProcess_put_TraceLogOn_Proxy( 
    IProcess * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IProcess_put_TraceLogOn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IProcess_get_TraceFileName_Proxy( 
    IProcess * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IProcess_get_TraceFileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IProcess_put_TraceFileName_Proxy( 
    IProcess * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IProcess_put_TraceFileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IProcess_MultiImportToSQL_Proxy( 
    IProcess * This,
    /* [in] */ IDispatch *pDisp,
    /* [in] */ IdeSQL *pSQL,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IProcess_MultiImportToSQL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IProcess_get_Action_Proxy( 
    IProcess * This,
    /* [retval][out] */ DESTACTION *pVal);


void __RPC_STUB IProcess_get_Action_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IProcess_put_Action_Proxy( 
    IProcess * This,
    /* [in] */ DESTACTION newVal);


void __RPC_STUB IProcess_put_Action_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IProcess_DelimToSQL_Proxy( 
    IProcess * This,
    /* [in] */ IdeDelim *pDelim,
    /* [in] */ IdeSQL *pSQL,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IProcess_DelimToSQL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IProcess_IsDelimFileObject_Proxy( 
    IProcess * This,
    /* [in] */ IDispatch *pIDisp,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IProcess_IsDelimFileObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IProcess_get_ErrMsgList_Proxy( 
    IProcess * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IProcess_get_ErrMsgList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IProcess_GetFirstError_Proxy( 
    IProcess * This,
    /* [out] */ BSTR *pValOut,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IProcess_GetFirstError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IProcess_GetNextError_Proxy( 
    IProcess * This,
    /* [out] */ BSTR *pValOut,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IProcess_GetNextError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IProcess_get_AddSource_Proxy( 
    IProcess * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IProcess_get_AddSource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IProcess_put_AddSource_Proxy( 
    IProcess * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IProcess_put_AddSource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IProcess_XBaseToSQL_Proxy( 
    IProcess * This,
    /* [in] */ IdeXBase *pXBase,
    /* [in] */ IdeSQL *pSQL,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IProcess_XBaseToSQL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IProcess_IsXBaseObject_Proxy( 
    IProcess * This,
    /* [in] */ IDispatch *pIDisp,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IProcess_IsXBaseObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IProcess_SQLToXBase_Proxy( 
    IProcess * This,
    /* [in] */ IdeSQL *pSQL,
    /* [in] */ IdeXBase *pXBase,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IProcess_SQLToXBase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IProcess_SQLToDelim_Proxy( 
    IProcess * This,
    /* [in] */ IdeSQL *pSQL,
    /* [in] */ IdeDelim *pDelim,
    /* [retval][out] */ VARIANT_BOOL *pRetVal);


void __RPC_STUB IProcess_SQLToDelim_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IProcess_INTERFACE_DEFINED__ */



#ifndef __DTRANLib_LIBRARY_DEFINED__
#define __DTRANLib_LIBRARY_DEFINED__

/* library DTRANLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DTRANLib;

EXTERN_C const CLSID CLSID_FieldInfo;

#ifdef __cplusplus

class DECLSPEC_UUID("6259AC5E-86D3-493B-A219-FE93E4C924BC")
FieldInfo;
#endif

EXTERN_C const CLSID CLSID_Format;

#ifdef __cplusplus

class DECLSPEC_UUID("39908C62-33A1-4438-99D5-580979DE776B")
Format;
#endif

EXTERN_C const CLSID CLSID_deSQL;

#ifdef __cplusplus

class DECLSPEC_UUID("1AD87362-7E27-4161-A019-F2515B843D52")
deSQL;
#endif

EXTERN_C const CLSID CLSID_deFixed;

#ifdef __cplusplus

class DECLSPEC_UUID("DFC263FC-0B43-4E9A-95CA-8EDCE8F364C8")
deFixed;
#endif

EXTERN_C const CLSID CLSID_Process;

#ifdef __cplusplus

class DECLSPEC_UUID("A6D65811-5FBE-4B80-8D81-966046F7341C")
Process;
#endif

EXTERN_C const CLSID CLSID_deDelim;

#ifdef __cplusplus

class DECLSPEC_UUID("E600509E-7519-4ACB-BF32-FF9A4C2280FC")
deDelim;
#endif

EXTERN_C const CLSID CLSID_deXBase;

#ifdef __cplusplus

class DECLSPEC_UUID("C65415AF-3504-41C8-9A5E-A8EF72041E76")
deXBase;
#endif
#endif /* __DTRANLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


