.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword MMP_levelcontrol_ctor
.dword MMP_levelcontrol_dtor

# export table
/*0*/ .dword MMP_levelcontrol_setup
/*1*/ .dword MMP_levelcontrol_control
/*2*/ .dword MMP_levelcontrol_update
/*3*/ .dword MMP_levelcontrol_print
/*4*/ .dword MMP_levelcontrol_free
/*5*/ .dword MMP_levelcontrol_get_model_flags
/*6*/ .dword MMP_levelcontrol_get_data_size
