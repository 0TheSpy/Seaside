#ifndef UTLHANDLETABLE_H
#define	UTLHANDLETABLE_H

#ifdef _WIN32
#pragma once
#endif


#include "utlvector.h"
#include "utlqueue.h"


typedef unsigned int UtlHandle_t;
#define UTLHANDLE_INVALID ((UtlHandle_t)~0)


template< class T, int HandleBits >
class CUtlHandleTable
{
public:
	CUtlHandleTable();

	UtlHandle_t AddHandle();
	void RemoveHandle(UtlHandle_t h);

	void SetHandle(UtlHandle_t h, T* pData);
	T* GetHandle(UtlHandle_t h) const;
	T* GetHandle(UtlHandle_t h, bool checkValidity) const;

	bool IsHandleValid(UtlHandle_t h) const;

	unsigned int GetValidHandleCount() const;
	unsigned int GetHandleCount() const;
	UtlHandle_t GetHandleFromIndex(int i) const;
	int GetIndexFromHandle(UtlHandle_t h) const;

	void MarkHandleInvalid(UtlHandle_t h);
	void MarkHandleValid(UtlHandle_t h);

private:
	struct HandleType_t
	{
		HandleType_t(unsigned int i, unsigned int s) : nIndex(i), nSerial(s)
		{
			Assert(i < (1 << HandleBits));
			Assert(s < (1 << (31 - HandleBits)));
		}
		unsigned int nIndex : HandleBits;
		unsigned int nSerial : 31 - HandleBits;
	};

	struct EntryType_t
	{
		EntryType_t() : m_nSerial(0), nInvalid(0), m_pData(0) {}
		unsigned int m_nSerial : 31;
		unsigned int nInvalid : 1;
		T* m_pData;
	};

	static unsigned int GetSerialNumber(UtlHandle_t handle);
	static unsigned int GetListIndex(UtlHandle_t handle);
	static UtlHandle_t CreateHandle(unsigned int nSerial, unsigned int nIndex);
	const EntryType_t* GetEntry(UtlHandle_t handle, bool checkValidity) const;

	unsigned int m_nValidHandles;
	CUtlVector< EntryType_t > m_list;
	CUtlQueue< int > m_unused;
};


template< class T, int HandleBits >
CUtlHandleTable<T, HandleBits>::CUtlHandleTable() : m_nValidHandles(0)
{
}


template< class T, int HandleBits >
UtlHandle_t CUtlHandleTable<T, HandleBits>::AddHandle()
{
	unsigned int nIndex = (m_unused.Count() > 0) ? m_unused.RemoveAtHead() : m_list.AddToTail();

	EntryType_t& entry = m_list[nIndex];
	entry.nInvalid = 0;
	entry.m_pData = NULL;

	++m_nValidHandles;

	return CreateHandle(entry.m_nSerial, nIndex);
}

template< class T, int HandleBits >
void CUtlHandleTable<T, HandleBits>::RemoveHandle(UtlHandle_t handle)
{
	unsigned int nIndex = GetListIndex(handle);
	Assert(nIndex < (unsigned int)m_list.Count());
	if (nIndex >= (unsigned int)m_list.Count())
		return;

	EntryType_t& entry = m_list[nIndex];
	++entry.m_nSerial;     
	if (!entry.nInvalid)
	{
		entry.nInvalid = 1;
		--m_nValidHandles;
	}
	entry.m_pData = NULL;


	bool bStopUsing = (entry.m_nSerial >= ((1 << (31 - HandleBits)) - 1));
	if (!bStopUsing)
	{
		m_unused.Insert(nIndex);
	}
}


template< class T, int HandleBits >
void CUtlHandleTable<T, HandleBits>::SetHandle(UtlHandle_t handle, T* pData)
{
	EntryType_t* entry = const_cast<EntryType_t*>(GetEntry(handle, false));
	Assert(entry);
	if (entry == NULL)
		return;

	if (entry->nInvalid)
	{
		++m_nValidHandles;
		entry->nInvalid = 0;
	}
	entry->m_pData = pData;
}

template< class T, int HandleBits >
T* CUtlHandleTable<T, HandleBits>::GetHandle(UtlHandle_t handle) const
{
	const EntryType_t* entry = GetEntry(handle, true);
	return entry ? entry->m_pData : NULL;
}

template< class T, int HandleBits >
T* CUtlHandleTable<T, HandleBits>::GetHandle(UtlHandle_t handle, bool checkValidity) const
{
	const EntryType_t* entry = GetEntry(handle, checkValidity);
	return entry ? entry->m_pData : NULL;
}


template< class T, int HandleBits >
bool CUtlHandleTable<T, HandleBits>::IsHandleValid(UtlHandle_t handle) const
{
	if (handle == UTLHANDLE_INVALID)
		return false;

	unsigned int nIndex = GetListIndex(handle);
	AssertOnce(nIndex < (unsigned int)m_list.Count());
	if (nIndex >= (unsigned int)m_list.Count())
		return false;

	const EntryType_t& entry = m_list[nIndex];
	if (entry.m_nSerial != GetSerialNumber(handle))
		return false;

	if (1 == entry.nInvalid)
		return false;

	return true;
}


template< class T, int HandleBits >
unsigned int CUtlHandleTable<T, HandleBits>::GetValidHandleCount() const
{
	return m_nValidHandles;
}

template< class T, int HandleBits >
unsigned int CUtlHandleTable<T, HandleBits>::GetHandleCount() const
{
	return m_list.Count();
}

template< class T, int HandleBits >
UtlHandle_t CUtlHandleTable<T, HandleBits>::GetHandleFromIndex(int i) const
{
	if (m_list[i].m_pData)
		return CreateHandle(m_list[i].m_nSerial, i);
	return UTLHANDLE_INVALID;
}

template< class T, int HandleBits >
int CUtlHandleTable<T, HandleBits>::GetIndexFromHandle(UtlHandle_t h) const
{
	if (h == UTLHANDLE_INVALID)
		return -1;

	return GetListIndex(h);
}



template< class T, int HandleBits >
unsigned int CUtlHandleTable<T, HandleBits>::GetSerialNumber(UtlHandle_t handle)
{
	return ((HandleType_t*)&handle)->nSerial;
}

template< class T, int HandleBits >
unsigned int CUtlHandleTable<T, HandleBits>::GetListIndex(UtlHandle_t handle)
{
	return ((HandleType_t*)&handle)->nIndex;
}

template< class T, int HandleBits >
UtlHandle_t CUtlHandleTable<T, HandleBits>::CreateHandle(unsigned int nSerial, unsigned int nIndex)
{
	HandleType_t h(nIndex, nSerial);
	return *(UtlHandle_t*)&h;
}


template< class T, int HandleBits >
const typename CUtlHandleTable<T, HandleBits>::EntryType_t* CUtlHandleTable<T, HandleBits>::GetEntry(UtlHandle_t handle, bool checkValidity) const
{
	if (handle == UTLHANDLE_INVALID)
		return NULL;

	unsigned int nIndex = GetListIndex(handle);
	Assert(nIndex < (unsigned int)m_list.Count());
	if (nIndex >= (unsigned int)m_list.Count())
		return NULL;

	const EntryType_t& entry = m_list[nIndex];
	if (entry.m_nSerial != GetSerialNumber(handle))
		return NULL;

	if (checkValidity &&
		(1 == entry.nInvalid))
		return NULL;

	return &entry;
}

template< class T, int HandleBits >
void CUtlHandleTable<T, HandleBits>::MarkHandleInvalid(UtlHandle_t handle)
{
	if (handle == UTLHANDLE_INVALID)
		return;

	unsigned int nIndex = GetListIndex(handle);
	Assert(nIndex < (unsigned int)m_list.Count());
	if (nIndex >= (unsigned int)m_list.Count())
		return;

	EntryType_t& entry = m_list[nIndex];
	if (entry.m_nSerial != GetSerialNumber(handle))
		return;

	if (!entry.nInvalid)
	{
		--m_nValidHandles;
		entry.nInvalid = 1;
	}
}

template< class T, int HandleBits >
void CUtlHandleTable<T, HandleBits>::MarkHandleValid(UtlHandle_t handle)
{
	if (handle == UTLHANDLE_INVALID)
		return;

	unsigned int nIndex = GetListIndex(handle);
	Assert(nIndex < (unsigned int)m_list.Count());
	if (nIndex >= (unsigned int)m_list.Count())
		return;

	EntryType_t& entry = m_list[nIndex];
	if (entry.m_nSerial != GetSerialNumber(handle))
		return;

	if (entry.nInvalid)
	{
		++m_nValidHandles;
		entry.nInvalid = 0;
	}
}


template< class T >
class CUtlHandle
{
public:
	CUtlHandle();
	explicit CUtlHandle(T* pObject);
	CUtlHandle(UtlHandle_t h);
	CUtlHandle(const CUtlHandle<T>& h);

	void Set(T* pObject);
	void Set(UtlHandle_t h);
	const CUtlHandle<T>& operator=(UtlHandle_t h);
	const CUtlHandle<T>& operator=(T* pObject);

	T* Get();
	const T* Get() const;

	bool IsValid() const;

	operator T* ();
	operator UtlHandle_t();
	operator bool();
	T* operator->();
	const T* operator->() const;

	bool operator==(CUtlHandle<T> h) const;
	bool operator==(T* pObject) const;
	bool operator==(UtlHandle_t h) const;
	bool operator!=(CUtlHandle<T> h) const;
	bool operator!=(T* pObject) const;
	bool operator!=(UtlHandle_t h) const;

private:
	UtlHandle_t m_handle;
};


template< class T >
CUtlHandle<T>::CUtlHandle() : m_handle(UTLHANDLE_INVALID)
{
}

template< class T >
CUtlHandle<T>::CUtlHandle(T* pObject)
{
	Set(pObject);
}

template< class T >
CUtlHandle<T>::CUtlHandle(UtlHandle_t h)
{
	m_handle = h;
}

template< class T >
CUtlHandle<T>::CUtlHandle(const CUtlHandle<T>& h)
{
	m_handle = h.m_handle;
}


template< class T >
void CUtlHandle<T>::Set(T* pObject)
{
	m_handle = pObject ? pObject->GetHandle() : UTLHANDLE_INVALID;
}

template< class T >
void CUtlHandle<T>::Set(UtlHandle_t h)
{
	m_handle = h;
}

template< class T >
const CUtlHandle<T>& CUtlHandle<T>::operator=(UtlHandle_t h)
{
	Set(h);
	return *this;
}

template< class T >
const CUtlHandle<T>& CUtlHandle<T>::operator=(T* pObject)
{
	Set(pObject);
	return *this;
}


template< class T >
bool CUtlHandle<T>::IsValid() const
{
	return T::IsHandleValid(m_handle);
}


template< class T >
T* CUtlHandle<T>::Get()
{
	return T::GetPtrFromHandle(m_handle);
}

template< class T >
const T* CUtlHandle<T>::Get() const
{
	return T::GetPtrFromHandle(m_handle);
}


template< class T >
CUtlHandle<T>::operator T* ()
{
	return Get();
}

template< class T >
CUtlHandle<T>::operator UtlHandle_t()
{
	return m_handle;
}

template< class T >
T* CUtlHandle<T>::operator->()
{
	return Get();
}

template< class T >
const T* CUtlHandle<T>::operator->() const
{
	return Get();
}

template< class T >
CUtlHandle<T>::operator bool()
{
	return m_handle != UTLHANDLE_INVALID;
}


template< class T >
bool CUtlHandle<T>::operator==(CUtlHandle<T> h) const
{
	return m_handle == h.m_handle;
}

template< class T >
bool CUtlHandle<T>::operator==(T* pObject) const
{
	UtlHandle_t h = pObject ? pObject->GetHandle() : UTLHANDLE_INVALID;
	return m_handle == h;
}

template< class T >
bool CUtlHandle<T>::operator==(UtlHandle_t h) const
{
	return m_handle == h;
}

template< class T >
bool CUtlHandle<T>::operator!=(CUtlHandle<T> h) const
{
	return m_handle != h.m_handle;
}

template< class T >
bool CUtlHandle<T>::operator!=(T* pObject) const
{
	UtlHandle_t h = pObject ? pObject->GetHandle() : UTLHANDLE_INVALID;
	return m_handle != h;
}

template< class T >
bool CUtlHandle<T>::operator!=(UtlHandle_t h) const
{
	return m_handle != h;
}


#define DECLARE_HANDLES( _className, _handleBitCount )						\
	public:																	\
		UtlHandle_t GetHandle()												\
		{																	\
			return m_Handle;												\
		}																	\
		static _className* GetPtrFromHandle( UtlHandle_t h )				\
		{																	\
			return m_HandleTable.GetHandle( h );							\
		}																	\
		static bool IsHandleValid( UtlHandle_t h )							\
		{																	\
			return m_HandleTable.IsHandleValid( h );						\
		}																	\
	private:																\
		UtlHandle_t m_Handle;												\
		static CUtlHandleTable< _className, _handleBitCount > m_HandleTable


#define IMPLEMENT_HANDLES( _className, _handleBitCount )					\
	CUtlHandleTable< _className, _handleBitCount > _className::m_HandleTable;


#define CONSTRUCT_HANDLE( )						\
	m_Handle = m_HandleTable.AddHandle();		\
	m_HandleTable.SetHandle( m_Handle, this )

#define DESTRUCT_HANDLE()						\
	m_HandleTable.RemoveHandle( m_Handle );		\
	m_Handle = UTLHANDLE_INVALID



#endif  
