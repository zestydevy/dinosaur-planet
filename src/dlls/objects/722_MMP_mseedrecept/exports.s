.option pic2
.section ".exports"
.global _exports
_exports:

# ctor/dtor
.dword mmp_mseedrecept_ctor
.dword mmp_mseedrecept_dtor

# export table
/*0*/ .dword mmp_mseedrecept_setup
/*1*/ .dword mmp_mseedrecept_control
/*2*/ .dword mmp_mseedrecept_update
/*3*/ .dword mmp_mseedrecept_print
/*4*/ .dword mmp_mseedrecept_free
/*5*/ .dword mmp_mseedrecept_get_model_flags
/*6*/ .dword mmp_mseedrecept_get_data_size
/*7*/ .dword mmp_mseedrecept_func_BEC
/*8*/ .dword mmp_mseedrecept_func_D00
/*9*/ .dword mmp_mseedrecept_func_D18
/*10*/ .dword mmp_mseedrecept_func_D30
