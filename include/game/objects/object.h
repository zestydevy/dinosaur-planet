#ifndef _SYS_GAME_OBJECTS_OBJECT_H
#define _SYS_GAME_OBJECTS_OBJECT_H

#include "PR/gbi.h"
#include "sys/gfx/model.h"
#include "sys/gfx/texture.h"
#include "sys/math.h"
#include "dll_def.h"
#include "object_def.h"
#include "hitbox.h"
#include "types.h"
#include "unktypes.h"

/** Game Object system
 */

#define OBJECT_OPACITY_MAX 255

struct Object;

// This could be bit fields? so 0, 1, 2, 4, 8 (0 << 0, 0 << 1, 0 << 2 etc)
enum ObjectGroup {
	GROUP_NONE = 0,
	GROUP_UNK1 = 1, // player?
	GROUP_UNK16 = 16,
	GROUP_UNK29 = 29,
	GROUP_UNK41 = 41,
	GROUP_UNK46 = 46,
	GROUP_UNK48 = 48,
	GROUP_UNK54 = 54
};

#define OBJPRIORITY_TRIGGER 40
#define OBJPRIORITY_PLAYER 60
#define OBJPRIORITY_DEFAULT 80
#define OBJPRIORITY_MOBILE_MAP 90
#define OBJPRIORITY_ANIM 100

#define OBJSTATE_UNK_ATTACH_INDEX_MASK 0x3
#define OBJSTATE_NEXT_MODEL_INDEX_MASK 0x700
#define OBJSTATE_NEXT_MODEL_INDEX_OFFSET 8

// Note: bits 0-1 and 8-10 are integers and not flags
enum ObjectStateFlags {
    // For mobile map (world) objects, their hit lines will be deactivated
    OBJSTATE_DISABLE_MODLINES = 0x4,
    // For mobile map (world) objects, the created parent view-projection matrix
    // (used by objects in the mobile map) will not include the world object's
    // own scale (i.e. it will be implied to be 1.0).
    OBJSTATE_WORLD_MTX_IGNORE_SCALE = 0x8,
    // Object is in the global object list
    OBJSTATE_STANDALONE = 0x10,
    OBJSTATE_UNK20 = 0x20, // unused?
    // Object was destroyed (not necessarily freed just yet)
    OBJSTATE_DESTROYED = 0x40,
    OBJSTATE_UNK80 = 0x80,
    // If set, bits 8-10 are the model index to switch to
    OBJSTATE_PENDING_MODEL_SWITCH = 0x800,
    OBJSTATE_IN_SEQ = 0x1000,
    // Update function will not be run
    OBJSTATE_UPDATE_DISABLED = 0x2000,
    // Print function will not be run (does not necessarily mean object is invisible)
    OBJSTATE_PRINT_DISABLED = 0x4000,
    // Control function will not be run
    OBJSTATE_CONTROL_DISABLED = 0x8000
};

// base objdata of objects in group 16?
typedef struct {
/*0000*/ struct Object *unk0;
/*0004*/ u8 unk4[0x9C - 0x4];
/*009C*/ u8 unk9C;
} ObjectAnim_Data;

// objdata of object in Object::unkC0
typedef struct {
/*0000*/    u8 unk0[0x62 - 0x0];
/*0062*/    s8 unk62;
} ObjectC0_Data;

// Stored in the flags of Object.srt
enum ObjectFlags {
/*1*/ OBJFLAG_UNK_2 = 0x2,

      // don't automatically calculate previous local/global position object fields
/*3*/ OBJFLAG_MANUAL_PREV_POSITIONS = 0x8,

/*6*/ OBJFLAG_UNK_40 = 0x40,
/*7*/ OBJFLAG_FORCE_TRANSPARENT_DRAW_ORDER = 0x80,

      // don't add model display list to the main display list when drawing
/*9*/ OBJFLAG_SKIP_MODEL_DL = 0x200,

/*12*/ OBJFLAG_SHADOW_ONLY = 0x1000,
       // whether the associated ObjSetup should be freed when the object is freed
       // TODO: this flag means more, see magicdust dll
/*13*/ OBJFLAG_OWNS_SETUP = 0x2000,
       // also disables hitsphere detection?
/*14*/ OBJFLAG_INVISIBLE = 0x4000
};

enum ObjInitFlags {
    // Add object to the global object list
    OBJINIT_STANDALONE = 0x1,
    // Whether the object ID provided for object creation is actually a direct index into OBJECTS.tab
    OBJINIT_BY_TABIDX = 0x2,
    // Sets whether the object owns its setup pointer
    OBJINIT_FLAG4 = 0x4
};

enum ObjSetupLoadFlags {
    // never unload? unless map unloads or setup changes.
    // skips check for being outside block grid span.
    //
    // called "level" by default.dol
    OBJSETUP_LOAD_LEVEL = 0x1,
    // never unload? unless map unloads or setup changes (way less common).
    // can still unload via object groups, but none also set 0x10 in practice (at least not in maps).
    // skips check for being outside block grid span.
    //
    // called "manual" by default.dol
    OBJSETUP_LOAD_MANUAL = 0x2,
    // for distance check, use player position instead of camera(?).
    //
    // called "main" by default.dol
    OBJSETUP_LOAD_MAIN = 0x4,
    // called "camera" by default.dol
    OBJSETUP_LOAD_CAMERA = 0x8,
    // load while the map object group the object is in is enabled.
    //
    // called "group" by default.dol
    OBJSETUP_LOAD_IN_MAP_OBJGROUP = 0x10,
    // skip unload check for distance to player/camera
    OBJSETUP_LOAD_FLAG20 = 0x20
};

enum ObjSetupFadeFlags {
    // Disable distance based object fadeout.
    //
    // Called "manual" by default.dol.
    OBJSETUP_FADE_MANUAL = 0x1,
    // Use player position to calculate fade, instead of the camera position.
    //
    // Called "main" by default.dol.
    OBJSETUP_FADE_MAIN = 0x2,
    // Called "camera" by default.dol.
    OBJSETUP_FADE_CAMERA = 0x4,
    OBJSETUP_FADE_FLAG8 = 0x8
};

// Base struct, objects "inherit" from this and add their own setup info.
// Note: Curves and race checkpoints are an exception and use a different version of this base struct.
//
// Aka. "romdef"
typedef struct ObjSetup {
        // "type" (default.dol)
/*00*/  s16 objId;
/*02*/  u8 quarterSize;
        // Bits 0-7: Exclude from map acts 1-8
/*03*/  u8 actExclusions1;
/*04*/  u8 loadFlags;
/*05*/  union {
            u8 byte5;
            // Bits 7-4 (note the reversal): Exclude from map acts 9-12
            u8 actExclusions2;
            // Bits 0-3: Fade flags
            // "viewflags" (default.dol)
            u8 fadeFlags;
        };
/*06*/  union {
            u8 byte6;
            // If loadFlags & 0x10 IS set, the map object group this object is a part of.
            u8 mapObjGroup;
            // If loadFlags & 0x10 is NOT set, maximum distance object is loaded at (divided by 8).
            // "load range" (default.dol)
            u8 loadDistance;
        };
        // Max distance object is visible at (divided by 8).
        // "view range" (default.dol)
/*07*/  u8 fadeDistance;
/*08*/  f32 x;
/*0c*/  f32 y;
/*10*/  f32 z;
        // Unique ID within map (not globally unique across all maps).
        // Just called "ID" in default.dol.
/*14*/  s32 uID;
} ObjSetup;

typedef struct {
/*0000*/    u8 unk0[0x10 - 0x0];
/*0010*/    u8 *unk10;
/*0014*/    u8 unk14[0x44 - 0x14];
/*0044*/    s32 unk44;
/*0048*/    u8 unk48[0x5d - 0x48];
/*005D*/    s8 unk5D;
/*005E*/    u8 unk5E[0x71 - 0x5e];
/*0071*/    u8 unk71;
/*0072*/    u8 unk72;
} ObjectStruct50;

#define OBJ_SHADOW_FLAG_GET_TEX_SLOT(flags) (flags & 0x3)
#define OBJ_SHADOW_FLAG_MAKE_TEX_SLOT(slot) (slot & 0x3)

// Note: First two bits are the dynamic shadow texture slot index and not flags.
enum ObjShadowFlags {
    // Whether the shadow is enabled.
    OBJ_SHADOW_FLAG_ENABLED = 0x4,
    // Use dynamic shadow texture rendering.
    // If set, the first two bits of the flags are the texture slot.
    OBJ_SHADOW_FLAG_DYNAMIC_TEX = 0x8,
    // Use custom shadow direction.
    OBJ_SHADOW_FLAG_CUSTOM_DIR = 0x10,
    // Use custom object position when calculating shadow.
    OBJ_SHADOW_FLAG_CUSTOM_OBJ_POS = 0x20,
    // Use custom shadow color.
    OBJ_SHADOW_FLAG_CUSTOM_COLOR = 0x40,
    // Disables Z-buffering when rendering the shadow texture to a surface.
    OBJ_SHADOW_FLAG_NO_Z_BUFFER = 0x80, 
    // ?
    OBJ_SHADOW_FLAG_100 = 0x100,
    // Rotate shadow with object yaw.
    OBJ_SHADOW_FLAG_USE_OBJ_YAW = 0x200,
    // Use custom opacity.
    OBJ_SHADOW_FLAG_CUSTOM_OPACITY = 0x400,
    // Top down shadow angle, i.e. always assume the light source is right above the object.
    // Requires custom direction to be enabled.
    OBJ_SHADOW_FLAG_TOP_DOWN = 0x800,
    // If set, the shadow fades out. If unset after, the shadow fades back in.
    OBJ_SHADOW_FLAG_FADE_OUT = 0x1000,
    // Not used by shadow renderer.
    OBJ_SHADOW_FLAG_2000 = 0x2000,
    // Not used by shadow renderer.
    OBJ_SHADOW_FLAG_4000 = 0x4000,
    // Related to opacity?
    OBJ_SHADOW_FLAG_8000 = 0x8000,
    // Prevents shadow from fading back in if set. 
    // Ignored if the object is in group 1 (the player's group).
    OBJ_SHADOW_FLAG_PREVENT_FADE_IN = 0x10000,
    // Not used by shadow renderer.
    OBJ_SHADOW_FLAG_20000 = 0x20000,
    // Only cast shadow on the surface of water.
    // If not set, the shadow will ignore the water surface and instead cast below on the ground.
    OBJ_SHADOW_FLAG_WATER_SURFACE = 0x40000
};

typedef struct {
/* 0x0 */ f32 scale;
/* 0x4 */ Texture *texture;
/* 0x8 */ Texture *unk8; // unused?
/* 0xC */ Gfx *gdl;
/* 0x10 */ Gfx *gdl2;
/* 0x14 */ Vec3f dir; // Direction of the shadow (effectively the negation of the light angle).
/* 0x20 */ Vec3f tr; // If the "custom object position" flag is set, this position will be used instead of the object's position.
           // Scales how far away the shadow can render.
           // Technically, this affects how much surrounding geometry is searched when building lift planes.
/* 0x2C */ f32 maxDistScale;
/* 0x30 */ u32 flags; // ObjShadowFlags
/* 0x34 */ u16 unk34; // unused?
/* 0x36 */ s16 visibility; // How visible the shadow is (on a scale from 0-64).
           // Camera distance to object between start-end fades between max-min opacity.
           // i.e. dist <= start = max opacity, dist >= end = min opacity.
           // Ignored if a custom opacity is in use.
/* 0x38 */ u8 distFadeStart;
/* 0x39 */ u8 distFadeEnd;
/* 0x3A */ u8 distFadeMaxOpacity;
/* 0x3B */ u8 distFadeMinOpacity;
/* 0x3c */ u8 r; // Requires custom shadow color to be enabled.
/* 0x3d */ u8 g; // Requires custom shadow color to be enabled.
/* 0x3e */ u8 b; // Requires custom shadow color to be enabled.
/* 0x3f */ u8 a; // Requires custom shadow color to be enabled.
/* 0x40 */ u8 opacity; // Requires custom opacity to be enabled. Ignored if custom color is enabled.
/* 0x41 */ s8 bufferIdx; // Shadows toggle this each frame (front/back buffer)
} ObjectShadow;

typedef struct {
    s32 size; // size of data
    void *data;
} BinFileEntry;

typedef BinFileEntry ObjectEvent;

typedef struct {
/*0000*/    u8 unk0[0x80 - 0x00];
/*0080*/    MtxF unk80[2];              // probably length of 2
/*0100*/    struct Object *unk100[3];   // other Objects colliding with this Object?
/*010c*/    u8 unk10C;                  // index into field 0x80?
/*010d*/    u8 unk10D;
/*010e*/    UNK_TYPE_8 unk10E;
/*010f*/    s8 unk10F;                  // number of Objects listed in field unk100?
} ObjectPolyhits;

typedef struct {
    Vec3f drawPoint; //Coord to draw the LockIcon at (worldSpace)
    Vec3f refPoint;  //Coord to use for player-to-Object distance check (worldSpace)
} ObjectStruct74; //maybe "LockIconCoords"?

//Parameters for the Object's LockIcon
typedef struct {
/*0000*/    u8 interactRadius;  //Interact radius (LockIcon over Object, can interact with A) (stored divided by 4)
/*0001*/    u8 lockExitRadius;  //Lock-on exit radius (lock-on auto-released)
/*0002*/    u8 hlRadius;        //Highlight radius (LockIcon over Object, but greyed out) (stored divided by 4)
/*0003*/    u8 hlAngularRange;  //Highlight angular range (LockIcon only appears over Object while player within angular slice)
/*0004*/    u8 flags;           //LockIcon colour, and bit 6 also related to VoxMaps (TO-DO: figure out purpose)
} ObjectStruct78; //maybe "LockIconData"?

typedef struct {
/*0000*/    u32 id;
/*0004*/    struct Object *sender;
/*0008*/    void *arg;
} ObjMesgQueueMessage;

typedef struct {
/*0000*/    u32 count; // current number of messages in queue
/*0004*/    u32 capacity; // max number of messages
/*0008*/    ObjMesgQueueMessage queue[1]; // variable length
} ObjectMesgQueue;

// Base interface for object DLLs
//
// These initial 7 functions are inherited and implemented by all object DLLs.
// Any exports referenced with a higher index are specific to an object or subtype.
DLL_INTERFACE(DLL_IObject) {
    /*:*/ DLL_INTERFACE_BASE(DLL);
          // reset: true if this is not the first setup call and the object should only reset state and not perform init allocations
    /*0*/ void (*setup)(struct Object *obj, ObjSetup *setup, s32 reset);
    /*1*/ void (*control)(struct Object *obj);
    /*2*/ void (*update)(struct Object *obj);
          // visibility: true if the object passed distance fade and frustum culling checks
    /*3*/ void (*print)(struct Object *obj, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);
          // onlySelf: true if the object should only free itself and not child/associated objects (not including linked objects)
    /*4*/ void (*free)(struct Object *obj, s32 onlySelf);
    /*5*/ u32 (*get_model_flags)(struct Object *obj);
          // offset: 
          // - 1st call: byte offset from the start of the object's memory to the allocated data.
          // - 2nd call: address of the object's allocated data.
    /*6*/ u32 (*get_data_size)(struct Object *obj, u32 offsetAddr);
};

// arg3 and arg4 are unknown types
// always called by DLL 3 "ANIM" during cutscenes?
struct AnimObj_Data;
typedef int (*AnimationCallback)(struct Object *, struct Object *, struct AnimObj_Data *, s8);

/**
 * Game object instance.
 * 
 * Size: 0xe4, other object-related data is placed in the following memory
 */
typedef struct Object {
/*0000*/    SRT srt; // local space coordinates
/*0018*/    Vec3f globalPosition; // world space coordinates
/*0024*/    Vec3f velocity;
/*0030*/    struct Object *parent; // transform is relative to this object. doesn't form a strict hierarchy
/*0034*/    u8 mobileMapID; //self-mapID for mobile map objects (e.g. Galleon)
/*0035*/    s8 matrixIdx;
/*0036*/    u8 opacity;
/*0037*/    u8 opacityWithFade; // combination of opacity and object fade out due to distance
/*0038*/    struct Object *next; // the object after this in a linked list
/*003C*/    f32 loadDistance; // Note: If setup loadFlags & 0x10 is set, this value is garbage
/*0040*/    f32 fadeDistance;
/*0044*/    s16 group; // complete guess at a name, needs more investigation
/*0046*/    s16 id; // called "romdefno" in default.dol
/*0048*/    s16 tabIdx; // index of ObjDef in OBJECTS.TAB
/*004A*/    u8 unk4A[0x4c - 0x4a];
/*004C*/    ObjSetup *setup; // exact type depends on object (called "def" in default.dol)
/*0050*/    ObjDef* def; // called "objdata" in default.dol
/*0054*/    ObjectHitInfo* objhitInfo;
/*0058*/    ObjectPolyhits *polyhits;
/*005C*/    BinFileEntry *unk5C;
/*0060*/    ObjectEvent *curEvent;
/*0064*/    ObjectShadow* shadow;
/*0068*/    DLL_IObject *dll;
/*006C*/    s16 (*unk6C)[9];
/*0070*/    Vtx* unk70;
/*0074*/    ObjectStruct74* unk74;
/*0078*/    ObjectStruct78 *unk78; // related to ObjDef.unk40
/*007C*/    ModelInstance **modelInsts; // called "frames" in default.dol
/*0080*/    Vec3f prevLocalPosition; // srt position from previous tick
/*008C*/    Vec3f prevGlobalPosition; // global position from previous tick
/*0098*/    f32 animProgress;
/*009C*/    f32 animProgressLayered;
/*00A0*/    s16 curModAnimId;
/*00A2*/    s16 curModAnimIdLayered;
/*00A4*/    f32 depthSortVal; //angle between camera and Object?
/*00A8*/    f32 visRadius; // radius of a sphere centered on the object, of which determines if the object is within the camera frustum
/*00AC*/    s8 mapID;
/*00AD*/    s8 modelInstIdx; // called "modelno" in default.dol
/*00AE*/    s8 updatePriority;
/*00AF*/    u8 unkAF; //Target arrow flags (see InteractionArrowFlags)
/*00B0*/    u16 stateFlags; // ObjectStateFlags
/*00B2*/    s16 unkB2;
/*00B4*/    s16 unkB4;
/*00B6*/    u8 unkB6[2];
/*00B8*/    void* data; //type depends on object
/*00BC*/    AnimationCallback animCallback; // some kind of cutscene anim callback?
/*00C0*/    struct Object *unkC0; // related to group 16 objects?
/*00C4*/    struct Object* unkC4; // parent object (called "parentobject" in default.dol)
/*00C8*/    struct Object *linkedObject; // child? the linked object's parent is not necessarily set to the current object
/*00CC*/    ObjectMesgQueue *mesgQueue;
/*00D0*/    u8 unkD0[0xd4 - 0xd0];
/*00D4*/    u8 unkD4; // index into ObjDef.unk40
/*00D5*/    u8 unkD5[0xd6 - 0xd5];
/*00D6*/    u8 unkD6;
/*00D5*/    u8 unkD7[0xd8 - 0xd7];
/*00D8*/    u8 unkD8;
/*00D9*/    u8 unkD9;
/*00DA*/    u8 freeLock; // if set, object will not be freed even if attempted
/*00DB*/    u8 unkDB[0xdc - 0xdb];
/*00DC*/    s32 unkDC; // sometimes stores ID related to object's active sequence?
/*00E0*/    s32 unkE0; // lifetime?
} Object;

typedef struct ObjListItem {
    s16 count;
    s16 size;
    Object *obj;
} ObjListItem;

typedef struct {
/*00*/ s8 unk0[0x84 - 0];
/*84*/ u8 unk84;
} ObjectAnim_Data_2;

/** Used to load Tricky/Kyte's object 
  * 
*/
typedef struct {
/*00*/  ObjSetup base;
/*18*/  u8 unk18;
/*19*/  u8 unk19;
} SidekickSetup;

extern struct Object * object_pointer_array[]; //first is always player character.
extern u16 objectCount;

#endif
