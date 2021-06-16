#ifndef TEMPENTITY_H
#define TEMPENTITY_H
#ifdef _WIN32
#pragma once
#endif

#define TE_EXPLFLAG_NONE        0x0        
#define TE_EXPLFLAG_NOADDITIVE  0x1                
#define TE_EXPLFLAG_DLIGHT      0x2     
#define TE_EXPLFLAG_NOSOUND     0x4       
#define TE_EXPLFLAG_NOPARTICLES 0x8     
#define TE_EXPLFLAG_DRAWALPHA   0x10         
#define TE_EXPLFLAG_ROTATE      0x20        
#define TE_EXPLFLAG_NOFIREBALL  0x40         
#define TE_EXPLFLAG_NOFIREBALLSMOKE 0x80           
#define TE_EXPLFLAG_ICE         0x100      
#define TE_EXPLFLAG_SCALEPARTICLES      0x200

#define TE_BEAMPOINTS       0            
#define TE_SPRITE           1        
#define TE_BEAMDISK         2           
#define TE_BEAMCYLINDER     3               
#define TE_BEAMFOLLOW       4                  
#define TE_BEAMRING         5              
#define TE_BEAMSPLINE       6       
#define TE_BEAMRINGPOINT    7
#define TE_BEAMLASER        8           
#define TE_BEAMTESLA        9


#endif  