#if !defined( INETCHANNELINFO_H )
#define INETCHANNELINFO_H
#ifdef _WIN32
#pragma once
#endif


#define FLOW_OUTGOING	0		
#define FLOW_INCOMING	1
#define MAX_FLOWS		2		   

class INetChannelInfo
{
public:

	enum {
		GENERIC = 0,	// must be first and is default group
		LOCALPLAYER,	// bytes for local player entity update
		OTHERPLAYERS,	// bytes for other players update
		ENTITIES,		// all other entity bytes
		SOUNDS,			// game sounds
		EVENTS,			// event messages
		TEMPENTS,		// temp entities
		USERMESSAGES,	// user messages
		ENTMESSAGES,	// entity messages
		VOICE,			// voice data
		STRINGTABLE,	// a stringtable update
		MOVE,			// client move cmds
		STRINGCMD,		// string command
		SIGNON,			// various signondata
		PAINTMAP,			// paintmap data
		ENCRYPTED,		// encrypted data
		TOTAL,			// must be last and is not a real group
	};

	enum ENetworkEventType_t
	{
		k_ENetworkEventType_Generic = 0,		// Generic network event
		k_ENetworkEventType_TimedOut = 1,		// Network connection timed out
	};


	virtual const char* GetName(void) const = 0;	   
	virtual const char* GetAddress(void) const = 0;       
	virtual float		GetTime(void) const = 0;	   
	virtual float		GetTimeConnected(void) const = 0;	     
	virtual int			GetBufferSize(void) const = 0;	    
	virtual int			GetDataRate(void) const = 0;      

	virtual bool		IsLoopback(void) const = 0;	    
	virtual bool		IsTimingOut(void) const = 0;	    
	virtual bool		IsPlayback(void) const = 0;	    

	virtual float		GetLatency(int flow) const = 0;	        
	virtual float		GetAvgLatency(int flow) const = 0;      
	virtual float		GetAvgLoss(int flow) const = 0;	    
	virtual float		GetAvgChoke(int flow) const = 0;	    
	virtual float		GetAvgData(int flow) const = 0;	     
	virtual float		GetAvgPackets(int flow) const = 0;   
	virtual int			GetTotalData(int flow) const = 0;	      
	virtual int			GetSequenceNr(int flow) const = 0;	    
	virtual bool		IsValidPacket(int flow, int frame_number) const = 0;       
	virtual float		GetPacketTime(int flow, int frame_number) const = 0;      
	virtual int			GetPacketBytes(int flow, int frame_number, int group) const = 0;      
	virtual bool		GetStreamProgress(int flow, int* received, int* total) const = 0;      
	virtual float		GetTimeSinceLastReceived(void) const = 0;	        
	virtual	float		GetCommandInterpolationAmount(int flow, int frame_number) const = 0;
	virtual void		GetPacketResponseLatency(int flow, int frame_number, int* pnLatencyMsecs, int* pnChoke) const = 0;
	virtual void		GetRemoteFramerate(float* pflFrameTime, float* pflFrameTimeStdDeviation) const = 0;

	virtual float		GetTimeoutSeconds() const = 0;


};

#endif  