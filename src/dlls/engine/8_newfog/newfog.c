#include "common.h"

// size: 0x318
typedef struct {
    s32 unk0;
    u16 unk4;
    u16 unk6;
    s32 unk8;
    s32 unkC;
    u8 _unk10[0x14 - 0x10];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    u8 _unk4C[0x58 - 0x4C];
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    f32 unk70[33];
    f32 unkF4[33];
    f32 unk178[33];
    f32 unk1FC[22];
    f32 unk254[22];
    f32 unk2AC[22];
    f32 unk304;
    f32 unk308;
    f32 unk30C;
    f32 unk310;
    s8 unk314;
    s8 unk315;
    s8 unk316;
    s8 unk317;
} BSS0;

/*0x0*/ static s8 data_0 = 1;
/*0x4*/ static u8 data_4[] = {
    0x00, 0x00, 0x01, 0x02, 
    0x03, 0x04, 0x05, 0x06, 
    0x07, 0x00, 0x00, 0x00
};

/*0x0*/ static BSS0* bss_0[2];
/*0x8*/ static s8 bss_8;
/*0xC*/ static u32 bss_C;
/*0x10*/ static u8 bss_10[0x60];

static void dll_8_func_1F58(EnvFxAction* action, u8 arg1);
void dll_8_Func_22FC(s32* arg0, s32* arg1, s32* arg2, f32* arg3);

// offset: 0x0 | ctor
void dll_8_ctor(void* dll) {
    D_80092A7C[0] = -1;
    D_80092A7C[1] = -1;
    if (bss_0[0] != NULL) {
        mmFree(bss_0[0]);
    }
    if (bss_0[1] != NULL) {
        mmFree(bss_0[1]);
    }
    bss_0[0] = NULL;
    bss_0[1] = NULL;
}

// offset: 0x94 | dtor
void dll_8_dtor(void* dll) { }

// offset: 0xA0 | func: 0 | export: 0
void dll_8_Func_A0(Object* arg0, Object* arg1, EnvFxAction* arg2, s32 arg3, u16 arg4) {
    u8 sp2F;
    u8 var_v0;
    u8 i;
    PlayerEnvActions* temp_v0;

    sp2F = 0;
    temp_v0 = gDLL_29_Gplay->vtbl->get_current_player_envactions();
    if (arg2 != NULL) {
        D_80092A7C[0] = arg2->unk24 - 1;
        D_80092A7C[1] = arg2->unk24 - 1;
        temp_v0->unk8 = arg2->unk24 - 1;
        if (arg2->unk58 & 0x80) {
            var_v0 = 1;
        } else {
            var_v0 = 0;
        }
        if (bss_0[var_v0]->unk317 == 0) {
            if (arg2->unk58 & 0x40) {
                sp2F = 0x40;
            }
            dll_8_func_1F58(arg2, sp2F);
            if (arg2->unk58 & 0x40) {
                bss_0[var_v0]->unk316 = 1;
            }
            bss_0[var_v0]->unk4 = arg2->unk58 | 0x100;
            bss_0[var_v0]->unk315 = 1;
            bss_0[var_v0]->unk304 = 0.0f;
            return;
        }
        if (arg2->unk58 & 0x20) {
            envfxAction(NULL, NULL, 9, 0);
            return;
        }
        bss_0[var_v0]->unk4 = arg2->unk58 | 0x100;
        bss_0[var_v0]->unk315 = 1;
        bss_0[var_v0]->unk304 = 0.0f;
        for (i = 0; i < 11; i++) {
            bss_0[var_v0]->unkF4[i] = (f32) arg2->unkC[data_4[i]];
            bss_0[var_v0]->unkF4[i + 11] = (f32) arg2->unk14[data_4[i]];
            bss_0[var_v0]->unkF4[i + 22] = (f32) arg2->unk1C[data_4[i]];
            bss_0[var_v0]->unk254[i] = (f32) arg2->unk3E[data_4[i]];
            bss_0[var_v0]->unk254[i + 11] = (f32) arg2->unk2E[data_4[i]];
        }
        bss_0[var_v0]->unk3C = (s32) arg2->unk2A;
        bss_0[var_v0]->unk40 = (s32) arg2->unk2C;
        bss_0[var_v0]->unk314 = -1;
        if (arg2->unk59 & 0x20) {
            if (!(bss_0[var_v0]->unk6 & 0x20)) {
                bss_0[var_v0]->unk6 |= 0x20;
            }
        }
        if (!(arg2->unk59 & 0x20) && (bss_0[var_v0]->unk6 & 0x20)) {
            bss_0[var_v0]->unk6 ^= 0x20;
        }
        if (arg2->unk58 & 0x40) {
            bss_0[var_v0]->unk6 |= 0x40;
            bss_0[var_v0]->unk316 = 1;
        } else if (bss_0[var_v0]->unk6 & 0x40) {
            bss_0[var_v0]->unk6 ^= 0x40;
        }
        if (arg2->unk59 & 0x40) {
            if (!(bss_0[var_v0]->unk6 & 0x40)) {
                bss_0[var_v0]->unk6 |= 0x40;
                return;
            }
        }
        if (!(arg2->unk59 & 0x40)) {
            if (bss_0[var_v0]->unk6 & 0x40) {
                bss_0[var_v0]->unk6 ^= 0x40;
            }
        }
    }
}

// offset: 0x464 | func: 1 | export: 1
void dll_8_Func_464(void) {
    /*0x10*/ static s32 data_10 = 1;
    s32 i;

    D_80092A7C[0] = -1;
    D_80092A7C[1] = -1;
    for (i = 0; i < 2; i++) {
        if (bss_0[i] == NULL) {
            bss_0[i] = mmAlloc(sizeof(BSS0), ALLOC_TAG_SKY_COL, ALLOC_NAME("newfog"));
        }
        bzero(bss_0[i], sizeof(BSS0));
        bss_0[i]->unk24 = 0xFF;
        bss_0[i]->unk28 = 0xFF;
        bss_0[i]->unk2C = 0xFF;
        bss_0[i]->unk14 = 0x47E;
        bss_0[i]->unk18 = 0x4B5;
        bss_0[i]->unk30 = 0xFF;
        bss_0[i]->unk34 = 0xFF;
        bss_0[i]->unk38 = 0xFF;
        bss_0[i]->unk1C = 0x47E;
        bss_0[i]->unk20 = 0x4B5;
        if (data_10 != 0) {
            envfxAction(NULL, NULL, 9, 0);
            data_10 = 0;
        }
    }
}

/*0x14*/ static u32 data_14[] = {
    0x00000000, 0x00000000, 0x3f800000
};
/*0x20*/ static u32 data_20[] = {
    0xc47a0000, 0xc47a0000, 0xc47a0000
};
/*0x2C*/ static u32 data_2C = 0x00000000;
/*0x30*/ static u32 data_30[] = {
    0x01000000, 0x00000000, 0x00000000, 0x00000000
};

// offset: 0x5AC | func: 2 | export: 2
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/8_newfog/dll_8_Func_5AC.s")

// offset: 0x1234 | func: 3 | export: 3
void dll_8_Func_1234(Gfx** gdl) {
    if (bss_0[0] != NULL) {
        bss_8 = 2;
        dlSetFogColor(gdl, bss_0[0]->unk24, bss_0[0]->unk28, bss_0[0]->unk2C, 55);
        if (bss_0[0]->unk14 == bss_0[0]->unk18) {
            bss_0[0]->unk14 -= 20;
        }
        if (bss_0[0]->unk18 < bss_0[0]->unk14) {
            bss_0[0]->unk14 = bss_0[0]->unk18 - 20;
        }
        gSPFogPosition((*gdl)++, bss_0[0]->unk14, bss_0[0]->unk18);
    }
}

// offset: 0x139C | func: 4 | export: 4
s32 dll_8_Func_139C(s32 arg0) {
    return 0;
}

// offset: 0x13AC | func: 5 | export: 5
void dll_8_Func_13AC(Gfx** gdl) {
    s16 var_t0;
    f32 temp;

    if (bss_0[0] != NULL) {
        if ((data_0 == 0) && !(bss_0[0]->unk4 & 1)) {
            if (bss_0[0]->unk14 < 850) {
                var_t0 = 255;
            } else if (bss_0[0]->unk14 > 1010) {
                var_t0 = 0;
            } else {
                temp = (f32) bss_0[0]->unk14;
                var_t0 = (s16) (255.0f - (((temp - 850.0f) / 200.0f) * 255.0f));
            }
            dlSetPrimColor(gdl, bss_0[0]->unk24, bss_0[0]->unk28, bss_0[0]->unk2C, (u8) var_t0);
        } else {
            dlSetPrimColor(gdl, 255, 255, 255, 0);
        }
    }
}

// offset: 0x14B0 | func: 6 | export: 10
void dll_8_Func_14B0(Gfx** gdl) {
    if ((bss_0[0] != NULL) && (bss_8 != 2)) {
        bss_8 = 2;
        dlSetFogColor(gdl, bss_0[0]->unk24, bss_0[0]->unk28, bss_0[0]->unk2C, 255);
        if (bss_0[0]->unk14 == bss_0[0]->unk18) {
            bss_0[0]->unk14--;
        }
        gSPFogPosition((*gdl)++, bss_0[0]->unk14, bss_0[0]->unk18);
    }
}

// offset: 0x1608 | func: 7 | export: 11
void dll_8_Func_1608(Gfx** gdl) {
    if ((bss_0[0] != NULL) && (bss_8 != 1)) {
        bss_8 = 1;
        dlSetFogColor(gdl, bss_0[1]->unk24, bss_0[1]->unk28, bss_0[1]->unk2C, 255);
        if (bss_0[1]->unk14 == bss_0[1]->unk18) {
            bss_0[1]->unk14--;
        }
        gSPFogPosition((*gdl)++, bss_0[1]->unk14, bss_0[1]->unk18);
    }
}

// offset: 0x1764 | func: 8 | export: 6
void dll_8_Func_1764(s32* arg0, s32* arg1, s32* arg2) {
    s32 sp44;
    s32 sp40;
    s32 sp3C;
    s32 sp38;
    s32 sp34;
    s32 sp30;
    f32 sp2C;
    s32 _pad;

    sp2C = 0.0f;
    if ((bss_0[0] != NULL) && ((bss_0[0] == NULL) || (bss_0[0]->unk316 != 0))) {
        sp38 = *arg0;
        sp34 = *arg1;
        sp30 = *arg2;
        dll_8_Func_22FC(&sp44, &sp40, &sp3C, &sp2C);
        *arg0 = (s32) ((f32) sp38 + ((f32) (sp44 - sp38) * (0.25f * sp2C)));
        *arg1 = (s32) ((f32) sp34 + ((f32) (sp40 - sp34) * (0.25f * sp2C)));
        *arg2 = (s32) ((f32) sp30 + ((f32) (sp3C - sp30) * (0.25f * sp2C)));
    }
}

// offset: 0x18C0 | func: 9 | export: 9
void dll_8_Func_18C0(void) {

}

// offset: 0x18C8 | func: 10 | export: 12
s16 dll_8_Func_18C8(void) {
    s16 var_a0;
    f32 temp;

    if (bss_0[0] == NULL) {
        return 0xFF;
    }
    if (bss_0[0]->unk14 < 950) {
        var_a0 = 0;
    } else if (bss_0[0]->unk14 > 1210) {
        var_a0 = 255;
    } else {
        temp = (f32) bss_0[0]->unk14;
        var_a0 = (s16) (((temp - 950.0f) / 200.0f) * 255.0f);
    }
    return var_a0;
}

// offset: 0x1964 | func: 11
void dll_8_func_1964(s32 arg0) {
    f32 var_fv0;
    s32 i;

    i = 0;
    if (bss_0[arg0]->unk304 >= 1.0f) {
        bss_0[arg0]->unk4 &= ~0x100;
        bss_0[arg0]->unk308 = 0.0f;
        bss_0[arg0]->unk304 = 0.0f;
        bss_0[arg0]->unk30C = 1.0f;
        if ((bss_0[arg0]->unk316 != 0) && !(bss_0[arg0]->unk6 & 0x40)) {
            bss_0[arg0]->unk316 = 0;
        }
        for (i = 0; i < 33; i++) {
            bss_0[arg0]->unk70[i] = bss_0[arg0]->unkF4[i];
        }
        for (i = 0; i < 22; i++) {
            bss_0[arg0]->unk1FC[i] = bss_0[arg0]->unk254[i];
        }
        return;
    }
    if (bss_0[arg0]->unk315 != 0) {
        var_fv0 = ((f32) bss_0[arg0]->unk3C / 10.0f) * 60.0f;
        if (var_fv0 == 0.0f) {
            var_fv0 = 1.0f;
        }
        bss_0[arg0]->unk308 = 1.0f / var_fv0;
        for (i = 0; i < 33; i++) {
            bss_0[arg0]->unk178[i] = (bss_0[arg0]->unkF4[i] - bss_0[arg0]->unk70[i]) / var_fv0;
        }
        for (i = 0; i < 22; i++) {
            bss_0[arg0]->unk2AC[i] = (bss_0[arg0]->unk254[i] - bss_0[arg0]->unk1FC[i]) / var_fv0;
        }
        bss_0[arg0]->unk315 = 0;
    }
    for (i = 0; i < 33; i++) {
        bss_0[arg0]->unk70[i] += (bss_0[arg0]->unk178[i] * gUpdateRateF);
    }
    for (i = 0; i < 22; i++) {
        bss_0[arg0]->unk1FC[i] += bss_0[arg0]->unk2AC[i] * gUpdateRateF;
    }
    bss_0[arg0]->unk304 += (bss_0[arg0]->unk308 * gUpdateRateF);
    if ((bss_0[arg0]->unk4 & 1) && (bss_0[arg0]->unk310 > 0.0f)) {
        bss_0[arg0]->unk310 -= (255.0f * bss_0[arg0]->unk304);
        if (bss_0[arg0]->unk310 < 0.0f) {
            bss_0[arg0]->unk310 = 0.0f;
            data_0 = 1;
        }
    } else if ((bss_0[arg0]->unk4 & 4) && (bss_0[arg0]->unk310 < 255.0f)) {
        bss_0[arg0]->unk310 = bss_0[arg0]->unk304 * 255.0f;
        if (bss_0[arg0]->unk310 > 255.0f) {
            bss_0[arg0]->unk310 = 255.0f;
        }
    } else if (!(bss_0[arg0]->unk4 & 1) && (bss_0[arg0]->unk310 < 255.0f)) {
        bss_0[arg0]->unk310 = bss_0[arg0]->unk304 * 255.0f;
        if (bss_0[arg0]->unk310 > 255.0f) {
            bss_0[arg0]->unk310 = 255.0f;
        }
    }
    bss_0[arg0]->unk30C = bss_0[arg0]->unk304;
}

// offset: 0x1F58 | func: 12
static void dll_8_func_1F58(EnvFxAction* action, u8 arg1) {
    s32 i;
    s32 var_v0;
    f32 temp;
    
    if (action->unk58 & 0x80) {
        var_v0 = 1;
    } else {
        var_v0 = 0;
    }
    /* default.dol
    if (bss_0[var_v0] == NULL) {
        STUBBED_PRINTF("warning in newfog dll no spare memory for available\n");
    }
    */
    bss_0[var_v0]->unk0 = 0;
    bss_0[var_v0]->unk317 = 1;
    for (i = 0; i < 33; i++) {
        bss_0[var_v0]->unk178[i] = 0.0f;
    }
    for (i = 0; i < 33; i++) {
        bss_0[var_v0]->unk70[i] = 0.0f;
    }
    for (i = 0; i < 22; i++) {
        bss_0[var_v0]->unk2AC[i] = 0.0f;
    }
    for (i = 0; i < 11; i++) {
        bss_0[var_v0]->unk1FC[i] = 1400.0f;
        bss_0[var_v0]->unk1FC[i + 11] = 1600.0f;
    }
    for (i = 0; i < 11; i++) {
        bss_0[var_v0]->unkF4[i] = (f32) action->unkC[data_4[i]];
        bss_0[var_v0]->unkF4[i + 11] = (f32) action->unk14[data_4[i]];
        bss_0[var_v0]->unkF4[i + 22] = (f32) action->unk1C[data_4[i]];
        bss_0[var_v0]->unk254[i] = (f32) action->unk3E[data_4[i]];
        bss_0[var_v0]->unk254[i + 11] = (f32) action->unk2E[data_4[i]];
    }
    bss_0[var_v0]->unk4 = action->unk58;
    bss_0[var_v0]->unk6 = action->unk59;
    bss_0[var_v0]->unk64 = 0.0f;
    bss_0[var_v0]->unk68 = 0.0f;
    bss_0[var_v0]->unk314 = -1;
    bss_0[var_v0]->unk6C = 0.0f;
    if (action->unk2A == 0) {
        action->unk2A = 1;
    }
    if (action->unk2A != 0) {
        bss_0[var_v0]->unk3C = action->unk2A;
        bss_0[var_v0]->unk48 = 1;
        bss_0[var_v0]->unk8 = action->unk2E[0];
        bss_0[var_v0]->unk5C = 1.0f / (f32) action->unk2A;
    } else {
        bss_0[var_v0]->unk3C = 0;
        bss_0[var_v0]->unk5C = 1.0f;
    }
    if (action->unk2C == 0) {
        action->unk2C = 1;
    }
    if (action->unk2C != 0) {
        bss_0[var_v0]->unk40 = action->unk2C;
        temp = ((f32) action->unk2C / 10.0f) * 60.0f;
        bss_0[var_v0]->unk58 = 255.0f / temp;
        bss_0[var_v0]->unkC = 1500;
        bss_0[var_v0]->unk60 = 1.0f / (f32) action->unk2C;
    } else {
        bss_0[var_v0]->unk40 = 0;
        bss_0[var_v0]->unk60 = 1.0f;
    }
    bss_0[var_v0]->unk44 = 0;
}

// offset: 0x22FC | func: 13 | export: 7
void dll_8_Func_22FC(s32* arg0, s32* arg1, s32* arg2, f32* arg3) {
    if (bss_0[0] != NULL) {
        *arg0 = bss_0[0]->unk24;
        *arg1 = bss_0[0]->unk28;
        *arg2 = bss_0[0]->unk2C;
        *arg3 = bss_0[0]->unk30C;
    }
}

// offset: 0x2350 | func: 14 | export: 8
void dll_8_Func_2350(s32* arg0, s32* arg1) {
    if (bss_0[0] != NULL) {
        *arg0 = bss_0[0]->unk14;
        *arg1 = bss_0[0]->unk18;
    }
}
