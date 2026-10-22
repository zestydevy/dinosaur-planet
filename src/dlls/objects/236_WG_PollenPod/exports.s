.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword WGPollenPod_ctor
.dword WGPollenPod_dtor

# export table
/*0*/ .dword WGPollenPod_setup
/*1*/ .dword WGPollenPod_control
/*2*/ .dword WGPollenPod_update
/*3*/ .dword WGPollenPod_print
/*4*/ .dword WGPollenPod_free
/*5*/ .dword WGPollenPod_get_model_flags
/*6*/ .dword WGPollenPod_get_data_size
