#ifndef VSTDLIB_RANDOM_H
#define VSTDLIB_RANDOM_H

#include "vstdlib.h"
#include "basetypes.h"
#include "threadtools.h"
#include "interface.h"

#define NTAB 32

#pragma warning(push)
#pragma warning( disable:4251 )

class IUniformRandomStream
{
public:
	virtual void	SetSeed(int iSeed) = 0;

	virtual float	RandomFloat(float flMinVal = 0.0f, float flMaxVal = 1.0f) = 0;
	virtual int		RandomInt(int iMinVal, int iMaxVal) = 0;
	virtual float	RandomFloatExp(float flMinVal = 0.0f, float flMaxVal = 1.0f, float flExponent = 1.0f) = 0;
};


class VSTDLIB_CLASS CUniformRandomStream : public IUniformRandomStream
{
public:
	CUniformRandomStream();

	virtual void	SetSeed(int iSeed);

	virtual float	RandomFloat(float flMinVal = 0.0f, float flMaxVal = 1.0f);
	virtual int		RandomInt(int iMinVal, int iMaxVal);
	virtual float	RandomFloatExp(float flMinVal = 0.0f, float flMaxVal = 1.0f, float flExponent = 1.0f);

private:
	int		GenerateRandomNumber();

	int m_idum;
	int m_iy;
	int m_iv[NTAB];

	CThreadFastMutex m_mutex;
};


class VSTDLIB_CLASS CGaussianRandomStream
{
public:
	CGaussianRandomStream(IUniformRandomStream* pUniformStream = NULL);

	void	AttachToStream(IUniformRandomStream* pUniformStream = NULL);

	float	RandomFloat(float flMean = 0.0f, float flStdDev = 1.0f);

private:
	IUniformRandomStream* m_pUniformStream;
	bool	m_bHaveValue;
	float	m_flRandomValue;

	CThreadFastMutex m_mutex;
};


VSTDLIB_INTERFACE void	RandomSeed(int iSeed);
VSTDLIB_INTERFACE float	RandomFloat(float flMinVal = 0.0f, float flMaxVal = 1.0f);
VSTDLIB_INTERFACE float	RandomFloatExp(float flMinVal = 0.0f, float flMaxVal = 1.0f, float flExponent = 1.0f);
VSTDLIB_INTERFACE int	RandomInt(int iMinVal, int iMaxVal);
VSTDLIB_INTERFACE float	RandomGaussianFloat(float flMean = 0.0f, float flStdDev = 1.0f);


VSTDLIB_INTERFACE void	InstallUniformRandomStream(IUniformRandomStream* pStream);


#pragma warning(pop)

#endif  