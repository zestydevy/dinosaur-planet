#include "common.h"
#include "input.h"

void controller_thread_entry(void *arg);
void func_8003B6E0(OSSched *scheduler, int*, OSMesgQueue*, int);

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

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/start_controller_thread.s")
#else
void start_controller_thread(OSSched *scheduler) {
    osCreateMesgQueue(
        /*mq*/      &gControllerMesgQueue, 
        /*msg*/     &gControllerMesgQueue_Array[0], 
        /*count*/   CONTROLLER_MESG_QUEUE_LENGTH
    );

    func_8003B6E0(scheduler, &D_800A8608, &gControllerMesgQueue, 2);

    osCreateMesgQueue(
        /*mq*/      &gControllerMesgQueue3, 
        /*msg*/     &gControllerMesgQueue3_Array[0], 
        /*count*/   CONTROLLER_MESG_QUEUE_3_LENGTH
    );

    // Create and start controller thread
    osCreateThread(
        /*t*/       &gControllerThread, 
        /*id*/      CONTROLLER_THREAD_ID, 
        /*entry*/   &controller_thread_entry, 
        /*arg*/     NULL, 
        /*sp*/      &gControllerThreadStack[CONTROLLER_THREAD_STACKSIZE], 
        /*pri*/     12
    );

    osStartThread(&gControllerThread);
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/input/controller_thread_entry.s")

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/input/setup_controller_port_list.s")
#else
void _setup_controller_port_list() {
    int i;

    for (i = 0; i < MAXCONTROLLERS; ++i) {
        controllerPortList[i] = i;
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/input/update_controllerPortList.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_listed_port.s")
#else
u8 get_listed_port(int port) {
    return controllerPortList[port];
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/swap_controller_port_0_and_1.s")
#else
void swap_controller_port_0_and_1() {
    u8 port0 = controllerPortList[0];

    controllerPortList[0] = controllerPortList[1];
    controllerPortList[1] = port0;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80010C44.s")
#else
u16 func_80010C44(int port) {
    if (port > 0) {
        return 0;
    }

    return D_800a7f88[controllerPortList[port]].unk0x0 & buttonMask[port];
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_with_controller_buffer.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_masked_button_press.s")
#else
u16 get_masked_button_press(int port) {
    if (port > 0) {
        return 0;
    }

    return buttonInput0[controllerPortList[port]] & buttonMask[port];
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_button_press.s")
#else
u16 get_button_press(int port) {
    if (port > 0) {
        return 0;
    }

    return buttonInput0[controllerPortList[port]];
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80010DCC.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_masked_buttonInput1.s")
#else
u16 get_masked_buttonInput1(int port) {
    if (port > 0) {
        return 0;
    }

    return buttonInput1[controllerPortList[port]] & buttonMask[port];
}
#endif

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
