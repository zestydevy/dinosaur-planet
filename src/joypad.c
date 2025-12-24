#include "sys/joypad.h"
#include "sys/thread.h"
#include "macros.h"

#define THREAD_STACK_CONTROLLER 1024

/* -------- .data start -------- */
s32 gNoControllers = 0;
u16 D_8008C8A4 = 0xFFFF;
u16 gButtonMask[MAXCONTROLLERS] = { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF };
u8 gIgnoreJoystick = 0;
u8 D_8008C8B4 = 0;
/* -------- .data end -------- */

// TODO: bss size for this file is correct but IDO reorders it
/* -------- .bss start 800a7db0 -------- */
u8 gPrevContSnapshotsI;
u8 gApplyContInputs;
u8 gNumBufContSnapshots[2];
ControllersSnapshot *gContSnapshots[2];
ControllersSnapshot gContSnapshotsBuffer0[MAX_BUFFERED_CONT_SNAPSHOTS];
ControllersSnapshot gContSnapshotsBuffer1[MAX_BUFFERED_CONT_SNAPSHOTS];
OSMesgQueue gContInterruptQueue;
OSMesgQueue gContThreadMesgQueue;
OSMesgQueue gContThreadInputsAppliedQueue;
OSMesg gContInterruptBuffer[CONT_INTERRUPT_BUFFER_LENGTH];
OSMesg gContThreadMesgQueueBuffer[CONT_THREAD_MESG_QUEUE_BUFFER_LENGTH];
OSMesg gContThreadInputsAppliedQueueBuffer[CONT_THREAD_INPUTS_APPLIED_QUEUE_BUFFER_LENGTH];
OSMesg gContInterruptMessage;
OSContStatus gContStatuses[MAXCONTROLLERS];
OSContPad gContPads[MAXCONTROLLERS];
u16 gButtonPresses[MAXCONTROLLERS];
u16 gButtonReleases[MAXCONTROLLERS];
u8 gVirtualContPortMap[MAXCONTROLLERS];
s8 gLastJoyX[MAXCONTROLLERS];
s8 gLastJoyY[MAXCONTROLLERS];
s8 gMenuJoyXHoldTimer[MAXCONTROLLERS];
s8 gMenuJoyYHoldTimer[MAXCONTROLLERS];
s8 gMenuJoyXSign[MAXCONTROLLERS];
s8 gMenuJoyYSign[MAXCONTROLLERS];
OSThread gControllerThread;
u64 gControllerThreadStack[STACKSIZE(THREAD_STACK_CONTROLLER)+1];
OSScClient gContSchedulerClient;
s16 gContQueue2Message;
u8 _unk800A861A[0x1E]; // gap
s8 gMenuJoystickDelay;
/* -------- .bss end 800a8640 -------- */

/**
 * @returns The message queue associated with SI controller interrupts.
 * This is the same message queue that is passed to osContInit.
 * Offical name: joyMessageQ
 */
OSMesgQueue *joy_get_mesgq() {
    return &gContInterruptQueue;
}

void joy_read_nonblocking() {
    gContQueue2Message = 0xA;
    osSendMesg(&gContThreadMesgQueue, (OSMesg)&gContQueue2Message, OS_MESG_NOBLOCK);
}

/**
 * Signals the controller thread to apply controller inputs to input related globals
 * used by normal gameplay functions the next time it handles controller input and
 * blocks until this occurs.
 *
 * @see gContThreadMesgQueue
 */
void joy_read() {
    // Tell controller thread to apply controller inputs
    gContQueue2Message = 0xA;
    osSendMesg(&gContThreadMesgQueue, (OSMesg)&gContQueue2Message, OS_MESG_NOBLOCK);

    // Block until controller inputs have been applied
    osRecvMesg(&gContThreadInputsAppliedQueue, NULL, OS_MESG_BLOCK);
}

// Official name: joyInit
s32 joy_init() {
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
    joy_reset_map();

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

void joy_start_controller_thread(OSSched *scheduler) {
    osCreateMesgQueue(
        /*mq*/      &gContThreadMesgQueue,
        /*msg*/     &gContThreadMesgQueueBuffer[0],
        /*count*/   CONT_THREAD_MESG_QUEUE_BUFFER_LENGTH
    );

    osScAddClient(scheduler, &gContSchedulerClient, &gContThreadMesgQueue, OS_SC_ID_VIDEO);

    osCreateMesgQueue(
        /*mq*/      &gContThreadInputsAppliedQueue,
        /*msg*/     &gContThreadInputsAppliedQueueBuffer[0],
        /*count*/   CONT_THREAD_INPUTS_APPLIED_QUEUE_BUFFER_LENGTH
    );

    // Create and start controller thread
    osCreateThread(
        /*t*/       &gControllerThread,
        /*id*/      CONTROLLER_THREAD_ID,
        /*entry*/   &joy_controller_thread_entry,
        /*arg*/     NULL,
        /*sp*/      &gControllerThreadStack[STACKSIZE(THREAD_STACK_CONTROLLER)],
        /*pri*/     12
    );

    osStartThread(&gControllerThread);
}

void joy_controller_thread_entry(void* arg) {
    ControllersSnapshot* currSnap;
    ControllersSnapshot* compSnap;
    s16* message;
    s32 var_a2_2;
    s32 var_a3;
    s32 contIdx;
    s32 snapIdx;
    s32 stickX;
    s32 stickY;

    gContSnapshots[0] = gContSnapshotsBuffer0;
    gContSnapshots[1] = gContSnapshotsBuffer1;
    gNumBufContSnapshots[0] = 0;
    gNumBufContSnapshots[1] = 0;
    message = NULL;
    gPrevContSnapshotsI = 0;
    while (TRUE) {
        osRecvMesg(&gContThreadMesgQueue, (OSMesg *) &message, OS_MESG_BLOCK);
        switch (*message) {
            case 1:
                if (gNumBufContSnapshots[gPrevContSnapshotsI] < MAX_BUFFERED_CONT_SNAPSHOTS) {
                    compSnap = gContSnapshots[gPrevContSnapshotsI ^ 1];
                    compSnap = &compSnap[gNumBufContSnapshots[gPrevContSnapshotsI ^ 1] - 1];
                    currSnap = &gContSnapshots[gPrevContSnapshotsI][gNumBufContSnapshots[gPrevContSnapshotsI]];
                    gNumBufContSnapshots[gPrevContSnapshotsI]++;
                    if ((currSnap - 1) >= gContSnapshots[gPrevContSnapshotsI]) {
                        compSnap = (currSnap - 1);
                    }
                    if (osRecvMesg(&gContInterruptQueue, NULL, 0) == 0) {
                        osContGetReadData(currSnap->pads);
                        osContStartReadData(&gContInterruptQueue);
                    } else {
                        bcopy(compSnap, currSnap, sizeof(ControllersSnapshot));
                    }

                    for (contIdx = 0; contIdx < MAXCONTROLLERS; contIdx++) {
                        if (gNoControllers != 0) {
                            currSnap->pads[contIdx].button = 0;
                        }
                        currSnap->buttonPresses[contIdx] = (currSnap->pads[contIdx].button ^ compSnap->pads[contIdx].button) & currSnap->pads[contIdx].button & D_8008C8A4;
                        currSnap->buttonReleases[contIdx] = (currSnap->pads[contIdx].button ^ compSnap->pads[contIdx].button) & compSnap->pads[contIdx].button & D_8008C8A4;
                    }
                }
                if (gApplyContInputs != 0) {
                    bzero(gContPads, sizeof(gContPads));
                    for (contIdx = 0; contIdx < 4; contIdx++) {
                        var_a2_2 = 0;
                        var_a3 = 0;
                        gButtonPresses[contIdx] = 0;
                        gButtonReleases[contIdx] = 0;
                        
                        currSnap = gContSnapshots[gPrevContSnapshotsI];
                        for (snapIdx = 0; snapIdx < gNumBufContSnapshots[gPrevContSnapshotsI]; snapIdx++) {
                            gButtonPresses[contIdx] |= currSnap->buttonPresses[contIdx];
                            gButtonReleases[contIdx] |= currSnap->buttonReleases[contIdx];
                            gContPads[contIdx].button |= currSnap->pads[contIdx].button;
                            var_a2_2 += currSnap->pads[contIdx].stick_x;
                            var_a3 += currSnap->pads[contIdx].stick_y;
                            currSnap++;
                        }
                        gContPads[contIdx].stick_x = var_a2_2 / gNumBufContSnapshots[gPrevContSnapshotsI];
                        gContPads[contIdx].stick_y = var_a3 / gNumBufContSnapshots[gPrevContSnapshotsI];
                        stickX = gContPads[contIdx].stick_x;
                        stickY = gContPads[contIdx].stick_y;
                        gMenuJoyXSign[contIdx] = 0;
                        gMenuJoyYSign[contIdx] = 0;
                        
                        if ((stickX < -35) && (gLastJoyX[contIdx] >= -35)) {
                            gMenuJoyXSign[contIdx] = -1;
                            gMenuJoyXHoldTimer[contIdx] = 0;
                        }
                        if ((stickX > 35) && (gLastJoyX[contIdx] <= 35)) {
                            gMenuJoyXSign[contIdx] = 1;
                            gMenuJoyXHoldTimer[contIdx] = 0;
                        }
                        if ((stickY < -35) && (gLastJoyY[contIdx] >= -35)) {
                            gMenuJoyYSign[contIdx] = -1;
                            gMenuJoyYHoldTimer[contIdx] = 0;
                        }
                        if ((stickY > 35) && (gLastJoyY[contIdx] <= 35)) {
                            gMenuJoyYSign[contIdx] = 1;
                            gMenuJoyYHoldTimer[contIdx] = 0;
                        }
                        gLastJoyY[contIdx] = stickY;
                        if (gLastJoyY[contIdx] < -35) {
                            gMenuJoyYHoldTimer[contIdx] += 1;
                        } else if (gLastJoyY[contIdx] > 35) {
                            gMenuJoyYHoldTimer[contIdx] += 1;
                        } else {
                            gMenuJoyYHoldTimer[contIdx] = 0;
                        }
                        if (gMenuJoystickDelay < gMenuJoyYHoldTimer[contIdx]) {
                            gLastJoyY[contIdx] = 0;
                            gMenuJoyYHoldTimer[contIdx] = 0;
                        }
                        gLastJoyX[contIdx] = stickX;
                        if (gLastJoyX[contIdx] < -35) {
                            gMenuJoyXHoldTimer[contIdx] += 1;
                        } else if (gLastJoyX[contIdx] > 35) {
                            gMenuJoyXHoldTimer[contIdx] += 1;
                        } else {
                            gMenuJoyXHoldTimer[contIdx] = 0;
                        }
                        if (gMenuJoystickDelay < gMenuJoyXHoldTimer[contIdx]) {
                            gLastJoyX[contIdx] = 0;
                            gMenuJoyXHoldTimer[contIdx] = 0;
                        }
                        gButtonMask[contIdx] = 0xFFFF;
                    }
                    gIgnoreJoystick = 0;
                    gApplyContInputs = 0;
                    gPrevContSnapshotsI ^= 1;
                    gNumBufContSnapshots[gPrevContSnapshotsI] = 0;
                    osSendMesg(&gContThreadInputsAppliedQueue, &gContQueue2Message, 0);
                }
                break;
            case 0xA:
            default:
                gApplyContInputs = 1;
                break;
        }
    }
}

// Official name: joyResetMap
void joy_reset_map(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        gVirtualContPortMap[i] = i;
    }
}

/**
 * Assign the first four player ID's to the index of the connected players.
 * Assign the next four player ID's to the index of the players who are not connected.
 *
 * Official name: joyCreateMap
 */
void joy_create_map(s8 activePlayers[MAXCONTROLLERS]) {
    int i;
    int k;

    k = 0;

    for (i = 0; i < MAXCONTROLLERS; ++i) {
        if (activePlayers[i] != 0) {
            gVirtualContPortMap[k] = i;
            k += 1;
        }
    }

    for (i = 0; i < MAXCONTROLLERS; ++i) {
        if (activePlayers[i] == 0) {
            gVirtualContPortMap[k] = i;
            k += 1;
        }
    }
}

/**
 * Maps the given virtual controller port to its corresponding physical port.
 *
 * Official name: joyGetController
 */
u8 joy_get_controller(int virtualPort) {
    return gVirtualContPortMap[virtualPort];
}

/**
 * Swaps the first 2 virtual controller ports.
 */
void joy_swap_controllers() {
    u8 port0 = gVirtualContPortMap[0];

    gVirtualContPortMap[0] = gVirtualContPortMap[1];
    gVirtualContPortMap[1] = port0;
}

// Official name: joyGetButtons
u16 joy_get_buttons(int port) {
    if (port > 0) {
        return 0;
    }

    return gContPads[gVirtualContPortMap[port]].button & gButtonMask[port];
}

u16 joy_get_buttons_buffered(int port, int buffer) {
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

// Official name: joyGetPressed
u16 joy_get_pressed(int port) {
    if (port > 0) {
        return 0;
    }

    return gButtonPresses[gVirtualContPortMap[port]] & gButtonMask[port];
}

u16 joy_get_pressed_raw(int port) {
    if (port > 0) {
        return 0;
    }

    return gButtonPresses[gVirtualContPortMap[port]];
}

u16 joy_get_pressed_buffered(int port, int buffer) {
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

// Official name: joyGetReleased
u16 joy_get_released(int port) {
    if (port > 0) {
        return 0;
    }

    return gButtonReleases[gVirtualContPortMap[port]] & gButtonMask[port];
}

u16 joy_get_released_buffered(int port, int buffer) {
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

// Official name: joyGetStickX
s8 joy_get_stick_x(int port) {
    if (port > 0) {
        return 0;
    } else {
        return joy_handle_joystick_deadzone(gContPads[gVirtualContPortMap[port]].stick_x);
    }
}

s8 joy_get_stick_x_buffered(int port, int buffer) {
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

    return joy_handle_joystick_deadzone(pads[gVirtualContPortMap[port]].stick_x);
}

// Official name: joyGetStickY
s8 joy_get_stick_y(int port) {
    if (port > 0) {
        return 0;
    } else {
        return joy_handle_joystick_deadzone(gContPads[gVirtualContPortMap[port]].stick_y);
    }
}

s8 joy_get_stick_y_buffered(int port, int buffer) {
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

    return joy_handle_joystick_deadzone(pads[gVirtualContPortMap[port]].stick_y);
}

void joy_get_stick_menu_xy_sign(int port, s8 *xSign, s8 *ySign) {
    *xSign = gMenuJoyXSign[gVirtualContPortMap[port]];
    *ySign = gMenuJoyYSign[gVirtualContPortMap[port]];
}

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
s8 joy_handle_joystick_deadzone(s8 stick) {
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

// Official name: joySetSecurity
void joy_set_security() {
    D_8008C8A4 = 0;
}

void joy_set_button_mask(int port, u16 mask) {
    gButtonMask[port] &= ~mask;
}

// Clear joystick x/y to zero until the next controller read.
void joy_reset_joystick(int _) {
    gIgnoreJoystick = TRUE;
}

void joy_set_menu_joystick_delay(s8 delay) {
    gMenuJoystickDelay = delay;
}

void joy_reset_menu_joystick_delay() {
    gMenuJoystickDelay = 5;
}

int ret1_800112d8() {
    return 1;
}
