#include "dlls/engine/18_objfsa.h"
#include "sys/curves.h"
#include "sys/objanim.h"
#include "sys/objmsg.h"

// This is a modgfx interface?
DLL_INTERFACE(DLL_18_UnknownDLL) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
    /*0*/ void (*func0)(Object *, s32, SRT *, s32, s32, void *); // TODO: figure out exact sig
};

/*0x0*/ static SRT *_data_0 = NULL;

/*0x0*/ static s8 _bss_0;
/*0x1*/ static s8 _bss_1;
/*0x2*/ static s8 _bss_2;
/*0x4*/ static s16 _bss_4;
/*0x8*/ static f32 _bss_8;
/*0xC*/ static f32 _bss_C;
/*0x10*/ static s8 _bss_10;
/*0x14*/ static s32 _bss_14;
/*0x18*/ static s32 _bss_18;
/*0x1C*/ static s8 _bss_1C;

void objfsa_func_13E4(ObjFSA_Data *data, void *arg1);
void objfsa_func_1F64(Object *obj, ObjFSA_Data *data, f32 arg2, f32 arg3, f32 arg4, s32 arg5);
static void objfsa_run_anim_state(Object *obj, ObjFSA_Data *data, f32 arg2, ObjFSA_StateCallback *arg3);
static void objfsa_run_logic_state(Object *obj, ObjFSA_Data *data, f32 arg2, ObjFSA_StateCallback *arg3);
static void objfsa_func_1AC4(Object *obj, ObjFSA_Data *data);
static void objfsa_func_1C70(Object *obj, ObjFSA_Data *data, f32 arg2);
static void objfsa_func_1E30(Object *obj, ObjFSA_Data *data);

// offset: 0x0 | ctor
void objfsa_ctor(void *dll) { }

// offset: 0xC | dtor
void objfsa_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void objfsa_func_18(Object *obj, ObjFSA_Data *data, s32 arg2, s32 arg3) {
    u32 i;
    s8* var_v1;

    i = 0;
    var_v1 = (s8*)data;
    var_v1[0] = 0;
    while (i < sizeof(ObjFSA_Data)) {
        var_v1[i] = 0;
        i++;
    }
    data->unk264 = arg2;
    data->unk266 = arg3;
    data->logicState = 0;
    data->animState = 0;
    data->enteredAnimState = 1;
    data->enteredLogicState = 1;
    data->unk330 = -1;
    data->unk334 = -1;
    data->unk2B0 = 10.0f;
}

// offset: 0x90 | func: 1 | export: 1
void objfsa_tick(Object *obj, ObjFSA_Data *data, f32 arg2, f32 arg3, 
        ObjFSA_StateCallback *animStateCallbacks, ObjFSA_StateCallback *logicStateCallbacks) {
    f32 temp_fv0;
    f32 var_fv1;
    f32 temp_fv1;
    f32 temp_fv0_2;
    f32 temp_fa1;
    f32 temp_ft4;
    SRT* temp_v0_2;
    void *mesgArg;
    s8 sp2F;

    _bss_2 = 0;
    sp2F = data->unk341;
    
    if (data->target != NULL) {
        temp_fv0 = data->target->srt.transl.x - obj->srt.transl.x;
        temp_fv1 = data->target->srt.transl.z - obj->srt.transl.z;
        data->targetDist = sqrtf(SQ(temp_fv0) + SQ(temp_fv1));
    } else {
        data->targetDist = 0.0f;
    }
    if (obj->unk0xc0) {}
    if ((data->flags & 0x8000) && (obj->unk0xc0 == NULL)) {
        objfsa_run_logic_state(obj, data, arg2, logicStateCallbacks);
        data->logicStateTime += arg2;
        if ((f32) data->logicStateTime > 10000.0f) {
            data->logicStateTime = 10000;
        }
    }
    data->flags |= 0x8000;
    if (data->unk274 != NULL) {
        objfsa_func_1E30(obj, data);
    }
    objfsa_func_1AC4(obj, data);
    data->flags &= ~0x200000;
    data->unk341 = 0;
    _bss_1C = 0;
    data->flags &= ~0x80000;
    data->unk340 = 0;
    _bss_1 = 0;
    objfsa_run_anim_state(obj, data, arg2, animStateCallbacks);
    data->animStateTime += arg2;
    if ((f32) data->animStateTime > 10000.0f) {
        data->animStateTime = 10000;
    }
    _bss_8 = obj->srt.transl.x;
    _bss_C = obj->srt.transl.z;
    if (!(data->flags & 0x01000000)) {
        objfsa_func_1C70(obj, data, arg2);
    }
    temp_v0_2 = _data_0;
    if (temp_v0_2 != NULL) {
        temp_fa1 = temp_v0_2->transl.x - _bss_8;
        temp_ft4 = temp_v0_2->transl.z - _bss_C;
        temp_fv0_2 = sqrtf(SQ(temp_fa1) + SQ(temp_ft4));
        if (temp_fv0_2 < 10.0f) {
            temp_fv0 = obj->srt.transl.x - _bss_8;
            temp_fv1 = obj->srt.transl.z - _bss_C;
            var_fv1 = sqrtf(SQ(temp_fv0) + SQ(temp_fv1));
            if (var_fv1 < 0.1f) {
                var_fv1 = 0.1f;
            }
            if (temp_fv0_2 < 1.0f) {
                obj->srt.transl.x = temp_v0_2->transl.x;
                obj->srt.transl.z = temp_v0_2->transl.z;
            } else {
                if (temp_fv0_2 < var_fv1) {
                    var_fv1 = temp_fv0_2;
                }
                temp_fa1 /= temp_fv0_2;
                temp_ft4 /= temp_fv0_2;
                obj->srt.transl.x = (temp_fa1 * var_fv1) + _bss_8;
                obj->srt.transl.z = (temp_ft4 * var_fv1) + _bss_C;
            }
        }
    }
    _data_0 = NULL;
    if (!(data->flags & 0x01000000) && !(data->flags & 0x400000)) {
        gDLL_27->vtbl->func_1e8(obj, &data->unk4, arg2);
        gDLL_27->vtbl->func_5a8(obj, &data->unk4);
        gDLL_27->vtbl->func_624(obj, &data->unk4, arg3);
        if (data->unk4.unk25C & 0x10) {
            data->flags |= 0x40000;
        } else {
            data->flags &= ~0x40000;
        }
        if (data->flags & 0x800000) {
            if ((data->unk4.unk25C & 2) || (data->unk4.hitsTouchBits != 0)) {
                obj->speed.x = (obj->srt.transl.x - obj->objhitInfo->unk_0x10.x) / arg2;
                obj->speed.z = (obj->srt.transl.z - obj->objhitInfo->unk_0x10.z) / arg2;
            }
            data->flags &= ~0x800000;
        }
    }
    if (data->target != NULL && sp2F != data->unk341) {
        mesgArg = (void*)(s32)data->unk341;
        obj_send_mesg(data->target, 11, obj, mesgArg);
    }
}

// offset: 0x5E0 | func: 2 | export: 2
void objfsa_func_5E0(Object *obj, ObjFSA_Data *data, ObjFSA_StateCallback *arg2) {
    f32 sp24;
    f32 temp_fv0;
    f32 temp_fv0_2;
    f32 temp_fv1;

    if (data->unk340 & 1) {
        sp24 = fsin16_precise(obj->srt.yaw);
        temp_fv0 = fcos16_precise(obj->srt.yaw);
        if (data->unk340 & 8) {
            data->unk278 = (-obj->speed.z * temp_fv0) - (obj->speed.x * sp24);
            data->speed = data->unk278;
        } else {
            data->unk27C = (obj->speed.x * temp_fv0) - (obj->speed.z * sp24);
            data->unk278 = (-obj->speed.z * temp_fv0) - (obj->speed.x * sp24);
            if (data->unk340 & 4) {
                data->speed = sqrtf(SQ(obj->speed.x) + SQ(obj->speed.z));
            }
        }
        data->unk340 = 0;
        data->flags |= 0x80000;
        _bss_1C = 1;
        _bss_1 = 0;
        _bss_2 = 1;
        objfsa_run_anim_state(obj, data, delayFloat, arg2);
    }
}

// offset: 0x75C | func: 3 | export: 3
void objfsa_func_75C(SRT *srt) {
    _data_0 = srt;
}

// offset: 0x778 | func: 4 | export: 4
void objfsa_set_anim_state(Object *obj, ObjFSA_Data *data, s32 state) {
    ObjFSA_ExitCallback callback;

    if (state != data->animState) {
        callback = data->animExitAction;
        if (callback != NULL) {
            callback(obj, data);
            data->animExitAction = NULL;
        }
        data->prevAnimState = data->animState;
        data->animState = (s16) state;
    }
    data->animStateTime = 0;
    data->enteredAnimState = TRUE;
    data->unk341 = 0;
    data->unk340 = 0;
    data->unk34A = 0;
    data->unk270 = 0;
    if (obj->objhitInfo != NULL) {
        obj->objhitInfo->unk_0x61 = 0;
    }
}

// offset: 0x81C | func: 5
static void objfsa_run_anim_state(Object *obj, ObjFSA_Data *data, f32 arg2, ObjFSA_StateCallback *callbacks) {
    s32 prevState;
    s32 nextState;
    s32 stop;
    s32 transitionCount;
    u32 asyncTransition;
    s32 i;
    s32 _pad;
    UnkFunc_80024108Struct sp50;
    ObjFSA_StateCallback callback;

    asyncTransition = FALSE;
    transitionCount = 0;
    _bss_0 = 0;
    _bss_10 = 0;
    if (data->animState != data->prevAnimState) {
        data->enteredAnimState = TRUE;
        data->animStateTime = 0;
    }
    do {
        stop = FALSE;
        prevState = data->animState;
        callback = callbacks[data->animState];
        nextState = callback(obj, data, arg2);
        if (nextState > 0) {
            // Switch to next state (run synchronously)
            if (data->animExitAction != NULL) {
                data->animExitAction(obj, data);
                data->animExitAction = NULL;
            }
            data->prevAnimState = data->animState;
            data->animState = nextState - 1;
            data->enteredAnimState = TRUE;
            data->animStateTime = 0;
            data->unk341 = 0;
            data->unk340 = 0;
            data->unk34A = 0;
            data->unk270 = 0;
            if (obj->objhitInfo != NULL) {
                obj->objhitInfo->unk_0x61 = 0;
            }
        } else if (nextState < 0) {
            // Switch to next state (run asynchronously (i.e. on next tick))
            asyncTransition = TRUE;
            nextState = -nextState;
            if (nextState != prevState) {
                if (data->animExitAction != NULL) {
                    data->animExitAction(obj, data);
                    data->animExitAction = NULL;
                }
                data->prevAnimState = prevState;
                data->enteredAnimState = TRUE;
                data->animStateTime = 0;
                data->unk341 = 0;
                data->unk340 = 0;
                data->unk34A = 0;
                data->unk270 = 0;
                if (obj->objhitInfo != NULL) {
                    obj->objhitInfo->unk_0x61 = 0;
                }
            }
            data->animState = nextState;
            stop = TRUE;
        } else {
            stop = TRUE;
        }
        transitionCount += 1;
        // Bail after too many synchronous transitions
        if (transitionCount >= 256) {
            stop = TRUE;
        }
    } while (!stop);
    data->prevAnimState = data->animState;
    if (!asyncTransition) {
        data->enteredAnimState = FALSE;
    }
    if ((_bss_10 == 0) && !(data->unk340 & 1)) {
        sp50.unk1B = 0;
        data->unk33A = func_80024108(obj, data->unk298, arg2, &sp50);
        data->unk308 = 0;
        for (i = 0; i < sp50.unk1B; i++) {
            data->unk308 |= 1 << sp50.unk13[i];
        }
        func_80025780(obj, arg2, &sp50, NULL);
        data->flags &= ~0x10000;
    }
    if (!(data->flags & 0x4000)) {
        obj->srt.pitch -= (s32) ((f32) obj->srt.pitch * arg2 * 0.125f) & 0xFFFF & 0xFFFF;
        obj->srt.roll -= (s32) ((f32) obj->srt.roll * arg2 * 0.125f) & 0xFFFF & 0xFFFF;
    }
}

// offset: 0xB34 | func: 6
static void objfsa_run_logic_state(Object *obj, ObjFSA_Data *data, f32 arg2, ObjFSA_StateCallback *callbacks) {
    s32 prevState;
    s32 nextState;
    s32 stop;
    s32 transitionCount;
    u32 asyncTransition;
    ObjFSA_StateCallback callback;

    asyncTransition = FALSE;
    transitionCount = 0;
    if (data->logicState != data->prevLogicState) {
        data->enteredLogicState = TRUE;
        data->logicStateTime = 0;
    }
    do {
        prevState = data->logicState;
        stop = FALSE;
        callback = callbacks[data->logicState];
        nextState = callback(obj, data, arg2);
        if (nextState > 0) {
            // Switch to next state (run synchronously)
            data->prevLogicState = data->logicState;
            data->logicState = nextState - 1;
            data->enteredLogicState = TRUE;
            data->logicStateTime = 0;
        } else if (nextState < 0) {
            // Switch to next state (run asynchronously (i.e. on next tick))
            nextState = -nextState;
            asyncTransition = TRUE;
            if (nextState != prevState) {
                data->prevLogicState = prevState;
                data->enteredLogicState = TRUE;
                data->logicStateTime = 0;
            } else {
                data->enteredLogicState = FALSE;
            }
            data->logicState = nextState;
            stop = TRUE;
        } else {
            stop = TRUE;
        }
        transitionCount += 1;
        // Bail after too many synchronous transitions
        if (transitionCount >= 256) {
            stop = TRUE;
        }
    } while (!stop);
    data->prevLogicState = data->logicState;
    if (!asyncTransition) {
        data->enteredLogicState = FALSE;
    }
}

// offset: 0xC84 | func: 7 | export: 7
void objfsa_func_C84(Object *obj, ObjFSA_Data *data, f32 arg2, s32 arg3) {
    UnkFunc_80024108Struct sp34;
    s32 var_v1;

    sp34.unk12 = 0;
    data->unk33A = func_80024108(obj, data->unk298, arg2, &sp34);
    data->unk308 = 0;
    for (var_v1 = 0; var_v1 < sp34.unk1B; var_v1++) {
        data->unk308 |= 1 << sp34.unk13[var_v1];
    }
    func_80025780(obj, arg2, &sp34, NULL);
    data->flags &= ~0x10000;
    if (sp34.unk12 != 0) {
        if (arg3 & 0x10) {
            if (arg3 & 1) {
                data->unk2AC = -sp34.unk0[2];
            }
            if (arg3 & 2) {
                data->unk2AC = sp34.unk0[0];
            }
            if (arg3 & 4) {
                data->unk2AC = sp34.unk0[1];
            }
            if (arg3 & 8) {
                obj->srt.yaw += sp34.unkc[1];
            }
        } else {
            if (arg3 & 1) {
                data->unk278 = -sp34.unk0[2] / arg2;
            }
            if (arg3 & 2) {
                data->unk27C = sp34.unk0[0] / arg2;
            }
            if (arg3 & 8) {
                obj->srt.yaw += sp34.unkc[1];
            }
            if (arg3 & 4) {
                data->unk280 = sp34.unk0[1] / arg2;
                data->flags |= 0x10000;
            }
        }
    } else {
        data->unk278 = 0.0f;
        data->unk27C = 0.0f;
    }
    _bss_10 = 1;
}

// offset: 0xE60 | func: 8 | export: 8
void objfsa_func_E60(Object *obj, ObjFSA_Data *data, f32 arg2, f32 arg3, f32 arg4) {
    if (data->unk290 > 0.1f) {
        obj->srt.yaw += (((arg3 * arg2) / arg4) * 182.0f);
    }
}

// offset: 0xED4 | func: 9 | export: 9
void objfsa_func_ED4(Object *obj, ObjFSA_Data *data, f32 arg2, f32 arg3) {
    f32 temp_fa0;
    f32 var_fv0;

    var_fv0 = data->unk2A4 + (arg3 * arg2);
    if (var_fv0 > 1.0f) {
        var_fv0 = 1.0f;
    }
    temp_fa0 = var_fv0 - data->unk2A4;
    if (temp_fa0 > 0.0f) {
        obj->srt.transl.x += data->unk2EC.x * temp_fa0;
        obj->srt.transl.y += data->unk2EC.y * temp_fa0;
        obj->srt.transl.z += data->unk2EC.z * temp_fa0;
        data->unk2A4 = var_fv0;
    }
}

// offset: 0xF78 | func: 10 | export: 10
void objfsa_func_F78(Object *obj, ObjFSA_Data *data, f32 arg2, f32 arg3) {
    f32 temp_fa0;
    f32 var_fv0;

    var_fv0 = data->unk2A0 + (arg3 * arg2);
    if (var_fv0 > 1.0f) {
        var_fv0 = 1.0f;
    }
    temp_fa0 = var_fv0 - data->unk2A0;
    if (temp_fa0 > 0.0f) {
        obj->srt.yaw += (s16) (s32) (data->unk2F8 * temp_fa0);
        data->unk2A0 = var_fv0;
    }
}

// offset: 0x1008 | func: 11 | export: 11
void objfsa_func_1008(Object *obj, ObjFSA_Data *data, s32 arg2, s32 arg3) {
    f32 var_fv0;
    f32 var_fv1;
    s32 var_v1;

    if (data->target != NULL) {
        if (data->target->parent == obj->parent) {
            var_fv0 = data->target->srt.transl.x - obj->srt.transl.x;
            var_fv1 = data->target->srt.transl.z - obj->srt.transl.z;
        } else {
            var_fv0 = obj->positionMirror.x - data->target->positionMirror.x;
            var_fv1 = obj->positionMirror.z - data->target->positionMirror.z;
        }
        var_v1 = arctan2_f(-var_fv0, -var_fv1) - (obj->srt.yaw & 0xFFFF);
        if (var_v1 >= 0x8001) {
            var_v1 += 0xFFFF0001;
        }
        if (var_v1 < -0x8000) {
            var_v1 += 0xFFFF;
        }
        obj->srt.yaw += (s32) (((f32) var_v1 * delayFloat) / ((f32) arg3 * 3.0f));
    }
}

// offset: 0x1120 | func: 12 | export: 12
void objfsa_func_1120(Object *obj, ObjFSA_Data *data, s32 arg2, s32 arg3, ObjFSA_Func_11BC_Struct *arg4) {
    s32 temp_v0;

    temp_v0 = 1 << arg2;
    if (data->unk308 & temp_v0) {
        data->unk308 = data->unk308 & ~temp_v0;
        gDLL_6_AMSFX->vtbl->play_sound(obj, arg4[arg3].soundID, MAX_VOLUME, NULL, NULL, 0, NULL);
    }
}

// offset: 0x11BC | func: 13 | export: 13
void objfsa_func_11BC(Object *obj, ObjFSA_Data *data, s32 arg2, s32 arg3, ObjFSA_Func_11BC_Struct *arg4, f32 arg5, u8 volume) {
    s32 temp_v0;
    u32 temp_v0_2;

    temp_v0 = 1 << arg2;
    if (data->unk308 & temp_v0) {
        data->unk308 = data->unk308 & ~temp_v0;
        temp_v0_2 = gDLL_6_AMSFX->vtbl->play_sound(obj, arg4[arg3].soundID, volume, NULL, NULL, 0, NULL);
        if (temp_v0_2 != 0) {
            gDLL_6_AMSFX->vtbl->func_954(temp_v0_2, arg5);
        }
    }
}

// offset: 0x1280 | func: 14 | export: 14
void objfsa_func_1280(Object *obj, ObjFSA_Data *data, s32 arg2) {
    data->unk330 = gDLL_26_Curves->vtbl->curves_func_1e4(
        obj->srt.transl.x, obj->srt.transl.y, obj->srt.transl.z, 
        &arg2, 1, (s32) data->unk338);
}

// offset: 0x12F4 | func: 15 | export: 15
void objfsa_func_12F4(Object *obj, ObjFSA_Data *data, f32 arg2, f32 arg3) {
    CurveSetup* temp_v0;

    if (data->unk330 == -1) {
        data->unk2B4 = 0.0f;
        return;
    }
    temp_v0 = gDLL_26_Curves->vtbl->curves_func_39c(data->unk330);
    if (temp_v0 == NULL) {
        data->unk2B4 = 0.0f;
        return;
    }
    objfsa_func_1F64(obj, data, temp_v0->base.x, temp_v0->base.z, arg2, 1);
    if (data->unk2B4 < 20.0f) {
        objfsa_func_13E4(data, NULL);
    }
}

// offset: 0x13E4 | func: 16 | export: 22
void objfsa_func_13E4(ObjFSA_Data *data, void *arg1) { }

// offset: 0x13F4 | func: 17 | export: 16
void objfsa_func_13F4(Object *obj, ObjFSA_Data *data, f32 arg2, f32 arg3) {
    f32 var_fa0;
    f32 var_fa1;
    f32 sp2C;
    f32 temp_fv0_4;
    
    data->unk340 |= 1;
    if (_bss_1C == 0) {
        var_fa0 = -fsin16_precise(_bss_4) * data->unk290 * arg3;
        var_fa1 = -fcos16_precise(_bss_4) * data->unk290 * arg3;
        if (data->unk290 < 0.02f) {
            var_fa0 = 0.0f;
            var_fa1 = 0.0f;
        }
        obj->speed.x = obj->speed.x + (((var_fa0 - obj->speed.x) * arg2) / data->unk2B0);
        obj->speed.z = obj->speed.z + (((var_fa1 - obj->speed.z) * arg2) / data->unk2B0);
    } else {
        data->unk340 &= ~0x1;
    }
    data->speed = sqrtf(SQ(obj->speed.x) + SQ(obj->speed.z));
    if (data->speed < 0.04f) {
        data->speed = 0.0f;
        obj->speed.x = 0.0f;
        obj->speed.z = 0.0f;
    }
    sp2C = fsin16_precise(obj->srt.yaw);
    temp_fv0_4 = fcos16_precise(obj->srt.yaw);
    data->unk27C = (obj->speed.x * temp_fv0_4) - (obj->speed.z * sp2C);
    data->unk278 = (-obj->speed.z * temp_fv0_4) - (obj->speed.x * sp2C);
}

// offset: 0x15D0 | func: 18 | export: 17
void objfsa_func_15D0(Object *obj, ObjFSA_Data *data) {
    obj->speed.x = 0.0f;
    obj->speed.z = 0.0f;
    data->speed = 0.0f;
    data->unk278 = 0.0f;
    data->unk27C = 0.0f;
}

// offset: 0x1600 | func: 19 | export: 18
void objfsa_func_1600(Object *obj, ObjFSA_Data *data, s32 arg2, s32 arg3) {
    _bss_14 = arg2;
    _bss_18 = arg3;
}

// offset: 0x162C | func: 20 | export: 19
void objfsa_func_162C(Object *obj, ObjFSA_Data *data, s32 arg2, s32 arg3) {
    f32 sp34;
    f32 sp30;
    s32 var_a2;

    if (_bss_1C != 0) {
        if ((data->unk278 > 0.0f) && (_bss_14 != obj->curModAnimId)) {
            func_80023D30(obj, _bss_14, obj->animProgress, 0U);
            data->unk33A = 0;
        } else if ((data->unk278 < 0.0f) && (_bss_18 != obj->curModAnimId)) {
            func_80023D30(obj, _bss_18, obj->animProgress, 0U);
            data->unk33A = 0;
        }
        sp30 = sqrtf(SQ(data->unk278) + SQ(data->unk27C));
        if (func_8002493C(obj, sp30, &sp34) != 0) {
            data->unk298 = sp34;
        }
        if (sp30 != 0.0f) {
            sp34 = data->unk27C / sp30;
        } else {
            sp34 = 0.0f;
        }
        var_a2 = (s32) (sp34 * 1023.0f);
        if (var_a2 < 0) {
            var_a2 = -var_a2;
        }
        if (var_a2 >= 0x400) {
            var_a2 = 0x3FF;
        }
        if (data->unk27C > 0.0f) {
            func_80025540(obj, arg3, var_a2);
        } else {
            func_80025540(obj, arg2, var_a2);
        }
    }
}

// offset: 0x1824 | func: 21 | export: 20
void objfsa_func_1824(Object *obj, ObjFSA_Data *data, s32 arg2, s32 arg3, u32 arg5, u32 arg6) {
    DLL_18_UnknownDLL* temp_s3;

    temp_s3 = dll_load_deferred((arg2 + 0x1000), 1);
    while (arg3 != 0) {
        if (arg6 == 0) {
            temp_s3->vtbl->func0(obj, 0, 0, 1, -1, 0);
        } else if (arg6 == 1) {
            temp_s3->vtbl->func0(obj, 0, 0, 2, -1, 0);
        } else if (arg6 == 2) {
            temp_s3->vtbl->func0(obj, 0, 0, 4, -1, 0);
        }
        arg3--;
    }
    dll_unload(temp_s3);
}

// offset: 0x1978 | func: 22 | export: 21
void objfsa_func_1978(Object *obj, ObjFSA_Data *data, s32 arg2, s32 arg3, u32 arg4) {
    while ((arg3 != 0) && (obj != NULL)) {
        if (arg4 == 0) {
            gDLL_17->vtbl->func1(obj, arg2, NULL, 2, -1, NULL);
        } else if (arg4 == 1) {
            gDLL_17->vtbl->func1(obj, arg2, NULL, 2, -1, NULL);
        } else if (arg4 == 2) {
            gDLL_17->vtbl->func1(obj, arg2, NULL, 4, -1, NULL);
        }
        arg3 -= 1;
    }
}

// offset: 0x1AC4 | func: 23
static void objfsa_func_1AC4(Object *obj, ObjFSA_Data *data) {
    f32 temp_fv0;
    f32 temp_fv1;
    s32 var_v0;
    s32 var_v1;

    temp_fv0 = data->unk288;
    temp_fv1 = data->unk284;
    data->unk294 = data->unk290;
    data->unk290 = sqrtf((temp_fv0 * temp_fv0) + (temp_fv1 * temp_fv1));
    if (data->unk290 > 65.0f) {
        data->unk290 = 65.0f;
    }
    data->unk290 /= 65.0f;
    _bss_4 = arctan2_f(data->unk288, -data->unk284);
    _bss_4 -= data->unk324;
    var_v1 = _bss_4;
    var_v1 -= (obj->srt.yaw & 0xFFFF);
    if (var_v1 >= 0x8001) {
        var_v1 += 0xFFFF0001;
    }
    if (var_v1 < -0x8000) {
        var_v1 += 0xFFFF;
    }
    data->unk32A = (s16) (s32) ((f32) var_v1 / 182.04f);
    if (var_v1 < 0) {
        data->unk328 = -data->unk32A;
    } else {
        data->unk328 = data->unk32A;
    }
    var_v0 = 0x8000 - -var_v1;
    if (var_v0) {}
    var_v0 += 0x2000;
    if (data->unk290 < 0.05f) {
        data->unk33F = 0;
        return;
    }
    if (var_v0 < 0) {
        var_v0 += 0xFFFF;
    }
    if (var_v0 >= 0x10000) {
        var_v0 += 0xFFFF0001;
    }
    data->unk33F = 4 - (var_v0 / 16384);
}

// offset: 0x1C70 | func: 24
static void objfsa_func_1C70(Object *obj, ObjFSA_Data *data, f32 arg2) {
    SRT sp88;
    MtxF sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;

    if (!(data->flags & 0x02000000)) {
        if (!(data->flags & 0x200000)) {
            obj->speed.y *= 0.97f;
            obj->speed.y -= data->unk29C * arg2;
        }
        
        if (!(data->unk340 & 1) || (data->unk340 & 4)) {
            sp88.yaw = obj->srt.yaw;
            sp88.pitch = obj->srt.pitch;
            sp88.roll = 0;
            sp88.transl.x = 0;
            sp88.transl.y = 0;
            sp88.transl.z = 0;
            sp88.scale = 1.0f;
            matrix_from_srt(&sp48, &sp88);
            if (data->flags & 0x10000) {
                vec3_transform(&sp48, data->unk27C, data->unk280, -data->unk278, &sp40, &obj->speed.y, &sp3C);
            } else {
                vec3_transform(&sp48, data->unk27C, 0.0f, -data->unk278, &sp40, &sp44, &sp3C);
            }
            obj->speed.x = sp40;
            obj->speed.z = sp3C;
        }
        obj_integrate_speed(obj, obj->speed.x * arg2, obj->speed.y * arg2, obj->speed.z * arg2);
    }
}

// offset: 0x1E30 | func: 25
static void objfsa_func_1E30(Object *obj, ObjFSA_Data *data) {
    MtxF sp50;
    SRT sp38;

    sp38.yaw = obj->srt.yaw;
    sp38.pitch = obj->srt.pitch;
    sp38.roll = obj->srt.roll;
    sp38.transl.x = 0;
    sp38.transl.y = 0;
    sp38.transl.z = 0;
    sp38.scale = 1.0f;
    matrix_from_srt(&sp50, &sp38);
    
    vec3_transform(&sp50, 0.0f, 0.0f, 1.0f, &data->unk274[0].x, &data->unk274[0].y, &data->unk274[0].z);
    vec3_transform(&sp50, 0.0f, 1.0f, 0.0f, &data->unk274[1].x, &data->unk274[1].y, &data->unk274[1].z);
    vec3_transform(&sp50, 1.0f, 0.0f, 0.0f, &data->unk274[2].x, &data->unk274[2].y, &data->unk274[2].z);
}

// offset: 0x1F64 | func: 26 | export: 5
void objfsa_func_1F64(Object *obj, ObjFSA_Data *data, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    f32 temp_fv1;
    f32 temp_fa1;
    f32 var_ft4;
    f32 temp;

    data->flags &= ~0x100000;
    temp_fv1 = obj->srt.transl.x - arg2;
    temp_fa1 = obj->srt.transl.z - arg3;
    data->unk2B4 = sqrtf(SQ(temp_fv1) + SQ(temp_fa1));
    temp = data->unk2B4;
    if (1) {}
    var_ft4 = 65.0f;
    if (data->unk2B4 < 15.0f) {
        var_ft4 = data->unk2B4 * 3.0f;
        data->speed *= 0.9f;
    }
    if (var_ft4 < temp) {
        temp /= var_ft4;
        temp_fv1 /= temp;
        temp_fa1 /= temp;
    }
    data->unk288 = temp_fv1 * arg4;
    data->unk284 = -temp_fa1 * arg4;
    if (data->unk288 > 65.0f) {
        data->unk288 = 65.0f;
    }
    if (data->unk288 < -65.0f) {
        data->unk288 = -65.0f;
    }
    if (data->unk284 > 65.0f) {
        data->unk284 = 65.0f;
    }
    if (data->unk284 < -65.0f) {
        data->unk284 = -65.0f;
    }
}

// offset: 0x20CC | func: 27 | export: 6
void objfsa_func_20CC(Object *obj, ObjFSA_Data *data, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6) {
    f32 temp_fv1;
    f32 temp_fa1;
    f32 temp_fv0;

    temp_fv1 = obj->srt.transl.x - arg2;
    temp_fa1 = obj->srt.transl.z - arg3;
    temp_fv0 = sqrtf((temp_fv1 * temp_fv1) + (temp_fa1 * temp_fa1));
    data->unk2B4 = temp_fv0;
    if (temp_fv0 != 0.0f) {
        temp_fv1 /= temp_fv0;
        temp_fa1 /= temp_fv0;
    }
    if ((arg4 + arg5) < data->unk2B4) {
        data->unk288 = temp_fv1 * arg6;
        data->unk284 = -temp_fa1 * arg6;
    } else {
        data->speed *= 0.9f;
        data->unk288 = 0.0f;
        data->unk284 = 0.0f;
    }
    if (data->unk288 > 65.0f) {
        data->unk288 = 65.0f;
    }
    if (data->unk288 < -65.0f) {
        data->unk288 = -65.0f;
    }
    if (data->unk284 > 65.0f) {
        data->unk284 = 65.0f;
    }
    if (data->unk284 < -65.0f) {
        data->unk284 = -65.0f;
    }
}
