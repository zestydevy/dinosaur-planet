#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "macros.h"
#include "sys/objanim.h"
#include "sys/objtype.h"

#include "dlls/objects/211_tricky.h"
#include "dlls/objects/685_DBbonedust.h"

typedef enum {
    DBDustGeezer_STATE_Main = 0,
    DBDustGeezer_STATE_Cooldown = 1,
    DBDustGeezer_STATE_Hidden = 2
} DBDustGeezer_States;

typedef enum {
    DBDustGeezer_FLAG_None = 0,
    DBDustGeezer_FLAG_Dug_Up = 1
} DBDustGeezer_Flags;

typedef struct {
    ObjSetup base;
    s8 unused18;
    s8 randomnessRange;     //Controls the random variance in launched gems' start position/speed
    s16 cooldownDuration;   //Cooldown after launching all gems
    s16 launchedGemCount;   //How many gems to launch after being dug up (max of 8)
    s16 unused1E;
    s16 gamebitHidden;      //Hides geyser during setup if it's already dug up
} DBDustGeezer_Setup;

typedef struct {
    Object* boneDust[8];    //DBBoneDust objects
    s32 range;              //Controls the random variance in launched gems' start position/speed
    s32 gemsToLaunch;       //How many gems to launch after being dug up (max of 8)
    s32 cooldownTimer;      //Cooldown after launching all gems
    s32 soundHandle1;       //Sound handle (not fully used?)
    s32 soundHandle2;       //Sound handle (not fully used?)
    u8 state;               //State index
    u8 flags;               //Flag 1 starts gem launch
} DBDustGeezer_Data;

static int DBDustGeezer_launch_gem(Object* self);
static Object* DBDustGeezer_create_gem(Object* self, s32 objectID);
static void DBDustGeezer_scroll_texture_uvs(Object* self);

// offset: 0x0 | ctor
void DBDustGeezer_ctor(void *dll) { }

// offset: 0xC | dtor
void DBDustGeezer_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DBDustGeezer_setup(Object* self, DBDustGeezer_Setup* objSetup, int skipChildObjects) {
    DBDustGeezer_Data* objData;
    s32 i;

    objData = self->data;
    
    //Hide geyser if its gamebit is set
    if (main_get_bits(objSetup->gamebitHidden)) {
        objData->state = DBDustGeezer_STATE_Hidden;
    } else {
        objData->state = DBDustGeezer_STATE_Main;
    }

    if (skipChildObjects == FALSE) {
        STUBBED_PRINTF(" Creating BoneDust");
        for (i = 0; i < 8; i++){
            objData->boneDust[i] = DBDustGeezer_create_gem(self, OBJ_DBbonedust);
        }
    }
            
    obj_add_object_type(self, OBJTYPE_44);
    obj_add_object_type(self, OBJTYPE_51);
    objData->flags = 0;
    objData->soundHandle1 = 0;
    objData->range = objSetup->randomnessRange;
    self->srt.scale = self->def->scale * 0.5f;
    func_80023D30(self, 0, 0, 0);
}

// offset: 0x154 | func: 1 | export: 1
void DBDustGeezer_control(Object* self) {
    DBDustGeezer_Setup* objSetup;
    DBDustGeezer_Data* objData;
    f32 halfScale;
    s32 pad;
    SRT fxTrans;
    Object* player;
    Object* sidekick;

    objData = self->data;
    objSetup = (DBDustGeezer_Setup*)self->setup;
    
    switch (objData->state) {
    case DBDustGeezer_STATE_Hidden:
        break;
    case DBDustGeezer_STATE_Main:
        if (objData->flags & DBDustGeezer_FLAG_Dug_Up) {
            //Firing a gem into the air
            if (rand_next(0, 5) == 0) {
                STUBBED_PRINTF(" Firing %i ");
                if (DBDustGeezer_launch_gem(self) == FALSE){
                    STUBBED_PRINTF("WARNING: BoneDust Buffer Full ");
                }
                if (gDLL_6_AMSFX->vtbl->func_B48(objData->soundHandle1)) {
                    gDLL_6_AMSFX->vtbl->func_A1C(objData->soundHandle1);
                }
                gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_A21_Whoosh_Echoey, MAX_VOLUME, 0, 0, 0, 0);

                //Advance state when all gems launched
                objData->gemsToLaunch--;
                if (objData->gemsToLaunch == 0) {
                    objData->flags &= ~DBDustGeezer_FLAG_Dug_Up;
                    main_set_bits(objSetup->gamebitHidden, 0);
                    objData->state = DBDustGeezer_STATE_Cooldown;
                    objData->cooldownTimer = objSetup->cooldownDuration;
                }
            }
            fxTrans.transl.x = rand_next(-20, 20) * 0.001f;
            fxTrans.transl.z = rand_next( 20, 80) * 0.01f;
            fxTrans.transl.y = rand_next(-20, 20) * 0.001f;
            gDLL_17_partfx->vtbl->spawn(self, 0x3C3, &fxTrans, 0, -1, 0);
        } else {
            //Enable "Find" sidekick command when nearby
            player = get_player();
            sidekick = get_sidekick();
            if (sidekick && vec3_distance_squared(&self->positionMirror, &player->positionMirror) <= 40000.0f) {
                ((DLL_211_Tricky*)sidekick->dll)->vtbl->base.func14(sidekick, 1);
            }
        }
        break;
    case DBDustGeezer_STATE_Cooldown:
        //Small delay after launching all gems
        objData->cooldownTimer -= gUpdateRate;
        if (objData->cooldownTimer < 0) {
            main_set_bits(objSetup->gamebitHidden, 1);
            objData->state = DBDustGeezer_STATE_Main;
        }

        halfScale = self->def->scale * 0.5f;
        if (halfScale < self->srt.scale) {
            self->srt.scale *= 1.0f - (0.015f * gUpdateRate);
            self->srt.yaw += 0x200;
        }
        break;
    }
    
    if (func_80024108(self, 0.012f, gUpdateRateF, 0)) {
        self->srt.scale = self->def->scale * 0.5f;
        func_80023D30(self, 0, 0, 0);
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_A20_Electric_Surge_Quieter, MAX_VOLUME, 0, 0, 0, 0);
    }
    
    if (rand_next(0, 10) == 0) {
        STUBBED_PRINTF(" Called Part effect ");
        gDLL_17_partfx->vtbl->spawn(self, 0x3BE, 0, 0, -1, 0);
    }
    
    DBDustGeezer_scroll_texture_uvs(self);
}

// offset: 0x5EC | func: 2 | export: 2
void DBDustGeezer_update(Object *self) { }

// offset: 0x5F8 | func: 3 | export: 3
void DBDustGeezer_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    DBDustGeezer_Data* objData = self->data;
    if (visibility && (objData->state != DBDustGeezer_STATE_Hidden)) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x660 | func: 4 | export: 4
void DBDustGeezer_free(Object* self, int skipChildObjects) {
    DBDustGeezer_Data* objData;
    u32 soundHandle1;
    u32 soundHandle2;
    s32 i;

    objData = self->data;
    
    obj_free_object_type(self, OBJTYPE_44);
    
    soundHandle1 = objData->soundHandle1;
    if (soundHandle1) {
        gDLL_6_AMSFX->vtbl->func_A1C(soundHandle1);
    }
    
    soundHandle2 = objData->soundHandle2;
    if (soundHandle2) {
        gDLL_6_AMSFX->vtbl->func_A1C(soundHandle2);
    }

    if (skipChildObjects == FALSE) {
        for (i = 0; i < 8; i++){ 
            obj_destroy_object(objData->boneDust[i]); 
        }
    }
    
    obj_free_object_type(self, OBJTYPE_51);
}

// offset: 0x750 | func: 5 | export: 5
u32 DBDustGeezer_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x760 | func: 6 | export: 6
u32 DBDustGeezer_get_data_size(Object *self, u32 a1) {
    return sizeof(DBDustGeezer_Data);
}

// offset: 0x774 | func: 7 | export: 7
/** Called when dug up by Tricky, starts firing gems into the air */
f32 DBDustGeezer_dug_up(Object* self, s32 arg1) {
    DBDustGeezer_Data* objData;
    DBDustGeezer_Setup* objSetup;

    objData = self->data;
    objSetup = (DBDustGeezer_Setup*)self->setup;
    
    if ((objData->gemsToLaunch == 0) && (objData->state != DBDustGeezer_STATE_Cooldown)) {
        objData->flags |= DBDustGeezer_FLAG_Dug_Up;
        func_80023D30(self, 1, 0.0f, 0);
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_A22_Electric_Surge, MAX_VOLUME, 0, 0, 0, 0);
        self->srt.scale *= 1.5f;
        objData->gemsToLaunch = objSetup->launchedGemCount;
    }
    return 0.0f;
}

// offset: 0x854 | func: 8 | export: 8
/** Returns true if the geyser is in its cooldown state */
u8 DBDustGeezer_is_in_cooldown(Object* self) {
    DBDustGeezer_Data* objData = self->data;
    return objData->state == DBDustGeezer_STATE_Cooldown;
}

// offset: 0x870 | func: 9 | export: 9
s32 DBDustGeezer_func_870(Object* self) {
    return 0;
}

// offset: 0x880 | func: 10
/** Searches for a recyclable "DBBoneDust" object and launches it into the air */
int DBDustGeezer_launch_gem(Object* self) {
    DBDustGeezer_Data* objData;
    Object* dust;
    Vec3f position;
    Vec3f speed;
    SRT fxTrans;
    s32 dustIndex;

    objData = self->data;  

    //Search for a DBBoneDust object that's in its "hidden" state
    dustIndex = 0;
    while (((DLL_685_DBBoneDust*)objData->boneDust[dustIndex]->dll)->vtbl->is_hidden(objData->boneDust[dustIndex]) == FALSE){
        dustIndex++;
        if (dustIndex >= 8){
            break;
        }
    }

    //If none were found, return unsuccessfully
    if (dustIndex == 8) {
        return 0;
    }    
   
    //Launch the gem
    dust = objData->boneDust[dustIndex];
    position.x = rand_next(-objData->range, objData->range) + self->srt.transl.x;
    position.y = self->srt.transl.y + 20.0f;
    position.z = rand_next(-objData->range, objData->range) + self->srt.transl.z;
    speed.x = rand_next(-objData->range, objData->range) * 0.01f;
    speed.z = rand_next(-objData->range, objData->range) * 0.01f;
    speed.y = rand_next(50, 100) * 0.025f;
    ((DLL_685_DBBoneDust*)dust->dll)->vtbl->launch(dust, &position, &speed, self->srt.transl.y + 15.0f);
    
    //Create effects
    fxTrans.transl.x = speed.x;
    fxTrans.transl.y = speed.y;
    fxTrans.transl.z = speed.z;
    gDLL_17_partfx->vtbl->spawn(dust, 0x3C3, &fxTrans, 0, -1, 0);
    
    fxTrans.transl.x *= 0.9f;
    fxTrans.transl.z *= 1.1f;
    gDLL_17_partfx->vtbl->spawn(dust, 0x3C3, 0, 0, -1, 0);
    
    fxTrans.transl.x *= 0.9f;
    fxTrans.transl.z *= 1.1f;
    gDLL_17_partfx->vtbl->spawn(dust, 0x3C3, 0, 0, -1, 0);

    return 1;
}

// offset: 0xB8C | func: 11
Object* DBDustGeezer_create_gem(Object* self, s32 objectID) {
    DBBoneDust_Setup* dustSetup;

    dustSetup = (DBBoneDust_Setup*)obj_alloc_create_info(sizeof(DBBoneDust_Setup), objectID);
    dustSetup->unk1E = -1;
    dustSetup->base.x = self->srt.transl.x;
    dustSetup->base.y = self->srt.transl.y - 30.0f;
    dustSetup->base.z = self->srt.transl.z;
    dustSetup->unk20 = -1;
    dustSetup->base.loadFlags = 2;
    dustSetup->base.byte5 = 1;
    dustSetup->base.loadDistance = 0xFF;
    dustSetup->base.fadeDistance = 0xFF;
    
    return obj_create((ObjSetup*)dustSetup, 5, self->mapID, -1, self->parent);
}

// offset: 0xC40 | func: 12
void DBDustGeezer_scroll_texture_uvs(Object* self) {
    TextureAnimator* texAnim;

    texAnim = func_800348A0(self, 0, 0);
    if (!texAnim) {
        return;
    }
    
    texAnim->positionV += 0x40;
    if (texAnim->positionV > 0x800) {
        texAnim->positionV -= 0x800;
    }
}
