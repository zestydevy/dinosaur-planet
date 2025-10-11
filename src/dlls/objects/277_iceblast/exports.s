.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword iceblast_ctor
.dword iceblast_dtor

# export table
.dword iceblast_setup
.dword iceblast_control
.dword iceblast_update
.dword iceblast_print
.dword iceblast_free
.dword iceblast_get_model_flags
.dword iceblast_get_state_size
