#ifndef _SEGMENT_334F0
#define _SEGMENT_334F0

#include "PR/ultratypes.h"
#include "game/objects/object.h"

typedef enum {
    BLINK_Wait = 0,
    BLINK_Animate = 1,
    BLINK_Eyelid_Close_Finished = 0x80
} BlinkStates;

typedef enum {
    HEAD_TURN_Goal_Reached = 0,
    HEAD_TURN_Wait = 1,
    HEAD_TURN_Animate = 2
} HeadTurnStates;

typedef enum {
    HEAD_ANIMATION_TAG_Pupil_L = 0,
    HEAD_ANIMATION_TAG_Pupil_R = 1,
    HEAD_ANIMATION_TAG_Eyelid_L = 4,
    HEAD_ANIMATION_TAG_Eyelid_R = 5
} HeadAnimationTags;

// Size: 0x24
typedef struct HeadAnimation {
    /** Head aim */
    /* 0x00 */ s8 aimIsActive;
    /* 0x01 */ u8 pad1[0x4 - 0x1];
    /* 0x04 */ f32 headAimX;
    /* 0x08 */ f32 headAimY;
    /* 0x0C */ f32 headAimZ;
    /* 0x10 */ f32 headAimUnk;
    
    /** Randomised head turn */
    /* 0x14 */ s16 headGoalAngle;   
    /* 0x16 */ s16 headStartAngle;
    /* 0x18 */ s16 unk18;           //unused?
    /* 0x1A */ s16 headTurnState;
    /* 0x1C */ s16 headTurnDelay;   //random delay before next head turn

    /** Randomised blinks */
    /* 0x1E */ s8 blinkState;
    /* 0x1F */ s8 blinkDelayTimer;

    /** Randomised eye darts */
    /* 0x20 */ s8 eyeSpeed;       
    /* 0x21 */ s8 eyeDelayTimer;  //frames until next eye dart
    /* 0x22 */ s8 eyeGoal;        //goal position for current eye dart
} HeadAnimation;

typedef struct {
    s32 frame; //second-lowest byte used as frame offset for animated/multi-frame textures,
              //lowest byte opacity-blends next frame in (on some multi-frame textures)
    u32 unk4;
    s16 positionU; //U offset
    s16 positionV; //V offset
    u8 multiplyR; //allows colour to be multiplied with the texture
    u8 multiplyG; //allows colour to be multiplied with the texture
    u8 multiplyB; //allows colour to be multiplied with the texture
} TextureAnimator;

void func_80034BC0(Object* obj, HeadAnimation* arg1);
void func_80034B94(Object* arg0, HeadAnimation* arg1, s32 soundID);
void func_80032A08(Object* obj, HeadAnimation* arg1);
void func_800328F0(Object* obj, HeadAnimation* arg1, f32 arg2);
void func_80033B68(Object*, HeadAnimation*, f32);
void func_80034678(Object*, HeadAnimation*, f32);
TextureAnimator* func_800348A0(Object*, s32, s32);
void func_800339E0(Object*, s32, s32, f32);
void func_80033AA0(Object*, s32, s32, f32);
s16* func_80034804(Object *obj, s32 sequenceBoneID);
s32 func_80034250(HeadAnimation*, s16*);
s32 func_80034518(HeadAnimation*, s16*, f32, f32);
void func_80033C54(Object*, HeadAnimation*, f32, s16*);
void func_80033FD8(Object*, HeadAnimation*, f32, s16*);
s32 func_800343B8(HeadAnimation* arg0, s16* arg1, f32 arg2, f32 arg3);
void func_80032CF8(Object* obj, Object* otherObj, HeadAnimation* arg2, s32 arg3);
void func_80034B54(Object* arg0, HeadAnimation* arg1, s16* soundIDs, u8 arg3);
#endif //_SEGMENT_334F0
