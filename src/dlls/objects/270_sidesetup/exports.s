.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword sidesetup_ctor
.dword sidesetup_dtor

# export table
.dword sidesetup_setup
.dword sidesetup_control
.dword sidesetup_update
.dword sidesetup_print
.dword sidesetup_free
.dword sidesetup_get_model_flags
.dword sidesetup_get_state_size
