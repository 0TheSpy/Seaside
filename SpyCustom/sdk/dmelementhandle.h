#ifndef DMELEMENTHANDLE_H
#define DMELEMENTHANDLE_H

#ifdef _WIN32
#pragma once
#endif


#define PERFORM_HANDLE_TYPECHECKING 0
#if PERFORM_HANDLE_TYPECHECKING

struct DmElementHandle_t
{
	DmElementHandle_t() : handle(0xffffffff) {}
	explicit DmElementHandle_t(int h) : handle(h) {}
	inline bool operator==(const DmElementHandle_t& h) const { return handle == h.handle; }
	inline bool operator!=(const DmElementHandle_t& h) const { return handle != h.handle; }
	inline bool operator<(const DmElementHandle_t& h) const { return handle < h.handle; }
	int handle;
};
const DmElementHandle_t DMELEMENT_HANDLE_INVALID;

#else  

enum DmElementHandle_t
{
	DMELEMENT_HANDLE_INVALID = 0xffffffff
};

#endif  



#endif  