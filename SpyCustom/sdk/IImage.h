#ifndef IIMAGE_H
#define IIMAGE_H

#ifdef _WIN32
#pragma once
#endif

#include "VGUI.h"

class Color;

namespace vgui
{

	typedef unsigned long HTexture;

	enum iimage_rotation_t
	{
		ROTATED_UNROTATED = 0,
		ROTATED_CLOCKWISE_90,
		ROTATED_ANTICLOCKWISE_90,
		ROTATED_FLIPPED,
	};

	class IImage
	{
	public:
		virtual void Paint() = 0;

		virtual void SetPos(int x, int y) = 0;

		virtual void GetContentSize(int& wide, int& tall) = 0;

		virtual void GetSize(int& wide, int& tall) = 0;

		virtual void SetSize(int wide, int tall) = 0;

		virtual void SetColor(Color col) = 0;

		virtual ~IImage() {}

		virtual bool Evict() = 0;

		virtual int GetNumFrames() = 0;
		virtual void SetFrame(int nFrame) = 0;
		virtual HTexture GetID() = 0;

		virtual void SetRotation(int iRotation) = 0;
	};

}   


#endif  