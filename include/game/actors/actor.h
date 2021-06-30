typedef struct ObjDef {
	/*00*/ s16 objType; //ObjDefEnum
	/*02*/ u8 allocatedSize; //size of this entry in words
	/*03*/ u8 mapStates1; //whether to NOT load in states 1-8
	/*04*/ u8 loadFlags; //RomListObjLoadFlags
	/*05*/ u8 mapStates2; //whether to NOT load in states 15-9 (low bit unused?)
	/*06*/ u8 bound; //load if in range (bound<<3) of player
	/*07*/ u8 unk7;
	/*08*/ Vec3f pos;
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
/*003C*/  u8 unk0x3c[3];
/*0040*/  UNK_TYPE_32 unk0x40;
/*0044*/  UNK_TYPE_16 unk0x44;
/*0046*/  UNK_TYPE_16 unk0x46;
/*0048*/  void* ptr0x48;
/*004C*/  UNK_TYPE_32 unk0x4c;
/*0050*/  s16* ptr0x50;
/*0054*/  void* ptr0x54;
/*0058*/  UNK_TYPE_32 unk0x58;
/*005C*/  void* ptr0x5c;
/*0060*/  void* ptr0x60;
/*0064*/  void* ptr0x64;
/*0068*/  DllInstance **dll;
/*006C*/  void* ptr0x6c;
/*0070*/  void* ptr0x70;
/*0074*/  UNK_TYPE_32 unk0x74;
/*0078*/  UNK_TYPE_32 unk0x78;
/*007C*/  Model **models;
/*0080*/  Vec3f positionMirror2; //gets copied twice.
/*008C*/  Vec3f positionMirror3; //not sure why.
/*0098*/  float unk98;
/*009C*/  u8 unk0x9c;
/*009D*/  u8 unk0x9d[3]; //aligning?
/*00A0*/  s16 unk0xa0;
/*00A2*/  u8 unk0xa2[11];
/*00AD*/  s8 curModel;
/*00AE*/  u8 unk0xae;
/*00AF*/  u8 unk0xaf;
/*00B0*/  s16 unk0xb0;
/*00B2*/  u8 unk0xb2[6];
/*00B8*/  void* state; //type depends on object
/*00BC*/  UNK_TYPE_32 unk0xbc;
/*00C0*/  UNK_TYPE_32 unk0xc0;
/*00C4*/  UNK_TYPE_32 unk0xc4;
/*00C8*/  void* ptr0xc8;
/*00CC*/  void* ptr0xcc;
} TActor; //may be bigger, will know when constructor is understood.

extern struct TActor * object_pointer_array[]; //first is always player character.
extern u16 objectCount;
