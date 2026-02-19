#include "common.h"
#include "dlls/objects/210_player.h"
#include "dlls/engine/6_amsfx.h"

typedef struct {
/*0*/ s16 unk0;
/*4*/ f32 unk4;
/*8*/ s16 unk8;
/*A*/ s16 unkA;
/*C*/ s32 unkC;
} DLL525_Data;

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 _unk18;
/*19*/ s8 _unk19;
/*1A*/ s8 unk1A;
/*1B*/ u8 unk1B;
}DLL525_Setup;

static s32 dll_525_func_684(Object* arg0, void* arg1);
static int dll_525_func_81C(Object* arg0, Object* arg1, AnimObj_Data*arg2, s8 arg3);

// offset: 0x0 | ctor
void dll_525_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_525_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_525_setup(Object* self, DLL525_Setup* setup, s32 arg2) {
    DLL525_Data* DLL515Data;
    s16 temp_v1;

    DLL515Data = self->data;
    self->modelInstIdx = setup->unk1B;
    if (self->modelInstIdx < 0 || self->modelInstIdx >= 6) {
        self->modelInstIdx = 0;
    }
    self->srt.yaw = -0x7FFF;
    DLL515Data->unk8 = ((u16)self->srt.yaw) / 8192; // TODO: that cast is sus
    temp_v1 = rand_next(7, 0xA) * 0xA;
    DLL515Data->unk0 = temp_v1;
    DLL515Data->unk4 = temp_v1;
    if (rand_next(0, 1) != 0) {
        DLL515Data->unkA = 1;
    }
    self->animCallback = dll_525_func_81C;
    self->unkB0 |= 0x6000;
}

// offset: 0x114 | func: 1 | export: 1
void dll_525_control(Object* self) {
    s32 pad;
    s32 sp48;
    s32 sp44;
    Object* sp40;
    DLL525_Data* dll525Data;
    
    if (1){}

    dll525Data = self->data;
    if (self->modelInstIdx == 5) {
        if (main_get_bits(BIT_639) == 0) {
            gDLL_3_Animation->vtbl->func17(0, self, -1);
            return;
        }
        sp40 = get_player();
        if (((DLL_210_Player*)sp40->dll)->vtbl->get_vehicle(sp40) != 0) {
            main_set_bits(BIT_63B, 1U);
            return;
        }
        main_set_bits(BIT_63B, 0U);
        return;
    }
    if (main_get_bits(BIT_639) == 0) {
        dll525Data->unk4 -= gUpdateRateF;
        if (func_80025F40(self, &sp40, &sp48, &sp44) != 0) {
            gDLL_6_AMSFX->vtbl->play_sound(self, 0x797U, 0x7FU, NULL, NULL, 0, NULL);
            dll525Data->unkA = (s16) (dll525Data->unkA ^ 2);
            if (dll525Data->unkA & 2) {
                main_set_bits(BIT_639, dll_525_func_684(self, dll525Data));
            }
        }
        if (!(dll525Data->unkA & 2)) {
            if (dll525Data->unkA & 1) {
                if (dll525Data->unk4 < 0.0f) {
                    dll525Data->unk4 = dll525Data->unk0;
                    dll525Data->unk8 ++;
                    if (dll525Data->unk8 >= 8) {
                        dll525Data->unk8 = 0;
                    }
                    dll525Data->unkC = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_796, rand_next(0x40, 0x7F) , NULL, NULL, 0, NULL);
                    gDLL_6_AMSFX->vtbl->func_954(dll525Data->unkC, ((f32) rand_next(-0x32, 0x32) / 100.0f) + 1.0f);
                }
                if (((s32) (self->srt.yaw & 0xFFFF) / 8192) != dll525Data->unk8) {
                    self->srt.yaw = (s16) (s32) ((f32) self->srt.yaw + (512.0f * gUpdateRateF));
                }
            } else {
                if (dll525Data->unk4 < 0.0f) {
                    dll525Data->unk4 = dll525Data->unk0;
                    dll525Data->unk8 --;
                    if (dll525Data->unk8 < 0) {
                        dll525Data->unk8 = 7;
                    }
                    dll525Data->unkC = gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_796, rand_next(0x40, 0x7F) , NULL, NULL, 0, NULL);
                    gDLL_6_AMSFX->vtbl->func_954(dll525Data->unkC, ((f32) rand_next(-0x32, 0x32) / 100.0f) + 1.0f);
                }
                if (((s32) (self->srt.yaw & 0xFFFF) / 8192) != dll525Data->unk8) {
                    self->srt.yaw = (s16) (s32) ((f32) self->srt.yaw - (512.0f * gUpdateRateF));
                }
            }
        }
    }
}

// offset: 0x598 | func: 2 | export: 2
void dll_525_update(Object *self) { }

// offset: 0x5A4 | func: 3 | export: 3
void dll_525_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x5F8 | func: 4 | export: 4
void dll_525_free(Object* self, s32 a1) {
    DLL525_Data* dll525Data;
    u32 temp_a1;

    dll525Data = self->data;
    temp_a1 = dll525Data->unkC;
    if (temp_a1 != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(temp_a1);
        dll525Data->unkC = 0U;
    }
}

// offset: 0x660 | func: 5 | export: 5
u32 dll_525_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x670 | func: 6 | export: 6
u32 dll_525_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL525_Data);
}

// offset: 0x684 | func: 7
static s32 dll_525_func_684(Object* arg0, void* arg1) {
    Object* temp_v1;
    Object** temp_s6;
    s32 i;
    s32 sp58;
    s32 var_s1;
    DLL525_Data* dll525Data;
    
    var_s1 = 0;
    temp_s6 = get_world_objects(&i, &sp58);
    for ( ; i < sp58; i++) {
        temp_v1 = temp_s6[i];
        if (temp_v1->id == OBJ_SC_totempuzzle) {
            dll525Data = temp_v1->data;
            if (dll525Data->unkA & 2) {
                if (dll525Data->unkA & 1) {
                    if (dll525Data->unk8 == 4) {
                        var_s1 += 1;
                        if (arg0 == temp_v1) {
                            gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_798_Puzzle_Solved, MAX_VOLUME, NULL, NULL, 0, NULL);
                        }
                    }
                } else if (dll525Data->unk8 == 3) {
                    var_s1 += 1;
                    if (arg0 == temp_v1) {
                        gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_798_Puzzle_Solved, MAX_VOLUME, NULL, NULL, 0, NULL);
                    }
                }
            }
        } 
    }
    if (var_s1 == 5) {
        return 1;
    }
    return 0;
}

// offset: 0x81C | func: 8
static int dll_525_func_81C(Object* arg0, Object* arg1, AnimObj_Data*arg2, s8 arg3) {
    s32 var_v1;

    if (main_get_bits(BIT_639) != 0) {
        var_v1 = 0;
    } else {
        var_v1 = 1;
    }
    return var_v1;
}
