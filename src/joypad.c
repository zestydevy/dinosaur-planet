#include "sys/joypad.h"
#include "sys/thread.h"
#include "macros.h"

#define THREAD_STACK_CONTROLLER 1024
// The maximum number of buffered controller snapshots.
#define MAX_BUFFERED_CONT_SNAPSHOTS 4

/**
 * Represents a single snapshot of each controller.
 *
 * In addition to raw OSContPad snapshots, includes bitfields for
 * buttons being pressed and released.
 */
typedef struct _ControllersSnapshot {
    /**
     * A raw controller pad snapshot for each controller.
     */
    OSContPad pads[MAXCONTROLLERS];

    /**
     * For each controller, a button bitfield where set button bits indicate that
     * the button was just pressed down.
     */
    u16 buttonPresses[MAXCONTROLLERS];

    /**
     * For each controller, a button bitfield where set button bits indicate that
     * the button was just released.
     */
    u16 buttonReleases[MAXCONTROLLERS];
} ControllersSnapshot;

/* -------- .data start -------- */
/**
 * If TRUE, no controllers are inserted.
 *
 * @see joyInit
 */
s32 gNoControllers = 0;
u16 D_8008C8A4 = 0xFFFF;
/**
 * For each controller, a mask applied to their button state.
 *
 * Ex. If the mask is 0x0000 then all buttons will be reported as 0
 * regardless of whether they are actually pressed.
 *
 * At the end of each input frame (see joyControllerThreadEntry) this
 * is reset to 0xFFFF for each controller.
 */
u16 gButtonMask[MAXCONTROLLERS] = { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF };
u8 gIgnoreJoystick = 0;
u8 D_8008C8B4 = 0;
/* -------- .data end -------- */

// TODO: bss size for this file is correct but IDO reorders it
/* -------- .bss start 800a7db0 -------- */
/**
 * An index of gContSnapshots specifying which snapshot buffer contains previous controller snapshots.
 *
 * The index of current controller snapshots can be found by doing: gPrevContSnapshotsI ^ 1.
 */
u8 gPrevContSnapshotsI;
/**
 * Whether the controller thread should apply controller inputs to globals like gContPads
 * after collecting controller interrupts.
 *
 * @see gContThreadMesgQueue and joyControllerThreadEntry for more info.
 */
u8 gApplyContInputs;
/**
 * For the current and previous controller snapshots, the number of snapshots that are buffered.
 *
 * @details Which index is which is determined by gPrevContSnapshotsI.
 */
u8 gNumBufContSnapshots[2];
/**
 * Pointers to 2 arrays, where one is an array of current controller snapshots
 * and the other is an array of previous controller snapshots.
 *
 * @details Which array is currently which is determined by gPrevContSnapshotsI.
 * Each array is a buffer of up to 4 snapshots. This is done to avoid missing controller
 * interrupt data as it comes in on the queue, by being able to collect more than one at
 * a time when processing input.
 *
 * @see joyControllerThreadEntry
 */
ControllersSnapshot *gContSnapshots[2];
/**
 * A buffer of controller snapshots, gContSnapshots[0] is a pointer to this.
 */
ControllersSnapshot gContSnapshotsBuffer0[MAX_BUFFERED_CONT_SNAPSHOTS];
/**
 * A buffer of controller snapshots, gContSnapshots[1] is a pointer to this.
 */
ControllersSnapshot gContSnapshotsBuffer1[MAX_BUFFERED_CONT_SNAPSHOTS];
/**
 * The message queue associated with SI controller interrupts.
 *
 * This is the same message queue that is passed to osContInit.
 */
OSMesgQueue gContInterruptQueue;
/**
 * A message queue for signalling when the controller thread should process controller
 * interrupts and potentially to apply them to the controller globals that the game
 * reads from for normal gameplay.
 *
 * - For every non-1 value sent to this queue, the controller thread will set a flag saying
 * that next time the queue gets a value of 1 sent, it will process controller interrupts
 * AND apply them to globals like gContPads.
 * - For every value of 1 sent to this queue, the controller thread will process controller
 * interrupts and if a non-1 value was previously sent also apply them to globals like gContPads.
 *
 * While messages are not sent to this queue, the controller thread will block.
 *
 * Usually, 0xA, 0x1 is repeatedly sent each frame or so (not sure exactly when, but its related
 * to game ticks) to process controller input.
 *
 * See joyControllerThreadEntry for details.
 */
OSMesgQueue gContThreadMesgQueue;
/**
 * When the controller thread applies controller inputs to input globals used by normal
 * gameplay functions, a message is sent to this queue to signal this.
 */
OSMesgQueue gContThreadInputsAppliedQueue;
OSMesg gContInterruptBuffer[2];
OSMesg gContThreadMesgQueueBuffer[8];
OSMesg gContThreadInputsAppliedQueueBuffer[1];
/**
 * Message sent to the gContInterruptQueue when an SI interrupt occurs.
 */
OSMesg gContInterruptMessage;
/**
 * The status and type of each controller (SI device).
 */
OSContStatus gContStatuses[MAXCONTROLLERS];
/**
 * Current inputs of each controller.
 *
 * @details This data is created by combining multiple buffered controller snapshots
 * from gContSnapshots.
 */
OSContPad gContPads[MAXCONTROLLERS];
/**
 * For each controller, a bitfield of buttons that were just pressed.
 */
u16 gButtonPresses[MAXCONTROLLERS];
/**
 * For each controller, a bitfield of buttons that were just released.
 */
u16 gButtonReleases[MAXCONTROLLERS];
/**
 * Maps virtual controller ports to physical ports.
 *
 * Ex. gVirtualToPhysicalContPorts[virtualPort] = physicalPort
 */
u8 gVirtualContPortMap[MAXCONTROLLERS];
/**
 * Internal joystick X values per controller from last input frame used to set gMenuJoyXSign.
 *
 * @see gMenuJoyXSign, joyControllerThreadEntry
 */
s8 gLastJoyX[MAXCONTROLLERS];
/**
 * Internal joystick Y values per controller from last input frame used to set gMenuJoyYSign.
 *
 * @see gMenuJoyYSign, joyControllerThreadEntry
 */
s8 gLastJoyY[MAXCONTROLLERS];
/**
 * Increments from [0, gMenuJoystickDelay) while abs(joystick X) > 35.
 * Once it reaches gMenuJoystickDelay - 1, resets back to 0.
 */
s8 gMenuJoyXHoldTimer[MAXCONTROLLERS];
/**
 * Increments from [0, gMenuJoystickDelay) while abs(joystick Y) > 35.
 * Once it reaches gMenuJoystickDelay - 1, resets back to 0.
 */
s8 gMenuJoyYHoldTimer[MAXCONTROLLERS];
/**
 * For each controller, a value indicating that the joystick X axis is pushed
 * in a direction and that for the current frame it should result in X-axis menu movement.
 *
 * @details The joystick must be held in a direction for gMenuJoystickDelay frames
 * before the sign will be non-zero here, and it will remain non-zero just for the
 * one frame until the stick is held for another gMenuJoystickDelay frames.
 * This effectively allows the user to hold the joystick in a direction to move between
 * menus without it being too fast.
 *
 * @see gMenuJoystickDelay, gMenuJoyXHoldTimer
 */
s8 gMenuJoyXSign[MAXCONTROLLERS];
/**
 * For each controller, a value indicating that the joystick Y axis is pushed
 * in a direction and that for the current frame it should result in Y-axis menu movement.
 *
 * @details The joystick must be held in a direction for gMenuJoystickDelay frames
 * before the sign will be non-zero here, and it will remain non-zero just for the
 * one frame until the stick is held for another gMenuJoystickDelay frames.
 * This effectively allows the user to hold the joystick in a direction to move between
 * menus without it being too fast.
 *
 * @see gMenuJoystickDelay, gMenuJoyYHoldTimer
 */
s8 gMenuJoyYSign[MAXCONTROLLERS];
OSThread gControllerThread;
u64 gControllerThreadStack[STACKSIZE(THREAD_STACK_CONTROLLER)+1];
OSScClient gContSchedulerClient;
/**
 * The address of this message is sent to gContThreadMesgQueue.
 *
 * @see gContThreadMesgQueue for more info.
 */
s16 gContQueue2Message;
u8 _unk800A861A[0x1E]; // gap
/**
 * The number of controller input frames that a joystick axis must be held
 * before menu movement will automatically occur.
 *
 * For ex. if the user move the joystick down, the menu selection will move
 * instantly initially but then wait this many input frames before moving again
 * while the joystick is held.
 *
 * Defaults to 5.
 */
s8 gMenuJoystickDelay;
/* -------- .bss end 800a8640 -------- */

void joyControllerThreadEntry(void *arg);
s8 joyHandleStickDeadzone(s8 stick);

/**
 * @returns The message queue associated with SI controller interrupts.
 * This is the same message queue that is passed to osContInit.
 * Offical name: joyMessageQ
 */
OSMesgQueue *joyMessageQ(void) {
    return &gContInterruptQueue;
}

void joyReadNonblocking(void) {
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
void joyRead(void) {
    // Tell controller thread to apply controller inputs
    gContQueue2Message = 0xA;
    osSendMesg(&gContThreadMesgQueue, (OSMesg)&gContQueue2Message, OS_MESG_NOBLOCK);

    // Block until controller inputs have been applied
    osRecvMesg(&gContThreadInputsAppliedQueue, NULL, OS_MESG_BLOCK);
}

// Official name: joyInit
s32 joyInit(void) {
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
        ARRAYCOUNT(gContInterruptBuffer));

    osSetEventMesg(OS_EVENT_SI, &gContInterruptQueue, gContInterruptMessage);

    osContInit(&gContInterruptQueue, &contBitpattern, &gContStatuses[0]);

    // Start reading controller input
    osContStartReadData(&gContInterruptQueue);

    // Set default controller virtual->physical port mapping
    joyResetMap();

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

void joyStartControllerThread(OSSched *scheduler) {
    osCreateMesgQueue(
        /*mq*/      &gContThreadMesgQueue,
        /*msg*/     &gContThreadMesgQueueBuffer[0],
        /*count*/   ARRAYCOUNT(gContThreadMesgQueueBuffer)
    );

    osScAddClient(scheduler, &gContSchedulerClient, &gContThreadMesgQueue, OS_SC_ID_VIDEO);

    osCreateMesgQueue(
        /*mq*/      &gContThreadInputsAppliedQueue,
        /*msg*/     &gContThreadInputsAppliedQueueBuffer[0],
        /*count*/   ARRAYCOUNT(gContThreadInputsAppliedQueueBuffer)
    );

    // Create and start controller thread
    osCreateThread(
        /*t*/       &gControllerThread,
        /*id*/      CONTROLLER_THREAD_ID,
        /*entry*/   &joyControllerThreadEntry,
        /*arg*/     NULL,
        /*sp*/      &gControllerThreadStack[STACKSIZE(THREAD_STACK_CONTROLLER)],
        /*pri*/     12
    );

    osStartThread(&gControllerThread);
}

void joyControllerThreadEntry(void* arg) {
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
void joyResetMap(void) {
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
void joyCreateMap(s8 activePlayers[MAXCONTROLLERS]) {
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
u8 joyGetController(int virtualPort) {
    return gVirtualContPortMap[virtualPort];
}

/**
 * Swaps the first 2 virtual controller ports.
 */
void joySwapControllers(void) {
    u8 port0 = gVirtualContPortMap[0];

    gVirtualContPortMap[0] = gVirtualContPortMap[1];
    gVirtualContPortMap[1] = port0;
}

// Official name: joyGetButtons
u16 joyGetButtons(int port) {
    if (port > 0) {
        return 0;
    }

    return gContPads[gVirtualContPortMap[port]].button & gButtonMask[port];
}

u16 joyGetButtonsBuffered(int port, int buffer) {
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
u16 joyGetPressed(int port) {
    if (port > 0) {
        return 0;
    }

    return gButtonPresses[gVirtualContPortMap[port]] & gButtonMask[port];
}

u16 joyGetPressedRaw(int port) {
    if (port > 0) {
        return 0;
    }

    return gButtonPresses[gVirtualContPortMap[port]];
}

u16 joyGetPressedBuffered(int port, int buffer) {
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
u16 joyGetReleased(int port) {
    if (port > 0) {
        return 0;
    }

    return gButtonReleases[gVirtualContPortMap[port]] & gButtonMask[port];
}

u16 joyGetReleasedBuffered(int port, int buffer) {
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
s8 joyGetStickX(int port) {
    if (port > 0) {
        return 0;
    } else {
        return joyHandleStickDeadzone(gContPads[gVirtualContPortMap[port]].stick_x);
    }
}

s8 joyGetStickXBuffered(int port, int buffer) {
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

    return joyHandleStickDeadzone(pads[gVirtualContPortMap[port]].stick_x);
}

// Official name: joyGetStickY
s8 joyGetStickY(int port) {
    if (port > 0) {
        return 0;
    } else {
        return joyHandleStickDeadzone(gContPads[gVirtualContPortMap[port]].stick_y);
    }
}

s8 joyGetStickYBuffered(int port, int buffer) {
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

    return joyHandleStickDeadzone(pads[gVirtualContPortMap[port]].stick_y);
}

void joyGetStickMenuXYSign(int port, s8 *xSign, s8 *ySign) {
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
s8 joyHandleStickDeadzone(s8 stick) {
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
void joySetSecurity(void) {
    D_8008C8A4 = 0;
}

void joyDisableButtons(int port, u16 buttons) {
    gButtonMask[port] &= ~buttons;
}

// Clear joystick x/y to zero until the next controller read.
void joyDisableStick(int _) {
    gIgnoreJoystick = TRUE;
}

void joySetMenuStickDelay(s8 delay) {
    gMenuJoystickDelay = delay;
}

void joyResetMenuStickDelay(void) {
    gMenuJoystickDelay = 5;
}

int joy_func_800112d8(void) {
    return 1;
}
