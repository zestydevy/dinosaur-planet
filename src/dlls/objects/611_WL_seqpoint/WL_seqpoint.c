#include "dlls/objects/210_player.h"
#include "game/gamebits.h"
#include "sys/gfx/animseq.h"
#include "sys/map_enums.h"
#include "sys/objects.h"
#include "sys/objprint.h"
#include "sys/print.h"
#include "macros.h"
#include "dll.h"

typedef struct {
/*00*/ ObjSetup base;
/*18*/ s8 rotation; // yaw >> 8
/*19*/ s8 triggerCondition;
/*1A*/ s16 triggerRange;
/*1C*/ s16 seqno; // object seq index
/*1E*/ s16 conditionBit; // if not -1, bit that determines if the seqpoint is enabled. required value depends on the trigger condition
/*20*/ s16 triggeredBit; // if not -1, this bit is set to 1 once seqpoint is triggered. prevents re-trigger if bit is already 1
} WL_seqpoint_Setup;

typedef struct {
/*00*/ f32 triggerRange;
/*04*/ s16 conditionBit;
/*06*/ s16 triggeredBit;
/*08*/ s16 seqno;
/*0A*/ s16 unkA; // unused
/*0C*/ u8 lastMesg; // last seq message
/*0D*/ u8 triggered;
/*0E*/ s8 triggerCondition;
} WL_seqpoint_Data;

/*
- 0: 0x180 - spirit 1 release (krystal, act 2)
- 1: 0x17F - ? (unused)
- 2: 0x1A0 - sabre spies on sharpclaw with warp crystal (act 3)
- 3: 0x1A1 - sabre spies on sharpclaw stepping on pressure switch to open door (act 3)
- 4: 0x1B3 - randorn warns krystal of skeetlas (act 4)
- 5: 0x1B4 - krystal/randorn talk as the column to the main chamber brings krystal up (act 4)
- 6: 0x1B5 - spirit 3 release (krystal, act 4)
- 7: 0x1CB - sabre sees guardclaw guarding the column to the main chamber (act 5)
- 8: 0x1CC - ? (act 5)
- 9: 0x1CD - sabre calls out to randorn (act 5)
- 10: 0x1CE - spirit 4 release (sabre, act 5)
- 11: 0x1D3 - krystal finds randorn on the floor, randorn asks for food (act 6)
- 12: 0x1D4 - feeding randorn apple (act 6)
- 13: 0x1D6 - randorn explains how everyone arrived on dinosaur planet (act 6)
- 14: 0x1D1 - spirit 2 release (sabre, act 3)
- 15: 0x1D9 - spirit 5 release (krystal, act 6)
- 16: 0x20B - tricky senses something strange (act 7)
- 17: 0x20C - spirit 6 release (sabre, act 7)
- 18: 0x20D - spirit crystal tells sabre about EVIL (act 7)
- 19: 0x228 - spirit crystal tells sabre about EVIL again (act 7)
- 20: 0xE5 - randorn intro
- 21: 0x170 - DF spirit lore dump to randorn/krystal 
- 22: 0x296 - show krystal side orrery door (from switch in main chamber)
- 23: 0x2B6 - randorn walking to go sit down (act 2 with DF spirit)
- 24: 0x2B3 - feeding randorn apple (act 2)
- 25: 0x2B4 - feeding randorn apple (seq missing curves, unused)
- 26: 0x41E - WL_TransTop reveal (after sabre warp crystal)
- 27: 0xEF - scales escapes with kyte
- 28: 0x446 - look at randorn door from pressure switch
*/
enum WL_seqpoint_Sequence {
    WL_SEQPOINT_SEQ_0_Spirit_1_Release = 0,
    WL_SEQPOINT_SEQ_1 = 1,
    WL_SEQPOINT_SEQ_4_Randorn_Warns_Krystal_Of_Skeetlas = 4,
    WL_SEQPOINT_SEQ_5_Krystal_Randorn_Talk_About_Sabre = 5,
    WL_SEQPOINT_SEQ_6_Spirit_3_Release = 6,
    WL_SEQPOINT_SEQ_10_Spirit_4_Release = 10,
    WL_SEQPOINT_SEQ_11_Randorn_Asks_For_Food = 11,
    WL_SEQPOINT_SEQ_13_Randorn_Explains_Arrival = 13,
    WL_SEQPOINT_SEQ_14_Spirit_2_Release = 14,
    WL_SEQPOINT_SEQ_15_Spirit_5_Release = 15,
    WL_SEQPOINT_SEQ_16_Tricky_Senses_RockFall = 16,
    WL_SEQPOINT_SEQ_17_Spirit_6_Release = 17,
    WL_SEQPOINT_SEQ_18_SpiritCrystal_Talks_To_Sabre_1 = 18,
    WL_SEQPOINT_SEQ_19_SpiritCrystal_Talks_To_Sabre_2 = 19,
    WL_SEQPOINT_SEQ_21_DF_Spirit_Conversation = 21
};

static int WL_seqpoint_anim_callback(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 a3);

// offset: 0x0 | ctor
void WL_seqpoint_ctor(void *dll) { }

// offset: 0xC | dtor
void WL_seqpoint_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void WL_seqpoint_setup(Object* self, WL_seqpoint_Setup* setup, s32 reset) {
    WL_seqpoint_Data* objdata = self->data;
    self->animCallback = WL_seqpoint_anim_callback;
    self->srt.yaw = setup->rotation << 8;
    objdata->triggerRange = setup->triggerRange;
    objdata->seqno = setup->seqno;
    objdata->triggered = FALSE;
    objdata->triggerCondition = setup->triggerCondition;
    objdata->conditionBit = setup->conditionBit;
    objdata->triggeredBit = setup->triggeredBit;
    objdata->lastMesg = 0;
    objdata->unkA = 0;
}

// offset: 0x8C | func: 1 | export: 1
void WL_seqpoint_control(Object* self) {
    Object* player = objGetPlayer();
    WL_seqpoint_Data* objdata = self->data;
    
    if (objdata->triggeredBit != -1) {
        if (objdata->triggered) {
            if (mainGetBits(objdata->triggeredBit) == 0) {
                mainSetBits(objdata->triggeredBit, 1);
                objdata->triggered = TRUE;
            }
            return;
        } else {
            if (mainGetBits(objdata->triggeredBit) != 0) {
                objdata->triggered = TRUE;
                return;
            }
        }
    }

    if (!objdata->triggered) {
        switch (objdata->triggerCondition) {
        case 0: // range
            if (vec3_distance(&self->globalPosition, &player->globalPosition) < objdata->triggerRange) {
                gDLL_3_Animation->vtbl->start_obj_sequence(objdata->seqno, self, -1);
                objdata->triggered = TRUE;
            }
            break;
        case 1: // cond bit set
            if ((objdata->conditionBit != -1) && (mainGetBits(objdata->conditionBit) != 0)) {
                gDLL_3_Animation->vtbl->start_obj_sequence(objdata->seqno, self, -1);
                objdata->triggered = TRUE;
            }
            break;
        case 2: // range and cond bit set
            if (vec3_distance(&self->globalPosition, &player->globalPosition) < objdata->triggerRange) {
                if ((objdata->conditionBit != -1) && (mainGetBits(objdata->conditionBit) != 0)) {
                    gDLL_3_Animation->vtbl->start_obj_sequence(objdata->seqno, self, -1);
                    objdata->triggered = TRUE;
                }
            }
            break;
        case 3: // range and cond bit unset
            if (vec3_distance(&self->globalPosition, &player->globalPosition) < objdata->triggerRange) {
                if ((objdata->conditionBit != -1) && (mainGetBits(objdata->conditionBit) == 0)) {
                    STUBBED_PRINTF("starting seq %d\n", objdata->seqno); // printf location found via default.dol
                    gDLL_3_Animation->vtbl->start_obj_sequence(objdata->seqno, self, -1);
                    mainSetBits(objdata->conditionBit, 1);
                    objdata->triggered = TRUE;
                }
            }
            break;
        case 4: // cond bit unset
            if ((objdata->conditionBit != -1) && (mainGetBits(objdata->conditionBit) == 0)) {
                gDLL_3_Animation->vtbl->start_obj_sequence(objdata->seqno, self, -1);
                mainSetBits(objdata->conditionBit, 1);
                objdata->triggered = TRUE;
            }
            break;
        case 5: // cond bit set, repeat as long as bit is still set
            if ((objdata->conditionBit != -1) && (mainGetBits(objdata->conditionBit) != 0)) {
                gDLL_3_Animation->vtbl->start_obj_sequence(objdata->seqno, self, -1);
            }
            break;
        }
    }
}

// offset: 0x420 | func: 2 | export: 2
void WL_seqpoint_update(Object* self) { }

// offset: 0x42C | func: 3 | export: 3
void WL_seqpoint_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x480 | func: 4 | export: 4
void WL_seqpoint_free(Object* self, s32 onlySelf) { }

// offset: 0x490 | func: 5 | export: 5
u32 WL_seqpoint_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x4A0 | func: 6 | export: 6
u32 WL_seqpoint_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(WL_seqpoint_Data);
}

// offset: 0x4B4 | func: 7
static int WL_seqpoint_anim_callback(Object* actor, Object* animObj, AnimObj_Data* animObjData, s8 a3) {
    WL_seqpoint_Data* objdata = actor->data;
    Object* player = objGetPlayer();
    s32 i;
    
    animObjData->unk7A = -1;
    animObjData->unk62 = 0;
    // @bug: should be `i < animObjData->messageCount`. this loop will run 11 times, and write a 0 out of bounds
    //       of the messages array, setting messageCount to 0 and ending the loop rather than running infinitely.
    for (i = 0; animObjData->messageCount; i++) {
        switch (objdata->seqno) {
        case WL_SEQPOINT_SEQ_21_DF_Spirit_Conversation:
            switch (animObjData->messages[i]) {
            case 2:
                mainSetBits(BIT_125, 1);
                break;
            case 3:
                mainSetBits(BIT_125, 0);
                break;
            case 10:
                ((DLL_210_Player*)player->dll)->vtbl->set_spirit_bits(player, PLAYER_SPIRIT_1, TRUE);
                mainSetBits(BIT_Play_Seq_0170_WM_Return_to_Randorn_Quan_Ata_Lachu_Speaks, 1);
                break;
            }
            break;
        case WL_SEQPOINT_SEQ_19_SpiritCrystal_Talks_To_Sabre_2:
            if (animObjData->messages[i] != 0) {
                objdata->lastMesg = animObjData->messages[i];
                if (animObjData->messages[i] == 1) {
                    mainSetBits(objdata->conditionBit, 0);
                }
            }
            break;
        case WL_SEQPOINT_SEQ_18_SpiritCrystal_Talks_To_Sabre_1:
            if (animObjData->messages[i] != 0) {
                objdata->lastMesg = animObjData->messages[i];
                if (animObjData->messages[i] == 1) {
                    gDLL_29_Gplay->vtbl->set_act(MAP_CAPE_CLAW, 9);
                    mainSetBits(BIT_367, 1);
                    mainSetBits(BIT_368, 1);
                    mainSetBits(BIT_Play_Seq_020D, 0);
                }
            }
            break;
        case WL_SEQPOINT_SEQ_17_Spirit_6_Release:
            if (animObjData->messages[i] != 0) {
                objdata->lastMesg = animObjData->messages[i];
                switch (animObjData->messages[i]) {
                case 1:
                    mainSetBits(BIT_1D2, 1);
                    break;
                case 2:
                    mainSetBits(BIT_1D2, 0);
                    break;
                case 3:
                    mainSetBits(BIT_WM_Spirit_Release_Effect, 1);
                    break;
                case 4:
                    mainSetBits(BIT_WM_Spirit_Release_Effect, 0);
                    ((DLL_210_Player*)player->dll)->vtbl->set_spirit_bits(player, PLAYER_SPIRIT_7, FALSE);
                    mainSetBits(BIT_222, 1);
                    break;
                }
            }
            break;
        case WL_SEQPOINT_SEQ_16_Tricky_Senses_RockFall:
            if (animObjData->messages[i] != 0) {
                objdata->lastMesg = animObjData->messages[i];
            }
            switch (objdata->lastMesg) {
            case 1:
                diPrintf("\t\tWhat is it ? ...\n");
                break;
            case 2:
                diPrintf("\t\tI Can Feel Something Strange !\n");
                break;
            }
            break;
        case WL_SEQPOINT_SEQ_15_Spirit_5_Release:
            if (animObjData->messages[i] != 0) {
                objdata->lastMesg = animObjData->messages[i];
                switch (animObjData->messages[i]) {
                case 1:
                    mainSetBits(BIT_1CF, 1);
                    break;
                case 2:
                    mainSetBits(BIT_1CF, 0);
                    break;
                case 3:
                    mainSetBits(BIT_WM_Spirit_Release_Effect, 1);
                    break;
                case 4:
                    mainSetBits(BIT_WM_Spirit_Release_Effect, 0);
                    mainSetBits(BIT_221, 1);
                    ((DLL_210_Player*)player->dll)->vtbl->set_spirit_bits(player, PLAYER_SPIRIT_6, FALSE);
                    mainSetBits(BIT_31D, 1);
                    mainSetBits(BIT_31E, 1);
                    mainSetBits(BIT_31C, 1);
                    break;
                }
            }
            break;
        case WL_SEQPOINT_SEQ_14_Spirit_2_Release:
            if (animObjData->messages[i] != 0) {
                objdata->lastMesg = animObjData->messages[i];
                switch (animObjData->messages[i]) {
                case 1:
                    mainSetBits(BIT_125, 1);
                    break;
                case 2:
                    mainSetBits(BIT_125, 0);
                    break;
                case 3:
                    mainSetBits(BIT_WM_Spirit_Release_Effect, 1);
                    break;
                case 4:
                    mainSetBits(BIT_WM_Spirit_Release_Effect, 0);
                    mainSetBits(BIT_21F_Spirit_Collected, 1);
                    ((DLL_210_Player*)player->dll)->vtbl->set_spirit_bits(player, PLAYER_SPIRIT_2, FALSE);
                    mainSetBits(BIT_231, 1);
                    mainSetBits(BIT_232, 1);
                    mainSetBits(BIT_Spirit_2_Release_Sabre, 1);
                    break;
                }
            }
            break;
        case WL_SEQPOINT_SEQ_0_Spirit_1_Release:
            if (animObjData->messages[i] != 0) {
                objdata->lastMesg = animObjData->messages[i];
                switch (animObjData->messages[i]) {
                case 1:
                    mainSetBits(BIT_125, 1);
                    break;
                case 2:
                    mainSetBits(BIT_125, 0);
                    break;
                case 3:
                    mainSetBits(BIT_WM_Spirit_Release_Effect, 1);
                    break;
                case 5:
                    mainSetBits(BIT_Set_During_Spirit_Release_1, 1);
                    break;
                case 4:
                    mainSetBits(BIT_WM_Spirit_Release_Effect, 0);
                    mainSetBits(BIT_Set_During_Spirit_Release_1, 1);
                    ((DLL_210_Player*)player->dll)->vtbl->set_spirit_bits(player, PLAYER_SPIRIT_1, FALSE);
                    mainSetBits(BIT_231, 1);
                    mainSetBits(BIT_232, 1);
                    mainSetBits(BIT_Play_Seq_0180_Release_Spirit_1, 1);
                    break;
                }
            }
            break;
        case WL_SEQPOINT_SEQ_6_Spirit_3_Release:
            if (animObjData->messages[i] != 0) {
                objdata->lastMesg = animObjData->messages[i];
                switch (animObjData->messages[i]) {
                case 1:
                    mainSetBits(BIT_MMP_GP_Shrine_Spirit_Light_Beams, 1);
                    break;
                case 2:
                    mainSetBits(BIT_MMP_GP_Shrine_Spirit_Light_Beams, 0);
                    break;
                case 3:
                    mainSetBits(BIT_WM_Spirit_Release_Effect, 1);
                    break;
                case 4:
                    mainSetBits(BIT_WM_Spirit_Release_Effect, 0);
                    mainSetBits(BIT_21C, 1);
                    ((DLL_210_Player*)player->dll)->vtbl->set_spirit_bits(player, PLAYER_SPIRIT_3, FALSE);
                    mainSetBits(BIT_231, 1);
                    mainSetBits(BIT_232, 1);
                    mainSetBits(BIT_317, 1);
                    break;
                }
            }
            break;
        case WL_SEQPOINT_SEQ_10_Spirit_4_Release:
            if (animObjData->messages[i] != 0) {
                objdata->lastMesg = animObjData->messages[i];
                switch (animObjData->messages[i]) {
                case 1:
                    mainSetBits(BIT_143, 1);
                    break;
                case 2:
                    mainSetBits(BIT_143, 0);
                    break;
                case 3:
                    mainSetBits(BIT_WM_Spirit_Release_Effect, 1);
                    break;
                case 4:
                    mainSetBits(BIT_WM_Spirit_Release_Effect, 0);
                    mainSetBits(BIT_21D, 1);
                    ((DLL_210_Player*)player->dll)->vtbl->set_spirit_bits(player, PLAYER_SPIRIT_4, FALSE);
                    mainSetBits(BIT_2F5, 1);
                    mainSetBits(BIT_2F6, 1);
                    mainSetBits(BIT_318, 1);
                    break;
                }
            }
            break;
        case WL_SEQPOINT_SEQ_1:
            if (animObjData->messages[i] != 0) {
                objdata->lastMesg = animObjData->messages[i];
            }
            break;
        case WL_SEQPOINT_SEQ_4_Randorn_Warns_Krystal_Of_Skeetlas:
            if (animObjData->messages[i] != 0) {
                objdata->lastMesg = animObjData->messages[i];
            }
            switch (objdata->lastMesg) {
            case 1:
                diPrintf("\t\tRandorn...  Are You There ??\n");
                break;
            case 2:
                diPrintf("\t\tBe Careful..Skeetas Everywhere !\n");
                break;
            case 3:
                diPrintf("\t\tStay Close to a Torch!\n");
                diPrintf("\t\tLight Kills Them\n");
                break;
            }
            break;
        case WL_SEQPOINT_SEQ_5_Krystal_Randorn_Talk_About_Sabre:
            if (animObjData->messages[i] != 0) {
                objdata->lastMesg = animObjData->messages[i];
            }
            switch (objdata->lastMesg) {
            case 1:
                diPrintf("\t\tKrystal! How Is My Son?\n");
                break;
            case 2:
                diPrintf("\t\tSabre Is Just Glad Your Alive\n");
                break;
            case 3:
                diPrintf("\t\tWhen His Brother Died I blamed Myself\n");
                diPrintf("\t\tI Couldn't Face Being A Father\n");
                diPrintf("\t\tUntil I Found You My Dear Girl\n");
                break;
            case 4:
                diPrintf("\t\tHe Understands Why You Left Him\n");
                break;
            case 5:
                diPrintf("\t\tHe Is A Good Son\n");
                diPrintf("\t\tI've Yet To See Him To Apologise\n");
                break;
            case 6:
                diPrintf("\t\tHe Has Already Forgiven You\n");
                diPrintf("\t\tWhen This Is Over You Shall Be Reunited!\n");
                break;
            }
            break;
        case WL_SEQPOINT_SEQ_11_Randorn_Asks_For_Food:
            if (animObjData->messages[i] != 0) {
                objdata->lastMesg = animObjData->messages[i];
            }
            switch (objdata->lastMesg) {
            case 1:
                diPrintf("\t\tWhat's happening to you Randorn?\n");
                break;
            case 2:
                diPrintf("\t\tThere's not much energy left in this Mountain\n");
                diPrintf("\t\tI'm dying..I need food\n");
                break;
            }
            break;
        case WL_SEQPOINT_SEQ_13_Randorn_Explains_Arrival:
            if (animObjData->messages[i] != 0) {
                objdata->lastMesg = animObjData->messages[i];
            }
            switch (objdata->lastMesg) {
            case 1:
                diPrintf("\t\tDo You Remember How We Got Here?\n");
                break;
            case 2:
                diPrintf("\t\tOf Course I Remember...\n");
                diPrintf("\t\tBack On Our Own Planet We Found A\n");
                diPrintf("\t\tStrange Room In The Lost City..\n");
                diPrintf("\t\tThere We Uncovered A Message From The\n");
                diPrintf("\t\tKing EarthWalker, Begging Us To Help Him.\n");
                break;
            case 3:
                diPrintf("\t\tAnd Then I Entered A Wormhole\n");
                diPrintf("\t\tAnd Arrived On Dinosaur Planet\n");
                break;
            case 4:
                diPrintf("\t\tYou Followed Later With Sabre\n");
                break;
            case 5:
                diPrintf("\t\tYes, We Ended Up In A Chamber Beneath\n");
                diPrintf("\t\tOne Of The Earthwalker Temples\n");
                break;
            case 6:
                diPrintf("\t\tUnless You Can Find A Cure For Me\n");
                diPrintf("\t\tI'm Not Going To Last Much Longer\n");
                break;
            case 7:
                diPrintf("\t\tBut What Can We Do\n");
                break;
            case 8:
                diPrintf("\t\tThere Must Be A Similiar Room Here\n");
                diPrintf("\t\tThat Will Allow Us To Return\n");
                diPrintf("\t\tEither You Or Sabre Must Find It\n");
                break;
            }
            break;
        }
        animObjData->messages[i] = 0;
    }
    return 0;
}
