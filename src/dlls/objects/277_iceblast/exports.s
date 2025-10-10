.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword iceblast_ctor
.dword iceblast_dtor

# export table
.dword iceblast_create
.dword iceblast_update
.dword iceblast_func_26C
.dword iceblast_draw
.dword iceblast_destroy
.dword iceblast_get_model_flags
.dword iceblast_get_state_size
