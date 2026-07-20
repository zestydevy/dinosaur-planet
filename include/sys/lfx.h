#ifndef _SYS_LFX_H
#define _SYS_LFX_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

// size:0x28
typedef struct {
/*0000*/ u16 unk0;
/*0002*/ u16 unk2;
/*0004*/ s16 unk4;  //coords? light direction/origin?
/*0006*/ s16 unk6;
/*0008*/ s16 unk8;
/*000A*/ s16 unka;
/*000C*/ s16 unkC;  //y coord?
/*000E*/ u16 unke;  //LAction.bin index
/*0010*/ u16 unk10; //lightLayer (lights' influences can be combined when on different layers)
/*0012*/ union {
             u16 asShort;
             u8 asByte;
         } unk12;
/*0014*/ u8 unk14;
/*0015*/ u8 unk15; //colour1_r
/*0016*/ u8 unk16; //colour1_g
/*0017*/ u8 unk17; //colour1_b
/*0018*/ u8 unk18; //colour2_r
/*0019*/ u8 unk19; //colour2_g
/*001A*/ u8 unk1a; //colour2_b
/*001B*/ u8 unk1b;
/*001C*/ u8 unk1c;
/*001D*/ u8 unk1d;
/*001E*/ u8 unk1e;
/*001F*/ u8 unk1f; //radius1 (falloff distance for one of the colours)
/*0020*/ u8 unk20; //radius2 (falloff distance for the other colour)
/*0021*/ u8 unk21;
/*0022*/ u8 unk22;
/*0023*/ u8 unk23;
/*0024*/ u8 unk24[0x28 - 0x24];
} LightAction; //LActions.bin structs used by NewLfx DLL

/** Applies LightAction */
s32 lfxAction(Object* arg0, Object* arg1, u16 actionIndex, s8, s32, s32);
s32 lfxRestoreAction(Object*, Object*, u16, s8, s32, s32);

#endif
