#ifndef _SYS_GAME_OBJECTS_SETUPS_ANIMOBJ_H
#define _SYS_GAME_OBJECTS_SETUPS_ANIMOBJ_H

#include "game/objects/object.h"

// ctrl: 16 | size: 0x28
typedef struct {
    ObjSetup base;
    /** isObjSeq2CurveIndex (1 bit) | sequenceID (11 bits) | actorIndex (4 bits) */
    s16 sequenceIdBitfield;
    s16 unk1A; // event gamebit?
    s16 target; // target (actor) object ID + 4
    s8 unk1E;
    s8 seqSlot;
    s8 unk20;
    s8 unk21;
    s8 activate;
    s8 unk23;
    u8 camEaseDuration;
} AnimObj_Setup;

//For reading different values from `sequenceIdBitfield`
#define ANIMOBJ_SEQID(idBitfield) ((idBitfield & 0x7FF0) >> 4)
#define ANIMOBJ_ACTORIDX(idBitfield) (idBitfield & 0xF)

#endif
