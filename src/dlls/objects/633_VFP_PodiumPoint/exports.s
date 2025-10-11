.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword VFP_PodiumPoint_ctor
.dword VFP_PodiumPoint_dtor

# export table
.dword VFP_PodiumPoint_setup
.dword VFP_PodiumPoint_control
.dword VFP_PodiumPoint_update
.dword VFP_PodiumPoint_print
.dword VFP_PodiumPoint_free
.dword VFP_PodiumPoint_get_model_flags
.dword VFP_PodiumPoint_get_data_size
