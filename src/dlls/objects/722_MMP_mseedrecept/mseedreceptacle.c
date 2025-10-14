#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/214_animobj.h"
#include "game/gamebits.h"
#include "sys/objtype.h"
#include "libnaudio/n_libaudio.h"

typedef struct {
/*00*/	s16 objId;
/*02*/	u8 quarterSize;
/*03*/	u8 act;
/*04*/	u8 nextCurveGroup;
/*05*/	u8 prevCurveGroup;
/*06*/	u8 branch1CurveGroup;
/*07*/	u8 branch2CurveGroup;
/*08*/	f32 x;
/*0c*/	f32 y;
/*10*/	f32 z;
/*14*/	s32 uID;
} CurveSetup; //curves use the base objSetup fields differently, so creating a unique base struct

typedef struct {
/*00*/ CurveSetup base;
/*18*/ s8 curveType; //2) KTrex, 3) RedEye, 1A) camera?, 1B) camera?, 1D) ThornTail, 1F) crawlSpace, 22) Kyte, 24) Tricky
/*1C*/ s32 unk1C;
/*20*/ s32 nextCurveUID;
/*24*/ s32 prevCurveUID;
/*28*/ s32 branch1UID;
/*2C*/ s32 branch2UID;
/*30*/ s8 yaw;
/*31*/ s8 pitch;
/*32*/ s16 usedBit; //gameBit
/*34*/ s16 unk34; //gameBit?
/*36*/ s16 unk36;
/*38*/ s16 unk38; //gameBit?
} KyteCurve_Setup;

typedef struct {
    u8 unk0;
    u8 unk1;
    KyteCurve_Setup* curveSetup;
    s16 gamebitPlanted; //after using MoonSeed
    s16 gamebitGrown; //after using Flame on planted seed (at night only, according to design docs)
    s16 unkC;
    s16 unkE;
    f32 unk10;
} MoonSeedReceptacle_Data;

typedef struct {
    ObjSetup base;
    s16 unk18;
    u16 kyteFlightGroup;
    s16 unk1C;
    s8 unk1E;
    u8 yaw;
} MoonSeedReceptacle_Setup;

/*0x0*/ static u8 _bss_0[0x10];

static void moonSeedReceptacle_func_D40(Object* self);
static s32 moonSeedReceptacle_anim_callback(Object* self, s32 animObj, AnimObj_Data* animObjData, s32 arg3);

// offset: 0x0 | ctor
void moonSeedReceptacle_ctor(void *dll) { }

// offset: 0xC | dtor
void moonSeedReceptacle_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void moonSeedReceptacle_setup(Object* self, MoonSeedReceptacle_Setup* setup, s32 arg2) {
    MoonSeedReceptacle_Data* objData;

    objData = self->data;
    self->unk0xbc = (void*)&moonSeedReceptacle_anim_callback;
    self->srt.yaw = setup->yaw << 8;
    objData->unk0 = 0;
    
    obj_add_object_type(self, 0x30);

    switch (setup->base.uID) {
        //Soil spot just beyond SharpClaw outpost 
        case 0x41A5B: 
            objData->gamebitPlanted = BIT_MMP_MoonSeed_1_Planted;
            objData->gamebitGrown = BIT_MMP_MoonSeed_1_Grown;
            break;
        //Soil spot near ravine, leading up to meteoroid impact site
        case 0x41A59: 
            objData->gamebitPlanted = BIT_MMP_MoonSeed_2_Planted;
            objData->gamebitGrown = BIT_MMP_MoonSeed_2_Grown;
            break;
        //Soil spot leading out of meteoroid impact site, towards Shrine
        case 0x41A5C: 
            objData->gamebitPlanted = BIT_MMP_MoonSeed_3_Planted;
            objData->gamebitGrown = BIT_MMP_MoonSeed_3_Grown;
            break;
        //Soil spot leading up to dark tunnel
        case 0x41A5D:
            objData->gamebitPlanted = BIT_MMP_MoonSeed_4_Planted;
            objData->gamebitGrown = BIT_MMP_MoonSeed_4_Grown;
            break;
    }

    objData->unk1 = 0;
}

// offset: 0x118 | func: 1 | export: 1
void moonSeedReceptacle_control(Object* self) {
    MoonSeedReceptacle_Data *objData;
    MoonSeedReceptacle_Setup *objSetup;
    Object *kyte;
    SRT particleTrans;
    f32 time;
    u8 bssIndex;
    s32 count;
        
    objData = self->data;
    objSetup = (MoonSeedReceptacle_Setup *) self->setup;
    
    if (objData->unk1 & 1){
        objData->unk0 = 2;
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_798_Puzzle_Solved, MAX_VOLUME, NULL, NULL, 0, NULL);
        main_set_bits(objData->gamebitPlanted, 1);
        objData->unk1 &= 0xFFFE;
        self->unk_0x36 = 0xFF;
    }
    
    if (gDLL_7_Newday->vtbl->func8(&time) || objData->unk0 == 3){
        objData->unk1 |= 2;
    } else {
        objData->unk1 &= 0xFFFD;
    }
    
    bssIndex = 0;
    switch (objData->unk0){
        case 0:
            objData->unk0 = 1;
            self->srt.transl.y = objSetup->base.y - 10.0f;
            if (main_get_bits(objData->gamebitPlanted)){
                objData->unk0 = 2;
                self->srt.transl.y = objSetup->base.y;
                self->unk_0x36 = 0xFF;
            }
            if (main_get_bits(objData->gamebitGrown)){
                moonSeedReceptacle_func_D40(self);
            }
            return;
        
        case 1:
            if ((self->unk0xaf & 1) && gDLL_1_UI->vtbl->func7(0x86A)){
                count = main_get_bits(BIT_Inventory_MoonSeeds);
                if (count){
                    self->srt.transl.y = objSetup->base.y;
                    self->unk_0x36 = 0;
                    gDLL_3_Animation->vtbl->func17(0, self, -1);
                    main_set_bits(BIT_Inventory_MoonSeeds, count - 1);
                }
            }
            break;
        
        case 2:
            kyte = get_sidekick();
            if (objData->unk1 & 2){
                if (objData->unk1 & 4){
                    self->srt.transl.y = rand_next(-1, 1) + objSetup->base.y;
                    gDLL_17->vtbl->func1(self, 0x70F, NULL, 2, -1, NULL);
                }
                
                objData->unk10 -= delayFloat;
                if (objData->unk10 <= 0.0f){
                    if (rand_next(0, 1)){
                        objData->unk10 = 45.0f;
                        gDLL_6_AMSFX->vtbl->play_sound(self, rand_next(SOUND_A71_Rattling_1, SOUND_A72_Rattling_2), MAX_VOLUME, NULL, NULL, bssIndex, NULL);
                        objData->unk1 |= 4;
                    } else {
                        objData->unk10 = rand_next(50, 200);
                        objData->unk1 &= 0xFFFB;
                    }
                }
                
                particleTrans.transl.x = rand_next(-7, 7) + objSetup->base.x;
                particleTrans.transl.y = rand_next(0, 10) + objSetup->base.y;
                particleTrans.transl.z = rand_next(-7, 7) + objSetup->base.z;
                
                _bss_0[bssIndex] = (fsin16_precise(objData->unkC) + 1.0f) * 24.0f;
                gDLL_17->vtbl->func1(self, 0x70D, &particleTrans, 0x200001, -1, NULL);
                gDLL_17->vtbl->func1(self, 0x70E, NULL, 2, -1, &_bss_0[bssIndex]);
                
            }
            if (kyte && (self->unk0xaf & 4)){
                ((DLL_Unknown *) kyte->dll)->vtbl->func[14].withTwoArgs((s32)kyte, 4);
                if (gDLL_1_UI->vtbl->func7(4)){
                    main_set_bits(BIT_Kyte_Flight_Curve, objSetup->kyteFlightGroup);
                }
            }
            break;
        
        case 3:
            kyte = get_sidekick();
            self->srt.transl.y = objSetup->base.y;
            if (vec3_distance_xz_squared(&kyte->positionMirror, &self->positionMirror) <= 2500.0f){
                
                for (count = -1; count < (objData->unkC >> 0xD); count++){
                    particleTrans.transl.x = rand_next(-7, 7) + objSetup->base.x;
                    particleTrans.transl.y = rand_next(0, 10) + objSetup->base.y;
                    particleTrans.transl.z = rand_next(-7, 7) + objSetup->base.z;
                    gDLL_17->vtbl->func1(self, 0x70D, &particleTrans, 0x200001, -1, NULL);
                }
                
                _bss_0[bssIndex] = objData->unkC >> 7;
                gDLL_17->vtbl->func1(self, 0x70E, NULL, 2, -1, &_bss_0[bssIndex]);
            } else {
                particleTrans.transl.x = rand_next(-7, 7) + objSetup->base.x;
                particleTrans.transl.y = rand_next(0, 10) + objSetup->base.y;
                particleTrans.transl.z = rand_next(-7, 7) + objSetup->base.z;
                _bss_0[bssIndex] = 0x50;
                gDLL_17->vtbl->func1(self, 0x70D, &particleTrans, 0x200001, -1, NULL);
                gDLL_17->vtbl->func1(self, 0x70E, NULL, 2, -1, &_bss_0[bssIndex]);
            }
            break;
    
    }

}

// offset: 0x998 | func: 2 | export: 2
void moonSeedReceptacle_update(Object *self) { }

// offset: 0x9A4 | func: 3 | export: 3
void dll_722_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/722_MMP_mseedrecept/dll_722_print.s")

// offset: 0xB88 | func: 4 | export: 4
void moonSeedReceptacle_free(Object* self, s32 a1) {
    obj_free_object_type(self, 0x30);
}

// offset: 0xBC8 | func: 5 | export: 5
u32 moonSeedReceptacle_get_model_flags(Object *self){
    return 1;
}

// offset: 0xBD8 | func: 6 | export: 6
u32 moonSeedReceptacle_get_data_size(Object *self, u32 a1) {
    return sizeof(MoonSeedReceptacle_Data);
}

// offset: 0xBEC | func: 7 | export: 7
s32 moonSeedReceptacle_func_BEC(Object* self, s32 arg1) {
    MoonSeedReceptacle_Data* objData;
    s32 returnVal;

    objData = self->data;
    returnVal = FALSE;
    if (arg1 == 0) {
        if (objData->unk1 & 2) {
            objData->unk0 = 3;
            objData->unkC = 0;
        }
        returnVal = TRUE;
    } else if (arg1 == 1) {
        if (objData->unk0 == 3) {
            returnVal = TRUE;
            if (main_get_bits(objData->gamebitPlanted) && 
                !main_get_bits(objData->gamebitGrown)) {
                moonSeedReceptacle_func_D40(self);
            }
        } else {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_912_Object_Refused, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
    }
    return returnVal;
}

// offset: 0xD00 | func: 8 | export: 8
u32 moonSeedReceptacle_func_D00(Object *self, u32 a1, u32 a2) {
    return 0;
}

// offset: 0xD18 | func: 9 | export: 9
u32 moonSeedReceptacle_func_D18(Object *self, u32 a1, u32 a2) {
    return 0;
}

// offset: 0xD30 | func: 10 | export: 10
u32 moonSeedReceptacle_func_D30(Object *self){
    return 2;
}

// offset: 0xD40 | func: 11
void moonSeedReceptacle_func_D40(Object* self) {
    MoonSeedReceptacle_Data* objData;
    MoonSeedReceptacle_Setup* objSetup;
    KyteCurve_Setup* curveSetup;

    objData = self->data;
    objSetup = (MoonSeedReceptacle_Setup*)self->setup;
    
    if (main_get_bits(objData->gamebitPlanted)) {
        self->unk0xaf |= 8;
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_798_Puzzle_Solved, MAX_VOLUME, NULL, NULL, 0, NULL);
        main_set_bits(objData->gamebitGrown, 1);
        objData->unk0 = 4;
        self->srt.transl.y = objSetup->base.y;
        curveSetup = (KyteCurve_Setup*)gDLL_25->vtbl->dll_25_func_2BC4(self, objSetup->kyteFlightGroup);
        objData->curveSetup = curveSetup;
        if (curveSetup->usedBit != -1) {
            main_set_bits(curveSetup->usedBit, 1);
        }
    }
}

// offset: 0xE5C | func: 12
s32 moonSeedReceptacle_anim_callback(Object* self, s32 animObj, AnimObj_Data* animObjData, s32 arg3) {
    MoonSeedReceptacle_Data* objData;

    objData = self->data;
    objData->unk1 |= 1;
    return 0;
}

/*0x0*/ static const char str_0[] = "Error romdef ident no known!\n";
