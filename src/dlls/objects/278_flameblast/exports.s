.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword flameblast_ctor
.dword flameblast_dtor

# export table
.dword flameblast_create
.dword flameblast_update
.dword flameblast_func_274
.dword flameblast_draw
.dword flameblast_destroy
.dword flameblast_get_model_flags
.dword flameblast_get_state_size
