#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/210_player.h"
#include "game/gamebits.h"
#include "sys/objects.h"
#include "sys/objmsg.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "dll.h"
#include "macros.h"

typedef struct {
    ObjSetup base;
    u8 _unk18[0x1A - 0x18];
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
} WindLift_Setup;

// size: 0x18
typedef struct {
    Object* unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    u8 unk10;
    u8 unk11;
    s32 unk14;
} WindLift_Data_1C;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 _unkC[0x10 - 0xC];
    u32 unk10;
    f32 unk14;
    s32 unk18;
    WindLift_Data_1C unk1C[14];
    u8 _unk16C[0x170 - 0x16C];
    u32 unk170_0 : 1;
} WindLift_Data;

static void WindLift_func_880(Object*, Object*, WindLift_Data_1C*, f32, s32, s32, s32);
static void WindLift_func_F0C(WindLift_Data_1C* arg0, s32 arg1);

// offset: 0x0 | ctor
void WindLift_ctor(void* dll) { }

// offset: 0xC | dtor
void WindLift_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void WindLift_obj_Setup(Object* self, WindLift_Setup* setup, s32 reset) {
    s32 i;
    WindLift_Data* objdata = self->data;

    objdata->unk4 = setup->unk1E;
    objdata->unk0 = objdata->unk4 - 88;
    objdata->unk8 = setup->unk1C;
    objdata->unk18 = 0;
    objdata->unk14 = 2.0f;
    if ((mainGetBits(BIT_57) != 0) || (objdata->unk0 >= 4)) {
        objdata->unk18 = 60;
    }
    objdata->unk10 = 0;
    for (i = 0; i < 14; i++) {
        WindLift_func_F0C(&objdata->unk1C[i], objdata->unk0);
    }
}

// offset: 0xF8 | func: 1 | export: 1
void WindLift_obj_Control(Object* self) {
    WindLift_Data* objdata;
    WindLift_Setup* setup;
    Object* player;
    Object** objs;
    f32 sp6C;
    s32 sp68;
    s32 numObjs;
    s32 j;
    s32 slot;
    s32 i;
    s32 k;

    objdata = self->data;
    setup = (WindLift_Setup*)self->setup;
    if ((mainGetBits(BIT_57) != 0) || (objdata->unk0 >= 5)) {
        if ((objdata->unk0 == 2) && (mainGetBits(BIT_476) == 0)) {
            if (mainGetBits(BIT_5D) != 0) {
                mainSetBits(BIT_476, 1);
            }
        } else {
            if (((objdata->unk18++) < 60) && (mainGetBits(objdata->unk4) == 0)) {
                self->srt.yaw -= (s32) (gUpdateRate * 100 * objdata->unk18 * objdata->unk18) / 60;
                if (objdata->unk10 == 0) {
                    dll_amSfx->Play(self, SOUND_124, MAX_VOLUME, &objdata->unk10, NULL, 0, NULL);
                } else {
                    dll_amSfx->SetPitch(objdata->unk10, ((f32) objdata->unk18 / 60.0f) + 1.0f);
                }
                objSetModel(self, 0);
                return;
            }
            objSetModel(self, 1);
            sp68 = mainGetBits(objdata->unk8);
            if (sp68 != 0) {
                self->srt.yaw -= gUpdateRate * 0x9F4;
            } else {
                self->srt.yaw -= gUpdateRate * 0xCCC;
            }
            if (objdata->unk10 == 0) {
                dll_amSfx->Play(self, SOUND_124, MAX_VOLUME, &objdata->unk10, NULL, 0, NULL);
            } else {
                if (sp68 != 0) {
                    objdata->unk14 = (f32) (objdata->unk14 + ((1.0f - objdata->unk14) * 0.03125f));
                } else {
                    objdata->unk14 = (f32) (objdata->unk14 + ((2.0f - objdata->unk14) * 0.03125f));
                }
                dll_amSfx->SetPitch(objdata->unk10, objdata->unk14);
            }
            sp6C = (f32) setup->unk1A;
            player = objGetPlayer();
            if (mainGetBits(objdata->unk4) != 0) {
                if (objdata->unk170_0 == 0) {
                    objdata->unk170_0 = 1;
                    gDLL_5_AMSEQ2->vtbl->set(player, 0x11F, STUBBED_STR("windlift.c"), 0, AMSEQ_DEBUG_STR("AMB_WINDLIFT"));
                }
                if (player != NULL) {
                    WindLift_func_880(self, player, &objdata->unk1C[0], sp6C, sp68, TRUE, objdata->unk0);
                }
            } else {
                if (objdata->unk170_0 != 0) {
                    gDLL_5_AMSEQ2->vtbl->set(player, 0x128, STUBBED_STR("windlift.c"), 0, AMSEQ_DEBUG_STR("AMB_WINDLIFTOUT"));
                    objdata->unk170_0 = 0;
                }
                if (objdata->unk1C[0].unk10 & 0xE0) {
                    ((DLL_210_Player*)player->dll)->vtbl->func58(player, 0);
                    if (objdata->unk1C[0].unk10 & 0xE) {
                        objdata->unk1C[0].unk10 |= 2;
                    }
                    objdata->unk1C[0].unkC = 0.0f;
                    objdata->unk1C[0].unk11 = 0;
                    objdata->unk1C[0].unk10 &= ~0xF1;
                }
            }
            objs = objGetAllOfType(OBJTYPE_WindLiftable, &numObjs);
            numObjs += 2;
            if (numObjs > 14) {
                numObjs = 14;
            }
            i = 2;
            while (i < 14) {
                i++;
                objdata->unk1C[i - 1].unk14 = -1;
            }
            for (i = 2; i < numObjs; i++) {
                slot = -1;
                for (j = 2; j < 14; j++) {
                    if (objdata->unk1C[j].unk0 == *objs) {
                        slot = j;
                    }
                }
                if (slot == -1) {
                    for (j = 2; j < 14; j++) {
                        if (objdata->unk1C[j].unk0 == NULL) {
                            slot = j;
                            WindLift_func_F0C(&objdata->unk1C[j], objdata->unk0);
                            j = 2000; // rare use the 'break' keyword challenge (impossible)
                        }
                    }
                    if (slot != -1) {
                        objdata->unk1C[slot].unk0 = *objs;
                    } else {
                        return;
                    }
                }
                objdata->unk1C[slot].unk14 = slot;
                if (!((*objs)->stateFlags & 0x1000) && ((*objs) != NULL)) {
                    WindLift_func_880(self, *(objs++), &objdata->unk1C[slot], sp6C, sp68, FALSE, objdata->unk0);
                }
            }
            for (k = 2; k < 14; k++) {
                if (objdata->unk1C[k].unk14 == -1) {
                    objdata->unk1C[k].unk0 = NULL;
                }
            }
        }
    }
}

// offset: 0x7A0 | func: 2 | export: 2
void WindLift_obj_Update(Object* self) { }

// offset: 0x7AC | func: 3 | export: 3
void WindLift_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x800 | func: 4 | export: 4
void WindLift_obj_Free(Object* self, s32 onlySelf) {
    WindLift_Data* objdata = self->data;

    if (objdata->unk10 != 0) {
        dll_amSfx->Stop(objdata->unk10);
    }
}

// offset: 0x85C | func: 5 | export: 5
u32 WindLift_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x86C | func: 6 | export: 6
u32 WindLift_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(WindLift_Data);
}

// offset: 0x880 | func: 7
static void WindLift_func_880(Object* self, Object* obj, WindLift_Data_1C* arg2, f32 arg3, s32 arg4, s32 isPlayer, s32 arg6) {
    Object* player;
    f32 objHeight;
    f32 temp_fv0_2;
    f32 var_fa0;
    f32 xzDist;
    f32 var_ft5;
    f32 var_fv0_2;
    s32 temp;

    player = objGetPlayer();
    objHeight = obj->srt.transl.y - self->srt.transl.y;
    if (objHeight < 0.0f) {
        return;
    }
    xzDist = vec3DistanceXZ(&obj->globalPosition, &self->globalPosition);
    if (!(xzDist > 113.0f) || (arg2->unk10 & 0xE0)) {
        if (!(arg2->unk10 & 0x80) || arg4 == 0) {
            if (xzDist < 110.0f) {
                if (!(arg2->unk10 & 0xE0) || (arg2->unk10 & 0x80)) {
                    if (((!arg2->unk10) & 0x80) && (objHeight < 20.0f)) { // @bug? should probably be !(arg2->unk10 & 0x80)
                        arg2->unk10 |= 0x80;
                        return;
                    }
                    if (arg2->unk10 & 2) {
                        if ((objHeight / arg3) > 0.8f) {
                            arg2->unk10 |= 4;
                            arg2->unk10 &= ~0x8;
                        } else {
                            arg2->unk10 |= 8;
                            arg2->unk10 &= ~0x4;
                        }
                        arg2->unk10 &= ~0x2;
                    }
                    if (arg4 == 0) {
                        arg2->unk10 |= 0x40;
                        arg2->unk10 &= ~0x20;
                        temp = (((s32) (arg2->unk10 & 0xE0) >> 4) << 8) | arg6;
                        objSendMesg(obj, 0xF, self, (void* ) (temp));
                        arg2->unk10 &= ~0x80;
                    } else {
                        arg2->unk10 |= 0x20;
                        arg2->unk10 &= ~0x40;
                    }
                    do { } while (0); // @fake
                }
                var_ft5 = 0.324f;
                if ((arg2->unk10 & 0xE) && (arg2->unk10 & 8) && (arg4 == 0)) {
                    arg3 *= 0.6f;
                }
                arg3 *= 0.6f;
                if (!(arg3 <= 10.0f)) {
                    if (objHeight < 3.0f) {
                        objHeight = 3.0f;
                    }
                    if (arg4 == 0) {
                        temp_fv0_2 = arg3 - (arg2->unkC * arg2->unkC * arg2->unkC * (arg3 / 50.0f));
                        var_fa0 = (temp_fv0_2 - objHeight) < 0.0f
                            ? 0.0f
                            : (temp_fv0_2 - objHeight) > 20
                                ? 1.0f
                                : (temp_fv0_2 - objHeight) / 20;
                        if (arg2->unk10){} // @fake
                        arg2->unk10 |= 1;
                        if ((arg2->unkC < -0.2f && (arg2->unk11 % 2)) || (arg2->unkC > 0.2f && !(arg2->unk11 % 2))) {
                            if (arg2->unk10 & 8) {
                                if (arg2->unk11++ > 2) {
                                    arg2->unk10 &= ~0x8;
                                    arg2->unk10 |= 4;
                                }
                            }
                        }
                    } else {
                        if (arg2->unk10 & 0xE) {
                            var_fv0_2 = 0.1f;
                        } else {
                            var_fv0_2 = 0.5f;
                        }
                        if (var_fv0_2 < arg2->unkC) {
                            arg2->unk11 = 1;
                        }
                        var_ft5 *= 1.5f;
                        if (arg2->unk11 == 0) {
                            if (arg2->unk10 & 0xE) {
                                var_fa0 = 1.0f - (objHeight / (arg3 * 1.55f));
                            } else {
                                var_fa0 = 1.0f - (objHeight / (arg3 * 1.1f));
                            }
                            if (var_fa0 < 0) {
                                var_fa0 = 0.0f;
                            }
                            var_fa0 *= var_fa0;
                        } else {
                            var_fa0 = 0.01f;
                        }
                    }
                    arg2->unk8 = (var_ft5 * var_fa0) - 0.18f;
                    arg2->unkC += arg2->unk8;
                    if (arg2->unkC > 8.0f) {
                        arg2->unkC = 8.0f;
                    }
                    if (arg2->unkC == 0.0f) {
                        arg2->unkC = -0.001f;
                    }
                    if ((objHeight < 20.0f) && (arg4 != 0)) {
                        arg2->unk11 = 0;
                        arg2->unkC = 0.0f;
                        temp = (((s32) (arg2->unk10 & 0xE0) >> 4) << 8) | arg6;
                        objSendMesg(obj, 0x10, self, (void* ) (temp));
                        arg2->unk10 |= 0x80;
                        if (isPlayer) {
                            player->velocity.y = 0.0f;
                        }
                    }
                    if (isPlayer) {
                        ((DLL_210_Player*)obj->dll)->vtbl->func58(obj, arg2->unkC);
                        return;
                    }
                    obj->srt.transl.y += arg2->unkC * gUpdateRateF;
                    obj->velocity.y = arg2->unkC * gUpdateRateF;
                }
            } else {
                if (isPlayer) {
                    ((DLL_210_Player*)obj->dll)->vtbl->func58(obj, 0);
                }
                if (!isPlayer && (xzDist < 113.0f)) {
                    temp = (((s32) (arg2->unk10 & 0xE0) >> 4) << 8) | arg6;
                    objSendMesg(obj, 0x10, self, (void* ) (temp));
                    arg2->unk10 &= ~0xF1;
                    if (arg2->unk10 & 0xE) {
                        arg2->unk10 |= 2;
                    }
                    arg2->unkC = 0.0f;
                    arg2->unk11 = 0;
                }
            }
        }
    }
}

// offset: 0xF0C | func: 8
static void WindLift_func_F0C(WindLift_Data_1C* arg0, s32 arg1) {
    arg0->unk10 = 0;
    arg0->unk10 &= ~0xF1;
    arg0->unk10 |= ((arg1 == 1) || (arg1 == 2)) ? 2 : 0;
    arg0->unk4 = 0.1f;
    arg0->unk0 = NULL;
    arg0->unk11 = 0;
    arg0->unkC = 0.0f;
    arg0->unk8 = 0.0f;
}

/*0x0*/ static const char str_0[] = " Object List Full ";
/*0x14*/ static const char str_14[] = " Moving on Up ";
