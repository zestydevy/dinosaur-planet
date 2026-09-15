#include "dll.h"
#include "sys/main.h"
#include "game/gamebits.h"
#include "sys/objects.h"
#include "sys/objtype.h"
#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/common/kyte_target.h"

typedef struct {
/*00*/ f32 timer;
/*04*/ s32 flightCurve;
/*08*/ u8 state;
/*0C*/ CurveSetup* curveSetup;
} FindKyteObject_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 findRange;
/*19*/ u8 timerSeconds;
/*1A*/ u16 kyteFlightCurve;
/*1C*/ u8 kyteTalkSeq;
/*1D*/ u8 unk1D;
/*1E*/ u8 checkDistance2D;  //Use a lateral (2D) distance check instead of a 3D one
} FindKyteObject_Setup;

typedef enum {
    FindKyteObject_STATE_0_Finding_CurveSetup,
    FindKyteObject_STATE_1_Show_Find_Command_When_Nearby,
    FindKyteObject_STATE_2_Using_Find,
    FindKyteObject_STATE_3_Finished
} FindKyteObject_States;

// offset: 0x0 | ctor
void FindKyteObject_ctor(void* dll) { }

// offset: 0xC | dtor
void FindKyteObject_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void FindKyteObject_obj_Setup(Object* self, ObjSetup* setup, s32 reset) {
    FindKyteObject_Data* objdata;

    objdata = self->data;
    objdata->state = FindKyteObject_STATE_0_Finding_CurveSetup;
    objAddObjectType(self, OBJTYPE_KyteTarget);
}

// offset: 0x64 | func: 1 | export: 1
void FindKyteObject_obj_Control(Object* self) {
    CurveSetup* curveSetup;
    FindKyteObject_Setup* setup;
    Object* kyte;
    Object* player;
    f32 dist;
    s16 gamebit;
    FindKyteObject_Data* objdata;

    objdata = self->data;
    setup = (FindKyteObject_Setup*)self->setup;

    switch (objdata->state) {
    case FindKyteObject_STATE_0_Finding_CurveSetup:
        curveSetup = gDLL_25->vtbl->func_2A50(self, setup->kyteFlightCurve);
        objdata->curveSetup = curveSetup;
        if (curveSetup) {
            objdata->state = FindKyteObject_STATE_1_Show_Find_Command_When_Nearby;
        }
        break;
    case FindKyteObject_STATE_1_Show_Find_Command_When_Nearby:
        //If the curveSetup's "used" gamebit is specified and set, set Kyte's flight curve and go back to State 0
        gamebit = objdata->curveSetup->type22.usedBit;
        if ((gamebit != NO_GAMEBIT) && (mainGetBits(gamebit))) {
            mainSetBits(BIT_Kyte_Flight_Curve, objdata->flightCurve);
            objdata->state = FindKyteObject_STATE_0_Finding_CurveSetup;
            break;
        }

        //If Kyte's around the the player's near the object, enable the Find command option
        kyte = objGetSidekick();
        if (kyte) {
            player = objGetPlayer();
            if (setup->checkDistance2D) {
                dist = vec3DistanceXZSquared(&player->globalPosition, &self->globalPosition);
            } else {
                dist = vec3DistanceSquared(&player->globalPosition, &self->globalPosition);
            }

            if (dist <= SQ(setup->findRange * 2)) {
                //Enable Find command option
                ((DLL_ISidekick*)kyte->dll)->vtbl->EnableCommand(kyte, Sidekick_Command_INDEX_1_Find);

                //Advance state if Find command was used
                if (gDLL_1_cmdmenu->vtbl->was_this_item_used(Sidekick_Command_INDEX_1_Find)) {
                    objdata->flightCurve = mainGetBits(BIT_Kyte_Flight_Curve);
                    mainSetBits(BIT_Kyte_Flight_Curve, setup->kyteFlightCurve);

                    //Optionally set Kyte's talk sequence
                    if (setup->kyteTalkSeq != 0) {
                        mainSetBits(BIT_Kyte_Flight_Talk_Sequence, setup->kyteTalkSeq);
                    }
                    
                    objdata->timer = setup->timerSeconds * 60.0f;
                    objdata->state = FindKyteObject_STATE_2_Using_Find;
                }
            }
        }
        break;
    case FindKyteObject_STATE_2_Using_Find:
        objdata->timer -= gUpdateRateF;

        //Revert to State 1 after a few seconds, or once the curveSetup's "used" gamebit is set
        if ((objdata->timer <= 0.0f) || 
            ((gamebit = objdata->curveSetup->type22.usedBit, (gamebit != NO_GAMEBIT)) && mainGetBits(gamebit))
        ) {
            //Clear Kyte's talk sequence, if specified
            if (setup->kyteTalkSeq != 0) {
                mainSetBits(BIT_Kyte_Flight_Talk_Sequence, -1);
            }

            //Revert state
            objdata->state = FindKyteObject_STATE_1_Show_Find_Command_When_Nearby;

            //Set Kyte's flight curve
            if (mainGetBits(BIT_Kyte_Flight_Curve) == setup->kyteFlightCurve) {
                mainSetBits(BIT_Kyte_Flight_Curve, objdata->flightCurve);
            }
        }
        break;
    case FindKyteObject_STATE_3_Finished: //Inaccessible?
        break;
    }
}

// offset: 0x3AC | func: 2 | export: 2
void FindKyteObject_obj_Update(Object* self) { }

// offset: 0x3B8 | func: 3 | export: 3
void FindKyteObject_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x3D0 | func: 4 | export: 7
s32 FindKyteObject_kyteTarget_Interact(Object* self, s32 arg1) {
    return 0;
}

// offset: 0x3E4 | func: 5 | export: 8
s32 FindKyteObject_kyteTarget_Func_3E4(Object* self, s32 arg1, s32 arg2) {
    return 0;
}

// offset: 0x3FC | func: 6 | export: 4
void FindKyteObject_obj_Free(Object* self, s32 onlySelf) {
    objFreeObjectType(self, OBJTYPE_KyteTarget);
}

// offset: 0x43C | func: 7 | export: 5
u32 FindKyteObject_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x44C | func: 8 | export: 6
u32 FindKyteObject_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(FindKyteObject_Data);
}

// offset: 0x460 | func: 9 | export: 9
s32 FindKyteObject_kyteTarget_Approach(Object* self, s32 arg1, f32* deltaY) {
    return 0;
}

// offset: 0x478 | func: 10 | export: 10
s32 FindKyteObject_kyteTarget_GetType(Object* self) {
    //Returns a flag that can be used to filter between different KyteTarget objects (in this case a Find target)
    return 1;
}
