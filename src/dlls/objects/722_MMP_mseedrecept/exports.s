.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword moonSeedReceptacle_ctor
.dword moonSeedReceptacle_dtor

# export table
/*0*/ .dword moonSeedReceptacle_setup
/*1*/ .dword moonSeedReceptacle_control
/*2*/ .dword moonSeedReceptacle_update
/*3*/ .dword dll_722_print
/*4*/ .dword moonSeedReceptacle_free
/*5*/ .dword moonSeedReceptacle_get_model_flags
/*6*/ .dword moonSeedReceptacle_get_data_size
/*7*/ .dword moonSeedReceptacle_func_BEC
/*8*/ .dword moonSeedReceptacle_func_D00
/*9*/ .dword moonSeedReceptacle_func_D18
/*10*/ .dword moonSeedReceptacle_func_D30
