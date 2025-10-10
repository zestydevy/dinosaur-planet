.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword dll_496_ctor
.dword dll_496_dtor

# export table
.dword dll_496_func_18
.dword dll_496_func_24C
.dword dll_496_func_770
.dword dll_496_func_77C
.dword dll_496_func_804
.dword dll_496_get_model_flags
.dword dll_496_get_state_size
