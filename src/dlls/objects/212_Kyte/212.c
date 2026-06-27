#include "common.h"
#include "dlls/objects/278_flameblast.h"
#include "dlls/objects/332_FXEmit.h"

typedef struct {
    s16 unk0;
    u16 pad2;
    s32 unk4;
    u8 unk8;
} DLL212_3FF4;

typedef struct {
    void *unk0; // loaded dll
    Object *unk4[3];
    Object *unk10;
    s32 unk14;
    u8 pad18[0x24 - 0x18];
    Vec3f unk24;
    s32 unk30;
    u8 pad34[0x1D8 - 0x34];
    s16 unk1D8;
    u16 pad1DA;
    u32 pad1DC;
    u32 unk1E0;
    u8 pad1E4[0x278 - 0x1E4];
} DLL212_Data;

/*0x0*/ static u32 data_0[] = {
    0x0000000d, 0x00000005, 0xffffffff, 0x0000000b, 0x0000030a, 0x3f800000, 0x3f800000, 0x3f800000, 
    0x3f800000, 0x3f800000
};
/*0x28*/ static u32 data_28[] = {
    0x00000006, 0x00000013, 0x00000012, 0x00000015, 0xffffffff, 0x00000016, 0x00000018, 0x3f800000, 
    0x3f800000, 0x3f800000, 0x3f800000
};
/*0x54*/ static u32 data_54[] = {
    0x00000000, 0x00000009, 0x00000008, 0x00000004, 0x00000002, 0xffffffff, 0x3f800000, 0x3f800000, 
    0x3f800000, 0x3f800000, 0x00000000, 0x00000009, 0x00000008, 0x00000004, 0x00000002, 0xffffffff
};
/*0x94*/ static u32 data_94[] = {
    0x3fc00000, 0x40200000, 0x41a00000, 0x3e19999a
};
/*0xA4*/ static u32 data_A4[] = {
    (u32)&data_0, 0x00000002, 0x3edc28f6, 0x3f99999a, 0x40000000, 0x3e4ccccd
};
/*0xBC*/ static u32 data_BC[] = {
    (u32)&data_28, 0x00000003, 0x3dcccccd, 0x3ecccccd, 0x3ef0a3d7, 0x3e4ccccd
};
/*0xD4*/ static u32 data_D4[] = {
    (u32)&data_54, 0x00000003, 0x00000002, 0x00000000, 0x00000001
};
/*0xE8*/ static u32 data_E8 = 0x00000004;
/*0xEC*/ static u32 data_EC[] = {
    0x00000008, 0x00000002, 0x00000004, 0x0000c254, 0x00000292, 0x0000d144, 0x004148c4, 0x00404282, 
    0x0020c444, 0x08208482, 0x00000004, 0x00000002, 0x00094248, 0x00024248, 0x00044248, 0x00814008, 
    0x00104048, 0x0000d148, 0x0000c148, 0x0000c144, 0x0400c148, 0x08208444, 0x18200482
};
/*0x148*/ static s16 data_148[] = { -1, -1, 0x08bb, -1, -1, -1 };
/*0x154*/ static u32 data_154[] = {
    0x00000000, 0x00000002
};
/*0x15C*/ static u32 data_15C[] = {
    0x00000000, 0x00000000, 0x3f800000
};
/*0x168*/ static u32 data_168 = 0xffffffff;
/*0x16C*/ static u32 data_16C[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000
};
/*0x19C*/ static u32 data_19C[] = {
    0x00000000, 0x00000030, 0x00000004, 0x00000032
};
/*0x1AC*/ static s32 data_1AC[] = { 0x15, 0x09, 0x08, 0x16 };

/*0x0*/ static u8 bss_0[0x10];

// offset: 0x0 | ctor
void dll_212_ctor(void* dll) { }

// offset: 0xC | dtor
void dll_212_dtor(void* dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_212_setup(Object* self, ObjSetup* setup, s32 reset);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_setup.s")

// offset: 0x120 | func: 1 | export: 1
void dll_212_control(Object* self);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_control.s")

// offset: 0x6B8 | func: 2 | export: 2
void dll_212_update(Object* self) { }

// offset: 0x6C4 | func: 3 | export: 3
void dll_212_print(Object* self, Gfx** gdl, Mtx** mtxs, Vertex** vtxs, Triangle** pols, s8 visibility);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_print.s")

// offset: 0x868 | func: 4 | export: 4
void dll_212_free(Object* self, s32 onlySelf);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_free.s")

// offset: 0x8C4 | func: 5 | export: 5
u32 dll_212_get_model_flags(Object* self) {
    return MODFLAGS_NONE;
}

// offset: 0x8D4 | func: 6 | export: 6
u32 dll_212_get_data_size(Object* self, u32 offsetAddr) {
    return sizeof(DLL212_Data);
}

// offset: 0x8E8 | func: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_8E8.s")

// offset: 0xA94 | func: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_A94.s")

// offset: 0xAB4 | func: 9 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_AB4.s")

// offset: 0xAC4 | func: 10 | export: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_AC4.s")

// offset: 0xADC | func: 11 | export: 9
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_ADC.s")

// offset: 0xAEC | func: 12 | export: 10
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_AEC.s")

// offset: 0xB04 | func: 13 | export: 11
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_B04.s")

// offset: 0xB24 | func: 14 | export: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_B24.s")

// offset: 0xB34 | func: 15 | export: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_B34.s")

// offset: 0xB94 | func: 16 | export: 14
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_B94.s")

// offset: 0xC04 | func: 17 | export: 15
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_C04.s")

// offset: 0xC18 | func: 18 | export: 16
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_C18.s")

// offset: 0xC2C | func: 19 | export: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_C2C.s")

// offset: 0xC48 | func: 20 | export: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_C48.s")

// offset: 0xC5C | func: 21 | export: 19
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_C5C.s")

// offset: 0xC74 | func: 22 | export: 20
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_C74.s")

// offset: 0xC8C | func: 23 | export: 21
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_C8C.s")

// offset: 0xC9C | func: 24 | export: 22
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_C9C.s")

// offset: 0xCAC | func: 25 | export: 24
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_CAC.s")

// offset: 0xCCC | func: 26 | export: 25
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_CCC.s")

// offset: 0xCDC | func: 27 | export: 23
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_CDC.s")

// offset: 0xCE8 | func: 28
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_CE8.s")

// offset: 0xF08 | func: 29
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_F08.s")

// offset: 0x1134 | func: 30
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_1134.s")

// offset: 0x1404 | func: 31
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_1404.s")

// offset: 0x1730 | func: 32
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_1730.s")

// offset: 0x1864 | func: 33
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_1864.s")

// offset: 0x18F8 | func: 34
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_18F8.s")

// offset: 0x1D2C | func: 35
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_1D2C.s")

// offset: 0x1F14 | func: 36
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_1F14.s")

// offset: 0x200C | func: 37
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_200C.s")

// offset: 0x20A4 | func: 38
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_20A4.s")

// offset: 0x27D8 | func: 39
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_27D8.s")

// offset: 0x2B58 | func: 40
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_2B58.s")

// offset: 0x2DA4 | func: 41
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_2DA4.s")

// offset: 0x300C | func: 42
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_300C.s")

// offset: 0x3098 | func: 43
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_3098.s")

// offset: 0x319C | func: 44
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_319C.s")

// offset: 0x35C0 | func: 45
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/objects/212_Kyte/dll_212_func_35C0.s")

// offset: 0x3A2C | func: 46
s32 dll_212_func_3A2C(Object* self, DLL212_Data* objdata) {
    FlameBlast_Setup* temp_v0_2;
    FXEmit_Setup* temp_v0_3;
    SidekickStats* temp_v0;
    s32 i;

    gDLL_29_Gplay->vtbl->get_sidekick_stats();
    temp_v0 = gDLL_29_Gplay->vtbl->get_sidekick_stats();
    if ((temp_v0->redFood == 0) || (objdata->unk14 == 1)) {
        return 0;
    }

    temp_v0->redFood -= 1;
    objdata->unk14 = 1;
    objdata->unk30 = 0x1F4; // maybe: BIT_Used_Tricky_Cell_Key
    objdata->unk0 = dll_load_deferred(DLL_ID_178, 1);
    for (i = 0; i < 3; i++) {
        temp_v0_2 = obj_alloc_setup(sizeof(FlameBlast_Setup), OBJ_flameblast);
        temp_v0_2->base.x = self->srt.transl.x;
        temp_v0_2->base.y = self->srt.transl.y;
        temp_v0_2->base.z = self->srt.transl.z;
        temp_v0_2->base.loadFlags = 2;
        temp_v0_2->base.byte5 = 1;
        temp_v0_2->timer = i * 10;
        objdata->unk4[i] = obj_create(&temp_v0_2->base, 5, self->mapID, -1, self->parent);
    }

    temp_v0_3 = obj_alloc_setup(sizeof(FXEmit_Setup), OBJ_FXEmit);
    temp_v0_3->base.loadFlags = 2;
    temp_v0_3->base.byte5 = 1;
    temp_v0_3->base.x = self->srt.transl.x;
    temp_v0_3->base.y = self->srt.transl.y;
    temp_v0_3->base.z = self->srt.transl.z;
    temp_v0_3->toggleGamebit = -1;
    temp_v0_3->disableGamebit = -1;
    temp_v0_3->yaw = 0;
    temp_v0_3->flagConfig = 1;
    temp_v0_3->pitch = 0;
    temp_v0_3->roll = 0;
    temp_v0_3->rollSpeed = 0;
    temp_v0_3->pitchSpeed = 0;
    temp_v0_3->yawSpeed = 0;
    temp_v0_3->activationRange = 0;
    temp_v0_3->bank = 1;
    temp_v0_3->indexInBank = 0x4A;
    temp_v0_3->fxRate = -0x1E;
    objdata->unk10 = obj_create(&temp_v0_3->base, 5, self->mapID, -1, self->parent);
    return 1;
}

// offset: 0x3C4C | func: 47
CurveSetup* dll_212_func_3C4C(Object* self) {
    CurveSetup* temp_v0;
    CurveSetup* var_s3;
    f32 temp_fv0;
    f32 var_fs0;
    s32 i;

    var_fs0 = 700.0f;
    var_s3 = NULL;
    for (i = 0; i < 4; i++) {
        temp_v0 = gDLL_25->vtbl->func_2CF8(self, data_1AC[i]);
        if (temp_v0 != NULL) {
            temp_fv0 = vec3_distance(&self->globalPosition, &temp_v0->pos);
            if (temp_fv0 < var_fs0) {
                var_fs0 = temp_fv0;
                var_s3 = temp_v0;
            }
        }
    }

    return var_s3;
}

// offset: 0x3D30 | func: 48
void dll_212_func_3D30(Object* self, DLL212_Data* objdata) {
    SRT sp48;
    u8 var_s0;

    if (objdata->unk14 == 0) {
        return;
    }

    sp48.transl.x = objdata->unk24.x - self->srt.transl.x;
    sp48.transl.y = objdata->unk24.y - self->srt.transl.y;
    sp48.transl.z = objdata->unk24.z - self->srt.transl.z;
    sp48.scale = 1.0f;
    sp48.yaw = self->srt.yaw;
    sp48.pitch = self->srt.pitch;
    sp48.roll = self->srt.roll;
    if (objdata->unk14 == 1) {
        objdata->unk10->srt.transl.x = objdata->unk24.x;
        objdata->unk10->srt.transl.y = objdata->unk24.y;
        objdata->unk10->srt.transl.z = objdata->unk24.z;
        objdata->unk10->srt.roll = self->srt.roll;
        objdata->unk10->srt.pitch = self->srt.pitch;
        objdata->unk10->srt.yaw = self->srt.yaw;
        gDLL_17_partfx->vtbl->spawn(self, 0x535, &sp48, 2, -1, NULL);
        gDLL_17_partfx->vtbl->spawn(self, 0x532, &sp48, 2, -1, NULL);
        return;
    }

    gDLL_6_AMSFX->vtbl->play(self, SOUND_95B, MAX_VOLUME, NULL, NULL, 0, NULL);
    var_s0 = 20;
    // FAKE
    do {} while (0);
    while (var_s0--) {
        gDLL_17_partfx->vtbl->spawn(self, 0x533, &sp48, 2, -1, NULL);
    }
    objdata->unk14 = 0;
}

// offset: 0x3F44 | func: 49
void dll_212_func_3F44(s32 arg0, DLL212_Data* objdata) {
    s32 i;

    if (objdata->unk14 == 1) {
        objdata->unk14 = 2;
        dll_unload(objdata->unk0);
        obj_destroy_object(objdata->unk10);
        for (i = 0; i < 3; i++) { obj_destroy_object(objdata->unk4[i]); }
    }
}

// offset: 0x3FF4 | func: 50
void dll_212_func_3FF4(DLL212_3FF4* arg0, s32 arg1, s16 arg2) {
    arg0->unk4 = arg1;
    arg0->unk0 = arg2;
    arg0->unk8 = (arg0->unk8 & 0xFF7F) | 0x80;
}

// offset: 0x4020 | func: 51
void dll_212_func_4020(DLL212_3FF4* arg0, s32 arg1) {
    if (arg1 == arg0->unk4) {
        arg0->unk8 &= ~0x80;
    }
}

/*0x0*/ static const char str_0[] = "kyte has enterd an invalid state\n";
/*0x24*/ static const char str_24[] = " In Seq ";
/*0x30*/ static const char str_30[] = " Enable Dis %i ";
/*0x40*/ static const char str_40[] = " cleaning Up Kyte ";
/*0x54*/ static const char str_54[] = "sideCommandEnable error, command number too high\n";
/*0x88*/ static const char str_88[] = " Removing Distract ";
/*0x9C*/ static const char str_9C[] = "kyte startup warning, flight group is zero\n";
/*0xC8*/ static const char str_C8[] = "\nInitialising On Flight Group %i \n ";
/*0xEC*/ static const char str_EC[] = "\n Starting On Flight Group %i \n ";
/*0x110*/ static const char str_110[] = " Kyte Start on anon valid node ";
/*0x130*/ static const char str_130[] = "error in setting up the curve network\n";
/*0x158*/ static const char str_158[] = "curve %d, flightGroup %d, dir %d\n";
/*0x17C*/ static const char str_17C[] = "1.) Same as last frame\n";
/*0x194*/ static const char str_194[] = "2.) psuedonode\n";
/*0x1A4*/ static const char str_1A4[] = "3.) After psuedonode\n";
/*0x1BC*/ static const char str_1BC[] = "4.) Staying on the flight group %i \n";
/*0x1E4*/ static const char str_1E4[] = "setting the flight group to %d\n";
/*0x204*/ static const char str_204[] = "5.) Used route finding\n";
/*0x21C*/ static const char str_21C[] = "6.) Next node of standard type (flightGroup %d)\n";
/*0x250*/ static const char str_250[] = "7.) Staying on flight Group (even though invalid)\n";
/*0x284*/ static const char str_284[] = "8.) Just taking any old curve\n";
/*0x2A4*/ static const char str_2A4[] = "kyte error!, unable to find next curve node from %d!\n";
/*0x2DC*/ static const char str_2DC[] = "Extract route failed.\n";
/*0x2F4*/ static const char str_2F4[] = "Route finding, start %d finish %d\n";
/*0x318*/ static const char str_318[] = "routeFindNode error: couldn't find route between %d %d, return val %d\n";
/*0x360*/ static const char str_360[] = "Node exists in curve network, that isn't Kyte AI, from curvenode %d\n";
/*0x3A8*/ static const char str_3A8[] = " Found Perch ";
/*0x3B8*/ static const char str_3B8[] = " Found Perch ";
/*0x3C8*/ static const char str_3C8[] = " Perching %f ";
/*0x3D8*/ static const char str_3D8[] = "vel %f  dir %f ";
/*0x3E8*/ static const char str_3E8[] = "\n\n CURVES MOVE | CURVES MOVE \n\n";
/*0x408*/ static const char str_408[] = " Trams ";
/*0x410*/ static const char str_410[] = "Performing Kyte action ";
/*0x428*/ static const char str_428[] = "Performing Kyte action ";
/*0x440*/ static const char str_440[] = "     PERCH ::: READ KYTTE PERCH COMMAND \n\n";
/*0x46C*/ static const char str_46C[] = " \n\n Timer Reset \n";
/*0x480*/ static const char str_480[] = " LocK Activated ";
/*0x494*/ static const char str_494[] = "clearing up\n";

// offset: 0x4040 | func: 52 | export: 26
int dll_212_func_4040(Object* self, s32* arg1) {
    DLL212_Data* objdata;

    objdata = self->data;
    if ((objdata->unk1E0 >> 0x1F) != 0) {
        diPrintf(" COMMAND ACTIVE ");
        if (arg1 != NULL) {
            *arg1 = objdata->unk1D8;
        }
        return 1;
    }

    return 0;
}

// offset: 0x40BC | func: 53
int dll_212_func_40BC(u8 arg0) {
    return (data_148[arg0] + 1) == 0 || main_get_bits(data_148[arg0]) != 0;
}

