//Entry in a romlist, defining one instance of an object.
typedef struct ObjDef {
	/*00*/ s16 objType; //ObjDefEnum (which object is this?)
	/*02*/ u8 allocatedSize; //size of this entry in words
	/*03*/ u8 mapStates1; //whether to NOT load in states 1-8
	/*04*/ u8 loadFlags; //RomListObjLoadFlags
	/*05*/ u8 mapStates2; //whether to NOT load in states 15-9 (low bit unused?)
	/*06*/ u8 bound; //load if in range (bound<<3) of player
	/*07*/ u8 unk7;
	/*08*/ Vec3f pos; //global position
	/*14*/ int id; //unique ID
} ObjDef;

//used on its own sometimes
typedef struct ObjPos {
/*0000*/  Vec3s rotation;
/*0006*/  s16 unk0x6;
/*0008*/  float scale;
/*000C*/  Vec3f position; //note: >300 unit drop causes fall damage.
} ObjPos;

//prelimnary, lots of unknowns
//contains pointer-to-own-type fields, so `typedef struct _TActor {`
//must be used instead of `typedef struct {`
typedef struct TActor {
/*0000*/  ObjPos pos;
/*0018*/  Vec3f positionMirror; //local vs global?
/*0024*/  Vec3f speed;
/*0030*/  void* ptr0x30;
/*0034*/  UNK_TYPE_32 unk0x34;
/*0038*/  struct TActor* linkedActor;
/*003C*/  u8 unk0x3c[3]; //float loadDistance?
/*0040*/  UNK_TYPE_32 unk0x40; //float related to camera?
/*0044*/  UNK_TYPE_16 objId; //guessed from SFA
/*0046*/  UNK_TYPE_16 defNo; //guessed from SFA
/*0048*/  void* ptr0x48;
/*004C*/  UNK_TYPE_32 unk0x4c; //ObjDef*?
/*0050*/  ObjData* data;
/*0054*/  void* ptr0x54; //HitState*?
/*0058*/  UNK_TYPE_32 unk0x58; //HitboxMtx*?
/*005C*/  void* ptr0x5c;
/*0060*/  void* ptr0x60; //pEventName?
/*0064*/  void* ptr0x64; //shadow?
/*0068*/  DllInstance **dll; //same in SFA
/*006C*/  void* ptr0x6c; //vectors?
/*0070*/  void* ptr0x70; //textures?
/*0074*/  UNK_TYPE_32 unk0x74; //related to targeting?
/*0078*/  UNK_TYPE_32 unk0x78; //related to camera targeting?
/*007C*/  Model **models; //same in SFA
/*0080*/  Vec3f positionMirror2; //gets copied twice.
/*008C*/  Vec3f positionMirror3; //not sure why.
/*0098*/  float animTimer; //guessed from SFA
/*009C*/  u8 unk0x9c; //float related to animation?
/*009D*/  u8 unk0x9d[3]; //aligning?
/*00A0*/  s16 curAnimId;
/*00A2*/  u8 unk0xa2[11];
/*00AD*/  s8 curModel; //same in SFA
/*00AE*/  u8 priority; //guessed from SFA
/*00AF*/  u8 unk0xaf; //flags
/*00B0*/  s16 flagsB0;
/*00B2*/  u8 unk0xb2[6];
/*00B8*/  void* state; //type depends on object
/*00BC*/  UNK_TYPE_32 unk0xbc; //func ptr
/*00C0*/  UNK_TYPE_32 unk0xc0; //obj ptr
/*00C4*/  struct TActor *parent; //guessed from SFA
/*00C8*/  struct TActor *children[3]; //guessed from SFA
} TActor; //may be bigger, will know when constructor is understood.

extern struct TActor * object_pointer_array[]; //first is always player character.
extern u16 objectCount;
