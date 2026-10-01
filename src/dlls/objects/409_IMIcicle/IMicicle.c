#include "common.h"
#include "sys/gfx/modgfx.h"
#include "sys/newshadows.h"

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    u8 fallPlayerRange; //The player must be inside this lateral distance from the icicle, for it to fall
    u8 scale;
    s16 gamebitEnabled; //The icicle's state machine won't run until this gamebit is set (when specified)
    s16 fallDelayTimer; //The icicle will start falling after this delay, after the fall conditions are activated
} IMIcicle_Setup;

typedef struct {
    f32 groundY;
    f32 initialY;
    u32 soundHandle;
    u8 state;
    u8 fallStarted;
    u8 groundFound;
    s16 fallDelayTimer;
} IMIcicle_Data;

typedef enum {
    IMIcicle_STATE_0_Initial,
    IMIcicle_STATE_1_Falling,
    IMIcicle_STATE_2_Stuck_in_Snow,
    IMIcicle_STATE_3_Destroyed
} IMIcicle_States;

/*0x0*/ static DLL_IModgfx* sModGfxDLL;

static f32 IMIcicle_getGroundY(Object* self);
static s32 IMIcicle_shouldIcicleFall(Object* self, Object* obj);

// offset: 0x0 | ctor
void IMIcicle_ctor(void* dll) {
    sModGfxDLL = NULL;
}

// offset: 0x20 | dtor
void IMIcicle_dtor(void* dll) {
    if (sModGfxDLL) {
        dllFree(sModGfxDLL);
    }
    sModGfxDLL = NULL;
}

// offset: 0x70 | func: 0 | export: 0
void IMIcicle_obj_Setup(Object* self, IMIcicle_Setup* objSetup, s32 reset) {
    IMIcicle_Data* objData;
    ObjectHitInfo* objHits;
    ObjectShadow* shadow;

    objData = self->data;
    
    objData->state = IMIcicle_STATE_0_Initial;
    objData->initialY = self->srt.transl.y;
    objData->fallDelayTimer = objSetup->fallDelayTimer;

    self->srt.scale = objSetup->scale / 127.0f;
    
    objHits = self->objhitInfo;
    if (objHits != NULL) {
        objHits->unk52 *= self->srt.scale;
        objHits->unk56 *= self->srt.scale;
        objHits->unk54 *= self->srt.scale;
        objHits->unk58 &= ~1;
    }
    
    shadow = self->shadow;
    if (shadow != NULL) {
        shadow->flags |= OBJ_SHADOW_FLAG_NO_Z_BUFFER | OBJ_SHADOW_FLAG_CUSTOM_OBJ_POS | OBJ_SHADOW_FLAG_CUSTOM_DIR;
        shadow->flags |= OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_CUSTOM_OPACITY;
        shadow->tr.x = self->srt.transl.x;
        shadow->tr.z = self->srt.transl.z;
        shadow->scale *= self->srt.scale;
    }
}

// offset: 0x18C | func: 1 | export: 1
void IMIcicle_obj_Control(Object* self) {
    f32 distance;
    f32 tValue;
    Object* player;
    IMIcicle_Data* objData;
    ObjectHitInfo* objHits;
    ObjectShadow* objShadow;
    s32 opacity;
    IMIcicle_Setup* objSetup;

    objData = self->data;
    objHits = self->objhitInfo;
    objShadow = self->shadow;
    objSetup = (IMIcicle_Setup*)self->setup;
    
    if (sModGfxDLL == NULL) {
        sModGfxDLL = dllLoad(DLL_ID_107, 1);
    }
    
    //Find the ground under the icicle, and set shadow's Y
    if (objData->groundFound == FALSE) {
        objData->groundY = IMIcicle_getGroundY(self);
        if (objData->groundFound) {
            objShadow->tr.y = objData->groundY;
            shadows_func_8004D984(self);
        }
        return;
    }
    
    //Animate shadow's opacity
    if (objShadow != NULL) {
        //Get fall tValue
        tValue = (self->srt.transl.y - objData->groundY) / (objData->initialY - objData->groundY);
        if (tValue > 1.0f) {
            tValue = 1.0f;
        } else if (tValue < 0.0f) {
            tValue = 0.0f;
        }
        tValue = 1.0f - tValue;

        //Get player distance
        player = objGetPlayer();
        if (player != NULL) {
            distance = vec3Distance(&self->globalPosition, &player->globalPosition);
            if (distance > 350.0f) {
                distance = 350.0f;
            } else if (distance < 250.0f) {
                distance = 250.0f;
            }
        } else {
            distance = 350.0f;
        }
        distance = ((distance - 250.0f) / 100.0f);
        distance = 1.0f - distance;
        
        //Set shadow's opacity based on fall tValue and player distance
        opacity = (s32) ((((s32) (120 * tValue)) + 0x40) * distance * (self->opacityWithFade / 255.0f));
        objShadow->opacity = opacity;
    }
    
    //Return early if the icicle's gamebit isn't set yet
    if ((objSetup->gamebitEnabled != NO_GAMEBIT) && (mainGetBits(objSetup->gamebitEnabled) == FALSE)) {
        return;
    }
    
    //State Machine
    switch (objData->state) {
    case IMIcicle_STATE_3_Destroyed:
        break;
    case IMIcicle_STATE_0_Initial:
        if (IMIcicle_shouldIcicleFall(self, objGetPlayer())) {
            objData->fallDelayTimer -= gUpdateRate;
            if (objData->fallDelayTimer <= 0) {
                objData->state = IMIcicle_STATE_1_Falling;
            }
        }
        break;
    case IMIcicle_STATE_1_Falling:
        if (objData->fallStarted == FALSE) {
            objData->fallStarted = TRUE;
            self->velocity.y = 0.0f;
            dll_amSfx->Play(self, SOUND_50E, MAX_VOLUME, NULL, NULL, 0, NULL);

            if (objData->soundHandle != 0) {
                dll_amSfx->Stop(objData->soundHandle);
            }

            dll_amSfx->Play(self, SOUND_50F, MAX_VOLUME, &objData->soundHandle, NULL, 0, NULL);
            objHits->unk58 |= 1;
        }

        objHits->unk40 = 0x10;
        objHits->unk44 = 0x10;
        objHits->unk60 = 1;
        objHits->unk5F = 0xD;

        //Apply gravity and fall
        self->velocity.y += -0.15f * gUpdateRateF;
        self->srt.transl.y += self->velocity.y * gUpdateRateF;
        if (self->srt.transl.y < objData->groundY + 20.0f) {
            self->srt.transl.y = objData->groundY + 20.0f;
            objData->state = IMIcicle_STATE_2_Stuck_in_Snow;
        }
        break;
    case IMIcicle_STATE_2_Stuck_in_Snow:
        objHits->unk40 = 0x10;
        objHits->unk44 = 0x10;
        objHits->unk60 = 1;
        objHits->unk5F = 0xD;
        break;
    }
    
    //Shatter when an object (the player) collides with the icicle
    if (objHits->unk48 != NULL) {
        objHits->unk58 &= ~1;

        objData->state = IMIcicle_STATE_3_Destroyed;

        //Stop fall sound loop
        if (objData->soundHandle != 0) {
            dll_amSfx->Stop(objData->soundHandle);
        }

        //Play shattering sound and create modGfx fragments
        dll_amSfx->Play(self, SOUND_510_Ice_Shatter, MAX_VOLUME, &objData->soundHandle, NULL, 0, NULL);
        sModGfxDLL->vtbl->Spawn(self, 0xE, 0, 0x10000002, -1, 0);
    }
    
    self->velocity.x = 0.0f;
    self->velocity.z = 0.0f;
}

// offset: 0x74C | func: 2 | export: 2
void IMIcicle_obj_Update(Object* self) { }

// offset: 0x758 | func: 3 | export: 3
void IMIcicle_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    IMIcicle_Data* objData = self->data;
    
    if ((objData->state != IMIcicle_STATE_3_Destroyed) && visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x7BC | func: 4 | export: 4
void IMIcicle_obj_Free(Object* self, s32 onlySelf) {
    IMIcicle_Data* objData = self->data;

    if (objData->soundHandle) {
        dll_amSfx->Stop(objData->soundHandle);
    }
}

// offset: 0x818 | func: 5 | export: 5
u32 IMIcicle_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x828 | func: 6 | export: 6
u32 IMIcicle_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(IMIcicle_Data);
}

// offset: 0x83C | func: 7
f32 IMIcicle_getGroundY(Object* self) {
    f32 minDistance;
    s32 sampleIdx;
    s32 pad[2];
    f32 dy;
    s32 count;
    s32 i;
    f32 sampleY;
    TrackHeightResult** heightInfo;
    IMIcicle_Data* objData;

    objData = self->data;
    count = trackGetHeight(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &heightInfo, 0, 0);
    sampleIdx = -1;

    for (i = 0, minDistance = 100000.0f; i < count; i++) {
        dy = self->srt.transl.y - heightInfo[i]->y;
        if ((20.0f < dy) && (dy < minDistance)) {
            minDistance = dy;
            sampleIdx = i;
        }
    }
    
    if (sampleIdx != -1) {
        objData->groundFound = TRUE;
        return heightInfo[sampleIdx]->y;
    } else {
        return self->srt.transl.y;
    }
}

// offset: 0x940 | func: 8
s32 IMIcicle_shouldIcicleFall(Object* self, Object* player) {
    s32 pad[2];
    IMIcicle_Setup* objSetup;
    f32 dy;
    f32 lateralDistance;

    if (player == NULL) {
        return FALSE;
    }
    
    objSetup = (IMIcicle_Setup*)self->setup;
    lateralDistance = vec3DistanceXZ(&self->globalPosition, &player->globalPosition);
    
    dy = self->srt.transl.y - player->srt.transl.y;
    if (dy < 0.0f) {
        dy = 0.0f;
    }

    if ((lateralDistance < objSetup->fallPlayerRange) && (dy < 100.0f)) {
        return TRUE;
    } else {
        return FALSE;
    }
}
