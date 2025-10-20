.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword BWLevelControl_ctor
.dword BWLevelControl_dtor

# export table
/*0*/ .dword BWLevelControl_setup
/*1*/ .dword BWLevelControl_control
/*2*/ .dword BWLevelControl_update
/*3*/ .dword BWLevelControl_print
/*4*/ .dword BWLevelControl_free
/*5*/ .dword BWLevelControl_get_model_flags
/*6*/ .dword BWLevelControl_get_data_size
