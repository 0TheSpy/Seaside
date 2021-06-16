#ifndef IMESH_H
#define IMESH_H

#ifdef _WIN32
#pragma once
#endif

#include "interface.h"
#include "imaterial.h"
#include <float.h>
#include <string.h>
#include "dbg.h"
#include "meshutils.h"

#include "imaterialsystem.h" 

#ifdef OSX
#endif

class IMaterial;
class CMeshBuilder;
class IMaterialVar;
typedef uint64 VertexFormat_t;
struct ShaderStencilState_t;


#ifdef _DEBUG
#endif


enum
{
	VERTEX_MAX_TEXTURE_COORDINATES = 8,
	BONE_MATRIX_INDEX_INVALID = 255
};

enum
{
	INDEX_BUFFER_SIZE = 32768,
	DYNAMIC_VERTEX_BUFFER_MEMORY = (1024 + 512) * 1024,
	DYNAMIC_VERTEX_BUFFER_MEMORY_SMALL = 384 * 1024,        
};

enum WriteCombineOrdering_t
{
	MB_FIELD_NONE = -1,
	MB_FIELD_POSITION = 0,
	MB_FIELD_BONE_WEIGHTS,
	MB_FIELD_BONE_INDEX,
	MB_FIELD_NORMAL,
	MB_FIELD_COLOR,
	MB_FIELD_SPECULAR,
	MB_FIELD_TEXCOORD_FIRST,
	MB_FIELD_TEXCOORD_LAST = MB_FIELD_TEXCOORD_FIRST + VERTEX_MAX_TEXTURE_COORDINATES - 1,
	MB_FIELD_TANGENT_S,
	MB_FIELD_TANGENT_T,
	MB_FIELD_USERDATA,
};

#define MB_FIELD_TEXCOORD( nStage ) ( MB_FIELD_TEXCOORD_FIRST + ( nStage ) )

struct VertexDesc_t
{
	int	m_VertexSize_Position;
	int m_VertexSize_BoneWeight;
	int m_VertexSize_BoneMatrixIndex;
	int	m_VertexSize_Normal;
	int	m_VertexSize_Color;
	int	m_VertexSize_Specular;
	int m_VertexSize_TexCoord[VERTEX_MAX_TEXTURE_COORDINATES];
	int m_VertexSize_TangentS;
	int m_VertexSize_TangentT;
	int m_VertexSize_Wrinkle;

	int m_VertexSize_UserData;

	int m_ActualVertexSize;	          
	VertexCompressionType_t m_CompressionType;

	int m_NumBoneWeights;

	float* m_pPosition;

	float* m_pBoneWeight;

#ifndef NEW_SKINNING
	unsigned char* m_pBoneMatrixIndex;
#else
	float* m_pBoneMatrixIndex;
#endif

	float* m_pNormal;

	unsigned char* m_pColor;
	unsigned char* m_pSpecular;
	float* m_pTexCoord[VERTEX_MAX_TEXTURE_COORDINATES];

	float* m_pTangentS;
	float* m_pTangentT;

	float* m_pWrinkle;

	float* m_pUserData;

	int	m_nFirstVertex;

	unsigned int	m_nOffset;

#ifdef DEBUG_WRITE_COMBINE
	int m_nLastWrittenField;
	unsigned char* m_pLastWrittenAddress;
#endif
};

struct IndexDesc_t
{
	unsigned short* m_pIndices;

	unsigned int	m_nOffset;

	unsigned int	m_nFirstIndex;

	unsigned int	m_nIndexSize;
};


struct MeshDesc_t : public VertexDesc_t, public IndexDesc_t
{
};


struct ModelVertexDX8_t
{
	Vector			m_vecPosition;
	Vector			m_vecNormal;
	Vector2D		m_vecTexCoord;
	Vector4D		m_vecUserData;
};

struct QuadTessVertex_t
{
	Vector4D	m_vTangent;		        
	Vector4D	m_vUV01;		           
	Vector4D	m_vUV23;		           
};

struct MeshBoneRemap_t     
{
	DECLARE_BYTESWAP_DATADESC();
	int m_nActualBoneIndex;
	int m_nSrcBoneIndex;
};

struct MeshInstanceData_t
{
	int						m_nIndexOffset;
	int						m_nIndexCount;
	int						m_nBoneCount;
	MeshBoneRemap_t* m_pBoneRemap;		            
	matrix3x4_t* m_pPoseToWorld;	                
	const ITexture* m_pEnvCubemap;
	MaterialLightingState_t* m_pLightingState;
	MaterialPrimitiveType_t m_nPrimType;
	const IVertexBuffer* m_pVertexBuffer;
	int						m_nVertexOffsetInBytes;
	const IIndexBuffer* m_pIndexBuffer;
	const IVertexBuffer* m_pColorBuffer;
	int						m_nColorVertexOffsetInBytes;
	ShaderStencilState_t* m_pStencilState;
	Vector4D				m_DiffuseModulation;
};

struct MeshBuffersAllocationSettings_t
{
	uint32 m_uiIbUsageFlags;
};

inline float* OffsetFloatPointer(float* pBufferPointer, int nVertexCount, int vertexSize)
{
	return reinterpret_cast<float*>(
		reinterpret_cast<unsigned char*>(pBufferPointer) +
		nVertexCount * vertexSize);
}

inline const float* OffsetFloatPointer(const float* pBufferPointer, int nVertexCount, int vertexSize)
{
	return reinterpret_cast<const float*>(
		reinterpret_cast<unsigned char const*>(pBufferPointer) +
		nVertexCount * vertexSize);
}

inline void IncrementFloatPointer(float*& pBufferPointer, int vertexSize)
{
	pBufferPointer = reinterpret_cast<float*>(reinterpret_cast<unsigned char*>(pBufferPointer) + vertexSize);
}


class CPrimList
{
public:
	CPrimList();
	CPrimList(int nFirstIndex, int nIndexCount);

	int			m_FirstIndex;
	int			m_NumIndices;
};

inline CPrimList::CPrimList()
{
}

inline CPrimList::CPrimList(int nFirstIndex, int nIndexCount)
{
	m_FirstIndex = nFirstIndex;
	m_NumIndices = nIndexCount;
}

abstract_class IVertexBuffer
{
public:
	virtual int VertexCount() const = 0;
	virtual VertexFormat_t GetVertexFormat() const = 0;

	virtual bool IsDynamic() const = 0;

	virtual void BeginCastBuffer(VertexFormat_t format) = 0;
	virtual void EndCastBuffer() = 0;

	virtual int GetRoomRemaining() const = 0;

	virtual bool Lock(int nVertexCount, bool bAppend, VertexDesc_t& desc) = 0;
	virtual void Unlock(int nVertexCount, VertexDesc_t& desc) = 0;

	virtual void Spew(int nVertexCount, const VertexDesc_t& desc) = 0;

	virtual void ValidateData(int nVertexCount, const VertexDesc_t& desc) = 0;
};

abstract_class IIndexBuffer
{
public:
	virtual int IndexCount() const = 0;
	virtual MaterialIndexFormat_t IndexFormat() const = 0;

	virtual bool IsDynamic() const = 0;

	virtual void BeginCastBuffer(MaterialIndexFormat_t format) = 0;
	virtual void EndCastBuffer() = 0;

	virtual int GetRoomRemaining() const = 0;

	virtual bool Lock(int nMaxIndexCount, bool bAppend, IndexDesc_t& desc) = 0;
	virtual void Unlock(int nWrittenIndexCount, IndexDesc_t& desc) = 0;

	virtual void ModifyBegin(bool bReadOnly, int nFirstIndex, int nIndexCount, IndexDesc_t& desc) = 0;
	virtual void ModifyEnd(IndexDesc_t& desc) = 0;

	virtual void Spew(int nIndexCount, const IndexDesc_t& desc) = 0;

	virtual void ValidateData(int nIndexCount, const IndexDesc_t& desc) = 0;

	virtual IMesh* GetMesh() = 0;
};

abstract_class ICachedPerFrameMeshData
{
public:
	virtual void Free() = 0;
};

abstract_class IMesh : public IVertexBuffer, public IIndexBuffer
{
public:
	virtual void SetPrimitiveType(MaterialPrimitiveType_t type) = 0;

	virtual void Draw(int firstIndex = -1, int numIndices = 0) = 0;

	virtual void SetColorMesh(IMesh* pColorMesh, int nVertexOffset) = 0;

	virtual void Draw(CPrimList* pLists, int nLists) = 0;

	virtual void CopyToMeshBuilder(
		int iStartVert,		    
		int nVerts,
		int iStartIndex,	    
		int nIndices,
		int indexOffset,	      
		CMeshBuilder& builder) = 0;

	virtual void Spew(int numVerts, int numIndices, const MeshDesc_t& desc) = 0;

	virtual void ValidateData(int numVerts, int numIndices, const MeshDesc_t& desc) = 0;

	virtual void LockMesh(int numVerts, int numIndices, MeshDesc_t& desc, MeshBuffersAllocationSettings_t* pSettings) = 0;
	virtual void ModifyBegin(int firstVertex, int numVerts, int firstIndex, int numIndices, MeshDesc_t& desc) = 0;
	virtual void ModifyEnd(MeshDesc_t& desc) = 0;
	virtual void UnlockMesh(int numVerts, int numIndices, MeshDesc_t& desc) = 0;

	virtual void ModifyBeginEx(bool bReadOnly, int firstVertex, int numVerts, int firstIndex, int numIndices, MeshDesc_t& desc) = 0;

	virtual void SetFlexMesh(IMesh* pMesh, int nVertexOffset) = 0;

	virtual void DisableFlexMesh() = 0;

	virtual void MarkAsDrawn() = 0;

	virtual void DrawModulated(const Vector4D& vecDiffuseModulation, int firstIndex = -1, int numIndices = 0) = 0;

	virtual unsigned int ComputeMemoryUsed() = 0;

	virtual void* AccessRawHardwareDataStream(uint8 nRawStreamIndex, uint32 numBytes, uint32 uiFlags, void* pvContext) = 0;

	virtual ICachedPerFrameMeshData* GetCachedPerFrameMeshData() = 0;
	virtual void ReconstructFromCachedPerFrameMeshData(ICachedPerFrameMeshData* pData) = 0;
};


#include "meshreader.h"

#define INVALID_BUFFER_OFFSET 0xFFFFFFFFUL

#define VTX_HAVEPOS 1
#define VTX_HAVENORMAL 2
#define VTX_HAVECOLOR 4
#define VTX_HAVEALL ( VTX_HAVEPOS | VTX_HAVENORMAL | VTX_HAVECOLOR )


class CVertexBuilder : private VertexDesc_t
{
public:
	CVertexBuilder();
	CVertexBuilder(IVertexBuffer* pVertexBuffer, VertexFormat_t fmt = 0);
	~CVertexBuilder();

	bool Lock(int nMaxIndexCount, bool bAppend = false);
	void Unlock();

	void SpewData();

	int GetRoomRemaining() const;

	void Bind(IMatRenderContext* pContext, int nStreamID, VertexFormat_t usage = 0);

	int Offset() const;

	void SetCompressionType(VertexCompressionType_t compressionType);
	void ValidateCompressionType();

	void Begin(IVertexBuffer* pVertexBuffer, int nVertexCount, int* nFirstVertex);
	void Begin(IVertexBuffer* pVertexBuffer, int nVertexCount);

	void End(bool bSpewData = false);

	void BeginModify(IVertexBuffer* pVertexBuffer, int nFirstVertex = 0, int nVertexCount = -1);
	void EndModify(bool bSpewData = false);

	int VertexCount() const;

	int TotalVertexCount() const;

	void Reset();

	int VertexSize() { return m_ActualVertexSize; }

	int TextureCoordinateSize(int nTexCoordNumber) { return m_VertexSize_TexCoord[nTexCoordNumber]; }

	void* BaseVertexData();

	void SelectVertex(int idx);

	void AdvanceVertex(void);
	template<int nFlags, int nNumTexCoords> void AdvanceVertexF(void);
	void AdvanceVertices(int nVerts);

	int GetCurrentVertex() const;
	int GetFirstVertex() const;

	const float* Position() const;

	const float* Normal() const;

	unsigned int Color() const;

	unsigned char* Specular() const;

	const float* TexCoord(int stage) const;

	const float* TangentS() const;
	const float* TangentT() const;

	const float* BoneWeight() const;
	float Wrinkle() const;

	int NumBoneWeights() const;
#ifndef NEW_SKINNING
	unsigned char* BoneMatrix() const;
#else
	float* BoneMatrix() const;
#endif

	void Position3f(float x, float y, float z);
	void Position3fv(const float* v);

	void Normal3f(float nx, float ny, float nz);
	void Normal3fv(const float* n);
	void NormalDelta3fv(const float* n);
	void NormalDelta3f(float nx, float ny, float nz);
	template <VertexCompressionType_t T> void CompressedNormal3f(float nx, float ny, float nz);
	template <VertexCompressionType_t T> void CompressedNormal3fv(const float* n);

	void Color3f(float r, float g, float b);
	void Color3fv(const float* rgb);
	void Color4f(float r, float g, float b, float a);
	void Color4fv(const float* rgba);

	void Color3ub(unsigned char r, unsigned char g, unsigned char b);
	void Color3ubv(unsigned char const* rgb);
	void Color4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void Color4ubv(unsigned char const* rgba);
	void Color4Packed(int packedColor);
	int PackColor4(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	void Specular3f(float r, float g, float b);
	void Specular3fv(const float* rgb);
	void Specular4f(float r, float g, float b, float a);
	void Specular4fv(const float* rgba);

	void Specular3ub(unsigned char r, unsigned char g, unsigned char b);
	void Specular3ubv(unsigned char const* c);
	void Specular4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void Specular4ubv(unsigned char const* c);

	void TexCoord1f(int stage, float s);
	void TexCoord2f(int stage, float s, float t);
	void TexCoord2fv(int stage, const float* st);
	void TexCoord3f(int stage, float s, float t, float u);
	void TexCoord3fv(int stage, const float* stu);
	void TexCoord4f(int stage, float s, float t, float u, float w);
	void TexCoord4fv(int stage, const float* stuv);

	void TexCoordSubRect2f(int stage, float s, float t, float offsetS, float offsetT, float scaleS, float scaleT);
	void TexCoordSubRect2fv(int stage, const float* st, const float* offset, const float* scale);

	void TangentS3f(float sx, float sy, float sz);
	void TangentS3fv(const float* s);

	void TangentT3f(float tx, float ty, float tz);
	void TangentT3fv(const float* t);

	void Wrinkle1f(float flWrinkle);

	void BoneWeight(int idx, float weight);
	void BoneWeights2(float weight1, float weight2);

	template <VertexCompressionType_t T> void CompressedBoneWeight3fv(const float* pWeights);

	void BoneMatrix(int idx, int matrixIndex);
	void BoneMatrices4(int matrixIdx0, int matrixIdx1, int matrixIdx2, int matrixIdx3);

	void UserData(const float* pData);
	template <VertexCompressionType_t T> void CompressedUserData(const float* pData);

	void FastVertex(const ModelVertexDX8_t& vertex);
	void FastVertexSSE(const ModelVertexDX8_t& vertex);
	void FastQuadVertexSSE(const QuadTessVertex_t& vertex);

	void FastAdvanceNVertices(int n);

#if defined( _X360 )
	void VertexDX8ToX360(const ModelVertexDX8_t& vertex);
#endif

	void AttachBegin(IMesh* pMesh, int nMaxVertexCount, const MeshDesc_t& desc);
	void AttachEnd();
	void AttachBeginModify(IMesh* pMesh, int nFirstVertex, int nVertexCount, const MeshDesc_t& desc);
	void AttachEndModify();

private:
	IVertexBuffer* m_pVertexBuffer;

	bool m_bModify;

	int m_nMaxVertexCount;

	int m_nVertexCount;

	mutable int m_nCurrentVertex;

	mutable float* m_pCurrPosition;
	mutable float* m_pCurrNormal;
	mutable unsigned char* m_pCurrColor;
	mutable float* m_pCurrTexCoord[VERTEX_MAX_TEXTURE_COORDINATES];

	int m_nTotalVertexCount;

	unsigned int m_nBufferOffset;
	unsigned int m_nBufferFirstVertex;

#if ( COMPRESSED_NORMALS_TYPE == COMPRESSED_NORMALS_COMBINEDTANGENTS_UBYTE4 )
	bool m_bWrittenNormal : 1;
	bool m_bWrittenUserData : 1;
#endif

	friend class CMeshBuilder;
};


inline CVertexBuilder::CVertexBuilder()
{
	m_pVertexBuffer = NULL;
	m_nBufferOffset = INVALID_BUFFER_OFFSET;
	m_nBufferFirstVertex = 0;
	m_nVertexCount = 0;
	m_nCurrentVertex = 0;
	m_nMaxVertexCount = 0;
	m_nTotalVertexCount = 0;
	m_CompressionType = VERTEX_COMPRESSION_INVALID;

#ifdef _DEBUG
	m_pCurrPosition = NULL;
	m_pCurrNormal = NULL;
	m_pCurrColor = NULL;
	memset(m_pCurrTexCoord, 0, sizeof(m_pCurrTexCoord));
	m_bModify = false;
#endif
}

inline CVertexBuilder::CVertexBuilder(IVertexBuffer* pVertexBuffer, VertexFormat_t fmt)
{
	m_pVertexBuffer = pVertexBuffer;
	m_nBufferOffset = INVALID_BUFFER_OFFSET;
	m_nBufferFirstVertex = 0;
	m_nVertexCount = 0;
	m_nCurrentVertex = 0;
	m_nMaxVertexCount = 0;
	m_nTotalVertexCount = 0;
	m_CompressionType = VERTEX_COMPRESSION_INVALID;

	if (m_pVertexBuffer->IsDynamic())
	{
		m_pVertexBuffer->BeginCastBuffer(fmt);
	}
	else
	{
		Assert(m_pVertexBuffer->GetVertexFormat() == fmt);
	}

#ifdef _DEBUG
	m_pCurrPosition = NULL;
	m_pCurrNormal = NULL;
	m_pCurrColor = NULL;
	memset(m_pCurrTexCoord, 0, sizeof(m_pCurrTexCoord));
	m_bModify = false;
#endif
}

inline CVertexBuilder::~CVertexBuilder()
{
	if (m_pVertexBuffer && m_pVertexBuffer->IsDynamic())
	{
		m_pVertexBuffer->EndCastBuffer();
	}
}

inline bool CVertexBuilder::Lock(int nMaxVertexCount, bool bAppend)
{
	Assert(m_pVertexBuffer);
	m_bModify = false;
	m_nMaxVertexCount = nMaxVertexCount;
	bool bFirstLock = (m_nBufferOffset == INVALID_BUFFER_OFFSET);
	if (bFirstLock)
	{
		bAppend = false;
	}
	if (!bAppend)
	{
		m_nTotalVertexCount = 0;
	}

	if (!m_pVertexBuffer->Lock(m_nMaxVertexCount, bAppend, *this))
	{
		m_nMaxVertexCount = 0;
		return false;
	}

	Reset();

	if (bFirstLock)
	{
		m_nBufferOffset = m_nOffset;
		m_nBufferFirstVertex = m_nFirstVertex;
	}

	return true;
}

inline void CVertexBuilder::Unlock()
{
	Assert(!m_bModify && m_pVertexBuffer);

#ifdef _DEBUG
	m_pVertexBuffer->ValidateData(m_nVertexCount, *this);
#endif

	m_pVertexBuffer->Unlock(m_nVertexCount, *this);
	m_nTotalVertexCount += m_nVertexCount;

	m_nMaxVertexCount = 0;

#ifdef _DEBUG
	m_pCurrPosition = NULL;
	m_pCurrNormal = NULL;
	m_pCurrColor = NULL;
	memset(m_pCurrTexCoord, 0, sizeof(m_pCurrTexCoord));
	memset(static_cast<VertexDesc_t*>(this), 0, sizeof(VertexDesc_t));
#endif
}

inline void CVertexBuilder::SpewData()
{
	m_pVertexBuffer->Spew(m_nVertexCount, *this);
}


inline void CVertexBuilder::Bind(IMatRenderContext* pContext, int nStreamID, VertexFormat_t usage)
{
	if (m_pVertexBuffer && (m_nBufferOffset != INVALID_BUFFER_OFFSET))
	{
		pContext->BindVertexBuffer(nStreamID, m_pVertexBuffer, m_nBufferOffset,
			m_nFirstVertex, m_nTotalVertexCount, usage ? usage : m_pVertexBuffer->GetVertexFormat());
	}
	else
	{
		pContext->BindVertexBuffer(nStreamID, NULL, 0, 0, 0, 0);
	}
}


inline int CVertexBuilder::Offset() const
{
	return m_nBufferOffset;
}

inline int CVertexBuilder::GetFirstVertex() const
{
	return m_nBufferFirstVertex;
}

inline void CVertexBuilder::SetCompressionType(VertexCompressionType_t compressionType)
{
	m_CompressionType = compressionType;
}

inline void CVertexBuilder::ValidateCompressionType()
{
#ifdef _DEBUG
	VertexCompressionType_t vbCompressionType = CompressionType(m_pVertexBuffer->GetVertexFormat());
	if (vbCompressionType != VERTEX_COMPRESSION_NONE)
	{
		Assert(m_CompressionType == vbCompressionType);
		if (m_CompressionType != vbCompressionType)
		{
			Warning("ERROR: CVertexBuilder::SetCompressionType() must be called to specify the same vertex compression type (%s) as the vertex buffer being modified."
				"Junk vertices will be rendered, or there will be a crash in CVertexBuilder!\n",
				vbCompressionType == VERTEX_COMPRESSION_ON ? "VERTEX_COMPRESSION_ON" : "VERTEX_COMPRESSION_NONE");
		}
		Assert(!m_pVertexBuffer->IsDynamic());
	}
#endif
}

inline void CVertexBuilder::Begin(IVertexBuffer* pVertexBuffer, int nVertexCount)
{
	Assert(pVertexBuffer && (!m_pVertexBuffer));

	m_pVertexBuffer = pVertexBuffer;
	m_bModify = false;

	m_nMaxVertexCount = nVertexCount;
	m_nVertexCount = 0;

	ValidateCompressionType();

	m_pVertexBuffer->Lock(m_nMaxVertexCount, false, *this);

	Reset();
}


inline void CVertexBuilder::End(bool bSpewData)
{
	Assert(!m_bModify);

	if (bSpewData)
	{
		m_pVertexBuffer->Spew(m_nVertexCount, *this);
	}

#ifdef _DEBUG
	m_pVertexBuffer->ValidateData(m_nVertexCount, *this);
#endif

	m_pVertexBuffer->Unlock(m_nVertexCount, *this);

	m_pVertexBuffer = 0;
	m_nMaxVertexCount = 0;

	m_CompressionType = VERTEX_COMPRESSION_INVALID;

#ifdef _DEBUG
	m_pCurrPosition = NULL;
	m_pCurrNormal = NULL;
	m_pCurrColor = NULL;
	memset(m_pCurrTexCoord, 0, sizeof(m_pCurrTexCoord));
	memset(static_cast<VertexDesc_t*>(this), 0, sizeof(VertexDesc_t));
#endif
}


inline void CVertexBuilder::AttachBegin(IMesh* pMesh, int nMaxVertexCount, const MeshDesc_t& desc)
{
	m_pVertexBuffer = pMesh;
	memcpy(static_cast<VertexDesc_t*>(this), static_cast<const VertexDesc_t*>(&desc), sizeof(VertexDesc_t));
	m_nMaxVertexCount = nMaxVertexCount;
	m_NumBoneWeights = m_NumBoneWeights == 0 ? 0 : 2;	    
	m_nVertexCount = 0;
	m_bModify = false;

	ValidateCompressionType();

	if (m_nBufferOffset == INVALID_BUFFER_OFFSET)
	{
		m_nTotalVertexCount = 0;
		m_nBufferOffset = static_cast<const VertexDesc_t*>(&desc)->m_nOffset;
		m_nBufferFirstVertex = desc.m_nFirstVertex;
	}
}

inline void CVertexBuilder::AttachEnd()
{
	Assert(!m_bModify);

	m_nMaxVertexCount = 0;
	m_pVertexBuffer = NULL;

	m_CompressionType = VERTEX_COMPRESSION_INVALID;

#ifdef _DEBUG
	m_pCurrPosition = NULL;
	m_pCurrNormal = NULL;
	m_pCurrColor = NULL;
	memset(m_pCurrTexCoord, 0, sizeof(m_pCurrTexCoord));
	memset(static_cast<VertexDesc_t*>(this), 0, sizeof(VertexDesc_t));
#endif
}

inline void CVertexBuilder::AttachBeginModify(IMesh* pMesh, int nFirstVertex, int nVertexCount, const MeshDesc_t& desc)
{
	Assert(pMesh && (!m_pVertexBuffer));

	m_pVertexBuffer = pMesh;
	memcpy(static_cast<VertexDesc_t*>(this), static_cast<const VertexDesc_t*>(&desc), sizeof(VertexDesc_t));
	m_nMaxVertexCount = m_nVertexCount = nVertexCount;
	m_NumBoneWeights = m_NumBoneWeights == 0 ? 0 : 2;	    
	m_bModify = true;

	ValidateCompressionType();
}

inline void CVertexBuilder::AttachEndModify()
{
	Assert(m_pVertexBuffer);
	Assert(m_bModify);	     

	m_pVertexBuffer = 0;
	m_nMaxVertexCount = 0;

	m_CompressionType = VERTEX_COMPRESSION_INVALID;

#ifdef _DEBUG
	m_pCurrPosition = NULL;
	m_pCurrNormal = NULL;
	m_pCurrColor = NULL;
	memset(m_pCurrTexCoord, 0, sizeof(m_pCurrTexCoord));
	memset(static_cast<VertexDesc_t*>(this), 0, sizeof(VertexDesc_t));
#endif
}


inline unsigned char* FindMinAddress(void* pAddress1, void* pAddress2, int nAddress2Size)
{
	if (nAddress2Size == 0)
		return (unsigned char*)pAddress1;
	if (!pAddress1)
		return (unsigned char*)pAddress2;
	return (pAddress1 < pAddress2) ? (unsigned char*)pAddress1 : (unsigned char*)pAddress2;
}

inline void CVertexBuilder::Reset()
{
	m_nCurrentVertex = 0;

	m_pCurrPosition = m_pPosition;
	m_pCurrNormal = m_pNormal;
	for (size_t i = 0; i < NELEMS(m_pCurrTexCoord); i++)
	{
		m_pCurrTexCoord[i] = m_pTexCoord[i];
	}
	m_pCurrColor = m_pColor;

#if ( defined( _DEBUG ) && ( COMPRESSED_NORMALS_TYPE == COMPRESSED_NORMALS_COMBINEDTANGENTS_UBYTE4 ) )
	m_bWrittenNormal = false;
	m_bWrittenUserData = false;
#endif

#ifdef DEBUG_WRITE_COMBINE
	m_nLastWrittenField = MB_FIELD_NONE;
	m_pLastWrittenAddress = NULL;
	m_pLastWrittenAddress = FindMinAddress(m_pLastWrittenAddress, m_pPosition, m_VertexSize_Position);
	m_pLastWrittenAddress = FindMinAddress(m_pLastWrittenAddress, m_pBoneWeight, m_VertexSize_BoneWeight);
	m_pLastWrittenAddress = FindMinAddress(m_pLastWrittenAddress, m_pBoneMatrixIndex, m_VertexSize_BoneMatrixIndex);
	m_pLastWrittenAddress = FindMinAddress(m_pLastWrittenAddress, m_pNormal, m_VertexSize_Normal);
	m_pLastWrittenAddress = FindMinAddress(m_pLastWrittenAddress, m_pColor, m_VertexSize_Color);
	m_pLastWrittenAddress = FindMinAddress(m_pLastWrittenAddress, m_pSpecular, m_VertexSize_Specular);
	for (int i = 0; i < VERTEX_MAX_TEXTURE_COORDINATES; ++i)
	{
		m_pLastWrittenAddress = FindMinAddress(m_pLastWrittenAddress, m_pTexCoord[i], m_VertexSize_TexCoord[i]);
	}
	m_pLastWrittenAddress = FindMinAddress(m_pLastWrittenAddress, m_pTangentS, m_VertexSize_TangentS);
	m_pLastWrittenAddress = FindMinAddress(m_pLastWrittenAddress, m_pTangentT, m_VertexSize_TangentT);
	m_pLastWrittenAddress = FindMinAddress(m_pLastWrittenAddress, m_pUserData, m_VertexSize_UserData);
#endif
}


inline int CVertexBuilder::VertexCount() const
{
	return m_nVertexCount;
}


inline int CVertexBuilder::TotalVertexCount() const
{
	return m_nTotalVertexCount;
}


inline void* CVertexBuilder::BaseVertexData()
{
	Assert(m_pPosition);
	return m_pPosition;
}


inline void CVertexBuilder::SelectVertex(int nIndex)
{
	Assert((nIndex >= 0) && (nIndex < m_nMaxVertexCount));
	m_nCurrentVertex = nIndex;

	m_pCurrPosition = OffsetFloatPointer(m_pPosition, m_nCurrentVertex, m_VertexSize_Position);
	m_pCurrNormal = OffsetFloatPointer(m_pNormal, m_nCurrentVertex, m_VertexSize_Normal);

	COMPILE_TIME_ASSERT(VERTEX_MAX_TEXTURE_COORDINATES == 8);
	m_pCurrTexCoord[0] = OffsetFloatPointer(m_pTexCoord[0], m_nCurrentVertex, m_VertexSize_TexCoord[0]);
	m_pCurrTexCoord[1] = OffsetFloatPointer(m_pTexCoord[1], m_nCurrentVertex, m_VertexSize_TexCoord[1]);
	m_pCurrTexCoord[2] = OffsetFloatPointer(m_pTexCoord[2], m_nCurrentVertex, m_VertexSize_TexCoord[2]);
	m_pCurrTexCoord[3] = OffsetFloatPointer(m_pTexCoord[3], m_nCurrentVertex, m_VertexSize_TexCoord[3]);
	m_pCurrTexCoord[4] = OffsetFloatPointer(m_pTexCoord[4], m_nCurrentVertex, m_VertexSize_TexCoord[4]);
	m_pCurrTexCoord[5] = OffsetFloatPointer(m_pTexCoord[5], m_nCurrentVertex, m_VertexSize_TexCoord[5]);
	m_pCurrTexCoord[6] = OffsetFloatPointer(m_pTexCoord[6], m_nCurrentVertex, m_VertexSize_TexCoord[6]);
	m_pCurrTexCoord[7] = OffsetFloatPointer(m_pTexCoord[7], m_nCurrentVertex, m_VertexSize_TexCoord[7]);
	m_pCurrColor = m_pColor + m_nCurrentVertex * m_VertexSize_Color;

#if ( defined( _DEBUG ) && ( COMPRESSED_NORMALS_TYPE == COMPRESSED_NORMALS_COMBINEDTANGENTS_UBYTE4 ) )
	m_bWrittenNormal = false;
	m_bWrittenUserData = false;
#endif
}


template<int nFlags, int nNumTexCoords> FORCEINLINE void CVertexBuilder::AdvanceVertexF()
{
	if (++m_nCurrentVertex > m_nVertexCount)
	{
		m_nVertexCount = m_nCurrentVertex;
	}

	if (nFlags & VTX_HAVEPOS)
		IncrementFloatPointer(m_pCurrPosition, m_VertexSize_Position);
	if (nFlags & VTX_HAVENORMAL)
		IncrementFloatPointer(m_pCurrNormal, m_VertexSize_Normal);
	if (nFlags & VTX_HAVECOLOR)
		m_pCurrColor += m_VertexSize_Color;

	COMPILE_TIME_ASSERT(VERTEX_MAX_TEXTURE_COORDINATES == 8);
	if (nNumTexCoords > 0)
		IncrementFloatPointer(m_pCurrTexCoord[0], m_VertexSize_TexCoord[0]);
	if (nNumTexCoords > 1)
		IncrementFloatPointer(m_pCurrTexCoord[1], m_VertexSize_TexCoord[1]);
	if (nNumTexCoords > 2)
		IncrementFloatPointer(m_pCurrTexCoord[2], m_VertexSize_TexCoord[2]);
	if (nNumTexCoords > 3)
		IncrementFloatPointer(m_pCurrTexCoord[3], m_VertexSize_TexCoord[3]);
	if (nNumTexCoords > 4)
		IncrementFloatPointer(m_pCurrTexCoord[4], m_VertexSize_TexCoord[4]);
	if (nNumTexCoords > 5)
		IncrementFloatPointer(m_pCurrTexCoord[5], m_VertexSize_TexCoord[5]);
	if (nNumTexCoords > 6)
		IncrementFloatPointer(m_pCurrTexCoord[6], m_VertexSize_TexCoord[6]);
	if (nNumTexCoords > 7)
		IncrementFloatPointer(m_pCurrTexCoord[7], m_VertexSize_TexCoord[7]);

#if ( defined( _DEBUG ) && ( COMPRESSED_NORMALS_TYPE == COMPRESSED_NORMALS_COMBINEDTANGENTS_UBYTE4 ) )
	m_bWrittenNormal = false;
	m_bWrittenUserData = false;
#endif
}

inline void CVertexBuilder::AdvanceVertex()
{
	AdvanceVertexF<VTX_HAVEALL, 8>();
}


inline void CVertexBuilder::AdvanceVertices(int nVerts)
{
	m_nCurrentVertex += nVerts;
	if (m_nCurrentVertex > m_nVertexCount)
	{
		m_nVertexCount = m_nCurrentVertex;
	}

	IncrementFloatPointer(m_pCurrPosition, m_VertexSize_Position * nVerts);
	IncrementFloatPointer(m_pCurrNormal, m_VertexSize_Normal * nVerts);

	COMPILE_TIME_ASSERT(VERTEX_MAX_TEXTURE_COORDINATES == 8);
	IncrementFloatPointer(m_pCurrTexCoord[0], m_VertexSize_TexCoord[0] * nVerts);
	IncrementFloatPointer(m_pCurrTexCoord[1], m_VertexSize_TexCoord[1] * nVerts);
	IncrementFloatPointer(m_pCurrTexCoord[2], m_VertexSize_TexCoord[2] * nVerts);
	IncrementFloatPointer(m_pCurrTexCoord[3], m_VertexSize_TexCoord[3] * nVerts);
	IncrementFloatPointer(m_pCurrTexCoord[4], m_VertexSize_TexCoord[4] * nVerts);
	IncrementFloatPointer(m_pCurrTexCoord[5], m_VertexSize_TexCoord[5] * nVerts);
	IncrementFloatPointer(m_pCurrTexCoord[6], m_VertexSize_TexCoord[6] * nVerts);
	IncrementFloatPointer(m_pCurrTexCoord[7], m_VertexSize_TexCoord[7] * nVerts);
	m_pCurrColor += m_VertexSize_Color * nVerts;

#if ( defined( _DEBUG ) && ( COMPRESSED_NORMALS_TYPE == COMPRESSED_NORMALS_COMBINEDTANGENTS_UBYTE4 ) )
	m_bWrittenNormal = false;
	m_bWrittenUserData = false;
#endif
}


inline void CVertexBuilder::FastAdvanceNVertices(int n)
{
	m_nCurrentVertex += n;
	m_nVertexCount = m_nCurrentVertex;
}

inline void CVertexBuilder::FastVertex(const ModelVertexDX8_t& vertex)
{
	Assert(m_CompressionType == VERTEX_COMPRESSION_NONE);       
	Assert(m_nCurrentVertex < m_nMaxVertexCount);

#if defined( _WIN32 ) && !defined( _X360 )
	const void* pRead = &vertex;
	void* pCurrPos = m_pCurrPosition;
	__asm
	{
		mov esi, pRead
		mov edi, pCurrPos

		movq mm0, [esi + 0]
		movq mm1, [esi + 8]
		movq mm2, [esi + 16]
		movq mm3, [esi + 24]
		movq mm4, [esi + 32]
		movq mm5, [esi + 40]

		movntq[edi + 0], mm0
		movntq[edi + 8], mm1
		movntq[edi + 16], mm2
		movntq[edi + 24], mm3
		movntq[edi + 32], mm4
		movntq[edi + 40], mm5

		emms
	}
#elif defined(GNUC)
	const void* pRead = &vertex;
	void* pCurrPos = m_pCurrPosition;
	__asm__ __volatile__(
		"movq (%0), %%mm0\n"
		"movq 8(%0), %%mm1\n"
		"movq 16(%0), %%mm2\n"
		"movq 24(%0), %%mm3\n"
		"movq 32(%0), %%mm4\n"
		"movq 40(%0), %%mm5\n"
		"movq 48(%0), %%mm6\n"
		"movq 56(%0), %%mm7\n"
		"movntq %%mm0, (%1)\n"
		"movntq %%mm1, 8(%1)\n"
		"movntq %%mm2, 16(%1)\n"
		"movntq %%mm3, 24(%1)\n"
		"movntq %%mm4, 32(%1)\n"
		"movntq %%mm5, 40(%1)\n"
		"movntq %%mm6, 48(%1)\n"
		"movntq %%mm7, 56(%1)\n"
		"emms\n"
		:: "r" (pRead), "r" (pCurrPos) : "memory");
#else
	Error("Implement CMeshBuilder::FastVertex(dx8)");
#endif

	IncrementFloatPointer(m_pCurrPosition, m_VertexSize_Position);
#if ( defined( _DEBUG ) && ( COMPRESSED_NORMALS_TYPE == COMPRESSED_NORMALS_COMBINEDTANGENTS_UBYTE4 ) )
	m_bWrittenNormal = false;
	m_bWrittenUserData = false;
#endif
}

inline void CVertexBuilder::FastVertexSSE(const ModelVertexDX8_t& vertex)
{
	Assert(m_CompressionType == VERTEX_COMPRESSION_NONE);       
	Assert(m_nCurrentVertex < m_nMaxVertexCount);

#if defined( _WIN32 ) && !defined( _X360 )
	const void* pRead = &vertex;
	void* pCurrPos = m_pCurrPosition;
	__asm
	{
		mov esi, pRead
		mov edi, pCurrPos

		movaps xmm0, [esi + 0]
		movaps xmm1, [esi + 16]
		movaps xmm2, [esi + 32]

		movntps[edi + 0], xmm0
		movntps[edi + 16], xmm1
		movntps[edi + 32], xmm2
	}
#elif defined(GNUC)
	const void* pRead = &vertex;
	void* pCurrPos = m_pCurrPosition;
	__asm__ __volatile__(
		"movaps (%0), %%xmm0\n"
		"movaps 16(%0), %%xmm1\n"
		"movaps 32(%0), %%xmm2\n"
		"movaps 48(%0), %%xmm3\n"
		"movntps %%xmm0, (%1)\n"
		"movntps %%xmm1, 16(%1)\n"
		"movntps %%xmm2, 32(%1)\n"
		"movntps %%xmm3, 48(%1)\n"
		:: "r" (pRead), "r" (pCurrPos) : "memory");
#else
	Error("Implement CMeshBuilder::FastVertexSSE((dx8)");
#endif

	IncrementFloatPointer(m_pCurrPosition, m_VertexSize_Position);
#if ( defined( _DEBUG ) && ( COMPRESSED_NORMALS_TYPE == COMPRESSED_NORMALS_COMBINEDTANGENTS_UBYTE4 ) )
	m_bWrittenNormal = false;
	m_bWrittenUserData = false;
#endif
}


inline void CVertexBuilder::FastQuadVertexSSE(const QuadTessVertex_t& vertex)
{
	Assert(m_CompressionType == VERTEX_COMPRESSION_NONE);       
	Assert(m_nCurrentVertex < m_nMaxVertexCount);

#if defined( _WIN32 ) && !defined( _X360 )
	const void* pRead = &vertex;
	void* pCurrPos = m_pCurrPosition;
	__asm
	{
		mov esi, pRead
		mov edi, pCurrPos

		movaps xmm0, [esi + 0]
		movaps xmm1, [esi + 16]
		movaps xmm2, [esi + 32]

		movntps[edi + 0], xmm0
		movntps[edi + 16], xmm1
		movntps[edi + 32], xmm2
	}
#endif

	IncrementFloatPointer(m_pCurrPosition, m_VertexSize_Position);
#if ( defined( _DEBUG ) && ( COMPRESSED_NORMALS_TYPE == COMPRESSED_NORMALS_COMBINEDTANGENTS_UBYTE4 ) )
	m_bWrittenNormal = false;
	m_bWrittenUserData = false;
#endif
}



inline int CVertexBuilder::GetCurrentVertex() const
{
	return m_nCurrentVertex;
}


#if defined( _X360 )
inline void CVertexBuilder::VertexDX8ToX360(const ModelVertexDX8_t& vertex)
{
	Assert(m_CompressionType == VERTEX_COMPRESSION_NONE);       
	Assert(m_nCurrentVertex < m_nMaxVertexCount);

	unsigned char* pDst = (unsigned char*)m_pCurrPosition;

	Assert(m_VertexSize_Position > 0);      
	Assert(GetVertexElementSize(VERTEX_ELEMENT_POSITION, VERTEX_COMPRESSION_NONE) == sizeof(vertex.m_vecPosition));
	memcpy(pDst, vertex.m_vecPosition.Base(), sizeof(vertex.m_vecPosition));
	pDst += sizeof(vertex.m_vecPosition);

	if (m_VertexSize_Normal)
	{
		Assert(GetVertexElementSize(VERTEX_ELEMENT_NORMAL, VERTEX_COMPRESSION_NONE) == sizeof(vertex.m_vecNormal));
		memcpy(pDst, vertex.m_vecNormal.Base(), sizeof(vertex.m_vecNormal));
		pDst += sizeof(vertex.m_vecNormal);
	}

	if (m_VertexSize_TexCoord[0])
	{
		Assert(GetVertexElementSize(VERTEX_ELEMENT_TEXCOORD2D_0, VERTEX_COMPRESSION_NONE) == sizeof(vertex.m_vecTexCoord));
		memcpy(pDst, vertex.m_vecTexCoord.Base(), sizeof(vertex.m_vecTexCoord));
		pDst += sizeof(vertex.m_vecTexCoord);
	}

	if (m_VertexSize_UserData)
	{
		Assert(GetVertexElementSize(VERTEX_ELEMENT_USERDATA4, VERTEX_COMPRESSION_NONE) == sizeof(vertex.m_vecUserData));
		memcpy(pDst, vertex.m_vecUserData.Base(), sizeof(vertex.m_vecUserData));
		pDst += sizeof(vertex.m_vecUserData);
	}

	Assert(pDst - (unsigned char*)m_pCurrPosition == m_VertexSize_Position);

	IncrementFloatPointer(m_pCurrPosition, m_VertexSize_Position);

#if ( defined( _DEBUG ) && ( COMPRESSED_NORMALS_TYPE == COMPRESSED_NORMALS_COMBINEDTANGENTS_UBYTE4 ) )
	m_bWrittenNormal = false;
	m_bWrittenUserData = false;
#endif
}
#endif


inline const float* CVertexBuilder::Position() const
{
	Assert(m_CompressionType == VERTEX_COMPRESSION_NONE);
	Assert(m_nCurrentVertex < m_nMaxVertexCount);
	return m_pCurrPosition;
}

inline const float* CVertexBuilder::Normal() const
{
	Assert(m_CompressionType == VERTEX_COMPRESSION_NONE);
	Assert(m_nCurrentVertex < m_nMaxVertexCount);
	return m_pCurrNormal;
}

inline unsigned int CVertexBuilder::Color() const
{
	Assert(m_CompressionType == VERTEX_COMPRESSION_NONE);
	Assert(m_nCurrentVertex < m_nMaxVertexCount);
	unsigned int color;
	if (IsPC() || !IsX360())
	{
		color = (m_pCurrColor[3] << 24) | (m_pCurrColor[0] << 16) | (m_pCurrColor[1] << 8) | (m_pCurrColor[2]);
	}
	else
	{
		color = (m_pCurrColor[1] << 24) | (m_pCurrColor[2] << 16) | (m_pCurrColor[3] << 8) | (m_pCurrColor[0]);
	}
	return color;
}

inline unsigned char* CVertexBuilder::Specular() const
{
	Assert(m_CompressionType == VERTEX_COMPRESSION_NONE);
	Assert(m_nCurrentVertex < m_nMaxVertexCount);
	return m_pSpecular + m_nCurrentVertex * m_VertexSize_Specular;
}

inline const float* CVertexBuilder::TexCoord(int stage) const
{
	Assert(m_CompressionType == VERTEX_COMPRESSION_NONE);
	Assert(m_nCurrentVertex < m_nMaxVertexCount);
	return m_pCurrTexCoord[stage];
}

inline const float* CVertexBuilder::TangentS() const
{
	Assert(m_CompressionType == VERTEX_COMPRESSION_NONE);
	Assert(m_nCurrentVertex < m_nMaxVertexCount);
	return OffsetFloatPointer(m_pTangentS, m_nCurrentVertex, m_VertexSize_TangentS);
}

inline const float* CVertexBuilder::TangentT() const
{
	Assert(m_CompressionType == VERTEX_COMPRESSION_NONE);
	Assert(m_nCurrentVertex < m_nMaxVertexCount);
	return OffsetFloatPointer(m_pTangentT, m_nCurrentVertex, m_VertexSize_TangentT);
}

inline float CVertexBuilder::Wrinkle() const
{
	Assert(m_CompressionType == VERTEX_COMPRESSION_NONE);
	Assert(m_nCurrentVertex < m_nMaxVertexCount);
	return *OffsetFloatPointer(m_pWrinkle, m_nCurrentVertex, m_VertexSize_Wrinkle);
}

inline const float* CVertexBuilder::BoneWeight() const
{
	Assert(m_CompressionType == VERTEX_COMPRESSION_NONE);
	Assert(m_nCurrentVertex < m_nMaxVertexCount);
	return OffsetFloatPointer(m_pBoneWeight, m_nCurrentVertex, m_VertexSize_BoneWeight);
}

inline int CVertexBuilder::NumBoneWeights() const
{
	return m_NumBoneWeights;
}

#ifndef NEW_SKINNING
inline unsigned char* CVertexBuilder::BoneMatrix() const
{
	Assert(m_CompressionType == VERTEX_COMPRESSION_NONE);
	Assert(m_nCurrentVertex < m_nMaxVertexCount);
	return m_pBoneMatrixIndex + m_nCurrentVertex * m_VertexSize_BoneMatrixIndex;
}
#else
inline float* CVertexBuilder::BoneMatrix() const
{
	Assert(m_CompressionType == VERTEX_COMPRESSION_NONE);
	Assert(m_nCurrentVertex < m_nMaxVertexCount);
	return m_pBoneMatrixIndex + m_nCurrentVertex * m_VertexSize_BoneMatrixIndex;
}
#endif


inline void	CVertexBuilder::Position3f(float x, float y, float z)
{
	Assert(m_pPosition && m_pCurrPosition);
	Assert(IsFinite(x) && IsFinite(y) && IsFinite(z));
	float* pDst = m_pCurrPosition;
	*pDst++ = x;
	*pDst++ = y;
	*pDst = z;
}

inline void	CVertexBuilder::Position3fv(const float* v)
{
	Assert(v);
	Assert(m_pPosition && m_pCurrPosition);

	float* pDst = m_pCurrPosition;
	*pDst++ = *v++;
	*pDst++ = *v++;
	*pDst = *v;
}


inline void	CVertexBuilder::Normal3f(float nx, float ny, float nz)
{
	Assert(m_CompressionType == VERTEX_COMPRESSION_NONE);           
	Assert(m_pNormal);
	Assert(IsFinite(nx) && IsFinite(ny) && IsFinite(nz));
	Assert(nx >= -1.05f && nx <= 1.05f);
	Assert(ny >= -1.05f && ny <= 1.05f);
	Assert(nz >= -1.05f && nz <= 1.05f);

	float* pDst = m_pCurrNormal;
	*pDst++ = nx;
	*pDst++ = ny;
	*pDst = nz;
}

inline void	CVertexBuilder::Normal3fv(const float* n)
{
	Assert(m_CompressionType == VERTEX_COMPRESSION_NONE);           
	Assert(n);
	Assert(m_pNormal && m_pCurrNormal);
	Assert(IsFinite(n[0]) && IsFinite(n[1]) && IsFinite(n[2]));
	Assert(n[0] >= -1.05f && n[0] <= 1.05f);
	Assert(n[1] >= -1.05f && n[1] <= 1.05f);
	Assert(n[2] >= -1.05f && n[2] <= 1.05f);

	float* pDst = m_pCurrNormal;
	*pDst++ = *n++;
	*pDst++ = *n++;
	*pDst = *n;
}

inline void	CVertexBuilder::NormalDelta3f(float nx, float ny, float nz)
{
	Assert(m_CompressionType == VERTEX_COMPRESSION_NONE);           
	Assert(m_pNormal);
	Assert(IsFinite(nx) && IsFinite(ny) && IsFinite(nz));

	float* pDst = m_pCurrNormal;
	*pDst++ = nx;
	*pDst++ = ny;
	*pDst = nz;
}

inline void	CVertexBuilder::NormalDelta3fv(const float* n)
{
	Assert(m_CompressionType == VERTEX_COMPRESSION_NONE);           
	Assert(n);
	Assert(m_pNormal && m_pCurrNormal);
	Assert(IsFinite(n[0]) && IsFinite(n[1]) && IsFinite(n[2]));

	float* pDst = m_pCurrNormal;
	*pDst++ = *n++;
	*pDst++ = *n++;
	*pDst = *n;
}

template <VertexCompressionType_t T> inline void CVertexBuilder::CompressedNormal3f(float nx, float ny, float nz)
{
	Assert(T == m_CompressionType);
	Assert(m_pNormal && m_pCurrNormal);
	Assert(IsFinite(nx) && IsFinite(ny) && IsFinite(nz));
	Assert(nx >= -1.05f && nx <= 1.05f);
	Assert(ny >= -1.05f && ny <= 1.05f);
	Assert(nz >= -1.05f && nz <= 1.05f);
	if (T == VERTEX_COMPRESSION_ON)
	{
#if		( COMPRESSED_NORMALS_TYPE == COMPRESSED_NORMALS_SEPARATETANGENTS_SHORT2 )
		PackNormal_SHORT2(nx, ny, nz, (unsigned int*)m_pCurrNormal);

#else     
#ifdef _DEBUG
		Assert(m_bWrittenUserData == false);
		m_bWrittenNormal = true;
#endif
		PackNormal_UBYTE4(nx, ny, nz, (unsigned int*)m_pCurrNormal);
#endif
	}
	else
	{
		float* pDst = m_pCurrNormal;
		*pDst++ = nx;
		*pDst++ = ny;
		*pDst = nz;
	}
}

template <VertexCompressionType_t T> inline void CVertexBuilder::CompressedNormal3fv(const float* n)
{
	Assert(n);
	CompressedNormal3f<T>(n[0], n[1], n[2]);
}


inline void	CVertexBuilder::Color3f(float r, float g, float b)
{
	Assert(m_pColor && m_pCurrColor);
	Assert(IsFinite(r) && IsFinite(g) && IsFinite(b));
	Assert((r >= 0.0) && (g >= 0.0) && (b >= 0.0));
	Assert((r <= 1.0) && (g <= 1.0) && (b <= 1.0));

	int col = (FastFToC(b)) | (FastFToC(g) << 8) | (FastFToC(r) << 16) | 0xFF000000;
	*(int*)m_pCurrColor = col;
}

inline void	CVertexBuilder::Color3fv(const float* rgb)
{
	Assert(rgb);
	Assert(m_pColor && m_pCurrColor);
	Assert(IsFinite(rgb[0]) && IsFinite(rgb[1]) && IsFinite(rgb[2]));
	Assert((rgb[0] >= 0.0) && (rgb[1] >= 0.0) && (rgb[2] >= 0.0));
	Assert((rgb[0] <= 1.0) && (rgb[1] <= 1.0) && (rgb[2] <= 1.0));

	int col = (FastFToC(rgb[2])) | (FastFToC(rgb[1]) << 8) | (FastFToC(rgb[0]) << 16) | 0xFF000000;
	*(int*)m_pCurrColor = col;
}

inline void	CVertexBuilder::Color4f(float r, float g, float b, float a)
{
	Assert(m_pColor && m_pCurrColor);
	Assert(IsFinite(r) && IsFinite(g) && IsFinite(b) && IsFinite(a));
	Assert((r >= 0.0) && (g >= 0.0) && (b >= 0.0) && (a >= 0.0));
	Assert((r <= 1.0) && (g <= 1.0) && (b <= 1.0) && (a <= 1.0));

	int col = (FastFToC(b)) | (FastFToC(g) << 8) | (FastFToC(r) << 16) | (FastFToC(a) << 24);
	*(int*)m_pCurrColor = col;
}

inline void	CVertexBuilder::Color4fv(const float* rgba)
{
	Assert(rgba);
	Assert(m_pColor && m_pCurrColor);
	Assert(IsFinite(rgba[0]) && IsFinite(rgba[1]) && IsFinite(rgba[2]) && IsFinite(rgba[3]));
	Assert((rgba[0] >= 0.0) && (rgba[1] >= 0.0) && (rgba[2] >= 0.0) && (rgba[3] >= 0.0));
	Assert((rgba[0] <= 1.0) && (rgba[1] <= 1.0) && (rgba[2] <= 1.0) && (rgba[3] <= 1.0));

	int col = (FastFToC(rgba[2])) | (FastFToC(rgba[1]) << 8) | (FastFToC(rgba[0]) << 16) | (FastFToC(rgba[3]) << 24);
	*(int*)m_pCurrColor = col;
}


inline void CVertexBuilder::Color3ub(unsigned char r, unsigned char g, unsigned char b)
{
	Assert(m_pColor && m_pCurrColor);
#ifdef OPENGL_SWAP_COLORS
	int col = r | (g << 8) | (b << 16) | 0xFF000000;	      
#else
	int col = b | (g << 8) | (r << 16) | 0xFF000000;
#endif

	* (int*)m_pCurrColor = col;
}

inline void CVertexBuilder::Color3ubv(unsigned char const* rgb)
{
	Assert(rgb);
	Assert(m_pColor && m_pCurrColor);
#ifdef OPENGL_SWAP_COLORS
	int col = rgb[0] | (rgb[1] << 8) | (rgb[2] << 16) | 0xFF000000;	      
#else
	int col = rgb[2] | (rgb[1] << 8) | (rgb[0] << 16) | 0xFF000000;
#endif

	* (int*)m_pCurrColor = col;
}

inline void CVertexBuilder::Color4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	Assert(m_pColor && m_pCurrColor);
#ifdef OPENGL_SWAP_COLORS
	int col = r | (g << 8) | (b << 16) | (a << 24);	      
#else
	int col = b | (g << 8) | (r << 16) | (a << 24);
#endif

	* (int*)m_pCurrColor = col;
}

inline void CVertexBuilder::Color4ubv(unsigned char const* rgba)
{
	Assert(rgba);
	Assert(m_pColor && m_pCurrColor);
#ifdef OPENGL_SWAP_COLORS
	int col = rgba[0] | (rgba[1] << 8) | (rgba[2] << 16) | (rgba[3] << 24);	      
#else
	int col = rgba[2] | (rgba[1] << 8) | (rgba[0] << 16) | (rgba[3] << 24);
#endif
	* (int*)m_pCurrColor = col;
}

FORCEINLINE void CVertexBuilder::Color4Packed(int packedColor)
{
	*(int*)m_pCurrColor = packedColor;
}

FORCEINLINE int CVertexBuilder::PackColor4(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return b | (g << 8) | (r << 16) | (a << 24);
}


inline void	CVertexBuilder::Specular3f(float r, float g, float b)
{
	Assert(m_pSpecular);
	Assert(IsFinite(r) && IsFinite(g) && IsFinite(b));
	Assert((r >= 0.0) && (g >= 0.0) && (b >= 0.0));
	Assert((r <= 1.0) && (g <= 1.0) && (b <= 1.0));

	unsigned char* pSpecular = &m_pSpecular[m_nCurrentVertex * m_VertexSize_Specular];
	int col = (FastFToC(b)) | (FastFToC(g) << 8) | (FastFToC(r) << 16) | 0xFF000000;
	*(int*)pSpecular = col;
}

inline void	CVertexBuilder::Specular3fv(const float* rgb)
{
	Assert(rgb);
	Assert(m_pSpecular);
	Assert(IsFinite(rgb[0]) && IsFinite(rgb[1]) && IsFinite(rgb[2]));
	Assert((rgb[0] >= 0.0) && (rgb[1] >= 0.0) && (rgb[2] >= 0.0));
	Assert((rgb[0] <= 1.0) && (rgb[1] <= 1.0) && (rgb[2] <= 1.0));

	unsigned char* pSpecular = &m_pSpecular[m_nCurrentVertex * m_VertexSize_Specular];
	int col = (FastFToC(rgb[2])) | (FastFToC(rgb[1]) << 8) | (FastFToC(rgb[0]) << 16) | 0xFF000000;
	*(int*)pSpecular = col;
}

inline void	CVertexBuilder::Specular4f(float r, float g, float b, float a)
{
	Assert(m_pSpecular);
	Assert(IsFinite(r) && IsFinite(g) && IsFinite(b) && IsFinite(a));
	Assert((r >= 0.0) && (g >= 0.0) && (b >= 0.0) && (a >= 0.0));
	Assert((r <= 1.0) && (g <= 1.0) && (b <= 1.0) && (a <= 1.0f));

	unsigned char* pSpecular = &m_pSpecular[m_nCurrentVertex * m_VertexSize_Specular];
	int col = (FastFToC(b)) | (FastFToC(g) << 8) | (FastFToC(r) << 16) | (FastFToC(a) << 24);
	*(int*)pSpecular = col;
}

inline void	CVertexBuilder::Specular4fv(const float* rgb)
{
	Assert(rgb);
	Assert(m_pSpecular);
	Assert(IsFinite(rgb[0]) && IsFinite(rgb[1]) && IsFinite(rgb[2]) && IsFinite(rgb[3]));
	Assert((rgb[0] >= 0.0) && (rgb[1] >= 0.0) && (rgb[2] >= 0.0) && (rgb[3] >= 0.0));
	Assert((rgb[0] <= 1.0) && (rgb[1] <= 1.0) && (rgb[2] <= 1.0) && (rgb[3] <= 1.0));

	unsigned char* pSpecular = &m_pSpecular[m_nCurrentVertex * m_VertexSize_Specular];
	int col = (FastFToC(rgb[2])) | (FastFToC(rgb[1]) << 8) | (FastFToC(rgb[0]) << 16) | (FastFToC(rgb[3]) << 24);
	*(int*)pSpecular = col;
}

inline void CVertexBuilder::Specular3ub(unsigned char r, unsigned char g, unsigned char b)
{
	Assert(m_pSpecular);
	unsigned char* pSpecular = &m_pSpecular[m_nCurrentVertex * m_VertexSize_Specular];

#ifdef OPENGL_SWAP_COLORS
	int col = r | (g << 8) | (b << 16) | 0xFF000000;	      
#else
	int col = b | (g << 8) | (r << 16) | 0xFF000000;
#endif

	* (int*)pSpecular = col;
}

inline void CVertexBuilder::Specular3ubv(unsigned char const* c)
{
	Assert(m_pSpecular);
	unsigned char* pSpecular = &m_pSpecular[m_nCurrentVertex * m_VertexSize_Specular];

#ifdef OPENGL_SWAP_COLORS
	int col = c[0] | (c[1] << 8) | (c[2] << 16) | 0xFF000000;	      
#else
	int col = c[2] | (c[1] << 8) | (c[0] << 16) | 0xFF000000;
#endif

	* (int*)pSpecular = col;
}

inline void CVertexBuilder::Specular4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	Assert(m_pSpecular);
	unsigned char* pSpecular = &m_pSpecular[m_nCurrentVertex * m_VertexSize_Specular];

#ifdef OPENGL_SWAP_COLORS
	int col = r | (g << 8) | (b << 16) | (a << 24);	      
#else
	int col = b | (g << 8) | (r << 16) | (a << 24);
#endif

	* (int*)pSpecular = col;
}

inline void CVertexBuilder::Specular4ubv(unsigned char const* c)
{
	Assert(m_pSpecular);
	unsigned char* pSpecular = &m_pSpecular[m_nCurrentVertex * m_VertexSize_Specular];

#ifdef OPENGL_SWAP_COLORS
	int col = c[0] | (c[1] << 8) | (c[2] << 16) | (c[3] << 24);
#else
	int col = c[2] | (c[1] << 8) | (c[0] << 16) | (c[3] << 24);
#endif

	* (int*)pSpecular = col;
}


inline void CVertexBuilder::TexCoord1f(int nStage, float s)
{
	Assert(m_pTexCoord[nStage] && m_pCurrTexCoord[nStage]);
	Assert(IsFinite(s));

	float* pDst = m_pCurrTexCoord[nStage];
	*pDst = s;
}

inline void	CVertexBuilder::TexCoord2f(int nStage, float s, float t)
{
	Assert(m_pTexCoord[nStage] && m_pCurrTexCoord[nStage]);
	Assert(IsFinite(s) && IsFinite(t));

	float* pDst = m_pCurrTexCoord[nStage];
	*pDst++ = s;
	*pDst = t;
}

inline void	CVertexBuilder::TexCoord2fv(int nStage, const float* st)
{
	Assert(st);
	Assert(m_pTexCoord[nStage] && m_pCurrTexCoord[nStage]);
	Assert(IsFinite(st[0]) && IsFinite(st[1]));

	float* pDst = m_pCurrTexCoord[nStage];
	*pDst++ = *st++;
	*pDst = *st;
}

inline void	CVertexBuilder::TexCoord3f(int stage, float s, float t, float u)
{
	Assert(m_pTexCoord[stage] && m_pCurrTexCoord[stage]);
	Assert(IsFinite(s) && IsFinite(t) && IsFinite(u));
	float* pDst = m_pCurrTexCoord[stage];
	*pDst++ = s;
	*pDst++ = t;
	*pDst = u;
}

inline void	CVertexBuilder::TexCoord3fv(int stage, const float* stu)
{
	Assert(stu);
	Assert(m_pTexCoord[stage] && m_pCurrTexCoord[stage]);
	Assert(IsFinite(stu[0]) && IsFinite(stu[1]) && IsFinite(stu[2]));

	float* pDst = m_pCurrTexCoord[stage];
	*pDst++ = *stu++;
	*pDst++ = *stu++;
	*pDst = *stu;
}

inline void	CVertexBuilder::TexCoord4f(int stage, float s, float t, float u, float v)
{
	Assert(m_pTexCoord[stage] && m_pCurrTexCoord[stage]);
	Assert(IsFinite(s) && IsFinite(t) && IsFinite(u));
	float* pDst = m_pCurrTexCoord[stage];
	*pDst++ = s;
	*pDst++ = t;
	*pDst++ = u;
	*pDst = v;
}

inline void	CVertexBuilder::TexCoord4fv(int stage, const float* stuv)
{
	Assert(stuv);
	Assert(m_pTexCoord[stage] && m_pCurrTexCoord[stage]);
	Assert(IsFinite(stuv[0]) && IsFinite(stuv[1]) && IsFinite(stuv[2]));

	float* pDst = m_pCurrTexCoord[stage];
	*pDst++ = *stuv++;
	*pDst++ = *stuv++;
	*pDst++ = *stuv++;
	*pDst = *stuv;
}


inline void CVertexBuilder::TexCoordSubRect2f(int stage, float s, float t, float offsetS, float offsetT, float scaleS, float scaleT)
{
	Assert(m_pTexCoord[stage] && m_pCurrTexCoord[stage]);
	Assert(IsFinite(s) && IsFinite(t));

	float* pDst = m_pCurrTexCoord[stage];
	*pDst++ = (s * scaleS) + offsetS;
	*pDst = (t * scaleT) + offsetT;
}

inline void CVertexBuilder::TexCoordSubRect2fv(int stage, const float* st, const float* offset, const float* scale)
{
	Assert(st);
	Assert(m_pTexCoord[stage] && m_pCurrTexCoord[stage]);
	Assert(IsFinite(st[0]) && IsFinite(st[1]));

	float* pDst = m_pCurrTexCoord[stage];
	*pDst++ = (*st++ * *scale++) + *offset++;
	*pDst = (*st * *scale) + *offset;
}


inline void CVertexBuilder::TangentS3f(float sx, float sy, float sz)
{
	Assert(m_pTangentS);
	Assert(IsFinite(sx) && IsFinite(sy) && IsFinite(sz));

	float* pTangentS = OffsetFloatPointer(m_pTangentS, m_nCurrentVertex, m_VertexSize_TangentS);
	*pTangentS++ = sx;
	*pTangentS++ = sy;
	*pTangentS = sz;
}

inline void CVertexBuilder::TangentS3fv(const float* s)
{
	Assert(s);
	Assert(m_pTangentS);
	Assert(IsFinite(s[0]) && IsFinite(s[1]) && IsFinite(s[2]));

	float* pTangentS = OffsetFloatPointer(m_pTangentS, m_nCurrentVertex, m_VertexSize_TangentS);
	*pTangentS++ = *s++;
	*pTangentS++ = *s++;
	*pTangentS = *s;
}

inline void CVertexBuilder::TangentT3f(float tx, float ty, float tz)
{
	Assert(m_pTangentT);
	Assert(IsFinite(tx) && IsFinite(ty) && IsFinite(tz));

	float* pTangentT = OffsetFloatPointer(m_pTangentT, m_nCurrentVertex, m_VertexSize_TangentT);
	*pTangentT++ = tx;
	*pTangentT++ = ty;
	*pTangentT = tz;
}

inline void CVertexBuilder::TangentT3fv(const float* t)
{
	Assert(t);
	Assert(m_pTangentT);
	Assert(IsFinite(t[0]) && IsFinite(t[1]) && IsFinite(t[2]));

	float* pTangentT = OffsetFloatPointer(m_pTangentT, m_nCurrentVertex, m_VertexSize_TangentT);
	*pTangentT++ = *t++;
	*pTangentT++ = *t++;
	*pTangentT = *t;
}


inline void CVertexBuilder::Wrinkle1f(float flWrinkle)
{
	Assert(m_pWrinkle);
	Assert(IsFinite(flWrinkle));

	float* pWrinkle = OffsetFloatPointer(m_pWrinkle, m_nCurrentVertex, m_VertexSize_Wrinkle);
	*pWrinkle = flWrinkle;
}


inline void CVertexBuilder::BoneWeight(int idx, float weight)
{
	Assert(m_pBoneWeight);
	Assert(IsFinite(weight));
	Assert(idx >= 0);
	Assert(m_NumBoneWeights == 2);

	if (idx < m_NumBoneWeights)
	{
		float* pBoneWeight = OffsetFloatPointer(m_pBoneWeight, m_nCurrentVertex, m_VertexSize_BoneWeight);
		pBoneWeight[idx] = weight;
	}
}

inline void CVertexBuilder::BoneWeights2(float weight1, float weight2)
{
	Assert(m_pBoneWeight);
	Assert(IsFinite(weight1) && IsFinite(weight2));
	AssertOnce(m_NumBoneWeights == 2);

	float* pBoneWeight = OffsetFloatPointer(m_pBoneWeight, m_nCurrentVertex, m_VertexSize_BoneWeight);
	pBoneWeight[0] = weight1;
	pBoneWeight[1] = weight2;
}

inline void CVertexBuilder::BoneMatrix(int idx, int matrixIdx)
{
	Assert(m_pBoneMatrixIndex);
	Assert(idx >= 0);
	Assert(idx < 4);

	if (matrixIdx == BONE_MATRIX_INDEX_INVALID)
	{
		matrixIdx = 0;
	}
	Assert((matrixIdx >= 0) && (matrixIdx < 53));

#ifndef NEW_SKINNING
	unsigned char* pBoneMatrix = &m_pBoneMatrixIndex[m_nCurrentVertex * m_VertexSize_BoneMatrixIndex];
	if (IsX360())
	{
		idx = 3 - idx;
	}
	pBoneMatrix[idx] = (unsigned char)matrixIdx;
#else
	float* pBoneMatrix = &m_pBoneMatrixIndex[m_nCurrentVertex * m_VertexSize_BoneMatrixIndex];
	pBoneMatrix[idx] = matrixIdx;
#endif
}

inline void CVertexBuilder::BoneMatrices4(int matrixIdx0, int matrixIdx1, int matrixIdx2, int matrixIdx3)
{
	Assert(m_pBoneMatrixIndex);

	Assert(matrixIdx0 != BONE_MATRIX_INDEX_INVALID);
	Assert(matrixIdx1 != BONE_MATRIX_INDEX_INVALID);
	Assert(matrixIdx2 != BONE_MATRIX_INDEX_INVALID);
	Assert(matrixIdx3 != BONE_MATRIX_INDEX_INVALID);

#ifndef NEW_SKINNING
	int nVal;
	if (IsX360())
	{
		nVal = matrixIdx3 | (matrixIdx2 << 8) | (matrixIdx1 << 16) | (matrixIdx0 << 24);
	}
	else
	{
		nVal = matrixIdx0 | (matrixIdx1 << 8) | (matrixIdx2 << 16) | (matrixIdx3 << 24);
	}
	int* pBoneMatrix = (int*)(&m_pBoneMatrixIndex[m_nCurrentVertex * m_VertexSize_BoneMatrixIndex]);
	*pBoneMatrix = nVal;
#else
	float* pBoneMatrix = &m_pBoneMatrixIndex[m_nCurrentVertex * m_VertexSize_BoneMatrixIndex];
	pBoneMatrix[0] = matrixIdx0;
	pBoneMatrix[1] = matrixIdx1;
	pBoneMatrix[2] = matrixIdx2;
	pBoneMatrix[3] = matrixIdx3;
#endif
}

template <VertexCompressionType_t T> inline void CVertexBuilder::CompressedBoneWeight3fv(const float* pWeights)
{
	Assert(T == m_CompressionType);
	Assert(m_pBoneWeight);
	Assert(pWeights);

	float* pDestWeights = OffsetFloatPointer(m_pBoneWeight, m_nCurrentVertex, m_VertexSize_BoneWeight);

	if (T == VERTEX_COMPRESSION_ON)
	{
		Assert(m_NumBoneWeights <= 2);

		const int WEIGHT0_SHIFT = IsX360() ? 16 : 0;
		const int WEIGHT1_SHIFT = IsX360() ? 0 : 16;
		unsigned int* weights = (unsigned int*)pDestWeights;

		Assert(IsFinite(pWeights[0]) && (pWeights[0] >= 0.0f) && (pWeights[0] <= 1.0f));
		unsigned int weight0 = Float2Int(pWeights[0] * 32768.0f);
		*weights = (0x0000FFFF & (weight0 - 1)) << WEIGHT0_SHIFT;

#ifdef DEBUG
		if (m_NumBoneWeights == 1)
		{
			Assert(IsFinite(pWeights[1]) && (pWeights[1] >= 0.0f) && (pWeights[1] <= 1.0f));
			unsigned int weight1 = Float2Int(pWeights[1] * 32768.0f);
			Assert((weight0 + weight1) <= 32768);
		}
#endif

		if (m_NumBoneWeights > 1)
		{
			Assert(m_NumBoneWeights == 2);
			Assert(IsFinite(pWeights[1]) && (pWeights[1] >= 0.0f) && (pWeights[1] <= 1.0f));
			Assert(IsFinite(pWeights[2]) && (pWeights[2] >= 0.0f) && (pWeights[2] <= 1.0f));
			unsigned int weight1 = Float2Int(pWeights[1] * 32768.0f);
			unsigned int weight2 = Float2Int(pWeights[2] * 32768.0f);
			Assert((weight0 + weight1 + weight2) <= 32768);
			unsigned int residual = 32768 - (weight0 + weight1 + weight2);
			weight1 += residual;  
			*weights |= (0x0000FFFF & (weight1 - 1)) << WEIGHT1_SHIFT;
		}
	}
	else	  
	{
		pDestWeights[0] = pWeights[0];
		pDestWeights[1] = pWeights[1];
	}
}

inline void CVertexBuilder::UserData(const float* pData)
{
	Assert(m_CompressionType == VERTEX_COMPRESSION_NONE);           
	Assert(pData);

	int userDataSize = 4;  
	float* pUserData = OffsetFloatPointer(m_pUserData, m_nCurrentVertex, m_VertexSize_UserData);
	memcpy(pUserData, pData, sizeof(float) * userDataSize);
}

template <VertexCompressionType_t T> inline void CVertexBuilder::CompressedUserData(const float* pData)
{
	Assert(T == m_CompressionType);
	Assert(pData);
	Assert(IsFinite(pData[0]) && IsFinite(pData[1]) && IsFinite(pData[2]));
	Assert(pData[0] >= -1.05f && pData[0] <= 1.05f);
	Assert(pData[1] >= -1.05f && pData[1] <= 1.05f);
	Assert(pData[2] >= -1.05f && pData[2] <= 1.05f);
	Assert(pData[3] == +1.0f || pData[3] == -1.0f);
	if (T == VERTEX_COMPRESSION_ON)
	{
		float binormalSign = pData[3];

#if		( COMPRESSED_NORMALS_TYPE == COMPRESSED_NORMALS_SEPARATETANGENTS_SHORT2 )
		float* pUserData = OffsetFloatPointer(m_pUserData, m_nCurrentVertex, m_VertexSize_UserData);
		PackNormal_SHORT2(pData, (unsigned int*)pUserData, binormalSign);
#else      
		unsigned int existingNormalData = *(unsigned int*)m_pCurrNormal;
		Assert((existingNormalData & 0xFFFF0000) == 0);
#ifdef _DEBUG
		Assert(m_bWrittenNormal == true);
		m_bWrittenUserData = true;
#endif
		bool bIsTangent = true;
		unsigned int tangentData = 0;
		PackNormal_UBYTE4(pData, &tangentData, bIsTangent, binormalSign);
		*(unsigned int*)m_pCurrNormal = existingNormalData | tangentData;
#endif
	}
	else
	{
		int userDataSize = 4;  
		float* pUserData = OffsetFloatPointer(m_pUserData, m_nCurrentVertex, m_VertexSize_UserData);
		memcpy(pUserData, pData, sizeof(float) * userDataSize);
	}
}


class CIndexBuilder : private IndexDesc_t
{
public:
	CIndexBuilder();
	CIndexBuilder(IIndexBuffer* pIndexBuffer, MaterialIndexFormat_t fmt = MATERIAL_INDEX_FORMAT_UNKNOWN);
	~CIndexBuilder();

	bool Lock(int nMaxIndexCount, int nIndexOffset, bool bAppend = false);
	void Unlock();

	void SpewData();

	int GetRoomRemaining() const;

	void Bind(IMatRenderContext* pContext);

	int Offset() const;

	void Begin(IIndexBuffer* pIndexBuffer, int nMaxIndexCount, int nIndexOffset = 0);
	void End(bool bSpewData = false);

	void BeginModify(IIndexBuffer* pIndexBuffer, int firstIndex = 0, int numIndices = 0, int nIndexOffset = 0);
	void EndModify(bool bSpewData = false);

	int	IndexCount() const;

	int TotalIndexCount() const;

	void Reset();

	void SelectIndex(int nBufferIndex);

	void AdvanceIndex();
	void AdvanceIndices(int numIndices);

	int GetCurrentIndex();
	int GetFirstIndex() const;

	unsigned short const* Index() const;

	void Index(unsigned short nIndex);

	void FastIndex(unsigned short nIndex);

	void FastIndex2(unsigned short nIndex1, unsigned short nIndex2);

	void GenerateIndices(MaterialPrimitiveType_t primitiveType, int numIndices);

	void AttachBegin(IMesh* pMesh, int nMaxIndexCount, const MeshDesc_t& desc);
	void AttachEnd();
	void AttachBeginModify(IMesh* pMesh, int firstIndex, int numIndices, const MeshDesc_t& desc);
	void AttachEndModify();

	void FastTriangle(int startVert);
	void FastQuad(int startVert);
	void FastPolygon(int startVert, int numTriangles);
	void FastPolygonList(int startVert, int* pVertexCount, int polygonCount);
	void FastIndexList(const unsigned short* pIndexList, int startVert, int indexCount);

private:
	IIndexBuffer* m_pIndexBuffer;

	int				m_nMaxIndexCount;

	int				m_nIndexCount;

	int				m_nIndexOffset;

	mutable int		m_nCurrentIndex;

	int				m_nTotalIndexCount;

	unsigned int	m_nBufferOffset;
	unsigned int	m_nBufferFirstIndex;

	bool			m_bModify;
};


inline CIndexBuilder::CIndexBuilder() : m_pIndexBuffer(0), m_nMaxIndexCount(0),
m_nIndexCount(0), m_nCurrentIndex(0)
{
	m_nTotalIndexCount = 0;
	m_nBufferOffset = INVALID_BUFFER_OFFSET;
	m_nBufferFirstIndex = 0;
#ifdef _DEBUG
	m_bModify = false;
#endif
}

inline CIndexBuilder::CIndexBuilder(IIndexBuffer* pIndexBuffer, MaterialIndexFormat_t fmt)
{
	m_pIndexBuffer = pIndexBuffer;
	m_nBufferOffset = INVALID_BUFFER_OFFSET;
	m_nBufferFirstIndex = 0;
	m_nIndexCount = 0;
	m_nCurrentIndex = 0;
	m_nMaxIndexCount = 0;
	m_nTotalIndexCount = 0;
	if (m_pIndexBuffer->IsDynamic())
	{
		m_pIndexBuffer->BeginCastBuffer(fmt);
	}
	else
	{
		Assert(m_pIndexBuffer->IndexFormat() == fmt);
	}
#ifdef _DEBUG
	m_bModify = false;
#endif
}

inline CIndexBuilder::~CIndexBuilder()
{
	if (m_pIndexBuffer && m_pIndexBuffer->IsDynamic())
	{
		m_pIndexBuffer->EndCastBuffer();
	}
}


inline bool CIndexBuilder::Lock(int nMaxIndexCount, int nIndexOffset, bool bAppend)
{
	Assert(m_pIndexBuffer);
	m_bModify = false;
	m_nIndexOffset = nIndexOffset;
	m_nMaxIndexCount = nMaxIndexCount;
	m_nIndexCount = 0;
	bool bFirstLock = (m_nBufferOffset == INVALID_BUFFER_OFFSET);
	if (bFirstLock)
	{
		bAppend = false;
	}
	if (!bAppend)
	{
		m_nTotalIndexCount = 0;
	}
	Reset();

	if (!m_pIndexBuffer->Lock(m_nMaxIndexCount, bAppend, *this))
	{
		m_nMaxIndexCount = 0;
		return false;
	}

	if (bFirstLock)
	{
		m_nBufferOffset = m_nOffset;
		m_nBufferFirstIndex = m_nFirstIndex;
	}

	return true;
}

inline void CIndexBuilder::Unlock()
{
	Assert(!m_bModify && m_pIndexBuffer);

	m_pIndexBuffer->Unlock(m_nIndexCount, *this);
	m_nTotalIndexCount += m_nIndexCount;

	m_nMaxIndexCount = 0;

#ifdef _DEBUG
	memset((IndexDesc_t*)this, 0, sizeof(IndexDesc_t));
#endif
}

inline void CIndexBuilder::SpewData()
{
	m_pIndexBuffer->Spew(m_nIndexCount, *this);
}


inline void CIndexBuilder::Bind(IMatRenderContext* pContext)
{
	if (m_pIndexBuffer && (m_nBufferOffset != INVALID_BUFFER_OFFSET))
	{
		pContext->BindIndexBuffer(m_pIndexBuffer, m_nBufferOffset);
	}
	else
	{
		pContext->BindIndexBuffer(NULL, 0);
	}
}


inline int CIndexBuilder::Offset() const
{
	return m_nBufferOffset;
}

inline int CIndexBuilder::GetFirstIndex() const
{
	return m_nBufferFirstIndex;
}


inline void CIndexBuilder::Begin(IIndexBuffer* pIndexBuffer, int nMaxIndexCount, int nIndexOffset)
{
	Assert(pIndexBuffer && (!m_pIndexBuffer));

	m_pIndexBuffer = pIndexBuffer;
	m_nIndexCount = 0;
	m_nMaxIndexCount = nMaxIndexCount;
	m_nIndexOffset = nIndexOffset;

	m_bModify = false;

	m_pIndexBuffer->Lock(m_nMaxIndexCount, false, *this);

	Reset();
}

inline void CIndexBuilder::End(bool bSpewData)
{
	Assert(!m_bModify);

	if (bSpewData)
	{
		m_pIndexBuffer->Spew(m_nIndexCount, *this);
	}

	m_pIndexBuffer->Unlock(m_nIndexCount, *this);

	m_pIndexBuffer = 0;
	m_nMaxIndexCount = 0;

#ifdef _DEBUG
	memset((IndexDesc_t*)this, 0, sizeof(IndexDesc_t));
#endif
}


inline void CIndexBuilder::BeginModify(IIndexBuffer* pIndexBuffer, int nFirstIndex, int nIndexCount, int nIndexOffset)
{
	m_pIndexBuffer = pIndexBuffer;
	m_nIndexCount = nIndexCount;
	m_nMaxIndexCount = nIndexCount;
	m_nIndexOffset = nIndexOffset;
	m_bModify = true;

	m_pIndexBuffer->ModifyBegin(false, nFirstIndex, nIndexCount, *this);

	Reset();
}

inline void CIndexBuilder::EndModify(bool bSpewData)
{
	Assert(m_pIndexBuffer);
	Assert(m_bModify);	     

	if (bSpewData)
	{
		m_pIndexBuffer->Spew(m_nIndexCount, *this);
	}

	m_pIndexBuffer->ModifyEnd(*this);

	m_pIndexBuffer = 0;
	m_nMaxIndexCount = 0;

#ifdef _DEBUG
	memset((IndexDesc_t*)this, 0, sizeof(IndexDesc_t));
#endif
}


inline void CIndexBuilder::AttachBegin(IMesh* pMesh, int nMaxIndexCount, const MeshDesc_t& desc)
{
	m_pIndexBuffer = pMesh;
	m_nIndexCount = 0;
	m_nMaxIndexCount = nMaxIndexCount;

	m_bModify = false;

	m_nIndexOffset = desc.m_nFirstVertex;
	m_pIndices = desc.m_pIndices;
	m_nIndexSize = desc.m_nIndexSize;

	Reset();
}

inline void CIndexBuilder::AttachEnd()
{
	Assert(m_pIndexBuffer);
	Assert(!m_bModify);	     

	m_pIndexBuffer = 0;
	m_nMaxIndexCount = 0;

#ifdef _DEBUG
	memset((IndexDesc_t*)this, 0, sizeof(IndexDesc_t));
#endif
}

inline void CIndexBuilder::AttachBeginModify(IMesh* pMesh, int nFirstIndex, int nIndexCount, const MeshDesc_t& desc)
{
	m_pIndexBuffer = pMesh;
	m_nIndexCount = nIndexCount;
	m_nMaxIndexCount = nIndexCount;
	m_bModify = true;

	m_nIndexOffset = desc.m_nFirstVertex;
	m_pIndices = desc.m_pIndices;
	m_nIndexSize = desc.m_nIndexSize;

	Reset();
}

inline void CIndexBuilder::AttachEndModify()
{
	Assert(m_pIndexBuffer);
	Assert(m_bModify);	     

	m_pIndexBuffer = 0;
	m_nMaxIndexCount = 0;

#ifdef _DEBUG
	memset((IndexDesc_t*)this, 0, sizeof(IndexDesc_t));
#endif
}


inline void CIndexBuilder::Reset()
{
	m_nCurrentIndex = 0;
}


inline int CIndexBuilder::IndexCount() const
{
	return m_nIndexCount;
}


inline int CIndexBuilder::TotalIndexCount() const
{
	return m_nTotalIndexCount;
}


inline void CIndexBuilder::AdvanceIndex()
{
	m_nCurrentIndex += m_nIndexSize;
	if (m_nCurrentIndex > m_nIndexCount)
	{
		m_nIndexCount = m_nCurrentIndex;
	}
}

inline void CIndexBuilder::AdvanceIndices(int nIndices)
{
	m_nCurrentIndex += nIndices * m_nIndexSize;
	if (m_nCurrentIndex > m_nIndexCount)
	{
		m_nIndexCount = m_nCurrentIndex;
	}
}


inline int CIndexBuilder::GetCurrentIndex()
{
	return m_nCurrentIndex;
}

inline unsigned short const* CIndexBuilder::Index() const
{
	Assert(m_nCurrentIndex < m_nMaxIndexCount);
	return &m_pIndices[m_nCurrentIndex];
}

inline void CIndexBuilder::SelectIndex(int nIndex)
{
	Assert((nIndex >= 0) && (nIndex < m_nIndexCount));
	m_nCurrentIndex = nIndex * m_nIndexSize;
}


inline void CIndexBuilder::Index(unsigned short nIndex)
{
	Assert(m_pIndices);
	Assert(m_nCurrentIndex < m_nMaxIndexCount);
	m_pIndices[m_nCurrentIndex] = (unsigned short)(m_nIndexOffset + nIndex);
}

inline void CIndexBuilder::FastIndex(unsigned short nIndex)
{
	Assert(m_pIndices);
	Assert(m_nCurrentIndex < m_nMaxIndexCount);
	m_pIndices[m_nCurrentIndex] = (unsigned short)(m_nIndexOffset + nIndex);
	m_nCurrentIndex += m_nIndexSize;
	m_nIndexCount = m_nCurrentIndex;
}

FORCEINLINE void CIndexBuilder::FastTriangle(int startVert)
{
	startVert += m_nIndexOffset;
	unsigned short* pIndices = &m_pIndices[m_nCurrentIndex];
	*pIndices++ = startVert++;
	*pIndices++ = startVert++;
	*pIndices++ = startVert;
	AdvanceIndices(3);
}

FORCEINLINE void CIndexBuilder::FastQuad(int startVert)
{
	startVert += m_nIndexOffset;
	unsigned short* pIndices = &m_pIndices[m_nCurrentIndex];
	*pIndices++ = startVert++;
	*pIndices++ = startVert++;
	*pIndices++ = startVert;

	*pIndices++ = startVert - 2;
	*pIndices++ = startVert++;
	*pIndices++ = startVert;
	AdvanceIndices(6);
}

inline void CIndexBuilder::FastPolygon(int startVert, int triangleCount)
{
	unsigned short* pIndex = &m_pIndices[m_nCurrentIndex];
	startVert += m_nIndexOffset;
	if (!IsX360())
	{
		Assert(m_nIndexSize == 0 || m_nIndexSize == 1);
		triangleCount *= m_nIndexSize;
	}
	for (int v = 0; v < triangleCount; ++v)
	{
		*pIndex++ = startVert;
		*pIndex++ = startVert + v + 1;
		*pIndex++ = startVert + v + 2;
	}
	AdvanceIndices(triangleCount * 3);
}

inline void CIndexBuilder::FastPolygonList(int startVert, int* pVertexCount, int polygonCount)
{
	unsigned short* pIndex = &m_pIndices[m_nCurrentIndex];
	startVert += m_nIndexOffset;
	int indexOut = 0;

	if (!IsX360())
	{
		Assert(m_nIndexSize == 0 || m_nIndexSize == 1);
		polygonCount *= m_nIndexSize;
	}

	for (int i = 0; i < polygonCount; i++)
	{
		int vertexCount = pVertexCount[i];
		int triangleCount = vertexCount - 2;
		for (int v = 0; v < triangleCount; ++v)
		{
			*pIndex++ = startVert;
			*pIndex++ = startVert + v + 1;
			*pIndex++ = startVert + v + 2;
		}
		startVert += vertexCount;
		indexOut += triangleCount * 3;
	}
	AdvanceIndices(indexOut);
}

inline void CIndexBuilder::FastIndexList(const unsigned short* pIndexList, int startVert, int indexCount)
{
	unsigned short* pIndexOut = &m_pIndices[m_nCurrentIndex];
	startVert += m_nIndexOffset;
	if (!IsX360())
	{
		Assert(m_nIndexSize == 0 || m_nIndexSize == 1);
		indexCount *= m_nIndexSize;
	}
	for (int i = 0; i < indexCount; ++i)
	{
		pIndexOut[i] = startVert + pIndexList[i];
	}
	AdvanceIndices(indexCount);
}


inline void CIndexBuilder::FastIndex2(unsigned short nIndex1, unsigned short nIndex2)
{
	Assert(m_pIndices);
	Assert(m_nCurrentIndex < m_nMaxIndexCount - 1);
#ifndef _X360
	unsigned int nIndices = ((unsigned int)nIndex1 + m_nIndexOffset) | (((unsigned int)nIndex2 + m_nIndexOffset) << 16);
#else
	unsigned int nIndices = ((unsigned int)nIndex2 + m_nIndexOffset) | (((unsigned int)nIndex1 + m_nIndexOffset) << 16);
#endif

	* (int*)(&m_pIndices[m_nCurrentIndex]) = nIndices;
	m_nCurrentIndex += m_nIndexSize + m_nIndexSize;
	m_nIndexCount = m_nCurrentIndex;
}


inline void CIndexBuilder::GenerateIndices(MaterialPrimitiveType_t primitiveType, int nIndexCount)
{
	if (m_nIndexSize == 0)
		return;

	int nMaxIndices = m_nMaxIndexCount - m_nCurrentIndex;
	nIndexCount = MIN(nMaxIndices, nIndexCount);
	if (nIndexCount == 0)
		return;

	unsigned short* pIndices = &m_pIndices[m_nCurrentIndex];

	switch (primitiveType)
	{
	case MATERIAL_INSTANCED_QUADS:
		Assert(0);        
		break;
	case MATERIAL_QUADS:
		GenerateQuadIndexBuffer(pIndices, nIndexCount, m_nIndexOffset);
		break;
	case MATERIAL_POLYGON:
		GeneratePolygonIndexBuffer(pIndices, nIndexCount, m_nIndexOffset);
		break;
	case MATERIAL_LINE_STRIP:
		GenerateLineStripIndexBuffer(pIndices, nIndexCount, m_nIndexOffset);
		break;
	case MATERIAL_LINE_LOOP:
		GenerateLineLoopIndexBuffer(pIndices, nIndexCount, m_nIndexOffset);
		break;
	case MATERIAL_POINTS:
		Assert(0);        
		break;
	case MATERIAL_SUBD_QUADS_EXTRA:
	case MATERIAL_SUBD_QUADS_REG:
	default:
		GenerateSequentialIndexBuffer(pIndices, nIndexCount, m_nIndexOffset);
		break;
	}

	AdvanceIndices(nIndexCount);
}


class CMeshBuilder : public MeshDesc_t
{
public:
	CMeshBuilder();
	~CMeshBuilder() { Assert(!m_pMesh); }		          

	operator CIndexBuilder& () { return m_IndexBuilder; }

	void SetCompressionType(VertexCompressionType_t compressionType);

	void Begin(IMesh* pMesh, MaterialPrimitiveType_t type, int numPrimitives);

	void Begin(IMesh* pMesh, MaterialPrimitiveType_t type, int nVertexCount, int nIndexCount, int* nFirstVertex);
	void Begin(IMesh* pMesh, MaterialPrimitiveType_t type, int nVertexCount, int nIndexCount);

	void Begin(IVertexBuffer* pVertexBuffer, MaterialPrimitiveType_t type, int numPrimitives);
	void Begin(IVertexBuffer* pVertexBuffer, IIndexBuffer* pIndexBuffer, MaterialPrimitiveType_t type, int nVertexCount, int nIndexCount, int* nFirstVertex);
	void Begin(IVertexBuffer* pVertexBuffer, IIndexBuffer* pIndexBuffer, MaterialPrimitiveType_t type, int nVertexCount, int nIndexCount);

	void End(bool bSpewData = false, bool bDraw = false);

	void BeginModify(IMesh* pMesh, int nFirstVertex = 0, int nVertexCount = -1, int nFirstIndex = 0, int nIndexCount = 0);
	void EndModify(bool bSpewData = false);

	void DrawQuad(IMesh* pMesh, const float* v1, const float* v2,
		const float* v3, const float* v4, unsigned char const* pColor, bool wireframe = false);

	int VertexCount() const;
	int	IndexCount() const;

	void Reset();

	int VertexSize() { return m_ActualVertexSize; }

	int TextureCoordinateSize(int nTexCoordNumber) { return m_VertexSize_TexCoord[nTexCoordNumber]; }

	void* BaseVertexData();

	void SelectVertex(int idx);
	void SelectIndex(int idx);

	void SelectVertexFromIndex(int idx);

	void AdvanceVertex();
	template<int nFlags, int nNumTexCoords> void AdvanceVertexF();
	void AdvanceVertices(int nVerts);
	void AdvanceIndex();
	void AdvanceIndices(int nIndices);

	int GetCurrentVertex();
	int GetCurrentIndex();

	const float* Position() const;

	const float* Normal() const;

	unsigned int Color() const;

	unsigned char* Specular() const;

	const float* TexCoord(int stage) const;

	const float* TangentS() const;
	const float* TangentT() const;

	const float* BoneWeight() const;
	float Wrinkle() const;

	int NumBoneWeights() const;
#ifndef NEW_SKINNING
	unsigned char* BoneMatrix() const;
#else
	float* BoneMatrix() const;
#endif
	unsigned short const* Index() const;

	void Position3f(float x, float y, float z);
	void Position3fv(const float* v);

	void Normal3f(float nx, float ny, float nz);
	void Normal3fv(const float* n);
	void NormalDelta3fv(const float* n);
	void NormalDelta3f(float nx, float ny, float nz);

	template <VertexCompressionType_t T> void CompressedNormal3f(float nx, float ny, float nz);
	template <VertexCompressionType_t T> void CompressedNormal3fv(const float* n);

	void Color3f(float r, float g, float b);
	void Color3fv(const float* rgb);
	void Color4f(float r, float g, float b, float a);
	void Color4fv(const float* rgba);

	void Color3ub(unsigned char r, unsigned char g, unsigned char b);
	void Color3ubv(unsigned char const* rgb);
	void Color4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void Color4ubv(unsigned char const* rgba);
	void Color4Packed(int packedColor);
	int PackColor4(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	void Specular3f(float r, float g, float b);
	void Specular3fv(const float* rgb);
	void Specular4f(float r, float g, float b, float a);
	void Specular4fv(const float* rgba);

	void Specular3ub(unsigned char r, unsigned char g, unsigned char b);
	void Specular3ubv(unsigned char const* c);
	void Specular4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void Specular4ubv(unsigned char const* c);

	void TexCoord1f(int stage, float s);
	void TexCoord2f(int stage, float s, float t);
	void TexCoord2fv(int stage, const float* st);
	void TexCoord3f(int stage, float s, float t, float u);
	void TexCoord3fv(int stage, const float* stu);
	void TexCoord4f(int stage, float s, float t, float u, float w);
	void TexCoord4fv(int stage, const float* stuv);

	void TexCoordSubRect2f(int stage, float s, float t, float offsetS, float offsetT, float scaleS, float scaleT);
	void TexCoordSubRect2fv(int stage, const float* st, const float* offset, const float* scale);

	void TangentS3f(float sx, float sy, float sz);
	void TangentS3fv(const float* s);

	void TangentT3f(float tx, float ty, float tz);
	void TangentT3fv(const float* t);

	void Wrinkle1f(float flWrinkle);

	void BoneWeight(int idx, float weight);
	void BoneWeights2(float weight1, float weight2);

	template <VertexCompressionType_t T> void CompressedBoneWeight3fv(const float* pWeights);

	void BoneMatrix(int idx, int matrixIndex);
	void BoneMatrices4(int matrixIdx0, int matrixIdx1, int matrixIdx2, int matrixIdx3);

	void UserData(const float* pData);
	template <VertexCompressionType_t T> void CompressedUserData(const float* pData);

	void Index(unsigned short index);

	void FastIndex2(unsigned short nIndex1, unsigned short nIndex2);

	void FastIndex(unsigned short index);
	void FastQuad(int index);

	void FastVertex(const ModelVertexDX8_t& vertex);
	void FastVertexSSE(const ModelVertexDX8_t& vertex);
	void FastQuadVertexSSE(const QuadTessVertex_t& vertex);

	void FastAdvanceNVertices(int n);

#if defined( _X360 )
	void VertexDX8ToX360(const ModelVertexDX8_t& vertex);
#endif

	FORCEINLINE void* GetVertexDataPtr(int nWhatSizeIThinkItIs)
	{
		if (m_VertexBuilder.m_VertexSize_Position != nWhatSizeIThinkItIs)
			return NULL;
		return m_VertexBuilder.m_pCurrPosition;
	}


private:
	void ComputeNumVertsAndIndices(int* pMaxVertices, int* pMaxIndices,
		MaterialPrimitiveType_t type, int nPrimitiveCount);
	int IndicesFromVertices(MaterialPrimitiveType_t type, int nVertexCount);

	IMesh* m_pMesh;

	MaterialPrimitiveType_t m_Type;

	bool m_bGenerateIndices;

	CIndexBuilder	m_IndexBuilder;
	CVertexBuilder	m_VertexBuilder;
};


inline void CMeshBuilder::Begin(IVertexBuffer* pVertexBuffer, MaterialPrimitiveType_t type, int numPrimitives)
{
	Assert(0);
}

inline void CMeshBuilder::Begin(IVertexBuffer* pVertexBuffer, IIndexBuffer* pIndexBuffer, MaterialPrimitiveType_t type, int nVertexCount, int nIndexCount, int* nFirstVertex)
{
	Assert(0);
}

inline void CMeshBuilder::Begin(IVertexBuffer* pVertexBuffer, IIndexBuffer* pIndexBuffer, MaterialPrimitiveType_t type, int nVertexCount, int nIndexCount)
{
	Assert(0);
}


inline CMeshBuilder::CMeshBuilder() : m_pMesh(0), m_bGenerateIndices(false)
{
}


inline void CMeshBuilder::ComputeNumVertsAndIndices(int* pMaxVertices, int* pMaxIndices,
	MaterialPrimitiveType_t type, int nPrimitiveCount)
{
	switch (type)
	{
	case MATERIAL_POINTS:
		*pMaxVertices = *pMaxIndices = nPrimitiveCount;
		break;

	case MATERIAL_LINES:
		*pMaxVertices = *pMaxIndices = nPrimitiveCount * 2;
		break;

	case MATERIAL_LINE_STRIP:
		*pMaxVertices = nPrimitiveCount + 1;
		*pMaxIndices = nPrimitiveCount * 2;
		break;

	case MATERIAL_LINE_LOOP:
		*pMaxVertices = nPrimitiveCount;
		*pMaxIndices = nPrimitiveCount * 2;
		break;

	case MATERIAL_TRIANGLES:
		*pMaxVertices = *pMaxIndices = nPrimitiveCount * 3;
		break;

	case MATERIAL_TRIANGLE_STRIP:
		*pMaxVertices = *pMaxIndices = nPrimitiveCount + 2;
		break;

	case MATERIAL_QUADS:
		*pMaxVertices = nPrimitiveCount * 4;
		*pMaxIndices = nPrimitiveCount * 6;
		break;

	case MATERIAL_INSTANCED_QUADS:
		*pMaxVertices = nPrimitiveCount;
		*pMaxIndices = 0;     
		break;

	case MATERIAL_POLYGON:
		*pMaxVertices = nPrimitiveCount;
		*pMaxIndices = (nPrimitiveCount - 2) * 3;
		break;

	default:
		Assert(0);
	}

	Assert(*pMaxVertices <= 32768);
	Assert(*pMaxIndices <= 32768);
}


inline int CMeshBuilder::IndicesFromVertices(MaterialPrimitiveType_t type, int nVertexCount)
{
	switch (type)
	{
	case MATERIAL_QUADS:
		Assert((nVertexCount & 0x3) == 0);
		return (nVertexCount * 6) / 4;

	case MATERIAL_INSTANCED_QUADS:
		return 0;

	case MATERIAL_POLYGON:
		Assert(nVertexCount >= 3);
		return (nVertexCount - 2) * 3;

	case MATERIAL_LINE_STRIP:
		Assert(nVertexCount >= 2);
		return (nVertexCount - 1) * 2;

	case MATERIAL_LINE_LOOP:
		Assert(nVertexCount >= 3);
		return nVertexCount * 2;

	default:
		return nVertexCount;
	}
}

inline void CMeshBuilder::SetCompressionType(VertexCompressionType_t vertexCompressionType)
{
	m_CompressionType = vertexCompressionType;
	m_VertexBuilder.SetCompressionType(vertexCompressionType);
}

inline void CMeshBuilder::Begin(IMesh* pMesh, MaterialPrimitiveType_t type, int numPrimitives)
{
	Assert(pMesh && (!m_pMesh));
	Assert(type != MATERIAL_HETEROGENOUS);

	m_pMesh = pMesh;
	m_bGenerateIndices = true;
	m_Type = type;

	int nMaxVertexCount, nMaxIndexCount;
	ComputeNumVertsAndIndices(&nMaxVertexCount, &nMaxIndexCount, type, numPrimitives);

	switch (type)
	{
	case MATERIAL_INSTANCED_QUADS:
		m_pMesh->SetPrimitiveType(MATERIAL_INSTANCED_QUADS);
		break;

	case MATERIAL_QUADS:
	case MATERIAL_POLYGON:
		m_pMesh->SetPrimitiveType(MATERIAL_TRIANGLES);
		break;

	case MATERIAL_LINE_STRIP:
	case MATERIAL_LINE_LOOP:
		m_pMesh->SetPrimitiveType(MATERIAL_LINES);
		break;

	default:
		m_pMesh->SetPrimitiveType(type);
	}

	m_pMesh->LockMesh(nMaxVertexCount, nMaxIndexCount, *this, NULL);

	m_IndexBuilder.AttachBegin(pMesh, nMaxIndexCount, *this);
	m_VertexBuilder.AttachBegin(pMesh, nMaxVertexCount, *this);

	Reset();
}

inline void CMeshBuilder::Begin(IMesh* pMesh, MaterialPrimitiveType_t type, int nVertexCount, int nIndexCount, int* nFirstVertex)
{
	Begin(pMesh, type, nVertexCount, nIndexCount);

	*nFirstVertex = m_VertexBuilder.m_nFirstVertex * m_VertexBuilder.VertexSize();
}

inline void CMeshBuilder::Begin(IMesh* pMesh, MaterialPrimitiveType_t type, int nVertexCount, int nIndexCount)
{
	Assert(pMesh && (!m_pMesh));

	Assert((type != MATERIAL_QUADS) && (type != MATERIAL_INSTANCED_QUADS) && (type != MATERIAL_POLYGON) &&
		(type != MATERIAL_LINE_STRIP) && (type != MATERIAL_LINE_LOOP));

	Assert(type != MATERIAL_POINTS);

	m_pMesh = pMesh;
	m_bGenerateIndices = false;
	m_Type = type;

	m_pMesh->SetPrimitiveType(type);

	m_pMesh->LockMesh(nVertexCount, nIndexCount, *this, NULL);

	m_IndexBuilder.AttachBegin(pMesh, nIndexCount, *this);
	m_VertexBuilder.AttachBegin(pMesh, nVertexCount, *this);

	Reset();
}


inline void CMeshBuilder::End(bool bSpewData, bool bDraw)
{
	if (m_bGenerateIndices)
	{
		int nIndexCount = IndicesFromVertices(m_Type, m_VertexBuilder.VertexCount());
		m_IndexBuilder.GenerateIndices(m_Type, nIndexCount);
	}

	if (bSpewData)
	{
		m_pMesh->Spew(m_VertexBuilder.VertexCount(), m_IndexBuilder.IndexCount(), *this);
	}

#ifdef _DEBUG
	m_pMesh->ValidateData(m_VertexBuilder.VertexCount(), m_IndexBuilder.IndexCount(), *this);
#endif

	m_pMesh->UnlockMesh(m_VertexBuilder.VertexCount(), m_IndexBuilder.IndexCount(), *this);

	m_IndexBuilder.AttachEnd();
	m_VertexBuilder.AttachEnd();

	if (bDraw)
	{
		m_pMesh->Draw();
	}

	m_pMesh = 0;

#ifdef _DEBUG
	memset((MeshDesc_t*)this, 0, sizeof(MeshDesc_t));
#endif
}


inline void CMeshBuilder::BeginModify(IMesh* pMesh, int nFirstVertex, int nVertexCount, int nFirstIndex, int nIndexCount)
{
	Assert(pMesh && (!m_pMesh));

	if (nVertexCount < 0)
	{
		nVertexCount = pMesh->VertexCount();
	}

	m_pMesh = pMesh;
	m_bGenerateIndices = false;

	pMesh->ModifyBeginEx(false, nFirstVertex, nVertexCount, nFirstIndex, nIndexCount, *this);

	m_IndexBuilder.AttachBeginModify(pMesh, nFirstIndex, nIndexCount, *this);
	m_VertexBuilder.AttachBeginModify(pMesh, nFirstVertex, nVertexCount, *this);

	Reset();
}

inline void CMeshBuilder::EndModify(bool bSpewData)
{
	Assert(m_pMesh);

	if (bSpewData)
	{
		m_pMesh->Spew(m_VertexBuilder.VertexCount(), m_IndexBuilder.IndexCount(), *this);
	}
#ifdef _DEBUG
	m_pMesh->ValidateData(m_VertexBuilder.VertexCount(), m_IndexBuilder.IndexCount(), *this);
#endif

	m_pMesh->ModifyEnd(*this);
	m_pMesh = 0;

	m_IndexBuilder.AttachEndModify();
	m_VertexBuilder.AttachEndModify();

#ifdef _DEBUG
	memset((MeshDesc_t*)this, 0, sizeof(MeshDesc_t));
#endif
}


inline void CMeshBuilder::Reset()
{
	m_IndexBuilder.Reset();
	m_VertexBuilder.Reset();
}


FORCEINLINE void CMeshBuilder::SelectVertex(int nIndex)
{
	m_VertexBuilder.SelectVertex(nIndex);
}

inline void CMeshBuilder::SelectVertexFromIndex(int idx)
{
	int vertIdx = idx - m_nFirstVertex;
	SelectVertex(vertIdx);
}

FORCEINLINE void CMeshBuilder::SelectIndex(int idx)
{
	m_IndexBuilder.SelectIndex(idx);
}


template<int nFlags, int nNumTexCoords> FORCEINLINE void CMeshBuilder::AdvanceVertexF()
{
	m_VertexBuilder.AdvanceVertexF<nFlags, nNumTexCoords>();
}
FORCEINLINE void CMeshBuilder::AdvanceVertex()
{
	m_VertexBuilder.AdvanceVertex();
}

FORCEINLINE void CMeshBuilder::AdvanceVertices(int nVertexCount)
{
	m_VertexBuilder.AdvanceVertices(nVertexCount);
}

FORCEINLINE void CMeshBuilder::AdvanceIndex()
{
	m_IndexBuilder.AdvanceIndex();
}

FORCEINLINE void CMeshBuilder::AdvanceIndices(int nIndices)
{
	m_IndexBuilder.AdvanceIndices(nIndices);
}

FORCEINLINE int CMeshBuilder::GetCurrentVertex()
{
	return m_VertexBuilder.GetCurrentVertex();
}

FORCEINLINE int CMeshBuilder::GetCurrentIndex()
{
	return m_IndexBuilder.GetCurrentIndex();
}


inline void CMeshBuilder::DrawQuad(IMesh* pMesh, const float* v1, const float* v2,
	const float* v3, const float* v4, unsigned char const* pColor, bool wireframe)
{
	if (!wireframe)
	{
		Begin(pMesh, MATERIAL_TRIANGLE_STRIP, 2);

		Position3fv(v1);
		Color4ubv(pColor);
		AdvanceVertexF<VTX_HAVEPOS | VTX_HAVECOLOR, 0>();

		Position3fv(v2);
		Color4ubv(pColor);
		AdvanceVertexF<VTX_HAVEPOS | VTX_HAVECOLOR, 0>();

		Position3fv(v4);
		Color4ubv(pColor);
		AdvanceVertexF<VTX_HAVEPOS | VTX_HAVECOLOR, 0>();

		Position3fv(v3);
		Color4ubv(pColor);
		AdvanceVertexF<VTX_HAVEPOS | VTX_HAVECOLOR, 0>();
	}
	else
	{
		Begin(pMesh, MATERIAL_LINE_LOOP, 4);
		Position3fv(v1);
		Color4ubv(pColor);
		AdvanceVertexF<VTX_HAVEPOS | VTX_HAVECOLOR, 0>();

		Position3fv(v2);
		Color4ubv(pColor);
		AdvanceVertexF<VTX_HAVEPOS | VTX_HAVECOLOR, 0>();

		Position3fv(v3);
		Color4ubv(pColor);
		AdvanceVertexF<VTX_HAVEPOS | VTX_HAVECOLOR, 0>();

		Position3fv(v4);
		Color4ubv(pColor);
		AdvanceVertexF<VTX_HAVEPOS | VTX_HAVECOLOR, 0>();
	}

	End();
	pMesh->Draw();
}


FORCEINLINE int CMeshBuilder::VertexCount() const
{
	return m_VertexBuilder.VertexCount();
}

FORCEINLINE int CMeshBuilder::IndexCount() const
{
	return m_IndexBuilder.IndexCount();
}


FORCEINLINE void* CMeshBuilder::BaseVertexData()
{
	return m_VertexBuilder.BaseVertexData();
}

FORCEINLINE const float* CMeshBuilder::Position() const
{
	return m_VertexBuilder.Position();
}

FORCEINLINE const float* CMeshBuilder::Normal()	const
{
	return m_VertexBuilder.Normal();
}

FORCEINLINE unsigned int CMeshBuilder::Color() const
{
	return m_VertexBuilder.Color();
}

FORCEINLINE unsigned char* CMeshBuilder::Specular() const
{
	return m_VertexBuilder.Specular();
}

FORCEINLINE const float* CMeshBuilder::TexCoord(int nStage) const
{
	return m_VertexBuilder.TexCoord(nStage);
}

FORCEINLINE const float* CMeshBuilder::TangentS() const
{
	return m_VertexBuilder.TangentS();
}

FORCEINLINE const float* CMeshBuilder::TangentT() const
{
	return m_VertexBuilder.TangentT();
}

FORCEINLINE float CMeshBuilder::Wrinkle() const
{
	return m_VertexBuilder.Wrinkle();
}

FORCEINLINE const float* CMeshBuilder::BoneWeight() const
{
	return m_VertexBuilder.BoneWeight();
}

FORCEINLINE int CMeshBuilder::NumBoneWeights() const
{
	return m_VertexBuilder.NumBoneWeights();
}

FORCEINLINE unsigned short const* CMeshBuilder::Index() const
{
	return m_IndexBuilder.Index();
}


FORCEINLINE void CMeshBuilder::Index(unsigned short idx)
{
	m_IndexBuilder.Index(idx);
}


FORCEINLINE void CMeshBuilder::FastIndex(unsigned short idx)
{
	m_IndexBuilder.FastIndex(idx);
}

FORCEINLINE void CMeshBuilder::FastIndex2(unsigned short nIndex1, unsigned short nIndex2)
{
	m_IndexBuilder.FastIndex2(nIndex1, nIndex2);
}

FORCEINLINE void CMeshBuilder::FastQuad(int nIndex)
{
	m_IndexBuilder.FastQuad(nIndex);
}

FORCEINLINE void CMeshBuilder::FastAdvanceNVertices(int nVertexCount)
{
	m_VertexBuilder.FastAdvanceNVertices(nVertexCount);
}


FORCEINLINE void CMeshBuilder::FastVertex(const ModelVertexDX8_t& vertex)
{
	m_VertexBuilder.FastVertex(vertex);
}

FORCEINLINE void CMeshBuilder::FastVertexSSE(const ModelVertexDX8_t& vertex)
{
	m_VertexBuilder.FastVertexSSE(vertex);
}

FORCEINLINE void CMeshBuilder::FastQuadVertexSSE(const QuadTessVertex_t& vertex)
{
	m_VertexBuilder.FastQuadVertexSSE(vertex);
}


#if defined( _X360 )
inline void CMeshBuilder::VertexDX8ToX360(const ModelVertexDX8_t& vertex)
{
	m_VertexBuilder.VertexDX8ToX360(vertex);
}
#endif

FORCEINLINE void CMeshBuilder::Position3f(float x, float y, float z)
{
	m_VertexBuilder.Position3f(x, y, z);
}

FORCEINLINE void CMeshBuilder::Position3fv(const float* v)
{
	m_VertexBuilder.Position3fv(v);
}

FORCEINLINE void CMeshBuilder::Normal3f(float nx, float ny, float nz)
{
	m_VertexBuilder.Normal3f(nx, ny, nz);
}

FORCEINLINE void CMeshBuilder::Normal3fv(const float* n)
{
	m_VertexBuilder.Normal3fv(n);
}

FORCEINLINE void CMeshBuilder::NormalDelta3f(float nx, float ny, float nz)
{
	m_VertexBuilder.NormalDelta3f(nx, ny, nz);
}

FORCEINLINE void CMeshBuilder::NormalDelta3fv(const float* n)
{
	m_VertexBuilder.NormalDelta3fv(n);
}

FORCEINLINE void CMeshBuilder::Color3f(float r, float g, float b)
{
	m_VertexBuilder.Color3f(r, g, b);
}

FORCEINLINE void CMeshBuilder::Color3fv(const float* rgb)
{
	m_VertexBuilder.Color3fv(rgb);
}

FORCEINLINE void CMeshBuilder::Color4f(float r, float g, float b, float a)
{
	m_VertexBuilder.Color4f(r, g, b, a);
}

FORCEINLINE void CMeshBuilder::Color4fv(const float* rgba)
{
	m_VertexBuilder.Color4fv(rgba);
}

FORCEINLINE void CMeshBuilder::Color3ub(unsigned char r, unsigned char g, unsigned char b)
{
	m_VertexBuilder.Color3ub(r, g, b);
}

FORCEINLINE void CMeshBuilder::Color3ubv(unsigned char const* rgb)
{
	m_VertexBuilder.Color3ubv(rgb);
}

FORCEINLINE void CMeshBuilder::Color4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_VertexBuilder.Color4ub(r, g, b, a);
}

FORCEINLINE void CMeshBuilder::Color4ubv(unsigned char const* rgba)
{
	m_VertexBuilder.Color4ubv(rgba);
}

FORCEINLINE void CMeshBuilder::Color4Packed(int packedColor)
{
	m_VertexBuilder.Color4Packed(packedColor);
}

FORCEINLINE int CMeshBuilder::PackColor4(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return m_VertexBuilder.PackColor4(r, g, b, a);
}

FORCEINLINE void CMeshBuilder::Specular3f(float r, float g, float b)
{
	m_VertexBuilder.Specular3f(r, g, b);
}

FORCEINLINE void CMeshBuilder::Specular3fv(const float* rgb)
{
	m_VertexBuilder.Specular3fv(rgb);
}

FORCEINLINE void CMeshBuilder::Specular4f(float r, float g, float b, float a)
{
	m_VertexBuilder.Specular4f(r, g, b, a);
}

FORCEINLINE void CMeshBuilder::Specular4fv(const float* rgba)
{
	m_VertexBuilder.Specular4fv(rgba);
}

FORCEINLINE void CMeshBuilder::Specular3ub(unsigned char r, unsigned char g, unsigned char b)
{
	m_VertexBuilder.Specular3ub(r, g, b);
}

FORCEINLINE void CMeshBuilder::Specular3ubv(unsigned char const* c)
{
	m_VertexBuilder.Specular3ubv(c);
}

FORCEINLINE void CMeshBuilder::Specular4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_VertexBuilder.Specular4ub(r, g, b, a);
}

FORCEINLINE void CMeshBuilder::Specular4ubv(unsigned char const* c)
{
	m_VertexBuilder.Specular4ubv(c);
}

FORCEINLINE void CMeshBuilder::TexCoord1f(int nStage, float s)
{
	m_VertexBuilder.TexCoord1f(nStage, s);
}

FORCEINLINE void CMeshBuilder::TexCoord2f(int nStage, float s, float t)
{
	m_VertexBuilder.TexCoord2f(nStage, s, t);
}

FORCEINLINE void CMeshBuilder::TexCoord2fv(int nStage, const float* st)
{
	m_VertexBuilder.TexCoord2fv(nStage, st);
}

FORCEINLINE void CMeshBuilder::TexCoord3f(int nStage, float s, float t, float u)
{
	m_VertexBuilder.TexCoord3f(nStage, s, t, u);
}

FORCEINLINE void CMeshBuilder::TexCoord3fv(int nStage, const float* stu)
{
	m_VertexBuilder.TexCoord3fv(nStage, stu);
}

FORCEINLINE void CMeshBuilder::TexCoord4f(int nStage, float s, float t, float u, float v)
{
	m_VertexBuilder.TexCoord4f(nStage, s, t, u, v);
}

FORCEINLINE void CMeshBuilder::TexCoord4fv(int nStage, const float* stuv)
{
	m_VertexBuilder.TexCoord4fv(nStage, stuv);
}

FORCEINLINE void CMeshBuilder::TexCoordSubRect2f(int nStage, float s, float t, float offsetS, float offsetT, float scaleS, float scaleT)
{
	m_VertexBuilder.TexCoordSubRect2f(nStage, s, t, offsetS, offsetT, scaleS, scaleT);
}

FORCEINLINE void CMeshBuilder::TexCoordSubRect2fv(int nStage, const float* st, const float* offset, const float* scale)
{
	m_VertexBuilder.TexCoordSubRect2fv(nStage, st, offset, scale);
}

FORCEINLINE void CMeshBuilder::TangentS3f(float sx, float sy, float sz)
{
	m_VertexBuilder.TangentS3f(sx, sy, sz);
}

FORCEINLINE void CMeshBuilder::TangentS3fv(const float* s)
{
	m_VertexBuilder.TangentS3fv(s);
}

FORCEINLINE void CMeshBuilder::TangentT3f(float tx, float ty, float tz)
{
	m_VertexBuilder.TangentT3f(tx, ty, tz);
}

FORCEINLINE void CMeshBuilder::TangentT3fv(const float* t)
{
	m_VertexBuilder.TangentT3fv(t);
}

FORCEINLINE void CMeshBuilder::Wrinkle1f(float flWrinkle)
{
	m_VertexBuilder.Wrinkle1f(flWrinkle);
}

FORCEINLINE void CMeshBuilder::BoneWeight(int nIndex, float flWeight)
{
	m_VertexBuilder.BoneWeight(nIndex, flWeight);
}

FORCEINLINE void CMeshBuilder::BoneWeights2(float weight1, float weight2)
{
	m_VertexBuilder.BoneWeights2(weight1, weight2);
}

template <VertexCompressionType_t T> FORCEINLINE void CMeshBuilder::CompressedBoneWeight3fv(const float* pWeights)
{
	m_VertexBuilder.CompressedBoneWeight3fv<T>(pWeights);
}

FORCEINLINE void CMeshBuilder::BoneMatrix(int nIndex, int nMatrixIdx)
{
	m_VertexBuilder.BoneMatrix(nIndex, nMatrixIdx);
}

FORCEINLINE void CMeshBuilder::BoneMatrices4(int matrixIdx0, int matrixIdx1, int matrixIdx2, int matrixIdx3)
{
	m_VertexBuilder.BoneMatrices4(matrixIdx0, matrixIdx1, matrixIdx2, matrixIdx3);
}

FORCEINLINE void CMeshBuilder::UserData(const float* pData)
{
	m_VertexBuilder.UserData(pData);
}

template <VertexCompressionType_t T> FORCEINLINE void CMeshBuilder::CompressedUserData(const float* pData)
{
	m_VertexBuilder.CompressedUserData<T>(pData);
}

template <VertexCompressionType_t T> FORCEINLINE void CMeshBuilder::CompressedNormal3f(float nx, float ny, float nz)
{
	m_VertexBuilder.CompressedNormal3f<T>(nx, ny, nz);
}

template <VertexCompressionType_t T> FORCEINLINE void CMeshBuilder::CompressedNormal3fv(const float* n)
{
	m_VertexBuilder.CompressedNormal3fv<T>(n);
}

#endif  