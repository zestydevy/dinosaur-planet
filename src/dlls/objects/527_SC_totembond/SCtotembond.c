#include "common.h"
#include "game/objects/object.h"

#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/516_SC_lightfoot.h"
#include "dlls/objects/519_SC_levelcontrol.h"
#include "dlls/objects/725_SC_flamegameflame.h"
#include "sys/math.h"

typedef struct {
    ObjSetup base;
} SCTotemBond_Setup;

typedef struct {
    u8 flags;
    s16 directionIndex;     //Direction the totem pole is facing (used to index into `dActivateGamebits`, etc.)
    u32 soundHandle;        //Used for pole rotation sound
    s32 flameEnergyCount;   //Kyte's Flame energy
} SCTotemBond_Data;

typedef enum {
    SCTotemBond_FLAG_None = 0,
    SCTotemBond_FLAG_Init_Minigame = 1,
    SCTotemBond_FLAG_Minigame_Active = 2,
    SCTotemBond_FLAG_All_Directions_Defended = 4,
    SCTotemBond_FLAG_Finished = 8
} SCTotemBond_Flags;

typedef enum {
    SCTotemBond_SEQCMD_1_Initialise_Minigame = 1,
    SCTotemBond_SEQCMD_2_Set_Level_State_3 = 2,
    SCTotemBond_SEQCMD_3_Set_Level_State_2 = 3
} SCTotemBond_SeqCommands;

/*0x0*/ static u16 dActivateGamebits[] = {
    BIT_64D, BIT_64E, BIT_64F, BIT_650
};
/*0x8*/ static u16 dFinishedGamebits[] = {
    BIT_768, BIT_769, BIT_76A, BIT_76B
};
/*0x10*/ static u16 dKyteFlightCurveIDs[] = {
    0x84, 0x85, 0x86, 0x87
};

static int SCTotemBond_anim_callback(Object* self, Object* objOverride, AnimObj_Data* animData, s8 arg3);
static void SCTotemBond_init_minigame(Object* self, SCTotemBond_Data* objData);
static void SCTotemBond_finish_minigame(Object* self, SCTotemBond_Data* objData);
static s32 SCTotemBond_pick_new_direction(Object* self, SCTotemBond_Data* objData);
static void SCTotemBond_set_level_state(Object* self, u8 value);

// offset: 0x0 | ctor
void SCTotemBond_ctor(void *dll) { }

// offset: 0xC | dtor
void SCTotemBond_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SCTotemBond_setup(Object* self, SCTotemBond_Setup* objSetup, s32 arg2) {
    SCTotemBond_Data* objData = self->data;
    
    objData->directionIndex = (u16)self->srt.yaw / M_90_DEGREES;
    self->animCallback = SCTotemBond_anim_callback;
    self->stateFlags |= (OBJSTATE_UPDATE_DISABLED | OBJSTATE_PRINT_DISABLED);
}

// offset: 0x68 | func: 1 | export: 1
void SCTotemBond_control(Object* self) {
    SCTotemBond_Data* objData;
    Object* player;
    Object* sidekick;

    objData = self->data;
    player = get_player();
    sidekick = get_sidekick();
    
    if (objData->flags & SCTotemBond_FLAG_Init_Minigame) {
        SCTotemBond_init_minigame(self, objData);
    }
    
    if (objData->flags & SCTotemBond_FLAG_Minigame_Active) {
        if (sidekick != NULL) {
            ((DLL_ISidekick*)sidekick->dll)->vtbl->func18(sidekick, objData->flameEnergyCount);
        }
        
        ((DLL_210_Player*)player->dll)->vtbl->func71(player, &self->srt.transl, &self->srt, 0);
        
        //Rotate pole when gamebit set
        if (main_get_bits(BIT_LFV_Flame_Game_Rotate_Pole)) {
            main_set_bits(BIT_LFV_Flame_Game_Rotate_Pole, 0);
            if (SCTotemBond_pick_new_direction(self, objData)) {
                objData->flags |= SCTotemBond_FLAG_All_Directions_Defended;
            }
            
            objData->soundHandle = gDLL_6_AMSFX->vtbl->play(self, SOUND_796_Pole_Rotate, MAX_VOLUME, 0, 0, 0, 0);
        }

        //Rotate towards goal direction
        if ((u16)self->srt.yaw / M_90_DEGREES != objData->directionIndex) {
            self->srt.yaw -= 256.0f * gUpdateRateF;
            if ((u16)self->srt.yaw / M_90_DEGREES == objData->directionIndex) {
                //Set gamebit to enable the direction's Flame target
                main_set_bits(dActivateGamebits[objData->directionIndex], 1);
            }
        }
        
        if (objData->flags & (SCTotemBond_FLAG_All_Directions_Defended | SCTotemBond_FLAG_Finished)) {
            SCTotemBond_finish_minigame(self, objData);
        }
    }
}

// offset: 0x2B8 | func: 2 | export: 2
void SCTotemBond_update(Object *self) { }

// offset: 0x2C4 | func: 3 | export: 3
void SCTotemBond_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x318 | func: 4 | export: 4
void SCTotemBond_free(Object* self, s32 arg1) {
    SCTotemBond_Data* objData = self->data;
    
    if (objData->soundHandle != 0) {
        gDLL_6_AMSFX->vtbl->stop(objData->soundHandle);
        objData->soundHandle = 0;
    }
}

// offset: 0x380 | func: 5 | export: 5
u32 SCTotemBond_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x390 | func: 6 | export: 6
u32 SCTotemBond_get_data_size(Object *self, u32 a1) {
    return sizeof(SCTotemBond_Data);
}

// offset: 0x3A4 | func: 7
int SCTotemBond_anim_callback(Object* self, Object* objOverride, AnimObj_Data* animData, s8 arg3) {
    SCTotemBond_Data* objData;
    s32 i;

    objData = self->data;
    animData->unk62 = 0;

    for (i = 0; i < animData->unk98; i++) {
        switch (animData->unk8E[i]) {
        case SCTotemBond_SEQCMD_1_Initialise_Minigame:
            objData->flags |= SCTotemBond_FLAG_Init_Minigame;
            self->srt.yaw = M_180_DEGREES - 1;
            gDLL_3_Animation->vtbl->func19(0x56, 1, 0, 0);
            break;
        case SCTotemBond_SEQCMD_2_Set_Level_State_3:
            //Plays regular SwapStone Circle music
            SCTotemBond_set_level_state(self, 3);
            break;
        case SCTotemBond_SEQCMD_3_Set_Level_State_2:
            SCTotemBond_set_level_state(self, 2);
            break;
        }
    }
    
    return 0;
}

// offset: 0x510 | func: 8
static void SCTotemBond_create_lightfoot_and_flame_targets(Object* self, SCTotemBond_Data* objData, f32 lightFootRadius) {
    s32 angle;
    s8 lfIndex;
    s8 i;
    SCFlameGameFlame_Setup* setupFlame;
    SCLightFoot_Setup* setupLF;
    SCTotemBond_Setup* objSetup;

    //Create 4 LightFoot and 4 Flame targets
    for (i = 0, lfIndex = 1; i < 4; i++) {
        objSetup = (SCTotemBond_Setup*)self->setup;
        angle = i << 0xE; //degrees: 0, 90, 180, 270
        
        setupLF = obj_alloc_setup(sizeof(SCLightFoot_Setup), OBJ_SC_lightfootSpe);
        setupLF->base.x = (fsin16_precise(self->srt.yaw + angle) * lightFootRadius) + self->srt.transl.x;
        setupLF->base.y = self->srt.transl.y;
        setupLF->base.z = (fcos16_precise(self->srt.yaw + angle) * lightFootRadius) + self->srt.transl.z;
        setupLF->base.loadFlags = objSetup->base.loadFlags;
        setupLF->base.byte5 = objSetup->base.byte5;
        setupLF->base.byte6 = objSetup->base.byte6;
        setupLF->base.fadeDistance = objSetup->base.fadeDistance;
        setupLF->unk18 = -1;
        setupLF->unk1A = 0x64C;
        setupLF->unk1C = dFinishedGamebits[lfIndex];
        setupLF->unk30 = dActivateGamebits[lfIndex];
        setupLF->yaw = (self->srt.yaw + angle + M_180_DEGREES) >> 8;
        setupLF->unk32 = 1;
        
        setupFlame = obj_alloc_setup(sizeof(SCFlameGameFlame_Setup), OBJ_SC_flamegamefla);
        setupFlame->base.x = (fsin16_precise(self->srt.yaw + angle) * lightFootRadius) + self->srt.transl.x;
        setupFlame->base.y = self->srt.transl.y;
        setupFlame->base.z = (fcos16_precise(self->srt.yaw + angle) * lightFootRadius) + self->srt.transl.z;
        setupFlame->base.loadFlags = objSetup->base.loadFlags;
        setupFlame->base.byte5 = objSetup->base.byte5;
        setupFlame->base.byte6 = objSetup->base.byte6;
        setupFlame->base.fadeDistance = objSetup->base.fadeDistance;
        setupFlame->flameRange = 75;
        setupFlame->unk19 = 0;
        setupFlame->kyteFlightCurveID = dKyteFlightCurveIDs[lfIndex];
        setupFlame->unk1C = 5;
        setupFlame->unk1D = 7;
        setupFlame->unk1E = 7;
        setupFlame->yaw = (self->srt.yaw + angle + M_180_DEGREES) >> 8;
        
        obj_create((ObjSetup*)setupLF, 5, -1, -1, 0);
        obj_create((ObjSetup*)setupFlame, 5, -1, -1, 0);
        
        lfIndex++;
        if (lfIndex >= 4) {
            lfIndex = 0;
        }
    }
}

// offset: 0x7F8 | func: 9
void SCTotemBond_init_minigame(Object* self, SCTotemBond_Data* objData) {
    Object* player;
    Object* kyte;

    //Make sure Kyte has at least 1 unit of Flame energy
    kyte = get_sidekick();
    if (kyte != NULL) {
        objData->flameEnergyCount = ((DLL_ISidekick*)kyte->dll)->vtbl->get_red_food_count(kyte);
        if (objData->flameEnergyCount <= 0) {
            objData->flameEnergyCount = 1;
        }
    } else {
        objData->flameEnergyCount = 1;
    }
    
    self->srt.yaw = M_180_DEGREES - 1;
    func_800267A4(self);

    //Create minigame objects, and enable first Flame target
    SCTotemBond_create_lightfoot_and_flame_targets(self, objData, -70.0f);
    main_set_bits(dActivateGamebits[objData->directionIndex], 1);

    //Hide totem (for first-person view)
    self->opacity = 0;
    
    //Set object flags
    objData->flags &= ~SCTotemBond_FLAG_Init_Minigame;
    objData->flags |= SCTotemBond_FLAG_Minigame_Active;
    
    //Set Kyte's initial flight curve
    main_set_bits(BIT_Kyte_Flight_Curve, 0x83);
    
    //Create restart point
    player = get_player();
    gDLL_29_Gplay->vtbl->restart_set(&player->srt.transl, player->srt.yaw, map_get_layer());
}


// offset: 0x960 | func: 10
void SCTotemBond_finish_minigame(Object* self, SCTotemBond_Data* objData) {
    Object* player;

    player = get_player();
    
    gDLL_29_Gplay->vtbl->restart_clear();
    gDLL_2_Camera->vtbl->change_camera_module(DLL_ID_CAMNORMAL, 0, 3, 0, NULL, 0, 0);
    self->opacity = OBJECT_OPACITY_MAX;
    ((DLL_210_Player*)player->dll)->vtbl->func71(player, 0, 0, 0);
    
    func_8002674C(self);
    
    if (objData->flags & SCTotemBond_FLAG_All_Directions_Defended) {
        main_set_bits(BIT_LightFoot_Village_Krystal_Freed, 1);
    }
    objData->flags = SCTotemBond_FLAG_None;
}

// offset: 0xA70 | func: 11
/**
  * Picks a new direction for the player to face, 
  * excluding directions that Kyte has already defended using Flame.
  *
  * Returns 0 if successful, or 1 if no direction could be picked.
  */
s32 SCTotemBond_pick_new_direction(Object* self, SCTotemBond_Data* objData) {
    s8 directionsAvailable[4];
    u8 random;
    u8 directions;
    u8 i;

    for (i = 0, directions = 0; i < 4; i++) {
        if (main_get_bits(dFinishedGamebits[i]) == 0) {
            directionsAvailable[directions++] = i;
        }
    }
    
    if (!directions) {
        return 1;
    }
    
    random = directionsAvailable[rand_next(0, directions - 1)];
    if (objData->directionIndex == random) {
        main_set_bits(dActivateGamebits[objData->directionIndex], 1);
    }
    objData->directionIndex = random;
    
    return 0;
}

// offset: 0xB80 | func: 12
/**
  * Finds the `SC_levelcontrol` Object, and uses one of its functions to
  * set an overall state value for SwapStone Circle.
  */
void SCTotemBond_set_level_state(Object* self, u8 value) {
    Object** objects;
    s32 index;
    s32 count;

    for (objects = get_world_objects(&index, &count); index < count; index++) {
        if ((self != objects[index]) && (objects[index]->id == OBJ_SC_levelcontrol)) {
            ((DLL_519_SC_Levelcontrol*)objects[index]->dll)->vtbl->func7(objects[index], value);
            return;
        }
    }
}
