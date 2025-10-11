.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SC_levelcontrol_ctor
.dword SC_levelcontrol_dtor

# export table
.dword SC_levelcontrol_setup
.dword SC_levelcontrol_control
.dword SC_levelcontrol_update
.dword SC_levelcontrol_print
.dword SC_levelcontrol_free
.dword SC_levelcontrol_get_model_flags
.dword SC_levelcontrol_get_state_size
.dword SC_levelcontrol_func_660
.dword SC_levelcontrol_func_8A4
