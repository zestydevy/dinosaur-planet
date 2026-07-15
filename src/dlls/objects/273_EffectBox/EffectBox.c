#include "dlls/objects/210_player.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "sys/objtype.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ u8 unk18; // yaw?
/*19*/ u8 unk19; // pitch?
/*1A*/ u8 unk1A; // width? (x)
/*1B*/ u8 unk1B; // height? (y) divided by 2
/*1C*/ u8 unk1C; // length? (z)
/*1D*/ u8 effect;
/*1E*/ u8 _unk1E;
/*1F*/ u8 gamebitDisableValue; // disabled if the target gamebit is this value
/*20*/ s16 gamebit; // -1 if this effect box is always enabled
/*22*/ u8 target; // 0 = player, 1 = sidekick, 2 = pushblock
} EffectBox_Setup;

// offset: 0x0 | ctor
void EffectBox_ctor(void *dll) { }

// offset: 0xC | dtor
void EffectBox_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void EffectBox_setup(Object *self, EffectBox_Setup *setup, s32 reset) {
    if (self->unkDC == 0) {
        objAddEffectBox(self);
    }
    self->unkDC = 1;
    if (setup->gamebit >= 0) {
        self->unkE0 = setup->gamebit;
    } else {
        self->unkE0 = -1;
    }
    self->stateFlags |= (OBJSTATE_PRINT_DISABLED | OBJSTATE_UPDATE_DISABLED);
}

// offset: 0x9C | func: 1 | export: 1
void EffectBox_control(Object *self) {
    EffectBox_Setup *setup;
    Object *spB0;
    Object **objList;
    s32 i;
    s32 numObjs;
    Object *obj;
    f32 temp_fa0;
    f32 temp_fa1;
    f32 temp_fs0;
    f32 sp90;
    f32 temp_fs1;
    f32 temp_fs2;
    f32 temp_fs4;
    f32 temp_fv0;
    f32 temp_fv1;
    f32 var_fs3;
    f32 var_fs5;

    setup = (EffectBox_Setup*)self->setup;
    if ((self->unkE0 < 0) || (mainGetBits(self->unkE0) != setup->gamebitDisableValue)) {
        temp_fs4 = mathCosfInterp(-(setup->unk18 * 256));
        temp_fs0 = mathSinfInterp(-(setup->unk18 * 256));
        temp_fs1 = mathCosfInterp(-(setup->unk19 * 256));
        temp_fs2 = mathSinfInterp(-(setup->unk19 * 256));
        var_fs5 = (f32) setup->unk1A;
        sp90 = (f32) (setup->unk1B * 2);
        var_fs3 = (f32) setup->unk1C;
        switch (setup->target) {
        case 0:
            spB0 = objGetPlayer();
            if (spB0 != NULL) {
                objList = &spB0;
                numObjs = 1;
                break;
            }
            return;
        case 1:
            spB0 = objGetSidekick();
            if (spB0 != NULL) {
                objList = &spB0;
                numObjs = 1;
                break;
            }
            return;
        case 2:
            objList = objGetAllOfType(OBJTYPE_PushBlock, &numObjs);
            if (objList != NULL) {
                break;
            }
            return;
        }

        for (i = 0; i < numObjs; i++) {
            obj = objList[i];
            temp_fa0 = obj->srt.transl.x;
            temp_fv1 = obj->srt.transl.y;
            temp_fv0 = obj->srt.transl.z;
            temp_fa0 -= self->srt.transl.x;
            temp_fv1 -= self->srt.transl.y;
            temp_fv0 -= self->srt.transl.z;
            temp_fa1 = (temp_fa0 * temp_fs4) + (temp_fv0 * temp_fs0);
            if ((-var_fs5 < temp_fa1) && (temp_fa1 < var_fs5)) {
                temp_fv0 = (-temp_fa0 * temp_fs0) + (temp_fv0 * temp_fs4);
                temp_fv0 = (-temp_fv1 * temp_fs2) + (temp_fv0 * temp_fs1);
                if ((-var_fs3 < temp_fv0) && (temp_fv0 < var_fs3)) {
                    temp_fv1 = (temp_fv1 * temp_fs1) + (temp_fv0 * temp_fs2);
                    if ((temp_fv1 >= 0.0f) && (temp_fv1 < sp90)) {
                        switch (setup->target) {
                        case 1:
                            break;
                        case 0:
                            ((DLL_210_Player*)obj->dll)->vtbl->func67(obj, 1, (f32) setup->effect);
                            break;
                        case 2:
                            ((DLL_Unknown*)obj->dll)->vtbl->func[9].withTwoArgsCustom(obj, setup->effect);
                            break;
                        }
                    }
                }
            }
        }
    }
}

// offset: 0x45C | func: 2 | export: 2
void EffectBox_update(Object *self) { }

// offset: 0x468 | func: 3 | export: 3
void EffectBox_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x4BC | func: 4 | export: 4
void EffectBox_free(Object *self, s32 onlySelf) {
    objFreeEffectBox(self);
}

// offset: 0x4F8 | func: 5 | export: 5
u32 EffectBox_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x508 | func: 6 | export: 6
u32 EffectBox_get_data_size(Object *self, u32 offsetAddr) {
    return 0;
}
