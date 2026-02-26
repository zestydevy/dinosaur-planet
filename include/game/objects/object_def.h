#ifndef _SYS_GAME_OBJECTS_OBJECT_DEF_H
#define _SYS_GAME_OBJECTS_OBJECT_DEF_H

#include "sys/math.h"
#include "unktypes.h"

/** Object definitions in OBJECTS.BIN.
 */

enum ObjDataFlags44 { //copied from SFA; may be incorrect
    OBJDATA_FLAG44_HaveModels               = 0x00000001,
    OBJDATA_FLAG44_DifferentLightColor      = 0x00000010,
    OBJDATA_FLAG44_ModelRelated             = 0x00000020,
    OBJDATA_FLAG44_HasChildren              = 0x00000040,
    OBJDATA_FLAG44_EnableCulling            = 0x00000400,
    OBJDATA_FLAG44_UseDifferentModelLoading = 0x00000800,
    OBJDATA_FLAG44_DifferentCulling         = 0x00080000,
    OBJDATA_FLAG44_KeepHitboxWhenInvisible  = 0x00200000,
    OBJDATA_FLAG44_HasEvent                 = 0x00400000,
    OBJDATA_FLAG44_DidLoadModels            = 0x00800000
};

enum ObjShadowType { //copied from SFA; may be incorrect
    OBJ_SHADOW_NONE = 0x0,
    OBJ_SHADOW_BOX = 0x1,
    OBJ_SHADOW_GEOM = 0x2,
    OBJ_SHADOW_UNK03 = 0x3,
    OBJ_SHADOW_BLUE_GLOWING_RECT = 0x4
};

enum HitboxFlags60 { //copied from SFA; may be incorrect
    HITBOX_DISABLED        = 0x0001,
    HITBOX_NEED_POS_UPDATE = 0x0040,
    HITBOX_LOCK_ROT_Y      = 0x0800,
    HITBOX_LOCK_ROT_Z      = 0x1000
};

enum HitboxFlags62 { //copied from SFA; may be incorrect
    HITBOX_SCALE_BY_SIZE      = 0x01,
    HITBOX_SIZE_FLAG_02       = 0x02,
    HITBOX_DONT_UPDATE        = 0x08,
    HITBOX_USE_MODEL_FIELD_14 = 0x20
};

enum ObjDataFlags5F { //copied from SFA; may be incorrect
    OBJDATA_FLAG5F_CrazyTranslucentEffect   = 0x01, //for HagabonMk2?
    OBJDATA_FLAG5F_ShadowUsesNoTexture      = 0x02,
    OBJDATA_FLAG5F_ShadowUsesDepthTest      = 0x04,
    OBJDATA_FLAG5F_DontFollowParentRotation = 0x08,
    OBJDATA_FLAG5F_NoShadow                 = 0x10, //force depth test if 0x01 also set
    OBJDATA_FLAG5F_Visible                  = 0x20,
    OBJDATA_FLAG5F_DifferentTextures        = 0x80  //Super dark
};

//A point on a model where something like a weapon can be attached.
typedef struct AttachPoint { //copied from SFA; may be incorrect
/*00*/ Vec3f pos; //offset from bone
/*0C*/ Vec3s16 rot; //offset from bone
/*12*/ s8 bones[6]; //bone idx to use
} AttachPoint;

typedef struct {
/*00*/ s16 unk00; //arrowCoordX
/*02*/ s16 unk02; //arrowCoordY
/*04*/ s16 unk04; //arrowCoordZ
/*06*/ s16 unk06;
/*08*/ s16 unk08;
/*0a*/ s16 unk0a;
/*0c*/ u8 unk0c; //arrow interact radius (can press A)
/*0d*/ u8 unk0d; //lock-on exit radius
/*0e*/ u8 unk0e; //arrow highlight radius (greyed out)
/*0f*/ u8 unk0f; //interaction height (cylinder height for arrow zone)
/*10*/ u8 unk10; //colourIndex for arrow
/*11*/ s8 unk11[4];
/*15*/ UNK_TYPE_8 unk15;
/*16*/ UNK_TYPE_8 unk16;
/*17*/ UNK_TYPE_8 unk17;
} ObjDefStruct40;

typedef struct {
    s16 id;
    s16 fileOffsetInBytes;
    s16 fileSizeInBytes;
} ObjDefEvent;

typedef struct {
    s16 id;
    s16 fileOffsetInBytes;
    s16 fileSizeInBytes;
} ObjDefHit;

typedef struct {
    s16 id;
    s16 fileOffsetInBytes;
    s16 fileSizeInBytes;
} ObjDefWeaponData;

// size: 0x14
typedef struct {
    union {
        struct {
/*00*/    s16 Ax;
/*02*/    s16 Bx;
/*04*/    s16 Ay;
/*06*/    s16 By;
/*08*/    s16 Az;
/*0a*/    s16 Bz;
        };

        s16 somePos[6];
    };
/*0c*/    s8 heightA; //height can be treated as a single value "(heightA << 8) + heightB", depending on top settings bit
/*0d*/    s8 heightB;
/*0e*/    s8 settingsA;
/*0f*/    s8 settingsB;
/*10*/    s16 animatorID;
/*12*/    s16 pad;
} ModLine;

/** ModLine seem to end up reencoded into this format
  * The point data seems to be missing and index values are introduced,
  * so maybe they're combining coincident points across different lines,
  * and referencing those verts by index?
*/
// size: 0x10
typedef struct{
/*00*/    s8 heightA;
/*01*/    s8 heightB;
/*02*/    s8 settingsA;
/*03*/    s8 settingsB;
union {
    struct {
    /*04*/    s16 indexA; 
    /*06*/    s16 indexB; 
    /*08*/    s16 indexC; 
    /*0a*/    s16 indexD;
    };
    s16 indexes[4];
};
/*0c*/    s16 animatorID;
/*0e*/    s16 unkE;
} ModLineReencoded;

typedef struct {
    s16 unk0;             //always 0 in ROM
    s16 type;             //see Collectable_Types
    s16 unk4;             //always 0 in ROM
    s16 seqObjectID;      //objectID to display during item collection sequence (usually an Anim object like OBJ_AnimSpellstone)
    s8 interactionRadius; //radius for picking up
    s8 collectMessage;    //byte1 of message sent to player DLL when collected
    s8 unkA;              //always 0 in ROM
    s8 amountRestored;    //magic restored (Food collectables also have a value here, but it's unused)
} CollectableDef;

/**
 * Object definition.
 * 
 * Entry of OBJECTS.BIN
 * many fields copied from SFA; some may still be incorrect
 * 
 * Size: 0xAC. In OBJECTS.BIN, this is followed by variable size data related to the definition.
 */
typedef struct {
/*00*/ f32 shadowScale;
/*04*/ f32 scale;
/*08*/ u32 *pModelList;         // list of model IDs (offset in file, pointer after load)
/*0c*/ UNK_PTR *pTextures;      // list of texture IDs? (offset in file, pointer after load)
/*10*/ u8 *pSequenceBones; // list of sequenceBoneID + jointIDs (one for each modelIndex) that ANIMCURVES use to apply additive head/jaw animation
/*14*/ UNK_PTR *unk14;
/*18*/ CollectableDef *collectableDef;              // [optional] pickup obj related? (offset in file, pointer after load)
/*1c*/ s16 *pSeq;               // [optional] sequenceIDs (offset in file, pointer after load)
/*20*/ ObjDefEvent *pEvent;     // [optional] table of OBJEVENT offsets (offset in file, pointer after load)
/*24*/ ObjDefHit *pHits;       // [optional] table of OBJHITS offsets (offset in file, pointer after load)
/*28*/ ObjDefWeaponData *pWeaponData;   // [optional] table of WEAPONDATA offsets (offset in file, pointer after load)
/*2c*/ AttachPoint *pAttachPoints;      // (offset in file, pointer after load)
/*30*/ ModLine *pModLines; //ignored in file (zeroed on load) // TODO: confirm
/*34*/ ModLineReencoded* pIntersectPoints; //ignored in file (zeroed on load) (wObjList?) // TODO: confirm
/*38*/ u8 *nextIntersectPoint; // TODO: confirm
/*3c*/ Vec3f *nextIntersectLine; // TODO: confirm
/*40*/ ObjDefStruct40 *unk40; //z-targetting data
/*44*/ u32 flags; //ObjDataFlags44 // TODO: confirm (0x10000: uses colour multiplier?)
/*48*/ s16 shadowType; //ObjShadowType // TODO: confirm
/*4a*/ s16 shadowTexture; // TODO: confirm
/*4c*/ UNK_TYPE_8 unk4C;
/*4d*/ UNK_TYPE_8 unk4D;
/*4e*/ u16 hitbox_flags60; //HitboxFlags60 // TODO: confirm
/*50*/ UNK_TYPE_16 unk50;
/*52*/ UNK_TYPE_8 unk52;
/*53*/ UNK_TYPE_8 unk53;
/*54*/ UNK_TYPE_8 unk54;
/*55*/ u8 unk55;
/*56*/ u8 numPlayerObjs; //if > 0, objAddObjectType(obj, 8) // TODO: confirm
/*57*/ u8 unk57; //never read?
/*58*/ u16 dllID;
/*5a*/ s16 group; // TODO: rename to controlNo
/*5c*/ s8 modLinesSize; //ignored in file // TODO: confirm
/*5d*/ s8 numModels; // number of pModelList (called "noframes" (number of frames?) by default.dol)
/*5e*/ s8 unk5e;
/*5f*/ char name[16]; // not always the full name, cutoff after 15 characters (16 including null terminator)
/*6f*/ u8 unk6f; // class (according to default.dol)
/*70*/ u8 numAttachPoints;
/*71*/ u8 numAnimatedFrames;
/*72*/ u8 numSequenceBones;
/*73*/ u8 stateVar73; //1=translucent; 3=invincible - not flags // TODO: confirm
/*74*/ u8 unk74;
/*75*/ u8 unk75;
/*76*/ s16 modLineCount;
/*78*/ s16 modLineNo;
/*7a*/ s16 numSequences;
/*7c*/ s16 helpText; //one per model (GameTextId) // TODO: confirm //NOTE: possibly objHits flags?
/*7E*/ s8 unk7E;
/*7F*/ u8 unk7F;
/*80*/ s8 unk80;
/*81*/ u8 pad81;
/*82*/ s8 unk82;
/*83*/ s8 unk83;
/*84*/ s8 unk84;
/*85*/ u8 unk85;
/*86*/ u8 unk86;
/*87*/ u8 unk87; //lighting-related flags?
/*88*/ float lagVar88; //causes lag at ~65536.0; GPU hang at much more; related to shadow; maybe causing excessive map loads? // TODO: confirm
/*8c*/ u8 nLights; // TODO: confirm
/*8d*/ u8 lightIdx; // TODO: confirm
/*8e*/ u8 colorIdx; //related to textures; 1=dark, 2=default, 3+=corrupt, 77=crash, 0=normal // TODO: confirm
/*8f*/ u8 unk8F; //related to hitbox // TODO: confirm
/*90*/ u8 hitbox_flagsB6; // < 0xE = invincible (HitboxFlags62) // TODO: confirm
/*91*/ u8 _unk91[2];
/*93*/ s8 unk93; //render settings (can switch off vertex shading)
/*94*/ s16 unk94;
/*96*/ s16 unk96;
/*98*/ u8 _unk98[3];
/*9b*/ u8 unk9b; // length of unk40 (z-targetting data)
/*9c*/ u8 unk9c;
/*9d*/ u8 unk9d; // camera-related angle?
/*9e*/ u8 _unk9e[2];
/*a0*/ s16 unka0;
/*a2*/ s16 gametextIndex; //object description line index in gametext_3 or gametext_568 (appears when holding R) (-1 when unused)
/*a4*/ u8 _unka4[0xAA - 0xA4];
/*aa*/ s16 unkAA;
} ObjDef;

#endif
