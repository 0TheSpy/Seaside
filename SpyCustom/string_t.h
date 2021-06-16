#ifndef STRING_T_H
#define STRING_T_H

#if defined( _WIN32 )
#pragma once
#endif

#ifndef NO_STRING_T

#ifdef WEAK_STRING_T

typedef int	string_t;

#define NULL_STRING			0

#define STRING( offset )	( ( offset ) ? reinterpret_cast<const char *>( offset ) : "" )

#define MAKE_STRING( str )	( ( *str != 0 ) ? reinterpret_cast<int>( str ) : 0 )

#define IDENT_STRINGS( s1, s2 )	( *((void **)&(s1)) == *((void **)&(s2)) )

#else   

struct string_t
{
public:
	bool operator!() const { return (pszValue == NULL); }
	bool operator==(const string_t& rhs) const { return (pszValue == rhs.pszValue); }
	bool operator!=(const string_t& rhs) const { return (pszValue != rhs.pszValue); }
	bool operator<(const string_t& rhs) const { return ((void*)pszValue < (void*)rhs.pszValue); }

	const char* ToCStr() const { return (pszValue) ? pszValue : ""; }

protected:
	const char* pszValue;
};

struct castable_string_t : public string_t          
{
	castable_string_t() { pszValue = NULL; }
	castable_string_t(const char* pszFrom) { pszValue = (pszFrom && *pszFrom) ? pszFrom : 0; }
};

#define NULL_STRING			castable_string_t()

#define STRING( string_t_obj )	(string_t_obj).ToCStr()

#define MAKE_STRING( c_str )	castable_string_t( c_str )

#define IDENT_STRINGS( s1, s2 )	( *((void **)&(s1)) == *((void **)&(s2)) )

#endif

#else	 

typedef const char* string_t;
#define NULL_STRING				0
#define STRING( c_str )			( c_str )
#define MAKE_STRING( c_str )	( c_str )
#define IDENT_STRINGS( s1, s2 )	( *((void **)&(s1)) == *((void **)&(s2)) )

#endif	 

inline void EnsureValidValue(string_t& x) { x = NULL_STRING; }

#endif  
