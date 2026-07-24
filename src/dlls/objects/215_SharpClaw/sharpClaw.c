#include "common.h"
#include "dlls/engine/18_objfsa.h"
#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/210_player.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"


typedef struct {
    u8 _unk0[0x414];
} SharpClaw_Data;

typedef struct {
    s8 unk0;
    s8 unk1;
    s8 unk2[0xA - 2];
    s8 unkA;
    s8 unkB[0x10 - 0xB];
    u16 unk10;
    f32 unk14;
} SharpClaw_DataActual;

/*0x0*/ static u32 data_0[] = {
    0x00000013, 0x00000013, 0x00000015, 0x00000015, 0x00000015, 0x00000013, 0x00000013, 0x00000013, 
    0x00000013, 0x00000013, 0x00000013, 0x00000015, 0x00000015, 0x00000015, 0x00000015, 0x00000013, 
    0x00000015, 0x00000013, 0x00000013, 0x00000013, 0x00000013, 0x00000015, 0x00000013, 0x00000014, 
    0x00000013, 0x00000013, 0x00000013, 0x00000015
};
/*0x70*/ static u32 data_70[] = {
    0x0000010d, 0x0000010d, 0x0000010c, 0x0000010a, 0x0000010a, 0x0000010b, 0x0000010b, 0x0000010b, 
    0x0000010a
};
/*0x94*/ static u32 data_94[] = {
    0xffffff01, 0xffffffff, 0xffffffff, 0x0101ffff, 0xffffffff, 0xffffffff, 0x01ffffff
};
/*0xB0*/ static s16 data_B0[] = {
    SOUND_61_Footstep_Soft, SOUND_62_Footstep_Soft, 
    SOUND_69_Footstep_Wood_Creaky, SOUND_6A_Footstep_Wood_Creaky, 
    SOUND_65_Footstep_Snow, SOUND_66_Footstep_Snow, 
    SOUND_6D_Footstep_Water_Wade, SOUND_6E_Footstep_Water_Wade, 
    SOUND_280_Footstep_Stone, SOUND_281_Footstep_Stone, 
    0x0000, SOUND_169_SharpClaw_Taunt_1, 
    0x0000, SOUND_16A_SharpClaw_Taunt_2, 
    0x0000, SOUND_16B_SharpClaw_Taunt_3
};
/*0xD0*/ static u32 data_D0[] = {
    0x00000236, 0x00000237, 0x00000238, 0x00000239, 0x0000023a
};
/*0xE4*/ static u32 data_E4[] = {
    0x0000023b, 0x0000023f, 0x0000023d, 0x0000023e, 0x00000240, 0x0000023c
};
/*0xFC*/ static u32 data_FC[] = {
    0x000000b6, 0x00000099, 0x00000090, 0x00000091
};
/*0x10C*/ static u32 data_10C[] = {
    0x00000374, 0x00000375, 0x00000376, 0x0000025b, 0x0000025c
};
/*0x120*/ static u32 data_120[] = {
    0x00000377, 0x00000378, 0x00000379, 0x0000025b, 0x0000025c
};
/*0x134*/ static s16 data_134[] = {
    0x0000, 0x0001, 0x0002, 0x0000
};
/*0x13C*/ static s16 data_13C[] = {
    0x0000, 0x0004, 0x0002, 0x0000
};
/*0x144*/ static s16 data_144[] = {
    0x0015, 0x0017, 0x0018, 0x0013
};
/*0x14C*/ static f32 data_14C[] = {
    0, 0.05, 0.03, 0.7, 0.68, 1.05
};
/*0x164*/ static u32 data_164[] = {
    0x00000002, 0x00000003
};

//NOTE: interestingly nearly all the objIDs here are deleted objects!
/*0x16C*/ static u32 data_16C[] = {
    OBJ_ClubSharpClaw, 271, 277, OBJ_SnowClaw, 366, 415, 567, 580
};
/*0x18C*/ static u8 data_18C[] = {
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

/*0x0*/ static u8 bss_0[0x20];
/*0x20*/ static ObjFSA_StateCallback bss_20[26];
/*0x88*/ static ObjFSA_StateCallback bss_88[2];
/*0x90*/ static u8 _bss_90[0x30];

// offset: 0x0 | func: 0
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_func_0.s")

// offset: 0x1F8 | ctor
void SharpClaw_ctor(void* dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_ctor.s")

// offset: 0x238 | dtor
void SharpClaw_dtor(void* dll) { }

// offset: 0x244 | func: 1 | export: 0
void SharpClaw_obj_Setup(Object* self, ObjSetup* setup, s32 reset);
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_obj_Setup.s")
#else
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
    
    self->animCallback = SharpClaw_func_A88;
    
    objData = baddie->objdata;
    baddie->fsa.animState = 0;
    baddie->fsa.logicState = 0;
    baddie->unk3B6 = 0x96;
    objData->unk0 = 0;
    objData->unk1 = objSetup->unk2F;
    objAddObjectType(self, OBJTYPE_63);
}
#endif

// offset: 0x32C | func: 2 | export: 1
void SharpClaw_obj_Control(Object* self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_obj_Control.s")

// offset: 0x5E0 | func: 3 | export: 2
void SharpClaw_obj_Update(Object* self) {
    Baddie* baddie;
    ObjFSA_Data* fsa;
    ObjectHitInfo* objhitInfo;

    baddie = self->data;
    fsa = &((Baddie*)self->data)->fsa;
    
    gDLL_18_objfsa->vtbl->func2(self, fsa, bss_20);
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
void SharpClaw_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_obj_Print.s")

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
    u32 temp;

    temp = arg1;    
    baddie = self->data;
    objData = baddie->objdata;
    
    switch (temp) {
    case 1:
        objData->unkA = 1;
        break;
    case 2:
        objData->unkA = 2;
        break;
    }
}

// offset: 0xA88 | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_func_A88.s")

// offset: 0xE88 | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_func_E88.s")

// offset: 0x14C0 | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_func_14C0.s")

// offset: 0x17A0 | func: 13
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_func_17A0.s")
#else
void SharpClaw_func_17A0(Object* self, Baddie* baddie, ObjFSA_Data* fsa) {
    SharpClaw_DataActual* objData;
    Object* target;
    Object* player;

    objData = baddie->objdata;
    player = objGetPlayer();

    if (!(baddie->unk3B0 & 0x40) && (((DLL_210_Player*)player->dll)->vtbl->func50(player) != 0x40)) {
        target = gDLL_33_BaddieControl->vtbl->func17(self, fsa, baddie->unk3E2, 0x8000);
        if (target != NULL) {
            gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, &baddie->unk3B4, 0, 0, 0, 1);
            fsa->unk33D = 0;
            fsa->target = target;
            baddie->unk3B4 = 2;
            baddie->unk3B0 &= ~0x10;
            objData->unkA = 2;
        }
    }
}
#endif

// offset: 0x18EC | func: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_func_18EC.s")

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
        if (terrainType < 0x22) {
            soundIdx = data_18C[terrainType];
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
        soundHandle = gDLL_6_AMSFX->vtbl->Play(self, data_B0[soundIdx*2], MAX_VOLUME, NULL, NULL, 0, NULL);
        if (soundHandle != 0) {
            gDLL_6_AMSFX->vtbl->SetPitch(soundHandle, (mathRnd(1, 0xFA) * 0.001f) + 0.875f);
        }
    }
    
    if (flags & 2) {
        soundHandle = gDLL_6_AMSFX->vtbl->Play(self, data_B0[(soundIdx*2) + 1], MAX_VOLUME, NULL, NULL, 0, NULL);
        if (soundHandle != 0) {
            gDLL_6_AMSFX->vtbl->SetPitch(soundHandle, (mathRnd(1, 0xFA) * 0.001f) + 0.875f);
        }
    }
}

// offset: 0x2044 | func: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_func_2044.s")

// offset: 0x2534 | func: 17
static void SharpClaw_func_2534(u8 message, Object* arg1) {
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
s32 SharpClaw_func_27C4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    s16 soundID;

    if ((fsa->enteredAnimState) || (fsa->unk33A)) {
        fsa->enteredAnimState = 1;
        if (mathRnd(0, 2) == 0) {
            soundID = data_144[mathRnd(0, 3)];
            if (fsa->enteredAnimState) {
                objAnimSet(self, soundID, 0.0f, 0);
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
        fsa->enteredAnimState = 0;
        fsa->enteredLogicState = 0;
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
        return -1;
    }
    
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 0);
    
    return 0;
}

// offset: 0x298C | func: 20
s32 SharpClaw_func_298C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
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
            return 1;
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
s32 SharpClaw_func_2B54(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
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
            if (fsa->enteredAnimState != 0) {
                objAnimSet(self, 4, 0.0f, 0);
                fsa->unk33A = 0;
            }
            
            fsa->unk33A = 0;
        } else {
            if (fsa->enteredAnimState != 0) {
                objAnimSet(self, 0x124, 0.0f, 0);
                fsa->unk33A = 0;
            } 
            
            fsa->unk33A = 0;
        }
        
        fsa->unk33A = 0;
        
        fsa->enteredAnimState = 0;
        fsa->enteredLogicState = 0;
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
s32 SharpClaw_func_2D54(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
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
            gDLL_18_objfsa->vtbl->tick(self, fsa, 1.0f, 1.0f, bss_20, bss_88);
        }
    } 
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);
    gDLL_18_objfsa->vtbl->func7(self, fsa, updateRate, 1);
    
    return 0;
}

// offset: 0x2EA8 | func: 23
s32 SharpClaw_func_2EA8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
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
s32 SharpClaw_func_2F98(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
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
s32 SharpClaw_func_3090(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState != 0) {
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
s32 SharpClaw_func_3178(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
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
s32 SharpClaw_func_3260(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
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
s32 SharpClaw_func_330C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
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
s32 SharpClaw_func_33B8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredAnimState != 0) {
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
s32 SharpClaw_func_34A0(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
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
s32 SharpClaw_func_3588(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0x12D, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState != 0) {
        gDLL_6_AMSFX->vtbl->Play(self, data_B0[11], 0x7F, &baddie->unk3A8, NULL, 0, NULL);
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
s32 SharpClaw_func_36C4(Object* self, ObjFSA_Data* fsa, f32 arg2) {
    Baddie* baddie = self->data;

    if (fsa->enteredAnimState) {
        objAnimSet(self, 0x123, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState) {
        gDLL_6_AMSFX->vtbl->Play(self, data_B0[13], MAX_VOLUME, &baddie->unk3A8, NULL, 0, NULL);
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
s32 SharpClaw_func_3800(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->enteredAnimState) {
        objAnimSet(self, 0x12E, 0.0f, 0);
        fsa->unk33A = 0;
    }
    
    if (fsa->enteredAnimState) {
        gDLL_6_AMSFX->vtbl->Play(self, data_B0[15], 0x7F, &baddie->unk3A8, NULL, 0, NULL);
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
s32 SharpClaw_func_393C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
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
s32 SharpClaw_func_3A24(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
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
s32 SharpClaw_func_3B84(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    func_80026128(self, 0xA, 1, -1);
    
    if (fsa->enteredAnimState != 0) {
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
s32 SharpClaw_func_3CE8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    func_80026128(self, 0xA, 1, -1);
    
    if (fsa->enteredAnimState != 0) {
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
s32 SharpClaw_func_3E4C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
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
s32 SharpClaw_func_3F1C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    fsa->animTickDelta = 0.0f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    return 0;
}

// offset: 0x3F4C | func: 40
s32 SharpClaw_func_3F4C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
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
s32 SharpClaw_func_4008(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
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
s32 SharpClaw_func_40C4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
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
        gDLL_6_AMSFX->vtbl->Play(self, 0xB21, MAX_VOLUME, NULL, NULL, 0, NULL);
        fsa->unk34A |= 1;
    }
    
    if (!(fsa->unk34A & 2) && (self->animProgress > 0.3f)) {
        baddie = self->data;
        gDLL_6_AMSFX->vtbl->Play(self, 0xB1F, MAX_VOLUME, NULL, NULL, 0, NULL);
        fsa->unk34A |= 2;
        gDLL_33_BaddieControl->vtbl->drop_collectable(self, baddie->unk3E0, -1, 0);
    }
    
    return 0;
}

// offset: 0x424C | func: 43
s32 SharpClaw_func_424C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    fsa->unk341 = 0;
    fsa->animTickDelta = 0.01f;
    fsa->unk278 = 0.0f;
    fsa->unk27C = 0.0f;
    
    if (fsa->enteredAnimState) {
        gDLL_6_AMSFX->vtbl->Play(self, 0xB20, MAX_VOLUME, NULL, NULL, 0, NULL);
        objAnimSet(self, 0x12B, 0.0f, 0x10);
        fsa->unk33A = 0;
        self->opacity = 0xFF;
        baddie->unk3B2 |= 0x100;
    }
    
    if (fsa->unk33A) {
        return 1;
    } else {
        return 0;
    }
}

// offset: 0x433C | func: 44
s32 SharpClaw_func_433C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->target != NULL) {
        if ((4.0f * gUpdateRateF) < fsa->logicStateTime) {
            return 4;
        }

        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 1);
        gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);

    }

    return 0;
}

// offset: 0x4408 | func: 45
s32 SharpClaw_func_4408(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0x18);
    }

    if (fsa->unk33A != 0) {
        return 1;
    } else {
        return 0;
    }
}

// offset: 0x447C | func: 46
s32 SharpClaw_func_447C(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return 0;
}

// offset: 0x4494 | func: 47
s32 SharpClaw_func_4494(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;

    baddie = self->data;
    if (fsa->enteredLogicState) {
        if (baddie->unk3B0 & 1) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0xC);

        } else if (baddie->unk3B0 & 2) {
            gDLL_6_AMSFX->vtbl->Play(self, data_B0[11], 0x7F, &baddie->unk3A8, NULL, 0, NULL);
            return 5;
        } else {
            return 5;
        }
    }

    if (fsa->unk33A != 0) {
        return 5;
    } else {
        return 0;
    }
}

// offset: 0x4574 | func: 48
s32 SharpClaw_func_4574(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;

    if (fsa->targetDist < (s32) (baddie->unk3E2 * 0.55f)) {
        baddie->unk3B0 &= ~0x10;
        return 6;
    }
    
    return 0xC;
}

// offset: 0x4600 | func: 49
s32 SharpClaw_func_4600(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;
    SharpClaw_DataActual* objData;
    Object* target;

    baddie = self->data;

    target = gDLL_2_Camera->vtbl->get_target_object();

    
    if (fsa->enteredLogicState) {
        
        objData = baddie->objdata;
        objData->unk0 = 1;
        if ((target != self) && (baddie->unk3B0 & 1)) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0xD);
        } else {
            return 0xD;
        }
    } else {
        return 0xD;
    }
    
    gDLL_18_objfsa->vtbl->turn_to_target(self, fsa, updateRate, 4);

    return 0;
}

// offset: 0x46F8 | func: 50
s32 SharpClaw_func_46F8(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    return 5;
}

// offset: 0x4710 | func: 51
s32 SharpClaw_func_4710(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    SharpClaw_DataActual* objData;
    Baddie* baddie;

    baddie = self->data;
    objData = baddie->objdata;
    
    if (baddie->unk3B2 & 0x80) {
        objData->unk14 -= gUpdateRateF;
        if (objData->unk14 <= 0.0f) {
            objData->unk14 = 0.0f;
            baddie->unk3B2 &= ~0x80;
            if (fsa->hitpoints > 0) {
                return 0xD;
            } else {
                return 0xA;
            }
        }
    } else if (fsa->hitpoints <= 0) {
        return 0xA;
    } else if (fsa->unk33A != 0) {
        if (fsa->animState == 0x15) {
            gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0x16);
        } else if (fsa->hitpoints < mathRnd(2, 4)) {
            return 9;
        } else {
            baddie->unk3B6 = 0x12C;
            return 0xD;
        }
    }

    return 0;
}

// offset: 0x4858 | func: 52
s32 SharpClaw_func_4858(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    if (fsa->enteredLogicState) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 5);
    } else if (fsa->unk33A) {
        return 0xD;
    }

    return 0;
}

// offset: 0x48CC | func: 53
s32 SharpClaw_func_48CC(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie;

    baddie = self->data;
    if (fsa->enteredLogicState != 0) {
        gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 0x17);
        fsa->target = NULL;
        fsa->unk4.mode = 0;
        fsa->unk33D = 0;
        baddie->unk3B4 = 2;
        func_800267A4(self);
        self->unkAF |= 8;
    }

    if (self->opacity == 0) {
        if (self->setup == NULL) {
            objFreeObject(self);
        }

        objSendMesg(objGetPlayer(), 0xE0000, self, 0);
        return 0xB;
    } else {
        return 0;
    }
}

// offset: 0x49E4 | func: 54
s32 SharpClaw_func_49E4(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
    Baddie* baddie = self->data;
    
    if (fsa->enteredLogicState) {
        baddie->unk3B4 = 0;
        mainSetBits(baddie->unk39E, 0);
        mainSetBits(baddie->unk39C, 1);
    }
    
    return 0;
}

// offset: 0x4A5C | func: 55
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_func_4A5C.s")

// offset: 0x4CA4 | func: 56
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/215_SharpClaw/SharpClaw_func_4CA4.s")

// offset: 0x5398 | func: 57
s32 SharpClaw_func_5398(Object* self, ObjFSA_Data* fsa, f32 updateRate) {
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
        gDLL_33_BaddieControl->vtbl->func9(self, fsa, &baddie->unk34C, baddie->unk39E, &baddie->unk3B4, 0, 0, 0, 1);
        baddie->unk3B0 &= ~0x10;
        return 4;
    }
    
    gDLL_18_objfsa->vtbl->set_anim_state(self, fsa, 1);
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
