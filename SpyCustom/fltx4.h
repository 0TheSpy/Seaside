#ifndef FLTX4_H
#define FLTX4_H

#if defined(GNUC)
#define USE_STDC_FOR_SIMD 0
#else
#define USE_STDC_FOR_SIMD 0
#endif

#if (!defined(PLATFORM_PPC) && (USE_STDC_FOR_SIMD == 0))
#define _SSE1 1
#endif

#if USE_STDC_FOR_SIMD
#error "hello"
typedef union
{
	float  m128_f32[4];
	uint32 m128_u32[4];
} fltx4;

typedef fltx4 i32x4;
typedef fltx4 u32x4;

#ifdef _PS3
typedef fltx4 u32x4;
typedef fltx4 i32x4;
#endif
typedef fltx4 bi32x4;

#elif ( defined( _PS3 ) )

typedef union
{
	vec_float4	vmxf;
	vec_int4	vmxi;
	vec_uint4	vmxui;
#if defined(__SPU__)
	vec_uint4	vmxbi;
#else
	__vector bool vmxbi;
#endif

	struct
	{
		float x;
		float y;
		float z;
		float w;
	};

	float		m128_f32[4];
	uint32		m128_u32[4];
	int32		m128_i32[4];

} fltx4_union;

typedef vec_float4 fltx4;
typedef vec_uint4  u32x4;
typedef vec_int4   i32x4;

#if defined(__SPU__)
typedef vec_uint4 bi32x4;
#else
typedef __vector bool bi32x4;
#endif

#define DIFFERENT_NATIVE_VECTOR_TYPES             

#elif ( defined( _X360 ) )

typedef union
{
	__vector4	vmx;
	float		m128_f32[4];
	uint32		m128_u32[4];
} fltx4_union;

typedef __vector4 fltx4;
typedef __vector4 i32x4;                
typedef __vector4 u32x4;                 
typedef fltx4 bi32x4;
#else

typedef __m128 fltx4;
typedef __m128 i32x4;
typedef __m128 u32x4;
typedef __m128i shortx8;
typedef fltx4 bi32x4;

#endif

#endif