#pragma once
#ifndef IMGUI_DISABLE

#ifndef IMGUI_VERSION
#error Must include imgui.h before imgui_internal.h
#endif

#include <stdio.h>        
#include <stdlib.h>           
#include <math.h>               
#include <limits.h>       

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4251)                     
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#if __has_warning("-Wunknown-warning-option")
#pragma clang diagnostic ignored "-Wunknown-warning-option"              
#endif
#pragma clang diagnostic ignored "-Wunknown-pragmas"                     
#pragma clang diagnostic ignored "-Wunused-function"                  
#pragma clang diagnostic ignored "-Wmissing-prototypes"               
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"            
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"                      
#pragma GCC diagnostic ignored "-Wclass-memaccess"                          
#endif

#ifdef IMGUI_DISABLE_FORMAT_STRING_FUNCTIONS               
#error Use IMGUI_DISABLE_DEFAULT_FORMAT_FUNCTIONS
#endif
#ifdef IMGUI_DISABLE_MATH_FUNCTIONS                        
#error Use IMGUI_DISABLE_DEFAULT_MATH_FUNCTIONS
#endif



#ifndef IMGUI_ENABLE_FREETYPE
#define IMGUI_ENABLE_STB_TRUETYPE
#endif

struct ImBitVector;                     
struct ImRect;                           
struct ImDrawDataBuilder;                 
struct ImDrawListSharedData;              
struct ImGuiColorMod;                           
struct ImGuiContext;                    
struct ImGuiContextHook;                 
struct ImGuiDataTypeInfo;                  
struct ImGuiGroupData;                   
struct ImGuiInputTextState;                  
struct ImGuiLastItemDataBackup;           
struct ImGuiMenuColumns;                    
struct ImGuiNavMoveResult;                   
struct ImGuiMetricsConfig;                
struct ImGuiNextWindowData;             
struct ImGuiNextItemData;               
struct ImGuiOldColumnData;                    
struct ImGuiOldColumns;                       
struct ImGuiPopupData;                   
struct ImGuiSettingsHandler;                 
struct ImGuiStackSizes;                   
struct ImGuiStyleMod;                           
struct ImGuiTabBar;                      
struct ImGuiTabItem;                         
struct ImGuiTable;                      
struct ImGuiTableColumn;                   
struct ImGuiTableSettings;                
struct ImGuiTableColumnsSettings;         
struct ImGuiWindow;                     
struct ImGuiWindowTempData;                             
struct ImGuiWindowSettings;                              

typedef int ImGuiLayoutType;                            
typedef int ImGuiItemFlags;                             
typedef int ImGuiItemStatusFlags;                 
typedef int ImGuiOldColumnFlags;                   
typedef int ImGuiNavHighlightFlags;             
typedef int ImGuiNavDirSourceFlags;             
typedef int ImGuiNavMoveFlags;                        
typedef int ImGuiNextItemDataFlags;              
typedef int ImGuiNextWindowDataFlags;          
typedef int ImGuiSeparatorFlags;                   
typedef int ImGuiTextFlags;                             
typedef int ImGuiTooltipFlags;                       

typedef void (*ImGuiErrorLogCallback)(void* user_data, const char* fmt, ...);

#ifndef GImGui
extern IMGUI_API ImGuiContext* GImGui;      
#endif

namespace ImStb
{

#undef STB_TEXTEDIT_STRING
#undef STB_TEXTEDIT_CHARTYPE
#define STB_TEXTEDIT_STRING             ImGuiInputTextState
#define STB_TEXTEDIT_CHARTYPE           ImWchar
#define STB_TEXTEDIT_GETWIDTH_NEWLINE   (-1.0f)
#define STB_TEXTEDIT_UNDOSTATECOUNT     99
#define STB_TEXTEDIT_UNDOCHARCOUNT      999
#include "imstb_textedit.h"

}   

#ifndef IMGUI_DEBUG_LOG
#define IMGUI_DEBUG_LOG(_FMT,...)       printf("[%05d] " _FMT, GImGui->FrameCount, __VA_ARGS__)
#endif

#define IMGUI_DEBUG_LOG_POPUP(...)      ((void)0)         
#define IMGUI_DEBUG_LOG_NAV(...)        ((void)0)         

#if (__cplusplus >= 201100) || (defined(_MSVC_LANG) && _MSVC_LANG >= 201100)
#define IM_STATIC_ASSERT(_COND)         static_assert(_COND, "")
#else
#define IM_STATIC_ASSERT(_COND)         typedef char static_assertion_##__line__[(_COND)?1:-1]
#endif

#ifdef IMGUI_DEBUG_PARANOID
#define IM_ASSERT_PARANOID(_EXPR)       IM_ASSERT(_EXPR)
#else
#define IM_ASSERT_PARANOID(_EXPR)
#endif

#ifndef IM_ASSERT_USER_ERROR
#define IM_ASSERT_USER_ERROR(_EXP,_MSG) IM_ASSERT((_EXP) && _MSG)      
#endif

#define IM_PI                           3.14159265358979323846f
#ifdef _WIN32
#define IM_NEWLINE                      "\r\n"                    
#else
#define IM_NEWLINE                      "\n"
#endif
#define IM_TABSIZE                      (4)
#define IM_F32_TO_INT8_UNBOUND(_VAL)    ((int)((_VAL) * 255.0f + ((_VAL)>=0 ? 0.5f : -0.5f)))       
#define IM_F32_TO_INT8_SAT(_VAL)        ((int)(ImSaturate(_VAL) * 255.0f + 0.5f))                   
#define IM_FLOOR(_VAL)                  ((float)(int)(_VAL))                                            
#define IM_ROUND(_VAL)                  ((float)(int)((_VAL) + 0.5f))                           

#ifdef _MSC_VER
#define IMGUI_CDECL __cdecl
#else
#define IMGUI_CDECL
#endif

#ifndef IM_DEBUG_BREAK
#if defined(__clang__)
#define IM_DEBUG_BREAK()    __builtin_debugtrap()
#elif defined (_MSC_VER)
#define IM_DEBUG_BREAK()    __debugbreak()
#else
#define IM_DEBUG_BREAK()    IM_ASSERT(0)                    
#endif
#endif   

IMGUI_API ImGuiID       ImHashData(const void* data, size_t data_size, ImU32 seed = 0);
IMGUI_API ImGuiID       ImHashStr(const char* data, size_t data_size = 0, ImU32 seed = 0);
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
static inline ImGuiID   ImHash(const void* data, int size, ImU32 seed = 0) { return size ? ImHashData(data, (size_t)size, seed) : ImHashStr((const char*)data, 0, seed); }      
#endif

#define ImQsort         qsort

IMGUI_API ImU32         ImAlphaBlendColors(ImU32 col_a, ImU32 col_b);

static inline bool      ImIsPowerOfTwo(int v)           { return v != 0 && (v & (v - 1)) == 0; }
static inline bool      ImIsPowerOfTwo(ImU64 v)         { return v != 0 && (v & (v - 1)) == 0; }
static inline int       ImUpperPowerOfTwo(int v)        { v--; v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16; v++; return v; }

IMGUI_API int           ImStricmp(const char* str1, const char* str2);
IMGUI_API int           ImStrnicmp(const char* str1, const char* str2, size_t count);
IMGUI_API void          ImStrncpy(char* dst, const char* src, size_t count);
IMGUI_API char*         ImStrdup(const char* str);
IMGUI_API char*         ImStrdupcpy(char* dst, size_t* p_dst_size, const char* str);
IMGUI_API const char*   ImStrchrRange(const char* str_begin, const char* str_end, char c);
IMGUI_API int           ImStrlenW(const ImWchar* str);
IMGUI_API const char*   ImStreolRange(const char* str, const char* str_end);                  
IMGUI_API const ImWchar*ImStrbolW(const ImWchar* buf_mid_line, const ImWchar* buf_begin);     
IMGUI_API const char*   ImStristr(const char* haystack, const char* haystack_end, const char* needle, const char* needle_end);
IMGUI_API void          ImStrTrimBlanks(char* str);
IMGUI_API const char*   ImStrSkipBlank(const char* str);
IMGUI_API int           ImFormatString(char* buf, size_t buf_size, const char* fmt, ...) IM_FMTARGS(3);
IMGUI_API int           ImFormatStringV(char* buf, size_t buf_size, const char* fmt, va_list args) IM_FMTLIST(3);
IMGUI_API const char*   ImParseFormatFindStart(const char* format);
IMGUI_API const char*   ImParseFormatFindEnd(const char* format);
IMGUI_API const char*   ImParseFormatTrimDecorations(const char* format, char* buf, size_t buf_size);
IMGUI_API int           ImParseFormatPrecision(const char* format, int default_value);
static inline bool      ImCharIsBlankA(char c)          { return c == ' ' || c == '\t'; }
static inline bool      ImCharIsBlankW(unsigned int c)  { return c == ' ' || c == '\t' || c == 0x3000; }

IMGUI_API int           ImTextStrToUtf8(char* buf, int buf_size, const ImWchar* in_text, const ImWchar* in_text_end);           
IMGUI_API int           ImTextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end);                  
IMGUI_API int           ImTextStrFromUtf8(ImWchar* buf, int buf_size, const char* in_text, const char* in_text_end, const char** in_remaining = NULL);        
IMGUI_API int           ImTextCountCharsFromUtf8(const char* in_text, const char* in_text_end);                                    
IMGUI_API int           ImTextCountUtf8BytesFromChar(const char* in_text, const char* in_text_end);                                  
IMGUI_API int           ImTextCountUtf8BytesFromStr(const ImWchar* in_text, const ImWchar* in_text_end);                            

#ifdef IMGUI_DEFINE_MATH_OPERATORS
static inline ImVec2 operator*(const ImVec2& lhs, const float rhs)              { return ImVec2(lhs.x * rhs, lhs.y * rhs); }
static inline ImVec2 operator/(const ImVec2& lhs, const float rhs)              { return ImVec2(lhs.x / rhs, lhs.y / rhs); }
static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)            { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs)            { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
static inline ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs)            { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
static inline ImVec2 operator/(const ImVec2& lhs, const ImVec2& rhs)            { return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y); }
static inline ImVec2& operator*=(ImVec2& lhs, const float rhs)                  { lhs.x *= rhs; lhs.y *= rhs; return lhs; }
static inline ImVec2& operator/=(ImVec2& lhs, const float rhs)                  { lhs.x /= rhs; lhs.y /= rhs; return lhs; }
static inline ImVec2& operator+=(ImVec2& lhs, const ImVec2& rhs)                { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
static inline ImVec2& operator-=(ImVec2& lhs, const ImVec2& rhs)                { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }
static inline ImVec2& operator*=(ImVec2& lhs, const ImVec2& rhs)                { lhs.x *= rhs.x; lhs.y *= rhs.y; return lhs; }
static inline ImVec2& operator/=(ImVec2& lhs, const ImVec2& rhs)                { lhs.x /= rhs.x; lhs.y /= rhs.y; return lhs; }
static inline ImVec4 operator+(const ImVec4& lhs, const ImVec4& rhs)            { return ImVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }
static inline ImVec4 operator-(const ImVec4& lhs, const ImVec4& rhs)            { return ImVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
static inline ImVec4 operator*(const ImVec4& lhs, const ImVec4& rhs)            { return ImVec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); }
#endif

#ifdef IMGUI_DISABLE_FILE_FUNCTIONS
#define IMGUI_DISABLE_DEFAULT_FILE_FUNCTIONS
typedef void* ImFileHandle;
static inline ImFileHandle  ImFileOpen(const char*, const char*)                    { return NULL; }
static inline bool          ImFileClose(ImFileHandle)                               { return false; }
static inline ImU64         ImFileGetSize(ImFileHandle)                             { return (ImU64)-1; }
static inline ImU64         ImFileRead(void*, ImU64, ImU64, ImFileHandle)           { return 0; }
static inline ImU64         ImFileWrite(const void*, ImU64, ImU64, ImFileHandle)    { return 0; }
#endif
#ifndef IMGUI_DISABLE_DEFAULT_FILE_FUNCTIONS
typedef FILE* ImFileHandle;
IMGUI_API ImFileHandle      ImFileOpen(const char* filename, const char* mode);
IMGUI_API bool              ImFileClose(ImFileHandle file);
IMGUI_API ImU64             ImFileGetSize(ImFileHandle file);
IMGUI_API ImU64             ImFileRead(void* data, ImU64 size, ImU64 count, ImFileHandle file);
IMGUI_API ImU64             ImFileWrite(const void* data, ImU64 size, ImU64 count, ImFileHandle file);
#else
#define IMGUI_DISABLE_TTY_FUNCTIONS             
#endif
IMGUI_API void*             ImFileLoadToMemory(const char* filename, const char* mode, size_t* out_file_size = NULL, int padding_bytes = 0);

#ifndef IMGUI_DISABLE_DEFAULT_MATH_FUNCTIONS
#define ImFabs(X)           fabsf(X)
#define ImSqrt(X)           sqrtf(X)
#define ImFmod(X, Y)        fmodf((X), (Y))
#define ImCos(X)            cosf(X)
#define ImSin(X)            sinf(X)
#define ImAcos(X)           acosf(X)
#define ImAtan2(Y, X)       atan2f((Y), (X))
#define ImAtof(STR)         atof(STR)
#define ImFloorStd(X)       floorf(X)                                   
#define ImCeil(X)           ceilf(X)
static inline float  ImPow(float x, float y)    { return powf(x, y); }                    
static inline double ImPow(double x, double y)  { return pow(x, y); }
static inline float  ImLog(float x)             { return logf(x); }                       
static inline double ImLog(double x)            { return log(x); }
static inline float  ImAbs(float x)             { return fabsf(x); }
static inline double ImAbs(double x)            { return fabs(x); }
static inline float  ImSign(float x)            { return (x < 0.0f) ? -1.0f : ((x > 0.0f) ? 1.0f : 0.0f); }              
static inline double ImSign(double x)           { return (x < 0.0) ? -1.0 : ((x > 0.0) ? 1.0 : 0.0); }
#endif
template<typename T> static inline T ImMin(T lhs, T rhs)                        { return lhs < rhs ? lhs : rhs; }
template<typename T> static inline T ImMax(T lhs, T rhs)                        { return lhs >= rhs ? lhs : rhs; }
template<typename T> static inline T ImClamp(T v, T mn, T mx)                   { return (v < mn) ? mn : (v > mx) ? mx : v; }
template<typename T> static inline T ImLerp(T a, T b, float t)                  { return (T)(a + (b - a) * t); }
template<typename T> static inline void ImSwap(T& a, T& b)                      { T tmp = a; a = b; b = tmp; }
template<typename T> static inline T ImAddClampOverflow(T a, T b, T mn, T mx)   { if (b < 0 && (a < mn - b)) return mn; if (b > 0 && (a > mx - b)) return mx; return a + b; }
template<typename T> static inline T ImSubClampOverflow(T a, T b, T mn, T mx)   { if (b > 0 && (a < mn + b)) return mn; if (b < 0 && (a > mx + b)) return mx; return a - b; }
static inline ImVec2 ImMin(const ImVec2& lhs, const ImVec2& rhs)                { return ImVec2(lhs.x < rhs.x ? lhs.x : rhs.x, lhs.y < rhs.y ? lhs.y : rhs.y); }
static inline ImVec2 ImMax(const ImVec2& lhs, const ImVec2& rhs)                { return ImVec2(lhs.x >= rhs.x ? lhs.x : rhs.x, lhs.y >= rhs.y ? lhs.y : rhs.y); }
static inline ImVec2 ImClamp(const ImVec2& v, const ImVec2& mn, ImVec2 mx)      { return ImVec2((v.x < mn.x) ? mn.x : (v.x > mx.x) ? mx.x : v.x, (v.y < mn.y) ? mn.y : (v.y > mx.y) ? mx.y : v.y); }
static inline ImVec2 ImLerp(const ImVec2& a, const ImVec2& b, float t)          { return ImVec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t); }
static inline ImVec2 ImLerp(const ImVec2& a, const ImVec2& b, const ImVec2& t)  { return ImVec2(a.x + (b.x - a.x) * t.x, a.y + (b.y - a.y) * t.y); }
static inline ImVec4 ImLerp(const ImVec4& a, const ImVec4& b, float t)          { return ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t); }
static inline float  ImSaturate(float f)                                        { return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f; }
static inline float  ImLengthSqr(const ImVec2& lhs)                             { return (lhs.x * lhs.x) + (lhs.y * lhs.y); }
static inline float  ImLengthSqr(const ImVec4& lhs)                             { return (lhs.x * lhs.x) + (lhs.y * lhs.y) + (lhs.z * lhs.z) + (lhs.w * lhs.w); }
static inline float  ImInvLength(const ImVec2& lhs, float fail_value)           { float d = (lhs.x * lhs.x) + (lhs.y * lhs.y); if (d > 0.0f) return 1.0f / ImSqrt(d); return fail_value; }
static inline float  ImFloor(float f)                                           { return (float)(int)(f); }
static inline ImVec2 ImFloor(const ImVec2& v)                                   { return ImVec2((float)(int)(v.x), (float)(int)(v.y)); }
static inline int    ImModPositive(int a, int b)                                { return (a + b) % b; }
static inline float  ImDot(const ImVec2& a, const ImVec2& b)                    { return a.x * b.x + a.y * b.y; }
static inline ImVec2 ImRotate(const ImVec2& v, float cos_a, float sin_a)        { return ImVec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a); }
static inline float  ImLinearSweep(float current, float target, float speed)    { if (current < target) return ImMin(current + speed, target); if (current > target) return ImMax(current - speed, target); return current; }
static inline ImVec2 ImMul(const ImVec2& lhs, const ImVec2& rhs)                { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }

IMGUI_API ImVec2     ImBezierCubicCalc(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, float t);
IMGUI_API ImVec2     ImBezierCubicClosestPoint(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& p, int num_segments);              
IMGUI_API ImVec2     ImBezierCubicClosestPointCasteljau(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& p, float tess_tol);         
IMGUI_API ImVec2     ImBezierQuadraticCalc(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, float t);
IMGUI_API ImVec2     ImLineClosestPoint(const ImVec2& a, const ImVec2& b, const ImVec2& p);
IMGUI_API bool       ImTriangleContainsPoint(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p);
IMGUI_API ImVec2     ImTriangleClosestPoint(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p);
IMGUI_API void       ImTriangleBarycentricCoords(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p, float& out_u, float& out_v, float& out_w);
inline float         ImTriangleArea(const ImVec2& a, const ImVec2& b, const ImVec2& c) { return ImFabs((a.x * (b.y - c.y)) + (b.x * (c.y - a.y)) + (c.x * (a.y - b.y))) * 0.5f; }
IMGUI_API ImGuiDir   ImGetDirQuadrantFromDelta(float dx, float dy);

struct ImVec1
{
    float   x;
    ImVec1()         { x = 0.0f; }
    ImVec1(float _x) { x = _x; }
};

struct ImVec2ih
{
    short   x, y;
    ImVec2ih()                           { x = y = 0; }
    ImVec2ih(short _x, short _y)         { x = _x; y = _y; }
    explicit ImVec2ih(const ImVec2& rhs) { x = (short)rhs.x; y = (short)rhs.y; }
};

struct IMGUI_API ImRect
{
    ImVec2      Min;     
    ImVec2      Max;     

    ImRect()                                        : Min(0.0f, 0.0f), Max(0.0f, 0.0f)  {}
    ImRect(const ImVec2& min, const ImVec2& max)    : Min(min), Max(max)                {}
    ImRect(const ImVec4& v)                         : Min(v.x, v.y), Max(v.z, v.w)      {}
    ImRect(float x1, float y1, float x2, float y2)  : Min(x1, y1), Max(x2, y2)          {}

    ImVec2      GetCenter() const                   { return ImVec2((Min.x + Max.x) * 0.5f, (Min.y + Max.y) * 0.5f); }
    ImVec2      GetSize() const                     { return ImVec2(Max.x - Min.x, Max.y - Min.y); }
    float       GetWidth() const                    { return Max.x - Min.x; }
    float       GetHeight() const                   { return Max.y - Min.y; }
    ImVec2      GetTL() const                       { return Min; }                    
    ImVec2      GetTR() const                       { return ImVec2(Max.x, Min.y); }   
    ImVec2      GetBL() const                       { return ImVec2(Min.x, Max.y); }   
    ImVec2      GetBR() const                       { return Max; }                    
    bool        Contains(const ImVec2& p) const     { return p.x     >= Min.x && p.y     >= Min.y && p.x     <  Max.x && p.y     <  Max.y; }
    bool        Contains(const ImRect& r) const     { return r.Min.x >= Min.x && r.Min.y >= Min.y && r.Max.x <= Max.x && r.Max.y <= Max.y; }
    bool        Overlaps(const ImRect& r) const     { return r.Min.y <  Max.y && r.Max.y >  Min.y && r.Min.x <  Max.x && r.Max.x >  Min.x; }
    void        Add(const ImVec2& p)                { if (Min.x > p.x)     Min.x = p.x;     if (Min.y > p.y)     Min.y = p.y;     if (Max.x < p.x)     Max.x = p.x;     if (Max.y < p.y)     Max.y = p.y; }
    void        Add(const ImRect& r)                { if (Min.x > r.Min.x) Min.x = r.Min.x; if (Min.y > r.Min.y) Min.y = r.Min.y; if (Max.x < r.Max.x) Max.x = r.Max.x; if (Max.y < r.Max.y) Max.y = r.Max.y; }
    void        Expand(const float amount)          { Min.x -= amount;   Min.y -= amount;   Max.x += amount;   Max.y += amount; }
    void        Expand(const ImVec2& amount)        { Min.x -= amount.x; Min.y -= amount.y; Max.x += amount.x; Max.y += amount.y; }
    void        Translate(const ImVec2& d)          { Min.x += d.x; Min.y += d.y; Max.x += d.x; Max.y += d.y; }
    void        TranslateX(float dx)                { Min.x += dx; Max.x += dx; }
    void        TranslateY(float dy)                { Min.y += dy; Max.y += dy; }
    void        ClipWith(const ImRect& r)           { Min = ImMax(Min, r.Min); Max = ImMin(Max, r.Max); }                                     
    void        ClipWithFull(const ImRect& r)       { Min = ImClamp(Min, r.Min, r.Max); Max = ImClamp(Max, r.Min, r.Max); }         
    void        Floor()                             { Min.x = IM_FLOOR(Min.x); Min.y = IM_FLOOR(Min.y); Max.x = IM_FLOOR(Max.x); Max.y = IM_FLOOR(Max.y); }
    bool        IsInverted() const                  { return Min.x > Max.x || Min.y > Max.y; }
    ImVec4      ToVec4() const                      { return ImVec4(Min.x, Min.y, Max.x, Max.y); }
};

inline bool     ImBitArrayTestBit(const ImU32* arr, int n)      { ImU32 mask = (ImU32)1 << (n & 31); return (arr[n >> 5] & mask) != 0; }
inline void     ImBitArrayClearBit(ImU32* arr, int n)           { ImU32 mask = (ImU32)1 << (n & 31); arr[n >> 5] &= ~mask; }
inline void     ImBitArraySetBit(ImU32* arr, int n)             { ImU32 mask = (ImU32)1 << (n & 31); arr[n >> 5] |= mask; }
inline void     ImBitArraySetBitRange(ImU32* arr, int n, int n2)     
{
    n2--;
    while (n <= n2)
    {
        int a_mod = (n & 31);
        int b_mod = (n2 > (n | 31) ? 31 : (n2 & 31)) + 1;
        ImU32 mask = (ImU32)(((ImU64)1 << b_mod) - 1) & ~(ImU32)(((ImU64)1 << a_mod) - 1);
        arr[n >> 5] |= mask;
        n = (n + 32) & ~31;
    }
}

template<int BITCOUNT>
struct IMGUI_API ImBitArray
{
    ImU32           Storage[(BITCOUNT + 31) >> 5];
    ImBitArray()                                { }
    void            ClearAllBits()              { memset(Storage, 0, sizeof(Storage)); }
    void            SetAllBits()                { memset(Storage, 255, sizeof(Storage)); }
    bool            TestBit(int n) const        { IM_ASSERT(n < BITCOUNT); return ImBitArrayTestBit(Storage, n); }
    void            SetBit(int n)               { IM_ASSERT(n < BITCOUNT); ImBitArraySetBit(Storage, n); }
    void            ClearBit(int n)             { IM_ASSERT(n < BITCOUNT); ImBitArrayClearBit(Storage, n); }
    void            SetBitRange(int n, int n2)  { ImBitArraySetBitRange(Storage, n, n2); }     
};

struct IMGUI_API ImBitVector
{
    ImVector<ImU32> Storage;
    void            Create(int sz)              { Storage.resize((sz + 31) >> 5); memset(Storage.Data, 0, (size_t)Storage.Size * sizeof(Storage.Data[0])); }
    void            Clear()                     { Storage.clear(); }
    bool            TestBit(int n) const        { IM_ASSERT(n < (Storage.Size << 5)); return ImBitArrayTestBit(Storage.Data, n); }
    void            SetBit(int n)               { IM_ASSERT(n < (Storage.Size << 5)); ImBitArraySetBit(Storage.Data, n); }
    void            ClearBit(int n)             { IM_ASSERT(n < (Storage.Size << 5)); ImBitArrayClearBit(Storage.Data, n); }
};

template<typename T>
struct ImSpan
{
    T*                  Data;
    T*                  DataEnd;

    inline ImSpan()                                 { Data = DataEnd = NULL; }
    inline ImSpan(T* data, int size)                { Data = data; DataEnd = data + size; }
    inline ImSpan(T* data, T* data_end)             { Data = data; DataEnd = data_end; }

    inline void         set(T* data, int size)      { Data = data; DataEnd = data + size; }
    inline void         set(T* data, T* data_end)   { Data = data; DataEnd = data_end; }
    inline int          size() const                { return (int)(ptrdiff_t)(DataEnd - Data); }
    inline int          size_in_bytes() const       { return (int)(ptrdiff_t)(DataEnd - Data) * (int)sizeof(T); }
    inline T&           operator[](int i)           { T* p = Data + i; IM_ASSERT(p >= Data && p < DataEnd); return *p; }
    inline const T&     operator[](int i) const     { const T* p = Data + i; IM_ASSERT(p >= Data && p < DataEnd); return *p; }

    inline T*           begin()                     { return Data; }
    inline const T*     begin() const               { return Data; }
    inline T*           end()                       { return DataEnd; }
    inline const T*     end() const                 { return DataEnd; }

    inline int  index_from_ptr(const T* it) const   { IM_ASSERT(it >= Data && it < DataEnd); const ptrdiff_t off = it - Data; return (int)off; }
};

template<int CHUNKS>
struct ImSpanAllocator
{
    char*   BasePtr;
    int     TotalSize;
    int     CurrSpan;
    int     Offsets[CHUNKS];

    ImSpanAllocator()                               { memset(this, 0, sizeof(*this)); }
    inline void  ReserveBytes(int n, size_t sz)     { IM_ASSERT(n == CurrSpan && n < CHUNKS); IM_UNUSED(n); Offsets[CurrSpan++] = TotalSize; TotalSize += (int)sz; }
    inline int   GetArenaSizeInBytes()              { return TotalSize; }
    inline void  SetArenaBasePtr(void* base_ptr)    { BasePtr = (char*)base_ptr; }
    inline void* GetSpanPtrBegin(int n)             { IM_ASSERT(n >= 0 && n < CHUNKS && CurrSpan == CHUNKS); return (void*)(BasePtr + Offsets[n]); }
    inline void* GetSpanPtrEnd(int n)               { IM_ASSERT(n >= 0 && n < CHUNKS && CurrSpan == CHUNKS); return (n + 1 < CHUNKS) ? BasePtr + Offsets[n + 1] : (void*)(BasePtr + TotalSize); }
    template<typename T>
    inline void  GetSpan(int n, ImSpan<T>* span)    { span->set((T*)GetSpanPtrBegin(n), (T*)GetSpanPtrEnd(n)); }
};

typedef int ImPoolIdx;
template<typename T>
struct IMGUI_API ImPool
{
    ImVector<T>     Buf;          
    ImGuiStorage    Map;         
    ImPoolIdx       FreeIdx;         

    ImPool()    { FreeIdx = 0; }
    ~ImPool()   { Clear(); }
    T*          GetByKey(ImGuiID key)               { int idx = Map.GetInt(key, -1); return (idx != -1) ? &Buf[idx] : NULL; }
    T*          GetByIndex(ImPoolIdx n)             { return &Buf[n]; }
    ImPoolIdx   GetIndex(const T* p) const          { IM_ASSERT(p >= Buf.Data && p < Buf.Data + Buf.Size); return (ImPoolIdx)(p - Buf.Data); }
    T*          GetOrAddByKey(ImGuiID key)          { int* p_idx = Map.GetIntRef(key, -1); if (*p_idx != -1) return &Buf[*p_idx]; *p_idx = FreeIdx; return Add(); }
    bool        Contains(const T* p) const          { return (p >= Buf.Data && p < Buf.Data + Buf.Size); }
    void        Clear()                             { for (int n = 0; n < Map.Data.Size; n++) { int idx = Map.Data[n].val_i; if (idx != -1) Buf[idx].~T(); } Map.Clear(); Buf.clear(); FreeIdx = 0; }
    T*          Add()                               { int idx = FreeIdx; if (idx == Buf.Size) { Buf.resize(Buf.Size + 1); FreeIdx++; } else { FreeIdx = *(int*)&Buf[idx]; } IM_PLACEMENT_NEW(&Buf[idx]) T(); return &Buf[idx]; }
    void        Remove(ImGuiID key, const T* p)     { Remove(key, GetIndex(p)); }
    void        Remove(ImGuiID key, ImPoolIdx idx)  { Buf[idx].~T(); *(int*)&Buf[idx] = FreeIdx; FreeIdx = idx; Map.SetInt(key, -1); }
    void        Reserve(int capacity)               { Buf.reserve(capacity); Map.Data.reserve(capacity); }
    int         GetSize() const                     { return Buf.Size; }
};

template<typename T>
struct IMGUI_API ImChunkStream
{
    ImVector<char>  Buf;

    void    clear()                     { Buf.clear(); }
    bool    empty() const               { return Buf.Size == 0; }
    int     size() const                { return Buf.Size; }
    T*      alloc_chunk(size_t sz)      { size_t HDR_SZ = 4; sz = ((HDR_SZ + sz) + 3u) & ~3u; int off = Buf.Size; Buf.resize(off + (int)sz); ((int*)(void*)(Buf.Data + off))[0] = (int)sz; return (T*)(void*)(Buf.Data + off + (int)HDR_SZ); }
    T*      begin()                     { size_t HDR_SZ = 4; if (!Buf.Data) return NULL; return (T*)(void*)(Buf.Data + HDR_SZ); }
    T*      next_chunk(T* p)            { size_t HDR_SZ = 4; IM_ASSERT(p >= begin() && p < end()); p = (T*)(void*)((char*)(void*)p + chunk_size(p)); if (p == (T*)(void*)((char*)end() + HDR_SZ)) return (T*)0; IM_ASSERT(p < end()); return p; }
    int     chunk_size(const T* p)      { return ((const int*)p)[-1]; }
    T*      end()                       { return (T*)(void*)(Buf.Data + Buf.Size); }
    int     offset_from_ptr(const T* p) { IM_ASSERT(p >= begin() && p < end()); const ptrdiff_t off = (const char*)p - Buf.Data; return (int)off; }
    T*      ptr_from_offset(int off)    { IM_ASSERT(off >= 4 && off < Buf.Size); return (T*)(void*)(Buf.Data + off); }
    void    swap(ImChunkStream<T>& rhs) { rhs.Buf.swap(Buf); }

};

#define IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_MIN                     12
#define IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX                     512
#define IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC(_RAD,_MAXERROR)    ImClamp((int)((IM_PI * 2.0f) / ImAcos(((_RAD) - (_MAXERROR)) / (_RAD))), IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_MIN, IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX)

#ifndef IM_DRAWLIST_ARCFAST_TESSELLATION_MULTIPLIER
#define IM_DRAWLIST_ARCFAST_TESSELLATION_MULTIPLIER             1
#endif

struct IMGUI_API ImDrawListSharedData
{
    ImVec2          TexUvWhitePixel;                   
    ImFont*         Font;                              
    float           FontSize;                           
    float           CurveTessellationTol;            
    float           CircleSegmentMaxError;                   
    ImVec4          ClipRectFullscreen;            
    ImDrawListFlags InitialFlags;                                  

    ImVec2          ArcFastVtx[12 * IM_DRAWLIST_ARCFAST_TESSELLATION_MULTIPLIER];         
    ImU8            CircleSegmentCounts[64];                   
    const ImVec4*   TexUvLines;                        

    ImDrawListSharedData();
    void SetCircleSegmentMaxError(float max_error);
};

struct ImDrawDataBuilder
{
    ImVector<ImDrawList*>   Layers[2];                

    void Clear()            { for (int n = 0; n < IM_ARRAYSIZE(Layers); n++) Layers[n].resize(0); }
    void ClearFreeMemory()  { for (int n = 0; n < IM_ARRAYSIZE(Layers); n++) Layers[n].clear(); }
    IMGUI_API void FlattenIntoSingleLayer();
};

enum ImGuiItemFlags_
{
    ImGuiItemFlags_None                     = 0,
    ImGuiItemFlags_NoTabStop                = 1 << 0,   
    ImGuiItemFlags_ButtonRepeat             = 1 << 1,                   
    ImGuiItemFlags_Disabled                 = 1 << 2,                 
    ImGuiItemFlags_NoNav                    = 1 << 3,   
    ImGuiItemFlags_NoNavDefaultFocus        = 1 << 4,   
    ImGuiItemFlags_SelectableDontClosePopup = 1 << 5,             
    ImGuiItemFlags_MixedValue               = 1 << 6,                             
    ImGuiItemFlags_ReadOnly                 = 1 << 7,                 
    ImGuiItemFlags_Default_                 = 0
};

enum ImGuiItemStatusFlags_
{
    ImGuiItemStatusFlags_None               = 0,
    ImGuiItemStatusFlags_HoveredRect        = 1 << 0,
    ImGuiItemStatusFlags_HasDisplayRect     = 1 << 1,
    ImGuiItemStatusFlags_Edited             = 1 << 2,                      
    ImGuiItemStatusFlags_ToggledSelection   = 1 << 3,                           
    ImGuiItemStatusFlags_ToggledOpen        = 1 << 4,           
    ImGuiItemStatusFlags_HasDeactivated     = 1 << 5,                
    ImGuiItemStatusFlags_Deactivated        = 1 << 6          

#ifdef IMGUI_ENABLE_TEST_ENGINE
    ,   
    ImGuiItemStatusFlags_Openable           = 1 << 10,  
    ImGuiItemStatusFlags_Opened             = 1 << 11,  
    ImGuiItemStatusFlags_Checkable          = 1 << 12,  
    ImGuiItemStatusFlags_Checked            = 1 << 13   
#endif
};

enum ImGuiButtonFlagsPrivate_
{
    ImGuiButtonFlags_PressedOnClick         = 1 << 4,          
    ImGuiButtonFlags_PressedOnClickRelease  = 1 << 5,                       
    ImGuiButtonFlags_PressedOnClickReleaseAnywhere = 1 << 6,                   
    ImGuiButtonFlags_PressedOnRelease       = 1 << 7,          
    ImGuiButtonFlags_PressedOnDoubleClick   = 1 << 8,          
    ImGuiButtonFlags_PressedOnDragDropHold  = 1 << 9,                       
    ImGuiButtonFlags_Repeat                 = 1 << 10,     
    ImGuiButtonFlags_FlattenChildren        = 1 << 11,           
    ImGuiButtonFlags_AllowItemOverlap       = 1 << 12,                    
    ImGuiButtonFlags_DontClosePopups        = 1 << 13,           
    ImGuiButtonFlags_Disabled               = 1 << 14,    
    ImGuiButtonFlags_AlignTextBaseLine      = 1 << 15,                           
    ImGuiButtonFlags_NoKeyModifiers         = 1 << 16,           
    ImGuiButtonFlags_NoHoldingActiveId      = 1 << 17,           
    ImGuiButtonFlags_NoNavFocus             = 1 << 18,        
    ImGuiButtonFlags_NoHoveredOnFocus       = 1 << 19,             
    ImGuiButtonFlags_PressedOnMask_         = ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnClickReleaseAnywhere | ImGuiButtonFlags_PressedOnRelease | ImGuiButtonFlags_PressedOnDoubleClick | ImGuiButtonFlags_PressedOnDragDropHold,
    ImGuiButtonFlags_PressedOnDefault_      = ImGuiButtonFlags_PressedOnClickRelease
};

enum ImGuiSliderFlagsPrivate_
{
    ImGuiSliderFlags_Vertical               = 1 << 20,        
    ImGuiSliderFlags_ReadOnly               = 1 << 21
};

enum ImGuiSelectableFlagsPrivate_
{
    ImGuiSelectableFlags_NoHoldingActiveID      = 1 << 20,
    ImGuiSelectableFlags_SelectOnClick          = 1 << 21,            
    ImGuiSelectableFlags_SelectOnRelease        = 1 << 22,            
    ImGuiSelectableFlags_SpanAvailWidth         = 1 << 23,                            
    ImGuiSelectableFlags_DrawHoveredWhenHeld    = 1 << 24,                  
    ImGuiSelectableFlags_SetNavIdOnHover        = 1 << 25,           
    ImGuiSelectableFlags_NoPadWithHalfSpacing   = 1 << 26           
};

enum ImGuiTreeNodeFlagsPrivate_
{
    ImGuiTreeNodeFlags_ClipLabelForTrailingButton = 1 << 20
};

enum ImGuiSeparatorFlags_
{
    ImGuiSeparatorFlags_None                = 0,
    ImGuiSeparatorFlags_Horizontal          = 1 << 0,                  
    ImGuiSeparatorFlags_Vertical            = 1 << 1,
    ImGuiSeparatorFlags_SpanAllColumns      = 1 << 2
};

enum ImGuiTextFlags_
{
    ImGuiTextFlags_None = 0,
    ImGuiTextFlags_NoWidthForLargeClippedText = 1 << 0
};

enum ImGuiTooltipFlags_
{
    ImGuiTooltipFlags_None = 0,
    ImGuiTooltipFlags_OverridePreviousTooltip = 1 << 0               
};

enum ImGuiLayoutType_
{
    ImGuiLayoutType_Horizontal = 0,
    ImGuiLayoutType_Vertical = 1
};

enum ImGuiLogType
{
    ImGuiLogType_None = 0,
    ImGuiLogType_TTY,
    ImGuiLogType_File,
    ImGuiLogType_Buffer,
    ImGuiLogType_Clipboard
};

enum ImGuiAxis
{
    ImGuiAxis_None = -1,
    ImGuiAxis_X = 0,
    ImGuiAxis_Y = 1
};

enum ImGuiPlotType
{
    ImGuiPlotType_Lines,
    ImGuiPlotType_Histogram
};

enum ImGuiInputSource
{
    ImGuiInputSource_None = 0,
    ImGuiInputSource_Mouse,
    ImGuiInputSource_Nav,
    ImGuiInputSource_NavKeyboard,             
    ImGuiInputSource_NavGamepad,     
    ImGuiInputSource_COUNT
};

enum ImGuiInputReadMode
{
    ImGuiInputReadMode_Down,
    ImGuiInputReadMode_Pressed,
    ImGuiInputReadMode_Released,
    ImGuiInputReadMode_Repeat,
    ImGuiInputReadMode_RepeatSlow,
    ImGuiInputReadMode_RepeatFast
};

enum ImGuiNavHighlightFlags_
{
    ImGuiNavHighlightFlags_None         = 0,
    ImGuiNavHighlightFlags_TypeDefault  = 1 << 0,
    ImGuiNavHighlightFlags_TypeThin     = 1 << 1,
    ImGuiNavHighlightFlags_AlwaysDraw   = 1 << 2,                   
    ImGuiNavHighlightFlags_NoRounding   = 1 << 3
};

enum ImGuiNavDirSourceFlags_
{
    ImGuiNavDirSourceFlags_None         = 0,
    ImGuiNavDirSourceFlags_Keyboard     = 1 << 0,
    ImGuiNavDirSourceFlags_PadDPad      = 1 << 1,
    ImGuiNavDirSourceFlags_PadLStick    = 1 << 2
};

enum ImGuiNavMoveFlags_
{
    ImGuiNavMoveFlags_None                  = 0,
    ImGuiNavMoveFlags_LoopX                 = 1 << 0,          
    ImGuiNavMoveFlags_LoopY                 = 1 << 1,
    ImGuiNavMoveFlags_WrapX                 = 1 << 2,                     
    ImGuiNavMoveFlags_WrapY                 = 1 << 3,            
    ImGuiNavMoveFlags_AllowCurrentNavId     = 1 << 4,                                                  
    ImGuiNavMoveFlags_AlsoScoreVisibleSet   = 1 << 5,                 
    ImGuiNavMoveFlags_ScrollToEdge          = 1 << 6
};

enum ImGuiNavForward
{
    ImGuiNavForward_None,
    ImGuiNavForward_ForwardQueued,
    ImGuiNavForward_ForwardActive
};

enum ImGuiNavLayer
{
    ImGuiNavLayer_Main  = 0,       
    ImGuiNavLayer_Menu  = 1,         
    ImGuiNavLayer_COUNT
};

enum ImGuiPopupPositionPolicy
{
    ImGuiPopupPositionPolicy_Default,
    ImGuiPopupPositionPolicy_ComboBox,
    ImGuiPopupPositionPolicy_Tooltip
};

struct ImGuiDataTypeTempStorage
{
    ImU8        Data[8];               
};

struct ImGuiDataTypeInfo
{
    size_t      Size;              
    const char* Name;                   
    const char* PrintFmt;             
    const char* ScanFmt;              
};

enum ImGuiDataTypePrivate_
{
    ImGuiDataType_String = ImGuiDataType_COUNT + 1,
    ImGuiDataType_Pointer,
    ImGuiDataType_ID
};

struct ImGuiColorMod
{
    ImGuiCol    Col;
    ImVec4      BackupValue;
};

struct ImGuiStyleMod
{
    ImGuiStyleVar   VarIdx;
    union           { int BackupInt[2]; float BackupFloat[2]; };
    ImGuiStyleMod(ImGuiStyleVar idx, int v)     { VarIdx = idx; BackupInt[0] = v; }
    ImGuiStyleMod(ImGuiStyleVar idx, float v)   { VarIdx = idx; BackupFloat[0] = v; }
    ImGuiStyleMod(ImGuiStyleVar idx, ImVec2 v)  { VarIdx = idx; BackupFloat[0] = v.x; BackupFloat[1] = v.y; }
};

struct ImGuiGroupData
{
    ImGuiID     WindowID;
    ImVec2      BackupCursorPos;
    ImVec2      BackupCursorMaxPos;
    ImVec1      BackupIndent;
    ImVec1      BackupGroupOffset;
    ImVec2      BackupCurrLineSize;
    float       BackupCurrLineTextBaseOffset;
    ImGuiID     BackupActiveIdIsAlive;
    bool        BackupActiveIdPreviousFrameIsAlive;
    bool        EmitItem;
};

struct IMGUI_API ImGuiMenuColumns
{
    float       Spacing;
    float       Width, NextWidth;
    float       Pos[3], NextWidths[3];

    ImGuiMenuColumns() { memset(this, 0, sizeof(*this)); }
    void        Update(int count, float spacing, bool clear);
    float       DeclColumns(float w0, float w1, float w2);
    float       CalcExtraSpace(float avail_w) const;
};

struct IMGUI_API ImGuiInputTextState
{
    ImGuiID                 ID;                           
    int                     CurLenW, CurLenA;                             
    ImVector<ImWchar>       TextW;                                       
    ImVector<char>          TextA;                                  
    ImVector<char>          InitialTextA;                       
    bool                    TextAIsValid;                                 
    int                     BufCapacityA;              
    float                   ScrollX;                  
    ImStb::STB_TexteditState Stb;                      
    float                   CursorAnim;                           
    bool                    CursorFollow;                        
    bool                    SelectedAllMouseLock;                 
    bool                    Edited;                    
    ImGuiInputTextFlags     UserFlags;                     
    ImGuiInputTextCallback  UserCallback;            
    void*                   UserCallbackData;        

    ImGuiInputTextState()                   { memset(this, 0, sizeof(*this)); }
    void        ClearText()                 { CurLenW = CurLenA = 0; TextW[0] = 0; TextA[0] = 0; CursorClamp(); }
    void        ClearFreeMemory()           { TextW.clear(); TextA.clear(); InitialTextA.clear(); }
    int         GetUndoAvailCount() const   { return Stb.undostate.undo_point; }
    int         GetRedoAvailCount() const   { return STB_TEXTEDIT_UNDOSTATECOUNT - Stb.undostate.redo_point; }
    void        OnKeyPressed(int key);                 

    void        CursorAnimReset()           { CursorAnim = -0.30f; }                                               
    void        CursorClamp()               { Stb.cursor = ImMin(Stb.cursor, CurLenW); Stb.select_start = ImMin(Stb.select_start, CurLenW); Stb.select_end = ImMin(Stb.select_end, CurLenW); }
    bool        HasSelection() const        { return Stb.select_start != Stb.select_end; }
    void        ClearSelection()            { Stb.select_start = Stb.select_end = Stb.cursor; }
    void        SelectAll()                 { Stb.select_start = 0; Stb.cursor = Stb.select_end = CurLenW; Stb.has_preferred_x = 0; }
};

struct ImGuiPopupData
{
    ImGuiID             PopupId;           
    ImGuiWindow*        Window;                     
    ImGuiWindow*        SourceWindow;                
    int                 OpenFrameCount;    
    ImGuiID             OpenParentId;                         
    ImVec2              OpenPopupPos;               
    ImVec2              OpenMousePos;                

    ImGuiPopupData()    { memset(this, 0, sizeof(*this)); OpenFrameCount = -1; }
};

struct ImGuiNavMoveResult
{
    ImGuiWindow*    Window;                
    ImGuiID         ID;                    
    ImGuiID         FocusScopeId;            
    float           DistBox;                   
    float           DistCenter;                
    float           DistAxial;
    ImRect          RectRel;                    

    ImGuiNavMoveResult() { Clear(); }
    void Clear()         { Window = NULL; ID = FocusScopeId = 0; DistBox = DistCenter = DistAxial = FLT_MAX; RectRel = ImRect(); }
};

enum ImGuiNextWindowDataFlags_
{
    ImGuiNextWindowDataFlags_None               = 0,
    ImGuiNextWindowDataFlags_HasPos             = 1 << 0,
    ImGuiNextWindowDataFlags_HasSize            = 1 << 1,
    ImGuiNextWindowDataFlags_HasContentSize     = 1 << 2,
    ImGuiNextWindowDataFlags_HasCollapsed       = 1 << 3,
    ImGuiNextWindowDataFlags_HasSizeConstraint  = 1 << 4,
    ImGuiNextWindowDataFlags_HasFocus           = 1 << 5,
    ImGuiNextWindowDataFlags_HasBgAlpha         = 1 << 6,
    ImGuiNextWindowDataFlags_HasScroll          = 1 << 7
};

struct ImGuiNextWindowData
{
    ImGuiNextWindowDataFlags    Flags;
    ImGuiCond                   PosCond;
    ImGuiCond                   SizeCond;
    ImGuiCond                   CollapsedCond;
    ImVec2                      PosVal;
    ImVec2                      PosPivotVal;
    ImVec2                      SizeVal;
    ImVec2                      ContentSizeVal;
    ImVec2                      ScrollVal;
    bool                        CollapsedVal;
    ImRect                      SizeConstraintRect;
    ImGuiSizeCallback           SizeCallback;
    void*                       SizeCallbackUserData;
    float                       BgAlphaVal;                
    ImVec2                      MenuBarOffsetMinVal;                

    ImGuiNextWindowData()       { memset(this, 0, sizeof(*this)); }
    inline void ClearFlags()    { Flags = ImGuiNextWindowDataFlags_None; }
};

enum ImGuiNextItemDataFlags_
{
    ImGuiNextItemDataFlags_None     = 0,
    ImGuiNextItemDataFlags_HasWidth = 1 << 0,
    ImGuiNextItemDataFlags_HasOpen  = 1 << 1
};

struct ImGuiNextItemData
{
    ImGuiNextItemDataFlags      Flags;
    float                       Width;             
    ImGuiID                     FocusScopeId;                                      
    ImGuiCond                   OpenCond;
    bool                        OpenVal;           

    ImGuiNextItemData()         { memset(this, 0, sizeof(*this)); }
    inline void ClearFlags()    { Flags = ImGuiNextItemDataFlags_None; }      
};

struct ImGuiShrinkWidthItem
{
    int         Index;
    float       Width;
};

struct ImGuiPtrOrIndex
{
    void*       Ptr;                                 
    int         Index;                

    ImGuiPtrOrIndex(void* ptr)  { Ptr = ptr; Index = -1; }
    ImGuiPtrOrIndex(int index)  { Ptr = NULL; Index = index; }
};

enum ImGuiOldColumnFlags_
{
    ImGuiOldColumnFlags_None                    = 0,
    ImGuiOldColumnFlags_NoBorder                = 1 << 0,      
    ImGuiOldColumnFlags_NoResize                = 1 << 1,           
    ImGuiOldColumnFlags_NoPreserveWidths        = 1 << 2,          
    ImGuiOldColumnFlags_NoForceWithinWindow     = 1 << 3,          
    ImGuiOldColumnFlags_GrowParentContentsSize  = 1 << 4                          

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    , ImGuiColumnsFlags_None                    = ImGuiOldColumnFlags_None,
    ImGuiColumnsFlags_NoBorder                  = ImGuiOldColumnFlags_NoBorder,
    ImGuiColumnsFlags_NoResize                  = ImGuiOldColumnFlags_NoResize,
    ImGuiColumnsFlags_NoPreserveWidths          = ImGuiOldColumnFlags_NoPreserveWidths,
    ImGuiColumnsFlags_NoForceWithinWindow       = ImGuiOldColumnFlags_NoForceWithinWindow,
    ImGuiColumnsFlags_GrowParentContentsSize    = ImGuiOldColumnFlags_GrowParentContentsSize
#endif
};

struct ImGuiOldColumnData
{
    float               OffsetNorm;                    
    float               OffsetNormBeforeResize;
    ImGuiOldColumnFlags Flags;                
    ImRect              ClipRect;

    ImGuiOldColumnData() { memset(this, 0, sizeof(*this)); }
};

struct ImGuiOldColumns
{
    ImGuiID             ID;
    ImGuiOldColumnFlags Flags;
    bool                IsFirstFrame;
    bool                IsBeingResized;
    int                 Current;
    int                 Count;
    float               OffMinX, OffMaxX;          
    float               LineMinY, LineMaxY;
    float               HostCursorPosY;                 
    float               HostCursorMaxPosX;              
    ImRect              HostInitialClipRect;            
    ImRect              HostBackupClipRect;          
    ImRect              HostBackupParentWorkRect;       
    ImVector<ImGuiOldColumnData> Columns;
    ImDrawListSplitter  Splitter;

    ImGuiOldColumns()   { memset(this, 0, sizeof(*this)); }
};

#ifdef IMGUI_HAS_MULTI_SELECT
#endif   

#ifdef IMGUI_HAS_DOCK
#endif   

#ifdef IMGUI_HAS_VIEWPORT
#endif   

struct ImGuiWindowSettings
{
    ImGuiID     ID;
    ImVec2ih    Pos;
    ImVec2ih    Size;
    bool        Collapsed;
    bool        WantApply;                      

    ImGuiWindowSettings()       { memset(this, 0, sizeof(*this)); }
    char* GetName()             { return (char*)(this + 1); }
};

struct ImGuiSettingsHandler
{
    const char* TypeName;                 
    ImGuiID     TypeHash;         
    void        (*ClearAllFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler);                                    
    void        (*ReadInitFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler);                                       
    void*       (*ReadOpenFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler, const char* name);                         
    void        (*ReadLineFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler, void* entry, const char* line);            
    void        (*ApplyAllFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler);                                       
    void        (*WriteAllFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* out_buf);            
    void*       UserData;

    ImGuiSettingsHandler() { memset(this, 0, sizeof(*this)); }
};

struct ImGuiMetricsConfig
{
    bool        ShowWindowsRects;
    bool        ShowWindowsBeginOrder;
    bool        ShowTablesRects;
    bool        ShowDrawCmdMesh;
    bool        ShowDrawCmdBoundingBoxes;
    int         ShowWindowsRectsType;
    int         ShowTablesRectsType;

    ImGuiMetricsConfig()
    {
        ShowWindowsRects = false;
        ShowWindowsBeginOrder = false;
        ShowTablesRects = false;
        ShowDrawCmdMesh = true;
        ShowDrawCmdBoundingBoxes = true;
        ShowWindowsRectsType = -1;
        ShowTablesRectsType = -1;
    }
};

struct IMGUI_API ImGuiStackSizes
{
    short   SizeOfIDStack;
    short   SizeOfColorStack;
    short   SizeOfStyleVarStack;
    short   SizeOfFontStack;
    short   SizeOfFocusScopeStack;
    short   SizeOfGroupStack;
    short   SizeOfBeginPopupStack;

    ImGuiStackSizes() { memset(this, 0, sizeof(*this)); }
    void SetToCurrentState();
    void CompareWithCurrentState();
};

typedef void (*ImGuiContextHookCallback)(ImGuiContext* ctx, ImGuiContextHook* hook);
enum ImGuiContextHookType { ImGuiContextHookType_NewFramePre, ImGuiContextHookType_NewFramePost, ImGuiContextHookType_EndFramePre, ImGuiContextHookType_EndFramePost, ImGuiContextHookType_RenderPre, ImGuiContextHookType_RenderPost, ImGuiContextHookType_Shutdown, ImGuiContextHookType_PendingRemoval_ };

struct ImGuiContextHook
{
    ImGuiID                     HookId;           
    ImGuiContextHookType        Type;
    ImGuiID                     Owner;
    ImGuiContextHookCallback    Callback;
    void*                       UserData;

    ImGuiContextHook()          { memset(this, 0, sizeof(*this)); }
};

struct ImGuiContext
{
    bool                    Initialized;
    bool                    FontAtlasOwnedByContext;                         
    ImGuiIO                 IO;
    ImGuiStyle              Style;
    ImFont*                 Font;                                      
    float                   FontSize;                                       
    float                   FontBaseSize;                                 
    ImDrawListSharedData    DrawListSharedData;
    double                  Time;
    int                     FrameCount;
    int                     FrameCountEnded;
    int                     FrameCountRendered;
    bool                    WithinFrameScope;                         
    bool                    WithinFrameScopeWithImplicitWindow;               
    bool                    WithinEndChild;                        
    bool                    GcCompactAll;                          
    bool                    TestEngineHookItems;                        
    ImGuiID                 TestEngineHookIdInfo;                       
    void*                   TestEngine;                             

    ImVector<ImGuiWindow*>  Windows;                                    
    ImVector<ImGuiWindow*>  WindowsFocusOrder;                                                   
    ImVector<ImGuiWindow*>  WindowsTempSortBuffer;                             
    ImVector<ImGuiWindow*>  CurrentWindowStack;
    ImGuiStorage            WindowsById;                             
    int                     WindowsActiveCount;                        
    ImGuiWindow*            CurrentWindow;                          
    ImGuiWindow*            HoveredWindow;                                
    ImGuiWindow*            HoveredRootWindow;                                  
    ImGuiWindow*            HoveredWindowUnderMovingWindow;               
    ImGuiWindow*            MovingWindow;                                           
    ImGuiWindow*            WheelingWindow;                                                         
    ImVec2                  WheelingWindowRefMousePos;
    float                   WheelingWindowTimer;

    ImGuiID                 HoveredId;                                
    ImGuiID                 HoveredIdPreviousFrame;
    bool                    HoveredIdAllowOverlap;
    bool                    HoveredIdUsingMouseWheel;                      
    bool                    HoveredIdPreviousFrameUsingMouseWheel;
    bool                    HoveredIdDisabled;                                            
    float                   HoveredIdTimer;                         
    float                   HoveredIdNotActiveTimer;                       
    ImGuiID                 ActiveId;                             
    ImGuiID                 ActiveIdIsAlive;                                        
    float                   ActiveIdTimer;
    bool                    ActiveIdIsJustActivated;                     
    bool                    ActiveIdAllowOverlap;                               
    bool                    ActiveIdNoClearOnFocusLoss;                    
    bool                    ActiveIdHasBeenPressedBefore;                                
    bool                    ActiveIdHasBeenEditedBefore;                       
    bool                    ActiveIdHasBeenEditedThisFrame;
    bool                    ActiveIdUsingMouseWheel;                         
    ImU32                   ActiveIdUsingNavDirMask;                                
    ImU32                   ActiveIdUsingNavInputMask;                   
    ImU64                   ActiveIdUsingKeyInputMask;                                               
    ImVec2                  ActiveIdClickOffset;                            
    ImGuiWindow*            ActiveIdWindow;
    ImGuiInputSource        ActiveIdSource;                           
    int                     ActiveIdMouseButton;
    ImGuiID                 ActiveIdPreviousFrame;
    bool                    ActiveIdPreviousFrameIsAlive;
    bool                    ActiveIdPreviousFrameHasBeenEditedBefore;
    ImGuiWindow*            ActiveIdPreviousFrameWindow;
    ImGuiID                 LastActiveId;                               
    float                   LastActiveIdTimer;                                

    ImGuiNextWindowData     NextWindowData;                         
    ImGuiNextItemData       NextItemData;                           

    ImVector<ImGuiColorMod> ColorStack;                                
    ImVector<ImGuiStyleMod> StyleVarStack;                             
    ImVector<ImFont*>       FontStack;                                 
    ImVector<ImGuiID>       FocusScopeStack;                               
    ImVector<ImGuiItemFlags>ItemFlagsStack;                            
    ImVector<ImGuiGroupData>GroupStack;                                 
    ImVector<ImGuiPopupData>OpenPopupStack;                          
    ImVector<ImGuiPopupData>BeginPopupStack;                              

    ImGuiWindow*            NavWindow;                                  
    ImGuiID                 NavId;                                  
    ImGuiID                 NavFocusScopeId;                                         
    ImGuiID                 NavActivateId;                                     
    ImGuiID                 NavActivateDownId;                        
    ImGuiID                 NavActivatePressedId;                     
    ImGuiID                 NavInputId;                               
    ImGuiID                 NavJustTabbedId;                         
    ImGuiID                 NavJustMovedToId;                             
    ImGuiID                 NavJustMovedToFocusScopeId;                     
    ImGuiKeyModFlags        NavJustMovedToKeyMods;
    ImGuiID                 NavNextActivateId;                         
    ImGuiInputSource        NavInputSource;                                  
    ImRect                  NavScoringRect;                                    
    int                     NavScoringCount;                       
    ImGuiNavLayer           NavLayer;                                                
    int                     NavIdTabCounter;                           
    bool                    NavIdIsAlive;                                  
    bool                    NavMousePosDirty;                                      
    bool                    NavDisableHighlight;                                       
    bool                    NavDisableMouseHover;                              
    bool                    NavAnyRequest;                          
    bool                    NavInitRequest;                              
    bool                    NavInitRequestFromMove;
    ImGuiID                 NavInitResultId;                                   
    ImRect                  NavInitResultRectRel;                       
    bool                    NavMoveRequest;                          
    ImGuiNavMoveFlags       NavMoveRequestFlags;
    ImGuiNavForward         NavMoveRequestForward;                               
    ImGuiKeyModFlags        NavMoveRequestKeyMods;
    ImGuiDir                NavMoveDir, NavMoveDirLast;                     
    ImGuiDir                NavMoveClipDir;                                
    ImGuiNavMoveResult      NavMoveResultLocal;                       
    ImGuiNavMoveResult      NavMoveResultLocalVisibleSet;                     
    ImGuiNavMoveResult      NavMoveResultOther;                             
    ImGuiWindow*            NavWrapRequestWindow;                     
    ImGuiNavMoveFlags       NavWrapRequestFlags;                   

    ImGuiWindow*            NavWindowingTarget;                                 
    ImGuiWindow*            NavWindowingTargetAnim;                               
    ImGuiWindow*            NavWindowingListWindow;                    
    float                   NavWindowingTimer;
    float                   NavWindowingHighlightAlpha;
    bool                    NavWindowingToggleLayer;

    ImGuiWindow*            FocusRequestCurrWindow;             
    ImGuiWindow*            FocusRequestNextWindow;             
    int                     FocusRequestCurrCounterRegular;                               
    int                     FocusRequestCurrCounterTabStop;               
    int                     FocusRequestNextCounterRegular;         
    int                     FocusRequestNextCounterTabStop;      
    bool                    FocusTabPressed;                    

    ImDrawData              DrawData;                                     
    ImDrawDataBuilder       DrawDataBuilder;
    float                   DimBgRatio;                                       
    ImDrawList              BackgroundDrawList;                       
    ImDrawList              ForegroundDrawList;                                        
    ImGuiMouseCursor        MouseCursor;

    bool                    DragDropActive;
    bool                    DragDropWithinSource;                         
    bool                    DragDropWithinTarget;                         
    ImGuiDragDropFlags      DragDropSourceFlags;
    int                     DragDropSourceFrameCount;
    int                     DragDropMouseButton;
    ImGuiPayload            DragDropPayload;
    ImRect                  DragDropTargetRect;                             
    ImGuiID                 DragDropTargetId;
    ImGuiDragDropFlags      DragDropAcceptFlags;
    float                   DragDropAcceptIdCurrRectSurface;                
    ImGuiID                 DragDropAcceptIdCurr;                          
    ImGuiID                 DragDropAcceptIdPrev;                                  
    int                     DragDropAcceptFrameCount;                      
    ImGuiID                 DragDropHoldJustPressedId;                     
    ImVector<unsigned char> DragDropPayloadBufHeap;                       
    unsigned char           DragDropPayloadBufLocal[16];             

    ImGuiTable*                     CurrentTable;
    ImPool<ImGuiTable>              Tables;
    ImVector<ImGuiPtrOrIndex>       CurrentTableStack;
    ImVector<float>                 TablesLastTimeActive;                 
    ImVector<ImDrawChannel>         DrawChannelsTempMergeBuffer;

    ImGuiTabBar*                    CurrentTabBar;
    ImPool<ImGuiTabBar>             TabBars;
    ImVector<ImGuiPtrOrIndex>       CurrentTabBarStack;
    ImVector<ImGuiShrinkWidthItem>  ShrinkWidthBuffer;

    ImVec2                  LastValidMousePos;
    ImGuiInputTextState     InputTextState;
    ImFont                  InputTextPasswordFont;
    ImGuiID                 TempInputId;                                 
    ImGuiColorEditFlags     ColorEditOptions;                          
    float                   ColorEditLastHue;                                    
    float                   ColorEditLastSat;                                    
    float                   ColorEditLastColor[3];
    ImVec4                  ColorPickerRef;                               
    float                   SliderCurrentAccum;                        
    bool                    SliderCurrentAccumDirty;                          
    bool                    DragCurrentAccumDirty;
    float                   DragCurrentAccum;                               
    float                   DragSpeedDefaultRatio;                      
    float                   ScrollbarClickDeltaToGrabCenter;                  
    int                     TooltipOverrideCount;
    float                   TooltipSlowDelay;                                   
    ImVector<char>          ClipboardHandlerData;                      
    ImVector<ImGuiID>       MenusIdSubmittedThisFrame;                     

    ImVec2                  PlatformImePos;                                 
    ImVec2                  PlatformImeLastPos;
    char                    PlatformLocaleDecimalPoint;            

    bool                    SettingsLoaded;
    float                   SettingsDirtyTimer;                          
    ImGuiTextBuffer         SettingsIniData;                        
    ImVector<ImGuiSettingsHandler>      SettingsHandlers;            
    ImChunkStream<ImGuiWindowSettings>  SettingsWindows;            
    ImChunkStream<ImGuiTableSettings>   SettingsTables;             
    ImVector<ImGuiContextHook>          Hooks;                        
    ImGuiID                             HookIdNext;                

    bool                    LogEnabled;                           
    ImGuiLogType            LogType;                              
    ImFileHandle            LogFile;                                   
    ImGuiTextBuffer         LogBuffer;                                            
    const char*             LogNextPrefix;
    const char*             LogNextSuffix;
    float                   LogLinePosY;
    bool                    LogLineFirstItem;
    int                     LogDepthRef;
    int                     LogDepthToExpand;
    int                     LogDepthToExpandDefault;                        

    bool                    DebugItemPickerActive;                     
    ImGuiID                 DebugItemPickerBreakId;                    
    ImGuiMetricsConfig      DebugMetricsConfig;

    float                   FramerateSecPerFrame[120];                     
    int                     FramerateSecPerFrameIdx;
    float                   FramerateSecPerFrameAccum;
    int                     WantCaptureMouseNextFrame;                 
    int                     WantCaptureKeyboardNextFrame;
    int                     WantTextInputNextFrame;
    char                    TempBuffer[1024 * 3 + 1];              

    ImGuiContext(ImFontAtlas* shared_font_atlas) : BackgroundDrawList(&DrawListSharedData), ForegroundDrawList(&DrawListSharedData)
    {
        Initialized = false;
        FontAtlasOwnedByContext = shared_font_atlas ? false : true;
        Font = NULL;
        FontSize = FontBaseSize = 0.0f;
        IO.Fonts = shared_font_atlas ? shared_font_atlas : IM_NEW(ImFontAtlas)();
        Time = 0.0f;
        FrameCount = 0;
        FrameCountEnded = FrameCountRendered = -1;
        WithinFrameScope = WithinFrameScopeWithImplicitWindow = WithinEndChild = false;
        GcCompactAll = false;
        TestEngineHookItems = false;
        TestEngineHookIdInfo = 0;
        TestEngine = NULL;

        WindowsActiveCount = 0;
        CurrentWindow = NULL;
        HoveredWindow = NULL;
        HoveredRootWindow = NULL;
        HoveredWindowUnderMovingWindow = NULL;
        MovingWindow = NULL;
        WheelingWindow = NULL;
        WheelingWindowTimer = 0.0f;

        HoveredId = HoveredIdPreviousFrame = 0;
        HoveredIdAllowOverlap = false;
        HoveredIdUsingMouseWheel = HoveredIdPreviousFrameUsingMouseWheel = false;
        HoveredIdDisabled = false;
        HoveredIdTimer = HoveredIdNotActiveTimer = 0.0f;
        ActiveId = 0;
        ActiveIdIsAlive = 0;
        ActiveIdTimer = 0.0f;
        ActiveIdIsJustActivated = false;
        ActiveIdAllowOverlap = false;
        ActiveIdNoClearOnFocusLoss = false;
        ActiveIdHasBeenPressedBefore = false;
        ActiveIdHasBeenEditedBefore = false;
        ActiveIdHasBeenEditedThisFrame = false;
        ActiveIdUsingMouseWheel = false;
        ActiveIdUsingNavDirMask = 0x00;
        ActiveIdUsingNavInputMask = 0x00;
        ActiveIdUsingKeyInputMask = 0x00;
        ActiveIdClickOffset = ImVec2(-1, -1);
        ActiveIdWindow = NULL;
        ActiveIdSource = ImGuiInputSource_None;
        ActiveIdMouseButton = 0;
        ActiveIdPreviousFrame = 0;
        ActiveIdPreviousFrameIsAlive = false;
        ActiveIdPreviousFrameHasBeenEditedBefore = false;
        ActiveIdPreviousFrameWindow = NULL;
        LastActiveId = 0;
        LastActiveIdTimer = 0.0f;

        NavWindow = NULL;
        NavId = NavFocusScopeId = NavActivateId = NavActivateDownId = NavActivatePressedId = NavInputId = 0;
        NavJustTabbedId = NavJustMovedToId = NavJustMovedToFocusScopeId = NavNextActivateId = 0;
        NavJustMovedToKeyMods = ImGuiKeyModFlags_None;
        NavInputSource = ImGuiInputSource_None;
        NavScoringRect = ImRect();
        NavScoringCount = 0;
        NavLayer = ImGuiNavLayer_Main;
        NavIdTabCounter = INT_MAX;
        NavIdIsAlive = false;
        NavMousePosDirty = false;
        NavDisableHighlight = true;
        NavDisableMouseHover = false;
        NavAnyRequest = false;
        NavInitRequest = false;
        NavInitRequestFromMove = false;
        NavInitResultId = 0;
        NavMoveRequest = false;
        NavMoveRequestFlags = ImGuiNavMoveFlags_None;
        NavMoveRequestForward = ImGuiNavForward_None;
        NavMoveRequestKeyMods = ImGuiKeyModFlags_None;
        NavMoveDir = NavMoveDirLast = NavMoveClipDir = ImGuiDir_None;
        NavWrapRequestWindow = NULL;
        NavWrapRequestFlags = ImGuiNavMoveFlags_None;

        NavWindowingTarget = NavWindowingTargetAnim = NavWindowingListWindow = NULL;
        NavWindowingTimer = NavWindowingHighlightAlpha = 0.0f;
        NavWindowingToggleLayer = false;

        FocusRequestCurrWindow = FocusRequestNextWindow = NULL;
        FocusRequestCurrCounterRegular = FocusRequestCurrCounterTabStop = INT_MAX;
        FocusRequestNextCounterRegular = FocusRequestNextCounterTabStop = INT_MAX;
        FocusTabPressed = false;

        DimBgRatio = 0.0f;
        BackgroundDrawList._OwnerName = "##Background";       
        ForegroundDrawList._OwnerName = "##Foreground";       
        MouseCursor = ImGuiMouseCursor_Arrow;

        DragDropActive = DragDropWithinSource = DragDropWithinTarget = false;
        DragDropSourceFlags = ImGuiDragDropFlags_None;
        DragDropSourceFrameCount = -1;
        DragDropMouseButton = -1;
        DragDropTargetId = 0;
        DragDropAcceptFlags = ImGuiDragDropFlags_None;
        DragDropAcceptIdCurrRectSurface = 0.0f;
        DragDropAcceptIdPrev = DragDropAcceptIdCurr = 0;
        DragDropAcceptFrameCount = -1;
        DragDropHoldJustPressedId = 0;
        memset(DragDropPayloadBufLocal, 0, sizeof(DragDropPayloadBufLocal));

        CurrentTable = NULL;
        CurrentTabBar = NULL;

        LastValidMousePos = ImVec2(0.0f, 0.0f);
        TempInputId = 0;
        ColorEditOptions = ImGuiColorEditFlags__OptionsDefault;
        ColorEditLastHue = ColorEditLastSat = 0.0f;
        ColorEditLastColor[0] = ColorEditLastColor[1] = ColorEditLastColor[2] = FLT_MAX;
        SliderCurrentAccum = 0.0f;
        SliderCurrentAccumDirty = false;
        DragCurrentAccumDirty = false;
        DragCurrentAccum = 0.0f;
        DragSpeedDefaultRatio = 1.0f / 100.0f;
        ScrollbarClickDeltaToGrabCenter = 0.0f;
        TooltipOverrideCount = 0;
        TooltipSlowDelay = 0.50f;

        PlatformImePos = PlatformImeLastPos = ImVec2(FLT_MAX, FLT_MAX);
        PlatformLocaleDecimalPoint = '.';

        SettingsLoaded = false;
        SettingsDirtyTimer = 0.0f;
        HookIdNext = 0;

        LogEnabled = false;
        LogType = ImGuiLogType_None;
        LogNextPrefix = LogNextSuffix = NULL;
        LogFile = NULL;
        LogLinePosY = FLT_MAX;
        LogLineFirstItem = false;
        LogDepthRef = 0;
        LogDepthToExpand = LogDepthToExpandDefault = 2;

        DebugItemPickerActive = false;
        DebugItemPickerBreakId = 0;

        memset(FramerateSecPerFrame, 0, sizeof(FramerateSecPerFrame));
        FramerateSecPerFrameIdx = 0;
        FramerateSecPerFrameAccum = 0.0f;
        WantCaptureMouseNextFrame = WantCaptureKeyboardNextFrame = WantTextInputNextFrame = -1;
        memset(TempBuffer, 0, sizeof(TempBuffer));
    }
};

struct IMGUI_API ImGuiWindowTempData
{
    ImVec2                  CursorPos;                    
    ImVec2                  CursorPosPrevLine;
    ImVec2                  CursorStartPos;                   
    ImVec2                  CursorMaxPos;                                
    ImVec2                  IdealMaxPos;                               
    ImVec2                  CurrLineSize;
    ImVec2                  PrevLineSize;
    float                   CurrLineTextBaseOffset;                    
    float                   PrevLineTextBaseOffset;
    ImVec1                  Indent;                             
    ImVec1                  ColumnsOffset;                                      
    ImVec1                  GroupOffset;

    ImGuiID                 LastItemId;                 
    ImGuiItemStatusFlags    LastItemStatusFlags;           
    ImRect                  LastItemRect;                
    ImRect                  LastItemDisplayRect;                

    ImGuiNavLayer           NavLayerCurrent;                
    int                     NavLayerActiveMask;               
    int                     NavLayerActiveMaskNext;           
    ImGuiID                 NavFocusScopeIdCurrent;       
    bool                    NavHideHighlightOneFrame;
    bool                    NavHasScroll;                    

    bool                    MenuBarAppending;          
    ImVec2                  MenuBarOffset;                                            
    ImGuiMenuColumns        MenuColumns;                   
    int                     TreeDepth;                 
    ImU32                   TreeJumpToParentOnPopMask;                 
    ImVector<ImGuiWindow*>  ChildWindows;
    ImGuiStorage*           StateStorage;                     
    ImGuiOldColumns*        CurrentColumns;            
    int                     CurrentTableIdx;             
    ImGuiLayoutType         LayoutType;
    ImGuiLayoutType         ParentLayoutType;                 
    int                     FocusCounterRegular;                     
    int                     FocusCounterTabStop;                 

    ImGuiItemFlags          ItemFlags;                
    float                   ItemWidth;                              
    float                   TextWrapPos;                
    ImVector<float>         ItemWidthStack;                    
    ImVector<float>         TextWrapPosStack;                   
    ImGuiStackSizes         StackSizesOnBegin;             
};

struct IMGUI_API ImGuiWindow
{
    char*                   Name;                                     
    ImGuiID                 ID;                                   
    ImGuiWindowFlags        Flags;                                 
    ImVec2                  Pos;                                      
    ImVec2                  Size;                                       
    ImVec2                  SizeFull;                               
    ImVec2                  ContentSize;                                                
    ImVec2                  ContentSizeIdeal;
    ImVec2                  ContentSizeExplicit;                            
    ImVec2                  WindowPadding;                             
    float                   WindowRounding;                                          
    float                   WindowBorderSize;                           
    int                     NameBufLen;                                   
    ImGuiID                 MoveId;                               
    ImGuiID                 ChildId;                                             
    ImVec2                  Scroll;
    ImVec2                  ScrollMax;
    ImVec2                  ScrollTarget;                                             
    ImVec2                  ScrollTargetCenterRatio;                               
    ImVec2                  ScrollTargetEdgeSnapDist;                  
    ImVec2                  ScrollbarSizes;                                              
    bool                    ScrollbarX, ScrollbarY;                
    bool                    Active;                                    
    bool                    WasActive;
    bool                    WriteAccessed;                                
    bool                    Collapsed;                                  
    bool                    WantCollapseToggle;
    bool                    SkipItems;                                        
    bool                    Appearing;                                     
    bool                    Hidden;                                    
    bool                    IsFallbackWindow;                        
    bool                    HasCloseButton;                                
    signed char             ResizeBorderHeld;                             
    short                   BeginCount;                                           
    short                   BeginOrderWithinParent;                          
    short                   BeginOrderWithinContext;                           
    ImGuiID                 PopupId;                                                
    ImS8                    AutoFitFramesX, AutoFitFramesY;
    ImS8                    AutoFitChildAxises;
    bool                    AutoFitOnlyGrows;
    ImGuiDir                AutoPosLastDirection;
    ImS8                    HiddenFramesCanSkipItems;                 
    ImS8                    HiddenFramesCannotSkipItems;                          
    ImS8                    HiddenFramesForRenderOnly;                    
    ImGuiCond               SetWindowPosAllowFlags : 8;                
    ImGuiCond               SetWindowSizeAllowFlags : 8;               
    ImGuiCond               SetWindowCollapsedAllowFlags : 8;          
    ImVec2                  SetWindowPosVal;                                        
    ImVec2                  SetWindowPosPivot;                                       

    ImVector<ImGuiID>       IDStack;                                                    
    ImGuiWindowTempData     DC;                                                      

    ImRect                  OuterRectClipped;                               
    ImRect                  InnerRect;                                   
    ImRect                  InnerClipRect;                                     
    ImRect                  WorkRect;                                                         
    ImRect                  ParentWorkRect;                                                    
    ImRect                  ClipRect;                                       
    ImRect                  ContentRegionRect;                                                
    ImVec2ih                HitTestHoleSize;                               
    ImVec2ih                HitTestHoleOffset;

    int                     LastFrameActive;                           
    float                   LastTimeActive;                                    
    float                   ItemWidthDefault;
    ImGuiStorage            StateStorage;
    ImVector<ImGuiOldColumns> ColumnsStorage;
    float                   FontWindowScale;                          
    int                     SettingsOffset;                                     

    ImDrawList*             DrawList;                                          
    ImDrawList              DrawListInst;
    ImGuiWindow*            ParentWindow;                                       
    ImGuiWindow*            RootWindow;                                        
    ImGuiWindow*            RootWindowForTitleBarHighlight;                     
    ImGuiWindow*            RootWindowForNav;                               

    ImGuiWindow*            NavLastChildNavWindow;                                             
    ImGuiID                 NavLastIds[ImGuiNavLayer_COUNT];             
    ImRect                  NavRectRel[ImGuiNavLayer_COUNT];          

    int                     MemoryDrawListIdxCapacity;                            
    int                     MemoryDrawListVtxCapacity;
    bool                    MemoryCompacted;                             

public:
    ImGuiWindow(ImGuiContext* context, const char* name);
    ~ImGuiWindow();

    ImGuiID     GetID(const char* str, const char* str_end = NULL);
    ImGuiID     GetID(const void* ptr);
    ImGuiID     GetID(int n);
    ImGuiID     GetIDNoKeepAlive(const char* str, const char* str_end = NULL);
    ImGuiID     GetIDNoKeepAlive(const void* ptr);
    ImGuiID     GetIDNoKeepAlive(int n);
    ImGuiID     GetIDFromRectangle(const ImRect& r_abs);

    ImRect      Rect() const            { return ImRect(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y); }
    float       CalcFontSize() const    { ImGuiContext& g = *GImGui; float scale = g.FontBaseSize * FontWindowScale; if (ParentWindow) scale *= ParentWindow->FontWindowScale; return scale; }
    float       TitleBarHeight() const  { ImGuiContext& g = *GImGui; return (Flags & ImGuiWindowFlags_NoTitleBar) ? 0.0f : CalcFontSize() + g.Style.FramePadding.y * 2.0f; }
    ImRect      TitleBarRect() const    { return ImRect(Pos, ImVec2(Pos.x + SizeFull.x, Pos.y + TitleBarHeight())); }
    float       MenuBarHeight() const   { ImGuiContext& g = *GImGui; return (Flags & ImGuiWindowFlags_MenuBar) ? DC.MenuBarOffset.y + CalcFontSize() + g.Style.FramePadding.y * 2.0f : 0.0f; }
    ImRect      MenuBarRect() const     { float y1 = Pos.y + TitleBarHeight(); return ImRect(Pos.x, y1, Pos.x + SizeFull.x, y1 + MenuBarHeight()); }
};

struct ImGuiLastItemDataBackup
{
    ImGuiID                 LastItemId;
    ImGuiItemStatusFlags    LastItemStatusFlags;
    ImRect                  LastItemRect;
    ImRect                  LastItemDisplayRect;

    ImGuiLastItemDataBackup() { Backup(); }
    void Backup()           { ImGuiWindow* window = GImGui->CurrentWindow; LastItemId = window->DC.LastItemId; LastItemStatusFlags = window->DC.LastItemStatusFlags; LastItemRect = window->DC.LastItemRect; LastItemDisplayRect = window->DC.LastItemDisplayRect; }
    void Restore() const    { ImGuiWindow* window = GImGui->CurrentWindow; window->DC.LastItemId = LastItemId; window->DC.LastItemStatusFlags = LastItemStatusFlags; window->DC.LastItemRect = LastItemRect; window->DC.LastItemDisplayRect = LastItemDisplayRect; }
};

enum ImGuiTabBarFlagsPrivate_
{
    ImGuiTabBarFlags_DockNode                   = 1 << 20,                        
    ImGuiTabBarFlags_IsFocused                  = 1 << 21,
    ImGuiTabBarFlags_SaveSettings               = 1 << 22                        
};

enum ImGuiTabItemFlagsPrivate_
{
    ImGuiTabItemFlags_NoCloseButton             = 1 << 20,                      
    ImGuiTabItemFlags_Button                    = 1 << 21               
};

struct ImGuiTabItem
{
    ImGuiID             ID;
    ImGuiTabItemFlags   Flags;
    int                 LastFrameVisible;
    int                 LastFrameSelected;                      
    float               Offset;                       
    float               Width;                     
    float               ContentWidth;                  
    ImS16               NameOffset;                     
    ImS16               BeginOrder;                      
    ImS16               IndexDuringLayout;           
    bool                WantClose;                   

    ImGuiTabItem()      { memset(this, 0, sizeof(*this)); LastFrameVisible = LastFrameSelected = -1; NameOffset = BeginOrder = IndexDuringLayout = -1; }
};

struct ImGuiTabBar
{
    ImVector<ImGuiTabItem> Tabs;
    ImGuiTabBarFlags    Flags;
    ImGuiID             ID;                           
    ImGuiID             SelectedTabId;            
    ImGuiID             NextSelectedTabId;
    ImGuiID             VisibleTabId;                       
    int                 CurrFrameVisible;
    int                 PrevFrameVisible;
    ImRect              BarRect;
    float               CurrTabsContentsHeight;
    float               PrevTabsContentsHeight;           
    float               WidthAllTabs;                   
    float               WidthAllTabsIdeal;                
    float               ScrollingAnim;
    float               ScrollingTarget;
    float               ScrollingTargetDistToVisibility;
    float               ScrollingSpeed;
    float               ScrollingRectMinX;
    float               ScrollingRectMaxX;
    ImGuiID             ReorderRequestTabId;
    ImS8                ReorderRequestDir;
    ImS8                BeginCount;
    bool                WantLayout;
    bool                VisibleTabWasSubmitted;
    bool                TabsAddedNew;                               
    ImS16               TabsActiveCount;              
    ImS16               LastTabItemIdx;                  
    float               ItemSpacingY;
    ImVec2              FramePadding;                  
    ImVec2              BackupCursorPos;
    ImGuiTextBuffer     TabsNames;                         

    ImGuiTabBar();
    int                 GetTabOrder(const ImGuiTabItem* tab) const  { return Tabs.index_from_ptr(tab); }
    const char*         GetTabName(const ImGuiTabItem* tab) const
    {
        IM_ASSERT(tab->NameOffset != -1 && (int)tab->NameOffset < TabsNames.Buf.Size);
        return TabsNames.Buf.Data + tab->NameOffset;
    }
};

#ifdef IMGUI_HAS_TABLE

#define IM_COL32_DISABLE                IM_COL32(0,0,0,1)              
#define IMGUI_TABLE_MAX_COLUMNS         64                                 
#define IMGUI_TABLE_MAX_DRAW_CHANNELS   (4 + 64 * 2)          

typedef ImS8 ImGuiTableColumnIdx;
typedef ImU8 ImGuiTableDrawChannelIdx;

struct ImGuiTableColumn
{
    ImGuiTableColumnFlags   Flags;                                       
    float                   WidthGiven;                                                   
    float                   MinX;                             
    float                   MaxX;
    float                   WidthRequest;                                      
    float                   WidthAuto;                        
    float                   StretchWeight;                             
    float                   InitStretchWeightOrWidth;                    
    ImRect                  ClipRect;                            
    ImGuiID                 UserID;                              
    float                   WorkMinX;                                      
    float                   WorkMaxX;                               
    float                   ItemWidth;                               
    float                   ContentMaxXFrozen;                              
    float                   ContentMaxXUnfrozen;
    float                   ContentMaxXHeadersUsed;                                 
    float                   ContentMaxXHeadersIdeal;
    ImS16                   NameOffset;                         
    ImGuiTableColumnIdx     DisplayOrder;                             
    ImGuiTableColumnIdx     IndexWithinEnabledSet;                
    ImGuiTableColumnIdx     PrevEnabledColumn;                          
    ImGuiTableColumnIdx     NextEnabledColumn;                          
    ImGuiTableColumnIdx     SortOrder;                                            
    ImGuiTableDrawChannelIdx DrawChannelCurrent;               
    ImGuiTableDrawChannelIdx DrawChannelFrozen;
    ImGuiTableDrawChannelIdx DrawChannelUnfrozen;
    bool                    IsEnabled;                                       
    bool                    IsEnabledNextFrame;
    bool                    IsVisibleX;                                  
    bool                    IsVisibleY;
    bool                    IsRequestOutput;                               
    bool                    IsSkipItems;                                    
    bool                    IsPreserveWidthAuto;
    ImS8                    NavLayerCurrent;                    
    ImU8                    AutoFitQueue;                               
    ImU8                    CannotSkipItemsQueue;                       
    ImU8                    SortDirection : 2;                 
    ImU8                    SortDirectionsAvailCount : 2;           
    ImU8                    SortDirectionsAvailMask : 4;           
    ImU8                    SortDirectionsAvailList;               

    ImGuiTableColumn()
    {
        memset(this, 0, sizeof(*this));
        StretchWeight = WidthRequest = -1.0f;
        NameOffset = -1;
        DisplayOrder = IndexWithinEnabledSet = -1;
        PrevEnabledColumn = NextEnabledColumn = -1;
        SortOrder = -1;
        SortDirection = ImGuiSortDirection_None;
        DrawChannelCurrent = DrawChannelFrozen = DrawChannelUnfrozen = (ImU8)-1;
    }
};

struct ImGuiTableCellData
{
    ImU32                       BgColor;      
    ImGuiTableColumnIdx         Column;       
};

struct ImGuiTable
{
    ImGuiID                     ID;
    ImGuiTableFlags             Flags;
    void*                       RawData;                            
    ImSpan<ImGuiTableColumn>    Columns;                       
    ImSpan<ImGuiTableColumnIdx> DisplayOrderToIndex;                       
    ImSpan<ImGuiTableCellData>  RowCellData;                          
    ImU64                       EnabledMaskByDisplayOrder;       
    ImU64                       EnabledMaskByIndex;                              
    ImU64                       VisibleMaskByIndex;                        
    ImU64                       RequestOutputMaskByIndex;               
    ImGuiTableFlags             SettingsLoadedFlags;                          
    int                         SettingsOffset;                
    int                         LastFrameActive;
    int                         ColumnsCount;                     
    int                         CurrentRow;
    int                         CurrentColumn;
    ImS16                       InstanceCurrent;                                                  
    ImS16                       InstanceInteracted;                      
    float                       RowPosY1;
    float                       RowPosY2;
    float                       RowMinHeight;                   
    float                       RowTextBaseline;
    float                       RowIndentOffsetX;
    ImGuiTableRowFlags          RowFlags : 16;                   
    ImGuiTableRowFlags          LastRowFlags : 16;
    int                         RowBgColorCounter;                                
    ImU32                       RowBgColor[2];                    
    ImU32                       BorderColorStrong;
    ImU32                       BorderColorLight;
    float                       BorderX1;
    float                       BorderX2;
    float                       HostIndentX;
    float                       MinColumnWidth;
    float                       OuterPaddingX;
    float                       CellPaddingX;                   
    float                       CellPaddingY;
    float                       CellSpacingX1;                  
    float                       CellSpacingX2;
    float                       LastOuterHeight;                 
    float                       LastFirstRowHeight;                
    float                       InnerWidth;                               
    float                       ColumnsGivenWidth;               
    float                       ColumnsAutoFitWidth;                             
    float                       ResizedColumnNextWidth;
    float                       ResizeLockMinContentsX2;                        
    float                       RefScale;                              
    ImRect                      OuterRect;                                    
    ImRect                      InnerRect;                              
    ImRect                      WorkRect;
    ImRect                      InnerClipRect;
    ImRect                      BgClipRect;                          
    ImRect                      Bg0ClipRectForDrawCmd;                           
    ImRect                      Bg2ClipRectForDrawCmd;                                
    ImRect                      HostClipRect;                                      
    ImRect                      HostBackupWorkRect;                 
    ImRect                      HostBackupParentWorkRect;           
    ImRect                      HostBackupInnerClipRect;         
    ImVec2                      HostBackupPrevLineSize;             
    ImVec2                      HostBackupCurrLineSize;             
    ImVec2                      HostBackupCursorMaxPos;             
    ImVec2                      UserOuterSize;                  
    ImVec1                      HostBackupColumnsOffset;            
    float                       HostBackupItemWidth;                
    int                         HostBackupItemWidthStackSize;        
    ImGuiWindow*                OuterWindow;                     
    ImGuiWindow*                InnerWindow;                           
    ImGuiTextBuffer             ColumnsNames;                    
    ImDrawListSplitter          DrawSplitter;                                      
    ImGuiTableColumnSortSpecs   SortSpecsSingle;
    ImVector<ImGuiTableColumnSortSpecs> SortSpecsMulti;              
    ImGuiTableSortSpecs         SortSpecs;                             
    ImGuiTableColumnIdx         SortSpecsCount;
    ImGuiTableColumnIdx         ColumnsEnabledCount;              
    ImGuiTableColumnIdx         ColumnsEnabledFixedCount;         
    ImGuiTableColumnIdx         DeclColumnsCount;               
    ImGuiTableColumnIdx         HoveredColumnBody;                              
    ImGuiTableColumnIdx         HoveredColumnBorder;                  
    ImGuiTableColumnIdx         AutoFitSingleColumn;              
    ImGuiTableColumnIdx         ResizedColumn;                      
    ImGuiTableColumnIdx         LastResizedColumn;                  
    ImGuiTableColumnIdx         HeldHeaderColumn;                 
    ImGuiTableColumnIdx         ReorderColumn;                     
    ImGuiTableColumnIdx         ReorderColumnDir;              
    ImGuiTableColumnIdx         LeftMostStretchedColumn;         
    ImGuiTableColumnIdx         RightMostStretchedColumn;        
    ImGuiTableColumnIdx         RightMostEnabledColumn;          
    ImGuiTableColumnIdx         ContextPopupColumn;                      
    ImGuiTableColumnIdx         FreezeRowsRequest;              
    ImGuiTableColumnIdx         FreezeRowsCount;                         
    ImGuiTableColumnIdx         FreezeColumnsRequest;           
    ImGuiTableColumnIdx         FreezeColumnsCount;                      
    ImGuiTableColumnIdx         RowCellDataCurrent;                 
    ImGuiTableDrawChannelIdx    DummyDrawChannel;               
    ImGuiTableDrawChannelIdx    Bg2DrawChannelCurrent;                     
    ImGuiTableDrawChannelIdx    Bg2DrawChannelUnfrozen;
    bool                        IsLayoutLocked;                        
    bool                        IsInsideRow;                    
    bool                        IsInitializing;
    bool                        IsSortSpecsDirty;
    bool                        IsUsingHeaders;                      
    bool                        IsContextPopupOpen;                    
    bool                        IsSettingsRequestLoad;
    bool                        IsSettingsDirty;                          
    bool                        IsDefaultDisplayOrder;                 
    bool                        IsResetAllRequest;
    bool                        IsResetDisplayOrderRequest;
    bool                        IsUnfrozenRows;                     
    bool                        IsDefaultSizingPolicy;                 
    bool                        MemoryCompacted;
    bool                        HostSkipItems;                               

    IMGUI_API ImGuiTable()      { memset(this, 0, sizeof(*this)); LastFrameActive = -1; }
    IMGUI_API ~ImGuiTable()     { IM_FREE(RawData); }
};

struct ImGuiTableColumnSettings
{
    float                   WidthOrWeight;
    ImGuiID                 UserID;
    ImGuiTableColumnIdx     Index;
    ImGuiTableColumnIdx     DisplayOrder;
    ImGuiTableColumnIdx     SortOrder;
    ImU8                    SortDirection : 2;
    ImU8                    IsEnabled : 1;     
    ImU8                    IsStretch : 1;

    ImGuiTableColumnSettings()
    {
        WidthOrWeight = 0.0f;
        UserID = 0;
        Index = -1;
        DisplayOrder = SortOrder = -1;
        SortDirection = ImGuiSortDirection_None;
        IsEnabled = 1;
        IsStretch = 0;
    }
};

struct ImGuiTableSettings
{
    ImGuiID                     ID;                            
    ImGuiTableFlags             SaveFlags;                              
    float                       RefScale;                          
    ImGuiTableColumnIdx         ColumnsCount;
    ImGuiTableColumnIdx         ColumnsCountMax;                               
    bool                        WantApply;                              

    ImGuiTableSettings()        { memset(this, 0, sizeof(*this)); }
    ImGuiTableColumnSettings*   GetColumnSettings()     { return (ImGuiTableColumnSettings*)(this + 1); }
};

#endif   

namespace ImGui
{
    inline    ImGuiWindow*  GetCurrentWindowRead()      { ImGuiContext& g = *GImGui; return g.CurrentWindow; }
    inline    ImGuiWindow*  GetCurrentWindow()          { ImGuiContext& g = *GImGui; g.CurrentWindow->WriteAccessed = true; return g.CurrentWindow; }
    IMGUI_API ImGuiWindow*  FindWindowByID(ImGuiID id);
    IMGUI_API ImGuiWindow*  FindWindowByName(const char* name);
    IMGUI_API void          UpdateWindowParentAndRootLinks(ImGuiWindow* window, ImGuiWindowFlags flags, ImGuiWindow* parent_window);
    IMGUI_API ImVec2        CalcWindowNextAutoFitSize(ImGuiWindow* window);
    IMGUI_API bool          IsWindowChildOf(ImGuiWindow* window, ImGuiWindow* potential_parent);
    IMGUI_API bool          IsWindowAbove(ImGuiWindow* potential_above, ImGuiWindow* potential_below);
    IMGUI_API bool          IsWindowNavFocusable(ImGuiWindow* window);
    IMGUI_API ImRect        GetWindowAllowedExtentRect(ImGuiWindow* window);
    IMGUI_API void          SetWindowPos(ImGuiWindow* window, const ImVec2& pos, ImGuiCond cond = 0);
    IMGUI_API void          SetWindowSize(ImGuiWindow* window, const ImVec2& size, ImGuiCond cond = 0);
    IMGUI_API void          SetWindowCollapsed(ImGuiWindow* window, bool collapsed, ImGuiCond cond = 0);
    IMGUI_API void          SetWindowHitTestHole(ImGuiWindow* window, const ImVec2& pos, const ImVec2& size);

    IMGUI_API void          FocusWindow(ImGuiWindow* window);
    IMGUI_API void          FocusTopMostWindowUnderOne(ImGuiWindow* under_this_window, ImGuiWindow* ignore_window);
    IMGUI_API void          BringWindowToFocusFront(ImGuiWindow* window);
    IMGUI_API void          BringWindowToDisplayFront(ImGuiWindow* window);
    IMGUI_API void          BringWindowToDisplayBack(ImGuiWindow* window);

    IMGUI_API void          SetCurrentFont(ImFont* font);
    inline ImFont*          GetDefaultFont() { ImGuiContext& g = *GImGui; return g.IO.FontDefault ? g.IO.FontDefault : g.IO.Fonts->Fonts[0]; }
    inline ImDrawList*      GetForegroundDrawList(ImGuiWindow* window) { IM_UNUSED(window); ImGuiContext& g = *GImGui; return &g.ForegroundDrawList; }             

    IMGUI_API void          Initialize(ImGuiContext* context);
    IMGUI_API void          Shutdown(ImGuiContext* context);                      

    IMGUI_API void          UpdateHoveredWindowAndCaptureFlags();
    IMGUI_API void          StartMouseMovingWindow(ImGuiWindow* window);
    IMGUI_API void          UpdateMouseMovingWindowNewFrame();
    IMGUI_API void          UpdateMouseMovingWindowEndFrame();

    IMGUI_API ImGuiID       AddContextHook(ImGuiContext* context, const ImGuiContextHook* hook);
    IMGUI_API void          RemoveContextHook(ImGuiContext* context, ImGuiID hook_to_remove);
    IMGUI_API void          CallContextHooks(ImGuiContext* context, ImGuiContextHookType type);

    IMGUI_API void                  MarkIniSettingsDirty();
    IMGUI_API void                  MarkIniSettingsDirty(ImGuiWindow* window);
    IMGUI_API void                  ClearIniSettings();
    IMGUI_API ImGuiWindowSettings*  CreateNewWindowSettings(const char* name);
    IMGUI_API ImGuiWindowSettings*  FindWindowSettings(ImGuiID id);
    IMGUI_API ImGuiWindowSettings*  FindOrCreateWindowSettings(const char* name);
    IMGUI_API ImGuiSettingsHandler* FindSettingsHandler(const char* type_name);

    IMGUI_API void          SetNextWindowScroll(const ImVec2& scroll);         
    IMGUI_API void          SetScrollX(ImGuiWindow* window, float scroll_x);
    IMGUI_API void          SetScrollY(ImGuiWindow* window, float scroll_y);
    IMGUI_API void          SetScrollFromPosX(ImGuiWindow* window, float local_x, float center_x_ratio);
    IMGUI_API void          SetScrollFromPosY(ImGuiWindow* window, float local_y, float center_y_ratio);
    IMGUI_API ImVec2        ScrollToBringRectIntoView(ImGuiWindow* window, const ImRect& item_rect);

    inline ImGuiID          GetItemID()     { ImGuiContext& g = *GImGui; return g.CurrentWindow->DC.LastItemId; }             
    inline ImGuiItemStatusFlags GetItemStatusFlags() { ImGuiContext& g = *GImGui; return g.CurrentWindow->DC.LastItemStatusFlags; }
    inline ImGuiID          GetActiveID()   { ImGuiContext& g = *GImGui; return g.ActiveId; }
    inline ImGuiID          GetFocusID()    { ImGuiContext& g = *GImGui; return g.NavId; }
    inline ImGuiItemFlags   GetItemsFlags() { ImGuiContext& g = *GImGui; return g.CurrentWindow->DC.ItemFlags; }
    IMGUI_API void          SetActiveID(ImGuiID id, ImGuiWindow* window);
    IMGUI_API void          SetFocusID(ImGuiID id, ImGuiWindow* window);
    IMGUI_API void          ClearActiveID();
    IMGUI_API ImGuiID       GetHoveredID();
    IMGUI_API void          SetHoveredID(ImGuiID id);
    IMGUI_API void          KeepAliveID(ImGuiID id);
    IMGUI_API void          MarkItemEdited(ImGuiID id);                 
    IMGUI_API void          PushOverrideID(ImGuiID id);                       
    IMGUI_API ImGuiID       GetIDWithSeed(const char* str_id_begin, const char* str_id_end, ImGuiID seed);

    IMGUI_API void          ItemSize(const ImVec2& size, float text_baseline_y = -1.0f);
    IMGUI_API void          ItemSize(const ImRect& bb, float text_baseline_y = -1.0f);
    IMGUI_API bool          ItemAdd(const ImRect& bb, ImGuiID id, const ImRect* nav_bb = NULL);
    IMGUI_API bool          ItemHoverable(const ImRect& bb, ImGuiID id);
    IMGUI_API bool          IsClippedEx(const ImRect& bb, ImGuiID id, bool clip_even_when_logged);
    IMGUI_API void          SetLastItemData(ImGuiWindow* window, ImGuiID item_id, ImGuiItemStatusFlags status_flags, const ImRect& item_rect);
    IMGUI_API bool          FocusableItemRegister(ImGuiWindow* window, ImGuiID id);         
    IMGUI_API void          FocusableItemUnregister(ImGuiWindow* window);
    IMGUI_API ImVec2        CalcItemSize(ImVec2 size, float default_w, float default_h);
    IMGUI_API float         CalcWrapWidthForPos(const ImVec2& pos, float wrap_pos_x);
    IMGUI_API void          PushMultiItemsWidths(int components, float width_full);
    IMGUI_API void          PushItemFlag(ImGuiItemFlags option, bool enabled);
    IMGUI_API void          PopItemFlag();
    IMGUI_API bool          IsItemToggledSelection();                                                        
    IMGUI_API ImVec2        GetContentRegionMaxAbs();
    IMGUI_API void          ShrinkWidths(ImGuiShrinkWidthItem* items, int count, float width_excess);

    IMGUI_API void          LogBegin(ImGuiLogType type, int auto_open_depth);                             
    IMGUI_API void          LogToBuffer(int auto_open_depth = -1);                           
    IMGUI_API void          LogRenderedText(const ImVec2* ref_pos, const char* text, const char* text_end = NULL);
    IMGUI_API void          LogSetNextTextDecoration(const char* prefix, const char* suffix);

    IMGUI_API bool          BeginChildEx(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags);
    IMGUI_API void          OpenPopupEx(ImGuiID id, ImGuiPopupFlags popup_flags = ImGuiPopupFlags_None);
    IMGUI_API void          ClosePopupToLevel(int remaining, bool restore_focus_to_window_under_popup);
    IMGUI_API void          ClosePopupsOverWindow(ImGuiWindow* ref_window, bool restore_focus_to_window_under_popup);
    IMGUI_API bool          IsPopupOpen(ImGuiID id, ImGuiPopupFlags popup_flags);
    IMGUI_API bool          BeginPopupEx(ImGuiID id, ImGuiWindowFlags extra_flags);
    IMGUI_API void          BeginTooltipEx(ImGuiWindowFlags extra_flags, ImGuiTooltipFlags tooltip_flags);
    IMGUI_API ImGuiWindow*  GetTopMostPopupModal();
    IMGUI_API ImVec2        FindBestWindowPosForPopup(ImGuiWindow* window);
    IMGUI_API ImVec2        FindBestWindowPosForPopupEx(const ImVec2& ref_pos, const ImVec2& size, ImGuiDir* last_dir, const ImRect& r_outer, const ImRect& r_avoid, ImGuiPopupPositionPolicy policy);

    IMGUI_API void          NavInitWindow(ImGuiWindow* window, bool force_reinit);
    IMGUI_API bool          NavMoveRequestButNoResultYet();
    IMGUI_API void          NavMoveRequestCancel();
    IMGUI_API void          NavMoveRequestForward(ImGuiDir move_dir, ImGuiDir clip_dir, const ImRect& bb_rel, ImGuiNavMoveFlags move_flags);
    IMGUI_API void          NavMoveRequestTryWrapping(ImGuiWindow* window, ImGuiNavMoveFlags move_flags);
    IMGUI_API float         GetNavInputAmount(ImGuiNavInput n, ImGuiInputReadMode mode);
    IMGUI_API ImVec2        GetNavInputAmount2d(ImGuiNavDirSourceFlags dir_sources, ImGuiInputReadMode mode, float slow_factor = 0.0f, float fast_factor = 0.0f);
    IMGUI_API int           CalcTypematicRepeatAmount(float t0, float t1, float repeat_delay, float repeat_rate);
    IMGUI_API void          ActivateItem(ImGuiID id);                              
    IMGUI_API void          SetNavID(ImGuiID id, int nav_layer, ImGuiID focus_scope_id);
    IMGUI_API void          SetNavIDWithRectRel(ImGuiID id, int nav_layer, ImGuiID focus_scope_id, const ImRect& rect_rel);

    IMGUI_API void          PushFocusScope(ImGuiID id);
    IMGUI_API void          PopFocusScope();
    inline ImGuiID          GetFocusedFocusScope()          { ImGuiContext& g = *GImGui; return g.NavFocusScopeId; }                                  
    inline ImGuiID          GetFocusScope()                 { ImGuiContext& g = *GImGui; return g.CurrentWindow->DC.NavFocusScopeIdCurrent; }            

    IMGUI_API void          SetItemUsingMouseWheel();
    inline bool             IsActiveIdUsingNavDir(ImGuiDir dir)                         { ImGuiContext& g = *GImGui; return (g.ActiveIdUsingNavDirMask & (1 << dir)) != 0; }
    inline bool             IsActiveIdUsingNavInput(ImGuiNavInput input)                { ImGuiContext& g = *GImGui; return (g.ActiveIdUsingNavInputMask & (1 << input)) != 0; }
    inline bool             IsActiveIdUsingKey(ImGuiKey key)                            { ImGuiContext& g = *GImGui; IM_ASSERT(key < 64); return (g.ActiveIdUsingKeyInputMask & ((ImU64)1 << key)) != 0; }
    IMGUI_API bool          IsMouseDragPastThreshold(ImGuiMouseButton button, float lock_threshold = -1.0f);
    inline bool             IsKeyPressedMap(ImGuiKey key, bool repeat = true)           { ImGuiContext& g = *GImGui; const int key_index = g.IO.KeyMap[key]; return (key_index >= 0) ? IsKeyPressed(key_index, repeat) : false; }
    inline bool             IsNavInputDown(ImGuiNavInput n)                             { ImGuiContext& g = *GImGui; return g.IO.NavInputs[n] > 0.0f; }
    inline bool             IsNavInputTest(ImGuiNavInput n, ImGuiInputReadMode rm)      { return (GetNavInputAmount(n, rm) > 0.0f); }
    IMGUI_API ImGuiKeyModFlags GetMergedKeyModFlags();

    IMGUI_API bool          BeginDragDropTargetCustom(const ImRect& bb, ImGuiID id);
    IMGUI_API void          ClearDragDrop();
    IMGUI_API bool          IsDragDropPayloadBeingAccepted();

    IMGUI_API void          SetWindowClipRectBeforeSetChannel(ImGuiWindow* window, const ImRect& clip_rect);
    IMGUI_API void          BeginColumns(const char* str_id, int count, ImGuiOldColumnFlags flags = 0);                
    IMGUI_API void          EndColumns();                                                                 
    IMGUI_API void          PushColumnClipRect(int column_index);
    IMGUI_API void          PushColumnsBackground();
    IMGUI_API void          PopColumnsBackground();
    IMGUI_API ImGuiID       GetColumnsID(const char* str_id, int count);
    IMGUI_API ImGuiOldColumns* FindOrCreateColumns(ImGuiWindow* window, ImGuiID id);
    IMGUI_API float         GetColumnOffsetFromNorm(const ImGuiOldColumns* columns, float offset_norm);
    IMGUI_API float         GetColumnNormFromOffset(const ImGuiOldColumns* columns, float offset);

    IMGUI_API void          TableOpenContextMenu(int column_n = -1);
    IMGUI_API void          TableSetColumnEnabled(int column_n, bool enabled);
    IMGUI_API void          TableSetColumnWidth(int column_n, float width);
    IMGUI_API void          TableSetColumnSortDirection(int column_n, ImGuiSortDirection sort_direction, bool append_to_sort_specs);
    IMGUI_API int           TableGetHoveredColumn();                               
    IMGUI_API float         TableGetHeaderRowHeight();
    IMGUI_API void          TablePushBackgroundChannel();
    IMGUI_API void          TablePopBackgroundChannel();

    IMGUI_API ImGuiTable*   TableFindByID(ImGuiID id);
    IMGUI_API bool          BeginTableEx(const char* name, ImGuiID id, int columns_count, ImGuiTableFlags flags = 0, const ImVec2& outer_size = ImVec2(0, 0), float inner_width = 0.0f);
    IMGUI_API void          TableBeginInitMemory(ImGuiTable* table, int columns_count);
    IMGUI_API void          TableBeginApplyRequests(ImGuiTable* table);
    IMGUI_API void          TableSetupDrawChannels(ImGuiTable* table);
    IMGUI_API void          TableUpdateLayout(ImGuiTable* table);
    IMGUI_API void          TableUpdateBorders(ImGuiTable* table);
    IMGUI_API void          TableUpdateColumnsWeightFromWidth(ImGuiTable* table);
    IMGUI_API void          TableDrawBorders(ImGuiTable* table);
    IMGUI_API void          TableDrawContextMenu(ImGuiTable* table);
    IMGUI_API void          TableMergeDrawChannels(ImGuiTable* table);
    IMGUI_API void          TableSortSpecsSanitize(ImGuiTable* table);
    IMGUI_API void          TableSortSpecsBuild(ImGuiTable* table);
    IMGUI_API ImGuiSortDirection TableGetColumnNextSortDirection(ImGuiTableColumn* column);
    IMGUI_API void          TableFixColumnSortDirection(ImGuiTable* table, ImGuiTableColumn* column);
    IMGUI_API float         TableGetColumnWidthAuto(ImGuiTable* table, ImGuiTableColumn* column);
    IMGUI_API void          TableBeginRow(ImGuiTable* table);
    IMGUI_API void          TableEndRow(ImGuiTable* table);
    IMGUI_API void          TableBeginCell(ImGuiTable* table, int column_n);
    IMGUI_API void          TableEndCell(ImGuiTable* table);
    IMGUI_API ImRect        TableGetCellBgRect(const ImGuiTable* table, int column_n);
    IMGUI_API const char*   TableGetColumnName(const ImGuiTable* table, int column_n);
    IMGUI_API ImGuiID       TableGetColumnResizeID(const ImGuiTable* table, int column_n, int instance_no = 0);
    IMGUI_API float         TableGetMaxColumnWidth(const ImGuiTable* table, int column_n);
    IMGUI_API void          TableSetColumnWidthAutoSingle(ImGuiTable* table, int column_n);
    IMGUI_API void          TableSetColumnWidthAutoAll(ImGuiTable* table);
    IMGUI_API void          TableRemove(ImGuiTable* table);
    IMGUI_API void          TableGcCompactTransientBuffers(ImGuiTable* table);
    IMGUI_API void          TableGcCompactSettings();

    IMGUI_API void                  TableLoadSettings(ImGuiTable* table);
    IMGUI_API void                  TableSaveSettings(ImGuiTable* table);
    IMGUI_API void                  TableResetSettings(ImGuiTable* table);
    IMGUI_API ImGuiTableSettings*   TableGetBoundSettings(ImGuiTable* table);
    IMGUI_API void                  TableSettingsInstallHandler(ImGuiContext* context);
    IMGUI_API ImGuiTableSettings*   TableSettingsCreate(ImGuiID id, int columns_count);
    IMGUI_API ImGuiTableSettings*   TableSettingsFindByID(ImGuiID id);

    IMGUI_API bool          BeginTabBarEx(ImGuiTabBar* tab_bar, const ImRect& bb, ImGuiTabBarFlags flags);
    IMGUI_API ImGuiTabItem* TabBarFindTabByID(ImGuiTabBar* tab_bar, ImGuiID tab_id);
    IMGUI_API void          TabBarRemoveTab(ImGuiTabBar* tab_bar, ImGuiID tab_id);
    IMGUI_API void          TabBarCloseTab(ImGuiTabBar* tab_bar, ImGuiTabItem* tab);
    IMGUI_API void          TabBarQueueReorder(ImGuiTabBar* tab_bar, const ImGuiTabItem* tab, int dir);
    IMGUI_API bool          TabBarProcessReorder(ImGuiTabBar* tab_bar);
    IMGUI_API bool          TabItemEx(ImGuiTabBar* tab_bar, const char* label, bool* p_open, ImGuiTabItemFlags flags);
    IMGUI_API ImVec2        TabItemCalcSize(const char* label, bool has_close_button);
    IMGUI_API void          TabItemBackground(ImDrawList* draw_list, const ImRect& bb, ImGuiTabItemFlags flags, ImU32 col);
    IMGUI_API void          TabItemLabelAndCloseButton(ImDrawList* draw_list, const ImRect& bb, ImGuiTabItemFlags flags, ImVec2 frame_padding, const char* label, ImGuiID tab_id, ImGuiID close_button_id, bool is_contents_visible, bool* out_just_closed, bool* out_text_clipped, bool tabActive = 1);

    IMGUI_API void          RenderText(ImVec2 pos, const char* text, const char* text_end = NULL, bool hide_text_after_hash = true, ImU32 customcol = 0);
    IMGUI_API void          RenderTextWrapped(ImVec2 pos, const char* text, const char* text_end, float wrap_width);
    IMGUI_API void          RenderTextClipped(const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align = ImVec2(0, 0), const ImRect* clip_rect = NULL);
    IMGUI_API void          RenderTextClipped(const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align, const ImRect* clip_rect, ImColor customcolor);

    IMGUI_API void          RenderTextClippedEx(ImDrawList* draw_list, const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align = ImVec2(0, 0), const ImRect* clip_rect = NULL, ImU32 Color = 0);
    IMGUI_API void          RenderTextEllipsis(ImDrawList* draw_list, const ImVec2& pos_min, const ImVec2& pos_max, float clip_max_x, float ellipsis_max_x, const char* text, const char* text_end, const ImVec2* text_size_if_known, ImU32 Color = 0);
    IMGUI_API void          RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border = true, float rounding = 0.0f);
    IMGUI_API void          RenderFrameBorder(ImVec2 p_min, ImVec2 p_max, float rounding = 0.0f);
    IMGUI_API void          RenderColorRectWithAlphaCheckerboard(ImDrawList* draw_list, ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, float grid_step, ImVec2 grid_off, float rounding = 0.0f, int rounding_corners_flags = ~0);
    IMGUI_API void          RenderNavHighlight(const ImRect& bb, ImGuiID id, ImGuiNavHighlightFlags flags = ImGuiNavHighlightFlags_TypeDefault);   
    IMGUI_API const char*   FindRenderedTextEnd(const char* text, const char* text_end = NULL);           

    IMGUI_API void          RenderArrow(ImDrawList* draw_list, ImVec2 pos, ImU32 col, ImGuiDir dir, float scale = 1.0f);
    IMGUI_API void          RenderBullet(ImDrawList* draw_list, ImVec2 pos, ImU32 col);
    IMGUI_API void          RenderCheckMark(ImDrawList* draw_list, ImVec2 pos, ImU32 col, float sz);
    IMGUI_API void          RenderMouseCursor(ImDrawList* draw_list, ImVec2 pos, float scale, ImGuiMouseCursor mouse_cursor, ImU32 col_fill, ImU32 col_border, ImU32 col_shadow);
    IMGUI_API void          RenderArrowPointingAt(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, ImGuiDir direction, ImU32 col);
    IMGUI_API void          RenderRectFilledRangeH(ImDrawList* draw_list, const ImRect& rect, ImU32 col, float x_start_norm, float x_end_norm, float rounding);
    IMGUI_API void          RenderRectFilledWithHole(ImDrawList* draw_list, ImRect outer, ImRect inner, ImU32 col, float rounding);

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    inline void RenderArrow(ImVec2 pos, ImGuiDir dir, float scale=1.0f) { ImGuiWindow* window = GetCurrentWindow(); RenderArrow(window->DrawList, pos, GetColorU32(ImGuiCol_Text), dir, scale); }
    inline void RenderBullet(ImVec2 pos)                                { ImGuiWindow* window = GetCurrentWindow(); RenderBullet(window->DrawList, pos, GetColorU32(ImGuiCol_Text)); }
#endif

    IMGUI_API void          TextEx(const char* text, const char* text_end = NULL, ImGuiTextFlags flags = 0);
    IMGUI_API bool          ButtonEx(const char* label, const ImVec2& size_arg = ImVec2(0, 0), ImGuiButtonFlags flags = 0);
    IMGUI_API bool          CloseButton(ImGuiID id, const ImVec2& pos);
    IMGUI_API bool          CollapseButton(ImGuiID id, const ImVec2& pos);
    IMGUI_API bool          ArrowButtonEx(const char* str_id, ImGuiDir dir, ImVec2 size_arg, ImGuiButtonFlags flags = 0);
    IMGUI_API void          Scrollbar(ImGuiAxis axis);
    IMGUI_API bool          ScrollbarEx(const ImRect& bb, ImGuiID id, ImGuiAxis axis, float* p_scroll_v, float avail_v, float contents_v, ImDrawCornerFlags rounding_corners);
    IMGUI_API bool          ImageButtonEx(ImGuiID id, ImTextureID texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec2& padding, const ImVec4& bg_col, const ImVec4& tint_col);
    IMGUI_API ImRect        GetWindowScrollbarRect(ImGuiWindow* window, ImGuiAxis axis);
    IMGUI_API ImGuiID       GetWindowScrollbarID(ImGuiWindow* window, ImGuiAxis axis);
    IMGUI_API ImGuiID       GetWindowResizeID(ImGuiWindow* window, int n);     
    IMGUI_API void          SeparatorEx(ImGuiSeparatorFlags flags);
    IMGUI_API bool          CheckboxFlags(const char* label, ImS64* flags, ImS64 flags_value);
    IMGUI_API bool          CheckboxFlags(const char* label, ImU64* flags, ImU64 flags_value);

    IMGUI_API bool          ButtonBehavior(const ImRect& bb, ImGuiID id, bool* out_hovered, bool* out_held, ImGuiButtonFlags flags = 0);
    IMGUI_API bool          DragBehavior(ImGuiID id, ImGuiDataType data_type, void* p_v, float v_speed, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags);
    IMGUI_API bool          SliderBehavior(const ImRect& bb, ImGuiID id, ImGuiDataType data_type, void* p_v, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags, ImRect* out_grab_bb);
    IMGUI_API bool          SplitterBehavior(const ImRect& bb, ImGuiID id, ImGuiAxis axis, float* size1, float* size2, float min_size1, float min_size2, float hover_extend = 0.0f, float hover_visibility_delay = 0.0f);
    IMGUI_API bool          TreeNodeBehavior(ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end = NULL);
    IMGUI_API bool          TreeNodeBehaviorIsOpen(ImGuiID id, ImGuiTreeNodeFlags flags = 0);                                
    IMGUI_API void          TreePushOverrideID(ImGuiID id);

    template<typename T, typename SIGNED_T, typename FLOAT_T>   IMGUI_API float ScaleRatioFromValueT(ImGuiDataType data_type, T v, T v_min, T v_max, bool is_logarithmic, float logarithmic_zero_epsilon, float zero_deadzone_size);
    template<typename T, typename SIGNED_T, typename FLOAT_T>   IMGUI_API T     ScaleValueFromRatioT(ImGuiDataType data_type, float t, T v_min, T v_max, bool is_logarithmic, float logarithmic_zero_epsilon, float zero_deadzone_size);
    template<typename T, typename SIGNED_T, typename FLOAT_T>   IMGUI_API bool  DragBehaviorT(ImGuiDataType data_type, T* v, float v_speed, T v_min, T v_max, const char* format, ImGuiSliderFlags flags);
    template<typename T, typename SIGNED_T, typename FLOAT_T>   IMGUI_API bool  SliderBehaviorT(const ImRect& bb, ImGuiID id, ImGuiDataType data_type, T* v, T v_min, T v_max, const char* format, ImGuiSliderFlags flags, ImRect* out_grab_bb);
    template<typename T, typename SIGNED_T>                     IMGUI_API T     RoundScalarWithFormatT(const char* format, ImGuiDataType data_type, T v);
    template<typename T>                                        IMGUI_API bool  CheckboxFlagsT(const char* label, T* flags, T flags_value);

    IMGUI_API const ImGuiDataTypeInfo*  DataTypeGetInfo(ImGuiDataType data_type);
    IMGUI_API int           DataTypeFormatString(char* buf, int buf_size, ImGuiDataType data_type, const void* p_data, const char* format);
    IMGUI_API void          DataTypeApplyOp(ImGuiDataType data_type, int op, void* output, const void* arg_1, const void* arg_2);
    IMGUI_API bool          DataTypeApplyOpFromText(const char* buf, const char* initial_value_buf, ImGuiDataType data_type, void* p_data, const char* format);
    IMGUI_API int           DataTypeCompare(ImGuiDataType data_type, const void* arg_1, const void* arg_2);
    IMGUI_API bool          DataTypeClamp(ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max);

    IMGUI_API bool          InputTextEx(const char* label, const char* hint, char* buf, int buf_size, const ImVec2& size_arg, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    IMGUI_API bool          TempInputText(const ImRect& bb, ImGuiID id, const char* label, char* buf, int buf_size, ImGuiInputTextFlags flags);
    IMGUI_API bool          TempInputScalar(const ImRect& bb, ImGuiID id, const char* label, ImGuiDataType data_type, void* p_data, const char* format, const void* p_clamp_min = NULL, const void* p_clamp_max = NULL);
    inline bool             TempInputIsActive(ImGuiID id)       { ImGuiContext& g = *GImGui; return (g.ActiveId == id && g.TempInputId == id); }
    inline ImGuiInputTextState* GetInputTextState(ImGuiID id)   { ImGuiContext& g = *GImGui; return (g.InputTextState.ID == id) ? &g.InputTextState : NULL; }       

    IMGUI_API void          ColorTooltip(const char* text, const float* col, ImGuiColorEditFlags flags);
    IMGUI_API void          ColorEditOptionsPopup(const float* col, ImGuiColorEditFlags flags);
    IMGUI_API void          ColorPickerOptionsPopup(const float* ref_col, ImGuiColorEditFlags flags);

    IMGUI_API int           PlotEx(ImGuiPlotType plot_type, const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 frame_size);

    IMGUI_API void          ShadeVertsLinearColorGradientKeepAlpha(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, ImVec2 gradient_p0, ImVec2 gradient_p1, ImU32 col0, ImU32 col1);
    IMGUI_API void          ShadeVertsLinearUV(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, bool clamp);

    IMGUI_API void          GcCompactTransientMiscBuffers();
    IMGUI_API void          GcCompactTransientWindowBuffers(ImGuiWindow* window);
    IMGUI_API void          GcAwakeTransientWindowBuffers(ImGuiWindow* window);

    IMGUI_API void          ErrorCheckEndFrameRecover(ImGuiErrorLogCallback log_callback, void* user_data = NULL);
    inline void             DebugDrawItemRect(ImU32 col = IM_COL32(255,0,0,255))    { ImGuiContext& g = *GImGui; ImGuiWindow* window = g.CurrentWindow; GetForegroundDrawList(window)->AddRect(window->DC.LastItemRect.Min, window->DC.LastItemRect.Max, col); }
    inline void             DebugStartItemPicker()                                  { ImGuiContext& g = *GImGui; g.DebugItemPickerActive = true; }

    IMGUI_API void          DebugNodeColumns(ImGuiOldColumns* columns);
    IMGUI_API void          DebugNodeDrawList(ImGuiWindow* window, const ImDrawList* draw_list, const char* label);
    IMGUI_API void          DebugNodeDrawCmdShowMeshAndBoundingBox(ImGuiWindow* window, const ImDrawList* draw_list, const ImDrawCmd* draw_cmd, bool show_mesh, bool show_aabb);
    IMGUI_API void          DebugNodeStorage(ImGuiStorage* storage, const char* label);
    IMGUI_API void          DebugNodeTabBar(ImGuiTabBar* tab_bar, const char* label);
    IMGUI_API void          DebugNodeTable(ImGuiTable* table);
    IMGUI_API void          DebugNodeTableSettings(ImGuiTableSettings* settings);
    IMGUI_API void          DebugNodeWindow(ImGuiWindow* window, const char* label);
    IMGUI_API void          DebugNodeWindowSettings(ImGuiWindowSettings* settings);
    IMGUI_API void          DebugNodeWindowsList(ImVector<ImGuiWindow*>* windows, const char* label);

}   


struct ImFontBuilderIO
{
    bool    (*FontBuilder_Build)(ImFontAtlas* atlas);
};

IMGUI_API const ImFontBuilderIO* ImFontAtlasGetBuilderForStbTruetype();
IMGUI_API void      ImFontAtlasBuildInit(ImFontAtlas* atlas);
IMGUI_API void      ImFontAtlasBuildSetupFont(ImFontAtlas* atlas, ImFont* font, ImFontConfig* font_config, float ascent, float descent);
IMGUI_API void      ImFontAtlasBuildPackCustomRects(ImFontAtlas* atlas, void* stbrp_context_opaque);
IMGUI_API void      ImFontAtlasBuildFinish(ImFontAtlas* atlas);
IMGUI_API void      ImFontAtlasBuildRender8bppRectFromString(ImFontAtlas* atlas, int x, int y, int w, int h, const char* in_str, char in_marker_char, unsigned char in_marker_pixel_value);
IMGUI_API void      ImFontAtlasBuildRender32bppRectFromString(ImFontAtlas* atlas, int x, int y, int w, int h, const char* in_str, char in_marker_char, unsigned int in_marker_pixel_value);
IMGUI_API void      ImFontAtlasBuildMultiplyCalcLookupTable(unsigned char out_table[256], float in_multiply_factor);
IMGUI_API void      ImFontAtlasBuildMultiplyRectAlpha8(const unsigned char table[256], unsigned char* pixels, int x, int y, int w, int h, int stride);

#ifdef IMGUI_ENABLE_TEST_ENGINE
extern void                 ImGuiTestEngineHook_ItemAdd(ImGuiContext* ctx, const ImRect& bb, ImGuiID id);
extern void                 ImGuiTestEngineHook_ItemInfo(ImGuiContext* ctx, ImGuiID id, const char* label, ImGuiItemStatusFlags flags);
extern void                 ImGuiTestEngineHook_IdInfo(ImGuiContext* ctx, ImGuiDataType data_type, ImGuiID id, const void* data_id);
extern void                 ImGuiTestEngineHook_IdInfo(ImGuiContext* ctx, ImGuiDataType data_type, ImGuiID id, const void* data_id, const void* data_id_end);
extern void                 ImGuiTestEngineHook_Log(ImGuiContext* ctx, const char* fmt, ...);
#define IMGUI_TEST_ENGINE_ITEM_ADD(_BB,_ID)                 if (g.TestEngineHookItems) ImGuiTestEngineHook_ItemAdd(&g, _BB, _ID)                   
#define IMGUI_TEST_ENGINE_ITEM_INFO(_ID,_LABEL,_FLAGS)      if (g.TestEngineHookItems) ImGuiTestEngineHook_ItemInfo(&g, _ID, _LABEL, _FLAGS)          
#define IMGUI_TEST_ENGINE_LOG(_FMT,...)                     if (g.TestEngineHookItems) ImGuiTestEngineHook_Log(&g, _FMT, __VA_ARGS__)                   
#define IMGUI_TEST_ENGINE_ID_INFO(_ID,_TYPE,_DATA)          if (g.TestEngineHookIdInfo == id) ImGuiTestEngineHook_IdInfo(&g, _TYPE, _ID, (const void*)(_DATA));
#define IMGUI_TEST_ENGINE_ID_INFO2(_ID,_TYPE,_DATA,_DATA2)  if (g.TestEngineHookIdInfo == id) ImGuiTestEngineHook_IdInfo(&g, _TYPE, _ID, (const void*)(_DATA), (const void*)(_DATA2));
#else
#define IMGUI_TEST_ENGINE_ITEM_ADD(_BB,_ID)                 do { } while (0)
#define IMGUI_TEST_ENGINE_ITEM_INFO(_ID,_LABEL,_FLAGS)      do { } while (0)
#define IMGUI_TEST_ENGINE_LOG(_FMT,...)                     do { } while (0)
#define IMGUI_TEST_ENGINE_ID_INFO(_ID,_TYPE,_DATA)          do { } while (0)
#define IMGUI_TEST_ENGINE_ID_INFO2(_ID,_TYPE,_DATA,_DATA2)  do { } while (0)
#endif

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning (pop)
#endif

#endif   
