#include "Protobuffs.hpp"
#include "Interfaces.hpp"
#include "inventory_changer.h"

extern Protobuffs ProtoFeatures;
#define CAST(cast, address, add) reinterpret_cast<cast>((uint32_t)address + (uint32_t)add)

void Protobuffs::WritePacket(std::string packet, void* thisPtr, void* oldEBP, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize)
{
	if ((uint32_t)packet.size() <= cubDest - 8)
	{
		memcpy((void*)((DWORD)pubDest + 8), (void*)packet.data(), packet.size());
		*pcubMsgSize = packet.size() + 8;
	}
	else if (iff.g_pMemAlloc)
	{
		auto memPtr = *CAST(void**, thisPtr, 0x18);
		auto memPtrSize = *CAST(uint32_t*, thisPtr, 0x1C);
		auto newSize = (memPtrSize - cubDest) + packet.size() + 8;

		auto memory = iff.g_pMemAlloc->Realloc(memPtr, newSize + 4);

		*CAST(void**, thisPtr, 0x18) = memory;
		*CAST(uint32_t*, thisPtr, 0x1C) = newSize;
		*CAST(void**, oldEBP, -0x14) = memory;

		memcpy(CAST(void*, memory, 0x1C), (void*)packet.data(), packet.size());

		*pcubMsgSize = packet.size() + 8;
	}
}

static bool onceChanger = false;

void Protobuffs::ReceiveMessage(void* thisPtr, void* oldEBP, uint32_t messageType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize)
{
	if (messageType == k_EMsgGCCStrike15_v2_MatchmakingGC2ClientHello)
	{
		printfdbg("Packet == k_EMsgGCCStrike15_v2_MatchmakingGC2ClientHello\n");

		if (g_Options.profile_active) {

			MatchmakingGC2ClientHello msg((void*)((DWORD)pubDest + 8), *pcubMsgSize - 8);
			MatchmakingGC2ClientHello::PlayerCommendationInfo commendations;
			commendations.cmd_friendly().set(g_Options.comfriendly);
			commendations.cmd_teaching().set(g_Options.comteacher);
			commendations.cmd_leader().set(g_Options.comleader);
			msg.commendation().set(commendations);
		
			msg.player_level().set(g_Options.level);
			msg.player_cur_xp().set(g_Options.xp);

			if (g_Options.banduration != 0) {
				msg.penalty_reason().set(g_Options.banreason);
				msg.penalty_seconds().set(g_Options.banduration);
			}

			msg.vac_banned().set(g_Options.vacban);

			MatchmakingGC2ClientHello::PlayerRankingInfo ranking;
			ranking.account_id().set(iff.g_SteamUser->GetSteamID().GetAccountID());
			ranking.rank_type_id().set(6);
			ranking.rank_id().set(g_Options.rankz.value->arr[0].rank);
			ranking.wins().set(g_Options.rankz.value->arr[0].wins);
			msg.ranking().set(ranking);

			auto packet = msg.serialize();
			WritePacket(packet, thisPtr, oldEBP, pubDest, cubDest, pcubMsgSize);

			printfdbg("send packet 0 6 rank %d wins %d\n", g_Options.rankz.value->arr[0].rank, g_Options.rankz.value->arr[0].wins);

		}

		
		
	}
	else if (messageType == k_EMsgGCCStrike15_v2_ClientGCRankUpdate)
	{
		printfdbg("Packet == k_EMsgGCCStrike15_v2_ClientGCRankUpdate\n");
		 
		if (g_Options.profile_active) {
			CMsgGCCStrike15_v2_ClientGCRankUpdate msg((void*)((DWORD)pubDest + 8), *pcubMsgSize - 8);

			auto ranking = msg.ranking().get();

			int ranktype = ranking.rank_type_id().get(); 
			printfdbg("ranktype = %d\n", ranktype); 
			int rankcount = 0;
			if (ranktype == 7) rankcount = 1;
			if (ranktype == 10) rankcount = 2;
			
			ranking.rank_id().set(g_Options.rankz.value->arr[rankcount].rank);
			ranking.wins().set(g_Options.rankz.value->arr[rankcount].wins);

			msg.ranking().set(ranking);

			auto packet = msg.serialize();
			WritePacket(packet, thisPtr, oldEBP, pubDest, cubDest, pcubMsgSize);

		}

	}
	else if (messageType == k_EMsgGCClientWelcome)
	{ 
		printfdbg("Packet == k_EMsgGCClientWelcome\n"); 
		auto packet = inventory_changer(pubDest, pcubMsgSize);
		WritePacket(packet, thisPtr, oldEBP, pubDest, cubDest, pcubMsgSize); 
	} 
	printfdbg(".GC Receive: %d\n", messageType); 
}

bool Protobuffs::PreSendMessage(uint32_t& unMsgType, void* pubData, uint32_t& cubData)
{
	uint32_t MessageType = unMsgType & 0x7FFFFFFF; 
	printfdbg(".GC Sent: %d\n", MessageType); 
	return true;
}

bool Protobuffs::SendClientHello()
{
	CMsgClientHello msg;
	msg.client_session_need().set(1);
	auto packet = msg.serialize();

	void* ptr = malloc(packet.size() + 8);

	if (!ptr)
		return false;

	((uint32_t*)ptr)[0] = k_EMsgGCClientHello | ((DWORD)1 << 31);
	((uint32_t*)ptr)[1] = 0;

	memcpy((void*)((DWORD)ptr + 8), (void*)packet.data(), packet.size());
	bool result = iff.g_SteamGameCoordinator->GCSendMessage(k_EMsgGCClientHello | ((DWORD)1 << 31), ptr, packet.size() + 8) == k_EGCResultOK;
	free(ptr);

	return result;
}

bool Protobuffs::SendMatchmakingClient2GCHello()
{
	ProtoWriter msg(0);
	auto packet = msg.serialize();
	void* ptr = malloc(packet.size() + 8);

	if (!ptr)
		return false;

	((uint32_t*)ptr)[0] = k_EMsgGCCStrike15_v2_MatchmakingClient2GCHello | ((DWORD)1 << 31);
	((uint32_t*)ptr)[1] = 0;

	memcpy((void*)((DWORD)ptr + 8), (void*)packet.data(), packet.size());
	bool result = iff.g_SteamGameCoordinator->GCSendMessage(k_EMsgGCCStrike15_v2_MatchmakingClient2GCHello | ((DWORD)1 << 31), ptr, packet.size() + 8) == k_EGCResultOK;
	free(ptr);

	return result;
}

bool Protobuffs::SendClientGcRankUpdate()
{
	MatchmakingGC2ClientHello::PlayerRankingInfo rank_wingman;
	rank_wingman.rank_type_id().set(7);       

	CMsgGCCStrike15_v2_ClientGCRankUpdate msg;
	msg.ranking().set(rank_wingman);

	auto packet = msg.serialize();

	void* ptr = malloc(packet.size() + 8);

	if (!ptr)
		return false;

	((uint32_t*)ptr)[0] = k_EMsgGCCStrike15_v2_ClientGCRankUpdate | ((DWORD)1 << 31);
	((uint32_t*)ptr)[1] = 0;

	memcpy((void*)((DWORD)ptr + 8), (void*)packet.data(), packet.size());
	bool result = iff.g_SteamGameCoordinator->GCSendMessage(k_EMsgGCCStrike15_v2_ClientGCRankUpdate | ((DWORD)1 << 31), ptr, packet.size() + 8) == k_EGCResultOK;
	free(ptr);

	return result;
}

bool Protobuffs::EquipWeapon(int weaponid, int classid, int slotid)
{
	CMsgAdjustItemEquippedState msg;
	msg.item_id().set(START_ITEM_INDEX + weaponid);
	msg.new_class().set(classid);
	msg.new_slot().set(slotid);
	msg.swap().set(true);
	auto packet = msg.serialize();

	void* ptr = malloc(packet.size() + 8);

	if (!ptr)
		return false;

	((uint32_t*)ptr)[0] = k_EMsgGCAdjustItemEquippedState | ((DWORD)1 << 31);
	((uint32_t*)ptr)[1] = 0;

	memcpy((void*)((DWORD)ptr + 8), (void*)packet.data(), packet.size());
	bool result = iff.g_SteamGameCoordinator->GCSendMessage(k_EMsgGCAdjustItemEquippedState | ((DWORD)1 << 31), ptr, packet.size() + 8) == k_EGCResultOK;
	free(ptr);

	return result;
}





VMTHook* ProtoHook = nullptr;
Protobuffs ProtoFeatures;



EGCResult __fastcall hkGCRetrieveMessage(void* ecx, void*, uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize)
{
	static auto oGCRetrieveMessage = ProtoHook->GetOriginal<EGCResult(__thiscall*)(void*, uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize)>(2);

	auto status = oGCRetrieveMessage(ecx, punMsgType, pubDest, cubDest, pcubMsgSize);


	if (status == k_EGCResultOK)
	{
		void* thisPtr = nullptr;
		__asm mov thisPtr, ebx;
		auto oldEBP = *reinterpret_cast<void**>((uint32_t)_AddressOfReturnAddress() - 4);

		uint32_t messageType = *punMsgType & 0x7FFFFFFF;
		ProtoFeatures.ReceiveMessage(thisPtr, oldEBP, messageType, pubDest, cubDest, pcubMsgSize);
	}


	return status;
}

EGCResult __fastcall hkGCSendMessage(void* ecx, void*, uint32_t unMsgType, const void* pubData, uint32_t cubData)
{
	static auto oGCSendMessage = ProtoHook->GetOriginal<EGCResult(__thiscall*)(void*, uint32_t unMsgType, const void* pubData, uint32_t cubData)>(0);

	bool sendMessage = ProtoFeatures.PreSendMessage(unMsgType, const_cast<void*>(pubData), cubData);

	if (!sendMessage)
		return k_EGCResultOK;


	return oGCSendMessage(ecx, unMsgType, const_cast<void*>(pubData), cubData);
}
