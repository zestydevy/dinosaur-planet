#include "dlls/objects/210_player.h"
#include "dlls/objects/214_animobj.h"
#include "game/gamebits.h"
#include "sys/dll.h"
#include "sys/gfx/modgfx.h"
#include "sys/objanim.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "dll.h"
#include "functions.h"

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
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
} ECSHshrine_Data;

typedef struct {
    ObjSetup base;
    u8 _unk18[0x1A - 0x18];
    s16 unk1A;
} ECSHshrine_Setup;

typedef struct {
    f32 unk0;
    f32 unk4;
} ECSHshrineStruct;

/*0x0*/ static ECSHshrineStruct _data_0[] = {
    {0.0f, 0.0f}, 
    {0.0f, 0.0f}, 
    {0.0f, 0.0f}, 
    {0.0f, 0.0f},
    {0.0f, 0.0f},
    {0.0f, 0.0f}
};
/*0x30*/ static s16 _data_30[] = {0, 1, 2, 3, 4, 5};
/*0x3C*/ static Texture *_data_3C = NULL;
/*0x40*/ static Object *_data_40 = NULL;

static int dll_469_func_15E0(Object *self, Object *a1, AnimObj_Data *a2, s8 a3);
static void dll_469_func_193C(Object *self);

// offset: 0x0 | ctor
void dll_469_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_469_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_469_setup(Object *self, ECSHshrine_Setup *setup, s32 arg2) {
    ECSHshrine_Data *objdata;
    DLL_IModgfx *temp_v0_2;

    objdata = self->data;
    self->srt.yaw = 0;
    objdata->unk0 = 0xA;
    if (setup->unk1A > 0) {
        objdata->unk0 = (s16) (setup->unk1A >> 8);
    }
    objdata->unk1B = 0;
    objdata->unk1C = 0;
    objdata->unk2 = 0;
    objdata->unk4 = 0;
    objdata->unk10 = 0;
    objdata->unk12 = 0;
    objdata->unk16 = 0;
    objdata->unk18 = -1;
    objdata->unk1A = 0;
    self->animCallback = dll_469_func_15E0;
    obj_init_mesg_queue(self, 4);
    main_set_bits(BIT_DB_Entered_Shrine_3, 1);
    main_set_bits(BIT_143, 0);
    main_set_bits(BIT_DB_Entered_Shrine_1, 1);
    main_set_bits(BIT_DB_Entered_Shrine_2, 1);
    main_set_bits(BIT_Test_of_Fear_Particles, 0);
    objdata->unk6 = 0xC;
    objdata->unkA = 0x1E;
    objdata->unk2 = 0xC8;
    gDLL_5_AMSEQ->vtbl->play_ex(2, 0x2B, 0x50, 1, 0);
    objdata->unk8 = 0;
    objdata->unkC = 0;
    objdata->unk1D = 0;
    temp_v0_2 = dll_load_deferred(DLL_ID_122, 1);
    objdata->unkE = temp_v0_2->vtbl->func0(self, 2, NULL, 0x402, -1, NULL);
    dll_unload(temp_v0_2);
    _data_40 = self;
    obj_add_object_type(self, OBJTYPE_13);
}

// offset: 0x208 | func: 1 | export: 1
void dll_469_control(Object* self) {
    ECSHshrine_Data* objdata = self->data;
    Object* sp60 = get_player();
    f32 var_fv0;
    f32 temp_ft2;
    f32 temp_ft5;
    s32 sp4C[] = {0, 0};
    DLL_IModgfx* sp48;
    Object* temp_v0_4;
    f32 sp40;
    s16 var_v0;
    s16 temp_t4;
    s32 i;

    sp40 = 1000.0f;
    dll_469_func_193C(self);
    main_set_bits(BIT_DB_Entered_Shrine_2, 1);
    if (objdata->unk8 != 0) {
        objdata->unk6 += objdata->unk8;
        if (objdata->unk6 < 0xD) {
            objdata->unk6 = 0xC;
            objdata->unk8 = 0;
        } else if (objdata->unk6 >= 0x46) {
            objdata->unk6 = 0x46;
            objdata->unk8 = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(2, objdata->unk6);
    }
    if (objdata->unkC != 0) {
        objdata->unkA += objdata->unkC;
        if ((objdata->unkA < 2) && (objdata->unkC <= 0)) {
            objdata->unkA = 1;
            objdata->unkC = 0;
        } else if ((objdata->unkA >= 0x46) && (objdata->unkC >= 0)) {
            objdata->unkA = 0x46;
            objdata->unkC = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(3, objdata->unkA);
    }
    if (objdata->unk2 > 0) {
        objdata->unk2 -= gUpdateRate;
        if (objdata->unk2 <= 0) {
            objdata->unk2 = 0;
            if (objdata->unk1D == 0) {
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, objdata->unkA, 0);
                objdata->unk1D = 1;
            }
        }
    } else {
        temp_v0_4 = obj_get_nearest_type_to(OBJTYPE_16, sp60, &sp40);
        if ((temp_v0_4 != NULL) && (sp40 < 300.0f) && (sp40 > 100.0f)) {
            var_fv0 = temp_v0_4->srt.transl.z - sp60->srt.transl.z;
            if (var_fv0 <= 0.0f) {
                if (var_fv0 < 0.0f) {
                    var_fv0 *= -1.0f;
                }
                if (objdata->unkA != 0x1E) {
                    objdata->unkA = 0x1E;
                }
                var_v0 = (s16) (s32) ((f32) objdata->unkA * ((var_fv0 - 100.0f) / 200.0f));
                if (var_v0 <= 0) {
                    var_v0 = 1;
                }
                gDLL_5_AMSEQ->vtbl->set_volume(3, var_v0);
                var_v0 = (s16) (s32) ((f32) objdata->unk6 * ((200.0f - (var_fv0 - 100.0f)) / 200.0f));
                if (var_v0 <= 0) {
                    var_v0 = 1;
                }
                gDLL_5_AMSEQ->vtbl->set_volume(2, var_v0);
            }
        }
        switch (objdata->unk1B) {
        case 0:
            if (vec3_distance(&self->positionMirror, &sp60->positionMirror) < (f32) objdata->unk0) {
                objdata->unk1B = 1;
                main_set_bits(BIT_DB_Entered_Shrine_3, 0);
                gDLL_3_Animation->vtbl->func17(0, self, -1);
                sp48 = dll_load_deferred(DLL_ID_147, 1);
                sp48->vtbl->func0(self, 2, NULL, 1, -1, NULL);
                dll_unload(sp48);
                sp48 = dll_load_deferred(DLL_ID_148, 1);
                sp48->vtbl->func0(self, 0, NULL, 1, -1, NULL);
                dll_unload(sp48);
                main_set_bits(BIT_DB_Entered_Shrine_1, 0);
                gDLL_14_Modgfx->vtbl->func7(&objdata->unkE);
            }
            break;
        case 1:
            if (objdata->unk1C == 1) {
                objdata->unk1B = 2;
                objdata->unk2 = 0xC8;
                objdata->unk16 = 6;
                gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_33E, MAX_VOLUME, NULL, NULL, 0, NULL);
                objdata->unk4 = 0;
                return;
            }
            break;
        case 2:
            objdata->unk1B = 3;
            objdata->unk2 = 0x50;
            objdata->unk16 = 8;
            objdata->unk4 = 0x28;
            objdata->unk12 = 0x10;
            objdata->unk1A = rand_next(0, 5);
            gDLL_3_Animation->vtbl->func17(3, self, -1);
            break;
        case 3:
        case 4:
        case 5:
            if (objdata->unk4 > 0) {
                objdata->unk4 -= 1;
                if (objdata->unk4 < 0) {
                    objdata->unk4 = 0;
                }
            } else {
                switch (objdata->unk16) {
                case 8:
                    objdata->unk16 = 2;
                    objdata->unk4 = 0x28;
                    objdata->unk2 = 0x3C;
                    break;
                case 9:
                    objdata->unk16 = 8;
                    objdata->unk4 = 0x28;
                    objdata->unk2 = 0x3C;
                    break;
                case 7:
                    objdata->unk16 = 3;
                    objdata->unk4 = 0x28;
                    objdata->unk2 = 0x3C;
                    break;
                case 2:
                    objdata->unk12 -= 1;
                    if (objdata->unk12 <= 0) {
                        objdata->unk16 = 5;
                        objdata->unk14 = 0x3E8;
                    } else {
                        objdata->unk16 = 0;
                        objdata->unk4 = 2;
                        if (objdata->unk1B == 3) {
                            i = rand_next(0, 1);
                        } else if (objdata->unk1B == 4) {
                            i = rand_next(0, 5);
                        } else {
                            i = rand_next(0, 7);
                        }
                        if (i == 0) {
                            for (i = 0; i < 6; i++) {
                                _data_30[i] += 1;
                                if (_data_30[i] > 5) {
                                    _data_30[i] = 0;
                                }
                            }
                        } else if (i == 1) {
                            for (i = 0; i < 6; i++) {
                                _data_30[i] -= 1;
                                if (_data_30[i] < 0) {
                                    _data_30[i] = 5;
                                }
                            }
                        } else if (i == 2) {
                            temp_t4 = _data_30[0];
                            _data_30[0] = _data_30[2];
                            _data_30[2] = _data_30[4];
                            _data_30[4] = temp_t4;
                        } else if (i == 3) {
                            temp_t4 = _data_30[4];
                            _data_30[4] = _data_30[0];
                            _data_30[0] = _data_30[2];
                            _data_30[2] = temp_t4;
                        } else if (i == 4) {
                            temp_t4 = _data_30[1];
                            _data_30[1] = _data_30[3];
                            _data_30[3] = _data_30[5];
                            _data_30[5] = temp_t4;
                        } else if (i == 5) {
                            temp_t4 = _data_30[5];
                            _data_30[5] = _data_30[1];
                            _data_30[1] = _data_30[3];
                            _data_30[3] = temp_t4;
                        } else if (i == 6) {
                            temp_ft5 = _data_0[1].unk0;
                            temp_ft2 = _data_0[1].unk4;
                            _data_0[1].unk0 = _data_0[2].unk0;
                            _data_0[1].unk4 = _data_0[2].unk4;
                            _data_0[2].unk0 = _data_0[4].unk0;
                            _data_0[2].unk4 = _data_0[4].unk4;
                            _data_0[4].unk0 = _data_0[5].unk0;
                            _data_0[4].unk4 = _data_0[5].unk4;
                            _data_0[5].unk0 = temp_ft5;
                            _data_0[5].unk4 = temp_ft2;
                        } else if (i == 7) {
                            temp_ft5 = _data_0[5].unk0;
                            temp_ft2 = _data_0[5].unk4;
                            _data_0[5].unk0 = _data_0[4].unk0;
                            _data_0[5].unk4 = _data_0[4].unk4;
                            _data_0[4].unk0 = _data_0[2].unk0;
                            _data_0[4].unk4 = _data_0[2].unk4;
                            _data_0[2].unk0 = _data_0[1].unk0;
                            _data_0[2].unk4 = _data_0[1].unk4;
                            _data_0[1].unk0 = temp_ft5;
                            _data_0[1].unk4 = temp_ft2;
                        }
                    }
                    break;
                case 0:
                    objdata->unk16 = 1;
                    objdata->unk4 = 0xC;
                    return;
                case 1:
                    objdata->unk16 = 4;
                    objdata->unk4 = 0;
                    return;
                case 4:
                    objdata->unk16 = 2;
                    objdata->unk4 = 0;
                    return;
                case 5:
                    if (objdata->unk18 == 0) {
                        objdata->unk1B = 9;
                        gDLL_5_AMSEQ->vtbl->play_ex(3, 0x35, 0x50, (s16) (u8) objdata->unkA, 0);
                        objdata->unkC = 1;
                        gDLL_3_Animation->vtbl->func17(2, self, -1);
                        objdata->unk2 = 0xA;
                        objdata->unk16 = 7;
                        gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_33E, MAX_VOLUME, NULL, NULL, 0, NULL);
                    } else if (objdata->unk18 == 1) {
                        if (objdata->unk1B == 3) {
                            objdata->unk1A = rand_next(0, 5);
                            objdata->unk1B = 4;
                            objdata->unk16 = 9;
                            objdata->unk2 = 0x96;
                            objdata->unk4 = 0xC;
                            objdata->unk12 = 0xA;
                            objdata->unk18 = -1;
                            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_344_Chime, MAX_VOLUME, NULL, NULL, 0, NULL);
                            gDLL_3_Animation->vtbl->func17(3, self, -1);
                        } else if (objdata->unk1B == 4) {
                            objdata->unk1A = rand_next(0, 5);
                            objdata->unk1B = 5;
                            objdata->unk16 = 9;
                            objdata->unk2 = 0x96;
                            objdata->unk4 = 0xC;
                            objdata->unk12 = 0x10;
                            objdata->unk18 = -1;
                            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_344_Chime, MAX_VOLUME, NULL, NULL, 0, NULL);
                            gDLL_3_Animation->vtbl->func17(3, self, -1);
                        } else {
                            objdata->unk2 = 0x82;
                            objdata->unk1B = 6;
                            objdata->unk18 = 0;
                            objdata->unk16 = 7;
                            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_344_Chime, MAX_VOLUME, NULL, NULL, 0, NULL);
                            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_33E, MAX_VOLUME, NULL, NULL, 0, NULL);
                        }
                    } else {
                        objdata->unk14 -= gUpdateRate;
                        if (objdata->unk14 <= 0) {
                            objdata->unk2 = 0x82;
                            objdata->unk1B = 6;
                            objdata->unk18 = 0;
                            objdata->unk16 = 7;
                            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_344_Chime, MAX_VOLUME, NULL, NULL, 0, NULL);
                            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_33E, MAX_VOLUME, NULL, NULL, 0, NULL);
                        }
                    }
                    break;
                }
            }
            break;
        case 6:
            if (((DLL_210_Player*)sp60->dll)->vtbl->func38(sp60, 2) != 0) {
                objdata->unkA = 1;
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, (s16) (u8) objdata->unkA, 0);
                objdata->unkC = 1;
                main_set_bits(BIT_DB_Entered_Shrine_3, 1);
                objdata->unk1B = 8;
            } else {
                main_set_bits(BIT_DB_Entered_Shrine_1, 0);
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, (s16) (u8) objdata->unkA, 0);
                objdata->unkC = 1;
                gDLL_3_Animation->vtbl->func17(1, self, -1);
                objdata->unk1B = 7;
            }
            break;
        case 7:
            main_set_bits(BIT_143, 0);
            objdata->unk1B = 8;
            ((DLL_210_Player*)sp60->dll)->vtbl->func39(sp60, 8, 1);
            gDLL_29_Gplay->vtbl->set_map_setup(MAP_WARLOCK_MOUNTAIN, 5);
            break;
        case 8:
            if (main_get_bits(BIT_FD) == 0) {
                main_set_bits(BIT_FD, 1);
            }
            main_set_bits(BIT_DB_Entered_Shrine_2, 0);
            main_set_bits(BIT_DB_Entered_Shrine_3, 0);
            objdata->unk1B = 0xA;
            break;
        case 9:
            objdata->unk1B = 0;
            objdata->unk10 = 0;
            objdata->unk12 = 0;
            objdata->unk16 = 0;
            objdata->unk18 = -1;
            objdata->unk4 = 0;
            objdata->unk1A = 0;
            objdata->unk1C = 0;
            objdata->unk2 = 0x190;
            main_set_bits(BIT_DB_Entered_Shrine_3, 1);
            main_set_bits(BIT_DB_Entered_Shrine_1, 1);
            main_set_bits(BIT_DB_Entered_Shrine_2, 1);
            sp48 = dll_load_deferred(DLL_ID_122, 1);
            objdata->unkE = sp48->vtbl->func0(self, 2, NULL, 0x402, -1, NULL);
            dll_unload(sp48);
            break;
        case 10:
            break;
        }
    }
}

// offset: 0x1320 | func: 2 | export: 2
void dll_469_update(Object *self) { }

// offset: 0x132C | func: 3 | export: 3
void dll_469_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1380 | func: 4 | export: 4
void dll_469_free(Object *self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_13);
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_5_AMSEQ->vtbl->set_volume(3, 0);
    gDLL_5_AMSEQ->vtbl->set_volume(2, 0);
}

// offset: 0x1430 | func: 5 | export: 5
u32 dll_469_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1440 | func: 6 | export: 6
u32 dll_469_get_data_size(Object *self, u32 a1) {
    return sizeof(ECSHshrine_Data);
}

// offset: 0x1454 | func: 7 | export: 7
void dll_469_func_1454(s16* arg0) {
    Object *object;
    ECSHshrine_Data* objdata;

    object = _data_40;
    if (object != NULL) {
        objdata = _data_40->data;
        *arg0 = objdata->unk10;
    }
}

// offset: 0x1484 | func: 8 | export: 8
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/469_ECSHshrine/dll_469_func_1484.s")
#else
// https://decomp.me/scratch/4UTLD
void dll_469_func_1484(u8 arg0, f32* arg1, f32* arg2) {
    if (_data_40 != NULL) {
        *arg1 = _data_0[_data_30[arg0]].unk0;
        *arg2 = _data_0[_data_30[arg0]].unk4;
    }
}
#endif

// offset: 0x14F4 | func: 9 | export: 9
void dll_469_func_14F4(s32* arg0, u8* arg1) {
    Object *object;
    ECSHshrine_Data* objdata;

    object = _data_40;
    if (object != NULL) {
        objdata = _data_40->data;
        *arg1 = objdata->unk1A;
        *arg0 = (s32) objdata->unk16;
    }
}

// offset: 0x152C | func: 10 | export: 10
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/469_ECSHshrine/dll_469_func_152C.s")
#else
// https://decomp.me/scratch/WuUl4
void dll_469_func_152C(u8 arg0, f32 arg1, f32 arg2) {
    if (_data_40 != NULL) {
        _data_0[_data_30[arg0]].unk0 = arg1;
        _data_0[_data_30[arg0]].unk4 = arg2;
    }
}
#endif

// offset: 0x1590 | func: 11 | export: 11
void dll_469_func_1590(u8 arg0) {
    Object *self;
    ECSHshrine_Data *objdata;

    self = _data_40;
    if (self != NULL) {
        self = _data_40;
        objdata = _data_40->data;
        objdata->unk18 = arg0 == objdata->unk1A ? 1 : 0;
    }
}

// offset: 0x15E0 | func: 12
static int dll_469_func_15E0(Object *self, Object *a1, AnimObj_Data *a2, s8 a3) {
    ECSHshrine_Data *objdata;
    s32 i;

    objdata = self->data;
    a2->unk7A = -1;
    a2->unk62 = 0;
    if (objdata->unkC != 0) {
        objdata->unkA += objdata->unkC;
        if ((objdata->unkA < 2) && (objdata->unkC <= 0)) {
            objdata->unkA = 1;
            objdata->unkC = 0;
        } else if ((objdata->unkA >= 0x46) && (objdata->unkC >= 0)) {
            objdata->unkA = 0x46;
            objdata->unkC = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(3, objdata->unkA);
    }
    for (i = 0; i < a2->unk98; i++) {
        if (a2->unk8E[i] != 0) {
            switch (a2->unk8E[i]) {
            case 1:
                func_80000860(self, self, 0xCB, 0);
                break;
            case 2:
                if (D_80092A7C[0] == -1) {
                    func_80000860(self, self, 0x14, 0);
                } else {
                    func_80000860(self, self, D_80092A7C[0], 0);
                }
                break;
            case 3:
                objdata->unk1C = 1;
                break;
            case 5:
                main_set_bits(BIT_143, 0);
                objdata->unk1C = 2;
                main_set_bits(BIT_DB_Entered_Shrine_3, 1);
                break;
            case 6:
                objdata->unk1B = 7;
                objdata->unk1C = 3;
                main_set_bits(BIT_DB_Entered_Shrine_3, 1);
                break;
            case 7:
                main_set_bits(BIT_143, 1);
                break;
            case 8:
                main_set_bits(BIT_143, 0);
                objdata->unkC = -3;
                break;
            case 10:
                main_set_bits(BIT_DB_Triggered_In_Shrine_Spirit_Cutscene, 1);
                if (_data_3C == 0) {
                    _data_3C = func_8004A1E8(1);
                }
                break;
            case 9:
                main_set_bits(BIT_DB_Entered_Shrine_2, 1);
                break;
            case 11:
                objdata->unkA = 0x64;
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x30, 0x50, (s16) (u8) objdata->unkA, 0);
                break;
            case 13:
                gDLL_3_Animation->vtbl->func19(0x5A, 0x64, 0, 0x50);
                break;
            }
        }
        a2->unk8E[i] = 0;
    }
    return 0;
}

// offset: 0x193C | func: 13
static void dll_469_func_193C(Object *self) {
    Object *sender;
    u32 mesgID;
    void* mesgArg;
    ECSHshrine_Data *objdata;

    objdata = self->data;
    mesgArg = NULL;
    while (obj_recv_mesg(self, &mesgID, &sender, &mesgArg) != 0) {
        switch (mesgID) {
        case 0x30005:
            objdata->unk8 = -3;
            break;
        case 0x30006:
            objdata->unk8 = 0x10;
            break;
        }
    }
}
