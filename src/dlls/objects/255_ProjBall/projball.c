#include "common.h"
#include "dlls/objects/255_ProjBall.h"
#include "game/objects/object.h"

typedef struct {
    u8 _unk0[0x8];
    Vec3f unk8;
    f32 unk14;
    f32 unk18;
    u8 _unk1C[0x34 - 0x1C];
    s16 unk34;
    s16 unk36;
    s16 unk38;
    s16 unk3A;
    u16 unk3C;
    u8 unk3E;
} ProjBall_Data; //0x40

/*0x0*/ static u32 data_0 = NULL;

// offset: 0x0 | ctor
void ProjBall_ctor(void* dll) { }

// offset: 0xC | dtor
void ProjBall_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void ProjBall_obj_Setup(Object* self, ProjBall_Setup* objSetup, s32 reset) {
    ProjBall_Data* objData;
    ProjBall_Setup* setup;

    objData = self->data;
    setup = (ProjBall_Setup*)self->setup;
    objData->unk34 = mathRnd(600, 900);
    objData->unk36 = mathRnd(-600, 600);

    if (setup->unk19 == 1) {
        lfxAction(self, self, 0x203, 0, 0, 0);
    } else if (setup->unk19 == 2) {
        lfxAction(self, self, 0x204, 0, 0, 0);
    } else if (setup->unk19 == 3) {
        lfxAction(self, self, 0x204, 0, 0, 0);
    } else {
        lfxAction(self, self, 0x201, 0, 0, 0);
    }

    if (setup->unk1A != 0) {
        objData->unk3E |= 4;
    }

    if (self->objhitInfo != NULL) {
        self->objhitInfo->unkA1 = 1;
    }
}

// offset: 0x188 | func: 1 | export: 1
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/255_ProjBall/ProjBall_obj_Control.s")
#else

static void ProjBall_func_9F0(Object* self, ProjBall_Data* objData, Object* obj);

void ProjBall_obj_Control(Object* self) {
    ProjBall_Data* objData;
    Object* sp68;
    ProjBall_Setup* objSetup;
    SRT fxTransform;
    f32 trackHeight;

    objData = self->data;
    sp68 = (Object*)self->unkE0;
    objSetup = (ProjBall_Setup*)self->setup;
    
    if (objData->unk18 == 0) {
        objData->unk14 = (60.0f / vec3Length(&self->velocity));
    }
    
    objData->unk18 += gUpdateRateF;
    if (objData->unk14 < objData->unk18) {
        func_80026128(self, 0xF, (objSetup->unk19) ? 3 : 1, 0);
    }
    
    fxTransform.transl.x = 0.0f;
    fxTransform.transl.y = 0.0f;
    fxTransform.transl.z = 0.0f;
    fxTransform.scale = objSetup->unk19;

    if (!(objData->unk3E & 1)) {
        objData->unk8.x = self->srt.transl.x;
        objData->unk8.y = self->srt.transl.y;
        objData->unk8.z = self->srt.transl.z;
        objData->unk3E |= 1;
    }
    
    if (self->objhitInfo->unk9D) {
        if (self->objhitInfo->unk9C != 0xE) {
            gDLL_6_AMSFX->vtbl->Play(self, SOUND_6C3, MAX_VOLUME, NULL, NULL, 0, NULL);
        } else {
            gDLL_6_AMSFX->vtbl->Play(self, SOUND_B62, MAX_VOLUME, NULL, NULL, 0, NULL);
            gDLL_24_Waterfx->vtbl->spawn_splash(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, 0.0f);
            gDLL_24_Waterfx->vtbl->spawn_circular_ripple(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, self->srt.yaw, 0.0f, 2);
        }
        objData->unk3A = 50;
    }
    
    if (objData->unk3A != 0) {
        if (!(objData->unk3E & 2)) {
            lfxAction(self, self, 1, 0, 0, 0);
            objData->unk3E |= 2;
        }
        self->velocity.x = 0.0f;
        self->velocity.y = 0.0f;
        self->velocity.z = 0.0f;
        func_80026160(self);
        objData->unk3A--;
        if (objData->unk3A <= 0) {
            objFreeObject(self);
        }
    } else {
        self->prevLocalPosition.x = self->srt.transl.x;
        self->prevLocalPosition.y = self->srt.transl.y;
        self->prevLocalPosition.z = self->srt.transl.z;
        self->srt.yaw += objData->unk36 * gUpdateRate;
        self->srt.roll += objData->unk34 * gUpdateRate;
        dll_partfx->spawn(self, PARTICLE_514, &fxTransform, PARTFXFLAG_4, -1, NULL);
        objData->unk38 -= gUpdateRate;
        if (objData->unk38 <= 0) {
            dll_partfx->spawn(self, PARTICLE_515, &fxTransform, PARTFXFLAG_4, -1, NULL);
            dll_partfx->spawn(self, PARTICLE_516, &fxTransform, PARTFXFLAG_4, -1, NULL);
            dll_partfx->spawn(self, PARTICLE_518, &fxTransform, PARTFXFLAG_4, -1, NULL);
            objData->unk38 = 50;
        }
        if (sp68 != NULL) {
            if (sp68->stateFlags & OBJSTATE_DESTROYED) {
                self->unkE0 = 0;
            } else {
                ProjBall_func_9F0(self, objData, sp68);
            }
        }
        objData->unk8.x += (self->velocity.x * gUpdateRateF);
        objData->unk8.y += (self->velocity.y * gUpdateRateF);
        objData->unk8.z += (self->velocity.z * gUpdateRateF);
        objData->unk3C += gUpdateRate * 1500;
        if (objData->unk3E & 4) {
            objData->unk8.y -= 2.0f * gUpdateRateF;
            if (trackGetHeightNearest(self, objData->unk8.x, objData->unk8.y, objData->unk8.z, &trackHeight, 0) == 0) {
                trackHeight -= 10.0f;
                if (trackHeight < 0.0f && trackHeight > -15.0f) {
                    objData->unk8.y -= trackHeight;
                }
            }
        }
        self->srt.transl.x = objData->unk8.x;
        self->srt.transl.y = objData->unk8.y;
        self->srt.transl.z = objData->unk8.z;
        
        if (sp68 != NULL) {
            self->srt.transl.x += mathSinfInterp(objData->unk3C) * 8.0f;
            self->srt.transl.z += mathCosfInterp(objData->unk3C) * 8.0f;
        }
        
        self->unkDC -= gUpdateRate;
        if (self->unkDC < 0) {
            objFreeObject(self);
        }
    }
}
#endif

// offset: 0x7DC | func: 2 | export: 2
void ProjBall_obj_Update(Object* self) {
    ProjBall_Data* objData;
    SRT fxTransform;
    ProjBall_Setup* objSetup;

    objData = self->data;
    objSetup = (ProjBall_Setup*)self->setup;
    
    fxTransform.transl.x = 0.0f;
    fxTransform.transl.y = 0.0f;
    fxTransform.transl.z = 0.0f;
    fxTransform.scale = objSetup->unk19;
    
    if (self->objhitInfo->unk48 != NULL) {
        dll_partfx->spawn(self, PARTICLE_517, &fxTransform, PARTFXFLAG_1, -1, NULL);
        dll_partfx->spawn(self, PARTICLE_517, &fxTransform, PARTFXFLAG_1, -1, NULL);
        dll_partfx->spawn(self, PARTICLE_517, &fxTransform, PARTFXFLAG_1, -1, NULL);
        objData->unk3A = 50;
    }
}

// offset: 0x8F8 | func: 3 | export: 3
void ProjBall_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) { }

// offset: 0x910 | func: 4 | export: 4
void ProjBall_obj_Free(Object* self, s32 onlySelf) {
    ProjBall_Data* objData = self->data;
    
    if ((objData->unk3E & 2) == FALSE) {
        lfxAction(self, self, 1, 0, 0, 0);
        objData->unk3E |= 2;
    }
    
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x9CC | func: 5 | export: 5
u32 ProjBall_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x9DC | func: 6 | export: 6
u32 ProjBall_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(ProjBall_Data);
}

// offset: 0x9F0 | func: 7
#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/255_ProjBall/ProjBall_func_9F0.s")
#else
void ProjBall_func_9F0(Object* self, ProjBall_Data* objData, Object* obj) {
    f32 dx; //3C
    f32 dy; //38
    f32 dz; //34
    f32 cos; //30
    f32 sin; //2C
    s16 angleB;
    s16 temp1;
    s16 sp26; //26
    s16 angleA; //24
    s16 sp22; //22
    s32 tempA;
    s32 tempB;
    f32 accelerationFactor;
    ObjectStruct74* coords;

    coords = &obj->unk74[obj->unkD4];
    if (coords == NULL) {
        return;
    }
    
    dx = coords->refPoint.x - objData->unk8.x;
    dy = (coords->refPoint.y - 8.0f) - objData->unk8.y;
    dz = coords->refPoint.z - objData->unk8.z;
    
    sp26 = mathAtan2f(self->velocity.x, self->velocity.z);
    sp22 = mathAtan2f(self->velocity.y, sqrtf(SQ(self->velocity.x) + SQ(self->velocity.z)));
    angleA = mathAtan2f(dx, dz);
    angleB = mathAtan2f(dy, sqrtf(SQ(dx) + SQ(dz)));
    
    angleA -= sp26;
    CIRCLE_WRAP(angleA);
    
    angleB -= sp22;
    CIRCLE_WRAP(angleB);
    
    angleA >>= 5;
    if (angleA > (M_1_DEGREE * 2)) {
        angleA = (M_1_DEGREE * 2);
    }
    if (angleA < -(M_1_DEGREE * 2)) {
        angleA = -(M_1_DEGREE * 2);
    }
    
    angleB >>= 4;
    if (angleB > (M_1_DEGREE * 4)) {
        angleB = (M_1_DEGREE * 4);
    }
    if (angleB < -(M_1_DEGREE * 4)) {
        angleB = -(M_1_DEGREE * 4);
    }
    
    angleA = gUpdateRate * angleA;
    angleB = gUpdateRate * angleB;
    sp26 += angleA;
    sp22 += angleB;
    self->velocity.x = mathSinfInterp(sp26);
    self->velocity.z = mathCosfInterp(sp26);
    sin = mathSinfInterp(sp22);
    cos = mathCosfInterp(sp22);
    if (cos != 0.0f) {
        sin /= cos;
    }
    self->velocity.y = sin;
    
    accelerationFactor = 5.0f / sqrtf(SQ(self->velocity.x) + SQ(self->velocity.y) + SQ(self->velocity.z));
    self->velocity.x *= accelerationFactor;
    self->velocity.y *= accelerationFactor;
    self->velocity.z *= accelerationFactor;
}
#endif
