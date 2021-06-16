#ifndef IMATERIALSYSTEMHARDWARECONFIG_H
#define IMATERIALSYSTEMHARDWARECONFIG_H

#ifdef _WIN32
#pragma once
#endif

#if defined( DX_TO_GL_ABSTRACTION )
#define IsPlatformOpenGL() true
#else
#define IsPlatformOpenGL() false
#endif

#include "interface.h"
#include "imageformat.h"
#include "imaterialsystem.h"
FORCEINLINE bool IsOpenGL(void)
{
	return IsPlatformOpenGL();
}

enum VertexCompressionType_t
{
	VERTEX_COMPRESSION_INVALID = 0xFFFFFFFF,

	VERTEX_COMPRESSION_NONE = 0,

	VERTEX_COMPRESSION_ON = 1
};


#ifdef _GAMECONSOLE
#define DEFCONFIGMETHOD( ret_type, method, xbox_return_value )		\
FORCEINLINE ret_type method const 									\
{																	\
	return xbox_return_value;										\
}


#else
#define DEFCONFIGMETHOD( ret_type, method, xbox_return_value )	\
virtual ret_type method const = 0;
#endif

enum ShadowFilterMode_t
{
	SHADOWFILTERMODE_DEFAULT = 0,

	NVIDIA_PCF = 0,
	ATI_NO_PCF_FETCH4 = 1,
	NVIDIA_PCF_CHEAP = 2,
	ATI_NOPCF = 3,

	GAMECONSOLE_NINE_TAP_PCF = 0,
	GAMECONSOLE_SINGLE_TAP_PCF = 1,

#if defined( _GAMECONSOLE )
	SHADOWFILTERMODE_FIRST_CHEAP_MODE = GAMECONSOLE_SINGLE_TAP_PCF,
#else
	SHADOWFILTERMODE_FIRST_CHEAP_MODE = NVIDIA_PCF_CHEAP,
#endif
};

enum CSMQualityMode_t
{
	CSMQUALITY_VERY_LOW,
	CSMQUALITY_LOW,
	CSMQUALITY_MEDIUM,
	CSMQUALITY_HIGH,

	CSMQUALITY_TOTAL_MODES
};

enum CSMShaderMode_t
{
	CSMSHADERMODE_LOW_OR_VERY_LOW = 0,
	CSMSHADERMODE_MEDIUM = 1,
	CSMSHADERMODE_HIGH = 2,
	CSMSHADERMODE_ATIFETCH4 = 3,

	CSMSHADERMODE_TOTAL_MODES
};

class IMaterialSystemHardwareConfig
{
public:
	virtual int	 GetFrameBufferColorDepth() const = 0;
	virtual int  GetSamplerCount() const = 0;
	virtual bool HasSetDeviceGammaRamp() const = 0;
	DEFCONFIGMETHOD(bool, SupportsStaticControlFlow(), true);
	virtual VertexCompressionType_t SupportsCompressedVertices() const = 0;
	virtual int  MaximumAnisotropicLevel() const = 0;	     
	virtual int  MaxTextureWidth() const = 0;
	virtual int  MaxTextureHeight() const = 0;
	virtual int	 TextureMemorySize() const = 0;
	virtual bool SupportsMipmappedCubemaps() const = 0;

	virtual int	 NumVertexShaderConstants() const = 0;
	virtual int	 NumPixelShaderConstants() const = 0;
	virtual int	 MaxNumLights() const = 0;
	virtual int	 MaxTextureAspectRatio() const = 0;
	virtual int	 MaxVertexShaderBlendMatrices() const = 0;
	virtual int	 MaxUserClipPlanes() const = 0;
	virtual bool UseFastClipping() const = 0;

	DEFCONFIGMETHOD(int, GetDXSupportLevel(), 98);
	virtual const char* GetShaderDLLName() const = 0;

	virtual bool ReadPixelsFromFrontBuffer() const = 0;

	virtual bool PreferDynamicTextures() const = 0;

	DEFCONFIGMETHOD(bool, SupportsHDR(), true);

	virtual bool NeedsAAClamp() const = 0;
	virtual bool NeedsATICentroidHack() const = 0;

	virtual int	 GetMaxDXSupportLevel() const = 0;

	virtual bool SpecifiesFogColorInLinearSpace() const = 0;

	DEFCONFIGMETHOD(bool, SupportsSRGB(), true);
	DEFCONFIGMETHOD(bool, FakeSRGBWrite(), false);
	DEFCONFIGMETHOD(bool, CanDoSRGBReadFromRTs(), true);

	virtual bool SupportsGLMixedSizeTargets() const = 0;

	virtual bool IsAAEnabled() const = 0;	    

	virtual int GetVertexSamplerCount() const = 0;
	virtual int GetMaxVertexTextureDimension() const = 0;

	virtual int  MaxTextureDepth() const = 0;

	virtual HDRType_t GetHDRType() const = 0;
	virtual HDRType_t GetHardwareHDRType() const = 0;

	virtual bool SupportsStreamOffset() const = 0;

	virtual int StencilBufferBits() const = 0;
	virtual int MaxViewports() const = 0;

	virtual void OverrideStreamOffsetSupport(bool bOverrideEnabled, bool bEnableSupport) = 0;

	virtual ShadowFilterMode_t GetShadowFilterMode(bool bForceLowQualityShadows, bool bPS30) const = 0;

	virtual int NeedsShaderSRGBConversion() const = 0;

	DEFCONFIGMETHOD(bool, UsesSRGBCorrectBlending(), IsX360());

	virtual bool HasFastVertexTextures() const = 0;
	virtual int MaxHWMorphBatchCount() const = 0;

	virtual bool SupportsHDRMode(HDRType_t nHDRMode) const = 0;

	virtual bool GetHDREnabled(void) const = 0;
	virtual void SetHDREnabled(bool bEnable) = 0;

	virtual bool SupportsBorderColor(void) const = 0;
	virtual bool SupportsFetch4(void) const = 0;

	virtual float GetShadowDepthBias() const = 0;
	virtual float GetShadowSlopeScaleDepthBias() const = 0;

	virtual bool PreferZPrepass() const = 0;

	virtual bool SuppressPixelShaderCentroidHackFixup() const = 0;
	virtual bool PreferTexturesInHWMemory() const = 0;
	virtual bool PreferHardwareSync() const = 0;
	virtual bool ActualHasFastVertexTextures() const = 0;

	virtual bool SupportsShadowDepthTextures(void) const = 0;
	virtual ImageFormat GetShadowDepthTextureFormat(void) const = 0;
	virtual ImageFormat GetHighPrecisionShadowDepthTextureFormat(void) const = 0;
	virtual ImageFormat GetNullTextureFormat(void) const = 0;
	virtual int	GetMinDXSupportLevel() const = 0;
	virtual bool IsUnsupported() const = 0;

	virtual float GetLightMapScaleFactor() const = 0;

	virtual bool SupportsCascadedShadowMapping() const = 0;
	virtual CSMQualityMode_t GetCSMQuality() const = 0;
	virtual bool SupportsBilinearPCFSampling() const = 0;
	virtual CSMShaderMode_t GetCSMShaderMode(CSMQualityMode_t nQualityLevel) const = 0;
	virtual bool GetCSMAccurateBlending(void) const = 0;
	virtual void SetCSMAccurateBlending(bool bEnable) = 0;

	virtual bool SupportsResolveDepth() const = 0;
	virtual bool HasFullResolutionDepthTexture() const = 0;

#if defined ( STDSHADER_DBG_DLL_EXPORT ) || defined( STDSHADER_DX9_DLL_EXPORT )
	inline bool SupportsPixelShaders_2_b() const { return GetDXSupportLevel() >= 92; }
	inline bool SupportsPixelShaders_3_0() const { return GetDXSupportLevel() >= 95; }
#endif

	inline bool ShouldAlwaysUseShaderModel2bShaders() const { return IsOpenGL(); }
	inline bool PlatformRequiresNonNullPixelShaders() const { return IsOpenGL(); }
};

#endif  
