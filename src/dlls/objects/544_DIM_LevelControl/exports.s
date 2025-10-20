.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DIM_LevelControl_ctor
.dword DIM_LevelControl_dtor

# export table
/*0*/ .dword DIM_LevelControl_setup
/*1*/ .dword DIM_LevelControl_control
/*2*/ .dword DIM_LevelControl_update
/*3*/ .dword DIM_LevelControl_print
/*4*/ .dword DIM_LevelControl_free
/*5*/ .dword DIM_LevelControl_get_model_flags
/*6*/ .dword DIM_LevelControl_get_data_size
