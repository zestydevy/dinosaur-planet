#ifndef _SYS_LFX_H
#define _SYS_LFX_H

#include "PR/ultratypes.h"
#include "game/objects/object.h"

enum LightActionType {
    LACTION_TYPE_NEWLFX = 0,
    LACTION_TYPE_TRKLIGHT = 2,
    LACTION_TYPE_SKYLIGHT = 5
};

// LACTIONS.bin entries
// size:0x28
typedef struct {
/*0000*/ u16 timer;
/*0002*/ u16 unk2;
/*0004*/ s16 x;  //coords? light direction/origin?
/*0006*/ s16 y;
/*0008*/ s16 z;
/*000A*/ s16 unka;
/*000C*/ s16 unkC;  //y coord?
/*000E*/ u16 id;  // LACTIONS.bin ID (1-indexed, 0 indicates an action that only exists at runtime)
/*0010*/ u16 slot;
/*0012*/ u8 flags;
/*0013*/ u8 flags2;
/*0014*/ u8 flags3;
/*0015*/ u8 r1;
/*0016*/ u8 g1;
/*0017*/ u8 b1;
/*0018*/ u8 r2;
/*0019*/ u8 g2;
/*001A*/ u8 b2;
/*001B*/ u8 type; // see LightActionType
/*001C*/ u8 unk1c;
/*001D*/ u8 max; // random max value
/*001E*/ u8 min; // random min value
/*001F*/ u8 maxRange; // falloff max range
/*0020*/ u8 minRange; // falloff min range
/*0021*/ u8 unk21;
/*0022*/ u8 speed;
/*0023*/ u8 unk23;
/*0024*/ u8 unk24[0x28 - 0x24];
} LightAction;

/** Applies LightAction */
s32 lfxAction(Object* arg0, Object* arg1, u16 actionIndex, s8, s32, SRT*);
s32 lfxRestoreAction(Object*, Object*, u16 actionIndex, s8, s32, SRT*);

#endif
