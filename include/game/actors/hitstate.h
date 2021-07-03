struct TActor;

//most of this is copied from SFA and will need to be corrected for DP

typedef struct {
    /*00*/ short animId;
    /*02*/ s8 unk02;
    /*03*/ s8 unk03;
    /*04*/ s8 unk04;
    /*05*/ s8 unk05;
} ObjHitsEntry;

enum HitstateFlags60 {
    HITSTATE_FLAG60_Disabled      = 0x0001,
    HITSTATE_FLAG60_NeedPosUpdate = 0x0040,
    HITSTATE_FLAG60_LockRotY      = 0x0800,
    HITSTATE_FLAG60_LockRotZ      = 0x1000
};

enum HitstateFlags62 {
    HITSTATE_FLAGS62_ScaleBySize     = 0x01,
    HITSTATE_FLAGS62_SizeFlag2       = 0x02,
    HITSTATE_FLAGS62_DontUpdate      = 0x08,
    HITSTATE_FLAGS62_UseModelField14 = 0x20
};

enum PolygonType {
    POLY_TYPE_Water       = 0x0e,
    POLY_TYPE_MetalOrRock = 0x19,
    NUM_POLY_TYPES        = 0x24
};

typedef struct HitState {
    /*00*/ struct TActor *curTouchedObject;
    /*04*/ short objHitsSize; //this >> 2 = #ptrs at field 8
    /*06*/ short maxHitsSize;
    /*08*/ ObjHitsEntry *objHits; //from OBJHITS.bin
    /*0C*/ float sizeXY_0C;
    /*10*/ Vec3f pos;
    /*1C*/ Vec3f prevPos;
    /*28*/ Vec3f size;
    /*34*/ float minSizeY;
    /*38*/ float sizeY;
    /*3C*/ Vec3f unk3C;
    /*48*/ UNK_TYPE_32 unk48;
    /*4C*/ u32 unk4C;
    /*50*/ struct TActor *lastTouchedObject;
    /*54*/ UNK_TYPE_32 unk54;
    /*58*/ u16 lowestDist; //never resets?
    /*5A*/ //short sizeXY; //commenting these to make the flags line up...
    /*5C*/ //short sizeX1; //need to verify which fields are actually missing
    /*5E*/ //short sizeX2;
    /*60*/ u16 flags_0x58; //XXX some of above fields must not be present
    /*62*/ u16 flags_0x5A; //in this version, but which?
    /*63*/ s8 unk63;
    /*64*/ short sizeZ;
    /*66*/ short sizeZ1;
    /*68*/ short sizeZ2;
    /*6A*/ s8 unk6A;
    /*6B*/ s8 unk6B;
    /*6C*/ s8 damage;
    /*6D*/ s8 unk6D;
    /*6E*/ s8 unk6E;
    /*6F*/ s8 unk6F;
    /*70*/ s8 unk70;
    /*71*/ s8 nHits; //max 3; only used for damage?
    /*72*/ s8 sphereIdxs[3];
    /*75*/ s8 objField6C[3];
    /*78*/ s8 recordedDamage[3];
    /*7B*/ s8 unk7B;
    /*7C*/ struct TActor *objs[3];
    /*88*/ float objX[3];
    /*94*/ float objY[3];
    /*A0*/ float objZ[3];
    /*AC*/ u8 typeOfPolyHit; //PolygonType
    /*AD*/ s8 unkAD;  
    /*AE*/ s8 disable;
    /*AF*/ s8 state;
    /*B0*/ s8 unkB0;
    /*B1*/ s8 unkB1;
    /*B2*/ short unkB2;
    /*B4*/ s8 unkB4;
    /*B5*/ s8 unkB5;
    /*B6*/ u16 flagsB6; //HitstateFlags62
} HitState;
