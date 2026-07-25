.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword MoonSeedReceptacle_ctor
.dword MoonSeedReceptacle_dtor

# export table
/*0*/ .dword MoonSeedReceptacle_setup
/*1*/ .dword MoonSeedReceptacle_control
/*2*/ .dword MoonSeedReceptacle_update
/*3*/ .dword MoonSeedReceptacle_print
/*4*/ .dword MoonSeedReceptacle_free
/*5*/ .dword MoonSeedReceptacle_get_model_flags
/*6*/ .dword MoonSeedReceptacle_get_data_size
/*7*/ .dword MoonSeedReceptacle_func_BEC
/*8*/ .dword MoonSeedReceptacle_func_D00
/*9*/ .dword MoonSeedReceptacle_func_D18
/*10*/ .dword MoonSeedReceptacle_func_D30
