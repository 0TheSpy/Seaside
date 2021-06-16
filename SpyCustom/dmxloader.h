#ifndef DMXLOADER_H
#define DMXLOADER_H

#ifdef _WIN32
#pragma once
#endif

class CUtlBuffer;
class CDmxElement;


bool SerializeDMX(CUtlBuffer& buf, CDmxElement* pRoot, const char* pFileName = NULL);
bool SerializeDMX(const char* pFileName, const char* pPathID, bool bTextMode, CDmxElement* pRoot);

bool UnserializeDMX(CUtlBuffer& buf, CDmxElement** ppRoot, const char* pFileName = NULL);
bool UnserializeDMX(const char* pFileName, const char* pPathID, bool bTextMode, CDmxElement** ppRoot);

void BeginDMXContext();
void EndDMXContext(bool bDecommitMemory);
void DecommitDMXMemory();


class CDMXContextHelper
{
public:
	CDMXContextHelper(bool bDecommitMemory) { m_bDecommitMemory = bDecommitMemory; BeginDMXContext(); }
	~CDMXContextHelper() { EndDMXContext(m_bDecommitMemory); }

private:
	bool m_bDecommitMemory;
};

#define DECLARE_DMX_CONTEXT( )	CDMXContextHelper __dmxContextHelper( true );
#define DECLARE_DMX_CONTEXT_NODECOMMIT( )	CDMXContextHelper __dmxContextHelper( false );
#define DECLARE_DMX_CONTEXT_DECOMMIT( _decommit )	CDMXContextHelper __dmxContextHelper( _decommit );


void* DMXAlloc(size_t size);


#define DECLARE_DMX_ALLOCATOR( )												\
	public:																		\
		inline void* operator new( size_t size ) { MEM_ALLOC_CREDIT_( "DMXAlloc" ); return DMXAlloc(size); }   \
		inline void* operator new( size_t size, int nBlockUse, const char *pFileName, int nLine ) { MEM_ALLOC_CREDIT_( "DMXAlloc" ); return DMXAlloc(size); }   \
		inline void  operator delete( void* p ) { }		\
		inline void  operator delete( void* p, int nBlockUse, const char *pFileName, int nLine ) { }   \

#endif  