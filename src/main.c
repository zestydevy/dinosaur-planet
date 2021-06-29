#include "common.h"
#include <PR/sched.h>

void func_8001440C(s32 arg0);
void clear_PlayerPosBuffer(void);
void func_800483BC(f32, f32, s32);
void game_init(void);
void init_bittable(void);
struct DllInstance **dll_load_deferred(s32, s32);

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

void osCreateScheduler(OSSched *s, void *stack, OSPri priority, u8 mode, u8 retraceCount);

void game_init(void)
{
    struct DllInstance *temp_AMSEQ_DLL;
    s32 tvMode;
    struct DllInstance **tmp3;

    init_memory();
    three_more_mallocs();
    create_asset_thread();

    if (0) ; //debug PAL?
    else if (osTvType == OS_TV_PAL) tvMode = OS_VI_PAL_LAN1;
    else if (osTvType == OS_TV_MPAL) tvMode = OS_VI_MPAL_LAN1;
    else tvMode = OS_VI_NTSC_LAN1;

    osCreateScheduler(&osscheduler_, &osscheduler_stack, 0xD, tvMode, 1);
    PiManager_thread_func();
    init_filesystem();
    create_3_megs_quues(&osscheduler_);
    alloc_frame_buffers();
    if (0) {};
    gFrameBufIdx = 0;
    gCurGfx = gMainGfx[gFrameBufIdx];
    some_controller_init_val = init_controller_data();
    controller_thread_func(&osscheduler_);
    create_crash_thread(&osscheduler_);
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
        (*gDLL_1E)->unk4.asVoid();
    }
    init_bittable();
    alSynFlag = 1;
    start_alSyn_thread();
    func_80012224(0);
    if (0);
    gDPFullSync(gCurGfx++);
    gSPEndDisplayList(gCurGfx++);
    func_800632B0();
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
    func_80037780(gMainGfx[gFrameBufIdx], gCurGfx, 0);
    temp_t9 = gFrameBufIdx ^ 1;
    gFrameBufIdx = temp_t9;
    gCurGfx = gMainGfx[temp_t9];
    gCurMtx = gMainMtx[temp_t9];
    gCurVtx = gMainVtx[temp_t9];
    gCurPol = gMainPol[temp_t9];
    func_80063330(gCurGfx, 0, &gCurPol, 0x28E);
    func_8003CC50(&gCurGfx, 0, 0x80000000);
    func_8003CC50(&gCurGfx, 1, framebufferCurrent);
    func_8003CC50(&gCurGfx, 2, D_800BCCB4);
    func_8003E9F0(&gCurGfx, delayByte);
    func_80040FD0();
    func_8003DB5C();
    temp_a0 = D_80092A90;
    if (temp_a0->unk28 != 0) {
        temp_a0->unk28 = 0;
        gDPPipeSync(gCurGfx++);
    }
    gDPSetDepthImage(gCurGfx++, 0x02000000);
    func_80037EC8(&gCurGfx);
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
    func_80037A14(&gCurGfx, &gCurMtx, phi_v1);
    func_80007178();
    func_80013D80();
    func_800121DC();
    (*gDLL_1C)->unk4.withThreeArgs(&gCurGfx, &gCurMtx, &gCurVtx);
    (*gDLL_subtitles)->unk1C(&gCurGfx);
    func_80003CBC();
    func_800129E4();
    func_80060B94(&gCurGfx);
    gDPFullSync(gCurGfx++);
    gSPEndDisplayList(gCurGfx++);
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
    write_cFile_label_pointers(&D_8009913C, 0x37C);
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
    osscheduler_stack = 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_800142F0.s")

void func_800143A4(void) {
    func_80048034();
    osscheduler_stack = 1;
}

s32 func_800143D0(s32 *arg0) {
    *arg0 = gMainGfx[gFrameBufIdx];
    return gCurGfx;
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
#pragma GLOBAL_ASM("asm/nonmatchings/main/alloc_frame_buffers.s")
#else
void alloc_frame_buffers(void) {
    //almost matching, probably just needs correct type
    Gfx *gfx;
    Mtx *mtx;
    void **pol;
    Vtx *vtx;

    //in default.dol these have names as well.
    //alloc graphic display list command buffers. ("main:gfx" in default.dol)
    gfx = (Gfx *) malloc(MAIN_GFX_BUF_SIZE * 2 * sizeof(Gfx), ALLOC_TAG_LISTS_COL, NULL);
    gMainGfx[0] = gfx;
    gMainGfx[1] = gfx + MAIN_GFX_BUF_SIZE;

    //matrix buffers ("main:mtx")
    mtx = (Mtx *) malloc(MAIN_MTX_BUF_SIZE * 2 * sizeof(Mtx), ALLOC_TAG_LISTS_COL, NULL);
    gMainMtx[0] = mtx;
    gMainMtx[1] = mtx + MAIN_MTX_BUF_SIZE;

    //polygon buffers? ("main:pol")
    pol = (void **) malloc(0x640, ALLOC_TAG_LISTS_COL, NULL); //XXX type
    gMainPol[0] = pol;
    gMainPol[1] = pol + 200;

    //vertex buffers ("main:vtx")
    vtx = (Vtx *) malloc(MAIN_VTX_BUF_SIZE * 2 * sizeof(Vtx), ALLOC_TAG_LISTS_COL, NULL);
    gMainVtx[0] = vtx;
    gMainVtx[1] = vtx + MAIN_VTX_BUF_SIZE;
}
#endif


void func_80014508(s8 arg0) {
    D_8008C94C = arg0;
}

s32 func_80014524(void) {
    if (some_controller_init_val == -1) {
        return 1;
    } else {
        return 0;
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
    queue_alloc_load_file(&gFile_BITTABLE, FILE_BITTABLE);
    gSizeBittable = get_file_size(FILE_BITTABLE) >> 1;
    charStats_pointer = (*gDLL_gplay)->unk_88();
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainSetBits.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/mainGetBit.s")

#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014930.s")

s32 mainDecrementBit(s32 bit) {
    s32 val = mainGetBit(bit);
    if (val != 0) {
        mainSetBits(bit, --val);
        return val;
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
    bzero(&PlayerPosBuffer, PLAYER_POSBUF_SIZE * sizeof(struct Vec3_Int));
    PlayerPosBuffer_index = 0;
}

void update_PlayerPosBuffer(void)
{
    TActor * player;
    struct Vec3_Int * pos;

    player = get_player();
    pos = (struct Vec3_Int *)&PlayerPosBuffer[PlayerPosBuffer_index];
    gFrameCount += delayByte;

    if (player != NULL)
    {
        pos->f.x = player->pos.position.x;
        pos->f.y = player->pos.position.y;
        pos->f.z = player->pos.position.z;
        pos->i = gFrameCount;

        if (++PlayerPosBuffer_index >= PLAYER_POSBUF_SIZE) {
            PlayerPosBuffer_index = 0;
        }
    }
}


#pragma GLOBAL_ASM("asm/nonmatchings/main/func_80014D34.s")
