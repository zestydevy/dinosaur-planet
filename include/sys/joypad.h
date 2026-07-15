/** Controller/gamepad system
 */
#ifndef _SYS_JOYPAD_H
#define _SYS_JOYPAD_H

#include "PR/ultratypes.h"
#include "PR/sched.h"

/**
 * Signals the controller thread to apply controller inputs to input related globals
 * used by normal gameplay functions the next time it handles controller input.
 *
 * @details Does not block.
 *
 * @see gContThreadMesgQueue
 */
void joyReadNonblocking(void);

/**
 * Initializes SI settings and controller globals.
 *
 * @returns The index of the last inserted controller.
 * For example, if one controller is inserted, 0 will be returned.
 * If no controllers are inserted, -1 will be returned.
 */
s32 joyInit(void);

void joyStartControllerThread(OSSched *scheduler);

/**
 * Gets a masked bitfield of held buttons on the given controller.
 *
 * @details gButtonMask may cause some buttons to be ignored.
 * Always returns 0 if port is not zero.
 */
u16 joyGetButtons(int port);

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
u16 joyGetButtonsBuffered(int port, int buffer);

/**
 * Gets a masked bitfield of buttons that were just pressed on the given controller.
 *
 * @details gButtonMask may cause some buttons to be ignored.
 * Always returns 0 if port is not zero.
 */
u16 joyGetPressed(int port);

/**
 * Gets a bitfield of buttons that were just pressed on the given controller.
 *
 * @details Always returns 0 if port is not zero.
 */
u16 joyGetPressedRaw(int port);

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
u16 joyGetPressedBuffered(int port, int buffer);

/**
 * Gets a masked bitfield of buttons that were just released on the given controller.
 *
 * @details gButtonMask may cause some buttons to be ignored.
 * Always returns 0 if port is not zero.
 */
u16 joyGetReleased(int port);

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
u16 joyGetReleasedBuffered(int port, int buffer);

/**
 * Returns the joystick X value of the controller in the given port with a deadzone applied.
 *
 * @details Always returns 0 if port is not zero.
 */
s8 joyGetStickX(int port);

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
s8 joyGetStickXBuffered(int port, int buffer);

/**
 * Returns the joystick Y value of the controller in the given port with a deadzone applied.
 *
 * @details Always returns 0 if port is not zero.
 */
s8 joyGetStickY(int port);

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
s8 joyGetStickYBuffered(int port, int buffer);

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
 * @see joySetMenuStickDelay
 */
void joyGetStickMenuXYSign(int port, s8 *xSign, s8 *ySign);

/**
 * Disables buttons for the given controller.
 *
 * @details The given button bits will be removed from the button mask until the next input frame.
 * This has an immediate effect on functions like joyGetButtons.
 */
void joyDisableButtons(int port, u16 buttons);

/**
 * Sets the number of input frames that must pass before menu movement occurs due to the
 * joystick being held in a direction.
 *
 * @see joyGetStickMenuXYSign for more info.
 */
void joySetMenuStickDelay(s8 delay);

/**
 * Resets the number of input frames that must pass before menu movement occurs due to the
 * joystick being held in a direction to 5.
 *
 * @see joyGetStickMenuXYSign for more info.
 */
void joyResetMenuStickDelay(void);

void joyResetMap(void);

#endif //_SYS_JOYPAD_H
