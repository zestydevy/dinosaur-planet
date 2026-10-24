#include "common.h"
#include "game/objects/object.h"
#include "game/objects/setups/103.h"
#include "game/objects/setups/117.h"
#include "sys/objhits.h"

#include "dlls/objects/226_tumbleweedbush.h"
#include "dlls/objects/227_tumbleweed.h"

/** Positions for weeds to grow at (relative to the tree)
  *
  * Tumbleweeds and Frost Weeds grow at different coords
  */
/*0x0*/ static Vec3f dWeedCoords[2][4] = {
    //Tumbleweeds
    {
        VEC3F(-22, 56, 0), 
        VEC3F(0, 95, 54), 
        VEC3F(18, 90, -12), 
        VEC3F(0, 0, 0)
    }, 

    //Frost Weeds
    {
        VEC3F(0, 90, 56),
        VEC3F(4, 135, -70), 
        VEC3F(34, 94, 0), 
        VEC3F(-60, 88, 0)
    }
};

#define TUMBLEWEED_LIMIT 7

static s8 TumbleweedBush_createTumbleweed(Object* self);

// offset: 0x0 | ctor
void TumbleweedBush_ctor(void* dll) { }

// offset: 0xC | dtor
void TumbleweedBush_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void TumbleweedBush_obj_Setup(Object* self, Ctrl117_Setup* objSetup, s32 reset) {
    s32 pad[2];
    s32 type;
    s32 i;
    TumbleweedBush_Data* objData;

    objData = self->data;
    objData->unk0 = 0.0f;
    objData->growWeedsRadius = objSetup->growWeedsRadius * 2;
    objData->carryingGold = objSetup->carryingGold;
    
    self->srt.roll = (objSetup->roll - 0x7F) << 7;
    self->srt.pitch = (objSetup->pitch - 0x7F) << 7;
    self->srt.yaw = objSetup->yaw << 8;
    self->srt.scale = objSetup->scale;

    func_8002683C(self, self->srt.scale * 15.0f, self->srt.scale * -5.0f, self->srt.scale * 100.0f);

    switch (self->id) {
    case OBJ_TumbleWeedBush1:
        objData->heldWeedCount = 3;
        /* fallthrough */
    case OBJ_TumbleWeedBush3:
        objData->heldWeedCount = 3;
        type = 0;
        break;
    case OBJ_TumbleWeedBush2:
        objData->heldWeedCount = 4;
        type = 1;
        break;
    }
    
    if (reset == FALSE) {
        for (i = 0; i < objData->heldWeedCount; i++) {
            objData->heldWeeds[i] = NULL;
            bcopy(&dWeedCoords[type][i], &objData->heldWeedCoords[i], sizeof(Vec3f));
            objData->heldWeedCoords[i].x *= self->srt.scale;
            objData->heldWeedCoords[i].y *= self->srt.scale;
            objData->heldWeedCoords[i].z *= self->srt.scale;
            mathRotateRPY(&self->srt, (f32*)&objData->heldWeedCoords[i]);
        }
    }
}

// offset: 0x24C | func: 1 | export: 1
void TumbleweedBush_obj_Control(Object* self) {
    TumbleweedBush_Data* objData;
    Object* player;
    Object* hitBy;
    s32 hitSphere;
    s32 hitDamage;
    f32 dx;
    f32 dz;
    f32 dayTime;
    u8 i;
    Object* weed;

    objData = self->data;
    player = objGetPlayer();
    
    //Check for attack collisions
    if (func_80025F40(self, &hitBy, &hitSphere, &hitDamage)) {
        //Drop all tumbleweeds (unless this is TumbleweedBush1 and it's nighttime)
        for (i = 0; i < objData->heldWeedCount; i++){
            if (objData->heldWeeds[i] != NULL) {
                if (self->id != OBJ_TumbleWeedBush1 || gDLL_7_Newday->vtbl->func8(&dayTime) ) {
                    dll_Tumbleweed(objData->heldWeeds[i])->Fall(objData->heldWeeds[i]);
                }
            }
        }
        
        dll_amSfx->Play(self, SOUND_544_Wood_Struck, MAX_VOLUME, 0, 0, 0, 0);
    }
    
    //Get player distance
    dx = self->srt.transl.x - player->srt.transl.x;
    dz = self->srt.transl.z - player->srt.transl.z;
    
    /* Add Tumbleweeds to the tree's branches when the player is nearby,
     * provided the tree has an empty Tumbleweed slot and there are 
     * fewer than 7 of the relevant kind of Tumbleweed currently loaded */
    if ((u16)sqrtf(SQ(dx) + SQ(dz)) < objData->growWeedsRadius) {
        while (TumbleweedBush_createTumbleweed(self) != -1) {};
    }

    //Clear the tree's Tumbleweed slots when the Tumbleweed has fallen from the tree
    for (i = 0; i < objData->heldWeedCount; i++){
        weed = objData->heldWeeds[i];
        if (weed && (dll_Tumbleweed(weed)->GetState(weed) > Tumbleweed_STATE_Fall_from_Tree)) {
            objData->heldWeeds[i] = NULL;
        }
    }
}

// offset: 0x50C | func: 2 | export: 2
void TumbleweedBush_obj_Update(Object* self) { }

// offset: 0x518 | func: 3 | export: 3
void TumbleweedBush_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    Ctrl117_Setup* objSetup = (Ctrl117_Setup*)self->setup;
    
    if (visibility) {
        //Tint the tree using a colour multiplier
        objprintSetMultiplierColor(objSetup->multiplyColourR, objSetup->multiplyColourG, objSetup->multiplyColourB);
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x5A8 | func: 4 | export: 4
void TumbleweedBush_obj_Free(Object* self, s32 onlySelf) { }

// offset: 0x5B8 | func: 5 | export: 5
u32 TumbleweedBush_obj_GetModelFlags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x5C8 | func: 6 | export: 6
u32 TumbleweedBush_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(TumbleweedBush_Data);
}

// offset: 0x5DC | func: 7 | export: 7
/** 
  * Removes a specific Tumbleweed Object from the tree's list of held Tumbleweeds.
  * NOTE: the Tumbleweed Object isn't deleted, just no longer referenced by the tree.
*/
void TumbleweedBush_RemoveTumbleweed(Object* self, Object* tumbleweed) {
    TumbleweedBush_Data* objData;
    s32 i;

    objData = self->data;

    for (i = 0; i < objData->heldWeedCount; i++){
        if (tumbleweed == objData->heldWeeds[i]) {
            objData->heldWeeds[i] = NULL;
        }
    }
}

// offset: 0x620 | func: 8
/* 
 * Attempts to add a Tumbleweed to the tree's branches, returning its index when successful.
 * 
 * Returns an error value of -1 if the tree's held Tumbleweed slots are full,
 * or if too many of the relevant kind of Tumbleweed are already loaded, 
 * or if it's nighttime and this is a TumbleweedBush1.
 */
s8 TumbleweedBush_createTumbleweed(Object* self) {
    TumbleweedBush_Data* objData;
    Ctrl117_Setup* objSetup;
    Ctrl103_Setup* weedSetup;
    Object** objects;
    s32 weedIdx;
    s32 i;
    s32 objCount;
    s32 existingTumbleweedCount;
    s32 objID;
    f32 dayTime;

    objData = self->data;
    objSetup = (Ctrl117_Setup*)self->setup;

    //Get tumbleweed objectID from tree objectID
    switch (self->id){
        default:
            STUBBED_PRINTF("TumbleWeed cannot determin control type because of an incorrect object number.");
            break;
        case OBJ_TumbleWeedBush1:
            if (gDLL_7_Newday->vtbl->func8(&dayTime) == 0) {
                return -1;
            }
            objID = OBJ_Tumbleweed1;
            break;
        case OBJ_TumbleWeedBush2:
            objID = OBJ_Tumbleweed2;
            break;
        case OBJ_TumbleWeedBush3:
            objID = OBJ_Tumbleweed3;
            break;
    }

    //Find empty slot
    for (i = 0, weedIdx = -1; (i < objData->heldWeedCount) && (weedIdx == -1); i++){
        if (objData->heldWeeds[i] == NULL) {
            weedIdx = i;
        }
    }
    if (weedIdx == -1) {
        return -1;
    }
    
    //Count existing tumbleweeds in world
    for (objects = objGetObjects(&i, &objCount), existingTumbleweedCount = 0; i < objCount;) {
        if (objID == objects[i++]->id) { 
            existingTumbleweedCount++;
        }
    }  
    if (existingTumbleweedCount >= TUMBLEWEED_LIMIT) {
        return -1;
    }    

    //Create a Tumbleweed
    weedSetup = (Ctrl103_Setup*)objAllocSetup(sizeof(Ctrl103_Setup), objID);
    weedSetup->base.x = objData->heldWeedCoords[weedIdx].x + self->srt.transl.x;
    weedSetup->base.y = objData->heldWeedCoords[weedIdx].y + self->srt.transl.y;
    weedSetup->base.z = objData->heldWeedCoords[weedIdx].z + self->srt.transl.z;
    weedSetup->base.loadFlags = objSetup->base.loadFlags;
    weedSetup->base.byte5 = objSetup->base.byte5;
    weedSetup->base.byte6 = objSetup->base.byte6;
    weedSetup->base.fadeDistance = objSetup->base.fadeDistance;
    weedSetup->tumbleweed.interactRadius = 64.0f;

    //Check if the Tumbleweed should be carrying a Shiny Nugget
    if (objData->carryingGold & 1) {
        objSetup = (Ctrl117_Setup*)self->setup;
        if ((objSetup->base.uID == 0x292C) && (objData->tumbleweedsGrown == 6)) {
            weedSetup->tumbleweed.carryingGold = 1;

            for (objects = objGetObjects(&i, &objCount); i < objCount; i++) {
                if (objects[i]->id == OBJ_SC_golden_nugge) {
                    weedSetup->base.x = objects[i]->srt.transl.x;
                    weedSetup->base.y = objects[i]->srt.transl.y;
                    weedSetup->base.z = objects[i]->srt.transl.z;

                    //Index break
                    i = objCount;
                }
            }
        }
    }
    
    objData->heldWeeds[weedIdx] = objSetupObject(&weedSetup->base, OBJINIT_STANDALONE | OBJINIT_FLAG4, self->mapID, -1, self->parent);
    dll_Tumbleweed(objData->heldWeeds[weedIdx])->SetHome(objData->heldWeeds[weedIdx], self->srt.transl.x, self->srt.transl.z);
    objData->tumbleweedsGrown++;
    return weedIdx;
}
