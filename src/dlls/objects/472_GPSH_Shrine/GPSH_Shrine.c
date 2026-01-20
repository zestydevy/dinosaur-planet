#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dll.h"
#include "dlls/objects/214_animobj.h"
#include "game/objects/object.h"
#include "sys/gfx/texture.h"
#include "sys/main.h"
#include "game/gamebits.h"
#include "sys/dll.h"
#include "sys/objects.h"
#include "sys/math.h"
#include "sys/map_enums.h"
#include "functions.h"
#include "sys/gfx/model.h"
#include "sys/map.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "sys/gfx/modgfx.h"
#include "sys/print.h"
#include "types.h"

typedef struct {
    ObjSetup base;
    s16 _unk18;
    s16 unk1A;
} GPSH_Shrine_Setup;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 _unk17;
    u8 unk18;
    u8 unk19;
} GPSH_Shrine_Data;

/*0x0*/ static Texture *_data_0 = NULL;

/*0x0*/ static u8 _bss_0[0x8];
/*0x8*/ static f32 _bss_8;
/*0xC*/ static u8 _bss_C[0x10];

static int GPSH_Shrine_func_1024(Object* a0, Object* a1, AnimObj_Data* a2, s8 a3);
static void GPSH_Shrine_func_13E0(Object* self);

// offset: 0x0 | ctor
void GPSH_Shrine_ctor(void *dll) { }

// offset: 0xC | dtor
void GPSH_Shrine_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void GPSH_Shrine_setup(Object* self, GPSH_Shrine_Setup* setup, s32 arg2) {
    GPSH_Shrine_Data* objdata;
    DLL_IModgfx* modgfxDLL;
    s16 temp_v0;

    objdata = self->data;
    self->srt.yaw = 0;
    objdata->unk0 = 0xA;
    temp_v0 = setup->unk1A;
    if (temp_v0 > 0) {
        objdata->unk0 = (s16) (temp_v0 >> 8);
    }
    objdata->unk15 = 0;
    objdata->unk16 = 0;
    objdata->unk2 = 0;
    self->animCallback = GPSH_Shrine_func_1024;
    obj_init_mesg_queue(self, 4);
    main_set_bits(BIT_DB_Entered_Shrine_3, 1);
    main_set_bits(BIT_MMP_GP_Shrine_Spirit_Light_Beams, 0);
    main_set_bits(BIT_DB_Entered_Shrine_1, 1);
    main_set_bits(BIT_DB_Entered_Shrine_2, 1);
    main_set_bits(BIT_Test_of_Fear_Particles, 0);
    main_set_bits(BIT_149, 0);
    main_set_bits(BIT_14C, 0);
    main_set_bits(BIT_14D, 0);
    main_set_bits(BIT_14E, 0);
    main_set_bits(BIT_14A, 0);
    main_set_bits(BIT_14B, 0);
    objdata->unk10 = 0;
    objdata->unk4 = 0xC;
    objdata->unk8 = 0x1E;
    objdata->unk2 = 0xC8;
    gDLL_5_AMSEQ->vtbl->func5(2, 0x2B, 0x50, 1, 0);
    objdata->unk6 = 0;
    objdata->unkA = 0;
    objdata->unk18 = 0;
    objdata->unk19 = 0;
    modgfxDLL = dll_load_deferred(DLL_ID_122, 1);
    objdata->unkC = modgfxDLL->vtbl->func0(self, 2, 0, 0x402, -1, 0);
    dll_unload(modgfxDLL);
    _bss_8 = 0.00001f;
    self->positionMirror.x = self->srt.transl.x;
    self->positionMirror.y = self->srt.transl.y;
    self->positionMirror.z = self->srt.transl.z;
}

// offset: 0x290 | func: 1 | export: 1
void GPSH_Shrine_control(Object* self) {
    GPSH_Shrine_Data* objdata;
    Object* player;
    Object** temp_v0_8;
    DLL_IModgfx* modgfxDLL;
    Object* temp_v0_5;
    f32 sp40;
    f32 sp3C;
    s16 var_v0;
    s32 sp34;

    objdata = self->data;
    player = get_player();
    sp34 = 0;
    sp40 = 1000.0f;
    sp3C = 0.0f;
    if (player == NULL) {
        return;
    }
    
    if (main_get_bits(BIT_5B1) != 0) {
        if (player->srt.scale >= 0.0265f) {
            player->srt.scale = 0.0495f - ((player->srt.transl.x + -16386.0f) * 0.0005f);
            diPrintf("scale %f %f  %f\n", &player->srt.scale, &sp3C, &player->srt.transl);
            if (player->srt.scale < 0.0265f) {
                player->srt.scale = 0.0265f;
            }
        }
    } else if (main_get_bits(BIT_5AD) != 0) {
        if (player->srt.scale >= 0.0265f) {
            player->srt.scale = ((player->srt.transl.x + -15613.0f) * 0.0005f) + 0.0495f;
            diPrintf("scale %f %f  %f\n", &player->srt.scale, &sp3C, &player->srt.transl);
            if (player->srt.scale < 0.0265f) {
                player->srt.scale = 0.0265f;
            }
        }
    } else if (player->srt.scale != 0.0495f) {
        player->srt.scale = 0.0495f;
    }
    GPSH_Shrine_func_13E0(self);
    main_set_bits(BIT_DB_Entered_Shrine_2, 1);
    if (objdata->unk6 != 0) {
        objdata->unk4 += objdata->unk6;
        if (objdata->unk4 < 0xD) {
            objdata->unk4 = 0xC;
            objdata->unk6 = 0;
        } else if (objdata->unk4 >= 0x46) {
            objdata->unk4 = 0x46;
            objdata->unk6 = 0;
        }
        gDLL_5_AMSEQ->vtbl->func13(2, objdata->unk4);
    }
    if (objdata->unkA != 0) {
        objdata->unk8 += objdata->unkA;
        if ((objdata->unk8 < 2) && (objdata->unkA <= 0)) {
            objdata->unk8 = 1;
            objdata->unkA = 0;
        }else if ((objdata->unk8 >= 0x46) && (objdata->unkA >= 0)) {
            objdata->unk8 = 0x46;
            objdata->unkA = 0;
        }
        gDLL_5_AMSEQ->vtbl->func13(3, objdata->unk8);
    }
    if (objdata->unk2 > 0) {
        objdata->unk2 -= gUpdateRate;
        if (objdata->unk2 <= 0) {
            objdata->unk2 = 0;
            if (objdata->unk18 == 0) {
                gDLL_5_AMSEQ->vtbl->func5(3, 0x2C, 0x50, objdata->unk8, 0);
                objdata->unk18 = 1;
            }
        }
        if ((objdata->unk15 == 2) && (objdata->unk2 < 0x29) && (objdata->unk19 == 0)) {
            objdata->unk19 = 1;
        }
    } else {
        temp_v0_5 = obj_get_nearest_type_to(OBJTYPE_16, player, &sp40);
        if ((temp_v0_5 != NULL) && (sp40 < 300.0f) && (sp40 > 100.0f)) {
            sp3C = temp_v0_5->srt.transl.z - player->srt.transl.z;
            if (sp3C <= 0.0f) {
                if (sp3C < 0.0f) {
                    sp3C *= -1.0f;
                }
                if (objdata->unk8 != 0x1E) {
                    objdata->unk8 = 0x1E;
                }
                var_v0 = ((f32) objdata->unk8 * ((sp3C - 100.0f) / 200.0f));
                if (var_v0 <= 0) {
                    var_v0 = 1;
                }
                gDLL_5_AMSEQ->vtbl->func13(3, var_v0);
                var_v0 = ((f32) objdata->unk4 * ((200.0f - (sp3C - 100.0f)) / 200.0f));
                if (var_v0 <= 0) {
                    var_v0 = 1;
                }
                gDLL_5_AMSEQ->vtbl->func13(2, var_v0);
            }
        }
        switch (objdata->unk15) {
        case 0:
            if (vec3_distance(&self->positionMirror, &player->positionMirror) < (f32) objdata->unk0) {
                objdata->unk15 = 1;
                main_set_bits(BIT_DB_Entered_Shrine_3, 0);
                gDLL_3_Animation->vtbl->func17(0, self, -1);
                modgfxDLL = dll_load_deferred(DLL_ID_147, 1);
                modgfxDLL->vtbl->func0(self, 2, 0, 1, -1, 0);
                dll_unload(modgfxDLL);
                modgfxDLL = dll_load_deferred(DLL_ID_148, 1);
                modgfxDLL->vtbl->func0(self, 0, 0, 1, -1, 0);
                dll_unload(modgfxDLL);
                main_set_bits(BIT_DB_Entered_Shrine_1, 0);
                gDLL_14_Modgfx->vtbl->func7(&objdata->unkC);
                objdata->unkC = -1;
                main_set_bits(BIT_5AF, 0);
            }
            break;
        case 1:
            if (objdata->unk16 == 1) {
                main_set_bits(BIT_148, 1);
                objdata->unk15 = 2;
                objdata->unk2 = 0x50;
                objdata->unk10 = 0x3A98;
                return;
            }
            break;
        case 2:
            objdata->unk10 -= gUpdateRate;
            diPrintf("\ntime %d\n", objdata->unk10);
            objdata->unk14 = 0;
            if (main_get_bits(BIT_149) != 0) {
                objdata->unk14++;
            }
            if (main_get_bits(BIT_14B) != 0) {
                objdata->unk14++;
            }
            if (main_get_bits(BIT_14E) != 0) {
                objdata->unk14++;;
            }
            if (main_get_bits(BIT_14D) != 0) {
                objdata->unk14++;
            }
            if (main_get_bits(BIT_14C) != 0) {
                objdata->unk14++;
            }
            if (main_get_bits(BIT_14A) != 0) {
                objdata->unk14++;
            }
            if (objdata->unk14 == 6) {
                objdata->unk15 = 4;
                objdata->unk2 = 0x78;
                return;
            }
            if (objdata->unk10 <= 0) {
                objdata->unk15 = 7;
                temp_v0_8 = obj_get_all_of_type(OBJTYPE_18, &sp34);
                while (sp34 != 0) {
                    obj_destroy_object(temp_v0_8[sp34 - 1]);
                    sp34--;
                    if ((!objdata) && (!objdata)){} // @fake
                }
                gDLL_3_Animation->vtbl->func17(2, self, -1);
            } else {
                objdata->unk14 = 0;
                return;
            }
            break;
        case 4:
            if (main_get_bits(BIT_SP_Map_MMP) != 0) {
                objdata->unk8 = 1;
                gDLL_5_AMSEQ->vtbl->func5(3, 0x2C, 0x50, (u8) objdata->unk8, 0);
                objdata->unkA = 1;
                main_set_bits(BIT_DB_Entered_Shrine_3, 1);
                objdata->unk15 = 6;
                return;
            }
            main_set_bits(BIT_DB_Entered_Shrine_1, 0);
            gDLL_5_AMSEQ->vtbl->func5(3, 0x2C, 0x50, (u8) objdata->unk8, 0);
            objdata->unkA = 1;
            gDLL_3_Animation->vtbl->func17(1, self, -1);
            objdata->unk15 = 5;
            return;
        case 5:
            if (main_get_bits(BIT_FD) == 0) {
                main_set_bits(BIT_FD, 1);
            }
            main_set_bits(BIT_MMP_GP_Shrine_Spirit_Light_Beams, 0);
            main_set_bits(BIT_DB_Entered_Shrine_2, 0);
            objdata->unk15 = 6;
            main_set_bits(BIT_DB_Entered_Shrine_1, 1);
            main_set_bits(BIT_SP_Map_MMP, 1);
            gDLL_29_Gplay->vtbl->set_map_setup(MAP_WARLOCK_MOUNTAIN, 8);
            break;
        case 7:
            objdata->unk15 = 0;
            objdata->unk16 = 0;
            objdata->unk2 = 0x190;
            main_set_bits(BIT_DB_Entered_Shrine_3, 1);
            main_set_bits(BIT_DB_Entered_Shrine_1, 1);
            main_set_bits(BIT_DB_Entered_Shrine_2, 1);
            modgfxDLL = dll_load_deferred(DLL_ID_122, 1);
            objdata->unkC = modgfxDLL->vtbl->func0(self, 2, 0, 0x402, -1, 0);
            dll_unload(modgfxDLL);
            main_set_bits(BIT_149, 0);
            main_set_bits(BIT_14C, 0);
            main_set_bits(BIT_14D, 0);
            main_set_bits(BIT_14E, 0);
            main_set_bits(BIT_14A, 0);
            main_set_bits(BIT_14B, 0);
            main_set_bits(BIT_14B, 0);
            main_set_bits(BIT_5AF, 1);
            main_set_bits(BIT_148, 0);
            break;
        }
    }
}

// offset: 0xF10 | func: 2 | export: 2
void GPSH_Shrine_update(Object *self) { }

// offset: 0xF1C | func: 3 | export: 3
void GPSH_Shrine_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xF70 | func: 4 | export: 4
void GPSH_Shrine_free(Object *self, s32 a1) {
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_5_AMSEQ->vtbl->func13(3, 0);
    gDLL_5_AMSEQ->vtbl->func13(2, 0);
}

// offset: 0x1000 | func: 5 | export: 5
u32 GPSH_Shrine_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1010 | func: 6 | export: 6
u32 GPSH_Shrine_get_data_size(Object *self, u32 a1) {
    return sizeof(GPSH_Shrine_Data);
}

// offset: 0x1024 | func: 7
static int GPSH_Shrine_func_1024(Object* a0, Object* a1, AnimObj_Data* a2, s8 a3) {
    GPSH_Shrine_Data* objdata;
    Object* player;
    s32 i;
    u8 temp_v0_2;

    objdata = a0->data;
    player = get_player();
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
        gDLL_5_AMSEQ->vtbl->func13(3, objdata->unk8);
    }
    for (i = 0; i < a2->unk98; i++) {
        temp_v0_2 = a2->unk8E[i];
        if (temp_v0_2 != 0) {
            switch (temp_v0_2) {
            case 1:
                func_80000860(a0, a0, 0xCD, 0);
                break;
            case 2:
                if (D_80092A7C[0] == -1) {
                    func_80000860(a0, a0, 0x14, 0);
                } else {
                    func_80000860(a0, a0, D_80092A7C[0], 0);
                }
                break;
            case 3:
                objdata->unk16 = 1;
                break;
            case 4:
                objdata->unk15 = 4;
                objdata->unk2 = 0x5A;
                break;
            case 5:
                objdata->unk16 = 2;
                main_set_bits(BIT_DB_Entered_Shrine_3, 1);
                break;
            case 6:
                objdata->unk15 = 5;
                objdata->unk16 = 3;
                main_set_bits(BIT_DB_Entered_Shrine_3, 1);
                break;
            case 7:
                main_set_bits(BIT_MMP_GP_Shrine_Spirit_Light_Beams, 1);
                break;
            case 8:
                main_set_bits(BIT_MMP_GP_Shrine_Spirit_Light_Beams, 0);
                objdata->unkA = -3;
                break;
            case 10:
                main_set_bits(BIT_DB_Triggered_In_Shrine_Spirit_Cutscene, 1);
                if (_data_0 == 0) {
                    _data_0 = func_8004A1E8(1);
                }
                break;
            case 9:
                main_set_bits(BIT_DB_Entered_Shrine_2, 1);
                break;
            case 11:
                objdata->unk8 = 0x64;
                gDLL_5_AMSEQ->vtbl->func5(3, 0x30, 0x50, (u8)objdata->unk8, 0);
                break;
            case 12:
                main_set_bits(BIT_Test_of_Fear_Particles, 0);
                break;
            }
        }
        a2->unk8E[i] = 0;
    }
    if ((objdata->unk15 == 3) && ((f32)objdata->unk0 < vec3_distance(&a0->positionMirror, &player->positionMirror))) {
        gDLL_3_Animation->vtbl->func18(a2->unk63);
    }
    return 0;
}

// offset: 0x13E0 | func: 8
static void GPSH_Shrine_func_13E0(Object* self) {
    Object* mesgSender;
    u32 mesgID;
    void* mesg;
    GPSH_Shrine_Data* objdata;

    objdata = self->data;
    mesg = NULL;
    while (obj_recv_mesg(self, &mesgID, &mesgSender, &mesg) != 0) {
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
