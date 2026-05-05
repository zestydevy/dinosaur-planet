#include "common.h"
#include "sys/math.h"
#include "sys/objhits.h"
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
    s8 unk0[0x108 - 0];
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
    objData->unk15C_29 = main_get_bits(objSetup->unk20);
    objData->unk15C_29 = 1;
    objData->unk15C_21 = 1;
    
    main_set_bits(BIT_649, 1);
    
    if (arg2 == 0) {
        dll_734_func_1588(self, main_get_bits(BIT_683));
    }
}

/*0x0*/ static const char str_0[] = " HELLO I AM THE PUSHCART";

// offset: 0x114 | func: 1 | export: 1
void dll_734_control(Object *self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/734_DR_PushCart/dll_734_control.s")

// offset: 0x7E0 | func: 2 | export: 2
void dll_734_update(Object* self) {
    if (func_80025F40(self, NULL, NULL, NULL) == Damage_Type_Explosion) {
        main_set_bits(BIT_822, 0);
        main_set_bits(BIT_823, 0);
        main_set_bits(BIT_824, 0);
        main_set_bits(BIT_825, 0);
        main_set_bits(BIT_826, 0);
        main_set_bits(BIT_827, 0);
    }
}

// offset: 0x8B8 | func: 3 | export: 3
void dll_734_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/734_DR_PushCart/dll_734_print.s")

// offset: 0xB8C | func: 4 | export: 4
void dll_734_free(Object* self, s32 arg1) {
    DRPushCart_Data* objData;

    objData = self->data;
    main_set_bits(BIT_683, objData->unk14C);
    
    if (arg1 == 0) {
        if (objData->unk108 != NULL) {
            obj_destroy_object(objData->unk108);
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
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/734_DR_PushCart/dll_734_func_C34.s")

// offset: 0xD30 | func: 8
void dll_734_func_D30(Object* self, s32 arg1) {
    LFXEmitter_Setup* lfxSetup;

    lfxSetup = (LFXEmitter_Setup*)obj_alloc_setup(sizeof(LFXEmitter_Setup), OBJ_LFXEmitter);
    lfxSetup->base.loadFlags = 2;
    lfxSetup->base.x = self->srt.transl.x;
    lfxSetup->base.y = self->srt.transl.y;
    lfxSetup->base.z = self->srt.transl.z;
    lfxSetup->unk1E = arg1;
    lfxSetup->unk22 = -1;
    obj_create((ObjSetup*)lfxSetup, 5, self->mapID, -1, self->parent);
}

// offset: 0xDD8 | func: 9
s32 dll_734_func_DD8(Object* self, u8 arg1, u8 arg2, s32* arg3) {
    DRPushCart_Data* objData;
    Object* player;
    f32 var_fv0;

    objData = self->data;
    player = get_player();
    
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
                main_set_bits(BIT_660, 1);
            } else {
                main_get_bits(BIT_661);
                if (main_get_bits(BIT_661) == 0) {
                    main_set_bits(BIT_788, 1);
                    objData->unk15C_28 = 1;
                    objData->unk118 = 0.0f;
                } else {
                    if (objData->unk118 < 0.0f) {
                        var_fv0 = -2.0f;
                    } else {
                        var_fv0 = 2.0f;
                    }
                    objData->unk110 += var_fv0;
                }
            }
        }
        break;
    case 9:
        if (!(objData->unk10C >= 0.0f)) {
            if (main_get_bits(BIT_661) == 0) {
                objData->unk118 = 0.0f;
                objData->unk15C_28 = 1;
            } else {
                if (objData->unk118 < 0.0f) {
                    var_fv0 = -2.0f;
                } else {
                    var_fv0 = 2.0f;
                }
                objData->unk110 += var_fv0;
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
                var_fv0 = -3.0f;
            } else {
                var_fv0 = 3.0f;
            }
            objData->unk110 += var_fv0;
        }
        break;
    case 7:
        if (objData->unk118 <= 0.0f) {
            objData->unk118 = 0.0f;
            objData->unk15C_28 = 3;
        }
        break;        
    case 10:
        if ((objData->unk15C_22) && (main_get_bits(BIT_689) == 0)) {
            main_set_bits(BIT_689, 1);
        }
        break;
    case 11:
        if ((objData->unk15C_22) && (self == player->parent)) {
            main_set_bits(BIT_68A, 1);
        }
        break;
    case 12:
        if ((objData->unk15C_22) && (self == player->parent)) {
            main_set_bits(BIT_68B, 1);
        }
        break;
    case 13:
        if ((main_get_bits(BIT_68A) != 0) && (objData->unk118 >= 0.0f)) {
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
            main_set_bits(BIT_788, 1);
        }
        break;
    case 16:
        if (objData->unk118 >= 0) {
            var_fv0 = objData->unk118;
        } else {
            var_fv0 = -objData->unk118;
        }
        
        if (var_fv0 == 2.0f) {
            objData->unk118 *= 0.5f;
        } else {
            objData->unk118 *= 2.0f;
        }
        break;
    }

    if (arg2 != 2) {
        if (arg2 == 8) {
            if (main_get_bits(BIT_67F) != 0) {
                *arg3 = 0;
            } else {
                *arg3 = 1;
            }
        }
    } else {
        main_set_bits(BIT_7BA, 1);
    }
    
    return 1;
}

// offset: 0x133C | func: 10
void dll_734_func_133C(Object* self, DRPushCart_Data* objData) {
    Object* player;
    f32 camShake;

    player = get_player();
    objData->unk10C = -objData->unk10C * 0.8f;
    objData->unk118 = 0.0f;
    
    if (self == player->parent) {
        camera_enable_y_offset();
        if (objData->unk10C >= 0.0f) {
            camShake = objData->unk10C;
        } else {
            camShake = -objData->unk10C;
        }
        camera_set_shake_offset(camShake);
    }
    
    gDLL_6_AMSFX->vtbl->play_sound(self, 0x856, MAX_VOLUME, NULL, NULL, 0, NULL);
}

// offset: 0x1434 | func: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/734_DR_PushCart/dll_734_func_1434.s")

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
    barrelSetup = (CFBarrel_Setup*)obj_alloc_setup(sizeof(CFBarrel_Setup), OBJ_CFbarrel);
    barrelSetup->base.loadFlags = objSetup->base.loadFlags;
    barrelSetup->base.byte6 = objSetup->base.byte6;
    barrelSetup->base.byte5 = objSetup->base.byte5;
    barrelSetup->base.fadeDistance = objSetup->base.fadeDistance;
    barrelSetup->base.x = coord->x;
    barrelSetup->base.y = coord->y;
    barrelSetup->base.z = coord->z;
    barrelSetup->unk18 = self->srt.yaw;
    barrelSetup->unk19 = 1;
    return obj_create((ObjSetup*)barrelSetup, 5, self->mapID, -1, self);
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
s32 dll_734_func_20BC-7(CurveSetup* curveSetup, s32 arg1, s32 arg2) {
    u32 curveLinks[4];
    s32 linkIdx;
    s32 i;
    s32 bit;
    
    for (i = 0, linkIdx = 0, bit = 1; i < 4; i++, bit *= 2) {
        if (((curveSetup->links[i] >= 0) && (curveSetup->unk1B & bit)) && (arg1 != curveSetup->links[i])) {
            curveLinks[linkIdx] = curveSetup->links[i];
            linkIdx++;
        }
    }

    bit = linkIdx - 1;
    if (linkIdx != 0) {
        if ((bit < arg2) != 0) {
            arg2 = bit;
        }
        if (arg2 == -1) {
            arg2 = rand_next(0, bit);
        }
        return curveLinks[arg2];
    } else {
        return -1;
    }
}
#endif

// offset: 0x21A0 | func: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/734_DR_PushCart/dll_734_func_21A0.s")

// offset: 0x25A4 | func: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/734_DR_PushCart/dll_734_func_25A4.s")

/*0x30*/ static const f32 rodata_30[] = {
    1.9046139090923178e+31, 1.1816695572882224e+22, 1.0842021724855044e-19, 7.716216751866123e-33, 2.7139198009492688e+20, 240596784.0, 2.667802940764995e+20, 2.0220383751744459e-16, 
    6.162975822039155e-33, 1.7399059176108167e-19, 4.730134741866897e+22, 4.835413438493264e+30, 5.079074069486449e+31, 7.503454996174864e+28, 0.0, 1.6543379606513273e-19, 
    52776228.0, 3456106496.0, 242.39601135253906, 7.397731894505344e+31, 1.3574490093801795e-19, 4.801150969608111e+30, 238.45361328125, 995842.3125, 
    1.2089258196146292e+25, 1.798431401641387e-19, 203.12603759765625, 3478278912.0, 1.7560140968175505e-19, 823218432.0, 1.0842021724855044e-19, 847523348480.0, 
    844914.0, 13913367552.0, 3458475753472.0, 1.798327874817847e-19, 869339456.0, 1.7047962365110034e-19, 2815439646228480.0, 0.0, 
    2.0024867774945033e-19, 5.0788279321895755e+31, 2.7484300404688788e+20, 1.748564689166657e-19, 2.0220370516854658e-16, 2.0024867774945033e-19, 5.0788279321895755e+31, 2.7484300404688788e+20, 
    1.401082657445381e-19, 236421632.0, 2.0220457867127343e-16, 4.4651249001114914e+30, 3.028101926759323e+24, 1.401082657445381e-19, 0.0
};
