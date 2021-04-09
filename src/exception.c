#include "common.h"
#include "video.h"

#pragma GLOBAL_ASM("asm/nonmatchings/exception/update_pi_man_array.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/PiManager_thread_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/PiManagerStarter.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/func_8006213C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/crash_controller_getter.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/some_crash_print.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/print_stack_trace.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/other_crash_print.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/crash_copy_control_inputs.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/func_80062CB8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/func_80062D38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/func_80062E38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/crash_print_line.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/get_err_string.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/exception/clear_framebuffer_current.s")
#else
/**
 * Sets all values of gFramebufferCurrent to 0.
 */
void clear_framebuffer_current() {
    u32 resEncoded = get_some_resolution_encoded();
    s32 valuesLeft = (/*hRes*/resEncoded & 0xffff) * (/*vRes*/(resEncoded >> 0x10) & 0xffff);
    u16 *framebufferPtr = gFramebufferCurrent;

    while (valuesLeft--) {
        *framebufferPtr = 0;
        ++framebufferPtr;
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/exception/write_cFile_label_pointers.s")

#pragma GLOBAL_ASM("asm/nonmatchings/exception/func_800631E0.s")
