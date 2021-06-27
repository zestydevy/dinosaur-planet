//prelimnary, lots of unknowns
//contains pointer-to-own-type fields, so `typedef struct _TActor {`
//must be used instead of `typedef struct {`
typedef struct TActor {
/*0000*/  Vec3s rotation;
/*0006*/  s16 unk0x6;
/*0008*/  float scale;
/*000C*/  Vec3f position; //note: >300 unit drop causes fall damage.
	Vec3f positionMirror; //local vs global?
	Vec3f speed;
	void* ptr0x30;
	UNK_TYPE_32 unk0x34;
	struct TActor* linkedActor;
	u8 unk0x3c[3];
	UNK_TYPE_16 unk0x44;
	UNK_TYPE_16 unk0x46;
	void* ptr0x48;
	UNK_TYPE_32 unk0x4c;
	void* ptr0x50;
	void* ptr0x54;
	UNK_TYPE_32 unk0x58;
	void* ptr0x5c;
	void* ptr0x60;
	void* ptr0x64;
	void* ptr0x68;
	void* ptr0x6c;
	void* ptr0x70;
	UNK_TYPE_32 unk0x74;
	Vec3f positionMirror2; //gets copied twice.
	Vec3f positionMirror3; //not sure why.
	float unk0x98;
	u8 unk0x9c;
	u8 unk0x9d[3]; //aligning?
	s16 unk0xa0;
	u8 unk0xa2[11];
	u8 unk0xad;
	u8 unk0xae;
	u8 unk0xaf;
	s16 unk0xb0;
	u8 unk0xb2[6];
	void* CharData; //0x8c1+ struct, has various player data.
	UNK_TYPE_32 unk0xbc;
	UNK_TYPE_32 unk0xc0;
	UNK_TYPE_32 unk0xc4;
	void* ptr0xc8;
	void* ptr0xcc;
} TActor; //may be bigger, will know when constructor is understood.

//pointer at 0x34c of "CharData" struct
struct CharacterStats{
	u8 unk0x0;
	s8 HPCurr;
	s8 HPMax; //only shows up to 104, code caps it at 80.
	u8 unk0x3; //aligning?
	s16 ManaCurr; //only mods when byte at "CharData"0x8bb is set.
	s16 ManaMax; //capped at 100.
	s16 Scarabs; //capped at 999.
	u8 unk0xa;
	u8 unk0xb;
};//seemingly consistent addrs: Fox/Sabre at 0x805c3964, Krystal 0x805c3970.

extern struct CharacterStats * charStats_pointer;
extern struct TActor * object_pointer_array[]; //first is always player character.
extern u16 objectCount;
