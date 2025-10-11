.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword flameblast_ctor
.dword flameblast_dtor

# export table
.dword flameblast_setup
.dword flameblast_control
.dword flameblast_update
.dword flameblast_print
.dword flameblast_free
.dword flameblast_get_model_flags
.dword flameblast_get_state_size
