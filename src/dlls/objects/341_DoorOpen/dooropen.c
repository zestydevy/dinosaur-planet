#include "common.h"
#include "sys/gfx/modgfx.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 pitch;       //Stored to objData but otherwise unused. Possibly pitch, based on surrounding fields.
    s16 roll;
    s16 scale;      //Stored to objData, but otherwise unused. Likely scale based on the `*0.01f` (typical of objSetup scale).
    s16 unk1E;      
    s16 gamebit;    //PartFX and modGfx are created when this gamebit is set. 
} DoorOpen_Setup;

typedef struct {
    f32 scale;      //Unused, but likely scale based on the `*0.01f` (typical of objSetup scale).
    s16 pitch;
    s16 gamebit;    //PartFX and modGfx are created when this gamebit is set. 
    s16 unused8;
    s8 fxCreated;   //PartFX and modGfx have been created
} DoorOpen_Data;

/*0x0*/ static DLL_IModgfx* dModGfxDLL = NULL;

static int DoorOpen_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void DoorOpen_ctor(void* dll) { }

// offset: 0xC | dtor
void DoorOpen_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DoorOpen_setup(Object* self, DoorOpen_Setup* objSetup, s32 reset) {
    DoorOpen_Data* objData = self->data;
    
    self->animCallback = DoorOpen_anim_callback;

    objData->scale = objSetup->scale * 0.01f;
    objData->pitch = objSetup->pitch;
    self->srt.yaw = objSetup->yaw << 8;
    self->srt.roll = objSetup->roll << 8;

    objData->gamebit = objSetup->gamebit;
    objData->fxCreated = main_get_bits(objData->gamebit);

    dModGfxDLL = dll_load_deferred(DLL_ID_162, 1);
}

// offset: 0xE0 | func: 1 | export: 1
void DoorOpen_control(Object* self) {
    DoorOpen_Data* objData = self->data;

    if ((objData->gamebit != NO_GAMEBIT) && main_get_bits(objData->gamebit) == FALSE) {
        objData->fxCreated = FALSE;
    }
    if (objData->fxCreated) {
        return;
    }
    
    if ((objData->gamebit != NO_GAMEBIT) && main_get_bits(objData->gamebit)) {
        objData->fxCreated = TRUE;
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_28A, NULL, 4, -1, NULL);
        dModGfxDLL->vtbl->func0(self, objData->pitch, 0, 4, -1, objData);
    }
}

// offset: 0x204 | func: 2 | export: 2
void DoorOpen_update(Object* self) { }

// offset: 0x210 | func: 3 | export: 3
void DoorOpen_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x228 | func: 4 | export: 4
void DoorOpen_free(Object* self, s32 onlySelf) {
    gDLL_13_Expgfx->vtbl->func5(self);
    gDLL_14_Modgfx->vtbl->func5(self);
    dll_unload(dModGfxDLL);
}

// offset: 0x2AC | func: 5 | export: 5
u32 DoorOpen_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x2BC | func: 6 | export: 6
u32 DoorOpen_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DoorOpen_Data);
}

// offset: 0x2D0 | func: 7
int DoorOpen_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    if (animData->lastMessage == 1) {
        animData->lastMessage = 0;
    }
    return 0;
}
