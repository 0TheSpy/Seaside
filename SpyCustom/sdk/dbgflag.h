#ifndef DBGFLAG_H
#define DBGFLAG_H
#ifdef _WIN32
#pragma once
#endif


#undef DBGFLAG_MEMORY
#undef DBGFLAG_MEMORY_NEWDEL
#undef DBGFLAG_VALIDATE
#undef DBGFLAG_ASSERT
#undef DBGFLAG_ASSERTFATAL
#undef DBGFLAG_ASSERTDLG
#undef DBGFLAG_STRINGS

#if defined( _DEBUG ) && !defined( PS3MEMOVERRIDEWRAP )

#define DBGFLAG_MEMORY
#ifdef _SERVER	                 
#define DBGFLAG_MEMORY_NEWDEL	
#endif
#ifdef STEAM
#define DBGFLAG_VALIDATE
#endif
#define DBGFLAG_ASSERT
#define DBGFLAG_ASSERTFATAL
#define DBGFLAG_ASSERTDLG
#define DBGFLAG_STRINGS


#else  
#ifdef STEAM
#define DBGFLAG_ASSERT
#endif
#define DBGFLAG_ASSERTFATAL		        
#define DBGFLAG_ASSERTDLG

#endif  

#if defined( _CERT )
#define DBGFLAG_STRINGS_STRIP
#endif

#endif  