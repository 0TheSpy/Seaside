#ifndef GROUNDLINK_H
#define GROUNDLINK_H
#ifdef _WIN32
#pragma once
#endif

struct groundlink_t
{
	EHANDLE					entity;
	groundlink_t* nextLink;
	groundlink_t* prevLink;
};

#endif  