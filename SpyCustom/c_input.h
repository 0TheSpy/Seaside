#pragma once

#include "usercmd.h"
#define MULTIPLAYER_BACKUP 150

class bf_write;
class bf_read;

class CVerifiedUserCmd
{
public:
	CUserCmd	m_cmd;
	CRC32_t		m_crc;
};

class CInput
{
public:
	virtual void  Init_All(void);
	virtual void  Shutdown_All(void);
	virtual int   GetButtonBits(int);
	virtual void  CreateMove(int sequence_number, float input_sample_frametime, bool active);
	virtual void  ExtraMouseSample(float frametime, bool active);
	virtual bool  WriteUsercmdDeltaToBuffer(bf_write* buf, int from, int to, bool isnewcommand);
	virtual void  EncodeUserCmdToBuffer(bf_write& buf, int slot);
	virtual void  DecodeUserCmdFromBuffer(bf_read& buf, int slot);


	inline CUserCmd* GetUserCmd(int sequence_number);
	inline CVerifiedUserCmd* GetVerifiedCmd(int sequence_number);

	bool                m_fTrackIRAvailable;            
	bool                m_fMouseInitialized;            
	bool                m_fMouseActive;                 
	bool                m_fJoystickAdvancedInit;        
	char                pad_0x08[0x2C];                 
	void* m_pKeys;                        
	char                pad_0x38[0x6C];                 
	bool                m_fCameraInterceptingMouse;     
	bool                m_fCameraInThirdPerson;         
	bool                m_fCameraMovingWithMouse;       
	Vector              m_vecCameraOffset;              
	bool                m_fCameraDistanceMove;          
	int                 m_nCameraOldX;                  
	int                 m_nCameraOldY;                  
	int                 m_nCameraX;                     
	int                 m_nCameraY;                     
	bool                m_CameraIsOrthographic;         
	QAngle              m_angPreviousViewAngles;        
	QAngle              m_angPreviousViewAnglesTilt;    
	float               m_flLastForwardMove;            
	int                 m_nClearInputState;             
	CUserCmd* m_pCommands;                    
	CVerifiedUserCmd* m_pVerifiedCommands;            
};

CUserCmd* CInput::GetUserCmd(int sequence_number)
{
	return &m_pCommands[sequence_number % MULTIPLAYER_BACKUP];
}

CVerifiedUserCmd* CInput::GetVerifiedCmd(int sequence_number)
{
	return &m_pVerifiedCommands[sequence_number % MULTIPLAYER_BACKUP];
}


#define IN_ATTACK  (1 << 0)
#define IN_JUMP   (1 << 1)
#define IN_DUCK   (1 << 2)
#define IN_FORWARD  (1 << 3)
#define IN_BACK   (1 << 4)
#define IN_USE   (1 << 5)
#define IN_CANCEL  (1 << 6)
#define IN_LEFT   (1 << 7)
#define IN_RIGHT  (1 << 8)
#define IN_MOVELEFT  (1 << 9)
#define IN_MOVERIGHT (1 << 10)
#define IN_ATTACK2  (1 << 11)
#define IN_RUN   (1 << 12)
#define IN_RELOAD  (1 << 13)
#define IN_ALT1   (1 << 14)
#define IN_ALT2   (1 << 15)
#define IN_SCORE  (1 << 16)            
#define IN_SPEED  (1 << 17)       
#define IN_WALK   (1 << 18)     
#define IN_ZOOM   (1 << 19)      
#define IN_WEAPON1  (1 << 20)     
#define IN_WEAPON2  (1 << 21)     
#define IN_BULLRUSH  (1 << 22)
#define IN_GRENADE1  (1 << 23)   
#define IN_GRENADE2  (1 << 24)   
#define IN_LOOKSPIN  (1 << 25)