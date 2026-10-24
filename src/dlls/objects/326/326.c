#include "common.h"

// unknown controlNo
typedef struct {
    ObjSetup base;
    u8 modelInstIdx;
    u8 scale;
    u8 yaw;
} DLL326_Setup;

typedef struct {
    s32 unk0; // sometimes stores ID related to object's active sequence?
    s16 unk4; // lifetime?
} LifetimeData;

// offset: 0x0 | ctor
void DLL326_ctor(void* dll) { }

// offset: 0xC | dtor
void DLL326_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DLL326_obj_Setup(Object* self, DLL326_Setup* objSetup, s32 reset) {
    f32 scale;

    self->srt.flags |= OBJFLAG_UNK_2;
    
    //Set scale 
    {
        scale = objSetup->scale & 0xFF;
        if (scale < 10.0f) {
            scale = 10.0f;
        }    
        scale /= 64;
        self->srt.scale = self->def->scale * scale;
        
        //Scale the shadow too
        if (self->shadow != NULL) {
            self->shadow->scale = self->def->shadowScale * scale;
        }
    }
    
    self->modelInstIdx = objSetup->modelInstIdx;

    self->srt.yaw = (objSetup->yaw & 0xFF) << 10;
    
    if (self->modelInstIdx >= self->def->numModels) {
        self->modelInstIdx = 0;
    }
    
    self->unkDC = 0;
    self->unkE0 = 0;
}

// offset: 0xD0 | func: 1 | export: 1   
void DLL326_obj_Control(Object* self) {
    LifetimeData* lifetime;

    if (self->objhitInfo != NULL) {
        lifetime = (LifetimeData*)&self->unkDC;
        if (lifetime->unk4 > 0) {
            lifetime->unk4 -= gUpdateRate;
        }
        
        if ((self->objhitInfo->unk58 & 8) && (lifetime->unk4 <= 0)) {
            lifetime->unk4 = 100;
        }
    }
}

// offset: 0x140 | func: 2 | export: 2
void DLL326_obj_Update(Object* self) { }

// offset: 0x14C | func: 3 | export: 3
void DLL326_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1A0 | func: 4 | export: 4
void DLL326_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x1B0 | func: 5 | export: 5
u32 DLL326_obj_GetModelFlags(Object* self) {
    return MODFLAGS_10 | MODFLAGS_SHADOW | MODFLAGS_1;
}

// offset: 0x1C0 | func: 6 | export: 6
u32 DLL326_obj_GetDataSize(Object* self, u32 offsetAddr){
    return 0;
}
