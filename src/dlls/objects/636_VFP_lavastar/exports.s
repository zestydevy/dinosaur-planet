.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword VFP_lavastar_ctor
.dword VFP_lavastar_dtor

# export table
.dword VFP_lavastar_setup
.dword VFP_lavastar_control
.dword VFP_lavastar_update
.dword VFP_lavastar_print
.dword VFP_lavastar_free
.dword VFP_lavastar_get_model_flags
.dword VFP_lavastar_get_state_size
