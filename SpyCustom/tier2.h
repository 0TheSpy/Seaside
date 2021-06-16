
#ifndef TIER2_H
#define TIER2_H

#if defined( _WIN32 )
#pragma once
#endif

#include "tier1.h"


void ConnectTier2Libraries(CreateInterfaceFn* pFactoryList, int nFactoryCount);
void DisconnectTier2Libraries();


void InitDefaultFileSystem(void);
void ShutdownDefaultFileSystem(void);


void InitCommandLineProgram(int& argc, char**& argv);


template< class IInterface, int ConVarFlag = 0 >
class CTier2AppSystem : public CTier1AppSystem< IInterface, ConVarFlag >
{
	typedef CTier1AppSystem< IInterface, ConVarFlag > BaseClass;

public:
	virtual bool Connect(CreateInterfaceFn factory)
	{
		if (!BaseClass::Connect(factory))
			return false;

		ConnectTier2Libraries(&factory, 1);
		return true;
	}

	virtual InitReturnVal_t Init()
	{
		InitReturnVal_t nRetVal = BaseClass::Init();
		if (nRetVal != INIT_OK)
			return nRetVal;

		return INIT_OK;
	}

	
	virtual AppSystemTier_t GetTier()
	{
		return APP_SYSTEM_TIER2;
	}
	virtual void Shutdown()
	{
		BaseClass::Shutdown();
	}

	virtual void Disconnect()
	{
		DisconnectTier2Libraries();
		BaseClass::Disconnect();
	}
};


enum FadeMode_t
{
	FADE_MODE_NONE = 0,
	FADE_MODE_LOW,
	FADE_MODE_MED,
	FADE_MODE_HIGH,
	FADE_MODE_360,
	FADE_MODE_PS3,
	FADE_MODE_LEVEL,

	FADE_MODE_COUNT,
};

struct FadeData_t
{
	float	m_flPixelMin;		           
	float	m_flPixelMax;		           
	float	m_flWidth;			           
	float	m_flFadeDistScale;	         
};

extern FadeData_t g_aFadeData[FADE_MODE_COUNT];


extern uint32 g_nResourceFrameCount;


#endif  