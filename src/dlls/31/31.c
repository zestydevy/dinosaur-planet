#include <PR/ultratypes.h>
#include <PR/os.h>

static u8 idk[0x70];
static OSPiHandle OSPiHandle_81083fb8;

s32 dll_31_osFlashSectorErase(u32 page_num);
s32 dll_31_osFlashWriteArray(u32 page_num);

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/31/dll_31_ctor.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/31/dll_31_dtor.s")
//dll_031.Save_func_0000_LE
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/31/dll_31_func_6C.s")
//dll_031.Save_func_0001_LE
#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/31/dll_31_func_224.s")
#else
s32 dll_31_func_224(u8 param1, u32 param2, s32 param3, s32 param4) {

}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/31/dll_31_func_404.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/31/dll_31_osFlashInit.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/31/dll_31_func_638.s")
#else
static void dll_31_func_638() { }
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/31/dll_31_osFlashReadStatus.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/31/dll_31_osFlashReadId.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/31/dll_31_osFlashClearStatus.s")
#else
static void dll_31_osFlashClearStatus() {
    osEPiWriteIo(&OSPiHandle_81083fb8, OSPiHandle_81083fb8.baseAddress | 0x10000, 0xd2000000);
    osEPiWriteIo(&OSPiHandle_81083fb8, OSPiHandle_81083fb8.baseAddress, 0);
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/31/dll_31_osFlashSectorErase.s")
#else
s32 dll_31_osFlashSectorErase(u32 page_num) {
    u32 data;

    osEPiWriteIo(&OSPiHandle_81083fb8, OSPiHandle_81083fb8.baseAddress | 0x10000, page_num | 0x4b000000);
    osEPiWriteIo(&OSPiHandle_81083fb8, OSPiHandle_81083fb8.baseAddress | 0x10000, 0x78000000);

    do {
        osEPiReadIo(&OSPiHandle_81083fb8, OSPiHandle_81083fb8.baseAddress, &data);
    } while ((data & 2) == 2);

    osEPiReadIo(&OSPiHandle_81083fb8, OSPiHandle_81083fb8.baseAddress, &data);

    dll_31_osFlashClearStatus();

    if ((data & 0xff) == 8 || (data & 0xff) == 0x48 || (data & 0x8) == 8) {
        return 0;
    } else {
        return -1;
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/31/dll_31_osFlashWriteBuffer.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dlls/31/dll_31_osFlashWriteArray.s")
#else
s32 dll_31_osFlashWriteArray(u32 page_num) {
    u32 data;

    osEPiWriteIo(&OSPiHandle_81083fb8, OSPiHandle_81083fb8.baseAddress | 0x10000, page_num | 0xa5000000);

    do {
        osEPiReadIo(&OSPiHandle_81083fb8, OSPiHandle_81083fb8.baseAddress, &data);
    } while ((data & 1) == 1);

    osEPiReadIo(&OSPiHandle_81083fb8, OSPiHandle_81083fb8.baseAddress, &data);

    dll_31_osFlashClearStatus();

    if ((data & 0xff) == 4 || (data & 0xff) == 0x44 || (data & 0x4) == 4) {
        return 0;
    } else {
        return -1;
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/dlls/31/dll_31_osFlashReadArray.s")
