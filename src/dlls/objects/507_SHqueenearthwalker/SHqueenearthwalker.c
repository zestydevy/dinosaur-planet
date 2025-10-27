#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dlls/objects/214_animobj.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "sys/controller.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/map_enums.h"
#include "sys/objanim.h"
#include "functions.h"
#include "types.h"
#include "dll.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
} SHqueenearthwalker_Setup;

typedef struct {
    u8 questProgress;
    u8 eatenWhiteMushrooms;
    u8 unk2;
    u8 unk3;
} SHqueenearthwalker_Data;

static int SHqueenearthwalker_func_4F8(Object* a0, Object* a1, AnimObj_Data* a2, s8 a3);

// offset: 0x0 | ctor
void SHqueenearthwalker_ctor(void *dll) { }

// offset: 0xC | dtor
void SHqueenearthwalker_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SHqueenearthwalker_setup(Object* self, SHqueenearthwalker_Setup* setup, s32 arg2) {
    SHqueenearthwalker_Data* objdata;

    objdata = self->data;
    self->srt.yaw = setup->unk18 << 8;
    self->animCallback = SHqueenearthwalker_func_4F8;
    objdata->eatenWhiteMushrooms = main_get_bits(BIT_SH_Queen_EW_White_Mushrooms_Eaten);
    objdata->questProgress = main_get_bits(BIT_SH_Queen_EW_Quest_Progress);
}

// offset: 0xA0 | func: 1 | export: 1
void SHqueenearthwalker_control(Object* self) {
    SHqueenearthwalker_Data* objdata;
    s32 prevQuestProgress;

    objdata = self->data;
    prevQuestProgress = objdata->questProgress;
    self->unkAF &= ~8;
    if (self->curModAnimId != 1) {
        func_80023D30(self, 1, 0.0f, 0);
    }
    func_80024108(self, 0.005f, delayByte, NULL);
    switch (objdata->questProgress) {

    case 1:
        objdata->questProgress = 2;
        break;
    case 2:
        if (self->unkAF & 1) {
            set_button_mask(0, A_BUTTON);
            gDLL_3_Animation->vtbl->func17(1, self, -1);
            main_set_bits(BIT_SH_Move_Thorntail_Blocking_Hollow_Log, 1);
            objdata->questProgress = 3;
        }
        break;
    case 3:
        if (self->unkAF & 4) {
            if (gDLL_1_UI->vtbl->func7(BIT_Inventory_White_Mushrooms) != 0) {
                set_button_mask(0, A_BUTTON);
                objdata->eatenWhiteMushrooms += main_get_bits(BIT_Inventory_White_Mushrooms);
                if (objdata->eatenWhiteMushrooms <= 0) {
                    gDLL_3_Animation->vtbl->func17(3, self, -1);
                } else {
                    objdata->questProgress = 4U;
                    gDLL_30_Task->vtbl->mark_task_completed(0xB);
                    main_set_bits(BIT_SH_Move_Thorntail_Blocking_Swapstone, 1);
                }
                main_set_bits(BIT_Inventory_White_Mushrooms, 0);
                main_set_bits(BIT_SH_Queen_EW_White_Mushrooms_Eaten, objdata->eatenWhiteMushrooms);
            } else if (self->unkAF & 1) {
                set_button_mask(0, A_BUTTON);
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
        objdata->questProgress = 7;
        break;
    case 7:
        break;
    default:
        objdata->questProgress = 1;
        break;
    }
    if (prevQuestProgress != objdata->questProgress) {
        main_set_bits(BIT_SH_Queen_EW_Quest_Progress, objdata->questProgress);
    }
}

// offset: 0x464 | func: 2 | export: 2
void SHqueenearthwalker_update(Object *self) { }

// offset: 0x470 | func: 3 | export: 3
void SHqueenearthwalker_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x4C4 | func: 4 | export: 4
void SHqueenearthwalker_free(Object *self, s32 a1) { }

// offset: 0x4D4 | func: 5 | export: 5
u32 SHqueenearthwalker_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x4E4 | func: 6 | export: 6
u32 SHqueenearthwalker_get_data_size(Object *self, u32 a1) {
    return sizeof(SHqueenearthwalker_Data);
}

// offset: 0x4F8 | func: 7
static int SHqueenearthwalker_func_4F8(Object* a0, Object* a1, AnimObj_Data* a2, s8 a3) {
    SHqueenearthwalker_Data* objdata;
    s32 sp40;
    s32 i;

    objdata = a0->data;
    sp40 = objdata->questProgress;
    a0->unkAF |= 8;
    a2->unk62 = 0;

    for (i = 0; i < a2->unk98; i++) {
        switch (a2->unk8E[i]) {
        case 3:
            objdata->questProgress = 5;
            gDLL_29_Gplay->vtbl->set_map_setup(MAP_VOLCANO_FORCE_POINT_TEMPLE, 11);
            main_set_bits(BIT_Play_Seq_0298_Queen_Shows_VFPT, 1);
            warpPlayer(WARP_VFP_CALDERA_LOWER, /*fadeToBlack=*/FALSE);
            break;
        case 4:
            objdata->questProgress = 6;
            gDLL_29_Gplay->vtbl->set_map_setup(MAP_VOLCANO_FORCE_POINT_TEMPLE, 11);
            main_set_bits(BIT_Play_Seq_0299_Queen_Shows_SpellStones, 1);
            warpPlayer(WARP_VFP_CALDERA_LOWER, /*fadeToBlack=*/FALSE);
            break;
        }
    }

    if (sp40 != objdata->questProgress) {
        main_set_bits(BIT_SH_Queen_EW_Quest_Progress, objdata->questProgress);
    }
    
    return 0;
}
