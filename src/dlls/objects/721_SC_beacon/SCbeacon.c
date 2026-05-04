#include "common.h"
#include "macros.h"
#include "unktypes.h"
#include "game/lightactions.h"
#include "game/objects/interaction_arrow.h"
#include "sys/map_enums.h"
#include "sys/objtype.h"
#include "sys/segment_1050.h"
#include "sys/gfx/model.h"
#include "sys/gfx/modgfx.h"
#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/210_player.h"
#include "dlls/objects/519_SC_levelcontrol.h"
#include "dlls/objects/721_SCbeacon.h"

static void SCbeacon_attempt_to_light(Object* self);
static int SCbeacon_anim_callback(Object* self, Object* override, AnimObj_Data* animData, s8 arg3);

// offset: 0x0 | ctor
void SCbeacon_ctor(void *dll) { }

// offset: 0xC | dtor
void SCbeacon_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void SCbeacon_setup(Object* self, SCbeacon_Setup* objSetup, s32 arg2) {
    SCbeacon_Data* objData = self->data;

    self->animCallback = SCbeacon_anim_callback;
    self->srt.yaw = objSetup->yaw << 8;
    objData->state = SCbeacon_STATE_Initial;
    obj_add_object_type(self, OBJTYPE_48);

    switch (objSetup->base.uID) {
    case SCbeacon_Near_Golden_Plains:
        objData->gamebitTwigs = BIT_SC_Beacon_Added_Twigs_1;
        objData->gamebitLit = BIT_SC_Beacon_Lit_1;
        objData->seqIdxPlaceTwigs = SCbeacon_SEQIDX_Placing_Twigs;
        objData->seqIdxLightTwigs = SCbeacon_SEQIDX_Lighting_Twigs;
        break;
    case SCbeacon_Near_Pond_with_Pole:
        objData->gamebitTwigs = BIT_SC_Beacon_Added_Twigs_2;
        objData->gamebitLit = BIT_SC_Beacon_Lit_2;
        objData->seqIdxPlaceTwigs = SCbeacon_SEQIDX_Placing_Twigs;
        objData->seqIdxLightTwigs = SCbeacon_SEQIDX_Lighting_Twigs;
        break;
    case SCbeacon_Near_Discovery_Falls:
        objData->gamebitTwigs = BIT_SC_Beacon_Added_Twigs_3;
        objData->gamebitLit = BIT_SC_Beacon_Lit_3;
        objData->seqIdxPlaceTwigs = SCbeacon_SEQIDX_Placing_Twigs;
        objData->seqIdxLightTwigs = SCbeacon_SEQIDX_Lighting_Twigs;
        break;
    default:
        STUBBED_PRINTF("Error romdef ident no known!\n");
        break;
    }
    
    objData->unk14 = NULL;
}

// offset: 0x110 | func: 1 | export: 1
void SCbeacon_control(Object* self) {
    SCbeacon_Data* objData;
    SCbeacon_Setup* objSetup;
    Object* sidekick;
    Object* player;
    u8 playerIsNearby;

    objData = self->data;
    objSetup = (SCbeacon_Setup*)self->setup;
    player = get_player();
    
    playerIsNearby = (vec3_distance_xz_squared(&player->globalPosition, &self->globalPosition) <= SQ(objSetup->playerRange));
    
    objData->flags &= ~SCbeacon_FLAG_Emit_Light;

    if (objData->flags & SCbeacon_FLAG_Add_Tumbleweed) {
        objData->state = SCbeacon_STATE_Twigs_in_Bowl;
        self->modelInstIdx = SCbeacon_MODEL_Twigs_in_Bowl;
        main_set_bits(objData->gamebitTwigs, 1);
        objData->flags &= ~SCbeacon_FLAG_Add_Tumbleweed;
    }

    switch (objData->state) {
    case SCbeacon_STATE_Initial:
        //Restore state based on gamebits
        self->modelInstIdx = SCbeacon_MODEL_Bowl_Empty;
        objData->state = SCbeacon_STATE_Bowl_Empty;

        if (main_get_bits(objData->gamebitTwigs)) {
            self->modelInstIdx = SCbeacon_MODEL_Twigs_in_Bowl;
            objData->state = SCbeacon_STATE_Twigs_in_Bowl;
        }

        if (main_get_bits(objData->gamebitLit)) {
            SCbeacon_attempt_to_light(self); //will only be successful here if twigs gamebit also set
        }
        break;
    case SCbeacon_STATE_Bowl_Empty:
        //Play a Tumbleweed depositing sequence when the player is holding a Tumbleweed while nearby
        if (playerIsNearby && (((DLL_210_Player*)player->dll)->vtbl->func48(player) == OBJ_Tumbleweed1twig)) {
            gDLL_3_Animation->vtbl->func17(objData->seqIdxPlaceTwigs, self, -1); //@bug: doesn't play because objectID not returned
        }
        /* fallthrough */
    case SCbeacon_STATE_Twigs_in_Bowl:
        sidekick = get_sidekick();
        if (sidekick && (self->unkAF & ARROW_FLAG_4_Highlighted)) {
            //Show Flame command option
            ((DLL_ISidekick*)sidekick->dll)->vtbl->func14(sidekick, Sidekick_Command_INDEX_4_Flame);

            //Check if Flame command was selected
            if (gDLL_1_cmdmenu->vtbl->was_this_item_used(Sidekick_Command_INDEX_4_Flame)) {
                main_set_bits(BIT_Kyte_Flight_Curve, objSetup->kyteCurveID);
            }
        }
        break;
    case SCbeacon_STATE_Lighting:
        sidekick = get_sidekick();
        if (vec3_distance_xz_squared(&sidekick->globalPosition, &self->globalPosition) <= 2500.0f) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_425, NULL, 2, -1, NULL); //create smoke
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_426, NULL, 2, -1, NULL); //create embers
        }
        break;
    case SCbeacon_STATE_Lit:
        if (playerIsNearby) {
            objData->flags |= SCbeacon_FLAG_Emit_Light;
        }
        
        //Use LightActions to emit a warm glow (or remove it)
        if ((objData->flags & SCbeacon_FLAG_Emit_Light) && !(objData->prevFlags & SCbeacon_FLAG_Emit_Light)) {
            func_80000450(self, self, LIGHTACTION_59, 0, 0, 0);
        } else if (!(objData->flags & SCbeacon_FLAG_Emit_Light) && (objData->prevFlags & SCbeacon_FLAG_Emit_Light)) {
            func_80000450(self, self, LIGHTACTION_5A, 0, 0, 0);
        }

        //Create smoke
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_425, NULL, 2, -1, NULL);
        break;
    }
    
    objData->prevFlags = objData->flags;
}

// offset: 0x4F0 | func: 2 | export: 2
void SCbeacon_update(Object *self) { }

// offset: 0x4FC | func: 3 | export: 3
void SCbeacon_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x550 | func: 4 | export: 4
void SCbeacon_free(Object* self, s32 arg1) {
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_13_Expgfx->vtbl->func5(self);
    obj_free_object_type(self, OBJTYPE_48);
}

// offset: 0x5D8 | func: 5 | export: 5
u32 SCbeacon_get_model_flags(Object *self){
    return MODFLAGS_1;
}

// offset: 0x5E8 | func: 6 | export: 6
u32 SCbeacon_get_data_size(Object *self, u32 a1) {
    return sizeof(SCbeacon_Data);
}

// offset: 0x5FC | func: 7 | export: 7
/**
  * Returns TRUE when the beacon is in its "lighting" state (emitting embers/smoke while Kyte is using Flame on it).
  * 
  * `finishLighting` == 0: advances beacon to "lighting" state, plays seq where Kyte begins lighting the beacon
  *
  * `finishLighting` == 1: advances beacon to "lit" state, and (if all beacons are lit) plays the DF entrance pool seq 
  */
int SCbeacon_handle_kyte_flame_seqs(Object* self, s32 finishLighting) {
    SCbeacon_Data* objData = self->data;
    s32 isBeingLit = FALSE;
    
    if (finishLighting == FALSE) {
        //Play the sequence where Kyte lights the beacon, and advance to "lighting" state (embers/smoke)
        gDLL_3_Animation->vtbl->func17(objData->seqIdxLightTwigs, self, -1);
        objData->state = SCbeacon_STATE_Lighting;
        isBeingLit = TRUE;

    } else if (finishLighting == TRUE) {
        //Check if the beacon is being lit (emitting embers/smoke while Kyte is using Flame on it)
        if (objData->state == SCbeacon_STATE_Lighting) {
            isBeingLit = TRUE;
        }

        //Finish lighting the beacon
        if (main_get_bits(objData->gamebitTwigs) && (main_get_bits(objData->gamebitLit) == FALSE)) {
            //Advance to "lit" state
            SCbeacon_attempt_to_light(self); //will always be successful here

            //Warp to Discovery Falls' entrance when all beacons are lit, and play pool-draining sequence
            if (main_get_bits(BIT_SC_Beacon_Lit_1) && 
                main_get_bits(BIT_SC_Beacon_Lit_2) && 
                main_get_bits(BIT_SC_Beacon_Lit_3)
            ) {
                main_set_bits(BIT_SC_All_Beacons_Lit, 1);
                gDLL_29_Gplay->vtbl->set_obj_group_status(MAP_SWAPSTONE_CIRCLE, SC_ObjGroup0_Main_SwapStone_Area, TRUE);
                warpPlayer(WARP_SC_DISCOVERY_FALLS_ENTRANCE_POND, TRUE); //SC_warppoint auto-plays seq 0x77
                gDLL_29_Gplay->vtbl->set_obj_group_status(MAP_SWAPSTONE_CIRCLE, SC_ObjGroup3_Golden_Plains_Approach, FALSE);
            }
        } else {
            objData->state = SCbeacon_STATE_Bowl_Empty;
        }
    }
    
    return isBeingLit;
}

// offset: 0x7D4 | func: 8 | export: 8
s32 SCbeacon_func_7D4(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2) {
    return 0;
}

// offset: 0x7EC | func: 9 | export: 9
s32 SCbeacon_func_7EC(UNK_TYPE_32 arg0, UNK_TYPE_32 arg1, UNK_TYPE_32 arg2) {
    return 0;
}

// offset: 0x804 | func: 10 | export: 10
/** 
  * Called by Kyte's DLL after issuing Flame command, as she approaches the beacon.
  *
  * TODO: what does the 2 signify? 
  */
s32 SCbeacon_func_804(Object* self) {
    return 2;
}

// offset: 0x814 | func: 11
/**
  * Lights the beacon if it has Tumbleweed twigs.
  */
void SCbeacon_attempt_to_light(Object* self) {
    s32 pad;
    SCbeacon_Setup* objSetup;
    DLL_IModgfx* modGfxDLL;
    CurveSetup* curveSetup;
    SCbeacon_Data* objData;

    objData = self->data;
    objSetup = (SCbeacon_Setup*)self->setup;
    
    if (main_get_bits(objData->gamebitTwigs)) {
        //Play burning sound loops
        gDLL_6_AMSFX->vtbl->play(self, SOUND_50a_Fire_Burning_Low_Loop, MAX_VOLUME, NULL, NULL, 0, NULL);
        gDLL_6_AMSFX->vtbl->play(self, SOUND_50b_Fire_Burning_High_Loop, MAX_VOLUME, NULL, NULL, 0, NULL);
        
        //Create fire model
        gDLL_14_Modgfx->vtbl->func10(self);
        modGfxDLL = dll_load_deferred(DLL_ID_114, 1);
        modGfxDLL->vtbl->func0(self, 2, 0, 0x10004, -1, 0);
        dll_unload(modGfxDLL);
        
        //Disable targetting and advance to lit state
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        main_set_bits(objData->gamebitLit, 1);        
        objData->state = SCbeacon_STATE_Lit;

        //Set Kyte curve gamebit
        objData->kyteCurve = gDLL_25->vtbl->func_2BC4(self, objSetup->kyteCurveID);
        if (objData->kyteCurve->type22.usedBit != NO_GAMEBIT) {
            main_set_bits(objData->kyteCurve->type22.usedBit, 1);
        }
    }
}

// offset: 0x9E0 | func: 12
int SCbeacon_anim_callback(Object* self, Object* override, AnimObj_Data* animData, s8 arg3) {
    SCbeacon_Data* objData;
    u8 playerIsNearby;
    Object* player;
    Object* sidekick;
    SCbeacon_Setup* objSetup;

    player = get_player();
    objData = self->data;
    objSetup = (SCbeacon_Setup*)self->setup;
    
    /* @bug: Causes Tumbleweed twigs to suddenly appear in the bowl when Kyte uses Flame,
             because any sequence that includes SC_beacon will cause this animCallback to be called!
             It's not checking if this is the "depositing Tumbleweed" seq vs. the "Kyte uses Flame" seq.
    */
    objData->flags |= SCbeacon_FLAG_Add_Tumbleweed;
    
    playerIsNearby = (vec3_distance_xz_squared(&player->globalPosition, &self->globalPosition) <= SQ(objSetup->playerRange));

    switch (objData->state) {
    case SCbeacon_STATE_Lighting:
        sidekick = get_sidekick();
        if (vec3_distance_xz_squared(&sidekick->globalPosition, &self->globalPosition) <= 2500.0f) {
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_425, NULL, 2, -1, NULL);
            gDLL_17_partfx->vtbl->spawn(self, PARTICLE_426, NULL, 2, -1, NULL);
        }
        break;
    case SCbeacon_STATE_Lit:
        if (playerIsNearby) {
            objData->flags |= SCbeacon_FLAG_Emit_Light;
        }
        
        //Use LightActions to emit a warm glow (or remove it)
        if ((objData->flags & SCbeacon_FLAG_Emit_Light) && !(objData->prevFlags & SCbeacon_FLAG_Emit_Light)) {
            func_80000450(self, self, LIGHTACTION_59, 0, 0, 0);
        } else if (!(objData->flags & SCbeacon_FLAG_Emit_Light) && (objData->prevFlags & SCbeacon_FLAG_Emit_Light)) {
            func_80000450(self, self, LIGHTACTION_5A, 0, 0, 0);
        }

        //Create smoke
        gDLL_17_partfx->vtbl->spawn(self, PARTICLE_425, NULL, 2, -1, NULL);
        break;
    }
    
    objData->prevFlags = objData->flags;
    
    return 0;
}
