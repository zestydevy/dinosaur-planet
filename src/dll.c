#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/dll/init_dll_system.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dll/func_8000BD1C.s")

void some_crash_setter(s32 arg0, s32 arg1) {
    D_800A7D14 = arg1;
    D_800A7D10 = arg0;
}

s32 func_8000BDE8(s32 *arg0) {
    *arg0 = (s32) D_800A7D14;
    return D_800A7D10;
}

#pragma GLOBAL_ASM("asm/nonmatchings/dll/dll_load_deferred.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dll/dll_load.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dll/func_8000C0B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dll/func_8000C258.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/dll/func_8000C3BC.s")
#else
s32 func_8000C3BC(void) { // tf??? intentional crash? this makes no sense, doesnt compile, should we leave as asm?
    *NULL = (u8)0;
    return 0;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/dll/dll_load_from_tab.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dll/dll_relocate.s")

#pragma GLOBAL_ASM("asm/nonmatchings/dll/func_8000C648.s")
