.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword sidesetup_ctor
.dword sidesetup_dtor

# export table
.dword sidesetup_create
.dword sidesetup_update
.dword sidesetup_func_48
.dword sidesetup_draw
.dword sidesetup_destroy
.dword sidesetup_func_7C
.dword sidesetup_get_state_size
