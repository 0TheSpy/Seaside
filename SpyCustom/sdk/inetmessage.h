#ifndef INETMESSAGE_H
#define INETMESSAGE_H

#include "bitbuf.h"

class INetMsgHandler;
class INetMessage;
class INetChannel;

class INetMessage
{
public:
	virtual	~INetMessage() {};

	virtual void	SetNetChannel(INetChannel* netchan) { DebuggerBreak(); return; }
	virtual void	SetReliable(bool state) = 0;	        

	virtual bool	Process(void) { DebuggerBreak(); return false; }

	virtual	bool	ReadFromBuffer(bf_read& buffer) = 0;       
	virtual	bool	WriteToBuffer(bf_write& buffer) const = 0;	      

	virtual bool	IsReliable(void) const = 0;        

	virtual int				GetType(void) const = 0;        
	virtual int				GetGroup(void) const = 0;	       
	virtual const char* GetName(void) const = 0;	      

	virtual INetChannel* GetNetChannel(void) const { DebuggerBreak(); return NULL; }

	virtual const char* ToString(void) const = 0;         
	virtual size_t			GetSize() const = 0;
};

class INetMessageBinder
{
public:
	virtual	~INetMessageBinder() {};

	virtual int	GetType(void) const = 0;        
	virtual void SetNetChannel(INetChannel* netchan) = 0;      
	virtual INetMessage* CreateFromBuffer(bf_read& buffer) = 0;
	virtual bool Process(const INetMessage& src) = 0;
};

class INetChannelHandler
{
public:
	virtual	~INetChannelHandler(void) {};

	virtual void ConnectionStart(INetChannel* chan) = 0;	       
	virtual void ConnectionStop() = 0;	       

	virtual void ConnectionClosing(const char* reason) = 0;         

	virtual void ConnectionCrashed(const char* reason) = 0;    

	virtual void PacketStart(int incoming_sequence, int outgoing_acknowledged) = 0;	       

	virtual void PacketEnd(void) = 0;      

	virtual void FileRequested(const char* fileName, unsigned int transferID, bool isReplayDemoFile) = 0;        

	virtual void FileReceived(const char* fileName, unsigned int transferID, bool isReplayDemoFile) = 0;     

	virtual void FileDenied(const char* fileName, unsigned int transferID, bool isReplayDemoFile) = 0;	        

	virtual void FileSent(const char* fileName, unsigned int transferID, bool isReplayDemoFile) = 0;	    

	virtual bool ChangeSplitscreenUser(int nSplitScreenUserSlot) = 0;                   
};

#endif

 
