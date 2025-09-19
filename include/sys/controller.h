/** Controller/gamepad system
 */
#ifndef _SYS_CONTROLLER_H
#define _SYS_CONTROLLER_H

#include "PR/ultratypes.h"
#include "PR/os.h"
#include "PR/sched.h"

// NOTE: This size is NOT CONFIRMED YET
#define CONTROLLER_THREAD_STACKSIZE 1152

// The length of gContInterruptBuffer
#define CONT_INTERRUPT_BUFFER_LENGTH 2
// The length of gContThreadMesgQueueBuffer
#define CONT_THREAD_MESG_QUEUE_BUFFER_LENGTH 8
// The length of gContThreadInputsAppliedQueueBuffer
#define CONT_THREAD_INPUTS_APPLIED_QUEUE_BUFFER_LENGTH 1

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

/**
 * Whether the controller thread should apply controller inputs to globals like gContPads
 * after collecting controller interrupts.
 *
 * @see gContThreadMesgQueue and controller_thread_entry for more info.
 */
extern u8 gApplyContInputs;

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
 * See controller_thread_entry for details.
 */
extern OSMesgQueue gContThreadMesgQueue;
/**
 * When the controller thread applies controller inputs to input globals used by normal
 * gameplay functions, a message is sent to this queue to signal this.
 */
extern OSMesgQueue gContThreadInputsAppliedQueue;

extern OSMesg gContInterruptBuffer[CONT_INTERRUPT_BUFFER_LENGTH];
extern OSMesg gContThreadMesgQueueBuffer[CONT_THREAD_MESG_QUEUE_BUFFER_LENGTH];
extern OSMesg gContThreadInputsAppliedQueueBuffer[CONT_THREAD_INPUTS_APPLIED_QUEUE_BUFFER_LENGTH];

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

extern OSScClient gContSchedulerClient;
/**
 * The address of this message is sent to gContThreadMesgQueue.
 *
 * @see gContThreadMesgQueue for more info.
 */
extern s16 gContQueue2Message;

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
 * Signals the controller thread to apply controller inputs to input related globals
 * used by normal gameplay functions the next time it handles controller input.
 *
 * @details Does not block.
 *
 * @see gContThreadMesgQueue
 */
void signal_apply_controller_inputs();

/**
 * Initializes SI settings and controller globals.
 *
 * @returns The index of the last inserted controller.
 * For example, if one controller is inserted, 0 will be returned.
 * If no controllers are inserted, -1 will be returned.
 */
s32 init_controller_data();

void start_controller_thread(OSSched *scheduler);

/**
 * Gets a masked bitfield of held buttons on the given controller.
 *
 * @details gButtonMask may cause some buttons to be ignored.
 * Always returns 0 if port is not zero.
 */
u16 get_masked_buttons(int port);

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
u16 get_masked_buttons_from_buffer(int port, int buffer);

/**
 * Gets a masked bitfield of buttons that were just pressed on the given controller.
 *
 * @details gButtonMask may cause some buttons to be ignored.
 * Always returns 0 if port is not zero.
 */
u16 get_masked_button_presses(int port);

/**
 * Gets a bitfield of buttons that were just pressed on the given controller.
 *
 * @details Always returns 0 if port is not zero.
 */
u16 get_button_presses(int port);

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
u16 get_masked_button_presses_from_buffer(int port, int buffer);

/**
 * Gets a masked bitfield of buttons that were just released on the given controller.
 *
 * @details gButtonMask may cause some buttons to be ignored.
 * Always returns 0 if port is not zero.
 */
u16 get_masked_button_releases(int port);

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
u16 get_masked_button_releases_from_buffer(int port, int buffer);

/**
 * Returns the joystick X value of the controller in the given port with a deadzone applied.
 *
 * @details Always returns 0 if port is not zero.
 */
s8 get_joystick_x(int port);

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
s8 get_joystick_x_from_buffer(int port, int buffer);

/**
 * Returns the joystick Y value of the controller in the given port with a deadzone applied.
 *
 * @details Always returns 0 if port is not zero.
 */
s8 get_joystick_y(int port);

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
s8 get_joystick_y_from_buffer(int port, int buffer);

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
void get_joystick_menu_xy_sign(int port, s8 *xSign, s8 *ySign);

/**
 * Sets the button mask for the given controller.
 *
 * @details Bits not set in the mask will result in those corresponding buttons
 * to be ignored. The button mask will be reset to 0xFFFF after the next input
 * frame. In other words, setting a button mask only applies to the next set
 * of button inputs.
 */
void set_button_mask(int port, u16 mask);

/**
 * Sets the number of input frames that must pass before menu movement occurs due to the
 * joystick being held in a direction.
 *
 * @see get_joystick_menu_xy_sign for more info.
 */
void set_menu_joystick_delay(s8 delay);

/**
 * Resets the number of input frames that must pass before menu movement occurs due to the
 * joystick being held in a direction to 5.
 *
 * @see get_joystick_menu_xy_sign for more info.
 */
void reset_menu_joystick_delay();

void controller_thread_entry(void *arg);
s8 handle_joystick_deadzone(s8 stick);
void init_virtual_cont_port_map();

enum ControllerButtons {
    CONTROLLER_BUTTON_A = 0x8000,
    CONTROLLER_BUTTON_B = 0x4000,
    CONTROLLER_BUTTON_Z = 0x2000,
    CONTROLLER_BUTTON_START = 0x1000,
    CONTROLLER_BUTTON_D_UP = 0x0800,
    CONTROLLER_BUTTON_D_DOWN = 0x0400,
    CONTROLLER_BUTTON_D_LEFT = 0x0200,
    CONTROLLER_BUTTON_D_RIGHT = 0x0100,
    CONTROLLER_BUTTON_L = 0x0020,
    CONTROLLER_BUTTON_R = 0x0010,
    CONTROLLER_BUTTON_C_UP = 0x0008,
    CONTROLLER_BUTTON_C_DOWN = 0x0004,
    CONTROLLER_BUTTON_C_LEFT = 0x0002,
    CONTROLLER_BUTTON_C_RIGHT = 0x0001
};

#endif //_SYS_CONTROLLER_H
