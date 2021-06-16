#ifndef TOUCHLINK_H
#define TOUCHLINK_H
#ifdef _WIN32
#pragma once
#endif


enum touchlink_flags_t
{
	FTOUCHLINK_START_TOUCH = 0x00000001,
};

struct touchlink_t
{
#if defined( CLIENT_DLL )
	C_BaseEntity* entityTouched;
#else
	EHANDLE				entityTouched;
#endif
	int					touchStamp;
	touchlink_t* nextLink;
	touchlink_t* prevLink;
	int					flags;
};

#define TOUCHSTAMP_EVENT_DRIVEN		-1


#endif  