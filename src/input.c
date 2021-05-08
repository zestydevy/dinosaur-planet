#include "common.h"
#include "input.h"

void func_8003B6E0(OSSched *scheduler, int*, OSMesgQueue*, int);

void controller_thread_entry(void *arg);
s8 handle_joystick_deadzone(s8 stick);
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

    // Initialize controller input globals and determine how many controllers are inserted
    gNoControllers = FALSE;

    bzero(&gContPads[0], sizeof(OSContPad) * MAXCONTROLLERS);

    gMenuJoystickDelay = 5;
    lastControllerIndex = -1;

    for (i = 0; i != MAXCONTROLLERS; ++i) {
        gLastJoyX[i] = gLastJoyY[i] = 0;
        gMenuJoyXHoldTimer[i] = gMenuJoyYHoldTimer[i] = 0;
        gMenuJoyXSign[i] = gMenuJoyYSign[i] = 0;
        
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
// Needs work, but isn't too far off
// May have minor errors still
void controller_thread_entry(void *_) {
    s16 *contMesg;
    u32 stackVar1;

    int i; // t3 in asm
    int k; // v1 in asm

    int totalStickX;
    int totalStickY;
    
    ControllersSnapshot *curSnaps;
    ControllersSnapshot *nextSnaps;
    ControllersSnapshot *nextSnap;
    ControllersSnapshot *curSnap;

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

            // Read available controller inputs into a buffer of snapshots
            if (gNumBufContSnapshots[gPrevContSnapshotsI] < MAX_BUFFERED_CONT_SNAPSHOTS) {
                curSnaps = gContSnapshots[gPrevContSnapshotsI ^ 1];
                nextSnaps = gContSnapshots[gPrevContSnapshotsI];
                nextSnap = &nextSnaps[gNumBufContSnapshots[gPrevContSnapshotsI]];
                curSnap = &curSnaps[gNumBufContSnapshots[gPrevContSnapshotsI ^ 1] - 1];

                gNumBufContSnapshots[gPrevContSnapshotsI] += 1;

                if ((nextSnap - 1) >= nextSnaps) {
                    curSnap = nextSnap - 1;
                }

                if (osRecvMesg(&gContInterruptQueue, NULL, OS_MESG_NOBLOCK) == 0) {
                    // Queue not empty
                    osContGetReadData(nextSnap->pads);
                    osContStartReadData(&gContInterruptQueue);
                } else {
                    // Queue empty
                    _bcopy(curSnap, nextSnap, sizeof(ControllersSnapshot));
                }

                for (i = 0; i != MAXCONTROLLERS; ++i) {
                    // If no controllers are inserted, assume no buttons are pressed
                    if (gNoControllers) {
                        nextSnap->pads[i].button = 0;
                    }

                    // Calculate which buttons were just pressed/released by comparing the next/current
                    // button inputs
                    nextSnap->buttonPresses[i] = (nextSnap->pads[i].button & (nextSnap->pads[i].button ^ curSnap->pads[i].button)) & D_8008C8A4;
                    nextSnap->buttonReleases[i] = (curSnap->pads[i].button & (nextSnap->pads[i].button ^ curSnap->pads[i].button)) & D_8008C8A4;
                }
            }

            if (D_800A7DB1 == 0) {
                // NOTE: Could also be a separate do-while starting at the beginning of the while (TRUE)
                continue;
            }

            // Clear cont pads since we're about to update them
            bzero(&gContPads[0], sizeof(OSContPad) * MAXCONTROLLERS);

            stackVar1 = gPrevContSnapshotsI;
            nextSnaps = gContSnapshots[gPrevContSnapshotsI];

            for (i = 0; i != MAXCONTROLLERS; ++i) {
                totalStickX = 0;
                totalStickY = 0;

                gButtonPresses[i] = 0;
                gButtonReleases[i] = 0;

                // Combine each buffered snapshot into a single controller state
                for (k = 0; k < gNumBufContSnapshots[gPrevContSnapshotsI]; ++k) {
                    gButtonPresses[i] |= nextSnaps[k].buttonPresses[i];
                    gButtonReleases[i] |= nextSnaps[k].buttonReleases[i];

                    gContPads[i].button |= nextSnaps[k].pads[i].button;

                    totalStickX += nextSnaps[k].pads[i].stick_x;
                    totalStickY += nextSnaps[k].pads[i].stick_y;
                }

                gContPads[i].stick_x = totalStickX / gNumBufContSnapshots[gPrevContSnapshotsI];
                gContPads[i].stick_y = totalStickY / gNumBufContSnapshots[gPrevContSnapshotsI];

                // Handle menu joystick signs by setting gMenuJoy*Sign to the held direction for one
                // input frame once initially and again every gMenuJoystickDelay input frames.
                gMenuJoyXSign[i] = 0;
                gMenuJoyYSign[i] = 0;

                if (gContPads[i].stick_x < -35 && gLastJoyX[i] > -36) {
                    gMenuJoyXSign[i] = -1;
                    gMenuJoyXHoldTimer[i] = 0;
                }

                if (gContPads[i].stick_x > 35 && gLastJoyX[i] < 36) {
                    gMenuJoyXSign[i] = 1;
                    gMenuJoyXHoldTimer[i] = 0;
                }

                if (gContPads[i].stick_y < -35 && gLastJoyY[i] > -36) {
                    gMenuJoyYSign[i] = -1;
                    gMenuJoyYHoldTimer[i] = 0;
                }

                if (gContPads[i].stick_y > 35 && gLastJoyY[i] < 36) {
                    gMenuJoyYSign[i] = 1;
                    gMenuJoyYHoldTimer[i] = 0;
                }

                gLastJoyY[i] = gContPads[i].stick_y;

                if (gLastJoyY[i] < -35) {
                    gMenuJoyYHoldTimer[i] += 1;
                } else {
                    if (gLastJoyY[i] < 36) {
                        gMenuJoyYHoldTimer[i] = 0;
                    } else {
                        gMenuJoyYHoldTimer[i] += 1;
                    }
                }

                if (gMenuJoystickDelay < gMenuJoyYHoldTimer[i]) {
                    gLastJoyY[i] = 0;
                    gMenuJoyYHoldTimer[i] = 0;
                }

                gLastJoyX[i] = gContPads[i].stick_x;

                if (gLastJoyX[i] < -35) {
                    gMenuJoyXHoldTimer[i] += 1;
                } else {
                    if (gMenuJoyXHoldTimer[i] < 36) {
                        gMenuJoyXHoldTimer[i] = 0;
                    } else {
                        gMenuJoyXHoldTimer[i] += 1;
                    }
                }

                if (gMenuJoystickDelay < gMenuJoyXHoldTimer[i]) {
                    gLastJoyX[i] = 0;
                    gMenuJoyXHoldTimer[i] = 0;
                }

                // Reset button mask to allow all buttons
                gButtonMask[i] = 0xffff;
            }

            // Reset ignore joystick
            gIgnoreJoystick = FALSE;

            // TODO:
            D_800A7DB1 = 0;

            // Reset buffered snapshot count (since we've handled them all)
            gNumBufContSnapshots[(gPrevContSnapshotsI ^ 1) & 0xff] = 0;

            // Swap snapshot arrays
            gPrevContSnapshotsI = gPrevContSnapshotsI ^ 1;

            // TODO:
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

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/update_virtual_cont_port_map.s")
#else
/**
 * TODO: Not sure what the intent of this function is, but here's some example in/outs:
 * ports = [3, 0, 2, 1]
 * gVirtualContPortMap = [0, 2, 3, 1]
 * 
 * ports = [0, 1, 2, 3]
 * gVirtualContPortMap = [1, 2, 3, 0]
 * 
 * ports = [1, 3, 2, 0]
 * gVirtualContPortMap = [0, 1, 2, 3]
 */
void update_virtual_cont_port_map(s8 ports[MAXCONTROLLERS]) {
    int i;
    int k;

    k = 0;

    for (i = 0; i < MAXCONTROLLERS; ++i) {
        if (ports[i] != 0) {
            gVirtualContPortMap[k] = i;
            k += 1;
        }
    }

    for (i = 0; i < MAXCONTROLLERS; ++i) {
        if (ports[i] == 0) {
            gVirtualContPortMap[k] = i;
            k += 1;
        }
    }
}
#endif

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
        return handle_joystick_deadzone(gContPads[gVirtualContPortMap[port]].stick_x);
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

    return handle_joystick_deadzone(pads[gVirtualContPortMap[port]].stick_x);
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
        return handle_joystick_deadzone(gContPads[gVirtualContPortMap[port]].stick_y);
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

    return handle_joystick_deadzone(pads[gVirtualContPortMap[port]].stick_y);
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/get_joystick_menu_xy_sign.s")
#else
/**
 * Gets the sign of the X and Y joystick axes of the given controller when menu movement should occur.
 * 
 * @details When the user holds a joystick in a given direction, the sign of that direction
 * can be retrieved here for one input frame initially and again every menu joystick delay frames.
 * This effectively allows the user to move between menu selections by holding the joystick
 * without selections changing too quickly.
 * 
 * @param[in] port The controller port.
 * @param[out] xSign The sign of the X axis, non-zero if X-axis movement should occur.
 * @param[out] ySign The sign of the Y axis, non-zero if Y-axis movement should occur..
 * 
 * @see set_menu_joystick_delay
 */
void get_joystick_menu_xy_sign(int port, s8 *xSign, s8 *ySign) {
    *xSign = gMenuJoyXSign[gVirtualContPortMap[port]];
    *ySign = gMenuJoyYSign[gVirtualContPortMap[port]];
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/handle_joystick_deadzone.s")
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
s8 handle_joystick_deadzone(s8 stick) {
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
 * to be ignored. The button mask will be reset to 0xFFFF after the next input
 * frame. In other words, setting a button mask only applies to the next set
 * of button inputs.
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
#pragma GLOBAL_ASM("asm/nonmatchings/input/set_menu_joystick_delay.s")
#else
/**
 * Sets the number of input frames that must pass before menu movement occurs due to the
 * joystick being held in a direction.
 * 
 * @see get_joystick_menu_xy_sign for more info.
 */
void set_menu_joystick_delay(s8 delay) {
    gMenuJoystickDelay = delay;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/reset_menu_joystick_delay.s")
#else
/**
 * Resets the number of input frames that must pass before menu movement occurs due to the
 * joystick being held in a direction to 5.
 * 
 * @see get_joystick_menu_xy_sign for more info.
 */
void reset_menu_joystick_delay() {
    gMenuJoystickDelay = 5;
}
#endif

#if 0
#pragma GLOBAL_ASM("asm/nonmatchings/input/ret1_800112d8.s")
#else
int ret1_800112d8() {
    return 1;
}
#endif
