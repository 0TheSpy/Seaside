#ifndef MATERIALSYSTEMUTIL_H
#define MATERIALSYSTEMUTIL_H

#ifdef _WIN32
#pragma once
#endif

#include "imageformat.h"   
#include "imaterialsystem.h"      

class IMaterial;
class ITexture;
class KeyValues;

class KeyValues;


class CMaterialReference
{
public:
	CMaterialReference(char const* pMaterialName = 0, const char* pTextureGroupName = 0, bool bComplain = true);
	~CMaterialReference();

	void Init(const char* pMaterialName, const char* pTextureGroupName, bool bComplain = true);
	void Init(const char* pMaterialName, KeyValues* pVMTKeyValues);
	void Init(IMaterial* pMaterial);
	void Init(CMaterialReference& ref);
	void Init(const char* pMaterialName, const char* pTextureGroupName, KeyValues* pVMTKeyValues);

	void Shutdown();
	bool IsValid() { return m_pMaterial != 0; }

	operator IMaterial* () { return m_pMaterial; }
	operator IMaterial* () const { return m_pMaterial; }
	operator IMaterial const* () const { return m_pMaterial; }
	IMaterial* operator->() { return m_pMaterial; }

private:
	IMaterial* m_pMaterial;
};

class CTextureReference
{
public:
	CTextureReference();
	CTextureReference(const CTextureReference& ref);
	~CTextureReference();

	void Init(char const* pTexture, const char* pTextureGroupName, bool bComplain = true);
	void InitProceduralTexture(const char* pTextureName, const char* pTextureGroupName, int w, int h, ImageFormat fmt, int nFlags);
	void InitRenderTarget(int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat fmt, MaterialRenderTargetDepth_t depth, bool bHDR, char* pStrOptionalName = NULL);
#if defined( _X360 )
	void InitRenderTargetTexture(int width, int height, RenderTargetSizeMode_t sizeMode, ImageFormat fmt, MaterialRenderTargetDepth_t depth, bool bHDR, char* pStrOptionalName = NULL);
	void InitRenderTargetSurface(int width, int height, ImageFormat fmt, bool bSameAsTexture);
#endif
	void Init(ITexture* pTexture);

	void Shutdown(bool bDeleteIfUnReferenced = false);
	bool IsValid() { return m_pTexture != 0; }

	operator ITexture* () { return m_pTexture; }
	operator ITexture const* () const { return m_pTexture; }
	ITexture* operator->() { return m_pTexture; }

	void operator=(CTextureReference& ref);

private:
	ITexture* m_pTexture;
};


#endif  