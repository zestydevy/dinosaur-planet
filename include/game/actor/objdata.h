#ifndef _SYS_GAME_ACTOR_OBJDATA_H
#define _SYS_GAME_ACTOR_OBJDATA_H

#include "sys/math.h"
#include "unktypes.h"

/** Object (actor) data in OBJECTS.BIN.
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
/*12*/ s8 bone; //bone idx to use
/*13*/ s8 unk13; //always same as bone?
/*14*/ s8 unk14; //always same as bone?
/*15*/ u8 unk15; //always 0xCD?
/*16*/ u8 unk16; //always 0xCD?
/*17*/ u8 unk17; //always 0xCD?
} AttachPoint;

typedef struct AButtonInteraction { //copied from SFA; may be incorrect
/*00*/ s16 unk00;
/*02*/ s16 unk02;
/*04*/ s16 unk04;
/*06*/ UNK_TYPE_8 unk06;
/*07*/ UNK_TYPE_8 unk07;
/*08*/ UNK_TYPE_8 unk08;
/*09*/ UNK_TYPE_8 unk09;
/*0a*/ UNK_TYPE_8 unk0a;
/*0b*/ UNK_TYPE_8 unk0b;
/*0c*/ UNK_TYPE_8 unk0c;
/*0d*/ UNK_TYPE_8 unk0d;
/*0e*/ UNK_TYPE_8 unk0e;
/*0f*/ UNK_TYPE_8 unk0f;
/*10*/ u8 unk10;
/*11*/ UNK_TYPE_8 unk11;
/*12*/ UNK_TYPE_8 unk12;
/*13*/ UNK_TYPE_8 unk13;
/*14*/ UNK_TYPE_8 unk14;
/*15*/ UNK_TYPE_8 unk15;
/*16*/ UNK_TYPE_8 unk16;
/*17*/ UNK_TYPE_8 unk17;
} AButtonInteraction;

//Entry in OBJECTS.BIN, aka ObjectFileStruct (made-up name)
//debug messages imply ObjData is the correct name or close to it
typedef struct ObjData { //copied from SFA; may be incorrect
/*00*/ float unk00; //copied to shadow field 0
/*04*/ float scale;
/*08*/ u32 *pModelList; //-> list of model IDs
/*0c*/ UNK_PTR *textures;
/*10*/ UNK_PTR *unk10;
/*14*/ UNK_PTR *unk14;
/*18*/ u32 *offset_0x18; //[OPTIONAL] a file containing functions
/*1c*/ u16 *pSeq; //[OPTIONAL] -> seq IDs
/*20*/ UNK_TYPE_8 *pEvent; //[OPTIONAL] offset into the file. changed to pointer on load
/*24*/ UNK_TYPE_8 *pHits; //[OPTIONAL]
/*28*/ UNK_TYPE_8 *pWeaponDa; //[OPTIONAL]
/*2c*/ AttachPoint *pAttachPoints;
/*30*/ s16 *pModLines; //ignored in file (zeroed on load)
/*34*/ UNK_PTR *pIntersectPoints; //ignored in file (zeroed on load) (wObjList?)
/*38*/ UNK_PTR *nextIntersectPoint;
/*3c*/ UNK_PTR *nextIntersectLine;
/*40*/ AButtonInteraction *aButtonInteraction;
/*44*/ u32 flags; //ObjDataFlags44
/*48*/ s16 shadowType; //ObjShadowType
/*4a*/ s16 shadowTexture;
/*4c*/ UNK_TYPE_8 unk4C;
/*4d*/ UNK_TYPE_8 unk4D;
/*4e*/ u16 hitbox_flags60; //HitboxFlags60
/*50*/ s16 dllNo; //if not -1, load this DLL; func 0 is a model callback
/*52*/ UNK_TYPE_8 unk52;
/*53*/ UNK_TYPE_8 unk53;
/*54*/ UNK_TYPE_8 unk54;
/*55*/ u8 nModels;
/*56*/ u8 numPlayerObjs; //if > 0, objAddObjectType(obj, 8)
/*57*/ u8 unk57; //never read?
/*58*/ u8 nAttachPoints;
/*59*/ u8 nTextures; //-> 0x10 bytes
/*5a*/ s16 objId;
/*5c*/ s8 modLinesSize; //ignored in file
/*5d*/ s8 modLinesIdx;
/*5e*/ s8 numSeqs;
/*5f*/ u8 flags_0x5f; //ObjDataFlags5F
/*60*/ u8 hitbox_fieldB0;
/*61*/ u8 hasHitbox; //or # hitboxes, but should only be 1
/*62*/ u8 hitboxSizeXY;
/*63*/ u8 hitbox_field6A;
/*64*/ u8 hitbox_field6B;
/*65*/ u8 hitbox_flags62;
/*66*/ u8 unk66;
/*67*/ u8 hitbox_fieldB5;
/*68*/ s16 hitboxSizeX1;
/*6a*/ s16 hitboxSizeY1; // > 0x169 = no shadow; also hitbox related
/*6c*/ s16 hitboxSizeZ1;
/*6e*/ s16 hitboxSizeZ2;
/*70*/ u8 hitbox_fieldB4; //related to hitbox (height?)
/*71*/ u8 flags_0x71; //related to hitbox
/*72*/ u8 numAButtonInteractions;
/*73*/ u8 stateVar73; //1=translucent; 3=invincible - not flags
/*74*/ u8 unk74;
/*75*/ u8 unk75;
/*76*/ s16 modLineCount;
/*78*/ s16 modLineNo;
/*7a*/ u8 unk7A;
/*7b*/ u8 unk7B;
/*7c*/ s16 helpTexts[4]; //one per model (GameTextId)
/*84*/ s16 unk84;
/*86*/ s16 unk86;
/*88*/ float lagVar88; //causes lag at ~65536.0; GPU hang at much more; related to shadow; maybe causing excessive map loads?
/*8c*/ u8 nLights;
/*8d*/ u8 lightIdx;
/*8e*/ u8 colorIdx; //related to textures; 1=dark, 2=default, 3+=corrupt, 77=crash, 0=normal
/*8f*/ u8 unk8F; //related to hitbox
/*90*/ u8 hitbox_flagsB6; // < 0xE = invincible (HitboxFlags62)
/*91*/ char name[11];
/*9c*/ u8 unk9c[4];
/*a0*/ s16 unka0;
} ObjData;

#endif //_SYS_GAME_ACTOR_OBJDATA_H
