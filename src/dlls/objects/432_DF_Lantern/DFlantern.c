#include "dll.h"
#include "dlls/engine/17_partfx.h"
#include "game/objects/object.h"
#include "game/objects/setups/117.h"
#include "sys/camera.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/vi.h"

typedef struct {
    u16 lfxActionIdxOn;    //LightAction to use when the player is near the lantern
    u16 lfxActionIdxOff;   //LightAction to use when the player isn't near the lantern
    u16 camDistance;       //Camera's current distance from the lantern
    u16 playerRange;       //Range (2D) for emitting a lightAction point light, and playing sound loop
    u16 camRange;          //Range (3D) for drawing the partFX glow (with expensive occlusion checks), and playing sound loop
    u8 useOtherLFXConfig;  //Boolean, affects which lightAction indices are used (TODO: what's the difference between them? Maybe one pair are night-only?)
    u8 flags;              //See `DF_Lantern_Flags`
    u8 prevFlags;          //Used to handle when flags change
    u32 soundHandle;       //For the crackling sound loop
} DF_Lantern_Data;

// offset: 0x0 | ctor
void DF_Lantern_ctor(void* dll) { }

// offset: 0xC | dtor
void DF_Lantern_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void DF_Lantern_obj_Setup(Object* self, DF_Lantern_Setup* objSetup, s32 reset) {
    DF_Lantern_Data* objData = self->data;

    objData->useOtherLFXConfig = objSetup->useOtherLFXConfig;
    objData->flags = 0;

    if (objData->useOtherLFXConfig == FALSE) {
        objData->lfxActionIdxOn = 490;
        objData->lfxActionIdxOff = 491;
        objData->playerRange = 100;
        objData->camRange = 250;
    } else {
        objData->lfxActionIdxOn = 492;
        objData->lfxActionIdxOff = 493;
        objData->playerRange = 100;
        objData->camRange = 250;
    }

    if (objSetup->flags & DF_Lantern_FLAG_20_Use_Fade_Distance) {
        objData->camRange = self->fadeDistance;
    }

    self->srt.scale = objSetup->scale;
    self->srt.roll = (objSetup->roll - 0x7F) << 8;
    self->srt.pitch = (objSetup->pitch - 0x7F) << 8;
    self->srt.yaw = objSetup->yaw << 8;

    if (objSetup->flags & DF_Lantern_FLAG_1_Player_Collision) {
        objData->flags |= DF_Lantern_FLAG_1_Player_Collision;
    }

    if (objData->flags & DF_Lantern_FLAG_1_Player_Collision) {
        func_800267A4(self);
    } else {
        func_8002674C(self);
    }
}

// offset: 0x1C0 | func: 1 | export: 1
void DF_Lantern_obj_Control(Object* self) {
    DF_Lantern_Data* objData;
    f32 projectedX;
    f32 projectedY;
    f32 projectedZ;
    f32 pointX;
    f32 pointY;
    f32 pointZ;
    f32 playerDistance;
    s32 screenX;
    s32 screenY;
    s32 zDepth;
    s32 zDepthOcclude;
    Camera* cam;
    Object* player;
    DF_Lantern_Setup* objSetup;

    objData = self->data;
    objSetup = (DF_Lantern_Setup*)self->setup;

    objData->flags &= ~(DF_Lantern_FLAG_2_Emit_Light | DF_Lantern_FLAG_4_Show_PartFX_Glow | DF_Lantern_FLAG_8_Play_Sound);

    if (gDLL_7_Newday->vtbl->func8(&projectedX) || objSetup->flags & DF_Lantern_FLAG_10_Always_Lit) {
        cam = camGetMain();
        projectedX = self->srt.transl.x - cam->srt.transl.x;
        projectedY = self->srt.transl.y - cam->srt.transl.y;
        projectedZ = self->srt.transl.z - cam->srt.transl.z;
        objData->camDistance = sqrtf(SQ(projectedX) + SQ(projectedY) + SQ(projectedZ));

        player = objGetPlayer();
        projectedX = self->srt.transl.x - player->srt.transl.x;
        projectedZ = self->srt.transl.z - player->srt.transl.z;
        playerDistance = (u16)sqrtf(SQ(projectedX) + SQ(projectedZ));

        if (playerDistance < objData->playerRange) {
            objData->flags |= DF_Lantern_FLAG_2_Emit_Light | DF_Lantern_FLAG_8_Play_Sound;
        }

        if (objData->camDistance < objData->camRange) {
            objData->flags |= DF_Lantern_FLAG_8_Play_Sound;
            pointX = self->srt.transl.x - gWorldX;
            pointY = self->srt.transl.y;
            pointZ = self->srt.transl.z - gWorldZ;
            camProjectPoint(pointX, pointY, pointZ, &projectedX, &projectedY, &projectedZ);
            camClipToScreen(projectedX, projectedY, projectedZ, &screenX, &screenY, NULL);
            zDepthOcclude = viObjDepth(screenX, screenY, self);
            camGetVec3ToCameraNormalized(self->srt.transl.x, self->srt.transl.f[1], self->srt.transl.f[2], &projectedX, &projectedY, &projectedZ);
            camProjectPoint(pointX += (projectedX * 20.0f), pointY += (projectedY * 20.0f), pointZ += (projectedZ * 20.0f), &projectedX, &projectedY, &projectedZ);
            camClipToScreen(projectedX, projectedY, projectedZ, NULL, NULL, &zDepth);
            if ((viContainsPoint(screenX, screenY) != 0) && (zDepth > 0) && (zDepth < zDepthOcclude)) {
                objData->flags |= DF_Lantern_FLAG_4_Show_PartFX_Glow;
            }
        }
    }

    if (objData->flags != objData->prevFlags) {
        if ((objData->flags & DF_Lantern_FLAG_2_Emit_Light) && !(objData->prevFlags & DF_Lantern_FLAG_2_Emit_Light)) {
            lfxAction(self, self, objData->lfxActionIdxOn, 0, 0, 0);
        } else if (((objData->flags & DF_Lantern_FLAG_2_Emit_Light) == FALSE) && (objData->prevFlags & DF_Lantern_FLAG_2_Emit_Light)) {
            lfxAction(self, self, objData->lfxActionIdxOff, 0, 0, 0);
        }

        if ((objData->flags & DF_Lantern_FLAG_8_Play_Sound) && !(objData->prevFlags & DF_Lantern_FLAG_8_Play_Sound)) {
            objData->soundHandle = dll_amSfx->Play(self, SOUND_612_Lantern_Crackle_Loop, MAX_VOLUME, NULL, NULL, 0, NULL);
        } else if (!(objData->flags & DF_Lantern_FLAG_8_Play_Sound) && (objData->prevFlags & DF_Lantern_FLAG_8_Play_Sound)) {
            dll_amSfx->Stop(objData->soundHandle);
        }
    }

    objData->prevFlags = objData->flags;
}

// offset: 0x738 | func: 2 | export: 2
void DF_Lantern_obj_Update(Object* self) { }

// offset: 0x744 | func: 3 | export: 3
void DF_Lantern_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DF_Lantern_Data* objData;
    u16 opacity;

    objData = self->data;

    if (visibility && (objData->flags & DF_Lantern_FLAG_4_Show_PartFX_Glow)) {
        opacity = (1.0f - ((f32)objData->camDistance / objData->camRange)) * 512;
        if (opacity > OBJECT_OPACITY_MAX) {
            opacity = OBJECT_OPACITY_MAX;
        }
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_42B, NULL, PARTFXFLAG_10 | PARTFXFLAG_2, -1, &opacity);
    }
}

// offset: 0x8C8 | func: 4 | export: 4
void DF_Lantern_obj_Free(Object* self, s32 onlySelf) {
    DF_Lantern_Data* objData = self->data;

    if (objData->flags & DF_Lantern_FLAG_2_Emit_Light) {
        lfxAction(self, self, objData->lfxActionIdxOff, 0, 0, 0);
    }

    //@bug: doesn't free soundHandle too?

    gDLL_13_Expgfx->vtbl->func5(self);
}

// offset: 0x94C | func: 5 | export: 5
u32 DF_Lantern_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x95C | func: 6 | export: 6
u32 DF_Lantern_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(DF_Lantern_Data);
}
