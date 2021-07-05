#include "common.h"
#include <PR/sched.h>
#include "crash.h"
#include "input.h"

void func_8001440C(s32 arg0);
void clear_PlayerPosBuffer(void);
void func_800483BC(f32, f32, s32);
void game_init(void);
void init_bittable(void);
struct DLLInstance **dll_load_deferred(s32, s32);

const char gameVer[] = "1.3623";
const char curentTime[] = "01/12/00 09:19";
const char gameName[] = "dragon1";
const char fileName[] = "main/main.c";
const char fileName2[] = "main/main.c";

const char warning1[] = " WARNING : temp dll no %i is alreadly created \n";
const char warning2[] = " WARNING : temp dll no %i is alreadly removed \n";

void mainproc(void * arg)
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

void osCreateScheduler(OSSched *s, void *stack, OSPri priority, u8 mode, u8 retreceCount);

void game_init(void) 
{
    struct DLLInstance **temp_AMSEQ_DLL;
    s32 tvMode;
    struct DLLInstance **tmp3;

    init_memory();
    three_more_mallocs();
    create_asset_thread();

    if (0) {
    } else if (osTvType == OS_TV_PAL) {
        tvMode = OS_VI_PAL_LAN1;
    } else if (osTvType == OS_TV_MPAL) {
        tvMode = OS_VI_MPAL_LAN1;
    } else {
        tvMode = OS_VI_NTSC_LAN1;
    }

    osCreateScheduler(&osscheduler_, &ossceduler_stack, 0xD, tvMode, 1);
    start_pi_manager_thread();
    init_filesystem();
    create_3_megs_quues(&osscheduler_);
    four_mallocs();
    if (0);
    D_800B09C1 = 0;
    D_800AE680 = D_800AE678[D_800B09C1];
    gLastInsertedControllerIndex = init_controller_data();
    start_controller_thread(&osscheduler_);
    start_crash_thread(&osscheduler_);
    init_textures();
    init_maps();
    func_8001CD00();
    init_models();
    init_dll_system();
    init_objects();
    func_80060A80();
    func_80053300();
    func_8004D470();
    func_8005C780();
    init_fonts();
    init_menu_related_globals();
    init_audio(&osscheduler_, 0xE);
    init_global_map();
    if (osMemSize != EXPANSION_SIZE) {
        temp_AMSEQ_DLL = dll_load_deferred(DLL_AMSEQ, 0x24);
        gDLL_AMSEQ2    = gDLL_AMSEQ = temp_AMSEQ_DLL;
        gDLL_AMSFX     = dll_load_deferred(DLL_AMSFX, 0x12);
        gDLL_text      = dll_load_deferred(DLL_TEXT, 5);
        gDLL_gplay     = dll_load_deferred(DLL_GPLAY, 0x2F);
        gDLL_savegame  = dll_load_deferred(DLL_SAVEGAME, 2);
        gDLL_1C        = dll_load_deferred(0x1C, 4);
    } else {
        gDLL_UI        = dll_load_deferred(DLL_UI, 0xF);
        gDLL_Camera    = dll_load_deferred(DLL_CAMERA, 0x17);
        gDLL_17        = dll_load_deferred(0x17, 8); //0x12 in SFA
        gDLL_12        = dll_load_deferred(0x12, 0x16); //0x0F in SFA
        gDLL_ANIM      = dll_load_deferred(DLL_ANIM, 0x1D);
        gDLL_1C        = dll_load_deferred(0x1C, 4); //0x16 in SFA
        gDLL_19        = dll_load_deferred(0x19, 0xE); //not present in SFA
        gDLL_Sky       = dll_load_deferred(DLL_SKY, 0xF);
        gDLL_08        = dll_load_deferred(8, 0xC); //0x06 in SFA
        gDLL_newclouds = dll_load_deferred(DLL_NEWCLOUDS, 8);
        gDLL_newstars  = dll_load_deferred(DLL_NEWSTARS, 3);
        gDLL_minic     = dll_load_deferred(DLL_MINIC, 0xA);
        gDLL_Race      = dll_load_deferred(DLL_RACE, 0xD);
        temp_AMSEQ_DLL = dll_load_deferred(DLL_AMSEQ, 0x24);
        gDLL_AMSEQ2    = gDLL_AMSEQ = temp_AMSEQ_DLL;
        gDLL_AMSFX     = dll_load_deferred(DLL_AMSFX, 0x12);
        gDLL_newlfx    = dll_load_deferred(DLL_NEWLFX, 7);
        gDLL_expgfx    = dll_load_deferred(DLL_EXPGFX, 0xA);
        gDLL_modgfx    = dll_load_deferred(DLL_MODGFX, 0xC);
        gDLL_projgfx   = dll_load_deferred(DLL_PROJGFX, 8);
        gDLL_10        = dll_load_deferred(0x10, 3);
        gDLL_11        = dll_load_deferred(0x11, 2); //probably particle FX
        gDLL_SCREENS   = dll_load_deferred(DLL_SCREENS, 3);
        gDLL_text      = dll_load_deferred(DLL_TEXT, 5);
        gDLL_subtitles = dll_load_deferred(DLL_SUBTITLES, 7);
        gDLL_waterfx   = dll_load_deferred(DLL_WATERFX, 7);
        gDLL_CURVES    = dll_load_deferred(DLL_CURVES, 0x26);
        gDLL_Link      = dll_load_deferred(DLL_LINK, 7);
        gDLL_1B        = dll_load_deferred(0x1B, 9); //0x15 in SFA
        gDLL_gplay     = dll_load_deferred(DLL_GPLAY, 0x24);
        gDLL_38        = dll_load_deferred(0x38, 0xA); //not present in SFA
        gDLL_1E        = dll_load_deferred(0x1E, 6);
        gDLL_savegame  = dll_load_deferred(DLL_SAVEGAME, 2); //param is 0x24 in SFA
        gDLL_20        = dll_load_deferred(0x20, 6); //0x18 in SFA
        gDLL_21        = dll_load_deferred(0x21, 0x16); //0x19 in SFA
        gDLL_3B        = dll_load_deferred(DLL_MINIMAP, 2);
        gDLL_36        = dll_load_deferred(0x36, 0xC); //0x2F in SFA
        gDLL_39        = dll_load_deferred(0x39, 4);
        gDLL_3A        = dll_load_deferred(0x3A, 2);
        (*gDLL_1E)->func[0].asVoid();
    }
    init_bittable();
    alSynFlag = 1;
    start_alSyn_thread();
    func_80012224(0);
    if (0);
    gDPFullSync(D_800AE680++);
    gSPEndDisplayList(D_800AE680++);
    dl_init_debug_infos();
    set_menu_page(2);
    if (osMemSize == EXPANSION_SIZE) {
        func_80014074();
    }
    func_80041D20(0);
    func_80041C6C(0);
}

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/main/game_tick.s")
#else
extern u32 *D_8009913C;
void _game_tick(void) {
    u8 sp27;
    f32 temp_f0;
    u8 temp_t9;
    u8 temp_v0_5;
    void *temp_a0;
    u8 phi_v1;

    osSetTime(0);
    func_80063300();
    func_80037780(D_800AE678[D_800B09C1], D_800AE680, 0);
    temp_t9 = D_800B09C1 ^ 1;
    D_800B09C1 = temp_t9;
    D_800AE680 = D_800AE678[temp_t9];
    D_800AE690 = D_800AE688[temp_t9];
    D_800AE6A0 = D_800AE698[temp_t9];
    D_800AE6B0 = D_800AE6A8[temp_t9]);
    dl_add_debug_info(D_800AE680, 0, &D_80099130, 0x28E);
    func_8003CC50(&D_800AE680, 0, 0x80000000);
    func_8003CC50(&D_800AE680, 1, gFramebufferCurrent);
    func_8003CC50(&D_800AE680, 2, D_800BCCB4);
    func_8003E9F0(&D_800AE680, delayByte);
    func_80040FD0();
    func_8003DB5C();
    temp_a0 = D_80092A90;
    if (temp_a0->unk28 != 0) {
        temp_a0->unk28 = 0;
        gDPPipeSync(D_800AE680++);
    }
    gDPSetDepthImage(D_800AE680++, 0x02000000);
    func_80037EC8(&D_800AE680);
    sp27 = 2;
    if (func_80041D5C() == 0) {
        phi_v1 = 0;
    } else {
        sp27 = 2;
        phi_v1 = 2;
        if (func_80041D74() == 0) {
            phi_v1 = 3;
        }
    }
    func_80037A14(&D_800AE680, &D_800AE690, phi_v1);
    func_80007178();
    func_80013D80();
    func_800121DC();
    (*D_8008C974)->unk4.withThreeArgs(&D_800AE680, &D_800AE690, &D_800AE6A0);
    (*gDLL_subtitles)->unk1C(&D_800AE680);
    func_80003CBC();
    func_800129E4();
    func_80060B94(&D_800AE680);
    gDPFullSync(D_800AE680++);
    gSPEndDisplayList(D_800AE680++);
    func_80037924();
    func_80020BB8();
    update_mem_mon_values();
    if (D_800B09C2 == 0) {
        func_80001A3C();
    }
    temp_v0_5 = video_func_returning_delay(0);
    delayByte = temp_v0_5;
    if (temp_v0_5 >= 7) {
        delayByte = 6;
    }
    delayFloat = delayByte;
    temp_f0 = delayFloat;
    inverseDelay = 1.0f / temp_f0;
    delayByteMirror = delayByte;
    delayFloatMirror = temp_f0;
    inverseDelayMirror = 1.0f / delayFloatMirror;
    func_80014074(&delayFloatMirror);
    write_c_file_label_pointers(&D_8009913C, 0x37C);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/main/game_tick_no_expansion.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80013D80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80013FB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014074.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800141A4.s")

void func_800142A0(f32 arg0, f32 arg1, s32 arg2) {
    func_8001440C(0);
    func_800483BC(arg0, arg1, arg2);
    clear_PlayerPosBuffer();
    ossceduler_stack = 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800142F0.s")

void func_800143A4(void) {
    func_80048034();
    ossceduler_stack = 1;
}

s32 func_800143D0(s32 *arg0) {
    *arg0 = D_800AE678[D_800B09C1];
    return D_800AE680;
}

s8 func_800143FC(void) {
    return D_8008C940;
}

void func_8001440C(s32 arg0) {
    D_8008C940 = arg0;
}

s8 func_8001441C(void) {
    return D_800B09C2;
}

void func_8001442C(void) {
    D_800B09C2 = 0;
}

void func_8001443C(s32 arg0) {
    D_800B09C2 = arg0;
}

// These smell like structs
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/main/four_mallocs.s")
#else
void _four_mallocs(void) {
    u8 *temp_v0;
    s32 tmp2;

    temp_v0 = (u8 *) malloc(0x11940, 1, 0);
    D_800AE678[0] = temp_v0;
    D_800AE678[1] = temp_v0 + 0x8CA0;
    temp_v0 = (u8 *) malloc(0x22600, 1, 0);
    D_800AE688[0] = temp_v0;
    D_800AE688[1] = temp_v0 + 0x11300;
    temp_v0 = (u8 *) malloc(0x640, 1, 0);
    D_800AE6A8[0] = temp_v0;
    D_800AE6A8[1] = temp_v0 + 0x320;
    temp_v0 = (u8 *) malloc(0x2580, 1, 0);
    D_800AE698[0] = temp_v0;
    D_800AE698[1] = temp_v0 + 0x12C0;
}
#endif


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
    func_800683E0(0);
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

void init_bittable(void) 
{
    queue_alloc_load_file(&gFile_BITTABLE, 0x37);
    gSizeBittable = get_file_size(BITTABLE_BIN) >> 1;
    charStats_pointer = (*gDLL_gplay)->func[0x21].asVoidS32();
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014670.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800147EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014930.s")

s32 func_800149AC(s32 arg0) {
    s32 temp_v0 = func_800147EC(arg0);

    if (temp_v0 != 0) {
        func_80014670(arg0, --temp_v0);
        return temp_v0;
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800149F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014A80.s")

void func_80014B1C(void) {
    if (gDLL_4C == 0) {
        gDLL_4B = dll_load_deferred(0x4B, 0xA);
        gDLL_4C = dll_load_deferred(0x4C, 3);
    }
}

void func_80014B6C(void) {
    if (gDLL_4C != 0) {
        func_8000C258(gDLL_4B);
        gDLL_4B = 0;
        func_8000C258(gDLL_4C);
        gDLL_4C = 0;
    }
}

void func_80014BBC(void) {
    D_800B09C3 = 0;
    D_800B09C4 = 0;
}

void func_800142F0(s32, s32, s32);

void func_80014BD4(s32 arg0, s32 arg1, s32 arg2) {
    D_800B09C3++;

    if (D_800B09C3 >= 5) {
        D_800B09C3 = 0;
        D_800B09C4 = 1;
    }
    func_800142F0(arg0, arg1, arg2);
    if (arg2) {}; // fake match
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
    bzero(&PlayerPosBuffer, 0x3C0);
    PlayerPosBuffer_index = 0;
}

void update_PlayerPosBuffer(void)
{
    TActor * player;
    struct Vec3_Int * pos;

    player = (TActor *)get_player();
    pos = (struct Vec3_Int *)&PlayerPosBuffer[PlayerPosBuffer_index];
    D_800AE674 += delayByte;

    if (player != NULL)
    {
        pos->f.x = player->srt.transl.x;
        pos->f.y = player->srt.transl.y;
        pos->f.z = player->srt.transl.z;
        pos->i = D_800AE674;
        
        if (++PlayerPosBuffer_index >= 0x3C) {
            PlayerPosBuffer_index = 0;
        }
    }
}


#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014D34.s")