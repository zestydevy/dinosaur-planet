#include "common.h"
#include "dlls/objects/338_LFXEmitter.h"
#include "game/objects/object_id.h"
#include "sys/objhits.h"
#include "sys/gfx/modgfx.h"

typedef struct {
    ObjSetup base;
    s16 gamebitFrozen;
    u8 scale;
    u8 unk1B;
    u8 lfxIgnorePlayerDistance;
    u16 lfxIdx;
} DFIceFire_Setup;

typedef struct {
    Object* lfxEmitter;
    u32 soundHandleFlame;
    u32 soundHandleBurn;
    u32 soundHandleHiss;
    s16 state;
    s16 timer;
    s8 flameHealth;
    s8 unk15;
} DFIceFire_Data;

typedef enum {
    DFIceFire_STATE_0_Burning,
    DFIceFire_STATE_1_Frozen,
    DFIceFire_STATE_2_Freezing
} DFIceFire_States;

// offset: 0x0 | ctor
void DFIceFire_ctor(void* dll) { }

// offset: 0xC | dtor
void DFIceFire_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DFIceFire_obj_Setup(Object* self, DFIceFire_Setup* objSetup, s32 reset) {
    DFIceFire_Data* objData;
    f32 scale;

    objData = self->data;
    
    //Set scale
    if (objSetup->scale != 0) {
        self->srt.scale = objSetup->scale * 0.13f;
    }
    
    //Flag that Modgfx haven't been created
    self->unkE0 = FALSE;
    
    //Check if the pipe's already frozen over, otherwise enable firey damage and sounds
    if ((objData->state = mainGetBits(objSetup->gamebitFrozen))) {
        objSetModel(self, 1);
        func_800267A4(self);
    } else {
        func_80026128(self, 0x13, 1, 0xF);
        dll_amSfx->Play(self, SOUND_9D2_Fire_Roaring_Loop, MAX_VOLUME, &objData->soundHandleFlame, NULL, 0, NULL);
        dll_amSfx->Play(self, SOUND_50b_Fire_Burning_High_Loop, MAX_VOLUME, &objData->soundHandleBurn, NULL, 0, NULL);
    }
    
    self->srt.roll = M_180_DEGREES - 1;
    self->srt.yaw = M_180_DEGREES - 1;
    
    objData->unk15 = objSetup->unk1B;
    objData->lfxEmitter = 0;
    objData->flameHealth = 20;
    
    scale = (self->srt.scale / self->def->scale) * 1.6f;
    if (self->objhitInfo != NULL) {
        self->objhitInfo->unk52 *= scale;
        self->objhitInfo->unk56 *= scale;
        self->objhitInfo->unk54 *= scale;
    }
}

// offset: 0x238 | func: 1 | export: 1
void DFIceFire_obj_Control(Object* self) {
    ObjSetup* setup;
    DLL_IModgfx* modGfxDLL;
    Object* player;
    SRT fxTransform;
    LFXEmitter_Setup* lfxSetup;
    s32 createLight;
    DFIceFire_Setup* objSetup;
    f32 playerDistance;
    DFIceFire_Data* objData;

    objData = self->data;
    player = objGetPlayer();
    objSetup = (DFIceFire_Setup*)self->setup;

    //Do nothing after being frozen over with the Ice Blast Spell
    if (objData->state == DFIceFire_STATE_1_Frozen) {
        return;
    }
    
    //Create firey modgfx
    if ((objData->state == DFIceFire_STATE_0_Burning) && (self->unkE0 == FALSE)) {
        fxTransform.transl.x = 0.0f;
        fxTransform.transl.y = 0.0f;
        fxTransform.transl.z = 0.0f;
        fxTransform.scale = objSetup->scale;
        fxTransform.roll = 0;
        fxTransform.yaw = 0;
        dll_modgfx->Func10(self);
        
        modGfxDLL = dllLoad(DLL_ID_116, 1);
        modGfxDLL->vtbl->Func0(self, 0, &fxTransform, 0x10004, -1, 0);
        dllFree(modGfxDLL);
        
        modGfxDLL = dllLoad(DLL_ID_115, 1);
        modGfxDLL->vtbl->Func0(self, 0, &fxTransform, 0x10004, -1, 0);
        dllFree(modGfxDLL);
        
        modGfxDLL = dllLoad(DLL_ID_114, 1);
        modGfxDLL->vtbl->Func0(self, 0, &fxTransform, 0x10004, -1, 0);
        dllFree(modGfxDLL);
        
        self->unkE0 = TRUE;
    }
    
    func_80026128(self, 8, 1, 0);
    
    playerDistance = vec3Distance(&player->globalPosition, &self->globalPosition);
    
    //Handle freezing state
    if (objData->state == DFIceFire_STATE_2_Freezing) {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_5A, NULL, 0, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_5B, NULL, 0, -1, NULL);
        
        //Play steam hiss sound near the end of the countdown
        if ((objData->timer < 60) && (objData->timer >= (60 - gUpdateRate))) {
            dll_amSfx->Play(self, SOUND_80C_Steam_Hissing, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        
        //Become fully frozen after three seconds
        objData->timer -= gUpdateRate;
        if (objData->timer < 0) {
            objData->state = DFIceFire_STATE_1_Frozen;
            dll_amSfx->Play(self, SOUND_80B_Crackling_Freezing, MAX_VOLUME, NULL, NULL, 0, NULL);
            
            //Free roaring flames sound loop
            if (dll_amSfx->IsPlaying(objData->soundHandleFlame)) {
                dll_amSfx->Stop(objData->soundHandleFlame);
            }            
            objData->soundHandleFlame = 0;
            
            //Free crackling fire sound loop
            if (dll_amSfx->IsPlaying(objData->soundHandleBurn)) {
                dll_amSfx->Stop(objData->soundHandleBurn);
            }
            objData->soundHandleBurn = 0;
            
            //Free lfxEmitter
            if (objData->lfxEmitter != NULL) {
                objFreeObject(objData->lfxEmitter);
                objData->lfxEmitter = NULL;
            }
        }
    }
    
    //Handle LFXEmitter
    if (objData->lfxEmitter != NULL) {
        //Unload lfxEmitter when player is at a distance (optionally)
        if ((objSetup->lfxIgnorePlayerDistance == FALSE) && (playerDistance > 110.0f)) {
            objFreeObject(objData->lfxEmitter);
            objData->lfxEmitter = NULL;
        }
    } else {
        //Create lfxEmitter when burning
        createLight = FALSE;
        if (objSetup->lfxIgnorePlayerDistance == FALSE) {
            if (playerDistance < 100.0f) {
                createLight = TRUE;
            }
        } else {
            createLight = TRUE;
        }
        
        if (createLight) {
            lfxSetup = objAllocSetup(sizeof(LFXEmitter_Setup), OBJ_LFXEmitter);
            setup = self->setup;
            lfxSetup->base.loadFlags = setup->loadFlags;
            lfxSetup->base.loadDistance = setup->loadDistance;
            lfxSetup->base.x = self->srt.transl.x;
            lfxSetup->base.y = self->srt.transl.y + mathRnd(0, 30);
            lfxSetup->base.z = self->srt.transl.z;
            lfxSetup->unk1E = objSetup->lfxIdx;
            if (lfxSetup->unk1E == 0) {
                lfxSetup->unk1E = 0x59;
            }
            lfxSetup->unk22 = -1;
            lfxSetup->unk18 = 2400;
            lfxSetup->unk1A = 2400;
            lfxSetup->unk1C = 200;
            objData->lfxEmitter = objSetupObject(&lfxSetup->base, 4 | 1, self->mapID, -1, self->parent);
        }
    }
}

// offset: 0x7D4 | func: 2 | export: 2
void DFIceFire_obj_Update(Object* self) {
    DFIceFire_Data* objData;
    DFIceFire_Setup* objSetup;

    objData = self->data;
    objSetup = (DFIceFire_Setup*)self->setup;
    
    //Check for Ice Blast Spell damage
    if (func_80025F40(self, NULL, NULL, NULL) != Damage_Type_Ice_Blast) {
        return;
    }

    if (objData->flameHealth-- < 0) {
        mainSetBits(objSetup->gamebitFrozen, TRUE);
        objData->state = DFIceFire_STATE_2_Freezing;
        objSetModel(self, 1);
        dll_modgfx->Func10(self);
        objData->timer = 180;
        func_800267A4(self);
        func_80026160(self);
        
        if (dll_amSfx->IsPlaying(objData->soundHandleFlame)) {
            dll_amSfx->Stop(objData->soundHandleFlame);
        }
        dll_amSfx->Play(self, SOUND_9CD, MAX_VOLUME, &objData->soundHandleFlame, NULL, 0, NULL);
    } else {
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_5A, NULL, 0, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_5B, NULL, 0, -1, NULL);
        
        if (dll_amSfx->IsPlaying(objData->soundHandleHiss)) {
            dll_amSfx->Stop(objData->soundHandleHiss);
        }
        dll_amSfx->Play(self, SOUND_9F5_Steam_Hiss, MAX_VOLUME, &objData->soundHandleHiss, NULL, 0, NULL);
    }
}

// offset: 0xA40 | func: 3 | export: 3
void DFIceFire_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xA94 | func: 4 | export: 4
void DFIceFire_obj_Free(Object* self, s32 onlySelf) {
    dll_modgfx->Func5(self);
    gDLL_13_Expgfx->vtbl->func5(self);

    //@bug: soundHandles not freed here just in case?
}

// offset: 0xB04 | func: 5 | export: 5
u32 DFIceFire_obj_GetModelFlags(Object* self) {
    return MODFLAGS_1;
}

// offset: 0xB14 | func: 6 | export: 6
u32 DFIceFire_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DFIceFire_Data);
}
