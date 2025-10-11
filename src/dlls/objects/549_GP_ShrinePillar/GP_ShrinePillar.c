#include "dlls/objects/214_animobj.h"
#include "sys/objanim.h"
#include "segment_334F0.h"

typedef struct {
    ObjCreateInfo base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s8 unk1E;
    u8 unk1F;
    u8 unk20;
    s16 unk22;
} GP_ShrinePillar_CreateInfo;

typedef struct {
    u8 unk0;
    u8 unk1;
    f32 unk4;
} GP_ShrinePillar_State;

static int dll_549_func_264(Object* a0, Object* a1, AnimObjState* a2, void* a3);

// offset: 0x0 | ctor
void dll_549_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_549_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_549_create(Object* arg0, GP_ShrinePillar_CreateInfo* arg1, s32 arg2) {
    s32* temp_v0;
    GP_ShrinePillar_State* state;

    state = (GP_ShrinePillar_State*)arg0->state;
    if (arg1->unk1A != -1) {
        if (main_get_bits(arg1->unk1A) != 0) {
            state->unk0 = 6;
        }
    } else {
        state->unk0 = 0;
    }
    state->unk1 = 1;
    arg0->srt.yaw = arg1->unk1F << 8;
    arg0->unk0xbc = (ObjectCallback)dll_549_func_264;
    temp_v0 = func_800348A0(arg0, 0, 0);
    if (temp_v0 != NULL) {
        *temp_v0 = 0;
    }
}

// offset: 0xE0 | func: 1 | export: 1
void dll_549_update(Object* self) {
    GP_ShrinePillar_State* state;
    GP_ShrinePillar_CreateInfo* createInfo;
    s32 var_a2;

    state = (GP_ShrinePillar_State*)self->state;
    createInfo = (GP_ShrinePillar_CreateInfo*)self->createInfo;
    diPrintf("control\n");
    if (state->unk1 != 0) {
        if ((createInfo->unk1C != 0) && (state->unk0 != 0)) {
            var_a2 = createInfo->unk20;
            gDLL_3_Animation->vtbl->func20(self, createInfo->unk1C);
        } else {
            var_a2 = -1; 
        }
        if (createInfo->unk1E != -1) {
            gDLL_3_Animation->vtbl->func17(createInfo->unk1E, self, var_a2);
        }
        state->unk1 = 0;
    }
}

static const char str_1[] = " PREMPT %i seqtime %i \n\n";

// offset: 0x1D0 | func: 2 | export: 2
void dll_549_func_1D0(Object *self) { }

// offset: 0x1DC | func: 3 | export: 3
void dll_549_draw(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x230 | func: 4 | export: 4
void dll_549_destroy(Object *self, s32 a1) { }

// offset: 0x240 | func: 5 | export: 5
u32 dll_549_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x250 | func: 6 | export: 6
u32 dll_549_get_state_size(Object *self, u32 a1) {
    return sizeof(GP_ShrinePillar_State);
}

// offset: 0x264 | func: 7
int dll_549_func_264(Object* a0, Object* a1, AnimObjState* a2, void* a3) {
    GP_ShrinePillar_CreateInfo* createInfo;
    GP_ShrinePillar_State* state;
    s32* temp_v0_2;
    s32* temp_v0;
    s32 var_v1_3;
    s32 var_v0_2;
    s32 var_v1_4;

    state = (GP_ShrinePillar_State*)a0->state;
    createInfo = (GP_ShrinePillar_CreateInfo*)a0->createInfo;
    diPrintf("override %d\n", state->unk0);
    switch (state->unk0) {
    case 0:
        if (main_get_bits(createInfo->unk18) != 0) {
            state->unk0 = 1;
        }
        break;
    case 1:
        for (var_v0_2 = 0; var_v0_2 < a2->unk98; var_v0_2++) {
            if (a2->unk8E[var_v0_2] == 1) {
                state->unk0 = 6;
                if (createInfo->unk1A != -1) {
                    main_set_bits(createInfo->unk1A, 1);
                }
            }
        }
        break;
    case 6:
        if (main_get_bits(createInfo->unk22) != 0) {
            state->unk0 = 7;
        }
        break;
    case 7:
        for (var_v0_2 = 0; var_v0_2 < a2->unk98; var_v0_2++) {
            if (a2->unk8E[var_v0_2] == 2) {
                state->unk0 = 2;
            }
        }
        break;
    case 2:
        if (func_80025F40(a0, NULL, NULL, NULL) == 0x19) {
            state->unk0 = 4;
        }
        break;
    case 3:
        state->unk4 -= delayFloat;
        if (state->unk4 <= 0.0f) {
            state->unk0 = 5;
        }
        break;
    case 4:
        temp_v0 = func_800348A0(a0, 0, 0);
        if (temp_v0 != NULL) {
            var_v1_3 = *temp_v0 + (delayByte * 8);
            if (var_v1_3 > 0x100) {
                var_v1_3 = 0x100;
                state->unk0 = 3;
                state->unk4 = 800.0f;
            }
            *temp_v0 = var_v1_3;
        }
        break;
    case 5:
        temp_v0_2 = func_800348A0(a0, 0, 0);
        if (temp_v0_2 != NULL) {
            var_v1_4 = *temp_v0_2 - (delayByte * 8);
            if (var_v1_4 < 0) {
                var_v1_4 = 0;
                state->unk0 = 2;
            }
            *temp_v0_2 = var_v1_4;
        }
        break;
    }
    if ((state->unk0 == 1) || (state->unk0 == 7) || (state->unk0 == 8)) {
        return 0;
    } else {
        return 1;
    }
}
