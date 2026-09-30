#include "common.h"
#include "game/objects/object.h"
#include "sys/gfx/modgfx.h"

/*0x0*/ static DLL_IModgfx* dModGfxDLL = NULL;

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
} DLL792_Setup;

typedef struct {
    s16 unk0;
} DLL792_Data;

// offset: 0x0 | ctor
void DLL792_ctor(void* dll) { }

// offset: 0xC | dtor
void DLL792_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DLL792_obj_Setup(Object* self, DLL792_Setup* objSetup, s32 reset) {
    DLL792_Data* objData = self->data;
    
    objData->unk0 = objSetup->unk1E;
    
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
    
    dModGfxDLL = dllLoad(DLL_ID_182, 1);
    
    //Create blue/purple-ish screen-filling flashes?
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_545, NULL, 0x802, -1, NULL);
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_545, NULL, 0x802, -1, NULL);
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_545, NULL, 0x802, -1, NULL);
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_546, NULL, 0x802, -1, NULL);
}

// offset: 0x170 | func: 1 | export: 1
void DLL792_obj_Control(Object* self) {
    //Create a massive radial flare effect
    if (mathRnd(0, 2) == 0) {
        dModGfxDLL->vtbl->Func0(self, 1, NULL, 4, -1, NULL);
    }
    
    //Creates blue particles, arranged in two vertical columns?
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_547, NULL, 4, -1, NULL);
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_547, NULL, 4, -1, NULL);
    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_547, NULL, 4, -1, NULL);
}

// offset: 0x298 | func: 2 | export: 2
void DLL792_obj_Update(Object* self) { }

// offset: 0x2A4 | func: 3 | export: 3
void DLL792_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x2BC | func: 4 | export: 4
void DLL792_obj_Free(Object* self, s32 onlySelf) {
    gDLL_13_Expgfx->vtbl->func5(self);
    dll_modgfx->Func4(self);
    dllFree(dModGfxDLL);
}

// offset: 0x340 | func: 5 | export: 5
u32 DLL792_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x350 | func: 6 | export: 6
u32 DLL792_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DLL792_Data);
}
