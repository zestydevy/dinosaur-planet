#ifndef _INPUT_H_
#define _INPUT_H_

#include "ultra64.h"

#include "constants.h"
#include "common_structs.h"
#include "variables.h"

#define CONTROLLER_THREAD_ID 0x62
// NOTE: This size is NOT CONFIRMED YET
#define CONTROLLER_THREAD_STACKSIZE 1152
// The length of gContInterruptBuffer
#define CONT_INTERRUPT_BUFFER_LENGTH 2
#define CONTROLLER_MESG_BUFFER_2_LENGTH 8
#define CONTROLLER_MESG_BUFFER_3_LENGTH 1

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

/**
 * If TRUE, no controllers are inserted.
 * 
 * @see init_controller_data
 */
extern s32 gNoControllers;

extern u16 D_8008C8A4;

/**
 * For each controller, a mask applied to their button state.
 * 
 * Ex. If the mask is 0x0000 then all buttons will be reported as 0 
 * regardless of whether they are actually pressed.
 * 
 * At the end of each input frame (see controller_thread_entry) this 
 * is reset to 0xFFFF for each controller.
 */
extern u16 gButtonMask[MAXCONTROLLERS];

extern u8 gIgnoreJoystick;

/**
 * An index of gContSnapshots specifying which snapshot buffer contains previous controller snapshots.
 * 
 * The index of current controller snapshots can be found by doing: gPrevContSnapshotsI ^ 1.
 */
extern u8 gPrevContSnapshotsI;

extern u8 D_800A7DB1;

/**
 * For the current and previous controller snapshots, the number of snapshots that are buffered.
 * 
 * @details Which index is which is determined by gPrevContSnapshotsI.
 */
extern u8 gNumBufContSnapshots[2];

/**
 * Pointers to 2 arrays, where one is an array of current controller snapshots 
 * and the other is an array of previous controller snapshots.
 * 
 * @details Which array is currently which is determined by gPrevContSnapshotsI.
 * Each array is a buffer of up to 4 snapshots. This is done to avoid missing controller
 * interrupt data as it comes in on the queue, by being able to collect more than one at
 * a time when processing input.
 * 
 * @see controller_thread_entry
 */
extern ControllersSnapshot *gContSnapshots[2];

/**
 * A buffer of controller snapshots, gContSnapshots[0] is a pointer to this.
 */
extern ControllersSnapshot gContSnapshotsBuffer0[MAX_BUFFERED_CONT_SNAPSHOTS];
/**
 * A buffer of controller snapshots, gContSnapshots[1] is a pointer to this.
 */
extern ControllersSnapshot gContSnapshotsBuffer1[MAX_BUFFERED_CONT_SNAPSHOTS];

/**
 * The message queue associated with SI controller interrupts.
 * 
 * This is the same message queue that is passed to osContInit.
 */
extern OSMesgQueue gContInterruptQueue;
extern OSMesgQueue gControllerMesgQueue2;
extern OSMesgQueue gControllerMesgQueue3;
extern OSMesg gContInterruptBuffer[CONT_INTERRUPT_BUFFER_LENGTH];
extern OSMesg gControllerMesgQueue2Buffer[CONTROLLER_MESG_BUFFER_2_LENGTH];
extern OSMesg gControllerMesgQueue3Buffer[CONTROLLER_MESG_BUFFER_3_LENGTH];

/**
 * Message sent to the gContInterruptQueue when an SI interrupt occurs.
 */
extern OSMesg gContInterruptMessage;

/**
 * The status and type of each controller (SI device).
 */
extern OSContStatus gContStatuses[MAXCONTROLLERS];

/**
 * Current inputs of each controller.
 * 
 * @details This data is created by combining multiple buffered controller snapshots 
 * from gContSnapshots.
 */
extern OSContPad gContPads[MAXCONTROLLERS];

/**
 * For each controller, a bitfield of buttons that were just pressed.
 */
extern u16 gButtonPresses[MAXCONTROLLERS];
/**
 * For each controller, a bitfield of buttons that were just released.
 */
extern u16 gButtonReleases[MAXCONTROLLERS];

/**
 * Maps virtual controller ports to physical ports.
 * 
 * Ex. gVirtualToPhysicalContPorts[virtualPort] = physicalPort
 */
extern u8 gVirtualContPortMap[MAXCONTROLLERS]; 

/**
 * Internal joystick X values per controller from last input frame used to set gMenuJoyXSign.
 * 
 * @see gMenuJoyXSign, controller_thread_entry
 */ 
extern s8 gLastJoyX[MAXCONTROLLERS];
/**
 * Internal joystick Y values per controller from last input frame used to set gMenuJoyYSign.
 * 
 * @see gMenuJoyYSign, controller_thread_entry 
 */ 
extern s8 gLastJoyY[MAXCONTROLLERS];
/**
 * Increments from [0, gMenuJoystickDelay) while abs(joystick X) > 35.
 * Once it reaches gMenuJoystickDelay - 1, resets back to 0.
 */
extern s8 gMenuJoyXHoldTimer[MAXCONTROLLERS];
/**
 * Increments from [0, gMenuJoystickDelay) while abs(joystick Y) > 35.
 * Once it reaches gMenuJoystickDelay - 1, resets back to 0.
 */
extern s8 gMenuJoyYHoldTimer[MAXCONTROLLERS];
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
extern s8 gMenuJoyXSign[MAXCONTROLLERS];
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
extern s8 gMenuJoyYSign[MAXCONTROLLERS];

extern OSThread gControllerThread;
extern u8 gControllerThreadStack[CONTROLLER_THREAD_STACKSIZE];

extern int D_800A8608;
extern s16 D_800A8618;

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
extern s8 gMenuJoystickDelay;

/**
 * Initializes SI settings and controller globals.
 * 
 * @returns The index of the last inserted controller.
 * For example, if one controller is inserted, 0 will be returned.
 * If no controllers are inserted, -1 will be returned.
 */
s32 init_controller_data();

void start_controller_thread(OSSched *scheduler);

#endif
