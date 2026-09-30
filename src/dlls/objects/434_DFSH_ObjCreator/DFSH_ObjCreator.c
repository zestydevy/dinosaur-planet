#include "dll.h"
#include "dlls/engine/33_BaddieControl.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/215_SharpClaw.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/dll.h"
#include "sys/gfx/modgfx.h"
#include "sys/main.h"
#include "sys/objprint.h"

/*
    The DFSH_ObjCreators are arranged like this around 
    the shrine's Krazoa floor symbol (showing uIDs, creatorIndex, and gamebit):

                      [Wall Portal]

                        [Columns]

                            O
    0x160A #0 (0xF6) ->  O     O  <- 0x160D #1 (0xF7)
                            ▲
    0x160E #2 (0xF9) ->  O     O  <- 0x1617 #1 (0xF8)
                            O
            
           [Entrance]
*/

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s16 gamebit; // Unused in this DLL, but the gamebit used to enable the ObjCreator in the `DFSH_Shrine` DLL is usually the same as the one here.
/*1A*/ u16 _unk1A; // Unused in the DLL, but usually set to 0x1C - maybe intended as hit points?
/*1C*/ u16 _unk1C;
/*1E*/ s8 rotation; // yaw >> 8
/*1F*/ s8 creatorIndex; // objCreator's index around the Krazoa symbol, used to determine which gamebit activates it and the parameters for the SharpClaw it spawns
} DFSH_ObjCreator_Setup;

typedef struct {
/*0*/ s16 timer;
/*2*/ s16 timerRate;
} DFSH_ObjCreator_Data;

// offset: 0x0 | ctor
void DFSH_ObjCreator_ctor(void* dll) { }

// offset: 0xC | dtor
void DFSH_ObjCreator_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFSH_ObjCreator_obj_Setup(Object* self, DFSH_ObjCreator_Setup* setup, s32 reset) {
    DFSH_ObjCreator_Data* objdata = self->data;

    self->srt.yaw = setup->rotation << 8;
    self->unkE0 = 0;
    objdata->timer = 100;
    objdata->timerRate = 0;
    self->opacityWithFade = OBJECT_OPACITY_MAX;
    self->opacity = OBJECT_OPACITY_MAX;
}

// offset: 0x50 | func: 1 | export: 1
void DFSH_ObjCreator_obj_Control(Object* self) {
    DFSH_ObjCreator_Setup* setup = (DFSH_ObjCreator_Setup*)self->setup;
    DFSH_ObjCreator_Data* objdata = self->data;
    Baddie_Setup* sharpClawSetup;
    Object* sharpClaw;
    Baddie* sharpClawBaddie;
    DLL_IModgfx* modgfx;

    //Check if the ObjCreators are deactivated
    if (mainGetBits(BIT_DFSH_ObjCreator_Stop)) {
        self->unkE0 = 0;
        return;
    }

    if ((self->unkE0 == 0) && mainGetBits(BIT_DF_Shrine_Activate_ObjCreator_1 + setup->creatorIndex)) {
        modgfx = dllLoad(DLL_ID_146, 1);
        modgfx->vtbl->func0(self, 0, 0, 1, -1, 0);
        modgfx->vtbl->func0(self, 1, 0, 1, -1, 0);
        dll_amSfx->Play(NULL, SOUND_303, MAX_VOLUME, NULL, NULL, 0, NULL);
        dllFree(modgfx);
        
        objdata->timerRate = 1;
        self->unkE0 = 1;
    }

    if (objdata->timerRate != 0) {
        objdata->timer -= objdata->timerRate * (s16) gUpdateRateF;
    }

    if (objdata->timer > 0) {
        return;
    }

    sharpClawSetup = objAllocSetup(sizeof(Baddie_Setup), OBJ_ClubSharpClaw);
    sharpClawSetup->base.x = setup->base.x;
    sharpClawSetup->base.y = setup->base.y;
    sharpClawSetup->base.z = setup->base.z;
    sharpClawSetup->base.loadFlags = setup->base.loadFlags;
    sharpClawSetup->base.byte5 = setup->base.byte5;
    sharpClawSetup->base.byte6 = setup->base.byte6;
    sharpClawSetup->base.fadeDistance = setup->base.fadeDistance;
    sharpClawSetup->initialWeaponID = 3;
    sharpClawSetup->unk18 = BIT_DF_Shrine_SharpClaw_Defeated;
    sharpClawSetup->unk30 = -1;
    sharpClawSetup->unk2A = self->srt.yaw >> 8;
    sharpClawSetup->unk2B = 2;

    if (mainGetBits(BIT_DF_Shrine_SharpClaw_Drop_Magic_Gems)) {
        //@bug: this seems to be using an objectID, but it should be using an index from `BaddieDrop_IDs`
        sharpClawSetup->unk22 = OBJ_BoneDust; 
    } else {
        sharpClawSetup->unk22 = -1;
    }

    sharpClawSetup->unk29 = 0xFF;
    sharpClawSetup->unk2E = -1;
    sharpClawSetup->unk34 = 0xFFFF;

    switch (setup->creatorIndex) {
    default:
        sharpClawSetup->quarterHitpoints = 3;
        break;
    case 0:
        sharpClawSetup->quarterHitpoints = 5;
        break;
    case 1:
        sharpClawSetup->quarterHitpoints = 3;
        break;
    case 2:
        sharpClawSetup->quarterHitpoints = 4;
        break;
    case 3:
        sharpClawSetup->quarterHitpoints = 3;
        break;
    }

    sharpClaw = objSetupObject(&sharpClawSetup->base, OBJINIT_FLAG4 | OBJINIT_STANDALONE, self->mapID, -1, self->parent);
    if (sharpClaw != NULL) {
        sharpClawBaddie = sharpClaw->data;
        if (sharpClawBaddie != NULL) {
            switch (setup->creatorIndex) {
            default:
                sharpClawBaddie->unk3B0 = SharpClaw_FLAG_20;
                break;
            case 0:
                sharpClawBaddie->unk3B0 = SharpClaw_FLAG_20;
                break;
            case 1:
                sharpClawBaddie->unk3B0 = SharpClaw_FLAG_20;
                break;
            case 2:
                sharpClawBaddie->unk3B0 = SharpClaw_FLAG_20 | SharpClaw_FLAG_80_Vulnerable_During_Attack;
                break;
            case 3:
                sharpClawBaddie->unk3B0 = SharpClaw_FLAG_20 | SharpClaw_FLAG_80_Vulnerable_During_Attack;
                break;
            }
        }
    }

    objdata->timer = 100;
    objdata->timerRate = 0;
}

// offset: 0x3D8 | func: 2 | export: 2
void DFSH_ObjCreator_obj_Update(Object* self) { }

// offset: 0x3E4 | func: 3 | export: 3
void DFSH_ObjCreator_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x438 | func: 4 | export: 4
void DFSH_ObjCreator_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x448 | func: 5 | export: 5
u32 DFSH_ObjCreator_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x458 | func: 6 | export: 6
u32 DFSH_ObjCreator_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DFSH_ObjCreator_Data);
}
