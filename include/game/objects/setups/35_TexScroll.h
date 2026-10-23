#ifndef _SYS_GAME_OBJECTS_SETUPS_TEXSCROLL_H
#define _SYS_GAME_OBJECTS_SETUPS_TEXSCROLL_H

#include "game/objects/object.h"

// ctrl: 35 | size: 0x24
typedef struct {
    ObjSetup base;
    s16 textureIndex;       //Primary material: index in TABLES.bin subfile #14 ("scroll table")
    s16 gamebitActivate;    //gamebitID that activates animation (or -1 to skip bit check)
    s8 uSpeedB;             //U scroll speed for secondary blend material 
    s8 vSpeedB;             //V scroll speed for secondary blend material 
    s8 uSpeedA;             //U scroll speed for primary material              
    s8 vSpeedA;             //V scroll speed for primary material              
    s16 blendTextureIndex;  //Secondary blend material: index in TABLES.bin subfile #14 (optional, -1 if unused)
                            //Blend material is used for multitextured scrolling water, etc.
} TexScroll_Setup;

#endif
