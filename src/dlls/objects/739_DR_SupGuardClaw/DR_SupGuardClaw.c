#include "common.h"
#include "macros.h"
#include "PR/os.h"
#include "sys/objhits.h"
#include "sys/objtype.h"
#include "game/objects/interaction_arrow.h"
#include "dlls/engine/53.h"
#include "dlls/objects/common/sidekick.h"
#include "dlls/objects/210_player.h"

typedef struct {
    s16 soundID;
    s16 unk2;
} TalkSounds;

typedef struct {
    ObjSetup base;
    s8 yaw;
    s8 characterType;    //0: SharpClaw, 1: GuardClaw
    s16 unused1A;
    s16 unused1C;
    s16 gamebitFinished; //Gamebit set when the character's interactions are finished (SharpClaw fed, GuardClaw tricked)
} DR_NPC_Setup;

typedef struct {
    u8 _unk0[0x4A9 - 0];
    u8 unk4A9;
    u8 _unk4AA[0x4DC - 0x4AA];
    HeadAnimation headAnimLook;
    HeadAnimation headAnimTalk;
    UnkFunc_80024108Struct animData;
    TalkSounds* soundsTalk;                 //Character sounds played at intervals
    s32 (*behaviourFunction)(Object* self); //Character-specific behaviour called from the control function. Returns TRUE when character's interactions are finished.
    s16* soundsAttack;                      //Sounds to play when attacking (only used by GuardClaw)
    s16* modAnims;                          //An array of modAnimIDs
    f32 animSpeed;                          //Animation playback speed
    s16 talkTimer;                          //Randomised talk countdown timer
    u16 finished : 1;                       //Character sets gamebit and disappears afterwards
} DR_NPC_Data;

typedef enum {
    DR_NPC_SharpClaw,
    DR_NPC_GuardClaw
} DR_NPC_CharacterTypes;

/*0x0*/ static s16 dSoundsGuardClawAttack[] = {
    SOUND_35E_SharpClaw_Staff_Swing, 
    SOUND_35D_GuardClaw_Yawn, 
    SOUND_35F_GuardClaw_Step, 
    0
};

/*0x8*/ static TalkSounds dSoundsGuardClaw[] = {
    {SOUND_361_GuardClaw_Go_Away, 0x1000},
    {SOUND_362_GuardClaw_Hah,     0x1000}
};

/*0x10*/ static TalkSounds dSoundsSharpClaw[] = {
    {SOUND_236_SharpClaw_Argh,    0x1000},
    {SOUND_239_SharpClaw_Snort,   0x1000}
};

/*0x18*/ static s16 dModAnimsSharpClaw[] = {
    0, 
    0x206, 
    0x127, 
    0, 
    0x30a, 
    0x0
};

//TODO: finish and move to GuardClaw header, or a separate header just for a model's modanim?
typedef enum {
    //Bank 0
    GuardClaw_MODANIM_0_Step_Right,
    GuardClaw_MODANIM_1_Step_Left,
    GuardClaw_MODANIM_2,
    GuardClaw_MODANIM_3,
    GuardClaw_MODANIM_4_Idle_LOOP,
    GuardClaw_MODANIM_5_Looking_Around,
    GuardClaw_MODANIM_6_Yawn,
    GuardClaw_MODANIM_7,
    GuardClaw_MODANIM_8_Staff_Swing_Right,
    GuardClaw_MODANIM_9_Staff_Swing_Left,
    GuardClaw_MODANIM_10,
    GuardClaw_MODANIM_11,
    GuardClaw_MODANIM_12,
    GuardClaw_MODANIM_13,
    GuardClaw_MODANIM_14,
    GuardClaw_MODANIM_15,
    GuardClaw_MODANIM_16,
    GuardClaw_MODANIM_17,
    GuardClaw_MODANIM_18,
    GuardClaw_MODANIM_19,
    GuardClaw_MODANIM_20,
    GuardClaw_MODANIM_21,

    //Bank 1
    GuardClaw_MODANIM_23 = 23,
    GuardClaw_MODANIM_24,
    GuardClaw_MODANIM_25,
    GuardClaw_MODANIM_26,
    GuardClaw_MODANIM_27,
    GuardClaw_MODANIM_28,
    GuardClaw_MODANIM_29,
    GuardClaw_MODANIM_30,
    GuardClaw_MODANIM_31
} GuardClaw_ModAnim;

/*0x24*/ static s16 dModAnimsGuardClaw[] = {
    GuardClaw_MODANIM_4_Idle_LOOP,
    GuardClaw_MODANIM_5_Looking_Around,
    GuardClaw_MODANIM_1_Step_Left,
    GuardClaw_MODANIM_8_Staff_Swing_Right, 
    GuardClaw_MODANIM_6_Yawn, 
    GuardClaw_MODANIM_0_Step_Right
};

static int DR_NPC_anim_callback(Object* self, Object* objSetup, AnimObj_Data* animData);
static s32 DR_NPC_sharpclaw_behaviour(Object* self);
static s32 DR_NPC_guardclaw_behaviour(Object* self);
static void DR_NPC_play_sounds(Object* self, UnkFunc_80024108Struct* objAnimData, s16* soundIDs);

// offset: 0x0 | ctor
void DR_NPC_ctor(void *dll) { }

// offset: 0xC | dtor
void DR_NPC_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void DR_NPC_setup(Object* self, DR_NPC_Setup* objSetup, s32 reset) {
    DR_NPC_Data* objData = self->data;
    u16 dDLL53Array1[] = {30, 0, 0};
    u16 dDLL53Array2[] = {25, 25, 25};

    self->animCallback = (void*)DR_NPC_anim_callback;
    self->srt.yaw = objSetup->yaw << 8;

    //Check if character's interactions already finished
    if (main_get_bits(objSetup->gamebitFinished)) {
        objData->finished = TRUE;
    }

    //Set up character-specific data
    if (objSetup->characterType == DR_NPC_SharpClaw) { 
        //SharpClaw (in imprisoned EarthWalker's cave)
        objData->modAnims = dModAnimsSharpClaw;
        objData->behaviourFunction = DR_NPC_sharpclaw_behaviour;
        objData->soundsAttack = NULL;
        objData->soundsTalk = dSoundsSharpClaw;
    } else {
        //GuardClaw (outside CloudRunner prison)
        objData->modAnims = dModAnimsGuardClaw;
        objData->behaviourFunction = DR_NPC_guardclaw_behaviour;
        objData->soundsAttack = dSoundsGuardClawAttack;
        objData->soundsTalk   = dSoundsGuardClaw;
        obj_add_object_type(self, OBJTYPE_4);

        //Hide self and stop updating after Illusion Spell used
        if (objData->finished) {
            obj_free_tick(self);
            self->srt.flags |= OBJFLAG_INVISIBLE;
        }

        //ObjHits
        func_80028D2C(self);
    }

    //Start playing one of two idle animations, picked at random
    objData->animSpeed = 0.01f;
    func_80023D30(self, objData->modAnims[rand_next(0, 1)], 0.0f, 0);

    self->stateFlags |= OBJSTATE_UPDATE_DISABLED;

    create_temp_dll(DLL_ID_53);
    ((DLL_53*)(gTempDLLInsts[1]))->vtbl->func2(self, objData, -0x11C7, 0x3554, 3);
    ((DLL_53*)(gTempDLLInsts[1]))->vtbl->func5(objData, 300, 120);
    ((DLL_53*)(gTempDLLInsts[1]))->vtbl->func6(objData, dDLL53Array2, dDLL53Array1, 3);

    objData->unk4A9 |= 10;
}

// offset: 0x290 | func: 1 | export: 1
void DR_NPC_control(Object* self) {
    DR_NPC_Data* objData;
    DR_NPC_Setup* objSetup;
    s16 temp;
    s16 animIndex;
    Object* sidekick;
    f32 distance;

    objData = self->data;
    objSetup = (DR_NPC_Setup*)self->setup;

    distance = 200.0f;

    //Run character-specific behaviour, and set a gamebit when character's task finished (SharpClaw fed, GuardClaw tricked)
    if ((objData->finished == FALSE) && objData->behaviourFunction(self)) {
        main_set_bits(objSetup->gamebitFinished, 1);
        objData->finished = TRUE;
    }

    //Get the difference between the character's setup yaw and current yaw
    temp = (objSetup->yaw << 8) - (self->srt.yaw & 0xFFFF);
    CIRCLE_WRAP(temp);

    //Rotate back towards the setup yaw if needed, playing a stepping animation
    if (temp != 0) {
        diPrintf(" YAW DIFF ");
        if (self->curModAnimId != objData->modAnims[2]) {
            func_80023D30(self, objData->modAnims[2], 0, 0);
        }
        self->srt.yaw += (temp + 1) >> 4;
        objData->animSpeed = (temp / 1024) * 0.01f;

        //Return to idle animation when rotation finished
        if (((temp >= 0) ? temp : -temp) < 0x400) {
            self->srt.yaw = objSetup->yaw << 8;
            func_80023D30(self, objData->modAnims[rand_next(0, 1)], 0, 0);
            objData->animSpeed = 0.01f;
        }
    }

    //Talk at random intervals
    if ((objData->talkTimer -= gUpdateRate) < 0) {
        objData->talkTimer = rand_next(50, 500);
        func_80034B54(self, &objData->headAnimTalk, &objData->soundsTalk[rand_next(0, 1)].soundID, 0);
    }

    //Pick the next animation to play after the current one ends
    if (func_80024108(self, objData->animSpeed, gUpdateRateF, &objData->animData)) {
        //87.5% chance of continuing idle
        if (rand_next(0, 7)) {
            temp = 0;
        } else {
            if (rand_next(0, 1)) {
                //6.25% chance of looking around (or in SharpClaw's case: huddling)
                animIndex = 1;
            } else {
                //6.25% chance of yawning (or in SharpClaw's case: head-scratch)
                animIndex = 4;
            }
            temp = animIndex;
        }
        func_80023D30(self, objData->modAnims[temp], 0, 0);

        //Set animation playback speed
        if (temp == 0) {
            objData->animSpeed = 0.01f;
        } else {
            objData->animSpeed = 0.005f;
        }
    }

    DR_NPC_play_sounds(self, &objData->animData, objData->soundsAttack);
    ((DLL_53*)gTempDLLInsts[1])->vtbl->func0(self, objData);
    func_80032A08(self, &objData->headAnimLook);
    func_80034BC0(self, &objData->headAnimTalk);

    //Show Distract command option
    sidekick = obj_get_nearest_type_to(OBJTYPE_SIDEKICK, self, &distance);
    if (sidekick != NULL) {
        ((DLL_ISidekick*)sidekick->dll)->vtbl->func14(sidekick, Sidekick_Command_INDEX_2_Distract);
    }
}

// offset: 0x68C | func: 2 | export: 2
void DR_NPC_update(Object *self) { }

// offset: 0x698 | func: 3 | export: 3
void DR_NPC_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    DR_NPC_Data* objData = self->data;

    if (visibility) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
        ((DLL_53*)(gTempDLLInsts[1]))->vtbl->func3(self, objData, 0);
    }
}

// offset: 0x71C | func: 4 | export: 4
void DR_NPC_free(Object* self, s32 onlySelf) {
    DR_NPC_Setup* objSetup = (DR_NPC_Setup*)self->setup;

    remove_temp_dll(DLL_ID_53);

    if (objSetup->characterType) {
        obj_free_object_type(self, OBJTYPE_4);
    }
}

// offset: 0x790 | func: 5 | export: 5
u32 DR_NPC_get_model_flags(Object *self) {
    return MODFLAGS_EVENTS | MODFLAGS_SHADOW | MODFLAGS_1;
}

// offset: 0x7A0 | func: 6 | export: 6
u32 DR_NPC_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DR_NPC_Data);
}

// offset: 0x7B4 | func: 7
int DR_NPC_anim_callback(Object* self, Object*arg1, AnimObj_Data* animData) {
    DR_NPC_Data* objData;
    DR_NPC_Setup* objSetup;
    Object* player;
    s16 modAnimID;
    s32 foodGamebit;
    s32 i;

    objData = self->data;
    player = get_player();
    objSetup = (DR_NPC_Setup*)self->setup;

    func_8002674C(self);
    func_80028D2C(self);

    //GuardClaw sequence subevents
    for (i = 0; i < animData->messageCount; i++) {
        if ((animData->messages[i] == 1) && (objSetup->characterType != DR_NPC_SharpClaw)) {
            obj_free_tick(self);
            func_800267A4(self);
            self->srt.flags |= OBJFLAG_INVISIBLE;
        }
    }

    modAnimID = objData->modAnims[2];
    if (((DLL_53*)(gTempDLLInsts[1]))->vtbl->func4(self, animData, objData, modAnimID, modAnimID) != 0) {
        return 1;
    }

    //Feeding the SharpClaw
    foodGamebit = ((DLL_210_Player*)player->dll)->vtbl->func74(player);
    if (foodGamebit != NO_GAMEBIT) {
        if ((foodGamebit == BIT_Dino_Egg_Count) || (foodGamebit == BIT_Moldy_Meat_Count)) {
            STUBBED_PRINTF(" Edanble FoodType %i ", foodGamebit);
            ((DLL_210_Player*)player->dll)->vtbl->func75(player, 1);
            
            main_set_bits(objSetup->gamebitFinished, 1);
            STUBBED_PRINTF(" \n Have Set Bit %i \n", objSetup->gamebitFinished);

            gDLL_3_Animation->vtbl->end_obj_sequence(self->seqSlot);
            return 0;
        } else {
            STUBBED_PRINTF(" FoodType %i ", foodGamebit);
            ((DLL_210_Player*)player->dll)->vtbl->func75(player, 2);
        }
    }

    return 0;
}

// offset: 0x9C8 | func: 8
s32 DR_NPC_sharpclaw_behaviour(Object* self) {
    Object* player;
    s32 foodGamebit;

    player = get_player();

    //Talking to the SharpClaw
    if (self->unkAF & ARROW_FLAG_1_Interacted) {
        joy_set_button_mask(0, A_BUTTON);
        if (gDLL_1_cmdmenu->vtbl->was_any_item_used() == 0) {
            gDLL_3_Animation->vtbl->start_obj_sequence(0, self, -1);
        }
        return FALSE;
    }

    //Giving food to the SharpClaw
    foodGamebit = ((DLL_210_Player*)player->dll)->vtbl->func74(player);
    if (foodGamebit != NO_GAMEBIT) {
        if ((foodGamebit == BIT_Dino_Egg_Count) || (foodGamebit == BIT_Moldy_Meat_Count)) {
            STUBBED_PRINTF(" Edanble FoodType %i ", foodGamebit);
            ((DLL_210_Player*)player->dll)->vtbl->func75(player, 1);
            return TRUE;
        } else {
            STUBBED_PRINTF(" FoodType %i ", foodGamebit);
            ((DLL_210_Player*)player->dll)->vtbl->func75(player, 2);
        }
    }

    return FALSE;
}

// offset: 0xB04 | func: 9
s32 DR_NPC_guardclaw_behaviour(Object* self) {
    Object* player;
    Object* sidekick;
    DR_NPC_Data* objData;

    player = get_player();
    sidekick = get_sidekick();
    objData = self->data;

    //Return TRUE when Illusion Spell is used
    if (((DLL_210_Player*)player->dll)->vtbl->func50(player) == BIT_Spell_Illusion) {
        return TRUE;
    }

    //Talking to GuardClaw
    if ((self->unkAF & ARROW_FLAG_1_Interacted) && (gDLL_1_cmdmenu->vtbl->was_any_item_used() == FALSE)) {
        joy_set_button_mask(0, A_BUTTON);
        self->objhitInfo->unk5F = 11;
        self->objhitInfo->unk60 = 4;
        gDLL_3_Animation->vtbl->start_obj_sequence(rand_next(0, 1), self, -1);
    }

    //Attack when sidekick or player nearby
    if (((sidekick && (vec3_distance_xz(&self->globalPosition, &sidekick->globalPosition) < 40.0f)) ||
         (player && (vec3_distance_xz(&self->globalPosition, &player->globalPosition) < 40.0f)))
        && (self->curModAnimId != GuardClaw_MODANIM_9_Staff_Swing_Left)
    ) {
        func_80023D30(self, GuardClaw_MODANIM_9_Staff_Swing_Left, 0.0f, 0);
        objData->animSpeed = 0.006f;
        if (sidekick != NULL) {
            ((DLL_ISidekick*)sidekick->dll)->vtbl->func21(sidekick, 0, 0);
        }
    }

    //Attacking
    if (self->curModAnimId == GuardClaw_MODANIM_9_Staff_Swing_Left) {
        self->objhitInfo->unk5F = 11;
        self->objhitInfo->unk60 = 4;
        func_80026128(self, Damage_Type_Sword_Staff_Strike2, 4, 7);
        func_80028D2C(self);
    }

    return FALSE;
}

// offset: 0xD78 | func: 10
void DR_NPC_play_sounds(Object* self, UnkFunc_80024108Struct* objAnimData, s16* soundIDs) {
    u8 bits;
    s32 i;

    bits = 0;
    for (i = 0; i < objAnimData->unk1B; i++) {
        switch (objAnimData->unk13[i]) {
        case 0:
            if (soundIDs != NULL) {
                gDLL_6_AMSFX->vtbl->play(self, soundIDs[0], MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            break;
        case 7:
            if (soundIDs != NULL) {
                gDLL_6_AMSFX->vtbl->play(self, soundIDs[1], MAX_VOLUME, NULL, NULL, 0, NULL);
            }
            break;
        case 1:
            bits |= 1;
            break;
        case 2:
            bits |= 2;
            break;
        case 3:
            bits |= 4;
            break;
        case 4:
            bits |= 8;
            break;
        default:
            break;
        }
    }

    //Play a sound if case 1/2/3/4 were reached (only the GuardClaw uses this, and the soundID is empty)
    if (bits && (soundIDs != NULL)) {
        gDLL_6_AMSFX->vtbl->play(self, soundIDs[3], MAX_VOLUME, NULL, NULL, 0, NULL);
    }
}
