#ifndef GAME_EVENT_LISTENER_H
#define GAME_EVENT_LISTENER_H
#ifdef _WIN32
#pragma once
#endif

#include "igameevents.h"
extern IGameEventManager2* gameeventmanager;

class CGameEventListener : public IGameEventListener2
{
public:
	CGameEventListener() : m_bRegisteredForEvents(false)
	{
	}

	~CGameEventListener()
	{
		StopListeningForAllEvents();
	}

	void ListenForGameEvent(const char* name)
	{
		m_bRegisteredForEvents = true;

#ifdef CLIENT_DLL
		bool bServerSide = false;
#else
		bool bServerSide = true;
#endif
		if (gameeventmanager)
			gameeventmanager->AddListener(this, name, bServerSide);
	}

	void ListenForAllGameEvents()
	{

#ifdef CLIENT_DLL
		bool bServerSide = false;
#else
		bool bServerSide = true;
#endif

		gameeventmanager->AddListenerGlobal(this, bServerSide);
	}

	void StopListeningForAllEvents()
	{
		if (m_bRegisteredForEvents)
		{
			if (gameeventmanager)
				gameeventmanager->RemoveListener(this);
			m_bRegisteredForEvents = false;
		}
	}

	virtual void FireGameEvent(IGameEvent* event) = 0;
	int m_nDebugID;
	virtual int GetEventDebugID(void);

private:

	bool m_bRegisteredForEvents;
};

#endif