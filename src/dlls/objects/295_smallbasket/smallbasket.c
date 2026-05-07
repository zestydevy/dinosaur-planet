#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dll.h"
#include "types.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object_id.h"
#include "game/objects/object.h"
#include "sys/dll.h"
#include "sys/joypad.h"
#include "sys/main.h"
#include "sys/gfx/model.h"
#include "sys/gfx/modgfx.h"
#include "sys/objects.h"
#include "sys/objmsg.h"
#include "sys/objprint.h"
#include "sys/objtype.h"
#include "sys/rand.h"

#include "dlls/objects/common/collectable.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/297_scarab.h"
#include "dlls/objects/615_crawler.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 yaw;
/*19*/ s8 storedItemID;
/*1A*/ s16 storedItemQuantity;
/*1C*/ s16 unk1C;                   //some time (in seconds)
/*1E*/ s16 gamebit;
/*20*/ s16 autoThrowRadius;
} SmallBasket_Setup;

typedef struct{
/*00*/ s32 unk0;                //timer
/*04*/ s32 unk4;                //timer value to set for unk0
/*08*/ u32 soundHandle;
/*0C*/ s16 playerMessageLo;
/*0E*/ s16 playerMessageHi;
/*10*/ s16 unk10;               //timer
/*12*/ s16 autoThrowRadius;
/*14*/ s16 shakeSoundTimer;
/*16*/ s16 soundID;
/*18*/ s8 unused18;
/*19*/ s8 unused19;
/*1A*/ s16 unk1A;               //timer
/*1C*/ s16 unused1C;
/*1E*/ s16 gamebit;             //When gamebit is set, causes basket to disappear immediately after setup
/*20*/ s8 unused20;
/*21*/ s8 carryFlags;
/*22*/ s8 beingCarried;
/*23*/ s8 throwFlags;                //flags/modes?
/*24*/ s8 unused24;
/*25*/ u8 storedItemType;
/*26*/ u8 storedItemCount;
} SmallBasket_Data;

/** Contents of the basket */
typedef enum {
    SmallBasket_ITEM_Scorpion = 0,
    SmallBasket_ITEM_Scarab_Green = 1,
    SmallBasket_ITEM_Scarab_Red = 2,
    SmallBasket_ITEM_Scarab_Gold = 3,
    SmallBasket_ITEM_Scarab_Rainbow = 4,
    SmallBasket_ITEM_Energy_Egg = 5,
    SmallBasket_ITEM_Apple = 6,
    SmallBasket_ITEM_Prioritise_Health = 7  //Contents determined by player health (apples/Scarabs/nothing), with randomised counts
} SmallBasket_Items;

typedef enum {
    Basket_NOT_CARRIED = 0,
    Basket_FLAG_1 = 1,
    Basket_STOP_CARRYING = 2
} SmallBasket_Carry_Flags;

typedef enum {
    Basket_NOT_THROWN = 0,
    Basket_THROWN = 1,
    Basket_THROWN_No_Items = 2
} SmallBasket_Throw_Flags;

/*0x0*/ static DLL_IModgfx* dModGfxDLL = NULL;
/*0x4*/ static s32 _data_4[4] = {0, 0, 0, 0};

static s32 smallbasket_handle_being_lifted(Object* self, Object* player, SmallBasket_Data* objData);
static int smallbasket_handle_collision(Object* self);
static s32 smallbasket_create_items(Object* self, Object* player, SmallBasket_Data* objData);
static void smallbasket_handle_attack_collisions(Object* arg0, Object* arg1, SmallBasket_Data* arg2);

/*0x0*/ static Vec4f sVec;

// offset: 0x0 | ctor
void smallbasket_ctor(void *dll) { }

// offset: 0xC | dtor
void smallbasket_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void smallbasket_setup(Object* self, SmallBasket_Setup* setup, s32 arg2) {
    SmallBasket_Data* objData;

    objData = self->data;
    self->objhitInfo->unk44 = 0x10;
    self->objhitInfo->unk40 = 0x10;
    func_800267A4(self);
    obj_add_object_type(self, OBJTYPE_18);

    if (setup->unk1C == 0) {
        objData->unk4 = 0;
    } else {
        objData->unk4 = setup->unk1C * 60;
    }

    dModGfxDLL = dll_load_deferred(DLL_ID_107, 1);

    objData->shakeSoundTimer = rand_next(0, 100) + 300;
    objData->storedItemCount = setup->storedItemQuantity;
    self->srt.yaw = setup->yaw << 8;
    objData->gamebit = setup->gamebit;

    objData->autoThrowRadius = setup->autoThrowRadius;
    if (objData->autoThrowRadius == 0) {
        objData->autoThrowRadius = 20;
    }
    
    objData->unk1A = 800;
    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;
    objData->storedItemType = setup->storedItemID;
    self->prevLocalPosition.y = self->srt.transl.y;
    self->prevLocalPosition.x = self->srt.transl.z;

    if (main_get_bits(objData->gamebit)) {
        objData->unk0 = 1;
        func_800267A4(self);
    }

    if (self->id == OBJ_SmallBasket) {
        objData->soundID = SOUND_6B6_Crate_Smash;
        return;
    }

    objData->soundID = SOUND_371_Crate_Smash;
}

// offset: 0x1B8 | func: 1 | export: 1
void smallbasket_control(Object* self) {
    Object* player;
    SmallBasket_Setup* objSetup;
    SmallBasket_Data* objData;
    s32 pad;
    SRT srt;
    ObjFSA_Data* playerFSA;
    f32 sp40;

    player = get_player();
    objSetup = (SmallBasket_Setup*)self->setup;

    sp40 = 1.0f;
    gDLL_7_Newday->vtbl->func5(&sp40);
    objData = self->data;

    if (gDLL_29_Gplay->vtbl->did_time_expire(objSetup->base.uID) == FALSE){
        return;
    }    

    playerFSA = player->data;

    if (objData->unk1A <= 0) {
        objData->unk1A = 800;
        objData->unk10 = 1;
        objData->throwFlags = Basket_NOT_THROWN;
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        smallbasket_create_items(self, player, objData);
        self->velocity.x = 0.0f;
        self->velocity.z = 0.0f;
    }
    
    if (objData->unk0) {
        objData->unk0 -= (s16) (gUpdateRateF * sp40);
        if (objData->unk0 <= 0) {
            objData->unk0 = 0;
            objData->unk10 = 0;
            func_8002674C(self);
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        }
    } else {
        if (objData->unk10) {
            objData->unk10 -= gUpdateRate;
            if (objData->unk10 <= 0) {
                if (objData->unk4) {
                    objData->unk0 = objData->unk4;
                } else {
                    objData->unk0 = 1;
                }

                gDLL_29_Gplay->vtbl->add_time(objSetup->base.uID, objData->unk4);
                self->srt.transl.x = objSetup->base.x;
                self->srt.transl.y = objSetup->base.y;
                self->srt.transl.z = objSetup->base.z;
                self->prevLocalPosition.x = objSetup->base.x;
                self->prevLocalPosition.y = objSetup->base.y;
                self->prevLocalPosition.z = objSetup->base.z;
                self->velocity.x = 0.0f;
                self->velocity.y = 0.0f;
                self->velocity.z = 0.0f;
                func_800267A4(self);
            }

            if (objData->unk10 <= 50) {
                return;
            }
        }

        if (objData->throwFlags != Basket_THROWN) {
            if (objData->carryFlags == Basket_NOT_CARRIED) {
                objData->carryFlags = smallbasket_handle_being_lifted(self, player, objData);
                if (objData->carryFlags != Basket_NOT_CARRIED) {
                    objData->beingCarried = TRUE;
                }

                if (self->unkE0 == 0) {
                    func_8002674C(self);
                    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
                }

                self->prevLocalPosition.f[1] = self->srt.transl.z;
                self->prevLocalPosition.f[2] = self->srt.transl.z;
                self->prevLocalPosition.f[0] = self->srt.transl.x;
            } else {
                func_800267A4(self);
                self->objhitInfo->unk10.f[0] = self->srt.transl.x;
                self->objhitInfo->unk10.f[1] = self->srt.transl.y;
                self->objhitInfo->unk10.f[2] = self->srt.transl.z;
                self->unkAF |= ARROW_FLAG_8_No_Targetting;
                
                //Attempt to place down the basket when pressing A (refused if there isn't enough room to place it down)
                if (joy_get_pressed(0) & A_BUTTON) {
                    if (((DLL_210_Player*)player->dll)->vtbl->func49(player)) {
                        objData->beingCarried = FALSE;
                    } else {
                        gDLL_6_AMSFX->vtbl->play(player, SOUND_912_Object_Refused, MAX_VOLUME, NULL, NULL, 0, NULL);
                    }
                }
                
                if (objData->beingCarried) {
                    objData->unk10 = 0;
                    objData->unk0 = 0;
                    obj_send_mesg(
                        player, 
                        0x100010,
                        self, 
                        (void*)((objData->playerMessageHi << 0x10) | (objData->playerMessageLo & 0xFFFF))
                    );
                }
                
                if (self->unkE0 == 1) {
                    objData->carryFlags = Basket_STOP_CARRYING;
                }
                
                //Throw the basket
                if ((objData->carryFlags == Basket_STOP_CARRYING) && (self->unkE0 == 0) && (player->curModAnimId != Player_MODANIM_447_Carrying_Idle)) {
                    objData->carryFlags = Basket_NOT_CARRIED;
                    objData->throwFlags = Basket_THROWN;
                    self->velocity.f[1] = (playerFSA->analogInputPower * 0.75f) + 2.2f;
                    self->velocity.f[2] = (playerFSA->analogInputPower * -0.75f) + -2.2f;
                    srt.roll = 0;
                    srt.pitch = 0;
                    srt.transl.x = 0.0f;
                    srt.transl.y = 0.0f;
                    srt.transl.z = 0.0f;
                    srt.scale = 1.0f;
                    srt.yaw = player->srt.yaw;
                    rotate_vec3((SRT*)&srt, self->velocity.f);
                    gDLL_6_AMSFX->vtbl->play(self, SOUND_637_Heavy_Whoosh, 0x43, NULL, NULL, 0, NULL);
                    gDLL_6_AMSFX->vtbl->play(self, SOUND_634, 0x61, NULL, NULL, 0, NULL);
                } else if ((objData->carryFlags == Basket_STOP_CARRYING) && (self->unkE0 == 0)) {
                    objData->carryFlags = Basket_NOT_CARRIED;
                    objData->throwFlags = Basket_THROWN_No_Items;
                    self->velocity.f[0] = 0.0f;
                    self->velocity.f[1] = 0.0f;
                    self->velocity.f[2] = 0.0f;
                    func_8002674C(self);
                    self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
                    func_80026160(self);
                }
            }
        } else if (objData->throwFlags != Basket_NOT_THROWN) {
            objData->unk1A -= gUpdateRate;
            if (objData->throwFlags == Basket_THROWN) {
                func_80026128(self, 15, 1, 0);
                if (self->velocity.y > -10.0f) {
                    self->velocity.y += -0.12f * gUpdateRateF;
                }
                func_8002674C(self);
            }
            self->srt.transl.x += self->velocity.x * gUpdateRateF;
            self->srt.transl.y += self->velocity.y * gUpdateRateF;
            self->srt.transl.z += self->velocity.z * gUpdateRateF;
            smallbasket_handle_collision(self);

            if ((self->objhitInfo->unk9D) && (objData->throwFlags == Basket_THROWN)) {
                //Destroy thrown basket upon collision, creating items
                dModGfxDLL->vtbl->func0(self, 1, NULL, 2, -1, NULL);
                gDLL_6_AMSFX->vtbl->play(self, objData->soundID, MAX_VOLUME, NULL, NULL, 0, NULL);
                objData->unk10 = 50;
                objData->throwFlags = Basket_NOT_THROWN;
                self->unkAF |= ARROW_FLAG_8_No_Targetting;
                smallbasket_create_items(self, player, objData);
                self->velocity.x = 0.0f;
                self->velocity.z = 0.0f;
                func_80026160(self);
            } else if ((self->objhitInfo->unk9D) && (objData->throwFlags == Basket_THROWN_No_Items)) {
                //Destroy thrown basket without creating items
                self->velocity.x = 0.0f;
                self->velocity.z = 0.0f;
                objData->unk10 = 500;
                objData->throwFlags = Basket_NOT_THROWN;
                self->unkE0 = 0;
                func_8002674C(self);
                self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
                func_80026160(self);
            }
        }
        
        objData->shakeSoundTimer -= gUpdateRate;

        if (objData->carryFlags) {
            //Automatically throw the basket when far from its origin
            if (SQ(objData->autoThrowRadius) <= vec3_distance_xz_squared(&self->globalPosition, (Vec3f*) &objSetup->base.x)) {
                self->velocity.x = 0.0f;
                self->velocity.z = 0.0f;
                objData->unk10 = 500;
                objData->throwFlags = 0;
                self->unkE0 = 0;
                func_8002674C(self);
                self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
                func_80026160(self);
            }
        } else {
            smallbasket_handle_attack_collisions(self, player, objData);
        }
        
        if ((objData->shakeSoundTimer <= 0) && objData->carryFlags) {
            //Play food rattling sounds
            if (objData->storedItemType == SmallBasket_ITEM_Energy_Egg || 
                objData->storedItemType == SmallBasket_ITEM_Apple
            ){
                gDLL_6_AMSFX->vtbl->play(NULL, SOUND_64D_Wooden_Rattle, 0x39, NULL, NULL, 0, NULL);
                objData->shakeSoundTimer = rand_next(0, 100) + 300;

            //Play Scarab rattling sounds
            } else if (objData->storedItemType == SmallBasket_ITEM_Scarab_Green   || 
                       objData->storedItemType == SmallBasket_ITEM_Scarab_Red     || 
                       objData->storedItemType == SmallBasket_ITEM_Scarab_Gold    || 
                       objData->storedItemType == SmallBasket_ITEM_Scarab_Rainbow
            ) {
                gDLL_6_AMSFX->vtbl->play(NULL, SOUND_64C_Wicker_Rattle, 0x39, NULL, NULL, 0, NULL);
                objData->shakeSoundTimer = rand_next(0, 100) + 300;
            }
            
            //Play Scorpion hiss sounds (neighbouring soundIDs picked at random)
            if (objData->storedItemType == SmallBasket_ITEM_Scorpion) {
                gDLL_6_AMSFX->vtbl->play(NULL, rand_next(0, 2) + SOUND_6B7_Crate_Hiss, 0x39, NULL, NULL, 0, NULL);
                objData->shakeSoundTimer = rand_next(0, 100) + 300;
            }
        }
    }
}

// offset: 0xC0C | func: 2 | export: 2
void smallbasket_update(Object *self) { }

// offset: 0xC18 | func: 3 | export: 3
void smallbasket_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    SmallBasket_Data* objData = self->data;
    SmallBasket_Setup *setup = (SmallBasket_Setup*)self->setup;
    
    if (gDLL_29_Gplay->vtbl->did_time_expire(setup->base.uID) == 0) {
        return;
    }

    if (!(objData->unk10 == 0 || objData->unk10 > 50) || objData->unk0 != 0) {
        return;
    }
    
    if (self->unkE0 != 0) {
        if (visibility != -1) {
            return;
        }
    } else if (!visibility){
        return;
    }

    draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
}

// offset: 0xD04 | func: 4 | export: 4
void smallbasket_free(Object* self, s32 arg1) {
    SmallBasket_Data* objdata = self->data;

    gDLL_14_Modgfx->vtbl->func5(self);
    dll_unload(dModGfxDLL);

    if (objdata->soundHandle) {
        gDLL_6_AMSFX->vtbl->stop(objdata->soundHandle);
        objdata->soundHandle = 0;
    }
}

// offset: 0xD9C | func: 5 | export: 5
u32 smallbasket_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0xDAC | func: 6 | export: 6
u32 smallbasket_get_data_size(Object *self, u32 a1) {
    return sizeof(SmallBasket_Data);
}

// offset: 0xDC0 | func: 7
s32 smallbasket_handle_being_lifted(Object* self, Object* player, SmallBasket_Data* objData) {
    s32 outVal;

    gDLL_2_Camera->vtbl->get_target_object();
    outVal = 0;
    if ((self->unkAF & ARROW_FLAG_1_Interacted) && (self->unkE0 == 0)) {
        joy_set_button_mask(0, A_BUTTON);
        objData->playerMessageLo = -0x8000;
        objData->playerMessageHi = 0;
        func_800267A4(self);
        outVal = 1;
    }
    return outVal;
}

// offset: 0xE78 | func: 8
/** Handles terrain collision, and returns TRUE when the basket collides with the ground. */
int smallbasket_handle_collision(Object* self) {
    Vec3f spF8[4];
    Vec3f spC8[4];
    Unk80027934 sp5C;
    AABBs32 aabb;
    ObjectHitInfo* objHits;
    f32* pad;
    u8 i;
    u32 temp2;
    f32* temp;

    objHits = self->objhitInfo;

    temp2 = objHits->unk40;
    if ((temp2 >> 4) && (objHits->unk61 == 0)) {
        spF8[0].x = self->srt.transl.x;
        spF8[0].y = self->srt.transl.y;
        spF8[0].z = self->srt.transl.z;
        spC8[0].x = self->prevLocalPosition.x;
        spC8[0].y = self->prevLocalPosition.y;
        spC8[0].z = self->prevLocalPosition.z;
        sp5C.unk40[0] = objHits->unk52;
        sp5C.unk50[0] = -1;
        sp5C.unk54[0] = 3;
    } else {
        return FALSE;
    }
        
    fit_aabb_around_cubes(&aabb, (Vec3f*)&spC8, (Vec3f*)&spF8, &sp5C.unk40[0], 1);
    func_80053750(self, &aabb, objHits->unkA1);
    i = func_8005509C(self, (f32*)&spC8, (f32*)&spF8, 1, &sp5C, 0);
    
    if (i) {
        if (i & 1) {
            i = 0;
        } else if (i & 2) {
            i = 1;
        } else if (i & 4) {
            i = 2;
        } else {
            i = 3;
        }
        
        objHits->unk9C = sp5C.unk50[i];       
        temp = (f32*)spF8;            
        objHits->unk34 = temp[i * 3 + 0];
        objHits->unk38 = temp[i * 3 + 1];
        objHits->unk3C = temp[i * 3 + 2];
        
        sVec.x = sp5C.unk0[i].x;
        sVec.y = sp5C.unk0[i].y;
        sVec.z = sp5C.unk0[i].z;
        sVec.w = sp5C.unk0[i].w;
        
        if (sp5C.unk58[i] != 0) {
            objHits->unk9D |= 2;
            self->srt.transl.x = objHits->unk34;
            self->srt.transl.y = objHits->unk38;
            self->srt.transl.z = objHits->unk3C;
            objHits->unk10.f[0] = self->prevLocalPosition.x;
            objHits->unk10.f[1] = self->prevLocalPosition.y;
            objHits->unk10.f[2] = self->prevLocalPosition.z;
            self->velocity.x = 0.0f;
            self->velocity.y = 0.0f;
            self->velocity.z = 0.0f;
            return TRUE;
        } else {
            objHits->unk9D |= 1;
            self->srt.transl.x = objHits->unk34;
            self->srt.transl.y = objHits->unk38;
            self->srt.transl.z = objHits->unk3C;
            objHits->unk10.f[0] = self->prevLocalPosition.x;
            objHits->unk10.f[1] = self->prevLocalPosition.y;
            objHits->unk10.f[2] = self->prevLocalPosition.z;
            self->velocity.x = 0.0f;
            self->velocity.y = 0.0f;
            self->velocity.z = 0.0f;
            return TRUE;
        }
    } else {
        return FALSE;
    }
}

// offset: 0x1104 | func: 9
s32 smallbasket_create_items(Object* self, Object* player, SmallBasket_Data* objData) {
    Scorpion_Setup* scorpionSetup;
    Scarab_Setup* scarabSetup;
    Collectable_Setup* foodSetup;
    SRT srt;
    s32 count;
    f32 magnitude;
    Object* obj;
    f32 health;
    f32 healthMax;
    s32 onGround;
    s32 yaw;
    s16 type;

    onGround = (sVec.y < /*0*/.25f);
    
    //Get the type/quantity of items to create
    if (objData->storedItemType == SmallBasket_ITEM_Prioritise_Health) {
        health = ((DLL_210_Player*)player->dll)->vtbl->get_health(player);
        healthMax = ((DLL_210_Player*)player->dll)->vtbl->get_health_max(player);
        magnitude = (health / healthMax) * 100.0f; //health as percent

        //If player is under 50% health, create a random amount of apples (potentially enough to fully restore health)
        if (magnitude <= 50.0f) {
            type = SmallBasket_ITEM_Apple;
            
            count = (s16) (healthMax * 0.25f); //NOTE: could create tons of apples at high max health
            if (count < 1) {
                count = 1;
            }
            count = rand_next(1, count);

        //If player health is under 75%, create either apples or green Scarabs
        } else if (magnitude <= 75.0f) {
            //Chance of creating apples: 28% when at 75% health, or up to 100% chance approaching 56% health?
            if (rand_next(0, (s16) (magnitude - 50.0f)) < 7) { 
                type = SmallBasket_ITEM_Apple;

                count = (s16) (healthMax * 0.25f); //NOTE: could create tons of apples at high max health
                if (count < 1){
                    count = 1;
                }
                count = rand_next(1, count);

            //Create Scarabs instead of apples (more likely as health approaches 75%)
            } else {
                type = SmallBasket_ITEM_Scarab_Green;
                count = rand_next(1, 4);
            }

        //Don't create anything if player's health is above 75%
        } else {
            return TRUE;
        }
    } else {
        count = objData->storedItemCount;
        type = objData->storedItemType;
    }

    while (count){
        count--;
        
        switch (type) {
        case SmallBasket_ITEM_Scorpion:
            scorpionSetup = (Scorpion_Setup*)obj_alloc_setup(sizeof(Scorpion_Setup), OBJ_Scorpion);
            scorpionSetup->unk18 = rand_next(-0x7F, 0x7E);
            scorpionSetup->base.x = rand_next(-10, 10) + self->srt.transl.x;
            scorpionSetup->base.y = self->srt.transl.y;
            scorpionSetup->base.z = rand_next(-10, 10) + self->srt.transl.z;
            scorpionSetup->unk1A = 49;
            scorpionSetup->unk19 = 4;
            obj_create((ObjSetup*)scorpionSetup, 5, self->mapID, -1, self->parent);
            break;
        case SmallBasket_ITEM_Scarab_Green:
            scarabSetup = (Scarab_Setup*)obj_alloc_setup(sizeof(Scarab_Setup), OBJ_Green_scarab);
            scarabSetup->base.x = self->srt.transl.x;
            scarabSetup->base.y = self->srt.transl.y;
            scarabSetup->base.z = self->srt.transl.z;
            scarabSetup->lifetime = 400;
            obj = obj_create((ObjSetup*)scarabSetup, 5, self->mapID, -1, self->parent);
            
            if (onGround) {
                obj->velocity.f[0] = sVec.x * 3;
                obj->velocity.f[1] = sVec.y * 4;
                obj->velocity.f[2] = sVec.z * 3;
            } else {
                obj->velocity.f[0] = self->srt.transl.x - player->srt.transl.x;
                obj->velocity.f[2] = self->srt.transl.z - player->srt.transl.z;
            }
            
            magnitude = SQ(obj->velocity.f[2]) + SQ(obj->velocity.f[0]);
            if (magnitude != 0.0f) {
                magnitude = sqrtf(magnitude);
                obj->velocity.f[0] /= magnitude;
                obj->velocity.f[2] /= magnitude;
            }
            
            obj->velocity.f[0] *= 1.0f - (0.01f * rand_next(0, 25));
            obj->velocity.f[2] *= 1.0f - (0.01f * rand_next(0, 25));
            obj->velocity.f[1] = 2.2f;
            
            srt.transl.x = 0;
            srt.transl.y = 0;
            srt.transl.z = 0;
            srt.scale = 1.0f;
            srt.roll = 0;
            srt.pitch = 0;
            srt.yaw = rand_next(-0x2710, 0x2710);
            rotate_vec3((SRT* ) &srt, obj->velocity.f);
            yaw = arctan2_f(obj->velocity.x, -obj->velocity.z) << 0x10 >> 0x10 ;
            yaw = obj->srt.yaw - (yaw & 0xFFFF);
            CIRCLE_WRAP(yaw)
            obj->srt.yaw = yaw;
            break;
        case SmallBasket_ITEM_Scarab_Red:
            scarabSetup = (Scarab_Setup*)obj_alloc_setup(sizeof(Scarab_Setup), OBJ_Red_scarab);
            scarabSetup->unk18 = rand_next(-0x7F, 0x7E);
            scarabSetup->base.x = self->srt.transl.x;
            scarabSetup->base.y = self->srt.transl.y;
            scarabSetup->base.z = self->srt.transl.z;
            scarabSetup->lifetime = 400;
            obj = obj_create((ObjSetup*)scarabSetup, 5, self->mapID, -1, self->parent);
            if (onGround != 0) {
                obj->velocity.f[0] = sVec.x * 3;
                obj->velocity.f[1] = sVec.y * 4;
                obj->velocity.f[2] = sVec.z * 3;
            } else {
                obj->velocity.f[0] = self->srt.transl.x - player->srt.transl.x;
                obj->velocity.f[2] = self->srt.transl.z - player->srt.transl.z;
            }
            
            magnitude = SQ(obj->velocity.f[2]) + SQ(obj->velocity.f[0]);
            if (magnitude != 0.0f) {
                magnitude = sqrtf(magnitude);
                obj->velocity.f[0] /= magnitude;
                obj->velocity.f[2] /= magnitude;
            }
            
            obj->velocity.f[0] *= 1.0f - (0.01f * rand_next(0, 25));
            obj->velocity.f[2] *= 1.0f - (0.01f * rand_next(0, 25));
            obj->velocity.f[1] = 2.2f;
            
            srt.transl.x = 0;
            srt.transl.y = 0;
            srt.transl.z = 0;
            srt.scale = 1.0f;
            srt.roll = 0;
            srt.pitch = 0;
            srt.yaw = rand_next(-0x2710, 0x2710);
            rotate_vec3((SRT* ) &srt, obj->velocity.f);
            yaw = arctan2_f(obj->velocity.x, -obj->velocity.z) << 0x10 >> 0x10 ;
            yaw = obj->srt.yaw - (yaw & 0xFFFF);
            CIRCLE_WRAP(yaw)
            obj->srt.yaw = yaw;
            break;
        case SmallBasket_ITEM_Scarab_Gold:
            scarabSetup = (Scarab_Setup*)obj_alloc_setup(sizeof(Scarab_Setup), OBJ_Gold_scarab);
            scarabSetup->unk18 = rand_next(-0x7F, 0x7E);
            scarabSetup->base.x = self->srt.transl.x;
            scarabSetup->base.y = self->srt.transl.y;
            scarabSetup->base.z = self->srt.transl.z;
            scarabSetup->lifetime = 2000;
            obj = obj_create((ObjSetup*)scarabSetup, 5, self->mapID, -1, self->parent);

            if (onGround != 0) {
                obj->velocity.f[0] = sVec.x * 3;
                obj->velocity.f[1] = sVec.y * 4;
                obj->velocity.f[2] = sVec.z * 3;
            } else {
                obj->velocity.f[0] = self->srt.transl.x - player->srt.transl.x;
                obj->velocity.f[2] = self->srt.transl.z - player->srt.transl.z;
            }
            
            magnitude = SQ(obj->velocity.f[2]) + SQ(obj->velocity.f[0]);
            if (magnitude != 0.0f) {
                magnitude = sqrtf(magnitude);
                obj->velocity.f[0] /= magnitude;
                obj->velocity.f[2] /= magnitude;
            }
            
            obj->velocity.f[0] *= 1.0f - (0.01f * rand_next(0, 25));
            obj->velocity.f[2] *= 1.0f - (0.01f * rand_next(0, 25));
            obj->velocity.f[1] = 2.2f;
            
            srt.transl.x = 0;
            srt.transl.y = 0;
            srt.transl.z = 0;
            srt.scale = 1.0f;
            srt.roll = 0;
            srt.pitch = 0;
            srt.yaw = rand_next(-0x2710, 0x2710);
            rotate_vec3((SRT* ) &srt, obj->velocity.f);
            yaw = arctan2_f(obj->velocity.x, -obj->velocity.z) << 0x10 >> 0x10 ;
            yaw = obj->srt.yaw - (yaw & 0xFFFF);
            CIRCLE_WRAP(yaw)
            obj->srt.yaw = yaw;
            break;
        case SmallBasket_ITEM_Scarab_Rainbow:
            scarabSetup = (Scarab_Setup*)obj_alloc_setup(sizeof(Scarab_Setup), OBJ_Rain_scarab);
            scarabSetup->unk18 = rand_next(-0x7F, 0x7E);
            scarabSetup->base.x = self->srt.transl.x;
            scarabSetup->base.y = self->srt.transl.y;
            scarabSetup->base.z = self->srt.transl.z;
            scarabSetup->lifetime = 800;
            obj = obj_create((ObjSetup*)scarabSetup, 5, self->mapID, -1, self->parent);
            
            if (onGround) {
                obj->velocity.f[0] = sVec.x * 3;
                obj->velocity.f[1] = sVec.y * 4;
                obj->velocity.f[2] = sVec.z * 3;
            } else {
                obj->velocity.f[0] = self->srt.transl.x - player->srt.transl.x;
                obj->velocity.f[2] = self->srt.transl.z - player->srt.transl.z;
            }
            
            magnitude = SQ(obj->velocity.f[2]) + SQ(obj->velocity.f[0]);
            if (magnitude != 0.0f) {
                magnitude = sqrtf(magnitude);
                obj->velocity.f[0] /= magnitude;
                obj->velocity.f[2] /= magnitude;
            }
            
            obj->velocity.f[0] *= 1.0f - (0.01f * rand_next(0, 25));
            obj->velocity.f[2] *= 1.0f - (0.01f * rand_next(0, 25));
            obj->velocity.f[1] = 2.2f;
            
            srt.transl.x = 0;
            srt.transl.y = 0;
            srt.transl.z = 0;
            srt.scale = 1.0f;
            srt.roll = 0;
            srt.pitch = 0;
            srt.yaw = rand_next(-0x2710, 0x2710);
            rotate_vec3((SRT* ) &srt, obj->velocity.f);
            yaw = arctan2_f(obj->velocity.x, -obj->velocity.z) << 0x10 >> 0x10 ;
            yaw = obj->srt.yaw - (yaw & 0xFFFF);
            CIRCLE_WRAP(yaw)
            obj->srt.yaw = yaw;
            break;
        case SmallBasket_ITEM_Energy_Egg:
        case SmallBasket_ITEM_Apple:
            if (objData->storedItemType == SmallBasket_ITEM_Energy_Egg) {
                foodSetup = (Collectable_Setup*)obj_alloc_setup(sizeof(Collectable_Setup), OBJ_meatPickup);
            } else {
                foodSetup = (Collectable_Setup*)obj_alloc_setup(sizeof(Collectable_Setup), OBJ_applePickup);
            }
            foodSetup->objHitsValue = 20;
            foodSetup->gamebitCount = NO_GAMEBIT;
            foodSetup->gamebitCollected = NO_GAMEBIT;
            if (objData->throwFlags != 0) {
                foodSetup->base.x = rand_next(-15, 15) + self->srt.transl.x;
                foodSetup->base.y = self->srt.transl.y + 15.0f;
                foodSetup->base.z = rand_next(-15, 15) + self->srt.transl.z;
            } else {
                foodSetup->base.x = self->srt.transl.x;
                foodSetup->base.y = self->srt.transl.y + 5.0f;
                foodSetup->base.z = self->srt.transl.z;
            }
            foodSetup->gamebitSecondary = NO_GAMEBIT;
            
            obj = obj_create((ObjSetup*)foodSetup, 5, self->mapID, -1, self->parent);
            if (onGround != 0) {
                obj->velocity.f[0] = sVec.x * 3;
                obj->velocity.f[1] = sVec.y * 4;
                obj->velocity.f[2] = sVec.z * 3;
            }
            
            magnitude = SQ(obj->velocity.f[2]) + SQ(obj->velocity.f[0]);
            if (magnitude != 0.0f) {
                magnitude = 2.0f * sqrtf(magnitude);
                obj->velocity.f[0] /= magnitude;
                obj->velocity.f[2] /= magnitude;
            }
            
            obj->velocity.f[0] *= 1.0f - (0.01f * rand_next(0, 25));
            obj->velocity.f[2] *= 1.0f - (0.01f * rand_next(0, 25));
            obj->velocity.f[1] = 2.2f;
            
            srt.transl.x = 0;
            srt.transl.y = 0;
            srt.transl.z = 0;
            srt.scale = 1.0f;
            srt.roll = 0;
            srt.pitch = 0;
            srt.yaw = rand_next(-0x2710, 0x2710);
            rotate_vec3((SRT*) &srt, obj->velocity.f);

            yaw = arctan2_f(obj->velocity.x, -obj->velocity.z) << 0x10 >> 0x10 ;
            yaw = obj->srt.yaw - (yaw & 0xFFFF);
            CIRCLE_WRAP(yaw)
            obj->srt.yaw = yaw;
            break;
        }
    }
    
    return TRUE;
}

// offset: 0x2024 | func: 10
void smallbasket_handle_attack_collisions(Object* self, Object* player, SmallBasket_Data* objData) {
    //Check for any kind of attack collision
    if (func_80025F40(self, NULL, NULL, NULL) != 0) {
        if (objData->soundHandle != 0) {
            gDLL_6_AMSFX->vtbl->stop(objData->soundHandle);
            objData->soundHandle = 0;
        } 

        dModGfxDLL->vtbl->func0(self, 1, NULL, 2, -1, NULL);
        gDLL_6_AMSFX->vtbl->play(self, objData->soundID, MAX_VOLUME, NULL, NULL, 0, NULL);
        objData->unk10 = 50;
        objData->throwFlags = 0;
        smallbasket_create_items(self, player, objData);
        self->velocity.x = 0.0f;
        self->velocity.z = 0.0f;
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        func_80026160(self);
    }
}
