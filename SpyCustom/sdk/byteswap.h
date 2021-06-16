#ifndef BYTESWAP_H
#define BYTESWAP_H
#if defined(_WIN32)
#pragma once
#endif

#include "datamap.h"	          

class CByteswap
{
public:
	CByteswap()
	{
		SetTargetBigEndian(IsMachineBigEndian());
	}

	void SwapFieldToTargetEndian(void* pOutputBuffer, void* pData, typedescription_t* pField);

	void SwapFieldsToTargetEndian(void* pOutputBuffer, void* pBaseData, datamap_t* pDataMap);

	template<typename T> inline void SwapFieldsToTargetEndian(T* pOutputBuffer, void* pBaseData, unsigned int objectCount = 1)
	{
		for (unsigned int i = 0; i < objectCount; ++i, ++pOutputBuffer)
		{
			SwapFieldsToTargetEndian((void*)pOutputBuffer, pBaseData, &T::m_DataMap);
			pBaseData = (byte*)pBaseData + sizeof(T);
		}
	}

	template<typename T> inline void SwapFieldsToTargetEndian(T* pOutputBuffer, unsigned int objectCount = 1)
	{
		SwapFieldsToTargetEndian<T>(pOutputBuffer, (void*)pOutputBuffer, objectCount);
	}

	static bool IsMachineBigEndian()
	{
		short nIsBigEndian = 1;

		return (bool)(0 == *(char*)&nIsBigEndian);
	}

	inline void SetTargetBigEndian(bool bigEndian)
	{
		m_bBigEndian = bigEndian;
		m_bSwapBytes = IsMachineBigEndian() != bigEndian;
	}

	inline void FlipTargetEndian(void)
	{
		m_bSwapBytes = !m_bSwapBytes;
		m_bBigEndian = !m_bBigEndian;
	}

	inline void ActivateByteSwapping(bool bActivate)
	{
		SetTargetBigEndian(IsMachineBigEndian() != bActivate);
	}

	inline bool IsSwappingBytes(void)	    
	{
		return m_bSwapBytes;
	}

	inline bool IsTargetBigEndian(void)	      
	{
		return m_bBigEndian;
	}

	template<typename T> inline int SourceIsNativeEndian(T input, T nativeConstant)
	{
		if (input == nativeConstant)
			return 1;

		int output;
		LowLevelByteSwap<T>(&output, &input);
		if (output == nativeConstant)
			return 0;

		assert(0);		              
		return -1;
	}

	template<typename T> inline void SwapBuffer(T* outputBuffer, T* inputBuffer = NULL, int count = 1)
	{
		assert(count >= 0);
		assert(outputBuffer);

		if (count <= 0 || !outputBuffer)
			return;

		if (inputBuffer == NULL)
		{
			inputBuffer = outputBuffer;
		}

		for (int i = 0; i < count; i++)
		{
			LowLevelByteSwap<T>(&outputBuffer[i], &inputBuffer[i]);
		}
	}

	template<typename T> inline void SwapBufferToTargetEndian(T* outputBuffer, T* inputBuffer = NULL, int count = 1)
	{
		assert(count >= 0);
		assert(outputBuffer);

		if (count <= 0 || !outputBuffer)
			return;

		if (inputBuffer == NULL)
		{
			inputBuffer = outputBuffer;
		}

		if (!m_bSwapBytes || (sizeof(T) == 1))
		{
			if (!inputBuffer)
				return;

			memcpy(outputBuffer, inputBuffer, count * sizeof(T));
			return;

		}

		for (int i = 0; i < count; i++)
		{
			LowLevelByteSwap<T>(&outputBuffer[i], &inputBuffer[i]);
		}
	}

private:
	template<typename T> static void LowLevelByteSwap(T* output, T* input)
	{
		T temp = *output;
#if defined( _X360 )
		DWORD* word = (DWORD*)input;
		switch (sizeof(T))
		{
		case 8:
		{
			__storewordbytereverse(*word, 0, &temp);
			__storewordbytereverse(*(word + 1), 4, &temp);
		}
		break;

		case 4:
			__storewordbytereverse(*word, 0, &temp);
			break;

		case 2:
			__storeshortbytereverse(*input, 0, &temp);
			break;

		default:
			Assert("Invalid size in CByteswap::LowLevelByteSwap" && 0);
		}
#else
		for (int i = 0; i < sizeof(T); i++)
		{
			((unsigned char*)&temp)[i] = ((unsigned char*)input)[sizeof(T) - (i + 1)];
		}
#endif
		Q_memcpy(output, &temp, sizeof(T));
	}

	unsigned int m_bSwapBytes : 1;
	unsigned int m_bBigEndian : 1;
};

#endif   