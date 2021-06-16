#ifndef IVDEBUGOVERLAY_H
#define IVDEBUGOVERLAY_H

#ifdef _WIN32
#pragma once
#endif

class Vector;

#define VDEBUG_OVERLAY_INTERFACE_VERSION	"VDebugOverlay003"

#define NDEBUG_PERSIST_TILL_NEXT_SERVER (0.0f)

class OverlayText_t;

abstract_class IVDebugOverlay
{
public:
    virtual void            __unkn() = 0;
    virtual void            AddEntityTextOverlay(int ent_index, int line_offset, float duration, int r, int g, int b, int a, const char* format, ...) = 0;
    virtual void            AddBoxOverlay(const Vector& origin, const Vector& mins, const Vector& max, Vector const& orientation, int r, int g, int b, int a, float duration) = 0;
    virtual void            AddSphereOverlay(const Vector& vOrigin, float flRadius, int nTheta, int nPhi, int r, int g, int b, int a, float flDuration) = 0;
    virtual void            AddTriangleOverlay(const Vector& p1, const Vector& p2, const Vector& p3, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
    virtual void            AddLineOverlay(const Vector& origin, const Vector& dest, int r, int g, int b, bool noDepthTest, float duration) = 0;
    virtual void            AddTextOverlay(const Vector& origin, float duration, const char* format, ...) = 0;
    virtual void            AddTextOverlay(const Vector& origin, int line_offset, float duration, const char* format, ...) = 0;
    virtual void            AddScreenTextOverlay(float flXPos, float flYPos, float flDuration, int r, int g, int b, int a, const char* text) = 0;
    virtual void            AddSweptBoxOverlay(const Vector& start, const Vector& end, const Vector& mins, const Vector& max, const QAngle& angles, int r, int g, int b, int a, float flDuration) = 0;
    virtual void            AddGridOverlay(const Vector& origin) = 0;
    virtual void            AddCoordFrameOverlay(const matrix3x4_t& frame, float flScale, int vColorTable[3][3] = NULL) = 0;
    virtual int             ScreenPosition(const Vector& point, Vector& screen) = 0;
    virtual int             ScreenPosition(float flXPos, float flYPos, Vector& screen) = 0;
    virtual OverlayText_t* GetFirst(void) = 0;
    virtual OverlayText_t* GetNext(OverlayText_t* current) = 0;
    virtual void            ClearDeadOverlays(void) = 0;
    virtual void            ClearAllOverlays() = 0;
    virtual void            AddTextOverlayRGB(const Vector& origin, int line_offset, float duration, float r, float g, float b, float alpha, const char* format, ...) = 0;
    virtual void            AddTextOverlayRGB(const Vector& origin, int line_offset, float duration, int r, int g, int b, int a, const char* format, ...) = 0;
    virtual void            AddLineOverlayAlpha(const Vector& origin, const Vector& dest, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
    virtual void            AddBoxOverlay2(const Vector& origin, const Vector& mins, const Vector& max, QAngle const& orientation, const Color& faceColor, const Color& edgeColor, float duration) = 0;
    virtual void            PurgeTextOverlays() = 0;
    virtual void            DrawPill(const Vector& mins, const Vector& max, float& diameter, int r, int g, int b, int a, float duration) = 0;
};
#endif  