.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword SHLevelControl_ctor
.dword SHLevelControl_dtor

# export table
/*0*/ .dword SHLevelControl_setup
/*1*/ .dword SHLevelControl_control
/*2*/ .dword SHLevelControl_update
/*3*/ .dword SHLevelControl_print
/*4*/ .dword SHLevelControl_free
/*5*/ .dword SHLevelControl_get_model_flags
/*6*/ .dword SHLevelControl_get_data_size
