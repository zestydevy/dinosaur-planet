#include "common.h"
#include "game/objects/object.h"

static int NWGeyser_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue);

// offset: 0x0 | ctor
void NWGeyser_ctor(void* dll) { }

// offset: 0xC | dtor
void NWGeyser_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void NWGeyser_obj_Setup(Object* self, ObjSetup* objSetup, s32 reset) {
    self->stateFlags |= OBJSTATE_PRINT_DISABLED | OBJSTATE_UPDATE_DISABLED;
    self->animCallback = NWGeyser_animCallback;
}

// offset: 0x4C | func: 1 | export: 1
void NWGeyser_obj_Control(Object* self) {
    //Switch off after the SnowHorn slams the ground to stop the geyser
    if (mainGetBits(BIT_SW_Grumpy_SnowHorn_Stopped_Geyser)) {
        self->srt.flags = OBJFLAG_INVISIBLE;
        self->stateFlags |= OBJSTATE_CONTROL_DISABLED;
        func_800267A4(self);
        return;
    }

    gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
    func_8002674C(self);
}

// offset: 0x100 | func: 2 | export: 2
void NWGeyser_obj_Update(Object* self) { }

// offset: 0x10C | func: 3 | export: 3
void NWGeyser_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x124 | func: 4 | export: 4
void NWGeyser_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x134 | func: 5 | export: 5
u32 NWGeyser_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x144 | func: 6 | export: 6
u32 NWGeyser_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return 0;
}

// offset: 0x158 | func: 7
int NWGeyser_animCallback(Object* self, Object* animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    TextureAnimator* texAnim;
    s16 scrollU;
    s16 scrollV;

    if (mainGetBits(BIT_SW_Grumpy_SnowHorn_Stopped_Geyser)) {
        animData->unk9D |= 4;
    }
    
    //Scroll geyser textures
    {
        texAnim = objExprGetTexAnimator(self, 0, 0);
        if (texAnim != NULL) {
            scrollU = texAnim->positionU;
            scrollU += 0x20;
            if (scrollU > 0x800) {
                scrollU -= 0x800;
            }
            texAnim->positionU = scrollU;

            scrollV = texAnim->positionV;
            scrollV += 0xA0;
            if (scrollV > 0x800) {
                scrollV -= 0x800;
            }
            texAnim->positionV = scrollV;
        }
        
        texAnim = objExprGetTexAnimator(self, 1, 0);
        if (texAnim != NULL) {
            scrollV = texAnim->positionV;
            scrollV += 0x100;
            if (scrollV > 0x800) {
                scrollV -= 0x800;
            }
            texAnim->positionV = scrollV;
        }
    }
    
    animData->unk62 = 0;
    animData->unk7A = animData->unk7C & ~0x40;
    
    return 0;
}
