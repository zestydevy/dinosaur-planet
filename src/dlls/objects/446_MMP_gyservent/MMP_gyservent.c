#include "common.h"
#include "dlls/engine/17_partfx.h"
#include "game/objects/object.h"
#include "game/objects/setups/51_Generic.h"

/*0x0*/ static u16 dSoundIDs[] = {
    SOUND_AC5_Steam_Venting_Hiss_Low, 
    SOUND_AC6_Steam_Venting_Hiss_High
};

// offset: 0x0 | ctor
void MMP_GeyserVent_ctor(void* dll) { }

// offset: 0xC | dtor
void MMP_GeyserVent_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void MMP_GeyserVent_obj_Setup(Object* self, MMP_GeyserVent_Setup* objSetup, s32 reset) {
    self->opacity = 0;
    self->stateFlags |= OBJSTATE_PRINT_DISABLED | OBJSTATE_UPDATE_DISABLED;
    self->unkDC = mathRnd(10, 200);
}

// offset: 0x80 | func: 1 | export: 1
void MMP_GeyserVent_obj_Control(Object* self) {
    MMP_GeyserVent_Setup* objSetup = (MMP_GeyserVent_Setup*)self->setup;

    if (mainGetBits(objSetup->gamebitOff)) {
        return;
    }
    
    self->unkDC -= gUpdateRate;
    if (self->unkDC < 0) {
        self->unkDC = mathRnd(70, 240);
        self->unkE0 = mathRnd(30, 60);
        dll_amSfx->Play(self, dSoundIDs[mathRnd(0, 1)], MAX_VOLUME, NULL, NULL, 0, NULL);
    }

    if (self->unkE0 != 0) {
        self->unkE0 -= gUpdateRate;
        if (self->unkE0 <= 0) {
            self->unkE0 = 0;
        } else {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_724, NULL, 2, -1, NULL);
        }
    }
}

// offset: 0x1DC | func: 2 | export: 2
void MMP_GeyserVent_obj_Update(Object* self) { }

// offset: 0x1E8 | func: 3 | export: 3
void MMP_GeyserVent_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x200 | func: 4 | export: 4
void MMP_GeyserVent_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x210 | func: 5 | export: 5
u32 MMP_GeyserVent_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x220 | func: 6 | export: 6
u32 MMP_GeyserVent_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return 0;
}
