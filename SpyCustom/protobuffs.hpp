#pragma once
#include <windows.h>
#include <string>

#include "VMT.hpp"

//#include "sdk/steam.h"
#include "sdk/steam_api.h"
#include "sdk/isteamgamecoordinator.h"
#include "intrin.h"

extern VMTHook* ProtoHook;

class Protobuffs
{
public:
	static void WritePacket(std::string packet, void* thisPtr, void* oldEBP, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize);
	void ReceiveMessage(void* thisPtr, void* oldEBP, uint32_t messageType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize);
	bool PreSendMessage(uint32_t& unMsgType, void* pubData, uint32_t& cubData);
	bool SendClientHello();
	bool SendMatchmakingClient2GCHello();
	bool SendClientGcRankUpdate();
	bool EquipWeapon(int weaponid, int classid, int slotid);
};


extern Protobuffs ProtoFeatures;

EGCResults __fastcall hkGCRetrieveMessage(void* ecx, void*, uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize);
EGCResults __fastcall hkGCSendMessage(void* ecx, void*, uint32_t unMsgType, const void* pubData, uint32_t cubData);


