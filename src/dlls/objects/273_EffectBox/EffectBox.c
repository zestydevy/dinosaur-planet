#include "dlls/objects/210_player.h"
#include "game/gamebits.h"
#include "game/objects/setups/9_EffectBox.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "sys/objtype.h"

typedef enum {
    EffectBox_EFFECT_Fall_Reset = 0,        //Puts camera underneath you and causes fall reset
    EffectBox_EFFECT_WMInsert_Unk = 4,      //? (used in the unused WMInsert map)
    EffectBox_EFFECT_Gravity_Unk = 8,       //? (something to do with player gravity?)
    EffectBox_EFFECT_Stealth_Region = 12    //Robots can't see you when inside! Used a lot in CRF
} EffectBox_Effects;

typedef enum {
    EffectBox_TARGET_Player = 0,
    EffectBox_TARGET_Sidekick = 1,
    EffectBox_TARGET_Pushblock = 2
} EffectBox_Targets;

// offset: 0x0 | ctor
void EffectBox_ctor(void* dll) { }

// offset: 0xC | dtor
void EffectBox_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void EffectBox_obj_Setup(Object* self, EffectBox_Setup* setup, s32 reset) {
    if (self->unkDC == FALSE) {
        objAddEffectBox(self);
    }
    self->unkDC = TRUE;

    if (setup->gamebitEnable > NO_GAMEBIT) {
        self->unkE0 = setup->gamebitEnable;
    } else {
        self->unkE0 = NO_GAMEBIT;
    }

    self->stateFlags |= (OBJSTATE_PRINT_DISABLED | OBJSTATE_UPDATE_DISABLED);
}

// offset: 0x9C | func: 1 | export: 1
void EffectBox_obj_Control(Object* self) {
    EffectBox_Setup* setup;
    Object* target;
    Object** objList;
    s32 i;
    s32 numObjs;
    Object* obj;
    f32 distance;
    f32 halfWidth;
    f32 halfLength;
    f32 height;
    f32 cosRoll;
    f32 sinRoll;
    f32 cosYaw;
    f32 sinYaw;
    f32 dx;
    f32 dy;
    f32 dz;

    setup = (EffectBox_Setup*)self->setup;
    if ((self->unkE0 >= 0) && (mainGetBits(self->unkE0) == setup->gamebitDisableValue)) {
        return;
    }

    cosYaw = mathCosfInterp(-(setup->yaw << 8));
    sinYaw = mathSinfInterp(-(setup->yaw << 8));
    cosRoll = mathCosfInterp(-(setup->roll << 8));
    sinRoll = mathSinfInterp(-(setup->roll << 8));

    halfWidth = setup->halfWidth;
    height = setup->halfHeight * 2;
    halfLength = setup->halfLength;

    switch (setup->target) {
    case EffectBox_TARGET_Player:
        target = objGetPlayer();
        if (target != NULL) {
            objList = &target;
            numObjs = 1;
            break;
        }
        return;
    case EffectBox_TARGET_Sidekick:
        target = objGetSidekick();
        if (target != NULL) {
            objList = &target;
            numObjs = 1;
            break;
        }
        return;
    case EffectBox_TARGET_Pushblock:
        objList = objGetAllOfType(OBJTYPE_PushBlock, &numObjs);
        if (objList != NULL) {
            break;
        }
        return;
    }

    for (i = 0; i < numObjs; i++) {
        obj = objList[i];
        
        dx = obj->srt.transl.x;
        dy = obj->srt.transl.y;
        dz = obj->srt.transl.z;

        dx -= self->srt.transl.x;
        dy -= self->srt.transl.y;
        dz -= self->srt.transl.z;

        //Check if the object's inside the box
        distance = (dx * cosYaw) + (dz * sinYaw);
        if ((-halfWidth < distance) && (distance < halfWidth)) {
            dz = (-dx * sinYaw) + (dz * cosYaw);
            dz = (-dy * sinRoll) + (dz * cosRoll);
            if ((-halfLength < dz) && (dz < halfLength)) {
                dy = (dy * cosRoll) + (dz * sinRoll);
                if ((dy >= 0.0f) && (dy < height)) {
                    //Apply the effect
                    switch (setup->target) {
                    case EffectBox_TARGET_Sidekick:
                        break;
                    case EffectBox_TARGET_Player:
                        ((DLL_210_Player*)obj->dll)->vtbl->func67(obj, 1, setup->effect);
                        break;
                    case EffectBox_TARGET_Pushblock:
                        ((DLL_Unknown*)obj->dll)->vtbl->func[9].withTwoArgsCustom(obj, setup->effect);
                        break;
                    }
                }
            }
        }
    }
}

// offset: 0x45C | func: 2 | export: 2
void EffectBox_obj_Update(Object* self) { }

// offset: 0x468 | func: 3 | export: 3
void EffectBox_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x4BC | func: 4 | export: 4
void EffectBox_obj_Free(Object* self, s32 onlySelf) {
    objFreeEffectBox(self);
}

// offset: 0x4F8 | func: 5 | export: 5
u32 EffectBox_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x508 | func: 6 | export: 6
u32 EffectBox_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return 0;
}
