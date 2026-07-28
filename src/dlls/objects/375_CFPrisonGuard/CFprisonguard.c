#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/210_player.h"
#include "game/objects/interaction_arrow.h"
#include "game/objects/object.h"
#include "macros.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/objects.h"
#include "sys/objhits.h"
#include "sys/objmsg.h"
#include "sys/objprint.h"

typedef struct {
    ObjSetup base;
    s8 yaw;
    s16 sightRange;
} CFPrisonGuard_Setup;

typedef struct {
    u32 soundHandle;
    f32 vanishTimer;
    s16 timer;
    s8 uncleEscapedPrev;
    s8 state;
    u8 flags;
} CFPrisonGuard_Data;

typedef enum {
    CFPrisonGuard_STATE_0,
    CFPrisonGuard_STATE_1,
    CFPrisonGuard_STATE_2,
    CFPrisonGuard_STATE_3,
    CFPrisonGuard_STATE_4,
    CFPrisonGuard_STATE_5,
    CFPrisonGuard_STATE_6,
    CFPrisonGuard_STATE_7
} CFPrisonGuard_States;

typedef enum {
    CFPrisonGuard_FLAG_0 = 0,
    CFPrisonGuard_FLAG_1_First_Tick = 1,
    CFPrisonGuard_FLAG_2_Asleep = 2,
    CFPrisonGuard_FLAG_4_Dead = 4
} CFPrisonGuard_Flags;

typedef enum {
    CFPrisonGuard_OBJSEQ_0_Sleeping,
    CFPrisonGuard_OBJSEQ_1_Player_Detected,
    CFPrisonGuard_OBJSEQ_2_Fooled_by_Disguise
} CFPrisonGuard_ObjSeqs;

static int CFPrisonGuard_animCallback(Object* self, Object* animObj, AnimObj_Data* animData/*, s8 prevCallbackValue */);

// offset: 0x0 | ctor
void CFPrisonGuard_ctor(void* dll) { }

// offset: 0xC | dtor
void CFPrisonGuard_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void CFPrisonGuard_obj_Setup(Object* self, CFPrisonGuard_Setup* objSetup, s32 reset) {
    CFPrisonGuard_Data* objData = self->data;
    
    objData->flags = CFPrisonGuard_FLAG_1_First_Tick;
    
    self->srt.yaw = objSetup->yaw << 8;
    self->animCallback = (void*)CFPrisonGuard_animCallback;
    
    objInitMesgQueue(self, 4);
    
    objData->uncleEscapedPrev = TRUE;

    if (mainGetBits(BIT_CRF_CloudRunner_Uncle_Freed)) {
        objData->flags |= CFPrisonGuard_FLAG_4_Dead;
    }
    
    self->unkAF &= ~ARROW_FLAG_10_Greyed_Out;
}

// offset: 0xD0 | func: 1 | export: 1
void CFPrisonGuard_obj_Control(Object* self) {
    CFPrisonGuard_Data* objData;
    Object* player;
    f32 distance;
    CFPrisonGuard_Setup* objSetup;
    u32 keyCollected;

    objData = self->data;
    player = objGetPlayer();
    objSetup = (CFPrisonGuard_Setup*)self->setup;
    
    if (mainGetBits(BIT_CRF_CloudRunner_Uncle_Escaped) || (objData->flags & CFPrisonGuard_FLAG_4_Dead)) {
        STUBBED_PRINTF(" Removing Guard ");
        self->unkAF |= ARROW_FLAG_8_No_Targetting;
        self->srt.flags |= OBJFLAG_INVISIBLE;
        objDisable(self);
    }
    
    keyCollected = mainGetBits(BIT_CRF_Prison_Key_1);

    distance = vec3Distance(&self->globalPosition, &player->globalPosition);
    
    //Catch the player if they're nearby and not using the Illusion Spell
    if ((keyCollected == FALSE) && 
        (distance < objSetup->sightRange) && 
        (((DLL_210_Player*)player->dll)->vtbl->func50(player) != BIT_Spell_Illusion)
    ) {
        gDLL_3_Animation->vtbl->start_obj_sequence(CFPrisonGuard_OBJSEQ_1_Player_Detected, self, -1);
    } 
    
    //Play the sleep sequence if this is the first time `control` is run (and no other flags are set)
    if (objData->flags == CFPrisonGuard_FLAG_1_First_Tick) {
        gDLL_3_Animation->vtbl->start_obj_sequence(CFPrisonGuard_OBJSEQ_0_Sleeping, self, -1);
        objData->flags = CFPrisonGuard_FLAG_2_Asleep;
    } else {
        STUBBED_PRINTF(" Waiting G ");
    }
}

// offset: 0x274 | func: 2 | export: 2
void CFPrisonGuard_obj_Update(Object* self) {
    CFPrisonGuard_Data* objData = self->data;
    
    if (func_80025F40(self, NULL, NULL, NULL) == Damage_Type_Bullet) {
        STUBBED_PRINTF(" Hit By Projectile My Friend");
        objData->state = CFPrisonGuard_STATE_7;
    }
}

// offset: 0x2D0 | func: 3 | export: 3
void CFPrisonGuard_obj_Print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    CFPrisonGuard_Data* objData = self->data;
    
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }

    //Handle dying effects
    if (visibility && (objData->vanishTimer > 0.0f)) {
        objData->vanishTimer += 0.01f * gUpdateRate;
        
        if (objData->vanishTimer < 1.5f) {
            gDLL_32_modelfx->vtbl->func2(self, PARTICLE_32F, &objData->vanishTimer);
        } else {
            gDLL_32_modelfx->vtbl->func2(self, PARTICLE_330, &objData->vanishTimer);
            gDLL_32_modelfx->vtbl->func2(self, PARTICLE_330, &objData->vanishTimer);
        }
    }
}

// offset: 0x42C | func: 4 | export: 4
void CFPrisonGuard_obj_Free(Object* self, s32 onlySelf) {
    CFPrisonGuard_Data* objData = self->data;

    if (objData->soundHandle != 0) {
        dll_amSfx->Stop(objData->soundHandle);
        objData->soundHandle = 0;
    }
}

// offset: 0x494 | func: 5 | export: 5
u32 CFPrisonGuard_obj_GetModelFlags(Object* self) {
    return MODFLAGS_EVENTS | MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0x4A4 | func: 6 | export: 6
u32 CFPrisonGuard_obj_GetDataSize(Object* self, u32 offsetAddr) {
    return sizeof(CFPrisonGuard_Data);
}

// offset: 0x4B8 | func: 7
int CFPrisonGuard_animCallback(Object* self, Object* animObj, AnimObj_Data* animData/*, s8 prevCallbackValue */) {
    Object* outSender;
    Object* player;
    CFPrisonGuard_Data* objData;
    u32 outMessageID;
    s32 outMesgArg;
    s8 bitValUncleEscaped;
    s8 bitValBoneHeadFreed;
    s8 guardDetectedPlayer;
    f32 distance;
    CFPrisonGuard_Setup* objSetup;

    objData = self->data;
    outMesgArg = 0;
    objSetup = (CFPrisonGuard_Setup*)self->setup;
    
    diPrintf(" State %i ", objData->state);

    switch (animData->lastMessage) {
    case 0x29:
        objData->vanishTimer = 0.0f;
        break;
    case 4:
        goto label1; 
label1: ;
        objData->state = CFPrisonGuard_STATE_6;
        return 0;
    case 5:
        objData->vanishTimer += 0.01f * gUpdateRate;
        break;
    }

    if (self->seqSlot < 0) {
        STUBBED_PRINTF(" of ");
        return 0;
    }
    
    func_8002674C(self);

    bitValUncleEscaped = mainGetBits(BIT_CRF_CloudRunner_Uncle_Escaped);
    bitValBoneHeadFreed = mainGetBits(BIT_CRF_BoneHead_Guardian_Freed);

    //Wake up and end sequence when the CloudRunner Uncle is freed
    if ((objData->flags & CFPrisonGuard_FLAG_2_Asleep) && mainGetBits(BIT_CRF_CloudRunner_Uncle_Freed)) {
        objData->flags &= ~CFPrisonGuard_FLAG_2_Asleep;
        return 4;
    }
    
    //Return early if the CloudRunner Uncle has escaped
    if (bitValUncleEscaped) {
        return 4;
    } else if (bitValUncleEscaped || (objData->state == CFPrisonGuard_STATE_5)) {
        goto label2; 
label2:

        objData->state = CFPrisonGuard_STATE_5;
        STUBBED_PRINTF(" og");
        return 0;
    }

    guardDetectedPlayer = FALSE;
    player = objGetPlayer();

    switch (objData->state) {
    case CFPrisonGuard_STATE_0:
        distance = vec3Distance(&self->globalPosition, &player->globalPosition);
        if ((bitValBoneHeadFreed == FALSE) && (distance < objSetup->sightRange)) {
            if (((DLL_210_Player*)player->dll)->vtbl->func50(player) != BIT_Spell_Illusion) {
                guardDetectedPlayer = TRUE;
                objData->state = CFPrisonGuard_STATE_4;
                break;
            } else {
                self->unkAF |= ARROW_FLAG_8_No_Targetting;
                objData->state = CFPrisonGuard_STATE_4;
                objData->timer = 20;
                gDLL_3_Animation->vtbl->start_obj_sequence(CFPrisonGuard_OBJSEQ_2_Fooled_by_Disguise, self, -1);
                return 4;
            }
        }
    default:
        break;
    case CFPrisonGuard_STATE_2:
        objData->timer -= gUpdateRate;
        if (objData->timer <= 0) {
            objData->state = CFPrisonGuard_STATE_1;
        }
        break;
    case CFPrisonGuard_STATE_1:
        distance = vec3Distance(&self->globalPosition, &player->globalPosition);
        if ((bitValBoneHeadFreed == FALSE) && (distance < objSetup->sightRange)) {
            if (((DLL_210_Player*)player->dll)->vtbl->func50(player) != BIT_Spell_Illusion) {
                guardDetectedPlayer = TRUE;
                objData->state = CFPrisonGuard_STATE_4;
            } else {
                objData->state = CFPrisonGuard_STATE_2;
            }
        }
        break;
    case CFPrisonGuard_STATE_3:
        objData->timer -= gUpdateRate;
        if (objData->timer <= 0) {
            objData->state = CFPrisonGuard_STATE_0;
        }
        break;
    case CFPrisonGuard_STATE_5:
        STUBBED_PRINTF(" Removed ");
        return 0;
    case CFPrisonGuard_STATE_6:
        return 0;
    case CFPrisonGuard_STATE_7:
        guardDetectedPlayer = TRUE;
        objData->state = CFPrisonGuard_STATE_4;
        break;
    }

    if (objData->soundHandle == 0) {
        if ((self->curModAnimId == 0x203) || (self->curModAnimId == 0x205)) {
            dll_amSfx->Play(self, SOUND_1C9_SharpClaw_Snore_Loop, MAX_VOLUME, &objData->soundHandle, NULL, 0, NULL);
        }
    } else {
        if ((self->curModAnimId != 0x203) && (self->curModAnimId != 0x205)) {
            dll_amSfx->Stop(objData->soundHandle);
            objData->soundHandle = 0;
        }
    }
    
    /* Guard detects the player when the "CloudRunner Uncle escaped" gamebit is set??
       (Unreachable, since it returns earlier than here when `bitValUncleEscaped` is nonzero.) */
    if (bitValUncleEscaped && (objData->uncleEscapedPrev == FALSE)) {
        guardDetectedPlayer = TRUE;
    }
    
    //Play player detected sequence
    if (guardDetectedPlayer) {
        gDLL_3_Animation->vtbl->start_obj_sequence(CFPrisonGuard_OBJSEQ_1_Player_Detected, self, -1);
        return 4;
    }
    
    objData->uncleEscapedPrev = bitValUncleEscaped;

    animData->unk62 = 0;
    
    while (objRecvMesg(self, &outMessageID, &outSender, (void*)&outMesgArg)) {
    }
    
    if (animData->lastMessage == 1) {
        lfxAction(self, self, 0x18, 0, 0, 0);
        animData->lastMessage = 0;
    }
    
    return 0;
}
