#include "common.h"
#include "dlls/objects/210_player.h"
#include "sys/gfx/model.h"
#include "sys/objanim.h"
#include "dlls/objects/common/foodbag.h"
#include "game/objects/interaction_arrow.h"

typedef struct {
    u8 _unk0[0x29];
    u8 unk29;
    u16 unk2A;
} Wizard_Data;

/*0x0*/ static u32 _data_0[] = {
    0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x0377ffff, 0xffffffff, 0x00000000, 
    0x00000000, 0x00000000, 0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x0377ffff, 
    0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 
    0x00000000, 0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x0377ffff, 0xffffffff, 
    0x00000000, 0x00000000, 0x00000000, 0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 
    0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 0x0377ffff, 0xffffffff, 0x00000000, 
    0x00000000, 0x00000000, 0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000
};
/*0xDC*/ static u32 _data_DC[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3ca3d70a, 0x429e0000, 0x43180000, 0x41a00000, 
    0x41a00000, 0x3c23d70a, 0x430a0000, 0xc0c00000, 0x41a00000, 0x41a00000, 0x3ca3d70a, 0xc2920000, 
    0xc2400000, 0x41a00000, 0x41a00000, 0x3ca3d70a, 0xc3780000, 0xc0e00000, 0x00000000, 0x00000000, 
    0x3ca3d70a, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3ca3d70a
};
/*0x154*/ static u32 _data_154[] = {
    0x00000166, 0x00000167, 0x00000256
};

// offset: 0x0 | ctor
void dll_598_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_598_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_598_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/598_WM_Wizard/dll_598_setup.s")

// offset: 0xE4 | func: 1 | export: 1
void dll_598_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/598_WM_Wizard/dll_598_control.s")

// offset: 0x238 | func: 2 | export: 2
void dll_598_update(Object *self) { }

// offset: 0x244 | func: 3 | export: 3
void dll_598_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        if (gDLL_29_Gplay->vtbl->get_map_setup((s32) self->mapID) == 4) {
            if (main_get_bits(BIT_2BD) != 0) {
                draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
            }
        } else {
            draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        }
    }
}

// offset: 0x330 | func: 4 | export: 4
void dll_598_free(Object *self, s32 a1) { }

// offset: 0x340 | func: 5 | export: 5
u32 dll_598_get_model_flags(Object* self) {
    return MODFLAGS_1;
}

// offset: 0x350 | func: 6 | export: 6
u32 dll_598_get_data_size(Object *self, u32 a1) {
    return sizeof(Wizard_Data);
}

// offset: 0x364 | func: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/598_WM_Wizard/dll_598_func_364.s")

// offset: 0x470 | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/598_WM_Wizard/dll_598_func_470.s")

// offset: 0x610 | func: 9
s32 dll_598_func_610(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return 0;
}

// offset: 0x62C | func: 10
s32 dll_598_func_62C(Object* arg0, s32 arg1, s32 arg2, s32 arg3) {
    arg0->unkAF |= 8;
    return 0;
}

// offset: 0x650 | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/598_WM_Wizard/dll_598_func_650.s")

// offset: 0x72C | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/598_WM_Wizard/dll_598_func_72C.s")

// offset: 0xA20 | func: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/598_WM_Wizard/dll_598_func_A20.s")

// offset: 0x1124 | func: 14
void dll_598_func_1124(Object* self) {

    self->unkAF |= 8;
    if (self->curModAnimId != 2) {
        func_80023D30(self, 2, 0.0f, 0U);
    }
    func_80024108(self, 0.005f, gUpdateRate, NULL);
}

// offset: 0x11C8 | func: 15
void dll_598_func_11C8(Object* self) {
    Wizard_Data* objdata = self->data;
    Object* player = get_player();
    s32 bit;
    Object* foodbag; 
    s32 sp24[3] = {BIT_Green_Apple_Count, BIT_Red_Apple_Count, BIT_Brown_Apple_Count};

    if (self->unkAF & ARROW_FLAG_8_No_Targetting) {
        self->unkAF ^= ARROW_FLAG_8_No_Targetting;
    }
    if (main_get_bits(BIT_2FB) == 0) {
        if (self->curModAnimId != 7) {
            func_80023D30(self, 7, 0.0f, 0);
        }

        func_80024108(self, 0.005f, gUpdateRate, NULL);
    } else {
        if (self->curModAnimId != 2) {
            func_80023D30(self, 2, 0.0f, 0); 
        }
    
        func_80024108(self, 0.005f, gUpdateRate, NULL);
    }
    if ((self->unkAF & 1) && (main_get_bits(BIT_2FB) == 0)) {
        main_set_bits(BIT_2FB, 1);
        objdata->unk29 = 0;
        joy_set_button_mask(0, A_BUTTON);
    } else if ((self->unkAF & 1) && (gDLL_1_cmdmenu->vtbl->func_F40() == BIT_Foodbag_Give)) {
        bit = gDLL_1_cmdmenu->vtbl->func_E2C(sp24, ARRAYCOUNT(sp24));

        // @fake
        if (objdata) {}
        if (bit >= 0) {
            main_set_bits(BIT_310, 1);
            objdata->unk29++;
            foodbag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 15);
            ((DLL_IFoodbag*)foodbag->dll)->vtbl->delete_food_by_gamebit(foodbag, bit);
            joy_set_button_mask(0, A_BUTTON);
        }
    }
}

