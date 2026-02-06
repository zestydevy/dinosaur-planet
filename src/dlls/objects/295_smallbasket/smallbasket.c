#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "game/objects/object.h"
#include "sys/gfx/model.h"
#include "types.h"

#include "PR/ultratypes.h"
#include "dll.h"
#include "game/objects/object.h"
#include "sys/dll.h"
#include "sys/rand.h"
#include "sys/objects.h"
#include "sys/main.h"
#include "sys/objtype.h"
#include "functions.h"
#include "sys/joypad.h"
#include "sys/objmsg.h"
#include "dlls/objects/210_player.h"


 typedef struct{
/*0x00*/   s32 unk0;
/*0x04*/   s32 unk4;
/*0x08*/   u32 soundHandle;
/*0xC*/    s16 unkC;
/*0xE*/    s16 unkE;
/*0x10*/   s16 unk10;
/*0x12*/   s16 unk12;
/*0x14*/   s16 shakeSoundTimer;
/*0x16*/   s16 unk16;
/*0x18*/   s8 unk18;
/*0x19*/   s8 unk19;
/*0x1A*/   s16 unk1A;
/*0x1C*/   s16 _unk1C;
/*0x1E*/   s16 unk1E;
/*0x20*/   s8 unk20;
/*0x21*/   s8 unk21;
/*0x22*/   s8 unk22;
/*0x23*/   s8 unk23;
/*0x24*/   s8 unk24;
/*0x25*/   u8 unk25;
/*0x26*/   s8 unk26;
/*0x27*/   s8 unk27;
} SmallBasket_Data;

typedef struct {
/*0x00*/   ObjSetup base;
/*0x18*/   s8 yaw;
/*0x19*/   s8 storedItemID;
/*0x1A*/   s16 storedItemQuantity;
/*0x1C*/   s16 unk1C;
/*0x1E*/   s16 unk1E;
/*0x20*/   s16 autoThrowRadius;
} SmallBasket_Setup;

/*0x0*/
    static DLL_Unknown* _data_0[] = {
    0, 0, 0, 0, 0, 0, 0, 0
};

//static void dll_295_func_1104(Object* arg0, Object* arg1, SmallBasket_Data* arg2);//

/*0x0*/ static u8 _bss_0[0x10];

// offset: 0x0 | ctor
void dll_295_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_295_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_295_setup(Object* self, SmallBasket_Setup* setup, s32 arg2) {
    s16 temp_v0;
    SmallBasket_Data* temp_s0;

    temp_s0 = self->data;
    self->objhitInfo->unk44 = 0x10;
    self->objhitInfo->unk40 = 0x10;
    func_800267A4(self);
    obj_add_object_type(self, 0x12);
    temp_v0 = setup->unk1C;
    if (temp_v0 == 0) {
        temp_s0->unk4 = 0;
    } else {
        temp_s0->unk4 = (s32) (temp_v0 * 0x3C);
    }
    *_data_0 = dll_load_deferred(0x1003U, 1U);
    temp_s0->shakeSoundTimer = (s16) (rand_next(0, 0x64) + 0x12C);
    temp_s0->unk26 = (s8) setup->storedItemQuantity;
    self->srt.yaw = setup->yaw << 8;
    temp_s0->unk1E = (s16) setup->unk1E;
    temp_s0->unk12 = (s16) setup->autoThrowRadius;
    if (temp_s0->unk12 == 0) {
        temp_s0->unk12 = 0x14;
    }
    temp_s0->unk1A = 0x320;
    self->unkB0 |= 0x2000;
    temp_s0->unk25 = (s8) setup->storedItemID;
    self->positionMirror2.f[1] = self->srt.transl.f[1];
    self->positionMirror2.f[0] = self->srt.transl.f[2];
    if (main_get_bits((s32) temp_s0->unk1E) != 0) {
        temp_s0->unk0 = 1;
        func_800267A4(self);
    }
    if (self->id == 0x3CF) {
        temp_s0->unk16 = 0x6B6;
        return;
    }
    temp_s0->unk16 = 0x371;
}



// offset: 0x1B8 | func: 1 | export: 1
void dll_295_control(Object *self);

static s8 dll_295_func_1104(Object* a0, Object* a1, void* a2){}
static s8 dll_295_func_2024(Object* a0, Object* a1, void* a2){}
static s8 dll_295_func_DC0(Object* a0, Object* a1, void* a2){}
static s8 dll_295_func_E78(Object* a0){}

void dll_295_control(Object* self) {
    Object* sp6C;
    ObjSetup* sp68;
    SmallBasket_Data* smallBasketData;
    s32 pad;
    SRT sp48; 
    ObjFSA_Data* sp44;
    f32 sp40;

    sp6C = get_player();
    sp68 = self->setup;
    sp40 = 1.0f;
    gDLL_7_Newday->vtbl->func5(&sp40);
    smallBasketData = self->data;

    if (gDLL_29_Gplay->vtbl->did_time_expire(sp68->uID) == 0){
        return;
    }    

    sp44 = sp6C->data;
    if (smallBasketData->unk1A <= 0) {
        smallBasketData->unk1A = 0x320;
        smallBasketData->unk10 = 1;
        smallBasketData->unk23 = 0;
        self->unkAF |= 8;
        dll_295_func_1104(self, sp6C, smallBasketData);
        self->speed.f[0] = 0.0f;
        self->speed.f[2] = 0.0f;
    }
    
    if (smallBasketData->unk0) {
        smallBasketData->unk0 -= (s16) (gUpdateRateF * sp40);
        if (smallBasketData->unk0 <= 0) {
            smallBasketData->unk0 = 0;
            smallBasketData->unk10 = 0;
            func_8002674C(self);
            self->unkAF &= 0xFFF7;
        }
    } else {
        if (smallBasketData->unk10) {
            smallBasketData->unk10 -= gUpdateRate;
            if (smallBasketData->unk10 <= 0) {
                if (smallBasketData->unk4) {
                    smallBasketData->unk0 = smallBasketData->unk4;
                } else {
                    smallBasketData->unk0 = 1;
                }
                gDLL_29_Gplay->vtbl->add_time(sp68->uID, smallBasketData->unk4);
                self->srt.transl.f[0] = sp68->x;
                self->srt.transl.f[1] = sp68->y;
                self->srt.transl.f[2] = sp68->z;
                self->positionMirror2.f[0] = sp68->x;
                self->positionMirror2.f[1] = sp68->y;
                self->positionMirror2.f[2] = sp68->z;
                self->speed.f[0] = 0.0f;
                self->speed.f[1] = 0.0f;
                self->speed.f[2] = 0.0f;
                func_800267A4(self);
            }

            //NOTE: changed the condition here just to allow the label be removed (while still having identical flow through the function)
            if (smallBasketData->unk10 < 0x33) {
                return;
            }
        }

        if (smallBasketData->unk23 != 1) {
            if (smallBasketData->unk21 == 0) {
                smallBasketData->unk21 = dll_295_func_DC0(self, sp6C, smallBasketData);
                if (smallBasketData->unk21) {
                    smallBasketData->unk22 = 1;
                }
                if (self->unkE0 == 0) {
                    func_8002674C(self);
                    self->unkAF &= 0xFFF7;
                }
                self->positionMirror2.f[1] = self->srt.transl.f[2];
                self->positionMirror2.f[2] = self->srt.transl.f[2];
                self->positionMirror2.f[0] = self->srt.transl.f[0];
            } else {
                func_800267A4(self);
                self->objhitInfo->unk10.f[0] = self->srt.transl.f[0];
                self->objhitInfo->unk10.f[1] = self->srt.transl.f[1];
                self->objhitInfo->unk10.f[2] = self->srt.transl.f[2];
                self->unkAF |= 8;
                
                if (joy_get_pressed(0) & A_BUTTON) {
                    if (((DLL_210_Player*)sp6C->dll)->vtbl->func49(sp6C)) {
                        smallBasketData->unk22 = 0;
                    } else {
                        gDLL_6_AMSFX->vtbl->play_sound(sp6C, 0x912, MAX_VOLUME, NULL, NULL, 0, NULL);
                    }
                }
                
                if (smallBasketData->unk22 != 0) {
                    smallBasketData->unk10 = 0;
                    smallBasketData->unk0 = 0;
                    obj_send_mesg(sp6C, 0x100010U, self, (void* ) ((smallBasketData->unkE << 0x10) | (smallBasketData->unkC & 0xFFFF)));
                }
                
                if (self->unkE0 == 1) {
                    smallBasketData->unk21 = 2;
                }
                
                if ((smallBasketData->unk21 == 2) && (self->unkE0 == 0) && (sp6C->curModAnimId != 0x447)) {
                    smallBasketData->unk21 = 0;
                    smallBasketData->unk23 = 1;
                    self->speed.f[1] = (sp44->unk290 * 0.75f) + 2.2f;
                    self->speed.f[2] = (sp44->unk290 * -0.75f) + -2.2f;
                    sp48.roll = 0;
                    sp48.pitch = 0;
                    sp48.transl.x = 0.0f;
                    sp48.transl.y = 0.0f;
                    sp48.transl.z = 0.0f;
                    sp48.scale = 1.0f;
                    sp48.yaw = sp6C->srt.yaw;
                    rotate_vec3((SRT* ) &sp48, self->speed.f);
                    gDLL_6_AMSFX->vtbl->play_sound(self, 0x637, 0x43, NULL, NULL, 0, NULL);
                    gDLL_6_AMSFX->vtbl->play_sound(self, 0x634, 0x61, NULL, NULL, 0, NULL);
                } else if ((smallBasketData->unk21 == 2) && (self->unkE0 == 0)) {
                    smallBasketData->unk21 = 0;
                    smallBasketData->unk23 = 2;
                    self->speed.f[0] = 0.0f;
                    self->speed.f[1] = 0.0f;
                    self->speed.f[2] = 0.0f;
                    func_8002674C(self);
                    self->unkAF &= 0xFFF7;
                    func_80026160(self);
                }
            }
        } else if (smallBasketData->unk23) {
            smallBasketData->unk1A -= gUpdateRate;
            if (smallBasketData->unk23 == 1) {
                func_80026128(self, 0xF, 1, 0);
                if (self->speed.f[1] > -10.0f) {
                    self->speed.f[1] += -0.12f * gUpdateRateF;
                }
                func_8002674C(self);
            }
            self->srt.transl.f[0] += self->speed.f[0] * gUpdateRateF;
            self->srt.transl.f[1] += self->speed.f[1] * gUpdateRateF;
            self->srt.transl.f[2] += self->speed.f[2] * gUpdateRateF;
            dll_295_func_E78(self);
            if ((self->objhitInfo->unk9D) && (smallBasketData->unk23 == 1)) {
                _data_0->vtbl->func[0].withSixArgs(self, 1, 0, 2, -1, 0);
                gDLL_6_AMSFX->vtbl->play_sound(self, smallBasketData->unk16, MAX_VOLUME, NULL, NULL, 0, NULL);
                smallBasketData->unk10 = 0x32;
                smallBasketData->unk23 = 0;
                self->unkAF |= 8;
                dll_295_func_1104(self, sp6C, smallBasketData);
                self->speed.f[0] = 0.0f;
                self->speed.f[2] = 0.0f;
                func_80026160(self);
            } else if ((self->objhitInfo->unk9D) && (smallBasketData->unk23 == 2)) {
                self->speed.f[0] = 0.0f;
                self->speed.f[2] = 0.0f;
                smallBasketData->unk10 = 0x1F4;
                smallBasketData->unk23 = 0;
                self->unkE0 = 0;
                func_8002674C(self);
                self->unkAF &= 0xFFF7;
                func_80026160(self);
            }
        }
        
        smallBasketData->shakeSoundTimer -= gUpdateRate;
        if (smallBasketData->unk21) {
            if (SQ(smallBasketData->unk12) <= vec3_distance_xz_squared(&self->positionMirror, (Vec3f* ) &sp68->x)) {
                self->speed.f[0] = 0.0f;
                self->speed.f[2] = 0.0f;
                smallBasketData->unk10 = 0x1F4;
                smallBasketData->unk23 = 0;
                self->unkE0 = 0;
                func_8002674C(self);
                self->unkAF &= 0xFFF7;
                func_80026160(self);
            }
        } else {
            dll_295_func_2024(self, sp6C, smallBasketData);
        }
        
        if ((smallBasketData->shakeSoundTimer <= 0) && smallBasketData->unk21) {
            if (smallBasketData->unk25 == 5 || 
                smallBasketData->unk25 == 6
            ){
                gDLL_6_AMSFX->vtbl->play_sound(NULL, 0x64D, 0x39, NULL, NULL, 0, NULL);
                smallBasketData->shakeSoundTimer = rand_next(0, 0x64) + 0x12C;
            } else if (smallBasketData->unk25 == 1 || 
                       smallBasketData->unk25 == 2 || 
                       smallBasketData->unk25 == 3 || 
                       smallBasketData->unk25 == 4
            ) {
                gDLL_6_AMSFX->vtbl->play_sound(NULL, 0x64C, 0x39, NULL, NULL, 0, NULL);
                smallBasketData->shakeSoundTimer = rand_next(0, 0x64) + 0x12C;
            }
            
            if (smallBasketData->unk25 == 0) {
                gDLL_6_AMSFX->vtbl->play_sound(NULL, rand_next(0, 2) + 0x6B7, 0x39, NULL, NULL, 0, NULL);
                smallBasketData->shakeSoundTimer = rand_next(0, 0x64) + 0x12C;
            }
        }
    }
}


// offset: 0xC0C | func: 2 | export: 2
void dll_295_update(Object *self) { }

// offset: 0xC18 | func: 3 | export: 3
void dll_295_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    SmallBasket_Data* sp2C = self->data;
    SmallBasket_Setup *setup = (SmallBasket_Setup*)self->setup;
    
    if (gDLL_29_Gplay->vtbl->did_time_expire(setup->base.uID) == 0) {
        return;
    }

    if (!(sp2C->unk10 == 0 || sp2C->unk10 >= 0x33) || sp2C->unk0 != 0) {
        return;
    }
    
    if (self->unkE0 != 0) {
        if (visibility != -1) {
            return;
        }
        goto draw;
    }

    if (visibility != 0) {
draw:
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}


// offset: 0xD04 | func: 4 | export: 4
void dll_295_free(Object* self, s32 arg1) {
    SmallBasket_Data* objdata = self->data;

    gDLL_14_Modgfx->vtbl->func5(self);
    dll_unload((void*)_data_0[0]);
    if (objdata->soundHandle) {
        gDLL_6_AMSFX->vtbl->func_A1C(objdata->soundHandle);
        objdata->soundHandle = 0;
    }
}

// offset: 0xD9C | func: 5 | export: 5
u32 dll_295_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0xDAC | func: 6 | export: 6
u32 dll_295_get_data_size(Object *self, u32 a1) {
    return sizeof(SmallBasket_Data);
}

// offset: 0xDC0 | func: 7
s32 dll_295_func_DC0(Object* arg0, Object* arg1, SmallBasket_Data* arg2) {
    s32 var_v1;

    gDLL_2_Camera->vtbl->func14();
    var_v1 = 0;
    if ((arg0->unkAF & 1) && (arg0->unkE0 == 0)) {
        joy_set_button_mask(0, 0x8000U);
        arg2->unkC = -0x8000;
        arg2->unkE = 0;
        func_800267A4(arg0);
        var_v1 = 1;
    }
    return var_v1;
}

// offset: 0xE78 | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/295_smallbasket/dll_295_func_E78.s")

// offset: 0x1104 | func: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/295_smallbasket/dll_295_func_1104.s")

// offset: 0x2024 | func: 10
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/295_smallbasket/dll_295_func_2024.s") 
// this func matches, needs the above func_1104 to be static to compile //
#else
void dll_295_func_2024(Object* arg0, Object* arg1, SmallBasket_Data* arg2) {

    if (func_80025F40(arg0, NULL, NULL, NULL) != 0) {
        if (arg2->soundHandle != 0) {
            gDLL_6_AMSFX->vtbl->func_A1C(arg2->soundHandle);
            arg2->soundHandle = 0U;
        } 
        _data_0[0]->vtbl->func [0].withSixArgs((s32)arg0, 1, 0, 2, -1, 0);
        gDLL_6_AMSFX->vtbl->play_sound(arg0, arg2->unk16, 0x7FU, NULL, NULL, 0, NULL);
        arg2->unk10 = 0x32;
        arg2->unk23 = 0;
        dll_295_func_1104(arg0, arg1, arg2);
        arg0->speed.f[0] = 0.0f;
        arg0->unkAF |= 8;
        arg0->speed.f[2] = 0.0f;
        func_80026160(arg0);
    }
}
#endif
