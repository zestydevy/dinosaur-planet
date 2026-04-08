#include "common.h"
#include "dll.h"
#include "dlls/objects/210_player.h"
#include "sys/gfx/model.h"
#include "sys/objanim.h"
#include "dlls/objects/common/foodbag.h"
#include "game/objects/interaction_arrow.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
} WMWizard_Setup;

typedef struct {
    Vec3f unk0;
    f32 unkC; //could be a Vec3f as well?
    f32 unk10;
    f32 unk14;
    s16 unk18;
    s16 unk1A;
    s32 unk1C;
    s16 unk20;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    u8 unk26;
    u8 unk27;
    u8 unk28;
    u8 unk29;
    u16 unk2A;
} WMWizard_Data;

/*0x0*/ static u32 _data_0[] = {
    0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 
    0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 
    0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 
    0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 
    0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 
    0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 
    0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000,
    0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 
    0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 
    0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000, 
    0x0377ffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000
};
typedef struct {
    f32 unk0; //x
    f32 unk4; //z
    f32 unk8; //modAnimID, but as a float??
    f32 unkC; //modAnimID, but as a float??
    f32 unk10; //anim playback speed?
} WizardFloatsUnk; //RandomWalkData?

/*0xDC*/ static WizardFloatsUnk _data_DC[] = {
    {0,   0,  0,  0, 0.02}, 
    {79, 152, 20, 20, 0.01}, 
    {138,  -6, 20, 20, 0.02},
    {-73, -48, 20, 20, 0.02}, 
    {-248, -7,  0,  0, 0.02}, 
    {  0,  0,  0,  0, 0.02}
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
    return sizeof(WMWizard_Data);
}

// offset: 0x364 | func: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/598_WM_Wizard/dll_598_func_364.s")

// offset: 0x470 | func: 8
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/598_WM_Wizard/dll_598_func_470.s")
#else
int dll_598_func_470(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3) {
    Wizard_Data* objData;
    Object* player;
    s32 i;

    player = get_player();
    objData = self->data;
    self->unkAF |= 8;

    for (i = 0; i < animData->unk98; i++) {
        if (objData->unk27 == 1) {
            if (animData->unk8E[i] == 4) {
                ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 5);
            }
        } else if (objData->unk27 != 2) {
            if (animData->unk8E[i] == 1) {
                main_set_bits(BIT_WM_Played_Randorn_First_Meeting, 1);
                objData->unk26 = 1;
            } else if (animData->unk8E[i] == 3) {
                ((DLL_210_Player*)player->dll)->vtbl->func41(player, 0, 1);
                ((DLL_210_Player*)player->dll)->vtbl->add_magic(player, 5);
            }
        }
    }
    
    return 0;
}
#endif

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
int dll_598_func_650(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 arg3) {
    WMWizard_Data* objData;
    s32 i;

    objData = self->data;
    self->unkAF |= 8;

    for (i = 0; i < animData->unk98; i++) {
        switch (animData->unk8E[i]) {
        case 0:
            break;
        case 1:
            if (objData->unk29 >= 2) {
                main_set_bits(BIT_314, 1);
            }
            break;
        }
    }

    return 0;
}

// offset: 0x72C | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/598_WM_Wizard/dll_598_func_72C.s")

// offset: 0xA20 | func: 13
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/598_WM_Wizard/dll_598_func_A20.s")
#else
void dll_598_func_A20(Object* self) {
    WMWizard_Data* objData = self->data;
    Object* player = get_player(); //70
    Object* foodbag;
    s32 foodGamebit;
    s32 sp5C[3] = {0x166, 0x167, 0x256}; //5C, 60, 64
    f32 dx; //58
    f32 dz; //54
    f32 distance; //50
    s16 dYaw; //4E
    s16 temp;
    UnkFunc_80024108Struct sp30; //30-4B
    WizardFloatsUnk* walkData;

    self->srt.transl.y = objData->unk0.y;

    //Check if player offers food
    if (main_get_bits(BIT_1FC)) {
        self->unkAF &= ~8;
        if ((self->unkAF & 1) && (gDLL_1_cmdmenu->vtbl->func_F40() == BIT_Foodbag_Give)) {
            foodGamebit = gDLL_1_cmdmenu->vtbl->func_E2C(sp5C, 3);
            if (foodGamebit >= 0) {
                main_set_bits(BIT_4D1, 1);
                objData->unk29++;
                main_set_bits(BIT_310, 1);
                foodbag = ((DLL_210_Player*)player->dll)->vtbl->func66(player, 0xF);
                ((DLL_IFoodbag*)foodbag->dll)->vtbl->delete_food_by_gamebit(foodbag, foodGamebit);
                joy_set_button_mask(0, A_BUTTON);
            }
        }
        return;
    }
    
    self->unkAF |= 8;

    //Pick next walk destination
    if (objData->unk20 <= 0) {
        switch (rand_next(1, 4)) {
        case 1:
            objData->unk25 = objData->unk24;
            objData->unk24 = 1;
            objData->unk20 = 0x190;
            break;
        case 2:
            objData->unk25 = objData->unk24;
            objData->unk24 = 2;
            objData->unk20 = 0x190;
            break;
        case 3:
            objData->unk25 = objData->unk24;
            objData->unk24 = 3;
            objData->unk20 = 0x190;
            break;
        case 4:
            objData->unk25 = objData->unk24;
            objData->unk24 = 4;
            objData->unk20 = 0x190;
            break;
        case 5: //unreachable?
            objData->unk25 = objData->unk24;
            objData->unk24 = 5;
            objData->unk20 = 0x190;
            break;
        }
        return;
    }

    //Handle random walk movement
    if (objData->unk24 == 0xC) {
        walkData = &_data_DC[objData->unk25];
        dYaw = ((arctan2_f(walkData->unk0, walkData->unk4) & 0xFFFF) - (self->srt.yaw & 0xFFFF)) & 0xFFFF;
        diPrintf("diff %d\n", dYaw);
        if ((dYaw < -0x3E8) || (dYaw > 0x3E8)) {
            if (dYaw > 0) {
                self->srt.yaw += gUpdateRate * 100;
                return;
            } else {
                self->srt.yaw -= gUpdateRate * 100;
                return;
            }
        }
        
        func_80023D30(self, _data_DC[objData->unk25].unk8, 0.0f, 0);
        objData->unkC = _data_DC[objData->unk25].unk10;
        objData->unk24 = 0xD;
    } else if (objData->unk24 == 0xD) {
        if (func_80024108(self, objData->unkC, gUpdateRateF, &sp30) != 0) {
            walkData = &_data_DC[objData->unk25];
            if (walkData->unk8 == self->curModAnimId) {
                func_80023D30(self, walkData->unkC, 0.0f, 0);
                objData->unkC = _data_DC[objData->unk25].unk10;
            }
        }
        
        objData->unk20 -= gUpdateRate;
        if (objData->unk20 <= 0) {
            objData->unk20 = 0;
        }
    } else {
        walkData = &_data_DC[objData->unk24];
        dx = walkData->unk0 - (self->srt.transl.x - objData->unk0.x);
        dz = walkData->unk4 - (self->srt.transl.z - objData->unk0.z);
        distance = sqrtf(SQ(dx) + SQ(dz));
        dYaw = arctan2_f(dx, dz) - self->srt.yaw;
        
        if ((dYaw >= -0x3E8) && (dYaw <= 0x3E8)) {
            if (self->curModAnimId != 0x3B) {
                func_80023D30(self, 0x3B, 0.0f, 0);
                objData->unkC = 0.04f;
            }
            self->speed.x = (dx / distance) * 0.25f;
            self->speed.z = (dz / distance) * 0.25f;
            func_8002493C(self, 0.25f, &objData->unkC);
        } else {
            if (self->curModAnimId != 0xC) {
                func_80023D30(self, 0xC, 0.0f, 0);
                objData->unkC = 0.01f;
            }
            
            if (dYaw > 0) {
                self->srt.yaw += gUpdateRate * 300;
            } else {
                self->srt.yaw -= gUpdateRate * 300;
            }
        }
        
        if (distance < 4.0f) {
            objData->unk25 = objData->unk24;
            objData->unk24 = 0xC;
            self->speed.x = 0.0f;
            self->speed.z = 0.0f;
        }
        
        self->srt.transl.x += (self->speed.x * gUpdateRateF);
        self->srt.transl.z += (self->speed.z * gUpdateRateF);
        func_80024108(self, objData->unkC, gUpdateRateF, &sp30);
    }
}
#endif

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
    WMWizard_Data* objdata = self->data;
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

