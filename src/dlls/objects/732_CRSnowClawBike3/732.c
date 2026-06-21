#include "common.h"
#include "game/gamebits.h"
#include "sys/curves.h"
#include "sys/gfx/model.h"
#include "sys/objtype.h"
#include "dlls/objects/267_checkpoint4.h"

typedef struct {
    s8 unk0[0xF];
    s8 unkF;
    s8 unk10;
} DLL732_Unk_2E0;

typedef struct {
    s8 unk0[0xE];
    s8 unkE;
    s8 unkF;
} DLL732_Unk_3618; //Maybe the same as Unk_2E0? Controller/joystick-related

typedef struct {
    SRT unk0;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
} DLL732_Data2AC;

typedef struct {
    ObjSetup base;
    u8 unk18;
    u8 unk19;
    s16 unk1A;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    u8 unk20;
} DLL732_Setup;

typedef struct {
    SRT unk0;
    RaceStruct unk18;
    s8 _unk3A[0x48 - 0x3A];
    u8 unk48;
    u8 unk49;
    DLL27_Data unk4C;
    DLL732_Data2AC unk2AC;
    DLL732_Unk_2E0 unk2E0;
    void* unk2F4; //DLL
    void* unk2F8; //DLL
    s16* unk2FC; //soundIDs?
    s8 _unk300[0x330 - 0x300];
    Vec3f unk330[6];
    s8 _unk378[0x384 - 0x378];
    f32 unk384;
    Vec3f unk388;
    s8 _unk394[0x3A0 - 0x394];
    Vec3f unk3A0; //previous position?
    Vec3f unk3AC;
    u32 unk3B8; //soundHandle
    u32 unk3BC; //soundHandle
    u32 unk3C0; //soundHandle
    u32 unk3C4; //soundHandle
    s32 unk3C8; //total damage taken?
    f32 unk3CC;
    f32 unk3D0;
    f32 unk3D4;
    s8 _unk3D8[0x3DC - 0x3D8];
    s16 unk3DC;
    s16 unk3DE;
    s16 unk3E0;
    s8 _unk3E2[0x3EC - 0x3E2];
    u8 unk3EC;
    u8 unk3ED;
    u8 unk3EE;
    u8 unk3EF; //flags
    s8 unk3F0;
    u8 unk3F1;
    u8 _unk3F2;
    u8 _unk3F3;
    u8 unk3F4_0 : 1; //flags
} DLL732_Data; //0x3F8

/*0x0*/ static s16 data_0[][3] = {
    { 0x0497, 0x0498, 0x049c },
    { 0x0479, 0x077e, 0x077f }
};
/*0xC*/ static u32 data_C[][3] = {
    {0x00030c60, 0x00030c60, 0x00030c60}, 
    {0x00034dd3, 0x00034dc7, 0x00034dc9}
};
/*0x24*/ static Vec3f data_24[] = {
    VEC3F(-6.5, 0, -7),
    VEC3F(6.5, 0, -7),
    VEC3F(6.5, 0, 7),
    VEC3F(-6.5, 0, 7)
};
/*0x54*/ static f32 data_54[] = {
    0.0f, 0.0f, 0.0f, 0.0f
};
/*0x64*/ static Vec3f data_64[] = {
    VEC3F(0, 1, -18), 
    VEC3F(-6.5, 1, -7), 
    VEC3F(6.5, 1, -7), 
    VEC3F(-6, 1, 11), 
    VEC3F(6, 1, 11)
};
/*0xA0*/ static f32 data_A0[] = {
    5, 4, 4, 4.5, 4.5, -5, 0, 0, 
    5, 0, 0, 0, 2.0001223, 0, 0, 0, 
    2.0157478, 0, 0, 0
};
/*0xF0*/ static f32 data_F0[] = {
    14.5, 0, 9, -14.5, 0, 9
};
/*0x108*/ static u8 data_108[] = {0x05, 0x05, 0x05, 0x05};
/*0x10C*/ static u32 data_10C[] = {
    0x00000006, 0x00000069, 0x00000069, 0x000000ff, 0x00000000
};

/*0x0*/ static Texture* bss_0;
/*0x4*/ static Texture* bss_4;
/*0x8*/ static Texture* bss_8;
/*0x10*/ static f32 bss_10[6];
/*0x28*/ static u8 bss_28[0x4];
/*0x2C*/ static u8 bss_2C[0x4];

static void dll_732_func_3FE0(Object* self, DLL732_Data* objData);

// offset: 0x0 | func: 0
static s32 dll_732_func_0(Object* self, DLL732_Data* objData, f32 arg2) {
    s32 pad;
    s32 sp30;
    DLL732_Data* objData2;

    objData2 = self->data;
    if (gDLL_4_Race->vtbl->func8(objData->unk18.unk10, &sp30)->unk20[1] == -1) {
        objData2->unk3F4_0 = rand_next(0, 1);
    }
    return gDLL_4_Race->vtbl->func5(&objData->unk0, &objData->unk18, arg2, 1, 0, objData2->unk3F4_0);
}

// offset: 0xE4 | ctor
void dll_732_ctor(void* dll) { }

// offset: 0xF0 | dtor
void dll_732_dtor(void* dll) { }

// offset: 0xFC | func: 1 | export: 0
void dll_732_setup(Object* self, ObjSetup* setup, s32 reset);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/732_CRSnowClawBike3/dll_732_setup.s")

/*0x0*/ static const char str_0[] = " FInished Is SEt for Some Reason \n";
/*0x24*/ static const char str_24[] = " FInished Is SEt for Some Reason \n";

// offset: 0x398 | func: 2 | export: 1
void dll_732_control(Object* self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/732_CRSnowClawBike3/dll_732_control.s")

// offset: 0x123C | func: 3 | export: 2
void dll_732_update(Object* self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/732_CRSnowClawBike3/dll_732_update.s")

// offset: 0x1534 | func: 4 | export: 3
void dll_732_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/732_CRSnowClawBike3/dll_732_print.s")

// offset: 0x1724 | func: 5 | export: 4
void dll_732_free(Object* self, s32 onlySelf) {
    DLL732_Data* objData;

    objData = self->data;

    if (objData->unk3B8 != 0) {
        gDLL_6_AMSFX->vtbl->stop(objData->unk3B8);
        objData->unk3B8 = 0;
    }

    if (objData->unk3BC != 0) {
        gDLL_6_AMSFX->vtbl->stop(objData->unk3BC);
        objData->unk3BC = 0;
    }

    if (objData->unk3C0 != 0) {
        gDLL_6_AMSFX->vtbl->stop(objData->unk3C0);
        objData->unk3C0 = 0;
    }

    if (objData->unk3C4 != 0) {
        gDLL_6_AMSFX->vtbl->stop(objData->unk3C4);
        objData->unk3C4 = 0;
    }

    if (objData->unk2F4 != NULL) {
        dll_unload(objData->unk2F4);
    }
    objData->unk2F4 = NULL;
    
    if (objData->unk2F8 != NULL) {
        dll_unload(objData->unk2F8);
    }
    objData->unk2F8 = NULL;
    
    obj_free_object_type(self, OBJTYPE_Vehicle);
    
    if (!(objData->unk3EF & 0x20)) {
        gDLL_1_cmdmenu->vtbl->energy_bar_free();
    }
    
    tex_free(bss_0);
    tex_free(bss_4);
    tex_free(bss_8);
}

// offset: 0x18D8 | func: 6 | export: 5
u32 dll_732_get_model_flags(Object* self) {
    return MODFLAGS_1 | MODFLAGS_SHADOW;
}

// offset: 0x18E8 | func: 7 | export: 6
u32 dll_732_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DLL732_Data);
}

// offset: 0x18FC | func: 8 | export: 7
u8 dll_732_func_18FC(Object* self, s32 arg1) {
    DLL732_Data* objData;
    DLL732_Setup* objSetup;

    objData = self->data;
    objSetup = (DLL732_Setup*)self->setup;
    
    if (objData->unk3EF & 0x20) {
        return 0;
    }

    if (objSetup->unk1A != NO_GAMEBIT) {
        if (main_get_bits(objSetup->unk1A) == FALSE) {
            return 0;
        }
    }
    
    return objData->unk3ED;
}

// offset: 0x1980 | func: 9 | export: 8
u8 dll_732_func_1980(Object* self) {
    DLL732_Data* objData = self->data;
    return objData->unk3ED;
}

// offset: 0x1990 | func: 10 | export: 9
void dll_732_func_1990(Object* self, f32* ox, f32* oy, f32* oz) {
    DLL732_Data* objData = self->data;
    
    *ox = objData->unk388.x;
    *oy = objData->unk388.y;
    *oz = objData->unk388.z;
}

// offset: 0x19B4 | func: 11 | export: 10
s32 dll_732_func_19B4(Object* self, s32 arg1) {
    DLL732_Data* objData = self->data;
    
    if (objData->unk3EF & 8) {
        return 0;
    }
    
    if (!(joy_get_pressed(0) & B_BUTTON)) {
        return 0;
    }
    
    return 2;
}

// offset: 0x1A24 | func: 12 | export: 11
s32 dll_732_func_1A24(s32 arg0) {
    return 2;
}

// offset: 0x1A34 | func: 13 | export: 12
void dll_732_func_1A34(Object* self, f32* ox, f32* oy, f32* oz) {
    DLL732_Data* objData = self->data;
    
    *ox = objData->unk3AC.x;
    *oy = objData->unk3AC.y;
    *oz = objData->unk3AC.z;
}

// offset: 0x1A58 | func: 14 | export: 13
s8 dll_732_func_1A58(Object* self) {
    DLL732_Data* objData = self->data;
    return objData->unk3F0;
}

// offset: 0x1A68 | func: 15 | export: 14
void dll_732_func_1A68(Object* self, s32 arg1) {
    DLL732_Data* objData = self->data;
    
    objData->unk3F0 = arg1;
    if ((arg1 == 2) && !(objData->unk3EF & 0x20)) {
        objData->unk3C8 = 0x2710;
        objData->unk3CC = 2.0f;
        objData->unk3D0 = 4.0f;
        objData->unk3D4 = 4.6f;
        gDLL_1_cmdmenu->vtbl->energy_bar_create(0, 13000, TEXTABLE_569, TEXTABLE_56A, 1);
    }
}

// offset: 0x1B10 | func: 16 | export: 15
void dll_732_func_1B10(Object* self, f32* arg1, s32* arg2) {
    DLL732_Data* objData = self->data;
    
    *arg1 = objData->unk3DC / 2500.0f;
    
    if (*arg1 > 1.0f) {
        *arg1 = 1.0f;
    } else if (*arg1 < -1.0f) {
        *arg1 = -1.0f;
    }
    
    *arg2 = objData->unk3DC < 0;
}

// offset: 0x1B9C | func: 17 | export: 16
f32 dll_732_func_1B9C(Object* self, f32* arg1) {
    DLL732_Data* objData;
    f32 distance;

    objData = self->data;
    
    *arg1 = 5.0f;

    distance = sqrtf(SQ(objData->unk2AC.unk0.transl.f[0]) + SQ(objData->unk2AC.unk0.transl.f[1]) + SQ(objData->unk2AC.unk0.transl.f[2])) * 0.2f;
    if (distance > 1.0f) {
        distance = 1.0f;
    }
    return distance;
}

// offset: 0x1C28 | func: 18 | export: 17
s8 dll_732_func_1C28(Object* self) {
    DLL732_Data* objData = self->data;
    return objData->unk3F1;
}

// offset: 0x1C38 | func: 19 | export: 18
void dll_732_func_1C38(Object* self) {
    s32 checkpointUID;
    checkpoint4_Setup* checkpointSetup;
    DLL732_Data* objData;
    DLL732_Setup* objSetup;

    objSetup = (DLL732_Setup*)self->setup;
    objData = self->data;
    
    switch (self->id) {
    default:
    case 0x38D:
        checkpointUID = data_C[objSetup->unk1C][0];
        break;
    case 0x38E:
        checkpointUID = data_C[objSetup->unk1C][1];
        break;
    case 0x4D4:
        checkpointUID = data_C[objSetup->unk1C][2];
        break;
    }
    
    checkpointSetup = (checkpoint4_Setup*)map_find_obj_setup(checkpointUID, NULL, NULL, NULL, NULL);
    if (checkpointSetup == NULL) {
        return;
    }
    
    self->srt.transl.x = checkpointSetup->base.x;
    self->srt.transl.y = checkpointSetup->base.y;
    self->srt.transl.z = checkpointSetup->base.z;
    self->srt.yaw = checkpointSetup->yaw << 8;

    dll_732_func_3FE0(self, objData);
    gDLL_4_Race->vtbl->func3(self, &objData->unk18, objSetup->unk1C);
    
    objData->unk0.transl.x = self->srt.transl.x;
    objData->unk0.transl.y = self->srt.transl.y;
    objData->unk0.transl.z = self->srt.transl.z;
    objData->unk0.yaw = self->srt.yaw;
}

// offset: 0x1DB8 | func: 20 | export: 19
void dll_732_func_1DB8(s32 arg0, s32 arg1) {

}

// offset: 0x1DC8 | func: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/732_CRSnowClawBike3/dll_732_func_1DC8.s")

// offset: 0x22BC | func: 22
void dll_732_func_22BC(Object* self, DLL732_Data2AC* arg1) {
    arg1->unk1C = 90;
    arg1->unk20 = 1/arg1->unk1C;
    arg1->unk28 = -11.0f;
    arg1->unk2C = 0.1f;
    arg1->unk30 = 13.0f;
    arg1->unk18 = (1/arg1->unk1C) * 0.01666666f;
    
    bss_10[3] = 0;
    bss_10[4] = 0;
    bss_10[5] = 0;
    bss_10[2] = 1;
}

// offset: 0x2340 | func: 23
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/732_CRSnowClawBike3/dll_732_func_2340.s")

// offset: 0x2E64 | func: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/732_CRSnowClawBike3/dll_732_func_2E64.s")

// offset: 0x3618 | func: 25
void dll_732_func_3618(Object* self, DLL732_Unk_3618* arg1, u8 controllerPort, s32 buffer) {
    arg1->unkE = joy_get_stick_x_buffered(controllerPort, buffer);
    arg1->unkF = joy_get_stick_y_buffered(controllerPort, buffer);
}

// offset: 0x3694 | func: 26
static void dll_732_func_3694(Object* self, DLL732_Data* objData, MtxF* arg2, s32 addToYaw, s32 useRoll, s32 usePitch) {
    SRT sp20;

    sp20.yaw = objData->unk3E0;
    sp20.pitch = usePitch ? self->srt.pitch : 0;
    sp20.roll = useRoll ? self->srt.roll : 0;
    sp20.scale = 1.0f;
    sp20.transl.x = self->srt.transl.x;
    sp20.transl.y = self->srt.transl.y;
    sp20.transl.z = self->srt.transl.z;
    
    if (addToYaw) {
        sp20.yaw += objData->unk3DC;
    }
    
    matrix_from_srt(arg2, &sp20);
}

// offset: 0x3748 | func: 27
void dll_732_func_3748(Object* self, DLL732_Data* objData) {
    s32 damageType;
    s32 hitSphereID;
    s32 hitDamage;
    Object* hitBy;

    damageType = func_80025F40(self, &hitBy, &hitSphereID, &hitDamage);
    if (!(self->objhitInfo->unk58 & 1) || (damageType == 0)) {
        return;
    }
    
    switch (damageType) {
    case Damage_Type_D:
        objData->unk3EC = 20;
        objData->unk384 = 0.8f;
        return;
    case Damage_Type_14:
        objData->unk3EC = 5;
        objData->unk384 = 0.75f;
        return;
    case Damage_Type_1D:
        objData->unk3C8 += (hitDamage * 100);
        objData->unk3CC = 2.0f;
        objData->unk3D0 = 4.0f;
        objData->unk3D4 = 4.6f;
        if (objData->unk3C8 > 13000) {
            objData->unk3C8 = 13000;
        }
        break;
    }
}

// offset: 0x3860 | func: 28
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/732_CRSnowClawBike3/dll_732_func_3860.s")

// offset: 0x3AF8 | func: 29
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/732_CRSnowClawBike3/dll_732_func_3AF8.s")

// offset: 0x3DAC | func: 30
s32 dll_732_func_3DAC(Object* self, s32 arg1, DLL732_Data* objData, DLL732_Unk_3618* arg3) {
    f32 dx;
    f32 dz;
    s32 sp3C;
    f32 var_fv1;
    s32 dYaw;
    s32 angle;
    DLL732_Setup* objSetup;

    objSetup = (DLL732_Setup*)self->setup;
    
    dx = self->srt.transl.x - objData->unk0.transl.x;
    dz = self->srt.transl.z - objData->unk0.transl.z;
    dx = sqrtf(SQ(dx) + SQ(dz));
    
    var_fv1 = 100.0f - dx;
    if (dx > 100.0f) {
        var_fv1 = 0.0f;
    }
    
    sp3C = dll_732_func_0(self, objData, var_fv1);
    
    gDLL_4_Race->vtbl->func4(self, &objData->unk18);
    gDLL_4_Race->vtbl->func10(&objData->unk18);
    if (sp3C != 0) {
        arg3->unkE = 0;
        arg3->unkF = 0;
        return 1;
    }

    dx = self->srt.transl.x - objData->unk0.transl.x;
    dz = self->srt.transl.z - objData->unk0.transl.z;
    dYaw = arctan2_f(dx, dz) - (self->srt.yaw & 0xFFFF);
    CIRCLE_WRAP(dYaw);
    
    angle = dYaw >> 5;
    if (angle > 65) {
        angle = 65;
    } else if (angle < -65) {
        angle = -65;
    }
    
    arg3->unkE = -angle;
    if (angle < 0) {
        angle = -angle;
    }

    arg3->unkF = (objSetup->unk20 - 8.0f) - (angle * (objSetup->unk20 * 0.02f));
    diPrintf(" YJOY %i ", arg3->unkF);
    return 0;
}

// offset: 0x3FE0 | func: 31
void dll_732_func_3FE0(Object* self, DLL732_Data* objData) {
    MtxF sp60;
    s32 i;

    gDLL_27->vtbl->reset(self, &objData->unk4C);
    dll_732_func_3694(self, objData, &sp60, 0, 0, 0);
    
    for (i = 0; i < objData->unk3EE; i++) {
        vec3_transform(&sp60, 
            data_64[i].x, data_64[i].y, data_64[i].z, 
            &objData->unk330[i].x, &objData->unk330[i].y, &objData->unk330[i].z
        );
    }
}

// offset: 0x40FC | func: 32
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/732_CRSnowClawBike3/dll_732_func_40FC.s")

// offset: 0x4B30 | func: 33 | export: 20
void dll_732_func_4B30(s32 arg0, s32 arg1, s32 arg2) {

}
