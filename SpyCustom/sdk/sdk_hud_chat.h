#ifndef SDK_HUD_CHAT_H
#define SDK_HUD_CHAT_H
#ifdef _WIN32
#pragma once
#endif

#include "hud_basechat.h"

#include "GetVFunc.hpp"

class CHudChatLine : public CBaseHudChatLine
{
	DECLARE_CLASS_SIMPLE(CHudChatLine, CBaseHudChatLine);

public:
	CHudChatLine(vgui::Panel* parent, const char* panelName) : CBaseHudChatLine(parent, panelName) {}

	virtual void	ApplySchemeSettings(vgui::IScheme* pScheme);

	void			PerformFadeout(void);

	void			MsgFunc_SayText(bf_read& msg);

private:
	CHudChatLine(const CHudChatLine&);     
};

class CHudChatInputLine : public CBaseHudChatInputLine
{
	DECLARE_CLASS_SIMPLE(CHudChatInputLine, CBaseHudChatInputLine);

public:
	CHudChatInputLine(CBaseHudChat* parent, char const* panelName) : CBaseHudChatInputLine(parent, panelName) {}

	virtual void	ApplySchemeSettings(vgui::IScheme* pScheme);
};

class CHudChat : public CBaseHudChat
{
	DECLARE_CLASS_SIMPLE(CHudChat, CBaseHudChat);

public:
	CHudChat(const char* pElementName);

	virtual void	CreateChatInputLine(void);
	virtual void	CreateChatLines(void);

	virtual void	Init(void);
	virtual void	Reset(void);
	virtual void	ApplySchemeSettings(vgui::IScheme* pScheme);

	void			MsgFunc_SayText(bf_read& msg);
	void			MsgFunc_TextMsg(bf_read& msg);

	enum ChatFilters
	{
		CHAT_FILTER_NONE = 0,
		CHAT_FILTER_JOINLEAVE = 0x000001,
		CHAT_FILTER_NAMECHANGE = 0x000002,
		CHAT_FILTER_PUBLICCHAT = 0x000004,
		CHAT_FILTER_SERVERMSG = 0x000008,
		CHAT_FILTER_TEAMCHANGE = 0x000010,
		CHAT_FILTER_ACHIEVEMENT = 0x000020,
	};

	void ChatPrintf_v(int iPlayerIndex, int iFilter, const char* fmt, ...)
	{
		char msg[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(msg, 1024, fmt, args); 
		getvfunc<void(__cdecl*)(void*, int, int, const char*, ...)>(this, 27)(this, iPlayerIndex, iFilter, fmt);
		va_end(args);
	} 
	

	int				GetChatInputOffset(void);
};

#endif	