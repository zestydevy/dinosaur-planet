#include "common.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "sys/gfx/modgfx.h"
#include "dlls/objects/210_player.h"
#include "sys/segment_1460.h"


typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    u8 unk10;
    s8 unk11;
    u8 unk12;
    s8 _unk13;
    u8 unk14;
} DFShrine_Data;
typedef struct {
    ObjSetup base;
    s16 _unk18;
    s16 unk1A;
} DFShrine_Setup;

/*0x0*/ static Texture* _data_0 = 0x00000000;
static int dll_433_func_F18(Object* self, Object* arg1, AnimObj_Data* arg2, s8 arg3);
static void dll_433_func_12AC(Object* arg0);

// offset: 0x0 | ctor
void dll_433_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_433_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_433_setup(Object* self, DFShrine_Setup* setup, s32 arg2) {
    DFShrine_Data* objdata;
    DLL_IModgfx* modgfxDLL;

    objdata = self->data;
    self->srt.yaw = 0;
    objdata->unk0 = 0xA;
    if (setup->unk1A > 0) {
        objdata->unk0 = (s16) (setup->unk1A >> 8);
    }
    objdata->unk11 = 0;
    objdata->unk12 = 0;
    objdata->unk2 = 0;
    objdata->unk10 = 0;
    self->animCallback = dll_433_func_F18;
    obj_init_mesg_queue(self, 4U);
    main_set_bits(BIT_DB_Entered_Shrine_3, 1U);
    main_set_bits(BIT_125, 0U);
    main_set_bits(BIT_DB_Entered_Shrine_1, 1U);
    main_set_bits(BIT_DB_Entered_Shrine_2, 1U);
    main_set_bits(BIT_1E7, 0U);
    objdata->unk4 = 0xC;
    objdata->unk8 = 0x1E;
    objdata->unk2 = 0xC8;
    objdata->unk6 = 0;
    objdata->unkA = 0;
    objdata->unk14 = 0;
    objdata->unkE = 0;
    modgfxDLL = dll_load_deferred(DLL_ID_122, 1U);
    objdata->unkC = modgfxDLL->vtbl->func0(self, 0, 0, 0x402, -1, 0);
    dll_unload(modgfxDLL);
}

// offset: 0x1A8 | func: 1 | export: 1
void dll_433_control(Object* self) {
    static u8 _data_4 = 1;
    DFShrine_Data* objdata = self->data;
    Object* player = get_player();
    DLL_IModgfx* modgfxDLL;
    f32 var_fv0;
    f32 sp44;
    Object* temp_v0_4;
    s16 var_v0;
    s16 sp3C;
    Object** sp38;
    s32 sp34;

    sp34 = 0;
    sp44 = 1000.0f;
    if (_data_4) {
        self->globalPosition.f[0] = self->srt.transl.f[0];
        self->globalPosition.f[1] = self->srt.transl.f[1];
        self->globalPosition.f[2] = self->srt.transl.f[2];
        main_set_bits(BIT_DB_Entered_Shrine_2, 1U);
        gDLL_5_AMSEQ->vtbl->play_ex(2U, 0x2BU, 0x50, 1, 0U);
    }
    if (_data_4) {
        ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 0x14);
        main_set_bits(BIT_Spell_Forcefield, 1U);
        _data_4 = 0;
    }
    dll_433_func_12AC(self);
    if (objdata->unk6 != 0) {
        objdata->unk4 += objdata->unk6;
        if (objdata->unk4 < 0xD) {
            objdata->unk4 = 0xC;
            objdata->unk6 = 0;
        }
        else if (objdata->unk4 >= 0x46) {
            objdata->unk4 = 0x46;

            objdata->unk6 = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(2U, objdata->unk4);
    }
    if (objdata->unkA != 0) {
        objdata->unk8 += objdata->unkA;
        if ((objdata->unk8 < 2) && (objdata->unkA <= 0)) {
            objdata->unk8 = 1;
            objdata->unkA = 0;
        } 
        else if ((objdata->unk8 >= 0x46) && (objdata->unkA >= 0)) {
            objdata->unk8 = 0x46;
            objdata->unkA = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(3U, objdata->unk8);
    }
    if (objdata->unk2 > 0) {
        objdata->unk2 -= gUpdateRate;
        if (objdata->unk2 <= 0) {
            objdata->unk2 = 0;
            if (objdata->unk14 == 0) {
                gDLL_5_AMSEQ->vtbl->play_ex(3U, 0x2CU, 0x50, objdata->unk8, 0U);
                objdata->unk14 = 1U;
            }
        }
    } else {
        temp_v0_4 = obj_get_nearest_type_to(OBJTYPE_16, player, &sp44);
        if ((temp_v0_4 != NULL) && (sp44 < 300.0f) && (sp44 > 100.0f)) {
            var_fv0 = temp_v0_4->srt.transl.f[2] - player->srt.transl.f[2];
            if (var_fv0 <= 0.0f) {
                if (var_fv0 < 0.0f) {
                    var_fv0 *= -1.0f;
                }
                if (objdata->unk8 != 0x1E) {
                    objdata->unk8 = 0x1E;
                }
                var_v0 = (s16) ((f32) objdata->unk8 * ((var_fv0 - 100.0f) / 200.0f));
                if (var_v0 <= 0) {
                    var_v0 = 1;
                }
                gDLL_5_AMSEQ->vtbl->set_volume(3U, var_v0);
                var_v0 = (s16) ((f32) objdata->unk4 * ((200.0f - (var_fv0 - 100.0f)) / 200.0f));
                if (var_v0 <= 0) {
                    var_v0 = 1;
                }
                gDLL_5_AMSEQ->vtbl->set_volume(2U, var_v0);
            }
        }
        switch (objdata->unk11) {
        case 0:
            if (vec3_distance(&self->globalPosition, &player->globalPosition) < (f32) objdata->unk0) {
                main_set_bits(BIT_589, 0U);
                objdata->unk11 = 1U;
                main_set_bits(BIT_DB_Entered_Shrine_3, 0U);
                gDLL_3_Animation->vtbl->func17(0, self, -1);
                modgfxDLL = dll_load_deferred(DLL_ID_147, 1U);
                modgfxDLL->vtbl->func0(self, 0, 0, 1, -1, 0);
                dll_unload(modgfxDLL);
                modgfxDLL = dll_load_deferred(DLL_ID_148, 1U);
                modgfxDLL->vtbl->func0(self, 0, 0, 1, -1, 0);
                dll_unload(modgfxDLL);
                main_set_bits(BIT_DB_Entered_Shrine_1, 0U);
                gDLL_14_Modgfx->vtbl->func7(&objdata->unkC);
            }
        default:
            return;
        case 1:
            if (objdata->unk12 == 1) {
                objdata->unk11 = 2U;
                objdata->unk2 = 0xA0;
                objdata->unkE = 0x1770;
                return;
            }
            break;
        case 2:
            objdata->unkE = (s16) (objdata->unkE - gUpdateRate);
            diPrintf("TIMER %d\n", objdata->unkE, player);
            if (objdata->unk10 == 0) {
                if (main_get_bits(BIT_F6) == 0) {
                    main_set_bits(BIT_F6, 1U);
                }
            } else if (objdata->unk10 == 1) {
                if (main_get_bits(BIT_F7) == 0) {
                    main_set_bits(BIT_F7, 1U);
                }
            } else if (objdata->unk10 == 3) {
                if (main_get_bits(BIT_F8) == 0) {
                    main_set_bits(BIT_F8, 1U);
                }
            } else if (objdata->unk10 == 4) {
                if (main_get_bits(BIT_FA) == 0) {
                    main_set_bits(BIT_FA, 1U);
                }
                if (main_get_bits(BIT_FB) == 0) {
                    main_set_bits(BIT_FB, 1U);
                }
            }
            if (main_get_bits(BIT_1E7) != 0) {
                objdata->unk10++;
                objdata->unk2 = 0x64;
                if (objdata->unk10 == 4) {
                    objdata->unk11 = 3U;
                    objdata->unk2 = 0xC8;
                    objdata->unkA = -3;
                    gDLL_5_AMSEQ->vtbl->play_ex(3U, 0xEU, 0x50, (u8)objdata->unk8, 0U);
                    objdata->unkA = 1;
                }
                main_set_bits(BIT_1E7, 0U);
            } else if (objdata->unkE <= 0) {
                objdata->unk11 = 3U;
                return;
            }
            break;
        case 3:
            if (objdata->unk10 != 4) {
                sp38 = obj_get_all_of_type(OBJTYPE_4, &sp34);
                sp3C = 0;
                while (sp3C < sp34) {
                    obj_destroy_object(sp38[sp3C]);
                    sp3C += 1;
                }
                objdata->unk2 = 0;
                gDLL_5_AMSEQ->vtbl->play_ex(3U, 0x35U, 0x50, (u8) objdata->unk8, 0U);
                objdata->unkA = 1;
                gDLL_3_Animation->vtbl->func17(2, self, -1);
                objdata->unk2 = 0xA;
                return;
            }
            if (((DLL_210_Player*)player->dll)->vtbl->func38(player, 1) != 0) {
                objdata->unk8 = 1;
                gDLL_5_AMSEQ->vtbl->play_ex(3U, 0x2CU, 0x50, (u8) objdata->unk8, 0U);
                objdata->unkA = 1;
                main_set_bits(BIT_DB_Entered_Shrine_3, 1U);
                objdata->unk11 = 5U;
                return;
            }
            objdata->unk2 = 0;
            main_set_bits(BIT_DB_Entered_Shrine_1, 0U);
            gDLL_3_Animation->vtbl->func17(1, self, -1);
            objdata->unk2 = 0xA;
            return;
        case 4:
            main_set_bits(BIT_125, 0U);
            objdata->unk11 = 5U;
            ((DLL_210_Player*)player->dll)->vtbl->func39(player, 1, 1);
            gDLL_29_Gplay->vtbl->set_map_setup(0xB, 2);
            break;
        case 5:
            if (main_get_bits(BIT_Shrine_Do_Exit_Warp) == 0) {
                main_set_bits(BIT_Shrine_Do_Exit_Warp, 1U);
            }
            main_set_bits(BIT_DB_Entered_Shrine_2, 0U);
            main_set_bits(BIT_DB_Entered_Shrine_3, 1U);
            objdata->unk11 = 7U;
            return;
        case 8:
            objdata->unk11 = 0U;
            objdata->unk12 = 0U;
            objdata->unk2 = 0x190;
            objdata->unk10 = 0U;
            main_set_bits(BIT_DB_Entered_Shrine_3, 1U);
            main_set_bits(BIT_125, 0U);
            main_set_bits(BIT_DB_Entered_Shrine_1, 1U);
            main_set_bits(BIT_DB_Entered_Shrine_2, 1U);
            main_set_bits(BIT_1E7, 0U);
            main_set_bits(BIT_F6, 0U);
            main_set_bits(BIT_F7, 0U);
            main_set_bits(BIT_F8, 0U);
            main_set_bits(BIT_F9, 0U);
            main_set_bits(BIT_FA, 0U);
            main_set_bits(BIT_FB, 0U);
            main_set_bits(BIT_589, 1U);
            objdata->unkE = 0;
            modgfxDLL = dll_load_deferred(DLL_ID_122, 1U);
            objdata->unkC = modgfxDLL->vtbl->func0(self, 0, 0, 0x402, -1, 0);
            dll_unload(modgfxDLL);
            break;
        }
    }
}

// offset: 0xE04 | func: 2 | export: 2
void dll_433_update(Object *self) { }

// offset: 0xE10 | func: 3 | export: 3
void dll_433_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xE64 | func: 4 | export: 4
void dll_433_free(Object* self, s32 a1) {
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_5_AMSEQ->vtbl->set_volume(3U, 0U);
    gDLL_5_AMSEQ->vtbl->set_volume(2U, 0U);
}

// offset: 0xEF4 | func: 5 | export: 5
u32 dll_433_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xF04 | func: 6 | export: 6
u32 dll_433_get_data_size(Object *self, u32 a1) {
    return sizeof(DFShrine_Data);
}

// offset: 0xF18 | func: 7
static int dll_433_func_F18(Object* self, Object* arg1, AnimObj_Data* arg2, s8 arg3) {
    s32 i;
    DFShrine_Data* objdata;

    objdata = self->data;
    arg2->unk62 = 0;
    if (objdata->unkA != 0) {
        objdata->unk8 += objdata->unkA;
        if ((objdata->unk8 < 2) && (objdata->unkA <= 0)) {
            objdata->unk8 = 1;
            objdata->unkA = 0;
        } else if ((objdata->unk8 >= 0x46) && (objdata->unkA >= 0)) {
            objdata->unk8 = 0x46;
            objdata->unkA = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(3U, objdata->unk8);
    }

    for (i = 0; i < arg2->unk98; i++) {
        s32 temp = arg2->unk8E[i];
        if (temp == 0) { // huh?
        } else {
            switch (temp) {
                case 1:
                    func_80000860(self, self, 0xC3U, 0);
                    break;
                case 2:
                    if (*D_80092A7C == -1) {
                        func_80000860(self, self, 0x14U, 0);
                    } else {
                        func_80000860(self, self, *D_80092A7C, 0);
                    }
                    break;
                case 3:
                    objdata->unk12 = 1;
                    break;
                case 4:
                    objdata->unk11 = 4;
                    objdata->unk12 = 2;
                    main_set_bits(BIT_DB_Entered_Shrine_3, 1U);
                    main_set_bits(BIT_125, 0U);
                    main_set_bits(BIT_DB_Entered_Shrine_1, 1U);
                    objdata->unkA = -3;
                    break;
                case 5:
                    objdata->unk11 = 8;
                    objdata->unk12 = 3;
                    objdata->unkA = -3;
                    main_set_bits(BIT_DB_Entered_Shrine_3, 1U);
                    break;
                case 6:
                    main_set_bits(BIT_125, 1U);
                    break;
                case 7:
                    main_set_bits(BIT_125, 0U);
                    objdata->unkA = -3;
                    break;
                case 9:
                    main_set_bits(BIT_DB_Triggered_In_Shrine_Spirit_Cutscene, 1U);
                    if (_data_0 == 0) {
                        _data_0 = block_texanim_get_tex(1);
                    }
                    break;
                case 8:
                    main_set_bits(BIT_DB_Entered_Shrine_2, 1U);
                    break;
                case 10:
                    objdata->unk8 = 0x64;
                    gDLL_5_AMSEQ->vtbl->play_ex(3U, 0x2DU, 0x50, (u8) objdata->unk8, 0U);
                    break;
                case 11:
                    objdata->unk8 = 0x64;
                    gDLL_5_AMSEQ->vtbl->play_ex(3U, 0x36U, 0x50, (u8) objdata->unk8, 0U);
                    break;
            }
        }
        arg2->unk8E[i] = 0U;
    }

    return 0;
}

// offset: 0x12AC | func: 8
static void dll_433_func_12AC(Object* arg0) {
    Object* sp54;
    u32 sp50;
    void* sp4C;
    DFShrine_Data* objdata;

    objdata = arg0->data;
    sp4C = NULL;
    if (obj_recv_mesg(arg0, &sp50, &sp54, &sp4C) != 0) {
        do {
            switch (sp50) {
            case 0x30005:
                objdata->unk6 = -3;
                break;
            case 0x30006:
                objdata->unk6 = 0x10;
                break;
            }
        } while (obj_recv_mesg(arg0, &sp50, &sp54, &sp4C) != 0);
    }
}

