#include "common.h"
#include "dlls/engine/18.h"
#include "dlls/objects/210_player.h"
#include "sys/objanim.h"
#include "sys/objmsg.h"
#include "sys/objtype.h"

// TODO: move to DLL 260
typedef struct {
    u8 _unk0[0x12 - 0x0];
    s16 unk12;
} Pollen_Data;

/////////////////
// TODO: voxmap stuff, move out of here
typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
} SomeVoxmapAllocStruct;

extern void func_80008E08(SomeVoxmapAllocStruct*);
extern void func_80008DC0(SomeVoxmapAllocStruct*);
///////////////

typedef struct {
    u8 _unk0[0x24 - 0x0];
    s8 unk24;
    u8 unk25;
    s8 unk26;
    s8 unk27;
} DLL33Data_34C;

typedef struct {
    DLL18_Data unk0;
    DLL33Data_34C unk34C;
    SomeVoxmapAllocStruct unk374;
    u8 _unk380[0x39C - 0x380];
    s16 unk39C;
    s16 unk39E;
    s16 unk3A0;
    s16 unk3A2;
    s16 unk3A4;
    s16 unk3A6;
    u32 unk3A8;
    u8 _unk3AC[0x3B0 - 0x3AC];
    u8 unk3B0;
    u8 _unk3B1;
    u16 unk3B2;
    u8 unk3B4;
    u8 _unk3B5;
    s16 unk3B6;
    u8 unk3B8;
    s8 unk3B9;
    s8 _unk3BA;
    s8 unk3BB;
    u8 _unk3BC[0x3E0 - 0x3BC];
    s16 unk3E0;
    u16 unk3E2;
    f32 unk3E4;
    f32 unk3E8;
    f32 unk3EC;
    u8 unk3F0;
    u8 _unk3F1[0x3F4 - 0x3F1];
    void *unk3F4;
    UnkCurvesStruct *unk3F8;
    s32 unk3FC;
} DLL33_Data;

typedef struct {
    ObjSetup base;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    u8 _unk24[0x27 - 0x24];
    s8 unk27;
    s8 unk28;
    u8 unk29;
    s8 unk2A;
    u8 unk2B;
    s16 unk2C;
    s8 unk2E;
    u8 unk2F;
    s16 unk30;
    u8 unk32;
} DLL33_ObjSetup;

/*0x0*/ static f32 _data_0 = 0.0;
/*0x4*/ static u32 _data_4 = 0x00000000;
/*0x8*/ static u32 _data_8[] = {
    0x00000000, 0x00000000, 0x00000000
};
/*0x14*/ static u32 _data_14 = 0x00000000;
/*0x18*/ static u32 _data_18[] = {
    0x00000000, 0x00000000, 0x00000000
};
/*0x24*/ static u32 _data_24 = 0x41c80000;

/*0x0*/ static Object *_bss_0;

// offset: 0x0 | ctor | export: 0
void dll_33_ctor(void *dll) { }

// offset: 0xC | dtor | export: 1
void dll_33_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 5
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/33/dll_33_func_18.s")

// offset: 0x264 | func: 1 | export: 6
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/33/dll_33_func_264.s")

// offset: 0x278 | func: 2 | export: 4
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/33/dll_33_func_278.s")

// offset: 0x4EC | func: 3 | export: 2
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/33/dll_33_func_4EC.s")

// offset: 0x768 | func: 4 | export: 3
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/33/dll_33_func_768.s")

// offset: 0x884 | func: 5 | export: 7
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/33/dll_33_func_884.s")

// offset: 0x8A4 | func: 6 | export: 8
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/33/dll_33_func_8A4.s")

// offset: 0x8B4 | func: 7 | export: 12
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/33/dll_33_func_8B4.s")

// offset: 0xC88 | func: 8 | export: 13
s32 dll_33_func_C88(Object* arg0, DLL33_Data* arg1, dll18_callback *arg2, dll18_callback *arg3, s16 arg4, f32* arg5, f32* arg6, s32* arg7) {
    f32 sp2C;
    f32 sp28;
    f32 temp_fv0;

    if (arg1->unk34C.unk25 != 0) {
        arg1->unk0.unk30C = 0;
        arg1->unk0.unk310 = 0;
        arg1->unk0.unk324 = 0;
        arg1->unk0.unk288 = 0.0f;
        arg1->unk0.unk284 = 0.0f;
        *arg7 = 1;
        sp2C = *arg5 - arg0->srt.transl.x;
        sp28 = *arg6 - arg0->srt.transl.z;
        temp_fv0 = sqrtf((sp2C * sp2C) + (sp28 * sp28));
        if (temp_fv0 < 10.0f) {
            *arg7 = 0;
        } else {
            sp2C = sp2C / temp_fv0;
            sp28 = sp28 / temp_fv0;
            arg1->unk0.unk288 = -sp2C * 50.0f;
            arg1->unk0.unk284 = sp28 * 50.0f;
            arg0->srt.transl.x += temp_fv0 * sp2C;
            arg0->srt.transl.z += temp_fv0 * sp28;
            gDLL_18->vtbl->func1(arg0, &arg1->unk0, delayFloat, delayFloat, arg2, arg3);
        }
        if (*arg7 == 0) {
            arg1->unk3B4 = 0;
            arg1->unk0.unk26C = arg4;
            arg1->unk0.unk2C8 = NULL;
            arg1->unk0.unk4.mode = 0;
            main_set_bits(arg1->unk39E, 0);
        }
        return 1;
    }
    
    return 0;
}

// offset: 0xE30 | func: 9 | export: 10
void dll_33_func_E30(Object* arg0, DLL33_Data* arg1, f32 arg2, s8 arg3) {
    arg1->unk0.unk324 = 0;
    arg1->unk0.unk0 |= 0x8000;
    if (arg0->objhitInfo != NULL) {
        func_80026128(arg0, 0, 0, -1);
    }
    if (arg3 != -1) {
        arg1->unk0.unk4.mode = arg3;
    }
    arg1->unk0.unk29C = arg2;
    arg1->unk0.unk288 = 0.0f;
    arg1->unk0.unk284 = 0.0f;
    arg1->unk0.unk310 = 0;
    arg1->unk0.unk30C = 0;
}

// offset: 0xED0 | func: 10 | export: 11
s32 dll_33_func_ED0(Object* arg0, DLL33_Data* arg1, u8 arg2) {
    if (arg2 && (arg1->unk0.unk348 <= 0) && (arg0->unk_0x36 == 0)) {
        return 0;
    }
    if ((arg0->parent == NULL) && (func_8004454C(arg0->srt.transl.x, arg0->srt.transl.y, arg0->srt.transl.z) < 0)) {
        return 0;
    }
    return 1;
}

// offset: 0xF60 | func: 11 | export: 16
s32 dll_33_func_F60(Object* arg0, DLL33_Data* arg1, f32 arg2, s32 arg3) {
    Object* player;
    Func_80059C40_Struct sp48;
    Vec3f sp3C;
    s32 var_v1;

    player = get_player();
    var_v1 = 0;
    if (arg1->unk0.unk33A != 0) {
        if ((player == arg1->unk0.unk2C8) && (arg1->unk0.unk348 != 0)) {
            if ((arg2 < arg1->unk0.unk2B8) && (arg3 != 0)) {
                var_v1 = 1;
            } else {
                if (((DLL_210_Player*)player->dll)->vtbl->func66(player, 1) == 0) {
                    var_v1 = 1;
                } else {
                    if (((DLL_210_Player*)player->dll)->vtbl->func26.withOneArgS32((s32)player) <= 0) { // TODO: player func
                        var_v1 = 1;
                    } else {
                        sp3C.x = player->srt.transl.x;
                        sp3C.y = player->srt.transl.y + 10.0f;
                        sp3C.z = player->srt.transl.z;
                        if (func_80059C40(&arg0->srt.transl, &sp3C, 1.0f, 0, &sp48, arg0, 4, -1, 0U, 0) != 0) {
                            var_v1 = 1;
                        }
                    }
                }
            }
        } else {
            var_v1 = 1;
        }
    }
    return var_v1;
}

// offset: 0x10F4 | func: 12 | export: 17
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/33/dll_33_func_10F4.s")

// offset: 0x148C | func: 13 | export: 9
void dll_33_func_148C(Object* arg0, DLL18_Data* arg1, DLL33Data_34C *arg2, s16 arg3, s8 *arg4, s16 arg5, s16 arg6, s32 arg7, s8 arg8) {
    if (arg2 != 0) {
        arg2->unk24 = 0;
        arg2->unk25 = 0;
        arg2->unk26 = 4;
        arg2->unk27 = 0x14;
    }
    if (arg5 != -1) {
        arg1->unk268 = arg5;
        arg1->unk273 = 1;
    }
    if (arg6 != -1) {
        gDLL_18->vtbl->func4(arg0, arg1, arg6);
    }
    if (arg4 != 0) {
        *arg4 = 2;
    }
    if (arg7 != 0) {
        func_80023D30(arg0, arg7, 0.0f, 0);
    }
    gDLL_27->vtbl->reset(arg0, &arg1->unk4);
    if (arg8 != -1) {
        arg1->unk4.mode = arg8;
    }
    if (arg3 != -1) {
        main_set_bits(arg3, 1);
    }
}

// offset: 0x15CC | func: 14 | export: 18
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/33/dll_33_func_15CC.s")

// offset: 0x18E4 | func: 15 | export: 19
s32 dll_33_func_18E4(Object* arg0, DLL18_Data* arg1, s32 arg2, s32 arg3, s32 *arg4, s8 *arg5, s16 arg6, u32* arg7, SRT* arg8) {
    DLL33_Data* objdata;
    Object* player;
    s32 sp5C;
    s32 sp58;
    s32 sp54;
    Object* sp50;
    f32 sp4C;
    f32 sp48;
    f32 sp44;

    objdata = (DLL33_Data*)arg0->data;
    player = get_player();
    if (objdata->unk3E8 > 0.0f) {
        objdata->unk3E8 += (delayFloat * objdata->unk3EC);
        if (objdata->unk3B2 & 0x20) {
            objdata->unk3B2 &= ~0x20;
            objdata->unk3B2 |= 0x40;
            if (objdata->unk3E8 > 2.0f) {
                objdata->unk3E8 = 0.0f;
                objdata->unk3B2 &= ~0x40;
            }
        } else if (objdata->unk3B2 & 0x40) {
            if (objdata->unk3E8 > 2.0f) {
                DLL33_ObjSetup* objsetup = (DLL33_ObjSetup*)arg0->setup;
                objdata->unk3E8 = 0.0f;
                objdata->unk3B2 &= ~0x40;
                arg1->unk348 = 0;
                arg0->unk_0x36 = 0;
                arg0->unk0xdc = 1;
                gDLL_29_Gplay->vtbl->add_time(objsetup->base.uID, (f32) (objsetup->unk2C * 60));
            }
        } else {
            if (objdata->unk3E8 < 0.0f) {
                objdata->unk3E8 = 0.0f;
            } else if (objdata->unk3E8 > 120.0f) {
                objdata->unk3E8 = (120.0f - (objdata->unk3E8 - 120.0f));
                objdata->unk3EC = -objdata->unk3EC;
            }
        }
    }
    if (arg1->unk348 == 0) {
        return 0;
    }
    sp5C = func_8002601C(arg0, &sp50, &sp58, &sp54, &sp4C, &sp48, &sp44);
    objdata->unk3F0 = (s8) sp58;
    if ((arg0 != NULL) && (sp5C != 0) && (sp50 != NULL)) {
        switch (arg0->id) {
        case OBJ_ScorpionRobot:
            if ((sp50->id != OBJ_sword) && (sp50->id != OBJ_staff) && (sp50->id != OBJ_projball)) {
                return 0;
            }
            break;
        case OBJ_WG_PollenCannon:
            if (sp50->id == OBJ_Pollen) {
                if (((Pollen_Data*)sp50->data)->unk12 == 0) {
                    sp50->unk_0x36 = 0;
                }
                return 0;
            }
            if (sp50->id == OBJ_PollenFragment) {
                sp50->unk_0x36 = 0;
                return 0;
            }
            break;
        }
    }
    
    if (sp5C != 0) {
        sp54 *= 4;
        if (arg8 != NULL) {
            arg8->transl.x = sp4C + gWorldX;
            arg8->transl.y = sp48;
            arg8->transl.z = sp44 + gWorldZ;
        }
        if (arg5 != 0) {
            if (arg5[sp5C - 2] != -1) {
                sp54 = (s32) arg5[sp5C - 2];
            }
        } else {
            sp54 = 0;
        }
        arg1->unk348 -= sp54;
        if (arg1->unk348 <= 0) {
            objdata->unk3B2 |= 0x20;
            objdata->unk3E8 = 1.0f;
            objdata->unk3EC = 0.01f;
            arg1->unk268 = arg6;
            arg1->unk348 = 0;
        } else if (sp54 != 0) {
            if ((arg1->unk2C8 == NULL) && (((DLL_210_Player*)player->dll)->vtbl->func66(player, 1) != 0)) {
                arg1->unk2C8 = player;
                arg1->unk33D = 0;
            }
            objdata->unk3E8 = 1.0f;
            objdata->unk3EC = 12.0f;
            if (arg4 != 0) {
                if (arg4[sp5C - 2] != -1) {
                    gDLL_18->vtbl->func4(arg0, arg1, arg4[sp5C - 2]);
                    arg1->unk268 = arg6;
                }
            }
            arg1->unk343 = (s8) sp5C;
        }
        if (*arg7 != 0) {
            gDLL_6_AMSFX->vtbl->func_A1C(*arg7);
            *arg7 = 0;
        }
        obj_send_mesg(sp50, 0xE0001, arg0, NULL);
    }
    return sp5C;
}

// offset: 0x1D88 | func: 16 | export: 20
s32 dll_33_func_1D88(Object* arg0, DLL18_Data* arg1, DLL33Data_34C *arg2, s16 arg3, s8 *arg4, s16 arg5, s16 arg6, s16 arg7) {
    Object* sp64;
    u32 sp60;
    u32 sp5C;

    sp5C = NULL;
    while (obj_recv_mesg(arg0, &sp60, &sp64, (void*)&sp5C) != 0) {
        switch (sp60) {
            case 0x140001:
                gDLL_6_AMSFX->vtbl->play_sound(arg0, SOUND_1E3_SharpClaw_Ah_Shuddup, MAX_VOLUME, NULL, NULL, 0, NULL);
                break;
            case 0x4:
                obj_send_mesg(sp64, 5, arg0, NULL);
                break;
            case 0xE0000:
                if (sp64 == arg1->unk2C8) {
                    arg1->unk268 = arg5;
                    arg1->unk2C8 = NULL;
                    arg1->unk33D = 0;
                }
                break;
            case 0xB:
                arg1->unk342 = sp5C;
                break;
            case 0x1:
            case 0xA0001:
                if (arg6 != arg1->unk268) {
                    dll_33_func_148C(arg0, arg1, arg2, arg3, arg4, arg5, arg7, 0, 1);
                    arg1->unk268 = arg6;
                    arg1->unk33D = 0;
                    arg1->unk2C8 = sp64;
                    return 1;
                }
                break;
            case 0x3:
                if (arg6 == arg1->unk268) {
                    arg1->unk33D = 0;
                    arg1->unk2C8 = NULL;
                    arg1->unk268 = arg5;
                    return 2;
                }
                break;
        }
    }

    return 0;
}

// offset: 0x1FAC | func: 17 | export: 14
// TODO: arg count might be off
void dll_33_func_1FAC(Object* arg0, DLL33_Data* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    gDLL_2_Camera->vtbl->func14();
}

// offset: 0x2000 | func: 18 | export: 21
void dll_33_func_2000(Object* obj, DLL33_ObjSetup* setup, DLL33_Data* data, s32 arg3, s32 arg4, s32 arg5, u8 arg6, f32 arg7) {
    s32 sp4C[] = { 0x2 };
    u8 sp4B;
    u8 temp_v0;
    s32 sp3C;
    s32 sp38;

    sp4B = 1;
    data->unk3F4 = &data->unk3FC;
    data->unk3B6 = 0;
    sp38 = arg6 & 1;
    sp3C = (s32) arg6;
    if ((sp38 == 0) && !(arg6 & 0x20)) {
        obj_add_object_type(obj, 4);
        obj_init_mesg_queue(obj, 4U);
    }
    gDLL_18->vtbl->func0(obj, &data->unk0, arg3, arg4);
    data->unk0.unk0 = 0;
    data->unk0.unk33D = 0;
    data->unk0.unk278 = 0.0f;
    data->unk0.unk27C = 0.0f;
    temp_v0 = setup->unk32;
    if (temp_v0 != 0) {
        data->unk0.unk348 = temp_v0 * 4;
    } else {
        data->unk0.unk348 = 0x18;
    }
    data->unk39E = setup->unk30;
    data->unk3A0 = setup->unk1A;
    data->unk3A2 = setup->unk1C;
    if (data->unk39E != -1) {
        main_set_bits(data->unk39E, 0U);
    }
    if (sp3C & 2) {
        gDLL_27->vtbl->init(&data->unk0.unk4, 0, arg5 | 0x200000, 1);
    } else {
        gDLL_27->vtbl->init(&data->unk0.unk4, 0, 0, 0);
    }
    gDLL_27->vtbl->setup_hits_collider(&data->unk0.unk4, 1, (Vec3f* ) _data_18, (f32* ) &_data_24, 4);
    if (sp3C & 4) {
        gDLL_27->vtbl->setup_terrain_collider(&data->unk0.unk4, 1, (Vec3f* ) _data_8, (f32* ) &_data_14, &sp4B);
    }
    gDLL_27->vtbl->reset(obj, &data->unk0.unk4);
    data->unk3A8 = 0;
    data->unk3B0 = setup->unk2B;
    data->unk3E0 = setup->unk22;
    data->unk3B8 = setup->unk2F;
    data->unk3B9 = setup->unk27;
    data->_unk3BA = setup->unk28;
    obj->unk0xb0 |= data->_unk3BA & 3;
    if (sp3C & 8) {
        data->unk3A4 = setup->unk20;
        data->unk3A6 = setup->unk1E;
    } else {
        data->unk3A4 = 0;
        data->unk3A6 = 0;
    }
    data->unk3B2 = 0;
    data->unk3E2 = setup->unk29 * 8;
    data->unk3B4 = 0;
    obj->srt.transl.x = setup->base.x;
    obj->srt.transl.y = setup->base.y;
    obj->srt.transl.z = setup->base.z;
    data->unk3E4 = arg7;
    obj->srt.yaw = setup->unk2A << 8;
    obj->unk_0x36 = 0xFF;
    obj->unk0xaf &= ~0x8;
    data->unk39C = setup->unk18;
    if (data->unk39C != -1) {
        obj->unk0xdc = main_get_bits(data->unk39C);
    } else {
        obj->unk0xdc = 0;
    }
    if (gDLL_29_Gplay->vtbl->did_time_expire(setup->base.uID) == 0) {
        obj->unk0xdc = 1;
    }
    if (obj->unk0xdc != 0) {
        func_800267A4(obj);
        return;
    }
    func_8002674C(obj);
    if (setup->unk2E == -1) {
        obj->unk_0xe0 = 1;
    } else {
        obj->unk_0xe0 = 0;
    }
    if ((sp38 == 0) && !(sp3C & 0x20)) {
        func_80008DC0(&data->unk374);
        data->unk34C.unk26 = 4;
        data->unk34C.unk27 = 0x14;
    }
    if (sp3C & 0x10) {
        if ((data->unk3F8 == NULL) && !(sp3C & 0x20)) {
            data->unk3F8 = mmAlloc(sizeof(UnkCurvesStruct), ALLOC_TAG_TEST_COL, NULL);
        }
        if (data->unk3F8 != NULL) {
            bzero(data->unk3F8, sizeof(UnkCurvesStruct));
        }
        if (gDLL_26_Curves->vtbl->curves_func_4288(data->unk3F8, obj, (f32) data->unk3E2, sp4C, -1) == 0) {
            data->unk3B2 |= 8;
        }
    } else {
        data->unk3F8 = NULL;
    }
}

// offset: 0x24FC | func: 19 | export: 15
void dll_33_func_24FC(Object* arg0, DLL33_Data* arg1, u8 arg2) {
    if (arg1->unk3A8 != 0) {
        gDLL_6_AMSFX->vtbl->func_A1C(arg1->unk3A8);
    }
    if (!(arg1->unk3B0 & arg2)) {
        if (arg1->unk3A6 != 0) {
            gDLL_5_AMSEQ2->vtbl->func1(arg0, arg1->unk3A6, 0, 0, 0);
        }
        if (arg1->unk3A4 != 0) {
            gDLL_5_AMSEQ2->vtbl->func1(arg0, arg1->unk3A4, 0, 0, 0);
        }
    }
    func_80008E08(&arg1->unk374);
    if (arg1->unk3F8 != NULL) {
        mmFree(arg1->unk3F8);
        arg1->unk3F8 = NULL;
    }
}

// offset: 0x2624 | func: 20 | export: 22
void dll_33_func_2624(Object* arg0, DLL33_Data* arg1) {
    s16 sp3C[] = { 
        OBJ_sword, 
        OBJ_staff, 
        OBJ_club, 
        OBJ_IceBall,
        OBJ_fishingnet
    };

    if ((arg1->unk3B9 != arg1->unk3BB) && (arg0->unk_0x36 != 0)) {
        if (arg0->linkedObject != NULL) {
            obj_destroy_object(arg0->linkedObject);
            arg0->linkedObject = NULL;
        }
        if (arg1->unk3B9 > 0) {
            arg0->linkedObject = obj_create(
                obj_alloc_create_info(sizeof(ObjSetup), sp3C[arg1->unk3B9 - 1]), 
                OBJSETUP_FLAG_4, 
                -1, 
                -1, 
                arg0->parent);
        }
        arg1->unk3BB = arg1->unk3B9;
    }
}

// offset: 0x2718 | func: 21 | export: 23
f32 dll_33_func_2718(Object* arg0) {
    f32 var_fv1;
    DLL33_ObjSetup *setup;
    DLL33_Data *objdata;

    objdata = (DLL33_Data*)arg0->data;
    setup = (DLL33_ObjSetup*)arg0->setup;

    if ((setup->unk32 != 0) && (objdata->unk0.unk348 != 0)) {
        var_fv1 = (f32) objdata->unk0.unk348 / (f32) (setup->unk32 * 4);
    } else {
        var_fv1 = 0.0f;
    }
    return var_fv1;
}
