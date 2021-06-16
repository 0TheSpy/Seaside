#ifndef VECTOR4D_H
#define VECTOR4D_H

#ifdef _WIN32
#pragma once
#endif

#include <math.h>
#include <stdlib.h>		       
#include <float.h>
#if !defined( _X360 )
#include <xmmintrin.h>	  
#endif
#include "basetypes.h"	      
#include "dbg.h"
#include "math_pfns.h"

class Vector;
class Vector2D;

class Vector4D
{
public:
	vec_t x, y, z, w;

	Vector4D(void);
	Vector4D(vec_t X, vec_t Y, vec_t Z, vec_t W);
	Vector4D(const float* pFloat);

	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f, vec_t iw = 0.0f);

	bool IsValid() const;

	vec_t operator[](int i) const;
	vec_t& operator[](int i);

	inline vec_t* Base();
	inline vec_t const* Base() const;

	Vector& AsVector3D();
	Vector const& AsVector3D() const;

	Vector2D& AsVector2D();
	Vector2D const& AsVector2D() const;

	void Random(vec_t minVal, vec_t maxVal);

	bool operator==(const Vector4D& v) const;
	bool operator!=(const Vector4D& v) const;

	Vector4D& operator+=(const Vector4D& v);
	Vector4D& operator-=(const Vector4D& v);
	Vector4D& operator*=(const Vector4D& v);
	Vector4D& operator*=(float s);
	Vector4D& operator/=(const Vector4D& v);
	Vector4D& operator/=(float s);

	void	Negate();

	vec_t	Length() const;

	vec_t	LengthSqr(void) const;

	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance&&
			y > -tolerance && y < tolerance&&
			z > -tolerance && z < tolerance&&
			w > -tolerance && w < tolerance);
	}

	vec_t	DistTo(const Vector4D& vOther) const;

	vec_t	DistToSqr(const Vector4D& vOther) const;

	void	CopyToArray(float* rgfl) const;

	void	MulAdd(Vector4D const& a, Vector4D const& b, float scalar);

	vec_t	Dot(Vector4D const& vOther) const;

#ifdef VECTOR_NO_SLOW_OPERATIONS
private:
#else
public:
#endif
	Vector4D(Vector4D const& vOther);

	Vector4D& operator=(Vector4D const& src);
};

const Vector4D vec4_origin(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4D vec4_invalid(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

class ALIGN16 Vector4DAligned : public Vector4D
{
public:
	Vector4DAligned(void) {}
	Vector4DAligned(vec_t X, vec_t Y, vec_t Z, vec_t W);

	inline void Set(vec_t X, vec_t Y, vec_t Z, vec_t W);
	inline void InitZero(void);

	inline __m128& AsM128() { return *(__m128*) & x; }
	inline const __m128& AsM128() const { return *(const __m128*) & x; }

private:
	Vector4DAligned(Vector4DAligned const& vOther);

	Vector4DAligned& operator=(Vector4DAligned const& src);
} ALIGN16_POST;

void Vector4DClear(Vector4D& a);

void Vector4DCopy(Vector4D const& src, Vector4D& dst);

void Vector4DAdd(Vector4D const& a, Vector4D const& b, Vector4D& result);
void Vector4DSubtract(Vector4D const& a, Vector4D const& b, Vector4D& result);
void Vector4DMultiply(Vector4D const& a, vec_t b, Vector4D& result);
void Vector4DMultiply(Vector4D const& a, Vector4D const& b, Vector4D& result);
void Vector4DDivide(Vector4D const& a, vec_t b, Vector4D& result);
void Vector4DDivide(Vector4D const& a, Vector4D const& b, Vector4D& result);
void Vector4DMA(Vector4D const& start, float s, Vector4D const& dir, Vector4D& result);

void Vector4DMultiplyAligned(Vector4DAligned const& a, vec_t b, Vector4DAligned& result);


#define Vector4DExpand( v ) (v).x, (v).y, (v).z, (v).w

vec_t Vector4DNormalize(Vector4D& v);

vec_t Vector4DLength(Vector4D const& v);

vec_t DotProduct4D(Vector4D const& a, Vector4D const& b);

void Vector4DLerp(Vector4D const& src1, Vector4D const& src2, vec_t t, Vector4D& dest);



inline Vector4D::Vector4D(void)
{
#ifdef _DEBUG
	x = y = z = w = VEC_T_NAN;
#endif
}

inline Vector4D::Vector4D(vec_t X, vec_t Y, vec_t Z, vec_t W)
{
	x = X; y = Y; z = Z; w = W;
	Assert(IsValid());
}

inline Vector4D::Vector4D(const float* pFloat)
{
	Assert(pFloat);
	x = pFloat[0]; y = pFloat[1]; z = pFloat[2]; w = pFloat[3];
	Assert(IsValid());
}


inline Vector4D::Vector4D(const Vector4D& vOther)
{
	Assert(vOther.IsValid());
	x = vOther.x; y = vOther.y; z = vOther.z; w = vOther.w;
}

inline void Vector4D::Init(vec_t ix, vec_t iy, vec_t iz, vec_t iw)
{
	x = ix; y = iy; z = iz;	w = iw;
	Assert(IsValid());
}

inline void Vector4D::Random(vec_t minVal, vec_t maxVal)
{
	x = minVal + ((vec_t)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
	y = minVal + ((vec_t)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
	z = minVal + ((vec_t)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
	w = minVal + ((vec_t)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
}

inline void Vector4DClear(Vector4D& a)
{
	a.x = a.y = a.z = a.w = 0.0f;
}

inline Vector4D& Vector4D::operator=(const Vector4D& vOther)
{
	Assert(vOther.IsValid());
	x = vOther.x; y = vOther.y; z = vOther.z; w = vOther.w;
	return *this;
}

inline vec_t& Vector4D::operator[](int i)
{
	Assert((i >= 0) && (i < 4));
	return ((vec_t*)this)[i];
}

inline vec_t Vector4D::operator[](int i) const
{
	Assert((i >= 0) && (i < 4));
	return ((vec_t*)this)[i];
}

inline Vector& Vector4D::AsVector3D()
{
	return *(Vector*)this;
}

inline Vector const& Vector4D::AsVector3D() const
{
	return *(Vector const*)this;
}

inline Vector2D& Vector4D::AsVector2D()
{
	return *(Vector2D*)this;
}

inline Vector2D const& Vector4D::AsVector2D() const
{
	return *(Vector2D const*)this;
}

inline vec_t* Vector4D::Base()
{
	return (vec_t*)this;
}

inline vec_t const* Vector4D::Base() const
{
	return (vec_t const*)this;
}

inline bool Vector4D::IsValid() const
{
	return IsFinite(x) && IsFinite(y) && IsFinite(z) && IsFinite(w);
}

inline bool Vector4D::operator==(Vector4D const& src) const
{
	Assert(src.IsValid() && IsValid());
	return (src.x == x) && (src.y == y) && (src.z == z) && (src.w == w);
}

inline bool Vector4D::operator!=(Vector4D const& src) const
{
	Assert(src.IsValid() && IsValid());
	return (src.x != x) || (src.y != y) || (src.z != z) || (src.w != w);
}


inline void Vector4DCopy(Vector4D const& src, Vector4D& dst)
{
	Assert(src.IsValid());
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
	dst.w = src.w;
}

inline void	Vector4D::CopyToArray(float* rgfl) const
{
	Assert(IsValid());
	Assert(rgfl);
	rgfl[0] = x; rgfl[1] = y; rgfl[2] = z; rgfl[3] = w;
}

inline void Vector4D::Negate()
{
	Assert(IsValid());
	x = -x; y = -y; z = -z; w = -w;
}

inline Vector4D& Vector4D::operator+=(const Vector4D& v)
{
	Assert(IsValid() && v.IsValid());
	x += v.x; y += v.y; z += v.z; w += v.w;
	return *this;
}

inline Vector4D& Vector4D::operator-=(const Vector4D& v)
{
	Assert(IsValid() && v.IsValid());
	x -= v.x; y -= v.y; z -= v.z; w -= v.w;
	return *this;
}

inline Vector4D& Vector4D::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;
	w *= fl;
	Assert(IsValid());
	return *this;
}

inline Vector4D& Vector4D::operator*=(Vector4D const& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	Assert(IsValid());
	return *this;
}

inline Vector4D& Vector4D::operator/=(float fl)
{
	Assert(fl != 0.0f);
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	w *= oofl;
	Assert(IsValid());
	return *this;
}

inline Vector4D& Vector4D::operator/=(Vector4D const& v)
{
	Assert(v.x != 0.0f && v.y != 0.0f && v.z != 0.0f && v.w != 0.0f);
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
	Assert(IsValid());
	return *this;
}

inline void Vector4DAdd(Vector4D const& a, Vector4D const& b, Vector4D& c)
{
	Assert(a.IsValid() && b.IsValid());
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	c.w = a.w + b.w;
}

inline void Vector4DSubtract(Vector4D const& a, Vector4D const& b, Vector4D& c)
{
	Assert(a.IsValid() && b.IsValid());
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	c.w = a.w - b.w;
}

inline void Vector4DMultiply(Vector4D const& a, vec_t b, Vector4D& c)
{
	Assert(a.IsValid() && IsFinite(b));
	c.x = a.x * b;
	c.y = a.y * b;
	c.z = a.z * b;
	c.w = a.w * b;
}

inline void Vector4DMultiply(Vector4D const& a, Vector4D const& b, Vector4D& c)
{
	Assert(a.IsValid() && b.IsValid());
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
	c.w = a.w * b.w;
}

inline void Vector4DDivide(Vector4D const& a, vec_t b, Vector4D& c)
{
	Assert(a.IsValid());
	Assert(b != 0.0f);
	vec_t oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
	c.z = a.z * oob;
	c.w = a.w * oob;
}

inline void Vector4DDivide(Vector4D const& a, Vector4D const& b, Vector4D& c)
{
	Assert(a.IsValid());
	Assert((b.x != 0.0f) && (b.y != 0.0f) && (b.z != 0.0f) && (b.w != 0.0f));
	c.x = a.x / b.x;
	c.y = a.y / b.y;
	c.z = a.z / b.z;
	c.w = a.w / b.w;
}

inline void Vector4DMA(Vector4D const& start, float s, Vector4D const& dir, Vector4D& result)
{
	Assert(start.IsValid() && IsFinite(s) && dir.IsValid());
	result.x = start.x + s * dir.x;
	result.y = start.y + s * dir.y;
	result.z = start.z + s * dir.z;
	result.w = start.w + s * dir.w;
}

inline void	Vector4D::MulAdd(Vector4D const& a, Vector4D const& b, float scalar)
{
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
	z = a.z + b.z * scalar;
	w = a.w + b.w * scalar;
}

inline void Vector4DLerp(const Vector4D& src1, const Vector4D& src2, vec_t t, Vector4D& dest)
{
	dest[0] = src1[0] + (src2[0] - src1[0]) * t;
	dest[1] = src1[1] + (src2[1] - src1[1]) * t;
	dest[2] = src1[2] + (src2[2] - src1[2]) * t;
	dest[3] = src1[3] + (src2[3] - src1[3]) * t;
}

inline vec_t DotProduct4D(const Vector4D& a, const Vector4D& b)
{
	Assert(a.IsValid() && b.IsValid());
	return(a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

inline vec_t Vector4D::Dot(Vector4D const& vOther) const
{
	return DotProduct4D(*this, vOther);
}


inline vec_t Vector4DLength(Vector4D const& v)
{
	Assert(v.IsValid());
	return (vec_t)FastSqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

inline vec_t Vector4D::LengthSqr(void) const
{
	Assert(IsValid());
	return (x * x + y * y + z * z + w * w);
}

inline vec_t Vector4D::Length(void) const
{
	return Vector4DLength(*this);
}


inline vec_t Vector4DNormalize(Vector4D& v)
{
	Assert(v.IsValid());
	vec_t l = v.Length();
	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		v.x = v.y = v.z = v.w = 0.0f;
	}
	return l;
}

inline vec_t Vector4D::DistTo(const Vector4D& vOther) const
{
	Vector4D delta;
	Vector4DSubtract(*this, vOther, delta);
	return delta.Length();
}

inline vec_t Vector4D::DistToSqr(const Vector4D& vOther) const
{
	Vector4D delta;
	Vector4DSubtract(*this, vOther, delta);
	return delta.LengthSqr();
}


inline Vector4DAligned::Vector4DAligned(vec_t X, vec_t Y, vec_t Z, vec_t W)
{
	x = X; y = Y; z = Z; w = W;
	Assert(IsValid());
}

inline void Vector4DAligned::Set(vec_t X, vec_t Y, vec_t Z, vec_t W)
{
	x = X; y = Y; z = Z; w = W;
	Assert(IsValid());
}

inline void Vector4DAligned::InitZero(void)
{
#if !defined( _X360 )
	this->AsM128() = _mm_set1_ps(0.0f);
#else
	this->AsM128() = __vspltisw(0);
#endif
	Assert(IsValid());
}

inline void Vector4DMultiplyAligned(Vector4DAligned const& a, Vector4DAligned const& b, Vector4DAligned& c)
{
	Assert(a.IsValid() && b.IsValid());
#if !defined( _X360 )
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
	c.w = a.w * b.w;
#else
	c.AsM128() = __vmulfp(a.AsM128(), b.AsM128());
#endif
}

inline void Vector4DWeightMAD(vec_t w, Vector4DAligned const& vInA, Vector4DAligned& vOutA, Vector4DAligned const& vInB, Vector4DAligned& vOutB)
{
	Assert(vInA.IsValid() && vInB.IsValid() && IsFinite(w));

#if !defined( _X360 )
	vOutA.x += vInA.x * w;
	vOutA.y += vInA.y * w;
	vOutA.z += vInA.z * w;
	vOutA.w += vInA.w * w;

	vOutB.x += vInB.x * w;
	vOutB.y += vInB.y * w;
	vOutB.z += vInB.z * w;
	vOutB.w += vInB.w * w;
#else
	__vector4 temp;

	temp = __lvlx(&w, 0);
	temp = __vspltw(temp, 0);

	vOutA.AsM128() = __vmaddfp(vInA.AsM128(), temp, vOutA.AsM128());
	vOutB.AsM128() = __vmaddfp(vInB.AsM128(), temp, vOutB.AsM128());
#endif
}

inline void Vector4DWeightMADSSE(vec_t w, Vector4DAligned const& vInA, Vector4DAligned& vOutA, Vector4DAligned const& vInB, Vector4DAligned& vOutB)
{
	Assert(vInA.IsValid() && vInB.IsValid() && IsFinite(w));

#if !defined( _X360 )
	__m128 packed = _mm_set1_ps(w);

	vOutA.AsM128() = _mm_add_ps(vOutA.AsM128(), _mm_mul_ps(vInA.AsM128(), packed));
	vOutB.AsM128() = _mm_add_ps(vOutB.AsM128(), _mm_mul_ps(vInB.AsM128(), packed));
#else
	__vector4 temp;

	temp = __lvlx(&w, 0);
	temp = __vspltw(temp, 0);

	vOutA.AsM128() = __vmaddfp(vInA.AsM128(), temp, vOutA.AsM128());
	vOutB.AsM128() = __vmaddfp(vInB.AsM128(), temp, vOutB.AsM128());
#endif
}

#endif  