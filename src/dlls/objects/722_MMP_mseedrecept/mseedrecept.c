#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "dll.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/common/sidekick.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "macros.h"
#include "sys/gfx/animseq.h"
#include "sys/main.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "sys/math.h"
#include "sys/objtype.h"
#include "sys/rand.h"
#include "types.h"

typedef struct {
    u8 state;
    u8 flags;
    CurveSetup* curveSetup;
    s16 gamebitPlanted; //After using MoonSeed
    s16 gamebitGrown;   //After using Flame on planted seed (only at night according to design docs, but at any time of day in this implementation)
    s16 glowPhase;      //Angle used for sinusoidal glowing at night
    f32 rattleTimer;    //Used for shaking sounds/effects when planted and not yet grown
} MoonSeedReceptacle_Data;

typedef struct {
    ObjSetup base;
    s16 unk18;
    u16 kyteFlightGroup;
    s16 unk1C;
    s8 unk1E;
    u8 yaw;
} MoonSeedReceptacle_Setup;

typedef enum {
    MoonSeedReceptacle_STATE_0_Init,
    MoonSeedReceptacle_STATE_1_Bare_Soil,       //Waiting for the player to plant a MoonSeed
    MoonSeedReceptacle_STATE_2_Seed_Planted,    //Waiting for Flame command
    MoonSeedReceptacle_STATE_3_Seed_Flamed,     //Kyte is using Flame on the Seed
    MoonSeedReceptacle_STATE_4_Grown            //Finished
} MoonSeedReceptacle_States;

typedef enum {
    MoonSeedReceptacle_FLAG_0_None = 0,
    MoonSeedReceptacle_FLAG_1_Sequence_Played = 1,
    MoonSeedReceptacle_FLAG_2_Glowing = 2,
    MoonSeedReceptacle_FLAG_4_Rattling = 4
} MoonSeedReceptacle_Flags;


typedef enum {
    MMP_MoonSeedReceptacle_ID_1 = 0x41A5B, //Between the SharpClaw outpost and CloudRunner Fortress, leading further into MMP.
    MMP_MoonSeedReceptacle_ID_2 = 0x41A59, //Near the ravine, leading up to meteoroid impact site.
    MMP_MoonSeedReceptacle_ID_3 = 0x41A5C, //Near the exit of the meteoroid impact site, leading towards the Krazoa Shrine.
    MMP_MoonSeedReceptacle_ID_4 = 0x41A5D  //Along the path with the first Lunaimar encounters, leading up to the dark tunnel that winds back towards the SharpClaw outpost. 
} MoonSeedReceptacle_uIDs;

/*0x0*/ static u8 sPartFXParams[0x10];

static void MoonSeedReceptacle_func_D40(Object* self);
static int MoonSeedReceptacle_anim_callback(Object* self, Object *animObj, AnimObj_Data* animObjData, s8 arg3);

// offset: 0x0 | ctor
void MoonSeedReceptacle_ctor(void *dll) { }

// offset: 0xC | dtor
void MoonSeedReceptacle_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void MoonSeedReceptacle_setup(Object* self, MoonSeedReceptacle_Setup* setup, s32 reset) {
    MoonSeedReceptacle_Data* objData;

    objData = self->data;
    self->animCallback = MoonSeedReceptacle_anim_callback;
    self->srt.yaw = setup->yaw << 8;
    objData->state = MoonSeedReceptacle_STATE_0_Init;
    
    objAddObjectType(self, OBJTYPE_KyteTarget);

    switch (setup->base.uID) {
        //Soil spot just beyond SharpClaw outpost 
        case MMP_MoonSeedReceptacle_ID_1: 
            objData->gamebitPlanted = BIT_MMP_MoonSeed_1_Planted;
            objData->gamebitGrown = BIT_MMP_MoonSeed_1_Grown;
            break;
        //Soil spot near ravine, leading up to meteoroid impact site
        case MMP_MoonSeedReceptacle_ID_2: 
            objData->gamebitPlanted = BIT_MMP_MoonSeed_2_Planted;
            objData->gamebitGrown = BIT_MMP_MoonSeed_2_Grown;
            break;
        //Soil spot leading out of meteoroid impact site, towards Shrine
        case MMP_MoonSeedReceptacle_ID_3: 
            objData->gamebitPlanted = BIT_MMP_MoonSeed_3_Planted;
            objData->gamebitGrown = BIT_MMP_MoonSeed_3_Grown;
            break;
        //Soil spot leading up to dark tunnel
        case MMP_MoonSeedReceptacle_ID_4:
            objData->gamebitPlanted = BIT_MMP_MoonSeed_4_Planted;
            objData->gamebitGrown = BIT_MMP_MoonSeed_4_Grown;
            break;
        default:
            STUBBED_PRINTF("Error romdef ident no known!\n");
    }

    objData->flags = MoonSeedReceptacle_FLAG_0_None;
}

// offset: 0x118 | func: 1 | export: 1
void MoonSeedReceptacle_control(Object* self) {
    MoonSeedReceptacle_Data *objData;
    MoonSeedReceptacle_Setup *objSetup;
    Object *kyte;
    SRT particleTrans;
    f32 time;
    u8 bssIndex;
    s32 count;
        
    objData = self->data;
    objSetup = (MoonSeedReceptacle_Setup *) self->setup;
    
    if (objData->flags & MoonSeedReceptacle_FLAG_1_Sequence_Played){
        objData->state = MoonSeedReceptacle_STATE_2_Seed_Planted;
        gDLL_6_AMSFX->vtbl->play(self, SOUND_798_Puzzle_Solved, MAX_VOLUME, NULL, NULL, 0, NULL);
        mainSetBits(objData->gamebitPlanted, TRUE);
        objData->flags &= ~MoonSeedReceptacle_FLAG_1_Sequence_Played;
        self->opacity = OBJECT_OPACITY_MAX;
    }
    
    //Glow at night-time, or while being Flamed
    if (gDLL_7_Newday->vtbl->func8(&time) || objData->state == MoonSeedReceptacle_STATE_3_Seed_Flamed){
        objData->flags |= MoonSeedReceptacle_FLAG_2_Glowing;
    } else {
        objData->flags &= ~MoonSeedReceptacle_FLAG_2_Glowing;
    }
    
    bssIndex = 0;

    switch (objData->state){
        case MoonSeedReceptacle_STATE_0_Init:
            objData->state = MoonSeedReceptacle_STATE_1_Bare_Soil;
            self->srt.transl.y = objSetup->base.y - 10.0f;

            //Restore state if a seed was planted previously
            if (mainGetBits(objData->gamebitPlanted)){
                objData->state = MoonSeedReceptacle_STATE_2_Seed_Planted;
                self->srt.transl.y = objSetup->base.y;
                self->opacity = OBJECT_OPACITY_MAX;
            }

            //Restore state if a seed was grown previously
            if (mainGetBits(objData->gamebitGrown)){
                MoonSeedReceptacle_func_D40(self);
            }
            return;
        
        case MoonSeedReceptacle_STATE_1_Bare_Soil:
            //Play a sequence when the player plants a MoonSeed
            if ((self->unkAF & ARROW_FLAG_1_Interacted) && 
                gDLL_1_cmdmenu->vtbl->was_this_item_used(BIT_Inventory_MoonSeeds)
            ){
                count = mainGetBits(BIT_Inventory_MoonSeeds);
                if (count){
                    self->srt.transl.y = objSetup->base.y;
                    self->opacity = 0;
                    gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
                    mainSetBits(BIT_Inventory_MoonSeeds, count - 1);
                }
            }
            break;
        
        case MoonSeedReceptacle_STATE_2_Seed_Planted:
            kyte = objGetSidekick();

            //Glow at night
            if (objData->flags & MoonSeedReceptacle_FLAG_2_Glowing){
                //While rattling: shake vertically and emit a particle
                if (objData->flags & MoonSeedReceptacle_FLAG_4_Rattling){
                    self->srt.transl.y = mathRnd(-1, 1) + objSetup->base.y;
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_70F, NULL, PARTFXFLAG_2, -1, NULL);
                }
                
                //Play a rattle sound randomly
                objData->rattleTimer -= gUpdateRateF;
                if (objData->rattleTimer <= 0.0f){
                    if (mathRnd(0, 1)){
                        objData->rattleTimer = 45.0f;
                        gDLL_6_AMSFX->vtbl->play(self, mathRnd(SOUND_A71_Rattling_1, SOUND_A72_Rattling_2), MAX_VOLUME, NULL, NULL, bssIndex, NULL);
                        objData->flags |= MoonSeedReceptacle_FLAG_4_Rattling;
                    } else {
                        objData->rattleTimer = mathRnd(50, 200);
                        objData->flags &= ~MoonSeedReceptacle_FLAG_4_Rattling;
                    }
                }
                
                //Create glow particles
                {
                    particleTrans.transl.x = mathRnd(-7, 7) + objSetup->base.x;
                    particleTrans.transl.y = mathRnd(0, 10) + objSetup->base.y;
                    particleTrans.transl.z = mathRnd(-7, 7) + objSetup->base.z;
                    
                    sPartFXParams[bssIndex] = (mathSinfInterp(objData->glowPhase) + 1.0f) * 24.0f;
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_70D, &particleTrans, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_70E, NULL, PARTFXFLAG_2, -1, &sPartFXParams[bssIndex]);
                }
            }

            //Show Flame command in the inventory when highlighted (can be used at any time of day)
            if (kyte && (self->unkAF & ARROW_FLAG_4_Highlighted)){
                //Enable Flame command option
                ((DLL_ISidekick*)kyte->dll)->vtbl->enable_command(kyte, Sidekick_Command_INDEX_4_Flame);

                //Check if Flame command was used
                if (gDLL_1_cmdmenu->vtbl->was_this_item_used(Sidekick_Command_INDEX_4_Flame)){
                    mainSetBits(BIT_Kyte_Flight_Curve, objSetup->kyteFlightGroup);
                }
            }
            break;
        
        case MoonSeedReceptacle_STATE_3_Seed_Flamed:
            kyte = objGetSidekick();
            self->srt.transl.y = objSetup->base.y;
            if (vec3DistanceXZSquared(&kyte->globalPosition, &self->globalPosition) <= SQ(50)){
                for (count = -1; count < (objData->glowPhase >> 0xD); count++){
                    particleTrans.transl.x = mathRnd(-7, 7) + objSetup->base.x;
                    particleTrans.transl.y = mathRnd(0, 10) + objSetup->base.y;
                    particleTrans.transl.z = mathRnd(-7, 7) + objSetup->base.z;
                    gDLL_17_partfx->vtbl->spawn(self, PARTICLE_70D, &particleTrans, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
                }
                
                sPartFXParams[bssIndex] = objData->glowPhase >> 7;
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_70E, NULL, 2, -1, &sPartFXParams[bssIndex]);
            } else {
                particleTrans.transl.x = mathRnd(-7, 7) + objSetup->base.x;
                particleTrans.transl.y = mathRnd(0, 10) + objSetup->base.y;
                particleTrans.transl.z = mathRnd(-7, 7) + objSetup->base.z;
                sPartFXParams[bssIndex] = 0x50;
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_70D, &particleTrans, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
                gDLL_17_partfx->vtbl->spawn(self, PARTICLE_70E, NULL, PARTFXFLAG_2, -1, &sPartFXParams[bssIndex]);
            }
            break;
    }

}

// offset: 0x998 | func: 2 | export: 2
void MoonSeedReceptacle_update(Object *self) { }

// offset: 0x9A4 | func: 3 | export: 3
void MoonSeedReceptacle_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    MoonSeedReceptacle_Data* objData;
    u8 otherColourR;
    u8 colourR;

    objData = self->data;
    
    if (visibility) {
        if (objData->state == MoonSeedReceptacle_STATE_2_Seed_Planted) {
            if (objData->flags & MoonSeedReceptacle_FLAG_2_Glowing) {
                objData->glowPhase += M_45_DEGREES/2;
                colourR = (u8) ((mathSinfInterp(objData->glowPhase) + 1) * 63.0f) + 0x7F;

                objprintSetMultiplierColor(colourR, 0xFF, 0xFF);
            }
        } else if (objData->state == MoonSeedReceptacle_STATE_3_Seed_Flamed) {
            if (objData->glowPhase < M_180_DEGREES - 0x300) {
                objData->glowPhase += 0xFF;
            }
            objprintSetMultiplierColor((otherColourR >> 7), 0xFF, 0xFF); //@bug: otherColourR uninitialised
        } else {
            objprintSetMultiplierColor(0xFF, 0xFF, 0xFF);
        }

        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0xB88 | func: 4 | export: 4
void MoonSeedReceptacle_free(Object* self, s32 onlySelf) {
    objFreeObjectType(self, OBJTYPE_KyteTarget);
}

// offset: 0xBC8 | func: 5 | export: 5
u32 MoonSeedReceptacle_get_model_flags(Object *self){
    return MODFLAGS_1;
}

// offset: 0xBD8 | func: 6 | export: 6
u32 MoonSeedReceptacle_get_data_size(Object *self, u32 a1) {
    return sizeof(MoonSeedReceptacle_Data);
}

// offset: 0xBEC | func: 7 | export: 7
s32 MoonSeedReceptacle_func_BEC(Object* self, s32 arg1) {
    MoonSeedReceptacle_Data* objData  = self->data;
    s32 returnVal = FALSE;

    if (arg1 == 0) {
        if (objData->flags & MoonSeedReceptacle_FLAG_2_Glowing) {
            objData->state = MoonSeedReceptacle_STATE_3_Seed_Flamed;
            objData->glowPhase = 0;
        }
        returnVal = TRUE;
    } else if (arg1 == 1) {
        if (objData->state == MoonSeedReceptacle_STATE_3_Seed_Flamed) {
            returnVal = TRUE;
            if (mainGetBits(objData->gamebitPlanted) && 
                !mainGetBits(objData->gamebitGrown)) {
                MoonSeedReceptacle_func_D40(self);
            }
        } else {
            gDLL_6_AMSFX->vtbl->play(self, SOUND_912_Object_Refused, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
    }

    return returnVal;
}

// offset: 0xD00 | func: 8 | export: 8
u32 MoonSeedReceptacle_func_D00(Object *self, u32 a1, u32 a2) {
    return 0;
}

// offset: 0xD18 | func: 9 | export: 9
u32 MoonSeedReceptacle_func_D18(Object *self, u32 a1, u32 a2) {
    return 0;
}

// offset: 0xD30 | func: 10 | export: 10
u32 MoonSeedReceptacle_func_D30(Object *self){
    return 2;
}

// offset: 0xD40 | func: 11
void MoonSeedReceptacle_func_D40(Object* self) {
    MoonSeedReceptacle_Data* objData;
    MoonSeedReceptacle_Setup* objSetup;
    CurveSetup* curveSetup;

    objData = self->data;
    objSetup = (MoonSeedReceptacle_Setup*)self->setup;
    
    if (mainGetBits(objData->gamebitPlanted) == FALSE) {
        return;
    }

    self->unkAF |= ARROW_FLAG_8_No_Targetting;
    gDLL_6_AMSFX->vtbl->play(self, SOUND_798_Puzzle_Solved, MAX_VOLUME, NULL, NULL, 0, NULL);
    mainSetBits(objData->gamebitGrown, TRUE);
    objData->state = MoonSeedReceptacle_STATE_4_Grown;
    self->srt.transl.y = objSetup->base.y;

    curveSetup = gDLL_25->vtbl->func_2BC4(self, objSetup->kyteFlightGroup);
    objData->curveSetup = curveSetup;
    if (curveSetup->type22.usedBit != NO_GAMEBIT) {
        mainSetBits(curveSetup->type22.usedBit, TRUE);
    }
}

// offset: 0xE5C | func: 12
static int MoonSeedReceptacle_anim_callback(Object* self, Object *animObj, AnimObj_Data* animData, s8 prevCallbackValue) {
    MoonSeedReceptacle_Data* objData = self->data;

    objData->flags |= MoonSeedReceptacle_FLAG_1_Sequence_Played;

    return 0;
}
