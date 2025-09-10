/** Animation system
 */
#ifndef _SYS_GFX_ANIMATION_H
#define _SYS_GFX_ANIMATION_H

#include "PR/ultratypes.h"

typedef struct {
/*0000*/    u8 unk_0x0;
/*0001*/	u8 unk_0x1;
/*0002*/	s16 unk_0x2;
/*0000*/	u8 totalBones;
/*0001*/	u8 totalKeyframes;
/*0002*/	u8 keyframeStride;
/*0003*/	u8 unk_0x9;
/** Encodes info about each bone component (SRT): 
base value throughout animation,
the number of bits used in adding a delta to the base per keyframe,
and whether TS components are also animated (bones always have rotation component)*/
/*0004*/	u16 animatedComponents; //array
} AnimationHeader;

typedef struct {
/*0000*/    u8 referenceCount;
/*0001*/	u8 unk_0x1; //animation flags (looping etc?)
/*0002*/	s16 unk_0x2; //keyframe_data_offset
/** keyframes for the root translation of the model
(used to map the anim playback progress based on character speed, e.g. during walk cycles) */
/*0004*/	s16 offset_rootMotion;
/*0006*/	AnimationHeader animHeader;
} Animation;

typedef struct {
/*0000*/    u8 boneRemaps[0x80];
/*0080*/	Animation anim;
} AmapPlusAnimation;

typedef struct {
/*0000*/    u32 unk_0x0;
/*0004*/    f32 curAnimationFrame[2];
/*000C*/    f32 unk_0xc[2];
/*0014*/    f32 totalAnimationFrames[2];
/*001C*/    AmapPlusAnimation *anims[2]; // Each item contains 0x80 bytes of amap data, followed by an Animation structure
/*0024*/    AmapPlusAnimation *anims2[2];
/*002C*/	u8 *unk_0x2c[2]; //anim_keyframe_deltas? (variable bitsize offset value added to component's base value throughout animation)
/*0034*/    AnimationHeader *unk_0x34[2];
/*003C*/    AnimationHeader *unk_0x3c[2];
/*0044*/    u16 animIndexes[2];
/*0048*/	u16 unk_0x48[2];
/*004C*/	u16 unk_0x4c[2][3];
/*0058*/    s16 unk_0x58[2]; //anim blend strength? (out of 1024)
/*005C*/	s16 unk_0x5c[2]; //anim blend duration?
/*0060*/    s8 unk_0x60[2];
/*0062*/    u8 unk_0x62[2];
/*0064*/    s16 modAnimIdBlend; //seems to be a modanimIndex (when it's not 0xFFFF)
} AnimState;

typedef struct{
    s32 referenceCount;
    Animation* animation;
} AnimSlot;

#endif //_SYS_GFX_ANIMATION_H
