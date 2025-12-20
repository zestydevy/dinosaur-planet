.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword CFLevelControl_ctor
.dword CFLevelControl_dtor

# export table
/*0*/ .dword CFLevelControl_setup
/*1*/ .dword CFLevelControl_control
/*2*/ .dword CFLevelControl_update
/*3*/ .dword CFLevelControl_print
/*4*/ .dword CFLevelControl_free
/*5*/ .dword CFLevelControl_get_model_flags
/*6*/ .dword CFLevelControl_get_data_size
