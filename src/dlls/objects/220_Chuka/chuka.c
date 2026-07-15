#include "common.h"
#include "dlls/engine/6_amsfx.h"
#include "dlls/objects/220_Chuka.h"
#include "dlls/objects/221_ChukaChuck.h"
#include "game/gamebits.h"
#include "game/objects/interaction_arrow.h"
#include "macros.h"
#include "sys/math.h"

typedef struct {
    ObjSetup base;
    s16 gamebitDead;
    s16 unk1A;
    s16 _unk1C;
    s16 _unk1E;
    s16 _unk20;
    s16 droppedItemIdx;
    s16 _unk24; 
    s8 _unk26;
    s8 playerAimOffsetY;
    s8 angularRange; //(Degrees) When the Chuka's yaw is within this angular range of facing towards the player (and the player is in lateral range), a ChukaChuck can be thrown
    u8 range2D;
    s8 yaw;
    u8 _unk2B;
    u8 _unk2C;
    u8 _unk2D;
    u8 _unk2E;
    u8 chuckProbability;
    s16 _unk30;
    u8 health;
} Chuka_Setup;

typedef struct {
    f32 blinkTimer;
    u8 flags;
    u16 range2D;            //When the player is inside this lateral range of the Chuka, a ChukaChuck can be thrown
    u16 angularRange;       //(Angle16) When the Chuka's yaw is within this angular range of facing towards the player (and the player is in lateral range), a ChukaChuck can be thrown
    s16 droppedItemIdx;     //The type of collectable dropped when defeated (Magic Gems, Energy Eggs, etc.)
    u8 health;
    u8 chuckProbability;    //Percentage likelihood of throwing a ChukaChuck at the player when in firing range
    u8 playerAimOffsetY;    //Vertical offset the Chuka aims at, relative to the player's feet
    s16 gamebitDead;
    u16 prevPlayerDistance2D;
} Chuka_Data;

typedef enum {
    Chuka_FLAG_0_None = 0,
    Chuka_FLAG_1_Player_In_Range = 1,
    Chuka_FLAG_2_Dead = 2,
    Chuka_FLAG_4_Do_Chuck = 4
} Chuka_Flags;

//Texture frame index flipbook animation used while blinking
static u8 dBlinkFrames[16] = {
    0, 1, 1, 2, 
    2, 2, 2, 2, 
    2, 2, 2, 2, 
    2, 1, 1, 0
}; 

typedef struct {
    u32 idle;
    u32 attackLanded;
    u32 attack;
    u32 die;
} ChukaSoundIDs;

static ChukaSoundIDs dSoundIDs = {
    SOUND_491, 
    SOUND_492, //not referenced, but the same soundID is used by `Chuka_receive_message` 
    SOUND_493, 
    SOUND_4B1
}; 

static void Chuka_die(Object* self, Chuka_Data* objData);
static void Chuka_chuck(Object* self);

#define CHUCK_DURATION 42 //frames

// offset: 0x0 | ctor
void Chuka_ctor(void *dll) { }

// offset: 0xC | dtor
void Chuka_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void Chuka_setup(Object* self, Chuka_Setup* setup, s32 reset) {
    Chuka_Data* objData = self->data;

    self->unkAF |= ARROW_FLAG_8_No_Targetting;

    objData->gamebitDead = setup->gamebitDead;
    if ((objData->gamebitDead != NO_GAMEBIT) && mainGetBits( objData->gamebitDead)) {
        Chuka_die(self, objData);
        return;
    }

    objData->range2D = setup->range2D * 8;
    objData->droppedItemIdx = setup->droppedItemIdx;
    objData->health = setup->health;
    objData->angularRange = ((u16)setup->angularRange) * 182;
    objData->chuckProbability = setup->chuckProbability;
    objData->playerAimOffsetY = setup->playerAimOffsetY;
    self->srt.yaw = setup->yaw << 8;
}


// offset: 0x118 | func: 1 | export: 1
void Chuka_control(Object* self) {
    Chuka_Data* objData;
    TextureAnimator* texAnim;
    f32 dx;
    f32 dz;
    u16 distance2D;
    Object* player;
    Object* hitBy;
    f32 pad;
    s32 hitSphereID;
    s32 hitDamage;
    s32 angle;
    Vec3f vPlayer;

    objData = self->data;

    if (objData->flags & Chuka_FLAG_2_Dead) {
        return;
    }

    //Handle blinking
    texAnim = objExprGetTexAnimator(self, 0, 0);
    if (objData->blinkTimer < 16) {
        if ((s32)objData->blinkTimer == 10) {
            objData->flags |= Chuka_FLAG_1_Player_In_Range;
        }

        texAnim->frame = dBlinkFrames[(s32)objData->blinkTimer] << 8;

        objData->blinkTimer++; //@framerate-dependent
        if (objData->blinkTimer == 16) {
            objData->blinkTimer = mathRnd(16, 245);
        }
    } else {
        if (gUpdateRateF <= (255.0f - objData->blinkTimer)) {
            objData->blinkTimer += gUpdateRateF;
        } else {
            objData->blinkTimer = 0.0f;
        }
        texAnim->frame = 0;
    }
    
    //Handle attacking/playing sounds when the player is in range
    {
        player = objGetPlayer();
        dx = player->srt.transl.x - self->srt.transl.x;
        dz = player->srt.transl.z - self->srt.transl.z;
        distance2D = sqrtf(SQ(dx) + SQ(dz));

        if (distance2D < objData->range2D) {
            if (objData->prevPlayerDistance2D >= objData->range2D) {
                objData->flags = Chuka_FLAG_1_Player_In_Range | Chuka_FLAG_4_Do_Chuck;
                objData->blinkTimer = 0.0f;
            }

            if (objData->flags & (Chuka_FLAG_1_Player_In_Range | Chuka_FLAG_4_Do_Chuck)) {
                //Get vector to player, and angle to player
                VECTOR_SUBTRACT(player->globalPosition, self->globalPosition, vPlayer); 
                angle = mathAtan2f(vPlayer.f[0], vPlayer.f[2]) - (self->srt.yaw & 0xFFFF);
                CIRCLE_WRAP(angle);
                if (((u16)angle < objData->angularRange) || (((M_360_DEGREES - 1 - objData->angularRange) & 0xFFFF) < (u16)angle)) {
                    if ((mathRnd(0, 99) < objData->chuckProbability) || (objData->flags & Chuka_FLAG_4_Do_Chuck)) {
                        gDLL_6_AMSFX->vtbl->play(self, dSoundIDs.attack, MAX_VOLUME, NULL, NULL, 0, NULL);
                        Chuka_chuck(self);
                    } else {
                        gDLL_6_AMSFX->vtbl->play(self, dSoundIDs.idle, MAX_VOLUME, NULL, NULL, 0, NULL);
                    }
                } else {
                    gDLL_6_AMSFX->vtbl->play(self, dSoundIDs.idle, MAX_VOLUME, NULL, NULL, 0, NULL);
                }
            }
        } else if (objData->flags & Chuka_FLAG_1_Player_In_Range) {
            gDLL_6_AMSFX->vtbl->play(self, dSoundIDs.idle, MAX_VOLUME, NULL, NULL, 0, NULL);
        }
        
        objData->prevPlayerDistance2D = distance2D;
    }

    //Check for Projectile Spell attacks
    if (func_80025F40(self, &hitBy, &hitSphereID, &hitDamage) == Damage_Type_Projectile) {
        objData->health--; //Lose health
        if (objData->health <= 0) {
            Chuka_die(self, objData);
            gDLL_33_BaddieControl->vtbl->drop_collectable(self, objData->droppedItemIdx, NO_GAMEBIT, TRUE);
            gDLL_6_AMSFX->vtbl->play(self, dSoundIDs.die, MAX_VOLUME, NULL, NULL, 0, NULL);
            mainSetBits(objData->gamebitDead, TRUE);
        }
    }

    objData->flags &= ~(Chuka_FLAG_1_Player_In_Range | Chuka_FLAG_4_Do_Chuck);
}


// offset: 0x670 | func: 2 | export: 2
void Chuka_update(Object *self) { }

// offset: 0x67C | func: 3 | export: 3
void Chuka_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility) {
    if (visibility) {
        objprintDrawModel(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x6D0 | func: 4 | export: 4
void Chuka_free(Object *self, s32 a1) { }

// offset: 0x6E0 | func: 5 | export: 5
u32 Chuka_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x6F0 | func: 6 | export: 6
u32 Chuka_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(Chuka_Data);
}

// offset: 0x704 | func: 7 | export: 7
/**
  * Called when the Chuka's projectile (ChukaChuck) successfully hits the player or the sidekick.
  */
void Chuka_receive_message(Object* self, u8 message) {
    switch (message) {
    case 0x80:
        gDLL_6_AMSFX->vtbl->play(self, SOUND_492, MAX_VOLUME, NULL, NULL, 0, NULL);
        break;
    default:
        STUBBED_PRINTF("BADDIE:Chuka Unknown message [%d]\n", message);
        break;
    }
}

// offset: 0x778 | func: 8
/**
  * Creates a `ChukaChuck` Object, thrown at the player.
  */
static void Chuka_chuck(Object* self) {
    Chuka_Data* objData;
    ChukaChuck_Setup* chuckSetup;
    Object* chuck;
    Object* player;

    objData = self->data;
    chuckSetup = objAllocSetup(sizeof(ChukaChuck_Setup), OBJ_ChukaChuck);
    chuckSetup->base.x = self->srt.transl.x;
    chuckSetup->base.y = self->srt.transl.y;
    chuckSetup->base.z = self->srt.transl.z;
    chuckSetup->base.loadFlags = OBJSETUP_LOAD_MANUAL;
    chuckSetup->base.fadeFlags = OBJSETUP_FADE_CAMERA;
    chuckSetup->base.fadeDistance = 0xFF;
    chuck = objSetupObject(&chuckSetup->base, (OBJINIT_STANDALONE | OBJINIT_FLAG4), -1, -1, NULL);;
    
    if (chuck != NULL) {
        player = objGetPlayer();
        chuck->velocity.x = (player->srt.transl.x - self->srt.transl.x) / CHUCK_DURATION;
        chuck->velocity.y = ((player->srt.transl.y + objData->playerAimOffsetY) - self->srt.transl.y) / CHUCK_DURATION;
        chuck->velocity.z = (player->srt.transl.z - self->srt.transl.z) / CHUCK_DURATION;
        chuck->unkC4 = self;
    }
}

// offset: 0x8A4 | func: 9
static void Chuka_die(Object* self, Chuka_Data* objData) {
    func_800267A4(self);
    self->srt.flags |= OBJFLAG_INVISIBLE;
    objData->flags |= Chuka_FLAG_2_Dead;
}
