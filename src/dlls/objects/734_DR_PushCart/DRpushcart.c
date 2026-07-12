#include "common.h"
#include "sys/math.h"
#include "sys/objhits.h"
#include "sys/objtype.h"
#include "dlls/objects/338_LFXEmitter.h"

typedef struct {
    ObjSetup base;
    s8 unk18;
    s8 unk19;
    s32 unk1C;
    s32 unk20;
} CFBarrel_Setup; //0x24

typedef struct {
    ObjSetup base;
    s8 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
} DRPushCart_Setup;

typedef struct {
    UnkCurvesStruct unk0;
    Object* unk108;
    f32 unk10C;
    f32 unk110;
    f32 unk114;
    f32 unk118;
    s8 unk11C[0x14C - 0x11C];
    u8 unk14C;
    s8 unk14D[0x15C - 0x14D];
    u32 unk15C_31 : 1;
    u32 unk15C_30 : 1;
    u32 unk15C_29 : 1;
    u32 unk15C_28 : 4;
    u32 unk15C_24 : 1;
    u32 unk15C_23 : 1;
    u32 unk15C_22 : 1;
    u32 unk15C_21 : 1;
} DRPushCart_Data;

/*0x0*/ static Vec3f data_0[] = {
    VEC3F(10, 14, 1), 
    VEC3F(10, 14, -19), 
    VEC3F(10, 14, -19),
    VEC3F(-8, 14, 1),
    VEC3F(-8, 14, -19),
    VEC3F(-8, 14, -19),
};

static void dll_734_func_133C(Object* self, DRPushCart_Data* objData);
static void dll_734_func_1588(Object* self, s32 barrelCount);
static Object* dll_734_func_1624(Object* self, Vec3f* coord);

// offset: 0x0 | ctor
void dll_734_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_734_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_734_setup(Object* self, DRPushCart_Setup* objSetup, s32 arg2) {
    DRPushCart_Data* objData = self->data;
    
    self->srt.yaw = objSetup->unk18 << 8;
    
    objData->unk114 = objSetup->unk1A;
    objData->unk10C = 0.0f;
    objData->unk15C_29 = mainGetBits(objSetup->unk20);
    objData->unk15C_29 = 1;
    objData->unk15C_21 = 1;
    
    mainSetBits(BIT_649, 1);
    
    if (arg2 == 0) {
        dll_734_func_1588(self, mainGetBits(BIT_683));
    }
}

/*0x0*/ static const char str_0[] = " HELLO I AM THE PUSHCART";

// offset: 0x114 | func: 1 | export: 1
void dll_734_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/734_DR_PushCart/dll_734_control.s")

// offset: 0x7E0 | func: 2 | export: 2
void dll_734_update(Object* self) {
    if (func_80025F40(self, NULL, NULL, NULL) == Damage_Type_Explosion) {
        mainSetBits(BIT_822, 0);
        mainSetBits(BIT_823, 0);
        mainSetBits(BIT_824, 0);
        mainSetBits(BIT_825, 0);
        mainSetBits(BIT_826, 0);
        mainSetBits(BIT_827, 0);
    }
}

// offset: 0x8B8 | func: 3 | export: 3
void dll_734_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/734_DR_PushCart/dll_734_print.s")

// offset: 0xB8C | func: 4 | export: 4
void dll_734_free(Object* self, s32 arg1) {
    DRPushCart_Data* objData;

    objData = self->data;
    mainSetBits(BIT_683, objData->unk14C);
    
    if (arg1 == 0) {
        if (objData->unk108 != NULL) {
            objFreeObject(objData->unk108);
            objData->unk108 = NULL;
        }
    }
}

// offset: 0xC10 | func: 5 | export: 5
u32 dll_734_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0xC20 | func: 6 | export: 6
u32 dll_734_get_data_size(Object *self, u32 offsetAddr) {
    return sizeof(DRPushCart_Data);
}

// offset: 0xC34 | func: 7 | export: 7
f32 dll_734_func_C34(Object* self, s16* arg1, f32 arg2) {
    DRPushCart_Data* objData;
    s32 pad;
    f32 sp24;
    s16 angle;
    f32 sign;

    objData = self->data;
    
    if (objData->unk114) {
        sp24 = arg2 / objData->unk114;
    }
    
    angle = self->srt.yaw - (*arg1 & 0xFFFF);
    CIRCLE_WRAP(angle);

    if (-fcos16_precise(angle) < 0.0f) {
        sign = -1.0f;
    } else {
        sign = 1.0f;
    }
    objData->unk110 = sp24 * sign;
    
    return objData->unk10C;
}

// offset: 0xD30 | func: 8
void dll_734_func_D30(Object* self, s32 arg1) {
    LFXEmitter_Setup* lfxSetup;

    lfxSetup = (LFXEmitter_Setup*)objAllocSetup(sizeof(LFXEmitter_Setup), OBJ_LFXEmitter);
    lfxSetup->base.loadFlags = 2;
    lfxSetup->base.x = self->srt.transl.x;
    lfxSetup->base.y = self->srt.transl.y;
    lfxSetup->base.z = self->srt.transl.z;
    lfxSetup->unk1E = arg1;
    lfxSetup->unk22 = -1;
    objSetupObject((ObjSetup*)lfxSetup, 5, self->mapID, -1, self->parent);
}

// offset: 0xDD8 | func: 9
s32 dll_734_func_DD8(Object* self, u8 arg1, u8 arg2, s32* arg3) {
    DRPushCart_Data* objData;
    Object* player;
    f32 temp;

    objData = self->data;
    player = objGetPlayer();
    
    *arg3 = -1;
    
    switch (arg1) {
    case 1:
        dll_734_func_133C(self, objData);
    default:
        break;
    case 2:
        break;
    case 3:
        if (!(objData->unk15C_30) && !(objData->unk10C <= 0.0f) && ((objData->unk15C_31) == 0)) {
            dll_734_func_133C(self, objData);
        } else {
            break;
        }
        return 1;
    case 4:
        if (!(objData->unk10C <= 0.0f)) {
            if (objData->unk15C_30) {
                mainSetBits(BIT_660, 1);
            } else {
                mainGetBits(BIT_661);
                if (mainGetBits(BIT_661) == 0) {
                    mainSetBits(BIT_788, 1);
                    objData->unk15C_28 = 1;
                    objData->unk118 = 0.0f;
                } else {
                    if (objData->unk118 < 0.0f) {
                        temp = -2.0f;
                    } else {
                        temp = 2.0f;
                    }
                    objData->unk110 += temp;
                }
            }
        }
        break;
    case 9:
        if (!(objData->unk10C >= 0.0f)) {
            if (mainGetBits(BIT_661) == 0) {
                objData->unk118 = 0.0f;
                objData->unk15C_28 = 1;
            } else {
                if (objData->unk118 < 0.0f) {
                    temp = -2.0f;
                } else {
                    temp = 2.0f;
                }
                objData->unk110 += temp;
            }
        }
        break;
    case 5:
        if (!(objData->unk15C_30)) {
            objData->unk15C_28 = 2;
        }
        break;
    case 6:
        if (!(objData->unk15C_30)) {
            if (objData->unk118 < 0.0f) {
                temp = -3.0f;
            } else {
                temp = 3.0f;
            }
            objData->unk110 += temp;
        }
        break;
    case 7:
        if (objData->unk118 <= 0.0f) {
            objData->unk118 = 0.0f;
            objData->unk15C_28 = 3;
        }
        break;        
    case 10:
        if ((objData->unk15C_22) && (mainGetBits(BIT_689) == 0)) {
            mainSetBits(BIT_689, 1);
        }
        break;
    case 11:
        if ((objData->unk15C_22) && (self == player->parent)) {
            mainSetBits(BIT_68A, 1);
        }
        break;
    case 12:
        if ((objData->unk15C_22) && (self == player->parent)) {
            mainSetBits(BIT_68B, 1);
        }
        break;
    case 13:
        if ((mainGetBits(BIT_68A) != 0) && (objData->unk118 >= 0.0f)) {
            dll_734_func_133C(self, objData);
        }
        break;
    case 14:
        if ((objData->unk15C_22) && (objData->unk118 <= 0.0f)) {
            dll_734_func_133C(self, objData);
        }
        break;
    case 15:
        if (!(objData->unk15C_30)) {
            mainSetBits(BIT_788, 1);
        }
        break;
    case 16:
        if (objData->unk118 >= 0) {
            temp = objData->unk118;
        } else {
            temp = -objData->unk118;
        }
        
        if (temp == 2.0f) {
            objData->unk118 *= 0.5f;
        } else {
            objData->unk118 *= 2.0f;
        }
        break;
    }

    if (arg2 != 2) {
        if (arg2 == 8) {
            if (mainGetBits(BIT_67F)) {
                *arg3 = 0;
            } else {
                *arg3 = 1;
            }
        }
    } else {
        mainSetBits(BIT_DR_Minecart_Track_Entrance_Demolished, 1);
    }
    
    return 1;
}

// offset: 0x133C | func: 10
void dll_734_func_133C(Object* self, DRPushCart_Data* objData) {
    Object* player;
    f32 camShake;

    player = objGetPlayer();
    objData->unk10C = -objData->unk10C * 0.8f;
    objData->unk118 = 0.0f;
    
    if (self == player->parent) {
        camUseShake();
        if (objData->unk10C >= 0.0f) {
            camShake = objData->unk10C;
        } else {
            camShake = -objData->unk10C;
        }
        camSetShakeOffset(camShake);
    }
    
    gDLL_6_AMSFX->vtbl->play(self, 0x856, MAX_VOLUME, NULL, NULL, 0, NULL);
}

// offset: 0x1434 | func: 11
s32 dll_734_func_1434(Object* self, u8* totalBarrelsAboard) {
    s32 count;
    Object** barrels;
    s32 barrelCount;
    s32 i;

    barrelCount = 0;
    barrels = obj_get_all_of_type(OBJTYPE_Barrel, &count);
    
    if (count == *totalBarrelsAboard) {
        return *totalBarrelsAboard;
    }

    for (i = 0; i < count; i++) {
        if (self == barrels[i]->parent) {
            barrelCount++;
        }
    }   

    barrelCount = (barrelCount < 0) ? 0 : MIN(6, barrelCount);
    *totalBarrelsAboard = barrelCount;
    return barrelCount;
}

// offset: 0x1588 | func: 12
void dll_734_func_1588(Object* self, s32 barrelCount) {
    Object* obj;
    s32 i;

    for (i = 0; i < barrelCount; i++) {
        obj = dll_734_func_1624(self, &data_0[i]);
        obj->srt.yaw = self->srt.yaw;
        obj->srt.pitch = self->srt.pitch;
        obj->srt.roll = self->srt.roll;
    }
}

// offset: 0x1624 | func: 13
Object* dll_734_func_1624(Object* self, Vec3f* coord) {
    CFBarrel_Setup* barrelSetup;
    DRPushCart_Setup* objSetup;

    objSetup = (DRPushCart_Setup*)self->setup;
    barrelSetup = (CFBarrel_Setup*)objAllocSetup(sizeof(CFBarrel_Setup), OBJ_CFbarrel);
    barrelSetup->base.loadFlags = objSetup->base.loadFlags;
    barrelSetup->base.byte6 = objSetup->base.byte6;
    barrelSetup->base.byte5 = objSetup->base.byte5;
    barrelSetup->base.fadeDistance = objSetup->base.fadeDistance;
    barrelSetup->base.x = coord->x;
    barrelSetup->base.y = coord->y;
    barrelSetup->base.z = coord->z;
    barrelSetup->unk18 = self->srt.yaw;
    barrelSetup->unk19 = 1;
    return objSetupObject((ObjSetup*)barrelSetup, 5, self->mapID, -1, self);
}

// offset: 0x16EC | func: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/734_DR_PushCart/dll_734_func_16EC.s")

// offset: 0x1A10 | func: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/734_DR_PushCart/dll_734_func_1A10.s")

// offset: 0x1FD8 | func: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/734_DR_PushCart/dll_734_func_1FD8.s")

// offset: 0x20BC | func: 17
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/734_DR_PushCart/dll_734_func_20BC.s")
#else
s32 dll_734_func_20BC-8(CurveSetup* curveSetup, s32 arg1, s32 arg2) {
    s32 curveLinks[4];
    s32 linkIdx;
    s32 i;
    s32 bit;
    
    for (i = 0, linkIdx = 0, bit = 1; i < 4; i++, bit <<= 1) {
        if (curveSetup->links[i] >= 0 && (curveSetup->unk1B & bit) && arg1 != curveSetup->links[i]) {
            curveLinks[linkIdx] = curveSetup->links[i];
            linkIdx++;
        }
    }
    
    if (linkIdx != 0) {
        arg2 = MIN(linkIdx - 1, arg2);
        if (arg2 == -1) {
            arg2 = rand_next(0, linkIdx - 1);
        }
        linkIdx = curveLinks[arg2];
        return linkIdx;
    } else {
        return -1;
    }
}
#endif

// offset: 0x21A0 | func: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/734_DR_PushCart/dll_734_func_21A0.s")

// offset: 0x25A4 | func: 19
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/734_DR_PushCart/dll_734_func_25A4.s")
#else

static void dll_734_func_21A0(DRPushCart_Data* self, Object* objData, s32 uID);

//Matches, but needs dll_734_func_21A0
void dll_734_func_25A4(Object* self, DRPushCart_Data* objData) {
    s32 sp34;

    sp34 = 1;
    if (mainGetBits(0x788) != 0) {
        dll_734_func_21A0(objData, self, 0x34BD4);
        objData->unk15C_28 = 1;
    } else if (!objData->unk15C_29) {
        dll_734_func_21A0(objData, self, 0x33988);
    } else {
        gDLL_26_Curves->vtbl->func_4288(&objData->unk0, self, 300.0f, &sp34, -1);
    }
    curves_func_800053B0(&objData->unk0, 0.01f);
    self->srt.transl.x = objData->unk0.unk68.f[0];
    self->srt.transl.y = objData->unk0.unk68.f[1];
    self->srt.transl.z = objData->unk0.unk68.f[2];
    self->srt.transl.y += 3.0f;
}
#endif

/*0x30*/ static const char str_30[] = "speed %f ";
/*0x3C*/ static const char str_3C[] = "\n Brake Message %i \n\n";
/*0x54*/ static const char str_54[] = " Mine Cart At Doors ";
/*0x6C*/ static const char str_6C[] = " COLLISION";
/*0x78*/ static const char str_78[] = "Creating  BarrelCnt Is %i ";
/*0x94*/ static const char str_94[] = " TRACK DOORS OPENDED ";
/*0xAC*/ static const char str_AC[] = "SETTING POSITION TO NODE IDNY \n\n";
/*0xD0*/ static const char str_D0[] = " list Branch No %i ";
/*0xE4*/ static const char str_E4[] = " list Branch %i Max %i Branch No %i ";
