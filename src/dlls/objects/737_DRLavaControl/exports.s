.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DRLavaControl_ctor
.dword DRLavaControl_dtor

# export table
/*0*/ .dword DRLavaControl_setup
/*1*/ .dword DRLavaControl_control
/*2*/ .dword DRLavaControl_update
/*3*/ .dword DRLavaControl_print
/*4*/ .dword DRLavaControl_free
/*5*/ .dword DRLavaControl_get_model_flags
/*6*/ .dword DRLavaControl_get_data_size
