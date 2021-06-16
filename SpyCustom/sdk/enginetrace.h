#ifndef ENGINETRACE_H
#define ENGINETRACE_H

#ifdef _WIN32
#pragma once
#endif


#include "IEngineTrace.h"

extern IEngineTrace* g_pEngineTraceServer;
extern IEngineTrace* g_pEngineTraceClient;


void EngineTraceRenderRayCasts();


#endif  