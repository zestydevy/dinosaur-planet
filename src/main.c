#include "sys/main.h"

#include "PR/os.h"
#include "PR/sched.h"
#include "libnaudio/n_unkfuncs.h"
#include "sys/asset_thread.h"
#include "sys/controller.h"
#include "sys/crash.h"
#include "sys/exception.h"
#include "sys/fs.h"
#include "sys/gfx/map.h"
#include "sys/memory.h"
#include "sys/objects.h"
#include "sys/print.h"
#include "sys/rarezip.h"
#include "sys/menu.h"
#include "sys/fonts.h"
#include "sys/boot.h"
#include "sys/dl_debug.h"
#include "dll.h"
#include "constants.h"
#include "functions.h"
#include "bss.h"

typedef struct {
    // Start bit index
    u16 start;
    // bits 0-4: length in bits (minus 1)
    // bit 5: has task
    // bits 6-7: which bitstring
    u8 field_0x2;
    u8 task;
} BitTableEntry;

// .rodata
const char gameVer[] = "1.3623";
const char curentTime[] = "01/12/00 09:19";
const char gameName[] = "dragon1";
const char fileName[] = "main/main.c";
const char fileName2[] = "main/main.c";

static const char warning1[] = " WARNING : temp dll no %i is alreadly created \n";
static const char warning2[] = " WARNING : temp dll no %i is alreadly removed \n";

// .data
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
u8 delayByte = 1;
u8 delayByteMirror = 1;
f32 delayFloat = 1.0f;
f32 delayFloatMirror = 1.0f;
f32 inverseDelay = 1.0f;
f32 inverseDelayMirror = 1.0f;
s32 D_8008C968 = -1;
u8 alSynFlag = 0;
s32 D_8008C970 = 0;
DLLInst_28_screen_fade *gDLL_28_screen_fade = NULL;
DLLInst_2_Camera *gDLL_Camera = NULL;
DLLInst_Unknown *gDLL_ANIM = NULL;
DLLInst_Unknown *gDLL_Sky = NULL;
DLLInst_Unknown *gDLL_8 = NULL;
DLLInst_Unknown *gDLL_newclouds = NULL;
DLLInst_Unknown *gDLL_newstars = NULL;
DLLInst_Unknown *gDLL_minic = NULL;
DLLInst_Unknown *gDLL_UI = NULL;
DLLInst_Unknown *gDLL_Race = NULL;
DLLInst_Unknown *gDLL_AMSEQ = NULL;
DLLInst_Unknown *gDLL_AMSEQ2 = NULL;
DLLInst_6_AMSFX *gDLL_AMSFX = NULL;
DLLInst_Unknown *gDLL_newlfx = NULL;
DLLInst_Unknown *gDLL_57 = NULL;
DLLInst_Unknown *gDLL_58 = NULL;
DLLInst_Unknown *gDLL_expgfx = NULL;
DLLInst_Unknown *gDLL_modgfx = NULL;
DLLInst_Unknown *gDLL_projgfx = NULL;
DLLInst_Unknown *gDLL_16 = NULL;
DLLInst_Unknown *gDLL_17 = NULL;
DLLInst_Unknown *gDLL_18 = NULL;
DLLInst_20_screens *gDLL_20_screens = NULL;
DLLInst_21_gametext *gDLL_21_gametext = NULL;
DLLInst_Unknown *gDLL_subtitles = NULL;
DLLInst_Unknown *gDLL_23 = NULL;
DLLInst_Unknown *gDLL_waterfx = NULL;
DLLInst_Unknown *gDLL_25 = NULL;
DLLInst_Unknown *gDLL_CURVES = NULL;
DLLInst_Unknown *gDLL_Link = NULL;
DLLInst_Unknown *gDLL_75 = NULL;
DLLInst_Unknown *gDLL_27 = NULL;
DLLInst_29_gplay *gDLL_29_gplay = NULL;
DLLInst_Unknown *gDLL_56 = NULL;
DLLInst_30_task *gDLL_30_task = NULL;
DLLInst_31_flash *gDLL_31_flash = NULL;
DLLInst_76 *gDLL_76 = NULL;
DLLInst_Unknown *gDLL_32 = NULL;
DLLInst_Unknown *gDLL_33 = NULL;
DLLInst_Unknown *gDLL_minimap = NULL;
DLLInst_Unknown *gDLL_54 = NULL;
s32 gTempDLLIds[3] = { -1, 53, 55 };
void *gTempDLLInsts[3] = { NULL, NULL, NULL };
u8 D_8008CA30 = 0;

// .bss (800ae2a0)
BSS_GLOBAL GplayStruct7 *gGplayState;
BSS_GLOBAL BitTableEntry *gFile_BITTABLE;
BSS_GLOBAL s16 gSizeBittable;
BSS_GLOBAL struct Vec3_Int PlayerPosBuffer[60]; //seems to buffer player coords with "timestamp"
BSS_GLOBAL s32 PlayerPosBuffer_index;
BSS_GLOBAL u32 D_800AE674;
BSS_GLOBAL Gfx *gMainGfx[2];
BSS_GLOBAL Gfx *gCurGfx;
BSS_GLOBAL Mtx *gMainMtx[2];
BSS_GLOBAL Mtx *gCurMtx;
BSS_GLOBAL Vertex *gMainVtx[2];
BSS_GLOBAL Vertex *gCurVtx;
BSS_GLOBAL Triangle *gMainPol[2];
BSS_GLOBAL Triangle *gCurPol;
BSS_GLOBAL OSSched osscheduler_;
BSS_GLOBAL u8 ossceduler_stack[OS_SC_STACKSIZE];
BSS_GLOBAL s8 D_800B09C0;
BSS_GLOBAL u8 gFrameBufIdx;
BSS_GLOBAL s8 gPauseState;
BSS_GLOBAL u8 D_800B09C3;
BSS_GLOBAL s8 D_800B09C4;

void func_8001440C(s32 arg0);
void clear_PlayerPosBuffer(void);
void game_init(void);
void init_bittable(void);
void game_tick_no_expansion(void);
void game_tick(void);
void func_80014074(void);
void alloc_frame_buffers(void);
void func_80013D80();
s8 func_800143FC();
void update_PlayerPosBuffer();

void mainproc(void *arg)
{
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

void game_init(void)
{
    DLLInst_Unknown *temp_AMSEQ_DLL;
    s32 tvMode;
    DLLInst_Unknown *tmp3;

    init_memory();
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

    osCreateScheduler(&osscheduler_, &ossceduler_stack[OS_SC_STACKSIZE], 0xD, tvMode, 1);
    start_pi_manager_thread();
    init_filesystem();
    create_3_megs_quues(&osscheduler_);
    alloc_frame_buffers();
    if (0) {};
    gFrameBufIdx = 0;
    gCurGfx = gMainGfx[gFrameBufIdx];
    gLastInsertedControllerIndex = init_controller_data();
    start_controller_thread(&osscheduler_);
    start_crash_thread(&osscheduler_);
    init_textures();
    init_maps();
    func_8001CD00();
    init_models();
    init_dll_system();
    init_objects();
    diPrintfInit();
    func_80053300();
    func_8004D470();
    func_8005C780();
    fonts_init();
    menu_init();
    init_audio(&osscheduler_, 0xE);
    init_global_map();
    if (osMemSize != EXPANSION_SIZE) {
        temp_AMSEQ_DLL = dll_load_deferred(DLL_AMSEQ, 36);
        gDLL_AMSEQ2    = gDLL_AMSEQ = temp_AMSEQ_DLL;
        gDLL_AMSFX     = dll_load_deferred(DLL_AMSFX, 18);
        gDLL_21_gametext = dll_load_deferred(DLL_TEXT, 5);
        gDLL_29_gplay  = dll_load_deferred(DLL_GPLAY, 47);
        gDLL_31_flash  = dll_load_deferred(DLL_FLASH, 2);
        gDLL_28_screen_fade = dll_load_deferred(DLL_SCREEN_FADE, 4);
    } else {
        gDLL_UI        = dll_load_deferred(DLL_UI, 15);
        gDLL_Camera    = dll_load_deferred(DLL_CAMERA, 23);
        gDLL_23        = dll_load_deferred(23, 8); //0x12 in SFA
        gDLL_18        = dll_load_deferred(18, 0x16); //0x0F in SFA
        gDLL_ANIM      = dll_load_deferred(DLL_ANIM, 29);
        gDLL_28_screen_fade = dll_load_deferred(DLL_SCREEN_FADE, 4); //0x16 in SFA
        gDLL_25        = dll_load_deferred(25, 0xE); //not present in SFA
        gDLL_Sky       = dll_load_deferred(DLL_SKY, 15);
        gDLL_8        = dll_load_deferred(8, 12); //0x06 in SFA
        gDLL_newclouds = dll_load_deferred(DLL_NEWCLOUDS, 8);
        gDLL_newstars  = dll_load_deferred(DLL_NEWSTARS, 3);
        gDLL_minic     = dll_load_deferred(DLL_MINIC, 10);
        gDLL_Race      = dll_load_deferred(DLL_RACE, 13);
        temp_AMSEQ_DLL = dll_load_deferred(DLL_AMSEQ, 36);
        gDLL_AMSEQ2    = gDLL_AMSEQ = temp_AMSEQ_DLL;
        gDLL_AMSFX     = dll_load_deferred(DLL_AMSFX, 18);
        gDLL_newlfx    = dll_load_deferred(DLL_NEWLFX, 7);
        gDLL_expgfx    = dll_load_deferred(DLL_EXPGFX, 10);
        gDLL_modgfx    = dll_load_deferred(DLL_MODGFX, 12);
        gDLL_projgfx   = dll_load_deferred(DLL_PROJGFX, 8);
        gDLL_16        = dll_load_deferred(16, 3);
        gDLL_17        = dll_load_deferred(17, 2); //probably particle FX
        gDLL_20_screens = dll_load_deferred(DLL_SCREENS, 3);
        gDLL_21_gametext = dll_load_deferred(DLL_TEXT, 5);
        gDLL_subtitles = dll_load_deferred(DLL_SUBTITLES, 7);
        gDLL_waterfx   = dll_load_deferred(DLL_WATERFX, 7);
        gDLL_CURVES    = dll_load_deferred(DLL_CURVES, 38);
        gDLL_Link      = dll_load_deferred(DLL_LINK, 7);
        gDLL_27        = dll_load_deferred(27, 9); //0x15 in SFA
        gDLL_29_gplay  = dll_load_deferred(DLL_GPLAY, 36);
        gDLL_56        = dll_load_deferred(56, 10); //not present in SFA
        gDLL_30_task   = dll_load_deferred(DLL_TASK, 6);
        gDLL_31_flash  = dll_load_deferred(DLL_FLASH, 2); //param is 0x24 in SFA
        gDLL_32        = dll_load_deferred(32, 6); //0x18 in SFA
        gDLL_33        = dll_load_deferred(33, 22); //0x19 in SFA
        gDLL_minimap   = dll_load_deferred(DLL_MINIMAP, 2);
        gDLL_54        = dll_load_deferred(54, 12); //0x2F in SFA
        gDLL_57        = dll_load_deferred(57, 4);
        gDLL_58        = dll_load_deferred(58, 2);
        gDLL_30_task->exports->load_recently_completed();
    }
    init_bittable();
    alSynFlag = 1;
    start_alSyn_thread();
    func_80012224(0);
    if (0) {};
    gDPFullSync(gCurGfx++);
    gSPEndDisplayList(gCurGfx++);
    dl_init_debug_infos();
    menu_set(MENU_2);
    if (osMemSize == EXPANSION_SIZE) {
        func_80014074();
    }
    func_80041D20(0);
    func_80041C6C(0);
}

void game_tick(void)
{
    u8 phi_v1;
    u32 delayAmount;
    Gfx **tmp_s0;

    osSetTime(0);
    dl_next_debug_info_set();

    tmp_s0 = &gCurGfx;

    // unused return type
    schedule_gfx_task(gMainGfx[gFrameBufIdx], gCurGfx, 0);

    gFrameBufIdx ^= 1;
    gCurGfx = gMainGfx[gFrameBufIdx];
    gCurMtx = gMainMtx[gFrameBufIdx];
    gCurVtx = gMainVtx[gFrameBufIdx];
    gCurPol = gMainPol[gFrameBufIdx];

    dl_add_debug_info(gCurGfx, 0, fileName, 0x28E);
    dl_segment(&gCurGfx, 0, (void*)0x80000000);
    dl_segment(&gCurGfx, 1, gFramebufferCurrent);
    dl_segment(&gCurGfx, 2, D_800BCCB4);
    func_8003E9F0(&gCurGfx, delayByte);
    dl_set_all_dirty();
    func_8003DB5C();

    if (gDLBuilder->needsPipeSync != 0) {
        gDLBuilder->needsPipeSync = 0U;
        gDPPipeSync(gCurGfx++);
    }

    gDPSetDepthImage(gCurGfx++, 0x02000000);

    func_80037EC8(&gCurGfx);
    phi_v1 = 2U;

    if (func_80041D5C() == 0)
        phi_v1 = 0U;
    else if (func_80041D74() == 0)
        phi_v1 = 3U;

    func_80037A14(&gCurGfx, &gCurMtx, phi_v1);
    func_80007178();
    func_80013D80();
    func_800121DC();
    gDLL_28_screen_fade->exports->draw(tmp_s0, &gCurMtx, &gCurVtx);
    gDLL_subtitles->exports->func[6].withOneArg((s32)tmp_s0);
    camera_tick();
    func_800129E4();
    diPrintfAll(tmp_s0); 

    gDPFullSync(gCurGfx++);
    gSPEndDisplayList(gCurGfx++);

    func_80037924();
    obj_do_deferred_free();
    update_mem_mon_values();
    
    if (gPauseState == 0) {
        func_80001A3C();
    }

    delayByte = video_func_returning_delay(0);
    
    if (0) {}

    delayAmount = (u32) delayByte;
    if (delayByte >= 7) {
        delayByte = 6;
        delayAmount = delayByte;
    }
    delayFloat = (f32) delayAmount;
    inverseDelay = 1.0f / delayFloat;
    delayByteMirror = (s8) delayAmount;
    delayFloatMirror = delayFloat;
    inverseDelayMirror = 1.0f / delayFloatMirror;

    func_80014074();
    write_c_file_label_pointers(fileName2, 0x37C);
}

void game_tick_no_expansion(void)
{
    u32 delayAmount;
    Gfx **tmp_s0;

    tmp_s0 = &gCurGfx;
    
    schedule_gfx_task(gMainGfx[gFrameBufIdx], gCurGfx, 0);
    
    gFrameBufIdx ^= 1;
    gCurGfx = gMainGfx[gFrameBufIdx];
    gCurMtx = gMainMtx[gFrameBufIdx];
    gCurVtx = gMainVtx[gFrameBufIdx];
    gCurPol = gMainPol[gFrameBufIdx];
    
    dl_segment(&gCurGfx, 0, (void*)0x80000000);
    dl_segment(&gCurGfx, 1, gFramebufferCurrent);
    dl_segment(&gCurGfx, 2, D_800BCCB4);
    dl_set_all_dirty();
    func_8003DB5C();

    if (gDLBuilder->needsPipeSync != 0) {
        gDLBuilder->needsPipeSync = 0U;
        gDPPipeSync(gCurGfx++);
    }

    gDPSetDepthImage(gCurGfx++, 0x02000000);

    func_80037EC8(&gCurGfx);
    menu_update1(); // ignored return value
    menu_draw(&gCurGfx, &gCurMtx, &gCurVtx, &gCurPol);
    func_800129E4();
    gDLL_28_screen_fade->exports->draw(tmp_s0, &gCurMtx, &gCurVtx);

    gDPFullSync(gCurGfx++);
    gSPEndDisplayList(gCurGfx++);

    func_80037924();
    update_mem_mon_values();

    delayByte = video_func_returning_delay(0);
    delayAmount = (u8)delayByte;
    if ((s32)delayAmount >= 7) {
        delayByte = 6;
        delayAmount = (u8)delayByte;
    }
    delayFloat = (f32) delayAmount;
    inverseDelay = 1.0f / delayFloat;
    delayByteMirror = delayAmount;
    delayFloatMirror = delayFloat;
    inverseDelayMirror = 1.0f / delayFloatMirror;
}

void func_80013D80(void)
{
    s32 button;

    set_button_mask(0, U_JPAD | R_JPAD);
    gDLL_Camera->exports->func19.asVoid();
    gDLL_subtitles->exports->func[5].asVoid();

    if (menu_update1() == 0)
    {
        button = get_masked_button_presses(0);

        if (gPauseState != 0) {
            draw_pause_screen_freeze_frame(&gCurGfx);
        }

        if (gPauseState == 0)
        {
            update_objects();
            func_80042174(0);

            if ((func_80001A2C() == 0) && (D_8008C94C == 0) && (func_800143FC() == 0) && ((button & START_BUTTON) != 0) && (get_gplay_bitstring(1103) == 0))
            {
                gPauseState = 1;
                set_button_mask(0, START_BUTTON);
                menu_set(MENU_8);
            }
            
            gDLL_29_gplay->exports->func_115C();
        
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

        gDLL_Race->exports->func[14].asVoid();

        if (gPauseState == 0) {
            func_8004225C(&gCurGfx, &gCurMtx, &gCurVtx, &gCurPol, &gCurVtx, &gCurPol);
        }

        gDLL_20_screens->exports->draw(&gCurGfx);
        menu_draw(&gCurGfx, &gCurMtx, &gCurVtx, &gCurPol);

        D_8008C94C -= delayByte;

        if ((s32) D_8008C94C < 0) {
            D_8008C94C = 0;
        }
    }
}

void func_80013FB4(void) {
    func_8005D410(OS_VI_PAL_LPN1, NULL, FALSE);
    func_80041D20(0);
    func_80041C6C(0);
    gDLL_AMSEQ->exports->func[6].withOneArg(3);
    gDLL_AMSEQ->exports->func[6].withOneArg(0);
    gDLL_AMSEQ->exports->func[6].withOneArg(1);
    gDLL_subtitles->exports->func[4].asVoid();
    unpause();
    func_800141A4(1, 0, 1, -1);
}

void func_80014074(void)
{
    if (D_800B09C0 != 0)
    {
        func_80017254(0);
        if (D_8008CA30 != 0)
        {
            func_8003798C(0, 0, 0);
            func_800668A4();
            func_800484A8();

            gCurGfx = gMainGfx[gFrameBufIdx];
            gDPFullSync(gCurGfx++);
            gSPEndDisplayList(gCurGfx++);
        }
        
        D_800B09C0 = 0;
        
        func_80017254(0);
        camera_init();

        if (D_8008C968 >= 0) {
            menu_set(D_8008C968);
            D_8008C968 = -1;
        }
        
        func_8004773C();
        
        if (gDLL_23 != NULL) {
            gDLL_23->exports->func[2].withOneArg(1);
        }
        
        func_80017254(2);
        D_8008CA30 = 1;
    }
}

void func_800141A4(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    GplayStruct5 *temp_v0;

    func_8001440C(0);

    if (arg2 < 0) {
        arg2 = 0;
    }

    clear_PlayerPosBuffer();

    gDLL_30_task->exports->load_recently_completed();
    gDLL_29_gplay->exports->func_EAC(arg2);

    temp_v0 = gDLL_29_gplay->exports->func_F04();

    func_80048054(arg0, arg1, &temp_v0->vec.x, &temp_v0->vec.y, &temp_v0->vec.z, &temp_v0->unk0xD);
    gDLL_29_gplay->exports->func_958(&temp_v0->vec, 0, 0, temp_v0->unk0xD);

    D_800B09C0 = 1;
    D_8008C968 = arg3;
}

void func_800142A0(f32 arg0, f32 arg1, f32 arg2) {
    func_8001440C(0);
    func_800483BC(arg0, arg1, arg2);
    clear_PlayerPosBuffer();
    D_800B09C0 = 1;
}

void func_800142F0(f32 x, f32 y, f32 z, s32 arg3)
{
    Vec3f pos;
    pos.x = x;
    pos.y = y;
    pos.z = z;

    func_8001440C(0);

    gDLL_29_gplay->exports->func_198(-1, 0);
    gDLL_29_gplay->exports->func_EAC(arg3);
    gDLL_29_gplay->exports->func_958(&pos, 0, 0, 0);
    gDLL_29_gplay->exports->func_AE0();
}

void func_800143A4(void) {
    func_80048034();
    D_800B09C0 = 1;
}

Gfx* func_800143D0(Gfx **arg0) {
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

void alloc_frame_buffers(void)
{
    // in default.dol these have names as well.
    // alloc graphic display list command buffers. ("main:gfx" in default.dol)
    gMainGfx[0] = malloc(MAIN_GFX_BUF_SIZE * 2, ALLOC_TAG_LISTS_COL, NULL);
    gMainGfx[1] = (Gfx*)((u32)gMainGfx[0] + MAIN_GFX_BUF_SIZE);

    // matrix buffers ("main:mtx")
    gMainMtx[0] = malloc(MAIN_MTX_BUF_SIZE * 2, ALLOC_TAG_LISTS_COL, NULL);
    gMainMtx[1] = (Mtx*)((u32)gMainMtx[0] + MAIN_MTX_BUF_SIZE);

    // polygon buffers? ("main:pol")
    gMainPol[0] = malloc(MAIN_POL_BUF_SIZE * 2, ALLOC_TAG_LISTS_COL, NULL);
    gMainPol[1] = (Triangle*)((u32)gMainPol[0] + MAIN_POL_BUF_SIZE);

    // vertex buffers ("main:vtx")
    gMainVtx[0] = malloc(MAIN_VTX_BUF_SIZE * 2, ALLOC_TAG_LISTS_COL, NULL);
    gMainVtx[1] = (Vertex*)((u32)gMainVtx[0] + MAIN_VTX_BUF_SIZE);
}

void func_80014508(s8 arg0) {
    D_8008C94C = arg0;
}

/**
 * @returns TRUE if no controllers are inserted.
 * @pre game_init must be called first.
 * @see game_init, init_controller_data
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

/*
 *
*/
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
    if((head == 'LSFS') || (head == 'MPFS')) {
        return;
    }
    else {
        int *write = (int *)(u32)EXPANSION_RAM_START;
        // copy protection failed. Wipe every 2nd word to prevent RAM analysis.
        // probably hackers trying to view our precious data.
        while((u32)write != 0x80000000) {
            *write = 0;
            write -= 2; // hmm...
        }
    }
}

OSSched *get_ossched(void) {
    return &osscheduler_;
}

void init_bittable(void) {
    queue_alloc_load_file((void**)&gFile_BITTABLE, 0x37);
    gSizeBittable = get_file_size(BITTABLE_BIN) >> 1;
    gGplayState = gDLL_29_gplay->exports->func_E74();
}

// offical name: mainSet ?
void set_gplay_bitstring(s32 entry, u32 value) {
    u8 *bitString;
    u8 _pad[12]; // fake match
    s32 idx;
    s32 mask;
    s32 endBit;
    s32 startBit;

    if (entry != 149 && entry != 150 && entry != -1) {
        switch (gFile_BITTABLE[entry].field_0x2 >> 6) {
            case 0:
                bitString = &gGplayState->bitString[0];
                break;
            case 1:
                bitString = &gGplayState->unk0.unk0.bitString[0];
                break;
            case 2:
                bitString = &gGplayState->unk0.unk0.unk0.bitString[0];
                break;
            case 3:
                bitString = &gGplayState->unk0.bitString[0];
                break;
        }

        if (gFile_BITTABLE[entry].field_0x2 & (1 << 5)) {
            gDLL_30_task->exports->mark_task_completed(gFile_BITTABLE[entry].task);
        }

        startBit = gFile_BITTABLE[entry].start;
        endBit = (gFile_BITTABLE[entry].field_0x2 & 0x1f) + 1;
        mask = 1;

        for (idx = startBit; idx < (startBit + endBit); idx++) {
            if (mask & value) {
                *(u8*)((u32)bitString + (idx >> 3)) |= (1 << (idx & 7));
            } else {
                *(u8*)((u32)bitString + (idx >> 3)) &= ~(1 << (idx & 7));
            }

            mask = mask << 1;
        }
    }
}

// offical name: mainGet ?
u32 get_gplay_bitstring(s32 entry) {
    u8 *bitString;
    u32 value;
    s32 idx;
    s32 mask;
    s32 endBit;
    s32 startBit;

    if (entry == 149) {
        return 1;
    } else if (entry == 150) {
        return 0;
    } else if (entry == -1) {
        return 0;
    } else if (entry < 0 || entry >= gSizeBittable) {
        return 0;
    } else {
        switch (gFile_BITTABLE[entry].field_0x2 >> 6) {
            case 0:
                bitString = &gGplayState->bitString[0];
                break;
            case 1:
                bitString = &gGplayState->unk0.unk0.bitString[0];
                break;
            case 2:
                bitString = &gGplayState->unk0.unk0.unk0.bitString[0];
                break;
            case 3:
                bitString = &gGplayState->unk0.bitString[0];
                break;
        }

        startBit = gFile_BITTABLE[entry].start;
        endBit = (gFile_BITTABLE[entry].field_0x2 & 0x1f) + 1;
        value = 0;
        mask = 1;

        for (idx = startBit; idx < (startBit + endBit); idx++) {
            // A clever way to read from bitString bit by bit 
            if ((*(u8*)((u32)bitString + (idx >> 3)) & (1 << (idx & 7))) != 0) {
                value |= mask;
            }

            mask = mask << 1;
        }
    }

    return value;
}

s32 increment_gplay_bitstring(s32 entry) {
    s32 val;
    s32 maxVal;

    val = get_gplay_bitstring(entry) + 1;

    maxVal = 1 << ((gFile_BITTABLE[entry].field_0x2 & 0x1f) + 1);

    if (val < maxVal) {
        set_gplay_bitstring(entry, val);
    } else {
        val -= 1;
    }

    return val;
}

s32 decrement_gplay_bitstring(s32 entry) {
    s32 val = get_gplay_bitstring(entry);
    if (val != 0) {
        set_gplay_bitstring(entry, --val);
        return val;
    }
    return 0;
}

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

void func_80014B1C(void) {
    if (gDLL_76 == 0) {
        gDLL_75 = dll_load_deferred(75, 10);
        gDLL_76 = dll_load_deferred(76, 3);
    }
}

void func_80014B6C(void) {
    if (gDLL_76 != 0) {
        dll_unload(gDLL_75);
        gDLL_75 = 0;
        dll_unload(gDLL_76);
        gDLL_76 = 0;
    }
}

void func_80014BBC(void) {
    D_800B09C3 = 0;
    D_800B09C4 = 0;
}

void func_80014BD4(f32 arg0, f32 arg1, f32 arg2, s32 arg3)
{
    D_800B09C3++;

    if (D_800B09C3 >= 5) {
        D_800B09C3 = 0;
        D_800B09C4 = 1;
    }

    func_800142F0(arg0, arg1, arg2, arg3);
}

s32 func_80014C28(void)
{
    s32 _v1 = D_800B09C3 + 1;
    if (_v1 >= 5)
        _v1 = 0;
    return _v1;
}

u8 func_80014C50(void)
{
    return D_800B09C3;
}

u8 func_80014C60(void)
{
    return D_800B09C4;
}

void clear_PlayerPosBuffer(void)
{
    bzero(&PlayerPosBuffer, PLAYER_POSBUF_SIZE * sizeof(struct Vec3_Int));
    PlayerPosBuffer_index = 0;
}

void update_PlayerPosBuffer(void)
{
    Object *player;
    struct Vec3_Int * pos;

    player = get_player();
    pos = (struct Vec3_Int *)&PlayerPosBuffer[PlayerPosBuffer_index];
    D_800AE674 += delayByte;

    if (player != NULL)
    {
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
