#ifndef IMAGE_H
#define IMAGE_H

#ifdef _WIN32
#pragma once
#endif

#include "VGUI.h"
#include "Color.h"
#include "IImage.h"

namespace vgui
{

	class Panel;

	class Image : public IImage
	{
	public:
		Image();
		virtual ~Image();

		virtual void SetPos(int x, int y);
		virtual void GetPos(int& x, int& y);
		virtual void GetSize(int& wide, int& tall);
		virtual void GetContentSize(int& wide, int& tall);
		virtual void SetColor(Color color);
		virtual void SetBkColor(Color color) { DrawSetColor(color); }
		virtual Color GetColor();
		virtual bool Evict();
		virtual int GetNumFrames();
		virtual void SetFrame(int nFrame);
		virtual HTexture GetID();
		virtual void SetRotation(int iRotation) { return; };

	protected:
		virtual void SetSize(int wide, int tall);
		virtual void DrawSetColor(Color color);
		virtual void DrawSetColor(int r, int g, int b, int a);
		virtual void DrawFilledRect(int x0, int y0, int x1, int y1);
		virtual void DrawOutlinedRect(int x0, int y0, int x1, int y1);
		virtual void DrawLine(int x0, int y0, int x1, int y1);
		virtual void DrawPolyLine(int* px, int* py, int numPoints);
		virtual void DrawSetTextFont(HFont font);
		virtual void DrawSetTextColor(Color color);
		virtual void DrawSetTextColor(int r, int g, int b, int a);
		virtual void DrawSetTextPos(int x, int y);
		virtual void DrawPrintText(const wchar_t* str, int strlen);
		virtual void DrawPrintText(int x, int y, const wchar_t* str, int strlen);
		virtual void DrawPrintChar(wchar_t ch);
		virtual void DrawPrintChar(int x, int y, wchar_t ch);
		virtual void DrawSetTexture(int id);
		virtual void DrawTexturedRect(int x0, int y0, int x1, int y1);
		virtual void Paint() = 0;

	private:
		int _pos[2];
		int _size[2];
		Color _color;
	};

}   

#endif  