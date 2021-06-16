#ifndef ANALYSIS_ANNOTATIONS_H
#define ANALYSIS_ANNOTATIONS_H

#if _MSC_VER >= 1600     
#pragma warning(error : 4789 )          

#ifdef _PREFAST_
#include <sal.h>

#define ANALYZE_SUPPRESS(wnum) __pragma(warning(suppress: wnum))
#define ANALYZE_SUPPRESS2(wnum1, wnum2) __pragma(warning(supress: wnum1  wnum2))
#define ANALYZE_SUPPRESS3(wnum1, wnum2, wnum3) __pragma(warning(suppress: wnum1 wnum2 wnum3))
#define ANALYZE_SUPPRESS4(wnum1, wnum2, wnum3, wnum4) __pragma(warning(suppress: wnum1 wnum2 wnum3 wnum4))

#define PRINTF_FORMAT_STRING _Printf_format_string_
#define SCANF_FORMAT_STRING _Scanf_format_string_impl_
#define IN_Z _In_z_
#define IN_CAP(x) _In_count_(x)
#define IN_BYTECAP(x) _In_bytecount_(x)
#define OUT_Z_CAP(x) _Out_z_cap_(x)
#define OUT_CAP(x) _Out_cap_(x)
#define OUT_CAP_C(x) _Out_cap_c_(x)         
#define OUT_BYTECAP(x) _Out_bytecap_(x)
#define OUT_Z_BYTECAP(x) _Out_z_bytecap_(x)
#define INOUT_BYTECAP(x) _Inout_bytecap_(x)
#define INOUT_Z_CAP(x) _Inout_z_cap_(x)
#define INOUT_Z_BYTECAP(x) _Inout_z_bytecap_(x)
#if _MSC_VER >= 1700
#define IN_Z_ARRAY _Pre_z_
#define OUT_Z_ARRAY _Post_z_
#define INOUT_Z_ARRAY _Prepost_z_
#else
#define IN_Z_ARRAY _Deref_pre_z_
#define OUT_Z_ARRAY _Deref_post_z_
#define INOUT_Z_ARRAY _Deref_prepost_z_
#endif    
#define MUST_CHECK_RETURN _Check_return_
#endif  
#endif         

#ifndef ANALYZE_SUPPRESS
#define ANALYZE_SUPPRESS(wnum)
#define ANALYZE_SUPPRESS2(wnum1, wnum2)
#define ANALYZE_SUPPRESS3(wnum1, wnum2, wnum3)
#define ANALYZE_SUPPRESS4(wnum1, wnum2, wnum3, wnum4)
#define PRINTF_FORMAT_STRING
#define SCANF_FORMAT_STRING
#define IN_Z
#define IN_CAP(x)
#define IN_BYTECAP(x)
#define OUT_Z_CAP(x)
#define OUT_CAP(x)
#define OUT_CAP_C(x)
#define OUT_BYTECAP(x)
#define OUT_Z_BYTECAP(x)
#define INOUT_BYTECAP(x)
#define INOUT_Z_CAP(x)
#define INOUT_Z_BYTECAP(x)
#define OUT_Z_ARRAY
#define INOUT_Z_ARRAY
#define MUST_CHECK_RETURN
#endif

#endif  