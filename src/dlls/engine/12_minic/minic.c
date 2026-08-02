#include "common.h"

// size: 0x28
typedef struct {
    u8 _unk0[0xC - 0x0];
    s32 unkC;
    u8 _unk10[0x14 - 0x10];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
} Data0_88;

typedef struct {
    s32 unk0;
    s32 unk4;
    void *unk8;
    void *unkC;
    Texture* unk10[16];
    Texture* unk50[8];
    s32 unk70;
    s32 unk74;
    s32 unk78;
    s32 unk7C;
    void *unk80;
    void *unk84;
    Data0_88 *unk88;
    Texture* unk8C;
    Texture* unk90;
    u8 _unk94[0xB8 - 0x94];
    s32 unkB8;
    s32 unkBC;
    f32 unkC0;
    f32 unkC4;
    f32 unkC8;
    f32 unkCC;
    f32 unkD0;
    f32 unkD4;
    f32 unkD8;
    f32 unkDC;
    f32 unkE0;
    f32 unkE4;
    f32 unkE8;
    u8 _unkEC[0xF4 - 0xEC];
    s16 unkF4;
    u16 unkF6;
    u16 unkF8;
    u16 unkFA;
    u16 unkFC;
    u8 _unkFE[0x141 - 0xFE];
    u8 unk141;
    u8 unk142;
    u8 unk143;
    u8 unk144;
    u8 unk145;
    u8 unk146;
    u8 unk147;
    u8 unk148;
    u8 _unk149[0x14C - 0x149];
    void *unk14C;
} Data0;

/*0x0*/ static Data0* data_0 = NULL;
/*0x4*/ static u32 data_4[] = {
    0x011c011d, 0x011e011f, 0x01200121, 0x01220123, 0x01240000
};
/*0x18*/ static s16 data_18[] = {
    0x01ae, 0x010c, 0x0128, 0x0247, 0x0388, 0x0000
};
/*0x24*/ static s16 data_24[] = {
    0x00fb, 0x0103, 0x0139, 0x039a
};
/*0x2C*/ static u8 data_2C = 1;
/*0x30*/ static u32 data_30[] = {
    0x00040000, 0x000300ff, 0x00ff0000, 0x01ff0000, 0x00ff0004, 0x00010000, 0x00ff00ff, 0x00ff01ff, 
    0x000001ff, 0x00040002, 0x000100ff, 0x00ff01ff, 0x01ff00ff, 0x01ff0004, 0x00050002, 0x00ff00ff, 
    0x01ff00ff, 0x01ff01ff, 0x00040003, 0x000600ff, 0x00ff0000, 0x00ff0000, 0x00000004, 0x00060007, 
    0x00ff00ff, 0x00000000, 0x00ff0000, 0x00040007, 0x000800ff, 0x00ff00ff, 0x000001ff, 0x00000004, 
    0x00080005, 0x00ff00ff, 0x01ff0000, 0x01ff00ff
};
/*0xC0*/ static u32 data_C0 = 0x000001ff;
/*0xC4*/ static u32 data_C4[] = {
    0x00ff01ff, 0x01ff01ff
};
/*0xCC*/ static u32 data_CC = 0x000000ff;
/*0xD0*/ static u32 data_D0[] = {
    0x00ff00ff, 0x01ff00ff, 0x00000000, 0x00ff0000, 0x01ff0000
};
/*0xE4*/ static u32 data_E4[] = {
    0x00000001, 0x00020000, 0x00000020, 0x00000020, 0x00200000, 0x00020003, 0x00000000, 0x00200020, 
    0x00000020
};
/*0x108*/ static u32 data_108[] = {
    0x00000000, 0x0c800000, 0x0c800c80, 0x00000c80
};
/*0x118*/ static u8 data_118[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/*0x120*/ static u32 data_120 = 0xffffffff;
/*0x124*/ static u32 data_124[] = {
    0x00140000, 0xffff0014, 0x0001ffff, 0x0014ffff, 0x00000014, 0x00000000, 0x00ff0001, 0x00000014, 
    0xffff0001, 0x00140000, 0x00010014, 0x00010001, 0x00140000
};
/*0x158*/ static u32 data_158[] = {
    0x457a0000, 0xc3520000, 0x00000000, 0x457a0000, 0x44160000, 0x00000000
};
/*0x170*/ static u32 data_170[] = {
    0x457a0000, 0x43960000, 0x00000000, 0x457a0000, 0x44548000, 0x00000000
};
/*0x188*/ static u32 data_188[] = {
    0x0fa0ff2e, 0x00000fa0, 0xff2e0000, 0x0fa00258, 0x00000fa0, 0x02580000
};
/*0x1A0*/ static u32 data_1A0[] = {
    0x0fa0012c, 0x00000fa0, 0x012c0000, 0x0fa00352, 0x00000fa0, 0x03520000
};
/*0x1B8*/ static s32 data_1B8 = 0x00000000;
/*0x1BC*/ static s32 data_1BC = 0x00000000;
/*0x1C0*/ static f32 data_1C0 = 0.0;
/*0x1C4*/ static f32 data_1C4 = 0.0;
/*0x1C8*/ static f32 data_1C8 = 0.0;
/*0x1CC*/ static u32 data_1CC = 0xffffffb0;
/*0x1D0*/ static f32 data_1D0 = 0.0;
/*0x1D4*/ static f32 data_1D4 = 0.0;
/*0x1D8*/ static f32 data_1D8 = 0.0;
/*0x1DC*/ static s32 data_1DC = 0x00000000;
/*0x1E0*/ static u32 data_1E0[] = {
    0x00000002, 0x80200300, 0x20000000, 0x01800002, 0x80200000
};
/*0x1F4*/ static u32 data_1F4[] = {
    0x00008000, 0x80200020
};
/*0x1FC*/ static u32 data_1FC[] = {
    0x00000000, 0x03e00000, 0x03e007e0, 0x000007e0
};
/*0x20C*/ static u32 data_20C[] = {
    0x00000000, 0x03e00000, 0x03e003e0, 0x000003e0
};
/*0x21C*/ static u32 data_21C[] = {
    0x00000000, 0x00000000, 0x447a0000, 0x00000000, 0x00000000
};

static void dll_12_func_3A1C(void);

// offset: 0x0 | ctor
void dll_12_ctor(void* dll) {
    D_80092A84[0] = -1;
    D_80092A84[1] = -1;
}

// offset: 0x28 | dtor
void dll_12_dtor(void* dll) { }

// offset: 0x34 | func: 0 | export: 0
void dll_12_Func_34(Object*,Object*,void*,s32,u16);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/12_minic/dll_12_Func_34.s")

// offset: 0xDA4 | func: 1 | export: 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/12_minic/dll_12_Func_DA4.s")

// offset: 0x1824 | func: 2 | export: 2
void dll_12_Func_1824(void) {
    s32 i;

    if ((data_0 != NULL) && ((data_1B8 != 0) || (data_1BC != 0))) {
        if (data_1B8 != 0) {
            if (data_0->unk141 != 0) {
                if ((s32) data_0->unkC8 >= data_0->unk143) {
                    data_0->unk141 = 0;
                } else {
                    data_0->unkC8 += (data_0->unkD8 * gUpdateRateF);
                    if (data_0->unkC8 > 255.0f) {
                        data_0->unkC8 = 255.0f;
                    }
                }
            } else if (data_0->unk142 != 0) {
                if (data_0->unkC8 <= 0.0f) {
                    data_0->unk142 = 0;
                    data_1B8 = 0;
                    dll_12_func_3A1C();
                    data_0->unk147 = 0;
                    data_0->unk148 = 0;
                    data_0->unkF6 = 0;
                    data_0->unkF8 = 0;
                } else {
                    data_0->unkC8 += (data_0->unkD8 * gUpdateRateF);
                    if (data_0->unkC8 < 0.0f) {
                        data_0->unkC8 = 0.0f;
                    }
                }
            }
        }
        if (data_1BC != 0) {
            if (data_0->unk144 != 0) {
                if (data_0->unkC0 >= 1.0f) {
                    data_0->unk144 = 0;
                } else {
                    data_0->unkC4 = (f32) (data_0->unkC4 + data_0->unkD4);
                    if ((s32) data_0->unkC4 >= (s32) data_0->unk146) {
                        data_0->unkC4 = (f32) data_0->unk146;
                    }
                    data_0->unkCC += data_0->unkDC;
                    if ((s32) data_0->unkCC >= 255) {
                        data_0->unkCC = 255;
                    }
                    data_0->unkC0 += (data_0->unkD0 * gUpdateRateF);
                }
            } else if (data_0->unk145 != 0) {
                if (data_0->unkC4 <= 0.0f) {
                    data_0->unk145 = 0;
                    data_1BC = 0;
                    data_0->unkC0 = 0.0f;
                } else {
                    data_0->unkC4 += (data_0->unkD4 * gUpdateRateF);
                }
                if (data_0->unkC4 < 0.0f) {
                    data_0->unkC4 = 0.0f;
                }
                if (data_0->unkC4 > 255.0f) {
                    data_0->unkC4 = 255.0f;
                }
                data_0->unkCC += (data_0->unkDC * gUpdateRateF);
                if (data_0->unkCC < 0.0f) {
                    data_0->unkCC = 0.0f;
                }
                if (data_0->unkCC > 255.0f) {
                    data_0->unkCC = 255.0f;
                }
            }
        }
        if (data_1B8 != 0) {
            for (i = 0; i < data_0->unkF4; i++) {
                if (data_0->unk88[i].unk20 != 0) {
                    data_0->unk88[i].unk24--;
                    if (data_0->unk88[i].unk24 <= 0) {
                        if (data_118[data_0->unk88[i].unk22] == 0) {
                            data_118[data_0->unk88[i].unk22] = 1;
                            data_0->unk88[i].unk20 = 0;
                            data_0->unk88[i].unk26 = 1;
                        } else {
                            data_0->unk88[i].unk24 = mathRnd(0, (s32) data_1C0 * 1530);
                        }
                    }
                }
            }
            for (i = 0; i < data_0->unkF4; i++) {
                if (data_0->unk88[i].unk20 == 0) {
                    data_0->unk88[i].unkC += (s32)data_0->unkE8;
                    if ((12000 - (s32)data_0->unkE8) < data_0->unk88[i].unkC) {
                        data_0->unk88[i].unkC = -12000;
                        data_0->unk88[i].unk14 = 800.0f;
                        data_0->unk88[i].unk18 = 0.0f;
                        data_0->unk88[i].unk1C = 0.0f;
                    }
                    if ((data_0->unk88[i].unk26 != 0) && (data_0->unk88[i].unkC >= -8400)) {
                        data_118[data_0->unk88[i].unk22] = 0;
                        data_0->unk88[i].unk26 = 0;
                    }
                    if (data_0->unk88[i].unkC < 0) {
                        data_0->unk88[i].unk14 -= data_1C4;
                    } else {
                        data_0->unk88[i].unk18 += data_1C4;
                    }
                    if (data_0->unk88[i].unkC < -3000 && data_0->unk88[i].unk1C <= 255.0f) {
                        data_0->unk88[i].unk1C += data_1C8;
                    } else if (data_0->unk88[i].unkC > 3000 && data_0->unk88[i].unk1C >= 0.0f) {
                        data_0->unk88[i].unk1C -= data_1C8;
                    }
                }
            }
        }
        if (data_1BC != 0) {
            data_0->unkBC += (s32) (data_0->unkE0 * gUpdateRateF);
            data_0->unkB8 += (s32) (data_0->unkE4 * gUpdateRateF);
            if (data_0->unkBC >= 0x3FFE) {
                data_0->unkBC -= 0x3FFE;
            } else if (data_0->unkBC < -0x3FFC) {
                data_0->unkBC += 0x3FFE;
            }
            if (data_0->unkB8 >= 0x3FFE) {
                data_0->unkB8 -= 0x3FFE;
            } else if (data_0->unkB8 < -0x3FFC) {
                data_0->unkB8 += 0x3FFE;
            }
        }
    }
}

// offset: 0x1F70 | func: 3 | export: 3
void dll_12_Func_1F70(Gfx **, Mtx **);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/12_minic/dll_12_Func_1F70.s")

// offset: 0x33C8 | func: 4 | export: 4
void dll_12_Func_33C8(void) {
    s32 i;

    if (data_0 != NULL) {
        if (data_0->unk8C != NULL) {
            texFreeTexture(data_0->unk8C);
            data_0->unk8C = NULL;
        }
        for (i = 0; i < 16; i++) {
            if (data_0->unk10[i] != NULL) {
                texFreeTexture(data_0->unk10[i]);
                data_0->unk10[i] = 0;
            }
        }
        for (i = 0; i < 8; i++) {
            if (data_0->unk50[i] != NULL) {
                texFreeTexture(data_0->unk50[i]);
                data_0->unk50[i] = 0;
            }
        }
        if (data_0->unk8 != NULL) {
            mmFree(data_0->unk8);
            data_0->unk8 = NULL;
        }
        if (data_0->unkC != NULL) {
            mmFree(data_0->unkC);
            data_0->unkC = NULL;
        }
        if (data_0->unk80 != NULL) {
            mmFree(data_0->unk80);
            data_0->unk80 = NULL;
        }
        if (data_0->unk84 != NULL) {
            mmFree(data_0->unk84);
            data_0->unk84 = NULL;
        }
        if (data_0->unk88 != NULL) {
            mmFree(data_0->unk88);
            data_0->unk88 = NULL;
        }
        if (data_0->unk8C != NULL) {
            texFreeTexture(data_0->unk8C);
            data_0->unk8C = NULL;
        }
        if (data_0->unk90 != NULL) {
            texFreeTexture(data_0->unk90);
            data_0->unk90 = NULL;
        }
        if (data_0->unk14C != NULL) {
            mmFree(data_0->unk14C);
            data_0->unk70 = 0;
            data_0->unk74 = 0;
            data_0->unk78 = 0;
            data_0->unk7C = 0;
            data_0->unk0 = 0;
            data_0->unk4 = 0;
        }
        if (data_0 != NULL) {
            mmFree(data_0);
            data_0 = NULL;
        }
        data_1BC = 0;
        data_1B8 = 0;
    }
}

// offset: 0x3658 | func: 5 | export: 6
void dll_12_Func_3658(u8 arg0) {
    PlayerEnvActions* envActions = gDLL_29_Gplay->vtbl->get_current_player_envactions();
    data_2C = arg0;
    if (!data_2C) {
        envActions->unk3C &= ~1;
    } else {
        envActions->unk3C |= 1;
    }
}

// offset: 0x36D4 | func: 6 | export: 5
void dll_12_Func_36D4(f32 arg0, f32 arg1, f32 arg2, s32 arg3) {
    data_1D0 = arg0;
    data_1D4 = arg1;
    data_1D8 = arg2;
    data_1DC = arg3;
}

// offset: 0x3710 | func: 7 | export: 7
void dll_12_Func_3710(u16 arg0) {
    if (data_0 != NULL) {
        data_0->unkFC = arg0;
    }
}

// offset: 0x3744 | func: 8 | export: 8
void dll_12_Func_3744(u16 arg0) {
    if (data_0 != NULL) {
        data_0->unkF6 = arg0;
    }
}

// offset: 0x3778 | func: 9 | export: 9
void dll_12_Func_3778(f32 arg0, f32 arg1) {
    if (arg0 < -50.0f) {
        arg0 = -50.0f;
    } else if (arg0 > 50.0f) {
        arg0 = 50.0f;
    }
    if (data_0 != NULL) {
        data_0->unkE0 = arg0 / 3.0f;
    }
    if (arg1 < -50.0f) {
        arg1 = -50.0f;
    } else if (arg1 > 50.0f) {
        arg1 = 50.0f;
    }
    if (data_0 != NULL) {
        data_0->unkE4 = arg1 / 3.0f;
    }
}

// offset: 0x3844 | func: 10
void dll_12_func_3844(s32 arg0) {
    s32 i;

    if (arg0 > 0) {
        arg0 -= 1;
        if (arg0 >= 5) {
            arg0 = 0;
        }
        if (data_0 != NULL) {
            for (i = 0; i < 16; i++) {
                if (data_0->unk10[i] != NULL) {
                    texFreeTexture(data_0->unk10[i]);
                }
                data_0->unk10[i] = 0;
                data_0->unk10[i] = texLoadTexture(data_18[arg0] + i);
            }
        }
    }
}

// offset: 0x3930 | func: 11
void dll_12_func_3930(s32 arg0) {
    s32 i;

    if (arg0 > 0) {
        arg0 -= 1;
        if (arg0 >= 4) {
            arg0 = 0;
        }
        if (data_0 != NULL) {
            for (i = 0; i < 8; i++) {
                if (data_0->unk50[i] != NULL) {
                    texFreeTexture(data_0->unk50[i]);
                }
                data_0->unk50[i] = 0;
                data_0->unk50[i] = texLoadTexture(data_24[arg0] + i);
            }
        }
    }
}

// offset: 0x3A1C | func: 12
static void dll_12_func_3A1C(void) {
    s32 i;

    if (data_0 != NULL) {
        if (data_0->unk8C != NULL) {
            texFreeTexture(data_0->unk8C);
            data_0->unk8C = NULL;
        }
        for (i = 0; i < 16; i++) {
            if (data_0->unk10[i] != NULL) {
                texFreeTexture(data_0->unk10[i]);
                data_0->unk10[i] = 0;
            }
        }
        for (i = 0; i < 8; i++) {
            if (data_0->unk50[i] != NULL) {
                texFreeTexture(data_0->unk50[i]);
                data_0->unk50[i] = 0;
            }
        }
        if (data_0->unk14C != NULL) {
            mmFree(data_0->unk14C);
            data_0->unk70 = 0;
            data_0->unk74 = 0;
            data_0->unk78 = 0;
            data_0->unk7C = 0;
            data_0->unk0 = 0;
            data_0->unk4 = 0;
        }
        if (data_0->unk8 != NULL) {
            mmFree(data_0->unk8);
            data_0->unk8 = NULL;
        }
        if (data_0->unkC != NULL) {
            mmFree(data_0->unkC);
            data_0->unkC = NULL;
        }
        if (data_0->unk80 != NULL) {
            mmFree(data_0->unk80);
            data_0->unk80 = NULL;
        }
        if (data_0->unk84 != NULL) {
            mmFree(data_0->unk84);
            data_0->unk84 = NULL;
        }
        if (data_0->unk88 != NULL) {
            mmFree(data_0->unk88);
            data_0->unk88 = NULL;
        }
        data_1B8 = 0;
    }
}

// offset: 0x3C38 | func: 13
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/12_minic/dll_12_func_3C38.s")

/*0x0*/ static const char str_0[] = "minic error %d null\n";
