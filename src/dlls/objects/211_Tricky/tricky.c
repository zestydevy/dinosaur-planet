#include "common.h"

typedef struct {
    u8 _unk0[0x610];
} DLL211_Data;

/*0x0*/ static u32 _data_0[] = {
    0x06d70500, 0x000001dc, 0x05000000
};
/*0xC*/ static u32 _data_C[] = {
    0x095b0100, 0x00000037, 0x10000000, 0x00381000, 0x00000039, 0x10000000
};
/*0x24*/ static u32 _data_24[] = {
    0x04b81500, 0x0014002a, 0x01000000, 0x002b0100, 0x0000002c, 0x01000000, 0x00340100, 0x00000035, 
    0x01000000, 0x00360100, 0x0000003a, 0x01000000, 0x003b0100, 0x0000003c, 0x01000000, 0x003d0100, 
    0x0000003e, 0x01000000, 0x003f1500, 0x00000000
};
/*0x74*/ static u32 _data_74[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x41880000, 0x00000000
};
/*0x8C*/ static u32 _data_8C[] = {
    0x00000000, 0x41080000
};
/*0x94*/ static u32 _data_94[] = {
    0x00000000, 0x00000000, 0x00000000
};
/*0xA0*/ static u32 _data_A0 = 0x41000000;
/*0xA4*/ static u32 _data_A4[] = {
    0x00000000, 0x3ea8f5c3, 0x3f7d70a4, 0x3ffd70a4, 0x40533333, 0x409e6666, 0x40ddc28f, 0x4113d70a, 
    0x413e147b, 0x416d999a, 0x419170a4, 0x41aeb852, 0x41cea3d7, 0x41f13333, 0x420b3333, 0x421f1eb8, 
    0x42345c29, 0x424aeb85
};
/*0xEC*/ static u32 _data_EC[] = {
    0x00000200, 0x04000007, 0x07000a00, 0x000d0000, 0x00000013, 0x00000000, 0x00001a1c, 0x1c000000, 
    0x00000023, 0x00000000, 0x00000000, 0x00001313, 0x00000000, 0x02000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};
/*0x164*/ static u32 _data_164 = 0x01020000;
/*0x168*/ static u32 _data_168[] = {
    0x00000000, 0x00000001, 0x00000002, 0x00000003, 0x00000004, 0x00000005
};

/*0x0*/ static u8 _bss_0[0x8];
/*0x8*/ static u8 _bss_8[0x8];
/*0x10*/ static u8 _bss_10[0x40];

// offset: 0x0 | ctor
void dll_211_ctor(void *dll);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_ctor.s")

// offset: 0xF8 | dtor
void dll_211_dtor(void *dll) { }

// offset: 0x104 | func: 0 | export: 0
void dll_211_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_setup.s")

// offset: 0x3FC | func: 1 | export: 1
void dll_211_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_control.s")

// offset: 0xD1C | func: 2 | export: 2
void dll_211_update(Object *self) { }

// offset: 0xD28 | func: 3 | export: 3
void dll_211_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_print.s")

// offset: 0xF20 | func: 4 | export: 4
void dll_211_free(Object *self, s32 a1);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_free.s")

// offset: 0x1040 | func: 5 | export: 5
u32 dll_211_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x1050 | func: 6 | export: 6
u32 dll_211_get_data_size(Object *self, u32 a1) {
    return sizeof(DLL211_Data);
}

// offset: 0x1064 | func: 7 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1064.s")

// offset: 0x1074 | func: 8 | export: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1074.s")

// offset: 0x108C | func: 9 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_108C.s")

// offset: 0x109C | func: 10 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_109C.s")

// offset: 0x10B4 | func: 11 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_10B4.s")

// offset: 0x10C4 | func: 12 | export: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_10C4.s")

// offset: 0x10D4 | func: 13 | export: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_10D4.s")

// offset: 0x1230 | func: 14 | export: 26
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1230.s")

// offset: 0x1248 | func: 15 | export: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1248.s")

// offset: 0x1270 | func: 16 | export: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1270.s")

// offset: 0x1284 | func: 17 | export: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1284.s")

// offset: 0x1298 | func: 18 | export: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1298.s")

// offset: 0x12B4 | func: 19 | export: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_12B4.s")

// offset: 0x12C8 | func: 20 | export: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_12C8.s")

// offset: 0x12D8 | func: 21 | export: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_12D8.s")

// offset: 0x12F0 | func: 22 | export: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_12F0.s")

// offset: 0x1398 | func: 23 | export: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1398.s")

// offset: 0x1408 | func: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1408.s")

// offset: 0x14E8 | func: 25 | export: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_14E8.s")

// offset: 0x1550 | func: 26 | export: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1550.s")

// offset: 0x1578 | func: 27 | export: 23
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1578.s")

// offset: 0x1624 | func: 28
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1624.s")

// offset: 0x183C | func: 29
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_183C.s")

// offset: 0x18C4 | func: 30
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_18C4.s")

// offset: 0x1F3C | func: 31
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_1F3C.s")

// offset: 0x23B8 | func: 32
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_23B8.s")

// offset: 0x2928 | func: 33
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_2928.s")

// offset: 0x2938 | func: 34
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_2938.s")

// offset: 0x2B50 | func: 35
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_2B50.s")

// offset: 0x2C64 | func: 36
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_2C64.s")

// offset: 0x3188 | func: 37
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_3188.s")

// offset: 0x3560 | func: 38
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_3560.s")

// offset: 0x3570 | func: 39
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_3570.s")

// offset: 0x3664 | func: 40
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_3664.s")

// offset: 0x3A78 | func: 41
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_3A78.s")

// offset: 0x3C1C | func: 42
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_3C1C.s")

// offset: 0x3CAC | func: 43
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_3CAC.s")

// offset: 0x4340 | func: 44
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_4340.s")

// offset: 0x43AC | func: 45
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_43AC.s")

// offset: 0x4418 | func: 46
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_4418.s")

// offset: 0x47E8 | func: 47
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_47E8.s")

// offset: 0x4974 | func: 48
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_4974.s")

// offset: 0x4C94 | func: 49
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_4C94.s")

// offset: 0x4F3C | func: 50
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_4F3C.s")

// offset: 0x507C | func: 51
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_507C.s")

// offset: 0x514C | func: 52
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_514C.s")

// offset: 0x52B8 | func: 53
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_52B8.s")

// offset: 0x53E4 | func: 54
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_53E4.s")

// offset: 0x7188 | func: 55
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_7188.s")

// offset: 0x726C | func: 56
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_726C.s")

/*0x0*/ static const char str_0[] = "%d %d %d %d\n";
/*0x10*/ static const char str_10[] = "hits: %d %d %d %d %d %d %d %d";
/*0x30*/ static const char str_30[] = "\nEnergy: %d, Skill %d\n";
/*0x48*/ static const char str_48[] = "find command used on the wrong object\n";
/*0x70*/ static const char str_70[] = "find command used on the wrong object\n";
/*0x98*/ static const char str_98[] = "sideCommandEnable error, command number too high\n";
/*0xCC*/ static const char str_CC[] = "sideFind 1\n";
/*0xD8*/ static const char str_D8[] = "sideFind 2\n";
/*0xE4*/ static const char str_E4[] = "Couldn't find a distract node\n";
/*0x104*/ static const char str_104[] = "DISTRACT_SPOTFINDING (1)\n";
/*0x120*/ static const char str_120[] = "DISTRACT_SPOTTOSPOT (1)\n";
/*0x13C*/ static const char str_13C[] = "DISTRACT_SPOTDISTRACTING (1)\n";
/*0x15C*/ static const char str_15C[] = "DISTRACT_SPOTRETURING (1)\n";
/*0x178*/ static const char str_178[] = "DISTRACT_SPOTFINDING (2)\n";
/*0x194*/ static const char str_194[] = "DISTRACT_SPOTDISTRACTING (2)\n";
/*0x1B4*/ static const char str_1B4[] = "FLAME_NONE\n";
/*0x1C0*/ static const char str_1C0[] = "FLAME_FINDING_OUT\n";
/*0x1D4*/ static const char str_1D4[] = "FLAME_GOINGTOEDGE\n";
/*0x1E8*/ static const char str_1E8[] = "FLAME_TOSTART\n";
/*0x1F8*/ static const char str_1F8[] = "FLAME_OUT\n";
/*0x204*/ static const char str_204[] = "FLAME_FINDING_IN\n";
/*0x218*/ static const char str_218[] = "FLAME_IN\n";
/*0x224*/ static const char str_224[] = "FLAME_TOEND\n";
/*0x234*/ static const char str_234[] = "GUARD_INIT\n";
/*0x240*/ static const char str_240[] = "GUARD_FINDING\n";
/*0x250*/ static const char str_250[] = "GUARD_TOSPOT\n";
/*0x260*/ static const char str_260[] = "GUARD_TOFRONT\n";
/*0x270*/ static const char str_270[] = "GUARD_TOBADDIE\n";
/*0x280*/ static const char str_280[] = "GUARD_FLAME\n";
/*0x290*/ static const char str_290[] = "GUARD_DOWNTOGROWL\n";
/*0x2A4*/ static const char str_2A4[] = "GUARD_GROWL\n";
/*0x2B4*/ static const char str_2B4[] = "GUARD_UPFROMGROWL\n";
/*0x2C8*/ static const char str_2C8[] = "error tricky should never stop when circling\n";
/*0x2F8*/ static const char str_2F8[] = "BADDIEALERT_GOTO\n";
/*0x30C*/ static const char str_30C[] = "BADDIEALERT_BARK %d %d\n";
/*0x324*/ static const char str_324[] = "BADDIEALLERT_GOTOFLAME\n";
/*0x33C*/ static const char str_33C[] = "BADDIEALLERT_FLAME\n";
/*0x350*/ static const char str_350[] = "GROWLAT_GOTO\n";
/*0x360*/ static const char str_360[] = "GROWLAT_GROWLING\n";
/*0x374*/ static const char str_374[] = "GROWLAT_GOTOFLAME\n";
/*0x388*/ static const char str_388[] = "GROWLAT_FLAME\n";
/*0x398*/ static const char str_398[] = "tricky wg %d->%d target wg %d, dest wg %d\n";
/*0x3C4*/ static const char str_3C4[] = "tricky last walk group is zero. Has he been loaded within a walk group? %f %f %f\n";
/*0x418*/ static const char str_418[] = "target is within a walkGroup or its patch\n";
/*0x444*/ static const char str_444[] = "target is not within a walkGroup or any patches\n";
/*0x478*/ static const char str_478[] = "target is within patch group %d\n";
/*0x49C*/ static const char str_49C[] = "Patch %d: Last xyz %f %f %f\n";
/*0x4BC*/ static const char str_4BC[] = "Last Patch Point %f %f %f\n";
/*0x4D8*/ static const char str_4D8[] = "tricky error, target patch %d, targetWalkGroup %d, trickyWalkGroup %d, tricky last walkGroup %d, tricky in patch %d\n";
/*0x550*/ static const char str_550[] = "tricky error 2!!!!!\n";
/*0x568*/ static const char str_568[] = "walk wait\n";
/*0x574*/ static const char str_574[] = "walk free\n";
/*0x580*/ static const char str_580[] = "walk start patch\n";
/*0x594*/ static const char str_594[] = "walk patch exit\n";
/*0x5A8*/ static const char str_5A8[] = "walk end patch\n";
/*0x5B8*/ static const char str_5B8[] = "about to turn\n";
/*0x5C8*/ static const char str_5C8[] = "large turn\n";
/*0x5D4*/ static const char str_5D4[] = "large turn\n";
/*0x5E0*/ static const char str_5E0[] = "standing\n";
/*0x5EC*/ static const char str_5EC[] = "walk to node %d %d\n";
/*0x600*/ static const char str_600[] = "walk types are %d %d %d\n";
/*0x61C*/ static const char str_61C[] = "curve setup\n";
/*0x62C*/ static const char str_62C[] = "walk nodes\n";
/*0x638*/ static const char str_638[] = "No route exists in the forward direction\n";
/*0x664*/ static const char str_664[] = "Reverse direction is quicker\n";
/*0x684*/ static const char str_684[] = "Reversing direction\n";
/*0x69C*/ static const char str_69C[] = "Jump run up\n";
/*0x6AC*/ static const char str_6AC[] = "Jump prep\n";
/*0x6B8*/ static const char str_6B8[] = "Jumping\n";
/*0x6C4*/ static const char str_6C4[] = "Jump up run up\n";
/*0x6D4*/ static const char str_6D4[] = "i before %d\n";
/*0x6E4*/ static const char str_6E4[] = "i now %d\n";
/*0x6F0*/ static const char str_6F0[] = "JUMPDOWN or JUMPUP\n";
/*0x704*/ static const char str_704[] = "JUMPDOWN_RUNUP\n";
/*0x714*/ static const char str_714[] = "TURNING\n";
/*0x720*/ static const char str_720[] = "entered a non valid movementstate\n";
/*0x744*/ static const char str_744[] = "tricky left a valid area, pushing him back...\n";
/*0x774*/ static const char str_774[] = "";
/*0x778*/ static const u32 _rodata_778[] = {
    0x40020100, 0x00000000, 0x00000000, 0x00000000, 0x40030102, 0x00000000, 0x00000000, 0x00000000, 
    0x40040506, 0x00000000, 0x00000000, 0x00000000, 0x40060507, 0x00000000, 0x00000000, 0x00000000, 
    0x40060302, 0x00000000, 0x00000000, 0x00000000, 0x40030607, 0x00000000, 0x00000000, 0x00000000, 
    0x40030701, 0x00000000, 0x00000000, 0x00000000, 0x40070501, 0x00000000, 0x00000000, 0x00000000, 
    0x40040001, 0x00000000, 0x00000000, 0x00000000, 0x40040105, 0x00000000, 0x00000000, 0x00000000, 
    0x40000402, 0x00000000, 0x00000000, 0x00000000, 0x40020406, 0x00000000, 0x00000000, 0x00000000
};
/*0x838*/ static const u32 _rodata_838[] = {
    0xc1400000, 0x41a00000, 0xc1a00000, 0x41400000, 0x41a00000, 0xc1a00000, 0xc1400000, 0x41a00000, 
    0x41a00000, 0x41400000, 0x41a00000, 0x41a00000
};
/*0x868*/ static const u32 _rodata_868[] = {
    0xc1400000, 0xc0e00000, 0xc1a00000, 0x41400000, 0xc0e00000, 0xc1a00000, 0xc1400000, 0xc0e00000, 
    0x41a00000, 0x41400000, 0xc0e00000, 0x41a00000
};

// offset: 0x74C4 | func: 57
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_74C4.s")

// offset: 0x7794 | func: 58
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_7794.s")

// offset: 0x7A7C | func: 59
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_7A7C.s")

// offset: 0x7BAC | func: 60
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_7BAC.s")

// offset: 0x7DB8 | func: 61
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_7DB8.s")

// offset: 0x7EFC | func: 62
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_7EFC.s")

// offset: 0x8114 | func: 63
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_8114.s")

// offset: 0x81A8 | func: 64
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_81A8.s")

// offset: 0x81D8 | func: 65
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_81D8.s")

// offset: 0x82B8 | func: 66
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_82B8.s")

// offset: 0x8308 | func: 67
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_8308.s")

// offset: 0x83BC | func: 68
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_83BC.s")

// offset: 0x8470 | func: 69
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_8470.s")

// offset: 0x871C | func: 70
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_871C.s")

// offset: 0x87E4 | func: 71
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_87E4.s")

// offset: 0x885C | func: 72
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_885C.s")

// offset: 0x88F4 | func: 73
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_88F4.s")

// offset: 0x8974 | func: 74
void dll_211_func_8974(Object* arg0, UnkCurvesStruct* arg1, f32 arg2) {
    f32 square;
    f32 var_fs0;
    f32 distanceSquared;
    s32 i;
    
    distanceSquared = (arg2 * gUpdateRateF) * 1.5f;
    square = distanceSquared * distanceSquared;
    distanceSquared = vec3_distance_xz_squared((Vec3f *) (&arg1->unk68), &arg0->srt.transl);
    
    if (arg1->unk80 != 0){
        var_fs0 = -2.0f;
    } else {
        var_fs0 = 2.0f;
    }
    
    for (i = 0; i < 5; i++){
        if (square < distanceSquared){
            break;
        }
        func_800053B0(arg1, var_fs0);
        distanceSquared = vec3_distance_xz_squared((Vec3f *) (&arg1->unk68), &arg0->srt.transl);
    }
}

// offset: 0x8A94 | func: 75
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_8A94.s")

// offset: 0x8B5C | func: 76
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_8B5C.s")

// offset: 0x8BEC | func: 77
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_8BEC.s")

// offset: 0x8ED0 | func: 78
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_8ED0.s")

// offset: 0x8F18 | func: 79
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_8F18.s")

// offset: 0x8F84 | func: 80
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_8F84.s")

// offset: 0x9024 | func: 81
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_9024.s")

// offset: 0x9050 | func: 82
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_9050.s")

// offset: 0x9200 | func: 83
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_9200.s")

// offset: 0x940C | func: 84
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_940C.s")

// offset: 0x94BC | func: 85
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_94BC.s")

// offset: 0x95E0 | func: 86
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_95E0.s")

// offset: 0x9668 | func: 87
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/211_Tricky/dll_211_func_9668.s")
