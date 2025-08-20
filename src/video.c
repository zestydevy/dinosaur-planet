#include "common.h"
#include "sys/controller.h"
#include "sys/gfx/gx.h"
#include "sys/main.h"
#include "sys/map.h"
#include "sys/newshadows.h"
#include "PR/gbi.h"

// TODO: This is part of a larger data structure and is only here to allow the current .data section
// for this file to be aligned correctly!
u8 D_80092ff0[] = { 0x07, 0xb1, 0x04, 0x03, 0x07, 0xb1, 0x00, 0x00 };

UnkHeapVidStruct *D_800BCC10;
UnkVidStruct2 D_800BCC18[4]; // size:0x50

extern OSIoMesg D_800BCC90;
extern OSDevMgr __osViDevMgr;
extern s8 D_80093064;

void func_8005D9D8();
void set_video_mode(s32 mode);
void initialize_framebuffers(int someBool, s32 width, s32 height);
void swap_framebuffer_pointers();
void set_custom_vi_mode();
void func_8005DEE8();

void func_8005C780() {
    s32 i;

    if (D_800BCC10 != NULL) {
        free(D_800BCC10);
    }

    D_800BCC10 = malloc(UNKNOWN_HEAP_VIDEO_STRUCT_SIZE, 0x13, 0);

    for (i = 0; i < UNKNOWN_VIDEO_STRUCTS_COUNT; i++)
    {
        gSPEndDisplayList(&gUnknownVideoStructs[i].dl);
        gUnknownVideoStructs[i].viMode = NULL;
        gUnknownVideoStructs[i].unk0x88 = 0;
    }

    for (i = 0; i < 4; i++)
    {
        D_800BCC18[i].unk0x10 = NULL;
        D_800BCC18[i].unk0x0 = 0;
        D_800BCC18[i].unk0x4 = 0;
        D_800BCC18[i].unk0x8 = 0;
        D_800BCC18[i].unk0xc = 0;
    }

    D_800BCC10->unk0x0 = -3.0f;
    D_800BCC10->unk0x4 = 0.0f;
    D_800BCC10->unk0x8 = -3.0f;
    D_800BCC10->unk0xc = -3.0f;
    D_800BCC10->unk0x10 = 0.0f;
    D_800BCC10->unk0x14 = 3.0f;
    
    D_800BCC10->unk0x18 = 3.0f;
    D_800BCC10->unk0x1c = 0.0f;
    D_800BCC10->unk0x20 = 3.0f;
    D_800BCC10->unk0x24 = 3.0f;
    D_800BCC10->unk0x28 = 0.0f;
    D_800BCC10->unk0x2c = -3.0f;
    
    D_800BCC10->unk0x30 = -3.0f;
    D_800BCC10->unk0x34 = 0.0f;
    D_800BCC10->unk0x38 = -3.0f;
    D_800BCC10->unk0x3c = -3.0f;
    D_800BCC10->unk0x40 = 0.0f;
    D_800BCC10->unk0x44 = 3.0f;
    
    D_800BCC10->unk0x48 = 3.0f;
    D_800BCC10->unk0x4c = 0.0f;
    D_800BCC10->unk0x50 = 3.0f;
    D_800BCC10->unk0x54 = 3.0f;
    D_800BCC10->unk0x58 = 0.0f;
    D_800BCC10->unk0x5c = -3.0f;

    D_800BCC68 = queue_load_texture_proxy(0x18);
    D_800BCC6C = queue_load_texture_proxy(0x19);
    D_800BCC70 = queue_load_texture_proxy(0x1a);

    D_800BCC78 = (u8)0;
}

void func_8005C998() {
    s32 i;

    for (i = 0; i < UNKNOWN_VIDEO_STRUCTS_COUNT; i++) {
        gSPEndDisplayList(&gUnknownVideoStructs[i].dl);
        gUnknownVideoStructs[i].viMode = NULL;
        gUnknownVideoStructs[i].unk0x88 = 0;
    }

    for (i = 0; i < 4; i++) {
        D_800BCC18[i].unk0x10 = 0;
        D_800BCC18[i].unk0x0 = 0.0f;
        D_800BCC18[i].unk0x4 = 0.0f;
        D_800BCC18[i].unk0x8 = 0.0f;
        D_800BCC18[i].unk0xc = 0;
    }
}

void func_8005CA5C(u32 param1) {
    D_800BCC78 = param1;

    if (param1 == 0) {
        func_8005C998();
    }
}

void func_8005CA88(f32 *a0, f32 *a1, u8 a2) {
    static s32 D_80092FF8 = 0;

    if (D_800BCC78 != 0) {
        D_800BCC18[D_80092FF8].unk0x10 = a0;
        D_800BCC18[D_80092FF8].unk0x0 = a1[0];
        D_800BCC18[D_80092FF8].unk0x4 = a0[4] + a1[1];
        D_800BCC18[D_80092FF8].unk0x8 = a1[2];
        D_800BCC18[D_80092FF8].unk0xc = a2;

        D_80092FF8 = D_80092FF8 + 1;

        if (D_80092FF8 == 4) {
            D_80092FF8 = 0;
        }
    }
}

void func_8005CB10(Gfx **gdl, Object *obj) {
    s32 i;
    Texture *tex;

    switch (obj->id) {
        case OBJ_Sabre:
            tex = D_800BCC68;
            break;
        case OBJ_Krystal:
            tex = D_800BCC6C;
            break;
        default:
            tex = D_800BCC68;
            break;
    }

    set_textures_on_gdl(gdl, tex, NULL, 0x1a, 0, 0, 1);

    i = 0;

    while (i < UNKNOWN_VIDEO_STRUCTS_COUNT) {
        if (obj == gUnknownVideoStructs[i].obj) {
            gSPDisplayList((*gdl)++, &gUnknownVideoStructs[i].dl);
        }

        i++;
    }
}

static s32 D_80092FFC = 1;
static s32 D_80093000 = 0;
void func_8005CF4C(Object *, UnkVidStruct2*);
void func_8005CDFC(s32 _);
void func_8005CC74(Gfx **gdl, Object *arg1) {
    s32 i;

    if (D_800BCC78 == 0) {
        return;
    }

    if (D_80092FFC != 0) {
        D_800BCC80[0] = 0.0f;
        D_800BCC80[1] = 0.0f;
    }

    if ((gWorldX != D_800BCC80[0]) || (gWorldZ != D_800BCC80[1])) {
        D_800BCC80[0] = gWorldX;
        D_800BCC80[1] = gWorldZ;
        func_8005CDFC(0);
    }

    for (i = 0; i < 4; i++) {
        if ((D_800BCC18[i].unk0x10 != 0) && (arg1 == D_800BCC18[i].unk0x10)) {
            func_8005CF4C(arg1, &D_800BCC18[i]);
        }
    }
    func_8005CB10(gdl, arg1);
    D_80092FFC = 0;
}

void func_8005CD80() {
    s32 i;
    s32 k;
    Vtx_t *ptr;

    for (i = 0; i < UNKNOWN_VIDEO_STRUCTS_COUNT; i++) {
        ptr = &gUnknownVideoStructs[i].unk0x18[0];
        
        if (gUnknownVideoStructs[i].viMode != NULL) {
            gUnknownVideoStructs[i].unk0x88 -= 5;

            if (gUnknownVideoStructs[i].unk0x88 < 0) {
                gUnknownVideoStructs[i].unk0x88 = 0;
            }

            for (k = 0; k < 4; k++) {
                (ptr++)->cn[3] = gUnknownVideoStructs[i].unk0x88;
            }
        }
    }
}

void func_8005CDFC(s32 _) {
    float var1;
    float var2;
    s32 i;
    s32 k;
    Vtx_t *ptr;

    var1 = 0;
    var2 = 0;

    for (i = 0; i < UNKNOWN_VIDEO_STRUCTS_COUNT; i++) {
        if (gUnknownVideoStructs[i].viMode != NULL) {
            ptr = &gUnknownVideoStructs[i].unk0x18[0];
            
            var1 = gWorldX - gUnknownVideoStructs[i].unk0x78;
            var2 = gWorldZ - gUnknownVideoStructs[i].unk0x7c;

            gUnknownVideoStructs[i].unk0x78 += var1;
            gUnknownVideoStructs[i].unk0x7c += var2;

            for (k = 0; k < 4; k++) {
                ptr->ob[0] -= var1;
                ptr->ob[2] -= var2;
                ptr++;
            }
        }
    }
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005CF4C.s")
#else
extern s32 D_80092BDC;
s32 func_80051D68(Object* arg0, s16 arg1, s16 arg2, UnkFunc80051D68Arg3* arg3, s32 arg4, Vec4f* arg5); /* extern */
void func_8005CF4C(Object* arg0, UnkVidStruct2* arg1) {
    s16 var_s2;
    s16 var_s3;
    Vtx_t* var_s0;
    s32 var_s1;
    UnkVidStruct* temp_v0;
    Vec3f spAC;
    SRT sp94;
    SRT sp7C;
    DLTri* var_s5;
    s32 temp_a2;
    Vec4f sp68;
    s16 sp66[2];

    sp68.x = 0.0f;
    sp68.y = 0.0f;
    sp68.z = 0.0f;
    var_s2 = 0;
    var_s3 = 0;
    arg1->unk0x10 = NULL;
    sp66[1] = arg0->srt.yaw;

    if (D_80093000 == 0x27) {
        D_80093000 = 0;
    }
    if (func_80051D68(arg0, arg1->unk0x0 - ((Vec3s32 *)D_80092BE0)->x, arg1->unk0x8 - ((Vec3s32 *)D_80092BE0)->z, (UnkFunc80051D68Arg3* ) D_80092BDC, D_80092C1C, &sp68) == 0) {
        return;
    }

    if (sp68.y != 0.0f) {
        if (sp68.x != 0.0f) {
            var_s2 = arctan2_f(sp68.x, sp68.y);
        }
        if (sp68.z != 0.0f) {
            var_s3 = arctan2_f(sp68.z, sp68.y);
            if (((!var_s5) && (!var_s5)) && (!var_s5)) {}
        }
    }

    gUnknownVideoStructs[D_80093000].obj = arg0;
    sp94.transl.x = 0.0f;
    sp94.transl.y = 0.0f;
    sp94.transl.z = 0.0f;
    sp94.scale = 1.0f;
    sp7C.transl.x = 0.0f;
    sp7C.transl.y = 0.0f;
    sp7C.transl.z = 0.0f;
    sp7C.scale = 1.0f;
    var_s0 = gUnknownVideoStructs[D_80093000].unk0x18;
    var_s5 = (DLTri *)gUnknownVideoStructs[D_80093000].unk0x58_pad;
    temp_v0 = &gUnknownVideoStructs[D_80093000];
    temp_v0->unk0x88 = 0xC8;
    temp_v0->viMode = 1;
    sp94.pitch = var_s3;
    sp94.roll = -var_s2;
    sp94.yaw = 0;
    sp7C.roll = 0;
    sp7C.pitch = 0;
    temp_v0->unk0x78 = gWorldX;
    temp_v0->unk0x7c = gWorldZ;
    sp7C.yaw = sp66[1];
    for (var_s1 = 0; var_s1 < 4; var_s1++, var_s0++) {
        spAC.x = D_800BCC10[var_s1].x;
        spAC.y = D_800BCC10[var_s1].y;
        spAC.z = D_800BCC10[var_s1].z;
        rotate_vec3(&sp7C, &spAC);
        rotate_vec3(&sp94, &spAC);
        var_s0->ob[0] = spAC.x + arg1->unk0x0 - gWorldX;
        var_s0->ob[1] = spAC.y + arg1->unk0x4 + 1;
        var_s0->ob[2] = spAC.z + arg1->unk0x8 - gWorldZ;
        var_s0->cn[0] = 0xFF;
        var_s0->cn[1] = 0xE1;
        var_s0->cn[2] = 0xE1;
        var_s0->cn[3] = temp_v0->unk0x88;
    }
    var_s0 -= 4;
    var_s0[0].tc[0] = 0;
    var_s0[0].tc[1] = 0x400;
    var_s0[1].tc[0] = 0;
    var_s0[1].tc[1] = 0;
    var_s0[2].tc[0] = 0x400;
    var_s0[2].tc[1] = 0;
    var_s0[3].tc[0] = 0x400;
    var_s0[3].tc[1] = 0x400;
    if (arg1->unk0xc == 1) {
        var_s5->v0 = 0;
        var_s5->v1 = 1;
        var_s5->v2 = 2;
        var_s5 += 1;
        var_s5->v0 = 0;
        var_s5->v1 = 2;
        var_s5->v2 = 3;
    } else if (arg1->unk0xc == 2) {
        var_s5->v0 = 0;
        var_s5->v1 = 1;
        var_s5->v2 = 2;
        var_s5 += 1;
        var_s5->v0 = 0;
        var_s5->v1 = 2;
        var_s5->v2 = 3;
    }
    var_s5--;
    gSPVertex(&temp_v0->dl, OS_PHYSICAL_TO_K0(var_s0), 4, 0)
    dl_triangles(&temp_v0->dl2, var_s5, 2);
    gSPEndDisplayList(temp_v0->dl2++)
    temp_a2 = D_80093000 + 1;
    if (temp_a2 == 39) {
        D_80093000 = temp_a2;
        gUnknownVideoStructs->viMode = NULL;
    } else {
        gUnknownVideoStructs[temp_a2].viMode = 0;
        D_80093000 = temp_a2;
    }
    func_8005CD80();
}
#endif


// Note: Return type is void* because the real type is unclear
void *func_8005D3A4(int param) {
    switch (param) {
        case 1:
            return &D_80092EC4;
        case 2:
            return &D_80092F0C;
        case 3:
            return &D_80092F54;
        case 4:
            return &D_80092F9C;
        case 5:
            return &D_80092FE4;  
        case 0:
        default:
            return &D_80092F54;
    } 
}

// file split here?
// look at rodata at 8009ace0

void func_8005D410(s32 videoMode, OSSched* scheduler, s32 someBool) {
    int i;
    u32 width;
    u32 height;
    
    if (osTvType == OS_TV_PAL) {
        gDisplayHertz = 50;
        aspectRatioFloat = gPalAspectRatio;
        D_800BCCB8 = D_8009AD68;
    } else if (osTvType == OS_TV_MPAL) {
        gDisplayHertz = 60;
        aspectRatioFloat = gMPalAspectRatio;
        D_800BCCB8 = 1.0f;
    } else {
        gDisplayHertz = 60;
        aspectRatioFloat = gNtscAspectRatio;
        D_800BCCB8 = 1.0f;
    }

    if (someBool && osTvType == OS_TV_PAL) {
        for (i = 0; i < VIDEO_RESOLUTIONS_COUNT; i++) {
            gResolutionArray[i].v += 20;
        }
    }

    func_8005D9D8();

    set_video_mode(videoMode);

    gFramebufferPointers[0] = NULL;
    gFramebufferPointers[1] = NULL;

    if (someBool) {
        osCreateMesgQueue(&OSMesgQueue_800bcce0, &D_800bccc0[0], 8);
        osScAddClient(scheduler, &D_800bce60, &OSMesgQueue_800bcce0, 2);
    }

    width = 320;
    height = 260;

    if (videoMode == OS_VI_PAL_LPN1) {
        width = 640;
        height = 480;
    }

    initialize_framebuffers(someBool, width, height);

    gFramebufferChoice = 1;

    swap_framebuffer_pointers();
    set_custom_vi_mode();

    D_800BCE14 = 0xc;

    osViBlack(TRUE);

    D_800BCE58 = 0;
    D_800BCE2C = 5;

    if (someBool) {
        D_800BCE18[0] = malloc(960, 2, NULL);
        D_800BCE18[1] = &D_800BCE18[0][480];
    }

    bzero(D_800BCE18[0], 960);

    D_800BCE20 = 0;
    D_800BCE22[0] = 0;
    D_800BCE22[1] = 0;
    D_800BCE34 = 1;
}

/**
 * Sets gVideoMode.
 */
void set_video_mode(s32 mode) {
    gVideoMode = mode;
}

/**
 * Returns gVideoMode.
 */
s32 get_video_mode() {
    return gVideoMode;
}

OSMesgQueue *get_addr_of_OSMesgQueue_8005D670() {
    return &OSMesgQueue_800bcce0;
}

/**
 * Sets gCurrentResolution*[framebufferIndex] to the resolution
 * specified by gVideoMode from gResolutionArray.
 */
void set_current_resolution_from_video_mode(int framebufferIndex) {
    // Note: framebufferIndex was decided on because another function calls this
    //       with gFramebufferChoice (which is either 0 or 1 presumably) and
    //       gCurrentResolutionH and gCurrentResolutionV both conveniently contain
    //       2 integers, which are likely for each framebuffer.
    gCurrentResolutionH[framebufferIndex] = gResolutionArray[gVideoMode & 7].h;
    gCurrentResolutionV[framebufferIndex] = gResolutionArray[gVideoMode & 7].v;
}

/**
 * Returns a video resolution encoded as 0xVVVV_HHHH.
 *
 * If the result of func_8005BC38 is 0, then it will be the current framebuffer's resolution.
 */
u32 get_some_resolution_encoded() {
    s32 var1;
    int flag;

    flag = func_8005BC38(&var1);

    if (flag == FALSE) {
        return (gCurrentResolutionV[gFramebufferChoice] << 0x10) |
                gCurrentResolutionH[gFramebufferChoice];
    } else {
        // Turns 0x0000_XXXX into 0xXXXX_XXXX
        //
        // Maybe this is to default to a square resolution?
        // Say, var1 is 480, this encodes the res as 480x480
        return (var1 << 0x10) | var1;
    }
}

/**
 * Returns the resolution of the framebuffer not currently in use encoded as 0xVVVV_HHHH.
 *
 * Note: The resolution is found by gCurrentResolution*[gFramebufferChoice < 1]
 */
u32 get_other_resolution_encoded() {
    return (gCurrentResolutionV[gFramebufferChoice < 1] << 0x10) |
            gCurrentResolutionH[gFramebufferChoice < 1];
}

void set_custom_vi_mode() {
    u8 viLpn;
    OSViMode *viMode;

    // Determine VI LPN from TV type
    viLpn = OS_VI_NTSC_LPN1;

    if (osTvType == OS_TV_PAL) {
        viLpn = OS_VI_PAL_LPN1;
    } else if (osTvType == OS_TV_MPAL) {
        viLpn = OS_VI_MPAL_LPN1;
    }

    // Determine VI mode from video mode and VI LPN
    switch (gVideoMode & 0x7) {
        case 0x1:
        default:
            viMode = &osViModeTable[2 + viLpn];
            break;
        case 0x6:
            viMode = &osViModeTable[11 + viLpn];
            break;
    }

    // Set gOSViModeCustom to the chosen VI mode
    bcopy(
        viMode,
        &gOSViModeCustom,
        sizeof(OSViMode)
    );

    // Make PAL-specific vStart adjustments
    if (osTvType == OS_TV_PAL) {
        gOSViModeCustom.fldRegs[0].vStart -= 0x180000;
        gOSViModeCustom.fldRegs[1].vStart -= 0x180000;
        gOSViModeCustom.fldRegs[0].vStart += 0x10;
        gOSViModeCustom.fldRegs[1].vStart += 0x10;
    }

    gOSViModeCustom.fldRegs[0].vStart += gVScaleMod * 0x20000;
    gOSViModeCustom.fldRegs[1].vStart += gVScaleMod * 0x20000;
    gOSViModeCustom.fldRegs[0].vStart += gVScaleMod * 0x2;
    gOSViModeCustom.fldRegs[1].vStart += gVScaleMod * 0x2;
    gOSViModeCustom.comRegs.hStart += gHStartMod * 0x20000;
    gOSViModeCustom.comRegs.hStart += gHStartMod * 0x2;

    // Use the custom VI mode and set some special features
    osViSetMode(&gOSViModeCustom);
    osViSetSpecialFeatures(OS_VI_DIVOT_ON);
    osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON);
    osViSetSpecialFeatures(OS_VI_GAMMA_OFF);
}

void initialize_framebuffers(int someBool, s32 width, s32 height) {
    VideoResolution *resPtr;
    u32 hRes;
    u32 vRes;
    
    // Get resolution by current video mode
    resPtr = &gResolutionArray[gVideoMode & 0x7];
    hRes = resPtr->h;
    vRes = resPtr->v;

    // Set current resolution
    gCurrentResolutionH[0] = hRes;
    gCurrentResolutionH[1] = hRes;
    gCurrentResolutionV[0] = vRes;
    gCurrentResolutionV[1] = vRes;

    if (osMemSize != 0x800000) {
        // No expansion pack detected
        gFramebufferPointers[0] = (u16*)(FRAMEBUFFER_ADDRESS_NO_EXP_PAK);
        gFramebufferPointers[1] = (u16*)(FRAMEBUFFER_ADDRESS_NO_EXP_PAK + ((width * height) * 2));
        
        gFramebufferStart = (u16*)(FRAMEBUFFER_ADDRESS_NO_EXP_PAK);
        return;
    }
    
    if (height == 480) {
        // PAL framebuffer height
        gFramebufferPointers[0] = (u16*)(FRAMEBUFFER_ADDRESS_EXP_PAK);
        gFramebufferPointers[1] = (u16*)(FRAMEBUFFER_ADDRESS_EXP_PAK + ((width * height) * 2));
        
        gFramebufferStart = (u16*)(FRAMEBUFFER_ADDRESS_EXP_PAK);
    } else {
        // NTSC/M-PAL framebuffer height
        gFramebufferPointers[0] = (u16*)(FRAMEBUFFER_ADDRESS_EXP_PAK);
        gFramebufferPointers[1] = (u16*)(FRAMEBUFFER_ADDRESS_EXP_PAK + ((width * height) * 2));
        
        gFramebufferEnd = (u16*)(((int) (FRAMEBUFFER_ADDRESS_EXP_PAK + ((width * height) * 2))) + ((width * height) * 2));
        gFramebufferStart = (u16*)0x80200000;
    }
}

void func_8005D9D8() {
    D_800BCE58 = 0;
    D_800BCE59 = 2;
    D_800BCE34 = 1;
}

void func_8005DA00(u32 param1) {
    D_800BCE34 = param1;
}

s32 video_func_returning_delay(s32 param1) {
    s32 ret;
    s32 vidMode;

    ret = 1;

    if (D_800BCE14 != 0) {
        D_800BCE14 -= 1;

        if (D_800BCE14 == 0) {
            osViBlack(FALSE);
        }
    }

    if (param1 != 8) {
        swap_framebuffer_pointers();
    }

    while (osRecvMesg(&OSMesgQueue_800bcce0, NULL, OS_MESG_NOBLOCK) != -1) {
        ret += 1;
    }

    D_800BCE59 = ret;

    if (D_800BCE59 < D_800BCE34) {
        D_800BCE59 = D_800BCE34;
    }

    while (ret < D_800BCE59) {
        osRecvMesg(&OSMesgQueue_800bcce0, NULL, OS_MESG_BLOCK);
        ret++;
    }

    if (D_80093060 != 0) {
        vidMode = get_video_mode();

        if (D_80093060 == 3) {
            set_video_mode(vidMode);
            set_current_resolution_from_video_mode(gFramebufferChoice);
            osViSwapBuffer(gFramebufferCurrent);
        } else if (D_80093060 == 2) {
            set_current_resolution_from_video_mode(gFramebufferChoice);
            osViSwapBuffer(gFramebufferCurrent);
        } else {
            D_800BCC90.hdr.type = 0x11;
            D_800BCC90.hdr.retQueue = (OSMesgQueue*)&gOSViModeCustom;
            osSendMesg(__osViDevMgr.evtQueue, &D_800BCC90, OS_MESG_BLOCK);
            osViSwapBuffer(gFramebufferCurrent);
            D_80093064 ^= 1;
        }

        D_80093060 -= 1;
    } else {
        if (get_pause_state() == 1) {
            // Create pause screen screenshot
            set_pause_state(2);
            bcopy(gFramebufferNext, gFramebufferEnd, 0x25800);
        } else {
            osViSwapBuffer(gFramebufferCurrent);
        }
    }

    signal_apply_controller_inputs();
    func_8005DEE8();
    osRecvMesg(&OSMesgQueue_800bcce0, NULL, OS_MESG_BLOCK);

    return ret;
}

void func_8005DC68() {}

void func_8005DC70(int _) {}

s32 func_8005DC7C() {
    return (s32)((f32)gDisplayHertz / (f32)D_800BCE59);
}

/**
 * Swaps gFramebufferCurrent and gFramebufferNext.
 *
 * Uses gFramebufferChoice to keep track of the index for the next framebuffer to swap to.
 */
void swap_framebuffer_pointers() {
    // Set the current framebuffer to the one last chosen for framebufferNext (see below)
    gFramebufferCurrent = gFramebufferPointers[gFramebufferChoice];

    // TODO: what is this doing?
    D_800BCCB4 = gFramebufferStart; // D_800BCCB4 = &framebufferCurrent+8

    // Swap choice to the other framebuffer index
    //
    // Assuming there are 2 framebuffers, this will flip between 0 and 1:
    //   0 ^ 1 = 1
    //   1 ^ 1 = 0
    gFramebufferChoice = gFramebufferChoice ^ 1;

    // Set next framebuffer to the swapped index
    gFramebufferNext = gFramebufferPointers[gFramebufferChoice];

    // TODO: what is this doing?
    D_800BCCB0 = gFramebufferStart; // D_800BCCB0 = &framebufferCurrent+4
}

/**
 * Returns gFramebufferStart.
 */
u16 *get_framebuffer_start() {
    return gFramebufferStart;
}

/**
 * Returns gFramebufferEnd.
 */
u16 *get_framebuffer_end() {
    return gFramebufferEnd;
}

#ifndef NON_MATCHING
#pragma GLOBAL_ASM("asm/nonmatchings/video/func_8005DD4C.s")
#else
s32 func_8005DD4C(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp2C;
    s32 var_v1;
    Vec3s32* sp24;
    s32 temp_t6;
    u8* sp1C;

    temp_t6 = (D_800BCE20 ^ 1) & 0xFF;
    sp24 = D_800BCE18[temp_t6];
    for (sp2C = 0, var_v1 = 0; var_v1 < D_800BCE22[temp_t6]; var_v1++) {
        if (arg2 == sp24[var_v1].z) {
            sp2C = sp24[var_v1].x;
            break;
        }
    }

    sp1C = &D_800BCE22[D_800BCE20];
    sp24 = D_800BCE18[D_800BCE20];
    sp24[*sp1C].z = arg2;
    sp24[*sp1C].x = 0;
    if (is_size_smaller_than_resolution(arg0, arg1) == 0) {
        temp_t6 = -1;
    } else {
        temp_t6 = (gCurrentResolutionH[gFramebufferChoice] * arg1) + arg0;
    }

    sp24[*sp1C].y = temp_t6;
    *sp1C += 1;
    return sp2C;
}
#endif

/**
 * Returns whether the given width and height is smaller than the current framebuffer's resolution.
 *
 * Note: Both width AND height must be smaller for this to return true.
 */
int is_size_smaller_than_resolution(s32 width, s32 height) {
    return width >= 0
        && (u32)width < gCurrentResolutionH[gFramebufferChoice]
        && height >= 0
        && (u32)height < gCurrentResolutionV[gFramebufferChoice];
}

void func_8005DEE8(void) {
    s32 i;
    u8 new_var;
    s32 new_var2;
    u32 temp_t5;
    u8 *temp;
    UnkVidStruct3* temp_a3;
    Vec3s32 *temp2;

    new_var = D_800BCE20;
    temp2 = D_800BCE18[new_var];
    i = 0;
    temp = &D_800BCE22[D_800BCE20];
    for (; *temp > i; i++) {
        new_var2 = temp2[i].y;
        temp_t5 = new_var2;
        if (new_var2 >= 0) {
            temp_t5 = gFramebufferStart[new_var2] >> 2;
            // temp_t5 >> 0xB loads the first 21 bits
            temp_a3 = &D_80093068[((temp_t5 >> 0xB) & 7)];
            // temp_t5 & 0x7FF loads the last 11 bits
            temp2[i].x = (temp_a3->unk0x4 + ((temp_t5 & 0x7FF) << temp_a3->unk0x0)) >> 3;
        } else {
            temp2[i].x = 0;
        }
    }

    D_800BCE20 ^= 1;
    D_800BCE22[D_800BCE20] = 0;
}

/**
 * Note: param1 is most likely a boolean
 */
void some_video_setup(s32 param1) {
    if (param1) {
        set_video_mode(7);
        initialize_framebuffers(1, gResolutionArray[7].h, gResolutionArray[7].v);

        set_custom_vi_mode();
        D_800BCE14 = 0xc;
        osViBlack(TRUE);
        D_800BCE58 = 0;
        D_800BCE2C = 0x5;
    } else {
        set_video_mode(1);
        initialize_framebuffers(1, gResolutionArray[0].h, gResolutionArray[0].v);

        set_custom_vi_mode();
        D_800BCE14 = 0xc;
        osViBlack(TRUE);
        D_800BCE58 = 0;
        D_800BCE2C = 0x5;
    }
}

void modify_vi_mode(u8 a0, s8 hStartMod, s8 vScaleMod) {
    u8 viLpn;
    OSViMode *viMode;

    gHStartMod = hStartMod;
    gVScaleMod = vScaleMod;

    // If a0 is 0, don't set custom vi mode
    if (a0 == 0) {
        return;
    }

    // Determine VI LPN from TV type
    if (osTvType == OS_TV_PAL) {
        viLpn = OS_VI_PAL_LPN1;
    } else if (osTvType == OS_TV_MPAL) {
        viLpn = OS_VI_MPAL_LPN1;
    } else {
        viLpn = OS_VI_NTSC_LPN1;
    }

    // Determine VI mode from video mode and VI LPN
    if (gVideoMode == OS_VI_PAL_LPN1 /*0xe*/) {
        viMode = &osViModeTable[11 + viLpn];
    } else {
        viMode = &osViModeTable[2 + viLpn];
    }

    // Set gOSViModeCustom to the chosen VI mode
    bcopy(
        viMode,
        &gOSViModeCustom,
        sizeof(OSViMode)
    );

    // Make PAL-specific vStart adjustments
    if (osTvType == OS_TV_PAL) {
        gOSViModeCustom.fldRegs[0].vStart -= 0x180000;
        gOSViModeCustom.fldRegs[1].vStart -= 0x180000;
        gOSViModeCustom.fldRegs[0].vStart += 0x10;
        gOSViModeCustom.fldRegs[1].vStart += 0x10;
    }

    gOSViModeCustom.fldRegs[0].vStart += gVScaleMod * 0x20000;
    gOSViModeCustom.fldRegs[1].vStart += gVScaleMod * 0x20000;
    gOSViModeCustom.fldRegs[0].vStart += gVScaleMod * 0x2;
    gOSViModeCustom.fldRegs[1].vStart += gVScaleMod * 0x2;
    gOSViModeCustom.comRegs.hStart += gHStartMod * 0x20000;
    gOSViModeCustom.comRegs.hStart += gHStartMod * 0x2;

    D_80093060 = 3;
}
