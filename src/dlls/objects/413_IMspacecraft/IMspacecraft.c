#include "common.h"
#include "segment_334F0.h"

typedef struct {
    s16 timer;
    u8 unk2;
    u8 unk3;
} DLL413_Data;

/*0x0*/ static SRT fxTransform;

// offset: 0x0 | ctor
void dll_413_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_413_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_413_setup(Object *self, ObjSetup *setup, s32 arg2);
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/413_IMspacecraft/dll_413_setup.s")
#else

int dll_413_func_1AC(Object* self, Object* arg1, AnimObj_Data* arg2, s8 arg3);

//matches but needs anim callback function to be static
void dll_413_setup(Object *self, ObjSetup *arg1, s32 arg2) {
    s32 index;
    TextureAnimator* animTexture;

    self->animCallback = dll_413_func_1AC;
    fxTransform.transl.x = 0.0f;
    fxTransform.transl.y = 0.0f;
    fxTransform.transl.z = 0.0f;

    //Setting thruster textures' initial colour multiplier to black
    for (index = 2; index < 7; index++){
        animTexture = func_800348A0(self, index, 0);
        animTexture->multiplyR = 1;
        animTexture->multiplyG = 1;
        animTexture->multiplyB = 1;
    }
}
#endif

// offset: 0xC4 | func: 1 | export: 1
void dll_413_control(Object *self) {
    if (self->unkDC == 0) {
        self->unkDC = 1;
    }
}

// offset: 0xE0 | func: 2 | export: 2
void dll_413_update(Object *self) { }

// offset: 0xEC | func: 3 | export: 3
void dll_413_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x140 | func: 4 | export: 4
void dll_413_free(Object *self, s32 arg1) {
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x188 | func: 5 | export: 5
u32 dll_413_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x198 | func: 6 | export: 6
u32 dll_413_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL413_Data);
}

// offset: 0x1AC | func: 7
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/413_IMspacecraft/dll_413_func_1AC.s")
#else
//https://decomp.me/scratch/3WlCz
int dll_413_func_1AC(Object* self, Object* arg1, AnimObj_Data* arg2, s8 arg3) {
    DLL413_Data *objData;
    s32 index;
    u8 subcommandID;
    TextureAnimator *animTexture;
    
    objData = self->data;
    
    animTexture = func_800348A0(self, 1, 0);
    animTexture->frame = (((objData->unk3 >> 1) & 1) ^ 1) << 8;
    
    if (!(objData->unk3 & 2)){
        objData->timer -= gUpdateRate;
        if ((objData->timer < 0) != 0){
            objData->unk3 |= 2;
            objData->timer = 120;
        }
    } else {
        objData->unk3 &= ~2;
    }
    
    if (objData->unk3 & 2){
        fxTransform.transl.x = 143.0f;
        fxTransform.transl.y = 16.0f;
        fxTransform.transl.z = -79.0f;
        gDLL_17_partfx->vtbl->spawn(self, 0x133, &fxTransform, 4, -1, NULL);
        
        fxTransform.transl.x = -143.0f;
        fxTransform.transl.y = 16.0f;
        fxTransform.transl.z = -79.0f;
        gDLL_17_partfx->vtbl->spawn(self, 0x133, &fxTransform, 4, -1, NULL);
    }
    
    animTexture = func_800348A0(self, 0, 0);
    animTexture->frame = 0x100;

    for (index = 0; index < arg2->unk98; index++){
        subcommandID = arg2->unk8E[index];

        switch (subcommandID){
        case 1:
        case 2:   
        case 3:
        case 4:
            objData->unk2 ^= 1 << (subcommandID - 1);
            if (1){}
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_509_Engine_Boom, 0x7F, NULL, NULL, 0, NULL);
            break;
        case 5:
            objData->unk2 ^= 0x40 | 0x20 | 0x10;
            break;
        case 6:
            objData->unk3 ^= 8;
            break;
        case 7:
            objData->unk3 ^= 4;
            break;
        default:
            break;
        }
    }
    
    return 0;
}

#endif

// offset: 0x470 | func: 8 | export: 7
s32 dll_413_func_470(Object* self, s32 arg1) {
  DLL413_Data *objData;
  s32 returnVal;
    
  objData = self->data;
    
  returnVal = 0;    
  if (objData->unk2 & (1 << arg1)){
    returnVal = 1;
  } else if (self && self && self){ //@fake?
  }
  return returnVal;
}

// offset: 0x4A4 | func: 9 | export: 8
s32 dll_413_func_4A4(Object *self) {
    DLL413_Data *objData = self->data;
    return objData->unk3 & 4;
}

// offset: 0x4BC | func: 10 | export: 9
// Use colour multiplier to adjust brightness of spaceship's thruster texture
void dll_413_func_4BC(Object* self, s32 animTextureNumber, f32 brightness) {
    u32 componentValue;
    TextureAnimator* animTexture;

    //Make sure components' multiply colour value isn't under 1
    //(0 would disable TextureAnimator's colour multiplier)
    brightness *= 255.0f;
    if (brightness < 1.0f) {
        brightness = 1.0f;
    }
    
    animTexture = func_800348A0(self, animTextureNumber + 2, 0);
    
    componentValue = brightness;
    
    animTexture->multiplyR = componentValue;
    if (animTexture->multiplyR <= 0) { //@note: already ensured it's at least 1?
        animTexture->multiplyR = 1;
    }
    
    animTexture->multiplyG = componentValue;
    if (animTexture->multiplyG <= 0) {
        animTexture->multiplyG = 1;
    }
    
    animTexture->multiplyB = componentValue;
    if (animTexture->multiplyB <= 0) {
        animTexture->multiplyB = 1;
    }
}
