

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


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



#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        EXTERN_C __declspec(selectany) const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif // !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_海鱼,0x9cf2e7e9,0xbd5a,0x4c2b,0x83,0xa5,0x1f,0xc0,0xf0,0xee,0x2a,0xc3);


MIDL_DEFINE_GUID(IID, DIID_IMy,0x7656fa3a,0x4221,0x460a,0x8f,0x2c,0xc2,0x29,0x1c,0xb2,0xce,0x1b);


MIDL_DEFINE_GUID(CLSID, CLSID_My,0x7dd0adaa,0xc857,0x4017,0x8a,0x73,0xa9,0xd3,0x7c,0xb8,0x57,0x7c);


MIDL_DEFINE_GUID(IID, DIID_I窗口_交换界面,0x5d89d068,0xfbf4,0x41d2,0x90,0xf0,0x3a,0x04,0xdc,0x89,0xbd,0x47);


MIDL_DEFINE_GUID(CLSID, CLSID_窗口_交换界面,0x8cb89fb1,0x70c0,0x47b8,0x95,0x44,0x45,0x84,0x09,0xef,0xe9,0xc9);


MIDL_DEFINE_GUID(IID, DIID_I窗口_交互界面,0x34fd6fe4,0xf559,0x4973,0xae,0xb5,0x15,0x64,0x8d,0xc9,0x63,0x96);


MIDL_DEFINE_GUID(CLSID, CLSID_窗口_交互界面,0xc41dd274,0xddbe,0x46db,0x9a,0xd5,0x54,0xd2,0x82,0xfc,0x11,0xf4);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



