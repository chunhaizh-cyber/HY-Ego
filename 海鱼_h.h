

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for My.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0628 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef ___BAA3D3E3_h_h__
#define ___BAA3D3E3_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if defined(_CONTROL_FLOW_GUARD_XFG)
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

#ifndef __IMy_FWD_DEFINED__
#define __IMy_FWD_DEFINED__
typedef interface IMy IMy;

#endif 	/* __IMy_FWD_DEFINED__ */


#ifndef __My_FWD_DEFINED__
#define __My_FWD_DEFINED__

#ifdef __cplusplus
typedef class My My;
#else
typedef struct My My;
#endif /* __cplusplus */

#endif 	/* __My_FWD_DEFINED__ */


#ifndef __I窗口_交换界面_FWD_DEFINED__
#define __I窗口_交换界面_FWD_DEFINED__
typedef interface I窗口_交换界面 I窗口_交换界面;

#endif 	/* __I窗口_交换界面_FWD_DEFINED__ */


#ifndef __窗口_交换界面_FWD_DEFINED__
#define __窗口_交换界面_FWD_DEFINED__

#ifdef __cplusplus
typedef class 窗口_交换界面 窗口_交换界面;
#else
typedef struct 窗口_交换界面 窗口_交换界面;
#endif /* __cplusplus */

#endif 	/* __窗口_交换界面_FWD_DEFINED__ */


#ifndef __I窗口_交互界面_FWD_DEFINED__
#define __I窗口_交互界面_FWD_DEFINED__
typedef interface I窗口_交互界面 I窗口_交互界面;

#endif 	/* __I窗口_交互界面_FWD_DEFINED__ */


#ifndef __窗口_交互界面_FWD_DEFINED__
#define __窗口_交互界面_FWD_DEFINED__

#ifdef __cplusplus
typedef class 窗口_交互界面 窗口_交互界面;
#else
typedef struct 窗口_交互界面 窗口_交互界面;
#endif /* __cplusplus */

#endif 	/* __窗口_交互界面_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __海鱼_LIBRARY_DEFINED__
#define __海鱼_LIBRARY_DEFINED__

/* library 海鱼 */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_海鱼;

#ifndef __IMy_DISPINTERFACE_DEFINED__
#define __IMy_DISPINTERFACE_DEFINED__

/* dispinterface IMy */
/* [uuid] */ 


EXTERN_C const IID DIID_IMy;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("7656fa3a-4221-460a-8f2c-c2291cb2ce1b")
    IMy : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IMyVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMy * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMy * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMy * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMy * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMy * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMy * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMy * This,
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
        
        END_INTERFACE
    } IMyVtbl;

    interface IMy
    {
        CONST_VTBL struct IMyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMy_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMy_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMy_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMy_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMy_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMy_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMy_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IMy_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_My;

#ifdef __cplusplus

class DECLSPEC_UUID("7dd0adaa-c857-4017-8a73-a9d37cb8577c")
My;
#endif

#ifndef __I窗口_交换界面_DISPINTERFACE_DEFINED__
#define __I窗口_交换界面_DISPINTERFACE_DEFINED__

/* dispinterface I窗口_交换界面 */
/* [uuid] */ 


EXTERN_C const IID DIID_I窗口_交换界面;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("5d89d068-fbf4-41d2-90f0-3a04dc89bd47")
    I窗口_交换界面 : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct I窗口_交换界面Vtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            I窗口_交换界面 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            I窗口_交换界面 * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            I窗口_交换界面 * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            I窗口_交换界面 * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            I窗口_交换界面 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            I窗口_交换界面 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            I窗口_交换界面 * This,
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
        
        END_INTERFACE
    } I窗口_交换界面Vtbl;

    interface I窗口_交换界面
    {
        CONST_VTBL struct I窗口_交换界面Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define I窗口_交换界面_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define I窗口_交换界面_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define I窗口_交换界面_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define I窗口_交换界面_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define I窗口_交换界面_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define I窗口_交换界面_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define I窗口_交换界面_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __I窗口_交换界面_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_窗口_交换界面;

#ifdef __cplusplus

class DECLSPEC_UUID("8cb89fb1-70c0-47b8-9544-458409efe9c9")
窗口_交换界面;
#endif

#ifndef __I窗口_交互界面_DISPINTERFACE_DEFINED__
#define __I窗口_交互界面_DISPINTERFACE_DEFINED__

/* dispinterface I窗口_交互界面 */
/* [uuid] */ 


EXTERN_C const IID DIID_I窗口_交互界面;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("34fd6fe4-f559-4973-aeb5-15648dc96396")
    I窗口_交互界面 : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct I窗口_交互界面Vtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            I窗口_交互界面 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            I窗口_交互界面 * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            I窗口_交互界面 * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            I窗口_交互界面 * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            I窗口_交互界面 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            I窗口_交互界面 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            I窗口_交互界面 * This,
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
        
        END_INTERFACE
    } I窗口_交互界面Vtbl;

    interface I窗口_交互界面
    {
        CONST_VTBL struct I窗口_交互界面Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define I窗口_交互界面_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define I窗口_交互界面_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define I窗口_交互界面_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define I窗口_交互界面_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define I窗口_交互界面_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define I窗口_交互界面_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define I窗口_交互界面_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __I窗口_交互界面_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_窗口_交互界面;

#ifdef __cplusplus

class DECLSPEC_UUID("c41dd274-ddbe-46db-9ad5-54d282fc11f4")
窗口_交互界面;
#endif
#endif /* __海鱼_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


