.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword fishingnet_ctor
.dword fishingnet_dtor

# export table
/*0*/ .dword fishingnet_setup
/*1*/ .dword fishingnet_control
/*2*/ .dword fishingnet_update
/*3*/ .dword fishingnet_print
/*4*/ .dword fishingnet_free
/*5*/ .dword fishingnet_get_model_flags
/*6*/ .dword fishingnet_get_data_size
/*7*/ .dword fishingnet_set_visibility
/*8*/ .dword fishingnet_draw
/*9*/ .dword fishingnet_get_transform
/*10*/ .dword fishingnet_catch
/*11*/ .dword fishingnet_func_4D4
