#include "common.h"
#include "sys/gfx/model.h"
#include "sys/objanim.h"
#include "sys/objtype.h"

typedef s32 (*PointBackBSSUnk0_UnkC)(void*, s32, s32);

typedef struct {
    s32 unk0; //flags?
    s16 unk4; //gamebitID
    s16 unk6;
    s32 unk8;
    PointBackBSSUnk0_UnkC unkC; //function
} PointBackBSSUnk0;

typedef struct {
    u8 unk0[0x9D - 0x0];
    u8 unk9D;
} PointBack_funcACC_UnkArg1;

typedef struct {
    s8 _unk0[0x8E - 0x0];
    u8 unk8E;
} PointBack_func1308_arg1; //might be AnimObj_Data?

typedef struct {
    s8 _unk0[0x2C - 0x0];
    s8 unk2C;
    s8 unk2D;
} DLL658_func19FC_arg1;

typedef struct {
    Vec3f unk0;
    Vec3f unkC;
    Vec3f unk18;
    Vec3f unk24;
    f32 unk30;
    f32 unk34;
} DLL658_func19FC_arg2;

typedef struct {
    s8 _unk0[0x13 - 0x0];
    u8 unk13[0x1B - 0x13];
    s8 unk1B;
} PointBack_func2178_arg1;

typedef struct {
    ObjSetup base;
    s16 unk18;
} PointBack_Setup;

typedef struct {
    s8 _unk0[0x4B8 - 0x0];
    HeadAnimation unk4B8;
    s8 _unk4DC[0x51C - 0x4DC];
    s32 unk51C; //bitfield for creating 4 effects in print function?
    s32 _unk520;
    u8 unk524;
    s8 _unk525[0x688 - 0x525];
    DLL27_Data unk688;
    s8 _unk8E8[0x920 - 0x8E8];
} PointBack_Data;

static f32 dll_658_func_1C24(DLL658_func19FC_arg2* arg0, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3, s32 arg4);

/*0x0*/ static u32 _data_0[] = {
    0x00000010, 0x3b449ba6, 0x00000011, 0x3b83126f
};

typedef struct {
    s16 soundID;
    s16 unk2;
} PointBack_Sound;

typedef struct {
    PointBack_Sound unk0[3];
    f32 unkC[18];
} PointBack_Data10;

/*0x10*/ static PointBack_Data10 _data_10 = {
    SOUND_A4E_Dinosaur_Grunt, 0x1000, 
    SOUND_A4F_Dinosaur_Grunt, 0x1000, 
    SOUND_A50_Dinosaur_Grunt, 0x1000, 
    0, 
    0, 
    -30, 
    0, 
    0, 
    0, 
    30, 
    0, 
    0, 
    0, 
    -30, 
    0, 
    0, 
    0, 
    30, 
    0, 
    7.5, 
    7.5
};

/*0x64*/ static u32 _data_64[] = {
    0x00000000, 0x00000000, 0xc1f00000, 0x00000000, 0x00000000, 0x41f00000
};
/*0x7C*/ static u32 _data_7C[] = {
    0x41000000, 0x41000000
};
/*0x84*/ static u32 _data_84[] = {
    0x0000001b, 0x039b0003, 0xffff0000, 0x00000000, 0x00000080, 0x086d0004, 0x00070000, 0x00000000, 
    0x000000a0, 0x039c0009, 0x00000000, 0x00000000, 0x00000000, 0xffff0009, 0xffff0000, 0x00000000
};
/*0xC4*/ static u32 _data_C4[] = {
    0x00000040, 0x047effff, 0xffff0000, 0x00000000, 0x00000000, 0x041fffff, 0xffff0000, 0x00000000, 
    0x00000004, 0xffffffff, 0x00090000, 0x00000000, 0x0000000a, 0xffff000a, 0x000b0000, 0x00000000, 
    0x00000004, 0xffff000c, 0xffff0000, 0x00000000
};
/*0x114*/ static u32 _data_114[] = {
    0x000000c0, 0x03ffffff, 0xffff0000, 0x00000000, 0x000000a0, 0x039effff, 0x00020000, 0x00000000, 
    0x00000080, 0xffff0002, 0xffff0000, 0x00000000, 0x00000000, 0xffffffff, 0xffff0000, 0x00000000
};
/*0x154*/ static u32 _data_154[] = {
    0x000f000f, 0x00000000, 0x00000000
};

/*0x0*/ static PointBackBSSUnk0* _bss_0;

// offset: 0x0 | ctor
void dll_658_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_658_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_658_setup(Object *self, ObjSetup *setup, s32 arg2);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/658_DBPointMum/dll_658_setup.s")

// offset: 0x358 | func: 1 | export: 1
void dll_658_control(Object *self);

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/658_DBPointMum/dll_658_control.s")
#else
static void dll_658_func_720(Object* self, s32 arg1);

void dll_658_control(Object* self) {
    PointBack_Data* objData;

    objData = self->data;
    dll_658_func_720(self, 0);
    
    gDLL_27->vtbl->func_1E8(self, &objData->unk688, gUpdateRateF);
    gDLL_27->vtbl->func_5A8(self, &objData->unk688);
    gDLL_27->vtbl->func_624(self, &objData->unk688, gUpdateRateF);
}
#endif

// offset: 0x430 | func: 2 | export: 2
void dll_658_update(Object *self) { }

// offset: 0x43C | func: 3 | export: 3
void dll_658_print(Object* self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    PointBack_Data* objData;
    SRT fxTransform;
    s32 i;
    s32 fxBitfield;

    objData = self->data;
    fxTransform.scale = 0.005f;
    if (!visibility) {
        return;
    }
    
    draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    //TO-DO: figure out DLL interface
    ((DLL_Unknown*)gTempDLLInsts[1])->vtbl->func[3].withThreeArgs((s32)self, (s32)objData, 0);

    fxBitfield = objData->unk51C;
    if (!fxBitfield) {
        return;
    }
    
    for (i = 0; i < 4; i++){
        if ((1 << i) & fxBitfield) {
            func_80031F6C(self, i + 2, &fxTransform.transl.x, &fxTransform.transl.y, &fxTransform.transl.z, 0);
            gDLL_17_partfx->vtbl->spawn(self, 0x676, &fxTransform, 0x210001, -1, 0);
        }
    }
}

// offset: 0x5B4 | func: 4 | export: 4
void dll_658_free(Object *self, s32 arg1) {
    remove_temp_dll(0x35);
}

// offset: 0x5F8 | func: 5 | export: 5
u32 dll_658_get_model_flags(Object *self){
    return MODFLAGS_80 | MODFLAGS_EVENTS | MODFLAGS_8 | MODFLAGS_1;
}

// offset: 0x608 | func: 6 | export: 6
u32 dll_658_get_data_size(Object *self, u32 a1) {
    return sizeof(PointBack_Data);
}

// offset: 0x61C | func: 7
s32 dll_658_func_61C(Object* self, s32 arg1, s32 arg2, s32 arg3) {
    PointBackBSSUnk0* temp_v1;
    PointBack_Data* objData;

    objData = self->data;
    if (((DLL_Unknown*)gTempDLLInsts[1])->vtbl->func[4].withFiveArgsS32((s32)self, arg2, (s32)objData, 1, 1)) {
        return 1;
    }
    
    temp_v1 = &_bss_0[objData->unk524];
    if (temp_v1->unk0 & 0x80) {
        return 0;
    }
    
    if (temp_v1->unk4 != NO_GAMEBIT) {
        if (main_get_bits(temp_v1->unk4)) {
            return 4;
        }
    }
    
    if (temp_v1->unkC != NULL) {
        return temp_v1->unkC(self, arg2, 0);
    }

    return 0;
}

// offset: 0x720 | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/658_DBPointMum/dll_658_func_720.s")

// offset: 0xACC | func: 9
s32 dll_658_func_ACC(Object* self, PointBack_funcACC_UnkArg1* arg1, s32 arg2) {
    Object* objects[3];
    f32 distance;
    s32 result;
    s32 i;
    
    distance = 300.0f;
    if (arg1 != NULL) {
        objects[0] = get_player();
        objects[1] = get_sidekick();
        objects[2] = obj_get_nearest_type_to(4, self, &distance);

        result = 0;
        for (i = 0; i < 3; i++) {
            if (objects[i] != NULL) {
                result |= vec3_distance(&self->positionMirror, &objects[i]->positionMirror) < 200.0f;
            }
        }
        
        if (result != 0) {
            arg1->unk9D |= 4;
        } else {
            arg1->unk9D |= 8;
        }
    }
    return 0;
}

// offset: 0xC08 | func: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/658_DBPointMum/dll_658_func_C08.s")

// offset: 0xF30 | func: 11
s32 dll_658_func_F30(Object* self, s32 arg1, s32 arg2) {
    PointBack_Data* objData;

    if (arg1 == 0) {
        if (self->curModAnimId != 0xD) {
            func_80023D30(self, 0xD, 0, 0);
        }
        
        func_80024108(self, 0.004f, gUpdateRate, 0);
        if (main_get_bits(BIT_424) && main_get_bits(BIT_425)) {
            objData = self->data;
            objData->unk524++;
            return 1;
        }
        
        main_get_bits(BIT_428); //@bug: called without being stored/used
        if (main_get_bits(BIT_428) >= 4) {
            return 1;
        }
    }

    return 0;
}

// offset: 0x1064 | func: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/658_DBPointMum/dll_658_func_1064.s")

// offset: 0x1308 | func: 13
s32 dll_658_func_1308(Object* self, PointBack_func1308_arg1* arg1, s32 arg2) {
    PointBack_Setup* objSetup;
    PointBack_Data* objData;

    if (arg1 != NULL) {
        if (arg1->unk8E == 1) {
            joy_get_pressed(0); //@bug: called without being used/stored
            
            if (joy_get_pressed(0) & B_BUTTON) {
                main_set_bits(BIT_42A, 1);
            } else {
                return 1;
            }
        }
        arg1->unk8E = 0;
        
    } else if (main_get_bits(BIT_42A)) {
        main_set_bits(BIT_41F, 0);
        main_set_bits(BIT_425, 0);
        main_set_bits(BIT_428, 0);
        main_set_bits(BIT_420, 0);
        main_set_bits(BIT_422, 0);
        main_set_bits(BIT_423, 0);
        main_set_bits(BIT_427, 0);
        main_set_bits(BIT_42A, 0);
        main_set_bits(BIT_426, 0);
        
        _bss_0[0].unk6 = 13;
        _bss_0[1].unk6 = -1;
        _bss_0[2].unk6 = 4;
        _bss_0[3].unk6 = 10;
        
        gDLL_28_ScreenFade->vtbl->fade_reversed(120, 1);
        
        objSetup = (PointBack_Setup*)self->setup;
        self->srt.transl.x = objSetup->base.x;
        self->srt.transl.y = objSetup->base.y;
        self->srt.transl.z = objSetup->base.z;
        
        objData = self->data;
        objData->unk524 = 0xFF;
        return 1;
    }

    return 0;
}

// offset: 0x153C | func: 14
CurveSetup* dll_658_func_153C(Object* self, s32 arg1, Vec3f* arg2, s32 arg3) {
    s32 curveUID;
    s32 sp2C[2];
    CurveSetup* curveSetup;

    curveSetup = NULL;
    
    if (arg3 == 1) {
        sp2C[0] = 0;
        sp2C[1] = 0;
    } else {
        sp2C[0] = 25;
        sp2C[1] = 21;
    }
    
    curveUID = gDLL_26_Curves->vtbl->func_1E4(self->srt.transl.x, self->srt.transl.y, self->srt.transl.z, sp2C, 2, arg1);
    if (curveUID >= 0) {
        curveSetup = gDLL_26_Curves->vtbl->func_39C(curveUID);
        arg2->x = curveSetup->pos.x;
        arg2->y = curveSetup->pos.y;
        arg2->z = curveSetup->pos.z;
    }
    return curveSetup;
}

// offset: 0x1618 | func: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/658_DBPointMum/dll_658_func_1618.s")

// offset: 0x18B8 | func: 16
void dll_658_func_18B8(Object* self, UnkCurvesStruct* arg1, s32 arg2, s32 arg3, f32 arg4) {
    s32 sp28[2];

    if (arg2 == 1) {
        sp28[0] = 0;
        sp28[1] = 0;
    } else {
        sp28[0] = 25;
        sp28[1] = 21;
    }
    
    gDLL_26_Curves->vtbl->func_4288(arg1, self, arg4, (s32*)&sp28, arg3);
}

// offset: 0x1944 | func: 17
s32 dll_658_func_1944(Object* self, UnkCurvesStruct* arg1, f32 arg2) {
    s32 var_v1;

    var_v1 = 0;
    if (func_800053B0(arg1, arg2) || arg1->unk10) {
        var_v1 = gDLL_26_Curves->vtbl->func_4704(arg1);
    }
    
    self->srt.transl.x = arg1->unk68.x;
    self->srt.transl.y = arg1->unk68.y;
    self->srt.transl.z = arg1->unk68.z;
    return var_v1;
}

// offset: 0x19FC | func: 18
s32 dll_658_func_19FC(Object* self, DLL658_func19FC_arg1* arg1, DLL658_func19FC_arg2* arg2, f32* arg3, f32 arg4) {
    SRT transform;
    s32 returnVal;

    returnVal = 0;
    if (arg1 != NULL) {
        arg2->unk18.y = 0.0f;
        arg2->unk18.z = 0.0f;
        arg2->unk24.y = 0.0f;
        arg2->unk24.z = 0.0f;
        arg2->unk18.x = -150.0f;
        arg2->unk24.x = -150.0f;
        rotate_vec_inv(&self->srt, (Vec3f*)&arg2->unk18);
        
        transform.roll = 0;
        transform.pitch = arg1->unk2D;
        transform.yaw = arg1->unk2C;
        rotate_vec_inv(&transform, (Vec3f*)&arg2->unk24);
        
        *arg3 = 0.0f;
        arg2->unk34 = dll_658_func_1C24(arg2, &arg2->unk18, &arg2->unkC, &arg2->unk24, 0xA);
    } else {
        *arg3 += (arg4 * gUpdateRate) / arg2->unk34;
        if (*arg3 >= 1.0f) {
            returnVal = 1;
            *arg3 = 1.0f;
        }
    }
    
    transform.scale = arg2->unk0.x;
    transform.transl.x = arg2->unkC.x;
    transform.transl.y = arg2->unk18.x;
    transform.transl.z = arg2->unk24.x;
    self->srt.transl.x = func_80004C5C((Vec4f*)&transform.scale, *arg3, 0);
    
    transform.scale = arg2->unk0.y;
    transform.transl.x = arg2->unkC.y;
    transform.transl.y = arg2->unk18.y;
    transform.transl.z = arg2->unk24.y;
    self->srt.transl.y = func_80004C5C((Vec4f*)&transform.scale, *arg3, 0);
    
    transform.scale = arg2->unk0.z;
    transform.transl.x = arg2->unkC.z;
    transform.transl.y = arg2->unk18.z;
    transform.transl.z = arg2->unk24.z;
    self->srt.transl.z = func_80004C5C((Vec4f*)&transform.scale, *arg3, 0);
    
    return returnVal;
}

// offset: 0x1C24 | func: 19
f32 dll_658_func_1C24(DLL658_func19FC_arg2* arg0, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3, s32 arg4) {
    f32 dx;
    f32 dy;
    f32 dz;
    f32 baseX;
    f32 baseY;
    f32 baseZ;
    f32 resultX;
    f32 resultY;
    f32 resultZ;
    f32 sum;
    s32 steps;
    Vec4f sp7C;
    f32 tValue;
    s32 i;

    steps = arg4 + 1;
    baseX = arg0->unk0.x;
    baseY = arg0->unk0.y;
    baseZ = arg0->unk0.z;
    
    i = 1;
    sum = 0.0f;
    if (steps > 1) {
        do {
            tValue = (f32) i / arg4;
            
            sp7C.x = arg0->unk0.x;
            sp7C.y = arg2->x;
            sp7C.z = arg1->x;
            sp7C.w = arg3->x;
            resultX = func_80004C5C(&sp7C, tValue, 0);
            dx = resultX - baseX;
            
            sp7C.x = arg0->unk0.y;
            sp7C.y = arg2->y;
            sp7C.z = arg1->y;
            sp7C.w = arg3->y;
            resultY = func_80004C5C(&sp7C, tValue, 0);
            dy = resultY - baseY;
            
            sp7C.x = arg0->unk0.z;
            sp7C.y = arg2->z;
            sp7C.z = arg1->z;
            sp7C.w = arg3->z;
            resultZ = func_80004C5C(&sp7C, tValue, 0);
            dz = resultZ - baseZ;
            
            sum += sqrtf(SQ(dx) + SQ(dy) + SQ(dz));
            
            baseX = resultX;
            baseY = resultY;
            baseZ = resultZ;
            
            i++;
        } while (i != steps);
    }
    
    return sum;
}

// offset: 0x1E14 | func: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/658_DBPointMum/dll_658_func_1E14.s")

// offset: 0x2178 | func: 21
s32 dll_658_func_2178(Object* self, PointBack_func2178_arg1* arg1) {
    PointBack_Data* objData;
    u8 outValue;
    s32 i;

    objData = self->data;

    for (outValue = 0, i = 0; i < arg1->unk1B; i++){
        switch (arg1->unk13[i]) {
        case 0:
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_A50_Dinosaur_Grunt, MAX_VOLUME, 0, 0, 0, 0);
            continue;
        case 7:
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_AC8_Ground_Impact, MAX_VOLUME, 0, 0, 0, 0);
            continue;
        case 9:
            gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_AC9_Impact_Crumbling, MAX_VOLUME, 0, 0, 0, 0);
            continue;
        case 10:
            func_80034B54(self, 
                &objData->unk4B8, 
                &_data_10.unk0[rand_next(0, 2)].soundID, 
                0);
            continue;
        case 1:
            outValue |= 1;
            continue;
        case 2:
            outValue |= 2;
            continue;
        case 3:
            outValue |= 4;
            continue;
        case 4:
            outValue |= 8;
            continue;
        }
    }
    
    if (outValue) {
        gDLL_6_AMSFX->vtbl->play_sound(self, SOUND_A73, MAX_VOLUME, 0, 0, 0, 0);
    }
    
    return outValue;
}

// offset: 0x23CC | func: 22
void dll_658_func_23CC(Object* self, s16 seqBoneID, s16 arg2, s16 arg3) {
    s16 *seqBone;
    
    seqBone = func_80034804(self, seqBoneID);
    
    if (!seqBone){
        return;
    }
    
    seqBone[1] += (arg2 - seqBone[1]) >> 1;
    seqBone[0] += (arg3 - seqBone[0]) >> 1;

    //@bug? Should one of these lines be affecting seqBone[0] instead? 
    seqBone[1] = (seqBone[1] < -0x1555) ? -0x1555 : ((seqBone[1] > 0x1555) ? 0x1555 : seqBone[1]);
    seqBone[1] = (seqBone[1] < -0x1555) ? -0x1555 : ((seqBone[1] > 0x1555) ? 0x1555 : seqBone[1]);

    if (arg2 ){ }
}

/*0x0*/ static const char str_0[] = " ACt Number %i ";
/*0x10*/ static const char str_10[] = " Running Mummy ";
/*0x20*/ static const char str_20[] = " Set OPERATION BIT %i  ";
/*0x38*/ static const char str_38[] = " Menu operation Activated  %i ";
/*0x58*/ static const char str_58[] = " Bit Set ";
/*0x64*/ static const char str_64[] = " No Bits Drowned %i \n";
/*0x7C*/ static const char str_7C[] = " Linking TO Object ";
/*0x90*/ static const char str_90[] = " Build Nest ";
/*0xA0*/ static const char str_A0[] = " t ";
/*0xA4*/ static const char str_A4[] = " KeyPressy ";
/*0xB0*/ static const char str_B0[] = " A Button ";
/*0xBC*/ static const char str_BC[] = " B button ";
/*0xC8*/ static const char str_C8[] = " Trying Again ";
/*0xD8*/ static const char str_D8[] = " Error Could not find node ";
/*0xF4*/ static const char str_F4[] = "Got Curve ";
/*0x100*/ static const char str_100[] = " Could Not Find Curve ";
/*0x118*/ static const char str_118[] = " PathId %i Loc id %i \n";
/*0x130*/ static const char str_130[] = " Tangent 1 %f %f %f \n";
/*0x148*/ static const char str_148[] = " Tangent 2 %f %f %f \n";
/*0x160*/ static const char str_160[] = "t value %f ";
/*0x16C*/ static const char str_16C[] = "Turning %i ";
