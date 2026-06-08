.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_73_ctor
.dword dll_73_dtor

# export table
/*0*/ .dword dll_73_init_text_window
/*1*/ .dword dll_73_init_text_window_with_margin
/*2*/ .dword dll_73_add_string
/*3*/ .dword dll_73_add_string_x
/*4*/ .dword dll_73_set_exit_value
/*5*/ .dword dll_73_handle_joystick_and_buttons
/*6*/ .dword dll_73_set_font_and_colour
/*7*/ .dword dll_73_enable_joy_buttons
/*8*/ .dword dll_73_get_total_items
