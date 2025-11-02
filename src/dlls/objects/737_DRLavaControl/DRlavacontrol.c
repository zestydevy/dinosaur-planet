#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dll.h"
#include "dlls/engine/6_amsfx.h"
#include "functions.h"
#include "game/gamebits.h"
#include "game/objects/object.h"
#include "game/objects/object_id.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/gfx/model.h"
#include "sys/objtype.h"
#include "types.h"
#include "sys/rand.h"

typedef struct {
/*0*/ Object* lfxEmitter;
/*4*/ s16 lfxEmitterVal;
/*6*/ s8 freezeTimer;
/*7*/ s8 freezeDuration;
/*8*/ s8 flags;
/*9*/ s8 effectIndex; //used to switch between different effect presets during Ice Spell cooldown
} DRLavaControl_Data;

typedef struct {
/*00*/    ObjSetup base;
/*18*/    s8 unused18;
/*18*/    s8 dataIndex;
/*1A*/    s16 freezeDuration;   //how long it takes to freeze with Ice Blast spell
/*1C*/    s16 unused1C;
/*1E*/    s16 gameBitFrozen;    //gamebit to set when fully cooled with Ice Blast
} DRLavaControl_Setup;

typedef struct {
/*00*/    ObjSetup base;
/*18*/    s16 unk18;
/*1A*/    s16 unk1A;
/*1C*/    s16 unk1C;
/*1E*/    s16 unk1E;
/*20*/    s16 unk20;
/*22*/    s16 unk22;
/*24*/    s8 unk24;
/*25*/    s8 unk25;
/*26*/    s16 unk26;
} LFXEmitter_Setup;

/*0x0*/ static s16 lfxEmitterUnk1E[] = {
    547, 550, 553
};

static void DRLavaControl_freeze(Object* self);
static void DRLavaControl_freeze_update_effects(Object* self, DRLavaControl_Data* objData, s32 effectIndex);
static void DRLavaControl_create_particles(Object* self, u32 effectIndex);
static Object* DRLavaControl_create_light(Object* self, s32 lfxSetupUnk1E);

// offset: 0x0 | ctor
void DRLavaControl_ctor(void *dll) { }

// offset: 0xC | dtor
void DRLavaControl_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DRLavaControl_setup(Object* self, DRLavaControl_Setup* objSetup, s32 arg2) {
    DRLavaControl_Data* objData;
    s32 isFrozen;

    objData = self->data;
    objData->freezeTimer = objSetup->freezeDuration;
    objData->freezeDuration = objData->freezeTimer;
    
    if (main_get_bits(objSetup->gameBitFrozen)) {
        isFrozen = TRUE;
    } else {
        isFrozen = FALSE;
    }
    
    objData->flags |= isFrozen;
    objData->lfxEmitterVal = lfxEmitterUnk1E[objSetup->dataIndex];
    if (objData->flags & 1) {
        DRLavaControl_freeze_update_effects(self, objData, 0);
        return;
    }
    DRLavaControl_freeze_update_effects(self, objData, 3);
    gDLL_5_AMSEQ2->vtbl->func0(self, 0x102, 0, 0, 0);
}

// offset: 0x130 | func: 1 | export: 1
void DRLavaControl_control(Object* self) {
    DRLavaControl_Data* objData = self->data;
    DRLavaControl_create_particles(self, objData->effectIndex);
}

// offset: 0x170 | func: 2 | export: 2
void DRLavaControl_update(Object* self) {
    //Check if hit with Ice Blast Spell
    if (func_80025F40(self, NULL, NULL, NULL) == 25) {
        DRLavaControl_freeze(self);
    }
}

// offset: 0x1D8 | func: 3 | export: 3
void DRLavaControl_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
}

// offset: 0x1F0 | func: 4 | export: 4
void DRLavaControl_free(Object* self, s32 arg1) {
    DRLavaControl_Data* objData = self->data;

    if (objData->lfxEmitter && (arg1 == 0)) {
        obj_destroy_object(objData->lfxEmitter);
    }
    gDLL_13_Expgfx->vtbl->func4(self);
}

// offset: 0x270 | func: 5 | export: 5
u32 DRLavaControl_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x280 | func: 6 | export: 6
u32 DRLavaControl_get_data_size(Object *self, u32 a1) {
    return sizeof(DRLavaControl_Data);
}

// offset: 0x294 | func: 7
// Called when cooled with the Ice Blast Spell
void DRLavaControl_freeze(Object* self) {
    DRLavaControl_Data* objData;
    DRLavaControl_Setup* objSetup;

    objData = self->data;
    if (objData->flags & 1) {
        return;
    }

    objSetup = (DRLavaControl_Setup*)self->setup;

    if (objData->freezeTimer > 0) {
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_80C_Steam_Hissing, MAX_VOLUME, NULL, NULL, 0, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_5A, NULL, PARTFXFLAG_2, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_5B, NULL, PARTFXFLAG_2, -1, NULL);
        objData->freezeTimer--; //@bug: doesn't use gUpdateRate, frame-rate dependent

        if (objData->freezeTimer == 0) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_80B_Crackling_Freezing, MAX_VOLUME, NULL, NULL, 0, NULL);
            objData->flags |= 1;
            main_set_bits(objSetup->gameBitFrozen, TRUE);
            // diPrintf(" bit set %i ", objSetup->gameBitFrozen);
            main_increment_bits(BIT_DR_Lava_Pools_Cooled_Count);
            gDLL_5_AMSEQ2->vtbl->func0(self, 0x102, 0, 0, 0);
        }
    }

    DRLavaControl_freeze_update_effects(self, objData, objData->freezeTimer / 5);
}

// offset: 0x47C | func: 8
void DRLavaControl_freeze_update_effects(Object* self, DRLavaControl_Data* objData, s32 effectIndex) {
    f32 distance;
    f32 temperature_tValue; //0.0 when cooled, 1.0 when hot
    u16 dataUnused[] = { 288, 287, 286, 282 }; //unused indices?
    Object* waveAnimator;
    
    if (objData->freezeTimer == 0) {
        temperature_tValue = 0.0f;
    } else {
        temperature_tValue = (f32) objData->freezeTimer / (f32) objData->freezeDuration;
    }
    
    distance = 500.0f;
    waveAnimator = obj_get_nearest_type_to(OBJTYPE_29, self, &distance);
    if (waveAnimator) {
        // diPrintf(" WAVE OBJ %x ", waveAnimator);
        ((DLL_Unknown*)waveAnimator->dll)->vtbl->func[7].withOneS32OneF32((s32)waveAnimator, temperature_tValue); //blending wave amplitude to 0 as lava cools?
        ((DLL_Unknown*)waveAnimator->dll)->vtbl->func[8].withTwoArgs((s32)waveAnimator, 1);
        ((DLL_Unknown*)waveAnimator->dll)->vtbl->func[9].withFourArgs((s32)waveAnimator, 0xB3, 0x18, 0x18); //reddish RGB colour?
    }
    
    if (effectIndex != objData->effectIndex) {
        if (objData->lfxEmitter) {
            obj_destroy_object(objData->lfxEmitter);
        }
        if (effectIndex) {
            // diPrintf(" Creating LIGHT %i  x %f z %f \n", arg2, self->srt.transl.x, self->srt.transl.z);
            objData->lfxEmitter = DRLavaControl_create_light(self, (objData->lfxEmitterVal - effectIndex) + 3);
        }
        objData->effectIndex = effectIndex;
    }
}

// offset: 0x604 | func: 9
Object* DRLavaControl_create_light(Object* self, s32 lfxSetupUnk1E) {
    LFXEmitter_Setup* setup;

    setup = (LFXEmitter_Setup*)obj_alloc_create_info(sizeof(LFXEmitter_Setup), OBJ_LFXEmitter);
    
    setup->base.loadParamA = 2;
    setup->base.loadParamB = 1;
    setup->base.loadDistance = 0xFF;
    setup->base.fadeDistance = 0xFF;
    setup->base.x = self->srt.transl.x;
    setup->base.y = self->srt.transl.y + 30.0f;
    setup->base.z = self->srt.transl.z;
    setup->unk20 = 0;
    setup->unk1E = lfxSetupUnk1E;
    setup->unk22 = -1;
    setup->unk18 = 0;
    rand_next(-500, 500);
    setup->unk1A = 0;
    setup->unk1C = 0;
    rand_next(-500, 500);
    setup->unk24 = 1;
    setup->unk25 = 50;
    
    return obj_create((ObjSetup*)setup, 5, self->mapID, -1, self->parent);
}

// offset: 0x720 | func: 10
void DRLavaControl_create_particles(Object* self, u32 effectIndex) {
    //Probabilities for particles to be created (e.g. 1/30, 1/5, 1/60)
    s32 particleChanceA[3] = {30, 5, 5};
    s32 particleChanceB[3] = {60, 10, 10};
    
    if (effectIndex){
        effectIndex--;        
        if (!rand_next(0, particleChanceA[effectIndex])){
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_684, NULL, PARTFXFLAG_2, -1, NULL);
        }
        if (!rand_next(0, particleChanceB[effectIndex])){
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_683, NULL, PARTFXFLAG_2, -1, NULL);
        }
    }
}

/*0x0*/ static const char str_0[] = " bit set %i ";
/*0x10*/ static const char str_10[] = " WAVE OBJ %x ";
/*0x20*/ static const char str_20[] = " Creating LIGHT %i  x %f z %f \n";
