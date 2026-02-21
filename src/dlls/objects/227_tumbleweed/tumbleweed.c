/* 
    OBJ_Tumbleweed1 used for SwapStone Circle, and the tree near the sand arches in Golden Plains (chases player)
    OBJ_Tumbleweed2 used for FrostWeeds (flees from player)
    OBJ_Tumbleweed3 used for the tree near the Chimney Swipe bonfire in Golden Plains (flees from player)

    OBJ_Tumbleweed1twig etc. refer to the separate object left behind after Tumbleweed is attacked
*/

#include "PR/gbi.h"
#include "PR/ultratypes.h"
#include "dll.h"
#include "dlls/engine/6_amsfx.h"
#include "functions.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object_id.h"
#include "sys/gfx/model.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "sys/rand.h"
#include "sys/joypad.h"
#include "game/objects/object.h"
#include "types.h"

#include "dlls/objects/211_tricky.h"
#include "dlls/objects/227_tumbleweed.h"

void func_8002674C(Object* obj);

/*0x0*/ static Vec3f dHitsTestPoint = {0.0, 0.0, 0.0};
/*0xC*/ static f32 dHitsTestRadius = 25.0;

#define SPEED_CAP_X 1.0f
#define SPEED_CAP_Z 1.0f
#define SPEED_CAP_Y_CHASE 1.0f
#define SPEED_CAP_Y_FLEE 1.5f

static void Tumbleweed_tick_handle_flags(Object* self);
static void Tumbleweed_tick_chase_player(Object* self);
static void Tumbleweed_tick_flee_from_player(Object* self);
static void Tumbleweed_bounce_and_roll(Object* self, Tumbleweed_Data* objData);
static int Tumbleweed_handle_carry_behaviour(Object* self);
static int Tumbleweed_did_player_lift_twig(Object* self);
static void Tumbleweed_create_twigs(Object* self);

// offset: 0x0 | ctor
void Tumbleweed_ctor(void* dll) { }

// offset: 0xC | dtor
void Tumbleweed_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void Tumbleweed_setup(Object* self, Tumbleweed_Setup* setup, GoldenNugget_Setup* arg2) { // arg2 fakematch, not actually a golden nugget setup pointer
    Tumbleweed_Data* objData;
    GoldenNugget_Setup* goldSetup;
    Object** objects;
    s32 index;
    s32 count;

    objData = self->data;
    
    objData->homeX = self->srt.transl.x;
    objData->homeZ = self->srt.transl.z;
    objData->interactRadius = 2.0f * setup->interactRadius;
    objData->carryingGold = setup->carryingGold;
    objData->flags = Tumbleweed_FLAG_None;
    objData->baseScale = self->srt.scale;
    objData->timer = objData->baseScale / rand_next(200, 500);
    objData->player = 0;
    
    self->speed.x = 0.0f;
    self->speed.y = 0.0f;
    self->speed.z = 0.0f;
     
    if (self->shadow) {
        self->shadow->flags |= OBJ_SHADOW_FLAG_TOP_DOWN | OBJ_SHADOW_FLAG_CUSTOM_DIR;
    }
    
    objData->carryMessageArgHi = 50;
    if (self->id == OBJ_Tumbleweed1twig || self->id == OBJ_Tumbleweed2twig || self->id == OBJ_Tumbleweed3twig) {
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        objData->state = Tumbleweed_STATE_Twigs;
        objData->timer = 480.0f;
        self->srt.scale = objData->baseScale;
    } else {
        self->srt.scale = 0.001f;
        gDLL_27->vtbl->init(&objData->collision, DLL27FLAG_NONE, DLL27FLAG_40000, DLL27MODE_1);
        gDLL_27->vtbl->setup_hits_collider(&objData->collision, 1, &dHitsTestPoint, &dHitsTestRadius, 4);
        gDLL_27->vtbl->reset(self, &objData->collision);
        objData->state = Tumbleweed_STATE_Growing_on_Tree;
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        
        if (objData->carryingGold & 1) {
            for (objects = get_world_objects(&index, &count); index < count; index++) {
                if (objects[index]->id == OBJ_SC_golden_nugge) {
                    
                    objData->goldenNugget = objects[index];
                    goldSetup = (GoldenNugget_Setup*)objData->goldenNugget->setup;
                    objData->goldDroppedGamebit = goldSetup->gamebitDropped;

                    if (main_get_bits(objData->goldDroppedGamebit)) {
                        objData->goldenNugget = NULL;
                    }

                    //Index break
                    index = count;
                }
            }
        }
    }
    
    obj_add_object_type(self, OBJTYPE_4);
    obj_add_object_type(self, OBJTYPE_51);
    func_800267A4(self);
}

// offset: 0x354 | func: 1 | export: 1
void Tumbleweed_control(Object* self) {
    Tumbleweed_Data* objData;
    Object* shiny;

    objData = self->data;
    if (self->id == OBJ_Tumbleweed1 || self->id == OBJ_Tumbleweed1twig) {
        Tumbleweed_tick_chase_player(self);
    } else {
        Tumbleweed_tick_flee_from_player(self);
    }
    Tumbleweed_tick_handle_flags(self);
    
    shiny = objData->goldenNugget;
    if (shiny) {
        //TODO: update with SCcollectables DLL interface
        ((DLL_Unknown*)shiny->dll)->vtbl->func[8].withTwoArgs((s32)shiny, (s32)self); 
    }
}

// offset: 0x420 | func: 2
void Tumbleweed_tick_handle_flags(Object* self) {
    Tumbleweed_Data *objData;
    s32 i;
    
    objData = self->data;
    
    if (objData->flags & Tumbleweed_FLAG_Create_Leaves) {
        switch (self->id) {
        case OBJ_Tumbleweed1:
        case OBJ_Tumbleweed1twig:
        case OBJ_Tumbleweed3:
        case OBJ_Tumbleweed3twig:
            //Create leaf particles
            for (i = 20; i > 0; i--) {
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_34D, NULL, PARTFXFLAG_2, -1, NULL);
            }
            break;
        default:
            //Create frosty leaf particles
            for (i = 20; i > 0; i--) {
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_32E, NULL, PARTFXFLAG_2, -1, NULL);
            }
            break;
        }
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_5F7_Tumbleweed_Disintegrate, MAX_VOLUME, 0, 0, 0, 0);
    }
    
    if (objData->flags & Tumbleweed_FLAG_Create_Dust) {
        switch (self->id) {
        case OBJ_Tumbleweed1:
        case OBJ_Tumbleweed1twig:
        case OBJ_Tumbleweed3:
        case OBJ_Tumbleweed3twig:
            //Create cloud of dust
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_34C, NULL, PARTFXFLAG_2, -1, NULL);
            break;
        default:
            //Create cloud of frost
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_32D, NULL, PARTFXFLAG_2, -1, NULL);
        }
    }
    
    if (objData->flags & Tumbleweed_FLAG_Expire) {
        //Hide Tumbleweed and set two second object deletion timer
        self->opacity = 0;
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        objData->state = Tumbleweed_STATE_Expire;
        objData->timer = 120.0f;
    }
    
    objData->flags = Tumbleweed_FLAG_None;
}

// offset: 0x658 | func: 3
void Tumbleweed_tick_chase_player(Object* self) {
    Tumbleweed_Data* objData;
    s32 hitSphereID;
    s32 hitDamage;
    Object* player;
    Object* hitBy;
    f32 dx;
    f32 dz;
    f32 dayTime;

    objData = self->data;

    if (objData->state == Tumbleweed_STATE_Growing_on_Tree) {
        //Do nothing during the day
        if (!gDLL_7_Newday->vtbl->func8(&dayTime)) {
            return;
        }
        
        //Start growing on tree at nighttime
        if (self->srt.scale < objData->baseScale) {
            self->srt.scale += objData->timer * gUpdateRateF;
            return;
        }
        objData->state = Tumbleweed_STATE_Fall_from_Tree;

    } else if (objData->state == Tumbleweed_STATE_Fall_from_Tree) {
        //Do nothing during the day
        if (!gDLL_7_Newday->vtbl->func8(&dayTime)) {
            return;
        }

        //At nighttime, drop from tree when player approaches
        if (objData->player != NULL) {
            player = objData->player;
        } else {
            player = get_player();
        }
        
        dx = self->srt.transl.x - player->srt.transl.x;
        dz = self->srt.transl.z - player->srt.transl.z;
        objData->characterDistance = sqrtf(SQ(dx) + SQ(dz));

        if (objData->characterDistance < objData->interactRadius) {
            objData->state = Tumbleweed_STATE_Rolling;
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
            func_8002674C(self);
        }

    } else if (objData->state == Tumbleweed_STATE_Rolling) {
        //Gravitate towards the player
        if (objData->player != NULL) {
            player = objData->player;
        } else {
            player = get_player();
        }
        
        dx = self->srt.transl.x - player->srt.transl.x;
        dz = self->srt.transl.z - player->srt.transl.z;       
        objData->characterDistance = sqrtf(SQ(dx) + SQ(dz));
        
        if (objData->characterDistance > 20.0f) {
            self->speed.x -= dx / (objData->characterDistance * 20.0f);
            self->speed.z -= dz / (objData->characterDistance * 20.0f);
            objData->rollSpeed = self->speed.x * 728.0f;
            objData->pitchSpeed = self->speed.z * 728.0f;
        } else {
            self->speed.x = 0.0f - (self->speed.x * 0.8f);
            self->speed.z = 0.0f - (self->speed.z * 0.8f);
        }
        
        //Move and apply collision
        Tumbleweed_bounce_and_roll(self, objData);
        gDLL_27->vtbl->func_624(self, &objData->collision, gUpdateRateF);

        //Check for attack collisions
        if (func_80025F40(self, &hitBy, &hitSphereID, &hitDamage)) {
            main_set_bits(BIT_Damaged_a_Tumbleweed, 1);
            objData->flags |= Tumbleweed_FLAG_Create_Leaves | Tumbleweed_FLAG_Create_Dust | Tumbleweed_FLAG_Expire;

            //Drop the Gold Nugget if it's being carried
            if (objData->goldenNugget != NULL) {
                main_set_bits(objData->goldDroppedGamebit, 1);
                objData->goldenNugget = NULL;
                return;
            }

            Tumbleweed_create_twigs(self);
        }

    } else if (objData->state == Tumbleweed_STATE_Twigs) {
        //Return early if being carried
        if (Tumbleweed_handle_carry_behaviour(self) == FALSE) {
            return;
        }

        //Check for attack collisions
        if (func_80025F40(self, &hitBy, &hitSphereID, &hitDamage)) {
            objData->timer = 0.0f;
        }

        //Handle expiry
        if (objData->timer <= 0.0f) {
            objData->flags |= Tumbleweed_FLAG_Create_Leaves | Tumbleweed_FLAG_Create_Dust | Tumbleweed_FLAG_Expire;
        } else {
            objData->timer -= gUpdateRateF;
        }

        //Stop, drop (speed.y), and roll (or use the terrain part of the motion function at least)
        self->speed.x = 0.0f;
        self->speed.y /= 1.1f; //@frame-rate dependent
        self->speed.z = 0.0f;
        Tumbleweed_bounce_and_roll(self, objData);

    } else {
        //Destroy self when expiry timer runs out
        if (objData->timer <= 0.0f) {
            obj_destroy_object(self);
            return;
        }
        objData->timer -= gUpdateRateF;
    }
}

// offset: 0xC04 | func: 4
/** 
  * Used by the FrostWeeds (OBJ_Tumbleweed2) and the Tumbleweeds near the Chimney Swipe (OBJ_Tumbleweed3).
  *
  * These Tumbleweeds are active day or night!
  */
void Tumbleweed_tick_flee_from_player(Object* self) {
    Tumbleweed_Data* objData;
    s32 hitSphereID;
    s32 hitDamage;
    u16 targetDistance;
    Object* hitBy;
    f32 sidekickDistance;
    Object* sidekick;
    f32 dx;
    f32 dz;
    f32 dxTarget;
    f32 dzTarget;
    f32 distance;
    f32 dx2;
    f32 dz2;
    Object* player;
    u16 i;

    objData = self->data;
    
    //Start growing on tree
    if (objData->state == Tumbleweed_STATE_Growing_on_Tree) {
        if (self->srt.scale < objData->baseScale) {
            self->srt.scale += objData->timer * gUpdateRateF;
            return;
        }
        objData->state = Tumbleweed_STATE_Fall_from_Tree;

    //Ready to fall from tree
    } else if (objData->state == Tumbleweed_STATE_Fall_from_Tree) {
        //Fall when struck by attack collision
        if (func_80025F40(self, &hitBy, &hitSphereID, &hitDamage)) {
            func_8002674C(self);
            objData->state = Tumbleweed_STATE_Rolling;
            self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
            objData->flags |= Tumbleweed_FLAG_Create_Leaves | Tumbleweed_FLAG_Create_Dust;
        }
        //NOTE: Tumbleweed's state can also be advanced by the "TumbleweedBush" object's control function

    } else if (objData->state == Tumbleweed_STATE_Rolling) {
        //Get distance to player/sidekick (whoever's closest)
        player = get_player();
        dx = self->srt.transl.x - player->srt.transl.x;
        dz = self->srt.transl.z - player->srt.transl.z;
        distance = SQ(dx) + SQ(dz);
        
        sidekick = get_sidekick();
        if (sidekick && sidekick->id == OBJ_Tricky) {
            //Enable "Find" sidekick command when nearby
            if (distance < 30625.0f) {
                ((DLL_211_Tricky*)sidekick->dll)->vtbl->base.func14(sidekick, 1);
            }
            dx2 = self->srt.transl.x - sidekick->srt.transl.x;
            dz2 = self->srt.transl.z - sidekick->srt.transl.z;
            sidekickDistance = SQ(dx2) + SQ(dz2);

            //If the tumbleweed is closer to Tricky than the player, focus on Tricky
            if (sidekickDistance < distance) {
                dx = dx2;
                dz = dz2;
                distance = sidekickDistance;
            }
        }
        objData->characterDistance = sqrtf(distance);

        //Get distance between self and home (or the player, if stored in objData via an export)
        player = objData->player;
        if (player != NULL) {
            dxTarget = self->srt.transl.x - objData->player->srt.transl.x;
            dzTarget = self->srt.transl.z - objData->player->srt.transl.z;
        } else {
            dxTarget = self->srt.transl.x - objData->homeX;
            dzTarget = self->srt.transl.z - objData->homeZ;
        }
        targetDistance = sqrtf(SQ(dxTarget) + SQ(dzTarget));

        if (objData->player != NULL) {
            //Aim towards player if stored in objData
            self->speed.x -= dxTarget / (targetDistance * 10.0f);
            self->speed.z -= dzTarget / (targetDistance * 10.0f);
        } else if ((objData->characterDistance < 150.0f) && (objData->characterDistance > 0)) {
            //Aim to stay a fixed distance away from player/sidekick
            self->speed.x -= dx / ((objData->characterDistance - 150.0f) * 15.0f);
            self->speed.z -= dz / ((objData->characterDistance - 150.0f) * 15.0f);
            objData->rollSpeed = self->speed.x * 728.0f;
            objData->pitchSpeed = self->speed.z * 728.0f;
        } else if ((targetDistance > 10.0f) && (targetDistance > 0)) {
            //Aim towards home (tree)
            self->speed.x -= dxTarget / (targetDistance * 10.0f);
            self->speed.z -= dzTarget / (targetDistance * 10.0f);
        }
        
        //Move and apply collision
        Tumbleweed_bounce_and_roll(self, objData);
        gDLL_27->vtbl->func_624(self, &objData->collision, gUpdateRateF);

        //React to attack collisions
        if (func_80025F40(self, &hitBy, &hitSphereID, &hitDamage)) {
            main_set_bits(BIT_Damaged_a_Tumbleweed, 1);
            objData->flags |= Tumbleweed_FLAG_Create_Leaves | Tumbleweed_FLAG_Create_Dust | Tumbleweed_FLAG_Expire;
            
            //Drop the Gold Nugget if it's being carried
            if (objData->goldenNugget != NULL) {
                main_set_bits(objData->goldDroppedGamebit, 1);
                objData->goldenNugget = NULL;
                return;
            }

            Tumbleweed_create_twigs(self);
        }

    } else if (objData->state == Tumbleweed_STATE_Twigs) {
        //Return early if being carried
        if (Tumbleweed_handle_carry_behaviour(self) == FALSE) {
            return;
        }

        //Check for attack collisions
        if (func_80025F40(self, &hitBy, &hitSphereID, &hitDamage)) {
            objData->timer = 0.0f;
        }

        //Handle expiry
        if (objData->timer <= 0.0f) {
            objData->flags |= Tumbleweed_FLAG_Create_Leaves | Tumbleweed_FLAG_Create_Dust | Tumbleweed_FLAG_Expire;
        } else {
            objData->timer -= gUpdateRateF;
        }

        //Stop, drop (speed.y), and roll (or use the terrain part of the motion function at least)
        self->speed.x = 0.0f;
        self->speed.y /= 1.1f; //@frame-rate dependent
        self->speed.z = 0.0f;
        Tumbleweed_bounce_and_roll(self, objData);

    } else if (objData->state == Tumbleweed_STATE_Gravitate) {
        //Gravitate towards a target point (e.g. when being eaten by Garunda Te)
        distance = vec3_distance_xz_squared(&self->srt.transl, objData->gravitateTarget);
        obj_integrate_speed(self, self->speed.x * gUpdateRateF, self->speed.y * gUpdateRateF, self->speed.z * gUpdateRateF);
        
        //Advance state after gravitating beyond the destination point
        if (distance < vec3_distance_xz_squared(&self->srt.transl, objData->gravitateTarget)) {
            self->srt.transl.x += (objData->gravitateTarget->x - self->srt.transl.x) * 0.5f;
            self->srt.transl.y += (objData->gravitateTarget->y - self->srt.transl.y) * 0.5f;
            self->srt.transl.z += (objData->gravitateTarget->z - self->srt.transl.z) * 0.5f;
            objData->state = Tumbleweed_STATE_Eaten;
        }

    } else if (objData->state == Tumbleweed_STATE_Eaten) {
        //Shrink (accounting for framerate)
        for (i = 0; i < (s32)gUpdateRateF; i++) {
            self->srt.scale *= 0.95f;
        }
        
        //Destroy self at end of shrink animation
        if (self->srt.scale < 0.01f) {
            obj_destroy_object(self);
        }
    
        //Attach to destination point
        self->srt.transl.x = objData->gravitateTarget->x;
        self->srt.transl.y = objData->gravitateTarget->y;
        self->srt.transl.z = objData->gravitateTarget->z;

    } else {
        //Destroy self when expiry timer runs out
        if (objData->timer <= 0.0f) {
            obj_destroy_object(self);
            return;
        }
        objData->timer -= gUpdateRateF;
    }
}

// offset: 0x1518 | func: 5 | export: 2
void Tumbleweed_update(Object* self) { }

// offset: 0x1524 | func: 6 | export: 3
void Tumbleweed_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility > 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1578 | func: 7 | export: 4
void Tumbleweed_free(Object* self, s32 arg1) {
    Tumbleweed_Data* objData;
    Object* object;
    s32 i;
    s32 count;
    s32 id;
    Object** objects;

    objData = self->data;
      
    switch (self->id) {
        case OBJ_Tumbleweed1:
        case OBJ_Tumbleweed1twig:
            id = OBJ_TumbleWeedBush1;
            break;
        case OBJ_Tumbleweed2:
        case OBJ_Tumbleweed2twig:
            id = OBJ_TumbleWeedBush2;
            break;
        case OBJ_Tumbleweed3:
        case OBJ_Tumbleweed3twig:
            id = OBJ_TumbleWeedBush3;
            break;
    }
    
    //Find parent tree object
    for (objects = get_world_objects(&i, &count); i < count; i++) {
        object = objects[i];
        if (id == object->id) {
            ((DLL_Unknown*)object->dll)->vtbl->func[7].withTwoArgs((s32)object, (s32)self);
        }
    }
    
    if (objData->goldenNugget) {
        main_set_bits(objData->goldDroppedGamebit, 1);
        objData->goldenNugget = NULL;
    }

    obj_free_object_type(self, OBJTYPE_4);
    obj_free_object_type(self, OBJTYPE_51);
}

// offset: 0x16F8 | func: 8 | export: 5
u32 Tumbleweed_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x1708 | func: 9 | export: 6
u32 Tumbleweed_get_data_size(Object* self, s32 arg1) {
    return sizeof(Tumbleweed_Data);
}

// offset: 0x171C | func: 10 | export: 7
u8 Tumbleweed_get_state(Object* self) {
    Tumbleweed_Data* objData = self->data;
    return objData->state;
}

// offset: 0x172C | func: 11 | export: 8
void Tumbleweed_set_home(Object* self, f32 x, f32 z) {
    Tumbleweed_Data *objData = self->data;
    objData->homeX = x;
    objData->homeZ = z;
}

// offset: 0x174C | func: 12 | export: 9
void Tumbleweed_fall(Object* self) {
    Tumbleweed_Data *objData = self->data;
    if (objData->state == Tumbleweed_STATE_Fall_from_Tree) {
        func_8002674C(self);
        objData->state = Tumbleweed_STATE_Rolling;
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
    }
}

// offset: 0x17C0 | func: 13 | export: 10
/** When being eaten */
void Tumbleweed_gravitate_towards_point(Object* self, Vec3f* point) {
    Tumbleweed_Data *objData = self->data;
    f32 dx;
    f32 dy;
    f32 dz;
    
    dx = point->x - self->srt.transl.x;
    dy = point->y - self->srt.transl.y;
    dz = point->z - self->srt.transl.z;
    
    self->speed.x = dx * 0.02f;
    self->speed.y = dy * 0.02f;
    self->speed.z = dz * 0.02f;
    
    objData->state = Tumbleweed_STATE_Gravitate;
    objData->gravitateTarget = point;
}

// offset: 0x1828 | func: 14 | export: 11
s32 Tumbleweed_is_gravitating(Object* self) {
    Tumbleweed_Data *objData = self->data;
    return objData->state == Tumbleweed_STATE_Gravitate;
}

// offset: 0x1840 | func: 15 | export: 12
/** 
  * Stores a reference to the player in objData.
  * 
  * Causes the shy kind of Tumbleweed to move towards the player instead!
  */
void Tumbleweed_store_player(Object* self, Object* player) {
    Tumbleweed_Data *objData = self->data;
    objData->player = player;
}

// offset: 0x1850 | func: 16
/** Updates Tumbleweed's movement, and handles ground plane collision */
void Tumbleweed_bounce_and_roll(Object* self, Tumbleweed_Data* objData) {
    f32 sampleHeight;
    s32 sampleCount;
    s32 minimumIndex;
    f32 **samples;
    f32 groundY;
    s32 volume;
    s32 i;

    samples = NULL;
    groundY = 10000.0f;
    sampleCount = func_80057F1C(self, self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, &samples, 0, 0);

    //Find minimum height in collision samples
    for (i = 0, minimumIndex = 0; i < sampleCount; i++) {
        sampleHeight = self->srt.transl.y - *samples[i];
        if (sampleHeight < 0.0f) {
            sampleHeight = (sampleHeight * -1.0f) + 10.0f;
        }
        if (sampleHeight < groundY) {
            minimumIndex = i;
            groundY = sampleHeight;
        }
    }

    //Limit x-component of speed
    if (self->speed.x > SPEED_CAP_X) {
        self->speed.x = SPEED_CAP_X;
    } else if (self->speed.x < -SPEED_CAP_X) {
        self->speed.x = -SPEED_CAP_X;
    }
    
    //Limit y-component of speed
    if (self->id == OBJ_Tumbleweed1 || self->id == OBJ_Tumbleweed1twig) {
        if (self->speed.y > SPEED_CAP_Y_CHASE) {
            self->speed.y = SPEED_CAP_Y_CHASE;
        } else if (self->speed.y < -SPEED_CAP_Y_CHASE) {
            self->speed.y = -SPEED_CAP_Y_CHASE;
        }
    } else {
        if (self->speed.y > SPEED_CAP_Y_FLEE) {
            self->speed.y = SPEED_CAP_Y_FLEE;
        }
        else if (self->speed.y < -SPEED_CAP_Y_FLEE) {
            self->speed.y = -SPEED_CAP_Y_FLEE;
        }
    }
    
    //Limit z-component of speed (@bug? handled separately to x-component, instead of via xz vector and overall lateral limit)
    if (self->speed.z > SPEED_CAP_Z) {
        self->speed.z = SPEED_CAP_Z;
    } else if (self->speed.z < -SPEED_CAP_Z) {
        self->speed.z = -SPEED_CAP_Z;
    }
    
    self->srt.transl.x += self->speed.x * gUpdateRateF;
    self->srt.transl.y += self->speed.y * gUpdateRateF;
    self->srt.transl.z += self->speed.z * gUpdateRateF;
    self->srt.roll += objData->rollSpeed * gUpdateRateF;
    self->srt.pitch += objData->pitchSpeed * gUpdateRateF;
    self->srt.yaw += objData->yawSpeed * gUpdateRateF;
    
    //Handle ground plane collisions
    if (samples) {        
        groundY = *samples[minimumIndex] + 7.0f;

        //Apply gravity when tumbleweed above ground
        if (groundY < self->srt.transl.y) {
            self->speed.y = self->speed.y + (-0.17f); //@bug: framerate dependent gravity
            return;
        }
        
        //Bounce when hitting ground
        self->srt.transl.y = groundY;
        if (self->id == OBJ_Tumbleweed2 || self->id == OBJ_Tumbleweed2twig) {
            self->speed.y = 0.0f - (((f32)objData->characterDistance / rand_next(140, 180)) * (self->speed.y * 0.8f));
        } else {
            self->speed.y = 0.0f - (((f32) objData->characterDistance / rand_next(20, 40)) * (self->speed.y * 0.8f));
        }
        
        //Squeak when bouncing
        volume = self->speed.y * 32.0f;
        if (volume > MAX_VOLUME) {
            volume = MAX_VOLUME;
        }
        if (volume > 0x10) {
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_5F6_Tumbleweed_Roll, volume, 0, 0, 0, 0);
            if (rand_next(0, 5) == 0) {
                gDLL_6_AMSFX->vtbl->play_sound(self, rand_next(SOUND_614_Tumbleweed_Squeak_1, SOUND_615_Tumbleweed_Squeak_2), volume, 0, 0, 0, 0);
            }
        }
    }
}

// offset: 0x1D64 | func: 17
/**
  * Handles the player lifting/dropping the Tumbleweed twigs.
  *
  * Returns TRUE when the Tumbleweed is on the ground, and FALSE when the Tumbleweed is being carried.
  */
int Tumbleweed_handle_carry_behaviour(Object* self) {
    u32 pad;
    Object* player;
    u32 messageArg;
    u32 soundVol;
    u32 soundID;
    Tumbleweed_Data* objData;

    objData = self->data;
    player = get_player();

    //Not being carried
    if (objData->carryFlags == Twig_FLAG_None) {
        //Check for player to lift twig via interaction arrow
        if ((objData->carryFlags = Tumbleweed_did_player_lift_twig(self))) {
            objData->beingCarried = TRUE;
        }
        self->unkAF &= ~ARROW_FLAG_8_No_Targetting;
        return TRUE;
    }
    
    //Being carried
    else {
        //Handle squeaking and growing in size temporarily
        objData->twigSqueakTimer -= gUpdateRateF;
        if (objData->twigSqueakTimer < 0.0f) {
            objData->twigSqueakTimer = rand_next(120, 240);
            soundID = rand_next(SOUND_614_Tumbleweed_Squeak_1, SOUND_615_Tumbleweed_Squeak_2);
            soundVol = rand_next(90, 100);
            gDLL_6_AMSFX->vtbl->play_sound(self, soundID, soundVol, 0, 0, 0, 0);
            self->srt.scale = 0.2f;
        } else {
            self->srt.scale = 0.15f;
        }
        
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        objData->timer = 0.0f;

        //Stop being carried when A button pressed (@bug: desync can occur with player FSA)
        if (joy_get_pressed(0) & A_BUTTON) {
            joy_set_button_mask(0, A_BUTTON);
            objData->beingCarried = FALSE;
        }

        //Check if Tumbleweed dropped by player
        if (self->unkE0 == 1) {
            objData->carryFlags = Twig_FLAG_Dropped;
        }
        if ((objData->carryFlags == Twig_FLAG_Dropped) && (self->unkE0 == 0)) {
            objData->carryFlags = Twig_FLAG_None;
            objData->beingCarried = FALSE;
        }

        //Send message to player object while being carried
        if (objData->beingCarried) {
            messageArg = (objData->carryMessageArgHi << 0x10) | (objData->carryMessageArgLo & 0xFFFF);
            obj_send_mesg(player, 0x100008, self, (void*)messageArg);
        }

        return FALSE;
    }
}

// offset: 0x1FA0 | func: 18
int Tumbleweed_did_player_lift_twig(Object* self) {
    Tumbleweed_Data* objData = self->data;
    s32 returnVal;

    objData = self->data;
    returnVal = FALSE;
    if ((self->unkAF & ARROW_FLAG_1_Interacted) && self->unkE0 == 0) {
        objData->carryMessageArgLo = 0;
        joy_set_button_mask(0, A_BUTTON);
        objData->twigSqueakTimer = rand_next(120, 240);
        returnVal = TRUE;
    }
    return returnVal;
}

// offset: 0x2038 | func: 19
/** 
  * Create a leafless version of the Tumbleweed, inheriting the leafy Tumbleweed's transform. 
  *
  * Used to turn the Tumbleweeds into a bundle of twigs when attacked.
*/
void Tumbleweed_create_twigs(Object* self) {
    Tumbleweed_Setup* setup;

    switch (self->id) {
        case OBJ_Tumbleweed1:
            setup = obj_alloc_create_info(sizeof(Tumbleweed_Setup), OBJ_Tumbleweed1twig);
            break;
        case OBJ_Tumbleweed2:
            setup = obj_alloc_create_info(sizeof(Tumbleweed_Setup), OBJ_Tumbleweed2twig);
            break;
        case OBJ_Tumbleweed3:
            setup = obj_alloc_create_info(sizeof(Tumbleweed_Setup), OBJ_Tumbleweed3twig);
            break;
    }
    
    setup->base.x = self->srt.transl.x;
    setup->base.y = self->srt.transl.y;
    setup->base.z = self->srt.transl.z;
    setup->base.fadeDistance = 0xFF;
    setup->base.loadFlags = OBJSETUP_LOAD_FLAG20;
    setup->base.fadeFlags = OBJSETUP_FADE_PLAYER_RELATIVE;
    setup->roll = self->srt.roll;
    setup->pitch = self->srt.pitch;
    setup->yaw = self->srt.yaw;
    setup->carryingGold = 0;
    setup->interactRadius = 64.0f;
    obj_create((ObjSetup*)setup, OBJ_INIT_FLAG1 | OBJ_INIT_FLAG4, self->mapID, -1, self->parent);
}
