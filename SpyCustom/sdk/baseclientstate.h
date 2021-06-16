#ifndef BASECLIENTSTATE_H
#define BASECLIENTSTATE_H
#ifdef _WIN32
#pragma once
#endif







class CBaseClientState
{
public:
	char _pad0[0x20C];
	int m_nDeltaTick;
	bool m_bPaused;
	char _pad1[0x3];
	int m_nViewEntity;
	char _pad2[0x8];
	char m_szLevelName[260];
	char m_szLevelNameShort[40];
};

class CClockDriftMgr
{
public:
	float m_ClockOffsets[16];   
	uint32_t m_iCurClockOffset; 
	uint32_t m_nServerTick;     
	uint32_t m_nClientTick;     
};  

class CEventInfo
{
public:
	enum
	{
		EVENT_INDEX_BITS = 8,
		EVENT_DATA_LEN_BITS = 11,
		MAX_EVENT_DATA = 192,             
	};

	inline CEventInfo()
	{
		classID = 0;
		fire_delay = 0.0f;
		flags = 0;
		pSendTable = NULL;
		pClientClass = NULL;
		m_Packed = 0;
	}

	short classID;
	short pad;
	float fire_delay;
	const void* pSendTable;
	const ClientClass* pClientClass;
	int m_Packed;
	int		flags;
	int filter[8];
	CEventInfo* next;
};

class CClientState
{
public:
	void ForceFullUpdate()
	{
		m_nDeltaTick = -1;
	}

	char pad_0000[156];
	INetChannel* m_NetChannel;
	int m_nChallengeNr;
	char pad_00A4[100];
	int m_nSignonState;
	int signon_pads[2];
	float m_flNextCmdTime;
	int m_nServerCount;
	int m_nCurrentSequence;
	int musor_pads[2];
	CClockDriftMgr m_ClockDriftMgr;
	int m_nDeltaTick;
	bool m_bPaused;
	char paused_align[3];
	int m_nViewEntity;
	int m_nPlayerSlot;
	int bruh;
	char m_szLevelName[260];
	char m_szLevelNameShort[80];
	char m_szGroupName[80];
	char pad_032[92];
	int m_nMaxClients;
	char pad_0314[18828];
	float m_nLastServerTickTime;
	bool m_bInSimulation;
	char pad_4C9D[3];
	int m_nOldTickCount;
	float m_flTickReminder;
	float m_flFrametime;
	int m_nLastOutgoingCommand;
	int m_nChokedCommands;
	int m_nLastCommandAck;
	int m_nPacketEndTickUpdate;
	int m_nCommandAck;
	int m_nSoundSequence;
	char pad_4CCD[76];
	QAngle viewangles;
	int pads[54];
	CEventInfo* m_pEvents;
};



#endif  
#pragma once
