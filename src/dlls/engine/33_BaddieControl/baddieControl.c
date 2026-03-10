#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/33_BaddieControl.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/214_animobj.h"
#include "dlls/objects/260_Pollen.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/memory.h"
#include "sys/objanim.h"
#include "sys/objects.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "sys/voxmap.h"
#include "dll.h"
#include "functions.h"

// baddieControl.c (default.dol)

// Common object setup for objects that drop from defeating a baddie
typedef struct {
    ObjSetup base;
    u8 _unk18[0x1A - 0x18];
    u8 unk1A;
    u8 unk1B;
    s16 unk1C;
    u8 _unk1E[0x24 - 0x1E];
    s16 unk24;
    u8 _unk26[0x2C - 0x26];
    s16 unk2C;
    s16 unk2E;
} BaddieDrop_ObjSetup;

/*0x0*/ static Object *_bss_0;

// offset: 0x0 | ctor | export: 0
void BaddieControl_ctor(void *dll) { }

// offset: 0xC | dtor | export: 1
void BaddieControl_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 5
s32 BaddieControl_func_18(Object* arg0, Baddie* arg1, f32 arg2) {
    Object* temp_v1;
    Vec3s16 spEC;
    Vec3s16 spE4;
    Vec3f spD8;
    s32 var_s0;
    u8 spD3;
    u8 var_s5;
    s16 spD0;
    Func_80059C40_Struct sp7C;
    u16 i;

    var_s5 = 0;

    spD8.x = arg0->srt.transl.x;
    spD8.y = arg0->srt.transl.y + 10.0f;
    spD8.z = arg0->srt.transl.z;
    func_80007EE0(&spD8, &spE4);
    if (arg0->parent != NULL) {
        spD0 = (u16)(arg0->srt.yaw + arg0->parent->srt.yaw);
    } else {
        spD0 = arg0->srt.yaw;
    }

    for (i = 0; i < 4; i++) {
        var_s0 = spD0 + (i << 0xE);
        spD8.x = arg0->srt.transl.x - (fsin16_precise(var_s0) * arg2);
        spD8.y = arg0->srt.transl.y + 10.0f;
        spD8.z = arg0->srt.transl.z - (fcos16_precise(var_s0) * arg2);
        func_80007EE0(&spD8, &spEC);
        if (arg0->parent != NULL) {
            var_s0 = 1;
        } else{
            var_s0 = func_80008048(&spEC, &spE4, NULL, &spD3, 0) & 0xFF;
            if (spD3 == 1) {
                var_s0 = 1;
            }
        }
        if ((var_s0 != 0) && (func_80059C40(&arg0->srt.transl, &spD8, 1.0f, 0, &sp7C, arg0, arg1->fsa.unk4.unk259, -1, 0, 0) != 0)) {
            var_s0 = 0;
        }

        var_s5 |= var_s0 << i;
    }

    return var_s5;
}

// offset: 0x264 | func: 1 | export: 6
s32 BaddieControl_func_264(UNK_PTR *arg0, UNK_PTR *arg1) {
    return 0;
}

// offset: 0x278 | func: 2 | export: 4
void BaddieControl_func_278(Object* arg0, Object* arg1, u8 arg2, s16* arg3, s16* arg4, u16* arg5) {
    Baddie* sp2C;
    Vec3f sp20;
    Object* temp_v1;
    s32 temp_a0;
    s32 temp_v0;
    s16 var_a1;
    s32 var_v1;
    u16 temp_v0_2;

    sp2C = arg0->data;
    if ((arg0 == NULL) || (arg1 == NULL)) {
        *arg3 = 0;
        *arg4 = 0;
        *arg5 = 0;
        return;
    }
    sp20.x = arg1->positionMirror.f[0] - arg0->positionMirror.f[0];
    sp20.y = arg1->positionMirror.f[1] - arg0->positionMirror.f[1];
    sp20.z = arg1->positionMirror.f[2] - arg0->positionMirror.f[2];
    temp_v0 = arctan2_f(-sp20.x, -sp20.z);
    if (arg0->parent != NULL) {
        var_a1 = arg0->srt.yaw + arg0->parent->srt.yaw;
    } else {
        var_a1 = arg0->srt.yaw;
    }
    var_v1 = temp_v0 - (var_a1 & 0xFFFF);
    CIRCLE_WRAP(var_v1)
    temp_v0_2 = var_v1;
    *arg4 = var_v1;
    if ((temp_v0_2 < 0x31C4) || (temp_v0_2 >= 0xCE3C)) {
        sp2C->unk3B2 &= ~0x10;
    } else {
        sp2C->unk3B2 |= 0x10;
    }
    *arg3 = (temp_v0_2 / (0x10000 / arg2));
    *arg5 = (u32) sqrtf(SQ(sp20.f[2]) + (SQ(sp20.x) + SQ(sp20.y)));
}

// offset: 0x4EC | func: 3 | export: 2
f32 BaddieControl_func_4EC(Object* arg0, f32 arg1, f32 arg2, f32 arg3, Object* arg4) {
    f32 sp5C;
    f32 sp58;
    f32 temp_fa0;
    f32 pad;
    f32 pad2;
    f32 sp48;
    f32 sp44;
    f32 sp40;

    sp48 = arg4->positionMirror.x - arg1;
    sp44 = arg4->positionMirror.z - arg2;
    sp40 = sqrtf(SQ(sp48) + SQ(sp44));
    if (sp40 < arg3) {
        sp5C = fsin16_precise(arg0->srt.yaw);
        sp58 = fcos16_precise(arg0->srt.yaw);
        temp_fa0 = -(((arg1 - sp5C) * sp5C) + ((arg2 - sp58) * sp58));
        sp44 = (arg4->positionMirror.x * sp5C) + (sp58 * arg4->positionMirror.z) + temp_fa0;
        sp48 = (arg4->positionMirror3.x * sp5C) + (sp58 * arg4->positionMirror3.z) + temp_fa0;
        // @fake
        do { } while (0);
        if ((sp44 > 0.0f) && (sp48 <= 1.0f)) {
            arg4->positionMirror.x -= sp5C * sp44;
            arg4->positionMirror.z -= sp58 * sp44;
            inverse_transform_point_by_object(arg4->positionMirror.x, arg4->positionMirror.y, arg4->positionMirror.z, arg4->srt.transl.f, &arg4->srt.transl.y, &arg4->srt.transl.z, arg4->parent);
        } else if (sp48 > 1.0f) {
            sp40 = 2.0f * arg3;
        }
    }
    if (sp40 < arg3) {
        sp48 = arg4->positionMirror.x;
        sp44 = arg4->positionMirror.z;
    } else {
        sp48 = arg1;
        sp44 = arg2;
    }

    sp5C = fsin16_precise(arg0->srt.yaw + 0x4000);
    sp58 = fcos16_precise(arg0->srt.yaw + 0x4000);
    temp_fa0 = -((arg0->srt.transl.x * sp5C) + (arg0->srt.transl.z * sp58));
    return -((sp5C * sp48) + (sp58 * sp44) + temp_fa0);
}

// offset: 0x768 | func: 4 | export: 3
void BaddieControl_func_768(Object* arg0, Baddie* arg1, u32 arg2, f32 arg3, f32 arg4) {
    if (arg1->fsa.unk290 < 0.005f) {
        arg1->fsa.unk328 = 0;
        arg1->fsa.unk32A = 0;
        arg1->fsa.unk290 = 0.0f;
        arg1->fsa.unk278 = 0.0f;
    }
    arg1->fsa.unk27C = 0.0f;
    arg0->srt.yaw = ((f32) arg0->srt.yaw + ((((f32) arg1->fsa.unk32A * gUpdateRateF) / arg4) * 182.0f));
    arg1->fsa.speed += ((arg1->fsa.unk290 - arg1->fsa.speed) / arg1->fsa.unk2B0) * gUpdateRateF;
    arg1->fsa.unk278 += ((arg1->fsa.unk290 - arg1->fsa.unk278) / arg1->fsa.unk2B0) * gUpdateRateF;
    if (arg3 < arg1->fsa.speed) {
        arg1->fsa.speed = arg3;
    }
    if (arg3 < arg1->fsa.unk278) {
        arg1->fsa.unk278 = arg3;
    }
}

// offset: 0x884 | func: 5 | export: 7
u16 BaddieControl_func_884(Object* arg0) {
    Baddie_Setup* temp_v1;

    temp_v1 = (Baddie_Setup *) arg0->setup;
    if (temp_v1 != NULL) {
        return temp_v1->unk34;
    }
    return 0xD2;
}

// offset: 0x8A4 | func: 6 | export: 8
f32 BaddieControl_func_8A4(Object* arg0) {
    Baddie *objdata = arg0->data;
    return objdata->unk3E4;
}

// offset: 0x8B4 | func: 7 | export: 12
s32 BaddieControl_func_8B4(Object* arg0, AnimObj_Data* arg1, Baddie* arg2, ObjFSA_StateCallback *arg3, ObjFSA_StateCallback *arg4, s16 arg5) {
    static f32 _data_0 = 0.0f;
    static s8 _data_4 = 0;
    Object* temp_v0;
    f32 temp_fv0;
    f32 temp_fv0_2;
    f32 sp38;
    f32 sp34;
    f32 sp30;
    f32 temp_fv1;
    f32 var_fv1;
    s16 temp_v1;
    s32 var_v0;

    arg2->fsa.unk30C = 0;
    arg2->fsa.unk310 = 0;
    arg2->fsa.unk324 = 0;
    // @fake
    if (1) {}
    arg2->fsa.unk288 = 0.0f;
    arg2->fsa.unk284 = 0.0f;
    if (arg1->unk62 != 1) {
        arg1->unk4C.x = arg0->srt.transl.x;
        arg1->unk4C.f[1] = arg0->srt.transl.f[1];
        arg1->unk4C.z = arg0->srt.transl.z;
        _data_0 = 10000.0f;
        _data_4 = 0;
    }
    arg1->unk7A = 0;
    arg1->unk62 = 1;
    temp_fv0 = arg1->unk4C.x - arg0->srt.transl.x;
    temp_fv1 = arg1->unk4C.z - arg0->srt.transl.z;
    sp30 = sqrtf(SQ(temp_fv0) + SQ(temp_fv1));
    if (arg2->fsa.target == NULL) {
        return 0;
    }
    sp38 = arg2->fsa.target->srt.transl.x - arg1->unk4C.x;
    sp34 = arg2->fsa.target->srt.transl.z - arg1->unk4C.z;
    temp_fv0_2 = sqrtf(SQ(sp38) + SQ(sp34));
    var_fv1 = (temp_fv0_2 - sp30) * gUpdateRateF * 0.25f;
    if (var_fv1 > 50.0f) {
        var_fv1 = 50.0f;
    } else if (var_fv1 < 15.0f) {
        var_fv1 = 15.0f;
    }
    if (sp30 <= _data_0) {
        _data_4++;
    }
    if ((temp_fv0_2 <= sp30) || (_data_4 >= 0xA)) {
        var_v0 = arg0->srt.yaw - (arg2->fsa.target->srt.yaw & 0xFFFF);
        CIRCLE_WRAP(var_v0)
        CLAMP(var_v0, -0x2000, 0x2000)
        arg0->srt.yaw -= (var_v0 * gUpdateRate) >> 3;
        if (_data_4 >= 0xB) {
            var_v0 = 0;
        }
        if ((var_v0 < 0x100) && (var_v0 >= -0xFF)) {
            arg1->unk62 = 0;
            arg1->animCurvesCurrentFrameB = arg1->animCurvesCurrentFrameA - 1;
        } else {
            gDLL_18_objfsa->vtbl->tick(arg0, &arg2->fsa, gUpdateRateF, gUpdateRateF, arg3, arg4);
        }
    } else {
        sp38 /= temp_fv0_2;
        sp34 /= temp_fv0_2;
        arg2->fsa.unk288 = -sp38 * var_fv1;
        arg2->fsa.unk284 = sp34 * var_fv1;
        arg0->srt.transl.x = arg1->unk4C.x + (sp30 * sp38);
        arg0->srt.transl.z = arg1->unk4C.z + (sp30 * sp34);
        gDLL_18_objfsa->vtbl->tick(arg0, &arg2->fsa, gUpdateRateF, gUpdateRateF, arg3, arg4);
    }
    _data_0 = sp30;
    if (arg1->unk62 == 0) {
        arg2->unk3B4 = 0;
        arg2->fsa.animState = arg5;
        arg2->fsa.target = NULL;
        arg1->unk7A = -1;
        arg1->unk7A &= ~0x40;
        arg2->fsa.unk4.mode = 0;
        main_set_bits(arg2->unk39E, 0);
    }
    return 1;
}

// offset: 0xC88 | func: 8 | export: 13
s32 BaddieControl_func_C88(Object* arg0, Baddie* arg1, ObjFSA_StateCallback *arg2, ObjFSA_StateCallback *arg3, s16 arg4, f32* arg5, f32* arg6, s32* arg7) {
    f32 sp2C;
    f32 sp28;
    f32 temp_fv0;

    if (arg1->unk34C.unk25 != 0) {
        arg1->fsa.unk30C = 0;
        arg1->fsa.unk310 = 0;
        arg1->fsa.unk324 = 0;
        arg1->fsa.unk288 = 0.0f;
        arg1->fsa.unk284 = 0.0f;
        *arg7 = 1;
        sp2C = *arg5 - arg0->srt.transl.x;
        sp28 = *arg6 - arg0->srt.transl.z;
        temp_fv0 = sqrtf((sp2C * sp2C) + (sp28 * sp28));
        if (temp_fv0 < 10.0f) {
            *arg7 = 0;
        } else {
            sp2C = sp2C / temp_fv0;
            sp28 = sp28 / temp_fv0;
            arg1->fsa.unk288 = -sp2C * 50.0f;
            arg1->fsa.unk284 = sp28 * 50.0f;
            arg0->srt.transl.x += temp_fv0 * sp2C;
            arg0->srt.transl.z += temp_fv0 * sp28;
            gDLL_18_objfsa->vtbl->tick(arg0, &arg1->fsa, gUpdateRateF, gUpdateRateF, arg2, arg3);
        }
        if (*arg7 == 0) {
            arg1->unk3B4 = 0;
            arg1->fsa.animState = arg4;
            arg1->fsa.target = NULL;
            arg1->fsa.unk4.mode = 0;
            main_set_bits(arg1->unk39E, 0);
        }
        return 1;
    }
    
    return 0;
}

// offset: 0xE30 | func: 9 | export: 10
void BaddieControl_func_E30(Object* arg0, ObjFSA_Data* fsa, f32 arg2, s8 arg3) {
    fsa->unk324 = 0;
    fsa->flags |= 0x8000;
    if (arg0->objhitInfo != NULL) {
        func_80026128(arg0, 0, 0, -1);
    }
    if (arg3 != -1) {
        fsa->unk4.mode = arg3;
    }
    fsa->unk29C = arg2;
    fsa->unk288 = 0.0f;
    fsa->unk284 = 0.0f;
    fsa->unk310 = 0;
    fsa->unk30C = 0;
}

// offset: 0xED0 | func: 10 | export: 11
s32 BaddieControl_func_ED0(Object* arg0, Baddie* arg1, u8 arg2) {
    if (arg2 && (arg1->fsa.hitpoints <= 0) && (arg0->opacity == 0)) {
        return 0;
    }
    if ((arg0->parent == NULL) && (func_8004454C(arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z) < 0)) {
        return 0;
    }
    return 1;
}

// offset: 0xF60 | func: 11 | export: 16
s32 BaddieControl_func_F60(Object* arg0, ObjFSA_Data* fsa, f32 arg2, s32 arg3) {
    Object* player;
    Func_80059C40_Struct sp48;
    Vec3f sp3C;
    s32 var_v1;

    player = get_player();
    var_v1 = 0;
    if (fsa->unk33A != 0) {
        if ((player == fsa->target) && (fsa->hitpoints != 0)) {
            if ((arg2 < fsa->targetDist) && (arg3 != 0)) {
                var_v1 = 1;
            } else {
                if (((DLL_210_Player*)player->dll)->vtbl->func66(player, 1) == 0) {
                    var_v1 = 1;
                } else {
                    if (((DLL_210_Player*)player->dll)->vtbl->get_health(player) <= 0) {
                        var_v1 = 1;
                    } else {
                        sp3C.x = player->srt.transl.x;
                        sp3C.y = player->srt.transl.y + 10.0f;
                        sp3C.z = player->srt.transl.z;
                        if (func_80059C40(&arg0->srt.transl, &sp3C, 1.0f, 0, &sp48, arg0, 4, -1, 0U, 0) != 0) {
                            var_v1 = 1;
                        }
                    }
                }
            }
        } else {
            var_v1 = 1;
        }
    }
    return var_v1;
}

// offset: 0x10F4 | func: 12 | export: 17
Object* BaddieControl_func_10F4(Object* arg0, ObjFSA_Data* fsa, f32 arg2, s32 arg3) {
    Object* curObj;
    s32 pad5;
    Object* spF8[2];
    s32 var_v1;
    s32 stop;
    s32 i;
    f32 spE0[3];
    s32 pad;
    s32 pad2;
    Vec3s16 spD0;
    Vec3s16 spC8;
    Vec3f spBC;
    Func_80059C40_Struct sp68;
    u8 temp_t3;
    u8 sp66;
    
    stop = FALSE;
    spF8[0] = get_player();
    spF8[1] = 0;
    i = 0;
    while (stop == FALSE && (curObj = spF8[i]) != NULL) {
        spE0[0] = curObj->positionMirror.x - arg0->positionMirror.x;
        spE0[1] = curObj->positionMirror.y - arg0->positionMirror.y;
        spE0[2] = curObj->positionMirror.z - arg0->positionMirror.z;
        if ((sqrtf(SQ(spE0[0]) + SQ(spE0[1]) + SQ(spE0[2])) < arg2) && (fsa->hitpoints != 0)) {
            if (((DLL_210_Player*)curObj->dll)->vtbl->func56(curObj) > 0.5f) {
                stop = TRUE;
            }
            var_v1 = arctan2_f(-spE0[0], -spE0[2]);
            if (arg0->parent != NULL) {
                var_v1 -= ((arg0->srt.yaw + arg0->parent->srt.yaw) & 0xFFFF);
                CIRCLE_WRAP(var_v1)
            } else {
                var_v1 -= (arg0->srt.yaw & 0xFFFF);
                CIRCLE_WRAP(var_v1)
            }
            if ((var_v1 < arg3) && (-arg3 < var_v1)) {
                stop = TRUE;
            }
            if (((DLL_210_Player*)curObj->dll)->vtbl->func66(curObj, 1) == 0) {
                stop = FALSE;
            }
            if (((DLL_210_Player*)curObj->dll)->vtbl->get_health(curObj) <= 0) {
                stop = FALSE;
            } else {
                spBC.x = arg0->srt.transl.x;
                spBC.y = arg0->srt.transl.y + 10.0f;
                spBC.z = arg0->srt.transl.z;
                func_80007EE0(&spBC, &spC8);
                spBC.x = curObj->srt.transl.x;
                spBC.y = curObj->srt.transl.y + 10.0f;
                spBC.z = curObj->srt.transl.z;
                func_80007EE0(&spBC, &spD0);
                temp_t3 = func_80008048(&spD0, &spC8, NULL, &sp66, 0);
                if ((sp66 == 1) || (temp_t3 != 0)) {
                    if (func_80059C40(&arg0->srt.transl, &spBC, 1.0f, 0, &sp68, arg0, 4, -1, 0, 0) != 0) {
                        stop = FALSE;
                    }
                } else {
                    stop = FALSE;
                }
            }
        }

        i += 1;
    }

    return curObj;
}

// offset: 0x148C | func: 13 | export: 9
void BaddieControl_func_148C(Object* arg0, ObjFSA_Data* arg1, Unk80009024 *arg2, s16 arg3, s8 *arg4, s16 arg5, s16 arg6, s32 arg7, s8 arg8) {
    if (arg2 != 0) {
        arg2->unk24 = 0;
        arg2->unk25 = 0;
        arg2->unk26 = 4;
        arg2->unk27 = 0x14;
    }
    if (arg5 != -1) {
        arg1->logicState = arg5;
        arg1->enteredLogicState = 1;
    }
    if (arg6 != -1) {
        gDLL_18_objfsa->vtbl->set_anim_state(arg0, arg1, arg6);
    }
    if (arg4 != 0) {
        *arg4 = 2;
    }
    if (arg7 != 0) {
        func_80023D30(arg0, arg7, 0.0f, 0);
    }
    gDLL_27->vtbl->reset(arg0, &arg1->unk4);
    if (arg8 != -1) {
        arg1->unk4.mode = arg8;
    }
    if (arg3 != -1) {
        main_set_bits(arg3, 1);
    }
}

// offset: 0x15CC | func: 14 | export: 18
Object* BaddieControl_func_15CC(Object* arg0, s32 arg1, s32 arg2, u8 arg3) {
    Baddie_Setup* sp4C;
    BaddieDrop_ObjSetup* var_v0;
    Object* temp_v0;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    f32 sp34;
    f32 temp_fv0;
    Baddie_Setup* setup2;

    sp4C = (Baddie_Setup*)arg0->setup;
    
    if (arg1 == 0) {
        return NULL;
    }
    switch (arg1) {
    case 1:
        var_v0 = obj_alloc_create_info(sizeof(BaddieDrop_ObjSetup), OBJ_MagicDustMid);
        break;
    case 2:
        var_v0 = obj_alloc_create_info(sizeof(BaddieDrop_ObjSetup), OBJ_EnergyGem1);
        break;
    case 3:
        var_v0 = obj_alloc_create_info(sizeof(BaddieDrop_ObjSetup), OBJ_meatPickup);
        break;
    case 4:
        var_v0 = obj_alloc_create_info(sizeof(BaddieDrop_ObjSetup), OBJ_MagicDustMid);
        if (arg2 != -1) {
            var_v0->unk1C = arg2;
        }
        break;
    case 5:
        setup2 = (Baddie_Setup*)arg0->setup;
        sp40 = arg0->positionMirror.x;
        sp3C = arg0->positionMirror.y;
        sp38 = arg0->positionMirror.z;
        if (setup2 != NULL) {
            arg0->positionMirror.x = setup2->base.x;
            arg0->positionMirror.y = setup2->base.y;
            arg0->positionMirror.z = setup2->base.z;
        }
        
        sp34 = 750.0f;
        temp_v0 = obj_get_nearest_type_to(OBJTYPE_5, arg0, &sp34);
        arg0->positionMirror.x = sp40;
        arg0->positionMirror.y = sp3C;
        arg0->positionMirror.z = sp38;
        if (temp_v0 != NULL) {
            temp_fv0 = arg0->srt.transl.x;
            temp_v0->positionMirror.x = temp_fv0;
            temp_v0->srt.transl.x = temp_fv0;
            
            temp_fv0 = arg0->srt.transl.y + 15.0f;
            temp_v0->positionMirror.y = temp_fv0;
            temp_v0->srt.transl.y = temp_fv0;
            
            temp_fv0 = arg0->srt.transl.z;
            temp_v0->positionMirror.z = temp_fv0;
            temp_v0->srt.transl.z = temp_fv0;
        }
        _bss_0 = temp_v0;
        return temp_v0;
    case 6:
        var_v0 = obj_alloc_create_info(sizeof(BaddieDrop_ObjSetup), OBJ_MagicDustSmall);
        break;
    case 7:
        var_v0 = obj_alloc_create_info(sizeof(BaddieDrop_ObjSetup), OBJ_MagicDustMid);
        break;
    case 8:
        var_v0 = obj_alloc_create_info(sizeof(BaddieDrop_ObjSetup), OBJ_MagicDustLarge);
        break;
    case 9:
        var_v0 = obj_alloc_create_info(sizeof(BaddieDrop_ObjSetup), OBJ_MagicDustHuge);
        break;
    default:
        return NULL;
    }
    
    var_v0->unk1A = 0x14;
    var_v0->unk2C = -1;
    var_v0->unk1C = -1;
    var_v0->unk24 = -1;
    var_v0->base.x = arg0->srt.transl.x;
    var_v0->base.y = arg0->srt.transl.y + 30.0f;
    var_v0->base.z = arg0->srt.transl.z;
    if (arg3 != 0) {
        var_v0->unk2E = 2;
    } else {
        var_v0->unk2E = 1;
    }
    var_v0->base.loadFlags = sp4C->base.loadFlags;
    var_v0->base.byte6 = sp4C->base.byte6;
    var_v0->base.byte5 = sp4C->base.byte5;
    var_v0->base.fadeDistance = sp4C->base.fadeDistance;
    temp_v0 = obj_create((ObjSetup*)var_v0, OBJ_INIT_FLAG1 | OBJ_INIT_FLAG4, (s32) arg0->mapID, -1, arg0->parent);
    _bss_0 = temp_v0;
    return temp_v0;
}

// offset: 0x18E4 | func: 15 | export: 19
s32 BaddieControl_check_hit(Object* obj, ObjFSA_Data* fsa, Unk80009024 *arg2, s32 arg3, 
        s32 *hitAnimStateMap, s8 *hitDamageMap, s16 hitLogicState, u32* arg7, SRT* hitSRT) {
    Baddie* baddie;
    Object* player;
    s32 hitType;
    s32 sp58;
    s32 damage;
    Object* sp50;
    f32 hitX;
    f32 hitY;
    f32 hitZ;

    baddie = (Baddie*)obj->data;
    player = get_player();
    if (baddie->unk3E8 > 0.0f) {
        baddie->unk3E8 += (gUpdateRateF * baddie->unk3EC);
        if (baddie->unk3B2 & 0x20) {
            baddie->unk3B2 &= ~0x20;
            baddie->unk3B2 |= 0x40;
            if (baddie->unk3E8 > 2.0f) {
                baddie->unk3E8 = 0.0f;
                baddie->unk3B2 &= ~0x40;
            }
        } else if (baddie->unk3B2 & 0x40) {
            if (baddie->unk3E8 > 2.0f) {
                Baddie_Setup* objsetup = (Baddie_Setup*)obj->setup;
                baddie->unk3E8 = 0.0f;
                baddie->unk3B2 &= ~0x40;
                fsa->hitpoints = 0;
                obj->opacity = 0;
                obj->unkDC = 1;
                gDLL_29_Gplay->vtbl->add_time(objsetup->base.uID, (f32) (objsetup->unk2C * 60));
            }
        } else {
            if (baddie->unk3E8 < 0.0f) {
                baddie->unk3E8 = 0.0f;
            } else if (baddie->unk3E8 > 120.0f) {
                baddie->unk3E8 = (120.0f - (baddie->unk3E8 - 120.0f));
                baddie->unk3EC = -baddie->unk3EC;
            }
        }
    }
    if (fsa->hitpoints == 0) {
        return 0;
    }
    hitType = func_8002601C(obj, &sp50, &sp58, &damage, &hitX, &hitY, &hitZ);
    baddie->unk3F0 = (s8) sp58;
    if ((obj != NULL) && (hitType != 0) && (sp50 != NULL)) {
        switch (obj->id) {
        case OBJ_ScorpionRobot:
            if ((sp50->id != OBJ_sword) && (sp50->id != OBJ_staff) && (sp50->id != OBJ_projball)) {
                return 0;
            }
            break;
        case OBJ_WG_PollenCannon:
            if (sp50->id == OBJ_Pollen) {
                if (((Pollen_Data*)sp50->data)->unk12 == 0) {
                    sp50->opacity = 0;
                }
                return 0;
            }
            if (sp50->id == OBJ_PollenFragment) {
                sp50->opacity = 0;
                return 0;
            }
            break;
        }
    }
    
    if (hitType != 0) {
        damage *= 4;
        if (hitSRT != NULL) {
            hitSRT->transl.x = hitX + gWorldX;
            hitSRT->transl.y = hitY;
            hitSRT->transl.z = hitZ + gWorldZ;
        }
        if (hitDamageMap != 0) {
            if (hitDamageMap[hitType - 2] != -1) {
                damage = (s32) hitDamageMap[hitType - 2];
            }
        } else {
            damage = 0;
        }
        // STUBBED_PRINTF("%s hit by type %d for %d points\n", obj->def->name, hitType, damage); (default.dol)
        fsa->hitpoints -= damage;
        if (fsa->hitpoints <= 0) {
            baddie->unk3B2 |= 0x20;
            baddie->unk3E8 = 1.0f;
            baddie->unk3EC = 0.01f;
            fsa->logicState = hitLogicState;
            fsa->hitpoints = 0;
        } else if (damage != 0) {
            if ((fsa->target == NULL) && (((DLL_210_Player*)player->dll)->vtbl->func66(player, 1) != 0)) {
                fsa->target = player;
                fsa->unk33D = 0;
            }
            baddie->unk3E8 = 1.0f;
            baddie->unk3EC = 12.0f;
            if (hitAnimStateMap != 0) {
                if (hitAnimStateMap[hitType - 2] != -1) {
                    gDLL_18_objfsa->vtbl->set_anim_state(obj, fsa, hitAnimStateMap[hitType - 2]);
                    fsa->logicState = hitLogicState;
                }
            }
            fsa->lastHitType = (s8) hitType;
        }
        if (*arg7 != 0) {
            gDLL_6_AMSFX->vtbl->func_A1C(*arg7);
            *arg7 = 0;
        }
        obj_send_mesg(sp50, 0xE0001, obj, NULL);
    }
    return hitType;
}

// offset: 0x1D88 | func: 16 | export: 20
s32 BaddieControl_func_1D88(Object* arg0, ObjFSA_Data* arg1, Unk80009024 *arg2, s16 arg3, s8 *arg4, s16 arg5, s16 arg6, s16 arg7) {
    Object* sp64;
    u32 sp60;
    u32 sp5C;

    sp5C = NULL;
    while (obj_recv_mesg(arg0, &sp60, &sp64, (void*)&sp5C) != 0) {
        switch (sp60) {
            case 0x140001:
                gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_1E3_SharpClaw_Ah_Shuddup, MAX_VOLUME, NULL, NULL, 0, NULL);
                break;
            case 0x4:
                obj_send_mesg(sp64, 5, arg0, NULL);
                break;
            case 0xE0000:
                if (sp64 == arg1->target) {
                    arg1->logicState = arg5;
                    arg1->target = NULL;
                    arg1->unk33D = 0;
                }
                break;
            case 0xB:
                arg1->unk342 = sp5C;
                break;
            case 0x1:
            case 0xA0001:
                if (arg6 != arg1->logicState) {
                    BaddieControl_func_148C(arg0, arg1, arg2, arg3, arg4, arg5, arg7, 0, 1);
                    arg1->logicState = arg6;
                    arg1->unk33D = 0;
                    arg1->target = sp64;
                    return 1;
                }
                break;
            case 0x3:
                if (arg6 == arg1->logicState) {
                    arg1->unk33D = 0;
                    arg1->target = NULL;
                    arg1->logicState = arg5;
                    return 2;
                }
                break;
        }
    }

    return 0;
}

// offset: 0x1FAC | func: 17 | export: 14
// TODO: arg count might be off
void BaddieControl_func_1FAC(Object* arg0, Baddie* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    gDLL_2_Camera->vtbl->func14();
}

/*0x8*/ static Vec3f _data_8 = { 0.0f, 0.0f, 0.0f };
/*0x14*/ static f32 _data_14 = 0.0f;
/*0x18*/ static Vec3f _data_18 = { 0.0f, 0.0f, 0.0f };
/*0x24*/ static f32 _data_24 = 25.0f;

// offset: 0x2000 | func: 18 | export: 21
void BaddieControl_setup(Object* obj, Baddie_Setup* setup, Baddie* baddie, s32 arg3, s32 arg4, s32 arg5, u8 arg6, f32 arg7) {
    s32 sp4C[] = { 0x2 };
    u8 sp4B;
    u8 hitpoints;
    s32 sp3C;
    s32 sp38;

    sp4B = 1;
    baddie->objdata = (void*)(baddie + 1);
    baddie->unk3B6 = 0;
    sp38 = arg6 & 1;
    sp3C = (s32) arg6;
    if ((sp38 == 0) && !(arg6 & 0x20)) {
        obj_add_object_type(obj, OBJTYPE_4);
        obj_init_mesg_queue(obj, 4);
    }
    gDLL_18_objfsa->vtbl->func0(obj, &baddie->fsa, arg3, arg4);
    baddie->fsa.flags = 0;
    baddie->fsa.unk33D = 0;
    baddie->fsa.unk278 = 0.0f;
    baddie->fsa.unk27C = 0.0f;
    hitpoints = setup->quarterHitpoints;
    if (hitpoints != 0) {
        baddie->fsa.hitpoints = hitpoints * 4;
    } else {
        baddie->fsa.hitpoints = 6 * 4; // default to 6 HP
    }
    baddie->unk39E = setup->unk30;
    baddie->unk3A0 = setup->unk1A;
    baddie->unk3A2 = setup->unk1C;
    if (baddie->unk39E != -1) {
        main_set_bits(baddie->unk39E, 0);
    }
    if (sp3C & 2) {
        gDLL_27->vtbl->init(&baddie->fsa.unk4, DLL27FLAG_NONE, arg5 | DLL27FLAG_200000, DLL27MODE_1);
    } else {
        gDLL_27->vtbl->init(&baddie->fsa.unk4, DLL27FLAG_NONE, DLL27FLAG_NONE, DLL27MODE_DISABLED);
    }
    gDLL_27->vtbl->setup_hits_collider(&baddie->fsa.unk4, 1, &_data_18, &_data_24, 4);
    if (sp3C & 4) {
        gDLL_27->vtbl->setup_terrain_collider(&baddie->fsa.unk4, 1, &_data_8, &_data_14, &sp4B);
    }
    gDLL_27->vtbl->reset(obj, &baddie->fsa.unk4);
    baddie->unk3A8 = 0;
    baddie->unk3B0 = setup->unk2B;
    baddie->unk3E0 = setup->unk22;
    baddie->unk3B8 = setup->unk2F;
    baddie->nextWeaponID = setup->initialWeaponID;
    baddie->unk3BA = setup->unk28;
    obj->unkB0 |= baddie->unk3BA & 3;
    if (sp3C & 8) {
        baddie->unk3A4 = setup->unk20;
        baddie->unk3A6 = setup->unk1E;
    } else {
        baddie->unk3A4 = 0;
        baddie->unk3A6 = 0;
    }
    baddie->unk3B2 = 0;
    baddie->unk3E2 = setup->unk29 * 8;
    baddie->unk3B4 = 0;
    obj->srt.transl.x = setup->base.x;
    obj->srt.transl.y = setup->base.y;
    obj->srt.transl.z = setup->base.z;
    baddie->unk3E4 = arg7;
    obj->srt.yaw = setup->unk2A << 8;
    obj->opacity = 255;
    obj->unkAF &= ~0x8;
    baddie->unk39C = setup->unk18;
    if (baddie->unk39C != -1) {
        obj->unkDC = main_get_bits(baddie->unk39C);
    } else {
        obj->unkDC = 0;
    }
    if (gDLL_29_Gplay->vtbl->did_time_expire(setup->base.uID) == 0) {
        obj->unkDC = 1;
    }
    if (obj->unkDC != 0) {
        func_800267A4(obj);
        return;
    }
    func_8002674C(obj);
    if (setup->unk2E == -1) {
        obj->unkE0 = 1;
    } else {
        obj->unkE0 = 0;
    }
    if ((sp38 == 0) && !(sp3C & 0x20)) {
        func_80008DC0(&baddie->unk374);
        baddie->unk34C.unk26 = 4;
        baddie->unk34C.unk27 = 0x14;
    }
    if (sp3C & 0x10) {
        if ((baddie->unk3F8 == NULL) && !(sp3C & 0x20)) {
            baddie->unk3F8 = mmAlloc(sizeof(UnkCurvesStruct), ALLOC_TAG_TEST_COL, ALLOC_NAME("BaddieControlDLL"));
        }
        if (baddie->unk3F8 != NULL) {
            bzero(baddie->unk3F8, sizeof(UnkCurvesStruct));
        }
        if (gDLL_26_Curves->vtbl->func_4288(baddie->unk3F8, obj, (f32) baddie->unk3E2, sp4C, -1) == 0) {
            baddie->unk3B2 |= 8;
        }
    } else {
        baddie->unk3F8 = NULL;
    }
}

// offset: 0x24FC | func: 19 | export: 15
void BaddieControl_func_24FC(Object* arg0, Baddie* arg1, u8 arg2) {
    if (arg1->unk3A8 != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(arg1->unk3A8);
    }
    if (!(arg1->unk3B0 & arg2)) {
        if (arg1->unk3A6 != 0) {
            // "baddie->fightlockbreakmusic" (default.dol)
            gDLL_5_AMSEQ2->vtbl->free(arg0, arg1->unk3A6, 0, 0, 0);
        }
        if (arg1->unk3A4 != 0) {
            // "baddie->fightlockmusic" (default.dol)
            gDLL_5_AMSEQ2->vtbl->free(arg0, arg1->unk3A4, 0, 0, 0);
        }
    }
    func_80008E08(&arg1->unk374);
    if (arg1->unk3F8 != NULL) {
        mmFree(arg1->unk3F8);
        arg1->unk3F8 = NULL;
    }
}

// offset: 0x2624 | func: 20 | export: 22
void BaddieControl_change_weapon(Object *obj, Baddie *baddie) {
    s16 weaponObjIDs[] = { 
        OBJ_sword, 
        OBJ_staff, 
        OBJ_club, 
        OBJ_IceBall,
        OBJ_fishingnet
    };

    if ((baddie->nextWeaponID != baddie->weaponID) && (obj->opacity != 0)) {
        if (obj->linkedObject != NULL) {
            obj_destroy_object(obj->linkedObject);
            obj->linkedObject = NULL;
        }
        if (baddie->nextWeaponID > 0) {
            obj->linkedObject = obj_create(
                obj_alloc_create_info(sizeof(ObjSetup), weaponObjIDs[baddie->nextWeaponID - 1]), 
                OBJ_INIT_FLAG4, 
                -1, 
                -1, 
                obj->parent);
        }
        baddie->weaponID = baddie->nextWeaponID;
    }
}

// offset: 0x2718 | func: 21 | export: 23
f32 BaddieControl_get_health_ratio(Object* obj) {
    f32 healthRatio;
    Baddie_Setup *setup;
    Baddie *objdata;

    objdata = (Baddie*)obj->data;
    setup = (Baddie_Setup*)obj->setup;

    if ((setup->quarterHitpoints != 0) && (objdata->fsa.hitpoints != 0)) {
        healthRatio = (f32) objdata->fsa.hitpoints / (f32) (setup->quarterHitpoints * 4);
    } else {
        healthRatio = 0.0f;
    }
    return healthRatio;
}
