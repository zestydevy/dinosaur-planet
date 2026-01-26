#include "dlls/objects/214_animobj.h"
#include "game/gamebits.h"
#include "sys/main.h"
#include "sys/menu.h"
#include "sys/objtype.h"
#include "dll.h"
#include "functions.h"

typedef struct {
    f32 unk0;
    u8 unk4;
    u8 unk5;
    u8 unk6;
} WCLevelControl_Data;

/*0x0*/ static u8 _data_0[8][8] = {
    {0, 0, 0, 0, 0, 0, 8, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 4, 0, 0, 0}, 
    {0, 2, 0, 0, 0, 0, 3, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 7}, 
    {0, 0, 0, 0, 6, 0, 0, 0}, 
    {0, 5, 0, 0, 1, 0, 0, 0}
};
/*0x40*/ static u8 _data_40[8][8] = {
    {0, 0, 0, 0, 0, 0, 4, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 3}, 
    {0, 0, 0, 0, 2, 0, 0, 0}, 
    {0, 1, 0, 0, 0, 0, 0, 0}
};
/*0x80*/ static u8 _data_80[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 1, 5, 0, 0, 2, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 3}, 
    {0, 0, 0, 0, 0, 0, 6, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 7}, 
    {0, 8, 0, 0, 4, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}
};
/*0xC0*/ static u8 _data_C0[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 1, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 2, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 3}, 
    {0, 4, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}
};

/*0x0*/ static u8 _bss_0[8][8];
/*0x40*/ static u8 _bss_40[8][8];

static int dll_779_func_12EC(Object *self, Object *a1, AnimObj_Data *a2, s8 a3);
static void dll_779_func_1444(Object *self, WCLevelControl_Data *objdata);
static void dll_779_func_1928(Object *self, WCLevelControl_Data *objdata);
static void dll_779_func_1CF4(void);
static void dll_779_func_1D54(void);

// offset: 0x0 | ctor
void dll_779_ctor(void *dll) { }

// offset: 0xC | dtor
void dll_779_dtor(void *dll) { }

// offset: 0x18 | func: 0 | export: 0
void dll_779_setup(Object *self, ObjSetup *setup, s32 arg2) {
    WCLevelControl_Data *objdata;

    objdata = self->data;
    self->animCallback = dll_779_func_12EC;
    dll_779_func_1CF4();
    dll_779_func_1D54();
    if (main_get_bits(BIT_7FA) != 0) {
        objdata->unk5 |= 8;
    }
    if (main_get_bits(BIT_7F9) != 0) {
        objdata->unk5 |= 4;
    }
    if (main_get_bits(BIT_813) != 0) {
        objdata->unk5 |= 0x20;
    }
    if (main_get_bits(BIT_812) != 0) {
        objdata->unk5 |= 0x10;
    }
    if (main_get_bits(BIT_2A5) != 0) {
        objdata->unk5 |= 0x40;
    }
    obj_add_object_type(self, OBJTYPE_10);
    main_set_bits(BIT_226, 1);
    main_set_bits(BIT_2A6, 1);
    main_set_bits(BIT_206, 1);
    main_set_bits(BIT_25F, 1);
}

// offset: 0x1B4 | func: 1 | export: 1
void dll_779_control(Object *self) {
    WCLevelControl_Data *objdata;
    f32 sp30;
    u8 act;

    objdata = (WCLevelControl_Data*)self->data;
    if (self->unkDC == 0) {
        func_80000860(self, self, 0x1FB, 0);
        func_80000860(self, self, 0x1FC, 0);
        func_80000860(self, self, 0x149, 0);
        func_80000450(self, self, 0x97, 0, 0, 0);
        func_80000450(self, self, 0x24F, 0, 0, 0);
        self->unkDC = 1;
    }
    act = gDLL_29_Gplay->vtbl->get_map_setup(self->mapID);
    if ((act == 1) || (act != 2)) {
        dll_779_func_1444(self, objdata);
    } else {
        dll_779_func_1928(self, objdata);
    }
    if (gDLL_7_Newday->vtbl->func8(&sp30) != 0) {
        main_set_bits(BIT_7F3, 1);
        main_set_bits(BIT_7F1, 0);
    } else {
        main_set_bits(BIT_7F3, 0);
        main_set_bits(BIT_7F1, 1);
    }
}

// offset: 0x3A0 | func: 2 | export: 2
void dll_779_update(Object *self) { }

// offset: 0x3AC | func: 3 | export: 3
void dll_779_print(Object *self, Gfx **gdl, Mtx **mtxs, Vertex **vtxs, Triangle **pols, s8 visibility) {
    if (visibility != 0) {
        draw_object(self, gdl, mtxs, vtxs, pols, 1.0f);
    }
}

// offset: 0x400 | func: 4 | export: 4
void dll_779_free(Object *self, s32 a1) {
    obj_free_object_type(self, OBJTYPE_10);
}

// offset: 0x440 | func: 5 | export: 5
u32 dll_779_get_model_flags(Object *self) {
    return MODFLAGS_NONE;
}

// offset: 0x450 | func: 6 | export: 6
u32 dll_779_get_data_size(Object *self, u32 a1) {
    return sizeof(WCLevelControl_Data);
}

// offset: 0x464 | func: 7 | export: 7
void dll_779_func_464(SRT *arg0, s16 arg1, s16 arg2, f32 *arg3, f32 *arg4) {
    f32 sp2C;
    f32 sp28;

    func_8004478C(arg0->transl.x, arg0->transl.y, arg0->transl.z, &sp2C, &sp28);
    *arg3 = sp2C + 32.0f + (f32) (arg1 * 48) + 24.0f;
    *arg4 = sp28 + 129.0f + (f32) (arg2 * 48) + 24.0f;
}

// offset: 0x534 | func: 8 | export: 8
void dll_779_func_534(SRT *arg0, f32 arg1, f32 arg2, s16 *arg3, s16 *arg4) {
    f32 sp2C;
    f32 sp28;

    func_8004478C(arg0->transl.x, arg0->transl.y, arg0->transl.z, &sp2C, &sp28);
    *arg3 = (s16) ((s16) (s32) ((arg1 - sp2C) - 32.0f) / 48);
    *arg4 = (s16) ((s16) (s32) ((arg2 - sp28) - 129.0f) / 48);
}

// offset: 0x64C | func: 9 | export: 9
void dll_779_func_64C(s16 arg0, s16 x, s16 y) {
    if ((x >= 0) && (x < 8) && (y >= 0) && (y < 8)) {
        _bss_0[x][y] = (u8) arg0;
    }
}

// offset: 0x6C0 | func: 10 | export: 10
u8 dll_779_func_6C0(s16 x, s16 y) {
    if ((x < 0) || (x >= 8) || (y < 0) || (y >= 8)) {
        return 0;
    }
    return _bss_0[x][y];
}

// offset: 0x730 | func: 11 | export: 11
void dll_779_func_730(s16 arg0, s16 *outX, s16 *outY) {
    s32 x;
    s32 y;

    for (x = 0; x < 8; x++) {
        for (y = 0; y < 8; y++) {
            if (arg0 == _data_0[x][y]) {
                *outX = x;
                *outY = y;
                return;
            }
        }
    }
}

// offset: 0x7FC | func: 12 | export: 12
void dll_779_func_7FC(s16 arg0, s16 *outX, s16 *outY) {
    s32 x;
    s32 y;

    for (x = 0; x < 8; x++) {
        for (y = 0; y < 8; y++) {
            if (arg0 == _data_40[x][y]) {
                *outX = x;
                *outY = y;
                return;
            }
        }
    }
}

// offset: 0x8C8 | func: 13 | export: 13
s32 dll_779_func_8C8(SRT *arg0, s16 arg1, s16 arg2, f32 *arg3, f32 *arg4, s32 arg5, s32 arg6) {
    s32 var_v0;
    s32 var_a1;
    s32 var_a1_2;
    f32 sp38;

    if (arg5 != 0) {
        if (arg5 == -1) {
            dll_779_func_464(arg0, 7, arg2, arg3, arg4);
            arg1 += 1;
            var_a1 = 8;
        } else {
            dll_779_func_464(arg0, 0, arg2, arg3, arg4);
            arg1 -= 1;
            var_a1 = -1;
        }
        var_v0 = arg1;
        while (var_v0 != var_a1) {
            if (_bss_0[var_v0][arg2] != 0) {
                if (_bss_0[var_v0][arg2] < 5) {
                    var_v0 += arg5;
                    dll_779_func_464(arg0, (s16) var_v0, arg2, arg3, &sp38);
                    return 1;
                } else {
                    dll_779_func_464(arg0, var_v0, arg2, arg3, &sp38);
                    return 2;
                }
            }
            var_v0 -= arg5;
        }
    } else {
        if (arg6 == -1) {
            dll_779_func_464(arg0, arg1, 7, arg3, arg4);
            arg2 += 1;
            var_a1_2 = 8;
        } else {
            dll_779_func_464(arg0, arg1, 0, arg3, arg4);
            arg2 -= 1;
            var_a1_2 = -1;
        }
        var_v0 = arg2;
        while (var_v0 != var_a1_2) {
            if (_bss_0[arg1][var_v0] != 0) {
                if (_bss_0[arg1][var_v0] < 5) {
                    var_v0 += arg6;
                    dll_779_func_464(arg0, arg1, (s16) var_v0, &sp38, arg4);
                    return 1;
                } else {
                    dll_779_func_464(arg0, arg1, var_v0, &sp38, arg4);
                    return 2;
                }
            }
            var_v0 -= arg6;
        }
    }
    
    return 4;
}

// offset: 0xBA8 | func: 14 | export: 14
void dll_779_func_BA8(SRT *arg0, s16 arg1, s16 arg2, f32* arg3, f32 *arg4) {
    f32 sp2C;
    f32 sp28;

    func_8004478C(arg0->transl.x, arg0->transl.y, arg0->transl.z, &sp2C, &sp28);
    *arg3 = sp2C + 224.0f + (f32) (arg1 * 48) + 24.0f;
    *arg4 = sp28 + 128.0f + (f32) (arg2 * 48) + 24.0f;
}

// offset: 0xC78 | func: 15 | export: 15
void dll_779_func_C78(SRT *arg0, f32 arg1, f32 arg2, s16 *arg3, s16 *arg4) {
    f32 sp2C;
    f32 sp28;

    func_8004478C(arg0->transl.x, arg0->transl.y, arg0->transl.z, &sp2C, &sp28);
    *arg3 = (s16) ((s16) (s32) ((arg1 - sp2C) - 224.0f) / 48);
    *arg4 = (s16) ((s16) (s32) ((arg2 - sp28) - 128.0f) / 48);
}

// offset: 0xD90 | func: 16 | export: 16
void dll_779_func_D90(s16 arg0, s16 x, s16 y) {
    if ((x >= 0) && (x < 8) && (y >= 0) && (y < 8)) {
        _bss_40[x][y] = (u8) arg0;
    }
}

// offset: 0xE04 | func: 17 | export: 17
u8 dll_779_func_E04(s16 x, s16 y) {
    if ((x < 0) || (x >= 8) || (y < 0) || (y >= 8)) {
        return 0;
    }
    return _bss_40[x][y];
}

// offset: 0xE74 | func: 18 | export: 18
void dll_779_func_E74(s16 arg0, s16 *outX, s16 *outY) {
    s32 x;
    s32 y;

    for (x = 0; x < 8; x++) {
        for (y = 0; y < 8; y++) {
            if (arg0 == _data_80[x][y]) {
                *outX = x;
                *outY = y;
                return;
            }
        }
    }
}

// offset: 0xF40 | func: 19 | export: 19
void dll_779_func_F40(s16 arg0, s16 *outX, s16 *outY) {
    s32 x;
    s32 y;

    for (x = 0; x < 8; x++) {
        for (y = 0; y < 8; y++) {
            if (arg0 == _data_C0[x][y]) {
                *outX = x;
                *outY = y;
                return;
            }
        }
    }
}

// offset: 0x100C | func: 20 | export: 20
s32 dll_779_func_100C(SRT *arg0, s16 arg1, s16 arg2, f32 *arg3, f32 *arg4, s32 arg5, s32 arg6) {
    s32 var_v0;
    s32 var_a1;
    s32 var_a1_2;
    f32 sp38;

    if (arg5 != 0) {
        if (arg5 == -1) {
            dll_779_func_BA8(arg0, 7, arg2, arg3, arg4);
            arg1 += 1;
            var_a1 = 8;
        } else {
            dll_779_func_BA8(arg0, 0, arg2, arg3, arg4);
            arg1 -= 1;
            var_a1 = -1;
        }
        var_v0 = arg1;
        while (var_v0 != var_a1) {
            if (_bss_40[var_v0][arg2] != 0) {
                if (_bss_40[var_v0][arg2] < 5) {
                    var_v0 += arg5;
                    dll_779_func_BA8(arg0, (s16) var_v0, arg2, arg3, &sp38);
                    return 1;
                } else {
                    dll_779_func_BA8(arg0, var_v0, arg2, arg3, &sp38);
                    return 2;
                }
            }
            var_v0 -= arg5;
        }
    } else {
        if (arg6 == -1) {
            dll_779_func_BA8(arg0, arg1, 7, arg3, arg4);
            arg2 += 1;
            var_a1_2 = 8;
        } else {
            dll_779_func_BA8(arg0, arg1, 0, arg3, arg4);
            arg2 -= 1;
            var_a1_2 = -1;
        }
        var_v0 = arg2;
        while (var_v0 != var_a1_2) {
            if (_bss_40[arg1][var_v0] != 0) {
                if (_bss_40[arg1][var_v0] < 5) {
                    var_v0 += arg6;
                    dll_779_func_BA8(arg0, arg1, (s16) var_v0, &sp38, arg4);
                    return 1;
                } else {
                    dll_779_func_BA8(arg0, arg1, var_v0, &sp38, arg4);
                    return 2;
                }
            }
            var_v0 -= arg6;
        }
    }

    return 4;
}

// offset: 0x12EC | func: 21
static int dll_779_func_12EC(Object *self, Object *a1, AnimObj_Data *a2, s8 a3) {
    WCLevelControl_Data* objdata;
    s32 i;
    
    objdata = self->data;
    objdata->unk5 |= 1;
    objdata->unk5 &= ~2;
    if (objdata->unk6 == 1) {
        objdata->unk0 -= gUpdateRateF;
        if (objdata->unk0 <= 0.0f) {
            main_set_bits(BIT_7F7, 1);
        }
    } else if (objdata->unk6 == 2) {
        objdata->unk0 -= gUpdateRateF;
        if (objdata->unk0 <= 0.0f) {
            main_set_bits(BIT_802, 1);
        }
    }
    for (i = 0; i < a2->unk98; i++) {
        switch (a2->unk8E[i]) {
            case 1:
                objdata->unk4 = 6;
                break;
            default:
                break;
        }
    }
    return 0;
}

// offset: 0x1444 | func: 22
static void dll_779_func_1444(Object *self, WCLevelControl_Data *objdata) {
    if (objdata->unk5 & 2) {
        return;
    }

    objdata->unk6 = objdata->unk4;

    switch (objdata->unk4) {
    case 1:
        if (objdata->unk5 & 1) {
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x106, 0, 0, 0);
            func_8000F64C(0x11, 0x3C);
            func_8000F6CC();
        } else if (main_get_bits(BIT_7F9) != 0) {
            objdata->unk5 |= 4;
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x104, 0, 0, 0);
            func_8000FAC8();
            if (main_get_bits(BIT_7FA) != 0) {
                gDLL_3_Animation->vtbl->func17(0, self, -1);
                objdata->unk4 = 3;
            } else {
                gDLL_3_Animation->vtbl->func17(1, self, -1);
                objdata->unk4 = 0;
            }
        } else if (func_8000FB1C() != 0) {
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x104, 0, 0, 0);
            main_set_bits(BIT_7EF, 0);
            main_set_bits(BIT_7ED, 0);
            objdata->unk4 = 0;
        }
        break;
    case 2:
        if (objdata->unk5 & 1) {
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x106, 0, 0, 0);
            func_8000F64C(0x11, 0x3C);
            func_8000F6CC();
        } else if (main_get_bits(BIT_7FA) != 0) {
            objdata->unk5 |= 8;
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x104, 0, 0, 0);
            func_8000FAC8();
            if (main_get_bits(BIT_7F9) != 0) {
                gDLL_3_Animation->vtbl->func17(0, self, -1);
                objdata->unk4 = 3;
            } else {
                gDLL_3_Animation->vtbl->func17(1, self, -1);
                objdata->unk4 = 0;
            }
        } else if (func_8000FB1C() != 0) {
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x104, 0, 0, 0);
            main_set_bits(BIT_7F0, 0);
            main_set_bits(BIT_7EE, 0);
            objdata->unk4 = 0;
        }
        break;
    case 3:
        break;
    default:
        if (!(objdata->unk5 & 4) && (main_get_bits(BIT_7ED) != 0)) {
            main_set_bits(BIT_7EF, 1);
            objdata->unk4 = 1;
            objdata->unk5 |= 2;
            objdata->unk0 = 70.0f;
        } else if (!(objdata->unk5 & 8) && (main_get_bits(BIT_7EE) != 0)) {
            main_set_bits(BIT_7F0, 1);
            objdata->unk4 = 2;
            objdata->unk5 |= 2;
            objdata->unk0 = 70.0f;
        }
        break;
    }

    objdata->unk5 &= ~0x1;
}

// offset: 0x1928 | func: 23
static void dll_779_func_1928(Object *self, WCLevelControl_Data *objdata) {
    u8 sp37;
    u8 temp;
    f32 sp30;

    sp37 = gDLL_7_Newday->vtbl->func8(&sp30);
    switch(objdata->unk4) {
        case 6:
            gDLL_5_AMSEQ2->vtbl->set(NULL, 0x106, 0, 0, 0);
            func_8000F64C(0x11, 0x3C);
            func_8000F6CC();
            objdata->unk4 = 4;
            break;
        case 4:
            if (func_8000FB1C() != 0) {
                gDLL_5_AMSEQ2->vtbl->set(NULL, 0x104, 0, 0, 0);
                if (main_get_bits(BIT_2A5) != 0) {
                    objdata->unk5 |= 0x40;
                } else {
                    main_set_bits(BIT_274, 0);
                    main_set_bits(BIT_2B1, 0);
                    main_set_bits(BIT_226, 1);
                    main_set_bits(BIT_2A6, 1);
                    main_set_bits(BIT_206, 1);
                    main_set_bits(BIT_25F, 1);
                }
                objdata->unk4 = 0;
            }
            break;
        default:
            if (!(objdata->unk5 & 0x40)) {
                if ((main_get_bits(BIT_2B1) != 0) && (main_get_bits(BIT_204) != 0)) {
                    main_set_bits(BIT_226, 0);
                    main_set_bits(BIT_2A6, 0);
                    main_set_bits(BIT_206, 0);
                    main_set_bits(BIT_25F, 0);
                    main_set_bits(BIT_274, 1);
                    objdata->unk4 = 6;
                }
            }
            break;
    }

    if (!(objdata->unk5 & 0x10)) {
        temp = main_get_bits(BIT_810); // get sun block puzzle pieces in correct place
        if (temp == 4) {
            main_set_bits(BIT_812, 1);
            objdata->unk5 |= 0x10;
        } else if ((sp37 != 0) || (main_get_bits(BIT_808) != 0)) {
            dll_779_func_1CF4();
        }
    }
    if (!(objdata->unk5 & 0x20)) {
        temp = main_get_bits(BIT_811); // get moon block puzzle pieces in correct place
        if (temp == 4) {
            main_set_bits(BIT_813, 1);
            objdata->unk5 |= 0x20;
        } else if ((sp37 == 0) || (main_get_bits(BIT_809) != 0)) {
            dll_779_func_1D54();
        }
    }
    objdata->unk5 &= ~0x1;
}

// offset: 0x1CF4 | func: 24
static void dll_779_func_1CF4(void) {
    main_set_bits(BIT_810, 0);
    bcopy(_data_0, _bss_0, sizeof(_bss_0));
}

// offset: 0x1D54 | func: 25
static void dll_779_func_1D54(void) {
    main_set_bits(BIT_811, 0);
    bcopy(_data_80, _bss_40, sizeof(_bss_40));
}

/*0x0*/ static const char str_0[] = "SUNBLOCK: invalid grid position\n";
/*0x24*/ static const char str_24[] = "SUNBLOCK: invalid grid position\n";
/*0x48*/ static const char str_48[] = "SUN GRID PUZZLE: Error cannot find given id\n";
/*0x78*/ static const char str_78[] = "SUN GRID PUZZLE: Error cannot find given id\n";
/*0xA8*/ static const char str_A8[] = "MOONBLOCK: invalid grid position\n";
/*0xCC*/ static const char str_CC[] = "MOONBLOCK: invalid grid position\n";
/*0xF0*/ static const char str_F0[] = "MOON GRID PUZZLE: Error cannot find given id\n";
/*0x120*/ static const char str_120[] = "MOON GRID PUZZLE: Error cannot find given id\n";
