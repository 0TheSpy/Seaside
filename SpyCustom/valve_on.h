
#ifdef STEAM
#ifdef ENFORCE_WCHAR
#define char DontUseChar_SeeWcharOn.h
#endif


#define malloc( cub ) HEY_DONT_USE_MALLOC_USE_PVALLOC
#define realloc( pvOld, cub ) HEY_DONT_USE_REALLOC_USE_PVREALLOC
#define _expand( pvOld, cub ) HEY_DONT_USE_EXPAND_USE_PVEXPAND
#define free( pv ) HEY_DONT_USE_FREE_USE_FREEPV

#endif