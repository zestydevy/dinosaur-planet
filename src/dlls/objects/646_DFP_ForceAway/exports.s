.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DFP_ForceAway_ctor
.dword DFP_ForceAway_dtor

# export table
/*0*/ .dword DFP_ForceAway_setup
/*1*/ .dword DFP_ForceAway_control
/*2*/ .dword DFP_ForceAway_update
/*3*/ .dword DFP_ForceAway_print
/*4*/ .dword DFP_ForceAway_free
/*5*/ .dword DFP_ForceAway_get_model_flags
/*6*/ .dword DFP_ForceAway_get_data_size
