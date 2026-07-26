#include "common.h"
#include "dlls/engine/18_objfsa.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/210_player.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "sys/gfx/modgfx.h"
#include "sys/math.h"
#include "sys/menu.h"
#include "sys/objhits.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"

typedef struct {
    s8 unk0;
    u8 unk1;
    u8 unk2;
    f32 unk4;
    s16 unk8;
    u8 unkA;
    u16 unkC;
    s16 unkE;
    u16 unk10;
    f32 freezeTimer;
} SharpClaw_DataActual;

typedef struct {
    Baddie baddie;
    SharpClaw_DataActual objData;
} SharpClaw_Data;

typedef enum {
    SharpClaw_ASTATE_0,
    SharpClaw_ASTATE_1,
    SharpClaw_ASTATE_2,
    SharpClaw_ASTATE_3,
    SharpClaw_ASTATE_4,
    SharpClaw_ASTATE_5,
    SharpClaw_ASTATE_6,
    SharpClaw_ASTATE_7,
    SharpClaw_ASTATE_8,
    SharpClaw_ASTATE_9,
    SharpClaw_ASTATE_10,
    SharpClaw_ASTATE_11,
    SharpClaw_ASTATE_12,
    SharpClaw_ASTATE_13,
    SharpClaw_ASTATE_14,
    SharpClaw_ASTATE_15,
    SharpClaw_ASTATE_16,
    SharpClaw_ASTATE_17,
    SharpClaw_ASTATE_18,
    SharpClaw_ASTATE_19,
    SharpClaw_ASTATE_20,
    SharpClaw_ASTATE_21,
    SharpClaw_ASTATE_22,
    SharpClaw_ASTATE_23,
    SharpClaw_ASTATE_24
} SharpClaw_AnimStates;

typedef enum {
    SharpClaw_LSTATE_0,
    SharpClaw_LSTATE_1,
    SharpClaw_LSTATE_2,
    SharpClaw_LSTATE_3,
    SharpClaw_LSTATE_4,
    SharpClaw_LSTATE_5,
    SharpClaw_LSTATE_6,
    SharpClaw_LSTATE_7,
    SharpClaw_LSTATE_8,
    SharpClaw_LSTATE_9,
    SharpClaw_LSTATE_10,
    SharpClaw_LSTATE_11,
    SharpClaw_LSTATE_12,
    SharpClaw_LSTATE_13
} SharpClaw_LogicStates;

/*0x0*/ static s32 data_0[] = {
    0x00000013, 0x00000013, 0x00000015, 0x00000015, 0x00000015, 0x00000013, 0x00000013, 0x00000013, 
    0x00000013, 0x00000013, 0x00000013, 0x00000015, 0x00000015, 0x00000015, 0x00000015, 0x00000013, 
    0x00000015, 0x00000013, 0x00000013, 0x00000013, 0x00000013, 0x00000015, 0x00000013, 0x00000014, 
    0x00000013, 0x00000013, 0x00000013, 0x00000015
};
/*0x70*/ static u32 data_70[] = {
    0x0000010d, 0x0000010d, 0x0000010c, 0x0000010a, 0x0000010a, 0x0000010b, 0x0000010b, 0x0000010b, 
    0x0000010a
};
/*0x94*/ static s8 data_94[] = {
    0xff, 0xff, 0xff, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x01, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0xff, 0xff, 0xff
};
/*0xB0*/ static s16 dFootstepSounds[] = {
    SOUND_61_Footstep_Soft,         SOUND_62_Footstep_Soft, 
    SOUND_69_Footstep_Wood_Creaky,  SOUND_6A_Footstep_Wood_Creaky, 
    SOUND_65_Footstep_Snow,         SOUND_66_Footstep_Snow, 
    SOUND_6D_Footstep_Water_Wade,   SOUND_6E_Footstep_Water_Wade, 
    SOUND_280_Footstep_Stone,       SOUND_281_Footstep_Stone
};
/*0xC4*/ static s32 dTauntSounds[] = {
    SOUND_169_SharpClaw_Taunt_1, 
    SOUND_16A_SharpClaw_Taunt_2, 
    SOUND_16B_SharpClaw_Taunt_3
};
/*0xD0*/ static u32 data_D0[] = {
    0x00000236, 
    0x00000237, 
    0x00000238, 
    0x00000239, 
    0x0000023a
};
/*0xE4*/ static u32 data_E4[] = {
    0x0000023b, 
    0x0000023f, 
    0x0000023d, 
    0x0000023e, 
    0x00000240, 
    0x0000023c
};
/*0xFC*/ static u32 data_FC[] = {
    0x000000b6, 0x00000099, 0x00000090, 0x00000091
};
/*0x10C*/ static u32 data_10C[] = {
    0x00000374, 
    0x00000375, 
    0x00000376, 
    0x0000025b, 
    0x0000025c
};
/*0x120*/ static u32 data_120[] = {
    0x00000377, 
    0x00000378, 
    0x00000379, 
    0x0000025b, 
    0x0000025c
};
/*0x134*/ static s16 data_134[] = {
    0x0000, 0x0001, 0x0002, 0x0000
};
/*0x13C*/ static s16 data_13C[] = {
    0x0000, 0x0004, 0x0002, 0x0000
};
/*0x144*/ static s16 data_144[] = { //modAnims
    0x0015, 0x0017, 0x0018, 0x0013
};
/*0x14C*/ static f32 data_14C[] = {
    0, 0.05, 0.03, 0.7, 0.68, 1.05
};
/*0x164*/ static s32 data_164[] = {
    0x00000002, 0x00000003
};

//NOTE: interestingly nearly all the objIDs here are deleted objects!
/*0x16C*/ static u32 data_16C[] = {
    OBJ_ClubSharpClaw, 271, 277, OBJ_SnowClaw, 366, 415, 567, 580
};
/*0x18C*/ static u8 dTerrainTypeSoundMap[] = {
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00, 0x01, 0x04, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00
};
/*0x1B0*/ static u32 data_1B0[] = {
    0x00000006, 0x00000069, 0x00000069, 0x000000ff
};
/*0x1C0*/ static u16 data_1C0[] = {
    0x0206, 0x0167, 0x0165, 0x0206
};
/*0x1C8*/ static u16 data_1C8[] = {
    0x0206, 0x0167, 0x0165, 0x0206
};

/*0x0*/ static SRT bss_0;
/*0x1C*/ static s32 bss_1C;
/*0x20*/ static ObjFSA_StateCallback sAnimStateCallbacks[26];
/*0x88*/ static ObjFSA_StateCallback sLogicStateCallbacks[14];

static s32 SharpClaw_animState0(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState1(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState2(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState3(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState5(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState6(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState7(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState8(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState9(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState10(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState11(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState12(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState13(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState14(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState15(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState16(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState17(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState18(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState19(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState20(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState21(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState22(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState23(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_animState24(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState0(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState1(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState2(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState3(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState4(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState5(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState6(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState7(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState8(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState9(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState10(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState11(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState12(Object* self, ObjFSA_Data* fsa, f32 updateRate);
static s32 SharpClaw_logicState13(Object* self, ObjFSA_Data* fsa, f32 updateRate);

// offset: 0x0 | func: 0
static void SharpClaw_func_0(void) {
    sAnimStateCallbacks[SharpClaw_ASTATE_0] = SharpClaw_animState0;
    sAnimStateCallbacks[SharpClaw_ASTATE_1] = SharpClaw_animState1;
    sAnimStateCallbacks[SharpClaw_ASTATE_2] = SharpClaw_animState2;
    sAnimStateCallbacks[SharpClaw_ASTATE_3] = SharpClaw_animState3;
    sAnimStateCallbacks[SharpClaw_ASTATE_4] = SharpClaw_animState4;
    sAnimStateCallbacks[SharpClaw_ASTATE_5] = SharpClaw_animState5;
    sAnimStateCallbacks[SharpClaw_ASTATE_6] = SharpClaw_animState6;
    sAnimStateCallbacks[SharpClaw_ASTATE_7] = SharpClaw_animState7;
    sAnimStateCallbacks[SharpClaw_ASTATE_8] = SharpClaw_animState8;
    sAnimStateCallbacks[SharpClaw_ASTATE_9] = SharpClaw_animState9;
    sAnimStateCallbacks[SharpClaw_ASTATE_10] = SharpClaw_animState10;
    sAnimStateCallbacks[SharpClaw_ASTATE_11] = SharpClaw_animState11;
    sAnimStateCallbacks[SharpClaw_ASTATE_12] = SharpClaw_animState12;
    sAnimStateCallbacks[SharpClaw_ASTATE_13] = SharpClaw_animState13;
    sAnimStateCallbacks[SharpClaw_ASTATE_14] = SharpClaw_animState14;
    sAnimStateCallbacks[SharpClaw_ASTATE_15] = SharpClaw_animState15;
    sAnimStateCallbacks[SharpClaw_ASTATE_16] = SharpClaw_animState16;
    sAnimStateCallbacks[SharpClaw_ASTATE_17] = SharpClaw_animState17;
    sAnimStateCallbacks[SharpClaw_ASTATE_18] = SharpClaw_animState18;
    sAnimStateCallbacks[SharpClaw_ASTATE_19] = SharpClaw_animState19;
    sAnimStateCallbacks[SharpClaw_ASTATE_20] = SharpClaw_animState20;
    sAnimStateCallbacks[SharpClaw_ASTATE_21] = SharpClaw_animState21;
    sAnimStateCallbacks[SharpClaw_ASTATE_22] = SharpClaw_animState22;
    sAnimStateCallbacks[SharpClaw_ASTATE_23] = SharpClaw_animState23;
    sAnimStateCallbacks[SharpClaw_ASTATE_24] = SharpClaw_animState24;
    
    sLogicStateCallbacks[SharpClaw_LSTATE_0] = SharpClaw_logicState0;
    sLogicStateCallbacks[SharpClaw_LSTATE_1] = SharpClaw_logicState1;
    sLogicStateCallbacks[SharpClaw_LSTATE_2] = SharpClaw_logicState2;
    sLogicStateCallbacks[SharpClaw_LSTATE_3] = SharpClaw_logicState3;
    sLogicStateCallbacks[SharpClaw_LSTATE_4] = SharpClaw_logicState4;
    sLogicStateCallbacks[SharpClaw_LSTATE_5] = SharpClaw_logicState5;
    sLogicStateCallbacks[SharpClaw_LSTATE_6] = SharpClaw_logicState6;
    sLogicStateCallbacks[SharpClaw_LSTATE_7] = SharpClaw_logicState7;
    sLogicStateCallbacks[SharpClaw_LSTATE_8] = SharpClaw_logicState8;
    sLogicStateCallbacks[SharpClaw_LSTATE_9] = SharpClaw_logicState9;
    sLogicStateCallbacks[SharpClaw_LSTATE_10] = SharpClaw_logicState10;
    sLogicStateCallbacks[SharpClaw_LSTATE_11] = SharpClaw_logicState11;
    sLogicStateCallbacks[SharpClaw_LSTATE_12] = SharpClaw_logicState12;
    sLogicStateCallbacks[SharpClaw_LSTATE_13] = SharpClaw_logicState13;
}

// offset: 0x1F8 | ctor
void SharpClaw_ctor(void* dll) {
    SharpClaw_func_0();
}

// offset: 0x238 | dtor
void SharpClaw_dtor(void* dll) { }

// offset: 0x244 | func: 1 | export: 0
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_obj_Setup.s")
#else

static int SharpClaw_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

//Matches, but needs SharpClaw_func_2044 to be static
void SharpClaw_obj_Setup(Object* self, Baddie_Setup* objSetup, s32 reset) {
    Baddie* baddie;
    u8 flags;
    SharpClaw_DataActual* objData;

    baddie = self->data;
    
    flags = 0x10 | 4 | 2;
    if (reset) {
        flags = 0x10 | 4 | 2 | 1;
    }
    if ((objSetup->unk2B & 0x20) == FALSE) {
        flags |= 8;
    }
    gDLL_33_BaddieControl->vtbl->setup(self, objSetup, baddie, 0x19, 0xE, 0x10E, flags, 20.0f);
    
    self->animCallback = SharpClaw_animCallback;
    
    objData = baddie->objdata;
    baddie->fsa.animState = SharpClaw_ASTATE_0;
    baddie->fsa.logicState = SharpClaw_LSTATE_0;
    baddie->unk3B6 = 150;
    objData->unk0 = 0;
    objData->unk1 = objSetup->unk2F;
    objAddObjectType(self, OBJTYPE_63);
}
#endif

// offset: 0x32C | func: 2 | export: 1
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_obj_Control.s")
#else

static void SharpClaw_func_14C0(Object* self, AnimObj_Data* animData, Baddie* baddie, ObjFSA_Data* fsa);
static void SharpClaw_func_18EC(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void SharpClaw_func_E88(Object* self, Baddie* baddie, ObjFSA_Data* fsa);

//Matches, but needs SharpClaw_func_2044 to be static
void SharpClaw_obj_Control(Object* self) {
    Baddie* baddie;
    Baddie_Setup* objSetup;

    baddie = self->data;
    objSetup = (Baddie_Setup*)self->setup;
    
    if (menuGetCurrent() == MENU_TITLE_SCREEN) {
        return;
    }
    
    if (self->unkDC != 0) {
        if (gDLL_29_Gplay->vtbl->did_time_expire(objSetup->base.uID) != 0) {
            gDLL_33_BaddieControl->vtbl->setup(self, objSetup, baddie, 0x19, 0xE, 0x10E, 0x36U, 20.0f);
            baddie->fsa.logicState = 1;
            baddie->fsa.enteredLogicState = TRUE;
            self->opacity = 0;
            baddie->unk3B6 = 0x96;
        }
    } else {        
        if (!(baddie->unk3B0 & 4) && (self->unkE0 == 0)) {
            if (objSetup && !(baddie->unk3B0 & 8)) {
                self->srt.transl.x = objSetup->base.x;
                self->srt.transl.y = objSetup->base.y;
                self->srt.transl.z = objSetup->base.z;
            }
            
            gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->unk2E, self, -1);
            self->unkE0 = 1;
            return;
        }
        
        if (baddie->unk3B2 & 2) {
            if (!(baddie->unk3B0 & 4)) {
                gDLL_33_BaddieControl->vtbl->func9(self, &baddie->fsa, &baddie->unk34C, baddie->unk39E, (s8*)&baddie->unk3B4, 0, 0, 0, 1);
                if (baddie->unk3B2 & 4) {
                    baddie->fsa.logicState = 0xD;
                }
            }
            
            baddie->unk3B0 &= ~4;
            baddie->unk3B2 &= ~2;
            
            if (baddie->fsa.hitpoints > 0) {
                func_8002674C(self);
            }
        }
        
        if (gDLL_33_BaddieControl->vtbl->func11(self, baddie, 1)) {
            SharpClaw_func_E88(self, baddie, &baddie->fsa);
            if (baddie->unk3B4 == 2) {
                SharpClaw_func_14C0(self, 0, baddie, &baddie->fsa);
            } else {
                SharpClaw_func_18EC(self, baddie, &baddie->fsa);
            }
        }
    }
}
#endif

// offset: 0x5E0 | func: 3 | export: 2
void SharpClaw_obj_Update(Object* self) {
    Baddie* baddie;
    ObjFSA_Data* fsa;
    ObjectHitInfo* objhitInfo;

    baddie = self->data;
    fsa = &((Baddie*)self->data)->fsa;
    
    gDLL_18_objfsa->vtbl->func2(self, fsa, sAnimStateCallbacks);
    if (fsa->unk341 == 1) {
        if (self->linkedObject != NULL) {
            objhitInfo = self->linkedObject->objhitInfo;
        } else {
            objhitInfo = self->objhitInfo;
        }
        
        if ((self->objhitInfo->unk48 != NULL) || (objhitInfo->unk48 != NULL)) {
            self->objhitInfo->unk61 = 1;
        }
    }
}

// offset: 0x68C | func: 4 | export: 3
void SharpClaw_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Baddie* baddie;
    SharpClaw_DataActual* objData;

    baddie = self->data;
    objData = baddie->objdata;

    if (!visibility || self->unkDC) {
        return;
    }
    
    //Tint blue/red while frozen or hurt
    if (objData->freezeTimer > 0.0f) {
        objprintSetBlendColor(0x64, 0xFF, 0xFF, 0x9B);
    } else {
        if (baddie->unk3E8 != 0.0f) {
            objprintSetBlendColor(0xC8, 0, 0, baddie->unk3E8);
        }
    }
    
    objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    
    //Create particles while frozen
    if (objData->freezeTimer > 0.0f) {
        gDLL_32->vtbl->func2(self, PARTICLE_52A, NULL);
    }
    
    if (baddie->unk3B2 & (0x40 | 0x20)) {
        if (baddie->unk3B2 & 0x20) {
            gDLL_32->vtbl->func2(self, PARTICLE_330, &baddie->unk3E8);
            gDLL_32->vtbl->func2(self, PARTICLE_330, &baddie->unk3E8);
        }
        gDLL_32->vtbl->func2(self, PARTICLE_32F, &baddie->unk3E8);
    }
    
    if (baddie->unk3B2 & 0x100) {
        gDLL_32->vtbl->func2(self, PARTICLE_333, &baddie->unk3E8);
        gDLL_32->vtbl->func2(self, PARTICLE_334, &baddie->unk3E8);
        baddie->unk3B2 &= ~0x100;
    }
}

// offset: 0x958 | func: 5 | export: 4
void SharpClaw_obj_Free(Object* self, s32 onlySelf) {
    Baddie* baddie = self->data;
    
    objFreeObjectType(self, OBJTYPE_Baddie);
    objFreeObjectType(self, OBJTYPE_63);

    if (self->linkedObject != NULL) {
        objFreeObject(self->linkedObject);
        self->linkedObject = NULL;
    }
    
    gDLL_33_BaddieControl->vtbl->free(self, baddie, 0x20);
}

// offset: 0xA0C | func: 6 | export: 5
u32 SharpClaw_obj_GetModelFlags(Object* self) {
    return MODFLAGS_100 | MODFLAGS_EVENTS | MODFLAGS_8 | MODFLAGS_SHADOW | MODFLAGS_1;
}

// offset: 0xA1C | func: 7 | export: 6
u32 SharpClaw_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(SharpClaw_Data);
}

// offset: 0xA30 | func: 8 | export: 7
s16 SharpClaw_Func_A30(Object* self) {
    Baddie* baddie = self->data;
    return baddie->fsa.logicState;
}

// offset: 0xA40 | func: 9 | export: 8
void SharpClaw_Func_A40(Object* self, u8 arg1) {
    Baddie* baddie;
    SharpClaw_DataActual* objData;
 
    baddie = self->data;
    objData = baddie->objdata;
    
    switch (arg1) {
    case 1:
        objData->unkA = 1;
        break;
    case 2:
        objData->unkA = 2;
        break;
    }
}

// offset: 0xA88 | func: 10
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_animCallback.s")
#else

static void SharpClaw_func_14C0(Object* self, AnimObj_Data* animData, Baddie* baddie, ObjFSA_Data* fsa);
static void SharpClaw_func_17A0(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void SharpClaw_func_E88(Object* self, Baddie* baddie, ObjFSA_Data* fsa);

//Matches, but needs SharpClaw_func_2044 to be static
int SharpClaw_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    Baddie* baddie;
    Baddie_Setup* objSetup;
    Object* sidekick;
    s32 i;

    baddie = self->data;
    objSetup = (Baddie_Setup*)self->setup;
    
    if (self->unkDC != 0) {
        return 0;
    }

    for (i = 0; i < animData->messageCount; i++) {
        switch (animData->messages[i]) {
        case 1:
            sidekick = objGetSidekick();
            if (sidekick != NULL) {
                ((DLL_ISidekick*)sidekick->dll)->vtbl->func21(sidekick, 1, self);
                baddie->unk3B2 |= 4;
                baddie->unk3B4 = 2;
                baddie->fsa.logicState = 0xD;
            }
            break;
        case 2:
            baddie->nextWeaponID = 3;
            SharpClaw_func_E88(self, baddie, &baddie->fsa);
            break;
        case 3:
            gDLL_3_Animation->vtbl->set_camera_module(0x5B, 4, (s32)self, 0x3C);
            break;
        }
    }
    
    if (self->seqSlot != -1) {
        if (gDLL_33_BaddieControl->vtbl->func11(self, baddie, 1U) == 0) {
            func_800267A4(self);
            return 1;
        }
        if (baddie->fsa.hitpoints > 0) {
            func_8002674C(self);
        }
        SharpClaw_func_E88(self, baddie, &baddie->fsa);
        if ((baddie->unk3A0 != NO_GAMEBIT) && mainGetBits(baddie->unk3A0)) {
            gDLL_3_Animation->vtbl->func21(animData, objSetup->unk2C);
            baddie->unk3A0 = -1;
        }

        switch (baddie->unk3B4) {
        case 2:
            animData->unk7A = 0;
            SharpClaw_func_14C0(self, animData, baddie, &baddie->fsa);
            if (baddie->unk3B4 == 1) {
                baddie->fsa.logicState = 0xB;
                gDLL_18_objfsa->vtbl->tick(self, &baddie->fsa, 1.0f, 1.0f, sAnimStateCallbacks, sLogicStateCallbacks);
                animData->unk62 = 0;
            }
            break;
        case 1:
            if (gDLL_33_BaddieControl->vtbl->func12(self, animData, baddie, sAnimStateCallbacks, sLogicStateCallbacks, 0)) {
                gDLL_33_BaddieControl->vtbl->func10(self, &baddie->fsa, 0.17f, 1);
            }
            SharpClaw_func_17A0(self, baddie, &baddie->fsa);
            break;
        case 0:
        default:
            animData->unk7A = -1;
            animData->unk7A &= 0xFFBF;
            SharpClaw_func_17A0(self, baddie, &baddie->fsa);
            break;
        }
    }

    if (self->seqSlot == -1) {
        baddie->unk3B2 |= 2;
        func_800267A4(self);
        return 0;
    }
    
    return baddie->unk3B4 != 0;
}
#endif

// offset: 0xE88 | func: 11
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_func_E88.s")
#else

static void SharpClaw_func_1E2C(Object* self, Baddie* baddie, ObjFSA_Data* fsa);
static void SharpClaw_func_2044(Object* self, SRT* fxTransform, s32 useModGfx);
static s32 SharpClaw_func_267C(u8 message, Object* baddie);

//Matches, but needs SharpClaw_func_2044 to be static
void SharpClaw_func_E88(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    SharpClaw_DataActual* objData;
    s32 hit;
    s32 temp_v0_2;

    objData = baddie->objdata;

    if (self->linkedObject != NULL) {
        self->linkedObject->parent = self->parent;
    }
    
    gDLL_33_BaddieControl->vtbl->func4(self, objGetPlayer(), 0x10, &objData->unkC, &objData->unkE, &objData->unk10);
    fsa->targetDist = objData->unk10;
    if (!(baddie->unk3B0 & 0x20)) {
        gDLL_33_BaddieControl->vtbl->func14(self, (Baddie*)fsa, &baddie->unk3B2, 9, 0xA, baddie->unk3A6, baddie->unk3A4);
    }
    gDLL_33_BaddieControl->vtbl->change_weapon(self, baddie);
    
    SharpClaw_func_1E2C(self, baddie, fsa);
    objExprEyeIdle(self, &baddie->unk3BC);

    temp_v0_2 = gDLL_33_BaddieControl->vtbl->func20(self, fsa, &baddie->unk34C, baddie->unk39E, &baddie->unk3B4, 0, 0, 0);
    if (temp_v0_2 == 1) {
        baddie->unk3B2 |= 4;
    } else if (temp_v0_2 == 2) {
        baddie->unk3B2 &= ~4;
    }
    
    if ((fsa->animState == SharpClaw_ASTATE_16) || 
        (fsa->animState == SharpClaw_ASTATE_17) || 
        (fsa->animState == SharpClaw_ASTATE_18)
    ) {
        func_80028D2C(self);
    } else {
        func_80026160(self);
    }
    
    objData->unk4 += gUpdateRateF;
    
    if (baddie->unk3B0 & 0x80) {
        if ((fsa->animState == SharpClaw_ASTATE_16) || 
            (fsa->animState == SharpClaw_ASTATE_17) || 
            (fsa->animState == SharpClaw_ASTATE_18) || 
            (baddie->unk3B2 & 0x10)
        ) {
            hit = gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, &baddie->unk34C, baddie->unk39E, data_0, data_94, 7, &baddie->unk3A8, &bss_0);
            if (hit) {
                SharpClaw_func_2044(self, &bss_0, 0);
            }
        } else {
            hit = gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, NULL, 7, &baddie->unk3A8, &bss_0);
            if (hit) {
                SharpClaw_func_2044(self, &bss_0, 1);
            }
        }
    } else {
        if (((fsa->animState == SharpClaw_ASTATE_15) || (fsa->animState == SharpClaw_ASTATE_5)) 
            && !(baddie->unk3B2 & 0x10)
        ) {
            hit = gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, &baddie->unk34C, baddie->unk39E, NULL, NULL, 7, &baddie->unk3A8, &bss_0);
            if (hit) {
                SharpClaw_func_2044(self, &bss_0, 1);
            }
        } else {
            hit = gDLL_33_BaddieControl->vtbl->check_hit(self, fsa, &baddie->unk34C, baddie->unk39E, data_0, data_94, 7, &baddie->unk3A8, &bss_0);
            if (hit) {
                SharpClaw_func_2044(self, &bss_0, 0);
            }
        }
    }
    
    if ((hit == Damage_Type_Explosion) || 
        (hit == Damage_Type_E) || 
        (hit == Damage_Type_Projectile)
    ) {
        baddie->unk3B4 = 2;
        fsa->target = objGetPlayer();
        return;
    }
    
    //Become frozen by the Ice Blast Spell
    if (hit == Damage_Type_Ice_Blast) {
        baddie->unk3B4 = 2;
        objData->freezeTimer = 400.0f;
        baddie->unk3B2 |= 0x80;
        return;
    }
    
    if (hit) {
        baddie->unk3B4 = 2;
        if (objData->unk4 < 240.0f) {
            objData->unk8++;
        } else {
            objData->unk8 = 0;
        }
        
        if (fsa->hitpoints > 0) {
            objData->unk4 = 0.0f;
            if (objData->unk8 >= 2) {
                objData->unk8 = 0;
                fsa->logicState = 8;
                fsa->enteredLogicState = TRUE;
                if (SharpClaw_func_267C(2, self)) {
                    objData->unkA = 1;
                }
            } else if (SharpClaw_func_267C(2, self)) {
                objData->unkA = 1;
            }
        }
    }
}

#endif

// offset: 0x14C0 | func: 12
void SharpClaw_func_14C0(Object* self, AnimObj_Data* animData, Baddie* baddie, ObjFSA_Data* fsa) {
    SharpClaw_DataActual* objData;
    Baddie_Setup* objSetup;
    UnkCurvesStruct* temp_a0;
    Object* sidekick;
    Object* target;

    objData = baddie->objdata;
    objSetup = (Baddie_Setup*)self->setup;
    target = fsa->target;
    if (target != NULL) {
        objExpr_func_80032C0C(self, target, &baddie->unk3BC, 0x19);
    }
    
    if (gDLL_33_BaddieControl->vtbl->func16(self, fsa, baddie->unk3E2 * 1.39f, !(baddie->unk3B0 & 0x10))) {
        fsa->target = baddie->unk3AC;
        fsa->unk33D = 0;
        if (objSetup->unk2E != -1) {
            baddie->unk3B4 = 1;
            if (animData != NULL) {
                gDLL_3_Animation->vtbl->func21(animData, objSetup->unk24);
            }
        } else {
            fsa->speed = 0.0f;
            fsa->unk278 = 0.0f;
            objData->unk2 = 0;
            temp_a0 = baddie->unk3F8;
            baddie->unk3B4 = 0;
            if (temp_a0 != NULL) {
                if (gDLL_26_Curves->vtbl->func_4288(temp_a0, self, 700.0f, data_164, -1) != 0) {
                    baddie->unk3B2 &= ~8;
                } else {
                    baddie->unk3B2 |= 8;
                }
            }
        }
        if (fsa->hitpoints != 0) {
            if (baddie->unk3B2 & 4) {
                sidekick = objGetSidekick();
                if (sidekick != NULL) {
                    ((DLL_Unknown*)sidekick->dll)->vtbl->func[21].withThreeArgs(sidekick, 1, self);
                }
                fsa->logicState = 0xD;
            } else {
                fsa->logicState = 0xB;
            }
        }
    }
    
    if (fsa->hitpoints > 0) {
        func_8002674C(self);
    }
    
    gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.17f, 1);
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, sAnimStateCallbacks, sLogicStateCallbacks);
    self->animObj = baddie->unk3AC;
}

// offset: 0x17A0 | func: 13
void SharpClaw_func_17A0(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    SharpClaw_DataActual* objData;
    Object* target;
    Object* player;
    u32 temp = 2;

    objData = baddie->objdata;
    player = objGetPlayer();

    if (!(baddie->unk3B0 & 0x40) && (((DLL_210_Player*)player->dll)->vtbl->func50(player) != 0x40)) {
        target = gDLL_33_BaddieControl->vtbl->func17(self, fsa, baddie->unk3E2, 0x8000);
        if (target != NULL) {
            gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, &baddie->unk3B4, 0, 0, 0, 1);
            fsa->unk33D = 0;
            fsa->target = target;
            baddie->unk3B0 &= ~0x10;
            baddie->unk3B4 = temp;
            objData->unkA = temp;
        }
    }
}

#define WRAP_F(x,low,high) { if ((x) > (high)) (x) = -(((high) * 2) - 1) + (x); if ((x) < (low)) (x) = (((high) * 2) - 1) + (x); }
#define CIRCLE_WRAP_F(x) WRAP_F(x, -M_180_DEGREES_F, M_180_DEGREES_F)

// offset: 0x18EC | func: 14
void SharpClaw_func_18EC(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    Object* target;
    s32 pad[3];
    UnkCurvesStruct* curves;
    SharpClaw_DataActual* objData;
    Object* player;
    f32 dx;
    f32 dz;
    f32 yawDiff;
    f32 absYawDiff;

    curves = baddie->unk3F8;
    objData = baddie->objdata;
    player = objGetPlayer();
    
    if (fsa->hitpoints == 0) {
        return;
    }
    
    gDLL_33_BaddieControl->vtbl->func10(self, fsa, 0.17f, 1);
    
    if (((baddie->unk3B2 & 4) == FALSE) && (fsa->logicState != 1)) {
        fsa->logicState = 2;
        if (baddie->unk3B2 & 8) {
            dx = curves->unk0.unk68.x - self->srt.transl.x;
            dz = curves->unk0.unk68.z - self->srt.transl.z;
            dx = sqrtf(SQ(dx) + SQ(dz));
            if ((curves_func_800053B0(&curves->unk0, 10.0f / dx) || curves->unk0.unk10) && 
                gDLL_26_Curves->vtbl->func_4704(curves) && 
                gDLL_26_Curves->vtbl->func_4288(baddie->unk3F8, self, 700.0f, data_164, -1)
            ) {
                baddie->unk3B2 &= ~8;
            }

            if (objData->unk2 < objData->unk1) {
                objData->unk2 += gUpdateRate;
            }

            yawDiff = (((u16)mathAtan2f(curves->unk0.unk74, curves->unk0.unk7C)) - ((u16)self->srt.yaw & 0xFFFF)) + M_180_DEGREES;
            //Similar to CIRCLE_WRAP, but applied to a float?
            CIRCLE_WRAP_F(yawDiff);

            absYawDiff = (yawDiff < 0) ? -yawDiff : yawDiff;
            
            fsa->unk278 = 1.0f - (absYawDiff / (M_180_DEGREES_F - 1.0f));
            if (fsa->unk278 < 0.01f) {
                fsa->unk278 = 0.01f;
            }

            fsa->unk278 *= objData->unk2 / 100.0f;
            fsa->speed = fsa->unk278;
            gDLL_18_objfsa->vtbl->func6(self, fsa, curves->unk0.unk68.x, curves->unk0.unk68.f[2], 0.0f, 0.0f, 60.0f);
            if ((fsa->enteredAnimState) || (fsa->unk33A)) {
                gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_1);
            }
        } else if ((fsa->enteredAnimState) || (fsa->unk33A)) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_0);
        }
        
        if ((fsa->enteredAnimState || fsa->unk33A) && !(baddie->unk3B0 & 0x40) && (((DLL_210_Player*)player->dll)->vtbl->func50(player) != 0x40)) {           
            target = gDLL_33_BaddieControl->vtbl->func17(self, fsa, baddie->unk3E2, 0x8000);
            if (target != NULL) {
                gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, &baddie->unk3B4, 0, 0, 0, 1);
                fsa->target = target;
                fsa->unk33D = 0;
                baddie->unk3B0 &= ~0x10;
                baddie->unk3B4 = 2;
                objData->unkA = 2;
            }
        }
    }
    
    baddie->unk3AC = self->animObj;
    self->animObj = NULL;
    gDLL_18_objfsa->vtbl->tick(self, fsa, gUpdateRateF, gUpdateRateF, sAnimStateCallbacks, sLogicStateCallbacks);
    self->animObj = baddie->unk3AC;
}

// offset: 0x1E2C | func: 15
void SharpClaw_func_1E2C(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    s32 flags;
    s32 soundIdx;
    u32 soundHandle;
    u32 terrainType;
    s16 soundID;

    soundIdx = 0;
    if (baddie->fsa.unk4.mode != 0) {
        terrainType = baddie->fsa.unk4.unk68.unk50[0];
        if (terrainType < 34) {
            soundIdx = dTerrainTypeSoundMap[terrainType];
        }
        if (soundIdx >= 5) {
            soundIdx = 0;
        }
    }
    
    flags = 0;    
    if (fsa->unk308 & 2) {
        flags = 1;
    }
    if (fsa->unk308 & 4) {
        flags |= 2;
    }
    
    if (flags & 1) {
        soundHandle = dll_amSfx->Play(self, dFootstepSounds[soundIdx*2], MAX_VOLUME, NULL, NULL, 0, NULL);
        if (soundHandle != 0) {
            dll_amSfx->SetPitch(soundHandle, (mathRnd(1, 250) * 0.001f) + 0.875f);
        }
    }
    
    if (flags & 2) {
        soundHandle = dll_amSfx->Play(self, dFootstepSounds[(soundIdx*2) + 1], MAX_VOLUME, NULL, NULL, 0, NULL);
        if (soundHandle != 0) {
            dll_amSfx->SetPitch(soundHandle, (mathRnd(1, 250) * 0.001f) + 0.875f);
        }
    }
}

// offset: 0x2044 | func: 16
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_func_2044.s")
#else
//https://decomp.me/scratch/lQrpb
void SharpClaw_func_2044(Object* self, SRT* fxTransform, s32 useModGfx) {
    DLL_IModgfx* modGfxDLL;
    s32 i;
/*0x1B0*/ s32 data_1B0[] = { 0x00000006, 0x00000069, 0x00000069, 0x000000ff }; //60
/*0x1C0*/ s16 data_1C0[] = { 0x0206, 0x0167, 0x0165, 0x0206 }; //58
/*0x1C0*/ s16 data_1C8[] = { 0x0206, 0x0167, 0x0165, 0x0206 }; //50
    f32 fxParam; //4C
    Object* player; //48    
    
    player = objGetPlayer();

    
    if (useModGfx == FALSE) {
        fxParam = 0.014f;
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_325, fxTransform, 0x200001, -1, &fxParam);
        
        fxTransform->scale = 92.0f;
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_323, fxTransform, 0x200001, -1, NULL);
        
        fxParam = 0.015f;
        fxTransform->scale = 231.0f;
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_323, fxTransform, 0x200001, -1, &fxParam);
        
        fxTransform->transl.x -= self->globalPosition.x;
        fxTransform->transl.y -= self->globalPosition.y;
        fxTransform->transl.z -= self->globalPosition.z;
        fxTransform->scale = 123.0f;

        for (i = 0; i < 15; i++) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_324, fxTransform, 2, -1, NULL);
        }
        
        if (player->id != OBJ_Sabre) {
            dll_amSfx->Play(self, data_10C[mathRnd(0, 2)], MAX_VOLUME, NULL, NULL, 0, NULL);
        } else {
            dll_amSfx->Play(self, data_120[mathRnd(0, 2)], MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        dll_amSfx->Play(self, data_D0[mathRnd(0, 4)], MAX_VOLUME, NULL, NULL, 0, NULL);
    } else {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_328, fxTransform, 0x200001, -1, NULL);
        
        fxTransform->transl.x -= self->globalPosition.x;
        fxTransform->transl.y -= self->globalPosition.y;
        fxTransform->transl.z -= self->globalPosition.z;
        dll_amSfx->Play(self, data_10C[mathRnd(3, 4)], MAX_VOLUME, NULL, NULL, 0, NULL);
        
        modGfxDLL = dllLoad(DLL_ID_106, 1);
        
        data_1B0[1] += mathRnd(0, 155);
        data_1B0[2] += mathRnd(0, 155);
        
        fxTransform->yaw = 0;
        fxTransform->pitch = 0;
        fxTransform->roll = 0;
        fxTransform->scale = 1.0f;
        
        modGfxDLL->vtbl->func0(self, 0, fxTransform, 1, -1, data_1B0);
        if (modGfxDLL != NULL) {
            dllFree(modGfxDLL);
        }
        
        if (useModGfx) {}
    }
}
#endif

// offset: 0x2534 | func: 17
static void SharpClaw_func_2534(u8 message, Object* self) {
    s32 index;
    s32 count;
    u32 j;
    Object** objects;
    
    for (objects = objGetObjects(&index, &count); index < count; index++) {
        if (self != objects[index]) {
            for (j = 0; j < ARRAYCOUNT(data_16C); j++) {
                if (objects[index]->id == (s32)data_16C[j]){
                    if (((DLL_IBaddie*)objects[index]->dll)->vtbl->get_fsa_state(objects[index]) == 0xC) {
                        ((DLL_IBaddie*)objects[index]->dll)->vtbl->send_message(objects[index], message);
                    }
                }
            }
        }        
    }
}

// offset: 0x267C | func: 18
s32 SharpClaw_func_267C(u8 message, Object* arg1) {
    s32 index;
    s32 count;
    u32 j;
    Object** objects;
    
    for (objects = objGetObjects(&index, &count); index < count; index++) {
        if (arg1 != objects[index]) {
            for (j = 0; j < ARRAYCOUNT(data_16C); j++) {
                if (objects[index]->id == (s32)data_16C[j]){
                    if (((DLL_IBaddie*)objects[index]->dll)->vtbl->get_fsa_state(objects[index]) == 0xC) {
                        ((DLL_IBaddie*)objects[index]->dll)->vtbl->send_message(objects[index], message);
                        return 1;
                    }
                }
            }
        }        
    }

    return 0;
}

// offset: 0x27C4 | func: 19
s32 SharpClaw_animState0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s16 modAnimIdx;

    if ((fsa->enteredAnimState) || (fsa->unk33A)) {
        fsa->enteredAnimState = TRUE;
        if (mathRnd(0, 2) == 0) {
            modAnimIdx = data_144[mathRnd(0, 3)];
            if (fsa->enteredAnimState) {
                objAnimSet(self, modAnimIdx, 0.0f, 0);
                fsa->unk33A = 0;
            }
        } else {
            if (fsa->enteredAnimState) {
                objAnimSet(self, 0x16, 0.0f, 0);
                fsa->unk33A = 0;
            } 
            fsa->unk33A = 0;
        }
        
        fsa->unk33A = 0;
        fsa->enteredAnimState = FALSE;
        fsa->enteredLogicState = FALSE;
        fsa->speed = 0.0f;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
        fsa->unk2B0 = 8.0f;
        fsa->animTickDelta = 0.005f;
        self->velocity.x = 0.0f;
        self->velocity.y = 0.0f;
        self->velocity.z = 0.0f;
    }
    
    if (fsa->analogInputPower < 0.05f) {
        fsa->unk328 = 0;
        fsa->unk32A = 0;
        fsa->analogInputPower = 0.0f;
    }
    
    if (fsa->analogInputPower >= 0.2f) {
        return FSA_NEXTSTATE_ASYNC(1);
    }
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x298C | func: 20
s32 SharpClaw_animState1(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    f32 animProgress;
    s32 animChanged;
    f32* thresholds;
    s32 idx;
    s16* modAnims;

    baddie = self->data;
    fsa->animTickDelta = 0.025f;
    
    if (fsa->logicState == 2) {
        modAnims = data_134;
    } else {
        modAnims = data_13C;
    }
    
    if (fsa->analogInputPower < 0.005f) {
        fsa->analogInputPower = 0.005f;
    }
    
    gDLL_33_BaddieControl->vtbl->func3(self, fsa, baddie, 1.0f, 12.0f);
    
    animChanged = FALSE;
    animProgress = self->animProgress;

    idx = 0;
    while (self->curModAnimId != modAnims[idx] && idx != 3) {idx++;}
    if (idx >= 3) {
        idx = 0;
    }
    
    thresholds = &data_14C[idx << 1];
    if (fsa->speed < thresholds[0]) {
        animChanged = TRUE;
        if (idx == 1) {
            return FSA_NEXTSTATE_SYNC(0);
        }
        idx--;
    } else if (thresholds[1] <= fsa->speed) {
        animChanged = TRUE;
        if (idx == 0) {
            animProgress = 0.0f;
        }
        idx++;
    }
    
    if (animChanged) {
        objAnimSet(self, modAnims[idx], animProgress, 0);
    }
    objGetAnimChange(self, fsa->unk278, &fsa->animTickDelta);
    
    return 0;
}

// offset: 0x2B54 | func: 21
s32 SharpClaw_animState2(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    SharpClaw_DataActual* objData;

    baddie = self->data;
    
    if (fsa->enteredAnimState || fsa->unk33A) {
        if (fsa->targetDist < 80.0f) {
            if (mathRnd(0, 4) == 0) {
                SharpClaw_func_2534(1, self);
                objData = baddie->objdata;
                objData->unkA = 2;
            }
            
            if (fsa->enteredAnimState) {
                objAnimSet(self, 0x103, 0.0f, 0);
                fsa->unk33A = 0;
            }
            
            fsa->unk33A = 0;
        } else if (fsa->targetDist > 110.0f) {
            if (fsa->enteredAnimState) {
                objAnimSet(self, 4, 0.0f, 0);
                fsa->unk33A = 0;
            }
            
            fsa->unk33A = 0;
        } else {
            if (fsa->enteredAnimState) {
                objAnimSet(self, 0x124, 0.0f, 0);
                fsa->unk33A = 0;
            } 
            
            fsa->unk33A = 0;
        }
        
        fsa->unk33A = 0;
        
        fsa->enteredAnimState = FALSE;
        fsa->enteredLogicState = FALSE;
        fsa->speed = 0.0f;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
        fsa->unk2B0 = 8.0f;
        fsa->animTickDelta = 0.035f;
        self->velocity.x = 0.0f;
        self->velocity.y = 0.0f;
        self->velocity.z = 0.0f;
    }
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 2);
    
    return 0;
}

// offset: 0x2D54 | func: 22
s32 SharpClaw_animState3(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    SharpClaw_DataActual* objData;

    baddie = self->data;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 4, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    fsa->animTickDelta = 0.035f;
    fsa->unk278 = 0.0f;
    
    if (fsa->target != NULL) {
        objData = baddie->objdata;
        if (objData->unk10 < 0x1A) {
            fsa->unk33A = 1;
            gDLL_18_objfsa->vtbl->tick(self, fsa, 1.0f, 1.0f, sAnimStateCallbacks, sLogicStateCallbacks);
        }
    } 
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    
    return 0;
}

// offset: 0x2EA8 | func: 23
s32 SharpClaw_animState4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0x102, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, 5, data_E4);
    
    fsa->unk341 = 1;
    fsa->animTickDelta = 0.035f;
    fsa->unk278 = 1.0f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    
    return 0;
}

// offset: 0x2F98 | func: 24
s32 SharpClaw_animState5(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0x103, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, 2, data_E4);
    
    fsa->unk341 = 1;
    fsa->animTickDelta = 0.035f;
    fsa->unk278 = -1.0f;
    fsa->unk27C = 0.0f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    
    return 0;
}

// offset: 0x3090 | func: 25
s32 SharpClaw_animState6(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0x104, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    fsa->animTickDelta = 0.0336f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 3);
    
    return 0;
}

// offset: 0x3178 | func: 26
s32 SharpClaw_animState7(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0x105, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    fsa->animTickDelta = 0.0336f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 3);
    
    return 0;
}

// offset: 0x3260 | func: 27
s32 SharpClaw_animState8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        fsa->unk341 = 1;
        fsa->animTickDelta = 0.027999999f;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
        objAnimSet(self, 0x125, 0.0f, 4);
        self->srt.yaw -= M_90_DEGREES;
    }
    
    objGetAnimChange(self, fsa->unk278, &fsa->animTickDelta);
    
    return 0;
}

// offset: 0x330C | func: 28
s32 SharpClaw_animState9(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        fsa->unk341 = 1;
        fsa->animTickDelta = 0.027999999f;
        fsa->unk278 = 0.0f;
        fsa->unk27C = 0.0f;
        objAnimSet(self, 0x126, 0.0f, 4);
        self->srt.yaw += M_90_DEGREES;
    }
    
    objGetAnimChange(self, fsa->unk278, &fsa->animTickDelta);
    
    return 0;
}

// offset: 0x33B8 | func: 29
s32 SharpClaw_animState10(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0x12C, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    fsa->animTickDelta = 0.049f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 8);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x34A0 | func: 30
s32 SharpClaw_animState11(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0x12F, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    fsa->animTickDelta = 0.049f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 8);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x3588 | func: 31
s32 SharpClaw_animState12(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0x12D, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState) {
        dll_amSfx->Play(self, dTauntSounds[0], MAX_VOLUME, &baddie->unk3A8, NULL, 0, NULL);
    }
    
    fsa->unk341 = 8;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    fsa->animTickDelta = 0.0039f;
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x36C4 | func: 32
s32 SharpClaw_animState13(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    Baddie* baddie = self->data;

    if (fsa->enteredAnimState) {
        objAnimSet(self, 0x123, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState) {
        dll_amSfx->Play(self, dTauntSounds[1], MAX_VOLUME, &baddie->unk3A8, NULL, 0, NULL);
    }
    
    fsa->unk341 = 8;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    fsa->animTickDelta = 0.0045f;
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, arg2, 4);
    gDLL_18_objfsa->vtbl->func7(self, fsa, arg2, 0);
    
    return 0;
}

// offset: 0x3800 | func: 33
s32 SharpClaw_animState14(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0x12E, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState) {
        dll_amSfx->Play(self, dTauntSounds[2], MAX_VOLUME, &baddie->unk3A8, NULL, 0, NULL);
    }
    
    fsa->unk341 = 8;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    fsa->animTickDelta = 0.006f;
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x393C | func: 34
s32 SharpClaw_animState15(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0x124, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 1;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    fsa->animTickDelta = 0.021f;
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 8);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x3A24 | func: 35
s32 SharpClaw_animState16(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    func_80026128(self, 0xB, 1, -1);
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0x106, 0.24f, 0);
        fsa->unk33A = 0;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, mathRnd(0, 2), data_E4);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 6, 0, data_FC);
    
    fsa->unk341 = 1;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    fsa->animTickDelta = 0.0112000005f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x3B84 | func: 36
s32 SharpClaw_animState17(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    func_80026128(self, 0xA, 1, -1);
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0x107, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, mathRnd(0, 2), data_E4);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 6, 2, data_FC);
    
    fsa->unk341 = 1;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    fsa->animTickDelta = 0.0182f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x3CE8 | func: 37
s32 SharpClaw_animState18(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    func_80026128(self, 0xA, 1, -1);
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0x108, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    gDLL_18_objfsa->vtbl->func12(self, fsa, 0, mathRnd(0, 2), data_E4);
    gDLL_18_objfsa->vtbl->func12(self, fsa, 6, 3, data_FC);
    
    fsa->unk341 = 1;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    fsa->animTickDelta = 0.0182f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x3E4C | func: 38
s32 SharpClaw_animState19(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, data_70[baddie->unk3F0], 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.021f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x3F1C | func: 39
s32 SharpClaw_animState20(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    fsa->animTickDelta = 0.0f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    return 0;
}

// offset: 0x3F4C | func: 40
s32 SharpClaw_animState21(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0x12A, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.007f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 3);
    
    return 0;
}

// offset: 0x4008 | func: 41
s32 SharpClaw_animState22(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0x12B, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    fsa->unk341 = 3;
    fsa->animTickDelta = 0.015f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 3);
    
    return 0;
}

// offset: 0x40C4 | func: 42
s32 SharpClaw_animState23(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;

    fsa->unk341 = 3;
    fsa->animTickDelta = 0.005f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0x101, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (!(fsa->unk34A & 1)) {
        dll_amSfx->Play(self, 0xB21, MAX_VOLUME, NULL, NULL, 0, NULL);
        fsa->unk34A |= 1;
    }
    
    if (!(fsa->unk34A & 2) && (self->animProgress > 0.3f)) {
        baddie = self->data;
        dll_amSfx->Play(self, SOUND_B1F_Slow_Magic_Chimes, MAX_VOLUME, NULL, NULL, 0, NULL);
        fsa->unk34A |= 2;
        gDLL_33_BaddieControl->vtbl->drop_collectable(self, baddie->unk3E0, -1, 0);
    }
    
    return 0;
}

// offset: 0x424C | func: 43
s32 SharpClaw_animState24(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    fsa->unk341 = 0;
    fsa->animTickDelta = 0.01f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    if (fsa->enteredAnimState) {
        dll_amSfx->Play(self, SOUND_B20_Low_Grunt, MAX_VOLUME, NULL, NULL, 0, NULL);
        objAnimSet(self, 0x12B, 0.0f, 0x10);
        fsa->unk33A = 0;
        self->opacity = OBJECT_OPACITY_MAX;
        baddie->unk3B2 |= 0x100;
    }
    
    if (fsa->unk33A) {
        return FSA_NEXTSTATE_SYNC(0);
    } else {
        return 0;
    }
}

// offset: 0x433C | func: 44
s32 SharpClaw_logicState0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->target != NULL) {
        if ((4.0f * gUpdateRateF) < fsa->logicStateTime) {
            return FSA_NEXTSTATE_SYNC(3);
        }

        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_1);
        gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);

    }

    return 0;
}

// offset: 0x4408 | func: 45
s32 SharpClaw_logicState1(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_24);
    }

    if (fsa->unk33A) {
        return FSA_NEXTSTATE_SYNC(0);
    } else {
        return 0;
    }
}

// offset: 0x447C | func: 46
s32 SharpClaw_logicState2(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return 0;
}

// offset: 0x4494 | func: 47
s32 SharpClaw_logicState3(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;

    baddie = self->data;
    if (fsa->enteredLogicState) {
        if (baddie->unk3B0 & 1) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_12);

        } else if (baddie->unk3B0 & 2) {
            dll_amSfx->Play(self, dTauntSounds[0], MAX_VOLUME, &baddie->unk3A8, NULL, 0, NULL);
            return FSA_NEXTSTATE_SYNC(4);
        } else {
            return FSA_NEXTSTATE_SYNC(4);
        }
    }

    if (fsa->unk33A) {
        return FSA_NEXTSTATE_SYNC(4);
    } else {
        return 0;
    }
}

// offset: 0x4574 | func: 48
s32 SharpClaw_logicState4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;

    if (fsa->targetDist < (s32) (baddie->unk3E2 * 0.55f)) {
        baddie->unk3B0 &= ~0x10;
        return FSA_NEXTSTATE_SYNC(5);
    }
    
    return FSA_NEXTSTATE_SYNC(11);
}

// offset: 0x4600 | func: 49
s32 SharpClaw_logicState5(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    SharpClaw_DataActual* objData;
    Object* target;

    baddie = self->data;

    target = gDLL_2_Camera->vtbl->get_target_object();

    
    if (fsa->enteredLogicState) {
        
        objData = baddie->objdata;
        objData->unk0 = 1;
        if ((target != self) && (baddie->unk3B0 & 1)) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_13);
        } else {
            return FSA_NEXTSTATE_SYNC(12);
        }
    } else {
        return FSA_NEXTSTATE_SYNC(12);
    }
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);

    return 0;
}

// offset: 0x46F8 | func: 50
s32 SharpClaw_logicState6(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return FSA_NEXTSTATE_SYNC(4);
}

// offset: 0x4710 | func: 51
s32 SharpClaw_logicState7(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    SharpClaw_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;
    objData = baddie->objdata;
    
    if (baddie->unk3B2 & 0x80) {
        objData->freezeTimer -= gUpdateRateF;
        if (objData->freezeTimer <= 0.0f) {
            objData->freezeTimer = 0.0f;
            baddie->unk3B2 &= ~0x80;
            if (fsa->hitpoints > 0) {
                return FSA_NEXTSTATE_SYNC(12);
            } else {
                return FSA_NEXTSTATE_SYNC(9);
            }
        }
    } else if (fsa->hitpoints <= 0) {
        return FSA_NEXTSTATE_SYNC(9);
    } else if (fsa->unk33A) {
        if (fsa->animState == SharpClaw_ASTATE_21) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_22);
        } else if (fsa->hitpoints < mathRnd(2, 4)) {
            return FSA_NEXTSTATE_SYNC(8);
        } else {
            baddie->unk3B6 = 300;
            return FSA_NEXTSTATE_SYNC(12);
        }
    }

    return 0;
}

// offset: 0x4858 | func: 52
s32 SharpClaw_logicState8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_5);
    } else if (fsa->unk33A) {
        return FSA_NEXTSTATE_SYNC(12);
    }

    return 0;
}

// offset: 0x48CC | func: 53
s32 SharpClaw_logicState9(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;

    baddie = self->data;
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_23);
        fsa->target = NULL;
        fsa->unk4.mode = 0;
        fsa->unk33D = 0;
        baddie->unk3B4 = 2;
        func_800267A4(self);
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
    }

    if (self->opacity == 0) {
        if (self->setup == NULL) {
            objFreeObject(self);
        }

        objSendMesg(objGetPlayer(), 0xE0000, self, 0);
        return FSA_NEXTSTATE_SYNC(10);
    } else {
        return 0;
    }
}

// offset: 0x49E4 | func: 54
s32 SharpClaw_logicState10(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->enteredLogicState) {
        baddie->unk3B4 = 0;
        mainSetBits(baddie->unk39E, 0);
        mainSetBits(baddie->unk39C, 1);
    }
    
    return 0;
}

// offset: 0x4A5C | func: 55
s32 SharpClaw_logicState11(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Unk80009024* vox;
    Baddie* baddie;

    baddie = self->data;
    fsa->unk278 = 1.0f;
    fsa->speed = 1.0f;
    if (fsa->target != NULL) {
        if (fsa->enteredLogicState) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_1);
        }
        
        vox = &baddie->unk34C;
        fsa->xAnalogInput = 0.0f;
        fsa->yAnalogInput = 0.0f;
        bcopy(&self->srt.transl, vox, sizeof(Vec3f));
        bcopy(&fsa->target->srt.transl, &baddie->unk34C.unkC, sizeof(Vec3f));
        vox_func_80009024(vox, &baddie->unk374);

        if ((fsa->targetDist < (baddie->unk3E2 * 0.55f)) && (baddie->unk3B4 == 2)) {
            baddie->unk3B0 &= ~0x10;
            return FSA_NEXTSTATE_SYNC(5);
        }
        
        if (vox->unk25 == 0) {
            gDLL_18_objfsa->vtbl->func6(self, fsa, vox->unk18.x, vox->unk18.f[2], 0.0f, 0.0f, 60.0f);
        } else {
            gDLL_18_objfsa->vtbl->func6(self, fsa, vox->unk18.x, vox->unk18.f[2], 15.0f, 30.0f, 60.0f);
        }
    } else if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_0);
    }
    
    return 0;
}

// offset: 0x4CA4 | func: 56
s32 SharpClaw_logicState12(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    SharpClaw_DataActual* objData;
    s32 pad[2];
    u8 flags;
    s32 random;

    baddie = self->data;
    
    if (fsa->enteredLogicState) {
        fsa->animTickDelta *= 4.0f;
    }
    
    if ((fsa->unk33A) || (fsa->enteredLogicState)) {
        objData = baddie->objdata;
        if (objData->unkA == 1) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_2);
            return 0;
        }
        
        SharpClaw_func_2534(1, self);
        if (gDLL_2_Camera->vtbl->get_target_object() != self) {
            if (objData->unk0 >= 7) {
                if (baddie->unk3B0 & 1) {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_14);
                }
                objData->unk0 = 0;
                
                return 0;
            } else if (objData->unk0 > 0) {
                objData->unk0++;
            }
        }

        flags = gDLL_33_BaddieControl->vtbl->func5(self, fsa, 75.0f);

        if (baddie->unk3B6 > 0x14) {
            baddie->unk3B6 -= 0x14;
        }

        if ((objData->unkC < 2) || (objData->unkC >= 0xE)) {
            if (flags & 1) {
                if (objData->unk10 >= 0x56) {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_4);
                } else if (objData->unk10 >= 0x2C) {
                    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_3);
                } else if (objData->unk10 < 0x1A) {
                    if ((objData->unkC >= 0xE) && (flags & 8)) {
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_6);
                    } else if (flags & 2) {
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_7);
                    } else if (flags & 4) {
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_5);
                    } else {
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_17);
                    }
                } else {
                    if ((objData->unkC == 1) || (objData->unkC == 0xE)) {
                        random = mathRnd(0, 1);
                    } else {
                        random = mathRnd(0, 2);
                    }
                    
                    switch (random) {
                    case 0:
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_16);
                        break;
                    case 1:
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_17);
                        break;
                    default:
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_18);
                        break;
                    }
                    baddie->unk3B6 = 400;
                }
            } else {
                if (mathRnd(0, 1)) {
                    if ((objData->unkC >= 0xE) && (flags & 8)) {
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_6);
                    } else if (flags & 2) {
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_7);
                    } else if (flags & 4) {
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_5);
                    } else {
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_17);
                    }
                } else {
                    switch (mathRnd(0, 2)) {
                    case 0:
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_16);
                        break;
                    case 1:
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_17);
                        break;
                    default:
                        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_18);
                        break;
                    }
                    baddie->unk3B6 = 400;
                }
            }
        } else if ((objData->unkC >= 2) && (objData->unkC < 4)) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_10);
        } else if ((objData->unkC >= 4) && (objData->unkC < 8)) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_9);
        } else if ((objData->unkC >= 8) && (objData->unkC < 12)) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_8);
        } else {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_11);
        }
    }
    
    return 0;
}

// offset: 0x5398 | func: 57
s32 SharpClaw_logicState13(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Object* player;
    Object* sidekick;
    Unk80009024* vox;
    Baddie* baddie;
    Player_Data* playerData;

    baddie = self->data;
    player = objGetPlayer();
    sidekick = objGetSidekick();
    fsa->target = sidekick;

    playerData = player->data;
    
    if (self == playerData->unk0.target) {
        if (fsa->target != NULL) {
            ((DLL_ISidekick*)fsa->target->dll)->vtbl->func21(fsa->target, 0, 0);
        }
        
        fsa->target = player;
        fsa->unk33D = 0;
        gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, (s8*)&baddie->unk3B4, 0, 0, 0, 1);
        baddie->unk3B0 &= ~0x10;
        return FSA_NEXTSTATE_SYNC(3);
    }
    
    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, SharpClaw_ASTATE_1);
    fsa->xAnalogInput = 0.0f;
    fsa->yAnalogInput = 0.0f;
    
    vox = &baddie->unk34C;
    bcopy(&self->srt.transl, vox, sizeof(Vec3f));
    bcopy(&fsa->target->srt.transl, &baddie->unk34C.unkC, sizeof(Vec3f));
    vox_func_80009024(&baddie->unk34C, &baddie->unk374);
    
    if (vox->unk25 == 0) {
        gDLL_18_objfsa->vtbl->func6(self, fsa, vox->unk18.x, vox->unk18.z, 0.0f, 0.0f, 60.0f);
    } else {
        gDLL_18_objfsa->vtbl->func6(self, fsa, vox->unk18.x, vox->unk18.z, 15.0f, 30.0f, 60.0f);
    }
    
    return 0;
}
