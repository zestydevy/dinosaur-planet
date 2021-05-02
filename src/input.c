#include "common.h"
#include "input.h"

void func_8003B6E0(OSSched *scheduler, int*, OSMesgQueue*, int);

void controller_thread_entry(void *arg);
s8 handle_stick_deadzone(s8 stick);
void setup_controller_port_list();

#if 0 
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_controller_interrupt_queue.s")
#else
/**
 * @returns The message queue associated with SI controller interrupts.
 * This is the same message queue that is passed to osContInit.
 */
OSMesgQueue *get_controller_interrupt_queue() {
    return &gContInterruptQueue;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80010200.s")
#else
void func_80010200() {
    D_800A8618 = 0xa;

    osSendMesg(&gControllerMesgQueue2, (OSMesg)&D_800A8618, OS_MESG_NOBLOCK);
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80010238.s")
#else
void func_80010238() {
    D_800A8618 = 0xa;

    osSendMesg(&gControllerMesgQueue2, (OSMesg)&D_800A8618, OS_MESG_NOBLOCK);

    osRecvMesg(&gControllerMesgQueue3, NULL, OS_MESG_BLOCK);
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/init_controller_data.s")
#else
s32 init_controller_data() {
    s32 lastControllerIndex;
    s32 i;
    // Bits 0-3 specify which controllers are inserted
    u8 contBitpattern;

    // Initialize SI settings
    //
    // osContInit requires a message queue associated with SI interrupts
    osCreateMesgQueue(
        &gContInterruptQueue, 
        &gContInterruptBuffer[0],
        CONT_INTERRUPT_BUFFER_LENGTH);

    osSetEventMesg(OS_EVENT_SI, &gContInterruptQueue, gContInterruptMessage);

    osContInit(&gContInterruptQueue, &contBitpattern, &gContStatuses[0]);
    
    // Start reading controller input
    osContStartReadData(&gContInterruptQueue);

    // Setup controller port mapping
    // TODO: verify this comment
    setup_controller_port_list();

    gNoControllers = FALSE;

    // Initialize gContPads memory
    bzero(&gContPads[0], sizeof(OSContPad) * MAXCONTROLLERS);

    // TODO: comment this
    menuInputDelay = 5;

    // Initialize controller input globals and determine how many controllers are inserted
    lastControllerIndex = -1;

    for (i = 0; i != MAXCONTROLLERS; ++i) {
        joyXMirror[i] = joyYMirror[i] = 0;
        joyYHoldTimer[i] = joyXHoldTimer[i] = 0;
        joyXSign[i] = joyYSign[i] = 0;
        
        buttonInput0[i] = 0;
        buttonInput1[i] = 0;
        
        // Note: Must check bitpattern and errno to determine if a controller is inserted
        if ((contBitpattern & (1 << i)) && !(gContStatuses[i].errno & CONT_NO_RESPONSE_ERROR)) {
            lastControllerIndex = i;
        }
    }

    if (lastControllerIndex == -1) {
        gNoControllers = TRUE;
    }

    return lastControllerIndex;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/start_controller_thread.s")
#else
void start_controller_thread(OSSched *scheduler) {
    osCreateMesgQueue(
        /*mq*/      &gControllerMesgQueue2, 
        /*msg*/     &gControllerMesgQueue2Buffer[0], 
        /*count*/   CONTROLLER_MESG_BUFFER_2_LENGTH
    );

    func_8003B6E0(scheduler, &D_800A8608, &gControllerMesgQueue2, 2);

    osCreateMesgQueue(
        /*mq*/      &gControllerMesgQueue3, 
        /*msg*/     &gControllerMesgQueue3Buffer[0], 
        /*count*/   CONTROLLER_MESG_BUFFER_3_LENGTH
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
// Functionally equivalent
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
// get_virtual/physical_port ?
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

    return gContPads[controllerPortList[port]].button & buttonMask[port];
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
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_masked_button_input1.s")
#else
u16 get_masked_button_input1(int port) {
    if (port > 0) {
        return 0;
    }

    return buttonInput1[controllerPortList[port]] & buttonMask[port];
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80010EC8.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_controller_stick_x.s")
#else
/**
 * Returns the joystick X value of the controller in the given port with a deadzone applied.
 */
s8 get_controller_stick_x(int port) {
    if (port > 0) {
        return 0;
    } else {
        return handle_stick_deadzone(gContPads[controllerPortList[port]].stick_x);
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80010FC8.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_controller_stick_y.s")
#else
/**
 * Returns the joystick Y value of the controller in the given port with a deadzone applied.
 */
s8 get_controller_stick_y(int port) {
    if (port > 0) {
        return 0;
    } else {
        return handle_stick_deadzone(gContPads[controllerPortList[port]].stick_y);
    }
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/input/func_800110CC.s")
#else
// Kinda close, but missing something important
s8 _func_800110CC(int port, int a1) {
    int bufferIndex;

    if (port > 0) {
        return 0;
    } else {
        if (a1 < 0) {
            a1 = 0;
            bufferIndex = buttonQueue_[0] ^ 1;
        } else {
            bufferIndex = buttonQueue_[0] ^ 1;

            if (a1 >= buttonQueue_[bufferIndex]) {
                a1 = buttonQueue_[bufferIndex] - 1;
            }
        }

        return handle_stick_deadzone(
            contpad_buffer[bufferIndex][a1].unk0x0[controllerPortList[port]].stick_y);
    }
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_joy_xy_sign.s")
#else
void get_joy_xy_sign(int port, s8 *xSign, s8 *ySign) {
    *xSign = joyXSign[controllerPortList[port]];
    *ySign = joyYSign[controllerPortList[port]];
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/handle_stick_deadzone.s")
#else
/**
 * Applies a lower deadzone of -8,8 and upper deadzone of -70,70 to a joystick axis input.
 * 
 * For example:
 * - If stick is between -8 and 8, the returned value is 0
 * - If stick is greater than 8 or less than -8, the returned value will be moved towards
 *   zero by 8 and clamped at an upper value of 70 and a lower value of -70.
 * 
 * Returns a joystick axis value between [-70, 70].
 */
s8 handle_stick_deadzone(s8 stick) {
    s8 adjustedStick;

    if (D_8008C8B0 != 0) {
        return 0;
    }

    if (stick < 8 && stick > -8) {
        return 0;
    }

    if (stick > 0) {
        stick = stick - 8;

        if (stick > 70) {
            stick = 70;
        }

        adjustedStick = stick;
    } else {
        stick = stick + 8;

        if (stick < -70) {
            stick = -70;
        }

        adjustedStick = stick;
    }

    return adjustedStick;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/func_8001124C.s")
#else
void func_8001124C() {
    D_8008C8A4 = 0;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/set_button_mask.s")
#else
void set_button_mask(int index, u16 mask) {
    buttonMask[index] &= ~mask;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80011290.s")
#else
void func_80011290(int _) {
    D_8008C8B0 = 1;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/set_input_menu_delay.s")
#else
void set_input_menu_delay(s8 delay) {
    menuInputDelay = delay;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/reset_input_menu_delay.s")
#else
void reset_input_menu_delay() {
    menuInputDelay = 5;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/ret1_800112d8.s")
#else
int ret1_800112d8() {
    return 1;
}
#endif
