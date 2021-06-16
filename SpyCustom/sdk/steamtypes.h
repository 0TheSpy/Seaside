#ifndef STEAMTYPES_H
#define STEAMTYPES_H
#ifdef _WIN32
#pragma once
#endif

#define S_CALLTYPE __cdecl

#ifndef WCHARTYPES_H
typedef unsigned char uint8;
#endif

#if defined( __GNUC__ ) && !defined(POSIX)
#if __GNUC__ < 4
#error "Steamworks requires GCC 4.X (4.2 or 4.4 have been tested)"
#endif
#define POSIX 1
#endif

#if defined(__x86_64__) || defined(_WIN64)
#define X64BITS
#endif

#if !defined(VALVE_BIG_ENDIAN) && defined(_PS3)
#define VALVE_BIG_ENDIAN
#endif

typedef unsigned char uint8;
typedef signed char int8;

#if defined( _WIN32 )

typedef __int16 int16;
typedef unsigned __int16 uint16;
typedef __int32 int32;
typedef unsigned __int32 uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;

typedef int64 lint64;
typedef uint64 ulint64;

#ifdef X64BITS
typedef __int64 intp;				         
typedef unsigned __int64 uintp;		         
#else
typedef __int32 intp;
typedef unsigned __int32 uintp;
#endif

#else  

typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;

typedef long int lint64;
typedef unsigned long int ulint64;

#ifdef X64BITS
typedef long long intp;
typedef unsigned long long uintp;
#else
typedef int intp;
typedef unsigned int uintp;
#endif

#endif   

#ifdef __clang__
# define CLANG_ATTR(ATTR) __attribute__((annotate( ATTR )))
#else
# define CLANG_ATTR(ATTR)
#endif

#define METHOD_DESC(DESC) CLANG_ATTR( "desc:" #DESC ";" )
#define IGNOREATTR() CLANG_ATTR( "ignore" )
#define OUT_STRUCT() CLANG_ATTR( "out_struct: ;" )
#define OUT_ARRAY_CALL(COUNTER,FUNCTION,PARAMS) CLANG_ATTR( "out_array_call:" #COUNTER "," #FUNCTION "," #PARAMS ";" )
#define OUT_ARRAY_COUNT(COUNTER, DESC) CLANG_ATTR( "out_array_count:" #COUNTER  ";desc:" #DESC )
#define ARRAY_COUNT(COUNTER) CLANG_ATTR( "array_count:" #COUNTER ";" )
#define ARRAY_COUNT_D(COUNTER, DESC) CLANG_ATTR( "array_count:" #COUNTER ";desc:" #DESC )
#define BUFFER_COUNT(COUNTER) CLANG_ATTR( "buffer_count:" #COUNTER ";" )
#define OUT_BUFFER_COUNT(COUNTER) CLANG_ATTR( "out_buffer_count:" #COUNTER ";" )
#define OUT_STRING_COUNT(COUNTER) CLANG_ATTR( "out_string_count:" #COUNTER ";" )
#define DESC(DESC) CLANG_ATTR("desc:" #DESC ";")


const int k_cubSaltSize = 8;
typedef	uint8 Salt_t[k_cubSaltSize];

typedef uint64 GID_t;

const GID_t k_GIDNil = 0xffffffffffffffffull;

typedef uint64 JobID_t;			      
typedef GID_t TxnID_t;			       

const GID_t k_TxnIDNil = k_GIDNil;
const GID_t k_TxnIDUnknown = 0;

const JobID_t k_JobIDNil = 0xffffffffffffffffull;

typedef uint32 PackageId_t;
const PackageId_t k_uPackageIdFreeSub = 0x0;
const PackageId_t k_uPackageIdInvalid = 0xFFFFFFFF;

typedef uint32 BundleId_t;
const BundleId_t k_uBundleIdInvalid = 0;

typedef uint32 AppId_t;
const AppId_t k_uAppIdInvalid = 0x0;

typedef uint64 AssetClassId_t;
const AssetClassId_t k_ulAssetClassIdInvalid = 0x0;

typedef uint32 PhysicalItemId_t;
const PhysicalItemId_t k_uPhysicalItemIdInvalid = 0x0;


typedef uint32 DepotId_t;
const DepotId_t k_uDepotIdInvalid = 0x0;

typedef uint32 RTime32;

typedef uint32 CellID_t;
const CellID_t k_uCellIDInvalid = 0xFFFFFFFF;

typedef uint64 SteamAPICall_t;
const SteamAPICall_t k_uAPICallInvalid = 0x0;

typedef uint32 AccountID_t;

typedef uint32 PartnerId_t;
const PartnerId_t k_uPartnerIdInvalid = 0;

typedef uint64 ManifestId_t;
const ManifestId_t k_uManifestIdInvalid = 0;



#endif  