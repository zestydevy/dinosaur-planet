.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword VFP_FlamePoint_ctor
.dword VFP_FlamePoint_dtor

# export table
/*0*/ .dword VFP_FlamePoint_setup
/*1*/ .dword VFP_FlamePoint_control
/*2*/ .dword VFP_FlamePoint_update
/*3*/ .dword VFP_FlamePoint_print
/*4*/ .dword VFP_FlamePoint_free
/*5*/ .dword VFP_FlamePoint_get_model_flags
/*6*/ .dword VFP_FlamePoint_get_data_size
/*7*/ .dword VFP_FlamePoint_func_280
