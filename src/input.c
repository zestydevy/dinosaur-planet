#include "common.h"
#include "input.h"

#if 0 
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_controller_mesg_queue_2.s")
#else
OSMesgQueue *get_controller_mesg_queue_2() {
    return &gControllerMesgQueue2;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80010200.s")
#else
void func_80010200() {
    D_800A8618 = 0xa;

    osSendMesg(&gControllerMesgQueue, (OSMesg)&D_800A8618, OS_MESG_NOBLOCK);
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80010238.s")
#else
void func_80010238() {
    D_800A8618 = 0xa;

    osSendMesg(&gControllerMesgQueue, &D_800A8618, OS_MESG_NOBLOCK);
    
    osRecvMesg(&gControllerMesgQueue3, NULL, OS_MESG_BLOCK);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/input/init_controller_data.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/controller_thread_func.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/update_controller_buffer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/label_controllerPortList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/update_controllerPortList.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/get_listed_port.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80010C24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80010C44.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_with_controller_buffer.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/get_masked_button_press.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/get_button_press.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80010DCC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/get_masked_buttonInput1.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/NOOP_80010ec0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80010EC8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80010F78.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80010FC8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_8001107C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_800110CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/get_joy_xy_sign.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_800111BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_8001124C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/set_buttonMask.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80011290.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/set_inputMenuDelayelay.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/reset_inputMenuDelayelay.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/ret1_800112d8.s")
