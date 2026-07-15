#include "common.h"
#include "game/objects/object.h"
#include "sys/gfx/modgfx.h"
#include "sys/math.h"

/*0x0*/ static DLL_IModgfx* dModGfxDLL = NULL;
/*0x4*/ static u8 dFlags = 0;

/*0x0*/ static f32 sTimer;

typedef struct {
    ObjSetup base;
} BossDrakorDiamond_Setup;

typedef enum {
    FLAG_None = 0,
    FLAG_Spinning = 1
} BossDrakorDiamond_Flags;

typedef enum {
    ANIMCMD_Start_Spinning = 1,
    ANIMCMD_Stop_Spinning = 2
} BossDrakorDiamond_AnimCmds;

static int BossDrakorDiamond_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void BossDrakorDiamond_ctor(void* dll) { }

// offset: 0xC | dtor
void BossDrakorDiamond_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void BossDrakorDiamond_setup(Object* self, BossDrakorDiamond_Setup* objSetup, s32 reset) {
    dModGfxDLL = dllLoad(DLL_ID_158, 1);
    sTimer = 0.0f;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED;
    self->animCallback = BossDrakorDiamond_anim_callback;
    self->opacity = 0;
    dFlags |= FLAG_Spinning;
}

// offset: 0xAC | func: 1 | export: 1
void BossDrakorDiamond_control(Object* self) {
    if (dFlags & FLAG_Spinning) {
        self->srt.yaw += M_90_DEGREES / 16; //@framerate-dependent
    }
    
    //Decrement unused timer (which loops every 1/6th of a second)
    sTimer -= gUpdateRateF;
    if (sTimer <= 0.0f) {
        sTimer += 10.0f;
    }
}

// offset: 0x128 | func: 2 | export: 2
void BossDrakorDiamond_update(Object* self) { }

// offset: 0x134 | func: 3 | export: 3
void BossDrakorDiamond_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x14C | func: 4 | export: 4
void BossDrakorDiamond_free(Object* self, s32 onlySelf) {
    if (dModGfxDLL != NULL) {
        dllFree(dModGfxDLL);
        dModGfxDLL = NULL;
    }
}

// offset: 0x1A4 | func: 5 | export: 5
u32 BossDrakorDiamond_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x1B4 | func: 6 | export: 6
u32 BossDrakorDiamond_get_data_size(Object* self, u32 offsetAddr) {
    return 0;
}

// offset: 0x1C8 | func: 7
int BossDrakorDiamond_anim_callback(Object* self, Object* overrideObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    u8 i;

    for (i = 0; i < animData->messageCount; i++) {
        switch (animData->messages[i]) {
        case ANIMCMD_Start_Spinning:
            dFlags |= FLAG_Spinning;
            break;
        case ANIMCMD_Stop_Spinning:
            dFlags &= ~FLAG_Spinning;
            break;
        } 
    }
    
    BossDrakorDiamond_control(self);
    
    return 0;
}
