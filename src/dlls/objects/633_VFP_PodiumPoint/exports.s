.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword VFP_PodiumPoint_ctor
.dword VFP_PodiumPoint_dtor

# export table
.dword VFP_PodiumPoint_create
.dword VFP_PodiumPoint_update
.dword VFP_PodiumPoint_func_128
.dword VFP_PodiumPoint_draw
.dword VFP_PodiumPoint_destroy
.dword VFP_PodiumPoint_get_model_flags
.dword VFP_PodiumPoint_get_state_size
