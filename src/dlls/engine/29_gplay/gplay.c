#include "PR/ultratypes.h"

#include "dlls/engine/28_screen_fade.h"
#include "game/objects/object.h"
#include "sys/dll.h"
#include "sys/main.h"
#include "sys/math.h"
#include "sys/fs.h"
#include "sys/menu.h"
#include "sys/memory.h"
#include "variables.h"
#include "functions.h"
#include "dll.h"
#include "prevent_bss_reordering.h"

static const char str1[] = "gplayLoadGame error: savegame and backup failed to load.\n";
static const char str2[] = "gplayLoadOptions error: saveoptions failed to load.\n";
static const char str3[] = " WARNING : Not Enough Memort for Restart Point ";
static const char str4[] = "\nWARNING gplay : Restart Point Not Set \n";
static const char str5[] = "gameplay.c: gplayGetCurrentPlayerLactions: playerno overflow\n";
static const char str6[] = "gameplay.c: gplayGetCurrentPlayerEnvactions: playerno overflow\n";
static const char str7[] = "gplayAddTime: MAX_TIMESAVES reached\n";
static const char str8[] = "Error in gplaySetObjGroupStatus (mapno %d, groupno %d)\n";
static const char str9[] = "CHEAT OUT OF RANGE\n";
static const char str10[] = "CHEAT OUT OF RANGE\n";
static const char str11[] = "CHEAT OUT OF RANGE\n";
static const char str12[] = "CHEAT OUT OF RANGE\n";
static const char str13[] = "CINEMA OUT OF RANGE\n";
static const char str14[] = "CINEMA OUT OF RANGE\n";

static u16 data_0[120] = {
    0x0000, 0x0000, 0x076e, 0x08ec, 0x04fe, 0x00df, 0x00e0, 0x00e1, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7, 0x00e8, 
    0x00e9, 0x00ea, 0x00eb, 0x0492, 0x0000, 0x05d0, 0x0000, 0x00ed, 0x00ed, 0x00ed, 0x00f0, 0x0000, 0x0229, 0x00ee, 0x0000, 0x00ef, 
    0x0000, 0x0000, 0x0000, 0x03ee, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0349, 0x0000, 0x0492, 0x0492, 
    0x0547, 0x0000, 0x05d0, 0x0000, 0x076f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};
static u16 data_F0[120] = {
    0x03e0, 0x03e0, 0x05db, 0x08ed, 0x0500, 0x07ce, 0x0480, 0x0452, 0x0452, 0x047b, 0x04ae, 0x0405, 0x0458, 0x036a, 0x04a6, 0x045a, 
    0x047c, 0x0000, 0x042e, 0x0493, 0x0000, 0x05d1, 0x0000, 0x03ad, 0x03ad, 0x03ad, 0x0517, 0x0373, 0x0443, 0x03b7, 0x0421, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0397, 0x0000, 0x0000, 0x0000, 0x0473, 0x0000, 0x0000, 0x0000, 0x04a3, 0x0000, 0x0000, 0x0493, 0x0493, 
    0x0548, 0x0000, 0x05d1, 0x0601, 0x05dc, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x03e0, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};
static Vec3f data_1E0 = { -14149.352f, -82.0f, -15569.178f };

/*0x0*/    static FlashStruct *bss_0;
/*0x4*/    static GplayStruct3 *bss_4;
/*0x8*/    static GplayStruct4 *bss_8;
/*0x10*/   static GplayStruct7 bss_10;
/*0x183C*/ static s8 bss_183C;
/*0x1840*/ static u8 bss_1840[40];
/*0x1868*/ static u32 bss_1868[120];
/*0x1A48*/ static s8 bss_1A48[2];
/*0x1A4A*/ static u8 _bss_0x1a4a[0x4];
/*0x1A50*/ static s32 bss_1A50[2];

s32 gplay_load_save(s8 idx, u8 startGame);
void gplay_func_6AC();
void gplay_func_958(Vec3f *param1, s16 param2, s32 param3, s32 param4);
void gplay_start_game();
static void gplay_func_D94();
static void gplay_func_1314();
void gplay_func_139C(s32 param1, s32 param2);
void gplay_func_15B8(s32 param1);

void gplay_ctor(DLLFile *self)  {
    bss_0 = (FlashStruct*)malloc(sizeof(FlashStruct), 0xFFFF00FF, NULL);
    gplay_func_1314();
    bss_4 = NULL;
    bss_8 = (GplayStruct4*)malloc(128, 0xFFFF00FF, NULL);
    bss_1A48[0] = -1;
    bss_1A50[0] = -1;
}

void gplay_dtor(DLLFile *self)  {
    free(bss_0);
    if (bss_4 != NULL) {
        free(bss_4);
    }
    free(bss_8);
}

void gplay_erase_save(s8 idx) {
    gplay_func_1314();
    bss_183C = idx;
    bss_10.unk0.unk0.unk0.unk0x2f6 = 1;
    bss_10.unk0.unk0.unk0.unk0x2fc = -1.0f;
    gplay_func_6AC();
    gplay_func_6AC();
}

// regalloc
#ifndef NON_MATCHING
void gplay_init_save(s8 idx, char *filename);
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/engine/29_gplay/gplay_init_save.s")
#else
static const Vec3f rodata_230 = { 43000.0f, 0.0f, 0.0f };
void gplay_init_save(s8 idx, char *filename) {
    Vec3f vec;
    s32 i;
    u8 *dst;
    u8 var;

    vec = data_1E0;

    gplay_func_1314();

    bss_183C = idx;
    bss_10.unk0.unk0.unk0.character = 1;

    for (i = 0; i < 2; i++) {
        bss_10.unk0.unk0.unk0.unk0x0[i].hp = 12;
        bss_10.unk0.unk0.unk0.unk0x0[i].hpMax = 12;
        bss_10.unk0.unk0.unk0.unk0x0[i].manaMax = 25;
        bss_10.unk0.unk0.unk0.unk0x0[i].mana = 0;

        bss_10.unk0.unk0.unk0.unk0x18[i].unk0x0 = 4;
        bss_10.unk0.unk0.unk0.unk0x18[i].unk0x1 = 5;

        bss_10.unk0.unk0x16F4[i].unk0x0 = -1;
        bss_10.unk0.unk0x16F4[i].unk0x2 = -1;
        bss_10.unk0.unk0x16F4[i].unk0x4 = -1;
        bss_10.unk0.unk0x16F4[i].unk0x6 = -1;
        bss_10.unk0.unk0x16F4[i].unk0x8 = -1;
        bss_10.unk0.unk0x16F4[i].unk0xa = -1;
        bss_10.unk0.unk0x16F4[i].unk0xc = -1;
        bss_10.unk0.unk0x16F4[i].unk0xe = -1;
        bss_10.unk0.unk0x16F4[i].unk0x12 = -1;

        //bss_10.unk0.unk0x171C[i].unk0x0 = 43000.0F;
        bss_10.unk0.unk0x171C[i].unk0x0 = rodata_230.x;
        bss_10.unk0.unk0x171C[i].unk0x4 = -1;
        bss_10.unk0.unk0x171C[i].unk0x6 = -1;
        bss_10.unk0.unk0x171C[i].unk0x8 = -1;
        bss_10.unk0.unk0x171C[i].unk0xa = -1;
        bss_10.unk0.unk0x171C[i].unk0xc = -1;
        bss_10.unk0.unk0x171C[i].unk0xe = -1;
        bss_10.unk0.unk0x171C[i].unk0x3d = -1;
        bss_10.unk0.unk0x171C[i].unk0x3e = -1;
        bss_10.unk0.unk0x171C[i].unk0x3f = -1;
        bss_10.unk0.unk0x171C[i].unk0x3c = 1;

        bss_10.unk0.unk0x179c[i].unk0x6 = -1;
        bss_10.unk0.unk0x179c[i].unk0x4 = -1;
        bss_10.unk0.unk0x179c[i].unk0x2 = -1;
        bss_10.unk0.unk0x179c[i].unk0x0 = -1;
    }

    for (i = 0; i < 120; i++) {
        if (data_0[i] != 0) {
            gDLL_29_gplay->exports->func_139C(i, 1);
        }
    }

    set_gplay_bitstring(0x4E5, 1);

    if (filename != NULL) {
        dst = &bss_10.unk0.unk0.unk0.saveFilename[0];

        var = *filename;
        *dst = var;
        dst++;
        filename++;

        while (var != NULL) {
            var = *filename;
            *dst = var;
            dst++;
            filename++;
        }
    }

    gplay_func_958(&vec, 0, 0, 0);
    gplay_func_6AC();
    gplay_func_6AC();
}
#endif

s32 gplay_load_save(s8 idx, u8 startGame) {
    u8 var1;
    u8 var2;
    FlashStruct *flashPtr;
    s32 ret;

    if (idx != bss_183C) {
        if (idx < 4) {
            bss_183C = idx;

            flashPtr = (FlashStruct*)malloc(sizeof(FlashStruct), 0xFFFF00FF, NULL);

            var1 = gDLL_31_flash->exports->load_game(bss_0, idx, sizeof(FlashStruct), 1);
            var2 = gDLL_31_flash->exports->load_game(flashPtr, idx + 4, sizeof(FlashStruct), 1);

            if (var1 == 0) {
                if (var2 == 0) {
                    free(flashPtr);
                    ret = 0;
                } else {
                    free(bss_0);
                    ret = 2;
                    bss_0 = flashPtr;
                }
            } else if (var2 == 0) {
                free(flashPtr);
                ret = 2;
            } else if (flashPtr->gplay.unk0.unk0.unk0x2fc < bss_0->gplay.unk0.unk0.unk0x2fc) {
                free(flashPtr);
                ret = 1;
            } else {
                free(bss_0);
                ret = 2;
                bss_0 = flashPtr;
            }
        } else {
            queue_load_file_region_to_ptr(
                (void**)bss_0,
                SAVEGAME_BIN,
                idx * sizeof(FlashStruct) - 0x6000,
                sizeof(FlashStruct));
            ret = 1;
        }
    } else {
        ret = 1;
    }

    if (startGame) {
        gplay_start_game();
    } else {
        bcopy(bss_0, &bss_10.unk0, 0x17ac);
    }

    return ret;
}

void gplay_copy_save(s8 srcIdx, s8 dstIdx) {
    gplay_load_save(srcIdx, /*startGame*/FALSE);
    bss_183C = dstIdx;
    gplay_func_6AC();
    gplay_func_6AC();
}

void gplay_func_6AC() {
    if (bss_183C != -1) {
        bss_10.unk0.unk0.unk0.unk0x2f8 += 1;

        bcopy(&bss_10.unk0.unk0.unk0, &bss_0->gplay.unk0.unk0, sizeof(GplayStruct8));

        gDLL_31_flash->exports->save_game(
            bss_0, 
            bss_183C + (bss_0->gplay.unk0.unk0.unk0x2f8 % 2) * 4, 
            sizeof(FlashStruct), 
            1);
        
        if (bss_4 != NULL) {
            bcopy(&bss_10.unk0.unk0, &bss_4->unk0, sizeof(GplayStruct9));
        }
    }
}

u32 gplay_func_79C() {
    u32 var;
    s32 ret;
    
    var = 1;

    ret = gDLL_31_flash->exports->load_game(
        bss_8, 3, sizeof(GplayStruct4), 0);
    
    if (ret == 0) {
        bzero(bss_8, sizeof(GplayStruct4));
        var = 0;
        bss_8->unk0x8 = 0x7f;
        bss_8->unk0x9 = 0x7f;
        bss_8->unk0xa = 0x7f;
    }

    bss_8->unk0x0 = 0;
    
    if (bss_8->unk0xb < -7) {
        bss_8->unk0xb = -7;
    }
    if (bss_8->unk0xb > 7) {
        bss_8->unk0xb = 7;
    }
    if (bss_8->unk0xc < -7) {
        bss_8->unk0xc = -7;
    }
    if (bss_8->unk0xc > 7) {
        bss_8->unk0xc = 7;
    }

    return var;
}

void gplay_func_8D8() {
    gDLL_31_flash->exports->save_game(
        bss_8, 3, sizeof(GplayStruct4), 0);
}

GplayStruct4 *gplay_func_930() {
    return bss_8;
}

void gplay_func_94C(s32 param1) {

}

void gplay_func_958(Vec3f *param1, s16 param2, s32 param3, s32 param4) {
    if ((param3 & 1) != 0) {
        bcopy(&bss_10.unk0.unk0, &bss_0->gplay.unk0, sizeof(GplayStruct9));
    } else {
        if (func_8001EBE0() != 0) {
            bss_10.unk0.unk0x16F4[bss_10.unk0.unk0.unk0.character].unk0x10 |= 1;
        } else {
            bss_10.unk0.unk0x16F4[bss_10.unk0.unk0.unk0.character].unk0x10 &= ~1;
        }

        bss_10.unk0.unk0x16d4[bss_10.unk0.unk0.unk0.character].vec.x = param1->x;
        bss_10.unk0.unk0x16d4[bss_10.unk0.unk0.unk0.character].vec.y = param1->y;
        bss_10.unk0.unk0x16d4[bss_10.unk0.unk0.unk0.character].vec.z = param1->z;
        bss_10.unk0.unk0x16d4[bss_10.unk0.unk0.unk0.character].unk0xC = param2 >> 8;
        bss_10.unk0.unk0x16d4[bss_10.unk0.unk0.unk0.character].unk0xD = param4;

        bcopy(&bss_10.unk0, &bss_0->gplay, sizeof(GplayStruct3));
    }

    if (bss_4 != NULL) {
        bcopy(&bss_10.unk0.unk0, &bss_4->unk0, sizeof(GplayStruct9));
    }
}

void gplay_start_game() {
    bcopy(&bss_0->gplay, &bss_10.unk0, sizeof(GplayStruct3));
    gplay_func_D94();
}

void gplay_func_B3C(Vec3f *param1, s16 param2, s32 param3) {
    if (bss_4 == NULL) {
        bss_4 = (GplayStruct3*)malloc(sizeof(GplayStruct3), 0xFFFF00FF, NULL);
        if (bss_4 == NULL) {
            return;
        }
    }

    bcopy(&bss_10.unk0, bss_4, sizeof(GplayStruct3));

    if (func_8001EBE0() != 0) {
        bss_4->unk0x16F4[bss_4->unk0.unk0.character].unk0x10 |= 1;
    } else {
        bss_4->unk0x16F4[bss_4->unk0.unk0.character].unk0x10 &= ~1;
    }

    bss_4->unk0x16d4[bss_4->unk0.unk0.character].vec.x = param1->x;
    bss_4->unk0x16d4[bss_4->unk0.unk0.character].vec.y = param1->y;
    bss_4->unk0x16d4[bss_4->unk0.unk0.character].vec.z = param1->z;
    bss_4->unk0x16d4[bss_4->unk0.unk0.character].unk0xC = (u8)(param2 >> 8);
    bss_4->unk0x16d4[bss_10.unk0.unk0.unk0.character].unk0xD = param3;
}

void gplay_func_CBC() {
    if (bss_4 != NULL) {
        bcopy(bss_4, &bss_10.unk0, sizeof(GplayStruct3));
        gplay_func_D94();
    }
}

void gplay_func_D20() {
    if (bss_4 != NULL) {
        free(bss_4);
        bss_4 = NULL;
    }
}

s32 gplay_func_D70() {
    return bss_4 != NULL;
}

static void gplay_func_D94() {
    bss_1A48[0] = -1;
    bss_1A50[0] = -1;

    bzero(&bss_10.bitString, 128);

    unpause();

    func_800142A0(
        bss_10.unk0.unk0x16d4[bss_10.unk0.unk0.unk0.character].vec.x,
        bss_10.unk0.unk0x16d4[bss_10.unk0.unk0.unk0.character].vec.y,
        bss_10.unk0.unk0x16d4[bss_10.unk0.unk0.unk0.character].vec.z);
    
    if (menu_get_current() != MENU_4) {
        menu_set(MENU_1);
    }

    gDLL_28_screen_fade->exports->fade_reversed(40, SCREEN_FADE_BLACK);
}

GplayStruct7 *gplay_func_E74() {
    return &bss_10;
}

u8 gplay_func_E90() {
    return bss_10.unk0.unk0.unk0.character;
}

void gplay_func_EAC(u8 character) {
    bss_10.unk0.unk0.unk0.character = character;
}

GplayStruct10 *gplay_func_ED4() {
    return &bss_10.unk0.unk0.unk0.unk0x0[bss_10.unk0.unk0.unk0.character];
}

GplayStruct5 *gplay_func_F04() {
    return &bss_10.unk0.unk0x16d4[bss_10.unk0.unk0.unk0.character];
}

GplayStruct11 *gplay_func_F30() {
    return &bss_10.unk0.unk0.unk0.unk0x18[bss_10.unk0.unk0.unk0.character];
}

GplayStruct6 *gplay_func_F60() {
    if (bss_10.unk0.unk0.unk0.character > 1) {
        return &bss_10.unk0.unk0x16F4[0];
    }

    return &bss_10.unk0.unk0x16F4[bss_10.unk0.unk0.unk0.character];
}

GplayStruct12 *gplay_func_FA8() {
    if (bss_10.unk0.unk0.unk0.character > 1) {
        return &bss_10.unk0.unk0x171C[0];
    }

    return &bss_10.unk0.unk0x171C[bss_10.unk0.unk0.unk0.character];
}

GplayStruct13 *gplay_func_FE8() {
    return &bss_10.unk0.unk0x179c[bss_10.unk0.unk0.unk0.character];
}

void gplay_func_1014(u32 param1, f32 param2) {
    s32 i;

    if (bss_10.unk0.unk0.unk0.unk0x302 == 256) {
        return;
    }

    param2 *= 20.0f;
    param2 += bss_10.unk0.unk0.unk0.unk0x2fc;

    for (i = 0; i < bss_10.unk0.unk0.unk0.unk0x302; i++) {
        if (param1 == bss_10.unk0.unk0.unk0.unk0xad4[i].unk0x0) {
            break;
        }
    }

    if (i == bss_10.unk0.unk0.unk0.unk0x302) {
        bss_10.unk0.unk0.unk0.unk0x302 += 1;
    }

    bss_10.unk0.unk0.unk0.unk0xad4[i].unk0x0 = param1;
    bss_10.unk0.unk0.unk0.unk0xad4[i].unk0x4 = param2;      
}

s32 gplay_func_109C(u32 param1) {
    s32 i;

    for (i = 0; i < bss_10.unk0.unk0.unk0.unk0x302; i++) {
        if (param1 == bss_10.unk0.unk0.unk0.unk0xad4[i].unk0x0) {
            return 0;
        }
    }

    return 1;
}

f32 gplay_func_10F4(u32 param1) {
    s32 i;

    for (i = 0; i < bss_10.unk0.unk0.unk0.unk0x302; i++) {
        if (param1 == bss_10.unk0.unk0.unk0.unk0xad4[i].unk0x0) {
            return bss_10.unk0.unk0.unk0.unk0xad4[i].unk0x4 - bss_10.unk0.unk0.unk0.unk0x2fc;
        }
    }

    return 0.0f;
}

void gplay_func_115C() {
    s32 var1;

    bss_10.unk0.unk0.unk0.unk0x2fc += delayFloat;

    var1 = 0;

    while (bss_10.unk0.unk0.unk0.unk0x302 > var1) {
        if (bss_10.unk0.unk0.unk0.unk0xad4[var1].unk0x4 < bss_10.unk0.unk0.unk0.unk0x2fc) {
            bss_10.unk0.unk0.unk0.unk0x302 = bss_10.unk0.unk0.unk0.unk0x302 - 1;

            bss_10.unk0.unk0.unk0.unk0xad4[var1].unk0x0 = bss_10.unk0.unk0.unk0.unk0xad4[bss_10.unk0.unk0.unk0.unk0x302].unk0x0;
            bss_10.unk0.unk0.unk0.unk0xad4[var1].unk0x4 = bss_10.unk0.unk0.unk0.unk0xad4[bss_10.unk0.unk0.unk0.unk0x302].unk0x4;
        } else {
            var1++;
        }
    }
}

s16 gplay_func_121C() {
    return bss_10.unk0.unk0.unk0.unk0x300;
}

void gplay_func_1238(s32 param1) {
    bss_10.unk0.unk0.unk0.unk0x300 = param1;
}

void *gplay_func_1254() {
    return &bss_10.unk0.unk0.unk0.unk0x304;
}

u32 gplay_func_1270() {
    return (u32)bss_10.unk0.unk0.unk0.unk0x2fc;
}

static void gplay_func_1314() {
    bss_183C = -1;
    bzero(&bss_10, sizeof(GplayStruct7));
    bzero(bss_0, sizeof(FlashStruct));
}

void gplay_func_1378(s32 param1, s32 param2) {
    bss_1840[param2 - 80] = param1;
}

void gplay_func_139C(s32 param1, s32 param2) {
    if (param1 >= 80) {
        param1 = bss_1840[param1 - 80];
    }

    set_gplay_bitstring(data_0[param1], param2);

    bss_1A48[0] = param1;
    bss_1A48[1] = param2;

    gplay_func_15B8(param1);
}

u8 gplay_func_143C(s32 param1) {
    if (param1 >= 80) {
        param1 = bss_1840[param1 - 80];
    }

    if (param1 != bss_1A48[0]) {
        bss_1A48[0] = param1;

        if (param1 < 0 || param1 >= 120 || !data_0[param1]) {
            bss_1A48[1] = 0;
        } else {
            bss_1A48[1] = get_gplay_bitstring(data_0[param1]);
        }
    }

    return bss_1A48[1];
}

u8 gplay_func_14F0(s32 param1, s32 param2) {
    if (param1 >= 80) {
        param1 = bss_1840[param1 - 80];
    }

    if (param1 != bss_1A50[0]) {
        bss_1A50[0] = param1;
        bss_1A50[1] = get_gplay_bitstring(data_F0[param1]);
    }

    return (bss_1A50[1] >> param2) & 1;
}

u16 gplay_func_1590(s32 param1) {
    return data_F0[param1];
}

void gplay_func_15B8(s32 param1) {
    if (param1 >= 80) {
        param1 = bss_1840[param1 - 80];
    }

    bss_1868[param1] = get_gplay_bitstring(data_F0[param1]);
}

u32 gplay_func_163C(s32 param1) {
    if (param1 >= 80) {
        param1 = bss_1840[param1 - 80];
    }
    
    return bss_1868[param1];
}

void gplay_func_1680(s32 param1) {
    if (param1 >= 80) {
        param1 = bss_1840[param1 - 80];
    }
    
    bss_1868[param1] = 0;
}

void gplay_func_16C4(s32 param1, s32 param2, s32 param3) {
    s32 var2;
    s32 bit;
    s32 i;

    if (param1 >= 80) {
        param1 = bss_1840[param1 - 80];
    }

    if ((param1 < 120 && data_F0[param1] != 0) || (param3 >= 0 && 0)) {
        if (param3 == -1) {
            param3 = 1;
        }

        if (param3 == -2) {
            param3 = 0;
        }

        bit = get_gplay_bitstring(data_F0[param1]);

        var2 = bit;
        if (param3 != 0) {
            var2 |= (1 << param2);
        } else {
            var2 &= ~(1 << param2);
        }

        set_gplay_bitstring(data_F0[param1], var2);

        bss_1A50[0] = param1;
        bss_1A50[1] = var2;

        if (param3 != 0) {
            if (((1 << param2) & bit) == 0) {
                for (i = 0; i < 120; i++) {
                    if (data_F0[i] == data_F0[param1]) {
                        bss_1868[i] = bss_1868[i] | (1 << param2);
                    }
                }
            }
        } else {
            for (i = 0; i < 120; i++) {
                if (data_F0[i] == data_F0[param1]) {
                    bss_1868[i] = bss_1868[i] & ~(1 << param2);
                }
            }
        }
    }
}

GplayStruct14 *gplay_func_1974() {
    return &bss_10.unk0.unk0.unk0.unk0x20[bss_10.unk0.unk0.unk0.character];
}

GplayStruct14 *gplay_func_19B8() {
    return &bss_10.unk0.unk0.unk0.unk0x188[bss_10.unk0.unk0.unk0.character];
}

u32 gplay_func_19FC(u8 param1) {
    if (param1 >= 32) {
        return 0;
    }

    return bss_8->unk0x10 & (1 << param1);
}

void gplay_func_1A48(u8 param1) {
    if (param1 < 32) {
        bss_8->unk0x10 |= (1 << param1);
    }
}

s32 gplay_func_1A90(u8 param1) {
    s32 ret;

    ret = 0;

    if (param1 >= 32) {
        return 0;
    } 

    ret = (bss_8->unk0x10 & (1 << param1)) != 0;
    if (ret) {
        return (bss_8->unk0x14 & (1 << param1)) != 0;
    }

    // hmm...
}

void gplay_func_1AF8(u8 param1, u8 param2) {
    if (param1 < 32 && (bss_8->unk0x10 & (1 << param1)) != 0) {
        if (param2 != 0) {
            bss_8->unk0x14 |= (1 << param1);
        } else {
            bss_8->unk0x14 &= ~(1 << param1);
        }
    }
}

u32 gplay_func_1B78(u8 param1) {
    if (param1 >= 32) {
        return 0;
    }

    return bss_8->unk0x18 & (1 << param1);
}

void gplay_func_1BC4(u8 param1) {
    if (param1 < 32) {
        bss_8->unk0x18 |= (1 << param1);
    }
}
