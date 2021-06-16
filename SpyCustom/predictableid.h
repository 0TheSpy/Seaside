#ifndef PREDICTABLEID_H
#define PREDICTABLEID_H
#ifdef _WIN32
#pragma once
#endif

#if !defined( NO_ENTITY_PREDICTION )
class CPredictableId
{
public:
	CPredictableId(void);

	static void		ResetInstanceCounters(void);

	bool			IsActive(void) const;

	void			Init(int player, int command, const char* classname, const char* module, int line);

	int				GetPlayer(void) const;
	int				GetHash(void) const;
	int				GetInstanceNumber(void) const;
	int				GetCommandNumber(void) const;

	void			SetAcknowledged(bool ack);
	bool			GetAcknowledged(void) const;

	int				GetRaw(void) const;
	void			SetRaw(int raw);

	char const* Describe(void) const;

	bool operator ==(const CPredictableId& other) const;
	bool operator !=(const CPredictableId& other) const;
private:
	void			SetCommandNumber(int commandNumber);
	void			SetPlayer(int playerIndex);
	void			SetInstanceNumber(int counter);

	struct bitfields
	{
		unsigned int ack : 1;	 
		unsigned int player : 5;	 
		unsigned int command : 10;	 
		unsigned int hash : 12;	 
		unsigned int instance : 4;	 
	} m_PredictableID;
};

FORCEINLINE void NetworkVarConstruct(CPredictableId& x) {}

#endif

#endif  