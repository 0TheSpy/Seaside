#ifndef SMARTPTR_H
#define SMARTPTR_H
#ifdef _WIN32
#pragma once
#endif


class CRefCountAccessor
{
public:
	template< class T >
	static void AddRef(T* pObj)
	{
		pObj->AddRef();
	}

	template< class T >
	static void Release(T* pObj)
	{
		pObj->Release();
	}
};

class CRefCountAccessorLongName
{
public:
	template< class T >
	static void AddRef(T* pObj)
	{
		pObj->AddReference();
	}

	template< class T >
	static void Release(T* pObj)
	{
		pObj->ReleaseReference();
	}
};


template < typename T >
class CPlainAutoPtr
{
public:
	explicit CPlainAutoPtr(T* p = NULL) : m_p(p) {}
	~CPlainAutoPtr(void) { Delete(); }

public:
	void Delete(void) { delete Detach(); }

private:	        
	CPlainAutoPtr(CPlainAutoPtr const& x);
	CPlainAutoPtr& operator = (CPlainAutoPtr const& x);

public:
	void Attach(T* p) { m_p = p; }
	T* Detach(void) { T* p(m_p); m_p = NULL; return p; }

public:
	bool IsValid(void) const { return m_p != NULL; }
	T* Get(void) const { return m_p; }
	T* operator -> (void) const { return Get(); }
	T& operator *  (void) const { return *Get(); }

private:
	T* m_p;
};

template < typename T >
class CArrayAutoPtr : public CPlainAutoPtr < T >             
{
public:
	explicit CArrayAutoPtr(T* p = NULL) { this->Attach(p); }
	~CArrayAutoPtr(void) { this->Delete(); }

public:
	void Delete(void) { delete[] CPlainAutoPtr < T >::Detach(); }

public:
	T& operator [] (int k) const { return CPlainAutoPtr < T >::Get()[k]; }
};


template<class T, class RefCountAccessor = CRefCountAccessor>
class CSmartPtr
{
public:
	CSmartPtr();
	CSmartPtr(T* pObj);
	CSmartPtr(const CSmartPtr<T, RefCountAccessor>& other);
	~CSmartPtr();

	T* operator=(T* pObj);
	void			operator=(const CSmartPtr<T, RefCountAccessor>& other);
	const T* operator->() const;
	T* operator->();
	bool			operator!() const;
	bool			operator==(const T* pOther) const;
	bool			IsValid() const;       
	T* GetObject() const;           
	void			MarkDeleted();

private:
	T* m_pObj;
};


template< class T, class RefCountAccessor >
inline CSmartPtr<T, RefCountAccessor>::CSmartPtr()
{
	m_pObj = NULL;
}

template< class T, class RefCountAccessor >
inline CSmartPtr<T, RefCountAccessor>::CSmartPtr(T* pObj)
{
	m_pObj = NULL;
	*this = pObj;
}

template< class T, class RefCountAccessor >
inline CSmartPtr<T, RefCountAccessor>::CSmartPtr(const CSmartPtr<T, RefCountAccessor>& other)
{
	m_pObj = NULL;
	*this = other;
}

template< class T, class RefCountAccessor >
inline CSmartPtr<T, RefCountAccessor>::~CSmartPtr()
{
	if (m_pObj)
	{
		RefCountAccessor::Release(m_pObj);
	}
}

template< class T, class RefCountAccessor >
inline T* CSmartPtr<T, RefCountAccessor>::operator=(T* pObj)
{
	if (pObj == m_pObj)
		return pObj;

	if (pObj)
	{
		RefCountAccessor::AddRef(pObj);
	}
	if (m_pObj)
	{
		RefCountAccessor::Release(m_pObj);
	}
	m_pObj = pObj;
	return pObj;
}

template< class T, class RefCountAccessor >
inline void	CSmartPtr<T, RefCountAccessor>::MarkDeleted()
{
	m_pObj = NULL;
}

template< class T, class RefCountAccessor >
inline void CSmartPtr<T, RefCountAccessor>::operator=(const CSmartPtr<T, RefCountAccessor>& other)
{
	*this = other.m_pObj;
}

template< class T, class RefCountAccessor >
inline const T* CSmartPtr<T, RefCountAccessor>::operator->() const
{
	return m_pObj;
}

template< class T, class RefCountAccessor >
inline T* CSmartPtr<T, RefCountAccessor>::operator->()
{
	return m_pObj;
}

template< class T, class RefCountAccessor >
inline bool CSmartPtr<T, RefCountAccessor>::operator!() const
{
	return !m_pObj;
}

template< class T, class RefCountAccessor >
inline bool CSmartPtr<T, RefCountAccessor>::operator==(const T* pOther) const
{
	return m_pObj == pOther;
}

template< class T, class RefCountAccessor >
inline bool CSmartPtr<T, RefCountAccessor>::IsValid() const
{
	return m_pObj != NULL;
}

template< class T, class RefCountAccessor >
inline T* CSmartPtr<T, RefCountAccessor>::GetObject() const
{
	return m_pObj;
}


template < typename T >
class CAutoPushPop
{
public:
	explicit CAutoPushPop(T& var) : m_rVar(var), m_valPop(var) {}
	CAutoPushPop(T& var, T const& valPush) : m_rVar(var), m_valPop(var) { m_rVar = valPush; }
	CAutoPushPop(T& var, T const& valPush, T const& valPop) : m_rVar(var), m_valPop(var) { m_rVar = valPush; }

	~CAutoPushPop() { m_rVar = m_valPop; }

private:	  
	CAutoPushPop(CAutoPushPop const& x);
	CAutoPushPop& operator = (CAutoPushPop const& x);

public:
	T& Get() { return m_rVar; }

private:
	T& m_rVar;
	T m_valPop;
};


#endif  