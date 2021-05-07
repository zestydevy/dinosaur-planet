#include "common.h"
#include "input.h"

void func_8003B6E0(OSSched *scheduler, int*, OSMesgQueue*, int);

void controller_thread_entry(void *arg);
s8 handle_stick_deadzone(s8 stick);
void init_virtual_cont_port_map();

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

    // Set default controller virtual->physical port mapping
    init_virtual_cont_port_map();

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
        
        gButtonPresses[i] = 0;
        gButtonReleases[i] = 0;
        
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
    
    ControllersSnapshot *buf2;
    ControllersSnapshot *buf1;
    ControllersSnapshot *struct1;
    ControllersSnapshot *struct2;

    gContSnapshots[0] = &gContSnapshotsBuffer0[0];
    gContSnapshots[1] = &gContSnapshotsBuffer1[0];

    gNumBufContSnapshots[0] = 0;
    gNumBufContSnapshots[1] = 0;

    contMesg = 0;

    gPrevContSnapshotsI = 0;

    while (TRUE) {
        osRecvMesg(&gControllerMesgQueue2, (OSMesg*)&contMesg, OS_MESG_BLOCK);

        if (*contMesg != 1) {
            if (*contMesg != 0xA) { // ???
                
            }

            D_800A7DB1 = 1;
        } else {
            stackVar1 = gPrevContSnapshotsI;

            if (gNumBufContSnapshots[gPrevContSnapshotsI] < MAXCONTROLLERS) {
                buf2 = gContSnapshots[gPrevContSnapshotsI ^ 1];
                buf1 = gContSnapshots[gPrevContSnapshotsI];
                struct1 = &buf1[gNumBufContSnapshots[gPrevContSnapshotsI]];
                struct2 = &buf2[gNumBufContSnapshots[gPrevContSnapshotsI ^ 1] - 1];

                gNumBufContSnapshots[gPrevContSnapshotsI] += 1;

                if ((struct1 - 1) >= buf1) {
                    struct2 = struct1 - 1;
                }

                if (osRecvMesg(&gContInterruptQueue, NULL, OS_MESG_NOBLOCK) == 0) {
                    // Queue not empty
                    osContGetReadData(struct1->pads);
                    osContStartReadData(&gContInterruptQueue);
                } else {
                    // Queue empty
                    _bcopy(struct2, struct1, sizeof(ControllersSnapshot));
                }

                for (i = 0; i != MAXCONTROLLERS; ++i) {
                    if (gNoControllers) {
                        struct1->pads[i].button = 0;
                    }

                    struct1->buttonPresses[i] = (struct1->pads[i].button & (struct1->pads[i].button ^ struct2->pads[i].button)) & D_8008C8A4;
                    struct1->buttonReleases[i] = (struct2->pads[i].button & (struct1->pads[i].button ^ struct2->pads[i].button)) & D_8008C8A4;
                }
            }

            if (D_800A7DB1 == 0) {
                // NOTE: Could also be a separate do-while starting at the beginning of the while (TRUE)
                continue;
            }

            bzero(&gContPads[0], sizeof(OSContPad) * MAXCONTROLLERS);

            stackVar1 = gPrevContSnapshotsI;
            buf1 = gContSnapshots[gPrevContSnapshotsI];

            for (i = 0; i != MAXCONTROLLERS; ++i) {
                totalStickX = 0;
                totalStickY = 0;

                gButtonPresses[i] = 0;
                gButtonReleases[i] = 0;

                for (k = 0; k < gNumBufContSnapshots[gPrevContSnapshotsI]; ++k) {
                    gButtonPresses[i] |= buf1[k].buttonPresses[i];
                    gButtonReleases[i] |= buf1[k].buttonReleases[i];

                    gContPads[i].button |= buf1[k].pads[i].button;

                    totalStickX += buf1[k].pads[i].stick_x;
                    totalStickY += buf1[k].pads[i].stick_y;
                }

                gContPads[i].stick_x = totalStickX / gNumBufContSnapshots[gPrevContSnapshotsI];
                gContPads[i].stick_y = totalStickY / gNumBufContSnapshots[gPrevContSnapshotsI];

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

                gButtonMask[i] = 0xffff;
            }

            gIgnoreJoystick = FALSE;
            D_800A7DB1 = 0;

            gNumBufContSnapshots[(gPrevContSnapshotsI ^ 1) & 0xff] = 0;
            gPrevContSnapshotsI = gPrevContSnapshotsI ^ 1;

            osSendMesg(&gControllerMesgQueue3, &D_800A8618, OS_MESG_NOBLOCK);
        }
    }
}
#endif

#if 1
#pragma GLOBAL_ASM("asm/nonmatchings/input/init_virtual_cont_port_map.s")
#else
// Functionally equivalent
void _init_virtual_cont_port_map() {
    int i;

    for (i = 0; i < MAXCONTROLLERS; ++i) {
        gVirtualContPortMap[i] = i;
    }
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/input/update_controllerPortList.s")

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_physical_cont_port.s")
#else
/**
 * Maps the given virtual controller port to its corresponding physical port.
 */
u8 get_physical_cont_port(int virtualPort) {
    return gVirtualContPortMap[virtualPort];
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/swap_controller_port_0_and_1.s")
#else
/**
 * Swaps the first 2 virtual controller ports.
 */
void swap_controller_port_0_and_1() {
    u8 port0 = gVirtualContPortMap[0];

    gVirtualContPortMap[0] = gVirtualContPortMap[1];
    gVirtualContPortMap[1] = port0;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_masked_buttons.s")
#else
/**
 * Gets a masked bitfield of held buttons on the given controller.
 * 
 * @details gButtonMask may cause some buttons to be ignored.
 * Always returns 0 if port is not zero.
 */
u16 get_masked_buttons(int port) {
    if (port > 0) {
        return 0;
    }

    return gContPads[gVirtualContPortMap[port]].button & gButtonMask[port];
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_masked_buttons_from_buffer.s")
#else
/**
 * Gets a masked bitfield of held buttons on the given controller from the 
 * requested buffered snapshot.
 * 
 * @details Gets buttons directly from the current controller snapshots (gContSnapshots)
 * at the requested buffer index. If there are less buffered snapshots than the
 * requested index, the last available snapshot in the buffer will be used.
 * 
 * gButtonMask may cause some buttons to be ignored.
 * Always returns 0 if port is not zero.
 */
u16 get_masked_buttons_from_buffer(int port, int buffer) {
    OSContPad *pads;
    ControllersSnapshot *snapshot;
    
    if (port > 0) {
        return 0;
    }

    if (buffer < 0) {
        buffer = 0;
    } else if (buffer >= gNumBufContSnapshots[gPrevContSnapshotsI ^ 1]) {
        buffer = gNumBufContSnapshots[gPrevContSnapshotsI ^ 1] - 1;
    }

    snapshot = gContSnapshots[gPrevContSnapshotsI ^ 1];
    snapshot = &snapshot[buffer];

    pads = snapshot->pads;

    return pads[gVirtualContPortMap[port]].button & gButtonMask[port];
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_masked_button_presses.s")
#else
/**
 * Gets a masked bitfield of buttons that were just pressed on the given controller.
 * 
 * @details gButtonMask may cause some buttons to be ignored.
 * Always returns 0 if port is not zero.
 */
u16 get_masked_button_presses(int port) {
    if (port > 0) {
        return 0;
    }

    return gButtonPresses[gVirtualContPortMap[port]] & gButtonMask[port];
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_button_presses.s")
#else
/**
 * Gets a bitfield of buttons that were just pressed on the given controller.
 * 
 * @details Always returns 0 if port is not zero.
 */
u16 get_button_presses(int port) {
    if (port > 0) {
        return 0;
    }

    return gButtonPresses[gVirtualContPortMap[port]];
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_masked_button_presses_from_buffer.s")
#else
/**
 * Gets a masked bitfield of buttons that were just pressed on the given 
 * controller from the requested buffered snapshot.
 * 
 * @details Gets buttons directly from the current controller snapshots (gContSnapshots)
 * at the requested buffer index. If there are less buffered snapshots than the
 * requested index, the last available snapshot in the buffer will be used.
 * 
 * gButtonMask may cause some buttons to be ignored.
 * Always returns 0 if port is not zero.
 */
u16 get_masked_button_presses_from_buffer(int port, int buffer) {
    ControllersSnapshot *snapshot;
    
    if (port > 0) {
        return 0;
    }

    if (buffer < 0) {
        buffer = 0;
    } else if (buffer >= gNumBufContSnapshots[gPrevContSnapshotsI ^ 1]) {
        buffer = gNumBufContSnapshots[gPrevContSnapshotsI ^ 1] - 1;
    }

    snapshot = gContSnapshots[gPrevContSnapshotsI ^ 1];
    snapshot = &snapshot[buffer];

    return snapshot->buttonPresses[gVirtualContPortMap[port]] & gButtonMask[port];
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_masked_button_releases.s")
#else
/**
 * Gets a masked bitfield of buttons that were just released on the given controller.
 * 
 * @details gButtonMask may cause some buttons to be ignored.
 * Always returns 0 if port is not zero.
 */
u16 get_masked_button_releases(int port) {
    if (port > 0) {
        return 0;
    }

    return gButtonReleases[gVirtualContPortMap[port]] & gButtonMask[port];
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_masked_button_releases_from_buffer.s")
#else
/**
 * Gets a masked bitfield of buttons that were just released on the given 
 * controller from the requested buffered snapshot.
 * 
 * @details Gets buttons directly from the current controller snapshots (gContSnapshots)
 * at the requested buffer index. If there are less buffered snapshots than the
 * requested index, the last available snapshot in the buffer will be used.
 * 
 * gButtonMask may cause some buttons to be ignored.
 * Always returns 0 if port is not zero.
 */
u16 get_masked_button_releases_from_buffer(int port, int buffer) {
    ControllersSnapshot *snapshot;
    
    if (port > 0) {
        return 0;
    }

    if (buffer < 0) {
        buffer = 0;
    } else if (buffer >= gNumBufContSnapshots[gPrevContSnapshotsI ^ 1]) {
        buffer = gNumBufContSnapshots[gPrevContSnapshotsI ^ 1] - 1;
    }

    snapshot = gContSnapshots[gPrevContSnapshotsI ^ 1];
    snapshot = &snapshot[buffer];

    return snapshot->buttonReleases[gVirtualContPortMap[port]] & gButtonMask[port];
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_joystick_x.s")
#else
/**
 * Returns the joystick X value of the controller in the given port with a deadzone applied.
 * 
 * @details Always returns 0 if port is not zero.
 */
s8 get_joystick_x(int port) {
    if (port > 0) {
        return 0;
    } else {
        return handle_stick_deadzone(gContPads[gVirtualContPortMap[port]].stick_x);
    }
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_joystick_x_from_buffer.s")
#else
/**
 * Gets the joystick X value on the given controller from the 
 * requested buffered snapshot with a deadzone applied.
 * 
 * @details Gets values directly from the current controller snapshots (gContSnapshots)
 * at the requested buffer index. If there are less buffered snapshots than the
 * requested index, the last available snapshot in the buffer will be used.
 * 
 * Always returns 0 if port is not zero.
 */
s8 get_joystick_x_from_buffer(int port, int buffer) {
    OSContPad *pads;
    ControllersSnapshot *snapshot;
    
    if (port > 0) {
        return 0;
    }

    if (buffer < 0) {
        buffer = 0;
    } else if (buffer >= gNumBufContSnapshots[gPrevContSnapshotsI ^ 1]) {
        buffer = gNumBufContSnapshots[gPrevContSnapshotsI ^ 1] - 1;
    }

    snapshot = gContSnapshots[gPrevContSnapshotsI ^ 1];
    snapshot = &snapshot[buffer];

    pads = snapshot->pads;

    return handle_stick_deadzone(pads[gVirtualContPortMap[port]].stick_x);
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_joystick_y.s")
#else
/**
 * Returns the joystick Y value of the controller in the given port with a deadzone applied.
 * 
 * @details Always returns 0 if port is not zero.
 */
s8 get_joystick_y(int port) {
    if (port > 0) {
        return 0;
    } else {
        return handle_stick_deadzone(gContPads[gVirtualContPortMap[port]].stick_y);
    }
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_joystick_y_from_buffer.s")
#else
/**
 * Gets the joystick Y value on the given controller from the 
 * requested buffered snapshot with a deadzone applied.
 * 
 * @details Gets values directly from the current controller snapshots (gContSnapshots)
 * at the requested buffer index. If there are less buffered snapshots than the
 * requested index, the last available snapshot in the buffer will be used.
 * 
 * Always returns 0 if port is not zero.
 */
s8 get_joystick_y_from_buffer(int port, int buffer) {
    OSContPad *pads;
    ControllersSnapshot *snapshot;
    
    if (port > 0) {
        return 0;
    }

    if (buffer < 0) {
        buffer = 0;
    } else if (buffer >= gNumBufContSnapshots[gPrevContSnapshotsI ^ 1]) {
        buffer = gNumBufContSnapshots[gPrevContSnapshotsI ^ 1] - 1;
    }

    snapshot = gContSnapshots[gPrevContSnapshotsI ^ 1];
    snapshot = &snapshot[buffer];

    pads = snapshot->pads;

    return handle_stick_deadzone(pads[gVirtualContPortMap[port]].stick_y);
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_joystick_xy_sign.s")
#else
/**
 * Gets the sign of the X and Y joystick axes on the given controller.
 * 
 * @param[in] port The controller port.
 * @param[out] xSign The sign of the X axis.
 * @param[out] ySign The sign of the Y axis.
 */
void get_joystick_xy_sign(int port, s8 *xSign, s8 *ySign) {
    *xSign = joyXSign[gVirtualContPortMap[port]];
    *ySign = joyYSign[gVirtualContPortMap[port]];
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
/**
 * Sets the button mask for the given controller.
 * 
 * @details Bits not set in the mask will result in those corresponding buttons
 * to be ignored.
 */
void set_button_mask(int port, u16 mask) {
    gButtonMask[port] &= ~mask;
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
