.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword VFP_lavastar_ctor
.dword VFP_lavastar_dtor

# export table
.dword VFP_lavastar_create
.dword VFP_lavastar_update
.dword VFP_lavastar_func_2BC
.dword VFP_lavastar_draw
.dword VFP_lavastar_destroy
.dword VFP_lavastar_func_390
.dword VFP_lavastar_get_state_size
