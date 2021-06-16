#ifndef MATH_LIB_H
#define MATH_LIB_H

#include <math.h>
#include "sdk/basetypes.h"
#include "sdk/commonmacros.h"
#include "sdk/vector.h"
#include "sdk/vector2d.h"
#include "sdk/dbg.h"

#include "sdk/math_pfns.h"

#if defined(__i386__) || defined(_M_IX86)
#include <xmmintrin.h>
#endif

#undef clamp


#ifdef FP_EXCEPTIONS_ENABLED
#include <float.h>     
#endif

class FPExceptionDisabler
{
public:
#ifdef FP_EXCEPTIONS_ENABLED
	FPExceptionDisabler();
	~FPExceptionDisabler();

private:
	unsigned int mOldValues;
#else
	FPExceptionDisabler() {}
	~FPExceptionDisabler() {}
#endif

private:
	FPExceptionDisabler(const FPExceptionDisabler&);
	FPExceptionDisabler& operator=(const FPExceptionDisabler&);
};

class FPExceptionEnabler
{
public:
#ifdef FP_EXCEPTIONS_ENABLED
	FPExceptionEnabler(unsigned int enableBits = _EM_OVERFLOW | _EM_ZERODIVIDE | _EM_INVALID);
	~FPExceptionEnabler();

private:
	unsigned int mOldValues;
#else
	FPExceptionEnabler(unsigned int enableBits = 0)
	{
	}
	~FPExceptionEnabler()
	{
	}
#endif

private:
	FPExceptionEnabler(const FPExceptionEnabler&);
	FPExceptionEnabler& operator=(const FPExceptionEnabler&);
};



#ifdef DEBUG     
FORCEINLINE float clamp(float val, float minVal, float maxVal)
{
	if (maxVal < minVal)
		return maxVal;
	else if (val < minVal)
		return minVal;
	else if (val > maxVal)
		return maxVal;
	else
		return val;
}
#else  
FORCEINLINE float clamp(float val, float minVal, float maxVal)
{
#if defined(__i386__) || defined(_M_IX86)
	_mm_store_ss(&val,
		_mm_min_ss(
			_mm_max_ss(
				_mm_load_ss(&val),
				_mm_load_ss(&minVal)),
			_mm_load_ss(&maxVal)));
#else
	val = fpmax(minVal, val);
	val = fpmin(maxVal, val);
#endif
	return val;
}
#endif  

template< class T >
inline T clamp(T const& val, T const& minVal, T const& maxVal)
{
	if (maxVal < minVal)
		return maxVal;
	else if (val < minVal)
		return minVal;
	else if (val > maxVal)
		return maxVal;
	else
		return val;
}


struct cplane_t
{
	Vector	normal;
	float	dist;
	byte	type;			    
	byte	signbits;		     
	byte	pad[2];

#ifdef VECTOR_NO_SLOW_OPERATIONS
	cplane_t() {}

private:
	cplane_t(const cplane_t& vOther);
#endif
};

#define CPLANE_NORMAL_X			0
#define CPLANE_NORMAL_Y			4
#define CPLANE_NORMAL_Z			8
#define CPLANE_DIST				12
#define CPLANE_TYPE				16
#define CPLANE_SIGNBITS			17
#define CPLANE_PAD0				18
#define CPLANE_PAD1				19

#define	PLANE_X			0
#define	PLANE_Y			1
#define	PLANE_Z			2

#define	PLANE_ANYX		3
#define	PLANE_ANYY		4
#define	PLANE_ANYZ		5


enum
{
	FRUSTUM_RIGHT = 0,
	FRUSTUM_LEFT = 1,
	FRUSTUM_TOP = 2,
	FRUSTUM_BOTTOM = 3,
	FRUSTUM_NEARZ = 4,
	FRUSTUM_FARZ = 5,
	FRUSTUM_NUMPLANES = 6
};

extern int SignbitsForPlane(cplane_t* out);

class Frustum_t
{
public:
	void SetPlane(int i, int nType, const Vector& vecNormal, float dist)
	{
		m_Plane[i].normal = vecNormal;
		m_Plane[i].dist = dist;
		m_Plane[i].type = nType;
		m_Plane[i].signbits = SignbitsForPlane(&m_Plane[i]);
		m_AbsNormal[i].Init(fabs(vecNormal.x), fabs(vecNormal.y), fabs(vecNormal.z));
	}

	inline const cplane_t* GetPlane(int i) const { return &m_Plane[i]; }
	inline const Vector& GetAbsNormal(int i) const { return m_AbsNormal[i]; }

private:
	cplane_t	m_Plane[FRUSTUM_NUMPLANES];
	Vector		m_AbsNormal[FRUSTUM_NUMPLANES];
};

float CalcFovY(float flFovX, float flScreenAspect);
float CalcFovX(float flFovY, float flScreenAspect);

void GeneratePerspectiveFrustum(const Vector& origin, const QAngle& angles, float flZNear, float flZFar, float flFovX, float flAspectRatio, Frustum_t& frustum);
void GeneratePerspectiveFrustum(const Vector& origin, const Vector& forward, const Vector& right, const Vector& up, float flZNear, float flZFar, float flFovX, float flFovY, Frustum_t& frustum);

bool R_CullBox(const Vector& mins, const Vector& maxs, const Frustum_t& frustum);
bool R_CullBoxSkipNear(const Vector& mins, const Vector& maxs, const Frustum_t& frustum);

struct matrix3x4_t
{
	matrix3x4_t() {}
	matrix3x4_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		m_flMatVal[0][0] = m00;	m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
		m_flMatVal[1][0] = m10;	m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
		m_flMatVal[2][0] = m20;	m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
	}

	void Init(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector& vecOrigin)
	{
		m_flMatVal[0][0] = xAxis.x; m_flMatVal[0][1] = yAxis.x; m_flMatVal[0][2] = zAxis.x; m_flMatVal[0][3] = vecOrigin.x;
		m_flMatVal[1][0] = xAxis.y; m_flMatVal[1][1] = yAxis.y; m_flMatVal[1][2] = zAxis.y; m_flMatVal[1][3] = vecOrigin.y;
		m_flMatVal[2][0] = xAxis.z; m_flMatVal[2][1] = yAxis.z; m_flMatVal[2][2] = zAxis.z; m_flMatVal[2][3] = vecOrigin.z;
	}

	matrix3x4_t(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector& vecOrigin)
	{
		Init(xAxis, yAxis, zAxis, vecOrigin);
	}

	inline void Invalidate(void)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m_flMatVal[i][j] = VEC_T_NAN;
			}
		}
	}

	float* operator[](int i) { Assert((i >= 0) && (i < 3)); return m_flMatVal[i]; }
	const float* operator[](int i) const { Assert((i >= 0) && (i < 3)); return m_flMatVal[i]; }
	float* Base() { return &m_flMatVal[0][0]; }
	const float* Base() const { return &m_flMatVal[0][0]; }

	float m_flMatVal[3][4];
};

#include "fltx4.h"

class ALIGN16 matrix3x4a_t : public matrix3x4_t
{
public:
	matrix3x4a_t(const matrix3x4_t& src) { *this = src; };
	matrix3x4a_t& operator=(const matrix3x4_t& src) { memcpy(Base(), src.Base(), sizeof(float) * 3 * 4); return *this; };

	matrix3x4a_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		AssertDbg(((size_t)Base() & 0xf) == 0);
		m_flMatVal[0][0] = m00;	m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
		m_flMatVal[1][0] = m10;	m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
		m_flMatVal[2][0] = m20;	m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
	}
	matrix3x4a_t() {}

	static FORCEINLINE bool TypeIsAlignedForSIMD(void) { return true; }


	FORCEINLINE fltx4& SIMDRow(uint nIdx) { AssertDbg(nIdx < 3); return *((fltx4*)(&(m_flMatVal[nIdx]))); }
	FORCEINLINE const fltx4& SIMDRow(uint nIdx) const { AssertDbg(nIdx < 3); return *((const fltx4*)(&(m_flMatVal[nIdx]))); }

} ALIGN16_POST;



#ifndef M_PI
#define M_PI		3.14159265358979323846	      
#endif

#define M_PI_F		((float)(M_PI))	    

#ifndef RAD2DEG
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )
#endif

#ifndef DEG2RAD
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )
#endif

#define	SIDE_FRONT	0
#define	SIDE_BACK	1
#define	SIDE_ON		2
#define SIDE_CROSS  -2         

#define ON_VIS_EPSILON  0.01              
#define	EQUAL_EPSILON	0.001              

extern bool s_bMathlibInitialized;

extern  const Vector vec3_origin;
extern  const QAngle vec3_angle;
extern	const Quaternion quat_identity;
extern const Vector vec3_invalid;
extern	const int nanmask;

#define	IS_NAN(x) (((*(int *)&x)&nanmask)==nanmask)

FORCEINLINE vec_t DotProduct(const vec_t* v1, const vec_t* v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}
FORCEINLINE void VectorSubtract(const vec_t* a, const vec_t* b, vec_t* c)
{
	c[0] = a[0] - b[0];
	c[1] = a[1] - b[1];
	c[2] = a[2] - b[2];
}
FORCEINLINE void VectorAdd(const vec_t* a, const vec_t* b, vec_t* c)
{
	c[0] = a[0] + b[0];
	c[1] = a[1] + b[1];
	c[2] = a[2] + b[2];
}
FORCEINLINE void VectorCopy(const vec_t* a, vec_t* b)
{
	b[0] = a[0];
	b[1] = a[1];
	b[2] = a[2];
}
FORCEINLINE void VectorClear(vec_t* a)
{
	a[0] = a[1] = a[2] = 0;
}

FORCEINLINE float VectorMaximum(const vec_t* v)
{
	return max(v[0], max(v[1], v[2]));
}

FORCEINLINE float VectorMaximum(const Vector& v)
{
	return max(v.x, max(v.y, v.z));
}

FORCEINLINE void VectorScale(const float* in, vec_t scale, float* out)
{
	out[0] = in[0] * scale;
	out[1] = in[1] * scale;
	out[2] = in[2] * scale;
}


inline void VectorFill(vec_t* a, float b)
{
	a[0] = a[1] = a[2] = b;
}

inline void VectorNegate(vec_t* a)
{
	a[0] = -a[0];
	a[1] = -a[1];
	a[2] = -a[2];
}


#define Vector2Clear(x)			{(x)[0]=(x)[1]=0;}
#define Vector2Negate(x)		{(x)[0]=-((x)[0]);(x)[1]=-((x)[1]);}
#define Vector2Copy(a,b)		{(b)[0]=(a)[0];(b)[1]=(a)[1];}
#define Vector2Subtract(a,b,c)	{(c)[0]=(a)[0]-(b)[0];(c)[1]=(a)[1]-(b)[1];}
#define Vector2Add(a,b,c)		{(c)[0]=(a)[0]+(b)[0];(c)[1]=(a)[1]+(b)[1];}
#define Vector2Scale(a,b,c)		{(c)[0]=(b)*(a)[0];(c)[1]=(b)*(a)[1];}

#define VECTOR_COPY( A, B ) do { (B)[0] = (A)[0]; (B)[1] = (A)[1]; (B)[2]=(A)[2]; } while(0)
#define DOT_PRODUCT( A, B ) ( (A)[0]*(B)[0] + (A)[1]*(B)[1] + (A)[2]*(B)[2] )

FORCEINLINE void VectorMAInline(const float* start, float scale, const float* direction, float* dest)
{
	dest[0] = start[0] + direction[0] * scale;
	dest[1] = start[1] + direction[1] * scale;
	dest[2] = start[2] + direction[2] * scale;
}

FORCEINLINE void VectorMAInline(const Vector& start, float scale, const Vector& direction, Vector& dest)
{
	dest.x = start.x + direction.x * scale;
	dest.y = start.y + direction.y * scale;
	dest.z = start.z + direction.z * scale;
}

FORCEINLINE void VectorMA(const Vector& start, float scale, const Vector& direction, Vector& dest)
{
	VectorMAInline(start, scale, direction, dest);
}

FORCEINLINE void VectorMA(const float* start, float scale, const float* direction, float* dest)
{
	VectorMAInline(start, scale, direction, dest);
}


int VectorCompare(const float* v1, const float* v2);

inline float VectorLength(const float* v)
{
	return FastSqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + FLT_EPSILON);
}

void CrossProduct(const float* v1, const float* v2, float* cross);

qboolean VectorsEqual(const float* v1, const float* v2);

inline vec_t RoundInt(vec_t in)
{
	return floor(in + 0.5f);
}

int Q_log2(int val);

void inline SinCos(float radians, float* sine, float* cosine)
{
#if defined( _X360 )
	XMScalarSinCos(sine, cosine, radians);
#elif defined( PLATFORM_WINDOWS_PC32 )
	_asm
	{
		fld		DWORD PTR[radians]
		fsincos

		mov edx, DWORD PTR[cosine]
		mov eax, DWORD PTR[sine]

		fstp DWORD PTR[edx]
		fstp DWORD PTR[eax]
	}
#elif defined( PLATFORM_WINDOWS_PC64 )
	*sine = sin(radians);
	*cosine = cos(radians);
#elif defined( POSIX )
	register double __cosr, __sinr;
	__asm ("fsincos" : "=t" (__cosr), "=u" (__sinr) : "0" (radians));

	*sine = __sinr;
	*cosine = __cosr;
#endif
}

#define SIN_TABLE_SIZE	256
#define FTOIBIAS		12582912.f
extern float SinCosTable[SIN_TABLE_SIZE];

inline float TableCos(float theta)
{
	union
	{
		int i;
		float f;
	} ftmp;

	ftmp.f = theta * (float)(SIN_TABLE_SIZE / (2.0f * M_PI)) + (FTOIBIAS + (SIN_TABLE_SIZE / 4));
	return SinCosTable[ftmp.i & (SIN_TABLE_SIZE - 1)];
}

inline float TableSin(float theta)
{
	union
	{
		int i;
		float f;
	} ftmp;

	ftmp.f = theta * (float)(SIN_TABLE_SIZE / (2.0f * M_PI)) + FTOIBIAS;
	return SinCosTable[ftmp.i & (SIN_TABLE_SIZE - 1)];
}

template<class T>
FORCEINLINE T Square(T const& a)
{
	return a * a;
}


FORCEINLINE uint SmallestPowerOfTwoGreaterOrEqual(uint x)
{
	x -= 1;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	return x + 1;
}

FORCEINLINE uint LargestPowerOfTwoLessThanOrEqual(uint x)
{
	if (x >= 0x80000000)
		return 0x80000000;

	return SmallestPowerOfTwoGreaterOrEqual(x + 1) >> 1;
}


void FloorDivMod(double numer, double denom, int* quotient, int* rem);
int GreatestCommonDivisor(int i1, int i2);

bool IsDenormal(const float& val);

enum
{
	PITCH = 0,	   
	YAW,		   
	ROLL		  
};

void MatrixAngles(const matrix3x4_t& matrix, float* angles);  
void MatrixVectors(const matrix3x4_t& matrix, Vector* pForward, Vector* pRight, Vector* pUp);
void VectorTransform(const float* in1, const matrix3x4_t& in2, float* out);
void VectorITransform(const float* in1, const matrix3x4_t& in2, float* out);
void VectorRotate(const float* in1, const matrix3x4_t& in2, float* out);
void VectorRotate(const Vector& in1, const QAngle& in2, Vector& out);
void VectorRotate(const Vector& in1, const Quaternion& in2, Vector& out);
void VectorIRotate(const float* in1, const matrix3x4_t& in2, float* out);

#ifndef VECTOR_NO_SLOW_OPERATIONS

QAngle TransformAnglesToLocalSpace(const QAngle& angles, const matrix3x4_t& parentMatrix);
QAngle TransformAnglesToWorldSpace(const QAngle& angles, const matrix3x4_t& parentMatrix);

#endif

void MatrixInitialize(matrix3x4_t& mat, const Vector& vecOrigin, const Vector& vecXAxis, const Vector& vecYAxis, const Vector& vecZAxis);
void MatrixCopy(const matrix3x4_t& in, matrix3x4_t& out);
void MatrixInvert(const matrix3x4_t& in, matrix3x4_t& out);

bool MatricesAreEqual(const matrix3x4_t& src1, const matrix3x4_t& src2, float flTolerance = 1e-5);

void MatrixGetColumn(const matrix3x4_t& in, int column, Vector& out);
void MatrixSetColumn(const Vector& in, int column, matrix3x4_t& out);

inline void MatrixGetTranslation(const matrix3x4_t& in, Vector& out)
{
	MatrixGetColumn(in, 3, out);
}

inline void MatrixSetTranslation(const Vector& in, matrix3x4_t& out)
{
	MatrixSetColumn(in, 3, out);
}

void MatrixScaleBy(const float flScale, matrix3x4_t& out);
void MatrixScaleByZero(matrix3x4_t& out);

void ConcatRotations(const matrix3x4_t& in1, const matrix3x4_t& in2, matrix3x4_t& out);
void ConcatTransforms(const matrix3x4_t& in1, const matrix3x4_t& in2, matrix3x4_t& out);

inline void MatrixMultiply(const matrix3x4_t& in1, const matrix3x4_t& in2, matrix3x4_t& out)
{
	ConcatTransforms(in1, in2, out);
}

void QuaternionSlerp(const Quaternion& p, const Quaternion& q, float t, Quaternion& qt);
void QuaternionSlerpNoAlign(const Quaternion& p, const Quaternion& q, float t, Quaternion& qt);
void QuaternionBlend(const Quaternion& p, const Quaternion& q, float t, Quaternion& qt);
void QuaternionBlendNoAlign(const Quaternion& p, const Quaternion& q, float t, Quaternion& qt);
void QuaternionIdentityBlend(const Quaternion& p, float t, Quaternion& qt);
float QuaternionAngleDiff(const Quaternion& p, const Quaternion& q);
void QuaternionScale(const Quaternion& p, float t, Quaternion& q);
void QuaternionAlign(const Quaternion& p, const Quaternion& q, Quaternion& qt);
float QuaternionDotProduct(const Quaternion& p, const Quaternion& q);
void QuaternionConjugate(const Quaternion& p, Quaternion& q);
void QuaternionInvert(const Quaternion& p, Quaternion& q);
float QuaternionNormalize(Quaternion& q);
void QuaternionAdd(const Quaternion& p, const Quaternion& q, Quaternion& qt);
void QuaternionMult(const Quaternion& p, const Quaternion& q, Quaternion& qt);
void QuaternionMatrix(const Quaternion& q, matrix3x4_t& matrix);
void QuaternionMatrix(const Quaternion& q, const Vector& pos, matrix3x4_t& matrix);
void QuaternionAngles(const Quaternion& q, QAngle& angles);
void AngleQuaternion(const QAngle& angles, Quaternion& qt);
void QuaternionAngles(const Quaternion& q, RadianEuler& angles);
void AngleQuaternion(RadianEuler const& angles, Quaternion& qt);
void QuaternionAxisAngle(const Quaternion& q, Vector& axis, float& angle);
void AxisAngleQuaternion(const Vector& axis, float angle, Quaternion& q);
void BasisToQuaternion(const Vector& vecForward, const Vector& vecRight, const Vector& vecUp, Quaternion& q);
void MatrixQuaternion(const matrix3x4_t& mat, Quaternion& q);

inline float MatrixRowDotProduct(const matrix3x4_t& in1, int row, const Vector& in2)
{
	Assert((row >= 0) && (row < 3));
	return DotProduct(in1[row], in2.Base());
}

inline float MatrixColumnDotProduct(const matrix3x4_t& in1, int col, const Vector& in2)
{
	Assert((col >= 0) && (col < 4));
	return in1[0][col] * in2[0] + in1[1][col] * in2[1] + in1[2][col] * in2[2];
}

int __cdecl BoxOnPlaneSide(const float* emins, const float* emaxs, const cplane_t* plane);

inline float anglemod(float a)
{
	a = (360.f / 65536) * ((int)(a * (65536.f / 360.0f)) & 65535);
	return a;
}

inline float RemapVal(float val, float A, float B, float C, float D)
{
	if (A == B)
		return val >= B ? D : C;
	return C + (D - C) * (val - A) / (B - A);
}

inline float RemapValClamped(float val, float A, float B, float C, float D)
{
	if (A == B)
		return val >= B ? D : C;
	float cVal = (val - A) / (B - A);
	cVal = clamp(cVal, 0.0f, 1.0f);

	return C + (D - C) * cVal;
}

template <class T>
FORCEINLINE T Lerp(float flPercent, T const& A, T const& B)
{
	return A + (B - A) * flPercent;
}

FORCEINLINE float Sqr(float f)
{
	return f * f;
}

static inline float FLerp(float f1, float f2, float i1, float i2, float x)
{
	return f1 + (f2 - f1) * (x - i1) / (i2 - i1);
}


#ifndef VECTOR_NO_SLOW_OPERATIONS

template<> FORCEINLINE QAngle Lerp<QAngle>(float flPercent, const QAngle& q1, const QAngle& q2)
{
	if (q1 == q2)
		return q1;

	Quaternion src, dest;

	AngleQuaternion(q1, src);
	AngleQuaternion(q2, dest);

	Quaternion result;

	QuaternionSlerp(src, dest, flPercent, result);

	QAngle output;
	QuaternionAngles(result, output);
	return output;
}

#else

#pragma error

template<> FORCEINLINE QAngleByValue Lerp<QAngleByValue>(float flPercent, const QAngleByValue& q1, const QAngleByValue& q2)
{
	if (q1 == q2)
		return q1;

	Quaternion src, dest;

	AngleQuaternion(q1, src);
	AngleQuaternion(q2, dest);

	Quaternion result;

	QuaternionSlerp(src, dest, flPercent, result);

	QAngleByValue output;
	QuaternionAngles(result, output);
	return output;
}

#endif  


template <class T>
FORCEINLINE void V_swap(T& x, T& y)
{
	T temp = x;
	x = y;
	y = temp;
}

template <class T> FORCEINLINE T AVG(T a, T b)
{
	return (a + b) / 2;
}

#define NELEMS(x) ARRAYSIZE(x)

#define XYZ(v) (v).x,(v).y,(v).z


inline float Sign(float x)
{
	return (x < 0.0f) ? -1.0f : 1.0f;
}

inline int ClampArrayBounds(int n, unsigned maxindex)
{
	unsigned int inrangemask = 0xFFFFFFFF + (((unsigned)n) > maxindex);
	unsigned int lessthan0mask = 0xFFFFFFFF + (n >= 0);

	int result = (inrangemask & n);

	result |= ((~inrangemask) & (~lessthan0mask)) & maxindex;

	return result;
}


#define BOX_ON_PLANE_SIDE(emins, emaxs, p)	\
	(((p)->type < 3)?						\
	(										\
		((p)->dist <= (emins)[(p)->type])?	\
			1								\
		:									\
		(									\
			((p)->dist >= (emaxs)[(p)->type])?\
				2							\
			:								\
				3							\
		)									\
	)										\
	:										\
		BoxOnPlaneSide( (emins), (emaxs), (p)))

void AngleVectors(const QAngle& angles, Vector* forward);
void AngleVectors(const QAngle& angles, Vector* forward, Vector* right, Vector* up);
void AngleVectorsTranspose(const QAngle& angles, Vector* forward, Vector* right, Vector* up);
void AngleMatrix(const QAngle& angles, matrix3x4_t& mat);
void AngleMatrix(const QAngle& angles, const Vector& position, matrix3x4_t& mat);
void AngleMatrix(const RadianEuler& angles, matrix3x4_t& mat);
void AngleMatrix(RadianEuler const& angles, const Vector& position, matrix3x4_t& mat);
void AngleIMatrix(const QAngle& angles, matrix3x4_t& mat);
void AngleIMatrix(const QAngle& angles, const Vector& position, matrix3x4_t& mat);
void AngleIMatrix(const RadianEuler& angles, matrix3x4_t& mat);
void VectorAngles(const Vector& forward, QAngle& angles);
void VectorAngles(const Vector& forward, const Vector& pseudoup, QAngle& angles);
void VectorMatrix(const Vector& forward, matrix3x4_t& mat);
void VectorVectors(const Vector& forward, Vector& right, Vector& up);
void SetIdentityMatrix(matrix3x4_t& mat);
void SetScaleMatrix(float x, float y, float z, matrix3x4_t& dst);
void MatrixBuildRotationAboutAxis(const Vector& vAxisOfRot, float angleDegrees, matrix3x4_t& dst);

inline void SetScaleMatrix(float flScale, matrix3x4_t& dst)
{
	SetScaleMatrix(flScale, flScale, flScale, dst);
}

inline void SetScaleMatrix(const Vector& scale, matrix3x4_t& dst)
{
	SetScaleMatrix(scale.x, scale.y, scale.z, dst);
}

void MatrixTranspose(matrix3x4_t& mat);
void MatrixTranspose(const matrix3x4_t& src, matrix3x4_t& dst);
void MatrixInverseTranspose(const matrix3x4_t& src, matrix3x4_t& dst);

inline void PositionMatrix(const Vector& position, matrix3x4_t& mat)
{
	MatrixSetColumn(position, 3, mat);
}

inline void MatrixPosition(const matrix3x4_t& matrix, Vector& position)
{
	MatrixGetColumn(matrix, 3, position);
}

inline void VectorRotate(const Vector& in1, const matrix3x4_t& in2, Vector& out)
{
	VectorRotate(&in1.x, in2, &out.x);
}

inline void VectorIRotate(const Vector& in1, const matrix3x4_t& in2, Vector& out)
{
	VectorIRotate(&in1.x, in2, &out.x);
}

inline void MatrixAngles(const matrix3x4_t& matrix, QAngle& angles)
{
	MatrixAngles(matrix, &angles.x);
}

inline void MatrixAngles(const matrix3x4_t& matrix, QAngle& angles, Vector& position)
{
	MatrixAngles(matrix, angles);
	MatrixPosition(matrix, position);
}

inline void MatrixAngles(const matrix3x4_t& matrix, RadianEuler& angles)
{
	MatrixAngles(matrix, &angles.x);

	angles.Init(DEG2RAD(angles.z), DEG2RAD(angles.x), DEG2RAD(angles.y));
}

void MatrixAngles(const matrix3x4_t& mat, RadianEuler& angles, Vector& position);

void MatrixAngles(const matrix3x4_t& mat, Quaternion& q, Vector& position);

inline int VectorCompare(const Vector& v1, const Vector& v2)
{
	return v1 == v2;
}

inline void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out)
{
	VectorTransform(&in1.x, in2, &out.x);
}

inline void VectorITransform(const Vector& in1, const matrix3x4_t& in2, Vector& out)
{
	VectorITransform(&in1.x, in2, &out.x);
}

inline int BoxOnPlaneSide(const Vector& emins, const Vector& emaxs, const cplane_t* plane)
{
	return BoxOnPlaneSide(&emins.x, &emaxs.x, plane);
}

inline void VectorFill(Vector& a, float b)
{
	a[0] = a[1] = a[2] = b;
}

inline void VectorNegate(Vector& a)
{
	a[0] = -a[0];
	a[1] = -a[1];
	a[2] = -a[2];
}

inline vec_t VectorAvg(Vector& a)
{
	return (a[0] + a[1] + a[2]) / 3;
}

inline int FASTCALL BoxOnPlaneSide2(const Vector& emins, const Vector& emaxs, const cplane_t* p, float tolerance = 0.f)
{
	Vector	corners[2];

	if (p->normal[0] < 0)
	{
		corners[0][0] = emins[0];
		corners[1][0] = emaxs[0];
	}
	else
	{
		corners[1][0] = emins[0];
		corners[0][0] = emaxs[0];
	}

	if (p->normal[1] < 0)
	{
		corners[0][1] = emins[1];
		corners[1][1] = emaxs[1];
	}
	else
	{
		corners[1][1] = emins[1];
		corners[0][1] = emaxs[1];
	}

	if (p->normal[2] < 0)
	{
		corners[0][2] = emins[2];
		corners[1][2] = emaxs[2];
	}
	else
	{
		corners[1][2] = emins[2];
		corners[0][2] = emaxs[2];
	}

	int sides = 0;

	float dist1 = DotProduct(p->normal, corners[0]) - p->dist;
	if (dist1 >= tolerance)
		sides = 1;

	float dist2 = DotProduct(p->normal, corners[1]) - p->dist;
	if (dist2 < -tolerance)
		sides |= 2;

	return sides;
}

void ClearBounds(Vector& mins, Vector& maxs);
void AddPointToBounds(const Vector& v, Vector& mins, Vector& maxs);

void BuildGammaTable(float gamma, float texGamma, float brightness, int overbright);

inline float TexLightToLinear(int c, int exponent)
{
	extern float power2_n[256];
	Assert(exponent >= -128 && exponent <= 127);
	return (float)c * power2_n[exponent + 128];
}


int LinearToTexture(float f);
int LinearToScreenGamma(float f);
float TextureToLinear(int c);

struct ColorRGBExp32
{
	byte r, g, b;
	signed char exponent;
};

void ColorRGBExp32ToVector(const ColorRGBExp32& in, Vector& out);
void VectorToColorRGBExp32(const Vector& v, ColorRGBExp32& c);

bool SolveQuadratic(float a, float b, float c, float& root1, float& root2);

bool SolveInverseQuadratic(float x1, float y1, float x2, float y2, float x3, float y3, float& a, float& b, float& c);

bool SolveInverseQuadraticMonotonic(float x1, float y1, float x2, float y2,
	float x3, float y3, float& a, float& b, float& c);




bool SolveInverseReciprocalQuadratic(float x1, float y1, float x2, float y2, float x3, float y3, float& a, float& b, float& c);

void VectorYawRotate(const Vector& in, float flYaw, Vector& out);


float Bias(float x, float biasAmt);


float Gain(float x, float biasAmt);


float SmoothCurve(float x);


float SmoothCurve_Tweak(float x, float flPeakPos = 0.5, float flPeakSharpness = 0.5);


inline float ExponentialDecay(float halflife, float dt)
{
	return expf(-0.69314718f / halflife * dt);
}

inline float ExponentialDecay(float decayTo, float decayTime, float dt)
{
	return expf(logf(decayTo) / decayTime * dt);
}

inline float ExponentialDecayIntegral(float decayTo, float decayTime, float dt)
{
	return (powf(decayTo, dt / decayTime) * decayTime - decayTime) / logf(decayTo);
}

inline float SimpleSpline(float value)
{
	float valueSquared = value * value;

	return (3 * valueSquared - 2 * valueSquared * value);
}

inline float SimpleSplineRemapVal(float val, float A, float B, float C, float D)
{
	if (A == B)
		return val >= B ? D : C;
	float cVal = (val - A) / (B - A);
	return C + (D - C) * SimpleSpline(cVal);
}

inline float SimpleSplineRemapValClamped(float val, float A, float B, float C, float D)
{
	if (A == B)
		return val >= B ? D : C;
	float cVal = (val - A) / (B - A);
	cVal = clamp(cVal, 0.0f, 1.0f);
	return C + (D - C) * SimpleSpline(cVal);
}

FORCEINLINE int RoundFloatToInt(float f)
{
#if defined(__i386__) || defined(_M_IX86) || defined( PLATFORM_WINDOWS_PC64 )
	return _mm_cvtss_si32(_mm_load_ss(&f));
#elif defined( _X360 )
#ifdef Assert
	Assert(IsFPUControlWordSet());
#endif
	union
	{
		double flResult;
		int pResult[2];
	};
	flResult = __fctiw(f);
	return pResult[1];
#else
#error Unknown architecture
#endif
}

FORCEINLINE unsigned char RoundFloatToByte(float f)
{
	int nResult = RoundFloatToInt(f);
#ifdef Assert
	Assert((nResult & ~0xFF) == 0);
#endif
	return (unsigned char)nResult;
}

FORCEINLINE unsigned long RoundFloatToUnsignedLong(float f)
{
#if defined( _X360 )
#ifdef Assert
	Assert(IsFPUControlWordSet());
#endif
	union
	{
		double flResult;
		int pIntResult[2];
		unsigned long pResult[2];
	};
	flResult = __fctiw(f);
	Assert(pIntResult[1] >= 0);
	return pResult[1];
#else   

#if defined( PLATFORM_WINDOWS_PC64 )
	uint nRet = (uint)f;
	if (nRet & 1)
	{
		if ((f - floor(f) >= 0.5))
		{
			nRet++;
		}
	}
	else
	{
		if ((f - floor(f) > 0.5))
		{
			nRet++;
		}
	}
	return nRet;
#else  
	unsigned char nResult[8];

#if defined( _WIN32 )
	__asm
	{
		fld f
		fistp       qword ptr nResult
	}
#elif POSIX
	__asm __volatile__(
	"fistpl %0;": "=m" (nResult) : "t" (f) : "st"
		);
#endif

	return *((unsigned long*)nResult);
#endif  
#endif  
}

FORCEINLINE bool IsIntegralValue(float flValue, float flTolerance = 0.001f)
{
	return fabs(RoundFloatToInt(flValue) - flValue) < flTolerance;
}

FORCEINLINE int Float2Int(float a)
{
#if defined( _X360 )
	union
	{
		double flResult;
		int pResult[2];
	};
	flResult = __fctiwz(a);
	return pResult[1];
#else   
	return (int)a;
#endif
}

inline int Floor2Int(float a)
{
	int RetVal;
#if defined( __i386__ )
	__m128 a128 = _mm_set_ss(a);
	RetVal = _mm_cvtss_si32(a128);
	__m128 rounded128 = _mm_cvt_si2ss(_mm_setzero_ps(), RetVal);
	RetVal -= _mm_comigt_ss(rounded128, a128);
#else
	RetVal = static_cast<int>(floor(a));
#endif
	return RetVal;
}

FORCEINLINE unsigned int FastFToC(float c)
{
#if defined( __i386__ )
	union { float f; int i; } convert = { c * 255.0f + (float)(1 << 23) };
	return convert.i & 255;
#else
	return Float2Int(c * 255.0f);
#endif
}

FORCEINLINE int FastFloatToSmallInt(float c)
{
#if defined( __i386__ )
	union { float f; int i; } convert = { c + (float)(3 << 22) };
	return (convert.i & ((1 << 23) - 1)) - (1 << 22);
#else
	return Float2Int(c);
#endif
}

inline float ClampToMsec(float in)
{
	int msec = Floor2Int(in * 1000.0f + 0.5f);
	return 0.001f * msec;
}

inline int Ceil2Int(float a)
{
	int RetVal;
#if defined( __i386__ )
	__m128 a128 = _mm_load_ss(&a);
	RetVal = _mm_cvtss_si32(a128);
	__m128 rounded128 = _mm_cvt_si2ss(_mm_setzero_ps(), RetVal);
	RetVal += _mm_comilt_ss(rounded128, a128);
#else
	RetVal = static_cast<int>(ceil(a));
#endif
	return RetVal;
}


#define TriArea2D( A, B, C ) \
	( 0.5f * ( ( B.x - A.x ) * ( C.y - A.y ) - ( B.y - A.y ) * ( C.x - A.x ) ) )

#define TriArea2DTimesTwo( A, B, C ) \
	( ( ( B.x - A.x ) * ( C.y - A.y ) - ( B.y - A.y ) * ( C.x - A.x ) ) )


inline void GetBarycentricCoords2D(
	Vector2D const& A,
	Vector2D const& B,
	Vector2D const& C,
	Vector2D const& pt,
	float bcCoords[3])
{
	float invTriArea = 1.0f / TriArea2DTimesTwo(A, B, C);

	bcCoords[0] = TriArea2DTimesTwo(B, C, pt) * invTriArea;
	bcCoords[1] = TriArea2DTimesTwo(C, A, pt) * invTriArea;
	bcCoords[2] = TriArea2DTimesTwo(A, B, pt) * invTriArea;
}


inline bool QuickBoxSphereTest(
	const Vector& vOrigin,
	float flRadius,
	const Vector& bbMin,
	const Vector& bbMax)
{
	return vOrigin.x - flRadius < bbMax.x&& vOrigin.x + flRadius > bbMin.x &&
		vOrigin.y - flRadius < bbMax.y&& vOrigin.y + flRadius > bbMin.y &&
		vOrigin.z - flRadius < bbMax.z&& vOrigin.z + flRadius > bbMin.z;
}


inline bool QuickBoxIntersectTest(
	const Vector& vBox1Min,
	const Vector& vBox1Max,
	const Vector& vBox2Min,
	const Vector& vBox2Max)
{
	return
		vBox1Min.x < vBox2Max.x&& vBox1Max.x > vBox2Min.x &&
		vBox1Min.y < vBox2Max.y&& vBox1Max.y > vBox2Min.y &&
		vBox1Min.z < vBox2Max.z&& vBox1Max.z > vBox2Min.z;
}


extern float GammaToLinearFullRange(float gamma);
extern float LinearToGammaFullRange(float linear);
extern float GammaToLinear(float gamma);
extern float LinearToGamma(float linear);

extern float SrgbGammaToLinear(float flSrgbGammaValue);
extern float SrgbLinearToGamma(float flLinearValue);
extern float X360GammaToLinear(float fl360GammaValue);
extern float X360LinearToGamma(float flLinearValue);
extern float SrgbGammaTo360Gamma(float flSrgbGammaValue);

FORCEINLINE float LinearToVertexLight(float f)
{
	extern float lineartovertex[4096];

	int i = RoundFloatToInt(f * 1024.f);

	if ((unsigned)i > 4095)
	{
		if (i < 0)
			i = 0;		              
		else
			i = 4095;
	}

	return lineartovertex[i];
}


FORCEINLINE unsigned char LinearToLightmap(float f)
{
	extern unsigned char lineartolightmap[4096];

	int i = RoundFloatToInt(f * 1024.f);	   

	if ((unsigned)i > 4095)
	{
		if (i < 0)
			i = 0;		              
		else
			i = 4095;
	}

	return lineartolightmap[i];
}

FORCEINLINE void ColorClamp(Vector& color)
{
	float maxc = max(color.x, max(color.y, color.z));
	if (maxc > 1.0f)
	{
		float ooMax = 1.0f / maxc;
		color.x *= ooMax;
		color.y *= ooMax;
		color.z *= ooMax;
	}

	if (color[0] < 0.f) color[0] = 0.f;
	if (color[1] < 0.f) color[1] = 0.f;
	if (color[2] < 0.f) color[2] = 0.f;
}

inline void ColorClampTruncate(Vector& color)
{
	if (color[0] > 1.0f) color[0] = 1.0f; else if (color[0] < 0.0f) color[0] = 0.0f;
	if (color[1] > 1.0f) color[1] = 1.0f; else if (color[1] < 0.0f) color[1] = 0.0f;
	if (color[2] > 1.0f) color[2] = 1.0f; else if (color[2] < 0.0f) color[2] = 0.0f;
}

void Catmull_Rom_Spline(
	const Vector& p1,
	const Vector& p2,
	const Vector& p3,
	const Vector& p4,
	float t,
	Vector& output);

void Catmull_Rom_Spline_Tangent(
	const Vector& p1,
	const Vector& p2,
	const Vector& p3,
	const Vector& p4,
	float t,
	Vector& output);

void Catmull_Rom_Spline_Integral(
	const Vector& p1,
	const Vector& p2,
	const Vector& p3,
	const Vector& p4,
	float t,
	Vector& output);

void Catmull_Rom_Spline_Integral(
	const Vector& p1,
	const Vector& p2,
	const Vector& p3,
	const Vector& p4,
	Vector& output);

void Catmull_Rom_Spline_Normalize(
	const Vector& p1,
	const Vector& p2,
	const Vector& p3,
	const Vector& p4,
	float t,
	Vector& output);

void Catmull_Rom_Spline_Integral_Normalize(
	const Vector& p1,
	const Vector& p2,
	const Vector& p3,
	const Vector& p4,
	float t,
	Vector& output);

void Catmull_Rom_Spline_NormalizeX(
	const Vector& p1,
	const Vector& p2,
	const Vector& p3,
	const Vector& p4,
	float t,
	Vector& output);

void Catmull_Rom_Spline_NormalizeX(
	const Vector& p1,
	const Vector& p2,
	const Vector& p3,
	const Vector& p4,
	float t,
	Vector& output);

void Hermite_Spline(
	const Vector& p1,
	const Vector& p2,
	const Vector& d1,
	const Vector& d2,
	float t,
	Vector& output);

float Hermite_Spline(
	float p1,
	float p2,
	float d1,
	float d2,
	float t);

void Hermite_Spline(
	const Vector& p0,
	const Vector& p1,
	const Vector& p2,
	float t,
	Vector& output);

float Hermite_Spline(
	float p0,
	float p1,
	float p2,
	float t);


void Hermite_SplineBasis(float t, float basis[]);

void Hermite_Spline(
	const Quaternion& q0,
	const Quaternion& q1,
	const Quaternion& q2,
	float t,
	Quaternion& output);


void Kochanek_Bartels_Spline(
	float tension,
	float bias,
	float continuity,
	const Vector& p1,
	const Vector& p2,
	const Vector& p3,
	const Vector& p4,
	float t,
	Vector& output);

void Kochanek_Bartels_Spline_NormalizeX(
	float tension,
	float bias,
	float continuity,
	const Vector& p1,
	const Vector& p2,
	const Vector& p3,
	const Vector& p4,
	float t,
	Vector& output);

void Cubic_Spline(
	const Vector& p1,
	const Vector& p2,
	const Vector& p3,
	const Vector& p4,
	float t,
	Vector& output);

void Cubic_Spline_NormalizeX(
	const Vector& p1,
	const Vector& p2,
	const Vector& p3,
	const Vector& p4,
	float t,
	Vector& output);

void BSpline(
	const Vector& p1,
	const Vector& p2,
	const Vector& p3,
	const Vector& p4,
	float t,
	Vector& output);

void BSpline_NormalizeX(
	const Vector& p1,
	const Vector& p2,
	const Vector& p3,
	const Vector& p4,
	float t,
	Vector& output);

void Parabolic_Spline(
	const Vector& p1,
	const Vector& p2,
	const Vector& p3,
	const Vector& p4,
	float t,
	Vector& output);

void Parabolic_Spline_NormalizeX(
	const Vector& p1,
	const Vector& p2,
	const Vector& p3,
	const Vector& p4,
	float t,
	Vector& output);

FORCEINLINE float QuinticInterpolatingPolynomial(float t)
{
	return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

void GetInterpolationData(float const* pKnotPositions,
	float const* pKnotValues,
	int nNumValuesinList,
	int nInterpolationRange,
	float flPositionToInterpolateAt,
	bool bWrap,
	float* pValueA,
	float* pValueB,
	float* pInterpolationValue);

float RangeCompressor(float flValue, float flMin, float flMax, float flBase);

float CalcSqrDistanceToAABB(const Vector& mins, const Vector& maxs, const Vector& point);
void CalcClosestPointOnAABB(const Vector& mins, const Vector& maxs, const Vector& point, Vector& closestOut);
void CalcSqrDistAndClosestPointOnAABB(const Vector& mins, const Vector& maxs, const Vector& point, Vector& closestOut, float& distSqrOut);

inline float CalcDistanceToAABB(const Vector& mins, const Vector& maxs, const Vector& point)
{
	float flDistSqr = CalcSqrDistanceToAABB(mins, maxs, point);
	return sqrt(flDistSqr);
}

void  CalcClosestPointOnLine(const Vector& P, const Vector& vLineA, const Vector& vLineB, Vector& vClosest, float* t = 0);
float CalcDistanceToLine(const Vector& P, const Vector& vLineA, const Vector& vLineB, float* t = 0);
float CalcDistanceSqrToLine(const Vector& P, const Vector& vLineA, const Vector& vLineB, float* t = 0);

void  CalcClosestPointOnLineSegment(const Vector& P, const Vector& vLineA, const Vector& vLineB, Vector& vClosest, float* t = 0);
float CalcDistanceToLineSegment(const Vector& P, const Vector& vLineA, const Vector& vLineB, float* t = 0);
float CalcDistanceSqrToLineSegment(const Vector& P, const Vector& vLineA, const Vector& vLineB, float* t = 0);

bool CalcLineToLineIntersectionSegment(
	const Vector& p1, const Vector& p2, const Vector& p3, const Vector& p4, Vector* s1, Vector* s2,
	float* t1, float* t2);

void  CalcClosestPointOnLine2D(Vector2D const& P, Vector2D const& vLineA, Vector2D const& vLineB, Vector2D& vClosest, float* t = 0);
float CalcDistanceToLine2D(Vector2D const& P, Vector2D const& vLineA, Vector2D const& vLineB, float* t = 0);
float CalcDistanceSqrToLine2D(Vector2D const& P, Vector2D const& vLineA, Vector2D const& vLineB, float* t = 0);
void  CalcClosestPointOnLineSegment2D(Vector2D const& P, Vector2D const& vLineA, Vector2D const& vLineB, Vector2D& vClosest, float* t = 0);
float CalcDistanceToLineSegment2D(Vector2D const& P, Vector2D const& vLineA, Vector2D const& vLineB, float* t = 0);
float CalcDistanceSqrToLineSegment2D(Vector2D const& P, Vector2D const& vLineA, Vector2D const& vLineB, float* t = 0);

void MathLib_Init(float gamma = 2.2f, float texGamma = 2.2f, float brightness = 0.0f, int overbright = 2.0f, bool bAllow3DNow = true, bool bAllowSSE = true, bool bAllowSSE2 = true, bool bAllowMMX = true);
bool MathLib_3DNowEnabled(void);
bool MathLib_MMXEnabled(void);
bool MathLib_SSEEnabled(void);
bool MathLib_SSE2Enabled(void);

float Approach(float target, float value, float speed);
float ApproachAngle(float target, float value, float speed);
float AngleDiff(float destAngle, float srcAngle);
float AngleDistance(float next, float cur);
float AngleNormalize(float angle);

float AngleNormalizePositive(float angle);

bool AnglesAreEqual(float a, float b, float tolerance = 0.0f);


void RotationDeltaAxisAngle(const QAngle& srcAngles, const QAngle& destAngles, Vector& deltaAxis, float& deltaAngle);
void RotationDelta(const QAngle& srcAngles, const QAngle& destAngles, QAngle* out);

void ComputeTrianglePlane(const Vector& v1, const Vector& v2, const Vector& v3, Vector& normal, float& intercept);
int PolyFromPlane(Vector* outVerts, const Vector& normal, float dist, float fHalfScale = 9000.0f);
int ClipPolyToPlane(Vector* inVerts, int vertCount, Vector* outVerts, const Vector& normal, float dist, float fOnPlaneEpsilon = 0.1f);
int ClipPolyToPlane_Precise(double* inVerts, int vertCount, double* outVerts, const double* normal, double dist, double fOnPlaneEpsilon = 0.1);

void CalcTriangleTangentSpace(const Vector& p0, const Vector& p1, const Vector& p2,
	const Vector2D& t0, const Vector2D& t1, const Vector2D& t2,
	Vector& sVect, Vector& tVect);

void TransformAABB(const matrix3x4_t& in1, const Vector& vecMinsIn, const Vector& vecMaxsIn, Vector& vecMinsOut, Vector& vecMaxsOut);

void ITransformAABB(const matrix3x4_t& in1, const Vector& vecMinsIn, const Vector& vecMaxsIn, Vector& vecMinsOut, Vector& vecMaxsOut);

void RotateAABB(const matrix3x4_t& in1, const Vector& vecMinsIn, const Vector& vecMaxsIn, Vector& vecMinsOut, Vector& vecMaxsOut);

void IRotateAABB(const matrix3x4_t& in1, const Vector& vecMinsIn, const Vector& vecMaxsIn, Vector& vecMinsOut, Vector& vecMaxsOut);

inline void MatrixTransformPlane(const matrix3x4_t& src, const cplane_t& inPlane, cplane_t& outPlane)
{
	VectorRotate(inPlane.normal, src, outPlane.normal);
	outPlane.dist = inPlane.dist * DotProduct(outPlane.normal, outPlane.normal);
	outPlane.dist += outPlane.normal.x * src[0][3] + outPlane.normal.y * src[1][3] + outPlane.normal.z * src[2][3];
}

inline void MatrixITransformPlane(const matrix3x4_t& src, const cplane_t& inPlane, cplane_t& outPlane)
{
	Vector vecTranslation;
	MatrixGetColumn(src, 3, vecTranslation);

	Vector vecInvTranslation;
	VectorIRotate(vecTranslation, src, vecInvTranslation);

	VectorIRotate(inPlane.normal, src, outPlane.normal);
	outPlane.dist = inPlane.dist * DotProduct(outPlane.normal, outPlane.normal);
	outPlane.dist -= outPlane.normal.x * vecInvTranslation[0] + outPlane.normal.y * vecInvTranslation[1] + outPlane.normal.z * vecInvTranslation[2];
}

int CeilPow2(int in);
int FloorPow2(int in);

FORCEINLINE float* UnpackNormal_HEND3N(const unsigned int* pPackedNormal, float* pNormal)
{
	int temp[3];
	temp[0] = ((*pPackedNormal >> 0L) & 0x7ff);
	if (temp[0] & 0x400)
	{
		temp[0] = 2048 - temp[0];
	}
	temp[1] = ((*pPackedNormal >> 11L) & 0x7ff);
	if (temp[1] & 0x400)
	{
		temp[1] = 2048 - temp[1];
	}
	temp[2] = ((*pPackedNormal >> 22L) & 0x3ff);
	if (temp[2] & 0x200)
	{
		temp[2] = 1024 - temp[2];
	}
	pNormal[0] = (float)temp[0] * 1.0f / 1023.0f;
	pNormal[1] = (float)temp[1] * 1.0f / 1023.0f;
	pNormal[2] = (float)temp[2] * 1.0f / 511.0f;
	return pNormal;
}

FORCEINLINE unsigned int* PackNormal_HEND3N(const float* pNormal, unsigned int* pPackedNormal)
{
	int temp[3];

	temp[0] = Float2Int(pNormal[0] * 1023.0f);
	temp[1] = Float2Int(pNormal[1] * 1023.0f);
	temp[2] = Float2Int(pNormal[2] * 511.0f);

	Assert(temp[0] >= -1023 && temp[0] <= 1023);
	Assert(temp[1] >= -1023 && temp[1] <= 1023);
	Assert(temp[2] >= -511 && temp[2] <= 511);

	*pPackedNormal = ((temp[2] & 0x3ff) << 22L) |
		((temp[1] & 0x7ff) << 11L) |
		((temp[0] & 0x7ff) << 0L);
	return pPackedNormal;
}

FORCEINLINE unsigned int* PackNormal_HEND3N(float nx, float ny, float nz, unsigned int* pPackedNormal)
{
	int temp[3];

	temp[0] = Float2Int(nx * 1023.0f);
	temp[1] = Float2Int(ny * 1023.0f);
	temp[2] = Float2Int(nz * 511.0f);

	Assert(temp[0] >= -1023 && temp[0] <= 1023);
	Assert(temp[1] >= -1023 && temp[1] <= 1023);
	Assert(temp[2] >= -511 && temp[2] <= 511);

	*pPackedNormal = ((temp[2] & 0x3ff) << 22L) |
		((temp[1] & 0x7ff) << 11L) |
		((temp[0] & 0x7ff) << 0L);
	return pPackedNormal;
}

FORCEINLINE float* UnpackNormal_SHORT2(const unsigned int* pPackedNormal, float* pNormal, bool bIsTangent = FALSE)
{
	short iX = (*pPackedNormal & 0x0000FFFF);
	short iY = (*pPackedNormal & 0xFFFF0000) >> 16;

	float zSign = +1;
	if (iX < 0)
	{
		zSign = -1;
		iX = -iX;
	}
	float tSign = +1;
	if (iY < 0)
	{
		tSign = -1;
		iY = -iY;
	}

	pNormal[0] = (iX - 16384.0f) / 16384.0f;
	pNormal[1] = (iY - 16384.0f) / 16384.0f;
	pNormal[2] = zSign * sqrtf(1.0f - (pNormal[0] * pNormal[0] + pNormal[1] * pNormal[1]));
	if (bIsTangent)
	{
		pNormal[3] = tSign;
	}

	return pNormal;
}

FORCEINLINE unsigned int* PackNormal_SHORT2(float nx, float ny, float nz, unsigned int* pPackedNormal, float binormalSign = +1.0f)
{
	nx += 1;					   
	ny += 1;
	nx *= 16384.0f;				     
	ny *= 16384.0f;

	nx = max(nx, 1.0f);		       
	ny = max(ny, 1.0f);
	nx = min(nx, 32767.0f);	       
	ny = min(ny, 32767.0f);

	if (nz < 0.0f)
		nx = -nx;				      

	ny *= binormalSign;			             

	short sX = (short)nx;		   
	short sY = (short)ny;

	*pPackedNormal = (sX & 0x0000FFFF) | (sY << 16);               

	return pPackedNormal;
}

FORCEINLINE unsigned int* PackNormal_SHORT2(const float* pNormal, unsigned int* pPackedNormal, float binormalSign = +1.0f)
{
	return PackNormal_SHORT2(pNormal[0], pNormal[1], pNormal[2], pPackedNormal, binormalSign);
}

FORCEINLINE float* UnpackNormal_UBYTE4(const unsigned int* pPackedNormal, float* pNormal, bool bIsTangent = FALSE)
{
	unsigned char cX, cY;
	if (bIsTangent)
	{
		cX = *pPackedNormal >> 16;					  
		cY = *pPackedNormal >> 24;					  
	}
	else
	{
		cX = *pPackedNormal >> 0;					  
		cY = *pPackedNormal >> 8;					  
	}

	float x = cX - 128.0f;
	float y = cY - 128.0f;
	float z;

	float zSignBit = x < 0 ? 1.0f : 0.0f;			         
	float tSignBit = y < 0 ? 1.0f : 0.0f;
	float zSign = -(2 * zSignBit - 1);			    
	float tSign = -(2 * tSignBit - 1);

	x = x * zSign - zSignBit;							 
	y = y * tSign - tSignBit;
	x = x - 64;										 
	y = y - 64;

	float xSignBit = x < 0 ? 1.0f : 0.0f;	         
	float ySignBit = y < 0 ? 1.0f : 0.0f;
	float xSign = -(2 * xSignBit - 1);			    
	float ySign = -(2 * ySignBit - 1);

	x = (x * xSign - xSignBit) / 63.0f;				  
	y = (y * ySign - ySignBit) / 63.0f;
	z = 1.0f - x - y;

	float oolen = 1.0f / sqrt(x * x + y * y + z * z);	  
	x *= oolen * xSign;					  
	y *= oolen * ySign;
	z *= oolen * zSign;

	pNormal[0] = x;
	pNormal[1] = y;
	pNormal[2] = z;
	if (bIsTangent)
	{
		pNormal[3] = tSign;
	}

	return pNormal;
}

FORCEINLINE unsigned int* PackNormal_UBYTE4(float nx, float ny, float nz, unsigned int* pPackedNormal, bool bIsTangent = false, float binormalSign = +1.0f)
{
	float xSign = nx < 0.0f ? -1.0f : 1.0f;			    
	float ySign = ny < 0.0f ? -1.0f : 1.0f;
	float zSign = nz < 0.0f ? -1.0f : 1.0f;
	float tSign = binormalSign;
	Assert((binormalSign == +1.0f) || (binormalSign == -1.0f));

	float xSignBit = 0.5f * (1 - xSign);			   
	float ySignBit = 0.5f * (1 - ySign);			       
	float zSignBit = 0.5f * (1 - zSign);
	float tSignBit = 0.5f * (1 - binormalSign);

	float absX = xSign * nx;							   
	float absY = ySign * ny;
	float absZ = zSign * nz;

	float xbits = absX / (absX + absY + absZ);	    
	float ybits = absY / (absX + absY + absZ);

	xbits *= 63;									 
	ybits *= 63;

	xbits = xbits * xSign - xSignBit;				  
	ybits = ybits * ySign - ySignBit;
	xbits += 64.0f;									  
	ybits += 64.0f;

	xbits = xbits * zSign - zSignBit;				      
	ybits = ybits * tSign - tSignBit;				  

	xbits += 128.0f;								  
	ybits += 128.0f;

	unsigned char cX = (unsigned char)xbits;
	unsigned char cY = (unsigned char)ybits;

	if (!bIsTangent)
		*pPackedNormal = (cX << 0) | (cY << 8);	   
	else
		*pPackedNormal = (cX << 16) | (cY << 24);	   

	return pPackedNormal;
}

FORCEINLINE unsigned int* PackNormal_UBYTE4(const float* pNormal, unsigned int* pPackedNormal, bool bIsTangent = false, float binormalSign = +1.0f)
{
	return PackNormal_UBYTE4(pNormal[0], pNormal[1], pNormal[2], pPackedNormal, bIsTangent, binormalSign);
}


void RGBtoHSV(const Vector& rgb, Vector& hsv);


void HSVtoRGB(const Vector& hsv, Vector& rgb);


float FastLog2(float i);			   
float FastPow2(float i);			 
float FastPow(float a, float b);	 
float FastPow10(float i);			 

inline bool CloseEnough(float a, float b, float epsilon = EQUAL_EPSILON)
{
	return fabs(a - b) <= epsilon;
}

inline bool CloseEnough(const Vector& a, const Vector& b, float epsilon = EQUAL_EPSILON)
{
	return fabs(a.x - b.x) <= epsilon &&
		fabs(a.y - b.y) <= epsilon &&
		fabs(a.z - b.z) <= epsilon;
}

bool AlmostEqual(float a, float b, int maxUlps = 10);

inline bool AlmostEqual(const Vector& a, const Vector& b, int maxUlps = 10)
{
	return AlmostEqual(a.x, b.x, maxUlps) &&
		AlmostEqual(a.y, b.y, maxUlps) &&
		AlmostEqual(a.z, b.z, maxUlps);
}


#endif	 