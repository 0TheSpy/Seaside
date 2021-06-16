#ifndef SHAKE_H
#define SHAKE_H
#ifdef _WIN32
#pragma once
#endif


struct ScreenShake_t
{
	int		command;
	float	amplitude;
	float	frequency;
	float	duration;
};

enum ShakeCommand_t
{
	SHAKE_START = 0,		          
	SHAKE_STOP,				          
	SHAKE_AMPLITUDE,		              
	SHAKE_FREQUENCY,		              
	SHAKE_START_RUMBLEONLY,	            
	SHAKE_START_NORUMBLE,	         
};


extern int gmsgShake;

extern int gmsgFade;

#define FFADE_IN			0x0001		          
#define FFADE_OUT			0x0002		    
#define FFADE_MODULATE		0x0004		   
#define FFADE_STAYOUT		0x0008		           
#define FFADE_PURGE			0x0010		         

#define SCREENFADE_FRACBITS		9		       
struct ScreenFade_t
{
	unsigned short 	duration;		        
	unsigned short 	holdTime;		             
	short			fadeFlags;		 
	byte			r, g, b, a;		       
};


#endif  