#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dlls/objects/214_animobj.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/controller.h"
#include "sys/main.h"
#include "sys/map.h"
#include "functions.h"
#include "types.h"
#include "dll.h"

typedef struct {
    ObjCreateInfo base;
    s8 unk18;
} SHqueenearthwalker_CreateInfo;

typedef struct {
    u8 questProgress;
    u8 eatenWhiteMushrooms;
    u8 unk2;
    u8 unk3;
} SHqueenearthwalker_State;

static s32 SHqueenearthwalker_func_4F8(Object* a0, Object* a1, AnimObjState* a2, void* a3);

// offset: 0x0 | ctor
void SHqueenearthwalker_ctor(void *dll) { }

// offset: 0xC | dtor
void SHqueenearthwalker_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SHqueenearthwalker_create(Object* self, SHqueenearthwalker_CreateInfo* createInfo, s32 arg2) {
    SHqueenearthwalker_State* sp24;

    sp24 = self->state;
    self->srt.yaw = createInfo->unk18 << 8;
    self->unk0xbc = (ObjectCallback)SHqueenearthwalker_func_4F8;
    sp24->eatenWhiteMushrooms = main_get_bits(BIT_SH_Queen_EW_White_Mushrooms_Eaten);
    sp24->questProgress = main_get_bits(BIT_SH_Queen_EW_Quest_Progress);
}

// offset: 0xA0 | func: 1 | export: 1
void SHqueenearthwalker_update(Object* self) {
    SHqueenearthwalker_State* state;
    s32 prevQuestProgress;

    state = self->state;
    prevQuestProgress = state->questProgress;
    self->unk0xaf &= ~8;
    if (self->curModAnimId != 1) {
        func_80023D30(self, 1, 0.0f, 0);
    }
    func_80024108(self, 0.005f, delayByte, 0);
    switch (state->questProgress) {

    case 1:
        state->questProgress = 2;
        break;
    case 2:
        if (self->unk0xaf & 1) {
            set_button_mask(0, 0x8000);
            gDLL_3_Animation->vtbl->func17(1, self, -1);
            main_set_bits(BIT_SH_Move_Thorntail_Blocking_Hollow_Log, 1);
            state->questProgress = 3;
        }
        break;
    case 3:
        if (self->unk0xaf & 4) {
            if (gDLL_1_UI->vtbl->func7(BIT_Inventory_White_Mushrooms) != 0) {
                set_button_mask(0, 0x8000);
                state->eatenWhiteMushrooms += main_get_bits(BIT_Inventory_White_Mushrooms);
                if (state->eatenWhiteMushrooms <= 0) {
                    gDLL_3_Animation->vtbl->func17(3, self, -1);
                } else {
                    state->questProgress = 4U;
                    gDLL_30_Task->vtbl->mark_task_completed(0xB);
                    main_set_bits(BIT_SH_Move_Thorntail_Blocking_Swapstone, 1);
                }
                main_set_bits(BIT_Inventory_White_Mushrooms, 0);
                main_set_bits(BIT_SH_Queen_EW_White_Mushrooms_Eaten, state->eatenWhiteMushrooms);
            } else if (self->unk0xaf & 1) {
                set_button_mask(0, 0x8000);
                gDLL_3_Animation->vtbl->func17(4, self, -1);
            }
        }
        break;
    case 4:
        gDLL_3_Animation->vtbl->func17(2, self, -1);
        break;
    case 5:
        gDLL_3_Animation->vtbl->func17(6, self, -1);
        break;
    case 6:
        gDLL_3_Animation->vtbl->func17(7, self, -1);
        state->questProgress = 7;
        break;
    case 7:
        break;
    default:
        state->questProgress = 1;
        break;
    }
    if (prevQuestProgress != state->questProgress) {
        main_set_bits(BIT_SH_Queen_EW_Quest_Progress, state->questProgress);
    }
}

// offset: 0x464 | func: 2 | export: 2
void SHqueenearthwalker_func_464(Object *self) { }

// offset: 0x470 | func: 3 | export: 3
void SHqueenearthwalker_draw(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x4C4 | func: 4 | export: 4
void SHqueenearthwalker_destroy(Object *self, s32 a1) { }

// offset: 0x4D4 | func: 5 | export: 5
s32 SHqueenearthwalker_get_setup_flags(Object* self) {
    return 0;
}

// offset: 0x4E4 | func: 6 | export: 6
u32 SHqueenearthwalker_get_state_size(Object *self, u32 a1) {
    return sizeof(SHqueenearthwalker_State);
}

// offset: 0x4F8 | func: 7
static s32 SHqueenearthwalker_func_4F8(Object* a0, Object* a1, AnimObjState* a2, void* a3) {
    SHqueenearthwalker_State* state;
    s32 sp40;
    s32 i;

    state = a0->state;
    sp40 = state->questProgress;
    a0->unk0xaf |= 8;
    a2->unk62 = 0;

    for (i = 0; i < a2->unk98; i++) {
        switch (a2->unk8E[i]) {
        case 3:
            state->questProgress = 5;
            gDLL_29_Gplay->vtbl->set_map_setup(MAP_VOLCANO_FORCE_POINT_TEMPLE, 11);
            main_set_bits(BIT_Play_Seq_0298_Queen_Shows_VFPT, 1);
            warpPlayer(WARP_VFP_CALDERA_LOWER, /*fadeToBlack=*/FALSE);
            break;
        case 4:
            state->questProgress = 6;
            gDLL_29_Gplay->vtbl->set_map_setup(MAP_VOLCANO_FORCE_POINT_TEMPLE, 11);
            main_set_bits(BIT_Play_Seq_0299_Queen_Shows_SpellStones, 1);
            warpPlayer(WARP_VFP_CALDERA_LOWER, /*fadeToBlack=*/FALSE);
            break;
        }
    }

    if (sp40 != state->questProgress) {
        main_set_bits(BIT_SH_Queen_EW_Quest_Progress, state->questProgress);
    }
    
    return 0;
}
