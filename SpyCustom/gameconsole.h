#ifndef GAMECONSOLE_H
#define GAMECONSOLE_H
#ifdef _WIN32
#pragma once
#endif

#include "IGameConsole.h"

class CGameConsoleDialog;

class CGameConsole : public IGameConsole
{
public:
	CGameConsole();
	~CGameConsole();

	void Initialize();

	virtual void Activate();
	virtual void Hide();
	virtual void Clear();

	void HideImmediately(void);

	virtual bool IsConsoleVisible();

	void ActivateDelayed(float time);

	void SetParent(int parent);

	void Shutdown(void);

	static void OnCmdCondump();
private:

	bool m_bInitialized;
	CGameConsoleDialog* m_pConsole;
};

extern CGameConsole& GameConsole();

#endif  