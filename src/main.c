#include "sys/main.h"

#include "PR/os.h"
#include "PR/sched.h"
#include "game/gamebits.h"
#include "libnaudio/n_unkfuncs.h"
#include "sys/audio/amAudio.h"
#include "sys/asset.h"
#include "sys/audio.h"
#include "sys/dll.h"
#include "sys/joypad.h"
#include "sys/reset.h"
#include "sys/di_cpu.h"
#include "sys/pi.h"
#include "sys/memory.h"
#include "sys/newshadows.h"
#include "sys/objects.h"
#include "sys/footsteps.h"
#include "sys/print.h"
#include "sys/rarezip.h"
#include "sys/rcp.h"
#include "sys/menu.h"
#include "sys/fonts.h"
#include "sys/boot.h"
#include "sys/di_rcp.h"
#include "sys/rsp_segment.h"
#include "sys/thread.h"
#include "sys/voxmap.h"
#include "sys/framebuffer_fx.h"
#include "sys/segment_1D900.h"
#include "sys/intersect.h"
#include "sys/map.h"
#include "sys/map_enums.h"
#include "dll.h"
#include "constants.h"
#include "dongle.h"
#include "macros.h"

/* -------- .data start -------- */
const char *gGameBuildVersion = "1.3623";
const char *gGameBuildDateTime = "01/12/00 09:19";
const char *gGameBuildName = "dragon1";
char gDBVersion[] = "Version 2.8 14/12/98 15.30 L.Schuneman";
static u8 _unkBytes[] = { 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x01
};
u8 D_8008C940 = 0;
u8 D_8008C944 = 0xFF;
/**
 * The index of the last inserted controller.
 *
 * For example, if one controller is inserted then this will be 0.
 * If no controllers are inserted, this will be -1.
 */
s8 gLastInsertedControllerIndex = 0;
s8 D_8008C94C = 0;
u8 gUpdateRate = 1;
u8 gUpdateRateMirror = 1;
f32 gUpdateRateF = 1.0f;
f32 gUpdateRateMirrorF = 1.0f;
f32 gUpdateRateInverseF = 1.0f;
f32 gUpdateRateInverseMirrorF = 1.0f;
s32 gMainMapChangeNextMenu = -1;
u8 alSynFlag = 0;
s32 D_8008C970 = 0;
DLL_28_screen_fade *gDLL_28_ScreenFade = NULL;
DLL_2_camera *gDLL_2_Camera = NULL;
DLL_3_animation *gDLL_3_Animation = NULL;
DLL_7_newday *gDLL_7_Newday = NULL;
DLL_8 *gDLL_8 = NULL;
DLL_9_newclouds *gDLL_9_Newclouds = NULL;
DLL_10_newstars *gDLL_10_Newstars = NULL;
DLL_12_minic *gDLL_12_Minic = NULL;
DLL_1_cmdmenu *gDLL_1_cmdmenu = NULL;
DLL_4_race *gDLL_4_Race = NULL;
DLL_5_AMSEQ *gDLL_5_AMSEQ = NULL;
DLL_5_AMSEQ *gDLL_5_AMSEQ2 = NULL;
DLL_6_AMSFX *gDLL_6_AMSFX = NULL;
DLL_11_newlfx *gDLL_11_Newlfx = NULL;
DLL_57 *gDLL_57 = NULL;
DLL_58 *gDLL_58 = NULL;
DLL_13_expgfx *gDLL_13_Expgfx = NULL;
DLL_14_modgfx *gDLL_14_Modgfx = NULL;
DLL_15_projgfx *gDLL_15_Projgfx = NULL;
DLL_16 *gDLL_16 = NULL;
DLL_17 *gDLL_17_partfx = NULL;
DLL_18_objfsa *gDLL_18_objfsa = NULL;
DLL_20_screens *gDLL_20_Screens = NULL;
DLL_21_gametext *gDLL_21_Gametext = NULL;
DLL_22_subtitles *gDLL_22_Subtitles = NULL;
DLL_23 *gDLL_23 = NULL;
DLL_24_waterfx *gDLL_24_Waterfx = NULL;
DLL_25 *gDLL_25 = NULL;
DLL_26_curves *gDLL_26_Curves = NULL;
DLL_74_picmenu *gDLL_74_Picmenu = NULL;
DLL_75 *gDLL_75 = NULL;
DLL_27 *gDLL_27 = NULL;
DLL_29_gplay *gDLL_29_Gplay = NULL;
DLL_56 *gDLL_56 = NULL;
DLL_30_task *gDLL_30_Task = NULL;
DLL_31_flash *gDLL_31_Flash = NULL;
DLL_76 *gDLL_76 = NULL;
DLL_32 *gDLL_32 = NULL;
DLL_33_BaddieControl *gDLL_33_BaddieControl = NULL;
DLL_59_minimap *gDLL_59_Minimap = NULL;
DLL_54 *gDLL_54_pickup = NULL;

s32 gTempDLLIds[3] = {-1, 53, 55};
void *gTempDLLInsts[3] = {NULL, NULL, NULL};
u8 D_8008CA30 = 0;
/* -------- .data end -------- */

/* -------- .bss start 800ae2a0 -------- */
GameState *gGplayState;
BitTableEntry *gFile_BITTABLE;
s16 gSizeBittable;
struct Vec3_Int PlayerPosBuffer[60]; // seems to buffer player coords with "timestamp"
s32 PlayerPosBuffer_index;
u32 D_800AE674;
// official name: gfxBase
Gfx *gMainGfx[2];
// official name: gfx
Gfx *gCurGfx;
// official name: mtxBase
Mtx *gMainMtx[2];
// official name: mtx
Mtx *gCurMtx;
// official name: vtxBase
Vertex *gMainVtx[2];
// official name: vtx
Vertex *gCurVtx;
// official name: polBase
Triangle *gMainPol[2];
// official name: pol
Triangle *gCurPol;
OSSched osscheduler_;
u64 ossceduler_stack[STACKSIZE(OS_SC_STACKSIZE)];
s8 gMainDoMapChange;
u8 gFrameBufIdx;
s8 gPauseState;
u8 gDemoState;
s8 gDemoFinished;
/* -------- .bss end 800b09d0 -------- */

void main_func_8001440C(s32 arg0);
void mainClearPlayerPosBuffer(void);
void mainInit(void);
void mainInitBits(void);
void mainTickNoExpansion(void);
void mainTick(void);
void mainHandleMapChange(void);
void mainAllocFrameBuffers(void);
void main_func_80013D80(void);
s8 main_func_800143FC(void);
void mainUpdatePlayerPosBuffer();
void mainTestWrite(void);
void mainCheckDongle(void);

// official name: mainThread ?
void mainThreadEntry(void *arg) {
#ifndef NON_MATCHING
    mainTestWrite(); // ROM write check
#endif
    mainInit();

    while(TRUE) {
#ifndef NON_MATCHING
        mainCheckDongle();  // copy protection check
#endif
        if (osMemSize != EXPANSION_RAM_SIZE) {
            mainTickNoExpansion();
        } else {
            mainTick();
        }

        bootCheckStack();
    }
}

void mainInit(void) {
    s32 tvMode;

    mmInit();
    rarezipInit();
    assetInit();

    if (0) {
    } else if (osTvType == OS_TV_PAL) {
        tvMode = OS_VI_PAL_LAN1;
    } else if (osTvType == OS_TV_MPAL) {
        tvMode = OS_VI_MPAL_LAN1;
    } else {
        tvMode = OS_VI_NTSC_LAN1;
    }

    osCreateScheduler(&osscheduler_, &ossceduler_stack[STACKSIZE(OS_SC_STACKSIZE)], 
        OS_SCHEDULER_THREAD_PRIORITY, tvMode, 1);
    diCpuTraceInit();
    piInit();
    rcpInit(&osscheduler_);
    mainAllocFrameBuffers();
    if (0) {};
    gFrameBufIdx = 0;
    gCurGfx = gMainGfx[gFrameBufIdx];
    gLastInsertedControllerIndex = joyInit();
    joyStartControllerThread(&osscheduler_);
    resetInit(&osscheduler_);
    texInitTextures();
    trackInit();
    func_8001CD00();
    modInit();
    dllInit();
    objInit();
    diPrintfInit();
    trackIntersectInit();
    shadowsInit();
    footstepsInit();
    fontsInit();
    menuInit();
    amCreateAudioMgr(&osscheduler_, AUDIO_THREAD_PRIORITY);
    mapInitGlobalMap();
    if (osMemSize != EXPANSION_RAM_SIZE) {
        gDLL_5_AMSEQ2 = gDLL_5_AMSEQ = dllLoad(DLL_ID_AMSEQ, 36);
        gDLL_6_AMSFX = dllLoad(DLL_ID_AMSFX, 18);
        gDLL_21_Gametext = dllLoad(DLL_ID_TEXT, 5);
        gDLL_29_Gplay = dllLoad(DLL_ID_GPLAY, 47);
        gDLL_31_Flash = dllLoad(DLL_ID_FLASH, 2);
        gDLL_28_ScreenFade = dllLoad(DLL_ID_SCREEN_FADE, 4);
    } else {
        gDLL_1_cmdmenu = dllLoad(DLL_ID_CMDMENU, 15);
        gDLL_2_Camera = dllLoad(DLL_ID_CAMERA, 23);
        gDLL_23 = dllLoad(DLL_ID_23, 8);  // 0x12 in SFA
        gDLL_18_objfsa = dllLoad(DLL_ID_18, 22); // 0x0F in SFA
        gDLL_3_Animation = dllLoad(DLL_ID_ANIM, 29);
        gDLL_28_ScreenFade = dllLoad(DLL_ID_SCREEN_FADE, 4); // 0x16 in SFA
        gDLL_25 = dllLoad(DLL_ID_25, 14);                    // not present in SFA
        gDLL_7_Newday = dllLoad(DLL_ID_NEWDAY, 15);
        gDLL_8 = dllLoad(DLL_ID_8, 12); // 0x06 in SFA
        gDLL_9_Newclouds = dllLoad(DLL_ID_NEWCLOUDS, 8);
        gDLL_10_Newstars = dllLoad(DLL_ID_NEWSTARS, 3);
        gDLL_12_Minic = dllLoad(DLL_ID_MINIC, 10);
        gDLL_4_Race = dllLoad(DLL_ID_RACE, 13);
        gDLL_5_AMSEQ2 = gDLL_5_AMSEQ = dllLoad(DLL_ID_AMSEQ, 36);
        gDLL_6_AMSFX = dllLoad(DLL_ID_AMSFX, 18);
        gDLL_11_Newlfx = dllLoad(DLL_ID_NEWLFX, 7);
        gDLL_13_Expgfx = dllLoad(DLL_ID_EXPGFX, 10);
        gDLL_14_Modgfx = dllLoad(DLL_ID_MODGFX, 12);
        gDLL_15_Projgfx = dllLoad(DLL_ID_PROJGFX, 8);
        gDLL_16 = dllLoad(DLL_ID_16, 3);
        gDLL_17_partfx = dllLoad(DLL_ID_PARTFX, 2); // probably particle FX
        gDLL_20_Screens = dllLoad(DLL_ID_SCREENS, 3);
        gDLL_21_Gametext = dllLoad(DLL_ID_TEXT, 5);
        gDLL_22_Subtitles = dllLoad(DLL_ID_SUBTITLES, 7);
        gDLL_24_Waterfx = dllLoad(DLL_ID_WATERFX, 7);
        gDLL_26_Curves = dllLoad(DLL_ID_CURVES, 38);
        gDLL_74_Picmenu = dllLoad(DLL_ID_PICMENU, 7);
        gDLL_27 = dllLoad(DLL_ID_27, 9); // 0x15 in SFA
        gDLL_29_Gplay = dllLoad(DLL_ID_GPLAY, 36);
        gDLL_56 = dllLoad(DLL_ID_56, 10); // not present in SFA
        gDLL_30_Task = dllLoad(DLL_ID_TASK, 6);
        gDLL_31_Flash = dllLoad(DLL_ID_FLASH, 2); // param is 0x24 in SFA
        gDLL_32 = dllLoad(DLL_ID_32, 6);          // 0x18 in SFA
        gDLL_33_BaddieControl = dllLoad(DLL_ID_33, 22);         // 0x19 in SFA
        gDLL_59_Minimap = dllLoad(DLL_ID_MINIMAP, 2);
        gDLL_54_pickup = dllLoad(DLL_ID_54, 12); // 0x2F in SFA
        gDLL_57 = dllLoad(DLL_ID_57, 4);
        gDLL_58 = dllLoad(DLL_ID_58, 2);
        gDLL_30_Task->vtbl->load_recently_completed();
    }
    mainInitBits();
    alSynFlag = 1;
    amGo();
    am_func_80012224(0);
    if (0) {};
    gDPFullSync(gCurGfx++);
    gSPEndDisplayList(gCurGfx++);
    diRcpTraceInit();
    menuSet(MENU_POST);
    if (osMemSize == EXPANSION_RAM_SIZE) {
        mainHandleMapChange();
    }
    trackSetZBufferOn(FALSE);
    trackSetSkyOn(FALSE);
}

void mainTick(void) {
    u8 clearFlags;
    u32 updateRate;
    Gfx **gdl;

    osSetTime(0);
    diRcpTraceReset();

    gdl = &gCurGfx;

    // unused return type
    rcpF3DEX_2_XBUS(gMainGfx[gFrameBufIdx], gCurGfx, 0);

    gFrameBufIdx ^= 1;
    gCurGfx = gMainGfx[gFrameBufIdx];
    gCurMtx = gMainMtx[gFrameBufIdx];
    gCurVtx = gMainVtx[gFrameBufIdx];
    gCurPol = gMainPol[gFrameBufIdx];

    diRcpTrace(gCurGfx, 0, "main/main.c", 0x28E);
    segSetBase(&gCurGfx, SEGMENT_MAIN, (void *)K0BASE);
    segSetBase(&gCurGfx, SEGMENT_FRAMEBUFFER, gFrontFramebuffer);
    segSetBase(&gCurGfx, SEGMENT_ZBUFFER, gFrontDepthBuffer);
    fbfxTick(&gCurGfx, gUpdateRate);
    dlSetAllDirty();
    texRenderReset();

    if (gDLBuilder->needsPipeSync != 0) {
        gDLBuilder->needsPipeSync = 0;
        gDPPipeSync(gCurGfx++);
    }

    gDPSetDepthImage(gCurGfx++, SEGMENT_ADDR(SEGMENT_ZBUFFER, 0));

    rcpInitSp(&gCurGfx);

    clearFlags = CLEAR_ZBUFFER;
    if (trackIsZBufferOn() == FALSE) {
        clearFlags = CLEAR_NONE;
    } else if (trackIsSkyOn() == FALSE) {
        clearFlags = CLEAR_COLOR | CLEAR_ZBUFFER;
    }

    rcpClearScreen(&gCurGfx, &gCurMtx, clearFlags);
    voxUpdateCacheTimers();
    main_func_80013D80();
    am_func_800121DC();
    gDLL_28_ScreenFade->vtbl->draw(gdl, &gCurMtx, &gCurVtx);
    gDLL_22_Subtitles->vtbl->func_578(gdl);
    camTick();
    assetQueueTick();
    diPrintfAll(gdl);

    gDPFullSync(gCurGfx++);
    gSPEndDisplayList(gCurGfx++);

    rcpWaitDP();
    objDoDeferredFree();
    mmFreeTick();

    if (gPauseState == 0) {
        camApplyAlternateTrigger();
    }

    gUpdateRate = viFrameSync(0);
    
    if (0) {}

    updateRate = (u32) gUpdateRate;
    if (gUpdateRate > 6) {
        gUpdateRate = 6;
        updateRate = gUpdateRate;
    }
    gUpdateRateF = (f32) updateRate;
    gUpdateRateInverseF = 1.0f / gUpdateRateF;
    gUpdateRateMirror = updateRate;
    gUpdateRateMirrorF = gUpdateRateF;
    gUpdateRateInverseMirrorF = 1.0f / gUpdateRateMirrorF;

    mainHandleMapChange();
    diCpuTraceFile("main/main.c", 0x37C);
}

void mainTickNoExpansion(void) {
    u32 updateRate;
    Gfx **tmp_s0;

    tmp_s0 = &gCurGfx;

    rcpF3DEX_2_XBUS(gMainGfx[gFrameBufIdx], gCurGfx, 0);

    gFrameBufIdx ^= 1;
    gCurGfx = gMainGfx[gFrameBufIdx];
    gCurMtx = gMainMtx[gFrameBufIdx];
    gCurVtx = gMainVtx[gFrameBufIdx];
    gCurPol = gMainPol[gFrameBufIdx];

    segSetBase(&gCurGfx, SEGMENT_MAIN, (void *)K0BASE);
    segSetBase(&gCurGfx, SEGMENT_FRAMEBUFFER, gFrontFramebuffer);
    segSetBase(&gCurGfx, SEGMENT_ZBUFFER, gFrontDepthBuffer);
    dlSetAllDirty();
    texRenderReset();

    if (gDLBuilder->needsPipeSync != 0) {
        gDLBuilder->needsPipeSync = 0U;
        gDPPipeSync(gCurGfx++);
    }

    gDPSetDepthImage(gCurGfx++, SEGMENT_ADDR(SEGMENT_ZBUFFER, 0x0));

    rcpInitSp(&gCurGfx);
    menuUpdate1(); // ignored return value
    menuDraw(&gCurGfx, &gCurMtx, &gCurVtx, &gCurPol);
    assetQueueTick();
    gDLL_28_ScreenFade->vtbl->draw(tmp_s0, &gCurMtx, &gCurVtx);

    gDPFullSync(gCurGfx++);
    gSPEndDisplayList(gCurGfx++);

    rcpWaitDP();
    mmFreeTick();

    gUpdateRate = viFrameSync(0);
    updateRate = (u8)gUpdateRate;
    if ((s32)updateRate > 6) {
        gUpdateRate = 6;
        updateRate = (u8)gUpdateRate;
    }
    gUpdateRateF = (f32) updateRate;
    gUpdateRateInverseF = 1.0f / gUpdateRateF;
    gUpdateRateMirror = updateRate;
    gUpdateRateMirrorF = gUpdateRateF;
    gUpdateRateInverseMirrorF = 1.0f / gUpdateRateMirrorF;
}

void main_func_80013D80(void) {
    s32 button;

    joyDisableButtons(0, U_JPAD | R_JPAD);
    gDLL_2_Camera->vtbl->lock_icon_tick();
    gDLL_22_Subtitles->vtbl->func_4C0();

    if (menuUpdate1() == 0) {
        button = joyGetPressed(0);

        if (gPauseState != 0) {
            rcpDrawPauseScreenFreezeFrame(&gCurGfx);
        }

        if (gPauseState == 0) {
            objTick();
            trackTick(0);

            if ((camIsAlternateActive() == 0) 
                    && (D_8008C94C == 0) 
                    && (main_func_800143FC() == 0) 
                    && ((button & START_BUTTON) != 0) 
                    && (mainGetBits(BIT_Menus_Selection_Blocked) == 0)) {
                gPauseState = 1;
                joyDisableButtons(0, START_BUTTON);
                menuSet(MENU_PAUSE);
            }

            gDLL_29_Gplay->vtbl->tick();
        } else {
            objUpdateObjModels();
        }

        if (gPauseState == 0) {
            mainUpdatePlayerPosBuffer();
        }

        menuUpdate2();
        trackIntersectTick();
        map_func_8004A67C();
        mapUpdateStreaming();
        objHandleAnimseqActors();

        gDLL_4_Race->vtbl->func14();

        if (gPauseState == 0) {
            trackDraw(&gCurGfx, &gCurMtx, &gCurVtx, &gCurPol, &gCurVtx, &gCurPol);
        }

        gDLL_20_Screens->vtbl->draw(&gCurGfx);
        menuDraw(&gCurGfx, &gCurMtx, &gCurVtx, &gCurPol);

        D_8008C94C -= gUpdateRate;

        if ((s32)D_8008C94C < 0) {
            D_8008C94C = 0;
        }
    }
}

void main_func_80013FB4(void) {
    viInit(OS_VI_PAL_LPN1, NULL, FALSE);
    trackSetZBufferOn(FALSE);
    trackSetSkyOn(FALSE);
    gDLL_5_AMSEQ->vtbl->stop(3);
    gDLL_5_AMSEQ->vtbl->stop(0);
    gDLL_5_AMSEQ->vtbl->stop(1);
    gDLL_22_Subtitles->vtbl->func_448();
    mainUnpause();
    mainChangeMap(MAP_FRONT_END2, 0, PLAYER_KRYSTAL, /*don't change menu*/-1);
}

void mainHandleMapChange(void) {
    if (gMainDoMapChange) {
        // "$$$$$  CHANGEMAP \n" (default.dol)
        mmSetDelay(0);
        if (D_8008CA30 != 0) {
            rcpSetScreenColour(0, 0, 0);
            func_800668A4();
            map_func_800484A8();

            gCurGfx = gMainGfx[gFrameBufIdx];
            gDPFullSync(gCurGfx++);
            gSPEndDisplayList(gCurGfx++);
        }

        gMainDoMapChange = FALSE;

        mmSetDelay(0);
        camInit();

        if (gMainMapChangeNextMenu >= 0) {
            menuSet(gMainMapChangeNextMenu);
            gMainMapChangeNextMenu = -1;
        }

        map_func_8004773C();

        if (gDLL_23 != NULL) {
            gDLL_23->vtbl->func_18(1);
        }

        mmSetDelay(2);
        D_8008CA30 = 1;
    }
}

// officialName: mainChangeMap
void mainChangeMap(s32 mapID, s32 setupID, s32 playerno, s32 menuID) {
    PlayerLocation *location;

    // "mainChangeMap(%d,%d,%d)\n" (default.dol)

    main_func_8001440C(0);

    if (playerno <= PLAYER_NONE) {
        playerno = PLAYER_SABRE;
    }

    mainClearPlayerPosBuffer();

    gDLL_30_Task->vtbl->load_recently_completed();
    gDLL_29_Gplay->vtbl->set_playerno(playerno);

    location = gDLL_29_Gplay->vtbl->get_player_saved_location();

    map_func_80048054(mapID, setupID, &location->vec.x, &location->vec.y, &location->vec.z, &location->mapLayer);
    gDLL_29_Gplay->vtbl->savepoint(&location->vec, 0, 0, location->mapLayer);

    gMainDoMapChange = TRUE;
    gMainMapChangeNextMenu = menuID;
}

void main_func_800142A0(f32 arg0, f32 arg1, f32 arg2) {
    main_func_8001440C(0);
    map_func_800483BC(arg0, arg1, arg2);
    mainClearPlayerPosBuffer();
    gMainDoMapChange = TRUE;
}

void mainStartGame(f32 x, f32 y, f32 z, s32 playerno) {
    Vec3f pos;
    pos.x = x;
    pos.y = y;
    pos.z = z;

    main_func_8001440C(0);

    gDLL_29_Gplay->vtbl->init_save(-1, NULL);
    gDLL_29_Gplay->vtbl->set_playerno(playerno);
    gDLL_29_Gplay->vtbl->savepoint(&pos, 0, 0, 0);
    gDLL_29_Gplay->vtbl->start_loaded_game();
}

void main_func_800143A4(void) {
    map_func_80048034();
    gMainDoMapChange = TRUE;
}

Gfx *main_func_800143D0(Gfx **arg0) {
    *arg0 = gMainGfx[gFrameBufIdx];
    return gCurGfx;
}

s8 main_func_800143FC(void) {
    return D_8008C940;
}

void main_func_8001440C(s32 arg0) {
    D_8008C940 = arg0;
}

// official name: mainGetPauseMode ?
s8 mainGetPauseState(void) {
    return gPauseState;
}

void mainUnpause(void) {
    gPauseState = 0;
}

// official name: mainSetPauseMode ?
void mainSetPauseState(s32 state) {
    gPauseState = state;
}

#define MAIN_GFX_BUF_SIZE (sizeof(Gfx) * 4500)
#define MAIN_MTX_BUF_SIZE (sizeof(Mtx) * 1100)
#define MAIN_POL_BUF_SIZE (sizeof(Triangle) * 50)
#define MAIN_VTX_BUF_SIZE (sizeof(Vertex) * 480)

void mainAllocFrameBuffers(void) {
    // in default.dol these have names as well.
    // alloc graphic display list command buffers. ("main:gfx" in default.dol)
    gMainGfx[0] = mmAlloc(MAIN_GFX_BUF_SIZE * 2, ALLOC_TAG_LISTS_COL, NULL);
    gMainGfx[1] = (Gfx *)((u32)gMainGfx[0] + MAIN_GFX_BUF_SIZE);

    // matrix buffers ("main:mtx")
    gMainMtx[0] = mmAlloc(MAIN_MTX_BUF_SIZE * 2, ALLOC_TAG_LISTS_COL, NULL);
    gMainMtx[1] = (Mtx *)((u32)gMainMtx[0] + MAIN_MTX_BUF_SIZE);

    // polygon buffers? ("main:pol")
    gMainPol[0] = mmAlloc(MAIN_POL_BUF_SIZE * 2, ALLOC_TAG_LISTS_COL, NULL);
    gMainPol[1] = (Triangle *)((u32)gMainPol[0] + MAIN_POL_BUF_SIZE);

    // vertex buffers ("main:vtx")
    gMainVtx[0] = mmAlloc(MAIN_VTX_BUF_SIZE * 2, ALLOC_TAG_LISTS_COL, NULL);
    gMainVtx[1] = (Vertex *)((u32)gMainVtx[0] + MAIN_VTX_BUF_SIZE);
}

void main_func_80014508(s8 arg0) {
    D_8008C94C = arg0;
}

/**
 * @returns TRUE if no controllers are inserted.
 * @pre mainInit must be called first.
 * @see mainInit, joyInit
 */
s32 mainAreNoControllersInserted(void) {
    if (gLastInsertedControllerIndex == -1) {
        // No controllers are inserted
        return TRUE;
    } else {
        return FALSE;
    }
}

s32 main_ret1_8001454c(void) {
    return 1;
}

void func_initing_rumblepak(void) {
    osContStartQuery(0);
    osContGetQuery(0);
    osMotorInit(0, 0, 0);
    _depth2Cents(0);
}

void mainTestWrite(void) {
    HW_REG2(0x1C000C02, u16) = 0x4040;
}

/*
 * Probe the copy protection dongle for the correct magic string and
 * if failed, wipe a majority of RAM to prevent RAM viewing.
 */
void mainCheckDongle(void) {
    // attempt to get the first magic short from the dongle. if it is
    // connected, this will retrieve correctly.
    u32 head = ACCESS_1;

    // append the other part.
    head <<= 16;
    head |= ACCESS_2;

    /*
     * Perform the check against the 2 known codes:
     *
     * 'LSFS' (0x4C534653)
     * 'MPFS' (0x4D504653)
     *
     * It is not known which one the original dongle for this ROM
     * was intended to use.
     */
    
    if ((head == DONGLE_LSFS) || (head == DONGLE_MPFS)) {
        return;
    } else {
        int *write = (int *)(u32)NO_EXPANSION_RAM_END;
        // copy protection failed. Wipe every 2nd word to prevent RAM analysis.
        // probably hackers trying to view our precious data.
        while ((u32)write != RAM_START) {
            *write = 0;
            write -= 2; // hmm...
        }
    }
}

OSSched *mainGetScheduler(void) {
    return &osscheduler_;
}

void mainInitBits(void) {
    assetRomLoad((void **)&gFile_BITTABLE, BITTABLE_BIN);
    // @bug: This should be dividing by 4 (not 2) since each entry is 4 bytes long
    gSizeBittable = piRomGetFileSize(BITTABLE_BIN) >> 1;
    gGplayState = gDLL_29_Gplay->vtbl->get_state();
}

// offical name: mainSetBits
void mainSetBits(s32 entry, u32 value) {
    u8 *bitString;
    u8 _pad[12]; // fake match
    s32 idx;
    s32 mask;
    s32 endBit;
    s32 startBit;

    if (entry != BIT_ALWAYS_1 && entry != BIT_ALWAYS_0 && entry != -1) {
        switch (gFile_BITTABLE[entry].field_0x2 >> 6) {
            case 0: // Never saved to savegame
                bitString = &gGplayState->bitString[0];
                break;
            case 1: // Saved with savepoints
                bitString = &gGplayState->save.main.bitString[0];
                break;
            case 2: // Always saved
                bitString = &gGplayState->save.file.bitString[0];
                break;
            case 3: // Saved with savepoints that include a map save
                bitString = &gGplayState->save.map.bitString[0];
                break;
        }

        if (gFile_BITTABLE[entry].field_0x2 & (1 << 5)) {
            gDLL_30_Task->vtbl->mark_task_completed(gFile_BITTABLE[entry].task);
        }

        startBit = gFile_BITTABLE[entry].start;
        endBit = (gFile_BITTABLE[entry].field_0x2 & 0x1f) + 1;
        mask = 1;

        for (idx = startBit; idx < (startBit + endBit); idx++) {
            if (mask & value) {
                *(u8 *)((u32)bitString + (idx >> 3)) |= (1 << (idx & 7));
            } else {
                *(u8 *)((u32)bitString + (idx >> 3)) &= ~(1 << (idx & 7));
            }

            mask = mask << 1;
        }
    }
}

// offical name: mainGetBits
u32 mainGetBits(s32 entry) {
    u8 *bitString;
    u32 value;
    s32 idx;
    s32 mask;
    s32 endBit;
    s32 startBit;

    if (entry == BIT_ALWAYS_1) {
        return 1;
    } else if (entry == BIT_ALWAYS_0) {
        return 0;
    } else if (entry == -1) {
        return 0;
    } else if (entry < 0 || entry >= gSizeBittable) {
        return 0;
    } else {
        switch (gFile_BITTABLE[entry].field_0x2 >> 6) {
            case 0: // Never saved to savegame
                bitString = &gGplayState->bitString[0];
                break;
            case 1: // Saved with savepoints
                bitString = &gGplayState->save.main.bitString[0];
                break;
            case 2: // Always saved
                bitString = &gGplayState->save.file.bitString[0];
                break;
            case 3: // Saved with savepoints that include a map save
                bitString = &gGplayState->save.map.bitString[0];
                break;
        }

        startBit = gFile_BITTABLE[entry].start;
        endBit = (gFile_BITTABLE[entry].field_0x2 & 0x1f) + 1;
        value = 0;
        mask = 1;

        for (idx = startBit; idx < (startBit + endBit); idx++) {
            // A clever way to read from bitString bit by bit
            if ((*(u8 *)((u32)bitString + (idx >> 3)) & (1 << (idx & 7))) != 0) {
                value |= mask;
            }

            mask = mask << 1;
        }
    }

    return value;
}

s32 mainIncrementBits(s32 entry) {
    s32 val;
    s32 maxVal;

    val = mainGetBits(entry) + 1;

    maxVal = 1 << ((gFile_BITTABLE[entry].field_0x2 & 0x1f) + 1);

    if (val < maxVal) {
        mainSetBits(entry, val);
    } else {
        val -= 1;
    }

    return val;
}

s32 mainDecrementBits(s32 entry) {
    s32 val = mainGetBits(entry);
    if (val != 0) {
        mainSetBits(entry, --val);
        return val;
    }
    return 0;
}

s32 mainCreateTempDLL(s32 id) {
    u32 idx;

    idx = 0;

    while (idx < 3 && id != gTempDLLIds[idx]) {
        idx++;
    }

    if (idx == 3) {
        return 0;
    }

    if (gTempDLLInsts[idx] != NULL) {
        STUBBED_PRINTF(" WARNING : temp dll no %i is alreadly created \n");
    }

    gTempDLLInsts[idx] = dllLoad(id, 1);

    return 1;
}

s32 mainRemoveTempDLL(s32 id) {
    u32 idx;

    idx = 0;

    while (idx < 3 && id != gTempDLLIds[idx]) {
        idx++;
    }

    if (idx == 3) {
        return 0;
    }

    if (gTempDLLInsts[idx] == NULL) {
        STUBBED_PRINTF(" WARNING : temp dll no %i is alreadly removed \n");
        return 0;
    }

    if (dllFree(gTempDLLInsts[idx])) {
        gTempDLLInsts[idx] = NULL;
    }

    return 1;
}

void mainLoadFrontend(void) {
    if (gDLL_76 == 0) {
        gDLL_75 = dllLoad(DLL_ID_75, 10);
        gDLL_76 = dllLoad(DLL_ID_76, 3);
    }
}

void mainUnloadFrontend(void) {
    if (gDLL_76 != 0) {
        dllFree(gDLL_75);
        gDLL_75 = 0;
        dllFree(gDLL_76);
        gDLL_76 = 0;
    }
}

void mainDemoReset(void) {
    gDemoState = 0;
    gDemoFinished = 0;
}

void mainDemoStart(f32 x, f32 y, f32 z, s32 playerno) {
    gDemoState++;

    if (gDemoState >= 5) {
        gDemoState = 0;
        gDemoFinished = 1;
    }

    mainStartGame(x, y, z, playerno);
}

s32 mainDemoNext(void) {
    s32 _v1 = gDemoState + 1;
    if (_v1 >= 5)
        _v1 = 0;
    return _v1;
}

u8 mainDemoState(void) {
    return gDemoState;
}

u8 mainDemoFinished(void) {
    return gDemoFinished;
}

void mainClearPlayerPosBuffer(void) {
    bzero(&PlayerPosBuffer, PLAYER_POSBUF_SIZE * sizeof(struct Vec3_Int));
    PlayerPosBuffer_index = 0;
}

void mainUpdatePlayerPosBuffer(void) {
    Object *player;
    struct Vec3_Int *pos;

    player = objGetPlayer();
    pos = (struct Vec3_Int *)&PlayerPosBuffer[PlayerPosBuffer_index];
    D_800AE674 += gUpdateRate;

    if (player != NULL) {
        pos->f.x = player->srt.transl.x;
        pos->f.y = player->srt.transl.y;
        pos->f.z = player->srt.transl.z;
        pos->i = D_800AE674;

        if (++PlayerPosBuffer_index >= PLAYER_POSBUF_SIZE) {
            PlayerPosBuffer_index = 0;
        }
    }
}

void mainGetBufferedPlayerPos(f32 param1, f32 *outX, f32 *outY, f32 *outZ) {
    struct Vec3_Int *pos;
    u32 var;
    s32 i;

    var = D_800AE674 - (param1 * 60.0f);

    i = PLAYER_POSBUF_SIZE;
    pos = &PlayerPosBuffer[PlayerPosBuffer_index];

    do {
        pos--;
        if (pos < &PlayerPosBuffer[0]) {
            pos = &PlayerPosBuffer[PLAYER_POSBUF_SIZE - 1];
        }

    } while (i-- && pos->i != 0 && pos->i > var);

    *outX = pos->f.x;
    *outY = pos->f.y;
    *outZ = pos->f.z;
}
