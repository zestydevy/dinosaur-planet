#include "common.h"
#include "unktypes.h"
#include "game/gamebits.h"
#include "sys/gfx/model.h"
#include "sys/objtype.h"

#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/725_SC_flamegameflame.h"

/*0x0*/ static u16 dActivateGamebits[] = {
    BIT_64D, BIT_64E, BIT_64F, BIT_650
};
/*0x8*/ static u16 dFinishedGamebits[] = {
    BIT_768, BIT_769, BIT_76A, BIT_76B
};
/*0x10*/ static u16 dKyteFlightCurveIDs[] = {
    0x84, 0x85, 0x86, 0x87
};

// offset: 0x0 | ctor
void SCFlameGameFlame_ctor(void *dll) { }

// offset: 0xC | dtor
void SCFlameGameFlame_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SCFlameGameFlame_setup(Object* self, SCFlameGameFlame_Setup* objSetup, s32 arg2) {
    SCFlameGameFlame_Data* objData;
    s32 i;

    objData = self->data;
    self->srt.yaw = objSetup->yaw << 8;
    obj_add_object_type(self, OBJTYPE_48);

    for (i = 0, objData->unk4 = 0; i < 4; i++) {
        if (objSetup->kyteFlightCurveID == dKyteFlightCurveIDs[i]) {
            objData->gamebitActive = dActivateGamebits[i];
            objData->gamebitFinished = dFinishedGamebits[i];

            //index break
            i = 5;
        }
    }
}

// offset: 0xE0 | func: 1 | export: 1
void SCFlameGameFlame_control(Object* self) {
    SCFlameGameFlame_Data* objData;
    SCFlameGameFlame_Setup* objSetup;
    Object* sidekick;

    objData = self->data;
    objSetup = (SCFlameGameFlame_Setup*)self->setup;
    sidekick = get_sidekick();
    
    if (
        (objData->flags & SCFlameGameFlame_FLAG_Target_in_View) && 
        main_get_bits(objData->gamebitActive)
    ) {
        //Check if Kyte is nearby
        if (sidekick != NULL) {
            if (vec3_distance_xz_squared(&sidekick->positionMirror, &self->positionMirror) <= SQ(objSetup->flameRange)) {
                objData->flags |= SCFlameGameFlame_FLAG_Kyte_is_Nearby;

                //Kyte uses the Flame command
                ((DLL_ISidekick*)sidekick->dll)->vtbl->func14(sidekick, BIT_4);
            } else {
                objData->flags &= ~SCFlameGameFlame_FLAG_Kyte_is_Nearby;
            }
        }

        //Check if the player issued the Flame command from the inventory
        if (gDLL_1_cmdmenu->vtbl->func_DF4(BIT_4)) {
            main_set_bits(BIT_Kyte_Flight_Curve, objSetup->kyteFlightCurveID);
            objData->flags |= SCFlameGameFlame_FLAG_Player_Gave_Command;
        }
    }

    //When Flame is finished, set a gamebit and destroy self
    if ((objData->flags & SCFlameGameFlame_FLAG_Finished) && 
        (vec3_distance_xz_squared(&sidekick->positionMirror, &self->positionMirror) <= 400.0f)
    ) {
        main_set_bits(objData->gamebitFinished, 1);
        objData->flags &= ~SCFlameGameFlame_FLAG_Finished;
        obj_destroy_object(self);
    }
}

// offset: 0x2A4 | func: 2 | export: 2
void SCFlameGameFlame_update(Object *self) { }

// offset: 0x2B0 | func: 3 | export: 3
void SCFlameGameFlame_print(Object* self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    SCFlameGameFlame_Data* objData = self->data;
    
    objData->flags &= ~SCFlameGameFlame_FLAG_Target_in_View;
    if (visibility) {
        objData->flags |= SCFlameGameFlame_FLAG_Target_in_View;
    }
}

// offset: 0x2F0 | func: 4 | export: 4
void SCFlameGameFlame_free(Object* self, s32 arg1) {
    obj_free_object_type(self, OBJTYPE_48);
}

// offset: 0x330 | func: 5 | export: 5
u32 SCFlameGameFlame_get_model_flags(Object *self) {
    return MODFLAGS_1;
}

// offset: 0x340 | func: 6 | export: 6
u32 SCFlameGameFlame_get_data_size(Object *self, u32 a1) {
    return sizeof(SCFlameGameFlame_Data);
}

// offset: 0x354 | func: 7 | export: 7
int SCFlameGameFlame_func_354(Object* self, s32 arg1) {
    SCFlameGameFlame_Data* objData = self->data;
    s32 outValue;
    
    outValue = FALSE;
    
    if (arg1 == FALSE) {
        outValue = TRUE;
        objData->flags &= ~SCFlameGameFlame_FLAG_Player_Gave_Command;
        objData->flags |= SCFlameGameFlame_FLAG_Finished;
    }
    
    if (arg1 == TRUE) {
        outValue = TRUE;
        objData->flags &= ~SCFlameGameFlame_FLAG_Finished;
    }
    
    return outValue;
}

// offset: 0x3A4 | func: 8 | export: 8
s32 SCFlameGameFlame_func_3A4(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2) {
    return 0;
}

// offset: 0x3BC | func: 9 | export: 9
s32 SCFlameGameFlame_func_3BC(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2) {
    return 0;
}

// offset: 0x3D4 | func: 10 | export: 10
s32 SCFlameGameFlame_func_3D4(Object* self) {
    SCFlameGameFlame_Data* objData = self->data;
    
    if (objData->flags & SCFlameGameFlame_FLAG_Player_Gave_Command) {
        return 2;
    } else {
        return 0;
    }
}
