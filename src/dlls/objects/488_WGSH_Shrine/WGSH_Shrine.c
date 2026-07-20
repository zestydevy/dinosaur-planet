#include "dlls/objects/210_player.h"
#include "game/gamebits.h"
#include "sys/gfx/animseq.h"
#include "sys/dll.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "sys/objprint.h"
#include "sys/gfx/modgfx.h"
#include "sys/print.h"
#include "sys/envfx.h"
#include "dll.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
} WGSH_Shrine_Setup;

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
} WGSH_Shrine_Data;

static int dll_488_func_C90(Object *self, Object *a1, AnimObj_Data *a2, s8 a3);
static void dll_488_func_FEC(Object *self);

/*0x0*/ static Texture *_data_0 = NULL;

// offset: 0x0 | ctor
void dll_488_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_488_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_488_setup(Object *self, WGSH_Shrine_Setup *setup, s32 arg2) {
    WGSH_Shrine_Data *objdata;
    DLL_IModgfx *sp30;

    objdata = self->data;
    self->srt.yaw = 0;
    objdata->unk0 = 0xA;
    if (setup->unk1A > 0) {
        objdata->unk0 = (s16) (setup->unk1A >> 8);
    }
    objdata->unk13 = 0;
    objdata->unk14 = 0;
    objdata->unk2 = 0;
    objdata->unk12 = 0;
    self->animCallback = dll_488_func_C90;
    objInitMesgQueue(self, 4);
    mainSetBits(BIT_DB_Entered_Shrine_3, 1);
    mainSetBits(BIT_1D2, 0);
    mainSetBits(BIT_DB_Entered_Shrine_1, 1);
    mainSetBits(BIT_DB_Entered_Shrine_2, 1);
    mainSetBits(BIT_Spell_Projectile, 1);
    mainSetBits(BIT_Spell_Illusion, 1);
    mainSetBits(BIT_Spell_Forcefield, 1);
    mainSetBits(BIT_1D8, 0);
    objdata->unk4 = 0xC;
    objdata->unk8 = 0x1E;
    objdata->unk2 = 0xC8;
    gDLL_5_AMSEQ->vtbl->play_ex(2, 0x2B, 0x50, 1, 0);
    objdata->unk6 = 0;
    objdata->unkA = 0;
    objdata->unk16 = 0;
    objdata->unk10 = 0xC8;
    objdata->unkE = 0xFA0;
    sp30 = dllLoad(DLL_ID_122, 1);
    objdata->unkC = sp30->vtbl->func0(self, 1, 0, 0x402, -1, 0);
    dllFree(sp30);
    self->globalPosition.x = self->srt.transl.x;
    self->globalPosition.y = self->srt.transl.y;
    self->globalPosition.z = self->srt.transl.z;
}

// offset: 0x244 | func: 1 | export: 1
void dll_488_control(Object *self) {
    WGSH_Shrine_Data *objdata;
    Object *sp48;
    DLL_IModgfx *sp44;
    Object *temp_v0_4;
    f32 sp3C;
    f32 temp_fv1;
    s16 var_v0;

    objdata = self->data;
    sp48 = objGetPlayer();
    sp3C = 1000.0f;
    dll_488_func_FEC(self);
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
            if (objdata->unk16 == 0) {
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, objdata->unk8, 0);
                objdata->unk16 = 1;
            }
        }
    } else {
        temp_v0_4 = objGetNearestTypeTo(OBJTYPE_Door, sp48, &sp3C);
        if ((temp_v0_4 != NULL) && (sp3C < 300.0f) && (sp3C > 100.0f)) {
            temp_fv1 = temp_v0_4->srt.transl.z - sp48->srt.transl.z;
            if (temp_fv1 <= 0.0f) {
                if (temp_fv1 < 0.0f) {
                    temp_fv1 *= -1.0f;
                }
                if (objdata->unk8 != 0x1E) {
                    objdata->unk8 = 0x1E;
                }
                var_v0 = (s16) (s32) ((f32) objdata->unk8 * ((temp_fv1 - 100.0f) / 200.0f));
                if (var_v0 <= 0) {
                    var_v0 = 1;
                }
                gDLL_5_AMSEQ->vtbl->set_volume(3, var_v0);
                var_v0 = (s16) (s32) ((f32) objdata->unk4 * ((200.0f - (temp_fv1 - 100.0f)) / 200.0f));
                if (var_v0 <= 0) {
                    var_v0 = 1;
                }
                gDLL_5_AMSEQ->vtbl->set_volume(2, var_v0);
            }
        }
        switch (objdata->unk13) {
        case 0:
            if (vec3Distance(&self->globalPosition, &sp48->globalPosition) < (f32) objdata->unk0) {
                objdata->unk13 = 1;
                mainSetBits(BIT_DB_Entered_Shrine_3, 0);
                gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
                sp44 = dllLoad(DLL_ID_147, 1);
                sp44->vtbl->func0(self, 1, 0, 1, -1, 0);
                dllFree(sp44);
                sp44 = dllLoad(DLL_ID_148, 1);
                sp44->vtbl->func0(self, 0, 0, 1, -1, 0);
                dllFree(sp44);
                mainSetBits(BIT_DB_Entered_Shrine_1, 0);
                gDLL_14_Modgfx->vtbl->func7(&objdata->unkC);
            }
        default:
            return;
        case 1:
            if (objdata->unk14 == 1) {
                objdata->unk13 = 2;
                objdata->unk2 = 0xA0;
                return;
            }
            break;
        case 2:
            if ((objdata->unk12 == 0) && (mainGetBits(BIT_1D3) == 0)) {
                mainSetBits(BIT_1D3, 1);
            }
            if (mainGetBits(BIT_1D8) != 0) {
                objdata->unk12 += 1;
                mainSetBits(BIT_1D8, 0);
            }
            objdata->unkE -= (s16) gUpdateRateF;
            diPrintf("time %d\n", objdata->unkE);
            if (objdata->unkE <= 0) {
                mainSetBits(BIT_1D4, 1);
                gDLL_3_Animation->vtbl->start_obj_sequence(2, self, -1);
                objdata->unk2 = 0xA;
                objdata->unk13 = 6;
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x35, 0x50, (s16) (u8) objdata->unk8, 0);
                objdata->unkA = 1;
                mainSetBits(BIT_1D3, 0);
            } else if (objdata->unk12 == 1) {
                objdata->unk13 = 3;
                objdata->unk2 = 0xC8;
                objdata->unkA = -3;
                return;
            }
            break;
        case 3:
            if (mainGetBits(BIT_1D1) != 0) {
                objdata->unk8 = 1;
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, (s16) (u8) objdata->unk8, 0);
                objdata->unkA = 1;
                mainSetBits(BIT_DB_Entered_Shrine_3, 1);
                objdata->unk13 = 5;
                return;
            }
            ((DLL_210_Player*)sp48->dll)->vtbl->func51(sp48, -1);
            mainSetBits(BIT_DB_Entered_Shrine_1, 0);
            gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2A, 0x50, (s16) (u8) objdata->unk8, 0);
            objdata->unkA = 1;
            gDLL_3_Animation->vtbl->start_obj_sequence(1, self, -1);
            objdata->unk13 = 4;
            return;
        case 4:
            if (mainGetBits(BIT_Shrine_Do_Exit_Warp) == 0) {
                mainSetBits(BIT_Shrine_Do_Exit_Warp, 1);
            }
            mainSetBits(BIT_1D2, 0);
            mainSetBits(BIT_DB_Entered_Shrine_2, 0);
            objdata->unk13 = 5;
            gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, (s16) (u8) objdata->unk8, 0);
            break;
        case 6:
            objdata->unk13 = 0;
            objdata->unk14 = 0;
            objdata->unk2 = 0x190;
            mainSetBits(BIT_DB_Entered_Shrine_3, 1);
            mainSetBits(BIT_DB_Entered_Shrine_1, 1);
            mainSetBits(BIT_DB_Entered_Shrine_2, 1);
            sp44 = dllLoad(DLL_ID_122, 1);
            objdata->unkC = sp44->vtbl->func0(self, 2, 0, 0x402, -1, 0);
            dllFree(sp44);
            mainSetBits(BIT_1D8, 0);
            objdata->unk12 = 0;
            objdata->unkE = 0xFA0;
            mainSetBits(BIT_1D4, 0);
            break;
        }
    }
}

// offset: 0xBC4 | func: 2 | export: 2
void dll_488_update(Object *self) { }

// offset: 0xBD0 | func: 3 | export: 3
void dll_488_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xC24 | func: 4 | export: 4
void dll_488_free(Object *self, s32 a1) {
    gDLL_14_Modgfx->vtbl->func5(self);
}

// offset: 0xC6C | func: 5 | export: 5
u32 dll_488_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xC7C | func: 6 | export: 6
u32 dll_488_get_data_size(Object *self, u32 a1) {
    return sizeof(WGSH_Shrine_Data);
}

// offset: 0xC90 | func: 7
static int dll_488_func_C90(Object *self, Object *a1, AnimObj_Data *a2, s8 a3) {
    WGSH_Shrine_Data *objdata;
    s32 i;
    u8 temp_v0_2;

    objdata = self->data;
    a2->unk7A = -1;
    a2->unk62 = 0;
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
    for (i = 0; i < a2->messageCount; i++) {
        temp_v0_2 = a2->messages[i];
        if (temp_v0_2 != 0) {
            switch (temp_v0_2) {
            case 1:
                envfxAction(self, self, 0xC3, 0);
                break;
            case 2:
                if (D_80092A7C[0] == -1) {
                    envfxAction(self, self, 0x14, 0);
                } else {
                    envfxAction(self, self, D_80092A7C[0], 0);
                }
                break;
            case 3:
                objdata->unk14 = 1;
                break;
            case 4:
                objdata->unk13 = 4;
                objdata->unk14 = 2;
                mainSetBits(BIT_DB_Entered_Shrine_3, 1);
                mainSetBits(BIT_1D2, 0);
                mainSetBits(BIT_DB_Entered_Shrine_1, 1);
                objdata->unkA = -3;
                break;
            case 5:
                objdata->unk13 = 6;
                objdata->unk14 = 3;
                objdata->unkA = -3;
                mainSetBits(BIT_DB_Entered_Shrine_3, 1);
                break;
            case 6:
                mainSetBits(BIT_1D2, 1);
                break;
            case 7:
                mainSetBits(BIT_1D2, 0);
                objdata->unkA = -3;
                break;
            case 9:
                mainSetBits(BIT_DB_Triggered_In_Shrine_Spirit_Cutscene, 1);
                if (_data_0 == 0) {
                    _data_0 = blockTexanimGetTex(1);
                }
                break;
            case 8:
                mainSetBits(BIT_DB_Entered_Shrine_2, 1);
                break;
            case 11:
                objdata->unk8 = 0x64;
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2D, 0x50, (s16) (u8) objdata->unk8, 0);
                break;
            default:
                break;
            }
        }
        a2->messages[i] = 0;
    }
    return 0;
}

// offset: 0xFEC | func: 8
static void dll_488_func_FEC(Object *self) {
    Object *sender;
    u32 mesgID;
    void *mesgArg;
    WGSH_Shrine_Data *objdata;

    objdata = self->data;
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
