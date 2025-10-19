.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword DIM_BossSpit_ctor
.dword DIM_BossSpit_dtor

# export table
/*0*/ .dword DIM_BossSpit_setup
/*1*/ .dword DIM_BossSpit_control
/*2*/ .dword DIM_BossSpit_update
/*3*/ .dword DIM_BossSpit_print
/*4*/ .dword DIM_BossSpit_free
/*5*/ .dword DIM_BossSpit_get_model_flags
/*6*/ .dword DIM_BossSpit_get_data_size
