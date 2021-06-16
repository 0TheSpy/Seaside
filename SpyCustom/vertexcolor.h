#ifndef VERTEXCOLOR_H
#define VERTEXCOLOR_H

#ifdef COMPILER_MSVC
#pragma once
#endif

#include "platform.h"


struct VertexColor_t
{
	VertexColor_t() {};
	VertexColor_t(const VertexColor_t& src);
	VertexColor_t(uint8 ir, uint8 ig, uint8 ib, uint8 ia);

	uint32 AsUint32() const;
	uint32* AsUint32Ptr();
	const uint32* AsUint32Ptr() const;

	VertexColor_t& operator=(const VertexColor_t& src);
	VertexColor_t& operator=(const color32& src);

	bool operator==(const VertexColor_t& src) const;
	bool operator!=(const VertexColor_t& src) const;

#ifdef PLATFORM_X360
	uint8 a, b, g, r;
#else
	uint8 r, g, b, a;
#endif
};


inline VertexColor_t::VertexColor_t(const VertexColor_t& src)
{
	*AsUint32Ptr() = src.AsUint32();
}

inline VertexColor_t::VertexColor_t(uint8 ir, uint8 ig, uint8 ib, uint8 ia) : r(ir), g(ig), b(ib), a(ia)
{
}


inline uint32 VertexColor_t::AsUint32() const
{
	return *reinterpret_cast<const uint32*>(this);
}

inline uint32* VertexColor_t::AsUint32Ptr()
{
	return reinterpret_cast<uint32*>(this);
}

inline const uint32* VertexColor_t::AsUint32Ptr() const
{
	return reinterpret_cast<const uint32*>(this);
}


inline VertexColor_t& VertexColor_t::operator=(const VertexColor_t& src)
{
	*AsUint32Ptr() = src.AsUint32();
	return *this;
}

inline VertexColor_t& VertexColor_t::operator=(const color32& src)
{
	r = src.r;
	g = src.g;
	b = src.b;
	a = src.a;
	return *this;
}


inline bool VertexColor_t::operator==(const VertexColor_t& src) const
{
	return AsUint32() == src.AsUint32();
}

inline bool VertexColor_t::operator!=(const VertexColor_t& src) const
{
	return AsUint32() != src.AsUint32();
}



#endif  