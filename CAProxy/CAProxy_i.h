

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Tue Mar 14 03:09:05 2017
 */
/* Compiler settings for CAProxy.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

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

#ifndef __CAProxy_i_h__
#define __CAProxy_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IKeyClient_FWD_DEFINED__
#define __IKeyClient_FWD_DEFINED__
typedef interface IKeyClient IKeyClient;

#endif 	/* __IKeyClient_FWD_DEFINED__ */


#ifndef __KeyClient_FWD_DEFINED__
#define __KeyClient_FWD_DEFINED__

#ifdef __cplusplus
typedef class KeyClient KeyClient;
#else
typedef struct KeyClient KeyClient;
#endif /* __cplusplus */

#endif 	/* __KeyClient_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IKeyClient_INTERFACE_DEFINED__
#define __IKeyClient_INTERFACE_DEFINED__

/* interface IKeyClient */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IKeyClient;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F13C3022-B19D-49D4-9282-881078586642")
    IKeyClient : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE help( 
            /* [retval][out] */ BSTR *v) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_GetVersion( 
            /* [retval][out] */ LONG *puiVersion) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_SetSignMethod( 
            /* [in] */ LONG SignMethod) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_GetSignMethod( 
            /* [retval][out] */ LONG *SignMethod) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_SetEncryptMethod( 
            /* [in] */ LONG EncryptMethod) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_GetUserList( 
            /* [retval][out] */ BSTR *userList) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_ExportUserCert( 
            /* [in] */ BSTR CertID,
            /* [retval][out] */ BSTR *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_Login( 
            /* [in] */ BSTR CertID,
            /* [in] */ BSTR PassWd,
            /* [retval][out] */ LONG *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_ChangePassWd( 
            /* [in] */ BSTR CertID,
            /* [in] */ BSTR OldPassWd,
            /* [in] */ BSTR NewPassWd,
            /* [retval][out] */ BOOL *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_ExportExChangeUserCert( 
            /* [in] */ BSTR CertID,
            /* [retval][out] */ BSTR *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_GetCertInfo( 
            /* [in] */ BSTR Cert,
            /* [in] */ LONG Type,
            /* [retval][out] */ BSTR *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_GetCertInfoByOid( 
            /* [in] */ BSTR Cert,
            /* [in] */ BSTR Oid,
            /* [retval][out] */ BSTR *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_GetUserInfo( 
            /* [in] */ BSTR Cert,
            /* [in] */ LONG Type,
            /* [retval][out] */ BSTR *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_ValidateCert( 
            /* [in] */ BSTR Cert,
            /* [retval][out] */ BOOL *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_SignData( 
            /* [in] */ BSTR Cert,
            /* [in] */ BSTR InData,
            /* [retval][out] */ BSTR *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_VerifySignedData( 
            /* [in] */ BSTR Cert,
            /* [in] */ BSTR InData,
            /* [in] */ BSTR SignValue,
            /* [retval][out] */ BOOL *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_SignFile( 
            /* [in] */ BSTR Cert,
            /* [in] */ BSTR InFile,
            /* [retval][out] */ BSTR *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_VerifySignedFile( 
            /* [in] */ BSTR Cert,
            /* [in] */ BSTR InFile,
            /* [in] */ BSTR SignValue,
            /* [retval][out] */ BOOL *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_EncryptData( 
            /* [in] */ BSTR SymmKey,
            /* [in] */ BSTR Indata,
            /* [retval][out] */ BSTR *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_DecryptData( 
            /* [in] */ BSTR SymmKey,
            /* [in] */ BSTR Indata,
            /* [retval][out] */ BSTR *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_EncryptFile( 
            /* [in] */ BSTR SymmKey,
            /* [in] */ BSTR InFile,
            /* [in] */ BSTR OutFile,
            /* [retval][out] */ BOOL *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_DecryptFile( 
            /* [in] */ BSTR SymmKey,
            /* [in] */ BSTR InFile,
            /* [in] */ BSTR OutFile,
            /* [retval][out] */ BOOL *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_PubKeyEncrypt( 
            /* [in] */ BSTR Cert,
            /* [in] */ BSTR InData,
            /* [retval][out] */ BSTR *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_PriKeyDecrypt( 
            /* [in] */ BSTR CertID,
            /* [in] */ BSTR InData,
            /* [retval][out] */ BSTR *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_SignDataByP7( 
            /* [in] */ BSTR CertID,
            /* [in] */ BSTR InData,
            /* [retval][out] */ BSTR *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_VerifySignedDataByP7( 
            /* [in] */ BSTR P7Data,
            /* [retval][out] */ BOOL *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_GetP7SignDataInfo( 
            /* [in] */ BSTR P7Data,
            /* [in] */ LONG type,
            /* [retval][out] */ BSTR *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_SignDataXML( 
            /* [in] */ BSTR CertID,
            /* [in] */ BSTR InData,
            /* [retval][out] */ BSTR *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_VerifySignedDataXML( 
            /* [in] */ BSTR InData,
            /* [retval][out] */ BOOL *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_GetXMLSignatureInfo( 
            /* [in] */ BSTR XMLSignedData,
            /* [in] */ LONG type,
            /* [retval][out] */ BSTR *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_CheckSupport( 
            /* [retval][out] */ LONG *rv) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SOF_GenRandom( 
            LONG len,
            /* [retval][out] */ BSTR *rv) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IKeyClientVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKeyClient * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKeyClient * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKeyClient * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IKeyClient * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IKeyClient * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IKeyClient * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IKeyClient * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *help )( 
            IKeyClient * This,
            /* [retval][out] */ BSTR *v);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_GetVersion )( 
            IKeyClient * This,
            /* [retval][out] */ LONG *puiVersion);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_SetSignMethod )( 
            IKeyClient * This,
            /* [in] */ LONG SignMethod);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_GetSignMethod )( 
            IKeyClient * This,
            /* [retval][out] */ LONG *SignMethod);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_SetEncryptMethod )( 
            IKeyClient * This,
            /* [in] */ LONG EncryptMethod);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_GetUserList )( 
            IKeyClient * This,
            /* [retval][out] */ BSTR *userList);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_ExportUserCert )( 
            IKeyClient * This,
            /* [in] */ BSTR CertID,
            /* [retval][out] */ BSTR *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_Login )( 
            IKeyClient * This,
            /* [in] */ BSTR CertID,
            /* [in] */ BSTR PassWd,
            /* [retval][out] */ LONG *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_ChangePassWd )( 
            IKeyClient * This,
            /* [in] */ BSTR CertID,
            /* [in] */ BSTR OldPassWd,
            /* [in] */ BSTR NewPassWd,
            /* [retval][out] */ BOOL *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_ExportExChangeUserCert )( 
            IKeyClient * This,
            /* [in] */ BSTR CertID,
            /* [retval][out] */ BSTR *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_GetCertInfo )( 
            IKeyClient * This,
            /* [in] */ BSTR Cert,
            /* [in] */ LONG Type,
            /* [retval][out] */ BSTR *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_GetCertInfoByOid )( 
            IKeyClient * This,
            /* [in] */ BSTR Cert,
            /* [in] */ BSTR Oid,
            /* [retval][out] */ BSTR *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_GetUserInfo )( 
            IKeyClient * This,
            /* [in] */ BSTR Cert,
            /* [in] */ LONG Type,
            /* [retval][out] */ BSTR *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_ValidateCert )( 
            IKeyClient * This,
            /* [in] */ BSTR Cert,
            /* [retval][out] */ BOOL *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_SignData )( 
            IKeyClient * This,
            /* [in] */ BSTR Cert,
            /* [in] */ BSTR InData,
            /* [retval][out] */ BSTR *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_VerifySignedData )( 
            IKeyClient * This,
            /* [in] */ BSTR Cert,
            /* [in] */ BSTR InData,
            /* [in] */ BSTR SignValue,
            /* [retval][out] */ BOOL *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_SignFile )( 
            IKeyClient * This,
            /* [in] */ BSTR Cert,
            /* [in] */ BSTR InFile,
            /* [retval][out] */ BSTR *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_VerifySignedFile )( 
            IKeyClient * This,
            /* [in] */ BSTR Cert,
            /* [in] */ BSTR InFile,
            /* [in] */ BSTR SignValue,
            /* [retval][out] */ BOOL *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_EncryptData )( 
            IKeyClient * This,
            /* [in] */ BSTR SymmKey,
            /* [in] */ BSTR Indata,
            /* [retval][out] */ BSTR *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_DecryptData )( 
            IKeyClient * This,
            /* [in] */ BSTR SymmKey,
            /* [in] */ BSTR Indata,
            /* [retval][out] */ BSTR *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_EncryptFile )( 
            IKeyClient * This,
            /* [in] */ BSTR SymmKey,
            /* [in] */ BSTR InFile,
            /* [in] */ BSTR OutFile,
            /* [retval][out] */ BOOL *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_DecryptFile )( 
            IKeyClient * This,
            /* [in] */ BSTR SymmKey,
            /* [in] */ BSTR InFile,
            /* [in] */ BSTR OutFile,
            /* [retval][out] */ BOOL *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_PubKeyEncrypt )( 
            IKeyClient * This,
            /* [in] */ BSTR Cert,
            /* [in] */ BSTR InData,
            /* [retval][out] */ BSTR *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_PriKeyDecrypt )( 
            IKeyClient * This,
            /* [in] */ BSTR CertID,
            /* [in] */ BSTR InData,
            /* [retval][out] */ BSTR *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_SignDataByP7 )( 
            IKeyClient * This,
            /* [in] */ BSTR CertID,
            /* [in] */ BSTR InData,
            /* [retval][out] */ BSTR *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_VerifySignedDataByP7 )( 
            IKeyClient * This,
            /* [in] */ BSTR P7Data,
            /* [retval][out] */ BOOL *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_GetP7SignDataInfo )( 
            IKeyClient * This,
            /* [in] */ BSTR P7Data,
            /* [in] */ LONG type,
            /* [retval][out] */ BSTR *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_SignDataXML )( 
            IKeyClient * This,
            /* [in] */ BSTR CertID,
            /* [in] */ BSTR InData,
            /* [retval][out] */ BSTR *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_VerifySignedDataXML )( 
            IKeyClient * This,
            /* [in] */ BSTR InData,
            /* [retval][out] */ BOOL *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_GetXMLSignatureInfo )( 
            IKeyClient * This,
            /* [in] */ BSTR XMLSignedData,
            /* [in] */ LONG type,
            /* [retval][out] */ BSTR *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_CheckSupport )( 
            IKeyClient * This,
            /* [retval][out] */ LONG *rv);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SOF_GenRandom )( 
            IKeyClient * This,
            LONG len,
            /* [retval][out] */ BSTR *rv);
        
        END_INTERFACE
    } IKeyClientVtbl;

    interface IKeyClient
    {
        CONST_VTBL struct IKeyClientVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKeyClient_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKeyClient_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKeyClient_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKeyClient_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IKeyClient_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IKeyClient_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IKeyClient_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IKeyClient_help(This,v)	\
    ( (This)->lpVtbl -> help(This,v) ) 

#define IKeyClient_SOF_GetVersion(This,puiVersion)	\
    ( (This)->lpVtbl -> SOF_GetVersion(This,puiVersion) ) 

#define IKeyClient_SOF_SetSignMethod(This,SignMethod)	\
    ( (This)->lpVtbl -> SOF_SetSignMethod(This,SignMethod) ) 

#define IKeyClient_SOF_GetSignMethod(This,SignMethod)	\
    ( (This)->lpVtbl -> SOF_GetSignMethod(This,SignMethod) ) 

#define IKeyClient_SOF_SetEncryptMethod(This,EncryptMethod)	\
    ( (This)->lpVtbl -> SOF_SetEncryptMethod(This,EncryptMethod) ) 

#define IKeyClient_SOF_GetUserList(This,userList)	\
    ( (This)->lpVtbl -> SOF_GetUserList(This,userList) ) 

#define IKeyClient_SOF_ExportUserCert(This,CertID,rv)	\
    ( (This)->lpVtbl -> SOF_ExportUserCert(This,CertID,rv) ) 

#define IKeyClient_SOF_Login(This,CertID,PassWd,rv)	\
    ( (This)->lpVtbl -> SOF_Login(This,CertID,PassWd,rv) ) 

#define IKeyClient_SOF_ChangePassWd(This,CertID,OldPassWd,NewPassWd,rv)	\
    ( (This)->lpVtbl -> SOF_ChangePassWd(This,CertID,OldPassWd,NewPassWd,rv) ) 

#define IKeyClient_SOF_ExportExChangeUserCert(This,CertID,rv)	\
    ( (This)->lpVtbl -> SOF_ExportExChangeUserCert(This,CertID,rv) ) 

#define IKeyClient_SOF_GetCertInfo(This,Cert,Type,rv)	\
    ( (This)->lpVtbl -> SOF_GetCertInfo(This,Cert,Type,rv) ) 

#define IKeyClient_SOF_GetCertInfoByOid(This,Cert,Oid,rv)	\
    ( (This)->lpVtbl -> SOF_GetCertInfoByOid(This,Cert,Oid,rv) ) 

#define IKeyClient_SOF_GetUserInfo(This,Cert,Type,rv)	\
    ( (This)->lpVtbl -> SOF_GetUserInfo(This,Cert,Type,rv) ) 

#define IKeyClient_SOF_ValidateCert(This,Cert,rv)	\
    ( (This)->lpVtbl -> SOF_ValidateCert(This,Cert,rv) ) 

#define IKeyClient_SOF_SignData(This,Cert,InData,rv)	\
    ( (This)->lpVtbl -> SOF_SignData(This,Cert,InData,rv) ) 

#define IKeyClient_SOF_VerifySignedData(This,Cert,InData,SignValue,rv)	\
    ( (This)->lpVtbl -> SOF_VerifySignedData(This,Cert,InData,SignValue,rv) ) 

#define IKeyClient_SOF_SignFile(This,Cert,InFile,rv)	\
    ( (This)->lpVtbl -> SOF_SignFile(This,Cert,InFile,rv) ) 

#define IKeyClient_SOF_VerifySignedFile(This,Cert,InFile,SignValue,rv)	\
    ( (This)->lpVtbl -> SOF_VerifySignedFile(This,Cert,InFile,SignValue,rv) ) 

#define IKeyClient_SOF_EncryptData(This,SymmKey,Indata,rv)	\
    ( (This)->lpVtbl -> SOF_EncryptData(This,SymmKey,Indata,rv) ) 

#define IKeyClient_SOF_DecryptData(This,SymmKey,Indata,rv)	\
    ( (This)->lpVtbl -> SOF_DecryptData(This,SymmKey,Indata,rv) ) 

#define IKeyClient_SOF_EncryptFile(This,SymmKey,InFile,OutFile,rv)	\
    ( (This)->lpVtbl -> SOF_EncryptFile(This,SymmKey,InFile,OutFile,rv) ) 

#define IKeyClient_SOF_DecryptFile(This,SymmKey,InFile,OutFile,rv)	\
    ( (This)->lpVtbl -> SOF_DecryptFile(This,SymmKey,InFile,OutFile,rv) ) 

#define IKeyClient_SOF_PubKeyEncrypt(This,Cert,InData,rv)	\
    ( (This)->lpVtbl -> SOF_PubKeyEncrypt(This,Cert,InData,rv) ) 

#define IKeyClient_SOF_PriKeyDecrypt(This,CertID,InData,rv)	\
    ( (This)->lpVtbl -> SOF_PriKeyDecrypt(This,CertID,InData,rv) ) 

#define IKeyClient_SOF_SignDataByP7(This,CertID,InData,rv)	\
    ( (This)->lpVtbl -> SOF_SignDataByP7(This,CertID,InData,rv) ) 

#define IKeyClient_SOF_VerifySignedDataByP7(This,P7Data,rv)	\
    ( (This)->lpVtbl -> SOF_VerifySignedDataByP7(This,P7Data,rv) ) 

#define IKeyClient_SOF_GetP7SignDataInfo(This,P7Data,type,rv)	\
    ( (This)->lpVtbl -> SOF_GetP7SignDataInfo(This,P7Data,type,rv) ) 

#define IKeyClient_SOF_SignDataXML(This,CertID,InData,rv)	\
    ( (This)->lpVtbl -> SOF_SignDataXML(This,CertID,InData,rv) ) 

#define IKeyClient_SOF_VerifySignedDataXML(This,InData,rv)	\
    ( (This)->lpVtbl -> SOF_VerifySignedDataXML(This,InData,rv) ) 

#define IKeyClient_SOF_GetXMLSignatureInfo(This,XMLSignedData,type,rv)	\
    ( (This)->lpVtbl -> SOF_GetXMLSignatureInfo(This,XMLSignedData,type,rv) ) 

#define IKeyClient_SOF_CheckSupport(This,rv)	\
    ( (This)->lpVtbl -> SOF_CheckSupport(This,rv) ) 

#define IKeyClient_SOF_GenRandom(This,len,rv)	\
    ( (This)->lpVtbl -> SOF_GenRandom(This,len,rv) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IKeyClient_INTERFACE_DEFINED__ */



#ifndef __CAProxyLib_LIBRARY_DEFINED__
#define __CAProxyLib_LIBRARY_DEFINED__

/* library CAProxyLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_CAProxyLib;

EXTERN_C const CLSID CLSID_KeyClient;

#ifdef __cplusplus

class DECLSPEC_UUID("5788ACA3-28AE-402F-B952-8DB113F8396F")
KeyClient;
#endif
#endif /* __CAProxyLib_LIBRARY_DEFINED__ */

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


