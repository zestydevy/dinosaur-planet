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
    D_800A8618 = 0xA;

    osSendMesg(&gControllerMesgQueue2, (OSMesg)&D_800A8618, OS_MESG_NOBLOCK);
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80010238.s")
#else
void func_80010238() {
    D_800A8618 = 0xA;

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
        joyXHoldTimer[i] = joyYHoldTimer[i] = 0;
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

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/input/controller_thread_entry.s")
#else
void controller_thread_entry(void *_) {
    s16 *contMesg;
    u32 stackVar1;

    int i; // t3 in asm
    int k; // v1 in asm

    int totalStickX;
    int totalStickY;
    
    UnkInputStruct *buf2;
    UnkInputStruct *buf1;
    UnkInputStruct *struct1;
    UnkInputStruct *struct2;

    contpad_buffer[0] = &D_800A7DC0[0];
    contpad_buffer[1] = &D_800A7E60[0];

    D_800A7DB2[0] = 0;
    D_800A7DB2[1] = 0;

    contMesg = 0;

    buttonQueue_ = 0;

    while (TRUE) {
        osRecvMesg(&gControllerMesgQueue2, (OSMesg*)&contMesg, OS_MESG_BLOCK);

        if (*contMesg != 1) {
            if (*contMesg != 0xA) { // ???
                
            }

            D_800A7DB1 = 1;
        } else {
            stackVar1 = buttonQueue_;

            if (D_800A7DB2[buttonQueue_] < MAXCONTROLLERS) {
                buf2 = contpad_buffer[buttonQueue_ ^ 1];
                buf1 = contpad_buffer[buttonQueue_];
                struct1 = &buf1[D_800A7DB2[buttonQueue_]];
                struct2 = &buf2[D_800A7DB2[buttonQueue_ ^ 1] - 1];

                D_800A7DB2[buttonQueue_] += 1;

                if ((struct1 - 1) >= buf1) {
                    struct2 = struct1 - 1;
                }

                if (osRecvMesg(&gContInterruptQueue, NULL, OS_MESG_NOBLOCK) == 0) {
                    // Queue not empty
                    osContGetReadData(struct1->unk0x0);
                    osContStartReadData(&gContInterruptQueue);
                } else {
                    // Queue empty
                    _bcopy(struct2, struct1, sizeof(UnkInputStruct));
                }

                for (i = 0; i != MAXCONTROLLERS; ++i) {
                    if (gNoControllers) {
                        struct1->unk0x0[i].button = 0;
                    }

                    struct1->unk0x18[i] = (struct1->unk0x0[i].button & (struct1->unk0x0[i].button ^ struct2->unk0x0[i].button)) & D_8008C8A4;
                    struct1->unk0x20[i] = (struct2->unk0x0[i].button & (struct1->unk0x0[i].button ^ struct2->unk0x0[i].button)) & D_8008C8A4;
                }
            }

            if (D_800A7DB1 == 0) {
                // NOTE: Could also be a separate do-while starting at the beginning of the while (TRUE)
                continue;
            }

            bzero(&gContPads[0], sizeof(OSContPad) * MAXCONTROLLERS);

            stackVar1 = buttonQueue_;
            buf1 = contpad_buffer[buttonQueue_];

            for (i = 0; i != MAXCONTROLLERS; ++i) {
                totalStickX = 0;
                totalStickY = 0;

                buttonInput0[i] = 0;
                buttonInput1[i] = 0;

                for (k = 0; k < D_800A7DB2[buttonQueue_]; ++k) {
                    buttonInput0[i] |= buf1[k].unk0x18[i];
                    buttonInput1[i] |= buf1[k].unk0x20[i];

                    gContPads[i].button |= buf1[k].unk0x0[i].button;

                    totalStickX += buf1[k].unk0x0[i].stick_x;
                    totalStickY += buf1[k].unk0x0[i].stick_y;
                }

                gContPads[i].stick_x = totalStickX / D_800A7DB2[buttonQueue_];
                gContPads[i].stick_y = totalStickY / D_800A7DB2[buttonQueue_];

                joyXSign[i] = 0;
                joyYSign[i] = 0;

                if (gContPads[i].stick_x < -35 && joyXMirror[i] > -36) {
                    joyXSign[i] = -1;
                    joyXHoldTimer[i] = 0;
                }

                if (gContPads[i].stick_x > 35 && joyXMirror[i] < 36) {
                    joyXSign[i] = 1;
                    joyXHoldTimer[i] = 0;
                }

                if (gContPads[i].stick_y < -35 && joyYMirror[i] > -36) {
                    joyYSign[i] = -1;
                    joyYHoldTimer[i] = 0;
                }

                if (gContPads[i].stick_y > 35 && joyYMirror[i] < 36) {
                    joyYSign[i] = 1;
                    joyYHoldTimer[i] = 0;
                }

                joyYMirror[i] = gContPads[i].stick_y;

                if (joyYMirror[i] < -35) {
                    joyYHoldTimer[i] += 1;
                } else {
                    if (joyYMirror[i] < 36) {
                        joyYHoldTimer[i] = 0;
                    } else {
                        joyYHoldTimer[i] += 1;
                    }
                }

                if (menuInputDelay < joyYHoldTimer[i]) {
                    joyYMirror[i] = 0;
                    joyYHoldTimer[i] = 0;
                }

                joyXMirror[i] = gContPads[i].stick_x;

                if (joyXMirror[i] < -35) {
                    joyXHoldTimer[i] += 1;
                } else {
                    if (joyXHoldTimer[i] < 36) {
                        joyXHoldTimer[i] = 0;
                    } else {
                        joyXHoldTimer[i] += 1;
                    }
                }

                if (menuInputDelay < joyXHoldTimer[i]) {
                    joyXMirror[i] = 0;
                    joyXHoldTimer[i] = 0;
                }

                buttonMask[i] = 0xffff;
            }

            gIgnoreJoystick = FALSE;
            D_800A7DB1 = 0;

            D_800A7DB2[(buttonQueue_ ^ 1) & 0xff] = 0;
            buttonQueue_ = buttonQueue_ ^ 1;

            osSendMesg(&gControllerMesgQueue3, &D_800A8618, OS_MESG_NOBLOCK);
        }
    }
}
#endif

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

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/func_with_controller_buffer.s")
#else
u16 func_with_controller_buffer(int port, int a1) {
    OSContPad *pads;
    UnkInputStruct *buffer;
    
    if (port > 0) {
        return 0;
    }

    if (a1 < 0) {
        a1 = 0;
    } else if (a1 >= D_800A7DB2[buttonQueue_ ^ 1]) {
        a1 = D_800A7DB2[buttonQueue_ ^ 1] - 1;
    }

    buffer = contpad_buffer[buttonQueue_ ^ 1];
    buffer = &buffer[a1];

    pads = buffer->unk0x0;

    return pads[controllerPortList[port]].button & buttonMask[port];
}
#endif

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

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80010DCC.s")
#else
u16 func_80010DCC(int port, int a1) {
    UnkInputStruct *buffer;
    
    if (port > 0) {
        return 0;
    }

    if (a1 < 0) {
        a1 = 0;
    } else if (a1 >= D_800A7DB2[buttonQueue_ ^ 1]) {
        a1 = D_800A7DB2[buttonQueue_ ^ 1] - 1;
    }

    buffer = contpad_buffer[buttonQueue_ ^ 1];
    buffer = &buffer[a1];

    return buffer->unk0x18[controllerPortList[port]] & buttonMask[port];
}
#endif

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

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80010EC8.s")
#else
u16 func_80010EC8(int port, int a1) {
    UnkInputStruct *buffer;
    
    if (port > 0) {
        return 0;
    }

    if (a1 < 0) {
        a1 = 0;
    } else if (a1 >= D_800A7DB2[buttonQueue_ ^ 1]) {
        a1 = D_800A7DB2[buttonQueue_ ^ 1] - 1;
    }

    buffer = contpad_buffer[buttonQueue_ ^ 1];
    buffer = &buffer[a1];

    return buffer->unk0x20[controllerPortList[port]] & buttonMask[port];
}
#endif

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

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80010FC8.s")
#else
s8 func_80010FC8(int port, int a1) {
    OSContPad *pads;
    UnkInputStruct *buffer;
    
    if (port > 0) {
        return 0;
    }

    if (a1 < 0) {
        a1 = 0;
    } else if (a1 >= D_800A7DB2[buttonQueue_ ^ 1]) {
        a1 = D_800A7DB2[buttonQueue_ ^ 1] - 1;
    }

    buffer = contpad_buffer[buttonQueue_ ^ 1];
    buffer = &buffer[a1];

    pads = buffer->unk0x0;

    return handle_stick_deadzone(pads[controllerPortList[port]].stick_x);
}
#endif

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

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/func_800110CC.s")
#else
s8 func_800110CC(int port, int a1) {
    OSContPad *pads;
    UnkInputStruct *buffer;
    
    if (port > 0) {
        return 0;
    }

    if (a1 < 0) {
        a1 = 0;
    } else if (a1 >= D_800A7DB2[buttonQueue_ ^ 1]) {
        a1 = D_800A7DB2[buttonQueue_ ^ 1] - 1;
    }

    buffer = contpad_buffer[buttonQueue_ ^ 1];
    buffer = &buffer[a1];

    pads = buffer->unk0x0;

    return handle_stick_deadzone(pads[controllerPortList[port]].stick_y);
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
 * If gIgnoreJoystick is TRUE, this will always return 0.
 * 
 * Returns a joystick axis value between [-70, 70].
 */
s8 handle_stick_deadzone(s8 stick) {
    s8 adjustedStick;

    if (gIgnoreJoystick) {
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
    gIgnoreJoystick = TRUE;
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
