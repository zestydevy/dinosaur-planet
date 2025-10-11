.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_496_ctor
.dword dll_496_dtor

# export table
.dword dll_496_setup
.dword dll_496_control
.dword dll_496_update
.dword dll_496_print
.dword dll_496_free
.dword dll_496_get_model_flags
.dword dll_496_get_state_size
