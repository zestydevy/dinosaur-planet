#ifndef _INPUT_H_
#define _INPUT_H_

#include "common.h"

#define CONTROLLER_THREAD_ID 0x62

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

#endif
