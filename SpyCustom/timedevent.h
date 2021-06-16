#ifndef TIMEDEVENT_H
#define TIMEDEVENT_H
#ifdef _WIN32
#pragma once
#endif

class TimedEvent
{
public:
	TimedEvent()
	{
		m_TimeBetweenEvents = -1;
		m_fNextEvent = 0;
	}

	inline void	Init(float rate)
	{
		m_TimeBetweenEvents = 1.0f / rate;
		m_fNextEvent = 0;
	}

	inline void ResetRate(float rate)
	{
		m_TimeBetweenEvents = 1.0f / rate;
	}

	inline bool NextEvent(float& curDelta)
	{
		Assert(m_TimeBetweenEvents != -1);

		if (curDelta >= m_fNextEvent)
		{
			curDelta -= m_fNextEvent;

			m_fNextEvent = m_TimeBetweenEvents;
			return true;
		}
		else
		{
			m_fNextEvent -= curDelta;
			return false;
		}
	}

private:
	float		m_TimeBetweenEvents;
	float		m_fNextEvent;	       
};

#endif  