#include "dlls/objects/210_player.h"
#include "dlls/objects/214_animobj.h"
#include "game/gamebits.h"
#include "sys/dll.h"
#include "sys/gfx/modgfx.h"
#include "sys/objanim.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"
#include "dll.h"
#include "functions.h"

#include "dlls/objects/469_ECSHshrine.h"

#define SPIRIT_NUMBER(spiritNum) (1 << (spiritNum - 1))

#define NO_CHOICE_YET -1

#define WHISPERS_VOL_MIN 12
#define WHISPERS_VOL_MAX 70
#define WHISPERS_VOL_INIT WHISPERS_VOL_MIN

#define CORRIDOR_VOL_MIN 1
#define CORRIDOR_VOL_MAX 70
#define CORRIDOR_VOL_INIT 30

/** Positions (XZ) of each of the 6 cups.
  *
  * Slot 0 is the one at the bottom of the Krazoa symbol (closest to the door),
  * and the other indices proceed anticlockwise from there (when viewed from above)
  */
/*0x0*/ static Vec2f dCupCoords[] = {
    {0.0f, 0.0f}, 
    {0.0f, 0.0f}, 
    {0.0f, 0.0f}, 
    {0.0f, 0.0f},
    {0.0f, 0.0f},
    {0.0f, 0.0f}
};

/** 6 slots for the cups: stores the index of the cup at each slot. */
/*0x30*/ static s16 dCupSlots[] = {0, 1, 2, 3, 4, 5};
/*0x3C*/ static Texture *dTexture = NULL;
/*0x40*/ static Object *dShrine = NULL;

typedef enum {
    Shrine_MUSICTRACK_Whispering = 2,
    Shrine_MUSICTRACK_Corridor = 3
} ECSHshrine_MusicTracks;

static int ECSHshrine_anim_callback(Object *self, Object *a1, AnimObj_Data *a2, s8 a3);
static void ECSHshrine_handle_messages(Object *self);

// offset: 0x0 | ctor
void ECSHshrine_ctor(void *dll) { }

// offset: 0xC | dtor
void ECSHshrine_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void ECSHshrine_setup(Object *self, ECSHshrine_Setup *setup, s32 arg2) {
    ECSHshrine_Data *objdata;
    DLL_IModgfx *modGfxDLL;

    objdata = self->data;
    self->srt.yaw = 0;
    objdata->testStartRadius = 10;
    if (setup->testStartRadius > 0) {
        objdata->testStartRadius = setup->testStartRadius >> 8;
    }
    objdata->state = ECShrine_STATE_Waiting;
    objdata->seqValue = 0;
    objdata->musicPlayTimer = 0;
    objdata->delayTimer = 0;
    objdata->unk10 = 0;
    objdata->shuffles = 0;
    objdata->substate = Cup_STATE_Stopped;
    objdata->choiceWasCorrect = NO_CHOICE_YET;
    objdata->spiritCup = 0;
    self->animCallback = ECSHshrine_anim_callback;
    obj_init_mesg_queue(self, 4);
    main_set_bits(BIT_DB_Entered_Shrine_3, 1);
    main_set_bits(BIT_143, 0);
    main_set_bits(BIT_DB_Entered_Shrine_1, 1);
    main_set_bits(BIT_DB_Entered_Shrine_2, 1);
    main_set_bits(BIT_Test_of_Fear_Particles, 0);
    objdata->musWhispersVol = WHISPERS_VOL_INIT;
    objdata->musCorridorVol = CORRIDOR_VOL_INIT;
    objdata->musicPlayTimer = 200;
    gDLL_5_AMSEQ->vtbl->play_ex(2, 43, 0x50, 1, 0);
    objdata->musWhipersVolSpeed = 0;
    objdata->musCorridorVolSpeed = 0;
    objdata->musicStarted = FALSE;

    //Create glowing circle around test's startpoint
    modGfxDLL = dll_load_deferred(DLL_ID_122, 1);
    objdata->modGfxCircle = modGfxDLL->vtbl->func0(self, 2, NULL, 0x402, -1, NULL);
    dll_unload(modGfxDLL);

    dShrine = self;
    obj_add_object_type(self, OBJTYPE_13);
}

// offset: 0x208 | func: 1 | export: 1
void ECSHshrine_control(Object* self) {
    ECSHshrine_Data* objdata = self->data;
    Object* player = get_player();
    f32 dz;
    f32 tempCupZ;
    f32 tempCupX;
    s32 sp4C[] = {0, 0};
    DLL_IModgfx* modGfxDLL;
    Object* door;
    f32 objectDistance;
    s16 volume;
    s16 tempCupIndex;
    s32 i;

    objectDistance = 1000.0f;
    ECSHshrine_handle_messages(self);
    main_set_bits(BIT_DB_Entered_Shrine_2, 1);

    //Update music volume (whispers)
    if (objdata->musWhipersVolSpeed != 0) {
        objdata->musWhispersVol += objdata->musWhipersVolSpeed;
        if (objdata->musWhispersVol <= WHISPERS_VOL_MIN) {
            objdata->musWhispersVol = WHISPERS_VOL_MIN;
            objdata->musWhipersVolSpeed = 0;
        } else if (objdata->musWhispersVol >= WHISPERS_VOL_MAX) {
            objdata->musWhispersVol = WHISPERS_VOL_MAX;
            objdata->musWhipersVolSpeed = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(Shrine_MUSICTRACK_Whispering, objdata->musWhispersVol);
    }

    //Update music volume (corridor ambience)
    if (objdata->musCorridorVolSpeed != 0) {
        objdata->musCorridorVol += objdata->musCorridorVolSpeed;
        if ((objdata->musCorridorVol <= CORRIDOR_VOL_MIN) && (objdata->musCorridorVolSpeed <= 0)) {
            objdata->musCorridorVol = CORRIDOR_VOL_MIN;
            objdata->musCorridorVolSpeed = 0;
        } else if ((objdata->musCorridorVol >= CORRIDOR_VOL_MAX) && (objdata->musCorridorVolSpeed >= 0)) {
            objdata->musCorridorVol = CORRIDOR_VOL_MAX;
            objdata->musCorridorVolSpeed = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(Shrine_MUSICTRACK_Corridor, objdata->musCorridorVol);
    }

    //Start corridor music (and return early)
    if (objdata->musicPlayTimer > 0) {
        objdata->musicPlayTimer -= gUpdateRate;
        if (objdata->musicPlayTimer <= 0) {
            objdata->musicPlayTimer = 0;
            if (objdata->musicStarted == FALSE) {
                gDLL_5_AMSEQ->vtbl->play_ex(3, 44, 0x50, objdata->musCorridorVol, 0);
                objdata->musicStarted = TRUE;
            }
        }
        return;
    }

    //Crossfade music volumes as player approaches door
    door = obj_get_nearest_type_to(OBJTYPE_16, player, &objectDistance);
    if ((door != NULL) && (objectDistance < 300.0f) && (objectDistance > 100.0f)) {
        dz = door->srt.transl.z - player->srt.transl.z;

        //If player ahead of door (in shrine corridor)
        if (dz <= 0.0f) {
            //Get absolute distance along z
            if (dz < 0.0f) {
                dz *= -1.0f;
            }

            if (objdata->musCorridorVol != CORRIDOR_VOL_INIT) {
                objdata->musCorridorVol = CORRIDOR_VOL_INIT;
            }

            //Cross-fade music tracks as player approaches door
            volume = ((f32) objdata->musCorridorVol * ((dz - 100.0f) / 200.0f));
            if (volume < 1) {
                volume = 1;
            }
            gDLL_5_AMSEQ->vtbl->set_volume(Shrine_MUSICTRACK_Corridor, volume); //fade out corridor music

            volume = ((f32) objdata->musWhispersVol * ((200.0f - (dz - 100.0f)) / 200.0f));
            if (volume < 1) {
                volume = 1;
            }
            gDLL_5_AMSEQ->vtbl->set_volume(Shrine_MUSICTRACK_Whispering, volume); //fade in Krazoa whispering
        }
    }

    //STATE MACHINE
    switch (objdata->state) {
    case ECShrine_STATE_Waiting:
        if (vec3_distance(&self->positionMirror, &player->positionMirror) < objdata->testStartRadius) {
            objdata->state = ECShrine_STATE_Test_Start;
            main_set_bits(BIT_DB_Entered_Shrine_3, 0);
            gDLL_3_Animation->vtbl->func17(0, self, -1);

            modGfxDLL = dll_load_deferred(DLL_ID_147, 1);
            modGfxDLL->vtbl->func0(self, 2, NULL, 1, -1, NULL);
            dll_unload(modGfxDLL);

            modGfxDLL = dll_load_deferred(DLL_ID_148, 1);
            modGfxDLL->vtbl->func0(self, 0, NULL, 1, -1, NULL);
            dll_unload(modGfxDLL);

            main_set_bits(BIT_DB_Entered_Shrine_1, 0);
            gDLL_14_Modgfx->vtbl->func7(&objdata->modGfxCircle);
        }
        break;
    case ECShrine_STATE_Test_Start:
        //Wait for sequence to advance state
        if (objdata->seqValue == 1) {
            objdata->state = ECShrine_STATE_Cup_Game_Start;
            objdata->musicPlayTimer = 200;
            objdata->substate = Cup_STATE_Rise_Up;
            gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_33E_Reverse_Magic_Hiss, MAX_VOLUME, NULL, NULL, 0, NULL);
            objdata->delayTimer = 0;
        }
        break;
    case ECShrine_STATE_Cup_Game_Start:
        objdata->state = ECShrine_STATE_Cups_Round_1;
        objdata->musicPlayTimer = 80;
        objdata->substate = Cup_STATE_Round_Start;
        objdata->delayTimer = 40;
        objdata->shuffles = 16;
        objdata->spiritCup = rand_next(0, 5);
        gDLL_3_Animation->vtbl->func17(3, self, -1);
        break;
    case ECShrine_STATE_Cups_Round_1:
    case ECShrine_STATE_Cups_Round_2:
    case ECShrine_STATE_Cups_Round_3:
        if (objdata->delayTimer > 0) {
            objdata->delayTimer--; //@bug: framerate dependent
            if (objdata->delayTimer < 0) {
                objdata->delayTimer = 0;
            }
        } else {
            switch (objdata->substate) {
            case Cup_STATE_Round_Start:
                objdata->substate = Cup_STATE_Shuffle;
                objdata->delayTimer = 40;
                objdata->musicPlayTimer = 60;
                break;
            case Cup_STATE_Round_End:
                objdata->substate = Cup_STATE_Round_Start;
                objdata->delayTimer = 40;
                objdata->musicPlayTimer = 60;
                break;
            case Cup_STATE_Sink_Down:
                objdata->substate = Cup_STATE_Underground;
                objdata->delayTimer = 40;
                objdata->musicPlayTimer = 60;
                break;
            case Cup_STATE_Shuffle:
                //Shuffle the cups
                objdata->shuffles--;
                if (objdata->shuffles <= 0) {
                    objdata->substate = Cup_STATE_Await_Choice;
                    objdata->choiceTimeout = 1000;
                } else {
                    objdata->substate = Cup_STATE_Stopped;
                    objdata->delayTimer = 2;

                    //Pick one of 8 different ways of shuffling cups (some options only used in later rounds)
                    if (objdata->state == ECShrine_STATE_Cups_Round_1) {
                        i = rand_next(0, 1);
                    } else if (objdata->state == ECShrine_STATE_Cups_Round_2) {
                        i = rand_next(0, 5);
                    } else {
                        i = rand_next(0, 7);
                    }

                    //Perform the chosen shuffle
                    if (i == 0) {
                        //Swap all cup slots clockwise
                        for (i = 0; i < 6; i++) {
                            dCupSlots[i]++;
                            if (dCupSlots[i] > 5) {
                                dCupSlots[i] = 0;
                            }
                        }
                    } else if (i == 1) {
                        //Swap all cup slots anticlockwise
                        for (i = 0; i < 6; i++) {
                            dCupSlots[i]--;
                            if (dCupSlots[i] < 0) {
                                dCupSlots[i] = 5;
                            }
                        }
                    } else if (i == 2) {
                        //Swap slots 0, 2, 4 clockwise
                        tempCupIndex = dCupSlots[0];
                        dCupSlots[0] = dCupSlots[2];
                        dCupSlots[2] = dCupSlots[4];
                        dCupSlots[4] = tempCupIndex;
                    } else if (i == 3) {
                        //Swap slots 0, 2, 4 anticlockwise
                        tempCupIndex = dCupSlots[4];
                        dCupSlots[4] = dCupSlots[0];
                        dCupSlots[0] = dCupSlots[2];
                        dCupSlots[2] = tempCupIndex;
                    } else if (i == 4) {
                        //Swap slots 1, 3, 5 clockwise
                        tempCupIndex = dCupSlots[1];
                        dCupSlots[1] = dCupSlots[3];
                        dCupSlots[3] = dCupSlots[5];
                        dCupSlots[5] = tempCupIndex;
                    } else if (i == 5) {
                        //Swap slots 1, 3, 5 anticlockwise
                        tempCupIndex = dCupSlots[5];
                        dCupSlots[5] = dCupSlots[1];
                        dCupSlots[1] = dCupSlots[3];
                        dCupSlots[3] = tempCupIndex;
                    } else if (i == 6) {
                        //Swap slots 1, 2, 4, 5 clockwise (using coords rather than index)
                        tempCupX = dCupCoords[1].x;
                        tempCupZ = dCupCoords[1].y;
                        dCupCoords[1].x = dCupCoords[2].x;
                        dCupCoords[1].y = dCupCoords[2].y;
                        dCupCoords[2].x = dCupCoords[4].x;
                        dCupCoords[2].y = dCupCoords[4].y;
                        dCupCoords[4].x = dCupCoords[5].x;
                        dCupCoords[4].y = dCupCoords[5].y;
                        dCupCoords[5].x = tempCupX;
                        dCupCoords[5].y = tempCupZ;
                    } else if (i == 7) {
                        //Swap slots 1, 2, 4, 5 anticlockwise (using coords rather than index)
                        tempCupX = dCupCoords[5].x;
                        tempCupZ = dCupCoords[5].y;
                        dCupCoords[5].x = dCupCoords[4].x;
                        dCupCoords[5].y = dCupCoords[4].y;
                        dCupCoords[4].x = dCupCoords[2].x;
                        dCupCoords[4].y = dCupCoords[2].y;
                        dCupCoords[2].x = dCupCoords[1].x;
                        dCupCoords[2].y = dCupCoords[1].y;
                        dCupCoords[1].x = tempCupX;
                        dCupCoords[1].y = tempCupZ;
                    }
                }
                break;
            case Cup_STATE_Stopped:
                objdata->substate = Cup_STATE_Moving;
                objdata->delayTimer = 12;
                return;
            case Cup_STATE_Moving:
                objdata->substate = Cup_STATE_Move_Finished;
                objdata->delayTimer = 0;
                return;
            case Cup_STATE_Move_Finished:
                objdata->substate = Cup_STATE_Shuffle;
                objdata->delayTimer = 0;
                return;
            case Cup_STATE_Await_Choice:
                if (objdata->choiceWasCorrect == FALSE) {
                    objdata->state = ECShrine_STATE_Test_Failure;
                    gDLL_5_AMSEQ->vtbl->play_ex(3, 0x35, 0x50, (u8) objdata->musCorridorVol, 0);
                    objdata->musCorridorVolSpeed = 1;
                    gDLL_3_Animation->vtbl->func17(2, self, -1);
                    objdata->musicPlayTimer = 10;
                    objdata->substate = Cup_STATE_Sink_Down;
                    gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_33E_Reverse_Magic_Hiss, MAX_VOLUME, NULL, NULL, 0, NULL);
                } else if (objdata->choiceWasCorrect == TRUE) {
                    if (objdata->state == ECShrine_STATE_Cups_Round_1) {
                        //Go to round 2
                        objdata->spiritCup = rand_next(0, 5);
                        objdata->state = ECShrine_STATE_Cups_Round_2;
                        objdata->substate = Cup_STATE_Round_End;
                        objdata->musicPlayTimer = 150;
                        objdata->delayTimer = 12;
                        objdata->shuffles = 10;
                        objdata->choiceWasCorrect = NO_CHOICE_YET;
                        gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_344_Chime, MAX_VOLUME, NULL, NULL, 0, NULL);
                        gDLL_3_Animation->vtbl->func17(3, self, -1);
                    } else if (objdata->state == ECShrine_STATE_Cups_Round_2) {
                        //Go to round 3
                        objdata->spiritCup = rand_next(0, 5);
                        objdata->state = ECShrine_STATE_Cups_Round_3;
                        objdata->substate = Cup_STATE_Round_End;
                        objdata->musicPlayTimer = 150;
                        objdata->delayTimer = 12;
                        objdata->shuffles = 16;
                        objdata->choiceWasCorrect = NO_CHOICE_YET;
                        gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_344_Chime, MAX_VOLUME, NULL, NULL, 0, NULL);
                        gDLL_3_Animation->vtbl->func17(3, self, -1);
                    } else {
                        //Test won!
                        objdata->musicPlayTimer = 130;
                        objdata->state = ECShrine_STATE_Test_Successful;
                        objdata->choiceWasCorrect = 0;
                        objdata->substate = Cup_STATE_Sink_Down;
                        gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_344_Chime, MAX_VOLUME, NULL, NULL, 0, NULL);
                        gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_33E_Reverse_Magic_Hiss, MAX_VOLUME, NULL, NULL, 0, NULL);
                    }
                } else {
                    //Automatically win if the player takes too long choosing (@bug? Possibly supposed to fail instead)
                    objdata->choiceTimeout -= gUpdateRate;
                    if (objdata->choiceTimeout <= 0) {
                        objdata->musicPlayTimer = 130;
                        objdata->state = ECShrine_STATE_Test_Successful;
                        objdata->choiceWasCorrect = FALSE;
                        objdata->substate = Cup_STATE_Sink_Down;
                        gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_344_Chime, MAX_VOLUME, NULL, NULL, 0, NULL);
                        gDLL_6_AMSFX->vtbl->play_sound(NULL, SOUND_33E_Reverse_Magic_Hiss, MAX_VOLUME, NULL, NULL, 0, NULL);
                    }
                }
                break;
            }
        }
        break;
    case ECShrine_STATE_Test_Successful:
        //Check if player has Spirit #2 (Diamond Bay: Test of Strength) @bug?: should check for Spirit #4?
        if (((DLL_210_Player*)player->dll)->vtbl->func38(player, SPIRIT_NUMBER(2)) != 0) {
            objdata->musCorridorVol = 1;
            gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, (u8) objdata->musCorridorVol, 0);
            objdata->musCorridorVolSpeed = 1;
            main_set_bits(BIT_DB_Entered_Shrine_3, 1);
            objdata->state = ECShrine_STATE_Warp_Away;
        } else {
            main_set_bits(BIT_DB_Entered_Shrine_1, 0);
            gDLL_5_AMSEQ->vtbl->play_ex(3, 0x2C, 0x50, (u8) objdata->musCorridorVol, 0);
            objdata->musCorridorVolSpeed = 1;
            gDLL_3_Animation->vtbl->func17(1, self, -1);
            objdata->state = ECShrine_STATE_Grant_Spirit;
        }
        break;
    case ECShrine_STATE_Grant_Spirit:
        main_set_bits(BIT_143, 0);
        objdata->state = ECShrine_STATE_Warp_Away;
        ((DLL_210_Player*)player->dll)->vtbl->func39(player, SPIRIT_NUMBER(4), 1);
        gDLL_29_Gplay->vtbl->set_map_setup(MAP_WARLOCK_MOUNTAIN, 5);
        break;
    case ECShrine_STATE_Warp_Away:
        if (main_get_bits(BIT_Shrine_Do_Exit_Warp) == 0) {
            main_set_bits(BIT_Shrine_Do_Exit_Warp, 1);
        }
        main_set_bits(BIT_DB_Entered_Shrine_2, 0);
        main_set_bits(BIT_DB_Entered_Shrine_3, 0);
        objdata->state = ECShrine_STATE_Finished;
        break;
    case ECShrine_STATE_Test_Failure:
        //Reset shrine
        objdata->state = ECShrine_STATE_Waiting;
        objdata->unk10 = 0;
        objdata->shuffles = 0;
        objdata->substate = Cup_STATE_Stopped;
        objdata->choiceWasCorrect = NO_CHOICE_YET;
        objdata->delayTimer = 0;
        objdata->spiritCup = 0;
        objdata->seqValue = 0;
        objdata->musicPlayTimer = 400;
        
        main_set_bits(BIT_DB_Entered_Shrine_3, 1);
        main_set_bits(BIT_DB_Entered_Shrine_1, 1);
        main_set_bits(BIT_DB_Entered_Shrine_2, 1);

        modGfxDLL = dll_load_deferred(DLL_ID_122, 1);
        objdata->modGfxCircle = modGfxDLL->vtbl->func0(self, 2, NULL, 0x402, -1, NULL);
        dll_unload(modGfxDLL);
        break;
    case ECShrine_STATE_Finished:
        break;
    }
}

// offset: 0x1320 | func: 2 | export: 2
void ECSHshrine_update(Object *self) { }

// offset: 0x132C | func: 3 | export: 3
void ECSHshrine_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x1380 | func: 4 | export: 4
void ECSHshrine_free(Object *self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_13);
    gDLL_14_Modgfx->vtbl->func5(self);
    gDLL_5_AMSEQ->vtbl->set_volume(3, 0);
    gDLL_5_AMSEQ->vtbl->set_volume(2, 0);
}

// offset: 0x1430 | func: 5 | export: 5
u32 ECSHshrine_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1440 | func: 6 | export: 6
u32 ECSHshrine_get_data_size(Object *self, u32 a1) {
    return sizeof(ECSHshrine_Data);
}

// offset: 0x1454 | func: 7 | export: 7
void ECSHshrine_func_1454(s16* arg0) {
    ECSHshrine_Data* objdata;
    Object *object;

    object = dShrine;
    if (object != NULL) {
        objdata = dShrine->data;
        *arg0 = objdata->unk10;
    }
}

// offset: 0x1484 | func: 8 | export: 8
/** 
  * Exports the x and z coords of a particular cup index.
  */
void ECSHshrine_get_cup_coords(u8 cupIndex, f32* x, f32* z) {
    f32* xzComponents = (f32*)dCupCoords;
    if (dShrine != NULL) {
        *x = xzComponents[(dCupSlots[cupIndex] * 2)];
        *z = xzComponents[(dCupSlots[cupIndex] * 2) + 1];
    }
}

// offset: 0x14F4 | func: 9 | export: 9
/** 
  * Exports the cup minigame's substate value, and the index of the cup currently holding the Krazoa Spirit.
  */
void ECSHshrine_get_minigame_state(s32* cupSubstate, u8* cupWithSpirit) {
    Object *object;
    ECSHshrine_Data* objdata;

    object = dShrine;
    if (object != NULL) {
        objdata = dShrine->data;
        *cupWithSpirit = objdata->spiritCup;
        *cupSubstate = objdata->substate;
    }
}

// offset: 0x152C | func: 10 | export: 10
/** 
  * Sets the x and z coords of a particular cup.
  */
void ECSHshrine_set_cup_coords(u8 cupIndex, f32 x, f32 z) {
    f32* xzComponents;
    
    if (dShrine != NULL) {
        xzComponents = (f32*)dCupCoords;
        xzComponents[(dCupSlots[cupIndex] * 2)] = x;
        xzComponents[(dCupSlots[cupIndex] * 2) + 1] = z;
    }
}

// offset: 0x1590 | func: 11 | export: 11
/*
 * Used when player touches a cup.
 */
void ECSHshrine_choose_cup(u8 chosenCupIndex) {
    Object *self;
    ECSHshrine_Data *objdata;

    self = dShrine;
    if (self != NULL) {
        self = dShrine;
        objdata = dShrine->data;
        objdata->choiceWasCorrect = chosenCupIndex == objdata->spiritCup ? TRUE : FALSE;
    }
}

// offset: 0x15E0 | func: 12
static int ECSHshrine_anim_callback(Object *self, Object *a1, AnimObj_Data *aData, s8 a3) {
    ECSHshrine_Data *objdata;
    s32 i;

    objdata = self->data;
    aData->unk7A = -1;
    aData->unk62 = 0;
    if (objdata->musCorridorVolSpeed != 0) {
        objdata->musCorridorVol += objdata->musCorridorVolSpeed;
        if ((objdata->musCorridorVol <= 1) && (objdata->musCorridorVolSpeed <= 0)) {
            objdata->musCorridorVol = 1;
            objdata->musCorridorVolSpeed = 0;
        } else if ((objdata->musCorridorVol >= 70) && (objdata->musCorridorVolSpeed >= 0)) {
            objdata->musCorridorVol = 70;
            objdata->musCorridorVolSpeed = 0;
        }
        gDLL_5_AMSEQ->vtbl->set_volume(3, objdata->musCorridorVol);
    }

    for (i = 0; i < aData->unk98; i++) {
        if (aData->unk8E[i] != 0) {
            switch (aData->unk8E[i]) {
            case 1:
                func_80000860(self, self, 0xCB, 0);
                break;
            case 2:
                if (D_80092A7C[0] == -1) {
                    func_80000860(self, self, 0x14, 0);
                } else {
                    func_80000860(self, self, D_80092A7C[0], 0);
                }
                break;
            case 3:
                objdata->seqValue = 1;
                break;
            case 5:
                main_set_bits(BIT_143, 0);
                objdata->seqValue = 2;
                main_set_bits(BIT_DB_Entered_Shrine_3, 1);
                break;
            case 6:
                objdata->state = ECShrine_STATE_Grant_Spirit;
                objdata->seqValue = 3;
                main_set_bits(BIT_DB_Entered_Shrine_3, 1);
                break;
            case 7:
                main_set_bits(BIT_143, 1);
                break;
            case 8:
                main_set_bits(BIT_143, 0);
                objdata->musCorridorVolSpeed = -3;
                break;
            case 10:
                main_set_bits(BIT_DB_Triggered_In_Shrine_Spirit_Cutscene, 1);
                if (dTexture == NULL) {
                    dTexture = func_8004A1E8(1);
                }
                break;
            case 9:
                main_set_bits(BIT_DB_Entered_Shrine_2, 1);
                break;
            case 11:
                objdata->musCorridorVol = 100;
                gDLL_5_AMSEQ->vtbl->play_ex(3, 0x30, 0x50, (u8) objdata->musCorridorVol, 0);
                break;
            case 13:
                gDLL_3_Animation->vtbl->func19(0x5A, 0x64, 0, 0x50);
                break;
            }
        }
        aData->unk8E[i] = 0;
    }
    return 0;
}

// offset: 0x193C | func: 13
static void ECSHshrine_handle_messages(Object *self) {
    Object *sender;
    u32 mesgID;
    void* mesgArg;
    ECSHshrine_Data *objdata;

    objdata = self->data;
    mesgArg = NULL;
    while (obj_recv_mesg(self, &mesgID, &sender, &mesgArg) != 0) {
        switch (mesgID) {
        case 0x30005:
            objdata->musWhipersVolSpeed = -3;
            break;
        case 0x30006:
            objdata->musWhipersVolSpeed = 16;
            break;
        }
    }
}
