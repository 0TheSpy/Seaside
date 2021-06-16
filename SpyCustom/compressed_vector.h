#ifndef COMPRESSED_VECTOR_H
#define COMPRESSED_VECTOR_H

#ifdef _WIN32
#pragma once
#endif

#include <math.h>
#include <float.h>

#include "basetypes.h"

#include <stdlib.h>

#include "dbg.h"
#include "vector.h"

#include "mathlib.h"

#if defined( _X360 )
#pragma bitfield_order( push, lsb_to_msb )
#endif
class Vector32
{
public:
	Vector32(void);
	Vector32(vec_t X, vec_t Y, vec_t Z);

	Vector32& operator=(const Vector& vOther);
	operator Vector ();

private:
	unsigned short x : 10;
	unsigned short y : 10;
	unsigned short z : 10;
	unsigned short exp : 2;
};

inline Vector32& Vector32::operator=(const Vector& vOther)
{
	CHECK_VALID(vOther);

	static float expScale[4] = { 4.0f, 16.0f, 32.f, 64.f };

	float fmax = Max(fabs(vOther.x), fabs(vOther.y));
	fmax = Max(fmax, (float)fabs(vOther.z));

	for (exp = 0; exp < 3; exp++)
	{
		if (fmax < expScale[exp])
			break;
	}
	Assert(fmax < expScale[exp]);

	float fexp = 512.0f / expScale[exp];

	x = Clamp((int)(vOther.x * fexp) + 512, 0, 1023);
	y = Clamp((int)(vOther.y * fexp) + 512, 0, 1023);
	z = Clamp((int)(vOther.z * fexp) + 512, 0, 1023);
	return *this;
}


inline Vector32::operator Vector ()
{
	Vector tmp;

	static float expScale[4] = { 4.0f, 16.0f, 32.f, 64.f };

	float fexp = expScale[exp] / 512.0f;

	tmp.x = (((int)x) - 512) * fexp;
	tmp.y = (((int)y) - 512) * fexp;
	tmp.z = (((int)z) - 512) * fexp;
	return tmp;
}


class Normal32
{
public:
	Normal32(void);
	Normal32(vec_t X, vec_t Y, vec_t Z);

	Normal32& operator=(const Vector& vOther);
	operator Vector ();

private:
	unsigned short x : 15;
	unsigned short y : 15;
	unsigned short zneg : 1;
};


inline Normal32& Normal32::operator=(const Vector& vOther)
{
	CHECK_VALID(vOther);

	x = Clamp((int)(vOther.x * 16384) + 16384, 0, 32767);
	y = Clamp((int)(vOther.y * 16384) + 16384, 0, 32767);
	zneg = (vOther.z < 0);
	return *this;
}


inline Normal32::operator Vector ()
{
	Vector tmp;

	tmp.x = ((int)x - 16384) * (1 / 16384.0);
	tmp.y = ((int)y - 16384) * (1 / 16384.0);
	tmp.z = sqrt(1 - tmp.x * tmp.x - tmp.y * tmp.y);
	if (zneg)
		tmp.z = -tmp.z;
	return tmp;
}


class Quaternion64
{
public:
	Quaternion64(void);
	Quaternion64(vec_t X, vec_t Y, vec_t Z);

	Quaternion64& operator=(const Quaternion& vOther);
	operator Quaternion ();
private:
	uint64 x : 21;
	uint64 y : 21;
	uint64 z : 21;
	uint64 wneg : 1;
};


inline Quaternion64::operator Quaternion ()
{
	Quaternion tmp;

	tmp.x = ((int)x - 1048576) * (1 / 1048576.5f);
	tmp.y = ((int)y - 1048576) * (1 / 1048576.5f);
	tmp.z = ((int)z - 1048576) * (1 / 1048576.5f);
	tmp.w = sqrt(1 - tmp.x * tmp.x - tmp.y * tmp.y - tmp.z * tmp.z);
	if (wneg)
		tmp.w = -tmp.w;
	return tmp;
}

inline Quaternion64& Quaternion64::operator=(const Quaternion& vOther)
{
	CHECK_VALID(vOther);

	x = Clamp((int)(vOther.x * 1048576) + 1048576, 0, 2097151);
	y = Clamp((int)(vOther.y * 1048576) + 1048576, 0, 2097151);
	z = Clamp((int)(vOther.z * 1048576) + 1048576, 0, 2097151);
	wneg = (vOther.w < 0);
	return *this;
}

class Quaternion48
{
public:
	Quaternion48(void);
	Quaternion48(vec_t X, vec_t Y, vec_t Z);

	Quaternion48& operator=(const Quaternion& vOther);
	operator Quaternion ();
private:
	unsigned short x : 16;
	unsigned short y : 16;
	unsigned short z : 15;
	unsigned short wneg : 1;
};


inline Quaternion48::operator Quaternion ()
{
	Quaternion tmp;

	tmp.x = ((int)x - 32768) * (1 / 32768.0);
	tmp.y = ((int)y - 32768) * (1 / 32768.0);
	tmp.z = ((int)z - 16384) * (1 / 16384.0);
	tmp.w = sqrt(1 - tmp.x * tmp.x - tmp.y * tmp.y - tmp.z * tmp.z);
	if (wneg)
		tmp.w = -tmp.w;
	return tmp;
}

inline Quaternion48& Quaternion48::operator=(const Quaternion& vOther)
{
	CHECK_VALID(vOther);

	x = Clamp((int)(vOther.x * 32768) + 32768, 0, 65535);
	y = Clamp((int)(vOther.y * 32768) + 32768, 0, 65535);
	z = Clamp((int)(vOther.z * 16384) + 16384, 0, 32767);
	wneg = (vOther.w < 0);
	return *this;
}

class Quaternion32
{
public:
	Quaternion32(void);
	Quaternion32(vec_t X, vec_t Y, vec_t Z);

	Quaternion32& operator=(const Quaternion& vOther);
	operator Quaternion ();
private:
	unsigned int x : 11;
	unsigned int y : 10;
	unsigned int z : 10;
	unsigned int wneg : 1;
};


inline Quaternion32::operator Quaternion ()
{
	Quaternion tmp;

	tmp.x = ((int)x - 1024) * (1 / 1024.0);
	tmp.y = ((int)y - 512) * (1 / 512.0);
	tmp.z = ((int)z - 512) * (1 / 512.0);
	tmp.w = sqrt(1 - tmp.x * tmp.x - tmp.y * tmp.y - tmp.z * tmp.z);
	if (wneg)
		tmp.w = -tmp.w;
	return tmp;
}

inline Quaternion32& Quaternion32::operator=(const Quaternion& vOther)
{
	CHECK_VALID(vOther);

	x = Clamp((int)(vOther.x * 1024) + 1024, 0, 2047);
	y = Clamp((int)(vOther.y * 512) + 512, 0, 1023);
	z = Clamp((int)(vOther.z * 512) + 512, 0, 1023);
	wneg = (vOther.w < 0);
	return *this;
}


const int float32bias = 127;
const int float16bias = 15;

const float maxfloat16bits = 65504.0f;

class float16
{
public:
	void Init() { m_storage.rawWord = 0; }
	unsigned short GetBits() const
	{
		return m_storage.rawWord;
	}
	float GetFloat() const
	{
		return Convert16bitFloatTo32bits(m_storage.rawWord);
	}
	void SetFloat(float in)
	{
		m_storage.rawWord = ConvertFloatTo16bits(in);
	}

	bool IsInfinity() const
	{
		return m_storage.bits.biased_exponent == 31 && m_storage.bits.mantissa == 0;
	}
	bool IsNaN() const
	{
		return m_storage.bits.biased_exponent == 31 && m_storage.bits.mantissa != 0;
	}

	bool operator==(const float16 other) const { return m_storage.rawWord == other.m_storage.rawWord; }
	bool operator!=(const float16 other) const { return m_storage.rawWord != other.m_storage.rawWord; }

protected:
	union float32bits
	{
		float rawFloat;
		struct
		{
			unsigned int mantissa : 23;
			unsigned int biased_exponent : 8;
			unsigned int sign : 1;
		} bits;
	};

	union float16bits
	{
		unsigned short rawWord;
		struct
		{
			unsigned short mantissa : 10;
			unsigned short biased_exponent : 5;
			unsigned short sign : 1;
		} bits;
	};

	static bool IsNaN(float16bits in)
	{
		return in.bits.biased_exponent == 31 && in.bits.mantissa != 0;
	}
	static bool IsInfinity(float16bits in)
	{
		return in.bits.biased_exponent == 31 && in.bits.mantissa == 0;
	}

	static unsigned short ConvertFloatTo16bits(float input)
	{
		if (input > maxfloat16bits)
			input = maxfloat16bits;
		else if (input < -maxfloat16bits)
			input = -maxfloat16bits;

		float16bits output;
		float32bits inFloat;

		inFloat.rawFloat = input;

		output.bits.sign = inFloat.bits.sign;

		if ((inFloat.bits.biased_exponent == 0) && (inFloat.bits.mantissa == 0))
		{
			output.bits.mantissa = 0;
			output.bits.biased_exponent = 0;
		}
		else if ((inFloat.bits.biased_exponent == 0) && (inFloat.bits.mantissa != 0))
		{
			output.bits.mantissa = 0;
			output.bits.biased_exponent = 0;
		}
		else if ((inFloat.bits.biased_exponent == 0xff) && (inFloat.bits.mantissa == 0))
		{
#if 0
			output.bits.mantissa = 0;
			output.bits.biased_exponent = 31;
#else
			output.bits.mantissa = 0x3ff;
			output.bits.biased_exponent = 0x1e;
#endif
		}
		else if ((inFloat.bits.biased_exponent == 0xff) && (inFloat.bits.mantissa != 0))
		{
#if 0
			output.bits.mantissa = 1;
			output.bits.biased_exponent = 31;
#else
			output.bits.mantissa = 0;
			output.bits.biased_exponent = 0;
#endif
		}
		else
		{
			int new_exp = inFloat.bits.biased_exponent - 127;

			if (new_exp < -24)
			{
				output.bits.mantissa = 0;
				output.bits.biased_exponent = 0;
			}

			if (new_exp < -14)
			{
				output.bits.biased_exponent = 0;
				unsigned int exp_val = (unsigned int)(-14 - (inFloat.bits.biased_exponent - float32bias));
				if (exp_val > 0 && exp_val < 11)
				{
					output.bits.mantissa = (1 << (10 - exp_val)) + (inFloat.bits.mantissa >> (13 + exp_val));
				}
			}
			else if (new_exp > 15)
			{
#if 0
				output.bits.mantissa = 0;
				output.bits.biased_exponent = 31;
#else
				output.bits.mantissa = 0x3ff;
				output.bits.biased_exponent = 0x1e;
#endif
			}
			else
			{
				output.bits.biased_exponent = new_exp + 15;
				output.bits.mantissa = (inFloat.bits.mantissa >> 13);
			}
		}
		return output.rawWord;
	}

	static float Convert16bitFloatTo32bits(unsigned short input)
	{
		float32bits output;
		const float16bits& inFloat = *((float16bits*)&input);

		if (IsInfinity(inFloat))
		{
			return maxfloat16bits * ((inFloat.bits.sign == 1) ? -1.0f : 1.0f);
		}
		if (IsNaN(inFloat))
		{
			return 0.0;
		}
		if (inFloat.bits.biased_exponent == 0 && inFloat.bits.mantissa != 0)
		{
			const float half_denorm = (1.0f / 16384.0f);  
			float mantissa = ((float)(inFloat.bits.mantissa)) / 1024.0f;
			float sgn = (inFloat.bits.sign) ? -1.0f : 1.0f;
			output.rawFloat = sgn * mantissa * half_denorm;
		}
		else
		{
			unsigned mantissa = inFloat.bits.mantissa;
			unsigned biased_exponent = inFloat.bits.biased_exponent;
			unsigned sign = ((unsigned)inFloat.bits.sign) << 31;
			biased_exponent = ((biased_exponent - float16bias + float32bias) * (biased_exponent != 0)) << 23;
			mantissa <<= (23 - 10);

			*((unsigned*)&output) = (mantissa | biased_exponent | sign);
		}

		return output.rawFloat;
	}


	float16bits m_storage;
};

class float16_with_assign : public float16
{
public:
	float16_with_assign() {}
	float16_with_assign(float f) { m_storage.rawWord = ConvertFloatTo16bits(f); }

	float16& operator=(const float16& other) { m_storage.rawWord = ((float16_with_assign&)other).m_storage.rawWord; return *this; }
	float16& operator=(const float& other) { m_storage.rawWord = ConvertFloatTo16bits(other); return *this; }
	operator float() const { return Convert16bitFloatTo32bits(m_storage.rawWord); }
};

class Vector48
{
public:
	Vector48(void) {}
	Vector48(vec_t X, vec_t Y, vec_t Z) { x.SetFloat(X); y.SetFloat(Y); z.SetFloat(Z); }

	Vector48& operator=(const Vector& vOther);
	operator Vector ();

	const float operator[](int i) const { return (((float16*)this)[i]).GetFloat(); }

	float16 x;
	float16 y;
	float16 z;
};

inline Vector48& Vector48::operator=(const Vector& vOther)
{
	CHECK_VALID(vOther);

	x.SetFloat(vOther.x);
	y.SetFloat(vOther.y);
	z.SetFloat(vOther.z);
	return *this;
}


inline Vector48::operator Vector ()
{
	Vector tmp;

	tmp.x = x.GetFloat();
	tmp.y = y.GetFloat();
	tmp.z = z.GetFloat();

	return tmp;
}

class Vector2d32
{
public:
	Vector2d32(void) {}
	Vector2d32(vec_t X, vec_t Y) { x.SetFloat(X); y.SetFloat(Y); }

	Vector2d32& operator=(const Vector& vOther);
	Vector2d32& operator=(const Vector2D& vOther);

	operator Vector2D ();

	void Init(vec_t ix = 0.f, vec_t iy = 0.f);

	float16_with_assign x;
	float16_with_assign y;
};

inline Vector2d32& Vector2d32::operator=(const Vector2D& vOther)
{
	x.SetFloat(vOther.x);
	y.SetFloat(vOther.y);
	return *this;
}

inline Vector2d32::operator Vector2D ()
{
	Vector2D tmp;

	tmp.x = x.GetFloat();
	tmp.y = y.GetFloat();

	return tmp;
}

inline void Vector2d32::Init(vec_t ix, vec_t iy)
{
	x.SetFloat(ix);
	y.SetFloat(iy);
}

#if defined( _X360 )
#pragma bitfield_order( pop )
#endif

#endif
