#pragma once
#include <iostream>

template<typename FuncType>
__forceinline static FuncType CallVFunction(void* ppClass, int index)
{
	int* pVTable = *(int**)ppClass;
	int dwAddress = pVTable[index];
	return (FuncType)(dwAddress);
}

enum EGCResult
{
	k_EGCResultOK = 0,
	k_EGCResultNoMessage = 1,                  
	k_EGCResultBufferTooSmall = 2,               
	k_EGCResultNotLoggedOn = 3,                
	k_EGCResultInvalidMessage = 4,                
};

class ISteamGameCoordinator
{
public:
	virtual EGCResult GCSendMessage(int unMsgType, const void* pubData, int cubData) = 0;
	virtual bool IsMessageAvailable(int* pcubMsgSize) = 0;
	virtual EGCResult GCRetrieveMessage(int* punMsgType, void* pubDest, int cubDest, int* pcubMsgSize) = 0;

};

class CSteamID
{
public:
	CSteamID()
	{
		m_steamid.m_comp.m_unAccountID = 0;
		m_steamid.m_comp.m_EAccountType = 0;
		m_steamid.m_comp.m_EUniverse = 0;
		m_steamid.m_comp.m_unAccountInstance = 0;
	}
	uint32_t GetAccountID() const { return m_steamid.m_comp.m_unAccountID; }

private:
	union SteamID_t
	{
		struct SteamIDComponent_t
		{
			uint32_t			m_unAccountID : 32;			   
			unsigned int		m_unAccountInstance : 20;	         
			unsigned int		m_EAccountType : 4;			              
			int					m_EUniverse : 8;	     
		} m_comp;

		uint64_t m_unAll64Bits;
	} m_steamid;
};

class ISteamUser
{
public:
	virtual uint32_t GetHSteamUser() = 0;
	virtual bool BLoggedOn() = 0;
	virtual CSteamID GetSteamID() = 0;
};

class ISteamClient
{
public:
	ISteamUser* GetISteamUser(void* hSteamUser, void* hSteamPipe, const char* pchVersion)
	{
		typedef ISteamUser* (__stdcall* func)(void*, void*, const char*);
		return CallVFunction<func>(this, 5)(hSteamUser, hSteamPipe, pchVersion);
	}

	ISteamGameCoordinator* GetISteamGenericInterface(void* hSteamUser, void* hSteamPipe, const char* pchVersion)
	{
		typedef ISteamGameCoordinator* (__stdcall* func)(void*, void*, const char*);
		return CallVFunction<func>(this, 12)(hSteamUser, hSteamPipe, pchVersion);
	}
};