#include "common.h"
#include "sys/gfx/modgfx.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 mode;                //Sets object transform flags when nonzero, and enables control function when set to 2
    s16 explosionScale;     //Scale for modGfxDLL function call transform, and for object's objHits
    s16 modGfxArg1;         //Arg1 for the modGfxDLL's function call (type of model to create?)
    s16 gamebitExplode;     //Explodes when this gamebit is set, or sets this gamebit when damaged by barrel explosion
} DRExplodeDoor_Setup;

typedef struct {
    s32 unused0;
} DRExplodeDoor_Data;

typedef enum {
    DRExplodeDoor_MODE_No_Transform_Flags = 0,
    DRExplodeDoor_MODE_Enable_Control_Function = 2
} DRExplodeDoor_Modes;

// offset: 0x0 | ctor
void DRExplodeDoor_ctor(void *dll) { }

// offset: 0xC | dtor
void DRExplodeDoor_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DRExplodeDoor_setup(Object* self, DRExplodeDoor_Setup* objSetup, s32 arg2) {
    func_8002674C(self);
    
    //Set conditional transform flags
    if (objSetup->mode != DRExplodeDoor_MODE_No_Transform_Flags) {
        self->srt.flags |= 0x4000;
    }
    
    //Check if exploded previously
    if (main_get_bits(objSetup->gamebitExplode)) {
        self->srt.flags |= 0x4000;
        self->unkB0 |= 0x2000;
        obj_free_tick(self);
    }

    //Set up objHits
    if (objSetup->explosionScale) {
        func_80026940(
            self, 
            (f32) (objSetup->explosionScale >= 0 ? objSetup->explosionScale : -objSetup->explosionScale)
        );
    }
    
    self->srt.yaw = objSetup->yaw << 8;
}

// offset: 0x120 | func: 1 | export: 1
void DRExplodeDoor_control(Object* self) {
    DRExplodeDoor_Setup* objSetup = (DRExplodeDoor_Setup*)self->setup;
    u16 modGfxArg[3] = {10, 20, 0};
    DLL_IModgfx* dll;
    
    //Explode when gamebit set (only allowed when objSetup->mode is 2)
    if ((objSetup->mode == DRExplodeDoor_MODE_Enable_Control_Function) && main_get_bits(objSetup->gamebitExplode)) {
        dll = dll_load_deferred(DLL_ID_107, 1);
        dll->vtbl->func0(self, 16, NULL, 2, -1, &modGfxArg);
        dll_unload(dll);
        
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_8C7_Explosion_High, MAX_VOLUME, 0, 0, 0, 0);
        obj_free_tick(self);
    }
}

// offset: 0x244 | func: 2 | export: 2
void DRExplodeDoor_update(Object* self) {
    DRExplodeDoor_Setup* objSetup = (DRExplodeDoor_Setup*)self->setup;
    s32 hitDamage;
    s32 pad;
    s16 modGfxArg[3] = {10, 15, 0};
    DLL_IModgfx* modGfxDLL;
    SRT modGfxTrans;
    
    //Check for damage from barrel explosions, or for the explosion gamebit
    if (func_80025F40(self, NULL, NULL, &hitDamage) == 5 || main_get_bits(objSetup->gamebitExplode)) {
        bzero(&modGfxTrans, sizeof(SRT));
        modGfxTrans.scale = objSetup->explosionScale * 0.03125f;
        
        modGfxDLL = dll_load_deferred(DLL_ID_180, 1);
        modGfxDLL->vtbl->func0(self, objSetup->modGfxArg1, &modGfxTrans, 2, -1, &modGfxArg);
        dll_unload(modGfxDLL);
        
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_860_Explosion_Mid, MAX_VOLUME, 0, 0, 0, 0);
        self->unkB0 |= 0x2000;
        self->srt.flags |= 0x4000;
        
        main_set_bits(objSetup->gamebitExplode, 1);
        func_800267A4(self);
    }
}

// offset: 0x3EC | func: 3 | export: 3
void DRExplodeDoor_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}
// offset: 0x440 | func: 4 | export: 4
void DRExplodeDoor_free(Object *self, s32 a1) { }

// offset: 0x450 | func: 5 | export: 5
u32 DRExplodeDoor_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x460 | func: 6 | export: 6
u32 DRExplodeDoor_get_data_size(Object *self, u32 a1){
    return sizeof(DRExplodeDoor_Data);
}
