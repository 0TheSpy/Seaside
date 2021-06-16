#ifndef CLIENT_RENDER_HANDLE_H
#define CLIENT_RENDER_HANDLE_H
#ifdef _WIN32
#pragma once
#endif


class IClientRenderable;


typedef unsigned short ClientRenderHandle_t;

enum
{
	INVALID_CLIENT_RENDER_HANDLE = (ClientRenderHandle_t)0xffff,
};


#endif  