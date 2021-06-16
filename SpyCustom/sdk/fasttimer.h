#ifndef FASTTIMER_H
#define FASTTIMER_H
#ifdef _WIN32
#pragma once
#endif

#ifdef _WIN32
#include <intrin.h>
#endif

#include <assert.h>
#include "platform.h"

PLATFORM_INTERFACE uint64 g_ClockSpeed;
#if defined( _X360 ) && defined( _CERT )
PLATFORM_INTERFACE unsigned long g_dwFakeFastCounter;
#endif

PLATFORM_INTERFACE double g_ClockSpeedMicrosecondsMultiplier;
PLATFORM_INTERFACE double g_ClockSpeedMillisecondsMultiplier;
PLATFORM_INTERFACE double g_ClockSpeedSecondsMultiplier;

class CCycleCount
{
	friend class CFastTimer;

public:
	CCycleCount();
	CCycleCount(uint64 cycles);

	void			Sample();	                  

	void			Init();		   
	void			Init(float initTimeMsec);
	void			Init(double initTimeMsec) { Init((float)initTimeMsec); }
	void			Init(uint64 cycles);
	bool			IsLessThan(CCycleCount const& other) const;					   

	unsigned long	GetCycles()  const;
	uint64			GetLongCycles() const;

	unsigned long	GetMicroseconds() const;
	uint64			GetUlMicroseconds() const;
	double			GetMicrosecondsF() const;
	void			SetMicroseconds(unsigned long nMicroseconds);

	unsigned long	GetMilliseconds() const;
	double			GetMillisecondsF() const;

	double			GetSeconds() const;

	CCycleCount& operator+=(CCycleCount const& other);

	static void		Add(CCycleCount const& rSrc1, CCycleCount const& rSrc2, CCycleCount& dest);	    

	static void		Sub(CCycleCount const& rSrc1, CCycleCount const& rSrc2, CCycleCount& dest);	    

	static uint64	GetTimestamp();

	uint64			m_Int64;
};

class PLATFORM_CLASS CClockSpeedInit
{
public:
	CClockSpeedInit()
	{
		Init();
	}

	static void Init();
};

class CFastTimer
{
public:
	void				Start();
	void				End();

	const CCycleCount& GetDuration() const;	         
	CCycleCount 		GetDurationInProgress() const;       

	static inline int64	GetClockSpeed();

private:
	CCycleCount	m_Duration;
#ifdef DEBUG_FASTTIMER
	bool m_bRunning;		    
#endif
};


class CTimeScope
{
public:
	CTimeScope(CFastTimer* pTimer);
	~CTimeScope();

private:
	CFastTimer* m_pTimer;
};

inline CTimeScope::CTimeScope(CFastTimer* pTotal)
{
	m_pTimer = pTotal;
	m_pTimer->Start();
}

inline CTimeScope::~CTimeScope()
{
	m_pTimer->End();
}

class CTimeAdder
{
public:
	CTimeAdder(CCycleCount* pTotal);
	~CTimeAdder();

	void		End();

private:
	CCycleCount* m_pTotal;
	CFastTimer	m_Timer;
};

inline CTimeAdder::CTimeAdder(CCycleCount* pTotal)
{
	m_pTotal = pTotal;
	m_Timer.Start();
}

inline CTimeAdder::~CTimeAdder()
{
	End();
}

inline void CTimeAdder::End()
{
	if (m_pTotal)
	{
		m_Timer.End();
		*m_pTotal += m_Timer.GetDuration();
		m_pTotal = 0;
	}
}



#define PROFILE_SCOPE(name) \
	class C##name##ACC : public CAverageCycleCounter \
	{ \
	public: \
		~C##name##ACC() \
		{ \
			Msg("%-48s: %6.3f avg (%8.1f total, %7.3f peak, %5d iters)\n",  \
				#name, \
				GetAverageMilliseconds(), \
				GetTotalMilliseconds(), \
				GetPeakMilliseconds(), \
				GetIters() ); \
		} \
	}; \
	static C##name##ACC name##_ACC; \
	CAverageTimeMarker name##_ATM( &name##_ACC )

#define TIME_SCOPE(name) \
	class CTimeScopeMsg_##name \
	{ \
	public: \
		CTimeScopeMsg_##name() { m_Timer.Start(); } \
		~CTimeScopeMsg_##name() \
		{ \
			m_Timer.End(); \
			Msg( #name "time: %.4fms\n", m_Timer.GetDuration().GetMillisecondsF() ); \
		} \
	private:	\
		CFastTimer	m_Timer; \
	} name##_TSM;


class CAverageCycleCounter
{
public:
	CAverageCycleCounter();

	void Init();
	void MarkIter(const CCycleCount& duration);

	unsigned GetIters() const;

	double GetAverageMilliseconds() const;
	double GetTotalMilliseconds() const;
	double GetPeakMilliseconds() const;

private:
	unsigned	m_nIters;
	CCycleCount m_Total;
	CCycleCount	m_Peak;
};

class CAverageTimeMarker
{
public:
	CAverageTimeMarker(CAverageCycleCounter* pCounter);
	~CAverageTimeMarker();

private:
	CAverageCycleCounter* m_pCounter;
	CFastTimer	m_Timer;
};


inline CCycleCount::CCycleCount()
{
	Init((uint64)0);
}

inline CCycleCount::CCycleCount(uint64 cycles)
{
	Init(cycles);
}

inline void CCycleCount::Init()
{
	Init((uint64)0);
}

inline void CCycleCount::Init(float initTimeMsec)
{
	if (g_ClockSpeedMillisecondsMultiplier > 0)
		Init((uint64)(initTimeMsec / g_ClockSpeedMillisecondsMultiplier));
	else
		Init((uint64)0);
}

inline void CCycleCount::Init(uint64 cycles)
{
	m_Int64 = cycles;
}

inline void CCycleCount::Sample()
{
	m_Int64 = Plat_Rdtsc();
}

inline CCycleCount& CCycleCount::operator+=(CCycleCount const& other)
{
	m_Int64 += other.m_Int64;
	return *this;
}


inline void CCycleCount::Add(CCycleCount const& rSrc1, CCycleCount const& rSrc2, CCycleCount& dest)
{
	dest.m_Int64 = rSrc1.m_Int64 + rSrc2.m_Int64;
}

inline void CCycleCount::Sub(CCycleCount const& rSrc1, CCycleCount const& rSrc2, CCycleCount& dest)
{
	dest.m_Int64 = rSrc1.m_Int64 - rSrc2.m_Int64;
}

inline uint64 CCycleCount::GetTimestamp()
{
	CCycleCount c;
	c.Sample();
	return c.GetLongCycles();
}

inline bool CCycleCount::IsLessThan(CCycleCount const& other) const
{
	return m_Int64 < other.m_Int64;
}


inline unsigned long CCycleCount::GetCycles() const
{
	return (unsigned long)m_Int64;
}

inline uint64 CCycleCount::GetLongCycles() const
{
	return m_Int64;
}

inline unsigned long CCycleCount::GetMicroseconds() const
{
	return (unsigned long)((m_Int64 * 1000000) / g_ClockSpeed);
}

inline uint64 CCycleCount::GetUlMicroseconds() const
{
	return ((m_Int64 * 1000000) / g_ClockSpeed);
}


inline double CCycleCount::GetMicrosecondsF() const
{
	return (double)(m_Int64 * g_ClockSpeedMicrosecondsMultiplier);
}


inline void	CCycleCount::SetMicroseconds(unsigned long nMicroseconds)
{
	m_Int64 = ((uint64)nMicroseconds * g_ClockSpeed) / 1000000;
}


inline unsigned long CCycleCount::GetMilliseconds() const
{
	return (unsigned long)((m_Int64 * 1000) / g_ClockSpeed);
}


inline double CCycleCount::GetMillisecondsF() const
{
	return (double)(m_Int64 * g_ClockSpeedMillisecondsMultiplier);
}


inline double CCycleCount::GetSeconds() const
{
	return (double)(m_Int64 * g_ClockSpeedSecondsMultiplier);
}


inline void CFastTimer::Start()
{
	m_Duration.Sample();
#ifdef DEBUG_FASTTIMER
	m_bRunning = true;
#endif
}


inline void CFastTimer::End()
{
	CCycleCount cnt;
	cnt.Sample();
	if (IsX360())
	{
		if ((uint64)cnt.m_Int64 <= (uint64)m_Duration.m_Int64)
		{
			cnt.m_Int64 += 0x100000000LL;
		}
	}

	m_Duration.m_Int64 = cnt.m_Int64 - m_Duration.m_Int64;

#ifdef DEBUG_FASTTIMER
	m_bRunning = false;
#endif
}

inline CCycleCount CFastTimer::GetDurationInProgress() const
{
	CCycleCount cnt;
	cnt.Sample();
	if (IsX360())
	{
		if ((uint64)cnt.m_Int64 <= (uint64)m_Duration.m_Int64)
		{
			cnt.m_Int64 += 0x100000000LL;
		}
	}

	CCycleCount result;
	result.m_Int64 = cnt.m_Int64 - m_Duration.m_Int64;

	return result;
}


inline int64 CFastTimer::GetClockSpeed()
{
	return g_ClockSpeed;
}


inline CCycleCount const& CFastTimer::GetDuration() const
{
#ifdef DEBUG_FASTTIMER
	assert(!m_bRunning);
#endif
	return m_Duration;
}


inline CAverageCycleCounter::CAverageCycleCounter()
	: m_nIters(0)
{
}

inline void CAverageCycleCounter::Init()
{
	m_Total.Init();
	m_Peak.Init();
	m_nIters = 0;
}

inline void CAverageCycleCounter::MarkIter(const CCycleCount& duration)
{
	++m_nIters;
	m_Total += duration;
	if (m_Peak.IsLessThan(duration))
		m_Peak = duration;
}

inline unsigned CAverageCycleCounter::GetIters() const
{
	return m_nIters;
}

inline double CAverageCycleCounter::GetAverageMilliseconds() const
{
	if (m_nIters)
		return (m_Total.GetMillisecondsF() / (double)m_nIters);
	else
		return 0;
}

inline double CAverageCycleCounter::GetTotalMilliseconds() const
{
	return m_Total.GetMillisecondsF();
}

inline double CAverageCycleCounter::GetPeakMilliseconds() const
{
	return m_Peak.GetMillisecondsF();
}

inline CAverageTimeMarker::CAverageTimeMarker(CAverageCycleCounter* pCounter)
{
	m_pCounter = pCounter;
	m_Timer.Start();
}

inline CAverageTimeMarker::~CAverageTimeMarker()
{
	m_Timer.End();
	m_pCounter->MarkIter(m_Timer.GetDuration());
}


class CLimitTimer
{
public:
	CLimitTimer() {}
	CLimitTimer(uint64 cMicroSecDuration) { SetLimit(cMicroSecDuration); }
	void SetLimit(uint64 m_cMicroSecDuration);
	bool BLimitReached() const;

	int CMicroSecOverage() const;
	uint64 CMicroSecLeft() const;

private:
	uint64 m_lCycleLimit;
};


inline void CLimitTimer::SetLimit(uint64 cMicroSecDuration)
{
	uint64 dlCycles = ((uint64)cMicroSecDuration * g_ClockSpeed) / (uint64)1000000L;
	CCycleCount cycleCount;
	cycleCount.Sample();
	m_lCycleLimit = cycleCount.GetLongCycles() + dlCycles;
}


inline bool CLimitTimer::BLimitReached() const
{
	CCycleCount cycleCount;
	cycleCount.Sample();
	return (cycleCount.GetLongCycles() >= m_lCycleLimit);
}


inline int CLimitTimer::CMicroSecOverage() const
{
	CCycleCount cycleCount;
	cycleCount.Sample();
	uint64 lcCycles = cycleCount.GetLongCycles();

	if (lcCycles < m_lCycleLimit)
		return 0;

	return((int)((lcCycles - m_lCycleLimit) * (uint64)1000000L / g_ClockSpeed));
}


inline uint64 CLimitTimer::CMicroSecLeft() const
{
	CCycleCount cycleCount;
	cycleCount.Sample();
	uint64 lcCycles = cycleCount.GetLongCycles();

	if (lcCycles >= m_lCycleLimit)
		return 0;

	return((uint64)((m_lCycleLimit - lcCycles) * (uint64)1000000L / g_ClockSpeed));
}


#endif  