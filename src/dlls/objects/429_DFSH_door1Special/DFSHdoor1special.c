#include "common.h"
#include "game/gamebits.h"

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    s16 unk22;
} DLL429_Setup;

typedef struct {
    u16 unk0;
    u8 unk2;
    u8 unk3;
    u8 unk4;
} DLL429_Data;

static int dll_429_func_324(Object* self, Object* arg1, AnimObj_Data* animData, s8 arg3);

// offset: 0x0 | ctor
void dll_429_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_429_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_429_setup(Object* self, DLL429_Setup* objSetup, s32 reset) {
    DLL429_Data* objData;
    TextureAnimator* texAnim;

    objData = self->data;
    
    if (main_get_bits(objSetup->unk22)) {
        objData->unk3 = 2;
    } else {
        objData->unk3 = 0;
    }
    
    texAnim = func_800348A0(self, 0, 0);
    if (texAnim != NULL) {
        if (objData->unk3 == 2) {
            texAnim->frame = 1;
        } else {
            texAnim->frame = 0;
        }
    }
    
    objData->unk4 = 1;
    self->srt.yaw = objSetup->unk1F << 8;
    self->animCallback = dll_429_func_324;
    
    if (objSetup->unk21 == 0) {
        objSetup->unk21 = 0x40;
    }
    
    self->srt.scale = objSetup->unk21 * ONE_SIXTY_FOURTH_F;
    if (self->srt.scale == 0.0f) {
        self->srt.scale = 1.0f;
    }
    self->srt.scale *= self->def->scale;
    
    if (objSetup->unk1A != NO_GAMEBIT) {
        objData->unk2 = main_get_bits(objSetup->unk1A);
    } else {
        objData->unk2 = 0;
    }
    
    objData->unk0 = 0;
}

// offset: 0x1B0 | func: 1 | export: 1
void dll_429_control(Object* self) {
    DLL429_Data* objData;
    DLL429_Setup* objSetup;
    s32 arg2;

    objData = self->data;
    objSetup = (DLL429_Setup*)self->setup;
    
    if (!objData->unk4) {
        return;
    }
    
    if (objSetup->unk1C && objData->unk2) {
        arg2 = objSetup->unk20;
        gDLL_3_Animation->vtbl->preempt_sequence_time(self, objSetup->unk1C);
    } else {
        arg2 = -1;    
    }

    if (objSetup->unk1E != -1) {
        gDLL_3_Animation->vtbl->start_obj_sequence(objSetup->unk1E, self, arg2);
    }
    objData->unk4 = 0;
}

// offset: 0x290 | func: 2 | export: 2
void dll_429_update(Object *self) { }

// offset: 0x29C | func: 3 | export: 3
void dll_429_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x2F0 | func: 4 | export: 4
void dll_429_free(Object *self, s32 onlySelf) { }

// offset: 0x300 | func: 5 | export: 5
u32 dll_429_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x310 | func: 6 | export: 6
u32 dll_429_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DLL429_Data);
}

// offset: 0x324 | func: 7
int dll_429_func_324(Object* self, Object* arg1, AnimObj_Data* animData, s8 arg3) {
    DLL429_Data* objData;
    DLL429_Setup* objSetup;
    TextureAnimator* texAnim;
    s32 i;
    s32 frame;

    objData = self->data;
    objSetup = (DLL429_Setup*)self->setup;
    
    switch (objData->unk3) {
    case 0:
        if (main_get_bits(objSetup->unk22)) {
            objData->unk3 = 1;
        }
        break;
    case 1:
        texAnim = func_800348A0(self, 0, 0);
        if (texAnim != NULL) {
            frame = texAnim->frame + (gUpdateRate * 8);
            if (frame > 0x100) {
                frame = 0x100;
                objData->unk3 = 2;
            }
            texAnim->frame = frame;
        }
        break;
    case 2:
    default:
        texAnim = func_800348A0(self, 0, 0);
        if (texAnim != NULL) {
            objData->unk0 += gUpdateRate * 800;
            texAnim->frame = 0x100 - ((1.0f - fcos16_precise(objData->unk0)) * 50.0f);
        }
        break;
    }
    
    if (objData->unk2 == 0) {
        if (main_get_bits(objSetup->unk18)) {
            objData->unk2 = 2;
        }
    } else if ((objData->unk2 == 1) && (main_get_bits(objSetup->unk18) == 0)) {
        objData->unk2 = 3;
    }
    
    if (objData->unk2 == 2) {
        for (i = 0; i < animData->messageCount; i++) {
            if (animData->messages[i] == 2) {
                objData->unk2 = 1;
                if (objSetup->unk1A != NO_GAMEBIT) {
                    main_set_bits(objSetup->unk1A, 1);
                }
            }
        }
    } else if (objData->unk2 == 3) {
        for (i = 0; i < animData->messageCount; i++) {
            if (animData->messages[i] == 1) {
                objData->unk2 = 0;
                if (objSetup->unk1A != 1) { //@bug?: should this be != -1 (NO_GAMEBIT)?
                    main_set_bits(objSetup->unk1A, 0);
                }
            }
        }
    }
    
    return !(objData->unk2 == 2) && !(objData->unk2 == 3);
}
