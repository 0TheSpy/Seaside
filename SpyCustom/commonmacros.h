#ifndef COMMONMACROS_H
#define COMMONMACROS_H

#ifdef _WIN32
#pragma once
#endif

#define MAKEID(d,c,b,a)					( ((int)(a) << 24) | ((int)(b) << 16) | ((int)(c) << 8) | ((int)(d)) )

#define STRING_MATCHES_ID( p, id )		( (*((int *)(p)) == (id) ) ? true : false )
#define ID_TO_STRING( id, p )			( (p)[3] = (((id)>>24) & 0xFF), (p)[2] = (((id)>>16) & 0xFF), (p)[1] = (((id)>>8) & 0xFF), (p)[0] = (((id)>>0) & 0xFF) )

#define SETBITS(iBitVector, bits)		((iBitVector) |= (bits))
#define CLEARBITS(iBitVector, bits)		((iBitVector) &= ~(bits))
#define FBitSet(iBitVector, bits)		((iBitVector) & (bits))

template <typename T>
inline bool IsPowerOfTwo(T value)
{
	return (value & (value - (T)1)) == (T)0;
}

#ifndef REFERENCE
#define REFERENCE(arg) ((void)arg)
#endif

#define CONST_INTEGER_AS_STRING(x) #x             
#define __HACK_LINE_AS_STRING__(x) CONST_INTEGER_AS_STRING(x)                   
#define __LINE__AS_STRING __HACK_LINE_AS_STRING__(__LINE__)         

#ifdef ARRAYSIZE
#undef ARRAYSIZE
#endif

#define RTL_NUMBER_OF_V1(A) (sizeof(A)/sizeof((A)[0]))

#if defined(__cplusplus) && \
    !defined(MIDL_PASS) && \
    !defined(RC_INVOKED) && \
    (_MSC_FULL_VER >= 13009466) && \
    !defined(SORTPP_PASS)

#if !defined(UNALIGNED)
#if defined(_M_IA64) || defined(_M_AMD64)
#define UNALIGNED __unaligned
#else
#define UNALIGNED
#endif
#endif

extern "C++"         
template <typename T, size_t N>
char(*RtlpNumberOf(UNALIGNED T(&)[N]))[N];

#ifdef _PREFAST_
#define RTL_NUMBER_OF_V2(A) (sizeof(*RtlpNumberOf(A))+0)
#else
#define RTL_NUMBER_OF_V2(A) (sizeof(*RtlpNumberOf(A)))
#endif

#else
#define RTL_NUMBER_OF_V2(A) RTL_NUMBER_OF_V1(A)
#endif

#define ARRAYSIZE(A)    RTL_NUMBER_OF_V2(A)
#define _ARRAYSIZE(A)   RTL_NUMBER_OF_V1(A)

#define Q_ARRAYSIZE(p)		ARRAYSIZE(p)

template< typename IndexType, typename T, unsigned int N >
IndexType ClampedArrayIndex(const T(&buffer)[N], IndexType index)
{
	NOTE_UNUSED(buffer);
	return clamp(index, 0, (IndexType)N - 1);
}

template< typename T, unsigned int N >
T ClampedArrayElement(const T(&buffer)[N], unsigned int uIndex)
{
	if (uIndex >= N)
		uIndex = N - 1;
	return buffer[uIndex];
}

#endif  