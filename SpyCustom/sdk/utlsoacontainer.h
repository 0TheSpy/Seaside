#ifndef UTLSOACONTAINER_H
#define UTLSOACONTAINER_H

#ifdef _WIN32
#pragma once
#endif


#include "platform.h"
#include "dbg.h"
#include "threadtools.h"
#include "utlmemory.h"
#include "utlblockmemory.h"
#include "ssemath.h"


template<class T> class CStridedPtr
{
protected:
	T* m_pData;
	size_t m_nStride;

public:
	FORCEINLINE CStridedPtr<T>(void* pData, size_t nByteStride)
	{
		m_pData = reinterpret_cast<T*>(pData);
		m_nStride = nByteStride / sizeof(T);
	}

	FORCEINLINE CStridedPtr<T>(void) {}
	T* operator->(void) const
	{
		return m_pData;
	}

	T& operator*(void) const
	{
		return *m_pData;
	}

	FORCEINLINE operator T* (void)
	{
		return m_pData;
	}

	FORCEINLINE CStridedPtr<T>& operator++(void)
	{
		m_pData += m_nStride;
		return *this;
	}

	FORCEINLINE void operator+=(size_t nNumElements)
	{
		m_pData += nNumElements * m_nStride;
	}

};

template<class T> class CStridedConstPtr
{
protected:
	const T* m_pData;
	size_t m_nStride;

public:
	FORCEINLINE CStridedConstPtr<T>(void const* pData, size_t nByteStride)
	{
		m_pData = reinterpret_cast<T const*>(pData);
		m_nStride = nByteStride / sizeof(T);
	}

	FORCEINLINE CStridedConstPtr<T>(void) {}

	const T* operator->(void) const
	{
		return m_pData;
	}

	const T& operator*(void) const
	{
		return *m_pData;
	}

	FORCEINLINE operator const T* (void) const
	{
		return m_pData;
	}

	FORCEINLINE CStridedConstPtr<T>& operator++(void)
	{
		m_pData += m_nStride;
		return *this;
	}
	FORCEINLINE void operator+=(size_t nNumElements)
	{
		m_pData += nNumElements * m_nStride;
	}
};

enum EAttributeDataType
{
	ATTRDATATYPE_FLOAT = 0,									   
	ATTRDATATYPE_4V = 1,									       
	ATTRDATATYPE_INT = 2,									     
															ATTRDATATYPE_POINTER = 3,								  
															ATTRDATATYPE_NONE = -1,									    
};

#define MAX_SOA_FIELDS 32

class CSOAContainer
{

protected:
	int m_nColumns;											       
	int m_nRows;
	int m_nSlices;

	int m_nPaddedColumns;									       
	int m_nNumQuadsPerRow;									        

	uint8* m_pDataMemory;									    
	uint8* m_pAttributePtrs[MAX_SOA_FIELDS];

	EAttributeDataType m_nDataType[MAX_SOA_FIELDS];

	size_t m_nStrideInBytes[MAX_SOA_FIELDS];			         
	size_t m_nRowStrideInBytes[MAX_SOA_FIELDS];			           
	size_t m_nSliceStrideInBytes[MAX_SOA_FIELDS];                  



	uint32 m_nFieldPresentMask;

	FORCEINLINE void Init(void)
	{
		memset(m_nDataType, 0xff, sizeof(m_nDataType));
		m_pDataMemory = 0;
		m_nColumns = m_nPaddedColumns = m_nRows = m_nSlices = 0;
		m_nFieldPresentMask = 0;
	}
public:


	CSOAContainer(void)									      
	{
		Init();
	}

	void Purge(void);										       

	~CSOAContainer(void);

	CSOAContainer(int nCols, int nRows, ...);

	size_t ElementSize(void) const;					       

	FORCEINLINE void SetAttributeType(int nAttrIdx, EAttributeDataType nDataType, bool bAllocateMemory = true)
	{
		Assert(!m_pDataMemory);							     
		Assert(nAttrIdx < MAX_SOA_FIELDS);
		m_nDataType[nAttrIdx] = nDataType;
		if ((m_nDataType[nAttrIdx] != ATTRDATATYPE_NONE) && bAllocateMemory)
			m_nFieldPresentMask |= (1 << nAttrIdx);
		else
			m_nFieldPresentMask &= ~(1 << nAttrIdx);
	}

	FORCEINLINE int NumRows(void) const
	{
		return m_nRows;
	}

	FORCEINLINE int NumCols(void) const
	{
		return m_nColumns;
	}
	FORCEINLINE int NumSlices(void) const
	{
		return m_nSlices;
	}


	FORCEINLINE void AssertDataType(int nAttrIdx, EAttributeDataType nDataType) const
	{
		Assert(nAttrIdx >= 0);
		Assert(nAttrIdx < MAX_SOA_FIELDS);
		Assert(m_nStrideInBytes[nAttrIdx]);
	}


	FORCEINLINE int NumQuadsPerRow(void) const
	{
		return m_nNumQuadsPerRow;
	}

	FORCEINLINE int Count(void) const						   
	{
		return NumCols();
	}

	FORCEINLINE int NumElements(void) const
	{
		return NumCols() * NumRows() * NumSlices();
	}


	FORCEINLINE size_t RowToRowStep(int nAttrIdx) const
	{
		return 0;
	}

	FORCEINLINE void* RowPtr(int nAttributeIdx, int nRowNumber, int nSliceNumber = 0) const
	{
		Assert(nRowNumber < m_nRows);
		Assert(nAttributeIdx < MAX_SOA_FIELDS);
		Assert(m_nDataType[nAttributeIdx] != ATTRDATATYPE_NONE);
		Assert(m_nFieldPresentMask & (1 << nAttributeIdx));
		return m_pAttributePtrs[nAttributeIdx] +
			+nRowNumber * m_nRowStrideInBytes[nAttributeIdx]
			+ nSliceNumber * m_nSliceStrideInBytes[nAttributeIdx];
	}

	FORCEINLINE void const* ConstRowPtr(int nAttributeIdx, int nRowNumber, int nSliceNumber = 0) const
	{
		Assert(nRowNumber < m_nRows);
		Assert(nAttributeIdx < MAX_SOA_FIELDS);
		Assert(m_nDataType[nAttributeIdx] != ATTRDATATYPE_NONE);
		return m_pAttributePtrs[nAttributeIdx]
			+ nRowNumber * m_nRowStrideInBytes[nAttributeIdx]
			+ nSliceNumber * m_nSliceStrideInBytes[nAttributeIdx];
	}


	template<class T> FORCEINLINE T* ElementPointer(int nAttributeIdx,
		int nX = 0, int nY = 0, int nZ = 0) const
	{
		Assert(nAttributeIdx < MAX_SOA_FIELDS);
		Assert(nX < m_nColumns);
		Assert(nY < m_nRows);
		Assert(nZ < m_nSlices);
		Assert(m_nDataType[nAttributeIdx] != ATTRDATATYPE_NONE);
		Assert(m_nDataType[nAttributeIdx] != ATTRDATATYPE_4V);
		return reinterpret_cast<T*>(m_pAttributePtrs[nAttributeIdx]
			+ nX * sizeof(float)
			+ nY * m_nRowStrideInBytes[nAttributeIdx]
			+ nZ * m_nSliceStrideInBytes[nAttributeIdx]
			);
	}

	FORCEINLINE size_t ItemByteStride(int nAttributeIdx) const
	{
		Assert(nAttributeIdx < MAX_SOA_FIELDS);
		Assert(m_nDataType[nAttributeIdx] != ATTRDATATYPE_NONE);
		return m_nStrideInBytes[nAttributeIdx];
	}

	void CopyAttrFrom(CSOAContainer const& other, int nAttributeIdx);

	void CopyAttrToAttr(int nSrcAttributeIndex, int nDestAttributeIndex);

	FORCEINLINE void MoveDataFrom(CSOAContainer other)
	{
		(*this) = other;
		other.Init();
	}



	void AllocateData(int nNCols, int nNRows, int nSlices = 1);          

	void RandomizeAttribute(int nAttr, float flMin, float flMax) const;

	void FillAttrWithInterpolatedValues(int nAttr, float flValue00, float flValue10, float flValue01, float flValue11) const;
	void FillAttrWithInterpolatedValues(int nAttr, Vector flValue00, Vector flValue10,
		Vector const& flValue01, Vector const& flValue11) const;

};

class CFltX4AttributeIterator : public CStridedConstPtr<fltx4>
{
	FORCEINLINE CFltX4AttributeIterator(CSOAContainer const* pContainer, int nAttribute, int nRowNumber = 0)
		: CStridedConstPtr<fltx4>(pContainer->ConstRowPtr(nAttribute, nRowNumber),
			pContainer->ItemByteStride(nAttribute))
	{
	}
};

class CFltX4AttributeWriteIterator : public CStridedPtr<fltx4>
{
	FORCEINLINE CFltX4AttributeWriteIterator(CSOAContainer const* pContainer, int nAttribute, int nRowNumber = 0)
		: CStridedPtr<fltx4>(pContainer->RowPtr(nAttribute, nRowNumber),
			pContainer->ItemByteStride(nAttribute))
	{
	}

};


#endif