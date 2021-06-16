#ifndef _MATH_PFNS_H_
#define _MATH_PFNS_H_

#if defined( _X360 )
#include <xboxmath.h>
#endif

#if !defined( _X360 )

extern float (*pfSqrt)(float x);
extern float (*pfRSqrt)(float x);
extern float (*pfRSqrtFast)(float x);
extern void  (*pfFastSinCos)(float x, float* s, float* c);
extern float (*pfFastCos)(float x);

#define FastSqrt(x)			(*pfSqrt)(x)
#define	FastRSqrt(x)		(*pfRSqrt)(x)
#define FastRSqrtFast(x)    (*pfRSqrtFast)(x)
#define FastSinCos(x,s,c)   (*pfFastSinCos)(x,s,c)
#define FastCos(x)			(*pfFastCos)(x)

#if defined(__i386__) || defined(_M_IX86)
#undef FastSqrt
#define FastSqrt(x)			::sqrtf(x)
#endif

#endif  

#if defined( _X360 )

FORCEINLINE float _VMX_Sqrt(float x)
{
	return __fsqrts(x);
}

FORCEINLINE float _VMX_RSqrt(float x)
{
	float rroot = __frsqrte(x);

	return (0.5f * rroot) * (3.0f - (x * rroot) * rroot);
}

FORCEINLINE float _VMX_RSqrtFast(float x)
{
	return __frsqrte(x);
}

FORCEINLINE void _VMX_SinCos(float a, float* pS, float* pC)
{
	XMScalarSinCos(pS, pC, a);
}

FORCEINLINE float _VMX_Cos(float a)
{
	return XMScalarCos(a);
}

#define FastSqrt(x)			_VMX_Sqrt(x)
#define	FastRSqrt(x)		_VMX_RSqrt(x)
#define FastRSqrtFast(x)	_VMX_RSqrtFast(x)
#define FastSinCos(x,s,c)	_VMX_SinCos(x,s,c)
#define FastCos(x)			_VMX_Cos(x)

#endif  

#endif  