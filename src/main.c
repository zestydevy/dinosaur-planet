#include "sys/main.h"

#include "PR/os.h"
#include "PR/sched.h"
#include "game/gamebits.h"
#include "libnaudio/n_unkfuncs.h"
#include "sys/asset_thread.h"
#include "sys/audio.h"
#include "sys/joypad.h"
#include "sys/crash.h"
#include "sys/exception.h"
#include "sys/fs.h"
#include "sys/gfx/map.h"
#include "sys/memory.h"
#include "sys/newshadows.h"
#include "sys/objects.h"
#include "sys/oldshadows.h"
#include "sys/print.h"
#include "sys/rarezip.h"
#include "sys/rcp.h"
#include "sys/menu.h"
#include "sys/fonts.h"
#include "sys/boot.h"
#include "sys/dl_debug.h"
#include "sys/rsp_segment.h"
#include "sys/voxmap.h"
#include "sys/framebuffer_fx.h"
#include "dll.h"
#include "constants.h"
#include "functions.h"

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
s32 D_8008C968 = -1;
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
DLL_1_UI *gDLL_1_UI = NULL;
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
DLL_33 *gDLL_33 = NULL;
DLL_59_minimap *gDLL_59_Minimap = NULL;
DLL_54 *gDLL_54 = NULL;

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
s8 D_800B09C0;
u8 gFrameBufIdx;
s8 gPauseState;
u8 gDemoState;
s8 gDemoFinished;
/* -------- .bss end 800b09d0 -------- */

void func_8001440C(s32 arg0);
void clear_PlayerPosBuffer(void);
void game_init(void);
void init_bittable(void);
void game_tick_no_expansion(void);
void game_tick(void);
void func_80014074(void);
void alloc_frame_buffers(void);
void func_80013D80(void);
s8 func_800143FC(void);
void update_PlayerPosBuffer();
void test_write(void);
void check_dongle(void);

void mainproc(void *arg) {
    test_write(); // ROM write check
    game_init();

    while(TRUE) {
        check_dongle();  // copy protection check

        if (osMemSize != EXPANSION_SIZE) {
            game_tick_no_expansion();
        } else {
            game_tick();
        }

        thread_timer_tick();
    }
}

void game_init(void) {
    s32 tvMode;

    mmInit();
    rarezip_init();
    create_asset_thread();

    if (0) {
    } else if (osTvType == OS_TV_PAL) {
        tvMode = OS_VI_PAL_LAN1;
    } else if (osTvType == OS_TV_MPAL) {
        tvMode = OS_VI_MPAL_LAN1;
    } else {
        tvMode = OS_VI_NTSC_LAN1;
    }

    osCreateScheduler(&osscheduler_, &ossceduler_stack[STACKSIZE(OS_SC_STACKSIZE)], 0xD, tvMode, 1);
    start_pi_manager_thread();
    init_filesystem();
    gfxtask_init(&osscheduler_);
    alloc_frame_buffers();
    if (0) {};
    gFrameBufIdx = 0;
    gCurGfx = gMainGfx[gFrameBufIdx];
    gLastInsertedControllerIndex = joy_init();
    joy_start_controller_thread(&osscheduler_);
    start_crash_thread(&osscheduler_);
    tex_init();
    init_maps();
    func_8001CD00();
    init_models();
    init_dll_system();
    init_objects();
    diPrintfInit();
    func_80053300();
    shadows_init();
    oldshadow_init();
    fonts_init();
    menu_init();
    init_audio(&osscheduler_, /*threadPriority=*/14);
    init_global_map();
    if (osMemSize != EXPANSION_SIZE) {
        gDLL_5_AMSEQ2 = gDLL_5_AMSEQ = dll_load_deferred(DLL_ID_AMSEQ, 36);
        gDLL_6_AMSFX = dll_load_deferred(DLL_ID_AMSFX, 18);
        gDLL_21_Gametext = dll_load_deferred(DLL_ID_TEXT, 5);
        gDLL_29_Gplay = dll_load_deferred(DLL_ID_GPLAY, 47);
        gDLL_31_Flash = dll_load_deferred(DLL_ID_FLASH, 2);
        gDLL_28_ScreenFade = dll_load_deferred(DLL_ID_SCREEN_FADE, 4);
    } else {
        gDLL_1_UI = dll_load_deferred(DLL_ID_UI, 15);
        gDLL_2_Camera = dll_load_deferred(DLL_ID_CAMERA, 23);
        gDLL_23 = dll_load_deferred(DLL_ID_23, 8);  // 0x12 in SFA
        gDLL_18_objfsa = dll_load_deferred(DLL_ID_18, 22); // 0x0F in SFA
        gDLL_3_Animation = dll_load_deferred(DLL_ID_ANIM, 29);
        gDLL_28_ScreenFade = dll_load_deferred(DLL_ID_SCREEN_FADE, 4); // 0x16 in SFA
        gDLL_25 = dll_load_deferred(DLL_ID_25, 14);                    // not present in SFA
        gDLL_7_Newday = dll_load_deferred(DLL_ID_NEWDAY, 15);
        gDLL_8 = dll_load_deferred(DLL_ID_8, 12); // 0x06 in SFA
        gDLL_9_Newclouds = dll_load_deferred(DLL_ID_NEWCLOUDS, 8);
        gDLL_10_Newstars = dll_load_deferred(DLL_ID_NEWSTARS, 3);
        gDLL_12_Minic = dll_load_deferred(DLL_ID_MINIC, 10);
        gDLL_4_Race = dll_load_deferred(DLL_ID_RACE, 13);
        gDLL_5_AMSEQ2 = gDLL_5_AMSEQ = dll_load_deferred(DLL_ID_AMSEQ, 36);
        gDLL_6_AMSFX = dll_load_deferred(DLL_ID_AMSFX, 18);
        gDLL_11_Newlfx = dll_load_deferred(DLL_ID_NEWLFX, 7);
        gDLL_13_Expgfx = dll_load_deferred(DLL_ID_EXPGFX, 10);
        gDLL_14_Modgfx = dll_load_deferred(DLL_ID_MODGFX, 12);
        gDLL_15_Projgfx = dll_load_deferred(DLL_ID_PROJGFX, 8);
        gDLL_16 = dll_load_deferred(DLL_ID_16, 3);
        gDLL_17_partfx = dll_load_deferred(DLL_ID_PARTFX, 2); // probably particle FX
        gDLL_20_Screens = dll_load_deferred(DLL_ID_SCREENS, 3);
        gDLL_21_Gametext = dll_load_deferred(DLL_ID_TEXT, 5);
        gDLL_22_Subtitles = dll_load_deferred(DLL_ID_SUBTITLES, 7);
        gDLL_24_Waterfx = dll_load_deferred(DLL_ID_WATERFX, 7);
        gDLL_26_Curves = dll_load_deferred(DLL_ID_CURVES, 38);
        gDLL_74_Picmenu = dll_load_deferred(DLL_ID_PICMENU, 7);
        gDLL_27 = dll_load_deferred(DLL_ID_27, 9); // 0x15 in SFA
        gDLL_29_Gplay = dll_load_deferred(DLL_ID_GPLAY, 36);
        gDLL_56 = dll_load_deferred(DLL_ID_56, 10); // not present in SFA
        gDLL_30_Task = dll_load_deferred(DLL_ID_TASK, 6);
        gDLL_31_Flash = dll_load_deferred(DLL_ID_FLASH, 2); // param is 0x24 in SFA
        gDLL_32 = dll_load_deferred(DLL_ID_32, 6);          // 0x18 in SFA
        gDLL_33 = dll_load_deferred(DLL_ID_33, 22);         // 0x19 in SFA
        gDLL_59_Minimap = dll_load_deferred(DLL_ID_MINIMAP, 2);
        gDLL_54 = dll_load_deferred(DLL_ID_54, 12); // 0x2F in SFA
        gDLL_57 = dll_load_deferred(DLL_ID_57, 4);
        gDLL_58 = dll_load_deferred(DLL_ID_58, 2);
        gDLL_30_Task->vtbl->load_recently_completed();
    }
    init_bittable();
    alSynFlag = 1;
    start_audio_thread();
    audio_func_80012224(0);
    if (0) {};
    gDPFullSync(gCurGfx++);
    gSPEndDisplayList(gCurGfx++);
    dl_init_debug_infos();
    menu_set(MENU_POST);
    if (osMemSize == EXPANSION_SIZE) {
        func_80014074();
    }
    func_80041D20(0);
    func_80041C6C(0);
}

void game_tick(void) {
    u8 phi_v1;
    u32 updateRate;
    Gfx **gdl;

    osSetTime(0);
    dl_next_debug_info_set();

    gdl = &gCurGfx;

    // unused return type
    gfxtask_run_xbus(gMainGfx[gFrameBufIdx], gCurGfx, 0);

    gFrameBufIdx ^= 1;
    gCurGfx = gMainGfx[gFrameBufIdx];
    gCurMtx = gMainMtx[gFrameBufIdx];
    gCurVtx = gMainVtx[gFrameBufIdx];
    gCurPol = gMainPol[gFrameBufIdx];

    dl_add_debug_info(gCurGfx, 0, "main/main.c", 0x28E);
    rsp_segment(&gCurGfx, SEGMENT_MAIN, (void *)K0BASE);
    rsp_segment(&gCurGfx, SEGMENT_FRAMEBUFFER, gFrontFramebuffer);
    rsp_segment(&gCurGfx, SEGMENT_ZBUFFER, gFrontDepthBuffer);
    fbfx_tick(&gCurGfx, gUpdateRate);
    dl_set_all_dirty();
    tex_render_reset();

    if (gDLBuilder->needsPipeSync != 0) {
        gDLBuilder->needsPipeSync = 0;
        gDPPipeSync(gCurGfx++);
    }

    gDPSetDepthImage(gCurGfx++, SEGMENT_ADDR(SEGMENT_ZBUFFER, 0));

    rsp_init(&gCurGfx);
    phi_v1 = 2;

    if (func_80041D5C() == 0)
        phi_v1 = 0;
    else if (func_80041D74() == 0)
        phi_v1 = 3;

    func_80037A14(&gCurGfx, &gCurMtx, phi_v1);
    voxmap_update_cache_timers();
    func_80013D80();
    audio_func_800121DC();
    gDLL_28_ScreenFade->vtbl->draw(gdl, &gCurMtx, &gCurVtx);
    gDLL_22_Subtitles->vtbl->func_578(gdl);
    camera_tick();
    func_800129E4();
    diPrintfAll(gdl);

    gDPFullSync(gCurGfx++);
    gSPEndDisplayList(gCurGfx++);

    gfxtask_wait();
    obj_do_deferred_free();
    mmFreeTick();

    if (gPauseState == 0) {
        func_80001A3C();
    }

    gUpdateRate = vi_frame_sync(0);
    
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

    func_80014074();
    write_c_file_label_pointers("main/main.c", 0x37C);
}

void game_tick_no_expansion(void) {
    u32 updateRate;
    Gfx **tmp_s0;

    tmp_s0 = &gCurGfx;

    gfxtask_run_xbus(gMainGfx[gFrameBufIdx], gCurGfx, 0);

    gFrameBufIdx ^= 1;
    gCurGfx = gMainGfx[gFrameBufIdx];
    gCurMtx = gMainMtx[gFrameBufIdx];
    gCurVtx = gMainVtx[gFrameBufIdx];
    gCurPol = gMainPol[gFrameBufIdx];

    rsp_segment(&gCurGfx, SEGMENT_MAIN, (void *)K0BASE);
    rsp_segment(&gCurGfx, SEGMENT_FRAMEBUFFER, gFrontFramebuffer);
    rsp_segment(&gCurGfx, SEGMENT_ZBUFFER, gFrontDepthBuffer);
    dl_set_all_dirty();
    tex_render_reset();

    if (gDLBuilder->needsPipeSync != 0) {
        gDLBuilder->needsPipeSync = 0U;
        gDPPipeSync(gCurGfx++);
    }

    gDPSetDepthImage(gCurGfx++, SEGMENT_ADDR(SEGMENT_ZBUFFER, 0x0));

    rsp_init(&gCurGfx);
    menu_update1(); // ignored return value
    menu_draw(&gCurGfx, &gCurMtx, &gCurVtx, &gCurPol);
    func_800129E4();
    gDLL_28_ScreenFade->vtbl->draw(tmp_s0, &gCurMtx, &gCurVtx);

    gDPFullSync(gCurGfx++);
    gSPEndDisplayList(gCurGfx++);

    gfxtask_wait();
    mmFreeTick();

    gUpdateRate = vi_frame_sync(0);
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

void func_80013D80(void) {
    s32 button;

    joy_set_button_mask(0, U_JPAD | R_JPAD);
    gDLL_2_Camera->vtbl->func19();
    gDLL_22_Subtitles->vtbl->func_4C0();

    if (menu_update1() == 0) {
        button = joy_get_pressed(0);

        if (gPauseState != 0) {
            draw_pause_screen_freeze_frame(&gCurGfx);
        }

        if (gPauseState == 0) {
            update_objects();
            func_80042174(0);

            if ((func_80001A2C() == 0) && (D_8008C94C == 0) && (func_800143FC() == 0) && ((button & START_BUTTON) != 0) && (main_get_bits(BIT_44F) == 0)) {
                gPauseState = 1;
                joy_set_button_mask(0, START_BUTTON);
                menu_set(MENU_PAUSE);
            }

            gDLL_29_Gplay->vtbl->tick();
        } else {
            update_obj_models();
        }

        if (gPauseState == 0) {
            update_PlayerPosBuffer();
        }

        menu_update2();
        func_800591EC();
        func_8004A67C();
        map_update_streaming();
        func_800210DC();

        gDLL_4_Race->vtbl->func14();

        if (gPauseState == 0) {
            func_8004225C(&gCurGfx, &gCurMtx, &gCurVtx, &gCurPol, &gCurVtx, &gCurPol);
        }

        gDLL_20_Screens->vtbl->draw(&gCurGfx);
        menu_draw(&gCurGfx, &gCurMtx, &gCurVtx, &gCurPol);

        D_8008C94C -= gUpdateRate;

        if ((s32)D_8008C94C < 0) {
            D_8008C94C = 0;
        }
    }
}

void func_80013FB4(void) {
    vi_init(OS_VI_PAL_LPN1, NULL, FALSE);
    func_80041D20(0);
    func_80041C6C(0);
    gDLL_5_AMSEQ->vtbl->stop(3);
    gDLL_5_AMSEQ->vtbl->stop(0);
    gDLL_5_AMSEQ->vtbl->stop(1);
    gDLL_22_Subtitles->vtbl->func_448();
    unpause();
    func_800141A4(1, 0, PLAYER_KRYSTAL, -1);
}

void func_80014074(void) {
    if (D_800B09C0 != 0) {
        mmSetDelay(0);
        if (D_8008CA30 != 0) {
            rcp_set_screen_color(0, 0, 0);
            func_800668A4();
            map_func_800484A8();

            gCurGfx = gMainGfx[gFrameBufIdx];
            gDPFullSync(gCurGfx++);
            gSPEndDisplayList(gCurGfx++);
        }

        D_800B09C0 = 0;

        mmSetDelay(0);
        camera_init();

        if (D_8008C968 >= 0) {
            menu_set(D_8008C968);
            D_8008C968 = -1;
        }

        map_func_8004773C();

        if (gDLL_23 != NULL) {
            gDLL_23->vtbl->func_18(1);
        }

        mmSetDelay(2);
        D_8008CA30 = 1;
    }
}

void func_800141A4(s32 mapID, s32 arg1, s32 playerno, s32 arg3) {
    PlayerLocation *temp_v0;

    func_8001440C(0);

    if (playerno <= PLAYER_NONE) {
        playerno = PLAYER_SABRE;
    }

    clear_PlayerPosBuffer();

    gDLL_30_Task->vtbl->load_recently_completed();
    gDLL_29_Gplay->vtbl->set_playerno(playerno);

    temp_v0 = gDLL_29_Gplay->vtbl->get_player_saved_location();

    map_func_80048054(mapID, arg1, &temp_v0->vec.x, &temp_v0->vec.y, &temp_v0->vec.z, &temp_v0->mapLayer);
    gDLL_29_Gplay->vtbl->checkpoint(&temp_v0->vec, 0, 0, temp_v0->mapLayer);

    D_800B09C0 = 1;
    D_8008C968 = arg3;
}

void func_800142A0(f32 arg0, f32 arg1, f32 arg2) {
    func_8001440C(0);
    map_func_800483BC(arg0, arg1, arg2);
    clear_PlayerPosBuffer();
    D_800B09C0 = 1;
}

void main_start_game(f32 x, f32 y, f32 z, s32 playerno) {
    Vec3f pos;
    pos.x = x;
    pos.y = y;
    pos.z = z;

    func_8001440C(0);

    gDLL_29_Gplay->vtbl->init_save(-1, NULL);
    gDLL_29_Gplay->vtbl->set_playerno(playerno);
    gDLL_29_Gplay->vtbl->checkpoint(&pos, 0, 0, 0);
    gDLL_29_Gplay->vtbl->start_loaded_game();
}

void func_800143A4(void) {
    map_func_80048034();
    D_800B09C0 = 1;
}

Gfx *func_800143D0(Gfx **arg0) {
    *arg0 = gMainGfx[gFrameBufIdx];
    return gCurGfx;
}

s8 func_800143FC(void) {
    return D_8008C940;
}

void func_8001440C(s32 arg0) {
    D_8008C940 = arg0;
}

s8 get_pause_state(void) {
    return gPauseState;
}

void unpause(void) {
    gPauseState = 0;
}

void set_pause_state(s32 state) {
    gPauseState = state;
}

#define MAIN_GFX_BUF_SIZE (sizeof(Gfx) * 4500)
#define MAIN_MTX_BUF_SIZE (sizeof(Mtx) * 1100)
#define MAIN_POL_BUF_SIZE (sizeof(Triangle) * 50)
#define MAIN_VTX_BUF_SIZE (sizeof(Vertex) * 480)

void alloc_frame_buffers(void) {
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

void func_80014508(s8 arg0) {
    D_8008C94C = arg0;
}

/**
 * @returns TRUE if no controllers are inserted.
 * @pre game_init must be called first.
 * @see game_init, joy_init
 */
s32 are_no_controllers_inserted(void) {
    if (gLastInsertedControllerIndex == -1) {
        // No controllers are inserted
        return TRUE;
    } else {
        return FALSE;
    }
}

s32 ret1_8001454c(void) {
    return 1;
}

void func_initing_rumblepak(void) {
    osContStartQuery(0);
    osContGetQuery(0);
    osMotorInit(0, 0, 0);
    _depth2Cents(0);
}

void test_write(void) {
    HW_REG2(0x1C000C02, u16) = 0x4040;
}

/*
 * Probe the copy protection dongle for the correct magic string and
 * if failed, wipe a majority of RAM to prevent RAM viewing.
 */
void check_dongle(void) {
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
    if ((head == 'LSFS') || (head == 'MPFS')) {
        return;
    } else {
        int *write = (int *)(u32)EXPANSION_RAM_START;
        // copy protection failed. Wipe every 2nd word to prevent RAM analysis.
        // probably hackers trying to view our precious data.
        while ((u32)write != 0x80000000) {
            *write = 0;
            write -= 2; // hmm...
        }
    }
}

OSSched *get_ossched(void) {
    return &osscheduler_;
}

void init_bittable(void) {
    queue_alloc_load_file((void **)&gFile_BITTABLE, 0x37);
    gSizeBittable = get_file_size(BITTABLE_BIN) >> 1;
    gGplayState = gDLL_29_Gplay->vtbl->get_state();
}

// offical name: mainSetBits
void main_set_bits(s32 entry, u32 value) {
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
            case 1: // Saved with checkpoints
                bitString = &gGplayState->save.chkpnt.bitString[0];
                break;
            case 2: // Always saved
                bitString = &gGplayState->save.file.bitString[0];
                break;
            case 3: // Saved with map saves
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
u32 main_get_bits(s32 entry) {
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
            case 1: // Saved with checkpoints
                bitString = &gGplayState->save.chkpnt.bitString[0];
                break;
            case 2: // Always saved
                bitString = &gGplayState->save.file.bitString[0];
                break;
            case 3: // Saved with map saves
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

s32 main_increment_bits(s32 entry) {
    s32 val;
    s32 maxVal;

    val = main_get_bits(entry) + 1;

    maxVal = 1 << ((gFile_BITTABLE[entry].field_0x2 & 0x1f) + 1);

    if (val < maxVal) {
        main_set_bits(entry, val);
    } else {
        val -= 1;
    }

    return val;
}

s32 main_decrement_bits(s32 entry) {
    s32 val = main_get_bits(entry);
    if (val != 0) {
        main_set_bits(entry, --val);
        return val;
    }
    return 0;
}

static const char warning1[] = " WARNING : temp dll no %i is alreadly created \n";
s32 create_temp_dll(s32 id) {
    u32 idx;

    idx = 0;

    while (idx < 3 && id != gTempDLLIds[idx]) {
        idx++;
    }

    if (idx == 3) {
        return 0;
    }

    if (gTempDLLInsts[idx] != NULL) {
    }

    gTempDLLInsts[idx] = dll_load_deferred(id, 1);

    return 1;
}

static const char warning2[] = " WARNING : temp dll no %i is alreadly removed \n";
s32 remove_temp_dll(s32 id) {
    u32 idx;

    idx = 0;

    while (idx < 3 && id != gTempDLLIds[idx]) {
        idx++;
    }

    if (idx == 3) {
        return 0;
    }

    if (gTempDLLInsts[idx] == NULL) {
        return 0;
    }

    if (dll_unload(gTempDLLInsts[idx])) {
        gTempDLLInsts[idx] = NULL;
    }

    return 1;
}

void main_load_frontend(void) {
    if (gDLL_76 == 0) {
        gDLL_75 = dll_load_deferred(DLL_ID_75, 10);
        gDLL_76 = dll_load_deferred(DLL_ID_76, 3);
    }
}

void main_unload_frontend(void) {
    if (gDLL_76 != 0) {
        dll_unload(gDLL_75);
        gDLL_75 = 0;
        dll_unload(gDLL_76);
        gDLL_76 = 0;
    }
}

void main_demo_reset(void) {
    gDemoState = 0;
    gDemoFinished = 0;
}

void main_demo_start(f32 x, f32 y, f32 z, s32 playerno) {
    gDemoState++;

    if (gDemoState >= 5) {
        gDemoState = 0;
        gDemoFinished = 1;
    }

    main_start_game(x, y, z, playerno);
}

s32 main_demo_next(void) {
    s32 _v1 = gDemoState + 1;
    if (_v1 >= 5)
        _v1 = 0;
    return _v1;
}

u8 main_demo_state(void) {
    return gDemoState;
}

u8 main_demo_finished(void) {
    return gDemoFinished;
}

void clear_PlayerPosBuffer(void) {
    bzero(&PlayerPosBuffer, PLAYER_POSBUF_SIZE * sizeof(struct Vec3_Int));
    PlayerPosBuffer_index = 0;
}

void update_PlayerPosBuffer(void) {
    Object *player;
    struct Vec3_Int *pos;

    player = get_player();
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

void func_80014D34(f32 param1, f32 *outX, f32 *outY, f32 *outZ) {
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
