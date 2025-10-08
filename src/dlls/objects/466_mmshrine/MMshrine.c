#include "PR/ultratypes.h"
#include "functions.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "dll.h"
#include "sys/main.h"
#include "sys/objects.h"

s32 obj_recv_mesg(Object *obj, u32 *outMesgID, Object **outSender, void **outMesgArg);
Texture* func_8004A1E8(s32 match_value);
u16 get_masked_buttons(int port);

extern s32 D_80092A7C;

typedef struct {
/*00*/ s16 unk0;
/*02*/ s16 unk2;
/*04*/ s16 unk4;
/*06*/ s16 unk6;
/*08*/ union {
    s16 unk8_s16;
    struct {
        u8 unk8;
        u8 unk9;
    };
};
/*0A*/ s16 unkA;
/*0C*/ s16 unkC;
/*0E*/ s8 unkE;
/*0F*/ u8 unkF;
/*10*/ s8 unk10;
/*11*/ s8 unk11;
/*12*/ s8 unk12;
/*13*/ s8 unk13;
} ShrineState;

typedef struct {
/*00*/ ObjCreateInfo base;
/*18*/ s16 unk18;
/*1A*/ s16 unk1A;
} ShrineCreateInfo;

typedef struct {
/*00*/ s8 unk0[0x62 - 0];
/*62*/ s8 unk62;
/*63*/ s8 unk63;
/*00*/ s8 unk64[0x7A - 0x64];
/*7A*/ s16 unk7A;
/*00*/ s8 unk7C[0x8E - 0x7C];
/*7A*/ u8 unk8E[0x98 - 0x8E];
/*7A*/ u8 unk98;
} UnkShrineStruct;

/*0x0*/ static Texture* _data_0 = 0;
/*0x4*/ static u32 _data_4[] = {
    0x01000000, 0x00000000, 0x00000000
};

// offset: 0x0 | ctor
void MMshrine_ctor(void *dll) {
}

// offset: 0xC | dtor
void MMshrine_dtor(void *dll) {
}

// offset: 0x18 | func: 0 | export: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/466_mmshrine/dll_466_func_18.s")

// offset: 0x1BC | func: 1 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/466_mmshrine/dll_466_func_1BC.s")

// offset: 0xB3C | func: 2 | export: 2
void MMshrine_func_B3C(Object *self) {
}

// offset: 0xB48 | func: 3 | export: 3
void MMshrine_draw(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xB9C | func: 4 | export: 4
void MMshrine_destroy(Object *self, s32 arg1) {
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_5_AMSEQ->vtbl->func13(3, 0);
    gDLL_5_AMSEQ->vtbl->func13(2, 0);
}

// offset: 0xC2C | func: 5 | export: 5
s32 MMshrine_func_C2C(Object *self) {
    return 0;
}

// offset: 0xC3C | func: 6 | export: 6
s32 MMshrine_get_state_size(Object *self, s32 arg1) {
    return 0x14;
}

// offset: 0xC50 | func: 7
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/466_mmshrine/dll_466_func_C50.s")
#else
s32 dll_466_func_C50(Object* self, s32 arg1, UnkShrineStruct* arg2, s32 arg3) {
    ShrineState* state;
    Object* player;
    s16 temp_v0;
    UnkShrineStruct* var_s2;
    s32 var_s3;

    state = self->state;
    player = get_player();
    
    arg2->unk7A = -1;
    arg2->unk62 = 0;
    
    if (state->unkA) {
        state->unk8_s16 += state->unkA;
        if (state->unk8_s16 < 2 && state->unkA <= 0) {
            state->unk8_s16 = 1;
            state->unkA = 0;
        } else if (state->unk8_s16 >= 0x46 && state->unkA >= 0) {
            state->unk8_s16 = 0x46;
            state->unkA = 0;
        }
        gDLL_5_AMSEQ->vtbl->func13(3, (u8)state->unk8_s16);
    }

    for (var_s3 = 0; var_s3 < arg2->unk98; var_s3++){
        switch (arg2->unk8E[var_s3]) {
            case 1:
                func_80000860(self, self, 0xC7, 0);
                break;
            case 2:
                if (D_80092A7C == -1) {
                    func_80000860(self, self, 0x14, 0);
                } else {
                    func_80000860(self, self, D_80092A7C, 0);
                }
                break;
            case 3:
                state->unk10 = 1;
                break;
            case 4:
                state->unk2 = 0;
                break;
            case 5:
                state->unkF = 5;
                state->unk10 = 2;
                set_gplay_bitstring(BIT_DB_Entered_Shrine_3, 1);
                break;
            case 6:
                state->unk10 = 3;
                set_gplay_bitstring(BIT_DB_Entered_Shrine_3, 1);
                break;
            case 7:
                set_gplay_bitstring(BIT_MMP_GP_Shrine_Spirit_Light_Beams, 1);
                break;
            case 8:
                set_gplay_bitstring(BIT_MMP_GP_Shrine_Spirit_Light_Beams, 0);
                state->unkA = -3;
                break;
            case 10:
                set_gplay_bitstring(BIT_MMP_GP_Shrine_Spirit_Light_Beams, 1);
                if (_data_0 == NULL)
                    _data_0 = func_8004A1E8(1);
                break;
            case 9:
                set_gplay_bitstring(BIT_DB_Entered_Shrine_2, 1);
                break;
            case 11:
                state->unk8_s16 = 0x64;
                gDLL_5_AMSEQ->vtbl->func5.withFiveArgs(3, 0x2f, 0x50, ((u8)state->unk8_s16 << 0x10) >> 0x10, 0);
                break;
            case 12:
                func_80000860(self, self, 0xCE, 0);
                set_gplay_bitstring(BIT_Test_of_Fear_Particles, 1);
                gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_342, MAX_VOLUME, 0, 0, 0, 0);
                break;
            case 13:
                if (D_80092A7C == -1) {
                    func_80000860(self, self, 0x14, 0);
                } else {
                    func_80000860(self, self, D_80092A7C, 0);
                }
                set_gplay_bitstring(BIT_Test_of_Fear_Particles, 0);
                break;
        }
        arg2->unk8E[var_s3] = 0;
    }
    
    if (state->unkF == 8) {
        if (vec3_distance(&self->positionMirror, &player->positionMirror) > 10.0f) {
            gDLL_3_Animation->vtbl->func18(arg2->unk63);
            state->unkF = 7;
        } else if (get_masked_buttons(0)) {
            gDLL_3_Animation->vtbl->func18(arg2->unk63);
            state->unkF = 7;
        }
    }
    
    return 0;
}

#endif

// offset: 0x1140 | func: 8
void MMshrine_func_1140(Object *arg0) {
    Object *sp54;
    u32 message;
    void *outMesgArg;
    ShrineState *state;

    state = arg0->state;
    outMesgArg = 0;

    while (obj_recv_mesg(arg0, &message, &sp54, &outMesgArg)){
        switch (message) {
            case 0x30005:
                state->unk6 = -3;
                break;
            case 0x30006:
                state->unk6 = 0x10;
                break;
        }
    }
}
