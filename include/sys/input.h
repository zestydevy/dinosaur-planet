#ifndef _SYS_INPUT_H
#define _SYS_INPUT_H

s8 get_joystick_x_from_buffer(int port, int buffer);
s8 get_joystick_y_from_buffer(int port, int buffer);
u16 get_masked_buttons_from_buffer(int port, int buffer);
u16 get_masked_button_releases_from_buffer(int port, int buffer);
u16 get_masked_button_presses_from_buffer(int port, int buffer);

#endif
