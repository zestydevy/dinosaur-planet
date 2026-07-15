#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "game/gamebits.h"
#include "sys/gfx/animseq.h"
#include "sys/dll.h"
#include "sys/gfx/modgfx.h"
#include "sys/main.h"
#include "sys/map_enums.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "sys/objprint.h"
#include "sys/rand.h"
#include "sys/segment_1460.h"
#include "dll.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
} NWSH_Shrine_Data;

typedef struct {
    ObjSetup base;
    u8 _unk18[0x1A - 0x18];
    s16 unk1A;
} NWSH_Shrine_Setup;

/*0x0*/ static Texture *_data_0 = NULL;

static int dll_481_func_C10(Object *self, Object *a1, AnimObj_Data *a2, s8 a3);
static void dll_481_func_1084(Object* self);

// offset: 0x0 | ctor
void dll_481_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_481_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_481_setup(Object *self, NWSH_Shrine_Setup *setup, s32 arg2) {
    NWSH_Shrine_Data *objdata;
    DLL_IModgfx *sp30;

    objdata = self->data;
    self->srt.yaw = 0;
    objdata->unk0 = 0xA;
    if (setup->unk1A > 0) {
        objdata->unk0 = (s16) (setup->unk1A >> 2);
    }
    objdata->unk12 = 0;
    objdata->unk13 = 0;
    objdata->unk2 = 0;
    self->animCallback = dll_481_func_C10;
    objInitMesgQueue(self, 4);
    mainSetBits(BIT_DB_Entered_Shrine_3, 1);
    mainSetBits(BIT_15F, 0);
    mainSetBits(BIT_DB_Entered_Shrine_1, 1);
    mainSetBits(BIT_DB_Entered_Shrine_2, 1);
    mainSetBits(BIT_DB_Triggered_In_Shrine_Spirit_Cutscene, 0);
    objdata->unk17 = 0;
    objdata->unk4 = 0xC;
    objdata->unk8 = 0x1E;
    objdata->unk2 = 0xC8;
    gDLL_5_AMSEQ->vtbl->play_ex(2, 0x2B, 0x50, 1, 0);
    objdata->unk6 = 0;
    objdata->unkA = 0;
    objdata->unk15 = 0;
    objdata->unk16 = 0;
    objdata->unkE = 0x12C;
    objdata->unk10 = 0x514;
    sp30 = dllLoad(DLL_ID_122, 1);
    objdata->unkC = sp30->vtbl->func0(self, 3, 0, 0x402, -1, 0);
    dllFree(sp30);
}

// offset: 0x1E8 | func: 1 | export: 1
void dll_481_control(Object *self) {
    NWSH_Shrine_Data *objdata;
    Object *player;
    Object *temp_v0_4;
    f32 var_fv0;
    f32 sp3C;
    s16 var_v0;

    objdata = self->data;
    player = objGetPlayer();
    sp3C = 1000.0f;
    self->globalPosition.x = self->srt.transl.x;
    self->globalPosition.y = self->srt.transl.y;
    self->globalPosition.z = self->srt.transl.z;
    dll_481_func_1084(self);
    mainSetBits(BIT_DB_Entered_Shrine_2, 1);
    if (objdata->unk6 != 0) {
        objdata->unk4 += objdata->unk6;
        if (objdata->unk4 < 0xD) {
            objdata->unk4 = 0xC;
            objdata->unk6 = 0;
        } else if (objdata->unk4 >= 0x46) {
            objdata->unk4 = 0x46;
            objdata->unk6 = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(2, objdata->unk4);
    }
    if (objdata->unkA != 0) {
        objdata->unk8 += objdata->unkA;
        if ((objdata->unk8 < 2) && (objdata->unkA <= 0)) {
            objdata->unk8 = 1;
            objdata->unkA = 0;
        } else if ((objdata->unk8 >= 0x46) && (objdata->unkA >= 0)) {
            objdata->unk8 = 0x46;
            objdata->unkA = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(3, objdata->unk8);
    }
    if (objdata->unk2 > 0) {
        objdata->unk2 -= gUpdateRate;
        if (objdata->unk2 <= 0) {
            objdata->unk2 = 0;
            if (objdata->unk15 == 0) {
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x34, 0x50, objdata->unk8, 0);
                objdata->unk15 = 1;
            }
        }
        if ((objdata->unk12 == 2) && (objdata->unk2 < 0x29) && (objdata->unk16 == 0)) {
            objdata->unk16 = 1;
        }
    } else {
        temp_v0_4 = objGetNearestTypeTo(OBJTYPE_Door, player, &sp3C);
        if ((temp_v0_4 != NULL) && (sp3C < 300.0f) && (sp3C > 100.0f)) {
            var_fv0 = temp_v0_4->srt.transl.z - player->srt.transl.z;
            if (var_fv0 <= 0.0f) {
                if (var_fv0 < 0.0f) {
                    var_fv0 *= -1.0f;
                }
                if (objdata->unk8 != 0x1E) {
                    objdata->unk8 = 0x1E;
                }
                var_v0 = ((f32) objdata->unk8 * ((var_fv0 - 100.0f) / 200.0f));
                if (var_v0 <= 0) {
                    var_v0 = 1;
                }
                gDLL_5_AMSEQ->vtbl->set_volume(3, var_v0);
                var_v0 = ((f32) objdata->unk4 * ((200.0f - (var_fv0 - 100.0f)) / 200.0f));
                if (var_v0 <= 0) {
                    var_v0 = 1;
                }
                gDLL_5_AMSEQ->vtbl->set_volume(2, var_v0);
            }
        }
        switch (objdata->unk12) {
        case 0:
            if (objdata->unkE <= 0) {
                mainSetBits(BIT_176, 1);
                camSetShakeOffset(1.0f);
                objdata->unkE = mathRnd(0x64, 0x96);
                objdata->unk10 = 0x91;
            } else {
                if (objdata->unk10 != -0x3E7) {
                    if (objdata->unk10 < 0) {
                        dll_amSfx->Play(NULL, 0x3B9, 0x46, NULL, NULL, 0, NULL);
                        objdata->unk10 = -0x3E7;
                    } else {
                        objdata->unk10 -= gUpdateRate;
                    }
                }
                mainSetBits(BIT_176, 0);
                objdata->unkE -= gUpdateRate;
            }
            if (vec3Distance(&self->globalPosition, &player->globalPosition) < (f32) objdata->unk0) {
                mainSetBits(BIT_5C6, 1);
                objdata->unk12 = 1;
                mainSetBits(BIT_DB_Entered_Shrine_3, 0);
                objdata->unk13 = 1;
                gDLL_14_Modgfx->vtbl->func7(&objdata->unkC);
            }
        default:
            return;
        case 1:
            if (objdata->unk13 == 1) {
                objdata->unk12 = 2;
                return;
            }
            break;
        case 2:
            gDLL_3_Animation->vtbl->start_obj_sequence(3, self, -1);
            objdata->unk12 = 3;
            return;
        case 7:
            gDLL_3_Animation->vtbl->start_obj_sequence(4, self, -1);
            break;
        case 8:
            gDLL_3_Animation->vtbl->start_obj_sequence(5, self, -1);
            gDLL_5_AMSEQ->vtbl->play_ex(3, 0x35, 0x50, (s16) (u8) objdata->unk8, 0);
            mainSetBits(BIT_15F, 0);
            mainSetBits(BIT_DB_Entered_Shrine_2, 0);
            mainSetBits(BIT_DB_Entered_Shrine_3, 1);
            mainSetBits(BIT_5BE, 0);
            mainSetBits(BIT_1CB, 0);
            mainSetBits(BIT_5C6, 0);
            objdata->unk12 = 6;
            return;
        case 4:
            if (mainGetBits(BIT_SP_Replay_Disk_WM) != 0) {
                objdata->unk8 = 1;
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x34, 0x50, (s16) (u8) objdata->unk8, 0);
                objdata->unkA = 1;
                mainSetBits(BIT_DB_Entered_Shrine_3, 1);
                objdata->unk12 = 6;
                return;
            }
            mainSetBits(BIT_DB_Entered_Shrine_1, 0);
            gDLL_5_AMSEQ->vtbl->play_ex(3, 0x34, 0x50, (s16) (u8) objdata->unk8, 0);
            objdata->unkA = 1;
            gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
            objdata->unk12 = 5;
            return;
        case 5:
            if (mainGetBits(BIT_Shrine_Do_Exit_Warp) == 0) {
                mainSetBits(BIT_Shrine_Do_Exit_Warp, 1);
            }
            mainSetBits(BIT_15F, 0);
            mainSetBits(BIT_DB_Entered_Shrine_2, 0);
            mainSetBits(BIT_DB_Entered_Shrine_3, 1);
            objdata->unk12 = 6;
            mainSetBits(BIT_DB_Entered_Shrine_1, 1);
            mainSetBits(BIT_SP_Replay_Disk_WM, 1);
            gDLL_29_Gplay->vtbl->set_act(MAP_WARLOCK_MOUNTAIN, 9);
            break;
        case 6:
            if (mainGetBits(BIT_5C2) == 0) {
                mainSetBits(BIT_5C2, 1);
            }
            mainSetBits(BIT_5C6, 0);
            break;
        }
    }
}

// offset: 0xB08 | func: 2 | export: 2
void dll_481_update(Object *self) { }

// offset: 0xB14 | func: 3 | export: 3
void dll_481_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
}

// offset: 0xB5C | func: 4 | export: 4
void dll_481_free(Object* self, s32 a1) {
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_5_AMSEQ->vtbl->set_volume(3, 0);
    gDLL_5_AMSEQ->vtbl->set_volume(2, 0);
}

// offset: 0xBEC | func: 5 | export: 5
u32 dll_481_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xBFC | func: 6 | export: 6
u32 dll_481_get_data_size(Object *self, u32 a1) {
    return sizeof(NWSH_Shrine_Data);
}

// offset: 0xC10 | func: 7
int dll_481_func_C10(Object *self, Object *a1, AnimObj_Data *a2, s8 a3) {
    NWSH_Shrine_Data *objdata = self->data;
    a2->unk7A = -1;
    a2->unk62 = 0;
    dll_481_func_1084(self);
    if (objdata->unkA != 0) {
        objdata->unk8 += objdata->unkA;
        if ((objdata->unk8 < 2) && (objdata->unkA <= 0)) {
            objdata->unk8 = 1;
            objdata->unkA = 0;
        } else if ((objdata->unk8 >= 0x46) && (objdata->unkA >= 0)) {
            objdata->unk8 = 0x46;
            objdata->unkA = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(3, objdata->unk8);
    }
    if (a2->lastMessage != 0) {
        switch (a2->lastMessage) {
        case 1:
            func_80000860(self, self, 0xD5, 0);
            break;
        case 2:
            if (D_80092A7C[0] == -1) {
                func_80000860(self, self, 0x14, 0);
            } else {
                func_80000860(self, self, D_80092A7C[0], 0);
            }
            break;
        case 3:
            objdata->unk13 = 1;
            break;
        case 4:
            objdata->unk12 = 4;
            break;
        case 5:
            objdata->unk12 = 5;
            objdata->unk13 = 2;
            mainSetBits(BIT_DB_Entered_Shrine_3, 1);
            break;
        case 6:
            objdata->unk12 = 4;
            objdata->unk13 = 3;
            mainSetBits(BIT_DB_Entered_Shrine_3, 1);
            break;
        case 7:
            mainSetBits(BIT_15F, 1);
            break;
        case 8:
            mainSetBits(BIT_15F, 0);
            objdata->unkA = -3;
            break;
        case 10:
            mainSetBits(BIT_DB_Triggered_In_Shrine_Spirit_Cutscene, 1);
            if (_data_0 == NULL) {
                _data_0 = blockTexanimGetTex(1);
            }
            break;
        case 9:
            mainSetBits(BIT_DB_Entered_Shrine_2, 1);
            break;
        case 11:
            objdata->unk8 = 0x64;
            gDLL_5_AMSEQ->vtbl->play_ex(3, 0x34, 0x50, (s16) (u8) objdata->unk8, 0);
            break;
        case 13:
            gDLL_3_Animation->vtbl->set_camera_module(DLL_ID_CAMSTATIC, 0x65, 0, 0x50);
            break;
        }
    }
    a2->lastMessage = 0;
    if (objdata->unk12 == 3) {
        if (mainGetBits(BIT_5BE) != 0) {
            objdata->unk12 = 7;
            gDLL_3_Animation->vtbl->end_obj_sequence(a2->seqSlot);
        } else if (mainGetBits(BIT_1CB) != 0) {
            objdata->unk12 = 8;
            gDLL_3_Animation->vtbl->end_obj_sequence(a2->seqSlot);
        } else if (objdata->unkE <= 0) {
            mainSetBits(BIT_176, 1);
            mainSetBits(BIT_1CA, 1);
            camSetShakeOffset(1.0f);
            objdata->unkE = mathRnd(0xC8, 0xFA);
        } else {
            mainSetBits(BIT_176, 0);
            mainSetBits(BIT_1CA, 0);
            objdata->unkE -= gUpdateRate;
        }
    }
    return 0;
}

// offset: 0x1084 | func: 8
static void dll_481_func_1084(Object* self) {
    Object* sender;
    u32 mesgID;
    void* mesgArg;
    NWSH_Shrine_Data* objdata = self->data;

    mesgArg = NULL;
    while (objRecvMesg(self, &mesgID, &sender, &mesgArg) != 0) {
        switch (mesgID) {
        case 0x30005:
            objdata->unk6 = -3;
            break;
        case 0x30006:
            objdata->unk6 = 0x10;
            break;
        }
    }
}

/*0x0*/ static const char str_0[] = "##############SAVED#############\n";
